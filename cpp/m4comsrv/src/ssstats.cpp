//## begin module%34EDD1C20263.cm preserve=no
//## end module%34EDD1C20263.cm

//## begin module%34EDD1C20263.cp preserve=no
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
//## end module%34EDD1C20263.cp

//## Module: ssstats%34EDD1C20263; Package body
//## Subsystem: version::src%379ECBD7029A
//## Source file: F:\integration\m4comsrv\src\ssstats.cpp

//## begin module%34EDD1C20263.additionalIncludes preserve=no
//## end module%34EDD1C20263.additionalIncludes

//## begin module%34EDD1C20263.includes preserve=yes
#include <request.hpp>
//## end module%34EDD1C20263.includes

// syncro
#include <syncro.hpp>
// SessManager
#include <sessmanager.hpp>
// ssstats
#include <ssstats.hpp>

class ClActiveClassStats;

//## begin module%34EDD1C20263.declarations preserve=no
//## end module%34EDD1C20263.declarations

//## begin module%34EDD1C20263.additionalDeclarations preserve=yes
//## end module%34EDD1C20263.additionalDeclarations


// Class ClActiveListStats 



ClActiveListStats::ClActiveListStats ()
  //## begin ClActiveListStats::ClActiveListStats%888317705.hasinit preserve=no
      : m_oMutex(M4_TRUE)
  //## end ClActiveListStats::ClActiveListStats%888317705.hasinit
  //## begin ClActiveListStats::ClActiveListStats%888317705.initialization preserve=yes
  //## end ClActiveListStats::ClActiveListStats%888317705.initialization
{
  //## begin ClActiveListStats::ClActiveListStats%888317705.body preserve=yes
	m_oMutex.Lock () ;
	m_oStatsList.clear ( ) ;
	m_oMutex.Unlock () ;
  //## end ClActiveListStats::ClActiveListStats%888317705.body
}


ClActiveListStats::~ClActiveListStats ()
{
  //## begin ClActiveListStats::~ClActiveListStats%888317706.body preserve=yes
  //## end ClActiveListStats::~ClActiveListStats%888317706.body
}



//## Other Operations (implementation)
void ClActiveListStats::AddActiveClassStat (ClActiveClassStats *ai_poStat)
{
  //## begin ClActiveListStats::AddActiveClassStat%888225156.body preserve=yes
	m_oMutex.Lock () ;
	
	m_oStatsList.push_back ( ai_poStat ) ;

	m_oMutex.Unlock () ;
  //## end ClActiveListStats::AddActiveClassStat%888225156.body
}

// Additional Declarations
  //## begin ClActiveListStats%34EDC7E90204.declarations preserve=yes
  //## end ClActiveListStats%34EDC7E90204.declarations

// Class ClSessManagerListStats 

// Additional Declarations
  //## begin ClSessManagerListStats%34EDCCB6034C.declarations preserve=yes
  //## end ClSessManagerListStats%34EDCCB6034C.declarations

//## begin module%34EDD1C20263.epilog preserve=yes
//## end module%34EDD1C20263.epilog


// Detached code regions:
// WARNING: this code will be lost if code is regenerated.
#if 0
//## begin ClDistributorListStats::GetQueueItems%886068759.body preserve=no
	return m_iQueueItems ;
//## end ClDistributorListStats::GetQueueItems%886068759.body

#endif
