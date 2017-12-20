// CProperty.cpp : Implementation of CCProperty
#include "stdafx.h"
#include "m4prop.h"
#include "cproperty.h"

/////////////////////////////////////////////////////////////////////////////
// CCProperty


STDMETHODIMP CCProperty::get_Name(BSTR * pVal)
{
	if (m_strName)
		*pVal = ::SysAllocString(m_strName);
	else
		*pVal = ::SysAllocString(L"");

	return S_OK;
}

STDMETHODIMP CCProperty::put_Name(BSTR newVal)
{
	RELEASE_BSTR(m_strName);
	if (newVal)
		m_strName = ::SysAllocString(newVal);
	return S_OK;
}

STDMETHODIMP CCProperty::get_Value(VARIANT * pVal)
{
	return VariantCopy(pVal, &m_vValue);

}

STDMETHODIMP CCProperty::put_Value(VARIANT newVal)
{
	return VariantCopy(&m_vValue, &newVal);	
}

STDMETHODIMP CCProperty::get_Type(M4PropertyType * pVal)
{
	*pVal = m_iType;
	return S_OK;
}

STDMETHODIMP CCProperty::put_Type(M4PropertyType newVal)
{
	m_iType = newVal;
	return S_OK;
}

STDMETHODIMP CCProperty::get_Editable(VARIANT_BOOL * pVal)
{
	*pVal = m_bEditable;
	return S_OK;
}

STDMETHODIMP CCProperty::put_Editable(VARIANT_BOOL newVal)
{
	m_bEditable = newVal;
	return S_OK;
}

STDMETHODIMP CCProperty::get_Group(BSTR * pVal)
{
	if (m_strGroup)
		*pVal = ::SysAllocString(m_strGroup);
	else
		*pVal = ::SysAllocString(L"");
	return S_OK;
}

STDMETHODIMP CCProperty::put_Group(BSTR newVal)
{
	RELEASE_BSTR(m_strGroup);
	if (newVal)
		m_strGroup = ::SysAllocString(newVal);

	return S_OK;
}

STDMETHODIMP CCProperty::get_DefValue(VARIANT * pVal)
{
	
	// No es necesario hacer un Free del destino, ya se encarga el VariantCopy
	return VariantCopy(pVal, &m_vDefValue);
}

STDMETHODIMP CCProperty::put_DefValue(VARIANT newVal)
{

	// No es necesario hacer un Free del destino, ya se encarga el VariantCopy
	return VariantCopy(&m_vDefValue, &newVal);
}
