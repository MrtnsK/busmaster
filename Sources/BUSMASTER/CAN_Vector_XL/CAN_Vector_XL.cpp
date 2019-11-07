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
 * \file      CAN_Vector_XL.cpp
 * \brief     Source file for Vector XL DIL functions
 * \author    Tobias Lorenz, Arunkumar Karri
 * \copyright Copyright (c) 2011, ETAS GmbH. All rights reserved.
 *
 * Source file for Vector XL DIL functions
 */
// CAN_Vector_XL.cpp : Defines the initialization routines for the DLL.
//

/* C++ includes */

#include "CAN_Vector_XL_stdafx.h"

/* C++ includes */
#include <sstream>
#include <string>
#include <vector>

/* Project includes */
#include "CAN_Vector_XL.h"
#include "Utility/Utility_Thread.h"
#include "BaseDIL_CAN_Controller.h"
#include "DIL_Interface/HardwareListingCAN.h"
#include "ChangeRegisters.h"
#include "../Application/MultiLanguage.h"
#include "Utility/MultiLanguageSupport.h"
#include "DILPluginHelperDefs.h"

#define DYNAMIC_XLDRIVER_DLL
#include "EXTERNAL/vxlapi.h"

#define USAGE_EXPORT
#include "CAN_Vector_XL_Extern.h"

BEGIN_MESSAGE_MAP(CCAN_Vector_XL, CWinApp)
END_MESSAGE_MAP()


/**
 * CCAN_Vector_XL construction
 */
CCAN_Vector_XL::CCAN_Vector_XL()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}


// The one and only CCAN_Vector_XL object
CCAN_Vector_XL theApp;


/**
 * CCAN_Vector_XL initialization
 */

static HINSTANCE ghLangInst=nullptr;

BOOL CCAN_Vector_XL::InitInstance()
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

/* function pointers */
XLGETAPPLCONFIG                xlGetApplConfig = nullptr;
XLSETAPPLCONFIG                xlSetApplConfig = nullptr;
XLGETDRIVERCONFIG              xlGetDriverConfig = nullptr;
XLGETCHANNELINDEX              xlGetChannelIndex = nullptr;
XLGETCHANNELMASK               xlGetChannelMask = nullptr;
XLOPENPORT                     xlOpenPort = nullptr;
XLSETTIMERRATE                 xlSetTimerRate = nullptr;
XLRESETCLOCK                   xlResetClock = nullptr;
XLSETNOTIFICATION              xlSetNotification = nullptr;
XLFLUSHRECEIVEQUEUE            xlFlushReceiveQueue = nullptr;
XLGETRECEIVEQUEUELEVEL         xlGetReceiveQueueLevel = nullptr;
XLACTIVATECHANNEL              xlActivateChannel = nullptr;
XLRECEIVE                      xlReceive = nullptr;
XLGETEVENTSTRING               xlGetEventString = nullptr;
XLGETERRORSTRING               xlGetErrorString = nullptr;
XLGETSYNCTIME                  xlGetSyncTime = nullptr;
XLGENERATESYNCPULSE            xlGenerateSyncPulse = nullptr;
XLPOPUPHWCONFIG                xlPopupHwConfig = nullptr;
XLDEACTIVATECHANNEL            xlDeactivateChannel = nullptr;
XLCLOSEPORT                    xlClosePort = nullptr;
XLSETTIMERBASEDNOTIFY          xlSetTimerBasedNotify = nullptr;
XLSETTIMERRATEANDCHANNEL       xlSetTimerRateAndChannel = nullptr;
XLGETLICENSEINFO               xlGetLicenseInfo = nullptr;
XLSETGLOBALTIMESYNC            xlSetGlobalTimeSync = nullptr;

/* CAN specific functions */
XLCANSETCHANNELOUTPUT          xlCanSetChannelOutput = nullptr;
XLCANSETCHANNELMODE            xlCanSetChannelMode = nullptr;
XLCANSETRECEIVEMODE            xlCanSetReceiveMode = nullptr;
XLCANSETCHANNELTRANSCEIVER     xlCanSetChannelTransceiver = nullptr;
XLCANSETCHANNELPARAMS          xlCanSetChannelParams = nullptr;
XLCANSETCHANNELPARAMSC200      xlCanSetChannelParamsC200 = nullptr;
XLCANSETCHANNELBITRATE         xlCanSetChannelBitrate = nullptr;
XLCANSETCHANNELACCEPTANCE      xlCanSetChannelAcceptance = nullptr;
XLCANADDACCEPTANCERANGE        xlCanAddAcceptanceRange = nullptr;
XLCANREMOVEACCEPTANCERANGE     xlCanRemoveAcceptanceRange = nullptr;
XLCANRESETACCEPTANCE           xlCanResetAcceptance = nullptr;
XLCANREQUESTCHIPSTATE          xlCanRequestChipState = nullptr;
XLCANFLUSHTRANSMITQUEUE        xlCanFlushTransmitQueue = nullptr;
XLCANTRANSMIT                  xlCanTransmit = nullptr;

static  CRITICAL_SECTION sg_CritSectForWrite;       // To make it thread safe

enum
{
    CREATE_MAP_TIMESTAMP = 0x1,
    CALC_TIMESTAMP_READY = 0x2,
};

/**
 * Current state machine
 */
static BYTE sg_byCurrState = CREATE_MAP_TIMESTAMP;

#define MAX_BUFF_ALLOWED 16
#define RECEIVE_EVENT_SIZE 1                // DO NOT EDIT! Currently 1 is supported only
#define RX_QUEUE_SIZE      4096             // internal driver queue size in CAN events
#define defUSB_MODE_ACTIVE                     1
#define defUSB_MODE_PASSIVE                    2
#define defUSB_MODE_SIMULATE                   3

/**
 * Client and Client Buffer map
 */
typedef struct tagClientBufMap
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
} SCLIENTBUFMAP;

/* Local variables */
static XLCLOSEDRIVER           xlDllCloseDriver = nullptr;
static XLOPENDRIVER            xlDllOpenDriver = nullptr;

/* Forward declarations*/
static int nInitHwNetwork(PSCONTROLLER_DETAILS InitData,UINT unDefaultChannelCnt = 0);
static BOOL bRemoveClient(DWORD dwClientId);
static DWORD dwGetAvailableClientSlot();
static BOOL bClientExist(std::string pcClientName, INT& Index);
static BOOL bClientIdExist(const DWORD& dwClientId);
static BOOL bGetClientObj(DWORD dwClientID, UINT& unClientIndex);
static void vRetrieveAndLog(DWORD /*dwErrorCode*/, char* File, int Line);
static BOOL bIsBufferExists(const SCLIENTBUFMAP& sClientObj, const CBaseCANBufFSE* pBuf);
static int nConnect(bool bConnect);
static int nGetNoOfConnectedHardware(void);
static BOOL bRemoveClientBuffer(CBaseCANBufFSE* RootBufferArray[MAX_BUFF_ALLOWED],
                                UINT& unCount, CBaseCANBufFSE* BufferToRemove);
static int nDisconnectFromDriver();
static int nSetApplyConfiguration();

// state variables
static bool sg_bIsConnected = false;
static unsigned short sg_ucControllerMode = defUSB_MODE_ACTIVE;
static CPARAM_THREADPROC sg_sParmRThread;
static int sg_nFRAMES = 128;
const int ENTRIES_IN_GBUF       = 2000;
static STCANDATA sg_asCANMsg;
static SCONTROLLER_DETAILS sg_ControllerDetails[defNO_OF_CHANNELS];
static INTERFACE_HW sg_HardwareIntr[defNO_OF_CHANNELS];


// TZM specific Global variables
#define CAN_MAX_ERRSTR 256
std::string sg_acErrStr = "";
static UINT sg_unClientCnt = 0;
#define MAX_CLIENT_ALLOWED 16
static SCLIENTBUFMAP sg_asClientToBufMap[MAX_CLIENT_ALLOWED];
static UINT sg_unDevChannelMap[defNO_OF_CHANNELS] = {(UINT)-1};

static HINSTANCE               hxlDll;

static HWND sg_hOwnerWnd = nullptr;

/////////////////////////////////////////////////////////////////////////////
// globals

char            g_AppName[XL_MAX_LENGTH+1]  = "BUSMASTER";            //!< Application name which is displayed in VHWconf
XLportHandle    g_xlPortHandle[MAX_CLIENT_ALLOWED]
    = {XL_INVALID_PORTHANDLE};               //!< Global porthandles
XLdriverConfig  g_xlDrvConfig;                                        //!< Contains the actual hardware configuration
XLaccess        g_xlChannelMask             = 0;                      //!< Global channelmask (includes all founded channels)
XLaccess        g_xlPermissionMask          = 0;                      //!< Global permissionmask (includes all founded channels)
unsigned int    g_BaudRate                  = 500000;                 //!< Default baudrate 500KBauds
int             g_silent                    = 0;                      //!< flag to visualize the message events (on/off)
unsigned int    g_TimerRate                 = 0;                      //!< Global timerrate (to toggel)
XLhandle        g_hDataEvent[MAX_CLIENT_ALLOWED]  = {0};
////////////////////////////////////////////////////////////////////////////

/* CDIL_CAN_VectorXL class definition */
class CDIL_CAN_VectorXL : public CBaseDIL_CAN_Controller
{
public:
    /* STARTS IMPLEMENTATION OF THE INTERFACE FUNCTIONS... */
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

CDIL_CAN_VectorXL* g_pouDIL_CAN_VectorXL = nullptr;

/**
* \brief         Returns the CDIL_CAN_VectorXL object
* \param[out]    ppvInterface, is void pointer to take back the reference to CDIL_CAN_VectorXL object
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
USAGEMODE HRESULT GetIDIL_CAN_Controller(void** ppvInterface)
{
    HRESULT hResult = S_OK;
    if ( nullptr == g_pouDIL_CAN_VectorXL )
    {
        if ((g_pouDIL_CAN_VectorXL = new CDIL_CAN_VectorXL) == nullptr)
        {
            hResult = S_FALSE;
        }
    }
    *ppvInterface = (void*) g_pouDIL_CAN_VectorXL;  /* Doesn't matter even if g_pouDIL_CAN_VectorXL is null */

    return hResult;
}

/**
 * Number of Channels
 */
static UINT sg_nNoOfChannels = 0;

static char sg_omErrStr[256] = {0};

// Count variables
static UCHAR sg_ucNoOfHardware = 0;

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

