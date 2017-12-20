//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapitemdef.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//     Implementation of CWrapItemDef
//
//==============================================================================

#include "wores.hpp"
#include "stdafx.h"
#include "m4chwrap.h"
#include "wrapitemdef.h"
#include "itemdef.hpp"
#include "wrapdef.h"
#include "gstring.h"
#include "item.hpp"
#include "rerror.h"
#include "access.hpp"
#include "channel.hpp"
#include "cllstr.hpp"

/////////////////////////////////////////////////////////////////////////////
// CWrapItemDef
STDMETHODIMP CWrapItemDef::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = { &IID_IWrapItemDef };

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
		{
			return S_OK;
		}
	}
	return S_FALSE;
}

CWrapItemDef::CWrapItemDef()
	: m_poItem(0)
{
}

CWrapItemDef::~CWrapItemDef()
{
	m_poItem = 0;
}

void CWrapItemDef::Init(ClItem* ai_VarRef)
{
	m_poItem = ai_VarRef;

	//m_poItem->Index = m_poItem->Index;
	TRACE3("Initialising with item address %p and index %d", m_poItem, static_cast<long>(m_poItem->GetIndex()));
}

STDMETHODIMP CWrapItemDef::get_Id(BSTR* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_Id);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.Id());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_Id, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_Id, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_Name(BSTR* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_Name);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.Name());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_Name, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_Name, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_Dmd(BSTR* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_Dmd);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.Dmd());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_Dmd, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_Dmd, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_DmdComponent(BSTR* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_DmdComponent);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.DmdComponent());

	if(tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_DmdComponent, S_OK);

	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_DmdComponent, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_DmdField(BSTR* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_DmdField);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.DmdField());
	
	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_DmdField, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_DmdField, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_Precision(long* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_Precision);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.Precision();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_Precision, S_OK);
}

STDMETHODIMP CWrapItemDef::get_MaxSize(long* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_MaxSize);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.MaxSize();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_MaxSize, S_OK);
}

STDMETHODIMP CWrapItemDef::get_ReadObjectAlias(BSTR* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_ReadObjectAlias);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.ReadObjectAlias());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_ReadObjectAlias, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_ReadObjectAlias, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_WriteObjectAlias(BSTR* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_WriteObjectAlias);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.WriteObjectAlias());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_WriteObjectAlias, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_WriteObjectAlias, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_ReadFieldId(BSTR* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_ReadFieldId);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.ReadFieldId());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_ReadFieldId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_ReadFieldId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_WriteFieldId(BSTR* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_WriteFieldId);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.WriteFieldId());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_WriteFieldId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_WriteFieldId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_Index(short* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_Index);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.Index();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_Index, S_OK);
}

STDMETHODIMP CWrapItemDef::get_Type(short* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_Type);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.Type();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_Type, S_OK);
}

STDMETHODIMP CWrapItemDef::get_TheoricScope(short* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_TheoricScope);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.TheoricScope();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_TheoricScope, S_OK);
}

STDMETHODIMP CWrapItemDef::get_M4Type(short* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_M4Type);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.M4Type();
		
	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_M4Type, S_OK);
}

STDMETHODIMP CWrapItemDef::get_CppType(short* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_CppType);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.CppType();
	
	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_CppType, S_OK);
}

STDMETHODIMP CWrapItemDef::get_Scale(short* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_Scale);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.Scale();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_Scale, S_OK);
}

STDMETHODIMP CWrapItemDef::get_SliceBhrv(short* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_SliceBhrv);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.SliceBhrv();
	
	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_SliceBhrv, S_OK);
}

STDMETHODIMP CWrapItemDef::get_SliceSplit(short* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_SliceSplit);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.SliceSplit();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_SliceSplit, S_OK);
}

STDMETHODIMP CWrapItemDef::get_DirtyCheck(short* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_DirtyCheck);

// esta propiedad desaparece	
	*pVal = 0;

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_DirtyCheck, S_OK);
}

STDMETHODIMP CWrapItemDef::get_IdTotalize(short* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_IdTotalize);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.IdTotalize();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_IdTotalize, S_OK);
}

STDMETHODIMP CWrapItemDef::get_TransactionMode(short* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_TransactionMode);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.TransactionMode();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_TransactionMode, S_OK);
}

STDMETHODIMP CWrapItemDef::get_MethodLevel(short* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_MethodLevel);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.MethodLevel();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_MethodLevel, S_OK);
}

