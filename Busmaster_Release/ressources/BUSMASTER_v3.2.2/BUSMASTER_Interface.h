

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 04:14:07 2038
 */
/* Compiler settings for BUSMASTER.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __BUSMASTER_Interface_h__
#define __BUSMASTER_Interface_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___IAppEvents_FWD_DEFINED__
#define ___IAppEvents_FWD_DEFINED__
typedef interface _IAppEvents _IAppEvents;

#endif 	/* ___IAppEvents_FWD_DEFINED__ */


#ifndef __IBusMasterApp_FWD_DEFINED__
#define __IBusMasterApp_FWD_DEFINED__
typedef interface IBusMasterApp IBusMasterApp;

#endif 	/* __IBusMasterApp_FWD_DEFINED__ */


#ifndef __BusMasterApp_FWD_DEFINED__
#define __BusMasterApp_FWD_DEFINED__

#ifdef __cplusplus
typedef class BusMasterApp BusMasterApp;
#else
typedef struct BusMasterApp BusMasterApp;
#endif /* __cplusplus */

#endif 	/* __BusMasterApp_FWD_DEFINED__ */


#ifndef __ICANComm_FWD_DEFINED__
#define __ICANComm_FWD_DEFINED__
typedef interface ICANComm ICANComm;

#endif 	/* __ICANComm_FWD_DEFINED__ */


#ifndef __Comm_FWD_DEFINED__
#define __Comm_FWD_DEFINED__

#ifdef __cplusplus
typedef class Comm Comm;
#else
typedef struct Comm Comm;
#endif /* __cplusplus */

#endif 	/* __Comm_FWD_DEFINED__ */


#ifndef __ICANLog_FWD_DEFINED__
#define __ICANLog_FWD_DEFINED__
typedef interface ICANLog ICANLog;

#endif 	/* __ICANLog_FWD_DEFINED__ */


#ifndef __Log_FWD_DEFINED__
#define __Log_FWD_DEFINED__

#ifdef __cplusplus
typedef class Log Log;
#else
typedef struct Log Log;
#endif /* __cplusplus */

#endif 	/* __Log_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __CAN_MonitorApp_LIBRARY_DEFINED__
#define __CAN_MonitorApp_LIBRARY_DEFINED__

/* library CAN_MonitorApp */
/* [version][uuid] */ 

#define	MAX_BLOCK_NAME	( 32 )

#define	MAX_FILE_PATH	( 256 )

typedef /* [public][public][public][public][public][public][public][version][uuid] */  DECLSPEC_UUID("B78C73E2-4F4D-42C1-AF94-04D1AA8C6410") 
enum __MIDL___MIDL_itf_BUSMASTER_0000_0000_0001
    {
        None	= 0,
        Start	= ( None + 1 ) ,
        Stop	= ( Start + 1 ) ,
        Both	= ( Stop + 1 ) ,
        Stopped	= ( Both + 1 ) 
    } 	eLoggingTriggerState;

typedef /* [public][public][public][public][public][version][uuid] */  DECLSPEC_UUID("194345BB-4E5D-4A96-8A35-EBF3FCA06E8E") 
enum __MIDL___MIDL_itf_BUSMASTER_0000_0000_0002
    {
        System	= 0,
        Absolute	= ( System + 1 ) ,
        Relative	= ( Absolute + 1 ) 
    } 	eTimeMode;

typedef /* [public][public][public][public][public][version][uuid] */  DECLSPEC_UUID("87458EB5-7047-4C71-A569-606110E60230") 
enum __MIDL___MIDL_itf_BUSMASTER_0000_0000_0003
    {
        Hex	= 0,
        Deci	= ( Hex + 1 ) 
    } 	eNumericMode;

typedef /* [public][public][public][public][public][version][uuid] */  DECLSPEC_UUID("11FA404A-0517-4B9E-9B86-BEC3AF44E2F1") 
enum __MIDL___MIDL_itf_BUSMASTER_0000_0000_0004
    {
        Append	= 0,
        OverWrite	= ( Append + 1 ) 
    } 	eFileMode;

typedef /* [public][version][uuid] */  DECLSPEC_UUID("B687F83A-3BFC-46DF-ACBC-22789F22D34A") 
enum __MIDL___MIDL_itf_BUSMASTER_0000_0000_0005
    {
        Signal_watch	= 0,
        network_statistics	= ( Signal_watch + 1 ) ,
        trace	= ( network_statistics + 1 ) ,
        graph	= ( trace + 1 ) 
    } 	eWindow;

