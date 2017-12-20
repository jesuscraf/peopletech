//## begin module%38F2109000DB.cm preserve=no
//## end module%38F2109000DB.cm

//## begin module%38F2109000DB.cp preserve=no
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
//## end module%38F2109000DB.cp

//## Module: clssinvoker%38F2109000DB; Package body
//## Subsystem: M4Subsystems::src%37D3A22003E4
//## Source file: F:\integration\m4subsystems\src\clssinvoker.cpp

//## begin module%38F2109000DB.additionalIncludes preserve=no
//## end module%38F2109000DB.additionalIncludes

//## begin module%38F2109000DB.includes preserve=yes
#include <clbaseaction.hpp>
#include <clssimplementacion.hpp>
#include <stdarg.h>
#include <clssaction.hpp>
//## end module%38F2109000DB.includes

// actionworker
#include <actionworker.hpp>
// actioninvoker
#include <actioninvoker.hpp>
// clssinvoker
#include <clssinvoker.hpp>
//## begin module%38F2109000DB.declarations preserve=no
//## end module%38F2109000DB.declarations

//## begin module%38F2109000DB.additionalDeclarations preserve=yes
//## end module%38F2109000DB.additionalDeclarations


// Class ClSSInvoker 

//## begin ClSSInvoker::m_poInstance%391923D401EC.role preserve=no  protected: static ClSSInvoker { -> RHAN}
ClSSInvoker *ClSSInvoker::m_poInstance;
//## end ClSSInvoker::m_poInstance%391923D401EC.role

//## begin ClSSInvoker::<m_pClSSInvoker>%391923D401C4.role preserve=no  protected: static ClSSInvoker { -> RHGA}
//## end ClSSInvoker::<m_pClSSInvoker>%391923D401C4.role


//## Other Operations (implementation)
ClSSInvoker * ClSSInvoker::GetInstance ()
{
  //## begin ClSSInvoker::GetInstance%957943667.body preserve=yes
	if ( ! m_poInstance ) 
	{
		m_poInstance = new ClSSInvoker () ;
	}

	return m_poInstance ;
  //## end ClSSInvoker::GetInstance%957943667.body
}

// Additional Declarations
  //## begin ClSSInvoker%391923800213.declarations preserve=yes
  //## end ClSSInvoker%391923800213.declarations

// Class ClSSActionWorker 


ClSSActionWorker::ClSSActionWorker (ClActionsQ &ai_poActionsQ, ClActionsQ *ai_poActionsRecycleBin)
  //## begin ClSSActionWorker::ClSSActionWorker%975501556.hasinit preserve=no
  //## end ClSSActionWorker::ClSSActionWorker%975501556.hasinit
  //## begin ClSSActionWorker::ClSSActionWorker%975501556.initialization preserve=yes
  :ClActionWorker ( ai_poActionsQ ) , m_poRecycleBin ( ai_poActionsRecycleBin )
  //## end ClSSActionWorker::ClSSActionWorker%975501556.initialization
{
  //## begin ClSSActionWorker::ClSSActionWorker%975501556.body preserve=yes
  //## end ClSSActionWorker::ClSSActionWorker%975501556.body
}



//## Other Operations (implementation)
void ClSSActionWorker::ExecuteAction (ClBaseAction &ai_oAction)
{
  //## begin ClSSActionWorker::ExecuteAction%975501557.body preserve=yes
	ai_oAction.Execute ( ) ;

	m_poRecycleBin -> Push ( &ai_oAction) ;
  //## end ClSSActionWorker::ExecuteAction%975501557.body
}

// Additional Declarations
  //## begin ClSSActionWorker%3A24F710003A.declarations preserve=yes
  //## end ClSSActionWorker%3A24F710003A.declarations

//## begin module%38F2109000DB.epilog preserve=yes
//## end module%38F2109000DB.epilog
