
//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdtwrapper.dll
// File:                m4mdtwrapper.cpp   
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                30-03-2004
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene la implementación del API VB que exporta la
//    librería m4mdtransfer
//
//
//==============================================================================


#include "m4mdtwrapper.hpp"
#include "m4mdtdef.hpp"
#include "m4mdtres.hpp"
#include "m4mdterror.hpp"
#include "m4mdtservice.hpp"
#include "m4imdtparameter.hpp"
#include "m4itransformer.hpp"
#include "m4mdtutils.hpp"
#include "m4unicode.hpp"

#include <sys/types.h>
#include <sys/stat.h>




//=================================================================================
// Variables globales
//=================================================================================

// Servicio con todos los objetos de la librería m4mdtransfer
static	ClMDTService				s_oMDTService ;

// Dispositivo de salida de errores a parámetro
static	ClMDTParameterErrorDevice	s_oMDTParameterDevice ;




//=================================================================================
// Clases auxiliares
//=================================================================================

//=================================================================================
//
// ClMDTWErrorHandler
//
// Clase que gestiona el el añadir y liberar un dispositivo de error del gestor
// Sirve para usarse como variable local
//
//=================================================================================

class	ClMDTWErrorHandler
{

public:

//=================================================================================
// Funciones de inicialización
//=================================================================================

		//=================================================================================
		// Constructor
		// Inicializa el miembro a NULL
		//=================================================================================
		ClMDTWErrorHandler( void )
		{
			m_poIErrorDevice = NULL ;
		}

		//=================================================================================
		// Destructor
		// Saca miembro de la lista
		//=================================================================================
		virtual ~ClMDTWErrorHandler( void )
		{
			m4return_t	iResult = M4_ERROR ;

			if( m_poIErrorDevice != NULL )
			{
				iResult = M4MDTErrorManager::RemoveErrorDevice( m_poIErrorDevice ) ;
			}
		}

		//=================================================================================
		// Inicializa el objeto con el dispositivo
		//
		// @param ai_poIErrorDevice
		//
		//			Dispositivo de salida
		//
		// @param ai_iParameter
		//
		//			Parámetro de salida
		//
		// Pasos
		//		Establece el parámetro del dispositivo
		//		Añade el dispositivo a la lista
		//
		//=================================================================================
		m4return_t	Init( ClMDTParameterErrorDevice *ai_poIErrorDevice, m4int32_t ai_iParameter )
		{
			m4return_t	iResult = M4_ERROR ;

			m_poIErrorDevice = ai_poIErrorDevice ;

			if( m_poIErrorDevice == NULL )
			{
				return( M4_ERROR ) ;
			}

			iResult = m_poIErrorDevice->SetParameter( ai_iParameter ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}

			iResult = M4MDTErrorManager::AddErrorDevice( m_poIErrorDevice ) ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}

			return( M4_SUCCESS ) ;
		}


protected :

//=================================================================================
// Miembros
//=================================================================================

	//=================================================================================
	// Dispositivo de salida
	//=================================================================================
	ClMDTParameterErrorDevice *m_poIErrorDevice ;

} ;



//=================================================================================
// Manejo de parámetros
//=================================================================================


//=================================================================================
// Crea un parámetro nuevo
//
// Pasos
//		Inicializa la variable de salida a -1
//		Obtiene el manejador de parámetros y delega la ejecución
//		Convierte la variable local a la variable de salida
//
//=================================================================================

