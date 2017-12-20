
//==============================================================================
//
// (c) Copyright  1991-2015 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4log.dll
// File:                execlogs.hpp   
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

#ifndef execlogs_hpp
#define execlogs_hpp 1

#include "m4logfile.hpp"
#include "m4thread.hpp"
#include "m4stl.hpp"


// ----------------------------------------------------------------------------
// M4ExecLogFile
//
// Specialization for writting the user time audit information into the trace.
// There is only one instance per executor type (oltp, xml, metadata).
// ----------------------------------------------------------------------------

class M4ThreadLogger;

class M4_DECL_M4LOG M4ExecLogFile: public M4LogFile
{
	public:
	
		// Constructor.
		M4ExecLogFile();

		// Destrcutor.
		~M4ExecLogFile	();

		// Initializer.
		void		Initialize( m4pcchar_t ai_pccName, m4uint32_t ai_iSize, m4pcchar_t ai_pccHeader = NULL, m4pcchar_t ai_pccExtension = "log", m4bool_t ai_bTick = M4_FALSE, m4bool_t ai_bTime = M4_FALSE, m4bool_t ai_bThread = M4_FALSE, m4bool_t ai_bIsUnicode = M4_FALSE, m4uint32_t ai_iTotalSize = 0 );

		// API for writting in the log file, including the members of a given logger object.
		m4bool_t	WriteLine( M4ThreadLogger* ai_pThreadLogger, m4pcchar_t ai_pccFormat, ... );

	private:

		// Internal.
		m4bool_t	_WriteUsrTimeAudit( M4ThreadLogger* ai_pThreadLogger );

		// Status.
		m4bool_t	m_bInitialized;
};


// ----------------------------------------------------------------------------
// M4ExecLogger
//
// API for writing in a given executor log file.
// There is one instance per thread in order to simplify the arguments passing.
// ----------------------------------------------------------------------------

// Types.
typedef map< m4iden_t, M4ThreadLogger*, less< m4iden_t > >	ThreadLoggersMap_t;
typedef ThreadLoggersMap_t::iterator						ThreadLoggersMapIt_t;

// Executor types
typedef enum { OLTP_LOG, XML_LOG, MD_LOG }					eLogFile_t;


class M4_DECL_M4LOG M4ThreadLogger
{
	public:

		// Destructor.
		~M4ThreadLogger();

		// Initializer.
		void		InitLogFile( m4pcchar_t ai_pccName, m4uint32_t ai_iSize, m4pcchar_t ai_pccHeader = NULL, m4pcchar_t ai_pccExtension = "log", m4bool_t ai_bTick = M4_FALSE, m4bool_t ai_bTime = M4_FALSE, m4bool_t ai_bThread = M4_FALSE, m4bool_t ai_bIsUnicode = M4_FALSE, m4uint32_t ai_iTotalSize = 0 );

		// API for writing in the executor log.
		m4bool_t	WriteLine( m4pcchar_t ai_pccFormat, ... );	

		// Resets User time auditory attributes.
		void		Reset();

		// Getters.
		m4pchar_t	GetSessionKey();
		m4pchar_t	GetServerName();
		int			GetServerPort();
		int			GetSessionId();
		int			GetClickNum();
		m4pchar_t	GetReqUniqueId();
		int			GetReqNumber();

		// Setters.
		void		SetSessionKey( m4pchar_t ai_pcSessionKey );
		void		SetServerName( m4pcchar_t ai_pcServerName );
		void		SetServerPort( int ai_iServerPort );
		void		SetSessionId( int ai_iSessionId );
		void		SetClickNum( int ai_iClickNum );
		void		SetReqUniqueId( m4pchar_t ai_pcReqUniqueId );
		void		SetReqNumber( int ai_iReqNumber);
		
		// Object factory.
		static M4ThreadLogger*	GetInstance( eLogFile_t ai_iLogFile );

	private:

		// Constructor.
		M4ThreadLogger( eLogFile_t ai_iLogFile );

		// Executor type.
		eLogFile_t	m_iLogFile;

		// User time auditory attributes.
		m4pchar_t	m_pcSessionKey;
		m4pchar_t	m_pcServerName;
		int			m_iServerPort;
		int			m_iSessionId;
		int			m_iClickNum;
		m4pchar_t	m_pcReqUniqueId;
		int			m_iReqNumber;

		// A single log file object for the m4oltp.log.
		static M4ExecLogFile*		s_pOltpLogFile;

		// A single log file object for the m4xml.log.
		static M4ExecLogFile*		s_pXMLLogFile;

		// A single log file object for the m4metadata.log.
		static M4ExecLogFile*		s_pMDLogFile;

		// Instances repository.
		static ThreadLoggersMap_t	s_ThreadLoggersStore;

		// Access control.
		static ClMutex				s_oMutex;
};

#endif
