//	==============================================================================
//
//	 (c) Copyright  1991-2008 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            m4systray
//	 File:              procmonitor.hpp
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

#ifndef procmonitor_hpp
#define procmonitor_hpp 1

#include <windows.h>
#include <m4types.hpp>


// -- -------------------------------------------------------------------------------
// -- Client application monitorization class.
// -- -------------------------------------------------------------------------------
class ClProcessMonitor
{

	public:

		ClProcessMonitor( DWORD ai_dwPid, DWORD ai_dwPStart, m4pchar_t ai_pcParameters ) ;
		virtual ~ClProcessMonitor() ;
		
		// -- Closes current user session.
		m4return_t CloseUserSession() ;

		// -- Waits until client application finishes.
		m4return_t WaitProcClosing() ;

	protected:

		DWORD		m_dwPid ;
		DWORD		m_dwPStartup ;
		m4pchar_t	m_pcParameters ;
		HMODULE		m_hModAppClientLib ;

		m4return_t PollProcess( m4bool_t& ao_bIsAlive ) ;

} ;


#endif