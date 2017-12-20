// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__135B2DF4_E60B_11D3_BDB5_00C04F71652D__INCLUDED_)
#define AFX_STDAFX_H__135B2DF4_E60B_11D3_BDB5_00C04F71652D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

#define RELEASE(p) if (p) {p->Release(); p=NULL;}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__135B2DF4_E60B_11D3_BDB5_00C04F71652D__INCLUDED)
