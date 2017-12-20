// M4PrnDlg.h : header file
//

#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// CM4PrintDialog dialog

class CM4Tree32Ctrl;

class CM4PrintDialog : public CDialog
{
	CPrintDialog *m_pCDlg;
	BOOL m_bCancel;
	BOOL m_bDialog;

protected:
	BOOL Printing (void);
	void MessageProcess (void);
	void ShowPreview (void);
	BOOL GetPageSize(long &cxPage, long &cyPage);

// Construction
public:
	CM4Tree32Ctrl *m_pTreeCtrl;
	CM4PrintDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CM4PrintDialog)
	enum { IDD = IDD_DG_PRINT };
	double	m_dScale;
	long	m_lPage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CM4PrintDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CM4PrintDialog)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBtPrinter();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBtCancel();
	afx_msg void OnUpdateEdScale();
	afx_msg void OnRdTree();
	afx_msg void OnRdWindow();
	afx_msg void OnRdSubtree();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
