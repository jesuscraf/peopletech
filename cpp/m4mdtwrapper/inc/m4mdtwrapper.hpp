
//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdtwrapper.dll
// File:                m4mdtwrapper.hpp   
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
//    Este m�dulo contiene el API VB que exporta la librer�a m4mdtransfer
//
//
//==============================================================================


#include "m4mdtwrapper_dll.hpp"

#include "wtypes.h"




#ifndef __M4MDTWRAPPER_HPP__
#define __M4MDTWRAPPER_HPP__



//=================================================================================
// Manejo de par�metros
//=================================================================================


//=================================================================================
//
// Crea un par�metro nuevo
//
// @param ao_plParameter
//
//			Handle del par�metro que se acaba de crear
//			-1 si ha habido alg�n error
//
// @return
//
//			TRUE si todo va bien o FALSE si ha habido un error
//
//=================================================================================
extern "C" M4_DECL_M4MDTWRAPPER BOOL WINAPI M4MDTW_createParameter( LPLONG ao_plParameter ) ;
								

//=================================================================================
//
// Libera un par�metro existente
//
// @param ai_lParameter
//
//			Handle del par�metro que se quiere borrar
//
// @return
//
//			TRUE si todo va bien o FALSE si ha habido un error
//
//=================================================================================
extern "C" M4_DECL_M4MDTWRAPPER BOOL WINAPI M4MDTW_releaseParameter( LONG ai_lParameter ) ;


//=================================================================================
//
// Crea una nueva pareja de valores, la rellena con los valores pasados y la a�ade
// al conjunto de parejas al final
// El valor de la pareja se crea de tipo cadena
//
// @param ai_lParameter
//
//			Handle del par�metro al que se a�ade la pareja de valores
//
// @param ai_sFirst
//
//			Par�metro con la clave de la pareja
//			Esta memoria no es liberada por la clase de par�metros, ya que no se
//			la queda en propiedad sino que hace una copia
//
// @param ai_sSecond
//
//			Par�metro con el valor de tipo cadena de la pareja
//			Esta memoria no es liberada por la clase de par�metros, ya que no se
//			la queda en propiedad sino que hace una copia
//
// @return
//
//			TRUE si todo va bien o FALSE si ha habido un error
//
//=================================================================================

extern "C" M4_DECL_M4MDTWRAPPER BOOL WINAPI M4MDTW_addParameterStringValue( LONG ai_lParameter, LPCSTR ai_csFirst, LPCSTR ai_csSecond ) ;


//=================================================================================
//
// Crea una nueva pareja de valores, la rellena con los valores pasados y la a�ade
// al conjunto de parejas al final
// El valor de la pareja se crea de tipo par�metro
//
// @param ai_lParameter
//
//			Handle del par�metro al que se a�ade la pareja de valores
//
// @param ai_sFirst
//
//			Par�metro con la clave de la pareja
//			Esta memoria no es liberada por la clase de par�metros, ya que no se
//			la queda en propiedad sino que hace una copia
//
// @param ai_lSecond
//
//			Par�metro con el valor de tipo par�metro de la pareja
//
// @return
//
//			TRUE si todo va bien o FALSE si ha habido un error
//
//=================================================================================
extern "C" M4_DECL_M4MDTWRAPPER BOOL WINAPI M4MDTW_addParameterHandleValue( LONG ai_lParameter, LPCSTR ai_csFirst, LONG ai_lSecond ) ;


//=================================================================================
//
// Elimina una nueva pareja de valores de tipo cadena
//
// @param ai_lParameter
//
//			Handle del par�metro del que se borra la pareja de valores
//
// @param ai_sFirst
//
//			Par�metro con la clave de la pareja
//
// @param ai_sSecond
//
//			Par�metro con el valor de tipo cadena de la pareja
//
// @return
//
//			TRUE si todo va bien o FALSE si ha habido un error
//
//=================================================================================
extern "C" M4_DECL_M4MDTWRAPPER BOOL WINAPI M4MDTW_removeParameterStringValue( LONG ai_lParameter, LPCSTR ai_csFirst, LPCSTR ai_csSecond ) ;

//=================================================================================
//
// Elimina una nueva pareja de valores de tipo par�metro
//
// @param ai_lParameter
//
//			Handle del par�metro del que se borra la pareja de valores
//
// @param ai_sFirst
//
//			Par�metro con la clave de la pareja
//
// @param ai_lSecond
//
//			Par�metro con el valor de tipo par�metro de la pareja
//
// @return
//
//			TRUE si todo va bien o FALSE si ha habido un error
//
//=================================================================================
extern "C" M4_DECL_M4MDTWRAPPER BOOL WINAPI M4MDTW_removeParameterHandleValue( LONG ai_lParameter, LPCSTR ai_csFirst, LONG ai_lSecond ) ;


