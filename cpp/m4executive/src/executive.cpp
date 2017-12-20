//## begin module%342013E201D0.cm preserve=no
//## end module%342013E201D0.cm

//## begin module%342013E201D0.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:			Executive
//	 File:				Executive.cpp
//	 Project:           M4ComSrv
//	 Author:			Meta Software M.S. , S.A
//	 Date:				971003
//	 Language:			C++
//	 Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	==============================================================================
//## end module%342013E201D0.cp

//## Module: Executive%342013E201D0; Package body
//## Subsystem: M4Executive::src%379462FF0256
//## Source file: F:\integration\m4executive\src\executive.cpp

//## begin module%342013E201D0.additionalIncludes preserve=no
//## end module%342013E201D0.additionalIncludes

//## begin module%342013E201D0.includes preserve=yes
#include <statistics.hpp>
#include <boolcondition.hpp>
//## end module%342013E201D0.includes

// Executive
#include <executive.hpp>
// ImsgContainer
#include <imsgcontainer.hpp>
// ClAdminTag
#include <cladmintag.hpp>
// ExVariables
#include <exvariables.hpp>
// DataStorages
#include <datastorages.hpp>
// m4types
#include <m4types.hpp>
// CriticalSection
#include <criticalsection.hpp>
// BlockSynchronization
#include <blocksynchronization.hpp>
// USInterface
#include <usinterface.hpp>

class ClConfiguration;
class ClNavigatorFirewall;
class ClChannel;

//## begin module%342013E201D0.declarations preserve=no
//## end module%342013E201D0.declarations

//## begin module%342013E201D0.additionalDeclarations preserve=yes
//## end module%342013E201D0.additionalDeclarations


// Class ClExecutive 





//## begin ClExecutive::m_bMutInit%379745AC0175.role preserve=no  protected: static m4bool_t { -> 1VHAN}
m4bool_t ClExecutive::m_bMutInit = M4_FALSE;
//## end ClExecutive::m_bMutInit%379745AC0175.role






//## begin ClExecutive::m_oMut%392AAD660360.role preserve=no  protected: static ClCriticalSection { -> 1VHAN}
ClCriticalSection ClExecutive::m_oMut;
//## end ClExecutive::m_oMut%392AAD660360.role


ClExecutive::ClExecutive (ClEngine *ai_pEngine, ClStatistic *ai_poStats)
  //## begin ClExecutive::ClExecutive%875605500.hasinit preserve=no
      : m_eOwnState(STATE_ACTIVE), m_pEngine(ai_pEngine), m_bInitialized(M4_FALSE), m_poSession(NULL), m_oMutAccess(M4_TRUE), m_pStatus(NULL), m_pCondIntrVal(NULL)
  //## end ClExecutive::ClExecutive%875605500.hasinit
  //## begin ClExecutive::ClExecutive%875605500.initialization preserve=yes
  //## end ClExecutive::ClExecutive%875605500.initialization
{
  //## begin ClExecutive::ClExecutive%875605500.body preserve=yes
	if (m_bMutInit == M4_FALSE)
	{
		m_bMutInit = M4_TRUE;
		m_oMut.Init();
	}

	m_eOwnState = STATE_ACTIVE ;
	m_pCondIntrVal = new ClBooleanCondition();
    m_pCondIntrVal->Init();
  //## end ClExecutive::ClExecutive%875605500.body
}


ClExecutive::~ClExecutive ()
{
  //## begin ClExecutive::~ClExecutive%875605501.body preserve=yes
    delete m_pCondIntrVal;
    m_pCondIntrVal = NULL;

	if (M4_TRUE == m_bInitialized)
	{
		if (NULL != m_pEngine)
		{
			m_pEngine->End();
			delete m_pEngine;

			m_pEngine = NULL;
		}
		m_poSession = NULL;
	}
  //## end ClExecutive::~ClExecutive%875605501.body
}



//## Other Operations (implementation)
m4return_t ClExecutive::DumpLevel (const m4int16_t ai_iLevel)
{
  //## begin ClExecutive::DumpLevel%921058932.body preserve=yes
	return M4_SUCCESS;
  //## end ClExecutive::DumpLevel%921058932.body
}

