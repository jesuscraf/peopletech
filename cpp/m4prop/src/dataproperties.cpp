// DataProperties.cpp : Implementation of CDataProperties
#include "stdafx.h"
#include "m4prop.h"
#include "dataproperties.h"

/////////////////////////////////////////////////////////////////////////////
// CDataProperties


STDMETHODIMP CDataProperties::get_IdChannel(BSTR * pVal)
{
	*pVal = GetBSTRFromId(m_nmIdChannel);
	if (*pVal == NULL)
		*pVal = SysAllocString(L"");
	return S_OK;
}

STDMETHODIMP CDataProperties::put_IdChannel(BSTR newVal)
{
	m_nmIdChannel = GetIdFromBSTR(newVal);
	return S_OK;
}

STDMETHODIMP CDataProperties::get_IdChannelAlias(BSTR * pVal)
{
	*pVal = GetBSTRFromId(m_nmIdChannelAlias);
	if (*pVal == NULL)
		*pVal = SysAllocString(L"");
	return S_OK;
}

STDMETHODIMP CDataProperties::put_IdChannelAlias(BSTR newVal)
{
	m_nmIdChannelAlias = GetIdFromBSTR(newVal);
	return S_OK;
}

STDMETHODIMP CDataProperties::get_IdChannelKey(BSTR * pVal)
{
	*pVal = GetBSTRFromId(m_nmIdChannelKey);
	if (*pVal == NULL)
		*pVal = SysAllocString(L"");
	return S_OK;
}

STDMETHODIMP CDataProperties::put_IdChannelKey(BSTR newVal)
{
	m_nmIdChannelKey = GetIdFromBSTR(newVal);
	return S_OK;
}

STDMETHODIMP CDataProperties::get_IdNode(BSTR * pVal)
{
	*pVal = GetBSTRFromId(m_nmIdNode);
	if (*pVal == NULL)
		*pVal = SysAllocString(L"");
	return S_OK;
}

STDMETHODIMP CDataProperties::put_IdNode(BSTR newVal)
{
	m_nmIdNode = GetIdFromBSTR(newVal);
	return S_OK;
}

STDMETHODIMP CDataProperties::get_IdItem(BSTR * pVal)
{
	*pVal = GetBSTRFromId(m_nmIdItem);
	if (*pVal == NULL)
		*pVal = SysAllocString(L"");
	return S_OK;
}

STDMETHODIMP CDataProperties::put_IdItem(BSTR newVal)
{
	m_nmIdItem = GetIdFromBSTR(newVal);
	return S_OK;
}

STDMETHODIMP CDataProperties::get_IdAccessAlias(BSTR * pVal)
{
	*pVal = GetBSTRFromId(m_nmIdAccessAlias);
	if (*pVal == NULL)
		*pVal = SysAllocString(L"");
	return S_OK;
}

STDMETHODIMP CDataProperties::put_IdAccessAlias(BSTR newVal)
{
	m_nmIdAccessAlias = GetIdFromBSTR(newVal);
	return S_OK;
}

STDMETHODIMP CDataProperties::get_IdPresentation(long * pVal)
{
	*pVal = m_nIdPresentation;
	return S_OK;
}

STDMETHODIMP CDataProperties::put_IdPresentation(long newVal)
{
	m_nIdPresentation = newVal;
	return S_OK;
}

STDMETHODIMP CDataProperties::get_Grants(short * pVal)
{
	*pVal = m_nGrants;
	return S_OK;
}

STDMETHODIMP CDataProperties::put_Grants(short newVal)
{
	if ((newVal >= 0) && (newVal <= 31))
	{
		m_nGrants = newVal;
		return S_FALSE;
	}
	return S_OK;
}

STDMETHODIMP CDataProperties::Clone(IDataProperties * * ppDataProps)
{
	*ppDataProps = NULL;
	
	CComObject<CDataProperties> * pNew = NULL;
	CComObject<CDataProperties>::CreateInstance(&pNew);
	if (pNew)
	{
		pNew->m_nGrants = m_nGrants;
		pNew->m_nStartPos = m_nStartPos;
		pNew->m_nIdPresentation = m_nIdPresentation;
		pNew->m_nmIdChannel = m_nmIdChannel;
		pNew->m_nmIdChannelKey = m_nmIdChannelKey;
		pNew->m_nmIdChannelAlias = m_nmIdChannelAlias;
		pNew->m_nmIdNode = m_nmIdNode;
		pNew->m_nmIdItem = m_nmIdItem;
		pNew->m_nmIdAccessAlias = m_nmIdAccessAlias;
		
		return pNew->QueryInterface(IID_IDataProperties, (void**)ppDataProps);
	}
	return E_FAIL;
}

STDMETHODIMP CDataProperties::get_StartPos(long * pVal)
{
	if (pVal)
		*pVal = m_nStartPos;
	return S_OK;
}

STDMETHODIMP CDataProperties::put_StartPos(long newVal)
{
	m_nStartPos = newVal;
	return S_OK;
}

STDMETHODIMP CDataProperties::Reset()
{
	m_nmIdChannel = -1;
	m_nmIdNode = -1;
	m_nmIdItem = -1;
	m_nmIdAccessAlias = -1;
	m_nmIdChannelKey = -1;
	m_nmIdChannelAlias = -1;
	m_nIdPresentation = -1;
	m_nGrants = 31; 
	m_nStartPos = -6;

	return S_OK;
}
