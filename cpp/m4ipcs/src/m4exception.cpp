//## begin module%378CB242024C.cm preserve=no
//## end module%378CB242024C.cm

//## begin module%378CB242024C.cp preserve=no
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
//## end module%378CB242024C.cp

//## Module: m4exception%378CB242024C; Package body
//## Subsystem: M4Ipcs::src%3742D0220389
//	d:\fuentesservidor\m4ipcs\version\src
//## Source file: D:\FuentesServidor\m4ipcs\version\src\m4exception.cpp

//## begin module%378CB242024C.additionalIncludes preserve=no
//## end module%378CB242024C.additionalIncludes

//## begin module%378CB242024C.includes preserve=yes
#include <m4types.hpp>
#include "m4trace.hpp"
#include "m4thread.hpp"
#ifdef _UNIX
	// ADD FJSM 990903 Contro+ de excepciones
	#include <setjmp.h>
	extern jmp_buf cancelExecutionThread;
	#include <signal.h>
#else
	#include <process.h>
	#include <ctime>
#endif
//## end module%378CB242024C.includes

// m4types
#include <m4types.hpp>
// m4exception
#include <m4exception.hpp>
// ClThreadObjects
#include <threadobjs.hpp>

//## begin module%378CB242024C.declarations preserve=no
//## end module%378CB242024C.declarations

//## begin module%378CB242024C.additionalDeclarations preserve=yes
//## end module%378CB242024C.additionalDeclarations


// Class M4RuntimeException 


// Additional Declarations
  //## begin M4RuntimeException%3742D0210049.declarations preserve=yes
  //## end M4RuntimeException%3742D0210049.declarations

//## begin module%378CB242024C.epilog preserve=yes

#ifdef _UNIX
///////////////////////////
//UNIX
////////////////////////////
#define M4_EXT_CORE_GENERATOR "m4kshdump"
void InvokeExternalDebugInfoCollector()
{
	char acBuffer[ 256 ] ;
	m4iden_t idPID = getpid() ;

	m4Trace( printf( "%p: FirstChanceException. Launching %s\n", M4Thread::GetCurrentIdThread(), M4_EXT_CORE_GENERATOR ) ) ;
	sprintf( acBuffer,"%s %u %d", M4_EXT_CORE_GENERATOR, idPID, M4Thread::GetCurrentIdThread() ) ;

	// Executes the external script.
	system( acBuffer ) ;
}

