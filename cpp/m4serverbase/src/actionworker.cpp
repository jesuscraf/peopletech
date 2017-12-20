//## begin module%39490E670011.cm preserve=no
//## end module%39490E670011.cm

//## begin module%39490E670011.cp preserve=no
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
//## end module%39490E670011.cp

//## Module: actionworker%39490E670011; Package body
//## Subsystem: M4ServerBase::src%379431BE02D0
//## Source file: F:\integration\m4serverbase\src\actionworker.cpp

//## begin module%39490E670011.additionalIncludes preserve=no
//## end module%39490E670011.additionalIncludes

//## begin module%39490E670011.includes preserve=yes
//## end module%39490E670011.includes

// m4thread
#include <m4thread.hpp>
// actionworker
#include <actionworker.hpp>
// FastQ
#include <fastfifoq.hpp>
//## begin module%39490E670011.declarations preserve=no
//## end module%39490E670011.declarations

//## begin module%39490E670011.additionalDeclarations preserve=yes
//## end module%39490E670011.additionalDeclarations


// Class ClActionWorker 


ClActionWorker::ClActionWorker (ClActionsQ &ai_poActionsQ)
  //## begin ClActionWorker::ClActionWorker%961054365.hasinit preserve=no
  //## end ClActionWorker::ClActionWorker%961054365.hasinit
  //## begin ClActionWorker::ClActionWorker%961054365.initialization preserve=yes
  //## end ClActionWorker::ClActionWorker%961054365.initialization
{
  //## begin ClActionWorker::ClActionWorker%961054365.body preserve=yes
	m_poActionsQueue = &ai_poActionsQ ;
  //## end ClActionWorker::ClActionWorker%961054365.body
}


ClActionWorker::~ClActionWorker ()
{
  //## begin ClActionWorker::~ClActionWorker%961054366.body preserve=yes
  //## end ClActionWorker::~ClActionWorker%961054366.body
}



//## Other Operations (implementation)
void ClActionWorker::Run (void )
{
  //## begin ClActionWorker::Run%961054367.body preserve=yes
	ClBaseAction * poAction ;

	while ( ! IsTerminate () )
	{
		if ( M4_SUCCESS == m_poActionsQueue -> Pop ( poAction ) )
		{
			if ( poAction )
			{
				ExecuteAction ( *poAction ) ;
			}
		}
	}
  //## end ClActionWorker::Run%961054367.body
}

void ClActionWorker::ExecuteAction (ClBaseAction &ai_oAction)
{
  //## begin ClActionWorker::ExecuteAction%961054368.body preserve=yes
	ai_oAction.Execute () ;
  //## end ClActionWorker::ExecuteAction%961054368.body
}

// Additional Declarations
  //## begin ClActionWorker%39490D3902C9.declarations preserve=yes
  //## end ClActionWorker%39490D3902C9.declarations

//## begin module%39490E670011.epilog preserve=yes
//## end module%39490E670011.epilog
