
//==============================================================================
//
// (c) Copyright  1991-2015 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4java.dll
// File:                m4jobject.cpp   
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
//    This module implements a class for java object.
//
//
//==============================================================================

#include "m4jobject.hpp"
#include "m4pprocess.hpp"
#include "m4javares.hpp"
#include "m4objlog.hpp"
#include "cldates.hpp"
#include "m4unicode.hpp"

m4pchar_t	M4CloneString( m4pcchar_t ai_pccSource, m4int32_t ai_iLength = -1 ) ;



#define	M4JAVA_SECONDS_DAY						86400
#define M4JAVA_FIRST_UNIX_DATE					719152


//=================================================================================
//
// M4JavaObject
//
// Clase que define un objeto java
//
//=================================================================================

// Funciones de lectura =======================================================

m4bool_t	M4JavaObject::GetNull( void ) const
{
	return( m_eType == Null ) ;
}


m4double_t	M4JavaObject::GetObject( void ) const
{
	if( m_eType == Object )
	{
		return( m_vValue.Object ) ;
	}
	return( 0 ) ;
}


m4double_t	M4JavaObject::GetDouble( void ) const
{
	if( m_eType == Double )
	{
		return( m_vValue.Double ) ;
	}
	return( 0 ) ;
}


m4date_t	M4JavaObject::GetDate( void ) const
{
	if( m_eType == Date )
	{
		return( m_vValue.Date ) ;
	}
	return( 0 ) ;
}


m4date_t	M4JavaObject::GetCalendar( void ) const
{
	if( m_eType == Calendar )
	{
		return( m_vValue.Calendar ) ;
	}
	return( 0 ) ;
}


m4float_t	M4JavaObject::GetFloat( void ) const
{
	if( m_eType == Float )
	{
		return( m_vValue.Float ) ;
	}
	return( 0 ) ;
}


m4pcchar_t	M4JavaObject::GetString( void ) const
{
	if( m_eType == String )
	{
		return( m_vValue.String ) ;
	}
	return( NULL ) ;
}


m4int64_t	M4JavaObject::GetLong( void ) const
{
	if( m_eType == Long )
	{
		return( m_vValue.Long ) ;
	}
	return( 0 ) ;
}


m4int32_t	M4JavaObject::GetInteger( void ) const
{
	if( m_eType == Integer )
	{
		return( m_vValue.Integer ) ;
	}
	return( 0 ) ;
}


m4int16_t	M4JavaObject::GetShort( void ) const
{
	if( m_eType == Short )
	{
		return( m_vValue.Short ) ;
	}
	return( 0 ) ;
}


m4uint16_t	M4JavaObject::GetChar( void ) const
{
	if( m_eType == Char )
	{
		return( m_vValue.Char ) ;
	}
	return( 0 ) ;
}


m4int8_t	M4JavaObject::GetByte( void ) const
{
	if( m_eType == Byte )
	{
		return( m_vValue.Byte ) ;
	}
	return( 0 ) ;
}


m4bool_t	M4JavaObject::GetBoolean( void ) const
{
	if( m_eType == Boolean )
	{
		return( m_vValue.Boolean ) ;
	}
	return( M4_FALSE ) ;
}


m4return_t	M4JavaObject::GetObject( m4double_t &ao_rdObject ) const
{
	ao_rdObject = 0 ;

	if( m_eType == Object )
	{
		ao_rdObject = m_vValue.Object ;
		return( M4_SUCCESS ) ;
	}
	return( M4_ERROR ) ;
}


m4return_t	M4JavaObject::GetDouble( m4double_t &ao_rdDouble ) const
{
	ao_rdDouble = 0 ;

	if( m_eType == Double )
	{
		ao_rdDouble = m_vValue.Double ;
		return( M4_SUCCESS ) ;
	}
	return( M4_ERROR ) ;
}


m4return_t	M4JavaObject::GetDate( m4date_t &ao_rdDate ) const
{
	ao_rdDate = 0 ;

	if( m_eType == Date )
	{
		ao_rdDate = m_vValue.Date ;
		return( M4_SUCCESS ) ;
	}
	return( M4_ERROR ) ;
}


m4return_t	M4JavaObject::GetCalendar( m4date_t &ao_rdCalendar ) const
{
	ao_rdCalendar = 0 ;

	if( m_eType == Calendar )
	{
		ao_rdCalendar = m_vValue.Calendar ;
		return( M4_SUCCESS ) ;
	}
	return( M4_ERROR ) ;
}


m4return_t	M4JavaObject::GetFloat( m4float_t &ao_rdFloat ) const
{
	ao_rdFloat = 0 ;

	if( m_eType == Float )
	{
		ao_rdFloat = m_vValue.Float ;
		return( M4_SUCCESS ) ;
	}
	return( M4_ERROR ) ;
}


