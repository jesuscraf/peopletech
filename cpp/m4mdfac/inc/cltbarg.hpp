
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltbarg.hpp   
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
//    This module defines the temporary business method argument
//
//
//==============================================================================



#include "mdfacglb.hpp"
#include "cldefine.hpp"
#include "facide.hpp"


#ifndef __CLTBARG_HPP__
#define __CLTBARG_HPP__



//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClTempNode ;




//=================================================================================
//
// ClTempBusinessArg
//
// Clase que implementa la definición de los argumentos de los métodos de negocio
//
//=================================================================================

class	ClTempBusinessArg : public ClIdentifier
{

protected:

// Fijos
    m4uint8_t			m_iType ;

// Auxiliares
	ClTempNode			*m_poNode ;

// Ordenación
    m4char_t			m_acMethodId[ M4CL_MAX_BMETHOD_ID + 2 ] ;

public:

	m4pcchar_t	GetIdentifier( void ) const
	{
		return( GetMethodId() ) ;
	}


// Funciones de inicialización ================================================
	
		ClTempBusinessArg( void ) ;
        ~ClTempBusinessArg( void ) {} ;


// Funciones de lectura de la parte fija ======================================

    m4uint8_t GetType( void ) const
    {
        return( m_iType ) ;
    }

	ClTempNode*	GetNode( void )
	{
		return( m_poNode ) ;
	}

// Funciones de escritura de parte fija =======================================

    void    SetType( m4uint8_t ai_iType )
    {
        m_iType = ai_iType ;
    }

	void	SetNode( ClTempNode *ai_poNode )
	{
		m_poNode = ai_poNode ;
	}

// Funciones de ordenación ====================================================

	m4pcchar_t	GetMethodId( void ) const
	{
		return( m_acMethodId + 1 ) ;
	}

	m4return_t	SetMethodId( m4pcchar_t ai_pccMethodId, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccMethodId, m_acMethodId, M4CL_MAX_BMETHOD_ID, ai_bConvert ) ) ;
	}
} ;



#endif

