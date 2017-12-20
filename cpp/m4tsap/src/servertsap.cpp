//## begin module%37776848017F.cm preserve=no
//## end module%37776848017F.cm

//## begin module%37776848017F.cp preserve=no
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
//## end module%37776848017F.cp

//## Module: servertsap%37776848017F; Package body
//## Subsystem: M4Tsap::src%3777684800C1
//	d:\compon\m4tsap\version\src
//## Source file: d:\Datos\m4tsap\version\src\servertsap.cpp

//## begin module%37776848017F.additionalIncludes preserve=no
//## end module%37776848017F.additionalIncludes

//## begin module%37776848017F.includes preserve=yes
#include <stdio.h>

#include "tsap.hpp"
#include "tsapimpl.hpp"
#include "tsapimplfactory.hpp"
#include "m4trace.hpp"





//## end module%37776848017F.includes

// m4types
#include <m4types.hpp>


//## begin module%37776848017F.declarations preserve=no
//## end module%37776848017F.declarations

//## begin module%37776848017F.additionalDeclarations preserve=yes
//## end module%37776848017F.additionalDeclarations


// Class ClServerTSAP 










ClServerTSAP::ClServerTSAP (int ai_iPort, m4bool_t ai_bStream, int ai_iBacklog)
  //## begin ClServerTSAP::ClServerTSAP%1113354433.hasinit preserve=no
  //## end ClServerTSAP::ClServerTSAP%1113354433.hasinit
  //## begin ClServerTSAP::ClServerTSAP%1113354433.initialization preserve=yes
  //## end ClServerTSAP::ClServerTSAP%1113354433.initialization
{
  //## begin ClServerTSAP::ClServerTSAP%1113354433.body preserve=yes
	#ifdef DEBUG
  printf("ClServerTSAP::ClServerTSAP-1\n  this = 0x%08x\n  port = %d\n",
	 (unsigned int) this, port);
  fflush(stdout);
#endif
  m_bError = M4_FALSE;
  m_bStream = ai_bStream;
  m_iBackLog = ai_iBacklog;
  m_bSSLMode = M4_FALSE;

  if (ClTSAP::Factory() == NULL)
  {
	  m4Trace(fprintf(stderr, "ClTSAP::Factory() es NULL\n"));
	  return;
  }

  if ((m_poTSAPImpl = ClTSAP::Factory()->CreateTSAPImpl()) == NULL ||
      m_poTSAPImpl->Create(ai_bStream) < 0 ||
      m_poTSAPImpl->Bind(NULL, ai_iPort) < 0) {
    m_bError = M4_TRUE;
  }

#ifdef DEBUG
  printf("ClServerTSAP::ClServerTSAP-1-s\n");
  fflush(stdout);
#endif

  //## end ClServerTSAP::ClServerTSAP%1113354433.body
}

ClServerTSAP::ClServerTSAP (ClInetAddress* ai_poInetAddress, int ai_iPort, m4bool_t ai_bStream, int ai_bBacklog)
  //## begin ClServerTSAP::ClServerTSAP%-1603296603.hasinit preserve=no
  //## end ClServerTSAP::ClServerTSAP%-1603296603.hasinit
  //## begin ClServerTSAP::ClServerTSAP%-1603296603.initialization preserve=yes
  //## end ClServerTSAP::ClServerTSAP%-1603296603.initialization
{
  //## begin ClServerTSAP::ClServerTSAP%-1603296603.body preserve=yes
#ifdef DEBUG
  printf("ClServerTSAP::ClServerTSAP-2\n  this = 0x%08x\n  port = %d\n",
	 (unsigned int) this, ai_iPort);
  fflush(stdout);
#endif

  m_bError = M4_FALSE;
  m_bStream = ai_bStream;
  m_iBackLog = ai_bBacklog;
   m_bSSLMode = M4_FALSE;
  if ((m_poTSAPImpl = ClTSAP::Factory()->CreateTSAPImpl()) == NULL ||
      m_poTSAPImpl->Create(ai_bStream) < 0 ||
      m_poTSAPImpl->Bind(ai_poInetAddress, ai_iPort) < 0) {
    m_bError = M4_TRUE;
  }
  //## end ClServerTSAP::ClServerTSAP%-1603296603.body
}

