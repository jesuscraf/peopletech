
//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdtransfer
// File:                m4mdtservice.hpp
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
//    Este módulo contiene la declaración de la clase del servicio de la
//    librería m4mdtransfer
//
//==============================================================================


#include "m4mdtransfer_dll.hpp"
#include "m4imdtservice.hpp"

#include "m4mdtparameter.hpp"
#include "m4mdterror.hpp"
#include "m4dbtypes.hpp"




#ifndef __M4MDTSERVICE_HPP__
#define __M4MDTSERVICE_HPP__



//=================================================================================
// Declaraciones por adelantado
//=================================================================================
class M4ODBC;
class ClMDTPresentationTool;
class ClMDTDictionary;
class ClMDTTransformer;



//=================================================================================
//
// ClMDTService
//
// Clase que implementa el gestor de servicios
// Esta clase sirve para controlar el ciclo de vida de los objetos que se
// crean en la librería
//
//=================================================================================

class	M4_DECL_M4MDTRANSFER	ClMDTService: public IMDTService
{

public:


//=================================================================================
// Funciones de inicialización
//=================================================================================

		ClMDTService( void ) ;
		virtual ~ClMDTService( void ) ;


	//=================================================================================
	//
	// Inicializa los objetos de la librería
	//
	// @param ai_iLanguage
	//
	//		Lenguaje de trabajo. Español o inglés
	//
	// @param ai_iConnections
	//
	//		Handle del array de conexiones.
	//		Son parejas de nombre de conexión-DSN de conexión
	//
	// @param ai_pccDictConnection
	//
	//		DSN de la conexión del diccionario
	//
	// @param ai_eDictDBType
	//
	//		Tipo de base de datos de la conexión del diccionario
	//
	// @param ai_pccWorkingDir
	//
	//		Directorio de trabajo
	//
	// @param ai_pccDebugDir
	//
	//		Directorio de depuración
	//
	// @param ai_pccCacheDir
	//
	//		Directorio de caches
	//
	// @param ai_iVersion
	//
	//		Versión del repositorio de RAMDL
	//
	// @param ai_iTraceLevel
	//
	//		Nivel de traza
	//
	// @return
	//
	//			M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	Init( m4uint32_t ai_iLanguage, m4int32_t ai_iConnections, m4pcchar_t ai_pccDictConnection, eDDBBType_t ai_eDictDBType, m4pcchar_t ai_pccWorkingDir, m4pcchar_t ai_pccDebugDir, m4pcchar_t ai_pccCacheDir, m4uint32_t ai_iVersion, m4uint32_t ai_iTraceLevel ) ;

	//=================================================================================
	//
	// Finaliza los objetos de la librería
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	End( void ) ;

	//=================================================================================
	// Devuelve si la librería ha sido inicializada o no
	// 
	// @return
	//
	//		M4_TRUE si la librería ha sido inicializada o M4_FALSE sino
	//
	//=================================================================================
	m4bool_t	IsInitialised( void ) const ;



//=================================================================================
// Funciones del interfaz
//=================================================================================

	//=================================================================================
	// Obtiene la fecha de la sesión
	// Los parámetros están especificados en el interfaz
	//=================================================================================
	virtual	const string&	GetSessionDate( void ) const ;

	//=================================================================================
	// Obtiene el directorio temporal
	// Los parámetros están especificados en el interfaz
	//=================================================================================
	virtual	const string&	GetDebugDir( void ) const ;

	//=================================================================================
	// Obtiene el manejador de parámetros de lectura
	// Los parámetros están especificados en el interfaz
	//=================================================================================
	virtual const IMDTReadParameterManager*	GetIReadParameterManager( void ) const ;

	//=================================================================================
	// Obtiene el manejador de parámetros de escritura
	// Los parámetros están especificados en el interfaz
	//=================================================================================
	virtual IMDTWriteParameterManager*	GetIWriteParameterManager( void ) ;

	//=================================================================================
	// Obtiene el manejador de parámetros de lectura escritura escritura
	// Los parámetros están especificados en el interfaz
	//=================================================================================
	virtual IMDTReadWriteParameterManager*	GetIReadWriteParameterManager( void ) ;

	//=================================================================================
	// Obtiene la tool de presentaciones
	// Los parámetros están especificados en el interfaz
	//=================================================================================
	virtual IMDTPresentationTool*	GetIPresentationTool( void ) ;

	//=================================================================================
	// Obtiene el manejador de base de datos
	// Los parámetros están especificados en el interfaz
	//=================================================================================
	virtual M4IODBC*	GetIODBC( void ) ;

	//=================================================================================
	// Obtiene el diccionario
	// Los parámetros están especificados en el interfaz
	//=================================================================================
	virtual IMDTDictionary*	GetIDictionary( void ) ;

	//=================================================================================
	// Obtiene el transformador
	// Los parámetros están especificados en el interfaz
	//=================================================================================
	virtual IMDTTransformer*	GetITransformer( void ) ;


protected:

//=================================================================================
// Funciones
//=================================================================================

	//=================================================================================
	// Inicializa el core XML
	// Devuelve un error en caso de excepcion.
	//
	// @return
	//
	//			M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//=================================================================================

	m4return_t ClMDTService::InitXML( void );

	//=================================================================================
	// Termina el core XML
	// Devuelve un error en caso de excepcion.
	//
	// @return
	//
	//			M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//=================================================================================

	m4return_t ClMDTService::EndXML( void );

//=================================================================================
// Miembros
//=================================================================================

	//=================================================================================
	// Indicador de inicialización de la librería
	//=================================================================================
	m4bool_t				m_bInitialised ;

	//=================================================================================
	// Fecha de la sessión
	//=================================================================================
	string					m_sSessionDate ;

	//=================================================================================
	// Directorio temporal
	//=================================================================================
	string					m_sDebugDir ;

	//=================================================================================
	// Manejador de parámetros
	//=================================================================================
	ClMDTParameterManager	m_oParameterManager ;

	//=================================================================================
	// Tool de presentaciones
	//=================================================================================
	ClMDTPresentationTool	*m_poPresentationTool ;

	//=================================================================================
	// Manejador de base de datos
	//=================================================================================
	M4ODBC					*m_poDBManager ;

	//=================================================================================
	// Diccionario
	//=================================================================================
	ClMDTDictionary			*m_poDictionary ;

	//=================================================================================
	// Transformador
	//=================================================================================
	ClMDTTransformer		*m_poTransformer ;

	//=================================================================================
	// Dispositivo de error a fichero
	//=================================================================================
	ClMDTFileErrorDevice	m_oFileDevice ;

	//=================================================================================
	// Dispositivo de error a log
	//=================================================================================
	ClMDTLogErrorDevice		m_oLogDevice ;

} ;



#endif


