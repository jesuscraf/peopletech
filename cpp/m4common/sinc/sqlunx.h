//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              All
// File:                sqlunx.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                25-03-1998
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    Mappings of Windows-style declarations and typedefs for unix.
//    Similar to the sqlunx.h from InterSolv 3.0 ODBC Drivers
//
//==============================================================================

#ifndef __SQLUNX_M4__
#define __SQLUNX_M4__

// Incompatibilidad entre sqlunx.h & m4sqlunx.h
#ifndef __SQLUNX
#define __SQLUNX
#include "m4sqlunx.h"
#else
#error Can´t include sqlunx.h from InterSolv ODBC 3.0 and sqlunx.h from Meta4
#endif	// __SQLUNX

#endif	// __SQLUNX_M4__
