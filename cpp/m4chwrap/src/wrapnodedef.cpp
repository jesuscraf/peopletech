//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapnodedef.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//     Implementation of CWrapNodeDef
//
//==============================================================================

#include "stdafx.h"
#include "m4chwrap.h"
#include "wrapnodedef.h"
#include "nodedef.hpp"
#include "wrapdef.h"
#include "gstring.h"
#include "cldates.hpp"
#include "cvdate.h"
#include "m4var.hpp"

/////////////////////////////////////////////////////////////////////////////
// CWrapNodeDef

CWrapNodeDef::CWrapNodeDef()
	: m_poNodeDef(0)
{
}

void CWrapNodeDef::Init(ClNodeDef* ai_NodeDef)
{
	m_poNodeDef = ai_NodeDef;
}

STDMETHODIMP CWrapNodeDef::get_PeriodCorrected(BYTE* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_PeriodCorrected);

	*pVal = m_poNodeDef->PeriodCorrected();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_PeriodCorrected, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_Completeness(BYTE* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_Completeness);

	*pVal = m_poNodeDef->Completeness();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_Completeness, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_Temporality(BYTE* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_Temporality);

	*pVal = m_poNodeDef->Temporality();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_Temporality, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_UniqueRow(BYTE* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_UniqueRow);

