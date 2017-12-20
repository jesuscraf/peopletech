//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             pageprn.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:24/09/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================
#ifndef __PAGEPRN__H__
#define __PAGEPRN__H__

#include "clmetric.h"

//==============================================================================ClENG_PagePrinter

class ClENG_PagePrinter {

MIT_PUBLIC:

	enum ORIENTATION { PORTRAIT, LANDSCAPE };
	enum PAPER_SIZE {A4, A3, LEGAL, LETTER, EXECUTIVE, PERSONAL };
	enum PAPER_SOURCE { MANUAL, UPPER_BIN, LOWER_BIN , USER_DEFINED , PRINTER_DEFAULT_BIN};

MIT_PRIVATE:
	ORIENTATION   m_Orientation ;
	PAPER_SIZE     m_PageSize ;
	PAPER_SOURCE  m_PaperSource ;
	DvcUnit_t m_SizeX, m_SizeY ;
	m4bool_t   m_LastFontUnderline ; //Dice si la ultima fuente usada era Underline

MIT_PUBLIC:

	// SizeX y SizeY solo son utiles si PageSize es personal. Si la orientacion es LANDSCAPE,
	//	los tamaños se invertiran.

	void Init( ORIENTATION ai_Orientation, PAPER_SIZE  ai_PageSize, 
			PAPER_SOURCE  ai_PaperSource,
			DvcUnit_t ai_SizeX,   DvcUnit_t ai_SizeY ) ;

	virtual size_t GetSizeof() { return sizeof(ClENG_PagePrinter) ; }

	ORIENTATION    GetOrientation() { return m_Orientation ; } 
	PAPER_SIZE     GetPageSize() { return m_PageSize ; } 
	PAPER_SOURCE   GetPaperSource() { return m_PaperSource ; } 
	DvcUnit_t GetSizeX() { return m_SizeX; }
	DvcUnit_t GetSizeY() { return m_SizeY;}

	//  Los Dvcs que hagan uso de esta capacidad, deben de actualizar el valor tambien. El mejor
	//sitio es en el SelectFontDevice, pues cualquier cambio de Font pasa por ahí. Con hacer
	//en este punto un SetUnderline conforme a la Font pasada, ya esta.

	m4bool_t  IsUnderline() { return m_LastFontUnderline ;} 
	void  SetUnderline( m4bool_t ai_bSw ) { m_LastFontUnderline=ai_bSw ;} 
	
} ;



#endif

