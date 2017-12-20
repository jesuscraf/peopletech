//## begin module%3742D022038A.cm preserve=no
//## end module%3742D022038A.cm

//## begin module%3742D022038A.cp preserve=no
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
//## end module%3742D022038A.cp

//## Module: boolcondition%3742D022038A; Package body
//## Subsystem: M4Ipcs::src%3742D0220389
//	d:\fuentesservidor\m4ipcs\version\src
//## Source file: D:\eduardoma\m4ipcs\src\boolcondition.cpp

//## begin module%3742D022038A.additionalIncludes preserve=no
//## end module%3742D022038A.additionalIncludes

//## begin module%3742D022038A.includes preserve=yes
#ifdef _UNIX
	#include <time.h>
#endif
#include <errno.h>
//## end module%3742D022038A.includes

// m4types
#include <m4types.hpp>
// CriticalSection
#include <criticalsection.hpp>
// syncro
#include <syncro.hpp>
// boolcondition
#include <boolcondition.hpp>
//## begin module%3742D022038A.declarations preserve=no
//## end module%3742D022038A.declarations

//## begin module%3742D022038A.additionalDeclarations preserve=yes
//## end module%3742D022038A.additionalDeclarations


// Class ClBooleanCondition 










ClBooleanCondition::ClBooleanCondition (ClSynObject *ai_pMutex)
  //## begin ClBooleanCondition::ClBooleanCondition%1917792126.hasinit preserve=no
      : m_bInitialized(M4_FALSE), m_uiWaiting(0), m_bSignal(0)
  //## end ClBooleanCondition::ClBooleanCondition%1917792126.hasinit
  //## begin ClBooleanCondition::ClBooleanCondition%1917792126.initialization preserve=yes
#ifdef _WINDOWS
  ,m_oWait(0, 1), m_oHandShake(0, 1)
#endif
  //## end ClBooleanCondition::ClBooleanCondition%1917792126.initialization
{
  //## begin ClBooleanCondition::ClBooleanCondition%1917792126.body preserve=yes
	if (ai_pMutex != NULL) {
		m_pCheckMutex = ai_pMutex;
		m_bOwner = M4_FALSE;
	}
	else {
		m_pCheckMutex = new ClCriticalSection(M4_TRUE);
		m_bOwner = M4_TRUE;
	}
  //## end ClBooleanCondition::ClBooleanCondition%1917792126.body
}


ClBooleanCondition::~ClBooleanCondition ()
{
  //## begin ClBooleanCondition::~ClBooleanCondition%110867509.body preserve=yes
#ifdef _UNIX
	pthread_cond_destroy(&m_oWait);
	pthread_cond_destroy(&m_oHandShake);
#endif
	if(m_bOwner == M4_TRUE) {
		delete m_pCheckMutex;
		m_pCheckMutex = NULL;
	}
  //## end ClBooleanCondition::~ClBooleanCondition%110867509.body
}



//## Other Operations (implementation)
m4return_t ClBooleanCondition::Init ()
{
  //## begin ClBooleanCondition::Init%1876779477.body preserve=yes
	m_pCheckMutex->Lock();
	if (m_bInitialized == M4_TRUE) {
		m_pCheckMutex->Unlock();
		return M4_WARNING;
	}
	m_pCheckMutex->Unlock();

	m4return_t		 ret = M4_SUCCESS;
	m_oInternalMutex.Init();
	m_bInitialized = M4_TRUE;
#ifdef _UNIX
	int error = 0;
	int error1 = 0;
	#ifdef _HP1020_
		error = pthread_cond_init(&m_oWait, pthread_condattr_default);
		error1 = pthread_cond_init(&m_oHandShake, pthread_condattr_default);
	#else
		error = pthread_cond_init(&m_oWait, NULL);
		error1 = pthread_cond_init(&m_oHandShake, NULL);
	#endif
	if (error!=0 || error1!=0) {
		m_bInitialized = M4_FALSE;
		ret = M4_ERROR;
	}
#endif
	return ret;
  //## end ClBooleanCondition::Init%1876779477.body
}

m4return_t ClBooleanCondition::Lock (void )
{
  //## begin ClBooleanCondition::Lock%32644181.body preserve=yes
	if (M4_FALSE == m_bInitialized)
		return M4_ERROR;

	return m_pCheckMutex->Lock();
  //## end ClBooleanCondition::Lock%32644181.body
}

