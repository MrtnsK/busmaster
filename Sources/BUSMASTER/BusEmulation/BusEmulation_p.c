

/* this ALWAYS GENERATED file contains the proxy stub code */


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

#if !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_)


#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4152 )  /* function/data pointer conversion in expression */
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif /* __RPCPROXY_H_VERSION__ */


#include "BusEmulation.h"

#define TYPE_FORMAT_STRING_SIZE   1239                              
#define PROC_FORMAT_STRING_SIZE   307                               
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   2            

typedef struct _BusEmulation_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } BusEmulation_MIDL_TYPE_FORMAT_STRING;

typedef struct _BusEmulation_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } BusEmulation_MIDL_PROC_FORMAT_STRING;

typedef struct _BusEmulation_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } BusEmulation_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const BusEmulation_MIDL_TYPE_FORMAT_STRING BusEmulation__MIDL_TypeFormatString;
extern const BusEmulation_MIDL_PROC_FORMAT_STRING BusEmulation__MIDL_ProcFormatString;
extern const BusEmulation_MIDL_EXPR_FORMAT_STRING BusEmulation__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ISimENG_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO ISimENG_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif
#if !(TARGET_IS_NT60_OR_LATER)
#error You need Windows Vista or later to run this stub because it uses these features:
#error   forced complex structure or array, new range semantics, compiled for Windows Vista.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const BusEmulation_MIDL_PROC_FORMAT_STRING BusEmulation__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure SendMessage */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
/*  8 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 10 */	NdrFcShort( 0x9c ),	/* 156 */
/* 12 */	NdrFcShort( 0x8 ),	/* 8 */
/* 14 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 16 */	0x8,		/* 8 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 18 */	NdrFcShort( 0x0 ),	/* 0 */
/* 20 */	NdrFcShort( 0x0 ),	/* 0 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ClientID */

/* 24 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 26 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 28 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter CurrDataLength */

/* 30 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 32 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 34 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter pbCurrDataByte */

/* 36 */	NdrFcShort( 0xa ),	/* Flags:  must free, in, */
/* 38 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 40 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */

/* 42 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 44 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 46 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RegisterClient */

/* 48 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 50 */	NdrFcLong( 0x0 ),	/* 0 */
/* 54 */	NdrFcShort( 0x8 ),	/* 8 */
/* 56 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 58 */	NdrFcShort( 0xc ),	/* 12 */
/* 60 */	NdrFcShort( 0x22 ),	/* 34 */
/* 62 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x6,		/* 6 */
/* 64 */	0x8,		/* 8 */
			0x43,		/* Ext Flags:  new corr desc, clt corr check, has range on conformance */
/* 66 */	NdrFcShort( 0x1 ),	/* 1 */
/* 68 */	NdrFcShort( 0x0 ),	/* 0 */
/* 70 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter Bus */

/* 72 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 74 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 76 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter MaxDataLen */

/* 78 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 80 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 82 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter ClientID */

/* 84 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 86 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 88 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter PipeName */

/* 90 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 92 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 94 */	NdrFcShort( 0x34 ),	/* Type Offset=52 */

	/* Parameter EventName */

/* 96 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 98 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 100 */	NdrFcShort( 0x34 ),	/* Type Offset=52 */

	/* Return value */

/* 102 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 104 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 106 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure UnregisterClient */

/* 108 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 110 */	NdrFcLong( 0x0 ),	/* 0 */
/* 114 */	NdrFcShort( 0x9 ),	/* 9 */
/* 116 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 118 */	NdrFcShort( 0x6 ),	/* 6 */
/* 120 */	NdrFcShort( 0x8 ),	/* 8 */
/* 122 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 124 */	0x8,		/* 8 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 126 */	NdrFcShort( 0x0 ),	/* 0 */
/* 128 */	NdrFcShort( 0x0 ),	/* 0 */
/* 130 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ClientID */

/* 132 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 134 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 136 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 138 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 140 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 142 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ConnectNode */

/* 144 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 146 */	NdrFcLong( 0x0 ),	/* 0 */
/* 150 */	NdrFcShort( 0xa ),	/* 10 */
/* 152 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 154 */	NdrFcShort( 0x6 ),	/* 6 */
/* 156 */	NdrFcShort( 0x8 ),	/* 8 */
/* 158 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 160 */	0x8,		/* 8 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 162 */	NdrFcShort( 0x0 ),	/* 0 */
/* 164 */	NdrFcShort( 0x0 ),	/* 0 */
/* 166 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ClientID */

/* 168 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 170 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 172 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 174 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 176 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 178 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DisconnectNode */

