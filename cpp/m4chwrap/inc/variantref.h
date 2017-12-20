#ifndef _VARIANTREF_H
#define _VARIANTREF_H
//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             variantref.h
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//   Declaration of ClVariantRef
//
//==============================================================================

class ClVariantRef
{
public:
	ClVariantRef() 
	{
		m_oVariant.vt = VT_BYREF;
		m_oVariant.byref = 0;
	}

	~ClVariantRef()
	{
		m_oVariant.byref = 0;		
	}

	ClVariantRef(void* ai_pvAddr)
	{
		m_oVariant.vt = VT_BYREF;
		m_oVariant.byref = ai_pvAddr;
	}

	ClVariantRef(const VARIANT& ai_Variant)
	{
		m_oVariant.vt = VT_BYREF;

		if (ai_Variant.vt == VT_BYREF)
		{
			m_oVariant.byref = ai_Variant.byref;
		}
		else
		{
			m_oVariant.byref = 0;
		}
	}

	operator void* () const
	{
		return m_oVariant.byref; 
	}

	ClVariantRef(const ClVariantRef& ai_Variant)
	{
		m_oVariant.vt = VT_BYREF;
		m_oVariant.byref = ai_Variant;
	}

	ClVariantRef& operator=(
		const ClVariantRef& ai_Variant)
	{
		m_oVariant.byref = ai_Variant;
		return *this;
	}

	ClVariantRef& operator=(void* ai_pvAddr)
	{
		m_oVariant.byref = ai_pvAddr;
		return *this;
	}

	ClVariantRef& operator=(VARIANT& ai_pvAddr)
	{
		m_oVariant.byref = ai_pvAddr.byref;
		return *this;
	}

	operator VARIANT ()
	{
		return m_oVariant;
	}

private:
	VARIANT m_oVariant; 
};

#endif
