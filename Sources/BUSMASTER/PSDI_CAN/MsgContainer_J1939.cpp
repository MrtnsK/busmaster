/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  MsgContainer_J1939.cpp
  Description   :
  $Log:   X:/Archive/Sources/PSDI_CAN/MsgContainer_J1939.cpv  $

      Rev 1.4   06 Jun 2011 11:57:32   CANMNTTM


      Rev 1.3   15 Apr 2011 19:28:10   CANMNTTM
   Added RBEI Copyright information.

  Author(s)     :  Pradeep Kadoor
  Date Created  :  15/04/2011
  Modified By   :
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/
#include "PSDI_CAN/stdafx_CAN.h"
#include "include/Utils_Macro.h"
#include "error.h"
#include "Include/BaseDefs.h"
//#include "Include/Can_Error_defs.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
//#include "DataTypes/MsgBufAll_Datatypes.h"
//#include "DIL_Datatypes.h"
#include "MsgContainer_J1939.h"
#include "Utility\MultiLanguageSupport.h"
//#include "../Application/GettextBusmaster.h"
#include "../Application/HashDefines.h"

const int nBitsIn4Bytes          = 32;
const int nBitsIn5Bytes          = 40;
const int nBitsIn6Bytes          = 48;
const int nBitsIn7Bytes          = 56;

//const int TX_MESSAGE = 0x20000000;  // bitwise OR to make it a Tx message
//const int RX_MESSAGE = 0xdfffffff;  // bitwise AND to make it a Rx message

#define MAKE_RTR_MESSAGE_TYPE(MSGID)         (MSGID | 0x80000000)
#define MAKE_EXTENDED_MESSAGE_TYPE(MSGID)    (MSGID | 0x40000000)
#define MAKE_STANDARD_MESSAGE_TYPE(MSGID)    (MSGID & 0xBfffffff)
#define MAKE_DEFAULT_MESSAGE_TYPE(MSGID)     (MSGID & 0x3fffffff)
#define MAKE_RX_TX_MESSAGE(MSGID, MSGTYPE)   ((MSGTYPE) ? (MSGID) : ((MSGID) | TX_MESSAGE))
#define MAKE_CHANNEL_SPECIFIC_MESSAGE(MSGID, CHANNELID) ( ((unsigned __int64)((UINT)(MSGID))) | (((__int64)(CHANNELID)) << nBitsIn4Bytes) )
#define MAKE_SOURCE_SPECIFIC_MESSAGE(MSGID, SOURCE) ( ((unsigned __int64)((MSGID))) | (((__int64)(SOURCE)) << nBitsIn5Bytes) )
#define MAKE_DEST_SPECIFIC_MESSAGE(MSGID, DEST) ( ((unsigned __int64)((MSGID))) | (((__int64)(DEST)) << nBitsIn6Bytes) )
#define MAKE_TYPE_SPECIFIC_MESSAGE(MSGID, TYPE) ( ((unsigned __int64)((MSGID))) | (((__int64)(TYPE)) << nBitsIn7Bytes) )

/******************************************************************************
    Function Name    :  CMsgContainerJ1939
    Input(s)         :
    Output           :
    Functionality    :  Constructor
    Member of        :  CMsgContainerJ1939
    Friend of        :      -
    Author(s)        :  Arun kumar K
    Date Created     :  20.01.2011
******************************************************************************/
CMsgContainerJ1939::CMsgContainerJ1939(void)
{
    InitializeCriticalSection(&m_sCritSecDataSync);
    InitializeCriticalSection(&m_omCritSecFilter);
    m_pouDIL_J1939 = nullptr;
    // Allocate necessary amount of memory.
    m_sJ1939Data.m_unDLC = MAX_DATA_LEN_J1939;
    m_sJ1939Data.m_pbyData = new BYTE[m_sJ1939Data.m_unDLC];// For basic data object
    ASSERT(nullptr != m_sJ1939Data.m_pbyData);

    // For raw data bytes. It should be equal to the size of m_sJ1939Data
    m_pbyJ1939Data = new BYTE[MAX_MSG_LEN_J1939 +  sizeof(__int64)];
    ASSERT(nullptr != m_pbyJ1939Data);
    USHORT Length = ushCalculateStrLen(true, MAX_DATA_LEN_J1939);
    m_sOutFormattedData.m_pcDataHex = new char[Length];
    ASSERT(nullptr != m_sOutFormattedData.m_pcDataHex);
    memset(m_sOutFormattedData.m_pcDataHex, '\0', Length * sizeof(char));
    Length = ushCalculateStrLen(false, MAX_DATA_LEN_J1939);
    m_sOutFormattedData.m_pcDataDec = new char[Length];
    ASSERT(nullptr != m_sOutFormattedData.m_pcDataDec);
    memset(m_sOutFormattedData.m_pcDataDec, '\0', Length * sizeof(char));
}

/******************************************************************************
    Function Name    :  ~CMsgContainerJ1939
    Input(s)         :
    Output           :
    Functionality    :  Destructor
    Member of        :  CMsgContainerJ1939
    Friend of        :      -
    Author(s)        :  Arun kumar K
    Date Created     :  20.01.2011
******************************************************************************/
CMsgContainerJ1939::~CMsgContainerJ1939(void)
{
    DeleteCriticalSection(&m_sCritSecDataSync);
    DeleteCriticalSection(&m_omCritSecFilter);
    //m_sDataCopyThread.bTerminateThread();
    DELETE_ARRAY(m_pbyJ1939Data);
    DELETE_ARRAY(m_sJ1939Data.m_pbyData);
    DELETE_ARRAY(m_sOutFormattedData.m_pcDataDec);
    DELETE_ARRAY(m_sOutFormattedData.m_pcDataHex);
}

