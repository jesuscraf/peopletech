//==============================================================================
//
// (c) Copyright  1991-2008 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4ini
// File:                m4xmlreg.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                22-01-2008
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module defines the interface to access registry information from xml files.
//
//
//==============================================================================


#ifndef __M4XMLREG_HPP__
#define __M4XMLREG_HPP__


#include "m4types.hpp"
#include "m4ini.hpp"
#include "xercesc/dom/DOMNode.hpp"
#include "xercesc/parsers/XercesDOMParser.hpp"

XERCES_CPP_NAMESPACE_USE




// ============================================================================
// Defines
// ============================================================================

#define	M4INI_MAX_REG_VALUE_LEN							1024



//=============================================================================
//
// M4XmlRegistryFile
//
// Clase que proporciona acceso a entradas de registro guardadas en un fichero xml 
//
//=============================================================================

class	M4_DECL_M4INI	M4XmlRegistryFile
{

protected:

	m4bool_t		m_bIsEmpty ;
	m4pchar_t		m_pcFile ;
	DOMNode			*m_poRoot ;
	XercesDOMParser	m_oParser ;

	m4return_t	_SetFile( m4pcchar_t ai_pccFile ) ;

public:

	static M4XmlRegistryFile*	GetRegistryFile(unsigned short ai_iHandle);
	static m4bool_t				FreeRegistryFile(unsigned short ai_iHandle);

// Funciones de inicialización ================================================
	
		M4XmlRegistryFile( void ) ;
		~M4XmlRegistryFile( void ) ;

// Funciones de acceso ========================================================

	m4bool_t	IsEmpty( void ) const
	{
		return( m_bIsEmpty ) ;
	}

	void	Release( m4pchar_t& ai_rpcMemory ) ;

	m4return_t	LoadMachine( void ) ;
	m4return_t	Load( m4pcchar_t ai_pccFile, m4bool_t ai_bIsXml ) ;
	m4return_t	GetValue( m4pcchar_t ai_pccKey, m4pcchar_t ai_pccValue, m4pchar_t &ao_rpcData ) ;
	m4return_t	SetValue( m4pcchar_t ai_pccKey, m4pcchar_t ai_pccValue, m4pcchar_t ai_pccData ) ;
	m4return_t	GetValueNames( m4pcchar_t ai_pccKey, list<m4pchar_t> &ao_rlValues ) ;
	m4return_t	GetSubKeyNames( m4pcchar_t ai_pccKey, list<m4pchar_t> &ao_rlSubKeys ) ;
	m4return_t	DeleteValue( m4pcchar_t ai_pccKey, m4pcchar_t ai_pccValue ) ;
	m4return_t	DeleteKey( m4pcchar_t ai_pccKey ) ;
	m4return_t	Merge( M4XmlRegistryFile& ai_roSource ) ;
	m4return_t	Save( m4pcchar_t ai_pccFile ) const ;

} ;



//=============================================================================
//
// M4XmlRegistry
//
// Clase que proporciona el recubrimiento del registro desde ficheros xml 
//
//=============================================================================

class	M4_DECL_M4INI M4XmlRegistry
{

protected:

	M4XmlRegistryFile		m_MachineRegistryFile ;
	M4XmlRegistryFile		m_UserRegistryFile ;

	m4bool_t				m_bMachineInit ;
	m4bool_t				m_bUserInit ;

	static	M4XmlRegistry	*ms_poInstance ;

public:

// Funciones de inicialización ================================================
	
	static M4XmlRegistry* Instance( void ) ;
	static void FreeInstance( void ) ;

		M4XmlRegistry( void ) ;
		~M4XmlRegistry( void ) ;


// Funciones de acceso ========================================================

