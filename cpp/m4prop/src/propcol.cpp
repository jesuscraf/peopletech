// PropCol.cpp : Implementation of CPropCol
#include "stdafx.h"
#include "m4prop.h"
#include "propcol.h"


/////////////////////////////////////////////////////////////////////////////
// CPropCol


STDMETHODIMP CPropCol::AddProperty(BSTR strName, VARIANT vValue, VARIANT_BOOL bEditable, 
								   M4PropertyType nType, VARIANT vDefValue)
{
	// TODO: Add your implementation code here

	VARIANT v;
	ICProperty *pProp;

	VariantInit(&v);
	v.vt = VT_BSTR;
	v.bstrVal = ::SysAllocString(strName);
	HRESULT hr = get_Item(v, &pProp);
	if (SUCCEEDED(hr))
	{
		pProp->put_Value(vValue);
		pProp->put_Type(nType);
		if (vDefValue.vt == VT_ERROR)
		{
			// Hacemos un Clear del Default Value
			VariantClear(&vDefValue);
		}
		pProp->put_DefValue(vDefValue);
		VariantClear(&v);
	}
	// Creamos una propiedad nueva
	CoCreateInstance(CLSID_CProperty, NULL, CLSCTX_INPROC_SERVER, IID_ICProperty, (void**)&pProp);
	pProp->put_Name(strName);
	pProp->put_Value(vValue);
	pProp->put_Type(nType);
	pProp->put_Editable(bEditable);
	if (vDefValue.vt == VT_ERROR)
	{
		// Hacemos un Clear del Default Value
		VariantClear(&vDefValue);
	}
	pProp->put_DefValue(vDefValue);
	hr = Add(pProp, v);
	VariantClear(&v);
	RELEASE(pProp);
	return hr;

}

STDMETHODIMP CPropCol::get_Property(VARIANT nIndex, VARIANT vDefValue, ICProperty ** pVal)
{
	// TODO: Add your implementation code here
	ICProperty * pProp;

	VARIANT v;
	HRESULT hr;

	if (nIndex.vt !=  VT_BSTR)
	{
		VariantInit(&v);
		v.vt = VT_BSTR;
		v.bstrVal = ::SysAllocString(nIndex.bstrVal);
		hr = get_Item(v, &pProp);
	}
	else
	{
		hr = get_Item(nIndex, &pProp);
	}
	if (SUCCEEDED(hr))
	{
		*pVal = pProp;
		return S_OK;
	}
	CoCreateInstance(CLSID_CProperty, NULL, CLSCTX_INPROC_SERVER, IID_ICProperty, (void**)&pProp);
	pProp->put_Name(nIndex.bstrVal);
	pProp->put_Value(vDefValue);
	pProp->put_Type(FE_NOTDEFINED);
	pProp->put_Editable(0);
	*pVal = pProp;
	return S_OK;		

}
