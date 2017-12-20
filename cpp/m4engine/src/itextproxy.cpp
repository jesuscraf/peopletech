//==============================================================================
//
// (c) Copyright  1991-2008 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             iTextProxy.cpp
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


#include "itextproxy.h"
#include "itextcontext.h"
#include "engdf.inl"
#include "cldvcpdf.hmg"



//=================================================================================
//
// CliTextProxy
//
// Clase que implementa el objeto de API de acceso a iText
//
//=================================================================================

// Constantes =================================================================

const int CliTextProxy::DEBUG_NONE = 0 ;
const int CliTextProxy::DEBUG_INFO = 1 ;
const int CliTextProxy::DEBUG_TRACE = 2 ;
		
const int CliTextProxy::STYLE_FIRST = 0 ;
const int CliTextProxy::STYLE_SOLID = 0 ;
const int CliTextProxy::STYLE_DASH = 1 ;
const int CliTextProxy::STYLE_DOT = 2 ;
const int CliTextProxy::STYLE_DASH_DOT = 3 ;
const int CliTextProxy::STYLE_DASH_DOT_DOT = 4 ;
const int CliTextProxy::STYLE_LAST = 4 ;

const int CliTextProxy::FONT_NORMAL = 0 ;
const int CliTextProxy::FONT_BOLD = 1 ;
const int CliTextProxy::FONT_ITALIC = 2 ;
const int CliTextProxy::FONT_UNDERLINE = 4 ;
const int CliTextProxy::FONT_STRIKETHRU = 8 ;

const int CliTextProxy::TEXT_LTR = 0 ;
const int CliTextProxy::TEXT_RTL = 1 ;


// Funciones de inicialización ================================================

	CliTextProxy::CliTextProxy( CliTextContext& ai_roiTextContext, m4pcchar_t ai_pccFileName, m4bool_t ai_bProtect, m4int32_t ai_iDebugLevel, ClMIT_Trace *ai_poTrace )
	{

		m4return_t	iResult = M4_ERROR ;


		m_poiTextContext = &ai_roiTextContext ;

		iResult = m_poiTextContext->NewiTextStub( ai_pccFileName, ai_bProtect, ai_iDebugLevel, m_diTextObject ) ;

		if( iResult != M4_SUCCESS || m_diTextObject == 0 )
		{
			*ai_poTrace << ClMIT_Msg( &TRACE_ARRAY_UNICODE[ CLDVCPDF_UNICODE_MSG_4 ] ) << SEND_MSG ;
		}
	}


// Funciones del API ==========================================================

void	CliTextProxy::SetTextDirection( m4int32_t ai_iTextDirection )
{
	m_poiTextContext->SetTextDirection( m_diTextObject, ai_iTextDirection ) ;
}

void	CliTextProxy::SetCheckLimits( m4bool_t ai_bCheckLimits )
{
	m_poiTextContext->SetCheckLimits( m_diTextObject, ai_bCheckLimits ) ;
}

void	CliTextProxy::AddText( m4double_t ai_dPosX, m4double_t ai_dPosY, m4pcchar_t ai_pccText, m4int32_t ai_iLength )
{
	m_poiTextContext->AddText( m_diTextObject, ai_dPosX, ai_dPosY, ai_pccText, ai_iLength ) ;
}


m4int32_t	CliTextProxy::AddNewPage( m4double_t ai_dWidth, m4double_t ai_dHeight )
{
	return( m_poiTextContext->AddNewPage( m_diTextObject, ai_dWidth, ai_dHeight ) ) ;
}


m4int32_t	CliTextProxy::GetActualPage( void )
{
	return( m_poiTextContext->GetActualPage( m_diTextObject ) ) ;
}


m4int32_t	CliTextProxy::GetNumPages( void )
{
	return( m_poiTextContext->GetNumPages( m_diTextObject ) ) ;
}


void	CliTextProxy::SetActualPage( m4int32_t ai_iNumPage )
{
	m_poiTextContext->SetActualPage( m_diTextObject, ai_iNumPage ) ;
}


m4bool_t	CliTextProxy::SetFont( m4pcchar_t ai_pccFontName, m4double_t ai_dFontSize, m4int32_t ai_iFontStyle )
{
	return( m_poiTextContext->SetFont( m_diTextObject, ai_pccFontName, ai_dFontSize, ai_iFontStyle ) ) ;
}


void	CliTextProxy::SetColor( m4int32_t ai_iRed, m4int32_t ai_iGreen, m4int32_t ai_iBlue )
{
	m_poiTextContext->SetColor( m_diTextObject, ai_iRed, ai_iGreen, ai_iBlue ) ;
}


void	CliTextProxy::AddLine( m4double_t ai_dPosX1, m4double_t ai_dPosY1, m4double_t ai_dPosX2, m4double_t ai_dPosY2, m4double_t ai_dWidth, m4int32_t ai_iStyle )
{
	m_poiTextContext->AddLine( m_diTextObject, ai_dPosX1, ai_dPosY1, ai_dPosX2, ai_dPosY2, ai_dWidth, ai_iStyle ) ;
}


void	CliTextProxy::AddRectangle( m4double_t ai_dPosX, m4double_t ai_dPosY, m4double_t ai_dWidth, m4double_t ai_dHeight, m4double_t ai_dLineWidth, m4int32_t ai_iStyle )
{
	m_poiTextContext->AddRectangle( m_diTextObject, ai_dPosX, ai_dPosY, ai_dWidth, ai_dHeight, ai_dLineWidth, ai_iStyle ) ;
}


void	CliTextProxy::FillZone( m4double_t ai_dPosX, m4double_t ai_dPosY, m4double_t ai_dWidth, m4double_t ai_dHeight )
{
	m_poiTextContext->FillZone( m_diTextObject, ai_dPosX, ai_dPosY, ai_dWidth, ai_dHeight ) ;
}


void	CliTextProxy::AddImage( m4pcchar_t ai_pccFileName, m4double_t ai_dPosX, m4double_t ai_dPosY, m4double_t ai_dWidth, m4double_t ai_dHeight, m4int32_t ai_iLength )
{
	m_poiTextContext->AddImage( m_diTextObject, ai_pccFileName, ai_dPosX, ai_dPosY, ai_dWidth, ai_dHeight, ai_iLength ) ;
}


void	CliTextProxy::Close( void )
{
	 m_poiTextContext->Close( m_diTextObject ) ;
	 m_diTextObject = 0 ;
}

