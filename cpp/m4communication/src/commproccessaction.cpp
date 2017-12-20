//## begin module%3961A2850355.cm preserve=no
//## end module%3961A2850355.cm

//## begin module%3961A2850355.cp preserve=no
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
//## end module%3961A2850355.cp

//## Module: commproccessaction%3961A2850355; Package body
//## Subsystem: M4Communication::M4COML::src%38D1FCE80326
//## Source file: e:\mybuild\m4communication\src\commproccessaction.cpp

//## begin module%3961A2850355.additionalIncludes preserve=no
//## end module%3961A2850355.additionalIncludes

//## begin module%3961A2850355.includes preserve=yes
//## end module%3961A2850355.includes

// m4types
#include <m4types.hpp>
#include <basiclog.hpp>
// commproccessaction
#include <commproccessaction.hpp>
// CommAction
#include <commaction.hpp>
// CommComun
#include <commcomun.hpp>
//## begin module%3961A2850355.declarations preserve=no
//## end module%3961A2850355.declarations

//## begin module%3961A2850355.additionalDeclarations preserve=yes

// No funciona el BodyReference de Rose
#include <commtask.hpp>
#include <communicationlayer.hpp>
#include <clarraylibres.hpp>

#define M4_SRV_TRACE_COMP_ID "COMM"
#include <m4srvtrace.h>
#include <m4date.hpp>
//## end module%3961A2850355.additionalDeclarations


// Class ClCommProccessOpenPortAction 

ClCommProccessOpenPortAction::ClCommProccessOpenPortAction (m4tliid_t ai_Identifier, m4return_t ai_Success, ClCommTask *ai_pTask)
  //## begin ClCommProccessOpenPortAction::ClCommProccessOpenPortAction%962701624.hasinit preserve=no
  //## end ClCommProccessOpenPortAction::ClCommProccessOpenPortAction%962701624.hasinit
  //## begin ClCommProccessOpenPortAction::ClCommProccessOpenPortAction%962701624.initialization preserve=yes
:ClCommProccessBaseAction(ai_Identifier,ai_Success,ai_pTask)
  //## end ClCommProccessOpenPortAction::ClCommProccessOpenPortAction%962701624.initialization
{
  //## begin ClCommProccessOpenPortAction::ClCommProccessOpenPortAction%962701624.body preserve=yes
  //## end ClCommProccessOpenPortAction::ClCommProccessOpenPortAction%962701624.body
}



//## Other Operations (implementation)
m4return_t ClCommProccessOpenPortAction::Execute ()
{
  //## begin ClCommProccessOpenPortAction::Execute%962701632.body preserve=yes
	M4_SRV_DEBUG_X_N(1,"POP ");
	ClParamList *taskParams=	m_poTask->GetParams();

	taskParams->SetM4Return(M4_PARAM_RETURN_RESULT ,m_Success);
	taskParams->SetPointer(M4_PARAM_TLIID,m_TLIdentifier);
	m_poTask->SetTLIID(m_TLIdentifier);
	ClCommCondition *theCondition=m_poTask->GetCondition();
	if (NULL!=theCondition)
    {
        m_poTask->SetCondition(NULL);
	    m4sleep(1);
	    theCondition->Signal();
    }

	if(M4_SUCCESS!=m_Success)
	{
		m_poTask->RemoveParams();
		m_poCommImplementation->m_oBin.Release(m_poTask);
	}

	return M4_SUCCESS;
  //## end ClCommProccessOpenPortAction::Execute%962701632.body
}

// Additional Declarations
  //## begin ClCommProccessOpenPortAction%3961963001CF.declarations preserve=yes
  //## end ClCommProccessOpenPortAction%3961963001CF.declarations

// Class ClCommProccessClosePortAction 

