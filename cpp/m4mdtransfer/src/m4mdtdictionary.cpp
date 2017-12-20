
//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdtransfer
// File:                m4mdtdictionary.cpp
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                12-04-2004
// Language:            C++
// Operating System:    WINDOWS, UNIX
// Design Document:     
//
//
// Definition:
//
//    Este módulo contiene las funciones del diccionario de la librería
//
//
//==============================================================================


#include "m4mdtdictionary.hpp"

#include <stdlib.h>
#include <errno.h>

#include "m4mdtinterfaces.hpp"
#include "m4mdtres.hpp"
#include "m4mdtutils.hpp"
#include "cldefine.hpp"
#include "m4mdtodbc.hpp"
#include "xmlstreamgenerator.hpp"
#include "m4unicode.hpp"


#include "xercesc/framework/LocalFileInputSource.hpp"



//=================================================================================
// Variables globales
//=================================================================================

static	const string		s_csObjectSelect		= "SELECT A.ID_OBJECT, B.XML FROM M4RDC_RAMDL_OBJECTS A, M4RDC_RAMDL_OBJEC1 B WHERE A.ID_OBJECT = ? AND A.VER_LOWEST <= ? AND A.VER_HIGHEST >= ? AND B.ID_OBJECT = A.ID_OBJECT AND B.VER_LOWEST = A.VER_LOWEST" ;

static	const string		s_csTableSelect			= "SELECT ID_TRANS_OBJESP, ID_TRANS_OBJENG FROM M4RDC_LOGIC_OBJECT WHERE ID_OBJECT = ?" ;
static	const string		s_csFieldSelect			= "SELECT F.ID_FIELD, F.ID_TRANS_FLDESP, F.ID_TRANS_FLDENG, RF.ID_REAL_FIELD, RO.ID_REAL_OBJECT, RO.IS_PRINCIPAL, T.ID_M4_TYPE, T.PREC, T.SCALE, F.POSITION, F.POS_PK, F.ID_INTERNAL_FIELD FROM M4RDC_FIELDS F, M4RDC_REAL_FIELDS RF, M4RDC_REAL_OBJECTS RO, M4RDC_EXTENDED_TPS T WHERE F.ID_OBJECT = ? AND F.DTE_CLOSED = {ts '4000-01-01 00:00:00'} AND RF.ID_OBJECT = F.ID_OBJECT AND RF.ID_FIELD = F.ID_FIELD AND RF.DTE_CLOSED = {ts '4000-01-01 00:00:00'} AND RO.ID_OBJECT = RF.ID_OBJECT AND RO.ID_REAL_OBJECT = RF.ID_REAL_OBJECT AND RO.DTE_CLOSED = {ts '4000-01-01 00:00:00'} AND T.ID_TYPE = F.ID_TYPE AND T.DTE_CLOSED = {ts '4000-01-01 00:00:00'}" ;
static	const string		s_csLookUpSelect		= "SELECT R.ID_FIELD, FU.ID_LU_OBJECT, FU.ID_LU_FIELD, RO.ID_REAL_OBJECT, RF.ID_REAL_FIELD FROM M4RDC_RLTION_FLDS R, M4RDC_FIELDS F, M4RDC_FIELDS_LU FU, M4RDC_REAL_OBJECTS RO, M4RDC_REAL_FIELDS RF WHERE R.ID_OBJECT = ? AND R.DTE_CLOSED = {ts '4000-01-01 00:00:00'} AND F.ID_OBJECT = R.ID_PARENT_OBJECT AND F.ID_FIELD = R.ID_PARENT_FIELD AND FU.ID_OBJECT = F.ID_OBJECT AND FU.ID_FIELD = F.ID_FIELD AND RO.ID_OBJECT = FU.ID_LU_OBJECT AND RO.IS_PRINCIPAL = '1' AND RF.ID_OBJECT = RO.ID_OBJECT AND RF.ID_REAL_OBJECT = RO.ID_REAL_OBJECT AND RF.ID_FIELD = FU.ID_FIELD" ;
 
static	const string		s_csTransFieldSelect	= "SELECT RF.ID_REAL_FIELD FROM M4RDC_FIELDS_TRANS T, M4RDC_REAL_FIELDS RF WHERE T.ID_OBJECT = ? AND T.ID_FIELD = ? AND T.ID_LANGUAGE = ? AND RF.ID_OBJECT = T.ID_OBJECT AND RF.ID_FIELD = T.ID_FIELD_TRANS" ;
static	const string		s_csRealFieldSelect		= "SELECT RF.ID_REAL_FIELD FROM M4RDC_REAL_FIELDS RF WHERE RF.ID_OBJECT = ? AND RF.ID_FIELD = ?" ;

// Bug 0127575
static	const string		s_csFieldConfigSelect	= "SELECT B.ID_OBJECT, B.ID_FIELD, B.IS_TRANSFERABLE FROM M4RDC_CONFIG A, M4RDC_FIELD_CONFIG B WHERE A.IS_ENABLE = 0 AND A.ID_CONFIGURATION = B.ID_CONFIGURATION ORDER BY B.ID_CONFIGURATION, B.ID_OBJECT, B.ID_FIELD";
static	const string		s_csObjectConfigSelect	= "SELECT B.ID_OBJECT, B.IS_TRANSFERABLE FROM M4RDC_CONFIG A, M4RDC_OBJ_CONFIG B WHERE A.IS_ENABLE = 0 AND A.ID_CONFIGURATION = B.ID_CONFIGURATION";
				
static	const string		s_csConfigurationIdsSelect		= "SELECT A.ID_CONFIGURATION FROM M4RDC_CONFIG A WHERE A.IS_ENABLE = 1";

static	const m4uint32_t	s_ciObjects_ObjectPos			= 0 ;
static	const m4uint32_t	s_ciObjects_XMLPos				= 1 ;

static	const m4uint32_t	s_ciTables_EspPos				= 0 ;
static	const m4uint32_t	s_ciTables_EngPos				= 1 ;

static	const m4uint32_t	s_ciFields_FieldPos				= 0 ;
static	const m4uint32_t	s_ciFields_EspPos				= 1 ;
static	const m4uint32_t	s_ciFields_EngPos				= 2 ;
static	const m4uint32_t	s_ciFields_RealFieldPos			= 3 ;
static	const m4uint32_t	s_ciFields_RealObjPos			= 4 ;
static	const m4uint32_t	s_ciFields_IsPrincipalPos		= 5 ;
static	const m4uint32_t	s_ciFields_M4TypePos			= 6 ;
static	const m4uint32_t	s_ciFields_PrecisionPos			= 7 ;
static	const m4uint32_t	s_ciFields_ScalePos				= 8 ;
static	const m4uint32_t	s_ciFields_PositionPos			= 9 ;
static	const m4uint32_t	s_ciFields_PosPkPos				= 10 ;
static	const m4uint32_t	s_ciFields_InternalFieldPos		= 11 ;

static	const m4uint32_t	s_ciLookUps_FieldPos			= 0 ;
static	const m4uint32_t	s_ciLookUps_LuObjectPos			= 1 ;
static	const m4uint32_t	s_ciLookUps_LuFieldPos			= 2 ;
static	const m4uint32_t	s_ciLookUps_RealObjPos			= 3 ;
static	const m4uint32_t	s_ciLookUps_RealFieldPos		= 4 ;

static	const m4uint32_t	s_ciTransFields_RealFieldPos	= 0 ;

static	const m4uint32_t	s_ciRealFields_RealFieldPos		= 0 ;

static	const m4uint32_t	s_ciIdObject_ObjectConfigPos		= 0;
static	const m4uint32_t	s_ciIsTransferable_ObjectConfigPos	= 1;

static	const m4uint32_t	s_ciIdObject_FieldConfigPos			= 0;
static	const m4uint32_t	s_ciIdField_FieldConfigPos			= 1;
static	const m4uint32_t	s_ciIsTransferable_FieldConfigPos	= 2;

static	const m4uint32_t	s_ciIdConfig_ConfigPos				= 0;


//=================================================================================
// Funciones globales
//=================================================================================

//=================================================================================
//
// Compara 2 registros de campos de una tabla lógica del diccionario
//
// @param ai_pcvRow1
//
//		Puntero al primer registro
//
// @param ai_pcvRow1
//
//		Puntero al segundo registro
//
// @return
//
//		< 0 si el primer registro va antes que el segundo
//		> 0 si el primer registro va después que el segundo
//		= 0 si los dos registros son iguales
//
// Pasos
//		Primero se ordena por las tablas principales
//		Después se ordena por identificador de tabla física
//		Dentro de una tabla física se ordena por pk. Los campos que son pk van
//			antes de los que no son pk
//		Dentro de los que son pk se ordena por posición de la pk. Los campos
//			con menor número de pk van antes
//		Dentro de los que no son pk se ordena por campos long. Los campos que
//			son long van después de los que no lo son
//		Dentro de los campos que no son pk y no son campos long se ordena por
//			posición del campo
//		Dentro de los campos que no son pk y son campos long también se ordena por
//			posición del campo
//
//=================================================================================

