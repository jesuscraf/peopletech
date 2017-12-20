
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#ifdef M4_POLL_IMPLEMENTATION
#include "poll.h"
#endif

#include "tsap.hpp"
#include "unixtsap.hpp"

ClUnixSelectMask::ClUnixSelectMask(unsigned int size)
{
	iMaskSize = FD_SETSIZE;
	FD_ZERO(&ulaMask);
}

ClUnixSelectMask::~ClUnixSelectMask()
{
}

void ClUnixSelectMask::Set(unsigned int n)
{
	if (n > iMaskSize)
		return;

	FD_SET(n, &ulaMask);
	
}
void ClUnixSelectMask::Clear(unsigned int n)
{
	if (n > iMaskSize)
	    return;

	FD_CLR(n, &ulaMask);
}

m4bool_t ClUnixSelectMask::IsSet(unsigned int n) const
{
	int ret =  FD_ISSET(n, &ulaMask);

	return (ret != 0)?M4_TRUE:M4_FALSE;
}

int ClUnixSelectMask::NextSet(unsigned int n) const
{
  while (!IsSet(n) && n < (unsigned int)iMaskSize)
	  n++;

  if (n >= iMaskSize)
    return -1; // Hemos llegado al final de la mascara

  return n;
}

void ClUnixSelectMask::Zero()
{
}

unsigned int ClUnixSelectMask::GetSize() const
{
  return iMaskSize;
}

void ClUnixSelectMask::Resize(unsigned int newsize)
{

}

void* ClUnixSelectMask::GetMask()
{
	return (void*) &ulaMask;
}


ClUnixSelectMask& ClUnixSelectMask::operator=(const ClUnixSelectMask& src)
{
	memcpy(&ulaMask, &src.ulaMask, sizeof(ulaMask));

	return *this;
}

m4bool_t ClUnixSelectMask::IsIn(unsigned int ai_iIdentificador)
{
	return IsSet(ai_iIdentificador);
}


#ifdef M4_POLL_IMPLEMENTATION

ClUnixPollFds::ClUnixPollFds(int ai_iSize)
{
	m_iSize = ai_iSize;
	m_iNumFds = 0;
	m_iPositionRead = 0;
	m_iPositionWrite = 0;
	m_iPositionExceptions = 0;
	m_pPollFds = (struct pollfd *)calloc (sizeof(struct	pollfd), ai_iSize);
	m_pTSAPList = new ClTSAP *[ai_iSize];
	m_pChanges = (Changes_t *)calloc(MAX_CHANGES, sizeof(Changes_t));
	m_iChanges = 0;
}

ClUnixPollFds:: ~ClUnixPollFds()
{
	if (m_pChanges != NULL)
		free(m_pChanges);

	if (m_pPollFds != NULL)
		free(m_pPollFds);

	if (m_pTSAPList != NULL)
		delete m_pTSAPList;
}
void ClUnixPollFds::Set(ClTSAP* ai_pClTSAP, int ai_iMode)
{
	m4uint32_t i, position = -1;

	for ( i = 0 ; i < m_iChanges ; i++ )
	{
		if (m_pChanges [i].pClTSAP == ai_pClTSAP) {
			position = i;
			break;
		}
	}

	if (position == -1) {
		
		if (m_iChanges >= MAX_CHANGES) {
			m4Trace(fprintf(stderr, "Error. no hay espacio para más cambios en objeto select\n"));
			return;
		}

		position = m_iChanges++;
		m_pChanges[position].pClTSAP = ai_pClTSAP;
		m_pChanges[position].iMode = ai_iMode;
		m_pChanges[position].iChange = CHANGE_ADD;

		return;
	}

	// checks the delete mode
	if (m_pChanges[position].iChange == CHANGE_REMOVE) {
		return;
	}
	
	m_pChanges[position].iMode |= ai_iMode;
}

void ClUnixPollFds::InternalSet(ClTSAP* ai_pClTSAP, int ai_iMode)
{
	if (m_pPollFds == NULL || m_pTSAPList == NULL)
	{
		m4Trace(fprintf(stderr, "Error, new devolvió NULL en ClUnixPollFds::constructor\n"));

		return;
	}

	int	i;
	int	position;
	int	iMask;

	if (m_iNumFds >= m_iSize)
	{
		m4Trace(fprintf(stderr, "Error, número excesivo de fds en ClUnixPollFds::Set\n"));

		return;
	}
	for (i = 0, position = -1; i < m_iNumFds; i++)
	{
		if (m_pTSAPList[i] == ai_pClTSAP)
		{
			position = i;
			break;
		}
	}

	if (position == -1)
	{
		position = m_iNumFds++;
		m_pPollFds[position].fd = ai_pClTSAP->GetSystemDescriptor();
		m_pPollFds[position].events = 0;
		m_pPollFds[position].revents = 0;
		m_pTSAPList[position] = ai_pClTSAP;
	}

	iMask = 0;

	if (ai_iMode & M4_READ)
		iMask |= POLLRDNORM;
	
	if (ai_iMode & M4_WRITE)
		iMask |= POLLWRNORM;

	if (ai_iMode & M4_EXCEPTIONS)
		iMask |= POLLPRI;

	m_pPollFds[position].events |= iMask;
		
	if (iMask == 0)
			m4Trace(fprintf(stderr, "Error, modo %d no reconocible en  ClUnixPollFds::Set\n", ai_iMode));
}