ClCommProccessClosePortAction::ClCommProccessClosePortAction (m4tliid_t ai_Identifier, m4return_t ai_Success, ClCommTask *ai_pTask)
  //## begin ClCommProccessClosePortAction::ClCommProccessClosePortAction%962701625.hasinit preserve=no
  //## end ClCommProccessClosePortAction::ClCommProccessClosePortAction%962701625.hasinit
  //## begin ClCommProccessClosePortAction::ClCommProccessClosePortAction%962701625.initialization preserve=yes
  :ClCommProccessBaseAction(ai_Identifier,ai_Success,ai_pTask)
  //## end ClCommProccessClosePortAction::ClCommProccessClosePortAction%962701625.initialization
{
  //## begin ClCommProccessClosePortAction::ClCommProccessClosePortAction%962701625.body preserve=yes
  //## end ClCommProccessClosePortAction::ClCommProccessClosePortAction%962701625.body
}



//## Other Operations (implementation)
m4return_t ClCommProccessClosePortAction::Execute ()
{
  //## begin ClCommProccessClosePortAction::Execute%962701633.body preserve=yes
	M4_SRV_DEBUG_X_N(1,"PCP ");
	M4_ASSERT(NULL!=m_poTask);

	ClParamList *taskParams=	m_poTask->GetParams();

	taskParams->DeleteParam(M4_PARAM_RETURN_RESULT );
	taskParams->SetM4Return(M4_PARAM_RETURN_RESULT ,m_Success);
	ClCommCondition *theCondition=m_poTask->GetCondition();
	if (NULL!=theCondition)
    {
        m_poTask->SetCondition(NULL);
	    m4sleep(1);
	    theCondition->Signal();
    }

	return M4_SUCCESS;
  //## end ClCommProccessClosePortAction::Execute%962701633.body
}

// Additional Declarations
  //## begin ClCommProccessClosePortAction%3961981A01D3.declarations preserve=yes
  //## end ClCommProccessClosePortAction%3961981A01D3.declarations

// Class ClCommProccessBeginReadAction 

ClCommProccessBeginReadAction::ClCommProccessBeginReadAction (m4tliid_t ai_Identifier, m4return_t ai_Success, ClCommTask *ai_pTask)
  //## begin ClCommProccessBeginReadAction::ClCommProccessBeginReadAction%962701626.hasinit preserve=no
  //## end ClCommProccessBeginReadAction::ClCommProccessBeginReadAction%962701626.hasinit
  //## begin ClCommProccessBeginReadAction::ClCommProccessBeginReadAction%962701626.initialization preserve=yes
:ClCommProccessBaseAction(ai_Identifier,ai_Success,ai_pTask)
  //## end ClCommProccessBeginReadAction::ClCommProccessBeginReadAction%962701626.initialization
{
  //## begin ClCommProccessBeginReadAction::ClCommProccessBeginReadAction%962701626.body preserve=yes
  //## end ClCommProccessBeginReadAction::ClCommProccessBeginReadAction%962701626.body
}



//## Other Operations (implementation)
m4return_t ClCommProccessBeginReadAction::Execute ()
{
  //## begin ClCommProccessBeginReadAction::Execute%962701634.body preserve=yes
	M4_SRV_DEBUG_X_N(1,"PBR ");
	M4_ASSERT(NULL!=m_poTask);
	if(M4_SUCCESS != m_Success)
	{
		M4_SRV_ERROR_N(0,"Error en capa transporte en ProccessBeginRead");

		m4handle_t hConnectionID=m_poTask->GetConnectionID();

		ClParamList *theParams=m_poTask->GetParams();
		m4bool_t bDeleteParams=M4_FALSE;
		if(NULL==theParams)
		{
			theParams=new ClParamList;
			bDeleteParams=M4_TRUE;
		}
	
		theParams->SetInt(M4_PARAM_CONNECTIONID,hConnectionID);
		theParams->SetM4Return (M4_PARAM_RETURN_RESULT,M4_ERROR);
		Notify(theParams);

		if(bDeleteParams)
			delete theParams;
		else
			m_poTask->RemoveParams ();
		return M4_ERROR;
	}

	return M4_SUCCESS;
  //## end ClCommProccessBeginReadAction::Execute%962701634.body
}

