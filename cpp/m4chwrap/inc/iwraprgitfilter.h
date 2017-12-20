// WrapColArgument.h : Declaration of the CIWrapRgItFilter

#ifndef __WRAPRGITFILTER_H_
#define __WRAPRGITFILTER_H_

#include "resource.h"       // main symbols

#include "iwrapfilter.h"

class ClFilterInterface;

/////////////////////////////////////////////////////////////////////////////
// CIWrapRgItFilter
class ATL_NO_VTABLE CIWrapRgItFilter : 
	//public CComObjectRootEx<CComSingleThreadModel>,
	//public CComCoClass<CIWrapRgItFilter, &CLSID_WrapColArgument>,
	public IDispatchImpl<IWrapRgItFilter, &IID_IWrapRgItFilter, &LIBID_CHANNELWRAPPERLib>,

    public CIWrapFilter
{
public:
	CIWrapRgItFilter();

/*
DECLARE_REGISTRY_RESOURCEID(IDR_WRAPRGITFILTER)

BEGIN_COM_MAP(CIWrapRgItFilter)
	COM_INTERFACE_ENTRY(IWrapRgItFilter)
	COM_INTERFACE_ENTRY(IDispatch)
	//COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
*/

// IWrapRgItFilter
public:
	STDMETHOD (BFind) (/*[in]*/ SAFEARRAY ** ai_saArguments, VARIANT_BOOL* ao_pbFound, VARIANT_BOOL* ao_pbVal );
	STDMETHOD (Add)(/*[in]*/VARIANT ai_SourceOper, /*[in]*/int ai_type, /*[in]*/VARIANT ai_saArguments, /*[in]*/BSTR ai_stName, /*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD (AddLN4)(/*[in]*/BSTR ai_stSourceCode, /*[in]*/int ai_type, /*[in]*/VARIANT ai_saArguments, /*[in]*/BSTR ai_stName, /*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD (AddOperation)(/*[in]*/int ai_OperType, /*[in]*/int ai_type, /*[in]*/BSTR ai_stName, /*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD (AddFast)(/*[in]*/BSTR ai_pItemName, /*[in]*/VARIANT ai_Value, /*[in]*/int ai_type, /*[in]*/BSTR ai_stName, /*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD (DeleteLast)(/*[out, retval]*/ short* ao_RetVal);
	STDMETHOD (Delete)(/*[in]*/BSTR ai_stName, /*[out, retval]*/ short* ao_RetVal);
    STDMETHOD (Copy)(/*[in]*/IWrapRgItFilter **ai_RgItFilter, /*[out, retval]*/ short* ao_RetVal);
	
    STDMETHODIMP get_Count (short* ao_iCount);
    STDMETHODIMP Item(VARIANT ai_var, IWrapFilter** ao_poArg);

    STDMETHOD (RefreshAll)(/*[out, retval]*/ short* ao_RetVal);

	STDMETHOD (AddSort) (/*[in]*/VARIANT ai_Items, /*[in]*/VARIANT ai_Orders, /*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD (AddSortById)(/*[in]*/ long ai_IndexId, /*[out, retval]*/ short* ao_RetVal);
	STDMETHOD (AddSortByPos)(/*[in]*/ short ai_iPos, /*[out, retval]*/ short* ao_RetVal);

	STDMETHOD (NamedSort) (/*[in]*/VARIANT ai_Items, /*[in]*/VARIANT ai_Orders, /*[in]*/BSTR ai_stName, /*[out, retval]*/ short* ao_sRetVal);
	STDMETHOD (NamedSortById)(/*[in]*/ long ai_IndexId, /*[in]*/BSTR ai_stName, /*[out, retval]*/ short* ao_RetVal);
	STDMETHOD (NamedSortByPos)(/*[in]*/ short ai_iPos, /*[in]*/BSTR ai_stName, /*[out, retval]*/ short* ao_RetVal);
// Se expone en COM el método Clear (en lugar de RemoveAll) ya que en .NET aunque aparece no ejecuta nada en la invocación
	STDMETHOD (Clear)(/*[out, retval]*/ short* ao_RetVal);
	STDMETHOD (Exists)(/*[in]*/BSTR ai_stName, /*[out, retval]*/ VARIANT_BOOL* ao_pbVal);
	STDMETHOD (NameByPos)(/*[in]*/ short ai_iPos, /*[out, retval]*/ BSTR* ao_pstName);

    void Init (ClFilterInterface *ai_pofi);

private:
    ClFilterInterface *m_poFI;
};

#endif //__WRAPRGITFILTER_H_
