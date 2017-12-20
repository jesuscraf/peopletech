#if !defined(AFX_M4TREE32_H__64F5A7AE_BFA7_11D0_8DA1_00C04FB7146E__INCLUDED_)
#define AFX_M4TREE32_H__64F5A7AE_BFA7_11D0_8DA1_00C04FB7146E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// M4Tree32.h : main header file for M4TREE32.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CM4Tree32App : See M4Tree32.cpp for implementation.

class CM4Tree32App : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_M4TREE32_H__64F5A7AE_BFA7_11D0_8DA1_00C04FB7146E__INCLUDED)
