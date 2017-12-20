// DLine.cpp : Implementation of CDLine
#include "stdafx.h"
#include "m4design.h"
#include "handle.h"
#include "line.h"
#include "dpoint.h"
#include "dline.h"
#include "ocidl.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CDLine

//##ModelId=396AF9990182
STDMETHODIMP CDLine::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IDesignLine
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

//##ModelId=396AF99900C1
STDMETHODIMP CDLine::get_Left(long *pVal)
{
	if (m_oLine)
	{
		// Obtenemos el Rect Absoluto.
		RECT rc;
		m_oLine->AbsRectReal(&rc);
		*pVal = rc.left;
		return S_OK;
	}

	return E_FAIL;
}

//##ModelId=396AF99900B7
STDMETHODIMP CDLine::put_Left(long newVal)
{
	if (m_oLine)
	{
		// Obtenemos el Rect Absoluto.
		RECT rc;
		m_oLine->AbsRectReal(&rc);
		rc.left = newVal;
		m_oLine->SetRect(rc);
		return S_OK;
	}

	return E_FAIL;
}

//##ModelId=396AF99900D6
STDMETHODIMP CDLine::get_Top(long *pVal)
{
	if (m_oLine)
	{
		// Obtenemos el Rect Absoluto.
		RECT rc;
		m_oLine->AbsRectReal(&rc);
		*pVal = rc.top;
		return S_OK;
	}

	return E_FAIL;
}

//##ModelId=396AF99900CB
STDMETHODIMP CDLine::put_Top(long newVal)
{
	if (m_oLine)
	{
		// Obtenemos el Rect Absoluto.
		RECT rc;
		m_oLine->AbsRectReal(&rc);
		rc.top = newVal;
		m_oLine->SetRect(rc);
		return S_OK;
	}

	return E_FAIL;
}

//##ModelId=396AF99900EB
STDMETHODIMP CDLine::get_Right(long *pVal)
{
	if (m_oLine)
	{
		// Obtenemos el Rect Absoluto.
		RECT rc;
		m_oLine->AbsRectReal(&rc);
		*pVal = rc.right;
		return S_OK;
	}

	return E_FAIL;
}

//##ModelId=396AF99900E1
STDMETHODIMP CDLine::put_Right(long newVal)
{
	if (m_oLine)
	{
		// Obtenemos el Rect Absoluto.
		RECT rc;
		m_oLine->AbsRectReal(&rc);
		rc.right = newVal;
		m_oLine->SetRect(rc);
		return S_OK;
	}

	return E_FAIL;
}

//##ModelId=396AF99900FD
STDMETHODIMP CDLine::get_Bottom(long *pVal)
{
	if (m_oLine)
	{
		// Obtenemos el Rect Absoluto.
		RECT rc;
		m_oLine->AbsRectReal(&rc);
		*pVal = rc.bottom;
		return S_OK;
	}

	return E_FAIL;
}

//##ModelId=396AF99900F4
STDMETHODIMP CDLine::put_Bottom(long newVal)
{
	if (m_oLine)
	{
		// Obtenemos el Rect Absoluto.
		RECT rc;
		m_oLine->AbsRectReal(&rc);
		rc.bottom = newVal;
		m_oLine->SetRect(rc);
		return S_OK;
	}

	return E_FAIL;
}

//##ModelId=396AF9990107
STDMETHODIMP CDLine::Move(long Left, long Top, long Right, long Bottom)
{
	RECT rc;

	rc.bottom = Bottom;
	rc.left = Left;
	rc.right = Right;
	rc.top = Top;
	
	m_oLine->SetRect(rc);

	return S_OK;
}

//##ModelId=396AF999011C
STDMETHODIMP CDLine::get_SelectType(M4SelectionType *pVal)
{
	if (m_oLine)
	{
		*pVal = m_oLine->m_eSelected;
		return S_OK;
	}

	return E_FAIL;
}

//##ModelId=396AF9990126
STDMETHODIMP CDLine::get_Key(BSTR *pVal)
{
	USES_CONVERSION;

	if (m_oLine)
	{
		*pVal = A2BSTR(m_oLine->m_strKey);
		return S_OK;
	}

	return E_FAIL;
}

