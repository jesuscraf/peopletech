//## begin module%36D3C65D01D1.cm preserve=no
//## end module%36D3C65D01D1.cm

//## begin module%36D3C65D01D1.cp preserve=no
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
//## end module%36D3C65D01D1.cp

//## Module: serverevent%36D3C65D01D1; Package body
//## Subsystem: M4ServerEvent::src%379434EF009A
//## Source file: F:\integration\m4serverevent\src\serverevent.cpp

//## begin module%36D3C65D01D1.additionalIncludes preserve=no
//## end module%36D3C65D01D1.additionalIncludes

//## begin module%36D3C65D01D1.includes preserve=yes
//## end module%36D3C65D01D1.includes

// syncro
#include <syncro.hpp>
// serverevent
#include <serverevent.hpp>
//## begin module%36D3C65D01D1.declarations preserve=no
//## end module%36D3C65D01D1.declarations

//## begin module%36D3C65D01D1.additionalDeclarations preserve=yes
//## end module%36D3C65D01D1.additionalDeclarations


// Class ClServerEvent 





ClServerEvent::ClServerEvent ()
  //## begin ClServerEvent::ClServerEvent%919850042.hasinit preserve=no
  //## end ClServerEvent::ClServerEvent%919850042.hasinit
  //## begin ClServerEvent::ClServerEvent%919850042.initialization preserve=yes
  //## end ClServerEvent::ClServerEvent%919850042.initialization
{
  //## begin ClServerEvent::ClServerEvent%919850042.body preserve=yes
	m_oMutexObserver.Init() ;
	m_oMutexObserved.Init() ;
  //## end ClServerEvent::ClServerEvent%919850042.body
}


ClServerEvent::~ClServerEvent ()
{
  //## begin ClServerEvent::~ClServerEvent%919850043.body preserve=yes
	OBSERVERS::iterator Iterator ;
	ClServerEvent* poServerEvent = NULL ;

	
	m_oMutexObserver.Lock () ;

	for (Iterator = m_oObservers.begin () ; Iterator != m_oObservers.end () ; Iterator ++ )
	{
		poServerEvent = (*Iterator) ;

		poServerEvent -> UnregisterObserved( this ) ;
	}

	m_oObservers.clear();
	m_oMutexObserver.Unlock () ;

	
	m_oMutexObserved.Lock () ;

	for (Iterator = m_oObservedSubjects.begin () ; Iterator != m_oObservedSubjects.end () ; Iterator ++ )
	{
		poServerEvent = (*Iterator) ;

		poServerEvent -> DetachObserver ( this ) ;
	}

	m_oObservedSubjects.clear();
	m_oMutexObserved.Unlock () ;

  //## end ClServerEvent::~ClServerEvent%919850043.body
}



//## Other Operations (implementation)
void ClServerEvent::AttachObserver (ClServerEvent *ai_poObserver)
{
  //## begin ClServerEvent::AttachObserver%919850044.body preserve=yes

	m_oMutexObserver.Lock () ;
	
	m_oObservers.remove ( ai_poObserver ) ;
	m_oObservers.push_back ( ai_poObserver ) ;

	m_oMutexObserver.Unlock () ;

	ai_poObserver -> RegisterObserved ( this ) ;
  //## end ClServerEvent::AttachObserver%919850044.body
}

void ClServerEvent::DetachObserver (ClServerEvent *ai_poObserver)
{
  //## begin ClServerEvent::DetachObserver%919850045.body preserve=yes
	m_oMutexObserver.Lock () ;
	m_oObservers.remove ( ai_poObserver ) ;
	m_oMutexObserver.Unlock () ;
  //## end ClServerEvent::DetachObserver%919850045.body
}

void ClServerEvent::Notify (EVENTTYPE ai_iEvent)
{
  //## begin ClServerEvent::Notify%919850046.body preserve=yes
	OBSERVERS::iterator Iterator ;
	ClServerEvent* poServerEvent = NULL ;

	m_oMutexObserver.Lock () ;

	for (Iterator = m_oObservers.begin () ; Iterator != m_oObservers.end () ; Iterator ++ )
	{
		poServerEvent = (*Iterator) ;

		poServerEvent -> Update (ai_iEvent ,  this ) ;
	}

	m_oMutexObserver.Unlock () ;

  //## end ClServerEvent::Notify%919850046.body
}

void ClServerEvent::Update (EVENTTYPE ai_iEvent, ClServerEvent *ai_oSubject)
{
  //## begin ClServerEvent::Update%919850047.body preserve=yes
  //## end ClServerEvent::Update%919850047.body
}

void ClServerEvent::RegisterObserved (ClServerEvent *ai_poObservedSubject)
{
  //## begin ClServerEvent::RegisterObserved%945425942.body preserve=yes
	m_oMutexObserved.Lock () ;
	m_oObservedSubjects.remove (  ai_poObservedSubject ) ;
	m_oObservedSubjects.push_back ( ai_poObservedSubject ) ;
	m_oMutexObserved.Unlock () ;

  //## end ClServerEvent::RegisterObserved%945425942.body
}

void ClServerEvent::UnregisterObserved (ClServerEvent *ai_poObservedSubject)
{
  //## begin ClServerEvent::UnregisterObserved%945425943.body preserve=yes
	m_oMutexObserved.Lock () ;
	m_oObservedSubjects.remove (  ai_poObservedSubject ) ;
	m_oMutexObserved.Unlock() ;
  //## end ClServerEvent::UnregisterObserved%945425943.body
}

// Additional Declarations
  //## begin ClServerEvent%36D3C3DF024C.declarations preserve=yes
  //## end ClServerEvent%36D3C3DF024C.declarations

//## begin module%36D3C65D01D1.epilog preserve=yes
//## end module%36D3C65D01D1.epilog
