
//==============================================================================
//
// (c) Copyright  1991-2015 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4java.dll
// File:                m4jcontext.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                05-01-2015
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines a class for java execution contexts.
//
//
//==============================================================================

#include "m4java_dll.hpp"
#include "m4types.hpp"
#include "m4jobject.hpp"
#include "m4pprocess.hpp"
#include "m4jhandleprovider.hpp"

class ClVMBaseEnv;
class M4JavaConfiguration;
class IExecutor;




#ifndef __M4JCONTEXT_HPP__
#define __M4JCONTEXT_HPP__


//=================================================================================
//
// M4JavaContext
//
// Clase que define un contexto de ejecución de java identificado por un classpath
//
//=================================================================================

class M4_DECL_M4JAVA	M4JavaContext
{

protected:

	m4uint32_t				m_iTimeOut ;
	m4bool_t				m_bTrace ;
	m4bool_t				m_bFirstTime ;
	m4bool_t				m_bIsDisposed ;
	m4uint32_t				m_iReferences ;

	m4pchar_t				m_pcIdentifier ;
	M4PipedProcess			m_oProcess ;
	SetHandleData			m_oHandleData ;

	m4bool_t				m_bOwner ;
	M4JavaHandleProvider	*m_poHandleProvider ;
	IExecutor				*m_poExecutor ;

public:

// Funciones de inicialización ================================================
	
        M4JavaContext( M4JavaHandleProvider *ai_poHandleProvider = NULL, IExecutor *ai_poExecutor = NULL ) ;
        M4JavaContext( ClVMBaseEnv *ai_poEnvironment, m4pcchar_t ai_pccSource, m4pcchar_t ai_pccClassPath, m4pcchar_t ai_pccIdentifier, M4JavaHandleProvider *ai_poHandleProvider = NULL, IExecutor *ai_poExecutor = NULL ) ;
        ~M4JavaContext( void ) ;

	m4return_t	Open( ClVMBaseEnv *ai_poEnvironment, m4pcchar_t ai_pccSource, m4pcchar_t ai_pccClassPath, m4pcchar_t ai_pccIdentifier ) ;
	m4return_t	Close( m4bool_t ai_bDisposing = M4_FALSE ) ;

// Funciones de estado ========================================================

	m4bool_t	IsReady( void ) const
	{
		return( m_oProcess.IsReady() ) ;
	}

	m4bool_t	IsEmpty( void ) const
	{
		return( m_oHandleData.size() == 0 ) ;
	}

// Funciones de lectura =======================================================

	m4pcchar_t	GetIdentifier( void ) const
	{
		return( m_pcIdentifier ) ;
	}

// Funciones de ciclo de vida =================================================

	m4bool_t	IsReferenced( void )
	{
		return( m_iReferences > 0 ) ;
	}

	m4uint32_t	AddReference( void )
	{
		return( m_iReferences++ ) ;
	}

	m4uint32_t	RemoveReference( void )
	{
		return( m_iReferences-- ) ;
	}

// Funciones de ejecución =====================================================

	m4return_t	CreateObject ( m4pcchar_t ai_pccClassName,                          M4JavaObject ai_avArguments[], m4uint16_t ai_iLength, m4double_t &ao_rdHandle,   m4bool_t ai_bDumpError, m4pcchar_t ai_pccSource ) ;
	m4return_t	DestroyObject( m4double_t ai_dHandle,                                                                                                                m4bool_t ai_bDumpError, m4pcchar_t ai_pccSource ) ;
	m4return_t	NewMethod    ( m4pcchar_t ai_pccClassName, m4pcchar_t ai_pccMethod, M4JavaObject ai_avTypes[],     m4uint16_t ai_iLength, m4uint32_t &ao_riResult,   m4bool_t ai_bDumpError, m4pcchar_t ai_pccSource ) ;
	m4return_t	DeleteMethod ( m4uint32_t ai_iMethod,                                                                                                                m4bool_t ai_bDumpError, m4pcchar_t ai_pccSource ) ;
	m4return_t	InvokeMethod ( m4double_t ai_dHandle,      m4pcchar_t ai_pccMethod, M4JavaObject ai_avArguments[], m4uint16_t ai_iLength, M4JavaObject &ao_rvResult, m4bool_t ai_bDumpError, m4pcchar_t ai_pccSource ) ;
	m4return_t	InvokeMethod ( m4double_t ai_dHandle,      m4uint32_t ai_iMethod,   M4JavaObject ai_avArguments[], m4uint16_t ai_iLength, M4JavaObject &ao_rvResult, m4bool_t ai_bDumpError, m4pcchar_t ai_pccSource ) ;
	m4return_t	InvokeMethod ( m4pcchar_t ai_pccClassName, m4pcchar_t ai_pccMethod, M4JavaObject ai_avArguments[], m4uint16_t ai_iLength, M4JavaObject &ao_rvResult, m4bool_t ai_bDumpError, m4pcchar_t ai_pccSource ) ;
	m4return_t	InvokeMethod (                             m4uint32_t ai_iMethod,   M4JavaObject ai_avArguments[], m4uint16_t ai_iLength, M4JavaObject &ao_rvResult, m4bool_t ai_bDumpError, m4pcchar_t ai_pccSource ) ;

protected:

// Funciones internas =========================================================

	m4return_t	_Init( M4JavaHandleProvider *ai_poHandleProvider, IExecutor *ai_poExecutor ) ;

	m4return_t	_Serialize( m4uint8_t ai_iOperation, M4JavaObject ai_apvHeaders[], m4uint16_t ai_iHeaders, M4JavaObject ai_avArguments[], m4uint16_t ai_iArguments, m4pcchar_t ai_pccSource ) ;
	m4return_t	_DeSerialize( M4JavaObject &ao_rvResult, m4bool_t ai_bDumpError ) ;
	m4return_t	_Invoke( m4uint8_t ai_iOperation, M4JavaObject ai_apvHeaders[], m4uint16_t ai_iHeaders, M4JavaObject ai_avArguments[], m4uint16_t ai_iArguments, M4JavaObject &ao_rvResult, m4bool_t ai_bDumpError, m4pcchar_t ai_pccSource ) ;


// Funciones de ciclo de vida =================================================

	m4bool_t	IsDisposed( void ) const
	{
		return( m_bIsDisposed ) ;
	}

	void	Dispose( void )
	{
		m_bIsDisposed = M4_TRUE ;
	}

friend class M4JavaManager;
} ;

#endif

