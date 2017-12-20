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
#include "wraprunninginfo.h"
#include "wrapdef.h"
#include "gstring.h"
#include "cvdate.h"
#include "wores.hpp"
#include "rerror.h"


/////////////////////////////////////////////////////////////////////////////
// ClIWrapExecutionPoint

CWrapRunningFormInfo::CWrapRunningFormInfo()
{
}

CWrapRunningFormInfo::~CWrapRunningFormInfo()
{

}

void CWrapRunningFormInfo::Init(ClRunningFormInfo *ai_poInfo)
{
	m_oInfo=*ai_poInfo;
}

STDMETHODIMP CWrapRunningFormInfo::get_M4Object(/*[out, retval]*/ BSTR* ao_pstName)
{
	ENTER_METHOD(CWrapRunningFormInfo::get_M4Object);

	gstring tmpGstring( m_oInfo.GetChannelId() );

	if (tmpGstring)
	{
		*ao_pstName = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapRunningFormInfo::get_M4Object, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapRunningFormInfo::get_M4Object,	E_UNEXPECTED);
	}
}


STDMETHODIMP CWrapRunningFormInfo::get_Node(/*[out, retval]*/ BSTR* ao_pstName)
{
	ENTER_METHOD(CWrapRunningFormInfo::get_Node);

	gstring tmpGstring( m_oInfo.GetNodeId() );

	if (tmpGstring)
	{
		*ao_pstName = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapRunningFormInfo::get_Node, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapRunningFormInfo::get_Node,	E_UNEXPECTED);
	}
}


STDMETHODIMP CWrapRunningFormInfo::get_Item(/*[out, retval]*/ BSTR* ao_pstName)
{
	ENTER_METHOD(CWrapRunningFormInfo::get_Item);

	gstring tmpGstring( m_oInfo.GetItemId() );

	if (tmpGstring)
	{
		*ao_pstName = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapRunningFormInfo::get_Item, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapRunningFormInfo::get_Item,	E_UNEXPECTED);
	}
}


