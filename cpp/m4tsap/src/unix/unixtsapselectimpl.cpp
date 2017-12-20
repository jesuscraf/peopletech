#ifndef ROSE_ANALYZER
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#ifdef DEBUG
#include <stdio.h>
#include <errno.h>
#include <string.h>
#endif

#include "tsap.hpp"
#include "unixtsap.hpp"

ClUnixTSAPSelectImpl::ClUnixTSAPSelectImpl(unsigned int nTSAPs)
{
#ifndef M4_POLL_IMPLEMENTATION
  iNumTSAPs = (nTSAPs == 0 ? FD_SETSIZE : nTSAPs);
  oReadMask = new ClUnixSelectMask(iNumTSAPs);
  oWriteMask = new ClUnixSelectMask(iNumTSAPs);
  oExceptionsMask = new ClUnixSelectMask(iNumTSAPs);
  oMasterReadMask = new ClUnixSelectMask(iNumTSAPs);
  oMasterWriteMask = new ClUnixSelectMask(iNumTSAPs);
  oMasterExceptionsMask = new ClUnixSelectMask(iNumTSAPs);
  iNextRead = iNextWrite = iNextExceptions = 0;
  oaTSAPList = new ClTSAP *[iNumTSAPs *2];
#endif
}

void ClUnixTSAPSelectImpl::Add(ClTSAP* s, m4bool_t read,
			       m4bool_t write, m4bool_t exceptions)
{
#ifndef M4_POLL_IMPLEMENTATION
	oaTSAPList[s->GetImplementation()->GetSystemDescriptor()] = s;
	if (read)
		oMasterReadMask->Set(s->GetImplementation()->GetSystemDescriptor());
	if (write)
		oMasterWriteMask->Set(s->GetImplementation()->GetSystemDescriptor());
	if (exceptions)
		oMasterExceptionsMask->Set(s->GetImplementation()->GetSystemDescriptor());
#else
	int	iMask = 0;
	
	if (read == M4_TRUE)
		iMask |= M4_READ;
	
	if (write == M4_TRUE)
		iMask |= M4_WRITE;

	if (exceptions == M4_TRUE)
		iMask |= M4_EXCEPTIONS;
	
	SetPollFds.Set(s, iMask);
#endif

}

m4return_t ClUnixTSAPSelectImpl::Remove (ClTSAP* ai_poTSAP, m4bool_t ai_bRead, m4bool_t ai_bWrite,m4bool_t ai_bExceptions)
{

#ifndef M4_POLL_IMPLEMENTATION
  if (oaTSAPList[ai_poTSAP->GetImplementation()->GetSystemDescriptor()] == NULL)
	  return M4_FALSE;

  oaTSAPList[ai_poTSAP->GetImplementation()->GetSystemDescriptor()] = NULL;
  oMasterReadMask->Clear(ai_poTSAP->GetImplementation()->GetSystemDescriptor());
  oMasterWriteMask->Clear(ai_poTSAP->GetImplementation()->GetSystemDescriptor());
  oMasterExceptionsMask->Clear(ai_poTSAP->GetImplementation()->GetSystemDescriptor());

  return M4_SUCCESS;
#else
	return SetPollFds.Clear(ai_poTSAP,ai_bRead, ai_bWrite, ai_bExceptions);
#endif
}

m4return_t ClUnixTSAPSelectImpl::Remove(ClTSAP* s)
{

#ifndef M4_POLL_IMPLEMENTATION
  if (oaTSAPList[s->GetImplementation()->GetSystemDescriptor()] == NULL)
	  return M4_FALSE;

  oaTSAPList[s->GetImplementation()->GetSystemDescriptor()] = NULL;
  oMasterReadMask->Clear(s->GetImplementation()->GetSystemDescriptor());
  oMasterWriteMask->Clear(s->GetImplementation()->GetSystemDescriptor());
  oMasterExceptionsMask->Clear(s->GetImplementation()->GetSystemDescriptor());

  return M4_SUCCESS;
#else
	return SetPollFds.Clear(s);
#endif
}

