// CanBox2.h: interface for the CCanBox2 class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_CANBOX2_H__DBCF2258_7C8C_4041_A855_760FB1F541CD__INCLUDED_)
#define AFX_CANBOX2_H__DBCF2258_7C8C_4041_A855_760FB1F541CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <sstream>
#include <string>
#include "SIECA132.h"
#include "CanUsbTypes.h"
#include "GestionMessages.h"
#include "vxlapi.h"
//raw path during developpment, need to be relative in the future release
#include "..\..\Kernel\BusmasterDriverInterface\Include\CANDriverDefines.h"
#include "..\..\Kernel\BusmasterDriverInterface\Include\DILPluginHelperDefs.h"
#include "..\..\Kernel\BusmasterDriverInterface\Include\DeviceListInfo.h"
#include "..\..\Kernel\BusmasterDriverInterface\Include\Error.h"
#include "..\..\Kernel\BusmasterDriverInterface\Include\BaseDIL_CAN_Controller.h"
#include "..\Utility\Utility_Thread.h"
#include "..\Utility\Utility_Thread.cpp"
#include "..\Utility\MultiLanguageSupport.h"

#include "windows.h"

#if defined USAGEMODE
#undef USAGEMODE
#endif
#if defined USAGE_EXPORT
#define USAGEMODE   __declspec(dllexport)
#else
#define USAGEMODE   __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C" {  // only need to export C interface if used by C++ source code
#endif

	/*  Exported function list */
	USAGEMODE HRESULT GetIDIL_CAN_Controller(void** ppvInterface);

#ifdef __cplusplus
}
#endif

//***************************************************************************************
//***************************************************************************************
//																						*
//	ACTIVATION DES OPTION à LA COMPILATION												*
//																						*
//		----------------------------------   											*
//	La variable NIV_AFFICH_ERROR permet de déterminer le niveau d'affichage des			*
//	messages d'erreur.																	*
//																						*
//		0	--> Pas d'affichage															*
//		1	-->	Niveau utilisateur														*
//		2	-->	Niveau débugage et évaluation											*
//																						*
#define NIV_AFFICH_ERROR 0															

// Activation du test des tensions du bus au démarrage de l'appli
#define Test_Tension	NOT_OK	


// Activation de la détection d'errorframe au démarrage de l'appli
#define Signal_ErrorFrame	NOT_OK

// Acquittement de transmission ( Mettre à ON si mesure tension )
// 1 --> OFF
// 0 --> ON
#define Ack	0

//Affichage des message box de DEBUG													*
#define DEBUG_MB	0
#define DEBUG_EXT	0
//																						*
//																						*
//***************************************************************************************
//***************************************************************************************



//***************************************************************************************
//***************************************************************************************
//*																						*
//*	DEFINE VXLAPI (CanCase)																*
//*																						*
//*																						*
#define RX_QUEUE_SIZE      4096           // internal driver queue size in CAN events   *
#define RECEIVE_EVENT_SIZE 1              // DO NOT EDIT! Currently 1 is supported only *
//*																						*
//*																						*
//***************************************************************************************
//***************************************************************************************



DWORD WINAPI ThreadDepilement(LPVOID lpParam);

typedef struct
{
	CGestionMessages Can[2];
}Liste;

#define DllExport	__declspec( dllexport )

class CCanBox2 : public CBaseDIL_CAN_Controller
{

