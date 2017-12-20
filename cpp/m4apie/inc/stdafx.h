// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__F8088408_C268_11D1_9633_00C04FC2444B__INCLUDED_)
#define AFX_STDAFX_H__F8088408_C268_11D1_9633_00C04FC2444B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define STRICT

#include <afxcmn.h>
#include <afxwin.h>
#include <afxdlgs.h>
#include <afxdisp.h>
#include "definitions.h"

#define _ATL_APARTMENT_THREADED

/////////////////////////////  JFM    //////////////////////
//const IID IID_IM4FontHolder = {0x06854C11,0x088F,0x11D3,{0xBC,0x9D,0x00,0xC0,0x4F,0x71,0x65,0x2D}};

//const IID IID_ICPropBag = {0x13313658,0x2584,0x11D2,{0x96,0x9E,0x00,0xC0,0x4F,0xC2,0x44,0x4B}};

#define		JFM_MAX_CHAR	512

/////////////////////////////  JFM    //////////////////////


#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
#include <ocidl.h>	// Added by ClassView


EXTERN_C const CLSID CLSID_CPropBag;

#ifdef __cplusplus

class DECLSPEC_UUID("13313659-2584-11D2-969E-00C04FC2444B")
CPropBag;
#endif



//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__F8088408_C268_11D1_9633_00C04FC2444B__INCLUDED)
