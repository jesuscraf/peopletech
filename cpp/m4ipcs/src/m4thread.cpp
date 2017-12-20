//## begin module%3742D02203B1.cm preserve=no
//## end module%3742D02203B1.cm

//## begin module%3742D02203B1.cp preserve=no
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
//## end module%3742D02203B1.cp

//## Module: m4thread%3742D02203B1; Package body
//## Subsystem: M4Ipcs::src%3742D0220389
//	d:\fuentesservidor\m4ipcs\version\src
//## Source file: D:\Work\v600\m4ipcs\src\m4thread.cpp

//## begin module%3742D02203B1.additionalIncludes preserve=no
//## end module%3742D02203B1.additionalIncludes

//## begin module%3742D02203B1.includes preserve=yes

#ifdef _WINDOWS
	#include <process.h>
#endif

#include <signal.h>
// ADD FJSM 990903 Control de excepciones
#include <setjmp.h>
jmp_buf cancelExecutionThread;
#include <m4exception.hpp>
//## end module%3742D02203B1.includes

// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
// CriticalSection
#include <criticalsection.hpp>
// m4thread
#include <m4thread.hpp>
// m4condition
#include <m4condition.hpp>
// ClThreadObjects
#include <threadobjs.hpp>

//## begin module%3742D02203B1.declarations preserve=no
//## end module%3742D02203B1.declarations

//## begin module%3742D02203B1.additionalDeclarations preserve=yes
#define M4_THREADPROP_CHECKJUMP "M4_CHECKJUMP_OBJECT"
	
M4_THREAD_RETURN  Func2(m4pvoid_t arg)
{
	M4Thread *func_ej=(M4Thread *)arg;

	func_ej->SetIdThread(M4Thread::GetCurrentIdThread());

	M4Thread::RegisterThread(func_ej);

	M4Thread *pM4Thread=NULL;

//DEL FJSM 13-10-99
// Siempre se activa el manejador, pero en él, se 
//comprueba si tengo o no activas las excepciones para tratarlo
//	if (func_ej->GetUseException() == M4_TRUE)
//	{
#ifndef M4_NO_EXCEPTIONS

	#ifdef _UNIX
	// el SIGPIPE se ignora para que si un cliente se desconecta no tire el server
		signal(SIGPIPE, SIG_IGN);
		signal(SIGCHLD, SIG_IGN);

		signal(SIGHUP, FirstChanceException);
		signal(SIGINT, FirstChanceException);
		signal(SIGQUIT, FirstChanceException);
		signal(SIGILL, FirstChanceException);
		signal(SIGTRAP, FirstChanceException);
		signal(SIGIOT, FirstChanceException);
		signal(SIGABRT, FirstChanceException);
		signal(SIGEMT, FirstChanceException);
		signal(SIGFPE, FirstChanceException);
		signal(SIGBUS, FirstChanceException);
		signal(SIGSEGV, FirstChanceException);
		signal(SIGSYS, FirstChanceException);
		signal(SIGALRM, FirstChanceException);
		signal(SIGTERM, FirstChanceException);
		signal(SIGUSR1, FirstChanceException);
		signal(SIGUSR2, FirstChanceException);
		signal(SIGPOLL, FirstChanceException);
		signal(SIGIO, FirstChanceException);
		signal(SIGVTALRM, FirstChanceException);
		signal(SIGPROF, FirstChanceException);
		signal(SIGXCPU, FirstChanceException);
		signal(SIGXFSZ, FirstChanceException);
	#else
		//establecemos el manejador de excepciones C
		_set_se_translator(FirstChanceException);
	#endif
	//Controlará aquellas excepciones que no se haya sido controladas
		set_terminate(TerminateChanceException);

		try {
	#ifdef _UNIX
				if (setjmp(cancelExecutionThread) == 0)
	#endif
					func_ej->Run();
			}
			catch (M4RuntimeException) {
				//En caso que no se controle totalmente en el interior de la
				//funcionalidad del thread.
				if ((M4Thread::GetInstance(M4Thread::GetCurrentIdThread(), pM4Thread) == M4_SUCCESS) && (pM4Thread)) {
					M4UserHandler_t pFunc = NULL;
					m4pvoid_t param=NULL;
					pFunc = pM4Thread->GetUserHandler(param);
					if (pFunc)
						pFunc(param);
				}
			}
#else
		func_ej->Run();
#endif

	ClThreadObjects::RemoveThread();

	if ((M4Thread::GetInstance(M4Thread::GetCurrentIdThread(), pM4Thread) == M4_SUCCESS) && (pM4Thread)) {
		if (pM4Thread) {
			M4Thread::DeregisterThread(pM4Thread->GetIdThread());
			if (pM4Thread->GetDestroy()==M4_TRUE) {
				//Lo hacemos lo ultimo por si yo no borro que no me puedan borrar
				pM4Thread->SetExit();
				delete pM4Thread;
			}
			else
				//Lo hacemos lo ultimo por si yo no borro que no me puedan borrar
				pM4Thread->SetExit();
		}
	}

#ifdef _UNIX
	pthread_exit(NULL);
#else
	_endthreadex(0);
#endif

	return NULL;
}

