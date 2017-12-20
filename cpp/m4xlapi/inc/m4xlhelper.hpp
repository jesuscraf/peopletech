//==============================================================================
//
// (c) Copyright  1991-2013 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4xlapi
// File:             m4xlhelper.hpp
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
//==============================================================================

#ifndef m4xlhelper_hpp
#define m4xlhelper_hpp 1

#include "m4stl.hpp"
#include <execontx.hpp>
#include <m4xlapierrors.hpp>
#include <m4var.hpp>
#include <libxl.h>

using namespace libxl;


// -----------------------------------------------------------------------------
// Helper macros.
// -----------------------------------------------------------------------------
#define CH_RETURN(X, Y) { int _retVal = Y; m4VariantType* _pvReturn = X; _pvReturn->Type = M4CL_CPP_TYPE_NUMBER; _pvReturn->Data.DoubleData = _retVal; return(M4_SUCCESS); }
#define CHECK_ARG_NUM(X, Y, Z) { m4uint32_t _iNum = X; m4uint32_t _iReqNum = Y; const char* _sFunc = Z; if( _iNum != _iReqNum){ SETCODEF( M4_XL_ERR_BAD_ARGUMENTS, ERRORLOG, "#*s1*#%s#", _sFunc ); CH_RETURN( &ai_pvReturn, M4_ERROR ) } }
#define CHECK_ARG_TYPE(X, Y, Z) { m4char_t _iType = X; m4char_t _iReqType = Y; const char* _sFunc = Z; if( _iType != _iReqType){ SETCODEF( M4_XL_ERR_BAD_ARGUMENTS, ERRORLOG, "#*s1*#%s#", _sFunc ); CH_RETURN( &ai_pvReturn, M4_ERROR ) } }


// -----------------------------------------------------------------------------
// M4Object API item names.
// -----------------------------------------------------------------------------
#define ITEM_WBOOK_POINTER	"EXCEL_WORKBOOK"
#define ITEM_WBOOK_PATH		"WORKBOOK_PATH"
#define ITEM_WSHEET_POINTER	"EXCEL_WORKSHEET"
#define ITEM_WSHEET_INDEX	"EXCEL_WORKSHEET_INDEX"
#define ITEM_WSHEET_NAME	"EXCEL_WORKSHEET_NAME"


// -----------------------------------------------------------------------------
// Enumeration for the values supported encodings.
// -----------------------------------------------------------------------------

typedef	enum eXLEncodings
{
	M4XL_M4CPP,
	M4XL_ANSI,
	M4XL_UTF8,
	M4XL_UTF16LE,
	M4XL_UTF16BE
} eXLEncodings_t;


// -----------------------------------------------------------------------------
// Helper functions.
// -----------------------------------------------------------------------------

// Returns whether an Excel file belongs to an XML format.
m4return_t	IsXMLWorkbook			( const char* ai_pcFile, m4bool_t& ao_bIsXML );

// Returns whether an Excel file is a template.
m4return_t	IsTemplateWorkbook		( const char* ai_pcFile, m4bool_t& ao_bIsTemplate );

// Trims a string left and right.
m4return_t	TrimValue				( char*& ao_pcValue );

// Returns an upper case copy of a string.
wchar_t*	DuplicateInUpperCase	( const wchar_t *s1 );

// Composes a wide char string, given a printf-like format and parameters.
m4return_t	ComposeString			( wchar_t* ao_pwcString, size_t ai_ilen, wchar_t* ai_pwcFormat, ... );

// Splits all the time components into single elements.
m4return_t	GetTimeNumbers			( wchar_t* ai_pwcTime, int& ao_iHH, int& ao_imm, int& ao_iSS );

// Splits all the date components into single elements.
m4return_t	GetDateNumbers			( wchar_t* ai_pwcDate, int& ao_iYY, int& ao_iMM, int& ao_iDD, int& ao_iHH, int& ao_imm, int& ao_iSS );

// Transforms Meta4 cpp strings to libxl encoded strings.
m4return_t	CppStrToLibxlStr		( const char* ai_pcInString, wchar_t*& ao_pwcOutString );

// Transforms libxl encoded strings to Meta4 cpp strings.
m4return_t	LibxlStrToCppStr		( const wchar_t* ai_pwcInString, char*& ao_pcOutString );

// Transforms UTF16 strings to platform specific wide char strings.
m4return_t	UTF16StrToNativeWCStr	( const unsigned short* ai_pcInString, int ai_iLength, wchar_t*& ao_pwcOutString, m4bool_t& ao_bIsNew );

// Transforms platform specific wide char strings to UTF16 strings.
m4return_t	NativeWCStrToUTF16Str	( const wchar_t* ai_pwcInString, unsigned short*& ao_pcOutString, m4bool_t& ao_bIsNew );

// Swaps the bytes in a UTF16 characters string.
m4return_t	SwapBytesUTF16String	( unsigned short* ai_pcString, int ai_iLength );

// Converts a string encoded in any encoding to a native wide char string.
m4return_t	StrToNativeWCStr		( const char* ai_pcInString, int ai_iLength, eXLEncodings_t ai_iEncoding, wchar_t*& ao_pwcOutString );

// Removes a character by position in a wide char string.
m4return_t	RemoveCharAt			( wchar_t* ai_pwcString, int ai_iPos );

// Copies the contents of a range, with its format, in another area to the bottom.
m4return_t CopyRangeDown( Book*	ai_pWorkbook, Sheet* ai_pWorksheet, int ai_iSrcStartRow, int ai_iSrcStartCol, int ai_iSrcEndRow, int ai_iSrcEndCol, int ai_iTgtStartRow, int ai_iTgtStartCol );

