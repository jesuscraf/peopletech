//## begin module%377768480229.cm preserve=no
//## end module%377768480229.cm

//## begin module%377768480229.cp preserve=no
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
//## end module%377768480229.cp

//## Module: ssltsapimpl%377768480229; Package body
//## Subsystem: M4Tsap::src%3777684800C1
//	d:\compon\m4tsap\version\src
//## Source file: d:\compon\m4tsap\version\src\ssltsapimpl.cpp

//## begin module%377768480229.additionalIncludes preserve=no
//## end module%377768480229.additionalIncludes

//## begin module%377768480229.includes preserve=yes

#include <stdio.h>
#include <m4winsock.hpp>
#include "m4trace.hpp"
#include "m4log.hpp"
#include "m4srvres.hpp"

#ifdef _HP
#define _NO-USE_SSL
#endif

#ifdef _UNIX
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#ifdef _SOLARIS
#include "moveupfds.hpp"
#endif

#endif


#include "ssltsapimpl.hpp"
#include <syncro.hpp>
//## end module%377768480229.includes

// m4types
#include <m4types.hpp>
// tsapimpl
#include <tsapimpl.hpp>
// tsap
#include <tsap.hpp>
// ssl
//#include <ssl.h>
//## begin module%377768480229.declarations preserve=no
//## end module%377768480229.declarations

//## begin module%377768480229.additionalDeclarations preserve=yes
#ifdef _SOLARIS
static ClMutex g_sMutex[CRYPTO_NUM_LOCKS];

extern "C" void solaris_locking_callback(int mode, int type, const char* file, int line)
{
	if (mode & CRYPTO_LOCK)
		g_sMutex[type].Lock();
	else
		g_sMutex[type].Unlock();
}

#endif // _SOLARIS

void	InitUnlock();
void	InitLock();

#ifndef _NO_USE_SSL
m4bool_t	bSSLClientInitialized = M4_FALSE;
m4bool_t	bSSLServerInitialized = M4_FALSE;

extern "C" int  verify_callback(int ok, X509_STORE_CTX *ctx)
{
	return ok;
}
static SSL_CTX	*s_CtxClient	= NULL;
static SSL_CTX	*s_CtxServer	= NULL;
static int		s_ModeMeta4SSL	= 0;

SSL_CTX	*GetCTXClient()
{
	return s_CtxClient;
}

SSL_CTX	*GetCTXServer()
{
	return s_CtxServer;
}

int GetModeMeta4SSL()
{
	return	s_ModeMeta4SSL;
}


int FuncionPasswd(char *buf,int size,int verify)
{
  char mi_key[] = "jrm1jrm";
  int  mi_keylength = (int) strlen(mi_key);

  if (size < mi_keylength) // error of some type
	  return -1;

 strcpy(buf, mi_key);

  return(mi_keylength);
}


