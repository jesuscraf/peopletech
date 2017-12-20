//## begin module%3961A474031B.cm preserve=no
//## end module%3961A474031B.cm

//## begin module%3961A474031B.cp preserve=no
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
//## end module%3961A474031B.cp

//## Module: commeventhandler%3961A474031B; Package body
//## Subsystem: M4Communication::M4COML::src%38D1FCE80326
//## Source file: e:\mybuild\m4communication\src\commeventhandler.cpp

//## begin module%3961A474031B.additionalIncludes preserve=no
//## end module%3961A474031B.additionalIncludes

//## begin module%3961A474031B.includes preserve=yes
//## end module%3961A474031B.includes

// commeventhandler
#include <commeventhandler.hpp>
// CommComun
#include <commcomun.hpp>
#include <basiclog.hpp>

class ClCommTask;
//class ClCommReadingTask;

//## begin module%3961A474031B.declarations preserve=no
//## end module%3961A474031B.declarations

//## begin module%3961A474031B.additionalDeclarations preserve=yes
#include <commtask.hpp>
#include <communicationlayer.hpp>
#include <commproccessaction.hpp>
#include <iexecutable.hpp>

#define M4_SRV_TRACE_COMP_ID "COMM"
#include <m4srvtrace.h>
//## end module%3961A474031B.additionalDeclarations


// Class ClCommEventHandler 




//## Other Operations (implementation)
m4return_t ClCommEventHandler::ProccessRead (m4tliid_t ai_Identifier, int ai_Len)
{
  //## begin ClCommEventHandler::ProccessRead%955107286.body preserve=yes
	m4return_t iRet = M4_ERROR;

	switch (ai_Len)
	{
	case -1:
		m4TraceLevel(3,cerr <<"Read ha devuelto -1 Error:"<<ClCommunicationImplementation::GetInstance()->GetCTL().GetTSAPError(ai_Identifier)<<endl);
		((ClCommReadingTask *)m_poTask)->CloseSocket(M4_TRUE);
		return M4_ERROR;	// Para que lo quite de su select	
	case 0:
		((ClCommReadingTask *)m_poTask)->CloseSocket(M4_TRUE);
			return M4_ERROR;	// Para que lo quite de su select	
		break;
	default:
		m4TraceLevel(3,cout<<m_poTask->GetConnectionID()<<" Leido "<<ai_Len<<endl);
		iRet = ((ClCommReadingTask *)m_poTask)->Readed(ai_Len);

		// Bg 125975.
		// En caso de recibir dos pdus (ACK + ERROR tras cancel),
		// llamamos de nuevo para procesar la segunda.
		if(iRet == M4_WARNING)
		{
			((ClCommReadingTask *)m_poTask)->SetState(NONE);
			iRet = ((ClCommReadingTask *)m_poTask)->Readed(0);
		}

		return iRet;
	}
  //## end ClCommEventHandler::ProccessRead%955107286.body
}

m4return_t ClCommEventHandler::ProccessWrite (m4tliid_t ai_Identifier, m4return_t ai_Return)
{
  //## begin ClCommEventHandler::ProccessWrite%955107287.body preserve=yes
	if(M4_SUCCESS!=ai_Return)
	{
		M4_SRV_ERROR(0,"Hubo un error al escribir con errorcode:%0:s", ClCommunicationImplementation::GetInstance()->GetCTL().GetTSAPError(ai_Identifier));
	}
	m4return_t iRet=((ClCommWrittingTask *)m_poTask)->Written(ai_Return);
	return M4_SUCCESS;
  //## end ClCommEventHandler::ProccessWrite%955107287.body
}

