//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapcolprop.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//     Implementation of CWrapColProp
//
//==============================================================================

#include "wores.hpp"
#include "stdafx.h"
#include "m4chwrap.h"
#include "colprop.hpp"
#include "wrapchannel.h"
#include "wrapcolprop.h"
#include "m4types.hpp"
#include "cltypes.hpp"
#include "newinterface.h"
#include "wrapdef.h"
#include "gstring.h"
#include "rerror.h"
#include "channel.hpp"
#include "chan_knl.hpp"


/////////////////////////////////////////////////////////////////////////////
// CWrapColProp
STDMETHODIMP CWrapColProp::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = { &IID_IWrapColProp };

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
		{
			return S_OK;
		}
	}
	return S_FALSE;
}


CWrapColProp::CWrapColProp()
	: m_poColProp(0)
{
}


CWrapColProp::~CWrapColProp()
{
	m_poColProp = 0;
}


void CWrapColProp::Init(ClColProp* ai_VarRef)
{
	m_poColProp = ai_VarRef;

	TRACE2("m_poColProp has address %p", m_poColProp);
}


STDMETHODIMP CWrapColProp::Count(int* ao_iCount)
{
	ENTER_METHOD(CWrapColProp::Count);

	*ao_iCount = m_poColProp->Count();

	TRACE2("Returning count %d", *ao_iCount);

	LEAVE_METHOD(CWrapColProp::Count, S_OK);
}


STDMETHODIMP CWrapColProp::GetPropName(int ai_iId, BSTR* ao_pstName)
{
	ENTER_METHOD(CWrapColProp::GetPropName);

	gstring tmpGstring(m_poColProp->GetPropName(ai_iId));

	if (tmpGstring)
	{
		TRACE3("Got property with name %s using id %d", tmpGstring, ai_iId);

		*ao_pstName = (BSTR)tmpGstring;
	
		if (ao_pstName)
		{
			LEAVE_METHOD(CWrapColProp::GetPropName, S_OK);
		}
	}

	LEAVE_METHOD(CWrapColProp::GetPropName, S_OK);
}


HRESULT	_GetValue( BSTR ai_stName, ClColProp* ai_poColProp, VARIANT *ao_poVariant )
{
	gstring tmpGstring( ai_stName ) ;

	if( tmpGstring != NULL )
	{
		m4VariantType	vValue;
		ai_poColProp->GetPropValue( tmpGstring, vValue ) ;
		vValue.GetVARIANT( *ao_poVariant ) ;
	}
	return( S_OK ) ;
}


STDMETHODIMP CWrapColProp::GetPropValue( BSTR ai_stName, VARIANT* ao_poVariant )
{
	ENTER_METHOD( CWrapColProp::GetPropValue ) ;

	HRESULT	hResult = _GetValue( ai_stName, m_poColProp, ao_poVariant ) ;

	LEAVE_METHOD( CWrapColProp::GetPropValue, hResult ) ;
}


STDMETHODIMP CWrapColProp::GetPropValueById(int ai_iId, VARIANT* ao_poVariant)
{
	m4VariantType poVar;

	ENTER_METHOD(CWrapColProp::GetPropValueById);

	m4return_t RetVal = m_poColProp->GetPropValue(ai_iId, poVar);

	poVar.GetVARIANT(*ao_poVariant);

	if (RetVal != M4_SUCCESS)
	{
		FormatError(M4_CH_WO_CANT_GET_PROPERTY, "Cannot get property given id %d", ai_iId);
	}

	LEAVE_METHOD(CWrapColProp::GetPropValueById, S_OK);
}


HRESULT	_SetValue( BSTR ai_stName, ClColProp* ai_poColProp, VARIANT& ai_roVariant )
{
	gstring tmpGstring( ai_stName ) ;

	if( tmpGstring != NULL )
	{
		// Convert the VARIANT to a m4VariantType
		m4VariantType	vValue( ai_roVariant ) ;
		ai_poColProp->SetOrNewProperty( tmpGstring, vValue, M4_PROP_FULL_STATE ) ;
	}
	return( S_OK ) ;
}


STDMETHODIMP CWrapColProp::SetPropValue( BSTR ai_stName, VARIANT ai_oVariant )
{
	ENTER_METHOD( CWrapColProp::SetPropValue ) ;

	HRESULT	hResult = _SetValue( ai_stName, m_poColProp, ai_oVariant ) ;

	LEAVE_METHOD( CWrapColProp::SetPropValue, hResult ) ;
}


STDMETHODIMP CWrapColProp::SetValue(/*[in]*/ IWrapChannel* ai_poChannel, /*[in]*/ BSTR ai_stName, /*[in]*/ VARIANT ai_oVariant, /*[out]*/ VARIANT* ao_poVariant, /*[out]*/ ULONGLONG* ao_plOldTime, /*[out]*/ ULONGLONG* ao_plNewTime, /*[out, retval]*/ short* ao_piReturn)
{
	ENTER_METHOD( CWrapColProp::SetValue ) ;

	HRESULT		hResult = S_OK ;


	*ao_plOldTime = 0 ;
	*ao_plNewTime = 0 ;
	*ao_piReturn = M4_ERROR ;

	*ao_plOldTime = m_poColProp->GetTimeStamp() ;

	hResult = _SetValue( ai_stName, m_poColProp, ai_oVariant ) ;
	_GetValue( ai_stName, m_poColProp, ao_poVariant ) ;

	*ao_plNewTime = m_poColProp->GetTimeStamp() ;

	if( hResult != S_OK )
	{
		LEAVE_METHOD( CWrapColProp::SetValue, S_OK ) ;
	}

	*ao_piReturn = M4_SUCCESS ;

	LEAVE_METHOD( CWrapColProp::SetValue, S_OK ) ;
}



STDMETHODIMP CWrapColProp::get_GetPropFlagsById(int ai_iId, short ai_iFlag, short * pVal)
{
	ENTER_METHOD(CWrapColProp::get_GetPropFlagsById);

	*pVal = m_poColProp->GetPropFlags( ai_iId, m4uint8_t( ai_iFlag ) ) ;

	TRACE2("Returning flag %d", *pVal);

	LEAVE_METHOD(CWrapColProp::get_GetPropFlagsById, S_OK);
}
