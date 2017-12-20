//## begin module%36D3C65C003F.cm preserve=no
//## end module%36D3C65C003F.cm

//## begin module%36D3C65C003F.cp preserve=no
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
//## end module%36D3C65C003F.cp

//## Module: serverevent%36D3C65C003F; Package specification
//## Subsystem: M4ServerEvent::inc%379434EF0090
//## Source file: F:\integration\m4serverevent\inc\serverevent.hpp

#ifndef serverevent_h
#define serverevent_h 1

//## begin module%36D3C65C003F.additionalIncludes preserve=no
#include "m4serverevent_dll.hpp"
//## end module%36D3C65C003F.additionalIncludes

//## begin module%36D3C65C003F.includes preserve=yes
//## end module%36D3C65C003F.includes

// syncro
#include "syncro.hpp"

class M4_DECL_M4SERVEREVENT ClServerEvent;

//## begin module%36D3C65C003F.declarations preserve=no
//## end module%36D3C65C003F.declarations

//## begin module%36D3C65C003F.additionalDeclarations preserve=yes
//## end module%36D3C65C003F.additionalDeclarations


//## begin OBSERVERS%35BF4A3D01AB.preface preserve=yes
//## end OBSERVERS%35BF4A3D01AB.preface

//## Class: OBSERVERS%35BF4A3D01AB
//	Lista de objetos ClServerEnvent
//## Category: M4ServerEvent%36D3C9E50311
//## Subsystem: M4ServerEvent::inc%379434EF0090
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%35BF789F00E7;list { -> }
//## Uses: <unnamed>%36D3C5820371;ClServerEvent { -> F}

typedef list < ClServerEvent *  > OBSERVERS;

//## begin OBSERVERS%35BF4A3D01AB.postscript preserve=yes
//## end OBSERVERS%35BF4A3D01AB.postscript

//## begin EVENTTYPE%35BF850C031C.preface preserve=yes
//## end EVENTTYPE%35BF850C031C.preface

//## Class: EVENTTYPE%35BF850C031C
//	Enumerado en el que se definen los diferentes tipos de eventos que se pueden
//	producir en el servidor
//## Category: M4ServerEvent%36D3C9E50311
//## Subsystem: M4ServerEvent::inc%379434EF0090
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef enum { CLOSE_SESSION , LDB_RYPTURE_CONNECTION , CANCELED_REQUEST , SERVICE_SHUTDOWN , GREEN_MEMORY_LEVEL , YELLOW_MEMORY_LEVEL , RED_MEMORY_LEVEL , MEMORY_ALERT , SERVER_READY , ADMIN_SHUTDOWN , TCP_PORT_BUSY , CONFIG_ERROR, LDB_SHUTDOWN , SHUTTING_DOWN , SERVER_STOPPED , REQUEST_CANCELED } EVENTTYPE;

//## begin EVENTTYPE%35BF850C031C.postscript preserve=yes
//## end EVENTTYPE%35BF850C031C.postscript

//## begin ClServerEvent%36D3C3DF024C.preface preserve=yes
//## end ClServerEvent%36D3C3DF024C.preface

//## Class: ClServerEvent%36D3C3DF024C
//	Esta clase implementa unmecanismo para la notificación de eventos de unos
//	objetos a otros de forma síncrona. Cada objeto es tanto quien puede lanzar
//	el evento como recibirlo. Para ello cada uno de ellos tiene una lista tanto
//	de a quien ha de notificar el evento que el produzca como de qienes va a
//	recibir eventos (esta última lista es útil a la hora de desregistrarse)
//## Category: M4ServerEvent%36D3C9E50311
//## Subsystem: M4ServerEvent::inc%379434EF0090
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37444002039B;EVENTTYPE { -> }

class M4_DECL_M4SERVEREVENT ClServerEvent
{
  //## begin ClServerEvent%36D3C3DF024C.initialDeclarations preserve=yes
  //## end ClServerEvent%36D3C3DF024C.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClServerEvent%919850042
      ClServerEvent ();

    //## Destructor (specified)
      //## Operation: ~ClServerEvent%919850043
      ~ClServerEvent ();


