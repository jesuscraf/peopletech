//==============================================================================
//
// (c) Copyright  1991-2015 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4doc 
// File:             m4filedownldr.hpp
// Project:          Document Management
// Author:           Meta Software M.S. , S.A
// Date:             04-March-2015
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
//
// Definition:
//		Class M4FileDownloader
//==============================================================================


#ifndef M4FILEDOWNLDR_HPP 
#define M4FILEDOWNLDR_HPP 1

#include "m4types.hpp"
#include "m4doc_dll.hpp"

#ifdef _WINDOWS
	#include <windows.h>
	#include <wininet.h>
#endif


	
class M4_DECL_M4DOC M4FileDownloader
{
	public:

		M4FileDownloader();
		~M4FileDownloader();

		m4return_t	DownloadFile( m4pcchar_t ai_pccSource, m4pchar_t aio_pcTarget, m4int_t ai_iLength );

	private:

	#ifdef _WINDOWS
		char		m_acURL[ INTERNET_MAX_URL_LENGTH + 1 ];
		
		HINTERNET	m_hInstance;
		HINTERNET	m_hConnect;
		HINTERNET	m_hRequest;
	#endif

		m4bool_t	m_bSSL;
		m4pchar_t	m_pcServer;
		m4int_t		m_iPort;
		m4pchar_t	m_pcResName;
		m4pchar_t	m_pcResPath;

	protected:

		m4return_t	scanFileURL( m4pcchar_t ai_pccSource );
		m4return_t	composeTargetPath( m4pchar_t aio_pcTarget, m4int_t ai_iLength );
		m4return_t	composeReqHeaders( m4pcchar_t ai_pcFile, m4pchar_t aio_pcHeaders, m4int_t ai_iLength );
		m4return_t	dumpSysLog( DWORD ai_dwError, m4pcchar_t ai_pccFunction );
};

#endif
