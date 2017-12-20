
//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdtransfer
// File:                m4imdtparameter.hpp
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                30-03-2004
// Language:            C++
// Operating System:    WINDOWS, UNIX
// Design Document:     
//
//
// Definition:
//
//    Este m�dulo contiene el interfaz de par�metros de la librer�a
//    m4mdtransfer
//    Los par�metros son arrays de valores que permiten el paso de datos
//    complejos entre la librer�a m4easysp y VB
//    Un argumento complejo viene representado por un handle (m4int32_t)
//    -1 es un handle no v�lido
//    Un argumento complejo almacena un conjunto de parejas de valores
//    Las parejas de valores pueden ser 2 cadenas (clave, valor) o bien
//    una cadena y un handle de argumento complejo (clave, argumento)
//    La segunda opci�n permite tener agumentos complejos en forma de �rbol
//    El orden de las parejas vienen dado por el orden en que se a�aden al
//    conjunto
//
//
//==============================================================================


#include "m4mdtransfer_dll.hpp"

#include "m4types.hpp"




#ifndef __M4IMDTPARAMETER_HPP__
#define __M4IMDTPARAMETER_HPP__




//=================================================================================
//
// IReadParameterManager
//
// Interfaz de lectura de par�metros
//
//=================================================================================

class	M4_DECL_M4MDTRANSFER	IMDTReadParameterManager
{

public:


	//=================================================================================
	//
	// Obtiene el n�mero de parejas de valores de un par�metro
	//
	// @param ai_iParameter
	//
	//			Handle del par�metro que se quiere consultar
	//
	// @param ao_riSize
	//
	//			Par�metro de salida donde se deja el n�mero de parejas
	//			0 si ha habido alg�n error
	//
	// @return
	//
	//			M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	virtual	m4return_t	GetParameterSize( m4int32_t ai_iParameter, m4uint32_t& ao_riSize ) const = 0 ;


	//=================================================================================
	//
	// Obtiene los valores de una pareja de valores de un par�metro
	// La pareja se identifica por su posici�n dentro del conjunto
	// El valor de la pareja debe ser de tipo cadena
	//
	// @param ai_iParameter
	//
	//			Handle del par�metro que se quiere consultar
	//
	// @param ai_iPosition
	//
	//			Posici�n de la pareja que se quiere consultar
	//
	// @param ao_rpccFirst
	//
	//			Par�metro de salida con la clave de la pareja
	//			Esta memoria no debe ser liberada por el usuario
	//			NULL si ha habido alg�n error
	//
	// @param ai_rpccSecond
	//
	//			Par�metro de salida con el valor de tipo cadena de la pareja
	//			Esta memoria no debe ser liberada por el usuario
	//			NULL si ha habido alg�n error
	//
	// @return
	//
	//			M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	virtual	m4return_t	GetParameterValue( m4int32_t ai_iParameter, m4uint32_t ai_iPosition, m4pcchar_t& ao_rpccFirst, m4pcchar_t& ai_rpccSecond ) const = 0 ;


	//=================================================================================
	//
	// Obtiene los valores de una pareja de valores de un par�metro
	// La pareja se identifica por su posici�n dentro del conjunto
	// El valor de la pareja debe ser de tipo par�metro
	//
	// @param ai_iParameter
	//
	//			Handle del par�metro que se quiere consultar
	//
	// @param ai_iPosition
	//
	//			Posici�n de la pareja que se quiere consultar
	//
	// @param ao_rpccFirst
	//
	//			Par�metro de salida con la clave de la pareja
	//			Esta memoria no debe ser liberada por el usuario
	//			NULL si ha habido alg�n error
	//
	// @param ai_riSecond
	//
	//			Par�metro de salida con el valor de tipo par�metro de la pareja
	//			-1 si ha habido alg�n error
	//
	// @return
	//
	//			M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	virtual	m4return_t	GetParameterValue( m4int32_t ai_iParameter, m4uint32_t ai_iPosition, m4pcchar_t& ao_rpccFirst, m4int32_t& ai_riSecond ) const = 0 ;

} ;



//=================================================================================
//
// IWriteParameterManager
//
// Interfaz de escritura de par�metros
//
//=================================================================================

