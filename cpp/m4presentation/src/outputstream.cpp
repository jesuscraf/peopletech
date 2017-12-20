//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// File:             OutputStream.cpp
// Project:          Service presentation
// Author:           Meta Software M.S. , S.A
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Implementation of the COutStream class.
//
//
//==============================================================================


#include "asndef.h"
#include "m4unicode.hpp"
extern int	M4IsUnicode( void ) ;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COutputStream::COutputStream()//: m_stream(filename,ios::out|ios::binary)
{
	m_cont = 0;
}

COutputStream::~COutputStream()
{
	m_stream.erase(m_stream.begin(), m_stream.end());
}

m4uint32_t COutputStream::GetLength() const
{
	return m_cont;
}

void COutputStream::writeByte(m4uchar_t ai_iByte)
{
	m_stream.push_back(ai_iByte);
	m_cont++;
}

void COutputStream::writeString( m4uchar_t *ai_pcString, m4uint32_t ai_iLength, m4uint32_t ai_iTypePosition )
{
	if( M4IsUnicode() == 1 && M4IsAscii( (char*)ai_pcString, ai_iLength ) == 0 )
	{
		m_stream[ ai_iTypePosition ] = ASNUnicodeString;
	}

	for( m4uint32_t i = 0; i < ai_iLength; i++ )
	{
		m_stream.push_back( ai_pcString[ i ] );
	}

	m_cont += ai_iLength;
}