// Additional Declarations
  //## begin ClCommProccessBeginReadAction%3961985B0014.declarations preserve=yes
  //## end ClCommProccessBeginReadAction%3961985B0014.declarations

// Class ClCommProccessStopReadingAction 

ClCommProccessStopReadingAction::ClCommProccessStopReadingAction (m4tliid_t ai_Identifier, m4return_t ai_Success, ClCommTask *ai_pTask)
  //## begin ClCommProccessStopReadingAction::ClCommProccessStopReadingAction%962701627.hasinit preserve=no
  //## end ClCommProccessStopReadingAction::ClCommProccessStopReadingAction%962701627.hasinit
  //## begin ClCommProccessStopReadingAction::ClCommProccessStopReadingAction%962701627.initialization preserve=yes
:ClCommProccessBaseAction(ai_Identifier,ai_Success,ai_pTask)
  //## end ClCommProccessStopReadingAction::ClCommProccessStopReadingAction%962701627.initialization
{
  //## begin ClCommProccessStopReadingAction::ClCommProccessStopReadingAction%962701627.body preserve=yes
  //## end ClCommProccessStopReadingAction::ClCommProccessStopReadingAction%962701627.body
}



//## Other Operations (implementation)
m4return_t ClCommProccessStopReadingAction::Execute ()
{
  //## begin ClCommProccessStopReadingAction::Execute%962701635.body preserve=yes
	M4_SRV_DEBUG_X_N(1,"PSR ");
	M4_ASSERT(NULL!=m_poTask);

	ClParamList *theParams=m_poTask->GetParams();
	m4handle_t hConnID=m_poTask->GetConnectionID();

	m4bool_t bBorraParam=M4_FALSE;
	if(NULL==theParams)
	{
		theParams=new ClParamList ;
		bBorraParam=M4_TRUE;
		theParams->SetInt(M4_PARAM_CONNECTIONID,hConnID);
		theParams->SetInt(M4_PARAM_SESSIONID,hConnID);
	}
	else
		theParams->DeleteParam(M4_PARAM_RETURN_RESULT);

	if(m_poTask->GetState()==DETECTED_CLOSED)
	{
		theParams->SetM4Return(M4_PARAM_RETURN_RESULT,M4_ERROR);
		Notify(theParams);
	}
	else
	{
		theParams->SetM4Return(M4_PARAM_RETURN_RESULT,M4_SUCCESS);
		m_poCommImplementation->AddReadingTask(hConnID,NULL);
		Notify(theParams);
		m_poCommImplementation->m_oBin.Release(m_poTask);
	}


	if(bBorraParam)
		delete theParams;
	return M4_SUCCESS;
  //## end ClCommProccessStopReadingAction::Execute%962701635.body
}

// Additional Declarations
  //## begin ClCommProccessStopReadingAction%39619AE30156.declarations preserve=yes
  //## end ClCommProccessStopReadingAction%39619AE30156.declarations

// Class ClCommProccessStopWritingAction 

ClCommProccessStopWritingAction::ClCommProccessStopWritingAction (m4tliid_t ai_Identifier, m4return_t ai_Success, ClCommTask *ai_pTask)
  //## begin ClCommProccessStopWritingAction::ClCommProccessStopWritingAction%962701628.hasinit preserve=no
  //## end ClCommProccessStopWritingAction::ClCommProccessStopWritingAction%962701628.hasinit
  //## begin ClCommProccessStopWritingAction::ClCommProccessStopWritingAction%962701628.initialization preserve=yes
