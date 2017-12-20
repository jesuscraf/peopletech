
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltplug.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                29-07-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the temporary plug of nodes
//
//
//==============================================================================



#include "mdfacglb.hpp"
#include "cltypes.hpp"
#include "clpstack.hpp"
#include "spoolstr.h"



#ifndef __CLTPLUG_HPP__
#define __CLTPLUG_HPP__



//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClTempNode ;
class	ClTempPlugItem ;
class	ClMdStatistics ;




//=================================================================================
//
// ClTempPlug
//
// Clase que implementa la definición de los carriles
//
//=================================================================================

class	ClTempPlug
{

protected:

// Serializacion
	m4uint32_t			m_iHandle ;
	ClStaticPoolStr		*m_poUpperPool ;
	ClStaticPoolStr		*m_poNoUpperPool ;

// Fijos
    m4uint32_t			m_iUsedT3 ;
	m4uint32_t			m_iUsedT3Name ;
    m4uint32_t			m_iUsedNode ;

    m4uint16_t			m_iSCRStart ;

    m4uint8_t			m_iType ;
    m4uint8_t			m_iIsFilter ;
    m4uint8_t			m_iRelationTypeId ;
	m4uint8_t			m_iIsMultiselection ;

// Componentes
    ClReferenceStack	m_oItems ;

// Auxiliares
	ClTempNode			*m_poNode ;

// Client server
	m4uint16_t			m_iItemsToCompile ;

// Ordenación
	m4char_t			m_acNodeId[ M4CL_MAX_NODE_ID + 2 ] ;


public:


// Funciones de inicialización ================================================
	
        ClTempPlug( void ) ;
        ~ClTempPlug( void ) {} ;


// Funciones de lectura de la parte fija ======================================

    m4uint32_t  GetUsedT3IdHandle( void ) const
    {
		return( m_iUsedT3 ) ;
    }

    m4uint32_t  GetUsedNodeIdHandle( void ) const
    {
		return( m_iUsedNode ) ;
    }

    m4pcchar_t  GetUsedT3Id( void ) const
    {
		return( m_poUpperPool->GetString( m_iUsedT3 ) ) ;
    }

    m4pcchar_t  GetUsedNodeId( void ) const
    {
		return( m_poUpperPool->GetString( m_iUsedNode ) ) ;
    }


// Otras funciones de lectura ================================================

	ClTempNode*	GetNode( void ) const
	{
		return( m_poNode ) ;
	}

    m4uint16_t  GetNumberOfItems( void ) const
    {
        return( m_oItems.GetLength() ) ;
    }

    ClTempPlugItem*	GetItemByPosition(  m4uint16_t ai_iPosition ) ;


// Funciones de escritura de parte fija =======================================

	m4return_t	SetUsedT3Id( m4pcchar_t ai_pccUsedT3Id )
	{
		return( M4MdFacSetPoolId( ai_pccUsedT3Id, m_iUsedT3, m_poUpperPool ) ) ;
	}

	m4return_t	SetUsedT3Name( m4pcchar_t ai_pccUsedT3Name )
	{
		return( M4MdFacSetPoolId( ai_pccUsedT3Name, m_iUsedT3Name, m_poNoUpperPool ) ) ;
	}

	m4return_t	SetUsedNodeId( m4pcchar_t ai_pccUsedNodeId )
	{
		return( M4MdFacSetPoolId( ai_pccUsedNodeId, m_iUsedNode, m_poUpperPool ) ) ;
	}

	void	SetSCRStart( m4uint16_t ai_iSCRStart )
	{
		m_iSCRStart = ai_iSCRStart ;
	}

    void    SetType(  m4uint8_t ai_iType )
    {
	        m_iType = ai_iType ;
    }

    void    SetIsFilter(  m4uint8_t ai_iIsFilter )
    {
        m_iIsFilter = ai_iIsFilter ;
    }
   
    void    SetRelationTypeId(  m4uint8_t ai_iRelationTypeId )
    {
        m_iRelationTypeId = ai_iRelationTypeId ;
    }

    void    SetIsMultiselection(  m4uint8_t ai_iIsMultiselection )
    {
        m_iIsMultiselection = ai_iIsMultiselection ;
    }

	void	SetNode( ClTempNode *ai_poNode )
	{
		m_poNode = ai_poNode ;
	}


// Funciones de referencia ====================================================

    void	AddItem( void )
	{
		m_oItems.AddElement() ;
	}

	void	InitItems( ClTempPlugItem **ai_ppoItems )
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

	void	SetNoUpperPool( ClStaticPoolStr *ai_poPool )
	{
		m_poNoUpperPool = ai_poPool ;
	}

    m4return_t	GetSize( m4uint32_t &ao_iSize, ClMdStatistics *ai_poStatistics ) ;
	m4return_t	Compile( m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize, m4uint32_t ai_iPoolSize ) ;


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

