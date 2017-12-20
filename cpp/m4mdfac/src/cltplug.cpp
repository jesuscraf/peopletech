
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltplug.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                30-07-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the functios of the temporary plug of nodes
//
//
//==============================================================================


#include "m4facres.hpp"

#include "cltplug.hpp"
#include "m4mdrt.hpp"
#include "cltiplug.hpp"
#include "chlog.hpp"




//=================================================================================
//
// ClTempPlug
//
// Clase que implementa la definición de los carriles
//
//=================================================================================

// Funciones de inicialización ================================================
	
ClTempPlug :: ClTempPlug( void )
{
	m_iHandle = 0 ;
	m_poUpperPool = NULL ;
	m_poNoUpperPool = NULL ;

	m_iUsedT3 = 0 ;
	m_iUsedT3Name = 0 ;
	m_iUsedNode = 0 ;

	m_iSCRStart = 0 ;

    m_iType = 0 ;
    m_iIsFilter = 0 ;
	m_iRelationTypeId = 0 ;
	m_iIsMultiselection = 0 ;

	m_poNode = NULL ;

	m_iItemsToCompile = 0 ;

	m_acNodeId[ 0 ] = '\0' ;
	m_acNodeId[ 1 ] = '\0' ;
}



// Otras funciones de lectura ================================================

ClTempPlugItem*	ClTempPlug :: GetItemByPosition(  m4uint16_t ai_iPosition )
{

    m4return_t      iResult ;
    m4pvoid_t       pvElement ;
    ClTempPlugItem	*poResult ;


	iResult = m_oItems.GetElement( ai_iPosition, pvElement ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    poResult = ( ClTempPlugItem* ) pvElement ;

	return( poResult ) ;
}



// Funciones de serialización =================================================

m4return_t	ClTempPlug :: GetSize( m4uint32_t &ao_iSize, ClMdStatistics *ai_poStatistics )
{

    m4return_t	    iResult ;
	m4uint8_t		iIsToCompile ;
	m4uint16_t	    i ;
	m4uint16_t	    iLength ;
	m4pvoid_t	    pvPointer ;
    ClTempPlugItem  *poItem ;


// Fijos ======================================================================

    ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	m_iHandle = ao_iSize ;

	ao_iSize += M4CL_MCR_PLUG_FIXED_SIZE ;


// Variables ==================================================================

//	iLength = m_oItems.GetLength() ;

	ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	ao_iSize += m_iItemsToCompile * M4_SIZE_OF_INT32 ;


	if( ai_poStatistics != NULL )
	{
		ai_poStatistics->AddValue( M4MDRT_STT_T3_CON, ao_iSize - m_iHandle ) ;
	}

// Componentes ================================================================
    
// plug items

    iLength = m_oItems.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oItems.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poItem = ( ClTempPlugItem* ) pvPointer ;
		iIsToCompile = poItem->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			iResult = poItem->GetSize( ao_iSize, ai_poStatistics ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}


    return( M4_SUCCESS ) ;
}


m4return_t	ClTempPlug :: Compile( m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize, m4uint32_t ai_iPoolSize )
{

    m4return_t	    iResult ;
	m4uint8_t		iIsToCompile ;
	m4uint16_t	    i ;
	m4uint16_t	    iLength ;
	m4uint32_t	    iOffset ;
	m4uint32_t	    iHandle ;
	m4pvoid_t	    pvPointer ;
    ClTempPlugItem  *poItem ;


	iOffset = m_iHandle ;

// fijos

	M4ClPackInt32( ai_pcBuffer, iOffset, m_iUsedT3 + ai_iBufferSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iUsedT3Name + ai_iBufferSize + ai_iPoolSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iUsedNode + ai_iBufferSize ) ;

	M4ClPackInt16( ai_pcBuffer, iOffset, m_iSCRStart ) ;

    M4ClPackInt8( ai_pcBuffer, iOffset, m_iType ) ;
    M4ClPackInt8( ai_pcBuffer, iOffset, m_iIsFilter ) ;
    M4ClPackInt8( ai_pcBuffer, iOffset, m_iRelationTypeId ) ;
    M4ClPackInt8( ai_pcBuffer, iOffset, m_iIsMultiselection ) ;


	iLength = m_oItems.GetLength() ;

	iOffset = M4_ROUND_TO_4( iOffset ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, m_iItemsToCompile ) ;
    iOffset += M4_SIZE_OF_INT16 ;   // nop


    iOffset = M4_ROUND_TO_4( iOffset ) ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oItems.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poItem = ( ClTempPlugItem* ) pvPointer ;
		iIsToCompile = poItem->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			iHandle = poItem->GetHandle() ;
			M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
			iOffset = M4_ROUND_TO_4( iOffset ) ;
		}
	}


// variables
    
    iLength = m_oItems.GetLength() ;

    for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oItems.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poItem = ( ClTempPlugItem* ) pvPointer ;
		iIsToCompile = poItem->GetIsToCompile() ;

		if( iIsToCompile == 1 )
		{
			iResult = poItem->Compile( ai_pcBuffer, ai_iBufferSize ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
    		}
		}
	}

	return( M4_SUCCESS ) ;
}

