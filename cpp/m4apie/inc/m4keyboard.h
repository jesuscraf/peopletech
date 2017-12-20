// M4Keyboard.h : Declaration of the CM4Keyboard

#ifndef __M4KEYBOARD_H_
#define __M4KEYBOARD_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CM4Keyboard
//##ModelId=396B329401D8
class ATL_NO_VTABLE CM4Keyboard : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CM4Keyboard, &CLSID_M4Keyboard>,
	public IDispatchImpl<IM4Keyboard, &IID_IM4Keyboard, &LIBID_M4APIELib>
{
public:
	//##ModelId=396B32940261
	CM4Keyboard()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_M4KEYBOARD)

BEGIN_COM_MAP(CM4Keyboard)
	COM_INTERFACE_ENTRY(IM4Keyboard)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IM4Keyboard
public:
	//##ModelId=396B32940259
	STDMETHOD(KeyStatus)(/*[in]*/ M4KeyEnum nKey, /*[out, retval]*/ VARIANT_BOOL *pRet);
};

#endif //__M4KEYBOARD_H_