STDMETHODIMP CWrapRunningFormInfo::get_Rule(/*[out, retval]*/ BSTR* ao_pstName)
{
	ENTER_METHOD(CWrapRunningFormInfo::get_Rule);

	gstring tmpGstring( m_oInfo.GetRuleId() );

	if (tmpGstring)
	{
		*ao_pstName = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapRunningFormInfo::get_Rule, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapRunningFormInfo::get_Rule,	E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapRunningFormInfo::get_RuleTI(/*[out, retval]*/ BSTR* ao_pstName)
{
	ENTER_METHOD(CWrapRunningFormInfo::get_RuleTI);

	gstring tmpGstring( m_oInfo.GetRuleTIId() );

	if (tmpGstring)
	{
		*ao_pstName = (BSTR)tmpGstring;
		LEAVE_METHOD(CWrapRunningFormInfo::get_RuleTI, S_OK);
	}
	else
	{
		LEAVE_METHOD(CWrapRunningFormInfo::get_RuleTI, E_UNEXPECTED);
	}
}


STDMETHODIMP CWrapRunningFormInfo::get_Line(/*[out, retval]*/ int* ao_piLine)
{
	ENTER_METHOD(CWrapRunningFormInfo::get_Line);
	
	*ao_piLine = m_oInfo.GetLine();

	LEAVE_METHOD(CWrapRunningFormInfo::get_Line, S_OK);
}


STDMETHODIMP CWrapRunningFormInfo::get_StartDate (/*[out, retval]*/ DATE *pVal)
{
	ENTER_METHOD(CWrapRunningFormInfo::get_StartDate);
	
	*pVal = M4dateToDate(m_oInfo.GetStartDate());

	LEAVE_METHOD(CWrapRunningFormInfo::get_StartDate, S_OK);

}

STDMETHODIMP CWrapRunningFormInfo::get_EndDate(/*[out, retval]*/ DATE *pVal)
{
	ENTER_METHOD(CWrapRunningFormInfo::get_EndDate);
	
	*pVal = M4dateToDate(m_oInfo.GetEndDate());

	LEAVE_METHOD(CWrapRunningFormInfo::get_EndDate, S_OK);
}


STDMETHODIMP CWrapRunningFormInfo::GetDates(/*[out]*/ DATE *pStart,/*[out]*/ DATE *pEnd,/*[out]*/ DATE *pRun, /*[out, retval]*/ short* ao_RetVal)
{
	ENTER_METHOD(CWrapRunningFormInfo::GetDates);

	m4date_t Start, End, Run;

	m_oInfo.GetDates(Start, End, Run);
	*pStart = M4dateToDate(Start);
	*pEnd = M4dateToDate(End);
	*pRun = M4dateToDate(Run);

	LEAVE_METHOD(CWrapRunningFormInfo::GetDates, S_OK);
}


STDMETHODIMP CWrapRunningFormInfo::get_IsVariableArgs(/*[out, retval]*/ VARIANT_BOOL* ao_pbVal)
{
	ENTER_METHOD(CWrapRunningFormInfo::get_IsVariableArgs);

	if (m_oInfo.GetIsVariable())
	{
		*ao_pbVal = VARIANT_TRUE;
	}
	else
	{
		*ao_pbVal = VARIANT_FALSE;
	}
	
	LEAVE_METHOD(CWrapRunningFormInfo::get_IsVariableArgs, S_OK);
}


STDMETHODIMP CWrapRunningFormInfo::get_NumberOfArguments(/*[out, retval]*/ int* ao_piVal)
{
	ENTER_METHOD(CWrapRunningFormInfo::get_NumberOfArguments);
	
	*ao_piVal = m_oInfo.GetNumberofArguments();

	LEAVE_METHOD(CWrapRunningFormInfo::get_NumberOfArguments, S_OK);
}


STDMETHODIMP CWrapRunningFormInfo::GetArguments(/*[out]*/ SAFEARRAY** ao_saArguments, /*[out, retval]*/ short* ao_pbVal)
{
	ENTER_METHOD(CWrapRunningFormInfo::GetArguments);

	int iNumArgs,i;

	iNumArgs = m_oInfo.GetNumberofArguments();

	if (!iNumArgs)
	{
		*ao_pbVal=M4_SUCCESS;
		LEAVE_METHOD(CWrapRunningFormInfo::GetArguments,	S_OK);
	}

	VARIANT *pVariantTemp;
	m4VariantType *poVariant=new m4VariantType [iNumArgs];

	*ao_pbVal=m_oInfo.GetArguments(poVariant,iNumArgs);

	if (*ao_pbVal==M4_SUCCESS)
	{
		//creamos el safe array y retornamos la info
		if (SafeArrayDestroy(*ao_saArguments) != S_OK)
		{ // Error freeing array
			*ao_pbVal=M4_ERROR;
			LEAVE_METHOD(CWrapRunningFormInfo::GetArguments,	S_OK);
		}

		if ((*ao_saArguments=SafeArrayCreateVector(VT_VARIANT, 0, iNumArgs)) == NULL)
		{ 
			*ao_pbVal=M4_ERROR;
			LEAVE_METHOD(CWrapRunningFormInfo::GetArguments,	S_OK);
		}
		
		//para acceder al SafeArray como si fuera un array
		if (SafeArrayAccessData(*ao_saArguments, (void HUGEP* FAR*)&pVariantTemp))
		{
			*ao_pbVal=M4_ERROR;
			LEAVE_METHOD(CWrapRunningFormInfo::GetArguments,	S_OK);
		}
		
		for (i=0; i<iNumArgs; ++i)
		{
			poVariant[i].GetVARIANT( pVariantTemp[i] ) ;
		}
		
		SafeArrayUnaccessData(*ao_saArguments); //desbloqueamos
	}

	delete []poVariant;
	LEAVE_METHOD(CWrapRunningFormInfo::GetArguments,	S_OK);
}


STDMETHODIMP CWrapRunningFormInfo::GetArgument(/*[in]*/ int ai_iPos, /*[out, retval]*/ VARIANT* ao_pvVal)
{
	ENTER_METHOD(CWrapRunningFormInfo::GetArgument);
	
	m4VariantType vVar;
	m4return_t ret=m_oInfo.GetArgument(ai_iPos, vVar);

	if (ret == M4_SUCCESS)
	{
		vVar.GetVARIANT( *ao_pvVal ) ;
		LEAVE_METHOD(CWrapRunningFormInfo::GetArgument,	S_OK);
	}

	LEAVE_METHOD(CWrapRunningFormInfo::GetArgument, E_UNEXPECTED);
}


STDMETHODIMP CWrapRunningFormInfo::get_M4ObjectHandle(/*[out, retval]*/ long *pVal)
{
	ENTER_METHOD(CWrapRunningFormInfo::M4ObjectHandle);
	
	*pVal=m_oInfo.GetChannelHandle();
	
	LEAVE_METHOD(CWrapRunningFormInfo::M4ObjectHandle, S_OK);
}


STDMETHODIMP CWrapRunningFormInfo::get_AccessHandle(/*[out, retval]*/ long *pVal)
{
	ENTER_METHOD(CWrapRunningFormInfo::AccessHandle);
	
	*pVal=m_oInfo.GetAccessHandle();
	
	LEAVE_METHOD(CWrapRunningFormInfo::AccessHandle, S_OK);
}


STDMETHODIMP CWrapRunningFormInfo::get_IsClientSide(/*[out, retval]*/ VARIANT_BOOL* ao_pbVal)
{
	ENTER_METHOD(CWrapRunningFormInfo::get_IsClientSide);

	if (m_oInfo.GetIsClientSide())
	{
		*ao_pbVal=VARIANT_TRUE;
	}
	else
	{
		*ao_pbVal=VARIANT_FALSE;
	}
	
	LEAVE_METHOD(CWrapRunningFormInfo::get_IsClientSide, S_OK);
}

