
//==============================================================================
//
// (c) Copyright  1991-2015 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4dm.dll
// File:                m4jmanager.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                05-01-2015
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module implements a class for java execution manager.
//
//
//==============================================================================

#include "m4jmanager.hpp"
#include "m4jhandledata.hpp"
#include "m4jcontext.hpp"
#include "dmres.hpp"
#include "m4objlog.hpp"
#include "channel.hpp"
#include "syncro.hpp"




//=================================================================================
//
// M4JavaManager
//
// Clase que define un manager de ejecución de java
//
//=================================================================================

// Funciones de inicialización ================================================
	
M4JavaManager::M4JavaManager( m4uint8_t ai_iCsType )
: m_oHandleProvider( _NewIdentifier( ai_iCsType ) )
{
}


M4JavaManager::~M4JavaManager( void )
{

	itMapJavaContext	itContext ;


    itContext = m_oContexts.begin() ;

	while( itContext != m_oContexts.end() )
	{
		if( itContext->first != NULL )
		{
			delete( itContext->first ) ;
		}

		if( itContext->second != NULL )
		{
			delete( itContext->second ) ;
		}
		itContext++ ;
	}

	m_oContexts.clear() ;
}


// Funciones de lectura =======================================================

m4pcchar_t	M4JavaManager::GetJavaClassName( m4double_t ai_dHandle ) const
{
	return( m_oHandleProvider.GetJavaClassName( ai_dHandle ) ) ;
}


// Funciones de ejecución =====================================================

