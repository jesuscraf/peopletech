
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                clttiref.hpp   
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
//    En este módulo se declara el objeto para representar las referencias a Tis
//
//
//==============================================================================


#include "mdfacglb.hpp"

#include "cldefine.hpp"
#include "facide.hpp"



#ifndef __CLTTIREF_HPP__
#define __CLTTIREF_HPP__




//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClTempNode ;



//=================================================================================
//
// ClTempTiReference
//
// Clase que implementa la definición de las referencias de Tis
//
//=================================================================================

class	ClTempTiReference
{

protected:

	ClTempNode	*m_poNode ;
	m4uint32_t	m_iReferences ;


public:


// Funciones de inicialización ================================================
	
		ClTempTiReference( void ) ;
		~ClTempTiReference( void ) {} ;


// Funciones de lectura =======================================================

	ClTempNode*	GetNode( void ) const
	{
		return( m_poNode ) ;
	}

	m4uint32_t	GetReferences( void ) const
	{
		return( m_iReferences ) ;
	}


// Funciones de escritura =====================================================

	void	SetNode( ClTempNode *ai_poNode )
	{
		m_iReferences++ ;

		if( m_poNode == NULL )
		{
			m_poNode = ai_poNode ;
		}
	}

	void    SetReferences( m4uint32_t ai_iReferences )
	{
		m_iReferences = ai_iReferences ;
	}

} ;


#endif

