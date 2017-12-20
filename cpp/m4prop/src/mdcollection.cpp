// MDCollection.cpp : Implementation of CMDCollection
#include "stdafx.h"
#include "m4prop.h"
#include "mdcollection.h"

/////////////////////////////////////////////////////////////////////////////
// CMDCollection


STDMETHODIMP CMDCollection::IsPresent(VARIANT vIndex, VARIANT_BOOL * pretVal)
{
	// TODO: Add your implementation code here
	IMDObject *pDefProp;

	HRESULT hr = get_Item(vIndex, &pDefProp);
	*pretVal = 0;   // False
	if (SUCCEEDED(hr))
	{
		*pretVal = -1;  // True
		// Hay que liberar la referencia que estamos pillando
		RELEASE(pDefProp);
	}
	return S_OK;
}
