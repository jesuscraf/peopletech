
//==============================================================================
//
// (c) Copyright  1991-2015 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4java.dll
// File:                m4jhandleprovider.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                21-01-2015
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module implements a class for providing java object handles.
//
//
//==============================================================================

#include "m4jhandleprovider.hpp"
#include "m4jhandledata.hpp"
#include "m4javares.hpp"
#include "m4objlog.hpp"
#include <math.h>



#define	M4_MIN_HANDLE					-99999999999.999
#define	M4_MAX_HANDLE					          -0.701


extern	m4double_t	M4VMRound( m4double_t ai_dValue, int ai_iScale ) ;


m4int8_t	M4HandleCompare( m4double_t ai_dHandle1, m4double_t ai_dHandle2 )
{
	m4double_t	dDifference = ai_dHandle1 - ai_dHandle2 ;

	if( fabs( dDifference ) <= 10e-4 )
	{
		return( 0 ) ;
	}

	if( dDifference > 0 )
	{
		return( 1 ) ;
	}

	return( -1 ) ;
}



//=================================================================================
//
// M4JavaHandleProvider
//
// Clase que define un proveedor de handles de objetos java
//
//=================================================================================

// Funciones de inicialización ================================================
	
M4JavaHandleProvider::M4JavaHandleProvider( m4uint16_t ai_iIdentifier )
{
	m_bRecycling = M4_FALSE ;
	m_iIdentifier = ai_iIdentifier ;
	m_dFirstHandle = -99999999999.0 - m4double_t( m_iIdentifier ) / 1000 ;
	m_dHandle = m_dFirstHandle ;
}


M4JavaHandleProvider::~M4JavaHandleProvider( void )
{

	itMapHandleData	itHandleData ;


    itHandleData = m_oHandleData.begin() ;

	while( itHandleData != m_oHandleData.end() )
	{
		if( itHandleData->second != NULL )
		{
			delete( itHandleData->second ) ;
		}
		itHandleData++ ;
	}
	m_oHandleData.clear() ;
}


// Funciones de lectura =======================================================

m4pcchar_t	M4JavaHandleProvider::GetJavaClassName( m4double_t ai_dHandle ) const
{

	const_itMapHandleData	itHandleData ;


    itHandleData = m_oHandleData.find( ai_dHandle ) ;

	if( itHandleData != m_oHandleData.end() )
	{
		return( itHandleData->second->GetJavaClassName() ) ;
	}

	return( NULL ) ;
}


M4JavaHandleData*	M4JavaHandleProvider::GetHandleData( m4double_t ai_dHandle ) const
{
	const_itMapHandleData	itHandleData ;
	return( _GetHandleData( ai_dHandle, itHandleData ) ) ;
}


// Funciones de ciclo de vida =================================================

m4return_t	M4JavaHandleProvider::NewObject( m4pcchar_t ai_pccClassName, M4JavaContext* ai_poContext )
{

	m4double_t			dHandle = 0 ;
	M4JavaHandleData	*poHandleData = NULL ;


	dHandle = m_dHandle ;
	_NextHandle() ;

	poHandleData = new M4JavaHandleData( ai_pccClassName, ai_poContext ) ;
	CHECK_CHLOG_ERRORF( poHandleData == NULL, M4_ERROR, M4JAVA_NO_MEMORY, m4uint32_t( sizeof( M4JavaHandleData ) ) << __LINE__ << __FILE__ ) ;

	m_oHandleData.insert( MapHandleData::value_type( dHandle, poHandleData ) ) ;

	return( M4_SUCCESS ) ;
}


m4return_t	M4JavaHandleProvider::DeleteObject( m4double_t ai_dHandle )
{

	M4JavaHandleData		*poHandleData = NULL ;
	const_itMapHandleData	itHandleData ;


	poHandleData = _GetHandleData( ai_dHandle, itHandleData ) ;

	if( poHandleData == NULL )
	{
		return( M4_ERROR ) ;
	}

	delete( poHandleData ) ;
	m_oHandleData.erase( itHandleData ) ;
	return( M4_SUCCESS ) ;
}


// Funciones internas =========================================================

M4JavaHandleData*	M4JavaHandleProvider::_GetHandleData( m4double_t ai_dHandle, const_itMapHandleData& ai_roIterator ) const
{

	m4int16_t	iIdentifier = 0 ;


    ai_roIterator = m_oHandleData.find( ai_dHandle ) ;

	if( ai_roIterator != m_oHandleData.end() )
	{
		return( ai_roIterator->second ) ;
	}

	// No es un handle
	CHECK_CHLOG_ERRORF( M4HandleCompare( ai_dHandle, M4_MIN_HANDLE ) < 0, NULL, M4JAVA_BAD_HANDLE, ai_dHandle ) ;
	CHECK_CHLOG_ERRORF( M4HandleCompare( ai_dHandle, M4_MAX_HANDLE ) > 0, NULL, M4JAVA_BAD_HANDLE, ai_dHandle ) ;

	iIdentifier = m4int16_t( M4VMRound( ( -ai_dHandle + m4int64_t( ai_dHandle ) ) * 1000, 0 ) ) ;

	// No es un handle
	CHECK_CHLOG_ERRORF( iIdentifier < 701, NULL, M4JAVA_BAD_HANDLE, ai_dHandle ) ;

	// No es de este channel manager
	CHECK_CHLOG_ERRORF( iIdentifier != m_iIdentifier, NULL, M4JAVA_NO_LOCAL_HANDLE, ai_dHandle ) ;

	// No es un handle
	CHECK_CHLOG_ERRORF( m_bRecycling == M4_FALSE && M4HandleCompare( ai_dHandle, m_dHandle ) >= 0, NULL, M4JAVA_BAD_HANDLE, ai_dHandle ) ;

	// Handle ya borrado
	DUMP_CHLOG_ERRORF( M4JAVA_DELETED_HANDLE, ai_dHandle ) ;
	return( NULL ) ;
}


void	M4JavaHandleProvider::_NextHandle( void )
{

	m4double_t		dHandle = 0 ;
	m4double_t		dStartHandle = 0 ;
	itMapHandleData	itHandleData ;


	dHandle = m_dHandle + 1 ;

	if( M4HandleCompare( dHandle, M4_MAX_HANDLE ) > 0 )
	{
		dHandle = m_dFirstHandle ;

		if( m_bRecycling == M4_FALSE )
		{
			m_bRecycling = M4_TRUE ;
		}
	}

	if( m_bRecycling == M4_TRUE )
	{
		dStartHandle = dHandle ;

		while( 1 )
		{
		    itHandleData = m_oHandleData.find( dHandle ) ;

			if( itHandleData == m_oHandleData.end() )
			{
				break ;
			}

			dHandle++ ;

			if( M4HandleCompare( dHandle, M4_MAX_HANDLE ) > 0 )
			{
				dHandle = m_dFirstHandle ;
			}

			if( M4HandleCompare( dHandle, dStartHandle ) == 0 )
			{
				DUMP_CHLOG_ERROR( M4JAVA_NO_MORE_HANDLES ) ;
				dHandle = 0 ;
				break ;
			}
		}
	}

	m_dHandle = dHandle ;
}


