/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  MonitorNode.cpp
  Description   :
  $Log:   X:/Archive/Sources/DIL_J1939/MonitorNode.cpv  $

  Rev 1.11   07 Jun 2011 11:11:16   CANMNTTM

  Rev 1.10   15 Apr 2011 18:48:38   CANMNTTM
  Added RBEI Copyright information.

  Rev 1.9   23 Mar 2011 14:58:04   CANMNTTM
  Support for PDU format 2 message

  Rev 1.8   02 Mar 2011 11:36:44   CANMNTTM
  SetCallBackFuncPtr function is added.

  Rev 1.7   30 Dec 2010 18:48:48   CANMNTTM

  Rev 1.6   29 Dec 2010 19:32:38   CANMNTTM
  Connection mode for both transmission and reception added.

  Rev 1.5   23 Dec 2010 16:52:20   CANMNTTM
  Macro MAX_MSG_LEN_J1939
  instead of MAX_DATA_LEN_J1939 wherever applicable.

  Rev 1.4   22 Dec 2010 19:23:42   CANMNTTM
  1. Implemented Call back mechanism.
  2. Exported function SetClBckFnPtrs and GetTimeOut added.

  Rev 1.3   14 Dec 2010 17:53:04   CANMNTTM
  Defined a virtual function vTransmitMessage.

  Rev 1.1   13 Dec 2010 16:37:06   CANMNTTM
  Nodes are made independent of channel.
  Now nodes can send message in any channel.

  Rev 1.0   06 Dec 2010 18:47:22   rac2kor

  Author(s)     :  Pradeep Kadoor
  Date Created  :  23/11/2010
  Modified By   :
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
  ******************************************************************************/
#include "DIL_J1939_stdafx.h"

#include "J1939_UtilityFuncs.h"
#include "MonitorNode.h"
#include "TransferLayer.h"
#include "NetworkMgmt.h"
#include "../../../BUSMASTER/Utility/MultiLanguageSupport.h"

CMonitorNode::CMonitorNode(int nNodeNo, char* pacNodeName,
                           UINT64 un64ECUName, BYTE byPrefAdres) :CNodeConManager(nNodeNo, pacNodeName, un64ECUName, byPrefAdres, TRUE)
{
    for (int i = 0; i < MAX_NODE_TO_MONITOR; i++)
    {
        m_pMonNodeConDetArr[i] = nullptr;
    }
}

CMonitorNode::~CMonitorNode(void)
{
    vRemoveAllConnections();
}

void CMonitorNode::vRemoveAllConnections()
{
    //Stop the read thread and then stop any proccessing of data
    //in det object and delete them. Delete the node manager
    //and set the value in array to nullptr
    for (int i = 0; i < MAX_NODE_TO_MONITOR; i++)
    {
        if (nullptr != m_pMonNodeConDetArr[i])
        {
            delete m_pMonNodeConDetArr[i];
            m_pMonNodeConDetArr[i] = nullptr;
        }
    }
}
BOOL CMonitorNode::bAddConDetObj(CConnectionDet* pConDet)
{
    BOOL bReturn = FALSE;
    if (m_byConCount < MAX_NODE_TO_MONITOR)
    {
        m_pMonNodeConDetArr[m_byConCount++] = pConDet;
        bReturn = TRUE;
    }
    return bReturn;
}

