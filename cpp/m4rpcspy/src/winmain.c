#include <stdio.h>
#include <windows.h>
#include <shellapi.h>
#include <commctrl.h>
#include <commdlg.h>  
#include <sqlext.h>
#include <cderr.h>
#include <richedit.h>

#include <ssdll.h>

#include "m4rpcspy.h"
#include "winmain.h"
#include "rpcspymsgs.h"



extern	int	M4CGetRegistryValue( const char* ai_pccKey, const char* ai_pccValue, char* ao_pcData, size_t ai_iSize, size_t* ao_piLength ) ;


/********************/
/* Global variables */
/********************/

#define c_TB_BUTTON_COUNT 9


// Window handles
HINSTANCE g_hInstance;
HWND      g_hHiddenTopMost;
HWND      g_hParent;
HWND      g_hSpread;
HWND      g_hRichBox;
HWND      g_hToolBar;
HWND      g_hDlgAbort;

BOOL      g_bUserAbort;

// Shell vars.
NOTIFYICONDATA g_stIconData;

TBBUTTON  g_TBButtons[c_TB_BUTTON_COUNT];

// RPC handles
unsigned char   g_szProtocolSequence[16]= { "ncalrpc" };
unsigned char   g_szEndpoint       [256]= { "rpcspy"  };
unsigned char * g_szSecurity = NULL;
unsigned int    g_cMinCalls  = 1;
unsigned int    g_cMaxCalls  = 20;
unsigned int    g_fDontWait  = TRUE;

/* Maximum number of rows to keep in memory */
long         g_lRowLimit  = 500;

long       g_lLogLevel = IDI_TRACE ;
const char szAppName[] = {"RCPSpy"};
char       szResBuffer[MAX_RES_BUFFER + 1];


// Inicialización del skin
int	g_iSkinInitialized = 0 ;





//=================================================================================
//
// Obtiene un mensaje de texto en el lenguaje del thread
//
// @param ai_iMessage
//
//		Parámetro de entrada con el número del mensaje de texto
//
// @param ai_lpBuffer
//
//		Parámetro de salida con la cadena del mensaje de texto o cadena vacía
//			si ha habido algún error
//
// @param ai_nSize
//
//		Parámetro de entrada con el tamaño del buffer con la cadena de salida
//
// @return
//
//		El número de caracteres del mensaje de texto sin el 0 final
//		o 0 si ha habido un error
//
// Pasos
//		Se inicializa el parámetro de salida
//		Se obtiene el locale actual
//		Como módulo se pone 0 porque coge el actual
//		Obtiene el texto del mensaje con la función del sistema correspondiente
//		Si no se ha obtenido el mensaje se intenta en el lenguaje que se pueda
//		Si no se ha obtenido el mensaje se devuelve 0
//		Si se ha obtenido el mensaje correctamente se le quita el retorno de
//			carro final si lo tiene
//		Se devuelve la longitud del mensaje
//
//=================================================================================

static	int	_loadString( int ai_iMessage, LPTSTR ai_lpBuffer, DWORD ai_nSize )
{

	LCID	iLocale = 0 ;
	DWORD	dwFlags = 0 ;
	DWORD	dwResult = 0 ;

	
	*ai_lpBuffer = '\0' ;

	iLocale = GetThreadLocale() ;

	dwFlags = FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS ;
	dwResult = FormatMessage( dwFlags, 0, ai_iMessage, iLocale, ai_lpBuffer, ai_nSize, NULL ) ;

	if( dwResult <= 0 )
	{
		iLocale = 0 ;
		dwResult = FormatMessage( dwFlags, 0, ai_iMessage, iLocale, ai_lpBuffer, ai_nSize, NULL ) ;

		if( dwResult <= 0 )
		{
			return( 0 ) ;
		}
	}

	if( dwResult >= 2 && ai_lpBuffer[ dwResult - 2 ] == '\r' && ai_lpBuffer[ dwResult - 1 ] == '\n' )
	{
		dwResult -= 2 ;
		ai_lpBuffer[ dwResult ] = '\0' ;
	}

	return( dwResult ) ;
}




//___________________________________________________________________________

short LogEvent_S (  handle_t ai_hBinding,
	long lPicture,
	unsigned char* szSource,
	unsigned char* szOpCode,
	unsigned char* szObjType,
	unsigned char* szOrigin,
	unsigned char* szSink,
	unsigned char* szTitle ,
	unsigned char* szText )
  {
#ifndef _WIN64
  /* Main function for logging an event into the spread */

  SS_COORD      lLastRow;
  SS_CELLTYPE   CellType;
  SYSTEMTIME    stTime;
  char          szDate[64];
  char          szTime[64];
  char          szDateTime[128];
  unsigned char szNil[1] = { '\0' };
  HBITMAP       hBitmap;

  if ( lPicture>=g_lLogLevel )
    {
    GetSystemTime (&stTime);

    if ( NULL==szSource ) szSource=szNil;
    if ( NULL==szTitle  ) szTitle =szNil;
    if ( NULL==szText   ) szText  =szNil;

    lLastRow = SSGetMaxRows(g_hSpread) + 1;

    /* If spread if full then clear it before continuing inserting */
    if (lLastRow>g_lRowLimit)
      {
      SSSetMaxRows (g_hSpread, 0);
      SendMessage ( g_hRichBox, WM_SETTEXT, 0, (LPARAM) "" );
      lLastRow = 1l;
      }

    SSSetMaxRows (g_hSpread, lLastRow);

    if ( lPicture>=IDI_TRACE && lPicture<=IDI_CRIT )
	{
      hBitmap = LoadBitmap(g_hInstance, (LPCSTR) lPicture);
	}
    else  
	{
      hBitmap = LoadBitmap(g_hInstance, (LPCSTR) IDI_QUEST);
	}

	/* Bug  0168558
	Se limita la longitud a 32KB porque el spread no admite más
	*/
	if( lstrlen( szSource ) > 32000 )
	{
		szSource[ 32000 ] = '\0';
	}

	if( lstrlen( szTitle ) > 32000 )
	{
		szTitle[ 32000 ] = '\0';
	}

	if( lstrlen( szText ) > 32000 )
	{
		szText[ 32000 ] = '\0';
	}

    /* Set cell types for different columns */
    SSSetTypePictureHandle(g_hSpread, &CellType, VPS_BMP|VPS_CENTER, hBitmap, 0, TRUE);
    SSSetCellType(g_hSpread, COL_ICON, lLastRow, &CellType);

    SSSetTypeEdit(g_hSpread, &CellType, ES_AUTOHSCROLL | ES_LEFT, lstrlen(szSource), SS_CHRSET_CHR, SS_CASE_NOCASE);
    SSSetCellType(g_hSpread, COL_SOURCE, lLastRow, &CellType);

    SSSetTypeEdit(g_hSpread, &CellType, ES_AUTOHSCROLL | ES_LEFT, lstrlen(szTitle), SS_CHRSET_CHR, SS_CASE_NOCASE);
    SSSetCellType(g_hSpread, COL_BRIEF, lLastRow, &CellType);

    SSSetTypeEdit(g_hSpread, &CellType, ES_AUTOHSCROLL | ES_LEFT, lstrlen(szText), SS_CHRSET_CHR, SS_CASE_NOCASE);
    SSSetCellType(g_hSpread, COL_TEXT, lLastRow, &CellType);

    GetDateFormat (LOCALE_SYSTEM_DEFAULT, DATE_SHORTDATE, &stTime, NULL, szDate, 63);
    GetTimeFormat (LOCALE_SYSTEM_DEFAULT, TIME_FORCE24HOURFORMAT, &stTime, NULL, szTime, 63);
    wsprintf ( szDateTime, "%s %s", szDate, szTime );

    SSSetData ( g_hSpread, COL_DATE  , lLastRow, szDateTime );
    SSSetData ( g_hSpread, COL_SOURCE, lLastRow, szSource   );
    SSSetData ( g_hSpread, COL_BRIEF , lLastRow, szTitle    );
    SSSetData ( g_hSpread, COL_TEXT  , lLastRow, szText     );

    SSSetLockRange(g_hSpread, COL_ICON, lLastRow, COL_TEXT, lLastRow, TRUE);

    } /* end if lPicture>=g_lLogLevel */
#endif
  return 0;
}


