// CPropBag.cpp : Implementation of CCPropBag
#include "stdafx.h"
#include "mybstr.h"
#include "myvariant.h"
#include "m4prop.h"
#include "cpropbag.h"
#include "cpropcol.h"
#include "cchildcol.h"
#include "csingleprop.h"
#include "stdio.h"
#include "m4unicode.hpp"


/////////////////////////////////////////////////////////////////////////////
// CCPropBag

STDMETHODIMP CCPropBag::get_PropCol(ICPropCol * * pVal)
{
	*pVal = m_PropCol;
	m_PropCol->AddRef();
	return S_OK;
}

STDMETHODIMP CCPropBag::get_ChildCol(ICChildCol * * pVal)
{
	*pVal = m_ChildCol;
	m_ChildCol->AddRef();
	return S_OK;
}

STDMETHODIMP CCPropBag::get_Alias(BSTR * pVal)
{
	if (m_bstrAlias)
		*pVal = ::SysAllocString(m_bstrAlias);
	else
		*pVal = ::SysAllocString(L"");
	return S_OK;
}

STDMETHODIMP CCPropBag::put_Alias(BSTR newVal)
{
	::SysFreeString(m_bstrAlias);
	m_bstrAlias = NULL;
	if (newVal)
		m_bstrAlias = ::SysAllocString(newVal);
	return S_OK;
}

STDMETHODIMP CCPropBag::get_ClassName(BSTR * pVal)
{
	if (m_bstrClassName)
		*pVal = ::SysAllocString(m_bstrClassName);
	else
		*pVal = ::SysAllocString(L"");
	return S_OK;
}

STDMETHODIMP CCPropBag::put_ClassName(BSTR newVal)
{
	::SysFreeString(m_bstrClassName);
	m_bstrClassName = NULL;
	if (newVal)
		m_bstrClassName = ::SysAllocString(newVal);
	return S_OK;
}

STDMETHODIMP CCPropBag::ConditionalAddProperty(BSTR Name, VARIANT vValue, VARIANT vDefault)
{
	CMyVariant v1, v2;
	v1 = vValue;
	v2 = vDefault;
	if (v1 != v2)
	{
		return AddProperty(Name, v1.m_vData);
	}
	else
	{
		VARIANT v;
		VariantInit(&v);
		v.vt = VT_BSTR;
		v.bstrVal = ::SysAllocString(Name);
		HRESULT hr = m_PropCol->Remove(v);
		VariantClear(&v);
	}
	return S_OK;

}

STDMETHODIMP CCPropBag::AddProperty(BSTR strName, VARIANT vValue)
{
	
	//VARIANT v;
	ICSingleProp *pSingleProp;

	// Comprobamos si es ClassName
	if (wcscmp(L"Classname", strName) == 0)
	{
		::SysFreeString(m_bstrClassName);
		m_bstrClassName = ::SysAllocString(vValue.bstrVal);
		return S_OK;
	}
	// Comprobamos si es Alias
	if (wcscmp(L"Alias", strName) == 0)
	{
		::SysFreeString(m_bstrAlias);
		m_bstrAlias = ::SysAllocString(vValue.bstrVal);
		return S_OK;
	}


	VARIANT v;
	VariantInit(&v);
	v.vt = VT_BSTR;
	v.bstrVal = strName;

	HRESULT hr = m_PropCol->get_Item(v, &pSingleProp);
	if (SUCCEEDED(hr))
	{
		// Le metemos el valor sin quedarnos con referencia
		HRESULT hr = pSingleProp->put_Value(vValue);
		RELEASE(pSingleProp);
		return hr;
	}
	// Creamos una propiedad nueva
	CComObject<CCSingleProp> * pSP;
	hr = CComObject<CCSingleProp>::CreateInstance(&pSP);
	if (hr == S_OK)
	{
		hr = pSP->QueryInterface(IID_ICSingleProp, (void**)&pSingleProp);
	}
	if (hr == S_OK)
	{
		pSingleProp->put_Name(strName);
		pSingleProp->put_Value(vValue);
		hr = m_PropCol->Add(pSingleProp, v);
		RELEASE(pSingleProp);
	}
	return hr;

}

STDMETHODIMP CCPropBag::AddChild(ICPropBag * * newValue)
{
	VARIANT v;
	BSTR bstrKey = NULL;
	long KeyLength = 0;

	VariantInit(&v);
	(*newValue)->get_Alias(&bstrKey);
	KeyLength = ::SysStringLen(bstrKey);
	if (KeyLength > 0)
	{
		v.vt = VT_BSTR;
		v.bstrVal = ::SysAllocString(bstrKey);
	}
	::SysFreeString(bstrKey);
	HRESULT hr = m_ChildCol->Add(*newValue, v);
	VariantClear(&v);

	if (FAILED(hr))
	{
		// Creamos información de error

		ICreateErrorInfo * pCreateErrorInfo;
		CreateErrorInfo(&pCreateErrorInfo);
		wchar_t tmpBuff[255];
				
		wcscpy(tmpBuff, L"Duplicate Object: ");
		wcscat(tmpBuff, v.bstrVal);
		wcscat(tmpBuff, L" In ");
		wcscat(tmpBuff, m_bstrAlias);

		pCreateErrorInfo->SetDescription(tmpBuff);
		pCreateErrorInfo->SetGUID(IID_ICPropBag);
		pCreateErrorInfo->SetSource(L"Property Bags");

		IErrorInfo* pErrorInfo;
		pCreateErrorInfo->QueryInterface(IID_IErrorInfo, (void**)&pErrorInfo);
		SetErrorInfo(0, pErrorInfo);
		RELEASE(pErrorInfo);
		RELEASE(pCreateErrorInfo);
	}
	return hr;
}