typedef /* [public][version][uuid] */  DECLSPEC_UUID("C73746FF-B6F1-49A9-82AD-ACD29318D86E") 
enum __MIDL___MIDL_itf_BUSMASTER_0000_0000_0006
    {
        Active	= 0,
        Passive	= ( Active + 1 ) ,
        Simulation	= ( Passive + 1 ) 
    } 	eControllerMode;

typedef /* [public][version][uuid] */  DECLSPEC_UUID("CB996B66-3B04-49AF-BC24-5B0870CE4783") 
enum __MIDL___MIDL_itf_BUSMASTER_0000_0000_0007
    {
        all	= 0,
        timers	= ( all + 1 ) ,
        key	= ( timers + 1 ) ,
        message	= ( key + 1 ) ,
        error	= ( message + 1 ) 
    } 	eHandlerType;

typedef /* [public][version][uuid] */  DECLSPEC_UUID("966A2D62-9CC2-4E7A-BBA2-FF84426F30C2") 
enum __MIDL___MIDL_itf_BUSMASTER_0000_0000_0008
    {
        Log	= 0,
        Display	= ( Log + 1 ) 
    } 	EFILTERMODULE;

typedef /* [public][public][public][version][uuid] */  DECLSPEC_UUID("4B92B469-C5A8-497C-B4ED-D533E4434292") struct __MIDL___MIDL_itf_BUSMASTER_0000_0000_0009
    {
    unsigned int m_unMsgID;
    unsigned char m_bEXTENDED;
    unsigned char m_bRTR;
    unsigned char m_ucDataLen;
    unsigned char m_ucData[ 8 ];
    unsigned char m_ucChannel;
    } 	CAN_MSGS;

typedef /* [public][public][version][uuid] */  DECLSPEC_UUID("7404B3EA-6EA9-4423-AAE4-5FCC80A10F38") struct __MIDL___MIDL_itf_BUSMASTER_0000_0000_0010
    {
    double m_dBusLoad;
    double m_dPeakBusLoad;
    unsigned int m_unTotalMsgCount;
    unsigned int m_unMsgPerSecond;
    unsigned int m_unTotalTxMsgCount;
    double m_dTotalTxMsgRate;
    unsigned int m_unTxSTDMsgCount;
    double m_dTxSTDMsgRate;
    unsigned int m_unTxEXTDMsgCount;
    double m_dTxEXTMsgRate;
    unsigned int m_unTxSTD_RTRMsgCount;
    unsigned int m_unTxEXTD_RTRMsgCount;
    unsigned int m_unTotalRxMsgCount;
    double m_dTotalRxMsgRate;
    unsigned int m_unRxSTDMsgCount;
    double m_dRxSTDMsgRate;
    unsigned int m_unRxEXTDMsgCount;
    double m_dRxEXTMsgRate;
    unsigned int m_unRxSTD_RTRMsgCount;
    unsigned int m_unRxEXTD_RTRMsgCount;
    unsigned int m_unErrorTxCount;
    double m_dErrorTxRate;
    unsigned int m_unErrorRxCount;
    double m_dErrorRxRate;
    unsigned int m_unErrorTotalCount;
    double m_dErrorRate;
    unsigned int m_unDLCCount;
    double m_dBaudRate;
    double m_dTotalBusLoad;
    int m_nSamples;
    double m_dAvarageBusLoad;
    unsigned char m_ucTxErrorCounter;
    unsigned char m_ucRxErrorCounter;
    unsigned char m_ucTxPeakErrorCount;
    unsigned char m_ucRxPeakErrorCount;
    unsigned char m_ucStatus;
    } 	sBUSSTATISTICS_USR;

typedef /* [public][public][version][uuid] */  DECLSPEC_UUID("A739CC36-8E06-4AB2-81D7-E18DACB459C1") struct __MIDL___MIDL_itf_BUSMASTER_0000_0000_0011
    {
    unsigned long m_dMessageCode;
    unsigned int m_dNumberOfSignals;
    unsigned int m_dMessageLength;
    BOOL m_bMessageFrameFormat;
    int m_nMsgDataFormat;
    } 	sMESSAGESTRUCT;

typedef /* [version][uuid] */  DECLSPEC_UUID("D178B15F-81F0-4BCA-8898-34077760ED90") struct tagTxBlock_Usr
    {
    BSTR m_strBlockName;
    BOOL m_bPeriodical;
    USHORT m_ushInterval;
    unsigned char m_ucTrigger;
    BOOL m_bTxAllFrame;
    UCHAR m_ucKeyValue;
    UINT m_unTimeInterval;
    } 	STXBLOCK_USR;