static SYSTEMTIME sg_CurrSysTime;
static UINT64 sg_TimeStamp = 0;

/**
 * Query Tick Count
 */
static LARGE_INTEGER sg_QueryTickCount;
static LARGE_INTEGER sg_lnFrequency;

/**
 * Channel information
 */
struct CChannel
{
    /* To store baud rate information */
    USHORT  m_usClock;
    USHORT  m_usSampling;
    UINT    m_unBaudrate;

    /* Bit Timing */
    USHORT  m_usBaudRate; /*BTR0,BTR1*/
    BYTE    m_bCNF1;
    BYTE    m_bCNF2;
    BYTE    m_bCNF3;

    /* Acceptance Filter information for standard and extended envelopes*/
    SACC_FILTER_INFO     m_sFilter[2];

    /**
     * To store controller state
     * 0 - Active State
     * 1 - Passive State
     * 2 - Bus Off State
     * -1 - For invalid value
     */
    UCHAR m_ucControllerState;

    /**
     * Pointer to corresponding XLchannelConfig
     */
    XLchannelConfig* m_pXLChannelInfo;

    /** Programmed warning limit of this channel */
    UCHAR m_ucWarningLimit;

    /** Tx Error counter value */
    UCHAR m_ucTxErrorCounter;

    /** Rx Error counter value */
    UCHAR m_ucRxErrorCounter;

    /** Peak Tx Error counter value */
    UCHAR m_ucPeakTxErrorCounter;

    /** Peak Rx Error counter value */
    UCHAR m_ucPeakRxErrorCounter;

    /** Tx Error Handler execution state */
    BOOL m_bTxErrorExecuted;

    /** Rx Error Handler execution state */
    BOOL m_bRxErrorExecuted;

    // Init members with default value
    CChannel()
    {
        // Baud Rate
        m_usBaudRate = defBAUD_RATE;

        // Controller state
        m_ucControllerState = defMODE_ACTIVE;

        // Pointer to corresponding XLchannelConfig
        m_pXLChannelInfo = nullptr;

        // Programmed warning limit of this channel
        m_ucWarningLimit = defWARNING_LIMIT_INT;

        // Tx Error counter value
        m_ucTxErrorCounter = 0;

        // Rx Error counter value
        m_ucRxErrorCounter = 0;

        // Peak Tx Error counter value
        m_ucPeakTxErrorCounter = 0;

        // Peak Rx Error counter value
        m_ucPeakRxErrorCounter = 0;

        // Tx Error Handler execution state
        m_bTxErrorExecuted = FALSE;

        // Rx Error Handler execution state
        m_bRxErrorExecuted = FALSE;
    }
};


/**
 * Array of channel objects. The size of this array is maximun number
 * of channels the application will support.
 */
static CChannel sg_aodChannels[ defNO_OF_CHANNELS ];

static INT sg_anSelectedItems[CHANNEL_ALLOWED];

/**
* \brief         Sets the application params.
* \param[in]     hWndOwner, is the main frame HWND value
* \param[in]     pILog, is pointer to error logger object
* \return        S_OK (always)
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_SetAppParams(HWND hWndOwner)
{
    sg_hOwnerWnd = hWndOwner;

    /* Initialise both the time parameters */
    GetLocalTime(&sg_CurrSysTime);
    sg_TimeStamp = 0x0;

    /* Query Tick Count */
    sg_QueryTickCount.QuadPart = 0;

    /* INITIALISE_ARRAY(sg_acErrStr); */
    sg_acErrStr = "";
    CAN_ManageMsgBuf(MSGBUF_CLEAR, 0, nullptr);

    return S_OK;
}


/**
* \brief         Unloads verdor's driver lobrary
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_UnloadDriverLibrary(void)
{
    if (hxlDll != nullptr)
    {
        FreeLibrary(hxlDll);
        hxlDll = nullptr;
    }

    return S_OK;
}

/**
* \brief         Registers the buffer pBufObj to the client ClientID
* \param[in]     bAction, contains one of the values MSGBUF_ADD or MSGBUF_CLEAR
* \param[in]     ClientID, is the client ID
* \param[in]     pBufObj, is pointer to CBaseCANBufFSE object
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj)
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
                /* Add msg buffer */
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
                /* clear msg buffer */
                if (pBufObj != nullptr)
                {
                    /* Remove only buffer mentioned */
                    bRemoveClientBuffer(sClientObj.pClientBuf, sClientObj.unBufCount, pBufObj);
                }
                else
                {
                    /* Remove all */
                    for (UINT i = 0; i < sClientObj.unBufCount; i++)
                    {
                        sClientObj.pClientBuf[i] = nullptr;
                    }
                    sClientObj.unBufCount = 0;
                }
                hResult = S_OK;
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
            /* clear msg buffer */
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
* \brief         Registers a client to the DIL.
* \param[in]     bRegister, if TRUE signifies 'Register', FALSE indicates 'Unregister'
* \param[out]    ClientID, is Client ID assigned, will be used for further client related calls
* \param[in]     pacClientName, is the client name
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName)
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
                /* Currently store the client information */
                if (_tcscmp(pacClientName, CAN_MONITOR_NODE) == 0)
                {
                    /* First slot is reserved to monitor node */
                    ClientID = 1;
                    _tcscpy(sg_asClientToBufMap[0].pacClientName, pacClientName);
                    sg_asClientToBufMap[0].dwClientID = ClientID;
                    sg_asClientToBufMap[0].unBufCount = 0;
                }
                else
                {
                    /*if (!bClientExist(CAN_MONITOR_NODE, Index))
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

                    if ( sg_bIsConnected == true )
                    {
                        // ------------------------------------
                        // open ONE port PER each node including all channels
                        // ------------------------------------
                        XLstatus xlStatus = xlOpenPort(&g_xlPortHandle[Index], g_AppName, g_xlChannelMask, &g_xlPermissionMask,
                                                       RX_QUEUE_SIZE, XL_INTERFACE_VERSION, XL_BUS_TYPE_CAN);
                        int a =0;
                    }

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

/**
* \brief         Returns the controller status.hEvent will be registered
*                and will be set whenever there is change in the controller status.
* \param[in]     hEvent, is the handle of the event
* \param[in]    unCntrlStatus, indicates contoller status
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_GetCntrlStatus(const HANDLE& /*hEvent*/, UINT& /*unCntrlStatus*/)
{
    return S_OK;
}

/**
* \brief         Loads vendor's driver library
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_LoadDriverLibrary(void)
{
    HRESULT hResult = S_OK;

    if (hxlDll != nullptr)
    {
        hResult = DLL_ALREADY_LOADED;
    }

    if (hResult == S_OK)
    {
        hxlDll = LoadLibrary("vxlapi.dll");
        if (hxlDll == nullptr)
        {
            hResult = ERR_LOAD_DRIVER;
        }
        else
        {
            xlDllOpenDriver           = (XLOPENDRIVER)                GetProcAddress(hxlDll,"xlOpenDriver");
            xlDllCloseDriver          = (XLCLOSEDRIVER)               GetProcAddress(hxlDll,"xlCloseDriver");

            /* bus independed functions */
            xlGetApplConfig            = (XLGETAPPLCONFIG)            GetProcAddress(hxlDll,"xlGetApplConfig");
            xlSetApplConfig            = (XLSETAPPLCONFIG)            GetProcAddress(hxlDll,"xlSetApplConfig");
            xlGetDriverConfig          = (XLGETDRIVERCONFIG)          GetProcAddress(hxlDll,"xlGetDriverConfig");
            xlGetChannelIndex          = (XLGETCHANNELINDEX)          GetProcAddress(hxlDll,"xlGetChannelIndex");
            xlGetChannelMask           = (XLGETCHANNELMASK)           GetProcAddress(hxlDll,"xlGetChannelMask");
            xlOpenPort                 = (XLOPENPORT)                 GetProcAddress(hxlDll,"xlOpenPort");
            xlSetTimerRate             = (XLSETTIMERRATE)             GetProcAddress(hxlDll,"xlSetTimerRate");
            xlResetClock               = (XLRESETCLOCK)               GetProcAddress(hxlDll,"xlResetClock");
            xlSetNotification          = (XLSETNOTIFICATION)          GetProcAddress(hxlDll,"xlSetNotification");
            xlFlushReceiveQueue        = (XLFLUSHRECEIVEQUEUE)        GetProcAddress(hxlDll,"xlFlushReceiveQueue");
            xlGetReceiveQueueLevel     = (XLGETRECEIVEQUEUELEVEL)     GetProcAddress(hxlDll,"xlGetReceiveQueueLevel");
            xlActivateChannel          = (XLACTIVATECHANNEL)          GetProcAddress(hxlDll,"xlActivateChannel");
            xlReceive                  = (XLRECEIVE)                  GetProcAddress(hxlDll,"xlReceive");
            xlGetEventString           = (XLGETEVENTSTRING)           GetProcAddress(hxlDll,"xlGetEventString");
            xlGetErrorString           = (XLGETERRORSTRING)           GetProcAddress(hxlDll,"xlGetErrorString");
            xlGenerateSyncPulse        = (XLGENERATESYNCPULSE)        GetProcAddress(hxlDll,"xlGenerateSyncPulse");
            xlGetSyncTime              = (XLGETSYNCTIME)              GetProcAddress(hxlDll,"xlGetSyncTime");
            xlPopupHwConfig            = (XLPOPUPHWCONFIG)            GetProcAddress(hxlDll,"xlPopupHwConfig");
            xlDeactivateChannel        = (XLDEACTIVATECHANNEL)        GetProcAddress(hxlDll,"xlDeactivateChannel");
            xlClosePort                = (XLCLOSEPORT)                GetProcAddress(hxlDll,"xlClosePort");
            //xlSetTimerBasedNotify      = (XLSETTIMERBASEDNOTIFY)      GetProcAddress(hxlDll,"xlSetTimerBasedNotify");
            //xlSetTimerRateAndChannel   = (XLSETTIMERRATEANDCHANNEL)   GetProcAddress(hxlDll, "xlSetTimerRateAndChannel");
            //xlGetLicenseInfo           = (XLGETLICENSEINFO)           GetProcAddress(hxlDll, "xlGetLicenseInfo");
            xlSetGlobalTimeSync        = (XLSETGLOBALTIMESYNC)        GetProcAddress(hxlDll, "xlSetGlobalTimeSync");


            /* CAN specific functions */
            xlCanSetChannelOutput      = (XLCANSETCHANNELOUTPUT)      GetProcAddress(hxlDll,"xlCanSetChannelOutput");
            xlCanSetChannelMode        = (XLCANSETCHANNELMODE)        GetProcAddress(hxlDll,"xlCanSetChannelMode");
            xlCanSetReceiveMode        = (XLCANSETRECEIVEMODE)        GetProcAddress(hxlDll,"xlCanSetReceiveMode");
            xlCanSetChannelTransceiver = (XLCANSETCHANNELTRANSCEIVER) GetProcAddress(hxlDll,"xlCanSetChannelTransceiver");
            xlCanSetChannelParams      = (XLCANSETCHANNELPARAMS)      GetProcAddress(hxlDll,"xlCanSetChannelParams");
            xlCanSetChannelParamsC200  = (XLCANSETCHANNELPARAMSC200)  GetProcAddress(hxlDll,"xlCanSetChannelParamsC200");
            xlCanSetChannelBitrate     = (XLCANSETCHANNELBITRATE)     GetProcAddress(hxlDll,"xlCanSetChannelBitrate");
            xlCanSetChannelAcceptance  = (XLCANSETCHANNELACCEPTANCE)  GetProcAddress(hxlDll,"xlCanSetChannelAcceptance");
            xlCanAddAcceptanceRange    = (XLCANADDACCEPTANCERANGE)    GetProcAddress(hxlDll,"xlCanAddAcceptanceRange");
            xlCanRemoveAcceptanceRange = (XLCANREMOVEACCEPTANCERANGE) GetProcAddress(hxlDll,"xlCanRemoveAcceptanceRange");
            xlCanResetAcceptance       = (XLCANRESETACCEPTANCE)       GetProcAddress(hxlDll,"xlCanResetAcceptance");
            xlCanRequestChipState      = (XLCANREQUESTCHIPSTATE)      GetProcAddress(hxlDll,"xlCanRequestChipState");
            xlCanFlushTransmitQueue    = (XLCANFLUSHTRANSMITQUEUE)    GetProcAddress(hxlDll,"xlCanFlushTransmitQueue");
            xlCanTransmit              = (XLCANTRANSMIT)              GetProcAddress(hxlDll,"xlCanTransmit");

            /* check for error */
            if (!xlDllOpenDriver || !xlDllCloseDriver ||
                    !xlGetApplConfig || !xlSetApplConfig ||
                    !xlGetDriverConfig || !xlGetChannelIndex ||
                    !xlGetChannelMask || !xlOpenPort ||
                    !xlSetTimerRate || !xlResetClock ||
                    !xlSetNotification || !xlFlushReceiveQueue ||
                    !xlGetReceiveQueueLevel || !xlActivateChannel ||
                    !xlReceive || !xlGetEventString ||
                    !xlGetErrorString || !xlGenerateSyncPulse ||
                    !xlGetSyncTime || !xlPopupHwConfig ||
                    !xlDeactivateChannel || !xlClosePort ||
                    /*!xlSetTimerBasedNotify || *//*!xlSetTimerRateAndChannel ||*/
                    /*!xlGetLicenseInfo || */!xlCanSetChannelOutput ||
                    !xlCanSetChannelMode || !xlCanSetReceiveMode ||
                    !xlCanSetChannelTransceiver || !xlCanSetChannelParams ||
                    !xlCanSetChannelParamsC200 || !xlCanSetChannelBitrate ||
                    !xlCanSetChannelAcceptance || !xlCanAddAcceptanceRange ||
                    !xlCanRemoveAcceptanceRange || !xlCanResetAcceptance ||
                    !xlCanRequestChipState || !xlCanFlushTransmitQueue ||
                    !xlCanTransmit || !xlSetGlobalTimeSync)
            {
                FreeLibrary(hxlDll);
                hResult = ERR_LOAD_DRIVER;
            }
        }
    }

    return hResult;
}