/* 180 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 182 */	NdrFcLong( 0x0 ),	/* 0 */
/* 186 */	NdrFcShort( 0xb ),	/* 11 */
/* 188 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 190 */	NdrFcShort( 0x6 ),	/* 6 */
/* 192 */	NdrFcShort( 0x8 ),	/* 8 */
/* 194 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 196 */	0x8,		/* 8 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 198 */	NdrFcShort( 0x0 ),	/* 0 */
/* 200 */	NdrFcShort( 0x0 ),	/* 0 */
/* 202 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ClientID */

/* 204 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 206 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 208 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 210 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 212 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 214 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetTimeModeMapping */

/* 216 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 218 */	NdrFcLong( 0x0 ),	/* 0 */
/* 222 */	NdrFcShort( 0xc ),	/* 12 */
/* 224 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 226 */	NdrFcShort( 0x0 ),	/* 0 */
/* 228 */	NdrFcShort( 0x8c ),	/* 140 */
/* 230 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 232 */	0x8,		/* 8 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 234 */	NdrFcShort( 0x0 ),	/* 0 */
/* 236 */	NdrFcShort( 0x0 ),	/* 0 */
/* 238 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter CurrSysTime */

/* 240 */	NdrFcShort( 0x4112 ),	/* Flags:  must free, out, simple ref, srv alloc size=16 */
/* 242 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 244 */	NdrFcShort( 0x42 ),	/* Type Offset=66 */

	/* Parameter TimeStamp */

/* 246 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 248 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 250 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Parameter lQueryTickCount */

/* 252 */	NdrFcShort( 0x2112 ),	/* Flags:  must free, out, simple ref, srv alloc size=8 */
/* 254 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 256 */	NdrFcShort( 0x58 ),	/* Type Offset=88 */

	/* Return value */

/* 258 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 260 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 262 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetCurrentStatus */

/* 264 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 266 */	NdrFcLong( 0x0 ),	/* 0 */
/* 270 */	NdrFcShort( 0xd ),	/* 13 */
/* 272 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 274 */	NdrFcShort( 0x6 ),	/* 6 */
/* 276 */	NdrFcShort( 0x8 ),	/* 8 */
/* 278 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 280 */	0x8,		/* 8 */
			0x47,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, has range on conformance */
/* 282 */	NdrFcShort( 0x1 ),	/* 1 */
/* 284 */	NdrFcShort( 0x1 ),	/* 1 */
/* 286 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ClientID */

/* 288 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 290 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 292 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter pNodeStatus */

/* 294 */	NdrFcShort( 0x11b ),	/* Flags:  must size, must free, in, out, simple ref, */
/* 296 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 298 */	NdrFcShort( 0x4cc ),	/* Type Offset=1228 */

	/* Return value */

/* 300 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 302 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 304 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const BusEmulation_MIDL_TYPE_FORMAT_STRING BusEmulation__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/*  4 */	NdrFcShort( 0x80 ),	/* 128 */
/*  6 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/*  8 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 10 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 12 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 14 */	NdrFcShort( 0x26 ),	/* Offset= 38 (52) */
/* 16 */	
			0x13, 0x0,	/* FC_OP */
/* 18 */	NdrFcShort( 0x18 ),	/* Offset= 24 (42) */
/* 20 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 22 */	NdrFcShort( 0x2 ),	/* 2 */
/* 24 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 26 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 28 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 30 */	0x0 , 
			0x0,		/* 0 */
/* 32 */	NdrFcLong( 0x0 ),	/* 0 */
/* 36 */	NdrFcLong( 0x0 ),	/* 0 */
/* 40 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 42 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 44 */	NdrFcShort( 0x8 ),	/* 8 */
/* 46 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (20) */
/* 48 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 50 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 52 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 54 */	NdrFcShort( 0x0 ),	/* 0 */
/* 56 */	NdrFcShort( 0x4 ),	/* 4 */
/* 58 */	NdrFcShort( 0x0 ),	/* 0 */
/* 60 */	NdrFcShort( 0xffd4 ),	/* Offset= -44 (16) */
/* 62 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 64 */	NdrFcShort( 0x2 ),	/* Offset= 2 (66) */
/* 66 */	
			0x15,		/* FC_STRUCT */
			0x1,		/* 1 */
/* 68 */	NdrFcShort( 0x10 ),	/* 16 */
/* 70 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 72 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 74 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 76 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 78 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 80 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 82 */	0xb,		/* FC_HYPER */
			0x5c,		/* FC_PAD */
