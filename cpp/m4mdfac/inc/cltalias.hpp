
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltalias.hpp   
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
//    This module defines the alias of a node
//
//
//==============================================================================



#include "mdfacglb.hpp"

#include "facide.hpp"
#include "cltypes.hpp"
#include "cldefine.hpp"
#include "spoolstr.h"



#ifndef __CLTALIAS_HPP__
#define __CLTALIAS_HPP__



//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClTempNode ;




//=================================================================================
//
// ClTempAlias
//
// Clase que implementa la definición de los alias
//
//=================================================================================

class	ClTempAlias : public ClTwoIdentifier
{

protected:

    m4uint32_t		m_iAliasId ;
    ClTempNode		*m_poUsedNode ;

    ClStaticPoolStr	*m_poUpperPool ;

// Ordenación
	m4char_t		m_acNodeId[ M4CL_MAX_NODE_ID + 2 ] ;


public:

	m4pcchar_t	GetParentIdentifier( void ) const
	{
		return( GetNodeId() ) ;
	}

	m4pcchar_t	GetIdentifier( void ) const
	{
		return( GetAliasId() ) ;
	}


// Funciones de inicialización ================================================
	
	ClTempAlias( void ) ;
    ~ClTempAlias( void ) {} ;


// Funciones de lectura de la parte fija ======================================

	m4uint32_t	GetAliasIdHandle( void ) const
	{
		return( m_iAliasId ) ;
	}

	m4pcchar_t	GetAliasId( void ) const
	{
		return( m_poUpperPool->GetString( m_iAliasId ) ) ;
	}

	ClTempNode*	GetUsedNode( void ) const
	{
		return( m_poUsedNode ) ;
	}


// Otras funciones de lectura ================================================

	m4uint32_t  GetUsedNodeHandle( void ) const ;
	m4uint8_t	GetUsedNodeIsToCompile( void ) const ;


// Funciones de escritura de parte fija =======================================

	m4return_t  SetAliasId( m4pcchar_t ai_pccAliasId )
	{
		return( M4MdFacSetPoolId( ai_pccAliasId, m_iAliasId, m_poUpperPool ) ) ;
	}

	void SetUsedNode( ClTempNode *ai_poUsedNode )
	{
		m_poUsedNode = ai_poUsedNode ;
	}


// Funciones de serialización =================================================

	void	SetUpperPool( ClStaticPoolStr *ai_poPool )
	{
		m_poUpperPool = ai_poPool ;
	}


// Funciones de ordenación ====================================================

	m4pcchar_t	GetNodeId( void ) const
	{
		return( m_acNodeId + 1 ) ;
	}

	m4return_t	SetNodeId( m4pcchar_t ai_pccNodeId, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccNodeId, m_acNodeId, M4CL_MAX_NODE_ID, ai_bConvert ) ) ;
	}

} ;



#endif

