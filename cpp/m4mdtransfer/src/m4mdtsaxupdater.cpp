
//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdtransfer
// File:                m4mdtsaxupdater.cpp
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                29-04-2004
// Language:            C++
// Operating System:    WINDOWS, UNIX
// Design Document:     
//
//
// Definition:
//
//    Este módulo contiene las funciones de la clase del transformador
//    sax para actualizar la base de datos con el xml enriquecido
//
//
//==============================================================================


#include "m4mdtsaxupdater.hpp"

#include "m4mdtdef.hpp"
#include "m4mdtres.hpp"
#include "cldefine.hpp"
#include "m4mdtutils.hpp"
#include "m4mdtodbc.hpp"
#include "m4imdtdictionary.hpp"
#include "m4imdtprestool.hpp"
#include "defmain.hpp"
#include "m4unicode.hpp"

#include "xercesc/parsers/XercesDOMParser.hpp"
#include "xercesc/dom/DOMElement.hpp"


#include <sys/types.h>
#include <sys/stat.h>




//=================================================================================
// Constantes
//=================================================================================

// Nombres de los argumentos de la función OBL2Presentation en el orden correcto 
const m4char_t	s_accOBL2PresArguments[ M4MDT_OBL2PRES_ARG_NUMBER ][ M4MDT_MAX_UPDATE_FUNCTION_ARGUMENT_SIZE ] = 
{
	"presentation",
	"obl",
	"package",
	"bra",
	"eng",
	"esp",
	"fra",
	"gen",
	"ger",
	"ita"
} ;

// Nombres de los argumentos de la función today en el orden correcto 
const m4char_t	s_accTodayArguments[ M4MDT_TODAY_ARG_NUMBER ][ M4MDT_MAX_UPDATE_FUNCTION_ARGUMENT_SIZE ] = 
{
	"date"
} ;

// Nombres de los argumentos de la función file2BLOB en el orden correcto 
const m4char_t	s_accFile2BLOBArguments[ M4MDT_FILE2BLOB_ARG_NUMBER ][ M4MDT_MAX_UPDATE_FUNCTION_ARGUMENT_SIZE ] = 
{
	"description",
	"extension",
	"file",
	"filename",
	"blob"
} ;

// Nombres de los argumentos de la función external2BLOB en el orden correcto 
const m4char_t	s_accExternal2BLOBArguments[ M4MDT_EXTERNAL2BLOB_ARG_NUMBER ][ M4MDT_MAX_UPDATE_FUNCTION_ARGUMENT_SIZE ] = 
{
	"description",
	"extension",
	"file",
	"blob"
} ;

// Nombres de los argumentos de la función value en el orden correcto 
const m4char_t	s_accValueArguments[ M4MDT_VALUE_ARG_NUMBER ][ M4MDT_MAX_UPDATE_FUNCTION_ARGUMENT_SIZE ] = 
{
	"value",
	"field"
} ;

// Nombres de los argumentos de la función normalizeReturns en el orden correcto 
const m4char_t	s_accNormalizeReturnsArguments[ M4MDT_NORMALIZERETURNS_ARG_NUMBER ][ M4MDT_MAX_UPDATE_FUNCTION_ARGUMENT_SIZE ] = 
{
	"field",
	"output"
} ;

// Nombres de los argumentos de la función External2PresentationPackage en el orden correcto 
const m4char_t	s_accExternal2PresentationPackageArguments[ M4MDT_EXTERNAL2PRESENTATIONPACK_ARG_NUMBER ][ M4MDT_MAX_UPDATE_FUNCTION_ARGUMENT_SIZE ] = 
{
	"field",
	"file",
	"package"
} ;

// Nombres de los argumentos de la función File2Field en el orden correcto 
const m4char_t	s_accFile2FieldArguments[ M4MDT_FILE2FIELD_ARG_NUMBER ][ M4MDT_MAX_UPDATE_FUNCTION_ARGUMENT_SIZE ] = 
{
	"file",
	"field"
} ;


//=================================================================================
// Funciones estáticas
//=================================================================================

//=================================================================================
//
// Obtine el texto asociado a un estado
//
// @param ai_eState
//
//		Enumerado que representa el estado
//
// @return
//
//		Texto asociado al estado
//
//=================================================================================

static	m4pcchar_t	M4MDTGetStatusText( SaxUpdaterState_t ai_eState )
{

	m4pcchar_t	pccResult = NULL ;


	switch( ai_eState )
	{
		case m4mdt_update_none :
			pccResult = "none" ;
			break ;

		case m4mdt_update_object :
			pccResult = "<object-data>" ;
			break ;

		case m4mdt_update_table :
			pccResult = "<table>" ;
			break ;

		case m4mdt_update_register :
			pccResult = "<register>" ;
			break ;

		case m4mdt_update_field :
			pccResult = "<field>" ;
			break ;

		case m4mdt_update_characters :
			pccResult = "characters" ;
			break ;

		default :
			pccResult = "undefined" ;
			break ;
	}

	return( pccResult ) ;
}



//=================================================================================
//
// Busca una nombre en un array de cadenas de nombres de parámetros de funciones
// de transformación
//
// @param ai_pccName
//
//		Nombre que hay que buscar
//
// @param ai_acNameArray
//
//		Array de nombres en los que buscar
//
// @param ai_iArrayLength
//
//		Tamaño del array
//
// @return
//
//		Posición de nombre en el array o -1 si no está
//
// Pasos
//		Itera por al array y si encuentra el elemento devuelve la posición
//		Sino devuelve -1
//
//=================================================================================

static	m4int16_t	M4MDTFindParameterByName( m4pcchar_t ai_pccName, const m4char_t ai_acNameArray[][ M4MDT_MAX_UPDATE_FUNCTION_ARGUMENT_SIZE ], m4uint16_t ai_iArrayLength )
{

	m4int16_t	iResult = -1 ;
	m4uint16_t	i = 0 ;
	m4bool_t	bFound = M4_FALSE ;


	iResult = -1 ;
	bFound = M4_FALSE ;

	for( i = 0 ; i < ai_iArrayLength && bFound == M4_FALSE ; i++ )
	{
		if( strcmpi( ai_pccName, ai_acNameArray[ i ] ) == 0 )
		{
			bFound = M4_TRUE ;
			iResult = i ;
		}
	}

	return( iResult ) ;
}







//=================================================================================
//
// ClMDTSaxUpdater
//
//=================================================================================


//=================================================================================
// Funciones de inicialización
//=================================================================================


//=================================================================================
//
// Constructor
//
// Inicializa los miembros a los valores por defecto
//
//=================================================================================

ClMDTSaxUpdater::ClMDTSaxUpdater( void )
{
	m_wsCDATAPrefix = NULL ;

	m_eState = m4mdt_update_none ;

	m_bIsMultiline = M4_FALSE ;
	m_vValue.SetNull() ;

	m_vUpdateFunctions.reserve( 10 ) ;

	m_vUpdateValues.reserve( 10 ) ;

	m_poRows = NULL ;

	m_pIODBC = NULL ;
	m_pIDictionary = NULL ;
	m_pIPresentationTool = NULL ;
}



//=================================================================================
//
// Destructor
//
// Libera los recursos si no están ya liberados
//
//=================================================================================

ClMDTSaxUpdater::~ClMDTSaxUpdater( void )
{

	m4return_t	iResult = M4_ERROR ;


	iResult = _EndRegister() ;
	iResult = _EndTable() ;

	if( m_wsCDATAPrefix != NULL )
	{
		delete [] m_wsCDATAPrefix ;
		m_wsCDATAPrefix = NULL ;
	}
}



//=================================================================================
//
// Inicializa el objeto con los objetos que usa y las variables necesarias
//
// Pasos
//		Se comprueba que los parámetros de entrada no sean nulos
//		Se crea el prefijo del CDATA
//		Se asignan las variables
//		Se registran las funciones de procesamiento de elementos
//
//=================================================================================

m4return_t	ClMDTSaxUpdater::Init( m4pcchar_t ai_pccConnection, const string& ai_rsTransactionDate, const string& ai_rsDataFile, const string& ai_rsDebugDir, M4IODBC *ai_pIODBC, IMDTDictionary *ai_pIDictionary, IMDTPresentationTool *ai_pIPresentationTool )
{

	M4MDT_RETURN_ERROR2( ai_pccConnection == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pccConnection", "ClMDTSaxUpdater::Init", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ai_pIODBC == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pIODBC", "ClMDTSaxUpdater::Init", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ai_pIDictionary == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pIDictionary", "ClMDTSaxUpdater::Init", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ai_pIPresentationTool == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pIPresentationTool", "ClMDTSaxUpdater::Init", "%s#%s" ) ;


	// UNICODE XML
	m_wsCDATAPrefix = (XMLCh*)M4CppToXML( M4MDT_XML_CDATA_PREFIX ) ;
	M4MDT_RETURN_ERROR1( m_wsCDATAPrefix == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof( XMLCh ) * M4MDT_XML_CDATA_PREFIX_LENGTH, "%d" ) ;

	m_sConnection = ai_pccConnection ;
	m_vTansactionDate.Set( ai_rsTransactionDate ) ;
	m_sDataFile = ai_rsDataFile ;
	m_sDebugDir = ai_rsDebugDir ;
	m_pIODBC = ai_pIODBC ;
	m_pIDictionary = ai_pIDictionary ;
	m_pIPresentationTool = ai_pIPresentationTool ;


	//void
	RegisterElementProc( "object-data", (ElementProcStart)&ClMDTSaxUpdater::_ProcObject ) ;
	RegisterElementProc( "table",		(ElementProcStart)&ClMDTSaxUpdater::_ProcTable ) ;
	RegisterElementProc( "register",	(ElementProcStart)&ClMDTSaxUpdater::_ProcRegister ) ;
	RegisterElementProc( "field",		(ElementProcStart)&ClMDTSaxUpdater::_ProcField ) ;

	RegisterElementEndProc( "field",		(ElementProcEnd)&ClMDTSaxUpdater::_ProcEndField ) ;
	RegisterElementEndProc( "register",		(ElementProcEnd)&ClMDTSaxUpdater::_ProcEndRegister ) ;
	RegisterElementEndProc( "table",		(ElementProcEnd)&ClMDTSaxUpdater::_ProcEndTable ) ;
	RegisterElementEndProc( "object-data",	(ElementProcEnd)&ClMDTSaxUpdater::_ProcEndObject ) ;


	return( M4_SUCCESS ) ;
}




//=================================================================================
// Funciones de actualización
//=================================================================================

