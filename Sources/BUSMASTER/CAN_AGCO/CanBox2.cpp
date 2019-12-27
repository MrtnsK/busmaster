// CanBox2.cpp: implementation of the CCanBox2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IOCanBox2.h"
#include "CanBox2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define CAN_MAX_ERRSTR 256
#define MAX_BUFF_ALLOWED 16
#define canOK 1
#define MAX_CLIENT_ALLOWED 16
#define	NUMBER_OF_CHANNEL 2

CString tmp;
long errSIECA;

enum
{
	CREATE_MAP_TIMESTAMP = 0x1,
	CALC_TIMESTAMP_READY = 0x2,
};

Liste ListeMessages;

unsigned long mask_court=0;
unsigned long mask_long=0;
int btr0,btr1;

int filt[16]={0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00	};

int Buf_BusOff_Counter,Buf_BusOff_Counter2;
int Buf_TxError,Buf_TxErrorCan2;
int Buf_Dominant_High_Level;
int Buf_Dominant_Low_Level;
int Buf_Recessif_High_Level;
int Buf_Recessif_Low_Level;
int Flag_Dominant_High_error;
int Flag_Dominant_Low_error;
int Flag_Recessif_High_error;
int Flag_Recessif_Low_error;

unsigned int InfoData[4];
unsigned char Data[16];

HINSTANCE hSIECADLL,hCAPIDLL,hSWITCH;
HINSTANCE hVXLAPIDLL;
HANDLE g_hCan;
HANDLE InputHandle=NULL;
HANDLE g_hDataEvent[defNO_OF_CHANNELS] = { 0 };

int ret_DesactiveCanBox;
int BusLoadSecure[2] = {1,1};

int	iInst = 0;

CStdioFile	fCanLog;
static SYSTEMTIME       sg_CurrSysTime = { '\0' };
static LARGE_INTEGER    sg_QueryTickCount;
static LARGE_INTEGER	sg_lnFrequency;
static UINT64           sg_TimeStampRef = 0x0;
static UINT64			sg_TimeStamp = 0;
static HWND				sg_hOwnerWnd = nullptr;
std::string				sg_acErrStr = "";
static CPARAM_THREADPROC sg_sParmRThread;
static UINT				sg_nNoOfChannels = 0;
static STCANDATA		sg_asCANMsg;
int						sg_arrReadHandles[CHANNEL_ALLOWED];
static INT				sg_anSelectedItems[CHANNEL_ALLOWED];
static INTERFACE_HW		sg_HardwareIntr[defNO_OF_CHANNELS];

//CAN_SetConfigData variables
static UCHAR sg_ucNoOfHardware = 0;
static SCONTROLLER_DETAILS sg_ControllerDetails[defNO_OF_CHANNELS];
enum
{
	STATE_DRIVER_SELECTED = 0x0,
	STATE_HW_INTERFACE_LISTED,
	STATE_HW_INTERFACE_SELECTED,
	STATE_CONNECTED
};
BYTE sg_bCurrState;

//MANAGE MSG BUFFER variables
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
		memset(pacClientName, 0, sizeof(char) * MAX_PATH);
		for (int i = 0; i < MAX_BUFF_ALLOWED; i++)
		{
			pClientBuf[i] = nullptr;
		}

	}
} SCLIENTBUFMAP;
static UINT sg_unClientCnt = 0;
static BOOL sg_bIsConnected = FALSE;
static SCLIENTBUFMAP sg_asClientToBufMap[MAX_CLIENT_ALLOWED];
static BOOL bRemoveClientBuffer(CBaseCANBufFSE* RootBufferArray[MAX_BUFF_ALLOWED], UINT& unCount, CBaseCANBufFSE* BufferToRemove);
static BOOL bIsBufferExists(const SCLIENTBUFMAP& sClientObj, const CBaseCANBufFSE* pBuf);
static BOOL bGetClientObj(DWORD dwClientID, UINT& unClientIndex);
static DWORD dwGetAvailableClientSlot();
static BOOL bClientExist(std::string pcClientName, INT& Index);
static BOOL bRemoveClient(DWORD dwClientId);

//////////Vector ////////////

XLstatus errVXLAPI;


//*******************************************************************
//*******************************************************************
//*						SELECTION HARDWARE							*
//*																	*
//*		--> 0: CanBox												*
//*		--> 1: CanCase												*
//*		--> Si les 2 sont branch�es la CanCase est prise par d�faut	*
//*
int HARDWARE = 0;												
//*																	*
//*******************************************************************
//*******************************************************************

////////////////////////////


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCanBox2::CCanBox2()
{
	Buf_BusOff_Counter=0;
	Buf_BusOff_Counter2 = 0;
	Buf_TxError = 0;
	Buf_TxErrorCan2 = 0;
	Buf_Dominant_High_Level=0;
	Buf_Dominant_Low_Level=0;
	Buf_Recessif_High_Level=0;
	Buf_Recessif_Low_Level=0;
	Flag_Dominant_High_error=0;
	Flag_Dominant_Low_error=0;
	Flag_Recessif_High_error=0;
	Flag_Recessif_Low_error=0;
	
	hThread = NULL;
	bStopThread = false;

	bLogFile = 0;
	ulNum = 0;

	/*
	* SFE: 02.10.2009
	* init handle and bool parameter for later comparision
	*/
	FlagFinThread = true;
	d_eventStop = NULL;

//	fCanLog.Open("MF_IOCanBox_Log.txt",CFile::modeCreate|CFile::modeWrite);


	//////////Vector ////////////
	strcpy(g_AppName,"xlCANdemo");            
	g_xlPortHandle              = XL_INVALID_PORTHANDLE;  
	g_xlChannelMask             = 0;                      
	g_xlPermissionMask          = 0;                      	                      
	CounterRx					= 0;
	CounterEx					= 0;
	ErCounter					= 0;

		
	//lancerConsole();	//Pour le debuggage
	/////////////////////////////

}

CCanBox2::~CCanBox2()
{
	/*
	* SFE: 02.10.2009
	* set stop event to force function ThreadDepilement to stop and wait until the thread has stopped
	*/
	while(FlagFinThread != true)
	{
		SetEvent(d_eventStop);
		SetEvent(d_eventStop2);
	};

	/*
	* SFE: 02.10.2009
	* check if handle exists and close handle in destructor,
	*/
	if(d_eventStop!=NULL)
	{
		CloseHandle(d_eventStop);
		CloseHandle(d_eventStop2);
		d_eventStop = NULL;
		d_eventStop2 = NULL;
	}

	hThread = NULL;
	hThread2 = NULL;

	/*if(HARDWARE == 0)*/	FreeLibrary(hSIECADLL);
	/*if(HARDWARE == 1)*/	FreeLibrary(hVXLAPIDLL);

//	fCanLog.Close();
}

CCanBox2* g_pouDIL_CCanBox2 = nullptr;

HRESULT GetIDIL_CAN_Controller(void** ppvInterface)
{
	HRESULT hResult = S_OK;
	if (nullptr == g_pouDIL_CCanBox2)
	{
		if ((g_pouDIL_CCanBox2 = new CCanBox2) == nullptr)
		{
			hResult = S_FALSE;
		}
	}
	*ppvInterface = (void*)g_pouDIL_CCanBox2;

	return hResult;
}

HRESULT CCanBox2::CAN_PerformInitOperations(void)
{
	Initialisation(2, 0, CAN_250, CAN_SAMPLE_3, 0);
	hCan2 = hCan;
	Initialisation(1, 0, CAN_250, CAN_SAMPLE_3, 0);
	return S_OK;
}


HRESULT CCanBox2::CAN_PerformClosureOperations(void)
{
	if (hCan)
		dll_canClose(hCan);
	if (hCan2)
		dll_canClose(hCan2);
	return S_OK;
}

HRESULT CCanBox2::CAN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER& QueryTickCount)
{
	GetLocalTime(&sg_CurrSysTime);
	CurrSysTime = sg_CurrSysTime;
	TimeStamp = sg_TimeStampRef;
	sg_QueryTickCount.QuadPart = 0;
	QueryTickCount = sg_QueryTickCount;
	return S_OK;
}

HRESULT CCanBox2::CAN_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount, PSCONTROLLER_DETAILS InitData)
{
	USES_CONVERSION;
	HRESULT hResult = S_OK;
	unsigned long	pulSNHigh = 0;
	unsigned long	pulSNLow = 0;
	int			tmpid;

	GetHWinfo(&pulSNHigh, &pulSNLow, &tmpid);
	g_serialnumber = decriptSN(pulSNHigh, pulSNLow);
	//for (UINT i = 0; i < 2; i++)
	//{	
	//	sSelHwInterface[i].m_dwIdInterface = (ULONG)i;
	//	sSelHwInterface[i].m_dwVendor = (ULONG)11928;
	//	sSelHwInterface[i].m_bytNetworkID = i + 1;
	//	std::ostringstream name;
	//	name << "AGCO " << g_serialnumber << " Can" << i + 1;
	//	sSelHwInterface[i].m_acDeviceName = name.str();
	//}
	nCount = 2;
	sg_bCurrState = STATE_HW_INTERFACE_LISTED;
	return hResult;
}

HRESULT CCanBox2::CAN_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount)
{
	USES_CONVERSION;

	VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_LISTED, ERR_IMPROPER_STATE);

	sg_bCurrState = STATE_HW_INTERFACE_SELECTED;
	return S_OK;
}

HRESULT CCanBox2::CAN_DeselectHwInterface(void)
{
	VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

	sg_bCurrState = STATE_HW_INTERFACE_LISTED;
	return S_OK;
}

HRESULT CCanBox2::CAN_SetConfigData(PSCONTROLLER_DETAILS InitData, int Length)
{
	VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);
	USES_CONVERSION;


	//* Fill the hardware description details */
	//for (UINT nCount = 0; nCount < 1; nCount++)
	//{ // crash ncount == 1
	//	((PSCONTROLLER_DETAILS)InitData)[nCount].m_omHardwareDesc = sg_ControllerDetails[nCount].m_omHardwareDesc;
	//}

	//for (int i = 0; i < Length; i++)
	//{
	//	sg_ControllerDetails[i] = InitData[i];
	//}

	return S_OK;
}

static void vWriteIntoClientsBuffer(STCANDATA& sCanData, UINT unClientIndex)
{
	/* Write into the respective client's buffer */
	for (UINT j = 0; j < sg_asClientToBufMap[unClientIndex].unBufCount; j++)
	{
		sg_asClientToBufMap[unClientIndex].pClientBuf[j]->WriteIntoBuffer(&sCanData);
	}
}

void CCanBox2::ProcessCANMsg(CMSG canmsg, unsigned int nChannelIndex)
{
	if (oldtstamp == 0)
		oldtstamp = canmsg.ul_tstamp;
	sg_asCANMsg.m_lTickCount.QuadPart = canmsg.ul_tstamp - oldtstamp;
	sg_asCANMsg.m_ucDataType = RX_FLAG;
	sg_asCANMsg.m_uDataInfo.m_sCANMsg.m_ucDataLen = canmsg.by_len;
	sg_asCANMsg.m_uDataInfo.m_sCANMsg.m_unMsgID = canmsg.l_id;
	sg_asCANMsg.m_uDataInfo.m_sCANMsg.m_ucEXTENDED = canmsg.by_extended;
	sg_asCANMsg.m_uDataInfo.m_sCANMsg.m_ucRTR = canmsg.by_remote;
	sg_asCANMsg.m_uDataInfo.m_sCANMsg.m_ucChannel = nChannelIndex + 1;
	sg_asCANMsg.m_uDataInfo.m_sCANMsg.m_ucData[0] = canmsg.aby_data[0];
	sg_asCANMsg.m_uDataInfo.m_sCANMsg.m_ucData[1] = canmsg.aby_data[1];
	sg_asCANMsg.m_uDataInfo.m_sCANMsg.m_ucData[2] = canmsg.aby_data[2];
	sg_asCANMsg.m_uDataInfo.m_sCANMsg.m_ucData[3] = canmsg.aby_data[3];
	sg_asCANMsg.m_uDataInfo.m_sCANMsg.m_ucData[4] = canmsg.aby_data[4];
	sg_asCANMsg.m_uDataInfo.m_sCANMsg.m_ucData[5] = canmsg.aby_data[5];
	sg_asCANMsg.m_uDataInfo.m_sCANMsg.m_ucData[6] = canmsg.aby_data[6];
	sg_asCANMsg.m_uDataInfo.m_sCANMsg.m_ucData[7] = canmsg.aby_data[7];
	vWriteIntoClientsBuffer(sg_asCANMsg, 0);
}

