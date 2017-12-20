
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltindit.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                24-10-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the temporary index item
//
//
//==============================================================================



#include "cltypes.hpp"
#include "cltnitem.hpp"
#include "mdfacglb.hpp"



#ifndef __CLTINDIT_HPP__
#define __CLTINDIT_HPP__



//=================================================================================
//
// ClTempIndexItem
//
// Clase que implementa la definición de los items de indice
//
//=================================================================================

class	ClTempIndexItem
{

protected:

// Auxiliares
    ClTempNitem		*m_poNitem ;
    m4uint8_t       m_iOrder ;
    m4uint8_t       m_iFunction ;
    m4uint8_t       m_iWay ;

// Client server
    m4uint8_t		m_iIsToCompile ;

// Ordenación
    m4char_t		m_acNodeId[ M4CL_MAX_NODE_ID + 2 ] ;
    m4char_t		m_acItemId[ M4CL_MAX_ITEM_ID + 2 ] ;
    m4uint32_t		m_iIndexId ;


public:


// Funciones de inicialización ================================================
	
        ClTempIndexItem( void )
		{
			m_poNitem = NULL ;
			m_iOrder = 0 ;
			m_iFunction = 0 ;
            m_iWay = 0 ;
			m_iIsToCompile = 1 ;

			m_acNodeId[ 0 ] = '\0' ;
			m_acNodeId[ 1 ] = '\0' ;
			m_acItemId[ 0 ] = '\0' ;
			m_acItemId[ 1 ] = '\0' ;
			m_iIndexId = 0 ;
		}

        ~ClTempIndexItem( void ) {} ;


// Funciones de lectura =======================================================

    ClTempNitem*    GetNitem( void ) const
	{
		return( m_poNitem ) ;
	}

    m4uint32_t  GetNitemHandle( void ) const
    {
        return( m_poNitem->GetHandle() ) ;
    }

	m4uint8_t	GetOrder( void ) const
	{
		return( m_iOrder ) ;
	}

	m4uint8_t	GetFunction( void ) const
	{
		return( m_iFunction ) ;
	}

	m4uint8_t	GetWay( void ) const
	{
		return( m_iWay ) ;
	}


// Funciones de escritura =====================================================

    void    SetNitem( ClTempNitem* ai_poNitem )
	{
		m_poNitem = ai_poNitem ;
	}

	void	SetOrder( m4uint8_t ai_iOrder )
	{
		m_iOrder = ai_iOrder ;
	}

	void	SetFunction( m4uint8_t ai_iFunction )
	{
		m_iFunction = ai_iFunction ;
	}

	void	SetWay( m4uint8_t ai_iWay )
	{
		m_iWay = ai_iWay ;
	}


// Funciones de client server =================================================

	void    SetIsToCompile( m4uint8_t ai_iIsToCompile )
    {
        m_iIsToCompile = ai_iIsToCompile ;
    }

    m4uint8_t	GetIsToCompile( void )
    {
        return( m_iIsToCompile ) ;
    }


// Funciones de ordenación ====================================================

	m4pcchar_t	GetNodeId( void ) const
	{
		return( m_acNodeId + 1 ) ;
	}

	m4pcchar_t	GetItemId( void ) const
	{
		return( m_acItemId + 1 ) ;
	}

	m4uint32_t  GetIndexId( void ) const
	{
		return( m_iIndexId ) ;
	}

	m4return_t	SetItemId( m4pcchar_t ai_pccItemId, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccItemId, m_acItemId, M4CL_MAX_ITEM_ID, ai_bConvert ) ) ;
	}

	m4return_t	SetNodeId( m4pcchar_t ai_pccNodeId, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccNodeId, m_acNodeId, M4CL_MAX_NODE_ID, ai_bConvert ) ) ;
	}

	void    SetIndexId( m4uint32_t ai_iIndexId )
	{
		m_iIndexId = ai_iIndexId ;
	}

} ;



#endif

