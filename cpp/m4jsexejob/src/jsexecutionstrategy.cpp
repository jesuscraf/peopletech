//## begin module%37F38E3903C3.cm preserve=no
//## end module%37F38E3903C3.cm

//## begin module%37F38E3903C3.cp preserve=no
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
//## end module%37F38E3903C3.cp

//## Module: jsexecutionstrategy%37F38E3903C3; Package body
//## Subsystem: M4JSExeJob::m4jsjob::src%37E244D003E3
//## Source file: C:\Trabajo\Server\m4jsexejob\version\src\jsexecutionstrategy.cpp

//## begin module%37F38E3903C3.additionalIncludes preserve=no
//## end module%37F38E3903C3.additionalIncludes

//## begin module%37F38E3903C3.includes preserve=yes
//## end module%37F38E3903C3.includes

// jsjob
#include <jsjob.hpp>
// jsexecutionstrategy
#include <jsexecutionstrategy.hpp>
//## begin module%37F38E3903C3.declarations preserve=no
//## end module%37F38E3903C3.declarations

//## begin module%37F38E3903C3.additionalDeclarations preserve=yes
//## end module%37F38E3903C3.additionalDeclarations


// Class ClJSExecutionStrategy 



//## Other Operations (implementation)
ClJSExecutionStrategy * ClJSExecutionStrategy::GetStrategyType (eStrategyType uiStrategyType)
{
  //## begin ClJSExecutionStrategy::GetStrategyType%938709377.body preserve=yes
	ClJSExecutionStrategy * pJobRes = NULL;  /* Resultado del método          */

	switch (uiStrategyType)
	{
		case LN4SimpleStrategy:
			pJobRes = ClJSLN4SimpleExecStrategy::Instance();
			break;
		case AdminSimpleStrategy:
			pJobRes = ClJSAdminSimpleExecStrategy::Instance();
			break;
		case InitialCompoundStrategy:
			pJobRes = ClJSInitialCompoundExecStrategy::Instance();
			break;
		case FinalCompoundStrategy:
			pJobRes = ClJSFinalCompoundExecStrategy::Instance();
			break;
		default:
			pJobRes = NULL;
			break;
	}

	return pJobRes;
  //## end ClJSExecutionStrategy::GetStrategyType%938709377.body
}

// Additional Declarations
  //## begin ClJSExecutionStrategy%37F3401802A1.declarations preserve=yes
  //## end ClJSExecutionStrategy%37F3401802A1.declarations

// Class ClJSLN4SimpleExecStrategy 

//## begin ClJSLN4SimpleExecStrategy::s_pInstance%37F38B8802F0.attr preserve=no  private: static ClJSLN4SimpleExecStrategy {RA} NULL
ClJSLN4SimpleExecStrategy *ClJSLN4SimpleExecStrategy::s_pInstance = NULL;
//## end ClJSLN4SimpleExecStrategy::s_pInstance%37F38B8802F0.attr

ClJSLN4SimpleExecStrategy::ClJSLN4SimpleExecStrategy ()
  //## begin ClJSLN4SimpleExecStrategy::ClJSLN4SimpleExecStrategy%938709385.hasinit preserve=no
  //## end ClJSLN4SimpleExecStrategy::ClJSLN4SimpleExecStrategy%938709385.hasinit
  //## begin ClJSLN4SimpleExecStrategy::ClJSLN4SimpleExecStrategy%938709385.initialization preserve=yes
  //## end ClJSLN4SimpleExecStrategy::ClJSLN4SimpleExecStrategy%938709385.initialization
{
  //## begin ClJSLN4SimpleExecStrategy::ClJSLN4SimpleExecStrategy%938709385.body preserve=yes
  //## end ClJSLN4SimpleExecStrategy::ClJSLN4SimpleExecStrategy%938709385.body
}



//## Other Operations (implementation)
ClJSLN4SimpleExecStrategy * ClJSLN4SimpleExecStrategy::Instance ()
{
  //## begin ClJSLN4SimpleExecStrategy::Instance%938709384.body preserve=yes
	if (s_pInstance == NULL)
		s_pInstance = new ClJSLN4SimpleExecStrategy;

	return s_pInstance;

  //## end ClJSLN4SimpleExecStrategy::Instance%938709384.body
}

m4return_t ClJSLN4SimpleExecStrategy::Execute (ClJSJob *ai_pJob)
{
  //## begin ClJSLN4SimpleExecStrategy::Execute%938766908.body preserve=yes
	ai_pJob->SetStatus( ClJSJob::StatusExecuted);

	return M4_SUCCESS;
  //## end ClJSLN4SimpleExecStrategy::Execute%938766908.body
}

