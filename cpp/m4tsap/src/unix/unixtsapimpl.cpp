// Revision 1.2  1997/10/03  14:39:52  pjlobo
// Añadido control de errores
//
// Revision 1.1  1997/10/03  09:24:44  pjlobo
// Initial revision
//
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include "m4log.hpp"
#include "m4srvres.hpp"



#include "tsap.hpp"
#include "unix/unixtsap.hpp"

#ifdef _SOLARIS
#include "unix/moveupfds.hpp"
#endif

typedef unsigned char byte;
int ClUnixTSAPImpl::Accept(ClTSAPImpl* newTSAP)
{
	int	i;
	struct sockaddr_in saddr;
#ifdef _AIX
	size_t slen;
#else
	int slen;
#endif
	ClUnixTSAPImpl* ns = (ClUnixTSAPImpl*) newTSAP;
	
	slen = sizeof(saddr);
	ns->iSocket = accept(iSocket, (struct sockaddr*) &saddr, &slen);
	if (ns->iSocket == -1) 
	{
		m_iError = M4TSAP_ACCEPT_FAILED;
		return -1;
	}

#ifdef _SOLARIS
	//MOD JOSEAVM 29-6-2001
	//if (ClMoveUpFds::MoveUp(ns->iSocket) != M4_SUCCESS)
	int iPrevSock=ns->iSocket;
	ns->iSocket=fcntl( ns->iSocket, F_DUPFD, DELTA_FDS );
	if(ns->iSocket<0)
	{
		m_iError = M4TSAP_ACCEPT_FAILED;
		return -1;
	}
	else
	{
		close(iPrevSock);
	}
	//END MOD 
#endif

	if (saddr.sin_family != AF_INET) {
		// Es un error. Actualmente solo tratamos direcciones de la
		// familia AF_INET
		m_iError = M4TSAP_INVALID_INET_ADDRESS;
		return -1;
	}

	ns->m_iPort = saddr.sin_port;
	// XXX MUCHO OJO CON ESTO: mirar maquinas "little endian" y "big endian"
	ns->m_poInetAddress = new ClInetAddress((const byte*) &saddr.sin_addr.s_addr);
	
	if (ns->m_poInetAddress == NULL)
	{
		m_iError = M4TSAP_INVALID_INET_ADDRESS;
		return -1;
	}
	return 0;
}

int ClUnixTSAPImpl::Bind(ClInetAddress* src, int lport)
{
	struct sockaddr_in saddr;
	
	if (src == NULL) {
		saddr.sin_addr.s_addr = INADDR_ANY;
	} else {
		memcpy(&saddr.sin_addr.s_addr, (const void*) src->GetAddress(),
			src->m_s_iAddrLen);
	}
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(lport);
#ifdef BSD
	saddr.sin_len = sizeof(saddr);
#endif
	if (bind(iSocket, (struct sockaddr*) &saddr, sizeof(saddr)) < 0) 
	{
		m_iError = M4TSAP_BIND_FAILED;
		return -1;
	}
	return 0;
}

void ClUnixTSAPImpl::Close()
{
//MOD JOSEAVM 9-7-2001

//#ifdef _SOLARIS
//	ClMoveUpFds::Clean(iSocket);
//#endif

// END MOD
	close(iSocket);
}

int ClUnixTSAPImpl::Connect(const char* hostName, int port)
{
	struct sockaddr_in saddr;
	struct hostent* hp;
	
	hp = gethostbyname(hostName);
	if (hp == NULL) 
	{
		m_iError = M4TSAP_INVALID_HOSTNAME;
		return -1;
	}
	memcpy(&saddr.sin_addr.s_addr, hp->h_addr, hp->h_length);
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
#ifdef BSD
	saddr.sin_len = sizeof(saddr);
#endif
	if (connect(iSocket, (struct sockaddr*) &saddr, sizeof(saddr)) < 0) 
	{
		m_iError = M4TSAP_CONNECT_FAILED;
		return -1;
	}
	
	return 0;
}

int ClUnixTSAPImpl::Connect(ClInetAddress* dst, int port)
{
	struct sockaddr_in saddr;
	
	memcpy(&saddr.sin_addr.s_addr, (const void*) dst->GetAddress(),
		dst->m_s_iAddrLen);
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
#ifdef BSD
	saddr.sin_len = sizeof(saddr);
#endif
	if (connect(iSocket, (struct sockaddr*) &saddr, sizeof(saddr)) < 0) 
	{
		m_iError = M4TSAP_CONNECT_FAILED;
		return -1;
	}
	
	return 0;
}