int SSL_Initialize(int ai_iModeSSL, void *ai_pData)
{
#ifdef _SOLARIS
	m4return_t iRet;
	for(int i=0;i<CRYPTO_NUM_LOCKS;i++)
		iRet=g_sMutex[i].Init();

	CRYPTO_set_locking_callback(solaris_locking_callback);
#endif	 // SOLARIS

	typedef struct
	{
		char	*PrivateKeyFile;
		char	*CertificateFile;

	}SSL_Data;

	int	ret;

	s_ModeMeta4SSL = ai_iModeSSL;

	if (bSSLClientInitialized == M4_FALSE &&bSSLServerInitialized == M4_FALSE)
	{
		SSL_load_error_strings();
		SSLeay_add_ssl_algorithms();
	}

	switch (ai_iModeSSL)
	{
		case M4_SSL_CLIENT_CERTIFICATE:

			if (s_CtxClient == NULL)
			{
				// Bug 0287327 Se usa la versión TLS1.2 de protocolo
				s_CtxClient = SSL_CTX_new (TLSv1_2_client_method());
			}
			if (s_CtxClient == NULL)
			{
				m4Trace(fprintf(stderr, "Error en SSL_CTX_new (TLSv1_2_client_method())"));
				return -1;
			}

			if (!SSL_CTX_load_verify_locations(s_CtxClient, ((SSL_Data *)ai_pData)->PrivateKeyFile, ((SSL_Data *)ai_pData)->CertificateFile))
			{
				m4Trace(fprintf(stderr, "Error en SSL_CTX_load_verify_locations"));
				return -1;
			}

			if (!SSL_CTX_set_default_verify_paths(s_CtxClient))			
			{
				m4Trace(fprintf(stderr, "Error en SSL_CTX_set_default_verify_paths"));
				return -1;
			}

			SSL_CTX_set_verify(s_CtxClient, SSL_VERIFY_PEER, verify_callback);
			break;

		case M4_SSL_SERVER:

			if (s_CtxServer == NULL)
			{
				// Bug 0287327 Se usa la versión TLS1.2 de protocolo
				s_CtxServer = SSL_CTX_new (TLSv1_2_server_method());
			}

			if (s_CtxServer == NULL)
			{
				m4Trace(fprintf(stderr, "Error en SSL_CTX_new (TLSv1_2_server_method())"));
				return -1;
			}

			/* Se almacena la clave privada del servidor en el contexto */
			ret = SSL_CTX_use_RSAPrivateKey_file (s_CtxServer, ((SSL_Data *)ai_pData)->PrivateKeyFile,  SSL_FILETYPE_PEM);
			if (ret != 1)
			{
				m4Trace(fprintf(stderr, "Error en SSL_CTX_use_RSAPrivateKey_file"));
				return -1;
			}

			/* Se almacena el certificado del servidor en el contexto */
			ret = SSL_CTX_use_certificate_file (s_CtxServer, ((SSL_Data *)ai_pData)->CertificateFile, SSL_FILETYPE_PEM);
			if (ret != 1)
			{
				m4Trace(fprintf(stderr, "Error en SSL_CTX_use_certificate_file"));
				return -1;
			}
			break;

		case M4_SSL_CLIENT:

			// Este no se usa
			if (s_CtxClient == NULL)
			{
				// Bug 0287327 Se usa la versión TLS1.2 de protocolo
				s_CtxClient = SSL_CTX_new (TLSv1_2_client_method());
			}
			if (s_CtxClient == NULL)
			{
				m4Trace(fprintf(stderr, "Error en SSL_CTX_new (TLSv1_2_client_method())"));
				return -1;
			}
			break;

		case M4_SSL_CLIENTSERVER:

			// Este no se usa
			m4Trace(fprintf(stderr, "Opción M4_SSL_CLIENTSERVER no implementada"));
			return -1;

		case M4_SSL_CLIENTSERVER_CERTIFICATE:

			// Este no se usa
			m4Trace(fprintf(stderr, "Opción M4_SSL_CLIENTSERVER_CERTIFICATE no implementada"));
			return -1;

		default:

			m4Trace(fprintf(stderr, "Error de parámetros, modo desconocido"));
			return -1;
	}

	return 0;
}
#endif  //De _NO_USE_SSL



//## end module%377768480229.additionalDeclarations


// Class ClSSLTSAPImpl 





ClSSLTSAPImpl::ClSSLTSAPImpl (int ai_iMode, void* ai_pData)
  //## begin ClSSLTSAPImpl::ClSSLTSAPImpl%1820327814.hasinit preserve=no
  //## end ClSSLTSAPImpl::ClSSLTSAPImpl%1820327814.hasinit
  //## begin ClSSLTSAPImpl::ClSSLTSAPImpl%1820327814.initialization preserve=yes
  //## end ClSSLTSAPImpl::ClSSLTSAPImpl%1820327814.initialization
{
  //## begin ClSSLTSAPImpl::ClSSLTSAPImpl%1820327814.body preserve=yes
	m_ssl = NULL;
	m_iMode = ai_iMode;
	m_pData = ai_pData;
	m_iError = M4TSAP_OK;
	m_iCeroCounter=0;
  //## end ClSSLTSAPImpl::ClSSLTSAPImpl%1820327814.body
}

