
#include "wores.hpp"
#include "stdafx.h"
#include "m4chwrap.h"
#include "iwrapargument.h"
#include "wrapdef.h"
#include "rerror.h"
#include "clargume.hpp"



/////////////////////////////////////////////////////////////////////////////
// CIWrapArgument

/*
STDMETHODIMP CIWrapArgument::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = { &IID_IWrapArgument };

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
		{
			return S_OK;
		}
	}
	return S_FALSE;
}
*/

STDMETHODIMP CIWrapArgument::get_Value(VARIANT * ao_poVariant)
{
	m4VariantType var;

	ENTER_METHOD(CIWrapArgument::get_Value);

	m_poArg->Get(var);

	var.GetVARIANT(*ao_poVariant);
	
	//
	// For commentary on this see cvadte.cpp
	//
#ifdef _ZERO_DATE_IS_ERR
	
	if (ao_poVariant->vt == VT_DATE && !ao_poVariant->date)
	{
		FormatError(M4_CH_WO_INVALID_DATE, "Invalid date");
		LEAVE_METHOD(CIWrapArgument::get_Value, E_UNEXPECTED);
	}
#endif
	
	LEAVE_METHOD(CIWrapArgument::get_Value, S_OK);
}

STDMETHODIMP CIWrapArgument::put_Value(VARIANT ai_oVariant)
{
	ENTER_METHOD(CIWrapArgument::put_Value);

	m4VariantType var(ai_oVariant);

	if (m_poArg->Set(var) == M4_ERROR)
	{
		LEAVE_METHOD(CIWrapArgument::put_Value, E_UNEXPECTED);
	}
	else
	{
		LEAVE_METHOD(CIWrapArgument::put_Value, S_OK);
	}
}

STDMETHODIMP CIWrapArgument::get_ArgumentName (BSTR * pVal)
{
	ENTER_METHOD(CIWrapArgument::get_ArgumentName);

    m4pchar_t pNameArg = 0;
    m_poArg->GetName (pNameArg);

    if (pNameArg)
	{
	    gstring tmpGstring (pNameArg);

		*pVal = (BSTR)tmpGstring;
		
		LEAVE_METHOD(CIWrapArgument::get_ArgumentName, S_OK);
	}
	else
	{
		LEAVE_METHOD(CIWrapArgument::get_ArgumentName, E_UNEXPECTED);
	}

	LEAVE_METHOD(CIWrapArgument::get_ArgumentName, S_OK);
}

//----------------------------------------------------------------------------

CIWrapArgument::CIWrapArgument ()
{
    m_poArg = 0;//&ClArgumentList::sNoArgs;
}

void CIWrapArgument::Init (_AClArgument *ai_poarg)
{
    m_poArg = ai_poarg;
}
