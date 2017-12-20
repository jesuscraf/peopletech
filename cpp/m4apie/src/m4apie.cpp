// M4APIE.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//		To build a separate proxy/stub DLL, 
//		run nmake -f M4APIEps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include "initguid.h"
#include "m4apie.h"
#include "m4apie_i.c"
#include "graphop.h"
#include "m4inireg.hpp"
#include "m4regkey.h"
#include "m4uid.h"
#include "_base64.h"
#include "m4base64.h"
#include "colorselect.h"
#include "m4keyboard.h"
#include "fontselect.h"
#include "dispatchconvert.h"
#include "m4general.h"
#include "m4xmlregistry.h"
#include "m4fontholder.h"


CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
	OBJECT_ENTRY(CLSID_GraphOp, CGraphOp)
	OBJECT_ENTRY(CLSID_M4RegKey, CM4RegKey)
	OBJECT_ENTRY(CLSID_M4UID, CM4UID)
	OBJECT_ENTRY(CLSID_M4Base64, CM4Base64)
	//OBJECT_ENTRY(CLSID_MemStat, CMemStat)
	//OBJECT_ENTRY(CLSID_Animate, CAnimate)
	OBJECT_ENTRY(CLSID_ColorSelect, CColorSelect)
	//OBJECT_ENTRY(CLSID_LocalID, CLocalID)
	OBJECT_ENTRY(CLSID_M4Keyboard, CM4Keyboard)
	OBJECT_ENTRY(CLSID_FontSelect, CFontSelect)
	OBJECT_ENTRY(CLSID_DispatchConvert, CDispatchConvert)
	OBJECT_ENTRY(CLSID_M4General, CM4General)
	OBJECT_ENTRY(CLSID_M4XmlRegistry, CM4XmlRegistry)
	OBJECT_ENTRY(CLSID_M4FontHolder, CM4FontHolder)
END_OBJECT_MAP()

//##ModelId=396B329800FE
class CM4APIEApp : public CWinApp
{
public:
	//##ModelId=396B32980112
	virtual BOOL InitInstance();
	//##ModelId=396B32980109
	virtual int ExitInstance();
};

CM4APIEApp theApp;

//##ModelId=396B32980112
BOOL CM4APIEApp::InitInstance()
{
	_Module.Init(ObjectMap, m_hInstance);
	return CWinApp::InitInstance();
}

//##ModelId=396B32980109
int CM4APIEApp::ExitInstance()
{
	_Module.Term();
	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (AfxDllCanUnloadNow()==S_OK && _Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	// registers object, typelib and all interfaces in typelib
	return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	_Module.UnregisterServer();
	return S_OK;
}

