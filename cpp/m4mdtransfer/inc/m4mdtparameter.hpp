
//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdtransfer
// File:                m4mdtparameter.hpp
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
//    Este módulo contiene la declaración de la clase de parámetros de la
//    librería m4mdtransfer
//
//
//==============================================================================


#include "m4stl.hpp"
#include "m4imdtparameter.hpp"





#ifndef __M4MDTPARAMETER_HPP__
#define __M4MDTPARAMETER_HPP__



//=================================================================================
// Defines
//=================================================================================


// Tipos de parejas de parámetros. O cadena o argumento complejo
typedef enum {
	
	m4mdt_string	=	0,
	m4mdt_parameter	=	1

} ParameterValueType_t;


// Estructura de una pareja de valores. Tiene 2 elementos: el primero
// es una cadena y el segundo una cadena o un argumento complejo
typedef struct {

	m4pchar_t	First ;

	ParameterValueType_t	Type ;
	union {

		m4int32_t	Parameter ;
		m4pchar_t	Value ;
	} Second ;

} ParameterValue_t;


// Vector de parejas de parámetro
typedef vector< ParameterValue_t >									ParameterValueVector_t ;

// Mapa de parámetros
typedef map< m4int32_t, ParameterValueVector_t*, less <m4int32_t> >	ParameterMap_t ;






//=================================================================================
//
// ClMDTParameterManager
//
// Clase que implementa el gestor de parámetros
//
//=================================================================================

class	ClMDTParameterManager : public IMDTReadWriteParameterManager
{

public:


//=================================================================================
// Funciones de inicialización
//=================================================================================

		ClMDTParameterManager( void ) ;
		virtual ~ClMDTParameterManager( void ) ;


	//=================================================================================
	//
	// Inicializa el objeto
	//
	// @return
	//
	//			M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	Init( void ) ;



//=================================================================================
// Funciones del interfaz
//=================================================================================

	//=================================================================================
	// Obtiene el número de parejas de valores de un parámetro
	// Los parámetros están estecificados en el interfaz
	//=================================================================================
	virtual	m4return_t	GetParameterSize( m4int32_t ai_iParameter, m4uint32_t& ao_riSize ) const ;

	//=================================================================================
	// Obtiene los valores de una pareja de valores de un parámetro
	// La pareja se identifica por su posición dentro del conjunto
	// El valor de la pareja debe ser de tipo cadena
	// Los parámetros están estecificados en el interfaz
	//=================================================================================
	virtual	m4return_t	GetParameterValue( m4int32_t ai_iParameter, m4uint32_t ai_iPosition, m4pcchar_t& ao_rpccFirst, m4pcchar_t& ai_rpccSecond ) const ;

	//=================================================================================
	// Obtiene los valores de una pareja de valores de un parámetro
	// La pareja se identifica por su posición dentro del conjunto
	// El valor de la pareja debe ser de tipo parámetro
	// Los parámetros están estecificados en el interfaz
	//=================================================================================
	virtual	m4return_t	GetParameterValue( m4int32_t ai_iParameter, m4uint32_t ai_iPosition, m4pcchar_t& ao_rpccFirst, m4int32_t& ai_riSecond ) const ;

	//=================================================================================
	// Crea un parámetro nuevo
	// Los parámetros están estecificados en el interfaz
	//=================================================================================
	virtual	m4return_t	CreateParameter( m4int32_t& ao_riParameter ) ;

	//=================================================================================
	// Libera un parámetro existente
	// Los parámetros están estecificados en el interfaz
	//=================================================================================
	virtual	m4return_t	ReleaseParameter( m4int32_t ai_iParameter ) ;

	//=================================================================================
	// Crea una nueva pareja de valores, la rellena con los valores pasados y la añade
	// al conjunto de parejas al final
	// El valor de la pareja se crea de tipo cadena
	// Los parámetros están estecificados en el interfaz
	//=================================================================================
	virtual	m4return_t	AddParameterValue( m4int32_t ai_iParameter, m4pcchar_t ai_pccFirst, m4pcchar_t ai_pccSecond ) ;

	//=================================================================================
	// Crea una nueva pareja de valores, la rellena con los valores pasados y la añade
	// al conjunto de parejas al final
	// El valor de la pareja se crea de tipo parámetro
	// Los parámetros están estecificados en el interfaz
	//=================================================================================
	virtual	m4return_t	AddParameterValue( m4int32_t ai_iParameter, m4pcchar_t ai_pccFirst, m4int32_t ai_iSecond ) ;

	//=================================================================================
	// Elimina una nueva pareja de valores de tipo cadena
	// Los parámetros están estecificados en el interfaz
	//=================================================================================
	virtual	m4return_t	RemoveParameterValue( m4int32_t ai_iParameter, m4pcchar_t ai_pccFirst, m4pcchar_t ai_pccSecond ) ;

	//=================================================================================
	// Elimina una nueva pareja de valores de tipo parámetro
	// Los parámetros están estecificados en el interfaz
	//=================================================================================
	virtual	m4return_t	RemoveParameterValue( m4int32_t ai_iParameter, m4pcchar_t ai_pccFirst, m4int32_t ai_iSecond ) ;




protected:

//=================================================================================
// Funciones auxiliares
//=================================================================================

	//=================================================================================
	//
	// Obtiene el vector de valores de un parámetro identificado por su handle
	//
	// @param ai_iParameter
	//
	//			Handle del parámetro que se quiere consultar
	//
	// @return
	//			El vector de valores o NULL si ha habido algún error
	//
	//=================================================================================
	ParameterValueVector_t*	_GetParameter( m4int32_t ai_iParameter ) const ;

	//=================================================================================
	//
	// Obtiene la estructura con los valores de una pareja de valores de un parámetro
	// La pareja se identifica por su posición dentro del conjunto
	//
	// @param ai_iParameter
	//
	//			Handle del parámetro que se quiere consultar
	//
	// @param ai_iPosition
	//
	//			Posición de la pareja que se quiere consultar
	//
	// @return
	//			La pareja de valores o NULL si ha habido algún error
	//
	//=================================================================================
	ParameterValue_t*	_GetParameterValue( m4int32_t ai_iParameter, m4uint32_t ai_iPosition ) const ;

	//=================================================================================
	//
	// Libera un parámetro existente
	//
	// @param ai_iParameter
	//
	//			Handle del parámetro que se quiere borrar
	//
	// @param ai_bRecursive
	//
	//			Indica si se deben (M4_TRUE) o no (M4_FALSE) borrar los hijos de
	//			tipo parámetro de las parejas de valores del parámetro
	//
	// @return
	//
	//			M4_SUCCESS si todo va bien o M4_ERROR si ha habido un error
	//
	//=================================================================================
	m4return_t	_ReleaseParameter( m4int32_t ai_iParameter, m4bool_t ai_bRecursive ) ;



//=================================================================================
// Miembros
//=================================================================================

	//=================================================================================
	// Ordinal consecutiva del último handle de parámetro creado
	//=================================================================================
	m4int32_t		m_iLastParameter ;

	//=================================================================================
	// Conjunto de parámetros dados por sus handles
	//=================================================================================
	ParameterMap_t	m_oParameters ;

} ;



#endif


