//==============================================================================
//
// (c) Copyright  1991-2008 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4xlapi
// File:             m4xlformatsmngr.cpp
// Project:			 mind3.x
// Author:           Meta Software M.S. , S.A
// Date:             03/06/2014 
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

#include <m4xlformatsmngr.hpp>
#include <m4xlhelper.hpp>
#include <m4xlapierrors.hpp>
#include <m4xlapichannel.hpp>
#include <access.hpp>
#include <wchar.h>
#include <m4thread.hpp>
#include <m4date.hpp>
#include <m4unicode.hpp>
#include <sys/stat.h>

// -----------------------------------------------------------------------------
// Class ClXlFormatsMngr
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Trace log
// -----------------------------------------------------------------------------

extern  m4bool_t s_bTraceEnabled;
#define TRACE_FORMATS_CACHE(T, U, V, W, X, Y, Z) { if (s_bTraceEnabled == M4_TRUE){ traceFormatsCache( T, U, V, W, X, Y, Z ); } }


// -----------------------------------------------------------------------------
// Class constants.
// -----------------------------------------------------------------------------
const char*	ClXlFormatsMngr::s_apcChangeNames[]			= {	"Font-Bold", "Font-Color-Index",					\
															"Font-Size", "Font-Underline",						\
															"Font-Name", "Font-Italic",							\
															"Border-Style", "Border-Color-Index",				\
															"Number-Format", "Pattern-Foreground-Color-Index",	\
															"Pattern-Foreground-Color-RGB", "Cell-Type",		\
															"Undef-Attribute" };

const int	ClXlFormatsMngr::s_iMaxRowsInNarrowRange	= 999;
const int	ClXlFormatsMngr::s_iMaxColsInNarrowRange	= 255;

// -----------------------------------------------------------------------------
// Accepts requests for changing a cell attribute for a single cell, 
// a user defined range, a full row range or a full column range.
// 
//	ai_pvChangeArgs:	LN4 arguments.
//	ai_pvContext:		M4Object context.
//	ai_iChange:			Style change.
// -----------------------------------------------------------------------------
static const int	s_iNumCommonArgs = 6;
m4return_t ClXlFormatsMngr::ChangeCellsAttribute( m4VariantType *ai_pvArg, m4pvoid_t ai_pvContext, eXlChange_t ai_iChange )
{
	m4return_t	iRet;


	if( ai_pvArg[0].Type == M4CL_CPP_TYPE_NUMBER && ai_pvArg[1].Type == M4CL_CPP_TYPE_NUMBER && ai_pvArg[0].Data.DoubleData >= 0 && ai_pvArg[1].Data.DoubleData >= 0 )
	{
		// Change single cell.
		iRet = changeSingleCellAttr( ai_pvArg[0].Data.DoubleData, ai_pvArg[1].Data.DoubleData, ai_iChange, ai_pvArg + s_iNumCommonArgs, ai_pvContext );
	}
	else if( ai_pvArg[2].Type == M4CL_CPP_TYPE_STRING_VAR && ai_pvArg[3].Type == M4CL_CPP_TYPE_STRING_VAR && strlen( ai_pvArg[2].Data.PointerChar ) > 0 && strlen( ai_pvArg[3].Data.PointerChar ) > 0 )
	{
		// Change user defined range.
		iRet = changeUserRangeAttr( ai_pvArg[2].Data.PointerChar, ai_pvArg[3].Data.PointerChar, ai_iChange, ai_pvArg + s_iNumCommonArgs, ai_pvContext );
	}
	else if( ai_pvArg[4].Type == M4CL_CPP_TYPE_NUMBER && ai_pvArg[4].Data.DoubleData >= 0 )
	{
		// Change full row range.
		iRet = changeNarrowRangeAttr( ai_pvArg[4].Data.DoubleData, -1, ai_iChange, ai_pvArg + s_iNumCommonArgs, ai_pvContext );
	}
	else if( ai_pvArg[5].Type == M4CL_CPP_TYPE_NUMBER && ai_pvArg[5].Data.DoubleData >= 0 )
	{
		// Change full column range.
		iRet = changeNarrowRangeAttr( -1, ai_pvArg[5].Data.DoubleData, ai_iChange, ai_pvArg + s_iNumCommonArgs, ai_pvContext );
	}
	else
	{
		return M4_ERROR;
	}


	return iRet;
}


// -----------------------------------------------------------------------------
// Gets the format resulting of changing another one.
//
//	ai_pFormat:			Source format.
//	ai_iChange:			Format change. 
//	ai_pvChangeArgs:	Change arguments.
//	ai_pBook:			Book object.
//	ai_pAccess:			M4Object instance.
//	ao_pNewFormat:		Output format.
// -----------------------------------------------------------------------------
m4return_t ClXlFormatsMngr::GetSuitableFormat( Format* ai_pFormat, eXlChange_t ai_iChange, m4VariantType* ai_pvChangeArgs, Book* ai_pBook, ClAccess* ai_pAccess, Format*& ao_pNewFormat )
{
	m4return_t		iRet;
	Font			*pFont, *pNewFont = NULL;
	M4ClString		strKey;
	bool			bRet;
	m4bool_t		bCanIgnore, bFontReady = M4_FALSE;
	int				iNumFormat = -1;
	wchar_t*		pwcNumFormat;
	

	ao_pNewFormat = NULL;


	if( ai_pFormat == NULL )
	{
		return M4_ERROR;
	}

	
	// Checks if the value is valid for the change.
	iRet = checkChangeValue( ai_iChange, ai_pvChangeArgs );

	if( iRet == M4_ERROR )
	{
		return M4_ERROR;
	}


	// Checks if the change is already present in the source format.
	iRet = canIgnoreChange( ai_pFormat, ai_iChange, ai_pvChangeArgs, ai_pBook, ai_pAccess, bCanIgnore );

	if( iRet == M4_ERROR )
	{
		return M4_ERROR;
	}
	
	if( bCanIgnore == M4_TRUE )
	{
		ao_pNewFormat = ai_pFormat;
		return M4_SUCCESS;
	}


	// Builds the key for the requested change.
	iRet = buildChangeKey( ai_iChange, ai_pvChangeArgs, strKey );

	if( iRet == M4_ERROR || strKey.empty() )
	{
		return M4_ERROR;
	}


	// Checks if the change against this format has already been done.
	iRet = APIChannel::GetObjTransformation(ai_pAccess, APIChannel::FORMATS, ai_pFormat, strKey, (void*&)ao_pNewFormat);

	if( iRet == M4_ERROR )
	{
		return M4_ERROR;
	}

	// Format found.
	if( ao_pNewFormat != NULL )
	{
		TRACE_FORMATS_CACHE(ai_pAccess, ai_pBook, "Reuse", "Format", ai_pFormat, strKey, ao_pNewFormat);

		return M4_SUCCESS;
	}


	// Creates a new libxl format.
	ao_pNewFormat = ai_pBook->addFormat( ai_pFormat );
	
	if( ao_pNewFormat == NULL )
	{
		return M4_ERROR;
	}

	
	// Stores the new format.
	APIChannel::SaveObjTransformation(ai_pAccess, APIChannel::FORMATS, ai_pFormat, strKey, ao_pNewFormat);

	TRACE_FORMATS_CACHE(ai_pAccess, ai_pBook, "Create", "Format", ai_pFormat, strKey, ao_pNewFormat);


	// Asks if the font object will be modified.
	if( isFontChange( ai_iChange ) == M4_TRUE )
	{
		pFont = ai_pFormat->font();

		if( pFont == NULL )
		{
			SETCODEF( M4_XL_ERR_CHNG_CELL_STYLE_LIB_ERR, ERRORLOG, "#*s1*#%s#", ai_pBook->errorMessage() );
			return M4_ERROR;
		}


		// Checks if the change against this font has already been done.
		iRet = APIChannel::GetObjTransformation(ai_pAccess, APIChannel::FONTS, pFont, strKey, (void*&)pNewFont);

		if( iRet == M4_ERROR )
		{
			return M4_ERROR;
		}

		// Format not found.
		if( pNewFont != NULL )
		{
			TRACE_FORMATS_CACHE(ai_pAccess, ai_pBook, "Reuse", "Font", pFont, strKey, pNewFont);
			bFontReady = M4_TRUE;
		}
		else
		{
			// Creates a new libxl font.
			pNewFont = ai_pBook->addFont( pFont );	

			if( pNewFont == NULL )
			{
				return M4_ERROR;
			}

			// Stores the new font.
			APIChannel::SaveObjTransformation(ai_pAccess, APIChannel::FONTS, pFont, strKey, pNewFont);

			TRACE_FORMATS_CACHE(ai_pAccess, ai_pBook, "Create", "Font", pFont, strKey, pNewFont);
		}


		// Binds format with font.
		bRet = ao_pNewFormat->setFont( pNewFont );

		if( bRet == false )
		{
			SETCODEF( M4_XL_ERR_CHNG_CELL_STYLE_LIB_ERR, ERRORLOG, "#*s1*#%s#", ai_pBook->errorMessage() );
			return M4_ERROR;
		}

		
		// Format recovered from the store so not need to apply the change.
		if( bFontReady == M4_TRUE )
		{
			return M4_SUCCESS;
		}
	}

	if( ai_iChange == M4XL_NUMBERFORMAT )
	{
		iRet = APIChannel::GetNumberFormat( ai_pvChangeArgs[0].Data.PointerChar, ai_pAccess, iNumFormat );

		if( iRet == M4_ERROR )
		{
			return M4_ERROR;
		}

		if( iNumFormat == -1 )
		{
			iRet = CppStrToLibxlStr( ai_pvChangeArgs[0].Data.PointerChar, pwcNumFormat );
			if( iRet == M4_ERROR )
			{
				SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", getChangeName( ai_iChange ) );
				return M4_ERROR;
			}

			iNumFormat = ai_pBook->addCustomNumFormat( pwcNumFormat );

			delete[] pwcNumFormat;

			// Stores the new numformat.
			storeNumFormat( ai_pBook, iNumFormat, ai_pvChangeArgs[0].Data.PointerChar, ai_pAccess );
		}
		else
		{
			TRACE_FORMATS_CACHE(ai_pAccess, ai_pBook, "Reuse", "NumFormat", 0, ai_pvChangeArgs[0].Data.PointerChar, (void*)iNumFormat);
		}
	}


	// Performs the requested Change.
	iRet = applyChange( ao_pNewFormat, pNewFont, iNumFormat, ai_iChange, ai_pvChangeArgs, ai_pBook );


	return iRet;
}