//## end module%3742D02203B1.additionalDeclarations


// Class M4Thread 

//## begin M4Thread::s_ListThread%3742D02200EF.attr preserve=no  protected: static M4ThreadMap_t {VA} 
M4ThreadMap_t M4Thread::s_ListThread;
//## end M4Thread::s_ListThread%3742D02200EF.attr


















//## begin M4Thread::s_oMutexRegister%3923F6DB00E6.role preserve=no  protected: static ClCriticalSection { -> VHAN}
ClCriticalSection M4Thread::s_oMutexRegister(M4_TRUE);
//## end M4Thread::s_oMutexRegister%3923F6DB00E6.role

M4Thread::M4Thread ()
  //## begin M4Thread::M4Thread%1087019654.hasinit preserve=no
  //## end M4Thread::M4Thread%1087019654.hasinit
  //## begin M4Thread::M4Thread%1087019654.initialization preserve=yes
  //## end M4Thread::M4Thread%1087019654.initialization
{
  //## begin M4Thread::M4Thread%1087019654.body preserve=yes
	m_create=M4_FALSE;
	m_suspend=M4_FALSE;
	m_terminate=M4_FALSE;
	m_exit=M4_FALSE;
	m_pcStringsId=NULL;
	m_destroy = M4_FALSE;
#ifdef _WINDOWS
	m_thid=0;
#endif
	m_hHandleMutex.Init();
	m_oCondVar=new ClCondition;

#ifndef M4_NO_EXCEPTIONS
	// Control de excepciones
	m_paramUserHandler = NULL;
	m_pfUserHandler = NULL;
	m_UseException=M4_FALSE;
	m_uiExceptionMask = M4EXCEPTION_ALL ^ M4EXCEPTION_UNDEFINED;

	m_pfThrowFunction = NULL ;

	#ifdef _UNIX
		//ADD FJSM 990404
		m_iNumException = 1;
		m_iLastException = 0;
		//END ADD
	#endif
#endif
  //## end M4Thread::M4Thread%1087019654.body
}


M4Thread::~M4Thread ()
{
  //## begin M4Thread::~M4Thread%39119088.body preserve=yes
#ifdef _WINDOWS
	if (0!=m_thid)
		CloseHandle((HANDLE) m_thid );
#endif
	m_oCondVar->Close();

	delete m_oCondVar;

	if (m_pcStringsId != NULL)
		delete[] m_pcStringsId;

  //## end M4Thread::~M4Thread%39119088.body
}



//## Other Operations (implementation)
m4bool_t M4Thread::IsSuspend (void )
{
  //## begin M4Thread::IsSuspend%1234446964.body preserve=yes
	m4bool_t aux;

	m_hHandleMutex.Lock();

	aux=m_suspend;

	m_hHandleMutex.Unlock();

	return aux;
  //## end M4Thread::IsSuspend%1234446964.body
}

m4bool_t M4Thread::IsTerminate (void )
{
  //## begin M4Thread::IsTerminate%-139898326.body preserve=yes
	m4bool_t aux;

	m_hHandleMutex.Lock();
	
	aux=m_terminate;

	m_hHandleMutex.Unlock();

	return aux;
  //## end M4Thread::IsTerminate%-139898326.body
}

m4bool_t M4Thread::IsCreated ()
{
  //## begin M4Thread::IsCreated%927723379.body preserve=yes
	m4bool_t aux;

	m_hHandleMutex.Lock();
	
	aux=m_create;

	m_hHandleMutex.Unlock();

	return aux;
  //## end M4Thread::IsCreated%927723379.body
}

