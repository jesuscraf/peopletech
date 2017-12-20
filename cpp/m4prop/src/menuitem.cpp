// MenuItem.cpp : Implementation of CMenuItem
#include "stdafx.h"
#include "m4prop.h"
#include "menuitem.h"

/////////////////////////////////////////////////////////////////////////////
// CMenuItem


STDMETHODIMP CMenuItem::get_Key(BSTR * pVal)
{
	if (m_strKey)
		*pVal = ::SysAllocString(m_strKey);
	else
		*pVal = ::SysAllocString(L"");

	return S_OK;
}

STDMETHODIMP CMenuItem::put_Key(BSTR newVal)
{
	RELEASE_BSTR(m_strKey);
	if (newVal)
		m_strKey = ::SysAllocString(newVal);

	return S_OK;
}

STDMETHODIMP CMenuItem::get_Caption(BSTR * pVal)
{
	if (m_strCaption)
		*pVal = ::SysAllocString(m_strCaption);
	else
		*pVal = ::SysAllocString(L"");

	return S_OK;
}

STDMETHODIMP CMenuItem::put_Caption(BSTR newVal)
{

	RELEASE_BSTR(m_strCaption);
	if (newVal)
		m_strCaption = ::SysAllocString(newVal);

	return S_OK;
}

STDMETHODIMP CMenuItem::get_Checked(VARIANT_BOOL * pVal)
{
	if (pVal)
	{
		*pVal = m_bChecked;
	}

	return S_OK;
}

STDMETHODIMP CMenuItem::put_Checked(VARIANT_BOOL newVal)
{
	m_bChecked = newVal;

	return S_OK;
}

STDMETHODIMP CMenuItem::get_Enabled(VARIANT_BOOL * pVal)
{
	if (pVal)
	{
		*pVal = m_bEnabled;
	}

	return S_OK;
}

STDMETHODIMP CMenuItem::put_Enabled(VARIANT_BOOL newVal)
{

	m_bEnabled = newVal;
	return S_OK;
}

STDMETHODIMP CMenuItem::get_Bold(VARIANT_BOOL * pVal)
{

	if (pVal)
	{
		*pVal = m_bBold;
	}
	return S_OK;
}

STDMETHODIMP CMenuItem::put_Bold(VARIANT_BOOL newVal)
{
	m_bBold = newVal;
	return S_OK;
}

STDMETHODIMP CMenuItem::get_Visible(VARIANT_BOOL * pVal)
{
	if (pVal)
	{
		*pVal = m_bVisible;
	}
	return S_OK;
}

STDMETHODIMP CMenuItem::put_Visible(VARIANT_BOOL newVal)
{
	m_bVisible = newVal;
	return S_OK;
}