STDMETHODIMP CCPropBag::Property(BSTR PropertyName, VARIANT* defValue, VARIANT * vReturn)
{
	
	ICSingleProp *pSingleProp;

	HRESULT hr = m_PropCol->get_Item(CComVariant(PropertyName), &pSingleProp);
	if (SUCCEEDED(hr))
	{
		// No necesito guardar el objeto para nada
		HRESULT hr = pSingleProp->get_Value(vReturn);
		RELEASE(pSingleProp);
		return hr;
	}
	::VariantCopy(vReturn, defValue);
	return S_OK;
}


long CCPropBag::MyLength()
{

	if(m_nLength != 0)
	{
		return 0;
	}
	else
		return m_nLength;

}


long CCPropBag::CalculatePropLength()
{
	
	long ret;
	m_PropCol->get_Length(&ret);
	return ret;

}

STDMETHODIMP CCPropBag::Invalidatelength()
{
	if( m_bstrClassName == NULL )
	{
		return S_OK;
	}

	m_ChildCol->InvalidateLength();

	long ntmpVal = 0;
	m_PropCol->get_Length(&ntmpVal);
	m_nLength = ntmpVal;
	m_ChildCol->get_Length(&ntmpVal);
	m_nLength += ntmpVal;

	char* pTemp = M4VBToCpp(m_bstrClassName);
	long ntmpLength = strlen(pTemp);
	delete pTemp;

	if (ntmpLength != 0)
	{
		m_nLength += ntmpLength + 12;
		m_nLength += Longlength(ntmpLength);
	}

	pTemp = M4VBToCpp(m_bstrAlias);
	ntmpLength = strlen(pTemp);
	delete pTemp;

	if (ntmpLength != 0)
	{
		m_nLength += ntmpLength + 8;
		m_nLength += Longlength(ntmpLength);
	}

	return S_OK;
}

STDMETHODIMP CCPropBag::get_Length(long * pVal)
{

	if (m_nLength == 0) Invalidatelength();

	*pVal = m_nLength;


	return S_OK;
}

STDMETHODIMP CCPropBag::WriteASN(unsigned char** charIn)
{

	USES_CONVERSION;
	
	if( m_bstrClassName == NULL )
	{
		return S_OK;
	}

	char* tmpName = M4VBToCpp(m_bstrClassName);
	long nPropLength = strlen(tmpName);
	if (nPropLength != 0)
	{
		writeString(charIn, "Classname", 9);
		writeString(charIn, tmpName, nPropLength);
	}
	delete tmpName;

	tmpName = M4VBToCpp(m_bstrAlias);
	nPropLength = strlen(tmpName);
	if (nPropLength != 0)
	{
		writeString(charIn, "Alias", 5); 
		writeString(charIn, tmpName, nPropLength);
	}
	delete tmpName;
	
	CCPropCol * cpc = static_cast<CCPropCol *>(m_PropCol);
	CCChildCol * ccc = static_cast<CCChildCol *>(m_ChildCol); 
	cpc->WriteASN(charIn);
	ccc->WriteASN(charIn);

	return S_OK;
}

STDMETHODIMP CCPropBag::WriteXML(ClLongString &roXML)
{
	int iLength = -1;

	if( m_bstrClassName == NULL )
	{
		return S_OK;
	}

	iLength = -1;
	char* tmpName = M4VBToCpp(m_bstrClassName);
	char* pcElement = M4CppToUtf8(tmpName, iLength);
	delete tmpName;
	roXML.Concat("<");
	roXML.Concat(pcElement);

	iLength = -1;
	tmpName = M4VBToCpp(m_bstrAlias);
	char* pcAlias = M4CppToUtf8(tmpName, iLength);
	delete tmpName;
	roXML.Concat(" Alias='");
	roXML.Concat(pcAlias, iLength, M4MDRT_CODE_MODE_XML);
	roXML.Concat("'");
	delete pcAlias;

	CCPropCol *cpc = static_cast<CCPropCol *>(m_PropCol);
	CCChildCol *ccc = static_cast<CCChildCol *>(m_ChildCol); 

	cpc->WriteXML(roXML);

	long iChildNumber = 0;
	ccc->get_Count(&iChildNumber);

	if(iChildNumber > 0)
	{
		roXML.Concat(">");
		ccc->WriteXML(roXML);
		roXML.Concat("</");
		roXML.Concat(pcElement);
		roXML.Concat(">");
	}
	else
	{
		roXML.Concat("/>");
	}

	delete pcElement;
	return S_OK;
}

void CCPropBag::PrePersistProccess()
{
	CCPropCol * cpc = static_cast<CCPropCol *>(m_PropCol);
	cpc->PrePersistProccess();
	CCChildCol * ccc = (CCChildCol *)m_ChildCol;
	ccc->PrePersistProccess();
}

STDMETHODIMP CCPropBag::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_ICPropBag,
	};
	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}
