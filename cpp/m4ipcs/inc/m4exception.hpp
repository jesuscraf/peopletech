//## begin module%378CB08B002E.cm preserve=no
//## end module%378CB08B002E.cm

//## begin module%378CB08B002E.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.hpp
//	 Project:            M4ComSrv
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
//	=
//## end module%378CB08B002E.cp

//## Module: m4exception%378CB08B002E; Package specification
//## Subsystem: M4Ipcs::inc%3742D01E02A7
//	d:\fuentesservidor\m4ipcs\version\inc
//## Source file: D:\FuentesServidor\m4ipcs\version\inc\m4exception.hpp

#ifndef m4exception_h
#define m4exception_h 1

//## begin module%378CB08B002E.additionalIncludes preserve=no
//## end module%378CB08B002E.additionalIncludes

//## begin module%378CB08B002E.includes preserve=yes
#include <m4ipcs_dll.hpp>
//## end module%378CB08B002E.includes

// m4types
#include <m4types.hpp>
#include "m4stl.hpp"

//## begin module%378CB08B002E.declarations preserve=no
//## end module%378CB08B002E.declarations

//## begin module%378CB08B002E.additionalDeclarations preserve=yes
//constantes para establecer las mascaras de señales que deseamos captar
const m4uint32_t M4EXCEPTION_ABRT = 0x00000002; //SIGIOT, SIGABRT. Abnormal termination
const m4uint32_t M4EXCEPTION_FPE = 0x00000004; //Floating-point error
const m4uint32_t M4EXCEPTION_TERMINATE = 0x00000008; //Termination request sent to the program
const m4uint32_t M4EXCEPTION_ACCESS_VIOLATION = 0x00000010; //Illegal storage access
const m4uint32_t M4EXCEPTION_IN_PAGE_ERROR = 0x00000020; //System can't load the page
const m4uint32_t M4EXCEPTION_INVALID_HANDLE = 0x00000040; //handle has already been closed
const m4uint32_t M4EXCEPTION_ILLEGAL_INSTRUCTION = 0x00000080;	//SIGSYS, Illegal instruction
const m4uint32_t M4EXCEPTION_ARRAY_BOUNDS_EXCEDEED = 0x00000100; //x86 family processors no generate
const m4uint32_t M4EXCEPTION_PRIV_INSTRUCTION = 0x00000800;
const m4uint32_t M4EXCEPTION_STACK_OVERFLOW = 0x00001000;
const m4uint32_t M4EXCEPTION_CONTROL_C = 0x00002000; //CTRL+C interrupt
const m4uint32_t M4EXCEPTION_IGNORED = 0x00004000;		//Uso interno, la excepcion se ignorará
const m4uint32_t M4EXCEPTION_EXIT = 0x00010000; //hungup, interrupt, quit ...
const m4uint32_t M4EXCEPTION_TIMER = 0x00020000;  //sigvtalrm, sigprof
const m4uint32_t M4EXCEPTION_EXCEEDED = 0x00040000;  //sigxcpu, sigxfsz
const m4uint32_t M4EXCEPTION_TRAP_POLL = 0x00080000; //SIGTRAP, SIGPOLL, SIGIO
const m4uint32_t M4EXCEPTION_INSTRUCTION = 0x00100000; //SIGEMT
const m4uint32_t M4EXCEPTION_BUS = 0x00200000; //SIGBUS
const m4uint32_t M4EXCEPTION_PIPE = 0x00400000;//SIGPIPE
const m4uint32_t M4EXCEPTION_USER = 0x00800000; //SIGUSER1, SIGUSER2
const m4uint32_t M4EXCEPTION_NONE	=	0x00000000;	//No cogemos ninguna señal

//Cuando se produce una excepcion no contemplada.
//Inicialmente se ignoran (no se lanza el throw)
const m4uint32_t M4EXCEPTION_UNDEFINED = 0x00008000;	

//Cogemos todas las señales, incluido M4EXCEPTION_UNDEFINED
const m4uint32_t M4EXCEPTION_ALL	=	0xFFFFFFFF;

