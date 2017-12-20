
#include "stdafx.h"
#include "m4chwrap.h"
#include "iwrapfilter.h"
#include "wrapdef.h"
#include "rerror.h"
#include "regi_smp.hpp"
#include "wores.hpp"
#include "m4objlog.hpp"

/////////////////////////////////////////////////////////////////////////////
// CIWrapFilter

STDMETHODIMP CIWrapFilter::get_coArgument(IWrapColArgument ** pVal)
{
	ENTER_METHOD(CIWrapFilter::get_coArgument);

	// Just cast this and return a pointer to
	// the RgItFilter
	// Add one to the reference counts...
	// of the base class...
	*pVal = (IWrapColArgument*) this;

	TRACE("Casting Filter to a collection of Arguments");

	((IUnknown*)*pVal)->AddRef();

	LEAVE_METHOD(CIWrapFilter::get_coArgument, S_OK);
}

STDMETHODIMP CIWrapFilter::Refresh(short * ao_RetVal)
{
	ENTER_METHOD(CIWrapFilter::Refresh);

    // Bug 0113644 no implementado todavía
	DUMP_CHLOG_ERRORF( M4_CH_WO_NOT_SUPPORTED_FUNCTION, "CIWrapFilter::Refresh" << "M4ChWrap.dll" );

	LEAVE_METHOD(CIWrapFilter::Refresh, S_OK);
}

STDMETHODIMP CIWrapFilter::Delete (short * ao_RetVal)
{
	ENTER_METHOD(CIWrapFilter::Delete);

    *ao_RetVal = m_pfilter->Delete();

    TRACE2("Returning %d", *ao_RetVal);

	LEAVE_METHOD(CIWrapFilter::Delete, S_OK);
}

STDMETHODIMP CIWrapFilter::get_Name (BSTR * pVal)
{
	ENTER_METHOD(CIWrapFilter::get_Name);

    m4VariantType v;
    m4return_t res = m_pfilter->GetName (v);

    if (res == M4_SUCCESS) {
	    gstring tmpGstring (v.Data.PointerChar);

		*pVal = (BSTR)tmpGstring;
		
		LEAVE_METHOD(CIWrapFilter::get_Name, S_OK);
	}
	else {
		LEAVE_METHOD(CIWrapFilter::get_Name, E_UNEXPECTED);
	}

	LEAVE_METHOD(CIWrapFilter::get_Name, S_OK);
}

//----------------------------------------------------------------------------

CIWrapFilter::CIWrapFilter ()
{
    m_pfilter = 0;
}

void CIWrapFilter::Init (_ClRegisterFilter *ai_pfilter)
{
    m_pfilter = ai_pfilter;
    CIWrapColArgument::Init (&( (*m_pfilter)->Arguments () ));
}

