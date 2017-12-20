
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                clttiinh.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                14-04-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    En este módulo se implementa el objeto para representar la herencia de TIs
//
//
//==============================================================================



#include <stddef.h>
#include "clttiinh.hpp"



//=================================================================================
//
// ClTempTiInherit
//
// Clase que implementa la definición de la herencia de TIs
//
//=================================================================================

// Funciones de inicialización ================================================
	
ClTempTiInherit :: ClTempTiInherit( void )
{
	m_acTiId[ 0 ] = '\0' ;
	m_acTiId[ 1 ] = '\0' ;

	m_poBaseTi = NULL ;
	m_iLevel = 0 ;
}



