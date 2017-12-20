// PresentationItem.h : Declaration of the CPresentationItem

#ifndef __PRESENTATIONITEM_H_
#define __PRESENTATIONITEM_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CPresentationItem
class ATL_NO_VTABLE CPresentationItem : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CPresentationItem, &CLSID_PresentationItem>,
	public IDispatchImpl<IPresentationItem, &IID_IPresentationItem, &LIBID_M4DHOLib>
{
public:
	CPresentationItem()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_PRESENTATIONITEM)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CPresentationItem)
	COM_INTERFACE_ENTRY(IPresentationItem)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IPresentationItem
public:
	STDMETHOD(get_PropBag)(/*[out, retval]*/ ICPropBag **pVal);
	STDMETHOD(putref_PropBag)(/*[in]*/ ICPropBag *newVal);
	STDMETHOD(get_PropBagDef)(/*[out, retval]*/ ICPropBagDef **pVal);
	STDMETHOD(putref_PropBagDef)(/*[in]*/ ICPropBagDef *newVal);
	STDMETHOD(get_IDMetaleng)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_IDMetaleng)(/*[in]*/ long newVal);
	STDMETHOD(get_BlockRobot)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_BlockRobot)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_Description)(/*[out, retval]*/ ITraductProp **pVal);
	STDMETHOD(putref_Description)(/*[in]*/ ITraductProp *newVal);
	STDMETHOD(get_PresentType)(/*[out, retval]*/ M4PresentType *pVal);
	STDMETHOD(put_PresentType)(/*[in]*/ M4PresentType newVal);
	STDMETHOD(get_MultiCompany)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_MultiCompany)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_MainObject)(/*[out, retval]*/ IDesignObject **pVal);
	STDMETHOD(putref_MainObject)(/*[in]*/ IDesignObject *newVal);
	STDMETHOD(get_DateUpdate)(/*[out, retval]*/ DATE *pVal);
	STDMETHOD(put_DateUpdate)(/*[in]*/ DATE newVal);
	STDMETHOD(get_DateCreate)(/*[out, retval]*/ DATE *pVal);
	STDMETHOD(put_DateCreate)(/*[in]*/ DATE newVal);
	STDMETHOD(get_ModifiedFlag)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_ModifiedFlag)(/*[in]*/ short newVal);
	STDMETHOD(get_OwnerFlag)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_OwnerFlag)(/*[in]*/ long newVal);
	STDMETHOD(get_PresentationID)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_PresentationID)(/*[in]*/ BSTR newVal);

private:
	BSTR				m_bsPresentationID;
	BSTR				m_bsParentPresentationID;
	BSTR				m_bsIDChannel;
	long				m_nOwnerFlag;
	short				m_nModifiedFlag;
	DATE				m_dDateCreate;
	DATE				m_dDateUpdate;
	IDesignObject *		m_oMainObject;
	VARIANT_BOOL		m_bMultiCompany;
	VARIANT_BOOL		m_bIsInherited;
	VARIANT_BOOL		m_bReadonly;
	ITraductProp *		m_oTraductProp;
	VARIANT_BOOL		m_bBlockRobot;
	long				m_nIdMetaleng;
	M4PresentType		m_ePresentType;
	ICPropBagDef *		m_oPropBagDef;
	ICPropBagDef *		m_oParentPropBagDef;
	ICPropBag *			m_oPropBag;
	M4PresentationState m_ePresentState;
	M4PresentStyles		m_ePresentStyle;


public:
	STDMETHOD(get_Readonly)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Readonly)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_IDChannel)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_IDChannel)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_PresentationStyle)(/*[out, retval]*/ M4PresentStyles *pVal);
	STDMETHOD(put_PresentationStyle)(/*[in]*/ M4PresentStyles newVal);
	STDMETHOD(get_ParentPresentationID)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_ParentPresentationID)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_ParentPropBagDef)(/*[out, retval]*/ ICPropBagDef * *pVal);
	STDMETHOD(putref_ParentPropBagDef)(/*[in]*/ ICPropBagDef * newVal);
	STDMETHOD(get_IsInherited)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_IsInherited)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_State)(/*[out, retval]*/ M4PresentationState *pVal);
	STDMETHOD(put_State)(/*[in]*/ M4PresentationState newVal);
	
	void FinalRelease()
	{
		RELEASE(m_oMainObject);
		RELEASE(m_oTraductProp);
		RELEASE(m_oPropBagDef);
		RELEASE(m_oParentPropBagDef);
		RELEASE(m_oPropBag);
	}
	
	HRESULT FinalConstruct()
	{
		m_bsPresentationID = NULL;
		m_bsParentPresentationID = NULL;
		m_bsIDChannel = NULL;
		m_nOwnerFlag = 0;
		m_nModifiedFlag = 0;
		m_dDateCreate = 0;
		m_dDateUpdate = 0;
		m_oMainObject = NULL;
		m_bMultiCompany = 0;
		m_bIsInherited = 0;
		m_bReadonly = 0;
		m_oTraductProp = NULL;
		m_bBlockRobot = 0;
		m_nIdMetaleng = 202;
		m_ePresentType = PT_PRESENTATION;
		m_oPropBagDef = NULL;
		m_oParentPropBagDef = NULL;
		m_oPropBag = NULL;
		m_ePresentState = PR_STATE_NEW;
		m_ePresentStyle = M4PS_STANDARD;
		return 0;
	}

};

#endif //__PRESENTATIONITEM_H_
