//==============================================================================
//
// (c) Copyright  1991-2013 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4xlapi
// File:             m4csvsheetaccessor.cpp
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

#include <m4xlsheetaccessor.hpp>
#include <m4xlhelper.hpp>
#include <m4xlapierrors.hpp>
#include <m4log.hpp>
#include <cldatesbase.hpp>
#include <m4unicode.hpp>
#include <m4xlformatsmngr.hpp>
#include <wchar.h>
#include <math.h>

// -----------------------------------------------------------------------------
// Class ClXlSheetAccessor
// -----------------------------------------------------------------------------


// Internal class constants.
const wchar_t* ClXlSheetAccessor::apwcDateTokens[]	= { L"MMM", L"D", L"YY" };
const wchar_t* ClXlSheetAccessor::apwcTimeTokens[]	= { L"H", L"[M]", L"S", L"AM/PM", L"A/P" };
const wchar_t* ClXlSheetAccessor::apwcNoisyTags[]	= { L"[WHITE]", L"[MAGENTA]", L"[RED]" };


// -----------------------------------------------------------------------------
// Constructor
// 
//	ai_iInitRow:	Row number of the base cell.
//	ai_iInitCol:	Column number of the base cell.
//	ai_pBook:		xl wbook reference.
//	ai_pSheet:		xl wsheet reference.
// -----------------------------------------------------------------------------
ClXlSheetAccessor::ClXlSheetAccessor( int ai_iInitRow, int ai_iInitCol, Book* ai_pBook, Sheet* ai_pSheet, ClVMRunContext* ai_pContext, const char* ai_pcForcedColTypes )
{
	m_iInitRow				= ai_iInitRow;
	m_iInitCol				= ai_iInitCol;
	m_pBook					= ai_pBook;
	m_pSheet				= ai_pSheet;
	m_iExtrapolationMode	= GEN_MODE;
	m_pContext				= ai_pContext;

	initialize( ai_pcForcedColTypes );
}


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
ClXlSheetAccessor::~ClXlSheetAccessor()
{
	if( m_pvForcedColTypes != NULL )
	{
		m_pvForcedColTypes->clear();
		delete m_pvForcedColTypes;
	}
}


