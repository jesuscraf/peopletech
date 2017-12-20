//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                m4var.hpp   
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

#ifndef __M4VAR_HPP__
#define __M4VAR_HPP__


#include "m4variant_dll.hpp"

//class ostream;
#include "m4stl.hpp"


#include "cldefine.hpp"
#include "m4system.hpp"
#include "m4varbase.hpp"

// For serializing
class ClGenericIODriver;

class ClSize;


class M4_DECL_M4VARIANT m4VariantType : public m4VariantTypeBase
{
public:

	// Default constructor
	m4VariantType( void ) : m4VariantTypeBase () {}

	// Copy constructor
	m4VariantType( const m4VariantType &ai_var );

	// Basic types constructors
	m4VariantType( const m4double_t &ai_double ) : m4VariantTypeBase( ai_double ) {}
	m4VariantType( const m4pchar_t &ai_char, m4bool_t ai_bCopy = M4_FALSE ) : m4VariantTypeBase (ai_char,ai_bCopy) {}
	
	// Destructor
    ~m4VariantType( void ) {}
	/************************************
	************************************
	El destructor de la clase padre no es virtual debido a un BUG del compilador de MS:
	--	Si creas un array de 0 elementos de algo que tenga el destructor virtual, al destruirlo da GP
		porque intenta ir a la vftbl.
	Con esta solución ahora no hay problemas porque el destructor de la clase hija no hace nada.
	Pero podría haber problemas (memory leaks) en el futuro si se mete algo de funcionalidad en algun 
	destructor de alguna clase que derive de esta al llamarse al constructor de la hija y luego al de la 
	padres, pero solamente se llama al destructor de la padre.
	************************************
	************************************/


	// Aritmethical operators
	m4VariantType&	Sum ( m4VariantType &ai_var, m4bool_t ai_bConcatStrings );
	m4VariantType&	Dec ( m4VariantType &ai_var );        
	m4VariantType&	Mult( m4VariantType &ai_var );
	m4VariantType&	Div ( m4VariantType &ai_var );
	m4VariantType&	Max ( m4VariantType &ai_var );
	m4VariantType&	Min ( m4VariantType &ai_var );

	//
	// Type conversion
	//
	// Return a double everywhere its expected
	operator m4double_t( void ) const;

	// Return a pointer to a string everywhere its expected
	operator m4pchar_t( void ) const;


	void Set( const m4VariantType &ai_var, m4bool_t ai_bCopy = M4_FALSE )
	{
		m4VariantTypeBase::Set( ai_var, ai_bCopy );
	}

	// Windows specific
#ifdef _WINDOWS

	// Constructor with variant
	m4VariantType( VARIANT& ai_VARIANT );

	// VARIANT Assignment
	m4VariantType&	operator = ( VARIANT &ai_VARIANT );

	// Assigns a VARIANT 
	void SetVARIANT( const VARIANT &ai_VARIANT ); 

	/**
	 * Da el valor del m4VariantType al VARIANT que se pasa como parámetro por referencia.
	 *
	 * @param ao_VARIANT	VARIANT por referencia al que se le da el valor del m4VariantType.
	 * @return void.
	 */
	void GetVARIANT( VARIANT& ao_VARIANT ) const;

#endif

	//
	// Serialization
	//
	m4return_t Serialize( ClGenericIODriver& IOD );
	m4return_t DeSerialize( ClGenericIODriver& IOD );
	
	m4return_t GetSize( ClSize &ao_size ) const;

};


// funcion de comparación de variants
typedef	int( *m4CompareVariant_f )( const m4VariantType*, const m4VariantType*, void* ) ;


m4bool_t M4_DECL_M4VARIANT operator == (const m4VariantType &ai_left, const m4VariantType &ai_right);
m4bool_t M4_DECL_M4VARIANT operator != (const m4VariantType &ai_left, const m4VariantType &ai_right);


// Funciones de comparacion de variants

int M4_DECL_M4VARIANT M4VariantIsEqual						( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux=0 ) ;
int M4_DECL_M4VARIANT M4VariantIsDistinct					( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux=0 ) ;
int M4_DECL_M4VARIANT M4VariantIsBigger						( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux=0 ) ;
int M4_DECL_M4VARIANT M4VariantIsSmaller					( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux=0 ) ;
int M4_DECL_M4VARIANT M4VariantIsBiggerOrEqual				( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux=0 ) ;
int M4_DECL_M4VARIANT M4VariantIsSmallerOrEqual				( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux=0 ) ;
int M4_DECL_M4VARIANT M4VariantMatchRegex					( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux=0 ) ;

int M4_DECL_M4VARIANT M4VariantIsEqualOrNull				( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux=0 ) ;
int M4_DECL_M4VARIANT M4VariantIsDistinctOrNull				( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux=0 ) ;
int M4_DECL_M4VARIANT M4VariantIsBiggerOrNull				( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux=0 ) ;
int M4_DECL_M4VARIANT M4VariantIsSmallerOrNull				( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux=0 ) ;
int M4_DECL_M4VARIANT M4VariantIsBiggerOrEqualOrNull		( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux=0 ) ;
int M4_DECL_M4VARIANT M4VariantIsSmallerOrEqualOrNull		( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux=0 ) ;
int M4_DECL_M4VARIANT M4VariantMatchRegexOrNull				( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux=0 ) ;

int M4_DECL_M4VARIANT M4VariantCaseIsEqual					( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux=0 ) ;
int M4_DECL_M4VARIANT M4VariantCaseIsDistinct				( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux=0 ) ;
int M4_DECL_M4VARIANT M4VariantCaseIsBigger					( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux=0 ) ;
int M4_DECL_M4VARIANT M4VariantCaseIsSmaller				( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux=0 ) ;
int M4_DECL_M4VARIANT M4VariantCaseIsBiggerOrEqual			( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux=0 ) ;
int M4_DECL_M4VARIANT M4VariantCaseIsSmallerOrEqual			( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux=0 ) ;
int M4_DECL_M4VARIANT M4VariantCaseMatchRegex				( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux=0 ) ;


int M4_DECL_M4VARIANT M4VariantCaseIsEqualOrNull			( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux=0 ) ;
int M4_DECL_M4VARIANT M4VariantCaseIsDistinctOrNull			( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux=0 ) ;
int M4_DECL_M4VARIANT M4VariantCaseIsBiggerOrNull			( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux=0 ) ;
int M4_DECL_M4VARIANT M4VariantCaseIsSmallerOrNull			( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux=0 ) ;
int M4_DECL_M4VARIANT M4VariantCaseIsBiggerOrEqualOrNull	( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux=0 ) ;
int M4_DECL_M4VARIANT M4VariantCaseIsSmallerOrEqualOrNull	( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux=0 ) ;
int M4_DECL_M4VARIANT M4VariantCaseMatchRegexOrNull			( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2, void *ai_pvAux=0 ) ;


m4int8_t M4_DECL_M4VARIANT M4VariantCompare( const m4VariantType *pstVariant1, const m4VariantType *pstVariant2 );


//dump facilities:

M4_DECL_M4VARIANT ostream & operator<< (ostream &ai_ofs, const m4VariantType &ai_v1);


extern m4CompareVariant_f M4_DECL_M4VARIANT g_apfM4VariantCompareFunctions[ M4CL_MAX_VARIANT_COMPARE_FUNCTION ] ;



#include "m4var.inl"



#endif