void	CCanBox2::can_reader(void)
{
	long			len = 1;
	CMSG			canmsg;

	while (!bStopThread)
	{
		if (dll_canRead(hCan, &canmsg, &len) == NTCAN_SUCCESS)
			ProcessCANMsg(canmsg, 0);
	}
}

void	CCanBox2::can_reader2(void)
{
	long			len = 1;
	CMSG			canmsg;

	while (!bStopThread)
	{
		if (dll_canRead(hCan2, &canmsg, &len) == NTCAN_SUCCESS)
			ProcessCANMsg(canmsg, 1);
	}
}

DWORD WINAPI can_read(LPVOID lpParam)
{
	CCanBox2 *CanUsb = (CCanBox2*)lpParam;

	while (WaitForSingleObject(CanUsb->d_eventStop, 0) == WAIT_TIMEOUT)
	{
		CanUsb->can_reader();
	}
	CanUsb->FlagFinThread = true;
	return S_OK;
}

DWORD WINAPI can_read2(LPVOID lpParam)
{
	CCanBox2 *CanUsb = (CCanBox2*)lpParam;

	while (WaitForSingleObject(CanUsb->d_eventStop2, 0) == WAIT_TIMEOUT)
	{
		CanUsb->can_reader2();
	}
	CanUsb->FlagFinThread = true;
	return S_OK;
}

HRESULT CCanBox2::CAN_StartHardware(void)
{
	VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);
	sg_bCurrState = STATE_CONNECTED;
	DWORD dwThreadId = 1;
	DWORD dwThreadId2 = 2;

	FlagFinThread = false;
	bStopThread = false;
	d_eventStop = CreateEvent(NULL, FALSE, FALSE, "event_StopThread");
	d_eventStop2 = CreateEvent(NULL, FALSE, FALSE, "event_StopThread2");
	hThread = CreateThread(NULL, 0, can_read, this, 0, &dwThreadId);
	SetThreadPriority(hThread, THREAD_PRIORITY_NORMAL);
	hThread2 = CreateThread(NULL, 0, can_read2, this, 0, &dwThreadId2);
	SetThreadPriority(hThread2, THREAD_PRIORITY_NORMAL);

	return S_OK;
}

HRESULT CCanBox2::CAN_StopHardware(void)
{  
	VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);
	sg_bCurrState = STATE_HW_INTERFACE_SELECTED;
	ClearBuffer();
	if (d_eventStop)
		SetEvent(d_eventStop);
	if (d_eventStop2)
		SetEvent(d_eventStop2);
	if (hThread && hThread2)
		if (dll_canIsHandleValid(hCan) == NTCAN_SUCCESS && dll_canIsHandleValid(hCan2) == NTCAN_SUCCESS)
		{
			bStopThread = true;
			dll_canBreakCanRead(hCan);
			dll_canBreakCanRead(hCan2);
		}
	if (dll_canIsHandleValid(hCan) == NTCAN_SUCCESS)
	{
		errSIECA = dll_canIdDeleteArray(hCan);
		if (errSIECA != NTCAN_SUCCESS)
			return S_FALSE;
	}
	if (dll_canIsHandleValid(hCan2) == NTCAN_SUCCESS)
	{
		errSIECA = dll_canIdDeleteArray(hCan2);
		if (errSIECA != NTCAN_SUCCESS)
			return S_FALSE;
	}
	return S_OK;
}

HRESULT CCanBox2::CAN_GetCurrStatus(STATUSMSG& StatusData)
{
	StatusData.wControllerStatus = NORMAL_ACTIVE;
	return S_OK;
}

HRESULT CCanBox2::CAN_GetTxMsgBuffer(BYTE*& pouFlxTxMsgBuffer)
{
	return S_OK;
}


void	CCanBox2::ProcessCANtx(STCAN_MSG canmsg)
{
	STCANDATA	CANMsg;

	CANMsg.m_uDataInfo.m_sCANMsg = canmsg;
	CANMsg.m_ucDataType = TX_FLAG;
	CANMsg.m_lTickCount.QuadPart = sg_asCANMsg.m_lTickCount.QuadPart;
	vWriteIntoClientsBuffer(CANMsg, 0);
}

HRESULT CCanBox2::CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& cmsg)
{
	VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);
	CMSG	canmsg;
	long	len = 1;
	
	canmsg.ul_tstamp = sg_asCANMsg.m_lTickCount.QuadPart;
	canmsg.by_len = cmsg.m_ucDataLen;
	canmsg.l_id = cmsg.m_unMsgID;
	canmsg.by_extended = cmsg.m_ucEXTENDED;
	canmsg.by_remote = cmsg.m_ucRTR;
	canmsg.aby_data[0] = cmsg.m_ucData[0];
	canmsg.aby_data[1] = cmsg.m_ucData[1];
	canmsg.aby_data[2] = cmsg.m_ucData[2];
	canmsg.aby_data[3] = cmsg.m_ucData[3];
	canmsg.aby_data[4] = cmsg.m_ucData[4];
	canmsg.aby_data[5] = cmsg.m_ucData[5];
	canmsg.aby_data[6] = cmsg.m_ucData[6];
	canmsg.aby_data[7] = cmsg.m_ucData[7];

	if (cmsg.m_ucChannel && cmsg.m_ucChannel <= 2)
	{
		if (cmsg.m_ucChannel == 1)
			errSIECA = dll_canSend(hCan, &canmsg, &len);
		else if (cmsg.m_ucChannel == 2)
			errSIECA = dll_canSend(hCan2, &canmsg, &len);
		if (errSIECA == 0)
			ProcessCANtx(cmsg);
	}
	return S_OK;
}

HRESULT CCanBox2::CAN_GetBusConfigInfo(BYTE* /*BusInfo*/)
{
	return S_OK;
}

HRESULT CCanBox2::CAN_GetLastErrorString(std::string& acErrorStr)
{ //not sure at all about this one
//	USES_CONVERSION;
//
//	char acErrText[MAX_PATH] = { '\0' };
//
//	/* Get the error text for the corresponding error code */
//
//	size_t nStrLen = strlen(acErrText);
//	if (nStrLen > CAN_MAX_ERRSTR)
//	{
//		nStrLen = CAN_MAX_ERRSTR;
//	}
//	acErrorStr = acErrText;
	return S_OK;
}