int ClUnixTSAPSelectImpl::Select(ClMutex *ao_pMutex, unsigned long ai_Segundos, 
					unsigned long ai_Microsegundos,
					m4bool_t ai_bRead,
					m4bool_t ai_bWrite,
					m4bool_t ai_bExceptions)
{
#ifndef M4_POLL_IMPLEMENTATION
  int ret;
  struct  timeval timeout;
	  
  if ((ai_Segundos != 0L) || (ai_Microsegundos != 0L))
  {
	  timeout.tv_sec = ai_Segundos;   
	  timeout.tv_usec = ai_Microsegundos; 
  }


  if (ao_pMutex != NULL)
	  ao_pMutex->Lock();

  *oReadMask = *oMasterReadMask;
  *oWriteMask = *oMasterWriteMask;
  *oExceptionsMask = *oMasterExceptionsMask;
  if (ao_pMutex != NULL)
	  ao_pMutex->Unlock();

  // XXX Habria que comprobar que el select no devuelve error. Ademas, el
  // valor devuelto puede usarse para optimizar las funciones de lectura
  // NextRead, NextWrite y NextExceptions.
    if ((ret = select( iNumTSAPs, 
		(fd_set *) ((ai_bRead)? oReadMask -> GetMask() : NULL),
		(fd_set *) ((ai_bWrite)? oWriteMask -> GetMask(): NULL),
		(fd_set *) ((ai_bExceptions)? oExceptionsMask -> GetMask(): NULL), 
		(ai_Segundos == 0L && ai_Microsegundos == 0L)? NULL: &timeout)) == -1)
	return -1;

  iNextRead = iNextWrite = iNextExceptions = 0;

  return ret;

#else
	int iMillisecs = ai_Segundos * 1000 + ai_Microsegundos / 1000; 

	if (iMillisecs == 0)
		iMillisecs = -1;

	return SetPollFds.Poll(ao_pMutex, iMillisecs);

#endif
}

m4bool_t ClUnixTSAPSelectImpl::IsInMask(ClTSAP* ai_poTSAP, 
								m4int8_t ai_iTipoMascara)
{
#ifndef M4_POLL_IMPLEMENTATION
	switch (ai_iTipoMascara)
	{
		case IS_IN_READ:
			if (oMasterReadMask == NULL)
				return M4_FALSE;

				return oMasterReadMask->IsIn(ai_poTSAP->GetImplementation()->GetSystemDescriptor());

		case IS_IN_WRITE:
			if (oMasterWriteMask == NULL)
				return M4_FALSE;

			return oMasterWriteMask->IsIn(ai_poTSAP->GetImplementation()->GetSystemDescriptor());

		case IS_IN_EXCEPTIONS:
			if (oMasterExceptionsMask == NULL)
				return M4_FALSE;

			return oMasterExceptionsMask->IsIn(ai_poTSAP->GetImplementation()->GetSystemDescriptor());
	}

	return M4_FALSE;
#else
	int	iMask;
	switch (ai_iTipoMascara)
	{
		case IS_IN_READ:
			iMask = M4_READ;
			break;

		case IS_IN_WRITE:
			iMask = M4_WRITE;
			break;

		case IS_IN_EXCEPTIONS:
			iMask = M4_EXCEPTIONS;
			break;

		default:
			m4Trace(fprintf(stderr, "Error, modo %d no reconocible en  ClUnixTSAPSelectImpl::IsInMask\n", ai_iTipoMascara));
			return M4_FALSE;
	}

	return SetPollFds.IsIn(ai_poTSAP, iMask);
#endif
}

ClTSAP* ClUnixTSAPSelectImpl::NextRead()
{
#ifndef M4_POLL_IMPLEMENTATION
  int s;

  s = oReadMask->NextSet(iNextRead);
  if (s == -1 || m_poMasterReadMask->IsIn(s) == M4_FALSE)
  {
	  	iNextRead = 0;
   	 	return NULL;
	}
  iNextRead = s + 1;
  return oaTSAPList[s];

#else
  return SetPollFds.NextSet(M4_READ);
#endif
}

ClTSAP* ClUnixTSAPSelectImpl::NextWrite()
{
#ifndef M4_POLL_IMPLEMENTATION
  int s;

  s = oWriteMask->NextSet(iNextWrite);
  if (s == -1 || m_poMasterWriteMask->IsIn(s) == M4_FALSE)
  {

  	iNextWrite = 0;
    return NULL;
	}
  iNextWrite = s + 1;
  return oaTSAPList[s];
#else
  return SetPollFds.NextSet(M4_WRITE);
#endif
}

ClTSAP* ClUnixTSAPSelectImpl::NextExceptions()
{
#ifndef M4_POLL_IMPLEMENTATION
  int s;

  s = oExceptionsMask->NextSet(iNextExceptions);
  if (s == -1 || m_poMasterExceptionsMask->IsIn(s) == M4_FALSE)
  {
  	iNextExceptions = 0;
    return NULL;
	}
  iNextExceptions = s + 1;
  return oaTSAPList[s];
#else
  return SetPollFds.NextSet(M4_EXCEPTIONS);
#endif
}


