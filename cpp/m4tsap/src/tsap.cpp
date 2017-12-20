//## begin module%377768480284.cm preserve=no
//## end module%377768480284.cm

//## begin module%377768480284.cp preserve=no
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
//## end module%377768480284.cp

//## Module: tsap%377768480284; Package body
//## Subsystem: M4Tsap::src%3777684800C1
//	d:\compon\m4tsap\version\src
//## Source file: d:\compon\m4tsap\version\src\tsap.cpp

//## begin module%377768480284.additionalIncludes preserve=no
//## end module%377768480284.additionalIncludes

//## begin module%377768480284.includes preserve=yes
#include "m4winsock.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#ifdef UNIX
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
#endif

#include <syncro.hpp>
#include <blocksynchronization.hpp>


#include "m4trace.hpp"
#include "m4string.hpp"
#include "tsap.hpp"
#include "tsapimplfactory.hpp"
#include "tsapimpl.hpp"
#include "ssltsapimpl.hpp"
#ifdef _UNIX
#include "unix/unixtsap.hpp"
#else
#include "windowstsap.hpp"
//## end module%377768480284.includes

//## begin module%377768480284.declarations preserve=no
//## end module%377768480284.declarations

//## begin module%377768480284.additionalDeclarations preserve=yes
#endif
ClMutex	sMutex(M4_TRUE);
m4bool_t	bSystemInitialized = M4_FALSE;

void	InitUnlock()
{
	sMutex.Unlock();
}

void	InitLock()
{
	sMutex.Lock();
}

#ifdef _WINDOWS

ClWindowsTSAPImplFactory* s_poTSAPImplFactory = new ClWindowsTSAPImplFactory();


int SocketInit( void )
{


	sMutex.Lock();

	if (bSystemInitialized == M4_FALSE)
	{
		ClTSAP::SetTSAPImplFactory(s_poTSAPImplFactory);
		
		WORD wVersionRequested;
		WSADATA wsaData;
		
		int ret;
		
		wVersionRequested = MAKEWORD( 1, 1 );
		
		ret = WSAStartup( wVersionRequested, &wsaData );
		if (ret != 0)
		{
			
			sMutex.Unlock();
			return M4SOCK_ERROR;
			
		}
		else
		{
			
			if ( LOBYTE( wsaData.wVersion ) != 1 ||
				HIBYTE( wsaData.wVersion ) != 1 )
			{
				// Error: Versión no aceptable de winsocks.dll
				
				WSACleanup( );
				
				
				sMutex.Unlock();
				return M4SOCK_ERROR;
			}
		}
		bSystemInitialized = M4_TRUE;
	}
	sMutex.Unlock();

	return 0;
}



#endif

#ifdef _UNIX

ClUnixTSAPImplFactory* s_poTSAPImplFactory = new ClUnixTSAPImplFactory();

int SocketInit( void )
{
	ClTSAP::SetTSAPImplFactory(s_poTSAPImplFactory);
	return 0;
}

#endif


void SocketFinish()
{
	if (s_poTSAPImplFactory != NULL)
		delete s_poTSAPImplFactory;
}


//## end module%377768480284.additionalDeclarations


// Class ClTSAP 



//## begin ClTSAP::m_s_poTSAPImplFactory%37776844034A.role preserve=no  private: static ClTSAPImplFactory {1 -> 1RFHAPN}
ClTSAPImplFactory *ClTSAP::m_s_poTSAPImplFactory;
//## end ClTSAP::m_s_poTSAPImplFactory%37776844034A.role