/**
* \brief         Performs intial operations.
*                Initializes filter, queue, controller config with default values.
* \param         void
* \return        S_OK if the open driver call successfull otherwise S_FALSE
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_PerformInitOperations(void)
{
    HRESULT hResult = S_FALSE;
    sg_asCANMsg.m_uDataInfo.m_sCANMsg.m_bCANFD = false;

    /* Register Monitor client */
    DWORD dwClientID = 0;
    CAN_RegisterClient(TRUE, dwClientID, CAN_MONITOR_NODE);

    // ------------------------------------
    // open the driver
    // ------------------------------------
    if (xlDllOpenDriver() == XL_SUCCESS)
    {
        hResult = S_OK;
    }

    //Initialize the selected channel items array to -1
    for ( UINT i = 0; i< CHANNEL_ALLOWED; i++ )
    {
        sg_anSelectedItems[i] = -1;
    }
    /*Set CAN FD field to false*/
    sg_asCANMsg.m_uDataInfo.m_sCANMsg.m_bCANFD = false;

    return hResult;
}

/**
* \brief         Performs closure operations.
* \param         void
* \return        S_OK if the CAN_StopHardware call successfull otherwise S_FALSE
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_PerformClosureOperations(void)
{
    HRESULT hResult = S_OK;

    hResult = CAN_StopHardware();

    if (hResult == S_OK)
    {
        // ------------------------------------
        // Close the driver
        // ------------------------------------
        if (nullptr != xlDllCloseDriver)
        {
            xlDllCloseDriver();
        }

        UINT ClientIndex = 0;
        while (sg_unClientCnt > 0)
        {
            bRemoveClient(sg_asClientToBufMap[ClientIndex].dwClientID);
        }

        if (hResult == S_OK)
        {
            sg_bCurrState = STATE_DRIVER_SELECTED;
        }
    }

    return hResult;
}

/**
* \brief         Gets the time mode mapping of the hardware. CurrSysTime
*                will be updated with the system time ref.
*                TimeStamp will be updated with the corresponding timestamp.
* \param[out]    CurrSysTime, is SYSTEMTIME structure
* \param[out]    TimeStamp, is UINT64
* \param[out]    QueryTickCount, is LARGE_INTEGER
* \return        S_OK for success
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER& QueryTickCount)
{
    CurrSysTime = sg_CurrSysTime;
    TimeStamp = sg_TimeStamp;
    QueryTickCount = sg_QueryTickCount;

    return S_OK;
}

/**
* \brief         Lists the hardware interface available.
* \param[out]    asSelHwInterface, is INTERFACE_HW_LIST structure
* \param[out]    nCount , is INT contains the selected channel count.
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_ListHwInterfaces(INTERFACE_HW_LIST& asSelHwInterface, INT& nCount, PSCONTROLLER_DETAILS InitData)
{
    USES_CONVERSION;
    HRESULT hResult = S_FALSE;

    hResult = nInitHwNetwork(InitData,nCount);
    if ( hResult == 0)
    {
        nCount = sg_nNoOfChannels;
        for (UINT i = 0; i < sg_nNoOfChannels; i++)
        {
            asSelHwInterface[i].m_dwIdInterface = i;
            if(nullptr == sg_aodChannels[i].m_pXLChannelInfo)
            {
                return S_FALSE;
            }
            unsigned int serialNumber = sg_aodChannels[i].m_pXLChannelInfo->serialNumber;
            std::ostringstream oss;
            oss << std::dec << serialNumber;
            std::string strTemp =  oss.str();
            asSelHwInterface[i].m_acDescription = strTemp;
            std::ostringstream oss1;
            oss1 << "Vector - " << sg_aodChannels[i].m_pXLChannelInfo->name << " SN - " <<serialNumber;
            //oss1 << "Channel Index - " <<(int)sg_aodChannels[i].m_pXLChannelInfo->channelIndex;
            sg_ControllerDetails[i].m_omHardwareDesc = oss1.str();
            asSelHwInterface[i].m_acNameInterface = oss1.str();
            sg_bCurrState = STATE_HW_INTERFACE_LISTED;
        }
        hResult = S_OK;
    }

    return hResult;
}

/**
* \brief         Selects the hardware interface selected by the user.
* \param[out]    asSelHwInterface, is INTERFACE_HW_LIST structure
* \param[out]    nCount , is INT contains the selected channel count.
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_SelectHwInterface(const INTERFACE_HW_LIST& asSelHwInterface, INT /*nCount*/)
{
    USES_CONVERSION;

    VALIDATE_POINTER_RETURN_VAL(hxlDll, S_FALSE);

    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_LISTED, ERR_IMPROPER_STATE);

    /* Check for the success */
    sg_bCurrState = STATE_HW_INTERFACE_SELECTED;
    for (UINT nCount = 0; nCount < sg_ucNoOfHardware; nCount++)
    {
        sg_ControllerDetails[nCount].m_omHardwareDesc = asSelHwInterface[nCount].m_acNameInterface;
    }

    return S_OK;
}

/**
* \brief         Deselects the selected hardware interface.
* \param         void
* \return        S_OK if CAN_ResetHardware call is success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_DeselectHwInterface(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_OK;

    sg_bCurrState = STATE_HW_INTERFACE_LISTED;

    return hResult;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Copies the controller config values into channel's
 * controller config structure.
 */
