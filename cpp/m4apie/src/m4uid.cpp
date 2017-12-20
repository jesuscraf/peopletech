// M4UID.cpp : Implementation of CM4UID
#include "stdafx.h"
#include "m4apie.h"
#include "m4uid.h"

/////////////////////////////////////////////////////////////////////////////
// CM4UID


//##ModelId=396B3294038D
STDMETHODIMP CM4UID::GenerateNewUID()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	CoCreateGuid(&m_guid);
	return S_OK;
}

//##ModelId=396B32940397
STDMETHODIMP CM4UID::get_M4UID(BSTR * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	::SysFreeString(*pVal);
	*pVal = ::SysAllocStringLen(NULL, 38);
	StringFromGUID2(m_guid, *pVal, 39);

	return S_OK;
}

//##ModelId=396B329403A2
STDMETHODIMP CM4UID::ReserveGUIDs(long nNumber)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	// TODO: Add your implementation code here
	long n;
	GUID guid;

	for (n=0; n< nNumber; n++)
	{
		CoCreateGuid(&guid);
	}
	return S_OK;
}

//##ModelId=396B329403A5
STDMETHODIMP CM4UID::NumberOfRefs(IDispatch ** oObject, long * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here

	long RefNumber = 0; 
	if (*oObject != NULL)
	{
		RefNumber = (*oObject)->AddRef();
		RefNumber = (*oObject)->Release();
	}

	*pVal = RefNumber;

	return S_OK;
}
