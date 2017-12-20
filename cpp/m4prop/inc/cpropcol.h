// CPropCol.h : Declaration of the CCPropCol

#ifndef __CPROPCOL_H_
#define __CPROPCOL_H_

#include "resource.h"       // main symbols

//##. Begin Adri y Juanma.
//	Optimización de ECI. Eliminamos todas las cadenas que almacenan nombre
//	de propiedades y las sustituimos por un código
//  esta colección es la que contiene Keys numéricas en vez de cadenas
#include "cmycollectionbycode.h"
#include "cllstr.hpp"
//End

/////////////////////////////////////////////////////////////////////////////
// CCPropCol
class ATL_NO_VTABLE CCPropCol : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCPropCol, &CLSID_CPropCol>,
	//public IDispatchImpl<ICPropCol, &IID_ICPropCol, &LIBID_M4PROPLib>
	//##. Begin Adri y Juanma.
		// public CMyCollection<ICSingleProp, IDispatchImpl<ICPropCol, &IID_ICPropCol, &LIBID_M4PROPLib> >
	public CMyCollectionByCode<ICSingleProp, IDispatchImpl<ICPropCol, &IID_ICPropCol, &LIBID_M4PROPLib> >
	// End

	//public CColTemplate<ICSingleProp, IDispatchImpl<ICPropCol, &IID_ICPropCol, &LIBID_M4PROPLib> >


{
public:
	CCPropCol()
	{
		//ATLTRACE("Construcción de CPropCol\n");
	}

	~CCPropCol()
	{
		RemoveAll();
		//ATLTRACE("Destrucción de CPropCol\n");
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


DECLARE_REGISTRY_RESOURCEID(IDR_CPROPCOL)

BEGIN_COM_MAP(CCPropCol)
	COM_INTERFACE_ENTRY(ICPropCol)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// ICPropCol
public:
	void PrePersistProccess();
	STDMETHOD(WriteASN)(/*[in, out]*/ unsigned char** charIn);
	STDMETHOD(WriteXML)(/*[in, out]*/ ClLongString &roXML);
	STDMETHOD(get_Length)(/*[out, retval]*/ long *pVal);
	long Getlength();
};

#endif //__CPROPCOL_H_
