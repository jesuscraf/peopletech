
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltarg.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                01-07-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the temporary argument functions
//
//
//==============================================================================


#include "cltarg.hpp"



//=============================================================================
//
// ClTempArgument
//
// Clase que implementa los argumentos temporales
//
//=============================================================================

// Funciones de inicialización ================================================
	
ClTempArgument :: ClTempArgument( void )
{
    m_iArgumentId = 0 ;
    m_iName = 0 ;
    m_iType = 0 ;
	m_iPrecision = 0 ;
    m_iM4Type = 0 ;
    m_iOrder = 0 ;
    m_iRealOrder = 0 ;

	m_poUpperPool = NULL ;
	m_poNoUpperPool = NULL ;

	m_acTiId  [ 0 ] = '\0' ;
	m_acTiId  [ 1 ] = '\0' ;
	m_acItemId[ 0 ] = '\0' ;
	m_acItemId[ 1 ] = '\0' ;
}


