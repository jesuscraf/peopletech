
//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdtransfer
// File:                m4mdtsaxupdater.hpp
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
//    Este m�dulo contiene la declaraci�n de la clase del transformador
//    sax para actualizar la base de datos con el xml enriquecido
//
//
//==============================================================================


#include "m4stl.hpp"
#include "m4mdtdef.hpp"
#include "m4mdtvalue.hpp"
#include "m4xmlbase.hpp"
#include "m4mdtutils.hpp"
#include "m4imdtservice.hpp"


#ifndef __M4MDTSAXUPDATER_HPP__
#define __M4MDTSAXUPDATER_HPP__



//=================================================================================
// Declaraciones por adelantado
//=================================================================================
class M4IODBC;
class IMDTDictionary;
class IMDTPresentationTool;
class M4Row;
class ClMDTUpdateFunction;
class ClMDTSaxUpdater;




//=================================================================================
// Defines
//=================================================================================

// Estado del parseo
typedef enum {
	
	m4mdt_update_none		=	0,
	m4mdt_update_object		=	1,
	m4mdt_update_table		=	2,
	m4mdt_update_register	=	3,
	m4mdt_update_field		=	4,
	m4mdt_update_characters	=	5

} SaxUpdaterState_t;


// Vector funciones
typedef vector< ClMDTUpdateFunction* >		FunctionVector_t ;

// Vector de valores de funciones
typedef vector< ClMDTValue* >				ValuesVector_t ;

// Funci�n de update
typedef	m4return_t	(ClMDTSaxUpdater::*M4MDT_UpdateFunction_t) ( ClMDTValue* ai_avArguments ) ;






//=================================================================================
//
// ClMDTSaxUpdater
//
// Clase que implementa el transformador sax para actualizar
// Esta es la clase base que implementa la funcionalidad general de recorrido
// del fichero. A partir de esta se deben generar una clase para el borrado y otra
// para la inserci�n.
//
//=================================================================================

class	ClMDTSaxUpdater : public M4SAXHandlerBase
{

public:


//=================================================================================
// Funciones de inicializaci�n
//=================================================================================

		ClMDTSaxUpdater( void ) ;
		virtual ~ClMDTSaxUpdater( void ) ;


	//=================================================================================
	//
	// Inicializa el objeto con los objetos que usa y las variables necesarias
	//
	// @param ai_pccConnection
	//
	//		Identificador de la conexi�n de la base de datos destino
	//
	// @param ai_rsTransactionDate
	//
	//		Referencia a una cadena con la fecha de trabajo
	//
	// @param ai_rsDataFile
	//
	//		Referencia a una cadena con el fichero de datos
	//
	// @param ai_rsDebugDir
	//
	//		Referencia a una cadena con el directorio temporal
	//
	// @param ai_pIODBC
	//
	//		Referencia a un manejador de base de datos ya existente
	//
	// @param ai_pIDictionary
	//
	//		Referencia al diccionario
	//
	// @param ai_pIPresentationTool
	//
	//		Referencia a la herramienta de transformaci�n de presentaciones
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	Init( m4pcchar_t ai_pccConnection, const string& ai_rsTransactionDate, const string& ai_rsDataFile, const string& ai_rsDebugDir, M4IODBC *ai_pIODBC, IMDTDictionary *ai_pIDictionary, IMDTPresentationTool *ai_pIPresentationTool ) ;


//=================================================================================
// Funciones de actualizaci�n
//=================================================================================

	//=================================================================================
	//
	// Convierte una presentaci�n en formato OBL a sus partes
	//
	// @param ai_avArguments
	//
	//		Array de argumentos de entrada salida
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	OBL2Presentation( ClMDTValue* ai_avArguments ) ;

	//=================================================================================
	//
	// Obtiene la fecha actual que es la fecha de comienzo de transacci�n
	//
	// @param ai_avArguments
	//
	//		Array de argumentos de entrada salida
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	Today( ClMDTValue* ai_avArguments ) ;

	//=================================================================================
	//
	// Genera un campo blob en formato de base de datos a partir de sus partes
	//
	// @param ai_avArguments
	//
	//		Array de argumentos de entrada salida
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	File2BLOB( ClMDTValue* ai_avArguments ) ;

	//=================================================================================
	//
	// Genera un campo blob en formato de base de datos a partir de sus partes
	// como fichero externo
	//
	// @param ai_avArguments
	//
	//		Array de argumentos de entrada salida
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	External2BLOB( ClMDTValue* ai_avArguments ) ;