void CMonitorNode::vReadCANdataBuffer()
{
    static STCANDATA CurrMsgCAN;
    while (m_ouCANBuff.GetMsgCount() > 0)
    {
        //Now broadcast frames to each of the connection det obj
        //They should filter it out and work
        m_ouCANBuff.ReadFromBuffer(&CurrMsgCAN);
        //If the message is either RX or TX then only process it
        //If Error then notify user ****TBD****
        if ((CurrMsgCAN.m_ucDataType == RX_FLAG) ||
                (CurrMsgCAN.m_ucDataType == TX_FLAG))
        {
            //Monitor node should handle RX/TX messages but
            //Other nodes should handle only RX
            if (CurrMsgCAN.m_uDataInfo.m_sCANMsg.m_ucEXTENDED == 1)
            {
                vProcessCANMsgByMonNode(CurrMsgCAN);
            }
        }
    }
}
void static vGetSrcDestFromId(BYTE& bySrc, BYTE& byDest, UINT32 unExtId)
{
    UNION_29_BIT_ID uExtId = { 0 };
    uExtId.m_unExtID = unExtId;
    bySrc = uExtId.m_s29BitId.m_bySrcAddress;
    byDest = uExtId.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific;
}
void CMonitorNode::vProcessBroadCastDataByMonNode(const STCANDATA& CurrMsgCAN,
        CConnectionDet* pConDet)
{
    const STCAN_MSG& sCanMsg = CurrMsgCAN.m_uDataInfo.m_sCANMsg;
    BYTE byLastReceived = (pConDet->m_BCRxSeqVar) % MAX_SEQ_NO;
    if (byLastReceived == (sCanMsg.m_ucData[0] - 1))
    {
        pConDet->m_BCRxSeqVar++;
        UINT unDataIndex = (pConDet->m_BCRxSeqVar - 1) * MAX_TPDU_DATA_SIZE;
        if (pConDet->m_BCRxSeqVar == pConDet->m_BCTotalPackets)
        {
            for (UINT i = 0; i < pConDet->m_BCLastFrameLen; i++)
            {
                pConDet->m_BCRXLongData[unDataIndex + i] = sCanMsg.m_ucData[i + 1];
            }
            STJ1939_MSG sJ1939Msg;
            vCreateTempJ1939Msg(sJ1939Msg, CurrMsgCAN, pConDet->m_BCRXLongDataLen,
                                pConDet->m_BCRXLongData, MSG_TYPE_BROADCAST);

            sJ1939Msg.m_sMsgProperties.m_uExtendedID.m_s29BitId.vSetPGN(pConDet->m_BCPGN);
            WriteIntoClientsBuffer(sJ1939Msg);
            pConDet->m_BCRxSeqVar = 0;
            vCloseConnection(pConDet);
        }
        else
        {
            memcpy(&(pConDet->m_BCRXLongData[unDataIndex]), &(sCanMsg.m_ucData[1]),
                   MAX_TPDU_DATA_SIZE);
        }
    }
}
void CMonitorNode::vTransmitMessage(STJ1939_MSG* psMsg)
{
    switch (psMsg->m_sMsgProperties.m_eType)
    {
        case MSG_TYPE_BROADCAST:
        {
            UINT unPGN = psMsg->m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN();
            BYTE byDestAddress = psMsg->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific;

            if (psMsg->m_unDLC > MAX_FRAME_DATA_SIZE)
            {
                BYTE byPriority = psMsg->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPriority;
                vSendBAM(psMsg->m_unDLC, unPGN, byDestAddress, byPriority, psMsg->m_sMsgProperties.m_byChannel);
                vSendBroadCastData(psMsg->m_pbyData, psMsg->m_unDLC, byDestAddress, byPriority, psMsg->m_sMsgProperties.m_byChannel);
                EXECUTE_CLBCK_FN(CLBCK_FN_BC_LDATA_CONF,
                                 unPGN, byGetNodeAddress(),
                                 byDestAddress, TRUE);
            }
            else
            {
                vSendFrame((UCHAR)psMsg->m_unDLC, psMsg->m_pbyData,
                           psMsg->m_sMsgProperties.m_uExtendedID.m_unExtID, psMsg->m_sMsgProperties.m_byChannel);
                EXECUTE_CLBCK_FN(CLBCK_FN_BC_LDATA_CONF,
                                 unPGN, byGetNodeAddress(),
                                 byDestAddress, TRUE);
            }
        }
        break;
        case MSG_TYPE_DATA:
        {
            BYTE byDestAddress = psMsg->m_sMsgProperties.
                                 m_uExtendedID.m_s29BitId.
                                 m_uPGN.m_sPGN.m_byPDU_Specific;
            BYTE byPriorirty = psMsg->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPriority;
            UINT unPGN = psMsg->m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN();
            if (psMsg->m_unDLC > MAX_FRAME_DATA_SIZE)
            {
                HRESULT hResult = TransmitRequestToSend(byDestAddress, byPriorirty,
                                                        psMsg->m_unDLC, unPGN, psMsg->m_sMsgProperties.m_byChannel);
                TRACE("SENT REQUEST TO SEND\n");
                if ((hResult == DATA_CLEAR_2_SEND) || (hResult == DATA_DELAY_2_SEND))
                {
                    m_pConDet->vSetConStatus(T_CONNECTED);
                    m_pConDet->m_unPGN = unPGN;
                    if (SendLongMsg(psMsg, m_pConDet, hResult) == DATA_EOM)
                    {
                        psMsg->m_sMsgProperties.m_eDirection = DIR_TX;
                        EXECUTE_CLBCK_FN(CLBCK_FN_LDATA_CONF,
                                         unPGN, byGetNodeAddress(),
                                         byDestAddress, TRUE);
                    }
                    else
                    {
                        EXECUTE_CLBCK_FN(CLBCK_FN_LDATA_CONF,
                                         unPGN, byGetNodeAddress(),
                                         byDestAddress, FALSE);
                    }
                    TRACE("SENT LONG\n");
                }
                else
                {
                    EXECUTE_CLBCK_FN(CLBCK_FN_LDATA_CONF,
                                     unPGN, byGetNodeAddress(),
                                     byDestAddress, FALSE);
                }
                vCloseConnection(m_pConDet);
            }
            else
            {
                vSendFrame((UCHAR)psMsg->m_unDLC, psMsg->m_pbyData,
                           psMsg->m_sMsgProperties.m_uExtendedID.m_unExtID, psMsg->m_sMsgProperties.m_byChannel);
                EXECUTE_CLBCK_FN(CLBCK_FN_LDATA_CONF,
                                 unPGN, byGetNodeAddress(),
                                 byDestAddress, TRUE);
            }
        }
        break;
        case MSG_TYPE_NM_ACL:
        {
            vSendFrame((UCHAR)psMsg->m_unDLC, psMsg->m_pbyData,
                       psMsg->m_sMsgProperties.m_uExtendedID.m_unExtID, psMsg->m_sMsgProperties.m_byChannel);
            Sleep(CNetworkMgmt::sg_unTO_RESPONSE);
        }
        break;
        case MSG_TYPE_GROUP_FUNCTIONS:
        {
            //TBD
        }
        break;
    }
}
void CMonitorNode::vProcessLongDataByMonNode(const STCANDATA& CurrMsgCAN,
        CConnectionDet* pConDet)
{
    const STCAN_MSG& sCanMsg = CurrMsgCAN.m_uDataInfo.m_sCANMsg;
    BYTE byLastReceived = (pConDet->m_byRxSeqNo) % MAX_SEQ_NO;
    if (byLastReceived == (sCanMsg.m_ucData[0] - 1))
    {
        //First increment the seq counter
        pConDet->m_byRxSeqNo++;

        pConDet->m_byCurrPacket = (pConDet->m_byCurrPacket + 1) % pConDet->m_byMaxPacketWOC2S;

        UINT unDataIndex = (pConDet->m_byRxSeqNo - 1) * MAX_TPDU_DATA_SIZE;
        //If it is the last packet of the whole msg send EOM msg come out of wait
        if (pConDet->m_byRxSeqNo == pConDet->m_unRxTotalPackets)
        {
            for (UINT i = 0; i < pConDet->m_unRxLastFrameLen; i++)
            {
                pConDet->m_RxLongData[unDataIndex + i] = sCanMsg.m_ucData[i + 1];
            }
            STJ1939_MSG sJ1939Msg;
            vCreateTempJ1939Msg(sJ1939Msg, CurrMsgCAN, pConDet->m_unRXLongDataLen,
                                pConDet->m_RxLongData, MSG_TYPE_DATA);
            sJ1939Msg.m_sMsgProperties.m_uExtendedID.m_s29BitId.vSetPGN(pConDet->m_unPGN);
            WriteIntoClientsBuffer(sJ1939Msg);
            vCloseConnection(pConDet);
        }
        else
        {
            memcpy(&(pConDet->m_RxLongData[unDataIndex]), &(sCanMsg.m_ucData[1]),
                   MAX_TPDU_DATA_SIZE);
        }
    }
}
BOOL CMonitorNode::bProcessConLevelMsgByMon(const STCANDATA& CurrMsgCAN)
{
    BOOL bIsProcessed = TRUE;
    STCAN_MSG sCanMsg = CurrMsgCAN.m_uDataInfo.m_sCANMsg;
    BYTE bySrc = 254, byDest = 254;
    vGetSrcDestFromId(bySrc, byDest, sCanMsg.m_unMsgID);
    CConnectionDet* pConDet = pGetConDet(bySrc, byDest);
    if (bIsEOM_ACK(sCanMsg.m_unMsgID, sCanMsg.m_ucData[0]))
    {
        pConDet = pGetConDet(byDest, bySrc);
        if (pConDet != nullptr)
        {
            pConDet->m_byResult = DATA_EOM;
            SetEvent(pConDet->m_hDataAckWait);
            vCloseConnection(pConDet);
        }
    }
    else if (bIsConAbortMsg(sCanMsg.m_unMsgID, sCanMsg.m_ucData[0]))
    {
        pConDet = pGetConDet(byDest, bySrc);
        if (pConDet != nullptr)
        {
            pConDet->m_byResult = DATA_CON_ABORT;
            SetEvent(pConDet->m_hDataAckWait);
            vCloseConnection(pConDet);
        }
    }
    else if (bIsConAckMsg(sCanMsg))
    {
        pConDet = pGetConDet(byDest, bySrc);
        if (pConDet != nullptr)
        {
            pConDet->m_byResult = DATA_CLEAR_2_SEND;
            if (sCanMsg.m_ucData[1] == 0)
            {
                pConDet->m_byResult = DATA_DELAY_2_SEND;
            }
            SetEvent(m_pConDet->m_hDataAckWait);
            pConDet->vSetConStatus(T_CONNECTED);
            pConDet->m_byMaxPacketWOC2S = sCanMsg.m_ucData[1];
            ASSERT(sCanMsg.m_ucData[2] == 0);
            UINT32 unPGN;
            unPGN = (UINT32)sCanMsg.m_ucData[5];
            unPGN |= ((UINT32)sCanMsg.m_ucData[6]) << 8;
            unPGN |= ((UINT32)sCanMsg.m_ucData[7]) << 16;
            ASSERT(pConDet->m_unPGN == unPGN);
        }
    }
    else if (bIsDataAckMsg(sCanMsg))
    {
        pConDet = pGetConDet(byDest, bySrc);
        if (pConDet != nullptr)
        {
            pConDet->m_byResult = DATA_CLEAR_2_SEND;
            if (sCanMsg.m_ucData[1] == 0)
            {
                pConDet->m_byResult = DATA_DELAY_2_SEND;
            }
            pConDet->m_byTxAckSeqNo = sCanMsg.m_ucData[2];
            SetEvent(pConDet->m_hDataAckWait);

            pConDet->vSetConStatus(T_CONNECTED);
            pConDet->m_byMaxPacketWOC2S = sCanMsg.m_ucData[1];
            pConDet->m_byTxAckSeqNo = sCanMsg.m_ucData[2];
            UINT32 unPGN;
            unPGN = (UINT32)sCanMsg.m_ucData[5];
            unPGN |= ((UINT32)sCanMsg.m_ucData[6]) << 8;
            unPGN |= ((UINT32)sCanMsg.m_ucData[7]) << 16;
            ASSERT(pConDet->m_unPGN == unPGN);
        }
    }
    else if (bIsConReqMsg(sCanMsg.m_unMsgID, sCanMsg.m_ucData[0]))
    {
        if (pConDet == nullptr)
        {
            pConDet = pAddConDet(sCanMsg.m_unMsgID);
        }
        if (pConDet != nullptr)
        {
            if (pConDet->eGetConStatus() == T_DISCONNECTED)
            {
                pConDet->m_eRxConMode = CM_STANDARD;
                pConDet->m_byRxSeqNo = 0;
                WORD* pwLen = (WORD*)&(sCanMsg.m_ucData[1]);
                pConDet->m_unRXLongDataLen = *pwLen;

                /* Validate the maximum allowed size for a J1939 message data */
                if (*pwLen > MAX_DATA_LEN_J1939)
                {
                    /* If maximum size exceeds, RTS frame bytes 1 and 2 might have
                       been sent in reverse order, so try to change the byte order */
                    pConDet->m_unRXLongDataLen = (UINT32)sCanMsg.m_ucData[2];
                    pConDet->m_unRXLongDataLen |= ((UINT32)sCanMsg.m_ucData[1]) << 8;
                    if (pConDet->m_unRXLongDataLen > MAX_DATA_LEN_J1939)
                    {
                        bIsProcessed = FALSE;
                    }
                }
                else
                {
                    pConDet->m_unRXLongDataLen = *pwLen;
                }

                pConDet->m_unRxLastFrameLen = (UINT)byGetLastFrameLen(pConDet->m_unRXLongDataLen);
                pConDet->m_unRxTotalPackets = sCanMsg.m_ucData[3];
                pConDet->m_byMaxPacketWOC2S = sCanMsg.m_ucData[4];
                pConDet->m_unPGN = (UINT32)sCanMsg.m_ucData[5];
                pConDet->m_unPGN |= ((UINT32)sCanMsg.m_ucData[6]) << 8;
                pConDet->m_unPGN |= ((UINT32)sCanMsg.m_ucData[7]) << 16;
            }
        }
    }
    else if (bIsBAM(sCanMsg.m_unMsgID, sCanMsg.m_ucData[0]))    /* BROADCAST Announce*/
    {
        if (pConDet == nullptr)
        {
            pConDet = pAddConDet(sCanMsg.m_unMsgID);
        }
        if (pConDet != nullptr)
        {
            if (pConDet->eGetConStatus() == T_DISCONNECTED)
            {
                pConDet->m_eRxConMode = CM_BROADCAST;
                pConDet->vSetConStatus(T_CONNECTED);
                pConDet->m_BCRxSeqVar = 0;
                WORD* pwLen = (WORD*)&(sCanMsg.m_ucData[1]);
                pConDet->m_BCRXLongDataLen = *pwLen;
                pConDet->m_BCLastFrameLen = (UINT)byGetLastFrameLen(pConDet->m_BCRXLongDataLen);
                pConDet->m_BCTotalPackets = sCanMsg.m_ucData[3];
                pConDet->m_BCPGN = (UINT32)sCanMsg.m_ucData[5];
                pConDet->m_BCPGN |= ((UINT32)sCanMsg.m_ucData[6]) << 8;
                pConDet->m_BCPGN |= ((UINT32)sCanMsg.m_ucData[7]) << 16;
            }
        }
    }
    else if (bIsTPDT(sCanMsg.m_unMsgID))
    {
        if (pConDet != nullptr)
        {
            if (pConDet->m_eRxConMode == CM_BROADCAST)
            {
                vProcessBroadCastDataByMonNode(CurrMsgCAN, pConDet);
            }
            else if (pConDet->m_eRxConMode == CM_STANDARD)
            {
                vProcessLongDataByMonNode(CurrMsgCAN, pConDet);
            }
        }
    }
    else
    {
        bIsProcessed = FALSE;
    }
    return bIsProcessed;
}
void CMonitorNode::vProcessCANMsgByMonNode(const STCANDATA CurrMsgCAN)
{
    bProcessNodeLevelMsgByMonNode(CurrMsgCAN);
    if (bIsConLevelMsg(CurrMsgCAN.m_uDataInfo.m_sCANMsg.m_unMsgID))
    {
        bProcessConLevelMsgByMon(CurrMsgCAN);
    }
}
BOOL CMonitorNode::bProcessNodeLevelMsgByMonNode(const STCANDATA& sCanData)
{
    //Send it to each connection
    BOOL bResult = TRUE;
    STJ1939_MSG sJ1939Msg;
    EJ1939_MSG_TYPE eType = eGetMsgType(sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID,
                                        (BYTE*)sCanData.m_uDataInfo.m_sCANMsg.m_ucData);
    vCreateTempJ1939Msg(sJ1939Msg, sCanData, sCanData.m_uDataInfo.m_sCANMsg.m_ucDataLen,
                        (BYTE*)sCanData.m_uDataInfo.m_sCANMsg.m_ucData, eType);
    WriteIntoClientsBuffer(sJ1939Msg);
    return bResult;
}
CConnectionDet* CMonitorNode::pouCreateAndAddConnnection(UINT unId)
{
    BYTE bySrc = 254, byDest = 254;
    vGetSrcDestFromId(bySrc, byDest, unId);
    CConnectionDet* pConDet = new CConnectionDet(bySrc, byDest);
    if (nullptr != pConDet)
    {
        bAddConDetObj(pConDet);
    }
    return pConDet;
}

CConnectionDet* CMonitorNode::pGetConDet(BYTE bySrc, BYTE byDest)
{
    CConnectionDet* pConDet = nullptr;
    if ((m_pConDet->m_bySrcAddress == bySrc)
            && (m_pConDet->m_byDestAddress == byDest))
    {
        pConDet = m_pConDet;
    }
    else
    {
        for (int i = 0; (i < m_byConCount) && (pConDet == nullptr); i++)
        {
            if (nullptr != m_pMonNodeConDetArr[i])
            {
                /* Create a new connection if source address is new */
                if (bySrc == m_pMonNodeConDetArr[i]->m_bySrcAddress)
                {
                    pConDet = m_pMonNodeConDetArr[i];
                }
            }
        }
    }
    return pConDet;
}

CConnectionDet* CMonitorNode::pAddConDet(UINT unID)
{
    BYTE bySrc = 254, byDest = 254;
    vGetSrcDestFromId(bySrc, byDest, unID);
    CConnectionDet* pConDet = pGetConDet(bySrc, byDest);
    if (nullptr == pConDet)
    {
        pConDet = pouCreateAndAddConnnection(unID);
    }
    return pConDet;
}
