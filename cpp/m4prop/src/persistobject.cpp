// PersistObject.cpp : Implementation of CPersistObject
#include "stdafx.h"
#include "m4prop.h"
#include "persistobject.h"

/////////////////////////////////////////////////////////////////////////////
// CPersistObject


STDMETHODIMP CPersistObject::get_PersistTraductProp(ITraductProp * * pVal)
{
	*pVal = m_pTraduct;
	if (m_pTraduct)
	{
		m_pTraduct->AddRef();
	}
	return S_OK;
}

STDMETHODIMP CPersistObject::putref_PersistTraductProp(ITraductProp * newVal)
{
	RELEASE(m_pTraduct);
	m_pTraduct = newVal;
	if (m_pTraduct)
	{
		m_pTraduct->AddRef();
	}

	return S_OK;
}

STDMETHODIMP CPersistObject::get_PersistMain(BSTR * pVal)
{
	if (m_strMain)
		*pVal = ::SysAllocStringLen(m_strMain, ::SysStringLen(m_strMain));
	else
		*pVal = ::SysAllocString(L"");
	return S_OK;
}

STDMETHODIMP CPersistObject::put_PersistMain(BSTR newVal)
{
	RELEASE_BSTR(m_strMain);
	if (newVal)
		m_strMain = ::SysAllocStringLen(newVal, ::SysStringLen(newVal));
	return S_OK;
}