void FirstChanceException(int ai_isignal) {
	m4Trace(printf("%p: FirstChanceException. Signal: %d\n",M4Thread::GetCurrentIdThread(), ai_isignal))
	string Description;
	signal(ai_isignal, LastChanceException); //Por si hay problemas en el tratamiento
	unsigned int M4Exc;
	char p[100];

	// Recolectamos información del proceso.
	if( ai_isignal != SIGTERM )
	{
		InvokeExternalDebugInfoCollector() ;
	}

	ClThreadObjects::RemoveThread();
	
	switch (ai_isignal) {
	case SIGHUP:
		Description = "Hangup";
		M4Exc = M4EXCEPTION_EXIT;
		break;
	case SIGINT:
		Description = "Interrup";
		M4Exc = M4EXCEPTION_EXIT;
		break;
	case SIGQUIT:
		Description = "quit";
		M4Exc = M4EXCEPTION_EXIT;
		break;
	case SIGILL:
		Description = "Illegal instruction";
		M4Exc = M4EXCEPTION_ILLEGAL_INSTRUCTION;
		break;
	case SIGTRAP:
		Description = "Trace trap";
		M4Exc = M4EXCEPTION_TRAP_POLL;
		break;
	case SIGABRT:  //case SIGIOT
		Description = "Abort or IOT Instruction";
		M4Exc = M4EXCEPTION_ABRT;
		break;
	case SIGEMT:
		Description = "EMT Instruction";
		M4Exc = M4EXCEPTION_INSTRUCTION;
		break;
	case SIGFPE:
		Description = "Floating point exception";
		M4Exc = M4EXCEPTION_FPE;
		break;
	case SIGBUS:
		Description = "Bus error";
		M4Exc = M4EXCEPTION_BUS;
		break;
	case SIGSEGV:
		M4Exc = M4EXCEPTION_ACCESS_VIOLATION;
		Description = "Segmentation violation";
		break;
	case SIGSYS:
		Description = "Bad argument to system call";
		M4Exc = M4EXCEPTION_ILLEGAL_INSTRUCTION;
		break;
	case SIGPIPE:
		Description = "Write on a pipe with no one to read it";
		M4Exc = M4EXCEPTION_PIPE;
		break;
	case SIGALRM:
		Description = "Alarm clock";
		M4Exc = M4EXCEPTION_TIMER;
		break;
	case SIGTERM:
		Description = "Software termination signal from kill";
		M4Exc = M4EXCEPTION_TERMINATE;
		break;
	case SIGUSR1:
		Description = "User defined signal 1";
		M4Exc = M4EXCEPTION_USER;
		break;
	case SIGUSR2:
		Description = "User defined signal 2";
		M4Exc = M4EXCEPTION_USER;
		break;
	case SIGPOLL: //case SIGIO
		Description = "Pollable event occurred";
		M4Exc = M4EXCEPTION_TRAP_POLL;
		break;
	case SIGVTALRM:
		Description = "Virtual Timer expired";
		M4Exc = M4EXCEPTION_TIMER;
		break;
	case SIGPROF:
		Description = "Profiling timer expired";
		M4Exc = M4EXCEPTION_TIMER;
		break;
	case SIGXCPU:
		Description = "Exceeded cpu limit";
		M4Exc = M4EXCEPTION_EXCEEDED;
		break;
	case SIGXFSZ:
		Description = "Exceeded file size limit";
		M4Exc = M4EXCEPTION_EXCEEDED;
		break;
	default:
		sprintf(p, "%d\0", ai_isignal);
		Description = p;
		M4Exc = M4EXCEPTION_UNDEFINED;
	}

	M4Thread *pM4Thread=NULL;
	if ((M4Thread::GetInstance(M4Thread::GetCurrentIdThread(), pM4Thread) == M4_SUCCESS) && (pM4Thread)) {
		if (pM4Thread->GetUseException() == M4_TRUE) {		
			if ((M4Exc != M4EXCEPTION_IGNORED) && (M4Exc & pM4Thread->GetExceptionMask())) {
				//ADD FJSM 990404 Evitar bucles infinitos
				if (pM4Thread->m_iNumException < MAX_NUM_EXCEPTIONS) {
					if (pM4Thread->m_iLastException == M4Exc)
						pM4Thread->m_iNumException ++;
					else {
						pM4Thread->m_iNumException=1;
						pM4Thread->m_iLastException = M4Exc;
					}
				//END ADD
					signal(ai_isignal,  FirstChanceException );
					m4Trace(printf("%p: FirstChanceException. Throw Signal: %d\n",M4Thread::GetCurrentIdThread(), M4Exc))
					// Si hay funcion throw registrada se utiliza esa
					M4ThrowFunction_t pfThrowFunction = pM4Thread->GetThrowFunction() ;

					// fix bug 0079564
					M4RuntimeException e(M4Exc, Description);
					M4CheckJump::Throw(e);

					if( pfThrowFunction != NULL )
					{
						pfThrowFunction( e ) ;
					}
					else
					{
						throw e;
					}

				//ADD FJSM 990404 Evitar bucles infinitos
				}
				else
					LastChanceException(ai_isignal);
				//END ADD
			}
//MOD FJSM 990427			
			else {
				//Este señal no la tenemos enmascarada,
				//luego establecemos lo de por defecto y relanzamos
				signal(ai_isignal, SIG_DFL);
				raise(ai_isignal);
			}
		}
		else {
			//Este thread no tiene manejo de excepciones,
			//luego establecemos lo de por defecto y relanzamos
			m4Trace(printf("%p: FirstChanceException. Catch Signal in a thread without Exceptions: %u\n",M4Thread::GetCurrentIdThread(), M4Exc))
			signal(ai_isignal, SIG_DFL);
			raise(ai_isignal);
		}
	}
	else {
		//La cosa esta muy fea
		//luego establecemos lo de por defecto y relanzamos
		signal(ai_isignal, SIG_DFL);
		raise(ai_isignal);
	}
//END MOD FJSM 990427
}

void TerminateChanceException(void) {
	m4Trace(printf("%p: TerminateChanceException\n",M4Thread::GetCurrentIdThread()))
	LastChanceException(M4EXCEPTION_TERMINATE);
}

