// PresentationItem.cpp : Implementation of CPresentationItem
#include "stdafx.h"
#include "m4dho.h"
#include "presentationitem.h"

/////////////////////////////////////////////////////////////////////////////
// CPresentationItem


STDMETHODIMP CPresentationItem::get_PresentationID(BSTR *pVal)
{
	if (pVal != NULL)
	{
		if (m_bsPresentationID)
			*pVal = SysAllocString(m_bsPresentationID);
		else
			*pVal = SysAllocString(L"");
		return S_OK;
	}

	return E_POINTER;
}

STDMETHODIMP CPresentationItem::put_PresentationID(BSTR newVal)
{
	SysFreeString(m_bsPresentationID);
	m_bsPresentationID = NULL;

	if (newVal)
		m_bsPresentationID = SysAllocString(newVal);

	return S_OK;
}

STDMETHODIMP CPresentationItem::get_OwnerFlag(long *pVal)
{
	if (pVal)
		*pVal = m_nOwnerFlag;
	else
		return E_POINTER;

	return S_OK;
}

STDMETHODIMP CPresentationItem::put_OwnerFlag(long newVal)
{
	m_nOwnerFlag = newVal;

	return S_OK;
}

STDMETHODIMP CPresentationItem::get_ModifiedFlag(short *pVal)
{
	if (pVal)
		*pVal = m_nModifiedFlag;
	else
		return E_POINTER;

	return S_OK;
}

STDMETHODIMP CPresentationItem::put_ModifiedFlag(short newVal)
{
	m_nModifiedFlag = newVal;

	return S_OK;
}

STDMETHODIMP CPresentationItem::get_DateCreate(DATE *pVal)
{
	if (pVal)
		*pVal = m_dDateCreate;
	else
		return E_POINTER;

	return S_OK;
}

STDMETHODIMP CPresentationItem::put_DateCreate(DATE newVal)
{
	m_dDateCreate = newVal;

	return S_OK;
}

STDMETHODIMP CPresentationItem::get_DateUpdate(DATE *pVal)
{
	if (pVal)
		*pVal = m_dDateUpdate;
	else
		return E_POINTER;

	return S_OK;
}

STDMETHODIMP CPresentationItem::put_DateUpdate(DATE newVal)
{
	m_dDateUpdate = newVal;

	return S_OK;
}

STDMETHODIMP CPresentationItem::get_MainObject(IDesignObject **pVal)
{
	if (pVal)
	{
		if (m_oMainObject)
		{
			*pVal = m_oMainObject;
			m_oMainObject->AddRef();
		}
		else
			*pVal = NULL;

		return S_OK;
	}

	return E_POINTER;
}

STDMETHODIMP CPresentationItem::putref_MainObject(IDesignObject *newVal)
{
	if (m_oMainObject)
		m_oMainObject->Release();

	m_oMainObject = newVal;

	if (m_oMainObject)
		m_oMainObject->AddRef();
	
	return S_OK;
}

STDMETHODIMP CPresentationItem::get_PresentType(M4PresentType *pVal)
{
	if (pVal)
		*pVal = m_ePresentType;
	else
		return E_POINTER;

	return S_OK;
}

STDMETHODIMP CPresentationItem::put_PresentType(M4PresentType newVal)
{
	m_ePresentType = newVal;

	return S_OK;
}

STDMETHODIMP CPresentationItem::get_MultiCompany(VARIANT_BOOL *pVal)
{
	if (pVal)
		*pVal = m_bMultiCompany;
	else
		return E_POINTER;

	return S_OK;
}

STDMETHODIMP CPresentationItem::put_MultiCompany(VARIANT_BOOL newVal)
{
	m_bMultiCompany = newVal;

	return S_OK;
}

STDMETHODIMP CPresentationItem::get_Description(ITraductProp **pVal)
{
	if (pVal)
	{
		if (m_oTraductProp)
		{
			*pVal = m_oTraductProp;
			m_oTraductProp->AddRef();
		}
		else
			*pVal = NULL;

		return S_OK;
	}

	return E_POINTER;
}

STDMETHODIMP CPresentationItem::putref_Description(ITraductProp *newVal)
{
	if (m_oTraductProp)
		m_oTraductProp->Release();

	m_oTraductProp = newVal;

	if (m_oTraductProp)
		m_oTraductProp->AddRef();
	
	return S_OK;
}

STDMETHODIMP CPresentationItem::get_BlockRobot(VARIANT_BOOL *pVal)
{
	if (pVal)
		*pVal = m_bBlockRobot;
	else
		return E_POINTER;

	return S_OK;
}

STDMETHODIMP CPresentationItem::put_BlockRobot(VARIANT_BOOL newVal)
{
	m_bBlockRobot = newVal;

	return S_OK;
}

STDMETHODIMP CPresentationItem::get_IDMetaleng(long *pVal)
{
	if (pVal)
		*pVal = m_nIdMetaleng;
	else
		return E_POINTER;

	return S_OK;
}

