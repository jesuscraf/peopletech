//## begin module%396B5C77035C.cm preserve=no
//## end module%396B5C77035C.cm

//## begin module%396B5C77035C.cp preserve=no
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
//## end module%396B5C77035C.cp

//## Module: clientagent%396B5C77035C; Package body
//## Subsystem: M4Dispatcher::src%3923BBFB0332
//## Source file: F:\integration\m4dispatcher\src\clientagent.cpp

//## begin module%396B5C77035C.additionalIncludes preserve=no
//## end module%396B5C77035C.additionalIncludes

//## begin module%396B5C77035C.includes preserve=yes
#include <executiveagent.hpp>
#include <dspactions.hpp>
#include <dspinvoker.hpp>
#include <actionidentifiers.hpp>
#include <m4xmlstreamgen.hpp>
#include <basiclog.hpp>
#include <m4dpchres.hpp>

#include <m4memdbg.h>
#include <m4srvtrace.h>

#define M4_SRV_TRACE_COMP_ID "DPCH"


//## end module%396B5C77035C.includes

// dspclientreqpdu
#include <dspclientreqpdu.hpp>
// clientagent
#include <clientagent.hpp>
// dspagent
#include <dspagent.hpp>
//## begin module%396B5C77035C.declarations preserve=no
//## end module%396B5C77035C.declarations

//## begin module%396B5C77035C.additionalDeclarations preserve=yes
//## end module%396B5C77035C.additionalDeclarations


// Class ClClientAgent 

ClClientAgent::ClClientAgent (ClSiteNodeRepository *ai_poServerRepository, ClDspExecutiveAgent *ai_poExecutiveAgent)
  //## begin ClClientAgent::ClClientAgent%963314069.hasinit preserve=no
  //## end ClClientAgent::ClClientAgent%963314069.hasinit
  //## begin ClClientAgent::ClClientAgent%963314069.initialization preserve=yes
  :ClDspAgent ( ai_poServerRepository , ai_poExecutiveAgent)
  //## end ClClientAgent::ClClientAgent%963314069.initialization
{
  //## begin ClClientAgent::ClClientAgent%963314069.body preserve=yes
  //## end ClClientAgent::ClClientAgent%963314069.body
}


ClClientAgent::~ClClientAgent ()
{
  //## begin ClClientAgent::~ClClientAgent%963314070.body preserve=yes
  //## end ClClientAgent::~ClClientAgent%963314070.body
}



//## Other Operations (implementation)
m4return_t ClClientAgent::ProcessAction (ClActionIdentifier ai_oActionIdentifier, ClParamList *ai_poParamList, ClBaseAction *&ao_poAction)
{
  //## begin ClClientAgent::ProcessAction%963314071.body preserve=yes
    ClBaseAction * poAction = NULL ;

	switch ( ai_oActionIdentifier )
	{

	case PROCESS_PDU_ID :
		M4_MDBG_NEW( poAction, poAction = new ClProcessPDUAction (this ) ) ;
			break ;
	default :

		M4_SRV_ERROR(M4_ERR_DPCH_UNKNOWNCLIENTAGENTACTION, 
			"Error proccessing action of the Agent for Client Management. Unknown Action Identifier %0:s.",
			ai_oActionIdentifier);			

		return M4_ERROR ;
	}


	poAction -> SetParams ( ai_poParamList ) ;

	m_poExecutiveAgent ->PushAction ( poAction ) ;


	return M4_SUCCESS ;
  //## end ClClientAgent::ProcessAction%963314071.body
}

m4return_t ClClientAgent::Start ()
{
  //## begin ClClientAgent::Start%963314072.body preserve=yes
	ClDispatcherInvoker * poInvoker = ClDispatcherInvoker::Instance () ;


	if ( M4_ERROR == poInvoker -> RegisterAction (  PROCESS_PDU ,PROCESS_PDU_ID , this ) )
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOINITIALIZEAGENTACTION, 
			"Error starting Agent for %0:s.\n Action %1:s not registered.",	
			"Client Management" << PROCESS_PDU);					
		return M4_ERROR ;
	}
	
	return M4_SUCCESS ;
  //## end ClClientAgent::Start%963314072.body
}

m4return_t ClClientAgent::RedirectClientConnection (ClDspClientRequestPDU *ai_poClientRequest)
{
  //## begin ClClientAgent::RedirectClientConnection%963314073.body preserve=yes
	ClSiteNodeInformation oSiteNode ;
	ClSiteNodeContainer oSiteContainer ;
	eDspClientTypes iClientType = CPP_CLIENT;

	
	ai_poClientRequest->GetClientType( iClientType );

	if ( M4_ERROR == m_poServerRepository ->GetCandidateNode ( oSiteNode, iClientType ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOCLIENTAGENTSERVERCANDIDATE,
			"Error: The Agent for Client Management cannot get a Candidate Server for Application Client." );
		return M4_ERROR ;
	}

	ClConnectReplyGen oReplyGen ;

	oSiteContainer.AddNode ( oSiteNode ) ;


	return ai_poClientRequest -> BuildReqReply ( oSiteContainer , &oReplyGen ) ;
  //## end ClClientAgent::RedirectClientConnection%963314073.body
}

// Additional Declarations
  //## begin ClClientAgent%396B588D038D.declarations preserve=yes
  //## end ClClientAgent%396B588D038D.declarations

//## begin module%396B5C77035C.epilog preserve=yes
//## end module%396B5C77035C.epilog