class	M4_DECL_M4MDTRANSFER	IMDTWriteParameterManager
{

public:


	//=================================================================================
	//
	// Crea un par�metro nuevo
	//
	// @param ao_riParameter
	//
	//			Handle del par�metro que se acaba de crear
	//			-1 si ha habido alg�n error
	//
	// @return
	//
	//			M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	virtual	m4return_t	CreateParameter( m4int32_t& ao_riParameter ) = 0 ;


	//=================================================================================
	//
	// Libera un par�metro existente
	//
	// @param ai_iParameter
	//
	//			Handle del par�metro que se quiere borrar
	//
	// @return
	//
	//			M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	virtual	m4return_t	ReleaseParameter( m4int32_t ai_iParameter ) = 0 ;


	//=================================================================================
	//
	// Crea una nueva pareja de valores, la rellena con los valores pasados y la a�ade
	// al conjunto de parejas al final
	// El valor de la pareja se crea de tipo cadena
	//
	// @param ai_iParameter
	//
	//			Handle del par�metro al que se a�ade la pareja de valores
	//
	// @param ai_pccFirst
	//
	//			Par�metro con la clave de la pareja
	//			Esta memoria no es liberada por la clase de par�metros, ya que no se
	//			la queda en propiedad sino que hace una copia
	//
	// @param ai_pccSecond
	//
	//			Par�metro con el valor de tipo cadena de la pareja
	//			Esta memoria no es liberada por la clase de par�metros, ya que no se
	//			la queda en propiedad sino que hace una copia
	//
	// @return
	//
	//			M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	virtual	m4return_t	AddParameterValue( m4int32_t ai_iParameter, m4pcchar_t ai_pccFirst, m4pcchar_t ai_pccSecond ) = 0 ;


	//=================================================================================
	//
	// Crea una nueva pareja de valores, la rellena con los valores pasados y la a�ade
	// al conjunto de parejas al final
	// El valor de la pareja se crea de tipo par�metro
	//
	// @param ai_iParameter
	//
	//			Handle del par�metro al que se a�ade la pareja de valores
	//
	// @param ai_pccFirst
	//
	//			Par�metro con la clave de la pareja
	//			Esta memoria no es liberada por la clase de par�metros, ya que no se
	//			la queda en propiedad sino que hace una copia
	//
	// @param ai_iSecond
	//
	//			Par�metro con el valor de tipo par�metro de la pareja
	//
	// @return
	//
	//			M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	virtual	m4return_t	AddParameterValue( m4int32_t ai_iParameter, m4pcchar_t ai_pccFirst, m4int32_t ai_iSecond ) = 0 ;


	//=================================================================================
	//
	// Elimina una nueva pareja de valores de tipo cadena
	//
	// @param ai_iParameter
	//
	//			Handle del par�metro del que se borra la pareja de valores
	//
	// @param ai_pccFirst
	//
	//			Par�metro con la clave de la pareja
	//
	// @param ai_pccSecond
	//
	//			Par�metro con el valor de tipo cadena de la pareja
	//
	// @return
	//
	//			M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	virtual	m4return_t	RemoveParameterValue( m4int32_t ai_iParameter, m4pcchar_t ai_pccFirst, m4pcchar_t ai_pccSecond ) = 0 ;


	//=================================================================================
	//
	// Elimina una nueva pareja de valores de tipo par�metro
	//
	// @param ai_iParameter
	//
	//			Handle del par�metro del que se borra la pareja de valores
	//
	// @param ai_pccFirst
	//
	//			Par�metro con la clave de la pareja
	//
	// @param ai_iSecond
	//
	//			Par�metro con el valor de tipo par�metro de la pareja
	//
	// @return
	//
	//			M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	virtual	m4return_t	RemoveParameterValue( m4int32_t ai_iParameter, m4pcchar_t ai_pccFirst, m4int32_t ai_iSecond ) = 0 ;

} ;



//=================================================================================
//
// IMDTReadWriteParameterManager
//
// Interfaz de lectura escritura de par�metros
//
//=================================================================================

class	M4_DECL_M4MDTRANSFER	IMDTReadWriteParameterManager : public IMDTReadParameterManager, public IMDTWriteParameterManager
{
} ;


#endif


