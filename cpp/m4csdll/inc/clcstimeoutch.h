#if !defined(AFX_CLCSTIMEOUTCH_H__4A271365_7F95_11D2_8DBF_00C04FA9BAF9__INCLUDED_)
#define AFX_CLCSTIMEOUTCH_H__4A271365_7F95_11D2_8DBF_00C04FA9BAF9__INCLUDED_

#define SECS_IN_A_DAY 86400l

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// clcstimeoutch.h : header file
//

#include "m4csdllmsgs.h"

/////////////////////////////////////////////////////////////////////////////
// ClCSTimeOutChange dialog

class ClCSTimeOutChange : public CDialog
{
// Construction
public:
	void SetValue(const char *ai_pcMessage, const long ai_iCurrent);
	long GetValue();
	ClCSTimeOutChange(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ClCSTimeOutChange)
	enum { IDD = IDD_CS_TEXT2 };
	UINT	m_Edit1;
	BOOL	m_Check1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ClCSTimeOutChange)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ClCSTimeOutChange)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeCsEdit1();
	afx_msg void OnDeltaposCsSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCsCheck1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_iIsDone;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLCSTIMEOUTCH_H__4A271365_7F95_11D2_8DBF_00C04FA9BAF9__INCLUDED_)
