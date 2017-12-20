
//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdtransfer
// File:                m4mdtservice.cpp
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                07-04-2004
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     
//
//
// Definition:
//
//    Este módulo contiene las funciones de la clase del servicio de la
//    librería m4mdtransfer
//
//==============================================================================


#include "m4mdtservice.hpp"

#include "m4mdtdef.hpp"
#include "m4mdtres.hpp"
#include "m4mdtprestool.hpp"
#include "m4mdtodbc.hpp"
#include "m4mdtdictionary.hpp"
#include "m4transformer.hpp"
#include "cldates.hpp"
#include "m4unicode.hpp"




//=================================================================================
// Constantes de nombres de los atributos que se buscan
//=================================================================================

// UNICODE XML
XMLCh*		IMDTService::sm_sXMLChContentType		= L"content-type" ;
XMLCh*		IMDTService::sm_sXMLChDB				= L"db" ;
XMLCh*		IMDTService::sm_sXMLChInsert			= L"insert" ;
XMLCh*		IMDTService::sm_sXMLChIsNull			= L"is-null" ;
XMLCh*		IMDTService::sm_sXMLChIsPrintable		= L"is-printable" ;
XMLCh*		IMDTService::sm_sXMLChMultiline			= L"multiline" ;
XMLCh*		IMDTService::sm_sXMLChName				= L"name" ;
XMLCh*		IMDTService::sm_sXMLChSelect			= L"select" ;
XMLCh*		IMDTService::sm_sXMLChOperation			= L"op" ;
XMLCh*		IMDTService::sm_sXMLChUpdateFunctions	= L"update-functions" ;
XMLCh*		IMDTService::sm_sXMLChValue				= L"value" ;
XMLCh*		IMDTService::sm_sXMLChVisible			= L"visible" ;
XMLCh*		IMDTService::sm_sXMLChAlias				= L"alias" ;
XMLCh*		IMDTService::sm_sXMLChCaption			= L"caption" ;
XMLCh*		IMDTService::sm_sXMLChDelete			= L"delete" ;
XMLCh*		IMDTService::sm_sXMLChDeleteNumber		= L"delete-number" ;
XMLCh*		IMDTService::sm_sXMLChFieldPosition		= L"field-position" ;
XMLCh*		IMDTService::sm_sXMLChHasWhere			= L"has-where" ;
XMLCh*		IMDTService::sm_sXMLChInsertNumber		= L"insert-number" ;
XMLCh*		IMDTService::sm_sXMLChInternalField		= L"internal-field" ;
XMLCh*		IMDTService::sm_sXMLChLogicField		= L"logic-field" ;
XMLCh*		IMDTService::sm_sXMLChLogicFields		= L"logic-fields" ;
XMLCh*		IMDTService::sm_sXMLChLogicPosition		= L"logic-position" ;
XMLCh*		IMDTService::sm_sXMLChLogicObject		= L"logic-object" ;
XMLCh*		IMDTService::sm_sXMLChLookup			= L"lookup" ;
XMLCh*		IMDTService::sm_sXMLChLookups			= L"lookups" ;
XMLCh*		IMDTService::sm_sXMLChLookupRegister	= L"lookup-register" ;
XMLCh*		IMDTService::sm_sXMLChM4Type			= L"m4type" ;
XMLCh*		IMDTService::sm_sXMLChPkPosition		= L"pk-position" ;
XMLCh*		IMDTService::sm_sXMLChPrecision			= L"precision" ;
XMLCh*		IMDTService::sm_sXMLChRealField			= L"real-field" ;
XMLCh*		IMDTService::sm_sXMLChRealObject		= L"real-object" ;
XMLCh*		IMDTService::sm_sXMLChRealObjects		= L"real-objects" ;
XMLCh*		IMDTService::sm_sXMLChScale				= L"scale" ;
XMLCh*		IMDTService::sm_sXMLChTablePosition		= L"table-position" ;
XMLCh*		IMDTService::sm_sXMLChTable				= L"table" ;
XMLCh*		IMDTService::sm_sXMLChRegister			= L"register" ;
XMLCh*		IMDTService::sm_sXMLChField				= L"field" ;




//=================================================================================
// Funciones de inicialización
//=================================================================================


