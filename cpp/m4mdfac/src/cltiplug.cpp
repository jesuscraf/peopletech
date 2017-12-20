
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltiplug.cpp   
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
//    This module defines the functions for the temporary plug item
//
//
//==============================================================================


#include "m4facres.hpp"

#include "cltiplug.hpp"
#include "m4mdrt.hpp"
#include "chlog.hpp"
#include "cltnitem.hpp"




//=================================================================================
//
// ClTempPlugItem
//
// Clase que implementa la definición de los items de carril
//
//=================================================================================

// Funciones de inicialización ================================================
	
ClTempPlugItem :: ClTempPlugItem( void )
{
	m_iHandle = 0 ;
	m_poUpperPool = NULL ;

	m_iUsedItemId = 0 ;
    m_iRelation = 0 ;

    m_poSonNitem = 0 ;

	m_iIsToCompile = 1 ;

	m_acNodeId    [ 0 ] = '\0' ;
	m_acNodeId    [ 1 ] = '\0' ;
	m_acUsedT3Id  [ 0 ] = '\0' ;
	m_acUsedT3Id  [ 1 ] = '\0' ;
	m_acUsedNodeId[ 0 ] = '\0' ;
	m_acUsedNodeId[ 1 ] = '\0' ;
	m_acItemId    [ 0 ] = '\0' ;
	m_acItemId    [ 1 ] = '\0' ;
}



// Funciones de serialización =================================================

m4return_t	ClTempPlugItem :: GetSize( m4uint32_t &ao_iSize, ClMdStatistics *ai_poStatistics )
{

// Fijos ======================================================================

    ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	m_iHandle = ao_iSize ;

	ao_iSize += M4CL_MCR_PLUG_ITEM_FIXED_SIZE ;


	if( ai_poStatistics != NULL )
	{
		ai_poStatistics->AddValue( M4MDRT_STT_T3_ITEM_CON, ao_iSize - m_iHandle ) ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	ClTempPlugItem :: Compile( m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize )
{

	m4uint32_t  iOffset ;
	m4uint32_t  iHandle ;

    
    iOffset = m_iHandle ;


	M4ClPackInt32( ai_pcBuffer, iOffset, m_iUsedItemId + ai_iBufferSize ) ;

    if( m_poSonNitem != NULL )
    {
        iHandle = m_poSonNitem->GetHandle() ;
    }
    else
    {
        iHandle = 0 ;
    }

	M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;

    M4ClPackInt8( ai_pcBuffer, iOffset, m_iRelation ) ;


	return( M4_SUCCESS ) ;
}


