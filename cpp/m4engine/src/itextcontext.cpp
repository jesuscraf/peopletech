
//==============================================================================
//
// (c) Copyright  1991-2015 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              enginedll
// File:                itextcontext.cpp   
// Project:             PeopleNet      
// Author:              Meta Software M.S. , S.A
// Date:                24-09-2015
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    En este módulo se implementa el objeto para manejar el conexto java de acceso
//    a iText
//
//==============================================================================


#include "itextcontext.h"
#include "m4jobject.hpp"
#include "m4jcontext.hpp"
#include "engdf.inl"
#include "cldvcpdf.hmg"



#define __S__(x)			#x
#define __L__(x)			__S__(x)
#define __SOURCE__			__FILE__ "(" __L__(__LINE__) ")"


#define	ITEXT_CLASSNAME		"com.meta4.reports.engine.iTextStub"	




//=================================================================================
//
// CliTextContext
//
// Clase que implementa el objeto para manejar el conexto java de acceso a iText
//
//=================================================================================

m4return_t	CliTextContext::_Init( ClVMBaseEnv *ai_poEnvironment, ClMIT_Trace *ai_poTrace )
{

	m4return_t	iResult = M4_ERROR ;


	iResult = m_poJavaContext->Open( ai_poEnvironment, "m4engine", NULL, NULL ) ;

	if( iResult != M4_SUCCESS )
	{
		// No se ha podido inicializar la máquina virtual de java
		*ai_poTrace << ClMIT_Msg( &TRACE_ARRAY_UNICODE[ CLDVCPDF_UNICODE_MSG_2 ] ) << SEND_MSG ;
		return( M4_ERROR ) ;
	}

	iResult = _InitMethods() ;

	if( iResult != M4_SUCCESS )
	{
		// Versión incorrecta de m4pdf_engine.jar
		*ai_poTrace << ClMIT_Msg( &TRACE_ARRAY_UNICODE[ CLDVCPDF_UNICODE_MSG_4 ] ) << SEND_MSG ;
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	CliTextContext::_InitMethods( void )
{

	m4return_t		iResult = NULL ;
	M4JavaObject	aoTypes[ 6 ] ;


	// SetTextDirection
	aoTypes[ 0 ].SetByte( M4JavaObject::Integer ) ;

	iResult = m_poJavaContext->NewMethod( ITEXT_CLASSNAME, "SetTextDirection", aoTypes, 1, m_iSetTextDirection, M4_FALSE, __SOURCE__ ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// SetCheckLimits
	aoTypes[ 0 ].SetByte( M4JavaObject::Boolean ) ;

	iResult = m_poJavaContext->NewMethod( ITEXT_CLASSNAME, "SetCheckLimits", aoTypes, 1, m_iSetCheckLimits, M4_FALSE, __SOURCE__ ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// AddText
	aoTypes[ 0 ].SetByte( M4JavaObject::String ) ;
	aoTypes[ 1 ].SetByte( M4JavaObject::Float ) ;
	aoTypes[ 2 ].SetByte( M4JavaObject::Float ) ;

	iResult = m_poJavaContext->NewMethod( ITEXT_CLASSNAME, "AddText", aoTypes, 3, m_iAddText, M4_FALSE, __SOURCE__ ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// AddNewPage
	aoTypes[ 0 ].SetByte( M4JavaObject::Float ) ;
	aoTypes[ 1 ].SetByte( M4JavaObject::Float ) ;

	iResult = m_poJavaContext->NewMethod( ITEXT_CLASSNAME, "AddNewPage", aoTypes, 2, m_iAddNewPage, M4_FALSE, __SOURCE__ ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// GetActualPage
	iResult = m_poJavaContext->NewMethod( ITEXT_CLASSNAME, "GetActualPage", NULL, 0, m_iGetActualPage, M4_FALSE, __SOURCE__ ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// GetNumPages
	iResult = m_poJavaContext->NewMethod( ITEXT_CLASSNAME, "GetNumPages", aoTypes, 0, m_iGetNumPages, M4_FALSE, __SOURCE__ ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// SetActualPage
	aoTypes[ 0 ].SetByte( M4JavaObject::Integer ) ;

	iResult = m_poJavaContext->NewMethod( ITEXT_CLASSNAME, "SetActualPage", aoTypes, 1, m_iSetActualPage, M4_FALSE, __SOURCE__ ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// SetFont
	aoTypes[ 0 ].SetByte( M4JavaObject::String ) ;
	aoTypes[ 1 ].SetByte( M4JavaObject::Float ) ;
	aoTypes[ 2 ].SetByte( M4JavaObject::Integer ) ;

	iResult = m_poJavaContext->NewMethod( ITEXT_CLASSNAME, "SetFont", aoTypes, 3, m_iSetFont, M4_FALSE, __SOURCE__ ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// SetColor
	aoTypes[ 0 ].SetByte( M4JavaObject::Integer ) ;
	aoTypes[ 1 ].SetByte( M4JavaObject::Integer ) ;
	aoTypes[ 2 ].SetByte( M4JavaObject::Integer ) ;

	iResult = m_poJavaContext->NewMethod( ITEXT_CLASSNAME, "SetColor", aoTypes, 3, m_iSetColor, M4_FALSE, __SOURCE__ ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// AddLine
	aoTypes[ 0 ].SetByte( M4JavaObject::Float ) ;
	aoTypes[ 1 ].SetByte( M4JavaObject::Float ) ;
	aoTypes[ 2 ].SetByte( M4JavaObject::Float ) ;
	aoTypes[ 3 ].SetByte( M4JavaObject::Float ) ;
	aoTypes[ 4 ].SetByte( M4JavaObject::Float ) ;
	aoTypes[ 5 ].SetByte( M4JavaObject::Integer ) ;

	iResult = m_poJavaContext->NewMethod( ITEXT_CLASSNAME, "AddLine", aoTypes, 6, m_iAddLine, M4_FALSE, __SOURCE__ ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// AddRectangle
	aoTypes[ 0 ].SetByte( M4JavaObject::Float ) ;
	aoTypes[ 1 ].SetByte( M4JavaObject::Float ) ;
	aoTypes[ 2 ].SetByte( M4JavaObject::Float ) ;
	aoTypes[ 3 ].SetByte( M4JavaObject::Float ) ;
	aoTypes[ 4 ].SetByte( M4JavaObject::Float ) ;
	aoTypes[ 5 ].SetByte( M4JavaObject::Integer ) ;

	iResult = m_poJavaContext->NewMethod( ITEXT_CLASSNAME, "AddRectangle", aoTypes, 6, m_iAddRectangle, M4_FALSE, __SOURCE__ ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// FillZone
	aoTypes[ 0 ].SetByte( M4JavaObject::Float ) ;
	aoTypes[ 1 ].SetByte( M4JavaObject::Float ) ;
	aoTypes[ 2 ].SetByte( M4JavaObject::Float ) ;
	aoTypes[ 3 ].SetByte( M4JavaObject::Float ) ;

	iResult = m_poJavaContext->NewMethod( ITEXT_CLASSNAME, "FillZone", aoTypes, 4, m_iFillZone, M4_FALSE, __SOURCE__ ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// AddImage
	aoTypes[ 0 ].SetByte( M4JavaObject::String ) ;
	aoTypes[ 1 ].SetByte( M4JavaObject::Float ) ;
	aoTypes[ 2 ].SetByte( M4JavaObject::Float ) ;
	aoTypes[ 3 ].SetByte( M4JavaObject::Float ) ;
	aoTypes[ 4 ].SetByte( M4JavaObject::Float ) ;

	iResult = m_poJavaContext->NewMethod( ITEXT_CLASSNAME, "AddImage", aoTypes, 5, m_iAddImage, M4_FALSE, __SOURCE__ ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// Close
	iResult = m_poJavaContext->NewMethod( ITEXT_CLASSNAME, "Close", aoTypes, 0, m_iClose, M4_FALSE, __SOURCE__ ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}



// Funciones de inicialización ================================================

CliTextContext::CliTextContext( void )
{
	m_iSetTextDirection = 0 ;
	m_iSetCheckLimits = 0 ;
	m_iAddText = 0 ;
	m_iAddNewPage = 0 ;
	m_iGetActualPage = 0 ;
	m_iGetNumPages = 0 ;
	m_iSetActualPage = 0 ;
	m_iSetFont = 0 ;
	m_iSetColor = 0 ;
	m_iAddLine = 0 ;
	m_iAddRectangle = 0 ;
	m_iFillZone = 0 ;
	m_iAddImage = 0 ;
	m_iClose = 0 ;

	m_poJavaContext = NULL ;
}


CliTextContext::~CliTextContext( void )
{
	if( m_poJavaContext != NULL )
	{
		delete( m_poJavaContext ) ;
		m_poJavaContext = NULL ;
	}
}


m4return_t	CliTextContext::Init( ClVMBaseEnv *ai_poEnvironment, IExecutor *ai_poExecutor, ClMIT_Trace *ai_poTrace )
{

	m4return_t	iResult = M4_ERROR ;


	if( m_poJavaContext != NULL )
	{
		return( M4_SUCCESS ) ;
	}

//	m_poJavaContext = new M4JavaContext( NULL, ai_poExecutor ) ;	De momento no se permite timeout en los reports
	m_poJavaContext = new M4JavaContext( NULL, NULL ) ;

	iResult = _Init( ai_poEnvironment, ai_poTrace ) ;

	if( iResult != M4_SUCCESS )
	{
		delete( m_poJavaContext ) ;
		m_poJavaContext = NULL ;
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t	CliTextContext::NewiTextStub( m4pcchar_t ai_pccFileName, m4bool_t ai_bProtect, m4int32_t ai_iDebugLevel, m4double_t& ao_rdHandle )
{

	M4JavaObject	avArguments[ 3 ] ;


	ao_rdHandle = 0 ;

	if( IsReady() == M4_FALSE )
	{
		return( M4_ERROR ) ;
	}

	avArguments[ 0 ].SetString( ai_pccFileName, M4_FALSE ) ;
	avArguments[ 1 ].SetBoolean( ai_bProtect ) ;
	avArguments[ 2 ].SetInteger( ai_iDebugLevel ) ;

	return( m_poJavaContext->CreateObject( ITEXT_CLASSNAME, avArguments, 3, ao_rdHandle, M4_FALSE, __SOURCE__ ) ) ;
}



// Funciones de lectura =======================================================

m4bool_t	CliTextContext::IsReady( void ) const
{
	return( m_poJavaContext != NULL && m_poJavaContext->IsReady() ) ;
}


m4bool_t	CliTextContext::IsInitialized( void ) const
{
	return ( m_poJavaContext != NULL ) ;
}



// Funciones del API ==========================================================

void	CliTextContext::SetTextDirection( m4double_t ai_dHandle, m4int32_t ai_iTextDirection )
{

	M4JavaObject	vResult ;
	M4JavaObject	avArguments[ 1 ] ;


	if( IsReady() == M4_TRUE && ai_dHandle != 0 )
	{
		avArguments[ 0 ].SetInteger( ai_iTextDirection ) ;

		m_poJavaContext->InvokeMethod( ai_dHandle, m_iSetTextDirection, avArguments, 1, vResult, M4_FALSE, __SOURCE__ ) ;
	}
}


void	CliTextContext::SetCheckLimits( m4double_t ai_dHandle, m4bool_t ai_bCheckLimits )
{

	M4JavaObject	vResult ;
	M4JavaObject	avArguments[ 1 ] ;


	if( IsReady() == M4_TRUE && ai_dHandle != 0 )
	{
		avArguments[ 0 ].SetBoolean( ai_bCheckLimits ) ;

		m_poJavaContext->InvokeMethod( ai_dHandle, m_iSetCheckLimits, avArguments, 1, vResult, M4_FALSE, __SOURCE__ ) ;
	}
}


void	CliTextContext::AddText( m4double_t ai_dHandle, m4double_t ai_dPosX, m4double_t ai_dPosY, m4pcchar_t ai_pccText, m4int32_t ai_iLength )
{

	M4JavaObject	vResult ;
	M4JavaObject	avArguments[ 3 ] ;


	if( IsReady() == M4_TRUE && ai_dHandle != 0 && ai_iLength != 0 )
	{
		m4pchar_t pcText;
		m4char_t oldChar;
		if (ai_iLength > 0)
		{
			pcText = (m4char_t*)ai_pccText;
			oldChar = pcText[ai_iLength];
			pcText[ai_iLength] = '\0';
		}
		avArguments[ 0 ].SetString( ai_pccText, M4_FALSE ) ;
		avArguments[ 1 ].SetFloat( ai_dPosX ) ;
		avArguments[ 2 ].SetFloat( ai_dPosY ) ;

		m_poJavaContext->InvokeMethod( ai_dHandle, m_iAddText, avArguments, 3, vResult, M4_FALSE, __SOURCE__ ) ;
		if (ai_iLength > 0)
		{
			pcText[ai_iLength] = oldChar;
		}
	}
}


m4int32_t	CliTextContext::AddNewPage( m4double_t ai_dHandle, m4double_t ai_dWidth, m4double_t ai_dHeight )
{

	m4return_t		iResult = M4_ERROR ;
	M4JavaObject	vResult ;
	M4JavaObject	avArguments[ 2 ] ;


	if( IsReady() == M4_TRUE && ai_dHandle != 0 )
	{
		avArguments[ 0 ].SetFloat( ai_dWidth ) ;
		avArguments[ 1 ].SetFloat( ai_dHeight ) ;

		iResult = m_poJavaContext->InvokeMethod( ai_dHandle, m_iAddNewPage, avArguments, 2, vResult, M4_FALSE, __SOURCE__ ) ;

		if( iResult == M4_SUCCESS )
		{
			return( vResult.GetInteger() ) ;
		}
	}

	return( -1 ) ;
}


m4int32_t	CliTextContext::GetActualPage( m4double_t ai_dHandle )
{

	m4return_t		iResult = M4_ERROR ;
	M4JavaObject	vResult ;


	if( IsReady() == M4_TRUE && ai_dHandle != 0 )
	{
		iResult = m_poJavaContext->InvokeMethod( ai_dHandle, m_iGetActualPage, NULL, 0, vResult, M4_FALSE, __SOURCE__ ) ;

		if( iResult == M4_SUCCESS )
		{
			return( vResult.GetInteger() ) ;
		}
	}

	return( -1 ) ;
}


m4int32_t	CliTextContext::GetNumPages( m4double_t ai_dHandle )
{

	m4return_t		iResult = M4_ERROR ;
	M4JavaObject	vResult ;


	if( IsReady() == M4_TRUE && ai_dHandle != 0 )
	{
		iResult = m_poJavaContext->InvokeMethod( ai_dHandle, m_iGetNumPages, NULL, 0, vResult, M4_FALSE, __SOURCE__ ) ;

		if( iResult == M4_SUCCESS )
		{
			return( vResult.GetInteger() ) ;
		}
	}

	return( -1 ) ;
}


void	CliTextContext::SetActualPage( m4double_t ai_dHandle, m4int32_t ai_iNumPage )
{

	M4JavaObject	vResult ;
	M4JavaObject	avArguments[ 1 ] ;


	if( IsReady() == M4_TRUE && ai_dHandle != 0 )
	{
		avArguments[ 0 ].SetInteger( ai_iNumPage ) ;

		m_poJavaContext->InvokeMethod( ai_dHandle, m_iSetActualPage, avArguments, 1, vResult, M4_FALSE, __SOURCE__ ) ;
	}
}


m4bool_t	CliTextContext::SetFont( m4double_t ai_dHandle, m4pcchar_t ai_pccFontName, m4double_t ai_dFontSize, m4int32_t ai_iFontStyle )
{

	m4return_t		iResult = M4_ERROR ;
	M4JavaObject	vResult ;
	M4JavaObject	avArguments[ 3 ] ;


	if( IsReady() == M4_TRUE && ai_dHandle != 0 )
	{
		avArguments[ 0 ].SetString( ai_pccFontName, M4_FALSE ) ;
		avArguments[ 1 ].SetFloat( ai_dFontSize ) ;
		avArguments[ 2 ].SetInteger( ai_iFontStyle ) ;

		iResult = m_poJavaContext->InvokeMethod( ai_dHandle, m_iSetFont, avArguments, 3, vResult, M4_FALSE, __SOURCE__ ) ;

		if( iResult == M4_SUCCESS )
		{
			return( vResult.GetBoolean() ) ;
		}
	}

	return( M4_FALSE ) ;
}


void	CliTextContext::SetColor( m4double_t ai_dHandle, m4int32_t ai_iRed, m4int32_t ai_iGreen, m4int32_t ai_iBlue )
{

	M4JavaObject	vResult ;
	M4JavaObject	avArguments[ 3 ] ;


	if( IsReady() == M4_TRUE && ai_dHandle != 0 )
	{
		avArguments[ 0 ].SetInteger( ai_iRed ) ;
		avArguments[ 1 ].SetInteger( ai_iGreen ) ;
		avArguments[ 2 ].SetInteger( ai_iBlue ) ;

		m_poJavaContext->InvokeMethod( ai_dHandle, m_iSetColor, avArguments, 3, vResult, M4_FALSE, __SOURCE__ ) ;
	}
}


void	CliTextContext::AddLine( m4double_t ai_dHandle, m4double_t ai_dPosX1, m4double_t ai_dPosY1, m4double_t ai_dPosX2, m4double_t ai_dPosY2, m4double_t ai_dWidth, m4int32_t ai_iStyle )
{

	M4JavaObject	vResult ;
	M4JavaObject	avArguments[ 6 ] ;


	if( IsReady() == M4_TRUE && ai_dHandle != 0 )
	{
		avArguments[ 0 ].SetFloat( ai_dPosX1 ) ;
		avArguments[ 1 ].SetFloat( ai_dPosY1 ) ;
		avArguments[ 2 ].SetFloat( ai_dPosX2 ) ;
		avArguments[ 3 ].SetFloat( ai_dPosY2 ) ;
		avArguments[ 4 ].SetFloat( ai_dWidth ) ;
		avArguments[ 5 ].SetInteger( ai_iStyle ) ;

		m_poJavaContext->InvokeMethod( ai_dHandle, m_iAddLine, avArguments, 6, vResult, M4_FALSE, __SOURCE__ ) ;
	}
}


void	CliTextContext::AddRectangle( m4double_t ai_dHandle, m4double_t ai_dPosX, m4double_t ai_dPosY, m4double_t ai_dWidth, m4double_t ai_dHeight, m4double_t ai_dLineWidth, m4int32_t ai_iStyle )
{

	M4JavaObject	vResult ;
	M4JavaObject	avArguments[ 6 ] ;


	if( IsReady() == M4_TRUE && ai_dHandle != 0 )
	{
		avArguments[ 0 ].SetFloat( ai_dPosX ) ;
		avArguments[ 1 ].SetFloat( ai_dPosY ) ;
		avArguments[ 2 ].SetFloat( ai_dWidth ) ;
		avArguments[ 3 ].SetFloat( ai_dHeight ) ;
		avArguments[ 4 ].SetFloat( ai_dLineWidth ) ;
		avArguments[ 5 ].SetInteger( ai_iStyle ) ;

		m_poJavaContext->InvokeMethod( ai_dHandle, m_iAddRectangle, avArguments, 6, vResult, M4_FALSE, __SOURCE__ ) ;
	}
}


void	CliTextContext::FillZone( m4double_t ai_dHandle, m4double_t ai_dPosX, m4double_t ai_dPosY, m4double_t ai_dWidth, m4double_t ai_dHeight )
{

	M4JavaObject	vResult ;
	M4JavaObject	avArguments[ 4 ] ;


	if( IsReady() == M4_TRUE && ai_dHandle != 0 )
	{
		avArguments[ 0 ].SetFloat( ai_dPosX ) ;
		avArguments[ 1 ].SetFloat( ai_dPosY ) ;
		avArguments[ 2 ].SetFloat( ai_dWidth ) ;
		avArguments[ 3 ].SetFloat( ai_dHeight ) ;

		m_poJavaContext->InvokeMethod( ai_dHandle, m_iFillZone, avArguments, 4, vResult, M4_FALSE, __SOURCE__ ) ;
	}
}


void	CliTextContext::AddImage( m4double_t ai_dHandle, m4pcchar_t ai_pccFileName, m4double_t ai_dPosX, m4double_t ai_dPosY, m4double_t ai_dWidth, m4double_t ai_dHeight, m4int32_t ai_iLength )
{

	M4JavaObject	vResult ;
	M4JavaObject	avArguments[ 5 ] ;


	if( IsReady() == M4_TRUE && ai_dHandle != 0 && ai_iLength != 0 )
	{
		m4pchar_t pcText;
		m4char_t oldChar;
		if (ai_iLength > 0)
		{
			pcText = (m4char_t*)ai_pccFileName;
			oldChar = pcText[ai_iLength];
			pcText[ai_iLength] = '\0';
		}
		avArguments[ 0 ].SetString( ai_pccFileName, M4_FALSE ) ;
		avArguments[ 1 ].SetFloat( ai_dPosX ) ;
		avArguments[ 2 ].SetFloat( ai_dPosY ) ;
		avArguments[ 3 ].SetFloat( ai_dWidth ) ;
		avArguments[ 4 ].SetFloat( ai_dHeight ) ;

		m_poJavaContext->InvokeMethod( ai_dHandle, m_iAddImage, avArguments, 5, vResult, M4_FALSE, __SOURCE__ ) ;
		if (ai_iLength > 0)
		{
			pcText[ai_iLength] = oldChar;
		}
	}
}


void	CliTextContext::Close( m4double_t ai_dHandle  )
{

	M4JavaObject	vResult ;


	if( IsReady() == M4_TRUE && ai_dHandle != 0 )
	{
		 m_poJavaContext->InvokeMethod( ai_dHandle, m_iClose, NULL, 0, vResult, M4_FALSE, __SOURCE__ ) ;
		 m_poJavaContext->DestroyObject( ai_dHandle, M4_FALSE, __SOURCE__ ) ;
	}
}