// -----------------------------------------------------------------------------
// Changes an attribute of a single cell.
// 
//	ai_iRow:			Cell's row number.
//	ai_iCol:			Cell's column number.
//	ai_iChange:			Style Change.
//	ai_pvChangeArgs:	Change arguments.
//	ai_pvContext:		M4Object context.
// -----------------------------------------------------------------------------
m4return_t ClXlFormatsMngr::changeSingleCellAttr( int ai_iRow, int ai_iCol, eXlChange_t ai_iChange, m4VariantType* ai_pvChangeArgs, m4pvoid_t ai_pvContext )
{
	m4return_t			iRet;
	ClVMRunContext*		pContext = ( ClVMRunContext* )ai_pvContext;
	Book*				pWorkbook;
	Sheet*				pWorksheet;
	Format				*pFormat, *pNewFormat;
	const char*			pcMethod;
	

	pcMethod = getChangeName( ai_iChange );


	// Retrieves the Excel objects.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKBOOK_POINTER, ( void*& )pWorkbook);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WB_NOT_LOADED, ERRORLOG, "#*s1*#%s#", pcMethod );
		return M4_ERROR;
	}

	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, ( void*& )pWorksheet);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WS_NOT_LOADED, ERRORLOG, "#*s1*#%s#", pcMethod );
		return M4_ERROR;
	}


	pFormat	= pWorksheet->cellFormat( ai_iRow, ai_iCol );


	// Gets a new format for the cell or an already created one.
	iRet = GetSuitableFormat( pFormat, ai_iChange, ai_pvChangeArgs, pWorkbook, pContext->m_pAccess, pNewFormat );

	if( iRet == M4_ERROR || pNewFormat == NULL )
	{
		SETCODEF( M4_XL_ERR_CHANGE_STYLE_GEN, ERRORLOG, "#*s1*#%s#%d#%d#", pcMethod, ai_iRow, ai_iCol );
		return M4_ERROR;
	}


	// Could be that the change request was already done.
	if( pFormat != pNewFormat )
	{
		// Change the format of a cell.
		pWorksheet->setCellFormat( ai_iRow, ai_iCol, pNewFormat );
	}


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Changes an attribute of the cells in a user defined range.
// 
//	ai_pcStartCell:		Range start cell.
//	ai_pcEndCell:		Range end cell.
//	ai_iChange:			Style Change.
//	ai_pvChangeArgs:	Change arguments.
//	ai_pvContext:		M4Object context.
// -----------------------------------------------------------------------------
m4return_t ClXlFormatsMngr::changeUserRangeAttr( char* ai_pcStartCell, char* ai_pcEndCell, eXlChange_t ai_iChange, m4VariantType* ai_pvChangeArgs, m4pvoid_t ai_pvContext )
{
	m4return_t			iRet;
	ClVMRunContext*		pContext = ( ClVMRunContext* )ai_pvContext;
	Sheet*				pWorksheet;
	wchar_t*			pwcCellRef;
	const char*			pcMethod;
	int					iStartRow, iStartCol, iEndRow, iEndCol;


	pcMethod = getChangeName( ai_iChange );


	// Retrieves the Excel objects.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, ( void*& )pWorksheet);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WS_NOT_LOADED, ERRORLOG, "#*s1*#%s#", pcMethod );
		return M4_ERROR;
	}


	// Gets the coordinates of the start and end of range.
	iRet = CppStrToLibxlStr( ai_pcStartCell, pwcCellRef );
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", "changeUserRangeAttr" );
		return M4_ERROR;
	}

	pWorksheet->addrToRowCol( pwcCellRef, &iStartRow, &iStartCol );

	delete[] pwcCellRef;

	iRet = CppStrToLibxlStr( ai_pcEndCell, pwcCellRef );
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", "changeUserRangeAttr" );
		return M4_ERROR;
	}

	pWorksheet->addrToRowCol( pwcCellRef, &iEndRow, &iEndCol );

	delete[] pwcCellRef;

	
	// Performs the changes.
	iRet = applyChangeUserRangeAttr(iStartRow, iEndRow, iStartCol, iEndCol, ai_iChange, ai_pvChangeArgs, ai_pvContext);


	return iRet;
}


