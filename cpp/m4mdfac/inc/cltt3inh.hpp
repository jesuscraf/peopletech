
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltt3inh.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                28-06-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    En este módulo se declara el objeto para representar la herencia de T3s
//
//
//==============================================================================


#include "mdfacglb.hpp"

#include "cldefine.hpp"
#include "facide.hpp"
#include "spoolstr.h"



#ifndef __CLTT3INH_HPP__
#define __CLTT3INH_HPP__




//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClTempT3 ;



//=================================================================================
//
// ClTempT3Inherit
//
// Clase que implementa la definición de la herencia de T3s
//
//=================================================================================

class	ClTempT3Inherit : public ClNumericId
{

protected:

	m4uint32_t			m_iBaseT3Id ;
	m4uint8_t			m_iLevel ;
	m4uint8_t			m_iIndex ;

// Auxiliares
	ClStaticPoolStr		*m_poUpperPool ;


public:

	m4uint32_t	GetIdentifier( void ) const
	{
		return( m4uint32_t( GetLevel() ) ) ;
	}


// Funciones de inicialización ================================================
	
		ClTempT3Inherit( void ) ;
		~ClTempT3Inherit( void ) {} ;

	void	SetUpperPool( ClStaticPoolStr *ai_poPool )
	{
		m_poUpperPool = ai_poPool ;
	}


// Funciones de lectura =======================================================

	m4uint32_t	GetBaseT3IdHandle( void ) const
	{
		return( m_iBaseT3Id ) ;
	}

	m4pcchar_t	GetBaseT3Id( void ) const
	{
		return( m_poUpperPool->GetString( m_iBaseT3Id ) ) ;
	}

	m4uint8_t	GetLevel( void ) const
	{
		return( m_iLevel ) ;
	}

	m4uint8_t	GetIndex( void ) const
	{
		return( m_iIndex ) ;
	}


// Funciones de escritura =====================================================

	m4return_t  SetBaseT3Id( m4pcchar_t ai_pccT3Id )
	{
		return( M4MdFacSetPoolId( ai_pccT3Id, m_iBaseT3Id, m_poUpperPool ) ) ;
	}

	void    SetLevel( m4uint8_t ai_iLevel )
	{
		m_iLevel = ai_iLevel ;
	}

	void    SetIndex( m4uint8_t ai_iIndex )
	{
		m_iIndex = ai_iIndex ;
	}

} ;


#endif