typedef /* [version][uuid] */  DECLSPEC_UUID("C06EA17C-964D-4089-83CC-C129811E9435") STXBLOCK_USR *PSTXBLOCK_USR;

typedef /* [public][public][version][uuid] */  DECLSPEC_UUID("674F8D0C-A7DE-4672-8C5A-3FBF31EAF828") struct __MIDL___MIDL_itf_BUSMASTER_0000_0000_0012
    {
    UCHAR m_ucFilterType;
    UINT m_unMsgIDFrom;
    UINT m_unMsgIDTo;
    UCHAR m_ucDirection;
    UCHAR m_ucEXTENDED;
    UCHAR m_ucRTR;
    UCHAR m_ucChannel;
    } 	SFILTER_USR;

typedef /* [version][uuid] */  DECLSPEC_UUID("17A319D9-499B-4B2F-8701-B39EE14A44BE") SFILTER_USR *PSFILTER_USR;

typedef /* [public][public][public][public][public][public][version][uuid] */  DECLSPEC_UUID("84BBC375-4560-4793-8695-A3D53DE2F787") struct __MIDL___MIDL_itf_BUSMASTER_0000_0000_0013
    {
    eLoggingTriggerState m_unTriggerType;
    UINT m_unStartID;
    UINT m_unStopID;
    } 	SLOGTRIGGER_USR;

typedef /* [version][uuid] */  DECLSPEC_UUID("410C3330-0D5A-4AA6-98B9-5D3DFFC9B161") SLOGTRIGGER_USR *PSLOGTRIGGER_USR;

typedef /* [version][uuid] */  DECLSPEC_UUID("153552BE-D9D1-4281-BDB5-47E3698E09A5") struct tagLoggingBlockUsr
    {
    eTimeMode m_eLogTimerMode;
    eNumericMode m_eNumFormat;
    eFileMode m_eFileMode;
    SLOGTRIGGER_USR m_sLogTrigger;
    unsigned char m_sLogFileName[ 256 ];
    } 	SLOGGINGBLOCK_USR;

typedef /* [version][uuid] */  DECLSPEC_UUID("76E5E4DE-8505-42A9-9BF9-7E147B5D4943") SLOGGINGBLOCK_USR *PSLOGGINGBLOCK_USR;


EXTERN_C const IID LIBID_CAN_MonitorApp;

#ifndef ___IAppEvents_INTERFACE_DEFINED__
#define ___IAppEvents_INTERFACE_DEFINED__

/* interface _IAppEvents */
/* [object][helpstring][dual][oleautomation][uuid] */ 