// -----------------------------------------------------------------------------
// Changes an attribute of a one-dimension range (row or column).
// 
//	ai_iRow:			Row number.
//	ai_iCol:			Column number.
//	ai_iChange:			Style Change.
//	ai_pvChangeArgs:	Change arguments.
//	ai_pvContext:		M4Object context.
// -----------------------------------------------------------------------------
m4return_t ClXlFormatsMngr::changeNarrowRangeAttr( int ai_iRow, int ai_iCol, eXlChange_t ai_iChange, m4VariantType* ai_pvChangeArgs, m4pvoid_t ai_pvContext )
{
	m4return_t			iRet;
	ClVMRunContext*		pContext = ( ClVMRunContext* )ai_pvContext;
	Book*				pWorkbook;
	Sheet*				pWorksheet;
	Format				*pFormat, *pNewFormat;
	const char*			pcMethod;
	int					iRow, iCol, iLastRow, iLastCol;
	bool				bRet;
	char				*pcStartCell, *pcEndCell;


	pcMethod = getChangeName( ai_iChange );


	// Retrieves the Excel objects.
	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKBOOK_POINTER, ( void*& )pWorkbook);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WB_NOT_LOADED, ERRORLOG, "#*s1*#%s#", pcMethod );
		return M4_ERROR;
	}

	iRet = APIChannel::GetItem(pContext->m_pAccess, APIChannel::WORKSHEETS, APIChannel::WORKSHEET_POINTER, ( void*& )pWorksheet);
	if( iRet == M4_ERROR )
	{
		SETCODEF( M4_XL_ERR_WS_NOT_LOADED, ERRORLOG, "#*s1*#%s#", pcMethod );
		return M4_ERROR;
	}

	iRow = ai_iRow;
	iCol = ai_iCol;

	if( iRow == -1 )
	{
		iLastCol = pWorksheet->lastCol();

		// The same format can be applied in all the column. 
		if( iLastCol <= ai_iCol )
		{
			iRow = 0;
		}
		else
		{
			// Change individually the format of each cell in the column.
			iRet = LibxlStrToCppStr( pWorksheet->rowColToAddr( 0, ai_iCol ), pcStartCell );
			if( iRet == M4_ERROR )
			{
				SETCODEF( M4_XL_ERR_GEN_XL_2_CPP, ERRORLOG, "#*s1*#%s#", "ChangeColumnRangeFormat" );
				return M4_ERROR;
			}

			iRet = LibxlStrToCppStr( pWorksheet->rowColToAddr( s_iMaxRowsInNarrowRange, ai_iCol ), pcEndCell );
			if( iRet == M4_ERROR )
			{
				delete[] pcStartCell;
				SETCODEF( M4_XL_ERR_GEN_XL_2_CPP, ERRORLOG, "#*s1*#%s#", "ChangeColumnRangeFormat" );
				return M4_ERROR;
			}

			iRet = changeUserRangeAttr( pcStartCell, pcEndCell, ai_iChange, ai_pvChangeArgs, ai_pvContext );

			delete[] pcStartCell;
			delete[] pcEndCell;

			return iRet;
		}
	}
	else if( iCol == -1 )
	{
		iLastRow = pWorksheet->lastRow();

		// The same format can be applied in all the row. 
		if( iLastRow <= ai_iRow )
		{
			iCol = 0;
		}
		else
		{
			// Change individually the format of each cell in the row.
			iRet = LibxlStrToCppStr( pWorksheet->rowColToAddr( ai_iRow, 0 ), pcStartCell );
			if( iRet == M4_ERROR )
			{
				SETCODEF( M4_XL_ERR_GEN_XL_2_CPP, ERRORLOG, "#*s1*#%s#", "ChangeRowRangeFormat" );
				return M4_ERROR;
			}

			iRet = LibxlStrToCppStr( pWorksheet->rowColToAddr( ai_iRow, s_iMaxColsInNarrowRange ), pcEndCell );
			if( iRet == M4_ERROR )
			{
				delete[] pcStartCell;
				SETCODEF( M4_XL_ERR_GEN_XL_2_CPP, ERRORLOG, "#*s1*#%s#", "ChangeRowRangeFormat" );
				return M4_ERROR;
			}

			iRet = changeUserRangeAttr( pcStartCell, pcEndCell, ai_iChange, ai_pvChangeArgs, ai_pvContext );

			delete[] pcStartCell;
			delete[] pcEndCell;

			return iRet;
		}
	}

	pFormat	= pWorksheet->cellFormat( iRow, iCol );

	// Gets a new format for the cell or an already created one.
	iRet = GetSuitableFormat( pFormat, ai_iChange, ai_pvChangeArgs, pWorkbook, pContext->m_pAccess, pNewFormat );

	if( iRet == M4_ERROR || pNewFormat == NULL )
	{
		SETCODEF( M4_XL_ERR_CHANGE_STYLE_GEN, ERRORLOG, "#*s1*#%s#%d#%d#", pcMethod, ai_iRow, ai_iCol );
		return M4_ERROR;
	}


	// Could be that the change request was already done.
	if( pFormat != pNewFormat )
	{
		if( ai_iCol == -1 )
		{
			// Change the format of a row.
			bRet = pWorksheet->setRow( ai_iRow, pWorksheet->rowHeight( ai_iRow ), pNewFormat );

			if( bRet == false)
			{
				SETCODEF( M4_XL_ERR_CHNG_ROW_STYLE_LIB_ERR_DET, ERRORLOG, "#*s1*#%s#%s#%d#", pWorkbook->errorMessage(), pcMethod, ai_iRow );
				return M4_ERROR;
			}
		}
		else
		{
			// Change the format of a column.
			bRet = pWorksheet->setCol( ai_iCol, ai_iCol, pWorksheet->colWidth( ai_iCol ), pNewFormat);

			if( bRet == false)
			{
				SETCODEF( M4_XL_ERR_CHNG_ROW_STYLE_LIB_ERR_DET, ERRORLOG, "#*s1*#%s#%s#%d#", pWorkbook->errorMessage(), pcMethod, ai_iCol );
				return M4_ERROR;
			}
		}
	}


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Apply the changes in the cells of a user range.
//
//	ai_iStartRow:		Range start row.
//	ai_iEndRow:			Range end row.
//	ai_iStartCol:		Range start column.
//	ai_iEndCol:			Range end column.
//	ai_iChange:			Style Change.
//	ai_pvChangeArgs:	Change arguments.
//	ai_pvContext:		M4Object context.
// -----------------------------------------------------------------------------
m4return_t ClXlFormatsMngr::applyChangeUserRangeAttr(int ai_iStartRow, int ai_iEndRow, int ai_iStartCol, int ai_iEndCol, eXlChange_t ai_iChange, m4VariantType* ai_pvChangeArgs, m4pvoid_t ai_pvContext )
{
	m4return_t			iRet;
	eXlBorderEdges_t	iBorderEdge;
	bool				bOutside = false;
	m4VariantType		vChangeArgs[ 3 ];


	if( ai_iChange == M4XL_BORDER_STYLE )
	{
		iBorderEdge = ( eXlBorderEdges_t )( int )ai_pvChangeArgs[1].Data.DoubleData;
		bOutside	= ( bool )ai_pvChangeArgs[2].Data.DoubleData;
	}

	if( ai_iChange != M4XL_BORDER_STYLE || bOutside == false )
	{
		// Performs the change in all the cells.
		for( int iRowIdx = ai_iStartRow; iRowIdx <= ai_iEndRow; iRowIdx++ )
		{
			for( int iColIdx = ai_iStartCol; iColIdx <= ai_iEndCol; iColIdx++ )
			{
				iRet = changeSingleCellAttr( iRowIdx, iColIdx, ai_iChange, ai_pvChangeArgs, ai_pvContext );

				if( iRet == M4_ERROR )
				{
					return M4_ERROR;
				}
			}
		}
	}
	else
	{
		// Performs the change only in the external shape of the range.

		// Common arguments.
		vChangeArgs[ 0 ] = ai_pvChangeArgs[ 0 ];
		vChangeArgs[ 2 ] = ai_pvChangeArgs[ 2 ];

		// Top-left corner.
		if( iBorderEdge & ( M4XL_EDGE_TOP | M4XL_EDGE_LEFT ) )
		{
			vChangeArgs[ 1 ] = iBorderEdge & ( M4XL_EDGE_TOP | M4XL_EDGE_LEFT );

			iRet = changeSingleCellAttr( ai_iStartRow, ai_iStartCol, ai_iChange, vChangeArgs, ai_pvContext );

			if( iRet == M4_ERROR )
			{
				return M4_ERROR;
			}
		}

		// Top-right corner.
		if( iBorderEdge & ( M4XL_EDGE_TOP | M4XL_EDGE_RIGHT ) )
		{
			vChangeArgs[ 1 ] = iBorderEdge & ( M4XL_EDGE_TOP | M4XL_EDGE_RIGHT );
		
			iRet = changeSingleCellAttr( ai_iStartRow, ai_iEndCol, ai_iChange, vChangeArgs, ai_pvContext );

			if( iRet == M4_ERROR )
			{
				return M4_ERROR;
			}
		}

		// Bottom-right corner.
		if( iBorderEdge & ( M4XL_EDGE_BOTTOM | M4XL_EDGE_RIGHT ) )
		{
			vChangeArgs[ 1 ] = iBorderEdge & ( M4XL_EDGE_BOTTOM | M4XL_EDGE_RIGHT );

			iRet = changeSingleCellAttr( ai_iEndRow, ai_iEndCol, ai_iChange, vChangeArgs, ai_pvContext );

			if( iRet == M4_ERROR )
			{
				return M4_ERROR;
			}
		}

		// Bottom-left corner.
		if( iBorderEdge & ( M4XL_EDGE_BOTTOM | M4XL_EDGE_LEFT ) )
		{
			vChangeArgs[ 1 ] = iBorderEdge & ( M4XL_EDGE_BOTTOM | M4XL_EDGE_LEFT );

			iRet = changeSingleCellAttr( ai_iEndRow, ai_iStartCol, ai_iChange, vChangeArgs, ai_pvContext );

			if( iRet == M4_ERROR )
			{
				return M4_ERROR;
			}
		}

		// Top line without corners.
		if( iBorderEdge & M4XL_EDGE_TOP )
		{
			vChangeArgs[ 1 ] = M4XL_EDGE_TOP;

			for( int iColIdx = ai_iStartCol + 1; iColIdx < ai_iEndCol; iColIdx++ )
			{
				iRet = changeSingleCellAttr( ai_iStartRow, iColIdx, ai_iChange, vChangeArgs, ai_pvContext );

				if( iRet == M4_ERROR )
				{
					return M4_ERROR;
				}
			}
		}

		// Right line without corners.
		if( iBorderEdge & M4XL_EDGE_RIGHT )
		{
			vChangeArgs[ 1 ] = M4XL_EDGE_RIGHT;

			for( int iRowIdx = ai_iStartRow + 1; iRowIdx < ai_iEndRow; iRowIdx++ )
			{
				iRet = changeSingleCellAttr( iRowIdx, ai_iEndCol, ai_iChange, vChangeArgs, ai_pvContext );

				if( iRet == M4_ERROR )
				{
					return M4_ERROR;
				}
			}
		}

		// Bottom line without corners.
		if( iBorderEdge & M4XL_EDGE_BOTTOM )
		{
			vChangeArgs[ 1 ] = M4XL_EDGE_BOTTOM;

			for( int iColIdx = ai_iStartCol + 1; iColIdx < ai_iEndCol; iColIdx++ )
			{
				iRet = changeSingleCellAttr( ai_iEndRow, iColIdx, ai_iChange, vChangeArgs, ai_pvContext );

				if( iRet == M4_ERROR )
				{
					return M4_ERROR;
				}
			}
		}

		// Left line without corners.
		if( iBorderEdge & M4XL_EDGE_LEFT )
		{
			vChangeArgs[ 1 ] = M4XL_EDGE_LEFT;

			for( int iRowIdx = ai_iStartRow + 1; iRowIdx < ai_iEndRow; iRowIdx++ )
			{
				iRet = changeSingleCellAttr( iRowIdx, ai_iStartCol, ai_iChange, vChangeArgs, ai_pvContext );

				if( iRet == M4_ERROR )
				{
					return M4_ERROR;
				}
			}
		}
	}


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Gets the format-change operation name for a given code.
//
//	ai_iChange:	Format change. 
// -----------------------------------------------------------------------------
const char*	ClXlFormatsMngr::getChangeName( eXlChange_t ai_iChange )
{
	const char*	pcName;


	if( ai_iChange < M4XL_TOP_ITEM )
	{
		pcName = s_apcChangeNames[ ai_iChange ];
	}
	else
	{
		pcName = s_apcChangeNames[ M4XL_TOP_ITEM ];
	}


	return pcName;
}


// -----------------------------------------------------------------------------
// Checks if a value is right for the specified change.
//
//	ai_iChange:			Format change. 
//	ai_pvChangeArgs:	Change arguments.
// -----------------------------------------------------------------------------
m4return_t ClXlFormatsMngr::checkChangeValue( eXlChange_t ai_iChange, m4VariantType* ai_pvChangeArgs)
{
	switch( ai_iChange )
	{
		case M4XL_FONT_BOLD:
			break;

		case M4XL_FONT_COLOR_INDEX:

			if( checkColorIndexValue( getXlColorIndex( ai_pvChangeArgs[0].Data.DoubleData ) ) == M4_ERROR )
			{
				SETCODEF( M4_XL_ERR_CHANGE_STYLE_BAD_COLOR_INDEX, ERRORLOG, "#*s1*#%d#", ai_pvChangeArgs[0].Data.DoubleData );
				return M4_ERROR;
			}
			break;

		case M4XL_FONT_SIZE:
			break;

		case M4XL_FONT_UNDERLINE:

			if( checkUnderlineValue( getXlUnderlineStyle( ai_pvChangeArgs[0].Data.DoubleData ) ) == M4_ERROR )
			{
				SETCODEF( M4_XL_ERR_CHANGE_STYLE_BAD_UNDERLINE_TYPE, ERRORLOG, "#*s1*#%d#", ai_pvChangeArgs[0].Data.DoubleData );
				return M4_ERROR;
			}
			break;

		case M4XL_FONT_NAME:
			break;

		case M4XL_FONT_ITALIC:
			break;

		case M4XL_BORDER_STYLE:

			if( checkBorderStyleValue( getXlBorderStyle( ai_pvChangeArgs[0].Data.DoubleData ) ) == M4_ERROR )
			{
				SETCODEF( M4_XL_ERR_CHANGE_STYLE_BAD_BORDER_TYPE, ERRORLOG, "#*s1*#%d#", ai_pvChangeArgs[0].Data.DoubleData );
				return M4_ERROR;
			}

			if( checkBorderEdgeValue( ai_pvChangeArgs[1].Data.DoubleData ) == M4_ERROR )
			{
				SETCODEF( M4_XL_ERR_CHANGE_STYLE_BAD_BORDER_EDGE, ERRORLOG, "#*s1*#%d#", ai_pvChangeArgs[1].Data.DoubleData );
				return M4_ERROR;
			}
			break;
			
		case M4XL_BORDER_COLOR_INDEX:

			if( checkColorIndexValue( getXlColorIndex( ai_pvChangeArgs[0].Data.DoubleData ) ) == M4_ERROR )
			{
				SETCODEF( M4_XL_ERR_CHANGE_STYLE_BAD_COLOR_INDEX, ERRORLOG, "#*s1*#%d#", ai_pvChangeArgs[0].Data.DoubleData );
				return M4_ERROR;
			}
			
			if( checkBorderEdgeValue( ai_pvChangeArgs[1].Data.DoubleData ) == M4_ERROR )
			{
				SETCODEF( M4_XL_ERR_CHANGE_COLOR_BAD_BORDER_EDGE, ERRORLOG, "#*s1*#%d#", ai_pvChangeArgs[1].Data.DoubleData );
				return M4_ERROR;
			}
			break;

		case M4XL_NUMBERFORMAT:

			if( checkNumberFormatValue( ai_pvChangeArgs[0].Data.PointerChar ) == M4_ERROR )
			{
				SETCODEF( M4_XL_ERR_CHANGE_NUMBERFORMAT_BAD_ARG, ERRORLOG, "#*s1*#" );
				return M4_ERROR;
			}
			break;

		case M4XL_PATTERN_FG_COLOR_INDEX:

			if( checkColorIndexValue( getXlColorIndex( ai_pvChangeArgs[0].Data.DoubleData ) ) == M4_ERROR )
			{
				SETCODEF( M4_XL_ERR_CHANGE_STYLE_BAD_COLOR_INDEX, ERRORLOG, "#*s1*#%d#", (int)ai_pvChangeArgs[0].Data.DoubleData );
				return M4_ERROR;
			}

			break;

		case M4XL_PATTERN_FG_COLOR_RGB:

			if( checkRGBComponentValue( ai_pvChangeArgs[0].Data.DoubleData ) == M4_ERROR )
			{
				SETCODEF( M4_XL_ERR_CHANGE_STYLE_BAD_COLOR_RGB, ERRORLOG, "#*s1*#%d#", (int)ai_pvChangeArgs[0].Data.DoubleData );
				return M4_ERROR;
			}
			if( checkRGBComponentValue( ai_pvChangeArgs[1].Data.DoubleData ) == M4_ERROR )
			{
				SETCODEF( M4_XL_ERR_CHANGE_STYLE_BAD_COLOR_RGB, ERRORLOG, "#*s1*#%d#", (int)ai_pvChangeArgs[1].Data.DoubleData );
				return M4_ERROR;
			}
			if( checkRGBComponentValue( ai_pvChangeArgs[2].Data.DoubleData ) == M4_ERROR )
			{
				SETCODEF( M4_XL_ERR_CHANGE_STYLE_BAD_COLOR_RGB, ERRORLOG, "#*s1*#%d#", (int)ai_pvChangeArgs[2].Data.DoubleData );
				return M4_ERROR;
			}

			break;

		case M4XL_CELL_TYPE:
			
			if( checkCellTypeValue( getXlNumberFormat( ai_pvChangeArgs[0].Data.DoubleData ) ) == M4_ERROR )
			{
				SETCODEF( M4_XL_ERR_CHANGE_NUMBERFORMAT_BAD_ARG, ERRORLOG, "#*s1*#" );
				return M4_ERROR;
			}
			break;

		default:
			return M4_ERROR;
	}


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Checks if the requested change is already done in the source format.
//
//	ai_pFormat:			Source format.
//	ai_iChange:			Format change. 
//	ai_pvChangeArgs:	Change arguments.
//	ai_pBook:			Book object.
//	ao_bCanIgnore:		The change can be ignored (true/false).
// -----------------------------------------------------------------------------
m4return_t ClXlFormatsMngr::canIgnoreChange( Format* ai_pFormat, eXlChange_t ai_iChange, m4VariantType* ai_pvChangeArgs, Book* ai_pBook, ClAccess* ai_pAccess, m4bool_t& ao_bCanIgnore )
{
	m4return_t			iRet;
	Font				*pFont = NULL;
	char*				pcName;
	int					iCmp;
	BorderStyle			iBorderStyle;
	eXlBorderEdges_t	iBorderEdge;
	Color				iColor;
	int					iNumFormat;
	const wchar_t*		pwcNumFormat;
	char*				pcNumFormat;
	int					iRed, iGreen, iBlue;

	ao_bCanIgnore = M4_FALSE;


	// If the font object will be modified retrieves a reference.
	if( isFontChange( ai_iChange ) == M4_TRUE )
	{
		pFont = ai_pFormat->font();
		if( pFont == NULL )
		{
			SETCODEF( M4_XL_ERR_CHNG_CELL_STYLE_LIB_ERR, ERRORLOG, "#*s1*#%s#", ai_pBook->errorMessage() );
			return M4_ERROR;
		}
	}


	// Specific change type checking.
	switch( ai_iChange )
	{
		case M4XL_FONT_BOLD:
			
			if( pFont->bold() == ( bool )ai_pvChangeArgs[0].Data.DoubleData )
			{
				ao_bCanIgnore = M4_TRUE;
				return M4_SUCCESS;
			}
			break;

		case M4XL_FONT_COLOR_INDEX:

			{
				ClXlRGBModeRestorer oRGBModeRestorer( ai_pBook );
				ai_pBook->setRgbMode( false );

				if( pFont->color() == getXlColorIndex( ai_pvChangeArgs[0].Data.DoubleData ) )
				{
					ao_bCanIgnore = M4_TRUE;
					return M4_SUCCESS;
				}
			}
			break;

		case M4XL_FONT_SIZE:
				
			if( pFont->size() == ( int )ai_pvChangeArgs[0].Data.DoubleData )
			{
				ao_bCanIgnore = M4_TRUE;
				return M4_SUCCESS;
			}
			break;

		case M4XL_FONT_UNDERLINE:

			if( pFont->underline() == getXlUnderlineStyle( ai_pvChangeArgs[0].Data.DoubleData ) )
			{
				ao_bCanIgnore = M4_TRUE;
				return M4_SUCCESS;
			}
			break;

		case M4XL_FONT_NAME:

			iRet = LibxlStrToCppStr( pFont->name(), pcName );
			if( iRet == M4_ERROR )
			{
				SETCODEF( M4_XL_ERR_GEN_XL_2_CPP, ERRORLOG, "#*s1*#%s#", "canIgnoreChange" );
				return M4_ERROR;
			}

			iCmp = stricmp( pcName, ai_pvChangeArgs[0].Data.PointerChar );

			delete[] pcName;

			if( iCmp == 0 )
			{
				ao_bCanIgnore = M4_TRUE;
				return M4_SUCCESS;
			}
			break;

		case M4XL_FONT_ITALIC:
			
			if( pFont->italic() == ( bool )ai_pvChangeArgs[0].Data.DoubleData )
			{
				ao_bCanIgnore = M4_TRUE;
				return M4_SUCCESS;
			}
			break;

		case M4XL_BORDER_STYLE:
			
			iBorderStyle = getXlBorderStyle( ai_pvChangeArgs[0].Data.DoubleData );

			iBorderEdge = ( eXlBorderEdges_t )( int )ai_pvChangeArgs[1].Data.DoubleData;

			if( ( ( iBorderEdge & M4XL_EDGE_TOP ) && iBorderStyle != ai_pFormat->borderTop() ) ||
				( ( iBorderEdge & M4XL_EDGE_RIGHT ) && iBorderStyle != ai_pFormat->borderRight() ) ||
				( ( iBorderEdge & M4XL_EDGE_BOTTOM ) && iBorderStyle != ai_pFormat->borderBottom() ) ||
				( ( iBorderEdge & M4XL_EDGE_LEFT ) && iBorderStyle != ai_pFormat->borderLeft() ) )
			{
				ao_bCanIgnore = M4_FALSE;
			}
			else
			{
				ao_bCanIgnore = M4_TRUE;
				return M4_SUCCESS;
			}
			break;

		case M4XL_BORDER_COLOR_INDEX:
			
			{
				ClXlRGBModeRestorer oRGBModeRestorer( ai_pBook );
				ai_pBook->setRgbMode( false );

				iColor = getXlColorIndex( ai_pvChangeArgs[0].Data.DoubleData );

				iBorderEdge = ( eXlBorderEdges_t )( int )ai_pvChangeArgs[1].Data.DoubleData;

				if( ( ( iBorderEdge & M4XL_EDGE_TOP ) && iColor != ai_pFormat->borderTopColor() ) ||
					( ( iBorderEdge & M4XL_EDGE_RIGHT ) && iColor != ai_pFormat->borderRightColor() ) ||
					( ( iBorderEdge & M4XL_EDGE_BOTTOM ) && iColor != ai_pFormat->borderBottomColor() ) ||
					( ( iBorderEdge & M4XL_EDGE_LEFT ) && iColor != ai_pFormat->borderLeftColor() ) )
				{
					ao_bCanIgnore = M4_FALSE;
				}
				else
				{
					ao_bCanIgnore = M4_TRUE;
					return M4_SUCCESS;
				}
			}
			break;

		case M4XL_NUMBERFORMAT:

			iNumFormat = ai_pFormat->numFormat();
			pwcNumFormat = ai_pBook->customNumFormat( iNumFormat );

			if( pwcNumFormat != NULL )
			{
				// Current format has a custom number format.
				iRet = LibxlStrToCppStr( pwcNumFormat, pcNumFormat );
				if( iRet == M4_ERROR )
				{
					SETCODEF( M4_XL_ERR_GEN_XL_2_CPP, ERRORLOG, "#*s1*#%s#", "canIgnoreChange" );
					return M4_ERROR;
				}

				iCmp = stricmp( ai_pvChangeArgs[0].Data.PointerChar, pcNumFormat );

				delete[] pcNumFormat;

				if( iCmp == 0 )
				{
					ao_bCanIgnore = M4_TRUE;
					return M4_SUCCESS;
				}
			}
			else
			{
				// Current format should have a built-in number format.
				iRet = APIChannel::GetNumberFormat( ai_pvChangeArgs[0].Data.PointerChar, ai_pAccess, iNumFormat );

				if( iRet == M4_ERROR )
				{
					return M4_ERROR;
				}

				if( iNumFormat == ai_pFormat->numFormat() )
				{
					ao_bCanIgnore = M4_TRUE;
					return M4_SUCCESS;
				}
			}
			break;

		case M4XL_PATTERN_FG_COLOR_INDEX:
			
			{
				ClXlRGBModeRestorer oRGBModeRestorer( ai_pBook );
				ai_pBook->setRgbMode( false );

				iColor = getXlColorIndex( ai_pvChangeArgs[0].Data.DoubleData );

				if( ai_pFormat->patternForegroundColor() == iColor )
				{
					ao_bCanIgnore = M4_TRUE;
					return M4_SUCCESS;
				}
			}
			break;

		case M4XL_PATTERN_FG_COLOR_RGB:

			{
				ClXlRGBModeRestorer oRGBModeRestorer( ai_pBook );
				ai_pBook->setRgbMode( true );

				iColor = ai_pFormat->patternForegroundColor();
				ai_pBook->colorUnpack( iColor, &iRed, &iGreen, &iBlue );

				if( ai_pvChangeArgs[0].Data.DoubleData == iRed && ai_pvChangeArgs[1].Data.DoubleData == iGreen && ai_pvChangeArgs[2].Data.DoubleData == iBlue )
				{
					ao_bCanIgnore = M4_TRUE;
					return M4_SUCCESS;
				}
			}
			break;

		case M4XL_CELL_TYPE:

			iNumFormat = ai_pFormat->numFormat();

			if( iNumFormat == ai_pvChangeArgs[0].Data.DoubleData )
			{
				ao_bCanIgnore = M4_TRUE;
				return M4_SUCCESS;
			}
			break;

		default:

			return M4_ERROR;
	}


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Builds the key that identifies a change operation.
//
//	ai_iChange:			Format change. 
//	ai_pvChangeArgs:	Change arguments.
//	ao_strKey:			Output change key.
// -----------------------------------------------------------------------------
m4return_t ClXlFormatsMngr::buildChangeKey( eXlChange_t ai_iChange, m4VariantType* ai_pvChangeArgs, M4ClString& ao_strKey )
{
	char	iType;
	int		iNumArgs;

	switch( ai_iChange )
	{
		case M4XL_FONT_BOLD:
		case M4XL_FONT_COLOR_INDEX:
		case M4XL_FONT_SIZE:
		case M4XL_FONT_UNDERLINE:
		case M4XL_FONT_NAME:
		case M4XL_FONT_ITALIC:
		case M4XL_NUMBERFORMAT:
		case M4XL_PATTERN_FG_COLOR_INDEX:
		case M4XL_CELL_TYPE:
			iNumArgs = 1;
			break;
		case M4XL_BORDER_STYLE:
		case M4XL_BORDER_COLOR_INDEX:
			iNumArgs = 2;
			break;
		case M4XL_PATTERN_FG_COLOR_RGB:
			iNumArgs = 3;
			break;
		default:
			return M4_ERROR;
	}


	ao_strKey = "";
	ao_strKey += ai_iChange;

	for( int i = 0; i < iNumArgs; i++ )
	{
		ao_strKey += ":";

		iType = ai_pvChangeArgs[i].GetType();

		switch( iType )
		{
			case M4CL_CPP_TYPE_NUMBER:
				
				ao_strKey += ai_pvChangeArgs[i].Data.DoubleData;
				break;

			case M4CL_CPP_TYPE_STRING_VAR:

				ao_strKey += ai_pvChangeArgs[i].Data.PointerChar;
				break;

			default:

				SETCODEF( M4_XL_ERR_CHANGE_STYLE_BAD_VAL_TYPE, ERRORLOG, "#*s1*#" );
				return M4_ERROR;
		}
	}


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Returns if this change affects the font object.
//
//	ai_iChange:		Format change. 
// -----------------------------------------------------------------------------
m4bool_t ClXlFormatsMngr::isFontChange( eXlChange_t ai_iChange )
{
	if( ai_iChange <= M4XL_FONT_ITALIC )
	{
		return M4_TRUE;
	}
	else
	{
		return M4_FALSE;
	}
}


// -----------------------------------------------------------------------------
// Performs the change.
//
//	ai_pFormat:			Source format.
//	ai_pFont:			Source font.
//	ai_iChange:			Format or font change. 
//	ai_pvChangeArgs:	Change arguments.
// -----------------------------------------------------------------------------
m4return_t ClXlFormatsMngr::applyChange( Format* ai_pFormat, Font* ai_pFont, int ai_iNumFormat, eXlChange_t ai_iChange, m4VariantType* ai_pvChangeArgs, Book* ai_pBook )
{
	m4return_t			iRet;
	wchar_t*			pwcName;
	eXlBorderEdges_t	iBorderEdge;
	FillPattern			iFillPattern;
	Color				iColor;


	// Performs the specific change requested.
	switch( ai_iChange )
	{
		case M4XL_FONT_BOLD:

			ai_pFont->setBold( ( bool )ai_pvChangeArgs[0].Data.DoubleData );
			break;

		case M4XL_FONT_COLOR_INDEX:
		
			{
				ClXlRGBModeRestorer oRGBModeRestorer( ai_pBook );
				ai_pBook->setRgbMode( false );
			
				ai_pFont->setColor( getXlColorIndex( ai_pvChangeArgs[0].Data.DoubleData ) );
			}
			break;

		case M4XL_FONT_SIZE:

			ai_pFont->setSize( ( int )ai_pvChangeArgs[0].Data.DoubleData );
			break;

		case M4XL_FONT_UNDERLINE:

			ai_pFont->setUnderline( getXlUnderlineStyle( ai_pvChangeArgs[0].Data.DoubleData ) );
			break;

		case M4XL_FONT_NAME:

			iRet = CppStrToLibxlStr( ai_pvChangeArgs[0].Data.PointerChar, pwcName );
			if( iRet == M4_ERROR )
			{
				SETCODEF( M4_XL_ERR_GEN_CPP_2_XL, ERRORLOG, "#*s1*#%s#", getChangeName( ai_iChange ) );
				return M4_ERROR;
			}

			ai_pFont->setName( pwcName );

			delete[] pwcName;
			break;

		case M4XL_FONT_ITALIC:

			ai_pFont->setItalic( ( bool )ai_pvChangeArgs[0].Data.DoubleData );
			break;

		case M4XL_BORDER_STYLE:

			iBorderEdge = ( eXlBorderEdges_t )( int )ai_pvChangeArgs[1].Data.DoubleData;

			if( ( iBorderEdge & M4XL_EDGE_TOP ) && ( iBorderEdge & M4XL_EDGE_RIGHT ) &&
				( iBorderEdge & M4XL_EDGE_BOTTOM ) && ( iBorderEdge & M4XL_EDGE_LEFT ) )
			{
				ai_pFormat->setBorder( getXlBorderStyle( ai_pvChangeArgs[0].Data.DoubleData ) );
			}
			else
			{
				if( iBorderEdge & M4XL_EDGE_TOP )
				{
					ai_pFormat->setBorderTop( getXlBorderStyle( ai_pvChangeArgs[0].Data.DoubleData ) );
				}

				if( iBorderEdge & M4XL_EDGE_RIGHT )
				{
					ai_pFormat->setBorderRight( getXlBorderStyle( ai_pvChangeArgs[0].Data.DoubleData ) );
				}

				if( iBorderEdge & M4XL_EDGE_BOTTOM )
				{
					ai_pFormat->setBorderBottom( getXlBorderStyle( ai_pvChangeArgs[0].Data.DoubleData ) );
				}

				if( iBorderEdge & M4XL_EDGE_LEFT )
				{
					ai_pFormat->setBorderLeft( getXlBorderStyle( ai_pvChangeArgs[0].Data.DoubleData ) );
				}
			}
			break;

		case M4XL_BORDER_COLOR_INDEX:
			
			{
				ClXlRGBModeRestorer oRGBModeRestorer( ai_pBook );
				ai_pBook->setRgbMode( false );

				iBorderEdge = ( eXlBorderEdges_t )( int )ai_pvChangeArgs[1].Data.DoubleData;

				if( ( iBorderEdge & M4XL_EDGE_TOP ) && ( iBorderEdge & M4XL_EDGE_RIGHT ) &&
					( iBorderEdge & M4XL_EDGE_BOTTOM ) && ( iBorderEdge & M4XL_EDGE_LEFT ) )
				{
					ai_pFormat->setBorderColor( getXlColorIndex( ai_pvChangeArgs[0].Data.DoubleData ) );
				}
				else
				{
					if( iBorderEdge & M4XL_EDGE_TOP )
					{
						ai_pFormat->setBorderTopColor( getXlColorIndex( ai_pvChangeArgs[0].Data.DoubleData ) );
					}

					if( iBorderEdge & M4XL_EDGE_RIGHT )
					{
						ai_pFormat->setBorderRightColor( getXlColorIndex( ai_pvChangeArgs[0].Data.DoubleData ) );
					}

					if( iBorderEdge & M4XL_EDGE_BOTTOM )
					{
						ai_pFormat->setBorderBottomColor( getXlColorIndex( ai_pvChangeArgs[0].Data.DoubleData ) );
					}

					if( iBorderEdge & M4XL_EDGE_LEFT )
					{
						ai_pFormat->setBorderLeftColor( getXlColorIndex( ai_pvChangeArgs[0].Data.DoubleData ) );
					}
				}
			}
			break;

		case M4XL_NUMBERFORMAT:

			ai_pFormat->setNumFormat( ai_iNumFormat );
			break;

		case M4XL_PATTERN_FG_COLOR_INDEX:
		
			{
				ClXlRGBModeRestorer oRGBModeRestorer( ai_pBook );
				ai_pBook->setRgbMode( false );

				iFillPattern = ai_pFormat->fillPattern();

				if( iFillPattern == FILLPATTERN_NONE )
				{
					ai_pFormat->setFillPattern( FILLPATTERN_SOLID );
				}

				ai_pFormat->setPatternForegroundColor( getXlColorIndex( ai_pvChangeArgs[0].Data.DoubleData ) );
			}
			break;

		case M4XL_PATTERN_FG_COLOR_RGB:
		
			{			
				ClXlRGBModeRestorer oRGBModeRestorer( ai_pBook );
				ai_pBook->setRgbMode( true );

				iFillPattern = ai_pFormat->fillPattern();

				if( iFillPattern == FILLPATTERN_NONE )
				{
					ai_pFormat->setFillPattern( FILLPATTERN_SOLID );
				}

				iColor = ai_pBook->colorPack( ai_pvChangeArgs[0].Data.DoubleData, ai_pvChangeArgs[1].Data.DoubleData, ai_pvChangeArgs[2].Data.DoubleData );
				ai_pFormat->setPatternForegroundColor( iColor );
			}
			break;

		case M4XL_CELL_TYPE:
			
			ai_pFormat->setNumFormat( ( int )ai_pvChangeArgs[0].Data.DoubleData );
			break;
	}


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Gets the libxl color index for a given integer value. 
//
//	ai_iColor:	Color index.
// -----------------------------------------------------------------------------
Color ClXlFormatsMngr::getXlColorIndex( int ai_iColor )
{
	return( static_cast< Color >( ( int )ai_iColor + 7 ) );
}


// -----------------------------------------------------------------------------
// Gets the libxl underline value for a given integer value. 
//
//	ai_iUnderline:	Underline style number.
// -----------------------------------------------------------------------------
Underline ClXlFormatsMngr::getXlUnderlineStyle( int ai_iUnderline )
{
	return( static_cast< Underline >( ( ai_iUnderline < 4 ) ? ai_iUnderline - 1 : ( ai_iUnderline == 4 ) ? UNDERLINE_SINGLEACC : ( ai_iUnderline == 5 ) ? UNDERLINE_DOUBLEACC : ai_iUnderline ) );
}


// -----------------------------------------------------------------------------
// Gets the libxl border style value for a given integer value. 
//
//	ai_iBorderStyle:	Border style number.
// -----------------------------------------------------------------------------
enum BorderStyle ClXlFormatsMngr::getXlBorderStyle( int ai_iBorderStyle )
{
	return( static_cast< BorderStyle >( ai_iBorderStyle ) );
}


// -----------------------------------------------------------------------------
// Gets the libxl number format value for a given string pattern. 
//
//	ai_pcPattern:	Number format string pattern.
// -----------------------------------------------------------------------------
enum NumFormat ClXlFormatsMngr::getXlNumberFormat( char* ai_pcPattern )
{
	if( *ai_pcPattern == '@' )
	{
		return NUMFORMAT_TEXT;
	}

	// Default
	return NUMFORMAT_TEXT;
}


// -----------------------------------------------------------------------------
// Gets the libxl number format value for a given number format. 
//
//	ai_iNumberFormat:	Number format.
// -----------------------------------------------------------------------------
enum NumFormat ClXlFormatsMngr::getXlNumberFormat( int ai_iNumberFormat )
{
	return( static_cast< NumFormat >( ai_iNumberFormat ) );
}


// -----------------------------------------------------------------------------
// Checks if a given color index is right for libxl.
//
//	ai_iColor:	Color index.
// -----------------------------------------------------------------------------
m4return_t ClXlFormatsMngr::checkColorIndexValue( Color ai_iColor )
{
	switch( ai_iColor )
	{
		case COLOR_DEFAULT_FOREGROUND:
		case COLOR_DEFAULT_BACKGROUND:
		case COLOR_TOOLTIP:
		case COLOR_AUTO:
			return M4_SUCCESS;
	}

	if( ai_iColor < COLOR_BLACK || ai_iColor > COLOR_GRAY80 )
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Checks if a given underline value is right for libxl.
//
//	ai_iUnderline:	Underline style number.
// -----------------------------------------------------------------------------
m4return_t ClXlFormatsMngr::checkUnderlineValue( Underline ai_iUnderline )
{
	if( ai_iUnderline < 1 || ( ai_iUnderline > 2 && ai_iUnderline != UNDERLINE_SINGLEACC && ai_iUnderline != UNDERLINE_DOUBLEACC ) )
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Checks if a given border style is right for libxl.
//
//	ai_iBorderStyle:	Border style number.
// -----------------------------------------------------------------------------
m4return_t ClXlFormatsMngr::checkBorderStyleValue( BorderStyle ai_iBorderStyle )
{
	if( ai_iBorderStyle < BORDERSTYLE_NONE || ai_iBorderStyle > BORDERSTYLE_SLANTDASHDOT )
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Checks if a given border edge is right for libxl.
//
//	ai_iEdge:	Border edge.
// -----------------------------------------------------------------------------
m4return_t ClXlFormatsMngr::checkBorderEdgeValue( int ai_iEdge )
{
	if( ai_iEdge < 0 || ai_iEdge > 15 )
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Checks if a given number format string pattern is right for libxl.
//
//	ai_pcPattern:	number format string pattern
// -----------------------------------------------------------------------------
m4return_t ClXlFormatsMngr::checkNumberFormatValue( char* ai_pcPattern )
{
	if( ai_pcPattern == NULL || strlen( ai_pcPattern ) == 0 )
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Checks if a RGB component is right for libxl.
//
//	ai_iComp: RGB color component.
// -----------------------------------------------------------------------------
m4return_t ClXlFormatsMngr::checkRGBComponentValue( int ai_iComp )
{
	if( ai_iComp < 0 || ai_iComp > 255 )
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Checks if a given number format value (for cell type change) is right for libxl.
//
//	ai_iNumFormat:	Number format.
// -----------------------------------------------------------------------------
m4return_t ClXlFormatsMngr::checkCellTypeValue( NumFormat ai_iNumFormat )
{
	if( ai_iNumFormat < NUMFORMAT_GENERAL || ai_iNumFormat > NUMFORMAT_TEXT || ( ai_iNumFormat > NUMFORMAT_CUSTOM_MDYYYY_HMM && ai_iNumFormat < NUMFORMAT_NUMBER_SEP_NEGBRA ) )
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Stores a new number format.
//
//	ai_iNumFormatId:	Number format Id.
//	ai_pcNumFormatStr:	Number format string.
//	ai_pAccess:			M4Object instance.
// -----------------------------------------------------------------------------
m4return_t ClXlFormatsMngr::storeNumFormat( Book* ai_pBook, int ai_iNumFormatId, char* ai_pcNumFormatStr, ClAccess* ai_pAccess )
{
	m4return_t		iRet;


	iRet = APIChannel::AddRegister(ai_pAccess,  APIChannel::NUMFORMATS);

	if( iRet == M4_ERROR )
	{
		return M4_ERROR;
	}
	
	iRet = APIChannel::SetItem(ai_pAccess, APIChannel::NUMFORMATS, APIChannel::NUMFORMAT_ID, ai_iNumFormatId);

	if( iRet == M4_ERROR )
	{
		return M4_ERROR;
	}

	iRet = APIChannel::SetItem(ai_pAccess, APIChannel::NUMFORMATS, APIChannel::NUMFORMAT_STRING, ai_pcNumFormatStr);

	if( iRet == M4_ERROR )
	{
		return M4_ERROR;
	}

	TRACE_FORMATS_CACHE(ai_pAccess, ai_pBook, "Create", "NumFormat", 0, ai_pcNumFormatStr, (void*)ai_iNumFormatId);


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Class ClXlRGBModeRestorer
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------

ClXlRGBModeRestorer::ClXlRGBModeRestorer( Book* ai_pBook )
{
	m_pBook = ai_pBook;
	m_bRGBMode = ai_pBook->rgbMode();
}


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------

ClXlRGBModeRestorer::~ClXlRGBModeRestorer()
{
	m_pBook->setRgbMode( m_bRGBMode );
}


// -----------------------------------------------------------------------------
// Traces a operation in the formats cache.
//
//	ai_pAccess:		Channel access.
//	ai_pBook:		Book object.
//	ai_pcOperation:	Create/Reuse.
//	ai_iStore:		Object references container.
//	ai_pSrcObj:		Source object.
//	ai_pcKey:		Change key.
//	ai_pObj:		Resulting object.
// -----------------------------------------------------------------------------
m4return_t ClXlFormatsMngr::traceFormatsCache(ClAccess* ai_pAccess, Book* ai_pBook, char* ai_pcOperation, char* ai_pcStore, void* ai_pSrcObj, char* ai_pcKey, void* ai_pObj)
{
	int				iRet;
	string			sTraceFile;
	char			acBook[17];
	struct stat		stBuf;
	FILE*			pfLog = NULL;
	M4ClTimeStamp	oTime;
	M4ClString		sTime;
	char			*pcKey, *pcVal;


	// This check should not be necessary.
	if (s_bTraceEnabled == false)
	{
		return M4_ERROR;
	}

	iRet = GetTraceDir(ai_pAccess, sTraceFile);

	if (iRet == M4_ERROR)
	{
		return M4_ERROR;
	}

	sprintf(acBook, "%p", ai_pBook);

	sTraceFile += GetDirSeparator();
	sTraceFile += "m4xtrace_formats_";
	sTraceFile += acBook;
	sTraceFile += ".log";

	iRet = stat(sTraceFile.c_str(), &stBuf);

	pfLog = fopen(sTraceFile.c_str(), "at");

	if (!pfLog)
	{
		return M4_ERROR;
	}

	if (iRet != 0)
	{
		fprintf(pfLog, "Time\tChannel\tBook\tResult\tStore\tSrcObject\tChange\tValue\tTgtObject\n");
	}

	oTime.now();
	sTime = oTime;

	if (strcmp(ai_pcStore, "NumFormat") != 0)
	{
		pcKey = new char[strlen(ai_pcKey) + 1];
		strcpy(pcKey, ai_pcKey);
		pcVal = strstr(pcKey, ":");

		if (pcVal != NULL)
		{
			*pcVal++ = NULL;
		}
		else
		{
			pcVal = "";
		}

		fprintf(pfLog, "%s\t0x%p\t0x%p\t%s\t%s\t0x%p\t%s\t%s\t0x%p\n", (char*)sTime, ai_pAccess, ai_pBook, ai_pcOperation, ai_pcStore, ai_pSrcObj, getChangeName(static_cast<eXlChange>(atoi(pcKey))), pcVal, ai_pObj);

		delete pcKey;
	}
	else
	{
		fprintf(pfLog, "%s\t0x%p\t0x%p\t%s\t%s\t-\t%s\t%s\t%d\n", (char*)sTime, ai_pAccess, ai_pBook, ai_pcOperation, ai_pcStore, "GetNumFormat", ai_pcKey, (int)ai_pObj);
	}

	fclose(pfLog);


	return M4_SUCCESS;
}
