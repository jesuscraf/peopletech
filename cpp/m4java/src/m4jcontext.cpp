
//==============================================================================
//
// (c) Copyright  1991-2015 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4java.dll
// File:                m4jcontext.cpp   
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
//    This module implements a class for java execution contexts.
//
//
//==============================================================================

#include "m4jcontext.hpp"
#include "m4jconfiguration.hpp"
#include "m4javares.hpp"
#include "m4stl.hpp"
#include "m4objlog.hpp"
#include "cldates.hpp"
#include "blocksynchronization.hpp"


m4pchar_t	M4CloneString( m4pcchar_t ai_pccSource, m4int32_t ai_iLength = -1 ) ;
void		M4FreeString( m4pchar_t &aio_rpcString ) ;



#define	M4_JAVA_SERVICE						"com.meta4.javaservice.M4JavaService"

#ifdef _UNIX
	#define	M4_JAVA_PATH_SEPARATOR			":"
#else
	#define	M4_JAVA_PATH_SEPARATOR			";"
#endif




//=================================================================================
//
// M4JavaContext
//
// Clase que define un contexto de ejecución de java identificado por un classpath
//
//=================================================================================

// Funciones de inicialización ================================================
	
M4JavaContext::M4JavaContext( M4JavaHandleProvider *ai_poHandleProvider, IExecutor *ai_poExecutor )
{
	_Init( ai_poHandleProvider, ai_poExecutor ) ;
}


M4JavaContext::M4JavaContext( ClVMBaseEnv *ai_poEnvironment, m4pcchar_t ai_pccSource, m4pcchar_t ai_pccClassPath, m4pcchar_t ai_pccIdentifier, M4JavaHandleProvider *ai_poHandleProvider, IExecutor *ai_poExecutor )
{
	_Init( ai_poHandleProvider, ai_poExecutor ) ;
	Open( ai_poEnvironment, ai_pccSource, ai_pccClassPath, ai_pccIdentifier ) ;
}


M4JavaContext::~M4JavaContext( void )
{
	Close( M4_TRUE ) ;

	if( m_poHandleProvider != NULL )
	{
		if( m_bOwner == M4_TRUE )
		{
			delete( m_poHandleProvider ) ;
		}
		m_poHandleProvider = NULL ;
	}
}


