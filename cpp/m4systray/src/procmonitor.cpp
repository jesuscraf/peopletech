//	==============================================================================
//
//	 (c) Copyright  1991-2008 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            m4systray
//	 File:              procmonitor.cpp
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

#include <procmonitor.hpp>
#include <logmonitor.hpp>
#include <tlhelp32.h>

#define	MAX_NUM_RETRIES 5
// -- --------------------------------------------------------------------------
// -- Meta4 client process monitor application cttor.
// --
// -- Args:
// --
// --	ai_dwPid:			Process Id.
// --	ai_dwPStart:		Process startup time.
// --	ai_pcParameters		Encrypted serialized communication parameters.
// -- --------------------------------------------------------------------------
ClProcessMonitor::ClProcessMonitor( DWORD ai_dwPid, DWORD ai_dwPStart, m4pchar_t ai_pcParameters )
{
	m_dwPid				= ai_dwPid ;
	m_dwPStartup		= ai_dwPStart ;
	m_pcParameters		= ai_pcParameters ;
	m_hModAppClientLib	= NULL ;
}


// -- --------------------------------------------------------------------------
// -- Meta4 client process monitor application dttor.
// -- --------------------------------------------------------------------------
ClProcessMonitor::~ClProcessMonitor()
{
	if( NULL != m_hModAppClientLib )
	{
		FreeLibrary( m_hModAppClientLib ) ;
	}
}


// -- --------------------------------------------------------------------------
// -- Waits until target process closes.
// --
// -- Return: M4_SUCCESS / M4_ERROR
// -- --------------------------------------------------------------------------
m4return_t ClProcessMonitor::WaitProcClosing()
{
	m4return_t	iRet = M4_SUCCESS ;
	m4bool_t	bIsAlive = M4_TRUE ;
	m4uint_t	iDelay = 10000 ;


	while( M4_TRUE == bIsAlive )
	{

		 iRet = PollProcess( bIsAlive ) ;

		 if( M4_SUCCESS != iRet )
		 {
			 return iRet ;
		 }
		
		 if( M4_TRUE == bIsAlive )
		 {
			Sleep( iDelay ) ;
		 }

	}


	return M4_SUCCESS ;
}


// -- --------------------------------------------------------------------------
// -- Checks if target process is alive.
// --
// -- Args:
// --
// --	ao_bIsAlive: Output arg, tells the caller if the target process is
// --				 still alive or not.
// --
// -- Return: M4_SUCCESS / M4_ERROR
// -- --------------------------------------------------------------------------
m4return_t ClProcessMonitor::PollProcess( m4bool_t& ao_bIsAlive )
{
	m4return_t		iRet = M4_SUCCESS ;
	HANDLE			hProcs ;
	PROCESSENTRY32	stProcEntry ;
	BOOL			bRet ;
	HANDLE			hProc ;
	FILETIME		stCreatTime;
	FILETIME		stUnused;

	
	ao_bIsAlive = M4_TRUE ;


	hProcs = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );

	if( INVALID_HANDLE_VALUE == hProcs )
	{
		ClLogMonitor::DumpSysLog( "CreateToolhelp32Snapshot" ) ;

		return M4_ERROR ;
	}


	stProcEntry.dwSize = sizeof( stProcEntry ) ;

	bRet = Process32First( hProcs, &stProcEntry ) ;
	
	if( false == bRet )
	{
		ClLogMonitor::DumpSysLog( "Process32First" ) ;
		CloseHandle (hProcs) ;

		return M4_ERROR ;
	}


	// -- Search for target application.
	while( ( stProcEntry.th32ProcessID != m_dwPid ) && Process32Next( hProcs, &stProcEntry ) ) ;


	// -- Process found.
	if( stProcEntry.th32ProcessID == m_dwPid )
	{

		hProc = OpenProcess( PROCESS_QUERY_INFORMATION, FALSE, m_dwPid ) ;

		if( NULL != hProc )
		{

			// -- Get process creation time.
			bRet = GetProcessTimes( hProc, &stCreatTime, &stUnused, &stUnused, &stUnused ) ;

			if( false != bRet )
			{

				if( stCreatTime.dwLowDateTime == m_dwPStartup )
				{

					ClLogMonitor::DumpLog( true, LOG_DEBUG, "Process running [exe %s] [pid %lu] [stt %lu]", stProcEntry.szExeFile, m_dwPid, m_dwPStartup ) ;
					ao_bIsAlive = M4_TRUE ;
				}
				else
				{
					ClLogMonitor::DumpLog( false, LOG_WARNING, "Process running [exe %s] [pid %lu] [stt %lu]. Startup does not match [%lu]", stProcEntry.szExeFile, m_dwPid, stCreatTime.dwLowDateTime, m_dwPStartup ) ;
					ao_bIsAlive = M4_FALSE ;

				}

			}
			else
			{
				ClLogMonitor::DumpSysLog( "GetProcessTimes" ) ;
				iRet = M4_ERROR ;
			}

		}
		else
		{
			ClLogMonitor::DumpSysLog( "OpenProcess" ) ;
			iRet = M4_ERROR ;
		}

		CloseHandle( hProc ) ;

	}
	else
	{
		ClLogMonitor::DumpLog( false, LOG_DEBUG, "Process not running [pid %lu]", m_dwPid ) ;
		ao_bIsAlive = M4_FALSE ;
	}


	CloseHandle( hProcs ) ;


	return iRet ;

}