STDMETHODIMP CWrapItemDef::get_IsVariableLength(short* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_IsVariableLength);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.IsVariableLength();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_IsVariableLength, S_OK);
}

STDMETHODIMP CWrapItemDef::get_SentTotType(short* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_SentTotType);
		
	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.SentTotType();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_SentTotType, S_OK);
}

STDMETHODIMP CWrapItemDef::get_Scope(short* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_Scope);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.Scope();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_Scope, S_OK);
}

STDMETHODIMP CWrapItemDef::get_Order(SHORT* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_Order);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.Order();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_Order, S_OK);
}

STDMETHODIMP CWrapItemDef::get_IsVisible(BYTE* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_IsVisible);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.IsVisible();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_IsVisible, S_OK);
}

STDMETHODIMP CWrapItemDef::get_ModifyData(BYTE* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_ModifyData);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.ModifyData();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_ModifyData, S_OK);
}

STDMETHODIMP CWrapItemDef::get_NumberOfArguments(SHORT* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_NumberOfArguments);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.NumberOfArguments();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_NumberOfArguments, S_OK);
}

STDMETHODIMP
CWrapItemDef::get_NumberOfReferedArguments(SHORT* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_NumberOfReferedArguments);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.NumberOfReferedArguments();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_NumberOfReferedArguments, S_OK);
}

STDMETHODIMP CWrapItemDef::get_ArgumentTypeByPosition(SHORT ai_pos, SHORT* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_ArgumentTypeByPosition);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.ArgumentTypeByPosition(ai_pos);

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_ArgumentTypeByPosition, S_OK);
}

STDMETHODIMP CWrapItemDef::get_ArgumentIdByPosition(SHORT ai_pos, BSTR* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_ArgumentIdByPosition);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.ArgumentIdByPosition(ai_pos));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_ArgumentIdByPosition, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_ArgumentIdByPosition, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_ArgumentM4TypeByPosition(short ai_pos, short* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_ArgumentM4TypeByPosition);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.ArgumentM4TypeByPosition(ai_pos);

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_ArgumentM4TypeByPosition, S_OK);
}

STDMETHODIMP CWrapItemDef::get_NumberOfConditionants(short* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_NumberOfConditionants);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.NumberOfConditionants();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_NumberOfConditionants, S_OK);
}

STDMETHODIMP CWrapItemDef::get_ConditionantIndexByPosition(short ai_pos, short* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_ConditionantIndexByPosition);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.ConditionantIndexByPosition(ai_pos);

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_ConditionantIndexByPosition, S_OK);
}

STDMETHODIMP CWrapItemDef::get_ConditionantIdByPosition(short ai_pos, BSTR* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_ConditionantIdByPosition);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.ConditionantIdByPosition(ai_pos));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_ConditionantIdByPosition, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_ConditionantIdByPosition, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_ConditionantNodeIndexByPosition(short ai_pos, short* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_ConditionantNodeIndexByPosition);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.ConditionantNodeIndexByPosition(ai_pos);

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_ConditionantNodeIndexByPosition, S_OK);
}

STDMETHODIMP CWrapItemDef::get_ConditionantNodeIdByPosition(short ai_pos, BSTR* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_ConditionantNodeIdByPosition);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.ConditionantNodeIdByPosition(ai_pos));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_ConditionantNodeIdByPosition, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_ConditionantNodeIdByPosition, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_ReadObjectId(BSTR* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_ReadObjectId);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.ReadObjectId());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_ReadObjectId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_ReadObjectId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_WriteObjectId(BSTR* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_WriteObjectId);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.WriteObjectId());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_WriteObjectId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_WriteObjectId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_VariableArguments(BYTE * pVal)
{
	ENTER_METHOD(CWrapItemDef::get_VariableArguments);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.VariableArguments();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_VariableArguments, S_OK);
}

STDMETHODIMP CWrapItemDef::get_InternalType(BYTE* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_InternalType);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.InternalType();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_InternalType, S_OK);
}

