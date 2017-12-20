//==============================================================================
//
// (c) Copyright  1991-2008 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             iTextProxy.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:             13/08/2008
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Owner:			Antonio Durán
//	Modifications:	
//
// Definition:
//
//    Proxy class for iTextStub Java class
//	This class and the Java class are the wrapper for Java iText module (PDF generation)
//
//
//==============================================================================

#ifndef __ITEXTPROXY_H__
#define __ITEXTPROXY_H__


#include "mit.h"

class CliTextContext;




//=================================================================================
//
// CliTextProxy
//
// Clase que implementa el objeto de API de acceso a iText
//
//=================================================================================

class	CliTextProxy
{

protected:

	m4double_t		m_diTextObject ;
	CliTextContext	*m_poiTextContext ;

public:

// Constantes =================================================================

	static const int DEBUG_NONE ;
	static const int DEBUG_INFO ;
	static const int DEBUG_TRACE ;

	static const int STYLE_FIRST ;
	static const int STYLE_SOLID ;
	static const int STYLE_DASH ;
	static const int STYLE_DOT ;
	static const int STYLE_DASH_DOT ;
	static const int STYLE_DASH_DOT_DOT ;
	static const int STYLE_LAST ;

	static const int FONT_NORMAL ;
	static const int FONT_BOLD ;
	static const int FONT_ITALIC ;
	static const int FONT_UNDERLINE ;
	static const int FONT_STRIKETHRU ;

	static const int TEXT_LTR ;
	static const int TEXT_RTL ;

// Funciones de inicialización ================================================

		CliTextProxy( CliTextContext& ai_roiTextContext, m4pcchar_t ai_pccFileName, m4bool_t ai_bProtect, m4int32_t ai_iDebugLevel, ClMIT_Trace *ai_poTrace ) ;
		~CliTextProxy( void ) {} ;

// Funciones de lectura =======================================================

	m4bool_t	IsReady( void )
	{
		return( m_diTextObject != 0 ) ;
	}

// Funciones del API ==========================================================

	void		SetTextDirection( m4int32_t ai_iTextDirection ) ;
	void		SetCheckLimits( m4bool_t ai_bCheckLimits ) ;
	void		AddText( m4double_t ai_dPosX, m4double_t ai_dPosY, m4pcchar_t ai_pccText, m4int32_t ai_iLength = -1 ) ;
	m4int32_t	AddNewPage( m4double_t ai_dWidth, m4double_t ai_dHeight )  ;
	m4int32_t	GetActualPage( void) ;
	m4int32_t	GetNumPages( void ) ;
	void		SetActualPage( m4int32_t ai_iNumPage ) ;
	m4bool_t	SetFont( m4pcchar_t ai_pccFontName, m4double_t ai_dFontSize, m4int32_t ai_iFontStyle ) ;
	void		SetColor( m4int32_t ai_iRed, m4int32_t ai_iGreen, m4int32_t ai_iBlue ) ;
	void		AddLine( m4double_t ai_dPosX1, m4double_t ai_dPosY1, m4double_t ai_dPosX2, m4double_t ai_dPosY2, m4double_t ai_dWidth, m4int32_t ai_iStyle ) ;
	void		AddRectangle( m4double_t ai_dPosX, m4double_t ai_dPosY, m4double_t ai_dWidth, m4double_t ai_dHeight, m4double_t ai_dLineWidth, m4int32_t ai_iStyle ) ;
	void		FillZone( m4double_t ai_dPosX, m4double_t ai_dPosY, m4double_t ai_dWidth, m4double_t ai_dHeight ) ;
	void		AddImage( m4pcchar_t ai_pccFileName, m4double_t ai_dPosX, m4double_t ai_dPosY, m4double_t ai_dWidth, m4double_t ai_dHeight, m4int32_t ai_iLength = -1 ) ;
	void		Close( void  ) ;
};


#endif
