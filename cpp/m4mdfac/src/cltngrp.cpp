
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltngrp.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                30-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the functions for the temporary node group
//
//
//==============================================================================


#include "m4facres.hpp"

#include "cltngrp.hpp"
#include "cltnitem.hpp"
#include "m4mdrt.hpp"
#include "chlog.hpp"




//=================================================================================
//
// ClTempGroup
//
// Clase que implementa la definición de los grupos
//
//=================================================================================
	
ClTempGroup :: ClTempGroup( void )
{
	m_iHandle = 0 ;
	m_poUpperPool = NULL ;

	m_iGroupId = 0 ;

	m_oNitems.SetDelta( 0 ) ;
}



// Otras funciones de escritura ===============================================

m4return_t	ClTempGroup :: AddNitem( ClTempNitem* ai_poNitem )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_poNitem == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iResult = m_oNitems.AddElement( ai_poNitem ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}



// Funciones de serialización =================================================

m4return_t	ClTempGroup :: GetSize( m4uint32_t &ao_iSize, ClMdStatistics *ai_poStatistics )
{

	m4uint16_t  iLength ;


// Fijos ======================================================================

	ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	m_iHandle = ao_iSize ;

	ao_iSize += M4CL_MCR_GROUP_FIXED_SIZE ;


// Variables ==================================================================

    iLength = m_oNitems.GetLength() ;

    ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	ao_iSize += M4_SIZE_OF_INT32 ; // medio nop

    ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	ao_iSize += iLength * M4_SIZE_OF_INT32 ;


	if( ai_poStatistics != NULL )
	{
		ai_poStatistics->AddValue( M4MDRT_STT_GROUP, ao_iSize - m_iHandle ) ;
	}

    return( M4_SUCCESS ) ;
}


m4return_t	ClTempGroup :: Compile( m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize )
{

	m4return_t		iResult ;
	m4uint16_t		i ;
	m4uint16_t		iLength ;
	m4uint32_t		iOffset ;
	m4uint32_t		iHandle ;
	m4pvoid_t	    pvPointer ;
	ClTempNitem	    *poNitem ;


	iOffset = m_iHandle ;

// fijos

// variables

	iLength = m_oNitems.GetLength() ;

	iOffset = M4_ROUND_TO_4( iOffset ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, iLength ) ;
    iOffset += M4_SIZE_OF_INT16 ;   // nop

    iOffset = M4_ROUND_TO_4( iOffset ) ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oNitems.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		if( pvPointer != NULL )
		{
			poNitem = ( ClTempNitem* ) pvPointer ;
            iHandle = poNitem->GetHandle() ;

			M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
			iOffset = M4_ROUND_TO_4( iOffset ) ;
		}
	}

    return( M4_SUCCESS ) ;
}


