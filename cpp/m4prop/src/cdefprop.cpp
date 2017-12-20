// CDefProp.cpp : Implementation of CCDefProp
#include "stdafx.h"
#include "m4prop.h"
#include "propdata.h"
#include "cdefprop.h"
#include "myvariant.h"
#include "m4unicode.hpp"


/////////////////////////////////////////////////////////////////////////////
// CCDefProp


STDMETHODIMP CCDefProp::get_Name(BSTR * pVal)
{
	USES_CONVERSION;

	if (m_pData)
	{
		if (pVal && *pVal)
		{
			::SysFreeString(*pVal);
		}
		const char * cTemp = GetNameFromPropId(m_pData->nIdProperty);
		*pVal = M4CppToVB(cTemp);
	}
	else
	{
		return E_INVALIDARG;
	}
	return S_OK;
}

STDMETHODIMP CCDefProp::put_Name(BSTR newVal)
{
	USES_CONVERSION;

	if (m_pData)
	{
		if (newVal)
		{
			char* pTemp = M4VBToCpp(newVal);
			m_pData->nIdProperty = GetIdFromPropName(pTemp);
			delete pTemp;
		}
		else
		{
			m_pData->nIdProperty = -1;
		}
	}
	else
	{
		return E_INVALIDARG;
	}
	return S_OK;
}

STDMETHODIMP CCDefProp::get_Value(VARIANT * pVal)
{
	if (m_pData)
	{
		return GetValueFromDefProp(m_pData, pVal);
	}
	return E_INVALIDARG;
}

STDMETHODIMP CCDefProp::put_Value(VARIANT newVal)
{
	if (m_pData)
	{
		return PutValueInDefProp(m_pData, &newVal);
	}
	return E_INVALIDARG;
}

STDMETHODIMP CCDefProp::get_DefValue(VARIANT * pVal)
{
	if (m_pData)
	{
		return GetDefValueFromDefProp(m_pData, pVal);
	}
	return E_INVALIDARG;
}

STDMETHODIMP CCDefProp::put_DefValue(VARIANT newVal)
{
	if (m_pData)
	{
		return PutDefValueInDefProp(m_pData, &newVal);
	}
	return E_INVALIDARG;
}

STDMETHODIMP CCDefProp::WriteASN(unsigned char * * charIn)
{
	USES_CONVERSION;
		
	return E_NOTIMPL;
}


STDMETHODIMP CCDefProp::get_Type(M4PropertyType * pVal)
{
	if (m_pData)
	{
		*pVal = m_pData->Type;
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP CCDefProp::put_Type(M4PropertyType newVal)
{

	if (m_pData)
	{
		m_pData->Type = newVal;
		return S_OK;
	}
	return E_INVALIDARG;
}

void CCDefProp::SetData(DefProp *pData)
{
	ATLASSERT(m_pData == NULL);
	m_pData = pData;
}
