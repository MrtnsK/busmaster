/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file      CAN_ICS_neoVI.cpp
 * \brief     Exports API functions for IntrepidCS neoVI CAN Hardware interface
 * \author    Pradeep Kadoor, Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Exports API functions for IntrepidCS neoVI CAN Hardware interface
 */
#include "CAN_ICS_neoVI_stdafx.h"
#include "CAN_ICS_neoVI.h"
//#include "include/Error.h"
//#include "include/basedefs.h"
//#include "DataTypes/Base_WrapperErrorLogger.h"
//#include "DataTypes/MsgBufAll_DataTypes.h"
//#include "DataTypes/DIL_Datatypes.h"
//#include "Include/CAN_Error_Defs.h"
//#include "Include/CanUsbDefs.h"
//#include "Include/Struct_CAN.h"
#include "CAN_ICS_neoVI_Channel.h"
#include "CAN_ICS_neoVI_Network.h"
#include "Utility/Utility_Thread.h"
//#include "Include/DIL_CommonDefs.h"
#include "EXTERNAL/icsnVC40.h"
//#include "DIL_Interface/BaseDIL_CAN_Controller.h"
#include "DIL_Interface/HardwareListingCAN.h"
#include "ChangeRegisters_CAN_ICS_NeoVI.h"
#include "../Application/MultiLanguage.h"

#define USAGE_EXPORT
#include "CAN_ICS_neoVI_Extern.h"
#include <sstream>
#include <string>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CCAN_ICS_neoVIApp, CWinApp)
END_MESSAGE_MAP()


#define defUSB_MODE_ACTIVE                     1
#define defUSB_MODE_PASSIVE                    2
#define defUSB_MODE_SIMULATE                   3


CCAN_ICS_neoVIApp::CCAN_ICS_neoVIApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}


/** The one and only CCAN_ICS_neoVIApp object */
CCAN_ICS_neoVIApp theApp;

static HINSTANCE ghLangInst = nullptr;

BOOL CCAN_ICS_neoVIApp::InitInstance()
{
    // Begin of Multiple Language support
    if ( CMultiLanguage::m_nLocales <= 0 )    // Not detected yet
    {
        CMultiLanguage::DetectLangID(); // Detect language as user locale
        CMultiLanguage::DetectUILanguage();    // Detect language in MUI OS
    }
    TCHAR szModuleFileName[MAX_PATH];        // Get Module File Name and path
    int ret = ::GetModuleFileName(theApp.m_hInstance, szModuleFileName, MAX_PATH);
    if ( ret == 0 || ret == MAX_PATH )
    {
        ASSERT(false);
    }
    // Load resource-only language DLL. It will use the languages
    // detected above, take first available language,
    // or you can specify another language as second parameter to
    // LoadLangResourceDLL. And try that first.
    ghLangInst = CMultiLanguage::LoadLangResourceDLL( szModuleFileName );
    if (ghLangInst)
    {
        AfxSetResourceHandle( ghLangInst );
    }
    // End of Multiple Language support

    CWinApp::InitInstance();

    return TRUE;
}

#define MAX_BUFF_ALLOWED 16
#define defSTR_MISSING_HARDWARE "Required number of hardware are not present\nRequired: %2d    Available: %2d"

const int CONFIGBYTES_TOTAL = 1024;
const BYTE CREATE_MAP_TIMESTAMP = 0x1;
const BYTE CALC_TIMESTAMP_READY = 0x2;

/**
 * Current state machine
 */
static BYTE sg_byCurrState = CREATE_MAP_TIMESTAMP;

/**
 * Client and Client Buffer map
 */
struct tagClientBufMap
{
    DWORD dwClientID;
    BYTE hClientHandle;
    CBaseCANBufFSE* pClientBuf[MAX_BUFF_ALLOWED];
    char pacClientName[MAX_PATH];
    UINT unBufCount;
    tagClientBufMap()
    {
        dwClientID = 0;
        hClientHandle = 0;
        unBufCount = 0;
        memset(pacClientName, 0, sizeof (char) * MAX_PATH);
        for (int i = 0; i < MAX_BUFF_ALLOWED; i++)
        {
            pClientBuf[i] = nullptr;
        }

    }
};
typedef tagClientBufMap sClientBufMap;
typedef sClientBufMap SCLIENTBUFMAP;
typedef sClientBufMap* PSCLIENTBUFMAP;

typedef struct tagAckMap
{
    UINT m_MsgID;
    UINT m_ClientID;
    UINT m_Channel;

    BOOL operator == (const tagAckMap& RefObj)const
    {
        return ((m_MsgID == RefObj.m_MsgID) && (m_Channel == RefObj.m_Channel));
    }
} SACK_MAP;

static  CRITICAL_SECTION sg_CritSectForAckBuf;       // To make it thread safe
typedef std::list<SACK_MAP> CACK_MAP_LIST;
static CACK_MAP_LIST sg_asAckMapBuf;

/**
 * Starts code for the state machine
 */
enum
{
    STATE_DRIVER_SELECTED    = 0x0,
    STATE_HW_INTERFACE_LISTED,
    STATE_HW_INTERFACE_SELECTED,
    STATE_CONNECTED
};
BYTE sg_bCurrState = STATE_DRIVER_SELECTED;

//#define VALIDATE_VALUE_RETURN_VAL(Val1, Val2, RetVal)   if (Val1 != Val2) {return RetVal;}

static SYSTEMTIME sg_CurrSysTime;
static UINT64 sg_TimeStamp = 0;

/**
 * Query Tick Count
 */
static LARGE_INTEGER sg_QueryTickCount;
static LARGE_INTEGER sg_lnFrequency;
static HWND sg_hOwnerWnd = nullptr;

//static CBaseCANBufFSE* sg_pCanBufObj[MAX_BUFF_ALLOWED];

static CPARAM_THREADPROC sg_sParmRThread;
static STATUSMSG sg_sCurrStatus;
static CNetwork* sg_podActiveNetwork;
static CNetwork sg_odSimulationNetwork;
#define MAX_CANHW 16
//static BYTE sg_hHardware[ MAX_CANHW ];
// The message buffer
const int ENTRIES_IN_GBUF       = 2000;
static int sg_nFRAMES = 128;
static STCANDATA sg_asMsgBuffer[ENTRIES_IN_GBUF];
static SCONTROLLER_DETAILS sg_ControllerDetails[defNO_OF_CHANNELS];
static INTERFACE_HW sg_HardwareIntr[defNO_OF_CHANNELS];

/**
 * Network for actual hardware
 */
static CNetwork sg_odHardwareNetwork;

static NeoDevice sg_ndNeoToOpen [CHANNEL_CAN_MAX];

// Create time struct. Use 0 to transmit the message with out any delay
typedef struct
{
    DWORD millis;          /*< base-value: milliseconds: 0.. 2^32-1 */
    WORD  millis_overflow; /*< roll-arounds of millis */
    WORD  micros;          /*< microseconds: 0..999 */
} TCANTimestamp;

static TCANTimestamp sg_sTime;
// static global variables ends

const int SIZE_WORD     = sizeof(WORD);
const int SIZE_CHAR     = sizeof(char);

// TZM specific Global variables
#define CAN_MAX_ERRSTR 256
#define MAX_CLIENT_ALLOWED 16
std::string sg_acErrStr = "";
static UINT sg_unClientCnt = 0;
static SCLIENTBUFMAP sg_asClientToBufMap[MAX_CLIENT_ALLOWED];
static UINT sg_unCntrlrInitialised = 0;
static HMODULE sg_hDll = nullptr;
static HANDLE m_hDataEvent = nullptr;
static HANDLE sg_hCntrlStateChangeEvent = nullptr;
static DWORD  sg_dwClientID = 0;

// state variables
static BOOL sg_bIsConnected = FALSE;
static BOOL sg_bIsDriverRunning = FALSE;
static UCHAR sg_ucControllerMode = defUSB_MODE_ACTIVE;

// Count variables
static UCHAR sg_ucNoOfHardware = 0;
static UCHAR sg_nNoOfChannels = 0;
static DWORD m_dwDriverId = 0;
static DWORD m_dwPrevDriverId = 0;

static int nGetChannelsInNeoVI(int nDevIndex);
static void vMapDeviceChannelIndex();
HRESULT hFillHardwareDesc(PSCONTROLLER_DETAILS pControllerDetails);

/*Please recheck and retain only necessary variables*/

#define NEW_LINE                "\n"
#define TOTAL_ERROR             600
#define MAX_BUFFER_VALUECAN     20000
#define WAITTIME_NEOVI          100
#define NETWORKS_COUNT          51

const int NEOVI_OK = 1;
const long VALUECAN_ERROR_BITS = SPY_STATUS_GLOBAL_ERR | SPY_STATUS_CRC_ERROR |
                                 SPY_STATUS_INCOMPLETE_FRAME | SPY_STATUS_UNDEFINED_ERROR
                                 | SPY_STATUS_BAD_MESSAGE_BIT_TIME_ERROR;

#define defSTR_ERROR_REPORT_FORMAT          "Channel %-2d : %s"
#define defSTR_WARNING_LIMIT_SET_FAILED     "Setting warning limit failed"
#define defSTR_FILTER_SET_FAILED            "Setting hardware filter failed"
#define defSTR_CONNECT_FAILED               "Connect failed"
#define defSTR_RESET_FAILED                 "Hardware reset failed"
#define defSTR_CLIENT_RESET_FAILED          "Software reset failed"
#define defSTR_CONTROLLER_MODE_SET_FAILED   "Setting hardware mode failed"

static UCHAR m_unWarningLimit[defNO_OF_CHANNELS] = {0};
static UCHAR m_unWarningCount[defNO_OF_CHANNELS] = {0};
static UCHAR m_unRxError = 0;
static UCHAR m_unTxError = 0;
const int MAX_DEVICES  = 255;
static BYTE m_bytNetworkIDs[MAX_DEVICES] = {0};
static unsigned char m_ucNetworkID[NETWORKS_COUNT] = {0};
static int m_anhObject[MAX_DEVICES][NETWORKS_COUNT+1] = {0};
static int m_anhWriteObject[MAX_DEVICES] = {0};
static std::string m_omErrStr;
static BOOL m_bInSimMode = FALSE;
static int s_anErrorCodes[TOTAL_ERROR] = {0};

static INT sg_anSelectedItems[CHANNEL_ALLOWED];

/* Recheck ends */

/* Error Definitions */
#define CAN_USB_OK 0
#define CAN_QRCV_EMPTY 0x20

/* icsneo40.dll Basic Functions */
typedef int  (__stdcall* FINDNEODEVICES)(unsigned long DeviceTypes, NeoDevice* pNeoDevice, int* pNumDevices);
static FINDNEODEVICES icsneoFindNeoDevices;
typedef int  (__stdcall* OPENNEODEVICE)(NeoDevice* pNeoDevice, int* hObject, unsigned char* bNetworkIDs, int bConfigRead, int bSyncToPC);
static OPENNEODEVICE icsneoOpenNeoDevice;
typedef int  (__stdcall* CLOSEPORT)(int hObject, int* pNumberOfErrors);
static CLOSEPORT icsneoClosePort;
typedef void (__stdcall* FREEOBJECT)(int hObject);
static FREEOBJECT icsneoFreeObject;

/* icsneo40.dll Message Functions */
typedef int  (__stdcall* GETMESSAGES)(int hObject, icsSpyMessage* pMsg, int* pNumberOfMessages, int* pNumberOfErrors);
static GETMESSAGES icsneoGetMessages;
typedef int  (__stdcall* TXMESSAGES)(int hObject, icsSpyMessage* pMsg, int lNetworkID, int lNumMessages);
static TXMESSAGES icsneoTxMessages;
typedef int  (__stdcall* WAITFORRXMSGS)(int hObject, unsigned int iTimeOut);
static WAITFORRXMSGS icsneoWaitForRxMessagesWithTimeOut;
typedef int  (__stdcall* ENABLERXQUEUE)(int hObject, int iEnable);
static ENABLERXQUEUE icsneoEnableNetworkRXQueue;
typedef int  (__stdcall* GETTSFORMSG)(int hObject, icsSpyMessage* pMsg, double* pTimeStamp);
static GETTSFORMSG icsneoGetTimeStampForMsg;

/* icsneo40.dll Device Functions */
typedef int (__stdcall* GETCONFIG)(int hObject, unsigned char* pData, int* lNumBytes);
static GETCONFIG icsneoGetConfiguration;
typedef int (__stdcall* SENDCONFIG)(int hObject, unsigned char* pData, int lNumBytes);
static SENDCONFIG icsneoSendConfiguration;
typedef int (__stdcall* GETFIRESETTINGS)(int hObject, SFireSettings* pSettings, int iNumBytes);
static GETFIRESETTINGS icsneoGetFireSettings;
typedef int (__stdcall* SETFIRESETTINGS)(int hObject, SFireSettings* pSettings, int iNumBytes, int bSaveToEEPROM);
static SETFIRESETTINGS icsneoSetFireSettings;
typedef int (__stdcall* GETVCAN3SETTINGS)(int hObject, SVCAN3Settings* pSettings, int iNumBytes);
static GETVCAN3SETTINGS icsneoGetVCAN3Settings;
typedef int (__stdcall* SETVCAN3SETTINGS)(int hObject, SVCAN3Settings* pSettings, int iNumBytes, int bSaveToEEPROM);
static SETVCAN3SETTINGS icsneoSetVCAN3Settings;
typedef int (__stdcall* SETBITRATE)(int hObject, int BitRate, int NetworkID);
static SETBITRATE icsneoSetBitRate;
typedef int (__stdcall* GETDEVICEPARMS)(int hObject, char* pParameter, char* pValues, short ValuesLength);
static GETDEVICEPARMS icsneoGetDeviceParameters;
typedef int (__stdcall* SETDEVICEPARMS)(int hObject, char* pParmValue, int* pErrorIndex, int bSaveToEEPROM);
static SETDEVICEPARMS icsneoSetDeviceParameters;
typedef int (__stdcall* GETHWLICENSE)(int hObject, int* pnHardwareLic);
static GETHWLICENSE icsneoGetHardwareLicense;
typedef int (__stdcall* GETHWFIRMWAREINFO)(int hObject, stAPIFirmwareInfo* pInfo);
static GETHWFIRMWAREINFO icsneoGetHWFirmwareInfo;

/* icsneo40.dll Error Functions */
typedef int (__stdcall* GETLASTAPIERROR)(int hObject, unsigned long* pErrorNumber);
static GETLASTAPIERROR icsneoGetLastAPIError;
typedef int (__stdcall* GETERRMSGS)(int hObject, int* pErrorMsgs, int* pNumberOfErrors);
static GETERRMSGS icsneoGetErrorMessages;
typedef int (__stdcall* GETERRORINFO)(int lErrorNumber, char* szErrorDescriptionShort,
                                      char* szErrorDescriptionLong, int* lMaxLengthShort,
                                      int* lMaxLengthLong,int* lErrorSeverity,int* lRestartNeeded);