// Copies the contents of a range, with its format, in another area to the right.
m4return_t CopyRangeRight( Book* ai_pWorkbook, Sheet* ai_pWorksheet, int ai_iSrcStartRow, int ai_iSrcStartCol, int ai_iSrcEndRow, int ai_iSrcEndCol, int ai_iTgtStartRow, int ai_iTgtStartCol );

// Sets format to the bottom in the cells of a range.
m4return_t FillFormatDown( Book* ai_pWorkbook, Sheet* ai_pWorksheet, int ai_iTgtStartRow, int ai_iTgtStartCol, int ai_iTgtEndRow, int ai_iTgtEndCol, bool ai_bPrevFormat, bool ai_bClearContent, bool ai_bRemoveBorder, ClAccess* ai_pAccess = 0 );

// Sets format to the right in the cells of a range.
m4return_t FillFormatRight( Book* ai_pWorkbook, Sheet* ai_pWorksheet, int ai_iTgtStartRow, int ai_iTgtStartCol, int ai_iTgtEndRow, int ai_iTgtEndCol, bool ai_bPrevFormat, bool ai_bClearContent, bool ai_bRemoveBorder, ClAccess* ai_pAccess = 0 );

// Removes invalid chars in a sheet name.
m4return_t SanitizeSheetName( wchar_t* ai_pwcName );

// Returns if it is the date for m4 time variant.
bool	IsDateForM4Time( int ai_iYears, int ai_iMonths, int ai_iDays );

// Sets the date for m4 time variant.
void	SetDateForM4Time( int& ao_iYears, int& ao_iMonths, int& ao_iDays );

// Clones the format of a block to another block moving vertically.
m4return_t CopyFormatDown( Book* ai_pWorkbook, Sheet* ai_pWorksheet, int ai_iSrcStartRow, int ai_iSrcStartCol, int ai_iSrcEndRow, int ai_iSrcEndCol, int ai_iTgtStartRow, int ai_iTgtStartCol );

// Clones the format of a block to another block moving horizontally.
m4return_t CopyFormatRight( Book* ai_pWorkbook, Sheet* ai_pWorksheet, int ai_iSrcStartRow, int ai_iSrcStartCol, int ai_iSrcEndRow, int ai_iSrcEndCol, int ai_iTgtStartRow, int ai_iTgtStartCol );

// Gets directory separator.
char* GetDirSeparator();

// Gets traces directory.
m4return_t GetTraceDir(ClAccess* ai_pAccess, string& ao_sPath);

// Checks if trace is enabled.
m4return_t CheckTraceMode(ClAccess* ai_pAccess);

// Copies a file.
m4return_t CopySingleFile(const char* ai_pcSource, const char* ai_pcTarget);


// -----------------------------------------------------------------------------
// Helper classes.
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Class ClXlSheetAccessor
//
// Class for deleting Meta4Object access references.
// -----------------------------------------------------------------------------
class ClObjTerminator
{
	public:

		ClObjTerminator	( ClAccess*	ai_poAccess );
		~ClObjTerminator();

	private:

		ClAccess*	m_poAccess;
};


// -----------------------------------------------------------------------------
// Class ClExcelColNames
//
// Class for storing all required Excel column names.
// -----------------------------------------------------------------------------

class ClExcelColNames
{
	public:

		ClExcelColNames	( int ai_iNumCols );
		~ClExcelColNames();

		// Returns the name of the column for a given index.
		char*		GetColName	( int ai_iNumCol );

	private:

		int			n_iNumCols;
		char*		m_pcXlsColNames;
		const int	c_iXlsColumnWidth;
		m4return_t	initialize();
};


// -----------------------------------------------------------------------------
// Class ClCellPos
//
// Class for storing cell coordinates.
// -----------------------------------------------------------------------------

class ClCellPos
{
	public:

		ClCellPos();
		ClCellPos( int ai_iRow, int ai_iCol );
		~ClCellPos();

		void Init( int ai_iRow, int ai_iCol );

		bool operator < ( const ClCellPos& c2 ) const;

	private:

		int	m_iRow;
		int	m_iCol;
};


// -----------------------------------------------------------------------------
// Set container for ClCells objects.
// -----------------------------------------------------------------------------
typedef set< ClCellPos, less< ClCellPos > > CellPosSet_t;
typedef CellPosSet_t::iterator CellPosSetIt_t;


// -----------------------------------------------------------------------------
// Class ClAPICallTracer
//
// Class for tracing API calls.
// -----------------------------------------------------------------------------
class ClAPICallTracer
{
	public:

		// Traces an API call.
		static m4return_t TraceAPICall(ClVMRunContext* ai_pContext, m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg);

	private:

		static int s_iFileCounter;

		// Gets the HL method invoker.
		static m4return_t getAPIMethod(ClVMRunContext* ai_pContext, m4VariantType *ai_pvArg, m4uint32_t& aio_iLongArg, m4pcchar_t& ao_pcChannel, m4pcchar_t& ao_pcNode, m4pcchar_t& ao_pcMethod);

		// Manages a file argument.
		static m4return_t processFileArg(ClVMRunContext* ai_pContext, m4pcchar_t ai_pcArgFile, string& ao_sArgFile);

		// Substrings replacer.
		static void replaceStr(string& ai_sSource, const string& ai_sWhat, const string& ai_sWith);
};

#endif
