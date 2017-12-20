//==============================================================================
//
// (c) Copyright  1991-2009 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4ctlhelper
// File:             m4colors.h
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

#ifndef _M4COLORS_H_
#define _M4COLORS_H_

#define BASE_SYSCOLOR	0x80000000
#define BASE_M4COLOR	0x90000000

#define M4DISABLECOLOR		0
#define M4UNDERLINECOLOR	1
#define M4GRIDLINESCOLOR	2
#define M4POPUPMENUCOLOR	3

#define WHITE 0xFFFFFF

/// Ajusta los colores definidos por Meta4 a sus valores por defecto
void ResetColors();

/// Actualiza el color con índice iIndex al color definido por iColors
void UpdateColor(unsigned int iIndex, COLORREF iColor);

/// Realiza la traducción de colores de sistema y de los definidos por Meta4
COLORREF TranslateColor(COLORREF iColor);

#endif
