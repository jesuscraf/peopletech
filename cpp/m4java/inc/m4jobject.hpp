
//==============================================================================
//
// (c) Copyright  1991-2015 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4java.dll
// File:                m4jobject.hpp   
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
//    This module defines a class for java object.
//
//
//==============================================================================

#include "m4java_dll.hpp"
#include "m4types.hpp"
#include "m4var.hpp"
#include <string.h>

class M4PipedProcess;



#ifndef __M4JOBJECT_HPP__
#define __M4JOBJECT_HPP__



//=================================================================================
//
// M4JavaObject
//
// Clase que define un objeto java
//
//=================================================================================

class	M4_DECL_M4JAVA	M4JavaObject
{

public:

	enum	eJavaType_t { Null = 0, Object, Double, Date, Calendar, Float, String, Long, Integer, Short, Char, Byte, Boolean } ;

protected:

	union
	{
		m4double_t	Object ;
		m4double_t	Double ;
		m4date_t	Date ;
		m4date_t	Calendar ;
		m4float_t	Float ;
		m4pchar_t	String ;
		m4int64_t	Long ;
		m4int32_t	Integer ;
		m4int16_t	Short ;
		m4uint16_t	Char ;
		m4int8_t	Byte ;
		m4bool_t	Boolean ; }	m_vValue ;
	eJavaType_t					m_eType ;
	m4bool_t					m_bOwner ;

public:

// Funciones de inicialización ================================================

		M4JavaObject( void )
		{
			_Init() ;
		}

		~M4JavaObject( void )
		{
			_Release() ;
		}

// Funciones de lectura =======================================================

	eJavaType_t	GetType( void ) const
	{
		return( m_eType ) ;
	}

	m4bool_t	GetNull    ( void ) const ;
	m4double_t	GetObject  ( void ) const ;
	m4double_t	GetDouble  ( void ) const ;
	m4date_t	GetDate    ( void ) const ;
	m4date_t	GetCalendar( void ) const ;
	m4float_t	GetFloat   ( void ) const ;
	m4pcchar_t	GetString  ( void ) const ;
	m4int64_t	GetLong    ( void ) const ;
	m4int32_t	GetInteger ( void ) const ;
	m4int16_t	GetShort   ( void ) const ;
	m4uint16_t	GetChar    ( void ) const ;
	m4int8_t	GetByte    ( void ) const ;
	m4bool_t	GetBoolean ( void ) const ;

	m4return_t	GetObject  ( m4double_t    &ao_rdObject   ) const ;
	m4return_t	GetDouble  ( m4double_t    &ao_rdDouble   ) const ;
	m4return_t	GetDate    ( m4date_t      &ao_rdDate     ) const ;
	m4return_t	GetCalendar( m4date_t      &ao_rdCalendar ) const ;
	m4return_t	GetFloat   ( m4float_t     &ao_rdFloat    ) const ;
	m4return_t	GetString  ( m4pcchar_t    &ao_rpccString ) const ;
	m4return_t	GetLong    ( m4int64_t     &ao_riLong     ) const ;
	m4return_t	GetInteger ( m4int32_t     &ao_riInteger  ) const ;
	m4return_t	GetShort   ( m4int16_t     &ao_riShort    ) const ;
	m4return_t	GetChar    ( m4uint16_t    &ao_riChar     ) const ;
	m4return_t	GetByte    ( m4int8_t      &ao_riByte     ) const ;
	m4return_t	GetBoolean ( m4bool_t      &ao_riBoolean  ) const ;

	void		GetVariant ( m4VariantType &ao_rvValue, m4bool_t ai_bCopy, m4bool_t ai_bOwner = M4_FALSE ) ;

// Funciones de escritura =====================================================

	void SetOwner( m4bool_t ai_bOwner )
	{
		m_bOwner = ai_bOwner ;
	}

	void	SetNull    ( void                       ) ;
	void	SetObject  ( m4double_t    ai_dObject   ) ;
	void	SetDouble  ( m4double_t    ai_dDouble   ) ;
	void	SetDate    ( m4date_t      ai_dDate     ) ;
	void	SetCalendar( m4date_t      ai_dCalendar ) ;
	void	SetFloat   ( m4float_t     ai_dFloat    ) ;
	void	SetString  ( m4pcchar_t    ai_pccString, m4bool_t ai_bCopy, m4int32_t ai_iLength = -1 ) ;
	void	SetLong    ( m4int64_t     ai_iLong     ) ;
	void	SetInteger ( m4int32_t     ai_iInteger  ) ;
	void	SetShort   ( m4int16_t     ai_iShort    ) ;
	void	SetChar    ( m4uint16_t    ai_iChar     ) ;
	void	SetByte    ( m4int8_t      ai_iByte     ) ;
	void	SetBoolean ( m4bool_t      ai_iBoolean  ) ;

	void	SetVariant ( m4VariantType &ai_rvValue, m4bool_t ai_bCopy, m4bool_t ai_bOwner = M4_FALSE ) ;

protected:

// Funciones internas =========================================================

	void	_Init( void )
	{
		memset( this, 0, sizeof( M4JavaObject ) ) ;
	}

	void	_Release( void ) ;

// Funciones de serialización =================================================

	m4return_t	Write( M4PipedProcess &ai_roPipe ) ;
	m4return_t	Read ( M4PipedProcess &ai_roPipe, m4bool_t ai_bCopy ) ;

friend class M4JavaContext;

};


#endif

