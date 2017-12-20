
//==============================================================================
//
// (c) Copyright  1991-2012 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4log.dll
// File:                m4logfile.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                27-02-2012
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the class for handling log files dumping and backup
//
//
//==============================================================================


#include <stdio.h>
#include <stdarg.h>
#include "m4stl.hpp"
#include "m4log_dll.hpp"


#ifndef _M4LOGFILE_HPP_
#define _M4LOGFILE_HPP_


//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class ClMutex;




//=============================================================================
// Estructuras auxiliares
//=============================================================================

typedef struct StBackUp_tag
{
	m4date_t	m_dDate ;
	m4uint32_t	m_iSize ;
} StBackUp_t;


#ifdef _SOLARIS
typedef os_queue<StBackUp_t>	BackUpList_t ;
#else
typedef queue<StBackUp_t>		BackUpList_t ;
#endif


// ============================================================================
// Defines
// ============================================================================

#define	M4LOG_START					" ========== Application start ===================================================\n\n"

#ifdef _WINDOWS
#define	M4LOG_LONG_FORMAT			"%I64d"
#define	M4LOG_MAX_PATH				MAX_PATH
#define	M4LOG_DEBUG_DIR				"TEMP"
#else
#define	M4LOG_LONG_FORMAT			"%lld"
#define	M4LOG_MAX_PATH				PATH_MAX
#define	M4LOG_DEBUG_DIR				"HOME"
#endif

//=================================================================================
//
// M4LogFile
//
// Clase que implementa los ficheros de traza
//
//=================================================================================

class	M4_DECL_M4LOG	M4LogFile
{

protected:

	m4date_t		m_bStart ;
	m4bool_t		m_bTick ;
	m4bool_t		m_bTime ;
	m4bool_t		m_bThread ;
	m4bool_t		m_bIsUnicode ;
    size_t			m_iLength ;
    m4uint32_t		m_iSize ;
    m4uint32_t		m_iTotalLength ;
    m4uint32_t		m_iTotalSize ;
	m4pchar_t		m_pcName ;
	m4pchar_t		m_pcHeader ;
	m4pchar_t		m_pcExtension ;
	m4pchar_t		m_pcDirectory ;
	FILE*			m_pfFile ;
	ClMutex*		m_poMutex ;
	BackUpList_t*	m_poBackUps ;

	static	m4date_t	sm_dMaxPerdiod ;


// Funciones auxiliares =======================================================

	void		_Init( m4pcchar_t ai_pccName, m4uint32_t ai_iSize, m4pcchar_t ai_pccHeader = NULL, m4pcchar_t ai_pccExtension = "log", m4bool_t ai_bTick = M4_FALSE, m4bool_t ai_bTime = M4_FALSE, m4bool_t ai_bThread = M4_FALSE, m4bool_t ai_bIsUnicode = M4_FALSE, m4uint32_t ai_iTotalSize = 0 );
	m4pchar_t	_GetDirectory( void ) ;
	void		_ComposeFileName( m4pchar_t ao_pcFile, m4date_t ai_dDate = 0 ) ;
	void		_LoadBackUps( void ) ;
	void		_AddBackUp( m4date_t ai_dDate ) ;
	void		_CleanBackUps( void ) ;
	void		_Rename( void ) ;
	m4bool_t	_Open( void ) ;
	m4bool_t	_WriteString( m4pcchar_t ai_pccString ) ;
	m4bool_t	_WriteString( m4pcchar_t ai_pccFormat, va_list ai_vaList ) ;
	m4bool_t	_WriteLineStart( void ) ;
	m4bool_t	_WriteLineEnd( void ) ;

public:

// Funciones de inicialización ================================================

		M4LogFile();
		M4LogFile( m4pcchar_t ai_pccName, m4uint32_t ai_iSize, m4pcchar_t ai_pccHeader = NULL, m4pcchar_t ai_pccExtension = "log", m4bool_t ai_bTick = M4_FALSE, m4bool_t ai_bTime = M4_FALSE, m4bool_t ai_bThread = M4_FALSE, m4bool_t ai_bIsUnicode = M4_FALSE, m4uint32_t ai_iTotalSize = 0 ) ;
		~M4LogFile( void ) ;

// Funciones de escritura =====================================================

	m4bool_t	WriteString( m4pcchar_t ai_pccString ) ;
	m4bool_t	WriteLineStart( void ) ;
	m4bool_t	WriteLineEnd( void ) ;

	m4bool_t	WriteLine( m4pcchar_t ai_pccFormat, ... ) ;

// Funciones estáticas ========================================================

	static m4date_t	GetMaxPeriod()
	{
		return( sm_dMaxPerdiod ) ;
	}

	static void	SetMaxPeriod( m4date_t ai_dMaxPeriod )
	{
		sm_dMaxPerdiod = ai_dMaxPeriod ;
	}
} ;


#endif