//=================================================================================
//
// Convierte una presentación en formato OBL a sus partes
//
// Pasos
//		Se comprueba que los parámetros de entrada no sean nulos
//		Se resetean las variables de salida
//		Se obtiene el nombre de la presentación y el obl
//		Se delega la ejecución en la herramienta de presentaciones
//		Se asignan las variables de salida
//		Se libera la memoria
//
//=================================================================================

m4return_t	ClMDTSaxUpdater::OBL2Presentation( ClMDTValue* ai_avArguments )
{

	m4return_t	iResult = M4_ERROR ;

	size_t		iOblSize = 0 ;
	size_t		iPackageSize = 0 ;
	size_t		iBrazilianSize = 0 ;
	size_t		iEnglishSize = 0 ;
	size_t		iSpanishSize = 0 ;
	size_t		iFrenchSize = 0 ;
	size_t		iGenericSize = 0 ;
	size_t		iGermanSize = 0 ;
	size_t		iItalianSize = 0 ;

	m4pcchar_t	pccPresentation = NULL ;
	m4pcchar_t	pccObl = NULL ;
	m4pchar_t	pcPackage = NULL ;
	m4pchar_t	pcBrazilian = NULL ;
	m4pchar_t	pcEnglish = NULL ;
	m4pchar_t	pcSpanish = NULL ;
	m4pchar_t	pcFrench = NULL ;
	m4pchar_t	pcGeneric = NULL ;
	m4pchar_t	pcGerman = NULL ;
	m4pchar_t	pcItalian = NULL ;


	M4MDT_RETURN_ERROR2( ai_avArguments == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_avArguments", "M4MDTFToday", "%s#%s" ) ;


	ai_avArguments[ 2 ].SetNull() ;
	ai_avArguments[ 3 ].SetNull() ;
	ai_avArguments[ 4 ].SetNull() ;
	ai_avArguments[ 5 ].SetNull() ;
	ai_avArguments[ 6 ].SetNull() ;
	ai_avArguments[ 7 ].SetNull() ;
	ai_avArguments[ 8 ].SetNull() ;
	ai_avArguments[ 9 ].SetNull() ;

	if( ai_avArguments[ 0 ].IsNull() == M4_FALSE )
	{
		pccPresentation = ai_avArguments[ 0 ].c_str() ;
	}
	else
	{
		pccPresentation = "" ;
	}

	if( ai_avArguments[ 1 ].IsNull() == M4_FALSE )
	{
		iOblSize = ai_avArguments[ 1 ].size() ;
		pccObl = ai_avArguments[ 1 ].c_str() ;
	}
	else
	{
		iOblSize = 0 ;
		pccObl = NULL ;
	}

	// Obtener las configuraciones activas.
	vector<string> vEnableConfigurationIds;
	vEnableConfigurationIds.reserve(10);
	iResult = m_pIDictionary->GetEnableConfigurationIds(vEnableConfigurationIds);
	M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::GetEnableConfigurationIds", "OBL2Presentation", "%s#%s");

	iResult = m_pIPresentationTool->OBL2Presentation(
					pccObl, iOblSize,
					pcPackage, iPackageSize,
					pcBrazilian, iBrazilianSize,
					pcEnglish, iEnglishSize,
					pcSpanish, iSpanishSize,
					pcFrench, iFrenchSize,
					pcGeneric, iGenericSize,
					pcGerman, iGermanSize,
					pcItalian, iItalianSize,
					pccPresentation,
					vEnableConfigurationIds) ;

	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "M4MDTFOBL2Presentation", "ao_pIPresentationTool->OBL2Presentation", "%s#%s" ) ;

	ai_avArguments[ 2 ].Set( pcPackage, iPackageSize ) ;
	ai_avArguments[ 3 ].Set( pcBrazilian, iBrazilianSize ) ;
	ai_avArguments[ 4 ].Set( pcEnglish, iEnglishSize ) ;
	ai_avArguments[ 5 ].Set( pcSpanish, iSpanishSize ) ;
	ai_avArguments[ 6 ].Set( pcFrench, iFrenchSize ) ;
	ai_avArguments[ 7 ].Set( pcGeneric, iGenericSize ) ;
	ai_avArguments[ 8 ].Set( pcGerman, iGermanSize ) ;
	ai_avArguments[ 9 ].Set( pcItalian, iItalianSize ) ;

	delete [] pcPackage ;
	delete [] pcBrazilian ;
	delete [] pcEnglish ;
	delete [] pcSpanish ;
	delete [] pcFrench ;
	delete [] pcGeneric ;
	delete [] pcGerman ;
	delete [] pcItalian ;


	return( M4_SUCCESS ) ;
}



//=================================================================================
//
// Obtiene la fecha actual que es la fecha de comienzo de transacción
//
// Pasos
//		Se comprueba que los parámetros de entrada no sean nulos
//		Se asigna la variable de salida
//
//=================================================================================

