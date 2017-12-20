// clcstimeoutch.cpp : implementation file
//

#include "stdafx.h"
#include "m4csdll.h"
#include "clcstimeoutch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ClCSTimeOutChange dialog


ClCSTimeOutChange::ClCSTimeOutChange(CWnd* pParent /*=NULL*/)
	: CDialog(ClCSTimeOutChange::IDD, pParent)
{
	//{{AFX_DATA_INIT(ClCSTimeOutChange)
	m_Edit1 = 0;
	m_Check1 = FALSE;
	//}}AFX_DATA_INIT
	m_iIsDone = 0;
}


void ClCSTimeOutChange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ClCSTimeOutChange)
	DDX_Text(pDX, IDC_CS_EDIT1, m_Edit1);
	DDV_MinMaxUInt(pDX, m_Edit1, 1, 86400);
	DDX_Check(pDX, IDC_CS_CHECK1, m_Check1);
	//}}AFX_DATA_MAP
}


BOOL ClCSTimeOutChange::OnInitDialog()
{
	BOOL bResult = CDialog::OnInitDialog();

	CString Buffer;

	_loadString( IDC_CS_TEXT2, Buffer ) ;
	SetWindowText(Buffer);

	_loadString( IDC_CS_STATIC2, Buffer ) ;
	::SetWindowText(::GetDlgItem(m_hWnd,IDC_CS_STATIC2),Buffer);

	_loadString( IDC_CS_STATIC3, Buffer ) ;
	::SetWindowText(::GetDlgItem(m_hWnd,IDC_CS_STATIC3),Buffer);

	_loadString( IDOK, Buffer ) ;
	::SetWindowText(::GetDlgItem(m_hWnd,IDOK),Buffer);

	_loadString( IDCANCEL, Buffer ) ;
	::SetWindowText(::GetDlgItem(m_hWnd,IDCANCEL),Buffer);

	return bResult;
}



BEGIN_MESSAGE_MAP(ClCSTimeOutChange, CDialog)
	//{{AFX_MSG_MAP(ClCSTimeOutChange)
	ON_EN_CHANGE(IDC_CS_EDIT1, OnChangeCsEdit1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CS_SPIN1, OnDeltaposCsSpin1)
	ON_BN_CLICKED(IDC_CS_CHECK1, OnCsCheck1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ClCSTimeOutChange message handlers

void ClCSTimeOutChange::OnCancel() 
{

	CDialog::OnCancel();

	m_iIsDone = 1;
	
}

void ClCSTimeOutChange::OnOK() 
{
	CDialog::OnOK();

	m_iIsDone = 1;
}

long ClCSTimeOutChange::GetValue()
{
	if (!m_iIsDone)
		return -1;
	return m_Check1 ? 0 : m_Edit1;
}

void ClCSTimeOutChange::SetValue(const char *ai_pcMessage, const long ai_iCurrent)
{
	m_Edit1 = (UINT) ai_iCurrent;

}

void ClCSTimeOutChange::OnDeltaposCsSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	long iNewValue = m_Edit1 - pNMUpDown->iDelta*30;
	iNewValue -= iNewValue % 30;
	if (iNewValue < 1)
		iNewValue = 1;
	else
		if (iNewValue > SECS_IN_A_DAY)
			iNewValue = SECS_IN_A_DAY;

	m_Edit1 = iNewValue;

	*pResult = 0;

	UpdateData(FALSE);

}


void ClCSTimeOutChange::OnChangeCsEdit1() 
{
	UpdateData();
}


void ClCSTimeOutChange::OnCsCheck1() 
{
	if(!UpdateData())
	{
		m_Check1 = FALSE;
		UpdateData(FALSE);
		return;
	}

	if (m_Check1)
	{
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_CS_STATIC2),FALSE);
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_CS_EDIT1  ),FALSE);
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_CS_SPIN1  ),FALSE);
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_CS_STATIC3),FALSE);
	}
	else
	{
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_CS_STATIC2),TRUE);
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_CS_EDIT1  ),TRUE);
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_CS_SPIN1  ),TRUE);
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_CS_STATIC3),TRUE);
	}
}
