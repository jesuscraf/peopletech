//==============================================================================
//
// (c) Copyright  1991-2009 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4ctlhelper
// File:             m4hook.cpp
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
//    This module manages the system messages hooking
//
//
//==============================================================================

#include <stdafx.h>
#include "m4hook.h"
#include "m4colors.h"


// set to 1 to generate traces in the temp directory
#ifndef TRACE_EVENTS
#define TRACE_EVENTS 0
#endif
#define TRACE_FILE "c:/temp/clicks.log"

// to support old compilation environments
#ifndef WM_XBUTTONDOWN
#define WM_XBUTTONDOWN 0x020B
#endif
#ifndef WM_MOUSEHWHEEL
#define WM_MOUSEHWHEEL 0x020E
#endif
#ifndef WM_NCXBUTTONDOWN
#define WM_NCXBUTTONDOWN 0x00AB
#endif


static HHOOK g_CBTHookID = NULL;
static HHOOK g_MouseHookID = NULL;
static HHOOK g_KeyboardHookID = NULL;
static DWORD g_nUserEventsCounter = 0;
static m4date_t g_nUserEventsTimeStamp = 0;

static WORD g_vKeyUserEvents[] = {WM_LBUTTONDOWN,WM_RBUTTONDOWN,WM_MBUTTONDOWN,WM_XBUTTONDOWN,
						WM_MOUSEWHEEL, WM_MOUSEHWHEEL,
						WM_NCLBUTTONDOWN,WM_NCRBUTTONDOWN,WM_NCMBUTTONDOWN,WM_NCXBUTTONDOWN, 0};

static char M4_OLDWNDPROC[] = "M4C_OLDWNDPROC";

bool _bOverrideBordersVB = false;
bool _bOverrideChartFXMenu = false;

/// Registar acción de usuario
inline void RegisterUserEvent()
{
	// Se actualizar estado tras un nuevo evento de usuario
	g_nUserEventsCounter++;
	g_nUserEventsTimeStamp = ClActualDate();
}

/// Iniciar Hooking's para el thread indicado por dwThreadId
void HookMsg(DWORD dwThreadId)
{
	if (g_CBTHookID == NULL)
	{
		//g_CBTHookID = SetWindowsHookEx(WH_CALLWNDPROC, CallHookWndProc, NULL, dwThreadId);
		g_CBTHookID		 = SetWindowsHookEx(WH_CBT, CallHookCBT, NULL, dwThreadId);
		g_MouseHookID	 = SetWindowsHookEx(WH_MOUSE, CallHookMouse, NULL, dwThreadId);
		g_KeyboardHookID = SetWindowsHookEx(WH_KEYBOARD, CallHookKeyboard, NULL, dwThreadId);
	}
}

// finaliza hook
void DisposeHook(HHOOK &hHook)
{
	if (hHook != NULL)
	{
		UnhookWindowsHookEx(hHook);
		hHook = NULL;
	}
}

#if (TRACE_EVENTS)

#define TRACE_TO_FILE(a,b,c,d) clickTracer(a,b,c,d)
static FILE *tracer = NULL;

void clickTracer(const char *info, int nCode, WPARAM wParam, LPARAM lParam)
{
	if (tracer == NULL)
	{
		tracer = fopen(TRACE_FILE,"wt");
	}

	fprintf(tracer,"%ld\t%s\t%ld\t%d\t%u\t%ld\n",clock(),info,g_nUserEventsCounter, nCode, wParam, lParam);
	fflush(tracer);
}
#else

#define TRACE_TO_FILE(a,b,c,d) {}

#endif

/// Finalizar el Hooking
void UnHookMsg()
{
	DisposeHook(g_KeyboardHookID);
	DisposeHook(g_MouseHookID);
	DisposeHook(g_CBTHookID);
}

/// Establece si se deben sobreescribir los bordes de controles de VB
void set_OverrideBordersVB(bool bValue)
{
	_bOverrideBordersVB = bValue;
}

/// Establece si se deben sobreescribir los menús del ChartFX
void set_OverrideChartFXMenu(bool bValue)
{
	_bOverrideChartFXMenu = bValue;
}

/// Subclasifica la ventana o el control indentificado por hWnd en la función definida por pFunWndProc
void SubClass(HWND hWnd, TSubClassProc pFunWndProc)
{
	WNDPROC lhOldWndProc;

	// Se utiliza M4_OLDWNDPROC para almacenar el antiguo puntero de función
	// y así poder realizar la insercíón en la cadenad de sublasificación
	lhOldWndProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)pFunWndProc);
	SetProp (hWnd, M4_OLDWNDPROC, lhOldWndProc);
}

/// Elimina la subclasificación de la ventana o el control identificado por hWnd
void DeSubClass(HWND hWnd)
{
	WNDPROC lhOldWndProc;

	lhOldWndProc = (WNDPROC) GetProp(hWnd, M4_OLDWNDPROC);
	
	RemoveProp (hWnd, M4_OLDWNDPROC);

	if (lhOldWndProc != NULL)
	{
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)lhOldWndProc);
	}
}

