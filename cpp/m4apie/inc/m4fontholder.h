// M4FontHolder.h : Declaration of the CM4FontHolder

#ifndef __M4FONTHOLDER_H_
#define __M4FONTHOLDER_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CM4FontHolder
//##ModelId=396B329A031E
class ATL_NO_VTABLE CM4FontHolder : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CM4FontHolder, &CLSID_M4FontHolder>,
	public IDispatchImpl<IM4FontHolder, &IID_IM4FontHolder, &LIBID_M4APIELib>
{
public:
	//##ModelId=396B329A03D2
	CM4FontHolder()
	{
		LPOLESTR pName = (LPOLESTR)_alloca(250);   // MS Sans Serif , 8, False, False, False
		wcscpy(pName, L"MS Sans Serif");
		m_sFontName = SysAllocString( pName ) ;
		m_sFontText = SysAllocString( pName ) ;
		m_sFontBold = SysAllocString( L"False" ) ;
		m_sFontItalic = SysAllocString( L"False" ) ;
		m_sFontUnderline = SysAllocString( L"False" ) ;
		m_sFontSize = SysAllocString( L"8" ) ;		
		ATLTRACE("Creación de M4FontHolder\n");
	}

	//##ModelId=396B329A03CA
	~CM4FontHolder()
	{
		
		// El CComPtr Ya se encarga de hacer el Release en su destructor
		/*if (m_pFont)
		{
			m_pFont->Release();
			m_pFont = NULL;
		}*/

		put_FontName( NULL ) ;
		put_FontText( NULL ) ;
		put_FontBold( NULL ) ;
		put_FontItalic( NULL ) ;
		put_FontUnderline( NULL ) ;
		put_FontSize( NULL ) ;

		ATLTRACE("Destrucción de M4FontHolder\n");
	}


DECLARE_REGISTRY_RESOURCEID(IDR_M4FONTHOLDER)

BEGIN_COM_MAP(CM4FontHolder)
	COM_INTERFACE_ENTRY(IM4FontHolder)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IM4FontHolder
public:
	STDMETHOD(get_FontSize)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_FontSize)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_FontUnderline)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_FontUnderline)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_FontItalic)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_FontItalic)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_FontBold)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_FontBold)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Name)(/*[in]*/ BSTR newVal);
	//##ModelId=396B329A03C1
	STDMETHOD(Clone)(/*[out, retval]*/ IM4FontHolder ** pFontHolder);

protected:
	STDMETHOD(put_FontText)(/*[in]*/ BSTR newVal);
	STDMETHOD(put_FontName)(/*[in]*/ BSTR newVal);

	BSTR m_sFontName ;
	BSTR m_sFontText ;
	BSTR m_sFontBold ;
	BSTR m_sFontItalic ;
	BSTR m_sFontUnderline ;
	BSTR m_sFontSize ;
};

#endif //__M4FONTHOLDER_H_
