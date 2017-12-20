//==============================================================================
//
// (c) Copyright  1991-2008 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4xlapi
// File:             m4xlhelper.cpp
// Project:			 mind3.x
// Author:           Meta Software M.S. , S.A
// Date:             20/01/2014 
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
//
//
//==============================================================================

#include <m4xlhelper.hpp>
#include <m4xlformatsmngr.hpp>
#include <m4xlapierrors.hpp>
#include <m4xlapichannel.hpp>
#include <access.hpp>
#include <m4unicode.hpp>
#include <m4mdrt.hpp>
#include <chan_knl.hpp>
#include <file_manager.hpp>
#include <wchar.h>
#include <sys/stat.h>

// -----------------------------------------------------------------------------
// Trace log
// -----------------------------------------------------------------------------
m4bool_t s_bTraceEnabled = M4_FALSE;

// -----------------------------------------------------------------------------
// Helper functions.
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Returns whether an Excel file belongs to an XML format.
// 
//	ai_pcFile:		Excel file path.
//	ao_bIsXML:		Output boolean.
// -----------------------------------------------------------------------------
m4return_t IsXMLWorkbook( const char* ai_pcFile, m4bool_t& ao_bIsXML )
{
	const char* pcExt;

	if( ai_pcFile == NULL )
	{
		return M4_ERROR;
	}

	pcExt = strrchr( ai_pcFile, '.' );

	if( pcExt == NULL )
	{
		return M4_ERROR;
	}
	
	if( strlen( pcExt ) == 5 )
	{
		ao_bIsXML = M4_TRUE;
	}
	else
	{
		ao_bIsXML = M4_FALSE;
	}


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Returns whether an Excel file is a template.
// 
//	ai_pcFile:		Excel file path.
//	ao_bIsTemplate:	Output boolean.
// -----------------------------------------------------------------------------
m4return_t	IsTemplateWorkbook( const char* ai_pcFile, m4bool_t& ao_bIsTemplate )
{
	const char* pcExt;

	if( ai_pcFile == NULL )
	{
		return M4_ERROR;
	}

	pcExt = strrchr( ai_pcFile, '.' );

	if( pcExt == NULL )
	{
		return M4_ERROR;
	}

	// File gets corrupt with this: !strcmpi( pcExt, ".xltm" )
	if( !strcmpi( pcExt, ".xltx" ) || !strcmpi( pcExt, ".xlt" ) )
	{
		ao_bIsTemplate = M4_TRUE;
	}
	else
	{
		ao_bIsTemplate = M4_FALSE;
	}


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Trims a string left and right.
//
//	ao_pcValue: Input/output string.
// -----------------------------------------------------------------------------
m4return_t TrimValue( char*& ao_pcValue )
{
	m4bool_t bTrimmed = M4_FALSE;

	if( ao_pcValue == NULL )
	{
		return M4_ERROR;
	}

	// Trims right.
	int	iIdx = strlen( ao_pcValue ) - 1;
	while( ao_pcValue[ iIdx ] == 32 )
	{
		ao_pcValue[ iIdx-- ] = '\0';
		bTrimmed = M4_TRUE;
	}

	// Trims left.
	char* pcData = ao_pcValue;
	while( *pcData == 32 )
	{
		pcData++;
		bTrimmed = M4_TRUE;
	}

	// Sets the trimmed value.
	if( bTrimmed == M4_TRUE )
	{
		char* pcItemData = new char[ strlen( pcData ) + 1 ];
		strcpy( pcItemData, pcData );

		delete ao_pcValue;
		ao_pcValue = pcItemData;
	}

	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Returns an upper case copy of a string.
//
//	ai_pwcString:	Input string.
//
//	Returns: Upper case copy of the string.
// -----------------------------------------------------------------------------
wchar_t* DuplicateInUpperCase( const wchar_t *ai_pwcString )
{
	size_t		iLen;
	wchar_t*	pwcOut;
	

	iLen = ( wcslen( ai_pwcString ) + 1 );
	pwcOut = new wchar_t[ iLen + 1 ];

	for( size_t i = 0; i < iLen; i++ )
	{
		pwcOut[ i ] = towupper( ai_pwcString [ i ] );
	}

	pwcOut[ iLen ] = 0;
    
    return( pwcOut );
}


// -----------------------------------------------------------------------------
// Composes a wide char string, given a printf-like format and parameters.
//
//	ao_pwcString:	Output buffer.
//	ai_iLen:		Buffer size.
//	ai_pwcFormat:	String format.
//	...:			Format string parameter values.
// -----------------------------------------------------------------------------
m4return_t ComposeString( wchar_t* ao_pwcString, size_t ai_iLen, wchar_t* ai_pwcFormat, ... )
{
	va_list	list;


	if( ao_pwcString == NULL )
	{
		return M4_ERROR;
	}

	va_start( list, ai_pwcFormat );

#ifdef UNIX
	vswprintf( ao_pwcString, ai_iLen, ai_pwcFormat, list );
#else
	vswprintf( ao_pwcString, ai_pwcFormat, list );
#endif

	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Splits all the time components into single elements.
// 
//	ai_pwcTime:		String with the time.
//	ao_iHH:			Hours in the time string.
//	ao_imm:			Minutes in the time string.
//	ao_iSS:			Seconds in the time string.
// -----------------------------------------------------------------------------
m4return_t	GetTimeNumbers( wchar_t* ai_pwcTime, int& ao_iHH, int& ao_imm, int& ao_iSS )
{
	size_t	iLen;
	int		iAssigned;


	ao_iHH = ao_imm = ao_iSS = 0;

	if( ai_pwcTime == NULL )
	{
		return M4_ERROR;
	}

	iLen = wcslen( ai_pwcTime );

	// Time
	if( iLen == 8 )
	{
		iAssigned = swscanf( ai_pwcTime, L"%2d:%2d:%2d", &ao_iHH, &ao_imm, &ao_iSS );

		if( iAssigned == 3 )
		{
			return M4_SUCCESS;
		}
	}

	
	return M4_ERROR;
}


// -----------------------------------------------------------------------------
// Splits all the date components into single elements.
// 
//	ai_pwcDate:		String with the date.
//	ao_iYY:			Year in the date string.
//	ao_iMM:			Month in the date string.
//	ao_iDD:			Day in the date string.
//	ao_iHH:			Hours in the date string.
//	ao_imm:			Minutes in the date string.
//	ao_iSS:			Seconds in the date string.
// -----------------------------------------------------------------------------
m4return_t	GetDateNumbers( wchar_t* ai_pwcDate, int& ao_iYY, int& ao_iMM, int& ao_iDD, int& ao_iHH, int& ao_imm, int& ao_iSS )
{
	size_t	iLen;
	int		iAssigned;


	ao_iYY = ao_iMM = ao_iDD = ao_iHH = ao_imm = ao_iSS = 0;

	if( ai_pwcDate == NULL )
	{
		return M4_ERROR;
	}

	iLen = wcslen( ai_pwcDate );

	// Date-time.
	if( iLen == 19 )
	{
		iAssigned = swscanf( ai_pwcDate, L"%4d-%2d-%2d %2d:%2d:%2d", &ao_iYY, &ao_iMM, &ao_iDD, &ao_iHH, &ao_imm, &ao_iSS );
		if( iAssigned == 6 )
		{
			return M4_SUCCESS;
		}
	}

	// Date.
	if( iLen == 10 )
	{
		iAssigned = swscanf( ai_pwcDate, L"%4d-%2d-%2d", &ao_iYY, &ao_iMM, &ao_iDD );
		if( iAssigned == 3 )
		{
			return M4_SUCCESS;
		}
	}


	return M4_ERROR;
}


// -----------------------------------------------------------------------------
// Transforms Meta4 cpp strings to libxl encoded strings.
// 
//	ai_pcInString:	Meta4 cpp encoded string (ANSI/UTF8).
//	ao_pcString:	Libxl encoded string (UTF16/UTF32).
// -----------------------------------------------------------------------------
m4return_t	CppStrToLibxlStr( const char* ai_pcInString, wchar_t*& ao_pwcOutString )
{
	m4return_t	iRet;
	int			iLength = -1;
	M4XMLCh*	pcUTF16Str;
	m4bool_t	bNew;

	
	ao_pwcOutString = NULL;

	if( ai_pcInString == NULL )
	{
		return M4_ERROR;
	}


	pcUTF16Str = M4CppToUtf16( ai_pcInString, iLength );

	if( iLength < 0 )
	{
		return M4_ERROR;
	}

	// Gets the platform specific wide char string.
	iRet = UTF16StrToNativeWCStr( pcUTF16Str, iLength, ao_pwcOutString, bNew );

	if( iRet == M4_SUCCESS && bNew == M4_TRUE )
	{
		delete [] pcUTF16Str;
	}


	return iRet;
}


// -----------------------------------------------------------------------------
// Transforms libxl encoded strings to Meta4 cpp strings.
// 
//	ai_pwcInString:	Libxl encoded string (UTF16/UTF32).
//	ao_pcOutString:	Meta4 cpp encoded string (ANSI/UTF8).
// -----------------------------------------------------------------------------
m4return_t	LibxlStrToCppStr( const wchar_t* ai_pwcInString, char*& ao_pcOutString )
{
	m4return_t		iRet;
	M4XMLCh*		pcUnpaddedString;
	int				iLength;
	m4bool_t		bNew;


	ao_pcOutString = NULL;

	if( ai_pwcInString == NULL )
	{
		return M4_ERROR;
	}

	// Must unpad leading zeroes.
	iRet = NativeWCStrToUTF16Str( ai_pwcInString, pcUnpaddedString, bNew );

	if( iRet == M4_ERROR )
	{
		return M4_ERROR;
	}

	iLength = -1;
	ao_pcOutString = M4Utf16ToCpp( pcUnpaddedString, iLength );

	if( bNew == M4_TRUE )
	{
		delete [] pcUnpaddedString;
	}


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Transforms UTF16 strings to platform specific wide char strings (padding).
// 
//	ai_pcInString:		Input UTF16 string. 
//	ai_iLength:			Input string character length.
//	ao_pwcOutString:	Output native wide char string
//	ao_bIsNew:			Output boolean. Indicates whether or not the output string
//						has been allocated in memory.
// -----------------------------------------------------------------------------
m4return_t	UTF16StrToNativeWCStr( const unsigned short* ai_pcInString, int ai_iLength, wchar_t*& ao_pwcOutString, m4bool_t& ao_bIsNew )
{
	ao_pwcOutString	= NULL;
	ao_bIsNew		= M4_FALSE;


	if( ai_iLength < 0 )
	{
		return M4_ERROR;
	}


	if( sizeof( wchar_t ) == 4 )
	{
		ao_pwcOutString = new wchar_t[ ai_iLength + 1 ];
	
		for( int i = 0; i <= ai_iLength; i++ )
		{
			*( ao_pwcOutString + i ) = *( (( unsigned short* )( ai_pcInString )) + i );
		}

		ao_bIsNew = M4_TRUE;
	}
	else
	{
		ao_pwcOutString = ( wchar_t* )ai_pcInString;
	}


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Transforms platform specific wide char strings to UTF16 strings (unpadding).
// 
//	ai_pwcInString	Input platform wide char string
//	ao_pcOutString	Output UTF16 string. 
//	ao_bIsNew:		Output boolean. Indicates whether or not the output string
//					has been allocated in memory.
// -----------------------------------------------------------------------------
m4return_t	NativeWCStrToUTF16Str( const wchar_t* ai_pwcInString, unsigned short*& ao_pcOutString, m4bool_t& ao_bIsNew )
{
	size_t	iLength;


	if( ai_pwcInString == NULL )
	{
		return M4_ERROR;
	}


	if( sizeof( wchar_t ) == 4 )
	{
		iLength = wcslen( ai_pwcInString );

		ao_pcOutString = new unsigned short[ iLength + 1 ];
	
		for( int i = 0; i <= iLength; i++ )
		{
			*( ao_pcOutString + i ) = *( ai_pwcInString + i );
		}

		ao_bIsNew = M4_TRUE;
	}
	else
	{
		ao_pcOutString = (unsigned short*)ai_pwcInString;

		ao_bIsNew = M4_FALSE;
	}

	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Changes the endian swaping the bytes in a UTF16 string.
//
//	ai_pcString:	Input UTF16 string.
//	ai_iLength:		Input string character length.
// -----------------------------------------------------------------------------
m4return_t SwapBytesUTF16String( unsigned short* ai_pcString, int ai_iLength )
{
	unsigned short	iChar;
	char			cLByte;
	char*			pcWord;


	for( int i = 0; i <= ai_iLength; i++ )
	{
		pcWord			= ( char* )&ai_pcString[ i ];
		cLByte			= *pcWord;
		*pcWord			= *( pcWord + 1 );
		*( pcWord + 1 )	= cLByte;
	}

	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Converts a string in any encoding type to a native wide char string.
//
//	ai_pcInString:		Input string.
//	ai_iLength:			Input string length in bytes.
//	ai_iEncoding:		Input string encoding.
//	ao_pwcOutString:	Output wide char string.
// -----------------------------------------------------------------------------
m4return_t	StrToNativeWCStr( const char* ai_pcInString, int ai_iLength, eXLEncodings_t ai_iEncoding, wchar_t*& ao_pwcOutString )
{
	m4return_t	iRet;
	char*		pcCppStr;
	int			iLength;
	M4XMLCh*	pcUTF16Str;
	m4bool_t	bNew;


	ao_pwcOutString	= NULL;

	if( ai_pcInString == NULL || ai_iLength == 0 )
	{
		return M4_SUCCESS;
	}


	// Single and multibyte encodings ...
	if( ai_iEncoding <= M4XL_UTF8 )
	{
		m4bool_t	bInternalDelete = M4_FALSE;

		// .. are converted to a base common encoding (cpp string ) ...
		if( ai_iEncoding == M4XL_UTF8 )
		{
			pcCppStr = M4Utf8ToCpp( ai_pcInString, ai_iLength );
			bInternalDelete  = M4_TRUE;
		}
		else if( ai_iEncoding == M4XL_ANSI )
		{
			pcCppStr = M4ANSIToCpp( ai_pcInString, ai_iLength );
			bInternalDelete  = M4_TRUE;
		}
		else if( ai_iEncoding == M4XL_M4CPP )
		{
			pcCppStr = ( char* )ai_pcInString;
		}

		// ... then to UTF16 ...
		iLength = -1;
		pcUTF16Str = M4CppToUtf16( pcCppStr, iLength );

		if( bInternalDelete == M4_TRUE )
		{
			delete [] pcCppStr;
		}

		// ... and finally to native wide char string.
		iRet = UTF16StrToNativeWCStr( pcUTF16Str, iLength, ao_pwcOutString, bNew );

		if( iRet == M4_SUCCESS && bNew == M4_TRUE )
		{
			delete [] pcUTF16Str;
		}
	}
	else
	{
		// Double byte encondings ...

		// Input string is copied into a new memory block. The caller always must delete the output pointer.
		pcUTF16Str = ( unsigned short* )new char[ ai_iLength + 2 ];
		memcpy( pcUTF16Str, ai_pcInString , ai_iLength );
		( ( char* )pcUTF16Str )[ ai_iLength ] = ( ( char* )pcUTF16Str )[ ai_iLength + 1 ] = 0;

		// if the endian does not match the platform endian, each double byte in the string is swaped.
#ifdef UNIX
		if( ai_iEncoding == M4XL_UTF16LE )
		{
			SwapBytesUTF16String( pcUTF16Str, ai_iLength / 2 );
		}
#else
		if( ai_iEncoding == M4XL_UTF16BE )
		{
			SwapBytesUTF16String( pcUTF16Str, ai_iLength / 2 );
		}
#endif
		
		// Finally converts to native wide char string.
		iRet = UTF16StrToNativeWCStr( pcUTF16Str, ai_iLength / 2, ao_pwcOutString, bNew );

		if( iRet == M4_SUCCESS && bNew == M4_TRUE )
		{
			delete [] pcUTF16Str;
		}
	}


	return iRet;
}


// -----------------------------------------------------------------------------
// Removes a character by position in a wide char string.
//
//	ai_pwcString:	Input wide char string.
//	ai_iPos:		Position to remove.
// -----------------------------------------------------------------------------
m4return_t	RemoveCharAt( wchar_t* ai_pwcString, int ai_iPos )
{
	size_t	iLen;


	if( ai_pwcString == NULL )
	{
		return M4_ERROR;
	}


	iLen = wcslen( ai_pwcString );
	if( ai_iPos >= iLen )
	{
		return	M4_ERROR;
	}

	for( int i = ai_iPos; i < iLen; i ++ )
	{
		ai_pwcString[ i ] = ai_pwcString[ i + 1 ];
	}


	return M4_SUCCESS;

}


// -----------------------------------------------------------------------------
// Helper function for iteration backward/forward 
// -----------------------------------------------------------------------------
inline bool	loopCondition( bool ai_bShiftForward, int ai_iIndex, int ai_iLowerLimit, int ai_iUpperLimit )
{
	if( ai_bShiftForward == true )
	{
		return ( ai_iIndex >= ai_iLowerLimit );
	}
	else
	{
		return ( ai_iIndex <= ai_iUpperLimit );
	}
}


// -----------------------------------------------------------------------------
// Copies the contents of a range, with its format, in another area to the bottom.
//
//	ai_pWorkbook:		Workbook reference.
//	ai_pWorksheet:		Worksheet reference.
//	ai_iSrcStartRow:	Source range start row.
//	ai_iSrcStartCol:	Source range start column.
//	ai_iSrcEndRow:		Source range end row.
//	ai_iSrcEndCol:		Source range end column.
//	ai_iTgtStartRow:	Target range start row.
//	ai_iTgtStartCol:	Target range start column.
// -----------------------------------------------------------------------------
m4return_t CopyRangeDown( Book* ai_pWorkbook, Sheet* ai_pWorksheet, int ai_iSrcStartRow, int ai_iSrcStartCol, int ai_iSrcEndRow, int ai_iSrcEndCol, int ai_iTgtStartRow, int ai_iTgtStartCol )
{
	bool			bRet;
	bool			bShiftDown;
	CellType		iCellType;
	Format			*pFormat;
	bool			bMerged;
	int				iSrcRow, iSrcCol, iTgtRow, iTgtCol;
	int				iMergedRowFirst, iMergedRowLast, iMergedColFirst, iMergedColLast;
	int				iMergedBlkNumRows, iMergedBlkNumCols;
	int				iMergedBlkTgtRowFirst, iMergedBlkTgtRowLast;
	ClCellPos		oCellPos;
	CellPosSet_t	setMergedBlocks;
	bool			bInserted;
	int				iMRF, iMRL, iMCF, iMCL;
	bool			bDoMerge;
	bool			bIsFormula;
	const wchar_t*	pwcFormula;
	

	bShiftDown = ( ai_iSrcStartRow < ai_iTgtStartRow );


	for( int iRow = ( bShiftDown ) ? ai_iSrcEndRow : ai_iSrcStartRow; loopCondition( bShiftDown, iRow, ai_iSrcStartRow, ai_iSrcEndRow ); ( bShiftDown ) ? iRow-- : iRow++ )
	{
		for( int iCol = ai_iSrcEndCol; iCol >= ai_iSrcStartCol; iCol-- )
		{
			// Default source and target cells.
			iSrcRow = iRow;
			iSrcCol = iCol;
			iTgtRow = ai_iTgtStartRow + ( iRow - ai_iSrcStartRow );
			iTgtCol = ai_iTgtStartCol + ( iCol - ai_iSrcStartCol );

			bMerged = ai_pWorksheet->getMerge( iSrcRow, iSrcCol, &iMergedRowFirst, &iMergedRowLast, &iMergedColFirst, &iMergedColLast );

			if( bMerged == true )
			{
				oCellPos.Init( iMergedRowFirst, iMergedColFirst );
				bInserted = (setMergedBlocks.insert( oCellPos )).second;

				// Process the merged block only once.
				if( bInserted == false )
				{
					continue;
				}

				// Merged block sizes.
				iMergedBlkNumRows = iMergedRowLast - iMergedRowFirst;
				iMergedBlkNumCols = iMergedColLast - iMergedColFirst;

				// Initial cells of the merged block.
				if( bShiftDown == true )
				{
					iMergedBlkTgtRowFirst	= iTgtRow - iMergedBlkNumRows;
					iMergedBlkTgtRowLast	= iTgtRow;
				}
				else
				{
					iMergedBlkTgtRowFirst	= iTgtRow;
					iMergedBlkTgtRowLast	= iTgtRow + iMergedBlkNumRows;
				}

				bDoMerge = true;

				// Before merge nothing it unmerges previous cell grouping. If not it will corrupt the output file.
				bMerged = ai_pWorksheet->getMerge( iMergedBlkTgtRowFirst, iTgtCol - iMergedBlkNumCols, &iMRF, &iMRL, &iMCF, &iMCL );

				if( bMerged == true )
				{
					if( iMRF != iMergedBlkTgtRowFirst || iMRL != iMergedBlkTgtRowLast || iMCF != ( iTgtCol - iMergedBlkNumCols ) || iMCL != iTgtCol )
					{
						bMerged = ai_pWorksheet->delMerge( iMergedBlkTgtRowFirst, iTgtCol - iMergedBlkNumCols );
						
						if( bMerged == false )
						{
							SETCODEF( M4_XL_ERR_UNMERGE_CELLS_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#%d#", ai_pWorkbook->errorMessage(), iMergedBlkTgtRowFirst, iTgtCol - iMergedBlkNumCols );
						}
					}
					else
					{
						bDoMerge = false;
					}
				}

				if( bDoMerge == true )
				{
					// Merges the target block.
					bMerged = ai_pWorksheet->setMerge( iMergedBlkTgtRowFirst, iMergedBlkTgtRowLast, iTgtCol - iMergedBlkNumCols, iTgtCol );

					if( bMerged == false )
					{
						SETCODEF( M4_XL_ERR_MERGE_CELLS_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#%d#%d#%d#", ai_pWorkbook->errorMessage(), iMergedBlkTgtRowFirst, iMergedBlkTgtRowLast, iTgtCol - iMergedBlkNumCols, iTgtCol );
					}
				}

				// Copies only the initial cell of the merged block.
				iSrcRow = iMergedRowFirst;
				iSrcCol = iMergedColFirst;
				iTgtRow = iMergedBlkTgtRowFirst;
				iTgtCol = iTgtCol - iMergedBlkNumCols;

				// Clone the borders of the cells in the merged block (since copyCell will not do it).
				CopyFormatDown( ai_pWorkbook, ai_pWorksheet, iMergedRowFirst, iMergedColFirst, iMergedRowLast, iMergedColLast, iTgtRow, iTgtCol );
			}
			else
			{
				// Unmerge blocks.
				bMerged = ai_pWorksheet->getMerge( iTgtRow, iTgtCol, &iMergedRowFirst, &iMergedRowLast, &iMergedColFirst, &iMergedColLast );

				if( bMerged == true )
				{
					bMerged = ai_pWorksheet->delMerge( iTgtRow, iTgtCol );
					
					if( bMerged == false )
					{
						SETCODEF( M4_XL_ERR_UNMERGE_CELLS_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#%d#", ai_pWorkbook->errorMessage(), iTgtRow, iTgtCol );
					}
				}
			}


			iCellType = ai_pWorksheet->cellType( iSrcRow, iSrcCol );

			if( iCellType == CELLTYPE_EMPTY )
			{
				pFormat = ai_pWorksheet->cellFormat( iSrcRow, iSrcCol );
				ai_pWorksheet->writeBlank( iTgtRow, iTgtCol, pFormat );
				continue;
			}

			bIsFormula = ai_pWorksheet->isFormula( iSrcRow, iSrcCol );

			if( bIsFormula == true )
			{
				pwcFormula = ai_pWorksheet->readFormula( iSrcRow, iSrcCol, &pFormat );

				if( pwcFormula != NULL )
				{
					bRet = ai_pWorksheet->writeFormula( iTgtRow, iTgtCol, pwcFormula, pFormat );
					if( bRet == false )
					{
						SETCODEF( M4_XL_ERR_SET_FORMULA_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#%d#", ai_pWorkbook->errorMessage(), iTgtRow, iTgtCol );
					}
					continue;
				}
			}

			bRet = ai_pWorksheet->copyCell( iSrcRow, iSrcCol, iTgtRow, iTgtCol );

			if( bRet == false )
			{
				SETCODEF( M4_XL_ERR_COPY_CELL_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#%d#%d#%d#", ai_pWorkbook->errorMessage(), iSrcRow, iSrcCol, iTgtRow, iTgtCol );
				return M4_ERROR;
			}
		}
	}


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Copies the contents of a range, with its format, in another area to the right.
//
//	ai_pWorkbook:		Workbook reference.
//	ai_pWorksheet:		Worksheet reference.
//	ai_iSrcStartRow:	Source range start row.
//	ai_iSrcStartCol:	Source range start column.
//	ai_iSrcEndRow:		Source range end row.
//	ai_iSrcEndCol:		Source range end column.
//	ai_iTgtStartRow:	Target range start row.
//	ai_iTgtStartCol:	Target range start column.
// -----------------------------------------------------------------------------
m4return_t CopyRangeRight( Book* ai_pWorkbook, Sheet* ai_pWorksheet, int ai_iSrcStartRow, int ai_iSrcStartCol, int ai_iSrcEndRow, int ai_iSrcEndCol, int ai_iTgtStartRow, int ai_iTgtStartCol )
{
	bool			bRet;
	bool			bShiftRight;
	CellType		iCellType;
	Format			*pFormat;
	bool			bMerged;
	int				iSrcRow, iSrcCol, iTgtRow, iTgtCol;
	int				iMergedRowFirst, iMergedRowLast, iMergedColFirst, iMergedColLast;
	int				iMergedBlkNumRows, iMergedBlkNumCols;
	int				iMergedBlkTgtColFirst, iMergedBlkTgtColLast;
	ClCellPos		oCellPos;
	CellPosSet_t	setMergedBlocks;
	bool			bInserted;
	int				iMRF, iMRL, iMCF, iMCL;
	bool			bDoMerge;
	bool			bIsFormula;
	const wchar_t*	pwcFormula;

	
	bShiftRight = ( ai_iSrcStartCol < ai_iTgtStartCol );


	for( int iCol = ( bShiftRight ) ? ai_iSrcEndCol : ai_iSrcStartCol; loopCondition( bShiftRight, iCol, ai_iSrcStartCol, ai_iSrcEndCol ); ( bShiftRight ) ? iCol-- : iCol++ )
	{
		for( int iRow = ai_iSrcEndRow; iRow >= ai_iSrcStartRow; iRow-- )
		{
			// Default source and target cells.
			iSrcRow = iRow;
			iSrcCol = iCol;
			iTgtRow = ai_iTgtStartRow + ( iRow - ai_iSrcStartRow );
			iTgtCol = ai_iTgtStartCol + ( iCol - ai_iSrcStartCol );

			bMerged = ai_pWorksheet->getMerge( iSrcRow, iSrcCol, &iMergedRowFirst, &iMergedRowLast, &iMergedColFirst, &iMergedColLast );

			if( bMerged == true )
			{
				oCellPos.Init( iMergedRowFirst, iMergedColFirst );
				bInserted = (setMergedBlocks.insert( oCellPos )).second;

				// Process the merged block only once.
				if( bInserted == false )
				{
					continue;
				}

				// Merged block sizes.
				iMergedBlkNumRows = iMergedRowLast - iMergedRowFirst;
				iMergedBlkNumCols = iMergedColLast - iMergedColFirst;

				// Initial cells of the merged block.
				if( bShiftRight == true )
				{
					iMergedBlkTgtColFirst	= iTgtCol - iMergedBlkNumCols;
					iMergedBlkTgtColLast	= iTgtCol;
				}
				else
				{
					iMergedBlkTgtColFirst	= iTgtCol;
					iMergedBlkTgtColLast	= iTgtCol + iMergedBlkNumCols;
				}

				bDoMerge = true;

				// Before merge nothing it unmerges previous cell grouping. If not it will corrupt the output file.
				bMerged = ai_pWorksheet->getMerge( iTgtRow - iMergedBlkNumRows,  iMergedBlkTgtColFirst, &iMRF, &iMRL, &iMCF, &iMCL );

				if( bMerged == true )
				{
					if( iMRF != ( iTgtRow - iMergedBlkNumRows ) || iMRL != iTgtRow || iMCF != iMergedBlkTgtColFirst || iMCL != iMergedBlkTgtColLast )
					{
						bMerged = ai_pWorksheet->delMerge( iTgtRow - iMergedBlkNumRows,  iMergedBlkTgtColFirst );
						
						if( bMerged == false )
						{
							SETCODEF( M4_XL_ERR_UNMERGE_CELLS_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#%d#", ai_pWorkbook->errorMessage(), iTgtRow - iMergedBlkNumRows,  iMergedBlkTgtColFirst );
						}
					}
					else
					{
						bDoMerge = false;
					}
				}
				
				if( bDoMerge == true )
				{
					// Merges the target block.
					bMerged = ai_pWorksheet->setMerge( iTgtRow - iMergedBlkNumRows, iTgtRow, iMergedBlkTgtColFirst, iMergedBlkTgtColLast );

					if( bMerged == false )
					{
						SETCODEF( M4_XL_ERR_MERGE_CELLS_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#%d#%d#%d#", ai_pWorkbook->errorMessage(), iTgtRow - iMergedBlkNumRows, iTgtRow, iMergedBlkTgtColFirst, iMergedBlkTgtColLast );
					}
				}

				// Copies only the initial cell of the merged block.
				iSrcRow = iMergedRowFirst;
				iSrcCol = iMergedColFirst;
				iTgtRow = iTgtRow - iMergedBlkNumRows;
				iTgtCol = iMergedBlkTgtColFirst;

				// Clone the borders of the cells in the merged block (since copyCell will not do it).
				CopyFormatRight( ai_pWorkbook, ai_pWorksheet, iMergedRowFirst, iMergedColFirst, iMergedRowLast, iMergedColLast, iTgtRow, iTgtCol );
			}
			else
			{
				// Unmerge blocks.
				bMerged = ai_pWorksheet->getMerge( iTgtRow, iTgtCol, &iMergedRowFirst, &iMergedRowLast, &iMergedColFirst, &iMergedColLast );

				if( bMerged == true )
				{
					bMerged = ai_pWorksheet->delMerge( iTgtRow, iTgtCol );
					
					if( bMerged == false )
					{
						SETCODEF( M4_XL_ERR_UNMERGE_CELLS_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#%d#", ai_pWorkbook->errorMessage(), iTgtRow, iTgtCol );
					}
				}
			}


			iCellType = ai_pWorksheet->cellType( iSrcRow, iSrcCol );

			if( iCellType == CELLTYPE_EMPTY )
			{
				pFormat = ai_pWorksheet->cellFormat( iSrcRow, iSrcCol );
				ai_pWorksheet->writeBlank( iTgtRow, iTgtCol, pFormat );
				continue;
			}

			bIsFormula = ai_pWorksheet->isFormula( iSrcRow, iSrcCol );

			if( bIsFormula == true )
			{
				pwcFormula = ai_pWorksheet->readFormula( iSrcRow, iSrcCol, &pFormat );

				if( pwcFormula != NULL )
				{
					bRet = ai_pWorksheet->writeFormula( iTgtRow, iTgtCol, pwcFormula, pFormat );
					if( bRet == false )
					{
						SETCODEF( M4_XL_ERR_SET_FORMULA_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#%d#", ai_pWorkbook->errorMessage(), iTgtRow, iTgtCol );
					}
					continue;
				}
			}

			bRet = ai_pWorksheet->copyCell( iSrcRow, iSrcCol, iTgtRow, iTgtCol );

			if( bRet == false )
			{
				SETCODEF( M4_XL_ERR_COPY_CELL_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#%d#%d#%d#", ai_pWorkbook->errorMessage(), iSrcRow, iSrcCol, iTgtRow, iTgtCol );
				return M4_ERROR;
			}
		}
	}


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Sets format to the bottom in the cells of a range.
//
//	ai_pWorkbook:		Workbook reference.
//	ai_pWorksheet:		Worksheet reference.
//	ai_iTgtStartRow:	Range start row.
//	ai_iTgtStartCol:	Range start column.
//	ai_iTgtEndRow:		Range end row.
//	ai_iTgtEndCol:		Range end column.
//	ai_bPrevFormat:		Apply the format of the previous row.
//	ai_bClearContent:	Clear the contents of the cells in the range.
//	ai_bRemoveBorder:	Remove de borders of the cells in the range.
//	ai_pAccess:			M4Object API access.
// -----------------------------------------------------------------------------
m4return_t FillFormatDown( Book* ai_pWorkbook, Sheet* ai_pWorksheet, int ai_iTgtStartRow, int ai_iTgtStartCol, int ai_iTgtEndRow, int ai_iTgtEndCol, bool ai_bPrevFormat, bool ai_bClearContent, bool ai_bRemoveBorder, ClAccess* ai_pAccess )
{
	m4return_t		iRet;
	Format			*pFormat, *pNewFormat;
	m4VariantType	vChangeArgs[ 3 ];
	bool			bMerged;
	int				iMergedRowFirst, iMergedRowLast, iMergedColFirst, iMergedColLast;


	if( ai_bPrevFormat == false )
	{
		pFormat = ai_pWorkbook->addFormat();

		if( pFormat == NULL )
		{
			return M4_ERROR;
		}
	}

	for( int iCol = ai_iTgtStartCol; iCol <= ai_iTgtEndCol; iCol++ )
	{
		if( ai_bPrevFormat == true )
		{
			pFormat = ai_pWorksheet->cellFormat( ai_iTgtStartRow - 1, iCol );
		}

		if( ai_bRemoveBorder == true )
		{
			// Gets a new format for the cell or an already created one.
			vChangeArgs[ 0 ] = (const m4double_t)BORDERSTYLE_NONE;
			vChangeArgs[ 1 ] = (const m4double_t)( ClXlFormatsMngr::M4XL_EDGE_TOP | ClXlFormatsMngr::M4XL_EDGE_RIGHT | ClXlFormatsMngr::M4XL_EDGE_BOTTOM | ClXlFormatsMngr::M4XL_EDGE_LEFT );
			vChangeArgs[ 2 ] = (const m4double_t)false;

			iRet = ClXlFormatsMngr::GetSuitableFormat( pFormat, ClXlFormatsMngr::M4XL_BORDER_STYLE, vChangeArgs, ai_pWorkbook, ai_pAccess, pNewFormat );

			if( iRet == M4_ERROR || pNewFormat == NULL )
			{
				SETCODEF( M4_XL_ERR_INSERT_RANGE_REMOVE_BORDER_ERR, ERRORLOG, "#*s1*#" );
				return M4_ERROR;
			}

			pFormat = pNewFormat;
		}

		for( int iRow = ai_iTgtStartRow; iRow <= ai_iTgtEndRow; iRow++ )		
		{
			// Unmerge blocks.
			bMerged = ai_pWorksheet->getMerge( iRow, iCol, &iMergedRowFirst, &iMergedRowLast, &iMergedColFirst, &iMergedColLast );

			if( bMerged == true )
			{
				bMerged = ai_pWorksheet->delMerge( iRow, iCol );
				
				if( bMerged == false )
				{
					SETCODEF( M4_XL_ERR_UNMERGE_CELLS_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#%d#", ai_pWorkbook->errorMessage(), iRow, iCol );
				}
			}

			if( ai_bClearContent == true )
			{
				ai_pWorksheet->writeBlank( iRow, iCol, pFormat );
			}
			else
			{
				ai_pWorksheet->setCellFormat( iRow, iCol, pFormat );
			}
		}
	}


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Sets format to the right in the cells of a range.
//
//	ai_pWorkbook:		Workbook reference.
//	ai_pWorksheet:		Worksheet reference.
//	ai_iTgtStartRow:	Range start row.
//	ai_iTgtStartCol:	Range start column.
//	ai_iTgtEndRow:		Range end row.
//	ai_iTgtEndCol:		Range end column.
//	ai_bPrevFormat:		Apply the format of the previous col.
//	ai_bClearContent:	Clear the contents of the cells in the range.
//	ai_bRemoveBorder:	Remove de borders of the cells in the range.
//	ai_pAccess:			M4Object API access.
// -----------------------------------------------------------------------------
m4return_t FillFormatRight( Book* ai_pWorkbook, Sheet* ai_pWorksheet, int ai_iTgtStartRow, int ai_iTgtStartCol, int ai_iTgtEndRow, int ai_iTgtEndCol, bool ai_bPrevFormat, bool ai_bClearContent, bool ai_bRemoveBorder, ClAccess* ai_pAccess )
{
	m4return_t		iRet;
	Format			*pFormat, *pNewFormat;
	m4VariantType	vChangeArgs[ 3 ];
	bool			bMerged;
	int				iMergedRowFirst, iMergedRowLast, iMergedColFirst, iMergedColLast;


	if( ai_bPrevFormat == false )
	{
		pFormat = ai_pWorkbook->addFormat();

		if( pFormat == NULL )
		{
			return M4_ERROR;
		}
	}

	for( int iRow = ai_iTgtStartRow; iRow <= ai_iTgtEndRow; iRow++ )
	{
		if( ai_bPrevFormat == true )
		{
			pFormat = ai_pWorksheet->cellFormat( iRow, ai_iTgtStartCol - 1 );
		}

		if( ai_bRemoveBorder == true )
		{
			// Gets a new format for the cell or an already created one.
			vChangeArgs[ 0 ] = (const m4double_t)BORDERSTYLE_NONE;
			vChangeArgs[ 1 ] = (const m4double_t)( ClXlFormatsMngr::M4XL_EDGE_TOP | ClXlFormatsMngr::M4XL_EDGE_RIGHT | ClXlFormatsMngr::M4XL_EDGE_BOTTOM | ClXlFormatsMngr::M4XL_EDGE_LEFT );
			vChangeArgs[ 2 ] = (const m4double_t)false;

			iRet = ClXlFormatsMngr::GetSuitableFormat( pFormat, ClXlFormatsMngr::M4XL_BORDER_STYLE, vChangeArgs, ai_pWorkbook, ai_pAccess, pNewFormat );

			if( iRet == M4_ERROR || pNewFormat == NULL )
			{
				SETCODEF( M4_XL_ERR_INSERT_RANGE_REMOVE_BORDER_ERR, ERRORLOG, "#*s1*#" );
				return M4_ERROR;
			}

			pFormat = pNewFormat;
		}

		for( int iCol = ai_iTgtStartCol; iCol <= ai_iTgtEndCol; iCol++ )		
		{
			// Unmerge blocks.
			bMerged = ai_pWorksheet->getMerge( iRow, iCol, &iMergedRowFirst, &iMergedRowLast, &iMergedColFirst, &iMergedColLast );

			if( bMerged == true )
			{
				bMerged = ai_pWorksheet->delMerge( iRow, iCol );
				
				if( bMerged == false )
				{
					SETCODEF( M4_XL_ERR_UNMERGE_CELLS_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#%d#", ai_pWorkbook->errorMessage(), iRow, iCol );
				}
			}

			if( ai_bClearContent == true )
			{
				ai_pWorksheet->writeBlank( iRow, iCol, pFormat );
			}
			else
			{
				ai_pWorksheet->setCellFormat( iRow, iCol, pFormat );
			}
		}
	}


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Removes invalid chars of a sheet name.
//
//	ai_pwcName:	Sheet name.
// -----------------------------------------------------------------------------
#define	SHEET_NAME_CHAR_REPLACER	L' '
#define	SHEET_NAME_MAX_LENGTH		31
m4return_t SanitizeSheetName( wchar_t* ai_pwcName )
{
	size_t			iLen;
	wchar_t*		pwcSubStr;
	static wchar_t*	awcInvalidChars[] = { L"\\", L"/", L"?", L"*", L"[", L"]" };


	if( ai_pwcName == NULL )
	{
		return M4_ERROR;
	}


	iLen = wcslen( ai_pwcName );

	if( iLen == 0 )
	{
		return M4_ERROR;
	}


	// Truncates it if the name exceeds the maximum length.
	if( iLen > SHEET_NAME_MAX_LENGTH )
	{
		ai_pwcName[ SHEET_NAME_MAX_LENGTH ] = NULL;
	}

	// Replaces all invalid characters.
	for( int i = 0; i < sizeof( awcInvalidChars ) / sizeof( awcInvalidChars[ 0 ] ); i++ )
	{
		while( ( pwcSubStr = wcsstr( ai_pwcName, awcInvalidChars[ i ] ) ) != NULL )
		{
			*pwcSubStr = SHEET_NAME_CHAR_REPLACER;
		}
	}


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Returns if it is the date for m4 time variant.
// Hides magic date for time type.
// -----------------------------------------------------------------------------
bool IsDateForM4Time( int ai_iYears, int ai_iMonths, int ai_iDays )
{
	if( ai_iYears == 1970 && ai_iMonths == 9 && ai_iDays == 25 )
	{
		return true;
	}

	return false;
}

// -----------------------------------------------------------------------------
// Sets the date for m4 time variant.
// Hides magic date for time type.
// -----------------------------------------------------------------------------
void SetDateForM4Time( int& ao_iYears, int& ao_iMonths, int& ao_iDays )
{
	ao_iYears	= 1970;
	ao_iMonths	= 9;
	ao_iDays	= 25;
}


// -----------------------------------------------------------------------------
// Clones the format of a block to another block moving vertically.
//
//	ai_pWorkbook:		Workbook reference.
//	ai_pWorksheet:		Worksheet reference.
//	ai_iSrcStartRow:	Source range start row.
//	ai_iSrcStartCol:	Source range start column.
//	ai_iSrcEndRow:		Source range end row.
//	ai_iSrcEndCol:		Source range end column.
//	ai_iTgtStartRow:	Target range start row.
//	ai_iTgtStartCol:	Target range start column.
// -----------------------------------------------------------------------------
m4return_t CopyFormatDown( Book* ai_pWorkbook, Sheet* ai_pWorksheet, int ai_iSrcStartRow, int ai_iSrcStartCol, int ai_iSrcEndRow, int ai_iSrcEndCol, int ai_iTgtStartRow, int ai_iTgtStartCol )
{
	bool		bShiftDown;
	Format		*pFormat;

	
	bShiftDown = ( ai_iSrcStartRow < ai_iTgtStartRow );

	for( int iRow = ( bShiftDown ) ? ai_iSrcEndRow : ai_iSrcStartRow; loopCondition( bShiftDown, iRow, ai_iSrcStartRow, ai_iSrcEndRow ); ( bShiftDown ) ? iRow-- : iRow++ )
	{
		for( int iCol = ai_iSrcEndCol; iCol >= ai_iSrcStartCol; iCol-- )
		{
			pFormat = ai_pWorksheet->cellFormat( iRow, iCol );
			ai_pWorksheet->setCellFormat( ai_iTgtStartRow + ( iRow - ai_iSrcStartRow ), ai_iTgtStartCol + ( iCol - ai_iSrcStartCol ), pFormat );
		}
	}


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Clones the format of a block to another block moving horizontally.
//
//	ai_pWorkbook:		Workbook reference.
//	ai_pWorksheet:		Worksheet reference.
//	ai_iSrcStartRow:	Source range start row.
//	ai_iSrcStartCol:	Source range start column.
//	ai_iSrcEndRow:		Source range end row.
//	ai_iSrcEndCol:		Source range end column.
//	ai_iTgtStartRow:	Target range start row.
//	ai_iTgtStartCol:	Target range start column.
// -----------------------------------------------------------------------------
m4return_t CopyFormatRight( Book* ai_pWorkbook, Sheet* ai_pWorksheet, int ai_iSrcStartRow, int ai_iSrcStartCol, int ai_iSrcEndRow, int ai_iSrcEndCol, int ai_iTgtStartRow, int ai_iTgtStartCol )
{
	bool		bShiftRight;
	Format		*pFormat;

	
	bShiftRight = ( ai_iSrcStartCol < ai_iTgtStartCol );

	for( int iCol = ( bShiftRight ) ? ai_iSrcEndCol : ai_iSrcStartCol; loopCondition( bShiftRight, iCol, ai_iSrcStartCol, ai_iSrcEndCol ); ( bShiftRight ) ? iCol-- : iCol++ )
	{
		for( int iRow = ai_iSrcEndRow; iRow >= ai_iSrcStartRow; iRow-- )
		{
			pFormat = ai_pWorksheet->cellFormat( iRow, iCol );
			ai_pWorksheet->setCellFormat( ai_iTgtStartRow + ( iRow - ai_iSrcStartRow ), ai_iTgtStartCol + ( iCol - ai_iSrcStartCol ), pFormat );
		}
	}


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Gets directory separator.
// -----------------------------------------------------------------------------
char* GetDirSeparator()
{
#ifdef _WINDOWS
	return "\\";
#else
	return "/";
#endif
}


// -----------------------------------------------------------------------------
// Gets traces directory.
//
//	ai_pAccess:	M4Object API access.
//	ao_sPath:	Output trace directory.
// -----------------------------------------------------------------------------
m4return_t GetTraceDir(ClAccess* ai_pAccess, string& ao_sPath)
{
	m4return_t		iRet;
	m4pchar_t		pcBaseDir;
	m4pchar_t		pcTraceKey;
	char*			pcTraceFile;
	int				iLength = -1;


	ao_sPath = "";

	iRet = APIChannel::GetItem(ai_pAccess, APIChannel::WORKBOOKS, APIChannel::TRACE_BASE_PATH, pcBaseDir);

	if (iRet == M4_ERROR)
	{
		return M4_ERROR;
	}

	iRet = APIChannel::GetItem(ai_pAccess, APIChannel::WORKBOOKS, APIChannel::TRACE_KEY, pcTraceKey);

	if (iRet == M4_ERROR)
	{
		return M4_ERROR;
	}

	ao_sPath = pcBaseDir;
	ao_sPath += GetDirSeparator();
	ao_sPath += "m4xltrace_";
	ao_sPath += pcTraceKey;

	pcTraceFile = M4CppToANSI(ao_sPath.c_str(), iLength);
	ao_sPath = pcTraceFile;

	delete pcTraceFile;


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Checks if trace is enabled and if so, it creates the traces directory.
//
//	ai_pAccess:	M4Object API access.
//
// Must be called from initial API methods and only from them (Open/create books).
// -----------------------------------------------------------------------------
m4return_t CheckTraceMode(ClAccess* ai_pAccess)
{
	m4return_t	iRet;
	m4pchar_t	pcBaseDir;
	string		sActivatorFile;
	char*		pcActivatorFile;
	int			iLength = -1;
	struct stat	stBuf;
	string		sTracePath;


	iRet = APIChannel::GetItem(ai_pAccess, APIChannel::WORKBOOKS, APIChannel::TRACE_BASE_PATH, pcBaseDir);

	if (iRet == M4_ERROR)
	{
		return M4_ERROR;
	}

	sActivatorFile = pcBaseDir;
	sActivatorFile += GetDirSeparator();
	sActivatorFile += "m4xlapi.trace.on";

	pcActivatorFile = M4CppToANSI(sActivatorFile.c_str(), iLength);

	s_bTraceEnabled = (stat(pcActivatorFile, &stBuf) == 0);

	delete pcActivatorFile;

	if (s_bTraceEnabled == M4_TRUE)
	{
		iRet = GetTraceDir(ai_pAccess, sTracePath);

		if (iRet == M4_SUCCESS)
		{
			if (stat(sTracePath.c_str(), &stBuf) != 0)
			{
				iRet = ClFileManager::Instance()->CreatePath(sTracePath.c_str());
			}
		}
	}

	if (iRet == M4_ERROR)
	{
		s_bTraceEnabled = M4_FALSE;
	}


	return iRet;
}


// -----------------------------------------------------------------------------
// Copies a file.
// -----------------------------------------------------------------------------
m4return_t CopySingleFile(const char* ai_pcSource, const char* ai_pcTarget)
{
	FILE	*pfSource, *pfTarget;
	char	acBuffer[BUFSIZ];
	size_t  iSize;

	pfSource = fopen(ai_pcSource, "rb");

	if (pfSource == NULL)
	{
		return M4_ERROR;
	}

	pfTarget = fopen(ai_pcTarget, "wb");

	if (pfTarget == NULL)
	{
		fclose(pfSource);
		return M4_ERROR;
	}

	while ((iSize = fread(acBuffer, sizeof(char), sizeof(acBuffer), pfSource)) > 0)
	{
		if (fwrite(acBuffer, sizeof(char), iSize, pfTarget) != iSize)
		{
			fclose(pfSource);
			fclose(pfTarget);
			return M4_ERROR;
		}
	}

	fclose(pfSource);
	fclose(pfTarget);


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Helper classes.
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Class ClObjTerminator
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Constructor
// 
//	ai_poAccess:	M4Object instance access.
// -----------------------------------------------------------------------------
ClObjTerminator::ClObjTerminator( ClAccess*	ai_poAccess )
{
	m_poAccess = ai_poAccess;
}


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
ClObjTerminator::~ClObjTerminator()
{
	if( m_poAccess != NULL )
	{
		m_poAccess->Destroy();
	}
}


// -----------------------------------------------------------------------------
// Class ClExcelColNames
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Constructor
// 
//	ai_iNumCols:	Number of columns to manage.
// -----------------------------------------------------------------------------
ClExcelColNames::ClExcelColNames( int ai_iNumCols ) : c_iXlsColumnWidth( 4 )
{
	n_iNumCols = ai_iNumCols;
	initialize();
}


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
ClExcelColNames::~ClExcelColNames()
{
	if( m_pcXlsColNames != NULL )
	{
		delete [] m_pcXlsColNames;
	}
}


// -----------------------------------------------------------------------------
// Returns the name of the column for a given index.
//
//	ai_iNumCol: Column index.
// -----------------------------------------------------------------------------
char* ClExcelColNames::GetColName( int ai_iNumCol )
{
	if( m_pcXlsColNames == NULL )
	{
		return NULL;
	}

	if( ai_iNumCol < n_iNumCols )
	{
		return m_pcXlsColNames + ( ai_iNumCol * c_iXlsColumnWidth );
	}

	return NULL;
}


// -----------------------------------------------------------------------------
// Builds all the required column names.
// -----------------------------------------------------------------------------
m4return_t ClExcelColNames::initialize()
{
	char*	pcColumn = NULL;
	int		iRange1c, iRange2c;

	m_pcXlsColNames = new char[ n_iNumCols * c_iXlsColumnWidth ];

	if( m_pcXlsColNames == NULL )
	{
		return M4_ERROR;
	}

	memset( m_pcXlsColNames, 0, n_iNumCols * c_iXlsColumnWidth );

	for( int iIdx = 1; iIdx <= n_iNumCols; iIdx++ )
	{
		pcColumn = ( char* ) m_pcXlsColNames + ( (iIdx-1) * c_iXlsColumnWidth );

		// -- Column is in the A to Z range
		if( iIdx <= 26 )
		{
			*pcColumn = 64 + iIdx; 
		}
		else if( iIdx <= 702 )
		{
			// -- Column is in the AA to ZZ range.
			iRange1c   = ( int )( iIdx / 26.0 );
			*pcColumn++	= ( !(iIdx % 26) ) ? 63 + iRange1c : 64 + iRange1c;
			*pcColumn	= ( !(iIdx % 26) ) ? 90 : 64 + ( iIdx % 26 );
		}
		else
		{
			// -- Column is in the AAA to XFD range.
			iRange1c   = ( int )( (iIdx - 702) / 676.0 ); // 702 is A to ZZ and 676 is AAA to AZZ
			iRange2c   = ( int )( ( (iIdx - 702) % 676 ) / 26.0 ); 

			*pcColumn++	= ( !( (iIdx - 702) % 676 ) ) ? 64 + iRange1c : 65 + iRange1c;
			*pcColumn++	= ( !( (iIdx - 702) % 676 )  ) ? 90 : ( !( ( (iIdx - 702) % 676 ) % 26 ) ) ? 64 + iRange2c : 65 + iRange2c;
			*pcColumn	= ( !(iIdx % 26) ) ? 90 : 64 + ( iIdx % 26 );
		}
	}

	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Class ClCellPos
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------

ClCellPos::ClCellPos()
{
	m_iRow = 0;
	m_iCol = 0;
}


// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------

ClCellPos::ClCellPos( int ai_iRow, int ai_iCol )
{
	m_iRow = ai_iRow;
	m_iCol = ai_iCol;
}


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
ClCellPos::~ClCellPos()
{}


// -----------------------------------------------------------------------------
// Initializer
// -----------------------------------------------------------------------------
void ClCellPos::Init( int ai_iRow, int ai_iCol )
{
	m_iRow = ai_iRow;
	m_iCol = ai_iCol;
}


// -----------------------------------------------------------------------------
// Less operator
// -----------------------------------------------------------------------------
bool ClCellPos::operator < ( const ClCellPos& c2 ) const
{
	if( m_iRow < c2.m_iRow )
	{
		return true;
	}
	else if( m_iRow > c2.m_iRow )
	{
		return false;
	}
	else
	{
		return( m_iCol < c2.m_iCol );
	}
}


// -----------------------------------------------------------------------------
// Class ClAPICallTracer
// -----------------------------------------------------------------------------

int ClAPICallTracer::s_iFileCounter = 0;

// -----------------------------------------------------------------------------
// Traces an API call.
//
//	ai_pContext:	CVM context.
//	ai_pvArg:		Function arguments.
//	ai_iLongArg:	Arguments number.
// -----------------------------------------------------------------------------
m4return_t ClAPICallTracer::TraceAPICall(ClVMRunContext* ai_pContext, m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg)
{
	m4return_t	iRet;
	string		sTraceFile;
	char		acHandle[17];
	FILE*		pfLog = NULL;
	string		sStrArg;
	m4pcchar_t	pcChannel, pcNode, pcMethod;


	// This check should not be necessary.
	if (s_bTraceEnabled == false)
	{
		return M4_ERROR;
	}

	iRet = GetTraceDir(ai_pContext->m_pAccess, sTraceFile);

	if (iRet == M4_ERROR)
	{
		return M4_ERROR;
	}

	iRet = getAPIMethod(ai_pContext, ai_pvArg, ai_iLongArg, pcChannel, pcNode, pcMethod);

	if (iRet == M4_ERROR)
	{
		return M4_ERROR;
	}

	sprintf(acHandle, "%d", ai_pContext->m_pAccess->GetHandle());

	sTraceFile += GetDirSeparator();
	sTraceFile += "m4xtrace_api_";
	sTraceFile += acHandle;
	sTraceFile += ".ln4";

	pfLog = fopen(sTraceFile.c_str(), "at");

	if (!pfLog)
	{
		return M4_ERROR;
	}

	for (int i = 0; i < ai_iLongArg; i++)
	{
		fprintf(pfLog, "v%d = ", i + 1);

		switch (ai_pvArg[i].Type)
		{
		case M4CL_CPP_TYPE_STRING_VAR:

			// Makes a local copy of input files.
			if ( (i == 0 && !strcmp(pcNode, "SNTC_EXCEL_CPP_WORKBOOK") && (!strcmp(pcMethod, "OPEN_WORKBOOK") || !strcmp(pcMethod, "OPEN_WORKBOOK_PARTIALLY")) ) || \
				 (i == 2 && !strcmp(pcNode, "SNTC_EXCEL_CPP_WORKSHEET") && (!strcmp(pcMethod, "DLL_IMPORT_CSV") || !strcmp(pcMethod, "DLL_IMPORT_CSV_EX")) ))
			{
				iRet = processFileArg(ai_pContext, ai_pvArg[i].Data.PointerChar, sStrArg);

				if (iRet == M4_ERROR)
				{
					fclose(pfLog);
					return M4_ERROR;
				}
			}
			else
			{
				sStrArg = ai_pvArg[i].Data.PointerChar;

				replaceStr(sStrArg, "\"", "\" + chr(34) + \"");
				replaceStr(sStrArg, "\t", "\" + chr(9) + \"");
				replaceStr(sStrArg, "\r", "\" + chr(13) + \"");
				replaceStr(sStrArg, "\n", "\" + chr(10) + \"");
			}

			fprintf(pfLog, "\"%s\"\n", sStrArg.c_str());
			break;

		case M4CL_CPP_TYPE_NULL:

			fprintf(pfLog, "NullValue()\n");
			break;

		case M4CL_CPP_TYPE_NUMBER:
		case M4CL_CPP_TYPE_DATE:

			fprintf(pfLog, "%f\n", ai_pvArg[i].Data.DoubleData);
			break;

		default:

			fprintf(pfLog, "v%d = ", i + 1);
			fprintf(pfLog, "\"__unsupported_type_%d__\"\n", ai_pvArg[i].Type);
			break;
		}
	}

	fprintf(pfLog, "%s!%s.%s(", pcChannel, pcNode, pcMethod);


	for (int i = 0; i < ai_iLongArg; i++)
	{
		if (i > 0)
		{
			fprintf(pfLog, ", ");
		}

		fprintf(pfLog, "v%d", i + 1);
	}

	fprintf(pfLog, ")\n\n");
	fclose(pfLog);


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Gets the HL method invoker.
//
//	ai_pContext:	CVM context.
//	ai_pvArg:		Function arguments.
//	aio_iLongArg:	Input/output arguments number.
//	ao_pcChannel:	Output channel.
//	ao_pcNode:		Output node.
//	ao_pcMethod:	Output method.
// -----------------------------------------------------------------------------
m4return_t ClAPICallTracer::getAPIMethod(ClVMRunContext* ai_pContext, m4VariantType *ai_pvArg, m4uint32_t& aio_iLongArg, m4pcchar_t& ao_pcChannel, m4pcchar_t& ao_pcNode, m4pcchar_t& ao_pcMethod)
{
	ao_pcChannel = ai_pContext->m_pAccess->GetpChannel()->GetpCMCR()->GetChannelId();
	ao_pcNode    = ai_pContext->m_pAccess->GetpChannel()->GetpCMCR()->GetNodeId(ai_pContext->m_pRecordSet->GetpNodeDef()->GetHandle());
	ao_pcMethod  = ai_pContext->m_pAccess->GetpChannel()->GetpCMCR()->GetItemId(ai_pContext->m_hItem);

	// Replace methods that needs some work done by its upper level method.
	if (!strcmp(ao_pcNode, "SNTC_EXCEL_CPP_WORKBOOK"))
	{
		if (!strcmp(ao_pcMethod, "DLL_CREATE_WB"))
		{
			ao_pcMethod = "CREATE_WORKBOOK";
		}
		else if (!strcmp(ao_pcMethod, "DLL_NEW_WB_FILE"))
		{
			ao_pcMethod = "NEW_WORKBOOK";
			aio_iLongArg = 1;
		}
		else if (!strcmp(ao_pcMethod, "DLL_OPEN_WB"))
		{
			ao_pcMethod = "OPEN_WORKBOOK";
			aio_iLongArg = 1;
		}
		else if (!strcmp(ao_pcMethod, "DLL_OPEN_WB_PARTIALLY"))
		{
			ao_pcMethod = "OPEN_WORKBOOK_PARTIALLY";
			aio_iLongArg = 4;
		}
	}
	else if (!strcmp(ao_pcNode, "SNTC_EXCEL_CPP_WORKSHEET"))
	{
		if (!strcmp(ao_pcMethod, "DLL_NEW_WS"))
		{
			ao_pcMethod = "NEW_WORKSHEET";
		}
		else if (!strcmp(ao_pcMethod, "DLL_ADD_WS"))
		{
			ao_pcMethod = "ADD_WORKSHEET";
		}
		else if (!strcmp(ao_pcMethod, "DLL_GET_WS") && aio_iLongArg == 1)
		{
			if (ai_pvArg[0].Type == M4CL_CPP_TYPE_STRING_VAR)
			{
				ao_pcMethod = "GET_WORKSHEET_BY_NAME";
			}
			else if (ai_pvArg[0].Type == M4CL_CPP_TYPE_NUMBER)
			{
				ao_pcMethod = "GET_WORKSHEET";
			}
		}
	}


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Manages a file argument.
//
//	ai_pContext:	CVM context.
//	ai_pcArgFile:	File path.
//	ao_sArgFile:	Output file path where the copy was made.
// -----------------------------------------------------------------------------
m4return_t ClAPICallTracer::processFileArg(ClVMRunContext* ai_pContext, m4pcchar_t ai_pcArgFile, string& ao_sArgFile)
{
	m4return_t	iRet;
	char*		pcInputFile;
	int			iLength = -1;
	char		acFileNum[10];
	const char*	pcDotPos;


	iRet = GetTraceDir(ai_pContext->m_pAccess, ao_sArgFile);

	if (iRet == M4_ERROR)
	{
		return M4_ERROR;
	}

	sprintf(acFileNum, "%d", ++s_iFileCounter);
	pcDotPos = strrchr(ai_pcArgFile, '.');

	ao_sArgFile += GetDirSeparator();
	ao_sArgFile += "m4xlarg_input_file_";
	ao_sArgFile += acFileNum;

	if (pcDotPos != NULL)
	{
		ao_sArgFile += pcDotPos;
	}

	pcInputFile = M4CppToANSI(ai_pcArgFile, iLength);

	iRet = CopySingleFile(pcInputFile, ao_sArgFile.c_str());

	delete pcInputFile;


	return iRet;
}


// -----------------------------------------------------------------------------
// Substrings replacer.
// -----------------------------------------------------------------------------
void ClAPICallTracer::replaceStr(string& ai_sSource, const string& ai_sWhat, const string& ai_sWith)
{
	size_t start_pos = 0;

	while ((start_pos = ai_sSource.find(ai_sWhat, start_pos)) != string::npos) {
		ai_sSource.replace(start_pos, ai_sWhat.length(), ai_sWith);
		start_pos += ai_sWith.length();
	}
}