int	M4MDTCompareFieldsByTableAndType( m4pcvoid_t ai_pcvRow1, m4pcvoid_t ai_pcvRow2 )
{

	m4return_t	iResult = M4_ERROR ;
	int			iDifference = 0 ;
	int			iValue1 = -1 ;
	int			iValue2 = -1 ;
	ClMDTValue	vValue1 ;
	ClMDTValue	vValue2 ;
	M4Row		*poRow1 = NULL ;
	M4Row		*poRow2 = NULL ;


	poRow1 = (*( M4SorteableRow** ) ai_pcvRow1)->m_poRow ;
	poRow2 = (*( M4SorteableRow** ) ai_pcvRow2)->m_poRow ;


	iResult = poRow1->GetValue( s_ciFields_IsPrincipalPos, vValue1 ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, 0, M4MDTERROR_INTERNAL_ERROR, "M4MDTCompareFieldsByTableAndType", "GetValue", "IS_PRINCIPAL", "%s#%s - Parameter = <%s>" ) ;

	iResult = poRow2->GetValue( s_ciFields_IsPrincipalPos, vValue2 ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, 0, M4MDTERROR_INTERNAL_ERROR, "M4MDTCompareFieldsByTableAndType", "GetValue", "IS_PRINCIPAL", "%s#%s - Parameter = <%s>" ) ;

	iValue1 = atoi( vValue1.c_str() ) ;
	iValue2 = atoi( vValue2.c_str() ) ;

	iDifference = iValue2 - iValue1 ;

    if( iDifference != 0 )
    {
		return( iDifference ) ;
    }


	iResult = poRow1->GetValue( s_ciFields_RealObjPos, vValue1 ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, 0, M4MDTERROR_INTERNAL_ERROR, "M4MDTCompareFieldsByTableAndType", "GetValue", "ID_REAL_OBJECT", "%s#%s - Parameter = <%s>" ) ;

	iResult = poRow2->GetValue( s_ciFields_RealObjPos, vValue2 ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, 0, M4MDTERROR_INTERNAL_ERROR, "M4MDTCompareFieldsByTableAndType", "GetValue", "ID_REAL_OBJECT", "%s#%s - Parameter = <%s>" ) ;

	iDifference = strcmpi( vValue1.c_str(), vValue2.c_str() ) ;

    if( iDifference != 0 )
    {
		return( iDifference ) ;
    }


	iResult = poRow1->GetValue( s_ciFields_PosPkPos, vValue1 ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, 0, M4MDTERROR_INTERNAL_ERROR, "M4MDTCompareFieldsByTableAndType", "GetValue", "POS_PK", "%s#%s - Parameter = <%s>" ) ;

	iResult = poRow2->GetValue( s_ciFields_PosPkPos, vValue2 ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, 0, M4MDTERROR_INTERNAL_ERROR, "M4MDTCompareFieldsByTableAndType", "GetValue", "POS_PK", "%s#%s - Parameter = <%s>" ) ;

	iValue1 = atoi( vValue1.c_str() ) ;
	iValue2 = atoi( vValue2.c_str() ) ;

	if( iValue1 != 0 )
	{
		if( iValue2 != 0 )
		{
			return( iValue1 - iValue2 ) ;
		}

		return( -1 ) ;
	}
	else
	{
		if( iValue2 != 0 )
		{
			return( 1 ) ;
		}
	}


	iResult = poRow1->GetValue( s_ciFields_M4TypePos, vValue1 ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, 0, M4MDTERROR_INTERNAL_ERROR, "M4MDTCompareFieldsByTableAndType", "GetValue", "ID_M4_TYPE", "%s#%s - Parameter = <%s>" ) ;

	iResult = poRow2->GetValue( s_ciFields_M4TypePos, vValue2 ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, 0, M4MDTERROR_INTERNAL_ERROR, "M4MDTCompareFieldsByTableAndType", "GetValue", "ID_M4_TYPE", "%s#%s - Parameter = <%s>" ) ;

	iValue1 = atoi( vValue1.c_str() ) ;
	iValue2 = atoi( vValue2.c_str() ) ;

	if( iValue1 != M4CL_M4_TYPE_LONG && iValue1 != M4CL_M4_TYPE_UNI_LONG && iValue1 != M4CL_M4_TYPE_BINARY_STRING && iValue1 != M4CL_M4_TYPE_BLOB )
	{
		if( iValue2 == M4CL_M4_TYPE_LONG || iValue2 == M4CL_M4_TYPE_UNI_LONG  || iValue2 == M4CL_M4_TYPE_BINARY_STRING || iValue2 == M4CL_M4_TYPE_BLOB )
		{
			return( -1 ) ;
		}
	}
	else
	{
		if( iValue2 != M4CL_M4_TYPE_LONG && iValue2 != M4CL_M4_TYPE_UNI_LONG && iValue2 != M4CL_M4_TYPE_BINARY_STRING && iValue2 != M4CL_M4_TYPE_BLOB )
		{
			return( 1 ) ;
		}
	}


	iResult = poRow1->GetValue( s_ciFields_PositionPos, vValue1 ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, 0, M4MDTERROR_INTERNAL_ERROR, "M4MDTCompareFieldsByTableAndType", "GetValue", "ID_FIELD", "%s#%s - Parameter = <%s>" ) ;

	iResult = poRow2->GetValue( s_ciFields_PositionPos, vValue2 ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, 0, M4MDTERROR_INTERNAL_ERROR, "M4MDTCompareFieldsByTableAndType", "GetValue", "ID_FIELD", "%s#%s - Parameter = <%s>" ) ;

	iValue1 = atoi( vValue1.c_str() ) ;
	iValue2 = atoi( vValue2.c_str() ) ;

	iDifference = iValue1 - iValue2 ;


	return( iDifference ) ;
}


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

ClMDTDictionary::ClMDTDictionary( void )
{
	m_iLanguage = M4MDT_LANG_ENGLISH ;
	m_pIODBC = NULL ;
	m_oExclusionObjectMap = NULL;
	m_oExclusionObjectFieldMap = NULL;
}



//=================================================================================
//
// Destructor
//
//=================================================================================

ClMDTDictionary::~ClMDTDictionary( void )
{

	m4pchar_t		pcKey = NULL ;
	ClMDTCacheDOM	* poCacheDOM = NULL ;
	exclusionData_t	* poFieldMap = NULL;

	M4MDTCacheDomMap_t::iterator		itCache ;
	exclusionObjectField_t::iterator	itExclusion ;


	itCache = m_oCacheDomMap.begin() ;

	while( itCache != m_oCacheDomMap.end() )
	{
		pcKey = (*itCache).first ;
		poCacheDOM = (*itCache).second ;

		itCache++ ;

		if( pcKey != NULL )
		{
			delete [] pcKey ;
		}

		if( poCacheDOM != NULL )
		{
			delete( poCacheDOM ) ;
		}
	}

	m_oCacheDomMap.clear() ;

	// Borrar campos y objetos lógicos excluidos.

	/* Bug 0127755
	Hay que chequear que los objetos no sean nulos
	*/
	if( m_oExclusionObjectFieldMap != NULL )
	{
		itExclusion = m_oExclusionObjectFieldMap->begin();
		while (itExclusion != m_oExclusionObjectFieldMap->end())
		{
			poFieldMap = (*itExclusion).second;
			itExclusion++;

			if (poFieldMap != NULL)
			{
				delete poFieldMap;
			}
		}

		m_oExclusionObjectFieldMap->clear();
		delete m_oExclusionObjectFieldMap;
		m_oExclusionObjectFieldMap = NULL;
	}

	if( m_oExclusionObjectMap != NULL )
	{
		m_oExclusionObjectMap->clear();
		delete m_oExclusionObjectMap;
		m_oExclusionObjectMap = NULL;
	}
}


//=================================================================================
// Inicializa el objeto
//
// Pasos
//		Se comprueba que los parámetros de entrada no sean nulos
//		Se asignan los valores de los argumentos a los miembros. Los que son cadena
//			se copian, de modo que los argumentos se pueden borrar una vez llamada
//			a la función.
//
//=================================================================================

