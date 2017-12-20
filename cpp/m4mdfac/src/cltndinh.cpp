
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltndinh.cpp   
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
//    En este módulo se implementa el objeto para representar la sobreescritura de nodos
//
//
//==============================================================================



#include <stddef.h>
#include "cltndinh.hpp"



//=================================================================================
//
// ClTempNodeInherit
//
// Clase que implementa la definición de la sobreescritura de nodos
//
//=================================================================================

// Funciones de inicialización ================================================
	
ClTempNodeInherit :: ClTempNodeInherit( void )
{
	m_poTi = NULL ;
	m_iLevel = 0 ;

	m_acNodeId[ 0 ] = '\0' ;
	m_acNodeId[ 1 ] = '\0' ;
}



