//	==============================================================================
//
//	 (c) Copyright  1991-2008 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            m4systray
//	 File:              logmonitor.hpp
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

#ifndef logmonitor_hpp
#define logmonitor_hpp 1

#include <m4types.hpp>
#include <m4string.hpp>
#include <stdio.h>


typedef enum { LOG_ERROR, LOG_WARNING, LOG_DEBUG } eM4ErrorType_t ;


// -- -------------------------------------------------------------------------------
// -- Log class.
// -- -------------------------------------------------------------------------------
class ClLogMonitor
{
	public:

		virtual ~ClLogMonitor() ;


		// -- Initialization tasks. Not mandatory.

		static void InitializeLog() ;


		// -- Closing log.

		static void FinalizeLog() ;

		
		// -- Dumps a single log line.
		// --
		// --	ai_bOverwriteLastLine:			false to append new log lines at the end of the file.
		// --									true to overwrite the last log line, except fist time that 
		// --									behavior is as it was false.
		// --	ai_iType:						Log type (error, warning, debug).
		// --	ai_pccFormat:					printf - like format.
		// --	...:							Format arguments.

		static int DumpLog( bool ai_bOverwriteLastLine, eM4ErrorType_t ai_iType, m4pcchar_t ai_pccFormat ... ) ;


		// -- Dumps a single system error log line.
		// --
		// --	ai_pccFunction:					System function called.

		static int DumpSysLog( m4pcchar_t ai_pccFunction ) ;


	private:

		ClLogMonitor() ;
		
		static m4return_t GetErrorMessage( m4int32_t &aio_iErrorCode, string &aio_sErrorMsg ) ;		

		static FILE *ms_pfOutput ;
		static m4pcchar_t ms_ppccErrorTypes[3] ;
		static m4pcchar_t ms_pccLogFileName ;
} ;


#endif