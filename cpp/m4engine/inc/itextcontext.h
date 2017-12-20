
//==============================================================================
//
// (c) Copyright  1991-2015 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              enginedll
// File:                itextcontext.hpp   
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
//    En este módulo se declara el objeto para manejar el conexto java de acceso
//    a iText
//
//==============================================================================


#include "mit.h"



#ifndef __M4ITEXTCONTEXT_HPP__
#define __M4ITEXTCONTEXT_HPP__



//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	M4JavaContext ;
class	ClVMBaseEnv ;
class	IExecutor ;




//=================================================================================
//
// CliTextContext
//
// Clase que implementa el objeto para manejar el conetxto java de acceso a iText
//
//=================================================================================

class	CliTextContext
{

protected:

	m4uint32_t		m_iSetTextDirection ;
	m4uint32_t		m_iSetCheckLimits ;
	m4uint32_t		m_iAddText ;
	m4uint32_t		m_iAddNewPage ;
	m4uint32_t		m_iGetActualPage ;
	m4uint32_t		m_iGetNumPages ;
	m4uint32_t		m_iSetActualPage ;
	m4uint32_t		m_iSetFont ;
	m4uint32_t		m_iSetColor ;
	m4uint32_t		m_iAddLine ;
	m4uint32_t		m_iAddRectangle ;
	m4uint32_t		m_iFillZone ;
	m4uint32_t		m_iAddImage ;
	m4uint32_t		m_iClose ;

	M4JavaContext*	m_poJavaContext ;

	m4return_t	_Init( ClVMBaseEnv *ai_poEnvironment, ClMIT_Trace *ai_poTrace ) ;
	m4return_t	_InitMethods( void ) ;

public:


// Funciones de inicialización ================================================

		CliTextContext( void ) ;
		~CliTextContext( void ) ;

	m4return_t	Init( ClVMBaseEnv *ai_poEnvironment, IExecutor *ai_poExecutor, ClMIT_Trace *ai_poTrace ) ;
	m4return_t	NewiTextStub( m4pcchar_t ai_pccFileName, m4bool_t ai_bProtect, m4int32_t ai_iDebugLevel, m4double_t& ao_rdHandle ) ;

// Funciones de lectura =======================================================

	m4bool_t	IsReady( void ) const ;
	m4bool_t	IsInitialized( void ) const ;

// Funciones del API ==========================================================

	void		SetTextDirection( m4double_t ai_dHandle, m4int32_t ai_iTextDirection ) ;
	void		SetCheckLimits  ( m4double_t ai_dHandle, m4bool_t ai_bCheckLimits ) ;
	void		AddText			( m4double_t ai_dHandle, m4double_t ai_dPosX, m4double_t ai_dPosY, m4pcchar_t ai_pccText, m4int32_t ai_iLength = -1 ) ;
	m4int32_t	AddNewPage		( m4double_t ai_dHandle, m4double_t ai_dWidth, m4double_t ai_dHeight )  ;
	m4int32_t	GetActualPage	( m4double_t ai_dHandle ) ;
	m4int32_t	GetNumPages		( m4double_t ai_dHandle ) ;
	void		SetActualPage	( m4double_t ai_dHandle, m4int32_t ai_iNumPage ) ;
	m4bool_t	SetFont			( m4double_t ai_dHandle, m4pcchar_t ai_pccFontName, m4double_t ai_dFontSize, m4int32_t ai_iFontStyle ) ;
	void		SetColor		( m4double_t ai_dHandle, m4int32_t ai_iRed, m4int32_t ai_iGreen, m4int32_t ai_iBlue ) ;
	void		AddLine			( m4double_t ai_dHandle, m4double_t ai_dPosX1, m4double_t ai_dPosY1, m4double_t ai_dPosX2, m4double_t ai_dPosY2, m4double_t ai_dWidth, m4int32_t ai_iStyle ) ;
	void		AddRectangle	( m4double_t ai_dHandle, m4double_t ai_dPosX, m4double_t ai_dPosY, m4double_t ai_dWidth, m4double_t ai_dHeight, m4double_t ai_dLineWidth, m4int32_t ai_iStyle ) ;
	void		FillZone		( m4double_t ai_dHandle, m4double_t ai_dPosX, m4double_t ai_dPosY, m4double_t ai_dWidth, m4double_t ai_dHeight ) ;
	void		AddImage		( m4double_t ai_dHandle, m4pcchar_t ai_pccFileName, m4double_t ai_dPosX, m4double_t ai_dPosY, m4double_t ai_dWidth, m4double_t ai_dHeight, m4int32_t ai_iLength = -1 ) ;
	void		Close			( m4double_t ai_dHandle  ) ;

};


#endif

