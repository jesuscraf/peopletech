// hodll.cpp : Defines the initialization routines for the DLL.
//
//
//Código implementado por Eduardo Fernandes.
//


#include "stdafx.h"
#include "hodll.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#pragma data_seg(".SHARDAT")
static HHOOK hkb=NULL;
FILE *f1;
#pragma data_seg()

HINSTANCE hins;
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CHodllApp

BEGIN_MESSAGE_MAP(CHodllApp, CWinApp)
//{{AFX_MSG_MAP(CHodllApp)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHodllApp construction


LRESULT KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	
	int iChangeDecimalPoint = 0;
	int iIgnore = 0;
	char separador[4];
	
	GetLocaleInfo (LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, separador, 1);
	
	if ((HC_ACTION==nCode) && (wParam == VK_DECIMAL) && (separador[0] == ','))
	{
		iIgnore = 1;
		if ((DWORD)lParam & 0x40000000)
		{
			iChangeDecimalPoint = 1;
		}
	}
	
	LRESULT RetVal = 1;
	
	if (!iIgnore) 
	{
		RetVal = CallNextHookEx( hkb, nCode, wParam, lParam );
	}
	else
	{
		if (iChangeDecimalPoint)
		{
			// Bug 0207018
			if (GetKeyState(VK_CAPITAL) & 1)
			{
				// If caps lock is on,    Simulate a key release
				keybd_event(VK_CAPITAL, 0, KEYEVENTF_EXTENDEDKEY |0, 0);
				keybd_event(VK_CAPITAL, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
				
				//Send comma key
				keybd_event(0xbc,0x33,0 | 0,0);
				
				// Simulate a key press
				keybd_event(VK_CAPITAL, 0, KEYEVENTF_EXTENDEDKEY |0, 0);
				keybd_event(VK_CAPITAL, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
			}
			else
			{
				keybd_event(0xbc,0x33,0 | 0,0);
			}

			return 1;
		}
	}
	
	return  RetVal;	
}

BOOL InstallHook()
{
	DWORD t_id;
	t_id = GetCurrentThreadId ();
	hkb=SetWindowsHookEx(WH_KEYBOARD,(HOOKPROC)KeyboardProc,hins,t_id);
	
	return TRUE;
}

BOOL UnHook()
{
	
	BOOL unhooked = UnhookWindowsHookEx(hkb);
	// MessageBox(0,"exit","sasa",MB_OK);
	return unhooked;
} 


BOOL CHodllApp::InitInstance ()
{
	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	hins=AfxGetInstanceHandle();
	return TRUE;
	
}
BOOL CHodllApp::ExitInstance ()
{
	return TRUE;
}

CHodllApp::CHodllApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CHodllApp object

CHodllApp theApp;
