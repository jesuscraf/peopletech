// ColorSelect.cpp : Implementation of CColorSelect
#include "stdafx.h"
#include "m4apie.h"
#include "colorselect.h"

/////////////////////////////////////////////////////////////////////////////
// CColorSelect


//##ModelId=396B32990381
STDMETHODIMP CColorSelect::get_Color(long * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = m_colorValue;

	return S_OK;
}

/*STDMETHODIMP CColorSelect::put_Color(long newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_colorValue = newVal;

	return S_OK;
}*/

/*STDMETHODIMP CColorSelect::Show()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (m_colorDlg.DoModal() == IDOK)
    {
        m_colorValue = CColorBtnDlg::colors[m_colorDlg.colorindex];
    }	

	return S_OK;
}*/

//##ModelId=396B32990384
STDMETHODIMP CColorSelect::Show(VARIANT_BOOL * bResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (m_colorDlg.DoModal() == IDOK)
    {
        m_colorValue = CColorBtnDlg::colors[m_colorDlg.colorindex];
		*bResult = -1;
    }
	else
	{
		*bResult = 0;
	}

	return S_OK;
}