// esta propiedad desaparece	
	*pVal = 0;

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_UniqueRow, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_AutoLoad(BYTE* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_AutoLoad);

	*pVal = m_poNodeDef->AutoLoad();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_AutoLoad, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_IsRoot(BYTE* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_IsRoot);

	*pVal = m_poNodeDef->IsRoot();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_IsRoot, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_Capacity(BYTE* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_Capacity);

	*pVal = m_poNodeDef->Capacity();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_Capacity, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_Index(SHORT* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_Index);

	*pVal = m_poNodeDef->Index();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_Index, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_NumberOfRegisterItems(SHORT* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_NumberOfRegisterItems);

	*pVal = m_poNodeDef->NumberOfRegisterItems();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_NumberOfRegisterItems, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_NumberOfBlockItems(SHORT* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_NumberOfBlockItems);

	*pVal = m_poNodeDef->NumberOfBlockItems();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_NumberOfBlockItems, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_NumberOfNodeItems(SHORT* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_NumberOfNodeItems);

	*pVal = m_poNodeDef->NumberOfNodeItems();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_NumberOfNodeItems, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_NumberOfItems(SHORT* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_NumberOfItems);

	*pVal = m_poNodeDef->NumberOfItems();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_NumberOfItems, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_NumRows(LONG* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_NumRows);

	*pVal = m_poNodeDef->NumRows();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_NumRows, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_EndDateItem(BSTR* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_EndDateItem);

	gstring tmpGstring(m_poNodeDef->EndDateItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapNodeDef::get_EndDateItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapNodeDef::get_EndDateItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_StartDateItem(BSTR *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_StartDateItem);

	gstring tmpGstring(m_poNodeDef->StartDateItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapNodeDef::get_StartDateItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapNodeDef::get_StartDateItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_TiId(BSTR* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_TiId);

	gstring tmpGstring(m_poNodeDef->TiId());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapNodeDef::get_TiId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapNodeDef::get_TiId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_Id(BSTR* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_Id);

	gstring tmpGstring(m_poNodeDef->Id());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;

		LEAVE_METHOD(CWrapNodeDef::get_Id, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapNodeDef::get_Id, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_IsVisible(BYTE* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_IsVisible);

	*pVal = m_poNodeDef->IsVisible();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_IsVisible, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_Name(BSTR* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_Name);

	gstring tmpGstring(m_poNodeDef->Name());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;

		LEAVE_METHOD(CWrapNodeDef::get_Name, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapNodeDef::get_Name, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_NodeStartCorDateItem( BSTR *pVal )
{
	ENTER_METHOD(CWrapNodeDef::get_NodeStartCorDateItem);

	gstring tmpGstring(m_poNodeDef->StartCorDateItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapNodeDef::get_NodeStartCorDateItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapNodeDef::get_NodeStartCorDateItem, E_UNEXPECTED);
	}
}


STDMETHODIMP CWrapNodeDef::get_NodeEndCorDateItem( BSTR *pVal )
{
	ENTER_METHOD(CWrapNodeDef::get_NodeEndCorDateItem);

	gstring tmpGstring(m_poNodeDef->EndCorDateItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;

		LEAVE_METHOD(CWrapNodeDef::get_NodeEndCorDateItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapNodeDef::get_NodeEndCorDateItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_GetNumberOfPlugs(short* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_NumberOfPlugs);

	*pVal = m_poNodeDef->GetNumberOfPlugs();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_NumberOfPlugs, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_GetPlugFatherChanneIdByPosition(short ai_iPlugPosition, BSTR* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_GetPlugFatherChanneIdByPosition);

	gstring tmpGstring(m_poNodeDef->GetPlugFatherChanneIdByPosition(ai_iPlugPosition));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_GetPlugFatherChanneIdByPosition, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_GetPlugFatherChanneIdByPosition, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_GetPlugFatherNodeIdByPosition(short ai_iPlugPosition, BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_GetPlugFatherNodeIdByPosition);

	gstring tmpGstring(m_poNodeDef->GetPlugFatherNodeIdByPosition(ai_iPlugPosition));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_GetPlugFatherNodeIdByPosition, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_GetPlugFatherNodeIdByPosition, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_GetNumberOfGroups(short* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_NumberOfGroups);

	*pVal = m_poNodeDef->GetNumberOfGroups();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_NumberOfGroups, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_GetGroupIdByPosition(short ai_iGroupPosition, BSTR* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_GetGroupIdByPosition);

	gstring tmpGstring(m_poNodeDef->GetGroupIdByPosition(ai_iGroupPosition));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_GetGroupIdByPosition, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_GetGroupIdByPosition, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_GetGroupNumberOfItems(short ai_iGroupPosition, short* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_GetGroupNumberOfItems);

	*pVal = m_poNodeDef->GetGroupNumberOfItems(ai_iGroupPosition);

	TRACE2("Returning %u", *pVal);

	LEAVE_METHOD(CWrapNodeDef::get_GetGroupNumberOfItems, S_OK);
}

STDMETHODIMP
CWrapNodeDef::get_GetGroupItemIdByPosition(short ai_iGroupPosition, short ai_iItemPosition, BSTR* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_GetGroupItemIdByPosition);

	gstring tmpGstring(m_poNodeDef->GetGroupItemIdByPosition(ai_iGroupPosition, ai_iItemPosition));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_GetGroupItemIdByPosition, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_GetGroupItemIdByPosition, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_GetNodeVirtualFlagItem(BSTR* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_GetNodeVirtualFlagItem);

	gstring tmpGstring(m_poNodeDef->VirtualFlagItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_GetNodeVirtualFlagItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_GetNodeVirtualFlagItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_FilterStartDateItem(BSTR* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_FilterStartDateItem);

	gstring tmpGstring(m_poNodeDef->FilterStartDateItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_FilterStartDateItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_FilterStartDateItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_FilterEndDateItem(BSTR* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_FilterEndDateItem);

	gstring tmpGstring(m_poNodeDef->FilterEndDateItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_FilterEndDateItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_FilterEndDateItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_FilterStartCorDateItem(BSTR* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_FilterStartCorDateItem);

	gstring tmpGstring(m_poNodeDef->FilterStartCorDateItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_FilterStartCorDateItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_FilterStartCorDateItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_FilterEndCorDateItem(BSTR* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_FilterEndCorDateItem);

	gstring tmpGstring(m_poNodeDef->FilterEndCorDateItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_FilterEndCorDateItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_FilterEndCorDateItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_CurrencyTypeItem(BSTR* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_CurrencyTypeItem);

	gstring tmpGstring(m_poNodeDef->CurrencyTypeItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_CurrencyTypeItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_CurrencyTypeItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_CurrencyExchDateItem(BSTR* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_CurrencyExchDateItem);

	gstring tmpGstring(m_poNodeDef->CurrencyExchDateItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_CurrencyExchDateItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_CurrencyExchDateItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_ChangeReasonItem(BSTR* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_ChangeReasonItem);

	gstring tmpGstring(m_poNodeDef->ChangeReasonItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_ChangeReasonItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_ChangeReasonItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_DmdComponentItem(BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_DmdComponentItem);

	gstring tmpGstring(m_poNodeDef->DmdComponentItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_DmdComponentItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_DmdComponentItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_DmdValueItem(BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_DmdValueItem);

	gstring tmpGstring(m_poNodeDef->DmdValueItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_DmdValueItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_DmdValueItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_LoadSQLItem(BSTR* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_LoadSQLItem);

	gstring tmpGstring(m_poNodeDef->LoadSQLItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_LoadSQLItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_LoadSQLItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_DynFilter(BYTE* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_DynFilter);

	*pVal = m_poNodeDef->DynFilter();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_DynFilter, S_OK);
}

STDMETHODIMP
CWrapNodeDef::get_GetPlugTypeByPosition(short ai_iPosition, BYTE* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_GetPlugTypeByPosition);

	*pVal = m_poNodeDef->GetPlugTypeByPosition(ai_iPosition);

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_GetPlugTypeByPosition, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_GetPlugIsFilterByPosition(short ai_iPosition, BYTE* pVal )
{
	ENTER_METHOD(CWrapNodeDef::get_GetPlugIsFilterByPosition);

	*pVal = m_poNodeDef->GetPlugIsFilterByPosition(ai_iPosition);

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_GetPlugIsFilterByPosition, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_GetNodeReadObjectId(BSTR* ao_stObjectId)
{
	ENTER_METHOD(CWrapNodeDef::GetNodeReadObjectId);

	gstring tmpGstring(m_poNodeDef->GetNodeReadObjectId());

	if (tmpGstring)
	{
		*ao_stObjectId = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::GetNodeReadObjectId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::GetNodeReadObjectId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_GetNodeWriteObjectId(BSTR* ao_stObjectId)
{
	ENTER_METHOD(CWrapNodeDef::GetNodeWriteObjectId);

	gstring tmpGstring(m_poNodeDef->GetNodeWriteObjectId());

	if (tmpGstring)
	{
		*ao_stObjectId = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::GetNodeWriteObjectId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::GetNodeWriteObjectId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_GetNodeReadSentenceId(long* ao_Sentence)
{
	ENTER_METHOD(CWrapNodeDef::GetNodeReadSentenceId);

	*ao_Sentence = atoi( m_poNodeDef->GetNodeReadSentenceId() ) ;

	TRACE2("Returning %u", *ao_Sentence);
	LEAVE_METHOD(CWrapNodeDef::GetNodeReadSentenceId, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_GetNodeWriteSentenceId(long* ao_Sentence)
{
	ENTER_METHOD(CWrapNodeDef::GetNodeWriteSentenceId);

	*ao_Sentence = atoi( m_poNodeDef->GetNodeWriteSentenceId() ) ;

	TRACE2("Returning %u", *ao_Sentence);
	LEAVE_METHOD(CWrapNodeDef::GetNodeWriteSentenceId, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_NumberOfConnectors(short* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_NumberOfConnectors);

	*pVal = m_poNodeDef->NumberOfConnectors();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_NumberOfConnectors, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_NumberOfRBConnectors(short* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_NumberOfRBConnectors);

	*pVal = m_poNodeDef->NumberOfRBConnectors();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_NumberOfRBConnectors, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_NumberOfBBConnectors(short* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_NumberOfBBConnectors);

	*pVal = m_poNodeDef->NumberOfBBConnectors();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_NumberOfBBConnectors, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_NumberOfNRConnectors(short* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_NumberOfNRConnectors);

	*pVal = m_poNodeDef->NumberOfNRConnectors();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_NumberOfNRConnectors, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_FirstRBConnector(short* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_FirstRBConnector);

	*pVal = m_poNodeDef->RBFirstConnector();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_FirstRBConnector, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_FirstBBConnector(short* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_FirstBBConnector);

	*pVal = m_poNodeDef->BBFirstConnector();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_FirstBBConnector, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_FirstNRConnector(short* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_FirstNRConnector);

	*pVal = m_poNodeDef->NRFirstConnector();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_FirstNRConnector, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_GetConnectorHandleByPosition(short ai_iPos, long * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_GetConnectorHandleByPosition);

	*pVal = m_poNodeDef->GetConnectorHandleByPosition(ai_iPos);

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_GetConnectorHandleByPosition, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_GetNodeConnectorFatherNodeHandle(long ai_NodeHandle, long* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_GetNodeConnectorFatherNodeHandle);

	*pVal = m_poNodeDef->GetNodeConnectorFatherNodeHandle(ai_NodeHandle);

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_GetNodeConnectorFatherNodeHandle, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_GetNodeConnectorSonNodeHandle(long ai_ConnectorHandle, long* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_GetNodeConnectorSonNodeHandle);

	*pVal = m_poNodeDef->GetNodeConnectorSonNodeHandle(ai_ConnectorHandle);

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_GetNodeConnectorSonNodeHandle, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_PriorityItem(BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_PriorityItem);

	gstring tmpGstring(m_poNodeDef->PriorityItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_PriorityItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_PriorityItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_ImputeDateItem(BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_ImputeDateItem);

	gstring tmpGstring(m_poNodeDef->ImputeDateItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_ImputeDateItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_ImputeDateItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_PayDateItem(BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_PayDateItem);

	gstring tmpGstring(m_poNodeDef->PayDateItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_PayDateItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_PayDateItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_PayTypeItem(BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_PayTypeItem);

	gstring tmpGstring(m_poNodeDef->PayTypeItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_PayTypeItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_PayTypeItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_PayFrequencyItem(BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_PayFrequencyItem);

	gstring tmpGstring(m_poNodeDef->PayFrequencyItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_PayFrequencyItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_PayFrequencyItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_DefaultIndex(long * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_DefaultIndex);

	*pVal = m_poNodeDef->DefaultIndex();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_DefaultIndex, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_IdAutoFilter(short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_IdAutoFilter);

	*pVal = m_poNodeDef->IdAutoFilter();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_IdAutoFilter, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_NumberOfIndexes(short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_NumberOfIndexes);

	*pVal = m_poNodeDef->NumberOfIndexes();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_NumberOfIndexes, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_IndexId(short ai_iIndexPosition, long * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_IndexId);

	*pVal = m_poNodeDef->IndexId( ai_iIndexPosition );

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_IndexId, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_IndexNumberOfItems(short ai_iIndexPosition, short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_IndexNumberOfItems);

	*pVal = m_poNodeDef->IndexNumberOfItems( ai_iIndexPosition );

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_IndexNumberOfItems, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_IndexItemId(short ai_iIndexPosition, short ai_iItemPosition, BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_IndexItemId);

	gstring tmpGstring(m_poNodeDef->IndexItemId( ai_iIndexPosition, ai_iItemPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_IndexItemId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_IndexItemId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_IndexItemFunction(short ai_iIndexPosition, short ai_iItemPosition, short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_IndexItemFunction);

	*pVal = m_poNodeDef->IndexItemFunction( ai_iIndexPosition, ai_iItemPosition );

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_IndexItemFunction, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_IndexItemWay(short ai_iIndexPosition, short ai_iItemPosition, short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_IndexItemWay);

	*pVal = m_poNodeDef->IndexItemWay( ai_iIndexPosition, ai_iItemPosition );

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_IndexItemWay, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_NumberOfAlias(short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_NumberOfAlias);

	*pVal = m_poNodeDef->NumberOfAlias();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_NumberOfAlias, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_AliasId(short ai_iAliasPosition, BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_AliasId);

	gstring tmpGstring(m_poNodeDef->AliasId( ai_iAliasPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_AliasId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_AliasId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_AliasNodeId(short ai_iAliasPosition, BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_AliasNodeId);

	gstring tmpGstring(m_poNodeDef->AliasNodeId( ai_iAliasPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_AliasNodeId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_AliasNodeId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_NumberOfT3Alias(short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_NumberOfT3Alias);

	*pVal = m_poNodeDef->NumberOfT3Alias();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_NumberOfT3Alias, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_T3AliasId(short ai_iT3AliasPosition, BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_T3AliasId);

	gstring tmpGstring(m_poNodeDef->T3AliasId( ai_iT3AliasPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_T3AliasId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_T3AliasId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_T3AliasInstance(short ai_iT3AliasPosition, BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_T3AliasInstance);

	gstring tmpGstring(m_poNodeDef->T3AliasInstance( ai_iT3AliasPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_T3AliasInstance, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_T3AliasInstance, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_GetNodeReadSentenceString(BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_GetNodeReadSentenceString);

	gstring tmpGstring(m_poNodeDef->GetNodeReadSentenceId());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_GetNodeReadSentenceString, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_GetNodeReadSentenceString, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_GetNodeWriteSentenceString(BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_GetNodeWriteSentenceString);

	gstring tmpGstring(m_poNodeDef->GetNodeWriteSentenceId());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_GetNodeWriteSentenceString, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_GetNodeWriteSentenceString, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_NumberOfDmds(short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_NumberOfDmds);

	*pVal = m_poNodeDef->NumberOfDmds();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_NumberOfDmds, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_DmdId(short ai_iDmdPosition, BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_DmdId);

	gstring tmpGstring(m_poNodeDef->DmdId( ai_iDmdPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_DmdId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_DmdId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_DmdComponentId(short ai_iDmdPosition, BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_DmdComponentId);

	gstring tmpGstring(m_poNodeDef->DmdComponentId( ai_iDmdPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_DmdComponentId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_DmdComponentId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_DmdFieldId(short ai_iDmdPosition, BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_DmdFieldId);

	gstring tmpGstring(m_poNodeDef->DmdFieldId( ai_iDmdPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_DmdFieldId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_DmdFieldId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_CsType(short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_CsType);

	*pVal = m_poNodeDef->CsType();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_CsType, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_NumKeepRows(long * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_NumKeepRows);

	*pVal = m_poNodeDef->NumKeepRows();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_NumKeepRows, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_NumberOfFilters(short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_NumberOfFilters);

	*pVal = m_poNodeDef->NumberOfQBFFilters();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_NumberOfFilters, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_FilterId(short ai_iFilterPosition, BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_FilterId);

	gstring tmpGstring(m_poNodeDef->QBFFilterId( ai_iFilterPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_FilterId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_FilterId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_FilterT3Id(short ai_iFilterPosition, BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_FilterT3Id);

	gstring tmpGstring(m_poNodeDef->QBFFilterT3Id( ai_iFilterPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_FilterT3Id, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_FilterT3Id, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_FilterNodeId(short ai_iFilterPosition, BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_FilterNodeId);

	gstring tmpGstring(m_poNodeDef->QBFFilterNodeId( ai_iFilterPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_FilterNodeId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_FilterNodeId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_SyncType(short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_SyncType);

	*pVal = m_poNodeDef->SyncType();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_SyncType, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_SecRead(short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_SecRead);

	*pVal = m_poNodeDef->SecRead();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_SecRead, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_SecWrite(short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_SecWrite);

	*pVal = m_poNodeDef->SecWrite();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_SecWrite, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_SecDelete(short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_SecDelete);

	*pVal = m_poNodeDef->SecDelete();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_SecDelete, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_SecUpdate(short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_SecUpdate);

	*pVal = m_poNodeDef->SecUpdate();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_SecUpdate, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_SecExecute(short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_SecExecute);

	*pVal = m_poNodeDef->SecExecute();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_SecExecute, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_FilterIsApplicable(short ai_iFilterPosition, short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_FilterIsApplicable);

	*pVal = m_poNodeDef->QBFFilterIsApplicable( ai_iFilterPosition );

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_FilterIsApplicable, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_PlugIsApplicable(short ai_iPlugPosition, short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_PlugIsApplicable);

	*pVal = m_poNodeDef->PlugIsApplicable( ai_iPlugPosition );

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_PlugIsApplicable, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_ClosingDate(DATE * pVal)
{
	m4VariantType tmpVar;

	ENTER_METHOD(CWrapAccess::get_ClosingDate);

	tmpVar.Type = M4CL_CPP_TYPE_DATE ;
	tmpVar.Data.DoubleData = ClActualDate() ;
	
	*pVal = M4dateToDate(tmpVar);

	TRACE3("Converting m4date %g to date %g", tmpVar, *pVal);
	LEAVE_METHOD(CWrapAccess::get_ClosingDate, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_IsExternal(BYTE *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_IsExternal);

	*pVal = m_poNodeDef->IsExternal();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_IsExternal, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_OleInterfaceGuid(BSTR *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_OleInterfaceGuid);

	gstring tmpGstring(m_poNodeDef->OleNodeInterId());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_OleInterfaceGuid, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_OleInterfaceGuid, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_OleDispId(long *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_OleDispId);

	*pVal = m_poNodeDef->OleDispId();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_OleDispId, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_ClosingDateItem(BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_ClosingDateItem);

	gstring tmpGstring(m_poNodeDef->ClosingDateItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_ClosingDateItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_ClosingDateItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_SysParamsItem(BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_SysParamsItem);

	gstring tmpGstring(m_poNodeDef->SysParamsItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_SysParamsItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_SysParamsItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_ImputePayTypeItem(BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_ImputePayTypeItem);

	gstring tmpGstring(m_poNodeDef->ImputePayTypeItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_ImputePayTypeItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_ImputePayTypeItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_ImputePayFrequencyItem(BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_ImputePayFrequencyItem);

	gstring tmpGstring(m_poNodeDef->ImputePayFrequencyItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_ImputePayFrequencyItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_ImputePayFrequencyItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_NumRowsDB(long * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_NumRowsDB);

	*pVal = m_poNodeDef->NumRowsDB();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_NumRowsDB, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_NumberOfSysSentences(short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_NumberOfSysSentences);

	*pVal = m_poNodeDef->NumberOfSysSentences();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_NumberOfSysSentences, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_SysSentenceNode(short ai_iPosition, BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_SysSentenceNode);

	gstring tmpGstring(m_poNodeDef->SysSentenceNode(ai_iPosition));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_SysSentenceNode, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_SysSentenceNode, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_SysSentenceItem(short ai_iPosition, BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_SysSentenceItem);

	gstring tmpGstring(m_poNodeDef->SysSentenceItem(ai_iPosition));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_SysSentenceItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_SysSentenceItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_PlugNumberOfItems(short ai_iPlugPosition, short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_PlugNumberOfItems);

	*pVal = m_poNodeDef->PlugNumberOfItems( ai_iPlugPosition );

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_PlugNumberOfItems, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_PlugFatherItem(short ai_iPlugPosition, short ai_iItemPosition, BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_PlugFatherItem);

	gstring tmpGstring(m_poNodeDef->PlugFatherItem(ai_iPlugPosition, ai_iItemPosition));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_PlugFatherItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_PlugFatherItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_PlugSonItem(short ai_iPlugPosition, short ai_iItemPosition, BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_PlugSonItem);

	gstring tmpGstring(m_poNodeDef->PlugSonItem(ai_iPlugPosition, ai_iItemPosition));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_PlugSonItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_PlugSonItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_PlugItemRelation(short ai_iPlugPosition, short ai_iItemPosition, short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_PlugItemRelation);

	*pVal = m_poNodeDef->PlugItemRelation( ai_iPlugPosition, ai_iItemPosition );

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_PlugItemRelation, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_SecDynFilter(short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_SecDynFilter);

	*pVal = m_poNodeDef->SecDynFilter();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_SecDynFilter, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_SecFilter(BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_SecFilter);

	gstring tmpGstring(m_poNodeDef->SecFilter());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_SecFilter, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_SecFilter, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_PlugRelationTypeId(short ai_iPosition, short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_PlugRelationTypeId);

	*pVal = m_poNodeDef->PlugRelationTypeId(ai_iPosition);

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_PlugRelationTypeId, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_ReadSentenceApiSql(BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_ReadSentenceApiSql);

	gstring tmpGstring(m_poNodeDef->ReadSentenceApiSql());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_ReadSentenceApiSql, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_ReadSentenceApiSql, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_NodeType(short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_NodeType);

	*pVal = m_poNodeDef->NodeType();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_NodeType, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_IsOrdered(short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_IsOrdered);

	*pVal = m_poNodeDef->IsOrdered();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_IsOrdered, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_AffectsDB(short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_AffectsDB);

	*pVal = m_poNodeDef->AffectsDB();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_AffectsDB, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_Order(short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_Order);

	*pVal = m_poNodeDef->NodeOrder();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_Order, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_DBReload(short * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_DBReload);

	*pVal = m_poNodeDef->DBReload();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_DBReload, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_MaxNumBlocks(long * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_MaxNumBlocks);

	*pVal = m_poNodeDef->MaxNumBlocks();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_MaxNumBlocks, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_MaxNumRecords(long * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_MaxNumRecords);

	*pVal = m_poNodeDef->MaxNumRecords();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_MaxNumRecords, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_GroupObjects(BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_GroupObjects);

	gstring tmpGstring(m_poNodeDef->GroupObjects());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapNodeDef::get_GroupObjects, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapNodeDef::get_GroupObjects, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_SentenceGroupObjects(BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_SentenceGroupObjects);

	gstring tmpGstring(m_poNodeDef->SentenceGroupObjects());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapNodeDef::get_SentenceGroupObjects, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapNodeDef::get_SentenceGroupObjects, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_BDLChecksItem(BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_BDLChecksItem);

	gstring tmpGstring(m_poNodeDef->BDLChecksItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_BDLChecksItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_BDLChecksItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_RAMOrderByItem(BSTR * pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_RAMOrderByItem);

	gstring tmpGstring(m_poNodeDef->RAMOrderByItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_RAMOrderByItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_RAMOrderByItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_OrganizationIdItem(BSTR *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_OrganizationIdItem);

	gstring tmpGstring(m_poNodeDef->OrganizationIdItem());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_OrganizationIdItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_OrganizationIdItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_M4ObjId(BSTR *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_M4ObjId);

	gstring tmpGstring(m_poNodeDef->M4ObjId());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_M4ObjId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_M4ObjId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_TiInheritLevel(short *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_TiInheritLevel);

	*pVal = m_poNodeDef->TiInheritLevel();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_TiInheritLevel, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_GetPlugIsMultiselectionByPosition(short ai_iPosition, BYTE *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_GetPlugIsMultiselectionByPosition);

	*pVal = m_poNodeDef->GetPlugIsMultiselectionByPosition(ai_iPosition);

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_GetPlugIsMultiselectionByPosition, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_FilterSysSentenceItem(short ai_iFilterPosition, BSTR *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_FilterSysSentenceItem);

	gstring tmpGstring(m_poNodeDef->QBFFilterSysSentenceItem( ai_iFilterPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_FilterSysSentenceItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_FilterSysSentenceItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_FilterSysParamItem(short ai_iFilterPosition, BSTR *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_FilterSysParamItem);

	gstring tmpGstring(m_poNodeDef->QBFFilterSysParamItem( ai_iFilterPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_FilterSysParamItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_FilterSysParamItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_FilterIsMultiselection(short ai_iFilterPosition, BYTE *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_FilterIsMultiselection);

	*pVal = m_poNodeDef->QBFFilterBehaviourType(ai_iFilterPosition);

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_FilterIsMultiselection, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_NumberOfFindFilters(short *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_NumberOfFindFilters);

	*pVal = m_poNodeDef->NumberOfFindFilters();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_NumberOfFindFilters, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_FindFilterId(short ai_iFilterPosition, BSTR *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_FindFilterId);

	gstring tmpGstring(m_poNodeDef->FindFilterId( ai_iFilterPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_FindFilterId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_FindFilterId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_FindFilterT3Id(short ai_iFilterPosition, BSTR *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_FindFilterT3Id);

	gstring tmpGstring(m_poNodeDef->FindFilterT3Id( ai_iFilterPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_FindFilterT3Id, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_FindFilterT3Id, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_FindFilterNodeId(short ai_iFilterPosition, BSTR *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_FindFilterNodeId);

	gstring tmpGstring(m_poNodeDef->FindFilterNodeId( ai_iFilterPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_FindFilterNodeId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_FindFilterNodeId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_FindFilterSysSentenceItem(short ai_iFilterPosition, BSTR *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_FindFilterSysSentenceItem);

	gstring tmpGstring(m_poNodeDef->FindFilterSysSentenceItem( ai_iFilterPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_FindFilterSysSentenceItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_FindFilterSysSentenceItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_FindFilterSysParamItem(short ai_iFilterPosition, BSTR *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_FindFilterSysParamItem);

	gstring tmpGstring(m_poNodeDef->FindFilterSysParamItem( ai_iFilterPosition ));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapItemDef::get_FindFilterSysParamItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapItemDef::get_FindFilterSysParamItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapNodeDef::get_FindFilterIsMultiselection(short ai_iFilterPosition, BYTE *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_FindFilterIsMultiselection);

	*pVal = m_poNodeDef->FindFilterBehaviourType(ai_iFilterPosition);

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_FindFilterIsMultiselection, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_FindFilterIsApplicable(short ai_iFilterPosition, BYTE *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_FindFilterIsApplicable);

	*pVal = m_poNodeDef->FindFilterIsApplicable( ai_iFilterPosition );

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_FindFilterIsApplicable, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_FilterIsOwnNode(short ai_iFilterPosition, BYTE *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_FilterIsOwnNode);

	*pVal = m_poNodeDef->QBFFilterIsOwnNode(ai_iFilterPosition);

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_FilterIsOwnNode, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_FindFilterIsOwnNode(short ai_iFilterPosition, BYTE *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_FindFilterIsOwnNode);

	*pVal = m_poNodeDef->FindFilterIsOwnNode(ai_iFilterPosition);

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_FindFilterIsOwnNode, S_OK);
}

STDMETHODIMP CWrapNodeDef::get_IsOverWritten(short *pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_IsOverWritten);

	*pVal = m_poNodeDef->IsOverWritten();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::get_IsOverWritten, S_OK);
}
