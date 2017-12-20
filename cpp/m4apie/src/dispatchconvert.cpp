// DispatchConvert.cpp : Implementation of CDispatchConvert
#include "stdafx.h"
#include "m4apie.h"
#include "dispatchconvert.h"

/////////////////////////////////////////////////////////////////////////////
// CDispatchConvert


//##ModelId=396B3295010E
STDMETHODIMP CDispatchConvert::Object2Long(IDispatch * Object, long * Value)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// Función directa
	*Value = (long)Object;

	return S_OK;
}

//##ModelId=396B3295011B
STDMETHODIMP CDispatchConvert::Long2Object(long * Value, IDispatch ** Object)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// Función Inversa
	*Object = (IDispatch*) *Value;

	return S_OK;
}