m4return_t	M4JavaObject::GetString( m4pcchar_t &ao_rpccString ) const
{
	ao_rpccString = NULL ;

	if( m_eType == String )
	{
		ao_rpccString = m_vValue.String ;
		return( M4_SUCCESS ) ;
	}
	return( M4_ERROR ) ;
}


m4return_t	M4JavaObject::GetLong( m4int64_t &ao_riLong ) const
{
	ao_riLong = 0 ;

	if( m_eType == Long )
	{
		ao_riLong = m_vValue.Long ;
		return( M4_SUCCESS ) ;
	}
	return( M4_ERROR ) ;
}


m4return_t	M4JavaObject::GetInteger( m4int32_t &ao_riInteger ) const
{
	ao_riInteger = 0 ;

	if( m_eType == Integer )
	{
		ao_riInteger = m_vValue.Integer ;
		return( M4_SUCCESS ) ;
	}
	return( M4_ERROR ) ;
}


m4return_t	M4JavaObject::GetShort( m4int16_t &ao_riShort ) const
{
	ao_riShort = 0 ;

	if( m_eType == Short )
	{
		ao_riShort = m_vValue.Short ;
		return( M4_SUCCESS ) ;
	}
	return( M4_ERROR ) ;
}


m4return_t	M4JavaObject::GetChar( m4uint16_t &ao_riChar ) const
{
	ao_riChar = 0 ;

	if( m_eType == Char )
	{
		ao_riChar = m_vValue.Char ;
		return( M4_SUCCESS ) ;
	}
	return( M4_ERROR ) ;
}


m4return_t	M4JavaObject::GetByte( m4int8_t &ao_riByte ) const
{
	ao_riByte = 0 ;

	if( m_eType == Byte )
	{
		ao_riByte = m_vValue.Byte ;
		return( M4_SUCCESS ) ;
	}
	return( M4_ERROR ) ;
}


m4return_t	M4JavaObject::GetBoolean( m4bool_t &ao_riBoolean ) const
{
	ao_riBoolean = M4_FALSE ;

	if( m_eType == Boolean )
	{
		ao_riBoolean = m_vValue.Boolean ;
		return( M4_SUCCESS ) ;
	}
	return( M4_ERROR ) ;
}


void	M4JavaObject::GetVariant( m4VariantType &ao_rvValue, m4bool_t ai_bCopy, m4bool_t ai_bOwner )
{

	int			iLength = 0 ;
	M4XMLCh		aiBuffer[ 2 ] = { 0, 0 } ;
		

	switch( m_eType )
	{
		case Object:
			ao_rvValue.SetDouble( m_vValue.Object ) ;
			break ;

		case Double:
			ao_rvValue.SetDouble( m_vValue.Double ) ;
			break ;

		case Date:
			ao_rvValue.SetDate( m_vValue.Date ) ;
			break ;

		case Calendar:
			ao_rvValue.SetDate( m_vValue.Calendar ) ;
			break ;

		case Float:
			ao_rvValue.SetDouble( m_vValue.Float ) ;
			break ;

		case String:
			if( ai_bCopy == M4_TRUE )
			{
				ao_rvValue.SetString( m_vValue.String ) ;
			}
			else
			{
				ao_rvValue.SetStringRef( m_vValue.String ) ;

				if( ai_bOwner == M4_TRUE && m_bOwner == M4_TRUE )
				{
					m_bOwner = M4_FALSE ;
					ao_rvValue.SetOwner( M4_TRUE ) ;
				}
			}
			break ;

		case Long:
			ao_rvValue.SetDouble( m_vValue.Long ) ;
			break ;

		case Integer:
			ao_rvValue.SetDouble( m_vValue.Integer ) ;
			break ;

		case Short:
			ao_rvValue.SetDouble( m_vValue.Short ) ;
			break ;

		case Char:
			aiBuffer[ 0 ] = m_vValue.Char ;
			iLength = 1 ;
			ao_rvValue.SetStringRef( M4Utf16ToCpp( aiBuffer, iLength ) ) ;
			ao_rvValue.SetOwner( M4_TRUE ) ;
			break ;

		case Byte:
			ao_rvValue.SetDouble( m_vValue.Byte ) ;
			break ;

		case Boolean:
			ao_rvValue.SetDouble( m_vValue.Boolean ) ;
			break ;

		default:
			ao_rvValue.SetNull() ;
			break ;
	}
}


// Funciones de escritura =====================================================

void	M4JavaObject::SetNull( void )
{
	_Release() ;
	m_eType = Null ;
}


void	M4JavaObject::SetObject( m4double_t ai_dObject )
{
	_Release() ;
	m_eType = Object ;
	m_vValue.Object = ai_dObject ;
}