static GETERRORINFO icsneoGetErrorInfo;

/* icsneo40.dll General Utility Functions */
typedef int (__stdcall* VALIDATEHOBJECT)(int hObject);
static VALIDATEHOBJECT icsneoValidateHObject;
typedef int (__stdcall* GETDLLVERSION)(void);
static GETDLLVERSION icsneoGetDLLVersion;
typedef int (__stdcall* GETSERIALNUMBER)(int hObject, unsigned int* iSerialNumber);
static GETSERIALNUMBER icsneoGetSerialNumber;
typedef int (__stdcall* STARTSOCKSERVER)(int hObject, int iPort);
static STARTSOCKSERVER icsneoStartSockServer;
typedef int (__stdcall* STOPSOCKSERVER)(int hObject);
static STOPSOCKSERVER icsneoStopSockServer;

#define MAX_CHAR_SHORT 128
#define MAX_CHAR_LONG  512
#define CAN_USBMSGTYPE_DATA 2

/* Function pointers ends*/
typedef struct tagDATINDSTR
{
    BOOL    m_bIsConnected;
    HANDLE  m_hHandle;
    BOOL    m_bToContinue;
    UINT    m_unChannels;
} sDatIndStr;

static sDatIndStr s_DatIndThread;

/* CDIL_CAN_ICSNeoVI class definition */
class CDIL_CAN_ICSNeoVI : public CBaseDIL_CAN_Controller
{
public:
    HRESULT CAN_PerformInitOperations(void);
    HRESULT CAN_PerformClosureOperations(void);
    HRESULT CAN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER& QueryTickCount);
    HRESULT CAN_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount, PSCONTROLLER_DETAILS InitData);
    HRESULT CAN_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount);
    HRESULT CAN_DeselectHwInterface(void);
    HRESULT CAN_SetConfigData(PSCONTROLLER_DETAILS InitData, int Length);
    HRESULT CAN_StartHardware(void);
    HRESULT CAN_StopHardware(void);
    HRESULT CAN_GetCurrStatus(STATUSMSG& StatusData);
    HRESULT CAN_GetTxMsgBuffer(BYTE*& pouFlxTxMsgBuffer);
    HRESULT CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& sCanTxMsg);
    HRESULT CAN_GetBusConfigInfo(BYTE* BusInfo);
    HRESULT CAN_GetLastErrorString(std::string& acErrorStr);
    HRESULT CAN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam);
    HRESULT CAN_SetControllerParams(int nValue, ECONTR_PARAM eContrparam);
    HRESULT CAN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam);
    HRESULT CAN_SetAppParams(HWND hWndOwner);
    HRESULT CAN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj);
    HRESULT CAN_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName);
    HRESULT CAN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus);
    HRESULT CAN_LoadDriverLibrary(void);
    HRESULT CAN_UnloadDriverLibrary(void);
    HRESULT CAN_SetHardwareChannel(PSCONTROLLER_DETAILS,DWORD dwDriverId,bool bIsHardwareListed, unsigned int unChannelCount);
};
static CDIL_CAN_ICSNeoVI* sg_pouDIL_CAN_ICSNeoVI = nullptr;

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Returns the interface to controller
 */
USAGEMODE HRESULT GetIDIL_CAN_Controller(void** ppvInterface)
{
    HRESULT hResult = S_OK;
    if ( nullptr == sg_pouDIL_CAN_ICSNeoVI )
    {
        if ((sg_pouDIL_CAN_ICSNeoVI = new CDIL_CAN_ICSNeoVI) == nullptr)
        {
            hResult = S_FALSE;
        }
    }
    *ppvInterface = (void*) sg_pouDIL_CAN_ICSNeoVI;  /* Doesn't matter even if sg_pouDIL_CAN_Kvaser is null */

    return hResult;
}

/* HELPER FUNCTIONS START */

/**
 * Function create and set read indication event
 */
static int nCreateAndSetReadIndicationEvent(HANDLE& hReadEvent)
{
    int nReturn = -1;

    // Create a event
    m_hDataEvent = CreateEvent( nullptr,           // lpEventAttributes
                                FALSE,          // bManualReset
                                FALSE,          // bInitialState
                                "");     // Name
    if (m_hDataEvent != nullptr)
    {
        s_DatIndThread.m_hHandle = m_hDataEvent;
        hReadEvent = m_hDataEvent;
        nReturn = S_OK;
    }
    return nReturn;
}

/**
 * Function to retreive error occurred and log it
 */
static void vRetrieveAndLog(DWORD /*dwErrorCode*/, char* /*File*/, int /*Line*/)
{
    USES_CONVERSION;

    char acErrText[MAX_PATH] = {'\0'};
    // Get the error text for the corresponding error code
    //if ((*pfCAN_GetErrText)(dwErrorCode, acErrText) == CAN_USB_OK)
    {
        sg_acErrStr = acErrText;
    }
}

static int nSetHardwareMode(UCHAR ucDeviceMode)
{
    // Make sure to set the network to Hardware
    sg_podActiveNetwork = &sg_odHardwareNetwork;
    sg_ucControllerMode = ucDeviceMode;
    return 0;
}

/**
 * \param[in] ucRxErr Rx Error Counter Value
 * \param[in] ucTxErr Tx Error Counter Value
 * \return Type of the error message: Error Bus, Error Warning Limit and Error Interrupt
 *
 * Posts message as per the error counter. This function will
 * update local state variable as per error codes.
 */
static UCHAR USB_ucHandleErrorCounter( UCHAR ucChannel,
                                       UCHAR ucRxErr,
                                       UCHAR ucTxErr )
{
    UCHAR ucRetVal = ERROR_BUS;

    CChannel& odChannel = sg_podActiveNetwork->m_aodChannels[ ucChannel ];

    // Check for Error Handler Execution
    // Warning Limit Execution
    if( ucRxErr == odChannel.m_ucWarningLimit &&
            odChannel.m_bRxErrorExecuted == FALSE )
    {
        // Set Error type as warning limit and set the handler execution
        // Flag
        ucRetVal = ERROR_WARNING_LIMIT_REACHED;
        odChannel.m_bRxErrorExecuted = TRUE;
    }
    // Tx Error Value
    else if( ucTxErr == odChannel.m_ucWarningLimit &&
             odChannel.m_bTxErrorExecuted == FALSE )
    {
        // Set Error type as warning limit and set the handler execution
        // Flag
        ucRetVal = ERROR_WARNING_LIMIT_REACHED;
        odChannel.m_bTxErrorExecuted = TRUE;
    }
    //  If the error counter value is 95 then execute the warning limit
    // handler if we are in warning limit state
    if( ucRxErr == odChannel.m_ucWarningLimit - 1 &&
            odChannel.m_bRxErrorExecuted == TRUE )
    {
        // Change the type. This is not real error message
        ucRetVal = ERROR_WARNING_LIMIT_REACHED;
        //ucRetVal = ERROR_UNKNOWN;
        odChannel.m_bRxErrorExecuted = FALSE;
    }
    if( ucTxErr == odChannel.m_ucWarningLimit - 1 &&
            odChannel.m_bTxErrorExecuted == TRUE )
    {
        // Change the type. This is not real error message
        ucRetVal = ERROR_WARNING_LIMIT_REACHED;
        //ucRetVal = ERROR_UNKNOWN;
        odChannel.m_bTxErrorExecuted = FALSE;
    }

    // Reset Event handlers state
    if( ucRxErr < odChannel.m_ucWarningLimit - 1)
    {
        odChannel.m_bRxErrorExecuted = FALSE;
    }
    if( ucTxErr < odChannel.m_ucWarningLimit - 1)
    {
        odChannel.m_bTxErrorExecuted = FALSE;
    }

    // Supress State Change Interrupt messages
    // Active -> Passive
    if( ucRxErr == 127 &&
            odChannel.m_ucControllerState == defCONTROLLER_PASSIVE )
    {
        // Not an error. This is interrupt message
        ucRetVal = ERROR_INTERRUPT;
    }

    // Active -> Passive
    if( ucTxErr == 127 &&
            odChannel.m_ucControllerState == defCONTROLLER_PASSIVE )
    {
        // Not an error. This is interrupt message
        ucRetVal = ERROR_INTERRUPT;
    }
    return ucRetVal;
}

/**
 * \param[in] lError Error code in Peak USB driver format
 * \param[in] byDir  Error direction Tx/Rx
 * \return Error code in BUSMASTER application format
 *
 * This will convert the error code from Perk USB driver format
 * to the format that is used by BUSMASTER.
 */
static UCHAR USB_ucGetErrorCode(LONG lError, BYTE byDir)
{
    UCHAR ucReturn = 0;

    // Tx Errors
    if( byDir == 1)
    {
        if (lError & SPY_STATUS_CRC_ERROR)
        {
            ucReturn = CRC_ERROR_TX;
        }
        else if (lError & SPY_STATUS_INCOMPLETE_FRAME )
        {
            ucReturn = FORM_ERROR_TX;
        }
        else
        {
            ucReturn = OTHER_ERROR_TX;
        }
    }
    // Rx Errors
    else
    {
        if (lError & SPY_STATUS_CRC_ERROR)
        {
            ucReturn = CRC_ERROR_RX;
        }
        else if (lError & SPY_STATUS_INCOMPLETE_FRAME)
        {
            ucReturn = FORM_ERROR_RX;
        }
        else
        {
            ucReturn = OTHER_ERROR_RX;
        }
    }
    // Return the error code
    return ucReturn;
}

/**
 * Function to create time mode mapping
 */
static void vCreateTimeModeMapping()
{
    //MessageBox(0, L"TIME", L"", 0);
    GetLocalTime(&sg_CurrSysTime);
    //Query Tick Count
    QueryPerformanceCounter(&sg_QueryTickCount);
    // Get frequency of the performance counter
    QueryPerformanceFrequency(&sg_lnFrequency);
    // Convert it to time stamp with the granularity of hundreds of microsecond
    if ((sg_QueryTickCount.QuadPart * 10000) > sg_lnFrequency.QuadPart)
    {
        sg_TimeStamp = (sg_QueryTickCount.QuadPart * 10000) / sg_lnFrequency.QuadPart;
    }
    else
    {
        sg_TimeStamp = (sg_QueryTickCount.QuadPart / sg_lnFrequency.QuadPart) * 10000;
    }
}


static BOOL bLoadDataFromContr(PSCONTROLLER_DETAILS pControllerDetails)
{
    BOOL bReturn = FALSE;
    // If successful
    if (pControllerDetails != nullptr)
    {
        for( int nIndex = 0; nIndex < defNO_OF_CHANNELS; nIndex++ )
        {
            char* pcStopStr = nullptr;
            CChannel& odChannel = sg_odHardwareNetwork.m_aodChannels[ nIndex ];

            // Get Warning Limit
            odChannel.m_ucWarningLimit = static_cast <UCHAR>(
                                             _tcstol( pControllerDetails[ nIndex ].m_omStrWarningLimit.c_str(),
                                                     &pcStopStr, defBASE_DEC ));

            odChannel.m_bCNF1 = static_cast <UCHAR>(
                                    _tcstol( pControllerDetails[ nIndex ].m_omStrCNF1.c_str(),
                                             &pcStopStr, defBASE_HEX));
            odChannel.m_bCNF2 = static_cast <UCHAR>(
                                    _tcstol( pControllerDetails[ nIndex ].m_omStrCNF2.c_str(),
                                             &pcStopStr, defBASE_HEX));
            odChannel.m_bCNF3 = static_cast <UCHAR>(
                                    _tcstol( pControllerDetails[ nIndex ].m_omStrCNF3.c_str(),
                                             &pcStopStr, defBASE_HEX));

            // Get Baud Rate
            odChannel.m_usBaudRate = static_cast <USHORT>(
                                         pControllerDetails[ nIndex ].m_nBTR0BTR1 );

            odChannel.m_unBaudrate = _tstoi( pControllerDetails[ nIndex ].m_omStrBaudrate.c_str());

            odChannel.m_ucControllerState = pControllerDetails[ nIndex ].m_ucControllerMode;
        }
        // Get Controller Mode
        // Consider only the first channel mode as controller mode
        //sg_ucControllerMode = pControllerDetails[ 0 ].m_ucControllerMode;

        bReturn = TRUE;
    }
    return bReturn;
}

/**
 * \param[in]  odChannel The current channel
 * \param[out] sCanData Application specific data format
 * \param[in]  ushRxErr Number of Rx error
 * \param[in]  ushTxErr Number of Tx error
 *
 * Based on the number of Rx and Tx error, this will take the
 * action necessary.
 */
static void vProcessError(STCANDATA& sCanData, CChannel& odChannel,
                          USHORT ushRxErr, USHORT ushTxErr)
{
    sCanData.m_uDataInfo.m_sErrInfo.m_ucErrType =
        USB_ucHandleErrorCounter(0, (UCHAR)ushRxErr, (UCHAR)ushTxErr);

    // Update Error Type
    if (sCanData.m_uDataInfo.m_sErrInfo.m_ucErrType == ERROR_INTERRUPT)
    {
        // This is interrupt message. So change the type
        sCanData.m_uDataInfo.m_sErrInfo.m_ucReg_ErrCap = ERROR_UNKNOWN;
    }

    // Update Rx Error Counter Value & Tx Error Counter Value
    odChannel.vUpdateErrorCounter((UCHAR)ushTxErr, (UCHAR)ushRxErr);
}

/**
 * \param[in]  CurrSpyMsg Message polled from the bus in neoVI format
 * \param[out] sCanData Application specific data format
 * \param[in]  unChannel channel
 * \return TRUE (always)
 *
 * This will classify the messages, which can be one of Rx, Tx or
 * Error messages. In case of Err messages this identifies under
 * what broader category (Rx / Tx) does this occur.
 */