:ClCommProccessBaseAction(ai_Identifier,ai_Success,ai_pTask)
  //## end ClCommProccessStopWritingAction::ClCommProccessStopWritingAction%962701628.initialization
{
  //## begin ClCommProccessStopWritingAction::ClCommProccessStopWritingAction%962701628.body preserve=yes
  //## end ClCommProccessStopWritingAction::ClCommProccessStopWritingAction%962701628.body
}



//## Other Operations (implementation)
m4return_t ClCommProccessStopWritingAction::Execute ()
{
  //## begin ClCommProccessStopWritingAction::Execute%962701636.body preserve=yes
	return M4_ERROR;
  //## end ClCommProccessStopWritingAction::Execute%962701636.body
}

// Additional Declarations
  //## begin ClCommProccessStopWritingAction%39619E1D0208.declarations preserve=yes
  //## end ClCommProccessStopWritingAction%39619E1D0208.declarations

// Class ClCommProccessStopConnectionAction 

ClCommProccessStopConnectionAction::ClCommProccessStopConnectionAction (m4tliid_t ai_Identifier, m4return_t ai_Success, ClCommTask *ai_pTask)
  //## begin ClCommProccessStopConnectionAction::ClCommProccessStopConnectionAction%962701629.hasinit preserve=no
  //## end ClCommProccessStopConnectionAction::ClCommProccessStopConnectionAction%962701629.hasinit
  //## begin ClCommProccessStopConnectionAction::ClCommProccessStopConnectionAction%962701629.initialization preserve=yes
:ClCommProccessBaseAction(ai_Identifier,ai_Success,ai_pTask)
  //## end ClCommProccessStopConnectionAction::ClCommProccessStopConnectionAction%962701629.initialization
{
  //## begin ClCommProccessStopConnectionAction::ClCommProccessStopConnectionAction%962701629.body preserve=yes
  //## end ClCommProccessStopConnectionAction::ClCommProccessStopConnectionAction%962701629.body
}



//## Other Operations (implementation)
m4return_t ClCommProccessStopConnectionAction::Execute ()
{
  //## begin ClCommProccessStopConnectionAction::Execute%962701637.body preserve=yes
	M4_SRV_DEBUG_X_N(1,"PSC ");
	M4_ASSERT(NULL!=m_poTask);
	
	m4handle_t hConnectionID=m_poTask->GetConnectionID();
	
	ClParamList *theParams=m_poTask->GetParams();
	m4bool_t bDeleteParams=M4_FALSE;
	if(NULL==theParams)
	{
		theParams=new ClParamList;
		bDeleteParams=M4_TRUE;
	}
	m_hConnectionID=m_poTask->GetConnectionID();
	theParams->SetInt(M4_PARAM_CONNECTIONID,m_hConnectionID);
	theParams->SetInt(M4_PARAM_SESSIONID,m_hConnectionID);

	if(m_poTask->GetCommProtocol())
		theParams->SetM4Return (M4_PARAM_RETURN_RESULT,M4_ERROR);
	else
		theParams->SetM4Return (M4_PARAM_RETURN_RESULT,M4_SUCCESS);
	m_poTask->RemoveParams ();
	ClCommunicationImplementation::GetInstance()->RemoveConnectionID(hConnectionID);
	Notify(theParams);

	if(bDeleteParams)
		delete theParams;
//	else
//		m_poTask->RemoveParams ();

	ClCommunicationImplementation::GetInstance()->m_oBin.Release(m_poTask);

	return M4_SUCCESS;
  //## end ClCommProccessStopConnectionAction::Execute%962701637.body
}

// Additional Declarations
  //## begin ClCommProccessStopConnectionAction%39619E6101DE.declarations preserve=yes
  //## end ClCommProccessStopConnectionAction%39619E6101DE.declarations

// Class ClCommProccessConnectAction 

