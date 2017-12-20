//	==============================================================================
//
//	 (c) Copyright  1991-2008 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            M4ClientAPI
//	 File:              appmonitor.hpp
//	 Project:           m4appclientdll
//	 Author:            Meta Software M.S. , S.A
//	 Date:				081128
//	 Language:          C++
//	 Operating System:  WINDOWS
//	 Design Document:
//
//	 Definition:
//
//	    This module defines client application monitor manager.
//
//	==============================================================================

#ifdef _WINDOWS

#ifndef appmonitorhttp_h
#define appmonitorhttp_h 1

#include <m4types.hpp>
#include <m4string.hpp>
#include "m4appclient_dll.hpp"


// -- -------------------------------------------------------------------------------
// -- Public function for user session disconnection. Entry point for m4systray.exe.
// -- -------------------------------------------------------------------------------
extern "C" M4_DECL_M4APPCLIENT m4return_t DisconnectUserSession( m4pcchar_t ai_pcParameters, m4pchar_t ao_acErrorMsg ) ;


// -- -------------------------------------------------------------------------------
// -- Client application monitor manager class.
// -- -------------------------------------------------------------------------------
class ClAppMonitor
{

	public:

		ClAppMonitor( m4string_t ai_strGWSrv, m4uint32_t ai_uiGWSrvPort, m4string_t ai_strAppSrv, m4uint32_t ai_uiAppSrvPort, m4string_t ai_strServlet, m4bool_t ai_bUseSSL, m4string_t ai_strCredential ) ;
		virtual ~ClAppMonitor() ;

	protected:

		m4string_t	m_strParameters ;
		int			m_hdAppMonitor ;

		m4return_t	SetMonitorParams( m4string_t ai_strGWSrv, m4uint32_t ai_uiGWSrvPort, m4string_t ai_strAppSrv, m4uint32_t ai_uiAppSrvPort, m4string_t ai_strServlet, m4bool_t ai_bUseSSL, m4string_t ai_strCredential ) ;
		m4return_t	GetMonitorPath( const m4pchar_t ao_pcPath, const m4int_t ai_iSize ) ;
		m4return_t	GetProcessInfo( m4pchar_t ao_acPid, m4pchar_t ao_acStartup ) ;

	public:

		// -- Executes the monitor application and returns to the caller.
		m4return_t	StartMonitor() ;

		// -- Stops the monitor application.
		m4return_t	StopMonitor() ;

		// -- Checks if the monitor application is running.
		m4return_t  IsAlive( m4bool_t& ao_bIsAlive ) ;

		// -- Monitor application executable file name.
		static m4pcchar_t ms_pccAppMonitorExeName ;

} ;


#endif

#endif