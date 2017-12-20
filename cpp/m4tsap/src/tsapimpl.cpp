//## begin module%3777684802E8.cm preserve=no
//## end module%3777684802E8.cm

//## begin module%3777684802E8.cp preserve=no
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
//## end module%3777684802E8.cp

//## Module: tsapimpl%3777684802E8; Package body
//## Subsystem: M4Tsap::src%3777684800C1
//	d:\compon\m4tsap\version\src
//## Source file: d:\compon\m4tsap\version\src\tsapimpl.cpp

//## begin module%3777684802E8.additionalIncludes preserve=no
//## end module%3777684802E8.additionalIncludes

//## begin module%3777684802E8.includes preserve=yes
#include "tsap.hpp"
#include "tsapimpl.hpp"
#include <stdlib.h>

//## end module%3777684802E8.includes

// m4types
#include <m4types.hpp>
//## begin module%3777684802E8.declarations preserve=no
//## end module%3777684802E8.declarations

//## begin module%3777684802E8.additionalDeclarations preserve=yes
//## end module%3777684802E8.additionalDeclarations


// Class ClTSAPImpl 








ClTSAPImpl::ClTSAPImpl ()
  //## begin ClTSAPImpl::ClTSAPImpl%-660507616.hasinit preserve=no
  //## end ClTSAPImpl::ClTSAPImpl%-660507616.hasinit
  //## begin ClTSAPImpl::ClTSAPImpl%-660507616.initialization preserve=yes
  //## end ClTSAPImpl::ClTSAPImpl%-660507616.initialization
{
  //## begin ClTSAPImpl::ClTSAPImpl%-660507616.body preserve=yes
  m_poInetAddress = NULL;
  m_iPort = m_iLocalPort = 0;
  m_iError = M4TSAP_OK;
  //## end ClTSAPImpl::ClTSAPImpl%-660507616.body
}


ClTSAPImpl::~ClTSAPImpl ()
{
  //## begin ClTSAPImpl::~ClTSAPImpl%-1398153073.body preserve=yes
	if (m_poInetAddress != NULL)
	  delete m_poInetAddress;
  //## end ClTSAPImpl::~ClTSAPImpl%-1398153073.body
}



//## Other Operations (implementation)
ClInetAddress* ClTSAPImpl::GetInetAddress () const
{
  //## begin ClTSAPImpl::GetInetAddress%-200261066.body preserve=yes
  return m_poInetAddress;
  //## end ClTSAPImpl::GetInetAddress%-200261066.body
}

int ClTSAPImpl::GetLocalPort () const
{
  //## begin ClTSAPImpl::GetLocalPort%-595919322.body preserve=yes
  return m_iLocalPort;
  //## end ClTSAPImpl::GetLocalPort%-595919322.body
}

int ClTSAPImpl::GetPort () const
{
  //## begin ClTSAPImpl::GetPort%151074921.body preserve=yes
  return m_iPort;
  //## end ClTSAPImpl::GetPort%151074921.body
}

char* ClTSAPImpl::ToString () const
{
  //## begin ClTSAPImpl::ToString%-503072780.body preserve=yes
  // XXX Chapuza de kilo
  return NULL;
  //## end ClTSAPImpl::ToString%-503072780.body
}

int ClTSAPImpl::GetError ()
{
  //## begin ClTSAPImpl::GetError%116183132.body preserve=yes

	return m_iError;
  //## end ClTSAPImpl::GetError%116183132.body
}

char* ClTSAPImpl::GetStringError ()
{
  //## begin ClTSAPImpl::GetStringError%-2079943342.body preserve=yes
	switch (m_iError)
	{
	case	M4TSAP_OK:
			return M4TSAP_STR_OK;
	case	M4TSAP_NOT_INITIALIZED:
			return M4TSAP_STR_NOT_INITIALIZED;
	case	M4TSAP_NOT_IMPLEMENTED:
			return M4TSAP_STR_NOT_IMPLEMENTED;
	case	M4TSAP_NOT_KNOWN:
			return M4TSAP_STR_NOT_KNOWN;
	case	M4TSAP_SOCKET_FAILED:
			return M4TSAP_STR_SOCKET_FAILED;
	case	M4TSAP_BIND_FAILED:
			return M4TSAP_STR_BIND_FAILED;
	case	M4TSAP_ACCEPT_FAILED:
			return M4TSAP_STR_ACCEPT_FAILED;
	case	M4TSAP_CONNECT_FAILED:
			return M4TSAP_STR_CONNECT_FAILED;
	case	M4TSAP_LISTEN_FAILED:
			return M4TSAP_STR_LISTEN_FAILED;
	case	M4TSAP_WRITE_FAILED:
			return M4TSAP_STR_WRITE_FAILED;
	case	M4TSAP_READ_FAILED:
			return M4TSAP_STR_READ_FAILED;
	case	M4TSAP_INVALID_HOSTNAME:
			return M4TSAP_STR_INVALID_HOSTNAME;
	case	M4TSAP_INVALID_INET_ADDRESS:
			return M4TSAP_STR_INVALID_INET_ADDRESS;
	case	M4TSAP_CERTIFICATE_CA:
			return M4TSAP_STR_CERTIFICATE_CA;
	case	M4TSAP_INVALID_PRIVATE_KEY:
			return M4TSAP_STR_INVALID_PRIVATE_KEY;
	case	M4TSAP_INVALID_CERTIFICATE:
			return M4TSAP_STR_INVALID_CERTIFICATE;
	case	M4TSAP_SSL_CLIENT_NOT_INITIALIZED:
			return M4TSAP_STR_SSL_CLIENT_NOT_INITIALIZED;
	case	M4TSAP_SSL_SERVER_NOT_INITIALIZED:
			return M4TSAP_STR_SSL_SERVER_NOT_INITIALIZED;
	case	M4TSAP_SSL_SSL_NEW_FAILED:
			return M4TSAP_STR_SSL_SSL_NEW_FAILED;
	case	M4TSAP_SSL_SET_CIPHER_LIST_FAILED:
			return M4TSAP_STR_SSL_SET_CIPHER_LIST_FAILED;
	case	M4TSAP_SSL_ACCEPT_FAILED:
			return M4TSAP_STR_SSL_ACCEPT_FAILED;
	case	M4TSAP_SSL_CONNECT_FAILED:
			return M4TSAP_STR_SSL_CONNECT_FAILED;
	case	M4TSAP_SSL_GET_PEER_CERTIFICATE_FAILED:
			return M4TSAP_STR_SSL_GET_PEER_CERTIFICATE_FAILED;
	case	M4TSAP_SSL_X509_NAME_ONLINE_FAILED:
			return M4TSAP_STR_SSL_X509_NAME_ONLINE_FAILED;
	case	M4TSAP_SSL_M_SSL_NOT_INITIALIZED:
			return M4TSAP_STR_SSL_M_SSL_NOT_INITIALIZED;
	case	M4TSAP_SSL_WRITE_FAILED:
			return M4TSAP_STR_SSL_WRITE_FAILED;
	case	M4TSAP_SSL_READ_FAILED:
			return M4TSAP_STR_SSL_READ_FAILED;
	default:
			return M4TSAP_STR_ERROR_INVEROSIMIL;
	}
  //## end ClTSAPImpl::GetStringError%-2079943342.body
}

// Additional Declarations
  //## begin ClTSAPImpl%3777684400C5.declarations preserve=yes
  //## end ClTSAPImpl%3777684400C5.declarations

//## begin module%3777684802E8.epilog preserve=yes
//## end module%3777684802E8.epilog