	m4return_t	GetValue( m4pcchar_t ai_pccKey, m4pcchar_t ai_pccValue, m4pchar_t &ao_rpcData ) ;
	m4return_t	SetValue( m4pcchar_t ai_pccKey, m4pcchar_t ai_pccValue, m4pcchar_t ai_pccData ) ;
	m4return_t	GetValueNames( m4pcchar_t ai_pccKey, list<m4pchar_t> &ao_rlValues ) ;
	m4return_t	GetSubKeyNames( m4pcchar_t ai_pccKey, list<m4pchar_t> &ao_rlSubKeys ) ;
	m4return_t	DeleteValue( m4pcchar_t ai_pccKey, m4pcchar_t ai_pccValue ) ;
	m4return_t	DeleteKey( m4pcchar_t ai_pccKey ) ;
	m4return_t	Save( m4pcchar_t ai_pccFile ) const ;

} ;


//=================================================================================
// Funciones en formato C
//=================================================================================

#ifdef _WINDOWS

//=================================================================================
// Funciones para el manejo del registro
//=================================================================================

// Private Declare Function M4UnLoadRegistry Lib "m4inidll" () As Byte

extern "C" M4_DECL_M4INI BYTE WINAPI M4UnLoadRegistry( void ) ;


//=================================================================================
// Funciones para las herramientas en VB
//=================================================================================

// Private Declare Function M4LoadMachineRegistry Lib "m4inidll" ( ByVal ai_iHandle as Integer ) As Byte
// Private Declare Function M4LoadRegistry Lib "m4inidll" ( ByVal ai_iHandle as Integer, ByVal ai_pccFile As String, ByVal ai_bIsXml As Boolean ) As Byte
// Private Declare Function M4GetRegistryValue Lib "m4inidll" ( ByVal ai_iHandle as Integer, ByVal ai_pccKey As String, ByVal ai_pccValue As String, ByRef ao_pcData As String ) As Byte
// Private Declare Function M4SetRegistryValue Lib "m4inidll" ( ByVal ai_iHandle as Integer, ByVal ai_pccKey As String, ByVal ai_pccValue As String, ByVal ai_pccData As String ) As Byte
// Private Declare Function M4GetRegistryValueNames Lib "m4inidll" ( ByVal ai_iHandle as Integer, ByVal ai_pccKey As String, ByRef ao_psaValues() As String ) As Byte
// Private Declare Function M4GetRegistrySubKeyNames Lib "m4inidll"( ByVal ai_iHandle as Integer, ByVal ai_pccKey As String, ByRef ao_psaKeys() As String ) As Byte
// Private Declare Function M4DeleteRegistryValue Lib "m4inidll" ( ByVal ai_iHandle as Integer, ByVal ai_pccKey As String, ByVal ai_pccValue As String ) As Byte
// Private Declare Function M4DeleteRegistryKey Lib "m4inidll" ( ByVal ai_iHandle as Integer, ByVal ai_pccKey As String ) As Byte
// Private Declare Function M4MergeRegistry Lib "m4inidll" ( ByVal ai_iSourceHandle as Integer, ByVal ai_iDestinationHandle as Integer ) As Byte
// Private Declare Function M4SaveRegistry Lib "m4inidll" ( ByVal ai_iHandle as Integer, ByVal ai_pccFile As String ) As Byte

