
//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdtransfer
// File:                m4mdtdictionary.hpp
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
//    Este módulo contiene la declaración de la clase del diccionario de la
//    librería m4mdtransfer
//
//
//==============================================================================


#include "m4stl.hpp"
#include "m4imdtdictionary.hpp"
#include "m4mdtdef.hpp"
#include "m4mdtvalue.hpp"
#include "cldates.hpp"

#include <stdio.h>

#include "xercesc/parsers/XercesDOMParser.hpp"


#ifndef __M4MDTDICTIONARY_HPP__
#define __M4MDTDICTIONARY_HPP__

//=================================================================================
// Declaraciones por adelantado
//=================================================================================
class M4IODBC;
class M4IReadResultSet;
class M4ResultSet;
class ClMDTCacheDOM;

XERCES_CPP_NAMESPACE_USE



//=================================================================================
// Defines
//=================================================================================

//=================================================================================
// Función de ordenación de nombres de elementos cacheables
//=================================================================================

struct CacheDOMNameLess : public binary_function<m4pchar_t, m4pchar_t, bool>
{
    bool operator()(const m4pchar_t &x, const m4pchar_t &y) const
	{
		return( strcmp( x, y ) < 0 ) ;
	}
} ;


//=================================================================================
// Mapa de documentos cacheables
//=================================================================================

typedef map< m4pchar_t, ClMDTCacheDOM*, CacheDOMNameLess > M4MDTCacheDomMap_t ;

//=================================================================================
// Mapa de objetos lógicos excluidos (no comparables).
//=================================================================================
typedef map< string, m4bool_t > exclusionData_t;

//=================================================================================
// Mapa de objetos lógicos y sus campos lógicos excluidos (no comparables).
//=================================================================================
typedef map< string, exclusionData_t * > exclusionObjectField_t;



//=================================================================================
// Clases auxiliares
//=================================================================================

//=================================================================================
//
// ClMDTCacheDOM
//
// Clase que contine las características de un Documento DOM cacheable
//
//=================================================================================
class	ClMDTCacheDOM
{

public:

//=================================================================================
// Funciones de inicialización
//=================================================================================

		//=================================================================================
		// Constructor
		// Toma la referencia del documento e inicializa la fecha
		//=================================================================================
		ClMDTCacheDOM( InputSource* ai_poSource )
		{
			m_oParser.parse( *ai_poSource ) ;

			m_poDocument = m_oParser.getDocument() ;
			m_dLastAccessed = ClActualDate() ;
		}

		//=================================================================================
		// Destructor
		// Libera el documento
		//=================================================================================
		virtual ~ClMDTCacheDOM( void )
		{
			m_poDocument = NULL ;
			m_dLastAccessed = 0 ;
		}


//=================================================================================
// Miembros
//=================================================================================

	//=================================================================================
	// Parser
	//=================================================================================
	XercesDOMParser			m_oParser ;

	//=================================================================================
	// Documento
	//=================================================================================
	xercesc::DOMDocument*	m_poDocument ;

	//=================================================================================
	// Fecha de último uso
	//=================================================================================
	m4date_t				m_dLastAccessed ;

} ;





//=================================================================================
//
// ClMDTDictionary
//
// Clase que implementa el diccionario
//
//=================================================================================

class	ClMDTDictionary : public IMDTDictionary
{

public:


//=================================================================================
// Funciones de inicialización
//=================================================================================

		ClMDTDictionary( void ) ;
		virtual ~ClMDTDictionary( void ) ;


	//=================================================================================
	//
	// Inicializa el objeto con los objetos que usa y las variables necesarias
	//
	// @param ai_iLanguage
	//
	//		Lenguaje de trabajo
	//
	// @param ai_pccConnection
	//
	//		Identificador de la conexión del diccionario
	//
	// @param ai_pccCacheDir
	//
	//		Directorio de trabajo con permisos de escritura donde se
	//			dejarán los XML cacheados
	//
	// @param ai_iVersion
	//
	//		Versión del repositorio del RAMDL
	//
	// @param ai_pIODBC
	//
	//		Referencia a un manejador de base de datos ya existente
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	Init( m4uint32_t ai_iLanguage, m4pcchar_t ai_pccConnection, m4pcchar_t ai_pccCacheDir, m4uint32_t ai_iVersion, M4IODBC *ai_pIODBC ) ;


//=================================================================================
// Funciones del interfaz
//=================================================================================

	//=================================================================================
	// Obtiene el XML de un objeto traspasable del diccionario de RAMDL
	// Los parámetros están especificados en el interfaz
	//=================================================================================
	virtual	m4return_t	GetXMLObject( m4pcchar_t ai_pccObject, InputSource* &ao_rpoSource ) ;


	//=================================================================================
	// Obtiene el XML de una tabla del diccionario de Meta4
	// Los parámetros están especificados en el interfaz
	//=================================================================================
	virtual	m4return_t	GetXMLTable( m4pcchar_t ai_pccTable, InputSource* &ao_rpoSource ) ;


