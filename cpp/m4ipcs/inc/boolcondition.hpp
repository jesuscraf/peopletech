//## begin module%3742D01E02A9.cm preserve=no
//## end module%3742D01E02A9.cm

//## begin module%3742D01E02A9.cp preserve=no
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
//## end module%3742D01E02A9.cp

//## Module: boolcondition%3742D01E02A9; Package specification
//## Subsystem: M4Ipcs::inc%3742D01E02A7
//	d:\fuentesservidor\m4ipcs\version\inc
//## Source file: D:\eduardoma\m4ipcs\inc\boolcondition.hpp

#ifndef booleancondition_h
#define booleancondition_h 1

//## begin module%3742D01E02A9.additionalIncludes preserve=no
//## end module%3742D01E02A9.additionalIncludes

//## begin module%3742D01E02A9.includes preserve=yes
#ifdef _UNIX
	#include <pthread.h>
#endif
#include <m4ipcs_dll.hpp>
//## end module%3742D01E02A9.includes

// m4types
#include <m4types.hpp>
// CriticalSection
#include <criticalsection.hpp>
// syncro
#include <syncro.hpp>
//## begin module%3742D01E02A9.declarations preserve=no
//## end module%3742D01E02A9.declarations

//## begin module%3742D01E02A9.additionalDeclarations preserve=yes
//## end module%3742D01E02A9.additionalDeclarations


//## begin M4Cond_t%3743DA680391.preface preserve=yes
#ifdef _WINDOWS
//## end M4Cond_t%3743DA680391.preface

//## Class: M4Cond_t%3743DA680391
//## Category: M4Ipcs%3742D01E02E3
//## Subsystem: M4Ipcs::inc%3742D01E02A7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef ClSemaphore M4Cond_t;

//## begin M4Cond_t%3743DA680391.postscript preserve=yes
#else
typedef pthread_cond_t M4Cond_t;
#endif
//## end M4Cond_t%3743DA680391.postscript

//## begin ClBooleanCondition%3742D01E02E4.preface preserve=yes
//## end ClBooleanCondition%3742D01E02E4.preface

//## Class: ClBooleanCondition%3742D01E02E4
//	Clase de sincronización con time-out.
//
//	Ejemplo de uso:
//
//	Lock();
//	while (condition)
//	     Wait();
//	//modificar condition;
//	Unlock();
//	//Zona proegida que tendrá tantos threads según
//	//tengamos definida la condition
//	Lock();
//	Notify();
//	//modificar condition
//	Unlock();
//## Category: M4Ipcs%3742D01E02E3
//## Subsystem: M4Ipcs::inc%3742D01E02A7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3742D01F026A;m4return_t { -> }

class M4_DECL_M4IPCS ClBooleanCondition : public ClSynObject  //## Inherits: <unnamed>%3742D01F0285
{
  //## begin ClBooleanCondition%3742D01E02E4.initialDeclarations preserve=yes
  //## end ClBooleanCondition%3742D01E02E4.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClBooleanCondition%1917792126; C++
      //	Constructor que puede recibir un puntero a un objeto ClMutex. Si no se le
      //	pasa ninguno se crea un mutex interno.
      //	La utilización de un mutex externo es util cuando por ejemplo se necesitan 2
      //	condition para una misma zona protegida.
      //	Ej: Colas, push y pop.
      ClBooleanCondition (ClSynObject *ai_pMutex = NULL);

    //## Destructor (specified)
      //## Operation: ~ClBooleanCondition%110867509; C++
      ~ClBooleanCondition ();


    //## Other Operations (specified)
      //## Operation: Init%1876779477; C++
      //	Función de inicialización interna.
      //	Se necesita llamar para incializar mutex, condition, etc... antes de poderse
      //	utilizar la clase.
      m4return_t Init ();

      //## Operation: Lock%32644181; C++
      //	Función para iniciar una zona protegida. Coge el mutex general.
      m4return_t Lock (void );

      //## Operation: Wait%-770769245; C++
      //	Esta función ha de llamarse despues de Lock.
      //
      //	Se debe llamar dendro de un while de comprobación de la condición, ya que
      //	desde que le despierten hasta que tome posesión del mutex general, puede
      //	entrar otro thread, y cambiar la condición de espera.
      //
      //	Recibe un parámetro que es el tiempo de time-out. Por defecto tiene el valor
      //	0 que indica que espera de forma infinita.
      //
      //	Cuando se duerme, se libera el mutex general, y al despertarse obtenie el
      //	mutex general.
      //
      //	En caso de despertarse por time-out, de devuelve M4_WARNING.
      m4return_t Wait (m4uint32_t ai_uiTimeOut = 0);

      //## Operation: Unlock%1580498493; C++
      //	Libera el mutex general.
      m4return_t Unlock (void );