//##ModelId=396AF999013A
STDMETHODIMP CDLine::get_Container(BSTR *pVal)
{
	USES_CONVERSION;

	if (m_oLine)
	{
		*pVal = A2BSTR(m_oLine->m_strContainer);
		return S_OK;
	}

	return E_FAIL;
}

//##ModelId=396AF999014E
STDMETHODIMP CDLine::get_Color(OLE_COLOR *pVal)
{
	if (m_oLine)
	{
		*pVal = m_oLine->m_Color;
		return S_OK;
	}

	return E_FAIL;
}

//##ModelId=396AF9990144
STDMETHODIMP CDLine::put_Color(OLE_COLOR newVal)
{
	if (m_oLine)
	{
		m_oLine->InvalidateLine();
		m_oLine->m_Color = newVal;
		return S_OK;
	}

	return E_FAIL;
}

//##ModelId=396AF999015A
STDMETHODIMP CDLine::get_Width(long *pVal)
{
	if (m_oLine)
	{
		*pVal = m_oLine->m_nWidth;
		return S_OK;
	}
	return E_FAIL;
}

//##ModelId=396AF9990151
STDMETHODIMP CDLine::put_Width(long newVal)
{
	if (m_oLine)
	{
		m_oLine->InvalidateLine();
		m_oLine->m_nWidth = (newVal >= 1 ? newVal : 1);
		return S_OK;
	}

	return E_FAIL;
}

//##ModelId=396AF999016E
STDMETHODIMP CDLine::get_Style(M4LineStyle *pVal)
{
	if (m_oLine)
	{
		*pVal = m_oLine->m_eStyle;
		return S_OK;
	}
	
	return E_FAIL;
}

//##ModelId=396AF9990164
STDMETHODIMP CDLine::put_Style(M4LineStyle newVal)
{
	if (m_oLine)
	{
		m_oLine->m_eStyle = newVal;
		m_oLine->InvalidateLine();
		return S_OK;
	}
	
	return E_FAIL;
}

STDMETHODIMP CDLine::get_PA(IDPoint **pVal)
{
	CComObject<CDPoint> * pPoint = NULL;
	CComObject<CDPoint>::CreateInstance(&pPoint);

	if (pPoint)
	{
		pPoint->m_oLine = m_oLine;
		pPoint->m_oHandle = &m_oLine->m_pA;
		pPoint->QueryInterface(IID_IDPoint, (void**)pVal);
		return S_OK;
	}

	return E_POINTER;
}

STDMETHODIMP CDLine::get_PB(IDPoint **pVal)
{
	CComObject<CDPoint> * pPoint = NULL;
	CComObject<CDPoint>::CreateInstance(&pPoint);

	if (pPoint)
	{
		pPoint->m_oLine = m_oLine;
		pPoint->m_oHandle = &m_oLine->m_pB;
		pPoint->QueryInterface(IID_IDPoint, (void**)pVal);
		return S_OK;
	}

	return E_POINTER;
}

STDMETHODIMP CDLine::get_EndCap(M4LineEndCap *pVal)
{
	if (pVal && m_oLine)
	{
		*pVal = m_oLine->m_eEndCap;
		return S_OK;
	}

	return E_POINTER;
}

STDMETHODIMP CDLine::put_EndCap(M4LineEndCap newVal)
{
	if (m_oLine)
	{
		m_oLine->m_eEndCap = newVal;
		m_oLine->InvalidateLine();
		return S_OK;
	}
	return E_POINTER;
}

STDMETHODIMP CDLine::get_LineDragMode(M4LineDrag *pVal)
{
	if (pVal && m_oLine)
	{
		*pVal = m_oLine->m_eLineDrag;
		return S_OK;
	}

	return E_POINTER;
}

STDMETHODIMP CDLine::put_LineDragMode(M4LineDrag newVal)
{
	if (m_oLine)
	{
		m_oLine->m_eLineDrag = newVal;
		m_oLine->InvalidateLine();
		return S_OK;
	}
	return E_POINTER;
}
