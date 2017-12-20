//==============================================================================
//
// (c) Copyright  1991-2017 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4xlapi
// File:             m4xlapichannel.cpp
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

#include <m4xlapichannel.hpp>

#include <m4xlapierrors.hpp>
#include <execontx.hpp>



// -----------------------------------------------------------------------------
// Class CLXLChannel
// -----------------------------------------------------------------------------


// XL API channel node names in the same order as eXlNodes enumeration.
const char*	ClXLAPIChannel::s_apcNodeNames[] = { "SNTC_EXCEL_CPP_WORKBOOK", "SNTC_EXCEL_CPP_WORKSHEET", "SNTC_LIBXL_FORMATS", "SNTC_LIBXL_FONTS", "SNTC_LIBXL_NUMFORMATS" };

// XL API channel item names in the same order as eXlItems enumeration.
const char* ClXLAPIChannel::s_apcItemNames[] = { "EXCEL_WORKBOOK", "WORKBOOK_PATH", "_TRACE_BASE_PATH", "_TRACE_KEY", \
												"EXCEL_WORKSHEET", "EXCEL_WORKSHEET_INDEX", "EXCEL_WORKSHEET_NAME", \
												"SRC_FORMAT", "KEY", "FORMAT", \
												"SRC_FONT", "KEY", "FONT", \
												"NUMFORMAT_ID", "NUMFORMAT_STR" };


