//## begin module%3912C5850294.cm preserve=no
//## end module%3912C5850294.cm

//## begin module%3912C5850294.cp preserve=no
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
//## end module%3912C5850294.cp

//## Module: clsubjct%3912C5850294; Package body
//## Subsystem: m4sysall::src%3912C5DD0060
//## Source file: d:\new\m4sysall\src\clsubjct.cpp

//## begin module%3912C5850294.additionalIncludes preserve=no
//## end module%3912C5850294.additionalIncludes

//## begin module%3912C5850294.includes preserve=yes
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// File:	clsubjct.cpp
// Author:	Stuart Murdoch (mailto:stuartm@meta4.es)
// Date:	8/06/1997
// Definition:
//    This module defines the subject base class, part of the
//    observer pattern. An object which needs to inform other
//    objects of a change in state derive from this base, and call
//    Notify() when they change state.  Observers, objects wish wish to
//    be informed of state change add themselves (subscribe) using the attach
// Reference:
//    Design Patterns pg.293
//

//#include "clsubjct.hpp"
//#include "clobserv.hpp"

//## end module%3912C5850294.includes

// clsubjct
#include <clsubjct.hpp>
//## begin module%3912C5850294.declarations preserve=no
//## end module%3912C5850294.declarations

//## begin module%3912C5850294.additionalDeclarations preserve=yes
//## end module%3912C5850294.additionalDeclarations


// Class ClSubject 


ClSubject::ClSubject ()
  //## begin ClSubject::ClSubject%554791384.hasinit preserve=no
  //## end ClSubject::ClSubject%554791384.hasinit
  //## begin ClSubject::ClSubject%554791384.initialization preserve=yes
  //## end ClSubject::ClSubject%554791384.initialization
{
  //## begin ClSubject::ClSubject%554791384.body preserve=yes
	// Do nothing special
  //## end ClSubject::ClSubject%554791384.body
}


ClSubject::~ClSubject (void )
{
  //## begin ClSubject::~ClSubject%-2095366624.body preserve=yes
	m_oObserverList.clear();
  //## end ClSubject::~ClSubject%-2095366624.body
}



//## Other Operations (implementation)
void ClSubject::Attach (ClObserver* ai_poClObserver)
{
  //## begin ClSubject::Attach%-1449382659.body preserve=yes
	m_oObserverList.push_back(ai_poClObserver);
  //## end ClSubject::Attach%-1449382659.body
}

void ClSubject::Detach (ClObserver* ai_poClObserver)
{
  //## begin ClSubject::Detach%-1250960091.body preserve=yes
	m_oObserverList.remove(ai_poClObserver);
  //## end ClSubject::Detach%-1250960091.body
}

void ClSubject::Notify (void )
{
  //## begin ClSubject::Notify%-928315364.body preserve=yes
	OBSERVERLIST::iterator oObserverListIt;

	for (oObserverListIt = m_oObserverList.begin();
		oObserverListIt != m_oObserverList.end();
		++oObserverListIt)
	{
		(*oObserverListIt)->Update(this);
	}
  //## end ClSubject::Notify%-928315364.body
}

// Additional Declarations
  //## begin ClSubject%3912C5830291.declarations preserve=yes
  //## end ClSubject%3912C5830291.declarations

//## begin module%3912C5850294.epilog preserve=yes
//## end module%3912C5850294.epilog