/******************************************************************************
    Function Name    :  InitTimeParams
    Input(s)         :
    Output           :
    Functionality    :  Initialize the refrence time
    Member of        :  CMsgContainerJ1939
    Friend of        :      -
    Author(s)        :  Arun kumar K
    Date Created     :  04.02.2011
******************************************************************************/
USHORT CMsgContainerJ1939::ushCalculateStrLen(bool bForHex, USHORT ushLength)
{
    USHORT Result = 0;

    if (bForHex)
    {
        Result = ushLength * 3 + 1;
    }
    else
    {
        Result = ushLength * 4 + 1;
    }

    return Result;
}

/******************************************************************************
    Function Name    :  InitTimeParams
    Input(s)         :
    Output           :
    Functionality    :  Initialize the refrence time
    Member of        :  CMsgContainerJ1939
    Friend of        :      -
    Author(s)        :  Arun kumar K
    Date Created     :  20.01.2011
******************************************************************************/
void CMsgContainerJ1939::InitTimeParams(void)
{
    SYSTEMTIME CurrSysTime;
    UINT64 unAbsTime;
    if (nullptr != m_pouDIL_J1939)
    {
        m_pouDIL_J1939->DILIJ_GetTimeModeMapping(CurrSysTime, unAbsTime);
        m_ouFormatJ1939.vSetTimeParams(CurrSysTime, unAbsTime);
    }
}

/******************************************************************************
    Function Name    :  bTobeBlocked
    Input(s)         :
    Output           :
    Functionality    :  Check for the block status of J1939 message
    Member of        :  CMsgContainerJ1939
    Friend of        :      -
    Author(s)        :  Arun kumar K
    Date Created     :  01.02.2011
******************************************************************************/
BOOL CMsgContainerJ1939::bTobeBlocked(STJ1939_MSG& sJ1939Data)
{
    static SFRAMEINFO_BASIC_J1939 sBasicJ1939Info;
    sBasicJ1939Info.m_dwPGN = sJ1939Data.m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN();

    EnterCriticalSection(&m_omCritSecFilter);
    BOOL bBlock = m_sFilterJ1939.bToBeBlocked(sBasicJ1939Info);
    LeaveCriticalSection(&m_omCritSecFilter);
    return bBlock;
}

/******************************************************************************
    Function Name    :  vRetrieveDataFromBuffer
    Input(s)         :
    Output           :
    Functionality    :  Read data from DIL buffer
    Member of        :  CMsgContainerJ1939
    Friend of        :      -
    Author(s)        :  Arun kumar K
    Date Created     :  20.01.2011
******************************************************************************/
void CMsgContainerJ1939::vRetrieveDataFromBuffer()
{
    EnterCriticalSection(&m_sCritSecDataSync);
    static INT nType = 0;
    static HRESULT Result = 0;
    while (m_ouVSEBufJ1939.GetMsgCount() > 0)
    {
        STJ1939_MSG sJ1939Msg;
        INT nSize = MAX_MSG_LEN_J1939;
        // First read the J1939 message
        Result = m_ouVSEBufJ1939.ReadFromBuffer(nType, m_pbyJ1939Data, nSize);

        if (Result == ERR_READ_MEMORY_SHORT)
        {
            CString omBuf;
            omBuf.Format(_("J1939 PSDI - ERR_READ_MEMORY_SHORT  nSize: %d"), nSize);
            ASSERT(!AfxMessageBox(omBuf));
        }
        else if (Result == EMPTY_APP_BUFFER)
        {
            CString omBuf;
            omBuf.Format(_("J1939 PSDI - EMPTY_APP_BUFFER"));
            ASSERT(!AfxMessageBox(omBuf));
        }
        if (Result == CALL_SUCCESS)
        {
            // Save it into the J1939 message structure
            m_sJ1939Data.vSetDataStream(m_pbyJ1939Data);

            vProcessNewData(m_sJ1939Data);
        }
    }
    LeaveCriticalSection(&m_sCritSecDataSync);
}

