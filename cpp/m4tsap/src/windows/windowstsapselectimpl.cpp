//## begin module%377768490004.cm preserve=no
//## end module%377768490004.cm

//## begin module%377768490004.cp preserve=no
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
//## end module%377768490004.cp

//## Module: windowstsapselectimpl%377768490004; Package body
//## Subsystem: M4Tsap::src::win32%377768480360
//	d:\compon\m4tsap\version\src\win32
//## Source file: d:\compon\m4tsap\version\src\win32\windowstsapselectimpl.cpp

//## begin module%377768490004.additionalIncludes preserve=no
//## end module%377768490004.additionalIncludes

//## begin module%377768490004.includes preserve=yes
#include <sys/types.h>
#include "tsap.hpp"

#include <m4winsock.hpp>

#include "windowstsapselectimpl.hpp"
#include "windowstsapimpl.hpp"


//## end module%377768490004.includes

// windowsselectmask
#include <windowsselectmask.hpp>
// m4types
#include <m4types.hpp>
// windowstsapselectimpl
#include <windowstsapselectimpl.hpp>
// selectimpl
#include <selectimpl.hpp>
// syncro
#include <syncro.hpp>
//## begin module%377768490004.declarations preserve=no
//## end module%377768490004.declarations

//## begin module%377768490004.additionalDeclarations preserve=yes
//## end module%377768490004.additionalDeclarations


// Class ClWindowsTSAPSelectImpl 












ClWindowsTSAPSelectImpl::ClWindowsTSAPSelectImpl (unsigned int nTSAPs)
  //## begin ClWindowsTSAPSelectImpl::ClWindowsTSAPSelectImpl%-568568961.hasinit preserve=no
  //## end ClWindowsTSAPSelectImpl::ClWindowsTSAPSelectImpl%-568568961.hasinit
  //## begin ClWindowsTSAPSelectImpl::ClWindowsTSAPSelectImpl%-568568961.initialization preserve=yes
  //## end ClWindowsTSAPSelectImpl::ClWindowsTSAPSelectImpl%-568568961.initialization
{
  //## begin ClWindowsTSAPSelectImpl::ClWindowsTSAPSelectImpl%-568568961.body preserve=yes
	iNumTSAPs= FD_SETSIZE;

  m_poReadMask = new ClWindowsSelectMask(iNumTSAPs);

  m_poWriteMask = new ClWindowsSelectMask(iNumTSAPs);

  m_poExceptionsMask = new ClWindowsSelectMask(iNumTSAPs);

  m_poMasterReadMask = new ClWindowsSelectMask(iNumTSAPs);

  m_poMasterWriteMask = new ClWindowsSelectMask(iNumTSAPs);

  m_poMasterExceptionsMask = new ClWindowsSelectMask(iNumTSAPs);

  //## end ClWindowsTSAPSelectImpl::ClWindowsTSAPSelectImpl%-568568961.body
}


ClWindowsTSAPSelectImpl::~ClWindowsTSAPSelectImpl ()
{
  //## begin ClWindowsTSAPSelectImpl::~ClWindowsTSAPSelectImpl%-1022061054.body preserve=yes
	if (m_poReadMask != NULL)
		delete m_poReadMask;

	if (m_poWriteMask != NULL)
		delete m_poWriteMask;

	if (m_poExceptionsMask)
		delete m_poExceptionsMask;

	if (m_poMasterReadMask != NULL)
		delete m_poMasterReadMask;

	if (m_poMasterWriteMask != NULL)
		delete m_poMasterWriteMask;

	if (m_poMasterExceptionsMask != NULL)
		delete m_poMasterExceptionsMask;
  //## end ClWindowsTSAPSelectImpl::~ClWindowsTSAPSelectImpl%-1022061054.body
}



//## Other Operations (implementation)
void ClWindowsTSAPSelectImpl::Add (ClTSAP* ai_poTSAP, m4bool_t ai_bRead, m4bool_t ai_bWrite, m4bool_t ai_bExceptions)
{
  //## begin ClWindowsTSAPSelectImpl::Add%688489493.body preserve=yes
	int iTSAPid= ai_poTSAP->GetImplementation()->GetSystemDescriptor();

	m_oaTSAPList[iTSAPid] = ai_poTSAP;

  if (ai_bRead)
  	  m_poMasterReadMask->Set(iTSAPid);

  if (ai_bWrite)
    m_poMasterWriteMask-> Set(iTSAPid);

  if (ai_bExceptions)
    m_poMasterExceptionsMask-> Set(iTSAPid);

  //## end ClWindowsTSAPSelectImpl::Add%688489493.body
}

m4return_t ClWindowsTSAPSelectImpl::Remove (ClTSAP* ai_poTSAP, m4bool_t ai_bRead, m4bool_t ai_bWrite,m4bool_t ai_bExceptions)
{
  //## begin ClWindowsTSAPSelectImpl::Remove%-312137576.body preserve=yes

	int	s = ai_poTSAP->GetImplementation()->GetSystemDescriptor();
	if (m_oaTSAPList[s] == NULL)
		return M4_ERROR;


	if (ai_bRead)
		m_poMasterReadMask->Clear(s);
	if (ai_bWrite)
		m_poMasterWriteMask->Clear(s);
	if (ai_bExceptions)
		m_poMasterExceptionsMask->Clear(s);

	if (!m_poMasterReadMask->IsIn(s) &&
		!m_poMasterWriteMask->IsIn(s) &&
		!m_poMasterExceptionsMask->IsIn(s))
		m_oaTSAPList[s] = NULL;
	
	return M4_SUCCESS;
  //## end ClWindowsTSAPSelectImpl::Remove%-312137576.body
}