m4return_t	M4JavaManager::CreateObject( ClChannel *ai_poChannel, m4pcchar_t ai_pccClassPath, m4pcchar_t ai_pccIdentifier, m4pcchar_t ai_pccClassName, M4JavaObject ai_apvArguments[], m4uint16_t ai_iLength, m4double_t &ao_rdHandle, m4pcchar_t ai_pccSource )
{

	m4return_t		iResult = NULL ;
	m4double_t		dNewHandle = 0 ;
	M4JavaContext	*poContext = NULL ;


	ao_rdHandle = 0 ;

	poContext = _GetContext( ai_poChannel, ai_pccSource, ai_pccClassPath, ai_pccIdentifier ) ;

	if( poContext == NULL )
	{
		return( M4_ERROR ) ;
	}

	iResult = poContext->CreateObject( ai_pccClassName, ai_apvArguments, ai_iLength, dNewHandle, M4_TRUE, ai_pccSource ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	ao_rdHandle = dNewHandle ;

	return( M4_SUCCESS ) ;
}


m4return_t	M4JavaManager::DestroyObject( ClChannel *ai_poChannel, m4double_t ai_dHandle, m4pcchar_t ai_pccSource )
{

	m4return_t			iResult = NULL ;
	M4JavaHandleData	*poHandleData = NULL ;
	M4JavaContext		*poContext = NULL ;


	poHandleData = m_oHandleProvider.GetHandleData( ai_dHandle ) ;

	if( poHandleData == NULL )
	{
		return( M4_ERROR ) ;
	}

	poContext = poHandleData->GetContext() ;

	if( poContext == NULL )
	{
		return( M4_ERROR ) ;
	}

	iResult = poContext->DestroyObject( ai_dHandle, M4_TRUE, ai_pccSource ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	M4JavaManager::InvokeMethod( ClChannel *ai_poChannel, m4double_t ai_dHandle, m4pcchar_t ai_pccMethod, M4JavaObject ai_apvArguments[], m4uint16_t ai_iLength, M4JavaObject &ao_rvResult, m4pcchar_t ai_pccSource )
{

	m4return_t			iResult = NULL ;
	M4JavaHandleData	*poHandleData = NULL ;
	M4JavaContext		*poContext = NULL ;


	ao_rvResult.SetNull() ;

	poHandleData = m_oHandleProvider.GetHandleData( ai_dHandle ) ;

	if( poHandleData == NULL )
	{
		return( M4_ERROR ) ;
	}

	poContext = poHandleData->GetContext() ;

	if( poContext == NULL )
	{
		return( M4_ERROR ) ;
	}

	iResult = poContext->InvokeMethod( ai_dHandle, ai_pccMethod, ai_apvArguments, ai_iLength, ao_rvResult, M4_TRUE, ai_pccSource ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	M4JavaManager::InvokeMethod( ClChannel *ai_poChannel, m4pcchar_t ai_pccClassPath, m4pcchar_t ai_pccIdentifier, m4pcchar_t ai_pccClassName, m4pcchar_t ai_pccMethod, M4JavaObject ai_apvArguments[], m4uint16_t ai_iLength, M4JavaObject &ao_rvResult, m4pcchar_t ai_pccSource )
{

	m4return_t		iResult = NULL ;
	M4JavaContext	*poContext = NULL ;


	ao_rvResult.SetNull() ;

	poContext = _GetContext( ai_poChannel, ai_pccSource, ai_pccClassPath, ai_pccIdentifier ) ;

	if( poContext == NULL )
	{
		return( M4_ERROR ) ;
	}

	iResult = poContext->InvokeMethod( ai_pccClassName, ai_pccMethod, ai_apvArguments, ai_iLength, ao_rvResult, M4_TRUE, ai_pccSource ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}


// Funciones internas =========================================================

m4int16_t	M4JavaManager::_NewIdentifier( m4uint8_t ai_iCsType )
{

	static	m4uint16_t	s_iIdentifier = 99 ;
	static ClMutex		s_oMutex( M4_TRUE ) ;


	s_oMutex.Lock() ;

	if( s_iIdentifier == 0 )
	{
		s_iIdentifier = 99 ;
	}

	m4int16_t	iIdentifier = s_iIdentifier-- ;

	s_oMutex.Unlock() ;

	if( ai_iCsType == M4CL_CSTYPE_FRONT )
	{
		iIdentifier += 800 ;
	}
	else
	{
		iIdentifier += 900 ;
	}

	return( iIdentifier ) ;
}


M4JavaContext*	M4JavaManager::_GetContext( ClChannel *ai_poChannel, m4pcchar_t ai_pccSource, m4pcchar_t ai_pccClassPath, m4pcchar_t ai_pccIdentifier )
{

	m4return_t			iResult = NULL ;
	m4uint32_t			iLength = 0 ;
	m4pchar_t			pcIdentifier = NULL ;
	M4JavaContext		*poContext = NULL ;
	ClChannelManager	*poChannelManager = NULL ;
	IExecutor			*poExecutor = NULL ;
	ClVMBaseEnv			*poEnvironment = NULL ;
	itMapJavaContext	itContext ;


	CHECK_CHLOG_ERRORF( ai_poChannel == NULL, NULL, M4_CH_DM_NULL_ARGUMENT, "ai_poChannel" << "M4JavaManager::_GetContext" << __LINE__ << __FILE__ ) ;

	if( ai_pccIdentifier == NULL )
	{
		ai_pccIdentifier = "" ;
	}

    itContext = m_oContexts.find( (m4pchar_t)ai_pccIdentifier ) ;

    if( itContext != m_oContexts.end() )
	{
		if( itContext->second->IsReady() == M4_TRUE )
		{
			// Si está en el mapa y es válida se usa
			poContext = itContext->second ;
		}
		else
		{
			// Si está en el mapa y no es válida no se usa y se quita del mapa
			if( itContext->first != NULL )
			{
				delete( itContext->first ) ;
			}

			if( itContext->second != NULL )
			{
				itContext->second->Dispose() ;
			}

			m_oContexts.erase( itContext ) ;
		}
	}

	if( poContext == NULL )
	{
		// Si no existe se construye
		poChannelManager = ai_poChannel->GetpChannelManager() ;

		if( poChannelManager != NULL )
		{
			poExecutor = poChannelManager->GetpExecutor() ;
			poEnvironment = poChannelManager->GetEnvironment() ;
		}

		poContext = new M4JavaContext( &m_oHandleProvider, poExecutor ) ;
		CHECK_CHLOG_ERRORF( poContext == NULL, NULL, M4_CH_DM_NO_MEMORY, m4uint32_t( sizeof( M4JavaContext ) ) << __LINE__ << __FILE__ ) ;

		iResult = poContext->Open( poEnvironment, ai_pccSource, ai_pccClassPath, ai_pccIdentifier ) ;

		if( iResult != M4_SUCCESS )
		{
			delete( poContext ) ;
			return( NULL ) ;
		}

		iLength = strlen( ai_pccIdentifier ) ;
		pcIdentifier = new m4char_t[ iLength + 1 ] ;
		CHECK_CHLOG_ERRORF( pcIdentifier == NULL, NULL, M4_CH_DM_NO_MEMORY, m4uint32_t( ( iLength + 1 ) * sizeof( m4char_t ) ) << __LINE__ << __FILE__ ) ;
		memcpy( pcIdentifier, ai_pccIdentifier, iLength + 1 ) ;

		m_oContexts.insert( MapJavaContext::value_type( pcIdentifier, poContext ) ) ;
	}

	ai_poChannel->ReferenceJavaContext( poContext ) ;

	return( poContext ) ;
}


// Funciones de ciclo de vida =================================================

m4return_t	M4JavaManager::UnReferenceJavaContext( M4JavaContext *ai_poContext )
{

	itMapJavaContext	itContext ;


	CHECK_CHLOG_ERRORF( ai_poContext == NULL, NULL, M4_CH_DM_NULL_ARGUMENT, "ai_poContext" << "M4JavaManager::UnReferenceJavaContext" << __LINE__ << __FILE__ ) ;

	if( ai_poContext->IsReferenced() == M4_FALSE )
	{
		if( ai_poContext->IsDisposed() == M4_FALSE )
		{
			itContext = m_oContexts.find( (m4pchar_t)ai_poContext->GetIdentifier() ) ;

			if( itContext != m_oContexts.end() )
			{
				if( itContext->first != NULL )
				{
					delete( itContext->first ) ;
				}

				m_oContexts.erase( itContext ) ;
			}
		}
		delete( ai_poContext ) ;
	}

	return( M4_SUCCESS ) ;
}