static BOOL bLoadDataFromContr(PSCONTROLLER_DETAILS pControllerDetails)
{
    BOOL bReturn = FALSE;
    // If successful
    if (pControllerDetails != nullptr)
    {
        for( UINT nIndex = 0; nIndex < sg_nNoOfChannels ; nIndex++ )
        {
            char* pcStopStr = nullptr;
            CChannel& odChannel = sg_aodChannels[ nIndex ];

            // Baudrate in BTR0BTR1 format
            odChannel.m_usBaudRate = static_cast <USHORT>(pControllerDetails[ nIndex ].m_nBTR0BTR1);
            // Baudrate value in decimal
            odChannel.m_unBaudrate = static_cast <UINT>(
                                         _tcstol( pControllerDetails[ nIndex ].m_omStrBaudrate.c_str(),
                                                  &pcStopStr, 10 ));

            // Get Warning Limit
            odChannel.m_ucWarningLimit = static_cast <UCHAR>(
                                             _tcstol( pControllerDetails[ nIndex ].m_omStrWarningLimit.c_str(),
                                                     &pcStopStr, 10 ));

            for ( int i = 0; i < defCAN_MSG_IDS; i++ )
            {
                // Get Acceptance Filter
                if ( pControllerDetails[ nIndex ].m_enmHWFilterType[i] == HW_FILTER_ACCEPT_ALL )
                {
                    odChannel.m_sFilter[i].m_ucACC_Code0 = 0;
                    odChannel.m_sFilter[i].m_ucACC_Code1 = 0;
                    odChannel.m_sFilter[i].m_ucACC_Code2 = 0;
                    odChannel.m_sFilter[i].m_ucACC_Code3 = 0;

                    odChannel.m_sFilter[i].m_ucACC_Mask0 = 0;
                    odChannel.m_sFilter[i].m_ucACC_Mask1 = 0;
                    odChannel.m_sFilter[i].m_ucACC_Mask2 = 0;
                    odChannel.m_sFilter[i].m_ucACC_Mask3 = 0;
                }
                else if( pControllerDetails[ nIndex ].m_enmHWFilterType[i] == HW_FILTER_REJECT_ALL )
                {
                    odChannel.m_sFilter[i].m_ucACC_Code0 = 0xFF;
                    odChannel.m_sFilter[i].m_ucACC_Code1 = 0xFF;
                    odChannel.m_sFilter[i].m_ucACC_Code2 = 0xFF;
                    odChannel.m_sFilter[i].m_ucACC_Code3 = 0xFF;

                    odChannel.m_sFilter[i].m_ucACC_Mask0 = 0xFF;
                    odChannel.m_sFilter[i].m_ucACC_Mask1 = 0xFF;
                    odChannel.m_sFilter[i].m_ucACC_Mask2 = 0xFF;
                    odChannel.m_sFilter[i].m_ucACC_Mask3 = 0xFF;

                    /* For standard message type */
                    if ( i == 0 )
                    {
                        odChannel.m_sFilter[i].m_ucACC_Code1 = 0x0F;
                        odChannel.m_sFilter[i].m_ucACC_Code2 = 0;
                        odChannel.m_sFilter[i].m_ucACC_Code3 = 0;

                        odChannel.m_sFilter[i].m_ucACC_Mask1 = 0x0F;
                        odChannel.m_sFilter[i].m_ucACC_Mask2 = 0;
                        odChannel.m_sFilter[i].m_ucACC_Mask3 = 0;
                    }
                }
                else
                {
                    odChannel.m_sFilter[i].m_ucACC_Code0 = static_cast <UCHAR>(
                            _tcstol( pControllerDetails[ nIndex ].m_omStrAccCodeByte1[i].c_str(),
                                     &pcStopStr, defBASE_HEX ));

                    odChannel.m_sFilter[i].m_ucACC_Code1 = static_cast <UCHAR>(
                            _tcstol( pControllerDetails[ nIndex ].m_omStrAccCodeByte2[i].c_str(),
                                     &pcStopStr, defBASE_HEX ));

                    odChannel.m_sFilter[i].m_ucACC_Code2 = static_cast <UCHAR>(
                            _tcstol( pControllerDetails[ nIndex ].m_omStrAccCodeByte3[i].c_str(),
                                     &pcStopStr, defBASE_HEX ));

                    odChannel.m_sFilter[i].m_ucACC_Code3 = static_cast <UCHAR>(
                            _tcstol(pControllerDetails[ nIndex ].m_omStrAccCodeByte4[i].c_str(),
                                    &pcStopStr, defBASE_HEX));

                    odChannel.m_sFilter[i].m_ucACC_Mask0 = static_cast <UCHAR>(
                            _tcstol( pControllerDetails[ nIndex ].m_omStrAccMaskByte1[i].c_str(),
                                     &pcStopStr, defBASE_HEX));

                    odChannel.m_sFilter[i].m_ucACC_Mask1 = static_cast <UCHAR>(
                            _tcstol( pControllerDetails[ nIndex ].m_omStrAccMaskByte2[i].c_str(),
                                     &pcStopStr, defBASE_HEX));

                    odChannel.m_sFilter[i].m_ucACC_Mask2 = static_cast <UCHAR>(
                            _tcstol( pControllerDetails[ nIndex ].m_omStrAccMaskByte3[i].c_str(),
                                     &pcStopStr, defBASE_HEX));

                    odChannel.m_sFilter[i].m_ucACC_Mask3 = static_cast <UCHAR>(
                            _tcstol( pControllerDetails[ nIndex ].m_omStrAccMaskByte4[i].c_str(),
                                     &pcStopStr, defBASE_HEX));
                }
                odChannel.m_sFilter[i].m_ucACC_Filter_Type = (UCHAR)i ;
            }


            // Get Baud Rate
            odChannel.m_usBaudRate = static_cast <USHORT>(
                                         pControllerDetails[ nIndex ].m_nBTR0BTR1 );
        }
        // Get Controller Mode
        // Consider only the first channel mode as controller mode
        sg_ucControllerMode = pControllerDetails[ 0 ].m_ucControllerMode;

        bReturn = TRUE;
    }
    return bReturn;
}

/**
* \brief         Callback function for configuration dialog
* \param[in]     pDatStream, contains SCONTROLLER_DETAILS structure
* \return        TRUE if CAN_SetConfigData call succeeds, else FALSE
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
BOOL Callback_DILTZM(BYTE /*Argument*/, PSCONTROLLER_DETAILS pDatStream, int /*Length*/)
{
    return (g_pouDIL_CAN_VectorXL->CAN_SetConfigData(pDatStream, 0) == S_OK);
}

/**
* \brief         Helper function to display configuration dialog
* \param[in]     pControllerDetails, is SCONTROLLER_DETAILS structure
* \param[in]     nCount , is the channel count
* \return        returns configuration confirmation status
* \authors       Arunkumar Karri
* \date          22.01.2013 Created
*/
int DisplayConfigurationDlg(HWND hParent, PSCONTROLLER_DETAILS pControllerDetails, UINT nCount)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    int nResult = WARNING_NOTCONFIRMED;

    CChangeRegisters ouChangeRegister(CWnd::FromHandle(hParent), pControllerDetails, nCount);
    ouChangeRegister.DoModal();
    nResult = ouChangeRegister.nGetInitStatus();

    return nResult;
}


/**
* \brief         Function to set the channel baud rate configured by user
* \param         void
* \return        canOK if succeeded, else respective error code
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
static int nSetBaudRate()
{
    XLstatus xlStatus = XL_SUCCESS;
    XLaccess xlChanMaskTx = 0;
    BYTE BTR0 = 0xC0, BTR1 = 0x3A;
    /* Set baud rate to all available hardware */
    for ( UINT unIndex = 0; unIndex < sg_nNoOfChannels; unIndex++)
    {
        xlStatus = 0;
        // Get Current channel reference
        CChannel& odChannel = sg_aodChannels[ unIndex ];

        BTR0 = odChannel.m_usBaudRate >> 8;
        BTR1 = odChannel.m_usBaudRate & 0xFF;

        //Get channel mask
        xlChanMaskTx = sg_aodChannels[unIndex].m_pXLChannelInfo->channelMask;
        // Set the baud rate
        xlStatus = xlCanSetChannelParamsC200(g_xlPortHandle[0], xlChanMaskTx, BTR0, BTR1);

        if( xlStatus != XL_SUCCESS )
        {
            xlStatus = xlCanSetChannelBitrate(g_xlPortHandle[0], xlChanMaskTx, odChannel.m_unBaudrate);

            // Check for failure
            if( xlStatus != XL_SUCCESS )
            {
                // break the loop
                unIndex = sg_nNoOfChannels;
            }
        }
    }
    return xlStatus;
}

/**
* \brief         Function to set the channel baud rate configured by user
* \param         void
* \return        canOK if succeeded, else respective error code
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
static int nSetFilter()
{
    XLstatus xlStatus = XL_SUCCESS;
    XLaccess xlChanMaskTx = 0;

    // To set no. shifts
    int nShift = sizeof( UCHAR ) * defBITS_IN_BYTE;
    // Set the client filter
    for ( UINT unIndex = 0; unIndex < sg_nNoOfChannels; unIndex++)
    {
        // Create DWORD Filter
        ULONG ulCode = 0, ulMask = 0;
        // Get the Filter
        for ( UINT i = 0 ; i < defCAN_MSG_IDS ; i++ )
        {
            const SACC_FILTER_INFO& sFilter = sg_aodChannels[ unIndex ].m_sFilter[i];

            // Create Code
            ulCode = ( sFilter.m_ucACC_Code3 << nShift * 3 ) |
                     ( sFilter.m_ucACC_Code2 << nShift * 2 ) |
                     ( sFilter.m_ucACC_Code1 << nShift ) |
                     sFilter.m_ucACC_Code0;
            // Create Mask
            ulMask = ( sFilter.m_ucACC_Mask3 << nShift * 3 ) |
                     ( sFilter.m_ucACC_Mask2 << nShift * 2 ) |
                     ( sFilter.m_ucACC_Mask1 << nShift ) |
                     sFilter.m_ucACC_Mask0;

            //Get channel mask
            xlChanMaskTx = sg_aodChannels[unIndex].m_pXLChannelInfo->channelMask;

            // Set the acceptance filter for all the client handles
            for (UINT unClientID = 0; unClientID < sg_unClientCnt; unClientID++)
            {
                // Id Range 1 = XL_CAN_STD, 2 = XL_CAN_EXT
                xlStatus = xlCanSetChannelAcceptance(g_xlPortHandle[unClientID], xlChanMaskTx, ulCode, ulMask, i+1);
            }
        }
        if( xlStatus != XL_SUCCESS )
        {
            vRetrieveAndLog(xlStatus, __FILE__, __LINE__);
            // Stop the loop as there is an error
            unIndex = sg_nNoOfChannels;
        }

    }

    return xlStatus;
}

/**
* \brief         Function to apply filters and baud rate to channels
* \param         void
* \return        defERR_OK if succeeded, else respective error code
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
static int nSetApplyConfiguration()
{
    int nReturn = S_OK;

    // Set baud rate only for hardware network
    if( sg_ucControllerMode != defUSB_MODE_SIMULATE )
    {
        // Set Baud Rate
        nReturn = nSetBaudRate ();
    }

    if( nReturn == S_OK )
    {
        // Set Filter
        nReturn = nSetFilter ();
    }

    return nReturn;
}

void CAN_setHardware(int nChannelCount)
{
    sg_HardwareList.m_nChannelCount = nChannelCount;
    for (UINT nChannel = 0; nChannel < nChannelCount; nChannel++)
    {
        std::ostringstream oss1;
        oss1 << "Vector - " << sg_HardwareIntr[nChannel].m_acDescription.c_str() << " SN - " <<sg_HardwareIntr[nChannel].m_dwVendor;
        //oss1 << "Channel Index - " <<(int)nChannel;
        std::string omHardwareChannel = oss1.str();
        sg_HardwareList.m_omHardwareChannel[nChannel] = omHardwareChannel;
        sg_HardwareList.m_ouChannelDetails[nChannel].m_omVendorId = g_xlDrvConfig.channel[sg_HardwareIntr[nChannel].m_dwIdInterface].serialNumber;
        sg_HardwareList.m_ouChannelDetails[nChannel].m_omChannelName = g_xlDrvConfig.channel[sg_HardwareIntr[nChannel].m_dwIdInterface].name;
        sg_HardwareList.m_ouChannelDetails[nChannel].m_omChannelIndex = g_xlDrvConfig.channel[sg_HardwareIntr[nChannel].m_dwIdInterface].hwChannel;
    }
}
/**
* \brief         Sets the controller configuration data supplied by ConfigFile.
* \param[in]     ConfigFile, is SCONTROLLER_DETAILS structure
* \param[in]     Length , is INT
* \return        S_OK for success
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_SetConfigData(PSCONTROLLER_DETAILS ConfigFile, int /* Length */)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    USES_CONVERSION;

    /* Fill the hardware description details */
    for (UINT nCount = 0; nCount < sg_ucNoOfHardware; nCount++)
    {
        ((PSCONTROLLER_DETAILS)ConfigFile)[nCount].m_omHardwareDesc = sg_ControllerDetails[nCount].m_omHardwareDesc;
    }
    for (UINT i = 0; i < defNO_OF_CHANNELS; i++)
    {
        sg_ControllerDetails[i] = ConfigFile[i];
    }
    bLoadDataFromContr(sg_ControllerDetails);

    return S_OK;
}