/* 84 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 86 */	NdrFcShort( 0x2 ),	/* Offset= 2 (88) */
/* 88 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 90 */	NdrFcShort( 0x8 ),	/* 8 */
/* 92 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 94 */	
			0x11, 0x0,	/* FC_RP */
/* 96 */	NdrFcShort( 0x46c ),	/* Offset= 1132 (1228) */
/* 98 */	
			0x13, 0x0,	/* FC_OP */
/* 100 */	NdrFcShort( 0x454 ),	/* Offset= 1108 (1208) */
/* 102 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 104 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 106 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 108 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 110 */	0x0 , 
			0x0,		/* 0 */
/* 112 */	NdrFcLong( 0x0 ),	/* 0 */
/* 116 */	NdrFcLong( 0x0 ),	/* 0 */
/* 120 */	NdrFcShort( 0x2 ),	/* Offset= 2 (122) */
/* 122 */	NdrFcShort( 0x10 ),	/* 16 */
/* 124 */	NdrFcShort( 0x2f ),	/* 47 */
/* 126 */	NdrFcLong( 0x14 ),	/* 20 */
/* 130 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 132 */	NdrFcLong( 0x3 ),	/* 3 */
/* 136 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 138 */	NdrFcLong( 0x11 ),	/* 17 */
/* 142 */	NdrFcShort( 0x8001 ),	/* Simple arm type: FC_BYTE */
/* 144 */	NdrFcLong( 0x2 ),	/* 2 */
/* 148 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 150 */	NdrFcLong( 0x4 ),	/* 4 */
/* 154 */	NdrFcShort( 0x800a ),	/* Simple arm type: FC_FLOAT */
/* 156 */	NdrFcLong( 0x5 ),	/* 5 */
/* 160 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 162 */	NdrFcLong( 0xb ),	/* 11 */
/* 166 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 168 */	NdrFcLong( 0xa ),	/* 10 */
/* 172 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 174 */	NdrFcLong( 0x6 ),	/* 6 */
/* 178 */	NdrFcShort( 0xffa6 ),	/* Offset= -90 (88) */
/* 180 */	NdrFcLong( 0x7 ),	/* 7 */
/* 184 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 186 */	NdrFcLong( 0x8 ),	/* 8 */
/* 190 */	NdrFcShort( 0xff52 ),	/* Offset= -174 (16) */
/* 192 */	NdrFcLong( 0xd ),	/* 13 */
/* 196 */	NdrFcShort( 0xd6 ),	/* Offset= 214 (410) */
/* 198 */	NdrFcLong( 0x9 ),	/* 9 */
/* 202 */	NdrFcShort( 0xe2 ),	/* Offset= 226 (428) */
/* 204 */	NdrFcLong( 0x2000 ),	/* 8192 */
/* 208 */	NdrFcShort( 0xee ),	/* Offset= 238 (446) */
/* 210 */	NdrFcLong( 0x24 ),	/* 36 */
/* 214 */	NdrFcShort( 0x398 ),	/* Offset= 920 (1134) */
/* 216 */	NdrFcLong( 0x4024 ),	/* 16420 */
/* 220 */	NdrFcShort( 0x392 ),	/* Offset= 914 (1134) */
/* 222 */	NdrFcLong( 0x4011 ),	/* 16401 */
/* 226 */	NdrFcShort( 0x390 ),	/* Offset= 912 (1138) */
/* 228 */	NdrFcLong( 0x4002 ),	/* 16386 */
/* 232 */	NdrFcShort( 0x38e ),	/* Offset= 910 (1142) */
/* 234 */	NdrFcLong( 0x4003 ),	/* 16387 */
/* 238 */	NdrFcShort( 0x38c ),	/* Offset= 908 (1146) */
/* 240 */	NdrFcLong( 0x4014 ),	/* 16404 */
/* 244 */	NdrFcShort( 0x38a ),	/* Offset= 906 (1150) */
/* 246 */	NdrFcLong( 0x4004 ),	/* 16388 */
/* 250 */	NdrFcShort( 0x388 ),	/* Offset= 904 (1154) */
/* 252 */	NdrFcLong( 0x4005 ),	/* 16389 */
/* 256 */	NdrFcShort( 0x386 ),	/* Offset= 902 (1158) */
/* 258 */	NdrFcLong( 0x400b ),	/* 16395 */
/* 262 */	NdrFcShort( 0x370 ),	/* Offset= 880 (1142) */
/* 264 */	NdrFcLong( 0x400a ),	/* 16394 */
/* 268 */	NdrFcShort( 0x36e ),	/* Offset= 878 (1146) */
/* 270 */	NdrFcLong( 0x4006 ),	/* 16390 */
/* 274 */	NdrFcShort( 0x378 ),	/* Offset= 888 (1162) */
/* 276 */	NdrFcLong( 0x4007 ),	/* 16391 */
/* 280 */	NdrFcShort( 0x36e ),	/* Offset= 878 (1158) */
/* 282 */	NdrFcLong( 0x4008 ),	/* 16392 */
/* 286 */	NdrFcShort( 0x370 ),	/* Offset= 880 (1166) */
/* 288 */	NdrFcLong( 0x400d ),	/* 16397 */
/* 292 */	NdrFcShort( 0x36e ),	/* Offset= 878 (1170) */
/* 294 */	NdrFcLong( 0x4009 ),	/* 16393 */
/* 298 */	NdrFcShort( 0x36c ),	/* Offset= 876 (1174) */
/* 300 */	NdrFcLong( 0x6000 ),	/* 24576 */
/* 304 */	NdrFcShort( 0x36a ),	/* Offset= 874 (1178) */
/* 306 */	NdrFcLong( 0x400c ),	/* 16396 */
/* 310 */	NdrFcShort( 0x368 ),	/* Offset= 872 (1182) */
/* 312 */	NdrFcLong( 0x10 ),	/* 16 */
/* 316 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 318 */	NdrFcLong( 0x12 ),	/* 18 */
/* 322 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 324 */	NdrFcLong( 0x13 ),	/* 19 */
/* 328 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 330 */	NdrFcLong( 0x15 ),	/* 21 */
/* 334 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 336 */	NdrFcLong( 0x16 ),	/* 22 */
/* 340 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 342 */	NdrFcLong( 0x17 ),	/* 23 */
/* 346 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 348 */	NdrFcLong( 0xe ),	/* 14 */
/* 352 */	NdrFcShort( 0x346 ),	/* Offset= 838 (1190) */
/* 354 */	NdrFcLong( 0x400e ),	/* 16398 */
/* 358 */	NdrFcShort( 0x34a ),	/* Offset= 842 (1200) */
/* 360 */	NdrFcLong( 0x4010 ),	/* 16400 */
/* 364 */	NdrFcShort( 0x348 ),	/* Offset= 840 (1204) */
/* 366 */	NdrFcLong( 0x4012 ),	/* 16402 */
/* 370 */	NdrFcShort( 0x304 ),	/* Offset= 772 (1142) */
/* 372 */	NdrFcLong( 0x4013 ),	/* 16403 */
/* 376 */	NdrFcShort( 0x302 ),	/* Offset= 770 (1146) */
/* 378 */	NdrFcLong( 0x4015 ),	/* 16405 */
/* 382 */	NdrFcShort( 0x300 ),	/* Offset= 768 (1150) */
/* 384 */	NdrFcLong( 0x4016 ),	/* 16406 */
/* 388 */	NdrFcShort( 0x2f6 ),	/* Offset= 758 (1146) */
/* 390 */	NdrFcLong( 0x4017 ),	/* 16407 */
/* 394 */	NdrFcShort( 0x2f0 ),	/* Offset= 752 (1146) */
/* 396 */	NdrFcLong( 0x0 ),	/* 0 */
/* 400 */	NdrFcShort( 0x0 ),	/* Offset= 0 (400) */
/* 402 */	NdrFcLong( 0x1 ),	/* 1 */
/* 406 */	NdrFcShort( 0x0 ),	/* Offset= 0 (406) */
/* 408 */	NdrFcShort( 0xffff ),	/* Offset= -1 (407) */
/* 410 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 412 */	NdrFcLong( 0x0 ),	/* 0 */
/* 416 */	NdrFcShort( 0x0 ),	/* 0 */
/* 418 */	NdrFcShort( 0x0 ),	/* 0 */
/* 420 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 422 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 424 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 426 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 428 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 430 */	NdrFcLong( 0x20400 ),	/* 132096 */
/* 434 */	NdrFcShort( 0x0 ),	/* 0 */
/* 436 */	NdrFcShort( 0x0 ),	/* 0 */
/* 438 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 440 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 442 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 444 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 446 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 448 */	NdrFcShort( 0x2 ),	/* Offset= 2 (450) */
/* 450 */	
			0x13, 0x0,	/* FC_OP */