ClServerTSAP::ClServerTSAP (int ai_iMode, void* ai_pData, int ai_iPort, m4bool_t ai_bStream, int ai_iBacklog)
  //## begin ClServerTSAP::ClServerTSAP%-326820959.hasinit preserve=no
  //## end ClServerTSAP::ClServerTSAP%-326820959.hasinit
  //## begin ClServerTSAP::ClServerTSAP%-326820959.initialization preserve=yes
  //## end ClServerTSAP::ClServerTSAP%-326820959.initialization
{
  //## begin ClServerTSAP::ClServerTSAP%-326820959.body preserve=yes
  m_bError = M4_FALSE;
  m_bStream = ai_bStream;
  m_iBackLog = ai_iBacklog;
  m_bSSLMode = M4_TRUE;
  m_iMode = ai_iMode;
  m_pData = ai_pData;

  if ((m_poTSAPImpl = ClTSAP::Factory()->CreateTSAPSSLImpl(ai_iMode, ai_pData)) == NULL ||
      m_poTSAPImpl->Create(ai_bStream) < 0 ||
      m_poTSAPImpl->Bind(NULL, ai_iPort) < 0) {
    m_bError = M4_TRUE;
  }

  //## end ClServerTSAP::ClServerTSAP%-326820959.body
}


ClServerTSAP::~ClServerTSAP ()
{
  //## begin ClServerTSAP::~ClServerTSAP%-1476029477.body preserve=yes
	if (m_poTSAPImpl != NULL)
		delete m_poTSAPImpl;
  //## end ClServerTSAP::~ClServerTSAP%-1476029477.body
}



//## Other Operations (implementation)
void ClServerTSAP::Close ()
{
  //## begin ClServerTSAP::Close%-76419223.body preserve=yes
  m_poTSAPImpl->Close();
  //## end ClServerTSAP::Close%-76419223.body
}

int ClServerTSAP::GetLocalPort () const
{
  //## begin ClServerTSAP::GetLocalPort%1085902291.body preserve=yes
  return m_poTSAPImpl->GetLocalPort();
  //## end ClServerTSAP::GetLocalPort%1085902291.body
}

char* ClServerTSAP::ToString () const
{
  //## begin ClServerTSAP::ToString%1569814343.body preserve=yes
  return "ClServerTSAP::ToString no implementado";
  //## end ClServerTSAP::ToString%1569814343.body
}

m4bool_t ClServerTSAP::Error () const
{
  //## begin ClServerTSAP::Error%-496976369.body preserve=yes
  return m_bError;
  //## end ClServerTSAP::Error%-496976369.body
}

int ClServerTSAP::GetError () const
{
  //## begin ClServerTSAP::GetError%1686994143.body preserve=yes
	if (m_poTSAPImpl)
		return m_poTSAPImpl->GetError();

	return M4TSAP_NOT_INITIALIZED;
  //## end ClServerTSAP::GetError%1686994143.body
}

char* ClServerTSAP::GetStringError () const
{
  //## begin ClServerTSAP::GetStringError%-626908055.body preserve=yes
	if (m_poTSAPImpl)
		return m_poTSAPImpl->GetStringError();

	return M4TSAP_STR_NOT_INITIALIZED;
  //## end ClServerTSAP::GetStringError%-626908055.body
}

