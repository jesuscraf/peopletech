//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// File:             inputStream.cpp
// Project:          Service presentation
// Author:           Meta Software M.S. , S.A
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Implementation of the CInputStream class.
//
//
//==============================================================================

#include "asndef.h"
#include "m4unicode.hpp"
extern int	M4IsUnicode( void ) ;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInputStream::CInputStream( m4puchar_t asn, m4uint32_t length )
{
	m_cont = 0;

	m_iLength = length;
	m_stream = asn;

	if( M4IsUnicode() == 0 )
	{
		m_iStringBaseType = ASNGeneralString;
	}
	else
	{
		m_iStringBaseType = ASNUnicodeString;
	}
}

m4uchar_t CInputStream::readByte( m4uchar_t& ao_rbByte )
{
	/* Bug 0176994
	Hay que controlar que no se lea fuera del array
	*/
	if( m_cont >= m_iLength )
	{
		ao_rbByte = 0;
		return 0;
	}

	ao_rbByte = m_stream[ m_cont++ ];
	return 1;
}

m4uint32_t CInputStream::readLength( m4uint32_t& ao_riLength )
{
	m4uint32_t	bytesRead = 0;
   	m4uchar_t	byte;
	m4uchar_t	i;

	bytesRead += readByte( byte );

	if( byte > 128 )
	{
		byte -= 128;
		ao_riLength = 0;

		for( i = byte ; i > 0 ; i-- )
		{
			bytesRead += readByte( byte );
			ao_riLength += ( byte << 8 * ( i - 1 ) );
		}
	}
	else
	{
		ao_riLength = byte;
	}

	return bytesRead;
}

m4uint32_t CInputStream::readInteger( m4uint32_t& ao_riInteger )
{
	m4bool_t	bNegative;
	m4uchar_t	i;
	m4uchar_t	byte;
	m4uchar_t	intByte;
	m4uint32_t	mask;
	m4uint32_t	bytesRead = 0;

	mask = 0xFFFFFFFF;
	bNegative = M4_FALSE;
	ao_riInteger = 0;

	bytesRead += readByte( byte );

	for( i = byte ; i > 0 ; i-- )
	{
		mask <<= 8;
		bytesRead += readByte( intByte );

		if( (i == byte) && (intByte & 0x80) )
		{
			bNegative = M4_TRUE;
		}
			
		ao_riInteger += ( m4uint32_t( intByte ) << 8 * ( i - 1 ) );
	}

	if( bNegative == M4_TRUE )
	{
		ao_riInteger |= mask;
	}

	return bytesRead;
}

m4uint32_t CInputStream::readString( m4pchar_t &ao_rpcString, m4uint32_t &ao_riLength, m4uchar_t ai_iType )
{
	m4uint32_t bytesRead = ao_riLength;

	ao_rpcString = new m4char_t[ bytesRead + 1 ];
	memcpy( ao_rpcString, m_stream + m_cont, bytesRead ) ;
	ao_rpcString[ bytesRead ] = '\0';
	m_cont += bytesRead;

	if( ai_iType != m_iStringBaseType && M4IsAscii( ao_rpcString, bytesRead ) == 0 )
	{
		m4pchar_t pcValue = ao_rpcString;
		int iLength = bytesRead;

		if( ai_iType == ASNUnicodeString )
		{
			// Si es Utf8 se pasa a ANSI
			ao_rpcString = M4Utf8ToCpp( pcValue, iLength );
		}
		else
		{
			// Si es ANSI se pasa a Utf8
			ao_rpcString = M4ANSIToCpp( pcValue, iLength );
		}
		delete pcValue;
		ao_riLength = iLength;
	}

	return bytesRead;
}

m4uint32_t CInputStream::readString( m4pchar_t &ao_rpcString, m4uint32_t &ao_riLength, m4pchar_t &ao_rpcCopy, m4return_t &ao_riResult )
{
	m4uchar_t	byte;
	m4uint32_t	bytesRead = 0;

	ao_rpcString = NULL;
	ao_riLength = 0;
	ao_rpcCopy = NULL;
	ao_riResult = M4_SUCCESS;

	bytesRead += readByte( byte );

	if( byte != ASNGeneralString && byte != ASNUnicodeString )
	{
		ao_riResult = M4_ERROR;
		return bytesRead;
	}

	bytesRead += readLength( ao_riLength );
	bytesRead += readStringReference( ao_rpcString, ao_riLength, byte, ao_rpcCopy );

	return bytesRead;
}

m4uint32_t CInputStream::readStringReference( m4pchar_t &ao_rpcString, m4uint32_t &ao_riLength, m4uchar_t ai_iType, m4pchar_t &ao_rpcCopy )
{
	m4uint32_t bytesRead = ao_riLength;

	ao_rpcCopy = NULL;
	ao_rpcString = (m4pchar_t)m_stream + m_cont;
	m_cont += bytesRead;

	if( ai_iType != m_iStringBaseType && M4IsAscii( ao_rpcString, bytesRead ) == 0 )
	{
		m4pchar_t pcValue = new m4char_t[ bytesRead + 1 ];
		memcpy( pcValue, ao_rpcString, bytesRead ) ;
		pcValue[ bytesRead ] = '\0';

		int iLength = bytesRead;

		if( ai_iType == ASNUnicodeString )
		{
			// Si es Utf8 se pasa a ANSI
			ao_rpcString = M4Utf8ToCpp( pcValue, iLength );
		}
		else
		{
			// Si es ANSI se pasa a Utf8
			ao_rpcString = M4ANSIToCpp( pcValue, iLength );
		}
		delete pcValue;
		ao_riLength = iLength;
		ao_rpcCopy = ao_rpcString;
	}

	return bytesRead;
}

m4bool_t CInputStream::eof( void ) const
{
	///  determinar fin de vector ASN1
	if (m_cont < m_iLength)
	{
		return M4_FALSE;
	}
	return M4_TRUE;
}