	//=================================================================================
	//
	// Asigna el valor de entrada al valor de salida
	//
	// @param ai_avArguments
	//
	//		Array de argumentos de entrada salida
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	Value( ClMDTValue* ai_avArguments ) ;


	//=================================================================================
	//
	// Cambia los retornos de carro \n por \r\n en el valor de entrada
	//
	// @param ai_avArguments
	//
	//		Array de argumentos de entrada salida
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	NormalizeReturns( ClMDTValue* ai_avArguments ) ;


	//=================================================================================
	//
	// Genera un fichero a partir de un campo package de las presentaciones de base de
	// datos.
	//
	// @param ai_avArguments
	//
	//		Array de argumentos de entrada salida
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	External2PresentationPackage( ClMDTValue* ai_avArguments );


	//=================================================================================
	//
	// Genera un fichero a partir de un campo de base de datos.
	//
	// @param ai_avArguments
	//
	//		Array de argumentos de entrada salida
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	File2Field( ClMDTValue* ai_avArguments );



//=================================================================================
// Funciones de procesamiento de elementos
//=================================================================================

	//=================================================================================
	//
	// Procesa un elemento de tipo CDATA
	//
	// @param ai_pccText
	//
	//		Texto del elemento
	//
	// @param ai_iLength
	//
	//		Longitud del texto
	//
	//=================================================================================
	void	characters( const XMLCh* const ai_pccText, const XMLSize_t ai_iLength ) ;


protected:


	//=================================================================================
	//
	// Procesa el comienzo del elemento <object-data>
	//
	// @param ai_pccName
	//
	//		Identificador del elemento que se est� procesando
	//
	// @param ai_roAttributes
	//
	//		Lista de atributos del elemento que se est� procesando
	//
	//=================================================================================
	m4void_t	_ProcObject( const XMLCh* const ai_pccName, AttributeList& ai_roAttributes ) ;

	//=================================================================================
	//
	// Procesa el comienzo del elemento <table>
	//
	// @param ai_pccName
	//
	//		Identificador del elemento que se est� procesando
	//
	// @param ai_roAttributes
	//
	//		Lista de atributos del elemento que se est� procesando
	//
	//=================================================================================
	m4void_t	_ProcTable( const XMLCh* const ai_pccName, AttributeList& ai_roAttributes ) ;

	//=================================================================================
	//
	// Procesa el comienzo del elemento <register>
	//
	// @param ai_pccName
	//
	//		Identificador del elemento que se est� procesando
	//
	// @param ai_roAttributes
	//
	//		Lista de atributos del elemento que se est� procesando
	//
	//=================================================================================
	m4void_t	_ProcRegister( const XMLCh* const ai_pccName, AttributeList& ai_roAttributes ) ;

	//=================================================================================
	//
	// Procesa el comienzo del elemento <field>
	//
	// @param ai_pccName
	//
	//		Identificador del elemento que se est� procesando
	//
	// @param ai_roAttributes
	//
	//		Lista de atributos del elemento que se est� procesando
	//
	//=================================================================================
	m4void_t	_ProcField( const XMLCh* const ai_pccName, AttributeList& ai_roAttributes ) ;

	//=================================================================================
	//
	// Procesa el final del elemento </field>
	//
	// @param ai_pccName
	//
	//		Identificador del elemento que se acaba de procesar
	//
	//=================================================================================
	m4void_t	_ProcEndField( const XMLCh* const ai_pccName ) ;

	//=================================================================================
	//
	// Procesa el final del elemento </register>
	//
	// @param ai_pccName
	//
	//		Identificador del elemento que se acaba de procesar
	//
	//=================================================================================
	m4void_t	_ProcEndRegister( const XMLCh* const ai_pccName ) ;

	//=================================================================================
	//
	// Procesa el final del elemento </table>
	//
	// @param ai_pccName
	//
	//		Identificador del elemento que se acaba de procesar
	//
	//=================================================================================
	m4void_t	_ProcEndTable( const XMLCh* const ai_pccName ) ;

	//=================================================================================
	//
	// Procesa el final del elemento </object-data>
	//
	// @param ai_pccName
	//
	//		Identificador del elemento que se acaba de procesar
	//
	//=================================================================================
	m4void_t	_ProcEndObject( const XMLCh* const ai_pccName ) ;



//=================================================================================
// Funciones auxiliares
//=================================================================================