//___________________________________________________________________________

/*
Mira el locale del RAMDL de la rama de registro y lo aplica
*/
int	SetLocale( void )
{

	int		iResult = -1 ;
	BOOL	bSet = TRUE ;
	int		iLocale = 0 ;
	size_t	iLength = 0 ;
	char	acBuffer[ 1024 + 1 ] ;


	iResult = M4CGetRegistryValue( "SOFTWARE\\Meta4\\PeopleNet\\RAMDL", "Locale", acBuffer, 1024, &iLength ) ;

	if( iResult != 0 )
	{
		return( -1 ) ;
	}

	sscanf( acBuffer, "%x", &iLocale ) ;
	bSet = SetThreadLocale( iLocale ) ;

	if( bSet == FALSE )
	{
		return( -1 ) ;
	}

	return( 0 ) ;
}



//___________________________________________________________________________

RPC_STATUS StartUpRPCserver()
  {
  RPC_STATUS status ;
  
  RPC_BINDING_VECTOR *pBindVector = NULL;
  char szBuffer[256];

	status = RpcServerUseProtseqEp(g_szProtocolSequence, g_cMaxCalls, g_szEndpoint, g_szSecurity);
	if (status)
	{
		wsprintf ( szBuffer, "Error: RpcServerUseProtseqEp returned status %ld.\n", status );
		OutputDebugString ( szBuffer );
	}
	else
	{
		status = RpcServerRegisterIf(RPCSPY_v2_0_s_ifspec, NULL, NULL);
		if (status)
		{
			wsprintf ( szBuffer, "Error: RpcServerRegisterIf returned status %ld.\n", status );
			OutputDebugString ( szBuffer );
		}
		else
		{
			status = RpcServerInqBindings(&pBindVector);
			if (status)
			{
				wsprintf ( szBuffer, "Error: RpcServerInqBindings returned status %ld.\n", status );
				OutputDebugString ( szBuffer );
			}
			else
			{
				// bg 115613
				status = RpcEpRegister( RPCSPY_v2_0_s_ifspec, pBindVector, 0, 0 ) ;				
				if (status)
				{
					wsprintf ( szBuffer, "Error: RpcEpRegister returned status %ld.\n", status );
					OutputDebugString ( szBuffer );
				}
				else
				{
					status = RpcServerListen(g_cMinCalls, g_cMaxCalls, g_fDontWait);
					if (status)
					{
						wsprintf ( szBuffer, "Error: RpcServerListen returned status %ld.\n", status );
						OutputDebugString ( szBuffer );
					}
				}
			}
		}
    }

  return status;
  }

//___________________________________________________________________________

RPC_STATUS StopRPCserver()
  {
  RPC_STATUS status;

  status = RpcMgmtStopServerListening ( NULL );
  status = RpcServerUnregisterIf ( RPCSPY_v2_0_s_ifspec, NULL, TRUE );
  
  return status;
  }

//___________________________________________________________________________

void InitPrintFormat( SS_PRINTFORMAT* pPrintFormat )
  {
  pPrintFormat->fDrawBorder = FALSE;
  pPrintFormat->fDrawShadows = FALSE;
  pPrintFormat->fDrawColors = FALSE;
  pPrintFormat->fShowGrid = FALSE;
  pPrintFormat->fShowColHeaders = TRUE;
  pPrintFormat->fShowRowHeaders = TRUE;
  pPrintFormat->fUseDataMax = TRUE;
  pPrintFormat->dPrintType = 0;
  pPrintFormat->x1CellRange = 0;
  pPrintFormat->y1CellRange = 0;
  pPrintFormat->x2CellRange = 0;
  pPrintFormat->y2CellRange = 0; 
  pPrintFormat->nPageStart = 0;
  pPrintFormat->nPageEnd = 0;
  pPrintFormat->fMarginLeft = (float)0.0;  
  pPrintFormat->fMarginTop = (float)0.0;
  pPrintFormat->fMarginRight = (float)0.0;
  pPrintFormat->fMarginBottom = (float)0.0;
  pPrintFormat->hDCPrinter = 0;
  }

//___________________________________________________________________________