static BYTE bClassifyMsgType(icsSpyMessage& CurrSpyMsg,
                             STCANDATA& sCanData, UINT unChannel)
{
    if (CurrSpyMsg.StatusBitField & VALUECAN_ERROR_BITS)
    {
        sCanData.m_ucDataType = ERR_FLAG;
        // Set bus error as default error. This will be
        // Modified by the function USB_ucHandleErrorCounter
        sCanData.m_uDataInfo.m_sErrInfo.m_ucErrType = ERROR_BUS;
        // Assign the channel number
        sCanData.m_uDataInfo.m_sErrInfo.m_ucChannel = (UCHAR)unChannel;
        // Assign error type in the Error Capture register
        // and the direction of the error
        BOOL bIsTxMsg = FALSE;
        if (CurrSpyMsg.StatusBitField & SPY_STATUS_TX_MSG)
        {
            bIsTxMsg = TRUE;
        }
        /*Set CAN FD to false*/
        sCanData.m_uDataInfo.m_sCANMsg.m_bCANFD = false;
        sCanData.m_uDataInfo.m_sErrInfo.m_ucReg_ErrCap =
            USB_ucGetErrorCode(CurrSpyMsg.StatusBitField, (BYTE) bIsTxMsg);
        //explaination of error bit
        sCanData.m_uDataInfo.m_sErrInfo.m_nSubError= 0;
        // Update error counter values
        if (bIsTxMsg)
        {
            m_unTxError++;
        }
        else
        {
            m_unRxError++;
        }
        sCanData.m_uDataInfo.m_sErrInfo.m_ucRxErrCount = m_unRxError;
        sCanData.m_uDataInfo.m_sErrInfo.m_ucTxErrCount = m_unTxError;
    }
    else if (CurrSpyMsg.StatusBitField & SPY_STATUS_CAN_BUS_OFF)
    {
        sCanData.m_ucDataType = ERR_FLAG;

        // Set bus error as default error. This will be
        // Modified by the function USB_ucHandleErrorCounter
        sCanData.m_uDataInfo.m_sErrInfo.m_ucErrType = ERROR_BUS;
        // Assign the channel number
        sCanData.m_uDataInfo.m_sErrInfo.m_ucChannel = (UCHAR)unChannel;

        for (USHORT j = 0x01; j <= 0x80; j <<= 1)
        {
            if (CurrSpyMsg.Data[0] & j)
            {
                UCHAR ucRxCnt = sCanData.m_uDataInfo.m_sErrInfo.m_ucRxErrCount;
                UCHAR ucTxCnt = sCanData.m_uDataInfo.m_sErrInfo.m_ucTxErrCount;
                switch (j)
                {
                    case 0x01:
                    {
                        ucRxCnt = (ucRxCnt < 96) ? 96 : ucRxCnt;
                        ucTxCnt = (ucTxCnt < 96) ? 96 : ucTxCnt;
                    }
                    break;
                    case 0x02:
                        ucRxCnt = (ucRxCnt < 96) ? 96 : ucRxCnt;
                        break;
                    case 0x04:
                        ucTxCnt = (ucTxCnt < 96) ? 96 : ucTxCnt;
                        break;
                    case 0x08:
                        ucTxCnt = (ucTxCnt < 128) ? 128 : ucTxCnt;
                        break;
                    case 0x10:
                        ucRxCnt = (ucRxCnt < 128) ? 128 : ucRxCnt;
                        break;
                    case 0x20:
                        ucTxCnt = (ucTxCnt < 255) ? 255 : ucTxCnt;
                        break;
                    case 0x40:
                    case 0x80:
                    default:
                        break;
                }
                sCanData.m_uDataInfo.m_sErrInfo.m_ucRxErrCount = ucRxCnt;
                sCanData.m_uDataInfo.m_sErrInfo.m_ucTxErrCount = ucTxCnt;
            }
        }
    }
    else
    {
        //Check for RTR Message
        if (CurrSpyMsg.StatusBitField & SPY_STATUS_REMOTE_FRAME)
        {
            sCanData.m_ucDataType = RX_FLAG;
            sCanData.m_uDataInfo.m_sCANMsg.m_ucRTR = TRUE;
        }
        else
        {
            sCanData.m_uDataInfo.m_sCANMsg.m_ucRTR = FALSE;
        }
        if (CurrSpyMsg.StatusBitField & SPY_STATUS_TX_MSG)
        {
            sCanData.m_ucDataType = TX_FLAG;
        }
        else if (CurrSpyMsg.StatusBitField & SPY_STATUS_NETWORK_MESSAGE_TYPE)
        {
            sCanData.m_ucDataType = RX_FLAG;
        }
        /*else
        {
            //ASSERT(false);
        }*/
        // Copy data length
        sCanData.m_uDataInfo.m_sCANMsg.m_ucDataLen = CurrSpyMsg.NumberBytesData;

        // Copy the message data
        memcpy(sCanData.m_uDataInfo.m_sCANMsg.m_ucData,
               CurrSpyMsg.Data, CurrSpyMsg.NumberBytesData);

        // Copy the message ID
        memcpy(&(sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID),
               &(CurrSpyMsg.ArbIDOrHeader), sizeof(UINT));

        // Check for extended message indication
        sCanData.m_uDataInfo.m_sCANMsg.m_ucEXTENDED =
            (CurrSpyMsg.StatusBitField & SPY_STATUS_XTD_FRAME) ? TRUE : FALSE;


    }
    return TRUE;
}

/**
 * \param[in] psCanDataArray Pointer to CAN Message Array of Structures
 * \param[in] nMessage Maximun number of message to read or size of the CAN Message Array
 * \param[out] Message Actual Messages Read
 * \return Returns S_OK if successful otherwise corresponding Error code.
 *
 * This function will read multiple CAN messages from the driver.
 * The other fuctionality is same as single message read. This
 * will update the variable nMessage with the actual messages
 * read.
 */
static int nReadMultiMessage(PSTCANDATA psCanDataArray,
                             int& nMessage, int nChannelIndex)
{
    int i = 0;
    int nReturn = 0;
    // Now the data messages
    CChannel& odChannel = sg_odHardwareNetwork.m_aodChannels[nChannelIndex];
    static int s_CurrIndex = 0, s_Messages = 0;
    static icsSpyMessage s_asSpyMsg[MAX_BUFFER_VALUECAN] = {0};
    int nErrMsg = 0;
    if (s_CurrIndex == 0)
    {
        nReturn = (*icsneoGetMessages)(m_anhObject[nChannelIndex][0], s_asSpyMsg,
                                       &s_Messages, &nErrMsg);
        //TRACE("s_Messages: %d  nErrMsg: %d nMessage: %d\n", s_Messages,
        //    nErrMsg, nMessage);
    }

    // Start of first level of error message processing
    USHORT ushRxErr = 0, ushTxErr = 0;
    if (nErrMsg > 0)
    {
        int nErrors = 0;
        nReturn = (*icsneoGetErrorMessages)(m_anhObject[nChannelIndex][0], s_anErrorCodes, &nErrors);
        if ((nReturn == NEOVI_OK) && (nErrors > 0))
        {
            for (int j = 0; j < nErrors; j++)
            {
                switch (s_anErrorCodes[j])
                {
                    case 2: // NEOVI_ERROR_DLL_USB_SEND_DATA_ERROR:
                    {
                        ++ushTxErr;
                    }
                    break;
                    case 39: // NEOVI_ERROR_DLL_RX_MSG_FRAME_ERR
                    case 40: // NEOVI_ERROR_DLL_RX_MSG_FIFO_OVER:
                    case 41: // NEOVI_ERROR_DLL_RX_MSG_CHK_SUM_ERR:
                    {
                        ++ushRxErr;
                    }
                    break;
                    default:
                    {
                        // Do nothing until further clarification is received
                    }
                    break;
                }
            }
        }
    }
    // End of first level of error message processing

    // START
    /* Create the time stamp map. This means getting the local time and assigning
    offset value to the QuadPart.
    */
    static LONGLONG QuadPartRef = 0;

    if (CREATE_MAP_TIMESTAMP == sg_byCurrState)
    {
        //ASSERT(s_CurrIndex < s_Messages);

        //CTimeManager::bReinitOffsetTimeValForICSneoVI();
        /*icsSpyMessage& CurrSpyMsg = s_asSpyMsg[s_CurrIndex];
        DOUBLE dTimestamp = 0;
        nReturn = (*icsneoGetTimeStampForMsg)(m_anhObject[nChannelIndex][0], &CurrSpyMsg, &dTimestamp);
        if (nReturn == NEOVI_OK)
        {
            QuadPartRef = (LONGLONG)(dTimestamp * 10000);//CurrSpyMsg.TimeHardware2 * 655.36 + CurrSpyMsg.TimeHardware * 0.01;

            sg_byCurrState = CALC_TIMESTAMP_READY;
            nReturn = S_OK;
            sg_TimeStamp =  ((LONGLONG)((CurrSpyMsg.TimeHardware2* 104.8576)/1000 + CurrSpyMsg.TimeHardware * 1.6))/10000;
            //sg_TimeStamp /= 10;
            //sg_TimeStamp = (LONGLONG)(CurrSpyMsg.TimeSystem * 10);
        }
        else
        {
            nReturn = -1;
        }*/
        icsSpyMessage& CurrSpyMsg = s_asSpyMsg[s_CurrIndex];
        DOUBLE dTimestamp = 0;
        nReturn = (*icsneoGetTimeStampForMsg)(m_anhObject[nChannelIndex][0], &CurrSpyMsg, &dTimestamp);
        if (nReturn == NEOVI_OK)
        {
            sg_byCurrState = CALC_TIMESTAMP_READY;
            nReturn = S_OK;

            LARGE_INTEGER g_QueryTickCount;
            QueryPerformanceCounter(&g_QueryTickCount);
            UINT64 unConnectionTime;
            unConnectionTime = ((g_QueryTickCount.QuadPart * 10000) / sg_lnFrequency.QuadPart) - sg_TimeStamp;

            //Time difference should be +ve value
            if((dTimestamp * 10000) >= unConnectionTime)
            {
                sg_TimeStamp  = (LONGLONG)((dTimestamp * 10000) - unConnectionTime);
            }
            else
            {
                sg_TimeStamp  = (LONGLONG)(unConnectionTime - (dTimestamp * 10000));
            }

        }
        else
        {
            nReturn = -1;
        }
    }

    // END

    int nLimForAppBuf = nMessage;//MIN(nMessage, s_Messages);
    bool bChannelCnfgrd = false;
    for (/*int i = 0*/; (i < nLimForAppBuf) && (s_CurrIndex < s_Messages); )
    {
        STCANDATA& sCanData = psCanDataArray[i];
        icsSpyMessage& CurrSpyMsg = s_asSpyMsg[s_CurrIndex];

        //sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel = (UCHAR)(nCurrChannel + 1);
        if ( m_anhObject[nChannelIndex][CurrSpyMsg.NetworkID+1] != 0 )
        {
            bChannelCnfgrd = true; // Set channel configured flag to true.
            sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel = (UCHAR)(m_anhObject[nChannelIndex][CurrSpyMsg.NetworkID+1]  );

            //sCanData.m_lTickCount.QuadPart = (LONGLONG)(CurrSpyMsg.TimeSystem * 10);

            DOUBLE dTimestamp = 0;
            (void) (*icsneoGetTimeStampForMsg)(m_anhObject[nChannelIndex][0], &CurrSpyMsg, &dTimestamp);
            sCanData.m_lTickCount.QuadPart = (dTimestamp*10000); //+ sg_TimeStamp// + (CurrSpyMsg.TimeHardware2* 0.1048576 + CurrSpyMsg.TimeHardware2 * 0.0000016);
            bClassifyMsgType(CurrSpyMsg, sCanData, sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel);

            if (sCanData.m_ucDataType == ERR_FLAG)
            {
                vProcessError(sCanData, odChannel, ushRxErr + 1, ushTxErr + 1);
                // Reset to zero
                ushRxErr = 0;
                ushTxErr = 0;
            }
        }
        else
        {
            bChannelCnfgrd = false;    // Set channel configured flag to false.
        }
        s_CurrIndex++;
        if ( bChannelCnfgrd )
        {
            i++;
        }
    }
    //TRACE("s_CurrIndex: %d i: %d\n", s_CurrIndex, i);

    //ASSERT(!(s_CurrIndex > MAX_BUFFER_VALUECAN));
    //ASSERT(!(s_CurrIndex > s_Messages));

    if ((s_CurrIndex == MAX_BUFFER_VALUECAN) || (s_CurrIndex == s_Messages))
    {
        s_CurrIndex = 0;
        s_Messages = 0;
    }

    // This code is needed when error messages don't occur in the list of
    // the regular message
    if ((ushRxErr != 0) || (ushTxErr != 0))
    {
        STCANDATA sCanData;
        vProcessError(sCanData, odChannel, ushRxErr, ushTxErr);
    }


    nMessage = i;

    return S_OK; // Multiple return statements had to be added because
    // neoVI specific codes and simulation related codes need to coexist.
    // Code for the later still employs Peak API interface.
}

static BOOL bGetClientObj(DWORD dwClientID, UINT& unClientIndex)
{
    BOOL bResult = FALSE;
    for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        if (sg_asClientToBufMap[i].dwClientID == dwClientID)
        {
            unClientIndex = i;
            i = sg_unClientCnt; //break the loop
            bResult = TRUE;
        }
    }
    return bResult;
}

static BOOL bRemoveClient(DWORD dwClientId)
{
    BOOL bResult = FALSE;
    if (sg_unClientCnt > 0)
    {
        UINT unClientIndex = (UINT)-1;
        if (bGetClientObj(dwClientId, unClientIndex))
        {
            //clear the client first
            if (sg_asClientToBufMap[unClientIndex].hClientHandle != 0)
            {
                HRESULT hResult = S_OK;//(*pfCAN_RemoveClient)(sg_asClientToBufMap[unClientIndex].hClientHandle);
                if (hResult == S_OK)
                {
                    sg_asClientToBufMap[unClientIndex].dwClientID = 0;
                    sg_asClientToBufMap[unClientIndex].hClientHandle = 0;
                    memset (sg_asClientToBufMap[unClientIndex].pacClientName, 0, sizeof (char) * MAX_PATH);
                    for (int i = 0; i < MAX_BUFF_ALLOWED; i++)
                    {
                        sg_asClientToBufMap[unClientIndex].pClientBuf[i] = nullptr;
                    }
                    sg_asClientToBufMap[unClientIndex].unBufCount = 0;
                    bResult = TRUE;
                }
                else
                {
                    vRetrieveAndLog(hResult, __FILE__, __LINE__);
                }
            }
            else
            {
                sg_asClientToBufMap[unClientIndex].dwClientID = 0;
                memset (sg_asClientToBufMap[unClientIndex].pacClientName, 0, sizeof (char) * MAX_PATH);
                for (int i = 0; i < MAX_BUFF_ALLOWED; i++)
                {
                    sg_asClientToBufMap[unClientIndex].pClientBuf[i] = nullptr;
                }
                sg_asClientToBufMap[unClientIndex].unBufCount = 0;
                bResult = TRUE;

            }
            if (bResult == TRUE)
            {
                if ((unClientIndex + 1) < sg_unClientCnt)
                {
                    sg_asClientToBufMap[unClientIndex] = sg_asClientToBufMap[sg_unClientCnt - 1];
                }
                sg_unClientCnt--;
            }
        }
    }
    return bResult;
}

static BOOL bRemoveClientBuffer(CBaseCANBufFSE* RootBufferArray[MAX_BUFF_ALLOWED], UINT& unCount, CBaseCANBufFSE* BufferToRemove)
{
    BOOL bReturn = TRUE;
    for (UINT i = 0; i < unCount; i++)
    {
        if (RootBufferArray[i] == BufferToRemove)
        {
            if (i < (unCount - 1)) //If not the last bufffer
            {
                RootBufferArray[i] = RootBufferArray[unCount - 1];
            }
            unCount--;
        }
    }
    return bReturn;
}

void vMarkEntryIntoMap(const SACK_MAP& RefObj)
{
    EnterCriticalSection(&sg_CritSectForAckBuf); // Lock the buffer
    sg_asAckMapBuf.push_back(RefObj);
    LeaveCriticalSection(&sg_CritSectForAckBuf); // Unlock the buffer
}