//=================================================================================
//
// Constructor
//
// Pasos
//		Inicializa lod miembros a NULL
//		El gestor de parámetros hay que inicializarlo porque se necesita antes de
//			la inicialización de los objetos
//		Inicializa las utilidades de XML
//		Inicializa las constantes, que deben ir después de la inicalización de
//			las utilidades XML
//
//=================================================================================

ClMDTService::ClMDTService( void )
{

	m4return_t	iResult = M4_SUCCESS ;


	m_bInitialised = M4_FALSE ;

	m_poPresentationTool = NULL ;
	m_poDBManager = NULL ;
	m_poDictionary = NULL ;
	m_poTransformer = NULL ;

	iResult = m_oParameterManager.Init() ;
	M4MDT_CHECK_ERROR2( iResult != M4_SUCCESS, M4MDTERROR_INTERNAL_ERROR, "ClMDTService::ClMDTService", "m_oParameterManager.Init", "%s#%s" ) ;

	iResult = InitXML() ;
	M4MDT_CHECK_ERROR2( iResult != M4_SUCCESS, M4MDTERROR_INTERNAL_ERROR, "ClMDTService::ClMDTService", "InitXML", "%s#%s" ) ;
}



//=================================================================================
//
// Destructor
//
// Libera los objetos de la librería
//
//=================================================================================

ClMDTService::~ClMDTService( void )
{
	if( m_poTransformer != NULL )
	{
		delete( m_poTransformer ) ;
		m_poTransformer = NULL ;
	}

	if( m_poDictionary != NULL )
	{
		delete( m_poDictionary ) ;
		m_poDictionary = NULL ;
	}

	if( m_poDBManager != NULL )
	{
		delete( m_poDBManager ) ;
		m_poDBManager = NULL ;
	}

	if( m_poPresentationTool != NULL )
	{
		delete( m_poPresentationTool ) ;
		m_poPresentationTool = NULL ;
	}

	EndXML() ;
}



//=================================================================================
// Inicializa el core XML
// Devuelve un error en caso de excepcion.
//
// Pasos
//		Se inicializa el core XML
//
//=================================================================================

m4return_t ClMDTService::InitXML( void )
{

	m4return_t iRes = M4_SUCCESS;

    try
    {
      	XMLPlatformUtils::Initialize() ;
    }

	catch( const XMLException& e )
	{
		// UNICODE XML
		m4pchar_t pcException = M4XMLToCpp( (M4XMLCh*)e.getMessage() ) ;
		M4MDT_DUMP_ERROR1( M4MDTERROR_EXCEPTION, pcException, "%s" ) ;
		delete [] pcException ;
		iRes = M4_ERROR;
    }

	return iRes;
}


//=================================================================================
// Termina el core XML
// Devuelve un error en caso de excepcion.
//
// Pasos
//		Termina el core XML
//
//=================================================================================

m4return_t ClMDTService::EndXML( void )
{

	m4return_t iRes = M4_SUCCESS;

    try
    {
      	XMLPlatformUtils::Terminate() ;
    }

	catch( const XMLException& e )
	{
		// UNICODE XML
		m4pchar_t pcException = M4XMLToCpp( (M4XMLCh*)e.getMessage() ) ;
		M4MDT_DUMP_ERROR1( M4MDTERROR_EXCEPTION, pcException, "%s" ) ;
		delete [] pcException ;
		iRes = M4_ERROR;
    }

	return iRes;
}


//=================================================================================
// Inicializa los objetos de la librería
// Cuando algo falla se liberan todos los objetos anteriormente creados
//
// Pasos
//		Se comprueba que los parámetros de entrada no sean nulos
//		Se inicializan los miebros
//		Se inicializa y añaden los dispositivos de errores. De momento log y fichero
//		Se crea el manejador de base de datos
//		Se inicializa el manejador de base de datos
//		Se crea la tool de presentaciones
//		Se crea el diccionario
//		Se inicializa el diccionario
//		Se crea el transformador
//		Se inicializa el transformador
//
//=================================================================================