ClTSAP::ClTSAP (const char* ai_pcHostname, int ai_iPort, m4bool_t ai_bStream)
  //## begin ClTSAP::ClTSAP%-1259939195.hasinit preserve=no
  //## end ClTSAP::ClTSAP%-1259939195.hasinit
  //## begin ClTSAP::ClTSAP%-1259939195.initialization preserve=yes
  //## end ClTSAP::ClTSAP%-1259939195.initialization
{
  //## begin ClTSAP::ClTSAP%-1259939195.body preserve=yes
	ClInetAddress* poInetAddress;
	m_bError = M4_FALSE;
	m_bConectado = M4_FALSE;
	m_bTipoSSL = M4_FALSE;
	m_poTSAPImpl = NULL;
	m_lId  = 0 ;

	if (m_s_poTSAPImplFactory == NULL)
		return;

	if ((poInetAddress = new ClInetAddress(ai_pcHostname)) == NULL)
    {
		m_bError = M4_TRUE;
		return;
	}

	if (!poInetAddress->IsRight())
    {
		m_bError = M4_TRUE;
		return;
	}

	if ((m_poTSAPImpl = m_s_poTSAPImplFactory->CreateTSAPImpl()) == NULL ||
			m_poTSAPImpl->Create(ai_bStream) == -1 ||
			m_poTSAPImpl->Bind(NULL, 0) == -1 ||
			m_poTSAPImpl->Connect(poInetAddress, ai_iPort) != 0)
	{
		m_bError = M4_TRUE;
		m_bConectado = M4_FALSE;
	}
	else
		m_bConectado = M4_TRUE;

	delete poInetAddress;
  //## end ClTSAP::ClTSAP%-1259939195.body
}

ClTSAP::ClTSAP (ClInetAddress* ai_poInetAddress, int ai_iPort, m4bool_t ai_bStream)
  //## begin ClTSAP::ClTSAP%70042277.hasinit preserve=no
  //## end ClTSAP::ClTSAP%70042277.hasinit
  //## begin ClTSAP::ClTSAP%70042277.initialization preserve=yes
  //## end ClTSAP::ClTSAP%70042277.initialization
{
  //## begin ClTSAP::ClTSAP%70042277.body preserve=yes
	if (m_s_poTSAPImplFactory == NULL)
		return;

	m_bError = M4_FALSE;
	m_bConectado = M4_FALSE;
	m_bTipoSSL = M4_FALSE;
	m_poTSAPImpl = NULL;
	m_lId  = 0 ;

	if ((m_poTSAPImpl = m_s_poTSAPImplFactory->CreateTSAPImpl()) == NULL ||
			m_poTSAPImpl->Create(ai_bStream) == -1 ||
			m_poTSAPImpl->Bind(NULL, 0) == -1 ||
			m_poTSAPImpl->Connect(ai_poInetAddress, ai_iPort) != 0)
		m_bError = M4_TRUE;
  //## end ClTSAP::ClTSAP%70042277.body
}

ClTSAP::ClTSAP ()
  //## begin ClTSAP::ClTSAP%1469968033.hasinit preserve=no
  //## end ClTSAP::ClTSAP%1469968033.hasinit
  //## begin ClTSAP::ClTSAP%1469968033.initialization preserve=yes
  //## end ClTSAP::ClTSAP%1469968033.initialization
{
  //## begin ClTSAP::ClTSAP%1469968033.body preserve=yes
	m_bError = M4_FALSE;
	m_poTSAPImpl = NULL;
	m_bConectado = M4_FALSE;
	m_bTipoSSL = M4_FALSE;
	m_lId  = 0 ;

	if ((m_poTSAPImpl = m_s_poTSAPImplFactory->CreateTSAPImpl()) == NULL)
		m_bError = M4_TRUE;

  //## end ClTSAP::ClTSAP%1469968033.body
}

