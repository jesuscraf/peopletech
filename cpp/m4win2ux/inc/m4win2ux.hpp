//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4win2ux
// File:                m4win2ux.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                04-11-1998
// Language:            C++
// Operating System:    WINDOWS, UNIX (Solaris, HP-UX)
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines functions in Win32 for Unix
//
//
//==============================================================================


#ifndef __M4WIN2UX__HPP__
#define __M4WIN2UX__HPP__

#include "m4win2ux_dll.hpp"
#include "m4types.hpp"
#include "m4win.hpp"
#include "syncro.hpp"					// M4IPCS



#ifdef _UNIX

// *********** _UNIX ****************

	#include <ctype.h>
	#include <string.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <assert.h>
	// IEEE floating-point arithmetic base conversion for Solaris
	#if defined(_M4SOLX86_) || defined(_M4SOLSPARC_)
	#include <floatingpoint.h>
	#endif

	#ifdef _KCC		// KCC
	#include <algobase>
	using std::min;
	using std::max;
	#endif			// KCC

	#include <ospace/std/algorithm>

	#ifdef _UNIX
	#if defined(SVR4) || defined(_SOLARIS_2_5_1)
	#include <sys/systeminfo.h>
	#endif
	#endif

	// Declaracion de las funciones de manipulacion de Directorios en Unix
	#include <unistd.h>
	#if defined(_M4SOLSPARC_) || defined(_M4SOLX86_)
	#include <sys/types.h>
	#endif
	#include <sys/stat.h>

	// Funciones en Win32, que son funciones templates en STL
	#define __min(a, b)		min(a, b)
	#define __max(a, b)		max(a, b)

// *********** _UNIX ****************

#else	// _UNIX

// *********** _WIN32 ****************

	// Declaracion de las funciones de manipulacion de Directorios en Win32
	#include <direct.h>

// *********** _WIN32 ****************

#endif	//_UNIX



#include "m4win2ux.inl"


#endif // __M4WIN2UX__HPP__

