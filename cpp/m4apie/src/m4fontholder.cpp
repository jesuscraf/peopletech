// M4FontHolder.cpp : Implementation of CM4FontHolder
#include "stdafx.h"
#include "m4apie.h"
#include "m4fontholder.h"

/////////////////////////////////////////////////////////////////////////////
// CM4FontHolder

//##ModelId=396B329A03C1
STDMETHODIMP CM4FontHolder::Clone(IM4FontHolder * * pFontHolder)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pFontHolder = NULL;
	IUnknown *pUnk;
	HRESULT hr = CoCreateInstance(CLSID_M4FontHolder, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&pUnk);
	if (hr == S_OK)
	{
		hr = pUnk->QueryInterface(IID_IM4FontHolder, (void**)pFontHolder);
		pUnk->Release();
		if (hr == S_OK)
		{
			(*pFontHolder)->put_FontName(m_sFontName);
			(*pFontHolder)->put_FontText(m_sFontText);
			(*pFontHolder)->put_FontBold(m_sFontBold);
			(*pFontHolder)->put_FontItalic(m_sFontItalic);
			(*pFontHolder)->put_FontUnderline(m_sFontUnderline);
			(*pFontHolder)->put_FontSize(m_sFontSize);
		}
		else
			hr = E_FAIL;
	}

	return hr;
}


/*
Obtiene el nombre del font
*/
STDMETHODIMP CM4FontHolder::get_Name(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if( *pVal != NULL )
	{
		SysFreeString( *pVal ) ;
		*pVal = NULL ;
	}

	*pVal = SysAllocString( m_sFontText ) ;

	return S_OK;
}

/*
Asigna el nombre del font
Primero se asigna el texto con el nuevo valor
Después se asigna el nombre del font
*/
STDMETHODIMP CM4FontHolder::put_Name(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	put_FontText( newVal ) ;

	put_FontName( newVal ) ;

	return S_OK;
}


STDMETHODIMP CM4FontHolder::put_FontName(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if( m_sFontName != NULL )
	{
		SysFreeString( m_sFontName ) ;
		m_sFontName = NULL ;
	}

	if( newVal != NULL )
	{
		m_sFontName = SysAllocString( newVal ) ;
	}

	return S_OK;
}

STDMETHODIMP CM4FontHolder::put_FontText(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if( m_sFontText != NULL )
	{
		SysFreeString( m_sFontText ) ;
		m_sFontText = NULL ;
	}

	if( newVal != NULL )
	{
		m_sFontText = SysAllocString( newVal ) ;
	}

	return S_OK;
}

STDMETHODIMP CM4FontHolder::get_FontBold(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
		
	if( *pVal != NULL )
	{
		SysFreeString( *pVal ) ;
		*pVal = NULL ;
	}

	*pVal = SysAllocString( m_sFontBold ) ;

	return S_OK;
}

STDMETHODIMP CM4FontHolder::put_FontBold(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if( m_sFontBold != NULL )
	{
		SysFreeString( m_sFontBold ) ;
		m_sFontBold = NULL ;
	}

	if( newVal != NULL )
	{
		m_sFontBold = SysAllocString( newVal ) ;
	}

	return S_OK;
}

STDMETHODIMP CM4FontHolder::get_FontItalic(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
		
	if( *pVal != NULL )
	{
		SysFreeString( *pVal ) ;
		*pVal = NULL ;
	}

	*pVal = SysAllocString( m_sFontItalic ) ;

	return S_OK;
}

STDMETHODIMP CM4FontHolder::put_FontItalic(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if( m_sFontItalic != NULL )
	{
		SysFreeString( m_sFontItalic ) ;
		m_sFontItalic = NULL ;
	}

	if( newVal != NULL )
	{
		m_sFontItalic = SysAllocString( newVal ) ;
	}

	return S_OK;
}

STDMETHODIMP CM4FontHolder::get_FontUnderline(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
		
	if( *pVal != NULL )
	{
		SysFreeString( *pVal ) ;
		*pVal = NULL ;
	}

	*pVal = SysAllocString( m_sFontUnderline ) ;

	return S_OK;
}

STDMETHODIMP CM4FontHolder::put_FontUnderline(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if( m_sFontUnderline != NULL )
	{
		SysFreeString( m_sFontUnderline ) ;
		m_sFontUnderline = NULL ;
	}

	if( newVal != NULL )
	{
		m_sFontUnderline = SysAllocString( newVal ) ;
	}

	return S_OK;
}

STDMETHODIMP CM4FontHolder::get_FontSize(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
		
	if( *pVal != NULL )
	{
		SysFreeString( *pVal ) ;
		*pVal = NULL ;
	}

	*pVal = SysAllocString( m_sFontSize ) ;

	return S_OK;
}

STDMETHODIMP CM4FontHolder::put_FontSize(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if( m_sFontSize != NULL )
	{
		SysFreeString( m_sFontSize ) ;
		m_sFontSize = NULL ;
	}

	if( newVal != NULL )
	{
		m_sFontSize = SysAllocString( newVal ) ;
	}

	return S_OK;
}

