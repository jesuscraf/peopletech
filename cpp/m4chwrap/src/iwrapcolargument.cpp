
#include "wores.hpp"
#include "stdafx.h"
#include "m4chwrap.h"
#include "iwrapcolargument.h"
#include "wrapdef.h"
#include "rerror.h"
#include "clargume.hpp"


/////////////////////////////////////////////////////////////////////////////
// CIWrapColArgument

STDMETHODIMP CIWrapColArgument::get_Count (long* ao_iCount)
{
	ENTER_METHOD(CIWrapColArgument::get_Count);

	*ao_iCount = m_poArgumentList->Count ();

	TRACE2("Returning a count of %d", *ao_iCount);

	LEAVE_METHOD(CIWrapColArgument::get_Count, S_OK);
}

STDMETHODIMP CIWrapColArgument::Item(VARIANT ai_var, IWrapArgument** ao_poArg)
{
	ENTER_METHOD(CIWrapColArgument::Item);

	_AClArgument *parg = 0;

	switch (ai_var.vt)
	{
	case VT_BSTR:
        {
            gstring tmpGstring(ai_var.bstrVal);
		    parg = &(*m_poArgumentList)[tmpGstring];		    
        }
        break;

	case VT_UI1:
		parg = &(*m_poArgumentList)[ai_var.bVal];
		break;

	case VT_I2:
        parg = &(*m_poArgumentList)[ai_var.iVal];
		break;

	case VT_I4:
        parg = &(*m_poArgumentList)[ai_var.lVal];
		break;

    default:
		FormatError(M4_CH_WO_INVALID_GET_ITEM, "Cannot get argument given variant type %d",	ai_var.vt);
		LEAVE_METHOD(CIWrapColArgument::Item, E_UNEXPECTED);
		break;
    }

    *ao_poArg = this;

    ((CIWrapArgument *)(*ao_poArg))->Init (parg);
	((IUnknown*)*ao_poArg)->AddRef();

    LEAVE_METHOD(CIWrapColArgument::Item, S_OK);
}

//----------------------------------------------------------------------------

CIWrapColArgument::CIWrapColArgument()
{
    m_poArgumentList = 0;
}

void CIWrapColArgument::Init (_ClArgumentList_Base *ai_pargs)
{
    m_poArgumentList = (ClArgumentList *)ai_pargs;
}