m4return_t M4Thread::Wait ()
{
  //## begin M4Thread::Wait%927795535.body preserve=yes
	if (m_oCondVar->Wait()==M4_ERROR)
		return M4_ERROR;

	return M4_SUCCESS;
  //## end M4Thread::Wait%927795535.body
}

m4bool_t M4Thread::IsRunning ()
{
  //## begin M4Thread::IsRunning%927723380.body preserve=yes
	m4bool_t aux;

	m_hHandleMutex.Lock();
	
	if (m_exit == M4_FALSE && m_create == M4_TRUE)
		aux = M4_TRUE;
	else
		aux = M4_FALSE;

	m_hHandleMutex.Unlock();

	return aux;
  //## end M4Thread::IsRunning%927723380.body
}

void M4Thread::RegisterThread (M4Thread* ai_pThread)
{
  //## begin M4Thread::RegisterThread%-323165801.body preserve=yes
	s_oMutexRegister.Lock();
	s_ListThread.insert(M4ThreadMap_t::value_type(M4Thread::GetCurrentIdThread(), ai_pThread));
	s_oMutexRegister.Unlock();
  //## end M4Thread::RegisterThread%-323165801.body
}

void M4Thread::DeregisterThread (m4iden_t ai_Ident)
{
  //## begin M4Thread::DeregisterThread%471052076.body preserve=yes
	s_oMutexRegister.Lock();
	M4ThreadMap_t::iterator theIterator;
	theIterator = s_ListThread.find(ai_Ident);
    if(theIterator != s_ListThread.end())
	{
		M4ThreadEventDispatcher::Dispatch();
		s_ListThread.erase(theIterator);
	}
	s_oMutexRegister.Unlock();
  //## end M4Thread::DeregisterThread%471052076.body
}

m4return_t M4Thread::GetInstance (m4iden_t ai_Ident, M4Thread*& ao_pThread)
{
  //## begin M4Thread::GetInstance%353962315.body preserve=yes

	// if ident is null we get the current thread
	if (ai_Ident == 0) {
		ai_Ident = GetCurrentIdThread();
	}

	m4return_t ret = M4_SUCCESS;
	s_oMutexRegister.Lock();
	M4ThreadMap_t::iterator theIterator;
	theIterator = s_ListThread.find(ai_Ident);
    if(theIterator != s_ListThread.end())
		ao_pThread = (*theIterator).second;
	else {
		ao_pThread = NULL;
		ret = M4_ERROR;
	}
	s_oMutexRegister.Unlock();
	return ret;
  //## end M4Thread::GetInstance%353962315.body
}

m4return_t M4Thread::Create (func lpSt, m4pvoid_t lpParameter)
{
  //## begin M4Thread::Create%631361272.body preserve=yes
	m4int16_t	ret = 0;
	unsigned int tid;

	m_oCondVar->Init();	

#ifdef _UNIX
	#ifdef _M4HPUX1020_
		int res=pthread_create(&m_thid,pthread_attr_default,lpSt,lpParameter);//creamos el thread
	#else
		pthread_attr_t detached_attr;
		pthread_attr_init(&detached_attr);
		pthread_attr_setdetachstate(&detached_attr, PTHREAD_CREATE_DETACHED);
		/*OSCAR
		  Se asigna primeramente la variable create a TRUE para evitar que el
		  thread se cree y teestee su propio estado antes de poder asignar la
		  variable a true*/

#ifdef _AIX
	// jcr. Para inicializar el tamaño de la pila a 256 KB
		pthread_attr_setstacksize( &detached_attr, 0x40000 ) ;
#endif

		int res=pthread_create(&m_thid, &detached_attr, lpSt, lpParameter);
		pthread_attr_destroy(&detached_attr);
	#endif
	if (res != 0)
	{
		m_hHandleMutex.Lock();
		m_create = M4_FALSE;
		m_hHandleMutex.Unlock();
		return M4_ERROR;
	}
#else   //De Unix
	m_thid = _beginthreadex( NULL, 0,lpSt,lpParameter,NULL, &tid);
	if ( m_thid == 0 )
	{
		m_hHandleMutex.Lock();
		m_create = M4_FALSE;
		m_hHandleMutex.Unlock();
		return M4_ERROR;
	}
#endif

	return M4_SUCCESS;
  //## end M4Thread::Create%631361272.body
}

