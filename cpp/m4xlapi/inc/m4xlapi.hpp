//==============================================================================
//
// (c) Copyright  1991-1999 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4xlapi
// File:             m4xlapi.hpp
// Project:			 mind3.x
// Author:           Meta Software M.S. , S.A
// Date:			 22/10/2013
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#ifndef _M4XLAPI_H_
#define _M4XLAPI_H_

#include "m4xlapi_dll.hpp"
#include <m4types.hpp>
#include <m4var.hpp>

// Query tool

// Opens an existing Excel workbook.
extern "C" M4_DECL_M4XLAPI  m4return_t OpenWorkbook				( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Closes the current Excel workbook.
extern "C" M4_DECL_M4XLAPI  m4return_t CloseWorkbook			( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Saves the current Excel workbook.
extern "C" M4_DECL_M4XLAPI  m4return_t SaveWorkbook				( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Retrieves a worksheet within the current Excel workbook.
extern "C" M4_DECL_M4XLAPI  m4return_t GetWorksheet				( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Converts a cell reference to row and column.
extern "C" M4_DECL_M4XLAPI  m4return_t GetRowCol				( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Imports a CSV file into the current worksheet. 
extern "C" M4_DECL_M4XLAPI  m4return_t ImportCSV				( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );

// Import tool

// Reads a cell value.
extern "C" M4_DECL_M4XLAPI  m4return_t GetCellValue				( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Writes a cell value.
extern "C" M4_DECL_M4XLAPI  m4return_t SetCellValue				( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Gets the last cell written in the current worksheet.
extern "C" M4_DECL_M4XLAPI  m4return_t GetLastCell				( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Writes the content of an Excel range into a M4Object node.
extern "C" M4_DECL_M4XLAPI  m4return_t DumpRangeToNode			( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Writes the values stored in a CSV string into the current worksheet.
extern "C" M4_DECL_M4XLAPI  m4return_t PasteSpecialText			( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );

// Generic API

// Creates a new Excel workbook file. Java implementation compatibility.
extern "C" M4_DECL_M4XLAPI  m4return_t NewWorkbookFile			( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Adds a new Excel worksheet.
extern "C" M4_DECL_M4XLAPI  m4return_t AddWorksheet				( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Copies a cell into another.
extern "C" M4_DECL_M4XLAPI  m4return_t CopyCell					( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Copies a row into another.
extern "C" M4_DECL_M4XLAPI  m4return_t CopyRow					( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Hides / shows a column.
extern "C" M4_DECL_M4XLAPI  m4return_t SetColHidden				( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Hides / shows a row.
extern "C" M4_DECL_M4XLAPI  m4return_t SetRowHidden				( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Imports a CSV file into the current worksheet, receiving a column types list. 
extern "C" M4_DECL_M4XLAPI  m4return_t ImportCSVEx				( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );

// Changes the bold attribute of a single cell, a user defined range, a full row range or a full column range.
extern "C" M4_DECL_M4XLAPI  m4return_t FontBold					( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Changes the font color attribute of a single cell, a user defined range, a full row range or a full column range.
extern "C" M4_DECL_M4XLAPI  m4return_t FontColorIndex			( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Changes the font size attribute of a single cell, a user defined range, a full row range or a full column range.
extern "C" M4_DECL_M4XLAPI  m4return_t FontSize					( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Changes the font underline attribute of a single cell, a user defined range, a full row range or a full column range.
extern "C" M4_DECL_M4XLAPI  m4return_t FontUnderline			( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Changes the font name attribute of a single cell, a user defined range, a full row range or a full column range.
extern "C" M4_DECL_M4XLAPI  m4return_t FontName					( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Changes the font italic attribute of a single cell, a user defined range, a full row range or a full column range.
extern "C" M4_DECL_M4XLAPI  m4return_t FontItalic				( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Changes the border style of a single cell, a user defined range, a full row range or a full column range.
extern "C" M4_DECL_M4XLAPI  m4return_t BorderStyle				( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Changes the border color attribute of a single cell, a user defined range, a full row range or a full column range.
extern "C" M4_DECL_M4XLAPI  m4return_t BorderColorIndex			( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );

// Inserts into the sheet the current range or a previously copied one.
extern "C" M4_DECL_M4XLAPI  m4return_t InsertRange				( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Deletes the current range.
extern "C" M4_DECL_M4XLAPI  m4return_t DeleteRange				( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Sets the numberformat of a single cell, a user defined range, a full row range or a full column range.
extern "C" M4_DECL_M4XLAPI  m4return_t SetNumberformat			( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Gets the numberformat of a single cell, a user defined range, a full row range or a full column range.
extern "C" M4_DECL_M4XLAPI  m4return_t GetNumberformat			( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Sets the name of the current sheet.
extern "C" M4_DECL_M4XLAPI  m4return_t SetSheetName				( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Sets the width of a column.
extern "C" M4_DECL_M4XLAPI  m4return_t SetColWidth				( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Protects/Unprotects the current worksheet.
extern "C" M4_DECL_M4XLAPI  m4return_t SetProtect				( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Clears the contents of a range.
extern "C" M4_DECL_M4XLAPI  m4return_t ClearContents			( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Creates a new Excel worksheet.
extern "C" M4_DECL_M4XLAPI  m4return_t NewWorksheet				( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Creates a new Excel workbook.
extern "C" M4_DECL_M4XLAPI  m4return_t CreateWorkbook			( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Gets the width of a column.
extern "C" M4_DECL_M4XLAPI  m4return_t GetColWidth				( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Sets the height of a row.
extern "C" M4_DECL_M4XLAPI  m4return_t SetRowHeight				( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Gets the height of a row.
extern "C" M4_DECL_M4XLAPI  m4return_t GetRowHeight				( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Gets the name of the current sheet.
extern "C" M4_DECL_M4XLAPI  m4return_t GetSheetName				( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Writes the values stored in a CSV string into the current worksheet, receiving a column types list. 
extern "C" M4_DECL_M4XLAPI  m4return_t PasteSpecialTextEx		( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Writes a cell comment.
extern "C" M4_DECL_M4XLAPI  m4return_t SetCellComment			( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Merges a set of cells.
extern "C" M4_DECL_M4XLAPI  m4return_t SetMerge					( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Sets the pattern foreground color by index.
extern "C" M4_DECL_M4XLAPI  m4return_t SetPatternFgColorIndex	( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Sets the pattern foreground color by its RGB components.
extern "C" M4_DECL_M4XLAPI  m4return_t SetPatternFgColorRGB		( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Get sheet count of current workbook.
extern "C" M4_DECL_M4XLAPI  m4return_t GetSheetCount			( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Get platform directory separator.
extern "C" M4_DECL_M4XLAPI  m4return_t GetDirSeparator			( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );
// Opens an existing Excel workbook partially.
extern "C" M4_DECL_M4XLAPI  m4return_t OpenWorkbookPartially( m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext );

#endif