m4return_t ClMDTDictionary::Init( m4uint32_t ai_iLanguage, m4pcchar_t ai_pccConnection, m4pcchar_t ai_pccCacheDir, m4uint32_t ai_iVersion, M4IODBC *ai_pIODBC )
{

	m4char_t	acVersion[ M4MDT_VERSION_MAX_SIZE + 1 ] ;


	M4MDT_RETURN_ERROR2( ai_pccConnection == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pccConnection", "ClMDTDictionary::Init", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ai_pccCacheDir == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pccCacheDir", "ClMDTDictionary::Init", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ai_pIODBC == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pIODBC", "ClMDTDictionary::Init", "%s#%s" ) ;

	m_iLanguage = ai_iLanguage ;
	m_sConnection = ai_pccConnection ;
	m_sCacheDir = ai_pccCacheDir ;

	sprintf( acVersion, "%d", ai_iVersion ) ;
	m_vVersion.Set( acVersion ) ;

	m_pIODBC = ai_pIODBC ;


	return( M4_SUCCESS ) ;
}




//=================================================================================
// Funciones del interfaz
//=================================================================================

//=================================================================================
// Obtiene el XML de un objeto traspasable del diccionario de RAMDL
// El nombre del fichero se compone del prefijo "MDTOBJECT_" y del sufijo lenguaje
//
// Pasos
//		Se inicializa el parámetro de salida a NULL
//		Se comprueba que los parámetros de entrada no sean nulos
//		Se comprueba si el fichero del objeto ya está en disco
//		Si no está en disco se crea el fichero, se llama a la función que
//			lo rellena y se cierra el fichero
//		Se crea un InputSource a partir del fichero
//		Sólo se captura la excepción al crear el InputSource porque hay que
//			liberar variables
//
//=================================================================================

m4return_t	ClMDTDictionary::GetXMLObject( m4pcchar_t ai_pccObject, InputSource* &ao_rpoSource )
{

	m4return_t	iResult = M4_SUCCESS ;
	m4pchar_t	pcException = NULL ;
	m4char_t	acFile[ M4MDT_MAX_FILE_NAME_SIZE + 1 ] ;
	XMLCh		*wsFile = NULL ;
	FILE		*pfFile = NULL ;


	ao_rpoSource = NULL ;

	M4MDT_RETURN_ERROR2( ai_pccObject == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pccObject", "ClMDTDictionary::GetXMLObject", "%s#%s" ) ;

	sprintf( acFile, "%s%sMDTOBJECT_%s.xml", m_sCacheDir.c_str(), M4_BACK_SLASH, ai_pccObject ) ;

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniANSI ;
	pfFile = M4Fopen( acFile, M4UniRead, eEncoding ) ;

	if( pfFile == NULL )
	{
		// UNICODE FILE
		eEncoding = M4UniANSI ;
		pfFile = M4Fopen( acFile, M4UniWriteBinary, eEncoding ) ;
		M4MDT_RETURN_ERROR2( pfFile == NULL, M4_ERROR, M4MDTERROR_NO_OPEN_FILE, acFile, strerror( errno ), "%s#%s" ) ;

		iResult = _GetObjectFromDatabase( ai_pccObject, pfFile, acFile ) ;
		fclose( pfFile ) ;
		pfFile = NULL ;
		M4MDT_CLEAN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, remove( acFile ), "ClMDTDictionary::GetXMLObject", "_GetObjectFromDatabase", "%s#%s" ) ;
	}
	else
	{
		fclose( pfFile ) ;
		pfFile = NULL ;
	}

	// UNICODE XML
	wsFile = (XMLCh*)M4CppToXML( acFile ) ;

    try
    {
		ao_rpoSource = new LocalFileInputSource( wsFile ) ;
		M4MDT_CHECK_ERROR1( ao_rpoSource == NULL, M4MDTERROR_NO_MEMORY, sizeof( LocalFileInputSource ), "%d" ) ;
    }
    catch( const XMLException &e )
    {
		// UNICODE XML
		pcException = M4XMLToCpp( (M4XMLCh*)e.getMessage() ) ;
		M4MDT_DUMP_ERROR1( M4MDTERROR_EXCEPTION, pcException, "%s" ) ;
		delete [] pcException ;

		ao_rpoSource = NULL ;
    }

	delete [] wsFile ;

	if( ao_rpoSource == NULL )
	{
		return( M4_ERROR ) ;
	}


	return( M4_SUCCESS ) ;
}



//=================================================================================
// Obtiene el XML de una tabla del diccionario de Meta4
// El nombre del fichero se compone del prefijo "MDTTABLE_" y del sufijo lenguaje
//
// Pasos
//		Se inicializa el parámetro de salida a NULL
//		Se comprueba que los parámetros de entrada no sean nulos
//		Se comprueba si el fichero de la tabla ya está en disco
//		Si no está en disco se crea el fichero, se llama a la función que
//			lo rellena y se cierra el fichero
//		Se crea un InputSource a partir del fichero
//		Sólo se captura la excepción al crear el InputSource porque hay que
//			liberar variables
//
//=================================================================================

m4return_t	ClMDTDictionary::GetXMLTable( m4pcchar_t ai_pccTable, InputSource* &ao_rpoSource )
{

	m4return_t	iResult = M4_SUCCESS ;
	m4pchar_t	pcException = NULL ;
	m4char_t	acFile[ M4MDT_MAX_FILE_NAME_SIZE + 1 ] ;
	XMLCh		*wsFile = NULL ;
	FILE		*pfFile = NULL ;


	ao_rpoSource = NULL ;

	M4MDT_RETURN_ERROR2( ai_pccTable == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pccTable", "ClMDTDictionary::GetXMLTable", "%s#%s" ) ;

	sprintf( acFile, "%s%sMDTTABLE_%s_%.0d.xml", m_sCacheDir.c_str(), M4_BACK_SLASH, ai_pccTable, m_iLanguage ) ;

	eUniFileEncodingType_t eEncoding = M4UniANSI ;
	pfFile = M4Fopen( acFile, M4UniRead, eEncoding ) ;

	if( pfFile == NULL )
	{
		// UNICODE FILE
		eEncoding = M4UniANSI ;
		pfFile = M4Fopen( acFile, M4UniWriteBinary, eEncoding ) ;
		M4MDT_RETURN_ERROR2( pfFile == NULL, M4_ERROR, M4MDTERROR_NO_OPEN_FILE, acFile, strerror( errno ), "%s#%s" ) ;

		iResult = _GetTableFromDatabase( ai_pccTable, pfFile, acFile ) ;
		fclose( pfFile ) ;
		pfFile = NULL ;
		M4MDT_CLEAN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, remove( acFile ), "ClMDTDictionary::GetTable", "_GetTableFromDatabase", "%s#%s" ) ;
	}
	else
	{
		fclose( pfFile ) ;
		pfFile = NULL ;
	}


	// UNICODE XML
	wsFile = (XMLCh*)M4CppToXML( acFile ) ;

    try
    {
		ao_rpoSource = new LocalFileInputSource( wsFile ) ;
		M4MDT_CHECK_ERROR1( ao_rpoSource == NULL, M4MDTERROR_NO_MEMORY, sizeof( LocalFileInputSource ), "%d" ) ;
    }
    catch( const XMLException &e )
    {
		// UNICODE XML
		pcException = M4XMLToCpp( (M4XMLCh*)e.getMessage() ) ;
		M4MDT_DUMP_ERROR1( M4MDTERROR_EXCEPTION, pcException, "%s" ) ;
		delete [] pcException ;

		ao_rpoSource = NULL ;
    }

	delete [] wsFile ;

	if( ao_rpoSource == NULL )
	{
		return( M4_ERROR ) ;
	}


	return( M4_SUCCESS ) ;
}



//=================================================================================
// Libera la fuente de lectura asociada a un objeto, tanto del diccionario
// de RAMDL como del de Meta4
//
// Pasos
//		Se comprueba que los parámetros de entrada no sean nulos
//		Se libera el objeto
//		Se pone a NULL la variable de salida
//
//=================================================================================

m4return_t	ClMDTDictionary::ReleaseXMLObject( InputSource* &aio_rpoSource )
{
	M4MDT_RETURN_ERROR2( aio_rpoSource == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "aio_rpoSource", "ClMDTDictionary::ReleaseXMLObject", "%s#%s" ) ;

	delete( aio_rpoSource ) ;
	aio_rpoSource = NULL ;

	return( M4_SUCCESS ) ;
}



//=================================================================================
// Obtiene el DOM de una tabla del diccionario de Meta4
// Los parámetros están especificados en el interfaz
//
// Pasos
//		Se inicializa el parámetro de salida a NULL
//		Se comprueba que los parámetros de entrada no sean nulos
//		Se busca el DOM de la tabla en el mapa
//		Si se encuentra se actualiza la variable de salida con el valor y se
//			actualiza la fecha de último acceso de este objeto
//		Si no se encuentra se llama al mantenimiento de la caché porque hay que
//			añadir un nuevo  objeto
//		Se obtiene el InputSource asociado a la tabla
//		Se parsea el InputSource y se obtiene el documento DOM de la tabla
//		Se libera el InputSource
//		Se crea un nuevo objeto DOM cacheable, se le asigna el documento DOM
//			y se inserta en el mapa
//
//=================================================================================

m4return_t	ClMDTDictionary::GetDOMTable( m4pcchar_t ai_pccTable, xercesc::DOMDocument* &ao_rpoDocument )
{

	m4return_t		iResult = M4_ERROR ;
	size_t			iLength = 0 ;
	m4pchar_t		pcKey = NULL ;
	ClMDTCacheDOM	*poCacheDOM = NULL ;
	InputSource		*poSource = NULL;

	M4MDTCacheDomMap_t::iterator	iterator ;


	ao_rpoDocument = NULL ;

	M4MDT_RETURN_ERROR2( ai_pccTable == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pccTable", "ClMDTDictionary::GetDOMTable", "%s#%s" ) ;


	iterator = m_oCacheDomMap.find( (m4pchar_t) ai_pccTable ) ;

	if( iterator != m_oCacheDomMap.end() )
	{
		poCacheDOM = (*iterator).second ;
		M4MDT_RETURN_ERROR2( poCacheDOM == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::GetDOMTable", "poCacheDOM == NULL", "%s#%s" ) ;

		ao_rpoDocument = poCacheDOM->m_poDocument ;
		poCacheDOM->m_dLastAccessed = ClActualDate() ;

		return( M4_SUCCESS ) ;
	}


	iResult = _PurgeDOMCache() ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::GetDOMTable", "_PurgeCache", "%s#%s" ) ;

	iResult = GetXMLTable( ai_pccTable, poSource ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::GetDOMTable", "GetXMLTable", "%s#%s" ) ;

	poCacheDOM = new ClMDTCacheDOM( poSource ) ;
	M4MDT_RETURN_ERROR1( poCacheDOM == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof( ClMDTCacheDOM ), "%d" ) ;

	ao_rpoDocument = poCacheDOM->m_poDocument ;

	iResult = ReleaseXMLObject( poSource ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::GetDOMTable", "ReleaseXMLObject", "%s#%s" ) ;

	iLength = strlen( ai_pccTable ) + 1 ;
	pcKey = new m4char_t[ iLength ] ;

	if( pcKey == NULL )
	{
		delete( poCacheDOM ) ;
		poCacheDOM = NULL ;
		M4MDT_DUMP_ERROR1( M4MDTERROR_NO_MEMORY, sizeof( m4char_t ) * iLength, "%d" ) ;
		return( M4_ERROR ) ;
	}

	memcpy( pcKey, ai_pccTable, iLength ) ;

	m_oCacheDomMap.insert(M4MDTCacheDomMap_t::value_type(pcKey, poCacheDOM));

	return( M4_SUCCESS ) ;
}



//=================================================================================
//
// Obtiene la configuración para un campo de un objeto lógico determinado.
// Este método recupera la información que el usuario ha especificado en las
// tablas de configuración de objetos y campos lógicos para las configuraciones 
// habilitadas. Estos campos son los campos lógicos que se considerean no comparables
// y traspasables o no traspasables según se indique.
//
// Pasos
//	Cargar los objetos y campos lógicos que se encuentran configurados, y si son
//	traspasables o no.
//	Cargar los objetos lógicos que se encuentran configurados, y si se traspasan o no.
//	El proceso de carga de configuración de objetos y campos lógicos, se realiza 
//	una sola vez
//	Dado el objeto lógico y el campo lógico, buscar primero en el mapa de configuración
//	de campos. Si se encuentra aquí, significa que el campo es no comparable, y 
//  traspasable o no traspasables, según se indique en el repositorio.
//	Si no se encuentra el campo en el mapa de configuración de campos, se busca
//	el objeto lógico en el mapa de configuración de objetos lógicos, si se encuentra 
//	aquí y se especifica que el objeto es traspasable, el campo es traspasable, y 
//	si se indica que el objeto es no traspasable, el campo es no traspasable.
//	Si no encontramos el objeto lógico y/o el campo lógico en nuestros mapas de 
//	configuración, significa que el campo es comparable y traspasable.
//
//=================================================================================
m4return_t	ClMDTDictionary::GetFieldConfiguration(m4pcchar_t ai_pccObjectName, m4pcchar_t ai_pccFieldName, m4bool_t &ao_bIsComparable, m4bool_t &ao_IsTransferable)
{
	m4return_t iResult = M4_ERROR;
	m4bool_t bIsComparable = M4_TRUE;
	m4bool_t bIsTransferable = M4_TRUE;

	// Cargar la configuración de objetos lógicos.
	if (m_oExclusionObjectMap == NULL)
	{
		m_oExclusionObjectMap = new exclusionData_t();
		iResult = _LoadObjectConfiguration();
		M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::GetFieldConfiguration", "_LoadObjectConfiguration", "%s#%s");
	}

	// Cargar la configuración de campos lógicos.
	if (m_oExclusionObjectFieldMap == NULL)
	{
		m_oExclusionObjectFieldMap = new exclusionObjectField_t();
		iResult = _LoadFieldConfiguration();
		M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::GetFieldConfiguration", "_LoadFieldConfiguration", "%s#%s");
	}

	// Buscar si el campo está dentro de los campos que se han configurado 
	// como no comparables.
	iResult = _GetFieldConfiguration(ai_pccObjectName, ai_pccFieldName, ao_bIsComparable, ao_IsTransferable);
	M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::GetFieldConfiguration", "_GetFieldConfiguration", "%s#%s");

	return M4_SUCCESS;
}



//=================================================================================
//
// Obtiene los identificadores de las configuraciones que están activas.
//
//=================================================================================
m4return_t	ClMDTDictionary::GetEnableConfigurationIds(vector<string> &ao_vConfigId)
{
	m4return_t iResult = M4_ERROR;

	// Obtener los identificadores de las configuraciones que están activas.
	iResult = _GetEnableConfigurationIds(ao_vConfigId);
	M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::GetEnableConfigurationIds", "_GetEnableConfigurationIds", "%s#%s");

	return M4_SUCCESS;
}


//=================================================================================
// Funciones auxiliares
//=================================================================================

//=================================================================================
// Realiza las tareas de mantenimiento de la caché en RAM de documentos DOM
// Deja espacio suficiente para añadir un nuevo elemento
//
// Pasos
//		Si el número de documentos en la caché ha alcanzado su número límite
//		elimina el que hace más tiempo que no se usa
//		Para ello itera por el mapa quedandose con una referencia al objeto que
//		hace más tiempo que se accedió
//		Para borrar el elemnto se obtiene su key y su contenido y se elimina del
//		mapa. A continuación se libera la memoria de la key y el contenido
//
//=================================================================================

m4return_t	ClMDTDictionary::_PurgeDOMCache( void )
{

	m4return_t		iResult = M4_ERROR ;
	size_t			iSize = 0 ;
	m4date_t		dLastAccessed = M4CL_MINUS_INFINITE_IN_JULIAN ;
	m4date_t		dMinLastAccessed = M4CL_MINUS_INFINITE_IN_JULIAN ;
	m4pchar_t		pcKey = NULL ;
	ClMDTCacheDOM	*poCacheDOM = NULL ;

	M4MDTCacheDomMap_t::iterator	iterator ;
	M4MDTCacheDomMap_t::iterator	itDelete ;


	iSize = m_oCacheDomMap.size() ;

	if( iSize < M4MDT_MAX_CACHE_DOMS )
	{
		return( M4_SUCCESS ) ;
	}

	dMinLastAccessed = M4CL_PLUS_INFINITE_IN_JULIAN ;
	itDelete = m_oCacheDomMap.begin() ;
	iterator = m_oCacheDomMap.begin() ;

	while( iterator != m_oCacheDomMap.end() )
	{
		dLastAccessed = (*iterator).second->m_dLastAccessed ;

		if( dLastAccessed < dMinLastAccessed )
		{
			dMinLastAccessed = dLastAccessed ;
			itDelete = iterator ;
		}

		iterator++ ;
	}

	if( itDelete != m_oCacheDomMap.end() )
	{
		pcKey = (*itDelete).first ;
		poCacheDOM = (*itDelete).second ;

		m_oCacheDomMap.erase( itDelete ) ;

		if( pcKey != NULL )
		{
			delete [] pcKey ;
			pcKey = NULL ;
		}

		if( poCacheDOM != NULL )
		{
			delete( poCacheDOM ) ;
			poCacheDOM = NULL ;
		}
	}


	return( M4_SUCCESS ) ;
}





//=================================================================================
// Genera el fichero XML de un objeto traspasable del diccionario de RAMDL
// a partir de su definición de base de datos
//
// Pasos
//		Se comprueba que los parámetros de entrada no sean nulos
//		Se definen los parámetros de la sentencia que son el id del objeto
//			y la versión
//		Se asignan los parámetros
//		Se ejecuta la sentencia
//		Se comprueba que venga un resultado y sólo uno
//		Se obtiene el valor del XML
//		Se comprueba que no sea nulo o vacío
//		Se copia el XML al fichero
//
//=================================================================================

m4return_t	ClMDTDictionary::_GetObjectFromDatabase( m4pcchar_t ai_pccObject, FILE *ao_pfFile, m4pcchar_t ai_pccFileName )
{

	m4return_t	iResult = NULL ;
	int			iWrite = -1 ;
	m4uint32_t	iRowNumber = 0 ;
	ClMDTValue	vXML ;
	M4Row		oParameters ;
	M4ResultSet	oResultSet ;


	M4MDT_RETURN_ERROR2( ai_pccObject == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pccObject", "ClMDTDictionary::_GetObjectFromDatabase", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ao_pfFile == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ao_pfFile", "ClMDTDictionary::_GetObjectFromDatabase", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ai_pccFileName == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pccFileName", "ClMDTDictionary::_GetObjectFromDatabase", "%s#%s" ) ;

	
	iResult = oParameters.Init( 3 ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetObjectFromDatabase", "Init", 3, "%s#%s - Parameter = <%d>" ) ;

	iResult = oParameters.SetFieldDescription( 0, M4CL_M4_TYPE_VAR_STRING, 30, 0 ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetObjectFromDatabase", "SetFieldDescription", 0, "%s#%s - Parameter = <%d>" ) ;

	iResult = oParameters.SetFieldDescription( 1, M4CL_M4_TYPE_NUMBER, 8, 0 ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetObjectFromDatabase", "SetFieldDescription", 1, "%s#%s - Parameter = <%d>" ) ;

	iResult = oParameters.SetFieldDescription( 2, M4CL_M4_TYPE_NUMBER, 8, 0 ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetObjectFromDatabase", "SetFieldDescription", 2, "%s#%s - Parameter = <%d>" ) ;

	iResult = oParameters.SetValue( 0, ai_pccObject ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetObjectFromDatabase", "SetValue", 0, "%s#%s - Parameter = <%d>" ) ;

	iResult = oParameters.SetValue( 1, m_vVersion ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetObjectFromDatabase", "SetValue", 1, "%s#%s - Parameter = <%d>" ) ;
	
	iResult = oParameters.SetValue( 2, m_vVersion ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetObjectFromDatabase", "SetValue", 2, "%s#%s - Parameter = <%d>" ) ;

	iResult = m_pIODBC->ExecuteQuery( m_sConnection, s_csObjectSelect, &oParameters, M4_TRUE, oResultSet ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetObjectFromDatabase", "ExecuteQuery", "%s#%s" ) ;

	iRowNumber = oResultSet.GetRowNumber() ;
	M4MDT_RETURN_ERROR2( iRowNumber > 1, M4_ERROR, M4MDTERROR_DUPLICATED_OBJECT, ai_pccObject, m_vVersion.c_str(), "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( iRowNumber < 1, M4_ERROR, M4MDTERROR_NO_SUCH_OBJECT, ai_pccObject, m_vVersion.c_str(), "%s#%s" ) ;
	
	iResult = oResultSet.GetValue( 0, s_ciObjects_XMLPos, vXML ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetObjectFromDatabase", "GetValue", "XML", "%s#%s - Parameter = <%s>" ) ;

	M4MDT_RETURN_ERROR2( vXML.IsNull() == M4_TRUE || vXML.empty(), M4_ERROR, M4MDTERROR_NO_SUCH_OBJECT, ai_pccObject, m_vVersion.c_str(), "%s#%s" ) ;

	iWrite = fprintf( ao_pfFile, "%s\n", vXML.c_str() ) ;
	M4MDT_RETURN_ERROR2( iWrite < 0, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, ai_pccFileName, strerror( errno ), "%s#%s" ) ;


	return( M4_SUCCESS ) ;
}



//=================================================================================
// Genera el fichero XML de una tabla del diccionario de Meta4
// a partir de su definición de base de datos
//
// Pasos
//		Se comprueba que los parámetros de entrada no sean nulos
//		Se copia el tag de inicio de XML
//		Se define el campo del nombre según el idioma. En la sentencia vienen los
//			2 y después se elige uno
//		Se definen los parámetros de la sentencia que es el id de la tabla
//		Se asignan los parámetros
//		Se ejecuta la sentencia
//		Se comprueba que venga un resultado y sólo uno
//		Se obtiene el valor del nombre
//		Si no viene nombre se pone el id
//		Se escapa el nombre traducido
//		Se copia la cabecera con el nombre al fichero
//		Se llama a la función que añade los campos de la tabla al fichero
//		Se llama a la función que añade los lookups de la tabla al fichero
//		Se copia el tag de final de tabla al fichero
//
//=================================================================================

m4return_t	ClMDTDictionary::_GetTableFromDatabase( m4pcchar_t ai_pccTable, FILE *ao_pfFile, m4pcchar_t ai_pccFileName )
{

	m4return_t	iResult = NULL ;
	int			iWrite = -1 ;
	m4uint32_t	iRowNumber = 0 ;
	m4uint32_t	iTranslated = 0 ;
	ClMDTValue	vTranslated ;
	M4Row		oParameters ;
	M4ResultSet	oResultSet ;
	M4ResultSet	oFieldsResultSet ;


	M4MDT_RETURN_ERROR2( ai_pccTable == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pccTable", "ClMDTDictionary::_GetTableFromDatabase", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ao_pfFile == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ao_pfFile", "ClMDTDictionary::_GetTableFromDatabase", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ai_pccFileName == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pccFileName", "ClMDTDictionary::_GetTableFromDatabase", "%s#%s" ) ;

	// UNICODE XML
	iWrite = fprintf( ao_pfFile, M4XMLHeader() ) ;
	M4MDT_RETURN_ERROR2( iWrite < 0, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, ai_pccFileName, strerror( errno ), "%s#%s" ) ;

	iWrite = fprintf( ao_pfFile, M4MDT_XML_COPYRIGHT ) ;
	M4MDT_RETURN_ERROR2( iWrite < 0, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, ai_pccFileName, strerror( errno ), "%s#%s" ) ;

	
	if( m_iLanguage == M4MDT_LANG_SPANISH )
	{
		iTranslated = s_ciTables_EspPos ;
	}
	else
	{
		iTranslated = s_ciTables_EngPos ;
	}

	
	iResult = oParameters.Init( 1 ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFromDatabase", "Init", 1, "%s#%s - Parameter = <%d>" ) ;

	iResult = oParameters.SetFieldDescription( 0, M4CL_M4_TYPE_VAR_STRING, 30, 0 ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFromDatabase", "SetFieldDescription", 0, "%s#%s - Parameter = <%d>" ) ;

	iResult = oParameters.SetValue( 0, ai_pccTable ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFromDatabase", "SetValue", 0, "%s#%s - Parameter = <%d>" ) ;

	iResult = m_pIODBC->ExecuteQuery( m_sConnection, s_csTableSelect, &oParameters, M4_TRUE, oResultSet ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFromDatabase", "ExecuteQuery", "%s#%s" ) ;

	iRowNumber = oResultSet.GetRowNumber() ;
	M4MDT_RETURN_ERROR1( iRowNumber > 1, M4_ERROR, M4MDTERROR_DUPLICATED_TABLE, ai_pccTable, "%s" ) ;
	M4MDT_RETURN_ERROR1( iRowNumber < 1, M4_ERROR, M4MDTERROR_NO_SUCH_TABLE, ai_pccTable, "%s" ) ;
	
	iResult = oResultSet.GetValue( 0, iTranslated, vTranslated ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFromDatabase", "GetValue", "ID_TRANS_OBJ", "%s#%s - Parameter = <%s>" ) ;

	if( vTranslated.IsNull() == M4_TRUE || vTranslated.empty() )
	{
		vTranslated.Set( ai_pccTable ) ;
	}

	iResult = M4XMLStreamGenerator::EscapeXMLStream( vTranslated ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFromDatabase", "M4XMLStreamGenerator::EscapeXMLStream", "%s#%s" ) ;

	iWrite = fprintf( ao_pfFile, "<!-- This XML defines logic object %s -->\n<logic-object name=\"%s\" caption=\"%s\">\n", ai_pccTable, ai_pccTable, vTranslated.c_str() ) ;
	M4MDT_RETURN_ERROR2( iWrite < 0, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, ai_pccFileName, strerror( errno ), "%s#%s" ) ;


	iResult = _GetTableFieldsFromDatabase( ai_pccTable, ao_pfFile, ai_pccFileName, oFieldsResultSet ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFromDatabase", "_GetTableFieldsFromDatabase", "%s#%s" ) ;

	iResult = _GetTableObjectsFromDatabase( oFieldsResultSet, ao_pfFile, ai_pccFileName ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFromDatabase", "_GetTableObjectsFromDatabase", "%s#%s" ) ;

	iResult = _GetTableLookupsFromDatabase( ai_pccTable, ao_pfFile, ai_pccFileName ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFromDatabase", "_GetTableLookupsFromDatabase", "%s#%s" ) ;


	iWrite = fprintf( ao_pfFile, "</logic-object>" ) ;
	M4MDT_RETURN_ERROR2( iWrite < 0, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, ai_pccFileName, strerror( errno ), "%s#%s" ) ;

	
	return( M4_SUCCESS ) ;
}

	
	
//=================================================================================
// Genera la parte de fichero XML de los campos una tabla del diccionario de Meta4
// a partir de su definición de base de datos
// Genera también la parte del objeto lógico que contiene la sentencia de lectura
//
// Pasos
//		Se comprueba que los parámetros de entrada no sean nulos
//		Se inicializan las cadenas de la sentencia
//		Se copia el tag de inicio de campos al fichero
//		Se define el campo del nombre según el idioma. En la sentencia vienen los
//			2 y después se elige uno
//		Se definen los parámetros de la sentencia que es el id de la tabla
//		Se asignan los parámetros
//		Se ejecuta la sentencia
//		Se ordenan los registros obtenidos para facilitar el procesamiento
//			El orden es tabla principal, tablas físicas, pk, campo no long,
//			posición del campo
//		Se obtiene el número de resultados
//		Se itera una vez por los resultados para calcular el número de campos
//			que no son de tipo long que va a haber en la sentencia de lectura.
//			Esto se necesita porque los campos long van a ir al final de
//			la sentencia.
//			En la iteración no se cuentan los campos traducidos (no existen
//			físicamente), los campos long, o los campos pk de las tablas no
//			principales (que van al join y no a la parte select)
//		Se itera una segunda vez por los resultados calculando la sentencia y
//			las posiciones de los campos
//		Los campos que son traducibles se ignoran
//		Se leen todas las columnas que se van a usar
//		Si el campo traducido es nulo se pone el identificador del campo
//		Para los tipos fecha se ajusta la precisión y la escala, porque sino
//			pueden provocar problemas en ODBC
//		Si se avanza de tabla física se incrementa la tabla en la que estamos
//			y se normalizan los valores de los atributos field-position y
//			pk-position para que empiecen en 1 para cada tabla
//			Además se pone la tabla en la parte from con su alias
//		Se incrementa el campo en el que estamos
//		Si el campo es pk se incrementa la pk en la que estamos y sino es pk 0.
//			Además, si es pk y no es la primera tabla va a la parte join de la
//			sentencia
//		Si no va al join se añade el campo a la parte select. Si es campo long a
//			la parte de longs incrementándose el long que es y calculando
//			el atributo logic-position con la base de los campos long. Si no
//			es campo long a la parte normal incrementándose el campos lógico
//			y calculando el atributo logic-position con la base de los campos
//			normales. 
//		Si va al join se añade el join con su homónimo de la primera tabla y se
//			pone el atributo logic-position a 0
//		Se escapa el nombre traducido
//		Se copian los atributos de los campos al fichero
//		Se copia el tag de final de campos al fichero
//		Si existen campos long se añaden a la parte select de campos normales
//		Se añade la parte from a la select
//		Si existen los joins se añaden a la parte select
//		Se copia el tag de objeto lógico al fichero
//
//=================================================================================

m4return_t	ClMDTDictionary::_GetTableFieldsFromDatabase( m4pcchar_t ai_pccTable, FILE *ao_pfFile, m4pcchar_t ai_pccFileName, M4ResultSet &ao_roResultSet )
{

	m4return_t	iResult = NULL ;
	m4bool_t	bJoin = M4_TRUE ;
	m4bool_t	bFirstField = M4_TRUE ;
	m4bool_t	bFirstLongField = M4_TRUE ;
	m4bool_t	bFirstFrom = M4_TRUE ;
	m4bool_t	bFirstWhere = M4_TRUE ;
	int			iInternalField = -1 ;
	int			iM4Type = 0 ;
	int			iPrecision = 0 ;
	int			iScale = 0 ;
	int			iPosPk = -1 ;
	int			iWrite = -1 ;
	m4uint32_t	i = 0 ;
	m4uint32_t	iPk = 0 ;
	m4uint32_t	iField = 0 ;
	m4uint32_t	iLogicField = 0 ;
	m4uint32_t	iLastPk = 0 ;
	m4uint32_t	iLastLogicField = 0 ;
	m4uint32_t	iLastLongField = 0 ;
	m4uint32_t	iNoLongFields = 0 ;
	m4uint32_t	iTableNumber = 0 ;
	m4uint32_t	iRowNumber = 0 ;
	m4uint32_t	iTranslated = 0 ;
	m4uint32_t	iHasWhere = 0 ;
	m4char_t	acLastTable[ M4MDT_MAX_REAL_TABLE_NAME_SIZE + 1 ] ;
	ClMDTValue	vField ;
	ClMDTValue	vTranslated ;
	ClMDTValue	vRealObject ;
	ClMDTValue	vRealField ;
	ClMDTValue	vM4Type ;
	ClMDTValue	vPrecision ;
	ClMDTValue	vScale ;
	ClMDTValue	vPosPK ;
	ClMDTValue	vInternalField ;
	M4Row		oParameters ;
	string		sSelect ;
	string		sLongSelect ;
	string		sFrom ;
	string		sWhere ;


	M4MDT_RETURN_ERROR2( ai_pccTable == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pccTable", "ClMDTDictionary::_GetTableFieldsFromDatabase", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ao_pfFile == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ao_pfFile", "ClMDTDictionary::_GetTableFieldsFromDatabase", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ai_pccFileName == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pccFileName", "ClMDTDictionary::_GetTableFieldsFromDatabase", "%s#%s" ) ;

	sSelect.reserve( 4096 ) ;
	sLongSelect.reserve( 1024 ) ;
	sFrom.reserve( 1024 ) ;
	sWhere.reserve( 1024 ) ;

	sSelect = "SELECT " ;
	sLongSelect = "" ;
	sFrom = " FROM " ;
	sWhere = " WHERE " ;


	iWrite = fprintf( ao_pfFile, "\t<logic-fields>\n" ) ;
	M4MDT_RETURN_ERROR2( iWrite < 0, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, ai_pccFileName, strerror( errno ), "%s#%s" ) ;
	

	if( m_iLanguage == M4MDT_LANG_SPANISH )
	{
		iTranslated = s_ciFields_EspPos ;
	}
	else
	{
		iTranslated = s_ciFields_EngPos ;
	}

	
	iResult = oParameters.Init( 1 ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFieldsFromDatabase", "Init", 1, "%s#%s - Parameter = <%d>" ) ;

	iResult = oParameters.SetFieldDescription( 0, M4CL_M4_TYPE_VAR_STRING, 30, 0 ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFieldsFromDatabase", "SetFieldDescription", 0, "%s#%s - Parameter = <%d>" ) ;

	iResult = oParameters.SetValue( 0, ai_pccTable ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFieldsFromDatabase", "SetValue", 0, "%s#%s - Parameter = <%d>" ) ;

	iResult = m_pIODBC->ExecuteQuery( m_sConnection, s_csFieldSelect, &oParameters, M4_TRUE, ao_roResultSet ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFieldsFromDatabase", "ExecuteQuery", "%s#%s" ) ;

	iResult = ao_roResultSet.Sort( M4MDTCompareFieldsByTableAndType, NULL) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFieldsFromDatabase", "Sort", "%s#%s" ) ;

	iRowNumber = ao_roResultSet.GetRowNumber() ;


	iNoLongFields = 0 ;
	iTableNumber = 0 ;
	*acLastTable = '\0' ;

	for( i = 0 ; i < iRowNumber ; i++ )
	{
		iResult = ao_roResultSet.GetValue( i, s_ciFields_InternalFieldPos, vInternalField ) ;
		M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFieldsFromDatabase", "GetValue", "ID_INTERNAL_FIELD", "%s#%s - Parameter = <%s>" ) ;

		iInternalField = atoi ( vInternalField.c_str() ) ;

		if( iInternalField == M4CL_INTERNAL_TYPE_TRANSLATION_PTR )
		{
			continue ;
		}

		iResult = ao_roResultSet.GetValue( i, s_ciFields_M4TypePos, vM4Type ) ;
		M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFieldsFromDatabase", "GetValue", "ID_M4_TYPE", "%s#%s - Parameter = <%s>" ) ;

		iM4Type = atoi( vM4Type.c_str() ) ;

		if( iM4Type == M4CL_M4_TYPE_LONG || iM4Type == M4CL_M4_TYPE_UNI_LONG || iM4Type == M4CL_M4_TYPE_BINARY_STRING || iM4Type == M4CL_M4_TYPE_BLOB )
		{
			continue ;
		}

		iResult = ao_roResultSet.GetValue( i, s_ciFields_RealObjPos, vRealObject ) ;
		M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFieldsFromDatabase", "GetValue", "ID_REAL_OBJECT", "%s#%s - Parameter = <%s>" ) ;

		if( strcmpi( vRealObject.c_str(), acLastTable ) != 0 )
		{
			iTableNumber++ ;
			strcpy( acLastTable, vRealObject.c_str() ) ;
		}

		if( iTableNumber == 1 )
		{
			iNoLongFields++ ;
		}
		else
		{
			iResult = ao_roResultSet.GetValue( i, s_ciFields_PosPkPos, vPosPK ) ;
			M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFieldsFromDatabase", "GetValue", "POS_PK", "%s#%s - Parameter = <%s>" ) ;

			iPosPk = atoi( vPosPK.c_str() ) ;

			if( iPosPk == 0 )
			{
				iNoLongFields++ ;
			}
		}
	}


	bFirstField = M4_TRUE ;
	bFirstLongField = M4_TRUE ;
	bFirstFrom = M4_TRUE ;
	bFirstWhere = M4_TRUE ;

	iLastLogicField = 0 ;
	iLastLongField = iNoLongFields ;

	iTableNumber = 0 ;
	*acLastTable = '\0' ;

	for( i = 0 ; i < iRowNumber ; i++ )
	{
		iResult = ao_roResultSet.GetValue( i, s_ciFields_InternalFieldPos, vInternalField ) ;
		M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFieldsFromDatabase", "GetValue", "ID_INTERNAL_FIELD", "%s#%s - Parameter = <%s>" ) ;

		iInternalField = atoi ( vInternalField.c_str() ) ;

		if( iInternalField == M4CL_INTERNAL_TYPE_TRANSLATION_PTR )
		{
			continue ;
		}

		iResult = ao_roResultSet.GetValue( i, s_ciFields_FieldPos, vField ) ;
		M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFieldsFromDatabase", "GetValue", "ID_FIELD", "%s#%s - Parameter = <%s>" ) ;

		iResult = ao_roResultSet.GetValue( i, iTranslated, vTranslated ) ;
		M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFieldsFromDatabase", "GetValue", "ID_TRANS_FLD", "%s#%s - Parameter = <%s>" ) ;

		iResult = ao_roResultSet.GetValue( i, s_ciFields_RealObjPos, vRealObject ) ;
		M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFieldsFromDatabase", "GetValue", "ID_REAL_OBJECT", "%s#%s - Parameter = <%s>" ) ;

		iResult = ao_roResultSet.GetValue( i, s_ciFields_RealFieldPos, vRealField ) ;
		M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFieldsFromDatabase", "GetValue", "ID_REAL_FIELD", "%s#%s - Parameter = <%s>" ) ;

		iResult = ao_roResultSet.GetValue( i, s_ciFields_M4TypePos, vM4Type ) ;
		M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFieldsFromDatabase", "GetValue", "ID_M4_TYPE", "%s#%s - Parameter = <%s>" ) ;

		iResult = ao_roResultSet.GetValue( i, s_ciFields_PrecisionPos, vPrecision ) ;
		M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFieldsFromDatabase", "GetValue", "PREC", "%s#%s - Parameter = <%s>" ) ;

		iResult = ao_roResultSet.GetValue( i, s_ciFields_ScalePos, vScale ) ;
		M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFieldsFromDatabase", "GetValue", "SCALE", "%s#%s - Parameter = <%s>" ) ;

		iResult = ao_roResultSet.GetValue( i, s_ciFields_PosPkPos, vPosPK ) ;
		M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFieldsFromDatabase", "GetValue", "POS_PK", "%s#%s - Parameter = <%s>" ) ;


		if( vTranslated.IsNull() == M4_TRUE || vTranslated.empty() )
		{
			vTranslated = vField ;
		}

		iM4Type = atoi( vM4Type.c_str() ) ;
		iPrecision = atoi( vPrecision.c_str() ) ;
		iScale = atoi( vScale.c_str() ) ;
		iPosPk = atoi( vPosPK.c_str() ) ;

		if( iM4Type == M4CL_M4_TYPE_TIMESTAMP )
		{
			iPrecision = M4CL_MAX_TIMESTAMP_PRECISION ;
			iScale = 0 ;
		}
		else if( iM4Type == M4CL_M4_TYPE_DATE )
		{
			iPrecision = M4CL_MAX_DATE_PRECISION ;
			iScale = 0 ;
		}


		if( strcmpi( vRealObject.c_str(), acLastTable ) != 0 )
		{
			iTableNumber++ ;
			strcpy( acLastTable, vRealObject.c_str() ) ;

			iField = 0 ;
			iLastPk = 0 ;

			if( bFirstFrom == M4_TRUE )
			{
				bFirstFrom = M4_FALSE ;
			}
			else
			{
				sFrom.append( ", " ) ;
			}

			sFrom.append( vRealObject ) ;
			sFrom.append( " " ) ;
			sFrom.append( 1, char( iTableNumber + 'A' - 1 ) ) ;
		}

		iField++ ;

		if( iPosPk != 0 )
		{
			iPk = ++iLastPk ;

			if( iTableNumber == 1 )
			{
				bJoin = M4_FALSE ;
			}
			else
			{
				bJoin = M4_TRUE ;
			}
		}
		else
		{
			iPk = 0 ;
			bJoin = M4_FALSE ;
		}

		if( bJoin == M4_FALSE )
		{
			if( iM4Type == M4CL_M4_TYPE_LONG || iM4Type == M4CL_M4_TYPE_UNI_LONG || iM4Type == M4CL_M4_TYPE_BINARY_STRING || iM4Type == M4CL_M4_TYPE_BLOB )
			{
				if( bFirstLongField == M4_TRUE )
				{
					bFirstLongField = M4_FALSE ;
				}
				else
				{
					sLongSelect.append( ", " ) ;
				}

				sLongSelect.append( 1, char( iTableNumber + 'A' - 1 ) ) ;
				sLongSelect.append( "." ) ;
				sLongSelect.append( vRealField ) ;

				iLogicField = ++iLastLongField ;
			}
			else
			{
				if( bFirstField == M4_TRUE )
				{
					bFirstField = M4_FALSE ;
				}
				else
				{
					sSelect.append( ", " ) ;
				}

				sSelect.append( 1, char( iTableNumber + 'A' - 1 ) ) ;
				sSelect.append( "." ) ;
				sSelect.append( vRealField ) ;

				iLogicField = ++iLastLogicField ;
			}
		}
		else
		{
			if( bFirstWhere == M4_TRUE )
			{
				bFirstWhere = M4_FALSE ;
			}
			else
			{
				sWhere.append( " AND " ) ;
			}

			sWhere.append( 1, char( iTableNumber + 'A' - 1 ) ) ;
			sWhere.append( "." ) ;
			sWhere.append( vRealField ) ;
			sWhere.append( " = A." ) ;
			sWhere.append( vRealField ) ;

			iLogicField = 0 ;
		}

		iResult = M4XMLStreamGenerator::EscapeXMLStream( vTranslated ) ;
		M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFieldsFromDatabase", "M4XMLStreamGenerator::EscapeXMLStream", "%s#%s" ) ;

		iWrite = fprintf( ao_pfFile, "\t\t<logic-field name=\"%s\" caption=\"%s\" alias=\"%c\" real-field=\"%s\" m4type=\"%d\" precision=\"%d\" scale=\"%d\" logic-position=\"%d\" field-position=\"%d\" pk-position=\"%d\" internal-field=\"%d\"/>\n", vField.c_str(), vTranslated.c_str(), char( iTableNumber + 'A' - 1 ), vRealField.c_str(), iM4Type, iPrecision, iScale, iLogicField, iField, iPk, iInternalField ) ;
		M4MDT_RETURN_ERROR2( iWrite < 0, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, ai_pccFileName, strerror( errno ), "%s#%s" ) ;
	}

	iWrite = fprintf( ao_pfFile, "\t</logic-fields>\n" ) ;
	M4MDT_RETURN_ERROR2( iWrite < 0, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, ai_pccFileName, strerror( errno ), "%s#%s" ) ;

	
	if( bFirstLongField == M4_FALSE )
	{
		if( bFirstField == M4_FALSE )
		{
			sSelect.append( ", " ) ;
		}
		sSelect.append( sLongSelect ) ;
	}

	sSelect.append( sFrom ) ;

	if( bFirstWhere == M4_FALSE )
	{
		iHasWhere = 1 ;
		sSelect.append( sWhere ) ;
	}
	else
	{
		iHasWhere = 0 ;
	}

	iWrite = fprintf( ao_pfFile, "\t<logic-object has-where=\"%d\" select=\"%s\"/>\n", iHasWhere, sSelect.c_str() ) ;
	M4MDT_RETURN_ERROR2( iWrite < 0, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, ai_pccFileName, strerror( errno ), "%s#%s" ) ;

	return( M4_SUCCESS ) ;
}



//=================================================================================
//
// Genera la parte de fichero XML de las tablas físicas una tabla del diccionario
// de Meta4 a partir de su definición de base de datos
//
// Pasos
//
//		Se comprueba que los parámetros de entrada no sean nulos
//		Se copia el tag de inicio de tablas al fichero
//		Se obtiene el número de resultados
//		Se itera por los resultados
//		Los campos que son traducibles se ignoran
//		Para cada campo se obtienen los valores del objeto real, del campo real y
//			de su posición en la pk
//		Se van generando las sentencias de borrado e inserción con los campos
//		Se asume que los campos vienen ordenados primero por tabla física
//		Se va calculando el ordinal de la tabla y el número de parámetros de las
//			sentencias
//		Se copian los atributos de los campos, que son las sentencias, el número
//			de parámetros de las sentencias y el ordinal de la tabla
//		Se copia el tag de final de campos al fichero
//
//=================================================================================

m4return_t	ClMDTDictionary::_GetTableObjectsFromDatabase( const M4IReadResultSet &ai_rcoResultSet, FILE *ao_pfFile, m4pcchar_t ai_pccFileName )
{

	m4return_t	iResult = NULL ;
	m4bool_t	bFirstPk = M4_TRUE ;
	m4bool_t	bFirstField = M4_TRUE ;
	int			iWrite = -1 ;
	int			iPosPk = -1 ;
	int			iInternalField = -1 ;
	m4uint32_t	i = 0 ;
	m4uint32_t	j = 0 ;
	m4uint32_t	iPkNumber = 0 ;
	m4uint32_t	iFieldNumber = 0 ;
	m4uint32_t	iTableNumber = 0 ;
	m4uint32_t	iRowNumber = 0 ;
	m4char_t	acLastTable[ M4MDT_MAX_REAL_TABLE_NAME_SIZE + 1 ] ;
	ClMDTValue	vRealObject ;
	ClMDTValue	vRealField ;
	ClMDTValue	vPosPK ;
	ClMDTValue	vInternalField ;
	string		sDelete ;
	string		sInsert ;


	M4MDT_RETURN_ERROR2( ao_pfFile == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ao_pfFile", "ClMDTDictionary::_GetTableFieldsFromDatabase", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ai_pccFileName == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pccFileName", "ClMDTDictionary::_GetTableFieldsFromDatabase", "%s#%s" ) ;

	iWrite = fprintf( ao_pfFile, "\t<real-objects>\n" ) ;
	M4MDT_RETURN_ERROR2( iWrite < 0, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, ai_pccFileName, strerror( errno ), "%s#%s" ) ;
	

	iRowNumber = ai_rcoResultSet.GetRowNumber() ;

	sDelete.reserve( 1024 ) ;
	sInsert.reserve( 4096 ) ;

	iTableNumber = 0 ;
	*acLastTable = '\0' ;

	for( i = 0 ; i < iRowNumber ; i++ )
	{
		iResult = ai_rcoResultSet.GetValue( i, s_ciFields_InternalFieldPos, vInternalField ) ;
		M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFieldsFromDatabase", "GetValue", "ID_INTERNAL_FIELD", "%s#%s - Parameter = <%s>" ) ;

		iInternalField = atoi ( vInternalField.c_str() ) ;

		if( iInternalField == M4CL_INTERNAL_TYPE_TRANSLATION_PTR )
		{
			continue ;
		}

		iResult = ai_rcoResultSet.GetValue( i, s_ciFields_RealObjPos, vRealObject ) ;
		M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFieldsFromDatabase", "GetValue", "ID_REAL_OBJECT", "%s#%s - Parameter = <%s>" ) ;

		iResult = ai_rcoResultSet.GetValue( i, s_ciFields_RealFieldPos, vRealField ) ;
		M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFieldsFromDatabase", "GetValue", "ID_REAL_FIELD", "%s#%s - Parameter = <%s>" ) ;

		iResult = ai_rcoResultSet.GetValue( i, s_ciFields_PosPkPos, vPosPK ) ;
		M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableFieldsFromDatabase", "GetValue", "POS_PK", "%s#%s - Parameter = <%s>" ) ;


		if( strcmpi( vRealObject.c_str(), acLastTable ) != 0 )
		{
			if( *acLastTable != '\0' )
			{
				sInsert.append( " ) VALUES ( " ) ;
				for( j = 0 ; j < iFieldNumber ; j++ )
				{
					if( j != 0 )
					{
						sInsert.append( ", " ) ;
					}
					sInsert.append( "?" ) ;
				}
				sInsert.append( " )" ) ;

				iWrite = fprintf( ao_pfFile, "\t\t<real-object name=\"%c\" table-position=\"%d\" delete-number=\"%d\" insert-number=\"%d\" delete=\"%s\" insert=\"%s\"/>\n", char( iTableNumber + 'A' - 1 ), iTableNumber, iPkNumber, iFieldNumber, sDelete.c_str(), sInsert.c_str() ) ;
				M4MDT_RETURN_ERROR2( iWrite < 0, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, ai_pccFileName, strerror( errno ), "%s#%s" ) ;
			}

			iTableNumber++ ;
			strcpy( acLastTable, vRealObject.c_str() ) ;

			bFirstPk = M4_TRUE ;
			bFirstField = M4_TRUE ;
			iPkNumber = 0 ;
			iFieldNumber = 0 ;

			sDelete = "DELETE FROM " ;
			sDelete.append( acLastTable ) ;

			sInsert = "INSERT INTO " ;
			sInsert.append( acLastTable ) ;
		}

		iPosPk = atoi( vPosPK.c_str() ) ;

		if( iPosPk != 0 )
		{
			if( bFirstPk == M4_TRUE )
			{
				bFirstPk = M4_FALSE ;
				sDelete.append( " WHERE " ) ;
			}
			else
			{
				sDelete.append( " AND " ) ;
			}

			sDelete.append( vRealField.c_str() ) ;
			sDelete.append( " = ?" ) ;

			iPkNumber++ ;
		}

		if( bFirstField == M4_TRUE )
		{
			bFirstField = M4_FALSE ;
			sInsert.append( " ( " ) ;
		}
		else
		{
			sInsert.append( ", " ) ;
		}

		sInsert.append( vRealField.c_str() ) ;

		iFieldNumber++ ;
	}

	sInsert.append( " ) VALUES ( " ) ;
	for( j = 0 ; j < iFieldNumber ; j++ )
	{
		if( j != 0 )
		{
			sInsert.append( ", " ) ;
		}
		sInsert.append( "?" ) ;
	}
	sInsert.append( " )" ) ;

	iWrite = fprintf( ao_pfFile, "\t\t<real-object name=\"%c\" table-position=\"%d\" delete-number=\"%d\" insert-number=\"%d\" delete=\"%s\" insert=\"%s\"/>\n", char( iTableNumber + 'A' - 1 ), iTableNumber, iPkNumber, iFieldNumber, sDelete.c_str(), sInsert.c_str() ) ;
	M4MDT_RETURN_ERROR2( iWrite < 0, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, ai_pccFileName, strerror( errno ), "%s#%s" ) ;

	iWrite = fprintf( ao_pfFile, "\t</real-objects>\n" ) ;
	M4MDT_RETURN_ERROR2( iWrite < 0, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, ai_pccFileName, strerror( errno ), "%s#%s" ) ;


	return( M4_SUCCESS ) ;
}



//=================================================================================
// Genera la parte de fichero XML de los lookups una tabla del diccionario de Meta4
// a partir de su definición de base de datos
//
// Pasos
//		Se comprueba que los parámetros de entrada no sean nulos
//		Se copia el tag de inicio de lookups al fichero
//		Se definen los parámetros de la sentencia que es el id de la tabla
//		Se asignan los parámetros
//		Se ejecuta la sentencia
//		Se obtiene el número de resultados
//		Se itera por los resultados
//		Los campos que son traducibles se ignoran
//		Para cada campo se obtienen los valores
//		Para cada campo se llama a obtener el nombre del campo real
//		Para cada campo se llama a generar la parte de XML que define su lookup
//		Se copia el tag de final de lookups al fichero
//
//=================================================================================

m4return_t	ClMDTDictionary::_GetTableLookupsFromDatabase( m4pcchar_t ai_pccTable, FILE *ao_pfFile, m4pcchar_t ai_pccFileName )
{

	m4return_t	iResult = NULL ;
	int			iWrite = -1 ;
	m4uint32_t	i = 0 ;
	m4uint32_t	iRowNumber = 0 ;
	ClMDTValue	vField ;
	ClMDTValue	vLuObject ;
	ClMDTValue	vLuField ;
	ClMDTValue	vLuRealField ;
	ClMDTValue	vRealObject ;
	ClMDTValue	vRealField ;
	M4Row		oParameters ;
	M4ResultSet	oResultSet ;

	
	M4MDT_RETURN_ERROR2( ai_pccTable == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pccTable", "ClMDTDictionary::_GetTableLookupsFromDatabase", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ao_pfFile == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ao_pfFile", "ClMDTDictionary::_GetTableLookupsFromDatabase", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ai_pccFileName == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pccFileName", "ClMDTDictionary::_GetTableLookupsFromDatabase", "%s#%s" ) ;

	iWrite = fprintf( ao_pfFile, "\t<lookups>\n" ) ;
	M4MDT_RETURN_ERROR2( iWrite < 0, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, ai_pccFileName, strerror( errno ), "%s#%s" ) ;


	iResult = oParameters.Init( 1 ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableLookupsFromDatabase", "Init", 1, "%s#%s - Parameter = <%d>" ) ;

	iResult = oParameters.SetFieldDescription( 0, M4CL_M4_TYPE_VAR_STRING, 30, 0 ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableLookupsFromDatabase", "SetFieldDescription", 0, "%s#%s - Parameter = <%d>" ) ;

	iResult = oParameters.SetValue( 0, ai_pccTable ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableLookupsFromDatabase", "SetValue", 0, "%s#%s - Parameter = <%d>" ) ;

	iResult = m_pIODBC->ExecuteQuery( m_sConnection, s_csLookUpSelect, &oParameters, M4_TRUE, oResultSet ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableLookupsFromDatabase", "ExecuteQuery", "%s#%s" ) ;

	iRowNumber = oResultSet.GetRowNumber() ;

	for( i = 0 ; i < iRowNumber ; i++ )
	{
		iResult = oResultSet.GetValue( i, s_ciLookUps_FieldPos, vField ) ;
		M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableLookupsFromDatabase", "GetValue", "ID_FIELD", "%s#%s - Parameter = <%s>" ) ;

		iResult = oResultSet.GetValue( i, s_ciLookUps_LuObjectPos, vLuObject ) ;
		M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableLookupsFromDatabase", "GetValue", "ID_LU_OBJECT", "%s#%s - Parameter = <%s>" ) ;

		iResult = oResultSet.GetValue( i, s_ciLookUps_LuFieldPos, vLuField ) ;
		M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableLookupsFromDatabase", "GetValue", "ID_LU_FIELD", "%s#%s - Parameter = <%s>" ) ;

		iResult = oResultSet.GetValue( i, s_ciLookUps_RealObjPos, vRealObject ) ;
		M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableLookupsFromDatabase", "GetValue", "ID_REAL_OBJECT", "%s#%s - Parameter = <%s>" ) ;

		iResult = oResultSet.GetValue( i, s_ciLookUps_RealFieldPos, vRealField ) ;
		M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableLookupsFromDatabase", "GetValue", "ID_REAL_FIELD", "%s#%s - Parameter = <%s>" ) ;


		iResult = _GetRealField( vLuObject, vLuField, vLuRealField ) ;
		M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableLookupsFromDatabase", "_GetRealField", "%s#%s" ) ;

		iResult = _GetTableLookupFromDatabase( vField, vRealObject, vRealField, vLuRealField, ao_pfFile, ai_pccFileName ) ;
		M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableLookupsFromDatabase", "_GetTableLookupFromDatabase", "%s#%s" ) ;
	}


	iWrite = fprintf( ao_pfFile, "\t</lookups>\n" ) ;
	M4MDT_RETURN_ERROR2( iWrite < 0, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, ai_pccFileName, strerror( errno ), "%s#%s" ) ;
	
	return( M4_SUCCESS ) ;
}



//=================================================================================
//
// Obtiene el nombre del campo físico de un campo lógico.
// Tiene en cuenta si el campo es traducible
//
// Pasos
//		Primero se calcula si el campo es traducible y se obtiene el nombre del
//			campo físico traducido en el idioma actual
//		Se define el idioma actual
//		Se definen los parámetros de la sentencia de campos traducidos que son la
//			tabla lógica, el campo lógico y el idioma
//		Se asignan los parámetros
//		Se ejecuta la sentencia
//		Se obtiene el número de resultados
//		Si existe un resultado se lee el valor del nombre del campo traducido y se
//			retorna
//		Se definen los parámetros de la sentencia de campos físicos que son la
//			tabla lógica y el campo lógico
//		Se asignan los parámetros
//		Se ejecuta la sentencia
//		Se obtiene el número de resultados
//		Si no existe un resultado o si existe más de uno se da un error
//		Se lee el valor del nombre del campo físico y se retorna
//
//=================================================================================

m4return_t	ClMDTDictionary::_GetRealField( const ClMDTValue& ai_rcvObject, const ClMDTValue& ai_rcvField, ClMDTValue& ai_rvRealField )
{

	m4return_t	iResult = NULL ;
	m4uint32_t	iRowNumber = 0 ;
	ClMDTValue	vLanguage ;
	M4Row		oTransParameters ;
	M4Row		oRealParameters ;
	M4ResultSet	oTransResultSet ;
	M4ResultSet	oRealResultSet ;


	if( m_iLanguage == M4MDT_LANG_SPANISH )
	{
		vLanguage.Set( M4MDT_LANG_SPANISH_STR ) ;
	}
	else
	{
		vLanguage.Set( M4MDT_LANG_ENGLISH_STR ) ;
	}


	iResult = oTransParameters.Init( 3 ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetRealField", "Init", 3, "%s#%s - Parameter = <%d>" ) ;

	iResult = oTransParameters.SetFieldDescription( 0, M4CL_M4_TYPE_VAR_STRING, 30, 0 ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetRealField", "SetFieldDescription", 0, "%s#%s - Parameter = <%d>" ) ;

	iResult = oTransParameters.SetFieldDescription( 1, M4CL_M4_TYPE_VAR_STRING, 30, 0 ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetRealField", "SetFieldDescription", 1, "%s#%s - Parameter = <%d>" ) ;

	iResult = oTransParameters.SetFieldDescription( 2, M4CL_M4_TYPE_NUMBER, 4, 0) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetRealField", "SetFieldDescription", 2, "%s#%s - Parameter = <%d>" ) ;

	iResult = oTransParameters.SetValue( 0, ai_rcvObject ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetRealField", "SetValue", 0, "%s#%s - Parameter = <%d>" ) ;

	iResult = oTransParameters.SetValue( 1, ai_rcvField ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetRealField", "SetValue", 1, "%s#%s - Parameter = <%d>" ) ;

	iResult = oTransParameters.SetValue( 2, vLanguage ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetRealField", "SetValue", 2, "%s#%s - Parameter = <%d>" ) ;

	iResult = m_pIODBC->ExecuteQuery( m_sConnection, s_csTransFieldSelect, &oTransParameters, M4_TRUE, oTransResultSet ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetRealField", "ExecuteQuery", "%s#%s" ) ;

	iRowNumber = oTransResultSet.GetRowNumber() ;

	if( iRowNumber == 1 )
	{
		iResult = oTransResultSet.GetValue( 0, s_ciTransFields_RealFieldPos, ai_rvRealField ) ;
		M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetRealField", "GetValue", "ID_REAL_FIELD", "%s#%s - Parameter = <%s>" ) ;

		return( M4_SUCCESS ) ;
	}


	iResult = oRealParameters.Init( 2 ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetRealField", "Init", 2, "%s#%s - Parameter = <%d>" ) ;

	iResult = oRealParameters.SetFieldDescription( 0, M4CL_M4_TYPE_VAR_STRING, 30, 0 ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetRealField", "SetFieldDescription", 0, "%s#%s - Parameter = <%d>" ) ;

	iResult = oRealParameters.SetFieldDescription( 1, M4CL_M4_TYPE_VAR_STRING, 30, 0 ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetRealField", "SetFieldDescription", 1, "%s#%s - Parameter = <%d>" ) ;

	iResult = oRealParameters.SetValue( 0, ai_rcvObject ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetRealField", "SetValue", 0, "%s#%s - Parameter = <%d>" ) ;

	iResult = oRealParameters.SetValue( 1, ai_rcvField ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetRealField", "SetValue", 1, "%s#%s - Parameter = <%d>" ) ;

	iResult = m_pIODBC->ExecuteQuery( m_sConnection, s_csRealFieldSelect, &oRealParameters, M4_TRUE, oRealResultSet ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetRealField", "ExecuteQuery", "%s#%s" ) ;

	iRowNumber = oRealResultSet.GetRowNumber() ;
	M4MDT_RETURN_ERROR2( iRowNumber != 1, M4_ERROR, M4MDTERROR_NO_REAL_FIELD, ai_rcvField.c_str(), ai_rcvObject.c_str(), "%s#%s" ) ;

	iResult = oRealResultSet.GetValue( 0, s_ciRealFields_RealFieldPos, ai_rvRealField ) ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetRealField", "GetValue", "ID_REAL_FIELD", "%s#%s - Parameter = <%s>" ) ;


	return( M4_SUCCESS ) ;
}



//=================================================================================
//
// Genera la parte de fichero XML de un lookup de un campo de una tabla del
// diccionario de Meta4 a partir de su definición de base de datos
//
// Pasos
//		Se comprueba que los parámetros de entrada no sean nulos
//		Se copia el tag de inicio de lookup al fichero
//		Se compone la sentencia sobre la tabla de lookup que no tiene parámetros
//		Se ejecuta la sentencia
//		Se obtiene el número de resultados
//		Se itera por los resultados
//		Para cada registro se obtiene el valor y su traducción
//		Se escapa el valor traducido
//		Se copian los atributos de los campos al fichero
//		Se copia el tag de final de lookup al fichero
//
//=================================================================================

m4return_t	ClMDTDictionary::_GetTableLookupFromDatabase( const ClMDTValue& ai_rcvField, const ClMDTValue& ai_rcvRealObject, const ClMDTValue& ai_rcvValueRealField, const ClMDTValue& ai_rcvCaptionRealField, FILE *ao_pfFile, m4pcchar_t ai_pccFileName )
{

	m4return_t	iResult = NULL ;
	int			iWrite = -1 ;
	m4uint32_t	i = 0 ;
	m4uint32_t	iRowNumber = 0 ;
	string		sSelect ;
	ClMDTValue	vValue ;
	ClMDTValue	vCaption ;
	M4Row		oParameters ;
	M4ResultSet	oResultSet ;

	
	M4MDT_RETURN_ERROR2( ao_pfFile == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ao_pfFile", "ClMDTDictionary::_GetTableLookupFromDatabase", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ai_pccFileName == NULL, M4_ERROR, M4MDTERROR_NULL_ARGUMENT, "ai_pccFileName", "ClMDTDictionary::_GetTableLookupFromDatabase", "%s#%s" ) ;

	
	iWrite = fprintf( ao_pfFile, "\t\t<lookup name=\"%s\">\n", ai_rcvField.c_str() ) ;
	M4MDT_RETURN_ERROR2( iWrite < 0, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, ai_pccFileName, strerror( errno ), "%s#%s" ) ;


	sSelect = "SELECT " ; 
	sSelect.append( ai_rcvValueRealField.c_str() ) ;
	sSelect.append( ", " ) ;
	sSelect.append( ai_rcvCaptionRealField.c_str() ) ;
	sSelect.append( " FROM " ) ;
	sSelect.append( ai_rcvRealObject.c_str() ) ;

	iResult = m_pIODBC->ExecuteQuery( m_sConnection, sSelect, &oParameters, M4_TRUE, oResultSet ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableLookupFromDatabase", "ExecuteQuery", "%s#%s" ) ;

	iRowNumber = oResultSet.GetRowNumber() ;

	for( i = 0 ; i < iRowNumber ; i++ )
	{
		iResult = oResultSet.GetValue( i, 0, vValue ) ;
		M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableLookupFromDatabase", "GetValue", ai_rcvValueRealField.c_str(), "%s#%s - Parameter = <%s>" ) ;

		iResult = oResultSet.GetValue( i, 1, vCaption ) ;
		M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableLookupFromDatabase", "GetValue", ai_rcvCaptionRealField.c_str(), "%s#%s - Parameter = <%s>" ) ;


		iResult = M4XMLStreamGenerator::EscapeXMLStream( vCaption ) ;
		M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetTableLookupFromDatabase", "M4XMLStreamGenerator::EscapeXMLStream", "%s#%s" ) ;

		iWrite = fprintf( ao_pfFile, "\t\t\t<lookup-register value=\"%s\" caption=\"%s\"/>\n", vValue.c_str(), vCaption.c_str() ) ;
		M4MDT_RETURN_ERROR2( iWrite < 0, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, ai_pccFileName, strerror( errno ), "%s#%s" ) ;
	}


	iWrite = fprintf( ao_pfFile, "\t\t</lookup>\n" ) ;
	M4MDT_RETURN_ERROR2( iWrite < 0, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, ai_pccFileName, strerror( errno ), "%s#%s" ) ;


	return( M4_SUCCESS ) ;
}




//=================================================================================
//
// Carga la configuración para de los campos lógicos.
// Este método recupera la información que el usuario ha especificado en las
// tablas de configuración de campos lógicos para las configuraciones habilitadas. 
// Estos campos son los campos lógicos que se consideran no comparables y 
// traspasables o no traspasables según se indique.
//
// Pasos
//	Cargar los campos lógicos de los objetos lógicos de las oonfiguraciones activas 
//	y si son traspasables o no.
//	Guardar los campos lógicos no conparables y sí traspasables en estructuras 
//	internas de la clase.
// 
//=================================================================================
m4return_t	ClMDTDictionary::_LoadFieldConfiguration()
{
	m4return_t							iResult = M4_ERROR;
	m4uint32_t							i = 0;
	m4uint32_t							iRowNumber = 0;
	m4int32_t							iIsTransferable = 1;
	M4Row								oFieldParameters;
	M4ResultSet							oFieldResultSet;
	M4ResultSet							oObjectResultSet;
	ClMDTValue							vIdObject;
	ClMDTValue							vIdField;
	ClMDTValue							vIsTransferable;
	exclusionData_t						* poFieldsMap = NULL;
	exclusionObjectField_t::iterator	objectIterator;
	exclusionData_t::iterator			iterator;
	
	iResult = m_pIODBC->ExecuteQuery(m_sConnection, s_csFieldConfigSelect, &oFieldParameters, M4_TRUE, oFieldResultSet);
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_LoadFieldConfiguration", "ExecuteQuery", "%s#%s" );

	iRowNumber = oFieldResultSet.GetRowNumber();
	for (i = 0 ; i < iRowNumber ; i++)
	{
		iResult = oFieldResultSet.GetValue(i, s_ciIdObject_FieldConfigPos, vIdObject);
		M4MDT_RETURN_ERROR3(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_LoadFieldConfiguration", "GetValue", "ID_OBJECT", "%s#%s - Parameter = <%s>");

		iResult = oFieldResultSet.GetValue(i, s_ciIdField_FieldConfigPos, vIdField);
		M4MDT_RETURN_ERROR3(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_LoadFieldConfiguration", "GetValue", "ID_FIELD", "%s#%s - Parameter = <%s>");

		iResult = oFieldResultSet.GetValue(i, s_ciIsTransferable_FieldConfigPos, vIsTransferable);
		M4MDT_RETURN_ERROR3(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_LoadFieldConfiguration", "GetValue", "IS_TRANSFERABLE", "%s#%s - Parameter = <%s>");

		// Guardar los campos traspasables de un mismo objeto lógico.
		// Verificar si ese objeto lógico se ha insertado ya.
		objectIterator = m_oExclusionObjectFieldMap->find(vIdObject.c_str());
		if (objectIterator != m_oExclusionObjectFieldMap->end())
		{
			// Ya está insertado, actualizar sus campos lógicos.
			poFieldsMap = (*objectIterator).second;
			if (poFieldsMap != NULL)
			{
				iterator = poFieldsMap->find(vIdField.c_str());
				if (iterator == poFieldsMap->end())
				{
					iIsTransferable = atoi(vIsTransferable.c_str());
					poFieldsMap->insert(exclusionData_t::value_type(vIdField.c_str(), iIsTransferable));
					// m_oExclusionObjectFieldMap->erase(objectIterator);
					m_oExclusionObjectFieldMap->insert(exclusionObjectField_t::value_type(vIdObject.c_str(), poFieldsMap));
				}
			}
		}
		else
		{
			poFieldsMap = new exclusionData_t();
			poFieldsMap->insert(exclusionData_t::value_type(vIdField.c_str(), iIsTransferable));
			m_oExclusionObjectFieldMap->insert(exclusionObjectField_t::value_type(vIdObject.c_str(), poFieldsMap));
		}
	}

	return M4_SUCCESS;
}


//=================================================================================
//
// Cargar la configuración para objetos lógicos.
// Este método recupera la información que el usuario ha especificado en las
// tablas de configuración de objetos para las configuraciones habilitadas. 
// Estos objetos son los objetos lógicos que se considerean no comparables y 
// traspasables o no traspasables según se indique.
//
// Pasos
//	Cargar los objetos de las oonfiguraciones activas y si son
//	traspasables o no.
//	Guardar los objetos lógicos no comparables y sí traspasables en estructuras 
//	internas de la clase.
// 
//=================================================================================
m4return_t	ClMDTDictionary::_LoadObjectConfiguration()
{
	m4return_t	iResult = M4_ERROR;
	m4uint32_t	i = 0;
	m4uint32_t	iRowNumber = 0;
	m4int32_t	iIsTransferable = 1;
	M4Row		oObjectParameters;
	M4ResultSet	oObjectResultSet;
	ClMDTValue	vIsTransferable;
	ClMDTValue	vIdObject;
	
	iResult = m_pIODBC->ExecuteQuery(m_sConnection, s_csObjectConfigSelect, &oObjectParameters, M4_TRUE, oObjectResultSet);
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_LoadObjectConfiguration", "ExecuteQuery", "%s#%s" ) ;

	iRowNumber = oObjectResultSet.GetRowNumber();
	for (i = 0 ; i < iRowNumber ; i++)
	{
		iResult = oObjectResultSet.GetValue(i, s_ciIdObject_ObjectConfigPos, vIdObject);
		M4MDT_RETURN_ERROR3(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_LoadObjectConfiguration", "GetValue", "IS_OBJECT", "%s#%s - Parameter = <%s>");

		iResult = oObjectResultSet.GetValue(i, s_ciIsTransferable_ObjectConfigPos, vIsTransferable);
		M4MDT_RETURN_ERROR3(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_LoadObjectConfiguration", "GetValue", "IS_TRANSFERABLE", "%s#%s - Parameter = <%s>");

		// Todos los objetos que se han obtenido son no comparables. Para saber si son 
		// traspasables, se consulta el campo IS_TRANSFERABLE.
		iIsTransferable = atoi(vIsTransferable.c_str());
		m_oExclusionObjectMap->insert(exclusionData_t::value_type(vIdObject.c_str(), iIsTransferable));
	}

	return M4_SUCCESS;
}


//=================================================================================
//
// Obtiene la información de configuración para un determinado campo lógico de 
// un objeto lógico.
// Este método recupera la información que el usuario ha especificado en las
// tablas de configuración de objetos/ campos lógicos.
//
// Pasos
//	Buscar si el campo se encuentra dentro del mapa de campos lógicos. Si se encuentra
//	aquí, el campo es no comparable y traspasable o no según se indique.
//	Si no se encuentra el campo en el mapa de campos lógicos, se debe buscar el objeto
//	lógico dentro de la lista de objetos lógicos excluidos. Si se encuentra aquí,
//	todos los campos del mismo son no comparables, traspasables o no, según se indique.
// 
//=================================================================================
m4return_t	ClMDTDictionary::_GetFieldConfiguration(m4pcchar_t ai_pccObjectName, m4pcchar_t ai_pccFieldName, m4bool_t &ao_bIsComparable, m4bool_t &ao_IsTransferable)
{
	// Buscar el campo lógico en el mapa de objetos lógicos / campos lógicos 
	// excluídos (no comparables).
	exclusionData_t * poFieldsMap = NULL;
	exclusionObjectField_t::iterator objectIterator;
	exclusionData_t::iterator iterator;
	
	// Por defecto, el campo es comparable y traspasable. Si el objeto lógico y/o el
	// campos lógico no se encuentran dentro de los mapas de objetos lógicos y campos
	// lógicos no comparables, se considera que este campo es comparable y traspasable.
	ao_bIsComparable = M4_TRUE;
	ao_IsTransferable = M4_TRUE;

	// Buscar el objeto lógico dentro del mapa de objetos/campos lógicos excluídos 
	// (no comparables).
	objectIterator = m_oExclusionObjectFieldMap->find((m4pchar_t) ai_pccObjectName);
	if (objectIterator != m_oExclusionObjectFieldMap->end())
	{
		// Obtener el mapa de campos lógicos del objeto y verificar si el campo 
		// es comparable/traspasable.
		poFieldsMap = (*objectIterator).second;
		if (poFieldsMap != NULL)
		{
			iterator = poFieldsMap->find((m4pchar_t) ai_pccFieldName);
			if (iterator != poFieldsMap->end())
			{
				// Obtener si el campo es traspasable o no.
				ao_IsTransferable = (*iterator).second;
				
				// Como se ha encontrado el campo dentro del mapa de campos excluidos, el campo
				// es no comparable.
				ao_bIsComparable = M4_FALSE;
				
				return M4_SUCCESS;
			}
		}
	}

	// No hemos encontrado el campo en el mapa de objetos y campos lógicos excluidos. 
	// Aún así, podría encontrarse el objeto lógico dentro del mapa de objetos lógicos
	// excluidos.
	// Buscar el objeto lógico dentro del mapa de objetos lógicos excluídos (no comparables).
	// Si se encuentra el objeto lógico del campo, significa que este objeto completo 
	// (todos sus campos lógicos) es no comparable y traspasable o no, según se indique.
	iterator = m_oExclusionObjectMap->find((m4pchar_t) ai_pccObjectName);
	if (iterator != m_oExclusionObjectMap->end())
	{
		// Obtener si el campo es traspasable o no.
		ao_IsTransferable = (*iterator).second;

		// Como se ha encontrado el campo dentro del mapa de campos excluidos, el campo
		// es no comparable.
		ao_bIsComparable = M4_FALSE;
	}
	
	return M4_SUCCESS;
}

//=================================================================================
//
// Cargar los identificadores de las configuraciones activas.
// Pasos
// Cargar los identificadores de las configuraciones activas.
// Guardar los identificadores en el vector de salida.
// 
//=================================================================================
m4return_t	ClMDTDictionary::_GetEnableConfigurationIds(vector<string> &ao_vConfigId)
{
	m4return_t	iResult = M4_ERROR;
	m4uint32_t	i = 0;
	m4uint32_t	iRowNumber = 0;
	M4Row		oIdConfigParameters;
	M4ResultSet	oIdConfigResultSet;
	ClMDTValue	vIdConfig;
	
	iResult = m_pIODBC->ExecuteQuery(m_sConnection, s_csConfigurationIdsSelect, &oIdConfigParameters, M4_TRUE, oIdConfigResultSet);
	M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetEnableConfigurationIds", "ExecuteQuery", "%s#%s");

	iRowNumber = oIdConfigResultSet.GetRowNumber();
	for (i = 0 ; i < iRowNumber ; i++)
	{
		iResult = oIdConfigResultSet.GetValue(i, s_ciIdConfig_ConfigPos, vIdConfig);
		M4MDT_RETURN_ERROR3(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTDictionary::_GetEnableConfigurationIds", "GetValue", "ID_CONFIG", "%s#%s - Parameter = <%s>");

		// Guardar el identificador de la configuración.
		ao_vConfigId.push_back(vIdConfig.c_str());
	}

	return M4_SUCCESS;
}

