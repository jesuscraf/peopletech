//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                m4varbase.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                15-05-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//   
//
//
//==============================================================================

#ifndef __M4VARBASE_HPP__
#define __M4VARBASE_HPP__

#include "m4variant_dll.hpp"
#include "m4sysalloc.hpp"
#include "cldefinebase.hpp"



union m4VariantData
{
	m4double_t	DoubleData;
	m4uint32_t	CharData;	//offset respecto al inicio del almacen de cadenas donde está esta cadena
	m4pchar_t	PointerChar;
};



class M4_DECL_M4VARIANT m4VariantTypeBase
{

public:
		m4VariantData Data;
		m4char_t      Type;	
		m4bool_t	  IsUser;

protected:
		m4bool_t	  m_bOwner;

public:

	// Default constructor
	m4VariantTypeBase( void );

	// Copy constructor
	m4VariantTypeBase( const m4VariantTypeBase &ai_var );

	// Basic types constructors
	m4VariantTypeBase( const m4double_t &ai_double );
	m4VariantTypeBase( const m4pchar_t &ai_char, m4bool_t ai_bCopy = M4_FALSE );

	// Destructor
    ~m4VariantTypeBase( void );
	/************************************
	************************************
	El destructor no es virtual debido a un BUG del compilador de MS:
	--	Si creas un array de 0 elementos de algo que tenga el destructor virtual, al destruirlo da GP
		porque intenta ir a la vftbl.
	Con esta solución ahora no hay problemas porque el destructor de la clase hija no hace nada.
	Pero podría haber problemas (memory leaks) en el futuro si se mete algo de funcionalidad en algun 
	destructor de alguna clase que derive de esta al llamarse al constructor de la hija y luego al de la 
	padres, pero solamente se llama al destructor de la padre.
	************************************
	************************************/
	
	
	m4VariantTypeBase &operator = ( const m4VariantTypeBase &ai_var );
	
	void FreeData( void );

	m4bool_t GetOwner( void ) const ;
	void SetOwner( m4bool_t ai_bOwner ) ;

	m4char_t GetType( void ) const;

	void SetDouble( m4double_t ai_double );
	void SetDate( m4date_t ai_date );
	void SetStringRef( const m4pchar_t ai_char, m4bool_t ai_bIsUser = M4_FALSE );
	void SetString( const m4pchar_t ai_char, m4bool_t ai_bIsUser = M4_FALSE );
	void SetSysString( const m4pchar_t ai_char, m4bool_t ai_bIsUser = M4_FALSE );
	void SetNull( void );

	void Set( const m4VariantTypeBase &ai_var, m4bool_t ai_bCopy = M4_FALSE );

protected:

	// Allocs space for this + ai_var strings.
	void AllocCatString( m4VariantTypeBase &ai_var );

};

#include "m4varbase.inl"

#endif	//__M4VARBASE_HPP__