void LastChanceException(int ai_isignal) {
	m4Trace(printf("%p: LastChanceException\n",M4Thread::GetCurrentIdThread()))

	M4Thread *pM4Thread=NULL;
	if ((M4Thread::GetInstance(M4Thread::GetCurrentIdThread(), pM4Thread) == M4_SUCCESS) && (pM4Thread)) {
		M4UserHandler_t pFunc;
		m4pvoid_t param=NULL;
		pFunc = pM4Thread->GetUserHandler(param);
		if (pFunc)
			pFunc(param);
	
		M4Thread::DeregisterThread(pM4Thread->GetIdThread());
		pM4Thread->SetExit();
	}

	//ADD FJSM 990404
	signal(ai_isignal,  FirstChanceException ); //Reestablecemos el manejador para el resto de threads
	//END ADD

	longjmp(cancelExecutionThread, 1); //Saltamos a la funcion Func2, justo despues de la llamada a Run
}
#else
///////////////////////////
//WINDOWS
////////////////////////////
#define M4_EXT_CORE_GENERATOR "m4dump.bat"
void InvokeExternalDebugInfoCollector()
{
	char acBuffer[ 256 ] ;
	m4iden_t idPID = getpid() ;
	time_t lNow = time( NULL ) ;

	
	m4Trace( printf( "%lu: FirstChanceException. Launching %s\n", M4Thread::GetCurrentIdThread(), M4_EXT_CORE_GENERATOR ) ) ;
	sprintf( acBuffer,"%s %u %ld %ld", M4_EXT_CORE_GENERATOR, idPID, M4Thread::GetCurrentIdThread(), lNow ) ;

	// Executes the external script.
	system( acBuffer ) ;
}
void TerminateChanceException(void) {
	m4Trace(printf("%lu: TerminateChanceException\n",M4Thread::GetCurrentIdThread()))
	LastChanceException(M4EXCEPTION_TERMINATE, 0);
}

void LastChanceException(unsigned int ai_isignal, EXCEPTION_POINTERS* p) {
	m4Trace(printf("%lu: LastChanceException\n",M4Thread::GetCurrentIdThread()))
	M4Thread *pM4Thread=NULL;
	if ((M4Thread::GetInstance(M4Thread::GetCurrentIdThread(), pM4Thread) == M4_SUCCESS) && (pM4Thread)) {
		M4UserHandler_t pFunc;
		m4pvoid_t param=NULL;
		pFunc = pM4Thread->GetUserHandler(param);
		if (pFunc)
			pFunc(param);
	
		M4Thread::DeregisterThread(pM4Thread->GetIdThread());
		pM4Thread->SetExit();
	}

	_endthreadex( 0 );
}