/**
* \brief         Writes the message 'sCanData' to the corresponding clients buffer
* \param[out]    sCanData, is STCANDATA structure
* \param[out]    unClientIndex , is UINT containing client ID
* \return        void
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static void vWriteIntoClientsBuffer(STCANDATA& sCanData, UINT unClientIndex)
{
    /* Write into the respective client's buffer */
    for (UINT j = 0; j < sg_asClientToBufMap[unClientIndex].unBufCount; j++)
    {
        sg_asClientToBufMap[unClientIndex].pClientBuf[j]->WriteIntoBuffer(&sCanData);
    }
}

/**
* \brief         Function to create time mode mapping
* \param[in]     hDataEvent, is HANDLE
* \return        void
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static void vCreateTimeModeMapping(HANDLE hDataEvent)
{
    WaitForSingleObject(hDataEvent, INFINITE);
    GetLocalTime(&sg_CurrSysTime);
    /*Query Tick Count*/
    QueryPerformanceCounter(&sg_QueryTickCount);
}

/**
* \brief         This will convert the error code from vector driver format
*                to the format that is used by BUSMASTER.
* \param[in]     lError Error code in Peak USB driver format
* \param[in]     byDir  Error direction Tx/Rx
* \return        UCHAR which indicates error code
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static UCHAR USB_ucGetErrorCode(LONG lError, BYTE byDir)
{
    UCHAR ucReturn = 0;

    // Tx Errors
    if( byDir == 1)
    {
        if (lError & XL_CAN_MSG_FLAG_ERROR_FRAME)
        {
            ucReturn = BIT_ERROR_TX;
        }
        else if (lError & XL_CAN_MSG_FLAG_NERR )
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
        if (lError & XL_CAN_MSG_FLAG_ERROR_FRAME)
        {
            ucReturn = BIT_ERROR_RX;
        }
        else if (lError & XL_CAN_MSG_FLAG_NERR)
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
* \brief         This will classify the messages, which can be one of Rx, Tx or
*                Error messages. In case of Err messages this identifies under
*                what broader category (Rx / Tx) does this occur.
* \param[in]     XLevent& xlEvent message polled from the bus in XLevent format
* \param[out]    sCanData Application specific data format
* \return        TRUE (always)
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static BYTE bClassifyMsgType(XLevent& xlEvent, STCANDATA& sCanData)
{
    sCanData.m_lTickCount.QuadPart = (LONGLONG)xlEvent.timeStamp / 100000;

    if (CREATE_MAP_TIMESTAMP == sg_byCurrState)
    {
        LARGE_INTEGER g_QueryTickCount;
        QueryPerformanceCounter(&g_QueryTickCount);
        UINT64 unConnectionTime;
        unConnectionTime = ((g_QueryTickCount.QuadPart * 10000) / sg_lnFrequency.QuadPart) - sg_TimeStamp;
        //Time difference should be +ve value
        if(sCanData.m_lTickCount.QuadPart >= unConnectionTime)
        {
            sg_TimeStamp  = (LONGLONG)(sCanData.m_lTickCount.QuadPart - unConnectionTime);
        }
        else
        {
            sg_TimeStamp  = (LONGLONG)(unConnectionTime - sCanData.m_lTickCount.QuadPart);
        }

        sg_byCurrState = CALC_TIMESTAMP_READY;
    }

    if ( !(xlEvent.tagData.msg.flags & XL_CAN_MSG_FLAG_ERROR_FRAME) &&
            !(xlEvent.tagData.msg.flags & XL_CAN_MSG_FLAG_OVERRUN)     &&
            !(xlEvent.tagData.msg.flags & XL_CAN_MSG_FLAG_NERR)         )
    {
        ///* Check for RTR Message */
        if (xlEvent.tagData.msg.flags & XL_CAN_MSG_FLAG_REMOTE_FRAME)
        {
            sCanData.m_ucDataType = RX_FLAG;
            sCanData.m_uDataInfo.m_sCANMsg.m_ucRTR = TRUE;
        }
        else
        {
            sCanData.m_uDataInfo.m_sCANMsg.m_ucRTR = FALSE;
        }

        if (xlEvent.tagData.msg.flags & XL_CAN_MSG_FLAG_TX_COMPLETED)
        {
            sCanData.m_ucDataType = TX_FLAG;
        }
        else
        {
            sCanData.m_ucDataType = RX_FLAG;
        }

        if ( xlEvent.tagData.msg.dlc <= 8 ) /* Valid CAN message length */
        {
            /* Copy data length */
            sCanData.m_uDataInfo.m_sCANMsg.m_ucDataLen = (UCHAR)xlEvent.tagData.msg.dlc;

            /* Copy the message data */
            memcpy(sCanData.m_uDataInfo.m_sCANMsg.m_ucData,
                   xlEvent.tagData.msg.data, xlEvent.tagData.msg.dlc);
        }

        /* Copy the message ID */
        sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID = (UINT)xlEvent.tagData.msg.id;

        /* Check for extended message indication */
        sCanData.m_uDataInfo.m_sCANMsg.m_ucEXTENDED =
            (xlEvent.tagData.msg.id & XL_CAN_EXT_MSG_ID) ? TRUE : FALSE;

        if ( sCanData.m_uDataInfo.m_sCANMsg.m_ucEXTENDED )
        {
            sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID ^= XL_CAN_EXT_MSG_ID;    // make it std
        }

        /* Set channel ID */
        sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel = (UCHAR)sg_unDevChannelMap[xlEvent.chanIndex] + 1;
    }
    else
    {
        sCanData.m_ucDataType = ERR_FLAG;
        // Set bus error as default error. This will be
        // Modified by the function USB_ucHandleErrorCounter
        sCanData.m_uDataInfo.m_sErrInfo.m_ucErrType = ERROR_BUS;
        // Assign the channel number
        sCanData.m_uDataInfo.m_sErrInfo.m_ucChannel = (UCHAR)sg_unDevChannelMap[xlEvent.chanIndex] + 1;
        sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel = (UCHAR)sg_unDevChannelMap[xlEvent.chanIndex] + 1;

        // Assign error type in the Error Capture register
        // and the direction of the error
        BOOL bIsTxMsg = FALSE;
        if (xlEvent.tagData.msg.flags & XL_CAN_MSG_FLAG_TX_COMPLETED)
        {
            bIsTxMsg = TRUE;
        }
        sCanData.m_uDataInfo.m_sErrInfo.m_ucReg_ErrCap =
            USB_ucGetErrorCode(xlEvent.tagData.msg.flags, (BYTE) bIsTxMsg);
        //explaination of error bit
        sCanData.m_uDataInfo.m_sErrInfo.m_nSubError= 0;
    }

    return TRUE;
}