BOOL bRemoveMapEntry(const SACK_MAP& RefObj, UINT& ClientID)
{
    EnterCriticalSection(&sg_CritSectForAckBuf); // Lock the buffer
    BOOL bResult = FALSE;
    CACK_MAP_LIST::iterator  iResult =
        std::find( sg_asAckMapBuf.begin(), sg_asAckMapBuf.end(), RefObj );
    //if ((*iResult).m_ClientID > 0)
    if (iResult != sg_asAckMapBuf.end())
    {
        bResult = TRUE;
        ClientID = (*iResult).m_ClientID;
        sg_asAckMapBuf.erase(iResult);
    }
    LeaveCriticalSection(&sg_CritSectForAckBuf); // Unlock the buffer
    return bResult;
}

/**
 * This function writes the message to the corresponding clients buffer
 */
static void vWriteIntoClientsBuffer(STCANDATA& sCanData)
{
    //Write into the client's buffer and Increment message Count
    if (sCanData.m_ucDataType == TX_FLAG)
    {
        static SACK_MAP sAckMap;
        UINT ClientId = 0;
        static UINT Index = (UINT)-1;
        sAckMap.m_Channel = sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel;
        sAckMap.m_MsgID = sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID;

        if (bRemoveMapEntry(sAckMap, ClientId))
        {
            BOOL bClientExists = bGetClientObj(ClientId, Index);
            for (UINT i = 0; i < sg_unClientCnt; i++)
            {
                //Tx for sender node
                if (/*(i == CAN_MONITOR_NODE_INDEX)  || */(bClientExists && (i == Index)))
                {
                    for (UINT j = 0; j < sg_asClientToBufMap[i].unBufCount; j++)
                    {
                        sg_asClientToBufMap[i].pClientBuf[j]->WriteIntoBuffer(&sCanData);
                    }
                }
                else
                {
                    //Send the other nodes as Rx.
                    for (UINT j = 0; j < sg_asClientToBufMap[i].unBufCount; j++)
                    {
                        static STCANDATA sTempCanData;
                        sTempCanData = sCanData;
                        sTempCanData.m_ucDataType = RX_FLAG;
                        sg_asClientToBufMap[i].pClientBuf[j]->WriteIntoBuffer(&sTempCanData);
                    }
                }
            }
        }
    }
    else // provide it to everybody
    {
        for (UINT i = 0; i < sg_unClientCnt; i++)
        {
            for (UINT j = 0; j < sg_asClientToBufMap[i].unBufCount; j++)
            {
                sg_asClientToBufMap[i].pClientBuf[j]->WriteIntoBuffer(&sCanData);
            }
        }
    }
}

/**
 * Processing of the received packets from bus
 */
static void ProcessCANMsg(int nChannelIndex)
{
    int nSize = sg_nFRAMES;
    if (nReadMultiMessage(sg_asMsgBuffer, nSize, nChannelIndex) == S_OK)
    {
        for (INT unCount = 0; unCount < nSize; unCount++)
        {
            vWriteIntoClientsBuffer(sg_asMsgBuffer[unCount]);
        }
    }
}

/**
 * Read thread procedure
 */
DWORD WINAPI CanMsgReadThreadProc_CAN_ICS_neoVI(LPVOID pVoid)
{
    USES_CONVERSION;

    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;

    // Validate certain required pointers
    VALIDATE_POINTER_RETURN_VAL(pThreadParam, (DWORD)-1);
    // Assign thread action to CREATE_TIME_MAP
    pThreadParam->m_unActionCode = CREATE_TIME_MAP;
    // Set the event to CAN_ICS_neoVI driver for wakeup and frame arrival notification
    nCreateAndSetReadIndicationEvent(pThreadParam->m_hActionEvent);

    // Get the handle to the controller and validate it
    VALIDATE_POINTER_RETURN_VAL(pThreadParam->m_hActionEvent, (DWORD)-1);

    DWORD dwResult = 0;
    bool bLoopON = true;

    while (bLoopON)
    {
        for (UINT i = 0; i < sg_odHardwareNetwork.m_nNoOfDevices && bLoopON ; i++)
        {
            dwResult = (*icsneoWaitForRxMessagesWithTimeOut)(m_anhObject[i][0], WAITTIME_NEOVI);
            //kadoor WaitForSingleObject(pThreadParam->m_hActionEvent, INFINITE);
            if (dwResult > 0)
            {
                switch (pThreadParam->m_unActionCode)
                {
                    case INVOKE_FUNCTION:
                    {
                        ProcessCANMsg(i); // Retrieve message from the driver
                    }
                    break;
                    case CREATE_TIME_MAP:
                    {
                        sg_byCurrState = CREATE_MAP_TIMESTAMP;
                        SetEvent(pThreadParam->m_hActionEvent);
                        //vCreateTimeModeMapping(pThreadParam->m_hActionEvent);
                        ProcessCANMsg(i);
                        pThreadParam->m_unActionCode = INVOKE_FUNCTION;
                    }
                    break;
                    case EXIT_THREAD:
                    {
                        bLoopON = false;
                    }
                    break;
                    default:
                    case INACTION:
                    {
                        // nothing right at this moment
                    }
                    break;
                }
            }
        }
    }

    SetEvent(pThreadParam->hGetExitNotifyEvent());

    // Close the notification event for startup
    CloseHandle(pThreadParam->m_hActionEvent);
    pThreadParam->m_hActionEvent = nullptr;

    return 0;
}

int ulGetESSerialNum(unsigned long DeviceType, int serialNumber, int nHardwareLic)
{
    switch (DeviceType)
    {
            /* neoVI Blue */
        case NEODEVICE_BLUE:
            return serialNumber;
            break;

            /* ValueCAN */
        case NEODEVICE_DW_VCAN:
            return serialNumber;
            break;

            /* neoVI Fire/Red */
        case NEODEVICE_FIRE:
            return serialNumber;
            break;

            /* ValueCAN3 and ETAS ES581 */
        case NEODEVICE_VCAN3:
            if ((serialNumber >= 80000) && (serialNumber <= 119999))
            {
                if (nHardwareLic == 8)   // Limited Version with only one channel
                {
                    return serialNumber-50000;
                }
                else     // Two channels
                {
                    return serialNumber-50000;
                }
            }
            else
            {
                return serialNumber;
            }
            break;
        default:
            return serialNumber;
            break;
    };
}
std::string omGetDeviceType(int /*i*/, unsigned long ulDeviceType, unsigned int serialNumber, int nHardwareLic, int nNetworkId)
{
    CHAR chTemp[MAX_PATH];
    char netid_str[256];

    switch (ulDeviceType)
    {
            /* neoVI Blue */
        case NEODEVICE_BLUE:
            /* neoVI Fire/Red */
        case NEODEVICE_FIRE:
            switch (nNetworkId)
            {
                case NETID_HSCAN:
                    strncpy(&netid_str[0], "HSCAN", sizeof(netid_str));
                    break;
                case NETID_MSCAN:
                    strncpy(&netid_str[0], "MSCAN", sizeof(netid_str));
                    break;
                case NETID_SWCAN:
                    strncpy(&netid_str[0], "SW CAN", sizeof(netid_str));
                    break;
                case NETID_LSFTCAN:
                    strncpy(&netid_str[0], "LSFT CAN", sizeof(netid_str));
                    break;
            }
            break;

            /* ValueCAN */
        case NEODEVICE_DW_VCAN:
            switch (nNetworkId)
            {
                case NETID_HSCAN:
                    strncpy(&netid_str[0], "CAN", sizeof(netid_str));
                    break;
            }
            break;

            /* ValueCAN3 */
        case NEODEVICE_VCAN3:
            if (nHardwareLic == 8)   // Limited Version with only one channel
            {
                switch (nNetworkId)
                {
                    case NETID_HSCAN:
                        strncpy(&netid_str[0], "CAN", sizeof(netid_str));
                        break;
                }
            }
            else     // Two channels
            {
                switch (nNetworkId)
                {
                    case NETID_HSCAN:
                        strncpy(&netid_str[0], "CAN 1", sizeof(netid_str));
                        break;
                    case NETID_MSCAN:
                        strncpy(&netid_str[0], "CAN 2", sizeof(netid_str));
                        break;
                }
            }
            break;
    }

    /* default case */
    if (strlen(&netid_str[0]) == 0)
    {
        switch (nNetworkId)
        {
            case NETID_HSCAN:
                strncpy(&netid_str[0], "HSCAN", sizeof(netid_str));
                break;
            case NETID_MSCAN:
                strncpy(&netid_str[0], "MSCAN", sizeof(netid_str));
                break;
            case NETID_SWCAN:
                strncpy(&netid_str[0], "SWCAN", sizeof(netid_str));
                break;
            case NETID_LSFTCAN:
                strncpy(&netid_str[0], "LSFTCAN", sizeof(netid_str));
                break;
        }
    }

    switch (ulDeviceType)
    {
            /* neoVI Blue */
        case NEODEVICE_BLUE:
            sprintf_s(chTemp, sizeof(chTemp), "neoVI Blue, Serial Number %d, Network: %s",
                      serialNumber, &netid_str[0]);

            break;

            /* ValueCAN */
        case NEODEVICE_DW_VCAN:
            _stprintf_s(chTemp, sizeof(chTemp), "ValueCAN, Serial Number %d, Network: %s",
                        serialNumber, &netid_str[0]);

            break;

            /* neoVI Fire/Red */
        case NEODEVICE_FIRE:
            _stprintf_s(chTemp, sizeof(chTemp), "neoVi Fire/Red, Serial Number %d, Network: %s",
                        serialNumber, &netid_str[0]);

            break;

            /* ValueCAN3 and ETAS ES581 */
        case NEODEVICE_VCAN3:
            if ((serialNumber >= 80000) && (serialNumber <= 119999))
            {
                if (nHardwareLic == 8)   // Limited Version with only one channel
                {
                    _stprintf_s(chTemp, sizeof(chTemp), "ES581.2, Serial Number %d, Network: %s",
                                serialNumber-50000, &netid_str[0]);

                }
                else     // Two channels
                {
                    _stprintf_s(chTemp, sizeof(chTemp), "ES581.3, Serial Number %d, Network: %s",
                                serialNumber-50000, &netid_str[0]);

                }
            }
            else
            {
                _stprintf_s(chTemp, sizeof(chTemp), "ValueCAN3, Serial Number %d, Network: %s",
                            serialNumber, &netid_str[0]);
            }
            break;

        default:
            _stprintf_s(chTemp, sizeof(chTemp), "Unknown, Serial Number %d", serialNumber);
            //chTemp = "Unknown";
            break;
    };

    return chTemp;
}
/**
 * This function will add channels to hardware inteface structure.
 */
static int nAddChanneltoHWInterfaceList(int narrNetwordID[], int nCntNtwIDs, int& nChannels, const int nDevID)
{
    int nResult = 0;
    int hObject = 0;
    std::stringstream acFirmware;
	
    nResult = (*icsneoOpenNeoDevice)(&sg_ndNeoToOpen[nDevID], &hObject, nullptr, 1, 0);
    int nHardwareLic = 0;
    if (nResult == NEOVI_OK && hObject != 0)
    {
		stAPIFirmwareInfo *objstFWInfo = new stAPIFirmwareInfo();
        int nErrors = 0;
        if ( icsneoGetHardwareLicense )
        {
			
            (*icsneoGetHardwareLicense)(hObject, &nHardwareLic);
        }
        if ( icsneoGetHWFirmwareInfo )
        {
			
            nResult = (*icsneoGetHWFirmwareInfo)(hObject, objstFWInfo);
            if ( nResult == 1 )
            {
				if (nullptr != objstFWInfo)
				{
					acFirmware << std::dec << (unsigned short)objstFWInfo->iAppMajor;
					acFirmware << '.';
					acFirmware << std::dec << (unsigned short)objstFWInfo->iAppMinor;
				}                
            }
        }
		
        (*icsneoClosePort)(hObject, &nErrors);
    }

	
    for ( int i=0; i<nCntNtwIDs; i++ )
    {
        sg_HardwareIntr[nChannels].m_dwIdInterface = sg_ndNeoToOpen[nDevID].Handle;
        sg_HardwareIntr[nChannels].m_dwVendor = sg_ndNeoToOpen[nDevID].SerialNumber;
        sg_HardwareIntr[nChannels].m_bytNetworkID = narrNetwordID[i];
        sg_HardwareIntr[nChannels].m_acDeviceName = acFirmware.str();

        sg_HardwareIntr[nChannels].m_acNameInterface = std::to_string(sg_ndNeoToOpen[nDevID].DeviceType);

        int ulSerialNumber = ulGetESSerialNum(sg_ndNeoToOpen[nDevID].DeviceType, sg_HardwareIntr[nChannels].m_dwVendor, nHardwareLic);

        std::stringstream acTempStr;
        acTempStr << "SN: " << std::dec << ulSerialNumber << ", ";
        acTempStr << "Port ID: " << sg_HardwareIntr[nChannels].m_dwIdInterface << "-CAN" << std::dec << narrNetwordID[i];
        sg_HardwareIntr[nChannels].m_acDescription = acTempStr.str();

        sg_HardwareList.m_omHardwareChannel[nChannels] = omGetDeviceType(nChannels, sg_ndNeoToOpen[nDevID].DeviceType
                , sg_ndNeoToOpen[nDevID].SerialNumber, nHardwareLic, sg_HardwareIntr[nChannels].m_bytNetworkID);
        nChannels++;
    }
	
    return nResult;
}

/**
 * This is USB Specific Function. This will create a single
 * network with available single hardware.
 */
static int nCreateSingleHardwareNetwork()
{
    // Create network here with net handle 1
    s_DatIndThread.m_bToContinue = TRUE;
    s_DatIndThread.m_bIsConnected = FALSE;
    s_DatIndThread.m_unChannels = 1;

    int hObject = 0;
    int nResult = (*icsneoOpenNeoDevice)(&sg_ndNeoToOpen[0], &hObject, nullptr, 1, 0);

    //If connection fails
    if ( nResult != NEOVI_OK)
    {
        return -1;
    }
    (*icsneoClosePort)(hObject, &nResult);

    // Set the number of channels
    sg_odHardwareNetwork.m_nNoOfChannels = 1;
    sg_odHardwareNetwork.m_nNoOfDevices  = 1;
    sg_nNoOfChannels = 1;
    // Assign hardware handle
    sg_odHardwareNetwork.m_aodChannels[ 0 ].m_hHardwareHandle = (BYTE)sg_ndNeoToOpen[0].Handle;

    // Assign Net Handle
    sg_odHardwareNetwork.m_aodChannels[ 0 ].m_hNetworkHandle = m_bytNetworkIDs[0] = NETID_HSCAN;


    return S_OK;
}

