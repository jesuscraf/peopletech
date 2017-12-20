// MDObject.cpp : Implementation of CMDObject
#include "stdafx.h"
#include "m4prop.h"
#include "mdobject.h"

/////////////////////////////////////////////////////////////////////////////
// CMDObject


STDMETHODIMP CMDObject::get_Obj(IDispatch * * pVal)
{
	// TODO: Add your implementation code here
	*pVal = m_pObject;
	if (m_pObject) m_pObject->AddRef();
	return S_OK;
}

STDMETHODIMP CMDObject::putref_Obj(IDispatch * *newVal)
{
	// TODO: Add your implementation code here
	RELEASE(m_pObject);
	m_pObject = *newVal;
	if (m_pObject) m_pObject->AddRef();
	return S_OK;
}

STDMETHODIMP CMDObject::get_Key(BSTR * pVal)
{
	if (m_bstrKey)
		*pVal = ::SysAllocString(m_bstrKey);
	else
		*pVal = ::SysAllocString(L"");
	return S_OK;
}

STDMETHODIMP CMDObject::put_Key(BSTR newVal)
{
	// TODO: Add your implementation code here
	RELEASE_BSTR(m_bstrKey);
	if (newVal)
		m_bstrKey = ::SysAllocString(newVal);
	return S_OK;

}

STDMETHODIMP CMDObject::get_PropBag(ICPropBag * * pVal)
{
	*pVal = m_PropBag;
	if (m_PropBag) m_PropBag->AddRef();
	return S_OK;
}

STDMETHODIMP CMDObject::putref_PropBag(ICPropBag * *newVal)
{
	RELEASE(m_PropBag);
	m_PropBag = *newVal;
	if (m_PropBag) m_PropBag->AddRef();
	return S_OK;
}
