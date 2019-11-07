/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  LogObjectJ1939.cpp
  Description   :  Source file for CLogObjectJ1939 class.
  Author(s)     :  Ratnadip Choudhury
  Date Created  :  30.11.2010
  Modified By   :
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/

#include "FrameProcessor_stdafx.h"
#include "LogObjectJ1939.h"            // For CLogObjectJ1939 class declaration


#define J1939_LOG_COLUMNS       "***<Time><Channel><CAN ID><PGN><Type><Source Node><Destination Node><Priority><Tx/Rx><DLC><DataBytes>***"



CLogObjectJ1939::CLogObjectJ1939(CString omVersion):CBaseLogObject(omVersion)
{
    // Initialise the filtering block
    m_sFilterApplied.vClear();
    m_pasControllerDetails = nullptr;
    m_nNumChannels = 0;
}

CLogObjectJ1939::~CLogObjectJ1939()
{
    if (nullptr != m_pasControllerDetails)
    {
        delete[] m_pasControllerDetails;
        m_pasControllerDetails = nullptr;
    }
}

void CLogObjectJ1939::Der_CopySpecificData(const CBaseLogObject* pouLogObjRef)
{
    const CLogObjectJ1939* pouLobObjCANRef = static_cast <const CLogObjectJ1939*> (pouLogObjRef);
    pouLobObjCANRef->GetFilterInfo(m_sFilterApplied);
}

bool CLogObjectJ1939::bLogData(const SFORMATTEDATA_J1939& sDataJ1939)
{
    // Multiple return statements are used to keep the code precise.

    /* Get Direction */
    EDIRECTION eDirMsg;
    if ( strcmp(sDataJ1939.m_acMsgDir, "Tx") == 0 )
    {
        eDirMsg = DIR_TX;
    }
    else
    {
        eDirMsg = DIR_RX;
    }

    /* Get PGN ID */
    UNION_29_BIT_ID sPGNId;
    UINT32 unPGNId;
    sPGNId.m_unExtID = sDataJ1939.m_dwMsgID;
    unPGNId = sPGNId.m_s29BitId.unGetPGN();

    SFRAMEINFO_BASIC_J1939 J1939Info_Basic =
    {
        unPGNId,
        _atoi64(sDataJ1939.m_acChannel), //KSS
        eDirMsg
    };

    // Assign appropriate values to FrameInfo_Basic

    if (bToBeLogged(J1939Info_Basic) == false)
    {
        return false;
    }

    CString omLogText = "";
    char* pTimeData = nullptr;
    char acID[16] = {'\0'};
    char* pPGN = nullptr;
    char* pData = nullptr;
    char* psSrcNode = nullptr;
    char* psDestNode = nullptr;

    switch (m_sLogInfo.m_eLogTimerMode) // Time Mode
    {
        case TIME_MODE_ABSOLUTE:
        {
            if(m_sLogInfo.m_bResetAbsTimeStamp)
            {
                pTimeData = (char*) (sDataJ1939.m_acTimeAbsReset);
            }
            else
            {
                pTimeData = (char*) (sDataJ1939.m_acTimeAbs);
            }
        }
        break;
        case TIME_MODE_RELATIVE:
        {
            pTimeData = (char*) (sDataJ1939.m_acTimeRel);
        }
        break;
        case TIME_MODE_SYSTEM:
        {
            pTimeData = (char*) (sDataJ1939.m_acTimeSys);
        }
        break;

        default:
            ASSERT(false);
            break;
    }

    switch (m_sLogInfo.m_eNumFormat)
    {
        case HEXADECIMAL:
        {
            sprintf_s(acID, "%x", sDataJ1939.m_dwMsgID);
            pPGN = (char*) (sDataJ1939.m_acPGNHex);
            pData = (char*) (sDataJ1939.m_pcDataHex);
            psSrcNode = (char*) (sDataJ1939.m_acSrcHex);
            psDestNode = (char*) (sDataJ1939.m_acDestHex);
        }
        break;
        case DEC:
        {
            sprintf_s(acID, "%d", sDataJ1939.m_dwMsgID);
            pPGN = (char*) (sDataJ1939.m_acPGNDec);
            pData = (char*) (sDataJ1939.m_pcDataDec);
            psSrcNode = (char*) (sDataJ1939.m_acSrcDec);
            psDestNode = (char*) (sDataJ1939.m_acDestDec);
        }
        break;

        default:
            ASSERT(false);
            break;
    }

    // First put everything in a string to get the length
    // <Time> <Channel> <ID> <PGN> <Type> <Src Node> <Dest Node> <Priority> <Direction> <DLC> <Data>
    omLogText.Format(  "%s %s %s %s %s %s %s %s %s %s %s\n",
                       pTimeData,
                       sDataJ1939.m_acChannel,
                       acID,
                       pPGN,
                       sDataJ1939.m_acMsgType,
                       psSrcNode,
                       psDestNode,
                       sDataJ1939.m_acPriority,
                       sDataJ1939.m_acMsgDir,
                       sDataJ1939.m_acDataLen,
                       pData);

    vWriteTextToFile(omLogText, J1939);

    return true;
}