/// Función de Hooking WH_CBT
LRESULT CALLBACK CallHookCBT(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (g_CBTHookID != NULL)
	{
		if (nCode == HCBT_CREATEWND)
		{
			CString sClass;
			int iRet;

			iRet = GetClassName((HWND)wParam, sClass.GetBuffer(256), 255);
			sClass.ReleaseBuffer(iRet);

			if ((sClass.Compare("ThunderPictureBox") == 0) ||
				(sClass.Compare("ThunderPictureBoxDC") == 0) ||
				(sClass.Compare("ThunderRT6PictureBox") == 0) ||
				(sClass.Compare("ThunderRT6PictureBoxDC") == 0))
			{
				// PictureBox VB
				SubClass ((HWND)wParam, ControlBorderWndProc);
			}
			else if (sClass.Compare("SfxBar_CmdBarMenu") == 0)
			{
				// Menus ChartFX de SoftwareFX
				SubClass ((HWND)wParam, ChartFXMenuWndProc);
			}
		}

		return CallNextHookEx(g_CBTHookID, nCode, wParam, lParam);
	}
	else
	{
		return 0;
	}
}

/// Función de Hooking WH_MOUSE
LRESULT CALLBACK CallHookMouse(int nCode, WPARAM wParam, LPARAM lParam)
{
	// checks for valid mouse events
	for (int i=0; g_vKeyUserEvents[i] != 0; ++i)
	{
		if (g_vKeyUserEvents[i] == wParam)
		{
			// increments user event counter
			RegisterUserEvent();
			TRACE_TO_FILE("CallHookMouse", nCode, wParam, lParam);
			break;
		}
	}

	if (g_MouseHookID != NULL)
	{
		return CallNextHookEx(g_MouseHookID, nCode, wParam, lParam);
	}
	else
	{
		return 0;
	}
}

/// Función de Hooking WH_KEYBOARD
LRESULT CALLBACK CallHookKeyboard(int nCode, WPARAM wParam, LPARAM lParam)
{
	// bit 31th indicates a key pressed event: 0 -> key pressed
	if ((lParam & 0x80000000) == 0)
	{
		// increments user event counter
		RegisterUserEvent();
		TRACE_TO_FILE("CallHookKeyboard", nCode, wParam, lParam);
	}

	if (g_KeyboardHookID != NULL)
	{
		return CallNextHookEx(g_KeyboardHookID, nCode, wParam, lParam);
	}
	else
	{
		return 0;
	}
}


