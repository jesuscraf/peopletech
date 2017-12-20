//## begin module%3777684800C2.cm preserve=no
//## end module%3777684800C2.cm

//## begin module%3777684800C2.cp preserve=no
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
//## end module%3777684800C2.cp

//## Module: inet%3777684800C2; Package body
//## Subsystem: M4Tsap::src%3777684800C1
//	d:\compon\m4tsap\version\src
//## Source file: d:\compon\m4tsap\version\src\inet.cpp

//## begin module%3777684800C2.additionalIncludes preserve=no
//## end module%3777684800C2.additionalIncludes

//## begin module%3777684800C2.includes preserve=yes




//	=========================================================
//	=====================
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
//	=========================================================
//	=====================










#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "m4winsock.hpp"
#ifdef _UNIX
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#endif

#include "tsap.hpp"
#include "m4trace.hpp"
//## end module%3777684800C2.includes

// m4types
#include <m4types.hpp>
//## begin module%3777684800C2.declarations preserve=no
//## end module%3777684800C2.declarations

//## begin module%3777684800C2.additionalDeclarations preserve=yes
//## end module%3777684800C2.additionalDeclarations


// Class ClInetAddress 


//## begin ClInetAddress::m_s_iAddrLen%37776844022C.role preserve=no  public: static m4size_t {1 -> 1VHAPN}
m4size_t ClInetAddress::m_s_iAddrLen = M4_SIZE_INET_ADDRESS;
//## end ClInetAddress::m_s_iAddrLen%37776844022C.role





ClInetAddress::ClInetAddress (const char* ai_pcHostname)
  //## begin ClInetAddress::ClInetAddress%40079955.hasinit preserve=no
  //## end ClInetAddress::ClInetAddress%40079955.hasinit
  //## begin ClInetAddress::ClInetAddress%40079955.initialization preserve=yes
  //## end ClInetAddress::ClInetAddress%40079955.initialization
{
  //## begin ClInetAddress::ClInetAddress%40079955.body preserve=yes
	// Siempre que se llame a este constructor, habrá que verificar
	// que es una dirección correcta	
	struct hostent* hp;
	if ((hp = gethostbyname(ai_pcHostname)) == NULL)
	{
		m_bCorrecta = M4_FALSE;	
	}
	else
	{
		memcpy(m_aAddress, hp->h_addr, m_s_iAddrLen);
		m_bCorrecta = M4_TRUE;	
	}
  //## end ClInetAddress::ClInetAddress%40079955.body
}

ClInetAddress::ClInetAddress (const m4uint8_t* ai_puiAddr)
  //## begin ClInetAddress::ClInetAddress%1461341715.hasinit preserve=no
  //## end ClInetAddress::ClInetAddress%1461341715.hasinit
  //## begin ClInetAddress::ClInetAddress%1461341715.initialization preserve=yes
  //## end ClInetAddress::ClInetAddress%1461341715.initialization
{
  //## begin ClInetAddress::ClInetAddress%1461341715.body preserve=yes
	m_bCorrecta = M4_TRUE;
	memcpy(m_aAddress, ai_puiAddr, m_s_iAddrLen);
  //## end ClInetAddress::ClInetAddress%1461341715.body
}



//## Other Operations (implementation)
const m4uint8_t* ClInetAddress::GetAddress () const
{
  //## begin ClInetAddress::GetAddress%-1928642072.body preserve=yes
	return m_aAddress;
  //## end ClInetAddress::GetAddress%-1928642072.body
}

m4bool_t ClInetAddress::IsRight () const
{
  //## begin ClInetAddress::IsRight%928100026.body preserve=yes
	return m_bCorrecta;
  //## end ClInetAddress::IsRight%928100026.body
}

m4bool_t ClInetAddress::GetIP (m4pchar_t ai_szAddress, int ai_iMaxLen)
{
  //## begin ClInetAddress::GetIP%-1379364228.body preserve=yes
 	if (ai_iMaxLen < 16)
	{
		m4Trace(fprintf(stderr, "Error en GetIP: parámetro ai_iMaxLen %d demasiado pequeño\n", ai_iMaxLen));

		return M4_FALSE;
	}
 	sprintf(ai_szAddress, "%d.%d.%d.%d", m_aAddress[0], m_aAddress[1], m_aAddress[2], m_aAddress[3]);
	
	return M4_TRUE;
  //## end ClInetAddress::GetIP%-1379364228.body
}

m4bool_t ClInetAddress::GetHostname (m4pchar_t ai_szHostName, int ai_iMaxLen)
{
  //## begin ClInetAddress::GetHostname%1323407581.body preserve=yes
 	struct hostent *pHost;

/*	struct hostent result;
        char buffer[4000];
        int h_errnop;
	if ((pHost = gethostbyaddr_r (( const char  *)&m_aAddress, sizeof(m_aAddress), AF_INET ,&result,buffer,sizeof(buffer),&h_errnop)) == NULL)*/
	if ((pHost = gethostbyaddr (( const char  *)&m_aAddress, sizeof(m_aAddress), AF_INET )) == NULL)
	{
		if (h_errno==-1)
		{	m4Trace(fprintf(stderr,"h_errno=-1,errno=%d\n",errno));}
		else
		{	m4Trace(fprintf(stderr,"h_errno=%d\n",h_errno));}
		return GetIP(ai_szHostName, ai_iMaxLen);
	}
	
	if ((size_t)ai_iMaxLen < strlen(pHost->h_name))
	{
		m4Trace(fprintf(stderr, "Error en GetIP: parámetro ai_iMaxLen %d demasiado pequeño\n", ai_iMaxLen));
		m4Trace(fprintf(stderr, "El nombre es %s\n", pHost->h_name));

		return M4_FALSE;
	}
	strcpy(ai_szHostName, pHost->h_name);

	return M4_TRUE;
  //## end ClInetAddress::GetHostname%1323407581.body
}

//## begin module%3777684800C2.epilog preserve=yes
//## end module%3777684800C2.epilog
