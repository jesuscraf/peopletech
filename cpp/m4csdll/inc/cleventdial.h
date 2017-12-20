#if !defined(AFX_LEVENTDIAL_H__BFC6FC92_2877_11D3_BC9D_00C04F62FB03__INCLUDED_)
#define AFX_LEVENTDIAL_H__BFC6FC92_2877_11D3_BC9D_00C04F62FB03__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// leventdial.h : header file
//
#include "stdafx.h"
#include "resource.h"
#include "m4csdllmsgs.h"
#include <csexternal.hpp>

/////////////////////////////////////////////////////////////////////////////
// ClEventDial dialog

class ClEventDial : public CDialog
{
// Construction
public:
	ClEventDial(CWnd* pParent = NULL);   // standard constructor
	void SetParam ( const void *ai_iDeferedTime ) ;
	void SetEvent ( const int ai_iEvent ) ;

// Dialog Data
	//{{AFX_DATA(ClEventDial)
//	enum { IDD = IDD_SRV_EVENT1 };
	enum { IDD = IDD_CS_TEXT3};
	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ClEventDial)

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	BOOL OnInitDialog() ;

// Implementation
protected:
	
	void * m_pParam ;
	int m_iEvent ;
	// Generated message map functions
	//{{AFX_MSG(ClEventDial)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEVENTDIAL_H__BFC6FC92_2877_11D3_BC9D_00C04F62FB03__INCLUDED_)