/*
Se actualizan los identificadores de los menus con el texto en el idioma correcto
*/
void	InitMenu( HWND hParentWnd )
{

	BOOL	bResult = TRUE ;
	int		iLength = 0 ;
	HMENU	hMenu = 0 ;


	hMenu = GetMenu( hParentWnd ) ;

	if( hMenu != 0 )
	{
		iLength = _loadString( IDM_FILE, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			bResult = ModifyMenu( hMenu, 0, MF_BYPOSITION | MF_STRING, IDM_FILE, szResBuffer ) ;
		}

		iLength = _loadString( IDM_EDIT, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			bResult = ModifyMenu( hMenu, 1, MF_BYPOSITION | MF_STRING, IDM_EDIT, szResBuffer ) ;
		}

		iLength = _loadString( IDM_OPTIONS, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			bResult = ModifyMenu( hMenu, 2, MF_BYPOSITION | MF_STRING, IDM_OPTIONS, szResBuffer ) ;
		}

		iLength = _loadString( IDM_HELP, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			bResult = ModifyMenu( hMenu, 3, MF_BYPOSITION | MF_STRING, IDM_HELP, szResBuffer ) ;
		}

		iLength = _loadString( IDM_SAVE, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			bResult = ModifyMenu( hMenu, IDM_SAVE, MF_BYCOMMAND | MF_STRING, IDM_SAVE, szResBuffer ) ;
		}

		iLength = _loadString( IDM_PRINT, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			bResult = ModifyMenu( hMenu, IDM_PRINT, MF_BYCOMMAND | MF_STRING, IDM_PRINT, szResBuffer ) ;
		}

		iLength = _loadString( IDM_EXIT, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			bResult = ModifyMenu( hMenu, IDM_EXIT, MF_BYCOMMAND | MF_STRING, IDM_EXIT, szResBuffer ) ;
		}

		iLength = _loadString( IDM_DEL, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			bResult = ModifyMenu( hMenu, IDM_DEL, MF_BYCOMMAND | MF_STRING, IDM_DEL, szResBuffer ) ;
		}

		iLength = _loadString( IDM_CLEARALL, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			bResult = ModifyMenu( hMenu, IDM_CLEARALL, MF_BYCOMMAND | MF_STRING, IDM_CLEARALL, szResBuffer ) ;
		}

		iLength = _loadString( IDM_OPTIONS, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			bResult = ModifyMenu( hMenu, IDM_OPTIONS, MF_BYCOMMAND | MF_STRING, IDM_OPTIONS, szResBuffer ) ;
		}

		iLength = _loadString( IDM_ABOUT, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			bResult = ModifyMenu( hMenu, IDM_ABOUT, MF_BYCOMMAND | MF_STRING, IDM_ABOUT, szResBuffer ) ;
		}
	}
}

//___________________________________________________________________________

void InitTBButtons ( void )
  {
  g_TBButtons[0].iBitmap  = 0;
  g_TBButtons[0].idCommand= 0;
  g_TBButtons[0].fsState  = (BYTE) 0;
  g_TBButtons[0].fsStyle  = (BYTE) TBSTYLE_SEP;
  g_TBButtons[0].dwData   = 0l;
  g_TBButtons[0].iString  = 0;

  g_TBButtons[1].iBitmap  = 0;
  g_TBButtons[1].idCommand= IDM_CLEARALL;
  g_TBButtons[1].fsState  = (BYTE) TBSTATE_ENABLED;
  g_TBButtons[1].fsStyle  = (BYTE) TBSTYLE_BUTTON;
  g_TBButtons[1].dwData   = 0l;
  g_TBButtons[1].iString  = 0;

  g_TBButtons[2].iBitmap  = 1;
  g_TBButtons[2].idCommand= IDM_DEL;
  g_TBButtons[2].fsState  = (BYTE) TBSTATE_ENABLED;
  g_TBButtons[2].fsStyle  = (BYTE) TBSTYLE_BUTTON;
  g_TBButtons[2].dwData   = 0l;
  g_TBButtons[2].iString  = 0;

  g_TBButtons[3].iBitmap  = 0;
  g_TBButtons[3].idCommand= 0;
  g_TBButtons[3].fsState  = (BYTE) 0;
  g_TBButtons[3].fsStyle  = (BYTE) TBSTYLE_SEP;
  g_TBButtons[3].dwData   = 0l;
  g_TBButtons[3].iString  = 0;

  g_TBButtons[4].iBitmap  = 2;
  g_TBButtons[4].idCommand= IDM_SAVE;
  g_TBButtons[4].fsState  = (BYTE) TBSTATE_ENABLED;
  g_TBButtons[4].fsStyle  = (BYTE) TBSTYLE_BUTTON;
  g_TBButtons[4].dwData   = 0l;
  g_TBButtons[4].iString  = 0;

  g_TBButtons[5].iBitmap  = 3;
  g_TBButtons[5].idCommand= IDM_PRINT;
  g_TBButtons[5].fsState  = (BYTE) TBSTATE_ENABLED;
  g_TBButtons[5].fsStyle  = (BYTE) TBSTYLE_BUTTON;
  g_TBButtons[5].dwData   = 0l;
  g_TBButtons[5].iString  = 0;

  g_TBButtons[6].iBitmap  = 0;
  g_TBButtons[6].idCommand= 0;
  g_TBButtons[6].fsState  = (BYTE) 0;
  g_TBButtons[6].fsStyle  = (BYTE) TBSTYLE_SEP;
  g_TBButtons[6].dwData   = 0l;
  g_TBButtons[6].iString  = 0;

  g_TBButtons[7].iBitmap  = 4;
  g_TBButtons[7].idCommand= IDM_OPTIONS ;
  g_TBButtons[7].fsState  = (BYTE) TBSTATE_ENABLED;
  g_TBButtons[7].fsStyle  = (BYTE) TBSTYLE_BUTTON;
  g_TBButtons[7].dwData   = 0l;
  g_TBButtons[7].iString  = 0;

  g_TBButtons[8].iBitmap  = 0;
  g_TBButtons[8].idCommand= 0;
  g_TBButtons[8].fsState  = (BYTE) 0;
  g_TBButtons[8].fsStyle  = (BYTE) TBSTYLE_SEP;
  g_TBButtons[8].dwData   = 0l;
  g_TBButtons[8].iString  = 0;

  }

//___________________________________________________________________________

DWORD InitSpread ( HWND /* in */ hParentWnd, HWND* /* out */ hSpread )
  {
  DWORD dwRetVal = 0;
#ifndef _WIN64
  COLORREF cBackground;
  COLORREF cForeground;
//  COLORREF cShadowColor;
//  COLORREF cShadowText;
//  COLORREF cShadowDark;
//  COLORREF cShadowLight;

  *hSpread = CreateWindow(SS_CLASSNAME, "", WS_CHILD|WS_VISIBLE|WS_TABSTOP, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, hParentWnd, (HMENU)IDC_SPREAD, g_hInstance, NULL);

  if ( NULL!= *hSpread )
    {
    /* Set spread columns to 5 and hide the fifth column */
    SSSetMaxCols (*hSpread, 5);
    SSSetMaxRows (*hSpread, 0);
    SSShowCol    (*hSpread, COL_TEXT, FALSE );

    SSSetRowHeaderDisplay(*hSpread, SS_HEADERDISPLAY_BLANK);

    /* Set width of each colum */
    SSSetColWidthInPixels(*hSpread, COL_ICON  , 40);
    SSSetColWidthInPixels(*hSpread, COL_DATE  , 128);
    SSSetColWidthInPixels(*hSpread, COL_SOURCE, 64);

    /* Set column headers */
    _loadString ( IDS_TYPE, szResBuffer, MAX_RES_BUFFER);
    SSSetData ( g_hSpread, COL_ICON, 0, szResBuffer );
    _loadString ( IDS_DATE, szResBuffer, MAX_RES_BUFFER);
    SSSetData ( g_hSpread, COL_DATE, 0, szResBuffer );
    _loadString ( IDS_SOURCE, szResBuffer, MAX_RES_BUFFER);
    SSSetData ( g_hSpread, COL_SOURCE, 0, szResBuffer );
    _loadString ( IDS_BRIEFING, szResBuffer, MAX_RES_BUFFER);
    SSSetData ( g_hSpread, COL_BRIEF, 0, szResBuffer );

	/* Los colores del skin */
	if( g_iSkinInitialized == 1 )
	{
		SSGetGrayAreaColor(g_hSpread, &cBackground, &cForeground);
		SSSetGrayAreaColor(g_hSpread, 0x8000000F, cForeground);

		// Bug 154366
		SSSetGridColor(g_hSpread, 0x80000000);

//		SSGetShadowColor(g_hSpread, &cShadowColor, &cShadowText, &cShadowDark, &cShadowLight);
//		SSSetShadowColor(g_hSpread, 0x8000000F, 0x80000010, 0x80000012, cShadowLight);
	}
    }
  else
  {
    dwRetVal = GetLastError();
  }
#endif
  return dwRetVal;
  }