/**
* \brief         This function will popup hardware selection dialog and gets the user selection of channels.
* \param[in]     psInterfaces, is INTERFACE_HW structue
* \param[out]    pnSelList, contains channels selected array
* \param[out]    nCount, contains selected channel count
* \return        returns 0 if success, else -1
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
int ListHardwareInterfaces(HWND hParent, INTERFACE_HW* psInterfaces, int* pnSelList, int& nCount, PSCONTROLLER_DETAILS InitData)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CWnd objMainWnd;
    objMainWnd.Attach(hParent);
	IChangeRegisters* pAdvancedSettings = new CChangeRegisters_CAN_ICS_neoVI (nullptr, InitData, nCount);
	CHardwareListingCAN HwList(psInterfaces, nCount, pnSelList, CAN, CHANNEL_ALLOWED,  &objMainWnd, InitData, pAdvancedSettings);
    INT nRet = HwList.DoModal();
    objMainWnd.Detach();

    if ( nRet == IDOK)
    {
        nCount = HwList.nGetSelectedList(pnSelList);
        return 0;
    }
    else
    {
        return -1;
    }
}

unsigned int GetSelectedChannelIndex(unsigned int unIndex)
{
    int nSelChannel = -1;
    std::string omHardwareName;
    for(int nChannel = 0; nChannel < defNO_OF_CHANNELS; nChannel++)
    {
        /*std::ostringstream oss1;
        oss1 << "Kvaser - " << sg_HardwareIntr[nChannel].m_acDescription.c_str() << ", Serial Number- "
            <<sg_HardwareIntr[nChannel].m_dwVendor
            << ", Firmware- " << sg_HardwareIntr[nChannel].m_acDeviceName.c_str();
        omHardwareName = oss1.str();*/

        if(sg_HardwareList.m_omHardwareChannel[nChannel] == sg_SelectedChannels.m_omHardwareChannel[unIndex]
                && sg_HardwareIntr[nChannel].m_acDescription.c_str() != "")
        {
            nSelChannel = nChannel;
            break;
        }
    }

    return nSelChannel;
}
/**
 * This function will get the hardware selection from the user
 * and will create essential networks.
 */
static int nCreateMultipleHardwareNetwork(PSCONTROLLER_DETAILS InitData, UINT unDefaultChannelCnt = 0, bool bAutoSelect = false)
{
    int nHardwareCountPrev = sg_ucNoOfHardware;
    int nHwCount = sg_ucNoOfHardware;
    int nChannels = 0;
	
    // Get Hardware Network Map
    for (int nCount = 0; nCount < nHwCount; nCount++)
    {
        NeoDevice* pNeoDevice = &sg_ndNeoToOpen[nCount];
        switch (pNeoDevice->DeviceType)
        {
                /* neoVI Blue - 4 channels*/
            case NEODEVICE_BLUE:
            {
                int narrBlueNtwID[] = {NETID_HSCAN, NETID_MSCAN, NETID_SWCAN, NETID_LSFTCAN};
                nAddChanneltoHWInterfaceList(narrBlueNtwID, 4, nChannels, nCount);
            }
            break;

            /* ValueCAN - 1 channel*/
            case NEODEVICE_DW_VCAN:
            {
                int narrVCANNtwID[] = {NETID_HSCAN};
                nAddChanneltoHWInterfaceList(narrVCANNtwID, 1, nChannels, nCount);
            }
            break;

            /* neoVI Fire/Red - 4 channels*/
            case NEODEVICE_FIRE:
            {
                int narrFIRENtwID[] = {NETID_HSCAN, NETID_MSCAN, NETID_SWCAN, NETID_LSFTCAN};
                nAddChanneltoHWInterfaceList(narrFIRENtwID, 4, nChannels, nCount);
            }
            break;

            /* ValueCAN3 - 2/1 channels*/
            case NEODEVICE_VCAN3:
            {
				
                int narrVCAN3NtwID[] = {NETID_HSCAN, NETID_MSCAN};
                int nCntNtwIDs = 2;
                int hObject = 0;
                int nResult = 0;				
                nResult = (*icsneoOpenNeoDevice)(pNeoDevice, &hObject, nullptr, 1, 0);
				
                if (nResult == NEOVI_OK && hObject!=0)
                {
                   // stAPIFirmwareInfo objstFWInfo;					
                    //(void) (*icsneoGetHWFirmwareInfo)(hObject, &objstFWInfo);
				
                    int nHardwareLic = 0;
                    int nErrors = 0;
                    if ( icsneoGetHardwareLicense )
                    {						
                        (*icsneoGetHardwareLicense)(hObject, &nHardwareLic);
                    }					
					
                    (*icsneoClosePort)(hObject, &nErrors);
					
                    // Check if it a Limited version with only one channel support.
                    if ( nHardwareLic == 8 )    // Single channel
                    {
                        nCntNtwIDs = 1;
                        if ( nHwCount == 1 )    //If only one device connected
                        {							
                            nCreateSingleHardwareNetwork();
							
                            return S_OK;
                        }
                    }
					nAddChanneltoHWInterfaceList(narrVCAN3NtwID, nCntNtwIDs, nChannels, nCount);					
                }
            }
            break;

            default:
            {
                /* Let's assume at least one channel... */
                int narrNetwordID[] = {NETID_HSCAN};
                nAddChanneltoHWInterfaceList(narrNetwordID, 1, nChannels, nCount);
            }
            break;
        }
    }

    if ( nChannels == 0 )
    {
        //No channels selected. this may arise due to already connected devices.
        return -1;
    }

    nHwCount = nChannels;   //Reassign hardware count according to final list of channels supported.

    if(bAutoSelect == false)
    {		
        /* If the default channel count parameter is set, prevent displaying the hardware selection dialog */
        if ( unDefaultChannelCnt && nHwCount >= unDefaultChannelCnt )
        {
            for (UINT i = 0; i < unDefaultChannelCnt; i++)
            {
                sg_anSelectedItems[i] = GetSelectedChannelIndex(i);
            }
            nHwCount = unDefaultChannelCnt;
        }
        else if ( ListHardwareInterfaces(sg_hOwnerWnd, sg_HardwareIntr, sg_anSelectedItems, nHwCount, InitData) != 0 )
        {
            sg_ucNoOfHardware = nHardwareCountPrev;
            return HW_INTERFACE_NO_SEL;
        }		
    }
    else
    {
        for (int nCount = 0; nCount < sg_ucNoOfHardware; nCount++)
        {
            sg_anSelectedItems[nCount] = GetSelectedChannelIndex(nCount);
        }

        nHwCount = sg_ucNoOfHardware;
    }
    sg_ucNoOfHardware = (UCHAR)nHwCount;
    sg_nNoOfChannels = (UCHAR)nHwCount;
    //Reorder hardware interface as per the user selection
    for (int nCount = 0; nCount < sg_ucNoOfHardware; nCount++)
    {
		if (sg_anSelectedItems[nCount] != -1)
		{
        sg_ndNeoToOpen[nCount].Handle       = (int)sg_HardwareIntr[sg_anSelectedItems[nCount]].m_dwIdInterface;
        sg_ndNeoToOpen[nCount].SerialNumber = (int)sg_HardwareIntr[sg_anSelectedItems[nCount]].m_dwVendor;
        _stscanf_s(sg_HardwareIntr[sg_anSelectedItems[nCount]].m_acNameInterface.c_str(), "%d", &sg_ndNeoToOpen[nCount].DeviceType);
        m_bytNetworkIDs[nCount]             =  sg_HardwareIntr[sg_anSelectedItems[nCount]].m_bytNetworkID;
		}       
    }
    for (int nIndex = 0; nIndex < sg_ucNoOfHardware; nIndex++)
    {
        if (nIndex == 0)// AFXBeginThread should be called only once.
        {
            s_DatIndThread.m_bToContinue = TRUE;
            s_DatIndThread.m_bIsConnected = FALSE;
            s_DatIndThread.m_unChannels = sg_ucNoOfHardware;
            sg_odHardwareNetwork.m_nNoOfChannels = (int)sg_ucNoOfHardware;
        }

        // Assign hardware handle
        sg_odHardwareNetwork.m_aodChannels[ nIndex ].m_hHardwareHandle = (BYTE)sg_ndNeoToOpen[nIndex].Handle;

        // Assign Net Handle
        sg_odHardwareNetwork.m_aodChannels[ nIndex ].m_hNetworkHandle = sg_HardwareIntr[nIndex].m_bytNetworkID;
    }
    return S_OK;
}

/**
 * \param[in] nHardwareCount Hardware Count
 * \return Returns S_OK if successful otherwise corresponding Error code.
 *
 * Finds the number of hardware connected. This is applicable
 * only for USB device. For parallel port this is not required.
 */
static int nGetNoOfConnectedHardware(int& nHardwareCount)
{
    // 0, Query successful, but no device found
    // > 0, Number of devices found
    // < 0, query for devices unsucessful
    int nReturn = NO_HW_INTERFACE;

    for (BYTE i = 0; i < 16; i++)
    {
        m_ucNetworkID[i] = i;
    }
    nHardwareCount = 32;
    // TODO: Add your command handler code here
    if ( icsneoFindNeoDevices(NEODEVICE_ALL , sg_ndNeoToOpen , &nHardwareCount ) )
    {
        if (nHardwareCount == 0)
        {
            m_omErrStr = "Query successful, but no device found";
        }
        nReturn = nHardwareCount;
    }
    else
    {
        nHardwareCount = 0;
        m_omErrStr = "Query for devices unsuccessful";
    }
    // Return the operation result
    return nReturn;
}

/**
 * \return Operation Result. 0 incase of no errors. Failure Error codes otherwise.
 *
 * This is USB Specific function.This function will create
 * find number of hardware connected. It will create network as
 * per hardware count. This will popup hardware selection dialog
 * in case there are more hardware present.
 */
static int nInitHwNetwork(PSCONTROLLER_DETAILS InitData, UINT unDefaultChannelCnt = 0)
{
    int nDevices = 0;
    int nReturn = NO_HW_INTERFACE;
    // Select Hardware
    // This function will be called only for USB mode
    nReturn = nGetNoOfConnectedHardware(nDevices);
    m_bInSimMode = (nDevices == 0);

    // Assign the device count
    sg_ucNoOfHardware = (UCHAR)nDevices;
    // Capture only Driver Not Running event
    // Take action based on number of Hardware Available

    // No Hardware found
    if( nDevices == 0 )
    {
        MessageBox(nullptr, m_omErrStr.c_str(), nullptr, MB_OK | MB_ICONERROR);
    }
    // Available hardware is lesser then the supported channels
    else
    {
        // Check whether channel selection dialog is required
        if(/* nDevices > 1 ||*/
                ( sg_ndNeoToOpen[0].DeviceType == NEODEVICE_VCAN3 ) )
        {
            // Get the selection from the user. This will also
            // create and assign the networks
            nReturn = nCreateMultipleHardwareNetwork(InitData, unDefaultChannelCnt);
        }
        //else
        //{
        //    // Use available one hardware
        //    nReturn = nCreateSingleHardwareNetwork();
        //}
    }
    return nReturn;
}

/**
 * \return Returns S_OK if successful otherwise corresponding Error code.
 *
 * This function will initialise hardware handlers of USB. This
 * will establish the connection with driver. This will create or
 * connect to the network. This will create client for USB. For
 * parallel port mode this will initialise connection with the
 * driver.
 */
static int nConnectToDriver(PSCONTROLLER_DETAILS InitData, UINT unDefaultChannelCnt = 0)
{
    int nReturn = -1;

    if( sg_bIsDriverRunning == TRUE )
    {
        // Select Hardware or Simulation Network
        nReturn = nInitHwNetwork(InitData, unDefaultChannelCnt);
    }
    return nReturn;
}

/**
 * \return TRUE if the driver is running. FALSE - IF it is not running
 */
static BOOL bGetDriverStatus()
{
    sg_bIsDriverRunning = TRUE;
    return TRUE;
}

/**
 * \return S_OK if successful otherwise corresponding Error code.
 *
 * This function will set the baud rate of the controller
 * Parallel Port Mode: Controller will be initialised with all
 * other parameters after baud rate change.
 */
static int nSetBaudRate()
{
    int nReturn = 0;

    // Set baud rate to all available hardware
    for( UINT unIndex = 0;
            unIndex < sg_odHardwareNetwork.m_nNoOfChannels;
            unIndex++)
    {
        FLOAT fBaudRate = (FLOAT)_tstof(sg_ControllerDetails[unIndex].m_omStrBaudrate.c_str());
        int nBitRate = (INT)(fBaudRate);

        // Set the baudrate
        nReturn = (*icsneoSetBitRate)(m_anhWriteObject[unIndex], nBitRate, m_bytNetworkIDs[unIndex]);
        if (nReturn != NEOVI_OK)
        {
            unIndex = sg_odHardwareNetwork.m_nNoOfDevices;
        }
        else
        {
            nReturn = 0;
        }
    }

    return nReturn;
}

/**
 * \return Returns S_OK if successful otherwise corresponding Error code.
 * This function will set the warning limit of the controller. In
 * USB mode this is not supported as the warning limit is
 * internally set to 96
 */
static int nSetWarningLimit()
{
    for (UINT i = 0; i < sg_podActiveNetwork->m_nNoOfChannels; i++)
    {
        CChannel& odChannel = sg_podActiveNetwork->m_aodChannels[i];
        m_unWarningLimit[i] = odChannel.m_ucWarningLimit;
    }
    return S_OK; // Multiple return statements had to be added because
    // neoVI specific codes and simulation related codes need to coexist.
    // Code for the later still employs Peak API interface.
}

/**
 * \return Operation Result. 0 incase of no errors. Failure Error codes otherwise.
 *
 * This function will set all controller parameters. This will
 * set Baud rate, Filter, Warning Limit and Controller Mode. In
 * case of USB the warning limit call will be ignored.
 */
static int nSetApplyConfiguration()
{
    int nReturn = S_OK;

    // Set Hardware Mode
    if ((nReturn = nSetHardwareMode(sg_ucControllerMode)) == S_OK)
    {
    }

    // Set warning limit only for hardware network
    if ((nReturn == S_OK) && (sg_ucControllerMode != defUSB_MODE_SIMULATE))
    {
        // Set Warning Limit
        nReturn = nSetWarningLimit();
    }

    return nReturn;
}

/**
 * \param[out] ucaTestResult Array that will hold test result. TRUE if hardware present and false if not connected
 * \return Operation Result. 0 incase of no errors. Failure Error codes otherwise.
 *
 * This function will check all hardware connectivity by getting
 * hardware parameter. In parallel port mode this will set the
 * baud rate to test hardware presence.
 */
static int nTestHardwareConnection(UCHAR& ucaTestResult, UINT nChannel) //const
{
    BYTE aucConfigBytes[CONFIGBYTES_TOTAL];
    int nReturn = 0;
    int nConfigBytes = 0;
    if (nChannel < sg_odHardwareNetwork.m_nNoOfDevices)
    {
        if ((icsneoGetConfiguration(m_anhObject[nChannel][0], aucConfigBytes, &nConfigBytes) == NEOVI_OK))
        {
            ucaTestResult = TRUE;
        }
        else
        {
            sg_bIsConnected = FALSE;
            ucaTestResult = FALSE;
        }
    }
    return nReturn;
}

