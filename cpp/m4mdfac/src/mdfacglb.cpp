
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                mdfacglb.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                25-04-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo tiene las funciones globales de la factoría
//
//
//==============================================================================



#include "mdfacglb.hpp"

#include "m4facres.hpp"
#include "chlog.hpp"
#include "spoolstr.h"




//=============================================================================
// Para asignar identificadores con longitud por delante en array local
//=============================================================================

m4return_t	M4MdFacSetLocalId( m4pcchar_t ai_pccSource, m4pchar_t ao_pcDest, m4uint16_t ai_iMaxLength, m4bool_t ai_bConvert )
{

	m4uint8_t	iTruncated = 0 ;
    m4uint16_t  iLength ;


	CHECK_CHLOG_DEBUGF( ai_pccSource == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

    iLength = strlen( ai_pccSource ) ;

    if( iLength > ai_iMaxLength )
    {
        iLength = ai_iMaxLength ;
		iTruncated = 1 ;
    }


	if( iLength != 0 )
	{
		*ao_pcDest = m4char_t( iLength ) ;
		memcpy( ao_pcDest + 1, ai_pccSource, sizeof( m4char_t) * iLength ) ;
		*( ao_pcDest + 1 + iLength ) = '\0' ;

		if( ai_bConvert == M4_TRUE )
		{
			strupr( ao_pcDest + 1 ) ;
		}
	}
	else
	{
		*ao_pcDest = '\0' ;
		*( ao_pcDest + 1 ) = '\0' ;
	}

	if( iTruncated == 1 )
	{
		DUMP_CHLOG_WARNINGF( M4_MDFAC_DATA_TRUNCATED, ai_pccSource << ai_iMaxLength << ao_pcDest + 1 ) ;
	}

    return( M4_SUCCESS ) ;
}



//=============================================================================
// Para asignar identificadores con longitud por delante en puntero local
//=============================================================================

m4return_t	M4MdFacSetLocalId( m4pcchar_t ai_pccSource, m4pchar_t& ao_rpcDest, m4bool_t ai_bConvert )
{

    m4uint16_t  iLength ;


	CHECK_CHLOG_DEBUGF( ai_pccSource == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	if( ao_rpcDest != NULL )
	{
		delete( ao_rpcDest ) ;
		ao_rpcDest = NULL ;
	}

    iLength = strlen( ai_pccSource ) ;

	if( iLength != 0 )
	{
		ao_rpcDest = new m4char_t[ iLength + 2 ] ;
		CHECK_CHLOG_ERRORF( ao_rpcDest == NULL, NULL, M4_MDFAC_NO_MEMORY, m4uint32_t( sizeof( m4char_t ) * ( iLength  + 2 ) ) << __LINE__ << __FILE__ ) ;

		*ao_rpcDest = m4char_t( iLength ) ;
		memcpy( ao_rpcDest + 1, ai_pccSource, sizeof( m4char_t) * iLength ) ;
		*( ao_rpcDest + 1 + iLength ) = '\0' ;

		if( ai_bConvert == M4_TRUE )
		{
			strupr( ao_rpcDest + 1 ) ;
		}
	}

    return( M4_SUCCESS ) ;
}



//=============================================================================
// Para asignar identificadores con longitud por delante en un pool
//=============================================================================

m4return_t	M4MdFacSetPoolId( m4pcchar_t ai_pccSource, m4uint32_t &ao_riDest, ClStaticPoolStr *ai_poPool )
{

	m4return_t	iResult ;


	CHECK_CHLOG_DEBUGF( ai_pccSource == NULL, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	iResult = ai_poPool->AddString( ai_pccSource, ao_riDest ) ;
	CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_MDFAC_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	return( M4_SUCCESS ) ;
}


