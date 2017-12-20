
//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdtransfer
// File:                m4mdterror.hpp
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                23-04-2004
// Language:            C++
// Operating System:    WINDOWS, UNIX
// Design Document:     
//
//
// Definition:
//
//    Este módulo contiene la declaración de las clases de errores de la
//    librería m4mdtransfer
//
//
//==============================================================================

#include "m4stl.hpp"
#include "m4imdterror.hpp"
#include "m4mdtdef.hpp"

#include <stdio.h>






#ifndef __M4MDTERROR_HPP__
#define __M4MDTERROR_HPP__




//=================================================================================
// Declaraciones por adelantado
//=================================================================================
class IMDTReadWriteParameterManager;


//=================================================================================
// Defines
//=================================================================================

// Set de dispositivos
typedef set< IMDTErrorDevice* >		DeviceSet_t ;
typedef DeviceSet_t::iterator		DeviceSetIt_t ;



//=================================================================================
//
// ClMDTParameterErrorDevice
//
// Clase que implementa un dispositivo de salida a parámetro
// Traduce los mensajes de error
//
//=================================================================================

class	M4_DECL_M4MDTRANSFER ClMDTParameterErrorDevice : public IMDTErrorDevice
{

public:


//=================================================================================
// Funciones de inicialización
//=================================================================================

		ClMDTParameterErrorDevice( void ) ;
		virtual ~ClMDTParameterErrorDevice( void ) ;


	//=================================================================================
	//
	// Inicializa el objeto con los objetos que usa y las variables necesarias
	//
	// @param ai_pccWorkingDir
	//
	//			Directorio de trabajo con permisos de lectura donde se encuentra
	//			el logmsg.ini
	//
	// @param ai_iLanguage
	//			Lenguaje de trabajo. Español o inglés
	//
	// @param ai_pIParameterManager
	//
	//			Referencia a un manejador de parámetros ya existente
	//
	// @return
	//
	//			M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	Init( m4pcchar_t ai_pccWorkingDir, m4uint32_t ai_iLanguage, IMDTReadWriteParameterManager *ai_pIParameterManager ) ;


//=================================================================================
// Funciones del interfaz
//=================================================================================

	//=================================================================================
	// Vuelca un mensaje al dispositivo de salida
	// Los parámetros están especificados en el interfaz
	//=================================================================================
	virtual	m4return_t	DumpMessage( m4uint32_t ai_iCode, m4uint32_t ai_iSeverity, m4pcchar_t ai_pccParameters ) ;



//=================================================================================
// Otras funciones
//=================================================================================

	//=================================================================================
	// Asigna el parámetro al que se vuelcan los errores
	//
	// @param ai_iParameter
	//
	//			Parámetro al que se vuelcan los errores
	//
	// @return
	//
	//			M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	SetParameter( m4int32_t ai_iParameter ) ;


protected:


//=================================================================================
// Miembros
//=================================================================================

	//=================================================================================
	// Parámetro donse se vuelca el mensaje de error
	//=================================================================================
	m4int32_t						m_iParameter ;

	//=================================================================================
	// Lenguaje de trabajo
	//=================================================================================
	m4uint32_t						m_iLanguage ;

	//=================================================================================
	// Interfaz para acceder al manejador de parámetros
	//=================================================================================
	IMDTReadWriteParameterManager	*m_pIParameterManager ;

} ;




//=================================================================================
//
// ClMDTFileErrorDevice
//
// Clase que implementa un dispositivo de salida a fichero
//
//=================================================================================

class	M4_DECL_M4MDTRANSFER ClMDTFileErrorDevice : public IMDTErrorDevice
{

public:


//=================================================================================
// Funciones de inicialización
//=================================================================================

		ClMDTFileErrorDevice( void ) ;
		virtual ~ClMDTFileErrorDevice( void ) ;


	//=================================================================================
	//
	// Inicializa el objeto con el directorio donde se dejará el fichero de salida
	//
	// @param ai_pccFileDir
	//
	//			Directorio de depuración con permisos de escritura donde se escribirá
	//			el fichero de los mensajes
	//
	// @return
	//
	//			M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	Init( m4pcchar_t ai_pccFileDir ) ;


//=================================================================================
// Funciones del interfaz
//=================================================================================

	//=================================================================================
	// Vuelca un mensaje al dispositivo de salida
	// Los parámetros están especificados en el interfaz
	//=================================================================================
	virtual	m4return_t	DumpMessage( m4uint32_t ai_iCode, m4uint32_t ai_iSeverity, m4pcchar_t ai_pccParameters ) ;


protected:


//=================================================================================
// Miembros
//=================================================================================

	//=================================================================================
	// Fichero donde se van volcando los errores
	//=================================================================================
	FILE	*m_pfFile ;

} ;



//=================================================================================
//
// ClMDTLogErrorDevice
//
// Clase que implementa un dispositivo de salida a la log
//
//=================================================================================

class	M4_DECL_M4MDTRANSFER ClMDTLogErrorDevice : public IMDTErrorDevice
{

public:


//=================================================================================
// Funciones de inicialización
//=================================================================================

