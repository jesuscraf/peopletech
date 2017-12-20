//	==============================================================================
//
//	 (c) Copyright  1991-2008 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            m4systray
//	 File:              mainmonitor.cpp
//	 Project:           m4systray
//	 Author:            Meta Software M.S. , S.A
//	 Date:				24/11/08
//	 Language:          C++
//	 Operating System:  WINDOWS
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	==============================================================================

#include <mainmonitor.hpp>
#include <procmonitor.hpp>
#include <logmonitor.hpp>

#define NUM_REQ_PARAMS 3


/* -- Redo if want to show tray icon ------------------------------------------------
// -- Add user32.lib in that case.
/* -- >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

const UINT ClSysTrayIcon::ms_cuiMessageId = WM_USER + 444 ;


// -- -------------------------------------------------------------------------------
// -- Cttor for class ClSysTrayIcon
// --
// --	ai_hInstance:	Current instance application handle.
// --	ai_hIcon:		Loaded icon handle.
// --
// -- While instantiated the icon will be showed in the system tray.
// -- -------------------------------------------------------------------------------
ClSysTrayIcon::ClSysTrayIcon( HINSTANCE ai_hInstance, HICON ai_hIcon )
{
	BOOL	bRet ;


	m_hWnd = CreateDialog( ai_hInstance, MAKEINTRESOURCE(IDD_M4SYSTRAY), NULL, (DLGPROC)ClSysTrayIcon::m4CBFunc );

	if( NULL == m_hWnd )
	{
		M4PRM_DUMP_ERROR( "SysTray icon: Error creating dialog" ) ;
		M4PRM_DUMP_SYS_ERROR1( "CreateDialog" ) ;
	}
	else
	{
		ZeroMemory( &m_stNIData, sizeof( NOTIFYICONDATA ) ) ;

		m_stNIData.cbSize			= sizeof( NOTIFYICONDATA ) ;
		m_stNIData.uID				= IDI_M4MAIN ;
		m_stNIData.hIcon			= ai_hIcon ;
		m_stNIData.hWnd				= m_hWnd ;
		m_stNIData.uFlags			= NIF_ICON  ;

		bRet = Shell_NotifyIcon( NIM_ADD, &m_stNIData ) ;

		if( FALSE == bRet )
		{
			M4PRM_DUMP_ERROR( "SysTray icon: Error showing icon" ) ;
			M4PRM_DUMP_SYS_ERROR1( "Shell_NotifyIcon" ) ;
		}
	}

}


// -- -------------------------------------------------------------------------------
// -- Dttor for class ClSysTrayIcon
// --
// -- Hides the icon.
// -- -------------------------------------------------------------------------------
ClSysTrayIcon::~ClSysTrayIcon()
{
	BOOL	bRet ;


	bRet = Shell_NotifyIcon( NIM_DELETE, &m_stNIData ) ;

	if( FALSE == bRet )
	{
		M4PRM_DUMP_ERROR( "SysTray icon: Error hiding icon" ) ;
		M4PRM_DUMP_SYS_ERROR1( "Shell_NotifyIcon" ) ;
	}

	DestroyWindow( m_hWnd ) ;

}


// -- -------------------------------------------------------------------------------
// -- Callback function.
// -- -------------------------------------------------------------------------------
LRESULT CALLBACK ClSysTrayIcon::m4CBFunc( HWND ai_hWnd, UINT ai_message, WPARAM ai_wParam, LPARAM ai_lParam )
{
	return true;
}

/* -- <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
-- End Redo ---------------------------------------------------------------------- */


// -- -------------------------------------------------------------------------------
// -- Monitorizes a Meta4 client application given by its pid and startup time. 
// -- When it is closed unexpectedly, sends a session closing request to the 
// -- application server throught the gateway. 
// --
// -- Parameters required:
// -- 
// --	1. Client application pid.
// --	2. Client application start up time.
// --	3. Encrypted communication parameters
// -- -------------------------------------------------------------------------------
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	m4return_t			iRet ;
	ClProcessMonitor*	pProcMonitor = NULL ;
	
	/* -- Redo if want to show tray icon ------------------------------------------------
	/* -- >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	// -- Delete pointer before exit in that case.

	ClSysTrayIcon*		pSysTrayIcon = NULL ;

	// -- Show m4 systray icon
	HICON				hIcon ;

	hIcon = (HICON)LoadImage( hInstance, MAKEINTRESOURCE(IDI_M4MAIN), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR) ;

	if( NULL != hIcon )
	{
		pSysTrayIcon = new ClSysTrayIcon( hInstance, hIcon ) ;
	}
	else
	{
		M4PRM_DUMP_ERROR( "SysTray icon: Error loading icon image" ) ;
		M4PRM_DUMP_SYS_ERROR1( "LoadImage" ) ;
	}
	/* -- <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	-- End Redo ---------------------------------------------------------------------- */


	ClLogMonitor::DumpLog( false, LOG_DEBUG, "Process monitor started" ) ;

	
	// -- Check parameters.
	if( NUM_REQ_PARAMS != __argc - 1 )
	{
		ClLogMonitor::DumpLog( false, LOG_ERROR, "Invalid number of arguments" ) ;
		ClLogMonitor::DumpLog( false, LOG_DEBUG, "Exiting monitor" ) ;

		return 0 ;
	}


	// -- Tracing.
	for( int iIdx = 1; iIdx <= NUM_REQ_PARAMS; iIdx++ )
	{
		ClLogMonitor::DumpLog( false, LOG_DEBUG, "Param #%d: \"%s\"", iIdx, __argv[iIdx] ) ;
	}


	// -- Creates the monitor object.
	pProcMonitor = new ClProcessMonitor( atol(__argv[1]), strtoul(__argv[2], NULL, 0), __argv[3] ) ;


	// -- Monitorizes client application.
	iRet = pProcMonitor->WaitProcClosing() ;

	if( M4_SUCCESS != iRet )
	{
		ClLogMonitor::DumpLog( false, LOG_ERROR, "Error polling client application [%d]", iRet ) ;
	}
	else
	{
		// Waits for last request execution, menu hits for instance.
		Sleep( 2000 ) ;

		// -- Closing client session.
		iRet = pProcMonitor->CloseUserSession() ;

	
		if( M4_SUCCESS == iRet )
		{
			ClLogMonitor::DumpLog( false, LOG_DEBUG, "User session successfully closed" ) ;
		}
		else
		{
			ClLogMonitor::DumpLog( false, LOG_DEBUG, "Error closing session [%d]", iRet ) ;
		}
	}

	
	delete pProcMonitor ;

	// -- Tracing.
	ClLogMonitor::DumpLog( false, LOG_DEBUG, "Process monitor ending" ) ;

	// -- Finishing.
	ClLogMonitor::FinalizeLog() ;


	return 0 ;
}