//___________________________________________________________________________

//___________________________________________________________________________

void RedrawChilds ( const int cx, const int cy, const int iTBHeight )
  {
#ifndef _WIN64
  static int iColWidth = 0;
  const  int cy23 = (2*(cy-iTBHeight))/3;

  if ( 0==iColWidth ) 
  {
	  SSGetColWidthInPixels(g_hSpread,0,&iColWidth);
  }

  MoveWindow ( g_hToolBar, 0, 0, cx, CW_USEDEFAULT, TRUE );
  MoveWindow ( g_hRichBox, 0, iTBHeight+cy23, cx, cy-cy23-iTBHeight, TRUE );
  MoveWindow ( g_hSpread , 0, iTBHeight, cx, cy23, FALSE );
  SSSetColWidthInPixels(g_hSpread, COL_BRIEF, cx-232-iColWidth-GetSystemMetrics(SM_CXVSCROLL));
#endif
  }

//___________________________________________________________________________

void ApplyOptions (HWND hDlg)
  {
  long lLogLevel;
  LRESULT iIndex;
  unsigned char szProtocolSequence[16];
  unsigned char szEndpoint       [256];
  unsigned char szMaxRows[6];

  for (lLogLevel=IDI_TRACE; lLogLevel<=IDI_QUEST; lLogLevel++)
  {
    if (BST_CHECKED==SendMessage (GetDlgItem(hDlg, lLogLevel), BM_GETCHECK, 0, 0))
	{
      g_lLogLevel = lLogLevel;
	  break ;
	}
  }
    
  iIndex = SendMessage ( GetDlgItem(hDlg, 1001), CB_GETCURSEL, 0, 0 );
  iIndex = SendMessage ( GetDlgItem(hDlg, 1001), CB_GETLBTEXT, iIndex, (LPARAM) szProtocolSequence );
  iIndex = SendMessage ( GetDlgItem(hDlg, 1002), WM_GETTEXT, 255, (LPARAM) szEndpoint );

  if ( (lstrcmp(szProtocolSequence, g_szProtocolSequence)!=0) || (lstrcmp(szEndpoint, g_szEndpoint)!=0))
    {
    _loadString ( IDS_APPLY_WARN, szResBuffer, MAX_RES_BUFFER);
    if (IDYES==MessageBox(g_hParent, szResBuffer, szAppName, MB_YESNO|MB_ICONEXCLAMATION|MB_DEFBUTTON2|MB_APPLMODAL)) 
      {
      StopRPCserver();
      lstrcpy(g_szProtocolSequence, szProtocolSequence);
      lstrcpy(g_szEndpoint, szEndpoint);
      StartUpRPCserver();
      }
    }
  
  SendMessage ( GetDlgItem(hDlg, 1003), WM_GETTEXT, 5, (LPARAM) szMaxRows );
  g_lRowLimit = atol(szMaxRows);
  }

//___________________________________________________________________________

LRESULT CALLBACK AboutDlgProc (HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
  int	iLength = 0 ;

  switch (uMsg)
  {
    case WM_INITDIALOG:
      /* Se inicializan los caption */

		iLength = _loadString( IDC_ABOUT_CAPTION, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			SendMessage( hDlg, WM_SETTEXT, 0, (LPARAM) szResBuffer ) ;
		}

		iLength = _loadString( IDC_OK, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			SendMessage( GetDlgItem( hDlg, IDC_OK ), WM_SETTEXT, 0, (LPARAM) szResBuffer ) ;
		}

		iLength = _loadString( IDC_ABOUT_TEXT, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			SendMessage( GetDlgItem( hDlg, IDC_ABOUT_TEXT ), WM_SETTEXT, 0, (LPARAM) szResBuffer ) ;
		}

      break;

    case WM_COMMAND:
      if (LOWORD(wParam)==IDC_OK || LOWORD(wParam)==IDCANCEL)
        {
        EndDialog(hDlg,0);
        return TRUE;
        }
      break;
  }
  return FALSE;
}

//___________________________________________________________________________

LRESULT CALLBACK AbortDlgProc (HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
  int	iLength = 0 ;

  switch (uMsg)
  {
    case WM_INITDIALOG:
      /* Se inicializan los caption */

		iLength = _loadString( IDC_PRINT_CAPTION, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			SendMessage( hDlg, WM_SETTEXT, 0, (LPARAM) szResBuffer ) ;
		}

		iLength = _loadString( IDC_CANCEL, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			SendMessage( GetDlgItem( hDlg, IDC_CANCEL ), WM_SETTEXT, 0, (LPARAM) szResBuffer ) ;
		}

      break;

    case WM_COMMAND:
      g_bUserAbort = TRUE;
      EnableWindow (GetParent(hDlg), TRUE);
      DestroyWindow (hDlg);
      g_hDlgAbort = 0;
      return TRUE;
      break;
  }
  return FALSE;
}

//___________________________________________________________________________