//Tocado para Remove condicional
m4return_t ClUnixPollFds::Clear(ClTSAP* ai_pClTSAP, m4bool_t ai_bRead,m4bool_t ai_bWrite,m4bool_t ai_bExceptions)
{
	//Primero, comprobaciones de que  la TSAP sí está en el POLL
	if (m_pPollFds == NULL || m_pTSAPList == NULL)
	{
		m4Trace(fprintf(stderr, "Error, new devolvió NULL en ClUnixPollFds::constructor\n"));

		return M4_ERROR;
	}

	int	i, position=-1;

	// finds the TSAP if already inserted
	for (i = 0; i < m_iChanges; i++)
	{
		if ( m_pChanges[i].pClTSAP == ai_pClTSAP ) {
			position = i;
			break;
		}
	}

	if (position == -1) {

		if (m_iChanges >= MAX_CHANGES)
		{
			m4Trace(fprintf(stderr, "Error. no hay espacio para más cambios en objeto select\n"));
			return M4_ERROR;
		}

		position = m_iChanges++;
		m_pChanges[position].pClTSAP = ai_pClTSAP;
		m_pChanges[position].iMode = 0;
	}

	m_pChanges[position].iChange = CHANGE_REMOVE;

	if (ai_bRead) {
		// we detected a disconnection. All flags reseted and only the read is set
		m_pChanges[position].iMode = M4_READ | M4_WRITE | M4_EXCEPTIONS;
	}
	else {
		if (ai_bWrite)
			m_pChanges[position].iMode |= M4_WRITE;
		if (ai_bExceptions)
			m_pChanges[position].iMode |= M4_EXCEPTIONS;
	}

	return M4_SUCCESS;
}

//Tocado para Remove condicional
void ClUnixPollFds::InternalClear(ClTSAP* ai_pClTSAP, int ai_iMode)
{
	if (m_pPollFds == NULL || m_pTSAPList == NULL)
	{
		m4Trace(fprintf(stderr, "Error, new devolvió NULL en ClUnixPollFds::constructor\n"));

		return;
	}

	int	i, position;

	for (i = 0, position = -1; i < m_iNumFds; i++)
		if (m_pTSAPList[i] == ai_pClTSAP)
		{
			position = i;
			break;
		}

	if (position == -1)
	{
		m4Trace(fprintf(stderr, "Error, no se encuentra TSAP en ClUnixPollFds::InternalClear\n"));

		return;
	}


	if (ai_iMode & M4_READ)
	{
		if (m_pPollFds[position].events & POLLRDNORM)
			m_pPollFds[position].events ^= POLLRDNORM;
		if (m_pPollFds[position].revents & POLLRDNORM)
			m_pPollFds[position].revents ^= POLLRDNORM;
	}
	if (ai_iMode & M4_WRITE)
	{
		if (m_pPollFds[position].events & POLLWRNORM)
			m_pPollFds[position].events ^= POLLWRNORM;
		if (m_pPollFds[position].revents & POLLWRNORM)
			m_pPollFds[position].revents ^= POLLWRNORM;
	}
	if (ai_iMode & M4_EXCEPTIONS)
	{
		if (m_pPollFds[position].events & POLLPRI)
			m_pPollFds[position].events ^= POLLPRI;
		if (m_pPollFds[position].revents & POLLPRI)
			m_pPollFds[position].revents ^= POLLPRI;
	}

	if (m_pPollFds[position].events == 0)
	{
		memset((void *)&m_pPollFds[position], 0, sizeof(struct	pollfd)); 
		
		if (m_iNumFds - position > 1)
		{
			memmove((void *)&m_pPollFds[position], 
				(void *)&m_pPollFds[position+1], 
				(m_iNumFds - position -1) * sizeof(struct	pollfd)); 
			
			memmove((void *)&m_pTSAPList[position], 
				(void *)&m_pTSAPList[position+1], 
				(m_iNumFds - position -1) * sizeof(ClTSAP *)); 
		}
		m_iNumFds--;
	}
}

