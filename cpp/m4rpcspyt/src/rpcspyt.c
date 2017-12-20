
#include <windows.h>
#include <stdio.h>
#include "m4rpcspyt_dll.hpp"
#include "winmain.h"

const char szDLLName[] = { "RPCSpyT" };

unsigned char   g_szProtocolSequence[16] = { "ncalrpc" } ;;
unsigned char   g_szNetworkAddress[256] = { "\0" };
unsigned char   g_szEndpoint[256]   = { "rpcspy" };
unsigned char * g_szUuid            = NULL;
unsigned char * g_szOptions         = NULL;
unsigned char * g_szStringBinding   = NULL;

long g_lLogLevel = IDI_INFO;

HANDLE g_hPrivateHeap;

char g_szDebugFile[1024];

handle_t hRPCSpy;


short LogEvent_S( 
    /* [in] */ handle_t ai_hBinding,
    /* [in] */ long lIcon,
    /* [string][in] */ unsigned char __RPC_FAR *pszSource,
    /* [string][in] */ unsigned char __RPC_FAR *pszOpCode,
    /* [string][in] */ unsigned char __RPC_FAR *pszObjType,
    /* [string][in] */ unsigned char __RPC_FAR *pszOrigin,
    /* [string][in] */ unsigned char __RPC_FAR *pszSink,
    /* [string][in] */ unsigned char __RPC_FAR *pszBrief,
    /* [string][in] */ unsigned char __RPC_FAR *pszText);


//____________________________________________________________________

M4_DECL_M4RPCSPYT short WINAPI LogEvent(
    /* [in] */ long lIcon,
    /* [string][in] */ unsigned char __RPC_FAR *pszSource,
    /* [string][in] */ unsigned char __RPC_FAR *pszBrief,
    /* [string][in] */ unsigned char __RPC_FAR *pszText)

  {
  short iRetVal;
  RPC_STATUS lResult = RPC_S_OK ;
  unsigned char* pucProtocol      = (unsigned char*)g_szProtocolSequence ;
  unsigned char* pucserverAddress = NULL ;
  unsigned char* pucStringBinding = NULL ;	
  RPC_BINDING_HANDLE pBinding = NULL ;

  // bg 115613
  lResult = RpcStringBindingCompose( 0, pucProtocol, pucserverAddress, 0, 0, &pucStringBinding ) ;
  if( lResult != RPC_S_OK )
  {
	  return -1 ;
  }

  lResult = RpcBindingFromStringBinding( pucStringBinding, &pBinding ) ;
  if( lResult != RPC_S_OK )
  {
	  return -1 ;
  }
  // bg 115613
  
  __try
    {
    iRetVal = lIcon>=g_lLogLevel ? LogEvent_S( pBinding, lIcon, pszSource, "?", "?", "?", "?", pszBrief, pszText) : 0;
    }
  __except (EXCEPTION_EXECUTE_HANDLER)
    {
    OutputDebugString ("RPC exception while calling LogEvent.\n");
    }

  return iRetVal;
  }

//____________________________________________________________________

M4_DECL_M4RPCSPYT short WINAPI LogEventEx(
    /* [in] */ long lIcon,
    /* [string][in] */ unsigned char __RPC_FAR *pszSource,
    /* [string][in] */ unsigned char __RPC_FAR *pszOpCode,
    /* [string][in] */ unsigned char __RPC_FAR *pszObjType,
    /* [string][in] */ unsigned char __RPC_FAR *pszOrigin,
    /* [string][in] */ unsigned char __RPC_FAR *pszSink,
    /* [string][in] */ unsigned char __RPC_FAR *pszBrief,
    /* [string][in] */ unsigned char __RPC_FAR *pszText)

  {
  short iRetVal;
  RPC_STATUS lResult = RPC_S_OK ;
  unsigned char* pucProtocol      = (unsigned char*)g_szProtocolSequence ;
  unsigned char* pucserverAddress = NULL ;
  unsigned char* pucStringBinding = NULL ;	
  RPC_BINDING_HANDLE pBinding = NULL ;

  // bg 115613
  lResult = RpcStringBindingCompose( 0, pucProtocol, pucserverAddress, 0, 0, &pucStringBinding ) ;
  if( lResult != RPC_S_OK )
  {
	  return -1 ;
  }

  lResult = RpcBindingFromStringBinding( pucStringBinding, &pBinding ) ;
  if( lResult != RPC_S_OK )
  {
	  return -1 ;
  }
  // bg 115613
  
  __try
    {
    iRetVal = lIcon>=g_lLogLevel ? LogEvent_S( pBinding, lIcon, pszSource, pszOpCode, pszObjType, pszOrigin, pszSink, pszBrief, pszText) : 0;
    }
  __except (EXCEPTION_EXECUTE_HANDLER)
    {
    OutputDebugString ("RPC exception while calling LogEvent.\n");
    }

  return iRetVal;
  }

