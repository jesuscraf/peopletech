//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapitemdef.h
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//    Declaration of the CWrapItemDef
//
//==============================================================================

#include "resource.h"       // main symbols
#include "m4types.hpp"
#include "index.hpp"

#ifndef __WRAPITEMDEF_H_
#define __WRAPITEMDEF_H_

class ClItem;

/////////////////////////////////////////////////////////////////////////////
// CWrapItemDef
class ATL_NO_VTABLE CWrapItemDef : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWrapItemDef, &CLSID_WrapItemDef>,
	public ISupportErrorInfo,
	public IDispatchImpl<IWrapItemDef, &IID_IWrapItemDef, &LIBID_CHANNELWRAPPERLib>
{
public:
	CWrapItemDef();
	~CWrapItemDef();

	void Init(ClItem* ai_VarRef);

DECLARE_REGISTRY_RESOURCEID(IDR_WRAPITEMDEF)

BEGIN_COM_MAP(CWrapItemDef)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IWrapItemDef)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IWrapItemDef
public:
	STDMETHOD(get_NumberOfConnectedItems)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_ConnectorCsTypeByPosition)(short ai_iPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_ConnectorContextByPosition)(short ai_iPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_ConnectorRelationByPosition)(short ai_iPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_ConnectorSpinByPosition)(short ai_iPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_ConnectorPrecedenceByPosition)(short ai_iPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_ConnectorSonItemByPosition)(short ai_iPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ConnectorSonNodeByPosition)(short ai_iPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ConnectorFatherItemByPosition)(short ai_iPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ConnectorFatherNodeByPosition)(short ai_iPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_SecExecute)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_SecUpdate)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_SecDelete)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_SecWrite)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_SecRead)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_SyncType)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_CsType)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_Synonym)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_DmdCrossTab)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_FormId)(short ai_iFormIdPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_NumberOfFormIds)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_ExecutedNodeId)(short ai_ExecutedPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ExecutedId)(short ai_ExecutedPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ExecutedNodeIndex)(short ai_ExecutedPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_ExecutedIndex)(short ai_ExecutedPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_NumberOfExecuted)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_AssignedNodeId)(short ai_AssignedPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_AssignedId)(short ai_AssignedPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_AssignedNodeIndex)(short ai_AssignedPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_AssignedIndex)(short ai_AssignedPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_NumberOfAssigned)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_IsInherited)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_DefaultValue)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(get_FatherItemNode)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_FatherItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_AuxiliarItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_AuxiliarItemProrat)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_SliceItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_InternalType)(/*[out, retval]*/ BYTE *pVal);
	STDMETHOD(get_VariableArguments)(/*[out, retval]*/ BYTE *pVal);
	STDMETHOD(get_WriteObjectId)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ReadObjectId)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ConditionantNodeIdByPosition)(/*[in]*/ short ai_pos, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ConditionantNodeIndexByPosition)(/*[in]*/ short ai_pos, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_ConditionantIdByPosition)(/*[in]*/ short ai_pos, /*[out, retval]*/ BSTR* pVal);
	STDMETHOD(get_ConditionantIndexByPosition)(/*[in]*/ short ai_pos, /*[out, retval]*/ short* pVal);
	STDMETHOD(get_NumberOfConditionants)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_ArgumentM4TypeByPosition)(/*[in]*/ short ai_pos, /*[out, retval]*/ short* pVal);
	STDMETHOD(get_ModifyData)(/*[out, retval]*/ BYTE *pVal);
	STDMETHOD(get_IsVisible)(/*[out, retval]*/ BYTE *pVal);
	STDMETHOD(get_Order)(/*[out, retval]*/ SHORT *pVal);
	STDMETHOD(get_Scope)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_SentTotType)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_IsVariableLength)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_MethodLevel)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_TransactionMode)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_IdTotalize)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_DirtyCheck)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_SliceSplit)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_SliceBhrv)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_Scale)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_CppType)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_M4Type)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_TheoricScope)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_Type)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_Index)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_WriteFieldId)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ReadFieldId)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_WriteObjectAlias)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ReadObjectAlias)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_MaxSize)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_Precision)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_DmdField)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_DmdComponent)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Dmd)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Id)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_NumberOfArguments)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_NumberOfReferedArguments)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_ArgumentTypeByPosition)(/*[in]*/short ai_pos,/*[out, retval]*/ short *pVal);
	STDMETHOD(get_ArgumentIdByPosition)(/*[in]*/short ai_pos,/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_IsPublic)(/* [retval][out] */ BYTE *pVal);

// ISupportsErrorInfo
public:
	STDMETHOD(get_IsConnected)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(get_ExecutedDependentNodeId)(short ai_ExecutedDependentPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ExecutedDependentId)(short ai_ExecutedDependentPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ExecutedDependentNodeIndex)(short ai_ExecutedDependentPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_ExecutedDependentIndex)(short ai_ExecutedDependentPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_NumberOfExecutedDependents)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_AssignedDependentNodeId)(short ai_AssignedDependentPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_AssignedDependentId)(short ai_AssignedDependentPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_AssignedDependentNodeIndex)(short ai_AssignedDependentPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_AssignedDependentIndex)(short ai_AssignedDependentPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_NumberOfAssignedDependents)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_NotNull)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_CodeSource)(short ai_iPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_CodeAutoGenerated)(short ai_iPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_CodePriority)(short ai_iPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_CodeMustAssign)(short ai_iPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_CodeType)(short ai_iPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_CodeSize)(short ai_iPosition, /*[out, retval]*/ long *pVal);
	STDMETHOD(get_CodeNumberOfInstructions)(short ai_iPosition, /*[out, retval]*/ long *pVal);
	STDMETHOD(get_CodeNumberOfVariales)(short ai_iPosition, /*[out, retval]*/ long *pVal);
	STDMETHOD(get_CodeRuleTiId)(short ai_iPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_CodeEndDate)(short ai_iPosition, /*[out, retval]*/ DATE *pVal);
	STDMETHOD(get_CodeStartDate)(short ai_iPosition, /*[out, retval]*/ DATE *pVal);
	STDMETHOD(get_CodeRuleId)(short ai_iPosition, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_GetNextCode)(short ai_iNext, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_MaxCodes)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_TiId)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_SecContinue)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_SliceTotalize)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_ArgumentNameByPosition)(SHORT ai_pos, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_GenerateSlices)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_ArgumentPositionById)(BSTR ai_pccArgumentId, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_ArgumentPrecisionByPosition)(short ai_iPosition, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_ConvertFunction)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_IsPK)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_AffectsDB)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_IsEventItem)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_IsNodalItem)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_ClientNotNull)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_CheckNotNull)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_DependentNodeIdByPosition)(short ai_pos, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_DependentNodeIndexByPosition)(short ai_pos, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_DependentIdByPosition)(short ai_pos, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_DependentIndexByPosition)(short ai_pos, /*[out, retval]*/ short *pVal);
	STDMETHOD(get_NumberOfDependents)(/*[out, retval]*/ short *pVal);
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

private:
	m4return_t SetBstr(m4pcchar_t ai_pszString, BSTR* ao_pstr);
	ClItem* m_poItem;

    // Instead of caching it, we get it when we need it.
    // This allows higher level caching in WrapItIt
	//ClItemIndex m_oIndex;
};

#endif //__WRAPITEMDEF_H_
