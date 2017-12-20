// M4UID.h : Declaration of the CM4UID

#ifndef __M4UID_H_
#define __M4UID_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CM4UID
//##ModelId=396B32940301
class ATL_NO_VTABLE CM4UID : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CM4UID, &CLSID_M4UID>,
	public IDispatchImpl<IM4UID, &IID_IM4UID, &LIBID_M4APIELib>
{
public:
	//##ModelId=396B329403AF
	CM4UID()
	{
		CoCreateGuid(&m_guid);
	}

DECLARE_REGISTRY_RESOURCEID(IDR_M4UID)

BEGIN_COM_MAP(CM4UID)
	COM_INTERFACE_ENTRY(IM4UID)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IM4UID
public:
	//##ModelId=396B329403A5
	STDMETHOD(NumberOfRefs)(/*[in, out]*/ IDispatch ** oObject, /*[out, retval]*/ long * pVal);
	//##ModelId=396B329403A2
	STDMETHOD(ReserveGUIDs)(/*[in]*/ long nNumber);
	//##ModelId=396B32940397
	STDMETHOD(get_M4UID)(/*[out, retval]*/ BSTR *pVal);
	//##ModelId=396B3294038D
	STDMETHOD(GenerateNewUID)();
protected:
	//##ModelId=396B3294035B
	GUID m_guid;
};

#endif //__M4UID_H_
