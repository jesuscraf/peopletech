//==============================================================================
//
// (c) Copyright  1991-2015 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           m4doc 
// File:             m4filedownldr.cpp
// Project:          Document Management
// Author:           Meta Software M.S. , S.A
// Date:             04-March-2015
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
//
// Definition:
//		Class M4FileDownloader implementation
//==============================================================================

#include "m4stl.hpp"
#include "m4filedownldr.hpp"
#include "m4docerrors.hpp"
#include "m4objlog.hpp"
#include <stdio.h>


#define CHECK_SYS_ERROR( COND, ERR, FUNC )	do { if( COND ) { dumpSysLog( ERR, FUNC ); return( M4_ERROR ); } } while( 0 )
#define GEN_BUFFER_SIZE		128
#define INET_BUFFER_SIZE	1024


M4FileDownloader::M4FileDownloader()
{
#ifdef _WINDOWS

	m_hInstance		= NULL;
	m_hConnect		= NULL;
	m_hRequest		= NULL;
	m_bSSL			= M4_FALSE;
	m_pcServer		= NULL;
	m_iPort			= -1;
	m_pcResName		= NULL;
	m_pcResPath		= NULL;

	memset( m_acURL, 0, INTERNET_MAX_URL_LENGTH + 1 );

#endif
}


M4FileDownloader::~M4FileDownloader()
{
#ifdef _WINDOWS

	if( m_hRequest != NULL )
	{
		 InternetCloseHandle( m_hRequest );
	}

	if( m_hConnect != NULL )
	{
		 InternetCloseHandle( m_hConnect );
	}

	if( m_hInstance != NULL )
	{
		 InternetCloseHandle( m_hInstance );
	}

#endif
}