//____________________________________________________________________

M4_DECL_M4RPCSPYT short WINAPI SetProtocolSequence ( const unsigned char* szProtSeq )
  {
  short iRetVal=0;

  if ( szProtSeq==NULL )
    iRetVal = -1;
  else if ( lstrlen(szProtSeq)>15 )
    iRetVal = -1;
  else
    lstrcpy ( g_szProtocolSequence, szProtSeq );

  return iRetVal;
  }

//____________________________________________________________________

M4_DECL_M4RPCSPYT unsigned char* WINAPI GetProtocolSequence ( void )
  {
  return g_szProtocolSequence;
  }

//____________________________________________________________________

M4_DECL_M4RPCSPYT short WINAPI SetNetAddr ( const unsigned char* szNetAddr )
  {
  short iRetVal=0;

  if ( szNetAddr==NULL )
    iRetVal = -1;
  else if ( lstrlen(szNetAddr)>255 )
    iRetVal = -1;
  else
    lstrcpy ( g_szNetworkAddress, szNetAddr );

  return iRetVal;
  }

//____________________________________________________________________

M4_DECL_M4RPCSPYT unsigned char* WINAPI GetNetAddr ( void )
  {
  return g_szNetworkAddress;
  }

//____________________________________________________________________

M4_DECL_M4RPCSPYT short WINAPI SetEndPoint ( const unsigned char* szEndPoint )
  {
  short iRetVal=0;

  if ( szEndPoint==NULL )
    iRetVal = -1;
  else if ( lstrlen(szEndPoint)>255 )
    iRetVal = -1;
  else
    lstrcpy ( g_szEndpoint, szEndPoint );

  return iRetVal;
  }

//____________________________________________________________________

M4_DECL_M4RPCSPYT unsigned char* WINAPI GetEndPoint ( void )
  {
  return g_szEndpoint;
  }

//____________________________________________________________________

M4_DECL_M4RPCSPYT short WINAPI GetDebugFile ( /* out */ char* szDebugFile, short* piDebugFile )
  {
  lstrcpy ( szDebugFile, g_szDebugFile );
  
  if (NULL!=piDebugFile) *piDebugFile = lstrlen(g_szDebugFile);

  return 0;
  }

//____________________________________________________________________

M4_DECL_M4RPCSPYT short WINAPI SetDebugFile ( const char* szFilePath )
  {
  short iRetVal=0;

  if ( NULL==szFilePath )
    *g_szDebugFile = 0;
  else if ( lstrlen(szFilePath)>1023 )
    iRetVal = -1;
  else
    lstrcpy ( g_szDebugFile, szFilePath );

  return iRetVal;
  }

//____________________________________________________________________

M4_DECL_M4RPCSPYT void WINAPI OutDebug ( const char* sz, long cb )
  {
  HANDLE hFile;
  DWORD  cbWritten;

  if ( *g_szDebugFile==0 )
    OutputDebugString (sz);
  else    
    {
    if (-1l==cb) cb = lstrlen(sz);
    hFile = CreateFile ( g_szDebugFile, GENERIC_WRITE, FILE_SHARE_READ, NULL,
                         OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL );
    if ( INVALID_HANDLE_VALUE==hFile )
      OutputDebugString (sz);
    else
      {
      SetFilePointer ( hFile, 0l, NULL, FILE_END );
      WriteFile ( hFile, sz, cb, &cbWritten, NULL );
      CloseHandle ( hFile );
      }
    }
  }

