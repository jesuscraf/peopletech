//==============================================================================
//
// (c) Copyright  1991-2009 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4ctlhelper
// File:             m4ctlhelper.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:             18/11/2009
// Language:         C++
// Operating System: Windows
// Design Document:
//
//	Owner:			Antonio Durán
//	Modifications:
//
// Definition:
//
//    Defines the initialization routines and API for the DLL.
//
//
//==============================================================================


#include "stdafx.h"
#include "m4ctlhelper.h"
#include "m4hook.h"
#include "m4colors.h"

#define M4OVERRIDEVBBORDER_INDEX	0
#define M4OVERRIDECHARTFXMENU_INDEX	1

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
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
// CM4ctlhelperApp

BEGIN_MESSAGE_MAP(CM4ctlhelperApp, CWinApp)
	//{{AFX_MSG_MAP(CM4ctlhelperApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CM4ctlhelperApp construction

CM4ctlhelperApp::CM4ctlhelperApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	ResetColors();
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CM4ctlhelperApp object

CM4ctlhelperApp theApp;

/// Iniciar el Hooking para el proceso actual
extern "C" M4_DECL_M4CTLHELPER void WINAPI M4HookMsg()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HookMsg(theApp.m_nThreadID);
}

/// Finalizar el Hooking
extern "C" M4_DECL_M4CTLHELPER void WINAPI M4UnHookMsg()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	UnHookMsg();
}

/// Ajusta los colores definidos por Meta4 a sus valores por defecto
extern "C" M4_DECL_M4CTLHELPER void WINAPI M4ResetColors()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	ResetColors();
}

/// Actualiza el color con índice iIndex al color definido por iColors
extern "C" M4_DECL_M4CTLHELPER void WINAPI M4UpdateColor(int iIndex, COLORREF iColor)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	UpdateColor(iIndex, iColor);
}

/// Realiza la subclasificación para la sobreescritura de bordes de controles de VB
extern "C" M4_DECL_M4CTLHELPER void WINAPI M4SubClassControlBorder(HWND hWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	SubClass (hWnd, ControlBorderWndProc);
}

/// Establece el valor iValue al Flag de comportamiento definido por iIndex
extern "C" M4_DECL_M4CTLHELPER void WINAPI M4SetFlag(int iIndex, int iValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	switch (iIndex)
	{
	case M4OVERRIDEVBBORDER_INDEX:
		// Determina si debe sobreescribir los bordes de los controles de VB
		set_OverrideBordersVB(iValue != 0);
		break;
	case M4OVERRIDECHARTFXMENU_INDEX:
		// Determina si debe intervenir en el dibujado de los menús del ChartFX
		set_OverrideChartFXMenu(iValue != 0);
		break;
	}
}

/// Exporta el número de eventos de usuario
extern "C" M4_DECL_M4CTLHELPER DWORD WINAPI M4GetUserEventsCounter()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return GetUserEventsCounter();
}

/// Fuerza notificación de evento de usuario
extern "C" M4_DECL_M4CTLHELPER void WINAPI M4NotifyUserEvent()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	NotifyUserEvent();
}

