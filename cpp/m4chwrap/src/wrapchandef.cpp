//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapchandef.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//     Implementation of CWrapChanDef
//
//==============================================================================

#include "stdafx.h"
#include "m4chwrap.h"
#include "wrapchandef.h"
#include "chandef.hpp"
#include "wrapdef.h"
#include "gstring.h"

/////////////////////////////////////////////////////////////////////////////
// CWrapChanDef
CWrapChanDef::CWrapChanDef()
{
	m_poChanDef = NULL;
}

CWrapChanDef::~CWrapChanDef()
{
	m_poChanDef = NULL;
}

void CWrapChanDef::Init(ClChannelDef* ai_poChanDef)
{
	m_poChanDef = ai_poChanDef;
}


STDMETHODIMP CWrapChanDef::get_Id(BSTR* pVal)
{
	ENTER_METHOD(CWrapChanDef::get_Id);

	gstring tmpGstring(m_poChanDef->Id());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;

		LEAVE_METHOD(CWrapChanDef::get_Id, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapChanDef::get_Id, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapChanDef::get_Name(BSTR* pVal)
{
	ENTER_METHOD(CWrapChanDef::get_Name);

	gstring tmpGstring(m_poChanDef->Name());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;

		LEAVE_METHOD(CWrapChanDef::get_Name, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapChanDef::get_Name, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapChanDef::get_NumberOfInstances(short* pVal)
{
	ENTER_METHOD(CWrapNodeDef::NumberOfInstances);

	*pVal = m_poChanDef->NumberOfInstances();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapNodeDef::NumberOfInstances, S_OK);
}

STDMETHODIMP CWrapChanDef::get_GetInstanceIdByPosition(short ai_iPosition, BSTR* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_GetInstanceIdByPosition);

	gstring tmpGstring(m_poChanDef->GetInstanceIdByPosition(ai_iPosition));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;

		LEAVE_METHOD(CWrapChanDef::get_GetInstanceIdByPosition, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapChanDef::get_GetInstanceIdByPosition, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapChanDef::get_GetInstanceT3ByPosition(short ai_iPosition, BSTR* pVal)
{
	ENTER_METHOD(CWrapNodeDef::get_GetInstanceT3ByPosition);

	gstring tmpGstring(m_poChanDef->GetInstanceT3ByPosition(ai_iPosition));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;

		LEAVE_METHOD(CWrapChanDef::get_GetInstanceT3ByPosition, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapChanDef::get_GetInstanceT3ByPosition, E_UNEXPECTED);
	}

}

STDMETHODIMP CWrapChanDef::get_NumberOfRoots(short* pVal)
{
	ENTER_METHOD(CWrapChanDef::get_NumberOfRoots);

	*pVal = m_poChanDef->NumberOfRoots();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_NumberOfRoots, S_OK);
}

STDMETHODIMP CWrapChanDef::get_RootIdByPosition(short ai_pos, BSTR* pVal)
{
	ENTER_METHOD(CWrapChanDef::get_RootIdByPosition);

	gstring tmpGstring(m_poChanDef->RootIdByPosition(ai_pos));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;

		LEAVE_METHOD(CWrapChanDef::get_RootIdByPosition, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapChanDef::get_RootIdByPosition, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapChanDef::get_Type(short * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_Type);

	*pVal = m_poChanDef->Type();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_Type, S_OK);
}

STDMETHODIMP CWrapChanDef::get_TimeUnit(short * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_TimeUnit);

	*pVal = m_poChanDef->TimeUnit();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_TimeUnit, S_OK);
}

STDMETHODIMP CWrapChanDef::get_Build(short * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_Build);

	*pVal = m_poChanDef->Build();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_Build, S_OK);
}

STDMETHODIMP CWrapChanDef::get_CsType(short * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_CsType);

	*pVal = m_poChanDef->CsType();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_CsType, S_OK);
}

STDMETHODIMP CWrapChanDef::get_IsSeparable(short * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_IsSeparable);

	*pVal = m_poChanDef->IsSeparable();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_IsSeparable, S_OK);
}

STDMETHODIMP CWrapChanDef::get_Version(double * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_Version);

	*pVal = m_poChanDef->Version();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_Version, S_OK);
}