/**
 * \return Operation Result. 0 incase of no errors. Failure Error codes otherwise.
 *
 * This will close the connection with the driver. This will be
 * called before deleting HI layer. This will be called during
 * application close.
 */
static int nDisconnectFromDriver()
{
    int nReturn = 0;
    sg_bIsDriverRunning = FALSE;

    int nErrors = 0;
    for (BYTE i = 0; i < sg_odHardwareNetwork.m_nNoOfDevices; i++)
    {
        if (m_anhObject[i][0] != 0)
        {
            // First disconnect the COM
            //(*icsneoFreeObject)(m_anhObject[i]);

            if ((*icsneoClosePort)(m_anhObject[i][0], &nErrors) == 1)
            {
                m_anhObject[i][0] = 0;
            }
            else
            {
                nReturn = -1;
            }
        }
    }
    sg_bCurrState = STATE_HW_INTERFACE_SELECTED;

    return nReturn;
}

/**
 * \param[in] bConnect TRUE to Connect, FALSE to Disconnect
 * \return Returns S_OK if successful otherwise corresponding Error code.
 *
 * This function will connect the tool with hardware. This will
 * establish the data link between the application and hardware.
 * Parallel Port Mode: Controller will be disconnected.
 * USB: Client will be disconnected from the network
 */
static int nConnect(BOOL bConnect, BYTE /*hClient*/)
{
    int nReturn = -1;
    BYTE aucConfigBytes[CONFIGBYTES_TOTAL];
    int nConfigBytes = 0;
    int ndDevicesOpened[MAX_DEVICES][2] = {0};  //Array of opened device [Port No, Handle]
    int nOpenedDeviceCnt = 0;

    if (!sg_bIsConnected && bConnect) // Disconnected and to be connected
    {
        for (UINT i = 0; i < sg_podActiveNetwork->m_nNoOfChannels; i++)
        {
            //Check if already connected..
            bool bFound = false;
            for (int nCnt = 0; nCnt<nOpenedDeviceCnt; nCnt++)
            {
                if ( ndDevicesOpened[nCnt][0] == sg_ndNeoToOpen[i].Handle )
                {
                    m_anhObject[i][0] = ndDevicesOpened[nCnt][1];
                    bFound = true;
                    break;
                }
            }
            if ( bFound == true )
            {
                //if already connected to it.
                nReturn = NEOVI_OK;
            }
            else
            {
                nReturn = (*icsneoOpenNeoDevice)(&sg_ndNeoToOpen[i], &(m_anhObject[i][0]), m_ucNetworkID, 1, 0);
            }

            if (nReturn != NEOVI_OK)
            {
                // In such a case it is wiser to carry out disconnecting
                nDisconnectFromDriver(); // operations from the driver

                // Try again
                (*icsneoOpenNeoDevice)(&sg_ndNeoToOpen[i], &(m_anhObject[i][0]), m_ucNetworkID, 1, 0);

                /* Checking return value of this function is not important
                because GetConfig function (called to check for the hardware
                presence) renders us the necessary information */
                nReturn = (*icsneoGetConfiguration)(m_anhObject[i][0], aucConfigBytes,
                                                    &nConfigBytes);
            }
            if (nReturn == NEOVI_OK) // Hardware is present
            {
                //Store opened devices Handle list.
                if ( !bFound )
                {
                    ndDevicesOpened[nOpenedDeviceCnt][0]  = sg_ndNeoToOpen[i].Handle;
                    ndDevicesOpened[nOpenedDeviceCnt++][1]= m_anhObject[i][0];
                }

                // OpenPort and this function must be called together.
                // CTimeManager::bReinitOffsetTimeValForICSneoVI();
                // Transit into 'CREATE TIME MAP' state
                sg_byCurrState = CREATE_MAP_TIMESTAMP;
                if (i == (sg_podActiveNetwork->m_nNoOfChannels -1))
                {
                    for ( UINT nInit = 0; nInit<sg_podActiveNetwork->m_nNoOfChannels; nInit++ )
                    {
                        m_anhWriteObject[nInit] = m_anhObject[nInit][0];
                    }
                    for ( int nInit = 0; nInit<nOpenedDeviceCnt; nInit++ )
                    {
                        m_anhObject[nInit][0] = ndDevicesOpened[nInit++][1];
                    }
                    sg_odHardwareNetwork.m_nNoOfDevices = nOpenedDeviceCnt;
                    sg_podActiveNetwork->m_nNoOfDevices = nOpenedDeviceCnt;
                    //Only at the last it has to be called
                    nSetBaudRate();
                    vMapDeviceChannelIndex();
                    vCreateTimeModeMapping();
                    sg_bIsConnected = bConnect;
                    s_DatIndThread.m_bIsConnected = sg_bIsConnected;
                }
                nReturn = S_OK;
            }
            else // Hardware is not present
            {
                nReturn = -1;
                // Display the error message when not called from COM interface
            }
        }
    }
    else if (sg_bIsConnected && !bConnect) // Connected & to be disconnected
    {
        sg_bIsConnected = bConnect;
        s_DatIndThread.m_bIsConnected = sg_bIsConnected;
        Sleep(0); // Let other threads run for once
        nReturn = nDisconnectFromDriver();
    }
    else
    {
        nReturn = S_OK;
    }
    if ( sg_bIsConnected )
    {
        InitializeCriticalSection(&sg_CritSectForAckBuf);
    }
    else
    {
        DeleteCriticalSection(&sg_CritSectForAckBuf);
    }
    return nReturn;
}

/**
 * Function to map device channels with BUSMASTER channel order
 */
static void vMapDeviceChannelIndex()
{
    //Reset previous channel ID assignment if any
    for (UINT i = 0; i < sg_podActiveNetwork->m_nNoOfDevices; i++)
    {
        for (UINT j = 1; j <= NETWORKS_COUNT; j++)
        {
            m_anhObject[i][j] = 0;    //Reset Channel Index
        }
    }

    for (UINT i = 0; i < sg_podActiveNetwork->m_nNoOfChannels; i++)
    {
        for (UINT j = 0 ; j < sg_podActiveNetwork->m_nNoOfDevices; j++)
        {
            if ( m_anhObject[j][0] ==  m_anhWriteObject[i] )
            {
                m_anhObject[j][m_bytNetworkIDs[i]+1] = i+1; //Assigning Channel Index
                break;
            }
        }
    }
}
/**
 * Function to set API function pointers
 */
HRESULT GetICS_neoVI_APIFuncPtrs(void)
{
    LRESULT unResult = 0;
    if (sg_hDll != nullptr)
    {
        //Check1
        icsneoFindNeoDevices = (FINDNEODEVICES) GetProcAddress(sg_hDll, "icsneoFindNeoDevices");
        if (nullptr == icsneoFindNeoDevices)
        {
            unResult = unResult | (1<<0);
        }
        //Check2
        icsneoOpenNeoDevice = (OPENNEODEVICE) GetProcAddress(sg_hDll, "icsneoOpenNeoDevice");
        if (nullptr == icsneoOpenNeoDevice)
        {
            unResult = unResult | (1<<1);
        }
        //Check3
        icsneoClosePort  = (CLOSEPORT) GetProcAddress(sg_hDll, "icsneoClosePort");
        if (nullptr == icsneoClosePort)
        {
            unResult = unResult | (1<<2);
        }
        //Check4
        icsneoFreeObject = (FREEOBJECT) GetProcAddress(sg_hDll, "icsneoFreeObject");
        if (nullptr == icsneoFreeObject)
        {
            unResult = unResult | (1<<3);
        }

        //Check5
        icsneoGetMessages = (GETMESSAGES) GetProcAddress(sg_hDll, "icsneoGetMessages");
        if (nullptr == icsneoGetMessages)
        {
            unResult = unResult | (1<<4);
        }
        //Check6
        icsneoTxMessages = (TXMESSAGES) GetProcAddress(sg_hDll, "icsneoTxMessages");
        if (nullptr == icsneoTxMessages)
        {
            unResult = unResult | (1<<5);
        }
        //Check7
        icsneoWaitForRxMessagesWithTimeOut = (WAITFORRXMSGS) GetProcAddress(sg_hDll, "icsneoWaitForRxMessagesWithTimeOut");
        if (nullptr == icsneoWaitForRxMessagesWithTimeOut)
        {
            unResult = unResult | (1<<6);
        }
        //Check8
        icsneoEnableNetworkRXQueue = (ENABLERXQUEUE) GetProcAddress(sg_hDll, "icsneoEnableNetworkRXQueue");
        if (nullptr == icsneoEnableNetworkRXQueue)
        {
            unResult = unResult | (1<<7);
        }
        //Check9
        icsneoGetTimeStampForMsg = (GETTSFORMSG) GetProcAddress(sg_hDll, "icsneoGetTimeStampForMsg");
        if (nullptr == icsneoGetTimeStampForMsg)
        {
            unResult = unResult | (1<<8);
        }

        //Check10
        icsneoGetConfiguration  = (GETCONFIG) GetProcAddress(sg_hDll, "icsneoGetConfiguration");
        if (nullptr == icsneoGetConfiguration)
        {
            unResult = unResult | (1<<9);
        }
        //Check11
        icsneoSendConfiguration  = (SENDCONFIG) GetProcAddress(sg_hDll, "icsneoSendConfiguration");
        if (nullptr == icsneoSendConfiguration)
        {
            unResult = unResult | (1<<10);
        }
        //Check12
        icsneoSetBitRate = (SETBITRATE) GetProcAddress(sg_hDll, "icsneoSetBitRate");
        if (nullptr == icsneoSetBitRate)
        {
            unResult = unResult | (1<<11);
        }

        //Check13
        icsneoGetLastAPIError  = (GETLASTAPIERROR) GetProcAddress(sg_hDll, "icsneoGetLastAPIError");
        if (nullptr == icsneoGetLastAPIError)
        {
            unResult = unResult | (1<<12);
        }
        //Check14
        icsneoGetErrorMessages  = (GETERRMSGS) GetProcAddress(sg_hDll, "icsneoGetErrorMessages");
        if (nullptr == icsneoGetErrorMessages)
        {
            unResult = unResult | (1<<13);
        }
        //Check15
        icsneoGetErrorInfo = (GETERRORINFO) GetProcAddress(sg_hDll, "icsneoGetErrorInfo");
        if (nullptr == icsneoGetErrorInfo)
        {
            unResult = unResult | (1<<14);
        }

        //Check16
        icsneoGetDLLVersion  = (GETDLLVERSION) GetProcAddress(sg_hDll, "icsneoGetDLLVersion");
        if (nullptr == icsneoGetDLLVersion)
        {
            unResult = unResult | (1<<15);
        }
        //Check17
        icsneoGetHardwareLicense = (GETHWLICENSE) GetProcAddress(sg_hDll, "icsneoGetHardwareLicense");
        if (nullptr == icsneoGetHardwareLicense)
        {
            unResult = unResult | (1<<16);
        }
        //Check18
        icsneoGetHWFirmwareInfo = (GETHWFIRMWAREINFO) GetProcAddress(sg_hDll, "icsneoGetHWFirmwareInfo");
        if (nullptr == icsneoGetHWFirmwareInfo)
        {
            unResult = unResult | (1<<17);
        }

        icsneoSetDeviceParameters = (SETDEVICEPARMS) GetProcAddress(sg_hDll, "icsneoSetDeviceParameters");
        //check for error
        if (unResult != 0)
        {
            FreeLibrary(sg_hDll);
        }
    }
    else
    {
        unResult = S_FALSE;
    }
    return (HRESULT) unResult;
}

/**
 * Perform initialization operations specific to TZM
 */
HRESULT CDIL_CAN_ICSNeoVI::CAN_PerformInitOperations(void)
{
    for(int nCnt =0; nCnt < 2000; nCnt++)
    {
        sg_asMsgBuffer[nCnt].m_uDataInfo.m_sCANMsg.m_bCANFD = false;
    }
    //Register Monitor client
    CAN_RegisterClient(TRUE, sg_dwClientID, CAN_MONITOR_NODE);
    sg_podActiveNetwork = &sg_odHardwareNetwork;

    //Initialize the selected channel items array to -1
    for ( UINT i = 0; i< CHANNEL_ALLOWED; i++ )
    {
        sg_anSelectedItems[i] = -1;
        sg_ControllerDetails[i].m_omHardwareDesc = "";
    }

    return S_OK;
}

/**
 * Perform closure operations specific to TZM
 */
HRESULT CDIL_CAN_ICSNeoVI::CAN_PerformClosureOperations(void)
{
    HRESULT hResult = S_OK;

    UINT ClientIndex = 0;
    while (sg_unClientCnt > 0)
    {
        bRemoveClient(sg_asClientToBufMap[ClientIndex].dwClientID);
    }
    nDisconnectFromDriver();
    hResult = CAN_DeselectHwInterface();
    if (hResult == S_OK)
    {
        sg_bCurrState = STATE_DRIVER_SELECTED;
    }
    return hResult;
}

/**
 * Retrieve time mode mapping
 */
HRESULT CDIL_CAN_ICSNeoVI::CAN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER& QueryTickCount)
{
    CurrSysTime = sg_CurrSysTime;
    TimeStamp = sg_TimeStamp;
    QueryTickCount = sg_QueryTickCount;
    return S_OK;
}

/**
 * Function to List Hardware interfaces connect to the system and requests to the
 * user to select
 */
HRESULT CDIL_CAN_ICSNeoVI::CAN_ListHwInterfaces(INTERFACE_HW_LIST& asSelHwInterface, INT& nCount, PSCONTROLLER_DETAILS InitData)
{
    USES_CONVERSION;
    HRESULT hResult = S_FALSE;
    if (bGetDriverStatus())
    {
        if (( hResult = nConnectToDriver(InitData, nCount) ) == CAN_USB_OK)
        {
            nCount = sg_ucNoOfHardware;
            for (UINT i = 0; i < sg_ucNoOfHardware; i++)
            {
                asSelHwInterface[i].m_dwIdInterface = (DWORD)sg_ndNeoToOpen[i].Handle;
                std::ostringstream oss;
                oss << std::dec << sg_ndNeoToOpen[i].SerialNumber;
                asSelHwInterface[i].m_acDescription = oss.str();
                asSelHwInterface[i].m_bytNetworkID = m_bytNetworkIDs[i];
                asSelHwInterface[i].m_acNameInterface = sg_HardwareList.m_omHardwareChannel[sg_anSelectedItems[i]].c_str();
                sg_ControllerDetails[i].m_omHardwareDesc = sg_HardwareList.m_omHardwareChannel[sg_anSelectedItems[i]].c_str();
                hResult = S_OK;
                sg_bCurrState = STATE_HW_INTERFACE_LISTED;
            }
            m_dwDriverId = m_dwPrevDriverId;
        }
    }
    return hResult;
}

/**
 * Function to deselect the chosen hardware interface
 */
HRESULT CDIL_CAN_ICSNeoVI::CAN_DeselectHwInterface(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_OK;

    sg_bCurrState = STATE_HW_INTERFACE_LISTED;

    return hResult;
}