	//=================================================================================
	//
	// Inicializa los datos necesarios para utilizar una tabla
	// Carga el diccionario y procesa la definici�n de la funci�n de actualizaci�n
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	_InitTable( void ) ;

	//=================================================================================
	//
	// Inicializa los datos necesarios para utilizar un registro
	// Crea los par�metros de la funci�n de actualizaci�n y las filas de par�metros
	// de la sentencia
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	_InitRegister( void ) ;

	//=================================================================================
	//
	// A�ade el valor de un campo a su correspondiente M4Row
	//
	// @param ai_pccField
	//
	//		Campo a establecer el valor
	//
	// @param ai_rvValue
	//
	//		Valor del campo
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	_SetValue( m4pcchar_t ai_pccField, ClMDTValue &ai_rvValue ) ;

	//=================================================================================
	//
	// Obtiene el valor de un campo en su correspondiente M4Row
	//
	// @param ai_pccField
	//
	//		Campo a obtener el valor
	//
	// @param ao_rvValue
	//
	//		Valor del campo
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	_GetValue(m4pcchar_t ai_pccField, ClMDTValue &ao_rvValue);
	
		
	//=================================================================================
	//
	// Realiza las transformaciones de la funci�n de actualizaci�n si la hay
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	_DoTransformations( void ) ;

	//=================================================================================
	//
	// Ejecuta la sentencia en la base de datos
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	_Execute( void ) ;

	//=================================================================================
	//
	// Libera la memoria asociada a un registro
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	_EndRegister( void ) ;

	//=================================================================================
	//
	// Libera la memoria asociada a una tabla
	//
	// @return
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	_EndTable( void ) ;



//=================================================================================
// Funciones virtuales
//=================================================================================

	//=================================================================================
	//
	// Devuelve el nombre del atributo que identifica la sentencia a ejecutar
	//
	// @return
	//		El nombre del atributo que identifica la sentencia a ejecutar
	//
	//=================================================================================
	virtual const XMLCh*	_GetSentenceName( void ) const = 0 ;

	//=================================================================================
	//
	// Devuelve el nombre del atributo que identifica el n�mero de par�metros
	// de la sentencia
	//
	// @return
	//		El nombre del atributo que identifica el n�mero de par�metros
	//
	//=================================================================================
	virtual const XMLCh*	_GetParameterNumberName( void ) const = 0 ;

	//=================================================================================
	//
	// Devuelve el nombre del atributo que identifica la posici�n del par�metro en 
	// la sentencia
	//
	// @return
	//		El nombre del atributo que identifica la posici�n del par�metro
	//
	//=================================================================================
	virtual const XMLCh*	_GetParameterPositionName( void ) const = 0 ;

	//=================================================================================
	//
	// Devuelve true si hay que aplicar las funciones de transformaci�n
	//
	// @return
	//		M4_TRUE si hay que aplicar transformaciones y M4_FALSE sino
	//
	//=================================================================================
	virtual m4bool_t	_HasToDoTransformations( void ) const = 0 ;



//=================================================================================
// Miembros
//=================================================================================

	//=================================================================================
	// Constante para el comienzo de un CDATA
	//=================================================================================
	XMLCh					*m_wsCDATAPrefix ;

	//=================================================================================
	// Estado de procesamiento
	//=================================================================================
	SaxUpdaterState_t		m_eState ;

	//=================================================================================
	// Fecha de la transacci�n
	//=================================================================================
	ClMDTValue				m_vTansactionDate ;

	//=================================================================================
	// Fichero de datos
	//=================================================================================
	string					m_sDataFile ;

	//=================================================================================
	// Directorio temporal
	//=================================================================================
	string					m_sDebugDir ;

	//=================================================================================
	// Objeto traspasable que se est� procesando
	//=================================================================================
	ClCharHolder			m_pcObject ;

	//=================================================================================
	// Tabla que se est� procesando
	//=================================================================================
	ClCharHolder			m_pcTable ;

	//=================================================================================
	// Texto de las funciones de update de la tabla que se est� procesando
	//=================================================================================
	ClCharHolder			m_pcUpdateFunctions ;

	//=================================================================================
	// Campo que se est� procesando
	//=================================================================================
	ClCharHolder			m_pcField ;

	//=================================================================================
	// Flag de si el valor que se est� procesando es multilinea o no
	//=================================================================================
	m4bool_t				m_bIsMultiline ;

	//=================================================================================
	// Valor que se est� procesando
	//=================================================================================
	ClMDTValue				m_vValue ;