BOOL WINAPI M4MDTW_createParameter( LPLONG ao_plParameter )
{

	m4return_t					iResult = M4_ERROR ;
	m4int32_t					iParameter = -1 ;
	IMDTWriteParameterManager	*poIWrite = NULL ;


	*ao_plParameter = -1 ;

	poIWrite = s_oMDTService.GetIWriteParameterManager() ;
	M4MDT_RETURN_ERROR2( poIWrite == NULL, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_createParameter", "GetIWriteParameterManager", "%s#%s" ) ;

	iResult = poIWrite->CreateParameter( iParameter ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_createParameter", "CreateParameter", "%s#%s" ) ;

	*ao_plParameter = iParameter ;

	return( TRUE ) ;
}

//=================================================================================
// Libera un parámetro existente
//
// Pasos
//		Obtiene el manejador de parámetros
//		Convierte la variable de entrada a la variable local
//		Delega la ejecución
//
//=================================================================================

BOOL WINAPI M4MDTW_releaseParameter( LONG ai_lParameter )
{

	m4return_t					iResult = M4_ERROR ;
	m4int32_t					iParameter = -1 ;
	IMDTWriteParameterManager	*poIWrite = NULL ;


	poIWrite = s_oMDTService.GetIWriteParameterManager() ;
	M4MDT_RETURN_ERROR2( poIWrite == NULL, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_releaseParameter", "GetIWriteParameterManager", "%s#%s" ) ;

	iParameter = ai_lParameter ;
	iResult = poIWrite->ReleaseParameter( iParameter ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_releaseParameter", "ReleaseParameter", "%s#%s" ) ;

	return( TRUE ) ;
}


//=================================================================================
// Crea una nueva pareja de valores, la rellena con los valores pasados y la añade
// al conjunto de parejas al final
// El valor de la pareja se crea de tipo cadena
//
// Pasos
//		Se chequea que los parámetros de entrada no sean nulos
//		Obtiene el manejador de parámetros
//		Convierte las variables de entrada a las variables locales
//		Delega la ejecución
//
//=================================================================================

BOOL WINAPI M4MDTW_addParameterStringValue( LONG ai_lParameter, LPCSTR ai_csFirst, LPCSTR ai_csSecond )
{

	m4return_t					iResult = M4_ERROR ;
	m4int32_t					iParameter = -1 ;
	m4pchar_t					pcFirst = NULL ;
	m4pchar_t					pcSecond = NULL ;
	int							iFirstSize = -1 ;
	int							iSecondSize = -1 ;
	IMDTWriteParameterManager	*poIWrite = NULL ;


	M4MDT_RETURN_ERROR2( ai_csFirst == NULL, FALSE, M4MDTERROR_NULL_ARGUMENT, "ai_csFirst", "M4MDTW_addParameterStringValue", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ai_csSecond == NULL, FALSE, M4MDTERROR_NULL_ARGUMENT, "ai_csSecond", "M4MDTW_addParameterStringValue", "%s#%s" ) ;

	poIWrite = s_oMDTService.GetIWriteParameterManager() ;
	M4MDT_RETURN_ERROR2( poIWrite == NULL, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_addParameterStringValue", "GetIWriteParameterManager", "%s#%s" ) ;

	iParameter = ai_lParameter ;
	// Antonio ADN. Se estaban añadiendo las cadenas en ANSI (que se lo que envía VB).
	// Se pasa a CPP que es con lo que se trabaja internamente
	pcFirst = M4ANSIToCpp( ai_csFirst, iFirstSize ) ;
	pcSecond = M4ANSIToCpp( ai_csSecond, iSecondSize ) ;

	iResult = poIWrite->AddParameterValue( iParameter, pcFirst, pcSecond ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_addParameterStringValue", "AddParameterValue", "%s#%s" ) ;

	delete [] pcFirst ;
	delete [] pcSecond ;

	return( TRUE ) ;
}


//=================================================================================
// Crea una nueva pareja de valores, la rellena con los valores pasados y la añade
// al conjunto de parejas al final
// El valor de la pareja se crea de tipo parámetro
//
// Pasos
//		Se chequea que los parámetros de entrada no sean nulos
//		Obtiene el manejador de parámetros
//		Convierte las variables de entrada a las variables locales
//		Delega la ejecución
//
//=================================================================================

BOOL WINAPI M4MDTW_addParameterHandleValue( LONG ai_lParameter, LPCSTR ai_csFirst, LONG ai_lSecond )
{

	m4return_t					iResult = M4_ERROR ;
	m4int32_t					iParameter = 0 ;
	m4int32_t					iSecond = NULL ;
	m4pchar_t					pcFirst = NULL ;
	int							iFirstSize = -1 ;
	IMDTWriteParameterManager	*poIWrite = NULL ;


	M4MDT_RETURN_ERROR2( ai_csFirst == NULL, FALSE, M4MDTERROR_NULL_ARGUMENT, "ai_csFirst", "M4MDTW_addParameterHandleValue", "%s#%s" ) ;

	poIWrite = s_oMDTService.GetIWriteParameterManager() ;
	M4MDT_RETURN_ERROR2( poIWrite == NULL, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_addParameterHandleValue", "GetIWriteParameterManager", "%s#%s" ) ;

	iParameter = ai_lParameter ;
	// Antonio ADN. Se estaban añadiendo las cadenas en ANSI (que se lo que envía VB).
	// Se pasa a CPP que es con lo que se trabaja internamente
	pcFirst = M4ANSIToCpp( ai_csFirst, iFirstSize ) ;
	iSecond = ai_lSecond ;

	iResult = poIWrite->AddParameterValue( iParameter, pcFirst, iSecond ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_addParameterHandleValue", "AddParameterValue", "%s#%s" ) ;

	delete [] pcFirst ;

	return( TRUE ) ;
}


//=================================================================================
// Elimina una nueva pareja de valores de tipo cadena
//
// Pasos
//		Se chequea que los parámetros de entrada no sean nulos
//		Obtiene el manejador de parámetros
//		Convierte las variables de entrada a las variables locales
//		Delega la ejecución
//
//=================================================================================

BOOL WINAPI M4MDTW_removeParameterStringValue( LONG ai_lParameter, LPCSTR ai_csFirst, LPCSTR ai_csSecond )
{

	m4return_t					iResult = M4_ERROR ;
	m4int32_t					iParameter = -1 ;
	m4pchar_t					pcFirst = NULL ;
	m4pchar_t					pcSecond = NULL ;
	int							iFirstSize = -1 ;
	int							iSecondSize = -1 ;
	IMDTWriteParameterManager	*poIWrite = NULL ;


	M4MDT_RETURN_ERROR2( ai_csFirst == NULL, FALSE, M4MDTERROR_NULL_ARGUMENT, "ai_csFirst", "M4MDTW_removeParameterStringValue", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ai_csSecond == NULL, FALSE, M4MDTERROR_NULL_ARGUMENT, "ai_csSecond", "M4MDTW_removeParameterStringValue", "%s#%s" ) ;

	poIWrite = s_oMDTService.GetIWriteParameterManager() ;
	M4MDT_RETURN_ERROR2( poIWrite == NULL, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_removeParameterStringValue", "GetIWriteParameterManager", "%s#%s" ) ;

	iParameter = ai_lParameter ;
	// Antonio ADN. Se estaban añadiendo las cadenas en ANSI (que se lo que envía VB).
	// Se pasa a CPP que es con lo que se trabaja internamente
	pcFirst = M4ANSIToCpp( ai_csFirst, iFirstSize ) ;
	pcSecond = M4ANSIToCpp( ai_csSecond, iSecondSize ) ;

	iResult = poIWrite->RemoveParameterValue( iParameter, pcFirst, pcSecond ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_removeParameterStringValue", "RemoveParameterValue", "%s#%s" ) ;

	delete [] pcFirst ;
	delete [] pcSecond ;

	return( TRUE ) ;
}


//=================================================================================
// Elimina una nueva pareja de valores de tipo parámetro
//
// Pasos
//		Se chequea que los parámetros de entrada no sean nulos
//		Obtiene el manejador de parámetros
//		Convierte las variables de entrada a las variables locales
//		Delega la ejecución
//
//=================================================================================

BOOL WINAPI M4MDTW_removeParameterHandleValue( LONG ai_lParameter, LPCSTR ai_csFirst, LONG ai_lSecond )
{

	m4return_t					iResult = M4_ERROR ;
	m4int32_t					iParameter = 0 ;
	m4int32_t					iSecond = NULL ;
	m4pchar_t					pcFirst = NULL ;
	int							iFirstSize = -1 ;
	IMDTWriteParameterManager	*poIWrite = NULL ;


	M4MDT_RETURN_ERROR2( ai_csFirst == NULL, FALSE, M4MDTERROR_NULL_ARGUMENT, "ai_csFirst", "M4MDTW_removeParameterHandleValue", "%s#%s" ) ;

	poIWrite = s_oMDTService.GetIWriteParameterManager() ;
	M4MDT_RETURN_ERROR2( poIWrite == NULL, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_removeParameterHandleValue", "GetIWriteParameterManager", "%s#%s" ) ;

	iParameter = ai_lParameter ;
	// Antonio ADN. Se estaban añadiendo las cadenas en ANSI (que se lo que envía VB).
	// Se pasa a CPP que es con lo que se trabaja internamente
	pcFirst = M4ANSIToCpp( ai_csFirst, iFirstSize ) ;
	iSecond = ai_lSecond ;

	iResult = poIWrite->RemoveParameterValue( iParameter, pcFirst, iSecond ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_removeParameterHandleValue", "RemoveParameterValue", "%s#%s" ) ;

	delete [] pcFirst ;

	return( TRUE ) ;
}


//=================================================================================
// Obtiene el número de parejas de valores de un parámetro
//
// Pasos
//		Inicializa la variable de salida a 0
//		Obtiene el manejador de parámetros
//		Convierte la variable de entrada a la variable local
//		Delega la ejecución
//		Convierte la variable local a la variable de salida
//
//=================================================================================

BOOL WINAPI M4MDTW_getParameterSize( LONG ai_lParameter, LPLONG ao_plSize )
{

	m4return_t						iResult = M4_ERROR ;
	m4int32_t						iParameter = -1 ;
	m4uint32_t						iSize = 0 ;
	const IMDTReadParameterManager	*pcoIRead = NULL ;


	*ao_plSize = 0 ;

	pcoIRead = s_oMDTService.GetIReadParameterManager() ;
	M4MDT_RETURN_ERROR2( pcoIRead == NULL, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_getParameterSize", "GetIReadParameterManager", "%s#%s" ) ;

	iParameter = ai_lParameter ;

	iResult = pcoIRead->GetParameterSize( iParameter, iSize ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_getParameterSize", "GetParameterSize", "%s#%s" ) ;

	*ao_plSize = iSize ;

	return( TRUE ) ;
}


//=================================================================================
// Obtiene los valores de una pareja de valores de un parámetro
// La pareja se identifica por su posición dentro del conjunto
// El valor de la pareja debe ser de tipo cadena
//
// Pasos
//		Libera las cadenas de salida si no están vacías
//		Obtiene el manejador de parámetros
//		Convierte las variables de entrada a las variables locales
//		Delega la ejecución
//		Convierte las variables locales a las variables de salida. Para las
//			cadenas se alloca memoria y se copian, sin convertir a unicode
//
//=================================================================================

BOOL WINAPI M4MDTW_getParameterStringValue( LONG ai_lParameter, LONG ai_lPosition, LPBSTR ao_psFirst, LPBSTR ao_psSecond )
{

	m4return_t						iResult = M4_ERROR ;
	m4int32_t						iParameter = -1 ;
	m4uint32_t						iPosition = 0 ;
	m4pcchar_t						pccFirst = NULL ;
	m4pcchar_t						pccSecond = NULL ;
	m4pchar_t						pcFirstANSI = NULL ;
	m4pchar_t						pcSecondANSI = NULL;
	int								iFirstSize = -1 ;
	int								iSecondSize = -1 ;

	const IMDTReadParameterManager	*pcoIRead = NULL ;


	if( *ao_psFirst != NULL )
	{
		SysFreeString( *ao_psFirst ) ;
		*ao_psFirst = NULL ;
	}

	if( *ao_psSecond != NULL )
	{
		SysFreeString( *ao_psSecond ) ;
		*ao_psSecond = NULL ;
	}

	pcoIRead = s_oMDTService.GetIReadParameterManager() ;
	M4MDT_RETURN_ERROR2( pcoIRead == NULL, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_getParameterStringValue", "GetIReadParameterManager", "%s#%s" ) ;

	iParameter = ai_lParameter ;
	iPosition = ai_lPosition ;

	iResult = pcoIRead->GetParameterValue( iParameter, iPosition, pccFirst, pccSecond ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_getParameterStringValue", "GetParameterValue", "%s#%s" ) ;

	// Antonio ADN. Se estaban devolviendo lo errores en CPP. Se realiza antes la conversión
	// a ANSI para que lleguen correctamente a VB.
	pcFirstANSI = M4CppToANSI( pccFirst, iFirstSize ) ;
	*ao_psFirst = SysAllocStringByteLen( pcFirstANSI, iFirstSize ) ;
	delete [] pcFirstANSI ;

	pcSecondANSI = M4CppToANSI( pccSecond, iSecondSize ) ;
	*ao_psSecond = SysAllocStringByteLen( pcSecondANSI, iSecondSize ) ;
	delete [] pcSecondANSI ;

	return( TRUE ) ;
}


//=================================================================================
// Obtiene los valores de una pareja de valores de un parámetro
// La pareja se identifica por su posición dentro del conjunto
// El valor de la pareja debe ser de tipo parámetro
//
// Pasos
//		Libera la cadena de salida si no está vacía
//		Inicializa la variable de salida a -1
//		Obtiene el manejador de parámetros
//		Convierte las variables de entrada a las variables locales
//		Delega la ejecución
//		Convierte las variables locales a las variables de salida. Para la
//			cadena se alloca memoria y se copia, sin convertir a unicode
//
//=================================================================================

BOOL WINAPI M4MDTW_getParameterHandleValue( LONG ai_lParameter, LONG ai_lPosition, LPBSTR ao_psFirst, LPLONG ao_plSecond )
{

	m4return_t						iResult = M4_ERROR ;
	m4int32_t						iParameter = -1 ;
	m4uint32_t						iPosition = 0 ;
	m4pcchar_t						pccFirst = NULL ;
	m4int32_t						iSecond = NULL ;
	m4pchar_t						pcFirstANSI = NULL ;
	int								iFirstSize = -1 ;
	const IMDTReadParameterManager	*pcoIRead = NULL ;


	if( *ao_psFirst != NULL )
	{
		SysFreeString( *ao_psFirst ) ;
		*ao_psFirst = NULL ;
	}

	*ao_plSecond = -1 ;

	pcoIRead = s_oMDTService.GetIReadParameterManager() ;
	M4MDT_RETURN_ERROR2( pcoIRead == NULL, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_getParameterHandleValue", "GetIReadParameterManager", "%s#%s" ) ;

	iParameter = ai_lParameter ;
	iPosition = ai_lPosition ;

	iResult = pcoIRead->GetParameterValue( iParameter, iPosition, pccFirst, iSecond ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_getParameterHandleValue", "GetParameterValue", "%s#%s" ) ;

	// Antonio ADN. Se estaban devolviendo lo errores en CPP. Se realiza antes la conversión
	// a ANSI para que lleguen correctamente a VB.
	pcFirstANSI = M4CppToANSI( pccFirst, iFirstSize ) ;
	*ao_psFirst = SysAllocStringByteLen( pcFirstANSI, iFirstSize ) ;
	delete [] pcFirstANSI ;

	*ao_plSecond = iSecond ;

	return( TRUE ) ;
}





//=================================================================================
// Funciones del API
//=================================================================================


//=================================================================================
// Inicializa la librería con los parámetros externos necesarios
//
// Pasos
//		Se chequea que los parámetros de entrada no sean nulos
//		Convierte las variables de entrada a las variables locales
//		Inicializa el dispositivo de errores
//		Activa la gestión de errores
//		Delega la ejecución
//
//=================================================================================

BOOL WINAPI M4MDTW_openSession( LONG ai_lConnections, LPCSTR ai_csDictionaryConnection, LPCSTR ai_csWorkingDir, LPCSTR ai_csDebugDir, LPCSTR ai_csCacheDir, LONG ai_lDataBase, LONG ai_lLanguage, LONG ai_lVersion, LONG ai_lTraceLevel, LONG ao_lErrors )
{

	m4return_t						iResult = M4_ERROR ;
	m4int32_t						iConnections = -1 ;
	m4uint32_t						iLanguage = 0 ;
	m4uint32_t						iVersion = 0 ;
	m4uint32_t						iTraceLevel = 0 ;
	eDDBBType_t						eDbType ;
	m4pchar_t						pcDictConnection = NULL ;
	m4pchar_t						pcWorkingDir = NULL ;
	m4pchar_t						pcDebugDir = NULL ;
	m4pchar_t						pcCacheDir = NULL ;
	int								iDictConnectionSize = -1 ;
	int								iWorkingDirSize = -1 ;
	int								iDebugDir = -1 ;
	int								iCacheDir = -1 ;
	IMDTReadWriteParameterManager	*poIReadWrite = NULL ;
	ClMDTWErrorHandler				oErrorHandler ;


	M4MDT_RETURN_ERROR2( ai_csDictionaryConnection == NULL, FALSE, M4MDTERROR_NULL_ARGUMENT, "ai_csDictionaryConnection", "M4MDTW_openSession", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ai_csWorkingDir == NULL, FALSE, M4MDTERROR_NULL_ARGUMENT, "ai_csWorkingDir", "M4MDTW_openSession", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ai_csDebugDir == NULL, FALSE, M4MDTERROR_NULL_ARGUMENT, "ai_csDebugDir", "M4MDTW_openSession", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ai_csCacheDir == NULL, FALSE, M4MDTERROR_NULL_ARGUMENT, "ai_csCacheDir", "M4MDTW_openSession", "%s#%s" ) ;

	iConnections = ai_lConnections ;
	// Antonio ADN. Se estaban añadiendo las cadenas en ANSI (que se lo que envía VB).
	// Se pasa a CPP que es con lo que se trabaja internamente
	pcDictConnection = M4ANSIToCpp( ai_csDictionaryConnection, iDictConnectionSize ) ;
	pcWorkingDir = M4ANSIToCpp( ai_csWorkingDir, iWorkingDirSize ) ;
	pcDebugDir = M4ANSIToCpp( ai_csDebugDir, iDebugDir ) ;
	pcCacheDir = M4ANSIToCpp( ai_csCacheDir, iCacheDir ) ;
	eDbType = eDDBBType_t( ai_lDataBase ) ;
	iLanguage = ai_lLanguage ;
	iVersion = ai_lVersion ;
	iTraceLevel = ai_lTraceLevel ;


	poIReadWrite = s_oMDTService.GetIReadWriteParameterManager() ;
	M4MDT_RETURN_ERROR2( poIReadWrite == NULL, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_openSession", "s_oMDTService.GetIReadWriteParameterManager", "%s#%s" ) ;

	iResult = s_oMDTParameterDevice.Init( pcWorkingDir, iLanguage, poIReadWrite ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_openSession", "s_oMDTParameterDevice.Init", "%s#%s" ) ;

	iResult = oErrorHandler.Init( &s_oMDTParameterDevice, ao_lErrors ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_openSession", "oErrorHandler.Init", "%s#%s" ) ;


	iResult = s_oMDTService.Init( iLanguage, iConnections, pcDictConnection, eDbType, pcWorkingDir, pcDebugDir, pcCacheDir, iVersion, iTraceLevel ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_openSession", "s_oMDTService.Init", "%s#%s" ) ;

	delete [] pcDictConnection ;
	delete [] pcWorkingDir ;
	delete [] pcDebugDir ;
	delete [] pcCacheDir ;

	return( TRUE ) ;
}


//=================================================================================
// Libera los recursos asociados a la librería
//
// Pasos
//		Activa la gestión de errores
//		Delega la ejecución
//
//=================================================================================

BOOL WINAPI M4MDTW_closeSession( LONG ao_lErrors )
{

	m4return_t			iResult = M4_ERROR ;
	ClMDTWErrorHandler	oErrorHandler ;


	iResult = oErrorHandler.Init( &s_oMDTParameterDevice, ao_lErrors ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_openSession", "oErrorHandler.Init", "%s#%s" ) ;

	iResult = s_oMDTService.End() ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_closeSession", "End", "%s#%s" ) ;

	return( TRUE ) ;
}



//=================================================================================
// Genera el XML de datos de un objeto traspasable para un repositorio concreto.
// Además proporciona una lista de los hijos de ese objeto
//
// Pasos
//		Activa la gestión de errores
//		Se chequea que los parámetros de entrada no sean nulos
//		Obtiene el transformador
//		Convierte las variables de entrada a las variables locales
//		Delega la ejecución
//
//=================================================================================

BOOL WINAPI M4MDTW_generateXMLObjectData( LPCSTR ai_csIdConnection, LPCSTR ai_csIdObject, LONG ai_lPKs, LPCSTR ai_csXMLFileName, LPCSTR ai_csXMLExclusionFileName, LONG ao_lChilds, LONG ao_lErrors )
{

	m4return_t			iResult = M4_ERROR ;
	m4int32_t			iPKs = -1 ;
	m4int32_t			iChilds = -1 ;
	m4pchar_t			pcIdConnection = NULL ;
	m4pchar_t			pcIdObject = NULL ;
	m4pchar_t			pcXMLFileName = NULL ;
	m4pchar_t			pcXMLExclusionFileName = NULL ;
	int					iIdConnectionSize = -1 ;
	int					iIdObjectSize = -1 ;
	int					iXMLFileName = -1 ;
	int					iXMLExclusionFileName = -1 ;
	IMDTTransformer		*poITransformer = NULL ;
	ClMDTWErrorHandler	oErrorHandler ;


	iResult = oErrorHandler.Init( &s_oMDTParameterDevice, ao_lErrors ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_openSession", "oErrorHandler.Init", "%s#%s" ) ;

	
	M4MDT_RETURN_ERROR2( ai_csIdConnection == NULL, FALSE, M4MDTERROR_NULL_ARGUMENT, "ai_csIdConnection", "M4MDTW_generateXMLObjectData", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ai_csIdObject == NULL, FALSE, M4MDTERROR_NULL_ARGUMENT, "ai_csIdObject", "M4MDTW_generateXMLObjectData", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ai_csXMLFileName == NULL, FALSE, M4MDTERROR_NULL_ARGUMENT, "ai_csXMLFileName", "M4MDTW_generateXMLObjectData", "%s#%s" ) ;


	poITransformer = s_oMDTService.GetITransformer() ;
	M4MDT_RETURN_ERROR2( poITransformer == NULL, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_generateXMLObjectData", "GetITransformer", "%s#%s" ) ;

	// Antonio ADN. Se estaban añadiendo las cadenas en ANSI (que se lo que envía VB).
	// Se pasa a CPP que es con lo que se trabaja internamente
	pcIdConnection = M4ANSIToCpp( ai_csIdConnection, iIdConnectionSize ) ;
	pcIdObject = M4ANSIToCpp( ai_csIdObject, iIdObjectSize ) ;
	iPKs = ai_lPKs ;
	pcXMLFileName = M4ANSIToCpp( ai_csXMLFileName, iXMLFileName ) ;
	pcXMLExclusionFileName = M4ANSIToCpp( ai_csXMLExclusionFileName, iXMLExclusionFileName ) ;
	iChilds = ao_lChilds ;
	
	iResult = poITransformer->GenerateXMLObjectData( pcIdConnection, pcIdObject, iPKs, pcXMLFileName, pcXMLExclusionFileName, iChilds ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_generateXMLObjectData", "GenerateXMLObjectData", "%s#%s" ) ;

	delete [] pcIdConnection ;
	delete [] pcIdObject ;
	delete [] pcXMLFileName ;
	delete [] pcXMLExclusionFileName ;

	return( TRUE ) ;
}

//=================================================================================
// Genera el XML de datos enriquecido (traducido) de un objeto traspasable a partir
// de su XML de datos
//
// Pasos
//		Activa la gestión de errores
//		Se chequea que los parámetros de entrada no sean nulos
//		Obtiene el transformador
//		Convierte las variables de entrada a las variables locales
//		Delega la ejecución
//
//=================================================================================

BOOL WINAPI M4MDTW_generateRichXMLObjectData( LPCSTR ai_csXMLFileName, LPCSTR ai_csRichXMLFileName, LONG ao_lErrors )
{

	m4return_t			iResult = M4_ERROR ;
	m4pchar_t			pcXMLFileName = NULL ;
	m4pchar_t			pcRichXMLFileName = NULL ;
	int					iXMLFileNameSize = -1 ;
	int					iRichXMLFileNameSize = -1 ;
	IMDTTransformer		*poITransformer = NULL ;
	ClMDTWErrorHandler	oErrorHandler ;


	iResult = oErrorHandler.Init( &s_oMDTParameterDevice, ao_lErrors ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_generateRichXMLObjectData", "oErrorHandler.Init", "%s#%s" ) ;

	
	M4MDT_RETURN_ERROR2( ai_csXMLFileName == NULL, FALSE, M4MDTERROR_NULL_ARGUMENT, "ai_csXMLFileName", "M4MDTW_generateRichXMLObjectData", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ai_csRichXMLFileName == NULL, FALSE, M4MDTERROR_NULL_ARGUMENT, "ai_csRichXMLFileName", "M4MDTW_generateRichXMLObjectData", "%s#%s" ) ;


	poITransformer = s_oMDTService.GetITransformer() ;
	M4MDT_RETURN_ERROR2( poITransformer == NULL, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_generateRichXMLObjectData", "GetITransformer", "%s#%s" ) ;

	// Antonio ADN. Se estaban añadiendo las cadenas en ANSI (que se lo que envía VB).
	// Se pasa a CPP que es con lo que se trabaja internamente
	pcXMLFileName = M4ANSIToCpp( ai_csXMLFileName, iXMLFileNameSize ) ;
	pcRichXMLFileName = M4ANSIToCpp( ai_csRichXMLFileName, iRichXMLFileNameSize ) ;
	
	iResult = poITransformer->GenerateRichXMLObjectData( pcXMLFileName, pcRichXMLFileName ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_generateRichXMLObjectData", "GenerateRichXMLObjectData", "%s#%s" ) ;

	delete [] pcXMLFileName ;
	delete [] pcRichXMLFileName ;

	return( TRUE ) ;
}


//=================================================================================
// Genera el fichero resultado de la mezcla del fichero XML de datos 
// de un objeto traspasable con el fichero XML complementario que contiene las 
// tablas del objeto, sus pks y los campos excluidos del proceso de generación del 
// fichero XML de datos.
//
// Pasos
//		
//
//=================================================================================

BOOL WINAPI M4MDTW_mergeXMLObjectData( LPCSTR ai_csXMLFileName, LPCSTR ai_csExclusionXMLFileName, LPCSTR ai_csMergedXMLFileName, LONG ao_lErrors )
{

	m4return_t			iResult = M4_ERROR;
	m4pchar_t			pcXMLFileName = NULL;
	m4pchar_t			pcExclusionXMLFileName = NULL;
	m4pchar_t			pcMergedXMLFileName = NULL;
	int					iXMLFileNameSize = -1 ;
	int					iExclusionXMLFileNameSize = -1 ;
	int					iMergedXMLFileNameSize = -1 ;
	IMDTTransformer		*poITransformer = NULL;
	ClMDTWErrorHandler	oErrorHandler;


	iResult = oErrorHandler.Init(&s_oMDTParameterDevice, ao_lErrors);
	M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_mergeXMLObjectData", "oErrorHandler.Init", "%s#%s");

	M4MDT_RETURN_ERROR2( ai_csXMLFileName == NULL, FALSE, M4MDTERROR_NULL_ARGUMENT, "ai_csXMLFileName", "M4MDTW_mergeXMLObjectData", "%s#%s");
	M4MDT_RETURN_ERROR2( ai_csExclusionXMLFileName == NULL, FALSE, M4MDTERROR_NULL_ARGUMENT, "ai_csExclusionXMLFileName", "M4MDTW_mergeXMLObjectData", "%s#%s");
	M4MDT_RETURN_ERROR2( ai_csMergedXMLFileName == NULL, FALSE, M4MDTERROR_NULL_ARGUMENT, "ai_csMergedXMLFileName", "M4MDTW_mergeXMLObjectData", "%s#%s");

	poITransformer = s_oMDTService.GetITransformer();
	M4MDT_RETURN_ERROR2(poITransformer == NULL, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_mergeXMLObjectData", "GetITransformer", "%s#%s");

	// Antonio ADN. Se estaban añadiendo las cadenas en ANSI (que se lo que envía VB).
	// Se pasa a CPP que es con lo que se trabaja internamente
	pcXMLFileName = M4ANSIToCpp( ai_csXMLFileName, iXMLFileNameSize );
	pcExclusionXMLFileName = M4ANSIToCpp( ai_csExclusionXMLFileName, iExclusionXMLFileNameSize );
	pcMergedXMLFileName = M4ANSIToCpp( ai_csMergedXMLFileName, iMergedXMLFileNameSize );

	iResult = poITransformer->MergeXMLObjectData(pcXMLFileName, pcExclusionXMLFileName, pcMergedXMLFileName);
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_mergeXMLObjectData", "MergeXMLObjectData", "%s#%s" ) ;

	return TRUE;
}


//=================================================================================
// Actualiza un repositorio a partir de las modificaciones en un XML de datos
// enriquecido
//
// Pasos
//		Activa la gestión de errores
//		Se chequea que los parámetros de entrada no sean nulos
//		Obtiene el transformador
//		Convierte las variables de entrada a las variables locales
//		Delega la ejecución
//
//=================================================================================
BOOL WINAPI M4MDTW_updateDB( LPCSTR ai_csIdConnection, LPCSTR ai_csXMLFileName, LPCSTR ai_csRichXMLFileName, LONG ao_lErrors )
{

	m4return_t			iResult = M4_ERROR ;
	m4pchar_t			pcIdConnection = NULL ;
	m4pchar_t			pcXMLFileName = NULL ;
	m4pchar_t			pcRichXMLFileName = NULL ;
	int					iIdConnectionSize = -1 ;
	int					iXMLFileNameSize = -1 ;
	int					iRichXMLFileNameSize = -1 ;
	IMDTTransformer		*poITransformer = NULL ;
	ClMDTWErrorHandler	oErrorHandler ;


	iResult = oErrorHandler.Init( &s_oMDTParameterDevice, ao_lErrors ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_openSession", "oErrorHandler.Init", "%s#%s" ) ;

	
	M4MDT_RETURN_ERROR2( ai_csIdConnection == NULL, FALSE, M4MDTERROR_NULL_ARGUMENT, "ai_csIdConnection", "M4MDTW_updateDB", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ai_csXMLFileName == NULL, FALSE, M4MDTERROR_NULL_ARGUMENT, "ai_csXMLFileName", "M4MDTW_updateDB", "%s#%s" ) ;
	M4MDT_RETURN_ERROR2( ai_csRichXMLFileName == NULL, FALSE, M4MDTERROR_NULL_ARGUMENT, "ai_csRichXMLFileName", "M4MDTW_updateDB", "%s#%s" ) ;


	poITransformer = s_oMDTService.GetITransformer() ;
	M4MDT_RETURN_ERROR2( poITransformer == NULL, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_updateDB", "GetITransformer", "%s#%s" ) ;

	// Antonio ADN. Se estaban añadiendo las cadenas en ANSI (que se lo que envía VB).
	// Se pasa a CPP que es con lo que se trabaja internamente
	pcIdConnection = M4ANSIToCpp( ai_csIdConnection, iIdConnectionSize ) ;
	pcXMLFileName = M4ANSIToCpp( ai_csXMLFileName, iXMLFileNameSize ) ;
	pcRichXMLFileName = M4ANSIToCpp( ai_csRichXMLFileName, iRichXMLFileNameSize ) ;
	
	iResult = poITransformer->UpdateDB( pcIdConnection, pcXMLFileName, pcRichXMLFileName ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, FALSE, M4MDTERROR_INTERNAL_ERROR, "M4MDTW_updateDB", "UpdateDB", "%s#%s" ) ;

	delete [] pcIdConnection ;
	delete [] pcXMLFileName ;
	delete [] pcRichXMLFileName ;

	return( TRUE ) ;
}