m4return_t ClCommEventHandler::ProccessOpenPort (m4tliid_t ai_Identifier, m4return_t ai_Success)
{
  //## begin ClCommEventHandler::ProccessOpenPort%955107288.body preserve=yes
	ClCommProccessOpenPortAction *pAction;

	if ((pAction = new ClCommProccessOpenPortAction(ai_Identifier, ai_Success, m_poTask)) == NULL)
		return M4_ERROR;

	ClCommunicationImplementation::GetInstance()->Push(pAction);

	return M4_SUCCESS;
  //## end ClCommEventHandler::ProccessOpenPort%955107288.body
}

m4return_t ClCommEventHandler::ProccessClosePort (m4tliid_t ai_Identifier, m4return_t ai_Success)
{
  //## begin ClCommEventHandler::ProccessClosePort%955107289.body preserve=yes
	ClCommProccessClosePortAction *pAction;

	if ((pAction = new ClCommProccessClosePortAction(ai_Identifier, ai_Success, m_poTask)) == NULL)
		return M4_ERROR;

	ClCommunicationImplementation::GetInstance()->Push(pAction);

	return M4_SUCCESS;
  //## end ClCommEventHandler::ProccessClosePort%955107289.body
}

m4return_t ClCommEventHandler::ProccessBeginRead (m4tliid_t ai_Identifier, m4return_t ai_Success)
{
  //## begin ClCommEventHandler::ProccessBeginRead%955107290.body preserve=yes
	
	ClCommProccessBeginReadAction *pAction;
	if(M4_ERROR==ai_Success)
	{
		m4TraceLevel(2,cerr <<"BeginRead ERROR"<<endl);
		if ((pAction = new ClCommProccessBeginReadAction(ai_Identifier, ai_Success, m_poTask)) == NULL)
			return M4_ERROR;
		ClCommunicationImplementation::GetInstance()->Push(pAction);
	}
	else
	{m4TraceLevel(2,cerr <<"BeginRead OK"<<endl);}
	return M4_SUCCESS;
  //## end ClCommEventHandler::ProccessBeginRead%955107290.body
}

m4return_t ClCommEventHandler::ProccessStopReading (m4tliid_t ai_Identifier, m4return_t ai_Success)
{
  //## begin ClCommEventHandler::ProccessStopReading%955107291.body preserve=yes
	ClCommProccessStopReadingAction *pAction;

	if ((pAction = 	new ClCommProccessStopReadingAction(ai_Identifier, ai_Success, m_poTask)) == NULL)
		return M4_ERROR;

	ClCommunicationImplementation::GetInstance()->Push(pAction);

	return M4_SUCCESS;
  //## end ClCommEventHandler::ProccessStopReading%955107291.body
}

m4return_t ClCommEventHandler::ProccessStopWriting (m4tliid_t ai_Identifier, m4return_t ai_Success)
{
  //## begin ClCommEventHandler::ProccessStopWriting%955107292.body preserve=yes
	ClCommProccessStopWritingAction *pAction;

	if ((pAction = 	new ClCommProccessStopWritingAction(ai_Identifier, ai_Success, m_poTask)) == NULL)
		return M4_ERROR;

	ClCommunicationImplementation::GetInstance()->Push(pAction);

	return M4_SUCCESS;
  //## end ClCommEventHandler::ProccessStopWriting%955107292.body
}

m4return_t ClCommEventHandler::ProccessConnect (m4tliid_t ai_Identifier, m4return_t ai_Success)
{
  //## begin ClCommEventHandler::ProccessConnect%955107293.body preserve=yes
	ClCommProccessConnectAction *pAction; 

	if ((pAction = 	new ClCommProccessConnectAction(ai_Identifier, ai_Success, m_poTask)) == NULL)
		return M4_ERROR;

	ClCommunicationImplementation::GetInstance()->Push(pAction);

	return M4_SUCCESS;
  //## end ClCommEventHandler::ProccessConnect%955107293.body
}