//____________________________________________________________________

M4_DECL_M4RPCSPYT short WINAPI SetLogLevel ( long lLevel )
  {
  g_lLogLevel = lLevel;

  return 0;
  }

//____________________________________________________________________

M4_DECL_M4RPCSPYT long WINAPI GetLogLevel ( void )
  {
  return g_lLogLevel;
  }

//____________________________________________________________________

M4_DECL_M4RPCSPYT RPC_STATUS WINAPI BindSpyServer ( DWORD dwEndPointProcess )
  {
  char szBuffer[256];
  RPC_STATUS status;

  // Se concatena el identificador del proceso padre (RAMDL) al endpoint
  sprintf(szBuffer, "%d", dwEndPointProcess);
  strcat(g_szEndpoint, szBuffer);

  hRPCSpy = NULL;
  status = RpcStringBindingCompose(NULL, g_szProtocolSequence, g_szNetworkAddress, g_szEndpoint, NULL, &g_szStringBinding);
  if ( status!=RPC_S_OK )
    MessageBox ( NULL, "Error while executing RpcStringBindingCompose(...), String representation for UUID is invalid.", szDLLName, MB_OK|MB_ICONSTOP );
  else
    {
    status = RpcBindingFromStringBinding(g_szStringBinding, &hRPCSpy);
    if (status!=RPC_S_OK)
      {
      wsprintf ( szBuffer, "Error code %i while executing RpcBindingFromStringBinding(..).", status );              
      MessageBox ( NULL, szBuffer, szDLLName, MB_OK|MB_ICONSTOP );
      }
    }
  return status;
  }

//____________________________________________________________________

M4_DECL_M4RPCSPYT RPC_STATUS WINAPI UnbindSpyServer ( void )
  {
  RPC_STATUS status=RPC_S_OK;

  if (NULL!=g_szStringBinding)
    {
    status = RpcStringFree (&g_szStringBinding);
    g_szStringBinding = NULL;
    }
  if (NULL!=hRPCSpy) 
    {
    status = RpcBindingFree(&hRPCSpy);
    hRPCSpy=NULL;
    }

  /* Bug 0172768
  Se deja el endpoint sin id de proceso para que la siguiente vez se vuelva a concatenar
  */
  strcpy(g_szEndpoint, "rpcspy");

  return status;
  }

//____________________________________________________________________

M4_DECL_M4RPCSPYT BOOL WINAPI DLLEntry ( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
  {
  static  unsigned long cClients=0ul;
  char szBuffer[256];

  switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH: /* The DLL is being mapped into the process's address space. */
      DisableThreadLibraryCalls ( hinstDLL );
      if ( 0ul==cClients++ )
        {
        g_hPrivateHeap = HeapCreate ( 0l, 4095l, 65535l );
        if (NULL==g_hPrivateHeap)
          {
          wsprintf ( szBuffer, "Error code %i while executing HeapCreate(..).", GetLastError() );
          MessageBox ( NULL, szBuffer, szDLLName, MB_OK|MB_ICONSTOP );
          }
        }
      break;
    case DLL_PROCESS_DETACH  : /* The DLL is being unmapped from the process's address space. */
      if ( 0ul==--cClients && NULL!=hRPCSpy )
        {
        UnbindSpyServer ();
        if (NULL!=g_hPrivateHeap) HeapDestroy (g_hPrivateHeap);
        }
      break;
    }
  return TRUE;
  }

/*********************************************************************/
/*                 MIDL allocate and free                            */
/*********************************************************************/

void __RPC_FAR * __RPC_API midl_user_allocate(size_t len)
  {
  return HeapAlloc(g_hPrivateHeap, 0l, len);
  }

void __RPC_API midl_user_free(void __RPC_FAR * ptr)
  {
  HeapFree(g_hPrivateHeap, 0l, ptr);
  }