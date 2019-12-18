//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// SIECA132.h
//
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
#ifndef AFX_SIECA132_H_INCLUDED_
    #define AFX_SIECA132_H_INCLUDED_

    #ifdef __cplusplus
extern "C"
{
    #endif
    #ifdef SIECA132_EXPORTS
        #define SIECA132_API    __declspec(dllexport)
    #else
        #define SIECA132_API    __declspec(dllimport)
    #endif

    //#pragma comment(lib, "SIECA132")
    // ################################################################################################
    // ------------------------------------------------------------------------------------------------
    // ErrorFlag - definitions
    #define RBUFOVERFLOW    0x01
    #define HWRBUFOVERFLOW  0x02
    #define BUSOFF          0x04
    #define ERRORPASSIVE    0x08
    #define TXERROR         0x10
    #define USBDATAERROR    0x20

    // ------------------------------------------------------------------------------------------------
    // token for hardware type
    // ------------------------------------------------------------------------------------------------
    #define HWPOWERPCI2NET  8
    #define HWPOWERPCI3NET  9
    #define HWCANAS         2

    // ------------------------------------------------------------------------------------------------
    #define BASEHANDLE              0xF800

    #define MAX_NUM_APIHANDLE       0x0200
    #define MAX_NUM_ACTIVE_HANDLES  0x0200  // must be less or equate MAX_NUM_APIHANDLE !
    #define MAX_NUM_J2534_RANGES    10
    #define MAX_NUM_J2534_2_FILTERS 10

    #define EXTFRAME_ONOFF          0xFF01

    // ------------------------------------------------------------------------------------------------

    // canMessage structure

    // ------------------------------------------------------------------------------------------------
    typedef struct st_canmsg
    {
        long l_id;
        unsigned char by_len;
        unsigned char by_msg_lost;
        unsigned char by_extended;
        unsigned char by_remote;
        unsigned char aby_data[8];
        unsigned long ul_tstamp;
    } CMSG;

    typedef struct st_string
    {
        char s[256];
    } T_STRING;

    typedef struct st_stringarray
    {
        T_STRING ac_applicationnames[MAX_NUM_APIHANDLE];
    } T_STRINGARRAY;

    typedef struct st_id_array
    {
        unsigned char aby_IDField[2048];
    } T_ID_ARRAY;

    typedef struct st_can_status
    {
        unsigned short w_hw_rev;        // hardware revision number (0..0xffff)
        unsigned short w_fw_rev;        // firmware revision number (0..0xffff)
        unsigned short w_drv_rev;       // software driver revision number (0..0xffff)
        unsigned short w_dll_rev;       // DLL revision number (0..0xffff)
        unsigned long ul_board_status;  // error status of the board ( 0 -> ok, != 0 -> error)
        unsigned char by_board_id;      // name of the board
        unsigned short w_busoffctr;     // busoff counter
        unsigned short w_errorflag;     // actual error state (error passiv, error activ, error occurance)
        unsigned short w_errorframectr; // number of errorframes counted on the bus
        unsigned short w_netctr;        // number of physical existing nets
        long l_baud;                    // baudrate
        unsigned int ui_epld_rev;       // EPLD revision number
    } CAN_IF_STATUS;

    // ------------------------------------------------------------------------------------------------
    typedef struct st_counterdata
    {
        unsigned long ul_timer;             // free running timer resolution 100 us
        unsigned long ul_rxframectr1;       // count of received frames
        unsigned long ul_txframectr1;       // count of transmitted frames
        unsigned long ul_txremframectr1;    // count of transmitted remote frames
        unsigned long ul_txdatabytectr1;    // count of transmitted data bytes
        unsigned long ul_rxdatabytectr1;    // count of received data bytes
        unsigned long reserved0;            // reserved for further use
        unsigned long ul_rxframectr2;       // count of received frames
        unsigned long ul_txframectr2;       // count of transmitted frames
        unsigned long ul_txremframectr2;    // count of transmitted remote frames
        unsigned long ul_txdatabytectr2;    // count of transmitted data bytes
        unsigned long ul_rxdatabytectr2;    // count of received data bytes
        unsigned long ul_CAN1ctrlflags;     // Statusflags of Full-CAN-Controller 0
        unsigned long ul_CAN2ctrlflags;     // Statusflags of Full-CAN-Controller 1
        unsigned long ul_errframectr1;      // Number of error frames (Channel 0)
        unsigned long ul_errframectr2;      // Number of error frames (Channel 1)
    } CTRDATA;

    typedef struct st_counterdata2
    {
        unsigned long ul_timer;             // 0		free running timer resolution 100 us
        unsigned long ul_rxframectr1;       // 1		count of received frames
        unsigned long ul_txframectr1;       // 2		count of transmitted frames
        unsigned long ul_txremframectr1;    // 3		count of transmitted remote frames
        unsigned long ul_txdatabytectr1;    // 4		count of transmitted data bytes
        unsigned long ul_rxdatabytectr1;    // 5		count of received data bytes
        unsigned long reserved0;            // 6		reserved for further use
        unsigned long ul_rxframectr2;       // 7		free running timer resolution 100 us
        unsigned long ul_txframectr2;       // 8		count of received frames
        unsigned long ul_txremframectr2;    // 9		count of transmitted frames
        unsigned long ul_txdatabytectr2;    // 10		count of transmitted remote frames
        unsigned long ul_rxdatabytectr2;    // 11		count of transmitted data bytes
        unsigned long ul_CAN1ctrlflags;     // 12		Statusflags of Full-CAN-Controller 1
        unsigned long ul_CAN2ctrlflags;     // 13		Statusflags of Full-CAN-Controller 1
        unsigned long ul_errctr1;           // 14		Errorframe counter CAN 1
        unsigned long ul_errctr2;           // 15		Errorframe counter CAN 2
        unsigned long ul_rxremframectr1;    // 16		rx Remote frame counter CAN 1
        unsigned long ul_erxframectr1;      // 17		count of received frames
        unsigned long ul_etxframectr1;      // 18		count of transmitted frames
        unsigned long ul_etxremframectr1;   // 19		count of transmitted remote frames
        unsigned long ul_erxremframectr1;   // 20		count of received remote frames
        unsigned long ul_rxremframectr2;    // 21		rx Remote frame counter CAN 2
        unsigned long ul_erxframectr2;      // 22		free running timer resolution 100 us
        unsigned long ul_etxframectr2;      // 23		count of received frames
        unsigned long ul_etxremframectr2;   // 24		count of transmitted frames
        unsigned long ul_erxremframectr2;   // 25       extended remote frame counter CAN 2
    } CTRDATA2;

    typedef enum e_TimeoutType { timeoutType_rx, timeoutType_tx } T_TIMEOUT_TYPE;

    typedef struct st_busload
    {
        unsigned long ul_intervall;         // intervall of the bus load calculation in msec
        unsigned long ul_stdframectr;       // frame counter of standard frames
        unsigned long ul_stdframebytectr;   // byte counter of standard frames
        unsigned long ul_extframectr;       // frame counter of extended frames
        unsigned long ul_extframebytectr;   // byte counter of extended frames
        double ObjectTime;
        unsigned long ul_load;              // busload in 1/100
    } BUSLOAD;

    typedef enum e_J2534Mode { j2534Mode_excl = 0, j2534Mode_incl = 1 } T_J2534_MODE;

    typedef enum e_FilterMode
    {
        filterMode_standard = 0,
        filterMode_j2534    = 1,
        filterMode_extended = 2,
        filterMode_j2534_2  = 3,
        filterMode_nofilter = 4
    } T_FILTER_MODE;

    typedef enum e_filteredBusload
    {
        filteredBusload_off         = 0,
        filteredBusload_hw_filter   = 1,
        filteredBusload_hw_sw_filter= 2
    } T_FILTERED_BUSLOAD;

    typedef enum e_canLevelMode { canLevelDominant = 0, canLevelRecessive = 1 } T_CANLEVEL_MODE;

    typedef struct st_j2534filter
    {
        unsigned long ul_rangestart;        // start of range
        unsigned long ul_rangestop;         // stop of range
        T_J2534_MODE t_mode;                // j2534-Mode (include/exclude)
    } J2534FILTER;

    typedef struct st_j2534_2filter
    {
        unsigned long ul_filter_id;         //
        unsigned long ul_mask_id;           // id mask
        unsigned __int64 ul_mask_data;      // data mask
        unsigned long ul_pattern_id;        // id pattern
        unsigned __int64 ul_pattern_data;   // data pattern
        T_J2534_MODE t_mode;                // j2534-Mode (include/exclude)
    } J2534_2FILTER;

    typedef struct st_emuHWFilter
    {
        unsigned long ul_filterMode;
        unsigned long arr_fullCanMask[16];
    } EMU_HWFILTER;

    typedef enum e_StateCusShaMemHolder
    {
        sharedmem_empty             = 0,
        sharedmem_request           = 1,
        sharedmem_available         = 2
    } T_STATE_CUS_SHAMEM_HOLDER;

    // MG:2005-04-15 defines for T_DeviceList structure:
    #define CANSTATUS_READY                         0x00000001
    #define CANSTATUS_WRONG_FIRMWARE_VERSION        0x00000002
    #define CANSTATUS_WRONG_DRIVER_VERSION          0x00000004

    #define CANFEATURES_ERROR_FRAME_DETECTION       0x00000001
    #define CANFEATURES_SEND_REMOTE_FRAME           0x00000002
    #define CANFEATURES_RECEIVE_REMOTE_FRAME        0x00000004
    #define CANFEATURES_HIGH_LOW_SPEED_SELECTABLE   0x00000008
    #define CANFEATURES_LISTEN_MODE                 0x00000010  // no acknowledge for CAN messages
    typedef struct
    {
        int Net;
        char Name[20];
        unsigned long ul_Status;        // MG:2005-04-15
        unsigned long ul_Features;      // MG:2005-04-15
        long Reserved[18];
    } T_DeviceList;

    typedef struct st_InternalDLLInformation
    {
        // ------------------------------------------------------------
        // cansend variable
        // variable von cansend zum buffer
        unsigned int aui_TxCounter[2];  // soviel daten wurden in buffer geschrieben
        unsigned int aui_TxHandleCounter[MAX_NUM_APIHANDLE];        // soviel daten wurden vom entspr. Handle in Puffer geschr.
        unsigned int aui_TxCounterRTR[2];                           // soviel daten wurden in buffer geschrieben
        unsigned int aui_TxHandleCounterRTR[MAX_NUM_APIHANDLE];     // soviel daten wurden vom entspr. Handle in Puffer geschr.

        // variable vom buffer zur CPCI
        unsigned int aui_TxThreadCounter[2];                        // soviel daten wurden ins DPRAM geschrieben
        unsigned int aui_TxThreadCounterRTR[2];                     // soviel RTR daten wurden ins DPRAM geschrieben

        // -------------------------------------------------------------
        // read variable
        unsigned int aui_RxCounter[MAX_NUM_APIHANDLE];              // soviel daten wurden von applik. aus puffer gelesen
        unsigned int aui_RxThreadCounter[2];                        // soviel messages wurden empfangen
        unsigned int aui_RxBufferCounter[MAX_NUM_APIHANDLE];        // soviel daten wurden in den puffer geschrieben
        unsigned int aui_InterfaceCtr[MAX_NUM_APIHANDLE];
        T_STRINGARRAY appNames;

        // -------------------------------------------------------------
        unsigned int aui_ThreadStatus[MAX_NUM_APIHANDLE];           // definiert ob der HW-Thread läuft oder nicht
        unsigned int aui_ZugriffsCounterRead[MAX_NUM_APIHANDLE];    // zählt RD Applikationszugriffe
        unsigned int aui_ZugriffsCounterWrite[MAX_NUM_APIHANDLE];   // zählt WR Applikationszugriffe

        // -------------------------------------------------------------
        unsigned int aui_HandleNr[MAX_NUM_APIHANDLE];               // zugeordnete Handle-Nummer
        unsigned int aui_NetzZuordnung[MAX_NUM_APIHANDLE];          // welcher Handle benutzt welches Netz
        unsigned int aui_NetzOwner[2];                  // welcher Handle besitzt welches Netz

        // -------------------------------------------------------------
        unsigned int aui_Reserve1601[MAX_NUM_APIHANDLE];
        unsigned int aui_Reserve1602[MAX_NUM_APIHANDLE];
        unsigned int aui_Reserve1603[MAX_NUM_APIHANDLE];
        unsigned int aui_Reserve1604[MAX_NUM_APIHANDLE];

        unsigned int ui_NetCount;
        unsigned int ui_Reserve;
        unsigned int aui_AnzahlEchoFrames[2];
        unsigned int ui_CloseZaehler;
        unsigned int ui_OpenZaehler;
        unsigned int ui_CloseFlag;
        unsigned int ui_OpenedHandles;
    } T_DllInfo;

    #define NTCAN_SUCCESS                       (0)
    #define NTCAN_RX_TIMEOUT                    (-1)
    #define NTCAN_TX_TIMEOUT                    (-2)
    #define NTCAN_CONTR_BUSOFF                  (-3)
    #define NTCAN_NO_ID_ENABLED                 (-4)
    #define NTCAN_ID_ALREADY_ENABLED            (-5)
    #define NTCAN_ID_NOT_ENABLED                (-6)
    #define NTCAN_INVALID_PARAMETER             (-7)
    #define NTCAN_INVALID_HANDLE                (-8)
    #define NTCAN_TOO_MANY_HANDLES              (-9)
    #define NTCAN_INIT_ERROR                    (-10)
    #define NTCAN_RESET_ERROR                   (-11)
    #define NTCAN_DRIVER_ERROR                  (-12)
    #define NTCAN_DLL_ALREADY_INIT              (-13)
    #define NTCAN_CHANNEL_NOT_INITIALIZED       (-14)
    #define NTCAN_TX_ERROR                      (-15)
    #define NTCAN_NO_SHAREDMEMORY               (-16)
    #define NTCAN_HARDWARE_NOT_FOUND            (-17)
    #define NTCAN_INVALID_NETNUMBER             (-18)
    #define NTCAN_TOO_MANY_J2534_RANGES         (-19)
    #define NTCAN_TOO_MANY_J2534_2_FILTERS      (-20)
    #define NTCAN_DRIVER_NOT_INSTALLED          (-21)
    #define NTCAN_NO_OWNER_RIGHTS               (-22)
    #define NTCAN_FIRMWARE_TOO_OLD              (-23)
    #define NTCAN_FIRMWARE_UNSUPPORTED          (-24)
    #define NTCAN_FIRMWAREUPDATE_FAILED         (-25)
    #define NTCAN_HARDWARE_NOT_SUPPORTED        (-26)
    #define NTCAN_FILE_NOT_FOUND                (-27)
    #define NTCAN_DEVICE_INFO_NOTAVAILABLE      (-100)
    #define NTCAN_DEVICE_NOHW_ADDRESS           (-101)
    #define NTCAN_NO_INTERRUPT_EVENT            (-102)  // can't create an interrupt event
    #define NTCAN_NO_INTERRUPT_EVENT_SET        (-103)  // can't set the interrupt event in the sys
    #define NTCAN_GET_MUTEX_FAILED              (-104)  // can't get the mutex
    #define NTCAN_NO_SHARED_MEMORY              (-105)
    #define NTCAN_NET_NOT_AVAILABLE             (-106)
    #define NTCAN_SETBAUDRATE_TIMEOUT           (-107)
    #define NTCAN_EXE_ALREADYSTARTED            (-108)
    #define NTCAN_NOTABLE_TOCREATE_SHAREDMEMORY (-109)
    #define NTCAN_HARDWARE_IN_USE               (-110)
    #define NTCAN_API_NOT_RUNNING               (-111)
    #define NTCAN_HARDWARENOTACTIVE             (-200)
    #define NTCAN_NOSUCCESS                     0xFFFF0000

    #define BAUD_1000KBIT                       0
    #define BAUD_800KBIT                        1
    #define BAUD_500KBIT                        2
    #define BAUD_250KBIT                        3
    #define BAUD_125KBIT                        4
    #define BAUD_100KBIT                        5
    #define BAUD_50KBIT                         6
    #define BAUD_20KBIT                         7
    #define BAUD_USERDEFINED                    0x80000000
    #define BAUD_SILENTMODE                     0x40000000
    #define BAUD_LOWSPEED                       0x20000000

    //-----------------------------------------------------------------------------------
    // The following declarations includes the prototype for the interface function and
    // the typedefinition of the function pointer to make easier if you want to
    // dynamically load the dll. (LoadLibrary, GetProcAddress)
    //-----------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------
    // canOpen
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanOpen)
        (
            long l_netnumber,
            long l_mode,
            long l_echoon,
            long l_txtimeout,
            long l_rxtimeout,
            const char*c_ApplicationName,
            const char*c_ReceiverEvent,
            const char*c_ErrorEvent,
            void**handle
        );

    extern long SIECA132_API __stdcall canOpen(long l_netnumber,
                                               long l_mode,
                                               long l_echoon,
                                               long l_txtimeout,
                                               long l_rxtimeout,
                                               const char* c_ApplicationName,
                                               const char* c_ReceiverEvent,
                                               const char* c_ErrorEvent,
                                               void ** handle);

    //-----------------------------------------------------------------------------------
    // canSetBaudrate
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanSetBaudrate) (void*handle, long l_baud);

    extern long SIECA132_API __stdcall canSetBaudrate(void* handle, long l_baud);

    //-----------------------------------------------------------------------------------
    // canSend
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanSend) (void*handle, CMSG*t_canmsg, long*l_len);

    extern long SIECA132_API __stdcall canSend(void* handle, CMSG* t_canmsg, long* l_len);

    //-----------------------------------------------------------------------------------
    // canWrite
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanWrite) (void*handle, CMSG*t_canmsg, long*l_len);

    extern long SIECA132_API __stdcall canWrite(void* handle, CMSG* t_canmsg, long* l_len);

    //-----------------------------------------------------------------------------------
    // canRead
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanRead) (void*handle, CMSG*t_canmsg, long*l_len);

    extern long SIECA132_API __stdcall canRead(void* handle, CMSG* t_canmsg, long* l_len);

    //-----------------------------------------------------------------------------------
    // canIdAdd
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanIdAdd) (void*handle, long l_id);

    extern long SIECA132_API __stdcall canIdAdd(void* handle, long l_id);

    //-----------------------------------------------------------------------------------
    // canIdDelete
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanIdDelete) (void*handle, long l_id);

    extern long SIECA132_API __stdcall canIdDelete(void* handle, long l_id);

    //-----------------------------------------------------------------------------------
    // canIdAddArray
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanIdAddArray) (void*handle, unsigned char*aby_id);

    extern long SIECA132_API __stdcall canIdAddArray(void* handle, unsigned char* aby_id);

    //-----------------------------------------------------------------------------------
    // canIdDeleteArray
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanIdDeleteArray) (void*handle);

    extern long SIECA132_API __stdcall canIdDeleteArray(void* handle);

    //-----------------------------------------------------------------------------------
    // canEnableAllIds
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanEnableAllIds) (void*handle, bool b_EnableAllIds);

    extern long SIECA132_API __stdcall canEnableAllIds(void* handle, bool b_EnableAllIds);

    //-----------------------------------------------------------------------------------
    // canAreAllIdsEnabled
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanAreAllIdsEnabled) (void*handle, bool*pb_AllIdsEnabled);

    extern long SIECA132_API __stdcall canAreAllIdsEnabled(void* handle, bool* pb_AllIdsEnabled);

    //-----------------------------------------------------------------------------------
    // canIDStatus
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanIDStatus) (void*handle, T_ID_ARRAY*idarray);

    extern long SIECA132_API __stdcall canIDStatus(void* handle, T_ID_ARRAY* idarray);

    //-----------------------------------------------------------------------------------
    // canBreakCanRead
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanBreakcanRead) (void*handle);

    extern long SIECA132_API __stdcall canBreakcanRead(void* handle);

    //-----------------------------------------------------------------------------------
    // canSetIdOffset
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanSetIdOffset) (void*handle, unsigned long ul_offset);

    extern long SIECA132_API __stdcall canSetIdOffset(void* handle, unsigned long ul_offset);

    //-----------------------------------------------------------------------------------
    // canGetIdOffset
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanGetIdOffset) (void*handle, unsigned long*ul_offset);

    extern long SIECA132_API __stdcall canGetIdOffset(void* handle, unsigned long* ul_offset);

    //-----------------------------------------------------------------------------------
    // canStatus
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanStatus) (void*handle, CAN_IF_STATUS*Status);

    extern long SIECA132_API __stdcall canStatus(void* handle, CAN_IF_STATUS* Status);

    //-----------------------------------------------------------------------------------
    // canGetCounter
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanGetCounter) (CTRDATA * data);

    extern long SIECA132_API __stdcall canGetCounter(CTRDATA* data);

    //-----------------------------------------------------------------------------------
    // canGetCounterExtended
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanGetCounterExtended) (void*handle, CTRDATA2*data);

    extern long SIECA132_API __stdcall canGetCounterExtended(void* handle, CTRDATA2* data);

    //-----------------------------------------------------------------------------------
    // canGetBusloadExtended
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanGetBusloadExtended) (void*handle, BUSLOAD*data);

    extern long SIECA132_API __stdcall canGetBusloadExtended(void* handle, BUSLOAD* data);

    //-----------------------------------------------------------------------------------
    // canClose
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanClose) (void*handle);

    extern long SIECA132_API __stdcall canClose(void* handle);

    //-----------------------------------------------------------------------------------
    // canGetTimeout
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanGetTimeout) (void*handle, T_TIMEOUT_TYPE t_type, long*pl_timeout);

    extern long SIECA132_API __stdcall canGetTimeout(void* handle, T_TIMEOUT_TYPE t_type, long* pl_timeout);

    //-----------------------------------------------------------------------------------
    // canSetTimeout
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanSetTimeout) (void*handle, T_TIMEOUT_TYPE t_type, long l_timeout);

    extern long SIECA132_API __stdcall canSetTimeout(void* handle, T_TIMEOUT_TYPE t_type, long l_timeout);

    //-----------------------------------------------------------------------------------
    // canSetTxDelay
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanSetTxDelay) (void*handle, long l_timeout);

    extern long SIECA132_API __stdcall canSetTxDelay(void* handle, long l_timeout);

    //-----------------------------------------------------------------------------------
    // canIsVirtualHw
    //-----------------------------------------------------------------------------------
    typedef unsigned char (__stdcall *LPFNDLLcanIsVirtualHw) (void);

    extern unsigned char SIECA132_API __stdcall canIsVirtualHw(void);

    //-----------------------------------------------------------------------------------
    // canGetDllInfo
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanGetDllInfo) (void*t_Info, void*Res);

    extern long SIECA132_API __stdcall canGetDllInfo(void* t_Info, void* Res);

    //-----------------------------------------------------------------------------------
    // canIsNetOwner
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanIsNetOwner) (void*handle);

    extern long SIECA132_API __stdcall canIsNetOwner(void* handle);

    //-----------------------------------------------------------------------------------
    // canInstruction
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanInstruction) (void*handle, unsigned int*data);

    extern long SIECA132_API __stdcall canInstruction(void* handle, unsigned int* data);

    //-----------------------------------------------------------------------------------
    // canInstruction
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanGetSyncTimer) (void*handle, unsigned long*timer, __int64*perfCounter);

    extern long SIECA132_API __stdcall canGetSyncTimer(void* handle, unsigned long* timer, __int64* perfCounter);

    //-----------------------------------------------------------------------------------
    // canSetOwner
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanSetOwner) (long l_netnumber, void*newOwner);

    extern long SIECA132_API __stdcall canSetOwner(long l_netnumber, void* newOwner);

    //-----------------------------------------------------------------------------------
    // canGetOwner
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanGetOwner) (long l_netnumber, void**handle);

    extern long SIECA132_API __stdcall canGetOwner(long l_netnumber, void ** handle);

    //-----------------------------------------------------------------------------------
    // canSetFilterMode
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanSetFilterMode) (void*handle, T_FILTER_MODE t_mode);

    extern long SIECA132_API __stdcall canSetFilterMode(void* handle, T_FILTER_MODE t_mode);

    //-----------------------------------------------------------------------------------
    // canGetFilterMode
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanGetFilterMode) (void*handle, T_FILTER_MODE*t_mode);

    extern long SIECA132_API __stdcall canGetFilterMode(void* handle, T_FILTER_MODE* t_mode);

    //-----------------------------------------------------------------------------------
    // canIsHandleValid
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanIsHandleValid) (void*handle);

    extern long SIECA132_API __stdcall canIsHandleValid(void* handle);

    //-----------------------------------------------------------------------------------
    // canSetFilterJ2534
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanSetFilterJ2534)
        (
            void*handle,
            T_J2534_MODE t_mode,
            unsigned long ul_rangestart,
            unsigned long ul_rangestop
        );

    extern long SIECA132_API __stdcall canSetFilterJ2534(void* handle,
                                                         T_J2534_MODE t_mode,
                                                         unsigned long ul_rangestart,
                                                         unsigned long ul_rangestop);

    //-----------------------------------------------------------------------------------
    // canDeleteFilterJ2534
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanDeleteFilterJ2534) (void*handle);

    extern long SIECA132_API __stdcall canDeleteFilterJ2534(void* handle);

    //-----------------------------------------------------------------------------------
    // canSetFilterJ2534_2
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanSetFilterJ2534_2)
        (
            void*handle,
            T_J2534_MODE t_mode,
            unsigned long ul_mask_id,
            unsigned __int64 ul_mask_data,
            unsigned long ul_pattern_id,
            unsigned __int64 ul_pattern_data
        );

    extern long SIECA132_API __stdcall canSetFilterJ2534_2(void* handle,
                                                           T_J2534_MODE t_mode,
                                                           unsigned long ul_mask_id,
                                                           unsigned __int64 ul_mask_data,
                                                           unsigned long ul_pattern_id,
                                                           unsigned __int64 ul_pattern_data);

    //-----------------------------------------------------------------------------------
    // canDeleteFilterJ2534_2
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanDeleteFilterJ2534_2) (void*handle);

    extern long SIECA132_API __stdcall canDeleteFilterJ2534_2(void* handle);

    //-----------------------------------------------------------------------------------
    // canClearBuffer
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanClearBuffer) (void*handle);

    extern long SIECA132_API __stdcall canClearBuffer(void* handle);

    //-----------------------------------------------------------------------------------
    // canResetCounter
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanResetCounter) (void*handle);

    extern long SIECA132_API __stdcall canResetCounter(void* handle);

    //-----------------------------------------------------------------------------------
    // canGetTime
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanGetTime) (unsigned int data[]);

    extern long SIECA132_API __stdcall canGetTime(unsigned int data[]);

    //-----------------------------------------------------------------------------------
    // canGetNumberOfConnectedDevices
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanGetNumberOfConnectedDevices) (long*devices);

    extern long SIECA132_API __stdcall canGetNumberOfConnectedDevices(long* devices);

    //-----------------------------------------------------------------------------------
    // canGetNumberOfConnectedDevices
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanGetDeviceList) (T_DeviceList * list);

    extern long SIECA132_API __stdcall canGetDeviceList(T_DeviceList* list);

    //-----------------------------------------------------------------------------------
    // canGetDeviceTimestampBase
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanGetDeviceTimestampBase) (long netnr, unsigned long*Base);

    extern long SIECA132_API __stdcall canGetDeviceTimestampBase(long netnr, unsigned long* Base);  // MG:2005-01-20

    //-----------------------------------------------------------------------------------
    // canGetEepromAccess
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanGetEepromAccess) (void*handle, unsigned long area, unsigned long*key);

    extern long SIECA132_API __stdcall canGetEepromAccess(void* handle, unsigned long area, unsigned long* key);

    //-----------------------------------------------------------------------------------
    // canReadEeprom
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanReadEeprom) (void*handle, long address, long quantity, unsigned char*value);

    extern long SIECA132_API __stdcall canReadEeprom(void* handle, long address, long quanity, unsigned char* value);

    //-----------------------------------------------------------------------------------
    // canWriteEeprom
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanWriteEeprom) (void*handle, long address, long quantity, unsigned char*value);

    extern long SIECA132_API __stdcall canWriteEeprom(void* handle, long address, long quantity, unsigned char* value);

    //-----------------------------------------------------------------------------------
    // canCloseEepromAccess
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanCloseEepromAccess) (void*handle);

    extern long SIECA132_API __stdcall canCloseEepromAccess(void* handle);

    //-----------------------------------------------------------------------------------
    // canFlashDevice
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanFlashDevice) (unsigned long CardNr, char*Filename);

    extern long SIECA132_API __stdcall canFlashDevice(unsigned long CardNr, char* Filename);

    //-----------------------------------------------------------------------------------
    // canSetHWFilterEmu
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanSetHWFilterEmu)
        (
            void*handle,
            unsigned long ul_filterMode,
            unsigned long*pul_filter
        );

    extern long SIECA132_API __stdcall canSetHWFilterEmu(void* handle,
                                                         unsigned long ul_filterMode,
                                                         unsigned long* pul_filter);

    //-----------------------------------------------------------------------------------
    // canEnableBusloadFiltered
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanEnableBusloadFiltered) (void*handle, T_FILTERED_BUSLOAD t_filteredBusload);

    extern long SIECA132_API __stdcall canEnableBusloadFiltered(void* handle, T_FILTERED_BUSLOAD t_filteredBusload);

    //-----------------------------------------------------------------------------------
    // canEnableHWExtendedId
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanEnableHWExtendedId) (void*handle, bool bEnableExtID);

    extern long SIECA132_API __stdcall canEnableHWExtendedId(void* handle, bool bEnableExtID);

    //-----------------------------------------------------------------------------------
    // canGetBusloadFiltered
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanGetBusloadFiltered) (void*handle, BUSLOAD*data);

    extern long SIECA132_API __stdcall canGetBusloadFiltered(void* handle, BUSLOAD* data);

    //-----------------------------------------------------------------------------------
    // canGetCanLevel
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanGetCanLevel)
        (
            void*handle,
            T_CANLEVEL_MODE t_canLevelMode,
            long l_Ident,
            double*canLevelLow,
            double*canLevelHigh
        );

    extern long SIECA132_API __stdcall canGetCanLevel(void* handle,
                                                      T_CANLEVEL_MODE t_canLevelMode,
                                                      long l_Ident,
                                                      double* canLevelLow,
                                                      double* canLevelHigh);

    //-----------------------------------------------------------------------------------
    // canSetTxTimeout
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanSetTxTimeout) (void*handle, unsigned long ulTimeout);

    extern long SIECA132_API __stdcall canSetTxTimeout(void* handle, long lTimeout);

    //-----------------------------------------------------------------------------------
    // SetPrioritySIECE132
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLSetPrioritySIECE132) (unsigned long ul_PriorityClaas);

    extern long SIECA132_API __stdcall SetPrioritySIECE132(unsigned long ul_PriorityClaas);

    //-----------------------------------------------------------------------------------
    // GetPrioritySIECE132
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLGetPrioritySIECE132) (unsigned long*pul_PriorityClaas);

    extern long SIECA132_API __stdcall GetPrioritySIECE132(unsigned long* pul_PriorityClaas);

    //-----------------------------------------------------------------------------------
    // createCustomSharedMemory
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcreateCustomSharedMemory)
        (
            const char*pName,
            unsigned long ulMaxSizeHigh,
            unsigned long ulMaxSizeLow,
            unsigned long flProtect
        );

    extern long SIECA132_API __stdcall createCustomSharedMemory(const char* pName,
                                                                unsigned long ulMaxSizeHigh,
                                                                unsigned long ulMaxSizeLow,
                                                                unsigned long flProtect);

    //-----------------------------------------------------------------------------------
    // canSetBaudrateForce
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanSetBaudrateForce) (void*handle, long l_baud);

    extern long SIECA132_API __stdcall canSetBaudrateForce(void* handle, long l_baud);

    //-----------------------------------------------------------------------------------
    // canGetDiffTimeLastFrame
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanGetDiffTimeLastFrame) (void*handle, long*pl_diffTime);

    extern long SIECA132_API __stdcall canGetDiffTimeLastFrame(void* handle, long* pl_diffTime);

    //-----------------------------------------------------------------------------------
    // canGetHWSerialNumber
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanGetHWSerialNumber)
        (
            void*handle,
            unsigned long*pulSNHigh,
            unsigned long*plSNLow
        );

    extern long SIECA132_API __stdcall canGetHWSerialNumber(void* handle,
                                                            unsigned long* pulSNHigh,
                                                            unsigned long* pulSNLow);

    //-----------------------------------------------------------------------------------
    // canConfirmedTransmit
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanConfirmedTransmit) (void*handle, CMSG*t_canmsg, long*l_len);

    extern long SIECA132_API __stdcall canConfirmedTransmit(void* handle, CMSG* t_canmsg, long* l_len);

	//-----------------------------------------------------------------------------------
    // canReadNoWait
    //-----------------------------------------------------------------------------------
    typedef long (__stdcall *LPFNDLLcanReadNoWait) (void*handle, CMSG*t_canmsg, long*l_len);

    extern long SIECA132_API __stdcall canReadNoWait(void* handle, CMSG* t_canmsg, long* l_len);

    #ifdef __cplusplus
}
    #endif
#endif // AFX_SIECA132_H_INCLUDED_
