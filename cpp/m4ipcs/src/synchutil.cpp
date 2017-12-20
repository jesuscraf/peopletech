//## begin module%37F20B420091.cm preserve=no
//## end module%37F20B420091.cm

//## begin module%37F20B420091.cp preserve=no
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
//## end module%37F20B420091.cp

//## Module: synchutil%37F20B420091; Subprogram body
//## Subsystem: M4Ipcs::src%3742D0220389
//	d:\fuentesservidor\m4ipcs\version\src
//## Source file: d:\eduardoma\m4ipcs\version\src\synchutil.cpp

//## begin module%37F20B420091.additionalIncludes preserve=no
//## end module%37F20B420091.additionalIncludes

//## begin module%37F20B420091.includes preserve=yes
//## end module%37F20B420091.includes

// syncronization
#include <m4syncronization.hpp>
// synchutil
#include <synchutil.hpp>
//## begin module%37F20B420091.declarations preserve=no
//## end module%37F20B420091.declarations

//## begin module%37F20B420091.additionalDeclarations preserve=yes
//## end module%37F20B420091.additionalDeclarations


// Class ClBlockWritersReaders 


ClBlockWritersReaders::ClBlockWritersReaders (ClWriterReaders *ai_poLock)
  //## begin ClBlockWritersReaders::ClBlockWritersReaders%938598215.hasinit preserve=no
      : m_poLocker(NULL)
  //## end ClBlockWritersReaders::ClBlockWritersReaders%938598215.hasinit
  //## begin ClBlockWritersReaders::ClBlockWritersReaders%938598215.initialization preserve=yes
  //## end ClBlockWritersReaders::ClBlockWritersReaders%938598215.initialization
{
  //## begin ClBlockWritersReaders::ClBlockWritersReaders%938598215.body preserve=yes
	m_poLocker = ai_poLock;
  //## end ClBlockWritersReaders::ClBlockWritersReaders%938598215.body
}


ClBlockWritersReaders::~ClBlockWritersReaders ()
{
  //## begin ClBlockWritersReaders::~ClBlockWritersReaders%938598216.body preserve=yes
  //## end ClBlockWritersReaders::~ClBlockWritersReaders%938598216.body
}


// Additional Declarations
  //## begin ClBlockWritersReaders%37F208A50003.declarations preserve=yes
  //## end ClBlockWritersReaders%37F208A50003.declarations

// Class ClBlockReaders 

ClBlockReaders::ClBlockReaders (ClWriterReaders *ai_poLock)
  //## begin ClBlockReaders::ClBlockReaders%938598219.hasinit preserve=no
  //## end ClBlockReaders::ClBlockReaders%938598219.hasinit
  //## begin ClBlockReaders::ClBlockReaders%938598219.initialization preserve=yes
  :	ClBlockWritersReaders(ai_poLock)
  //## end ClBlockReaders::ClBlockReaders%938598219.initialization
{
  //## begin ClBlockReaders::ClBlockReaders%938598219.body preserve=yes
	if (NULL != m_poLocker)
		m_poLocker->EnterReader();
  //## end ClBlockReaders::ClBlockReaders%938598219.body
}


ClBlockReaders::~ClBlockReaders ()
{
  //## begin ClBlockReaders::~ClBlockReaders%938598220.body preserve=yes
	if (NULL != m_poLocker)
		m_poLocker->LeaveReader();
  //## end ClBlockReaders::~ClBlockReaders%938598220.body
}


// Additional Declarations
  //## begin ClBlockReaders%37F20A4C0355.declarations preserve=yes
  //## end ClBlockReaders%37F20A4C0355.declarations

// Class ClBlockWriters 

ClBlockWriters::ClBlockWriters (ClWriterReaders *ai_poLock)
  //## begin ClBlockWriters::ClBlockWriters%938598217.hasinit preserve=no
  //## end ClBlockWriters::ClBlockWriters%938598217.hasinit
  //## begin ClBlockWriters::ClBlockWriters%938598217.initialization preserve=yes
  :	ClBlockWritersReaders(ai_poLock)
  //## end ClBlockWriters::ClBlockWriters%938598217.initialization
{
  //## begin ClBlockWriters::ClBlockWriters%938598217.body preserve=yes
	if (NULL != m_poLocker)
		m_poLocker->EnterWriter();
  //## end ClBlockWriters::ClBlockWriters%938598217.body
}


ClBlockWriters::~ClBlockWriters ()
{
  //## begin ClBlockWriters::~ClBlockWriters%938598218.body preserve=yes
	if (NULL != m_poLocker)
		m_poLocker->LeaveWriter();
  //## end ClBlockWriters::~ClBlockWriters%938598218.body
}


// Additional Declarations
  //## begin ClBlockWriters%37F20A34026A.declarations preserve=yes
  //## end ClBlockWriters%37F20A34026A.declarations

//## begin module%37F20B420091.epilog preserve=yes
//## end module%37F20B420091.epilog
