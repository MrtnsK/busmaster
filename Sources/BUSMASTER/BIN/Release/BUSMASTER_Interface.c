

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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



#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        EXTERN_C __declspec(selectany) const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif // !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_CAN_MonitorApp,0xC3F9B41A,0xD3AD,0x4b36,0xBA,0x65,0xC5,0xC2,0xF1,0xA3,0x14,0xD9);


MIDL_DEFINE_GUID(IID, IID__IAppEvents,0x12324088,0x748E,0x4017,0xBA,0x5A,0x3B,0x7F,0x61,0xCE,0x8F,0xBF);


MIDL_DEFINE_GUID(IID, IID_IBusMasterApp,0xB3DBF7E2,0x93DD,0x4c0c,0xA2,0x37,0x0E,0x8E,0x46,0xD3,0x54,0xC6);


MIDL_DEFINE_GUID(CLSID, CLSID_BusMasterApp,0x92D435C1,0xA552,0x4435,0xAD,0x1E,0x46,0x8B,0x4C,0x17,0xBD,0xC7);


MIDL_DEFINE_GUID(IID, IID_ICANComm,0xAF3A15D0,0x1444,0x4640,0xB6,0x0A,0xE3,0x60,0xEF,0xFF,0x50,0x51);


MIDL_DEFINE_GUID(CLSID, CLSID_Comm,0x1B90CA83,0x8652,0x41FC,0xB2,0x9A,0x19,0x1D,0x57,0xB4,0x55,0xA3);


MIDL_DEFINE_GUID(IID, IID_ICANLog,0x29AEFD20,0xEA16,0x4A3B,0xB0,0xB2,0x6E,0xEB,0x69,0x71,0xD0,0x66);


MIDL_DEFINE_GUID(CLSID, CLSID_Log,0xBADFFC69,0x8FCB,0x4181,0x88,0x0C,0x61,0x5D,0x09,0x84,0x8C,0xD6);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