m4return_t ClExecutive::ReleaseResources ()
{
  //## begin ClExecutive::ReleaseResources%927196072.body preserve=yes
	return M4_SUCCESS;
  //## end ClExecutive::ReleaseResources%927196072.body
}

m4int16_t ClExecutive::GetStatus ()
{
  //## begin ClExecutive::GetStatus%982580499.body preserve=yes
	m4int16_t	iStatus;


	m_oMutAccess.Lock () ;

	if (! m_pStatus)
	{
		iStatus = ClStatus::STATUS_INTR_QUIT ;
	}
	else
	{

		iStatus = m_pStatus -> GetIntrValueForAdmin ();
	}

	m_oMutAccess.Unlock () ;


	return iStatus ;
  //## end ClExecutive::GetStatus%982580499.body
}

m4return_t ClExecutive::AddTraceMessage (m4string_t &ai_strMessage) const
{
  //## begin ClExecutive::AddTraceMessage%986914179.body preserve=yes
	ClMutBlock	oBlock(m_oMut);

	if ( m_poMsgContainer )
		m_poMsgContainer -> StoreTraceMessage ( ai_strMessage ) ;

	return M4_SUCCESS ;
  //## end ClExecutive::AddTraceMessage%986914179.body
}

void ClExecutive::SetMsgContainer (ClMessageContainer *ai_poMsgContainer)
{
  //## begin ClExecutive::SetMsgContainer%986914180.body preserve=yes
	ClMutBlock	oBlock(m_oMut);
	
	m_poMsgContainer = ai_poMsgContainer ;
  //## end ClExecutive::SetMsgContainer%986914180.body
}

ClExecutive::eOwnState ClExecutive::GetOwnState ()
{
  //## begin ClExecutive::GetOwnState%990541046.body preserve=yes
	return m_eOwnState ;
  //## end ClExecutive::GetOwnState%990541046.body
}

m4return_t ClExecutive::SetOwnState (ClExecutive::eOwnState ai_eState)
{
  //## begin ClExecutive::SetOwnState%990541047.body preserve=yes
	if ( STATE_HALTED == m_eOwnState )
		return M4_ERROR ;

	m_eOwnState = ai_eState ;

	return M4_SUCCESS ;
  //## end ClExecutive::SetOwnState%990541047.body
}

// Class ClEngineArgs 




ClEngineArgs::ClEngineArgs (M4DataStorage *ai_pInData, M4DataStorage *ai_pOutData)
  //## begin ClEngineArgs::ClEngineArgs%879963494.hasinit preserve=no
  //## end ClEngineArgs::ClEngineArgs%879963494.hasinit
  //## begin ClEngineArgs::ClEngineArgs%879963494.initialization preserve=yes
      : m_pOutputData(ai_pOutData), m_pInputData(ai_pInData)
  //## end ClEngineArgs::ClEngineArgs%879963494.initialization
{
  //## begin ClEngineArgs::ClEngineArgs%879963494.body preserve=yes
  //## end ClEngineArgs::ClEngineArgs%879963494.body
}



//## Other Operations (implementation)
m4return_t ClEngineArgs::GetInputData (M4DataStorage *&ao_pInData) const
{
  //## begin ClEngineArgs::GetInputData%884795082.body preserve=yes
	if (NULL == m_pInputData)
	{
		return M4_ERROR;
	}
	else
	{
		ao_pInData = m_pInputData;
		return M4_SUCCESS;
	}
  //## end ClEngineArgs::GetInputData%884795082.body
}

m4return_t ClEngineArgs::GetOutputData (M4DataStorage *&ao_pOutData)
{
  //## begin ClEngineArgs::GetOutputData%884854922.body preserve=yes
	if (NULL == m_pOutputData)
	{
		return M4_ERROR;
	}
	else
	{
		ao_pOutData = m_pOutputData;
		return M4_SUCCESS;
	}
  //## end ClEngineArgs::GetOutputData%884854922.body
}

// Additional Declarations
  //## begin ClEngineArgs%343273B3022B.declarations preserve=yes
  //## end ClEngineArgs%343273B3022B.declarations

// Class ClImage 