ClSSLTSAPImpl::~ClSSLTSAPImpl ( )
{
	/*
      if ( m_pData )
		  delete ((char *)m_pData );
*/

      if ( m_ssl )
		SSL_free ( m_ssl ) ;
}


//## Other Operations (implementation)
int ClSSLTSAPImpl::Accept (ClTSAPImpl* ao_poTSAPImpl)
{
  //## begin ClSSLTSAPImpl::Accept%-1529727978.body preserve=yes
#ifndef _NO_USE_SSL
	struct sockaddr_in saddr;
#ifdef _AIX
	size_t slen;
#else
	int slen;
#endif
	ClSSLTSAPImpl* ns = (ClSSLTSAPImpl*) ao_poTSAPImpl;

	slen = sizeof( saddr );
	
	ns->m_iSocket = (int) accept( m_iSocket , (struct sockaddr*) &saddr , &slen );
#ifndef _UNIX
	if (ns->m_iSocket == INVALID_SOCKET)
#else
	if (ns->m_iSocket == -1)
#endif
	{
		m_iError = M4TSAP_ACCEPT_FAILED;
		
		return -1;
	}
#ifdef _SOLARIS
	//MOD JOSEAVM 29-6-2001
	//ClMoveUpFds::MoveUp(ns->m_iSocket);
	int iPrevSock=ns->m_iSocket;
	ns->m_iSocket=fcntl( ns->m_iSocket, F_DUPFD, DELTA_FDS );
	if (ns->m_iSocket <0)
	{
		m_iError = M4TSAP_ACCEPT_FAILED;
		return -1;
	}
	else
		close(iPrevSock);
	// END MOD
#endif
//if (saddr.sin_family != AF_INET) {
//    printf("No es una dirección de la familia AF_INET\n");
//  }
	ns->m_iPort = saddr.sin_port;
  // XXX MUCHO OJO CON ESTO: mirar maquinas "little endian" y "big endian"

#ifndef _UNIX
	m4uint8_t* addr= ( m4uint8_t*)&saddr.sin_addr.s_addr;
	if ((ns->m_poInetAddress = new ClInetAddress(addr)) == NULL)
	{
		closesocket(ns->m_iSocket);
		m_iError = M4TSAP_INVALID_INET_ADDRESS;
		
		return -1;
	}
#else
	if ((ns->m_poInetAddress = new ClInetAddress((const unsigned char*) &saddr.sin_addr.s_addr)) == NULL)
	{
		close(ns->m_iSocket);
		m_iError = M4TSAP_INVALID_INET_ADDRESS;
		return -1;
	}
#endif
	if (GetCTXServer() == NULL)
	{
#ifndef _UNIX
		closesocket(ns->m_iSocket);
#else
		close(ns->m_iSocket);
#endif
		m_iError = M4TSAP_SSL_SERVER_NOT_INITIALIZED;
		m4Trace(fprintf(stderr, "Error en Accept, ctx es null"));
		
		return -1;
	};
	
	ns->m_ssl = SSL_new (GetCTXServer());
	if (ns->m_ssl == NULL)
	{
#ifndef _UNIX
		closesocket(ns->m_iSocket);
#else

#ifdef _SOLARIS
		//MOD JOSEAVM 29-6-2001
		//ClMoveUpFds::Clean(ns->m_iSocket);
		//END
#endif
		close(ns->m_iSocket);
#endif
		m4Trace(fprintf(stderr, "Error en Accept, ssl es null"));
		m_iError = M4TSAP_SSL_SSL_NEW_FAILED;
		
		
		return -1;
	};
	SSL_clear(ns->m_ssl);
	SSL_set_fd (ns->m_ssl, ns->m_iSocket);
	
	/* Se acepta la conexion SSL (equivalente al "accept" del socket) */
	m4bool_t bErrorCipher = M4_FALSE;
	
	if (ns->m_iMode == M4_SSL_SET_PREFERED_CIPHER)
		if (!SSL_set_cipher_list(ns->m_ssl, (char *)ns->m_pData))
		{
			m4Trace(fprintf(stderr, "Error en SSL_set_cipher_list %s\n",
				(char *)m_pData));
			
			bErrorCipher = M4_TRUE;
			m_iError = M4TSAP_SSL_SET_CIPHER_LIST_FAILED;
			
			return -1;
		};
	if (bErrorCipher || SSL_accept(ns->m_ssl) <= 0)
	{
#ifndef _UNIX
		closesocket(ns->m_iSocket);
#else

#ifdef _SOLARIS
		//MOD JOSEAVM 29-6-2001
		//ClMoveUpFds::Clean(ns->m_iSocket);
		//END MOD
#endif
		close(ns->m_iSocket);
#endif
		m4Trace(fprintf(stderr, "Error en SSL_accept"));
		m_iError = M4TSAP_SSL_ACCEPT_FAILED;
		
		
		return -1;
	};
	
	
#endif
	return (0);
  //## end ClSSLTSAPImpl::Accept%-1529727978.body
}

