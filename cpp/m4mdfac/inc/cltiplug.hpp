
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltiplug.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                09-10-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the temporary plug item
//
//
//==============================================================================



#include "mdfacglb.hpp"
#include "cltypes.hpp"
#include "cldefine.hpp"
#include "spoolstr.h"



#ifndef __CLTIPLUG_HPP__
#define __CLTIPLUG_HPP__



//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class   ClTempNitem ;
class	ClMdStatistics ;



//=================================================================================
//
// ClTempPlugItem
//
// Clase que implementa la definición de los items de carril
//
//=================================================================================

class	ClTempPlugItem
{

protected:

// Serializacion
	m4uint32_t		m_iHandle ;
	ClStaticPoolStr	*m_poUpperPool ;

// Fijos
    m4uint32_t		m_iUsedItemId ;
    m4uint8_t		m_iRelation ;

// Auxiliares
    ClTempNitem     *m_poSonNitem ;

// Client server
    m4uint8_t		m_iIsToCompile ;

// Ordenación
	m4char_t		m_acNodeId    [ M4CL_MAX_NODE_ID + 2 ] ;
	m4char_t		m_acUsedT3Id  [ M4CL_MAX_T3_ID   + 2 ] ;
	m4char_t		m_acUsedNodeId[ M4CL_MAX_NODE_ID + 2 ] ;
	m4char_t		m_acItemId    [ M4CL_MAX_ITEM_ID + 2 ] ;


public:


// Funciones de inicialización ================================================
	
        ClTempPlugItem( void ) ;
        ~ClTempPlugItem( void ) {} ;


// Otras funciones de lectura ================================================

	ClTempNitem*	GetSonNitem( void )
    {
        return( m_poSonNitem ) ;
    }


// Funciones de escritura de parte fija =======================================

	m4return_t	SetUsedItemId( m4pcchar_t ai_pccUsedItemId )
	{
		return( M4MdFacSetPoolId( ai_pccUsedItemId, m_iUsedItemId, m_poUpperPool ) ) ;
	}

    void    SetRelation( m4uint8_t a_irelation )
    {
        m_iRelation = a_irelation ;
    }


// Otras funciones de escritura ===============================================

    void    SetSonNitem( ClTempNitem *ai_poSonNitem )
    {
        m_poSonNitem = ai_poSonNitem ;
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

	m4return_t	SetNodeId( m4pcchar_t ai_pccNodeId, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccNodeId, m_acNodeId, M4CL_MAX_NODE_ID, ai_bConvert ) ) ;
	}

	m4pcchar_t	GetUsedT3Id( void ) const
	{
		return( m_acUsedT3Id + 1 ) ;
	}

	m4return_t	SetUsedT3Id( m4pcchar_t ai_pccUsedT3Id, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccUsedT3Id, m_acUsedT3Id, M4CL_MAX_NODE_ID, ai_bConvert ) ) ;
	}

	m4pcchar_t	GetUsedNodeId( void ) const
	{
		return( m_acUsedNodeId + 1 ) ;
	}

	m4return_t	SetUsedNodeId( m4pcchar_t ai_pccUsedNodeId, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccUsedNodeId, m_acUsedNodeId, M4CL_MAX_T3_ID, ai_bConvert ) ) ;
	}

	m4pcchar_t	GetItemId( void ) const
	{
		return( m_acItemId + 1 ) ;
	}

	m4return_t	SetItemId( m4pcchar_t ai_pccItemId, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccItemId, m_acItemId, M4CL_MAX_ITEM_ID, ai_bConvert ) ) ;
	}

} ;



#endif

