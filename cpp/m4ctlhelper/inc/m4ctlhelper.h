//==============================================================================
//
// (c) Copyright  1991-2009 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4ctlhelper
// File:             m4ctlhelper.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:             18/11/2009
// Language:         C++
// Operating System: Windows
// Design Document:
//
//	Owner:			Antonio Durán
//	Modifications:
//
// Definition:
//
//    Defines the initialization routines and API for the DLL.
//	  Main header file for the M4CTLHELPER DLL
//
//
//==============================================================================

//

#if !defined(AFX_M4CTLHELPER_H__F813D495_9E5D_461E_A0DD_B2074AE455BD__INCLUDED_)
#define AFX_M4CTLHELPER_H__F813D495_9E5D_461E_A0DD_B2074AE455BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif


/////////////////////////////////////////////////////////////////////////////
// CM4ctlhelperApp
// See m4ctlhelper.cpp for the implementation of this class
//

class CM4ctlhelperApp : public CWinApp
{
public:
	CM4ctlhelperApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CM4ctlhelperApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CM4ctlhelperApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_M4CTLHELPER_H__F813D495_9E5D_461E_A0DD_B2074AE455BD__INCLUDED_)