	//=================================================================================
	// Libera la fuente de lectura asociada a un objeto, tanto del diccionario
	// de RAMDL como del de Meta4. Pone a nulo el objeto
	// Los parámetros están especificados en el interfaz
	//=================================================================================
	virtual	m4return_t	ReleaseXMLObject( InputSource* &aio_rpoSource ) ;


	//=================================================================================
	// Obtiene el DOM de una tabla del diccionario de Meta4
	// Los parámetros están especificados en el interfaz
	//=================================================================================
	virtual	m4return_t	GetDOMTable( m4pcchar_t ai_pccTable, xercesc::DOMDocument* &ao_rpoDocument ) ;


	//=================================================================================
	// Obtiene la configuración de un campo de un objeto lógico. Se devuelve
	// si el campo es comparable y si es traspasable.
	// Los parámetros están especificados en el interfaz
	//=================================================================================
	virtual m4return_t	GetFieldConfiguration(m4pcchar_t ai_pccObjectName, m4pcchar_t ai_pccFieldName, m4bool_t &ao_bIsComparable, m4bool_t &ao_IsTransferable);

	//=================================================================================
	// Obtiene los identificadores de las configuraciones activas.
	// Este método recupera los identificadores de las configuraciones que el usuario 
	// ha especificado en la tabla de configuraciones.
	// Los parámetros están especificados en el interfaz.
	//=================================================================================
	virtual m4return_t	GetEnableConfigurationIds(vector<string> &ao_vConfigId);


protected:

//=================================================================================
// Funciones auxiliares
//=================================================================================

	//=================================================================================
	//
	// Realiza las tareas de mantenimiento de la caché en RAM de documentos DOM
	// Deja espacio suficiente para añadir un nuevo elemento
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	_PurgeDOMCache( void ) ;


	//=================================================================================
	//
	// Genera el fichero XML de un objeto traspasable del diccionario de RAMDL
	// a partir de su definición de base de datos
	//
	// @param ai_iObject
	//
	//		Identificador del objeto traspasable
	//
	// @param ao_pfFile
	//
	//		Fichero donde se debe dejar el XML generado
	//
	// @param ai_pccFileName
	//
	//		Nombre del fichero donde se debe dejar el XML generado
	//		Se utiliza en los mensajes de error
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	_GetObjectFromDatabase( m4pcchar_t ai_pccObject, FILE *ao_pfFile, m4pcchar_t ai_pccFileName ) ;

	//=================================================================================
	//
	// Genera el fichero XML de una tabla del diccionario de Meta4
	// a partir de su definición de base de datos
	//
	// @param ai_pccTable
	//
	//		Identificador de la tabla
	//
	// @param ao_pfFile
	//
	//		Fichero donde se debe dejar el XML generado
	//
	// @param ai_pccFileName
	//
	//		Nombre del fichero donde se debe dejar el XML generado
	//		Se utiliza en los mensajes de error
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	_GetTableFromDatabase( m4pcchar_t ai_pccTable, FILE *ao_pfFile, m4pcchar_t ai_pccFileName ) ;

	//=================================================================================
	//
	// Genera la parte de fichero XML de los campos una tabla del diccionario de Meta4
	// a partir de su definición de base de datos
	//
	// @param ai_pccTable
	//
	//		Identificador de la tabla
	//
	// @param ao_pfFile
	//
	//		Fichero donde se debe dejar el XML generado
	//
	// @param ai_pccFileName
	//
	//		Nombre del fichero donde se debe dejar el XML generado
	//		Se utiliza en los mensajes de error
	//
	// @param ao_roResultSet
	//
	//		Parámetro de salida con el resulset de los campos de la tabla
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	_GetTableFieldsFromDatabase( m4pcchar_t ai_pccTable, FILE *ao_pfFile, m4pcchar_t ai_pccFileName, M4ResultSet &ao_roResultSet ) ;

	//=================================================================================
	//
	// Genera la parte de fichero XML de las tablas físicas una tabla del diccionario
	// de Meta4 a partir de su definición de base de datos
	//
	// @param ai_rcoResultSet
	//
	//		Parámetro de entrada con el resulset de los campos de la tabla
	//
	// @param ao_pfFile
	//
	//		Fichero donde se debe dejar el XML generado
	//
	// @param ai_pccFileName
	//
	//		Nombre del fichero donde se debe dejar el XML generado
	//		Se utiliza en los mensajes de error
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	_GetTableObjectsFromDatabase( const M4IReadResultSet &ai_rcoResultSet, FILE *ao_pfFile, m4pcchar_t ai_pccFileName ) ;

	//=================================================================================
	//
	// Genera la parte de fichero XML de los lookups de una tabla del diccionario de
	// Meta4 a partir de su definición de base de datos
	//
	// @param ai_pccTable
	//
	//		Identificador de la tabla
	//
	// @param ao_pfFile
	//
	//		Fichero donde se debe dejar el XML generado
	//
	// @param ai_pccFileName
	//
	//		Nombre del fichero donde se debe dejar el XML generado
	//		Se utiliza en los mensajes de error
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	_GetTableLookupsFromDatabase( m4pcchar_t ai_pccTable, FILE *ao_pfFile, m4pcchar_t ai_pccFileName ) ;

