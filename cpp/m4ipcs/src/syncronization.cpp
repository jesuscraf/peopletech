//## begin module%3742D0230006.cm preserve=no
//## end module%3742D0230006.cm

//## begin module%3742D0230006.cp preserve=no
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
//## end module%3742D0230006.cp

//## Module: syncronization%3742D0230006; Package body
//## Subsystem: M4Ipcs::src%3742D0220389
//	d:\fuentesservidor\m4ipcs\version\src
//## Source file: D:\FuentesServidor\m4ipcs\version\src\syncronization.cpp

//## begin module%3742D0230006.additionalIncludes preserve=no
//## end module%3742D0230006.additionalIncludes

//## begin module%3742D0230006.includes preserve=yes
//## end module%3742D0230006.includes

// m4types
#include <m4types.hpp>
// syncronization
#include <m4syncronization.hpp>
//## begin module%3742D0230006.declarations preserve=no
//## end module%3742D0230006.declarations

//## begin module%3742D0230006.additionalDeclarations preserve=yes
#ifdef _UNIX
#if defined _USE_ALGORITHM || defined _M4HPUX1020_
ClWriterReaders::ClWriterReaders()
{
	m_bError = M4_FALSE;

#ifdef _M4HPUX1020_
	if (pthread_mutex_init(&m_rwlp.m, pthread_mutexattr_default) != 0)
	{
			m_bError = M4_TRUE;
			return;
	}
	if (pthread_cond_init(&m_rwlp.readers_ok, pthread_condattr_default) != 0)
	{
			m_bError = M4_TRUE;
			return;
	}
	if (pthread_cond_init(&m_rwlp.writer_ok, pthread_condattr_default) != 0)
	{
			m_bError = M4_TRUE;
			return;
	}
#else
	if (pthread_mutex_init(&m_rwlp.m, NULL) != 0)
	{
			m_bError = M4_TRUE;
			return;
	}
	if (pthread_cond_init(&m_rwlp.readers_ok, NULL) != 0)
	{
			m_bError = M4_TRUE;
			return;
	}
	if (pthread_cond_init(&m_rwlp.writer_ok, NULL) != 0)
	{
			m_bError = M4_TRUE;
			return;
	}
#endif
	m_rwlp.rwlock = 0;
	m_rwlp.waiting_writers = 0;

}



void ClWriterReaders::EnterReader()
{
	if (m_bError)
	{
#ifdef DEBUG
		fprintf(stderr, "EnterReader inhabilitado por error en inicialización\n");
#endif
		return;
	}
	pthread_mutex_lock(&m_rwlp.m);
	while (m_rwlp.rwlock < 0 || m_rwlp.waiting_writers)
			pthread_cond_wait(&m_rwlp.readers_ok, &m_rwlp.m);
	m_rwlp.rwlock++;
	pthread_mutex_unlock(&m_rwlp.m);
}

void ClWriterReaders::EnterWriter()
{
	if (m_bError)
	{
#ifdef DEBUG
		fprintf(stderr, "EnterWriter inhabilitado por error en inicialización\n");
#endif
		return;
	}
	pthread_mutex_lock(&m_rwlp.m);
	while (m_rwlp.rwlock != 0)
	{
		m_rwlp.waiting_writers++;
		pthread_cond_wait(&m_rwlp.writer_ok, &m_rwlp.m);
		m_rwlp.waiting_writers--;
	}
	m_rwlp.rwlock = -1;
	pthread_mutex_unlock(&m_rwlp.m);
}

void ClWriterReaders::LeaveReader()
{
	if (m_bError)
	{
#ifdef DEBUG
		fprintf(stderr, "LeaveReader inhabilitado por error en inicialización\n");
#endif
		return;
	}
	ClWriterReaders::LeaveWriter();
}

void ClWriterReaders::LeaveWriter()
{
	int	ww,
		wr;

	if (m_bError)
	{
#ifdef DEBUG
		fprintf(stderr, "LeaveWriter inhabilitado por error en inicialización\n");
#endif
		return;
	}

	pthread_mutex_lock(&m_rwlp.m);
	if (m_rwlp.rwlock < 0)
		m_rwlp.rwlock = 0;
	else
		m_rwlp.rwlock--;

	ww = (m_rwlp.waiting_writers && m_rwlp.rwlock == 0);
	wr = (m_rwlp.waiting_writers == 0);
	pthread_mutex_unlock(&m_rwlp.m);

	if (ww)
		pthread_cond_signal(&m_rwlp.writer_ok);
	else if (wr)
		pthread_cond_broadcast(&m_rwlp.readers_ok);
}

ClWriterReaders::~ClWriterReaders()
{
	pthread_cond_destroy(&m_rwlp.writer_ok);
	pthread_cond_destroy(&m_rwlp.readers_ok);
	pthread_mutex_destroy(&m_rwlp.m);
}


#else  // De _USE_ALGORITHM || defined _M4HPUX1020_

ClWriterReaders::ClWriterReaders()
{
	m_bError = M4_FALSE;

#ifdef _SOLARIS
	if (rwlock_init(&m_rwlock, NULL, NULL) != 0)
	{
			m_bError = M4_TRUE;
			return;
	}
#else
	if (pthread_rwlock_init(&m_rwlock, NULL) != 0)
	{
			m_bError = M4_TRUE;
			return;
	}
#endif
}



void ClWriterReaders::EnterReader()
{
	if (m_bError)
	{
#ifdef DEBUG
		fprintf(stderr, "EnterReader inhabilitado por error en inicialización\n");
#endif
		return;
	}

#ifdef _SOLARIS
	rw_rdlock(&m_rwlock);
#else
	pthread_rwlock_rdlock(&m_rwlock);
#endif
}

