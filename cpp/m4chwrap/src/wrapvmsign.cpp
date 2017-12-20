//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapvmsign.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             27/6/98
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//     Implementation of CWrapVMSign
//
//==============================================================================

#include "stdafx.h"
#include "m4chwrap.h"
#include "wrapvmsign.h"
#include "wrapdef.h"
#include "gstring.h"
#include "cldmcontext.hpp"


/////////////////////////////////////////////////////////////////////////////
// CWrapVMSign

STDMETHODIMP CWrapVMSign::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = { &IID_IWrapVMSign };

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
		{
			return S_OK;
		}
	}
	return S_FALSE;
}

STDMETHODIMP CWrapVMSign::get_System(long* pVal)
{
	ENTER_METHOD(CWrapVMSign::get_System);

	//no se está usando
	//m_poClVMSign->GetSystem(*pVal);
	*pVal=0;

	TRACE2("Returning System = %d", *pVal);
	LEAVE_METHOD(CWrapVMSign::get_System, S_OK);
}

STDMETHODIMP CWrapVMSign::get_VMTypeID(VMTYPE* pVal)
{
	ENTER_METHOD(CWrapVMSign::get_VMTypeID);

	if (m_poDMContext->GetCSType() == M4CL_CSTYPE_BACK)
	{
		*pVal = VMTYPE(M4CL_VMTYPE_SERVER);
	}
	else{
		if (m_poDMContext->IsSM() )
		{
			*pVal = VMTYPE(M4CL_VMTYPE_EMULATOR);			
		}
		else
		{
			*pVal = VMTYPE(M4CL_VMTYPE_CLIENT);
		}
	}

	ATLTRACE("VM type ID = %d\n", *pVal);

	LEAVE_METHOD(CWrapVMSign::get_VMTypeID, S_OK);
}

STDMETHODIMP CWrapVMSign::get_VMCapacityID(long* pVal)
{
	ENTER_METHOD(CWrapVMSign::get_VMCapacityID);

	//no se está usando
	//m_poClVMSign->GetVMCapacity(*pVal);
	*pVal=0;

	TRACE2("Returning VM capacity ID = %d", *pVal);
	LEAVE_METHOD(CWrapVMSign::get_VMCapacityID, S_OK);
}

STDMETHODIMP CWrapVMSign::get_VMCapacityName(BSTR* pVal)
{
	ENTER_METHOD(CWrapVMSign::get_VMCapacityName);

	//m4pcchar_t psz;

	//no se está usando
	//m_poClVMSign->GetVMCapacity(&psz);

	gstring gstr("");
	*pVal = gstr;

	TRACE2("Returning VM capacity name = %s", "");
	LEAVE_METHOD(CWrapVMSign::get_VMCapacityName, S_OK);
}

STDMETHODIMP CWrapVMSign::get_CSType(CSTYPE* pVal)
{
	ENTER_METHOD(CWrapVMSign::get_CSType);
	
	*pVal=CSTYPE(m_poDMContext->GetCSType());

	LEAVE_METHOD(CWrapVMSign::get_CSType, S_OK);
}

STDMETHODIMP CWrapVMSign::get_NumberOfServices(long* pVal)
{
	ENTER_METHOD(CWrapVMSign::get_NumberOfServices);

	//no se está usando
	//*pVal = m_poClVMSign->GetNumberOfServices();
	*pVal=0;

	TRACE2("Returning number of services = %d", *pVal);
	LEAVE_METHOD(CWrapVMSign::get_NumberOfServices, S_OK);
}

STDMETHODIMP CWrapVMSign::GetServiceIDByPos(long ai_pos, long* ao_SvcId, short* ao_RetVal)
{
	ENTER_METHOD(CWrapVMSign::GetServiceIDByPos);

	//no se está usando
	//*ao_RetVal = m_poClVMSign->GetServiceByPos(ai_pos, *ao_SvcId);
	*ao_SvcId=0;

	TRACE2("Returning %d", *ao_RetVal);
	LEAVE_METHOD(CWrapVMSign::GetServiceIDByPos, S_OK);
}

STDMETHODIMP CWrapVMSign::GetServiceNameByID(long ai_id, BSTR* ao_SvcNam, short* ao_RetVal)
{
	ENTER_METHOD(CWrapVMSign::GetServiceNameByID);

	//m4pcchar_t psz;

	//no se está usando
	//*ao_RetVal = m_poClVMSign->GetServiceNameById(ai_id, &psz);

	gstring gstr("");
	*ao_SvcNam = gstr;

	TRACE3("Service name %s Returning %d", "", *ao_RetVal);
	LEAVE_METHOD(CWrapVMSign::GetServiceNameByID, S_OK);
}

STDMETHODIMP CWrapVMSign::GetIsLocalServiceByID(long ai_pos, VARIANT_BOOL* ao_fIsLocal, short* ao_RetVal)
{
	ENTER_METHOD(CWrapVMSign::GetIsLocalServiceByID);

	//m4bool_t fIsLocal;

	//no se está usando
	//*ao_RetVal = m_poClVMSign->AskIfIsLocalService(ai_pos, fIsLocal);

	//if (*ao_RetVal == M4_SUCCESS)
	//{
	//	*ao_fIsLocal = fIsLocal ? VARIANT_TRUE : VARIANT_FALSE;
	//}

	*ao_fIsLocal = VARIANT_TRUE;

	TRACE2("Returning %d", *ao_RetVal);
	LEAVE_METHOD(CWrapVMSign::GetIsLocalServiceByID, S_OK);
}
