
//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdtransfer
// File:                m4mdtvalue.hpp
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                11-05-2004
// Language:            C++
// Operating System:    WINDOWS, UNIX
// Design Document:     
//
//
// Definition:
//
//    Este módulo contiene la declaración de la clase valor de la
//    librería m4mdtransfer
//
//
//==============================================================================


#include "m4types.hpp"
#include "m4stl.hpp"



#ifndef __M4MDTVALUE_HPP__
#define __M4MDTVALUE_HPP__




//=================================================================================
//
// ClMDTValue
//
// Clase que implementa el tipo de datos que representa un valor que se va
// leer o insertar en la base de datos. Permite la representación de valores
// nulos
//
//=================================================================================

class	ClMDTValue : public string
{

public:


//=================================================================================
// Funciones de inicialización
//=================================================================================

		ClMDTValue( void ) ;
		ClMDTValue( m4bool_t ai_bIsNull ) ;
		ClMDTValue( m4pcchar_t ai_pccValue ) ;
		ClMDTValue( m4pcchar_t ai_pccValue, m4uint32_t ai_iSize ) ;
		ClMDTValue( const string& ai_rsValue ) ;
		ClMDTValue( const ClMDTValue& ai_rvValue ) ;

		virtual ~ClMDTValue( void ) ;


//=================================================================================
// Funciones de valor nulo
//=================================================================================

	//=================================================================================
	//
	// Devuelve si el valor es nulo o no
	//
	// @return
	//
	//		M4_TRUE si el valor es nulo o M4_FALSE si no es nulo
	//
	//=================================================================================
	m4bool_t	IsNull( void ) const ;

	//=================================================================================
	// Establece como nulo el valor
	//=================================================================================
	m4void_t	SetNull( void ) ;

	//=================================================================================
	//
	// Establece un valor a partir de un puntero a caracteres
	//
	// @param ai_pccValue
	//
	//		 Puntero a asignar
	//
	//=================================================================================
	m4void_t	Set( m4pcchar_t ai_pccValue ) ;

	//=================================================================================
	//
	// Establece un valor a partir de un puntero a caracteres y un tamaño
	//
	// @param ai_pccValue
	//
	//		 Puntero a asignar
	//
	// @param ai_iSize
	//
	//		 Tamaño de la cadena
	//
	//=================================================================================
	m4void_t	Set( m4pcchar_t ai_pccValue, m4uint32_t ai_iSize  ) ;

	//=================================================================================
	//
	// Establece un valor a partir de un string
	//
	// @param ai_rcsValue
	//
	//		 String a asignar
	//
	//=================================================================================
	m4void_t	Set( const string& ai_rcsValue ) ;

	//=================================================================================
	//
	// Establece un valor a partir de un valor
	//
	// @param ai_rvValue
	//
	//		 Valor a asignar
	//
	//=================================================================================
	m4void_t	Set( const ClMDTValue& ai_rcvValue ) ;


protected:


//=================================================================================
// Miembros
//=================================================================================

	//=================================================================================
	// Parámetro que indica si el error es nulo o no
	//=================================================================================
	m4bool_t	m_bIsNull ;

} ;




#endif


