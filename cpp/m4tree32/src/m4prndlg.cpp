// M4PrnDlg.cpp : implementation file
//

#include "stdafx.h"
#include "m4prndlg.h"
#include "cm4tree.hpp"
#include "cm4bitmp.hpp"
#include "m4tree32ctl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CM4PrintDialog dialog


CM4PrintDialog::CM4PrintDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CM4PrintDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CM4PrintDialog)
	m_dScale = 3.0;
	m_lPage = 0L;
	//}}AFX_DATA_INIT
}


void CM4PrintDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CM4PrintDialog)
	DDX_Text(pDX, IDC_ED_SCALE, m_dScale);
	DDX_Text(pDX, IDC_ED_PAGE, m_lPage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CM4PrintDialog, CDialog)
	//{{AFX_MSG_MAP(CM4PrintDialog)
	ON_BN_CLICKED(IDC_BT_PRINTER, OnBtPrinter)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BT_CANCEL, OnBtCancel)
	ON_EN_UPDATE(IDC_ED_SCALE, OnUpdateEdScale)
	ON_BN_CLICKED(IDC_RD_TREE, OnRdTree)
	ON_BN_CLICKED(IDC_RD_WINDOW, OnRdWindow)
	ON_BN_CLICKED(IDC_RD_SUBTREE, OnRdSubtree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CM4PrintDialog message handlers

BOOL CM4PrintDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_bCancel = FALSE;
	m_bDialog = FALSE;
    m_pCDlg = new CPrintDialog(FALSE, PD_ALLPAGES | PD_NOSELECTION | PD_RETURNDC, NULL);
	if (m_pCDlg == NULL)
		return FALSE;
	ShowPreview ();

	GetDlgItem (IDC_BT_CANCEL)->ShowWindow (SW_HIDE);
	GetDlgItem (IDC_ST_PROGRESS)->ShowWindow (SW_HIDE);

	GetDlgItem (IDC_BT_PRINTER)->ShowWindow (SW_SHOW);
	GetDlgItem (IDOK)->ShowWindow (SW_SHOW);
	GetDlgItem (IDCANCEL)->ShowWindow (SW_SHOW);
	GetDlgItem (IDC_RD_WINDOW)->ShowWindow (SW_SHOW);
	GetDlgItem (IDC_RD_TREE)->ShowWindow (SW_SHOW);
	GetDlgItem (IDC_RD_SUBTREE)->ShowWindow (SW_SHOW);
	GetDlgItem (IDC_ST_FIELD)->ShowWindow (SW_SHOW);
	GetDlgItem (IDC_ST_SCALE)->ShowWindow (SW_SHOW);
	GetDlgItem (IDC_ED_SCALE)->ShowWindow (SW_SHOW);
	GetDlgItem (IDC_ST_PAGE)->ShowWindow (SW_SHOW);
	GetDlgItem (IDC_ED_PAGE)->ShowWindow (SW_SHOW);
	GetDlgItem (IDC_CK_BORDER)->ShowWindow (SW_SHOW);

	((CButton *) GetDlgItem (IDC_RD_TREE))->SetCheck(TRUE);
	((CButton *) GetDlgItem (IDC_CK_BORDER))->SetCheck(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CM4PrintDialog::OnOK() 
{
	Printing();
	CDialog::OnOK();
}

void CM4PrintDialog::OnCancel() 
{
	CDialog::OnCancel();
}

void CM4PrintDialog::OnBtPrinter() 
{
	if (m_pCDlg->DoModal() == TRUE)
	{
		m_bDialog = TRUE;
		ShowPreview ();
	}
}

void CM4PrintDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	M4DELETE(m_pCDlg);
}

void CM4PrintDialog::OnBtCancel() 
{
	m_bCancel = TRUE;
}

void CM4PrintDialog::OnUpdateEdScale() 
{
	UpdateData (TRUE);
	ShowPreview ();
}

void CM4PrintDialog::OnRdTree() 
{
	ShowPreview ();
}

void CM4PrintDialog::OnRdWindow() 
{
	ShowPreview ();
}

void CM4PrintDialog::OnRdSubtree() 
{
	ShowPreview ();
}


void CM4PrintDialog::MessageProcess (void)
{
	MSG Message;
	if (::PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
	{
		::TranslateMessage(&Message);
		::DispatchMessage(&Message);
	}
}

void CM4PrintDialog::ShowPreview (void)
{
	CSize cDim;
	long cxPage;
	long cyPage;
	if (GetPageSize (cxPage, cyPage) == FALSE)
		return;
	long lNumPagX = 0L;
	long lNumPagY = 0L;
	if (m_pTreeCtrl->GetPTree ())
	{
		if (((CButton *) GetDlgItem (IDC_RD_WINDOW))->GetCheck() == FALSE)
		{
			CRect rcView (1, 1, (int) ((((double) cxPage)*(double).75)-1.0), (int) ((((double) cyPage)*(double).75)-1.0));
			CM4Node *pNode;
			if (((CButton *) GetDlgItem (IDC_RD_SUBTREE))->GetCheck() == TRUE)
				pNode = m_pTreeCtrl->GetPTree ()->M4GetCurrentNode ();
			else
				pNode = m_pTreeCtrl->GetPTree ()->M4GetRootNode ();
			long lLeft, lTop, lRight, lBottom;
			pNode->M4GetRectTotal (lLeft, lTop, lRight, lBottom);
			long lWidthTree = (long) (((double) (lRight-lLeft+(m_pTreeCtrl->GetLeftOffset()*2.0)))*m_dScale);
			long lHeightTree = (long) (((double) (lBottom-lTop+(m_pTreeCtrl->GetTopOffset()*2.0)))*m_dScale);
			m_pTreeCtrl->OffsetRect (lLeft, lTop, lRight, lBottom);
			lNumPagX = (long) (((((double) lWidthTree))/((double) rcView.Width ()))+1.0);
			lNumPagY = (long) (((((double) lHeightTree))/((double) rcView.Height ()))+1.0);
		}
		else
			lNumPagX = lNumPagY = 1L;
	}
	m_lPage = lNumPagX*lNumPagY;
	UpdateData (FALSE);
}

BOOL CM4PrintDialog::GetPageSize(long &cxPage, long &cyPage)
{
	PRINTDLG FAR * pPrintDlg = new PRINTDLG;
	if (pPrintDlg == NULL)
		return FALSE;
 
 	if(AfxGetApp()->GetPrinterDeviceDefaults(pPrintDlg))
	{
 		DEVNAMES FAR *lpDevNames = (DEVNAMES FAR *)::GlobalLock(pPrintDlg->hDevNames);
		DEVMODE FAR *lpDevMode = (DEVMODE FAR *)::GlobalLock(pPrintDlg->hDevMode);
		CString szDriver((LPSTR)lpDevNames + lpDevNames->wDriverOffset);
		CString szDevice((LPSTR)lpDevNames + lpDevNames->wDeviceOffset);
		CString szOutput((LPSTR)lpDevNames + lpDevNames->wOutputOffset);
 
		CDC pDC;
		pDC.CreateDC(szDriver, szDevice, szOutput, lpDevMode);
 
		cxPage = ::GetDeviceCaps (pDC.m_hDC, HORZRES);
		cyPage = ::GetDeviceCaps (pDC.m_hDC, VERTRES);

		DeleteDC (pDC.m_hDC);
		::GlobalUnlock(pPrintDlg->hDevNames);
		::GlobalUnlock(pPrintDlg->hDevMode);
 
		M4DELETE(pPrintDlg);
		return TRUE;
	}
	cxPage = 0L;
	cyPage = 0L;
	M4DELETE(pPrintDlg);
	return FALSE;
}

BOOL CM4PrintDialog::Printing (void)
{
	UpdateData (TRUE);
	
	SetWindowText ("Imprimiendo el documento");
	
	GetDlgItem (IDC_BT_PRINTER)->ShowWindow (SW_HIDE);
	GetDlgItem (IDOK)->ShowWindow (SW_HIDE);
	GetDlgItem (IDCANCEL)->ShowWindow (SW_HIDE);
	GetDlgItem (IDC_RD_WINDOW)->ShowWindow (SW_HIDE);
	GetDlgItem (IDC_RD_TREE)->ShowWindow (SW_HIDE);
	GetDlgItem (IDC_RD_SUBTREE)->ShowWindow (SW_HIDE);
	GetDlgItem (IDC_ST_FIELD)->ShowWindow (SW_HIDE);
	GetDlgItem (IDC_ST_SCALE)->ShowWindow (SW_HIDE);
	GetDlgItem (IDC_ED_SCALE)->ShowWindow (SW_HIDE);
	GetDlgItem (IDC_ST_PAGE)->ShowWindow (SW_HIDE);
	GetDlgItem (IDC_ED_PAGE)->ShowWindow (SW_HIDE);
	GetDlgItem (IDC_CK_BORDER)->ShowWindow (SW_HIDE);

	GetDlgItem (IDC_BT_CANCEL)->ShowWindow (SW_SHOW);
	GetDlgItem (IDC_ST_PROGRESS)->ShowWindow (SW_SHOW);

	Invalidate ();
	UpdateWindow ();

    HDC hdcPrn;
	if (m_bDialog == FALSE)
		m_pCDlg->GetDefaults ();
	hdcPrn = m_pCDlg->GetPrinterDC();
	if (hdcPrn != NULL)
    {
        CDC *pDC = new CDC;
		if (pDC == NULL)
			return FALSE;
        pDC->Attach (hdcPrn);
        pDC->StartDoc("M4Tree OCX");
		long cxPage = ::GetDeviceCaps (hdcPrn, HORZRES);
		long cyPage = ::GetDeviceCaps (hdcPrn, VERTRES);
		pDC->SetMapMode (MM_ISOTROPIC);
		pDC->SetWindowExt (1000, 1000);
		pDC->SetViewportExt ((int) (cxPage/2), (int) (cyPage/2));
		pDC->SetViewportOrg ((int) (cxPage/2), (int) (cyPage/2));
		if (!(GetDeviceCaps(pDC->m_hDC, RASTERCAPS) & RC_BITBLT)) 
		{ 
			DeleteDC(pDC->m_hDC); 
			return FALSE;
		}
		else
		{
			if (m_pTreeCtrl->GetPTree ())
			{
				m_pTreeCtrl->SetPrinting(TRUE);

				CDC *pDCMen = new CDC;
				if (pDCMen == NULL)
					return FALSE;
				CDC *pdc = m_pTreeCtrl->GetDC ();
				pDCMen->CreateCompatibleDC (pdc);

				CBitmap *pBitmap = new CBitmap;
				if (pBitmap == NULL)
				{
					M4DELETE(pDCMen);
					return FALSE;
				}
				CBitmap *oldBitmap;
				if (((CButton *) GetDlgItem (IDC_RD_WINDOW))->GetCheck() == FALSE)
				{
					double dScale = m_pTreeCtrl->GetScale ();
					m_pTreeCtrl->SetScale(m_dScale);
					CRect rcView;
					rcView.left = 0;
					rcView.top = 0;
					rcView.right = (int) (((double) cxPage)*(double).75);
					rcView.bottom = (int) (((double) cyPage)*(double).75);

					pBitmap->CreateCompatibleBitmap (pdc, rcView.Width (), rcView.Height ());
					oldBitmap = pDCMen->SelectObject (pBitmap);

					CRect rcBorder = rcView;
					rcView.left++;
					rcView.top++;
					rcView.right--;
					rcView.bottom--;
					long lOffsetScrollX = m_pTreeCtrl->GetOffsetScrollX();
					long lOffsetScrollY = m_pTreeCtrl->GetOffsetScrollY();
					m_pTreeCtrl->SetOffsetScrollX(0L);
					m_pTreeCtrl->SetOffsetScrollY(0L);
					CM4Node *pNode;
					if (((CButton *) GetDlgItem (IDC_RD_SUBTREE))->GetCheck() == TRUE)
						pNode = m_pTreeCtrl->GetPTree ()->M4GetCurrentNode ();
					else
						pNode = m_pTreeCtrl->GetPTree ()->M4GetRootNode ();
					long lLeft, lTop, lRight, lBottom;
					pNode->M4GetRectTotal (lLeft, lTop, lRight, lBottom);
					long lWidthTree = (long) (((double) (lRight-lLeft+(m_pTreeCtrl->GetLeftOffset()*2.0)))*m_dScale);
					long lHeightTree = (long) (((double) (lBottom-lTop+(m_pTreeCtrl->GetTopOffset()*2.0)))*m_dScale);
					m_pTreeCtrl->OffsetRect (lLeft, lTop, lRight, lBottom);

					long lNumPagX = (long) (((((double) lWidthTree))/((double) rcView.Width ()))+1.0);
					long lNumPagY = (long) (((((double) lHeightTree))/((double) rcView.Height ()))+1.0);
					if (lWidthTree > rcView.Width ())
						m_pTreeCtrl->SetOffsetScrollX((long) ((double) (-lLeft+(((rcView.Width()*lNumPagX)-lWidthTree)/2))+(((double) m_pTreeCtrl->GetLeftOffset())*m_dScale)));
					else
						m_pTreeCtrl->SetOffsetScrollX((long) ((double) (-lLeft+((rcView.Width()-lWidthTree)/2))+(((double) m_pTreeCtrl->GetLeftOffset())*m_dScale)));
					if (lHeightTree > rcView.Height ())
						m_pTreeCtrl->SetOffsetScrollY((long) ((double) (-lTop+(((rcView.Height()*lNumPagY)-lHeightTree)/2))+(((double) m_pTreeCtrl->GetTopOffset())*m_dScale)));
					else
						m_pTreeCtrl->SetOffsetScrollY((long) ((double) (-lTop+((rcView.Height()-lHeightTree)/2))+(((double) m_pTreeCtrl->GetTopOffset())*m_dScale)));
    				for (long j=0L; j<lNumPagY; j++)
					{
						for (long i=0L; i<lNumPagX; i++)
						{
							if (m_bCancel == FALSE)
							{
								CString cProgress;
								sprintf (cProgress.GetBuffer (50), "Imprimiendo la página %ld de %ld.", i+j*lNumPagX+1, lNumPagX*lNumPagY);
								cProgress.ReleaseBuffer ();
								SetDlgItemText (IDC_ST_PROGRESS, cProgress);
								pDC->StartPage();
								m_pTreeCtrl->FillSolidRect(pDCMen, rcBorder.left, rcBorder.top, rcBorder.Width (), rcBorder.Height (),  RGB(255,255,255));//TranslateColor(GetBackColor()));
								m_pTreeCtrl->DrawTree (pDCMen, pNode, rcView);
								MessageProcess ();
								if (((CButton *) GetDlgItem (IDC_CK_BORDER))->GetCheck() == TRUE)
									m_pTreeCtrl->Draw3dRect (pDCMen, rcBorder, RGB (0,0,0), RGB(0,0,0));
								pDC->BitBlt ((int) (-rcBorder.Width ()/2), (int) (-rcBorder.Height ()/2), rcBorder.Width (), rcBorder.Height (), pDCMen, 0, 0, SRCCOPY);
								pDC->EndPage();
								m_pTreeCtrl->SetOffsetScrollX(m_pTreeCtrl->GetOffsetScrollX ()-rcBorder.Width ()-1);
								MessageProcess ();
							}
						}	
						if (lWidthTree > rcView.Width ())
							m_pTreeCtrl->SetOffsetScrollX(-lLeft+(((rcView.Width()*lNumPagX)-lWidthTree)/2));
						else
							m_pTreeCtrl->SetOffsetScrollX(-lLeft+((rcView.Width()-lWidthTree)/2));
						m_pTreeCtrl->SetOffsetScrollY(m_pTreeCtrl->GetOffsetScrollY ()-rcBorder.Height ()-1);
					}
					m_pTreeCtrl->SetOffsetScrollX(lOffsetScrollX);
					m_pTreeCtrl->SetOffsetScrollY(lOffsetScrollY);
					m_pTreeCtrl->SetScale(dScale);
				}
				else
				{
					CRect rcView;
					m_pTreeCtrl->GetClientRect (&rcView);
					CRect rcMaxView = rcView; 
					rcMaxView.left = (int) (((double) rcMaxView.left)*m_dScale);
					rcMaxView.top = (int) (((double) rcMaxView.top)*m_dScale);
					rcMaxView.right = (int) (((double) rcMaxView.right)*m_dScale);
					rcMaxView.bottom = (int) (((double) rcMaxView.bottom)*m_dScale);

					pBitmap->CreateCompatibleBitmap (pdc, rcMaxView.Width (), rcMaxView.Height ());
					oldBitmap = pDCMen->SelectObject (pBitmap);

					CRect rcBorder = rcView;
					rcView.left++;
					rcView.top++;
					rcView.right--;
					rcView.bottom--;
					SetDlgItemText (IDC_ST_PROGRESS, "Imprimiendo la página 1 de 1.");
					pDC->StartPage();
					m_pTreeCtrl->FillSolidRect(pDCMen, rcBorder.left, rcBorder.top, rcBorder.Width (), rcBorder.Height (),  RGB(255,255,255));//TranslateColor(GetBackColor()));
					m_pTreeCtrl->DrawTree (pDCMen, m_pTreeCtrl->GetPTree ()->M4GetRootNode (), rcView);
					if (((CButton *) GetDlgItem (IDC_CK_BORDER))->GetCheck() == TRUE)
						m_pTreeCtrl->Draw3dRect (pDCMen, rcBorder, RGB (0,0,0), RGB(0,0,0));
					pDC->StretchBlt (-rcMaxView.Width ()/2, -rcMaxView.Height ()/2, rcMaxView.Width (), rcMaxView.Height (), pDCMen, 0, 0, rcBorder.Width (), rcBorder.Height (), SRCCOPY);
					pDC->EndPage();
				}
				m_pTreeCtrl->SetPrinting(FALSE);

				pDCMen->SelectObject (oldBitmap);
				DeleteObject (pBitmap->m_hObject);
				M4DELETE(pBitmap);
		
				DeleteDC (pDCMen->m_hDC);
				M4DELETE(pDCMen);
			}
		}
		if (m_bCancel == FALSE)
	        pDC->EndDoc();
		else
	        pDC->AbortDoc();
        pDC->Detach();
        M4DELETE(pDC);
    }
	return TRUE;
}