STDMETHODIMP CWrapItemDef::get_SliceItem(BSTR* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_SliceItem);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.SliceItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_SliceItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_SliceItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_AuxiliarItem(BSTR* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_AuxiliarItem);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.AuxiliarItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_AuxiliarItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_AuxiliarItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_AuxiliarItemProrat(BSTR* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_AuxiliarItemProrat);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.AuxiliarItemProrat());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_AuxiliarItemProrat, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_AuxiliarItemProrat, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_FatherItem(BSTR* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_FatherItem);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.FatherItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_FatherItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_FatherItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_FatherItemNode(BSTR* pVal)
{
	ENTER_METHOD(CWrapItemDef::get_FatherItemNode);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.FatherItemNode());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_FatherItemNode, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_FatherItemNode, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_DefaultValue(VARIANT * pVal)
{
	m4VariantType var;

	ENTER_METHOD(CWrapItemDef::get_DefaultValue);

	(*m_poItem)[m_poItem->GetIndex()].ItemDef.GetItemDefaultValue( var ) ;

	var.GetVARIANT(*pVal);

	LEAVE_METHOD(CWrapItemDef::get_DefaultValue, S_OK);
}

STDMETHODIMP CWrapItemDef::get_IsInherited(short * pVal)
{
	// TODO: Add your implementation code here

	ENTER_METHOD(CWrapItemDef::get_IsInherited);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.IsInherited();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_IsInherited, S_OK);
}

STDMETHODIMP CWrapItemDef::get_NumberOfAssigned(short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_NumberOfAssigned);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.NumberOfAssigned();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_NumberOfAssigned, S_OK);
}

STDMETHODIMP CWrapItemDef::get_AssignedIndex(short ai_AssignedPosition, short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_AssignedIndex);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.AssignedIndex( ai_AssignedPosition );

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_AssignedIndex, S_OK);
}

STDMETHODIMP CWrapItemDef::get_AssignedNodeIndex(short ai_AssignedPosition, short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_AssignedNodeIndex);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.AssignedNodeIndex( ai_AssignedPosition );

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_AssignedNodeIndex, S_OK);
}

STDMETHODIMP CWrapItemDef::get_AssignedId(short ai_AssignedPosition, BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_AssignedId);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.AssignedId( ai_AssignedPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_AssignedId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_AssignedId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_AssignedNodeId(short ai_AssignedPosition, BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_AssignedNodeId);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.AssignedNodeId( ai_AssignedPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_AssignedNodeId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_AssignedNodeId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_NumberOfExecuted(short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_NumberOfExecuted);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.NumberOfExecuted();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_NumberOfExecuted, S_OK);
}

STDMETHODIMP CWrapItemDef::get_ExecutedIndex(short ai_ExecutedPosition, short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_ExecutedIndex);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.ExecutedIndex( ai_ExecutedPosition );

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_ExecutedIndex, S_OK);
}

STDMETHODIMP CWrapItemDef::get_ExecutedNodeIndex(short ai_ExecutedPosition, short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_ExecutedNodeIndex);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.ExecutedNodeIndex( ai_ExecutedPosition );

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_ExecutedNodeIndex, S_OK);
}

STDMETHODIMP CWrapItemDef::get_ExecutedId(short ai_ExecutedPosition, BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_ExecutedId);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.ExecutedId( ai_ExecutedPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_ExecutedId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_ExecutedId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_ExecutedNodeId(short ai_ExecutedPosition, BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_ExecutedNodeId);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.ExecutedNodeId( ai_ExecutedPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_ExecutedNodeId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_ExecutedNodeId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_NumberOfFormIds(short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_NumberOfFormIds);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.NumberOfFormIds();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_NumberOfFormIds, S_OK);
}

STDMETHODIMP CWrapItemDef::get_FormId(short ai_iFormIdPosition, BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_FormId);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.FormId( ai_iFormIdPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_FormId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_FormId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_DmdCrossTab(BSTR * pVal)
{
	ENTER_METHOD(CWrapItemDef::get_DmdCrossTab);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.DmdCrossTab());

	if(tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_DmdCrossTab, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_DmdCrossTab, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_Synonym(long * pVal)
{
	ENTER_METHOD(CWrapItemDef::get_Synonym);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.Synonym();
	
	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_Synonym, S_OK);
}

STDMETHODIMP CWrapItemDef::get_CsType(short * pVal)
{
	ENTER_METHOD(CWrapItemDef::get_CsType);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.CsType();
	
	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_CsType, S_OK);
}

STDMETHODIMP CWrapItemDef::get_SyncType(short * pVal)
{
	ENTER_METHOD(CWrapItemDef::get_SyncType);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.SyncType();
	
	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_SyncType, S_OK);
}

