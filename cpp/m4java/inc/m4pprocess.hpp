
//==============================================================================
//
// (c) Copyright  1991-2015 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4ipcs.dll
// File:                m4pprocess.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                23-12-2014
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines a class for child processes connected by pipes.
//
//
//==============================================================================

#include "m4java_dll.hpp"
#include "m4types.hpp"
#include <stdio.h>

class IExecutor;



#ifndef __M4PPROCESS_HPP__
#define __M4PPROCESS_HPP__


//=================================================================================
//
// M4PipedProcess
//
// Clase que define un proceso hijo enlazado por pipes
//
//=================================================================================

class	M4PipedProcess
{

protected:

	m4process_t	m_iProcess ;
	m4pchar_t	m_pcError ;

	int			m_aiParentToChild[ 2 ] ;
	int			m_aiChildToParent[ 2 ] ;

	// Auxiliares
	m4uint32_t	m_iSize ;
	m4pchar_t	m_pcBuffer ;

public:

// Funciones de inicialización ================================================
	
        M4PipedProcess( void ) ;
        M4PipedProcess( m4pcchar_t ai_apccArguments[], m4uint32_t ai_iLength ) ;
        ~M4PipedProcess( void ) ;

	m4return_t	Open( m4pcchar_t ai_apccArguments[], m4uint32_t ai_iLength ) ;
	m4return_t	Close( void ) ;

// Funciones de error =========================================================

	m4bool_t	IsReady( void ) const
	{
		return( m_iProcess > 0 ) ;
	}

	m4pcchar_t	GetError( void ) const
	{
		return( m_pcError ) ;
	}

// Funciones sencillas de lectura y escritura =================================

	m4return_t	Write( m4pvoid_t ai_pvMemory, m4uint32_t ai_iLength ) ;
	m4return_t	Read( m4pvoid_t ao_pvMemory, m4uint32_t ai_iLength ) ;

// Funciones avanzadas de lectura y escritura =================================

	m4return_t	WriteString( m4pcchar_t ai_pccString, m4int32_t ai_iLength = -1 ) ;
	m4return_t	ReadString( m4pchar_t &ao_rpcString, m4int32_t *ao_piLength = NULL, m4bool_t ai_bCopy = M4_FALSE ) ;

	m4return_t	ReadByte( m4int8_t& ao_rbByte, m4uint32_t ai_iTimeOut, IExecutor* ai_poExecutor, m4return_t& ao_riCanceled ) ;

protected:

// Funciones internas =========================================================

	void		_Init( void ) ;
	void		_SetError( int ai_iError = -1 ) ;
	void		_FreeBuffer( void ) ;
	m4pchar_t	_New( m4uint32_t ai_iSize ) ;

} ;

#endif

