
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4objglb.dll
// File:                cllstr.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                26-05-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo implementa la clase para cadenas largas
//
//
//==============================================================================




#include "cllstr.hpp"

#include "m4glbres.hpp"
#include "chlog.hpp"
#include "m4unicode.hpp"


// Tamaños
#define	M4CL_LONG_STR_DEFAULT_DELTA		                        1024
#define	M4CL_LONG_STR_DEFAULT_INIT		                        4096



//=============================================================================
//
// ClLongString
//
// Clase que implementa una cadena dimensionable para los fórmulas polacas
//
//=============================================================================

m4return_t	ClLongString :: _Increment( m4uint32_t ai_iDelta )
{

	m4uint32_t	iSize ;
	m4pchar_t	pcTmp ;


	iSize = m_iSize + ai_iDelta ;

	if( iSize == 0 )
	{
		iSize = M4CL_LONG_STR_DEFAULT_INIT ;
	}

	pcTmp = new m4char_t[ iSize ] ;

	CHECK_CHLOG_DEBUGF( pcTmp == NULL, M4_ERROR, M4_OBJGLB_NO_MEMORY, m4uint32_t( sizeof( m4char_t ) * iSize ) << __LINE__ << __FILE__ ) ;

	m_iSize = iSize ;


	if( m_pcString != NULL )
	{
		if( m_iLength > 0 )
		{
			memcpy( pcTmp, m_pcString, sizeof( m4char_t ) * m_iLength ) ;
		}

		delete [] m_pcString ;
	}
	else
	{
		*pcTmp = '\0' ;
		m_iLength = 1 ;
	}

	m_pcString = pcTmp ;
		
	return( M4_SUCCESS ) ;
}


// Funciones de inicialización ================================================
	
ClLongString :: ClLongString( void )
{
	m_iSize = 0 ;
	m_iLength = 0 ;
	m_iDelta = M4CL_LONG_STR_DEFAULT_DELTA ;
	m_iInitSize = M4CL_LONG_STR_DEFAULT_INIT ;
	m_pcString = NULL ;
}


ClLongString :: ~ClLongString( void )
{
	if( m_pcString != NULL )
	{
		delete [] m_pcString ;
		m_pcString = NULL ;
	}
}


void	ClLongString :: Reset( void )
{
	if( m_pcString != NULL )
	{
		*m_pcString = '\0' ;
		m_iLength = 1 ;
	}
	else
	{
		m_iLength = 0 ;
	}
}


m4return_t	ClLongString :: Init( m4uint32_t ai_iSize, m4uint32_t ai_iDelta )
{
	if( ai_iSize == 0 )
	{
		ai_iSize = M4CL_LONG_STR_DEFAULT_INIT ;
	}

	if( ai_iDelta == 0 )
	{
		ai_iDelta = M4CL_LONG_STR_DEFAULT_DELTA ;
	}

	m_iSize = 0 ;
	m_iLength = 0 ;
	m_iDelta = ai_iDelta ;
	m_iInitSize = ai_iSize ;

	if( m_pcString != NULL )
	{
		delete [] m_pcString ;
		m_pcString = NULL ;
	}

	return( M4_SUCCESS ) ;
}


// Otras funciones de escritura ===============================================

m4return_t	ClLongString :: Concat( m4pcchar_t ai_pccString, m4uint32_t ai_iLength, m4uint8_t ai_iMode )
{

	m4uchar_t	c ;
	m4uint32_t	i ;
	m4uint32_t	iSize ;
	m4uint32_t	iIncrement ;
	m4pchar_t	pcString ;
	m4pcchar_t	pccScaped ;


	CHECK_CHLOG_DEBUGF( ai_pccString == NULL, M4_ERROR, M4_OBJGLB_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Si no es XML es normal y se hace tal cual ======== =========================

	if( ai_iMode != M4MDRT_CODE_MODE_XML )
	{
		pcString = Concat( ai_iLength ) ;
		CHECK_CHLOG_DEBUGF( pcString == NULL, M4_ERROR, M4_OBJGLB_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

		memcpy( pcString, ai_pccString, ai_iLength ) ;
		pcString[ ai_iLength ] = '\0' ;
		return( M4_SUCCESS ) ;
	}


// Si es XML hay que hacer el parseo ==========================================

	iIncrement = 0 ;

	for( i = 0 ; i < ai_iLength ; i++ )
	{
		c = ai_pccString[ i ] ;

		// UNICODE XML
		pccScaped = M4XMLScapeChar( c ) ;

		if( pccScaped != NULL )
		{
			iIncrement += strlen( pccScaped ) - 1 ;
		}
	}

	pcString = Concat( ai_iLength + iIncrement ) ;
	CHECK_CHLOG_DEBUGF( pcString == NULL, M4_ERROR, M4_OBJGLB_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	// Si no hay cambios va normal
	if( iIncrement == 0 )
	{
		memcpy( pcString, ai_pccString, ai_iLength ) ;
		pcString[ ai_iLength ] = '\0' ;
		return( M4_SUCCESS ) ;
	}

	
	for( i = 0 ; i < ai_iLength ; i++ )
	{
		c = ai_pccString[ i ] ;

		// UNICODE XML
		pccScaped = M4XMLScapeChar( c ) ;

		if( pccScaped != NULL )
		{
			iSize = strlen( pccScaped ) ;
			memcpy( pcString, pccScaped, iSize ) ;
			pcString += iSize ;
		}
		else
		{
			*pcString++ = c ;
		}
	}

	*pcString = '\0' ;


	return( M4_SUCCESS ) ;
}



m4return_t	ClLongString :: Concat( m4pcchar_t ai_pccString, m4uint8_t ai_iMode )
{
	return Concat( ai_pccString, strlen( ai_pccString ), ai_iMode ) ;
}



m4pchar_t	ClLongString :: Concat( m4uint32_t ai_iSize )
{

	m4pchar_t	pcResult ;
	m4return_t	iResult ;
	m4uint32_t	iDelta ;
	m4uint32_t	iSpace ;


	// Si no hay nada hay que reservar también para el '\0'
	if( m_iLength == 0 )
	{
		iSpace = ai_iSize + 1 ;
	}
	else
	{
		iSpace = ai_iSize ;
	}

	if( m_iLength + iSpace > m_iSize )
	{
		if( m_pcString != NULL )
		{
			iDelta = m_iDelta ;
		}
		else
		{
			iDelta = m_iInitSize ;
		}

		if( m_iSize + iDelta < m_iLength + iSpace )
		{
			iDelta = m_iLength + iSpace - m_iSize ;
		}

		iResult = _Increment( iDelta ) ;

		CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, NULL, M4_OBJGLB_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}

	pcResult = m_pcString + m_iLength - 1 ;
	m_iLength += ai_iSize ;

	return( pcResult ) ;
}



// Funciones varias ===========================================================

m4pchar_t	ClLongString :: Clone( void ) const
{

	m4pchar_t	pcResult ;


	if( m_iLength == 0 || m_pcString == NULL )
	{
		return( NULL ) ;
	}

	pcResult = new m4char_t[ m_iLength ] ;

	CHECK_CHLOG_DEBUGF( pcResult == NULL, pcResult, M4_OBJGLB_NO_MEMORY, m4uint32_t( sizeof( m4char_t ) * m_iLength ) << __LINE__ << __FILE__ ) ;

	memcpy( pcResult, m_pcString, m_iLength ) ;

	return( pcResult ) ;
}


