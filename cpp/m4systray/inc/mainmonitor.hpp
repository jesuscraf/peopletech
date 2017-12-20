//	==============================================================================
//
//	 (c) Copyright  1991-2008 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            m4systray
//	 File:              mainmonitor.hpp
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

#ifndef mainmonitor_hpp
#define mainmonitor_hpp 1


#include <windows.h>


/* -- Redo if want to show tray icon ------------------------------------------------
/* -- >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// -- -------------------------------------------------------------------------------
// -- Meta4 system tray icon wrapper.
// -- -------------------------------------------------------------------------------
class ClSysTrayIcon
{
	public:

		// -- Cttor.
		// --
		// --	ai_hInstance:	Current instance application handle.
		// --	ai_hIcon:		Loaded icon handle.

		ClSysTrayIcon( HINSTANCE ai_hInstance, HICON ai_hIcon ) ;

		// -- Dttor.

		virtual ~ClSysTrayIcon() ;

	
	public:

		static LRESULT CALLBACK m4CBFunc(HWND ai_hWnd, UINT ai_message, WPARAM ai_wParam, LPARAM ai_lParam) ;


	private:

		HINSTANCE			m_hInstance ;
		NOTIFYICONDATA		m_stNIData ;
		HWND				m_hWnd ;
		static const UINT	ms_cuiMessageId ;
} ;
/* -- <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
-- End Redo ---------------------------------------------------------------------- */

#endif