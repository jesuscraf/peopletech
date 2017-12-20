// M4General.cpp : Implementation of CM4General
#include "stdafx.h"
#include "m4apie.h"
#include "m4general.h"
#include "afxinet.h"

EXTERN_C const CLSID CLSID_CPropBag;

#ifdef __cplusplus

class DECLSPEC_UUID("13313659-2584-11D2-969E-00C04FC2444B")
CPropBag;
#endif

/////////////////////////////////////////////////////////////////////////////
// CM4General

BOOL CALLBACK EnumResNameProc( HMODULE hModule, LPCTSTR lpszType, LPTSTR lpszName, LONG_PTR lParam)
{
	USES_CONVERSION;
	
	ICPropBag *myCollection;
	//lParam debe ser tipo Collection

	myCollection = (ICPropBag *)lParam;
	
	if (myCollection == NULL) return false;

	/*VARIANT vAfter, vBefore;
	::VariantInit(&vAfter);
	::VariantInit(&vBefore);
	//CComVariant vItem(lpszName), vKey(lpszName);
	VARIANT vItem, vKey;
	::VariantInit(&vItem);
	::VariantInit(&vKey);

	vItem.vt = VT_BSTR;
	vKey.vt = VT_BSTR;
	vKey.bstrVal = ::SysAllocString(lpszName);*/

	HRESULT hr = myCollection->AddProperty(A2OLE(lpszName),CComVariant(lpszName));

/*	::VariantClear(&vAfter);
	::VariantClear(&vBefore);
	::VariantClear(&vItem);
	::VariantClear(&vKey);*/

	return true;
}

//##ModelId=396B32970010
STDMETHODIMP CM4General::GetIDsBitmapOfResource(BSTR nameLibrary, ICPropBag * * nameBitmap)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;

	if (nameLibrary)
	{
		if (nameBitmap)
		{
			HRESULT hr = CoCreateInstance(CLSID_CPropBag, NULL, CLSCTX_ALL , IID_ICPropBag, (void**)nameBitmap);
			HINSTANCE handleLibrary = LoadLibrary(OLE2A(nameLibrary));
			if ((handleLibrary != 0) && (hr == S_OK))
			{
				EnumResourceNames(handleLibrary, RT_BITMAP, &EnumResNameProc, (long)*nameBitmap);
				FreeLibrary(handleLibrary);
				return S_OK;
			}
			return E_INVALIDARG;
		}
		return E_INVALIDARG;

	}
	return E_INVALIDARG;
}
