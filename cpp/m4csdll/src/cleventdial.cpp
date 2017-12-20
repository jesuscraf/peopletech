// leventdial.cpp : implementation file
//

#include "stdafx.h"
#include "m4csdll.h"
#include "cleventdial.h"
#include <csexternal.hpp>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_PARAM_LEN 200
/////////////////////////////////////////////////////////////////////////////
// ClEventDial dialog


ClEventDial::ClEventDial(CWnd* pParent /*=NULL*/)
	: CDialog(ClEventDial::IDD, pParent)
{
	//{{AFX_DATA_INIT(ClEventDial)
	m_pParam = NULL ;
	m_iEvent = 0;
			// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void ClEventDial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ClEventDial)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ClEventDial, CDialog)
	//{{AFX_MSG_MAP(ClEventDial)
	ON_BN_CLICKED(IDOK, OnOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ClEventDial message handlers

void ClEventDial::OnOK() 
{
	// TODO: Add extra validation here
	EndDialog(0);
	CDialog::OnOK();
}


BOOL ClEventDial::OnInitDialog()
{
	BOOL bResult = CDialog::OnInitDialog();
	WINDOWPLACEMENT wPlacement ;
	CString Buffer;
	int iDeferedTime = 0;
	int iTimeout ;
	char pcBuffer[MAX_PARAM_LEN];
	char * szHost = NULL;
	char * szPort = NULL;
	

	switch ( m_iEvent )
	{
	case M4_CS_EVENT_SERVER_SHUTDOWN :
		//-------------------------------
		//Escribir el título de la ventana
		_loadString( IDC_CS_TEXT3, Buffer ) ;
		SetWindowText(Buffer);
		//-------------------------------

		if ( ! sscanf ( (char*) m_pParam , "%d" , &iDeferedTime ) ) 
		{
			iDeferedTime = -1 ;
		}

		if ( iDeferedTime> 0)
		{
			_loadString( IDC_CS_STATIC31, Buffer ) ;
			sprintf(pcBuffer,Buffer,iDeferedTime);
			::SetWindowText(::GetDlgItem(m_hWnd,IDC_CS_STATIC1),pcBuffer);

			::GetWindowPlacement ( ::GetDlgItem(m_hWnd,IDC_STATIC1) , &wPlacement ) ;
			wPlacement.showCmd = SW_SHOW ;
			::SetWindowPlacement ( ::GetDlgItem(m_hWnd,IDC_STATIC1) , &wPlacement ) ;

		}
		else
		{
			_loadString( IDC_CS_STATIC32, Buffer ) ;
			::SetWindowText(::GetDlgItem(m_hWnd,IDC_CS_STATIC1),Buffer);

			::GetWindowPlacement ( ::GetDlgItem(m_hWnd,IDC_STATIC2) , &wPlacement ) ;
			wPlacement.showCmd = SW_SHOW;
			::SetWindowPlacement ( ::GetDlgItem(m_hWnd,IDC_STATIC2) , &wPlacement ) ;
		}

		break ;

	case M4_CS_EVENT_SERVER_RUPTURE_CONNECTION	:

		//-------------------------------
		//Escribir el título de la ventana
		_loadString( IDC_CS_TEXT3, Buffer ) ;
		SetWindowText(Buffer);
		//-------------------------------

		szPort = strstr ( (char*)m_pParam , ":" ) ;

		if ( ! szPort  )
		{
			szHost = new char [ strlen ( "'Unknown'") +1 ] ;
			szPort = szHost ;
			strcpy ( szPort , "'Unknown'" ) ;
		}else
		{
			szHost = (char*)m_pParam ;
			szPort[0] = 0;
			szPort ++ ;
		}

		_loadString( IDC_CS_STATIC33, Buffer ) ;
		sprintf(pcBuffer,Buffer,szHost,szPort);
		::SetWindowText(::GetDlgItem(m_hWnd,IDC_CS_STATIC1),pcBuffer);

		::GetWindowPlacement ( ::GetDlgItem(m_hWnd,IDC_STATIC1) , &wPlacement ) ;
		wPlacement.showCmd = SW_SHOW;
		::SetWindowPlacement ( ::GetDlgItem(m_hWnd,IDC_STATIC1) , &wPlacement ) ;

		break ;

	case M4_CS_EVENT_SESSION_TIMEOUT :

		//-------------------------------
		//Escribir el título de la ventana
		_loadString( IDC_CS_TEXT3, Buffer ) ;
		SetWindowText(Buffer);
		//-------------------------------

		sscanf ( (char*)m_pParam , "%d" , &iTimeout ) ;

		_loadString( IDC_CS_STATIC34, Buffer ) ;
		sprintf(pcBuffer,Buffer,iTimeout ) ;
		::SetWindowText(::GetDlgItem(m_hWnd,IDC_CS_STATIC1),pcBuffer);

		::GetWindowPlacement ( ::GetDlgItem(m_hWnd,IDC_STATIC1) , &wPlacement ) ;
		wPlacement.showCmd = SW_SHOW;
		::SetWindowPlacement ( ::GetDlgItem(m_hWnd,IDC_STATIC1) , &wPlacement ) ;

		break ;

	case M4_CS_EVENT_ADMIN_MESSAGE :

		//-------------------------------
		//Escribir el título de la ventana
		_loadString( IDC_CS_TEXT4, Buffer ) ;
		SetWindowText(Buffer);
		//-------------------------------

		_loadString( IDC_CS_STATIC4, Buffer ) ;
		if ( MAX_PARAM_LEN <= (strlen (Buffer) + strlen ((char*)m_pParam)) )
		{
			((char*)m_pParam )[ MAX_PARAM_LEN - strlen ( Buffer ) -1] = 0 ;
		}
		sprintf(pcBuffer,Buffer,(char*)m_pParam );
		::SetWindowText(::GetDlgItem(m_hWnd,IDC_CS_STATIC1),pcBuffer);

		::GetWindowPlacement ( ::GetDlgItem(m_hWnd,IDC_STATIC2) , &wPlacement ) ;
		wPlacement.showCmd = SW_SHOW;
		::SetWindowPlacement ( ::GetDlgItem(m_hWnd,IDC_STATIC2) , &wPlacement ) ;

		break ;
	}

	_loadString( IDOK, Buffer ) ;
	::SetWindowText(::GetDlgItem(m_hWnd,IDOK),Buffer);
//oEventDial.m_hWnd , 
	if ( ! ::SetWindowPos ( m_hWnd , HWND_TOPMOST , 300, 200, 241, 91 , SWP_SHOWWINDOW | SWP_NOREPOSITION | SWP_NOSIZE  ) )
	{
		DWORD error = GetLastError() ;
	}
	

	return bResult;
}

	void ClEventDial::SetParam ( const void * ai_pParam) 
	{
		m_pParam= (void*)ai_pParam;
	}

	void ClEventDial::SetEvent ( const int ai_iEvent ) 
	{
		m_iEvent = ai_iEvent ;
	}
