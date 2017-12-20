
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltalias.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                28-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the functions of the alias of a node
//
//
//==============================================================================



#include "cltalias.hpp"
#include "cltnode.hpp"



//=================================================================================
//
// ClTempAlias
//
// Clase que implementa la definición de los alias
//
//=================================================================================
	
ClTempAlias :: ClTempAlias( void )
{
	m_iAliasId = 0 ;
	m_poUsedNode = NULL ;

    m_poUpperPool = NULL ;
}



// Otras funciones de lectura ================================================

m4uint32_t	ClTempAlias :: GetUsedNodeHandle( void ) const
{
	return( m_poUsedNode->GetHandle() ) ;
}


m4uint8_t	ClTempAlias :: GetUsedNodeIsToCompile( void ) const
{
	return( m_poUsedNode->GetIsToCompile() ) ;
}