LRESULT CALLBACK ConnectDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
  {
  int     iLength = 0 ;
  HWND    hCtrl;
  short   iCtrl;
  char    szMaxRows[6];

  switch (iMsg)
    {
    case WM_INITDIALOG:
      /* Se inicializan los caption */

		iLength = _loadString( IDC_OPTIONS_CAPTION, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			SendMessage( hDlg, WM_SETTEXT, 0, (LPARAM) szResBuffer ) ;
		}

		iLength = _loadString( IDC_OPTIONS, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			SendMessage( GetDlgItem( hDlg, IDC_OPTIONS ), WM_SETTEXT, 0, (LPARAM) szResBuffer ) ;
		}

		iLength = _loadString( IDC_SEQUENCE, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			SendMessage( GetDlgItem( hDlg, IDC_SEQUENCE ), WM_SETTEXT, 0, (LPARAM) szResBuffer ) ;
		}

		iLength = _loadString( IDC_ENDPOINT, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			SendMessage( GetDlgItem( hDlg, IDC_ENDPOINT ), WM_SETTEXT, 0, (LPARAM) szResBuffer ) ;
		}

		iLength = _loadString( IDC_LOG_LEVEL, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			SendMessage( GetDlgItem( hDlg, IDC_LOG_LEVEL ), WM_SETTEXT, 0, (LPARAM) szResBuffer ) ;
		}

		iLength = _loadString( IDC_TRACE, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			SendMessage( GetDlgItem( hDlg, IDI_TRACE ), WM_SETTEXT, 0, (LPARAM) szResBuffer ) ;
		}

		iLength = _loadString( IDC_EXCL, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			SendMessage( GetDlgItem( hDlg, IDI_EXCL ), WM_SETTEXT, 0, (LPARAM) szResBuffer ) ;
		}

		iLength = _loadString( IDC_INFO, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			SendMessage( GetDlgItem( hDlg, IDI_INFO ), WM_SETTEXT, 0, (LPARAM) szResBuffer ) ;
		}

		iLength = _loadString( IDC_CRIT, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			SendMessage( GetDlgItem( hDlg, IDI_CRIT ), WM_SETTEXT, 0, (LPARAM) szResBuffer ) ;
		}

		iLength = _loadString( IDC_QUEST, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			SendMessage( GetDlgItem( hDlg, IDI_QUEST ), WM_SETTEXT, 0, (LPARAM) szResBuffer ) ;
		}

		iLength = _loadString( IDC_MAX_ROWS, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			SendMessage( GetDlgItem( hDlg, IDC_MAX_ROWS ), WM_SETTEXT, 0, (LPARAM) szResBuffer ) ;
		}

		iLength = _loadString( IDC_OK, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			SendMessage( GetDlgItem( hDlg, IDC_OK ), WM_SETTEXT, 0, (LPARAM) szResBuffer ) ;
		}

		iLength = _loadString( IDC_CANCEL, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			SendMessage( GetDlgItem( hDlg, IDC_CANCEL ), WM_SETTEXT, 0, (LPARAM) szResBuffer ) ;
		}

		iLength = _loadString( IDC_APPLY, szResBuffer, MAX_RES_BUFFER ) ;
		if( iLength > 0 ) {
			SendMessage( GetDlgItem( hDlg, IDC_APPLY ), WM_SETTEXT, 0, (LPARAM) szResBuffer ) ;
		}

      /* Initialize RPC connectivity ComboBox */
      hCtrl = GetDlgItem(hDlg, 1001);
      SendMessage ( hCtrl, CB_ADDSTRING, 0, (LPARAM) "ncacn_dnet_nsp" );
      SendMessage ( hCtrl, CB_ADDSTRING, 1, (LPARAM) "ncacn_ip_tcp" );
      SendMessage ( hCtrl, CB_ADDSTRING, 2, (LPARAM) "ncacn_nb_nb" );
      SendMessage ( hCtrl, CB_ADDSTRING, 3, (LPARAM) "ncacn_nb_tcp" );
      SendMessage ( hCtrl, CB_ADDSTRING, 4, (LPARAM) "ncacn_np" );
      SendMessage ( hCtrl, CB_ADDSTRING, 5, (LPARAM) "ncacn_spx" );
      SendMessage ( hCtrl, CB_ADDSTRING, 6, (LPARAM) "ncalrpc" );
      SendMessage ( hCtrl, CB_SELECTSTRING, 0, (LPARAM) g_szProtocolSequence );

      /* Initialize RPC connectivity EditBox */
      SendMessage ( GetDlgItem(hDlg, 1002), WM_SETTEXT, 0, (LPARAM) g_szEndpoint );

      /* Limit Max. Rows to 9999 */
      hCtrl = GetDlgItem(hDlg, 1003);
      wsprintf (szMaxRows, "%li", g_lRowLimit);
      SendMessage ( hCtrl, EM_LIMITTEXT, 4, 0l);
      SendMessage ( hCtrl, WM_SETTEXT, 0, (LPARAM) szMaxRows);

      /* Set the proper radio button for Log Level */
      SendMessage ( GetDlgItem(hDlg, g_lLogLevel), BM_SETCHECK, 1, 0 );
      return TRUE;
	  break;
    case WM_COMMAND:
      switch (LOWORD(wParam))
        {
        case IDC_APPLY :
          ApplyOptions(hDlg);
          return TRUE;
        case IDC_OK:
          ApplyOptions(hDlg);
          EndDialog(hDlg,0);
          return TRUE;
        case IDC_CANCEL:
        case IDCANCEL:
          EndDialog(hDlg,0);
          return TRUE;
        case IDI_TRACE:
        case IDI_INFO :
        case IDI_EXCL :
        case IDI_CRIT :
        case IDI_QUEST:
          if (BN_CLICKED==HIWORD(wParam))
		  {
            for (iCtrl=IDI_TRACE; iCtrl<=IDI_QUEST; ++iCtrl )
			{
              if (iCtrl!=LOWORD(wParam))
			  {
                SendMessage ( GetDlgItem(hDlg, iCtrl), BM_SETCHECK, 0, 0 );
			  }
			}
		  }
          break;
        default:
          return FALSE;
		  break;
        }
	  break;
    default:
      return FALSE;
	  break;
    }
  return FALSE;
  }

//___________________________________________________________________________