STDMETHODIMP CPresentationItem::put_IDMetaleng(long newVal)
{
	m_nIdMetaleng = newVal;

	return S_OK;
}

STDMETHODIMP CPresentationItem::get_PropBagDef(ICPropBagDef **pVal)
{
	
	if (pVal)
	{
		if (m_oPropBagDef)
		{
			*pVal = m_oPropBagDef;
			m_oPropBagDef->AddRef();
		}
		else
			*pVal = NULL;

		return S_OK;
	}

	return E_POINTER;

}

STDMETHODIMP CPresentationItem::putref_PropBagDef(ICPropBagDef *newVal)
{
	if (m_oPropBagDef)
		m_oPropBagDef->Release();

	m_oPropBagDef = newVal;

	if (m_oPropBagDef)
		m_oPropBagDef->AddRef();
	
	return S_OK;
}

STDMETHODIMP CPresentationItem::get_PropBag(ICPropBag **pVal)
{

	if (pVal)
	{
		if (m_oPropBag)
		{
			*pVal = m_oPropBag;
			m_oPropBag->AddRef();
		}
		else
			*pVal = NULL;

		return S_OK;
	}

	return E_POINTER;

}

STDMETHODIMP CPresentationItem::putref_PropBag(ICPropBag *newVal)
{
	if (m_oPropBag)
		m_oPropBag->Release();

	m_oPropBag = newVal;

	if (m_oPropBag)
		m_oPropBag->AddRef();
	
	return S_OK;
}

STDMETHODIMP CPresentationItem::get_State(M4PresentationState *pVal)
{
	if (pVal)
		*pVal = m_ePresentState;
	else
		return E_POINTER;
	return S_OK;
}

STDMETHODIMP CPresentationItem::put_State(M4PresentationState newVal)
{
	m_ePresentState = newVal;
	return S_OK;
}

STDMETHODIMP CPresentationItem::get_IsInherited(VARIANT_BOOL *pVal)
{
	if (pVal)
		*pVal = m_bIsInherited;
	else
		return E_POINTER;

	return S_OK;
}

STDMETHODIMP CPresentationItem::put_IsInherited(VARIANT_BOOL newVal)
{
	m_bIsInherited = newVal;

	return S_OK;
}

STDMETHODIMP CPresentationItem::get_ParentPropBagDef(ICPropBagDef **pVal)
{
	if (pVal)
	{
		if (m_oParentPropBagDef)
		{
			*pVal = m_oParentPropBagDef;
			m_oParentPropBagDef->AddRef();
		}
		else
			*pVal = NULL;

		return S_OK;
	}

	return E_POINTER;
}

STDMETHODIMP CPresentationItem::putref_ParentPropBagDef(ICPropBagDef *newVal)
{
	if (m_oParentPropBagDef)
		m_oParentPropBagDef->Release();

	m_oParentPropBagDef = newVal;

	if (m_oParentPropBagDef)
		m_oParentPropBagDef->AddRef();
	
	return S_OK;
}

STDMETHODIMP CPresentationItem::get_ParentPresentationID(BSTR *pVal)
{
	if (pVal != NULL)
	{
		if (m_bsParentPresentationID)
			*pVal = SysAllocString(m_bsParentPresentationID);
		else
			*pVal = SysAllocString(L"");
		return S_OK;
	}

	return E_POINTER;
}

STDMETHODIMP CPresentationItem::put_ParentPresentationID(BSTR newVal)
{
	SysFreeString(m_bsParentPresentationID);
	m_bsParentPresentationID = NULL;

	if (newVal)
		m_bsParentPresentationID = SysAllocString(newVal);

	return S_OK;
}

STDMETHODIMP CPresentationItem::get_PresentationStyle(M4PresentStyles *pVal)
{
	if (pVal)
		*pVal = m_ePresentStyle;
	else
		return E_POINTER;
	return S_OK;
}

STDMETHODIMP CPresentationItem::put_PresentationStyle(M4PresentStyles newVal)
{
	m_ePresentStyle = newVal;
	return S_OK;
}

STDMETHODIMP CPresentationItem::get_IDChannel(BSTR *pVal)
{
	if (pVal != NULL)
	{
		if (m_bsIDChannel)
			*pVal = SysAllocString(m_bsIDChannel);
		else
			*pVal = SysAllocString(L"");
		return S_OK;
	}

	return E_POINTER;
}

STDMETHODIMP CPresentationItem::put_IDChannel(BSTR newVal)
{
	SysFreeString(m_bsIDChannel);
	m_bsIDChannel = NULL;

	if (newVal)
		m_bsIDChannel = SysAllocString(newVal);

	return S_OK;
}

STDMETHODIMP CPresentationItem::get_Readonly(VARIANT_BOOL *pVal)
{
	if (pVal)
		*pVal = m_bReadonly;
	else
		return E_POINTER;

	return S_OK;
}

STDMETHODIMP CPresentationItem::put_Readonly(VARIANT_BOOL newVal)
{
	m_bReadonly = newVal;

	return S_OK;
}
