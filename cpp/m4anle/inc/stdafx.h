// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__AA9A1765_CB03_11D2_BC56_00C04FA92178__INCLUDED_)
#define AFX_STDAFX_H__AA9A1765_CB03_11D2_BC56_00C04FA92178__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define STRICT

#include <afxwin.h>
#include <afxdisp.h>

#include <stdlib.h>
#include <stdio.h>
#include "m4stl.hpp"
#include "definitions.h"

#define RELEASE(p) if (p) {p->Release(); p= NULL;}
#define RELEASEBSTR(p) if (p) {::SysFreeString(p); p = NULL;}

#define _ATL_APARTMENT_THREADED

// Definiciones de los ClassID que se usan en éste proyecto

EXTERN_C const CLSID CLSID_CPropBag;

#ifdef __cplusplus

class DECLSPEC_UUID("13313659-2584-11D2-969E-00C04FC2444B")
CPropBag;
#endif

EXTERN_C const CLSID CLSID_ConvertPropBag;

#ifdef __cplusplus

class DECLSPEC_UUID("1331365D-2584-11D2-969E-00C04FC2444B")
ConvertPropBag;
#endif

EXTERN_C const CLSID CLSID_CPropBagDef;

#ifdef __cplusplus

class DECLSPEC_UUID("2D3708E9-64D8-11D2-BC7F-00C04F71652D")
CPropBagDef;
#endif

EXTERN_C const CLSID CLSID_DataProperties;

#ifdef __cplusplus

class DECLSPEC_UUID("B4981954-0D0C-11D3-BCAA-00C04F71652D")
DataProperties;
#endif

EXTERN_C const CLSID CLSID_TraductProp;

#ifdef __cplusplus

class DECLSPEC_UUID("D3A42354-0DC6-11D3-BCAB-00C04F71652D")
TraductProp;
#endif

EXTERN_C const CLSID CLSID_M4FontHolder;

#ifdef __cplusplus

class DECLSPEC_UUID("06854C14-088F-11D3-BC9D-00C04F71652D")
M4FontHolder;
#endif

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

void M4BSTRToCString( const BSTR& ai_oString, CString& ao_roOutput );


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__AA9A1765_CB03_11D2_BC56_00C04FA92178__INCLUDED)