ClImage::ClImage (ClEngineArgs *  ai_pArgs, ClEngineExecEnv *ai_pEngineEnv, ClStatus *ai_pStatus, ClStatistic *ai_pStats)
  //## begin ClImage::ClImage%876139086.hasinit preserve=no
  //## end ClImage::ClImage%876139086.hasinit
  //## begin ClImage::ClImage%876139086.initialization preserve=yes
  :	m_pArgs(ai_pArgs), m_pEnvironment(ai_pEngineEnv), m_pStats(ai_pStats), m_pStatus(ai_pStatus)
  //## end ClImage::ClImage%876139086.initialization
{
  //## begin ClImage::ClImage%876139086.body preserve=yes
  //## end ClImage::ClImage%876139086.body
}



//## Other Operations (implementation)
void ClImage::GetArgs (ClEngineArgs *&ao_pArgs) const
{
  //## begin ClImage::GetArgs%884623038.body preserve=yes
	ao_pArgs = m_pArgs;
  //## end ClImage::GetArgs%884623038.body
}

void ClImage::SetArgs (ClEngineArgs *ai_pArgs)
{
  //## begin ClImage::SetArgs%943284077.body preserve=yes
	m_pArgs = ai_pArgs;
  //## end ClImage::SetArgs%943284077.body
}

void ClImage::GetStats (ClStatistic *&ao_pStats) const
{
  //## begin ClImage::GetStats%890222217.body preserve=yes
	ao_pStats = m_pStats;
  //## end ClImage::GetStats%890222217.body
}

void ClImage::SetStats (ClStatistic *ai_pStats)
{
  //## begin ClImage::SetStats%943284078.body preserve=yes
	m_pStats = ai_pStats;
  //## end ClImage::SetStats%943284078.body
}

void ClImage::GetEnv (ClEngineExecEnv *&ao_pEngineEnv) const
{
  //## begin ClImage::GetEnv%895580956.body preserve=yes
	ao_pEngineEnv = m_pEnvironment;
  //## end ClImage::GetEnv%895580956.body
}

void ClImage::SetEnv (ClEngineExecEnv *ai_pEngineEnv)
{
  //## begin ClImage::SetEnv%896353044.body preserve=yes
	m_pEnvironment = ai_pEngineEnv;
  //## end ClImage::SetEnv%896353044.body
}

void ClImage::GetStatus (ClStatus *&ao_pStatus) const
{
  //## begin ClImage::GetStatus%901734574.body preserve=yes
	ao_pStatus = m_pStatus;
  //## end ClImage::GetStatus%901734574.body
}

void ClImage::SetStatus (ClStatus *ai_pStatus)
{
  //## begin ClImage::SetStatus%901734575.body preserve=yes
	m_pStatus = ai_pStatus;
  //## end ClImage::SetStatus%901734575.body
}

void ClImage::Reset ()
{
  //## begin ClImage::Reset%943350231.body preserve=yes
  	// Bg 124696
  	if( m_pArgs != NULL )
	{
		delete m_pArgs;
		m_pArgs = NULL;
	}	
	m_pEnvironment = NULL;
	m_pStats = NULL;
	m_pStatus = NULL;
	m_sReqUniqueId = "";
  //## end ClImage::Reset%943350231.body
}

void ClImage::GetReqUniqueId (m4string_t &ao_sReqUniqueId) const
{
	ao_sReqUniqueId = m_sReqUniqueId;
}

void ClImage::SetReqUniqueId (m4string_t &ai_sReqUniqueId)
{
	m_sReqUniqueId = ai_sReqUniqueId;
}

// Additional Declarations
  //## begin ClImage%3430E9EE031B.declarations preserve=yes
  //## end ClImage%3430E9EE031B.declarations

// Class ClEngine 



ClEngine::~ClEngine ()
{
  //## begin ClEngine::~ClEngine%915791251.body preserve=yes
  //## end ClEngine::~ClEngine%915791251.body
}


// Class ClExecSerializableObject 

// Additional Declarations
  //## begin ClExecSerializableObject%3471524B032C.declarations preserve=yes
  //## end ClExecSerializableObject%3471524B032C.declarations

//## begin module%342013E201D0.epilog preserve=yes
//## end module%342013E201D0.epilog
