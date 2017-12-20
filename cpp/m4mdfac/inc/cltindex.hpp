
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltindex.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                28-12-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the temporary index
//
//
//==============================================================================



#include "cltypes.hpp"
#include "clpstack.hpp"
#include "mdfacglb.hpp"



#ifndef __CLTINDEX_HPP__
#define __CLTINDEX_HPP__



//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class   ClTempIndexItem ;
class   ClTempNode ;
class	ClStaticPoolStr ;
class	ClMdStatistics ;



//=================================================================================
//
// ClTempIndex
//
// Clase que implementa la definición de los indices
//
//=================================================================================

class	ClTempIndex
{

protected:

// Serializacion
	m4uint32_t			m_iHandle ;
	ClStaticPoolStr		*m_poUpperPool ;

// Fijos
    m4uint32_t			m_iIndexId ;

// Variables
    ClReferenceStack	m_oItems ;

// Auxiliares
    m4uint8_t			m_iIsDefault ;
	ClTempNode			*m_poNode ;

// Client server
	m4uint16_t			m_iItemsToCompile ;

// Ordenación
    m4char_t			m_acNodeId[ M4CL_MAX_NODE_ID + 2 ] ;


public:


// Funciones de inicialización ================================================
	
		ClTempIndex( void ) ;
		~ClTempIndex( void ) {} ;


// Funciones de lectura de la parte fija ======================================

	m4uint32_t  GetIndexId( void ) const
	{
		return( m_iIndexId ) ;
	}


// Otras funciones de lectura ================================================

    m4uint8_t    GetIsDefault( void ) const
    {
        return( m_iIsDefault ) ;
    }

    ClTempNode*	GetNode( void ) const
    {
        return( m_poNode ) ;
    }

    m4uint16_t  GetNumberOfItems( void ) const
    {
        return( m_oItems.GetLength() ) ;
    }

    ClTempIndexItem*	GetItemByPosition( m4uint16_t ai_iPosition ) ;


// Otras funciones de escritura ===============================================

    void    SetIsDefault( m4uint8_t ai_iIsDefault )
    {
        m_iIsDefault = ai_iIsDefault ;
    }

   	void	SetNode(  ClTempNode* ai_poNode )
    {
        m_poNode = ai_poNode ;
    }


// Funciones de escritura de parte fija =======================================

	void    SetIndexId( m4uint32_t ai_iIndexId )
	{
		m_iIndexId = ai_iIndexId ;
	}


// Funciones de referencia ====================================================

    void	AddItem( void )
	{
		m_oItems.AddElement() ;
	}

	void	InitItems( ClTempIndexItem **ai_ppoItems )
	{
		m_oItems.Init( ( m4pvoid_t* ) ai_ppoItems ) ;
	}


// Funciones de serialización =================================================

	m4uint32_t	GetHandle( void ) const
	{
		return( m_iHandle ) ;
	}

	void	SetUpperPool( ClStaticPoolStr *ai_poPool )
	{
		m_poUpperPool = ai_poPool ;
	}

    m4return_t	GetSize( m4uint32_t &ao_iSize, ClMdStatistics *ai_poStatistics ) ;
	m4return_t	Compile( m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize ) ;


// Funciones de client server =================================================

	void	SetItemsToCompile( m4uint16_t ai_iItems )
	{
		m_iItemsToCompile = ai_iItems ;
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