m4return_t M4Thread::Start (m4pchar_t ai_StringsId)
{
  //## begin M4Thread::Start%356177253.body preserve=yes
	m_hHandleMutex.Lock();
	if (m_create == M4_TRUE) {
		//ya esta creado luego no lo volvemos a crear
		m_hHandleMutex.Unlock();
		return M4_ERROR;
	}
	m_create = M4_TRUE;
	m_hHandleMutex.Unlock();

	if (ai_StringsId!=NULL) {
		if (m_pcStringsId != NULL)
			delete[] m_pcStringsId;
		m_pcStringsId = new char[strlen(ai_StringsId)+1];
		strcpy(m_pcStringsId, ai_StringsId);
	}
	return this->Create(Func2,this);
  //## end M4Thread::Start%356177253.body
}

m4bool_t M4Thread::GetDestroy (void )
{
  //## begin M4Thread::GetDestroy%1652594998.body preserve=yes
	m4bool_t aux;

	m_hHandleMutex.Lock();
	
	aux=m_destroy;

	m_hHandleMutex.Unlock();

	return aux;
  //## end M4Thread::GetDestroy%1652594998.body
}

m4return_t M4Thread::Terminate (m4bool_t destroy)
{
  //## begin M4Thread::Terminate%858475301.body preserve=yes
	m4bool_t aux;

	m_hHandleMutex.Lock();

	m_terminate=M4_TRUE;

	m_destroy = destroy;

	if ((m_exit==M4_TRUE) && (m_destroy==M4_TRUE))
		aux = M4_TRUE;
	else
		aux = M4_FALSE;

	m_hHandleMutex.Unlock();

	if (aux==M4_TRUE)
		delete this; //me borro pues no tengo

	return M4_SUCCESS;
  //## end M4Thread::Terminate%858475301.body
}

m4return_t M4Thread::Suspend ()
{
  //## begin M4Thread::Suspend%75941335.body preserve=yes
	m_hHandleMutex.Lock();

    m_suspend=M4_TRUE;

	m_hHandleMutex.Unlock();
	
	return M4_SUCCESS;
  //## end M4Thread::Suspend%75941335.body
}

m4return_t M4Thread::Resume ()
{
  //## begin M4Thread::Resume%2123195767.body preserve=yes
    if (m_create==M4_FALSE)
	    return M4_ERROR;

	if ( m_oCondVar->Signal()==M4_ERROR)
		return M4_ERROR;
	
	m_hHandleMutex.Lock();

    m_suspend=M4_FALSE;

	m_hHandleMutex.Unlock();

	return M4_SUCCESS;
  //## end M4Thread::Resume%2123195767.body
}

m4bool_t M4Thread::IsExit (void )
{
  //## begin M4Thread::IsExit%-1368781007.body preserve=yes
	m4bool_t aux;

	m_hHandleMutex.Lock();

	aux=m_exit;

	m_hHandleMutex.Unlock();

	return aux;
  //## end M4Thread::IsExit%-1368781007.body
}

m4iden_t M4Thread::GetCurrentIdThread (void )
{
  //## begin M4Thread::GetCurrentIdThread%-1698837353.body preserve=yes
#ifdef _UNIX
	#ifdef _M4HPUX1020_
		pthread_t t = pthread_self();
		return (m4iden_t)t.field1;
	#else
		return  pthread_self();
	#endif
#else
	return GetCurrentThreadId();
#endif
  //## end M4Thread::GetCurrentIdThread%-1698837353.body
}

m4void_t M4Thread::SetExit (m4bool_t ai_bExit)
{
  //## begin M4Thread::SetExit%927126297.body preserve=yes
	m_hHandleMutex.Lock();

	m_exit = ai_bExit;

	m_hHandleMutex.Unlock();
  //## end M4Thread::SetExit%927126297.body
}