/* 452 */	NdrFcShort( 0x298 ),	/* Offset= 664 (1116) */
/* 454 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x49,		/* 73 */
/* 456 */	NdrFcShort( 0x18 ),	/* 24 */
/* 458 */	NdrFcShort( 0xa ),	/* 10 */
/* 460 */	NdrFcLong( 0x8 ),	/* 8 */
/* 464 */	NdrFcShort( 0x64 ),	/* Offset= 100 (564) */
/* 466 */	NdrFcLong( 0xd ),	/* 13 */
/* 470 */	NdrFcShort( 0x9c ),	/* Offset= 156 (626) */
/* 472 */	NdrFcLong( 0x9 ),	/* 9 */
/* 476 */	NdrFcShort( 0xd0 ),	/* Offset= 208 (684) */
/* 478 */	NdrFcLong( 0xc ),	/* 12 */
/* 482 */	NdrFcShort( 0x104 ),	/* Offset= 260 (742) */
/* 484 */	NdrFcLong( 0x24 ),	/* 36 */
/* 488 */	NdrFcShort( 0x174 ),	/* Offset= 372 (860) */
/* 490 */	NdrFcLong( 0x800d ),	/* 32781 */
/* 494 */	NdrFcShort( 0x190 ),	/* Offset= 400 (894) */
/* 496 */	NdrFcLong( 0x10 ),	/* 16 */
/* 500 */	NdrFcShort( 0x1b4 ),	/* Offset= 436 (936) */
/* 502 */	NdrFcLong( 0x2 ),	/* 2 */
/* 506 */	NdrFcShort( 0x1d8 ),	/* Offset= 472 (978) */
/* 508 */	NdrFcLong( 0x3 ),	/* 3 */
/* 512 */	NdrFcShort( 0x1fc ),	/* Offset= 508 (1020) */
/* 514 */	NdrFcLong( 0x14 ),	/* 20 */
/* 518 */	NdrFcShort( 0x220 ),	/* Offset= 544 (1062) */
/* 520 */	NdrFcShort( 0xffff ),	/* Offset= -1 (519) */
/* 522 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 524 */	NdrFcShort( 0x4 ),	/* 4 */
/* 526 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 528 */	NdrFcShort( 0x0 ),	/* 0 */
/* 530 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 532 */	0x0 , 
			0x0,		/* 0 */
