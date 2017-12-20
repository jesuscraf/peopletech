// FontSelect.h : Declaration of the CFontSelect

#ifndef __FONTSELECT_H_
#define __FONTSELECT_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CFontSelect
//##ModelId=396B329A009D
class ATL_NO_VTABLE CFontSelect : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFontSelect, &CLSID_FontSelect>,
	public IDispatchImpl<IFontSelect, &IID_IFontSelect, &LIBID_M4APIELib>
{
public:
	//##ModelId=396B329A0120
	CFontSelect()
	{
		FONTDESC fd;
		fd.cbSizeofstruct = sizeof(fd);
		LPOLESTR pName = (LPOLESTR)_alloca(250);   // MS Sans Serif , 8, False, False, False
		wcscpy(pName, L"MS Sans Serif");
		fd.lpstrName = pName;
		fd.cySize.Lo = 82500;
		fd.cySize.Hi = 0;
		fd.sWeight = 400;
		fd.sCharset = ANSI_CHARSET;
		fd.fItalic = 0;
		fd.fUnderline = 0;
		fd.fStrikethrough = 0;
		m_piFont = NULL;
		HRESULT hr = OleCreateFontIndirect( &fd, IID_IFontDisp, (void**) &m_piFont);
		//HINSTANCE hi = AfxGetInstanceHandle();
		ATLTRACE("Creación de FontSelect\n");

	}

	//##ModelId=396B329A011F
	~CFontSelect()
	{
		if (m_piFont)
		{
			
#ifdef DEBUG
			long refnum = m_piFont->Release();
#else
			m_piFont->Release();
#endif
			m_piFont = NULL;
		}
	}	


DECLARE_REGISTRY_RESOURCEID(IDR_FONTSELECT)

BEGIN_COM_MAP(CFontSelect)
	COM_INTERFACE_ENTRY(IFontSelect)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IFontSelect
public:
	//##ModelId=396B329A0115
	STDMETHOD(get_Font)(/*[out, retval]*/ IFontDisp * *pVal);
	//##ModelId=396B329A010C
	STDMETHOD(putref_Font)(/*[in]*/ IFontDisp ** newVal);
	//##ModelId=396B329A0101
	STDMETHOD(Show)(/*[out, retval]*/ VARIANT_BOOL * bResult);

private:
	//##ModelId=396B329A00F9
	IFontDisp	*m_piFont;
	
};

#endif //__FONTSELECT_H_