m4return_t M4Thread::AddProperty (m4pcchar_t ai_pcName, m4pvoid_t ai_pValue)
{
  //## begin M4Thread::AddProperty%1056555598.body preserve=yes
	if (ai_pcName == NULL) {
		return M4_ERROR;
	}
	
	M4Thread *ref = 0;
	GetInstance(0,ref);
	
	if (ref == NULL) {
		// there is no thread for me. Returns...
		return M4_ERROR;
	}

	// I know what you are thinking... there is no mutex here?
	// don't do that. There is no two threads working with the same object

	// adds the value
	m4string_t pk(ai_pcName);

	// fix bug 0094404
	ref->m_oPropertyBag.erase(pk);

	ref->m_oPropertyBag.insert(M4PropertyBag_t::value_type(pk,ai_pValue));

	return M4_SUCCESS;
  //## end M4Thread::AddProperty%1056555598.body
}

m4pvoid_t M4Thread::GetProperty (m4pcchar_t ai_pcName)
{
  //## begin M4Thread::GetProperty%1056555599.body preserve=yes
	
	if (ai_pcName == NULL) {
		return NULL;
	}
	
	M4Thread *ref = 0;
	GetInstance(0,ref);
	
	if (ref == NULL) {
		// there is no thread for me. Returns...
		return NULL;
	}

	// I know what you are thinking... there is no mutex here?
	// don't do that. There is no two threads working with the same object

	// gets the value back
	m4pvoid_t result = NULL;

	// adds the value
	m4string_t pk(ai_pcName);
	M4PropertyBag_t::iterator it = ref->m_oPropertyBag.find(pk);

	if (it != ref->m_oPropertyBag.end()) {
		result = (*it).second;
	}

	return result;
  //## end M4Thread::GetProperty%1056555599.body
}

// Additional Declarations
  //## begin M4Thread%3742D02200CF.declarations preserve=yes
  //## end M4Thread%3742D02200CF.declarations

// Class M4CheckJump 







M4CheckJump::M4CheckJump ()
  //## begin M4CheckJump::M4CheckJump%1056610573.hasinit preserve=no
      : m_bInitialized(M4_FALSE), m_poNext(NULL), m_poPrevious(NULL), m_bExceptionCached(M4_FALSE), m_oException(0,"")
  //## end M4CheckJump::M4CheckJump%1056610573.hasinit
  //## begin M4CheckJump::M4CheckJump%1056610573.initialization preserve=yes
  //## end M4CheckJump::M4CheckJump%1056610573.initialization
{
  //## begin M4CheckJump::M4CheckJump%1056610573.body preserve=yes
  //## end M4CheckJump::M4CheckJump%1056610573.body
}


M4CheckJump::~M4CheckJump ()
{
  //## begin M4CheckJump::~M4CheckJump%1056610574.body preserve=yes
#ifdef _AIX
	DeRegister();
#endif
  //## end M4CheckJump::~M4CheckJump%1056610574.body
}



//## Other Operations (implementation)
m4void_t M4CheckJump::Check ()
{
  //## begin M4CheckJump::Check%1056610577.body preserve=yes
#ifdef _AIX
	if (m_bInitialized) {
		return;
	}
	m_bInitialized = M4_TRUE;

	Register();

	if (setjmp(m_oJumpBuffer) == 0) {
		return;
	}

	// sets the internal flag
	M4CheckJump *ref = (M4CheckJump*) M4Thread::GetProperty(M4_THREADPROP_CHECKJUMP);
	if (ref == NULL) {
		// there is no register for me?? Internal error
		return;
	}

	ref->m_bExceptionCached = M4_TRUE;

#endif
  //## end M4CheckJump::Check%1056610577.body
}

m4void_t M4CheckJump::Throw (M4RuntimeException e)
{
  //## begin M4CheckJump::Throw%1056610572.body preserve=yes
#ifdef _AIX
	// gets my own instance
	M4CheckJump *ref = (M4CheckJump *) M4Thread::GetProperty(M4_THREADPROP_CHECKJUMP);
	if (ref == NULL) {
		// there is no object registered for this thread
		return;
	}

	// jumps
	ref->m_oException = e;
	longjmp(ref->m_oJumpBuffer,1);
#endif
  //## end M4CheckJump::Throw%1056610572.body
}

m4void_t M4CheckJump::Register ()
{
  //## begin M4CheckJump::Register%1056610575.body preserve=yes
	// are you there?
	M4CheckJump *poRef = (M4CheckJump*) M4Thread::GetProperty(M4_THREADPROP_CHECKJUMP);

	m_poNext = poRef;

	if (poRef != NULL) {
		poRef->m_poPrevious = this;
	}

	// updates the list
	M4Thread::AddProperty(M4_THREADPROP_CHECKJUMP,this);
  //## end M4CheckJump::Register%1056610575.body
}

