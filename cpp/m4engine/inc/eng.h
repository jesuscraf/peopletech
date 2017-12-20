//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             ENG.h
// Project:			 engindll.dll
// Author:           Meta Software M.S. , S.A
// Date:             14/10/99
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Manuel Lazcano Perez
//	Modifications:	Who && Date
//
// Definition:
//
//
//
//==============================================================================

#ifndef __ENG__H__
#define __ENG__H__

#include "m4engine_dll.hpp"


/*
Switchs de compilacion

FONT_M_DEBUG - Solo interviene en los CPP, para meter o no traza.

*/

/*
Enganche con FMETRICS
----------------------------------------------------------------------------------------------
*/
#include "fontm.h"

/*
Enganche con MIT
----------------------------------------------------------------------------------------------
*/

#include "mit.h"

/*
PlugIn
----------------------------------------------------------------------------------------------
*/

extern M4_DECL_M4ENGINE StMIT_CompPlugIn g_ENG_pstPlugIn ;


#endif

