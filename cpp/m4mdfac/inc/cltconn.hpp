
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltconn.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                07-07-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the temporary connector of nodes
//
//
//==============================================================================



#include "cltypes.hpp"
#include "cldefine.hpp"
#include "mdfacglb.hpp"



#ifndef __CLTCONN_HPP__
#define __CLTCONN_HPP__



//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClTempNode ;
class	ClTempSentence ;
class	ClStaticPoolStr ;
class	ClMdStatistics ;



//=================================================================================
//
// ClTempConnector
//
// Clase que implementa la definición de los conectore
//
//=================================================================================

class	ClTempConnector
{

protected:

// Serializacion
	m4uint32_t		m_iHandle ;
	ClStaticPoolStr	*m_poUpperPool ;

// Fijos
    m4uint8_t		m_iType ;
    m4uint8_t		m_iExpandDelete ;
    m4uint8_t		m_iExpandUndo ;

// Auxiliares
    ClTempNode		*m_poFatherNode ;
    ClTempNode		*m_poSonNode ;

    ClTempSentence  *m_poSentence ;

// Client server
    m4uint8_t		m_iIsToCompile ;

// Ordenación
    m4char_t		m_acFatherNodeId[ M4CL_MAX_NODE_ID + 2 ] ;

public:


// Funciones de inicialización ================================================
	
        ClTempConnector( void ) ;
        ~ClTempConnector( void ) {} ;


// Funciones de lectura de la parte fija ======================================

    m4uint8_t GetType( void ) const
    {
        return( m_iType ) ;
    }


// Otras funciones de lectura ================================================

    ClTempNode*	GetFatherNode( void ) const
    {
        return( m_poFatherNode ) ;
    }

    ClTempNode*	GetSonNode( void ) const
    {
        return( m_poSonNode ) ;
    }


// Funciones de escritura de parte fija =======================================

    void    SetType( m4uint8_t ai_iType )
    {
        m_iType = ai_iType ;
    }

    void    SetExpandDelete( m4uint8_t ai_iExpandDelete )
    {
        m_iExpandDelete = ai_iExpandDelete ;
    }

    void    SetExpandUndo( m4uint8_t ai_iExpandUndo )
    {
        m_iExpandUndo = ai_iExpandUndo ;
    }


// Otras funciones de escritura ===============================================

    void    SetSentence( ClTempSentence *ai_poSentence )
    {
        m_poSentence = ai_poSentence ;
    }

    void    SetFatherNode( ClTempNode* ai_poFatherNode )
    {
        m_poFatherNode = ai_poFatherNode ;
    }

    void    SetSonNode( ClTempNode* ai_poSonNode )
    {
        m_poSonNode = ai_poSonNode ;
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

	m4pcchar_t	GetFatherNodeId( void ) const
	{
		return( m_acFatherNodeId + 1 ) ;
	}

	m4return_t	SetFatherNodeId( m4pcchar_t ai_pccNodeId, m4bool_t ai_bConvert )
	{
		return( M4MdFacSetLocalId( ai_pccNodeId, m_acFatherNodeId, M4CL_MAX_NODE_ID, ai_bConvert ) ) ;
	}

} ;



#endif

