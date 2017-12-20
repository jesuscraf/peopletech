// FontSelect.cpp : Implementation of CFontSelect
#include "stdafx.h"
#include "m4apie.h"
#include "fontdialogselect.h"
#include "fontselect.h"

/////////////////////////////////////////////////////////////////////////////
// CFontSelect


//##ModelId=396B329A0101
STDMETHODIMP CFontSelect::Show(VARIANT_BOOL * bResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CFontDialogSelect dlg;

	// Añadimos la información de la Font
	IFont *piFont = NULL;
	BOOL	Underline;
	BOOL	Bold;
	BOOL	Italic; 
	CString FontName;
	BSTR	bstrFontName;
	CY		pointSize;
	long	dcp;

	*bResult = 0; // False por defecto
	if (m_piFont)
	{

		HDC cdc = GetDC(NULL);
		dcp = GetDeviceCaps(cdc ,LOGPIXELSY);
		ReleaseDC(NULL, cdc);

		HRESULT hr = m_piFont->QueryInterface(IID_IFont, (void**) &piFont);
		if (hr == S_OK)
		{
			
			piFont->get_Name(&bstrFontName);
			FontName = bstrFontName;
			::SysFreeString(bstrFontName);
			piFont->get_Bold(&Bold);
			piFont->get_Italic(&Italic);
			piFont->get_Underline(&Underline);
			piFont->get_Size(&pointSize);
			pointSize.Lo /= 10000;

			dlg.m_cf.lpLogFont->lfItalic = (BYTE) Italic;
			dlg.m_cf.lpLogFont->lfUnderline = (BYTE) Underline;
			
			if (Bold)
			{
				dlg.m_cf.lpLogFont->lfWeight = FW_BOLD;
			}
			else
			{
				dlg.m_cf.lpLogFont->lfWeight = FW_NORMAL;
			}
			
			dlg.m_cf.lpLogFont->lfHeight = -MulDiv(pointSize.Lo,dcp, 72);
			strcpy(dlg.m_cf.lpLogFont->lfFaceName, FontName);
			
			dlg.m_cf.Flags = CF_BOTH | CF_INITTOLOGFONTSTRUCT | CF_ENABLEHOOK | CF_EFFECTS;
		}
	}
	else
	{
	}

	if (dlg.DoModal() == IDOK)
	{
		// Aplicamos cambios
		if (piFont)
		{
			// Rellenamos
			FontName = dlg.m_cf.lpLogFont->lfFaceName;
			if (dlg.m_cf.lpLogFont->lfWeight > FW_NORMAL)
			{
				Bold = TRUE;
			}
			else
			{
				Bold = FALSE;
			}
			Underline = (long) dlg.m_cf.lpLogFont->lfUnderline;
			Italic = (long) dlg.m_cf.lpLogFont->lfItalic;
			
			pointSize.Lo = -MulDiv(dlg.m_cf.lpLogFont->lfHeight, 72, dcp);
			pointSize.Lo *= 10000;

			piFont->put_Name(FontName.AllocSysString());
			piFont->put_Bold(Bold);
			piFont->put_Italic(Italic);
			piFont->put_Underline(Underline);
			piFont->put_Size(pointSize);
		
			piFont->Release();
		}
		*bResult = -1;
	}
	// No Aplicamos nada




	return S_OK;
}

//##ModelId=396B329A0115
STDMETHODIMP CFontSelect::get_Font(IFontDisp * * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = (IFontDisp *)m_piFont;
	if (m_piFont)
	{
		m_piFont->AddRef();
	}
	return S_OK;
}

//##ModelId=396B329A010C
STDMETHODIMP CFontSelect::putref_Font(IFontDisp * * newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (m_piFont)
	{
		m_piFont->Release();
	}
	m_piFont = (IFontDisp *) *newVal;
	m_piFont->AddRef();

	return S_OK;
}