STDMETHODIMP CWrapItemDef::get_SecRead(short * pVal)
{
	ENTER_METHOD(CWrapItemDef::get_SecRead);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.SecRead();
	
	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_SecRead, S_OK);
}

STDMETHODIMP CWrapItemDef::get_SecWrite(short * pVal)
{
	ENTER_METHOD(CWrapItemDef::get_SecWrite);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.SecWrite();
	
	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_SecWrite, S_OK);
}

STDMETHODIMP CWrapItemDef::get_SecDelete(short * pVal)
{
	ENTER_METHOD(CWrapItemDef::get_SecDelete);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.SecDelete();
	
	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_SecDelete, S_OK);
}

STDMETHODIMP CWrapItemDef::get_SecUpdate(short * pVal)
{
	ENTER_METHOD(CWrapItemDef::get_SecUpdate);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.SecUpdate();
	
	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_SecUpdate, S_OK);
}

STDMETHODIMP CWrapItemDef::get_SecExecute(short * pVal)
{
	ENTER_METHOD(CWrapItemDef::get_SecExecute);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.SecExecute();
	
	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_SecExecute, S_OK);
}

STDMETHODIMP CWrapItemDef::get_ConnectorFatherNodeByPosition(short ai_iPosition, BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_ConnectorFatherNodeByPosition);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.ConnectorFatherNodeByPosition( ai_iPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_ConnectorFatherNodeByPosition, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_ConnectorFatherNodeByPosition, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_ConnectorFatherItemByPosition(short ai_iPosition, BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_ConnectorFatherItemByPosition);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.ConnectorFatherItemByPosition( ai_iPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_ConnectorFatherItemByPosition, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_ConnectorFatherItemByPosition, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_ConnectorSonNodeByPosition(short ai_iPosition, BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_ConnectorSonNodeByPosition);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.ConnectorSonNodeByPosition( ai_iPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_ConnectorSonNodeByPosition, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_ConnectorSonNodeByPosition, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_ConnectorSonItemByPosition(short ai_iPosition, BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_ConnectorSonItemByPosition);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.ConnectorSonItemByPosition( ai_iPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_ConnectorSonItemByPosition, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_ConnectorSonItemByPosition, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_ConnectorPrecedenceByPosition(short ai_iPosition, short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_ConnectorPrecedenceByPosition);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.ConnectorPrecedenceByPosition( ai_iPosition );

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_ConnectorPrecedenceByPosition, S_OK);
}

STDMETHODIMP CWrapItemDef::get_ConnectorSpinByPosition(short ai_iPosition, short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_ConnectorSpinByPosition);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.ConnectorSpinByPosition( ai_iPosition );

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_ConnectorSpinByPosition, S_OK);
}

STDMETHODIMP CWrapItemDef::get_ConnectorRelationByPosition(short ai_iPosition, short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_ConnectorRelationByPosition);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.ConnectorRelationByPosition( ai_iPosition );

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_ConnectorRelationByPosition, S_OK);
}

STDMETHODIMP CWrapItemDef::get_ConnectorContextByPosition(short ai_iPosition, short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_ConnectorContextByPosition);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.ConnectorContextByPosition( ai_iPosition );

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_ConnectorContextByPosition, S_OK);
}

STDMETHODIMP CWrapItemDef::get_ConnectorCsTypeByPosition(short ai_iPosition, short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_ConnectorCsTypeByPosition);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.ConnectorCsTypeByPosition( ai_iPosition );

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_ConnectorCsTypeByPosition, S_OK);
}

STDMETHODIMP CWrapItemDef::get_NumberOfConnectedItems(short * pVal)
{
	ENTER_METHOD(CWrapItemDef::get_NumberOfConnectedItems);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.NumberOfConnectedItems();
	
	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_NumberOfConnectedItems, S_OK);
}

STDMETHODIMP CWrapItemDef::get_IsPublic(BYTE *pVal)
{
	ENTER_METHOD(CWrapItemDef::get_IsPublic);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.IsPublic();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_IsPublic, S_OK);
}

STDMETHODIMP CWrapItemDef::get_NumberOfDependents(short * pVal)
{
	ENTER_METHOD(CWrapItemDef::get_NumberOfDependents);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.NumberOfDependents();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_NumberOfDependents, S_OK);
}