// -----------------------------------------------------------------------------
// Initialization of forced column types.
//
//	ai_pcForcedColTypes:	Column types serialized in a string.
// -----------------------------------------------------------------------------
m4return_t ClXlSheetAccessor::initialize( const char* ai_pcForcedColTypes )
{
	string	sTypesList;
	size_t	iPos;
	size_t	iLen;
	char*	pcTypesList;
	char*	pcToken;
	char	acNoTypeSubstr[ 8 ];


	if( ai_pcForcedColTypes == NULL || *ai_pcForcedColTypes == 0 )
	{
		m_pvForcedColTypes = NULL;
		return M4_SUCCESS;
	}


	// Avoid empty column type definitions.
	sprintf( acNoTypeSubstr, ";%d;", FCD_NO_TYPE );
	
	sTypesList = ai_pcForcedColTypes;
	while( ( iPos = sTypesList.find( ";;" ) ) != -1 )
	{
		sTypesList.replace( iPos, 2 , acNoTypeSubstr );
	}

	// Copy for strtok.
	iLen = sTypesList.length() + 1;
	pcTypesList = new char[ iLen ];
	memcpy( pcTypesList, sTypesList.c_str(), iLen );

	// Forced types mode.
	m_pvForcedColTypes = new vector< int >();
	m_pvForcedColTypes->reserve( ( iLen /2 ) + 1 );

	// Tokenize the list of column types for an easy to access container.
	pcToken = strtok( pcTypesList, ";" );
	while( pcToken != NULL )
	{
		m_pvForcedColTypes->push_back( atoi( pcToken ) );
		pcToken = strtok( NULL, ";" );
	}

	delete pcTypesList;


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Writes a value into a cell, given it with Meta4 format and optionally with 
// ISO format.
// 
//	ai_iRow:		Row number of the cell to write.
//	ai_iCol:		Column number of the cell to write.
//	ai_pwcM4Value:	String with the Meta4 formated value to write.
//	ai_pwcISOValue:	Optional string with the ISO formated value to write.
//
// Note: Used when importing a csv file or a csv string, through the m4csvparser.
// See Excel output of the query tool, the ImportCSVEx (Forced types),
// PasteSpecialText and the PasteSpecialTextEx API methods.
// -----------------------------------------------------------------------------
m4return_t ClXlSheetAccessor::WriteValue( int ai_iRow, int ai_iCol, wchar_t* ai_pwcM4Value, wchar_t* ai_pwcISOValue )
{
	m4return_t	iRet;
	int			iRow = m_iInitRow + ai_iRow;
	int			iCol = m_iInitCol + ai_iCol;
	eTargetType	iTargetType, iOriginalTgtType;
	Format*		pFormat;
	int			iNumFormat;
	double		dValue;
	double		dPackedDate;
	Format*		pNewFormat;
	wchar_t*	pwcValue;
	wchar_t*	pwcEndOfNumber = NULL;


	pFormat	= m_pSheet->cellFormat( iRow, iCol );

	// Regardless of the target type, it will be a blank cell.
	if( ai_pwcM4Value == NULL || *ai_pwcM4Value == 0 )
	{
		m_pSheet->writeBlank( iRow, iCol, pFormat );
		return M4_SUCCESS;
	}

	// Formulas support.
	if( *ai_pwcM4Value == L'=' )
	{
		iTargetType = TYPE_FORMULA;
	}
	else
	{
		if( pFormat	!= NULL )
		{
			iNumFormat = pFormat->numFormat();
			iTargetType = iOriginalTgtType = getTargetType( iNumFormat );

			// Normal behavior: retrieves the target column types from the template.
			if( m_pvForcedColTypes == NULL )
			{
				// bg 262777
				if( iTargetType == TYPE_GENERAL )
				{
					iTargetType = extrapolateTargetType( iTargetType, ai_pwcM4Value, ai_pwcISOValue );
				}
			}
			else
			{
				// Forced behavior: the target column type was specified by the API user.
				iTargetType = getForcedTargetType( ai_iCol, ai_pwcM4Value );

				// Trick. Values must meet the ISO format.
				if( ai_pwcISOValue == NULL )
				{
					ai_pwcISOValue = ai_pwcM4Value;
				}
			}


			if( iOriginalTgtType != iTargetType && ( iOriginalTgtType == TYPE_GENERAL || m_pvForcedColTypes != NULL ) )
			{
				// bg 265837
				// bg 294225. 
				//		When using forced types, format object must be changed always. 
				//		When type was extrapolated, format object is only changed from General to Dates.
				if( iTargetType == TYPE_DATE_TIME || iTargetType == TYPE_DATE || iTargetType == TYPE_TIME || m_pvForcedColTypes != NULL)
				{
					// Gets a new format based on the current one, where the number format matches the 
					// extrapolated/forced target type.
					iRet = GetTargetFormat( pFormat, iTargetType, m_pBook, ai_pwcM4Value, pNewFormat );
					
					if( iRet == M4_SUCCESS && pNewFormat != NULL )
					{
						pFormat = pNewFormat;
					}
					else
					{
						SETCODEF( M4_XL_ERR_FORMAT_NOT_ADDED, ERRORLOG, "#*s1*#%d#%d#", ai_iRow, ai_iCol );
						iTargetType = TYPE_TEXT;
					}
				}
			}
		}
		else
		{
			// Text will be the default behavior.
			iTargetType = TYPE_TEXT;
		}
	}


	switch( iTargetType )
	{
		case TYPE_FORMULA:

			iRet = writeCellFormula( iRow, iCol, ai_pwcM4Value, pFormat );

			break;

		case TYPE_NUMBER:
		case TYPE_PERCENTAGE:
			
			if( ai_pwcISOValue != NULL )
			{
				pwcValue = ai_pwcISOValue;
			}
			else
			{
				pwcValue = ai_pwcM4Value;
			}

			dValue = wcstod( pwcValue, &pwcEndOfNumber );

			if ((isnan(dValue) == true) || (dValue == 0.0 && pwcValue == pwcEndOfNumber))
			{
				// Something was wrong, sets the M4 formated value as string
				iRet = writeCellValue( iRow, iCol, ai_pwcM4Value, pFormat );
			}
			else
			{
				if( iTargetType == TYPE_PERCENTAGE )
				{
					dValue /= 100;
				}

				iRet = writeCellValue( iRow, iCol, dValue, pFormat );
			}

			break;

		case TYPE_DATE:
		case TYPE_DATE_TIME:

			if( ai_pwcISOValue != NULL )
			{
				iRet = getXlPackedDate( ai_pwcISOValue, dPackedDate );
			}
			else
			{
				iRet = getXlPackedDate( ai_pwcM4Value, dPackedDate );
			}

			if( iRet == M4_SUCCESS && dPackedDate > 0 )
			{
				iRet = writeCellValue( iRow, iCol, dPackedDate, pFormat );
			}
			else
			{
				// Something was wrong or date out of Excel limits (281049), so sets it as string.
				if( ai_pwcISOValue != NULL )
				{
					iRet = writeCellValue( iRow, iCol, ai_pwcISOValue, pFormat );
				}
				else
				{
					iRet = writeCellValue( iRow, iCol, ai_pwcM4Value, pFormat );
				}
			}


			break;

		case TYPE_TIME:
			
			if( ai_pwcISOValue != NULL )
			{
				iRet = getXlPackedTime( ai_pwcISOValue, dPackedDate );
			}
			else
			{
				iRet = getXlPackedTime( ai_pwcM4Value, dPackedDate );
			}


			if( iRet == M4_SUCCESS && dPackedDate > 0 )
			{
				iRet = writeCellValue( iRow, iCol, dPackedDate, pFormat );
			}
			else
			{
				// Something was wrong or date out of Excel limits (281049), so sets it as string.
				if( ai_pwcISOValue != NULL )
				{
					iRet = writeCellValue( iRow, iCol, ai_pwcISOValue, pFormat );
				}
				else
				{
					iRet = writeCellValue( iRow, iCol, ai_pwcM4Value, pFormat );
				}
			}

			break;
		
		case TYPE_TEXT:
			
			// In this case ISO value, if exists, is discarded. 
			iRet = writeCellValue( iRow, iCol, ai_pwcM4Value, pFormat );
			
			break;

		default:

			iRet = M4_ERROR;
	}


	return iRet;
}


// -----------------------------------------------------------------------------
// Writes a value stored in a Meta4 variant object, into a cell.
// 
//	ai_iRow:		Row number of the cell to write.
//	ai_iCol:		Column number of the cell to write.
//	ai_pvValue:		Meta4 variant object with the value to write.
//
// Note: Used when setting a single value (see SetCellValue API method).
// Forced column types not supported with this source values. Always retrieve 
// the target type from the template cell.
// -----------------------------------------------------------------------------
#define WCBUFFER_SIZE	128
m4return_t ClXlSheetAccessor::WriteValue( int ai_iRow, int ai_iCol, m4VariantType* ai_pvValue ) 
{
	m4return_t	iRet;
	int			iRow = m_iInitRow + ai_iRow;
	int			iCol = m_iInitCol + ai_iCol;
	char		iSourceType;
	eTargetType	iTargetType, iOriginalTgtType;
	Format*		pFormat, *pOriginalFormat;
	Format*		pNewFormat;
	int			iNumFormat;
	double		dValue;
	m4int32_t	iHours = 0, iMins = 0, iSecs = 0;
	int			iYears = 0, iMonths = 0, iDays = 0;
	wchar_t*	pwcValue;
	wchar_t		awcValue[ WCBUFFER_SIZE ];
	int			iLength = -1;
	double		dPackedDate;


	iSourceType = ai_pvValue->GetType();
	
	pOriginalFormat = pFormat = m_pSheet->cellFormat( iRow, iCol );

	// Regardless of the target type, it will be a blank cell.
	if( iSourceType == M4CL_CPP_TYPE_NULL )
	{
		m_pSheet->writeBlank( iRow, iCol, pFormat );
		return M4_SUCCESS;
	}

	// Formulas support.
	if( iSourceType == M4CL_CPP_TYPE_STRING_VAR && strlen( ai_pvValue->Data.PointerChar ) > 0 && *( ai_pvValue->Data.PointerChar ) == L'=' )
	{
		iTargetType = TYPE_FORMULA;
	}
	else
	{
		if( pFormat	!= NULL )
		{
			iNumFormat	= pFormat->numFormat();
			iTargetType = iOriginalTgtType = getTargetType( iNumFormat );
			
			// Performs the same behaviour that csv cell value writing.
			if( iOriginalTgtType == TYPE_GENERAL )
			{
				iTargetType = extrapolateTargetType( iTargetType, ai_pvValue );

				if( iOriginalTgtType != iTargetType )
				{
					if( iTargetType == TYPE_DATE_TIME || iTargetType == TYPE_DATE || iTargetType == TYPE_TIME )
					{
						// Gets a new format based on the current one, where the number format matches the 
						// extrapolated target type.
						iRet = GetTargetFormat( pFormat, iTargetType, m_pBook, NULL, pNewFormat );
						
						if( iRet == M4_SUCCESS && pNewFormat != NULL )
						{
							// It might be necessary to use the original format.
							pOriginalFormat = pFormat;
							pFormat = pNewFormat;
						}
						else
						{
							SETCODEF( M4_XL_ERR_FORMAT_NOT_ADDED, ERRORLOG, "#*s1*#%d#%d#", ai_iRow, ai_iCol );
							iTargetType = TYPE_TEXT;
						}
					}
				}
			}
		}
		else
		{
			// Text will be the default behavior.
			iTargetType = TYPE_TEXT;
		}
	}


	switch( iTargetType )
	{
		case TYPE_FORMULA:

			iRet = CppStrToLibxlStr( ai_pvValue->Data.PointerChar, pwcValue );
			if( iRet == M4_ERROR )
			{
				SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", "WriteValue" );
				return M4_ERROR;
			}

			iRet = writeCellFormula( iRow, iCol, pwcValue, pFormat );

			delete [] pwcValue;

			break;

		case TYPE_NUMBER:
		case TYPE_PERCENTAGE:
			
			switch( iSourceType )
			{
				case M4CL_CPP_TYPE_NUMBER:

					dValue = ai_pvValue->Data.DoubleData;

					if( iTargetType == TYPE_PERCENTAGE )
					{
						dValue /= 100;
					}

					iRet = writeCellValue( iRow, iCol, dValue, pFormat );

					break;

				case M4CL_CPP_TYPE_DATE:
					
					// Mismatched types, writing the datum as string.
					ClDateToNumbers( ai_pvValue->Data.DoubleData, iYears, iMonths, iDays, iHours, iMins, iSecs );

					if( IsDateForM4Time( iYears, iMonths, iDays ) == true )
					{
						ComposeString( awcValue, WCBUFFER_SIZE, L"%02d:%02d:%02d", iHours, iMins, iSecs );
					}
					else if( iHours == 0 && iMins == 0 && iSecs == 0 )
					{
						ComposeString( awcValue, WCBUFFER_SIZE, L"%04d-%02d-%02d", iYears, iMonths, iDays );
					}
					else
					{
						ComposeString( awcValue, WCBUFFER_SIZE, L"%04d-%02d-%02d %02d:%02d:%02d", iYears, iMonths, iDays, iHours, iMins, iSecs );
					}

					iRet = writeCellValue( iRow, iCol, awcValue, pFormat );
					
					break;

				case M4CL_CPP_TYPE_STRING_VAR:

					// Mismatched types, checks if the text only has digits...
					m4bool_t bIsNumber = M4_TRUE;
					iLength = strlen( ai_pvValue->Data.PointerChar );

					for( size_t i = 0; i < iLength; i++ )
					{
						if( isdigit( ai_pvValue->Data.PointerChar[ i ] ) == 0 && ai_pvValue->Data.PointerChar[ i ] != '.' && ( ai_pvValue->Data.PointerChar[ i ] != '-' || i > 0 ) )
						{
							bIsNumber = M4_FALSE;
							break;
						}
					}

					if( bIsNumber == M4_TRUE )
					{
						dValue = atof( ai_pvValue->Data.PointerChar );
						iRet = writeCellValue( iRow, iCol, dValue, pFormat );
					}
					else
					{
						// ... if not, writes the datum as string.
						iRet = CppStrToLibxlStr( ai_pvValue->Data.PointerChar, pwcValue );
						if( iRet == M4_ERROR )
						{
							SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", "WriteValue" );
							return M4_ERROR;
						}

						iRet = writeCellValue( iRow, iCol, pwcValue, pFormat );

						delete [] pwcValue;
					}

					break;
			}

			break;

		case TYPE_DATE:
		case TYPE_TIME:
		case TYPE_DATE_TIME:

			switch( iSourceType )
			{
				case M4CL_CPP_TYPE_NUMBER:

					// Mismatched types, writing the datum as string.
					ComposeString( awcValue, WCBUFFER_SIZE, L"%f", ai_pvValue->Data.DoubleData );

					iRet = writeCellValue( iRow, iCol, awcValue, pFormat );

					break;
					
				case M4CL_CPP_TYPE_DATE:

					ClDateToNumbers( ai_pvValue->Data.DoubleData, iYears, iMonths, iDays, iHours, iMins, iSecs );

					// Time type.
					if( IsDateForM4Time( iYears, iMonths, iDays ) == true || iTargetType == TYPE_TIME )
					{
						iYears	= 0;
						iMonths	= 0;
						iDays	= 0;
					}

					dPackedDate = m_pBook->datePack( iYears, iMonths, iDays, iHours, iMins, iSecs );

					if( dPackedDate >= 0 )
					{
						iRet = writeCellValue( iRow, iCol, dPackedDate, pFormat );
					}
					else
					{
						// Date out of Excel limits. Writes as ISO date text (281049).
						if( iHours == 0 && iMins == 0 && iSecs == 0 )
						{
							ComposeString( awcValue, WCBUFFER_SIZE, L"%d/%d/%d", iYears, iMonths, iDays );
						}
						else
						{
							ComposeString( awcValue, WCBUFFER_SIZE, L"%d/%d/%d %d:%d:%d", iYears, iMonths, iDays, iHours, iMins, iSecs );
						}

						// Use the original format
						iRet = writeCellValue( iRow, iCol, awcValue, pOriginalFormat );
					}
					
					break;

				case M4CL_CPP_TYPE_STRING_VAR:

					// Mismatched types.
					iRet = CppStrToLibxlStr( ai_pvValue->Data.PointerChar, pwcValue );
					if( iRet == M4_ERROR )
					{
						SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", "WriteValue" );
						return M4_ERROR;
					}

					// Try if source data is an ISO date.
					if( iTargetType == TYPE_DATE )
					{
						iRet = getXlPackedDate( pwcValue, dPackedDate );
					}
					else
					{
						iRet = getXlPackedTime( pwcValue, dPackedDate );
					}
					
					if( iRet == M4_SUCCESS && dPackedDate > 0 )
					{
						iRet = writeCellValue( iRow, iCol, dPackedDate, pFormat );
					}
					else
					{
						// Something was wrong or date out of Excel limits (281049), so sets it as string.
						iRet = writeCellValue( iRow, iCol, pwcValue, pFormat );

						delete [] pwcValue;
					}

					break;
			}

			break;

		case TYPE_TEXT:

			switch( iSourceType )
			{
				case M4CL_CPP_TYPE_NUMBER:
					
					double dIntPart;
					if( modf( ai_pvValue->Data.DoubleData, &dIntPart ) == 0.0 )
					{
						ComposeString( awcValue, WCBUFFER_SIZE, L"%.0f", ai_pvValue->Data.DoubleData );
					}
					else
					{
						ComposeString( awcValue, WCBUFFER_SIZE, L"%f", ai_pvValue->Data.DoubleData );
					}

					iRet = writeCellValue( iRow, iCol, awcValue, pFormat );

					break;

				case M4CL_CPP_TYPE_DATE:

					ClDateToNumbers( ai_pvValue->Data.DoubleData, iYears, iMonths, iDays, iHours, iMins, iSecs );

					if( IsDateForM4Time( iYears, iMonths, iDays ) == true )
					{
						ComposeString( awcValue, WCBUFFER_SIZE, L"%02d:%02d:%02d", iHours, iMins, iSecs );
					}
					else if( iHours == 0 && iMins == 0 && iSecs == 0 )
					{
						ComposeString( awcValue, WCBUFFER_SIZE, L"%04d-%02d-%02d", iYears, iMonths, iDays );
					}
					else
					{
						ComposeString( awcValue, WCBUFFER_SIZE, L"%04d-%02d-%02d %02d:%02d:%02d", iYears, iMonths, iDays, iHours, iMins, iSecs );
					}

					iRet = writeCellValue( iRow, iCol, awcValue, pFormat );

					break;

				case M4CL_CPP_TYPE_STRING_VAR:

					iRet = CppStrToLibxlStr( ai_pvValue->Data.PointerChar, pwcValue );
					if( iRet == M4_ERROR )
					{
						SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", "WriteValue" );
						return M4_ERROR;
					}

					iRet = writeCellValue( iRow, iCol, pwcValue, pFormat );

					delete [] pwcValue;

					break;
			}

			break;

		default:

			iRet = M4_ERROR;
	}

	return iRet;
}


// -----------------------------------------------------------------------------
// Reads a cell, storing the the value in a Meta4 variant object.
// 
//	ai_iRow:		Row number of the cell to read.
//	ai_iCol:		Column number of the cell to read.
//	ai_pvRefArg:	Output Meta4 variant object with the read value.
//
// Note: Used when getting a single value (see DumpRangeToNode iterative process
// and the GetCellValue API methods).
// -----------------------------------------------------------------------------
m4return_t	ClXlSheetAccessor::ReadValue( int ai_iRow, int ai_iCol, m4VariantType* ai_pvRefArg )
{
	m4return_t		iRet;
	int				iRow = m_iInitRow + ai_iRow;
	int				iCol = m_iInitCol + ai_iCol;
	double			dValue;
	int				iYear, iMonth, iDay, iHour, iMin , iSec, iMsec;
	m4date_t		dDate;
	const wchar_t*	pwcValue;
	char*			pcValue;
	bool			bValue;
	Format*			pFormat;
	int				iNumFormat;
	eTargetType		iTargetType = TYPE_TEXT;
	wchar_t			awcValue[ WCBUFFER_SIZE ];


	// Get the cell type.	
	CellType iCellType = m_pSheet->cellType( iRow, iCol );

	// bg 273132.
	pFormat	= m_pSheet->cellFormat( iRow, iCol );
	if( pFormat != NULL )
	{
		iNumFormat = pFormat->numFormat();
		iTargetType = getTargetType( iNumFormat );
	}

	// Data type switch.
	switch( iCellType )
	{
		case CELLTYPE_EMPTY:
		case CELLTYPE_BLANK: 
			
			ai_pvRefArg->SetNull();
			iRet = M4_SUCCESS;

			break;

		 case CELLTYPE_NUMBER:

			iRet = readCellValue( iRow, iCol, dValue );

			if( iRet == M4_SUCCESS )
			{
				if( m_pSheet->isDate( iRow, iCol ) == true )
				{
					if( m_pBook->dateUnpack( dValue, &iYear, &iMonth, &iDay, &iHour, &iMin, &iSec, &iMsec ) == true )
					{
						if( iYear == 0 )
						{
							// Time type.
							SetDateForM4Time( iYear, iMonth, iDay );
						}
						
						ClNumbersToDate( iYear, iMonth, iDay, iHour, iMin, iSec, dDate );

						ai_pvRefArg->Type = M4CL_CPP_TYPE_DATE;
						ai_pvRefArg->Data.DoubleData = dDate;
					}
					else
					{
						return M4_ERROR;
					}
				}
				else
				{
					// Cell type switch.
					switch( iTargetType )
					{
						case TYPE_NUMBER:
						case TYPE_PERCENTAGE:
					
							ai_pvRefArg->SetDouble( dValue );

							break;

						case TYPE_TEXT:
						case TYPE_GENERAL:

							double dIntPart;
							if( modf( dValue, &dIntPart ) == 0.0 )
							{
								ComposeString( awcValue, WCBUFFER_SIZE, L"%.0f", dValue );
							}
							else
							{
								ComposeString( awcValue, WCBUFFER_SIZE, L"%f", dValue );
							}

							LibxlStrToCppStr( awcValue, pcValue );
							ai_pvRefArg->SetString(	pcValue );

							delete [] pcValue;

							break;

						default:

							return M4_ERROR;
					}
				}
			}

			break;
		 
		 case CELLTYPE_STRING:

			// Target is always a variant string. If the text inside the cell contains a number or a date/time,
			// the cell type (aka data type) would be number type.
 			iRet = readCellValue( iRow, iCol, pcValue );

			if( iRet == M4_SUCCESS )
			{
				ai_pvRefArg->SetString( pcValue );
				delete [] pcValue;
			}

			break;        
		 
		 case CELLTYPE_BOOLEAN:

			iRet = readCellValue( iRow, iCol, bValue );

			if( iRet == M4_SUCCESS )
			{
				if( bValue == true )
				{
					if( iTargetType == TYPE_GENERAL )
					{
						ai_pvRefArg->SetString(	"True" );
					}
					else
					{
						ai_pvRefArg->SetDouble( 1 );
					}
				}
				else
				{
					if( iTargetType == TYPE_GENERAL )
					{
						ai_pvRefArg->SetString(	"False" );
					}
					else
					{
						ai_pvRefArg->SetDouble( 0 );
					}
				}
			}

			break;
		 
		 case CELLTYPE_ERROR: 
			
			// bg 315812
			ai_pvRefArg->SetNull();

			/*
			iRet = readCellErrValue( iRow, iCol, pcValue );

			if( iRet == M4_SUCCESS )
			{
				ai_pvRefArg->SetString( pcValue );
			}
			*/
			break;

		 default:

			iRet = M4_ERROR;
	}


	return iRet;
}


#define MAX_SHEET_NAME	32

// -----------------------------------------------------------------------------
// Gets the automatic name for a new sheet.
//
//	ao_pwcSheetName: Output sheet name.
// -----------------------------------------------------------------------------
m4return_t ClXlSheetAccessor::getNewSheetAutoName( wchar_t* ao_pwcSheetName )
{
	int		iSheetsCount;


	if( ao_pwcSheetName == NULL )
	{
		return M4_ERROR;
	}

	iSheetsCount = m_pBook->sheetCount();

#ifdef UNIX
	swprintf ( ao_pwcSheetName, MAX_SHEET_NAME, L"Sheet%d", iSheetsCount + 1 );
#else
	swprintf ( ao_pwcSheetName, L"Sheet%d", iSheetsCount + 1 );
#endif


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Gets the current sheet index.
//
//	ao_iIndex: Output index number.
// -----------------------------------------------------------------------------
m4return_t ClXlSheetAccessor::getCurrSheetIndex( int& ao_iIndex )
{
	int		iSheetsCount;
	Sheet*	pWorksheet;


	ao_iIndex = -1;

	iSheetsCount = m_pBook->sheetCount();

	for( int i = 0; i < iSheetsCount; i++ )
	{
		pWorksheet = m_pBook->getSheet( i );

		if( wcscmp( pWorksheet->name(), m_pSheet->name() ) == 0 )
		{
			ao_iIndex = i;
			return M4_SUCCESS;
		}
	}


	return M4_ERROR;
}


// -----------------------------------------------------------------------------
// Updates the accessor context.
//
//	ai_iRow:		Row number of the base cell.
//	ai_iCol:		Column number of the base cell.
//	ai_iSheetIndex:	WSheet index.
//	ai_pSheet:		WSheet name.
// -----------------------------------------------------------------------------
m4return_t ClXlSheetAccessor::ChangeSheet( int ai_iRow, int ai_iCol, int ai_iSheetIndex, wchar_t* ai_pwcSheetName )
{
	m4return_t	iRet;
	Sheet*		pWorksheet;
	wchar_t		awcSheetName[ MAX_SHEET_NAME ];
	wchar_t*	pwcSheetName;
	int			iCurrIndex;
	char*		pcSheetName;


	if( ai_iRow != -1 )
	{
		m_iInitRow = ai_iRow;
	}

	if( ai_iCol != -1 )
	{
		m_iInitCol = ai_iCol;
	}


	if( ai_pwcSheetName != NULL )
	{
		SanitizeSheetName( ai_pwcSheetName );
	}

	if( ai_iSheetIndex != -1 )
	{
		// Must retrieve an existing sheet by index.
		pWorksheet = m_pBook->getSheet( ai_iSheetIndex );
		
		if( pWorksheet != NULL )
		{
			// Sheet successfully retrieved.
			m_pSheet = pWorksheet;

			if( ai_pwcSheetName != NULL )
			{
				m_pSheet->setName( ai_pwcSheetName );
			}

			return M4_SUCCESS;
		}
	}

	// Specified sheet index not found or sheet index unspecified,
	// so it must retrieve the sheet after the current one ..
	iRet = getCurrSheetIndex( iCurrIndex );

	if( iRet == M4_ERROR )
	{
		return M4_ERROR;
	}

	pWorksheet = m_pBook->getSheet( iCurrIndex + 1 );
	if( pWorksheet != NULL )
	{
		// Sheet successfully retrieved.
		m_pSheet = pWorksheet;

		if( ai_pwcSheetName != NULL )
		{
			m_pSheet->setName( ai_pwcSheetName );
		}

		return M4_SUCCESS;
	}

	// ... it does not exist so it must insert a new sheet after the current one ...
	if( ai_pwcSheetName != NULL )
	{
		// ... with the name specified.
		pwcSheetName = ai_pwcSheetName;
	}
	else
	{
		// ... with an auto-generated name.
		pwcSheetName = awcSheetName;

		iRet = getNewSheetAutoName( pwcSheetName );

		if( iRet == M4_ERROR )
		{
			return M4_ERROR;
		}
	}

	// Do the sheet insert.
	pWorksheet = m_pBook->insertSheet( iCurrIndex + 1, pwcSheetName );

	if( pWorksheet == NULL )
	{
		LibxlStrToCppStr( pwcSheetName, pcSheetName );
		SETCODEF( M4_XL_ERR_NEW_WS_LIB_ERR, ERRORLOG, "#*s1*#%s#%s#", m_pBook->errorMessage(), pcSheetName );
		delete [] pcSheetName;

		return M4_ERROR;
	}

	// Sheet successfully inserted.
	m_pSheet = pWorksheet;


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Sets the types extrapolation mode.
//
//	ai_iMode: Extrapolation mode.
// -----------------------------------------------------------------------------
void ClXlSheetAccessor::SetTypeExtrapolationMode( eTypeExtrapolationMode ai_iMode )
{
	m_iExtrapolationMode = ai_iMode;
}


// -----------------------------------------------------------------------------
// Internal method for writing a string into a cell.
// 
//	ai_iRow:		Row number of the cell to write.
//	ai_iCol:		Column number of the cell to write.
//	ai_pwcValue:	String to write.
//	ai_pFormat:		Excel format of the cell.
// -----------------------------------------------------------------------------
m4return_t	ClXlSheetAccessor::writeCellValue( int ai_iRow, int ai_iCol, wchar_t* ai_pwcValue, Format* ai_pFormat )
{
	bool	bRet;


	bRet = m_pSheet->writeStr( ai_iRow, ai_iCol, ai_pwcValue, ai_pFormat );

	if( !bRet )
	{
		SETCODEF( M4_XL_ERR_PARS_WRITE_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#%d#", m_pBook->errorMessage(), ai_iRow, ai_iCol );
		return M4_ERROR;
	}

	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Internal method for writing a number into a cell.
// 
//	ai_iRow:		Row number of the cell to write.
//	ai_iCol:		Column number of the cell to write.
//	ai_dValue:		Number to write.
//	ai_pFormat:		Excel format of the cell.
// -----------------------------------------------------------------------------
m4return_t	ClXlSheetAccessor::writeCellValue( int ai_iRow, int ai_iCol, double ai_dValue, Format* ai_pFormat )
{
	bool	bRet;


	bRet = m_pSheet->writeNum( ai_iRow, ai_iCol, ai_dValue, ai_pFormat );

	if( !bRet )
	{
		SETCODEF( M4_XL_ERR_PARS_WRITE_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#%d#", m_pBook->errorMessage(), ai_iRow, ai_iCol );
		return M4_ERROR;
	}

	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Internal method for writing a formula into a cell.
// 
//	ai_iRow:		Row number of the cell to write.
//	ai_iCol:		Column number of the cell to write.
//	ai_pwcFormula:	Formula string to write.
//	ai_pFormat:		Excel format of the cell.
// -----------------------------------------------------------------------------
m4return_t	ClXlSheetAccessor::writeCellFormula( int ai_iRow, int ai_iCol, wchar_t* ai_pwcFormula, Format* ai_pFormat )
{
	bool	bRet;


	bRet = m_pSheet->writeFormula( ai_iRow, ai_iCol, ai_pwcFormula, ai_pFormat );

	if( !bRet )
	{
		SETCODEF( M4_XL_ERR_PARS_WRITE_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#%d#", m_pBook->errorMessage(), ai_iRow, ai_iCol );
		return M4_ERROR;
	}

	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Internal method for reading a string from a cell.
// 
//	ai_iRow:		Row number of the cell to read.
//	ai_iCol:		Column number of the cell to read.
//	ao_pwcValue:	Output string.
// -----------------------------------------------------------------------------
m4return_t ClXlSheetAccessor::readCellValue( int ai_iRow, int ai_iCol, char*& ao_pcValue )
{
	m4return_t		iRet;
	const wchar_t*	pwcValue;
	const char*		pccErrorMsg;


	pwcValue = m_pSheet->readStr( ai_iRow, ai_iCol );

	// Error condition.
	if( pwcValue == NULL )
	{
		pccErrorMsg =  m_pBook->errorMessage();
		if( pccErrorMsg != NULL )
		{
			SETCODEF( M4_XL_ERR_GET_SET_CELL_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#%d#", pccErrorMsg, ai_iRow, ai_iCol );
			return M4_ERROR;
		}
	}

	iRet = LibxlStrToCppStr( pwcValue, ao_pcValue );
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_GEN_XL_2_CPP, ERRORLOG, "#*s1*#%s#", "readCellValue" );
	}

	return iRet;
}


// -----------------------------------------------------------------------------
// Internal method for reading a number from a cell.
// 
//	ai_iRow:		Row number of the cell to read.
//	ai_iCol:		Column number of the cell to read.
//	ao_dValue:		Output number.
// -----------------------------------------------------------------------------
m4return_t	ClXlSheetAccessor::readCellValue( int ai_iRow, int ai_iCol, double& ao_dValue )
{
	Format*		pFormat;
	const char*	pccErrorMsg;


	ao_dValue = m_pSheet->readNum( ai_iRow, ai_iCol, &pFormat );

	// Error condition.
	if( pFormat == 0 )
	{
		pccErrorMsg =  m_pBook->errorMessage();
		if( pccErrorMsg != NULL )
		{
			SETCODEF( M4_XL_ERR_GET_SET_CELL_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#%d#", pccErrorMsg, ai_iRow, ai_iCol );
		}
		return M4_ERROR;
	}

	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Internal method for reading a boolean from a cell.
// 
//	ai_iRow:		Row number of the cell to read.
//	ai_iCol:		Column number of the cell to read.
//	ao_bValue:		Output boolean.
// -----------------------------------------------------------------------------
m4return_t	ClXlSheetAccessor::readCellValue( int ai_iRow, int ai_iCol, bool& ao_bValue )
{
	Format*		pFormat;
	const char*	pccErrorMsg;


	ao_bValue = m_pSheet->readBool( ai_iRow, ai_iCol, &pFormat );

	// Error condition.
	if( pFormat == 0 )
	{
		pccErrorMsg =  m_pBook->errorMessage();
		if( pccErrorMsg != NULL )
		{
			SETCODEF( M4_XL_ERR_GET_SET_CELL_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#%d#", pccErrorMsg, m_iInitRow + ai_iRow, m_iInitCol + ai_iCol );
		}
		return M4_ERROR;
	}

	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Internal method for reading an Excel error from a cell.
// 
//	ai_iRow:		Row number of the cell to read.
//	ai_iCol:		Column number of the cell to read.
//	ao_pcValue:		Output string with the cell error.
// -----------------------------------------------------------------------------
m4return_t	ClXlSheetAccessor::readCellErrValue( int ai_iRow, int ai_iCol, char*& ao_pcValue )
{
	ErrorType	iErrorType;


	iErrorType = m_pSheet->readError( ai_iRow, ai_iCol );

	switch( iErrorType )
	{
		case ERRORTYPE_NULL:
			ao_pcValue = "#NULL!";
			break;
		case ERRORTYPE_DIV_0:
			ao_pcValue = "#DIV/0!";
			break;
		case ERRORTYPE_VALUE:
			ao_pcValue = "#VALUE!";
			break;
		case ERRORTYPE_REF:
			ao_pcValue = "#REF!";
			break;
		case ERRORTYPE_NAME:
			ao_pcValue = "#NAME?";
			break;
		case ERRORTYPE_NUM:
			ao_pcValue = "#NUM!";
			break;
		case ERRORTYPE_NA:
			ao_pcValue = "#N/A";
			break;
		case ERRORTYPE_NOERROR:
			ao_pcValue = "#no error";
			break;
		default:
			ao_pcValue = "#Unknown error";
			break;
	 }

	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Internal method that returns the Meta4 target type of a cell, given its number
// format. The target type defines the way a value must be written into that cell.
// 
//	ai_iNumFormat:	Number format of the cell.
// -----------------------------------------------------------------------------
eTargetType ClXlSheetAccessor::getTargetType( int ai_iNumFormat )
{
	const wchar_t*	pwcCustomFormat;
	wchar_t*		pwcUprCustomFormat;
	const wchar_t*	pwcToken;
	const wchar_t*	pwcAuxToken1;
	const wchar_t*	pwcAuxToken2;
	int				i;


	// Built-in number formats.
	switch( ai_iNumFormat )
	{
		case NUMFORMAT_GENERAL:
			return( TYPE_GENERAL );

		case NUMFORMAT_NUMBER:
		case NUMFORMAT_NUMBER_D2:
		case NUMFORMAT_NUMBER_SEP:
		case NUMFORMAT_NUMBER_SEP_D2:
		case NUMFORMAT_CURRENCY_NEGBRA:
		case NUMFORMAT_CURRENCY_NEGBRARED:
		case NUMFORMAT_CURRENCY_D2_NEGBRA:
		case NUMFORMAT_CURRENCY_D2_NEGBRARED:
			return( TYPE_NUMBER );

		case NUMFORMAT_PERCENT:
		case NUMFORMAT_PERCENT_D2:
			return( TYPE_PERCENTAGE );

		case NUMFORMAT_SCIENTIFIC_D2:
			
		case NUMFORMAT_FRACTION_ONEDIG:
		case NUMFORMAT_FRACTION_TWODIG:
			return( TYPE_NUMBER );

		case NUMFORMAT_DATE:
		case NUMFORMAT_CUSTOM_D_MON_YY:
		case NUMFORMAT_CUSTOM_D_MON:
		case NUMFORMAT_CUSTOM_MON_YY:
			return( TYPE_DATE );

		case NUMFORMAT_CUSTOM_HMM_AM:
		case NUMFORMAT_CUSTOM_HMMSS_AM:
		case NUMFORMAT_CUSTOM_HMM:
		case NUMFORMAT_CUSTOM_HMMSS:
			return( TYPE_TIME );

		case NUMFORMAT_CUSTOM_MDYYYY_HMM:
			return( TYPE_DATE );

		case NUMFORMAT_NUMBER_SEP_NEGBRA:
		case NUMFORMAT_NUMBER_SEP_NEGBRARED:
		case NUMFORMAT_NUMBER_D2_SEP_NEGBRA:
		case NUMFORMAT_NUMBER_D2_SEP_NEGBRARED:
		case NUMFORMAT_ACCOUNT:
		case NUMFORMAT_ACCOUNTCUR:
		case NUMFORMAT_ACCOUNT_D2:
		case NUMFORMAT_ACCOUNT_D2_CUR:
			return( TYPE_NUMBER );

		case NUMFORMAT_CUSTOM_MMSS:
		case NUMFORMAT_CUSTOM_H0MMSS:
		case NUMFORMAT_CUSTOM_MMSS0:
		case NUMFORMAT_CUSTOM_000P0E_PLUS0:
			return( TYPE_TIME );

		case NUMFORMAT_TEXT:
			return( TYPE_TEXT );

		default:

			// Custom number formats. Could be interesting read this: 
			// http://office.microsoft.com/en-001/excel-help/create-a-custom-number-format-HP010342372.aspx

			pwcCustomFormat = m_pBook->customNumFormat( ai_iNumFormat );
			
			if( !pwcCustomFormat )
			{
				SETCODEF( M4_XL_ERR_PARS_NUMFORMAT_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#", m_pBook->errorMessage(), ai_iNumFormat );
				return( TYPE_TEXT );
			}


			pwcUprCustomFormat = DuplicateInUpperCase( pwcCustomFormat );

			// bg 261280
			removeNoisyTags( pwcUprCustomFormat );

			// Percentage token.
			pwcToken = wcsstr( pwcUprCustomFormat, L"%" );

			if( pwcToken != NULL )
			{
				delete [] pwcUprCustomFormat;
				return( TYPE_PERCENTAGE );
			}

			// Date tokens.
			for( i = 0; i < sizeof( apwcDateTokens ) / sizeof( apwcDateTokens[ 0 ] ); i++ )
			{
				pwcToken = wcsstr( pwcUprCustomFormat, apwcDateTokens[ i ] );

				if( pwcToken != NULL )
				{
					delete [] pwcUprCustomFormat;
					return( TYPE_DATE );
				}
			}

			// Shared tokens, date and time.
			pwcToken = wcsstr( pwcUprCustomFormat, L"M" );

			if( pwcToken != NULL )
			{
				pwcAuxToken1 = wcsstr( pwcUprCustomFormat, L"H" );
				pwcAuxToken2 = wcsstr( pwcUprCustomFormat, L"S" );

				delete [] pwcUprCustomFormat;

				if( ( pwcAuxToken1 == NULL || pwcAuxToken1 > pwcToken ) && ( pwcAuxToken2 == NULL || pwcAuxToken2 < pwcToken ) )
				{
					return( TYPE_DATE );
				}
				else
				{
					return( TYPE_TIME );
				}
			}

			// Time tokens.
			for( i = 0; i < sizeof( apwcTimeTokens ) / sizeof( apwcTimeTokens[ 0 ] ); i++ )
			{
				pwcToken = wcsstr( pwcUprCustomFormat, apwcTimeTokens[ i ] );

				if( pwcToken != NULL )
				{
					delete [] pwcUprCustomFormat;
					return( TYPE_TIME );
				}
			}

			delete [] pwcUprCustomFormat;
			return( TYPE_NUMBER );
	}
}


// -----------------------------------------------------------------------------
// Internal method that gives the last chance of changing the Meta4 target type
// of a cell, based on the original target type and the datum that is going to
// be set.
// 
//	ai_eTargetType:	Original target type.
//	ai_pwcM4Value:	String with the Meta4 formated value to write.
//	ai_pwcISOValue:	Optional string with the ISO formated value to write.
// -----------------------------------------------------------------------------
eTargetType	ClXlSheetAccessor::extrapolateTargetType( eTargetType ai_eTargetType, wchar_t* ai_pwcM4Value, wchar_t* ai_pwcISOValue )
{
	size_t		iLen;
	int			iAssigned, iYear, iMonth, iDay, iHour, iMin, iSec;
	wchar_t*	pwcValue;
	int			iDots = 0;


	if( ai_eTargetType != TYPE_GENERAL )
	{
		return ai_eTargetType;
	}

	// bg 292763.
	if( m_iExtrapolationMode == QUERY_MODE && ai_pwcISOValue == NULL )
	{
		// Do not extrapolate type if ISO value does not exists, because
		// extrapolable types as number or dates, arrives as ISO values.
		return TYPE_TEXT;
	}

	// Support for single value (empty ISO) extrapolation.
	if( ai_pwcISOValue != NULL )
	{
		pwcValue = ai_pwcISOValue;
	}
	else
	{
		pwcValue = ai_pwcM4Value;
	}

	iLen = wcslen( pwcValue );

	// Is it a date-time?
	if( iLen == 19 )
	{
		iAssigned = swscanf( pwcValue, L"%4d-%2d-%2d %2d:%2d:%2d", &iYear, &iMonth, &iDay, &iHour, &iMin, &iSec );
		if( iAssigned == 6 )
		{
			return TYPE_DATE_TIME;
		}
	}

	// Is it a date?
	if( iLen == 10 )
	{
		iAssigned = swscanf( pwcValue, L"%4d-%2d-%2d", &iYear, &iMonth, &iDay );
		if( iAssigned == 3 )
		{
			return TYPE_DATE;
		}
	}

	// Is it a time?
	if( iLen == 8 )
	{
		iAssigned = swscanf( pwcValue, L"%2d:%2d:%2d", &iHour, &iMin, &iSec );

		if( iAssigned == 3 )
		{
			return TYPE_TIME;
		}
	}

	// Is it a number?

	// bg 271996.
	if( pwcValue[ 0 ] == L'0' && iLen > 1 && pwcValue[ 1 ] != L'.' )
	{
		return TYPE_TEXT;
	}

	for( size_t i = 0; i < iLen; i++ )
	{
		if( iswdigit( pwcValue[ i ] ) == 0 && pwcValue[ i ] != '.' && ( pwcValue[ i ] != '-' || i > 0 ) )
		{
			return TYPE_TEXT;
		}

		if( pwcValue[ i ] == '.' )
		{
			if( ++iDots > 1 )
			{
				// Only one dot in a number. If more than one it could be a formated date.
				return TYPE_TEXT;
			}
		}
	}

	return TYPE_NUMBER;
}


// -----------------------------------------------------------------------------
// Internal method that gives the last chance of changing the Meta4 target type
// of a cell, based on the original target type and the datum that is going to
// be set.
// 
//	ai_eTargetType:	Original target type.
//	ai_pvValue:		Meta4 variant object with the value to write.
// -----------------------------------------------------------------------------
eTargetType	ClXlSheetAccessor::extrapolateTargetType( eTargetType ai_eTargetType, m4VariantType* ai_pvValue )
{
	m4int32_t	iHours = 0, iMins = 0, iSecs = 0;
	int			iYears = 0, iMonths = 0, iDays = 0;


	if( ai_eTargetType != TYPE_GENERAL )
	{
		return ai_eTargetType;
	}

	if( ai_pvValue->GetType() == M4CL_CPP_TYPE_NUMBER )
	{
		return TYPE_NUMBER;
	}
	else if( ai_pvValue->GetType() == M4CL_CPP_TYPE_DATE )
	{
		ClDateToNumbers( ai_pvValue->Data.DoubleData, iYears, iMonths, iDays, iHours, iMins, iSecs );

		if( iHours == 0 && iMins == 0 && iSecs == 0 )
		{
			return TYPE_DATE;
		}
		else
		{
			// Time type.
			if( IsDateForM4Time( iYears, iMonths, iDays ) == true )
			{
				return TYPE_TIME;
			}
			else
			{
				return TYPE_DATE_TIME;
			}
		}
		
	}

	return TYPE_TEXT;
}


// -----------------------------------------------------------------------------
// Internal method that overwrites all not meaningful characters in a number 
// format string, for obtaining the target type of its related cell.
// 
//	ai_pwcFormat:	Number format string.
// -----------------------------------------------------------------------------
void ClXlSheetAccessor::removeNoisyTags( wchar_t* ai_pwcFormat )
{
	wchar_t*	pwcTag;
	size_t		iLen;


	if( ai_pwcFormat == NULL )
	{
		return;
	}

	for( int i = 0; i < sizeof( apwcNoisyTags ) / sizeof( apwcNoisyTags[ 0 ] ); i++ )
	{
		pwcTag = wcsstr( ai_pwcFormat, apwcNoisyTags[ i ] );

		if( pwcTag != NULL )
		{
			iLen = wcslen( apwcNoisyTags[ i ] );
			
			for( int j = 0; j < iLen; j++ )
			{
				pwcTag[ j ] = 95;
			}
		}
	}
}


// -----------------------------------------------------------------------------
// Internal method that gets the xl packed date of a string ISO date.
// 
//	ai_pwcDate:		String ISO date.
//	ao_dPackedDate:	Output xl packed date
// -----------------------------------------------------------------------------
m4return_t ClXlSheetAccessor::getXlPackedDate( wchar_t* ai_pwcDate, double& ao_dPackedDate )
{
	m4return_t	iRet;
	int			iHours = 0, iMins = 0, iSecs = 0, iYears = 0, iMonths = 0, iDays = 0;

	
	ao_dPackedDate = 0;

	iRet = GetDateNumbers( ai_pwcDate, iYears, iMonths, iDays, iHours, iMins, iSecs );

	if( iRet == M4_SUCCESS )
	{
		ao_dPackedDate = m_pBook->datePack( iYears, iMonths, iDays, iHours, iMins, iSecs );
	}

	return iRet;
}


// -----------------------------------------------------------------------------
// Internal method that gets the xl packed time of a string ISO time.
// 
//	ai_pwcDate:		String ISO time.
//	ao_dPackedDate:	Output xl packed time.
// -----------------------------------------------------------------------------
m4return_t ClXlSheetAccessor::getXlPackedTime( wchar_t* ai_pwcTime, double& ao_dPackedTime )
{
	m4return_t	iRet;
	int			iHours = 0, iMins = 0, iSecs = 0;

	
	ao_dPackedTime = 0;

	iRet = GetTimeNumbers( ai_pwcTime, iHours, iMins, iSecs );

	if( iRet == M4_SUCCESS )
	{
		ao_dPackedTime = m_pBook->datePack( 0, 0, 0, iHours, iMins, iSecs );
	}

	return iRet;
}


// -----------------------------------------------------------------------------
// Internal method that returns the final target type based on the forced  
// target type
// 
//	ai_iCol:		Column index.
//	ai_pwcValue:	String with the value to be set.
// -----------------------------------------------------------------------------
eTargetType	ClXlSheetAccessor::getForcedTargetType( int ai_iCol, wchar_t* ai_pwcValue )
{
	size_t	iNumCols, iValueLen;


	iNumCols = m_pvForcedColTypes->size();
	if( ai_iCol >= iNumCols || ai_pwcValue == NULL )
	{
		// default.
		return TYPE_TEXT;
	}
	
	iValueLen = wcslen( ai_pwcValue );
	if( iValueLen == 0 )
	{
		// default.
		return TYPE_TEXT;
	}


	switch( m_pvForcedColTypes->at( ai_iCol ) )
	{
		case FCD_NO_TYPE:

			// default.
			return TYPE_TEXT;

		case FCD_NUMBER_TYPE:

			return TYPE_NUMBER;

		case FCD_STRING_TYPE:

			return TYPE_TEXT;

		case FCD_DATE_TYPE:

			if( ai_pwcValue[ 2 ] == ':' )
			{
				// backward compatibility.
				return TYPE_TIME;
			}
			return TYPE_DATE;

		case FCD_DATE_TIME_TYPE:

			return TYPE_DATE_TIME;

		case FCD_TIME_TYPE:

			return TYPE_TIME;

		default:

			// default.
			return TYPE_TEXT;
	}
}


// -----------------------------------------------------------------------------
// Gets a format object, based on a format template, for the target type specified. 
// 
//	ai_pTpltFormat:		Input template format.
//	ai_iTargetType:		Input target type of the format requested.
//	ai_pBook:			Input book object.
//	ai_pwcData:			Input value that will be set.
//	ao_pOutFormat:		Output format requested.
// -----------------------------------------------------------------------------
m4return_t	ClXlSheetAccessor::GetTargetFormat( Format* ai_pTpltFormat, eTargetType ai_iTargetType, Book* ai_pBook, wchar_t* ai_pwcData, Format*& ao_pOutFormat )
{
	int				iNumFormat;
	m4return_t		iRet;
	m4VariantType	vNumFormat;


	ao_pOutFormat = NULL;

	// The target number format for each target type is hard coded here.
	switch( ai_iTargetType )
	{
		case TYPE_DATE_TIME:
			iNumFormat = NUMFORMAT_CUSTOM_MDYYYY_HMM;
			break;

		case TYPE_DATE:
			iNumFormat = NUMFORMAT_DATE;
			break;

		case TYPE_TIME:
			iNumFormat = NUMFORMAT_CUSTOM_H0MMSS;
			break;

		case TYPE_NUMBER:
			iNumFormat = NUMFORMAT_NUMBER;

			if( ai_pwcData != NULL )
			{
				if( wcsstr( ai_pwcData, L"." ) != NULL )
				{
					iNumFormat = NUMFORMAT_NUMBER_D2;
				}
			}
			break;

		case TYPE_TEXT:
			iNumFormat = NUMFORMAT_TEXT;
			break;

		default:
			return M4_ERROR;
	}


	vNumFormat.Type = M4CL_CPP_TYPE_NUMBER;
	vNumFormat.Data.DoubleData = iNumFormat;

	iRet = ClXlFormatsMngr::GetSuitableFormat( ai_pTpltFormat, ClXlFormatsMngr::M4XL_CELL_TYPE, &vNumFormat, ai_pBook, m_pContext->m_pAccess, ao_pOutFormat );

	if( iRet == M4_ERROR || ao_pOutFormat == NULL )
	{
		return M4_ERROR;
	}


	return M4_SUCCESS;
}
