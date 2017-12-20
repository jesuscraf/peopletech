
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
//    Este módulo contiene el API VB que exporta la librería m4mdtransfer
//
//
//==============================================================================


#include "m4mdtwrapper_dll.hpp"

#include "wtypes.h"




#ifndef __M4MDTWRAPPER_HPP__
#define __M4MDTWRAPPER_HPP__



//=================================================================================
// Manejo de parámetros
//=================================================================================


//=================================================================================
//
// Crea un parámetro nuevo
//
// @param ao_plParameter
//
//			Handle del parámetro que se acaba de crear
//			-1 si ha habido algún error
//
// @return
//
//			TRUE si todo va bien o FALSE si ha habido un error
//
//=================================================================================
extern "C" M4_DECL_M4MDTWRAPPER BOOL WINAPI M4MDTW_createParameter( LPLONG ao_plParameter ) ;
								

//=================================================================================
//
// Libera un parámetro existente
//
// @param ai_lParameter
//
//			Handle del parámetro que se quiere borrar
//
// @return
//
//			TRUE si todo va bien o FALSE si ha habido un error
//
//=================================================================================
extern "C" M4_DECL_M4MDTWRAPPER BOOL WINAPI M4MDTW_releaseParameter( LONG ai_lParameter ) ;


//=================================================================================
//
// Crea una nueva pareja de valores, la rellena con los valores pasados y la añade
// al conjunto de parejas al final
// El valor de la pareja se crea de tipo cadena
//
// @param ai_lParameter
//
//			Handle del parámetro al que se añade la pareja de valores
//
// @param ai_sFirst
//
//			Parámetro con la clave de la pareja
//			Esta memoria no es liberada por la clase de parámetros, ya que no se
//			la queda en propiedad sino que hace una copia
//
// @param ai_sSecond
//
//			Parámetro con el valor de tipo cadena de la pareja
//			Esta memoria no es liberada por la clase de parámetros, ya que no se
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
// Crea una nueva pareja de valores, la rellena con los valores pasados y la añade
// al conjunto de parejas al final
// El valor de la pareja se crea de tipo parámetro
//
// @param ai_lParameter
//
//			Handle del parámetro al que se añade la pareja de valores
//
// @param ai_sFirst
//
//			Parámetro con la clave de la pareja
//			Esta memoria no es liberada por la clase de parámetros, ya que no se
//			la queda en propiedad sino que hace una copia
//
// @param ai_lSecond
//
//			Parámetro con el valor de tipo parámetro de la pareja
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
//			Handle del parámetro del que se borra la pareja de valores
//
// @param ai_sFirst
//
//			Parámetro con la clave de la pareja
//
// @param ai_sSecond
//
//			Parámetro con el valor de tipo cadena de la pareja
//
// @return
//
//			TRUE si todo va bien o FALSE si ha habido un error
//
//=================================================================================
extern "C" M4_DECL_M4MDTWRAPPER BOOL WINAPI M4MDTW_removeParameterStringValue( LONG ai_lParameter, LPCSTR ai_csFirst, LPCSTR ai_csSecond ) ;

//=================================================================================
//
// Elimina una nueva pareja de valores de tipo parámetro
//
// @param ai_lParameter
//
//			Handle del parámetro del que se borra la pareja de valores
//
// @param ai_sFirst
//
//			Parámetro con la clave de la pareja
//
// @param ai_lSecond
//
//			Parámetro con el valor de tipo parámetro de la pareja
//
// @return
//
//			TRUE si todo va bien o FALSE si ha habido un error
//
//=================================================================================
extern "C" M4_DECL_M4MDTWRAPPER BOOL WINAPI M4MDTW_removeParameterHandleValue( LONG ai_lParameter, LPCSTR ai_csFirst, LONG ai_lSecond ) ;


//=================================================================================
//
// Obtiene el número de parejas de valores de un parámetro
//
// @param ai_lParameter
//
//			Handle del parámetro que se quiere consultar
//
// @param ao_plSize
//
//			Parámetro de salida donde se deja el número de parejas
//			0 si ha habido algún error
//
// @return
//
//			TRUE si todo va bien o FALSE si ha habido un error
//
//=================================================================================
extern "C" M4_DECL_M4MDTWRAPPER BOOL WINAPI M4MDTW_getParameterSize( LONG ai_lParameter, LPLONG ao_plSize ) ;