	//=================================================================================
	// Funciones de actualizaci�n de la tabla que se est� procesando
	//=================================================================================
	FunctionVector_t		m_vUpdateFunctions ;

	//=================================================================================
	// Lista de los campos del diccionario de la tabla que se est� procesando
	//=================================================================================
	ClMDTDomTraverser		m_oFields ;

	//=================================================================================
	// Lista de las tablas f�sicas del diccionario de la tabla que se est� procesando
	//=================================================================================
	ClMDTDomTraverser		m_oTables ;

	//=================================================================================
	// Conjunto de valores de los argumentos de las funciones de update del registro
	// que se est� procesando
	//=================================================================================
	ValuesVector_t			m_vUpdateValues ;

	//=================================================================================
	// Array de par�metros de la sentencia de una tabla l�gica
	//=================================================================================
	M4Row					*m_poRows ;

	//=================================================================================
	// Conexi�n de la base de datos destino
	//=================================================================================
	string					m_sConnection ;

	//=================================================================================
	// Interfaz para acceder a la base de datos
	//=================================================================================
	M4IODBC					*m_pIODBC ;

	//=================================================================================
	// Interfaz para acceder al diccionario
	//=================================================================================
	IMDTDictionary			*m_pIDictionary ;

	//=================================================================================
	// Interfaz para la tool de presentaciones
	//=================================================================================
	IMDTPresentationTool	*m_pIPresentationTool ;

} ;





//=================================================================================
//
// ClMDTSaxDeleter
//
// Clase que implementa el transformador sax para borrar registros
//
//=================================================================================

class	ClMDTSaxDeleter : public ClMDTSaxUpdater
{

public:


//=================================================================================
// Funciones de inicializaci�n
//=================================================================================

		ClMDTSaxDeleter( void ) {} ;
		virtual ~ClMDTSaxDeleter( void ) {} ;


protected:

//=================================================================================
// Funciones virtuales
//=================================================================================

	//=================================================================================
	// Devuelve el nombre del atributo que identifica la sentencia a ejecutar
	// Los par�metros est�n estecificados en la clase base
	//=================================================================================
	virtual const XMLCh*	_GetSentenceName( void ) const
	{
		return( IMDTService::sm_sXMLChDelete ) ;
	}

	//=================================================================================
	// Devuelve el nombre del atributo que identifica el n�mero de par�metros
	// de la sentencia
	// Los par�metros est�n estecificados en la clase base
	//=================================================================================
	virtual const XMLCh*	_GetParameterNumberName( void ) const
	{
		return( IMDTService::sm_sXMLChDeleteNumber ) ;
	}

	//=================================================================================
	// Devuelve el nombre del atributo que identifica la posici�n del par�metro en 
	// la sentencia
	// Los par�metros est�n estecificados en la clase base
	//=================================================================================
	virtual const XMLCh*	_GetParameterPositionName( void ) const
	{
		return( IMDTService::sm_sXMLChPkPosition ) ;
	}

	//=================================================================================
	// Devuelve true si hay que aplicar las funciones de transformaci�n
	// Los par�metros est�n estecificados en la clase base
	//=================================================================================
	virtual m4bool_t	_HasToDoTransformations( void ) const
	{
		return( M4_FALSE ) ;
	}

} ;




//=================================================================================
//
// ClMDTSaxInserter
//
// Clase que implementa el transformador sax para insertar registros
//
//=================================================================================

class	ClMDTSaxInserter : public ClMDTSaxUpdater
{

public:


//=================================================================================
// Funciones de inicializaci�n
//=================================================================================

		ClMDTSaxInserter( void ) {} ;
		virtual ~ClMDTSaxInserter( void ) {} ;



protected:

//=================================================================================
// Funciones virtuales
//=================================================================================

	//=================================================================================
	// Devuelve el nombre del atributo que identifica la sentencia a ejecutar
	// Los par�metros est�n estecificados en la clase base
	//=================================================================================
	virtual const XMLCh*	_GetSentenceName( void ) const
	{
		return( IMDTService::sm_sXMLChInsert ) ;
	}

	//=================================================================================
	// Devuelve el nombre del atributo que identifica el n�mero de par�metros
	// de la sentencia
	// Los par�metros est�n estecificados en la clase base
	//=================================================================================
	virtual const XMLCh*	_GetParameterNumberName( void ) const
	{
		return( IMDTService::sm_sXMLChInsertNumber ) ;
	}