		ClMDTLogErrorDevice( void ) ;
		virtual ~ClMDTLogErrorDevice( void ) ;


	//=================================================================================
	//
	// Inicializa el objeto con el directorio de trabajo
	//
	// @param ai_pccWorkingDir
	//
	//			Directorio de trabajo donde está el logsys.ini
	//
	// @return
	//
	//			M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	Init( m4pcchar_t ai_pccWorkingDir ) ;


//=================================================================================
// Funciones del interfaz
//=================================================================================

	//=================================================================================
	// Vuelca un mensaje al dispositivo de salida
	// Los parámetros están especificados en el interfaz
	//=================================================================================
	virtual	m4return_t	DumpMessage( m4uint32_t ai_iCode, m4uint32_t ai_iSeverity, m4pcchar_t ai_pccParameters ) ;

} ;



//=================================================================================
//
// ClMDTAggregateErrorDevice
//
// Clase que implementa un multidispositivo de salida
//
//=================================================================================

class	M4_DECL_M4MDTRANSFER ClMDTAggregateErrorDevice : public IMDTErrorDevice
{

public:


//=================================================================================
// Funciones de inicialización
//=================================================================================

		ClMDTAggregateErrorDevice( void ) ;
		virtual ~ClMDTAggregateErrorDevice( void ) ;


//=================================================================================
// Funciones del interfaz
//=================================================================================

	//=================================================================================
	// Vuelca un mensaje al dispositivo de salida
	// Los parámetros están especificados en el interfaz
	//=================================================================================
	virtual	m4return_t	DumpMessage( m4uint32_t ai_iCode, m4uint32_t ai_iSeverity, m4pcchar_t ai_pccParameters ) ;



//=================================================================================
// Otras funciones
//=================================================================================

	//=================================================================================
	// Añade un dispositivo de errores a la lista de dispositivos
	//
	// @param ai_pIErrorDevice
	//
	//			Dispositivo de errores a añadir
	//
	// @return
	//
	//			M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	AddErrorDevice( IMDTErrorDevice *ai_pIErrorDevice ) ;

	//=================================================================================
	// Borra un dispositivo de errores de la lista de dispositivos
	//
	// @param ai_pIErrorDevice
	//
	//			Dispositivo de errores a borrar
	//
	// @return
	//
	//			M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	RemoveErrorDevice( IMDTErrorDevice *ai_pIErrorDevice ) ;


protected:

//=================================================================================
// Miembros
//=================================================================================

	//=================================================================================
	// Lista de dispositivos
	//=================================================================================
	DeviceSet_t	m_oDevices ;

	//=================================================================================
	// Estado de procesamiento
	//=================================================================================
	m4bool_t	m_bDumping ;

} ;




//=================================================================================
//
// M4MDTErrorManager
//
// Clase que implementa un gestor de errores
//
//=================================================================================

class M4_DECL_M4MDTRANSFER M4MDTErrorManager
{

public:

//=================================================================================
// Funciones globales
//=================================================================================

	//=================================================================================
	// Chequea si debe volcar un mesaje de error y si es asi lo vuelca al
	// dispositivo de salida
	//
	// @param ai_bCondition
	//
	//			Condición para volcar el mensaje de error
	//
	// @param ai_iCode
	//
	//			Número del error
	//
	// @param ai_pccFormat
	//
	//			Formato de los parámetros de error
	//
	// @return
	//
	//			M4_TRUE si se cumple al condición o M4_FALSE si no se cumple
	//
	//=================================================================================
	static	m4bool_t	CheckError( m4bool_t ai_bCondition, m4uint32_t ai_iCode, m4pcchar_t ai_pccFormat, ... ) ;

	//=================================================================================
	// Vuelca un mensaje de error al dispositivo de salida
	//
	// @param ai_iCode
	//
	//			Número del error
	//
	// @param ai_pccFormat
	//
	//			Formato de los parámetros de error
	//
	// @return
	//
	//			M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	static	m4return_t	DumpError( m4uint32_t ai_iCode, m4pcchar_t ai_pccFormat, ... ) ;

	//=================================================================================
	// Añade un dispositivo de errores a la lista de dispositivos
	//
	// @param ai_pIErrorDevice
	//
	//			Dispositivo de errores a añadir
	//
	// @return
	//
	//			M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	static	m4return_t	AddErrorDevice( IMDTErrorDevice *ai_pIErrorDevice ) ;

	//=================================================================================
	// Borra un dispositivo de errores de la lista de dispositivos
	//
	// @param ai_pIErrorDevice
	//
	//			Dispositivo de errores a borrar
	//
	// @return
	//
	//			M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	static	m4return_t	RemoveErrorDevice( IMDTErrorDevice *ai_pIErrorDevice ) ;


protected:

//=================================================================================
// Miembros
//=================================================================================

	//=================================================================================
	// Lista de dispositivos
	//=================================================================================
	static	ClMDTAggregateErrorDevice	m_oDevice ;

} ;




#endif