m4return_t	ClMDTSaxUpdater::Today( ClMDTValue* ai_avArguments )
{
	M4MDT_RETURN_ERROR2( ai_avArguments == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_avArguments", "M4MDTFToday", "%s#%s" ) ;

	ai_avArguments[ 0 ].Set( m_vTansactionDate ) ;

	return( M4_SUCCESS ) ;
}



//=================================================================================
//
// Genera un campo blob en formato de base de datos a partir de sus partes
//
// Pasos
//		Se comprueba que los parámetros de entrada no sean nulos
//		Se resetea la variable de salida
//		Si la descripción, la extensión y el contenido son nulos la salida es nula
//		Se obtiene la descripción, la extensión, el contenido y el nombre de
//			fichero de los argumentos. Si no viene nombre de fichero se toma
//			un nombre por defecto
//		Se crea un nombre de fichero y se abre para escritura, copiando el
//			contenido del blob. El nombre está cableado a uno fijo, por lo que no
//			puede haber más de un blob por registro
//		Se compone el blob de salida con el nombre del fichero, la descripción y
//			la extensión, que es como lo espera la capa de base de datos
//
//=================================================================================

m4return_t	ClMDTSaxUpdater::File2BLOB( ClMDTValue* ai_avArguments )
{

	size_t		iWrite = 0 ;
	size_t		iSize = 0 ;
	m4pcchar_t	pccDescription = NULL ;
	m4pcchar_t	pccExtension = NULL ;
	m4pcchar_t	pccContent = NULL ;
	m4pcchar_t	pccFileName = NULL ;
	m4char_t	acFile[ M4MDT_MAX_FILE_NAME_SIZE + 1 ] ;
	FILE		*pfFile = NULL ;


	M4MDT_RETURN_ERROR2( ai_avArguments == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_avArguments", "M4MDTFFile2BLOB", "%s#%s" ) ;


	ai_avArguments[ 4 ].SetNull() ;

	if( ai_avArguments[ 0 ].IsNull() == M4_TRUE && ai_avArguments[ 1 ].IsNull() == M4_TRUE && ai_avArguments[ 2 ].IsNull() == M4_TRUE )
	{
		return( M4_SUCCESS ) ;
	}


	if( ai_avArguments[ 0 ].IsNull() == M4_FALSE )
	{
		pccDescription = ai_avArguments[ 0 ].c_str() ;
	}
	else
	{
		pccDescription = "" ;
	}

	if( ai_avArguments[ 1 ].IsNull() == M4_FALSE )
	{
		pccExtension = ai_avArguments[ 1 ].c_str() ;
	}
	else
	{
		pccExtension = "" ;
	}

	if( ai_avArguments[ 2 ].IsNull() == M4_FALSE )
	{
		pccContent = ai_avArguments[ 2 ].c_str() ;
		iSize = ai_avArguments[ 2 ].size() ;
	}
	else
	{
		pccContent = "" ;
		iSize = 0 ;
	}

	if( ai_avArguments[ 3 ].IsNull() == M4_FALSE )
	{
		pccFileName = ai_avArguments[ 3 ].c_str() ;
	}

	if( pccFileName == NULL || *pccFileName == '\0' )
	{
		pccFileName = M4MDT_BLOB_FILE_NAME ;
	}


	sprintf( acFile, "%s%s%s.%s", m_sDebugDir.c_str(), M4_BACK_SLASH, pccFileName, pccExtension ) ;

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniANSI ;
	pfFile = M4Fopen( acFile, M4UniWriteBinary, eEncoding ) ;
	M4MDT_RETURN_ERROR2( pfFile == NULL, M4_ERROR, M4MDTERROR_NO_OPEN_FILE, acFile, strerror( errno ), "%s#%s" ) ;


	iWrite = fwrite( pccContent, sizeof(m4char_t), iSize, pfFile ) ;
	fclose( pfFile ) ;
	M4MDT_RETURN_ERROR2( iWrite != iSize, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, acFile, strerror( errno ), "%s#%s" ) ;


	ai_avArguments[ 4 ].Set( acFile ) ;
	ai_avArguments[ 4 ].append( 1, '\0' ) ;
	ai_avArguments[ 4 ].append( M4LDB_BLOB_DESCR_PREFIX ) ;
	ai_avArguments[ 4 ].append( pccDescription ) ;
	ai_avArguments[ 4 ].append( 1, '\0' ) ;
	ai_avArguments[ 4 ].append( pccExtension ) ;

	return( M4_SUCCESS ) ;
}



//=================================================================================
//
// Genera un campo blob en formato de base de datos a partir de sus partes
// como fichero externo
//
// Pasos
//		Se comprueba que los parámetros de entrada no sean nulos
//		Se resetea la variable de salida
//		Si la descripción, la extensión y el contenido son nulos la salida es nula
//		Se obtiene la descripción, la extensión y el fichero de los argumentos
//		El nombre de fichero hay que concatenerlo al path del fichero de datos
//		Se compone el blob de salida con la descripción, la extensión y el nombre
//			del fichero, que es como lo espera la capa de base de datos
//
//=================================================================================

m4return_t	ClMDTSaxUpdater::External2BLOB( ClMDTValue* ai_avArguments )
{

	size_t		iWrite = 0 ;
	size_t		iSize = 0 ;
	m4pcchar_t	pccDescription = NULL ;
	m4pcchar_t	pccExtension = NULL ;
	m4pcchar_t	pccFile = NULL ;
	m4pcchar_t	pccSlash = NULL ;
	string		sFile ;


	M4MDT_RETURN_ERROR2( ai_avArguments == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_avArguments", "M4MDTFFile2BLOB", "%s#%s" ) ;


	ai_avArguments[ 3 ].SetNull() ;

	if( ai_avArguments[ 0 ].IsNull() == M4_TRUE && ai_avArguments[ 1 ].IsNull() == M4_TRUE && ai_avArguments[ 2 ].IsNull() == M4_TRUE )
	{
		return( M4_SUCCESS ) ;
	}

	
	if( ai_avArguments[ 0 ].IsNull() == M4_FALSE )
	{
		pccDescription = ai_avArguments[ 0 ].c_str() ;
	}
	else
	{
		pccDescription = "" ;
	}

	if( ai_avArguments[ 1 ].IsNull() == M4_FALSE )
	{
		pccExtension = ai_avArguments[ 1 ].c_str() ;
	}
	else
	{
		pccExtension = "" ;
	}


	sFile.reserve( 1024 ) ;
	pccFile = m_sDataFile.c_str() ;
	pccSlash = strrchr( pccFile, M4_BACK_SLASH[ 0 ] );

	if( pccSlash != NULL )
	{
		sFile.assign( pccFile, pccSlash - pccFile + 1 ) ;
	}
	else
	{
		sFile.assign( "" ) ;
	}

	if( ai_avArguments[ 2 ].IsNull() == M4_FALSE )
	{
		sFile.append( ai_avArguments[ 2 ].c_str() ) ;
	}


	ai_avArguments[ 3 ].Set( sFile ) ;
	ai_avArguments[ 3 ].append( 1, '\0' ) ;
	ai_avArguments[ 3 ].append( M4LDB_BLOB_DESCR_PREFIX ) ;
	ai_avArguments[ 3 ].append( pccDescription ) ;
	ai_avArguments[ 3 ].append( 1, '\0' ) ;
	ai_avArguments[ 3 ].append( pccExtension ) ;

	return( M4_SUCCESS ) ;
}



//=================================================================================
//
// Asigna el valor de entrada al valor de salida
// Sirve para asignar valores constantes
//
// Pasos
//		Se comprueba que los parámetros de entrada no sean nulos
//		Se asigna la variable de salida con el valor de la de entrada
//
//=================================================================================

m4return_t	ClMDTSaxUpdater::Value( ClMDTValue* ai_avArguments )
{
	M4MDT_RETURN_ERROR2( ai_avArguments == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_avArguments", "M4MDTFToday", "%s#%s" ) ;

	ai_avArguments[ 1 ].Set( ai_avArguments[ 0 ] ) ;

	return( M4_SUCCESS ) ;
}



//=================================================================================
//
// Cambia los retornos de carro \n por \r\n en el valor de entrada
// Sirve para recuperar los retornos de carro de dos caracteres que elimina la xerces
//
// Pasos
//		Se comprueba que los parámetros de entrada no sean nulos
//		Se resetea la variable de salida
//		Se recorre la cadena de entrada asignándola a la de salida y si viene el
//			caracter \n sin un \r antes se mete \r\n
//
//=================================================================================

m4return_t	ClMDTSaxUpdater::NormalizeReturns( ClMDTValue* ai_avArguments )
{

	m4char_t	c = '\0' ;
	m4char_t	cLast = '\0' ;
	size_t		i = 0 ;
	size_t		iLength = 0 ;


	M4MDT_RETURN_ERROR2( ai_avArguments == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_avArguments", "M4MDTFToday", "%s#%s" ) ;


	ai_avArguments[ 1 ].Set( "" ) ;

	iLength = ai_avArguments[ 0 ].size() ;

	// Se reserva un poco más para optimizar
	ai_avArguments[ 1 ].reserve( iLength * 1.1 ) ;

	for( i = 0 ; i < iLength ; i++ )
	{
		c = ai_avArguments[ 0 ][ i ] ;

		if( c == '\n' && cLast != '\r' )
		{
			ai_avArguments[ 1 ] += '\r' ; 
		}

		ai_avArguments[ 1 ] += c ;
		cLast = c ;
	}

	return( M4_SUCCESS ) ;
}



//=================================================================================
//
// Genera un fichero a partir de un campo package de las presentaciones de base de
// datos.
//
// Pasos
//		Se comprueba que los parámetros de entrada no sean nulos
//		Si el valor del campo no es nulo se devuelve directamente
//		Si el valor es nulo se lee del fichero
//		Se comprueba que el fichero de entrada no esté vacio.
//		Se lee el contenido del fichero y se establece el valor de la
//		variable de salida.
//
//=================================================================================

m4return_t	ClMDTSaxUpdater::External2PresentationPackage(ClMDTValue* ai_avArguments)
{
	m4return_t		iResult = M4_ERROR ;
	m4pcchar_t		pccFileName = NULL;
	m4pchar_t		pcTempBuffer = NULL;
	struct _stat	oSBuffer;

	M4MDT_RETURN_ERROR2(ai_avArguments == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_avArguments", "External2PresentationPackage", "%s#%s");


	ai_avArguments[2].Set("");

	if( ai_avArguments[ 0 ].IsNull() == M4_FALSE && ai_avArguments[ 0 ].empty() == M4_FALSE )
	{
		// Si el campo ya tiene valor se deja ese
		ai_avArguments[ 2 ].Set( ai_avArguments[ 0 ] ) ;
		return M4_SUCCESS;
	}
	
	if (ai_avArguments[1].IsNull() == M4_FALSE)
	{
		pccFileName = ai_avArguments[1].c_str();
		
		// gets the file length
		// UNICODE FILE
		int iLength = -1 ;
		char* pcFileName = M4CppToANSI( pccFileName, iLength ) ;
		m4int16_t iRes = _stat(pcFileName, &oSBuffer);
		delete [] pcFileName ;
		M4MDT_RETURN_ERROR2(iRes == -1, M4_ERROR, M4MDTERROR_NO_STAT_FILE, pccFileName, strerror(errno), "%s#%s");
		
		// opens the file
		// UNICODE FILE
		eUniFileEncodingType_t eEncoding = M4UniANSI ;
		FILE * oInput = M4Fopen(pccFileName, M4UniReadBinary, eEncoding);
		M4MDT_RETURN_ERROR2(oInput == NULL, M4_ERROR, M4MDTERROR_NO_OPEN_FILE, pccFileName, strerror(errno), "%s#%s");

		m4uint32_t lSize = oSBuffer.st_size;
		pcTempBuffer = new m4char_t[lSize + 1];
		M4MDT_RETURN_ERROR1(pcTempBuffer == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, lSize, "%d");
		
		// reads the data
		if (lSize > 0 && fread(pcTempBuffer, lSize, 1, oInput) != 1) 
		{
			M4MDT_DUMP_ERROR2(M4MDTERROR_NO_READ_FILE, pccFileName, strerror(errno), "%s#%s");
			fclose(oInput);
			delete [] pcTempBuffer;
			return M4_ERROR;
		}

		pcTempBuffer[lSize] = '\0';
		ai_avArguments[2].Set(pcTempBuffer, lSize);

		// frees the memory
		delete [] pcTempBuffer;
	
		// closes the file
		iRes = fclose(oInput);
		M4MDT_CHECK_ERROR2(iRes == EOF, M4MDTERROR_NO_CLOSE_FILE, pccFileName, strerror(errno), "%s#%d");
	}
	else
	{
		ai_avArguments[2].Set("");
	}

	return M4_SUCCESS;
}



//=================================================================================
//
// Genera un fichero a partir de un campo de base de datos.
//
// Pasos
//		Se comprueba que los parámetros de entrada no sean nulos
//		Se comprueba que el fichero de entrada no esté vacio.
//		Se lee el contenido del fichero y se establece el valor de la
//		variable de salida.
//
//=================================================================================

m4return_t	ClMDTSaxUpdater::File2Field(ClMDTValue* ai_avArguments)
{
	m4return_t		iResult = M4_ERROR ;
	m4pcchar_t		pccFileName = NULL;
	m4pchar_t		pcTempBuffer = NULL;
	m4pchar_t		pcValue = NULL;
	struct _stat	oSBuffer;

	M4MDT_RETURN_ERROR2(ai_avArguments == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_avArguments", "File2Field", "%s#%s");


	ai_avArguments[1].Set("");

	if (ai_avArguments[0].IsNull() == M4_FALSE)
	{
		pccFileName = ai_avArguments[0].c_str();
		
		// gets the file length
		// UNICODE FILE
		int iLength = -1 ;
		char* pcFileName = M4CppToANSI( pccFileName, iLength ) ;
		m4int16_t iRes = _stat(pcFileName, &oSBuffer);
		delete [] pcFileName ;
		M4MDT_RETURN_ERROR2(iRes == -1, M4_ERROR, M4MDTERROR_NO_STAT_FILE, pccFileName, strerror(errno), "%s#%s");
		
		// opens the file
		// UNICODE FILE
		eUniFileEncodingType_t eEncoding = M4UniANSI ;
		FILE * oInput = M4Fopen(pccFileName, M4UniReadBinary, eEncoding);
		M4MDT_RETURN_ERROR2(oInput == NULL, M4_ERROR, M4MDTERROR_NO_OPEN_FILE, pccFileName, strerror(errno), "%s#%s");

		iLength = oSBuffer.st_size;
		pcTempBuffer = new m4char_t[iLength + 1];
		M4MDT_RETURN_ERROR1(pcTempBuffer == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, iLength, "%d");
		
		// reads the data
		if (iLength > 0 && fread(pcTempBuffer, iLength, 1, oInput) != 1) 
		{
			M4MDT_DUMP_ERROR2(M4MDTERROR_NO_READ_FILE, pccFileName, strerror(errno), "%s#%s");
			fclose(oInput);
			delete [] pcTempBuffer;
			return M4_ERROR;
		}

		pcTempBuffer[iLength] = '\0';
		// Bug 164684. El formato debe quedar en formato UTF-8 independientemente de que
		// estemos en build Unicode o no (VB lo escribirá así). Por lo tanto aquí se debe realizar
		// la conversión a Cpp
		pcValue = M4Utf8ToCpp( pcTempBuffer, iLength ) ;
		ai_avArguments[1].Set(pcValue, iLength);

		// frees the memory
		delete [] pcTempBuffer;
		delete [] pcValue;
	
		// closes the file
		iRes = fclose(oInput);
		M4MDT_CHECK_ERROR2(iRes == EOF, M4MDTERROR_NO_CLOSE_FILE, pccFileName, strerror(errno), "%s#%d");
	}
	else
	{
		ai_avArguments[1].Set("");
	}

	return M4_SUCCESS;
}



//=================================================================================
// Funciones de procesamiento de elementos
//=================================================================================


//=================================================================================
//
// Procesa un elemento de tipo CDATA
//
// Pasos
//		Si no se está procesando un campo o el campo no es multilinea se continua
//		Si no es de tipo CDATA con value no se procesa. Para ello se mira el
//			prefijo y el sufijo del tetxo
//		Se guarda el texto quitando prefijo y sufijo
//
//=================================================================================

void	ClMDTSaxUpdater::characters( const XMLCh* const ai_pccText, const XMLSize_t ai_iLength )
{

	size_t		iLength = 0 ;
	size_t		iSize = 0 ;
	m4pchar_t	pcText = NULL ;
	m4pchar_t	pcValue = NULL ;


	if( m_eState != m4mdt_update_field || m_bIsMultiline == M4_FALSE )
	{
		return ;
	}


	if( XMLString::startsWith( ai_pccText, m_wsCDATAPrefix ) == false )
	{
		return ;
	}

	// UNICODE XML
	pcText = M4XMLToCpp( (M4XMLCh*)ai_pccText ) ;

	pcValue = pcText + M4MDT_XML_CDATA_PREFIX_LENGTH ;
	iLength = strlen( pcValue ) ;

	if( iLength < M4MDT_XML_CDATA_SUFIX_LENGTH )
	{
		delete [] pcText ;
		return ;
	}

	iSize = iLength - M4MDT_XML_CDATA_SUFIX_LENGTH ;

	if( strcmpi( pcValue + iSize, M4MDT_XML_CDATA_SUFIX ) != 0 )
	{
		delete [] pcText ;
		return ;
	}

	m_vValue.Set( pcValue, iSize ) ;
	delete [] pcText ;
}



//=================================================================================
//
// Procesa el comienzo del elemento <object-data>
//
// Pasos
//		Se comprueba que el estado de parseo sea no procesando ningún elemento
//		Se guarda el tipo de objeto que estamos procesando, dado por el
//			atributo "name"
//		Se actualiza el estado de procesamiento
//
//=================================================================================

m4void_t	ClMDTSaxUpdater::_ProcObject( const XMLCh* const ai_pccName, AttributeList& ai_roAttributes )
{

	m4return_t	iResult = M4_ERROR ;


	if( m_eState != m4mdt_update_none )
	{
		M4MDT_DUMP_ERROR3( M4MDTERROR_SEQUENCE_ERROR, M4MDTGetStatusText( m4mdt_update_object ), M4MDTGetStatusText( m_eState ), "ClMDTSaxUpdater::_ProcObject", "%s#%s#%s" ) ;
		SetError( M4_ERROR ) ;
		return ;
	}


	iResult = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChName, ai_roAttributes, m_pcObject ) ;

	if( iResult != M4_SUCCESS )
	{
		M4MDT_DUMP_ERROR2( M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_RICH_DATA, "name", "object-data", "%s#%s" ) ;
		SetError( M4_ERROR ) ;
		return ;
	}

	m_eState = m4mdt_update_object ;
}



//=================================================================================
//
// Procesa el comienzo del elemento <table>
//
// Pasos
//		Se comprueba que el estado de parseo sea procesando un "object-data"
//		Se guarda el nombre de la tabla que estamos procesando, dado por el
//			atributo "name"
//		Si hay que hacer transformaciones se guarda la función de update de la
//			tabla si es que existe, dada por el	atributo "update-function"
//		Se actualiza el estado de procesamiento
//
//=================================================================================

m4void_t	ClMDTSaxUpdater::_ProcTable( const XMLCh* const ai_pccName, AttributeList& ai_roAttributes )
{

	m4return_t	iResult = M4_ERROR ;
	m4bool_t	bDoTransformations = M4_FALSE ;


	if( m_eState != m4mdt_update_object )
	{
		M4MDT_DUMP_ERROR3( M4MDTERROR_SEQUENCE_ERROR, M4MDTGetStatusText( m4mdt_update_table ), M4MDTGetStatusText( m_eState ), "ClMDTSaxUpdater::_ProcTable", "%s#%s#%s" ) ;
		SetError( M4_ERROR ) ;
		return ;
	}


	iResult = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChName, ai_roAttributes, m_pcTable ) ;

	if( iResult != M4_SUCCESS )
	{
		M4MDT_DUMP_ERROR2( M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_RICH_DATA, "name", "table", "%s#%s" ) ;
		SetError( M4_ERROR ) ;
		return ;
	}

	bDoTransformations = _HasToDoTransformations() ;

	if( bDoTransformations == M4_TRUE )
	{
		iResult = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChUpdateFunctions, ai_roAttributes, m_pcUpdateFunctions ) ;
	}

	m_eState = m4mdt_update_table ;
}



//=================================================================================
//
// Procesa el comienzo del elemento <register>
//
// Pasos
//		Se comprueba que el estado de parseo sea procesando un "table"
//		Se obtiene la información necesaria para procesar la tabla en la que
//			estamos si es que no se ha hecho antes.
//		Se crean las estructuras necesarias para almacenar los datos del registro
//			que se empieza a procesar
//		Se actualiza el estado de procesamiento
//
//=================================================================================

m4void_t	ClMDTSaxUpdater::_ProcRegister( const XMLCh* const ai_pccName, AttributeList& ai_roAttributes )
{

	m4return_t	iResult = NULL ;


	if( m_eState != m4mdt_update_table )
	{
		M4MDT_DUMP_ERROR3( M4MDTERROR_SEQUENCE_ERROR, M4MDTGetStatusText( m4mdt_update_register ), M4MDTGetStatusText( m_eState ), "ClMDTSaxUpdater::_ProcRegister", "%s#%s#%s" ) ;
		SetError( M4_ERROR ) ;
		return ;
	}


	iResult = _InitTable() ;

	if( iResult != M4_SUCCESS )
	{
		M4MDT_DUMP_ERROR2( M4MDTERROR_INTERNAL_ERROR, "ClMDTSaxUpdater::_ProcRegister", "_InitTable", "%s#%s" ) ;
		SetError( M4_ERROR ) ;
		return ;
	}

	iResult = _InitRegister() ;

	if( iResult != M4_SUCCESS )
	{
		M4MDT_DUMP_ERROR2( M4MDTERROR_INTERNAL_ERROR, "ClMDTSaxUpdater::_ProcRegister", "_InitRegister", "%s#%s" ) ;
		SetError( M4_ERROR ) ;
		return ;
	}

	m_eState = m4mdt_update_register ;
}



//=================================================================================
//
// Procesa el comienzo del elemento <field>
//
// Pasos
//		Se comprueba que el estado de parseo sea procesando un "register"
//		Se guarda el nombre del campo que estamos procesando, dado por el
//			atributo "name"
//		Si el campo es nulo se marca como tal y como no multilinea y no se hace
//			nada más. Para ello	se consulta si el atributo "is-null" existe y
//			vale "true"
//		Si no es nulo, y si el campo es multilinea se marca como tal y no se hace
//			nada más. Para ello se consulta si el atributo "multiline" existe y
//			vale "true"
//		Si no es nulo ni multilinea se guarda su valor, dado por el atributo
//			"value"
//		Se actualiza el estado de procesamiento
//
//=================================================================================

m4void_t	ClMDTSaxUpdater::_ProcField( const XMLCh* const ai_pccName, AttributeList& ai_roAttributes )
{

	m4return_t		iResult = M4_ERROR ;
	ClCharHolder	pcMultiline ;
	ClCharHolder	pcIsNull ;
	ClCharHolder	pcValue ;

	
	if( m_eState != m4mdt_update_register )
	{
		M4MDT_DUMP_ERROR3( M4MDTERROR_SEQUENCE_ERROR, M4MDTGetStatusText( m4mdt_update_field ), M4MDTGetStatusText( m_eState ), "ClMDTSaxUpdater::_ProcField", "%s#%s#%s" ) ;
		SetError( M4_ERROR ) ;
		return ;
	}


	iResult = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChName, ai_roAttributes, m_pcField ) ;

	if( iResult != M4_SUCCESS )
	{
		M4MDT_DUMP_ERROR2( M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_RICH_DATA, "name", "field", "%s#%s" ) ;
		SetError( M4_ERROR ) ;
		return ;
	}


	m_bIsMultiline = M4_FALSE ;
	m_vValue.SetNull() ;

	iResult = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChIsNull, ai_roAttributes, pcIsNull ) ;

	if( iResult == M4_SUCCESS && strcmpi( pcIsNull, "true" ) == 0 )
	{
		m_vValue.SetNull() ;
	}
	else
	{
		iResult = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChMultiline, ai_roAttributes, pcMultiline ) ;

		if( iResult == M4_SUCCESS && strcmpi( pcMultiline, "true" ) == 0 )
		{
			m_bIsMultiline = M4_TRUE ;
		}
		else
		{
			iResult = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChValue, ai_roAttributes, pcValue ) ;

			if( iResult != M4_SUCCESS )
			{
				M4MDT_DUMP_ERROR2( M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_RICH_DATA, "value", "field", "%s#%s" ) ;
				SetError( M4_ERROR ) ;
				return ;
			}

			m_vValue.Set( pcValue ) ;
		}
	}

	m_eState = m4mdt_update_field ;
}