EXTERN_C const IID IID__IAppEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("12324088-748E-4017-BA5A-3B7F61CE8FBF")
    _IAppEvents : public IUnknown
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnMessageReceived( 
            CAN_MSGS RxMsg) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct _IAppEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IAppEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IAppEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IAppEvents * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OnMessageReceived )( 
            _IAppEvents * This,
            CAN_MSGS RxMsg);
        
        END_INTERFACE
    } _IAppEventsVtbl;

    interface _IAppEvents
    {
        CONST_VTBL struct _IAppEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IAppEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IAppEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IAppEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IAppEvents_OnMessageReceived(This,RxMsg)	\
    ( (This)->lpVtbl -> OnMessageReceived(This,RxMsg) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ___IAppEvents_INTERFACE_DEFINED__ */


#ifndef __IBusMasterApp_INTERFACE_DEFINED__
#define __IBusMasterApp_INTERFACE_DEFINED__

/* interface IBusMasterApp */
/* [object][dual][oleautomation][uuid] */ 


EXTERN_C const IID IID_IBusMasterApp;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B3DBF7E2-93DD-4c0c-A237-0E8E46D354C6")
    IBusMasterApp : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Connect( 
            BOOL bConnect) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetMsgInfo( 
            BSTR MsgName,
            sMESSAGESTRUCT *sMsgStruct) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetNetworkStatistics( 
            int nChannel,
            sBUSSTATISTICS_USR *sStat) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetErrorCounter( 
            UCHAR *Tx,
            UCHAR *Rx,
            INT nChannel) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ImportDatabase( 
            BSTR DBFilePath) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadConfiguration( 
            BSTR FileName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveConfiguration( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveConfigurationAs( 
            BSTR ConfigPath) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RegisterClientForRx( 
            /* [in] */ USHORT usUniqueID,
            /* [out] */ BSTR *pEventName,
            /* [out] */ BSTR *pPIPEName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE UnRegisterClient( 
            /* [in] */ USHORT usUniqueID) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IBusMasterAppVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBusMasterApp * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBusMasterApp * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBusMasterApp * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IBusMasterApp * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IBusMasterApp * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IBusMasterApp * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IBusMasterApp * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect )( 
            IBusMasterApp * This,
            BOOL bConnect);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetMsgInfo )( 
            IBusMasterApp * This,
            BSTR MsgName,
            sMESSAGESTRUCT *sMsgStruct);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetNetworkStatistics )( 
            IBusMasterApp * This,
            int nChannel,
            sBUSSTATISTICS_USR *sStat);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetErrorCounter )( 
            IBusMasterApp * This,
            UCHAR *Tx,
            UCHAR *Rx,
            INT nChannel);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ImportDatabase )( 
            IBusMasterApp * This,
            BSTR DBFilePath);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadConfiguration )( 
            IBusMasterApp * This,
            BSTR FileName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SaveConfiguration )( 
            IBusMasterApp * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SaveConfigurationAs )( 
            IBusMasterApp * This,
            BSTR ConfigPath);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RegisterClientForRx )( 
            IBusMasterApp * This,
            /* [in] */ USHORT usUniqueID,
            /* [out] */ BSTR *pEventName,
            /* [out] */ BSTR *pPIPEName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *UnRegisterClient )( 
            IBusMasterApp * This,
            /* [in] */ USHORT usUniqueID);
        
        END_INTERFACE
    } IBusMasterAppVtbl;

    interface IBusMasterApp
    {
        CONST_VTBL struct IBusMasterAppVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBusMasterApp_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBusMasterApp_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBusMasterApp_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBusMasterApp_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IBusMasterApp_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IBusMasterApp_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IBusMasterApp_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IBusMasterApp_Connect(This,bConnect)	\
    ( (This)->lpVtbl -> Connect(This,bConnect) ) 

#define IBusMasterApp_GetMsgInfo(This,MsgName,sMsgStruct)	\
    ( (This)->lpVtbl -> GetMsgInfo(This,MsgName,sMsgStruct) ) 

#define IBusMasterApp_GetNetworkStatistics(This,nChannel,sStat)	\
    ( (This)->lpVtbl -> GetNetworkStatistics(This,nChannel,sStat) ) 

#define IBusMasterApp_GetErrorCounter(This,Tx,Rx,nChannel)	\
    ( (This)->lpVtbl -> GetErrorCounter(This,Tx,Rx,nChannel) ) 

#define IBusMasterApp_ImportDatabase(This,DBFilePath)	\
    ( (This)->lpVtbl -> ImportDatabase(This,DBFilePath) ) 

#define IBusMasterApp_LoadConfiguration(This,FileName)	\
    ( (This)->lpVtbl -> LoadConfiguration(This,FileName) ) 

#define IBusMasterApp_SaveConfiguration(This)	\
    ( (This)->lpVtbl -> SaveConfiguration(This) ) 

#define IBusMasterApp_SaveConfigurationAs(This,ConfigPath)	\
    ( (This)->lpVtbl -> SaveConfigurationAs(This,ConfigPath) ) 

#define IBusMasterApp_RegisterClientForRx(This,usUniqueID,pEventName,pPIPEName)	\
    ( (This)->lpVtbl -> RegisterClientForRx(This,usUniqueID,pEventName,pPIPEName) ) 

#define IBusMasterApp_UnRegisterClient(This,usUniqueID)	\
    ( (This)->lpVtbl -> UnRegisterClient(This,usUniqueID) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBusMasterApp_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_BusMasterApp;

#ifdef __cplusplus

class DECLSPEC_UUID("92D435C1-A552-4435-AD1E-468B4C17BDC7")
BusMasterApp;
#endif

#ifndef __ICANComm_INTERFACE_DEFINED__
#define __ICANComm_INTERFACE_DEFINED__

/* interface ICANComm */
/* [object][dual][oleautomation][uuid] */ 


EXTERN_C const IID IID_ICANComm;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("AF3A15D0-1444-4640-B60A-E360EFFF5051")
    ICANComm : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SendCANMSg( 
            CAN_MSGS *sMsg) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetApplication( 
            /* [out] */ IBusMasterApp **pApplication) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICANCommVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICANComm * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICANComm * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICANComm * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICANComm * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICANComm * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICANComm * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICANComm * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SendCANMSg )( 
            ICANComm * This,
            CAN_MSGS *sMsg);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetApplication )( 
            ICANComm * This,
            /* [out] */ IBusMasterApp **pApplication);
        
        END_INTERFACE
    } ICANCommVtbl;

    interface ICANComm
    {
        CONST_VTBL struct ICANCommVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICANComm_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICANComm_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICANComm_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICANComm_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICANComm_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICANComm_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICANComm_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ICANComm_SendCANMSg(This,sMsg)	\
    ( (This)->lpVtbl -> SendCANMSg(This,sMsg) ) 

#define ICANComm_GetApplication(This,pApplication)	\
    ( (This)->lpVtbl -> GetApplication(This,pApplication) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICANComm_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Comm;

#ifdef __cplusplus

class DECLSPEC_UUID("1B90CA83-8652-41FC-B29A-191D57B455A3")
Comm;
#endif

#ifndef __ICANLog_INTERFACE_DEFINED__
#define __ICANLog_INTERFACE_DEFINED__

/* interface ICANLog */
/* [object][dual][oleautomation][uuid] */ 


EXTERN_C const IID IID_ICANLog;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("29AEFD20-EA16-4A3B-B0B2-6EEB6971D066")
    ICANLog : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StopLogging( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartLogging( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddLoggingBlock( 
            SLOGGINGBLOCK_USR *psLoggingBlock) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetLoggingBlock( 
            /* [in] */ USHORT BlockIndex,
            /* [out] */ SLOGGINGBLOCK_USR *psLoggingBlock) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RemoveLoggingBlock( 
            /* [in] */ USHORT BlockIndex) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetLoggingBlockCount( 
            /* [out] */ USHORT *BlockCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ClearLoggingBlockList( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WriteToLogFile( 
            USHORT BlockIndex,
            BSTR pcStr) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetApplication( 
            /* [out] */ IBusMasterApp **pApplication) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICANLogVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICANLog * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICANLog * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICANLog * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICANLog * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICANLog * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICANLog * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICANLog * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StopLogging )( 
            ICANLog * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StartLogging )( 
            ICANLog * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddLoggingBlock )( 
            ICANLog * This,
            SLOGGINGBLOCK_USR *psLoggingBlock);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetLoggingBlock )( 
            ICANLog * This,
            /* [in] */ USHORT BlockIndex,
            /* [out] */ SLOGGINGBLOCK_USR *psLoggingBlock);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RemoveLoggingBlock )( 
            ICANLog * This,
            /* [in] */ USHORT BlockIndex);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetLoggingBlockCount )( 
            ICANLog * This,
            /* [out] */ USHORT *BlockCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ClearLoggingBlockList )( 
            ICANLog * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *WriteToLogFile )( 
            ICANLog * This,
            USHORT BlockIndex,
            BSTR pcStr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetApplication )( 
            ICANLog * This,
            /* [out] */ IBusMasterApp **pApplication);
        
        END_INTERFACE
    } ICANLogVtbl;

    interface ICANLog
    {
        CONST_VTBL struct ICANLogVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICANLog_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICANLog_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICANLog_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICANLog_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICANLog_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICANLog_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICANLog_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ICANLog_StopLogging(This)	\
    ( (This)->lpVtbl -> StopLogging(This) ) 

#define ICANLog_StartLogging(This)	\
    ( (This)->lpVtbl -> StartLogging(This) ) 

#define ICANLog_AddLoggingBlock(This,psLoggingBlock)	\
    ( (This)->lpVtbl -> AddLoggingBlock(This,psLoggingBlock) ) 

#define ICANLog_GetLoggingBlock(This,BlockIndex,psLoggingBlock)	\
    ( (This)->lpVtbl -> GetLoggingBlock(This,BlockIndex,psLoggingBlock) ) 

#define ICANLog_RemoveLoggingBlock(This,BlockIndex)	\
    ( (This)->lpVtbl -> RemoveLoggingBlock(This,BlockIndex) ) 

#define ICANLog_GetLoggingBlockCount(This,BlockCount)	\
    ( (This)->lpVtbl -> GetLoggingBlockCount(This,BlockCount) ) 

#define ICANLog_ClearLoggingBlockList(This)	\
    ( (This)->lpVtbl -> ClearLoggingBlockList(This) ) 

#define ICANLog_WriteToLogFile(This,BlockIndex,pcStr)	\
    ( (This)->lpVtbl -> WriteToLogFile(This,BlockIndex,pcStr) ) 

#define ICANLog_GetApplication(This,pApplication)	\
    ( (This)->lpVtbl -> GetApplication(This,pApplication) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICANLog_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Log;

#ifdef __cplusplus

class DECLSPEC_UUID("BADFFC69-8FCB-4181-880C-615D09848CD6")
Log;
#endif
#endif /* __CAN_MonitorApp_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


