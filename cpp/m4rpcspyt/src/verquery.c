/****************************************/
/* Module : SACARINO.C                  */
/* Version: 1.00.00.                    */
/* Copyright (c) 1996, Sergio Montoro.  */
/****************************************/


#include <windows.h> /* FROM WINUSER32 IMPORT HeapAlloc, HIWORD, LOWORD, ...; */
#include <winver.h>  /* IMPORT GetFileVersionInfo; */
#include "m4rpcspyt_dll.hpp"


/*___________________________________________________________*/


BOOL GetFileFixedInfo ( LPTSTR pszPath, VS_FIXEDFILEINFO* pFixedInfoOut )
  {
  BOOL  bRetVal;
  UINT  uLen;
  DWORD dwZero;
  DWORD cbVerInfo;
  void* pVerInfo;
  VS_FIXEDFILEINFO* pFixedInfo;

  cbVerInfo = GetFileVersionInfoSize ( pszPath, &dwZero );

  if ( 0==cbVerInfo )
	bRetVal = FALSE;
  else
    {
	pVerInfo = HeapAlloc ( GetProcessHeap(), HEAP_NO_SERIALIZE, cbVerInfo );
	if ( NULL==pVerInfo )
	  bRetVal = FALSE;
	else
	  {
      GetFileVersionInfo ( pszPath, 0, cbVerInfo, pVerInfo );
      VerQueryValue ( pVerInfo, "\\", (LPVOID*) &pFixedInfo, &uLen );
	  MoveMemory ( pFixedInfoOut, pFixedInfo, sizeof(VS_FIXEDFILEINFO) );
	  HeapFree ( GetProcessHeap(), HEAP_NO_SERIALIZE, pVerInfo );
      bRetVal = TRUE;
	  }
    }
  return bRetVal;
  }


/*___________________________________________________________*/

M4_DECL_M4RPCSPYT BOOL WINAPI GetFileFixedVersion ( LPTSTR pszPath, WORD* pw1st, WORD* pw2nd, WORD* pw3rd, WORD* pw4th )
  {
  BOOL bHasInfo;
  VS_FIXEDFILEINFO VerInfo;

  bHasInfo = GetFileFixedInfo ( pszPath, &VerInfo );

  if ( bHasInfo )
    {
    if ( pw1st!=NULL ) *pw1st = HIWORD(VerInfo.dwFileVersionMS);
    if ( pw2nd!=NULL ) *pw2nd = LOWORD(VerInfo.dwFileVersionMS);
    if ( pw3rd!=NULL ) *pw3rd = HIWORD(VerInfo.dwFileVersionLS);
    if ( pw4th!=NULL ) *pw4th = LOWORD(VerInfo.dwFileVersionLS);
    }

  return bHasInfo;
  }

/*___________________________________________________________*/

M4_DECL_M4RPCSPYT short WINAPI GetFileDescription ( LPTSTR pszPath, LPTSTR pszLocale_CodePage, LPTSTR pszBuffer, short cbBuffer )
  {
  short  iRetVal;
  BOOL   bHasInfo;
  LPVOID lpDesc;
  UINT   cbDesc;
  DWORD dwZero;
  DWORD cbVerInfo;
  void* pVerInfo;
  char szSubBlock[128];

  cbVerInfo = GetFileVersionInfoSize ( pszPath, &dwZero );

  if ( 0==cbVerInfo )
	iRetVal = 0;
  else
    {
	pVerInfo = HeapAlloc ( GetProcessHeap(), HEAP_NO_SERIALIZE, cbVerInfo );
	if ( NULL==pVerInfo )
	  iRetVal = 0;
	else
	  {
      wsprintf (szSubBlock, "\\StringFileInfo\\%s\\FileDescription", pszLocale_CodePage);
      GetFileVersionInfo ( pszPath, 0, cbVerInfo, pVerInfo );
      bHasInfo=VerQueryValue (pVerInfo, szSubBlock, &lpDesc, &cbDesc);
      if ( bHasInfo )
        {
        lstrcpyn (pszBuffer, lpDesc, cbBuffer);
        iRetVal = cbDesc;
        }
      else
        iRetVal = 0;
	  HeapFree ( GetProcessHeap(), HEAP_NO_SERIALIZE, pVerInfo );
      }
    }

  return iRetVal;
  }