	//=================================================================================
	//
	// Obtiene el nombre del campo físico de un campo lógico
	// Tiene en cuenta si el campo es traducible
	//
	// @param ai_rcvObject
	//
	//		Objeto lógico
	//
	// @param ai_rcvField
	//
	//		Campo lógico
	//
	// @param ai_rvRealField
	//
	//		Variable de salida donde se deja el campo físico
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	_GetRealField( const ClMDTValue& ai_rcvObject, const ClMDTValue& ai_rcvField, ClMDTValue& ai_rvRealField ) ;

	//=================================================================================
	//
	// Genera la parte de fichero XML de un lookup de un campo de una tabla del
	// diccionario de Meta4 a partir de su definición de base de datos
	//
	// @param ai_rcvField
	//
	//		Nombre del campo lógico
	//
	// @param ai_rcvRealObject
	//
	//		Nombre de la tabla física
	//
	// @param ai_rcvValueRealField
	//
	//		Nombre del campo físico del valor
	//
	// @param ai_rcvCaptionRealField
	//
	//		Nombre del campo físico del valor traducido
	//
	// @param ao_pfFile
	//
	//		Fichero donde se debe dejar el XML generado
	//
	// @param ai_pccFileName
	//
	//		Nombre del fichero donde se debe dejar el XML generado
	//		Se utiliza en los mensajes de error
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	_GetTableLookupFromDatabase( const ClMDTValue& ai_rcvField, const ClMDTValue& ai_rcvRealObject, const ClMDTValue& ai_rcvValueRealField, const ClMDTValue& ai_rcvCaptionRealField, FILE *ao_pfFile, m4pcchar_t ai_pccFileName ) ;


	//=================================================================================
	//
	// Carga la configuración para de los campos lógicos.
	// Este método recupera la información que el usuario ha especificado en las
	// tablas de configuración de campos lógicos para las configuraciones habilitadas. 
	// Estos campos son los campos lógicos que se consideran no comparables y 
	// traspasables o no traspasables según se indique.
	// 
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	_LoadObjectConfiguration();

	//=================================================================================
	//
	// Carga la configuración para de los campos lógicos.
	// Este método recupera la información que el usuario ha especificado en las
	// tablas de configuración de campos lógicos para las configuraciones habilitadas. 
	// Estos campos son los campos lógicos que se consideran no comparables y 
	// traspasables o no traspasables según se indique.
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	_LoadFieldConfiguration();

	//=================================================================================
	// Obtiene la información de configuración para un determinado campo lógico de 
	// un objeto lógico.
	// Este método recupera la información que el usuario ha especificado en las
	// tablas de configuración de objetos/ campos lógicos.
	// 
	// @param ai_pccObjectName
	//
	//		Nombre de objeto lógico
	//
	// @param ai_pccFieldName
	//
	//		Nombre de campo lógico
	//
	// @param ao_bIsComparable
	//
	//		Parámetro de salida que indica si el campos es comparable.
	//
	// @param ao_IsTransferable
	//
	//		Parámetro de salida que indica si el campos es traspasable.
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	_GetFieldConfiguration(m4pcchar_t ai_pccObjectName, m4pcchar_t ai_pccFieldName, m4bool_t &ao_bIsComparable, m4bool_t &ao_IsTransferable);

	
	//=================================================================================
	// Obtiene los identificadores de las configuraciones activas.
	// Este método recupera los identificadores de las configuraciones que el usuario 
	// ha especificado en la tabla de configuraciones.
	// 
	// @param ao_vIdConfig
	//
	//		Parámetro de salida que contiene los identificadores de las configuraciones.
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	_GetEnableConfigurationIds(vector<string> &ao_vConfigId);

//=================================================================================
// Miembros
//=================================================================================

	//=================================================================================
	// Lenguaje de trabajo
	//=================================================================================
	m4uint32_t	m_iLanguage ;

	//=================================================================================
	// Conexión del diccionario
	//=================================================================================
	string		m_sConnection ;

	//=================================================================================
	// Directorio de trabajo para manejo de ficheros XML
	//=================================================================================
	string		m_sCacheDir ;

	//=================================================================================
	// Versión del repositorio del RAMDL
	//=================================================================================
	ClMDTValue	m_vVersion ;

	//=================================================================================
	// Interfaz para acceder a la base de datos
	//=================================================================================
	M4IODBC		*m_pIODBC ;

	//=================================================================================
	// Mapa de objetos cacheables
	//=================================================================================
	M4MDTCacheDomMap_t	m_oCacheDomMap ;

	//=================================================================================
	// Mapa de objetos lógicos excluidos del proceso de comparación, traspasables o no 
	// traspasables.
	//=================================================================================
	exclusionData_t * m_oExclusionObjectMap;
	
	//=================================================================================
	// Mapa de objetos lógicos y campos lógicos excluidos del proceso de comparación, 
	// traspasables o no traspasables.
	//=================================================================================
	exclusionObjectField_t * m_oExclusionObjectFieldMap;

} ;

#endif