ClTSAP::ClTSAP (int ai_iMode, void* ai_pData, const char* ai_pcHostname, int ai_iPort, m4bool_t ai_bStream)
  //## begin ClTSAP::ClTSAP%-1779220893.hasinit preserve=no
  //## end ClTSAP::ClTSAP%-1779220893.hasinit
  //## begin ClTSAP::ClTSAP%-1779220893.initialization preserve=yes
  //## end ClTSAP::ClTSAP%-1779220893.initialization
{
  //## begin ClTSAP::ClTSAP%-1779220893.body preserve=yes
	m_bError = M4_FALSE;
	m_bConectado = M4_FALSE;
	m_poTSAPImpl = NULL;
	m_lId  = 0 ;

	if (m_s_poTSAPImplFactory == NULL)
		return;

	ClInetAddress* poInetAddress;
	m_bConectado = M4_FALSE;

	if (m_s_poTSAPImplFactory == NULL)
		return;

	if ((poInetAddress = new ClInetAddress(ai_pcHostname)) == NULL)
    {
		m_bError = M4_TRUE;
		return;
	}

	if (!poInetAddress->IsRight())
    {
		m_bError = M4_TRUE;
		return;
	}

	m_bTipoSSL = M4_TRUE;
	if ((m_poTSAPImpl = m_s_poTSAPImplFactory->CreateTSAPSSLImpl(ai_iMode, ai_pData)) == NULL ||
			m_poTSAPImpl->Create(ai_bStream) == -1 ||
			m_poTSAPImpl->Bind(NULL, 0) == -1 ||
			m_poTSAPImpl->Connect(poInetAddress, ai_iPort) != 0)
	{
		m_bError = M4_TRUE;
		m_bConectado = M4_FALSE;
	}
	else
		m_bConectado = M4_TRUE;

	delete poInetAddress;
  //## end ClTSAP::ClTSAP%-1779220893.body
}

ClTSAP::ClTSAP (int ai_iMode, void* ai_pData)
  //## begin ClTSAP::ClTSAP%682838846.hasinit preserve=no
  //## end ClTSAP::ClTSAP%682838846.hasinit
  //## begin ClTSAP::ClTSAP%682838846.initialization preserve=yes
  //## end ClTSAP::ClTSAP%682838846.initialization
{
  //## begin ClTSAP::ClTSAP%682838846.body preserve=yes
	m_bError = M4_FALSE;
	m_poTSAPImpl = NULL;
	m_bConectado = M4_FALSE;
	m_poTSAPImpl = NULL;
	m_lId  = 0 ;

	m_bTipoSSL = M4_TRUE;
	if ((m_poTSAPImpl = m_s_poTSAPImplFactory->CreateTSAPSSLImpl(ai_iMode, ai_pData)) == NULL)
		m_bError = M4_TRUE;
  //## end ClTSAP::ClTSAP%682838846.body
}


ClTSAP::~ClTSAP ()
{
  //## begin ClTSAP::~ClTSAP%760283972.body preserve=yes
//	if 	(m_bTipoSSL == M4_TRUE)
//	{
		if (m_poTSAPImpl != NULL)
		{
//			delete (ClSSLTSAPImpl *)m_poTSAPImpl;
			Close();
			delete m_poTSAPImpl;
			m_poTSAPImpl = NULL;
		}
//	}
//	else
//	if (m_poTSAPImpl != NULL)
	{
//#ifndef _UNIX
//		delete (ClWindowsTSAPImpl *)m_poTSAPImpl;
//#else
//		delete (ClUnixTSAPImpl *)m_poTSAPImpl;
//#endif
	}
  //## end ClTSAP::~ClTSAP%760283972.body
}