//=================================================================================
//
// Obtiene los valores de una pareja de valores de un parámetro
// La pareja se identifica por su posición dentro del conjunto
// El valor de la pareja debe ser de tipo cadena
//
// @param ai_lParameter
//
//			Handle del parámetro que se quiere consultar
//
// @param ai_lPosition
//
//			Posición de la pareja que se quiere consultar
//
// @param ao_psFirst
//
//			Parámetro de salida con la clave de la pareja
//			Esta memoria no debe ser liberada por el usuario
//			NULL si ha habido algún error
//
// @param ao_psSecond
//
//			Parámetro de salida con el valor de tipo cadena de la pareja
//			Esta memoria no debe ser liberada por el usuario
//			NULL si ha habido algún error
//
// @return
//
//			TRUE si todo va bien o FALSE si ha habido un error
//
//=================================================================================
extern "C" M4_DECL_M4MDTWRAPPER BOOL WINAPI M4MDTW_getParameterStringValue( LONG ai_lParameter, LONG ai_lPosition, LPBSTR ao_psFirst, LPBSTR ao_psSecond ) ;


//=================================================================================
//
// Obtiene los valores de una pareja de valores de un parámetro
// La pareja se identifica por su posición dentro del conjunto
// El valor de la pareja debe ser de tipo parámetro
//
// @param ai_lParameter
//
//			Handle del parámetro que se quiere consultar
//
// @param ai_lPosition
//
//			Posición de la pareja que se quiere consultar
//
// @param ao_psFirst
//
//			Parámetro de salida con la clave de la pareja
//			Esta memoria no debe ser liberada por el usuario
//			NULL si ha habido algún error
//
// @param ao_plSecond
//
//			Parámetro de salida con el valor de tipo parámetro de la pareja
//			-1 si ha habido algún error
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
// Inicializa la librería con los parámetros externos necesarios
//
// @param ai_lConnections
//
//			Array de conexiones. Viene dado por parejas de identificador-array
//			de valores. Y el array de valores viene dado por una pareja de
//			tipo de base de datos-DSN
//
// @param ai_csDictionaryConnection
//
//			DSN de la conexión del diccionario
//
// @param ai_csWorkingDir
//
//			Directorio de trabajo con permisos de lectura
//
// @param ai_csDebugDir
//
//			Directorio de depuración con permisos de escritura
//
// @param ai_csCacheDir
//
//			Directorio de cachés con permisos de escritura
//
// @param ai_lDataBase
//
//			Número que identifica a la base de datos
//
// @param ai_lLanguage
//
//			Número que representa el idioma en el que se trabaja
//
// @param ai_lVersion
//
//			Número de la versión en la que se está trabajando
//
// @param ai_lTraceLevel
//
//			Nivel de traza
//
// @param ao_lErrors
//
//			Array de errores producidos en la función. Viene dado por parejas
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
// Libera los recursos asociados a la librería
//
// @param ao_lErrors
//
//			Array de errores producidos en la función. Viene dado por parejas
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
// Además proporciona una lista de los hijos de ese objeto
//
// @param ai_csIdConnection
//
//			Identificador de la conexión del repositorio que se debe emplear
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
//			Array de errores producidos en la función. Viene dado por parejas
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
//			Array de errores producidos en la función. Viene dado por parejas
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
// tablas del objeto, sus pks y los campos excluidos del proceso de generación del 
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
//			Identificador de la conexión del repositorio que se debe emplear
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
//			Array de errores producidos en la función. Viene dado por parejas
//			tipo-mensaje
//
// @return
//
//			TRUE si todo va bien o FALSE si ha habido un error
//
//=================================================================================
extern "C" M4_DECL_M4MDTWRAPPER BOOL WINAPI M4MDTW_updateDB( LPCSTR ai_csIdConnection, LPCSTR ai_csXMLFileName, LPCSTR ai_csRichXMLFileName, LONG ao_lErrors ) ;




#endif



