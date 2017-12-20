// M4Base64.h : Declaration of the CM4Base64

#ifndef __M4BASE64_H_
#define __M4BASE64_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CM4Base64
//##ModelId=396B329A01AB
class ATL_NO_VTABLE CM4Base64 : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CM4Base64, &CLSID_M4Base64>,
	public IDispatchImpl<IM4Base64, &IID_IM4Base64, &LIBID_M4APIELib>
{
public:
	//##ModelId=396B329A0256
	CM4Base64()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_M4BASE64)

BEGIN_COM_MAP(CM4Base64)
	COM_INTERFACE_ENTRY(IM4Base64)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IM4Base64
public:
	//##ModelId=396B329A0245
	STDMETHOD(Code)(/*[in]*/ BSTR strIn,  BSTR *strOut);
	//##ModelId=396B329A023A
	STDMETHOD(Decode)(/*[in]*/ BSTR strIn, BSTR *strOut);

protected:
	//##ModelId=396B329A021C
	C_Base64	m_b64;

	//##ModelId=396B329A022E
	void ConvertBSTR2char(BSTR bstrIn, char * charOut, long length)
	{
		long	Contador;
		Contador = 0;
		while (Contador <= length)
		{
			charOut[Contador] = (char)bstrIn[Contador];
			Contador +=1;
		}
		// Le finalizamos con cero
		charOut[Contador]=0;
	}


};

#endif //__M4BASE64_H_
