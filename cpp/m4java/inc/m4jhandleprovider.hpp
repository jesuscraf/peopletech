
//==============================================================================
//
// (c) Copyright  1991-2015 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4java.dll
// File:                m4jhandleprovider.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                21-01-2015
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines a class for providing java object handles.
//
//
//==============================================================================

#include "m4java_dll.hpp"
#include "m4types.hpp"
#include "m4stl.hpp"

class M4JavaHandleData ;
class M4JavaContext ;



#ifndef __M4JHANDLEPROVIDER_HPP__
#define __M4JHANDLEPROVIDER_HPP__



m4int8_t	M4HandleCompare( m4double_t ai_dHandle1, m4double_t ai_dHandle2 ) ;

struct m4j_m4double_t_less
{
	m4bool_t operator()( m4double_t ai_pccDouble1, m4double_t ai_pccDouble2 ) const
	{
		return( M4HandleCompare( ai_pccDouble1, ai_pccDouble2 ) < 0 ) ;
	}
};

typedef set< m4double_t, m4j_m4double_t_less >						SetHandleData ;
typedef SetHandleData::iterator										itSetHandleData ;
typedef SetHandleData::const_iterator								const_itSetHandleData ;

typedef map<m4double_t, M4JavaHandleData*, m4j_m4double_t_less>		MapHandleData ;
typedef MapHandleData::iterator										itMapHandleData ;
typedef MapHandleData::const_iterator								const_itMapHandleData ;



//=================================================================================
//
// M4JavaHandleProvider
//
// Clase que define un proveedor de handles de objetos java
//
//=================================================================================

class M4_DECL_M4JAVA	M4JavaHandleProvider
{

protected:

	m4bool_t		m_bRecycling ;
	m4uint16_t		m_iIdentifier ;
	m4double_t		m_dFirstHandle ;
	m4double_t		m_dHandle ;
	MapHandleData	m_oHandleData ;

public:

// Funciones de inicialización ================================================
	
        M4JavaHandleProvider( m4uint16_t ai_iIdentifier ) ;
        ~M4JavaHandleProvider( void ) ;

// Funciones de lectura =======================================================

	m4pcchar_t			GetJavaClassName( m4double_t ai_dHandle ) const ;
	M4JavaHandleData*	GetHandleData( m4double_t ai_dHandle ) const ;

protected:

// Funciones de ciclo de vida =================================================

	m4double_t	GetHandle( void ) const
	{
		return( m_dHandle ) ;
	}

	m4return_t	NewObject( m4pcchar_t ai_pccClassName, M4JavaContext* ai_poContext ) ;
	m4return_t	DeleteObject( m4double_t ai_dHandle ) ;

// Funciones internas =========================================================

	M4JavaHandleData*	_GetHandleData( m4double_t ai_dHandle, const_itMapHandleData& ai_roIterator ) const ;
	void				_NextHandle( void ) ;

friend class M4JavaContext ;

} ;

#endif

