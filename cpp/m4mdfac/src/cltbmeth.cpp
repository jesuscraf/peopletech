
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                cltbmeth.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                23-11-2007
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the functios for the temporary business method
//
//
//==============================================================================


#include "m4facres.hpp"

#include "cltbmeth.hpp"
#include "m4mdrt.hpp"
#include "chlog.hpp"
#include "cltnitem.hpp"
#include "cltnode.hpp"
#include "cltbarg.hpp"




//=================================================================================
//
// ClTempBusinessMethod
//
// Clase que implementa la definición de los métodos de negocio
//
//=================================================================================

// Funciones de inicialización ================================================
	
ClTempBusinessMethod :: ClTempBusinessMethod( void )
{
	m_iHandle = 0 ;
	m_poUpperPool = NULL ;
	m_poNoUpperPool = NULL ;

	m_iMethodId = 0 ;
	m_iName = 0 ;
	m_iDescription = 0 ;

	m_poNitem = NULL ;
}



// Funciones de serialización =================================================

m4return_t	ClTempBusinessMethod :: GetSize( m4uint32_t &ao_iSize, ClMdStatistics *ai_poStatistics )
{

	m4uint16_t	iLength ;


// Fijos ======================================================================

    ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	m_iHandle = ao_iSize ;

	ao_iSize += M4CL_MCR_BUSINESS_METHOD_FIXED_SIZE ;


// Variables ==================================================================

	iLength = m_oArguments.GetLength() ;

	ao_iSize = M4_ROUND_TO_4( ao_iSize ) ;
	ao_iSize += iLength * ( M4CL_MCR_BUSINESS_ARG_SIZE ) ;

	if( ai_poStatistics != NULL )
	{
		ai_poStatistics->AddValue( M4MDRT_STT_BUSINESS_METHOD, ao_iSize - m_iHandle ) ;
	}

    return( M4_SUCCESS ) ;
}


m4return_t	ClTempBusinessMethod :: Compile( m4pchar_t ai_pcBuffer, m4uint32_t ai_iBufferSize, m4uint32_t ai_iPoolSize )
{

    m4return_t			iResult ;
	m4uint16_t			i ;
	m4uint16_t			iLength ;
	m4uint32_t			iOffset ;
	m4uint32_t			iHandle ;
	m4pvoid_t			pvPointer ;
	ClTempNode			*poNode ;
    ClTempBusinessArg	*poArgument ;


	iOffset = m_iHandle ;

// fijos

	M4ClPackInt32( ai_pcBuffer, iOffset, m_iMethodId + ai_iBufferSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iName + ai_iBufferSize + ai_iPoolSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_iDescription + ai_iBufferSize + ai_iPoolSize ) ;
	M4ClPackInt32( ai_pcBuffer, iOffset, m_poNitem->GetHandle() ) ;

	iLength = m_oArguments.GetLength() ;

	iOffset = M4_ROUND_TO_4( iOffset ) ;
	M4ClPackInt16( ai_pcBuffer, iOffset, iLength ) ;
    iOffset += M4_SIZE_OF_INT16 ;   // nop


    iOffset = M4_ROUND_TO_4( iOffset ) ;

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oArguments.GetElement( i, pvPointer ) ;
		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS || pvPointer == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		poArgument = ( ClTempBusinessArg* ) pvPointer ;

		poNode = poArgument->GetNode() ;

		if( poNode != NULL )
		{
			iHandle = poNode->GetHandle() ;
		}
		else
		{
			iHandle = 0 ;
		}

		M4ClPackInt32( ai_pcBuffer, iOffset, iHandle ) ;
	    M4ClPackInt8 ( ai_pcBuffer, iOffset, poArgument->GetType() ) ;
		iOffset = M4_ROUND_TO_4( iOffset ) ;
	}


	return( M4_SUCCESS ) ;
}