int ClSSLTSAPImpl::Bind (ClInetAddress* ai_poInetAddress, int lport)
{
  //## begin ClSSLTSAPImpl::Bind%1916733128.body preserve=yes
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

#ifndef _UNIX
	if (::bind(m_iSocket, (struct sockaddr*) &saddr, sizeof(saddr)) == SOCKET_ERROR)
	{
		m_iError = M4TSAP_BIND_FAILED;

		return -1;
	}
	
#else
	if (bind(m_iSocket, (struct sockaddr*) &saddr, sizeof(saddr)) < 0)
	{
		m_iError = M4TSAP_BIND_FAILED;

		return -1;
	}
	
#endif
	
	return 0;
  //## end ClSSLTSAPImpl::Bind%1916733128.body
}

void ClSSLTSAPImpl::Close ()
{
  //## begin ClSSLTSAPImpl::Close%-1694688536.body preserve=yes
#ifndef _NO_USE_SSL
	if (m_ssl != NULL)
		SSL_shutdown(m_ssl);
#ifndef _UNIX
	closesocket(m_iSocket);
#else
#ifdef _SOLARIS
	//MOD JOSEAVM 29-6-2001
	//ClMoveUpFds::Clean(m_iSocket);
	//END MOD
#endif
	close(m_iSocket);
#endif
	if (m_ssl != NULL)
	{
		SSL_free(m_ssl);
		m_ssl = NULL ;
	}
#endif
//## end ClSSLTSAPImpl::Close%-1694688536.body
}

