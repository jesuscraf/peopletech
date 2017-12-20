
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltindex.cpp   
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
//    This module defines the functions for the temporary index
//
//
//==============================================================================



#include "m4facres.hpp"

#include "cltindex.hpp"
#include "cltindit.hpp"
#include "m4mdrt.hpp"
#include "chlog.hpp"
#include "spoolstr.h"



//=================================================================================
//
// ClTempIndex
//
// Clase que implementa la definición de los indices
//
//=================================================================================

// Funciones de inicialización ================================================
	
ClTempIndex :: ClTempIndex( void )
{
	m_iHandle = 0 ;
	m_poUpperPool = NULL ;

	m_iIndexId = 0 ;

    m_iIsDefault = 0 ;
	m_poNode = NULL ;

	m_iItemsToCompile = 0 ;

	m_acNodeId[ 0 ] = '\0' ;
	m_acNodeId[ 1 ] = '\0' ;
}


// Otras funciones de lectura ================================================

ClTempIndexItem*	ClTempIndex :: GetItemByPosition(  m4uint16_t ai_iPosition )
{

    m4return_t      iResult ;
    m4pvoid_t       pvElement ;
    ClTempIndexItem	*poResult ;


	iResult = m_oItems.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempIndexItem* ) pvElement ;

	return( poResult ) ;
}



// Funciones de serialización =================================================

m4return_t	ClTempIndex :: GetSize( m4uint32_t &ao_iSize, ClMdStatistics *ai_poStatistics )
{

// Fijos ======================================================================

	ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	m_iHandle = ao_iSize ;

	ao_iSize += M4CL_MCR_INDEX_FIXED_SIZE ;


// Variables ==================================================================

//	iLength = m_oItems.GetLength() ;

	ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	ao_iSize += m_iItemsToCompile * M4CL_MCR_INDEX_ITEM_SIZE ;


	if( ai_poStatistics != NULL )
	{
		ai_poStatistics->AddValue( M4MDRT_STT_INDEX, ao_iSize - m_iHandle ) ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	ClTempIndex :: Compile( m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize )
{

	m4return_t			iResult ;
	m4uint8_t			iFunction ;
	m4uint8_t			iWay ;
	m4uint8_t			iIsToCompile ;
	m4uint16_t			i ;
	m4uint16_t			iLength ;
	m4uint32_t			iOffset ;
	m4uint32_t			iHandle ;
	m4pvoid_t           pvPointer ;
    ClTempIndexItem     *poIndexItem ;

    
    iOffset = m_iHandle ;

// fijos

	M4ClPackInt32( ai_pcBuffer, iOffset, m_iIndexId ) ;

    iLength = m_oItems.GetLength() ;

	iOffset = M4_ROUND_TO_4( iOffset ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, m_iItemsToCompile ) ;


// variables

    iOffset = M4_ROUND_TO_4( iOffset ) ;

    for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oItems.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poIndexItem = ( ClTempIndexItem* ) pvPointer ;
		iIsToCompile = poIndexItem->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			iHandle = poIndexItem->GetNitemHandle() ;
			iFunction = poIndexItem->GetFunction() ;
			iWay = poIndexItem->GetWay() ;

			M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
			M4ClPackInt8( ai_pcBuffer, iOffset, iFunction ) ;
			M4ClPackInt8( ai_pcBuffer, iOffset, iWay ) ;
			iOffset = M4_ROUND_TO_4( iOffset ) ;
		}
	}

	return( M4_SUCCESS ) ;
}



