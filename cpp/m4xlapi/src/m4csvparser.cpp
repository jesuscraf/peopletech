//==============================================================================
//
// (c) Copyright  1991-2013 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4xlapi
// File:             m4csvparser.cpp
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

#include <m4csvparser.hpp>
#include <m4log.hpp>
#include <m4xlapierrors.hpp>
#include <m4unicode.hpp>

// -----------------------------------------------------------------------------
// Class ClM4CsvParser
// -----------------------------------------------------------------------------

// Class constants.
const char	ClM4CsvParser::s_EnclosingChar				= '\"';
const char	ClM4CsvParser::s_RowTerminatorChar_n		= '\n';
const char	ClM4CsvParser::s_RowTerminatorChar_r		= '\r';
const int	ClM4CsvParser::s_MaxValueLength				= 8192;
const char	ClM4CsvParser::s_MultiValueSeparatorChar	= 0x3;


// -----------------------------------------------------------------------------
// Constructor
// 
//	ai_pSheetAccessor:	Reference to an object which implements ISheetAccessor.
// -----------------------------------------------------------------------------
ClM4CsvParser::ClM4CsvParser( ISheetAccessor* ai_pSheetAccessor )
{
	m_pfCSVFile			= NULL;
	m_pcCSVData			= NULL;
	m_pSheetAccessor	= ai_pSheetAccessor;
	m_iReadIndex		= 0;
	m_pCsvValue			= NULL;
	m_iCurrRow			= 0;
	m_iCurrCol			= 0;
}


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
ClM4CsvParser::~ClM4CsvParser()
{
	if( m_pfCSVFile != NULL )
	{
		fclose( m_pfCSVFile );
		m_pfCSVFile = NULL;
	}

	if( m_pCsvValue != NULL )
	{
		delete m_pCsvValue;
	}
}


