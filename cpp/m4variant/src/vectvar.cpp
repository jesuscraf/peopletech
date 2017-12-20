//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                vectvar.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                04-03-1998
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//			Variants   Vector.
//
//
//==============================================================================

#include "m4var.hpp"
#include "m4stl.hpp"
#include "vectvar.hpp"


ClVectorVariants &ClVectorVariants::Sum(ClVectorVariants &vvOper, m4bool_t ai_bConcatStrings)
{
	if (vvOper.GetSize() != m_iNumElem )
		return *this;
	for ( m4int32_t i = 0 ; i < m_iNumElem; i++)
	{
		m_vvVariants[i].Sum(vvOper[i], ai_bConcatStrings);
	}
	return *this;
}

ClVectorVariants &ClVectorVariants::Div(m4VariantType &vOper)
{
	for (m4int32_t i = 0; i < m_iNumElem ; i++)
	{
		m_vvVariants[i].Div(vOper);
	}
	return *this;
}

ClVectorVariants &ClVectorVariants::Max(ClVectorVariants &vvOper)
{
	if (vvOper.GetSize() != m_iNumElem )
		return *this;
	for (m4int32_t i = 0 ; i < m_iNumElem ; i++ )
	{
		m_vvVariants[i].Max(vvOper[i]);
	}
	return *this;
}
	
ClVectorVariants &ClVectorVariants::Min(ClVectorVariants &vvOper)
{
	if ( vvOper.GetSize() != m_iNumElem )
		return *this;
	for (m4int32_t i = 0; i < m_iNumElem ; i++ )
	{
		m_vvVariants[i].Min(vvOper[i]);
	}
	return *this;
}

void ClVectorVariants::FreeData (void)
{
	for (m4int32_t i = 0 ; i < m_iNumElem ; i++ )
	{
		m_vvVariants[i].FreeData();
	}
}

void ClVectorVariants::SetNull (void)
{
	for (m4int32_t i = 0 ; i < m_iNumElem ; i++ )
	{
		m_vvVariants[i].SetNull();
	}
}

void ClVectorVariants::SetDouble(m4double_t ai_double)
{
	for (m4int32_t i=0;i<m_iNumElem;i++)
	{
		m_vvVariants[i].SetDouble(ai_double);
	}

}

ClVectorVariants &ClVectorVariants::Dec(ClVectorVariants &vvOper)
{
	if (vvOper.GetSize() != m_iNumElem )
		return *this;
	for ( m4int32_t i = 0 ; i < m_iNumElem; i++)
	{
		m_vvVariants[i].Dec(vvOper[i]);
	}
	return *this;
}

ClVectorVariants &ClVectorVariants::Mult(m4VariantType &vOper)
{
	for (m4int32_t i = 0; i < m_iNumElem ; i++)
	{
		m_vvVariants[i].Mult(vOper);
	}
	return *this;
}