void ClJSLN4SimpleExecStrategy::DestroyInstance ()
{
  //## begin ClJSLN4SimpleExecStrategy::DestroyInstance%938766918.body preserve=yes
	if (s_pInstance != NULL) 
	{
		delete s_pInstance;
		s_pInstance = NULL;
	}

  //## end ClJSLN4SimpleExecStrategy::DestroyInstance%938766918.body
}

// Additional Declarations
  //## begin ClJSLN4SimpleExecStrategy%37F340AA01C4.declarations preserve=yes
  //## end ClJSLN4SimpleExecStrategy%37F340AA01C4.declarations

// Class ClJSAdminSimpleExecStrategy 

//## begin ClJSAdminSimpleExecStrategy::s_pInstance%37F38F00017B.attr preserve=no  private: static ClJSAdminSimpleExecStrategy {RA} NULL
ClJSAdminSimpleExecStrategy *ClJSAdminSimpleExecStrategy::s_pInstance = NULL;
//## end ClJSAdminSimpleExecStrategy::s_pInstance%37F38F00017B.attr

ClJSAdminSimpleExecStrategy::ClJSAdminSimpleExecStrategy ()
  //## begin ClJSAdminSimpleExecStrategy::ClJSAdminSimpleExecStrategy%938709383.hasinit preserve=no
  //## end ClJSAdminSimpleExecStrategy::ClJSAdminSimpleExecStrategy%938709383.hasinit
  //## begin ClJSAdminSimpleExecStrategy::ClJSAdminSimpleExecStrategy%938709383.initialization preserve=yes
  //## end ClJSAdminSimpleExecStrategy::ClJSAdminSimpleExecStrategy%938709383.initialization
{
  //## begin ClJSAdminSimpleExecStrategy::ClJSAdminSimpleExecStrategy%938709383.body preserve=yes
  //## end ClJSAdminSimpleExecStrategy::ClJSAdminSimpleExecStrategy%938709383.body
}



//## Other Operations (implementation)
ClJSAdminSimpleExecStrategy * ClJSAdminSimpleExecStrategy::Instance ()
{
  //## begin ClJSAdminSimpleExecStrategy::Instance%938709382.body preserve=yes
	if (s_pInstance == NULL)
		s_pInstance = new ClJSAdminSimpleExecStrategy;

	return s_pInstance;

  //## end ClJSAdminSimpleExecStrategy::Instance%938709382.body
}

m4return_t ClJSAdminSimpleExecStrategy::Execute (ClJSJob *ai_pJob)
{
  //## begin ClJSAdminSimpleExecStrategy::Execute%938766909.body preserve=yes
	ai_pJob->SetStatus( ClJSJob::StatusExecuted);

	return M4_SUCCESS;
  //## end ClJSAdminSimpleExecStrategy::Execute%938766909.body
}

void ClJSAdminSimpleExecStrategy::DestroyInstance ()
{
  //## begin ClJSAdminSimpleExecStrategy::DestroyInstance%938766919.body preserve=yes
	if (s_pInstance != NULL) 
	{
		delete s_pInstance;
		s_pInstance = NULL;
	}
  //## end ClJSAdminSimpleExecStrategy::DestroyInstance%938766919.body
}

// Additional Declarations
  //## begin ClJSAdminSimpleExecStrategy%37F340C201D3.declarations preserve=yes
  //## end ClJSAdminSimpleExecStrategy%37F340C201D3.declarations

// Class ClJSInitialCompoundExecStrategy 

//## begin ClJSInitialCompoundExecStrategy::s_pInstance%37F38FD500EB.attr preserve=no  private: static ClJSInitialCompoundExecStrategy {RA} 
ClJSInitialCompoundExecStrategy *ClJSInitialCompoundExecStrategy::s_pInstance;
//## end ClJSInitialCompoundExecStrategy::s_pInstance%37F38FD500EB.attr

ClJSInitialCompoundExecStrategy::ClJSInitialCompoundExecStrategy ()
  //## begin ClJSInitialCompoundExecStrategy::ClJSInitialCompoundExecStrategy%938709381.hasinit preserve=no
  //## end ClJSInitialCompoundExecStrategy::ClJSInitialCompoundExecStrategy%938709381.hasinit
  //## begin ClJSInitialCompoundExecStrategy::ClJSInitialCompoundExecStrategy%938709381.initialization preserve=yes
  //## end ClJSInitialCompoundExecStrategy::ClJSInitialCompoundExecStrategy%938709381.initialization
{
  //## begin ClJSInitialCompoundExecStrategy::ClJSInitialCompoundExecStrategy%938709381.body preserve=yes
  //## end ClJSInitialCompoundExecStrategy::ClJSInitialCompoundExecStrategy%938709381.body
}