//=================================================================================
//
// Procesa el final del elemento </field>
//
// Pasos
//		Si hay funciones de transformación se busca el campo que se está procesando
//			en el array de parámetros de cada una de ellas. Si se encuentra se
//			asigna el valor actual
//		Se pide procesar el valor del campo, guardándolo como parámetro de la
//			sentencia
//		Se resetea el nombre, el valor y los flags del campo
//		Se actualiza el estado de procesamiento
//
//=================================================================================

m4void_t	ClMDTSaxUpdater::_ProcEndField( const XMLCh* const ai_pccName )
{

	m4return_t			iResult = NULL ;
	size_t				i = 0 ;
	size_t				iFunctions = 0 ;
	m4int16_t			iParameter = -1 ;
	ClMDTValue			*poValues = NULL ;
	ClMDTUpdateFunction	*poFunction = NULL ;


	iResult = _SetValue( m_pcField, m_vValue ) ;

	m_pcField.SetNull() ;
	m_bIsMultiline = M4_FALSE ;
	m_vValue.SetNull() ;

	if( iResult != M4_SUCCESS )
	{
		M4MDT_DUMP_ERROR2( M4MDTERROR_INTERNAL_ERROR, "ClMDTSaxUpdater::_ProcEndField", "_SetValue", "%s#%s" ) ;
		SetError( M4_ERROR ) ;
		return ;
	}

	m_eState = m4mdt_update_register ;
}



