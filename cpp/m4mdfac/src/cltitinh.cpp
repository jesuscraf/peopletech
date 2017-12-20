
//==============================================================================
//
// (c) Copyright  1991-2009 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltcpt.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                09-03-2009
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    En este módulo se define el objeto para representar la sobreescritura de items
//
//
//==============================================================================


#include "cltitinh.hpp"



//=================================================================================
//
// ClTempItemInherit
//
// Clase que implementa la definición de la sobreescritura de items
//
//=================================================================================



// Funciones de inicialización ================================================
	
ClTempItemInherit :: ClTempItemInherit( void )
{
	m_acTiId    [ 0 ] = '\0' ;
	m_acTiId    [ 1 ] = '\0' ;
	m_acItemId  [ 0 ] = '\0' ;
	m_acItemId  [ 1 ] = '\0' ;
	m_pcItemName = NULL ;
}


