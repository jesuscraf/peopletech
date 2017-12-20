
//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdtransfer
// File:                m4mdtvalue.cpp
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
//    Este módulo contiene las funciones de la clase valor de la
//    librería m4mdtransfer
//
//
//==============================================================================


#include "m4mdtvalue.hpp"




//=================================================================================
// Funciones de inicialización
//=================================================================================


//=================================================================================
//
// Constructor
//
// Inicialmente el valor es nulo
//
//=================================================================================

ClMDTValue::ClMDTValue( void )
{
	m_bIsNull = M4_TRUE ;
}



//=================================================================================
//
// Constructor
//
// Inicializa la clase estableciendo si es o no nulo
//
//=================================================================================

ClMDTValue::ClMDTValue( m4bool_t ai_bIsNull )
{
	m_bIsNull = ai_bIsNull ;
}



//=================================================================================
//
// Constructor
//
// Inicializa la clase a partir de un puntero de caracteres
//
//=================================================================================

ClMDTValue::ClMDTValue( m4pcchar_t ai_pccValue )
{
	Set( ai_pccValue ) ;
}



//=================================================================================
//
// Constructor
//
// Inicializa la clase a partir de un puntero de caracteres y un tamaño
//
//=================================================================================

ClMDTValue::ClMDTValue( m4pcchar_t ai_pccValue, m4uint32_t ai_iSize )
{
	Set( ai_pccValue, ai_iSize ) ;
}



//=================================================================================
//
// Constructor
//
// Inicializa la clase a partir de un string
//
//=================================================================================

ClMDTValue::ClMDTValue( const string& ai_rsValue )
{
	Set( ai_rsValue ) ;
}



//=================================================================================
//
// Constructor
//
// Inicializa la clase a partir de otro valor
//
//=================================================================================

ClMDTValue::ClMDTValue( const ClMDTValue& ai_rvValue )
{
	Set( ai_rvValue ) ;
}






//=================================================================================
//
// Destructor
//
// De momento no hace nada
//
//=================================================================================

ClMDTValue::~ClMDTValue( void )
{
}




//=================================================================================
// Funciones de valor nulo
//=================================================================================

//=================================================================================
// Devuelve si el valor es nulo o no
//=================================================================================

m4bool_t	ClMDTValue::IsNull( void ) const
{
	return( m_bIsNull ) ;
}



//=================================================================================
// Establece como nulo el valor
//=================================================================================

m4void_t	ClMDTValue::SetNull( void )
{
	m_bIsNull = M4_TRUE ;
	assign( "" ) ;
}



//=================================================================================
// Establece un valor a partir de un puntero a caracteres
// Si el puntero es nulo el valor queda como nulo
//=================================================================================

m4void_t	ClMDTValue::Set( m4pcchar_t ai_pccValue )
{
	if( ai_pccValue == NULL )
	{
		SetNull() ;
	}
	else
	{
		m_bIsNull = M4_FALSE ;
		assign( ai_pccValue ) ;
	}
}



//=================================================================================
// Establece un valor a partir de un puntero a caracteres y un tamaño
// Si el puntero es nulo el valor queda como nulo
//=================================================================================

m4void_t	ClMDTValue::Set( m4pcchar_t ai_pccValue, m4uint32_t ai_iSize  )
{
	if( ai_pccValue == NULL )
	{
		SetNull() ;
	}
	else
	{
		m_bIsNull = M4_FALSE ;
		assign( ai_pccValue, ai_iSize ) ;
	}
}



//=================================================================================
// Establece un valor a partir de un string
// El valor queda como no nulo
//=================================================================================

m4void_t	ClMDTValue::Set( const string& ai_rcsValue )
{
	m_bIsNull = M4_FALSE ;
	assign( ai_rcsValue, 0, ai_rcsValue.size() ) ;
}



//=================================================================================
// Establece un valor a partir de un valor
//=================================================================================

m4void_t	ClMDTValue::Set( const ClMDTValue& ai_rcvValue )
{
	m_bIsNull = ai_rcvValue.m_bIsNull ;
	assign( ai_rcvValue, 0, ai_rcvValue.size() ) ;
}