//=================================================================================
//
// Procesa el final del elemento </register>
//
// Pasos
//		Se ejecutan las funciones de actualización
//		Se pide ejecutar la sentencia de todas las tablas físicas
//		Se liberan las estructuras que almacenaban los datos del registro
//		Se actualiza el estado de procesamiento
//
//=================================================================================

m4void_t	ClMDTSaxUpdater::_ProcEndRegister( const XMLCh* const ai_pccName )
{

	m4return_t	iResult = M4_ERROR ;


	iResult = _DoTransformations() ;

	if( iResult != M4_SUCCESS )
	{
		M4MDT_DUMP_ERROR2( M4MDTERROR_INTERNAL_ERROR, "ClMDTSaxUpdater::_ProcEndRegister", "_DoTransformations", "%s#%s" ) ;
		SetError( M4_ERROR ) ;
		return ;
	}

	iResult = _Execute() ;

	if( iResult != M4_SUCCESS )
	{
		M4MDT_DUMP_ERROR2( M4MDTERROR_INTERNAL_ERROR, "ClMDTSaxUpdater::_ProcEndRegister", "_Execute", "%s#%s" ) ;
		SetError( M4_ERROR ) ;
		return ;
	}

	iResult = _EndRegister() ;

	if( iResult != M4_SUCCESS )
	{
		M4MDT_DUMP_ERROR2( M4MDTERROR_INTERNAL_ERROR, "ClMDTSaxUpdater::_ProcEndRegister", "_EndRegister", "%s#%s" ) ;
		SetError( M4_ERROR ) ;
		return ;
	}

	m_eState = m4mdt_update_table ;
}



//=================================================================================
//
// Procesa el final del elemento </table>
//
// Pasos
//		Se libera la información necesaria para procesar la tabla
//		Se resetea el nombre de la tabla y la función de actualización
//		Se actualiza el estado de procesamiento
//
//=================================================================================

m4void_t	ClMDTSaxUpdater::_ProcEndTable( const XMLCh* const ai_pccName )
{

	m4return_t	iResult = NULL ;

	
	iResult = _EndTable() ;

	m_pcTable.SetNull() ;
	m_pcUpdateFunctions.SetNull() ;

	if( iResult != M4_SUCCESS )
	{
		M4MDT_DUMP_ERROR2( M4MDTERROR_INTERNAL_ERROR, "ClMDTSaxUpdater::_ProcEndTable", "_EndTable", "%s#%s" ) ;
		SetError( M4_ERROR ) ;
		return ;
	}

	m_eState = m4mdt_update_object ;
}



//=================================================================================
//
// Procesa el final del elemento </object-data>
//
// Pasos
//		Se resetea el nombre del objeto
//		Se actualiza el estado de procesamiento
//
//=================================================================================

m4void_t	ClMDTSaxUpdater::_ProcEndObject( const XMLCh* const ai_pccName )
{
	m_pcObject.SetNull() ;

	m_eState = m4mdt_update_none ;
}





//=================================================================================
// Funciones auxiliares
//=================================================================================


//=================================================================================
//
// Inicializa los datos necesarios para utilizar una tabla
// Carga el diccionario y procesa la definición de la función de actualización
//
// Pasos
//		Si la información del diccionario ya está cargada no se hace nada
//		Se pide al diccionario el xml de la tabla que se está procesando
//		Se parsea el xml buscando los elementos <logic-fields> y <real-objects>,
//			de los cuales se obtiene una referencia
//		Si existen funciones de transformación se procesa su defición. Se parsean
//			sus argumentos por nombre y después de rellenar la información de la
//			función se guarda en un vector
//
//=================================================================================

