
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltsent.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                13-10-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines functions for the temporary sentence
//
//
//==============================================================================



#include "m4facres.hpp"

#include "cltsent.hpp"
#include "m4mdrt.hpp"
#include "chlog.hpp"




//=================================================================================
//
// ClTempSentence
//
// Clase que implementa la definición de las sentencias
//
//=================================================================================
	
// Funciones de inicialización ================================================

ClTempSentence :: ClTempSentence( void )
{
	m_iHandle = 0 ;
	m_poUpperPool = NULL ;
	m_poNoUpperPool = NULL ;

	m_iSentenceId = 0 ;
	m_iGroupObjects = 0 ;
	m_iApiSql1 = 0 ;
	m_iApiSql2 = 0 ;
	m_iApiSql3 = 0 ;
	m_iApiSql4 = 0 ;
	m_iMaxNumCol = 0 ;

	m_iIsToCompile = 1 ;
}



// Funciones de serialización =================================================

m4return_t	ClTempSentence :: GetSize( m4uint32_t &ao_iSize, ClMdStatistics *ai_poStatistics )
{
    
// Fijos ======================================================================

	ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	m_iHandle = ao_iSize ;

	ao_iSize += M4CL_MCR_SENTENCE_FIXED_SIZE ;


	if( ai_poStatistics != NULL )
	{
		ai_poStatistics->AddValue( M4MDRT_STT_SENTENCE, ao_iSize - m_iHandle ) ;
	}

    return( M4_SUCCESS ) ;
}


m4return_t	ClTempSentence :: Compile( m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize, m4uint32_t ai_iPoolSize )
{

	m4uint32_t	iOffset ;


    iOffset = m_iHandle ;

// fijos

	M4ClPackInt32( ai_pcBuffer, iOffset, m_iSentenceId + ai_iBufferSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iGroupObjects + ai_iBufferSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iApiSql1 + ai_iBufferSize + ai_iPoolSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iApiSql2 + ai_iBufferSize + ai_iPoolSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iApiSql3 + ai_iBufferSize + ai_iPoolSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iApiSql4 + ai_iBufferSize + ai_iPoolSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iMaxNumCol ) ;

    return( M4_SUCCESS ) ;
}