ClCommProccessConnectAction::ClCommProccessConnectAction (m4tliid_t ai_Identifier, m4return_t ai_Success, ClCommTask *ai_pTask)
  //## begin ClCommProccessConnectAction::ClCommProccessConnectAction%962701630.hasinit preserve=no
  //## end ClCommProccessConnectAction::ClCommProccessConnectAction%962701630.hasinit
  //## begin ClCommProccessConnectAction::ClCommProccessConnectAction%962701630.initialization preserve=yes
:ClCommProccessBaseAction(ai_Identifier,ai_Success,ai_pTask)
  //## end ClCommProccessConnectAction::ClCommProccessConnectAction%962701630.initialization
{
  //## begin ClCommProccessConnectAction::ClCommProccessConnectAction%962701630.body preserve=yes
  //## end ClCommProccessConnectAction::ClCommProccessConnectAction%962701630.body
}



//## Other Operations (implementation)
m4return_t ClCommProccessConnectAction::Execute ()
{
  //## begin ClCommProccessConnectAction::Execute%962701638.body preserve=yes
	M4_SRV_DEBUG_X_N(1,"PC ");
	M4_ASSERT(NULL!=m_poTask);

	ClParamList *theParams=m_poTask->GetParams();
	theParams->SetM4Return (M4_PARAM_RETURN_RESULT,m_Success);
	if(M4_SUCCESS==m_Success)
	{
		m_poTask->SetState(CONNECTED);

		m4string_t strURL;
		m4pchar_t pcURL;
		if(M4_ERROR!=theParams->GetString(M4_PARAM_COMM_URL,pcURL))
			strURL=pcURL;
		ClCommunicationImplementation::GetInstance()->AddConnectionID(m_poTask->GetConnectionID(),m_TLIdentifier,((ClCommOpeningSocketTask*)m_poTask)->GetCommProtocolType(),strURL);
		theParams->SetPointer(M4_PARAM_TLIID,(void *)m_TLIdentifier);
	}
	else
	{
		m_poTask->SetState(NO_CONNECTED);
		int errorcode=ClCommunicationImplementation::GetInstance()->GetCTL().GetTSAPError(m_TLIdentifier);
		delete ( (ClTSAP *) m_TLIdentifier ) ;
		M4_SRV_ERROR(0,"Error procesando nuevo socket con error:%0:s", errorcode);
#ifdef _DEBUG
		m4pchar_t pcURL;
		if(M4_ERROR!=theParams->GetString(M4_PARAM_COMM_URL,pcURL))
			M4_SRV_DEBUG("NO CONNECTED:%0:s", pcURL);
#endif // _DEBUG
	}
	RemoveParams ();
	m_poTask->RemoveParams ();
	Notify(theParams);

	m_poCommImplementation->m_oBin.Release(m_poTask);

	return M4_SUCCESS;

  //## end ClCommProccessConnectAction::Execute%962701638.body
}

// Additional Declarations
  //## begin ClCommProccessConnectAction%39619F7F015D.declarations preserve=yes
  //## end ClCommProccessConnectAction%39619F7F015D.declarations

// Class ClCommProccessNewConnectionAction 

ClCommProccessNewConnectionAction::ClCommProccessNewConnectionAction (m4tliid_t ai_Identifier, m4return_t ai_Success, ClCommTask *ai_pTask)
  //## begin ClCommProccessNewConnectionAction::ClCommProccessNewConnectionAction%962701631.hasinit preserve=no
  //## end ClCommProccessNewConnectionAction::ClCommProccessNewConnectionAction%962701631.hasinit
  //## begin ClCommProccessNewConnectionAction::ClCommProccessNewConnectionAction%962701631.initialization preserve=yes
:ClCommProccessBaseAction(ai_Identifier,ai_Success,ai_pTask)
  //## end ClCommProccessNewConnectionAction::ClCommProccessNewConnectionAction%962701631.initialization
{
  //## begin ClCommProccessNewConnectionAction::ClCommProccessNewConnectionAction%962701631.body preserve=yes
  //## end ClCommProccessNewConnectionAction::ClCommProccessNewConnectionAction%962701631.body
}