m4return_t	ClMDTSaxUpdater::_InitTable( void )
{

	m4return_t				iResult = M4_ERROR ;
	m4bool_t				bMoreFunctions = M4_FALSE ;
	int						iScan = 0 ;
	unsigned int			iElement = -1 ;
	m4pcchar_t				pccFunctions = NULL ;
	ClMDTUpdateFunction		*poFunction = NULL ;
	xercesc::DOMDocument	*poDocument = NULL ;
	DOMNode					*poNode = NULL ;
	DOMNode					*poChildNode = NULL ;

	m4char_t				acFunctionName [ M4MDT_MAX_UPDATE_FUNCTION_NAME_SIZE ] ;
	m4char_t				acNames [ M4MDT_MAX_UPDATE_FUNCTION_ARG_NUMBER ][ M4MDT_MAX_UPDATE_FUNCTION_ARGUMENT_SIZE ] ;
	m4char_t				acValues[ M4MDT_MAX_UPDATE_FUNCTION_ARG_NUMBER ][ M4MDT_MAX_UPDATE_FUNCTION_ARGUMENT_SIZE ] ;

	ClMDTDomTraverser::iterator	itFieldIterator ;


	if( m_oFields.empty() == false && m_oTables.empty() == false )
	{
		return( M4_SUCCESS ) ;
	}

	iResult = m_pIDictionary->GetDOMTable( m_pcTable, poDocument ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS || poDocument == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSaxUpdater::_InitTable", "m_pIDictionary->GetDOMTable", (m4pcchar_t) m_pcTable, "%s#%s - Parameter = <%s>" ) ;

	poNode = poDocument->getDocumentElement() ;


	iElement = M4MDT_DOMFindChildNode( IMDTService::sm_sXMLChLogicFields, 0, poNode, poChildNode ) ;
	M4MDT_RETURN_ERROR3( iElement == -1, M4_ERROR, M4MDTERROR_NO_SUCH_ELEMENT_IN_DICT, "*", "logic-fields", (m4pcchar_t) m_pcTable, "%s#%s#%s" ) ;

	iResult = m_oFields.Init( IMDTService::sm_sXMLChLogicField, IMDTService::sm_sXMLChName, poChildNode ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSaxUpdater::_InitTable", "m_oFields.Init", "%s#%s" ) ;

	iElement = M4MDT_DOMFindChildNode( IMDTService::sm_sXMLChRealObjects, 0, poNode, poChildNode ) ;
	M4MDT_RETURN_ERROR3( iElement == -1, M4_ERROR, M4MDTERROR_NO_SUCH_ELEMENT_IN_DICT, "*", "real-objects", (m4pcchar_t) m_pcTable, "%s#%s#%s" ) ;

	iResult = m_oTables.Init( IMDTService::sm_sXMLChRealObject, IMDTService::sm_sXMLChName, poChildNode ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSaxUpdater::_InitTable", "m_oTables.Init", "%s#%s" ) ;


	if( m_pcUpdateFunctions != NULL )
	{
		pccFunctions = m_pcUpdateFunctions ;
		bMoreFunctions = M4_TRUE ;

		while( bMoreFunctions == M4_TRUE )
		{
			iScan = sscanf(		pccFunctions,
								"%[^( ] ( %[^= ] = %[^,) ] , %[^= ] = %[^,) ] , %[^= ] = %[^,) ] , %[^= ] = %[^,) ] , %[^= ] = %[^,) ] , %[^= ] = %[^,) ] , %[^= ] = %[^,) ] , %[^= ] = %[^,) ] , %[^= ] = %[^,) ] , %[^= ] = %[^) ] )",
								acFunctionName,
								acNames[ 0 ], acValues[ 0 ],
								acNames[ 1 ], acValues[ 1 ],
								acNames[ 2 ], acValues[ 2 ],
								acNames[ 3 ], acValues[ 3 ],
								acNames[ 4 ], acValues[ 4 ],
								acNames[ 5 ], acValues[ 5 ],
								acNames[ 6 ], acValues[ 6 ],
								acNames[ 7 ], acValues[ 7 ],
								acNames[ 8 ], acValues[ 8 ],
								acNames[ 9 ], acValues[ 9 ] ) ;

			M4MDT_RETURN_ERROR2( iScan < 1, M4_ERROR, M4MDTERROR_BAD_UPDATE_FUNCTION, pccFunctions, (m4pcchar_t) m_pcTable, "%s#%s" ) ;

			poFunction = new ClMDTUpdateFunction ;
			M4MDT_RETURN_ERROR1( poFunction == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof( ClMDTUpdateFunction ), "%d" ) ;

			m_vUpdateFunctions.push_back( poFunction ) ;

			if( strcmpi( acFunctionName, M4MDT_OBL2PRES_NAME ) == 0 )
			{
				M4MDT_RETURN_ERROR3( iScan != M4MDT_OBL2PRES_ARG_NUMBER * 2 + 1, M4_ERROR, M4MDTERROR_BAD_UPDATE_FUNCTION, M4MDT_OBL2PRES_NAME, (m4pcchar_t) m_pcTable, pccFunctions, "%s#%s#%s" ) ;


				iResult = poFunction->Init( &ClMDTSaxUpdater::OBL2Presentation, s_accOBL2PresArguments, acNames, acValues, M4MDT_OBL2PRES_ARG_NUMBER, acFunctionName, m_pcTable ) ;
				M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSaxUpdater::_InitTable", "poFunction->Init", "%s#%s" ) ;
			}
			else if( strcmpi( acFunctionName, M4MDT_TODAY_NAME ) == 0 )
			{
				M4MDT_RETURN_ERROR3( iScan != M4MDT_TODAY_ARG_NUMBER * 2 + 1, M4_ERROR, M4MDTERROR_BAD_UPDATE_FUNCTION, M4MDT_TODAY_NAME, (m4pcchar_t) m_pcTable, pccFunctions, "%s#%s#%s" ) ;


				iResult = poFunction->Init( &ClMDTSaxUpdater::Today, s_accTodayArguments, acNames, acValues, M4MDT_TODAY_ARG_NUMBER, acFunctionName, m_pcTable ) ;
				M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSaxUpdater::_InitTable", "poFunction->Init", "%s#%s" ) ;
			}
			else if( strcmpi( acFunctionName, M4MDT_FILE2BLOB_NAME ) == 0 )
			{
				M4MDT_RETURN_ERROR3( iScan != M4MDT_FILE2BLOB_ARG_NUMBER * 2 + 1, M4_ERROR, M4MDTERROR_BAD_UPDATE_FUNCTION, M4MDT_FILE2BLOB_NAME, (m4pcchar_t) m_pcTable, pccFunctions, "%s#%s#%s" ) ;

				iResult = poFunction->Init( &ClMDTSaxUpdater::File2BLOB, s_accFile2BLOBArguments, acNames, acValues, M4MDT_FILE2BLOB_ARG_NUMBER, acFunctionName, m_pcTable ) ;
				M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSaxUpdater::_InitTable", "poFunction->Init", "%s#%s" ) ;
			}
			else if( strcmpi( acFunctionName, M4MDT_EXTERNAL2BLOB_NAME ) == 0 )
			{
				M4MDT_RETURN_ERROR3( iScan != M4MDT_EXTERNAL2BLOB_ARG_NUMBER * 2 + 1, M4_ERROR, M4MDTERROR_BAD_UPDATE_FUNCTION, M4MDT_EXTERNAL2BLOB_NAME, (m4pcchar_t) m_pcTable, pccFunctions, "%s#%s#%s" ) ;

				iResult = poFunction->Init( &ClMDTSaxUpdater::External2BLOB, s_accExternal2BLOBArguments, acNames, acValues, M4MDT_EXTERNAL2BLOB_ARG_NUMBER, acFunctionName, m_pcTable ) ;
				M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSaxUpdater::_InitTable", "poFunction->Init", "%s#%s" ) ;
			}
			else if( strcmpi( acFunctionName, M4MDT_VALUE_NAME ) == 0 )
			{
				M4MDT_RETURN_ERROR3( iScan != M4MDT_VALUE_ARG_NUMBER * 2 + 1, M4_ERROR, M4MDTERROR_BAD_UPDATE_FUNCTION, M4MDT_VALUE_NAME, (m4pcchar_t) m_pcTable, pccFunctions, "%s#%s#%s" ) ;

				iResult = poFunction->Init( &ClMDTSaxUpdater::Value, s_accValueArguments, acNames, acValues, M4MDT_VALUE_ARG_NUMBER, acFunctionName, m_pcTable ) ;
				M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSaxUpdater::_InitTable", "poFunction->Init", "%s#%s" ) ;
			}
			else if( strcmpi( acFunctionName, M4MDT_NORMALIZERETURNS_NAME ) == 0 )
			{
				M4MDT_RETURN_ERROR3( iScan != M4MDT_NORMALIZERETURNS_ARG_NUMBER * 2 + 1, M4_ERROR, M4MDTERROR_BAD_UPDATE_FUNCTION, M4MDT_NORMALIZERETURNS_NAME, (m4pcchar_t) m_pcTable, pccFunctions, "%s#%s#%s" ) ;

				iResult = poFunction->Init( &ClMDTSaxUpdater::NormalizeReturns, s_accNormalizeReturnsArguments, acNames, acValues, M4MDT_NORMALIZERETURNS_ARG_NUMBER, acFunctionName, m_pcTable ) ;
				M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSaxUpdater::_InitTable", "poFunction->Init", "%s#%s" ) ;
			}
			else if( strcmpi( acFunctionName, M4MDT_EXTERNAL2PRESENTATIONPACK_NAME ) == 0 )
			{
				M4MDT_RETURN_ERROR3( iScan != M4MDT_EXTERNAL2PRESENTATIONPACK_ARG_NUMBER * 2 + 1, M4_ERROR, M4MDTERROR_BAD_UPDATE_FUNCTION, M4MDT_EXTERNAL2PRESENTATIONPACK_NAME, (m4pcchar_t) m_pcTable, pccFunctions, "%s#%s#%s" ) ;

				iResult = poFunction->Init( &ClMDTSaxUpdater::External2PresentationPackage, s_accExternal2PresentationPackageArguments, acNames, acValues, M4MDT_EXTERNAL2PRESENTATIONPACK_ARG_NUMBER, acFunctionName, m_pcTable ) ;
				M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSaxUpdater::_InitTable", "poFunction->Init", "%s#%s" ) ;
			}
			else if( strcmpi( acFunctionName, M4MDT_FILE2FIELD_NAME ) == 0 )
			{
				M4MDT_RETURN_ERROR3( iScan != M4MDT_FILE2FIELD_ARG_NUMBER * 2 + 1, M4_ERROR, M4MDTERROR_BAD_UPDATE_FUNCTION, M4MDT_FILE2FIELD_NAME, (m4pcchar_t) m_pcTable, pccFunctions, "%s#%s#%s" ) ;

				iResult = poFunction->Init(&ClMDTSaxUpdater::File2Field, s_accFile2FieldArguments, acNames, acValues, M4MDT_FILE2FIELD_ARG_NUMBER, acFunctionName, m_pcTable ) ;
				M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSaxUpdater::_InitTable", "poFunction->Init", "%s#%s" ) ;
			}
			else
			{
				M4MDT_DUMP_ERROR2( M4MDTERROR_UNSUPPORTED_UPDATE_FUNCTION, acFunctionName, (m4pcchar_t) m_pcTable, "%s#%s" ) ;
				return( M4_ERROR ) ;
			}

			pccFunctions = strchr( pccFunctions, ';' ) ;

			if( pccFunctions != NULL )
			{
				pccFunctions++ ;

				if( *pccFunctions == '\0' )
				{
					bMoreFunctions = M4_FALSE ;
				}
			}
			else
			{
				bMoreFunctions = M4_FALSE ;
			}
		}
	}


	return( M4_SUCCESS ) ;
}



//=================================================================================
//
// Inicializa los datos necesarios para utilizar un registro
// Crea los parámetros de la función de actualización y las filas de parámetros
// de la sentencia
//
// Pasos
//		Se reserva espacio para los parámetros de las funciones de actualización
//		Se asignan los argumentos constantes de las funciones de actualización
//		Se cuenta el número de tablas físicas existentes
//		Se crean los parámetros de la sentencia de todas las tablas físicas
//		Se reserva espacio en los parámetros de las sentencias de las tablas
//		Se rellena la descripción de los parámetros
//		Para los tipos internos especiales se rellena su valor y para los demás
//			se inicializa a null (para los campos no visibles)
//
//=================================================================================

m4return_t	ClMDTSaxUpdater::_InitRegister( void )
{

	m4return_t		iResult = M4_ERROR ;
	m4uint16_t		i = 0 ;
	m4uint16_t		iParameters = 0 ;
	m4uint32_t		iTable = 0 ;
	m4uint32_t		iField = 0 ;
	m4uint32_t		iTableNumber = 0 ;
	int				iParameterNumber = 0 ;
	int				iTablePosition = 0 ;
	int				iFieldPosition = 0 ;
	int				iM4Type = 0 ;
	int				iPrecision = 0 ;
	int				iScale = 0 ;
	int				iInternalField = 0 ;
	m4pcchar_t		pccParameter = NULL ;
	ClCharHolder	pcParameterNumber ;
	ClCharHolder	pcTablePosition ;
	ClCharHolder	pcFieldPosition ;
	ClCharHolder	pcM4Type ;
	ClCharHolder	pcPrecision ;
	ClCharHolder	pcScale ;
	ClCharHolder	pcAlias ;
	ClCharHolder	pcInternalField ;
	const XMLCh		*pcsParameterNumberName = NULL;
	const XMLCh		*pcsFieldPositionName = NULL ;
	ClMDTValue		*pvValues = NULL ;
	ClMDTValue		vNullValue ;
	DOMNode			*poTable = NULL ;
	DOMNode			*poField = NULL ;

	FunctionVector_t::iterator	itFunctionIterator ;
	ClMDTDomTraverser::iterator	itTableIterator ;
	ClMDTDomTraverser::iterator	itFieldIterator ;


	itFunctionIterator = m_vUpdateFunctions.begin() ;

	while( itFunctionIterator != m_vUpdateFunctions.end() )
	{
		iParameters = (*itFunctionIterator)->GetParameterNumber() ;

		pvValues = new ClMDTValue[ iParameters ] ;
		M4MDT_RETURN_ERROR1( pvValues == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof( ClMDTValue ) * iParameters, "%d" ) ;

		for( i = 0 ; i < iParameters ; i++ )
		{
			pccParameter = (*itFunctionIterator)->GetParameter( i ) ;

			if( pccParameter != NULL && *pccParameter != '@' )
			{
				pvValues[ i ].Set( pccParameter ) ;
			}
		}

		m_vUpdateValues.push_back( pvValues ) ;

		itFunctionIterator++ ;
	}


	iTableNumber = m_oTables.size() ; ;

	m_poRows = new M4Row[ iTableNumber ] ;
	M4MDT_RETURN_ERROR1( m_poRows == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof( M4Row ) * iTableNumber, "%d" ) ;


	pcsParameterNumberName = _GetParameterNumberName() ;

	iTable = 0 ;
	itTableIterator = m_oTables.begin() ;

	while( itTableIterator != m_oTables.end() )
	{
		poTable = (*itTableIterator).second ;

		iResult = M4MDT_DOMGetAttribute( pcsParameterNumberName, poTable, pcParameterNumber ) ;
		M4MDT_RETURN_ERROR4( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_DICT, "number-name", iTable, "real-object", (m4pcchar_t) m_pcTable, "%s#%d#%s#%s" ) ;

		iResult = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChTablePosition, poTable, pcTablePosition ) ;
		M4MDT_RETURN_ERROR4( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_DICT, "table-position", iTable, "real-object", (m4pcchar_t) m_pcTable, "%s#%d#%s#%s" ) ;

		iParameterNumber = atoi( pcParameterNumber ) ;
		iTablePosition = atoi( pcTablePosition ) ;

		iResult = m_poRows[ iTablePosition - 1 ].Init( iParameterNumber ) ;
		M4MDT_RETURN_ERROR4( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSaxUpdater::_InitRegister", "Init", iTable, iParameterNumber, "%s#%s - Table <%d> - Parameter <%d>" ) ;

		iTable++ ;
		itTableIterator++ ;
	}


	pcsFieldPositionName = _GetParameterPositionName() ;

	iField = 0 ;
	itFieldIterator = m_oFields.begin() ;

	while( itFieldIterator != m_oFields.end() )
	{
		poField = (*itFieldIterator).second ;

		iResult = M4MDT_DOMGetAttribute( pcsFieldPositionName, poField, pcFieldPosition ) ;
		M4MDT_RETURN_ERROR4( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_DICT, "position-name", iField, "logic-field", (m4pcchar_t) m_pcTable, "%s#%d#%s#%s" ) ;

		iFieldPosition = atoi( pcFieldPosition ) ;

		if( iFieldPosition > 0 )
		{
			iResult = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChM4Type, poField, pcM4Type ) ;
			M4MDT_RETURN_ERROR4( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_DICT, "m4type", iField, "logic-field", (m4pcchar_t) m_pcTable, "%s#%d#%s#%s" ) ;

			iResult = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChPrecision, poField, pcPrecision ) ;
			M4MDT_RETURN_ERROR4( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_DICT, "precision", iField, "logic-field", (m4pcchar_t) m_pcTable, "%s#%d#%s#%s" ) ;

			iResult = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChScale, poField, pcScale ) ;
			M4MDT_RETURN_ERROR4( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_DICT, "scale", iField, "logic-field", (m4pcchar_t) m_pcTable, "%s#%d#%s#%s" ) ;

			iResult = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChAlias, poField, pcAlias ) ;
			M4MDT_RETURN_ERROR4( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_DICT, "real_object", iField, "logic-field", (m4pcchar_t) m_pcTable, "%s#%d#%s#%s" ) ;

			iResult = m_oTables.FindChild( (m4pchar_t) pcAlias, poTable ) ;
			M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ELEMENT_IN_DICT, pcAlias, "real-object", (m4pcchar_t) m_pcTable, "%s#%s#%s" ) ;

			iResult = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChTablePosition, poTable, pcTablePosition ) ;
			M4MDT_RETURN_ERROR4( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_DICT, "table-position", pcAlias, "real-object", (m4pcchar_t) m_pcTable, "%s#%s#%s#%s" ) ;

			iM4Type        = atoi( pcM4Type ) ;
			iPrecision     = atoi( pcPrecision ) ;
			iScale         = atoi( pcScale ) ;
			iTablePosition = atoi( pcTablePosition ) ;

			iResult = m_poRows[ iTablePosition - 1 ].SetFieldDescription( iFieldPosition - 1, iM4Type, iPrecision, iScale ) ;
			M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSaxUpdater::_InitRegister", "SetFieldDescription", iFieldPosition - 1, "%s#%s - Parameter = <%d>" ) ;


			iResult = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChInternalField, poField, pcInternalField ) ;
			M4MDT_RETURN_ERROR4( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_DICT, "internal-field", iField, "logic-field", (m4pcchar_t) m_pcTable, "%s#%d#%s#%s" ) ;

			iInternalField = atoi( pcInternalField ) ;

			switch( iInternalField )
			{
				case M4CL_INTERNAL_TYPE_T_DATE :

					iResult = m_poRows[ iTablePosition - 1 ].SetValue( iFieldPosition - 1, m_vTansactionDate ) ;
					M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSaxUpdater::_InitRegister", "SetValue", iFieldPosition - 1, "%s#%s - Parameter = <%d>" ) ;
					break ;

				case M4CL_INTERNAL_TYPE_SECURITY_USER :
				case M4CL_INTERNAL_TYPE_APP_ROLE :

					iResult = m_poRows[ iTablePosition - 1 ].SetValue( iFieldPosition - 1, "RAMDL" ) ;
					M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSaxUpdater::_InitRegister", "SetValue", iFieldPosition - 1, "%s#%s - Parameter = <%d>" ) ;
					break ;

				default :

					iResult = m_poRows[ iTablePosition - 1 ].SetValue( iFieldPosition - 1, vNullValue ) ;
					M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSaxUpdater::_InitRegister", "SetValue", iFieldPosition - 1, "%s#%s - Parameter = <%d>" ) ;
					break ;
			}
		}

		iField++ ;
		itFieldIterator++ ;
	}


	return( M4_SUCCESS ) ;
}



