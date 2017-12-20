//==============================================================================
//
// (c) Copyright  1991-2009 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4ctlhelper
// File:             m4hook.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:             18/11/2009
// Language:         C++
// Operating System: Windows
// Design Document:
//
//	Owner:			Antonio Dur�n
//	Modifications:
//
// Definition:
//
//    This module manages the system messages hooking
//
//
//==============================================================================

#ifndef _M4HOOK_H_
#define _M4HOOK_H_

#include "m4ctlhelper_dll.hpp"
#include "cldates.hpp"

/// Prototipo de funci�n que recibe las notificaciones en la subclasificaci�n de un control
typedef LRESULT (CALLBACK * TSubClassProc) (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/// Iniciar el Hooking para el thread indicado por dwThreadId
void HookMsg(DWORD dwThreadId);

/// Finalizar el Hooking
void UnHookMsg();

/// Establece si se deben sobreescribir los bordes de controles de VB
void set_OverrideBordersVB(bool bValue);

/// Establece si se deben sobreescribir los men�s del ChartFX
void set_OverrideChartFXMenu(bool bValue);

/// Subclasifica la ventana o el control indentificado por hWnd en la funci�n definida por pFunWndProc
void SubClass(HWND hWnd, TSubClassProc pFunWndProc);

/// Elimina la subclasificaci�n de la ventana o el control identificado por hWnd
void DeSubClass(HWND hWnd);

/// Funci�n de Hooking WH_CBT
LRESULT CALLBACK CallHookCBT(int nCode, WPARAM wParam, LPARAM lParam);

/// Funci�n de Hooking WH_MOUSE
LRESULT CALLBACK CallHookMouse(int nCode, WPARAM wParam, LPARAM lParam);

/// Funci�n de Hooking WH_KEYBOARD
LRESULT CALLBACK CallHookKeyboard(int nCode, WPARAM wParam, LPARAM lParam);

/// Funci�n de sublasificaci�n para bordes de controles
LRESULT CALLBACK ControlBorderWndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/// Funci�n de sublasificaci�n para men�s del ChartFX
LRESULT CALLBACK ChartFXMenuWndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/// Exporta el n�mero de eventos de usuario
M4_DECL_M4CTLHELPER DWORD GetUserEventsCounter();

/// Fuerza notificaci�n de evento de usuario
void NotifyUserEvent();

/// Exporta el timestamp del �ltimo evento de usuario
M4_DECL_M4CTLHELPER m4date_t GetUserEventsTimeStamp();

#endif