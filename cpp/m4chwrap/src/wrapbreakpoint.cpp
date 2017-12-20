//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             iwraprgit.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//     Implementation of ClIWrapExecutionPoint
//
//==============================================================================

#include "wores.hpp"
#include "stdafx.h"
#include "m4chwrap.h"
#include "wrapbreakpoint.h"
#include "wrapdef.h"
#include "gstring.h"
#include "cvdate.h"


/////////////////////////////////////////////////////////////////////////////
// ClIWrapExecutionPoint

CWrapBreakpoint::CWrapBreakpoint()
{
}

CWrapBreakpoint::~CWrapBreakpoint()
{
}


void CWrapBreakpoint::Init(ClBreakpoint *ai_poBreakpoint)
{
	m_oBreakpoint=*ai_poBreakpoint;
}

void CWrapBreakpoint::UnWrap(ClBreakpoint ** ao_poBP)
{
	*ao_poBP = &m_oBreakpoint;
}

STDMETHODIMP CWrapBreakpoint::get_M4Object(/*[out, retval]*/ BSTR* ao_pstName)
{
	ENTER_METHOD(CWrapBreakpoint::get_M4Object);

	gstring tmpGstring( m_oBreakpoint.GetChannelId() );

	if (tmpGstring)
	{
		*ao_pstName = (BSTR)tmpGstring;

		LEAVE_METHOD(CWrapBreakpoint::get_M4Object, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapBreakpoint::get_M4Object, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapBreakpoint::put_M4Object(/*[in]*/ BSTR ai_pstName)
{
	ENTER_METHOD(CWrapBreakpoint::put_M4Object);

	gstring tmpGstring( ai_pstName );

	if (tmpGstring)
	{
		m_oBreakpoint.SetChannelId(tmpGstring);
		LEAVE_METHOD(CWrapBreakpoint::put_M4Object, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapBreakpoint::put_M4Object, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapBreakpoint::get_Node(/*[out, retval]*/ BSTR* ao_pstName)
{
	ENTER_METHOD(CWrapBreakpoint::get_Node);

	gstring tmpGstring( m_oBreakpoint.GetNodeId() );

	if (tmpGstring)
	{
		*ao_pstName = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapBreakpoint::get_Node, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapBreakpoint::get_Node, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapBreakpoint::put_Node(/*[in]*/ BSTR ai_pstName)
{
	ENTER_METHOD(CWrapBreakpoint::put_Node);

	gstring tmpGstring( ai_pstName );

	if (tmpGstring)
	{
		m_oBreakpoint.SetNodeId(tmpGstring);
		LEAVE_METHOD(CWrapBreakpoint::put_Node, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapBreakpoint::put_Node, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapBreakpoint::get_Item(/*[out, retval]*/ BSTR* ao_pstName)
{
	ENTER_METHOD(CWrapBreakpoint::get_Item);

	gstring tmpGstring( m_oBreakpoint.GetItemId() );

	if (tmpGstring)
	{
		*ao_pstName = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapBreakpoint::get_Item, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapBreakpoint::get_Item, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapBreakpoint::put_Item(/*[in]*/ BSTR ai_pstName)
{
	ENTER_METHOD(CWrapBreakpoint::put_Item);

	gstring tmpGstring( ai_pstName );

	if (tmpGstring)
	{
		m_oBreakpoint.SetItemId(tmpGstring);
		LEAVE_METHOD(CWrapBreakpoint::put_Item, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapBreakpoint::put_Item, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapBreakpoint::get_Rule(/*[out, retval]*/ BSTR* ao_pstName)
{
	ENTER_METHOD(CWrapBreakpoint::get_Rule);

	gstring tmpGstring( m_oBreakpoint.GetRuleId() );

	if (tmpGstring)
	{
		*ao_pstName = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapBreakpoint::get_Rule, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapBreakpoint::get_Rule, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapBreakpoint::put_Rule(/*[in]*/ BSTR ai_pstName)
{
	ENTER_METHOD(CWrapBreakpoint::put_Rule);

	gstring tmpGstring( ai_pstName );

	if (tmpGstring)
	{
		m_oBreakpoint.SetRuleId(tmpGstring);
		LEAVE_METHOD(CWrapBreakpoint::put_Rule, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapBreakpoint::put_Rule, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapBreakpoint::get_RuleTI(/*[out, retval]*/ BSTR* ao_pstName)
{
	ENTER_METHOD(CWrapBreakpoint::get_RuleTI);

	gstring tmpGstring( m_oBreakpoint.GetRuleTIId() );

	if (tmpGstring)
	{
		*ao_pstName = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapBreakpoint::get_RuleTI, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapBreakpoint::get_RuleTI, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapBreakpoint::put_RuleTI(/*[in]*/ BSTR ai_pstName)
{
	ENTER_METHOD(CWrapBreakpoint::put_RuleTI);

	gstring tmpGstring( ai_pstName );

	if (tmpGstring)
	{
		m_oBreakpoint.SetRuleTIId(tmpGstring);
		LEAVE_METHOD(CWrapBreakpoint::put_RuleTI, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapBreakpoint::put_RuleTI, E_UNEXPECTED);
	}

}

STDMETHODIMP CWrapBreakpoint::get_Line(/*[out, retval]*/ int* ao_piLine)
{
	ENTER_METHOD(CWrapBreakpoint::get_Line);
	
	*ao_piLine=m_oBreakpoint.GetLine();

	LEAVE_METHOD(CWrapBreakpoint::get_Line, S_OK);
}

STDMETHODIMP CWrapBreakpoint::put_Line(/*[in]*/ int ai_piLine)
{
	ENTER_METHOD(CWrapBreakpoint::put_Line);
	
	m_oBreakpoint.SetLine(ai_piLine);

	LEAVE_METHOD(CWrapBreakpoint::put_Line, S_OK);
}

STDMETHODIMP CWrapBreakpoint::get_StartDate(/*[out, retval]*/ DATE *pVal)
{
	ENTER_METHOD(CWrapBreakpoint::get_StartDate);
	
	*pVal=M4dateToDate(m_oBreakpoint.GetStartDate());

	LEAVE_METHOD(CWrapBreakpoint::get_StartDate, S_OK);
}

STDMETHODIMP CWrapBreakpoint::put_StartDate(/*[in]*/ DATE pVal)
{
	ENTER_METHOD(CWrapBreakpoint::put_StartDate);
	
	m_oBreakpoint.SetStartDate(DateToM4date(pVal) );

	LEAVE_METHOD(CWrapBreakpoint::put_StartDate, S_OK);
}

STDMETHODIMP CWrapBreakpoint::get_EndDate(/*[out, retval]*/ DATE *pVal)
{
	ENTER_METHOD(CWrapBreakpoint::get_EndDate);
	
	*pVal=M4dateToDate(m_oBreakpoint.GetEndDate());

	LEAVE_METHOD(CWrapBreakpoint::get_EndDate, S_OK);

}

STDMETHODIMP CWrapBreakpoint::put_EndDate(/*[in]*/ DATE pVal)
{
	ENTER_METHOD(CWrapBreakpoint::put_EndDate);
	
	m_oBreakpoint.SetEndDate(DateToM4date(pVal));

	LEAVE_METHOD(CWrapBreakpoint::put_EndDate, S_OK);
}

STDMETHODIMP CWrapBreakpoint::get_Active(/*[out, retval]*/ VARIANT_BOOL* ao_pbVal)
{
	ENTER_METHOD(CWrapBreakpoint::get_Active);

	if (m_oBreakpoint.GetActive())
	{
		*ao_pbVal=VARIANT_TRUE;
	}
	else
	{
		*ao_pbVal=VARIANT_FALSE;
	}
	
	LEAVE_METHOD(CWrapBreakpoint::get_Active, S_OK);
}

STDMETHODIMP CWrapBreakpoint::put_Active(/*[in]*/ VARIANT_BOOL ai_bVal)
{
	ENTER_METHOD(CWrapBreakpoint::put_Active);

	m_oBreakpoint.SetActive(ai_bVal== VARIANT_TRUE);
	
	LEAVE_METHOD(CWrapBreakpoint::put_Active, S_OK);
}

STDMETHODIMP CWrapBreakpoint::get_Value(/*[out, retval]*/ int* ao_piVal)
{
	ENTER_METHOD(CWrapBreakpoint::get_Value);
	
	*ao_piVal=m_oBreakpoint.GetValue();

	LEAVE_METHOD(CWrapBreakpoint::get_Value, S_OK);
}

STDMETHODIMP CWrapBreakpoint::put_Value(/*[in]*/ int ai_iVal)
{
	ENTER_METHOD(CWrapBreakpoint::put_Value);
	
	m_oBreakpoint.SetValue(ai_iVal);

	LEAVE_METHOD(CWrapBreakpoint::put_Value, S_OK);
}
