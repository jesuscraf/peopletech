// DLine.h : Declaration of the CDLine

#ifndef __DLINE_H_
#define __DLINE_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDLine
//##ModelId=396DF30B0064
class ATL_NO_VTABLE CDLine : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDLine, &CLSID_DesignLine>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CDLine>,
	public IDispatchImpl<IDesignLine, &IID_IDesignLine, &LIBID_M4DESIGNLib>
{
public:
	//##ModelId=396DF30B018A
	CDLine()
	{
		m_oLine = NULL;
		//m_bstrKey = NULL;
		//m_bstrContainer = NULL;
	}
	//##ModelId=396DF30B0189
	~CDLine()
	{
		//SysFreeString(m_bstrKey);
		//SysFreeString(m_bstrContainer);
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DLINE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDLine)
	COM_INTERFACE_ENTRY(IDesignLine)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CDLine)
END_CONNECTION_POINT_MAP()


// ISupportsErrorInfo
	//##ModelId=396DF30B017F
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IDLine
public:
	STDMETHOD(get_LineDragMode)(/*[out, retval]*/ M4LineDrag *pVal);
	STDMETHOD(put_LineDragMode)(/*[in]*/ M4LineDrag newVal);
	//##ModelId=397C1D0102FF
	STDMETHOD(get_EndCap)(/*[out, retval]*/ M4LineEndCap *pVal);
	//##ModelId=397C1D01006A
	STDMETHOD(put_EndCap)(/*[in]*/ M4LineEndCap newVal);
	//##ModelId=396DF30B0175
	STDMETHOD(get_PB)(/*[out, retval]*/ IDPoint* *pVal);
	//##ModelId=396DF30B016B
	STDMETHOD(get_PA)(/*[out, retval]*/ IDPoint * *pVal);

	//##ModelId=396DF30B0161
	STDMETHOD(get_Style)(/*[out, retval]*/ M4LineStyle *pVal);
	//##ModelId=396DF30B0157
	STDMETHOD(put_Style)(/*[in]*/ M4LineStyle newVal);
	//##ModelId=396DF30B014E
	STDMETHOD(get_Width)(/*[out, retval]*/ long *pVal);
	//##ModelId=396DF30B014B
	STDMETHOD(put_Width)(/*[in]*/ long newVal);
	//##ModelId=396DF30B0141
	STDMETHOD(get_Color)(/*[out, retval]*/ OLE_COLOR *pVal);
	//##ModelId=396DF30B0138
	STDMETHOD(put_Color)(/*[in]*/ OLE_COLOR newVal);
	//##ModelId=396DF30B0125
	STDMETHOD(get_Container)(/*[out, retval]*/ BSTR *pVal);
	//##ModelId=396DF30B011C
	STDMETHOD(get_Key)(/*[out, retval]*/ BSTR *pVal);
	//##ModelId=396DF30B0119
	STDMETHOD(get_SelectType)(/*[out, retval]*/ M4SelectionType *pVal);
	//##ModelId=396DF30B0105
	STDMETHOD(Move)(/*[in]*/ long Left, /*[in]*/ long Top, /*[in]*/ long Right, /*[in]*/ long Bottom);
	//##ModelId=396DF30B00FB
	STDMETHOD(get_Bottom)(/*[out, retval]*/ long *pVal);
	//##ModelId=396DF30B00F2
	STDMETHOD(put_Bottom)(/*[in]*/ long newVal);
	//##ModelId=396DF30B00E9
	STDMETHOD(get_Right)(/*[out, retval]*/ long *pVal);
	//##ModelId=396DF30B00DF
	STDMETHOD(put_Right)(/*[in]*/ long newVal);
	//##ModelId=396DF30B00D5
	STDMETHOD(get_Top)(/*[out, retval]*/ long *pVal);
	//##ModelId=396DF30B00CC
	STDMETHOD(put_Top)(/*[in]*/ long newVal);
	//##ModelId=396DF30B00C9
	STDMETHOD(get_Left)(/*[out, retval]*/ long *pVal);
	//##ModelId=396DF30B00BF
	STDMETHOD(put_Left)(/*[in]*/ long newVal);
	//##ModelId=396DF30B00B5
	CLine *			m_oLine;
	//##ModelId=396891E6008D
//	BSTR			m_bstrKey;
	//##ModelId=396891E6006F
//	BSTR			m_bstrContainer;
};

#endif //__DLINE_H_