void	M4JavaObject::SetDouble( m4double_t ai_dDouble )
{
	_Release() ;
	m_eType = Double ;
	m_vValue.Double = ai_dDouble ;
}


void	M4JavaObject::SetDate( m4date_t ai_dDate )
{
	_Release() ;
	m_eType = Date ;
	m_vValue.Date = ai_dDate ;
}


void	M4JavaObject::SetCalendar( m4date_t ai_dCalendar )
{
	_Release() ;
	m_eType = Calendar ;
	m_vValue.Calendar = ai_dCalendar ;
}


void	M4JavaObject::SetFloat( m4float_t ai_dFloat )
{
	_Release() ;
	m_eType = Float ;
	m_vValue.Float = ai_dFloat ;
}


void	M4JavaObject::SetString( m4pcchar_t ai_pccString, m4bool_t ai_bCopy, m4int32_t ai_iLength )
{
	_Release() ;

	if( ai_pccString == NULL )
	{
		m_eType = Null ;
	}
	else
	{
		m_bOwner = ai_bCopy ;
		m_eType = String ;

		if( ai_bCopy == M4_TRUE )
		{
			m_vValue.String = M4CloneString( ai_pccString, ai_iLength ) ;
		}
		else
		{
			m_vValue.String = (m4pchar_t)ai_pccString ;
		}
	}
}


void	M4JavaObject::SetLong( m4int64_t ai_iLong )
{
	_Release() ;
	m_eType = Long ;
	m_vValue.Long = ai_iLong ;
}


void	M4JavaObject::SetInteger( m4int32_t ai_iInteger )
{
	_Release() ;
	m_eType = Integer ;
	m_vValue.Integer = ai_iInteger ;
}


void	M4JavaObject::SetShort( m4int16_t ai_iShort )
{
	_Release() ;
	m_eType = Short ;
	m_vValue.Short = ai_iShort ;
}


void	M4JavaObject::SetByte( m4int8_t ai_iByte )
{
	_Release() ;
	m_eType = Byte ;
	m_vValue.Byte = ai_iByte ;
}


void	M4JavaObject::SetBoolean( m4bool_t ai_iBoolean )
{
	_Release() ;
	m_eType = Boolean ;
	m_vValue.Boolean = ai_iBoolean ;
}


void	M4JavaObject::SetVariant( m4VariantType &ai_rvValue, m4bool_t ai_bCopy, m4bool_t ai_bOwner )
{
	switch( ai_rvValue.Type )
	{
		case M4CL_CPP_TYPE_NUMBER:
			SetDouble( ai_rvValue.Data.DoubleData ) ;
			break ;

		case M4CL_CPP_TYPE_STRING_VAR:
			if( ai_bCopy == M4_TRUE )
			{
				SetString( ai_rvValue.Data.PointerChar, M4_TRUE ) ;
			}
			else
			{
				SetString( ai_rvValue.Data.PointerChar, M4_FALSE ) ;

				if( ai_bOwner == M4_TRUE && ai_rvValue.GetOwner() == M4_TRUE )
				{
					ai_rvValue.SetOwner( M4_FALSE ) ;
					m_bOwner = M4_TRUE ;
				}
			}
			break ;

		case M4CL_CPP_TYPE_DATE:
			SetCalendar( ai_rvValue.Data.DoubleData ) ;
			break ;

		default:
			SetNull() ;
			break ;
	}
}


// Funciones internas =========================================================

void	M4JavaObject::_Release( void )
{
	if( m_bOwner == M4_TRUE && m_vValue.String != NULL )
	{
		delete( m_vValue.String ) ;
	}
	_Init() ;
}


// Funciones de serialización =================================================