int ClSSLTSAPImpl::Connect (const char* ai_pcHostname, int ai_iPort)
{
  //## begin ClSSLTSAPImpl::Connect%-1656302624.body preserve=yes
#ifndef _NO_USE_SSL
	struct sockaddr_in saddr;
	X509*    server_cert;
	char*    str;
	char	buf[500];
	struct hostent* hp;
	
	hp = gethostbyname(ai_pcHostname);
	
	if (hp == NULL)
	{
		m_iError = M4TSAP_INVALID_HOSTNAME;
#ifndef _UNIX
		return WSAGetLastError();
#else
		return -1;
#endif
	}
	memcpy(&saddr.sin_addr.s_addr, hp->h_addr, hp->h_length);
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(ai_iPort);
#ifdef BSD
	saddr.sin_len = sizeof(saddr);
#endif
	if (connect(m_iSocket, (struct sockaddr*) &saddr, sizeof(saddr)) < 0)
#ifndef _UNIX
	{
		m4Trace(fprintf(stderr, "Error en connect"));
		m_iError = M4TSAP_CONNECT_FAILED;
		
		return WSAGetLastError();
	}
#else
	{
		m_iError = M4TSAP_CONNECT_FAILED;
		m4Trace(fprintf(stderr, "Error en connect"));
		
		return -1;
	}
#endif
	
	if (GetCTXClient() == NULL)
	{
		m4Trace(fprintf(stderr, "Error en Connect, ctx es null"));
		m_iError = M4TSAP_SSL_CLIENT_NOT_INITIALIZED;
		
		return -1;
	};
	
	m_ssl = SSL_new (GetCTXClient());
	if (m_ssl == NULL)
	{
		m4Trace(fprintf(stderr, "Error en Connect, SSL_new retorna null"));
		m_iError = M4TSAP_SSL_SSL_NEW_FAILED;
		
		return -1;
	};
	

	if (m_iMode == M4_SSL_SET_PREFERED_CIPHER)
		if (!SSL_set_cipher_list(m_ssl, (char *)m_pData))
		{
			m_iError = M4TSAP_SSL_SET_CIPHER_LIST_FAILED;
			m4Trace(fprintf(stderr, "Error en SSL_set_cipher_list %s\n",
						(char *)m_pData));
			
			return -1;
		};

	
	/* Se asocia la conexion SSL con el socket (TCP) */
	SSL_set_fd (m_ssl, m_iSocket);
	if (SSL_connect(m_ssl) == -1)
	{
		m4Trace(fprintf(stderr, "Error en SSL_connect"));
		m_iError = M4TSAP_SSL_CONNECT_FAILED;
		
		return -1;
	};
	if (GetModeMeta4SSL() == M4_SSL_CLIENT_CERTIFICATE ||
		GetModeMeta4SSL() == M4_SSL_CLIENTSERVER_CERTIFICATE)
	{
		server_cert = SSL_get_peer_certificate (m_ssl);
		if (server_cert == NULL)
		{
			m4Trace(fprintf(stderr, "Error en SSL_get_peer_certificate"));
			m_iError = M4TSAP_SSL_GET_PEER_CERTIFICATE_FAILED;
			
			return -1;
		};
		
		str = X509_NAME_oneline(X509_get_subject_name(server_cert), buf, 256);
		if (str == NULL)
		{
			m4Trace(fprintf(stderr, "Error en X509_NAME_oneline"));
			m_iError = M4TSAP_SSL_X509_NAME_ONLINE_FAILED;
			
			return -1;
		};
//		Free (str);
		
		/*  str = X509_NAME_oneline (X509_get_issuer_name  (server_cert));
		CHK_NULL(str);
		printf ("\t issuer: %s\n", str);
		Free (str);
		*/
		/* We could do all sorts of certificate verification stuff here before
		deallocating the certificate. */
		
		X509_free (server_cert);
	}
#endif
	return(0);
  //## end ClSSLTSAPImpl::Connect%-1656302624.body
}

