//==============================================================================
//
// (c) Copyright  1991-2017 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4xlapi
// File:             m4xlapichannel.hpp
// Project:			 mind3.x
// Author:           Meta Software M.S. , S.A
// Date:             11/12/2017
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

#ifndef m4xlapichannel_hpp
#define m4xlapichannel_hpp 1

#include <access.hpp>
#include <m4var.hpp>


// -----------------------------------------------------------------------------
// Class ClXLAPIChannel
//
// XL API channel static wrapper.
// -----------------------------------------------------------------------------
class ClXLAPIChannel
{
public:

	// Channel nodes enumeration.
	typedef	enum eXlNodes
	{
		WORKBOOKS,
		WORKSHEETS,
		FORMATS,
		FONTS,
		NUMFORMATS
	} Nodes;

	// Channel items enumeration.
	typedef	enum eXlItems
	{
		WORKBOOK_POINTER, WORKBOOK_PATH, TRACE_BASE_PATH, TRACE_KEY, \
		WORKSHEET_POINTER, WORKSHEET_INDEX, WORKSHEET_NAME, \
		FORMAT_SRC_POINTER, FORMAT_KEY, FORMAT_TGT_POINTER, \
		FONT_SRC_POINTER, FONT_KEY, FONT_TGT_POINTER, \
		NUMFORMAT_ID, NUMFORMAT_STRING
	} Items;


	// Adds a new register in the node.
	static m4return_t	AddRegister(ClAccess* ai_pAccess, Nodes ai_iNode);

	// Sets a pointer in the current Meta4Object.
	static m4return_t	SetItem(ClAccess* ai_pAccess, Nodes ai_iNode, Items ai_iItem, void* ai_pObject);

	// Sets a string in the current Meta4Object.
	static m4return_t	SetItem(ClAccess* ai_pAccess, Nodes ai_iNode, Items ai_iItem, m4pchar_t ai_pcString);

	// Sets a number in the current Meta4Object.
	static m4return_t	SetItem(ClAccess* ai_pAccess, Nodes ai_iNode, Items ai_iItem, m4uint32_t ai_iNumber);

	// Sets null value in the current Meta4Object.
	static m4return_t	SetItem(ClAccess* ai_pAccess, Nodes ai_iNode, Items ai_iItem);

	// Gets a pointer from the current Meta4Object.
	static m4return_t	GetItem(ClAccess* ai_pAccess, Nodes ai_iNode, Items ai_iItem, void*& ao_pObject);

	// Gets a string from the current Meta4Object.
	static m4return_t	GetItem(ClAccess* ai_pAccess, Nodes ai_iNode, Items ai_iItem, m4pchar_t& ao_pcString);

	// Gets a number from the current Meta4Object.
	static m4return_t	GetItem(ClAccess* ai_pAccess, Nodes ai_iNode, Items ai_iItem, m4uint32_t& ao_iNumber);

	// Removes a pointer in the current Meta4Object.
	static m4return_t	ErasePointerItem(ClAccess* ai_pAccess, Nodes ai_iNode, Items ai_iItem);

	// Saves a source object, a transformation key and its resulting object.
	static m4return_t	SaveObjTransformation(ClAccess* ai_pAccess, Nodes ai_iNode, void* ai_pSrcObject, char* ai_pcKey, void* ai_pTgtObject);

	// Retrieves a existing object resulting of applying a transformation key on a given one.
	static m4return_t	GetObjTransformation(ClAccess* ai_pAccess, Nodes ai_iNode, void* ai_pSrcObject, char* ai_pcKey, void*& ao_pTgtObject);

	// Returns, if known, the number format Id of a format pattern string.
	static m4return_t	GetNumberFormat(char* ai_pcPattern, ClAccess* ai_pAccess, int& ao_iId);

	// Returns, if known, the number format pattern of a number format Id.
	static m4return_t	GetNumberFormat(int ai_iId, ClAccess* ai_pAccess, char*& ao_pcPattern);

private:

	static const char* s_apcNodeNames[];
	static const char* s_apcItemNames[];

	// Sets a variant in the current Meta4Object.
	static m4return_t setItem(ClAccess* ai_pAccess, Nodes ai_iNode, Items ai_iItem, m4VariantType& ai_vData);

	// Gets a variant from the current Meta4Object.
	static m4return_t getItem(ClAccess* ai_pAccess, Nodes ai_iNode, Items ai_iItem, m4VariantType& ao_vData);
};

typedef class ClXLAPIChannel APIChannel;

#endif