//## Other Operations (implementation)
m4return_t ClCommProccessNewConnectionAction::Execute ()
{
  //## begin ClCommProccessNewConnectionAction::Execute%962701639.body preserve=yes
	M4_SRV_DEBUG_X_N(1,"PNC ");
	if(M4_ERROR==m_Success)
	{
		int errorcode=ClCommunicationImplementation::GetInstance()->GetCTL().GetTSAPError(m_TLIdentifier);
		M4_SRV_ERROR(0,"Error procesando nuevo socket con error:%0:s", errorcode);
	}

	int iProtocol=((ClCommOpenPortTask *)m_poTask)->GetCommProtocolType();
	m4handle_t hConnID=	(m4handle_t)ClIdGenerator::GetNewId();		

	ClCommunicationImplementation::GetInstance()->AddConnectionID(hConnID,m_TLIdentifier,iProtocol);

	m4pcchar_t pcActionName=m_poTask->GetActionName();

	ClParamList *ResultParams=new ClParamList ;
			
	ClExecutableInterface *theExecutor=m_poTask->GetExecutable();
	ResultParams->SetM4Return(M4_PARAM_RETURN_RESULT ,m_Success);
	ResultParams->SetInt(M4_PARAM_SESSIONID,hConnID);
	ResultParams->SetInt(M4_PARAM_CONNECTIONID,hConnID);
	ResultParams->SetInt(M4_PARAM_COMM_PROTOCOL_TYPE,iProtocol);
	ResultParams->SetPointer(M4_PARAM_TLIID,(void *)m_TLIdentifier);

	// eh 165963
	m4char_t	acRemoteHost[ 512 ] ;
	ClCommunicationImplementation::GetInstance()->GetCTL().GetTLIHostname( m_TLIdentifier, acRemoteHost, 512 ) ;
	ResultParams->SetString( M4_PARAM_REMOTE_HOST_NAME, acRemoteHost ) ;

	M4_ASSERT(NULL!=m_poExtExecutionCrono);
	m_poExtExecutionCrono->Start();
	
	if ( theExecutor )
		theExecutor->ExecuteAction((char *)pcActionName,ResultParams);
	m_poExtExecutionCrono->Stop();
	delete ResultParams;

	return M4_SUCCESS;
  //## end ClCommProccessNewConnectionAction::Execute%962701639.body
}

// Additional Declarations
  //## begin ClCommProccessNewConnectionAction%3961A04100C6.declarations preserve=yes
  //## end ClCommProccessNewConnectionAction%3961A04100C6.declarations

// Class ClCommProccessBaseAction 



ClCommProccessBaseAction::ClCommProccessBaseAction (m4tliid_t ai_Identifier, m4return_t ai_Success, ClCommTask *ai_pTask)
  //## begin ClCommProccessBaseAction::ClCommProccessBaseAction%965898339.hasinit preserve=no
  //## end ClCommProccessBaseAction::ClCommProccessBaseAction%965898339.hasinit
  //## begin ClCommProccessBaseAction::ClCommProccessBaseAction%965898339.initialization preserve=yes
  :m_TLIdentifier(ai_Identifier),m_Success(ai_Success)
  //## end ClCommProccessBaseAction::ClCommProccessBaseAction%965898339.initialization
{
  //## begin ClCommProccessBaseAction::ClCommProccessBaseAction%965898339.body preserve=yes
	m_poTask=ai_pTask;
  //## end ClCommProccessBaseAction::ClCommProccessBaseAction%965898339.body
}



//## Other Operations (implementation)
m4return_t ClCommProccessBaseAction::Execute ()
{
  //## begin ClCommProccessBaseAction::Execute%965898340.body preserve=yes
	return M4_ERROR;
  //## end ClCommProccessBaseAction::Execute%965898340.body
}

