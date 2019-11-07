

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 04:14:07 2038
 */
/* Compiler settings for BusEmulation.idl:
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

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __BusEmulation_h__
#define __BusEmulation_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ISimENG_FWD_DEFINED__
#define __ISimENG_FWD_DEFINED__
typedef interface ISimENG ISimENG;

#endif 	/* __ISimENG_FWD_DEFINED__ */


#ifndef __SimENG_FWD_DEFINED__
#define __SimENG_FWD_DEFINED__

#ifdef __cplusplus
typedef class SimENG SimENG;
#else
typedef struct SimENG SimENG;
#endif /* __cplusplus */

#endif 	/* __SimENG_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ISimENG_INTERFACE_DEFINED__
#define __ISimENG_INTERFACE_DEFINED__

/* interface ISimENG */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ISimENG;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("32610836-D66A-4A57-83D5-EA50ECB0B7BB")
    ISimENG : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SendMessage( 
            /* [in] */ USHORT ClientID,
            /* [in] */ USHORT CurrDataLength,
            /* [in] */ BYTE pbCurrDataByte[ 128 ]) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RegisterClient( 
            /* [in] */ USHORT Bus,
            /* [in] */ USHORT MaxDataLen,
            /* [out] */ USHORT *ClientID,
            /* [out] */ BSTR *PipeName,
            /* [out] */ BSTR *EventName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE UnregisterClient( 
            /* [in] */ USHORT ClientID) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ConnectNode( 
            /* [in] */ USHORT ClientID) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DisconnectNode( 
            /* [in] */ USHORT ClientID) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetTimeModeMapping( 
            /* [out] */ SYSTEMTIME *CurrSysTime,
            /* [out] */ ULONGLONG *TimeStamp,
            /* [out] */ LARGE_INTEGER *lQueryTickCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetCurrentStatus( 
            /* [in] */ USHORT ClientID,
            /* [out][in] */ VARIANT *pNodeStatus) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ISimENGVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISimENG * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISimENG * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISimENG * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISimENG * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISimENG * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISimENG * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISimENG * This,
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
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SendMessage )( 
            ISimENG * This,
            /* [in] */ USHORT ClientID,
            /* [in] */ USHORT CurrDataLength,
            /* [in] */ BYTE pbCurrDataByte[ 128 ]);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RegisterClient )( 
            ISimENG * This,
            /* [in] */ USHORT Bus,
            /* [in] */ USHORT MaxDataLen,
            /* [out] */ USHORT *ClientID,
            /* [out] */ BSTR *PipeName,
            /* [out] */ BSTR *EventName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *UnregisterClient )( 
            ISimENG * This,
            /* [in] */ USHORT ClientID);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ConnectNode )( 
            ISimENG * This,
            /* [in] */ USHORT ClientID);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DisconnectNode )( 
            ISimENG * This,
            /* [in] */ USHORT ClientID);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetTimeModeMapping )( 
            ISimENG * This,
            /* [out] */ SYSTEMTIME *CurrSysTime,
            /* [out] */ ULONGLONG *TimeStamp,
            /* [out] */ LARGE_INTEGER *lQueryTickCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetCurrentStatus )( 
            ISimENG * This,
            /* [in] */ USHORT ClientID,
            /* [out][in] */ VARIANT *pNodeStatus);
        
        END_INTERFACE
    } ISimENGVtbl;

    interface ISimENG
    {
        CONST_VTBL struct ISimENGVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISimENG_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISimENG_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISimENG_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISimENG_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISimENG_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISimENG_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISimENG_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISimENG_SendMessage(This,ClientID,CurrDataLength,pbCurrDataByte)	\
    ( (This)->lpVtbl -> SendMessage(This,ClientID,CurrDataLength,pbCurrDataByte) ) 

#define ISimENG_RegisterClient(This,Bus,MaxDataLen,ClientID,PipeName,EventName)	\
    ( (This)->lpVtbl -> RegisterClient(This,Bus,MaxDataLen,ClientID,PipeName,EventName) ) 

#define ISimENG_UnregisterClient(This,ClientID)	\
    ( (This)->lpVtbl -> UnregisterClient(This,ClientID) ) 

#define ISimENG_ConnectNode(This,ClientID)	\
    ( (This)->lpVtbl -> ConnectNode(This,ClientID) ) 

#define ISimENG_DisconnectNode(This,ClientID)	\
    ( (This)->lpVtbl -> DisconnectNode(This,ClientID) ) 

#define ISimENG_GetTimeModeMapping(This,CurrSysTime,TimeStamp,lQueryTickCount)	\
    ( (This)->lpVtbl -> GetTimeModeMapping(This,CurrSysTime,TimeStamp,lQueryTickCount) ) 

#define ISimENG_GetCurrentStatus(This,ClientID,pNodeStatus)	\
    ( (This)->lpVtbl -> GetCurrentStatus(This,ClientID,pNodeStatus) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISimENG_INTERFACE_DEFINED__ */



#ifndef __BusEmulationLib_LIBRARY_DEFINED__
#define __BusEmulationLib_LIBRARY_DEFINED__

/* library BusEmulationLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_BusEmulationLib;

EXTERN_C const CLSID CLSID_SimENG;

#ifdef __cplusplus

class DECLSPEC_UUID("5983045C-CA05-47CF-8CD8-86A4445FB48C")
SimENG;
#endif
#endif /* __BusEmulationLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

unsigned long             __RPC_USER  BSTR_UserSize64(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal64(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal64(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree64(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize64(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal64(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal64(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree64(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


