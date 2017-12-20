/* this ALWAYS GENERATED file contains the RPC client stubs */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Jun 10 15:00:01 2010
 */
/* Compiler settings for rpcspy.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )

#include <string.h>
#if defined( _ALPHA_ )
#include <stdarg.h>
#endif

#include "m4rpcspy.h"

#define TYPE_FORMAT_STRING_SIZE   7                                 
#define PROC_FORMAT_STRING_SIZE   75                                

extern unsigned char   g_szProtocolSequence[16];
extern unsigned char   g_szEndpoint[256];


typedef struct _MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } MIDL_PROC_FORMAT_STRING;


extern const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
extern const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;

/* Standard interface: RPCSPY, ver. 2.0,
   GUID={0xeab36df6,0xe585,0x11d1,{0x92,0xc2,0x00,0xc0,0x4f,0xbf,0x81,0x92}} */


static RPC_PROTSEQ_ENDPOINT __RpcProtseqEndpoint[] = 
    {
    {(unsigned char *) g_szProtocolSequence, (unsigned char *) g_szEndpoint}
    };


static const RPC_CLIENT_INTERFACE RPCSPY___RpcClientInterface =
    {
    sizeof(RPC_CLIENT_INTERFACE),
    {{0xeab36df6,0xe585,0x11d1,{0x92,0xc2,0x00,0xc0,0x4f,0xbf,0x81,0x92}},{2,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    0,
    1,
    __RpcProtseqEndpoint,
    0,
    0,
    0
    };
RPC_IF_HANDLE RPCSPY_v2_0_c_ifspec = (RPC_IF_HANDLE)& RPCSPY___RpcClientInterface;

extern const MIDL_STUB_DESC RPCSPY_StubDesc;

static RPC_BINDING_HANDLE RPCSPY__MIDL_AutoBindHandle;


short LogEvent_S( 
    /* [in] */ handle_t ai_hBinding,
    /* [in] */ long lIcon,
    /* [string][in] */ unsigned char __RPC_FAR *pszSource,
    /* [string][in] */ unsigned char __RPC_FAR *pszOpCode,
    /* [string][in] */ unsigned char __RPC_FAR *pszObjType,
    /* [string][in] */ unsigned char __RPC_FAR *pszOrigin,
    /* [string][in] */ unsigned char __RPC_FAR *pszSink,
    /* [string][in] */ unsigned char __RPC_FAR *pszBrief,
    /* [string][in] */ unsigned char __RPC_FAR *pszText)
{

    CLIENT_CALL_RETURN _RetVal;

    
#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pszText);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&RPCSPY_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[0],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&RPCSPY_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[0],
                  ( unsigned char __RPC_FAR * )&ai_hBinding,
                  ( unsigned char __RPC_FAR * )&lIcon,
                  ( unsigned char __RPC_FAR * )&pszSource,
                  ( unsigned char __RPC_FAR * )&pszOpCode,
                  ( unsigned char __RPC_FAR * )&pszObjType,
                  ( unsigned char __RPC_FAR * )&pszOrigin,
                  ( unsigned char __RPC_FAR * )&pszSink,
                  ( unsigned char __RPC_FAR * )&pszBrief,
                  ( unsigned char __RPC_FAR * )&pszText);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&RPCSPY_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[0],
                  ( unsigned char __RPC_FAR * )&ai_hBinding);
#endif
    return ( short  )_RetVal.Simple;
    
}


static const MIDL_STUB_DESC RPCSPY_StubDesc = 
    {
    (void __RPC_FAR *)& RPCSPY___RpcClientInterface,
    MIDL_user_allocate,
    MIDL_user_free,
    &RPCSPY__MIDL_AutoBindHandle,
    0,
    0,
    0,
    0,
    __MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x20000, /* Ndr library version */
    0,
    0x50100a4, /* MIDL Version 5.1.164 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    1,  /* Flags */
    0,  /* Reserved3 */
    0,  /* Reserved4 */
    0   /* Reserved5 */
    };

#if !defined(__RPC_WIN32__) && !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT40_OR_LATER)
#error You need a Windows NT 4.0 or later to run this stub because it uses these features:
#error   -Oif or -Oicf.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure LogEvent_S */

			0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x0 ),	/* 0 */
#ifndef _ALPHA_
/*  8 */	NdrFcShort( 0x28 ),	/* x86, MIPS, PPC Stack size/offset = 40 */
#else
			NdrFcShort( 0x50 ),	/* Alpha Stack size/offset = 80 */
#endif
/* 10 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
#ifndef _ALPHA_
/* 12 */	NdrFcShort( 0x0 ),	/* x86, MIPS, PPC Stack size/offset = 0 */
#else
			NdrFcShort( 0x0 ),	/* Alpha Stack size/offset = 0 */
#endif
/* 14 */	NdrFcShort( 0x8 ),	/* 8 */
/* 16 */	NdrFcShort( 0x6 ),	/* 6 */
/* 18 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x9,		/* 9 */

	/* Parameter ai_hBinding */

/* 20 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 22 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 24 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter lIcon */

/* 26 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
#ifndef _ALPHA_
/* 28 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 30 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszSource */

/* 32 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
#ifndef _ALPHA_
/* 34 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 36 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszOpCode */

/* 38 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
#ifndef _ALPHA_
/* 40 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 42 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszObjType */

/* 44 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
#ifndef _ALPHA_
/* 46 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 48 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszOrigin */

/* 50 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
#ifndef _ALPHA_
/* 52 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 54 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszSink */

/* 56 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
#ifndef _ALPHA_
/* 58 */	NdrFcShort( 0x1c ),	/* x86, MIPS, PPC Stack size/offset = 28 */
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 60 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszBrief */

/* 62 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
#ifndef _ALPHA_
/* 64 */	NdrFcShort( 0x20 ),	/* x86, MIPS, PPC Stack size/offset = 32 */
#else
			NdrFcShort( 0x40 ),	/* Alpha Stack size/offset = 64 */
#endif
/* 66 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter pszText */

/* 68 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 70 */	NdrFcShort( 0x24 ),	/* x86, MIPS, PPC Stack size/offset = 36 */
#else
			NdrFcShort( 0x48 ),	/* Alpha Stack size/offset = 72 */
#endif
/* 72 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

			0x0
        }
    };

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/*  4 */	
			0x22,		/* FC_C_CSTRING */
			0x5c,		/* FC_PAD */

			0x0
        }
    };