// To format the header
void CLogObjectJ1939::vFormatHeader(CString& omHeader, ETYPE_BUS /* eBus */)
{
    CBaseLogObject::vFormatHeader(omHeader, J1939);
    omHeader += J1939_LOG_COLUMNS;
    omHeader += L'\n';
}

// To format the footer
void CLogObjectJ1939::vFormatFooter(CString& omFooter)
{
    CBaseLogObject::vFormatFooter(omFooter);
}

/*******************************************************************************
//  Function Name  : bLogData
//  Input(s)       : none
//  Output         : none
//  Description    : Logs data. This will open the file in
//                   appropriate mode and will insert header if it is new.
//  Member of      : CBaseLogObject
//  Friend of      : None
//  Author         : Arun Kumar
//  Creation Date  : 10/11/06
//  Modifications  : Anish, the message structure is changed
//  Modifications  : Ratnadip Choudhury.
                     1. Changed the prototype
                     2. Shifted the formatting codes into vWriteTextToFile(...)
*******************************************************************************/
bool CLogObjectJ1939::bToBeLogged(SFRAMEINFO_BASIC_J1939& J1939Info_Basic)
{
    // Multiple return statements are used to keep the code precise.

    if (m_sLogInfo.m_bEnabled == false)
    {
        return false;
    }

    if (nullptr == m_pLogFile)
    {
        ASSERT(false);
        return false;
    }

    if (CAN_CHANNEL_ALL != m_sLogInfo.m_ChannelSelected)
    {
        if (m_sLogInfo.m_ChannelSelected != J1939Info_Basic.m_eChannel)
        {
            return false;
        }
    }
    if (m_sFilterApplied.m_bEnabled)
    {
        if (m_sFilterApplied.bToBeBlocked(J1939Info_Basic) == TRUE)
        {
            return false;
        }
    }

    // Check for the triggering conditions
    switch (m_CurrTriggerType)
    {
        case NONE:
            break;

        case STOPPED:
        {
            //If the log file is stopped then don't log
            return false;
        }
        break;
        case START:
        {
            if ((m_sLogInfo.m_sLogTrigger.m_unStartID == J1939Info_Basic.m_dwPGN)
                    && (J1939Info_Basic.m_eDrctn  == DIR_RX))
            {
                m_CurrTriggerType = NONE;
            }
            else
            {
                return false;
            }
        }
        break;
        case STOP:
        {
            if ((m_sLogInfo.m_sLogTrigger.m_unStopID == J1939Info_Basic.m_dwPGN)
                    && (J1939Info_Basic.m_eDrctn  == DIR_RX))
            {
                m_CurrTriggerType = STOPPED;
            }
        }
        break;

        case BOTH:
        {
            if ((m_sLogInfo.m_sLogTrigger.m_unStartID == J1939Info_Basic.m_dwPGN)
                    && (J1939Info_Basic.m_eDrctn  == DIR_RX))
            {
                m_CurrTriggerType = STOP;
            }
            else
            {
                return false;
            }
        }
        break;

        default:
            ASSERT(false);
            break;
    }

    return true;
}

