// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__E03EF4E5_1771_11D3_BDB1_00C04FA49636__INCLUDED_)
#define AFX_STDAFX_H__E03EF4E5_1771_11D3_BDB1_00C04FA49636__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define STRICT
#define _ATL_APARTMENT_THREADED


#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
#include "definitions.h"

EXTERN_C const CLSID CLSID_M4RegKey;

#ifdef __cplusplus

class DECLSPEC_UUID("15CCB9C3-7C63-11D1-8D15-444553540000")
M4RegKey;
#endif


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__E03EF4E5_1771_11D3_BDB1_00C04FA49636__INCLUDED)