//=================================================================================
//
// Añade el valor de un campo a su correspondiente M4Row
//
// Pasos
//		Se itera por todos los campos lógicos del diccionario que se llaman como
//			el campo que estamos procesando
//		Para cada campo encontrado se obtiene su posición en la	sentencia
//		Si tienen posición se sigue procesando ese campo
//		Se obtiene su tabla física
//		Se busca la tabla física y se obtiene su número de tabla
//		Se rellena el valor del parámetro de la sentencia
//		Hay que notar que los índices de las tablas y campos empiezan en 1
//		Si el campo es calculado, ya no va a aparecer en la lista y no se trata
//
//=================================================================================

m4return_t	ClMDTSaxUpdater::_SetValue( m4pcchar_t ai_pccField, ClMDTValue &ai_rvValue )
{

	m4return_t			iResult = M4_ERROR ;
	int					iFieldPosition = 0 ;
	int					iTablePosition = 0 ;
	size_t				i = 0 ;
	size_t				iFunctions = 0 ;
	m4int16_t			iParameter = -1 ;
	ClCharHolder		pcName ;
	ClCharHolder		pcFieldPosition ;
	ClCharHolder		pcTablePosition ;
	ClCharHolder		pcAlias ;
	const XMLCh			*pcsFieldPositionName = NULL ;
	DOMNode				*poField = NULL ;
	DOMNode				*poTable = NULL ;
	ClMDTValue			*poValues = NULL ;
	ClMDTUpdateFunction	*poFunction = NULL ;

	ClMDTDomTraverser::iterator	itFieldIterator ;


	pcsFieldPositionName = _GetParameterPositionName() ;

	itFieldIterator = m_oFields.find( ai_pccField ) ;

	while( itFieldIterator != m_oFields.end() )
	{
		poField = (*itFieldIterator).second ;

		iResult = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChName, poField, pcName ) ;
		M4MDT_RETURN_ERROR4( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_DICT, "name", ai_pccField, "logic-field", (m4pcchar_t) m_pcTable, "%s#%s#%s#%s" ) ;

		if( strcmpi( pcName, ai_pccField ) != 0 )
		{
			break ;
		}

		iResult = M4MDT_DOMGetAttribute( pcsFieldPositionName, poField, pcFieldPosition ) ;
		M4MDT_RETURN_ERROR4( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_DICT, "position-name", ai_pccField, "logic-field", (m4pcchar_t) m_pcTable, "%s#%s#%s#%s" ) ;

		iFieldPosition = atoi( pcFieldPosition ) ;

		if( iFieldPosition > 0 )
		{
			iResult = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChAlias, poField, pcAlias ) ;
			M4MDT_RETURN_ERROR4( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_DICT, "real_object", ai_pccField, "logic-field", (m4pcchar_t) m_pcTable, "%s#%s#%s#%s" ) ;

			iResult = m_oTables.FindChild( (m4pchar_t) pcAlias, poTable ) ;
			M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ELEMENT_IN_DICT, pcAlias, "real-object", (m4pcchar_t) m_pcTable, "%s#%s#%s" ) ;

			iResult = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChTablePosition, poTable, pcTablePosition ) ;
			M4MDT_RETURN_ERROR4( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_DICT, "table-position", pcAlias, "real-object", (m4pcchar_t) m_pcTable, "%s#%s#%s#%s" ) ;

			iTablePosition = atoi( pcTablePosition ) ;

			iResult = m_poRows[ iTablePosition - 1 ].SetValue( iFieldPosition - 1, ai_rvValue ) ;
			M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSaxUpdater::_SetValue", "SetValue", iFieldPosition - 1, "%s#%s - Parameter = <%d>" ) ;
		}

		itFieldIterator++ ;
	}

	// Bug 0157796. Actualizar los valores de los parámetros de las funciones.
	iFunctions = m_vUpdateFunctions.size();
	for (i = 0 ; i < iFunctions ; i++)
	{
		poFunction = m_vUpdateFunctions[i];
		iParameter = poFunction->FindParameterByValue(ai_pccField);

		if (iParameter != -1)
		{
			poValues = m_vUpdateValues[i] ;
			poValues[iParameter].Set(ai_rvValue);
		}
	}

	return( M4_SUCCESS ) ;
}


//=================================================================================
//
// Obtiene el valor de un campo en su correspondiente M4Row
//
// Pasos
//		Se itera por todos los campos lógicos del diccionario que se llaman como
//		el campo que estamos procesando
//		Para cada campo encontrado se obtiene su posición en la	sentencia
//		Si tienen posición se sigue procesando ese campo
//		Se obtiene su tabla física
//		Se busca la tabla física y se obtiene su número de tabla
//		Se obtiene el valor del parámetro de la sentencia
//		Hay que notar que los índices de las tablas y campos empiezan en 1
//		Si el campo es calculado, ya no va a aparecer en la lista y no se trata
//
//=================================================================================