//## Other Operations (implementation)
ClJSInitialCompoundExecStrategy * ClJSInitialCompoundExecStrategy::Instance ()
{
  //## begin ClJSInitialCompoundExecStrategy::Instance%938709380.body preserve=yes
	if (s_pInstance == NULL)
		s_pInstance = new ClJSInitialCompoundExecStrategy;

	return s_pInstance;

  //## end ClJSInitialCompoundExecStrategy::Instance%938709380.body
}

m4return_t ClJSInitialCompoundExecStrategy::Execute (ClJSJob *ai_pJob)
{
  //## begin ClJSInitialCompoundExecStrategy::Execute%938766910.body preserve=yes
	ai_pJob->SetStatus( ClJSJob::StatusExecuting);

	return M4_SUCCESS;
  //## end ClJSInitialCompoundExecStrategy::Execute%938766910.body
}

void ClJSInitialCompoundExecStrategy::DestroyInstance ()
{
  //## begin ClJSInitialCompoundExecStrategy::DestroyInstance%938766920.body preserve=yes
	if (s_pInstance != NULL) 
	{
		delete s_pInstance;
		s_pInstance = NULL;
	}
  //## end ClJSInitialCompoundExecStrategy::DestroyInstance%938766920.body
}

// Additional Declarations
  //## begin ClJSInitialCompoundExecStrategy%37F340D203C1.declarations preserve=yes
  //## end ClJSInitialCompoundExecStrategy%37F340D203C1.declarations

// Class ClJSFinalCompoundExecStrategy 

//## begin ClJSFinalCompoundExecStrategy::s_pInstance%37F390E70211.attr preserve=no  private: static ClJSFinalCompoundExecStrategy {RA} NULL
ClJSFinalCompoundExecStrategy *ClJSFinalCompoundExecStrategy::s_pInstance = NULL;
//## end ClJSFinalCompoundExecStrategy::s_pInstance%37F390E70211.attr

ClJSFinalCompoundExecStrategy::ClJSFinalCompoundExecStrategy ()
  //## begin ClJSFinalCompoundExecStrategy::ClJSFinalCompoundExecStrategy%938709379.hasinit preserve=no
  //## end ClJSFinalCompoundExecStrategy::ClJSFinalCompoundExecStrategy%938709379.hasinit
  //## begin ClJSFinalCompoundExecStrategy::ClJSFinalCompoundExecStrategy%938709379.initialization preserve=yes
  //## end ClJSFinalCompoundExecStrategy::ClJSFinalCompoundExecStrategy%938709379.initialization
{
  //## begin ClJSFinalCompoundExecStrategy::ClJSFinalCompoundExecStrategy%938709379.body preserve=yes
  //## end ClJSFinalCompoundExecStrategy::ClJSFinalCompoundExecStrategy%938709379.body
}



//## Other Operations (implementation)
ClJSFinalCompoundExecStrategy * ClJSFinalCompoundExecStrategy::Instance ()
{
  //## begin ClJSFinalCompoundExecStrategy::Instance%938709378.body preserve=yes
	if (s_pInstance == NULL)
		s_pInstance = new ClJSFinalCompoundExecStrategy;

	return s_pInstance;
  //## end ClJSFinalCompoundExecStrategy::Instance%938709378.body
}

m4return_t ClJSFinalCompoundExecStrategy::Execute (ClJSJob *ai_pJob)
{
  //## begin ClJSFinalCompoundExecStrategy::Execute%938766911.body preserve=yes
	ai_pJob->SetStatus( ClJSJob::StatusExecuted);

	return M4_SUCCESS;
  //## end ClJSFinalCompoundExecStrategy::Execute%938766911.body
}

void ClJSFinalCompoundExecStrategy::DestroyInstance ()
{
  //## begin ClJSFinalCompoundExecStrategy::DestroyInstance%938766921.body preserve=yes
	if (s_pInstance != NULL) 
	{
		delete s_pInstance;
		s_pInstance = NULL;
	}
  //## end ClJSFinalCompoundExecStrategy::DestroyInstance%938766921.body
}

// Additional Declarations
  //## begin ClJSFinalCompoundExecStrategy%37F340FE0298.declarations preserve=yes
  //## end ClJSFinalCompoundExecStrategy%37F340FE0298.declarations

//## begin module%37F38E3903C3.epilog preserve=yes
//## end module%37F38E3903C3.epilog