STDMETHODIMP CWrapItemDef::get_DependentIndexByPosition(short ai_pos, short * pVal)
{
	ENTER_METHOD(CWrapItemDef::get_DependentIndexByPosition);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.DependentIndexByPosition(ai_pos);

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_DependentIndexByPosition, S_OK);
}

STDMETHODIMP CWrapItemDef::get_DependentIdByPosition(short ai_pos, BSTR * pVal)
{
	ENTER_METHOD(CWrapItemDef::get_DependentIdByPosition);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.DependentIdByPosition(ai_pos));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_DependentIdByPosition, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_DependentIdByPosition, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_DependentNodeIndexByPosition(short ai_pos, short * pVal)
{
	ENTER_METHOD(CWrapItemDef::get_DependentNodeIndexByPosition);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.DependentNodeIndexByPosition(ai_pos);

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_DependentNodeIndexByPosition, S_OK);
}

STDMETHODIMP CWrapItemDef::get_DependentNodeIdByPosition(short ai_pos, BSTR * pVal)
{
	ENTER_METHOD(CWrapItemDef::get_DependentNodeIdByPosition);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.DependentNodeIdByPosition(ai_pos));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_DependentNodeIdByPosition, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_DependentNodeIdByPosition, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_CheckNotNull(short * pVal)
{
	ENTER_METHOD(CWrapItemDef::get_CheckNotNull);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.CheckNotNull();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_CheckNotNull, S_OK);
}

STDMETHODIMP CWrapItemDef::get_ClientNotNull(short * pVal)
{
	ENTER_METHOD(CWrapItemDef::get_ClientNotNull);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.ClientNotNull();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_ClientNotNull, S_OK);
}

STDMETHODIMP CWrapItemDef::get_IsNodalItem(short * pVal)
{
	ENTER_METHOD(CWrapItemDef::get_IsNodalItem);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.IsNodalItem();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_IsNodalItem, S_OK);
}

STDMETHODIMP CWrapItemDef::get_IsEventItem(short * pVal)
{
	ENTER_METHOD(CWrapItemDef::get_IsEventItem);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.IsEventItem();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_IsEventItem, S_OK);
}

STDMETHODIMP CWrapItemDef::get_AffectsDB(short * pVal)
{
	ENTER_METHOD(CWrapItemDef::get_AffectsDB);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.AffectsDB();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_AffectsDB, S_OK);
}

STDMETHODIMP CWrapItemDef::get_IsPK(short * pVal)
{
	ENTER_METHOD(CWrapItemDef::get_IsPK);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.IsPK();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_IsPK, S_OK);
}

STDMETHODIMP CWrapItemDef::get_ConvertFunction(short * pVal)
{
	ENTER_METHOD(CWrapItemDef::get_ConvertFunction);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.ConvertFunction();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_ConvertFunction, S_OK);
}

STDMETHODIMP CWrapItemDef::get_ArgumentPrecisionByPosition(short ai_iPosition, short * pVal)
{
	ENTER_METHOD(CWrapItemDef::get_ArgumentPrecisionByPosition);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.ArgumentPrecisionByPosition(ai_iPosition);

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_ArgumentPrecisionByPosition, S_OK);
}

STDMETHODIMP CWrapItemDef::get_ArgumentPositionById(BSTR ai_pccArgumentId, short * pVal)
{
	ENTER_METHOD(CWrapItemDef::get_ArgumentPositionById);

	gstring	tmpUArgumentId( ai_pccArgumentId ) ;

	if( tmpUArgumentId != NULL )
	{
		*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.ArgumentPositionById(tmpUArgumentId);
	}
	else
	{
		*pVal = -1;
	}

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_ArgumentPositionById, S_OK);
}

STDMETHODIMP CWrapItemDef::get_GenerateSlices(short * pVal)
{
	ENTER_METHOD(CWrapItemDef::get_GenerateSlices);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.GenerateSlices();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_GenerateSlices, S_OK);
}

STDMETHODIMP CWrapItemDef::get_ArgumentNameByPosition(SHORT ai_pos, BSTR * pVal)
{
	ENTER_METHOD(CWrapItemDef::get_ArgumentNameByPosition);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.ArgumentNameByPosition(ai_pos));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_ArgumentNameByPosition, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_ArgumentNameByPosition, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_SliceTotalize(short * pVal)
{
	ENTER_METHOD(CWrapItemDef::get_SliceTotalize);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.SliceTotalize();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_SliceTotalize, S_OK);
}