	public:
		//	BUSMASTER NEEDED FUNCTION
		HRESULT			CAN_PerformInitOperations(void);
		HRESULT			CAN_PerformClosureOperations(void);
		HRESULT			CAN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER& QueryTickCount);
		HRESULT			CAN_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount, PSCONTROLLER_DETAILS InitData);
		HRESULT			CAN_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount);
		HRESULT			CAN_DeselectHwInterface(void);
		HRESULT			CAN_SetConfigData(PSCONTROLLER_DETAILS InitData, int Length);
		HRESULT			CAN_StartHardware(void);
		HRESULT			CAN_StopHardware(void);
		HRESULT			CAN_GetCurrStatus(STATUSMSG& StatusData);
		HRESULT			CAN_GetTxMsgBuffer(BYTE*& pouFlxTxMsgBuffer);
		HRESULT			CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& sCanTxMsg);
		HRESULT			CAN_GetBusConfigInfo(BYTE* BusInfo);
		HRESULT			CAN_GetLastErrorString(std::string& acErrorStr);
		HRESULT			CAN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam);
		HRESULT			CAN_SetControllerParams(int nValue, ECONTR_PARAM eContrparam);
		HRESULT			CAN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam);
		HRESULT			CAN_SetAppParams(HWND hWndOwner);
		HRESULT			CAN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj);
		HRESULT			CAN_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName);
		HRESULT			CAN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus);
		HRESULT			CAN_LoadDriverLibrary(void);
		HRESULT			CAN_UnloadDriverLibrary(void);
		HRESULT			CAN_SetHardwareChannel(PSCONTROLLER_DETAILS, DWORD dwDriverId, bool bIsHardwareListed, unsigned int unChannelCount);
		HRESULT			GetHWinfo(unsigned long *pulSNHigh, unsigned long *pulSNLow, int *res);

		//  IOCANBOX FUNCTION
		int				ClearBuffer(void);
		int				Initialisation();
		int				Initialisation(int NumVoie,unsigned long vitesse,unsigned char echantillonnage);
		int				Initialisation(int NumVoie,int Echo,unsigned long vitesse,unsigned char echantillonnage);
		int				Initialisation(int NumVoie,int Echo,unsigned long vitesse,unsigned char echantillonnage, int HardwareChoosed);
		int				ActiveCanBox(void);
		int				DesactiveCanBox(void);
		int				AjouterMessageEmission(HANDLE channel, unsigned long Ident,int Ext,unsigned char Remote,unsigned char *data,unsigned char Dlc);
		int				AjouterMessageReception(unsigned long Ident, unsigned long etendu,unsigned long mask,void (* FonctionTraiteMessage)(messageStruct *message));
		int				EffacerMessageReception(unsigned long Ident,int Ext,void (* FonctionTraiteMessage)(messageStruct *message));
		int				Lire_Donnee(int numero, int *error);
		int				Ecrire_Donnee(int numero, int valeur);
		int				AppliqueFiltre(int etat);
		void			TraiteCanBox(int NumeroChannel);
		void			SendMessage(CString msg);
		T_DeviceList	*GetHardwareList(void);
		void			can_reader(void);
		void			can_reader2(void);
		void			ProcessCANMsg(CMSG canmsg, unsigned int nChannelIndex);
		void			ProcessCANtx(STCAN_MSG canmsg);
		char			*convertSN(unsigned long sn);
		char			*decriptSN(unsigned long sn1, unsigned long sn2);

		CCanBox2();
		virtual			~CCanBox2();

		//Variables
		int				SwitchCan = 0;
		int				NumChannel = 0;
		HANDLE			hThread;
		HANDLE			hThread2;
		HANDLE			d_eventStop;
		HANDLE			d_eventStop2;
		CStdioFile		rapport;
		int				bLogFile;
		unsigned long	ulNum;
		//Variables pour SIECA
		bool			FlagFinThread;
		bool			bStopThread;
		int				hardware;
		char			*g_serialnumber;
		long			oldtstamp = 0;


	private:
		CAN_IF_STATUS	GetBusInfos2(int *err);		//CanBox	public
		BUSLOAD			GetBusInfos(int *err);		//CanBox	public
		CTRDATA			GetCounter(int *err);		//CanBox	public
		CTRDATA2		GetCounterEx(int *err);		//CanBox	public
		int				canInstruction(int *arrData);	//CanBox	public
		//Variables pour SIECA
		HANDLE			hCan;			//CanBox	public
		HANDLE			hCan2;			//CanBox	public
		CAN_IF_STATUS	stCanStatus;	//CanBox	public
		CTRDATA			stCounter;		//CanBox	public
		CTRDATA2		stCounter2;		//CanBox	public
		BUSLOAD			stBusLoad;		//CanBox	public



	private:
		ULONG			ReadVersionCanUsb			(void);
		int				SetErrorFrameDetection		(unsigned int bitrate, int channel);
		int				MesureNiveauCan				(void);
		int				ReadEEPROM					(int NumByte);
		int				WriteEEPROM					(int NumByte,int value);
		int				ConfigureFiltre(unsigned long Ident,unsigned long masque);



										/****************************************************/
										/*													*/
										/*			 Fonctions SIECA132 (CanBox)			*/
										/*													*/
										/****************************************************/
		//GetProcAddress de SIECA132
		LPFNDLLcanOpen								dll_canOpen;
		LPFNDLLcanSetBaudrate						dll_canSetBaudrate;
		LPFNDLLcanSend								dll_canSend;
		LPFNDLLcanWrite								dll_canWrite;
		LPFNDLLcanRead								dll_canRead;
		LPFNDLLcanIdAdd								dll_canIdAdd;
		LPFNDLLcanIdDelete							dll_canIdDelete; 
		LPFNDLLcanIdAddArray						dll_canIdAddArray;
		LPFNDLLcanIdDeleteArray						dll_canIdDeleteArray;
		LPFNDLLcanEnableAllIds						dll_canEnableAllIds;
		LPFNDLLcanAreAllIdsEnabled					dll_canAreAllIdsEnabled;
		LPFNDLLcanIDStatus							dll_canIDStatus;
		LPFNDLLcanBreakcanRead						dll_canBreakCanRead;
		LPFNDLLcanSetIdOffset						dll_canSetIdOffset;
		LPFNDLLcanGetIdOffset						dll_canGetIdOffset;
		LPFNDLLcanStatus							dll_canStatus;
		LPFNDLLcanGetCounter						dll_canGetCounter;
		LPFNDLLcanGetCounterExtended				dll_canGetCounterExtended;
		LPFNDLLcanGetBusloadExtended				dll_canGetBusloadExtended;
		LPFNDLLcanClose								dll_canClose;
		LPFNDLLcanGetTimeout						dll_canGetTimeout;
		LPFNDLLcanSetTimeout						dll_canSetTimeout;
		LPFNDLLcanSetTxDelay						dll_canSetTxDelay;
		LPFNDLLcanIsVirtualHw						dll_canIsVirtualHw;
		LPFNDLLcanGetDllInfo						dll_canGetDllInfo;
		LPFNDLLcanIsNetOwner						dll_canIsNetOwner;
		LPFNDLLcanInstruction						dll_canInstruction;
		LPFNDLLcanGetSyncTimer						dll_canGetSyncTimer;
		LPFNDLLcanSetOwner							dll_canSetOwner;
		LPFNDLLcanGetOwner							dll_canGetOwner;
		LPFNDLLcanSetFilterMode						dll_canSetFilterMode;
		LPFNDLLcanGetFilterMode						dll_canGetFilterMode;
		LPFNDLLcanIsHandleValid						dll_canIsHandleValid;
		LPFNDLLcanSetFilterJ2534					dll_canSetFilterJ2534;
		LPFNDLLcanDeleteFilterJ2534					dll_canDeleteFilterJ2534;
		LPFNDLLcanSetFilterJ2534_2					dll_canSetFilterJ2534_2;
		LPFNDLLcanDeleteFilterJ2534_2				dll_canDeleteFilterJ2534_2;
		LPFNDLLcanClearBuffer						dll_canClearBuffer;
		LPFNDLLcanResetCounter						dll_canResetCounter;
		LPFNDLLcanGetTime							dll_canGetTime;
		LPFNDLLcanGetNumberOfConnectedDevices		dll_canGetNumberOfConnectedDevices;
		LPFNDLLcanGetDeviceList						dll_canGetDeviceList;
		LPFNDLLcanGetDeviceTimestampBase			dll_canGetDeviceTimestampBase;
		LPFNDLLcanGetEepromAccess					dll_canGetEepromAccess;
		LPFNDLLcanReadEeprom						dll_canReadEeprom;
		LPFNDLLcanWriteEeprom						dll_canWriteEeprom;
		LPFNDLLcanCloseEepromAccess					dll_canCloseEepromAccess;
		LPFNDLLcanFlashDevice						dll_canFlashDevice;
		LPFNDLLcanSetHWFilterEmu					dll_canSetHWFilterEmu;
		LPFNDLLcanEnableBusloadFiltered				dll_canEnableBusloadFilter;
		LPFNDLLcanEnableHWExtendedId				dll_canEnableHWExtendedId;
		LPFNDLLcanGetBusloadFiltered				dll_canGetBusloadFiltered;
		LPFNDLLcanGetCanLevel						dll_canGetCanLevel;
		LPFNDLLcanSetTxTimeout						dll_canSetTxTimeout;
		LPFNDLLSetPrioritySIECE132					dll_canSetPrioritySICE132;
		LPFNDLLGetPrioritySIECE132					dll_canGetPrioritySICE132;
		LPFNDLLcreateCustomSharedMemory				dll_createCustomSharedMemory;
		LPFNDLLcanSetBaudrateForce					dll_canSetBaudrateForce;
		LPFNDLLcanGetDiffTimeLastFrame				dll_canGetDiffTimeLastFrame;
		LPFNDLLcanGetHWSerialNumber					dll_canGetHWSerialNumber;
		LPFNDLLcanConfirmedTransmit					dll_canConfirmedTransmit;
		LPFNDLLcanReadNoWait						dll_canReadNoWait;



										/****************************************************/
										/*													*/
										/*			 Variable VXLAPI (CanCase)				*/
										/*													*/
										/****************************************************/


	private:
		char            g_AppName[XL_MAX_LENGTH+1];    //Application name which is displayed in VHWconf
		XLportHandle    g_xlPortHandle;                //Global porthandle (we use only one!)
		XLdriverConfig  g_xlDrvConfig;                 //Contains the actual hardware configuration
		XLaccess        g_xlChannelMask;               //Global channelmask (includes all founded channels)
		XLaccess        g_xlPermissionMask;            //Global permissionmask (includes all founded channels)


		
		int			CounterRx;
		int			CounterEx;
		int			ErCounter;		


		// tread variables
		XLhandle        g_hMsgEvent;                    //notification handle for the receive queue

		


		//Pour le debuggage
		//void lancerConsole();	//Permet d'écrire sur la console
		//void demoPrintConfig();	//Affiche les paramétres du matériel (CanCase)
		
};





#endif // !defined(AFX_CANBOX2_H__DBCF2258_7C8C_4041_A855_760FB1F541CD__INCLUDED_)