//## Other Operations (implementation)
m4bool_t ClTSAP::Init (const char* ai_pcHostname, int ai_iPort, m4bool_t ai_bStream)
{
  //## begin ClTSAP::Init%-1334151515.body preserve=yes
	ClInetAddress* poInetAddress;
	// Comprobar se ha llamado antes al constructor por defecto
	// Seguimos manteniendo la variable miembro bError por
	// consistencia con  implementaciones anteriores
	if (m_s_poTSAPImplFactory == NULL)
		return M4_FALSE;

	if (m_bError != M4_FALSE || m_poTSAPImpl == NULL)
		return M4_FALSE;

	if (m_poTSAPImpl == NULL)
		if ((m_poTSAPImpl= m_s_poTSAPImplFactory->CreateTSAPImpl()) == NULL)
		{
			m_bError = M4_TRUE;
			return M4_FALSE;
		}
	
	if ((poInetAddress = new ClInetAddress(ai_pcHostname)) == NULL)
    {
		m_bError = M4_TRUE;
		return M4_FALSE;
	}
	if (!poInetAddress->IsRight())
    {
		m_bError = M4_TRUE;
		return M4_FALSE;
	}

	if (m_poTSAPImpl->Create(ai_bStream) == -1 ||
			m_poTSAPImpl->Bind(NULL, 0) == -1 ||
			m_poTSAPImpl->Connect(poInetAddress, ai_iPort) != 0)
		m_bError = M4_TRUE;

	delete poInetAddress;

	if (!m_bError)
		m_bConectado = M4_TRUE;

	return M4_BOOL(!m_bError);
  //## end ClTSAP::Init%-1334151515.body
}

m4bool_t ClTSAP::Init (ClInetAddress* ai_poInetAddress, int ai_iPort, m4bool_t ai_bStream)
{
  //## begin ClTSAP::Init%1585032072.body preserve=yes
	if (m_s_poTSAPImplFactory == NULL)
		return M4_FALSE;

	m_bError = M4_FALSE;

	if (m_poTSAPImpl == NULL)
		if ((m_poTSAPImpl = m_s_poTSAPImplFactory->CreateTSAPImpl()) == NULL)
		{
			m_bError = M4_TRUE;
			return M4_FALSE;
		}
	if (m_poTSAPImpl->Create(ai_bStream) == -1 ||
			m_poTSAPImpl->Bind(NULL, 0) == -1 ||
			m_poTSAPImpl->Connect(ai_poInetAddress, ai_iPort) != 0)
	{
		m_bError = M4_TRUE;
		return M4_FALSE;
	}

	m_bConectado = M4_TRUE;
	return M4_TRUE;;
  //## end ClTSAP::Init%1585032072.body
}

void ClTSAP::Close ()
{
  //## begin ClTSAP::Close%-840728080.body preserve=yes
	// bugid: 0094197. Tenqo que cerrar siempre. El socket puede estar creado
    // aunque no estemos conectados.
    //if(m_bConectado)
		m_poTSAPImpl->Close();

	m_bConectado = M4_FALSE;
  //## end ClTSAP::Close%-840728080.body
}

int ClTSAP::Write (const void* ai_pcSrc, int ai_iLen)
{
  //## begin ClTSAP::Write%-824776902.body preserve=yes
	if (!m_bConectado)
		return -1;

  return m_poTSAPImpl->Write(ai_pcSrc, ai_iLen);
  //## end ClTSAP::Write%-824776902.body
}

int ClTSAP::Read (void* ao_pcDst, int ai_iLen, m4bool_t ai_bTotal)
{
  //## begin ClTSAP::Read%732633365.body preserve=yes
	if (!m_bConectado)
		return -1;

	if (ai_bTotal == M4_FALSE)
		return m_poTSAPImpl->Read(ao_pcDst, ai_iLen);

	int longitud_leido = 0;
	int	ret;
	char	*pChar = (char *)ao_pcDst;

	while (longitud_leido < ai_iLen)
	{
		ret = m_poTSAPImpl->Read((void *)&pChar[longitud_leido], ai_iLen - longitud_leido);
		longitud_leido += ret;
//		pChar += ret;
		if (ret <= 0)
			return ret;
}
	return longitud_leido;
  //## end ClTSAP::Read%732633365.body
}

int ClTSAP::SetBlockingIO (m4bool_t ai_bBlocking)
{
  //## begin ClTSAP::SetBlockingIO%61045530.body preserve=yes
	if (m_poTSAPImpl == NULL)
		return M4_FALSE;

	return m_poTSAPImpl->SetBlockingIO(ai_bBlocking);
  //## end ClTSAP::SetBlockingIO%61045530.body
}