m4return_t	M4JavaContext::Open( ClVMBaseEnv *ai_poEnvironment, m4pcchar_t ai_pccSource, m4pcchar_t ai_pccClassPath, m4pcchar_t ai_pccIdentifier )
{

	m4return_t			iResult = M4_ERROR ;
	m4bool_t			bNeedToDecompress = M4_FALSE ;
	m4uint32_t			iArguments = 0 ;
	string				sClassPath ;
	m4pcchar_t			pccClassPathFile = NULL ;
	m4pcchar_t			*ppccArguments = NULL ;
	M4JavaConfiguration	*poConfiguration = NULL ;


	if( ai_pccSource == NULL || *ai_pccSource == '\0' )
	{
		ai_pccSource = "None" ;
	}

	if( ai_pccClassPath == NULL )
	{
		ai_pccClassPath = "" ;
	}

	Close( M4_FALSE ) ;

	poConfiguration = M4JavaConfiguration::Instance( ai_poEnvironment ) ;

	if( poConfiguration == NULL )
	{
		return( M4_ERROR ) ;
	}

	m_iTimeOut = poConfiguration->GetTimeOut() ;
	m_bTrace = poConfiguration->GetTrace() ;

	if( ai_pccIdentifier != NULL )
	{
		m_pcIdentifier = M4CloneString( ai_pccIdentifier ) ;
		CHECK_CHLOG_ERRORF( m_pcIdentifier == NULL, M4_ERROR, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
	}

	sClassPath = "" ;

	if( *ai_pccClassPath != '\0' )
	{
		sClassPath.append( ai_pccClassPath ) ;
		sClassPath.append( M4_JAVA_PATH_SEPARATOR ) ;
	}
	sClassPath.append( poConfiguration->GetClassPath( pccClassPathFile ) ) ;

	iArguments = poConfiguration->GetArgumentsLength() ;
	ppccArguments = new m4pcchar_t[ iArguments + 7 ] ;
	CHECK_CHLOG_ERRORF( ppccArguments == NULL, M4_ERROR, M4JAVA_NO_MEMORY, m4uint32_t( ( iArguments + 7 ) * sizeof( m4pchar_t ) ) << __LINE__ << __FILE__ ) ;

	ppccArguments[ 0 ] = poConfiguration->GetExecutable() ;
	ppccArguments[ 1 ] = "-cp" ;
	ppccArguments[ 2 ] = sClassPath.c_str() ;
	memcpy( ppccArguments + 3, poConfiguration->GetArguments(), iArguments * sizeof( m4pchar_t ) ) ;
	ppccArguments[ 3 + iArguments ] = M4_JAVA_SERVICE ;
	ppccArguments[ 4 + iArguments ] = pccClassPathFile ;
	ppccArguments[ 5 + iArguments ] = m_bTrace == M4_TRUE ? "1" : "0" ;
	ppccArguments[ 6 + iArguments ] = ai_pccSource ;

	iResult = m_oProcess.Open( ppccArguments, iArguments + 7 ) ;
	delete( ppccArguments ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4JAVA_OPEN_PROCESS_ERROR, m_oProcess.GetError() << poConfiguration->GetExecutable() ) ;

	return( M4_SUCCESS ) ;
}


m4return_t	M4JavaContext::Close( m4bool_t ai_bDisposing )
{

	itSetHandleData	itHandleData ;


	M4FreeString( m_pcIdentifier ) ;

	m_oProcess.Close() ;

	if( m_poHandleProvider != NULL )
	{
		if( ai_bDisposing == M4_FALSE || m_bOwner == M4_FALSE )
		{
			itHandleData = m_oHandleData.begin() ;

			while( itHandleData != m_oHandleData.end() )
			{
				m_poHandleProvider->DeleteObject( *itHandleData ) ;
				itHandleData++ ;
			}
		}
	}

	m_oHandleData.clear() ;

	return( M4_SUCCESS ) ;
}


// Funciones de ejecución =====================================================

m4return_t	M4JavaContext::CreateObject( m4pcchar_t ai_pccClassName, M4JavaObject ai_avArguments[], m4uint16_t ai_iLength, m4double_t &ao_rdHandle, m4bool_t ai_bDumpError, m4pcchar_t ai_pccSource )
{

	m4return_t		iResult = M4_ERROR ;
	m4double_t		dNewHandle = 0 ;
	M4JavaObject	vHeaders[ 2 ] ;
	M4JavaObject	vResult ;


	CHECK_CHLOG_ERRORF( m_poHandleProvider == NULL, M4_ERROR, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	ao_rdHandle = 0 ;

	dNewHandle = m_poHandleProvider->GetHandle() ;

	vHeaders[ 0 ].SetString( (m4pchar_t)ai_pccClassName, M4_FALSE ) ;
	vHeaders[ 1 ].SetObject( dNewHandle ) ;

	iResult = _Invoke( 1, vHeaders, 2, ai_avArguments, ai_iLength, vResult, ai_bDumpError, ai_pccSource ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	ao_rdHandle = dNewHandle ;
	m_oHandleData.insert( dNewHandle ) ;

	iResult = m_poHandleProvider->NewObject( ai_pccClassName, this ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	M4JavaContext::DestroyObject( m4double_t ai_dHandle, m4bool_t ai_bDumpError, m4pcchar_t ai_pccSource )
{

	m4return_t		iResult = M4_ERROR ;
	M4JavaObject	vHeaders[ 1 ] ;
	M4JavaObject	vResult ;


	CHECK_CHLOG_ERRORF( m_poHandleProvider == NULL, M4_ERROR, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	m_oHandleData.erase( ai_dHandle ) ;
	m_poHandleProvider->DeleteObject( ai_dHandle ) ;

	vHeaders[ 0 ].SetObject( ai_dHandle ) ;

	iResult = _Invoke( 2, vHeaders, 1, NULL, 0, vResult, ai_bDumpError, ai_pccSource ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	M4JavaContext::NewMethod( m4pcchar_t ai_pccClassName, m4pcchar_t ai_pccMethod, M4JavaObject ai_avTypes[], m4uint16_t ai_iLength, m4uint32_t &ao_riResult, m4bool_t ai_bDumpError, m4pcchar_t ai_pccSource )
{

	m4return_t		iResult = M4_ERROR ;
	m4int32_t		iInteger = 0 ;
	M4JavaObject	vHeaders[ 2 ] ;
	M4JavaObject	vResult ;


	ao_riResult = 0 ;

	vHeaders[ 0 ].SetString( (m4pchar_t)ai_pccClassName, M4_FALSE ) ;
	vHeaders[ 1 ].SetString( (m4pchar_t)ai_pccMethod, M4_FALSE ) ;

	iResult = _Invoke( 3, vHeaders, 2, ai_avTypes, ai_iLength, vResult, ai_bDumpError, ai_pccSource ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = vResult.GetInteger( iInteger ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	ao_riResult = iInteger ;
	return( M4_SUCCESS ) ;
}


m4return_t	M4JavaContext::DeleteMethod( m4uint32_t ai_iMethod, m4bool_t ai_bDumpError, m4pcchar_t ai_pccSource )
{

	m4return_t		iResult = M4_ERROR ;
	M4JavaObject	vHeaders[ 1 ] ;
	M4JavaObject	vResult ;


	vHeaders[ 0 ].SetInteger( ai_iMethod ) ;

	iResult = _Invoke( 4, vHeaders, 1, NULL, 0, vResult, ai_bDumpError, ai_pccSource ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	M4JavaContext::InvokeMethod( m4double_t ai_dHandle, m4pcchar_t ai_pccMethod, M4JavaObject ai_avArguments[], m4uint16_t ai_iLength, M4JavaObject &ao_rvResult, m4bool_t ai_bDumpError, m4pcchar_t ai_pccSource )
{

	m4return_t		iResult = M4_ERROR ;
	m4double_t		dNewHandle = 0 ;
	M4JavaObject	vHeaders[ 3 ] ;


	CHECK_CHLOG_ERRORF( m_poHandleProvider == NULL, M4_ERROR, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	ao_rvResult.SetNull() ;

	dNewHandle = m_poHandleProvider->GetHandle() ;

	vHeaders[ 0 ].SetObject( ai_dHandle ) ;
	vHeaders[ 1 ].SetString( (m4pchar_t)ai_pccMethod, M4_FALSE ) ;
	vHeaders[ 2 ].SetObject( dNewHandle ) ;

	iResult = _Invoke( 5, vHeaders, 3, ai_avArguments, ai_iLength, ao_rvResult, ai_bDumpError, ai_pccSource ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( ao_rvResult.GetType() == M4JavaObject::Object && M4HandleCompare( ao_rvResult.GetObject(), dNewHandle ) == 0 )
	{
		m_oHandleData.insert( dNewHandle ) ;

		iResult = m_poHandleProvider->NewObject( NULL, this ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	return( M4_SUCCESS ) ;
}


m4return_t	M4JavaContext::InvokeMethod( m4double_t ai_dHandle, m4uint32_t ai_iMethod, M4JavaObject ai_avArguments[], m4uint16_t ai_iLength, M4JavaObject &ao_rvResult, m4bool_t ai_bDumpError, m4pcchar_t ai_pccSource )
{

	m4return_t		iResult = M4_ERROR ;
	m4double_t		dNewHandle = 0 ;
	M4JavaObject	vHeaders[ 3 ] ;


	CHECK_CHLOG_ERRORF( m_poHandleProvider == NULL, M4_ERROR, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	ao_rvResult.SetNull() ;

	dNewHandle = m_poHandleProvider->GetHandle() ;

	vHeaders[ 0 ].SetObject( ai_dHandle ) ;
	vHeaders[ 1 ].SetInteger( ai_iMethod ) ;
	vHeaders[ 2 ].SetObject( dNewHandle ) ;

	iResult = _Invoke( 6, vHeaders, 3, ai_avArguments, ai_iLength, ao_rvResult, ai_bDumpError, ai_pccSource ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( ao_rvResult.GetType() == M4JavaObject::Object && M4HandleCompare( ao_rvResult.GetObject(), dNewHandle ) == 0 )
	{
		m_oHandleData.insert( dNewHandle ) ;

		iResult = m_poHandleProvider->NewObject( NULL, this ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	return( M4_SUCCESS ) ;
}


m4return_t	M4JavaContext::InvokeMethod( m4pcchar_t ai_pccClassName, m4pcchar_t ai_pccMethod, M4JavaObject ai_avArguments[], m4uint16_t ai_iLength, M4JavaObject &ao_rvResult, m4bool_t ai_bDumpError, m4pcchar_t ai_pccSource )
{

	m4return_t		iResult = M4_ERROR ;
	m4double_t		dNewHandle = 0 ;
	M4JavaObject	vHeaders[ 3 ] ;


	CHECK_CHLOG_ERRORF( m_poHandleProvider == NULL, M4_ERROR, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	ao_rvResult.SetNull() ;

	dNewHandle = m_poHandleProvider->GetHandle() ;

	vHeaders[ 0 ].SetString( (m4pchar_t)ai_pccClassName, M4_FALSE ) ;
	vHeaders[ 1 ].SetString( (m4pchar_t)ai_pccMethod, M4_FALSE ) ;
	vHeaders[ 2 ].SetObject( dNewHandle ) ;

	iResult = _Invoke( 7, vHeaders, 3, ai_avArguments, ai_iLength, ao_rvResult, ai_bDumpError, ai_pccSource ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( ao_rvResult.GetType() == M4JavaObject::Object && M4HandleCompare( ao_rvResult.GetObject(), dNewHandle ) == 0 )
	{
		m_oHandleData.insert( dNewHandle ) ;

		iResult = m_poHandleProvider->NewObject( NULL, this ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	return( M4_SUCCESS ) ;
}


m4return_t	M4JavaContext::InvokeMethod( m4uint32_t ai_iMethod, M4JavaObject ai_avArguments[], m4uint16_t ai_iLength, M4JavaObject &ao_rvResult, m4bool_t ai_bDumpError, m4pcchar_t ai_pccSource )
{

	m4return_t		iResult = M4_ERROR ;
	m4double_t		dNewHandle = 0 ;
	M4JavaObject	vHeaders[ 2 ] ;


	CHECK_CHLOG_ERRORF( m_poHandleProvider == NULL, M4_ERROR, M4JAVA_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	ao_rvResult.SetNull() ;

	dNewHandle = m_poHandleProvider->GetHandle() ;

	vHeaders[ 0 ].SetInteger( ai_iMethod ) ;
	vHeaders[ 1 ].SetObject( dNewHandle ) ;

	iResult = _Invoke( 8, vHeaders, 2, ai_avArguments, ai_iLength, ao_rvResult, ai_bDumpError, ai_pccSource ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( ao_rvResult.GetType() == M4JavaObject::Object && M4HandleCompare( ao_rvResult.GetObject(), dNewHandle ) == 0 )
	{
		m_oHandleData.insert( dNewHandle ) ;

		iResult = m_poHandleProvider->NewObject( NULL, this ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	return( M4_SUCCESS ) ;
}



// Funciones internas =========================================================

m4return_t	M4JavaContext::_Init( M4JavaHandleProvider *ai_poHandleProvider, IExecutor *ai_poExecutor )
{

	static m4uint16_t	s_iIdentifier = 99 ;
	static ClMutex		s_oMutex( M4_TRUE ) ;


	m_iTimeOut = 0 ;
	m_bTrace = M4_FALSE ;
	m_bFirstTime = M4_TRUE ;
	m_bIsDisposed = M4_FALSE ;
	m_iReferences = 0 ;

	m_pcIdentifier = NULL ;
	m_poExecutor = ai_poExecutor ;

	if( ai_poHandleProvider != NULL )
	{
		m_bOwner = M4_FALSE ;
		m_poHandleProvider = ai_poHandleProvider ;
	}
	else
	{
		m_bOwner = M4_TRUE ;

		s_oMutex.Lock() ;

		if( s_iIdentifier == 0 )
		{
			s_iIdentifier = 99 ;
		}

		m4int16_t	iIdentifier = s_iIdentifier-- ;

		s_oMutex.Unlock() ;

		m_poHandleProvider = new M4JavaHandleProvider( 700 + iIdentifier ) ;
		CHECK_CHLOG_ERRORF( m_poHandleProvider == NULL, M4_ERROR, M4JAVA_NO_MEMORY, m4uint32_t( sizeof( M4JavaHandleProvider ) ) << __LINE__ << __FILE__ ) ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	M4JavaContext::_Serialize( m4uint8_t ai_iOperation, M4JavaObject ai_apvHeaders[], m4uint16_t ai_iHeaders, M4JavaObject ai_avArguments[], m4uint16_t ai_iArguments, m4pcchar_t ai_pccSource )
{

	m4return_t	iResult = M4_ERROR ;
	m4uint16_t	i = 0 ;
	m4uint16_t	iLength = 0 ;


	iResult = m_oProcess.Write( &m_bTrace, sizeof( m_bTrace ) ) ;

	if( m_bFirstTime == M4_TRUE )
	{
		m_bFirstTime = M4_FALSE ;
		CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4JAVA_SEND_DATA_FIRST_TIME_ERROR, m_oProcess.GetError() ) ;
	}
	else
	{
		CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4JAVA_SEND_DATA_ERROR, m_oProcess.GetError() ) ;
	}

	if( m_bTrace == M4_TRUE )
	{
		if( ai_pccSource == NULL || *ai_pccSource == '\0' )
		{
			ai_pccSource = "None" ;
		}

		iResult = m_oProcess.WriteString( ai_pccSource ) ;
		CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4JAVA_SEND_DATA_ERROR, m_oProcess.GetError() ) ;
	}

	iResult = m_oProcess.Write( &ai_iOperation, sizeof( ai_iOperation ) ) ;

	iLength = ai_iHeaders + ai_iArguments ;
	iResult = m_oProcess.Write( &iLength, sizeof( iLength ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4JAVA_SEND_DATA_ERROR, m_oProcess.GetError() ) ;

	for( i = 0 ; i < ai_iHeaders ; i++ )
	{
		iResult = ai_apvHeaders[ i ].Write( m_oProcess ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	for( i = 0 ; i < ai_iArguments ; i++ )
	{
		iResult = ai_avArguments[ i ].Write( m_oProcess ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	return( M4_SUCCESS ) ;
}


m4return_t	M4JavaContext::_DeSerialize( M4JavaObject &ao_rvResult, m4bool_t ai_bDumpError )
{

	m4return_t		iResult = M4_ERROR ;
	m4return_t		iReturn = M4_ERROR ;
	m4return_t		iCanceled = 0 ;
	m4uint16_t		i = 0 ;
	m4uint16_t		iLength = 0 ;
	m4uint32_t		iError = 0 ;
	m4char_t		acDate[ M4_SECOND_END + 1 ] ;
	m4VariantType	vValue ;
	M4JavaObject	vObject ;


	ao_rvResult.SetNull() ;

	iResult = m_oProcess.ReadByte( iReturn, m_iTimeOut, m_poExecutor, iCanceled ) ;

	if( iResult != M4_SUCCESS )
	{
		DUMP_CHLOG_ERRORF( M4JAVA_READ_DATA_ERROR, m_oProcess.GetError() ) ;
	}

	CHECK_CHLOG_ERROR ( iCanceled == -1, M4_ERROR, M4JAVA_CANCELED ) ;
	CHECK_CHLOG_ERRORF( iCanceled == -2, M4_ERROR, M4JAVA_COMPLEX_EXECUTION, m_iTimeOut ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( iReturn == M4_SUCCESS )
	{
		iResult = ao_rvResult.Read( m_oProcess, M4_TRUE ) ;
		CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4JAVA_READ_DATA_ERROR, m_oProcess.GetError() ) ;
	}
	else
	{
		iResult = m_oProcess.Read( &iError, sizeof( iError ) ) ;
		CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4JAVA_READ_DATA_ERROR, m_oProcess.GetError() ) ;

		if( ai_bDumpError == M4_TRUE )
		{
			g_oChLog << BeginError( iError ) ;
		}

		iResult = m_oProcess.Read( &iLength, sizeof( iLength ) ) ;

		if( iResult != M4_SUCCESS )
		{
			if( ai_bDumpError == M4_TRUE )
			{
				g_oChLog << EndLog ;
			}
			DUMP_CHLOG_ERRORF( M4JAVA_READ_DATA_ERROR, m_oProcess.GetError() ) ;
			return( M4_ERROR ) ;
		}

		for( i = 0 ; i < iLength ; i++ )
		{
			iResult = vObject.Read( m_oProcess, M4_FALSE ) ;

			if( iResult != M4_SUCCESS )
			{
				if( ai_bDumpError == M4_TRUE )
				{
					g_oChLog << EndLog ;
				}
				DUMP_CHLOG_ERRORF( M4JAVA_READ_DATA_ERROR, m_oProcess.GetError() ) ;
				return( M4_ERROR ) ;
			}

			if( ai_bDumpError == M4_TRUE )
			{
				vObject.GetVariant( vValue, M4_FALSE, M4_FALSE ) ;

				switch( vValue.Type )
				{
					case M4CL_CPP_TYPE_NUMBER:
						g_oChLog << vValue.Data.DoubleData ;
						break ;

					case M4CL_CPP_TYPE_STRING_VAR:
						g_oChLog << vValue.Data.PointerChar ;
						break ;

					case M4CL_CPP_TYPE_DATE:
						ClDateToString( vValue.Data.DoubleData, acDate, M4_SECOND_END + 1 ) ;
						g_oChLog << acDate ;
						break ;
					default:
						break ;
				}
			}
		}

		if( ai_bDumpError == M4_TRUE )
		{
			g_oChLog << EndLog ;
		}
	}

	return( iReturn ) ;
}


m4return_t	M4JavaContext::_Invoke( m4uint8_t ai_iOperation, M4JavaObject ai_apvHeaders[], m4uint16_t ai_iHeaders, M4JavaObject ai_avArguments[], m4uint16_t ai_iArguments, M4JavaObject &ao_rvResult, m4bool_t ai_bDumpError, m4pcchar_t ai_pccSource )
{

	m4return_t	iResult = M4_ERROR ;


	ao_rvResult.SetNull() ;

	iResult = _Serialize( ai_iOperation, ai_apvHeaders, ai_iHeaders, ai_avArguments, ai_iArguments, ai_pccSource ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = _DeSerialize( ao_rvResult, ai_bDumpError ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}