STDMETHODIMP CWrapItemDef::get_SecContinue(short * pVal)
{
	ENTER_METHOD(CWrapItemDef::get_SecContinue);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.SecContinue();
	
	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_SecContinue, S_OK);
}

STDMETHODIMP CWrapItemDef::get_TiId(BSTR *pVal)
{
	ENTER_METHOD(CWrapItemDef::get_TiId);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.TiId());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_TiId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_TiId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_MaxCodes(short *pVal)
{
	ENTER_METHOD(CWrapItemDef::get_MaxCodes);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.MaxCodes();
	
	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_MaxCodes, S_OK);
}

STDMETHODIMP CWrapItemDef::get_GetNextCode(short ai_iNext, short *pVal)
{
	ENTER_METHOD(CWrapItemDef::get_GetNextCode);

	(*m_poItem)[m_poItem->GetIndex()].ItemDef.GetNextCode( ai_iNext );

	*pVal = ai_iNext ;
	
	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_GetNextCode, S_OK);
}

STDMETHODIMP CWrapItemDef::get_CodeRuleId(short ai_iPosition, BSTR *pVal)
{
	ENTER_METHOD(CWrapItemDef::get_CodeRuleId);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.CodeRuleId( ai_iPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_CodeRuleId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_CodeRuleId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_CodeStartDate(short ai_iPosition, DATE *pVal)
{
	ENTER_METHOD(CWrapItemDef::get_CodeStartDate);

	*pVal =M4dateToDate( (*m_poItem)[m_poItem->GetIndex()].ItemDef.CodeStartDate( ai_iPosition ) );
	
	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_CodeStartDate, S_OK);
}

STDMETHODIMP CWrapItemDef::get_CodeEndDate(short ai_iPosition, DATE *pVal)
{
	ENTER_METHOD(CWrapItemDef::get_CodeEndDate);

	*pVal =M4dateToDate( (*m_poItem)[m_poItem->GetIndex()].ItemDef.CodeEndDate( ai_iPosition ) );
	
	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_CodeEndDate, S_OK);
}

STDMETHODIMP CWrapItemDef::get_CodeRuleTiId(short ai_iPosition, BSTR *pVal)
{
	ENTER_METHOD(CWrapItemDef::get_CodeRuleTiId);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.CodeRuleTiId( ai_iPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_CodeRuleTiId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_CodeRuleTiId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_CodeNumberOfVariales(short ai_iPosition, long *pVal)
{
	ENTER_METHOD(CWrapItemDef::get_CodeNumberOfVariales);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.CodeNumberOfVariales( ai_iPosition );
	
	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_CodeNumberOfVariales, S_OK);
}

STDMETHODIMP CWrapItemDef::get_CodeNumberOfInstructions(short ai_iPosition, long *pVal)
{
	ENTER_METHOD(CWrapItemDef::get_CodeNumberOfInstructions);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.CodeNumberOfInstructions( ai_iPosition );
	
	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_CodeNumberOfInstructions, S_OK);
}

STDMETHODIMP CWrapItemDef::get_CodeSize(short ai_iPosition, long *pVal)
{
	ENTER_METHOD(CWrapItemDef::get_CodeSize);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.CodeSize( ai_iPosition );
	
	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_CodeSize, S_OK);
}

STDMETHODIMP CWrapItemDef::get_CodeType(short ai_iPosition, short *pVal)
{
	ENTER_METHOD(CWrapItemDef::get_CodeType);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.CodeType( ai_iPosition );
	
	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_CodeType, S_OK);
}

STDMETHODIMP CWrapItemDef::get_CodeMustAssign(short ai_iPosition, short *pVal)
{
	ENTER_METHOD(CWrapItemDef::get_CodeMustAssign);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.CodeMustAssign( ai_iPosition );
	
	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_CodeMustAssign, S_OK);
}

STDMETHODIMP CWrapItemDef::get_CodePriority(short ai_iPosition, short *pVal)
{
	ENTER_METHOD(CWrapItemDef::get_CodePriority);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.CodePriority( ai_iPosition );
	
	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_CodePriority, S_OK);
}

STDMETHODIMP CWrapItemDef::get_CodeAutoGenerated(short ai_iPosition, short *pVal)
{
	ENTER_METHOD(CWrapItemDef::get_CodeAutoGenerated);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.CodeAutoGenerated( ai_iPosition );
	
	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_CodeAutoGenerated, S_OK);
}

