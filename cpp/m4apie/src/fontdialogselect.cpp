// FontDialogSelect.cpp : implementation file
//

#include "stdafx.h"
#include "m4apie.h"
#include <dlgs.h>
#include "fontdialogselect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFontDialogSelect

IMPLEMENT_DYNAMIC(CFontDialogSelect, CFontDialog)

CFontDialogSelect::CFontDialogSelect(LPLOGFONT lplfInitial, DWORD dwFlags, CDC* pdcPrinter, CWnd* pParentWnd) : 
	CFontDialog(lplfInitial, dwFlags, pdcPrinter, pParentWnd)
{
}


BEGIN_MESSAGE_MAP(CFontDialogSelect, CFontDialog)
	//{{AFX_MSG_MAP(CFontDialogSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//##ModelId=396B329A0008
BOOL CFontDialogSelect::OnInitDialog() 
{
	CFontDialog::OnInitDialog();
	
	long chLong;
	CWnd *pWnd;

	// Deshabilitamos el combo de selección de color
	pWnd = GetDlgItem(cmb4);
	chLong = GetWindowLong(pWnd->m_hWnd, GWL_STYLE);
	chLong |= WS_DISABLED;
	chLong ^= WS_VISIBLE;
	SetWindowLong(pWnd->m_hWnd, GWL_STYLE, chLong);
	// Le quitamos la label
	pWnd = GetDlgItem(stc4);
	SetWindowLong(pWnd->m_hWnd, GWL_STYLE, WS_DISABLED);

	// Deshabilitamos el combo de selección de Script
	pWnd = GetDlgItem(cmb5);
	chLong = GetWindowLong(pWnd->m_hWnd, GWL_STYLE);
	SetWindowLong(pWnd->m_hWnd, GWL_STYLE,  chLong | WS_DISABLED);

	//pWnd = GetDlgItem(stc7);
	//SetWindowLong(pWnd->m_hWnd, GWL_STYLE, WS_DISABLED);
	
	// Quitamos el Strike y movemos la de FontUnderline
	//CRect rc, rc2;
	
	pWnd = GetDlgItem(chx1);
	//pWnd->GetWindowRect(rc);
	chLong = GetWindowLong(pWnd->m_hWnd, GWL_STYLE);
	SetWindowLong(pWnd->m_hWnd, GWL_STYLE,  chLong | WS_DISABLED);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