// Additional Declarations
  //## begin ClCommProccessBaseAction%3992C56E0149.declarations preserve=yes
  //## end ClCommProccessBaseAction%3992C56E0149.declarations

// Class ClCommProccessExternalCloseAction 

ClCommProccessExternalCloseAction::ClCommProccessExternalCloseAction (m4tliid_t ai_Identifier, m4return_t ai_Success, ClCommTask *ai_pTask)
  //## begin ClCommProccessExternalCloseAction::ClCommProccessExternalCloseAction%977310088.hasinit preserve=no
  //## end ClCommProccessExternalCloseAction::ClCommProccessExternalCloseAction%977310088.hasinit
  //## begin ClCommProccessExternalCloseAction::ClCommProccessExternalCloseAction%977310088.initialization preserve=yes
:ClCommProccessBaseAction(ai_Identifier,ai_Success,ai_pTask)
  //## end ClCommProccessExternalCloseAction::ClCommProccessExternalCloseAction%977310088.initialization
{
  //## begin ClCommProccessExternalCloseAction::ClCommProccessExternalCloseAction%977310088.body preserve=yes
  //## end ClCommProccessExternalCloseAction::ClCommProccessExternalCloseAction%977310088.body
}



//## Other Operations (implementation)
m4return_t ClCommProccessExternalCloseAction::Execute ()
{
  //## begin ClCommProccessExternalCloseAction::Execute%977310089.body preserve=yes
	ClCommReadingTask *theReadingTask;
	M4_ASSERT(NULL!=m_poTask);
	theReadingTask=(ClCommReadingTask *)m_poTask;
	switch(theReadingTask->GetState ())
	{
	case DETECTED_CLOSED_AND_REQUEST_PENDING:
	case REQUEST_PENDING:
		theReadingTask->SetState(DETECTED_CLOSED_AND_REQUEST_PENDING);
		{
			ClCommProccessExternalCloseAction *theAction= new ClCommProccessExternalCloseAction (theReadingTask->GetTLIID(),M4_ERROR,theReadingTask) ;
			if(theReadingTask->GetState()!=REQUEST_PENDING)
				theReadingTask->SetState(DETECTED_CLOSED);
			else
				theReadingTask->SetState(DETECTED_CLOSED_AND_REQUEST_PENDING);

			theAction->SetCondition(theReadingTask->GetCondition ());


			theAction->SetExecutable(theReadingTask->GetExecutable());
			theAction->SetActionName((char *)theReadingTask->GetActionName());
			ClCommunicationImplementation::GetInstance()->Push(theAction);
		}
		break;
	case CLOSING:
	m4Trace	(cerr <<"No hacemos nada pues ya estamos cerrando"<<endl);
		break;
	case DETECTED_CLOSED:
		{
			ClCommCloseConnectionAction *theAction=new ClCommCloseConnectionAction  ();
			theAction->PrepareTask(m_hConnectionID,theReadingTask->GetTLIID(),(ClCommProtocol *)M4_TRUE);
            
            theAction->SetCondition(theReadingTask->GetCondition ());
			theAction->SetExecutable(theReadingTask->GetExecutable());
			theAction->SetActionName((char *)theReadingTask->GetActionName());

			ClCommunicationImplementation::GetInstance()->Push(theAction);
			return M4_SUCCESS;
		}
		break;
	default:
//		theReadingTask->SetState(DETECTED_CLOSED);
		return M4_ERROR ;

	}



	return M4_SUCCESS;
  //## end ClCommProccessExternalCloseAction::Execute%977310089.body
}

// Additional Declarations
  //## begin ClCommProccessExternalCloseAction%3A4090A002FE.declarations preserve=yes
  //## end ClCommProccessExternalCloseAction%3A4090A002FE.declarations

//## begin module%3961A2850355.epilog preserve=yes
//## end module%3961A2850355.epilog
