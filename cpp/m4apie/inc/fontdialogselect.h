#if !defined(AFX_FONTDIALOGSELECT_H__4806B307_941D_11D2_BCC3_00C04F71652D__INCLUDED_)
#define AFX_FONTDIALOGSELECT_H__4806B307_941D_11D2_BCC3_00C04F71652D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FontDialogSelect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFontDialogSelect dialog

//##ModelId=396B329903E5
class CFontDialogSelect : public CFontDialog
{
	DECLARE_DYNAMIC(CFontDialogSelect)

public:
	CFontDialogSelect(LPLOGFONT lplfInitial = NULL,
		DWORD dwFlags = CF_EFFECTS | CF_SCREENFONTS,
		CDC* pdcPrinter = NULL,
		CWnd* pParentWnd = NULL);
#ifndef _AFX_NO_RICHEDIT_SUPPORT
	CFontDialogSelect(const CHARFORMAT& charformat,
		DWORD dwFlags = CF_SCREENFONTS,
		CDC* pdcPrinter = NULL,
		CWnd* pParentWnd = NULL);
#endif

protected:
	//{{AFX_MSG(CFontDialogSelect)
	//##ModelId=396B329A0008
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FONTDIALOGSELECT_H__4806B307_941D_11D2_BCC3_00C04F71652D__INCLUDED_)