void FirstChanceException(unsigned int ai_isignal, EXCEPTION_POINTERS* p){

	m4Trace(printf("%lu: FirstChanceException. Signal:%u\n",M4Thread::GetCurrentIdThread(), ai_isignal))

	_set_se_translator( LastChanceException ); // Por si hay problemas en el tratamiento

	InvokeExternalDebugInfoCollector();

	ClThreadObjects::RemoveThread();

	unsigned int M4Exc;
	string Description;
	switch (ai_isignal) {
	case EXCEPTION_GUARD_PAGE:
		Description = "Memory Allocated as PAGE_GUARD by VirtualAlloc has been accesed";
		M4Exc = M4EXCEPTION_IGNORED;
		break;
	case EXCEPTION_DATATYPE_MISALIGNMENT:
		Description = "The thread tried to read or write data that is misaligned";
		M4Exc = M4EXCEPTION_IGNORED;
		break;
	case EXCEPTION_BREAKPOINT:
		Description = "A breakpoint was encountered";
		M4Exc = M4EXCEPTION_IGNORED;
		break;
	case EXCEPTION_SINGLE_STEP:
		Description = "The CPU has signaled that one instruction has been executed";
		M4Exc = M4EXCEPTION_IGNORED;
		break;
	case EXCEPTION_ACCESS_VIOLATION:
		Description = "Access violation";
		M4Exc = M4EXCEPTION_ACCESS_VIOLATION;
		break;
	case EXCEPTION_IN_PAGE_ERROR:
		Description = "In Page Error";
		M4Exc = M4EXCEPTION_IN_PAGE_ERROR;
		break;
	case STATUS_INVALID_HANDLE:
		Description = "Invalid handle";
		M4Exc = M4EXCEPTION_INVALID_HANDLE;
		break;
	case STATUS_NO_MEMORY:
		Description = "No memory or heap corruption";
		M4Exc = M4EXCEPTION_ACCESS_VIOLATION;
		break;
	case EXCEPTION_ILLEGAL_INSTRUCTION:
		Description = "Illegal instruction";
		M4Exc = M4EXCEPTION_ILLEGAL_INSTRUCTION;
		break;
	case EXCEPTION_NONCONTINUABLE_EXCEPTION:
		Description = "Noncontinuable exception";
		M4Exc = M4EXCEPTION_UNDEFINED;
		break;
	case EXCEPTION_INVALID_DISPOSITION:
		Description = "Invalid Disposition";
		M4Exc = M4EXCEPTION_UNDEFINED;
		break;
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		M4Exc = M4EXCEPTION_ARRAY_BOUNDS_EXCEDEED;
		Description = "Array bounds excedeed";
		break;
	case EXCEPTION_FLT_DENORMAL_OPERAND:
		M4Exc = M4EXCEPTION_FPE;
		Description = "Float denormal operand";
		break;
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		Description = "Float divide by zero";
		M4Exc = M4EXCEPTION_FPE;
		break;
	case EXCEPTION_FLT_INEXACT_RESULT:
		Description = "Float inexact result";
		M4Exc = M4EXCEPTION_FPE;
		break;
	case EXCEPTION_FLT_INVALID_OPERATION:
		Description = "Float invalid operation";
		M4Exc = M4EXCEPTION_FPE;
		break;
	case EXCEPTION_FLT_OVERFLOW:
		Description = "Float overflow";
		M4Exc = M4EXCEPTION_FPE;
		break;
	case EXCEPTION_FLT_STACK_CHECK:
		Description = "Float stack check";
		M4Exc = M4EXCEPTION_FPE;
		break;
	case EXCEPTION_FLT_UNDERFLOW:
		Description = "Float underflow";
		M4Exc = M4EXCEPTION_FPE;
		break;
	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		Description = "Int divide by zero";
		M4Exc = M4EXCEPTION_FPE;
		break;
	case EXCEPTION_INT_OVERFLOW:
		Description = "Int overflow";
		M4Exc = M4EXCEPTION_FPE;
		break;
	case EXCEPTION_PRIV_INSTRUCTION:
		Description = "Private instruction";
		M4Exc = M4EXCEPTION_PRIV_INSTRUCTION;
		break;
	case EXCEPTION_STACK_OVERFLOW:
		Description = "Stack overflow";
		M4Exc = M4EXCEPTION_STACK_OVERFLOW;
		break;
	case 0xE06D7363: //Visual C++ exception
		Description = "Visual C++ exception";
		M4Exc = M4EXCEPTION_UNDEFINED;
		break;
	case 0xC000021a: //unexpected windows subsystem termination
		Description = "Unexpected windows subsystem termination";
		M4Exc = M4EXCEPTION_TERMINATE;
		break;
	default:
		Description = ai_isignal;
		M4Exc = M4EXCEPTION_UNDEFINED;
	}

	M4Thread *pM4Thread=NULL;
	if ((M4Thread::GetInstance(M4Thread::GetCurrentIdThread(), pM4Thread) == M4_SUCCESS) && (pM4Thread)) {
		//ADD FJSM 13-10-99
		//Comprobamos si esta activo el uso de excecepciones
		if (pM4Thread->GetUseException() == M4_TRUE) {
		//END ADD
			//Siempre restauramos el manejador, para que siempre si esta en la mascara
			//se lance una excepcion C++
			if ((M4Exc != M4EXCEPTION_IGNORED) && (M4Exc & pM4Thread->GetExceptionMask())) {
					//Una excepcion C la convertimos a excepcion C++
					_set_se_translator( FirstChanceException );
					m4Trace(printf("%lu: FirstChanceException. Throw Signal: %u\n",M4Thread::GetCurrentIdThread(), M4Exc))
					M4RuntimeException e(M4Exc, Description);
					M4CheckJump::Throw(e);
					throw e;
			}
			else
				_set_se_translator( FirstChanceException );
		}
		else {
			//Este thread no tiene manejo de excepciones,
			m4Trace(printf("%lu: FirstChanceException. Catch Signal in a thread without Exceptions: %u\n",M4Thread::GetCurrentIdThread(), M4Exc))
			_set_se_translator( FirstChanceException );
		}
	}
	else
		_set_se_translator( FirstChanceException );
}
#endif 
//## end module%378CB242024C.epilog