	//=================================================================================
	// Devuelve el nombre del atributo que identifica la posici�n del par�metro en 
	// la sentencia
	// Los par�metros est�n estecificados en la clase base
	//=================================================================================
	virtual const XMLCh*	_GetParameterPositionName( void ) const
	{
		return( IMDTService::sm_sXMLChFieldPosition ) ;
	}

	//=================================================================================
	// Devuelve true si hay que aplicar las funciones de transformaci�n
	// Los par�metros est�n estecificados en la clase base
	//=================================================================================
	virtual m4bool_t	_HasToDoTransformations( void ) const
	{
		return( M4_TRUE ) ;
	}

} ;





//=================================================================================
//
// ClMDTUpdateFunction
//
// Clase que encapsula la definici�n de una funci�n de actualizaci�n
//
//=================================================================================

class	ClMDTUpdateFunction
{

public:


//=================================================================================
// Funciones de inicializaci�n
//=================================================================================

		ClMDTUpdateFunction( void ) ;
		virtual ~ClMDTUpdateFunction( void ) ;

	//=================================================================================
	//
	// Inicializa la definici�n de la funci�n
	//
	// @param ai_pfFunction
	//
	//		Puntero a la funci�n de actualizaci�n
	//
	// @param ai_accArguments
	//
	//		Array con la definici�n de los nombres de los argumentos
	//
	// @param ai_accNames
	//
	//		Array con los nombres actuales de los argumentos
	//
	// @param ai_accValues
	//
	//		Array con los valores para cada nombre de los argumentos
	//
	// @param ai_iParameterNumber
	//
	//		N�mero de argumentos de la funci�n
	//
	// @param ai_pccName
	//
	//		Nombre de la funci�n. Se emplea en los mensajes de error
	//
	// @param ai_pccTable
	//
	//		Nombre de la tabla que se est� procesando. Se emplea en los mensajes
	//			de error
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	Init( M4MDT_UpdateFunction_t ai_pfFunction, const m4char_t ai_accArguments[][ M4MDT_MAX_UPDATE_FUNCTION_ARGUMENT_SIZE ], const m4char_t ai_accNames[][ M4MDT_MAX_UPDATE_FUNCTION_ARGUMENT_SIZE ], const m4char_t ai_accValues[][ M4MDT_MAX_UPDATE_FUNCTION_ARGUMENT_SIZE ], m4uint16_t ai_iParameterNumber, m4pcchar_t ai_pccName, m4pcchar_t ai_pccTable ) ;



//=================================================================================
// Otras funciones
//=================================================================================

	//=================================================================================
	//
	// Busca un par�metro entre los argumentos de la funci�n
	//
	// @param ai_pccValue
	//
	//		Valor que hay que buscar
	//
	// @return
	//
	//		Posici�n de nombre en el array o -1 si no est�
	//
	//=================================================================================
	m4int16_t	FindParameterByValue( m4pcchar_t ai_pccValue ) ;

	//=================================================================================
	//
	// Obtiene el n�mero de par�metros de la funci�n
	//
	// @return
	//
	//		El n�mero de par�metros de la funci�n
	//
	//=================================================================================
	m4int16_t	GetParameterNumber( void ) const ;

	//=================================================================================
	//
	// Obtiene un par�metros de la funci�n por posici�n
	//
	// @param ai_iParameter
	//
	//		Posici�n del par�metro
	//
	// @return
	//
	//		Referencia al par�metro pedido o NULL si no se puede
	//
	//=================================================================================
	m4pcchar_t	GetParameter( m4int16_t ai_iParameter ) const ;

	//=================================================================================
	//
	// Ejecuta la funci�n de actualizaci�n
	//
	// @param ai_acvValues
	//
	//		Valores actuales de los argumentos de la funci�n
	//
	// @param ao_poUpdater
	//
	//		El objeto updater sobre el que se ejecuta esta funci�n
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	Execute( ClMDTValue *ai_acvValues, ClMDTSaxUpdater *ao_poUpdater ) const ;


protected:

	//=================================================================================
	// Puntero a la funci�n de update de la tabla que se est� procesando
	//=================================================================================
	M4MDT_UpdateFunction_t	m_pfFunction ;

	//=================================================================================
	// Definici�n de los argumentos de la funci�n de update
	//=================================================================================
	string					*m_asParameters ;

	//=================================================================================
	// N�mero de argumentos de la funci�n de update
	//=================================================================================
	m4uint16_t				m_iParameterNumber ;
} ;




#endif


