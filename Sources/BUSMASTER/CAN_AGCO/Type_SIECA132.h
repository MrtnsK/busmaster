//Baudrate
#define	CAN_1000K		0
#define	CAN_800K		1
#define	CAN_500K		2
#define	CAN_250K		3
#define	CAN_125K		4
#define	CAN_100K		5
#define	CAN_50K			6
#define	CAN_20K			7





typedef struct
{
	long			l_id;
	unsigned char	by_len;
	unsigned char	 by_msg_lost;
	unsigned char	by_extended;
	unsigned char	by_remote;
	unsigned char	aby_data[8];
	unsigned long	ul_tstamp;
} CMSG;

typedef struct
{
	unsigned char	aby_ID[2048];
} T_ID_ARRAY;

typedef struct
{
	unsigned short	w_hw_rev;
	unsigned short	w_fw_rev;
	unsigned short	w_drv_rev;
	unsigned short	w_dll_rev;
	unsigned long	ul_board_status;
	unsigned char	by_board_id;
	unsigned short	w_busoffctr;
	unsigned short	w_errorflag;
	unsigned short	w_errorframectr;
	unsigned short	w_netctr;
	unsigned short	w_baud;
	unsigned int	ui_epld_rev;
} CAN_IF_STATUS;

/*
typedef struct st_InternalDLLInformation 
{
	unsigned int	aui_TxCounter[2];
	unsigned int	aui_TxHandleCounter[MAX_NUM_APIHANDLE];
	unsigned int	aui_TxCounterRTR[2];
	unsigned int	aui_TxHandleCounterRTR[MAX_NUM_APIHANDLE];
	unsigned int	aui_TxThreadCounter[2];
	unsigned int	aui_TxThreadCounterRTR[2];
	unsigned int	aui_RxCounter[MAX_NUM_APIHANDLE];
	unsigned int	aui_RxThreadCounter[2];
	unsigned int	aui_RxBufferCounter[MAX_NUM_APIHANDLE];
	unsigned int	aui_InterfaceCtr[MAX_NUM_APIHANDLE];
	T_STRINGARRAY	appNames;
	unsigned int	aui_ThreadStatus[MAX_NUM_APIHANDLE];
	unsigned int	aui_ZugriffsCounterRead[MAX_NUM_APIHANDLE];
	unsigned int	aui_ZugriffsCounterWrite[MAX_NUM_APIHANDLE];
	unsigned int	aui_HandleNr[MAX_NUM_APIHANDLE];
	unsigned int	aui_NetzZuordnung[MAX_NUM_APIHANDLE];
	unsigned int	aui_NetzOwner[2];
	unsigned int	aui_Reserve1601[MAX_NUM_APIHANDLE];
	unsigned int	aui_Reserve1602[MAX_NUM_APIHANDLE];
	unsigned int	aui_Reserve1603[MAX_NUM_APIHANDLE];
	unsigned int	aui_Reserve1604[MAX_NUM_APIHANDLE];
	unsigned int	ui_NetCount;
	unsigned int	ui_Reserve;
	unsigned int	aui_AnzahlEchoFrames[2];
	unsigned int	ui_CloseZaehler;
	unsigned int	ui_OpenZaehler;
	unsigned int	ui_CloseFlag;
	unsigned int	ui_OpenedHandles;
} T_DllInfo;
*/

typedef struct
{
	unsigned long	ul_timer;
	unsigned long	ul_rxframectr1;
	unsigned long	ul_txframectr1;
	unsigned long	ul_txremframectr1;
	unsigned long	ul_txdatabytectr1;
	unsigned long	ul_rxdatabytectr1;
	unsigned long	reserved0;
	unsigned long	ul_rxframectr2;
	unsigned long	ul_txframectr2;
	unsigned long	ul_txremframectr2;
	unsigned long	ul_txdatabytectr2;
	unsigned long	ul_rxdatabytectr2;
	unsigned long	ul_CAN1ctrlflags;
	unsigned long	ul_CAN2ctrlflags;
	unsigned long	ul_errframectr1;
	unsigned long	ul_errframectr2;
} CTRDATA;

