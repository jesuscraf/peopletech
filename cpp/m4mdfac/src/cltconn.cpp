
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltconn.cpp   
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
//    This module defines the functions for the temporary connector of nodes
//
//
//==============================================================================


#include "cltconn.hpp"
#include "m4mdrt.hpp"
#include "cltnode.hpp"
#include "cltsent.hpp"





//=================================================================================
//
// ClTempSentence
//
// Clase que implementa la definición de las sentencias
//
//=================================================================================

// Funciones de inicialización ================================================

ClTempConnector :: ClTempConnector( void )
{
	m_iHandle = 0 ;
	m_poUpperPool = NULL ;

    m_iType = 0 ;
    m_iExpandDelete = 0 ;
	m_iExpandUndo = 0 ;
    m_poFatherNode = NULL ;
    m_poSonNode = NULL ;

    m_poSentence = NULL ;
	m_iIsToCompile = 1 ;

	m_acFatherNodeId[ 0 ] = '\0' ;
	m_acFatherNodeId[ 1 ] = '\0' ;
}



// Funciones de serialización =================================================

m4return_t	ClTempConnector :: GetSize( m4uint32_t &ao_iSize, ClMdStatistics *ai_poStatistics )
{

// Fijos ======================================================================

    ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	m_iHandle = ao_iSize ;

	ao_iSize += M4CL_MCR_NODE_CONNECTOR_FIXED_SIZE ;


	if( ai_poStatistics != NULL )
	{
		ai_poStatistics->AddValue( M4MDRT_STT_CONNECTOR, ao_iSize - m_iHandle ) ;
	}

    return( M4_SUCCESS ) ;
}


m4return_t	ClTempConnector :: Compile( m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize )
{

	m4uint32_t	    iHandle ;
	m4uint32_t	    iOffset ;

    
    iOffset = m_iHandle ;

// fijos

    iHandle = m_poFatherNode->GetHandle() ;
	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    iHandle = m_poSonNode->GetHandle() ;
	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    if( m_poSentence != NULL )
    {
        iHandle = m_poSentence->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    M4ClPackInt8( ai_pcBuffer, iOffset, m_iType ) ;
    M4ClPackInt8( ai_pcBuffer, iOffset, m_iExpandDelete ) ;
    M4ClPackInt8( ai_pcBuffer, iOffset, m_iExpandUndo ) ;

    return( M4_SUCCESS ) ;
}


