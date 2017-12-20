//==============================================================================
//
// (c) Copyright  1991-2013 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4xlapi
// File:             m4csvparser.hpp
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

#ifndef m4csvparser_hpp
#define m4csvparser_hpp 1

#include <m4csvvalue.hpp>
#include <m4xlsheetaccessor.hpp>


// -----------------------------------------------------------------------------
// Class ClM4CsvParser
//
// Parser for M4 CSV file.
// -----------------------------------------------------------------------------

class ClM4CsvParser
{
	public:

		ClM4CsvParser	( ISheetAccessor* ai_pSheetAccessor );
		~ClM4CsvParser	();
		
		// Sets the CSV file path to parse and the values delimiter character.
		m4return_t	SetCsvFile	( const char* ai_pcCSVFile, char ai_cValTerminator );

		// Sets the CSV string to parse and the values delimiter character.
		m4return_t	SetCsvString( const char* ai_pcCSVData, char ai_cValTerminator );

		// Parses the whole CSV file or string.
		m4return_t	Parse		();

	private:

		// Internal members.
		FILE*					m_pfCSVFile;
		const char*				m_pcCSVData;
		char					m_cValueTerminator;
		m4bool_t				m_bHasMoreRows;
		ClCsvValue*				m_pCsvValue;
		ISheetAccessor*			m_pSheetAccessor;
		int						m_iReadIndex;
		eXLEncodings_t			m_iEncoding;
		int						m_iCurrRow;
		int						m_iCurrCol;
		m4bool_t				m_bFailSafeMode;
		
		// Internal methods.
		m4return_t	parseSingleRow		();
		m4return_t	processReadedValue	( m4bool_t ai_bLastCol, m4bool_t& aio_bSheetReset );
		m4bool_t	scanChangeSheetParams( wchar_t* ai_pwcValue, int& ao_iRow, int& ao_iCol, int& ao_iSheetIndex, wchar_t*& ao_pwcSheetName );
		m4return_t	failSafeAppend		( ClCsvCharBase	*pCsvChar );
		m4bool_t	isRowTerminatorChar	( ClCsvCharBase	*ai_pCsvChar );

		// Internal constants
		static const char		s_EnclosingChar;
		static const char		s_RowTerminatorChar_n;
		static const char		s_RowTerminatorChar_r;
		static const int		s_MaxValueLength;
		static const char		s_MultiValueSeparatorChar;
};

#endif