void ProcessCDError(DWORD dwErrorCode, HWND hWnd)
  {
  /* Processes errors from the common dialog functions.
     This function is called whenever a common dialog function
     fails.  The CommonDialogExtendedError() value is passed to
     the function which maps the error value to a string table.
     The string is loaded and displayed for the user. */ 

  WORD  wStringID;
  TCHAR szBuffer[MAX_RES_BUFFER];

  switch(dwErrorCode)
    {
	case CDERR_DIALOGFAILURE:   wStringID=IDS_DIALOGFAILURE;   break;
	case CDERR_STRUCTSIZE:      wStringID=IDS_STRUCTSIZE;      break;
	case CDERR_INITIALIZATION:  wStringID=IDS_INITIALIZATION;  break;
	case CDERR_NOTEMPLATE:      wStringID=IDS_NOTEMPLATE;      break;
	case CDERR_NOHINSTANCE:     wStringID=IDS_NOHINSTANCE;     break;
	case CDERR_LOADSTRFAILURE:  wStringID=IDS_LOADSTRFAILURE;  break;
	case CDERR_FINDRESFAILURE:  wStringID=IDS_FINDRESFAILURE;  break;
	case CDERR_LOADRESFAILURE:  wStringID=IDS_LOADRESFAILURE;  break;
	case CDERR_LOCKRESFAILURE:  wStringID=IDS_LOCKRESFAILURE;  break;
	case CDERR_MEMALLOCFAILURE: wStringID=IDS_MEMALLOCFAILURE; break;
	case CDERR_MEMLOCKFAILURE:  wStringID=IDS_MEMLOCKFAILURE;  break;
	case CDERR_NOHOOK:          wStringID=IDS_NOHOOK;          break;
	case PDERR_SETUPFAILURE:    wStringID=IDS_SETUPFAILURE;    break;
	case PDERR_PARSEFAILURE:    wStringID=IDS_PARSEFAILURE;    break;
	case PDERR_RETDEFFAILURE:   wStringID=IDS_RETDEFFAILURE;   break;
	case PDERR_LOADDRVFAILURE:  wStringID=IDS_LOADDRVFAILURE;  break;
	case PDERR_GETDEVMODEFAIL:  wStringID=IDS_GETDEVMODEFAIL;  break;
	case PDERR_INITFAILURE:     wStringID=IDS_INITFAILURE;     break;
	case PDERR_NODEVICES:       wStringID=IDS_NODEVICES;       break;
	case PDERR_NODEFAULTPRN:    wStringID=IDS_NODEFAULTPRN;    break;
	case PDERR_DNDMMISMATCH:    wStringID=IDS_DNDMMISMATCH;    break;
	case PDERR_CREATEICFAILURE: wStringID=IDS_CREATEICFAILURE; break;
	case PDERR_PRINTERNOTFOUND: wStringID=IDS_PRINTERNOTFOUND; break;
	case CFERR_NOFONTS:         wStringID=IDS_NOFONTS;         break;
	case FNERR_SUBCLASSFAILURE: wStringID=IDS_SUBCLASSFAILURE; break;
	case FNERR_INVALIDFILENAME: wStringID=IDS_INVALIDFILENAME; break;
	case FNERR_BUFFERTOOSMALL:  wStringID=IDS_BUFFERTOOSMALL;  break;

	case 0:   //User may have hit CANCEL or we got a *very* random error
	  wStringID=0;
      break;
	default:
	    wStringID=IDS_UNKNOWNERROR;
    }

  if (0!=wStringID)
    {
    _loadString( wStringID, szBuffer, sizeof(szBuffer));
    MessageBox(hWnd, szBuffer, szAppName, MB_OK|MB_ICONSTOP|MB_APPLMODAL);
    }    
  }

//___________________________________________________________________________

void SaveSpread( void )
  {
#ifndef _WIN64
  OPENFILENAME of;
  char         szTitle[MAX_RES_BUFFER];
  TCHAR        szFile[MAX_PATH] = { '\0' };

  _loadString ( IDS_TTSAVE, szTitle, MAX_RES_BUFFER );
  
  // Fill in the OPENFILENAME structure to support a template and hook.
  of.lStructSize       = sizeof(OPENFILENAME);
  of.hwndOwner         = g_hParent;
  of.hInstance         = g_hInstance;
  of.lpstrFilter       = NULL;
  of.lpstrCustomFilter = NULL;
  of.nMaxCustFilter    = 0;
  of.nFilterIndex      = 0;
  of.lpstrFile         = szFile;
  of.nMaxFile          = sizeof(szFile);
  of.lpstrFileTitle    = NULL;
  of.nMaxFileTitle     = 0;
  of.lpstrInitialDir   = NULL;
  of.lpstrTitle        = szTitle;
  of.nFileOffset       = 0;
  of.nFileExtension    = 0;
  of.lpstrDefExt       = "txt";
  of.lCustData         = 0l;
  of.lpfnHook 		   = NULL;
  of.lpTemplateName    = NULL;
  of.Flags             = OFN_NOREADONLYRETURN|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY ;

  if (GetSaveFileName(&of))
  {
    SSSaveTabFile(g_hSpread, of.lpstrFile);    
  }
  else
  {
    ProcessCDError(CommDlgExtendedError(), g_hParent );	
  }
#endif
  }

//___________________________________________________________________________

void AddTrayIcon ( HWND hWnd )
  {
  g_stIconData.cbSize = sizeof(NOTIFYICONDATA);
  g_stIconData.hWnd   = hWnd;
  g_stIconData.uID    = 0;
  g_stIconData.uFlags = NIF_ICON|NIF_TIP|NIF_MESSAGE;
  g_stIconData.uCallbackMessage = WM_NOTIFYICON;
  g_stIconData.hIcon  = LoadIcon  (g_hInstance, (LPSTR) IDI_SPY);
  lstrcpy ( g_stIconData.szTip, "Event Spy" );
  Shell_NotifyIcon ( NIM_ADD, &g_stIconData );
  }

//___________________________________________________________________________

BOOL EXPORT CALLBACK SSAbortProc (HDC hDC, short nCode)
  {
  MSG Msg;
  
  while (!g_bUserAbort && PeekMessage (&Msg, 0, 0, 0, PM_REMOVE))
    {
    if (!g_hDlgAbort || !IsDialogMessage (g_hDlgAbort, &Msg))
      {
      TranslateMessage (&Msg);
      DispatchMessage (&Msg);
      }
    }

  if(g_bUserAbort) 
  {
	  g_bUserAbort = TRUE;
  }

  return !g_bUserAbort;
  }

//___________________________________________________________________________

short EXPORT CALLBACK SSPrintProc(HDC hDC, short dCommand, short nPageNum, LONG lAppData, LPSS_PRINTINFO lpPrintInfo)
  {
  SIZE sizeText;

  GetTextExtentPoint(hDC, "0", 1, &sizeText);

  switch( dCommand )
  {
    case SS_PRINTCMD_HEADERLEN:
      return (sizeText.cy * 3);
    case SS_PRINTCMD_FOOTERLEN:
      return (sizeText.cy * 3);
    case SS_PRINTCMD_PRINTHEADER:
      TextOut( hDC, 100, 0, "Header 1", 8);
      TextOut(hDC, 100, sizeText.cy, "Header 2", 8);
      TextOut(hDC, 100, sizeText.cy * 2, "Header 3", 8);
      break;
    case SS_PRINTCMD_PRINTFOOTER:
      TextOut(hDC, 100, 0, "Footer 1", 8);
      TextOut(hDC, 100, sizeText.cy, "Footer 2", 8);
      TextOut(hDC, 100, sizeText.cy * 2, "Footer 3", 8);
      break;
  }
  return 0;
}


//___________________________________________________________________________