/* 534 */	NdrFcLong( 0x0 ),	/* 0 */
/* 538 */	NdrFcLong( 0x0 ),	/* 0 */
/* 542 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 544 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 546 */	NdrFcShort( 0x4 ),	/* 4 */
/* 548 */	NdrFcShort( 0x0 ),	/* 0 */
/* 550 */	NdrFcShort( 0x1 ),	/* 1 */
/* 552 */	NdrFcShort( 0x0 ),	/* 0 */
/* 554 */	NdrFcShort( 0x0 ),	/* 0 */
/* 556 */	0x13, 0x0,	/* FC_OP */
/* 558 */	NdrFcShort( 0xfdfc ),	/* Offset= -516 (42) */
/* 560 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 562 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 564 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 566 */	NdrFcShort( 0x8 ),	/* 8 */
/* 568 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 570 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 572 */	NdrFcShort( 0x4 ),	/* 4 */
/* 574 */	NdrFcShort( 0x4 ),	/* 4 */
/* 576 */	0x11, 0x0,	/* FC_RP */
/* 578 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (522) */
/* 580 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 582 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 584 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 586 */	NdrFcShort( 0x0 ),	/* 0 */
/* 588 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 590 */	NdrFcShort( 0x0 ),	/* 0 */
/* 592 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 594 */	0x0 , 
			0x0,		/* 0 */
/* 596 */	NdrFcLong( 0x0 ),	/* 0 */
/* 600 */	NdrFcLong( 0x0 ),	/* 0 */
/* 604 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 608 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 610 */	0x0 , 
			0x0,		/* 0 */
/* 612 */	NdrFcLong( 0x0 ),	/* 0 */
/* 616 */	NdrFcLong( 0x0 ),	/* 0 */
/* 620 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 622 */	NdrFcShort( 0xff2c ),	/* Offset= -212 (410) */
/* 624 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 626 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 628 */	NdrFcShort( 0x8 ),	/* 8 */
/* 630 */	NdrFcShort( 0x0 ),	/* 0 */
/* 632 */	NdrFcShort( 0x6 ),	/* Offset= 6 (638) */
/* 634 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 636 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 638 */	
			0x11, 0x0,	/* FC_RP */
/* 640 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (584) */
/* 642 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 644 */	NdrFcShort( 0x0 ),	/* 0 */
/* 646 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 648 */	NdrFcShort( 0x0 ),	/* 0 */
/* 650 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 652 */	0x0 , 
			0x0,		/* 0 */
/* 654 */	NdrFcLong( 0x0 ),	/* 0 */
/* 658 */	NdrFcLong( 0x0 ),	/* 0 */
/* 662 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 666 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 668 */	0x0 , 
			0x0,		/* 0 */
