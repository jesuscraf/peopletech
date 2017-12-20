// DGenericObject.h : Declaration of the CDGenericObject

#ifndef __DGENERICOBJECT_H_
#define __DGENERICOBJECT_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDGenericObject
class ATL_NO_VTABLE CDGenericObject : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDGenericObject, &CLSID_DGenericObject>,
	public IDispatchImpl<IDGenericObject, &IID_IDGenericObject, &LIBID_M4PROPLib>,
//	public IDispatchImpl<IMenu, &IID_IMenu, &LIBID_M4PROPLib>,
	public IDesignObject,
	public IPropertiesEx,
	public IDataProps,
	public IMenu
{
public:
	CDGenericObject()
	{
		m_strContainer = NULL;
		m_nExecOrder = 0;
		
		CComObject<CContainedObjects> * pCont;
		CComObject<CContainedObjects>::CreateInstance(&pCont);
		pCont->QueryInterface(IID_IContainedObjects, (void**) &m_pContObj);
		m_pFEDesControl = NULL;
		m_pbGeneral = NULL;
		m_pData = NULL;
		m_pContainer = NULL;
		//ATLTRACE("Construcción GenericObject\n");
	}

	~CDGenericObject()
	{
		//::SysFreeString(m_strKey);
		::SysFreeString(m_strContainer);
		RELEASE(m_pContObj);
		RELEASE(m_pContainer);
		RELEASE(m_pFEDesControl);
		RELEASE(m_pbGeneral);
		RELEASE(m_pData);
//		ATLTRACE("Destrucción GenericObject\n");
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DGENERICOBJECT)

BEGIN_COM_MAP(CDGenericObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IDGenericObject)	
	COM_INTERFACE_ENTRY(IDesignObject)
	COM_INTERFACE_ENTRY(IPropertiesEx)
	COM_INTERFACE_ENTRY(IDataProps)
	COM_INTERFACE_ENTRY(IMenu)
END_COM_MAP()

// IDGenericObject
public:

private:
//	BSTR				m_strKey;
	BSTR				m_strContainer;
//	BSTR				m_strClassName;
	long				m_nExecOrder;
	IContainedObjects * m_pContObj;
	IFEDesControl *		m_pFEDesControl;
	ICPropBagDef *		m_pbGeneral;
	IDataProperties *	m_pData;
	IDesignObject *		m_pContainer;

public:
	// Interface IDesignObject
    STDMETHOD(Construct)(IDispatch ** oFact, IDispatch** DesControl, BSTR* strItemAlias);
    STDMETHOD(Destruct)(IDispatch ** oFact);
//    STDMETHOD(Persist)(ICPropBag** pb,VARIANT_BOOL* bPersist,IPersistObject** pPrObject,BSTR* strIDContainer);
    STDMETHOD(Persist2)(ICPropBagDef** pbd,VARIANT_BOOL* bPersist);
    STDMETHOD(get_Alias)(BSTR* bstrAlias);
    STDMETHOD(put_Alias)(BSTR* bstrAlias);
    STDMETHOD(get_Path)(BSTR* bstrPath);
    STDMETHOD(get_ClassName)(BSTR* bstrClassName);
    STDMETHOD(put_ClassName)(BSTR* bstrClassName);
    STDMETHOD(get_Key)(BSTR* bstrKey);
    STDMETHOD(put_Key)(BSTR* bstrKey);
    STDMETHOD(get_Container)(BSTR* );
    STDMETHOD(put_Container)(BSTR* );
    STDMETHOD(get_ProgId)(BSTR* );
    STDMETHOD(get_ExecOrder)(short* );
    STDMETHOD(put_ExecOrder)(short* );
    STDMETHOD(get_PropertyCol)(IPropCol** );
    STDMETHOD(put_PropertyCol)(IPropCol** );
//    STDMETHOD(GetIcon)(IPictureDisp** );
    STDMETHOD(get_Toolstring)(BSTR* );
    STDMETHOD(ShowProp)();
    STDMETHOD(get_InToolbar)(VARIANT_BOOL* );
    STDMETHOD(get_ContainedObjects)(IContainedObjects** );
//    STDMETHOD(get_Item)(VARIANT* nIndex,IDispatch** );
    STDMETHOD(get_Count)(short* );
    STDMETHOD(AddControl)(IDispatch** newValue,BSTR* strKey);
    STDMETHOD(RemoveControl)(BSTR* strKey);
	STDMETHOD(GetNextExecOrder)(short* );
//    STDMETHOD(IsInListObject)(VARIANT_BOOL* );
//    STDMETHOD(OrderExecOrder)(BSTR* strItemKey,short* nExecOrder);
	STDMETHOD(DestructChild)();
    STDMETHOD(PostConstruct)();
    STDMETHOD(ValidateInsertion)(BSTR* strClassName,VARIANT_BOOL* );
    STDMETHOD(get_VisualName)(BSTR* );
    STDMETHOD(get_CanRemoveNow)(VARIANT_BOOL* );
    STDMETHOD(RefreshView)();
//    STDMETHOD(get_GetDocumentInfo)(ICPropBag** );
    STDMETHOD(Activate)(IDispatch** pFactor,IDispatch** theDesign);
    STDMETHOD(Deactivate)();
    STDMETHOD(Constructed)(VARIANT_BOOL* );
    STDMETHOD(CheckState)(IDesignObject** ADesignObject);
    STDMETHOD(get_ReferencedObjects)(IContainedObjects** );
    STDMETHOD(get_PropBagDef)(ICPropBagDef** );
	STDMETHOD(get_DataProps)(IDataProperties** );
	STDMETHOD(SetInitialPBDef)(ICPropBagDef * );

// Implementación de la IPropertiesEx
    STDMETHOD(AssignInheritedProps)(ICPropBag** );
	STDMETHOD(ResetProps)();

// Interface IDataProps
	STDMETHOD(get_Grants)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_Grants)(/*[in]*/ short * newVal);
	STDMETHOD(get_IdPresentation)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_IdPresentation)(/*[in]*/ long * newVal);
	STDMETHOD(get_IdAccessAlias)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_IdAccessAlias)(/*[in]*/ BSTR * newVal);
	STDMETHOD(get_IdItem)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_IdItem)(/*[in]*/ BSTR * newVal);
	STDMETHOD(get_IdNode)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_IdNode)(/*[in]*/ BSTR * newVal);
	STDMETHOD(get_IdChannelKey)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_IdChannelKey)(/*[in]*/ BSTR * newVal);
	STDMETHOD(get_IdChannelAlias)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_IdChannelAlias)(/*[in]*/ BSTR * newVal);
	STDMETHOD(get_IdChannel)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_IdChannel)(/*[in]*/ BSTR * newVal);
	STDMETHOD(get_StartPos)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_StartPos)(/*[in]*/ long newVal);

// Interface IMenu
	STDMETHOD(InitializePopPup)(IMenuPopPup * tpp);
	STDMETHOD(ExecuteMenu)(/*[in]*/BSTR * Tag);

};

#endif //__DGENERICOBJECT_H_
