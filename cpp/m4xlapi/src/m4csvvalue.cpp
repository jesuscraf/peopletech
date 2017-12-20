//==============================================================================
//
// (c) Copyright  1991-2013 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4xlapi
// File:             m4csvvalue.cpp
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

#include <m4csvvalue.hpp>
#include <m4unicode.hpp>
#include <m4log.hpp>
#include <m4xlapierrors.hpp>


// -----------------------------------------------------------------------------
// Class ClCsvCharBase
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Contructor.
// -----------------------------------------------------------------------------
ClCsvCharBase::ClCsvCharBase()
{
	m_bUnassigned = true;
}


// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
ClCsvCharBase::~ClCsvCharBase(){}


// -----------------------------------------------------------------------------
// Returns the unassigned attribute.
// -----------------------------------------------------------------------------
bool ClCsvCharBase::IsUnassigned()
{
	return m_bUnassigned;
}


// -----------------------------------------------------------------------------
// Class ClCsvCharSingle
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Constructor.
// -----------------------------------------------------------------------------
ClCsvCharSingle::ClCsvCharSingle(){}


// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
ClCsvCharSingle::~ClCsvCharSingle(){}


// -----------------------------------------------------------------------------
// Loads a single-byte character from file.
// -----------------------------------------------------------------------------
m4return_t ClCsvCharSingle::Read( FILE* ai_pFile )
{
	m_cChar = fgetc( ai_pFile );

	m_bUnassigned = false;

	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Loads a single-byte character from buffer.
// -----------------------------------------------------------------------------
m4return_t ClCsvCharSingle::Read( const char* ai_pcData, int ai_iIndex )
{
	m_cChar = ai_pcData[ ai_iIndex ];

	m_bUnassigned = false;

	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Returns if this character is "end of file".
// -----------------------------------------------------------------------------
bool ClCsvCharSingle::IsEOF()
{
	return( m_cChar == EOF );
}


// -----------------------------------------------------------------------------
// Returns the size of the character.
// -----------------------------------------------------------------------------
int ClCsvCharSingle::GetSize()
{
	return sizeof( m_cChar );
}


// -----------------------------------------------------------------------------
// Returns the character as a sequence of bytes.
// -----------------------------------------------------------------------------
char* ClCsvCharSingle::GetBytes()
{
	return &m_cChar;
}


// -----------------------------------------------------------------------------
// Character comparison operator.
// -----------------------------------------------------------------------------
bool ClCsvCharSingle::operator==( const char ai_Char )
{
	return( m_cChar == ai_Char );
}


// -----------------------------------------------------------------------------
// Character comparison operator.
// -----------------------------------------------------------------------------
bool ClCsvCharSingle::operator!=( const char ai_Char )
{
	return ( m_cChar != ai_Char );
}


// -----------------------------------------------------------------------------
// Single-byte character object comparison operator.
// -----------------------------------------------------------------------------
bool ClCsvCharSingle::operator==( const ClCsvCharBase& ai_Char )
{
	ClCsvCharSingle*	pChar = (ClCsvCharSingle*)&ai_Char;

	return( m_cChar == pChar->m_cChar );
}


// -----------------------------------------------------------------------------
// Single-byte character object assignment operator.
// -----------------------------------------------------------------------------
ClCsvCharBase& ClCsvCharSingle::operator=( const ClCsvCharBase& ai_Char )
{
	ClCsvCharSingle*	pChar = (ClCsvCharSingle*)&ai_Char;

	m_cChar = pChar->m_cChar;

	m_bUnassigned = false;

	return *this;
}


// -----------------------------------------------------------------------------
// Class ClCsvCharDoubleBase
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Contructor.
// -----------------------------------------------------------------------------
ClCsvCharDoubleBase::ClCsvCharDoubleBase(){}

// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
ClCsvCharDoubleBase::~ClCsvCharDoubleBase(){}


// -----------------------------------------------------------------------------
// Loads an UTF16 character from file.
// -----------------------------------------------------------------------------
m4return_t ClCsvCharDoubleBase::Read( FILE* ai_pFile )
{
	char*	pcChar = ( char* )&m_iChar;

	*pcChar			= fgetc( ai_pFile );
	*( pcChar + 1 )	= fgetc( ai_pFile );

	m_bUnassigned = false;

	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Loads an UTF16 character from buffer.
// -----------------------------------------------------------------------------
m4return_t ClCsvCharDoubleBase::Read( const char* ai_pcData, int ai_iIndex )
{
	char* pcChar = ( char* )&m_iChar;

	*pcChar			= ai_pcData[ ai_iIndex ];
	*( pcChar + 1 )	= ai_pcData[ ai_iIndex + 1 ];

	m_bUnassigned = false;

	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Returns if this character is "end of file".
// -----------------------------------------------------------------------------
bool ClCsvCharDoubleBase::IsEOF()
{
	return( m_iChar == ( unsigned short )WEOF );
}

// -----------------------------------------------------------------------------
// Returns the size of the character.
// -----------------------------------------------------------------------------
int ClCsvCharDoubleBase::GetSize()
{
	return sizeof( m_iChar );
}


// -----------------------------------------------------------------------------
// Returns the character as a sequence of bytes.
// -----------------------------------------------------------------------------
char* ClCsvCharDoubleBase::GetBytes()
{
	return ( char* )&m_iChar;
}


// -----------------------------------------------------------------------------
// UTF16 character object comparison operator.
// -----------------------------------------------------------------------------
bool ClCsvCharDoubleBase::operator==( const ClCsvCharBase& ai_Char )
{
	ClCsvCharDoubleBase*	pChar = (ClCsvCharDoubleBase*)&ai_Char;

	return( m_iChar == pChar->m_iChar );
}


// -----------------------------------------------------------------------------
// UTF16 character object assignment operator.
// -----------------------------------------------------------------------------
ClCsvCharBase& ClCsvCharDoubleBase::operator=( const ClCsvCharBase& ai_Char )
{
	ClCsvCharDoubleBase*	pChar = (ClCsvCharDoubleBase*)&ai_Char;

	m_iChar = pChar->m_iChar;

	m_bUnassigned = false;

	return *this;
}


// -----------------------------------------------------------------------------
// Class ClCsvCharDoubleBE
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Constructor.
// -----------------------------------------------------------------------------
ClCsvCharDoubleBE::ClCsvCharDoubleBE(){}


// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
ClCsvCharDoubleBE::~ClCsvCharDoubleBE(){}


// -----------------------------------------------------------------------------
// Character comparison operator.
// -----------------------------------------------------------------------------
bool ClCsvCharDoubleBE::operator==( const char ai_Char )
{
	char* pcChar = (char*)&m_iChar;

	return( *( pcChar + 1 ) == ai_Char && *( pcChar ) == 0 );
}


// -----------------------------------------------------------------------------
// Character comparison operator.
// -----------------------------------------------------------------------------
bool ClCsvCharDoubleBE::operator!=( const char ai_Char )
{
	char* pcChar = (char*)&m_iChar;

	return( *( pcChar + 1 ) != ai_Char || *( pcChar ) != 0 );
}


// -----------------------------------------------------------------------------
// Class ClCsvCharDoubleLE
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Constructor.
// -----------------------------------------------------------------------------
ClCsvCharDoubleLE::ClCsvCharDoubleLE(){}


// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
ClCsvCharDoubleLE::~ClCsvCharDoubleLE(){}


// -----------------------------------------------------------------------------
// Character comparison operator.
// -----------------------------------------------------------------------------
bool ClCsvCharDoubleLE::operator==( const char ai_Char )
{
	char* pcChar = (char*)&m_iChar;

	return( *( pcChar ) == ai_Char && *( pcChar + 1 ) == 0 );
}


// -----------------------------------------------------------------------------
// Character comparison operator.
// -----------------------------------------------------------------------------
bool ClCsvCharDoubleLE::operator!=( const char ai_Char )
{
	char* pcChar = (char*)&m_iChar;

	return( *( pcChar ) != ai_Char || *( pcChar + 1 ) != 0 );
}


// -----------------------------------------------------------------------------
// Class ClCsvValue
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Constructor
// 
//	ai_lMaxValLen:	Maximum number of characters in the string.
// -----------------------------------------------------------------------------
ClCsvValue::ClCsvValue( int ai_lMaxValLen, eXLEncodings_t ai_iEncoding )
{
	m_pcBuffer = new char[ ai_lMaxValLen + 1 ];

	if( !m_pcBuffer )
	{
		m_lBufferSize = 0;
	}
	else
	{
		m_lBufferSize = ai_lMaxValLen + 1;
	}

	m_iEncoding	= ai_iEncoding;

	Reset();
}


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
ClCsvValue::~ClCsvValue()
{
	if( m_pcBuffer != NULL )
	{
		delete [] m_pcBuffer;
	}
}


// -----------------------------------------------------------------------------
// Clears the string content.
// -----------------------------------------------------------------------------
m4return_t ClCsvValue::Reset()
{
	if( !m_pcBuffer )
	{
		return M4_ERROR;
	}

	memset( m_pcBuffer, 0, m_lBufferSize );

	m_iInsPos = 0;


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Returns if the value is empty.
// -----------------------------------------------------------------------------
m4bool_t ClCsvValue::IsEmpty()
{
	return( m_iInsPos == 0 );
}


// -----------------------------------------------------------------------------
// Inserts a character at the end of the value.
// 
//	ai_iChar:	Character to append.
// -----------------------------------------------------------------------------
const int c_iGrowSize = 1024;
m4return_t	ClCsvValue::Append( ClCsvCharBase* ai_pChar )
{
	int		iCharSize;
	char*	pcNewBuffer;


	iCharSize = ai_pChar->GetSize();

	// Checks if there is enought room to place the character preserving the trailing null.
	// Have in mind that insertion position is pointing to the next byte being written, so  
	// we don't have to subtract one on both operands.
	if( m_iInsPos + iCharSize >= m_lBufferSize )
	{
		// Creates a bigger buffer.
		pcNewBuffer = new char[ m_lBufferSize + c_iGrowSize ];

		if( pcNewBuffer )
		{
			memset( pcNewBuffer + m_lBufferSize, 0, c_iGrowSize );
			memcpy( pcNewBuffer, m_pcBuffer, m_lBufferSize );
			delete [] m_pcBuffer;
			
			m_pcBuffer = pcNewBuffer;
			m_lBufferSize += c_iGrowSize;
		}

		// No memory or growing size is not enought for the appending char.
		if( m_iInsPos + iCharSize >= m_lBufferSize )
		{
			SETCODEF( M4_XL_ERR_PARS_SIZE_EXCEEDED, ERRORLOG, "#*s1*#" );
			return M4_ERROR;
		}
	}

	memcpy( m_pcBuffer + m_iInsPos, ai_pChar->GetBytes(), iCharSize );

	m_iInsPos += iCharSize;

	
	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Splits the value into the two strings contained.
//
//	ai_iStrSep:			Strings delimiter.
//	ai_iEnclosingChar:	Char used for enclosing values.
//	ai_bLastCol:		Boolean indicating whether the value is the last one in
//						the row or not.
//	ao_pwcM4Value:		Output left string.
//	ao_pwcISOValue:		Output right string.
//	ao_bIgnore:			Output boolean indicating if the parser must ignore the 
//						null pointer returned.
// -----------------------------------------------------------------------------
m4return_t	ClCsvValue::GetStrings( char ai_iStrSep, char ai_iEnclosingChar, m4bool_t ai_bLastCol, wchar_t*& ao_pwcM4Value, wchar_t*& ao_pwcISOValue, m4bool_t& ao_bIgnore )
{
	m4return_t		iRet;
	char			*pcM4Value = NULL, *pcISOValue = NULL;
	int				iM4Len, iISOLen;
	const wchar_t*	pwcSubStr;
	ClCsvCharBase	*pCsvChar;
	int				iCharSize;
	
	
	// Output defaults.
	ao_pwcM4Value = ao_pwcISOValue = NULL;
	ao_bIgnore = M4_FALSE;

	if( !m_pcBuffer )
	{
		return M4_ERROR;
	}


	if( m_iInsPos == 0 )
	{
		if( ai_bLastCol == M4_TRUE )
		{
			// Multibyte Csv ending with ;\n
			ao_bIgnore = M4_TRUE;
			return M4_SUCCESS;
		}
		// else: Empty datum (;;) that must overwrite the template cell.
	}

	
	// Builds a single character container.
	switch( m_iEncoding )
	{
		case M4XL_M4CPP:
		case M4XL_ANSI:
		case M4XL_UTF8:
			pCsvChar	 = new ClCsvCharSingle();
			break;
		case M4XL_UTF16LE:
			pCsvChar	 = new ClCsvCharDoubleLE();
			break;
		case M4XL_UTF16BE:
			pCsvChar	 = new ClCsvCharDoubleBE();
			break;
	}
	
	
	pcM4Value = m_pcBuffer;
	iCharSize	= pCsvChar->GetSize();

	// bg 266718
	// Removes \r coming from csv strings which have \r\n line endings.
	if( m_iInsPos > 0 )
	{
		pCsvChar->Read( pcM4Value, m_iInsPos - iCharSize );

		if( *pCsvChar == '\r' )
		{
			// bg 294158
			memset( pcM4Value + ( m_iInsPos - iCharSize ), 0, iCharSize );

			m_iInsPos -= iCharSize;
		}
	}

	if( m_iInsPos == 0 )
	{
		// Csv value ending with ;\r\n
		ao_bIgnore = M4_TRUE;
		delete pCsvChar;

		return M4_SUCCESS;
	}

	
	iM4Len		= m_iInsPos;
	pcISOValue	= NULL;
	iISOLen		= 0;
		
	
	// Splits the whole value to get the two strings.
	for( int i = 0; i < m_iInsPos; )
	{
		pCsvChar->Read( pcM4Value, i );

		if( *pCsvChar == ai_iStrSep )
		{
			memset( pcM4Value + i, 0, iCharSize );
			
			iM4Len = i;
			pcISOValue	= pcM4Value + i + iCharSize;
			iISOLen		= m_iInsPos - i - iCharSize;

			break;
		}

		i += iCharSize;
	}

	delete pCsvChar;


	// Csv value with ISO-only value.
	if( iM4Len == 0 )
	{
		return M4_SUCCESS;
	}
	

	// Converts the Meta4-formated string to native wide char string.
	iRet = StrToNativeWCStr( pcM4Value, iM4Len, m_iEncoding, ao_pwcM4Value );

	if( iRet == M4_ERROR )
	{
		return M4_ERROR;
	}

	if( iISOLen > 0 )
	{
		// Converts the ISO-formated string to native wide char string.
		iRet = StrToNativeWCStr( pcISOValue, iISOLen, m_iEncoding, ao_pwcISOValue );

		if( iRet == M4_ERROR )
		{
			return M4_ERROR;
		}
	}


	// Manage meta4 csv peculiarities.
	if( ao_pwcISOValue != NULL )
	{
		iISOLen = wcslen( ao_pwcISOValue );

		// If the whole value are double quote enclosed ...
		if( ao_pwcM4Value[ 0 ] == ai_iEnclosingChar && ao_pwcISOValue[ iISOLen - 1 ] == ai_iEnclosingChar )
		{
			RemoveCharAt( ao_pwcM4Value, 0 );
			RemoveCharAt( ao_pwcISOValue, iISOLen - 1 );

			// ... unscapes double quote characters in the Meta4-formated string ...
			pwcSubStr = ao_pwcM4Value;
			while( ( pwcSubStr = wcsstr( pwcSubStr, L"\"\"" ) ) != NULL )
			{
				RemoveCharAt( ao_pwcM4Value, pwcSubStr - ao_pwcM4Value );
				pwcSubStr++;
			}
			// ... and in the ISO-formated string.
			pwcSubStr = ao_pwcISOValue;
			while( ( pwcSubStr = wcsstr( pwcSubStr, L"\"\"" ) ) != NULL )
			{
				RemoveCharAt( ao_pwcISOValue, pwcSubStr - ao_pwcISOValue );
				pwcSubStr++;
			}
		}
	}
	else if( ao_pwcM4Value != NULL )
	{
		iM4Len = wcslen( ao_pwcM4Value );

		// If there is only one value, the Meta4-formated one, and it is double quote enclosed...
		if( ao_pwcM4Value[ 0 ] == ai_iEnclosingChar && ao_pwcM4Value[ iM4Len - 1 ] == ai_iEnclosingChar )
		{
			RemoveCharAt( ao_pwcM4Value, 0 );
			RemoveCharAt( ao_pwcM4Value, iM4Len - 2 );

			// ... unscapes double quote characters in the Meta4-formated string.
			pwcSubStr = ao_pwcM4Value;
			while( ( pwcSubStr = wcsstr( pwcSubStr, L"\"\"" ) ) != NULL )
			{
				RemoveCharAt( ao_pwcM4Value, pwcSubStr - ao_pwcM4Value );
				pwcSubStr++;
			}
		}
	}


	return M4_SUCCESS;
}