int ClSSLTSAPImpl::Connect (ClInetAddress* ai_InetAddress, int ai_iPort)
{
  //## begin ClSSLTSAPImpl::Connect%588994194.body preserve=yes
#ifndef _NO_USE_SSL
	struct sockaddr_in saddr;
	X509*    server_cert;
	char*    str;
	char	buf[256];
	
	memcpy(&saddr.sin_addr.s_addr, (const void*) ai_InetAddress->GetAddress(),
		ai_InetAddress->m_s_iAddrLen);
	//iAddrLen);
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(ai_iPort);
#ifdef BSD
	saddr.sin_len = sizeof(saddr);
#endif
#ifndef _UNIX
	if (connect(m_iSocket, (struct sockaddr*) &saddr, sizeof(saddr)) == SOCKET_ERROR)
	{
		m4Trace(fprintf(stderr, "Error en connect"));
		m_iError = M4TSAP_CONNECT_FAILED;
		
		return WSAGetLastError();
	}
#else
	if (connect(m_iSocket, (struct sockaddr*) &saddr, sizeof(saddr)) < 0)
	{
		m_iError = M4TSAP_CONNECT_FAILED;
		m4Trace(fprintf(stderr, "Error en connect"));
		
		return -1;
	}
#endif
	if (GetCTXClient() == NULL)
	{
		m_iError = M4TSAP_SSL_CLIENT_NOT_INITIALIZED;
		m4Trace(fprintf(stderr, "Error en Connect, ctx es null"));
		
		return -1;
	};
	
	m_ssl = SSL_new (GetCTXClient());
	if (m_ssl == NULL)
	{

		m4Trace(fprintf(stderr, "Error en Connect, SSL_new retorna null"));
		m_iError = M4TSAP_SSL_SSL_NEW_FAILED;
		
		return -1;
	};
	
	if (m_iMode == M4_SSL_SET_PREFERED_CIPHER)
		if (!SSL_set_cipher_list(m_ssl, (char *)m_pData))
		{
			m_iError = M4TSAP_SSL_SET_CIPHER_LIST_FAILED;
			m4Trace(fprintf(stderr, "Error en SSL_set_cipher_list %s\n",
						(char *)m_pData));
			
			return -1;
		};

	/* Se asocia la conexion SSL con el socket (TCP) */
	
	SSL_set_fd (m_ssl, m_iSocket);
	if (!SSL_connect(m_ssl))
	{
		m4Trace(fprintf(stderr, "Error en SSL_connect"));
		m_iError = M4TSAP_SSL_CONNECT_FAILED;
		
		return -1;
	};
	if (GetModeMeta4SSL() == M4_SSL_CLIENT_CERTIFICATE ||
		GetModeMeta4SSL() == M4_SSL_CLIENTSERVER_CERTIFICATE)
	{
		server_cert = SSL_get_peer_certificate (m_ssl);
		if (server_cert == NULL)
		{
			m_iError = M4TSAP_SSL_GET_PEER_CERTIFICATE_FAILED;
			m4Trace(fprintf(stderr, "Error en SSL_get_peer_certificate"));
			
			return -1;
		};
		
		str = X509_NAME_oneline(X509_get_subject_name(server_cert), buf, 256);
		if (str == NULL)
		{
			m4Trace(fprintf(stderr, "Error en X509_NAME_oneline"));
			m_iError = M4TSAP_SSL_X509_NAME_ONLINE_FAILED;
			
			return -1;
		};
//		Free (str);
		
		/*  str = X509_NAME_oneline (X509_get_issuer_name  (server_cert));
		CHK_NULL(str);
		printf ("\t issuer: %s\n", str);
		Free (str);
		*/
		/* We could do all sorts of certificate verification stuff here before
		deallocating the certificate. */
		
		X509_free (server_cert);
	}
#endif
	return 0;
  //## end ClSSLTSAPImpl::Connect%588994194.body
}

int ClSSLTSAPImpl::Create (m4bool_t ai_bStream)
{
  //## begin ClSSLTSAPImpl::Create%1176852083.body preserve=yes

#ifndef _UNIX
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
		return -1;
	}
	
#else
	if ((m_iSocket = socket(AF_INET, (ai_bStream == M4_TRUE)? SOCK_STREAM : SOCK_DGRAM, 0)) == -1)
	{
		/* Bug 0113374
		Se saca un mensaje de error con el mensaje del sistema
		*/
		int		iError = errno ;
		SETCODEF( M4_ERR_TSAP_NO_OPEN_SOCKECT, ERRORLOG, "#*S1*#%s#", strerror( iError ) ) ;

		m_iError = M4TSAP_SOCKET_FAILED;
		return -1;
	}

