//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                cltfilt.cpp   
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
//    This module defines the functions for the temporary filters
//
//
//==============================================================================



#include "m4facres.hpp"

#include "cltfilt.hpp"
#include "m4mdrt.hpp"
#include "chlog.hpp"



//=================================================================================
//
// ClTempFilter
//
// Clase que implementa la definición de los filtros
//
//=================================================================================

// Funciones de inicialización ================================================
	
ClTempFilter :: ClTempFilter( void )
{
	m_iHandle = 0 ;
	m_poUpperPool = NULL ;

    m_iFilterId = 0 ;
    m_iUsedT3Id = 0 ;
    m_iUsedNodeId = 0 ;
    m_iSysSentenceItem = 0 ;
    m_iSysParamItem = 0 ;

	m_iSCRStart = 0 ;

	m_iBehaviourType = 0 ;
	m_iIsOwnNode = 0 ;

	m_iType = 0 ;

	m_acNodeId[ 0 ] = '\0' ;
	m_acNodeId[ 1 ] = '\0' ;
}



// Funciones de serialización =================================================

m4return_t	ClTempFilter :: GetSize( m4uint32_t &ao_iSize, ClMdStatistics *ai_poStatistics )
{

// Fijos ======================================================================

	ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	m_iHandle = ao_iSize ;

	ao_iSize += M4CL_MCR_FILTER_FIXED_SIZE ;


	if( ai_poStatistics != NULL )
	{
		ai_poStatistics->AddValue( M4MDRT_STT_FILTER, ao_iSize - m_iHandle ) ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	ClTempFilter :: Compile( m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize )
{

	m4uint32_t	iOffset ;


    iOffset = m_iHandle ;

	M4ClPackInt32( ai_pcBuffer, iOffset, m_iFilterId + ai_iBufferSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iUsedT3Id + ai_iBufferSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iUsedNodeId + ai_iBufferSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iSysSentenceItem + ai_iBufferSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iSysParamItem + ai_iBufferSize ) ;

	M4ClPackInt16( ai_pcBuffer, iOffset, m_iSCRStart ) ;

    M4ClPackInt8( ai_pcBuffer, iOffset, m_iBehaviourType ) ;
    M4ClPackInt8( ai_pcBuffer, iOffset, m_iIsOwnNode ) ;

    return( M4_SUCCESS ) ;
}



