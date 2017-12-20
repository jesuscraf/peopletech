//## begin module%39479D8D003E.cm preserve=no
//## end module%39479D8D003E.cm

//## begin module%39479D8D003E.cp preserve=no
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
//## end module%39479D8D003E.cp

//## Module: dspinvoker%39479D8D003E; Package body
//## Subsystem: M4Dispatcher::src%3923BBFB0332
//## Source file: C:\WorkingFolder\m4dispatcher\src\dspinvoker.cpp

//## begin module%39479D8D003E.additionalIncludes preserve=no
//## end module%39479D8D003E.additionalIncludes

//## begin module%39479D8D003E.includes preserve=yes
//## end module%39479D8D003E.includes

// dspinvoker
#include <dspinvoker.hpp>
// actioninvoker
#include <actioninvoker.hpp>
//## begin module%39479D8D003E.declarations preserve=no
//## end module%39479D8D003E.declarations

//## begin module%39479D8D003E.additionalDeclarations preserve=yes

#include <m4memdbg.h>
#define M4_SRV_TRACE_COMP_ID "DPCH"

//## end module%39479D8D003E.additionalDeclarations


// Class ClDispatcherInvoker 

//## begin ClDispatcherInvoker::m_poInstance%39479A970016.role preserve=no  private: static ClDispatcherInvoker { -> RHAN}
ClDispatcherInvoker *ClDispatcherInvoker::m_poInstance = NULL;
//## end ClDispatcherInvoker::m_poInstance%39479A970016.role

//## begin ClDispatcherInvoker::<m_pClDispatcherInvoker>%39479A970002.role preserve=no  private: static ClDispatcherInvoker { -> RHGA}
//## end ClDispatcherInvoker::<m_pClDispatcherInvoker>%39479A970002.role

ClDispatcherInvoker::ClDispatcherInvoker ()
  //## begin ClDispatcherInvoker::ClDispatcherInvoker%960967034.hasinit preserve=no
  //## end ClDispatcherInvoker::ClDispatcherInvoker%960967034.hasinit
  //## begin ClDispatcherInvoker::ClDispatcherInvoker%960967034.initialization preserve=yes
  //## end ClDispatcherInvoker::ClDispatcherInvoker%960967034.initialization
{
  //## begin ClDispatcherInvoker::ClDispatcherInvoker%960967034.body preserve=yes
  //## end ClDispatcherInvoker::ClDispatcherInvoker%960967034.body
}


ClDispatcherInvoker::~ClDispatcherInvoker ()
{
  //## begin ClDispatcherInvoker::~ClDispatcherInvoker%960967035.body preserve=yes
  //## end ClDispatcherInvoker::~ClDispatcherInvoker%960967035.body
}



//## Other Operations (implementation)
ClDispatcherInvoker * ClDispatcherInvoker::Instance ()
{
  //## begin ClDispatcherInvoker::Instance%960967036.body preserve=yes
	if ( ! m_poInstance )
	{
		M4_MDBG_NEW( m_poInstance, m_poInstance = new ClDispatcherInvoker ) ;
	}

	return m_poInstance ;
  //## end ClDispatcherInvoker::Instance%960967036.body
}

ClDispatcherInvoker * ClDispatcherInvoker::FreeInstance ()
{
  //## begin ClDispatcherInvoker::FreeInstance%972579513.body preserve=yes

	M4_MDBG_DELETE( m_poInstance, delete m_poInstance ) ;
	m_poInstance=NULL ;

	return m_poInstance ;

 //## end ClDispatcherInvoker::FreeInstance%972579513.body
}


// Additional Declarations
  //## begin ClDispatcherInvoker%39479A8900CA.declarations preserve=yes
  //## end ClDispatcherInvoker%39479A8900CA.declarations

//## begin module%39479D8D003E.epilog preserve=yes
//## end module%39479D8D003E.epilog
