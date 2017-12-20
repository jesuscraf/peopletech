// DPoint.cpp : Implementation of CDPoint
#include "stdafx.h"
#include "m4design.h"
#include "handle.h"
#include "line.h"
#include "dpoint.h"

/////////////////////////////////////////////////////////////////////////////
// CDPoint


STDMETHODIMP CDPoint::get_x(long *pVal)
{
	if (m_oLine)
	{
		*pVal = m_oHandle->X;
		return S_OK;
	}

	return E_POINTER;
}

STDMETHODIMP CDPoint::put_x(long newVal)
{
	if (m_oLine)
	{
		m_oLine->InvalidateLine();
		m_oHandle->X = newVal;
		m_oLine->InvalidateLine();
		return S_OK;
	}

	return E_POINTER;
}

STDMETHODIMP CDPoint::get_y(long *pVal)
{
	if (m_oLine)
	{
		*pVal = m_oHandle->Y;
		return S_OK;
	}

	return E_POINTER;
}

STDMETHODIMP CDPoint::put_y(long newVal)
{
	if (m_oLine)
	{
		m_oLine->InvalidateLine();
		m_oHandle->Y = newVal;
		m_oLine->InvalidateLine();
		return S_OK;
	}

	return E_POINTER;
}
