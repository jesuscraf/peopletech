
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltndinh.hpp   
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
//    En este módulo se declara el objeto para representar la sobreescritura de nodos
//
//
//==============================================================================


#include "mdfacglb.hpp"
#include "cldefine.hpp"



#ifndef __CLTNDINH_HPP__
#define __CLTNDINH_HPP__




//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClTempTi ;



//=================================================================================
//
// ClTempNodeInherit
//
// Clase que implementa la definición de la sobreescritura de nodos
//
//=================================================================================

class	ClTempNodeInherit
{

protected:

	ClTempTi	*m_poTi ;
	m4uint8_t	m_iLevel ;

// Ordenación
    m4char_t	m_acNodeId[ M4CL_MAX_NODE_ID + 2 ] ;


public:


// Funciones de inicialización ================================================
	
		ClTempNodeInherit( void ) ;
		~ClTempNodeInherit( void ) {} ;


// Funciones de lectura =======================================================

	ClTempTi*	GetTi( void ) const
	{
		return( m_poTi ) ;
	}

	m4uint8_t	GetLevel( void ) const
	{
		return( m_iLevel ) ;
	}


// Funciones de escritura =====================================================

	void    SetTi( ClTempTi *ai_poTi )
	{
		m_poTi = ai_poTi ;
	}

	void    SetLevel( m4uint8_t ai_iLevel )
	{
		m_iLevel = ai_iLevel ;
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

