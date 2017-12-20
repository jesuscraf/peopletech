//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapnodedef.h
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//    Declaration of the CWrapNodeDef
//
//==============================================================================
#include "resource.h"       // main symbols

#ifndef __WRAPNODEDEF_H_
#define __WRAPNODEDEF_H_

class ClNodeDef;

/////////////////////////////////////////////////////////////////////////////
// CWrapNodeDef
class ATL_NO_VTABLE CWrapNodeDef : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapNodeDef, &CLSID_WrapNodeDef>,
	public IDispatchImpl<IWrapNodeDef, &IID_IWrapNodeDef, &LIBID_CHANNELWRAPPERLib>
{
public:
	CWrapNodeDef();
	void Init(ClNodeDef* ai_NodeDef);

DECLARE_REGISTRY_RESOURCEID(IDR_WRAPNODEDEF)

BEGIN_COM_MAP(CWrapNodeDef)
	COM_INTERFACE_ENTRY(IWrapNodeDef)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IWrapNodeDef
public:
	STDMETHOD(get_IsOverWritten)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_FindFilterIsOwnNode)(short ai_iFilterPosition, /*[out, retval]*/ BYTE *pVal);
	STDMETHOD(get_FilterIsOwnNode)(short ai_iFilterPosition, /*[out, retval]*/ BYTE *pVal);
	STDMETHOD(get_FindFilterIsApplicable)(short ai_iFilterPosition, /*[out, retval]*/ BYTE *pVal);
	STDMETHOD(get_FindFilterIsMultiselection)(short ai_iFilterPosition, /*[out, retval]*/ BYTE *pVal);
	STDMETHOD(get_FindFilterSysParamItem)(short ai_iFilterPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_FindFilterSysSentenceItem)(short ai_iFilterPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_FindFilterNodeId)(short ai_iFilterPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_FindFilterT3Id)(short ai_iFilterPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_FindFilterId)(short ai_iFilterPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_NumberOfFindFilters)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_FilterIsMultiselection)(short ai_iFilterPosition, /*[out, retval]*/ BYTE *pVal);
	STDMETHOD(get_FilterSysParamItem)(short ai_iFilterPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_FilterSysSentenceItem)(short ai_iFilterPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_GetPlugIsMultiselectionByPosition)(short ai_iPosition, /*[out, retval]*/ BYTE *pVal);
	STDMETHOD(get_TiInheritLevel)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_M4ObjId)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_OrganizationIdItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_RAMOrderByItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_BDLChecksItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_SentenceGroupObjects)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_GroupObjects)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_MaxNumRecords)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_MaxNumBlocks)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_DBReload)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_Order)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_AffectsDB)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_IsOrdered)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_NodeType)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_ReadSentenceApiSql)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_PlugRelationTypeId)(short ai_iPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_SecFilter)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_SecDynFilter)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_PlugItemRelation)(short ai_iPlugPosition, short ai_iItemPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_PlugSonItem)(short ai_iPlugPosition, short ai_iItemPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_PlugFatherItem)(short ai_iPlugPosition, short ai_iItemPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_PlugNumberOfItems)(short ai_iPlugPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_SysSentenceItem)(short ai_iPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_SysSentenceNode)(short ai_iPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_NumberOfSysSentences)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_NumRowsDB)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_ImputePayFrequencyItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ImputePayTypeItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_SysParamsItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ClosingDateItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ClosingDate)(/*[out, retval]*/ DATE *pVal);
	STDMETHOD(get_PlugIsApplicable)(short ai_iPlugPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_FilterIsApplicable)(short ai_iFilterPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_SecExecute)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_SecUpdate)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_SecDelete)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_SecWrite)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_SecRead)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_SyncType)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_FilterNodeId)(short ai_iFilterPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_FilterT3Id)(short ai_iFilterPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_FilterId)(short ai_iFilterPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_NumberOfFilters)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_NumKeepRows)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_CsType)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_DmdFieldId)(short ai_iDmdPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_DmdComponentId)(short ai_iDmdPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_DmdId)(short ai_iDmdPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_NumberOfDmds)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_GetNodeWriteSentenceString)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_GetNodeReadSentenceString)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_T3AliasInstance)(short ai_iT3AliasPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_T3AliasId)(short ai_iT3AliasPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_NumberOfT3Alias)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_AliasNodeId)(short ai_iAliasPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_AliasId)(short ai_iAliasPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_NumberOfAlias)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_IndexItemWay)(short ai_iIndexPosition, short ai_iItemPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_IndexItemFunction)(short ai_iIndexPosition, short ai_iItemPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_IndexItemId)(short ai_iIndexPosition, short ai_iItemPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_IndexNumberOfItems)(short ai_iIndexPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_IndexId)(short ai_iIndexPosition, /*[out, retval]*/ long *pVal);
	STDMETHOD(get_NumberOfIndexes)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_IdAutoFilter)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_DefaultIndex)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_PayFrequencyItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_PayTypeItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_PayDateItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ImputeDateItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_PriorityItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_GetNodeConnectorSonNodeHandle)(/*[in]*/ long ai_ConnectorHandle, /*[out, retval]*/ long *pVal);
	STDMETHOD(get_GetNodeConnectorFatherNodeHandle)(/*[in]*/ long ai_NodeHandle, /*[out, retval]*/ long *pVal);
	STDMETHOD(get_GetConnectorHandleByPosition)(/*[in]*/ short ai_iPos, /*[out, retval]*/ long *pVal);
	STDMETHOD(get_FirstNRConnector)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_FirstBBConnector)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_FirstRBConnector)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_NumberOfNRConnectors)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_NumberOfBBConnectors)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_NumberOfRBConnectors)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_NumberOfConnectors)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_GetNodeWriteSentenceId)(/*[out, retval]*/ long* ao_Sentence);
	STDMETHOD(get_GetNodeReadSentenceId)(/*[out, retval]*/ long* ao_Sentence);
	STDMETHOD(get_GetNodeWriteObjectId)(/*[out, retval]*/ BSTR* ao_stObjectId);
	STDMETHOD(get_GetNodeReadObjectId)(/*[out, retval]*/ BSTR* ao_stObjectId);
	STDMETHOD(get_GetPlugIsFilterByPosition)(/*[in]*/ short ai_iPosition, /*[out, retval]*/ BYTE *pVal);
	STDMETHOD(get_GetPlugTypeByPosition)(/*[in]*/ short ai_IPosition, /*[out, retval]*/ BYTE *pVal);
	STDMETHOD(get_DynFilter)(/*[out, retval]*/ BYTE *pVal);
	STDMETHOD(get_LoadSQLItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_DmdValueItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_DmdComponentItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ChangeReasonItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_CurrencyExchDateItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_CurrencyTypeItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_FilterEndCorDateItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_FilterStartCorDateItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_FilterEndDateItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_FilterStartDateItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_GetNodeVirtualFlagItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_GetGroupItemIdByPosition)(/*[in]*/ short ai_iGroupPosition, /*[in]*/ short ai_iItemPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_GetGroupNumberOfItems)(/*[in]*/ short ai_iGroupPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_GetGroupIdByPosition)(/*[in]*/ short ai_iGroupPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_GetNumberOfGroups)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_Id)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_TiId)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_StartDateItem)(/*[out, retval]*/ BSTR* pVal);
	STDMETHOD(get_EndDateItem)(/*[out, retval]*/ BSTR* pVal);
	STDMETHOD(get_NumRows)(/*[out, retval]*/ LONG *pVal);
	STDMETHOD(get_NumberOfItems)(/*[out, retval]*/ SHORT *pVal);
	STDMETHOD(get_NumberOfNodeItems)(/*[out, retval]*/ SHORT *pVal);
	STDMETHOD(get_NumberOfBlockItems)(/*[out, retval]*/ SHORT *pVal);
	STDMETHOD(get_NumberOfRegisterItems)(/*[out, retval]*/ SHORT *pVal);
	STDMETHOD(get_Index)(/*[out, retval]*/ SHORT *pVal);
	STDMETHOD(get_Capacity)(/*[out, retval]*/ BYTE *pVal);
	STDMETHOD(get_IsRoot)(/*[out, retval]*/ BYTE *pVal);
	STDMETHOD(get_AutoLoad)(/*[out, retval]*/ BYTE *pVal);
	STDMETHOD(get_UniqueRow)(/*[out, retval]*/ BYTE *pVal);
	STDMETHOD(get_Temporality)(/*[out, retval]*/ BYTE *pVal);
	STDMETHOD(get_Completeness)(/*[out, retval]*/ BYTE *pVal);
	STDMETHOD(get_PeriodCorrected)(/*[out, retval]*/ BYTE *pVal);
	STDMETHOD(get_NodeStartCorDateItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_NodeEndCorDateItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_GetPlugFatherNodeIdByPosition)(/*[in]*/ short ai_iPlugPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_GetPlugFatherChanneIdByPosition)(/*[in]*/ short ai_iPlugPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_GetNumberOfPlugs)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_IsVisible)(/*[out, retval]*/ BYTE *pVal);
	STDMETHOD(get_IsExternal)(/* [retval][out] */ BYTE *pVal);
	STDMETHOD(get_OleInterfaceGuid)(/* [retval][out] */ BSTR *pVal);
	STDMETHOD(get_OleDispId)(/* [retval][out] */ long *pVal);

private:
	ClNodeDef* m_poNodeDef;
};

#endif //__WRAPNODEDEF_H_
