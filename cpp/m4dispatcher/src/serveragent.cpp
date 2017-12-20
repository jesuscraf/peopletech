//## begin module%396B5C9300E5.cm preserve=no
//## end module%396B5C9300E5.cm

//## begin module%396B5C9300E5.cp preserve=no
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
//## end module%396B5C9300E5.cp

//## Module: serveragent%396B5C9300E5; Package body
//## Subsystem: M4Dispatcher::src%3923BBFB0332
//## Source file: F:\integration\m4dispatcher\src\serveragent.cpp

//## begin module%396B5C9300E5.additionalIncludes preserve=no
//## end module%396B5C9300E5.additionalIncludes

//## begin module%396B5C9300E5.includes preserve=yes
#include <executiveagent.hpp>
#include <dspactions.hpp>
#include <actionidentifiers.hpp>
#include <dspinvoker.hpp>
#include <basiclog.hpp>
#include <m4dpchres.hpp>
#include <dspparams.hpp>
#include <m4xmlstreamgen.hpp>

#include <m4memdbg.h>
#include <m4srvtrace.h>

#define M4_SRV_TRACE_COMP_ID "DPCH"

//## end module%396B5C9300E5.includes

// serveragent
#include <serveragent.hpp>
// dspagent
#include <dspagent.hpp>
//## begin module%396B5C9300E5.declarations preserve=no
//## end module%396B5C9300E5.declarations

//## begin module%396B5C9300E5.additionalDeclarations preserve=yes
//## end module%396B5C9300E5.additionalDeclarations


// Class ClServerAgent 

ClServerAgent::ClServerAgent (ClSiteNodeRepository *ai_poServerRepository, ClDspExecutiveAgent *ai_poExecutiveAgent)
  //## begin ClServerAgent::ClServerAgent%963314064.hasinit preserve=no
  //## end ClServerAgent::ClServerAgent%963314064.hasinit
  //## begin ClServerAgent::ClServerAgent%963314064.initialization preserve=yes
  :ClDspAgent ( ai_poServerRepository , ai_poExecutiveAgent )
  //## end ClServerAgent::ClServerAgent%963314064.initialization
{
  //## begin ClServerAgent::ClServerAgent%963314064.body preserve=yes
  //## end ClServerAgent::ClServerAgent%963314064.body
}


ClServerAgent::~ClServerAgent ()
{
  //## begin ClServerAgent::~ClServerAgent%963314065.body preserve=yes
  //## end ClServerAgent::~ClServerAgent%963314065.body
}



//## Other Operations (implementation)
m4return_t ClServerAgent::ProcessAction (ClActionIdentifier ai_oActionIdentifier, ClParamList *ai_poParamList, ClBaseAction *&ao_poAction)
{
  //## begin ClServerAgent::ProcessAction%963314066.body preserve=yes
	ClBaseAction * poAction ;
	m4uint32_t iConnectionID = 0;
	

	switch ( ai_oActionIdentifier )
	{
	case UPDATE_CONFIGURATION_ACTION_ID :
		M4_MDBG_NEW( (ClBaseAction * )poAction, poAction = new ClUpdateConfigAction ( this ) ) ;
			break ;
	case CHECK_SERVER_CONNECTION_ACTION_ID :		
		return( CheckServerConnection(ai_poParamList) );
		break;
	}

	poAction -> SetParams ( ai_poParamList ) ;

	m_poExecutiveAgent ->PushAction ( poAction ) ;


	return M4_SUCCESS ;
  //## end ClServerAgent::ProcessAction%963314066.body
}

m4return_t ClServerAgent::Start ()
{
  //## begin ClServerAgent::Start%963314067.body preserve=yes
	ClDispatcherInvoker * poInvoker = ClDispatcherInvoker::Instance () ;

	
	if ( M4_ERROR == poInvoker -> RegisterAction ( UPDATE_CONFIGURATION , UPDATE_CONFIGURATION_ACTION_ID, this ) )
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOINITIALIZEAGENTACTION, 
			"Error starting Agent for %0:s. Action %1:s not registered.",
			"Server Management" << UPDATE_CONFIGURATION);					
		return M4_ERROR ;
	}

	if ( M4_ERROR == poInvoker -> RegisterAction ( CHECK_SERVER_CONNECTION , CHECK_SERVER_CONNECTION_ACTION_ID, this ) )
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOINITIALIZEAGENTACTION, 
			"Error starting Agent for %0:s. Action %1:s not registered.",
			"Server Management" << CHECK_SERVER_CONNECTION);					
		return M4_ERROR ;
	}

	return M4_SUCCESS ;
  //## end ClServerAgent::Start%963314067.body
}