m4return_t ClBooleanCondition::Wait (m4uint32_t ai_uiTimeOut)
{
  //## begin ClBooleanCondition::Wait%-770769245.body preserve=yes
	//El tiempo en segundos

	if (M4_FALSE == m_bInitialized)
		return M4_ERROR;
	
	//////////
	//Aqui solo puede haber un thread, pues se tiene que llamar antes al Lock
	//////////

#ifdef _WINDOWS
	m4return_t		 ret = M4_SUCCESS;
	m_oInternalMutex.Lock();
	m_uiWaiting++;
	m_pCheckMutex->Unlock();

	m_oInternalMutex.Unlock();
	m4uint32_t uiTimeOut = ai_uiTimeOut * 1000;
	if (ai_uiTimeOut == 0)
		uiTimeOut = INFINITE;
	if (WaitForSingleObject(m_oWait.Handle(), uiTimeOut) == WAIT_TIMEOUT)
		ret = M4_WARNING;
	//////////
	//A partir de esta zona puede haber n threads (señal, broadcast, time-out..)
	//////////
	m_oInternalMutex.Lock();

	if (m_bSignal == 0 ) {
		m_uiWaiting--; //me he despertado y no hay señales, luego no han disminuido
					// el contador de waiting
	}
	else {
		m_bSignal -- ;
		//despertado el handshake
		m_oHandShake.Unlock();
	}

	m_oInternalMutex.Unlock();
	m_pCheckMutex->Lock();

#else

	m4return_t		 ret = M4_SUCCESS;
	m_oInternalMutex.Lock();
	m_uiWaiting++;
	m_pCheckMutex->Unlock();

	if (ai_uiTimeOut==0) {
		pthread_cond_wait(&m_oWait, m_oInternalMutex.Handle());
		//me despierto con el m_oInternalMutex cogido
	}
	else {
		//quiero un timeout
		struct timespec to;
		to.tv_sec = time(NULL) + ai_uiTimeOut;
		to.tv_nsec = 0;
		//////////
		//A partir de esta zona puede haber n threads (señal, broadcast, time-out..)
		//////////
		if(pthread_cond_timedwait(&m_oWait, m_oInternalMutex.Handle(), &to)==ETIMEDOUT) {
			//me despierto con el m_oInternalMutex cogido
			ret = M4_WARNING;
		}
	}

	if (m_bSignal == 0 ) {
		m_uiWaiting--; //me he despertado y no hay señales, luego no han disminuido
					// el contador de waiting
	}
	else {
		m_bSignal --;
		//despertado el handshake
		pthread_cond_signal(&m_oHandShake);
	}
	m_oInternalMutex.Unlock();
	m_pCheckMutex->Lock();

#endif

	//simpre salgo con m_pCheckMutex cogido
	return ret;
  //## end ClBooleanCondition::Wait%-770769245.body
}

m4return_t ClBooleanCondition::Unlock (void )
{
  //## begin ClBooleanCondition::Unlock%1580498493.body preserve=yes
	if (M4_FALSE == m_bInitialized)
		return M4_ERROR;

	return m_pCheckMutex->Unlock();;
  //## end ClBooleanCondition::Unlock%1580498493.body
}

m4return_t ClBooleanCondition::Notify (m4bool_t ai_bAll)
{
  //## begin ClBooleanCondition::Notify%927545397.body preserve=yes

	if (M4_FALSE == m_bInitialized)
		return M4_ERROR;

	//////////
	//Aqui solo puede haber un thread, pues se tiene que llamar antes al Lock
	//////////

#ifdef _WINDOWS
	/*variable que usamos para no hacer dos Unlock del m_oInternalMutex
	de forma que es true cuando hemos hecho un lock y false cuando hemos hecho
	el unlock */
	m4bool_t bIsLooked = M4_TRUE;
	m_oInternalMutex.Lock();
	while (m_uiWaiting > 0) { //despierto a todos si ai_all == M4_TRUE
		m_uiWaiting--;
		m_bSignal ++;
		m_oWait.Unlock();
		m_oInternalMutex.Unlock();
		m_oHandShake.Lock(); //espero confirmacion que he despertado
		if (ai_bAll == M4_TRUE) //tengo que repetir la operacion
			m_oInternalMutex.Lock();
		else {
			bIsLooked = M4_FALSE;
			break;
		}
	}
	if (bIsLooked == M4_TRUE)
		m_oInternalMutex.Unlock();

#else

	m_oInternalMutex.Lock();
	if (m_uiWaiting > 0) {
		m_uiWaiting--;
		m_bSignal ++;
		if (ai_bAll==M4_TRUE)
			pthread_cond_broadcast(&m_oWait);
		else
			pthread_cond_signal(&m_oWait);
		//espero confirmación que se ha despertado
		pthread_cond_wait(&m_oHandShake, m_oInternalMutex.Handle());
		//me despierto con m_oInternalMutex cogido
		m_oInternalMutex.Unlock();
	}
	else
		m_oInternalMutex.Unlock();

#endif

	return M4_SUCCESS;
  //## end ClBooleanCondition::Notify%927545397.body
}

// Additional Declarations
  //## begin ClBooleanCondition%3742D01E02E4.declarations preserve=yes
  //## end ClBooleanCondition%3742D01E02E4.declarations

//## begin module%3742D022038A.epilog preserve=yes
//## end module%3742D022038A.epilog
