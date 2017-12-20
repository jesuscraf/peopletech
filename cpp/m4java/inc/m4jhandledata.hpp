
//==============================================================================
//
// (c) Copyright  1991-2015 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4java.dll
// File:                m4jhandledata.hpp   
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
//    This module defines a class for java object handle data.
//
//
//==============================================================================

#include "m4java_dll.hpp"
#include "m4types.hpp"

class M4JavaContext ;



#ifndef __M4JHANDLEDATA_HPP__
#define __M4JHANDLEDATA_HPP__



//=================================================================================
//
// M4JavaHandleData
//
// Clase que define los datos asociados a un handle de objeto java
//
//=================================================================================

class	M4_DECL_M4JAVA	M4JavaHandleData
{

protected:

	m4pchar_t		m_pcClassName ;
	M4JavaContext*	m_poContext ;

public:

// Funciones de inicialización ================================================

	M4JavaHandleData( m4pcchar_t ai_pccClassName, M4JavaContext *ai_poContext ) ;
	~M4JavaHandleData( void ) ;

// Funciones de lectura =======================================================

	m4pcchar_t	GetJavaClassName( void ) const
	{
		return( m_pcClassName ) ;
	}

	M4JavaContext*	GetContext( void ) const
	{
		return( m_poContext ) ;
	}
};

#endif