m4return_t ClCommEventHandler::ProccessStopConnection (m4tliid_t ai_Identifier, m4return_t ai_Success)
{
  //## begin ClCommEventHandler::ProccessStopConnection%955107294.body preserve=yes
	ClCommProccessStopConnectionAction *pAction;

	if ((pAction = new ClCommProccessStopConnectionAction(ai_Identifier, ai_Success, m_poTask)) == NULL)
		return M4_ERROR;

	/* ADD OSCARG 26-9-02
	Hay que boorar el TSAP del mapa de la capa de comunicaciones porque a continuación 
	este thread va a borrar el TSAP*/
//	ClCommunicationImplementation::GetInstance()->RemoveConnectionID( m_poTask->GetConnectionID() );
	//END ADD OSCARG 26-9-02

	ClCommunicationImplementation::GetInstance()->Push(pAction);

	return M4_SUCCESS;
  //## end ClCommEventHandler::ProccessStopConnection%955107294.body
}

m4return_t ClCommEventHandler::ProccessNewConnection (m4tliid_t ai_Identifier, m4return_t ai_Success)
{
  //## begin ClCommEventHandler::ProccessNewConnection%955613881.body preserve=yes
	ClCommProccessNewConnectionAction *pAction;

	if ((pAction = new ClCommProccessNewConnectionAction(ai_Identifier, ai_Success, m_poTask)) == NULL)
		return M4_ERROR;

	ClCommunicationImplementation::GetInstance()->Push(pAction);

	return M4_SUCCESS;
  //## end ClCommEventHandler::ProccessNewConnection%955613881.body
}

m4return_t ClCommEventHandler::UpdateTask ()
{
  //## begin ClCommEventHandler::UpdateTask%955613888.body preserve=yes
	return M4_ERROR;
  //## end ClCommEventHandler::UpdateTask%955613888.body
}

m4return_t ClCommEventHandler::ProccessStop (m4return_t ai_Success)
{
  //## begin ClCommEventHandler::ProccessStop%957360601.body preserve=yes
	
	ClParamList *theParams=m_poTask->GetParams();
	if(theParams)	// Syncronous way
	{
		ClExecutableInterface *theExecutor=m_poTask->GetExecutable();
		theParams->DeleteParam(M4_PARAM_RETURN_RESULT);
		theParams->SetM4Return(M4_PARAM_RETURN_RESULT, ai_Success);

		if(NULL==theExecutor)
		{
			ClCommCondition *theCondition=m_poTask->GetCondition();
            m_poTask->SetCondition(NULL);
			m_poTask->RemoveParams();
			theCondition->Signal();
		}
		else
		{
			m4pcchar_t pcActionName=m_poTask->GetActionName();
			theExecutor->ExecuteAction((char *)pcActionName,theParams);
		}
		ClCommunicationImplementation::GetInstance()->m_oBin.Release(m_poTask);
		return M4_SUCCESS;
	}
	else
	{
/*		ClCommunicationImplementation *poImp=ClCommunicationImplementation::GetInstance();
		m4return_t iRet=poImp->StopCommunicationLayer();
		delete poImp;
		return iRet;*/
		ClCommShutDownAction *theAction=new ClCommShutDownAction ;
		theAction->m_bFirstShutdownStep=M4_FALSE;
		ClCommunicationImplementation::GetInstance()->Push(theAction);
		return M4_SUCCESS;
	}
  //## end ClCommEventHandler::ProccessStop%957360601.body
}

m4return_t ClCommEventHandler::ProccessGetTransferedBytes (m4tliid_t ai_Identifier, size_t ai_Readed, size_t ai_Writed, m4return_t ai_Return)
{
  //## begin ClCommEventHandler::ProccessGetTransferedBytes%958059393.body preserve=yes
	return M4_ERROR;
  //## end ClCommEventHandler::ProccessGetTransferedBytes%958059393.body
}

// Additional Declarations
  //## begin ClCommEventHandler%38EDC69303DF.declarations preserve=yes
  //## end ClCommEventHandler%38EDC69303DF.declarations

//## begin module%3961A474031B.epilog preserve=yes
//## end module%3961A474031B.epilog