typedef struct
{
    unsigned long	ul_timer;			// 0 free running timer resolution 100 us
    unsigned long	ul_rxframectr1;		// 1 count of received frames 
    unsigned long	ul_txframectr1;		// 2 count of transmitted frames  
    unsigned long	ul_txremframectr1;	// 3 count of transmitted remote frames
    unsigned long	ul_txdatabytectr1;	// 4 count of transmitted data bytes
    unsigned long	ul_rxdatabytectr1;	// 5 count of received data bytes
    unsigned long	reserved0;			// 6 reserved for further use
    unsigned long	ul_rxframectr2;		// 7 free running timer resolution 100 us
    unsigned long	ul_txframectr2;		// 8 count of received frames 
    unsigned long	ul_txremframectr2;	// 9 count of transmitted frames  
    unsigned long	ul_txdatabytectr2;	// 10 count of transmitted remote frames
    unsigned long	ul_rxdatabytectr2;	// 11 count of transmitted data bytes
    unsigned long	ul_CAN1ctrlflags;	// 12 Statusflags of Full-CAN-Controller 1
    unsigned long	ul_CAN2ctrlflags;	// 13 Statusflags of Full-CAN-Controller 1
    unsigned long	ul_errctr1;			// 14 Errorframe counter CAN 1
    unsigned long	ul_errctr2;			// 15 Errorframe counter CAN 2
    unsigned long	ul_rxremframectr1;	// 16 rx Remote frame counter CAN 1
    unsigned long	ul_erxframectr1;	// 17 count of received frames 
    unsigned long	ul_etxframectr1;	// 18 count of transmitted frames  
    unsigned long	ul_etxremframectr1;	// 19 count of transmitted remote frames
    unsigned long	ul_erxremframectr1;	// 20
    unsigned long	ul_rxremframectr2;	// 21 rx Remote frame counter CAN 2
    unsigned long	ul_erxframectr2;	// 22 free running timer resolution 100 us
    unsigned long	ul_etxframectr2;	// 23 count of received frames 
    unsigned long	ul_etxremframectr2;	// 24 count of transmitted frames  
    unsigned long	ul_erxremframectr2;	// 25 extended remote frame counter CAN 2
}CTRDATA2;

typedef struct
{
    unsigned long ul_intervall;			// intervall of the bus load calculation in msec
    unsigned long ul_stdframectr;		// frame counter of standard frames
    unsigned long ul_stdframebytectr;	// byte counter of standard frames
    unsigned long ul_extframectr;		// frame counter of extended frames
    unsigned long ul_extframebytectr;	// byte counter of extended frames
    double ObjectTime;
    unsigned long ul_load;				// busload in 1/100
}BUSLOAD;

typedef enum e_canLevelMode
{
	canLevelDominant = 0,
	canLevelRecessive = 1
} T_CANLEVEL_MODE;

typedef enum e_FilterMode
{
	filterMode_standard = 0,
	filterMode_j2534 = 1,
	filterMode_extended = 2,
	filterMode_j2534_2 = 3,
	filterMode_nofilter = 4
} T_FILTER_MODE;

typedef enum e_J2534Mode
{
	j2534Mode_excl = 0,
	j2534Mode_incl = 1
} T_J2534_MODE;



//Error Codes

#define NTCAN_NO_ERROR							0
#define NTCAN_RX_TIMEOUT						-1
#define	NTCAN_TX_TIMEOUT						-2
#define	NTCAN_NO_ID_ENABLED						-4
#define	NTCAN_ID_ALREADY_ENABLED				-5
#define	NTCAN_ID_NOT_ENABLED					-6
#define NTCAN_INVALID_PARAMETER					-7
#define NTCAN_INVALID_HANDLE					-8
#define NTCAN_TOO_MANY_HANDLES					-9
#define NTCAN_INIT_ERROR						-10
#define NTCAN_RESET_ERROR						-11
#define NTCAN_DRIVER_ERROR						-12
#define NTCAN_DLL_ALREADY_INIT					-13
#define	NTCAN_CHANNEL_NOT_INITIALIZED			-14
#define	NTCAN_TX_ERROR							-15
#define	NTCAN_INVALID_NETNUMBER					-18
#define	NTCAN_TOO_MANY_J2534_RANGES				-19
#define	NTCAN_TOO_MANY_J2534_2_FILTERS			-20
#define NTCAN_FIRMWARE_UNSUPPORTED				-24
#define	NTCAN_GET_MUTEX_FAILED					-104
#define	NTCAN_NOSUCCESS							0xFFFF0000