      //## Operation: Notify%927545397; C++
      //	Se debe invocar entre un Lock y un Unlock.
      //
      //	Recibe un parámetro que nos indica si despierta un thread o todos (por
      //	defecto despierta a uno, y por lo tanto tiene el valor del parámetro a M4_
      //	FALSE).
      //
      //	Enviará una señal solamente si hay un thread esperando. Y en caso de enviar
      //	la señal espera confirmación de que el thread se ha despertado.
      m4return_t Notify (m4bool_t ai_bAll = M4_FALSE);

    // Additional Public Declarations
      //## begin ClBooleanCondition%3742D01E02E4.public preserve=yes
      //## end ClBooleanCondition%3742D01E02E4.public

  protected:
    // Data Members for Associations

      //## Association: M4Ipcs::<unnamed>%3743BDBD00DB
      //## Role: ClBooleanCondition::m_oWait%3743BDBE00CA
      //## begin ClBooleanCondition::m_oWait%3743BDBE00CA.role preserve=no  protected: M4Cond_t {1 -> 1VHAN}
      M4Cond_t m_oWait;
      //## end ClBooleanCondition::m_oWait%3743BDBE00CA.role

      //## Association: M4Ipcs::<unnamed>%3743BF98030F
      //## Role: ClBooleanCondition::m_oHandShake%3743BF99031C
      //## begin ClBooleanCondition::m_oHandShake%3743BF99031C.role preserve=no  protected: M4Cond_t { -> 1VHAN}
      M4Cond_t m_oHandShake;
      //## end ClBooleanCondition::m_oHandShake%3743BF99031C.role

      //## Association: M4Ipcs::<unnamed>%3743CE0402DC
      //## Role: ClBooleanCondition::m_bInitialized%3743CE0501B2
      //## begin ClBooleanCondition::m_bInitialized%3743CE0501B2.role preserve=no  protected: m4bool_t { -> 1VHAN}
      m4bool_t m_bInitialized;
      //## end ClBooleanCondition::m_bInitialized%3743CE0501B2.role

      //## Association: M4Ipcs::<unnamed>%3743D257032E
      //## Role: ClBooleanCondition::m_uiWaiting%3743D2580394
      //## begin ClBooleanCondition::m_uiWaiting%3743D2580394.role preserve=no  protected: m4uint32_t { -> 1VHAN}
      m4uint32_t m_uiWaiting;
      //## end ClBooleanCondition::m_uiWaiting%3743D2580394.role

      //## Association: M4Ipcs::<unnamed>%37454B460332
      //## Role: ClBooleanCondition::m_bSignal%37454B4800C8
      //## begin ClBooleanCondition::m_bSignal%37454B4800C8.role preserve=no  protected: m4uint32_t { -> 1VHAN}
      m4uint32_t m_bSignal;
      //## end ClBooleanCondition::m_bSignal%37454B4800C8.role

      //## Association: M4Ipcs::<unnamed>%37493AFD00D4
      //## Role: ClBooleanCondition::m_bOwner%37493AFE00C2
      //## begin ClBooleanCondition::m_bOwner%37493AFE00C2.role preserve=no  protected: m4bool_t { -> 1VHAN}
      m4bool_t m_bOwner;
      //## end ClBooleanCondition::m_bOwner%37493AFE00C2.role

      //## Association: M4Ipcs::<unnamed>%392A498502A4
      //## Role: ClBooleanCondition::m_oInternalMutex%392A498502A5
      //## begin ClBooleanCondition::m_oInternalMutex%392A498502A5.role preserve=no  protected: ClCriticalSection { -> 1VHAN}
      ClCriticalSection m_oInternalMutex;
      //## end ClBooleanCondition::m_oInternalMutex%392A498502A5.role

      //## Association: M4Ipcs::<unnamed>%392A49920307
      //## Role: ClBooleanCondition::m_pCheckMutex%392A49920308
      //## begin ClBooleanCondition::m_pCheckMutex%392A49920308.role preserve=no  protected: ClSynObject { -> 1RHAN}
      ClSynObject *m_pCheckMutex;
      //## end ClBooleanCondition::m_pCheckMutex%392A49920308.role

    // Additional Protected Declarations
      //## begin ClBooleanCondition%3742D01E02E4.protected preserve=yes
      //## end ClBooleanCondition%3742D01E02E4.protected

  private:
    // Additional Private Declarations
      //## begin ClBooleanCondition%3742D01E02E4.private preserve=yes
      //## end ClBooleanCondition%3742D01E02E4.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClBooleanCondition%3742D01E02E4.implementation preserve=yes
      //## end ClBooleanCondition%3742D01E02E4.implementation

};

//## begin ClBooleanCondition%3742D01E02E4.postscript preserve=yes
//## end ClBooleanCondition%3742D01E02E4.postscript

// Class ClBooleanCondition 

//## begin module%3742D01E02A9.epilog preserve=yes
//## end module%3742D01E02A9.epilog


#endif