//=================================================================================
//
// Obtiene el n�mero de parejas de valores de un par�metro
//
// @param ai_lParameter
//
//			Handle del par�metro que se quiere consultar
//
// @param ao_plSize
//
//			Par�metro de salida donde se deja el n�mero de parejas
//			0 si ha habido alg�n error
//
// @return
//
//			TRUE si todo va bien o FALSE si ha habido un error
//
//=================================================================================
extern "C" M4_DECL_M4MDTWRAPPER BOOL WINAPI M4MDTW_getParameterSize( LONG ai_lParameter, LPLONG ao_plSize ) ;


//=================================================================================
//
// Obtiene los valores de una pareja de valores de un par�metro
// La pareja se identifica por su posici�n dentro del conjunto
// El valor de la pareja debe ser de tipo cadena
//
// @param ai_lParameter
//
//			Handle del par�metro que se quiere consultar
//
// @param ai_lPosition
//
//			Posici�n de la pareja que se quiere consultar
//
// @param ao_psFirst
//
//			Par�metro de salida con la clave de la pareja
//			Esta memoria no debe ser liberada por el usuario
//			NULL si ha habido alg�n error
//
// @param ao_psSecond
//
//			Par�metro de salida con el valor de tipo cadena de la pareja
//			Esta memoria no debe ser liberada por el usuario
//			NULL si ha habido alg�n error
//
// @return
//
//			TRUE si todo va bien o FALSE si ha habido un error
//
//=================================================================================
extern "C" M4_DECL_M4MDTWRAPPER BOOL WINAPI M4MDTW_getParameterStringValue( LONG ai_lParameter, LONG ai_lPosition, LPBSTR ao_psFirst, LPBSTR ao_psSecond ) ;


//=================================================================================
//
// Obtiene los valores de una pareja de valores de un par�metro
// La pareja se identifica por su posici�n dentro del conjunto
// El valor de la pareja debe ser de tipo par�metro
//
// @param ai_lParameter
//
//			Handle del par�metro que se quiere consultar
//
// @param ai_lPosition
//
//			Posici�n de la pareja que se quiere consultar
//
// @param ao_psFirst
//
//			Par�metro de salida con la clave de la pareja
//			Esta memoria no debe ser liberada por el usuario
//			NULL si ha habido alg�n error
//
// @param ao_plSecond
//
//			Par�metro de salida con el valor de tipo par�metro de la pareja
//			-1 si ha habido alg�n error
//
// @return
//
//			TRUE si todo va bien o FALSE si ha habido un error
//
//=================================================================================
extern "C" M4_DECL_M4MDTWRAPPER BOOL WINAPI M4MDTW_getParameterHandleValue( LONG ai_lParameter, LONG ai_lPosition, LPBSTR ao_psFirst, LPLONG ao_plSecond ) ;









//=================================================================================
// Funciones del API
//=================================================================================


//=================================================================================
//
// Inicializa la librer�a con los par�metros externos necesarios
//
// @param ai_lConnections
//
//			Array de conexiones. Viene dado por parejas de identificador-array
//			de valores. Y el array de valores viene dado por una pareja de
//			tipo de base de datos-DSN
//
// @param ai_csDictionaryConnection
//
//			DSN de la conexi�n del diccionario
//
// @param ai_csWorkingDir
//
//			Directorio de trabajo con permisos de lectura
//
// @param ai_csDebugDir
//
//			Directorio de depuraci�n con permisos de escritura
//
// @param ai_csCacheDir
//
//			Directorio de cach�s con permisos de escritura
//
// @param ai_lDataBase
//
//			N�mero que identifica a la base de datos
//
// @param ai_lLanguage
//
//			N�mero que representa el idioma en el que se trabaja
//
// @param ai_lVersion
//
//			N�mero de la versi�n en la que se est� trabajando
//
// @param ai_lTraceLevel
//
//			Nivel de traza
//
// @param ao_lErrors
//
//			Array de errores producidos en la funci�n. Viene dado por parejas
//			tipo-mensaje
//
// @return
//
//			TRUE si todo va bien o FALSE si ha habido un error
//
//=================================================================================
extern "C" M4_DECL_M4MDTWRAPPER BOOL WINAPI M4MDTW_openSession( LONG ai_lConnections, LPCSTR ai_csDictionaryConnection, LPCSTR ai_csWorkingDir, LPCSTR ai_csDebugDir, LPCSTR ai_csCacheDir, LONG ai_lDataBase, LONG ai_lLanguage, LONG ai_lVersion, LONG ai_lTraceLevel, LONG ao_lErrors ) ;