/**
* \brief         Processing of the received packets from bus
* \param[in]     XLevent& xlEvent message polled from the bus in XLevent format
* \param[in]     unClientIndex is the client index
* \return        void
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static void ProcessCANMsg(XLevent& xlEvent, UINT unClientIndex)
{
    int nSize = sg_nFRAMES;

    bClassifyMsgType(xlEvent, sg_asCANMsg);

    vWriteIntoClientsBuffer(sg_asCANMsg, unClientIndex);
}

/**
* \brief         Read thread procedure
* \param[in]     pVoid contains the CPARAM_THREADPROC class object
* \return        void
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
DWORD WINAPI CanMsgReadThreadProc_CAN_Vector_XL(LPVOID pVoid)
{
    USES_CONVERSION;
    XLstatus xlStatus = XL_SUCCESS;

    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;

    /* Assign thread action to CREATE_TIME_MAP */
    pThreadParam->m_unActionCode = CREATE_TIME_MAP;

    /* Set the event to CAN_Vector_XL driver for wakeup and frame arrival notification */
    for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        xlStatus = xlSetNotification (g_xlPortHandle[i], &g_hDataEvent[i], 1);
    }

    /* Set the thread action event to first notification event handle */
    if (g_hDataEvent[0] != nullptr)
    {
        pThreadParam->m_hActionEvent = g_hDataEvent[0];
    }

    /* Get the handle to the controller and validate it */
    VALIDATE_POINTER_RETURN_VAL( pThreadParam->m_hActionEvent, (DWORD)-1 );

    DWORD dwResult = 0;
    unsigned int msgsrx;
    XLevent  xlEvent;

    bool bLoopON = true;

    while (bLoopON)
    {
        WaitForMultipleObjects(sg_unClientCnt, g_hDataEvent, FALSE, INFINITE);
        switch (pThreadParam->m_unActionCode)
        {
            case INVOKE_FUNCTION:
            {
                bool bMoreDataExist;
                do
                {
                    bMoreDataExist = false;
                    /* Read all the available CAN messages using 'xlReceive'
                       across all the port handles */
                    for (UINT i = 0; i < sg_unClientCnt; i++)
                    {
                        msgsrx = RECEIVE_EVENT_SIZE;
                        xlStatus = xlReceive(g_xlPortHandle[i], &msgsrx, &xlEvent);
                        switch (xlStatus)
                        {
                            case XL_SUCCESS:
                            {
                                ProcessCANMsg(xlEvent, i);
                                bMoreDataExist = true;
                            }
                            break;

                            default:
                                break;
                        }
                    }
                }
                while (bMoreDataExist);
            }
            break;
            case EXIT_THREAD:
            {
                bLoopON = false;
            }
            break;
            case CREATE_TIME_MAP:
            {
                //vCreateTimeModeMapping(pThreadParam->m_hActionEvent);
                SetEvent(pThreadParam->m_hActionEvent);
                pThreadParam->m_unActionCode = INVOKE_FUNCTION;
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
    SetEvent(pThreadParam->hGetExitNotifyEvent());
    for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        ResetEvent(g_hDataEvent[i]);
        g_hDataEvent[i] = nullptr;
    }
    pThreadParam->m_hActionEvent = nullptr;

    return 0;
}

/**
* \brief         connects to the channels and initiates read thread.
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_StartHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    USES_CONVERSION;
    HRESULT hResult = S_OK;

    //Connect to the network
    hResult = nConnect(TRUE);
    if (hResult == S_OK)
    {
        hResult = S_OK;
        sg_bCurrState = STATE_CONNECTED;
        vCreateTimeModeMapping(g_hDataEvent[0]);
    }
    else
    {
        //log the error for open port failure
        vRetrieveAndLog(hResult, __FILE__, __LINE__);
        hResult = ERR_LOAD_HW_INTERFACE;
    }

    //If everything is ok start the read thread
    if (hResult == S_OK)
    {
        if (sg_sParmRThread.bStartThread(CanMsgReadThreadProc_CAN_Vector_XL))
        {
            hResult = S_OK;
        }
    }

    return hResult;
}

/**
* \brief         This will close the connection with the driver. This will be
*                called before deleting HI layer. This will be called during
*                application close.
* \param         void
* \return        Operation Result. 0 incase of no errors. Failure Error codes(-1) otherwise.
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static int nDisconnectFromDriver()
{
    int nReturn = 0;
    XLstatus xlStatus;

    for ( UINT i = 0; i< sg_unClientCnt; i++ )
    {
        if (g_xlPortHandle[i] != XL_INVALID_PORTHANDLE)
        {
            xlStatus = xlDeactivateChannel( g_xlPortHandle[i], g_xlChannelMask );
            xlStatus = xlClosePort(g_xlPortHandle[i]);
            //SSH + fix for Issue# 393 - cannot disconnect bus master in some scenarios.
            //g_xlPortHandle[i] = XL_INVALID_PORTHANDLE;
            //SSH -
        }
        else
        {
            nReturn = -1;
        }
    }
    sg_bCurrState = STATE_HW_INTERFACE_SELECTED;

    return nReturn;
}

/**
* \brief         Function to map device channels with BUSMASTER channel order
* \param         void
* \return        void
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static void vMapDeviceChannelIndex()
{
    //Reset previous channel ID assignment if any
    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        sg_unDevChannelMap[sg_aodChannels[i].m_pXLChannelInfo->channelIndex] = i;
    }
}

/**
* \brief         This function will connect the tool with hardware. This will
*                establish the data link between the application and hardware.
* \param[in]     bConnect TRUE to Connect, FALSE to Disconnect
* \return        Returns defERR_OK if successful otherwise corresponding Error code.
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static int nConnect(bool bConnect)
{
    int nReturn = -1;
    XLstatus xlStatus = XL_SUCCESS;

    if (!sg_bIsConnected && bConnect) // Disconnected and to be connected
    {
        /* Set the permission mask for all channel access */
        g_xlPermissionMask = g_xlChannelMask;

        for (UINT i = 0; i < sg_unClientCnt; i++)
        {
            // ------------------------------------
            // open ONE port PER each node including all channels
            // ------------------------------------
            xlStatus = xlOpenPort(&g_xlPortHandle[i], g_AppName, g_xlChannelMask, &g_xlPermissionMask,
                                  RX_QUEUE_SIZE, XL_INTERFACE_VERSION, XL_BUS_TYPE_CAN);
        }

        if (XL_SUCCESS == xlStatus)
        {
            //Calculate connected Timestamp
            QueryPerformanceCounter(&sg_QueryTickCount);
            // Get frequency of the performance counter
            QueryPerformanceFrequency(&sg_lnFrequency);
            // Convert it to time stamp with the granularity of hundreds of microsecond
            //if (sg_QueryTickCount.QuadPart * 10000 > sg_QueryTickCount.QuadPart)
            if ((sg_QueryTickCount.QuadPart * 10000) > sg_lnFrequency.QuadPart)
            {
                sg_TimeStamp = (sg_QueryTickCount.QuadPart * 10000) / sg_lnFrequency.QuadPart;
            }
            else
            {
                sg_TimeStamp = (sg_QueryTickCount.QuadPart / sg_lnFrequency.QuadPart) * 10000;
            }

            /* Transit into 'CREATE TIME MAP' state */
            sg_byCurrState = CREATE_MAP_TIMESTAMP;
            vMapDeviceChannelIndex();
            sg_bIsConnected = bConnect;
            // ------------------------------------
            // go with all selected channels on bus
            // ------------------------------------
            for ( UINT i = 0; i< sg_unClientCnt; i++ )
            {
                xlStatus = xlActivateChannel(g_xlPortHandle[i], g_xlChannelMask, XL_BUS_TYPE_CAN, XL_ACTIVATE_RESET_CLOCK);
            }
            if(xlStatus == XL_SUCCESS)
            {
                nReturn = S_OK;
                nSetApplyConfiguration();
            }
        }
    }
    else if (sg_bIsConnected && !bConnect) // Connected & to be disconnected
    {
        sg_bIsConnected = bConnect;
        Sleep(0); // Let other threads run for once
        nReturn = nDisconnectFromDriver();
    }
    else
    {
        nReturn = S_OK;
    }
    if ( sg_bIsConnected )
    {
        InitializeCriticalSection(&sg_CritSectForWrite);
    }
    else
    {
        DeleteCriticalSection(&sg_CritSectForWrite);
    }

    return nReturn;
}

/**
* \brief         Stops the controller.
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_StopHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_OK;

    //Terminate the read thread
    sg_sParmRThread.bTerminateThread();

    hResult = nConnect(FALSE);
    if (hResult == S_OK)
    {
        hResult = S_OK;
        sg_bCurrState = STATE_HW_INTERFACE_SELECTED;
    }
    else
    {
        //log the error for open port failure
        vRetrieveAndLog(hResult, __FILE__, __LINE__);
        hResult = ERR_LOAD_HW_INTERFACE;
    }

    return hResult;
}

/**
* \brief         Function to get Controller status
* \param[out]    StatusData, is s_STATUSMSG structure
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_GetCurrStatus(STATUSMSG& StatusData)
{
    StatusData.wControllerStatus = NORMAL_ACTIVE;

    return S_OK;
}

/**
* \brief         Gets the Tx queue configured.
* \param[out]    pouFlxTxMsgBuffer, is BYTE*
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_GetTxMsgBuffer(BYTE*& /*pouFlxTxMsgBuffer*/)
{
    return S_OK;
}

/**
* \brief         This will send a CAN message to the driver. In case of USB
*                this will write the message in to the driver buffer and will
*                return. In case if parallel port mode this will write the
*                message and will wait for the ACK event from the driver. If
*                the event fired this will return 0. Otherwise this will return
*                wait time out error. In parallel port it is a blocking call
*                and in case of failure condition this will take 2 seconds.
* \param[in]     sMessage Message to Transmit
* \return        Operation Result. 0 incase of no errors. Failure Error codes otherwise.
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static int nWriteMessage(STCAN_MSG sMessage, DWORD dwClientID)
{
    int nReturn = -1;
    XLaccess xlChanMaskTx = 0;
    UINT unClientIndex = (UINT)-1;

    if ((sMessage.m_ucChannel > 0) &&
            (sMessage.m_ucChannel <= sg_nNoOfChannels) &&
            (sg_nNoOfChannels <= defNO_OF_CHANNELS))
    {
        static XLevent       xlEvent;
        XLstatus             xlStatus = 0;
        unsigned int         messageCount = 1;

        memset(&xlEvent, 0, sizeof(xlEvent));

        xlEvent.tag                 = XL_TRANSMIT_MSG;
        /* if it is an extended frame */
        if (sMessage.m_ucEXTENDED == 1)
        {
            sMessage.m_unMsgID ^= XL_CAN_EXT_MSG_ID; // toggle ext/std
        }
        /* in case of remote frame */
        if (sMessage.m_ucRTR == 1)
        {
            xlEvent.tagData.msg.flags   = XL_CAN_MSG_FLAG_REMOTE_FRAME;
        }
        else
        {
            xlEvent.tagData.msg.flags   = 0;
        }
        xlEvent.tagData.msg.id      = sMessage.m_unMsgID;
        xlEvent.tagData.msg.dlc     = (unsigned short)sMessage.m_ucDataLen;
        memcpy(&xlEvent.tagData.msg.data, &sMessage.m_ucData, sMessage.m_ucDataLen);

        //Get channel mask
        xlChanMaskTx = sg_aodChannels[sMessage.m_ucChannel - 1].m_pXLChannelInfo->channelMask;
        //Transmit message
        if ( bGetClientObj(dwClientID, unClientIndex) )
        {
            xlStatus = xlCanTransmit(g_xlPortHandle[unClientIndex], xlChanMaskTx, &messageCount, &xlEvent);
        }
        //set result
        nReturn = xlStatus;
    }

    return nReturn;
}

/**
* \brief         Sends STCAN_MSG structure from the client dwClientID.
* \param[in]     dwClientID is the client ID
* \param[in]     sMessage is the application specific CAN message structure
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& sMessage)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_FALSE;
    if (bClientIdExist(dwClientID))
    {
        if (sMessage.m_ucChannel <= sg_nNoOfChannels)
        {
            EnterCriticalSection(&sg_CritSectForWrite); // Lock the buffer
            if (nWriteMessage(sMessage, dwClientID) == S_OK)
            {
                hResult = S_OK;
            }
            LeaveCriticalSection(&sg_CritSectForWrite); // Unlock the buffer
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
* \brief         Gets bus config info.
* \param[out]    BusInfo, is BYTE
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_GetBusConfigInfo(BYTE* /*BusInfo*/)
{
    return S_OK;
}


