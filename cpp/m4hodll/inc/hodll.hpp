//
//Código implementado por Eduardo Fernandes.
//


// hodll.h : main header file for the HODLL DLL
//

#if !defined(AFX_HODLL_H__B2A458DC_71E2_47D5_9EA0_58385D558643__INCLUDED_)
#define AFX_HODLL_H__B2A458DC_71E2_47D5_9EA0_58385D558643__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "m4hodll_dll.hpp"

/////////////////////////////////////////////////////////////////////////////
// CHodllApp
// See hodll.cpp for the implementation of this class
//
extern "C" M4_DECL_M4HODLL LRESULT KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
extern "C" M4_DECL_M4HODLL BOOL InstallHook();
extern "C" M4_DECL_M4HODLL BOOL UnHook();


class CHodllApp : public CWinApp
{
public:
BOOL ExitInstance ();
	CHodllApp();
  BOOL InitInstance ();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHodllApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CHodllApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HODLL_H__B2A458DC_71E2_47D5_9EA0_58385D558643__INCLUDED_)
