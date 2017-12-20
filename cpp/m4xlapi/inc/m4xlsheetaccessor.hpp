//==============================================================================
//
// (c) Copyright  1991-2013 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4xlapi
// File:             m4csvsheetaccessor.hpp
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

#ifndef m4xlsheetaccessor_hpp
#define m4xlsheetaccessor_hpp 1

#include <m4types.hpp>
#include <m4var.hpp>
#include <m4stl.hpp>
#include <execontx.hpp>
#include <libxl.h>

using namespace libxl;

typedef enum { GEN_MODE, QUERY_MODE } eTypeExtrapolationMode;

// -----------------------------------------------------------------------------
// Class ISheetAccessor
//
// Interface definition for reading from and writing to an Excel sheet
// -----------------------------------------------------------------------------

class ISheetAccessor
{
	public:
		
		virtual ~ISheetAccessor(){}

		// Writes a value into a cell, given it with Meta4 format and optionally with ISO format.
		virtual m4return_t WriteValue	( int ai_iRow, int ai_iCol, wchar_t* ai_pwcM4Value, wchar_t* ai_pwcISOValue ) = 0;

		// Writes a value stored in a Meta4 variant object, into a cell.
		virtual m4return_t WriteValue	( int ai_iRow, int ai_iCol, m4VariantType* ai_pvValue ) = 0;

		// Reads a cell, storing the the value in a Meta4 variant object.
		virtual m4return_t ReadValue	( int ai_iRow, int ai_iCol, m4VariantType* ai_pvValue ) = 0;

		// Updates the accessor context.
		virtual m4return_t ChangeSheet	( int ai_iRow, int ai_iCol, int ai_iSheetIndex, wchar_t* ai_pwcSheetName ) = 0;

		// Sets the types extrapolation mode.
		virtual void	   SetTypeExtrapolationMode( eTypeExtrapolationMode ai_iMode ) = 0;
};


// Enumeration with all cell-types behavior.
typedef enum { TYPE_NUMBER, TYPE_DATE, TYPE_TIME, TYPE_PERCENTAGE, TYPE_TEXT, TYPE_GENERAL, TYPE_DATE_TIME, TYPE_FORMULA } eTargetType;

// Enumeration with all forced target types (See ImportCSVEx/PasteSpecialTextEx).
typedef enum { FCD_NO_TYPE, FCD_NUMBER_TYPE, FCD_STRING_TYPE, FCD_DATE_TYPE, FCD_DATE_TIME_TYPE, FCD_TIME_TYPE } eForcedTargetType;


// -----------------------------------------------------------------------------
// Class ClXlSheetAccessor
//
// Class for reading and writing in Excel through libxl library.
// -----------------------------------------------------------------------------

class ClXlSheetAccessor: public ISheetAccessor
{
	public:

		ClXlSheetAccessor( int ai_iInitRow, int ai_iInitCol, Book* ai_pBook, Sheet* ai_pSheet, ClVMRunContext* ai_pContext, const char* ai_pcForcedColTypes = NULL );

		// Interface implementation.

		virtual ~ClXlSheetAccessor();

		// Writes a value into a cell, given it with Meta4 format and optionally with ISO format.
		virtual m4return_t WriteValue	( int ai_iRow, int ai_iCol, wchar_t* ai_pwcM4Value, wchar_t* ai_pwcISOValue );

		// Writes a value stored in a Meta4 variant object, into a cell.
		virtual m4return_t WriteValue	( int ai_iRow, int ai_iCol, m4VariantType* ai_pvValue );

		// Reads a cell, storing the the value in a Meta4 variant object.
		virtual m4return_t ReadValue	( int ai_iRow, int ai_iCol, m4VariantType* ai_pvRefArg );

		// Updates the accessor context.
		virtual m4return_t ChangeSheet	( int ai_iRow, int ai_iCol, int ai_iSheetIndex, wchar_t* ai_pwcSheetName );

		// Sets the types extrapolation mode.
		virtual void	   SetTypeExtrapolationMode( eTypeExtrapolationMode ai_iMode );

	private:
		
		// Internal members.
		int						m_iInitRow;
		int						m_iInitCol;
		Book*					m_pBook;
		Sheet*					m_pSheet;
		vector< int >*			m_pvForcedColTypes;
		eTypeExtrapolationMode	m_iExtrapolationMode;
		ClVMRunContext*			m_pContext;

		// Internal methods.
		m4return_t	initialize				( const char* ai_pcForcedColTypes );

		m4return_t	writeCellValue			( int ai_iRow, int ai_iCol, wchar_t* ai_pwcValue, Format* ai_pFormat );
		m4return_t	writeCellValue			( int ai_iRow, int ai_iCol, double ai_dValue, Format* ai_pFormat );

		m4return_t	writeCellFormula		( int ai_iRow, int ai_iCol, wchar_t* ai_pwcFormula, Format* ai_pFormat );

		m4return_t	readCellValue			( int ai_iRow, int ai_iCol, char*& ao_pcValue );
		m4return_t	readCellValue			( int ai_iRow, int ai_iCol, double& ao_dValue );
		m4return_t	readCellValue			( int ai_iRow, int ai_iCol, bool& ao_bValue );
		m4return_t	readCellErrValue		( int ai_iRow, int ai_iCol, char*& ao_pcValue );

		eTargetType	getTargetType			( int ai_iNumFormat );
		eTargetType	extrapolateTargetType	( eTargetType ai_eTargetType, wchar_t* ai_pwcM4Value, wchar_t* ai_pwcISOValue );
		eTargetType	extrapolateTargetType	( eTargetType ai_eTargetType, m4VariantType* ai_pvValue );
		void		removeNoisyTags			( wchar_t* ai_pwcFormat );
		
		m4return_t	getXlPackedDate			( wchar_t* ai_pwcDate, double& ao_dPackedDate );
		m4return_t	getXlPackedTime			( wchar_t* ai_pwcTime, double& ao_dPackedTime );

		eTargetType	getForcedTargetType		( int ai_iCol, wchar_t* ai_pwcValue );

		m4return_t	getNewSheetAutoName		( wchar_t* ao_pwcSheetName );
		m4return_t	getCurrSheetIndex		( int& ao_iIndex );

		m4return_t	GetTargetFormat			( Format* ai_pTpltFormat, eTargetType ai_iTargetType, Book* ai_pBook, wchar_t* ai_pwcData, Format*& ao_pOutFormat );

		// Internal constants
		static const wchar_t* apwcDateTokens[];
		static const wchar_t* apwcTimeTokens[];
		static const wchar_t* apwcNoisyTags	[];
};


#endif
