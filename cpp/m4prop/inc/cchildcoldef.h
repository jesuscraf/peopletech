// CChildColDef.h : Declaration of the CCChildColDef

#ifndef __CCHILDCOLDEF_H_
#define __CCHILDCOLDEF_H_

#include "resource.h"       // main symbols
#include "cexeccollection.h"

/////////////////////////////////////////////////////////////////////////////
// CCChildColDef
class ATL_NO_VTABLE CCChildColDef : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCChildColDef, &CLSID_CChildColDef>,
	public CExecCollection<ICPropBagDef, IDispatchImpl<ICChildColDef, &IID_ICChildColDef, &LIBID_M4PROPLib> >
	//public IDispatchImpl<ICChildColDef, &IID_ICChildColDef, &LIBID_M4PROPLib>
{
public:
	CCChildColDef()
	{
		//ATLTRACE("Construcción de ChildColDef\n");
	}
	~CCChildColDef()
	{
		//ATLTRACE("Destrucción de ChildColDef\n");
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


DECLARE_REGISTRY_RESOURCEID(IDR_CCHILDCOLDEF)

BEGIN_COM_MAP(CCChildColDef)
	COM_INTERFACE_ENTRY(ICChildColDef)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// ICChildColDef
public:
	ICPropBagDef * GetNext(M4PROP_POSITION ** pNode);
	M4PROP_POSITION * GetHeadNode();

	




//	STDMETHOD(WriteASN)(/*[in, out]*/ unsigned char ** charIn);
//	STDMETHOD(InvalidateLength)();
//	STDMETHOD(get_Length)(/*[out, retval]*/ long *pVal);
};

#endif //__CCHILDCOLDEF_H_
