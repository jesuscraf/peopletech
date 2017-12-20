//## begin module%3777684803B0.cm preserve=no
//## end module%3777684803B0.cm

//## begin module%3777684803B0.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================
//## end module%3777684803B0.cp

//## Module: windowstsapimpl%3777684803B0; Package body
//## Subsystem: M4Tsap::src::win32%377768480360
//	d:\compon\m4tsap\version\src\win32
//## Source file: d:\compon\m4tsap\version\src\win32\windowstsapimpl.cpp

//## begin module%3777684803B0.additionalIncludes preserve=no
//## end module%3777684803B0.additionalIncludes

//## begin module%3777684803B0.includes preserve=yes
#include "tsap.hpp"
#include <stdio.h>
#include "m4winsock.hpp"
#include "m4log.hpp"
#include "m4srvres.hpp"

#include "windowstsapimpl.hpp"

//## end module%3777684803B0.includes

// m4types
#include <m4types.hpp>
// tsapimpl
#include <tsapimpl.hpp>
//## begin module%3777684803B0.declarations preserve=no
//## end module%3777684803B0.declarations

//## begin module%3777684803B0.additionalDeclarations preserve=yes
//## end module%3777684803B0.additionalDeclarations


// Class ClWindowsTSAPImpl 


ClWindowsTSAPImpl::ClWindowsTSAPImpl ()
  //## begin ClWindowsTSAPImpl::ClWindowsTSAPImpl%310246555.hasinit preserve=no
  //## end ClWindowsTSAPImpl::ClWindowsTSAPImpl%310246555.hasinit
  //## begin ClWindowsTSAPImpl::ClWindowsTSAPImpl%310246555.initialization preserve=yes
  //## end ClWindowsTSAPImpl::ClWindowsTSAPImpl%310246555.initialization
{
  //## begin ClWindowsTSAPImpl::ClWindowsTSAPImpl%310246555.body preserve=yes
	m_iError = M4TSAP_OK;
	m_iSocket = 0;
  //## end ClWindowsTSAPImpl::ClWindowsTSAPImpl%310246555.body
}



//## Other Operations (implementation)
int ClWindowsTSAPImpl::Accept (ClTSAPImpl* ao_poTSAPImpl)
{
  //## begin ClWindowsTSAPImpl::Accept%1350106493.body preserve=yes
	struct sockaddr_in saddr;
	int slen;
	ClWindowsTSAPImpl* ns = (ClWindowsTSAPImpl*) ao_poTSAPImpl;
	
	slen = sizeof( saddr );
	
	ns -> m_iSocket = (int) accept( m_iSocket , (struct sockaddr*) &saddr , &slen );
	if (ns->m_iSocket == INVALID_SOCKET)
	{
		m_iError = M4TSAP_ACCEPT_FAILED;

		return -1;
	}
	//if (saddr.sin_family != AF_INET) {
	//    printf("No es una dirección de la familia AF_INET\n");
	//  }
	ns->m_iPort = saddr.sin_port;
	// XXX MUCHO OJO CON ESTO: mirar maquinas "little endian" y "big endian"
	
	m4uint8_t* addr= ( m4uint8_t*)&saddr.sin_addr.s_addr;
	if ((ns->m_poInetAddress = new ClInetAddress(addr)) == NULL)
	{
		m_iError = M4TSAP_INVALID_INET_ADDRESS;

		return errno;
	}
	
	return (0);
  //## end ClWindowsTSAPImpl::Accept%1350106493.body
}

