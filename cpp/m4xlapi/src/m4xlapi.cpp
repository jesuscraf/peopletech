//==============================================================================
//
// (c) Copyright  1991-2008 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4xlapi
// File:             m4xlapi.cpp
// Project:			 mind3.x
// Author:           Meta Software M.S. , S.A
// Date:             22/10/2013
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

#include <m4xlapi.hpp>
#include <m4xlhelper.hpp>
#include <m4xlformatsmngr.hpp>
#include <m4csvparser.hpp>
#include <m4xlapichannel.hpp>
#include <m4log.hpp>
#include <access.hpp>
#include <m4unicode.hpp>
#include <m4win2ux.hpp>


// -----------------------------------------------------------------------------
// Trace log
// -----------------------------------------------------------------------------

extern  m4bool_t s_bTraceEnabled;
#define TRACE_API_CALL(X, Y, Z) { if (s_bTraceEnabled == M4_TRUE){ ClAPICallTracer::TraceAPICall( X, Y, Z ); } }

// -----------------------------------------------------------------------------
// Public functions.
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Opens an existing Excel workbook.
// 
//	Arg_0: Path of the Excel file to open.
//	Arg_1: Optional. Temporary file used to improve the memory consumption.
// -----------------------------------------------------------------------------
m4return_t OpenWorkbook( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t		iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;
	wchar_t*		pwcFile = NULL;
	m4bool_t		bIsXML;
	Book*			pWorkbook;
	bool			bRet;
	wchar_t*		pwcTmpFile = NULL;

	
	CheckTraceMode(pContext->m_pAccess);
	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);
	
	// Checks function arguments.
	CHECK_ARG_NUM(ai_iLongArg, 2, "OpenWorkbook" )
	CHECK_ARG_TYPE(ai_pvArg[0].Type, M4CL_CPP_TYPE_STRING_VAR, "OpenWorkbook")


	// Determines the type of the file container to use.
	iRet = IsXMLWorkbook( ai_pvArg[0].Data.PointerChar, bIsXML );
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_OPEN_WB_BAD_FILE, ERRORLOG, "#*s1*#%s#", ai_pvArg[0].Data.PointerChar );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	if( bIsXML == M4_TRUE )
	{
		pWorkbook = xlCreateXMLBook();
	}
	else
	{
		pWorkbook = xlCreateBook();
	}

	if( pWorkbook == NULL )
	{
		SETCODEF( M4_XL_ERR_OPEN_WB_NULL_OBJ, ERRORLOG, "#*s1*#" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	iRet = CppStrToLibxlStr( ai_pvArg[0].Data.PointerChar, pwcFile );
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", "OpenWorkbook" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	// Temporal file used inside load function.
	if( ai_pvArg[1].Type == M4CL_CPP_TYPE_STRING_VAR )
	{
		iRet = CppStrToLibxlStr( ai_pvArg[1].Data.PointerChar, pwcTmpFile );

		if( iRet == M4_ERROR )
		{
			SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", "OpenWorkbook" );
			CH_RETURN( &ai_pvReturn, M4_ERROR )
		}
	}

	// Opens the file.
	bRet = pWorkbook->load( pwcFile, pwcTmpFile );

	delete [] pwcFile;
	if( pwcTmpFile != NULL )
	{
		delete [] pwcTmpFile;
	}
	
	if( bRet == false )
	{
		SETCODEF( M4_XL_ERR_OPEN_WB_LIB_ERR, ERRORLOG, "#*s1*#%s#%s#", pWorkbook->errorMessage(), ai_pvArg[0].Data.PointerChar );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	// Saves the xl object reference.
	APIChannel::SetItem(pContext->m_pAccess, APIChannel::WORKBOOKS, APIChannel::WORKBOOK_POINTER, pWorkbook);
	APIChannel::SetItem(pContext->m_pAccess, APIChannel::WORKBOOKS, APIChannel::WORKBOOK_PATH, ai_pvArg[0].Data.PointerChar);


	CH_RETURN( &ai_pvReturn, M4_SUCCESS )
}


// -----------------------------------------------------------------------------
// Closes the current Excel workbook.
// -----------------------------------------------------------------------------
m4return_t CloseWorkbook( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t		iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;
	Book*			pWorkbook;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Retrieves the Excel object.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKBOOKS, APIChannel::WORKBOOK_POINTER, ( void*& )pWorkbook);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WB_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "CloseWorkbook" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	// Releases the file.
	pWorkbook->release();


	// Removes the object pointer saved in the store.
	iRet = APIChannel::ErasePointerItem(pContext->m_pAccess, APIChannel::WORKBOOKS, APIChannel::WORKBOOK_POINTER);
	if( iRet == M4_ERROR )
	{
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	APIChannel::SetItem(pContext->m_pAccess, APIChannel::WORKBOOKS, APIChannel::WORKBOOK_PATH);

	CH_RETURN( &ai_pvReturn, M4_SUCCESS )
}


// -----------------------------------------------------------------------------
// Saves the current Excel workbook.
//
//	Arg_0: Use temporary file.
// -----------------------------------------------------------------------------
m4return_t SaveWorkbook( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t		iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;
	Book*			pWorkbook;
	m4pchar_t		pcPath;
	m4bool_t		bIsTemplate;
	wchar_t*		pwcFile = NULL;
	bool			bRet;

	
	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks function arguments.
	CHECK_ARG_NUM(ai_iLongArg, 1, "SaveWorkbook" )
	CHECK_ARG_TYPE(ai_pvArg[0].Type, M4CL_CPP_TYPE_NUMBER, "SaveWorkbook")

	// Retrieves the Excel object.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKBOOKS, APIChannel::WORKBOOK_POINTER, ( void*& )pWorkbook);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WB_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "SaveWorkbook" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKBOOKS, APIChannel::WORKBOOK_PATH, ( m4pchar_t& )pcPath);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_SAVE_WB_BAD_PATH, ERRORLOG, "#*s1*#" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	// Determines if the target file must be a template.
	iRet = IsTemplateWorkbook( pcPath, bIsTemplate );
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_NEW_WB_BAD_FILE, ERRORLOG, "#*s1*#%s#", pcPath );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	// Refreshes the template flag.
	if( bIsTemplate == M4_TRUE )
	{
		pWorkbook->setTemplate( true );
	}


	iRet = CppStrToLibxlStr( pcPath, pwcFile );
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", "SaveWorkbook" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	// Saves the file.
	bRet = pWorkbook->save( pwcFile, (bool)ai_pvArg[0].Data.DoubleData );
	
	delete [] pwcFile;

	if( bRet == false )
	{
		SETCODEF( M4_XL_ERR_SAVE_WB_LIB_ERR, ERRORLOG, "#*s1*#%s#%s#", pWorkbook->errorMessage(), pcPath );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	CH_RETURN( &ai_pvReturn, M4_SUCCESS )
}


// -----------------------------------------------------------------------------
// Retrieves a worksheet within the current Excel workbook.
// 
//	Arg_0: Worksheet index to open or worksheet name to open.
// -----------------------------------------------------------------------------
m4return_t GetWorksheet( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	int				iSheetIndex = -1;
	char*			pcSheetName;
	m4return_t		iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;
	Book*			pWorkbook;
	Sheet*			pWorksheet;
	int				iSheetCount;
	char*			pcCurrName;
	m4bool_t		bDelete = M4_FALSE;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks function arguments.
	CHECK_ARG_NUM(ai_iLongArg, 1, "GetWorksheet" )


	// A sheet index was given.
	if( ai_pvArg[0].Type == M4CL_CPP_TYPE_NUMBER ) 
	{
		iSheetIndex = ai_pvArg[0].Data.DoubleData;
		if( iSheetIndex < 0 )
		{
			char acBuffer[20];
			SETCODEF( M4_XL_ERR_OPEN_WS_BAD_INDEX, ERRORLOG, "#*s1*#%s#", itoa( iSheetIndex, acBuffer, 10 ) );
			CH_RETURN( &ai_pvReturn, M4_ERROR )
		}
	}
	else if( ai_pvArg[0].Type == M4CL_CPP_TYPE_STRING_VAR ) 
	{
		// A sheet name was given.
		pcSheetName = ai_pvArg[0].Data.PointerChar;
		if( pcSheetName == NULL || strlen( pcSheetName ) == 0 )
		{
			SETCODEF( M4_XL_ERR_OPEN_WS_BAD_INDEX, ERRORLOG, "#*s1*#%s#", "" );
			CH_RETURN( &ai_pvReturn, M4_ERROR )
		}
	}
	else
	{
		// Nothing to do.
		SETCODEF( M4_XL_ERR_BAD_ARGUMENTS, ERRORLOG, "#*s1*#%s#", "GetWorksheet" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	// Retrieves the Excel object.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKBOOK_POINTER, ( void*& )pWorkbook);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WB_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "GetWorksheet" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	// Gets the worksheet by index.
	if( iSheetIndex != -1 )
	{
		pWorksheet = pWorkbook->getSheet( iSheetIndex );
		if( pWorksheet == NULL )
		{
			SETCODEF( M4_XL_ERR_OPEN_WS_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#", pWorkbook->errorMessage(), iSheetIndex );
			CH_RETURN( &ai_pvReturn, M4_ERROR )
		}

		iRet = LibxlStrToCppStr( pWorksheet->name(), pcSheetName );
		if( iRet == M4_ERROR )
		{
			SETCODEF( M4_XL_ERR_GEN_XL_2_CPP, ERRORLOG, "#*s1*#%s#", "GetWorksheet" );
			CH_RETURN( &ai_pvReturn, M4_ERROR )
		}

		bDelete = M4_TRUE;
	}
	else
	{
		// Gets the worksheet by name.
		iSheetCount = pWorkbook->sheetCount();

		for( int i = 0; i < iSheetCount; i++ )
		{
			pWorksheet = pWorkbook->getSheet( i );
			if( pWorksheet == NULL )
			{
				SETCODEF( M4_XL_ERR_OPEN_WS_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#", pWorkbook->errorMessage(), i );
				CH_RETURN( &ai_pvReturn, M4_ERROR )
			}

			iRet = LibxlStrToCppStr( pWorksheet->name(), pcCurrName );
			if( iRet == M4_ERROR )
			{
				SETCODEF( M4_XL_ERR_GEN_XL_2_CPP, ERRORLOG, "#*s1*#%s#", "GetWorksheet" );
				CH_RETURN( &ai_pvReturn, M4_ERROR )
			}

			if( M4StrCmpi( pcSheetName, pcCurrName ) == 0 )
			{
				iSheetIndex = i;
				delete [] pcCurrName;
				break;
			}

			delete [] pcCurrName;
		}

		if( iSheetIndex == -1 )
		{
			SETCODEF( M4_XL_ERR_OPEN_WS_BAD_INDEX, ERRORLOG, "#*s1*#%s#", pcSheetName );
			CH_RETURN( &ai_pvReturn, M4_ERROR )
		}
	}


	// Saves the xl object reference.
	APIChannel::SetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, pWorksheet);
	APIChannel::SetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_INDEX, iSheetIndex);
	APIChannel::SetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_NAME, pcSheetName);


	if( bDelete == M4_TRUE )
	{
		delete [] pcSheetName;
	}


	CH_RETURN( &ai_pvReturn, M4_SUCCESS )
}


// -----------------------------------------------------------------------------
// Converts a cell reference to row and column.
// 
//	Arg_0: Cell reference.
//	Arg_1: Output row number.
//	Arg_2: Output column number.
// -----------------------------------------------------------------------------
m4return_t GetRowCol( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t		iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;
	Sheet*			pWorksheet;
	wchar_t*		pwcCelRef = NULL;
	int				iRow, iCol;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks function arguments.
	CHECK_ARG_NUM(ai_iLongArg, 3, "GetRowCol" )
	CHECK_ARG_TYPE(ai_pvArg[0].Type, M4CL_CPP_TYPE_STRING_VAR, "GetRowCol")
	CHECK_ARG_TYPE(ai_pvArg[1].Type, M4CL_CPP_TYPE_NUMBER, "GetRowCol")
	CHECK_ARG_TYPE(ai_pvArg[2].Type, M4CL_CPP_TYPE_NUMBER, "GetRowCol")

	
	// Retrieves the Excel object.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, ( void*& )pWorksheet);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WS_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "GetRowCol" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	iRet = CppStrToLibxlStr( ai_pvArg[0].Data.PointerChar, pwcCelRef );
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", "GetRowCol" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	// Gets the cell coordinates.
	pWorksheet->addrToRowCol( pwcCelRef, &iRow, &iCol );

	ai_pvArg[1].SetDouble( iRow );
	ai_pvArg[2].SetDouble( iCol );

	delete [] pwcCelRef;


	CH_RETURN( &ai_pvReturn, M4_SUCCESS )
}


// -----------------------------------------------------------------------------
// Imports a CSV file into the current worksheet. 
// 
//	Arg_0: Starting row number for the merge process.
//	Arg_1: Starting column number for the merge process.
//	Arg_2: Path of the csv file to merge.
//	Arg_3: Csv values delimiter.
//	Arg_4: Optional. Extrapolation mode.
// -----------------------------------------------------------------------------
m4return_t ImportCSV( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t			iRet;
	ClVMRunContext*		pContext = ( ClVMRunContext* )ai_pvContext;
	Book*				pWorkbook;
	Sheet*				pWorksheet;
	ClXlSheetAccessor*	pSheetWriter;
	ClM4CsvParser*		pCSVParser;
	int					iLength = -1;
	char*				pcCsvFile;

	
	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks function arguments.
	//CHECK_ARG_NUM(ai_iLongArg, 4, "ImportCSV" )
	CHECK_ARG_TYPE(ai_pvArg[0].Type, M4CL_CPP_TYPE_NUMBER, "ImportCSV")
	CHECK_ARG_TYPE(ai_pvArg[1].Type, M4CL_CPP_TYPE_NUMBER, "ImportCSV")
	CHECK_ARG_TYPE(ai_pvArg[2].Type, M4CL_CPP_TYPE_STRING_VAR, "ImportCSV")
	CHECK_ARG_TYPE(ai_pvArg[3].Type, M4CL_CPP_TYPE_STRING_VAR, "ImportCSV")


	// Retrieves the Excel objects.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKBOOK_POINTER, ( void*& )pWorkbook);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WB_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "ImportCSV" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, ( void*& )pWorksheet);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WS_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "ImportCSV" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	// Builds an Excel sheet writer.
	pSheetWriter = new ClXlSheetAccessor( ai_pvArg[0].Data.DoubleData, ai_pvArg[1].Data.DoubleData, pWorkbook, pWorksheet, pContext );

	// Sets the type extrapolation mode.
	if( ai_iLongArg == 5 )
	{
		CHECK_ARG_TYPE(ai_pvArg[4].Type, M4CL_CPP_TYPE_NUMBER, "ImportCSV")
		pSheetWriter->SetTypeExtrapolationMode( ( eTypeExtrapolationMode )( int ) ai_pvArg[4].Data.DoubleData );
	}
	
	// Builds the csv parser.
	pCSVParser = new ClM4CsvParser( pSheetWriter );

	// Loads the csv file to merge.
	pcCsvFile = M4CppToANSI( ai_pvArg[2].Data.PointerChar, iLength );
	iRet = pCSVParser->SetCsvFile( pcCsvFile, *ai_pvArg[3].Data.PointerChar );

	delete [] pcCsvFile;

	if( iRet == M4_SUCCESS )
	{
		// Merges the csv file.
		iRet = pCSVParser->Parse();

		if( iRet == M4_ERROR )
		{
			SETCODEF( M4_XL_ERR_PARS_GEN_ERR, ERRORLOG, "#*s1*#" );
		}
	}

	delete pCSVParser;
	delete pSheetWriter;

	
	CH_RETURN( &ai_pvReturn, iRet )
}


// -----------------------------------------------------------------------------
// Reads a cell value.
// 
//	Arg_0: Cell row number.
//	Arg_1: Cell column number.
//	Arg_2: Output variant value.
// -----------------------------------------------------------------------------
m4return_t GetCellValue( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t				iRet;
	ClVMRunContext*			pContext = ( ClVMRunContext* )ai_pvContext;
	Book*					pWorkbook;
	Sheet*					pWorksheet;
	int						iRow, iCol;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks function arguments.
	CHECK_ARG_NUM(ai_iLongArg, 3, "GetCellValue" )
	CHECK_ARG_TYPE(ai_pvArg[0].Type, M4CL_CPP_TYPE_NUMBER, "GetCellValue")
	CHECK_ARG_TYPE(ai_pvArg[1].Type, M4CL_CPP_TYPE_NUMBER, "GetCellValue")


	// Retrieves the Excel objects.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKBOOK_POINTER, ( void*& )pWorkbook);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WB_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "GetCellValue" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, ( void*& )pWorksheet);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WS_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "GetCellValue" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	
	// Gets the cell to read coordinates.
	iRow = ai_pvArg[0].Data.DoubleData;
	iCol = ai_pvArg[1].Data.DoubleData;

	
	// Builds an Excel sheet reader and reads the cell value.
	ClXlSheetAccessor oSheetReader( 0, 0, pWorkbook, pWorksheet, pContext );
	iRet = oSheetReader.ReadValue( iRow, iCol, &ai_pvArg[2] );


	CH_RETURN( &ai_pvReturn, iRet )
}


// -----------------------------------------------------------------------------
// Writes a cell value.
// 
//	Arg_0: Cell row number.
//	Arg_1: Cell column number.
//	Arg_2: Value to set.
// -----------------------------------------------------------------------------
m4return_t SetCellValue( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t			iRet;
	ClVMRunContext*		pContext = ( ClVMRunContext* )ai_pvContext;
	Book*				pWorkbook;
	Sheet*				pWorksheet;
	int					iRow, iCol;

	
	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks function arguments.
	CHECK_ARG_NUM(ai_iLongArg, 3, "SetCellValue" )
	CHECK_ARG_TYPE(ai_pvArg[0].Type, M4CL_CPP_TYPE_NUMBER, "SetCellValue")
	CHECK_ARG_TYPE(ai_pvArg[1].Type, M4CL_CPP_TYPE_NUMBER, "SetCellValue")


	// Retrieves the Excel objects.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKBOOK_POINTER, ( void*& )pWorkbook);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WB_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "GetCellValue" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, ( void*& )pWorksheet);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WS_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "GetCellValue" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	
	// Gets the cell to write coordinates.
	iRow = ai_pvArg[0].Data.DoubleData;
	iCol = ai_pvArg[1].Data.DoubleData;

	
	// Builds an Excel sheet writer and writes the cell value.
	ClXlSheetAccessor oSheetWriter( 0, 0, pWorkbook, pWorksheet, pContext );
	iRet = oSheetWriter.WriteValue( iRow, iCol, &ai_pvArg[2] );


	CH_RETURN( &ai_pvReturn, iRet )
}


// -----------------------------------------------------------------------------
// Gets the last cell written in the current worksheet.
// 
//	Arg_0: Output row number.
//	Arg_1: Output column number.
// -----------------------------------------------------------------------------
m4return_t GetLastCell( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t		iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;
	Sheet*			pWorksheet;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks function arguments.
	CHECK_ARG_NUM(ai_iLongArg, 2, "GetLastCell" )

	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, ( void*& )pWorksheet);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WS_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "GetLastCell" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	// Sets the last cell coordinates.
	ai_pvArg[0].SetDouble( pWorksheet->lastRow() );
	ai_pvArg[1].SetDouble( pWorksheet->lastCol() );


	CH_RETURN( &ai_pvReturn, M4_SUCCESS )
}


// -----------------------------------------------------------------------------
// Writes the content of an Excel range into a M4Object node.
// 
//	Arg_0: Cell where the range starts.
//	Arg_1: Cell where the range ends.
//	Arg_2: Handle of the M4Object instance where dump the range.
//	Arg_3: Node name where dump the range.
//	Arg_4: Column number where de range starts.
//	Arg_5: Row number where de range starts.
//	Arg_6: Number of rows to scan looking for not empty values (-1: Scan full range).
//	Arg_7: Output row index where found first not empty value.
//	Arg_8: Output column index where found first not empty value.
// -----------------------------------------------------------------------------
m4return_t DumpRangeToNode( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t			iRet;
	ClVMRunContext*		pContext = ( ClVMRunContext* )ai_pvContext;
	ClChannel*			pChannel;
	ClAccess*			poAccess;
	ClNode*				poNode;
	Book*				pWorkbook;
	Sheet*				pWorksheet;
	wchar_t*			pwcCelRef = NULL;
	int					iStartRow, iStartCol, iEndRow, iEndCol;
	m4VariantType		vValue;
	long				iRowsToScan, iRemaining;
	m4bool_t			bScanning = false;
	
	
	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks function arguments.
	CHECK_ARG_NUM(ai_iLongArg, 9, "DumpRangeToNode" )
	CHECK_ARG_TYPE(ai_pvArg[0].Type, M4CL_CPP_TYPE_STRING_VAR, "DumpRangeToNode")
	CHECK_ARG_TYPE(ai_pvArg[1].Type, M4CL_CPP_TYPE_STRING_VAR, "DumpRangeToNode")
	CHECK_ARG_TYPE(ai_pvArg[2].Type, M4CL_CPP_TYPE_NUMBER, "DumpRangeToNode")
	CHECK_ARG_TYPE(ai_pvArg[3].Type, M4CL_CPP_TYPE_STRING_VAR, "DumpRangeToNode")
	CHECK_ARG_TYPE(ai_pvArg[4].Type, M4CL_CPP_TYPE_NUMBER, "DumpRangeToNode")
	CHECK_ARG_TYPE(ai_pvArg[5].Type, M4CL_CPP_TYPE_NUMBER, "DumpRangeToNode")
	CHECK_ARG_TYPE(ai_pvArg[6].Type, M4CL_CPP_TYPE_NUMBER, "DumpRangeToNode")
	CHECK_ARG_TYPE(ai_pvArg[7].Type, M4CL_CPP_TYPE_NUMBER, "DumpRangeToNode")
	CHECK_ARG_TYPE(ai_pvArg[8].Type, M4CL_CPP_TYPE_NUMBER, "DumpRangeToNode")


	// Gets the target channel instance.
	pChannel = pContext->m_pAccess->GetpChannel()->GetpChannelManager()->GetChannelByHandle( ai_pvArg[2].Data.DoubleData );
	if( pChannel == NULL )
	{
		SETCODEF( M4_XL_ERR_DTN_BAD_INSTANCE, ERRORLOG, "#*s1*#%d#", ( int )ai_pvArg[2].Data.DoubleData );
		CH_RETURN( &ai_pvReturn, M4_SUCCESS )
	}

	poAccess = pChannel->CreateAccess();
	if( poAccess == NULL )
	{
		SETCODEF( M4_XL_ERR_DTN_BAD_INSTANCE, ERRORLOG, "#*s1*#%d#", ( int )ai_pvArg[2].Data.DoubleData );
		CH_RETURN( &ai_pvReturn, M4_SUCCESS )
	}

	ClObjTerminator oTerminator( poAccess );

	poNode = &( poAccess->Node[ ai_pvArg[3].Data.PointerChar ] );
	if( poNode == NULL )
	{
		SETCODEF( M4_XL_ERR_DTN_BAD_NODE, ERRORLOG, "#*s1*#%s#", ( int )ai_pvArg[3].Data.DoubleData );
		CH_RETURN( &ai_pvReturn, M4_SUCCESS )
	}
	

	// Retrieves the Excel objects.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKBOOK_POINTER, ( void*& )pWorkbook);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WB_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "DumpRangeToNode" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, ( void*& )pWorksheet);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WS_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "DumpRangeToNode" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	// Gets range to dump limits.
	iRet = CppStrToLibxlStr( ai_pvArg[0].Data.PointerChar, pwcCelRef );
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", "DumpRangeToNode" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	pWorksheet->addrToRowCol( pwcCelRef, &iStartRow, &iStartCol );
	delete [] pwcCelRef;

	iRet = CppStrToLibxlStr( ai_pvArg[1].Data.PointerChar, pwcCelRef );
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", "DumpRangeToNode" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	pWorksheet->addrToRowCol( pwcCelRef, &iEndRow, &iEndCol );
	delete [] pwcCelRef;


	// -- Pre-calculate all needed Excel column names.
	ClExcelColNames oExcelColNames( iEndCol + 1 );


	// -- Extended behavior
	iRowsToScan = 0;

	if( ai_pvArg[6].Data.DoubleData >= -1 )
	{
		iRowsToScan = ai_pvArg[6].Data.DoubleData;
	}

	iRemaining = iRowsToScan;


	// Builds an Excel sheet reader.
	ClXlSheetAccessor oSheetReader( 0, 0, pWorkbook, pWorksheet, pContext );

	// Initializes output arguments.
	ai_pvArg[7].Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvArg[7].Data.DoubleData = -1;

	ai_pvArg[8].Type = M4CL_CPP_TYPE_NUMBER;
	ai_pvArg[8].Data.DoubleData = -1;

		
	// Main rows iteration.
	for( long lRowIdx = iStartRow; lRowIdx <= iEndRow; lRowIdx++ )
	{
		//	Non-empty cell was not found in the specified number of rows. 
		if( bScanning == true && iRemaining == 0 )
		{
			CH_RETURN( &ai_pvReturn, M4_SUCCESS )
		}

		if( poNode->RecordSet.Register.Add() != M4_SUCCESS)
		{
			SETCODEF( M4_XL_ERR_DTN_ADD_REM_REGS, ERRORLOG, "#*s1*#" );
			CH_RETURN( &ai_pvReturn, M4_ERROR )
		}
		
		// Columns iteration.
		for( long lColIdx = iStartCol; lColIdx <= iEndCol; lColIdx++ )
		{
			iRet = oSheetReader.ReadValue( lRowIdx, lColIdx, &vValue );

			if( iRet == M4_ERROR )
			{
				CH_RETURN( &ai_pvReturn, M4_ERROR )
			}

			if( vValue.Type == M4CL_CPP_TYPE_NULL || ( vValue.Type == M4CL_CPP_TYPE_STRING_VAR && ( vValue.Data.PointerChar == NULL || ( vValue.Data.PointerChar != NULL && *(vValue.Data.PointerChar) == NULL ) ) ) )
			{
				// Checks for end condition.
				if( lColIdx == iStartCol )
				{
					// Removes the last register.
					if( poNode->RecordSet.Register.Destroy() != M4_SUCCESS)
					{
						SETCODEF( M4_XL_ERR_DTN_ADD_REM_REGS, ERRORLOG, "#*s1*#" );
						CH_RETURN( &ai_pvReturn, M4_ERROR )
					}

					// Basic behavior (not scanning).
					if( iRowsToScan == 0 )
					{
						CH_RETURN( &ai_pvReturn, M4_SUCCESS )
					}
					else
					{
						// It was specified to continue scanning looking for the first not empty cell:
						// in a fixed number of rows (n) or traversing the whole range (-1).
						bScanning = true;
					}

					// Decreases the number of rows to scan when specified a fixed number.
					if( iRowsToScan != -1 )
					{
						--iRemaining;
					}
				}
			}
			else
			{
				// Found non-empty cell, if scanning ...
				if( bScanning == true )
				{
					// ... sets the output arguments.
					ai_pvArg[7].Data.DoubleData = lRowIdx + 1;
					ai_pvArg[8].Data.DoubleData = lColIdx + 1;

					// Removes the last register.
					if( lColIdx == iStartCol )
					{
						if( poNode->RecordSet.Register.Destroy() != M4_SUCCESS)
						{
							SETCODEF( M4_XL_ERR_DTN_ADD_REM_REGS, ERRORLOG, "#*s1*#" );
							CH_RETURN( &ai_pvReturn, M4_ERROR )
						}
					}

					CH_RETURN( &ai_pvReturn, M4_SUCCESS )
				}
				else
				{
					// Trims data in case of string type.
					if( vValue.Type == M4CL_CPP_TYPE_STRING_VAR && vValue.Data.PointerChar != NULL && *(vValue.Data.PointerChar) != NULL )
					{
						TrimValue( vValue.Data.PointerChar );
					}

					// Sets the item value.
					if( poNode->RecordSet.Register.Item[ oExcelColNames.GetColName( lColIdx ) ].Value.Set( vValue ) != M4_SUCCESS )
					{
						SETCODEF( M4_XL_ERR_DTN_SET_ITEM_VALUE, ERRORLOG, "#*s1*#%s#", oExcelColNames.GetColName( lColIdx ) );
						CH_RETURN( &ai_pvReturn, M4_ERROR )
					}
				}
			}
		}

	}


	CH_RETURN( &ai_pvReturn, M4_SUCCESS )
}


// -----------------------------------------------------------------------------
// Writes the values stored in a CSV string into the current worksheet.
// 
//	Arg_0: csv string to merge.
//	Arg_1: Csv values delimiter.
//	Arg_2: Starting row number for the merge process.
//	Arg_3: Starting column number for the merge process.
// -----------------------------------------------------------------------------
m4return_t PasteSpecialText(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	m4return_t			iRet;
	ClVMRunContext*		pContext = ( ClVMRunContext* )ai_pvContext;
	Book*				pWorkbook;
	Sheet*				pWorksheet;
	ClXlSheetAccessor*	pSheetWriter;
	ClM4CsvParser*		pCSVParser;
	char				iValTerminator;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks function arguments.
	CHECK_ARG_NUM(ai_iLongArg, 4, "PasteSpecialText" )
	CHECK_ARG_TYPE(ai_pvArg[0].Type, M4CL_CPP_TYPE_STRING_VAR, "PasteSpecialText")
	//CHECK_ARG_TYPE(ai_pvArg[1].Type, M4CL_CPP_TYPE_NUMBER, "PasteSpecialText")	// Import tool sends a string data.
	CHECK_ARG_TYPE(ai_pvArg[2].Type, M4CL_CPP_TYPE_NUMBER, "PasteSpecialText")
	CHECK_ARG_TYPE(ai_pvArg[3].Type, M4CL_CPP_TYPE_NUMBER, "PasteSpecialText")


	// Retrieves the Excel objects.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKBOOK_POINTER, ( void*& )pWorkbook);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WB_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "PasteSpecialText" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, ( void*& )pWorksheet);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WS_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "PasteSpecialText" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	// Builds an Excel sheet writer.
	pSheetWriter = new ClXlSheetAccessor( ai_pvArg[2].Data.DoubleData, ai_pvArg[3].Data.DoubleData, pWorkbook, pWorksheet, pContext );

	// Builds the csv parser.
	pCSVParser	 = new ClM4CsvParser( pSheetWriter );

	if( ai_pvArg[1].Type == M4CL_CPP_TYPE_NUMBER )
	{
		iValTerminator = ai_pvArg[1].Data.DoubleData;
	}
	else
	{
		iValTerminator = *ai_pvArg[1].Data.PointerChar;
	}

	// Sets the csv string to merge.
	iRet = pCSVParser->SetCsvString( ai_pvArg[0].Data.PointerChar, iValTerminator );
	
	if( iRet == M4_SUCCESS )
	{
		// Merges the csv string.
		iRet = pCSVParser->Parse();

		if( iRet == M4_ERROR )
		{
			SETCODEF( M4_XL_ERR_PARS_GEN_ERR, ERRORLOG, "#*s1*#" );
		}
	}

	delete pCSVParser;
	delete pSheetWriter;


	CH_RETURN( &ai_pvReturn, M4_SUCCESS )
}


// -----------------------------------------------------------------------------
// Creates a new Excel workbook file. Java implementation compatibility.
// 
//	Arg_0: Path of the new Excel file.
// -----------------------------------------------------------------------------
m4return_t NewWorkbookFile( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t		iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;
	wchar_t*		pwcFile = NULL;
	m4bool_t		bIsXML;
	Book*			pWorkbook;
	bool			bRet;
	m4bool_t		bIsTemplate;

	
	CheckTraceMode(pContext->m_pAccess);
	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks function arguments.
	CHECK_ARG_NUM(ai_iLongArg, 1, "NewWorkbookFile" )
	CHECK_ARG_TYPE(ai_pvArg[0].Type, M4CL_CPP_TYPE_STRING_VAR, "NewWorkbookFile")


	// Determines the type of the file container to use.
	iRet = IsXMLWorkbook( ai_pvArg[0].Data.PointerChar, bIsXML );
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_NEW_WB_BAD_FILE, ERRORLOG, "#*s1*#%s#", ai_pvArg[0].Data.PointerChar );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	// Creates the new workbook.
	if( bIsXML == M4_TRUE )
	{
		pWorkbook = xlCreateXMLBook();
	}
	else
	{
		pWorkbook = xlCreateBook();
	}

	if( pWorkbook == NULL )
	{
		SETCODEF( M4_XL_ERR_NEW_WB_NULL_OBJ, ERRORLOG, "#*s1*#" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	
	// Saves the xl object reference.
	APIChannel::SetItem(pContext->m_pAccess, APIChannel::WORKBOOKS, APIChannel::WORKBOOK_POINTER, pWorkbook);
	APIChannel::SetItem(pContext->m_pAccess, APIChannel::WORKBOOKS, APIChannel::WORKBOOK_PATH, ai_pvArg[0].Data.PointerChar);


	// Determines if the target file must be a template.
	iRet = IsTemplateWorkbook( ai_pvArg[0].Data.PointerChar, bIsTemplate );
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_NEW_WB_BAD_FILE, ERRORLOG, "#*s1*#%s#", ai_pvArg[0].Data.PointerChar );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	// Refreshes the template flag.
	if( bIsTemplate == M4_TRUE )
	{
		pWorkbook->setTemplate( true );
	}


	iRet = CppStrToLibxlStr( ai_pvArg[0].Data.PointerChar, pwcFile );
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", "NewWorkbookFile" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	// Saves the new workbook.
	bRet = pWorkbook->save( pwcFile );

	delete [] pwcFile;

	if( bRet == false )
	{
		SETCODEF( M4_XL_ERR_NEW_WB_LIB_ERR, ERRORLOG, "#*s1*#%s#%s#", pWorkbook->errorMessage(), ai_pvArg[0].Data.PointerChar );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	CH_RETURN( &ai_pvReturn, M4_SUCCESS )
}


// -----------------------------------------------------------------------------
// Adds a new Excel worksheet.
// 
//	Arg_0:	optional. Name of the new worksheet.
//	Arg_1:	Optional. Index of the new worksheet.
//	Arg_2:	Optional. Worksheet from which to copy (handle).
// -----------------------------------------------------------------------------
m4return_t AddWorksheet( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t		iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;
	Book*			pWorkbook;
	wchar_t*		pwcSheetName = NULL;
	int				iSheetIndex;
	Sheet*			pWorksheet;
	char*			pcSheetName;
	char			acSheetName[ 32 ];
	Sheet*			pInitWsheet = NULL;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks function arguments.
	CHECK_ARG_NUM(ai_iLongArg, 3, "AddWorksheet" )

	// Optional argument. Default: auto naming.
	// CHECK_ARG_TYPE(ai_pvArg[0].Type, M4CL_CPP_TYPE_STRING_VAR, "AddWorksheet")
	
	// Optional argument. Default: append sheet.
	//CHECK_ARG_TYPE(ai_pvArg[1].Type, M4CL_CPP_TYPE_NUMBER, "AddWorksheet")
	
	// Optional argument. Default: Not copying.
	// CHECK_ARG_TYPE(ai_pvArg[2].Type, M4CL_CPP_TYPE_NUMBER, "AddWorksheet")


	// Retrieves the Excel object.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKBOOK_POINTER, ( void*& )pWorkbook);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WB_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "AddWorksheet" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	// Sheet name.
	if( ai_pvArg[0].Type == M4CL_CPP_TYPE_STRING_VAR && *ai_pvArg[0].Data.PointerChar != '\0' )
	{
		pcSheetName = ai_pvArg[0].Data.PointerChar;
	}
	else
	{
		// Builds the sheet name.
		iSheetIndex	= pWorkbook->sheetCount();
		sprintf( acSheetName, "Sheet%d", iSheetIndex + 1 );
		pcSheetName = acSheetName;
	}

	iRet = CppStrToLibxlStr( pcSheetName, pwcSheetName );
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", "AddWorksheet" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	SanitizeSheetName( pwcSheetName );


	// Insertion index.
	if( ai_pvArg[1].Type == M4CL_CPP_TYPE_NUMBER )
	{
		iSheetIndex = ai_pvArg[1].Data.DoubleData;
	}
	else
	{
		iSheetIndex = -1;
	}


	// Uses a sheet as source of the copy.
	if( ai_pvArg[2].Type == M4CL_CPP_TYPE_NUMBER && ai_pvArg[2].Data.DoubleData != 0 )
	{
		pInitWsheet = ( Sheet* )( pContext->m_pAccess->GetpChannel()->GetPointer( ai_pvArg[2].Data.DoubleData ) );
	}

	
	// Adds de new the worksheet.
	if( iSheetIndex == -1 )
	{
		iSheetIndex	= pWorkbook->sheetCount();
		pWorksheet	= pWorkbook->addSheet( pwcSheetName, pInitWsheet );
	}
	else
	{
		pWorksheet = pWorkbook->insertSheet( iSheetIndex, pwcSheetName, pInitWsheet );
	}

	delete [] pwcSheetName;


	if( pWorksheet == NULL )
	{
		SETCODEF( M4_XL_ERR_NEW_WS_LIB_ERR, ERRORLOG, "#*s1*#%s#%s#", pWorkbook->errorMessage(), ai_pvArg[0].Data.PointerChar );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	// Saves the xl object reference.
	APIChannel::SetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, pWorksheet);
	APIChannel::SetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_INDEX, iSheetIndex);
	APIChannel::SetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_NAME, ai_pvArg[0].Data.PointerChar);


	CH_RETURN( &ai_pvReturn, M4_SUCCESS )
}


// -----------------------------------------------------------------------------
// Copies a cell into another.
//
//	Arg_0:	Source row id.
//	Arg_1:	Source col id.
//	Arg_2:	Target row id.
//	Arg_3:	Target col id.
// -----------------------------------------------------------------------------
m4return_t CopyCell( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t		iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;
	Book*			pWorkbook;
	Sheet*			pWorksheet;
	int				iSrcRow, iSrcCol, iTgtRow, iTgtCol;
	bool			bRet;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks function arguments.
	CHECK_ARG_NUM(ai_iLongArg, 4, "CopyCell" )
	CHECK_ARG_TYPE(ai_pvArg[0].Type, M4CL_CPP_TYPE_NUMBER, "CopyCell")
	CHECK_ARG_TYPE(ai_pvArg[1].Type, M4CL_CPP_TYPE_NUMBER, "CopyCell")
	CHECK_ARG_TYPE(ai_pvArg[2].Type, M4CL_CPP_TYPE_NUMBER, "CopyCell")
	CHECK_ARG_TYPE(ai_pvArg[3].Type, M4CL_CPP_TYPE_NUMBER, "CopyCell")


	// Retrieves the Excel objects.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKBOOK_POINTER, ( void*& )pWorkbook);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WB_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "CopyCell" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, ( void*& )pWorksheet);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WS_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "CopyCell" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	iSrcRow = ai_pvArg[0].Data.DoubleData;
	iSrcCol = ai_pvArg[1].Data.DoubleData;
	iTgtRow = ai_pvArg[2].Data.DoubleData;
	iTgtCol = ai_pvArg[3].Data.DoubleData;

	// Copies the cell
	bRet = pWorksheet->copyCell( iSrcRow, iSrcCol, iTgtRow, iTgtCol );

	if( bRet == false )
	{
		SETCODEF( M4_XL_ERR_COPY_CELL_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#%d#%d#%d#", pWorkbook->errorMessage(), iSrcRow, iSrcCol, iTgtRow, iTgtCol );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	CH_RETURN( &ai_pvReturn, M4_SUCCESS )
}


// -----------------------------------------------------------------------------
// Copies a row into another.
//
//	Arg_0:	Source row id.
//	Arg_1:	Target row id.
// -----------------------------------------------------------------------------
m4return_t CopyRow( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t		iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;
	Book*			pWorkbook;
	Sheet*			pWorksheet;
	int				iSrcRow, iTgtRow;
	int				iLastCol;
	double			dRowHeight;
	bool			bRet;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks function arguments.
	CHECK_ARG_NUM(ai_iLongArg, 2, "CopyRow" )
	CHECK_ARG_TYPE(ai_pvArg[0].Type, M4CL_CPP_TYPE_NUMBER, "CopyRow")
	CHECK_ARG_TYPE(ai_pvArg[1].Type, M4CL_CPP_TYPE_NUMBER, "CopyRow")


	// Retrieves the Excel objects.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKBOOK_POINTER, ( void*& )pWorkbook);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WB_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "CopyRow" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, ( void*& )pWorksheet);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WS_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "CopyRow" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	iSrcRow = ai_pvArg[0].Data.DoubleData;
	iTgtRow = ai_pvArg[1].Data.DoubleData;


	// Java implementation compatibility.
	bRet = pWorksheet->insertRow( iTgtRow, iTgtRow );

	if( bRet == false )
	{
		SETCODEF( M4_XL_ERR_INSERT_ROW_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#", pWorkbook->errorMessage(), iTgtRow );
	}

	
	// Sets the target row height.
	dRowHeight = pWorksheet->rowHeight( iSrcRow );
	bRet = pWorksheet->setRow( iTgtRow, dRowHeight );

	if( bRet == false )
	{
		SETCODEF( M4_XL_ERR_SET_ROW_HEIGHT_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#%d#", pWorkbook->errorMessage(), iTgtRow, dRowHeight );
	}

	iLastCol = pWorksheet->lastCol();

	for( int iCol = 0; iCol < iLastCol; iCol++ )
	{
		// Copies each cell in the row.
		bRet = pWorksheet->copyCell( iSrcRow, iCol, iTgtRow, iCol );

		if( bRet == false )
		{
			SETCODEF( M4_XL_ERR_COPY_CELL_LIB_ERR, ERRORLOG, "#*s1*#%s#%s#%d#%d#%d#%d#", pWorkbook->errorMessage(), iSrcRow, iCol, iTgtRow, iCol );
			CH_RETURN( &ai_pvReturn, M4_ERROR )
		}
	}


	CH_RETURN( &ai_pvReturn, M4_SUCCESS )
}


// -----------------------------------------------------------------------------
// Hides / shows a column.
//
//	Arg_0:	Column id.
//	Arg_1:	Hide(1) / show(0).
// -----------------------------------------------------------------------------
m4return_t SetColHidden( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t		iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;
	Book*			pWorkbook;
	Sheet*			pWorksheet;
	int				iCol;
	bool			bHide;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks function arguments.
	CHECK_ARG_NUM(ai_iLongArg, 2, "SetColHidden" )
	CHECK_ARG_TYPE(ai_pvArg[0].Type, M4CL_CPP_TYPE_NUMBER, "SetColHidden")
	CHECK_ARG_TYPE(ai_pvArg[1].Type, M4CL_CPP_TYPE_NUMBER, "SetColHidden")


	// Retrieves the Excel objects.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKBOOK_POINTER, ( void*& )pWorkbook);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WB_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "SetColHidden" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, ( void*& )pWorksheet);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WS_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "SetColHidden" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	iCol = ai_pvArg[0].Data.DoubleData;
	bHide = ai_pvArg[1].Data.DoubleData != 0;


	// Hides or shows the column.
	pWorksheet->setColHidden( iCol, bHide );


	CH_RETURN( &ai_pvReturn, M4_SUCCESS )
}


// -----------------------------------------------------------------------------
// Hides / shows a row.
//
//	Arg_0:	Row id.
//	Arg_1:	Hide(1) / show(0).
// -----------------------------------------------------------------------------
m4return_t SetRowHidden( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t		iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;
	Book*			pWorkbook;
	Sheet*			pWorksheet;
	int				iRow;
	bool			bHide;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks function arguments.
	CHECK_ARG_NUM(ai_iLongArg, 2, "SetRowHidden" )
	CHECK_ARG_TYPE(ai_pvArg[0].Type, M4CL_CPP_TYPE_NUMBER, "SetRowHidden")
	CHECK_ARG_TYPE(ai_pvArg[1].Type, M4CL_CPP_TYPE_NUMBER, "SetRowHidden")


	// Retrieves the Excel objects.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKBOOK_POINTER, ( void*& )pWorkbook);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WB_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "SetRowHidden" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, ( void*& )pWorksheet);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WS_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "SetRowHidden" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	iRow = ai_pvArg[0].Data.DoubleData;
	bHide = ai_pvArg[1].Data.DoubleData != 0;


	// Hides or shows the row.
	pWorksheet->setRowHidden( iRow, bHide );


	CH_RETURN( &ai_pvReturn, M4_SUCCESS )
}


// -----------------------------------------------------------------------------
// Imports a CSV file into the current worksheet, receiving a columns type list. 
// 
//	Arg_0: Starting row number for the merge process.
//	Arg_1: Starting column number for the merge process.
//	Arg_2: Path of the csv file to merge.
//	Arg_3: Csv values delimiter.
//	Arg_4: Columns type list.
// -----------------------------------------------------------------------------
m4return_t ImportCSVEx( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t			iRet;
	ClVMRunContext*		pContext = ( ClVMRunContext* )ai_pvContext;
	Book*				pWorkbook;
	Sheet*				pWorksheet;
	ClXlSheetAccessor*	pSheetWriter;
	ClM4CsvParser*		pCSVParser;
	int					iLength = -1;
	char*				pcCsvFile;

	
	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks number of arguments.
	CHECK_ARG_NUM(ai_iLongArg, 5, "ImportCSVEx" )


	// Common implementation.
	if( !( ai_pvArg[4].Type == M4CL_CPP_TYPE_STRING_VAR && ai_pvArg[4].Data.PointerChar != NULL && strlen( ai_pvArg[4].Data.PointerChar ) > 0 ) )
	{
		iRet = ImportCSV( ai_pvArg, ai_iLongArg, ai_pvReturn, ai_pvContext );
		CH_RETURN( &ai_pvReturn, iRet )
	}


	// Extended implementation.

	// Checks function arguments.
	CHECK_ARG_TYPE(ai_pvArg[0].Type, M4CL_CPP_TYPE_NUMBER, "ImportCSVEx")
	CHECK_ARG_TYPE(ai_pvArg[1].Type, M4CL_CPP_TYPE_NUMBER, "ImportCSVEx")
	CHECK_ARG_TYPE(ai_pvArg[2].Type, M4CL_CPP_TYPE_STRING_VAR, "ImportCSVEx")
	CHECK_ARG_TYPE(ai_pvArg[3].Type, M4CL_CPP_TYPE_STRING_VAR, "ImportCSVEx")


	// Retrieves the Excel objects.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKBOOK_POINTER, ( void*& )pWorkbook);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WB_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "ImportCSV" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, ( void*& )pWorksheet);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WS_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "ImportCSV" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	// Builds an Excel sheet writer.
	pSheetWriter = new ClXlSheetAccessor( ai_pvArg[0].Data.DoubleData, ai_pvArg[1].Data.DoubleData, pWorkbook, pWorksheet, pContext, ai_pvArg[4].Data.PointerChar );
	
	// Builds the csv parser.
	pCSVParser = new ClM4CsvParser( pSheetWriter );

	// Loads the csv file to merge.
	pcCsvFile = M4CppToANSI( ai_pvArg[2].Data.PointerChar, iLength );
	iRet = pCSVParser->SetCsvFile( pcCsvFile, *ai_pvArg[3].Data.PointerChar );

	delete [] pcCsvFile;

	if( iRet == M4_SUCCESS )
	{
		// Merges the csv file.
		iRet = pCSVParser->Parse();

		if( iRet == M4_ERROR )
		{
			SETCODEF( M4_XL_ERR_PARS_GEN_ERR, ERRORLOG, "#*s1*#" );
		}
	}

	delete pCSVParser;
	delete pSheetWriter;

	
	CH_RETURN( &ai_pvReturn, iRet )
}


// -----------------------------------------------------------------------------
// Changes the bold attribute of a single cell, a user defined range, a full row 
// range or a full column range.
// 
//	Arg_0:	Cell's row number.
//	Arg_1:	Cell's column number.
//	Arg_2:	Range initial cell.
//	Arg_3:	Range end cell.
//	Arg_4:	Row index.
//	Arg_5:	Col index.
//	Arg_6:	Bold active (True/False).
// -----------------------------------------------------------------------------
m4return_t FontBold( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t	iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks number of arguments.
	CHECK_ARG_NUM(ai_iLongArg, 7, "FontBold")


	// Checks function arguments.
	CHECK_ARG_TYPE(ai_pvArg[6].Type, M4CL_CPP_TYPE_NUMBER, "FontBold")


	// Invoke the change.
	iRet = ClXlFormatsMngr::ChangeCellsAttribute( ai_pvArg, ai_pvContext, ClXlFormatsMngr::M4XL_FONT_BOLD );


	CH_RETURN( &ai_pvReturn, iRet )
}


// -----------------------------------------------------------------------------
// Changes the font color attribute of a single cell, a user defined range, 
// a full row range or a full column range.
// 
//	Arg_0:	Cell's row number.
//	Arg_1:	Cell's column number.
//	Arg_2:	Range initial cell.
//	Arg_3:	Range end cell.
//	Arg_4:	Row index.
//	Arg_5:	Col index.
//	Arg_6:	Color index.
// -----------------------------------------------------------------------------
m4return_t FontColorIndex( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t	iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks number of arguments.
	CHECK_ARG_NUM(ai_iLongArg, 7, "FontColorIndex")


	// Checks function arguments.
	CHECK_ARG_TYPE(ai_pvArg[6].Type, M4CL_CPP_TYPE_NUMBER, "FontColorIndex")


	// Invoke the change.
	iRet = ClXlFormatsMngr::ChangeCellsAttribute( ai_pvArg, ai_pvContext, ClXlFormatsMngr::M4XL_FONT_COLOR_INDEX );


	CH_RETURN( &ai_pvReturn, iRet )
}


// -----------------------------------------------------------------------------
// Changes the font size attribute of a single cell, a user defined range, 
// a full row range or a full column range.
// 
//	Arg_0:	Cell's row number.
//	Arg_1:	Cell's column number.
//	Arg_2:	Range initial cell.
//	Arg_3:	Range end cell.
//	Arg_4:	Row index.
//	Arg_5:	Col index.
//	Arg_6:	Font size.
// -----------------------------------------------------------------------------
m4return_t FontSize( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t	iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks number of arguments.
	CHECK_ARG_NUM(ai_iLongArg, 7, "FontSize")


	// Checks function arguments.
	CHECK_ARG_TYPE(ai_pvArg[6].Type, M4CL_CPP_TYPE_NUMBER, "FontSize")


	// Invoke the change.
	iRet = ClXlFormatsMngr::ChangeCellsAttribute( ai_pvArg, ai_pvContext, ClXlFormatsMngr::M4XL_FONT_SIZE );


	CH_RETURN( &ai_pvReturn, iRet )
}


// -----------------------------------------------------------------------------
// Changes the font underline attribute of a single cell, a user defined range,
// a full row range or a full column range.
// 
//	Arg_0:	Cell's row number.
//	Arg_1:	Cell's column number.
//	Arg_2:	Range initial cell.
//	Arg_3:	Range end cell.
//	Arg_4:	Row index.
//	Arg_5:	Col index.
//	Arg_6:	Underline style.
// -----------------------------------------------------------------------------
m4return_t FontUnderline( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t	iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks number of arguments.
	CHECK_ARG_NUM(ai_iLongArg, 7, "FontUnderline")


	// Checks function arguments.
	CHECK_ARG_TYPE(ai_pvArg[6].Type, M4CL_CPP_TYPE_NUMBER, "FontUnderline")


	// Invoke the change.
	iRet = ClXlFormatsMngr::ChangeCellsAttribute( ai_pvArg, ai_pvContext, ClXlFormatsMngr::M4XL_FONT_UNDERLINE );


	CH_RETURN( &ai_pvReturn, iRet )
}


// -----------------------------------------------------------------------------
// Changes the font name attribute of a single cell, a user defined range, 
// a full row range or a full column range.
// 
//	Arg_0:	Cell's row number.
//	Arg_1:	Cell's column number.
//	Arg_2:	Range initial cell.
//	Arg_3:	Range end cell.
//	Arg_4:	Row index.
//	Arg_5:	Col index.
//	Arg_6:	Font name.
// -----------------------------------------------------------------------------
m4return_t FontName( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t	iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks number of arguments.
	CHECK_ARG_NUM(ai_iLongArg, 7, "FontName")


	// Checks function arguments.
	CHECK_ARG_TYPE(ai_pvArg[6].Type, M4CL_CPP_TYPE_STRING_VAR, "FontName")


	// Invoke the change.
	iRet = ClXlFormatsMngr::ChangeCellsAttribute( ai_pvArg, ai_pvContext, ClXlFormatsMngr::M4XL_FONT_NAME );


	CH_RETURN( &ai_pvReturn, iRet )
}


// -----------------------------------------------------------------------------
// Changes the font italic attribute of a single cell, a user defined range, 
// a full row range or a full column range.
// 
//	Arg_0:	Cell's row number.
//	Arg_1:	Cell's column number.
//	Arg_2:	Range initial cell.
//	Arg_3:	Range end cell.
//	Arg_4:	Row index.
//	Arg_5:	Col index.
//	Arg_6:	Italic active (True/False).
// -----------------------------------------------------------------------------
m4return_t FontItalic( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t	iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks number of arguments.
	CHECK_ARG_NUM(ai_iLongArg, 7, "FontItalic")


	// Checks function arguments.
	CHECK_ARG_TYPE(ai_pvArg[6].Type, M4CL_CPP_TYPE_NUMBER, "FontItalic")


	// Invoke the change.
	iRet = ClXlFormatsMngr::ChangeCellsAttribute( ai_pvArg, ai_pvContext, ClXlFormatsMngr::M4XL_FONT_ITALIC );


	CH_RETURN( &ai_pvReturn, iRet )
}


// -----------------------------------------------------------------------------
// Changes the border style of a single cell, a user defined range, a full row 
// range or a full column range.
// 
//	Arg_0:	Cell's row number.
//	Arg_1:	Cell's column number.
//	Arg_2:	Range initial cell.
//	Arg_3:	Range end cell.
//	Arg_4:	Row index.
//	Arg_5:	Col index.
//	Arg_6:	Border style.
//	Arg_7:	Border edge.
//	Arg_8:	Outside.
// -----------------------------------------------------------------------------
m4return_t BorderStyle( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t	iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks number of arguments.
	CHECK_ARG_NUM(ai_iLongArg, 9, "BorderStyle")


	// Checks function arguments.
	CHECK_ARG_TYPE(ai_pvArg[6].Type, M4CL_CPP_TYPE_NUMBER, "BorderStyle")
	CHECK_ARG_TYPE(ai_pvArg[7].Type, M4CL_CPP_TYPE_NUMBER, "BorderStyle")
	CHECK_ARG_TYPE(ai_pvArg[8].Type, M4CL_CPP_TYPE_NUMBER, "BorderStyle")


	// Invoke the change.
	iRet = ClXlFormatsMngr::ChangeCellsAttribute( ai_pvArg, ai_pvContext, ClXlFormatsMngr::M4XL_BORDER_STYLE );


	CH_RETURN( &ai_pvReturn, iRet )
}


// -----------------------------------------------------------------------------
// Changes the border color index of a single cell, a user defined range, 
// a full row range or a full column range.
// 
//	Arg_0:	Cell's row number.
//	Arg_1:	Cell's column number.
//	Arg_2:	Range initial cell.
//	Arg_3:	Range end cell.
//	Arg_4:	Row index.
//	Arg_5:	Col index.
//	Arg_6:	Color index.
//	Arg_7:	Border edge.
//	Arg_8:	Outside.
// -----------------------------------------------------------------------------
m4return_t BorderColorIndex( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t	iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks number of arguments.
	CHECK_ARG_NUM(ai_iLongArg, 9, "BorderColorIndex")


	// Checks function arguments.
	CHECK_ARG_TYPE(ai_pvArg[6].Type, M4CL_CPP_TYPE_NUMBER, "BorderColorIndex")
	CHECK_ARG_TYPE(ai_pvArg[7].Type, M4CL_CPP_TYPE_NUMBER, "BorderColorIndex")
	CHECK_ARG_TYPE(ai_pvArg[8].Type, M4CL_CPP_TYPE_NUMBER, "BorderColorIndex")


	// Invoke the change.
	iRet = ClXlFormatsMngr::ChangeCellsAttribute( ai_pvArg, ai_pvContext, ClXlFormatsMngr::M4XL_BORDER_COLOR_INDEX );


	CH_RETURN( &ai_pvReturn, iRet )
}


const int c_iInsertShiftToDown  = -4121;
const int c_iInsertShiftToRight = -4161;

// -----------------------------------------------------------------------------
// Inserts into the sheet the current range or a previously copied one.
// 
//	Arg_0:	Range initial cell.
//	Arg_1:	Range end cell.
//	Arg_2:	Row index.
//	Arg_3:	Col index.
//	Arg_4:	Copied range initial cell.
//	Arg_5:	Copied range end cell
//	Arg_6:	Copied row index.
//	Arg_7:	Copied col index.
//	Arg_9:  Shift to down or right.
// -----------------------------------------------------------------------------
m4return_t InsertRange( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t		iRet;
	ClVMRunContext*	pContext = ( ClVMRunContext* )ai_pvContext;
	Book*			pWorkbook;
	Sheet*			pWorksheet;
	bool			bRet;
	int				iSrcStartRow, iSrcStartCol, iSrcEndRow, iSrcEndCol, iTgtStartRow, iTgtStartCol, iTgtEndRow, iTgtEndCol;
	wchar_t*		pwcCelRef;
	int				iOffset;
	bool			bShiftToDown = true;
	double			dWidth, dHeight;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks number of arguments.
	CHECK_ARG_NUM(ai_iLongArg, 9, "InsertRange")

	// Checks function arguments.
	CHECK_ARG_TYPE(ai_pvArg[8].Type, M4CL_CPP_TYPE_NUMBER, "InsertRange")


	// Default is shift to down.
	if( ai_pvArg[8].Data.DoubleData == c_iInsertShiftToRight )
	{
		bShiftToDown = false;
	}

	
	// Retrieves the Excel objects.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKBOOK_POINTER, ( void*& )pWorkbook);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WB_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "InsertRange" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, ( void*& )pWorksheet);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WS_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "InsertRange" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	// Determines the range shape.
	if( ai_pvArg[0].Type == M4CL_CPP_TYPE_STRING_VAR && ai_pvArg[1].Type == M4CL_CPP_TYPE_STRING_VAR && strlen( ai_pvArg[0].Data.PointerChar ) > 0 && strlen( ai_pvArg[1].Data.PointerChar ) > 0 )
	{
		// User defined range.
		iRet = CppStrToLibxlStr( ai_pvArg[0].Data.PointerChar, pwcCelRef );
		if( iRet == M4_ERROR )
		{
			SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", "InsertRange" );
			CH_RETURN( &ai_pvReturn, M4_ERROR )
		}

		pWorksheet->addrToRowCol( pwcCelRef, &iTgtStartRow, &iTgtStartCol );
		delete[] pwcCelRef;

		iRet = CppStrToLibxlStr( ai_pvArg[1].Data.PointerChar, pwcCelRef );
		if( iRet == M4_ERROR )
		{
			SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", "InsertRange" );
			CH_RETURN( &ai_pvReturn, M4_ERROR )
		}

		pWorksheet->addrToRowCol( pwcCelRef, &iTgtEndRow, &iTgtEndCol );
		delete[] pwcCelRef;
	
		// Inserts a copied range.
		if( ai_pvArg[4].Type == M4CL_CPP_TYPE_STRING_VAR && ai_pvArg[5].Type == M4CL_CPP_TYPE_STRING_VAR && strlen( ai_pvArg[4].Data.PointerChar ) > 0 && strlen( ai_pvArg[5].Data.PointerChar ) > 0 )
		{
			iRet = CppStrToLibxlStr( ai_pvArg[4].Data.PointerChar, pwcCelRef );
			if( iRet == M4_ERROR )
			{
				SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", "InsertRange" );
				CH_RETURN( &ai_pvReturn, M4_ERROR )
			}

			pWorksheet->addrToRowCol( pwcCelRef, &iSrcStartRow, &iSrcStartCol );
			delete[] pwcCelRef;

			iRet = CppStrToLibxlStr( ai_pvArg[5].Data.PointerChar, pwcCelRef );
			if( iRet == M4_ERROR )
			{
				SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", "InsertRange" );
				CH_RETURN( &ai_pvReturn, M4_ERROR )
			}

			pWorksheet->addrToRowCol( pwcCelRef, &iSrcEndRow, &iSrcEndCol );
			delete[] pwcCelRef;

			// Checks if insert is allowed in this cell.
			if( ( iTgtStartRow > iSrcEndRow ) || \
				( iTgtStartCol == iSrcStartCol && iTgtStartRow <= iSrcStartRow ) || \
				( iTgtStartCol > iSrcEndCol ) || \
				( ( iTgtStartCol + ( iSrcEndCol - iSrcStartCol ) ) < iSrcStartCol ) )
			{
				if( bShiftToDown == true )
				{
					// Scrolls down the target columns involved.
					iRet = CopyRangeDown( pWorkbook, pWorksheet, iTgtStartRow, iTgtStartCol, pWorksheet->lastRow() - 1, iTgtStartCol + ( iSrcEndCol - iSrcStartCol ), iTgtStartRow + ( iSrcEndRow - iSrcStartRow ) + 1, iTgtStartCol );

					if( iRet == M4_ERROR )
					{
						CH_RETURN( &ai_pvReturn, M4_ERROR )
					}

					// Adjusts the source position.
					if( iTgtStartCol == iSrcStartCol && iTgtStartRow <= iSrcStartRow )
					{
						iOffset		  = ( iSrcEndRow - iSrcStartRow ) + 1;
						iSrcStartRow += iOffset;
						iSrcEndRow	 += iOffset;
					}
					
					// Inserts the source range in the new hole.
					iRet = CopyRangeDown( pWorkbook, pWorksheet, iSrcStartRow, iSrcStartCol, iSrcEndRow, iSrcEndCol, iTgtStartRow, iTgtStartCol );
				}
				else
				{
					// Scrolls right the target rows involved.
					iRet = CopyRangeRight( pWorkbook, pWorksheet, iTgtStartRow, iTgtStartCol, iTgtStartRow + ( iSrcEndRow - iSrcStartRow ), pWorksheet->lastCol() - 1,  iTgtStartRow, iTgtStartCol + ( iSrcEndCol - iSrcStartCol ) + 1 );

					if( iRet == M4_ERROR )
					{
						CH_RETURN( &ai_pvReturn, M4_ERROR )
					}

					// Adjusts the source position.
					if( iTgtStartRow == iSrcStartRow && iTgtStartCol <= iSrcStartCol )
					{
						iOffset		  = ( iSrcEndCol - iSrcStartCol ) + 1;
						iSrcStartCol += iOffset;
						iSrcEndCol	 += iOffset;
					}
					
					// Inserts the source range in the new hole.
					iRet = CopyRangeRight( pWorkbook, pWorksheet, iSrcStartRow, iSrcStartCol, iSrcEndRow, iSrcEndCol, iTgtStartRow, iTgtStartCol );
				}
			}
			else
			{
				SETCODEF( M4_XL_ERR_INSERT_RANGE_OVERLAP_ERR, ERRORLOG, "#*s1*#" );
				CH_RETURN( &ai_pvReturn, M4_ERROR )
			}
		}
		else
		{
			// Inserts the current range shape.

			if( bShiftToDown == true )
			{
				// Scrolls down the target columns involved.
				iRet = CopyRangeDown( pWorkbook, pWorksheet, iTgtStartRow, iTgtStartCol, pWorksheet->lastRow() - 1, iTgtEndCol, iTgtEndRow + 1, iTgtStartCol );

				if( iRet == M4_ERROR )
				{
					CH_RETURN( &ai_pvReturn, M4_ERROR )
				}

				if( iTgtStartRow > 0 )
				{
					// Applies the format of the previous row.
					
					// Note: As Excel removes the borders of the template row, we also have to do it.
					iRet = FillFormatDown( pWorkbook, pWorksheet, iTgtStartRow, iTgtStartCol, iTgtEndRow, iTgtEndCol, true, true, true, pContext->m_pAccess );
				}
				else
				{
					// Applies blank format.
					iRet = FillFormatDown( pWorkbook, pWorksheet, iTgtStartRow, iTgtStartCol, iTgtEndRow, iTgtEndCol, false, true, false );
				}
			}
			else
			{
				// Scrolls right the target rows involved.
				iRet = CopyRangeRight( pWorkbook, pWorksheet, iTgtStartRow, iTgtStartCol, iTgtEndRow, pWorksheet->lastCol() - 1, iTgtStartRow, iTgtEndCol + 1 );

				if( iRet == M4_ERROR )
				{
					CH_RETURN( &ai_pvReturn, M4_ERROR )
				}

				if( iTgtStartCol > 0 )
				{
					// Applies the format of the previous row.
					
					// Note: As Excel removes the borders of the template row, we also have to do it.
					iRet = FillFormatRight( pWorkbook, pWorksheet, iTgtStartRow, iTgtStartCol, iTgtEndRow, iTgtEndCol, true, true, true, pContext->m_pAccess );
				}
				else
				{
					// Applies blank format.
					iRet = FillFormatRight( pWorkbook, pWorksheet, iTgtStartRow, iTgtStartCol, iTgtEndRow, iTgtEndCol, false, true, false );
				}
			}
		}
	}
	else if( ai_pvArg[2].Type == M4CL_CPP_TYPE_NUMBER && ai_pvArg[2].Data.DoubleData >= 0 )
	{
		// Full row range.
		iTgtStartRow = ai_pvArg[2].Data.DoubleData;

		bRet = pWorksheet->insertRow( iTgtStartRow, iTgtStartRow );

		if( bRet == false )
		{
			SETCODEF( M4_XL_ERR_INSERT_ROW_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#", pWorkbook->errorMessage(), iTgtStartRow );
		}

		// Inserts a copied range.
		if( ai_pvArg[6].Type == M4CL_CPP_TYPE_NUMBER && ai_pvArg[6].Data.DoubleData >= 0 )
		{
			iSrcStartRow = ai_pvArg[6].Data.DoubleData;

			if( iSrcStartRow >= iTgtStartRow )
			{
				iSrcStartRow++;
			}

			// Sets new row height equals to the copied one.
			dHeight = pWorksheet->rowHeight( iSrcStartRow );
			bRet = pWorksheet->setRow( iTgtStartRow, dHeight );

			if( bRet == false )
			{
				SETCODEF( M4_XL_ERR_SET_ROW_HEIGHT_LIB_ERR_DET, ERRORLOG, "#*s1*#%s#%d#", pWorkbook->errorMessage(), iTgtStartRow );
			}

			iRet = CopyRangeDown( pWorkbook, pWorksheet, iSrcStartRow, 0, iSrcStartRow, pWorksheet->lastCol() - 1, iTgtStartRow, 0 );
		}
		else
		{
			// Inserts the current range shape.

			if( iTgtStartRow > 0 )
			{
				// Sets new row height equals to the previous one.
				dHeight = pWorksheet->rowHeight( iTgtStartRow - 1 );
				bRet = pWorksheet->setRow( iTgtStartRow, dHeight );

				if( bRet == false )
				{
					SETCODEF( M4_XL_ERR_SET_ROW_HEIGHT_LIB_ERR_DET, ERRORLOG, "#*s1*#%s#%d#", pWorkbook->errorMessage(), iTgtStartRow );
				}

				// Applies the format of the previous row.

				// Note: As Excel removes the borders of the template row, we also have to do it.
				iRet = FillFormatDown( pWorkbook, pWorksheet, iTgtStartRow, 0, iTgtStartRow, pWorksheet->lastCol() - 1, true, true, true, pContext->m_pAccess );
			}
			else
			{
				// Applies blank format by default.
				// iRet = FillFormatDown( pWorkbook, pWorksheet, iTgtStartRow, 0, iTgtStartRow, pWorksheet->lastCol() - 1, false, true, false );
			}
	
		}
	}
	else if( ai_pvArg[3].Type == M4CL_CPP_TYPE_NUMBER && ai_pvArg[3].Data.DoubleData >= 0 )
	{
		// Full column range.
		iTgtStartCol = ai_pvArg[3].Data.DoubleData;

		bRet = pWorksheet->insertCol( iTgtStartCol, iTgtStartCol );

		if( bRet == false )
		{
			SETCODEF( M4_XL_ERR_INSERT_COL_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#", pWorkbook->errorMessage(), iTgtStartCol );
		}

		// Inserts a copied range.
		if( ai_pvArg[7].Type == M4CL_CPP_TYPE_NUMBER && ai_pvArg[7].Data.DoubleData >= 0 )
		{
			iSrcStartCol = ai_pvArg[7].Data.DoubleData;

			if( iSrcStartCol >= iTgtStartCol )
			{
				iSrcStartCol++;
			}

			// Sets new column width equals to the copied one.
			dWidth = pWorksheet->colWidth( iSrcStartCol );
			bRet = pWorksheet->setCol( iTgtStartCol, iTgtStartCol, dWidth );

			if( bRet == false )
			{
				SETCODEF( M4_XL_ERR_SET_COL_WIDTH_LIB_ERR_DET, ERRORLOG, "#*s1*#%s#%d#", pWorkbook->errorMessage(), iTgtStartCol );
			}

			iRet = CopyRangeRight( pWorkbook, pWorksheet, 0, iSrcStartCol, pWorksheet->lastRow() - 1, iSrcStartCol, 0, iTgtStartCol );
		}
		else
		{
			// Inserts the current range shape.

			if( iTgtStartCol > 0 )
			{
				// Sets new column width equals to the previous one.
				dWidth = pWorksheet->colWidth( iTgtStartCol - 1 );
				bRet = pWorksheet->setCol( iTgtStartCol, iTgtStartCol, dWidth );

				if( bRet == false )
				{
					SETCODEF( M4_XL_ERR_SET_COL_WIDTH_LIB_ERR_DET, ERRORLOG, "#*s1*#%s#%d#", pWorkbook->errorMessage(), iTgtStartCol );
				}

				// Applies the format of the previous col.
				iRet = FillFormatRight( pWorkbook, pWorksheet, 0, iTgtStartCol, pWorksheet->lastRow() - 1, iTgtStartCol, true, true, false );
			}
			else
			{
				// Applies blank format by default.
				// iRet = FillFormatDown( pWorkbook, pWorksheet, 0, iTgtStartCol, pWorksheet->lastRow() - 1, iTgtStartCol, false, true, false );
			}
		}
	}
	else
	{
		iRet = M4_ERROR;
	}


	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_INSERT_RANGE_GEN_ERR, ERRORLOG, "#*s1*#" );
	}


	CH_RETURN( &ai_pvReturn, iRet )
}


const int c_iInsertShiftToUp   = -4162;
const int c_iInsertShiftToLeft = -4159;

// -----------------------------------------------------------------------------
// Deletes the current range.
// 
//	Arg_0:	Range initial cell.
//	Arg_1:	Range end cell.
//	Arg_2:	Row index.
//	Arg_3:	Col index.
//	Arg_4:  Shift to up or left.
// -----------------------------------------------------------------------------
m4return_t DeleteRange( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t		iRet;
	ClVMRunContext*	pContext = ( ClVMRunContext* )ai_pvContext;
	Book*			pWorkbook;
	Sheet*			pWorksheet;
	bool			bRet;
	int				iStartRow, iStartCol, iEndRow, iEndCol;
	wchar_t*		pwcCelRef;
	bool			bShiftToUp = true;
	
	
	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks number of arguments.
	CHECK_ARG_NUM(ai_iLongArg, 5, "DeleteRange")

	// Checks function arguments.
	CHECK_ARG_TYPE(ai_pvArg[4].Type, M4CL_CPP_TYPE_NUMBER, "DeleteRange")


	// Default is shift to up.
	if( ai_pvArg[4].Data.DoubleData == c_iInsertShiftToLeft )
	{
		bShiftToUp = false;
	}

	
	// Retrieves the Excel objects.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKBOOK_POINTER, ( void*& )pWorkbook);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WB_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "DeleteRange" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, ( void*& )pWorksheet);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WS_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "DeleteRange" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	// Determines the range shape.
	if( ai_pvArg[0].Type == M4CL_CPP_TYPE_STRING_VAR && ai_pvArg[1].Type == M4CL_CPP_TYPE_STRING_VAR && strlen( ai_pvArg[0].Data.PointerChar ) > 0 && strlen( ai_pvArg[1].Data.PointerChar ) > 0 )
	{
		// User defined range.
		iRet = CppStrToLibxlStr( ai_pvArg[0].Data.PointerChar, pwcCelRef );
		if( iRet == M4_ERROR )
		{
			SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", "DeleteRange" );
			CH_RETURN( &ai_pvReturn, M4_ERROR )
		}

		pWorksheet->addrToRowCol( pwcCelRef, &iStartRow, &iStartCol );
		delete[] pwcCelRef;

		iRet = CppStrToLibxlStr( ai_pvArg[1].Data.PointerChar, pwcCelRef );
		if( iRet == M4_ERROR )
		{
			SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", "DeleteRange" );
			CH_RETURN( &ai_pvReturn, M4_ERROR )
		}

		pWorksheet->addrToRowCol( pwcCelRef, &iEndRow, &iEndCol );
		delete[] pwcCelRef;

		if( bShiftToUp == true )
		{
			iRet = CopyRangeDown( pWorkbook, pWorksheet, iEndRow + 1, iStartCol, pWorksheet->lastRow() - 1, iEndCol, iStartRow, iStartCol );

			if( iRet == M4_SUCCESS )
			{
				iRet = FillFormatDown( pWorkbook, pWorksheet, pWorksheet->lastRow() - 1 - ( iEndRow - iStartRow), iStartCol, pWorksheet->lastRow() - 1, iEndCol, false, true, false );
			}
		}
		else
		{
			iRet = CopyRangeRight( pWorkbook, pWorksheet, iStartRow, iEndCol + 1, iEndRow, pWorksheet->lastCol() - 1, iStartRow, iStartCol );

			if( iRet == M4_SUCCESS )
			{
				iRet = FillFormatRight( pWorkbook, pWorksheet, iStartRow, pWorksheet->lastCol() - 1 - ( iEndCol - iStartCol ), iEndRow, pWorksheet->lastCol() - 1, false, true, false );
			}
		}
	}
	else if( ai_pvArg[2].Type == M4CL_CPP_TYPE_NUMBER && ai_pvArg[2].Data.DoubleData >= 0 )
	{
		// Full row range.
		iStartRow = ai_pvArg[2].Data.DoubleData;

		bRet = pWorksheet->removeRow( iStartRow, iStartRow );

		if( bRet == false )
		{
			SETCODEF( M4_XL_ERR_DELETE_ROW_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#", pWorkbook->errorMessage(), iStartRow );
			CH_RETURN( &ai_pvReturn, M4_ERROR )
		}
	}
	else if( ai_pvArg[3].Type == M4CL_CPP_TYPE_NUMBER && ai_pvArg[3].Data.DoubleData >= 0 )
	{
		// Full column range.
		iStartCol = ai_pvArg[3].Data.DoubleData;

		bRet = pWorksheet->removeCol( iStartCol, iStartCol );

		if( bRet == false )
		{
			SETCODEF( M4_XL_ERR_DELETE_COL_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#", pWorkbook->errorMessage(), iStartCol );
			CH_RETURN( &ai_pvReturn, M4_ERROR )
		}
	}
	else
	{
		iRet = M4_ERROR;
	}


	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_DELETE_RANGE_GEN_ERR, ERRORLOG, "#*s1*#" );
	}


	CH_RETURN( &ai_pvReturn, iRet )
}


// -----------------------------------------------------------------------------
// Sets the numberformat of a single cell, a user defined range, 
// a full row range or a full column range.
// 
//	Arg_0:	Cell's row number.
//	Arg_1:	Cell's column number.
//	Arg_2:	Range initial cell.
//	Arg_3:	Range end cell.
//	Arg_4:	Row index.
//	Arg_5:	Col index.
//	Arg_6:	Numberformat.
// -----------------------------------------------------------------------------
m4return_t SetNumberformat( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t	iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks number of arguments.
	CHECK_ARG_NUM(ai_iLongArg, 7, "SetNumberformat")


	// Checks function arguments.
	CHECK_ARG_TYPE(ai_pvArg[6].Type, M4CL_CPP_TYPE_STRING_VAR, "SetNumberformat")


	// Invoke the change.
	iRet = ClXlFormatsMngr::ChangeCellsAttribute( ai_pvArg, ai_pvContext, ClXlFormatsMngr::M4XL_NUMBERFORMAT );


	CH_RETURN( &ai_pvReturn, iRet )
}


// -----------------------------------------------------------------------------
// Gets the numberformat of a single cell
// 
//	Arg_0:	Cell's row number.
//	Arg_1:	Cell's column number.
//	Arg_2:	Range initial cell.		[Ignored]
//	Arg_3:	Range end cell.			[Ignored]
//	Arg_4:	Row index.				[Ignored]
//	Arg_5:	Col index.				[Ignored]
//	Arg_6:	Output numberformat.
// -----------------------------------------------------------------------------
m4return_t GetNumberformat( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t		iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;
	Book*			pWorkbook;
	Sheet*			pWorksheet;
	Format*			pFormat;
	int				iNumFormat;
	const wchar_t*	pwcNumFormat;
	char*			pcNumFormat;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks number of arguments.
	CHECK_ARG_NUM(ai_iLongArg, 7, "GetNumberFormat")


	// Checks function arguments.
	CHECK_ARG_TYPE(ai_pvArg[0].Type, M4CL_CPP_TYPE_NUMBER, "GetNumberformat")
	CHECK_ARG_TYPE(ai_pvArg[1].Type, M4CL_CPP_TYPE_NUMBER, "GetNumberformat")


	// Retrieves the Excel objects.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKBOOK_POINTER, ( void*& )pWorkbook);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WB_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "GetNumberformat" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, ( void*& )pWorksheet);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WS_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "GetNumberformat" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	
	ai_pvArg[6].Type = M4CL_CPP_TYPE_NULL;

	pFormat	= pWorksheet->cellFormat( ai_pvArg[0].Data.DoubleData, ai_pvArg[1].Data.DoubleData );

	if( pFormat != NULL )
	{
		iNumFormat = pFormat->numFormat();
		
		ai_pvArg[6].Type = M4CL_CPP_TYPE_STRING_VAR;


		iRet = APIChannel::GetNumberFormat( iNumFormat, pContext->m_pAccess, pcNumFormat );

		if( iRet == M4_SUCCESS && pcNumFormat != NULL )
		{
			// Built-in number format or an already used custom number format.
			ai_pvArg[6].SetString( pcNumFormat );
		}
		else
		{
			// Custom number format.
			 pwcNumFormat = pWorkbook->customNumFormat( iNumFormat );

			 if( pwcNumFormat == NULL )
			 {
				SETCODEF( M4_XL_ERR_GET_NUM_FORMAT_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#", pWorkbook->errorMessage(), ai_pvArg[0].Data.DoubleData, ai_pvArg[1].Data.DoubleData );
				CH_RETURN( &ai_pvReturn, M4_ERROR )
			 }

			iRet = LibxlStrToCppStr( pwcNumFormat, pcNumFormat );
			if( iRet == M4_ERROR )
			{
				SETCODEF( M4_XL_ERR_GEN_XL_2_CPP, ERRORLOG, "#*s1*#%s#", "GetNumberformat" );
				CH_RETURN( &ai_pvReturn, M4_ERROR )
			}

			ai_pvArg[6].SetString( pcNumFormat );

			delete[] pcNumFormat;
		}
	}
	else
	{
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	CH_RETURN( &ai_pvReturn, M4_SUCCESS )
}


// -----------------------------------------------------------------------------
// Sets the name of the current sheet.
// 
//	Arg_0:	Sheet name.
// -----------------------------------------------------------------------------
m4return_t SetSheetName( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t		iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;
	Sheet*			pWorksheet;
	wchar_t*		pwcName;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks number of arguments.
	CHECK_ARG_NUM(ai_iLongArg, 1, "SetSheetName")


	// Checks function arguments.
	CHECK_ARG_TYPE(ai_pvArg[0].Type, M4CL_CPP_TYPE_STRING_VAR, "SetSheetName")


	// Retrieves the Excel objects.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, ( void*& )pWorksheet);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WS_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "SetSheetName" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	
	iRet = CppStrToLibxlStr( ai_pvArg[0].Data.PointerChar, pwcName );
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", "SetSheetName" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	SanitizeSheetName( pwcName );

	// Sets the name.
	pWorksheet->setName( pwcName );

	delete [] pwcName;

	APIChannel::SetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_NAME, ai_pvArg[0].Data.PointerChar);

	
	CH_RETURN( &ai_pvReturn, M4_SUCCESS )
}


// -----------------------------------------------------------------------------
// Sets the width of a column.
//
//	Arg_0:	Column index.
//	Arg_1:	Column width.
// -----------------------------------------------------------------------------
m4return_t SetColWidth( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t		iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;
	Book*			pWorkbook;
	Sheet*			pWorksheet;
	bool			bRet;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks number of arguments.
	CHECK_ARG_NUM(ai_iLongArg, 2, "SetColWidth")


	// Checks function arguments.
	CHECK_ARG_TYPE(ai_pvArg[0].Type, M4CL_CPP_TYPE_NUMBER, "SetColWidth")
	CHECK_ARG_TYPE(ai_pvArg[1].Type, M4CL_CPP_TYPE_NUMBER, "SetColWidth")


	// Retrieves the Excel objects.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKBOOK_POINTER, ( void*& )pWorkbook);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WB_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "SetColWidth" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, ( void*& )pWorksheet);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WS_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "SetColWidth" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	bRet = pWorksheet->setCol( ai_pvArg[0].Data.DoubleData, ai_pvArg[0].Data.DoubleData, ai_pvArg[1].Data.DoubleData );

	if( bRet == false)
	{
		SETCODEF( M4_XL_ERR_SET_COL_WIDTH_LIB_ERR_DET, ERRORLOG, "#*s1*#%s#%d#", pWorkbook->errorMessage(), ai_pvArg[0].Data.DoubleData );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	CH_RETURN( &ai_pvReturn, M4_SUCCESS )
}


// -----------------------------------------------------------------------------
// Protects/Unprotects the current worksheet.
//
//	Arg_0:	Protect(1) / Unprotect(0).
//	Arg_1:	optional. Password.
// -----------------------------------------------------------------------------
m4return_t SetProtect( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t		iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;
	Sheet*			pWorksheet;
	wchar_t*		pwcPwd = NULL;
	

	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks number of arguments.
	CHECK_ARG_NUM(ai_iLongArg, 2, "SetProtect")


	// Checks function arguments.
	CHECK_ARG_TYPE(ai_pvArg[0].Type, M4CL_CPP_TYPE_NUMBER, "SetProtect")


	// Retrieves the Excel objects.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, ( void*& )pWorksheet);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WS_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "SetProtect" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	if( ai_pvArg[1].Type == M4CL_CPP_TYPE_STRING_VAR && ai_pvArg[1].Data.PointerChar != NULL && *( ai_pvArg[1].Data.PointerChar) != NULL )
	{
		iRet = CppStrToLibxlStr( ai_pvArg[1].Data.PointerChar, pwcPwd );
		if( iRet == M4_ERROR )
		{
			SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", "SetProtect" );
			CH_RETURN( &ai_pvReturn, M4_ERROR )
		}
	}

	pWorksheet->setProtect( ai_pvArg[0].Data.DoubleData, pwcPwd, PROT_ALL );

	if( pwcPwd != NULL )
	{
		delete[] pwcPwd;
	}


	CH_RETURN( &ai_pvReturn, M4_SUCCESS )
}


// -----------------------------------------------------------------------------
// Clears the contents of a range.
// 
//	Arg_0:	Range initial cell.
//	Arg_1:	Range end cell.
//	Arg_2:	Row index.
//	Arg_3:	Col index.
// -----------------------------------------------------------------------------
m4return_t ClearContents( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t		iRet = M4_SUCCESS;
	ClVMRunContext*	pContext = ( ClVMRunContext* )ai_pvContext;
	Sheet*			pWorksheet;
	bool			bRet;
	int				iStartRow, iStartCol, iEndRow, iEndCol;
	wchar_t*		pwcCelRef;
	

	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks number of arguments.
	CHECK_ARG_NUM(ai_iLongArg, 4, "ClearContents")

	
	// Retrieves the Excel objects.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, ( void*& )pWorksheet);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WS_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "ClearContents" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	// Determines the range shape.
	if( ai_pvArg[0].Type == M4CL_CPP_TYPE_STRING_VAR && ai_pvArg[1].Type == M4CL_CPP_TYPE_STRING_VAR && strlen( ai_pvArg[0].Data.PointerChar ) > 0 && strlen( ai_pvArg[1].Data.PointerChar ) > 0 )
	{
		// User defined range.
		iRet = CppStrToLibxlStr( ai_pvArg[0].Data.PointerChar, pwcCelRef );
		if( iRet == M4_ERROR )
		{
			SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", "ClearContents" );
			CH_RETURN( &ai_pvReturn, M4_ERROR )
		}

		pWorksheet->addrToRowCol( pwcCelRef, &iStartRow, &iStartCol );
		delete[] pwcCelRef;

		iRet = CppStrToLibxlStr( ai_pvArg[1].Data.PointerChar, pwcCelRef );
		if( iRet == M4_ERROR )
		{
			SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", "ClearContents" );
			CH_RETURN( &ai_pvReturn, M4_ERROR )
		}

		pWorksheet->addrToRowCol( pwcCelRef, &iEndRow, &iEndCol );
		delete[] pwcCelRef;
	

		pWorksheet->clear( iStartRow, iEndRow, iStartCol, iEndCol );
	}
	else if( ai_pvArg[2].Type == M4CL_CPP_TYPE_NUMBER && ai_pvArg[2].Data.DoubleData >= 0 )
	{
		// Full row range.
		iStartRow = ai_pvArg[2].Data.DoubleData;

		pWorksheet->clear( iStartRow, iStartRow, 0, pWorksheet->lastCol() - 1 );
	}
	else if( ai_pvArg[3].Type == M4CL_CPP_TYPE_NUMBER && ai_pvArg[3].Data.DoubleData >= 0 )
	{
		// Full column range.
		iStartCol = ai_pvArg[3].Data.DoubleData;

		pWorksheet->clear( 0, pWorksheet->lastRow() - 1, iStartCol, iStartCol );
	}
	else
	{
		SETCODEF( M4_XL_ERR_CLEAR_CONTENTS_GEN_ERR, ERRORLOG, "#*s1*#" );
		iRet = M4_ERROR;
	}


	CH_RETURN( &ai_pvReturn, iRet )
}


// -----------------------------------------------------------------------------
// Creates a new Excel worksheet.
// -----------------------------------------------------------------------------
m4return_t NewWorksheet( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t		iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;
	Book*			pWorkbook;
	char			pcSheetName[ 32 ];
	wchar_t*		pwcSheetName = NULL;
	int				iSheetIndex;
	Sheet*			pWorksheet;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Retrieves the Excel object.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKBOOK_POINTER, ( void*& )pWorkbook);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WB_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "NewWorksheet" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	// Builds the sheet name.
	iSheetIndex	= pWorkbook->sheetCount();
	sprintf( pcSheetName, "Sheet%d", iSheetIndex + 1 );

	iRet = CppStrToLibxlStr( pcSheetName, pwcSheetName );
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", "NewWorksheet" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	// Creates de new the worksheet.
	pWorksheet	= pWorkbook->addSheet( pwcSheetName );

	delete [] pwcSheetName;

	if( pWorksheet == NULL )
	{
		SETCODEF( M4_XL_ERR_NEW_WS_LIB_ERR, ERRORLOG, "#*s1*#%s#%s#", pWorkbook->errorMessage(), pcSheetName );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	// Saves the xl object reference.
	APIChannel::SetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, pWorksheet);
	APIChannel::SetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_INDEX, iSheetIndex);
	APIChannel::SetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_NAME, pcSheetName);


	CH_RETURN( &ai_pvReturn, M4_SUCCESS )
}


// -----------------------------------------------------------------------------
// Creates a new Excel workbook.
// -----------------------------------------------------------------------------
m4return_t CreateWorkbook( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t		iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;
	Book*			pWorkbook;

	
	CheckTraceMode(pContext->m_pAccess);
	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);
	
	// Default format.
	pWorkbook = xlCreateXMLBook();

	if( pWorkbook == NULL )
	{
		SETCODEF( M4_XL_ERR_NEW_WB_NULL_OBJ, ERRORLOG, "#*s1*#" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	
	// Saves the xl object reference.
	APIChannel::SetItem(pContext->m_pAccess, APIChannel::WORKBOOKS, APIChannel::WORKBOOK_POINTER, pWorkbook);
	APIChannel::SetItem(pContext->m_pAccess, APIChannel::WORKBOOKS, APIChannel::WORKBOOK_PATH);


	CH_RETURN( &ai_pvReturn, M4_SUCCESS )
}


// -----------------------------------------------------------------------------
// Gets the width of a column.
//
//	Arg_0:	Column index.
//	Arg_1:	Output column width.
// -----------------------------------------------------------------------------
m4return_t GetColWidth( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t		iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;
	Sheet*			pWorksheet;
	double			dColWidth;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks number of arguments.
	CHECK_ARG_NUM(ai_iLongArg, 2, "GetColWidth")


	// Checks function arguments.
	CHECK_ARG_TYPE(ai_pvArg[0].Type, M4CL_CPP_TYPE_NUMBER, "GetColWidth")


	// Retrieves the Excel objects.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, ( void*& )pWorksheet);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WS_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "GetColWidth" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	dColWidth = pWorksheet->colWidth( ai_pvArg[0].Data.DoubleData );


	ai_pvArg[1].SetDouble( dColWidth );


	CH_RETURN( &ai_pvReturn, M4_SUCCESS )
}


// -----------------------------------------------------------------------------
// Sets the height of a row.
//
//	Arg_0:	Row index.
//	Arg_1:	Row height.
// -----------------------------------------------------------------------------
m4return_t SetRowHeight( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t		iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;
	Book*			pWorkbook;
	Sheet*			pWorksheet;
	bool			bRet;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks number of arguments.
	CHECK_ARG_NUM(ai_iLongArg, 2, "SetRowHeight")


	// Checks function arguments.
	CHECK_ARG_TYPE(ai_pvArg[0].Type, M4CL_CPP_TYPE_NUMBER, "SetRowHeight")
	CHECK_ARG_TYPE(ai_pvArg[1].Type, M4CL_CPP_TYPE_NUMBER, "SetRowHeight")


	// Retrieves the Excel objects.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKBOOK_POINTER, ( void*& )pWorkbook);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WB_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "SetRowHeight" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, ( void*& )pWorksheet);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WS_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "SetRowHeight" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	bRet = pWorksheet->setRow( ai_pvArg[0].Data.DoubleData, ai_pvArg[1].Data.DoubleData );

	if( bRet == false)
	{
		SETCODEF( M4_XL_ERR_SET_ROW_HEIGHT_LIB_ERR_DET, ERRORLOG, "#*s1*#%s#%d#", pWorkbook->errorMessage(), ai_pvArg[0].Data.DoubleData );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	CH_RETURN( &ai_pvReturn, M4_SUCCESS )
}



// -----------------------------------------------------------------------------
// Gets the height of a row.
//
//	Arg_0:	Row index.
//	Arg_1:	Output row height.
// -----------------------------------------------------------------------------
m4return_t GetRowHeight( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t		iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;
	Sheet*			pWorksheet;
	double			dRowHeight;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks number of arguments.
	CHECK_ARG_NUM(ai_iLongArg, 2, "GetRowHeight")


	// Checks function arguments.
	CHECK_ARG_TYPE(ai_pvArg[0].Type, M4CL_CPP_TYPE_NUMBER, "GetRowHeight")


	// Retrieves the Excel objects.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, ( void*& )pWorksheet);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WS_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "GetRowHeight" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	dRowHeight = pWorksheet->rowHeight( ai_pvArg[0].Data.DoubleData );


	ai_pvArg[1].SetDouble( dRowHeight );


	CH_RETURN( &ai_pvReturn, M4_SUCCESS )
}


// -----------------------------------------------------------------------------
// Gets the name of the current sheet.
// 
//	Arg_0:	Output sheet name.
// -----------------------------------------------------------------------------
m4return_t GetSheetName( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t		iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;
	Sheet*			pWorksheet;
	char*			pcSheetName;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks number of arguments.
	CHECK_ARG_NUM(ai_iLongArg, 1, "GetSheetName")


	// Retrieves the Excel objects.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, ( void*& )pWorksheet);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WS_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "GetSheetName" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	
	// Gets the name.
	iRet = LibxlStrToCppStr( pWorksheet->name(), pcSheetName );
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_GEN_XL_2_CPP, ERRORLOG, "#*s1*#%s#", "GetSheetName" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	ai_pvArg[0].SetString( pcSheetName );

	delete [] pcSheetName;

	
	CH_RETURN( &ai_pvReturn, M4_SUCCESS )
}


// -----------------------------------------------------------------------------
// Writes the values stored in a CSV string into the current worksheet, 
// receiving a columns type list. 
// 
//	Arg_0: csv string to merge.
//	Arg_1: Csv values delimiter.
//	Arg_2: Starting row number for the merge process.
//	Arg_3: Starting column number for the merge process.
//	Arg_4: Columns type list.
// -----------------------------------------------------------------------------
m4return_t PasteSpecialTextEx(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext)
{
	m4return_t			iRet;
	ClVMRunContext*		pContext = ( ClVMRunContext* )ai_pvContext;
	Book*				pWorkbook;
	Sheet*				pWorksheet;
	ClXlSheetAccessor*	pSheetWriter;
	ClM4CsvParser*		pCSVParser;
	char				iValTerminator;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks function arguments.
	CHECK_ARG_NUM(ai_iLongArg, 5, "PasteSpecialTextEx" )

	// Common implementation.
	if( !( ai_pvArg[4].Type == M4CL_CPP_TYPE_STRING_VAR && ai_pvArg[4].Data.PointerChar != NULL && strlen( ai_pvArg[4].Data.PointerChar ) > 0 ) )
	{
		iRet = PasteSpecialText( ai_pvArg, ai_iLongArg, ai_pvReturn, ai_pvContext );
		CH_RETURN( &ai_pvReturn, iRet )
	}


	// Extended implementation.

	CHECK_ARG_TYPE(ai_pvArg[0].Type, M4CL_CPP_TYPE_STRING_VAR, "PasteSpecialTextEx")
	//CHECK_ARG_TYPE(ai_pvArg[1].Type, M4CL_CPP_TYPE_NUMBER, "PasteSpecialTextEx")	// Import tool sends a string data.
	CHECK_ARG_TYPE(ai_pvArg[2].Type, M4CL_CPP_TYPE_NUMBER, "PasteSpecialTextEx")
	CHECK_ARG_TYPE(ai_pvArg[3].Type, M4CL_CPP_TYPE_NUMBER, "PasteSpecialTextEx")


	// Retrieves the Excel objects.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKBOOK_POINTER, ( void*& )pWorkbook);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WB_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "PasteSpecialTextEx" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, ( void*& )pWorksheet);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WS_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "PasteSpecialTextEx" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	// Builds an Excel sheet writer.
	pSheetWriter = new ClXlSheetAccessor( ai_pvArg[2].Data.DoubleData, ai_pvArg[3].Data.DoubleData, pWorkbook, pWorksheet, pContext, ai_pvArg[4].Data.PointerChar );

	// Builds the csv parser.
	pCSVParser	 = new ClM4CsvParser( pSheetWriter );

	if( ai_pvArg[1].Type == M4CL_CPP_TYPE_NUMBER )
	{
		iValTerminator = ai_pvArg[1].Data.DoubleData;
	}
	else
	{
		iValTerminator = *ai_pvArg[1].Data.PointerChar;
	}

	// Sets the csv string to merge.
	iRet = pCSVParser->SetCsvString( ai_pvArg[0].Data.PointerChar, iValTerminator );
	
	if( iRet == M4_SUCCESS )
	{
		// Merges the csv string.
		iRet = pCSVParser->Parse();

		if( iRet == M4_ERROR )
		{
			SETCODEF( M4_XL_ERR_PARS_GEN_ERR, ERRORLOG, "#*s1*#" );
		}
	}

	delete pCSVParser;
	delete pSheetWriter;


	CH_RETURN( &ai_pvReturn, M4_SUCCESS )
}


// -----------------------------------------------------------------------------
// Writes a cell comment.
// 
//	Arg_0: Cell row number.
//	Arg_1: Cell column number.
//	Arg_2: Comment to set.
//	Arg_3: optional. Author.
//	Arg_4: optional. Width.
//	Arg_5: optional. Height.
// -----------------------------------------------------------------------------
m4return_t SetCellComment( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t			iRet;
	ClVMRunContext*		pContext = ( ClVMRunContext* )ai_pvContext;
	Sheet*				pWorksheet;
	int					iRow, iCol;
	wchar_t				*pwcComment = NULL, *pwcAuthor = NULL;

	
	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks function arguments.
	CHECK_ARG_NUM(ai_iLongArg, 6, "SetCellComment" )

	CHECK_ARG_TYPE(ai_pvArg[0].Type, M4CL_CPP_TYPE_NUMBER, "SetCellComment")
	CHECK_ARG_TYPE(ai_pvArg[1].Type, M4CL_CPP_TYPE_NUMBER, "SetCellComment")
	CHECK_ARG_TYPE(ai_pvArg[2].Type, M4CL_CPP_TYPE_STRING_VAR, "SetCellComment")


	// Retrieves the Excel objects.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, ( void*& )pWorksheet);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WS_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "SetCellComment" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	
	// Gets the cell to write coordinates.
	iRow = ai_pvArg[0].Data.DoubleData;
	iCol = ai_pvArg[1].Data.DoubleData;

	// Comment value.
	iRet = CppStrToLibxlStr( ai_pvArg[2].Data.PointerChar, pwcComment );
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", "SetCellComment" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	// Optional author.
	if( ai_pvArg[3].Type == M4CL_CPP_TYPE_STRING_VAR && ai_pvArg[3].Data.PointerChar != NULL && *( ai_pvArg[3].Data.PointerChar) != NULL )
	{
		iRet = CppStrToLibxlStr( ai_pvArg[3].Data.PointerChar, pwcAuthor );
		if( iRet == M4_ERROR )
		{
			delete [] pwcComment;

			SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", "SetCellComment" );
			CH_RETURN( &ai_pvReturn, M4_ERROR )
		}
	}

	if( ai_pvArg[4].Type == M4CL_CPP_TYPE_NUMBER && ai_pvArg[4].Data.DoubleData > 0 && ai_pvArg[5].Type == M4CL_CPP_TYPE_NUMBER && ai_pvArg[5].Data.DoubleData > 0 )
	{
		// User specified the comment size.
		pWorksheet->writeComment( iRow, iCol, pwcComment, pwcAuthor, ai_pvArg[4].Data.DoubleData, ai_pvArg[5].Data.DoubleData );
	}
	else
	{
		// Lets libxl to choose de comment size.
		pWorksheet->writeComment( iRow, iCol, pwcComment, pwcAuthor );
	}
	

	delete [] pwcComment;

	if( pwcAuthor != NULL )
	{
		delete [] pwcAuthor;
	}


	CH_RETURN( &ai_pvReturn, iRet )
}

// -----------------------------------------------------------------------------
// Merges a set of cells.
//
//	Arg_0: Start row.
//	Arg_1: End row.
//	Arg_2: Start column.
//	Arg_3: End column.
// -----------------------------------------------------------------------------
m4return_t SetMerge( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t		iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;
	Book*			pWorkbook;
	Sheet*			pWorksheet;
	int				iStartRow, iEndRow, iStartCol, iEndCol;
	bool			bRet;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks function arguments.
	CHECK_ARG_NUM(ai_iLongArg, 4, "SetMerge" )

	CHECK_ARG_TYPE(ai_pvArg[0].Type, M4CL_CPP_TYPE_NUMBER, "SetMerge")
	CHECK_ARG_TYPE(ai_pvArg[1].Type, M4CL_CPP_TYPE_NUMBER, "SetMerge")
	CHECK_ARG_TYPE(ai_pvArg[2].Type, M4CL_CPP_TYPE_NUMBER, "SetMerge")
	CHECK_ARG_TYPE(ai_pvArg[3].Type, M4CL_CPP_TYPE_NUMBER, "SetMerge")


	// Retrieves the Excel objects.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKBOOK_POINTER, ( void*& )pWorkbook);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WB_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "SetMerge" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, ( void*& )pWorksheet);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WS_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "SetMerge" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	iStartRow	= ai_pvArg[0].Data.DoubleData;
	iEndRow		= ai_pvArg[1].Data.DoubleData;
	iStartCol	= ai_pvArg[2].Data.DoubleData;
	iEndCol		= ai_pvArg[3].Data.DoubleData;

	// Copies the cell
	bRet = pWorksheet->setMerge( iStartRow, iEndRow, iStartCol, iEndCol );

	if( bRet == false )
	{
		SETCODEF( M4_XL_ERR_MERGE_CELLS_LIB_ERR, ERRORLOG, "#*s1*#%s#%d#%d#%d#%d#", pWorkbook->errorMessage(), iStartRow, iStartCol, iEndRow, iEndCol );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	CH_RETURN( &ai_pvReturn, M4_SUCCESS )
}

// -----------------------------------------------------------------------------
// Sets the pattern foreground color by index of a single cell, a user defined range, 
// a full row range or a full column range.
// 
//	Arg_0:	Cell's row number.
//	Arg_1:	Cell's column number.
//	Arg_2:	Range initial cell.
//	Arg_3:	Range end cell.
//	Arg_4:	Row index.
//	Arg_5:	Col index.
//	Arg_6:	Color index.
// -----------------------------------------------------------------------------
m4return_t SetPatternFgColorIndex( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t	iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks number of arguments.
	CHECK_ARG_NUM(ai_iLongArg, 7, "SetPatternFgColorIndex")


	// Checks function arguments.
	CHECK_ARG_TYPE(ai_pvArg[6].Type, M4CL_CPP_TYPE_NUMBER, "SetPatternFgColorIndex")


	// Invoke the change.
	iRet = ClXlFormatsMngr::ChangeCellsAttribute( ai_pvArg, ai_pvContext, ClXlFormatsMngr::M4XL_PATTERN_FG_COLOR_INDEX );


	CH_RETURN( &ai_pvReturn, iRet )
}

// -----------------------------------------------------------------------------
// Sets the pattern foreground color by its RGB components of a single cell, a user defined range, 
// a full row range or a full column range.
// 
//	Arg_0:	Cell's row number.
//	Arg_1:	Cell's column number.
//	Arg_2:	Range initial cell.
//	Arg_3:	Range end cell.
//	Arg_4:	Row index.
//	Arg_5:	Col index.
//	Arg_6:	Red color.
//	Arg_7:	Green color.
//	Arg_8:	Blue color.
// -----------------------------------------------------------------------------
m4return_t SetPatternFgColorRGB( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t	iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks number of arguments.
	CHECK_ARG_NUM(ai_iLongArg, 9, "SetPatternFgColorRGB")


	// Checks function arguments.
	CHECK_ARG_TYPE(ai_pvArg[6].Type, M4CL_CPP_TYPE_NUMBER, "SetPatternFgColorRGB")
	CHECK_ARG_TYPE(ai_pvArg[7].Type, M4CL_CPP_TYPE_NUMBER, "SetPatternFgColorRGB")
	CHECK_ARG_TYPE(ai_pvArg[8].Type, M4CL_CPP_TYPE_NUMBER, "SetPatternFgColorRGB")


	// Invoke the change.
	iRet = ClXlFormatsMngr::ChangeCellsAttribute( ai_pvArg, ai_pvContext, ClXlFormatsMngr::M4XL_PATTERN_FG_COLOR_RGB );


	CH_RETURN( &ai_pvReturn, iRet )
}

// -----------------------------------------------------------------------------
// Get sheet count of current workbook.
//
//	Arg_0:	Output sheet count.
// -----------------------------------------------------------------------------
m4return_t GetSheetCount( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t		iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;
	Book*			pWorkbook;
	int				iCount;


	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);

	// Checks number of arguments.
	CHECK_ARG_NUM(ai_iLongArg, 1, "GetSheetCount")

	// Retrieves the Excel objects.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKBOOKS, APIChannel::WORKBOOK_POINTER, ( void*& )pWorkbook);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WB_NOT_LOADED, ERRORLOG, "#*s1*#%s#", "GetSheetCount" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	iCount = pWorkbook->sheetCount();

	ai_pvArg[0].SetDouble( iCount );


	CH_RETURN( &ai_pvReturn, M4_SUCCESS )
}


// -----------------------------------------------------------------------------
// Get platform directory separator.
// -----------------------------------------------------------------------------
m4return_t GetDirSeparator( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	ai_pvReturn.SetString(GetDirSeparator());

	return( M4_SUCCESS );
}


// -----------------------------------------------------------------------------
// Opens an existing Excel workbook partially.
// 
//	Arg_0: Path of the Excel file to open.
//	Arg_1: Sheet number.
//	Arg_2: Initial row.
//	Arg_3: Last row.
//	Arg_4: Optional. Temporary file used to improve the memory consumption.
// -----------------------------------------------------------------------------
m4return_t OpenWorkbookPartially( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext )
{
	m4return_t		iRet;
	ClVMRunContext* pContext = ( ClVMRunContext* )ai_pvContext;
	wchar_t*		pwcFile = NULL;
	m4bool_t		bIsXML;
	Book*			pWorkbook;
	bool			bRet;
	wchar_t*		pwcTmpFile = NULL;
	int				iFirstRow, iLastRow;


	CheckTraceMode(pContext->m_pAccess);
	TRACE_API_CALL(pContext, ai_pvArg, ai_iLongArg);
	
	// Checks function arguments.
	CHECK_ARG_NUM(ai_iLongArg, 5, "OpenWorkbookPartially" )
	CHECK_ARG_TYPE(ai_pvArg[0].Type, M4CL_CPP_TYPE_STRING_VAR, "OpenWorkbookPartially")
	CHECK_ARG_TYPE(ai_pvArg[1].Type, M4CL_CPP_TYPE_NUMBER, "OpenWorkbookPartially")
	CHECK_ARG_TYPE(ai_pvArg[2].Type, M4CL_CPP_TYPE_NUMBER, "OpenWorkbookPartially")
	CHECK_ARG_TYPE(ai_pvArg[3].Type, M4CL_CPP_TYPE_NUMBER, "OpenWorkbookPartially")


	// Determines the type of the file container to use.
	iRet = IsXMLWorkbook( ai_pvArg[0].Data.PointerChar, bIsXML );
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_OPEN_WB_BAD_FILE, ERRORLOG, "#*s1*#%s#", ai_pvArg[0].Data.PointerChar );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	if( bIsXML == M4_TRUE )
	{
		pWorkbook = xlCreateXMLBook();
	}
	else
	{
		pWorkbook = xlCreateBook();
	}

	if( pWorkbook == NULL )
	{
		SETCODEF( M4_XL_ERR_OPEN_WB_NULL_OBJ, ERRORLOG, "#*s1*#" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	iRet = CppStrToLibxlStr( ai_pvArg[0].Data.PointerChar, pwcFile );
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", "OpenWorkbookPartially" );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}

	iFirstRow = ai_pvArg[2].Data.DoubleData;
	iLastRow = ai_pvArg[3].Data.DoubleData;

	// Temporal file used inside load function.
	if( ai_pvArg[4].Type == M4CL_CPP_TYPE_STRING_VAR )
	{
		iRet = CppStrToLibxlStr( ai_pvArg[4].Data.PointerChar, pwcTmpFile );

		if( iRet == M4_ERROR )
		{
			SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", "OpenWorkbookPartially" );
			CH_RETURN( &ai_pvReturn, M4_ERROR )
		}
	}

	if( iLastRow > -1 && iLastRow > -1 )
	{
		bRet = pWorkbook->loadPartially( pwcFile, ai_pvArg[1].Data.DoubleData, iFirstRow, iLastRow, pwcTmpFile );
	}
	else
	{
		bRet = pWorkbook->loadSheet( pwcFile, ai_pvArg[1].Data.DoubleData, pwcTmpFile );
	}

	delete [] pwcFile;
	if( pwcTmpFile != NULL )
	{
		delete [] pwcTmpFile;
	}

	if( bRet == false )
	{
		SETCODEF( M4_XL_ERR_OPEN_WB_LIB_ERR, ERRORLOG, "#*s1*#%s#%s#", pWorkbook->errorMessage(), ai_pvArg[0].Data.PointerChar );
		CH_RETURN( &ai_pvReturn, M4_ERROR )
	}


	// Saves the xl object reference.
	APIChannel::SetItem(pContext->m_pAccess, APIChannel::WORKBOOKS, APIChannel::WORKBOOK_POINTER, pWorkbook);
	APIChannel::SetItem(pContext->m_pAccess, APIChannel::WORKBOOKS, APIChannel::WORKBOOK_PATH, ai_pvArg[0].Data.PointerChar);


	CH_RETURN( &ai_pvReturn, M4_SUCCESS )
}

/*
int testImportCSVFile( const wchar_t* ai_pwcXlsFile, const char* ai_pcCsvFile )
{
	m4return_t			iRet;
	Book*				pWorkbook;
	Sheet*				pWorksheet;
	ClXlSheetAccessor*	pSheetWriter;
	ClM4CsvParser*		pCSVParser;


	printf("[m4xlapi testImportCSVFile]\n");

	pWorkbook = xlCreateXMLBook();
	if( pWorkbook->load( ai_pwcXlsFile ) == false )
	{
		printf("ERROR loading xlsx file\n");
		return -1;
	}
	pWorksheet = pWorkbook->getSheet( 0 );
	if( !pWorksheet )
	{
		printf("ERROR getting sheet\n");
		return -1;
	}

	pSheetWriter = new ClXlSheetAccessor( 0, 0, pWorkbook, pWorksheet );
	pCSVParser = new ClM4CsvParser( pSheetWriter );

	iRet = pCSVParser->SetCsvFile( ai_pcCsvFile, ';' );

	if( iRet == M4_SUCCESS )
	{
		iRet = pCSVParser->Parse();

		if( iRet == M4_ERROR )
		{
			printf("ERROR while parsing csv\n");
		}
		else
		{
			pWorkbook->save( ai_pwcXlsFile );
		}
	}
	else
	{
		printf("ERROR setting csv fiel\n");
	}

	delete pCSVParser;
	delete pSheetWriter;

	return iRet;
}
*/