HRESULT CCanBox2::CAN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam)
{
	HRESULT hResult = S_OK;
	if ((sg_bCurrState == STATE_HW_INTERFACE_SELECTED) || (sg_bCurrState == STATE_CONNECTED))
	{
		switch (eContrParam)
		{
			case NUMBER_HW:
			{
				lParam = 2;
			}
			break;
			case NUMBER_CONNECTED_HW:
			{
				lParam = 2;
			}
			break;
			case DRIVER_STATUS:
			{
				lParam = true;
			}
			break;
			case HW_MODE:
			{
				lParam = true;
			}
			break;
			case CON_TEST:
			{
				lParam = true;
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

HRESULT CCanBox2::CAN_SetControllerParams(int nValue, ECONTR_PARAM eContrparam)
{
	//todo???
	return S_OK;
}

HRESULT	CCanBox2::CAN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam)
{
	HRESULT hResult = S_OK;
	unsigned char old = 0;
	CTRDATA stathw;

	if ((sg_bCurrState == STATE_CONNECTED) || (sg_bCurrState == STATE_HW_INTERFACE_SELECTED))
	{
		dll_canGetCounter(&stathw);
		if (nChannel <= 2)
		{
			if (eContrParam == ERR_CNT)
			{
				//sErrorCnt.m_ucTxErrCount = sg_aodChannels[nChannel].m_ucTxErrorCounter;
				sErrorCnt.m_ucRxErrCount = stathw.ul_errframectr1 + stathw.ul_errframectr2;
			}
			else if (eContrParam == PEAK_ERR_CNT)
			{
				//sErrorCnt.m_ucTxErrCount = sg_aodChannels[nChannel].m_ucPeakTxErrorCounter;
				if (stathw.ul_errframectr1 + stathw.ul_errframectr2 > old)
				{
					sErrorCnt.m_ucRxErrCount = stathw.ul_errframectr1 + stathw.ul_errframectr2;
					old = sErrorCnt.m_ucRxErrCount;
				}
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

HRESULT CCanBox2::CAN_SetAppParams(HWND hWndOwner)
{
	sg_hOwnerWnd = hWndOwner;

	// Initialise both the time parameters
	GetLocalTime(&sg_CurrSysTime);
	sg_TimeStamp = 0x0;
	sg_QueryTickCount.QuadPart = 0;	//Query Tick Count
	sg_acErrStr = "";
	CAN_ManageMsgBuf(MSGBUF_CLEAR, 0, nullptr);
	return(S_OK);
}

HRESULT CCanBox2::CAN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj)
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

HRESULT CCanBox2::CAN_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName)
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
			hResult = ERR_NO_MORE_CLIENT_ALLOWED;
	}
	else
	{
		if (bRemoveClient(ClientID))
			hResult = S_OK;
		else
			hResult = ERR_NO_CLIENT_EXIST;
	}

	return hResult;
}

HRESULT CCanBox2::CAN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus)
{
	return S_OK;
}

HRESULT CCanBox2::CAN_LoadDriverLibrary(void)
{
	return S_OK;
}

HRESULT CCanBox2::CAN_UnloadDriverLibrary(void)
{
	return S_OK;
}

HRESULT CCanBox2::CAN_SetHardwareChannel(PSCONTROLLER_DETAILS, DWORD dwDriverId, bool bIsHardwareListed, unsigned int unChannelCount)
{
	return S_OK;
}

void CCanBox2::SendMessage(CString msg)
{
	AfxMessageBox(msg);
}

HRESULT	CCanBox2::GetHWinfo(unsigned long *pulSNHigh, unsigned long *pulSNLow, int *res)
{
	(*res) = this->dll_canGetHWSerialNumber(hCan, pulSNHigh, pulSNLow);
	return S_OK; 
}

T_DeviceList	*CCanBox2::GetHardwareList()
{
	T_DeviceList	*hwlist;
	long			nb;

	if (this->dll_canGetNumberOfConnectedDevices(&nb) != NTCAN_SUCCESS)
		return NULL;
	this->hardware = nb;
	if (!(hwlist = (T_DeviceList*)malloc(sizeof(T_DeviceList) * nb)))
		return NULL;
	if (this->dll_canGetDeviceList(hwlist) != NTCAN_SUCCESS)
		return NULL;
	return hwlist;
}

int CCanBox2::Initialisation()
{
	return Initialisation(1,CAN_250,CAN_SAMPLE_3, 0);
}

int CCanBox2::Initialisation(int NumVoie,unsigned long vitesse,unsigned char echantillonnage)
{
	return Initialisation(NumVoie,0,vitesse,echantillonnage, 0);
}

int CCanBox2::Initialisation(int NumVoie,int Echo,unsigned long vitesse,unsigned char echantillonnage)
{
	return Initialisation(NumVoie,0,vitesse,echantillonnage, 0);
}

//Use var HardwareChoosed to 0 or 1 to test only one type of cards 0=test canbox, 1=test canbase
int CCanBox2::Initialisation(int NumVoie,int Echo,unsigned long vitesse,unsigned char echantillonnage, int HardwareChoosed)
{


		int VoixCanCase;
		CString msg;

		char buf[256];
		DWORD n;
		n = GetSystemDirectory(buf, 256); 
		strcpy(buf + n, "\\VXLAPI.DLL");
		
		if(HardwareChoosed == 0) //Si demander d'utiliser la canbox, on ne fait pas la partie canbase
			goto HardCanBox;
		
		if ((int) (hVXLAPIDLL = LoadLibrary ("VXLAPI.DLL")) <=  HINSTANCE_ERROR) 
		{    
		   if ((int) (hVXLAPIDLL = LoadLibrary (buf)) <=  HINSTANCE_ERROR) 
		   {
				if(HardwareChoosed == -1)
					goto HardCanBox;
				
				AfxMessageBox("Can't open VXLAPI.dll",MB_ICONERROR);

				return NOT_OK;
		   }
		}


		VoixCanCase = NumVoie-1;
		NumChannel = NumVoie;

		XLaccess      xlChanMaskTx = 0;

		int           stop = 0;
		int           activated = 0;
		unsigned char xlChanIndex = 0;
		unsigned int  txID = 0x01;
		int           outputMode = XL_OUTPUT_MODE_NORMAL;



		/*
		printf("----------------------------------------------------------\n");
		printf("- xlCANdemo - Test Application for XL Family Driver API  -\n");
		printf("-             Vector Informatik GmbH,  " __DATE__"       -\n");
		#ifdef WIN64
		printf("-             - 64bit Version -                          -\n");
		#endif
		printf("----------------------------------------------------------\n");
		*/



		XLstatus          Init_xlStatus;
		XLaccess          Init_xlChannelMaskTx = 0;

		Init_xlStatus = xlOpenDriver ();

		if(XL_SUCCESS != Init_xlStatus)
		{
			if(HardwareChoosed == -1)
				goto HardCanBox;

			msg.Format("Erreur xlOpenDriver (%d)",Init_xlStatus);
			AfxMessageBox(msg);

			return NOT_OK;
		}

		Init_xlStatus = xlGetDriverConfig(&g_xlDrvConfig);
		
		
		if(XL_SUCCESS != Init_xlStatus)
		{
			if(HardwareChoosed == -1)
				goto HardCanBox;

			msg.Format("Erreur xlGetDriverConfig (%d)",Init_xlStatus);
			AfxMessageBox(msg);

			return NOT_OK;
		}
		
		if(g_xlDrvConfig.channelCount<=2)
		{
			if(HardwareChoosed == -1)
				goto HardCanBox; //Do not return error message, try with another CAN card
			
			msg.Format("No Vector card detected",0);
			AfxMessageBox(msg);

			return NOT_OK;	//Pas de d�tection de la CanCase
		}
		//	demoPrintConfig();
	
   
		//	printf("Usage: xlCANdemo <BaudRate> <ApplicationName> <Identifier>\n\n");
    

		g_xlChannelMask = 0;

		// ------------------------------------
		// select the wanted channels
		// ------------------------------------
		if (g_xlDrvConfig.channel[VoixCanCase].channelBusCapabilities & XL_BUS_ACTIVE_CAP_CAN) { 
        
			if (!xlChanMaskTx) {
				xlChanMaskTx = g_xlDrvConfig.channel[VoixCanCase].channelMask;
				xlChanIndex  = g_xlDrvConfig.channel[VoixCanCase].channelIndex;
			}

			g_xlChannelMask |= g_xlDrvConfig.channel[VoixCanCase].channelMask;
		}
		

		if (!g_xlChannelMask) {
			printf("ERROR: no available channels found! (e.g. no CANcabs...)\n\n");
			Init_xlStatus = XL_ERROR;
		}


		g_xlPermissionMask = g_xlChannelMask;


		if(XL_SUCCESS != Init_xlStatus)
		{
			msg.Format("Erreur pas de canal disponible trouv� (%d)",Init_xlStatus);
			AfxMessageBox(msg);
			return NOT_OK;
		}

		// ------------------------------------
		// open ONE port including all channels
		// ------------------------------------
		Init_xlStatus = xlOpenPort(&g_xlPortHandle, g_AppName, g_xlChannelMask, &g_xlPermissionMask, RX_QUEUE_SIZE, XL_INTERFACE_VERSION, XL_BUS_TYPE_CAN);
	//	printf("- OpenPort         : CM=0x%I64x, PH=0x%02X, PM=0x%I64x, %s\n", 
	//		g_xlChannelMask, g_xlPortHandle, g_xlPermissionMask, xlGetErrorString(Init_xlStatus));
		

		
		if(XL_SUCCESS != Init_xlStatus)
		{
			msg.Format("Erreur xlOpenPort (%d)",Init_xlStatus);
			AfxMessageBox(msg);
			return NOT_OK;
		}
			

		if ( (XL_SUCCESS == Init_xlStatus) && (XL_INVALID_PORTHANDLE != g_xlPortHandle) ) {
    
		// ------------------------------------
		// if we have permission we set the
		// bus parameters (baudrate)
		// ------------------------------------
			if (g_xlChannelMask == g_xlPermissionMask) {
      
			  Init_xlStatus = xlCanSetChannelBitrate(g_xlPortHandle, g_xlChannelMask, vitesse);
			  
			  	if(XL_SUCCESS != Init_xlStatus)
				{
					msg.Format("Erreur xlCanSetChannelBitrate (%d)",Init_xlStatus);
					AfxMessageBox(msg);
					return NOT_OK;
				}
	//			printf("- SetChannelBitrate: baudr.=%u, %s\n",vitesse, xlGetErrorString(Init_xlStatus));
			} 
			else {
	//			printf("-                  : we have NO init access!\n");
	//			AfxMessageBox("we have NO init access");
			}
   
		}
		else {
  
			xlClosePort(g_xlPortHandle);
			g_xlPortHandle = XL_INVALID_PORTHANDLE;
			Init_xlStatus = XL_ERROR;
			return NOT_OK;
		}
  


		Init_xlStatus = xlCanResetAcceptance(g_xlPortHandle, g_xlChannelMask, XL_CAN_EXT);
		if(XL_SUCCESS != Init_xlStatus)
		{
			msg.Format("Erreur xlCanResetAcceptance (%d)",Init_xlStatus);
			AfxMessageBox(msg);
			return NOT_OK;
		}

	//	printf("- Init             : %s\n",  xlGetErrorString(Init_xlStatus));

		
		if(XL_SUCCESS == Init_xlStatus) {

			if (g_xlPortHandle!= XL_INVALID_PORTHANDLE) {

				Init_xlStatus = xlSetNotification (g_xlPortHandle, &g_hMsgEvent, 1);
									
				Init_xlStatus = ActiveCanBox();
			}

	//		printf("- Create RX thread : %s\n",  xlGetErrorString(Init_xlStatus));
		}



		if(XL_SUCCESS != Init_xlStatus)
		{
			msg.Format("Erreur ActiveCanBox()");
			AfxMessageBox(msg);
			return NOT_OK;
		}
			

		// ------------------------------------
		// go with all selected channels on bus
		// ------------------------------------
		Init_xlStatus = xlActivateChannel(g_xlPortHandle, g_xlChannelMask, XL_BUS_TYPE_CAN, XL_ACTIVATE_RESET_CLOCK);

		if(XL_SUCCESS != Init_xlStatus)
		{
			msg.Format("Erreur xlActivateChannel (%d)",Init_xlStatus);
			AfxMessageBox(msg);
			return NOT_OK;
		}
		
	//	printf("- ActivateChannel  : CM=0x%I64x, %s\n", g_xlChannelMask, xlGetErrorString(Init_xlStatus));
		

		ListeMessages.Can[NumChannel-1].bModeCanalyzer = false;

		return OK;


HardCanBox:

		unsigned char	bySpeed;
		
		HARDWARE = 0;

		NumChannel = NumVoie;


		n = GetSystemDirectory(buf, 256); 
		strcpy(buf + n, "\\SIECA132.DLL");


		
		
		if ((int) (hSIECADLL = LoadLibrary ("SIECA132.DLL")) <=  HINSTANCE_ERROR) 
		{    
		   if ((int) (hSIECADLL = LoadLibrary (buf)) <=  HINSTANCE_ERROR) 
		   {
				#if NIV_AFFICH_ERROR > 0
					AfxMessageBox("Can't open SIECA132.dll",MB_ICONERROR);
				#endif
				return NOT_OK;
		   }
		}

		dll_canOpen =						(long (__stdcall *)(long,long,long,long,long,const char *,const char *,const char *,void ** ))GetProcAddress (hSIECADLL,"canOpen");




		/*
		* SFE: 02.10.2009
		* use canSetBaudrateForce to set the baudrate even if are not the owner of the handle;
		* that prevents of caninit failures
		*/
		dll_canSetBaudrate =				(long (__stdcall *)(void *,long))GetProcAddress (hSIECADLL,"canSetBaudrateForce");
		dll_canSend =						(long (__stdcall *)(void *,struct st_canmsg *,long *))GetProcAddress (hSIECADLL,"canSend");
		dll_canWrite =						(long (__stdcall *)(void *,struct st_canmsg *,long *))GetProcAddress (hSIECADLL,"canWrite");
		dll_canRead =						(long (__stdcall *)(void *,struct st_canmsg *,long * ))GetProcAddress (hSIECADLL,"canRead");
		dll_canIdAdd =						(long (__stdcall *)(void *,long))GetProcAddress (hSIECADLL,"canIdAdd");
		dll_canIdDelete =					(long (__stdcall *)(void *,long))GetProcAddress (hSIECADLL,"canIdDelete");
		dll_canIdAddArray =					(long (__stdcall *)(void *,unsigned char *))GetProcAddress (hSIECADLL,"canIdAddArray");
		dll_canIdDeleteArray =				(long (__stdcall *)(void *))GetProcAddress (hSIECADLL,"canIdDeleteArray");
		dll_canEnableAllIds =				(long (__stdcall *)(void *,bool))GetProcAddress (hSIECADLL,"canEnableAllIds");
		dll_canAreAllIdsEnabled =			(long (__stdcall *)(void *,bool *))GetProcAddress (hSIECADLL,"canAreAllIdsEnabled");
		dll_canIDStatus =					(long (__stdcall *)(void *,struct st_id_array *))GetProcAddress (hSIECADLL,"canIDStatus");
		dll_canBreakCanRead =				(long (__stdcall *)(void *))GetProcAddress (hSIECADLL,"canBreakcanRead");
		dll_canSetIdOffset =				(long (__stdcall *)(void *,unsigned long))GetProcAddress (hSIECADLL,"canSetIdOffset");
		dll_canGetIdOffset =				(long (__stdcall *)(void *,unsigned long *))GetProcAddress (hSIECADLL,"canGetIdOffset");
		dll_canStatus =						(long (__stdcall *)(void *,struct st_can_status *))GetProcAddress (hSIECADLL,"canStatus");
		dll_canGetCounter =					(long (__stdcall *)(struct st_counterdata *))GetProcAddress (hSIECADLL,"canGetCounter");
		dll_canGetCounterExtended =			(long (__stdcall *)(void *,struct st_counterdata2 * ))GetProcAddress (hSIECADLL,"canGetCounterExtended");
		dll_canGetBusloadExtended =			(long (__stdcall *)(void *,struct st_busload *))GetProcAddress (hSIECADLL,"canGetBusloadExtended");
		dll_canClose =						(long (__stdcall *)(void *))GetProcAddress (hSIECADLL,"canClose");
		dll_canGetTimeout =					(long (__stdcall *)(void *,enum e_TimeoutType,long *))GetProcAddress (hSIECADLL,"canGetTimeout");
		dll_canSetTimeout =					(long (__stdcall *)(void *,enum e_TimeoutType,long))GetProcAddress (hSIECADLL,"canSetTimeout");
		dll_canSetTxDelay =					(long (__stdcall *)(void *,long))GetProcAddress (hSIECADLL,"canSetTxDelay");
		dll_canIsVirtualHw =				(unsigned char (__stdcall *)(void))GetProcAddress (hSIECADLL,"canIsVirtualHw");
		dll_canGetDllInfo =					(long (__stdcall *)(void *,void *))GetProcAddress (hSIECADLL,"canGetDllInfo");
		dll_canIsNetOwner =					(long (__stdcall *)(void *))GetProcAddress (hSIECADLL,"canIsNetOwner");
		dll_canInstruction =				(long (__stdcall *)(void *,unsigned int *))GetProcAddress (hSIECADLL,"canInstruction");
		dll_canGetSyncTimer =				(long (__stdcall *)(void *,unsigned long *,__int64 *))GetProcAddress (hSIECADLL,"canGetSyncTimer");
		dll_canSetOwner =					(long (__stdcall *)(long,void *))GetProcAddress (hSIECADLL,"canSetOwner");
		dll_canGetOwner =					(long (__stdcall *)(long,void ** ))GetProcAddress (hSIECADLL,"canGetOwner");
		dll_canSetFilterMode =				(long (__stdcall *)(void *,enum e_FilterMode))GetProcAddress (hSIECADLL,"canSetFilterMode");
		dll_canGetFilterMode =				(long (__stdcall *)(void *,enum e_FilterMode *))GetProcAddress (hSIECADLL,"canGetFilterMode");
		dll_canIsHandleValid =				(long (__stdcall *)(void *))GetProcAddress (hSIECADLL,"canIsHandleValid");
		dll_canSetFilterJ2534 =				(long (__stdcall *)(void *,enum e_J2534Mode,unsigned long,unsigned long))GetProcAddress (hSIECADLL,"canSetFilterJ2534");
		dll_canDeleteFilterJ2534 =			(long (__stdcall *)(void *))GetProcAddress (hSIECADLL,"canDeleteFilterJ2534");
		dll_canSetFilterJ2534_2 =			(long (__stdcall *)(void *,enum e_J2534Mode,unsigned long,unsigned __int64,unsigned long,unsigned __int64))GetProcAddress (hSIECADLL,"canSetFilterJ2534_2");
		dll_canDeleteFilterJ2534_2 =		(long (__stdcall *)(void *))GetProcAddress (hSIECADLL,"canDeletefilterJ2534_2");
		dll_canClearBuffer =				(long (__stdcall *)(void *))GetProcAddress (hSIECADLL,"canClearBuffer");
		dll_canResetCounter =				(long (__stdcall *)(void *))GetProcAddress (hSIECADLL,"canResetCounter");
		dll_canGetTime =					(long (__stdcall *)(unsigned int []))GetProcAddress (hSIECADLL,"canGetTime");
		dll_canGetNumberOfConnectedDevices =(long (__stdcall *)(long *))GetProcAddress (hSIECADLL,"canGetNumberOfConnectedDevices");
		dll_canGetDeviceList =				(long (__stdcall *)(T_DeviceList *))GetProcAddress (hSIECADLL,"canGetDeviceList");
		dll_canGetDeviceTimestampBase =		(long (__stdcall *)(long,unsigned long *))GetProcAddress (hSIECADLL,"canGetDeviceTimestampBase");
		dll_canGetEepromAccess =			(long (__stdcall *)(void *,unsigned long,unsigned long *))GetProcAddress (hSIECADLL,"canGetEepromAccess");
		dll_canReadEeprom =					(long (__stdcall *)(void *,long,long,unsigned char * ))GetProcAddress (hSIECADLL,"canReadEeprom");
		dll_canWriteEeprom =				(long (__stdcall *)(void *,long,long,unsigned char * ))GetProcAddress (hSIECADLL,"canWriteEeprom");
		dll_canCloseEepromAccess =			(long (__stdcall *)(void *))GetProcAddress (hSIECADLL,"canCloseEepromAccess");
		dll_canFlashDevice =				(long (__stdcall *)(unsigned long,char *))GetProcAddress (hSIECADLL,"canFlashDevice");
		dll_canSetHWFilterEmu =				(long (__stdcall *)(void *,unsigned long,unsigned long * ))GetProcAddress (hSIECADLL,"canSetHWFilterEmu");
		dll_canEnableBusloadFilter =		(long (__stdcall *)(void *,enum e_filteredBusload))GetProcAddress (hSIECADLL,"canEnableBusloadFilter");
		dll_canEnableHWExtendedId =			(long (__stdcall *)(void *,bool))GetProcAddress (hSIECADLL,"canEnableHWExtendedId");
		dll_canGetBusloadFiltered =			(long (__stdcall *)(void *,struct st_busload *))GetProcAddress (hSIECADLL,"canGetBusloadFiltered");
		dll_canGetCanLevel =				(long (__stdcall *)(void *,enum e_canLevelMode,long,double *,double *))GetProcAddress (hSIECADLL,"canGetCanLevel");
		dll_canSetTxTimeout =				(long (__stdcall *)(void *,unsigned long))GetProcAddress (hSIECADLL,"canSetTxTimeout");
		dll_canSetPrioritySICE132 =			(long (__stdcall *)(unsigned long))GetProcAddress (hSIECADLL,"canSetPrioritySIECE132");
		dll_canGetPrioritySICE132 =			(long (__stdcall *)(unsigned long *))GetProcAddress (hSIECADLL,"canGetPrioritySIECE132");
		dll_createCustomSharedMemory =		(long (__stdcall *)(const char *,unsigned long,unsigned long,unsigned long))GetProcAddress (hSIECADLL,"createCustomSharedMemory");
		dll_canSetBaudrateForce =			(long (__stdcall *)(void *,long))GetProcAddress (hSIECADLL,"canSetBaudrateForce");
		dll_canGetDiffTimeLastFrame =		(long (__stdcall *)(void *,long *))GetProcAddress (hSIECADLL,"canGetDiffTimeLastFrame");
		dll_canGetHWSerialNumber =			(long (__stdcall *)(void *,unsigned long *,unsigned long *))GetProcAddress (hSIECADLL,"canGetHWSerialNumber");
		dll_canConfirmedTransmit =			(long (__stdcall *)(void *,struct st_canmsg *,long * ))GetProcAddress (hSIECADLL,"canConfirmedTransmit");
		dll_canReadNoWait =					(long (__stdcall *)(void *,struct st_canmsg *,long * ))GetProcAddress (hSIECADLL,"canReadNoWait");

		errSIECA = dll_canOpen(NumVoie + 20, 0 ,Echo, 0, 0, "", "", "", &hCan);

		if (errSIECA != NTCAN_SUCCESS)
		{
			msg.Format("No AGCO card detected",0);
			AfxMessageBox(msg);
			
			if(HardwareChoosed == -1)
				HARDWARE = 1;	//Si CanBox non d�tect�e ouvrir la CanCase 

			return NOT_OK;
		}


		switch (vitesse)
		{ 
			case CAN_100: bySpeed = BAUD_100KBIT;break;
			case CAN_125: bySpeed = BAUD_125KBIT;break;
			case CAN_250: bySpeed = BAUD_250KBIT;break;
			case CAN_500: bySpeed = BAUD_500KBIT;break;
			case CAN_1000: bySpeed = BAUD_1000KBIT;break;
			default: bySpeed = BAUD_250KBIT;break;
		}

		errSIECA = dll_canSetBaudrate(hCan,bySpeed);
		if (errSIECA != NTCAN_SUCCESS)
		{
			msg.Format("Erreur dll_canSetBaudrate (%d)",errSIECA);
			AfxMessageBox(msg);
			return NOT_OK;
		} 

		errSIECA = dll_canSetFilterMode(hCan,filterMode_nofilter);
		if (errSIECA != NTCAN_SUCCESS)
		{
			msg.Format("Erreur dll_canEnableHWExtendedId (%d)",errSIECA);
			AfxMessageBox(msg);
			return NOT_OK;
		}

		errSIECA = dll_canEnableAllIds(hCan,true);
		if (errSIECA != NTCAN_SUCCESS)
		{
			msg.Format("Erreur dll_canEnableAllIds (%d)",errSIECA);
			AfxMessageBox(msg);
			return NOT_OK;
		}

		//ActiveCanBox();

		ListeMessages.Can[NumChannel-1].bModeCanalyzer = false;


		return OK;


}

int CCanBox2::DesactiveCanBox()
{
	if(HARDWARE == 0)
	{
		int timeout=0;

		// Check if thread-handle is still there, If it is there thread must be stopped.
		ClearBuffer();
		if (d_eventStop)
		{
			SetEvent( d_eventStop );
		}

		if (hThread)
		{
			/*
			* SFE: 02.10.2009
			* stops CanRead
			* when there is no message on the bus which can read with CanRead-function it waits and waits and waits, so we need to break it that we can disbale the canbox
			*/
			if(dll_canIsHandleValid(hCan) == NTCAN_SUCCESS)
			{
				bStopThread = true;
				dll_canBreakCanRead(hCan);
			}
		}

		if (bLogFile)
		{
			bLogFile = 0;
			rapport.Close();
			ulNum = 0;
		}

		//Delete filtered frames
		if (dll_canIsHandleValid(hCan) == NTCAN_SUCCESS)
		{
			errSIECA = dll_canIdDeleteArray(hCan);
			if (errSIECA != NTCAN_SUCCESS)
			{
				tmp.Format("Erreur dll_canIdDeleteArray (%d) (%d)",NumChannel,errSIECA);
				AfxMessageBox(tmp);
				return NOT_OK;
			}
		}

		
		/*
		* SFE: 02.10.2009
		* using the comparision if (dll_canIsHandleValid(hCan) == NTCAN_SUCCESS) instead of if(!dll_canIsHandleValid(hCan))
		*/
		if (dll_canIsHandleValid(hCan) == NTCAN_SUCCESS)
		{
			errSIECA = dll_canClose(hCan);
			if (errSIECA != NTCAN_SUCCESS)
			{
				tmp.Format("Erreur dll_CANClose (%d) (%d)",NumChannel,errSIECA);
				AfxMessageBox(tmp);
				return NOT_OK;
			}
		}

		return OK;
	}

	if(HARDWARE == 1)
	{
		int timeout=0;

		// Check if thread-handle is still there, If it is there thread must be stopped.
		ClearBuffer();
		if (d_eventStop)
		{
			SetEvent( d_eventStop );
		}

		if (hThread)
		{
				bStopThread = true;
		}

		if (bLogFile)
		{
			bLogFile = 0;
			rapport.Close();
			ulNum = 0;
		}

		//D�sactive le Canal
		errSIECA = xlDeactivateChannel(g_xlPortHandle, g_xlChannelMask);
		if(XL_SUCCESS != errSIECA)
		{
			tmp.Format("Erreur xlDeactivateChannel (%d)", errSIECA);
			AfxMessageBox(tmp);
			return NOT_OK;
		}
		
		//Ferme le Port
		errSIECA = xlClosePort(g_xlPortHandle);
		if(XL_SUCCESS != errSIECA)
		{
			tmp.Format("Erreur xlClosePort (%d)", errSIECA);
			AfxMessageBox(tmp);
			return NOT_OK;
		}

		//Ferme le Driver
		xlCloseDriver();


		return OK;
	}

}



int CCanBox2::ActiveCanBox()
{	
	if(HARDWARE == 0)
	{
		
		DWORD dwThreadId = 1; 


		if ( hSIECADLL != NULL )
		{
			if (hThread == NULL)
			{
				/*
				* SFE: 02.10.2009
				* set the variables just in case the thread needs to be started not always again when the function ActiveCanBox is called
				*/
				FlagFinThread = false;
				bStopThread = false;

				// cr�ation de l'�venenment indiquant que l'on souhaite arr�ter le thread
				d_eventStop = CreateEvent(NULL, FALSE, FALSE, "event_StopThread");

				// Cr�ation du thread de d�pilement qui r�agit � un �v�nement
				hThread = CreateThread(
										NULL,                        // default security attributes
										0,                           // use default stack size
										ThreadDepilement,	         // thread function
										this,						 // argument to thread function
										0,                           // use default creation flags
										&dwThreadId );               // returns the thread identifier
				
				//	Permet de r�gler la priorit� du thread 
				SetThreadPriority(hThread,THREAD_PRIORITY_NORMAL);// HIGHEST
				
				dll_canClearBuffer(hCan);

				ret_DesactiveCanBox = 0;

				if (hThread == NULL)
				{
					#if NIV_AFFICH_ERROR == 2
						AfxMessageBox("Cr�ation Thread Depilement --> Pas OK",MB_ICONERROR);
					#endif
					return NOT_OK;
				}
				else
				{
					//Cr�ation du fichier de log
					CString tmp;
					tmp.Format("Can%d_Log.txt",NumChannel);
					//bLogFile = rapport.Open(tmp,CFile::modeCreate | CFile::modeWrite);

					iInst++;
					tmp.Format("Thread %d cr��e",iInst);
			//		AfxMessageBox(tmp);

					return OK;
				}
			}
			else
			{
				return OK;
			}
		}
		else
		{
			return OK;
		}
	}


	if(HARDWARE == 1)
	{
				
		DWORD dwThreadId = 1; 
		
		
		if ( hVXLAPIDLL != NULL )
		{
			if (hThread == NULL)
			{
				/*
				* SFE: 02.10.2009
				* set the variables just in case the thread needs to be started not always again when the function ActiveCanBox is called
				*/
				FlagFinThread = false;
				bStopThread = false;

				// cr�ation de l'�venenment indiquant que l'on souhaite arr�ter le thread
				d_eventStop = CreateEvent(NULL, FALSE, FALSE, "event_StopThread");

				// Cr�ation du thread de d�pilement qui r�agit � un �v�nement
				hThread = CreateThread(
										NULL,                        // default security attributes
										0,                           // use default stack size
										ThreadDepilement,	         // thread function
										this,						 // argument to thread function
										0,                           // use default creation flags
										&dwThreadId );               // returns the thread identifier
					
				//	Permet de r�gler la priorit� du thread 
				SetThreadPriority(hThread,THREAD_PRIORITY_NORMAL);// HIGHEST
					
					

				ret_DesactiveCanBox = 0;

				if (hThread == NULL)
				{
					#if NIV_AFFICH_ERROR == 2
						AfxMessageBox("Cr�ation Thread Depilement --> Pas OK",MB_ICONERROR);
					#endif
					return NOT_OK;
				}
				else
				{
					//Cr�ation du fichier de log
					CString tmp;
					tmp.Format("Can%d_Log.txt",NumChannel);
					//bLogFile = rapport.Open(tmp,CFile::modeCreate | CFile::modeWrite);

					iInst++;
					tmp.Format("Thread %d cr��e",iInst);
			//		AfxMessageBox(tmp);

					return OK;
				}
			}
			else
			{
				return OK;
			}
		}
		else
		{
			return OK;
		}

	}
}



int CCanBox2::MesureNiveauCan()
{
	if(HARDWARE == 0)
	{
		CString text="";
		double DomLow=0,DomHigh=0,RecLow=0,RecHigh=0;

		//Test Niveau CAN Recessif
		errSIECA = dll_canGetCanLevel(hCan,canLevelRecessive,0x100,&RecLow,&RecHigh);
	/*	if (errSIECA == NTCAN_SUCCESS)
		{
			text.Format("R�cessif (%d) :\nNiv Low : %04.2f V\nNiv High : %04.2f V",NumChannel,RecLow,RecHigh);
			AfxMessageBox(text);

			// D�tection boitier mesure analogique
			if ( (RecLow < 4.99) && (RecHigh < 4.99) )
			{
				text.Format("Probl�me de niveaux CAN sur bit r�cessif !\nCAN Low: %04.2f V  CAN High: %04.2f V",RecLow,RecHigh);
			
				if ( (RecLow<0.5) && (RecHigh<0.5) && (RecLow!=0)  && (RecHigh!=0))
					text.Insert(85,"\n\nLe bo�tier n'est pas r�f�renc� � la masse !");
				else if ( (RecLow<0.5) || (RecHigh<0.5))
					text.Insert(85,"\n\nCourt-circuit � la masse !");
				
				AfxMessageBox(text,MB_ICONERROR);
				DesactiveCanBox();
				exit(1);
			}
		}
		else
		{
			text.Format("Erreur dll_canGetCanLevel (Recessif) (%d)",errSIECA);
			AfxMessageBox(text);
		}
	*/
		//Test Niveau CAN Dominant
		errSIECA = dll_canGetCanLevel(hCan,canLevelDominant,0x100,&DomLow,&DomHigh);
	/*	if (errSIECA == NTCAN_SUCCESS)
		{
			text.Format("Dominant (%d) :\nNiv Low : %04.2f V\nNiv High : %04.2f V",NumChannel,DomLow,DomHigh);
			AfxMessageBox(text);

			// D�tection boitier mesure analogique
			if ( (DomLow < 4.99) && (DomHigh < 4.99) )
			{
					
				text.Format("Probl�me de niveaux CAN sur bit Dominant !\nCAN Low: %04.2f V  CAN High: %04.2f V",DomLow,DomHigh);
			
				if ( (DomLow < (DomHigh+0.2)) || (DomLow > (DomHigh-0.2)) )
					text.Insert(85,"\n\nCourt-circuit entre la ligne CAN Low et CAN High !");
				else if ( (DomLow>=1.5) || (DomHigh>=2.99))
					text.Insert(85,"\n\nCourt-circuit au +12V !");
				else if ( (DomLow<0.5) || (DomHigh<0.5))
					text.Insert(85,"\n\nCourt-circuit � la masse !");
			
				AfxMessageBox(text,MB_ICONERROR);
				DesactiveCanBox();
				exit(1);
			}
		}
		else
		{
			text.Format("Erreur dll_canGetCanLevel (Dominant) (%d)",errSIECA);
			AfxMessageBox(text);
		}
	*/
		Buf_Dominant_Low_Level = DomLow * 100;
		Buf_Dominant_High_Level = DomHigh * 100;
		Buf_Recessif_Low_Level = RecLow * 100;
		Buf_Recessif_High_Level = RecHigh * 100;



		return OK;
	}
	
	if(HARDWARE == 1)
	{
		//Pas de fonction fournie par le constructeur (Vector) permettant de mesurer les niveaux de tension
		Buf_Dominant_Low_Level = 0;
		Buf_Dominant_High_Level = 0;
		Buf_Recessif_Low_Level = 0;
		Buf_Recessif_High_Level = 0;

		return OK;
	}

}

int CCanBox2::SetErrorFrameDetection(unsigned int bitrate, int channel)
{

	int rdata[16];
   
	rdata[0] = SET_EFRREC_FREQ;  
	rdata[3] = channel;   
	   
	rdata[4] = (bitrate >> 24) & 0xFF;
	rdata[5] = (bitrate >> 16) & 0xFF;
	rdata[6] = (bitrate >> 8 ) & 0xFF;
	rdata[7] = bitrate & 0xFF;

//	if ( ca_Instruction_1 (rdata) )  
		return OK;
//	else  
//		return NOT_OK;
}

ULONG CCanBox2::ReadVersionCanUsb()
{
   int sdata[16];
   int k=0;
   int erg = 0;
   char buf[256];
   ULONG ulVersion=0;
   PCHAR s=NULL;
   char szTemp[10];
      
   for (k=0 ; k<16 ; k++) sdata[k]=0;
   sdata[0]=0x25;

   erg = canInstruction(sdata);
   if(erg) 
   {
      buf[0]=0;
      for(k=3;k<16;k++) 
         buf[k-3] = (char)sdata[k];
      
      buf[12]=0;
      //-------------------------------------------------------
      // Convert String "V00.2400.03" to ULONG 0x00030018 (hex)
      //-------------------------------------------------------
      szTemp[0]=0;
      strncpy(szTemp, &buf[6], 2);
      ulVersion = strtoul(szTemp, &s, 10);
      ulVersion <<= 8;

      szTemp[0]=0;
      strncpy(szTemp, &buf[9], 2);
      ulVersion |= (UCHAR) (strtoul(szTemp, &s, 10) & 0xFF);
      ulVersion <<= 8;

      szTemp[0]=0;
      strncpy(szTemp, &buf[1], 2);
      ulVersion |= (UCHAR) (strtoul(szTemp, &s, 10) & 0xFF);
      ulVersion <<= 8;

      szTemp[0]=0;
      strncpy(szTemp, &buf[4], 2);
      ulVersion |= (UCHAR) (strtoul(szTemp, &s, 10) & 0xFF);
   } 
   else 
   {
      return NOT_OK;    
   } 

   return ulVersion;
}

//Thread de depilement
DWORD WINAPI ThreadDepilement(LPVOID lpParam)
{
	CCanBox2 *CanUsb = (CCanBox2*)lpParam;

	while( WaitForSingleObject( CanUsb->d_eventStop, 0 ) == WAIT_TIMEOUT )
	{
		//CanUsb->TraiteCanBox(CanUsb->NumChannel);
		Sleep(10);
	};

	CanUsb->FlagFinThread = true;
	
	/*
	* SFE: 02.10.2009
	* remove the close handle from here and move it to the destructor, because here it may happen that the handle isn't valid because the class object is already deleted. 
	* that race condition was the main reason why the activity-dll crashed the datacollection
	*/
	/*
	CloseHandle( CanUsb->d_eventStop );
	CanUsb->d_eventStop = NULL;
	*/
	return OK;

}

void CCanBox2::TraiteCanBox(int NumeroChannel)
{
	if(HARDWARE == 0)
	{
		messageStruct MessageRet,*MessageRech;
		unsigned int ui_data[4];
		unsigned char uc_data[16];
		
		CMSG msgData;
		long l_len = 1;

		void (* FonctionTraiteMessage)(messageStruct *MessageRech) = NULL;

		int status=0;
		
		// D�pile la file d'attente tant que pr�sence trame dans la file
		while( !bStopThread )
		{

			errSIECA = dll_canRead(hCan, &msgData, &l_len);
			dll_canBreakCanRead(hCan);

			if (errSIECA == NTCAN_SUCCESS)
			{
				//Adaptation de variables SIECA vers variables CANAPI
				ui_data[0] = msgData.l_id;

				ui_data[1] = msgData.ul_tstamp;
				ui_data[2] = 0;
				ui_data[3] = 0; 
				
				uc_data[0] = (NumeroChannel-1) + (msgData.by_extended<<1) + (msgData.by_remote<<2) + 0x80;
				uc_data[1] = 0;
				uc_data[2] = msgData.by_len;
				uc_data[3] = 0;
				uc_data[4] = msgData.aby_data[0];
				uc_data[5] = msgData.aby_data[1];
				uc_data[6] = msgData.aby_data[2];
				uc_data[7] = msgData.aby_data[3];
				uc_data[8] = msgData.aby_data[4];
				uc_data[9] = msgData.aby_data[5];
				uc_data[10] = msgData.aby_data[6];
				uc_data[11] = msgData.aby_data[7];
				uc_data[12] = 0;
				uc_data[13] = 0;
				uc_data[14] = 0;
				uc_data[15] = 0;

				// si recu message de donn�e au format standard ou �tendu
	//			if ((uc_data[0] & datflag) == datflag)
	//			{

					if (bLogFile)
					{
						if (ui_data[0] == 0x50F)
						{
							CString tmp,msg;
							tmp.Format("%10d\t0x%X\t%d\t",ulNum,ui_data[0],uc_data[2]);
							for (int i = 0 ; i < uc_data[2] ; i++)
							{
								msg = tmp;
								tmp.Format("%s\t%02X",msg,uc_data[4+i]);
							}
							msg.Format("%s\n",tmp);
							rapport.WriteString(msg);
						}
						ulNum++;
					}

					// recherche l'identifiant directement dans la liste de r�ception
					if( ( ListeMessages.Can[(uc_data[0] & ch2flag)].RetourneElement((unsigned long)ui_data[0],&MessageRech) != NOT_OK ) || (ListeMessages.Can[(uc_data[0] & ch2flag)].bModeCanalyzer))
					{
						MessageRet.ident = (unsigned long)ui_data[0];

						// si identifiant �tendu
						if ((uc_data[0]  & extflag) == extflag )
							MessageRet.ident |= CAN29BITS;
						
						MessageRet.remote = (unsigned char)((uc_data[0] & rtrflag) >> 2);

						if (MessageRet.remote == 1)
							MessageRet.taille = 0;
						else
							MessageRet.taille = (unsigned char)uc_data[2];	
						
						for (int i=0;i<MessageRet.taille;i++)
							MessageRet.data[i]=(unsigned char)uc_data[4+i];
						

						//Gestion du temps
						MessageRet.time.wMilliseconds = ui_data[1];
						if (MessageRet.time.wMilliseconds < MessageRet.uiMilliseconds)
						{
							MessageRet.uiCptOverflowMillisec++;
						}
						MessageRet.uiMilliseconds = MessageRet.time.wMilliseconds;

						if (MessageRet.uiCptOverflowMillisec >= 65535)
						{
							MessageRet.uiCptOverflowMillisec = 0;
						}
						MessageRet.ulTimeInMilliseconds		= (65535 * MessageRet.uiCptOverflowMillisec) + MessageRet.uiMilliseconds;
						MessageRet.uiTimeSpan				= MessageRet.ulTimeInMilliseconds - MessageRet.ulOldTimeInMilliseconds;
						MessageRet.ulOldTimeInMilliseconds	= MessageRet.ulTimeInMilliseconds;
						
						////////////////////////////////////////////////////////


						if (ListeMessages.Can[(uc_data[0] & ch2flag)].bModeCanalyzer)
						{
		 					FonctionTraiteMessage = (void (*)(messageStruct *MessageRech))(ListeMessages.Can[(uc_data[0] & ch2flag)].CanalyzerFonction);
						}
						else
						{
		 					FonctionTraiteMessage = (void (*)(messageStruct *MessageRech))(MessageRech->fonction);
						}
						FonctionTraiteMessage(&MessageRet);
					}
	/*				else
					{
						Nb_Mess = ListeMessages.Can[(uc_data[0] & ch2flag)].NombreMessage();
						
						// si l'identifiant n'a pas �t� directement trouv�, on recherche gr�ce au masque			
						for (int i=0;i< Nb_Mess;i++)
						{
							Nb_Mess = ListeMessages.Can[(uc_data[0] & ch2flag)].NombreMessage();
							
							if ( ListeMessages.Can[(uc_data[0] & ch2flag)].RetourneElement((unsigned int)i,&MessageRech) == OK )
							{
								if (!( (MessageRech->ident ^ (unsigned int)ui_data[0]) & (~MessageRech->masque)  ) )
								{								
									MessageRet.ident = (unsigned long)ui_data[0];

									if ((uc_data[0]  & extflag) == extflag )
										MessageRet.ident |= CAN29BITS;	
							
									MessageRet.remote = (unsigned char)((uc_data[0] & rtrflag) >> 2);
									if (MessageRet.remote == 1)
										MessageRet.taille = 0;
									else
										MessageRet.taille = (unsigned char)uc_data[2];	
													
									for (int i=0;i<MessageRet.taille;i++)
										MessageRet.data[i]=(unsigned char)uc_data[4+i];
								
									//Gestion du temps
									MessageRet.time.wMilliseconds = ui_data[1];
									if (MessageRet.time.wMilliseconds < MessageRet.uiMilliseconds)
									{
										MessageRet.uiCptOverflowMillisec++;
									}
									MessageRet.uiMilliseconds = MessageRet.time.wMilliseconds;

									if (MessageRet.uiCptOverflowMillisec >= 65535)
									{
										MessageRet.uiCptOverflowMillisec = 0;
									}
									MessageRet.ulTimeInMilliseconds		= (65535 * MessageRet.uiCptOverflowMillisec) + MessageRet.uiMilliseconds;
									MessageRet.uiTimeSpan				= MessageRet.ulTimeInMilliseconds - MessageRet.ulOldTimeInMilliseconds;
									MessageRet.ulOldTimeInMilliseconds	= MessageRet.ulTimeInMilliseconds;
									////////////////////////////////////////////////////////


									FonctionTraiteMessage = (void (*)(messageStruct *MessageRech))(MessageRech->fonction);
									FonctionTraiteMessage(&MessageRet);

									break;
								}
							}
						}
					}
	*/
	//			}
			}
			else
			{
				if (bLogFile)
				{
					CString tmp;
					tmp.Format("ERREUR RECEIVE CANUSB (%d)\n",errSIECA);
					rapport.WriteString(tmp);
				}
			}
		};

	}

	if(HARDWARE == 1)
	{
		messageStruct MessageRet,*MessageRech;
		unsigned int ui_data[4];
		unsigned char uc_data[16];
		
		unsigned int    msgsrx = RECEIVE_EVENT_SIZE;
		XLevent         xlEvent;



		void (* FonctionTraiteMessage)(messageStruct *MessageRech) = NULL;

		int status=0;
		
		// D�pile la file d'attente tant que pr�sence trame dans la file
		while( !bStopThread )
		{
			msgsrx = RECEIVE_EVENT_SIZE;


			errVXLAPI = xlReceive(g_xlPortHandle, &msgsrx, &xlEvent);
		
						

			if(errVXLAPI == XL_SUCCESS)	
			{
				CounterRx++;			
			} 
			if(xlEvent.tagData.msg.flags == XL_CAN_MSG_FLAG_ERROR_FRAME) ErCounter++;
			
			

			if (errVXLAPI == XL_SUCCESS)
			{
				//Adaptation de variables VXLAPI vers variables CANAPI
				if(xlEvent.tagData.msg.id <=0x7FF)	ui_data[0] = xlEvent.tagData.msg.id;
				else ui_data[0] = xlEvent.tagData.msg.id - 0x80000000;

				ui_data[1] = xlEvent.timeStamp / 100000;
				ui_data[2] = 0;
				ui_data[3] = 0;


				if(ui_data[0]<=0x7FF)	uc_data[0] = (NumeroChannel) + 0x7F;
				else					uc_data[0] = (NumeroChannel) + 0x81;

				uc_data[1] = 0;
				uc_data[2] = xlEvent.tagData.msg.dlc;
				uc_data[3] = 0;
				uc_data[4] = xlEvent.tagData.msg.data[0];
				uc_data[5] = xlEvent.tagData.msg.data[1];
				uc_data[6] = xlEvent.tagData.msg.data[2];
				uc_data[7] = xlEvent.tagData.msg.data[3];
				uc_data[8] = xlEvent.tagData.msg.data[4];
				uc_data[9] = xlEvent.tagData.msg.data[5];
				uc_data[10] = xlEvent.tagData.msg.data[6];
				uc_data[11] = xlEvent.tagData.msg.data[7];			
				uc_data[12] = 0;
				uc_data[13] = 0;
				uc_data[14] = 0;
				uc_data[15] = 0;

				// si recu message de donn�e au format standard ou �tendu
	//			if ((uc_data[0] & datflag) == datflag)
	//			{

				//	Permet de programmer plus rapidement les auto5 (pas utilis�)
					if (bLogFile)
					{
						if (ui_data[0] == 0x50F)
						{
							CString tmp,msg;
							tmp.Format("%10d\t0x%X\t%d\t",ulNum,ui_data[0],uc_data[2]);
							for (int i = 0 ; i < uc_data[2] ; i++)
							{
								msg = tmp;
								tmp.Format("%s\t%02X",msg,uc_data[4+i]);
							}
							msg.Format("%s\n",tmp);
							rapport.WriteString(msg);
						}
						ulNum++;
					}

				
					// recherche l'identifiant directement dans la liste de r�ception
					if( ( ListeMessages.Can[(uc_data[0] & ch2flag)].RetourneElement((unsigned long)ui_data[0],&MessageRech) != NOT_OK ) || (ListeMessages.Can[(uc_data[0] & ch2flag)].bModeCanalyzer))
					{
						MessageRet.ident = (unsigned long)ui_data[0];	

						// si identifiant �tendu
						if ((uc_data[0]  & extflag) == extflag )
							MessageRet.ident |= CAN29BITS;
						
						MessageRet.remote = (unsigned char)((uc_data[0] & rtrflag) >> 2);

						if (MessageRet.remote == 1)
							MessageRet.taille = 0;
						else
							MessageRet.taille = (unsigned char)uc_data[2];	
						
						for (int i=0;i<MessageRet.taille;i++)
							MessageRet.data[i]=(unsigned char)uc_data[4+i];
						

						//Gestion du temps
						MessageRet.time.wMilliseconds = ui_data[1];
						if (MessageRet.time.wMilliseconds < MessageRet.uiMilliseconds)
						{
							MessageRet.uiCptOverflowMillisec++;
						}
						MessageRet.uiMilliseconds = MessageRet.time.wMilliseconds;

						if (MessageRet.uiCptOverflowMillisec >= 65535)
						{
							MessageRet.uiCptOverflowMillisec = 0;
						}
						MessageRet.ulTimeInMilliseconds		= (65535 * MessageRet.uiCptOverflowMillisec) + MessageRet.uiMilliseconds;
						MessageRet.uiTimeSpan				= MessageRet.ulTimeInMilliseconds - MessageRet.ulOldTimeInMilliseconds;
						MessageRet.ulOldTimeInMilliseconds	= MessageRet.ulTimeInMilliseconds;

						////////////////////////////////////////////////////////

						if (ListeMessages.Can[(uc_data[0] & ch2flag)].bModeCanalyzer)
						{
		 					FonctionTraiteMessage = (void (*)(messageStruct *MessageRech))(ListeMessages.Can[(uc_data[0] & ch2flag)].CanalyzerFonction);
						}
						else
						{
		 					FonctionTraiteMessage = (void (*)(messageStruct *MessageRech))(MessageRech->fonction);
						}
						FonctionTraiteMessage(&MessageRet);
					}

			}
			else
			{
				if (bLogFile)
				{
					CString tmp;
					tmp.Format("ERREUR xlReceive (%d)\n",errVXLAPI);
					rapport.WriteString(tmp);
				}
			}
		};

	}

}

int CCanBox2::AjouterMessageEmission(HANDLE channel, unsigned long Ident, int Ext, unsigned char Remote, unsigned char *data, unsigned char Dlc)
{
	if(HARDWARE == 0)
	{
		CMSG	msgToSend;
		long	l_len;

		int DataToSendTX[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

		DataToSendTX[11] = Remote;
        
        if(Ext == CAN11BITS)
        {
            /* Id standard */
            DataToSendTX[0] = (Ident & 0x000007FF);
        }
        else
        {
            /* Id extended */
            DataToSendTX[0] = (Ident & 0x1FFFFFFF);
        }

		if (Remote == CAN_REMOTE)
		{
			DataToSendTX[1] = 0;	
		}
		else
		{
			DataToSendTX[1] = Dlc;
		
			for(int i=0; i<Dlc; i++)
            {
				DataToSendTX[2+i] = data[i];
            }
		}

		//Adaptation variables CANAPI vers SIECA
		l_len = 1;

		msgToSend.l_id = DataToSendTX[0];
		msgToSend.by_len = DataToSendTX[1];
		msgToSend.by_extended = (Ext >> 30);
		msgToSend.by_remote  = DataToSendTX[11];
        
		for(int i=0; i<msgToSend.by_len; i++)
        {
            msgToSend.aby_data[i] = DataToSendTX[2+i];
        }
		errSIECA = dll_canSend(hCan, &msgToSend, &l_len);
		if (errSIECA == NTCAN_SUCCESS)
			return OK;
		else
		{	
			#if NIV_AFFICH_ERROR == 2
				AfxMessageBox(LoadMyStringResource(7),MB_ICONERROR);
			#endif
			
			return NOT_OK;
		}
	}

	if(HARDWARE == 1)
	{

		XLevent			xlEventToSend;
		unsigned int	messageCount = 1;

		int DataToSendTX[16]={0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0 };

		DataToSendTX[0] = Ident;
		DataToSendTX[11] = Remote;

		if (Remote == 1)
		{
			DataToSendTX[1] = 0;	
		}
		else
		{
			DataToSendTX[1] = Dlc;
		
			for(int i=0;i<Dlc;i++)
				DataToSendTX[2+i] = data[i];
		}


		if (Ext == CAN29BITS)
		{
			/* Extended ID */
			/* Set the MSB */
			DataToSendTX[0] = (Ident | 0x80000000);
		}

		//Adaptation variables CANAPI vers VXLAPI
		xlEventToSend.tag				= XL_TRANSMIT_MSG;
		xlEventToSend.tagData.msg.id	= DataToSendTX[0];
		xlEventToSend.tagData.msg.dlc	= DataToSendTX[1];
		if(DataToSendTX[11] == 1)	xlEventToSend.tagData.msg.flags	= XL_CAN_MSG_FLAG_REMOTE_FRAME;
		else xlEventToSend.tagData.msg.flags	= 0;

		for(int i=0;i<xlEventToSend.tagData.msg.dlc;i++)
			xlEventToSend.tagData.msg.data[i] = DataToSendTX[2+i];
		
		errVXLAPI = xlCanTransmit(g_xlPortHandle, g_xlChannelMask, &messageCount, &xlEventToSend);


		if (errVXLAPI == XL_SUCCESS || errVXLAPI == XL_ERR_QUEUE_IS_EMPTY)
		{
/*
			printf("EX_MSG id=%d, l=%d, %d%d%d%d%d%d%d%d, CEX:%d\n", xlEventToSend.tagData.msg.id, xlEventToSend.tagData.msg.dlc, 
				xlEventToSend.tagData.msg.data[0], xlEventToSend.tagData.msg.data[1], xlEventToSend.tagData.msg.data[2], xlEventToSend.tagData.msg.data[3],
					xlEventToSend.tagData.msg.data[4], xlEventToSend.tagData.msg.data[5], xlEventToSend.tagData.msg.data[6], xlEventToSend.tagData.msg.data[7],
						CounterEx);
*/
			Sleep(1);

			CounterEx++;
			return OK;
		}
		else
		{	
			ErCounter++;
			#if NIV_AFFICH_ERROR == 2
				AfxMessageBox(LoadMyStringResource(7),MB_ICONERROR);
			#endif
			
			return NOT_OK;
		}
	}
}

int CCanBox2::AjouterMessageReception(unsigned long Ident, unsigned long etendu, unsigned long mask, void (__cdecl *FonctionTraiteMessage)(messageStruct *))
{

	if(HARDWARE == 0)
	{	
		int ret;
		unsigned char data[8]={0,0,0,0,0,0,0,0};


		CString tmp;
		tmp.Format("Ajout de 0x%X\n",Ident);
	//	AfxMessageBox(tmp);
	//	fCanLog.WriteString(tmp);



		#if (DEBUG_MB == 1)
			CString msgbx;
			msgbx.Format("Ajout R�ception (%d)\nIdent : 0x%X",NumChannel,Ident);
			AfxMessageBox(msgbx);
		#endif

		if (Ident == 0)
		{
			ListeMessages.Can[NumChannel-1].bModeCanalyzer = true;
		}
		else
		{
			ListeMessages.Can[NumChannel-1].bModeCanalyzer = false;
		}


	//	printf("\nIdent:%d	mask:%d	data:%c etendu:%d NumChannel:%d", Ident, mask, data, etendu, NumChannel);
		ret = ListeMessages.Can[NumChannel-1].AjouterMessage(Ident,mask,0,FonctionTraiteMessage,data,0);

		#if NIV_AFFICH_ERROR == 2
			if (ret == NOT_OK)	
				AfxMessageBox("AjouterMessageReception --> Pas OK",MB_ICONERROR);
		#endif

		
		CString msg;
		errSIECA = dll_canEnableAllIds(hCan,false);
		if (errSIECA != NTCAN_SUCCESS)
		{
			msg.Format("Erreur dll_canEnableAllIds (%d)",errSIECA);
	//		AfxMessageBox(msg);
			return NOT_OK;
		}

		errSIECA = dll_canIdAdd(hCan,Ident);
		if (errSIECA != NTCAN_SUCCESS)
		{
			msg.Format("Erreur dll_canIdAdd (%d)",errSIECA);
	//		AfxMessageBox(msg);
			return NOT_OK;
		}

		//INITIALISATION DU FILTRE
	//	ConfigureFiltre(Ident,mask);

		return ret; // Retourne l'erreur
	}

	if(HARDWARE == 1)
	{

		int ret;
		unsigned char data[8]={0,0,0,0,0,0,0,0};


		CString tmp;
		tmp.Format("Ajout de 0x%X\n",Ident);
	//	AfxMessageBox(tmp);
	//	fCanLog.WriteString(tmp);



		#if (DEBUG_MB == 1)
			CString msgbx;
			msgbx.Format("Ajout R�ception (%d)\nIdent : 0x%X",NumChannel,Ident);
			AfxMessageBox(msgbx);
		#endif

		if (Ident == 0)
		{
			ListeMessages.Can[NumChannel-1].bModeCanalyzer = true;
		}
		else
		{
			ListeMessages.Can[NumChannel-1].bModeCanalyzer = false;
		}


	//	printf("\nIdent:%d	mask:%d	data:%c etendu:%d NumChannel:%d", Ident, mask, data, etendu, NumChannel);		
		ret = ListeMessages.Can[NumChannel-1].AjouterMessage(Ident,mask,0,FonctionTraiteMessage,data,0);

		#if NIV_AFFICH_ERROR == 2
			if (ret == NOT_OK)	
				AfxMessageBox("AjouterMessageReception --> Pas OK",MB_ICONERROR);
		#endif


		////////////////////////////////////R�alise un filtre mat�riel//////////////////////////////////////
		
		CString msg;

		XLstatus xls;
		xls = xlCanSetChannelAcceptance(g_xlPortHandle, g_xlChannelMask, 0x000, 0x000, XL_CAN_STD);
		xls = xlCanSetChannelAcceptance(g_xlPortHandle, g_xlChannelMask, 0x000, 0x000, XL_CAN_EXT);
		if(xls!= XL_SUCCESS)
		{
			msg.Format("Erreur xlCanSetChannelAcceptance (%d)",xls);
			AfxMessageBox(msg);
			return NOT_OK;
		}

		//xls = xlCanAddAcceptanceRange(g_xlPortHandle, g_xlChannelMask, Ident, Ident);
		//if(xls!= XL_SUCCESS)
		//{
		//	msg.Format("Erreur xlCanAddAcceptanceRange (%d)",xls);
		//	//AfxMessageBox(msg);
		//	return NOT_OK;
		//}
		
		///////////////////////////////////////////////////////////////////////////////////////////////////


		//INITIALISATION DU FILTRE
	//	ConfigureFiltre(Ident,mask);

		return ret; // Retourne l'erreur
	}
}

// Calcul du filtre gr�ce au Id d�clar� en r�ception ainsi que leur masque
int CCanBox2::ConfigureFiltre(unsigned long Ident, unsigned long masque)
{

	// Si Id standard
	if ((Ident <= 0x7FF) && (masque <= 0x7FF)) 
	{
		mask_court |= masque;
		mask_court |= Ident;

		filt[12] |= ((mask_court >> 3) & 0xFF );
		filt[13] |= ((mask_court << 5 ) & 0xE0);
		filt[13] |= 7;
	}
	else 
	{	
		mask_long |= masque ;
		mask_long |= Ident ;
	
		filt[4] |= ((mask_long >> 21) & 0xFF );
		filt[5] |= (((mask_long >> 13)|16) & 0xF0);
		filt[5] |= (((mask_long >> 15)|8) & 0xFF);
		filt[6] |= ((mask_long >> 7) & 0xFF);
		filt[7] |= ((mask_long <<1) & 0xFF);
	}
	
	return OK;
}


int CCanBox2::AppliqueFiltre(int etat)
{
	int ret = 1;

	if (etat == 1)
	{
		// Initialisation du canal CAN
/*		ret = ca_InitCanCard_1( NumChannel,  // channel
								0x00,  // reserved
								0x00,  // reserved   
								0x00,  // reserved
								filt, // filter array of int[16];
      							btr0,  // BTR0   
   								btr1,  // BTR1   
   								0x00,  // reserved
   								0x00,  // typ 0 = 2 x 32 Bit, 1 = 4 x 16 Bit, 
		   								// 2 = 8 x 8 Bit, 3 = kein durchlass
   								0x00); // reserved
*/
		if (ret)
			return OK;
		else
			return NOT_OK;
	}
	else if (etat == 0)
	{
		int filtrePassant[16]={ 0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,
						0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF };

		// Initialisation du canal CAN
/*		ret = ca_InitCanCard_1( NumChannel,  // channel
								0x00,  // reserved
								0x00,  // reserved   
								0x00,  // reserved
								filtrePassant, // filter array of int[16];
      							btr0,  // BTR0   
   								btr1,  // BTR1   
   								0x00,  // reserved
   								0x00,  // typ 0 = 2 x 32 Bit, 1 = 4 x 16 Bit, 
		   								// 2 = 8 x 8 Bit, 3 = kein durchlass
   								0x00); // reserved
*/
		if (ret)
			return OK;
		else
			return NOT_OK;
	}
	else
		return NOT_OK;
}

int CCanBox2::canInstruction(int *arrData)
{
	if(HARDWARE == 0)
	{
		return dll_canInstruction(hCan,(unsigned int*)arrData);
	}

	if(HARDWARE == 1)
	{
		return OK;
	}
}

int CCanBox2::Ecrire_Donnee(int numero, int valeur)
{
	if(HARDWARE ==0)
	{
		switch(numero)
		{
			case CAN_LEVEL:
			{
				int data[16];
		
				data[0] = GET_CANPEGEL;
				if (NumChannel == 1)
					data[1]=0x01;
				else
					data[1]=0x05;

				// Mesure sur un bit r�cessif
				if (!canInstruction(data))
				{
					Buf_Recessif_Low_Level = data[1];
					Buf_Recessif_High_Level = data[2];
					Flag_Recessif_Low_error = data[3];
					Flag_Recessif_High_error = data[4];
								
				}
				else
				{
					return NOT_OK;
					break;
				}

				data[0]=GET_CANPEGEL;
				if (NumChannel == 1)
				{
					data[1]=0x03;
					data[2]=0x00;
					data[3]=0x01;
				}
				else
				{
					data[1]=0x07;
					data[2]=0x00;
					data[3]=0x02;
				}

				// Mesure sur un bit dominant
				if (!canInstruction(data)) 
				{
					Buf_Dominant_Low_Level = data[1];
					Buf_Dominant_High_Level = data[2];
					Flag_Dominant_Low_error = data[3];
					Flag_Dominant_High_error = data[4];
				}
				else
				{
					return NOT_OK;
					break;
				}


				return OK;

	//			MesureNiveauCan();
				break;
			}
			case FILTER_STATE: return AppliqueFiltre(valeur);break;
			case WRITE_CANUSB_EEPROM:return WriteEEPROM(numero,valeur);break;
			default: return UNKNOW_COMMAND;break;
		}
	}

	if(HARDWARE == 1)
	{
		switch(numero)
		{	
			case CAN_LEVEL: return OK;
			case FILTER_STATE: return AppliqueFiltre(valeur);break;
			case WRITE_CANUSB_EEPROM:return WriteEEPROM(numero,valeur);break;
			default: return UNKNOW_COMMAND;break;
		}

	}
	
}

int CCanBox2::WriteEEPROM(int NumByte, int value)
{
   int wdata[16];
          
   wdata[0] = 0xB1;       // Command Byte for EEPROM write
   wdata[2] = NumByte;          // Offset in the EEPROM Area (0-103)
   wdata[3] = 1;          // Number of Datenbytes
                             // (8 max., more than 8 cause an Errorreply 0 !)
   wdata[4] = value;
     
   if (!canInstruction(wdata)) 
   {
       return UNUSABLE_VALUE;
   } 
   else
   { 
       return VALUE_OK;
   }
}

int CCanBox2::Lire_Donnee(int numero, int *error)
{
	
	if(HARDWARE == 0)
	{
		int err1, err2, err3;

		stCounter2 = GetCounterEx(&err1);
		stBusLoad = GetBusInfos(&err2);
		stCanStatus = GetBusInfos2(&err3);
		

		switch (numero)
		{
			case ERROR_FRAMES_COUNTER:
			{
	/*			data[0] = READCTR;
				if (NumChannel == 1)
					Num = 1;
				else
					Num = 2;
				
				if (!canInstruction(data))
				{
					*error = VALUE_OK;
					return data[5+Num-1];
					break;
				}
				else
				{
					*error = UNUSABLE_VALUE;
					return 0;
					break;
				}
	*/			
				if (!err1)
				{
					*error = VALUE_OK;
					if (NumChannel == 1)
						return stCounter2.ul_errctr1;
					else
						return stCounter2.ul_errctr2;
				}
				else
				{
					*error = UNUSABLE_VALUE;
					return 0;
				}

				break;
			}
			case BUS_OFF_COUNTER:
			{
	/*			*error = VALUE_OK;
				return Buf_BusOff_Counter;
				break;
	*/
				if (!err3)
				{
					*error = VALUE_OK;
					return stCanStatus.w_busoffctr;
				}
				else
				{
					*error = UNUSABLE_VALUE;
					return 0;
				}

				break;
			}
			case BUS_OFF_COUNTER_CAN2:
			{
	/*			*error = VALUE_OK;
				return Buf_BusOff_Counter2;
				break;
	*/
				if (!err3)
				{
					*error = VALUE_OK;
					return stCanStatus.w_busoffctr;
				}
				else
				{
					*error = UNUSABLE_VALUE;
					return 0;
				}

				break;
			}
			case RX_COUNTER:
			{
	/*			data[0] = READCTR;
				
				if (NumChannel == 1)
					Num=2;
				else
					Num=4;

				if (!canInstruction(data))
				{
					*error = VALUE_OK;
					
					return data[Num];
					break;
					
				}
				else
				{
					*error = UNUSABLE_VALUE;
					return 0;
					break;
				}
	*/
				if (!err1)
				{
					*error = VALUE_OK;
					if (NumChannel == 1)
						return stCounter2.ul_rxframectr1 + stCounter2.ul_erxframectr1;
					else
						return stCounter2.ul_erxframectr2 + stCounter2.ul_erxframectr2;
				}
				else
				{
					*error = UNUSABLE_VALUE;
					return 0;
				}

				break;
			}
			case TX_COUNTER:
			{
	/*			data[0] = READCTR;
				if (NumChannel == 1)
					Num = 1;
				else
					Num = 3;
				if (!canInstruction(data))
				{
					*error = VALUE_OK;
					return data[Num]; 
					break;
				}
				else
				{
					*error = UNUSABLE_VALUE;
					return 0;
					break;
				}
	*/
				if (!err1)
				{
					*error = VALUE_OK;
					if (NumChannel == 1)
						return stCounter2.ul_txframectr1 + stCounter2.ul_etxframectr1;
					else
						return stCounter2.ul_txframectr2 + stCounter2.ul_etxframectr2;
				}
				else
				{
					*error = UNUSABLE_VALUE;
					return 0;
				}

				break;
			}
			case BUS_LOAD_PERCENT:
			{
	/*			data[0] = GET_BUSLOAD;
				if (NumChannel == 1)
					Num = 1;
				else
					Num = 2;
				if (!canInstruction(data))
				{
					*error = VALUE_OK;
					return data[14+Num-1];
					break;
				}
				else
				{
					*error = UNUSABLE_VALUE;
					return 0;
					break;
				}
	*/
				if (!err2)
				{
					*error = VALUE_OK;
					return stBusLoad.ul_load;
				}
				else
				{
					*error = UNUSABLE_VALUE;
					return 0;
				}

				break;
			}
			case DOMINANT_HIGH_LEVEL:
			{
				*error = VALUE_OK;
				return Buf_Dominant_High_Level;
				break;
			}
			case DOMINANT_LOW_LEVEL:
			{
				*error = VALUE_OK;
				return Buf_Dominant_Low_Level;
				break;
			}
			case RECESSIF_HIGH_LEVEL:
			{
				*error = VALUE_OK;
				return Buf_Recessif_High_Level;
				break;
			}
			case RECESSIF_LOW_LEVEL:
			{
				*error = VALUE_OK;
				return Buf_Recessif_Low_Level;
				break;
			}
			case DOMINANT_ERROR_HIGH:
			{
				*error = VALUE_OK;
				return Flag_Dominant_High_error;
				break;
			}
			case DOMINANT_ERROR_LOW:
			{
				*error = VALUE_OK;
				return Flag_Dominant_Low_error;
				break;
			}
			case RECESSIF_ERROR_HIGH:
			{
				*error = VALUE_OK;
				return Flag_Recessif_High_error;
				break;
			}
			case RECESSIF_ERROR_LOW:
			{
				*error = VALUE_OK;
				return Flag_Recessif_Low_error;
				break;
			}
			case READ_CANUSB_EEPROM:
			{
				int tmp = ReadEEPROM(numero);
				if (tmp == NOT_OK)
					*error = UNUSABLE_VALUE;
				else
					*error = VALUE_OK;
				return tmp;
				break;
			}
			case TX_ERROR_COUNTER:
			{
				*error = VALUE_OK;
				return Buf_TxError;
				break;
			}
			case TX_ERROR_COUNTER_CAN2:
			{
				*error = VALUE_OK;
				return Buf_TxErrorCan2;
				break;
			}
			default:
			{
				*error = UNKNOW_COMMAND;
				return 0;
				break;
			}
		}
		
		
	}

	if(HARDWARE == 1)
	{
		switch (numero)
		{
			case ERROR_FRAMES_COUNTER:
			{
				*error = VALUE_OK;			
				return ErCounter;
				
				break;
			}

			case BUS_OFF_COUNTER:
			{
				return 0;
				break;
			}

			case BUS_OFF_COUNTER_CAN2:
			{
				return 0;
				break;
			}

			case RX_COUNTER:
			{
				*error = VALUE_OK;
				return CounterRx;
				break;
			}

			case TX_COUNTER:
			{
				*error = VALUE_OK;
				return CounterEx;
				break;
			}

			case BUS_LOAD_PERCENT:
			{							
				*error = VALUE_OK;
				return 0;
				break;
			}

			case DOMINANT_HIGH_LEVEL:
			{
				*error = VALUE_OK;
				return Buf_Dominant_High_Level;
				break;
			}
			case DOMINANT_LOW_LEVEL:
			{
				*error = VALUE_OK;
				return Buf_Dominant_Low_Level;
				break;
			}
			case RECESSIF_HIGH_LEVEL:
			{
				*error = VALUE_OK;
				return Buf_Recessif_High_Level;
				break;
			}
			case RECESSIF_LOW_LEVEL:
			{
				*error = VALUE_OK;
				return Buf_Recessif_Low_Level;
				break;
			}
			case DOMINANT_ERROR_HIGH:
			{
				*error = VALUE_OK;
				return Flag_Dominant_High_error;
				break;
			}
			case DOMINANT_ERROR_LOW:
			{
				*error = VALUE_OK;
				return Flag_Dominant_Low_error;
				break;
			}
			case RECESSIF_ERROR_HIGH:
			{
				*error = VALUE_OK;
				return Flag_Recessif_High_error;
				break;
			}
			case RECESSIF_ERROR_LOW:
			{
				*error = VALUE_OK;
				return Flag_Recessif_Low_error;
				break;
			}
			case READ_CANUSB_EEPROM:
			{
				int tmp = ReadEEPROM(numero);
				if (tmp == NOT_OK)
					*error = UNUSABLE_VALUE;
				else
					*error = VALUE_OK;
				return tmp;
				break;
			}
			case TX_ERROR_COUNTER:
			{
				*error = VALUE_OK;
				return Buf_TxError;
				break;
			}
			case TX_ERROR_COUNTER_CAN2:
			{
				*error = VALUE_OK;
				return Buf_TxErrorCan2;
				break;
			}
			default:
			{
				*error = UNKNOW_COMMAND;
				return 0;
				break;
			}
		}

	}
}

int CCanBox2::ReadEEPROM(int NumByte)
{
	int wdata[16];

    wdata[0] = 0xB2;     // Command Byte for EEPROM read
    wdata[2] = NumByte;  // Offset in the EEPROM Area (0-103)
    wdata[3] = 1;        // Number of Datenbytes
                         // (8 max., more than 8 cause an Errorreply 0 !)
      
    if (!canInstruction(wdata)) 
	{
        return wdata[4];              
    }
	else
	{  
      return NOT_OK;  
    }
}

int CCanBox2::EffacerMessageReception(unsigned long Ident, int Ext, void (__cdecl *FonctionTraiteMessage)(messageStruct *))
{
	if(HARDWARE == 0)
	{
		int ret;
		messageStruct *MessageRech;

		CString tmp;
		tmp.Format("Effacement de 0x%X\n",Ident);
		//AfxMessageBox(tmp);
	//	fCanLog.WriteString(tmp);

		if (ListeMessages.Can[NumChannel-1].RetourneElement(Ident,&MessageRech) != NOT_OK )
		{
			ret = ListeMessages.Can[NumChannel-1].EffacerMessage(Ident,FonctionTraiteMessage);
		}
		else
		{
			ret = OK;
		}
		
		#if NIV_AFFICH_ERROR == 2
			if (ret == NOT_OK)	
				AfxMessageBox("EffacerMessageReception --> pas OK",MB_ICONERROR);
		#endif

		CString msg;
		errSIECA = dll_canIdDelete(hCan,Ident);
		if (errSIECA != NTCAN_SUCCESS)
		{
			msg.Format("Erreur dll_canIdDelete (%d)",errSIECA);
	//		AfxMessageBox(msg);
			return NOT_OK;
		}

		return ret;
	}

	if(HARDWARE == 1)
	{
		int ret;
		messageStruct *MessageRech;

		CString tmp;
		tmp.Format("Effacement de 0x%X\n",Ident);
		//AfxMessageBox(tmp);
	//	fCanLog.WriteString(tmp);

		if (ListeMessages.Can[NumChannel-1].RetourneElement(Ident,&MessageRech) != NOT_OK )
		{
			ret = ListeMessages.Can[NumChannel-1].EffacerMessage(Ident,FonctionTraiteMessage);
		}
		else
		{
			ret = OK;
		}
		
		#if NIV_AFFICH_ERROR == 2
			if (ret == NOT_OK)	
				AfxMessageBox("EffacerMessageReception --> pas OK",MB_ICONERROR);
		#endif



		errSIECA = xlCanRemoveAcceptanceRange(g_xlPortHandle, g_xlChannelMask, Ident, Ident);
		if(XL_SUCCESS != errSIECA)
		{
			tmp.Format("Erreur xlCanRemoveAcceptanceRange (%d)", errSIECA);
	//		AfxMessageBox(tmp);
			return NOT_OK;
		}

		return ret;
	}
}

CTRDATA CCanBox2::GetCounter(int *err)
{
	if(HARDWARE == 0)
	{
		errSIECA = dll_canGetCounter(&stCounter);
		*err = errSIECA;

		return stCounter;
	}

	if(HARDWARE == 1)
	{
		return stCounter;
	}
}

CTRDATA2 CCanBox2::GetCounterEx(int *err)
{
	if(HARDWARE == 0)
	{
		errSIECA = dll_canGetCounterExtended(hCan,&stCounter2);
		*err = errSIECA;
		
		
		return stCounter2;
	}
	if(HARDWARE == 1)
	{ 
		return stCounter2;
	}

}

BUSLOAD CCanBox2::GetBusInfos(int *err)
{
	if(HARDWARE == 0)
	{
		errSIECA = dll_canGetBusloadExtended(hCan,&stBusLoad);
		*err = errSIECA;
		return stBusLoad;
	}
	
	if(HARDWARE == 1)
	{
		return stBusLoad;
	}
}

CAN_IF_STATUS CCanBox2::GetBusInfos2(int *err)
{
	if(HARDWARE == 0)
	{
		errSIECA = dll_canGetDllInfo(hCan,&stCanStatus);
		*err = errSIECA;
		return stCanStatus;
	}

	if(HARDWARE == 1)
	{
		return stCanStatus;
	}
}

int CCanBox2::ClearBuffer()
{
	if(HARDWARE == 0)
	{
		dll_canClearBuffer(hCan);

		return OK;
	}

	if(HARDWARE == 1)
	{
		return OK;
	}

}

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

static void vRetrieveAndLog(DWORD /*dwErrorCode*/, char* /*File*/, int /*Line*/)
{
	USES_CONVERSION;

	char acErrText[MAX_PATH] = { '\0' };
	// Get the error text for the corresponding error code
	//if ((*pfCAN_GetErrText)(dwErrorCode, acErrText) == CAN_USB_OK)
	{
		sg_acErrStr = acErrText;
	}
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
					memset(sg_asClientToBufMap[unClientIndex].pacClientName, 0, sizeof(char) * MAX_PATH);
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
				memset(sg_asClientToBufMap[unClientIndex].pacClientName, 0, sizeof(char) * MAX_PATH);
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

#include <stdlib.h>

char *CCanBox2::convertSN(unsigned long sn)
{
	char	str[20];
	char	*serial;
	int		i;

	i = 0;
	if (!(serial = (char*)malloc(sizeof(char) * 5)))
		return (NULL);
	itoa(sn, str, 16);
	while (i < 4)
		serial[i] = str[i++];
	serial[i] = '\0';
	return(serial);
}

char	*CCanBox2::decriptSN(unsigned long sn1, unsigned long sn2)
{
	int		tmp;
	char	*tmp1;
	char	*tmp2;
	char	*serialnumber;
	char	*trash;
	int		i;
	int		len;

	i = 0;
	tmp1 = convertSN(sn1);
	tmp2 = convertSN(sn2);
	tmp = strtol(tmp2, &trash, 16);
	free(tmp2);
	if (!(tmp2 = (char*)malloc(sizeof(char) * 20)))
		return (NULL);
	itoa(tmp, tmp2, 10);
	len = strlen(tmp1) + strlen(tmp2) + 1;
	if (!(serialnumber = (char*)malloc(sizeof(char) * len)))
		return (NULL);
	while (i < len)
		serialnumber[i++] = '\0';
	strcat(serialnumber, tmp1);
	strcat(serialnumber, tmp2);
	free(tmp1);
	free(tmp2);
	return (serialnumber);
}


//Pour le debuggage
/*
void CCanBox2::lancerConsole()
{
    AllocConsole();
    SetConsoleTitle("Trace");
    // On redirige les flux d'I/O standard
    freopen("conin$", "r", stdin); 
    freopen("conout$", "w", stdout); 
    freopen("conout$", "w", stderr);
    return;
}
*/
/*
void CCanBox2::demoPrintConfig(void) {

  unsigned int i;
  char         str[XL_MAX_LENGTH + 1]="";

  printf("----------------------------------------------------------\n");
  printf("- %02d channels       Hardware Configuration               -\n", g_xlDrvConfig.channelCount);
  printf("----------------------------------------------------------\n");

  for (i=0; i < g_xlDrvConfig.channelCount; i++) {

    printf("- Ch:%02d, CM:0x%03I64x,", 
      g_xlDrvConfig.channel[i].channelIndex, g_xlDrvConfig.channel[i].channelMask);

    strncpy( str, g_xlDrvConfig.channel[i].name, 23);
    printf(" %23s,", str);

    memset(str, 0, sizeof(str));
    
    if (g_xlDrvConfig.channel[i].transceiverType != XL_TRANSCEIVER_TYPE_NONE) {
      strncpy( str, g_xlDrvConfig.channel[i].transceiverName, 13);
      printf("%13s -\n", str);
    }
    else printf("    no Cab!   -\n", str);
   
  
  }
  
  printf("----------------------------------------------------------\n\n");
 
}
*/