//=================================================================================
//
// Libera los recursos asociados a la librer�a
//
// @param ao_lErrors
//
//			Array de errores producidos en la funci�n. Viene dado por parejas
//			tipo-mensaje
//
// @return
//
//			TRUE si todo va bien o FALSE si ha habido un error
//
//=================================================================================
extern "C" M4_DECL_M4MDTWRAPPER BOOL WINAPI M4MDTW_closeSession( LONG ao_lErrors ) ;


//=================================================================================
//
// Genera el XML de datos de un objeto traspasable para un repositorio concreto.
// Adem�s proporciona una lista de los hijos de ese objeto
//
// @param ai_csIdConnection
//
//			Identificador de la conexi�n del repositorio que se debe emplear
//
// @param ai_csIdObject
//
//			Identificador del objeto que se quiere traspasar
//
// @param ai_lPKs
//
//			Array de pks del objeto a traspasar. Viene dado por parejas de
//			pk-valor
//
// @param ai_csXMLFileName
//
//			Nombre del fichero que se debe generar con el XML de datos
//
// @param ao_lChilds
//
//			Array con los hijos del objeto a traspasar. Viene dado por parejas de
//			identificador-array de pks. Y el array de pks viene dado por parejas de
//			pk-valor
//
// @param ao_lErrors
//
//			Array de errores producidos en la funci�n. Viene dado por parejas
//			tipo-mensaje
//
// @return
//
//			TRUE si todo va bien o FALSE si ha habido un error
//
//=================================================================================
extern "C" M4_DECL_M4MDTWRAPPER BOOL WINAPI M4MDTW_generateXMLObjectData( LPCSTR ai_csIdConnection, LPCSTR ai_csIdObject, LONG ai_lPKs, LPCSTR ai_csXMLFileName, LPCSTR ai_csExclusionXMLFileName, LONG ao_lChilds, LONG ao_lErrors ) ;

//=================================================================================
//
// Genera el XML de datos enriquecido (traducido) de un objeto traspasable a partir
// de su XML de datos
//
// @param ai_csXMLFileName
//
//			Nombre del fichero original con el XML de datos
//
// @param ai_csRichXMLFileName
//
//			Nombre del fichero que se debe generar con el XML de datos enriquecido
//
// @param ao_lErrors
//
//			Array de errores producidos en la funci�n. Viene dado por parejas
//			tipo-mensaje
//
// @return
//
//			TRUE si todo va bien o FALSE si ha habido un error
//
//=================================================================================
extern "C" M4_DECL_M4MDTWRAPPER BOOL WINAPI M4MDTW_generateRichXMLObjectData( LPCSTR ai_csXMLFileName, LPCSTR ai_csRichXMLFileName, LONG ao_lErrors ) ;


//=================================================================================
// Genera el fichero resultado de la mezcla del fichero XML de datos 
// de un objeto traspasable con el fichero XML complementario que contiene las 
// tablas del objeto, sus pks y los campos excluidos del proceso de generaci�n del 
// fichero XML de datos.
//
// Pasos
//		
//
//=================================================================================
extern "C" M4_DECL_M4MDTWRAPPER BOOL WINAPI M4MDTW_mergeXMLObjectData( LPCSTR ai_csXMLFileName, LPCSTR ai_csXMLExclusionFileName, LPCSTR ai_csXMLMergedFileName, LONG ao_lErrors );


//=================================================================================
//
// Actualiza un repositorio a partir de las modificaciones en un XML de datos
// enriquecido
//
// @param ai_csIdConnection
//
//			Identificador de la conexi�n del repositorio que se debe emplear
//
// @param ai_csXMLFileName
//
//			Nombre del fichero original con el XML de datos
//
// @param ai_csRichXMLFileName
//
//			Nombre del fichero con el XML de datos enriquecido y con las
//			del usuario modificaciones
//
// @param ao_lErrors
//
//			Array de errores producidos en la funci�n. Viene dado por parejas
//			tipo-mensaje
//
// @return
//
//			TRUE si todo va bien o FALSE si ha habido un error
//
//=================================================================================
extern "C" M4_DECL_M4MDTWRAPPER BOOL WINAPI M4MDTW_updateDB( LPCSTR ai_csIdConnection, LPCSTR ai_csXMLFileName, LPCSTR ai_csRichXMLFileName, LONG ao_lErrors ) ;




#endif



