// M4Design.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f M4Designps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "m4design.h"
#include "m4design_i.c"
#include "handle.h"
#include "line.h"
#include "hook_data.h"
#include "lineman.h"
#include "dline.h"
#include "linecol.h"
#include "dpoint.h"


CComModule _Module;

HBITMAP hBitmapEnabled;
HBITMAP hBitmapDisabled;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_LineMan, CLineMan)
OBJECT_ENTRY(CLSID_DesignLine, CDLine)
OBJECT_ENTRY(CLSID_LineCol, CLineCol)
OBJECT_ENTRY(CLSID_DPoint, CDPoint)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _Module.Init(ObjectMap, hInstance, &LIBID_M4DESIGNLib);
		hBitmapEnabled = LoadBitmap(_Module.m_hInstResource, MAKEINTRESOURCE(IDB_HANDLEENABLED));
		hBitmapDisabled = LoadBitmap(_Module.m_hInstResource, MAKEINTRESOURCE(IDB_HANDLEDISABLED));
        DisableThreadLibraryCalls(hInstance);

    }
    else if (dwReason == DLL_PROCESS_DETACH)
	{
		DeleteObject(hBitmapEnabled);
		DeleteObject(hBitmapDisabled);
		_Module.Term();
	}
        
    return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
    return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
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
    return _Module.UnregisterServer(TRUE);
}

HBITMAP GetEnabledBitmap()
{
	return hBitmapEnabled;
}

HBITMAP GetDisabledBitmap()
{
	return hBitmapDisabled;
}