m4void_t M4CheckJump::DeRegister ()
{
  //## begin M4CheckJump::DeRegister%1056610576.body preserve=yes
	M4CheckJump *poRef = (M4CheckJump*) M4Thread::GetProperty(M4_THREADPROP_CHECKJUMP);

	if (poRef == this) {
		// updates the list
		if (m_poNext != NULL) {
			m_poNext->m_poPrevious = NULL;
		}
		M4Thread::AddProperty(M4_THREADPROP_CHECKJUMP,m_poNext);
		return;
	}

	// scans and deletes the element
	while (poRef != NULL && poRef != this) {
		poRef = poRef->m_poNext;
	}

	// there is no entry for me. Already deleted
	if (poRef == NULL) {
		return;
	}

	// updates the list
	if (m_poPrevious != NULL) {
		m_poPrevious->m_poNext = m_poNext;
	}

	if (m_poNext != NULL) {
		m_poNext->m_poPrevious = m_poPrevious;
	}

  //## end M4CheckJump::DeRegister%1056610576.body
}

M4CheckJump* M4CheckJump::CheckThrow ()
{
  //## begin M4CheckJump::CheckThrow%1057245449.body preserve=yes
#ifndef _AIX
	return NULL;
#else
	// gets the current reference
	M4CheckJump *ref = (M4CheckJump*) M4Thread::GetProperty(M4_THREADPROP_CHECKJUMP);
	if (ref == NULL) {
		// there is no register for me
		return NULL;
	}

	// there is an exception there?
	if (! ref->m_bExceptionCached) {
		return NULL;
	}

	return ref;
	
	/* 
	// exception catched, pass it throw
	// this doesn't work on aix5l (gcc)
	M4Thread *pThread = NULL;
	m4return_t iRes = M4Thread::GetInstance(0,pThread);
	if (iRes != M4_SUCCESS || pThread == NULL) {
		return;
	}

	M4ThrowFunction_t f = pThread->GetThrowFunction() ;
	f(ref->m_oException);
	*/

#endif
  //## end M4CheckJump::CheckThrow%1057245449.body
}

// Additional Declarations
  //## begin M4CheckJump%3EFA98BC01B1.declarations preserve=yes
  //## end M4CheckJump%3EFA98BC01B1.declarations

//## begin module%3742D02203B1.epilog preserve=yes
M4ThreadDispatcherMap_t M4ThreadEventDispatcher::m_oMap;
ClMutex M4ThreadEventDispatcher::m_oMutex(M4_TRUE);

void M4ThreadEventDispatcher::AddHandler(M4IThreadEventHandler *ai_oHandler)
{
	M4ThreadDispatcherMap_t::iterator theIterator;
	M4IThreadEventHandler * oOldEventHandler;
	m4iden_t iIdThread = M4Thread::GetCurrentIdThread();

	m_oMutex.Lock();

	// Si ya existía un Event Handler, le eliminamos
	theIterator = m_oMap.find(iIdThread);
    if(theIterator != m_oMap.end())
	{
		oOldEventHandler = (*theIterator).second;
		m_oMap.erase(theIterator);
		delete oOldEventHandler;
	}
	
	// Añadimos el Handler
	m_oMap.insert(M4ThreadDispatcherMap_t::value_type(iIdThread, ai_oHandler));

	m_oMutex.Unlock();
}

void M4ThreadEventDispatcher::Dispatch()
{
	M4ThreadDispatcherMap_t::iterator theIterator;
	M4IThreadEventHandler * oEventHandler = NULL;
	m4iden_t iIdThread = M4Thread::GetCurrentIdThread();

	m_oMutex.Lock();

	// Buscamos el Event Handler, nos quedamos con él y le eliminamos del mapa
	theIterator = m_oMap.find(iIdThread);
    if(theIterator != m_oMap.end())
	{
		oEventHandler = (*theIterator).second;
		m_oMap.erase(theIterator);
	}
	
	m_oMutex.Unlock();

	// Invocamos el Event Handler si se encontró
	if (oEventHandler)
	{
		oEventHandler->EndThread();
		delete oEventHandler;
	}
}

//## end module%3742D02203B1.epilog