LRESULT CALLBACK HiddenWndProc (HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
  {
  LRESULT lRetVal;

  switch (iMsg)
    {
    case WM_DESTROY:
      PostQuitMessage(0);
      lRetVal = 0l;
      break;
    default:
      lRetVal = DefWindowProc (hWnd,iMsg,wParam, lParam);
    }
  return lRetVal;
  }

//___________________________________________________________________________

LRESULT CALLBACK WndProc (HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
  {
  static int iTBHeight;
  
  LRESULT lRetVal = 0;
  RECT    rect;
  int     iLength = 0 ;

#ifndef _WIN64
  int     iData;
  int     iBound;
  int     iCount;
  void*   pParam;
  LPSS_CELLCOORD lpCellCoord;
  LPSS_LEAVECELL lpLeaveCell;
  ABORTPROC      lpfnAbortProc;
  SS_PRINTFUNC   lpfnPrintProc;
  FARPROC        lpfnAbortDlgProc;
  SS_PRINTFORMAT PrintFormat;
#endif

  switch (iMsg)
    {
    case WM_CREATE:
      /* Initialize menus */
      InitMenu (hWnd);
      /* Initialize toolbar buttons */
      InitTBButtons ();
      /* Initialize log spread */
      InitSpread (hWnd, &g_hSpread);
      /* Create rich text box for displaying long information associated with each event */
      g_hRichBox = CreateWindow ( "RichEdit", "", WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_VSCROLL|ES_MULTILINE|ES_READONLY, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, hWnd, (HMENU)IDC_RTF, g_hInstance, NULL);
      g_hToolBar = CreateToolbarEx ( hWnd, WS_CHILD|WS_VISIBLE|TBSTYLE_TOOLTIPS, IDC_TOOLBAR, 4, g_hInstance, IDI_TBBITMAP, g_TBButtons, c_TB_BUTTON_COUNT, 16, 16, 16, 16, sizeof(TBBUTTON) );
      GetWindowRect (g_hToolBar, &rect);
      iTBHeight = (int) rect.bottom-rect.top;
      StartUpRPCserver();
      AddTrayIcon(hWnd);
      break;
    case WM_DESTROY:
      Shell_NotifyIcon ( NIM_DELETE, &g_stIconData );
      StopRPCserver();
      DestroyWindow(g_hToolBar);
      DestroyWindow(g_hRichBox);
      DestroyWindow(g_hSpread);
  
      SendMessage (g_hHiddenTopMost, WM_DESTROY, 0, 0L); 
      break;
    case WM_COMMAND:
      switch (LOWORD(wParam))
        {
        case IDM_EXIT: /* Close application */
          SendMessage ( g_hParent, WM_CLOSE, 0 ,0l );
          break;
        case IDM_DEL: /* Delete selected cells */
#ifndef _WIN64
          iCount = 0;
          iBound = SSGetMaxRows(g_hSpread);
          SSSetBool (g_hSpread, SSB_REDRAW, FALSE );
          for (iData=1; iData<=iBound; ++iData )
		  {
            if (SSIsCellInSelection(g_hSpread, 0, iData))
              {
              SSDelRow (g_hSpread, iData);
              iCount++;
              }
		  }
          SSSetMaxRows(g_hSpread, iBound-iCount);
          SSSetBool (g_hSpread, SSB_REDRAW, TRUE );
#endif
          break;
        case IDM_CLEARALL: /* Clear entire spread */
#ifndef _WIN64
          _loadString ( IDS_SURECLR, szResBuffer, MAX_RES_BUFFER);
          /* Display MessageBox asking for confirmation before clearing the spread */
          iData = MessageBox ( g_hParent, szResBuffer, szAppName, MB_YESNO|MB_ICONEXCLAMATION|MB_DEFBUTTON2|MB_APPLMODAL );
          if ( IDYES==iData )
            {
            SSSetMaxRows (g_hSpread, 0);
            SendMessage ( g_hRichBox, WM_SETTEXT, 0, (LPARAM) "" );
            }
#endif
          break;
        case IDM_OPTIONS:
          DialogBox ( g_hInstance, MAKEINTRESOURCE(IDD_CONNECT), g_hParent, ConnectDlgProc );
          break;
        case IDM_SAVE :
          SaveSpread ();
          break;
        case IDM_PRINT:
#ifndef _WIN64
          lpfnPrintProc   = (SS_PRINTFUNC) MakeProcInstance(SSPrintProc, g_hInstance);
          lpfnAbortProc   = (ABORTPROC)    MakeProcInstance(SSAbortProc, g_hInstance);
          lpfnAbortDlgProc= (FARPROC)      MakeProcInstance(AbortDlgProc, hInstance);
       
          InitPrintFormat (&PrintFormat);
          g_bUserAbort = FALSE;


          EnableWindow(hWnd, FALSE);
          g_hDlgAbort = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_ABORT), hWnd, (DLGPROC)lpfnAbortDlgProc);
          ShowWindow(g_hDlgAbort, SW_SHOW);
          SSSetPrintOptions(g_hSpread, &PrintFormat, lpfnPrintProc, 0L);
          SSPrint(g_hSpread, "RPC Spy", &PrintFormat, lpfnPrintProc, 0L, lpfnAbortProc);
          if (!g_bUserAbort)
		  {
            DestroyWindow(g_hDlgAbort);
		  }
          EnableWindow(hWnd, TRUE);
 
          FreeProcInstance(lpfnAbortDlgProc );
          FreeProcInstance(lpfnAbortProc);
          FreeProcInstance(lpfnPrintProc);
#endif
          break;
        case IDM_ABOUT:
          DialogBox ( g_hInstance, MAKEINTRESOURCE(IDD_ABOUT), g_hParent, AboutDlgProc );
          break;
        default:
          lRetVal = DefWindowProc (hWnd,iMsg,wParam, lParam);
        }
      break;
    case WM_ENTERSIZEMOVE:
      lRetVal = DefWindowProc (hWnd,iMsg,wParam, lParam);
      break;
    case WM_EXITSIZEMOVE:
      lRetVal = DefWindowProc (hWnd,iMsg,wParam, lParam);
      break;
    case WM_SIZE:
      if (SIZE_MINIMIZED==wParam) 
	  {
		  ShowWindow (hWnd, SW_HIDE);
	  }
      RedrawChilds (LOWORD(lParam), HIWORD(lParam), iTBHeight);
      break;
    case WM_NOTIFY:
      switch (((LPNMHDR) lParam)->code)
        {
        case TTN_NEEDTEXT:
          ((LPTOOLTIPTEXT) lParam)->hinst = g_hInstance;
          switch (((LPTOOLTIPTEXT) lParam)->hdr.idFrom)
            {
            case IDM_CLEARALL:
              iLength = _loadString( IDS_TTCLEAR, szResBuffer, MAX_RES_BUFFER ) ;
              break;
            case IDM_DEL:
              iLength = _loadString( IDS_TTDEL, szResBuffer, MAX_RES_BUFFER ) ;
              break;
            case IDM_SAVE:
              iLength = _loadString( IDS_TTSAVE, szResBuffer, MAX_RES_BUFFER ) ;
              break;
            case IDM_PRINT:
              iLength = _loadString( IDS_TTPRINT, szResBuffer, MAX_RES_BUFFER ) ;
              break;
            case IDM_OPTIONS:
              iLength = _loadString( IDS_TTOPTION, szResBuffer, MAX_RES_BUFFER ) ;
              break;
            }

            if( iLength > 0 )
			{
              ((LPTOOLTIPTEXT) lParam)->lpszText = szResBuffer;
            }
        }
      break;
    case WM_NOTIFYICON:
      if (WM_LBUTTONDBLCLK==lParam)
        {
        ShowWindow (hWnd, SW_RESTORE);
        SetForegroundWindow(hWnd);
        }
      break;
    case SSM_LEAVECELL:
#ifndef _WIN64
      lpLeaveCell = (LPSS_LEAVECELL) lParam;
      if (NULL!=lpLeaveCell)
        {
        if (lpLeaveCell->RowNew>0 && lpLeaveCell->RowNew<=SSGetMaxRows(g_hSpread))
          {
          iData = SSGetDataLen ( g_hSpread, COL_TEXT, lpLeaveCell->RowNew );
          if ( iData>0 )
            {
            pParam = HeapAlloc ( GetProcessHeap(), 0l, iData+sizeof(char) );
            if ( NULL!=pParam )
              {
              SSGetData   ( g_hSpread, COL_TEXT, lpLeaveCell->RowNew, (LPTSTR) pParam );
              SendMessage ( g_hRichBox, WM_SETTEXT, 0, (LPARAM) pParam );
              HeapFree    ( GetProcessHeap(), 0l, pParam );
              }
            else
			{
              SendMessage ( g_hRichBox, WM_SETTEXT, 0, (LPARAM) "<Text too long for being displayed>" );
			}
            }
          else
		  {
            SendMessage ( g_hRichBox, WM_SETTEXT, 0, (LPARAM) " " );          
		  }
          }
        }  /* end if (NULL!=lpLeaveCell) */   
      lRetVal = DefWindowProc (hWnd,iMsg,wParam,lParam);
#endif
      break;
    case SSM_CLICK:
#ifndef _WIN64
      lpCellCoord = (LPSS_CELLCOORD) lParam;
      if (NULL!=lpCellCoord)
        {
        if ( 0==lpCellCoord->Row )
          {
          if ( lpCellCoord->Col>COL_ICON )
            {
            /* Void */
            }
          }
        else
          {
          iData = SSGetDataLen ( g_hSpread, COL_TEXT, lpCellCoord->Row );
          if ( iData>0 )
            {
            pParam = HeapAlloc ( GetProcessHeap(), 0l, iData+sizeof(char) );
            if ( NULL!=pParam )
              {
              SSGetData   ( g_hSpread, COL_TEXT, lpCellCoord->Row, (LPTSTR) pParam );
              SendMessage ( g_hRichBox, WM_SETTEXT, 0, (LPARAM) pParam );
              HeapFree    ( GetProcessHeap(), 0l, pParam );
              }
            else
			{
              SendMessage ( g_hRichBox, WM_SETTEXT, 0, (LPARAM) "<Text too long for being displayed>" );
			}
            }
          else
		  {
            SendMessage ( g_hRichBox, WM_SETTEXT, 0, (LPARAM) " " );
		  }
          } 
        } /* end if (NULL!=lpCellCoord) */
#endif
      break;
    default:
      lRetVal = DefWindowProc (hWnd,iMsg,wParam, lParam);
    }
  return lRetVal;
  }

