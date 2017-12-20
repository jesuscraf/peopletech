// CPropBag.h : Declaration of the CCPropBag

#ifndef __CPROPBAG_H_
#define __CPROPBAG_H_

#include "resource.h"       // main symbols
#include "cllstr.hpp"
#include "m4unicode.hpp"
extern int	M4IsUnicode( void ) ;

/////////////////////////////////////////////////////////////////////////////
// CCPropBag
class ATL_NO_VTABLE CCPropBag : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCPropBag, &CLSID_CPropBag>,
	public ISupportErrorInfo,
	public IDispatchImpl<ICPropBag, &IID_ICPropBag, &LIBID_M4PROPLib>
{
public:
	CCPropBag()
	{
		HRESULT hr = CoCreateInstance(CLSID_CPropCol, NULL, CLSCTX_ALL , IID_ICPropCol, (void**)&m_PropCol);
		hr = CoCreateInstance(CLSID_CChildCol, NULL, CLSCTX_ALL , IID_ICChildCol, (void**)&m_ChildCol);
		m_nLength = 0;
		m_bstrClassName = NULL;
		m_bstrAlias = NULL;
		//ATLTRACE("Construcción de PropBag\n");
	}
	~CCPropBag()
	{
		::SysFreeString(m_bstrClassName);
		::SysFreeString(m_bstrAlias);
		RELEASE(m_PropCol);
		RELEASE(m_ChildCol);
		//ATLTRACE("Destrucción de PropBag\n");
	}


// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);


void writeString(unsigned char **strASN, char * strValue, long nstrLength)
{
	if( M4IsUnicode() == 1 && M4IsAscii( (char*)strValue, nstrLength ) == 0 )
	{
		(**strASN) = ASNUnicodeString;
	}
	else
	{
		(**strASN) = ASNGeneralString;
	}

	(*strASN)++;
		//incremento en el num de bytes el puntero strASN dentro de funcion
	writeLength(strASN, nstrLength);
	if (nstrLength != 0)
	{
		memcpy((*strASN), strValue, nstrLength);
		(*strASN) += nstrLength;
	}


}
long Longlength(long nLength)
{

	if (nLength >127)
	{
		unsigned long aux = nLength;
		long i = 1;
		while (aux != 0)
		{
			i++;
			aux >>= 8;
		}
		return i;
		
	}
	return 1;

}
void writeLength(unsigned char **strASN,long m_length)
{
	long aux;
	unsigned char m_bytes[5];
	int n_total;
	int i = 0,j = 0;

	if (m_length >127)
	{
		aux = m_length;
		while (aux != 0)
		{
			m_bytes[i++] = aux & (0xff);
			aux >>= 8;
		}

		n_total = 128 +i;

		/// i representa los bytes que ocupa el numero
		(**strASN) = n_total;
		(*strASN) ++;
		for(j=i-1;j>=0;j--)
		{
			(**strASN) = m_bytes[j];
			(*strASN)++;
		}
		
	}
	else
	{
		n_total = (unsigned char)m_length;
		(**strASN) = n_total;
		(*strASN)++;
	}
}



DECLARE_REGISTRY_RESOURCEID(IDR_CPROPBAG)

BEGIN_COM_MAP(CCPropBag)
	COM_INTERFACE_ENTRY(ICPropBag)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ICPropBag
protected:
	long m_nLength;
	ICPropCol  * m_PropCol;
	ICChildCol * m_ChildCol;
	BSTR m_bstrClassName;
	BSTR m_bstrAlias;


public:
	void PrePersistProccess();
	STDMETHOD(WriteASN)(/*[in, out]*/ unsigned char** charIn);
	STDMETHOD(WriteXML)(/*[in, out]*/ ClLongString &roXML);
	STDMETHOD(get_Length)(/*[out, retval]*/ long *pVal);
	STDMETHOD(Invalidatelength)();
	long CalculatePropLength();
	void InvalidateLength();
	long MyLength();
	STDMETHOD(Property)(/*[in, out]*/ BSTR PropertyName, /*[in, out]*/ VARIANT * defValue, /*[out, retval]*/ VARIANT * vReturn);
	STDMETHOD(AddChild)(/*[in, out]*/ ICPropBag * * newValue);
	STDMETHOD(AddProperty)(BSTR strName, VARIANT vValue);
	STDMETHOD(ConditionalAddProperty)(BSTR Name, VARIANT vValue, VARIANT vDefault);
	STDMETHOD(get_ClassName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_ClassName)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Alias)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Alias)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_ChildCol)(/*[out, retval]*/ ICChildCol * *pVal);
	STDMETHOD(get_PropCol)(/*[out, retval]*/ ICPropCol * *pVal);
};

#endif //__CPROPBAG_H_