/* 670 */	NdrFcLong( 0x0 ),	/* 0 */
/* 674 */	NdrFcLong( 0x0 ),	/* 0 */
/* 678 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 680 */	NdrFcShort( 0xff04 ),	/* Offset= -252 (428) */
/* 682 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 684 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 686 */	NdrFcShort( 0x8 ),	/* 8 */
/* 688 */	NdrFcShort( 0x0 ),	/* 0 */
/* 690 */	NdrFcShort( 0x6 ),	/* Offset= 6 (696) */
/* 692 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 694 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 696 */	
			0x11, 0x0,	/* FC_RP */
/* 698 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (642) */
/* 700 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 702 */	NdrFcShort( 0x4 ),	/* 4 */
/* 704 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 706 */	NdrFcShort( 0x0 ),	/* 0 */
/* 708 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 710 */	0x0 , 
			0x0,		/* 0 */
/* 712 */	NdrFcLong( 0x0 ),	/* 0 */
/* 716 */	NdrFcLong( 0x0 ),	/* 0 */
/* 720 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 722 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 724 */	NdrFcShort( 0x4 ),	/* 4 */
/* 726 */	NdrFcShort( 0x0 ),	/* 0 */
/* 728 */	NdrFcShort( 0x1 ),	/* 1 */
/* 730 */	NdrFcShort( 0x0 ),	/* 0 */
/* 732 */	NdrFcShort( 0x0 ),	/* 0 */
/* 734 */	0x13, 0x0,	/* FC_OP */
/* 736 */	NdrFcShort( 0x1d8 ),	/* Offset= 472 (1208) */
/* 738 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 740 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 742 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 744 */	NdrFcShort( 0x8 ),	/* 8 */
/* 746 */	NdrFcShort( 0x0 ),	/* 0 */
/* 748 */	NdrFcShort( 0x6 ),	/* Offset= 6 (754) */
/* 750 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 752 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 754 */	
			0x11, 0x0,	/* FC_RP */
/* 756 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (700) */
/* 758 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 760 */	NdrFcLong( 0x2f ),	/* 47 */
/* 764 */	NdrFcShort( 0x0 ),	/* 0 */
/* 766 */	NdrFcShort( 0x0 ),	/* 0 */
/* 768 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 770 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 772 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 774 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 776 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 778 */	NdrFcShort( 0x1 ),	/* 1 */
/* 780 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 782 */	NdrFcShort( 0x4 ),	/* 4 */
/* 784 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 786 */	0x0 , 
			0x0,		/* 0 */
/* 788 */	NdrFcLong( 0x0 ),	/* 0 */
/* 792 */	NdrFcLong( 0x0 ),	/* 0 */
/* 796 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 798 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 800 */	NdrFcShort( 0x10 ),	/* 16 */
/* 802 */	NdrFcShort( 0x0 ),	/* 0 */
/* 804 */	NdrFcShort( 0xa ),	/* Offset= 10 (814) */
/* 806 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 808 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 810 */	NdrFcShort( 0xffcc ),	/* Offset= -52 (758) */
/* 812 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 814 */	
			0x13, 0x20,	/* FC_OP [maybenull_sizeis] */
/* 816 */	NdrFcShort( 0xffd8 ),	/* Offset= -40 (776) */
/* 818 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 820 */	NdrFcShort( 0x4 ),	/* 4 */
/* 822 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 824 */	NdrFcShort( 0x0 ),	/* 0 */
/* 826 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 828 */	0x0 , 
			0x0,		/* 0 */
/* 830 */	NdrFcLong( 0x0 ),	/* 0 */
/* 834 */	NdrFcLong( 0x0 ),	/* 0 */
/* 838 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 840 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 842 */	NdrFcShort( 0x4 ),	/* 4 */
/* 844 */	NdrFcShort( 0x0 ),	/* 0 */
/* 846 */	NdrFcShort( 0x1 ),	/* 1 */
/* 848 */	NdrFcShort( 0x0 ),	/* 0 */
/* 850 */	NdrFcShort( 0x0 ),	/* 0 */
/* 852 */	0x13, 0x0,	/* FC_OP */
/* 854 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (798) */
/* 856 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 858 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 860 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 862 */	NdrFcShort( 0x8 ),	/* 8 */
/* 864 */	NdrFcShort( 0x0 ),	/* 0 */
/* 866 */	NdrFcShort( 0x6 ),	/* Offset= 6 (872) */
/* 868 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 870 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 872 */	
			0x11, 0x0,	/* FC_RP */
