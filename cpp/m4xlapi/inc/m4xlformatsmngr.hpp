//==============================================================================
//
// (c) Copyright  1991-2013 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4xlapi
// File:             m4xlformatsmngr.hpp
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

#ifndef m4xlformatsmngr_hpp
#define m4xlformatsmngr_hpp 1

#include <execontx.hpp>
#include <m4var.hpp>
#include <m4string.hpp> 
#include <libxl.h>

using namespace libxl;


// -----------------------------------------------------------------------------
// Class ClXlFormatsMngr
//
// Class for managing libxl format objects changes.
// 
// Note: It groups a set of methods related with libxl-format objects, optimizing 
// the number of instances for those objects. 
// This class can not be instantiated, the M4Object instance has the libxl object
// references. These references are valid while the M4Object is alive.
// -----------------------------------------------------------------------------

class ClXlFormatsMngr
{
	public:

		// Public enumerations.

		// Format changes supported.
		typedef	enum eXlChange
		{
			M4XL_FONT_BOLD,
			M4XL_FONT_COLOR_INDEX,
			M4XL_FONT_SIZE,
			M4XL_FONT_UNDERLINE,
			M4XL_FONT_NAME,
			M4XL_FONT_ITALIC,
			M4XL_BORDER_STYLE,
			M4XL_BORDER_COLOR_INDEX,
			M4XL_NUMBERFORMAT,
			M4XL_PATTERN_FG_COLOR_INDEX,
			M4XL_PATTERN_FG_COLOR_RGB,
			M4XL_CELL_TYPE,
			M4XL_TOP_ITEM
		} eXlChange_t;

		// Border edges.
		typedef	enum eXlBorderEdges
		{
			M4XL_EDGE_TOP = 1,
			M4XL_EDGE_RIGHT = 2,
			M4XL_EDGE_BOTTOM = 4,
			M4XL_EDGE_LEFT = 8
		} eXlBorderEdges_t;

		
		// Public methods.

		// Accepts requests for changing a cell attribute for a single cell, a user defined range, 
		// a full row range or a full column range.
		static m4return_t		ChangeCellsAttribute( m4VariantType *ai_pvArg, m4pvoid_t ai_pvContext, eXlChange_t ai_iChange );

		// Gets the format resulting of changing another one.
		static m4return_t		GetSuitableFormat( Format* ai_pFormat, eXlChange_t ai_iChange, m4VariantType* ai_pvChangeArgs, Book* ai_pBook, ClAccess* ai_pAccess, Format*& ao_pNewFormat );

	private:

		// Internal members.
		static const char*		s_apcChangeNames[];

		static const int		s_iMaxRowsInNarrowRange;
		static const int		s_iMaxColsInNarrowRange;

		// Internal methods.

		// Changes an attribute of a single cell.
		static m4return_t		changeSingleCellAttr( int ai_iRow, int ai_iCol, eXlChange_t ai_iChange, m4VariantType* ai_pvChangeArgs, m4pvoid_t ai_pvContext );

		// Changes an attribute of the cells in a user defined range.
		static m4return_t		changeUserRangeAttr( char* ai_pcStartCell, char* ai_pcEndCell, eXlChange_t ai_iChange, m4VariantType* ai_pvChangeArgs, m4pvoid_t ai_pvContext );

		// Changes an attribute of a one-dimension range (row or column).
		static m4return_t		changeNarrowRangeAttr( int ai_iRow, int ai_iCol, eXlChange_t ai_iChange, m4VariantType* ai_pvChangeArgs, m4pvoid_t ai_pvContext );

		// Apply the changes in the cells of a user range.
		static m4return_t		applyChangeUserRangeAttr(int ai_iStartRow, int ai_iEndRow, int ai_iStartCol, int ai_iEndCol, eXlChange_t ai_iChange, m4VariantType* ai_pvChangeArgs, m4pvoid_t ai_pvContext );

		// Gets the format-change operation name for a given code.
		static const char*		getChangeName( eXlChange_t ai_iChange );

		// Checks if a value is right for the specified change.
		static m4return_t		checkChangeValue( eXlChange_t ai_iChange, m4VariantType* ai_pvChangeArgs );

		// Checks if the requested change is already done in the source format.
		static m4return_t		canIgnoreChange( Format* ai_pFormat, eXlChange_t ai_iChange, m4VariantType* ai_pvChangeArgs, Book* ai_pBook, ClAccess* ai_pAccess, m4bool_t& ao_bCanIgnore );

		// Builds the key that identifies a change operation.
		static m4return_t		buildChangeKey( eXlChange_t ai_iChange, m4VariantType* ai_pvChangeArgs, M4ClString& ao_strKey );

		// Returns if this change affects the font object.
		static m4bool_t			isFontChange( eXlChange_t ai_iChange );

		// Performs the change.
		static m4return_t		applyChange( Format* ai_pFormat, Font* ai_pFont, int ai_iNumFormat, eXlChange_t ai_iChange, m4VariantType* ai_pvChangeArgs, Book* ai_pBook );
		
		// Gets the libxl color index for a given integer value. 
		static Color			getXlColorIndex( int ai_iColor );

		// Gets the libxl underline value for a given integer value. 
		static Underline		getXlUnderlineStyle( int ai_iUnderline );

		// Gets the libxl border style value for a given integer value. 
		static enum BorderStyle	getXlBorderStyle( int ai_iBorderStyle );

		// Gets the libxl number format value for a given string pattern. 
		static enum NumFormat	getXlNumberFormat( char* ai_pcPattern );

		// Gets the libxl number format value for a given number format. 
		static enum NumFormat	getXlNumberFormat( int ai_iNumberFormat );

		// Checks if a given color index is right for libxl.
		static m4return_t		checkColorIndexValue( Color ai_iColor );

		// Checks if a given underline value is right for libxl.
		static m4return_t		checkUnderlineValue( Underline ai_iUnderline );

		// Checks if a given border style is right for libxl.
		static m4return_t		checkBorderStyleValue( enum BorderStyle ai_iBorderStyle );

		// Checks if a given border edge is right for libxl.
		static m4return_t		checkBorderEdgeValue( int ai_iEdge );

		// Checks if a given number format string pattern is right for libxl.
		static m4return_t		checkNumberFormatValue( char* ai_pcPattern );

		// Checks if a RGB component is right for libxl.
		static m4return_t		checkRGBComponentValue( int ai_iComp );

		// Checks if a given number format value (for cell type change) is right for libxl.
		static m4return_t		checkCellTypeValue( NumFormat ai_iNumFormat );

		// Stores a new number format.
		static m4return_t		storeNumFormat( Book* ai_pBook, int ai_iNumFormatId, char* ai_pcNumFormatStr, ClAccess* ai_pAccess );

		// Traces a operation in the formats cache.
		static m4return_t		traceFormatsCache( ClAccess* ai_pAccess, Book* ai_pBook, char* ai_pcOperation, char* ai_pcStore, void* ai_pSrcObj, char* ai_pcKey, void* ai_pObj );
};


// -----------------------------------------------------------------------------
// Class ClXlRGBModeRestorer
//
//	Sets RGB mode to previous value on destruction.
// -----------------------------------------------------------------------------
class ClXlRGBModeRestorer
{
	public:
		
		ClXlRGBModeRestorer( Book* ai_pBook );
		~ClXlRGBModeRestorer();

	private:

		Book*	m_pBook;
		bool	m_bRGBMode;
};

#endif
