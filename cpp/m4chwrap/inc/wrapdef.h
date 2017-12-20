//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapdef.h
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//    Declaration of the trace macros
//
//==============================================================================

//
// Include files
//
#include "m4define.hpp"
#include "m4log.hpp"


#ifndef _WRAPDEF_H
#define _WRAPDEF_H


void	M4CatchExceptions( void ) ;
void	M4UncatchExceptions( void ) ;


//
// The trace macros
//
#if defined(_M4_TRACESUBS)

/*
Para trazar a fichero también, en vez de llamar a ALTRACE se llama a esta nueva que hace lo mismo
que ALTRACE y además vuelca a fichero
*/
inline void	M4AtlTrace(LPCSTR lpszFormat, ...)
{
	static	FILE*	s_pfFile = fopen("c:\\temp\\m4chwrap.log", "w" ) ;

	if( s_pfFile != NULL )
	{
		va_list args;
		va_start(args, lpszFormat);

		vfprintf( s_pfFile, lpszFormat, args );
		fflush( s_pfFile ) ;

		va_end(args);
	}
}

#define ENTER_METHOD(X)		do { M4CatchExceptions(); M4AtlTrace("%s\n", "Entering function " #X); } while (0)
#define LEAVE_METHOD(X,Y)	do { long _retval = Y; M4AtlTrace("Leaving function " #X " with value %d\n", _retval); M4UncatchExceptions(); return(_retval); } while (0)

#define TRACE(X)			do { M4AtlTrace("%s\n", X); } while (0)
#define TRACE2(X,Y)			do { M4AtlTrace(X, Y);    M4AtlTrace("\n"); } while (0)
#define TRACE3(X,Y,Z)		do { M4AtlTrace(X, Y, Z); M4AtlTrace("\n"); } while (0)

#else

inline void _cdecl WrapDummy(LPCTSTR , ...){}

#define ENTER_METHOD(X)		do { M4CatchExceptions(); } while (0)
#define LEAVE_METHOD(X,Y)	do { M4UncatchExceptions(); return Y; } while (0)

#define TRACE   1 ? (void)0 : WrapDummy
#define TRACE2  1 ? (void)0 : WrapDummy
#define TRACE3  1 ? (void)0 : WrapDummy

#endif

#endif