int ClWindowsTSAPImpl::Bind (ClInetAddress* ai_poInetAddress, int lport)
{
  //## begin ClWindowsTSAPImpl::Bind%1402584086.body preserve=yes
	struct sockaddr_in saddr;
	
	if (ai_poInetAddress == NULL)
		saddr.sin_addr.s_addr = INADDR_ANY;
	else
		memcpy(&saddr.sin_addr.s_addr, (const void*) ai_poInetAddress->GetAddress(),
		ai_poInetAddress->m_s_iAddrLen);
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(lport);
#ifdef BSD
	saddr.sin_len = sizeof(saddr);
#endif
	
	if (::bind(m_iSocket, (struct sockaddr*) &saddr, sizeof(saddr)) == SOCKET_ERROR)
	{
		m_iError = M4TSAP_BIND_FAILED;
		
		return -1;
	}
	
	
	return(0);
  //## end ClWindowsTSAPImpl::Bind%1402584086.body
}

void ClWindowsTSAPImpl::Close ()
{
  //## begin ClWindowsTSAPImpl::Close%1860826927.body preserve=yes
	closesocket(m_iSocket);
  //## end ClWindowsTSAPImpl::Close%1860826927.body
}

int ClWindowsTSAPImpl::Connect (const char* ai_pcHostname, int ai_iPort)
{
  //## begin ClWindowsTSAPImpl::Connect%-1851496694.body preserve=yes
	struct sockaddr_in saddr;
	struct hostent* hp;
	
	hp = gethostbyname(ai_pcHostname);
	if (hp == NULL)
	{
		m_iError = M4TSAP_INVALID_HOSTNAME;

		return WSAGetLastError();
	}
	
	memcpy(&saddr.sin_addr.s_addr, hp->h_addr, hp->h_length);
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(ai_iPort);
#ifdef BSD
	saddr.sin_len = sizeof(saddr);
#endif
	if (connect(m_iSocket, (struct sockaddr*) &saddr, sizeof(saddr)) < 0)
	{
		m_iError = M4TSAP_CONNECT_FAILED;
	
		return WSAGetLastError();
	}
	
	return(0);
  //## end ClWindowsTSAPImpl::Connect%-1851496694.body
}

int ClWindowsTSAPImpl::Connect (ClInetAddress* ai_InetAddress, int ai_iPort)
{
  //## begin ClWindowsTSAPImpl::Connect%-1754837781.body preserve=yes
	struct sockaddr_in saddr;
	
	memcpy(&saddr.sin_addr.s_addr, (const void*) ai_InetAddress->GetAddress(),
		ai_InetAddress->m_s_iAddrLen);
	//iAddrLen);
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(ai_iPort);
#ifdef BSD
	saddr.sin_len = sizeof(saddr);
#endif
	if (connect(m_iSocket, (struct sockaddr*) &saddr, sizeof(saddr)) == SOCKET_ERROR)
	{
		m_iError = M4TSAP_CONNECT_FAILED;

		// Bg 0132613. Se saca un mensaje de error en la consola con el mensaje del sistema (m4sdown.exe)
		int		iError = WSAGetLastError() ;

		DWORD	dwResult = 0 ;
		LPVOID	pvBuffer = NULL ;

		dwResult = FormatMessage
			( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				iError,
				0,
				(LPSTR) &pvBuffer,
				0,
				NULL
			) ;

		if( dwResult > 0 && pvBuffer != NULL )
		{
			if( dwResult >= 2 && ((m4pchar_t) pvBuffer)[ dwResult - 2 ] == '\r' )
			{
				((m4pchar_t) pvBuffer)[ dwResult - 2 ] = '\0' ;
			}
			printf("Error connecting to port %ld. Error %d: %s\n", ai_iPort, iError, (m4pchar_t) pvBuffer );

			LocalFree( pvBuffer ) ;
		}
	
		return iError;
	}
	
	return 0;
  //## end ClWindowsTSAPImpl::Connect%-1754837781.body
}