/**
* \brief         Gets last occured error and puts inside acErrorStr.
* \param[out]    acErrorStr, is CHAR contains error string
* \param[in]     nLength, is INT
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_GetLastErrorString(std::string& acErrorStr)
{
    acErrorStr = sg_acErrStr;
    return S_OK;
}

/**
* \brief         This function will check all hardware connectivity by activating selected channels.
* \param[out]    ucaTestResult Array that will hold test result.
                 TRUE if hardware present and false if not connected
* \param[in]     nChannel, indicates channel ID
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static int nTestHardwareConnection(UCHAR& ucaTestResult, UINT nChannel) //const
{
    int nReturn = 0;
    XLstatus xlStatus;
    if (nChannel < sg_nNoOfChannels)
    {
        // ------------------------------------
        // go with all selected channels on bus
        // ------------------------------------
        xlStatus = xlActivateChannel(g_xlPortHandle[0], g_xlChannelMask, XL_BUS_TYPE_CAN, XL_ACTIVATE_RESET_CLOCK);
        if ( xlStatus == XL_SUCCESS )
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
* \brief         Gets the controller parametes of the channel based on the request.
* \param[out]    lParam, the value of the controller parameter requested.
* \param[in]     nChannel, indicates channel ID
* \param[in]     eContrParam, indicates controller parameter
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam)
{
    HRESULT hResult = S_OK;
    if ((sg_bCurrState == STATE_HW_INTERFACE_SELECTED) || (sg_bCurrState == STATE_CONNECTED))
    {
        switch (eContrParam)
        {
            case NUMBER_HW:
            {
                lParam = sg_nNoOfChannels;
            }
            break;
            case NUMBER_CONNECTED_HW:
            {
                if (nGetNoOfConnectedHardware() > 0)
                {
                    lParam = sg_nNoOfChannels;
                }
                else
                {
                    hResult = S_FALSE;
                }
            }
            break;
            case DRIVER_STATUS:
            {
                lParam = true;
            }
            break;
            case HW_MODE:
            {
                if (nChannel < sg_nNoOfChannels)
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

HRESULT CDIL_CAN_VectorXL::CAN_SetControllerParams(INT nValue, ECONTR_PARAM eContrparam)
{
    switch(eContrparam)
    {
        case HW_MODE:
        {
            switch(nValue)
            {
                case defMODE_ACTIVE:
                {

                    for( UINT unClientIndex = 0; unClientIndex < sg_nNoOfChannels; unClientIndex++ )
                    {
                        //Get channel mask
                        XLuint64 xlChanMaskTx = sg_aodChannels[unClientIndex].m_pXLChannelInfo->channelMask;
                        //Transmit message
                        //if ( bGetClientObj(dwClientID, unClientIndex) )
                        {
                            xlCanSetChannelOutput(g_xlPortHandle[unClientIndex], xlChanMaskTx, 1);
                        }
                    }
                }
                break;
                case defMODE_PASSIVE:
                {
                    for( UINT unClientIndex = 0; unClientIndex < sg_nNoOfChannels; unClientIndex++ )
                    {
                        //Get channel mask
                        XLuint64 xlChanMaskTx = sg_aodChannels[unClientIndex].m_pXLChannelInfo->channelMask;
                        //Transmit message
                        //if ( bGetClientObj(dwClientID, unClientIndex) )
                        {
                            xlCanSetChannelOutput(g_xlPortHandle[unClientIndex], xlChanMaskTx, 3);
                        }
                    }
                }
                break;
            }
        }
    }
    return S_OK;
}

/**
* \brief         Gets the error counter for corresponding channel.
* \param[out]    sErrorCnt, is SERROR_CNT structure
* \param[in]     nChannel, indicates channel ID
* \param[in]     eContrParam, indicates controller parameter
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_CAN_VectorXL::CAN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam)
{
    HRESULT hResult = S_OK;
    if ((sg_bCurrState == STATE_CONNECTED) || (sg_bCurrState == STATE_HW_INTERFACE_SELECTED))
    {
        if (nChannel <= sg_nNoOfChannels)
        {
            if (eContrParam == ERR_CNT)
            {
                sErrorCnt.m_ucTxErrCount = sg_aodChannels[nChannel].m_ucTxErrorCounter;
                sErrorCnt.m_ucRxErrCount = sg_aodChannels[nChannel].m_ucRxErrorCounter;
            }
            else if (eContrParam == PEAK_ERR_CNT)
            {
                sErrorCnt.m_ucTxErrCount = sg_aodChannels[nChannel].m_ucPeakTxErrorCounter;
                sErrorCnt.m_ucRxErrCount = sg_aodChannels[nChannel].m_ucPeakRxErrorCounter;
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

/* HELPER FUNCTIONS START */

/**
* \brief         Finds the number of hardware connected.
* \param         void
* \return        defERR_OK if successful otherwise corresponding Error code.
*                0, Query successful, but no device found
*                > 0, Number of devices found
*                < 0, query for devices unsuccessful
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static int nGetNoOfConnectedHardware(void)
{
    int nResult = 0;
    XLstatus xlStatus = XL_SUCCESS;

    // ------------------------------------
    // get the hardware configuration
    // ------------------------------------
    if ( XL_SUCCESS == xlStatus )
    {
        xlStatus = xlGetDriverConfig(&g_xlDrvConfig);
    }

    if (XL_SUCCESS == xlStatus)
    {
        // ------------------------------------
        // select the wanted channels
        // ------------------------------------
        for (UINT i=0; i < g_xlDrvConfig.channelCount; i++)
        {
            // we take all hardware we found and
            // check that we have only CAN cabs/piggy's
            // at the moment there is no VN8910 XLAPI support!
            //if ( /*(g_xlDrvConfig.channel[i].channelBusCapabilities & XL_BUS_ACTIVE_CAP_CAN)
            //  &&*/ (g_xlDrvConfig.channel[i].hwType != XL_HWTYPE_VN8900) )
            //{
            if ( /*(g_xlDrvConfig.channel[i].hwType == XL_HWTYPE_CANCASEXL) &&*/
                !(g_xlDrvConfig.channel[i].channelBusCapabilities & XL_BUS_ACTIVE_CAP_CAN) )
            {
                continue;
            }

            nResult++;
            // }
        }
        if (!nResult)
        {
            _tcscpy(sg_omErrStr, _("No available channels found! (e.g. no CANcabs...)"));
            xlStatus = XL_ERROR;
        }
    }
    else
    {
        _tcscpy(sg_omErrStr, _("Problem Finding Device!"));
        nResult = -1;
    }
    /* Return the operation result */
    return nResult;
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
	IChangeRegisters* pAdvancedSettings = new CChangeRegisters(nullptr, InitData, nCount);
    CHardwareListingCAN HwList(psInterfaces, nCount, pnSelList, CAN, CHANNEL_ALLOWED, &objMainWnd, InitData, pAdvancedSettings );
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
    //std::string omHardwareName;
    for(int nChannel = 0; nChannel < defNO_OF_CHANNELS; nChannel++)
    {
        //omHardwareName = sg_HardwareIntr[nChannel].m_acDescription.c_str();
        std::ostringstream ossHardwaredesc;
        ossHardwaredesc << "Vector - " << sg_HardwareList.m_ouChannelDetails[nChannel].m_omChannelName << " SN - " << sg_HardwareList.m_ouChannelDetails[nChannel].m_omVendorId;
        //ossHardwaredesc << "Channel Index - " <<(int)g_xlDrvConfig.channel[sg_HardwareList.m_ouChannelDetails[nChannel].m_omChannelIndex].hwChannel;
        std::string omHardwareChannel = ossHardwaredesc.str();
        if(omHardwareChannel/*sg_HardwareList.m_omHardwareChannel[nChannel] */== sg_SelectedChannels.m_omHardwareChannel[unIndex]
                && sg_HardwareList.m_ouChannelDetails[nChannel].m_omChannelName != "")
        {
            nSelChannel = nChannel;
            break;
        }
        else
        {
            std::ostringstream ossHardwaredesc1;
            ossHardwaredesc1 << "Vector - " << sg_HardwareList.m_ouChannelDetails[nChannel].m_omChannelName << ", Serial Number- " << sg_HardwareList.m_ouChannelDetails[nChannel].m_omVendorId;

            omHardwareChannel = ossHardwaredesc1.str();

            if(omHardwareChannel/*sg_HardwareList.m_omHardwareChannel[nChannel] */== sg_SelectedChannels.m_omHardwareChannel[unIndex]
                    && sg_HardwareList.m_ouChannelDetails[nChannel].m_omChannelName != "")
            {
                nSelChannel = nChannel;
                break;
            }
        }
    }

    return nSelChannel;
}
/**
* \brief         This function will get the hardware selection from the user
*                and will create essential networks.
* \param         unDefaultChannelCnt
* \return        returns defERR_OK (always)
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static int nCreateMultipleHardwareNetwork(PSCONTROLLER_DETAILS InitData, UINT unDefaultChannelCnt = 0)
{
    int nHardwareCountPrev = sg_ucNoOfHardware;
    int nHwCount = sg_ucNoOfHardware;
    int nChannels = 0;
    // Get Hardware Network Map
    for (unsigned int nCount = 0; nCount < g_xlDrvConfig.channelCount; nCount++)
    {
        // we take all hardware we found and
        // check that we have only CAN cabs/piggy's
        // at the moment there is no VN8910 XLAPI support!
        //if ( /*(g_xlDrvConfig.channel[nCount].channelBusCapabilities & XL_BUS_ACTIVE_CAP_CAN)
        //      && */(g_xlDrvConfig.channel[nCount].hwType != XL_HWTYPE_VN8900) )
        //{
        if ( /*(g_xlDrvConfig.channel[nCount].hwType == XL_HWTYPE_CANCASEXL) &&*/
            !(g_xlDrvConfig.channel[nCount].channelBusCapabilities & XL_BUS_ACTIVE_CAP_CAN) )
        {
            continue;
        }
        sg_HardwareIntr[nChannels].m_dwIdInterface = nCount;
        sg_HardwareIntr[nChannels].m_dwVendor = g_xlDrvConfig.channel[nCount].serialNumber;

        sg_HardwareIntr[nChannels].m_acDescription = g_xlDrvConfig.channel[nCount].name;
        nChannels++;
        //}
    }
    nHwCount = nChannels;   //Reassign hardware count according to final list of channels supported.

    CAN_setHardware(nChannels);
    /* If the default channel count parameter is set, prevent displaying the hardware selection dialog */
    if ( unDefaultChannelCnt && nChannels >= unDefaultChannelCnt )
    {
        for (UINT i = 0; i < unDefaultChannelCnt; i++)
        {
            //sg_anSelectedItems[i] = i;
            sg_anSelectedItems[i] = GetSelectedChannelIndex(i);
        }
        nHwCount = unDefaultChannelCnt;
    }
    else if ( ListHardwareInterfaces(sg_hOwnerWnd, sg_HardwareIntr, sg_anSelectedItems, nHwCount,InitData) != 0 )
    {
        sg_ucNoOfHardware = nHardwareCountPrev;
        return HW_INTERFACE_NO_SEL;
    }
    sg_ucNoOfHardware = (UCHAR)nHwCount;
    sg_nNoOfChannels = (UINT)nHwCount;
    g_xlChannelMask = 0;
    //Reorder hardware interface as per the user selection
    for (int nCount = 0; nCount < sg_ucNoOfHardware; nCount++)
    {
        if(sg_anSelectedItems[nCount] != -1)
        {
            sg_aodChannels[nCount].m_pXLChannelInfo  = &g_xlDrvConfig.channel[sg_HardwareIntr[sg_anSelectedItems[nCount]].m_dwIdInterface];
            g_xlChannelMask |= sg_aodChannels[nCount].m_pXLChannelInfo->channelMask;
        }
    }
    g_xlPermissionMask = g_xlChannelMask;

    return S_OK;
}

