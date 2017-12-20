// TraductProp.cpp : Implementation of CTraductProp
#include "stdafx.h"
#include "m4prop.h"
#include "traductprop.h"
#include "m4unicode.hpp"


/////////////////////////////////////////////////////////////////////////////
// CTraductProp


STDMETHODIMP CTraductProp::Init(long PropertyNumber, _Collection * pCollection)
{

	IUnknown *pUnk = NULL;
	IEnumVARIANT *pEnum = NULL;
	VARIANT * pVar = new VARIANT;

	HRESULT hrres = S_OK;
	
	::VariantInit(pVar);
	FreeData();
	
	HRESULT hr = pCollection->_NewEnum(&pUnk);
	if ((hr == S_OK) && (pUnk != NULL))
	{
		hr = pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&pEnum);
		if ((hr == S_OK) && (pEnum != NULL))
		{
			InitData(PropertyNumber);
			pEnum->Reset();
			hrres = S_OK;
			for (int i = 0; i < PropertyNumber; i++)
			{
				hr = pEnum->Next(1, pVar, NULL);
				if((hr == S_OK) && (pVar->vt == VT_BSTR))
				{
					::SysFreeString(m_pBSTRIds[i]);
					m_pBSTRIds[i] = ::SysAllocString(pVar->bstrVal);
				}
				else
				{
					hrres = E_INVALIDARG;
				}
				::VariantClear(pVar);
			}
			RELEASE(pEnum);
		}
		RELEASE(pUnk);
	}
	
	delete pVar;
	return hrres;
}

void CTraductProp::FreeData()
{

	if (m_nPropNum> 0)
	{
		for (int i = 0; i< m_nPropNum; i++)
		{
			// Intentamos evitar el caché
			::SysFreeString(m_pBSTRData[i]);
			m_pBSTRData[i] = NULL;

			::SysFreeString(m_pBSTRIds[i]);
			m_pBSTRIds[i] = NULL;
		}
		free(m_pBSTRData);
		free(m_pBSTRIds);
		m_nPropNum = 0;
	}

}

void CTraductProp::InitData(long nPropNum)
{
	m_pBSTRData = (BSTR*)malloc(nPropNum*sizeof(BSTR*));
	m_pBSTRIds = (BSTR*)malloc(nPropNum*sizeof(BSTR*));
	m_nPropNum = nPropNum;

	for (int i=0; i<nPropNum; i++)
	{
		m_pBSTRData[i] = NULL;
		m_pBSTRIds[i] = NULL;
	}

}

STDMETHODIMP CTraductProp::AllValues(BSTR newVal)
{

	if (m_nPropNum > 0)
	{
		for (int i = 0; i<m_nPropNum; i++)
		{
			RELEASE_BSTR(m_pBSTRData[i]);
			if (newVal)
			{
				m_pBSTRData[i] = ::SysAllocStringLen(newVal, ::SysStringLen(newVal));
			}
		}
	} 

	return S_OK;
}

STDMETHODIMP CTraductProp::get_Property(BSTR * pVal)
{
	if (m_bstrProperty)
		*pVal = ::SysAllocStringLen(m_bstrProperty, ::SysStringLen(m_bstrProperty));
	else
		*pVal = ::SysAllocString(L"");
	return S_OK;
}

STDMETHODIMP CTraductProp::put_Property(BSTR newVal)
{
	RELEASE_BSTR(m_bstrProperty);
	if (newVal)
		m_bstrProperty = ::SysAllocStringLen(newVal, ::SysStringLen(newVal));
	return S_OK;
}

STDMETHODIMP CTraductProp::get_PropertyNumber(short * pVal)
{
	*pVal = (short) m_nPropNum;
	return S_OK;
}