int ClWindowsTSAPImpl::Create (m4bool_t ai_bStream)
{
  //## begin ClWindowsTSAPImpl::Create%2049243730.body preserve=yes
	if ((m_iSocket = (int) socket(AF_INET, ai_bStream ? SOCK_STREAM : SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		/* Bug 0113374
		Se saca un mensaje de error con el mensaje del sistema
		*/
		int		iError = WSAGetLastError() ;
		DWORD	dwResult = 0 ;
		LPVOID	pvBuffer = NULL ;

		dwResult = FormatMessage
			( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				iError,
				0,
				(LPSTR) &pvBuffer,
				0,
				NULL
			) ;

		if( dwResult > 0 && pvBuffer != NULL )
		{
			if( dwResult >= 2 && ((m4pchar_t) pvBuffer)[ dwResult - 2 ] == '\r' )
			{
				((m4pchar_t) pvBuffer)[ dwResult - 2 ] = '\0' ;
			}
			SETCODEF( M4_ERR_TSAP_NO_OPEN_SOCKECT, ERRORLOG, "#*S1*#%s#", (m4pchar_t) pvBuffer ) ;
			LocalFree( pvBuffer ) ;
		}

		m_iError = M4TSAP_SOCKET_FAILED;
		return iError;
	}
	
	return(0);
  //## end ClWindowsTSAPImpl::Create%2049243730.body
}

int ClWindowsTSAPImpl::Listen (int ai_bBacklog)
{
  //## begin ClWindowsTSAPImpl::Listen%1786549073.body preserve=yes
	if (listen(m_iSocket, ai_bBacklog) == SOCKET_ERROR)
	{
		m_iError = M4TSAP_LISTEN_FAILED;
	
		return -1;
	}
	return(0);
  //## end ClWindowsTSAPImpl::Listen%1786549073.body
}

int ClWindowsTSAPImpl::Write (const void* src, int len)
{
  //## begin ClWindowsTSAPImpl::Write%-1549279123.body preserve=yes
	int rc;
	
	
	rc = send ( m_iSocket  , (const char * )src , len , 0 );   // ( Especifico Windows )
	// Si tenemos salida no bloqueante y el buffer del sistema esta lleno, la
	// llamada a write devuelve -1. Sin embargo, esto en realidad no es un
	// error, sino una situacion normal, por lo que ocultamos
	// este "pseudo-error".
	if (rc == SOCKET_ERROR  && WSAGetLastError()  == WSAEWOULDBLOCK)
		return 0;
	
	if (rc == SOCKET_ERROR  && WSAGetLastError()  == WSAENOBUFS)
		return -2;

	if (rc == -1)
		m_iError = (m_iError == M4TSAP_OK)?M4TSAP_WRITE_FAILED:m_iError;
	
	
	return rc;
  //## end ClWindowsTSAPImpl::Write%-1549279123.body
}

int ClWindowsTSAPImpl::Read (void* dst, int len)
{
  //## begin ClWindowsTSAPImpl::Read%1555076021.body preserve=yes
	int rc;
	
	rc = recv(m_iSocket, (char *)dst, len , 0 );
	// Ver comentario de WindowsTSAPImpl::Write
	if (rc == SOCKET_ERROR  && WSAGetLastError()  == WSAEWOULDBLOCK)
		rc = 0;
	
	if (rc == -1)
		m_iError = (m_iError == M4TSAP_OK)?M4TSAP_READ_FAILED:m_iError;

	return rc;
  //## end ClWindowsTSAPImpl::Read%1555076021.body
}

int ClWindowsTSAPImpl::SetBlockingIO (m4bool_t ai_bBlocking)
{
  //## begin ClWindowsTSAPImpl::SetBlockingIO%1696209715.body preserve=yes
	unsigned long flags;
	
	if (ai_bBlocking)
		flags = 0L;
	else
		flags = 1L;
	
	if (ioctlsocket(m_iSocket, FIONBIO, &flags) != 0)
		return WSAGetLastError();
	
	return 0;
  //## end ClWindowsTSAPImpl::SetBlockingIO%1696209715.body
}

// Additional Declarations
  //## begin ClWindowsTSAPImpl%3777684501AD.declarations preserve=yes
  //## end ClWindowsTSAPImpl%3777684501AD.declarations

//## begin module%3777684803B0.epilog preserve=yes
//## end module%3777684803B0.epilog
