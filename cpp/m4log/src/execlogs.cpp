
//==============================================================================
//
// (c) Copyright  1991-2015 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4log.dll
// File:                execlogs.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                25-02-2015
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//	This module defines the class for handling log files per thread and
//	executor type (oltp, xml, metadata).
//
//==============================================================================

#include "execlogs.hpp"
#include "blocksynchronization.hpp"
#include "m4unicode.hpp"
#include <sys/stat.h>
#include "cldates.hpp"
#include <stdarg.h>

// ============================================================================
// Class M4ExecLogFile
// ============================================================================

// ----------------------------------------------------------------------------
// Constructor.
// ----------------------------------------------------------------------------
M4ExecLogFile::M4ExecLogFile()
{
	m_bInitialized	= M4_FALSE;
}

// ----------------------------------------------------------------------------
// Destructor.
// ----------------------------------------------------------------------------
M4ExecLogFile::~M4ExecLogFile()
{
}

// ----------------------------------------------------------------------------
// Initializes de log file object.
// ----------------------------------------------------------------------------
void M4ExecLogFile::Initialize( m4pcchar_t ai_pccName, m4uint32_t ai_iSize, m4pcchar_t ai_pccHeader, m4pcchar_t ai_pccExtension, m4bool_t ai_bTick, m4bool_t ai_bTime, m4bool_t ai_bThread, m4bool_t ai_bIsUnicode, m4uint32_t ai_iTotalSize )
{
	if( m_bInitialized == M4_TRUE )
	{
		return;
	}

	// Base clase initialization.
	_Init( ai_pccName, ai_iSize, ai_pccHeader, ai_pccExtension, ai_bTick, ai_bTime, ai_bThread, ai_bIsUnicode, ai_iTotalSize );
	
	m_bInitialized	= M4_TRUE;
}

// ----------------------------------------------------------------------------
// API for writting in the log file, including the members of a given logger
// object.
// Overrides base implementation.
// ----------------------------------------------------------------------------
m4bool_t M4ExecLogFile::WriteLine( M4ThreadLogger* ai_pThreadLogger, m4pcchar_t ai_pccFormat, ... )
{
	m4bool_t	bNewFile = M4_FALSE;
	va_list		vaList;


	if( m_bInitialized == M4_FALSE )
	{
		return M4_FALSE;
	}

	ClMutBlock oMutexBlock( m_poMutex );

	va_start( vaList, ai_pccFormat );

	bNewFile |= _WriteLineStart();
	bNewFile |= _WriteString( ai_pccFormat, vaList );
	bNewFile |= _WriteUsrTimeAudit( ai_pThreadLogger );
	bNewFile |= _WriteLineEnd();

	va_end( vaList );


	return( bNewFile );
}

// ----------------------------------------------------------------------------
// Internal. Writes the user-timed-actions-auditory primary key columns
// previously stored in the logger object that it receives.
// ----------------------------------------------------------------------------
m4bool_t M4ExecLogFile::_WriteUsrTimeAudit( M4ThreadLogger* ai_pThreadLogger )
{
	m4bool_t	bNewFile = M4_FALSE;
    m4char_t	acBuffer[ 128 + 1 ];

	if( ai_pThreadLogger == NULL )
	{
		return M4_FALSE;
	}

	sprintf( acBuffer, "\t%s\t", ( ai_pThreadLogger->GetSessionKey() != NULL ) ? ai_pThreadLogger->GetSessionKey() : "" );
	bNewFile |= _WriteString( acBuffer );

	sprintf( acBuffer, "%s\t", ( ai_pThreadLogger->GetServerName() != NULL ) ? ai_pThreadLogger->GetServerName() : "" );
	bNewFile |= _WriteString( acBuffer );

	if( ai_pThreadLogger->GetServerPort() != -1 )
	{
		sprintf( acBuffer, "%d\t", ai_pThreadLogger->GetServerPort() );
		bNewFile |= _WriteString( acBuffer );
	}

	if( ai_pThreadLogger->GetSessionId() != -1 )
	{
		sprintf( acBuffer, "%d\t", ai_pThreadLogger->GetSessionId() );
		bNewFile |= _WriteString( acBuffer );
	}

	if( ai_pThreadLogger->GetClickNum() != -1 )
	{
		sprintf( acBuffer, "%d\t", ai_pThreadLogger->GetClickNum() );
		bNewFile |= _WriteString( acBuffer );
	}

	sprintf( acBuffer, "%s", ( ai_pThreadLogger->GetReqUniqueId() != NULL ) ? ai_pThreadLogger->GetReqUniqueId() : "" );
	bNewFile |= _WriteString( acBuffer );

	if( ai_pThreadLogger->GetReqNumber() != -1 )
	{
		sprintf( acBuffer, "/%d", ai_pThreadLogger->GetReqNumber() );
		bNewFile |= _WriteString( acBuffer );
	}

	return( bNewFile );
}


