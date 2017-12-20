//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             pageprn.cpp
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

#include "eng.h"
#include "engdf.inl"
#include "pageprn.h"

//========================================================================ClENG_PagePrinter
void ClENG_PagePrinter::Init( ORIENTATION ai_Orientation, PAPER_SIZE ai_PageSize, 
	PAPER_SOURCE  ai_PaperSource,
	DvcUnit_t ai_SizeX,   DvcUnit_t ai_SizeY ) {

	m_Orientation = ai_Orientation;
	m_PageSize = ai_PageSize;
	m_PaperSource = ai_PaperSource;
	if ( ai_PageSize == PERSONAL ) {
		m_SizeX=ai_SizeX; 
		m_SizeY=ai_SizeY;
	} else if ( ai_PageSize == A4 ) {
		m_SizeX=ClMetric::MilimToDvc(210) ;
		m_SizeY=ClMetric::MilimToDvc(297) ;
	} else if ( ai_PageSize == A3 ) {
		m_SizeX=ClMetric::MilimToDvc(297) ;
		m_SizeY=ClMetric::MilimToDvc(420) ;
	} else if ( ai_PageSize == LEGAL ) {
		m_SizeX=ClMetric::InchToDvc(8.5) ;
		m_SizeY=ClMetric::InchToDvc(14) ;
	} else if ( ai_PageSize == LETTER ) {
		m_SizeX=ClMetric::InchToDvc(8.5) ;
		m_SizeY=ClMetric::InchToDvc(11) ;
	} else if ( ai_PageSize == EXECUTIVE ) {
		m_SizeX=ClMetric::InchToDvc(7.25) ;
		m_SizeY=ClMetric::InchToDvc(10.5) ;
	};
	SetUnderline(M4_FALSE);
	if (ai_Orientation==LANDSCAPE) {
		ai_SizeY=m_SizeY ;
		m_SizeY=m_SizeX ;
		m_SizeX=ai_SizeY ;
	} ;
}