void ClWriterReaders::EnterWriter()
{
	if (m_bError)
	{
#ifdef DEBUG
		fprintf(stderr, "EnterWriter inhabilitado por error en inicialización\n");
#endif
		return;
	}

#ifdef _SOLARIS
	if (rw_wrlock(&m_rwlock) != 0)
	{
			m_bError = M4_TRUE;
			return;
	}
#else
	if (pthread_rwlock_wrlock(&m_rwlock) != 0)
	{
			m_bError = M4_TRUE;
			return;
	}
#endif
}

void ClWriterReaders::LeaveReader()
{
	if (m_bError)
	{
#ifdef DEBUG
		fprintf(stderr, "LeaveReader inhabilitado por error en inicialización\n");
#endif
		return;
	}

	ClWriterReaders::LeaveWriter();
}

void ClWriterReaders::LeaveWriter()
{
	if (m_bError)
	{
#ifdef DEBUG
		fprintf(stderr, "LeaveWriter inhabilitado por error en inicialización\n");
#endif
		return;
	}

#ifdef _SOLARIS
	rw_unlock(&m_rwlock);
#else
	pthread_rwlock_unlock(&m_rwlock);
#endif
}

ClWriterReaders::~ClWriterReaders()
{
#ifdef _SOLARIS
	rwlock_destroy(&m_rwlock);
#else
	pthread_rwlock_destroy(&m_rwlock);
#endif
}

#endif // De _USE_ALGORITHM || defined _M4HPUX1020_

#else
#define WRITER 0
#define READER 1
//## end module%3742D0230006.additionalDeclarations


// Class ClWriterReaders 






ClWriterReaders::ClWriterReaders ()
  //## begin ClWriterReaders::ClWriterReaders%-337753720.hasinit preserve=no
  //## end ClWriterReaders::ClWriterReaders%-337753720.hasinit
  //## begin ClWriterReaders::ClWriterReaders%-337753720.initialization preserve=yes
  //## end ClWriterReaders::ClWriterReaders%-337753720.initialization
{
  //## begin ClWriterReaders::ClWriterReaders%-337753720.body preserve=yes
	hReaderEvent = NULL;
	hMutex = NULL;
	hWriterMutex = NULL;
	iCounter = -1;
	m_bError = M4_FALSE;

	if ((hReaderEvent = CreateEvent(NULL,TRUE,FALSE,NULL)) == NULL)
	{
		m_bError = M4_TRUE;
		return;
	}
	if ((hMutex = CreateEvent(NULL,FALSE,TRUE,NULL)) == NULL)
	{
		m_bError = M4_TRUE;
		return;
	}
	if ((hWriterMutex = CreateMutex(NULL,FALSE,NULL)) == NULL)
	{
		m_bError = M4_TRUE;
		return;
	}
	iCounter = -1;
  //## end ClWriterReaders::ClWriterReaders%-337753720.body
}


ClWriterReaders::~ClWriterReaders ()
{
  //## begin ClWriterReaders::~ClWriterReaders%-1216499026.body preserve=yes
	CloseHandle(hReaderEvent);
	CloseHandle(hMutex);
	CloseHandle(hWriterMutex);
  //## end ClWriterReaders::~ClWriterReaders%-1216499026.body
}



//## Other Operations (implementation)
void ClWriterReaders::EnterReader ()
{
  //## begin ClWriterReaders::EnterReader%1757037252.body preserve=yes
	if (m_bError)
	{
#ifdef DEBUG
		fprintf(stderr, "EnterReader inhabilitado por error en inicialización\n");
#endif
		return;
	}

	if (InterlockedIncrement(&iCounter) == 0)
	{
		WaitForSingleObject(hMutex, INFINITE);
        SetEvent(hReaderEvent);
	};
	WaitForSingleObject(hReaderEvent,INFINITE);
  //## end ClWriterReaders::EnterReader%1757037252.body
}

void ClWriterReaders::EnterWriter ()
{
  //## begin ClWriterReaders::EnterWriter%-1659769292.body preserve=yes
	if (m_bError)
	{
#ifdef DEBUG
		fprintf(stderr, "EnterWriter inhabilitado por error en inicialización\n");
#endif
		return;
	}

	WaitForSingleObject(hWriterMutex,INFINITE);
	WaitForSingleObject(hMutex, INFINITE);
  //## end ClWriterReaders::EnterWriter%-1659769292.body
}

void ClWriterReaders::LeaveReader ()
{
  //## begin ClWriterReaders::LeaveReader%-2086358376.body preserve=yes
	if (m_bError)
	{
#ifdef DEBUG
		fprintf(stderr, "LeaveReader inhabilitado por error en inicialización\n");
#endif
		return;
	}

  if (InterlockedDecrement(&iCounter) < 0)
  {
	  ResetEvent(hReaderEvent);
	  SetEvent(hMutex);
  };
  //## end ClWriterReaders::LeaveReader%-2086358376.body
}

void ClWriterReaders::LeaveWriter ()
{
  //## begin ClWriterReaders::LeaveWriter%-1432057576.body preserve=yes
	if (m_bError)
	{
#ifdef DEBUG
		fprintf(stderr, "LeaveWriter inhabilitado por error en inicialización\n");
#endif
		return;
	}

	SetEvent(hMutex);
	ReleaseMutex(hWriterMutex);
  //## end ClWriterReaders::LeaveWriter%-1432057576.body
}

// Additional Declarations
  //## begin ClWriterReaders%3742D0200392.declarations preserve=yes
  //## end ClWriterReaders%3742D0200392.declarations

//## begin module%3742D0230006.epilog preserve=yes
;

#endif //Windows


 
//## end module%3742D0230006.epilog