STDMETHODIMP CWrapChanDef::get_CsExeType(short * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_CsExeType);

	*pVal = m_poChanDef->CsExeType();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_CsExeType, S_OK);
}

STDMETHODIMP CWrapChanDef::get_VMCapacityReq(short * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_VMCapacityReq);

	*pVal = m_poChanDef->VMCapacityReq();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_VMCapacityReq, S_OK);
}

STDMETHODIMP CWrapChanDef::get_SecRead(short * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_SecRead);

	*pVal = m_poChanDef->SecRead();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_SecRead, S_OK);
}

STDMETHODIMP CWrapChanDef::get_SecWrite(short * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_SecWrite);

	*pVal = m_poChanDef->SecWrite();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_SecWrite, S_OK);
}

STDMETHODIMP CWrapChanDef::get_SecDelete(short * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_SecDelete);

	*pVal = m_poChanDef->SecDelete();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_SecDelete, S_OK);
}

STDMETHODIMP CWrapChanDef::get_SecUpdate(short * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_SecUpdate);

	*pVal = m_poChanDef->SecUpdate();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_SecUpdate, S_OK);
}

STDMETHODIMP CWrapChanDef::get_SecExecute(short * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_SecExecute);

	*pVal = m_poChanDef->SecExecute();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_SecExecute, S_OK);
}

STDMETHODIMP CWrapChanDef::get_InstanceScopeByPosition(short ai_iPosition, short * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_InstanceScopeByPosition);

	*pVal = m_poChanDef->GetInstanceScopeByPosition( ai_iPosition ) ;

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_InstanceScopeByPosition, S_OK);
}

STDMETHODIMP CWrapChanDef::get_InstanceOpenModeByPosition(short ai_iPosition, short * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_InstanceOpenModeByPosition);

	*pVal = m_poChanDef->GetInstanceOpenModeByPosition( ai_iPosition ) ;

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_InstanceOpenModeByPosition, S_OK);
}

STDMETHODIMP CWrapChanDef::get_InstanceCsAccessByPosition(short ai_iPosition, short * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_InstanceCsAccessByPosition);

	*pVal = m_poChanDef->GetInstanceCsAccessByPosition( ai_iPosition ) ;

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_InstanceCsAccessByPosition, S_OK);
}

STDMETHODIMP CWrapChanDef::get_IsExternal(BYTE *pVal)
{
	ENTER_METHOD(CWrapChanDef::get_IsExternal);

	*pVal = m_poChanDef->IsExternal();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_IsExternal, S_OK);
}

STDMETHODIMP CWrapChanDef::get_OleClassGuid(BSTR *pVal)
{
	ENTER_METHOD(CWrapChanDef::get_OleClassGuid);

	gstring tmpGstring(m_poChanDef->OleClassId());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;

		LEAVE_METHOD(CWrapChanDef::get_OleClassGuid, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapChanDef::get_OleClassGuid, E_UNEXPECTED);
	}
}

///////////////////////////////////////////////////////////////////////////////
//
// ColNodeDef Interface
//

STDMETHODIMP CWrapChanDef::get_ColNodeDef(IWrapColNodeDef** pVal)
{
	ENTER_METHOD(CWrapChanDef::get_ColNodeDef);

	// Just cast this and return a pointer to
	// the RgItFilter
	// Add one to the reference counts...
	// of the base class...
	*pVal = (IWrapColNodeDef*) this;

	TRACE("Casting Access to a collection of nodes");

	((IUnknown*)*pVal)->AddRef();

	LEAVE_METHOD(CWrapChanDef::get_ColNodeDef, S_OK);
}

STDMETHODIMP CWrapChanDef::get_HasSecurity(short * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_HasSecurity);

	*pVal = m_poChanDef->HasSecurity();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_HasSecurity, S_OK);
}

STDMETHODIMP CWrapChanDef::get_EnvironmentVars(long * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_EnvironmentVars);

	*pVal = m_poChanDef->EnvironmentVars();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_EnvironmentVars, S_OK);
}