// ============================================================================
// Class M4ThreadLogger
// ============================================================================

// ----------------------------------------------------------------------------
// Statics.
// ----------------------------------------------------------------------------
M4ExecLogFile*		M4ThreadLogger::s_pOltpLogFile = NULL;
M4ExecLogFile*		M4ThreadLogger::s_pXMLLogFile = NULL;
M4ExecLogFile*		M4ThreadLogger::s_pMDLogFile = NULL;
ThreadLoggersMap_t	M4ThreadLogger::s_ThreadLoggersStore;
ClMutex				M4ThreadLogger::s_oMutex( M4_TRUE );

// ----------------------------------------------------------------------------
// Constructor.
// ----------------------------------------------------------------------------
M4ThreadLogger::M4ThreadLogger( eLogFile_t ai_iLogFile )
{
	m_iLogFile		= ai_iLogFile;
	m_pcServerName	= NULL;

	Reset();
}

// ----------------------------------------------------------------------------
// Destrcutor.
// ----------------------------------------------------------------------------
M4ThreadLogger::~M4ThreadLogger()
{
	if( m_pcServerName != NULL )
	{
		delete m_pcServerName;
	}
}

// ----------------------------------------------------------------------------
// Initializes the logger object which in turn initializes the executor
// log file object that it contains.
// ----------------------------------------------------------------------------
void M4ThreadLogger::InitLogFile( m4pcchar_t ai_pccName, m4uint32_t ai_iSize, m4pcchar_t ai_pccHeader, m4pcchar_t ai_pccExtension, m4bool_t ai_bTick, m4bool_t ai_bTime, m4bool_t ai_bThread, m4bool_t ai_bIsUnicode, m4uint32_t ai_iTotalSize )
{
	M4ExecLogFile**	ppLogFile = NULL;

	
	s_oMutex.Lock();

	switch( m_iLogFile )
	{
		case OLTP_LOG: 

			ppLogFile = &s_pOltpLogFile;
			break;

		case XML_LOG:

			ppLogFile = &s_pXMLLogFile;
			break;

		case MD_LOG:

			ppLogFile = &s_pMDLogFile;
			break;
	}

	// There is only one instance per executor type.
	if( ppLogFile != NULL && *ppLogFile == NULL )
	{
		*ppLogFile = new M4ExecLogFile();

		(*ppLogFile)->Initialize( ai_pccName, ai_iSize, ai_pccHeader, ai_pccExtension, ai_bTick, ai_bTime, ai_bThread, ai_bIsUnicode, ai_iTotalSize );
	}

	s_oMutex.Unlock();
}

// ----------------------------------------------------------------------------
// Definitions.
// ----------------------------------------------------------------------------
#define MAX_LINE_LENGTH	1024

#if defined	(_WIN32)
	#define fComposeString	_vsnprintf
#else
	#define fComposeString	vsnprintf
#endif

// ----------------------------------------------------------------------------
// API for writing in the executor log.
// ----------------------------------------------------------------------------
m4bool_t	M4ThreadLogger::WriteLine( m4pcchar_t ai_pccFormat, ... )
{
	int				iRet;
	m4bool_t		bRet = M4_FALSE;
    va_list			vaList;
	m4char_t		acBuffer[ MAX_LINE_LENGTH + 1 ] ;
	M4ExecLogFile*	pLogFile = NULL;

	
	switch( m_iLogFile )
	{
		case OLTP_LOG: 

			pLogFile = s_pOltpLogFile;
			break;

		case XML_LOG:

			pLogFile = s_pXMLLogFile;
			break;

		case MD_LOG:

			pLogFile = s_pMDLogFile;
			break;
	}

	
	if( pLogFile != NULL )
	{
		va_start( vaList, ai_pccFormat );
		
		iRet = fComposeString( acBuffer, MAX_LINE_LENGTH, ai_pccFormat, vaList );
	
		if( iRet == -1 )
		{
			acBuffer[ MAX_LINE_LENGTH ] = '\0';
		}

		// Synchronized.
		bRet = pLogFile->WriteLine( this, "%s", acBuffer );

		va_end( vaList );
	}


	return bRet;
}

// ----------------------------------------------------------------------------
// Resets User time auditory attributes.
// ----------------------------------------------------------------------------
void M4ThreadLogger::Reset()
{
	m_pcSessionKey	= NULL;

	if( m_pcServerName != NULL )
	{
		delete m_pcServerName;
		m_pcServerName	= NULL;
	}
	
	m_iServerPort	= -1;
	m_iSessionId	= -1;
	m_iClickNum		= -1;
	m_pcReqUniqueId	= NULL;
	m_iReqNumber	= -1;
}