m4return_t	M4JavaObject::Write( M4PipedProcess &ai_roPipe )
{

	m4return_t	iResult = M4_ERROR ;
	m4uint8_t	iType = 0 ;
	m4uint32_t	iLength = 0 ;
	m4uint32_t	iDays = 0 ;
	m4uint32_t	iSeconds = 0 ;
	m4int64_t	iDate = 0 ;
	m4date_t	dDate = 0 ;


	iType = m_eType ;
	iResult = ai_roPipe.Write( &iType, sizeof( iType ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4JAVA_SEND_DATA_ERROR, ai_roPipe.GetError() ) ;

	if( m_eType != Null )
	{
		switch( m_eType )
		{
			case Object:
				iResult = ai_roPipe.Write( &m_vValue.Object, sizeof( m_vValue.Object ) ) ;
				break ;

			case Double:
				iResult = ai_roPipe.Write( &m_vValue.Double, sizeof( m_vValue.Double ) ) ;
				break ;

			case Date:
			case Calendar:
				if( m_eType == Date )
				{
					dDate = m_vValue.Date ; 
				}
				else
				{
					dDate = m_vValue.Calendar ; 
				}
				ClDateToLongs( dDate, iDays, iSeconds ) ;
				iDate = ( m4int64_t( iDays - M4JAVA_FIRST_UNIX_DATE ) * M4JAVA_SECONDS_DAY + iSeconds ) * 1000 ;
				iResult = ai_roPipe.Write( &iDate, sizeof( iDate ) ) ;
				break ;

			case Float:
				iResult = ai_roPipe.Write( &m_vValue.Float, sizeof( m_vValue.Float ) ) ;
				break ;

			case String:
				iResult = ai_roPipe.WriteString( m_vValue.String ) ;
				break ;

			case Long:
				iResult = ai_roPipe.Write( &m_vValue.Long, sizeof( m_vValue.Long ) ) ;
				break ;

			case Integer:
				iResult = ai_roPipe.Write( &m_vValue.Integer, sizeof( m_vValue.Integer ) ) ;
				break ;

			case Short:
				iResult = ai_roPipe.Write( &m_vValue.Short, sizeof( m_vValue.Short ) ) ;
				break ;

			case Char:
				iResult = ai_roPipe.Write( &m_vValue.Char, sizeof( m_vValue.Char ) ) ;
				break ;

			case Byte:
				iResult = ai_roPipe.Write( &m_vValue.Byte, sizeof( m_vValue.Byte ) ) ;
				break ;

			case Boolean:
				iResult = ai_roPipe.Write( &m_vValue.Boolean, sizeof( m_vValue.Boolean ) ) ;
				break ;

			default:
				break ;
		}
		CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4JAVA_SEND_DATA_ERROR, ai_roPipe.GetError() ) ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	M4JavaObject::Read( M4PipedProcess &ai_roPipe, m4bool_t ai_bCopy )
{

	m4return_t	iResult = M4_ERROR ;
	m4uint8_t	iType = 0 ;
	m4int32_t	iLength = 0 ;
	m4int64_t	iDate = 0 ;
	m4pchar_t	pcString = NULL ;
	m4date_t	dDate = 0 ;


	_Release() ;

	iResult = ai_roPipe.Read( &iType, sizeof( iType ) ) ;
	CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4JAVA_READ_DATA_ERROR, ai_roPipe.GetError() ) ;

	m_eType = (eJavaType_t)iType ;

	if( m_eType != Null )
	{
		switch( m_eType )
		{
			case Object:
				iResult = ai_roPipe.Read( &m_vValue.Object, sizeof( m_vValue.Object ) ) ;
				break ;

			case Double:
				iResult = ai_roPipe.Read( &m_vValue.Double, sizeof( m_vValue.Double ) ) ;
				break ;

			case Date:
			case Calendar:
				iResult = ai_roPipe.Read( &iDate, sizeof( iDate ) ) ;
				dDate = m4double_t( iDate / 1000 ) / M4JAVA_SECONDS_DAY + M4JAVA_FIRST_UNIX_DATE ;

				if( m_eType == Date )
				{
					m_vValue.Date = dDate ; 
				}
				else
				{
					m_vValue.Calendar = dDate ; 
				}
				break ;

			case Float:
				iResult = ai_roPipe.Read( &m_vValue.Float, sizeof( m_vValue.Float ) ) ;
				break ;

			case String:

				iResult = ai_roPipe.ReadString( pcString, &iLength, M4_FALSE ) ;
				SetString( pcString, ai_bCopy, iLength ) ;
				break ;

			case Long:
				iResult = ai_roPipe.Read( &m_vValue.Long, sizeof( m_vValue.Long ) ) ;
				break ;

			case Integer:
				iResult = ai_roPipe.Read( &m_vValue.Integer, sizeof( m_vValue.Integer ) ) ;
				break ;

			case Short:
				iResult = ai_roPipe.Read( &m_vValue.Short, sizeof( m_vValue.Short ) ) ;
				break ;

			case Char:
				iResult = ai_roPipe.Read( &m_vValue.Char, sizeof( m_vValue.Char ) ) ;
				break ;

			case Byte:
				iResult = ai_roPipe.Read( &m_vValue.Byte, sizeof( m_vValue.Byte ) ) ;
				break ;

			case Boolean:
				iResult = ai_roPipe.Read( &m_vValue.Boolean, sizeof( m_vValue.Boolean ) ) ;
				break ;

			default:
				break ;
		}
		CHECK_CHLOG_ERRORF( iResult != M4_SUCCESS, M4_ERROR, M4JAVA_READ_DATA_ERROR, ai_roPipe.GetError() ) ;
	}

	return( M4_SUCCESS ) ;
}