int ClUnixTSAPImpl::Create(m4bool_t stream)
{
	iSocket = socket(AF_INET, stream ? SOCK_STREAM : SOCK_DGRAM, 0);
	if (iSocket == -1) 
	{
		/* Bug 0113374
		Se saca un mensaje de error con el mensaje del sistema
		*/
		int	iError = errno ;
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

	iResult = setsockopt( iSocket, IPPROTO_TCP, TCP_NODELAY,    (void*) &iFlag, sizeof(iFlag) ) ;
	iResult = setsockopt( iSocket, IPPROTO_TCP, TCP_NODELAYACK, (void*) &iFlag, sizeof(iFlag) ) ;
#endif

#ifdef _SOLARIS
	//MOD JOSEAVM 29-6-2001
	//if (ClMoveUpFds::MoveUp(iSocket) != M4_SUCCESS)
	int iPrevSock=iSocket;	
	iSocket=fcntl( iSocket, F_DUPFD, DELTA_FDS );
	if(iSocket<0)
	{
		/* Bug 0113374
		Se saca un mensaje de error con el mensaje del sistema
		Si es un Invalid argument( 22 ) se pasa a Too many open files( 24 ) porque posiblemente
		DELTA_FDS que es 255 es más de los descriptores disponibles
		*/
		int	iError = errno ;

		if( iError == EINVAL )
		{
			iError = EMFILE ;
		}
		SETCODEF( M4_ERR_TSAP_NO_OPEN_SOCKECT, ERRORLOG, "#*S1*#%s#", strerror( iError ) ) ;

		m_iError = M4TSAP_SOCKET_FAILED;
		return -1;
	}
	else
	{
		close(iPrevSock);
	}

	// END MOD
#endif

	return 0;
}

int ClUnixTSAPImpl::Listen(int backlog)
{
	if (listen(iSocket, backlog) < 0) 
	{
		m_iError = M4TSAP_LISTEN_FAILED;
		return -1;
	}
	
	return 0;
}

int ClUnixTSAPImpl::Write(const void* src, int len)
{
	int rc;
	
	rc = write(iSocket, src, len);
	// Si tenemos salida no bloqueante y el buffer del sistema esta lleno, la
	// llamada a write devuelve -1. Sin embargo, esto en realidad no es un
	// error, sino una situacion normal, por lo que ocultamos
	// este "pseudo-error".
	if (rc == -1 && errno == EWOULDBLOCK)
		rc = 0;

	if (rc < 0)
		m_iError = (m_iError == M4TSAP_OK)?M4TSAP_WRITE_FAILED:m_iError;
	return rc;
}

int ClUnixTSAPImpl::Read(void* dst, int len)
{
	//	struct timeval tp_antes;
	//	struct timeval tp_despues;
	int rc;
	
	//	gettimeofday(&tp_antes, NULL);
	rc = read(iSocket, dst, len);
	//	gettimeofday(&tp_despues, NULL);
	//	fprintf(stderr, "Antes %ld %ld despues %ld %ld retorna %d errno %d",
	//		tp_antes.tv_sec,
	//		tp_antes.tv_usec,
	//		tp_despues.tv_sec,
	//		tp_despues.tv_usec,
	//		rc,
	//		errno);
	//fflush(stderr);	
	// Ver comentario de UnixTSAPImpl::Write
	if (rc == -1 && errno == EWOULDBLOCK)
		rc = 0;

	if (rc < 0)
		m_iError = (m_iError == M4TSAP_OK)?M4TSAP_READ_FAILED:m_iError;
	return rc;
}

int ClUnixTSAPImpl::SetBlockingIO(m4bool_t blocking)
{
	int flags;
	
	flags = fcntl(iSocket, F_GETFL, 0);
	if (flags == -1) 
	{
		return -1;
	}
	if (!blocking) {
		flags |= O_NONBLOCK;
	} else {
		flags &= ~O_NONBLOCK;
	}
	if (fcntl(iSocket, F_SETFL, flags) == -1) {
		return -1;
	}
	return 0;
}