// ----------------------------------------------------------------------------
// Getter for session key.
// ----------------------------------------------------------------------------
m4pchar_t M4ThreadLogger::GetSessionKey()
{
	return m_pcSessionKey;
}

// ----------------------------------------------------------------------------
// Getter for server name.
// ----------------------------------------------------------------------------
m4pchar_t M4ThreadLogger::GetServerName()
{
	return m_pcServerName;
}

// ----------------------------------------------------------------------------
// Getter for server port.
// ----------------------------------------------------------------------------
int	M4ThreadLogger::GetServerPort()
{
	return m_iServerPort;
}

// ----------------------------------------------------------------------------
// Getter for session Id.
// ----------------------------------------------------------------------------
int	M4ThreadLogger::GetSessionId()
{
	return m_iSessionId;
}

// ----------------------------------------------------------------------------
// Getter for user click number.
// ----------------------------------------------------------------------------
int	M4ThreadLogger::GetClickNum()
{
	return m_iClickNum;
}

// ----------------------------------------------------------------------------
// Getter for request unique Id.
// ----------------------------------------------------------------------------
m4pchar_t M4ThreadLogger::GetReqUniqueId()
{
	return m_pcReqUniqueId;
}

// ----------------------------------------------------------------------------
// Getter for request number (xml).
// ----------------------------------------------------------------------------
int	M4ThreadLogger::GetReqNumber()
{
	return m_iReqNumber;
}

// ----------------------------------------------------------------------------
// Setter for session key.
// Does not make a copy of the string passed because it comes from the 
// session channel.
// ----------------------------------------------------------------------------
void M4ThreadLogger::SetSessionKey( m4pchar_t ai_pcSessionKey )
{
	m_pcSessionKey = ai_pcSessionKey;
}

// ----------------------------------------------------------------------------
// Setter for server name.
// Makes a copy of the string because it is volatile.
// ----------------------------------------------------------------------------
void M4ThreadLogger::SetServerName( m4pcchar_t ai_pcServerName )
{
	size_t	iLen;

	if( ai_pcServerName == NULL )
	{
		if( m_pcServerName != NULL )
		{
			delete m_pcServerName;
			m_pcServerName = NULL;
		}

		return;
	}

	iLen = strlen( ai_pcServerName );

	if( m_pcServerName == NULL )
	{
		m_pcServerName = new m4char_t[ iLen + 1 ];
	}
	else if( iLen > strlen( m_pcServerName ) )
	{
		delete m_pcServerName;
		m_pcServerName = new m4char_t[ iLen + 1 ];
	}

	strcpy( m_pcServerName, ai_pcServerName );
}

// ----------------------------------------------------------------------------
// Setter for server port.
// ----------------------------------------------------------------------------
void M4ThreadLogger::SetServerPort( int ai_iServerPort )
{
	m_iServerPort = ai_iServerPort;
}

// ----------------------------------------------------------------------------
// Setter for session Id.
// ----------------------------------------------------------------------------
void M4ThreadLogger::SetSessionId( int ai_iSessionId )
{
	m_iSessionId = ai_iSessionId;
}

// ----------------------------------------------------------------------------
// Setter for user click number.
// ----------------------------------------------------------------------------
void M4ThreadLogger::SetClickNum( int ai_iClickNum )
{
	m_iClickNum = ai_iClickNum;
}

// ----------------------------------------------------------------------------
// Setter for request unique Id.
// ----------------------------------------------------------------------------
void M4ThreadLogger::SetReqUniqueId( m4pchar_t ai_pcReqUniqueId )
{
	m_pcReqUniqueId = ai_pcReqUniqueId;
}

// ----------------------------------------------------------------------------
// Setter for request number (xml).
// ----------------------------------------------------------------------------
void M4ThreadLogger::SetReqNumber( int ai_iReqNumber )
{
	m_iReqNumber = ai_iReqNumber;
}

// ----------------------------------------------------------------------------
// Object factory.
// ----------------------------------------------------------------------------
M4ThreadLogger*	M4ThreadLogger::GetInstance( eLogFile_t ai_iLogFile  )
{
	m4iden_t			lTheadId = M4Thread::GetCurrentIdThread();
	ThreadLoggersMapIt_t	iteLoggers;
	M4ThreadLogger*		pLogger;


	s_oMutex.Lock();

	iteLoggers = s_ThreadLoggersStore.find( lTheadId );

	if( iteLoggers == s_ThreadLoggersStore.end() )
	{	
		pLogger = new M4ThreadLogger( ai_iLogFile );
		s_ThreadLoggersStore[ lTheadId ] = pLogger;
	}
	else
	{
		pLogger = ( M4ThreadLogger* )( *iteLoggers ).second;
	}

	s_oMutex.Unlock();


	return pLogger;
}