#ifdef _AIX
	/* Bug 0122636
	Se ponen las opciones de no delay en el protocolo de sockets en AIX
	No se controlan errores, si no se puede nos quedamos sin optimización
	*/
	int iResult = 0 ;
	int iFlag = 1 ;

	iResult = setsockopt( m_iSocket, IPPROTO_TCP, TCP_NODELAY,    (void*) &iFlag, sizeof(iFlag) ) ;
	iResult = setsockopt( m_iSocket, IPPROTO_TCP, TCP_NODELAYACK, (void*) &iFlag, sizeof(iFlag) ) ;
#endif

#ifdef _SOLARIS
	//MOD JOSEAVM 29-6-2001
	//ClMoveUpFds::MoveUp(m_iSocket);
	int iPrevSock=m_iSocket;
	m_iSocket=fcntl( m_iSocket, F_DUPFD, DELTA_FDS );
	if(m_iSocket<0)
	{
		m_iError = M4TSAP_SOCKET_FAILED;
		return -1;
	}
	else
		close(iPrevSock);

	//END MOD
#endif
#endif
	
	return(0);
  //## end ClSSLTSAPImpl::Create%1176852083.body
}

int ClSSLTSAPImpl::Listen (int ai_bBacklog)
{
  //## begin ClSSLTSAPImpl::Listen%-1098192822.body preserve=yes
#ifndef _UNIX
	if (listen(m_iSocket, ai_bBacklog) == SOCKET_ERROR)
#else
	if (listen(m_iSocket, ai_bBacklog) < 0)
#endif
	{
		m_iError = M4TSAP_LISTEN_FAILED;

		return -1;
	}
		
	return 0;
  //## end ClSSLTSAPImpl::Listen%-1098192822.body
}

int ClSSLTSAPImpl::Write (const void* src, int len)
{
  //## begin ClSSLTSAPImpl::Write%-1444415595.body preserve=yes
#ifndef _NO_USE_SSL
	int rc,rc2;
	
	if (m_ssl == NULL)
	{
		m_iError = M4TSAP_SSL_M_SSL_NOT_INITIALIZED;
			
		return -1;
	}
	
	SetBlockingIO(M4_FALSE);
	rc = SSL_write (m_ssl, (char *)src, len);
	SetBlockingIO(M4_TRUE);

	switch (rc2=SSL_get_error(m_ssl,rc)) {
		case SSL_ERROR_WANT_WRITE:
		case SSL_ERROR_WANT_READ:
			rc = -2;
			break;
		default:
#ifndef _UNIX
			//  rc = send ( m_iSocket  , (const char * )src , len , 0 );   // ( Especifico Windows )
			// Si tenemos salida no bloqueante y el buffer del sistema esta lleno, la
			// llamada a write devuelve -1. Sin embargo, esto en realidad no es un
			// error, sino una situacion normal, por lo que ocultamos
			// este "pseudo-error".
			if (rc == SOCKET_ERROR  && WSAGetLastError()  == WSAEWOULDBLOCK) {
				rc = -2;
			}
#else
			if (rc == -1 && errno == EAGAIN) {
				rc = -2;
			}
#endif
	}

	if (rc < 0 && rc != -2) {
		m_iError = (m_iError == M4TSAP_OK)?M4TSAP_SSL_WRITE_FAILED:m_iError;
	}
	
	return rc;
#else
	return 0;
#endif
	//## end ClSSLTSAPImpl::Write%-1444415595.body
}

int ClSSLTSAPImpl::Read (void* dst, int len)
{
  //## begin ClSSLTSAPImpl::Read%-552243267.body preserve=yes
#ifndef _NO_USE_SSL
	int rc;
	
	if (m_ssl == NULL)
	{
		m_iError = M4TSAP_SSL_M_SSL_NOT_INITIALIZED;
		
		return -1;
	}
	
	rc = SSL_read(m_ssl, (char *)dst,  len);
//  rc = recv(m_iSocket, (char *)dst, len , 0 );
  // Ver comentario de WindowsTSAPImpl::Write
#ifndef _UNIX
	if (rc == SOCKET_ERROR  && WSAGetLastError()  == WSAEWOULDBLOCK)
		rc = 0;
#endif
	

	if(rc == 0)
	{	if(m_iCeroCounter>25)
		{
			rc=-1;
			m_iCeroCounter=0;
		}
		else
		{
			m_iCeroCounter++;
		}
	}
	else
		m_iCeroCounter=0;
	return rc;
#else
	return 0;
#endif
	//## end ClSSLTSAPImpl::Read%-552243267.body
}

