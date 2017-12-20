
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
//    Este m�dulo contiene la declaraci�n de la clase del servicio de la
//    librer�a m4mdtransfer
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
// crean en la librer�a
//
//=================================================================================

class	M4_DECL_M4MDTRANSFER	ClMDTService: public IMDTService
{

public:


//=================================================================================
// Funciones de inicializaci�n
//=================================================================================

		ClMDTService( void ) ;
		virtual ~ClMDTService( void ) ;


	//=================================================================================
	//
	// Inicializa los objetos de la librer�a
	//
	// @param ai_iLanguage
	//
	//		Lenguaje de trabajo. Espa�ol o ingl�s
	//
	// @param ai_iConnections
	//
	//		Handle del array de conexiones.
	//		Son parejas de nombre de conexi�n-DSN de conexi�n
	//
	// @param ai_pccDictConnection
	//
	//		DSN de la conexi�n del diccionario
	//
	// @param ai_eDictDBType
	//
	//		Tipo de base de datos de la conexi�n del diccionario
	//
	// @param ai_pccWorkingDir
	//
	//		Directorio de trabajo
	//
	// @param ai_pccDebugDir
	//
	//		Directorio de depuraci�n
	//
	// @param ai_pccCacheDir
	//
	//		Directorio de caches
	//
	// @param ai_iVersion
	//
	//		Versi�n del repositorio de RAMDL
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
	// Finaliza los objetos de la librer�a
	//
	// @return
	//
	//		M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	End( void ) ;

	//=================================================================================
	// Devuelve si la librer�a ha sido inicializada o no
	// 
	// @return
	//
	//		M4_TRUE si la librer�a ha sido inicializada o M4_FALSE sino
	//
	//=================================================================================
	m4bool_t	IsInitialised( void ) const ;



//=================================================================================
// Funciones del interfaz
//=================================================================================

	//=================================================================================
	// Obtiene la fecha de la sesi�n
	// Los par�metros est�n especificados en el interfaz
	//=================================================================================
	virtual	const string&	GetSessionDate( void ) const ;

	//=================================================================================
	// Obtiene el directorio temporal
	// Los par�metros est�n especificados en el interfaz
	//=================================================================================
	virtual	const string&	GetDebugDir( void ) const ;

	//=================================================================================
	// Obtiene el manejador de par�metros de lectura
	// Los par�metros est�n especificados en el interfaz
	//=================================================================================
	virtual const IMDTReadParameterManager*	GetIReadParameterManager( void ) const ;

	//=================================================================================
	// Obtiene el manejador de par�metros de escritura
	// Los par�metros est�n especificados en el interfaz
	//=================================================================================
	virtual IMDTWriteParameterManager*	GetIWriteParameterManager( void ) ;

	//=================================================================================
	// Obtiene el manejador de par�metros de lectura escritura escritura
	// Los par�metros est�n especificados en el interfaz
	//=================================================================================
	virtual IMDTReadWriteParameterManager*	GetIReadWriteParameterManager( void ) ;

	//=================================================================================
	// Obtiene la tool de presentaciones
	// Los par�metros est�n especificados en el interfaz
	//=================================================================================
	virtual IMDTPresentationTool*	GetIPresentationTool( void ) ;

	//=================================================================================
	// Obtiene el manejador de base de datos
	// Los par�metros est�n especificados en el interfaz
	//=================================================================================
	virtual M4IODBC*	GetIODBC( void ) ;

	//=================================================================================
	// Obtiene el diccionario
	// Los par�metros est�n especificados en el interfaz
	//=================================================================================
	virtual IMDTDictionary*	GetIDictionary( void ) ;

	//=================================================================================
	// Obtiene el transformador
	// Los par�metros est�n especificados en el interfaz
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
	// Indicador de inicializaci�n de la librer�a
	//=================================================================================
	m4bool_t				m_bInitialised ;

	//=================================================================================
	// Fecha de la sessi�n
	//=================================================================================
	string					m_sSessionDate ;

	//=================================================================================
	// Directorio temporal
	//=================================================================================
	string					m_sDebugDir ;

	//=================================================================================
	// Manejador de par�metros
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