STDMETHODIMP CTraductProp::TestEqual(VARIANT_BOOL * bRet)
{
	
	*bRet = -1; // Partimos de True

	if (m_nPropNum > 0)
	{
		// Primero comprobamos que los punteros son distintos de nulo
		int i = 0;

		BOOL bNull = true;
		for (i=0;i<(m_nPropNum); i++)
		{
			// Bug 0131881. Queremos que se traten como nulos también aquellos casos 
			// en los que el contenido sea nulo.
			if (m_pBSTRData[i] && *m_pBSTRData[i] != 0)
			{
				bNull = false;
				break;
			}
		}
		if (bNull)
		{
			// Todos son nulos ("")
			return S_OK;
		}
		
		for (i = 0; i<(m_nPropNum-1); i++)
		{
			if (m_pBSTRData[i] && m_pBSTRData[i+1])
			{
				// Habemus datos
				if (wcscmp(m_pBSTRData[i], m_pBSTRData[i+1]) != 0)
				{
					*bRet = 0;  // Un False como una casa
					return S_OK;
				}
			}
			else
			{
				if (m_pBSTRData[i])
				{
					// Si estamos aquí es que i+1 es nulo
					*bRet = 0;
					return S_OK;
				}
				else if (m_pBSTRData[i+1])
				{
					// Si estamos aquí es que i+1 es nulo
					*bRet = 0;
					return S_OK;
				}
				// Si estamos aquí los dos son nulos ("" = "")
				// Continuamos la búsqueda
			}
		}
	}
	else
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CTraductProp::Clone(ITraductProp * * pRetProp)
{
	//HRESULT hr = CoCreateInstance(CLSID_TraductProp, NULL, CLSCTX_INPROC_SERVER, IID_ITraductProp, (void**) pRetProp);
	
	HRESULT hr = E_INVALIDARG;
	if (pRetProp)
	{
		//RELEASE((*pRetProp));
		*pRetProp = NULL;
		CComObject<CTraductProp>  * pNew;
		hr = CComObject<CTraductProp>::CreateInstance(&pNew);
		if (hr == S_OK)
		{
			//CTraductProp * pTP = (CTraductProp*)*pRetProp;
			pNew->FreeData();
			pNew->InitData(m_nPropNum);
			pNew->put_Identifier(m_bstrIdentifier);
			for (int i=0; i<m_nPropNum; i++)
			{
				RELEASE_BSTR(pNew->m_pBSTRData[i]);
				if (m_pBSTRData[i])
					pNew->m_pBSTRData[i] = ::SysAllocStringLen(m_pBSTRData[i], ::SysStringLen(m_pBSTRData[i]));
				RELEASE_BSTR(pNew->m_pBSTRIds[i]);
				if (m_pBSTRIds[i])
					pNew->m_pBSTRIds[i] = ::SysAllocStringLen(m_pBSTRIds[i], ::SysStringLen(m_pBSTRIds[i]));
			}
			pNew->put_Property(m_bstrProperty);
			pNew->QueryInterface(IID_ITraductProp, (void**)pRetProp);
		}
	}

	return hr;
}

HRESULT CTraductProp::_get_ValueByLng(VARIANT vLangID, BSTR * pVal, bool & ao_bNullValue)
{
	HRESULT hr = E_INVALIDARG;
	
	if (m_nPropNum > 0)
	{
		if ((vLangID.vt == VT_BSTR) && (vLangID.bstrVal))
		{
			for(int i=0; i<m_nPropNum;i++)
			{
				if (wcscmp(vLangID.bstrVal, m_pBSTRIds[i]) == 0)
				{
					hr = GetValue(i, pVal, ao_bNullValue);
				}
			}	
		}
		else if(vLangID.vt == VT_I4)
		{
			hr = GetValue((int)vLangID.lVal-1, pVal, ao_bNullValue);						
		}
		else if(vLangID.vt == VT_I2)
		{
			hr = GetValue((int)vLangID.iVal-1, pVal, ao_bNullValue);						
		}
		else if(vLangID.vt == VT_UI1)
		{
			hr = GetValue((int)vLangID.bVal-1, pVal, ao_bNullValue);
		}
	}
	return hr;
}


STDMETHODIMP CTraductProp::get_ValueByLng(VARIANT vLangID, BSTR * pVal)
{
	bool bNullValue = false;
	return _get_ValueByLng(vLangID, pVal, bNullValue);
}

STDMETHODIMP CTraductProp::get_ValueByLng(VARIANT vLangID, BSTR * pVal, bool & ao_bNullValue)
{
	return _get_ValueByLng(vLangID, pVal, ao_bNullValue);
}

STDMETHODIMP CTraductProp::put_ValueByLng(VARIANT vLangID, BSTR newVal)
{
	HRESULT hr = E_INVALIDARG;

	if (m_nPropNum > 0)
	{
		if (vLangID.vt == VT_BSTR)
		{
			for(int i=0; i<m_nPropNum;i++)
			{
				if (wcscmp(vLangID.bstrVal, m_pBSTRIds[i]) == 0)
				{
					hr = PutValue(i, &newVal);
					break;
				}
			}	
		}
		else if(vLangID.vt == VT_I4)
		{
			hr = PutValue((int)vLangID.lVal-1, &newVal);
		}
		else if(vLangID.vt == VT_I2)
		{
			hr = PutValue((int)vLangID.iVal-1, &newVal);
		}
		else if(vLangID.vt == VT_UI1)
		{
			hr = PutValue((int)vLangID.bVal-1, &newVal);
		}
	}

	return S_OK;
}

HRESULT CTraductProp::GetValue(int i, BSTR * pVal, bool & ao_bNullValue)
{
	HRESULT hr = S_OK;
	ao_bNullValue = false;

	// Entramos con el entorno 0-(m_nPropNum-1)
	if ((m_nPropNum > i) && (i >= 0))
	{
		if (m_pBSTRData[i])
		{
			*pVal = ::SysAllocStringLen(m_pBSTRData[i], ::SysStringLen(m_pBSTRData[i]));
			ao_bNullValue = false;
		}
		else
		{
			*pVal = ::SysAllocString(L"");
			ao_bNullValue = true;
		}
	}
	else
	{
		hr = E_INVALIDARG;
	}
	return hr;

}

HRESULT CTraductProp::PutValue(int i, BSTR * newValue)
{
	HRESULT hr = S_OK;

	// Entramos con el entorno 0-(m_nPropNum-1)
	if ((m_nPropNum > i) && (i >= 0))
	{
		RELEASE_BSTR(m_pBSTRData[i]);
		if (*newValue)
		{
			m_pBSTRData[i] = ::SysAllocStringLen(*newValue, ::SysStringLen(*newValue));
		}
		else
		{
			m_pBSTRData[i] = ::SysAllocString(L"");
		}
	}
	else
	{
		hr = E_INVALIDARG;
	}
	return hr;

}

STDMETHODIMP CTraductProp::get_ID(long nIndex, BSTR * pVal)
{

	if ((m_nPropNum >= nIndex) && (nIndex > 0))
	{
		if (m_pBSTRIds[nIndex-1])
			*pVal = ::SysAllocStringLen(m_pBSTRIds[nIndex-1], ::SysStringLen(m_pBSTRIds[nIndex-1]));
		else
			*pVal = ::SysAllocString(L"");
	}
	else
		return E_INVALIDARG;

	return S_OK;
}

STDMETHODIMP CTraductProp::InitFromBSTRArray(VARIANT newValue)
{
	if (newValue.vt == (VT_ARRAY | VT_BSTR))
	{
		SAFEARRAY * pArray = newValue.parray;
		long nPropNumber = pArray->rgsabound[0].cElements;
		BSTR * pBstr = (BSTR*)pArray->pvData;
		
		if (nPropNumber > 0)
		{
			InitData(nPropNumber);
			for (long i= 0; i < nPropNumber; i++)
			{
				m_pBSTRIds[i] = ::SysAllocString(pBstr[i]);
			}
		}
		else
			return E_INVALIDARG;

		return S_OK;
	}
	return E_INVALIDARG;	
}

STDMETHODIMP CTraductProp::get_Identifier(BSTR *pVal)
{
	if (m_bstrIdentifier)
		*pVal = SysAllocString(m_bstrIdentifier);
	else
		*pVal = SysAllocString(L"");

	return S_OK;
}

STDMETHODIMP CTraductProp::put_Identifier(BSTR newVal)
{
	RELEASE_BSTR(m_bstrIdentifier);
	if (newVal)
		m_bstrIdentifier = SysAllocString(newVal);

	return S_OK;
}

STDMETHODIMP CTraductProp::CloneClean(ITraductProp **pRetProp)
{
	HRESULT hr = E_INVALIDARG;
	if (pRetProp)
	{
		*pRetProp = NULL;
		CComObject<CTraductProp>  * pNew;
		hr = CComObject<CTraductProp>::CreateInstance(&pNew);
		if (hr == S_OK)
		{
			pNew->FreeData();
			pNew->InitData(m_nPropNum);
			for (int i=0; i<m_nPropNum; i++)
			{
				RELEASE_BSTR(pNew->m_pBSTRData[i]);
				if (m_pBSTRData[i])
					pNew->m_pBSTRData[i] = ::SysAllocStringLen(m_pBSTRData[i], ::SysStringLen(m_pBSTRData[i]));
				RELEASE_BSTR(pNew->m_pBSTRIds[i]);
				if (m_pBSTRIds[i])
					pNew->m_pBSTRIds[i] = ::SysAllocStringLen(m_pBSTRIds[i], ::SysStringLen(m_pBSTRIds[i]));
			}
			pNew->put_Property(m_bstrProperty);
			pNew->QueryInterface(IID_ITraductProp, (void**)pRetProp);
		}
	}

	return hr;
}

STDMETHODIMP CTraductProp::ResetIdentifier()
{
	USES_CONVERSION;
	
	char * pNew =NULL;
	
	GetUniqueRef(&pNew);
	
	RELEASE_BSTR(m_bstrIdentifier);
	m_bstrIdentifier = M4CppToVB(pNew);
	free(pNew);

	return S_OK;
}
