//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             multipla.hpp
// Project:          
// Author:           Meta Software M.S. , S.A
// Date:             14/August/98
// Language:         C++
// Operating System: WINDOWS 
// Design Document:  
//
//
// Definition:
//
//    This module has the definitions for the multiplatform performance.
//
//
//==============================================================================


#ifndef _M4_M4DOCUMULTIP__HPP_
	#define _M4_M4DOCUMULTIP__HPP_


//Windows
#if defined( _WINDOWS ) || defined ( WINDOWS ) || defined( _WIN16 ) || defined( _M4WINX86_ )
	#include <io.h>
	#include <windows.h>
	#include "m4win.hpp"
//UNIX
#else
	#ifndef bool	
		#define bool m4bool_t
	#endif
	#ifdef BOOL
		#undef BOOL
	#endif
#endif

#ifdef _UNIX
#include <unistd.h>
#endif

#endif

//Dejar retorno de carro al final para UNIX


