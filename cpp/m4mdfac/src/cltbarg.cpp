
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltbarg.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                27-11-2007
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the functions for the temporary business method argument
//
//
//==============================================================================



#include "cltbarg.hpp"





//=================================================================================
//
// ClTempBusinessArg
//
// Clase que implementa la definición de los argumentos de los métodos de negocio
//
//=================================================================================


// Funciones de inicialización ================================================
	
ClTempBusinessArg :: ClTempBusinessArg( void )
{
	m_iType = 0 ;
	m_poNode = NULL ;

	m_acMethodId[ 0 ] = '\0' ;
	m_acMethodId[ 1 ] = '\0' ;
}

