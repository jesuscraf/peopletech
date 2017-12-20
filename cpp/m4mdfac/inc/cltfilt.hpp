
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltfilt.hpp   
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
//    This module defines the temporary filters
//
//
//==============================================================================



#include "mdfacglb.hpp"
#include "cltypes.hpp"
#include "spoolstr.h"



#ifndef __CLTFILT_HPP__
#define __CLTFILT_HPP__



//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClMdStatistics ;




//=================================================================================
//
// ClTempFilter
//
// Clase que implementa la definición de los filtros
//
//=================================================================================

class	ClTempFilter
{

protected:

// Serializacion
	m4uint32_t		m_iHandle ;
	ClStaticPoolStr	*m_poUpperPool ;

// Fijos
    m4uint32_t      m_iFilterId ;
    m4uint32_t      m_iUsedT3Id ;
    m4uint32_t      m_iUsedNodeId ;
    m4uint32_t      m_iSysSentenceItem ;
    m4uint32_t      m_iSysParamItem ;

    m4uint16_t      m_iSCRStart ;

	m4uint8_t		m_iBehaviourType ;
	m4uint8_t		m_iIsOwnNode ;

// Auxiliares
    m4uint8_t		m_iType ;

// Ordenación
	m4char_t		m_acNodeId[ M4CL_MAX_NODE_ID + 2 ] ;


public:


// Funciones de inicialización ================================================
	
        ClTempFilter( void ) ;
        ~ClTempFilter( void ) {} ;


// Funciones de lectura de la parte fija ======================================

	m4uint32_t	GetFilterIdHandle( void ) const
	{
		return( m_iFilterId ) ;
	}

	m4pcchar_t	GetFilterId( void ) const
	{
		return( m_poUpperPool->GetString( m_iFilterId ) ) ;
	}

	m4pcchar_t	GetUsedT3Id( void ) const
	{
		return( m_poUpperPool->GetString( m_iUsedT3Id ) ) ;
	}

	m4pcchar_t	GetUsedNodeId( void ) const
	{
		return( m_poUpperPool->GetString( m_iUsedNodeId ) ) ;
	}


// Otras funciones de lectura ================================================

    m4uint8_t    GetIsOwnNode(  void ) const
    {
        return( m_iIsOwnNode ) ;
    }

	m4uint8_t	GetType( void ) const
	{
		return( m_iType ) ;
	}


// Funciones de escritura de parte fija =======================================

    m4return_t  SetFilterId( m4pcchar_t ai_pccFilterId )
	{
		return( M4MdFacSetPoolId( ai_pccFilterId, m_iFilterId, m_poUpperPool ) ) ;
	}

    m4return_t  SetUsedT3Id( m4pcchar_t ai_pccT3Id )
	{
		return( M4MdFacSetPoolId( ai_pccT3Id, m_iUsedT3Id, m_poUpperPool ) ) ;
	}

    m4return_t  SetUsedNodeId( m4pcchar_t ai_pccNodeId )
	{
		return( M4MdFacSetPoolId( ai_pccNodeId, m_iUsedNodeId, m_poUpperPool ) ) ;
	}

    m4return_t  SetSysSentenceItem( m4pcchar_t ai_pccSysSentenceItem )
	{
		return( M4MdFacSetPoolId( ai_pccSysSentenceItem, m_iSysSentenceItem, m_poUpperPool ) ) ;
	}

    m4return_t  SetSysParamItem( m4pcchar_t ai_pccSysParamItem )
	{
		return( M4MdFacSetPoolId( ai_pccSysParamItem, m_iSysParamItem, m_poUpperPool ) ) ;
	}

	void	SetSCRStart( m4uint16_t ai_iSCRStart )
	{
		m_iSCRStart = ai_iSCRStart ;
	}

    void    SetBehaviourType(  m4uint8_t ai_iBehaviourType )
    {
        m_iBehaviourType = ai_iBehaviourType ;
    }

    void    SetIsOwnNode(  m4uint8_t ai_iIsOwnNode )
    {
        m_iIsOwnNode = ai_iIsOwnNode ;
    }


// Otras funciones de escritura ===============================================

	void	SetType( m4uint8_t ai_iType )
	{
		m_iType = ai_iType ;
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