//Funciones para el manejo de señales
#ifdef _UNIX
	//ADD FJSM 990404 Numero máximo de GP's consecutivos del mismo tipo
	const m4uint32_t MAX_NUM_EXCEPTIONS		=	5;
	//END ADD
#ifdef _SOLARIS
	#include <ospace/std/exception> //necesario para el prototipo de set_terminateC en solaris
    using std::set_terminate;
#endif
extern "C"
{
    void FirstChanceException(m4int_t ai_isignal);
	void LastChanceException(m4int_t ai_isignal);
}
#endif
#ifdef _WINDOWS
	#include <windows.h> //necesario para EXCEPTION_POINTERS
	#include <eh.h> //prototipo de set_terminate, _set_se_translator ...
	void FirstChanceException(unsigned int ai_isignal, EXCEPTION_POINTERS* p);
	void LastChanceException(unsigned int ai_isignal, EXCEPTION_POINTERS* p);
#endif

void TerminateChanceException();

//Clase para manejar todo tipo de excepciones de Runtime
#ifdef OS_OBJECTSPACE
	#define M4runtime_error os_runtime_error
#else
	#define M4runtime_error runtime_error
#endif
//## end module%378CB08B002E.additionalDeclarations


//## begin M4RuntimeException%3742D0210049.preface preserve=yes
//## end M4RuntimeException%3742D0210049.preface

//## Class: M4RuntimeException%3742D0210049
//## Category: M4Ipcs%3742D01E02E3
//## Subsystem: M4Ipcs::inc%3742D01E02A7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4IPCS M4RuntimeException : public M4runtime_error  //## Inherits: <unnamed>%3742D02200CE
{
  //## begin M4RuntimeException%3742D0210049.initialDeclarations preserve=yes
  //## end M4RuntimeException%3742D0210049.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: M4RuntimeException%-998325366; C++
      //## Exceptions: ()
      M4RuntimeException (m4uint32_t ai_uiException, string Des)
        //## begin M4RuntimeException::M4RuntimeException%-998325366.hasinit preserve=no
        //## end M4RuntimeException::M4RuntimeException%-998325366.hasinit
        //## begin M4RuntimeException::M4RuntimeException%-998325366.initialization preserve=yes
: M4runtime_error(Des) 
        //## end M4RuntimeException::M4RuntimeException%-998325366.initialization
      {
        //## begin M4RuntimeException::M4RuntimeException%-998325366.body preserve=yes
		  m_uiException = ai_uiException;
        //## end M4RuntimeException::M4RuntimeException%-998325366.body
      }


    //## Other Operations (specified)
      //## Operation: GetException%1945516831; C++
      m4uint32_t GetException ()
      {
        //## begin M4RuntimeException::GetException%1945516831.body preserve=yes
		  return m_uiException;
        //## end M4RuntimeException::GetException%1945516831.body
      }

    // Additional Public Declarations
      //## begin M4RuntimeException%3742D0210049.public preserve=yes
      //## end M4RuntimeException%3742D0210049.public

  protected:
    // Data Members for Associations

      //## Association: M4Ipcs::<unnamed>%3743D1DA0360
      //## Role: M4RuntimeException::m_uiException%3743D1DB0146
      //## begin M4RuntimeException::m_uiException%3743D1DB0146.role preserve=no  protected: m4uint32_t { -> 1VHAN}
      m4uint32_t m_uiException;
      //## end M4RuntimeException::m_uiException%3743D1DB0146.role

    // Additional Protected Declarations
      //## begin M4RuntimeException%3742D0210049.protected preserve=yes
      //## end M4RuntimeException%3742D0210049.protected

  private:
    // Additional Private Declarations
      //## begin M4RuntimeException%3742D0210049.private preserve=yes
      //## end M4RuntimeException%3742D0210049.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin M4RuntimeException%3742D0210049.implementation preserve=yes
      //## end M4RuntimeException%3742D0210049.implementation

};

//## begin M4RuntimeException%3742D0210049.postscript preserve=yes
//## end M4RuntimeException%3742D0210049.postscript

//## begin module%378CB08B002E.epilog preserve=yes
//## end module%378CB08B002E.epilog


#endif
