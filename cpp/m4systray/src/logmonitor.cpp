//	==============================================================================
//
//	 (c) Copyright  1991-2008 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            m4systray
//	 File:              logmonitor.cpp
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

#include <logmonitor.hpp>
#include <windows.h>
#include <time.h>


#define LOG_FILE_NAME "m4systray.log"


// -- --------------------------------------------------------------------------
// -- ClLogMonitor static attrs. initialization.
// -- --------------------------------------------------------------------------

FILE* ClLogMonitor::ms_pfOutput = NULL ;
m4pcchar_t ClLogMonitor::ms_ppccErrorTypes[3] = { "ERROR", "Warning", "Debug" } ;
m4pcchar_t ClLogMonitor::ms_pccLogFileName = LOG_FILE_NAME ;


// -- --------------------------------------------------------------------------
// -- Private cttor.
// -- --------------------------------------------------------------------------

ClLogMonitor::ClLogMonitor()
{
}


// -- --------------------------------------------------------------------------
// -- Destructor.
// -- --------------------------------------------------------------------------

ClLogMonitor::~ClLogMonitor()
{
}


// -- --------------------------------------------------------------------------
// -- Initializes log.
// -- --------------------------------------------------------------------------

void ClLogMonitor::InitializeLog()
{
	m4pcchar_t	pccFileDir = NULL ;
	m4char_t	acFile[ MAX_PATH + 1 ] ;


	if( ms_pfOutput == NULL )
	{

		pccFileDir = getenv( "temp" ) ;

		if( NULL == pccFileDir )
		{
			pccFileDir = "." ;
		}

		sprintf( acFile, "%s%s%s", pccFileDir, "\\", LOG_FILE_NAME ) ;


		// -- Clear previous trace.
		ms_pfOutput = fopen( acFile, "w" ) ;
	}

}


// -- --------------------------------------------------------------------------
// -- Finalizes log.
// -- --------------------------------------------------------------------------

void ClLogMonitor::FinalizeLog()
{

	if( ms_pfOutput != NULL )
	{
		fclose( ms_pfOutput ) ;
	}

}


// -- --------------------------------------------------------------------------
// -- Dumps a log string to the log file.
// --	
// --	ai_bOverwriteLastLine:			false to append new log lines at the end of the file.
// --									true to overwrite the last log line, except fist time that 
// --									behavior is as it was false.
// --	ai_iType:						Log type (error, warning, debug).
// --	ai_pccFormat:					printf - like format.
// --	...:							Format arguments.
// --
// -- --------------------------------------------------------------------------

int ClLogMonitor::DumpLog( bool ai_bOverwriteLastLine, eM4ErrorType_t ai_iType, m4pcchar_t ai_pccFormat, ... )
{
	int			iWrite =  0 ;
	va_list		list ;
	time_t		tTimeNow ;
	char*		pcTimeNow ;
	static long lLastLinePos = -1 ;


	if( ms_pfOutput == NULL )
	{
		InitializeLog();
	}

	
	if( ms_pfOutput != NULL )
	{
		tTimeNow = time(NULL) ;
		pcTimeNow = asctime( gmtime( &tTimeNow) ) ;
		pcTimeNow[ strlen(pcTimeNow) - 1 ] = 0 ;

		
		if( true == ai_bOverwriteLastLine )
		{
			// -- First time is ignored, it means that the line currently dumping
			// -- will be overwritten in the next call.
			if( -1 != lLastLinePos )
			{
				fseek( ms_pfOutput, lLastLinePos, SEEK_SET ) ;
			}

			lLastLinePos = ftell( ms_pfOutput );
		}


		fprintf( ms_pfOutput, "[%s] [%d] [%s] ", pcTimeNow, GetCurrentThreadId(), ms_ppccErrorTypes[ai_iType] ) ;

		va_start( list, ai_pccFormat ) ;
		iWrite = vfprintf( ms_pfOutput, ai_pccFormat, list ) ;
		fprintf( ms_pfOutput, "\n" ) ;
		fflush( ms_pfOutput ) ;
		va_end( list ) ;
	}


	return M4_SUCCESS ;
}


// -- --------------------------------------------------------------------------
// -- Dumps a system log string to the log file.
// --
// --	ai_pccFunction:					System function called.
// --
// -- --------------------------------------------------------------------------

int ClLogMonitor::DumpSysLog( m4pcchar_t ai_pccFunction )
{
	m4int32_t	iErrorCode ;
	string		sErrorMsg ;
	int			iWrite =  0 ;


	GetErrorMessage( iErrorCode , sErrorMsg ) ;
	sErrorMsg[sErrorMsg.length()-1] = 0 ;

	iWrite = DumpLog( true, LOG_ERROR, "Error \"%ld\" in the call to \"%s\": %s.", iErrorCode, ai_pccFunction, sErrorMsg.c_str() ) ;


	return iWrite ;
}


// -- --------------------------------------------------------------------------
// -- Gets a system error message.
// --
// --	aio_iErrorCode:					System error code.
// --	aio_sErrorMsg:					Output error message.
// --
// -- --------------------------------------------------------------------------

m4return_t ClLogMonitor::GetErrorMessage(m4int32_t &aio_iErrorCode, string &aio_sErrorMsg)
{
	DWORD dwError ;
	LPVOID lpMsgBuf = 0;

	
	aio_sErrorMsg = "";

	dwError = GetLastError();

	FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER
		| FORMAT_MESSAGE_IGNORE_INSERTS
		| FORMAT_MESSAGE_FROM_SYSTEM,
		0,
		dwError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPTSTR>(&lpMsgBuf),
		0,
		0);
	
	if (lpMsgBuf == 0)
    {
		aio_sErrorMsg = "Unexpected error condition.";
    }
	else
    {
		aio_sErrorMsg = reinterpret_cast<LPCTSTR>(lpMsgBuf);
		LocalFree (lpMsgBuf);
    }

	aio_iErrorCode = dwError;


	return M4_SUCCESS;
}