ClTSAP* ClServerTSAP::Accept (m4bool_t ai_bInitialize)
{
  //## begin ClServerTSAP::Accept%-12515548.body preserve=yes
	int ret;
	ClTSAP* poTSAP;
#ifdef DEBUG
  printf("ClServerTSAP::Accept\n"); fflush(stdout);
  printf("  oImpl = 0x%08lx\n", (unsigned int) oImpl); fflush(stdout);
#endif
	if (m_bSSLMode == M4_TRUE)
	{
		if ((poTSAP = new ClTSAP(m_iMode, m_pData)) == NULL)
		{
			m_bError = M4_TRUE;
			return NULL;
		}
		if (poTSAP->Error() == M4_TRUE)
		{
			delete poTSAP;
			m_bError = M4_TRUE;
			return NULL;
		}

	}
	else
	if ((poTSAP = new ClTSAP())  == NULL)
	{
		m_bError = M4_TRUE;
		return NULL;
	}
	if (ai_bInitialize == M4_FALSE && (ret = m_poTSAPImpl->Listen(m_iBackLog)) < 0)
	{
	    delete poTSAP;
	    m_bError = M4_TRUE;
	    return NULL;
	}
    if ( m_poTSAPImpl->Accept(poTSAP->GetImplementation()) < 0)
	{
#ifdef DEBUG
    printf("Saliendo de ClServerTSAP::Accept\n"); fflush(stdout);
#endif
    delete poTSAP;
    m_bError = M4_TRUE;
    return NULL;
  }
	poTSAP->SetConnected();

  return poTSAP;
  //## end ClServerTSAP::Accept%-12515548.body
}

// Bug 0130800
ClTSAP* ClServerTSAP::AcceptControl (m4bool_t ai_bInitialize)
{
  //## begin ClServerTSAP::Accept%-12515548.body preserve=yes
	int ret;
	ClControlReadTSAP* poTSAP;
#ifdef DEBUG
  printf("ClServerTSAP::Accept\n"); fflush(stdout);
  printf("  oImpl = 0x%08lx\n", (unsigned int) oImpl); fflush(stdout);
#endif
	if (m_bSSLMode == M4_TRUE)
	{
		if ((poTSAP = new ClControlReadTSAP(m_iMode, m_pData)) == NULL)
		{
			m_bError = M4_TRUE;
			return NULL;
		}
		if (poTSAP->Error() == M4_TRUE)
		{
			delete poTSAP;
			m_bError = M4_TRUE;
			return NULL;
		}

	}
	else
	if ((poTSAP = new ClControlReadTSAP())  == NULL)
	{
		m_bError = M4_TRUE;
		return NULL;
	}
	if (ai_bInitialize == M4_FALSE && (ret = m_poTSAPImpl->Listen(m_iBackLog)) < 0)
	{
	    delete poTSAP;
	    m_bError = M4_TRUE;
	    return NULL;
	}
    if ( m_poTSAPImpl->Accept(poTSAP->GetImplementation()) < 0)
	{
#ifdef DEBUG
    printf("Saliendo de ClServerTSAP::Accept\n"); fflush(stdout);
#endif
    delete poTSAP;
    m_bError = M4_TRUE;
    return NULL;
  }
	poTSAP->SetConnected();

  return poTSAP;
  //## end ClServerTSAP::Accept%-12515548.body
}

const ClInetAddress* ClServerTSAP::GetInetAddress () const
{
  //## begin ClServerTSAP::GetInetAddress%531080583.body preserve=yes
  return m_poTSAPImpl->GetInetAddress();
  //## end ClServerTSAP::GetInetAddress%531080583.body
}

int ClServerTSAP::GetSystemDescriptor ()
{
  //## begin ClServerTSAP::GetSystemDescriptor%939982877.body preserve=yes
	if (m_poTSAPImpl)
		return m_poTSAPImpl->GetSystemDescriptor();

	return -1;
  //## end ClServerTSAP::GetSystemDescriptor%939982877.body
}

//## begin module%37776848017F.epilog preserve=yes
//## end module%37776848017F.epilog