m4bool_t ClUnixPollFds::IsSet(ClTSAP* ai_pClTSAP, int ai_iMode) const
{
	if (m_pPollFds == NULL || m_pTSAPList == NULL)
	{
		m4Trace(fprintf(stderr, "Error, new devolvió NULL en ClUnixPollFds::constructor\n"));

		return M4_FALSE;
	}

	int	i, iMask;

	iMask = POLLERR | POLLHUP | POLLNVAL;

	switch(ai_iMode)
	{
		case M4_READ:
			iMask |= POLLRDNORM;
			break;
		
		case M4_WRITE:
			iMask |= POLLWRNORM;
			break;
		
		case M4_EXCEPTIONS:
			iMask |= POLLPRI;
			break;
		
		default:
			m4Trace(fprintf(stderr, "Error, modo %d no reconocible en  ClUnixPollFds::IsSet\n", ai_iMode));

			return M4_FALSE;
	}
	for (i = 0; i < m_iNumFds; i++)
		if (m_pTSAPList[i] == ai_pClTSAP)
		{
			if ( m_pPollFds[i].revents & POLLERR)
				m4Trace(fprintf(stderr, "Socket con POLLERR en select\n"));
			if ( m_pPollFds[i].revents & POLLHUP)
				m4Trace(fprintf(stderr, "Socket con POLLHUP en select\n"));
			if ( m_pPollFds[i].revents & POLLNVAL)
				m4Trace(fprintf(stderr, "Socket con POLLNVAL en select\n"));
			if ( m_pPollFds[i].events & iMask)
				return (m_pPollFds[i].revents & iMask)?M4_TRUE:M4_FALSE; 
			else
				return M4_FALSE;
		}
	return M4_FALSE;
}

ClTSAP * ClUnixPollFds::NextSet(int ai_iMode) 
{
	Regenerate();
	if (m_pPollFds == NULL || m_pTSAPList == NULL)
	{
		m4Trace(fprintf(stderr, "Error, new devolvió NULL en ClUnixPollFds::constructor\n"));

		return NULL;
	}

	int	iMask;
	ClTSAP	*pTSAP = NULL;

	switch(ai_iMode)
	{
		case M4_READ:
			iMask = POLLRDNORM;
			while (m_iPositionRead < m_iNumFds)
			{
				if (m_pPollFds[m_iPositionRead].events & iMask)
				{
					if (m_pPollFds[m_iPositionRead].revents & iMask)
					{
						pTSAP = m_pTSAPList[m_iPositionRead];
						break;
					}
				}
				m_iPositionRead++;
			}

			if (pTSAP != NULL)
				m_iPositionRead++;
			break;
		
		case M4_WRITE:
			iMask = POLLWRNORM;
			while (m_iPositionWrite < m_iNumFds)
			{
				if (m_pPollFds[m_iPositionWrite].events & iMask)
				{
					if (m_pPollFds[m_iPositionWrite].revents & iMask)
					{
						pTSAP = m_pTSAPList[m_iPositionWrite];
						break;
					}
				}
				m_iPositionWrite++;
			}

			if (pTSAP != NULL)
				m_iPositionWrite++;
			break;
		
		case M4_EXCEPTIONS:
			iMask = POLLPRI;
			while (m_iPositionExceptions < m_iNumFds)
			{
				if (m_pPollFds[m_iPositionExceptions].events & iMask)
				{
					if (m_pPollFds[m_iPositionExceptions].revents & iMask)
					{
						pTSAP = m_pTSAPList[m_iPositionExceptions];
						break;
					}
				}
				m_iPositionExceptions++;
			}

			if (pTSAP != NULL)
				m_iPositionExceptions++;
			break;
		
		default:
			m4Trace(fprintf(stderr, "Error, modo %d no reconocible en  ClUnixPollFds::NextSet\n", ai_iMode));
			return NULL;
	}

	return pTSAP;
}

int ClUnixPollFds::GetSize() const
{
	return m_iSize;
}

m4bool_t ClUnixPollFds::IsIn(ClTSAP* ai_pClTSAP, int ai_iMode)
{
	return IsSet(ai_pClTSAP, ai_iMode);
}
  
int ClUnixPollFds::Poll(ClMutex	*ai_poMutex, int ai_iMillisecs)
{
	if (m_pPollFds == NULL || m_pTSAPList == NULL)
	{
		m4Trace(fprintf(stderr, "Error, new devolvió NULL en ClUnixPollFds::constructor\n"));

		return 0;
	}

	int	i,
		ret; 
	if (ai_poMutex != NULL)
		ai_poMutex->Lock();
	Regenerate();
	for (i = 0; i < m_iNumFds; i++)
		m_pPollFds[i].revents = 0;
	if (ai_poMutex != NULL)
		ai_poMutex->Unlock();
	
	ret = poll(m_pPollFds, m_iNumFds, ai_iMillisecs);

	m_iPositionRead = 0;
	m_iPositionWrite = 0;
	m_iPositionExceptions = 0;

	return ret;
}

void ClUnixPollFds::Regenerate()
{
	int i;

	for (i = 0; i < m_iChanges; i++)
	{
		if (m_pChanges[i].iChange == CHANGE_ADD)
			InternalSet(m_pChanges[i].pClTSAP,
						m_pChanges[i].iMode);
		else
			InternalClear(m_pChanges[i].pClTSAP,m_pChanges[i].iMode);
	}
	m_iChanges = 0;
}

#endif