// -----------------------------------------------------------------------------
// Adds a new register in the node.
//
//	ai_pAccess: Channel access.
//	ai_iNode:	Node name index.
// -----------------------------------------------------------------------------
m4return_t ClXLAPIChannel::AddRegister(ClAccess* ai_pAccess, Nodes ai_iNode)
{
	ClNode* pNode = &(ai_pAccess->Node[s_apcNodeNames[ai_iNode]]);


	if (pNode == NULL)
	{
		return M4_ERROR;
	}

	pNode->RecordSet.Current.Add();

	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Sets a pointer in the current Meta4Object.
//
//	ai_pAccess:		XL channel access.
//	ai_iNode:		Node name index.
//	ai_iItem:		Item name index.
//	ai_pObject:		Input pointer.
// -----------------------------------------------------------------------------
m4return_t ClXLAPIChannel::SetItem(ClAccess* ai_pAccess, Nodes ai_iNode, Items ai_iItem, void* ai_pObject)
{
	m4uint32_t iHandle = ai_pAccess->GetpChannel()->GetPointerKey(ai_pObject);


	if (iHandle == 0)
	{
		iHandle = ai_pAccess->GetpChannel()->AddPointer(ai_pObject);
	}

	return SetItem(ai_pAccess, ai_iNode, ai_iItem, iHandle);
}


// -----------------------------------------------------------------------------
// Sets a string in the current Meta4Object.
//
//	ai_pAccess:		XL channel access.
//	ai_iNode:		Node name index.
//	ai_iItem:		Item name index.
//	ai_pcString:	Input string.
// -----------------------------------------------------------------------------
m4return_t ClXLAPIChannel::SetItem(ClAccess* ai_pAccess, Nodes ai_iNode, Items ai_iItem, m4pchar_t ai_pcString)
{
	m4VariantType vString;


	vString.Type = M4CL_CPP_TYPE_STRING_VAR;
	vString.Data.PointerChar = ai_pcString;

	return setItem(ai_pAccess, ai_iNode, ai_iItem, vString);
}


// -----------------------------------------------------------------------------
// Sets a number in the current Meta4Object.
//
//	ai_pAccess:		XL channel access.
//	ai_iNode:		Node name index.
//	ai_iItem:		Item name index.
//	ai_iNumber:		Input number.
// -----------------------------------------------------------------------------
m4return_t ClXLAPIChannel::SetItem(ClAccess* ai_pAccess, Nodes ai_iNode, Items ai_iItem, m4uint32_t ai_iNumber)
{
	m4VariantType vNumber;


	vNumber.Type = M4CL_CPP_TYPE_NUMBER;
	vNumber.Data.DoubleData = ai_iNumber;

	return setItem(ai_pAccess, ai_iNode, ai_iItem, vNumber);
}


// -----------------------------------------------------------------------------
// Sets null value in the current Meta4Object.
//
//	ai_pAccess:		XL channel access.
//	ai_iNode:		Node name index.
//	ai_iItem:		Item name index.
// -----------------------------------------------------------------------------
m4return_t ClXLAPIChannel::SetItem(ClAccess* ai_pAccess, Nodes ai_iNode, Items ai_iItem)
{
	m4VariantType vNull;


	vNull.Type = M4CL_CPP_TYPE_NULL;

	return setItem(ai_pAccess, ai_iNode, ai_iItem, vNull);
}


// -----------------------------------------------------------------------------
// Gets a pointer from the current Meta4Object.
//
//	ai_pAccess:		XL channel access.
//	ai_iNode:		Node name index.
//	ai_iItem:		Item name index.
//	ao_pObject:		Output pointer.
// -----------------------------------------------------------------------------
m4return_t ClXLAPIChannel::GetItem(ClAccess* ai_pAccess, Nodes ai_iNode, Items ai_iItem, void*& ao_pObject)
{
	m4return_t iRet;
	m4uint32_t iHandle;


	iRet = GetItem(ai_pAccess, ai_iNode, ai_iItem, iHandle);

	if (iRet == M4_ERROR || iHandle == 0)
	{
		return M4_ERROR;
	}

	ao_pObject = ai_pAccess->GetpChannel()->GetPointer(iHandle);

	if (ao_pObject == NULL)
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Gets a string from the current Meta4Object.
//
//	ai_pAccess:		XL channel access.
//	ai_iNode:		Node name index.
//	ai_iItem:		Item name index.
//	ao_pcString:	Output string.
// -----------------------------------------------------------------------------
m4return_t ClXLAPIChannel::GetItem(ClAccess* ai_pAccess, Nodes ai_iNode, Items ai_iItem, m4pchar_t& ao_pcString)
{
	m4return_t iRet;
	m4VariantType vString;


	iRet = getItem(ai_pAccess, ai_iNode, ai_iItem, vString);

	if (iRet == M4_ERROR || vString.Type != M4CL_CPP_TYPE_STRING_VAR)
	{
		return M4_ERROR;
	}

	ao_pcString = vString.Data.PointerChar;

	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Gets a number from the current Meta4Object.
//
//	ai_pAccess:		XL channel access.
//	ai_iNode:		Node name index.
//	ai_iItem:		Item name index.
//	ao_iNumber:		Output number.
// -----------------------------------------------------------------------------
m4return_t ClXLAPIChannel::GetItem(ClAccess* ai_pAccess, Nodes ai_iNode, Items ai_iItem, m4uint32_t& ao_iNumber)
{
	m4return_t iRet;
	m4VariantType vNumber;


	iRet = getItem(ai_pAccess, ai_iNode, ai_iItem, vNumber);

	if (iRet == M4_ERROR || vNumber.Type != M4CL_CPP_TYPE_NUMBER)
	{
		return M4_ERROR;
	}

	ao_iNumber = vNumber.Data.DoubleData;

	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Removes a pointer in the current Meta4Object.
//
//	ai_pAccess:		XL channel access.
//	ai_iNode:		Node name index.
//	ai_iItem:		Item name index.
// -----------------------------------------------------------------------------
m4return_t ClXLAPIChannel::ErasePointerItem(ClAccess* ai_pAccess, Nodes ai_iNode, Items ai_iItem)
{
	m4return_t iRet;
	m4uint32_t iHandle;


	iRet = GetItem(ai_pAccess, ai_iNode, ai_iItem, iHandle);

	if (iRet == M4_ERROR || iHandle == 0)
	{
		return M4_ERROR;
	}

	ai_pAccess->GetpChannel()->RemovePointer(iHandle);

	return SetItem(ai_pAccess, ai_iNode, ai_iItem);
}


// -----------------------------------------------------------------------------
// Saves a source object, a transformation key and its resulting object.
//
//	ai_pAccess:		XL channel access.
//	ai_iNode:		Node name index.
//	ai_pSrcObject:	Source object.
//	ai_pcKey:		Transformation key.
//	ai_pTgtObject:	Target object.
// -----------------------------------------------------------------------------
m4return_t	ClXLAPIChannel::SaveObjTransformation(ClAccess* ai_pAccess, Nodes ai_iNode, void* ai_pSrcObject, char* ai_pcKey, void* ai_pTgtObject)
{
	m4return_t		iRet;
	ClNode*			pNode;
	Items			iInitItem;
	m4uint32_t		iHandle;


	pNode = &(ai_pAccess->Node[s_apcNodeNames[ai_iNode]]);

	if (pNode == NULL)
	{
		return M4_ERROR;
	}

	if (ai_iNode == FORMATS)
	{
		iInitItem = FORMAT_SRC_POINTER;
	}
	else if (ai_iNode == FONTS)
	{
		iInitItem = FONT_SRC_POINTER;
	}
	else
	{
		return M4_ERROR;
	}

	iRet = AddRegister(ai_pAccess, ai_iNode);

	if (iRet == M4_ERROR)
	{
		return M4_ERROR;
	}

	SetItem(ai_pAccess, ai_iNode, iInitItem, ai_pSrcObject);
	SetItem(ai_pAccess, ai_iNode, (Items)(iInitItem + 1), ai_pcKey);
	SetItem(ai_pAccess, ai_iNode, (Items)(iInitItem + 2), ai_pTgtObject);


	return iRet;
}


// -----------------------------------------------------------------------------
// Retrieves a existing object resulting of applying a transformation key on a given one.
//
//	ai_pAccess:		XL channel access.
//	ai_iNode:		Node name index.
//	ai_pSrcObject:	Source object.
//	ai_pcKey:		Transformation key.
//	ao_pTgtObject:	Output target object.
// -----------------------------------------------------------------------------
m4return_t	ClXLAPIChannel::GetObjTransformation(ClAccess* ai_pAccess, Nodes ai_iNode, void* ai_pSrcObject, char* ai_pcKey, void*& ao_pTgtObject)
{
	m4return_t		iRet;
	ClNode*			pNode;
	m4uint32_t		iHandle;
	Items			iInitItem;
	m4VariantType	avArgs[6];
	m4int32_t		iReg = -1;
	m4VariantType	vVariant;


	ao_pTgtObject = NULL;

	pNode = &(ai_pAccess->Node[s_apcNodeNames[ai_iNode]]);

	if (pNode == NULL)
	{
		return M4_ERROR;
	}

	iHandle = ai_pAccess->GetpChannel()->GetPointerKey(ai_pSrcObject);

	if (iHandle == 0)
	{
		return M4_SUCCESS;
	}

	if (ai_iNode == FORMATS)
	{
		iInitItem = FORMAT_SRC_POINTER;
	}
	else if (ai_iNode == FONTS)
	{
		iInitItem = FONT_SRC_POINTER;
	}
	else
	{
		return M4_ERROR;
	}

	// Search comparison function.
	avArgs[2].Type = avArgs[5].Type = M4CL_CPP_TYPE_NUMBER;
	avArgs[2].Data.DoubleData = avArgs[5].Data.DoubleData = M4CL_EQUAL_FUNCTION;

	// Search criteria.
	avArgs[0].Type = M4CL_CPP_TYPE_STRING_VAR;
	avArgs[0].Data.PointerChar = (m4pchar_t)s_apcItemNames[iInitItem];
	avArgs[1].Type = M4CL_CPP_TYPE_NUMBER;
	avArgs[1].Data.DoubleData = iHandle;

	avArgs[3].Type = M4CL_CPP_TYPE_STRING_VAR;
	avArgs[3].Data.PointerChar = (m4pchar_t)s_apcItemNames[iInitItem + 1];
	avArgs[4].Type = M4CL_CPP_TYPE_STRING_VAR;
	avArgs[4].Data.PointerChar = ai_pcKey;

	// Performs the search in the node.
	iRet = pNode->RecordSet.Register.FindSec(avArgs, 6, &iReg);

	if (iRet == M4_SUCCESS && iReg > -1)
	{
		// Object found ...
		iRet = pNode->RecordSet.Register[iReg].Item[s_apcItemNames[iInitItem + 2]].Value.Get(vVariant);

		if (iRet == M4_SUCCESS && vVariant.Type == M4CL_CPP_TYPE_NUMBER)
		{
			// ... so reuse it.
			iHandle = (m4uint32_t)vVariant.Data.DoubleData;
			ao_pTgtObject = ai_pAccess->GetpChannel()->GetPointer(iHandle);

			if (ao_pTgtObject == NULL)
			{
				return M4_ERROR;
			}
		}
	}


	return M4_SUCCESS;
}


// -----------------------------------------------------------------------------
// Returns, if known, the number format Id of a format pattern string.
//
//	ai_pcPattern:	Number format string pattern
//	ai_pAccess:		M4Object instance.
//	ao_iId:			Output. Number format Id.
// -----------------------------------------------------------------------------
m4return_t ClXLAPIChannel::GetNumberFormat(char* ai_pcPattern, ClAccess* ai_pAccess, int& ao_iId)
{
	m4return_t		iRet;
	ClNode*			pNode;
	m4VariantType	avArgs[3];
	m4int32_t		iReg = -1;
	m4VariantType	vVariant;


	ao_iId = -1;

	pNode = &(ai_pAccess->Node[s_apcNodeNames[NUMFORMATS]]);

	if (pNode == NULL)
	{
		return M4_ERROR;
	}

	// Search comparison function.
	avArgs[2].Type = M4CL_CPP_TYPE_NUMBER;
	avArgs[2].Data.DoubleData = M4CL_EQUAL_FUNCTION;


	// Search criteria.
	avArgs[0].Type = M4CL_CPP_TYPE_STRING_VAR;
	avArgs[1].Type = M4CL_CPP_TYPE_STRING_VAR;
	avArgs[0].Data.PointerChar = (m4pchar_t)s_apcItemNames[NUMFORMAT_STRING];
	avArgs[1].Data.PointerChar = ai_pcPattern;

	// Performs the search in the node.
	iRet = pNode->RecordSet.Register.FindSec(avArgs, 3, &iReg);

	if (iRet == M4_SUCCESS && iReg > -1)
	{
		// Number format found.
		iRet = pNode->RecordSet.Register[iReg].Item[s_apcItemNames[NUMFORMAT_ID]].Value.Get(vVariant);

		if (iRet == M4_SUCCESS && vVariant.Type == M4CL_CPP_TYPE_NUMBER)
		{
			ao_iId = vVariant.Data.DoubleData;
		}
	}


	return iRet;
}


// -----------------------------------------------------------------------------
// Returns, if known, the number format pattern of a number format Id.
//
//	ai_iId:			Number format Id
//	ai_pAccess:		M4Object instance.
//	ao_iId:			Output. Number format string pattern.
// -----------------------------------------------------------------------------
m4return_t ClXLAPIChannel::GetNumberFormat(int ai_iId, ClAccess* ai_pAccess, char*& ao_pcPattern)
{
	m4return_t		iRet;
	ClNode*			pNode;
	m4VariantType	avArgs[3];
	m4int32_t		iReg = -1;
	m4VariantType	vVariant;


	ao_pcPattern = NULL;

	pNode = &(ai_pAccess->Node[s_apcNodeNames[NUMFORMATS]]);

	if (pNode == NULL)
	{
		return M4_ERROR;
	}

	// Search comparison function.
	avArgs[2].Type = M4CL_CPP_TYPE_NUMBER;
	avArgs[2].Data.DoubleData = M4CL_EQUAL_FUNCTION;


	// Search criteria.
	avArgs[0].Type = M4CL_CPP_TYPE_STRING_VAR;
	avArgs[1].Type = M4CL_CPP_TYPE_NUMBER;
	avArgs[0].Data.PointerChar = (char*)s_apcItemNames[NUMFORMAT_ID];
	avArgs[1].Data.DoubleData = ai_iId;

	// Performs the search in the node.
	iRet = pNode->RecordSet.Register.FindSec(avArgs, 3, &iReg);

	if (iRet == M4_SUCCESS && iReg > -1)
	{
		// Number format found.
		iRet = pNode->RecordSet.Register[iReg].Item[s_apcItemNames[NUMFORMAT_STRING]].Value.Get(vVariant);

		if (iRet == M4_SUCCESS && vVariant.Type == M4CL_CPP_TYPE_STRING_VAR)
		{
			ao_pcPattern = vVariant.Data.PointerChar;
		}
	}


	return iRet;
}

// -----------------------------------------------------------------------------
// Sets a variant in the current Meta4Object.
// -----------------------------------------------------------------------------
m4return_t ClXLAPIChannel::setItem(ClAccess* ai_pAccess, Nodes ai_iNode, Items ai_iItem, m4VariantType& ai_vData)
{
	ClNode* pNode = &(ai_pAccess->Node[s_apcNodeNames[ai_iNode]]);

	if (pNode == NULL)
	{
		return M4_ERROR;
	}

	return pNode->RecordSet.Current.Item[s_apcItemNames[ai_iItem]].Value.Set(ai_vData);
}


// -----------------------------------------------------------------------------
// Gets a variant from the current Meta4Object.
// -----------------------------------------------------------------------------
m4return_t ClXLAPIChannel::getItem(ClAccess* ai_pAccess, Nodes ai_iNode, Items ai_iItem, m4VariantType& ao_vData)
{
	ClNode* pNode = &(ai_pAccess->Node[s_apcNodeNames[ai_iNode]]);

	if (pNode == NULL)
	{
		return M4_ERROR;
	}

	return pNode->RecordSet.Current.Item[s_apcItemNames[ai_iItem]].Value.Get(ao_vData);
}