    //## Other Operations (specified)
      //## Operation: AttachObserver%919850044
      //	Añade al objeto un observador al que notificar los eventos que se produzcan
      //	en él.
      virtual void AttachObserver (ClServerEvent *ai_poObserver);

      //## Operation: DetachObserver%919850045
      //	DEsregistra un observador de la lista de observadores del objeto en el que
      //	se invoca,.
      virtual void DetachObserver (ClServerEvent *ai_poObserver);

      //## Operation: Notify%919850046
      //	Notifica un evneto producido a la lista de observadores del objeto
      virtual void Notify (EVENTTYPE ai_iEvent);

      //## Operation: Update%919850047
      //	Método invocado por el notificador del evento en el observador. Este es el
      //	método que se entera del evento que se ha producido
      virtual void Update (EVENTTYPE ai_iEvent, ClServerEvent *ai_oSubject);

      //## Operation: RegisterObserved%945425942
      //	Registra un objeto observado (el cual puede llegar a enviar un evento)
      void RegisterObserved (ClServerEvent *ai_poObservedSubject);

      //## Operation: UnregisterObserved%945425943
      //	Desregistra un objeto observado.
      void UnregisterObserved (ClServerEvent *ai_poObservedSubject);

    // Additional Public Declarations
      //## begin ClServerEvent%36D3C3DF024C.public preserve=yes
      //## end ClServerEvent%36D3C3DF024C.public

  protected:
    // Data Members for Associations

      //## Association: M4ServerEvent::<unnamed>%36D3D35701A6
      //## Role: ClServerEvent::m_oObservers%36D3D3580248
      //## begin ClServerEvent::m_oObservers%36D3D3580248.role preserve=no  protected: OBSERVERS { -> VHAN}
      OBSERVERS m_oObservers;
      //## end ClServerEvent::m_oObservers%36D3D3580248.role

    // Additional Protected Declarations
      //## begin ClServerEvent%36D3C3DF024C.protected preserve=yes
      //## end ClServerEvent%36D3C3DF024C.protected

  private:
    // Data Members for Associations

      //## Association: M4ServerEvent::<unnamed>%385A07B3019C
      //## Role: ClServerEvent::m_oObservedSubjects%385A07B400E9
      //	Cada ClServerEvent tiene una lista de objetos del mismo tipo que son
      //	observados por el objeto que contiene la lista. Es decir es una lista de Cl
      //	ServerEvents observados
      //## begin ClServerEvent::m_oObservedSubjects%385A07B400E9.role preserve=no  private: OBSERVERS { -> VHAN}
      OBSERVERS m_oObservedSubjects;
      //## end ClServerEvent::m_oObservedSubjects%385A07B400E9.role

      //## Association: M4ServerEvent::<unnamed>%385A0DAF034D
      //## Role: ClServerEvent::m_oMutexObserver%385A0DB001B4
      //	Mutex para sincronizar el acceso al mapa de observadores
      //## begin ClServerEvent::m_oMutexObserver%385A0DB001B4.role preserve=no  private: ClMutex { -> VHAN}
      ClMutex m_oMutexObserver;
      //## end ClServerEvent::m_oMutexObserver%385A0DB001B4.role

      //## Association: M4ServerEvent::<unnamed>%385A225C01D2
      //## Role: ClServerEvent::m_oMutexObserved%385A225D001A
      //	mutex para sincronizar el acceso al mapa de observados
      //## begin ClServerEvent::m_oMutexObserved%385A225D001A.role preserve=no  private: ClMutex { -> VHAN}
      ClMutex m_oMutexObserved;
      //## end ClServerEvent::m_oMutexObserved%385A225D001A.role

    // Additional Private Declarations
      //## begin ClServerEvent%36D3C3DF024C.private preserve=yes
      //## end ClServerEvent%36D3C3DF024C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClServerEvent%36D3C3DF024C.implementation preserve=yes
      //## end ClServerEvent%36D3C3DF024C.implementation

};

//## begin ClServerEvent%36D3C3DF024C.postscript preserve=yes
//## end ClServerEvent%36D3C3DF024C.postscript

// Class ClServerEvent 

//## begin module%36D3C65C003F.epilog preserve=yes
//## end module%36D3C65C003F.epilog


#endif