STDMETHODIMP CWrapChanDef::get_ServiceId(BSTR * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_ServiceId);

	gstring tmpGstring(m_poChanDef->ServiceId());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;

		LEAVE_METHOD(CWrapChanDef::get_ServiceId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapChanDef::get_ServiceId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapChanDef::get_ServiceAltId(BSTR * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_ServiceAltId);

	gstring tmpGstring(m_poChanDef->ServiceAltId());

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;

		LEAVE_METHOD(CWrapChanDef::get_ServiceAltId, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapChanDef::get_ServiceAltId, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapChanDef::get_IsCacheable(short * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_IsCacheable);

	*pVal = m_poChanDef->IsCacheable();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_IsCacheable, S_OK);
}

STDMETHODIMP CWrapChanDef::get_CacheMaxPeriod(double * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_CacheMaxPeriod);

	*pVal = m_poChanDef->CacheMaxPeriod();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_CacheMaxPeriod, S_OK);
}

STDMETHODIMP CWrapChanDef::get_NumberOfParameters(short * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_NumberOfParameters);

	*pVal = m_poChanDef->NumberOfParameters();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_NumberOfParameters, S_OK);
}

STDMETHODIMP CWrapChanDef::get_ParameterNode(short ai_iPosition, BSTR * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_ParameterNode);

	gstring tmpGstring(m_poChanDef->ParameterNode(ai_iPosition));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;

		LEAVE_METHOD(CWrapChanDef::get_ParameterNode, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapChanDef::get_ParameterNode, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapChanDef::get_ParameterItem(short ai_iPosition, BSTR * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_ParameterItem);

	gstring tmpGstring(m_poChanDef->ParameterItem(ai_iPosition));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;

		LEAVE_METHOD(CWrapChanDef::get_ParameterItem, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapChanDef::get_ParameterItem, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapChanDef::get_InstanceRSMByPosition(short ai_iPosition, BSTR * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_InstanceRSMByPosition);

	gstring tmpGstring(m_poChanDef->GetInstanceRoleByPosition(ai_iPosition));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;

		LEAVE_METHOD(CWrapChanDef::get_InstanceRSMByPosition, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapChanDef::get_InstanceRSMByPosition, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapChanDef::get_IsMDCacheable(short * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_IsMDCacheable);

	*pVal = m_poChanDef->IsMDCacheable();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_IsMDCacheable, S_OK);
}

STDMETHODIMP CWrapChanDef::get_OwnerFlag(long * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_OwnerFlag);

	*pVal = m_poChanDef->OwnerFlag();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_OwnerFlag, S_OK);
}

STDMETHODIMP CWrapChanDef::get_CheckConcurrency(short * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_CheckConcurrency);

	*pVal = m_poChanDef->CheckConcurrency();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_CheckConcurrency, S_OK);
}

STDMETHODIMP CWrapChanDef::get_InstanceOrganizationByPosition(short ai_iPosition, BSTR * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_InstanceOrganizationByPosition);

	gstring tmpGstring(m_poChanDef->GetInstanceOrganizationByPosition(ai_iPosition));

	if (tmpGstring)
	{
		*pVal = (BSTR)tmpGstring;

		LEAVE_METHOD(CWrapChanDef::get_InstanceOrganizationByPosition, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapChanDef::get_InstanceOrganizationByPosition, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapChanDef::get_InstanceOrgTypeByPosition(short ai_iPosition, short * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_InstanceOrgTypeByPosition);

	*pVal = m_poChanDef->GetInstanceOrganizationTypeByPosition( ai_iPosition ) ;

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_InstanceOrgTypeByPosition, S_OK);
}

STDMETHODIMP CWrapChanDef::get_OrgType(short * pVal)
{
	ENTER_METHOD(CWrapChanDef::get_OrgType);

	*pVal = m_poChanDef->OrganizationType();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_OrgType, S_OK);
}

STDMETHODIMP CWrapChanDef::get_InheritLevel(short *pVal)
{
	ENTER_METHOD(CWrapChanDef::get_InheritLevel);

	*pVal = m_poChanDef->InheritLevel();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_InheritLevel, S_OK);
}

STDMETHODIMP CWrapChanDef::get_CreationType(short *pVal)
{
	ENTER_METHOD(CWrapChanDef::get_CreationType);

	*pVal = m_poChanDef->CreationType();

	TRACE2("Returning %u", *pVal);
	LEAVE_METHOD(CWrapChanDef::get_CreationType, S_OK);
}