//___________________________________________________________________________

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
  {
  MSG        stMsg;
  WNDCLASSEX stWndCls;

  /* Declare a class name for the hidden window */
  static char szHiddenTopMostWndClass[] = { "HiddenTopMostWndClass" };

  // Se cambia el endpoint para que vaya para el proceso que llama
  strcat(g_szEndpoint, szCmdLine);

  SetLocale() ;

  /* Store handle of this instance in a global variable */
  g_hInstance = hInstance;

  LoadLibrary("RICHED32.DLL");

  /* Set properties for Hidden Window Class */
  stWndCls.cbSize       = sizeof(stWndCls);
  stWndCls.style        = CS_HREDRAW | CS_VREDRAW;
  stWndCls.cbClsExtra   = 0;
  stWndCls.cbWndExtra   = 0;
  stWndCls.hInstance    = hInstance;
  stWndCls.hIcon        = LoadIcon  (hInstance, (LPSTR) IDI_SPY);
  stWndCls.hIconSm      = LoadIcon  (hInstance, (LPSTR) IDI_SPY);
  stWndCls.hCursor      = LoadCursor(NULL, IDC_ARROW);
  stWndCls.hbrBackground= (HBRUSH) GetStockObject(WHITE_BRUSH);
  stWndCls.lpszMenuName = MAKEINTRESOURCE(IDM_TOPMENU);

  stWndCls.lpszClassName= szAppName;
  stWndCls.lpfnWndProc  = WndProc;
  RegisterClassEx (&stWndCls);

  stWndCls.lpszClassName= szHiddenTopMostWndClass;
  stWndCls.lpfnWndProc  = HiddenWndProc;
  RegisterClassEx (&stWndCls);

  /* Create the hidden window.
      This window is created with a TOOLWINDOW style so that it does not appear at
      windows taskbar */
  g_hHiddenTopMost = CreateWindowEx( WS_EX_TOOLWINDOW, szHiddenTopMostWndClass, "", 0,
                              CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                              GetDesktopWindow(), NULL, hInstance, NULL );

  _loadString ( IDS_SPYTITLE, szResBuffer, MAX_RES_BUFFER );

#ifdef _WIN64
  _loadString ( IDC_NO_64BITS, szResBuffer + strlen(szResBuffer), (DWORD)(MAX_RES_BUFFER - strlen(szResBuffer)) );
#endif

  /* Create the main visible window as a child of the hidden window,
      using this trick neither of HiddenWindow nor MainWindow appear at
      windows taskbar */
  g_hParent = CreateWindow( szAppName, szResBuffer, WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
                              CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                              g_hHiddenTopMost, NULL, hInstance, NULL );
  ShowWindow  ( g_hParent, iCmdShow );
  UpdateWindow( g_hParent );

  /* Proccess windows incoming messages */

  while ( GetMessage(&stMsg, NULL, 0,0) )
    {
      TranslateMessage(&stMsg);
      DispatchMessage (&stMsg);
    }

  return stMsg.wParam;
  }



/*********************************************************************/
/*                 MIDL allocate and free                            */
/*********************************************************************/

void __RPC_FAR * __RPC_API midl_user_allocate(size_t len)
  {
  return HeapAlloc(GetProcessHeap(), 0l, len);
  }

void __RPC_API midl_user_free(void __RPC_FAR * ptr)
  {
  HeapFree(GetProcessHeap(), 0l, ptr);
  }