m4return_t M4FileDownloader::DownloadFile( m4pcchar_t ai_pccSource, m4pchar_t aio_pcTarget, m4int_t ai_iLength )
{
#ifdef _WINDOWS

	m4return_t			iRet;
	size_t				iLen;
	DWORD				dwReqOptions;
	char				acBuffer[ GEN_BUFFER_SIZE + 1 ];
	DWORD				dwFlags, dwSize, dwStatus;
	BOOL				bRet;
	fstream				oOutStream;
	INTERNET_BUFFERS	stInetBuffer;
	char				acReadBuffer[ INET_BUFFER_SIZE + 1 ];
	m4bool_t			bFullyReaded = M4_FALSE;
	DWORD				dwReaded = 0;
	char*				pcStatusCodeDesc = NULL;
	bool				bDelete = false;
	

	// Checks output buffer.
	CHECK_CHLOG_ERROR( aio_pcTarget == NULL || ai_iLength < 1, M4_ERROR, M4DOCERROR_DWNLD_BAD_ARGS );


	// It is already a local file...
	if( strnicmp( ai_pccSource, "http", 4 ) )
	{
		iLen = strlen( ai_pccSource );
		CHECK_CHLOG_ERRORF( iLen > ai_iLength, M4_ERROR, M4DOCERROR_DWNLD_INSUFF_BUFFER, ai_pccSource );

		// ... so returns the path itself.
		strncpy( aio_pcTarget, ai_pccSource, ai_iLength );
		aio_pcTarget[ ai_iLength - 1 ] = '\0';

		return M4_SUCCESS;
	}


	// Extracts the URL components.
	iRet = scanFileURL( ai_pccSource );
	CHECK_CHLOG_ERRORF( iRet != M4_SUCCESS, M4_ERROR, M4DOCERROR_DWNLD_BAD_URL, ai_pccSource );


	// Builds the target path.
	if( *aio_pcTarget == NULL )
	{
		iRet = composeTargetPath( aio_pcTarget, ai_iLength );
		CHECK_CHLOG_ERROR( iRet != M4_SUCCESS, M4_ERROR, M4DOCERROR_DWNLD_BUILD_TGT_PATH );
	}

	
	// Connects to web server.
	m_hInstance = InternetOpen( "synchttp", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
	CHECK_SYS_ERROR( m_hInstance == NULL, GetLastError(), "InternetOpen" );

	m_hConnect = InternetConnect( m_hInstance, m_pcServer, m_iPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, INTERNET_NO_CALLBACK );
	CHECK_SYS_ERROR( m_hConnect == NULL, GetLastError(), "InternetConnect" );

	
	// Opens a request.
	dwReqOptions = INTERNET_FLAG_RESYNCHRONIZE;

	if( m_bSSL == M4_TRUE )
	{
		dwReqOptions |= INTERNET_FLAG_SECURE;
	}

    m_hRequest = HttpOpenRequest( m_hConnect, "GET", m_pcResPath, NULL, NULL, NULL, dwReqOptions, INTERNET_NO_CALLBACK);
	CHECK_SYS_ERROR( m_hRequest == NULL, GetLastError(), "HttpOpenRequest" );


	// Sets the If-Modified-Since header.
	iRet = composeReqHeaders( aio_pcTarget, acBuffer, GEN_BUFFER_SIZE );

	if( iRet == M4_SUCCESS && *acBuffer != '\0' )
	{
		HttpAddRequestHeaders( m_hRequest, acBuffer, -1, HTTP_ADDREQ_FLAG_ADD );
	}


	// Sets the SSL request options.
	if( m_bSSL == M4_TRUE )
	{
		dwSize = sizeof( dwFlags );

		bRet = InternetQueryOption( m_hRequest, INTERNET_OPTION_SECURITY_FLAGS, (LPVOID)&dwFlags, &dwSize );
		CHECK_SYS_ERROR( bRet == FALSE, GetLastError(), "InternetQueryOption" );

		dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | SECURITY_FLAG_IGNORE_CERT_DATE_INVALID;

		dwSize = sizeof( dwFlags );

		bRet = InternetSetOption( m_hRequest, INTERNET_OPTION_SECURITY_FLAGS, &dwFlags, dwSize );
		CHECK_SYS_ERROR( bRet == FALSE, GetLastError(), "InternetSetOption" );
	}

	
	// Sends the request.
	bRet = HttpSendRequest( m_hRequest, NULL, 0, NULL, 0 );
	CHECK_SYS_ERROR( bRet == FALSE, GetLastError(), "HttpSendRequest" );

    
	// Checks the response.
	memset( acBuffer, 0, GEN_BUFFER_SIZE + 1 );
	dwSize = GEN_BUFFER_SIZE;

	bRet = HttpQueryInfo( m_hRequest, HTTP_QUERY_STATUS_CODE, acBuffer, &dwSize, NULL );
	CHECK_SYS_ERROR( bRet == FALSE, GetLastError(), "HttpQueryInfo" );
	
	dwStatus = atoi( acBuffer );

	if( dwStatus == HTTP_STATUS_NOT_MODIFIED )
	{
		return M4_SUCCESS;
	}

	// bg 296458
	if( dwStatus != HTTP_STATUS_OK )
	{
		*acBuffer = '\0';
		dwSize = GEN_BUFFER_SIZE;
		pcStatusCodeDesc = acBuffer;

		bRet = HttpQueryInfo( m_hRequest, HTTP_QUERY_STATUS_TEXT, pcStatusCodeDesc, &dwSize, NULL );

		if( bRet == FALSE )
		{
			if( GetLastError() == ERROR_INSUFFICIENT_BUFFER )
			{
				bDelete = true;
				pcStatusCodeDesc = new char[ dwSize + 1 ];
				*pcStatusCodeDesc = '\0';
				HttpQueryInfo( m_hRequest, HTTP_QUERY_STATUS_TEXT, pcStatusCodeDesc, &dwSize, NULL );
			}
		}

		DUMP_CHLOG_ERRORF( M4DOCERROR_DWNLD_NOT_SUCCESS, ai_pccSource << dwStatus << pcStatusCodeDesc );

		if( bDelete == true )
		{
			delete pcStatusCodeDesc;
		}

		return M4_ERROR;
	}
	
	// Dumps to the output file.
	oOutStream.open( aio_pcTarget, ios::out | ios::binary );

	bRet = oOutStream.is_open();
	CHECK_CHLOG_ERRORF( bRet == false, M4_ERROR, M4DOCERROR_DWNLD_OPEN_FILE_ERROR, strerror(errno) );

    do
    {
        FillMemory( &stInetBuffer, sizeof( stInetBuffer ), 0 );
        stInetBuffer.dwStructSize	= sizeof( stInetBuffer );
        stInetBuffer.lpvBuffer		= acReadBuffer;
        stInetBuffer.dwBufferLength	= INET_BUFFER_SIZE;

		bRet = InternetReadFileEx( m_hRequest, &stInetBuffer, 0, INTERNET_NO_CALLBACK );
        CHECK_SYS_ERROR( bRet == FALSE, GetLastError(), "InternetReadFileEx" );

        if( stInetBuffer.dwBufferLength == 0 )
		{
            bFullyReaded = M4_TRUE;
		}
		else
		{
			dwReaded += stInetBuffer.dwBufferLength;
			oOutStream.write( ( const char * ) stInetBuffer.lpvBuffer, stInetBuffer.dwBufferLength );
		}
    } 
	while( bFullyReaded == M4_FALSE );

	oOutStream.close();
	

	// Checks for downloaded empty file.
	CHECK_CHLOG_ERRORF( dwReaded == 0, M4_ERROR, M4DOCERROR_DWNLD_EMPTY_FILE, ai_pccSource );


	return M4_SUCCESS;

#else

	return M4_ERROR;

#endif
}


m4return_t M4FileDownloader::scanFileURL( m4pcchar_t ai_pccSource )
{
#ifdef _WINDOWS
	
	m4pchar_t	pcPos1, pcPos2;
	m4char_t	cCopy;


	// Working copy.
	strncpy( m_acURL, ai_pccSource, INTERNET_MAX_URL_LENGTH );


	// -- SSL.
	if( !strnicmp( m_acURL, "https", 5 ) )
	{
		m_bSSL = M4_TRUE;
	}
	else
	{
		m_bSSL = M4_FALSE;
	}
	

	// -- Server.
	pcPos1 = strstr( m_acURL, "://" );

	if( pcPos1 == NULL )
	{
		return M4_ERROR;
	}

	pcPos1 += 3;
	pcPos2 = strstr( pcPos1, ":" );

	if( pcPos2 == NULL )
	{
		pcPos2 = strstr( pcPos1, "/" );
		
		if( pcPos2 == NULL )
		{
			return M4_ERROR;
		}
	}

	m_pcServer = pcPos1;
	cCopy = *pcPos2;
	*pcPos2 = '\0';
	

	// -- Port.
	if( cCopy == ':' )
	{
		pcPos2++;
		pcPos1 = strstr( pcPos2, "/" );

		if( pcPos1 == NULL )
		{
			return M4_ERROR;
		}

		*pcPos1 = '\0';
		m_iPort = atoi( pcPos2 );
		pcPos1++;
	}
	else
	{
		if( m_bSSL == M4_TRUE )
		{
			m_iPort = 443;
		}
		else
		{
			m_iPort = 80;
		}
		
		pcPos1 = ++pcPos2;
	}


	// -- URL.
	m_pcResPath = pcPos1;


	// -- File.
	pcPos1 = strrchr( m_pcResPath, '/' );

	if( pcPos1 == NULL || *(pcPos1 + 1) == NULL )
	{
		return M4_ERROR;
	}

	m_pcResName = pcPos1 + 1;


	return M4_SUCCESS;

#else

	return M4_ERROR;

#endif
}


m4return_t M4FileDownloader::composeTargetPath( m4pchar_t aio_pcTarget, m4int_t ai_iLength )
{
#ifdef _WINDOWS

	const char*	pccTempDir;
	size_t		iReqSize;


	pccTempDir = getenv( "TEMP" );
	
	if( pccTempDir == NULL )
	{
		return M4_ERROR;
	}

	// Checks for enough buffer.
	iReqSize = strlen( pccTempDir ) + strlen( m_pcResName ) + 2;	// slash char + '\0'.

	if( iReqSize > ai_iLength  )
	{
		return M4_ERROR;
	}

	sprintf( aio_pcTarget, "%s/%s", pccTempDir, m_pcResName );


	return M4_SUCCESS;

#else

	return M4_ERROR;

#endif
}


m4return_t M4FileDownloader::composeReqHeaders( m4pcchar_t ai_pcFile, m4pchar_t aio_pcHeaders, m4int_t ai_iLength )
{
#ifdef _WINDOWS

	HANDLE		hFile = INVALID_HANDLE_VALUE;
	FILETIME	stFTWrite;
	SYSTEMTIME	stSTUTC;
	char		acInetTime[ INTERNET_RFC1123_BUFSIZE ];
	const char*	pcIfModifiedSinceHeader = "If-Modified-Since: ";


	memset( aio_pcHeaders, 0, ai_iLength + 1 );

	// If exists a local copy of the fle, gets the last monification date.
	hFile = CreateFile( ai_pcFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if( hFile != INVALID_HANDLE_VALUE )
	{
		if( GetFileTime( hFile, NULL, NULL, &stFTWrite ) == TRUE )
		{
			if( FileTimeToSystemTime( &stFTWrite, &stSTUTC ) == TRUE )
			{
				if( InternetTimeFromSystemTime( &stSTUTC, INTERNET_RFC1123_FORMAT, acInetTime, INTERNET_RFC1123_BUFSIZE ) == TRUE )
				{
					if( strlen( pcIfModifiedSinceHeader ) + strlen( acInetTime ) <= ai_iLength )
					{
						sprintf( aio_pcHeaders, "%s%s", pcIfModifiedSinceHeader, acInetTime );
					}
				}
			}
		}

		CloseHandle( hFile );
	}


	return M4_SUCCESS;

#else

	return M4_ERROR;

#endif
}


m4return_t M4FileDownloader::dumpSysLog( DWORD ai_dwError, m4pcchar_t ai_pccFunction )
{
#ifdef _WINDOWS

	int		iLength;
	char*	lpMsgBuf = 0;


	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
					0, ai_dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPTSTR>(&lpMsgBuf), 0, 0 );
	
	if( lpMsgBuf == 0 )
    {
		lpMsgBuf = "Unexpected error";
    }
	else
    {
		iLength = strlen( lpMsgBuf );

		if( iLength > 2 && lpMsgBuf[ iLength - 2 ] == '\r' )
		{
			lpMsgBuf[ iLength - 2 ] = 0;
		}

		DUMP_CHLOG_ERRORF( M4DOCERROR_DWNLD_SYS_ERROR, ai_dwError << lpMsgBuf << ai_pccFunction );

		LocalFree( lpMsgBuf );
	}


	return M4_SUCCESS;

#else

	return M4_ERROR;

#endif
}