m4return_t ClServerAgent::UpdateConfiguration (ClSiteNodeInformation &ai_oSiteNode)
{
 //## begin ClServerAgent::UpdateConfiguration%963314068.body preserve=yes
//	ClSiteNodeInformation * poNodeInfo = NULL ;
	m4return_t iRet ;
//  M4_MDBG_NEW( poNodeInfo, poNodeInfo = new ClSiteNodeInformation (ai_szConfigId, ai_szHost , 0 , ai_szState , ai_dLoad);

	if ( M4_ERROR == ( iRet = m_poServerRepository  -> UpdateNode ( ai_oSiteNode) ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOSERVERAGENTNODEINFO,
			"Error updating Server Configuration from Server Agent. Cannot update Server Information." );

		return M4_ERROR ;
	}

	return M4_SUCCESS ;
  //## end ClServerAgent::UpdateConfiguration%963314068.body
}

m4return_t ClServerAgent::CheckServerConnection (ClParamList *ai_poParamList)
{
  //## begin ClServerAgent::CheckServerConnection%969372803.body preserve=yes

	ClSiteNodeContainer oSiteNodeList;
	ClSiteNodeInformation oSiteNodeInfo;
	m4int_t iConnectionID = 0;
	m4string_t strState;


	if ( M4_SUCCESS != (ai_poParamList -> GetInt ( "SESSIONID" , iConnectionID ) ) )
		return M4_ERROR;

	//
	// Search the Connection ID to know if the broken connection belongs to
	// a Server node. If the broken connection does not belong to a Server node
	// it must belongs to the Admin. Client
	//

	if (m_poServerRepository->Count() > 0 ) {

		if (M4_ERROR == m_poServerRepository->GetNodeList(oSiteNodeList))
			return M4_ERROR;

		if (M4_TRUE == oSiteNodeList.FindNodeConnectionID(iConnectionID, oSiteNodeInfo))
		{

			// Change the site node state 

			strState = oSiteNodeInfo.GetNodeState();

			if (  ! stricmp(strState.c_str(), STATE_SHUTING_DOWN ) )
			{

//Vamos a hacer que cunado hace shutdown en arranque reinicie
//				oSiteNodeInfo.SetNodeState(STATE_INACTIVE );
				if ( oSiteNodeInfo.m_iStopReason != STOP_ADMIN )
				{
					oSiteNodeInfo.SetNodeState(STATE_STARTING);
					if ( M4_ERROR == m_poServerRepository  -> UpdateNode ( oSiteNodeInfo) )
					{
						return M4_ERROR ;
					}
				}
			}
			else if ( 0 != stricmp(strState.c_str(), STATE_INACTIVE ) ) 
			{
				oSiteNodeInfo.SetNodeState(STATE_HALTED);
				if ( M4_ERROR == m_poServerRepository  -> UpdateNode ( oSiteNodeInfo) )
				{
					return M4_ERROR ;
				}
			}
			else if ((oSiteNodeInfo.m_iStopReason == STOP_MEMORY) 
						|| 	(oSiteNodeInfo.m_iStopReason == STOP_LDB)
						|| 	(oSiteNodeInfo.m_iStopReason == STOP_GENERAL))
			{
				oSiteNodeInfo.SetNodeState(STATE_HALTED);
				if ( M4_ERROR == m_poServerRepository  -> UpdateNode ( oSiteNodeInfo) )
				{
					return M4_ERROR ;
				}
			}
		}
	}
	return M4_SUCCESS;


  //## end ClServerAgent::CheckServerConnection%969372803.body
}

// Additional Declarations
  //## begin ClServerAgent%396B58810137.declarations preserve=yes
  //## end ClServerAgent%396B58810137.declarations

//## begin module%396B5C9300E5.epilog preserve=yes
//## end module%396B5C9300E5.epilog


