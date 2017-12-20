
//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdtransfer
// File:                m4mdterror.cpp
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
//    Este módulo contiene las funciones de las clases de errores de la
//    librería m4mdtransfer
//
//
//==============================================================================


#include "m4mdterror.hpp"

#include <stdio.h>
#include <stdarg.h>

#include "m4imdtparameter.hpp"
#include "cldefine.hpp"
#include "cldates.hpp"
#include "m4logsys.hpp"
#include "m4log.hpp"
#include "m4unicode.hpp"




//=================================================================================
// Miembros estáticos
//=================================================================================
ClMDTAggregateErrorDevice	M4MDTErrorManager::m_oDevice ;



//=================================================================================
// Funciones estáticas
//=================================================================================

//=================================================================================
//
// Obtine el texto asociado a una severidad
//
// @param ai_iSeverity
//
//			Severidad del mensaje
//
// @return
//
//			Texto asociado a una severidad
//
//=================================================================================

static	m4pcchar_t	M4MDTGetSeverityText( m4uint32_t ai_iSeverity )
{

	m4pcchar_t	pccResult = NULL ;


	switch( ai_iSeverity )
	{
		case M4_ERROR :
			pccResult = "ERROR" ;
			break ;

		case M4_WARNING :
			pccResult = "WARNING" ;
			break ;

		case M4_SUCCESS :
			pccResult = "INFO" ;
			break ;

		default :
			pccResult = "UNDEFINED" ;
			break ;
	}

	return( pccResult ) ;
}




//=================================================================================
//
// ClMDTParameterErrorDevice
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

ClMDTParameterErrorDevice::ClMDTParameterErrorDevice( void )
{
	m_iParameter = -1 ;
	m_iLanguage = M4MDT_LANG_ENGLISH ;
	m_pIParameterManager = NULL ;
}



//=================================================================================
//
// Destructor
//
// De momento no hace nada
//
//=================================================================================

ClMDTParameterErrorDevice::~ClMDTParameterErrorDevice( void )
{
}



//=================================================================================
// Inicializa el objeto con los objetos que usa y las variables necesarias
//
// Pasos
//		Inicializa los miembros
//		Inicializa el sistema de traduccion de errores con el directorio
//		del logmsg.ini
//
//=================================================================================

m4return_t	ClMDTParameterErrorDevice::Init( m4pcchar_t ai_pccWorkingDir, m4uint32_t ai_iLanguage, IMDTReadWriteParameterManager *ai_pIParameterManager )
{

	m_iLanguage = ai_iLanguage ;
	m_pIParameterManager = ai_pIParameterManager ;

	// void
	LogsysSetWorkingDir( ai_pccWorkingDir ) ;

	return( M4_SUCCESS ) ;
}



//=================================================================================
// Funciones del interfaz
//=================================================================================


//=================================================================================
// Vuelca un mensaje al dispositivo de salida
// Sólo lo vuelca si no hay ya mensajes volcados
//
// Pasos
//		Comprueba que no haya ya un mensaje en el dispositivo de salida
//		Llama a la función que calcula el tamaño del mensaje traducido
//		Si no se encuentra mensaje se vuelca un mensaje genérico
//		Reserva espacio para el mensaje
//		Llama a la función que traduce el mensaje
//		Añade el mensaje a la lista de mensajes
//		Libera el espacio del mensaje
//
//=================================================================================