int ClSSLTSAPImpl::SetBlockingIO (m4bool_t ai_bBlocking)
{
  //## begin ClSSLTSAPImpl::SetBlockingIO%-125221782.body preserve=yes
#ifndef _UNIX
	unsigned long flags;

	if (ai_bBlocking)
		flags = 0L;
	else
    flags = 1L;

	if (ioctlsocket(m_iSocket, FIONBIO, &flags) != 0)
		return WSAGetLastError();

	return 0;
#else
  //JRM Debido a que se ha encontrado un error en la librería ssleay
  //cuando se usan sockets no bloqueantes, esta función devuelve 0
  // sin cambiar el modo del socket, que por defecto es siempre bloqueante 
  // return 0;

  int flags;

  flags = fcntl(m_iSocket, F_GETFL, 0);
  if (flags == -1) {
	return -1;
  }
  if (!ai_bBlocking) {
    flags |= O_NONBLOCK;
  } else {
    flags &= ~O_NONBLOCK;
  }
  if (fcntl(m_iSocket, F_SETFL, flags) == -1) {
	return -1;
  }

  return 0;

#endif
  //## end ClSSLTSAPImpl::SetBlockingIO%-125221782.body
}

// Additional Declarations
  //## begin ClSSLTSAPImpl%3777684302F5.declarations preserve=yes
  //## end ClSSLTSAPImpl%3777684302F5.declarations

//## begin module%377768480229.epilog preserve=yes



#ifndef _UNIX
int SocketInit(int ai_iModeSSL, void *ai_pData)
{
	SocketInit();
	InitLock();

	switch (ai_iModeSSL)
	{
	case M4_SSL_CLIENT_CERTIFICATE:
		if (bSSLClientInitialized == M4_FALSE)
			if (SSL_Initialize(ai_iModeSSL, ai_pData) == 0)
				bSSLClientInitialized = M4_TRUE;
		break;
		
	case M4_SSL_SERVER:

	if (bSSLServerInitialized == M4_FALSE)
		if (SSL_Initialize(ai_iModeSSL, ai_pData) == 0)
			bSSLServerInitialized = M4_TRUE;
	}
	InitUnlock();

	return 0;
}

#else
int SocketInit(int ai_iModeSSL, void *ai_pData)
{
	int	ret = 0;

	SocketInit();
#ifndef _NO_USE_SSL
	InitLock();
	switch (ai_iModeSSL)
	{
	case M4_SSL_CLIENT_CERTIFICATE:
		if (bSSLClientInitialized == M4_FALSE)
			if (SSL_Initialize(ai_iModeSSL, ai_pData) == 0)
				bSSLClientInitialized = M4_TRUE;
		break;
		
	case M4_SSL_SERVER:

	if (bSSLServerInitialized == M4_FALSE)
		if (SSL_Initialize(ai_iModeSSL, ai_pData) == 0)
			bSSLServerInitialized = M4_TRUE;
	}
	InitUnlock();

#endif
	return ret;
}

#endif

void SocketFinish(int ai_iModeSSL)
{
#ifndef _NO_USE_SSL
	if (GetCTXClient() != NULL)
		SSL_CTX_free (GetCTXClient());

	if (GetCTXServer() != NULL)
		SSL_CTX_free (GetCTXServer());
#endif

	SocketFinish();
}
 
//## end module%377768480229.epilog