/**
* \brief         This function will create a single network with available single hardware.
* \param         void
* \return        returns defERR_OK (always)
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static int nCreateSingleHardwareNetwork()
{
    g_xlChannelMask = 0;

    // Set the number of channels as 1
    sg_ucNoOfHardware = (UCHAR)1;
    sg_nNoOfChannels = 1;

    for (UINT i=0; i < g_xlDrvConfig.channelCount; i++)
    {
        // we take all hardware we found and
        // check that we have only CAN cabs/piggy's
        // at the moment there is no VN8910 XLAPI support!
        if ( (g_xlDrvConfig.channel[i].channelBusCapabilities & XL_BUS_ACTIVE_CAP_CAN)
                /* && (g_xlDrvConfig.channel[i].hwType != XL_HWTYPE_VN8900)*/ )
        {
            sg_aodChannels[0].m_pXLChannelInfo  = &g_xlDrvConfig.channel[i];
            g_xlChannelMask |= sg_aodChannels[0].m_pXLChannelInfo->channelMask;
            break;
        }
    }

    g_xlPermissionMask = g_xlChannelMask;

    return S_OK;
}

/**
* \brief         This function will find number of hardwares connected.
*                It will create network as per hardware count.
*                This will popup hardware selection dialog in case there are more hardwares present.
* \param         unDefaultChannelCnt
* \return        Operation Result. 0 incase of no errors. Failure Error codes otherwise.
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static int nInitHwNetwork(PSCONTROLLER_DETAILS InitData, UINT unDefaultChannelCnt)
{
    int nChannelCount = 0;
    int nResult = NO_HW_INTERFACE;

    /* Select Hardware */
    nChannelCount = nGetNoOfConnectedHardware();
    if(nChannelCount == -1)
    {
        nResult = -1;
        return nResult;
    }

    // Assign the channel count
    sg_ucNoOfHardware = (UCHAR)nChannelCount;

    /* Capture only Driver Not Running event
     * Take action based on number of Hardware Available
     */
    char acNo_Of_Hw[256] = {0};
    sprintf(acNo_Of_Hw, _("Number of Vector hardwares Available: %d"), nChannelCount);

    /* No Hardware found */
    if( nChannelCount == 0 )
    {
        //MessageBox(nullptr,sg_omErrStr, nullptr, MB_OK | MB_ICONERROR);
        nChannelCount = -1;
    }
    /* Available hardware is lesser then the supported channels */
    else
    {
        // Check whether channel selection dialog is required
        //if( nChannelCount > 1)
        {
            // Get the selection from the user. This will also
            // create and assign the networks
            nResult = nCreateMultipleHardwareNetwork(InitData, unDefaultChannelCnt);
        }
   //     else
   //     {
   //         // Use available one hardware
			//nResult = nCreateSingleHardwareNetwork(InitData);
   //     }
    }
    return nResult;
}

/**
* \brief         This function will check if the client exists and gives back the client index.
* \param[in]     pcClientName, client name as TCHAR*
* \param[out]    Index, client index if found
* \return        TRUE if client name is found, else FALSE
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
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

/**
* \brief         This function will get available client slot
* \param         void
* \return        Returns the available client ID
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
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
            i = MAX_CLIENT_ALLOWED; // break the loop
        }
    }

    return nClientId;
}

/**
* \brief         This function will remove the existing client ID
* \param[in]     dwClientId, client ID to be removed
* \return        Returns TRUE if client ID removal is success, else FALSE
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static BOOL bRemoveClient(DWORD dwClientId)
{
    BOOL bResult = FALSE;
    if (sg_unClientCnt > 0)
    {
        UINT unClientIndex = (UINT)-1;
        if (bGetClientObj(dwClientId, unClientIndex))
        {
            /* clear the client first */
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

/**
* \brief         This function will check if the client ID exists
* \param[in]     dwClientId, client ID to be checked for existance
* \return        Returns TRUE if client ID existance is success, else FALSE
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
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
* \brief         This function will return the client index based on clientID
* \param[in]     dwClientId, client ID whose client index is needed
* \param[out]    unClientIndex, client index to be returned
* \return        Returns TRUE if client ID existance is success, else FALSE
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static BOOL bGetClientObj(DWORD dwClientID, UINT& unClientIndex)
{
    BOOL bResult = FALSE;
    for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        if (sg_asClientToBufMap[i].dwClientID == dwClientID)
        {
            unClientIndex = i;
            i = sg_unClientCnt; // break the loop
            bResult = TRUE;
        }
    }
    return bResult;
}

/**
* \brief         Function to retreive error occurred and log it
* \param[in]     File, pointer to log file
* \param[in]     Line, indicates line number in log file
* \return        void
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static void vRetrieveAndLog(DWORD /*dwErrorCode*/, char* /*File*/, int /*Line*/)
{
    USES_CONVERSION;

    char acErrText[MAX_PATH] = {'\0'};

    /* Get the error text for the corresponding error code */

    size_t nStrLen = strlen(acErrText);
    if (nStrLen > CAN_MAX_ERRSTR)
    {
        nStrLen = CAN_MAX_ERRSTR;
    }
    sg_acErrStr = acErrText;
}

/**
* \brief         Function to check if client buffer exists
* \param[in]     sClientObj, alias to SCLIENTBUFMAP object
* \param[in]     pBuf, pointer to CBaseCANBufFSE object
* \return        TRUE if buffer exists, else FALSE
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static BOOL bIsBufferExists(const SCLIENTBUFMAP& sClientObj, const CBaseCANBufFSE* pBuf)
{
    BOOL bExist = FALSE;
    for (UINT i = 0; i < sClientObj.unBufCount; i++)
    {
        if (pBuf == sClientObj.pClientBuf[i])
        {
            bExist = TRUE;
            i = sClientObj.unBufCount; // break the loop
        }
    }

    return bExist;
}

/**
* \brief         Function to remove exissting client buffer
* \param[in]     RootBufferArray, pointer to CBaseCANBufFSE class array
* \param[out]    unCount, indicates buffer count which will get reduced
* \param[in]     BufferToRemove, pointer to the buffer to be removed
* \return        TRUE if removed
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static BOOL bRemoveClientBuffer(CBaseCANBufFSE* RootBufferArray[MAX_BUFF_ALLOWED], UINT& unCount, CBaseCANBufFSE* BufferToRemove)
{
    BOOL bReturn = TRUE;
    for (UINT i = 0; i < unCount; i++)
    {
        if (RootBufferArray[i] == BufferToRemove)
        {
            if (i < (unCount - 1)) /* If not the last bufffer */
            {
                RootBufferArray[i] = RootBufferArray[unCount - 1];
            }
            unCount--;
        }
    }
    return bReturn;
}

HRESULT CDIL_CAN_VectorXL::CAN_SetHardwareChannel(PSCONTROLLER_DETAILS ouControllerDetails,DWORD /*dwDriverId*/,bool bIsHardwareListed, unsigned int unChannelCount)
{
    PSCONTROLLER_DETAILS psContrlDets = (PSCONTROLLER_DETAILS)ouControllerDetails;

    sg_SelectedChannels.m_nChannelCount = 0;
    g_xlChannelMask = 0;

    for ( UINT i = 0; i< CHANNEL_ALLOWED; i++ )
    {
        sg_anSelectedItems[i] = -1;
    }

    for (int nChannel = 0; nChannel < unChannelCount ; nChannel++)
    {
        if((psContrlDets[nChannel].m_omHardwareDesc != "") && (psContrlDets[nChannel].m_omHardwareDesc != "Simulation"))
        {
            sg_SelectedChannels.m_omHardwareChannel[nChannel] = psContrlDets[nChannel].m_omHardwareDesc;
            sg_ControllerDetails[nChannel].m_omHardwareDesc = psContrlDets[nChannel].m_omHardwareDesc;
            sg_SelectedChannels.m_nChannelCount += 1;
        }
    }

    sg_ucNoOfHardware = (UCHAR)sg_SelectedChannels.m_nChannelCount;
    sg_nNoOfChannels = (UINT)sg_SelectedChannels.m_nChannelCount;
    for (int nCount = 0; nCount < sg_SelectedChannels.m_nChannelCount; nCount++)
    {
        sg_anSelectedItems[nCount] = GetSelectedChannelIndex(nCount);
        if(sg_anSelectedItems[nCount] != -1)
        {
            sg_aodChannels[nCount].m_pXLChannelInfo  = &g_xlDrvConfig.channel[sg_HardwareIntr[sg_anSelectedItems[nCount]].m_dwIdInterface];
            g_xlChannelMask |= sg_aodChannels[nCount].m_pXLChannelInfo->channelMask;
        }
    }

    g_xlPermissionMask = g_xlChannelMask;

    if(true == bIsHardwareListed && g_xlChannelMask == 0)
    {
        nCreateSingleHardwareNetwork();

        return S_FALSE;
    }

    return S_OK;
}
