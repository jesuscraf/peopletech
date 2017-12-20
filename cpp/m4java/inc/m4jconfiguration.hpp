
//==============================================================================
//
// (c) Copyright  1991-2015 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4java.dll
// File:                m4jconfiguration.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                15-01-2015
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines a class for java configuration values.
//
//
//==============================================================================

#include "m4java_dll.hpp"
#include "m4stl.hpp"
#include "m4types.hpp"
#include "syncro.hpp"


#ifndef __M4JCONFIGURATION_HPP__
#define __M4JCONFIGURATION_HPP__

class ClVMBaseEnv;


//=================================================================================
//
// M4JavaConfiguration
//
// Clase que define la configuración java
//
//=================================================================================

class M4_DECL_M4JAVA	M4JavaConfiguration
{

protected:

	m4pchar_t	m_pcJarDirectory ;
	m4pchar_t	m_pcHome ;
	m4pchar_t	m_pcExecutable ;
	m4pchar_t	m_pcClassPath ;
	m4pchar_t	m_pcClassPathFile ;
	m4pchar_t*	m_ppcArguments ;
	m4uint32_t	m_iArgumentsLength ;
	m4bool_t	m_bTrace ;
	m4uint32_t	m_iTimeOut ;

	ClMutex		m_oClassPathMutex ;

public:

// Parte estática =============================================================

	static M4JavaConfiguration*	m_poInstance ;
	static ClMutex				m_oInstanceMutex ;

	static M4JavaConfiguration*	Instance( ClVMBaseEnv *ai_poEnvironment ) ;
	static void	Release( void ) ;

protected:

// Funciones de inicialización ================================================
	
        M4JavaConfiguration( void ) ;
        M4JavaConfiguration( ClVMBaseEnv *ai_poEnvironment ) ;
        ~M4JavaConfiguration( void ) ;

	m4return_t	Open( ClVMBaseEnv *ai_poEnvironment ) ;
	m4return_t	Close( void ) ;

public:

// Funciones de classpath =====================================================

	m4pcchar_t	GetClassPath( m4pcchar_t& ao_rpccClassPathFile ) ;
	m4return_t	SetClassPath( m4pcchar_t ai_pccClassPath, m4pcchar_t ai_pccFileClassPath ) ;
	m4return_t	NormalizePath( m4pcchar_t ai_pccPath, m4bool_t ai_bDecompress, m4bool_t& ao_rbNeedToDecompress, string& ao_rsJarPath ) ;
	m4return_t	NormalizeClassPath( m4pcchar_t ai_pccClassPath, m4bool_t ai_bDecompress, m4bool_t& ao_rbNeedToDecompress, string& ao_rsClassPath, string& ao_rsFileClassPath ) ;

// Funciones de lectura =======================================================

	m4pchar_t	GetJarDirectory( void ) const
	{
		return( m_pcJarDirectory ) ;
	}

	m4pchar_t	GetExecutable( void ) const
	{
		return( m_pcExecutable ) ;
	}

	m4pchar_t*	GetArguments( void ) const
	{
		return( m_ppcArguments ) ;
	}

	m4uint32_t	GetArgumentsLength( void ) const
	{
		return( m_iArgumentsLength ) ;
	}

	m4bool_t	GetTrace( void ) const
	{
		return( m_bTrace ) ;
	}

	m4uint32_t	GetTimeOut( void ) const
	{
		return( m_iTimeOut ) ;
	}

// Funciones de escritura =====================================================

	void	SetTrace( m4bool_t ai_bTrace )
	{
		m_bTrace = ai_bTrace ;
	}

protected:

// Funciones internas =========================================================

	void		_Init( void ) ;
	m4pcchar_t	_GetHome( void ) ;
	m4return_t	_ExpandPath( m4pcchar_t ai_pccPath, string& ao_rsPath ) ;
	m4return_t	_AddPath( m4pcchar_t ai_pccPath, m4bool_t ai_bDecompress, m4bool_t ai_bNormalize, m4bool_t& ao_rbNeedToDecompress, string& ao_rsClassPath, string& ao_rsFileClassPath ) ;
	m4uint32_t	_CalculateCrc( m4pcchar_t ai_pccPath ) ;
} ;

#endif

