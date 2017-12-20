// clcstimeout.cpp : implementation file
//

#include "stdafx.h"
#include "m4csdll.h"
#include "clcstimeout.h"
#include "clcstimeoutch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ClCSTimeOut dialog


ClCSTimeOut::ClCSTimeOut(CWnd* pParent /*=NULL*/)
	: CDialog(ClCSTimeOut::IDD, pParent)
{
	//{{AFX_DATA_INIT(ClCSTimeOut)
	//}}AFX_DATA_INIT
}


void ClCSTimeOut::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ClCSTimeOut)
	//}}AFX_DATA_MAP
}


BOOL ClCSTimeOut::OnInitDialog()
{
	BOOL bResult = CDialog::OnInitDialog();

	CString Buffer;
	char pcBuffer[128];

	_loadString( IDC_CS_TEXT1, Buffer ) ;
	SetWindowText(Buffer);

	_loadString( IDC_CS_STATIC1, Buffer ) ;
	sprintf(pcBuffer,Buffer,m_iTimeOut);
	::SetWindowText(::GetDlgItem(m_hWnd,IDC_CS_STATIC1),pcBuffer);

	_loadString( IDABORT, Buffer ) ;
	::SetWindowText(::GetDlgItem(m_hWnd,IDABORT),Buffer);

	_loadString( IDC_CS_CHANGE, Buffer ) ;
	::SetWindowText(::GetDlgItem(m_hWnd,IDC_CS_CHANGE),Buffer);

	_loadString( IDRETRY, Buffer ) ;
	::SetWindowText(::GetDlgItem(m_hWnd,IDRETRY),Buffer);

	return bResult;
}


BEGIN_MESSAGE_MAP(ClCSTimeOut, CDialog)
	//{{AFX_MSG_MAP(ClCSTimeOut)
	ON_BN_CLICKED(IDABORT, OnAbort)
	ON_BN_CLICKED(IDC_CS_CHANGE, OnChange)
	ON_BN_CLICKED(IDRETRY, OnRetry)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ClCSTimeOut message handlers

void ClCSTimeOut::OnAbort() 
{
	EndDialog(IDABORT);
}

void ClCSTimeOut::OnChange() 
{
	ClCSTimeOutChange oWindow;

	oWindow.SetValue(NULL, m_iTimeOut);

	switch(oWindow.DoModal())
	{
	case IDOK:
		m_iTimeOut = oWindow.GetValue();
		break;
		
	case IDCANCEL:
		return;

	default:
		return;
	}

	EndDialog(IDC_CS_CHANGE);
}

void ClCSTimeOut::OnRetry() 
{
	EndDialog(IDRETRY);
}

void ClCSTimeOut::SetTimeOut(const long ai_iTimeOut)
{
	m_iTimeOut = ai_iTimeOut;
}
