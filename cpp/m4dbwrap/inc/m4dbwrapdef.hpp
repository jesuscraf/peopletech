//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              dbwrap 
// File:                m4dbwrapdef.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                06-04-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module defines types & defines for M4DBWrap
//
//
//==============================================================================


#ifndef _M4DBWRAPDEF_HPP_
#define _M4DBWRAPDEF_HPP_

#include "ldb_def.hpp"
#include "m4dbtypes.hpp"
#include "cldefine.hpp"

#ifdef _WINDOWS
#include <windows.h>
#endif


//#define DBWRAP_TRACE

#ifdef DBWRAP_TRACE
#define	DBWRAP_BIN_TRACE( t, s, l, p )	do { fprintf( g_pFile, "\t %s %s[%d] %x <", s, t, l, p ) ; if( p != NULL ) { fwrite( p, l, 1, g_pFile ) ; } fprintf( g_pFile, ">\n", l ) ; fflush( g_pFile ) ; } while( 0 )
#else
#define	DBWRAP_BIN_TRACE( t, s, l, p )
#endif
		


#endif //_M4DBWRAPDEF_HPP_