static bool bIsErrorMsg(EJ1939_MSG_TYPE eType)
{
    if(eType == MSG_TYPE_NONE)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int stJ1939MsgSpl::nGetSize() const
{
    return sizeof(m_nDeltime) + STJ1939_MSG::unGetSize();
}

void stJ1939MsgSpl::vGetDataStream(BYTE* pbyData) const
{
    STJ1939_MSG::vGetDataStream(pbyData);
    pbyData += STJ1939_MSG::unGetSize();
    COPY_DATA(pbyData, &m_nDeltime, sizeof(m_nDeltime));
}

/******************************************************************************
    Function Name    :  vProcessNewData
    Input(s)         :  sJ1939Msg
    Output           :
    Functionality    :  Process a new Rx/Tx msg
    Member of        :  CMsgContainerJ1939
    Friend of        :      -
    Author(s)        :  Arun kumar K
    Date Created     :  31.01.2010
******************************************************************************/
void CMsgContainerJ1939::vProcessNewData(STJ1939_MSG& sJ1939Msg)
{

    // append : 1. Add item, get count, SetItemCount.
    // OW     : 1. Get ID, Create map index, Search if it is present in map
    //             if present change the offset and time stamp
    //          2. if not present and (get count < max count) save the Buffer index,
    //             ListCtrl index, time stamp, make offest = time stamp in disp entry
    //             and add to the map and array
    //          3. if not present and (get count = max count), do nothing
    //          4. SetItemcount

    if ( !bIsErrorMsg(sJ1939Msg.m_sMsgProperties.m_eType)  )
    {

        // Add to append buffer
        // If its the very first entry, the time stamp must
        if (m_sJ1939ReadMsgSpl.m_sMsgProperties.m_un64TimeStamp != 0) // be 0 and will
        {
            // retain such value.
            m_sJ1939ReadMsgSpl.m_nDeltime = _abs64( sJ1939Msg.m_sMsgProperties.m_un64TimeStamp -
                                                    m_sJ1939ReadMsgSpl.m_sMsgProperties.m_un64TimeStamp);
        }
        static BYTE arrBuf[MAX_MSG_LEN_J1939 + sizeof(__int64)];

        if (!bTobeBlocked(sJ1939Msg))
        {
            sJ1939Msg.vGetDataStream(arrBuf);
            m_sJ1939ReadMsgSpl.vSetDataStream(arrBuf);
            m_sJ1939ReadMsgSpl.vGetDataStream(arrBuf);
            m_ouAppendJ1939Buf.WriteIntoBuffer(J1939, arrBuf/*(BYTE*)&m_sJ1939ReadMsgSpl*/, m_sJ1939ReadMsgSpl.nGetSize());

            if (nullptr != m_pRxMsgCallBack)
            {
                m_pRxMsgCallBack->onRxMsg((void*)&sJ1939Msg);
            }
        }
    }
    //else //Add the error messages
    //{
    //    vProcessCurrErrorEntry(sCanData.m_uDataInfo.m_sErrInfo);
    //    // Add to append buffer
    //    // If its the very first entry, the time stamp must
    //    if (m_sCANReadDataSpl.m_lTickCount.QuadPart != 0) // be 0 and will
    //    {                                                     // retain such value.
    //        m_sCANReadDataSpl.m_nDeltime = sCanData.m_lTickCount.QuadPart -
    //                                       m_sCANReadDataSpl.m_lTickCount.QuadPart;
    //    }
    //    STCANDATA *pStcan = &m_sCANReadDataSpl;
    //    *pStcan = sCanData;
    //    m_ouAppendCanBuf.WriteIntoBuffer(&m_sCANReadDataSpl);

    //    if (nullptr != m_pRxMsgCallBack)
    //    {
    //        m_pRxMsgCallBack((void*)&sCanData, CAN);
    //    }
    //}
}

//****************Exported functions*******************************

/******************************************************************************
    Function Name    :  vInit
    Input(s)         :  CMcNetMessageMap pointer as void*
    Output           :
    Functionality    :
    Member of        :  CMsgContainerJ1939
    Friend of        :      -
    Author(s)        :  Arun kumar K
    Date Created     :  20.01.2011
******************************************************************************/
void CMsgContainerJ1939::vInit(void* /*pParam*/)
{
    //int nSize =sizeof(m_sJ1939ReadMsgSpl)*5000;
}


/**********************************************************************************
  Function Name :   bStartReadThread
  Input(s)      :   -
  Output        :   -
  Functionality :   -
  Member of     :   CMsgContainerJ1939
  Friend of     :   -
  Author(s)        :  Arun kumar K
  Date Created     :  20.01.2011
  Modifications :
************************************************************************************/
BOOL CMsgContainerJ1939:: bStartReadThread()
{
    int bResult = TRUE;
    HRESULT hResult;
    if (nullptr != m_pouDIL_J1939)
    {
        hResult = m_pouDIL_J1939->DILIJ_ManageMsgBuf(MSGBUF_ADD,
                  m_dwClientId, &m_ouVSEBufJ1939);
    }

    bResult = CMsgContainerBase::bStartReadThread(m_ouVSEBufJ1939.hGetNotifyingEvent());
    return bResult;
}

/**********************************************************************************
  Function Name :   hToggleDILBufferRead
  Input(s)      :   -
  Output        :   -
  Functionality :   -
  Member of     :   CMsgContainerJ1939
  Friend of     :   -
  Author(s)     :  Arun kumar K
  Date Created  :  28.03.2011
  Modifications :
************************************************************************************/
HRESULT CMsgContainerJ1939:: hToggleDILBufferRead(BOOL bRead)
{
    HRESULT hResult =  S_FALSE;
    if (nullptr != m_pouDIL_J1939)
    {
        if(bRead)
            hResult = m_pouDIL_J1939->DILIJ_ManageMsgBuf(MSGBUF_ADD,
                      m_dwClientId, &m_ouVSEBufJ1939);
        else
            hResult = m_pouDIL_J1939->DILIJ_ManageMsgBuf(MSGBUF_CLEAR,
                      m_dwClientId, &m_ouVSEBufJ1939);
    }
    return hResult;
}

/******************************************************************************
    Function Name    :  bStopReadThread
    Input(s)         :
    Output           :
    Functionality    :  Stop the read thread
    Member of        :  CMsgContainerJ1939
    Friend of        :      -
    Author(s)        :  Arun kumar K
    Date Created     :  20.01.2011
******************************************************************************/
BOOL CMsgContainerJ1939:: bStopReadThread()
{
    BOOL bReturn = CMsgContainerBase::bStopReadThread();
    if (nullptr != m_pouDIL_J1939)
    {
        m_pouDIL_J1939->DILIJ_ManageMsgBuf(MSGBUF_CLEAR,
                                           m_dwClientId, &m_ouVSEBufJ1939);
    }

    return bReturn;
}

/******************************************************************************
    Function Name    :  vEditClearAll
    Input(s)         :
    Output           :
    Functionality    :  Clear all the storage for UI
    Member of        :  CMsgContainerJ1939
    Friend of        :      -
    Author(s)        :  Arun kumar K
    Date Created     :  20.01.2011
******************************************************************************/
void CMsgContainerJ1939::vEditClearAll()
{
    m_ouOWJ1939Buf.vClearMessageBuffer();
    m_ouAppendJ1939Buf.vClearMessageBuffer();
    memset(&m_sJ1939ReadMsgSpl, 0, sizeof(m_sJ1939ReadMsgSpl));
}

/******************************************************************************
    Function Name    :  nGetAppendBufferCount
    Input(s)         :
    Output           :
    Functionality    :
    Member of        :  CMsgContainerJ1939
    Friend of        :      -
    Author(s)        :  Arun kumar K
    Date Created     :  20.01.2011
******************************************************************************/
int CMsgContainerJ1939::nGetAppendBufferCount()
{
    return m_ouAppendJ1939Buf.GetMsgCount();
}

/******************************************************************************
    Function Name    :  nGetOWBufferCount
    Input(s)         :
    Output           :
    Functionality    :
    Member of        :  CMsgContainerJ1939
    Friend of        :      -
    Author(s)        :  Arun kumar K
    Date Created     :  20.01.2011
******************************************************************************/
int CMsgContainerJ1939::nGetOWBufferCount()
{
    return m_ouOWJ1939Buf.GetMsgCount();
}

/******************************************************************************
    Function Name    :  hReadFromOWBuffer
    Input(s)         :
    Output           :
    Functionality    :
    Member of        :  CMsgContainerJ1939
    Friend of        :      -
    Author(s)        :  Arun kumar K
    Date Created     :  20.01.2011
******************************************************************************/
HRESULT CMsgContainerJ1939::hReadFromOWBuffer(void* psMsg, __int64 nMapIndex)
{
    return m_ouOWJ1939Buf.ReadFromBuffer((STJ1939_MSG*)psMsg, nMapIndex);
}

/******************************************************************************
    Function Name    :  hReadFromAppendBuffer
    Input(s)         :
    Output           :
    Functionality    :
    Member of        :  CMsgContainerJ1939
    Friend of        :      -
    Author(s)        :  Arun kumar K
    Date Created     :  20.01.2011
******************************************************************************/
HRESULT CMsgContainerJ1939::hReadFromAppendBuffer(void* pvMsg, int nMsgIndex)
{
    EnterCriticalSection(&m_sCritSecDataSync);
    STJ1939_MSG* psMsg = (STJ1939_MSG*)pvMsg;
    static STJ1939MSGSPL sJ1939MsgSpl;
    INT nType = 0;
    INT nSize = MAX_MSG_LEN_J1939 + sizeof(__int64);
    HRESULT hResult =  m_ouAppendJ1939Buf.ReadEntry(nType, m_pbyJ1939Data, nSize,
                       nMsgIndex, FALSE);
    if(hResult == S_OK)
    {
        sJ1939MsgSpl.vSetDataStream(m_pbyJ1939Data);
        STJ1939_MSG* psTemp = (STJ1939_MSG*)&sJ1939MsgSpl;
        *psMsg = *psTemp;
    }
    LeaveCriticalSection(&m_sCritSecDataSync);
    return hResult;
}


/******************************************************************************
    Function Name    :  vSaveOWandGetDetails
    Input(s)         :
    Output           :
    Functionality    :  Save to OW buffer and provide the details requested
                        by receive child/ dll user class
    Member of        :  CMsgContainerJ1939
    Friend of        :      -
    Author(s)        :  Arun kumar K
    Date Created     :  20.01.2011
******************************************************************************/
void CMsgContainerJ1939::vSaveOWandGetDetails( void* pMsg,
        __int64& dwMapIndex,
        __int64& dwTimeStamp,
        UINT& nMsgCode,
        int& nBufferIndex, EINTERPRET_MODE& interPretable)
{
    STJ1939_MSG* pouJ1939Data = (STJ1939_MSG*)pMsg;
    dwTimeStamp = pouJ1939Data->m_sMsgProperties.m_un64TimeStamp;
    nMsgCode   = pouJ1939Data->m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN();
    dwMapIndex =  nCreateMapIndexKey((LPVOID)pouJ1939Data);

    interPretable = MODE_NONE;
    if (nullptr != mBmNetwork)
    {
        int nCount = 0;

        IFrame* pouFrame = nullptr;
        mBmNetwork->GetFrame(J1939, 0, nMsgCode, nullptr, &pouFrame);
        if (nullptr != pouFrame)
        {
            if (pouFrame->GetSignalCount() > 0)
            {
                interPretable = INTERPRETABLE;
            }
        }
    }

    //Now write into the array
    m_ouOWJ1939Buf.WriteIntoBuffer(pouJ1939Data, dwMapIndex, nBufferIndex);
}

/******************************************************************************
    Function Name    :  nCreateMapIndexKey
    Input(s)         :
    Output           :
    Functionality    :  Creates a key for SmsgDispEntry map
    Member of        :  CMsgContainerJ1939
    Friend of        :      -
    Author(s)        :  Arun kumar K
    Date Created     :  01.02.2011
******************************************************************************/
__int64 CMsgContainerJ1939::nCreateMapIndexKey( LPVOID pMsgData )
{
    STJ1939_MSG* pouJ1939Msg = (STJ1939_MSG*) pMsgData;
    // Form message to get message index in the CMap
    UINT nMsgID = MAKE_RX_TX_MESSAGE(
                      pouJ1939Msg->m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN(),
                      IS_RX_MESSAGE(pouJ1939Msg->m_sMsgProperties.m_eDirection));

    // For extended message
    nMsgID = MAKE_EXTENDED_MESSAGE_TYPE(nMsgID);
    // Apply Channel Information
    __int64 n64MapIndex = MAKE_CHANNEL_SPECIFIC_MESSAGE( nMsgID,
                          pouJ1939Msg->m_sMsgProperties.m_byChannel);

    n64MapIndex = MAKE_SOURCE_SPECIFIC_MESSAGE( n64MapIndex,
                  pouJ1939Msg->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_bySrcAddress);

    n64MapIndex = MAKE_DEST_SPECIFIC_MESSAGE( n64MapIndex,
                  pouJ1939Msg->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific);

    n64MapIndex = MAKE_TYPE_SPECIFIC_MESSAGE( n64MapIndex,
                  pouJ1939Msg->m_sMsgProperties.m_eType);

    return n64MapIndex;
}

/******************************************************************************
    Function Name    :  ApplyFilterScheme
    Input(s)         :
    Output           :
    Functionality    :
    Member of        :  CMsgContainerJ1939
    Friend of        :      -
    Author(s)        :  Arun kumar K
    Date Created     :  01.02.2011
******************************************************************************/
HRESULT CMsgContainerJ1939::ApplyFilterScheme(void* pvFilterApplied)
{
    HRESULT hResult = S_FALSE;
    SFILTERAPPLIED_J1939* psFilterJ1939 = (SFILTERAPPLIED_J1939*)pvFilterApplied;
    if (psFilterJ1939 != nullptr)
    {
        EnterCriticalSection(&m_omCritSecFilter);
        if (m_sFilterJ1939.bClone(*psFilterJ1939) == TRUE)
        {
            hResult = TRUE;
        }
        LeaveCriticalSection(&m_omCritSecFilter);
    }
    return hResult;
}

/******************************************************************************
    Function Name    :  GetFilterScheme
    Input(s)         :
    Output           :
    Functionality    :
    Member of        :  CMsgContainerJ1939
    Friend of        :      -
    Author(s)        :  Arun kumar K
    Date Created     :  01.02.2011
******************************************************************************/
HRESULT CMsgContainerJ1939::GetFilterScheme(void* pvFilterApplied)
{
    HRESULT hResult = S_FALSE;
    SFILTERAPPLIED_J1939* psFilterJ1939 = (SFILTERAPPLIED_J1939*)pvFilterApplied;
    if (psFilterJ1939 != nullptr)
    {
        if (psFilterJ1939->bClone(m_sFilterJ1939) == TRUE)
        {
            hResult = TRUE;
        }
    }
    return hResult;
}

/******************************************************************************
    Function Name    :  EnableFilterApplied
    Input(s)         :
    Output           :
    Functionality    :
    Member of        :  CMsgContainerJ1939
    Friend of        :      -
    Author(s)        :  Arun kumar K
    Date Created     :  01.02.2011
******************************************************************************/
HRESULT CMsgContainerJ1939::EnableFilterApplied(BOOL bEnable)
{
    EnterCriticalSection(&m_omCritSecFilter);
    m_sFilterJ1939.m_bEnabled = bEnable;
    LeaveCriticalSection(&m_omCritSecFilter);
    return S_OK;
}

/******************************************************************************
    Function Name    :  hUpdateFormattedMsgStruct
    Input(s)         :
    Output           :
    Functionality    :  Format the requested Msg and save it in Format data
                        structure which is accessible from the User module
    Member of        :  CMsgContainerJ1939
    Friend of        :      -
    Author(s)        :  Arun kumar K
    Date Created     :  20.01.2011
******************************************************************************/
HRESULT CMsgContainerJ1939::hUpdateFormattedMsgStruct(int nListIndex,
        int& nMsgCode,
        BYTE bExprnFlag_Disp,
        __int64 nTimeOffset)
{
    HRESULT hResult = S_FALSE;
    INT nSize = 0;
    nMsgCode  = 0;
    //memset( &m_sOutFormattedData, 0, sizeof(m_sOutFormattedData) );
    if( IS_MODE_APPEND(bExprnFlag_Disp) )
    {
        INT nType = 0;
        nSize = MAX_MSG_LEN_J1939 + sizeof(__int64);

        //In append mode providing interpret state is not required
        hResult = m_ouAppendJ1939Buf.ReadEntry(nType, m_pbyJ1939Data, nSize,
                                               nListIndex, FALSE);

        if (hResult == S_OK)
        {
            // Save it into J1939 message structure
            m_sJ1939ReadMsgSpl.vSetDataStream(m_pbyJ1939Data);

            // Copy delta time
            BYTE* pBytTemp = m_pbyJ1939Data;
            pBytTemp += /*m_sJ1939Data.unGetSize()*/nSize - sizeof(__int64);
            memcpy(&m_sJ1939ReadMsgSpl.m_nDeltime, pBytTemp, sizeof(__int64));
        }
    }
    else
    {
        hResult = m_ouOWJ1939Buf.ReadFromBuffer(&m_sJ1939Data,
                                                nListIndex);
        if (hResult == S_OK)
        {
            UINT64 u64DelTS = m_sJ1939Data.m_sMsgProperties.m_un64TimeStamp - nTimeOffset;

            m_sJ1939Data.vGetDataStream(m_pbyJ1939Data);
            m_sJ1939ReadMsgSpl.vSetDataStream(m_pbyJ1939Data);
            m_sJ1939ReadMsgSpl.m_nDeltime = u64DelTS;
        }
    }

    if (hResult == S_OK)
    {
        m_ouFormatJ1939.vFormatJ1939DataMsg(
            (PSTJ1939_MSG) &(m_sJ1939ReadMsgSpl), m_sJ1939ReadMsgSpl.m_nDeltime,
            &m_sOutFormattedData, bExprnFlag_Disp);

        CString name;
        GetMessageName(m_sJ1939ReadMsgSpl, name, IS_NUM_HEX_SET(bExprnFlag_Disp));
        strcpy(m_sOutFormattedData.m_acMsgName, name.GetBuffer(0));

        //Now add the name of message if present in database else show the code
        if (nullptr != m_sJ1939ReadMsgSpl.m_pbyData)
        {
            //nMsgCode = m_sJ1939ReadMsgSpl.m_pbyData[0];
            nMsgCode = m_sJ1939ReadMsgSpl.m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN();
        }
    }
    return hResult;
}
HRESULT CMsgContainerJ1939::GetMessageName(STJ1939_MSG& msg, CString& msgName, bool formatHexForId)
{
    //Message Name
    std::string strName = "";
    IFrame* pouFrame = nullptr;
    ERRORCODE eResult = mBmNetwork->GetFrame(J1939, 0, msg.m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN(), nullptr, &pouFrame);
    if (EC_SUCCESS == eResult && nullptr != pouFrame)
    {
        pouFrame->GetName(strName);
        msgName = strName.c_str();
    }
    else
    {
        if (formatHexForId == true)
        {
            msgName.Format("0x%X", msg.m_sMsgProperties.m_uExtendedID.m_unExtID);
        }
        else
        {
            msgName.Format("%d", msg.m_sMsgProperties.m_uExtendedID.m_unExtID);
        }
    }
    return S_OK;
}
/******************************************************************************
    Function Name    :  vSetCurrMsgName
    Input(s)         :
    Output           :
    Functionality    :  Current msg name from DB
    Member of        :  CMsgContainerJ1939
    Friend of        :      -
    Author(s)        :  Arun kumar K
    Date Created     :  20.01.2011
******************************************************************************/
void CMsgContainerJ1939::vSetCurrMsgName(CString strMsgNameOrCode)
{
    CMsgContainerBase::bCopyStringTocharArr (m_sOutFormattedData.m_acMsgName, strMsgNameOrCode,
            sizeof(m_sOutFormattedData.m_acMsgName));
}
void CMsgContainerJ1939::vSetMsgLength(CString strsgLength)
{
    if(strsgLength.IsEmpty() == FALSE)
    {
        CMsgContainerBase::bCopyStringTocharArr (m_sOutFormattedData.m_acDataLen, strsgLength,
                sizeof(m_sOutFormattedData.m_acDataLen));
    }
}
/******************************************************************************
    Function Name    :  vClearFormattedMsgStruct
    Input(s)         :
    Output           :
    Functionality    :  Clear format data structure pointers
    Member of        :  CMsgContainerJ1939
    Friend of        :      -
    Author(s)        :  Arun kumar K
    Date Created     :  20.01.2011
******************************************************************************/
void CMsgContainerJ1939::vClearFormattedMsgStruct()
{
    strcpy_s(m_sOutFormattedData.m_acTimeAbs,  sizeof(m_sOutFormattedData.m_acTimeAbs),  "");
    strcpy_s(m_sOutFormattedData.m_acTimeRel,  sizeof(m_sOutFormattedData.m_acTimeRel),  "");
    strcpy_s(m_sOutFormattedData.m_acTimeSys,  sizeof(m_sOutFormattedData.m_acTimeSys),  "");
    strcpy_s(m_sOutFormattedData.m_pcDataHex,  MAX_DATA_LEN_J1939,  "");
    strcpy_s(m_sOutFormattedData.m_acMsgIDHex, sizeof(m_sOutFormattedData.m_acPGNHex), "");
    strcpy_s(m_sOutFormattedData.m_acPGNHex,   sizeof(m_sOutFormattedData.m_acPGNHex),   "");
    strcpy_s(m_sOutFormattedData.m_acSrcHex,   sizeof(m_sOutFormattedData.m_acSrcHex),   "");
    strcpy_s(m_sOutFormattedData.m_acDestHex,  sizeof(m_sOutFormattedData.m_acDestHex),  "");
    strcpy_s(m_sOutFormattedData.m_pcDataDec,  MAX_DATA_LEN_J1939,  "");
    strcpy_s(m_sOutFormattedData.m_acMsgIDDec, sizeof(m_sOutFormattedData.m_acMsgIDDec), "");
    strcpy_s(m_sOutFormattedData.m_acPGNDec,   sizeof(m_sOutFormattedData.m_acPGNDec),   "");
    strcpy_s(m_sOutFormattedData.m_acSrcDec,   sizeof(m_sOutFormattedData.m_acSrcDec),   "");
    strcpy_s(m_sOutFormattedData.m_acDestDec,  sizeof(m_sOutFormattedData.m_acDestDec),  "");
    strcpy_s(m_sOutFormattedData.m_acDataLen,  sizeof(m_sOutFormattedData.m_acDataLen),  "");
    strcpy_s(m_sOutFormattedData.m_acMsgType,  sizeof(m_sOutFormattedData.m_acMsgType),  "");
    strcpy_s(m_sOutFormattedData.m_acMsgDir,   sizeof(m_sOutFormattedData.m_acMsgDir),   "");
    strcpy_s(m_sOutFormattedData.m_acChannel,  sizeof(m_sOutFormattedData.m_acChannel),  "");
    strcpy_s(m_sOutFormattedData.m_acPriority, sizeof(m_sOutFormattedData.m_acPriority), "");
    strcpy_s(m_sOutFormattedData.m_acMsgName,  sizeof(m_sOutFormattedData.m_acMsgName),  "");
}
/******************************************************************************
    Function Name    :  vGetUpdatedCurrDataPtrArray
    Input(s)         :
    Output           :
    Functionality    :  Provide format data structure pointers to the user
    Member of        :  CMsgContainerJ1939
    Friend of        :      -
    Author(s)        :  Arun kumar K
    Date Created     :  20.01.2011
******************************************************************************/
void CMsgContainerJ1939::vGetUpdatedCurrDataPtrArray(SMSGWNDHDRCOL& sHdrColStruct,
        char* pomDataPtrArr[MAX_MSG_WND_COL_CNT],
        BYTE bExprnFlag_Disp)
{
    //Time mode
    if (IS_TM_ABS_SET(bExprnFlag_Disp))
    {
        pomDataPtrArr[sHdrColStruct.m_byTimePos] = m_sOutFormattedData.m_acTimeAbs;
    }

    else if (IS_TM_REL_SET(bExprnFlag_Disp))
    {
        pomDataPtrArr[sHdrColStruct.m_byTimePos] = m_sOutFormattedData.m_acTimeRel;
    }

    else if (IS_TM_SYS_SET(bExprnFlag_Disp))
    {
        pomDataPtrArr[sHdrColStruct.m_byTimePos] = m_sOutFormattedData.m_acTimeSys;
    }

    // Dec/Hex mode for DATA and ID
    if (IS_NUM_HEX_SET(bExprnFlag_Disp))
    {
        pomDataPtrArr[sHdrColStruct.m_byDataPos] = m_sOutFormattedData.m_pcDataHex;
        pomDataPtrArr[sHdrColStruct.m_byIDPos]   = m_sOutFormattedData.m_acMsgIDHex;
        pomDataPtrArr[sHdrColStruct.m_byPGNPos]  = m_sOutFormattedData.m_acPGNHex;
        pomDataPtrArr[sHdrColStruct.m_bySrcPos]  = m_sOutFormattedData.m_acSrcHex;
        pomDataPtrArr[sHdrColStruct.m_byDestPos] = m_sOutFormattedData.m_acDestHex;
    }
    else if (IS_NUM_DEC_SET(bExprnFlag_Disp))
    {
        pomDataPtrArr[sHdrColStruct.m_byDataPos] = m_sOutFormattedData.m_pcDataDec;
        pomDataPtrArr[sHdrColStruct.m_byIDPos]   = m_sOutFormattedData.m_acMsgIDDec;
        pomDataPtrArr[sHdrColStruct.m_byPGNPos]  = m_sOutFormattedData.m_acPGNDec;
        pomDataPtrArr[sHdrColStruct.m_bySrcPos]  = m_sOutFormattedData.m_acSrcDec;
        pomDataPtrArr[sHdrColStruct.m_byDestPos] = m_sOutFormattedData.m_acDestDec;
    }

    //Rest will always be same
    pomDataPtrArr[sHdrColStruct.m_byDLCPos]       = m_sOutFormattedData.m_acDataLen;
    pomDataPtrArr[sHdrColStruct.m_byMsgTypePos]   = m_sOutFormattedData.m_acMsgType;
    pomDataPtrArr[sHdrColStruct.m_byRxTxPos]      = m_sOutFormattedData.m_acMsgDir;
    pomDataPtrArr[sHdrColStruct.m_byChannel]      = m_sOutFormattedData.m_acChannel;
    pomDataPtrArr[sHdrColStruct.m_byPriorityPos]  = m_sOutFormattedData.m_acPriority;
    pomDataPtrArr[sHdrColStruct.m_byCodeNamePos]  = m_sOutFormattedData.m_acMsgName;
}

/******************************************************************************
    Function Name    :  bGetDilInterFace
    Input(s)         :
    Output           :
    Functionality    :  Get Dil interface pointer
    Member of        :  CMsgContainerJ1939
    Friend of        :      -
    Author(s)        :  Arun kumar K
    Date Created     :  20.01.2011
******************************************************************************/
BOOL CMsgContainerJ1939::bGetDilInterFace()
{
    BOOL bFound = FALSE;;
    DIL_GetInterface( J1939, (void**)&m_pouDIL_J1939 );
    if (nullptr != m_pouDIL_J1939)
    {
        bFound = TRUE;
    }
    return bFound;
}

/******************************************************************************
    Function Name    :  SetClientID
    Input(s)         :
    Output           :
    Functionality    :  Get Dil interface pointer
    Member of        :  CMsgContainerJ1939
    Friend of        :      -
    Author(s)        :  Arun kumar K
    Date Created     :  20.01.2011
******************************************************************************/
void CMsgContainerJ1939::SetClientID(DWORD dwClientID)
{
    m_dwClientId = dwClientID;
}

/******************************************************************************
    Function Name    :  DoSortBuffer
    Input(s)         :
    Output           :
    Functionality    :
    Member of        :  CMsgContainerJ1939
    Friend of        :      -
    Author(s)        :  Arun kumar K
    Date Created     :  20.01.2011
******************************************************************************/
void CMsgContainerJ1939::DoSortBuffer(int nField,bool bAscending)
{
    m_ouOWJ1939Buf.vDoSortBuffer(nField, bAscending);
    //m_ouAppendJ1939Buf.vDoSortBuffer(nField, bAscending);
}

/******************************************************************************
    Function Name    :  GetMapIndexAtID
    Input(s)         :
    Output           :
    Functionality    :
    Member of        :  CMsgContainerJ1939
    Friend of        :      -
    Author(s)        :  Arun kumar K
    Date Created     :  20.01.2011
******************************************************************************/
void CMsgContainerJ1939::GetMapIndexAtID(int nIndex,__int64& nMapIndex)
{
    m_ouOWJ1939Buf.nGetMapIndexAtID(nIndex,nMapIndex);
}

HRESULT CMsgContainerJ1939::LoadPage(const unsigned long& /*nPageNo*/)
{
    return S_OK;
}
HRESULT CMsgContainerJ1939::OverwritePage(const unsigned long& /*nLineNo*/)
{
    return S_OK;
}
HRESULT CMsgContainerJ1939::SetIBMNetWorkGetService(void* pBmNetWork)
{
    mBmNetwork = (IBMNetWorkGetService*)pBmNetWork;
    return S_OK;
}

HRESULT CMsgContainerJ1939::GetSignalInfoArray(__int64 nMapIndex, SSignalInfoArray& SigInfoArray, bool formatInHex)
{
    STJ1939_MSG sJ1939Msg;

    HRESULT hResult = hReadFromOWBuffer(&sJ1939Msg, nMapIndex);
    if (hResult == S_OK)
    {
        return GetInterPretedSignalList(sJ1939Msg, SigInfoArray, formatInHex);
    }

    return S_OK;
}

HRESULT CMsgContainerJ1939::GetInterPretedSignalList(STJ1939_MSG& ouMsg, SSignalInfoArray& ouSSignalInfoArray, bool isHexMode)
{
    if (nullptr == mBmNetwork)
    {
        return S_FALSE;
    }
    IFrame* pouFrame = nullptr;
    ERRORCODE eResult = mBmNetwork->GetFrame(J1939, 0, ouMsg.m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN(), nullptr, &pouFrame);
    ouSSignalInfoArray.RemoveAll();
    if (EC_SUCCESS == eResult && nullptr != pouFrame)
    {
        std::list<InterpreteSignals> ouSignal;
        std::string strName;
        pouFrame->GetName(strName);
        pouFrame->InterpretSignals(ouMsg.m_pbyData, ouMsg.m_unDLC, ouSignal, isHexMode, true);
for (auto ouSignalInfo : ouSignal)   //Only C++11;
        {
            SSignalInfo ouSignal;
            ouSignal.m_omEnggValue = ouSignalInfo.m_omEnggValue.c_str();
            ouSignal.m_omRawValue = ouSignalInfo.m_omRawValue.c_str();
            ouSignal.m_omSigName = ouSignalInfo.m_omSigName.c_str();
            ouSignal.m_omUnit = ouSignalInfo.m_omUnit.c_str();
            ouSignal.m_msgName = strName.c_str();
            ouSSignalInfoArray.Add(ouSignal);
        }
    }
    return S_OK;
}
HRESULT CMsgContainerJ1939::GetMessageDetails(__int64 nMapIndex, unsigned int& msgId, CString& msgName, bool formatHexForId)
{
    if (nullptr == mBmNetwork)
    {
        return S_FALSE;
    }

    STJ1939_MSG sJ1939Msg;
    std::string strName = "";
    HRESULT hResult = hReadFromOWBuffer(&sJ1939Msg, nMapIndex);

    msgId = sJ1939Msg.m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN();

    IFrame* pouFrame = nullptr;

    ERRORCODE eResult = mBmNetwork->GetFrame(LIN, 0, msgId, nullptr, &pouFrame);
    if (EC_SUCCESS == eResult && nullptr != pouFrame)
    {
        pouFrame->GetName(strName);
        msgName = strName.c_str();
    }
    else
    {
        if (formatHexForId == true)
        {
            msgName.Format("%d", msgId);
        }
        else
        {
            msgName.Format("0x%X", msgId);
        }
    }
}
COLORREF CMsgContainerJ1939::getMessageColor(long long key, bool isAppendMode, CMessageAttrib* msgAttributes/*has to be removed*/)
{
    if (msgAttributes == nullptr)
    {
        return RGB(0, 0, 0);
    }
    HRESULT hResult = S_FALSE;




    unsigned int pgn = 0;
    if (isAppendMode == true)
    {
        EnterCriticalSection(&m_sCritSecDataSync);
        STJ1939MSGSPL sJ1939MsgSpl;
        INT nType = 0;
        INT nSize = MAX_MSG_LEN_J1939 + sizeof(__int64);
        hResult = m_ouAppendJ1939Buf.ReadEntry(nType, m_pbyJ1939Data, nSize,
                                               (int)key, FALSE);
        sJ1939MsgSpl.vSetDataStream(m_pbyJ1939Data);
        pgn = sJ1939MsgSpl.m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN();
        LeaveCriticalSection(&m_sCritSecDataSync);

    }
    else
    {
        STJ1939_MSG psMsg;
        hResult = hReadFromOWBuffer(&psMsg, key);
        pgn = psMsg.m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN();
    }
    if (CALL_SUCCESS == hResult)
    {
        return msgAttributes->GetCanIDColour(pgn);
    }
    return RGB(0, 0, 0);
}
HRESULT CMsgContainerJ1939::sendMessage(long long /*key*/, bool /*isAppendMode*//*has to be removed*/)
{
    return S_FALSE;
}