/* 874 */	NdrFcShort( 0xffc8 ),	/* Offset= -56 (818) */
/* 876 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 878 */	NdrFcShort( 0x8 ),	/* 8 */
/* 880 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 882 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 884 */	NdrFcShort( 0x10 ),	/* 16 */
/* 886 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 888 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 890 */	0x0,		/* 0 */
			NdrFcShort( 0xfff1 ),	/* Offset= -15 (876) */
			0x5b,		/* FC_END */
/* 894 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 896 */	NdrFcShort( 0x18 ),	/* 24 */
/* 898 */	NdrFcShort( 0x0 ),	/* 0 */
/* 900 */	NdrFcShort( 0xa ),	/* Offset= 10 (910) */
/* 902 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 904 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 906 */	NdrFcShort( 0xffe8 ),	/* Offset= -24 (882) */
/* 908 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 910 */	
			0x11, 0x0,	/* FC_RP */
/* 912 */	NdrFcShort( 0xfeb8 ),	/* Offset= -328 (584) */
/* 914 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 916 */	NdrFcShort( 0x1 ),	/* 1 */
/* 918 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 920 */	NdrFcShort( 0x0 ),	/* 0 */
/* 922 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 924 */	0x0 , 
			0x0,		/* 0 */
/* 926 */	NdrFcLong( 0x0 ),	/* 0 */
/* 930 */	NdrFcLong( 0x0 ),	/* 0 */
/* 934 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 936 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 938 */	NdrFcShort( 0x8 ),	/* 8 */
/* 940 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 942 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 944 */	NdrFcShort( 0x4 ),	/* 4 */
/* 946 */	NdrFcShort( 0x4 ),	/* 4 */
/* 948 */	0x13, 0x20,	/* FC_OP [maybenull_sizeis] */
/* 950 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (914) */
/* 952 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 954 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 956 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 958 */	NdrFcShort( 0x2 ),	/* 2 */
/* 960 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 962 */	NdrFcShort( 0x0 ),	/* 0 */
/* 964 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 966 */	0x0 , 
			0x0,		/* 0 */
/* 968 */	NdrFcLong( 0x0 ),	/* 0 */
/* 972 */	NdrFcLong( 0x0 ),	/* 0 */
/* 976 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 978 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 980 */	NdrFcShort( 0x8 ),	/* 8 */
/* 982 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 984 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 986 */	NdrFcShort( 0x4 ),	/* 4 */
/* 988 */	NdrFcShort( 0x4 ),	/* 4 */
/* 990 */	0x13, 0x20,	/* FC_OP [maybenull_sizeis] */
/* 992 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (956) */
/* 994 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 996 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 998 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 1000 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1002 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1004 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1006 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1008 */	0x0 , 
			0x0,		/* 0 */
/* 1010 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1014 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1018 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1020 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 1022 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1024 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 1026 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 1028 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1030 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1032 */	0x13, 0x20,	/* FC_OP [maybenull_sizeis] */
/* 1034 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (998) */
/* 1036 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 1038 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1040 */	
			0x1b,		/* FC_CARRAY */
			0x7,		/* 7 */
/* 1042 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1044 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1046 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1048 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1050 */	0x0 , 
			0x0,		/* 0 */
/* 1052 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1056 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1060 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 1062 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 1064 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1066 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 1068 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 1070 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1072 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1074 */	0x13, 0x20,	/* FC_OP [maybenull_sizeis] */
/* 1076 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (1040) */
/* 1078 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 1080 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1082 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 1084 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1086 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1088 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1090 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 1092 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1094 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 1096 */	NdrFcShort( 0xffd8 ),	/* -40 */
/* 1098 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1100 */	0x0 , 
			0x0,		/* 0 */
/* 1102 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1106 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1110 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1112 */	NdrFcShort( 0xffe2 ),	/* Offset= -30 (1082) */
/* 1114 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1116 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1118 */	NdrFcShort( 0x28 ),	/* 40 */
/* 1120 */	NdrFcShort( 0xffe2 ),	/* Offset= -30 (1090) */
/* 1122 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1122) */
/* 1124 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1126 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1128 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1130 */	NdrFcShort( 0xfd5c ),	/* Offset= -676 (454) */
/* 1132 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1134 */	
			0x13, 0x0,	/* FC_OP */
/* 1136 */	NdrFcShort( 0xfeae ),	/* Offset= -338 (798) */
/* 1138 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1140 */	0x1,		/* FC_BYTE */
			0x5c,		/* FC_PAD */
