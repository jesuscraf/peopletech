//==============================================================================
//
// (c) Copyright  1991-2013 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4xlapi
// File:             m4csvvalue.hpp
// Project:			 mind3.x
// Author:           Meta Software M.S. , S.A
// Date:             12/11/2013
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Owner:			
//	Modifications:	
//
// Definition:
//
//    LN4 API for MS Excel.
//==============================================================================

#ifndef m4csvvalue_hpp
#define m4csvvalue_hpp 1

#include <m4xlhelper.hpp>
#include <m4types.hpp>
#include <wchar.h>


// -----------------------------------------------------------------------------
// Class ClCsvCharBase
//
// Base class for storing and managing single csv characters.
// -----------------------------------------------------------------------------

class ClCsvCharBase
{
	public:
		ClCsvCharBase();
		
		// Returns the unassigned attribute.
		bool IsUnassigned();

		virtual ~ClCsvCharBase();
		
		// Loads a single-byte character from file.
		virtual m4return_t Read( FILE* ai_pFile ) = 0;

		// Loads a single-byte character from buffer.
		virtual m4return_t Read( const char* ai_pcData, int ai_iIndex ) = 0;

		// Returns if this character is "end of file".
		virtual bool IsEOF() = 0;

		// Returns the size of the character.
		virtual int GetSize() = 0;

		// Returns the character as a sequence of bytes.
		virtual char* GetBytes() = 0;

		// Character comparison operator.
		virtual bool operator==( const char ai_Char ) = 0;

		// Character comparison operator.
		virtual bool operator!=( const char ai_Char ) = 0;

		// Character object comparison operator.
		virtual bool operator==( const ClCsvCharBase& ai_Char ) = 0;

		// Character object assignment operator.
		virtual ClCsvCharBase& operator=( const ClCsvCharBase& ai_Char ) = 0;

	protected:
		bool	m_bUnassigned;
};


// -----------------------------------------------------------------------------
// Class ClCsvCharSingle
//
// Class for storing and managing single-byte characters.
// -----------------------------------------------------------------------------

class ClCsvCharSingle: public ClCsvCharBase
{
	public:
		ClCsvCharSingle();
		~ClCsvCharSingle();

		// Loads a single-byte character from file.
		m4return_t Read( FILE* ai_pFile );

		// Loads a single-byte character from buffer.
		m4return_t Read( const char* ai_pcData, int ai_iIndex );

		// Returns if this character is "end of file".
		bool IsEOF();

		// Returns the size of the character.
		int GetSize();

		// Returns the character as a sequence of bytes.
		char* GetBytes();

		// Character comparison operator.
		bool operator==( const char ai_Char );

		// Character comparison operator.
		bool operator!=( const char ai_Char );

		// Single-byte character object comparison operator.
		bool operator==( const ClCsvCharBase& ai_Char );

		// Single-byte character object assignment operator.
		ClCsvCharBase& operator=( const ClCsvCharBase& ai_Char );

	protected:
		char	m_cChar;
};


// -----------------------------------------------------------------------------
// Class ClCsvCharDoubleBase
//
// Base class for storing and managing double-byte characters.
// -----------------------------------------------------------------------------

class ClCsvCharDoubleBase: public ClCsvCharBase
{
	public:
		ClCsvCharDoubleBase();
		virtual ~ClCsvCharDoubleBase();

		// Loads an UTF16 character from file.
		m4return_t Read( FILE* ai_pFile );

		// Loads an UTF16 character from buffer.
		m4return_t Read( const char* ai_pcData, int ai_iIndex );

		// Returns if this character is "end of file".
		bool IsEOF();

		// Returns the size of the character.
		int GetSize();

		// Returns the character as a sequence of bytes.
		char* GetBytes();

		// UTF16 character object comparison operator.
		bool operator==( const ClCsvCharBase& ai_Char );

		// UTF16 character object assignment operator.
		ClCsvCharBase& operator=( const ClCsvCharBase& ai_Char );

	protected:
		unsigned short	m_iChar;
};


// -----------------------------------------------------------------------------
// Class ClCsvCharDoubleBE
//
// Class for storing and managing UTF16 big-endian characters.
// -----------------------------------------------------------------------------

class ClCsvCharDoubleBE: public ClCsvCharDoubleBase
{
	public:
		ClCsvCharDoubleBE();
		~ClCsvCharDoubleBE();
		
		// Character comparison operator.
		bool operator==( const char ai_Char );

		// Character comparison operator.
		bool operator!=( const char ai_Char );
};


// -----------------------------------------------------------------------------
// Class ClCsvCharDoubleLE
//
// Class for storing and managing UTF16 little-endian characters.
// -----------------------------------------------------------------------------

class ClCsvCharDoubleLE: public ClCsvCharDoubleBase
{
	public:
		ClCsvCharDoubleLE();
		~ClCsvCharDoubleLE();

		// Character comparison operator.
		bool operator==( const char ai_Char );

		// Character comparison operator.
		bool operator!=( const char ai_Char );
};


// -----------------------------------------------------------------------------
// Class ClCsvValue
//
// Class for storing and managing a Meta4 csv value.
// -----------------------------------------------------------------------------

class ClCsvValue
{
	public:

		ClCsvValue( int ai_lMaxValLen, eXLEncodings_t ai_iEncoding );
		~ClCsvValue();

		// Clears the string content.
		m4return_t	Reset		();

		// Returns if the value is empty.
		m4bool_t	IsEmpty		();

		// Inserts a character at the end of the value.
		m4return_t	Append		( ClCsvCharBase* ai_pChar );

		// Splits the value into the two strings contained.
		m4return_t	GetStrings	( char ai_iStrSep, char ai_iEnclosingChar, m4bool_t ai_bLastCol, wchar_t*& pwcM4Value, wchar_t*& pwcISOValue, m4bool_t& ao_bIgnore );

	private:

		char*			m_pcBuffer;
		int				m_lBufferSize;
		int				m_iInsPos;
		eXLEncodings_t	m_iEncoding;
};


#endif
