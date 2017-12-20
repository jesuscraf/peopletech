#if !defined(AFX_CLCSTIMEOUT_H__4A271363_7F95_11D2_8DBF_00C04FA9BAF9__INCLUDED_)
#define AFX_CLCSTIMEOUT_H__4A271363_7F95_11D2_8DBF_00C04FA9BAF9__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// clcstimeout.h : header file
//

#include "m4csdllmsgs.h"

/////////////////////////////////////////////////////////////////////////////
// ClCSTimeOut dialog

class ClCSTimeOut : public CDialog
{
// Construction
public:
	void SetTimeOut(const long ai_iTimeOut);
	long GetTimeOut(void) { return m_iTimeOut; }
	ClCSTimeOut(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ClCSTimeOut)
	enum { IDD = IDD_CS_TEXT1 };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ClCSTimeOut)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	long m_iTimeOut;

	// Generated message map functions
	//{{AFX_MSG(ClCSTimeOut)
	afx_msg void OnAbort();
	afx_msg void OnChange();
	afx_msg void OnRetry();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLCSTIMEOUT_H__4A271363_7F95_11D2_8DBF_00C04FA9BAF9__INCLUDED_)