int ClTSAP::GetLocalPort () const
{
  //## begin ClTSAP::GetLocalPort%1039449503.body preserve=yes
	if (m_poTSAPImpl == NULL)
		return -1;

	return m_poTSAPImpl->GetLocalPort();
  //## end ClTSAP::GetLocalPort%1039449503.body
}

int ClTSAP::GetPort () const
{
  //## begin ClTSAP::GetPort%635553501.body preserve=yes
	if (m_poTSAPImpl == NULL)
		return -1;

	return m_poTSAPImpl->GetPort();
  //## end ClTSAP::GetPort%635553501.body
}

char* ClTSAP::ToString () const
{
  //## begin ClTSAP::ToString%-1942123768.body preserve=yes
	if (m_poTSAPImpl == NULL)
		return NULL;

	return m_poTSAPImpl->ToString();
  //## end ClTSAP::ToString%-1942123768.body
}

m4bool_t ClTSAP::Error () const
{
  //## begin ClTSAP::Error%502218168.body preserve=yes
  return m_bError;
  //## end ClTSAP::Error%502218168.body
}

int ClTSAP::GetError () const
{
  //## begin ClTSAP::GetError%52443838.body preserve=yes
	if (m_poTSAPImpl)
		return m_poTSAPImpl->GetError();

	return M4TSAP_NOT_INITIALIZED;
  //## end ClTSAP::GetError%52443838.body
}

char* ClTSAP::GetStringError () const
{
  //## begin ClTSAP::GetStringError%-1391374849.body preserve=yes
	if (m_poTSAPImpl)
		return m_poTSAPImpl->GetStringError();

	return M4TSAP_STR_NOT_INITIALIZED;
  //## end ClTSAP::GetStringError%-1391374849.body
}

m4int32_t ClTSAP::SetId (m4int32_t ai_lId)
{
  //## begin ClTSAP::SetId%1141040247.body preserve=yes
	m4int32_t	lOldId = m_lId;

	m_lId = ai_lId;

	return lOldId;
  //## end ClTSAP::SetId%1141040247.body
}

m4int32_t ClTSAP::GetId ()
{
  //## begin ClTSAP::GetId%-901954985.body preserve=yes
	return m_lId;
  //## end ClTSAP::GetId%-901954985.body
}

int ClTSAP::GetSystemDescriptor ()
{
  //## begin ClTSAP::GetSystemDescriptor%2114005834.body preserve=yes
	if (m_poTSAPImpl == NULL)
		return -1;

	return m_poTSAPImpl->GetSystemDescriptor();
  //## end ClTSAP::GetSystemDescriptor%2114005834.body
}

m4bool_t ClTSAP::GetRemoteIP (m4pchar_t ai_szAddress, int ai_iMaxLen)
{
  //## begin ClTSAP::GetRemoteIP%1432512714.body preserve=yes
	ClInetAddress	*pInetAddress;

	if ( m_poTSAPImpl == NULL)
	{
		m4Trace(fprintf(stderr, "Puntero a TSAP implementation nulo, TSAP incorrectamente inicializadas\n"));

		return M4_FALSE;
	}
	pInetAddress = m_poTSAPImpl->GetInetAddress();
	if (pInetAddress == NULL)
		return M4_FALSE;

	return pInetAddress->GetIP(ai_szAddress, ai_iMaxLen);

  //## end ClTSAP::GetRemoteIP%1432512714.body
}

m4bool_t ClTSAP::GetRemoteHostname (m4pchar_t ai_szAddress, int ai_iMaxLen)
{
  //## begin ClTSAP::GetRemoteHostname%-1762171823.body preserve=yes
	ClInetAddress	*pInetAddress;

	if ( m_poTSAPImpl == NULL)
	{
		m4Trace(fprintf(stderr, "Puntero a TSAP implementation nulo, TSAP incorrectamente inicializadas\n"));

		return M4_FALSE;
	}
	pInetAddress = m_poTSAPImpl->GetInetAddress();
	if (pInetAddress == NULL)
		return M4_FALSE;

	return pInetAddress->GetHostname(ai_szAddress, ai_iMaxLen);

  //## end ClTSAP::GetRemoteHostname%-1762171823.body
}