/// Función de sublasificación para bordes de controles
LRESULT CALLBACK ControlBorderWndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WNDPROC lhOldWndProc;
	bool bFixedSingle, b3DAppearance;
	HDC lhDC;
	HPEN lhPen, lhOldPen;
	RECT rRect;
	POINT rPoint;

	lhOldWndProc = (WNDPROC)GetProp(hWnd, M4_OLDWNDPROC);

	switch (uMsg)
	{
	case WM_NCPAINT:
		// Determina si se debe reemplazar el dibujado de los bordes
		if (_bOverrideBordersVB)
		{
			// Se obtienen, a partir de lo estilos, las propiedades equivalentes de bordes en VB
			if ((GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_CLIENTEDGE) == WS_EX_CLIENTEDGE)
			{
			  bFixedSingle = true;
			  b3DAppearance = true;
			}
			else if ((GetWindowLong(hWnd, GWL_STYLE) & WS_BORDER) == WS_BORDER)
			{
			  bFixedSingle = true;
			  b3DAppearance = false;
			}
			else
			{
			  bFixedSingle = false;
			  b3DAppearance = false;
			}

			// Se simula el comportamiento de VB siguiendo las propiedades del control
			if (b3DAppearance || bFixedSingle)
			{
			  // FUNCIONAN LOS DOS. El segundo es el ejemplo del MSDN
			  // pero nos quedamos con el método que funcionaba correctamente con ListView / TreeView
			  lhDC = GetDCEx(hWnd, 0, DCX_WINDOW | DCX_CACHE | DCX_CLIPSIBLINGS | DCX_LOCKWINDOWUPDATE);
			  // lhDC = GetDCEx(hWnd, wParam, DCX_WINDOW | DCX_INTERSECTRGN);
			  if (b3DAppearance)
			  {
				  lhPen = CreatePen(PS_SOLID, 1, TranslateColor(BASE_SYSCOLOR + COLOR_3DHILIGHT));
			  }
			  else
			  {
				  lhPen = CreatePen(PS_SOLID, 1, TranslateColor(BASE_SYSCOLOR + COLOR_3DSHADOW));
			  }
			  lhOldPen = (HPEN) SelectObject(lhDC, lhPen);
			  GetWindowRect(hWnd, &rRect);
			  OffsetRect (&rRect, -rRect.left - 1, -rRect.top - 1);
  
			  MoveToEx (lhDC, 0, 0, &rPoint);
			  LineTo (lhDC, rRect.right, 0);
			  LineTo (lhDC, rRect.right, rRect.bottom);
			  LineTo (lhDC, 0, rRect.bottom);
			  LineTo (lhDC, 0, 0);
  
			  if (b3DAppearance)
			  {
				lhPen = CreatePen(PS_SOLID, 1, TranslateColor(BASE_SYSCOLOR + COLOR_3DSHADOW));
				DeleteObject (SelectObject(lhDC, lhPen));
    
				MoveToEx (lhDC, 1, 1, &rPoint);
				LineTo (lhDC, rRect.right - 1, 1);
				LineTo (lhDC, rRect.right - 1, rRect.bottom - 1);
				LineTo (lhDC, 1, rRect.bottom - 1);
				LineTo (lhDC, 1, 1);
			  }
  
			  SelectObject (lhDC, lhOldPen);
			  DeleteObject (lhPen);
			  ReleaseDC (hWnd, lhDC);
			  return 0;
			}
		}
		break;
	case WM_DESTROY:
		// Elimina la subclasificación con el mensaje de destrucción de ventana o control
		DeSubClass (hWnd);
		break;
	}

	// Continua en la siguiente función de la cadena de subclasificación
	if (lhOldWndProc != NULL)
	{
		return CallWindowProc(lhOldWndProc, hWnd, uMsg, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

/// Función de sublasificación para menús del ChartFX
LRESULT CALLBACK ChartFXMenuWndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WNDPROC lhOldWndProc;
	HDC lhDC;
	BOOL lOldValue;
	LRESULT lRetValue;
	COLORREF lColor;

	lhOldWndProc = (WNDPROC)GetProp(hWnd, M4_OLDWNDPROC);

	switch (uMsg)
	{
	case WM_DRAWITEM:
		// Determina si se debe modificar el dibujado de los menus del ChartFX
		if (_bOverrideChartFXMenu)
		{
			// Se deshabilita temporalmente el ClearType (si procede) reseteando
			// el Font Smoothing
			SystemParametersInfo(SPI_GETFONTSMOOTHING, 0, &lOldValue, 0);
			SystemParametersInfo(SPI_SETFONTSMOOTHING, FALSE, 0, 0);
			//SystemParametersInfo(SPI_SETFONTSMOOTHINGTYPE, 0, (void*)FE_FONTSMOOTHINGSTANDARD, 0);

			if (lhOldWndProc != NULL)
			{
				lRetValue = CallWindowProc(lhOldWndProc, hWnd, uMsg, wParam, lParam);
			}
			else
			{
				lRetValue = DefWindowProc(hWnd, uMsg, wParam, lParam);
			}
			// Se restaura el antiguo valor de Font Smoothing
			SystemParametersInfo(SPI_SETFONTSMOOTHING, lOldValue, 0, 0);
			//SystemParametersInfo(SPI_SETFONTSMOOTHINGTYPE, 0, (void*)FE_FONTSMOOTHINGCLEARTYPE, 0);

			// Dejamos al ChartFX dibujar totalmente el Item de menu y si no se trata de un separador
			// realizamos un cambiazo de color para ajustar el texto
			if (((LPDRAWITEMSTRUCT)lParam)->itemID != 0)
			{
				// De momento se hace con GetPixel / SetPixel puesto que suelen ser
				// áras pequeñas. Sería más óptimo utilizar funciones BitBlt
				lColor = TranslateColor(BASE_M4COLOR + M4POPUPMENUCOLOR);
				lhDC = ((LPDRAWITEMSTRUCT)lParam)->hDC;

				for (long PosY = ((LPDRAWITEMSTRUCT)lParam)->rcItem.top;
						  PosY <= ((LPDRAWITEMSTRUCT)lParam)->rcItem.bottom; PosY++)
				{
					for (long PosX = ((LPDRAWITEMSTRUCT)lParam)->rcItem.left + 20;
							  PosX <= ((LPDRAWITEMSTRUCT)lParam)->rcItem.right; PosX++)
					{
						if (GetPixel(lhDC, PosX, PosY) == WHITE)
						{
							SetPixel(lhDC, PosX, PosY, lColor);
						}
					}
				}
			}
			return lRetValue;
		}
		break;

	case WM_DESTROY:
		// Elimina la subclasificación con el mensaje de destrucción de ventana o control
		DeSubClass (hWnd);
		break;
	}

	// Continua en la siguiente función de la cadena de subclasificación
	if (lhOldWndProc != NULL)
	{
		return CallWindowProc(lhOldWndProc, hWnd, uMsg, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

/// Exporta el número de eventos de usuario
DWORD GetUserEventsCounter()
{
	TRACE_TO_FILE("GetUserEventsCounter", 0, 0, 0);
	return g_nUserEventsCounter;
}

/// Fuerza notificación de evento de usuario
void NotifyUserEvent()
{
	RegisterUserEvent();
	TRACE_TO_FILE("NotifyUserEvent", 0, 0, 0);
}

/// Exporta el timestamp del último evento de usuario
m4date_t GetUserEventsTimeStamp()
{
	TRACE_TO_FILE("GetUserEventsTimeStamp", 0, 0, 0);
	return g_nUserEventsTimeStamp;
}