STDMETHODIMP CWrapItemDef::get_CodeSource(short ai_iPosition, BSTR *pVal)
{
	ENTER_METHOD(CWrapItemDef::get_CodeSource);

	m4return_t		iResult ;
	m4pcchar_t		pccCode ;
	ClLongString	oString ;

	oString.Init( 2000, 500 ) ;

	iResult = (*m_poItem)[m_poItem->GetIndex()].ItemDef.CodeSource( ai_iPosition, oString ) ;

	if( iResult != M4_SUCCESS )
	{
		pccCode = NULL ;
	}
	else
	{
		pccCode = oString.GetString() ;
	}

	if( pccCode == NULL )
	{
		pccCode = "" ;
	}

	gstring tmpGstring( pccCode );

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		
		LEAVE_METHOD(CWrapItemDef::get_CodeSource, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_CodeSource, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_NotNull(short *pVal)
{
	ENTER_METHOD(CWrapItemDef::get_NotNull);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.NotNull();

	TRACE2("Returning %d", *pVal);
	LEAVE_METHOD(CWrapItemDef::get_NotNull, S_OK);
}


STDMETHODIMP CWrapItemDef::get_NumberOfAssignedDependents(short *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_NumberOfAssignedDependents);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.NumberOfAssignedDependents();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_NumberOfAssignedDependents, S_OK);
}

STDMETHODIMP CWrapItemDef::get_AssignedDependentIndex(short ai_AssignedDependentPosition, short *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_AssignedDependentIndex);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.AssignedDependentIndex( ai_AssignedDependentPosition );

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_AssignedDependentIndex, S_OK);
}

STDMETHODIMP CWrapItemDef::get_AssignedDependentNodeIndex(short ai_AssignedDependentPosition, short *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_AssignedDependentNodeIndex);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.AssignedDependentNodeIndex( ai_AssignedDependentPosition );

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_AssignedDependentNodeIndex, S_OK);
}

STDMETHODIMP CWrapItemDef::get_AssignedDependentId(short ai_AssignedDependentPosition, BSTR *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_AssignedDependentId);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.AssignedDependentId( ai_AssignedDependentPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_AssignedDependentId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_AssignedDependentId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_AssignedDependentNodeId(short ai_AssignedDependentPosition, BSTR *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_AssignedDependentNodeId);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.AssignedDependentNodeId( ai_AssignedDependentPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_AssignedDependentNodeId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_AssignedDependentNodeId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_NumberOfExecutedDependents(short *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_NumberOfExecutedDependents);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.NumberOfExecutedDependents();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_NumberOfExecutedDependents, S_OK);
}

STDMETHODIMP CWrapItemDef::get_ExecutedDependentIndex(short ai_ExecutedDependentPosition, short *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_ExecutedDependentIndex);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.ExecutedDependentIndex( ai_ExecutedDependentPosition );

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_ExecutedDependentIndex, S_OK);
}

STDMETHODIMP CWrapItemDef::get_ExecutedDependentNodeIndex(short ai_ExecutedDependentPosition, short *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_ExecutedDependentNodeIndex);

	*pVal = (*m_poItem)[m_poItem->GetIndex()].ItemDef.ExecutedDependentNodeIndex( ai_ExecutedDependentPosition );

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_ExecutedDependentNodeIndex, S_OK);
}

STDMETHODIMP CWrapItemDef::get_ExecutedDependentId(short ai_ExecutedDependentPosition, BSTR *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_ExecutedDependentId);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.ExecutedDependentId( ai_ExecutedDependentPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_ExecutedDependentId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_ExecutedDependentId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_ExecutedDependentNodeId(short ai_ExecutedDependentPosition, BSTR *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_ExecutedDependentNodeId);

	gstring tmpGstring((*m_poItem)[m_poItem->GetIndex()].ItemDef.ExecutedDependentNodeId( ai_ExecutedDependentPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_ExecutedDependentNodeId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_ExecutedDependentNodeId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapItemDef::get_IsConnected(VARIANT_BOOL *pVal)
{
	ENTER_METHOD(CWrapItemDef::get_IsConnected);
	
	*pVal = ((*m_poItem)[m_poItem->GetIndex()].ItemDef.IsConnected()) ? VARIANT_TRUE : VARIANT_FALSE;

	LEAVE_METHOD(CWrapItemDef::get_IsConnected, S_OK);
}