extern "C" M4_DECL_M4INI BYTE WINAPI M4LoadMachineRegistry( unsigned short ai_iHandle ) ;
extern "C" M4_DECL_M4INI BYTE WINAPI M4LoadRegistry( unsigned short ai_iHandle, const BSTR ai_pccFile, VARIANT_BOOL ai_bIsXml ) ;
extern "C" M4_DECL_M4INI BYTE WINAPI M4GetRegistryValue( unsigned short ai_iHandle, const BSTR ai_pccKey, const BSTR ai_pccValue, BSTR *ao_pcData ) ;
extern "C" M4_DECL_M4INI BYTE WINAPI M4SetRegistryValue( unsigned short ai_iHandle, const BSTR ai_pccKey, const BSTR ai_pccValue, const BSTR ai_pccData ) ;
extern "C" M4_DECL_M4INI BYTE WINAPI M4GetRegistryValueNames( unsigned short ai_iHandle, const BSTR ai_pccKey, SAFEARRAY **ao_psaValues ) ;
extern "C" M4_DECL_M4INI BYTE WINAPI M4GetRegistrySubKeyNames( unsigned short ai_iHandle, const BSTR ai_pccKey, SAFEARRAY **ao_psaKeys ) ;
extern "C" M4_DECL_M4INI BYTE WINAPI M4DeleteRegistryValue( unsigned short ai_iHandle, const BSTR ai_pccKey, const BSTR ai_pccValue ) ;
extern "C" M4_DECL_M4INI BYTE WINAPI M4DeleteRegistryKey( unsigned short ai_iHandle, const BSTR ai_pccKey ) ;
extern "C" M4_DECL_M4INI BYTE WINAPI M4MergeRegistry( unsigned short ai_iSourceHandle, unsigned short ai_iDestinationHandle ) ;
extern "C" M4_DECL_M4INI BYTE WINAPI M4SaveRegistry( unsigned short ai_iHandle, const BSTR ai_pccFile ) ;


//=================================================================================
// Funciones para las herramientas en install script
//=================================================================================

// Private Declare Function M4WIGetRegistryValue Lib "m4inidll" ( ByVal ai_iHandle as Integer, ByVal ai_pccKey As String, ByVal ai_pccValue As String, ByRef ao_pcData As String, ByRef aio_piSize As Integer ) As Byte
// Private Declare Function M4WIGetRegistryValueCount Lib "m4inidll" ( ByVal ai_iHandle as Integer, ByVal ai_pccKey As String, ByRef aio_piSize As Integer ) As Byte
// Private Declare Function M4WIGetRegistrySubKeyCount Lib "m4inidll" ( ByVal ai_iHandle as Integer, ByVal ai_pccKey As String, ByRef aio_piSize As Integer ) As Byte
// Private Declare Function M4WIGetRegistryValueName Lib "m4inidll" ( ByVal ai_iHandle as Integer, ByVal ai_pccKey As String, ByVal ai_iPosition As Integer, ByRef ao_pcData As String, ByRef aio_piSize As Integer ) As Byte
// Private Declare Function M4WIGetRegistrySubKeyName Lib "m4inidll" ( ByVal ai_iHandle as Integer, ByVal ai_pccKey As String, ByVal ai_iPosition As Integer, ByRef ao_pcData As String, ByRef aio_piSize As Integer ) As Byte

extern "C" M4_DECL_M4INI BYTE WINAPI M4WIGetRegistryValue( unsigned short ai_iHandle, const BSTR ai_pccKey, const BSTR ai_pccValue, BSTR ao_pcData, short *ao_piCount ) ;
extern "C" M4_DECL_M4INI BYTE WINAPI M4WIGetRegistryValueCount( unsigned short ai_iHandle, const BSTR ai_pccKey, short *ao_piCount ) ;
extern "C" M4_DECL_M4INI BYTE WINAPI M4WIGetRegistrySubKeyCount( unsigned short ai_iHandle, const BSTR ai_pccKey, short *ao_piCount ) ;
extern "C" M4_DECL_M4INI BYTE WINAPI M4WIGetRegistryValueName( unsigned short ai_iHandle, const BSTR ai_pccKey, short ai_iPosition, BSTR ao_pcName, short *aio_piSize ) ;
extern "C" M4_DECL_M4INI BYTE WINAPI M4WIGetRegistrySubKeyName( unsigned short ai_iHandle, const BSTR ai_pccKey, short ai_iPosition, BSTR ao_pcName, short *aio_piSize ) ;

#endif


//=================================================================================
// Funciones para programas C (rpcspy)
//=================================================================================

extern "C" M4_DECL_M4INI int M4CGetRegistryValue( const char* ai_pccKey, const char* ai_pccValue, char* ao_pcData, size_t ai_iSize, size_t* ao_piLength ) ;


#endif