m4return_t	ClMDTService::Init( m4uint32_t ai_iLanguage, m4int32_t ai_iConnections, m4pcchar_t ai_pccDictConnection, eDDBBType_t ai_eDictDBType, m4pcchar_t ai_pccWorkingDir, m4pcchar_t ai_pccDebugDir, m4pcchar_t ai_pccCacheDir, m4uint32_t ai_iVersion, m4uint32_t ai_iTraceLevel )
{

	m4return_t	iResult = M4_ERROR ;
	int			iType = 0 ;
	m4uint32_t	i = 0 ;
	m4uint32_t	iLength = 0 ;
	m4int32_t	iSecond = -1 ;
	m4pcchar_t	pccName = NULL ;
	m4pcchar_t	pccType = NULL ;
	m4pcchar_t	pccDSN = NULL ;
    m4char_t	acDate[ M4_SECOND_END + 1 ] ;


	M4MDT_RETURN_ERROR2( ai_pccDictConnection == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pccDictConnection", "ClMDTService::Init", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ai_pccWorkingDir == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pccWorkingDir", "ClMDTService::Init", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ai_pccDebugDir == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pccDebugDir", "ClMDTService::Init", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ai_pccCacheDir == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pccCacheDir", "ClMDTService::Init", "%s#%s" ) ;

	M4MDT_RETURN_ERROR0( m_bInitialised == M4_TRUE, M4_ERROR, M4MDTERROR_LIBRARY_ALREADY_INITAILISED ) ;


	ClActualDate( acDate, M4_SECOND_END + 1 ) ;
	m_sSessionDate = acDate ;

	m_sDebugDir = ai_pccDebugDir ;


	iResult = m_oFileDevice.Init( ai_pccDebugDir ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTService::Init", "m_oFileDevice.Init", ai_pccDebugDir, "%s#%s - ai_pccDebugDir = <%s>" ) ;

	iResult = M4MDTErrorManager::AddErrorDevice( &m_oFileDevice ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTService::Init", "M4MDTErrorManager::AddErrorDevice", "%s#%s" ) ;

	/* De momento no se sacan los mensajes a la log
	iResult = m_oLogDevice.Init( ai_pccWorkingDir ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTService::Init", "m_oLogDevice.Init", ai_pccWorkingDir, "%s#%s - ai_pccWorkingDir = <%s>" ) ;

	iResult = M4MDTErrorManager::AddErrorDevice( &m_oLogDevice ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTService::Init", "M4MDTErrorManager::AddErrorDevice", "%s#%s" ) ;
	*/


	iResult = m_oParameterManager.GetParameterSize( ai_iConnections, iLength ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTService::Init", "m_oParameterManager.GetParameterSize", "%s#%s" ) ;

	m_poDBManager = new M4ODBC() ;
	M4MDT_RETURN_ERROR1( m_poDBManager == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof( M4ODBC ), "%d" ) ;

	iResult = m_poDBManager->Init( ai_iTraceLevel, ai_pccDebugDir ) ;

	if( iResult != M4_SUCCESS )
	{
		M4MDT_DUMP_ERROR2( M4MDTERROR_INTERNAL_ERROR, "ClMDTService::Init", "m_poDBManager->Init", "%s#%s" ) ;
		delete( m_poDBManager ) ;
		m_poDBManager = NULL ;
		return( M4_ERROR ) ;
	}

	for( i = 0 ; i < iLength ; i++ )
	{
		iResult = m_oParameterManager.GetParameterValue( ai_iConnections, i, pccName, iSecond ) ;

		if( iResult != M4_SUCCESS )
		{
			M4MDT_DUMP_ERROR3( M4MDTERROR_INTERNAL_ERROR, "ClMDTService::Init", "m_oParameterManager.GetParameterValue", i, "%s#%s - Parameter: <%d>" ) ;
			delete( m_poDBManager ) ;
			m_poDBManager = NULL ;
			return( M4_ERROR ) ;
		}

		// Se asume que sólo viene una pareja
		iResult = m_oParameterManager.GetParameterValue( iSecond, 0, pccDSN, pccType ) ;

		if( iResult != M4_SUCCESS )
		{
			M4MDT_DUMP_ERROR3( M4MDTERROR_INTERNAL_ERROR, "ClMDTService::Init", "m_oParameterManager.GetParameterValue", 0, "%s#%s - Parameter: <%d>" ) ;
			delete( m_poDBManager ) ;
			m_poDBManager = NULL ;
			return( M4_ERROR ) ;
		}

		iType = atoi( pccType ) ;

		iResult = m_poDBManager->AddConnection( pccName, pccDSN, eDDBBType_t( iType ) ) ;

		if( iResult != M4_SUCCESS )
		{
			M4MDT_DUMP_ERROR4( M4MDTERROR_INTERNAL_ERROR, "ClMDTService::Init", "m_poDBManager->AddConnection", i, pccName, "%s#%s - Parameter: <%d> - Name: <%s>" ) ;
			delete( m_poDBManager ) ;
			m_poDBManager = NULL ;
			return( M4_ERROR ) ;
		}
	}

	iResult = m_poDBManager->AddConnection( M4MDT_DICTIONARY_CONNECTION, ai_pccDictConnection, ai_eDictDBType ) ;

	if( iResult != M4_SUCCESS )
	{
		M4MDT_DUMP_ERROR3( M4MDTERROR_INTERNAL_ERROR, "ClMDTService::Init", "m_poDBManager->AddConnection", M4MDT_DICTIONARY_CONNECTION, "%s#%s - Name: <%s>" ) ;
		delete( m_poDBManager ) ;
		m_poDBManager = NULL ;
		return( M4_ERROR ) ;
	}


	m_poPresentationTool = new ClMDTPresentationTool() ;
	
	if( m_poPresentationTool == NULL )
	{
		M4MDT_DUMP_ERROR1( M4MDTERROR_NO_MEMORY, sizeof( ClMDTPresentationTool ), "%d" ) ;
		delete( m_poDBManager ) ;
		m_poDBManager = NULL ;
		return( M4_ERROR ) ;
	}


	m_poDictionary = new ClMDTDictionary() ;

	if( m_poDictionary == NULL )
	{
		M4MDT_DUMP_ERROR1( M4MDTERROR_NO_MEMORY, sizeof( M4ODBC ), "%d" ) ;
		delete( m_poPresentationTool ) ;
		delete( m_poDBManager ) ;
		m_poPresentationTool = NULL ;
		m_poDBManager = NULL ;
		return( M4_ERROR ) ;
	}

	iResult = m_poDictionary->Init( ai_iLanguage, M4MDT_DICTIONARY_CONNECTION, ai_pccCacheDir, ai_iVersion, m_poDBManager ) ;

	if( iResult != M4_SUCCESS )
	{
		M4MDT_DUMP_ERROR2( M4MDTERROR_INTERNAL_ERROR, "ClMDTService::Init", "m_poDictionary->Init", "%s#%s" ) ;
		delete( m_poPresentationTool ) ;
		delete( m_poDBManager ) ;
		delete( m_poDictionary ) ;
		m_poPresentationTool = NULL ;
		m_poDBManager = NULL ;
		m_poDictionary = NULL ;
		return( M4_ERROR ) ;
	}


	m_poTransformer = new ClMDTTransformer() ;

	if( m_poTransformer == NULL )
	{
		M4MDT_DUMP_ERROR1( M4MDTERROR_NO_MEMORY, sizeof( M4ODBC ), "%d" ) ;
		delete( m_poPresentationTool ) ;
		delete( m_poDBManager ) ;
		delete( m_poDictionary ) ;
		m_poPresentationTool = NULL ;
		m_poDBManager = NULL ;
		m_poDictionary = NULL ;
		return( M4_ERROR ) ;
	}

	iResult = m_poTransformer->Init( this ) ;

	if( iResult != M4_SUCCESS )
	{
		M4MDT_DUMP_ERROR2( M4MDTERROR_INTERNAL_ERROR, "ClMDTService::Init", "m_poTransformer->Init", "%s#%s" ) ;
		delete( m_poPresentationTool ) ;
		delete( m_poDBManager ) ;
		delete( m_poDictionary ) ;
		delete( m_poTransformer ) ;
		m_poPresentationTool = NULL ;
		m_poDBManager = NULL ;
		m_poDictionary = NULL ;
		m_poTransformer = NULL ;
		return( M4_ERROR ) ;
	}


	m_bInitialised = M4_TRUE ;

	return( M4_SUCCESS ) ;
}



//=================================================================================
// Finaliza los objetos de la librería
// Quita los dispositivos de errores de la lista
//=================================================================================

m4return_t	ClMDTService::End( void )
{

	m4return_t	iResult = M4_ERROR ;


	if( m_poTransformer != NULL )
	{
		delete( m_poTransformer ) ;
		m_poTransformer = NULL ;
	}

	if( m_poDictionary != NULL )
	{
		delete( m_poDictionary ) ;
		m_poDictionary = NULL ;
	}

	if( m_poDBManager != NULL )
	{
		delete( m_poDBManager ) ;
		m_poDBManager = NULL ;
	}

	if( m_poPresentationTool != NULL )
	{
		delete( m_poPresentationTool ) ;
		m_poPresentationTool = NULL ;
	}

	/* De momento no se sacan los mensajes a la log
	iResult = M4MDTErrorManager::RemoveErrorDevice( &m_oLogDevice ) ;
	M4MDT_CHECK_ERROR2( iResult != M4_SUCCESS, M4MDTERROR_INTERNAL_ERROR, "ClMDTService::End", "M4MDTErrorManager::RemoveErrorDevice", "%s#%s" ) ;
	*/

	iResult = M4MDTErrorManager::RemoveErrorDevice( &m_oFileDevice ) ;
	M4MDT_CHECK_ERROR2( iResult != M4_SUCCESS, M4MDTERROR_INTERNAL_ERROR, "ClMDTService::End", "M4MDTErrorManager::RemoveErrorDevice", "%s#%s" ) ;


	m_sSessionDate = "" ;


	m_bInitialised = M4_FALSE ;

	return( M4_SUCCESS ) ;
}



//=================================================================================
// Devuelve si la librería ha sido inicializada o no
//=================================================================================

m4bool_t	ClMDTService::IsInitialised( void ) const
{
	return( m_bInitialised ) ;
}



//=================================================================================
// Obtiene la fecha de la sesión
// Aunque no esté inicializado no se da error
//=================================================================================

const string&	ClMDTService::GetSessionDate( void ) const
{
	return( m_sSessionDate ) ;
}



//=================================================================================
// Obtiene el directorio temporal
// Aunque no esté inicializado no se da error
//=================================================================================

const string&	ClMDTService::GetDebugDir( void ) const
{
	return( m_sDebugDir ) ;
}



//=================================================================================
// Obtiene el manejador de parámetros de lectura
// Aunque no esté inicializado no se da error
//=================================================================================

const IMDTReadParameterManager*	ClMDTService::GetIReadParameterManager( void ) const
{
	return(  &m_oParameterManager ) ;
}



//=================================================================================
// Obtiene el manejador de parámetros de escritura
// Aunque no esté inicializado no se da error
//=================================================================================

IMDTWriteParameterManager*	ClMDTService::GetIWriteParameterManager( void )
{
	return( &m_oParameterManager ) ;
}



//=================================================================================
// Obtiene el manejador de parámetros de lectura escritura escritura
// Aunque no esté inicializado no se da error
//=================================================================================

IMDTReadWriteParameterManager*	ClMDTService::GetIReadWriteParameterManager( void )
{
	return( &m_oParameterManager ) ;
}



//=================================================================================
// Obtiene el manejador de base de datos
//=================================================================================

M4IODBC*	ClMDTService::GetIODBC( void )
{
	M4MDT_RETURN_ERROR1( m_bInitialised == M4_FALSE, NULL, M4MDTERROR_LIBRARY_NOT_INITAILISED, "ClMDTService::GetIODBC", "%s" ) ;
	return( m_poDBManager ) ;
}



//=================================================================================
// Obtiene la tool de presentaciones
//=================================================================================

IMDTPresentationTool*	ClMDTService::GetIPresentationTool( void )
{
	M4MDT_RETURN_ERROR1( m_bInitialised == M4_FALSE, NULL, M4MDTERROR_LIBRARY_NOT_INITAILISED, "ClMDTService::GetIPresentationTool", "%s" ) ;
	return( m_poPresentationTool ) ;
}



//=================================================================================
// Obtiene el diccionario
//=================================================================================

IMDTDictionary*	ClMDTService::GetIDictionary( void )
{
	M4MDT_RETURN_ERROR1( m_bInitialised == M4_FALSE, NULL, M4MDTERROR_LIBRARY_NOT_INITAILISED, "ClMDTService::GetIDictionary", "%s" ) ;
	return( m_poDictionary ) ;
}



//=================================================================================
// Obtiene el transformador
//=================================================================================

IMDTTransformer*	ClMDTService::GetITransformer( void )
{
	M4MDT_RETURN_ERROR1( m_bInitialised == M4_FALSE, NULL, M4MDTERROR_LIBRARY_NOT_INITAILISED, "ClMDTService::GetITransformer", "%s" ) ;
	return( m_poTransformer ) ;
}