// -- --------------------------------------------------------------------------
// -- Dynamic library load function prototype.
typedef m4return_t (*pLibFunct_t)( m4pchar_t, m4pchar_t ) ;
// -- --------------------------------------------------------------------------

// -- --------------------------------------------------------------------------
// -- Sends a cancelation and disconnection PDUs to the application server 
// -- on behalf of the current user.
// -- --------------------------------------------------------------------------
#define MAX_ERR_SIZE	1024
m4return_t ClProcessMonitor::CloseUserSession()
{
	m4return_t	iRet = M4_SUCCESS ;
	pLibFunct_t	pCloseSessionFunc = NULL ;
	m4char_t	acErrMsg[MAX_ERR_SIZE] ;
	m4bool_t	bRetry = M4_TRUE ;
	m4int_t		iNumRetries = 0 ;


	// -- Load required library.
	if( NULL == m_hModAppClientLib )
	{
		m_hModAppClientLib = LoadLibrary( "m4appclientDLL.dll" ) ;
	
		if( NULL == m_hModAppClientLib )
		{
			ClLogMonitor::DumpLog( false, LOG_ERROR, "Error loading \"m4appclientDLL.dll\" library" ) ;
			ClLogMonitor::DumpSysLog( "LoadLibrary" ) ;

			return M4_ERROR ;
		}
	}


	// -- Get function address.
	pCloseSessionFunc = (pLibFunct_t)( GetProcAddress( m_hModAppClientLib, "DisconnectUserSession" ) ) ;

	if( NULL == pCloseSessionFunc )
	{
		ClLogMonitor::DumpLog( false, LOG_ERROR, "Error getting function address \"DisconnectUserSession\"" ) ;
		ClLogMonitor::DumpSysLog( "GetProcAddress" ) ;

		return M4_ERROR ;
	}

	// bg 150709
	while( bRetry == M4_TRUE && iNumRetries < MAX_NUM_RETRIES )
	{
		
		if( iNumRetries == 0 )
		{
			ClLogMonitor::DumpLog( false, LOG_DEBUG, "Trying to close the session, first attempt of %d", MAX_NUM_RETRIES ) ;
		}
		else
		{
			Sleep( 1000 ) ;
			ClLogMonitor::DumpLog( false, LOG_DEBUG, "Re-trying to close session. Attempt %d of %d", iNumRetries + 1, MAX_NUM_RETRIES ) ;
		}

		memset(acErrMsg, 0, MAX_ERR_SIZE);
		iRet = static_cast< m4return_t >( pCloseSessionFunc( m_pcParameters, acErrMsg ) ) ;


		if( M4_SUCCESS != iRet )
		{
			ClLogMonitor::DumpLog( false, LOG_ERROR, acErrMsg ) ;

			iNumRetries++ ;
		}
		else
		{
			if( *acErrMsg != 0 )
			{
				ClLogMonitor::DumpLog( false, LOG_DEBUG, acErrMsg ) ;
			}
			bRetry = M4_FALSE ;
		}
	}


	return iRet ;

}