/**
 * Function to select hardware interface chosen by the user
 */
HRESULT CDIL_CAN_ICSNeoVI::CAN_SelectHwInterface(const INTERFACE_HW_LIST& asSelHwInterface, INT /*nCount*/)
{
    USES_CONVERSION;

    VALIDATE_POINTER_RETURN_VAL(sg_hDll, S_FALSE);

    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_LISTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_FALSE;

    if (bGetDriverStatus())
    {
        //Select the interface
        for (UINT i = 0; i < sg_ucNoOfHardware; i++)
        {
            sg_ndNeoToOpen[i].Handle       = (INT)asSelHwInterface[i].m_dwIdInterface;
            sg_ndNeoToOpen[i].SerialNumber = _ttoi(asSelHwInterface[i].m_acDescription.c_str());
            m_bytNetworkIDs[i]             = asSelHwInterface[i].m_bytNetworkID;
            sg_ControllerDetails[i].m_omHardwareDesc = asSelHwInterface[i].m_acNameInterface;
        }
        //Check for the success
        sg_bCurrState = STATE_HW_INTERFACE_SELECTED;
        hResult = S_OK;
    }
    else
    {
        hResult = S_FALSE;
    }
    return hResult;
}

/**
 * Function to set controller configuration
 */
HRESULT CDIL_CAN_ICSNeoVI::CAN_SetConfigData(PSCONTROLLER_DETAILS ConfigFile, int /* Length */)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    USES_CONVERSION;

    /* Fill the hardware description details */
    hFillHardwareDesc((PSCONTROLLER_DETAILS)ConfigFile);

    HRESULT hResult = S_FALSE;

    for(int i = 0; i < defNO_OF_CHANNELS; i++)
    {
        sg_ControllerDetails[i] = ConfigFile[i];
    }

    int nReturn = nSetApplyConfiguration();
    if (nReturn == S_OK)
    {
        hResult = S_OK;
    }

    return hResult;
}

HRESULT CDIL_CAN_ICSNeoVI::CAN_SetHardwareChannel(PSCONTROLLER_DETAILS ouControllerDetails,DWORD dwDriverId,bool /*bIsHardwareListed*/, unsigned int unChannelCount)
{
    if(m_dwDriverId != 0 && m_dwDriverId != dwDriverId )
    {
        m_dwPrevDriverId = dwDriverId;
        return S_OK;
    }

    m_dwDriverId = dwDriverId;
    m_dwPrevDriverId = dwDriverId;

    sg_SelectedChannels.m_nChannelCount = 0;

    for ( UINT i = 0; i< CHANNEL_ALLOWED; i++ )
    {
        sg_anSelectedItems[i] = -1;
    }

    for (int nChannel = 0; nChannel < unChannelCount/*(sg_ControllerDetails[nChannel].m_omHardwareDesc != "") && (sg_ControllerDetails[nChannel].m_omHardwareDesc != "Simulation")*/ ; nChannel++)
    {
        if((ouControllerDetails[nChannel].m_omHardwareDesc != "") && (ouControllerDetails[nChannel].m_omHardwareDesc != "Simulation"))
        {
            sg_SelectedChannels.m_omHardwareChannel[nChannel] = ouControllerDetails[nChannel].m_omHardwareDesc;
            sg_ControllerDetails[nChannel].m_omHardwareDesc = ouControllerDetails[nChannel].m_omHardwareDesc;
            sg_SelectedChannels.m_nChannelCount += 1;
        }
    }

    sg_ucNoOfHardware = sg_SelectedChannels.m_nChannelCount;
    sg_nNoOfChannels = sg_SelectedChannels.m_nChannelCount;

    if(sg_ucNoOfHardware > 0)
    {
        int nResult = nCreateMultipleHardwareNetwork(nullptr, 1, true);

        if(nResult != CAN_USB_OK)
        {
            return S_FALSE;
        }
    }

    /*for (int nCount = 0; nCount < sg_ucNoOfHardware; nCount++)
    {
        sg_anSelectedItems[nCount] = GetSelectedChannelIndex(nCount);

        if(sg_anSelectedItems[nCount] != -1)
        {
            sg_ndNeoToOpen[nCount].Handle       = (int)sg_HardwareIntr[sg_anSelectedItems[nCount]].m_dwIdInterface;
            sg_ndNeoToOpen[nCount].SerialNumber = (int)sg_HardwareIntr[sg_anSelectedItems[nCount]].m_dwVendor;
            _stscanf_s(sg_HardwareIntr[sg_anSelectedItems[nCount]].m_acNameInterface.c_str(), "%d", &sg_ndNeoToOpen[nCount].DeviceType);
            m_bytNetworkIDs[nCount]             =  sg_HardwareIntr[sg_anSelectedItems[nCount]].m_bytNetworkID;
        }
    }*/

    return S_OK;
}

BOOL Callback_DILTZM(BYTE /*Argument*/, PSCONTROLLER_DETAILS pDatStream, int /*Length*/)
{
    return ( sg_pouDIL_CAN_ICSNeoVI->CAN_SetConfigData(pDatStream, 0) == S_OK);
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Displays the configuration dialog for controller
 */
int DisplayConfigurationDlg(HWND hParent, DILCALLBACK /*ProcDIL*/,
                            PSCONTROLLER_DETAILS pControllerDetails, UINT nCount)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    int nResult = WARNING_NOTCONFIRMED;

    CChangeRegisters_CAN_ICS_neoVI ouChangeRegister(CWnd::FromHandle(hParent), pControllerDetails, nCount);
    ouChangeRegister.DoModal();
    nResult = ouChangeRegister.nGetInitStatus();

    return nResult;
}


/**
*  Function to fill the hardware description details
*/
HRESULT hFillHardwareDesc(PSCONTROLLER_DETAILS pControllerDetails)
{
    /* First initialize with existing hw description */
    for (UINT i = 0; i < (UINT)sg_ucNoOfHardware; i++)
    {
        unsigned int serialNumber = sg_ndNeoToOpen[i].SerialNumber;
        char netid_str[256];

        int nHardwareLic = 0;
        if (sg_ndNeoToOpen[i].DeviceType == NEODEVICE_VCAN3)
        {
            int hObject = 0;
            int nResult = 0;

            nResult = (*icsneoOpenNeoDevice)(&sg_ndNeoToOpen[i], &hObject, nullptr, 1, 0);
            if (nResult == NEOVI_OK && hObject!=0)
            {
                int nErrors = 0;
                if ( icsneoGetHardwareLicense )
                {
                    (*icsneoGetHardwareLicense)(hObject, &nHardwareLic);
                }
                (*icsneoClosePort)(hObject, &nErrors);
            }
        }

        switch (sg_ndNeoToOpen[i].DeviceType)
        {
                /* neoVI Blue */
            case NEODEVICE_BLUE:
                /* neoVI Fire/Red */
            case NEODEVICE_FIRE:
                switch (m_bytNetworkIDs[i])
                {
                    case NETID_HSCAN:
                        strncpy(&netid_str[0], "HSCAN", sizeof(netid_str));
                        break;
                    case NETID_MSCAN:
                        strncpy(&netid_str[0], "MSCAN", sizeof(netid_str));
                        break;
                    case NETID_SWCAN:
                        strncpy(&netid_str[0], "SW CAN", sizeof(netid_str));
                        break;
                    case NETID_LSFTCAN:
                        strncpy(&netid_str[0], "LSFT CAN", sizeof(netid_str));
                        break;
                }
                break;

                /* ValueCAN */
            case NEODEVICE_DW_VCAN:
                switch (m_bytNetworkIDs[i])
                {
                    case NETID_HSCAN:
                        strncpy(&netid_str[0], "CAN", sizeof(netid_str));
                        break;
                }
                break;

                /* ValueCAN3 */
            case NEODEVICE_VCAN3:
                if (nHardwareLic == 8)   // Limited Version with only one channel
                {
                    switch (m_bytNetworkIDs[i])
                    {
                        case NETID_HSCAN:
                            strncpy(&netid_str[0], "CAN", sizeof(netid_str));
                            break;
                    }
                }
                else     // Two channels
                {
                    switch (m_bytNetworkIDs[i])
                    {
                        case NETID_HSCAN:
                            strncpy(&netid_str[0], "CAN 1", sizeof(netid_str));
                            break;
                        case NETID_MSCAN:
                            strncpy(&netid_str[0], "CAN 2", sizeof(netid_str));
                            break;
                    }
                }
                break;
        }

        /* default case */
        if (strlen(&netid_str[0]) == 0)
        {
            switch (m_bytNetworkIDs[i])
            {
                case NETID_HSCAN:
                    strncpy(&netid_str[0], "HSCAN", sizeof(netid_str));
                    break;
                case NETID_MSCAN:
                    strncpy(&netid_str[0], "MSCAN", sizeof(netid_str));
                    break;
                case NETID_SWCAN:
                    strncpy(&netid_str[0], "SWCAN", sizeof(netid_str));
                    break;
                case NETID_LSFTCAN:
                    strncpy(&netid_str[0], "LSFTCAN", sizeof(netid_str));
                    break;
            }
        }
        CHAR chTemp[MAX_PATH];
        //string chTemp = "";
        std::ostringstream stringStream;
        stringStream.clear();

        switch (sg_ndNeoToOpen[i].DeviceType)
        {
                /* neoVI Blue */
            case NEODEVICE_BLUE:
                sprintf_s(chTemp, sizeof(chTemp), "neoVI Blue, Serial Number %d, Network: %s",
                          serialNumber, &netid_str[0]);

                break;

                /* ValueCAN */
            case NEODEVICE_DW_VCAN:
                _stprintf_s(chTemp, sizeof(chTemp), "ValueCAN, Serial Number %d, Network: %s",
                            serialNumber, &netid_str[0]);

                break;

                /* neoVI Fire/Red */
            case NEODEVICE_FIRE:
                _stprintf_s(chTemp, sizeof(chTemp), "neoVi Fire/Red, Serial Number %d, Network: %s",
                            serialNumber, &netid_str[0]);

                break;

                /* ValueCAN3 and ETAS ES581 */
            case NEODEVICE_VCAN3:
                if ((serialNumber >= 80000) && (serialNumber <= 119999))
                {
                    if (nHardwareLic == 8)   // Limited Version with only one channel
                    {
                        _stprintf_s(chTemp, sizeof(chTemp), "ES581.2, Serial Number %d, Network: %s",
                                    serialNumber-50000, &netid_str[0]);

                    }
                    else     // Two channels
                    {
                        _stprintf_s(chTemp, sizeof(chTemp), "ES581.3, Serial Number %d, Network: %s",
                                    serialNumber-50000, &netid_str[0]);

                    }
                }
                else
                {
                    _stprintf_s(chTemp, sizeof(chTemp), "ValueCAN3, Serial Number %d, Network: %s",
                                serialNumber, &netid_str[0]);
                }
                break;

            default:
                _stprintf_s(chTemp, sizeof(chTemp), "Unknown, Serial Number %d", serialNumber);
                //chTemp = "Unknown";
                break;
        };
        //stringStream << chTemp << ", Serial Number " << serialNumber << ", Network: " << netid_str[0];
        pControllerDetails[i].m_omHardwareDesc = chTemp;
    }
    return S_OK;
}

/**
 * Function to display config dialog
 */



/**
 * Function to start monitoring the bus
 */
HRESULT CDIL_CAN_ICSNeoVI::CAN_StartHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    USES_CONVERSION;
    HRESULT hResult = S_OK;
    sg_sParmRThread.m_pBuffer = (LPVOID) &s_DatIndThread;
    s_DatIndThread.m_bToContinue = TRUE;
    /* connect to the network */
    for (UINT ClientIndex = 0; ClientIndex < sg_unClientCnt; ClientIndex++)
    {
        BYTE hClient = sg_asClientToBufMap[ClientIndex].hClientHandle;
        hResult = nConnect(TRUE, hClient);
        if (hResult == CAN_USB_OK)
        {
            hResult = S_OK;
            sg_bCurrState = STATE_CONNECTED;
        }
        else
        {
            //log the error for open port failure
            vRetrieveAndLog(hResult, __FILE__, __LINE__);
            hResult = ERR_LOAD_HW_INTERFACE;
        }
    }
    /* If everything is ok, start the read thread */
    if (hResult == S_OK)
    {
        if (sg_sParmRThread.bStartThread(CanMsgReadThreadProc_CAN_ICS_neoVI))
        {
            hResult = S_OK;
        }
    }

    return hResult;
}

/**
 * Function to stop monitoring the bus
 */
HRESULT CDIL_CAN_ICSNeoVI::CAN_StopHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_OK;

    //Terminate the read thread
    sg_sParmRThread.bTerminateThread();

    for (UINT ClientIndex = 0; ClientIndex < sg_unClientCnt; ClientIndex++)
    {
        BYTE hClient = sg_asClientToBufMap[ClientIndex].hClientHandle;

        hResult = nConnect(FALSE, hClient);
        if (hResult == CAN_USB_OK)
        {
            hResult = S_OK;
            sg_bCurrState = STATE_HW_INTERFACE_SELECTED;
        }
        else
        {
            //log the error for open port failure
            vRetrieveAndLog(hResult, __FILE__, __LINE__);
            hResult = ERR_LOAD_HW_INTERFACE;
            ClientIndex = sg_unClientCnt; //break the loop
        }
    }
    return hResult;
}

static BOOL bClientExist(std::string pcClientName, INT& Index)
{
    for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        if (!_tcscmp(pcClientName.c_str(), sg_asClientToBufMap[i].pacClientName))
        {
            Index = i;
            return TRUE;
        }
    }
    return FALSE;
}

static BOOL bClientIdExist(const DWORD& dwClientId)
{
    BOOL bReturn = FALSE;
    for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        if (sg_asClientToBufMap[i].dwClientID == dwClientId)
        {
            bReturn = TRUE;
            i = sg_unClientCnt; // break the loop
        }
    }
    return bReturn;
}

/**
 * \param sErrorCount Error Counter Structure
 * \return Operation Result. 0 incase of no errors. Failure Error codes otherwise.
 *
 * This function will return the error counter values. In case of USB this is not supported.
 */
static int nGetErrorCounter( UINT unChannel, SERROR_CNT& sErrorCount)
{
    int nReturn = -1;

    // Check for the valid channel index
    if( unChannel < sg_podActiveNetwork->m_nNoOfChannels )
    {
        // Get the channel reference
        CChannel& odChannel = sg_podActiveNetwork->m_aodChannels[ unChannel ];
        // Assign the error counter value
        sErrorCount.m_ucRxErrCount = odChannel.m_ucRxErrorCounter;
        sErrorCount.m_ucTxErrCount = odChannel.m_ucTxErrorCounter;
        nReturn = S_OK;
    }
    else
    {
        // Invalid channel ID. Help debugging
    }

    return nReturn;
}

/**
 * Function to get Controller status
 */