/* 1142 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1144 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 1146 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1148 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 1150 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1152 */	0xb,		/* FC_HYPER */
			0x5c,		/* FC_PAD */
/* 1154 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1156 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 1158 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1160 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 1162 */	
			0x13, 0x0,	/* FC_OP */
/* 1164 */	NdrFcShort( 0xfbcc ),	/* Offset= -1076 (88) */
/* 1166 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1168 */	NdrFcShort( 0xfb80 ),	/* Offset= -1152 (16) */
/* 1170 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1172 */	NdrFcShort( 0xfd06 ),	/* Offset= -762 (410) */
/* 1174 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1176 */	NdrFcShort( 0xfd14 ),	/* Offset= -748 (428) */
/* 1178 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1180 */	NdrFcShort( 0xfd22 ),	/* Offset= -734 (446) */
/* 1182 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1184 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1186) */
/* 1186 */	
			0x13, 0x0,	/* FC_OP */
/* 1188 */	NdrFcShort( 0x14 ),	/* Offset= 20 (1208) */
/* 1190 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 1192 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1194 */	0x6,		/* FC_SHORT */
			0x1,		/* FC_BYTE */
/* 1196 */	0x1,		/* FC_BYTE */
			0x8,		/* FC_LONG */
/* 1198 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 1200 */	
			0x13, 0x0,	/* FC_OP */
/* 1202 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (1190) */
/* 1204 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1206 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 1208 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 1210 */	NdrFcShort( 0x20 ),	/* 32 */
/* 1212 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1214 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1214) */
/* 1216 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1218 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1220 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1222 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1224 */	NdrFcShort( 0xfb9e ),	/* Offset= -1122 (102) */
/* 1226 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1228 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1230 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1232 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1234 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1236 */	NdrFcShort( 0xfb8e ),	/* Offset= -1138 (98) */

			0x0
        }
    };

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = 
        {
            
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            },
            {
            VARIANT_UserSize
            ,VARIANT_UserMarshal
            ,VARIANT_UserUnmarshal
            ,VARIANT_UserFree
            }

        };



/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: ISimENG, ver. 0.0,
   GUID={0x32610836,0xD66A,0x4A57,{0x83,0xD5,0xEA,0x50,0xEC,0xB0,0xB7,0xBB}} */

#pragma code_seg(".orpc")
static const unsigned short ISimENG_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    48,
    108,
    144,
    180,
    216,
    264
    };

static const MIDL_STUBLESS_PROXY_INFO ISimENG_ProxyInfo =
    {
    &Object_StubDesc,
    BusEmulation__MIDL_ProcFormatString.Format,
    &ISimENG_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO ISimENG_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    BusEmulation__MIDL_ProcFormatString.Format,
    &ISimENG_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(14) _ISimENGProxyVtbl = 
{
    &ISimENG_ProxyInfo,
    &IID_ISimENG,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* ISimENG::SendMessage */ ,
    (void *) (INT_PTR) -1 /* ISimENG::RegisterClient */ ,
    (void *) (INT_PTR) -1 /* ISimENG::UnregisterClient */ ,
    (void *) (INT_PTR) -1 /* ISimENG::ConnectNode */ ,
    (void *) (INT_PTR) -1 /* ISimENG::DisconnectNode */ ,
    (void *) (INT_PTR) -1 /* ISimENG::GetTimeModeMapping */ ,
    (void *) (INT_PTR) -1 /* ISimENG::GetCurrentStatus */
};


static const PRPC_STUB_FUNCTION ISimENG_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _ISimENGStubVtbl =
{
    &IID_ISimENG,
    &ISimENG_ServerInfo,
    14,
    &ISimENG_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    BusEmulation__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x60001, /* Ndr library version */
    0,
    0x801026e, /* MIDL Version 8.1.622 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };

const CInterfaceProxyVtbl * const _BusEmulation_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_ISimENGProxyVtbl,
    0
};

const CInterfaceStubVtbl * const _BusEmulation_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_ISimENGStubVtbl,
    0
};

PCInterfaceName const _BusEmulation_InterfaceNamesList[] = 
{
    "ISimENG",
    0
};

const IID *  const _BusEmulation_BaseIIDList[] = 
{
    &IID_IDispatch,
    0
};


#define _BusEmulation_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _BusEmulation, pIID, n)

int __stdcall _BusEmulation_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_BusEmulation_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo BusEmulation_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _BusEmulation_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _BusEmulation_StubVtblList,
    (const PCInterfaceName * ) & _BusEmulation_InterfaceNamesList,
    (const IID ** ) & _BusEmulation_BaseIIDList,
    & _BusEmulation_IID_Lookup, 
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_) */