m4return_t	ClMDTSaxUpdater::_GetValue(m4pcchar_t ai_pccField, ClMDTValue &ao_rvValue)
{

	m4return_t		iResult = M4_ERROR;
	int				iFieldPosition = 0;
	int				iTablePosition = 0;
	ClCharHolder	pcName;
	ClCharHolder	pcFieldPosition;
	ClCharHolder	pcTablePosition;
	ClCharHolder	pcAlias;
	const XMLCh		*pcsFieldPositionName = NULL;
	DOMNode			*poField = NULL;
	DOMNode			*poTable = NULL;

	ClMDTDomTraverser::iterator	itFieldIterator;


	pcsFieldPositionName = _GetParameterPositionName();

	itFieldIterator = m_oFields.find( ai_pccField );

	while (itFieldIterator != m_oFields.end())
	{
		poField = (*itFieldIterator).second;

		iResult = M4MDT_DOMGetAttribute(IMDTService::sm_sXMLChName, poField, pcName);
		M4MDT_RETURN_ERROR4(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_DICT, "name", ai_pccField, "logic-field", (m4pcchar_t) m_pcTable, "%s#%s#%s#%s");

		if (strcmpi(pcName, ai_pccField) != 0)
		{
			break ;
		}

		iResult = M4MDT_DOMGetAttribute(pcsFieldPositionName, poField, pcFieldPosition);
		M4MDT_RETURN_ERROR4(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_DICT, "position-name", ai_pccField, "logic-field", (m4pcchar_t) m_pcTable, "%s#%s#%s#%s");

		iFieldPosition = atoi(pcFieldPosition);

		if (iFieldPosition > 0)
		{
			iResult = M4MDT_DOMGetAttribute(IMDTService::sm_sXMLChAlias, poField, pcAlias);
			M4MDT_RETURN_ERROR4(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_DICT, "real_object", ai_pccField, "logic-field", (m4pcchar_t) m_pcTable, "%s#%s#%s#%s");

			iResult = m_oTables.FindChild((m4pchar_t) pcAlias, poTable);
			M4MDT_RETURN_ERROR3(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ELEMENT_IN_DICT, pcAlias, "real-object", (m4pcchar_t) m_pcTable, "%s#%s#%s");

			iResult = M4MDT_DOMGetAttribute(IMDTService::sm_sXMLChTablePosition, poTable, pcTablePosition);
			M4MDT_RETURN_ERROR4(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_DICT, "table-position", pcAlias, "real-object", (m4pcchar_t) m_pcTable, "%s#%s#%s#%s");

			iTablePosition = atoi(pcTablePosition);

			iResult = m_poRows[iTablePosition - 1].GetValue(iFieldPosition - 1, ao_rvValue);
			M4MDT_RETURN_ERROR3(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSaxUpdater::_GetValue", "GetValue", iFieldPosition - 1, "%s#%s - Parameter = <%d>");
		}

		itFieldIterator++;
	}

	return M4_SUCCESS;
}


//=================================================================================
//
// Realiza las transformaciones de las funcies de actualización
//
// Pasos
//		Recorre las funciones de actualización
//		Invoca a la función de cada una de ellas con los parámetros
//			correspondientes
//		Después de invocar a cada una recorre los argumentos y si alguno es
//			campo lo actualiza
//
//=================================================================================

m4return_t	ClMDTSaxUpdater::_DoTransformations( void )
{

	m4return_t			iResult = M4_ERROR ;
	size_t				i = 0 ;
	size_t				iFunctions = 0 ;
	m4uint16_t			j = 0 ;
	m4uint16_t			iParameters = 0 ;
	m4pcchar_t			pccField = NULL ;
	ClMDTUpdateFunction	*poFunction = NULL ;
	ClMDTValue			*poValues = NULL ;


	iFunctions = m_vUpdateFunctions.size() ;

	for( i = 0 ; i < iFunctions ; i++ )
	{
		poFunction = m_vUpdateFunctions[ i ] ;
		poValues = m_vUpdateValues[ i ] ;

		iResult = poFunction->Execute( poValues, this ) ;
		M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSaxUpdater::_DoTransformations", "m_aoUpdateFunctions[ i ].Execute", i,  "%s#%s#%d" ) ;

		iParameters = poFunction->GetParameterNumber() ;

		for( j = 0 ; j < iParameters ; j++ )
		{
			pccField = poFunction->GetParameter( j ) ;

			if( *pccField == '@' )
			{
				iResult = _SetValue( pccField + 1, poValues[ j ] ) ;
				M4MDT_RETURN_ERROR4( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSaxUpdater::_DoTransformations", "_SetValue", i, j, "%s#%s#%d#%d" ) ;
			}
		}
	}

	return( M4_SUCCESS ) ;
}


	
//=================================================================================
//
// Ejecuta la sentencia en la base de datos
//
// Pasos
//		Se itera por todas las tablas físicas
//		Se obtiene la sentencia 
//		Se ejecuta la sentencia
//
//=================================================================================

m4return_t	ClMDTSaxUpdater::_Execute( void )
{

	m4return_t		iResult = M4_ERROR ;
	m4uint32_t		iTable = 0 ;
	int				iTablePosition = 0 ;
	ClCharHolder	pcSentence ;
	ClCharHolder	pcTablePosition ;
	const XMLCh		*pcsSentenceName = NULL ;
	M4ResultSet		oResultSet ;
	DOMNode			*poTable = NULL ;

	ClMDTDomTraverser::iterator	itTableIterator ;


	pcsSentenceName = _GetSentenceName() ;


	iTable = 0 ;
	itTableIterator = m_oTables.begin() ;

	while( itTableIterator != m_oTables.end() )
	{
		poTable = (*itTableIterator).second ;

		iResult = M4MDT_DOMGetAttribute( pcsSentenceName, poTable, pcSentence ) ;
		M4MDT_RETURN_ERROR4( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_DICT, "sentence-name", iTable, "real-object", (m4pcchar_t) m_pcTable, "%s#%d#%s#%s" ) ;

		iResult = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChTablePosition, poTable, pcTablePosition ) ;
		M4MDT_RETURN_ERROR4( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_NO_SUCH_ATTRIBUTE_IN_DICT, "table-position", iTable, "real-object", (m4pcchar_t) m_pcTable, "%s#%d#%s#%s" ) ;

		iTablePosition = atoi( pcTablePosition ) ;

		iResult = m_pIODBC->ExecuteQuery( m_sConnection, (m4pchar_t) pcSentence, &m_poRows[ iTablePosition - 1 ], M4_FALSE, oResultSet ) ;
		M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTSaxUpdater::_Execute", "ExecuteQuery", "%s#%s" ) ;

		iTable++ ;
		itTableIterator++ ;
	}


	return( M4_SUCCESS ) ;
}



//=================================================================================
//
// Libera la memoria asociada a un registro
// Libera los parámetros de la sentencia y los valores de las funciones de
// actualización
//
//=================================================================================

m4return_t	ClMDTSaxUpdater::_EndRegister( void )
{

	ValuesVector_t::iterator	iValueIterator ;


	if( m_poRows != NULL )
	{
		delete [] m_poRows ;
		m_poRows = NULL ;
	}

	iValueIterator = m_vUpdateValues.begin() ;

	while( iValueIterator != m_vUpdateValues.end() )
	{
		delete [] *iValueIterator ;
		iValueIterator++ ;
	}

	m_vUpdateValues.clear() ;

	return( M4_SUCCESS ) ;
}



//=================================================================================
//
// Libera la memoria asociada a una tabla
// Resetea el diccionario
// Libera la definición de la funciones de actualización
//
//=================================================================================

m4return_t	ClMDTSaxUpdater::_EndTable( void )
{

	FunctionVector_t::iterator	iFunctionIterator ;

	
	m_oFields.clear() ;
	m_oTables.clear() ;

	iFunctionIterator = m_vUpdateFunctions.begin() ;

	while( iFunctionIterator != m_vUpdateFunctions.end() )
	{
		delete *iFunctionIterator ;
		iFunctionIterator++ ;
	}

	m_vUpdateFunctions.clear() ;


	return( M4_SUCCESS ) ;
}







//=================================================================================
//
// ClMDTUpdateFunction
//
//=================================================================================


//=================================================================================
//
// Constructor
//
// Inicializa los miembros a los valores por defecto
//
//=================================================================================

ClMDTUpdateFunction::ClMDTUpdateFunction( void )
{
	m_pfFunction = NULL ;
	m_asParameters = NULL ;
	m_iParameterNumber = 0 ;
}



//=================================================================================
//
// Destructor
//
// Libera los recursos si no están ya liberados
//
//=================================================================================

ClMDTUpdateFunction::~ClMDTUpdateFunction( void )
{
	if( m_asParameters != NULL )
	{
		delete [] m_asParameters ;
	}
}



//=================================================================================
//
// Inicializa la definición de la función
//
//
//=================================================================================

m4return_t	ClMDTUpdateFunction::Init( M4MDT_UpdateFunction_t ai_pfFunction, const m4char_t ai_accArguments[][ M4MDT_MAX_UPDATE_FUNCTION_ARGUMENT_SIZE ], const m4char_t ai_accNames[][ M4MDT_MAX_UPDATE_FUNCTION_ARGUMENT_SIZE ], const m4char_t ai_accValues[][ M4MDT_MAX_UPDATE_FUNCTION_ARGUMENT_SIZE ], m4uint16_t ai_iParameterNumber, m4pcchar_t ai_pccName, m4pcchar_t ai_pccTable  )
{

	m4int16_t	iParameter = -1 ;
	m4uint16_t	i = 0 ;


	m_pfFunction = ai_pfFunction ;
	m_iParameterNumber = ai_iParameterNumber ;

	m_asParameters = new string[ m_iParameterNumber ] ;
	M4MDT_RETURN_ERROR1( m_asParameters == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof( string ) * m_iParameterNumber, "%d" ) ;


	for( i = 0 ; i < m_iParameterNumber ; i++ )
	{
		iParameter = M4MDTFindParameterByName( ai_accArguments[ i ], ai_accNames, m_iParameterNumber ) ;
		M4MDT_RETURN_ERROR3( iParameter == -1, M4_ERROR, M4MDTERROR_MISSING_UPDATE_ARGUMENT, ai_accArguments[ i ], ai_pccName, ai_pccTable, "%s#%s#%s" ) ;
		m_asParameters[ i ] = ai_accValues[ iParameter ] ;
	}


	return( M4_SUCCESS ) ;
}




//=================================================================================
// Otras funciones
//=================================================================================

//=================================================================================
//
// Busca un parámetro entre los argumentos de la función
//
// Pasos
//		Itera por al array de parámetros
//		Se busca si es un campo y comienza por arroba. Entonces se compara
//			quitando la arroba
//		Si coincide con el pedido se pone la variable de salida a M4_TRUE y se
//			retorna la	posición
//		Sino se encuentra devuelve -1
//
//=================================================================================

m4int16_t	ClMDTUpdateFunction::FindParameterByValue( m4pcchar_t ai_pccValue )
{

	m4int16_t	iResult = -1 ;
	m4bool_t	bFound = M4_FALSE ;
	m4uint16_t	i = 0 ;
	m4pcchar_t	pccValue = NULL ;


	iResult = -1 ;
	bFound = M4_FALSE ;

	for( i = 0 ; i < m_iParameterNumber && bFound == M4_FALSE ; i++ )
	{
		pccValue = m_asParameters[ i ].c_str() ;

		if( *pccValue == '@' )
		{
			if( strcmpi( ai_pccValue, pccValue + 1 ) == 0 )
			{
				bFound = M4_TRUE ;
				iResult = i ;
			}
		}
	}

	return( iResult ) ;
}



//=================================================================================
//
// Obtiene el número de parámetros de la función
//
//=================================================================================

m4int16_t	ClMDTUpdateFunction::GetParameterNumber( void ) const
{
	return( m_iParameterNumber ) ;
}



//=================================================================================
//
// Obtiene un parámetros de la función por posición
//
// Pasos
//		Se chequea que el parámetro éxista
//		Se devuelve el valor
//
//=================================================================================

m4pcchar_t	ClMDTUpdateFunction::GetParameter( m4int16_t ai_iParameter ) const
{
	M4MDT_RETURN_ERROR2( ai_iParameter >= m_iParameterNumber, NULL, M4MDTERROR_INTERNAL_ERROR, "ClMDTUpdateFunction::GetParameter", ai_iParameter,  "%s#%d" ) ;

	return( m_asParameters[ ai_iParameter ].c_str() ) ;
}


	
//=================================================================================
//
// Ejecuta la función de actualización
//
// Pasos
//		Chequea que la función de actualziación no sea nula
//		Ejecuta la función de actualización
//
//=================================================================================

m4return_t	ClMDTUpdateFunction::Execute( ClMDTValue *ai_acvValues, ClMDTSaxUpdater *ao_poUpdater ) const
{

	m4return_t	iResult = M4_ERROR ;


	M4MDT_RETURN_ERROR2( m_pfFunction == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTUpdateFunction::Execute", "m_pfFunction == NULL", "%s#%s" ) ;
	
	iResult = (ao_poUpdater->*m_pfFunction)( ai_acvValues ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTUpdateFunction::Execute", "m_pfFunction", "%s#%s" ) ;

	return( M4_SUCCESS ) ;
}




