//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           OLEExec.dll
// File:             oledef.hpp
// Project:          OLE Object integration
// Author:           Meta Software M.S. , S.A
// Date:             06/07/97
// Language:         C++
// Operating System: WINDOWS 
// Design Document:  
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#ifndef _OLEDEF_H
#define _OLEDEF_H

#include "m4define.hpp"
#include "m4log.hpp"
#include "errors.hpp"

#ifdef _DEBUG

#define TRACESUB(X)      SETCODE(M4_OBJ_OLE_ENTER_FUNCTION, DEBUGINFOLOG, "Entering function " #X);
#define TRACERETVAL(X,Y) { int _retval = Y; SETCODEF(M4_OBJ_OLE_EXIT_FUNCTION, DEBUGINFOLOG, "Leaving function " #X " with value %d", _retval); return(_retval); }
#define TRACERETVOID(X)  SETCODEF(M4_OBJ_OLE_EXIT_FUNCTION, DEBUGINFOLOG, "Leaving function " #X);
#define TRACE(X)         SETCODE(M4_OBJ_OLE_INFORMATION, DEBUGINFOLOG, X);
#define TRACE2(X,Y)      SETCODEF(M4_OBJ_OLE_INFORMATION, DEBUGINFOLOG, X, Y);
#define TRACE3(X,Y,Z)    SETCODEF(M4_OBJ_OLE_INFORMATION, DEBUGINFOLOG, X, Y, Z);

#else

#define TRACESUB(X)
#define TRACERETVAL(X,Y) return Y;
#define TRACERETVOID(X)  return;
#define TRACE(X)
#define TRACE2(X,Y)
#define TRACE3(X,Y,Z)

#endif

#endif
