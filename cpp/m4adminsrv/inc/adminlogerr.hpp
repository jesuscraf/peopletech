//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             adminlogerr.hpp
// Project:			 m4adminsrv
// Author:           Meta Software M.S. , S.A
// Date:13/01/1999
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines m4log errors for Meta4 Server Administrator.
//
//
//==============================================================================
#ifndef __ADMINLOGERR__HPP__
#define __ADMINLOGERR__HPP__

#include "m4log.hpp"
#include "m4res.hpp"


//----------------------------------------------------------------------------
// This sentence checks that M4_SRV_ADMINISTRATOR_ERROR_BASE is defined.
// The macro M4_SRV_ADMINISTRATOR_ERROR_BASE must be defined in m4res.hpp 
// file.
//----------------------------------------------------------------------------
#ifndef M4_SRV_ADMINISTRATOR_ERROR_BASE
#	error "M4_SRV_ADMINISTRATOR_ERROR_BASE not defined"
#endif

//----------------------------------------------------------------------------
// base code for errors, warning, debug and trace sections.
//----------------------------------------------------------------------------
#define M4_SRV_ADMINISTRATOR_WARNING_BASE		(0x2000 + M4_SRV_ADMINISTRATOR_ERROR_BASE)
#define M4_SRV_ADMINISTRATOR_DEBUG_BASE			(0x4000 + M4_SRV_ADMINISTRATOR_ERROR_BASE)
#define M4_SRV_ADMINISTRATOR_TRACE_BASE			(0x6000 + M4_SRV_ADMINISTRATOR_ERROR_BASE)


//----------------------------------------------------------------------------
// Administrator error codes.
//----------------------------------------------------------------------------
#define M4_SRV_ADM_InvalidAdministratorClient	(0x001 + M4_SRV_ADMINISTRATOR_ERROR_BASE)
#define M4_SRV_ADM_UserHasNotAdministratorRole	(0x002 + M4_SRV_ADMINISTRATOR_ERROR_BASE)


//----------------------------------------------------------------------------
// For each administration error define a inline function that is a wrapper 
// for the m4log global function.
//----------------------------------------------------------------------------
class M4AdminLogError
{
private:

	// this function return the log error type for each m4-admin-code
	inline static m4int16_t GetErrorType(const int ai_errorCode)
	{
		if (ai_errorCode > M4_SRV_ADMINISTRATOR_TRACE_BASE)		return TRACEINFOLOG;
		if (ai_errorCode > M4_SRV_ADMINISTRATOR_DEBUG_BASE)		return DEBUGINFOLOG;
		if (ai_errorCode > M4_SRV_ADMINISTRATOR_WARNING_BASE)	return WARNINGLOG;
		if (ai_errorCode > M4_SRV_ADMINISTRATOR_ERROR_BASE)		return ERRORLOG;

		return ERRORLOG;
	}

public:
	inline static void Setcodef(const int ai_errorCode)
	{
		SETCODEF(ai_errorCode, GetErrorType(ai_errorCode), "");
	}
};


#endif //__ADMINLOGERR__HPP__


