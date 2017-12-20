// CChildCol.h : Declaration of the CCChildCol

#ifndef __CCHILDCOL_H_
#define __CCHILDCOL_H_

#include "resource.h"       // main symbols
#include "cmycollection.h"
#include "cllstr.hpp"

/////////////////////////////////////////////////////////////////////////////
// CCChildCol
class ATL_NO_VTABLE CCChildCol : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCChildCol, &CLSID_CChildCol>,
	//public IDispatchImpl<ICChildCol, &IID_ICChildCol, &LIBID_M4PROPLib>
	public CMyCollection < ICPropBag, IDispatchImpl<ICChildCol, &IID_ICChildCol, &LIBID_M4PROPLib> >

{
public:
	CCChildCol()
	{
		//ATLTRACE("Construcción de ChildCol\n");
	}
	~CCChildCol()
	{
		RemoveAll();
		//ATLTRACE("Destrucción de ChildCol\n");
	}


DECLARE_REGISTRY_RESOURCEID(IDR_CCHILDCOL)

BEGIN_COM_MAP(CCChildCol)
	COM_INTERFACE_ENTRY(ICChildCol)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// ICChildCol
public:
	void PrePersistProccess();
	STDMETHOD(WriteASN)(/*[in, out]*/ unsigned char** charIn);
	STDMETHOD(WriteXML)(/*[in, out]*/ ClLongString &roXML);
	STDMETHOD(get_Length)(/*[out, retval]*/ long *pVal);
	STDMETHOD(InvalidateLength)();


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



};

#endif //__CCHILDCOL_H_