m4return_t	ClMDTParameterErrorDevice::DumpMessage( m4uint32_t ai_iCode, m4uint32_t ai_iSeverity, m4pcchar_t ai_pccParameters )
{

	m4return_t	iResult = M4_ERROR ;
	int			iCount = 0 ;
	m4uint32_t	iSize = 0 ;
	m4pcchar_t	pccType = NULL ;
	m4pchar_t	pcMessage = NULL ;


	if( m_pIParameterManager == NULL )
	{
		return( M4_ERROR ) ;
	}

	iResult = m_pIParameterManager->GetParameterSize( m_iParameter, iSize ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	if( iSize > 0 )
	{
		return( M4_SUCCESS ) ;
	}

	pccType = M4MDTGetSeverityText( ai_iSeverity ) ;


	iResult = FormatErrorCode( ai_iCode, (m4pchar_t) ai_pccParameters, 0, NULL, 0, &iSize, m_iLanguage ) ;

	if( iResult != M4_SUCCESS )
	{
		iSize = M4MDT_MAX_FORMATED_ERROR_SIZE + 1 ;
	}

	pcMessage = new m4char_t[ iSize ] ;

	if( pcMessage == NULL )
	{
		return( M4_ERROR ) ;
	}

	if( iResult != M4_SUCCESS )
	{
		iCount = snprintf( pcMessage, M4MDT_MAX_FORMATED_ERROR_SIZE, "Undefined error message \"%d\" with parameters \"%s\"", ai_iCode, ai_pccParameters ) ;

		if( iCount < 0 || iCount >= M4MDT_MAX_FORMATED_ERROR_SIZE )
		{
			pcMessage[ M4MDT_MAX_FORMATED_ERROR_SIZE ] = '\0' ;
		}
	}
	else
	{
		iResult = FormatErrorCode( ai_iCode, (m4pchar_t) ai_pccParameters, 0, pcMessage, iSize, &iSize, m_iLanguage ) ;

		if( iResult != M4_SUCCESS )
		{
			delete [] pcMessage ;
			return( M4_ERROR ) ;
		}
	}

	iResult = m_pIParameterManager->AddParameterValue( m_iParameter, pccType, pcMessage ) ;

	delete [] pcMessage ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}



//=================================================================================
// Otras funciones
//=================================================================================

//=================================================================================
// Asigna el parámetro al que se vuelcan los errores
//=================================================================================

m4return_t	ClMDTParameterErrorDevice::SetParameter( m4int32_t ai_iParameter )
{
	m_iParameter = ai_iParameter ;
	return( M4_SUCCESS ) ;
}






//=================================================================================
//
// ClMDTFileErrorDevice
//
//=================================================================================

//=================================================================================
// Funciones de inicialización
//=================================================================================


//=================================================================================
//
// Constructor
//
// Inicializa a fichero a NULL
//
//=================================================================================

ClMDTFileErrorDevice::ClMDTFileErrorDevice( void )
{
	m_pfFile = NULL ;
}



//=================================================================================
//
// Destructor
//
// Si el fichero está abierto se cierra
//
//=================================================================================

ClMDTFileErrorDevice::~ClMDTFileErrorDevice( void )
{
	if( m_pfFile != NULL )
	{
		fclose( m_pfFile ) ;
		m_pfFile = NULL ; 
	}
}



//=================================================================================
// Inicializa el objeto con los objetos que usa y las variables necesarias
//
// Pasos
//		Si el fichero ya está abierto lo cierra
//		Genera el path del fichero y lo intenta abrir para añadir
//		Si no se puede se devuelve error
//		Imprime la cabecera en el fichero con la fecha actual
//		Si no se puede escribir se devuelve error
//
//=================================================================================

m4return_t	ClMDTFileErrorDevice::Init( m4pcchar_t ai_pccFileDir )
{

	int			iWrite = 0 ;
	int			iFlush = 0 ;
    m4char_t	acDate[ M4_SECOND_END + 1 ] ;
	string		sFileName ;


	if( m_pfFile != NULL )
	{
		fclose( m_pfFile ) ;
		m_pfFile = NULL ;
	}

	if( ai_pccFileDir != NULL )
	{
		sFileName.append( ai_pccFileDir ) ;
		sFileName.append( M4_BACK_SLASH ) ;
	}
	sFileName.append( M4MDT_ERROR_FILE_NAME ) ;

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniNative ;

	m_pfFile = M4Fopen( sFileName.c_str(), M4UniAppendBinary, eEncoding ) ;

	if( m_pfFile == NULL )
	{
		return( M4_ERROR ) ;
	}

	ClActualDate( acDate, M4_SECOND_END + 1 ) ;

	iWrite = fprintf( m_pfFile, "\n-------------- %s %s --------------\n", M4MDT_ERROR_FILE_NAME, acDate ) ;

	if( iWrite < 0 )
	{
		return( M4_ERROR ) ;
	}

	iWrite = fprintf( m_pfFile, "Date\t\t\t\tType\t\tCode\t\tParameters\n" ) ;

	if( iWrite < 0 )
	{
		return( M4_ERROR ) ;
	}

	iFlush = fflush( m_pfFile ) ;

	if( iFlush == EOF )
	{
		return( M4_ERROR ) ;
	}
	

	return( M4_SUCCESS ) ;
}


//=================================================================================
// Funciones del interfaz
//=================================================================================


//=================================================================================
// Vuelca un mensaje al dispositivo de salida
//
// Pasos
//		Si el fichero no está abierto se da un error
//		Se obtiene la fecha actual
//		Se vuelca una linea en el fichero de error con la fecha, el tipo,
//			la severidad y los parámetros
//		Si no se puede escribir se devuelve error
//
//=================================================================================

m4return_t	ClMDTFileErrorDevice::DumpMessage( m4uint32_t ai_iCode, m4uint32_t ai_iSeverity, m4pcchar_t ai_pccParameters )
{

	int			iWrite = 0 ;
	int			iFlush = 0 ;
	m4pcchar_t	pccType = NULL ;
    m4char_t	acDate[ M4_SECOND_END + 1 ] ;


	if( m_pfFile == NULL )
	{
		return( M4_ERROR ) ;
	}

	ClActualDate( acDate, M4_SECOND_END + 1 ) ;

	pccType = M4MDTGetSeverityText( ai_iSeverity ) ;

	iWrite = fprintf( m_pfFile, "%s\t%s\t\t%d\t\t%s\n", acDate, pccType, ai_iCode, ai_pccParameters ) ;

	if( iWrite < 0 )
	{
		return( M4_ERROR ) ;
	}

	iFlush = fflush( m_pfFile ) ;

	if( iFlush == EOF )
	{
		return( M4_ERROR ) ;
	}


	return( M4_SUCCESS ) ;
}



//=================================================================================
//
// ClMDTLogErrorDevice
//
//=================================================================================


//=================================================================================
// Funciones de inicialización
//=================================================================================

//=================================================================================
//
// Constructor
//
// No hace nada
//
//=================================================================================

ClMDTLogErrorDevice::ClMDTLogErrorDevice( void )
{
}

//=================================================================================
//
// Destructor
//
// Finaliza la log
//
//=================================================================================

ClMDTLogErrorDevice::~ClMDTLogErrorDevice( void )
{
	m4return_t	iResult = M4_ERROR ;

	iResult = (m4return_t) DESTROYLOGMANAGER() ;
}


//=================================================================================
//
// Inicializa el objeto con el directorio de trabajo
//
//
//=================================================================================

m4return_t	ClMDTLogErrorDevice::Init( m4pcchar_t ai_pccWorkingDir )
{

	m4return_t	iResult = M4_ERROR ;
	string		sLogFile ;


	if( ai_pccWorkingDir != NULL )
	{
		sLogFile.append( ai_pccWorkingDir ) ;
		sLogFile.append( M4_BACK_SLASH ) ;
	}
	sLogFile.append( M4CH_LOGSYS_FILE ) ;

	iResult = (m4return_t) INITIALIZELOGMANAGER( (m4pchar_t) sLogFile.c_str() ) ;

	// Se debe controlar contra ERROR porque si ya está inicializado devuelve warning
	if( iResult == M4_ERROR )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}


//=================================================================================
// Funciones del interfaz
//=================================================================================

//=================================================================================
// Vuelca un mensaje al dispositivo de salida
//=================================================================================
m4return_t	ClMDTLogErrorDevice::DumpMessage( m4uint32_t ai_iCode, m4uint32_t ai_iSeverity, m4pcchar_t ai_pccParameters )
{

	m4return_t	iResult = M4_ERROR ;


	iResult = (m4return_t) SETCODE( ai_iCode, m4int16_t( ai_iSeverity ), ai_pccParameters ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}





//=================================================================================
//
// ClMDTAggregateErrorDevice
//
//=================================================================================

//=================================================================================
// Funciones de inicialización
//=================================================================================


//=================================================================================
//
// Constructor
//
// Inicializa la variable de estado
//
//=================================================================================

ClMDTAggregateErrorDevice::ClMDTAggregateErrorDevice( void )
{
	m_bDumping = M4_FALSE ;
}



//=================================================================================
//
// Destructor
//
// De momento no hace nada
//
//=================================================================================

ClMDTAggregateErrorDevice::~ClMDTAggregateErrorDevice( void )
{
}



//=================================================================================
// Funciones del interfaz
//=================================================================================


//=================================================================================
// Vuelca un mensaje al dispositivo de salida
//
// Pasos
//		Comprueba que no se esté ya volcando un mensaje, porque podría ser
//			un bucle infinito
//		Actualiza el estado de volcado a true
//		Itera por todos los dispositivos y va volcando a cada uno de ellos
//		Si alguno da error se continua, pero al final se devuelve error
//		Actualiza el estado de volcado a false
//
//=================================================================================

m4return_t	ClMDTAggregateErrorDevice::DumpMessage( m4uint32_t ai_iCode, m4uint32_t ai_iSeverity, m4pcchar_t ai_pccParameters )
{

	m4return_t		iResult = M4_ERROR ;
	m4return_t		iTotalResult = M4_ERROR ;
	DeviceSetIt_t	iterator ;


	if( m_bDumping == M4_TRUE )
	{
		return( M4_ERROR ) ;
	}

	m_bDumping = M4_TRUE ;


	iterator = m_oDevices.begin() ;

	iTotalResult = M4_SUCCESS ;
	while( iterator != m_oDevices.end() )
	{
		if( *iterator != NULL )
		{
			iResult = (*iterator)->DumpMessage( ai_iCode, ai_iSeverity, ai_pccParameters ) ;

			if( iResult != M4_SUCCESS )
			{
				iTotalResult = M4_ERROR ;
			}
		}

		iterator++ ;
	}


	m_bDumping = M4_FALSE ;

	return( iTotalResult ) ;
}




//=================================================================================
// Otras funciones
//=================================================================================

//=================================================================================
// Añade un dispositivo de errores a la lista de dispositivos
// Si no se puede añadir (posiblemente porque ya está) se da un error
//=================================================================================

m4return_t	ClMDTAggregateErrorDevice::AddErrorDevice( IMDTErrorDevice *ai_pIErrorDevice )
{

	pair<DeviceSet_t::iterator, bool>	pair ;


	pair = m_oDevices.insert( DeviceSet_t::value_type( ai_pIErrorDevice ) ) ;

	if( pair.second != true )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}



//=================================================================================
// Borra un dispositivo de errores de la lista de dispositivos
//
// Pasos
//		Delega en al función de borrado del set
//		Si no se ha borardo (porque no está ) se da un error
//
//=================================================================================

m4return_t	ClMDTAggregateErrorDevice::RemoveErrorDevice( IMDTErrorDevice *ai_pIErrorDevice )
{

	size_t	iSize = 0 ;


	iSize = m_oDevices.erase( ai_pIErrorDevice ) ;
	
	if( iSize < 1 )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}






//=================================================================================
//
// M4MDTErrorManager
//
//=================================================================================


//=================================================================================
// Funciones globales
//=================================================================================

//=================================================================================
// Chequea si debe volcar un mensaje de error y si es asi lo vuelca al
// dispositivo de salida
//
// Pasos
//		Se chequea la condición.
//		Si se cumple se formatea la cadena de error
//		Si se pasa del tamaño del buffer hay que nullterminar, porque en
//		windows no se nulltermina
//		vnsprintf en windows devuelve -1 si se pasa de tamaño y no nulltermina
//		vnsprintf en unix devuelve los caracteres que quería escribir si se pasa
//			de tamaño, y si que nulltermina
//		Se delega en el dispositivo agregado
//
//=================================================================================

m4bool_t	M4MDTErrorManager::CheckError( m4bool_t ai_bCondition, m4uint32_t ai_iCode, m4pcchar_t ai_pccFormat, ... )
{

	m4return_t	iResult = M4_ERROR ;
	int			iCount = 0 ;
	char		acTempBuffer[ M4MDT_MAX_FORMATED_ERROR_SIZE + 1 ] ;
	va_list		list ;

	
	if( ai_bCondition == M4_TRUE )
	{
		va_start( list, ai_pccFormat ) ;
		iCount = vsnprintf( acTempBuffer, M4MDT_MAX_FORMATED_ERROR_SIZE, ai_pccFormat, list ) ;
		va_end( list ) ;

		if( iCount < 0 || iCount >= M4MDT_MAX_FORMATED_ERROR_SIZE )
		{
			acTempBuffer[ M4MDT_MAX_FORMATED_ERROR_SIZE ] = '\0' ;
		}

		iResult = m_oDevice.DumpMessage( ai_iCode, M4_ERROR, acTempBuffer ) ;
		return( M4_TRUE ) ;
	}

	return( M4_FALSE ) ;
}



//=================================================================================
// Vuelca un mensaje de error al dispositivo de salida
//
// Pasos
//		Formatea la cadena de error
//		Si se pasa del tamaño del buffer hay que nullterminar, porque en
//		windows no se nulltermina
//		vnsprintf en windows devuelve -1 si se pasa de tamaño y no nulltermina
//		vnsprintf en unix devuelve los caracteres que quería escribir si se pasa
//			de tamaño, y si que nulltermina
//		Se delega en el dispositivo agregado
//
//=================================================================================

m4return_t	M4MDTErrorManager::DumpError( m4uint32_t ai_iCode, m4pcchar_t ai_pccFormat, ... )
{

	m4return_t	iResult = M4_ERROR ;
	int			iCount = 0 ;
	char		acTempBuffer[ M4MDT_MAX_FORMATED_ERROR_SIZE + 1 ] ;
	va_list		list ;


	va_start( list, ai_pccFormat ) ;
	iCount = vsnprintf( acTempBuffer, M4MDT_MAX_FORMATED_ERROR_SIZE, ai_pccFormat, list ) ;
	va_end( list ) ;

	if( iCount < 0 || iCount >= M4MDT_MAX_FORMATED_ERROR_SIZE )
	{
		acTempBuffer[ M4MDT_MAX_FORMATED_ERROR_SIZE ] = '\0' ;
	}

	iResult = m_oDevice.DumpMessage( ai_iCode, M4_ERROR, acTempBuffer ) ;

	return( iResult ) ;
}



//=================================================================================
// Añade un dispositivo de errores a la lista de dispositivos
//
// Pasos
//		Delega en el dispositivo agregado
//
//=================================================================================

m4return_t	M4MDTErrorManager::AddErrorDevice( IMDTErrorDevice *ai_pIErrorDevice )
{

	m4return_t	iResult = M4_ERROR ;


	iResult = m_oDevice.AddErrorDevice( ai_pIErrorDevice ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}



//=================================================================================
// Borra un dispositivo de errores de la lista de dispositivos
//
// Pasos
//		Delega en el dispositivo agregado
//
//=================================================================================

m4return_t	M4MDTErrorManager::RemoveErrorDevice( IMDTErrorDevice *ai_pIErrorDevice )
{

	m4return_t	iResult = M4_ERROR ;


	iResult = m_oDevice.RemoveErrorDevice( ai_pIErrorDevice ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}