BYTE* CLogObjectJ1939::Der_SetConfigData(BYTE* pvDataStream)
{
    bool bResult = false;
    BYTE* pbSStream = pvDataStream;

    //pbSStream = m_sFilterApplied.pbSetConfigData(pbSStream, bResult);

    return pbSStream;
}

BYTE* CLogObjectJ1939::Der_GetConfigData(BYTE* pvDataStream) const
{
    BYTE* pbTStream = pvDataStream;

    //pbTStream = m_sFilterApplied.pbGetConfigData(pbTStream);

    return pbTStream;
}

void CLogObjectJ1939::Der_GetConfigData(xmlNodePtr pNodePtr) const
{
    m_sFilterApplied.pbGetConfigFilterData(pNodePtr);
}

UINT CLogObjectJ1939::Der_unGetBufSize(void) const
{
    return 0;
}

void CLogObjectJ1939::EnableFilter(bool bEnable)
{
    m_sFilterApplied.m_bEnabled = bEnable;
}

void CLogObjectJ1939::GetFilterInfo(SFILTERAPPLIED_J1939& sFilterInfo) const
{
    sFilterInfo.bClone(m_sFilterApplied);
}

void CLogObjectJ1939::SetFilterInfo(const SFILTERAPPLIED_J1939& sFilterInfo)
{
    m_sFilterApplied.bClone(sFilterInfo);
}

void CLogObjectJ1939::Der_SetDatabaseFiles(const CStringArray& omList)
{
    // Clear before updating
    m_omListDBFiles.RemoveAll();

    for (int nIdx = 0; nIdx < omList.GetSize(); nIdx++)
    {
        m_omListDBFiles.Add(omList.GetAt(nIdx));
    }
}

// Get the list of database files associated
void CLogObjectJ1939::Der_GetDatabaseFiles(CStringArray& omList)
{
    omList.Append(m_omListDBFiles);
}

void CLogObjectJ1939::Der_SetChannelBaudRateDetails
(void* controllerDetails,
 int nNumChannels)
{
    SCONTROLLER_DETAILS* pTempControllerDetails=(SCONTROLLER_DETAILS*)controllerDetails;

    if (nullptr != m_pasControllerDetails)
    {
        delete[] m_pasControllerDetails;
    }
    m_pasControllerDetails = nullptr;

    m_pasControllerDetails = new SCONTROLLER_DETAILS [nNumChannels];
    for (int nIdx = 0; nIdx < nNumChannels; nIdx++)
    {
        m_pasControllerDetails[nIdx] = pTempControllerDetails[nIdx];
        //memcpy(m_pasControllerDetails + nIdx, controllerDetails + nIdx, sizeof(SCONTROLLER_DETAILS));
    }
    m_nNumChannels = nNumChannels;
}

// To get the channel baud rate info for each channel
void CLogObjectJ1939::Der_GetChannelBaudRateDetails
(void* controllerDetails, int& nNumChannels)
{
    SCONTROLLER_DETAILS* pTempControllerDetails=(SCONTROLLER_DETAILS*)controllerDetails;

    if (nullptr != m_pasControllerDetails && nullptr != pTempControllerDetails )
    {
        for (int nIdx = 0; nIdx < m_nNumChannels; nIdx++)
        {
            pTempControllerDetails[nIdx] = m_pasControllerDetails[nIdx];
            //memcpy(controllerDetails + nIdx, m_pasControllerDetails + nIdx, sizeof(SCONTROLLER_DETAILS));
        }
        nNumChannels = m_nNumChannels;
    }
}
