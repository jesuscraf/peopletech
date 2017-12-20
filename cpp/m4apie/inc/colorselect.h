// ColorSelect.h : Declaration of the CColorSelect

#ifndef __COLORSELECT_H_
#define __COLORSELECT_H_

#include "resource.h"       // main symbols
#include "colorbtn.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CColorSelect
//##ModelId=396B32990312
class ATL_NO_VTABLE CColorSelect : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CColorSelect, &CLSID_ColorSelect>,
	public IDispatchImpl<IColorSelect, &IID_IColorSelect, &LIBID_M4APIELib>
{
public:
	//##ModelId=396B3299038D
	CColorSelect()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_COLORSELECT)

BEGIN_COM_MAP(CColorSelect)
	COM_INTERFACE_ENTRY(IColorSelect)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IColorSelect
public:
	//##ModelId=396B32990384
	STDMETHOD(Show)(/*[out, retval]*/ VARIANT_BOOL * bResult);
	//##ModelId=396B32990381
	STDMETHOD(get_Color)(/*[out, retval]*/ long *pVal);

protected:
	//##ModelId=396B3299036F
	CColorBtnDlg m_colorDlg;
	//##ModelId=396B32990365
	COLORREF m_colorValue;
};

#endif //__COLORSELECT_H_
