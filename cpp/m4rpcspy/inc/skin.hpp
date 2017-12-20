
//==============================================================================
//
// (c) Copyright  1991-2005 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              rpcspy
// File:                skin.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                20-10-2005
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the wrapper class for the skin
//
//
//==============================================================================


#include <stdio.h>
#include <afxdisp.h>



#ifndef __SKIN__HPP__
#define __SKIN__HPP__



//=================================================================================
//
// ClCSkin
//
// Clase que recubre el uso del skin de Meta4
// Se ha creado usando el classwizard y se ha retocado
//
//=================================================================================

class ClCSkin : public COleDispatchDriver
{

public:

//=================================================================================
// Funciones de inicialización
//=================================================================================

	ClCSkin( void ) ;
	virtual ~ClCSkin( void ) {} ;

} ;


#endif

