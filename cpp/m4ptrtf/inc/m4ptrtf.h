// M4PTRTF.h : Declaration of the CM4PTRTF

#ifndef __M4PTRTF_H_
#define __M4PTRTF_H_

#include "resource.h"       // main symbols
#include "clspattern.h"
#include "clstextinfo.h"

/////////////////////////////////////////////////////////////////////////////
// CM4PTRTF
class ATL_NO_VTABLE CM4PTRTF :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CM4PTRTF, &CLSID_M4PTRTF>,
	public IDispatchImpl<IM4PTRTF, &IID_IM4PTRTF, &LIBID_M4PTRTFLib>
{
public:
	CM4PTRTF();
	~ CM4PTRTF();

DECLARE_REGISTRY_RESOURCEID(IDR_M4PTRTF)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CM4PTRTF)
	COM_INTERFACE_ENTRY(IM4PTRTF)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IM4PTRTF
public:
	STDMETHOD(get_LCID)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_LCID)(/*[in]*/ long newVal);
	STDMETHOD(SetColorInfo)(/*[in]*/ short Id, /*[in]*/ long Color, /*[out, retval]*/ VARIANT_BOOL *bRet);
	STDMETHOD(GetColorInfo)(/*[in]*/ short Id, /*[out, optional]*/ long *Color, /*[out, optional]*/ long *DefaultColor, /*[out, optional]*/ short *iKind, /*[out, optional]*/ BSTR *Description, /*[out, retval]*/ VARIANT_BOOL *bRet);
	STDMETHOD(NextBreakPointLine)(/*[in, optional, defaultvalue(-1)]*/ long Line, /*[out, retval]*/ long *pVal);
	STDMETHOD(NextSelectionLine)(/*[in, optional, defaultvalue(-1)]*/ long Line, /*[out, retval]*/ long *pVal);
	STDMETHOD(get_BreakPointMark)(/*[in]*/ long Line, /*[out, retval]*/ short *pVal);
	STDMETHOD(put_BreakPointMark)(/*[in]*/ long Line, /*[in]*/ short newVal);
	STDMETHOD(get_SelectionMark)(/*[in]*/ long Line, /*[out, retval]*/ short *pVal);
	STDMETHOD(put_SelectionMark)(/*[in]*/ long Line, /*[in]*/ short newVal);
	STDMETHOD(AdjustTextStatus)(/*[in]*/ long lStartLine, /*[in]*/ long lOldNumLines, /*[in]*/ long lNewNumLines);
	STDMETHOD(ClearStatus)();
	STDMETHOD(get_ActiveLine)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_ActiveLine)(/*[in]*/ long newVal);
	STDMETHOD(get_ParseMode)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_ParseMode)(/*[in]*/ short newVal);
	STDMETHOD(GenerateRTF)(/*[in]*/ BSTR sPlainText, /*[in, optional, defaultvalue(0) ]*/ long lStartLine, /*[in, out, optional, defaultvalue(0)]*/ VARIANT_BOOL * bPostProcess, /*[out, optional]*/ long * lEndLine, /*[out, retval]*/ BSTR * sRTFText);
private:
	clsPattern * m_Pattern;
	clsTextInfo * m_TextInfo;
	long m_LCID;
	CString m_sRTFHeader;

	void RegenerateHeader();
	void ClearHeader();

};

#endif //__M4PTRTF_H_