m4return_t ClWindowsTSAPSelectImpl::Remove (ClTSAP* s)
{
  //## begin ClWindowsTSAPSelectImpl::Remove%-312137576.body preserve=yes

	ClWindowsTSAPImpl* oTSAPImpl = (ClWindowsTSAPImpl*) s->GetImplementation();
	if (m_oaTSAPList[oTSAPImpl->GetIdTSAP()] == NULL)
		return M4_ERROR;

	m_oaTSAPList[oTSAPImpl->GetIdTSAP()] = NULL;
	m_poMasterReadMask->Clear(oTSAPImpl->GetIdTSAP());
	m_poMasterWriteMask->Clear(oTSAPImpl-> GetIdTSAP () );
	m_poMasterExceptionsMask->Clear(oTSAPImpl->GetIdTSAP());
	
	return M4_SUCCESS;
  //## end ClWindowsTSAPSelectImpl::Remove%-312137576.body
}

int ClWindowsTSAPSelectImpl::Select (ClMutex* ao_pMutex, unsigned long ai_Segundos, unsigned long ai_Microsegundos, m4bool_t ai_bRead, m4bool_t ai_bWrite, m4bool_t ai_bExceptions)
{
  //## begin ClWindowsTSAPSelectImpl::Select%1685621972.body preserve=yes
	int	ret;
	struct	timeval	timeout;

	if ((ai_Segundos != 0L) || (ai_Microsegundos != 0L))
	{
		timeout.tv_sec = ai_Segundos;	
		timeout.tv_usec = ai_Microsegundos;	
	}

	if (ao_pMutex != NULL)
		ao_pMutex->Lock();
	*m_poReadMask = *m_poMasterReadMask;
	*m_poWriteMask = *m_poMasterWriteMask;
	*m_poExceptionsMask = *m_poMasterExceptionsMask;
	if (ao_pMutex != NULL)
		ao_pMutex->Unlock();

  // XXX Habria que comprobar que el select no devuelve error. Ademas, el
  // valor devuelto puede usarse para optimizar las funciones de lectura
  // NextRead, NextWrite y NextExceptions.
	if ((ret = select( 0 ,
		(fd_set *) ((ai_bRead)? m_poReadMask -> GetMask() : NULL),
		(fd_set *) ((ai_bWrite)? m_poWriteMask -> GetMask(): NULL),
		(fd_set *) ((ai_bExceptions)? m_poExceptionsMask -> GetMask(): NULL),
		(ai_Segundos == 0L && ai_Microsegundos == 0L)? NULL: &timeout)) == SOCKET_ERROR)
		return -1;

  iNextRead = iNextWrite = iNextExceptions = 0;

  return ret;
  //## end ClWindowsTSAPSelectImpl::Select%1685621972.body
}

ClTSAP* ClWindowsTSAPSelectImpl::NextRead ()
{
  //## begin ClWindowsTSAPSelectImpl::NextRead%1071327791.body preserve=yes
	int s;

  s = m_poReadMask->NextSet(iNextRead);

  if (s == -1 || m_poMasterReadMask->IsIn(s) == M4_FALSE)
  {
    return NULL;
  }

  return m_oaTSAPList[s];

  //## end ClWindowsTSAPSelectImpl::NextRead%1071327791.body
}

ClTSAP* ClWindowsTSAPSelectImpl::NextWrite ()
{
  //## begin ClWindowsTSAPSelectImpl::NextWrite%525744225.body preserve=yes
	  int s;

  s = m_poWriteMask->NextSet(iNextWrite);

  if (s == -1 || m_poMasterWriteMask->IsIn(s) == M4_FALSE)
  {
    return NULL;
  }


  return m_oaTSAPList[s];

  //## end ClWindowsTSAPSelectImpl::NextWrite%525744225.body
}

ClTSAP* ClWindowsTSAPSelectImpl::NextExceptions ()
{
  //## begin ClWindowsTSAPSelectImpl::NextExceptions%-1452603615.body preserve=yes
  int s;

  s = m_poExceptionsMask -> NextSet( iNextExceptions );
  if (s == -1 || m_poMasterExceptionsMask->IsIn(s) == M4_FALSE)
  {
    return NULL;
  }


  return m_oaTSAPList[s];
  //## end ClWindowsTSAPSelectImpl::NextExceptions%-1452603615.body
}

m4bool_t ClWindowsTSAPSelectImpl::IsInMask (ClTSAP* ai_poTSAP, m4int8_t ai_iTipoMascara)
{
  //## begin ClWindowsTSAPSelectImpl::IsInMask%-983664596.body preserve=yes
	ClWindowsTSAPImpl* poTSAPImpl = (ClWindowsTSAPImpl*) ai_poTSAP->GetImplementation();
	switch (ai_iTipoMascara)
	{
		case IS_IN_READ:
			if (m_poMasterReadMask == NULL)
				return M4_FALSE;

			return m_poMasterReadMask->IsIn(poTSAPImpl->GetIdTSAP());

		case IS_IN_WRITE:
			if (m_poMasterWriteMask == NULL)
				return M4_FALSE;

			return m_poMasterWriteMask->IsIn(poTSAPImpl->GetIdTSAP());

		case IS_IN_EXCEPTIONS:
			if (m_poMasterExceptionsMask == NULL)
				return M4_FALSE;

			return m_poMasterExceptionsMask->IsIn(poTSAPImpl->GetIdTSAP());
	}

	return M4_FALSE;
  //## end ClWindowsTSAPSelectImpl::IsInMask%-983664596.body
}

// Additional Declarations
  //## begin ClWindowsTSAPSelectImpl%377768450258.declarations preserve=yes
  //## end ClWindowsTSAPSelectImpl%377768450258.declarations

//## begin module%377768490004.epilog preserve=yes
//## end module%377768490004.epilog