HRESULT CDIL_CAN_ICSNeoVI::CAN_GetCurrStatus(STATUSMSG& StatusData)
{
    if (sg_ucControllerMode == defUSB_MODE_ACTIVE)
    {
        StatusData.wControllerStatus = NORMAL_ACTIVE;
    }
    else if (sg_ucControllerMode == defUSB_MODE_PASSIVE)
    {
        StatusData.wControllerStatus = NORMAL_PASSIVE;
    }
    else if (sg_ucControllerMode == defUSB_MODE_SIMULATE)
    {
        StatusData.wControllerStatus = NOT_DEFINED;
    }
    return S_OK;
}

/**
 * Function to get Tx Msg Buffers configured from chi file
 */
HRESULT CDIL_CAN_ICSNeoVI::CAN_GetTxMsgBuffer(BYTE*& /*pouFlxTxMsgBuffer*/)
{
    return S_OK;
}

/**
 * \param[in] sMessage Message to Transmit
 * \return Operation Result. 0 incase of no errors. Failure Error codes otherwise.
 *
 * This will send a CAN message to the driver. In case of USB
 * this will write the message in to the driver buffer and will
 * return. In case if parallel port mode this will write the
 * message and will wait for the ACK event from the driver. If
 * the event fired this will return 0. Otherwise this will return
 * wait time out error. In parallel port it is a blocking call
 * and in case of failure condition this will take 2 seconds.
 */
static int nWriteMessage(STCAN_MSG sMessage)
{
    int nReturn = -1;

    // Return when in disconnected state
    if (!sg_bIsConnected)
    {
        return nReturn;
    }

    if (sg_podActiveNetwork != nullptr &&
            sMessage.m_ucChannel > 0 &&
            sMessage.m_ucChannel <= sg_podActiveNetwork->m_nNoOfChannels)
    {
        icsSpyMessage SpyMsg;
        memcpy(&(SpyMsg.ArbIDOrHeader), &(sMessage.m_unMsgID), sizeof(UINT));
        SpyMsg.NumberBytesData = sMessage.m_ucDataLen;
        SpyMsg.StatusBitField = 0;
        SpyMsg.StatusBitField2 = 0;
        if (sMessage.m_ucRTR == 1)
        {
            SpyMsg.StatusBitField |= SPY_STATUS_REMOTE_FRAME;
        }
        if (sMessage.m_ucEXTENDED == 1)
        {
            SpyMsg.StatusBitField |= SPY_STATUS_XTD_FRAME;
        }
        memcpy(SpyMsg.Data, sMessage.m_ucData, sMessage.m_ucDataLen);
        if ((*icsneoTxMessages)(m_anhWriteObject[(int)(sMessage.m_ucChannel) - 1], &SpyMsg,
                                m_bytNetworkIDs[(int)(sMessage.m_ucChannel) - 1], 1) != 0)
        {
            nReturn = 0;
        }
    }
    return nReturn; // Multiple return statements had to be added because
    // neoVI specific codes and simulation related codes need to coexist.
    // Code for the later still employs Peak API interface.
}

/**
 * Function to Send CAN Message to Transmit buffer. This is called only after checking the controller in active mode
 */
HRESULT CDIL_CAN_ICSNeoVI::CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& sMessage)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);
    static SACK_MAP sAckMap;
    HRESULT hResult = S_FALSE;
    if (bClientIdExist(dwClientID))
    {
        if (sMessage.m_ucChannel <= sg_ucNoOfHardware)
        {
            sAckMap.m_ClientID = dwClientID;
            sAckMap.m_MsgID = sMessage.m_unMsgID;
            sAckMap.m_Channel = sMessage.m_ucChannel;
            /*Mark an entry in Map. This is helpful to idendify
              which client has been sent this message in later stage*/
            vMarkEntryIntoMap(sAckMap);
            if (nWriteMessage(sMessage) == S_OK)
            {
                hResult = S_OK;
            }
        }
        else
        {
            hResult = ERR_INVALID_CHANNEL;
        }
    }
    else
    {
        hResult = ERR_NO_CLIENT_EXIST;
    }
    return hResult;
}




/**
 * Function to retreive error string of last occurred error
 */
HRESULT CDIL_CAN_ICSNeoVI::CAN_GetLastErrorString(std::string& acErrorStr)
{
    acErrorStr = sg_acErrStr;
    return S_OK;
}

/**
 * Set application parameters specific to CAN_USB
 */
HRESULT CDIL_CAN_ICSNeoVI::CAN_SetAppParams(HWND hWndOwner)
{
    sg_hOwnerWnd = hWndOwner;

    // Initialise both the time parameters
    GetLocalTime(&sg_CurrSysTime);
    sg_TimeStamp = 0x0;
    //INITIALISE_DATA(sg_sCurrStatus);
    memset(&sg_sCurrStatus, 0, sizeof(sg_sCurrStatus));
    //Query Tick Count
    sg_QueryTickCount.QuadPart = 0;
    //INITIALISE_ARRAY(sg_acErrStr);
    sg_acErrStr = "";;
    CAN_ManageMsgBuf(MSGBUF_CLEAR, 0, nullptr);

    return S_OK;
}

static BOOL bIsBufferExists(const SCLIENTBUFMAP& sClientObj, const CBaseCANBufFSE* pBuf)
{
    BOOL bExist = FALSE;
    for (UINT i = 0; i < sClientObj.unBufCount; i++)
    {
        if (pBuf == sClientObj.pClientBuf[i])
        {
            bExist = TRUE;
            i = sClientObj.unBufCount; //break the loop
        }
    }
    return bExist;
}

static DWORD dwGetAvailableClientSlot()
{
    DWORD nClientId = 2;
    for (int i = 0; i < MAX_CLIENT_ALLOWED; i++)
    {
        if (bClientIdExist(nClientId))
        {
            nClientId += 1;
        }
        else
        {
            i = MAX_CLIENT_ALLOWED; //break the loop
        }
    }
    return nClientId;
}

/**
 * Register Client
 */
HRESULT CDIL_CAN_ICSNeoVI::CAN_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName)
{
    USES_CONVERSION;
    HRESULT hResult = S_FALSE;
    if (bRegister)
    {
        if (sg_unClientCnt < MAX_CLIENT_ALLOWED)
        {
            INT Index = 0;
            if (!bClientExist(pacClientName, Index))
            {
                //Currently store the client information
                if (_tcscmp(pacClientName, CAN_MONITOR_NODE) == 0)
                {
                    //First slot is reserved to monitor node
                    ClientID = 1;
                    _tcscpy(sg_asClientToBufMap[0].pacClientName, pacClientName);
                    sg_asClientToBufMap[0].dwClientID = ClientID;
                    sg_asClientToBufMap[0].unBufCount = 0;
                }
                else
                {
                    /* if (!bClientExist(CAN_MONITOR_NODE, Index))
                     {
                         Index = sg_unClientCnt + 1;
                     }
                     else
                     {
                         Index = sg_unClientCnt;
                     }*/

                    Index = sg_unClientCnt;
                    ClientID = dwGetAvailableClientSlot();
                    _tcscpy(sg_asClientToBufMap[Index].pacClientName, pacClientName);

                    sg_asClientToBufMap[Index].dwClientID = ClientID;
                    sg_asClientToBufMap[Index].unBufCount = 0;
                }
                sg_unClientCnt++;
                hResult = S_OK;
            }
            else
            {
                ClientID = sg_asClientToBufMap[Index].dwClientID;
                hResult = ERR_CLIENT_EXISTS;
            }
        }
        else
        {
            hResult = ERR_NO_MORE_CLIENT_ALLOWED;
        }
    }
    else
    {
        if (bRemoveClient(ClientID))
        {
            hResult = S_OK;
        }
        else
        {
            hResult = ERR_NO_CLIENT_EXIST;
        }
    }
    return hResult;
}

HRESULT CDIL_CAN_ICSNeoVI::CAN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj)
{
    HRESULT hResult = S_FALSE;
    if (ClientID != 0)
    {
        UINT unClientIndex;
        if (bGetClientObj(ClientID, unClientIndex))
        {
            SCLIENTBUFMAP& sClientObj = sg_asClientToBufMap[unClientIndex];
            if (bAction == MSGBUF_ADD)
            {
                //Add msg buffer
                if (pBufObj != nullptr)
                {
                    if (sClientObj.unBufCount < MAX_BUFF_ALLOWED)
                    {
                        if (bIsBufferExists(sClientObj, pBufObj) == FALSE)
                        {
                            sClientObj.pClientBuf[sClientObj.unBufCount++] = pBufObj;
                            hResult = S_OK;
                        }
                        else
                        {
                            hResult = ERR_BUFFER_EXISTS;
                        }
                    }
                }
            }
            else if (bAction == MSGBUF_CLEAR)
            {
                //clear msg buffer
                if (pBufObj != nullptr) //REmove only buffer mentioned
                {
                    bRemoveClientBuffer(sClientObj.pClientBuf, sClientObj.unBufCount, pBufObj);
                }
                else //Remove all
                {
                    for (UINT i = 0; i < sClientObj.unBufCount; i++)
                    {
                        sClientObj.pClientBuf[i] = nullptr;
                    }
                    sClientObj.unBufCount = 0;
                }
                hResult = S_OK;
            }
            else
            {
                ////ASSERT(false);
            }
        }
        else
        {
            hResult = ERR_NO_CLIENT_EXIST;
        }
    }
    else
    {
        if (bAction == MSGBUF_CLEAR)
        {
            //clear msg buffer
            for (UINT i = 0; i < sg_unClientCnt; i++)
            {
                CAN_ManageMsgBuf(MSGBUF_CLEAR, sg_asClientToBufMap[i].dwClientID, nullptr);
            }
            hResult = S_OK;
        }
    }

    return hResult;
}

/**
 * Function to load driver icsneo40.dll
 */
HRESULT CDIL_CAN_ICSNeoVI::CAN_LoadDriverLibrary(void)
{
    USES_CONVERSION;

    HRESULT hResult = S_OK;

    if (sg_hDll != nullptr)
    {
        hResult = DLL_ALREADY_LOADED;
    }

    if (S_OK == hResult)
    {
        //Load the version i.e icsneo40.dll
        sg_hDll = LoadLibrary("icsneo40.dll");
        if (sg_hDll == nullptr)
        {
            hResult = ERR_LOAD_DRIVER;
        }

        if (sg_hDll != nullptr)
        {
            int nResult = GetICS_neoVI_APIFuncPtrs();
            if (nResult != 0)
            {
                // Log list of the function pointers non-retrievable
                // TO DO: specific information on failure in getting function pointer
                hResult = ERR_LOAD_DRIVER;
            }
        }
    }

    return hResult;
}

/**
 * Function to Unload Driver library
 */
HRESULT CDIL_CAN_ICSNeoVI::CAN_UnloadDriverLibrary(void)
{
    // Don't bother about the success & hence the result
    CAN_DeselectHwInterface();

    // Store the Boardinfo to global variable

    if (sg_hDll != nullptr)
    {
        FreeLibrary(sg_hDll);
        sg_hDll = nullptr;
    }
    return S_OK;
}




HRESULT CDIL_CAN_ICSNeoVI::CAN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus)
{
    if (hEvent != nullptr)
    {
        sg_hCntrlStateChangeEvent = hEvent;
    }
    unCntrlStatus = static_cast<UINT>(sg_ucControllerMode);
    return S_OK;
}
HRESULT CDIL_CAN_ICSNeoVI::CAN_SetControllerParams(int nValue, ECONTR_PARAM eContrparam)
{
    switch(eContrparam)
    {
        case HW_MODE:
        {
            switch(nValue)
            {
                case defMODE_ACTIVE:
                {
                    int iErrorIndex;
                    for (UINT i = 0; i < sg_podActiveNetwork->m_nNoOfChannels; i++)
                    {
                        (void) icsneoSetDeviceParameters(m_anhObject[0][0], "can1/Mode=0", &iErrorIndex, 1);
                    }
                }
                break;
                case defMODE_PASSIVE:
                {
                    int iErrorIndex;
                    for (UINT i = 0; i < sg_podActiveNetwork->m_nNoOfChannels; i++)
                    {
                        (void) icsneoSetDeviceParameters(m_anhObject[0][0], "can1/Mode=3", &iErrorIndex, 1);
                    }
                }
                break;
            }
        }
    }
    return S_OK;
}
HRESULT CDIL_CAN_ICSNeoVI::CAN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam)
{
    HRESULT hResult = S_OK;
    if ((sg_bCurrState == STATE_HW_INTERFACE_SELECTED) || (sg_bCurrState == STATE_CONNECTED))
    {
        switch (eContrParam)
        {

            case NUMBER_HW:
            {
                lParam = sg_ucNoOfHardware;
            }
            break;
            case NUMBER_CONNECTED_HW:
            {
                int nConHwCnt = 0;
                if (nGetNoOfConnectedHardware(nConHwCnt) == NEOVI_OK)
                {
                    lParam = nConHwCnt;
                }
                else
                {
                    hResult = S_FALSE;
                }
            }
            break;
            case DRIVER_STATUS:
            {
                lParam = sg_bIsDriverRunning;
            }
            break;
            case HW_MODE:
            {
                if (nChannel < sg_ucNoOfHardware)
                {
                    lParam = sg_ucControllerMode;
                    if( sg_ucControllerMode == 0 || sg_ucControllerMode > defMODE_SIMULATE )
                    {
                        lParam = defCONTROLLER_BUSOFF;
                        hResult = S_FALSE;
                    }
                }
                else
                {
                    //Unknown
                    lParam = defCONTROLLER_BUSOFF + 1;
                }
            }
            break;
            case CON_TEST:
            {
                UCHAR ucResult;
                if (nTestHardwareConnection(ucResult, nChannel) == S_OK)
                {
                    lParam = (LONG)ucResult;
                }
            }
            break;
            case CNTR_STATUS:
            {
                char pGetFireParms[] = "can1/Mode";
                char Values[500];
                (void) icsneoGetDeviceParameters(m_anhObject[nChannel][0], pGetFireParms, Values, 499);
                INT nMode = atoi(Values);
                if(nMode == 3)
                {
                    lParam = defMODE_PASSIVE;
                }
                if(nMode == 0)
                {
                    lParam = defMODE_ACTIVE;
                }
            }

            default:
            {
                hResult = S_FALSE;
            }
            break;

        }
    }
    else
    {
        hResult = ERR_IMPROPER_STATE;
    }
    return hResult;
}

HRESULT CDIL_CAN_ICSNeoVI::CAN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM /*eContrParam*/)
{
    HRESULT hResult = S_FALSE;
    if ((sg_bCurrState == STATE_CONNECTED) || (sg_bCurrState == STATE_HW_INTERFACE_SELECTED))
    {
        if (nChannel <= sg_ucNoOfHardware)
        {
            if (nGetErrorCounter(nChannel, sErrorCnt) == S_OK)
            {
                hResult = S_OK;
            }
        }
        else
        {
            hResult = ERR_INVALID_CHANNEL;
        }
    }
    else
    {
        hResult = ERR_IMPROPER_STATE;
    }
    return hResult;
}