// -----------------------------------------------------------------------------
// Sets the CSV file path to parse and the values delimiter character.
// 
//	ai_pcCSVFile:		CSV file path.
//	ai_cValTerminator:	Value delimiter.
//
//	Note: ANSI, UTF16 and UTF8 file encondings are supported.
// -----------------------------------------------------------------------------
m4return_t ClM4CsvParser::SetCsvFile( const char* ai_pcCSVFile, char ai_cValTerminator )
{
	size_t	iRead = 0;
	char	acBOM[ M4UNI_UTF8_BOM_SIZE ];
	
	// Working in file input mode.
	if( m_pcCSVData != NULL )
	{
		m_pcCSVData = NULL;
	}

	if( m_pfCSVFile != NULL )
	{
		fclose( m_pfCSVFile );
		m_pfCSVFile = NULL;
	}

	m_pfCSVFile = fopen( ai_pcCSVFile, "rb" );

	if( m_pfCSVFile == NULL )
	{
		SETCODEF( M4_XL_ERR_PARS_BAD_DATA_FILE, ERRORLOG, "#*s1*#%s#", ai_pcCSVFile );
		return M4_ERROR;
	}

	m_cValueTerminator	= ai_cValTerminator;
	m_bHasMoreRows		= M4_TRUE;

	
	// Checks if file is UTF8 encoded.
	iRead = fread( acBOM, 1, M4UNI_UTF8_BOM_SIZE, m_pfCSVFile );
	
	if( iRead == M4UNI_UTF8_BOM_SIZE && memcmp( acBOM, acUTF8BOM, M4UNI_UTF8_BOM_SIZE ) == 0 )
	{
		m_iEncoding = M4XL_UTF8;
		m_pCsvValue = new ClCsvValue( s_MaxValueLength, m_iEncoding );
	}
	else
	{

		rewind( m_pfCSVFile );

		// Checks if file is UTF16 encoded.
		iRead = fread( acBOM, 1, M4UNI_UTF16_BOM_SIZE, m_pfCSVFile );

		if( iRead == M4UNI_UTF16_BOM_SIZE && memcmp( acBOM, acUTF16WinBOM, M4UNI_UTF16_BOM_SIZE ) == 0 )
		{
			m_iEncoding = M4XL_UTF16LE;
			m_pCsvValue = new ClCsvValue( s_MaxValueLength, m_iEncoding );
		}
		else if ( iRead == M4UNI_UTF16_BOM_SIZE && memcmp( acBOM, acUTF16UnixBOM, M4UNI_UTF16_BOM_SIZE ) == 0 )
		{
			m_iEncoding = M4XL_UTF16BE;
			m_pCsvValue = new ClCsvValue( s_MaxValueLength, m_iEncoding );
		}
		else
		{
			m_iEncoding = M4XL_ANSI;
			m_pCsvValue = new ClCsvValue( s_MaxValueLength, m_iEncoding );

			rewind( m_pfCSVFile );
		}
	}

	
	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Sets the CSV string to parse and the values delimiter character.
// 
//	ai_pcCSVData:		CSV string
//	ai_cValTerminator:	Value delimiter.
//
//	Note: ANSI and UTF8 data encondings are supported.
// -----------------------------------------------------------------------------
m4return_t	ClM4CsvParser::SetCsvString( const char* ai_pcCSVData, char ai_cValTerminator )
{
	// Working in string input mode.
	if( m_pfCSVFile != NULL )
	{
		fclose( m_pfCSVFile );
		m_pfCSVFile = NULL;
	}

	m_pcCSVData = ai_pcCSVData;

	if( m_pcCSVData == NULL )
	{
		SETCODEF( M4_XL_ERR_PST_BAD_DATA, ERRORLOG, "#*s1*#" );
		return M4_ERROR;
	}

	m_cValueTerminator	= ai_cValTerminator;
	m_bHasMoreRows		= M4_TRUE;
	m_iReadIndex		= 0;

	m_iEncoding = M4XL_M4CPP;
	m_pCsvValue	= new ClCsvValue( s_MaxValueLength, m_iEncoding );


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Parses the whole CSV file or string.
// -----------------------------------------------------------------------------
m4return_t ClM4CsvParser::Parse()
{
	m4return_t	iRet;

	m_bFailSafeMode	= M4_FALSE;
	m_iCurrRow		= 0;

	// Parse all csv lines.
	while( m_bHasMoreRows == M4_TRUE )
	{
		iRet = parseSingleRow();

		if( iRet != M4_SUCCESS )
		{
			return M4_ERROR;
		}
	}

	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Parses a single line of the CSV file or string.
// -----------------------------------------------------------------------------
m4return_t ClM4CsvParser::parseSingleRow()
{
	m4return_t		iRet;
	m4bool_t		bRowCompleted = M4_FALSE;
	ClCsvCharBase	*pCsvChar, *pCsvPrevChar;
	m4bool_t		bReadingValue;
	int				iNumEnclChars;
	m4bool_t		bSheetReset;
	m4bool_t		bIsRowTerminatorChar;


	if( m_pfCSVFile == NULL && m_pcCSVData == NULL )
	{
		return M4_ERROR;
	}

	// Builds a single character container.
	switch( m_iEncoding )
	{
		case M4XL_M4CPP:
		case M4XL_ANSI:
		case M4XL_UTF8:
			pCsvChar	 = new ClCsvCharSingle();
			pCsvPrevChar = new ClCsvCharSingle();
			break;
		case M4XL_UTF16LE:
			pCsvChar	 = new ClCsvCharDoubleLE();
			pCsvPrevChar = new ClCsvCharDoubleLE();
			break;
		case M4XL_UTF16BE:
			pCsvChar	 = new ClCsvCharDoubleBE();
			pCsvPrevChar = new ClCsvCharDoubleBE();
			break;
	}

	m_pCsvValue->Reset();
	bReadingValue	= M4_FALSE;
	iNumEnclChars	= 0;
	m_iCurrCol		= 0;
	bSheetReset		= M4_FALSE;


	// Single row parse loop.
	while( bRowCompleted == M4_FALSE )
	{
		// Gets next char.
		if( m_pfCSVFile != NULL )
		{
			pCsvChar->Read( m_pfCSVFile );
		}
		else
		{
			pCsvChar->Read( m_pcCSVData, m_iReadIndex++ );
		}

		// Main switch for incoming chars.
		if( ( m_pfCSVFile != NULL && pCsvChar->IsEOF() ) || 
			( m_pfCSVFile == NULL && *pCsvChar == 0 ) )
		{
			// ImportCSVEx method is receiving csv files where there aren't value-separator
			// char at the end of each line, so the current value is completely read.
			iRet = processReadedValue( M4_FALSE, bSheetReset );

			if( iRet == M4_ERROR )
			{
				delete pCsvPrevChar;
				delete pCsvChar;

				return M4_ERROR;
			}

			if( bSheetReset == M4_FALSE )
			{
				m_iCurrCol++;
			}

			// The file has been completely processed.
			m_bHasMoreRows = M4_FALSE;
			bRowCompleted  = M4_TRUE;

		}
		else if( *pCsvChar == s_EnclosingChar )
		{
			iNumEnclChars++;

			if( bReadingValue == M4_FALSE )
			{
				if( ( *pCsvPrevChar == m_cValueTerminator || pCsvPrevChar->IsUnassigned() ) )
				{
					// It is starting a new quoted value.
					bReadingValue = M4_TRUE;
				}
			}
		
			failSafeAppend( pCsvChar );

		}
		else 
		{
			// Forces evaluation.
			bIsRowTerminatorChar = isRowTerminatorChar( pCsvChar );

			if( *pCsvChar == m_cValueTerminator || bIsRowTerminatorChar == M4_TRUE )
			{
				if( *pCsvPrevChar == s_EnclosingChar && ( iNumEnclChars %  2 ) == 0 )
				{
					// Rules for enclosing values:
					//
					//	1- Old fashion csv stored in db could be enclosed.
					//	2- Mandatory if the value contains a value terminator or an end of line.
					//	3- In case of 2, all quotes inside the value must be scaped with another quote.
					//
					// If the number of enclosing chars is even, we can be sure that the value has been 
					// read completely. If it would be inside the value and given that the rule 2 
					// forces to enclose all the value (it has a value terminator), the number of enclosing
					// chars should be one (odd). Similarly, if the value would contains an enclosing char
					// inside and given that the rule 3 forces to scape it, the number of enclosing
					// chars should be three and so (odd).
									
					// It is ending the current quoted value.
					bReadingValue = M4_FALSE;
				}

				if( bReadingValue == M4_TRUE )
				{
					failSafeAppend( pCsvChar );
				}
				else
				{
					// The current value is completely read.
					iRet = processReadedValue( M4_FALSE, bSheetReset );

					if( iRet == M4_ERROR )
					{
						delete pCsvPrevChar;
						delete pCsvChar;

						return M4_ERROR;
					}

					if( bSheetReset == M4_FALSE )
					{
						m_iCurrCol++;
					}

					if( bIsRowTerminatorChar == M4_TRUE )
					{
						bRowCompleted = M4_TRUE;
					}
					else
					{
						m_pCsvValue->Reset();
						iNumEnclChars  = 0;
					}
				}
			}
			else
			{
				failSafeAppend( pCsvChar );
			}
		}


		// Skip blanks for last chars checking.
		if( *pCsvChar != 32 && *pCsvChar != 0 && *pCsvChar != 13 )
		{
			*pCsvPrevChar = *pCsvChar;
		}
	}

	if( bSheetReset == M4_FALSE )
	{
		m_iCurrRow++;
	}

	delete pCsvPrevChar;
	delete pCsvChar;


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Process a csv value completely read.
// -----------------------------------------------------------------------------
m4return_t ClM4CsvParser::processReadedValue( m4bool_t ai_bLastCol, m4bool_t& aio_bSheetReset )
{
	m4return_t	iRet;
	wchar_t*	pwcM4Value = NULL;
	wchar_t*	pwcISOValue = NULL;
	m4bool_t	bIgnore = M4_FALSE;
	m4bool_t	bChangeSheet = M4_FALSE;
	int			iParamRow, iParamCol, iParamSheetIndex;
	wchar_t*	pwcParamSheetName;


	m_bFailSafeMode	= M4_FALSE;

	// The value can contain meta4 formated and ISO values, get both.
	iRet = m_pCsvValue->GetStrings( s_MultiValueSeparatorChar, s_EnclosingChar, ai_bLastCol, pwcM4Value, pwcISOValue, bIgnore );

	if( iRet == M4_ERROR )
	{
		return M4_ERROR;
	}

	if( bIgnore == M4_TRUE )
	{
		// (*) See bellow comment.
		return M4_SUCCESS;
	}

	// (*) Following set can not be done before returning when ignoring values. Cases like: chgsg;10
	// There was a reset while processing "chgsg". Now is processing "10" with a result of ignoring, and
	// neither should increase the current row index.
	
	aio_bSheetReset = M4_FALSE;


	// eh 276023
	if( pwcM4Value != NULL && pwcISOValue == NULL )
	{
		bChangeSheet = scanChangeSheetParams( pwcM4Value, iParamRow, iParamCol, iParamSheetIndex, pwcParamSheetName );

		if( bChangeSheet == M4_TRUE )
		{
			// Updates the accessor parameters.
			iRet = m_pSheetAccessor->ChangeSheet( iParamRow, iParamCol, iParamSheetIndex, pwcParamSheetName );

			// Resets current indexes.
			m_iCurrRow = m_iCurrCol = 0;
			aio_bSheetReset = M4_TRUE;

			if( pwcParamSheetName != NULL )
			{
				delete [] pwcParamSheetName;
			}
		}
	}

	if( iRet == M4_SUCCESS && bChangeSheet == M4_FALSE )
	{
		// Write the value in the Excel worksheet.
		iRet = m_pSheetAccessor->WriteValue( m_iCurrRow, m_iCurrCol, pwcM4Value, pwcISOValue );
	}

	// Splitting the value allocated memory for the resulting strings.
	delete [] pwcM4Value;

	if( pwcISOValue != NULL )
	{
		delete [] pwcISOValue;
	}


	return iRet;
}


// -----------------------------------------------------------------------------
// Extracts the change-sheet command parameters.
// Pattern: m4chgsh [Row] [Col] [SheetIndex] [SheetName]
// -----------------------------------------------------------------------------
#define MAX_PARAM_SIZE	64
m4bool_t	ClM4CsvParser::scanChangeSheetParams( wchar_t* ai_pwcValue, int& ao_iRow, int& ao_iCol, int& ao_iSheetIndex, wchar_t*& ao_pwcSheetName )
{
	m4bool_t	bRet = M4_FALSE;
	int			iLen;


	// Output defaults.
	ao_iRow = ao_iCol = ao_iSheetIndex = -1;
	ao_pwcSheetName = NULL;


	if( ai_pwcValue == NULL )
	{
		return bRet;
	}

	iLen = wcslen( ai_pwcValue );


	// Change-sheet minimum pattern: m4chgsh [] [] [] []
	if( iLen >= 19 )
	{
		int	iAssigned;
		wchar_t awcRow[ MAX_PARAM_SIZE ], awcCol[ MAX_PARAM_SIZE ], awcIndex[ MAX_PARAM_SIZE ], awcName[ MAX_PARAM_SIZE ];

		// In the worst case there might not be enought room for a single parameter.
		if( ( iLen - 19 ) >= MAX_PARAM_SIZE )
		{
			return bRet;
		}

#ifdef UNIX
		iAssigned = swscanf( ai_pwcValue, L"m4chgsh %S %S %S %[^\n]", awcRow, awcCol, awcIndex, awcName );
#else
		iAssigned = swscanf( ai_pwcValue, L"m4chgsh %s %s %s %[^\n]", awcRow, awcCol, awcIndex, awcName );
#endif

		if( iAssigned == 4 )
		{
			bRet = M4_TRUE;

			iLen = wcslen( awcRow );

			if( iLen > 2 && awcRow[ 0 ] == L'[' && awcRow[ iLen - 1 ] == L']' )
			{
				swscanf( awcRow + 1, L"%d", &ao_iRow );
			}

			iLen = wcslen( awcCol );

			if( iLen > 2 && awcCol[ 0 ] == L'[' && awcCol[ iLen - 1 ] == L']' )
			{
				swscanf( awcCol + 1, L"%d", &ao_iCol );
			}

			iLen = wcslen( awcIndex );

			if( iLen > 2 && awcIndex[ 0 ] == L'[' && awcIndex[ iLen - 1 ] == L']' )
			{
				swscanf( awcIndex + 1, L"%d", &ao_iSheetIndex );
			}

			iLen = wcslen( awcName );

			if( iLen > 2 && awcName[ 0 ] == L'[' && awcName[ iLen - 1 ] == L']' )
			{
				ao_pwcSheetName = new wchar_t[ iLen - 1 ];
				memcpy( ao_pwcSheetName, awcName + 1, ( iLen - 2  ) * sizeof( wchar_t ) );
				ao_pwcSheetName[ iLen - 2 ] = 0;
			}
		}
	}


	return bRet;
}


// -----------------------------------------------------------------------------
// Appends a csv character if not in fail-safe mode.
// -----------------------------------------------------------------------------
m4return_t	ClM4CsvParser::failSafeAppend( ClCsvCharBase *ai_pCsvChar )
{
	m4return_t	iRet;


	if( m_bFailSafeMode == M4_FALSE )
	{
		iRet = m_pCsvValue->Append( ai_pCsvChar );

		if( iRet == M4_ERROR )
		{
			m_bFailSafeMode = M4_TRUE;
		}
	}

	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Returns if current char must be interpreted as EOL.
//	Char 10 alone (Unix) or followed by 13 (Windows), always will be EOL.
//	Char 13 not followed by 10 (Mac) will be EOL too.
// -----------------------------------------------------------------------------
m4bool_t	ClM4CsvParser::isRowTerminatorChar( ClCsvCharBase *ai_pCsvChar )
{
	m4bool_t		bRet = M4_FALSE;
	ClCsvCharBase	*pCsvFollowingChar;


	if( *ai_pCsvChar == s_RowTerminatorChar_n )
	{
		bRet = M4_TRUE;
	}
	else if( *ai_pCsvChar == s_RowTerminatorChar_r )
	{
		switch( m_iEncoding )
		{
			case M4XL_M4CPP:
			case M4XL_ANSI:
			case M4XL_UTF8:
				pCsvFollowingChar = new ClCsvCharSingle();
				break;
			case M4XL_UTF16LE:
				pCsvFollowingChar = new ClCsvCharDoubleLE();
				break;
			case M4XL_UTF16BE:
				pCsvFollowingChar = new ClCsvCharDoubleBE();
				break;
		}

		if( m_pfCSVFile != NULL )
		{
			pCsvFollowingChar->Read( m_pfCSVFile );
			
			if( pCsvFollowingChar->IsEOF() )
			{
				m_bHasMoreRows = M4_FALSE;
			}
			else
			{
				fseek( m_pfCSVFile, -1 * pCsvFollowingChar->GetSize(), SEEK_CUR );
			}
		}
		else
		{
			pCsvFollowingChar->Read( m_pcCSVData, m_iReadIndex );
			
			if( *pCsvFollowingChar == 0 )
			{
				m_bHasMoreRows = M4_FALSE;
			}
		}

		if( *pCsvFollowingChar != s_RowTerminatorChar_n )
		{
			bRet = M4_TRUE;
		}


		delete pCsvFollowingChar;
	}


	return bRet;
}