const ClInetAddress* ClTSAP::GetRemoteInetAddress () const
{
  //## begin ClTSAP::GetRemoteInetAddress%218218125.body preserve=yes
	if (m_poTSAPImpl != NULL)
		return m_poTSAPImpl->GetInetAddress();

	return NULL;
  //## end ClTSAP::GetRemoteInetAddress%218218125.body
}

// Class ClServerTSAPSelectable 



// ClControlWriteTSAP     ---------------------------

ClControlWriteTSAP::ClControlWriteTSAP(): ClTSAP() {
	m_bIsSet = M4_FALSE;
	m_Mutex.Init();
}

int ClControlWriteTSAP::Write (const void* ai_pcSrc, int ai_iLen) {
	ClMutBlock oBlock(&m_Mutex);
	if (m_bIsSet) {
		return ai_iLen;
	}
	
	m_bIsSet = M4_TRUE;
	return ((ClTSAP*)this)->Write(ai_pcSrc,ai_iLen);
}

int ClControlWriteTSAP::Read (ClTSAP *ai_poTSAP, void* ao_pcDst, int ai_iLen, m4bool_t ai_bTotal) {
	ClMutBlock oBlock(&m_Mutex);
	if (m_bIsSet == M4_FALSE) {
		return 0;
	}
	
	m_bIsSet = M4_FALSE;
	return ai_poTSAP->Read(ao_pcDst,ai_iLen,ai_bTotal);
	
}

// End ClControlTSAP ---------------------------

// ClControlReadTSAP ---------------------------

ClControlReadTSAP::ClControlReadTSAP (int ai_iMode, void* ai_pData) : ClTSAP(ai_iMode,ai_pData){
}

ClControlReadTSAP::ClControlReadTSAP() : ClTSAP() {
}

int ClControlReadTSAP::Read (void* ao_pcDst, int ai_iLen, m4bool_t ai_bTotal) {
	return m_poWriteTSAP->Read(this, ao_pcDst, ai_iLen, ai_bTotal);
}

void ClControlReadTSAP::SetWriteTSAP(ClControlWriteTSAP *ai_poWriteTSAP) {
	m_poWriteTSAP = ai_poWriteTSAP;
}

// End ClControlReadTSAP ---------------------------


ClServerTSAPSelectable::ClServerTSAPSelectable (int ai_iPort, m4bool_t ai_bStream, int ai_iBacklog)
  //## begin ClServerTSAPSelectable::ClServerTSAPSelectable%1968226705.hasinit preserve=no
  //## end ClServerTSAPSelectable::ClServerTSAPSelectable%1968226705.hasinit
  //## begin ClServerTSAPSelectable::ClServerTSAPSelectable%1968226705.initialization preserve=yes
:ClTSAP()
  //## end ClServerTSAPSelectable::ClServerTSAPSelectable%1968226705.initialization
{
  //## begin ClServerTSAPSelectable::ClServerTSAPSelectable%1968226705.body preserve=yes
	m_bStream = ai_bStream;
	m_iBackLog = ai_iBacklog;

    // bug 0098015.
    m_bSSL = M4_FALSE;
	
	if (ClTSAP::Factory() == NULL)
	{
		m4Trace(fprintf(stderr, "ClTSAP::Factory() es NULL\n"));

		return;
	}
	
	if (m_poTSAPImpl->Create(ai_bStream) < 0 )
	{
		m_bError = M4_TRUE;

		return;
	}
	if (m_poTSAPImpl->Bind(NULL, ai_iPort) < 0)
	{
		m_bError = M4_TRUE;

		return;
	}

  //## end ClServerTSAPSelectable::ClServerTSAPSelectable%1968226705.body
}

