//==============================================================================
//
// (c) Copyright  1991-2009 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4ctlhelper
// File:             m4colors.cpp
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
//    This module manages the system and Meta4 defined colors for skinning
//
//
//==============================================================================

#include <stdafx.h>
#include "m4colors.h"

// Se utilizan los colores de sistema definidos en WinUser.h

#define SYSFIRSTCOLOR	COLOR_SCROLLBAR
#define SYSLASTCOLOR	COLOR_GRADIENTINACTIVECAPTION

#define M4FIRSTCOLOR	0
#define M4LASTCOLOR		3


static COLORREF _M4Colors[M4LASTCOLOR + 1];

/// Ajusta los colores definidos por Meta4 a sus valores por defecto
void ResetColors()
{
	_M4Colors[M4DISABLECOLOR] = COLOR_BTNFACE;
	_M4Colors[M4UNDERLINECOLOR] = WHITE;
	_M4Colors[M4GRIDLINESCOLOR] = COLOR_BTNFACE;
	_M4Colors[M4POPUPMENUCOLOR] = COLOR_MENUTEXT;
}

/// Actualiza el color con índice iIndex al color definido por iColors
void UpdateColor(unsigned int iIndex, COLORREF iColor)
{
	// Solo se actualizan los colores definidos por Meta4
	if ((iIndex >= BASE_M4COLOR + M4FIRSTCOLOR) && (iIndex <= BASE_M4COLOR + M4LASTCOLOR))
	{
		_M4Colors[iIndex - BASE_M4COLOR] = iColor;
	}
}

/// Realiza la traducción de colores de sistema y de los definidos por Meta4
COLORREF TranslateColor(COLORREF iColor)
{
	if ((iColor >= BASE_SYSCOLOR + SYSFIRSTCOLOR) && (iColor <= BASE_SYSCOLOR + SYSLASTCOLOR))
	{
		// Color de sistema
		return GetSysColor(iColor - BASE_SYSCOLOR);
	}
	else if ((iColor >= BASE_M4COLOR + M4FIRSTCOLOR) && (iColor <= BASE_M4COLOR + M4LASTCOLOR))
	{
		// Color definido por Meta4
		COLORREF retValue = _M4Colors[iColor - BASE_M4COLOR];
		if (retValue >= BASE_SYSCOLOR)
		{
			// El color definido se corresponde a un color de sistema => Llamada recursiva
			retValue = TranslateColor(retValue);
		}
		
		return retValue;
	}
	else
	{
		// Color normal
		return iColor;
	}
}