ClServerTSAPSelectable::ClServerTSAPSelectable (ClInetAddress* ai_poInetAddress, int ai_iPort, m4bool_t ai_bStream, int ai_bBacklog)
  //## begin ClServerTSAPSelectable::ClServerTSAPSelectable%713160224.hasinit preserve=no
  //## end ClServerTSAPSelectable::ClServerTSAPSelectable%713160224.hasinit
  //## begin ClServerTSAPSelectable::ClServerTSAPSelectable%713160224.initialization preserve=yes
:m_bSSL(M4_FALSE), ClTSAP()
  //## end ClServerTSAPSelectable::ClServerTSAPSelectable%713160224.initialization
{
  //## begin ClServerTSAPSelectable::ClServerTSAPSelectable%713160224.body preserve=yes
	if (ClTSAP::Factory() == NULL)
	{
		m4Trace(fprintf(stderr, "ClTSAP::Factory() es NULL\n"));

		return;
	}
	m_bStream = ai_bStream;
	m_iBackLog = ai_bBacklog;
	if (m_poTSAPImpl->Create(ai_bStream) < 0)
	{
		m_bError = M4_TRUE;

		return;
	}
	if (m_poTSAPImpl->Bind(ai_poInetAddress, ai_iPort) < 0)
	{
		m_bError = M4_TRUE;

		return;
	}
  //## end ClServerTSAPSelectable::ClServerTSAPSelectable%713160224.body
}
ClServerTSAPSelectable::ClServerTSAPSelectable (int ai_iMode, void* ai_pData, int ai_iPort, m4bool_t ai_bStream, int ai_iBacklog):
					m_iMode(ai_iMode),m_bSSL(M4_TRUE), m_pData(ai_pData), ClTSAP(ai_iMode, ai_pData)
{
	if (ClTSAP::Factory() == NULL)
	{
		m4Trace(fprintf(stderr, "ClTSAP::Factory() es NULL\n"));

		return;
	}
	m_bStream = ai_bStream;
	m_iBackLog = ai_iBacklog;
	
	if (m_poTSAPImpl->Create(ai_bStream) < 0)
	{
		m_bError = M4_TRUE;

		return;
	}
	if (m_poTSAPImpl->Bind(NULL, ai_iPort) < 0) 
	{
		m_bError = M4_TRUE;

		return;
	}
}


//## Other Operations (implementation)
ClTSAP* ClServerTSAPSelectable::Accept ()
{
  //## begin ClServerTSAPSelectable::Accept%1231405844.body preserve=yes
	if (ClTSAP::Factory() == NULL)
	{
		m4Trace(fprintf(stderr, "ClTSAP::Factory() es NULL\n"));

		return NULL;
	}
	ClTSAP* poTSAP;
	if (m_bSSL == M4_TRUE)
	{
		if ((poTSAP = new ClTSAP(m_iMode, m_pData)) == NULL)
		{
			return NULL;
		}
		if (poTSAP->Error() == M4_TRUE)
		{
			delete poTSAP;

			return NULL;
		}

	}
	else
	if ((poTSAP = new ClTSAP())  == NULL)
	{
		return NULL;
	}
    if ( m_poTSAPImpl->Accept(poTSAP->GetImplementation()) < 0)
	{
		delete poTSAP;
		
		return NULL;
	}
	poTSAP->SetConnected();
	
	return poTSAP;
  //## end ClServerTSAPSelectable::Accept%1231405844.body
}

m4return_t ClServerTSAPSelectable::Listen ()
{
  //## begin ClServerTSAPSelectable::Listen%610999058.body preserve=yes
 	if (ClTSAP::Factory() == NULL)
	{
		m4Trace(fprintf(stderr, "ClTSAP::Factory() es NULL\n"));

		return M4_ERROR;
	}
	if (m_poTSAPImpl->Listen(m_iBackLog) == 0)
		return M4_SUCCESS;

	return M4_ERROR;
  //## end ClServerTSAPSelectable::Listen%610999058.body
}

//## begin module%377768480284.epilog preserve=yes
//## end module%377768480284.epilog
