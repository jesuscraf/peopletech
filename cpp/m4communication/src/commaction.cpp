//## begin module%38DB631E018E.cm preserve=no
//## end module%38DB631E018E.cm

//## begin module%38DB631E018E.cp preserve=no
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
//## end module%38DB631E018E.cp

//## Module: CommAction%38DB631E018E; Package body
//## Subsystem: M4Communication::M4COML::src%38D1FCE80326
//## Source file: e:\mybuild\m4communication\src\commaction.cpp

//## begin module%38DB631E018E.additionalIncludes preserve=no
#include <m4trace.hpp>
//## end module%38DB631E018E.additionalIncludes

//## begin module%38DB631E018E.includes preserve=yes
#include <commactionsname.h>
#include <communicationlayer.hpp>
#include <m4communication.hpp>
#include <m4date.hpp>

#ifndef _UNIX
#include <m4lang.hpp>
#endif

void viewState(eCommTaskState theState)
{
	switch (theState)
	{
	case NONE:
		m4Trace(cerr<<"NONE");
		break;
	case ACCEPTING:
		m4Trace(cerr<< "ACCEPTING");
		break;
	case ACCEPTED:
		m4Trace(cerr<< "ACCEPTED");
		break;
	case ERROR_ACCEPTING:
		m4Trace(cerr<< "ERROR_ACCEPTING");
		break;
	case WAITING_FOR_READING:
		m4Trace(cerr<< "WAITING_FOR_READING");
		break;
	case READING:
		m4Trace(cerr<< "READING");
		break;
	case PAUSE_READING:
		m4Trace(cerr<< "PAUSE_READING");
		break;
	case START_WRITTING:
		m4Trace(cerr<< "START_WRITTING");
		break;
	case WRITTING:
		m4Trace(cerr<< "WRITTING");
		break;
	case CLOSING:
		m4Trace(cerr<< "CLOSING");
		break;
	case OPENING:
		m4Trace(cerr<< "OPENING");
		break;
	case ERROR_OPENING:
		m4Trace(cerr<< "ERROR_OPENING");
		break;
	case CONNECTING:
		m4Trace(cerr<< "CONNECTING");
		break;
	case CONNECTED:
		m4Trace(cerr<< "CONNECTED");
		break;
	case NO_CONNECTED:
		m4Trace(cerr<< "NO_CONNECTED");
		break;
	case FINISHING_WRITTING:
		m4Trace(cerr<< "FINISHING_WRITTING");
		break;
	case FINISH_ALL:
		m4Trace(cerr<< "FINISH_ALL");
		break;
	case DETECTED_CLOSED:
		m4Trace(cerr<< "DETECTED_CLOSED");
		break;
	case REQUEST_PENDING:
		m4Trace(cerr<< "REQUEST_PENDING");
		break;
	case DETECTED_CLOSED_AND_REQUEST_PENDING:
		m4Trace(cerr<< "DETECTED_CLOSED_AND_REQUEST_PENDING");
		break;
	default:
		m4Trace(cerr<< "unkown state:"<<theState);
		break;
	}

}


//## end module%38DB631E018E.includes

// FastQ
#include <fastfifoq.hpp>
// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
// CommAction
#include <commaction.hpp>
// clbaseaction
#include <clbaseaction.hpp>
// CommComun
#include <commcomun.hpp>
// m4crono
#include <m4crono.hpp>

class ClCommTask;
class ClCommProtocol;
class ClExecutableInterface;
class ClCommCondition;

//## begin module%38DB631E018E.declarations preserve=no
//## end module%38DB631E018E.declarations

//## begin module%38DB631E018E.additionalDeclarations preserve=yes
#define M4_SRV_TRACE_COMP_ID "COMM"
#include <m4srvtrace.h>
//## end module%38DB631E018E.additionalDeclarations


// Class ClCommAction 








ClCommAction::ClCommAction ()
  //## begin ClCommAction::ClCommAction%955103952.hasinit preserve=no
      : m_pExecutableInterface(NULL), m_poCondition(NULL), m_poTask(NULL), m_poExtExecutionCrono(NULL)
  //## end ClCommAction::ClCommAction%955103952.hasinit
  //## begin ClCommAction::ClCommAction%955103952.initialization preserve=yes
  ,ClBaseAction(NULL)
  //## end ClCommAction::ClCommAction%955103952.initialization
{
  //## begin ClCommAction::ClCommAction%955103952.body preserve=yes
	m_poCommImplementation=ClCommunicationImplementation::GetInstance();
  //## end ClCommAction::ClCommAction%955103952.body
}


ClCommAction::~ClCommAction ()
{
  //## begin ClCommAction::~ClCommAction%955103953.body preserve=yes
  //## end ClCommAction::~ClCommAction%955103953.body
}



//## Other Operations (implementation)
m4return_t ClCommAction::Execute ()
{
  //## begin ClCommAction::Execute%954227121.body preserve=yes
	M4_ASSERT(0);
	return M4_ERROR;
  //## end ClCommAction::Execute%954227121.body
}

m4return_t ClCommAction::SetCondition (ClCommCondition *ai_poCondition)
{
  //## begin ClCommAction::SetCondition%965219972.body preserve=yes
	m_poCondition=ai_poCondition;
	return M4_SUCCESS;
  //## end ClCommAction::SetCondition%965219972.body
}

ClCommCondition * ClCommAction::GetCondition ()
{
  //## begin ClCommAction::GetCondition%965219973.body preserve=yes
	return m_poCondition;
  //## end ClCommAction::GetCondition%965219973.body
}

m4return_t ClCommAction::SetActionName (m4pchar_t ai_szActionName)
{
  //## begin ClCommAction::SetActionName%965219974.body preserve=yes
	if(NULL==ai_szActionName)
		m_strActionName="";
	else
		m_strActionName=ai_szActionName;
	return M4_SUCCESS;
  //## end ClCommAction::SetActionName%965219974.body
}

m4pcchar_t ClCommAction::GetActionName ()
{
  //## begin ClCommAction::GetActionName%965219975.body preserve=yes
	return m_strActionName.c_str();
  //## end ClCommAction::GetActionName%965219975.body
}

m4return_t ClCommAction::SetExecutable (ClExecutableInterface *ai_pExecutable)
{
  //## begin ClCommAction::SetExecutable%965219976.body preserve=yes
	m_pExecutableInterface=ai_pExecutable;
	return M4_SUCCESS;
  //## end ClCommAction::SetExecutable%965219976.body
}

ClExecutableInterface * ClCommAction::GetExecutable ()
{
  //## begin ClCommAction::GetExecutable%965219977.body preserve=yes
	return m_pExecutableInterface;
  //## end ClCommAction::GetExecutable%965219977.body
}

m4return_t ClCommAction::_PassReturnInfo ()
{
  //## begin ClCommAction::_PassReturnInfo%965219986.body preserve=yes
	m_poTask->SetCondition(GetCondition());
	m_poTask->SetActionName((char *)GetActionName());
	m_poTask->SetExecutable(GetExecutable());
	return M4_SUCCESS;
  //## end ClCommAction::_PassReturnInfo%965219986.body
}

m4return_t ClCommAction::PrepareTask (m4handle_t ai_hConnectionID, m4tliid_t  ai_TLIID, ClCommProtocol *  ai_pProtocol)
{
  //## begin ClCommAction::PrepareTask%965898335.body preserve=yes
	m_hConnectionID=ai_hConnectionID;
	return M4_WARNING;
  //## end ClCommAction::PrepareTask%965898335.body
}

m4return_t ClCommAction::Notify (ClParamList *ai_poParams)
{
  //## begin ClCommAction::Notify%965977805.body preserve=yes
	ClExecutableInterface *theExecutor;
	m4return_t iRet;
	ClCommCondition *theCondition;
	m4pchar_t pcActionName=NULL;

	if(NULL!=m_poTask)
	{
		theCondition=m_poTask->GetCondition();
		m_poTask->SetCondition(NULL);
		theExecutor=m_poTask->GetExecutable();	
		if(NULL==theExecutor)
		{
			ai_poParams->SetInt("DeleteParam",1);
			m_poTask->RemoveParams();
		}
		pcActionName=(char *)	m_poTask->GetActionName();
	}
	else
	{
		theCondition=GetCondition();
		theExecutor=GetExecutable();	
		if(NULL==theExecutor)
			ai_poParams->SetInt("DeleteParam",1);
		
		pcActionName=(char *)GetActionName();
	}

	iRet=M4_SUCCESS;
	if(NULL==theExecutor)
	{
		RemoveParams();
		if(NULL!=theCondition)
			theCondition->Signal();
		else
		{m4TraceLevel(2,cerr <<"Nobody waiting for this action"<<endl);}
	}
	else
	{
		M4_ASSERT(NULL!=m_poExtExecutionCrono);
		m_poExtExecutionCrono->Start();
		iRet=theExecutor->ExecuteAction(pcActionName,ai_poParams);
		m_poExtExecutionCrono->Stop();
	}

	return iRet;
  //## end ClCommAction::Notify%965977805.body
}

m4return_t ClCommAction::SetExtExecutionCrono (ClCommExternalClock *ai_pExtExecutionCrono)
{
  //## begin ClCommAction::SetExtExecutionCrono%967474876.body preserve=yes
	m_poExtExecutionCrono=ai_pExtExecutionCrono;
	return M4_SUCCESS;
  //## end ClCommAction::SetExtExecutionCrono%967474876.body
}

// Additional Declarations
  //## begin ClCommAction%38D9E14E0268.declarations preserve=yes
  //## end ClCommAction%38D9E14E0268.declarations

// Class ClCommStartReadingAction 

ClCommStartReadingAction::ClCommStartReadingAction ()
  //## begin ClCommStartReadingAction::ClCommStartReadingAction%955103954.hasinit preserve=no
  //## end ClCommStartReadingAction::ClCommStartReadingAction%955103954.hasinit
  //## begin ClCommStartReadingAction::ClCommStartReadingAction%955103954.initialization preserve=yes
  //## end ClCommStartReadingAction::ClCommStartReadingAction%955103954.initialization
{
  //## begin ClCommStartReadingAction::ClCommStartReadingAction%955103954.body preserve=yes
  //## end ClCommStartReadingAction::ClCommStartReadingAction%955103954.body
}


ClCommStartReadingAction::~ClCommStartReadingAction ()
{
  //## begin ClCommStartReadingAction::~ClCommStartReadingAction%955103955.body preserve=yes
  //## end ClCommStartReadingAction::~ClCommStartReadingAction%955103955.body
}



//## Other Operations (implementation)
m4return_t ClCommStartReadingAction::Execute ()
{
  //## begin ClCommStartReadingAction::Execute%954227123.body preserve=yes
	m4TraceLevel(2,cout << "R "<<flush);
 	
	M4_ASSERT(NULL!=m_poTask);

	ClCommReadingTask *theReadingTask=(ClCommReadingTask *)m_poTask;

	if(PAUSE_READING==theReadingTask->GetState())		 // Se ha puesto en pausa para procesar lo leido
		theReadingTask->SetState(READING);
	else
		_PassReturnInfo();	
	//  BeginRead(m4tliid_t ai_Identifier, ClIOEventHandler *ai_pIOEventHandler);
	m_poCommImplementation->GetCTL().BeginRead(theReadingTask->GetTLIID(),theReadingTask->GetCommEventHandler());

	return M4_SUCCESS;
  //## end ClCommStartReadingAction::Execute%954227123.body
}

m4return_t ClCommStartReadingAction::PrepareTask (m4handle_t ai_hConnectionID, m4tliid_t  ai_TLIID, ClCommProtocol *  ai_pProtocol)
{
  //## begin ClCommStartReadingAction::PrepareTask%965898334.body preserve=yes
	ClCommReadingTask *theReadingTask;
	if(M4_ERROR==m_poCommImplementation->GetReadingTask(ai_hConnectionID,theReadingTask))
	{
		theReadingTask=(ClCommReadingTask*)m_poCommImplementation->m_oBin.Get(M4_COMM_READING_TASK);
		m_poCommImplementation->AddReadingTask(ai_hConnectionID,theReadingTask);
		theReadingTask->SetCommProtocol(ai_pProtocol); 
		theReadingTask->SetTLIID(ai_TLIID);		
		theReadingTask->SetConnectionID(ai_hConnectionID);
	}
	m_poTask=theReadingTask;
	m_poTask->SetParams(m_pParamList);
	RemoveParams();
	return M4_SUCCESS;
  //## end ClCommStartReadingAction::PrepareTask%965898334.body
}

// Additional Declarations
  //## begin ClCommStartReadingAction%38E06F94010A.declarations preserve=yes
  //## end ClCommStartReadingAction%38E06F94010A.declarations

// Class ClCommReadedAction 

ClCommReadedAction::ClCommReadedAction ()
  //## begin ClCommReadedAction::ClCommReadedAction%955469192.hasinit preserve=no
  //## end ClCommReadedAction::ClCommReadedAction%955469192.hasinit
  //## begin ClCommReadedAction::ClCommReadedAction%955469192.initialization preserve=yes
  //## end ClCommReadedAction::ClCommReadedAction%955469192.initialization
{
  //## begin ClCommReadedAction::ClCommReadedAction%955469192.body preserve=yes
  //## end ClCommReadedAction::ClCommReadedAction%955469192.body
}


ClCommReadedAction::~ClCommReadedAction ()
{
  //## begin ClCommReadedAction::~ClCommReadedAction%955469193.body preserve=yes
  //## end ClCommReadedAction::~ClCommReadedAction%955469193.body
}



//## Other Operations (implementation)
m4return_t ClCommReadedAction::Execute ()
{
  //## begin ClCommReadedAction::Execute%954227122.body preserve=yes
	m4TraceLevel(2,cout << "Rd "<<flush);

	M4_ASSERT(NULL!=m_poTask);
	((ClCommReadingTask*)m_poTask)->NotifyReaded();

	return M4_SUCCESS;

  //## end ClCommReadedAction::Execute%954227122.body
}

// Additional Declarations
  //## begin ClCommReadedAction%38E06FA801EF.declarations preserve=yes
  //## end ClCommReadedAction%38E06FA801EF.declarations

// Class ClCommStopReadingAction 

ClCommStopReadingAction::ClCommStopReadingAction ()
  //## begin ClCommStopReadingAction::ClCommStopReadingAction%955103956.hasinit preserve=no
  //## end ClCommStopReadingAction::ClCommStopReadingAction%955103956.hasinit
  //## begin ClCommStopReadingAction::ClCommStopReadingAction%955103956.initialization preserve=yes
  //## end ClCommStopReadingAction::ClCommStopReadingAction%955103956.initialization
{
  //## begin ClCommStopReadingAction::ClCommStopReadingAction%955103956.body preserve=yes
  //## end ClCommStopReadingAction::ClCommStopReadingAction%955103956.body
}


ClCommStopReadingAction::~ClCommStopReadingAction ()
{
  //## begin ClCommStopReadingAction::~ClCommStopReadingAction%955103957.body preserve=yes
  //## end ClCommStopReadingAction::~ClCommStopReadingAction%955103957.body
}



//## Other Operations (implementation)
m4return_t ClCommStopReadingAction::Execute ()
{
  //## begin ClCommStopReadingAction::Execute%954227124.body preserve=yes
	m4TraceLevel(2,cout << "SR "<<flush);
	if(NULL==m_poTask)
	{
		M4_SRV_DEBUG_N("Conection already closed");
		Notify(m_pParamList);
		return M4_ERROR;
	}
	if((NULL==m_poTask->GetParams()) && m_pParamList)
	{
		m_poTask->SetParams (m_pParamList);
		RemoveParams();
	}
	_PassReturnInfo();

//		m4return_t StopReading (m4tliid_t ai_Identifier, ClIOEventHandler *ai_pIOEventHandler);
	return m_poCommImplementation ->GetCTL().StopReading(m_poTask->GetTLIID(),m_poTask->GetCommEventHandler());
  //## end ClCommStopReadingAction::Execute%954227124.body
}

m4return_t ClCommStopReadingAction::PrepareTask (m4handle_t ai_hConnectionID, m4tliid_t  ai_TLIID, ClCommProtocol *  ai_pProtocol)
{
  //## begin ClCommStopReadingAction::PrepareTask%966440286.body preserve=yes
	ClCommReadingTask *theReadingTask;
	if(NULL==m_poTask)
	{
		if(M4_ERROR==m_poCommImplementation->GetReadingTask(ai_hConnectionID,theReadingTask))
		{
			M4_SRV_DEBUG("Connection %0:s is not reading", ai_hConnectionID );
			return M4_ERROR;
		}
		eCommTaskState TaskStatus=theReadingTask->GetState ();
		if ( (TaskStatus==DETECTED_CLOSED) || (TaskStatus==DETECTED_CLOSED_AND_REQUEST_PENDING))
			return M4_ERROR;

		m_poTask=theReadingTask;
		if(m_pParamList)
		{
			m_poTask->SetParams(m_pParamList);
			RemoveParams();
		}
	}
	m_poTask->SetState(CLOSING);
	return M4_SUCCESS;
  //## end ClCommStopReadingAction::PrepareTask%966440286.body
}

// Additional Declarations
  //## begin ClCommStopReadingAction%38E06FC703CA.declarations preserve=yes
  //## end ClCommStopReadingAction%38E06FC703CA.declarations

// Class ClCommWriteAction 

ClCommWriteAction::ClCommWriteAction ()
  //## begin ClCommWriteAction::ClCommWriteAction%955103958.hasinit preserve=no
  //## end ClCommWriteAction::ClCommWriteAction%955103958.hasinit
  //## begin ClCommWriteAction::ClCommWriteAction%955103958.initialization preserve=yes
  //## end ClCommWriteAction::ClCommWriteAction%955103958.initialization
{
  //## begin ClCommWriteAction::ClCommWriteAction%955103958.body preserve=yes
  //## end ClCommWriteAction::ClCommWriteAction%955103958.body
}


ClCommWriteAction::~ClCommWriteAction ()
{
  //## begin ClCommWriteAction::~ClCommWriteAction%955103959.body preserve=yes
  //## end ClCommWriteAction::~ClCommWriteAction%955103959.body
}



//## Other Operations (implementation)
m4return_t ClCommWriteAction::Execute ()
{
  //## begin ClCommWriteAction::Execute%954227127.body preserve=yes
	m4TraceLevel(2,cout << "W "<<flush);
	m4return_t iRet;
	M4_ASSERT(NULL!=m_poTask);
	eCommTaskState eTaskState=m_poTask->GetState();
	if( (eTaskState!=CLOSING) && (eTaskState!=DETECTED_CLOSED) )
	{
		_PassReturnInfo();
		
		iRet=((ClCommWrittingTask *)m_poTask)->PrepareWrite();
	}
	else
	{
		iRet= M4_ERROR;
		((ClCommWrittingTask *)m_poTask)->PrepareReturnParams(M4_ERROR);
		ClParamList *theparams=m_poTask->GetParams();
		m_poTask->RemoveParams();
		Notify(theparams);
		delete m_poTask->GetCommProtocol();
		ClCommunicationImplementation::GetInstance()->m_oBin.Release((ClCommWrittingTask *)m_poTask);
		return M4_ERROR;
	}

	if(iRet!=M4_SUCCESS)
	{
		((ClCommWrittingTask *)m_poTask)->PrepareReturnParams(M4_ERROR);
		ClParamList *theparams=m_poTask->GetParams();
		m_poTask->RemoveParams();
		Notify(theparams);
	}
	return iRet;
  //## end ClCommWriteAction::Execute%954227127.body
}

m4return_t ClCommWriteAction::PrepareTask (m4handle_t ai_hConnectionID, m4tliid_t  ai_TLIID, ClCommProtocol *  ai_pProtocol)
{
  //## begin ClCommWriteAction::PrepareTask%965898336.body preserve=yes
	ClCommWrittingTask*theWrittingTask;
	if(M4_ERROR==m_poCommImplementation->GetWrittingTask(ai_hConnectionID,theWrittingTask))
	{
		m_poTask=NULL;
		return M4_ERROR;

	}
	else
	{
		switch(theWrittingTask->GetState())
		{
		case CLOSING:
		case DETECTED_CLOSED:
		case DETECTED_CLOSED_AND_REQUEST_PENDING:
			viewState(theWrittingTask->GetState());
			return M4_ERROR;
			break;
		
		case WRITTING:
		case FINISHING_WRITTING:
			viewState(theWrittingTask->GetState());
			return M4_ERROR;
			break;
		case START_WRITTING:
		case NONE:
			//viewState(theWrittingTask->GetState());
			//theWrittingTask->SetState(START_WRITTING);
			break;
		default:
			viewState(theWrittingTask->GetState());
			M4_ASSERT(0);
			return M4_ERROR;
			break;
		}
	}
	m_poTask=theWrittingTask;
	return M4_SUCCESS;
  //## end ClCommWriteAction::PrepareTask%965898336.body
}

// Additional Declarations
  //## begin ClCommWriteAction%38E070210008.declarations preserve=yes
  //## end ClCommWriteAction%38E070210008.declarations

// Class ClCommWrittenAction 

ClCommWrittenAction::ClCommWrittenAction ()
  //## begin ClCommWrittenAction::ClCommWrittenAction%955103960.hasinit preserve=no
  //## end ClCommWrittenAction::ClCommWrittenAction%955103960.hasinit
  //## begin ClCommWrittenAction::ClCommWrittenAction%955103960.initialization preserve=yes
  //## end ClCommWrittenAction::ClCommWrittenAction%955103960.initialization
{
  //## begin ClCommWrittenAction::ClCommWrittenAction%955103960.body preserve=yes
  //## end ClCommWrittenAction::ClCommWrittenAction%955103960.body
}


ClCommWrittenAction::~ClCommWrittenAction ()
{
  //## begin ClCommWrittenAction::~ClCommWrittenAction%955103961.body preserve=yes
  //## end ClCommWrittenAction::~ClCommWrittenAction%955103961.body
}



//## Other Operations (implementation)
m4return_t ClCommWrittenAction::Execute ()
{
  //## begin ClCommWrittenAction::Execute%954227126.body preserve=yes
	m4TraceLevel(2,cout << "Wn "<<flush);

	Notify(m_pParamList);
	RemoveParams();

	return M4_SUCCESS;
  //## end ClCommWrittenAction::Execute%954227126.body
}

// Additional Declarations
  //## begin ClCommWrittenAction%38E0703401DC.declarations preserve=yes
  //## end ClCommWrittenAction%38E0703401DC.declarations

// Class ClCommStopWrittingAction 

ClCommStopWrittingAction::ClCommStopWrittingAction ()
  //## begin ClCommStopWrittingAction::ClCommStopWrittingAction%955103962.hasinit preserve=no
  //## end ClCommStopWrittingAction::ClCommStopWrittingAction%955103962.hasinit
  //## begin ClCommStopWrittingAction::ClCommStopWrittingAction%955103962.initialization preserve=yes
  //## end ClCommStopWrittingAction::ClCommStopWrittingAction%955103962.initialization
{
  //## begin ClCommStopWrittingAction::ClCommStopWrittingAction%955103962.body preserve=yes
  //## end ClCommStopWrittingAction::ClCommStopWrittingAction%955103962.body
}


ClCommStopWrittingAction::~ClCommStopWrittingAction ()
{
  //## begin ClCommStopWrittingAction::~ClCommStopWrittingAction%955103963.body preserve=yes
  //## end ClCommStopWrittingAction::~ClCommStopWrittingAction%955103963.body
}



//## Other Operations (implementation)
m4return_t ClCommStopWrittingAction::Execute ()
{
  //## begin ClCommStopWrittingAction::Execute%954227125.body preserve=yes
	m4TraceLevel(2,cout << "SW "<<flush);
	return M4_SUCCESS;
  //## end ClCommStopWrittingAction::Execute%954227125.body
}

// Additional Declarations
  //## begin ClCommStopWrittingAction%38E0706601F2.declarations preserve=yes
  //## end ClCommStopWrittingAction%38E0706601F2.declarations

// Class ClCommFindFreeConnAction 

ClCommFindFreeConnAction::ClCommFindFreeConnAction ()
  //## begin ClCommFindFreeConnAction::ClCommFindFreeConnAction%955103964.hasinit preserve=no
  //## end ClCommFindFreeConnAction::ClCommFindFreeConnAction%955103964.hasinit
  //## begin ClCommFindFreeConnAction::ClCommFindFreeConnAction%955103964.initialization preserve=yes
  //## end ClCommFindFreeConnAction::ClCommFindFreeConnAction%955103964.initialization
{
  //## begin ClCommFindFreeConnAction::ClCommFindFreeConnAction%955103964.body preserve=yes
  //## end ClCommFindFreeConnAction::ClCommFindFreeConnAction%955103964.body
}


ClCommFindFreeConnAction::~ClCommFindFreeConnAction ()
{
  //## begin ClCommFindFreeConnAction::~ClCommFindFreeConnAction%955103965.body preserve=yes
  //## end ClCommFindFreeConnAction::~ClCommFindFreeConnAction%955103965.body
}



//## Other Operations (implementation)
m4return_t ClCommFindFreeConnAction::Execute ()
{
  //## begin ClCommFindFreeConnAction::Execute%954835556.body preserve=yes
	m4TraceLevel(2,cout << "FF "<<flush);
	return M4_SUCCESS;
  //## end ClCommFindFreeConnAction::Execute%954835556.body
}

// Additional Declarations
  //## begin ClCommFindFreeConnAction%38E0815700D6.declarations preserve=yes
  //## end ClCommFindFreeConnAction%38E0815700D6.declarations

// Class ClCommStartAcceptingAction 

ClCommStartAcceptingAction::ClCommStartAcceptingAction ()
  //## begin ClCommStartAcceptingAction::ClCommStartAcceptingAction%955469194.hasinit preserve=no
  //## end ClCommStartAcceptingAction::ClCommStartAcceptingAction%955469194.hasinit
  //## begin ClCommStartAcceptingAction::ClCommStartAcceptingAction%955469194.initialization preserve=yes
  //## end ClCommStartAcceptingAction::ClCommStartAcceptingAction%955469194.initialization
{
  //## begin ClCommStartAcceptingAction::ClCommStartAcceptingAction%955469194.body preserve=yes
  //## end ClCommStartAcceptingAction::ClCommStartAcceptingAction%955469194.body
}


ClCommStartAcceptingAction::~ClCommStartAcceptingAction ()
{
  //## begin ClCommStartAcceptingAction::~ClCommStartAcceptingAction%955469195.body preserve=yes
  //## end ClCommStartAcceptingAction::~ClCommStartAcceptingAction%955469195.body
}



//## Other Operations (implementation)
m4return_t ClCommStartAcceptingAction::Execute ()
{
  //## begin ClCommStartAcceptingAction::Execute%955469196.body preserve=yes
	m4TraceLevel(2,cout<< "A "<<flush);
	
	M4_ASSERT(NULL!=m_poTask);

	m4return_t iRet;
	ClCommOpenPortTask *theAcceptingTask=(ClCommOpenPortTask *)m_poTask;
	m4int_t iPort=theAcceptingTask->GetPort();
	

	m4bool_t bSSL=M4_FALSE;
	m4pchar_t pcSSL_CERT;
	m4pchar_t pcSSL_PRIVKEY;
	m4pchar_t pcSSL_PREFERED_CIPHER;

	iRet=theAcceptingTask->GetSSLParams(pcSSL_PRIVKEY,pcSSL_CERT,pcSSL_PREFERED_CIPHER);

	if( (NULL!=pcSSL_CERT) &&	(NULL!=pcSSL_PRIVKEY) )
		bSSL=M4_TRUE;


	m_poCommImplementation->AddPort(iPort,m_poTask);

	m_poTask->SetCondition(GetCondition());

	if(bSSL)
		iRet=m_poCommImplementation ->GetCTL().OpenSSLPort(iPort,m_poTask->GetCommEventHandler(),pcSSL_CERT,pcSSL_PRIVKEY, pcSSL_PREFERED_CIPHER);
	else
	{
		// m4return_t OpenPort (int ai_iPort, ClIOEventHandler *ai_pIOEventHandler);
		iRet=m_poCommImplementation ->GetCTL().OpenPort(iPort,m_poTask->GetCommEventHandler());
	}

	return M4_SUCCESS;
  //## end ClCommStartAcceptingAction::Execute%955469196.body
}

m4return_t ClCommStartAcceptingAction::PrepareTask (m4handle_t ai_hConnectionID, m4tliid_t  ai_TLIID, ClCommProtocol *  ai_pProtocol)
{
  //## begin ClCommStartAcceptingAction::PrepareTask%966411245.body preserve=yes
	ClCommOpenPortTask *theAcceptingTask=(ClCommOpenPortTask *)m_poCommImplementation->m_oBin.Get(M4_COMM_OPENPORT_TASK);
	m_poTask=theAcceptingTask;

	int iPort;
	m_pParamList->GetInt(M4_PARAM_PORT,iPort);
	theAcceptingTask->SetPort(iPort);

	int iProtocolType;
	m_pParamList->GetInt(M4_PARAM_COMM_PROTOCOL_TYPE,iProtocolType);
	theAcceptingTask->SetCommProtocolType(iProtocolType);

	theAcceptingTask->SetActionName((char *)m_strActionName.c_str());
	theAcceptingTask->SetExecutable(m_pExecutableInterface);

	m4pchar_t pcSSL_CERT=NULL;
	m4pchar_t pcSSL_PRIVKEY=NULL;
	m4pchar_t pcSSL_PREFERED_CIPHER=NULL;

	m4return_t 	iRet=m_pParamList->GetString(M4_PARAM_SSL_CERT,pcSSL_CERT);
	if(M4_ERROR!=iRet)
	{
		m_pParamList->GetString(M4_PARAM_SSL_PRIVKEY,pcSSL_PRIVKEY);
		m_pParamList->GetString(M4_PARAM_SSL_PREFERED_CIPHER,pcSSL_PREFERED_CIPHER);
		theAcceptingTask->SetSSLParams(pcSSL_PRIVKEY,pcSSL_CERT,pcSSL_PREFERED_CIPHER);
	}
	m_poTask->SetParams(m_pParamList);
	RemoveParams();
	return M4_SUCCESS;
  //## end ClCommStartAcceptingAction::PrepareTask%966411245.body
}

// Additional Declarations
  //## begin ClCommStartAcceptingAction%38F34EDB00EB.declarations preserve=yes
  //## end ClCommStartAcceptingAction%38F34EDB00EB.declarations

// Class ClCommStopAcceptingAction 

ClCommStopAcceptingAction::ClCommStopAcceptingAction ()
  //## begin ClCommStopAcceptingAction::ClCommStopAcceptingAction%955469197.hasinit preserve=no
  //## end ClCommStopAcceptingAction::ClCommStopAcceptingAction%955469197.hasinit
  //## begin ClCommStopAcceptingAction::ClCommStopAcceptingAction%955469197.initialization preserve=yes
  //## end ClCommStopAcceptingAction::ClCommStopAcceptingAction%955469197.initialization
{
  //## begin ClCommStopAcceptingAction::ClCommStopAcceptingAction%955469197.body preserve=yes
  //## end ClCommStopAcceptingAction::ClCommStopAcceptingAction%955469197.body
}


ClCommStopAcceptingAction::~ClCommStopAcceptingAction ()
{
  //## begin ClCommStopAcceptingAction::~ClCommStopAcceptingAction%955469198.body preserve=yes
  //## end ClCommStopAcceptingAction::~ClCommStopAcceptingAction%955469198.body
}



//## Other Operations (implementation)
m4return_t ClCommStopAcceptingAction::Execute ()
{
  //## begin ClCommStopAcceptingAction::Execute%955469199.body preserve=yes
	m4TraceLevel(2,cout<< "SA "<<flush);
	M4_ASSERT(NULL!=m_poTask);
	_PassReturnInfo();
	//m4return_t ClTransportLayerSynchronous::ClosePort (m4tliid_t ai_Identifier, ClIOEventHandler *ai_pIOEventHandler)
	m_poCommImplementation ->GetCTL().ClosePort (m_poTask->GetTLIID (), m_poTask->GetCommEventHandler());

	return M4_SUCCESS;
  //## end ClCommStopAcceptingAction::Execute%955469199.body
}

m4return_t ClCommStopAcceptingAction::PrepareTask (m4handle_t ai_hConnectionID, m4tliid_t  ai_TLIID, ClCommProtocol *  ai_pProtocol)
{
  //## begin ClCommStopAcceptingAction::PrepareTask%966496757.body preserve=yes
	return M4_WARNING;
  //## end ClCommStopAcceptingAction::PrepareTask%966496757.body
}

// Additional Declarations
  //## begin ClCommStopAcceptingAction%38F351470146.declarations preserve=yes
  //## end ClCommStopAcceptingAction%38F351470146.declarations

// Class ClCommOpenConnectionAction 

ClCommOpenConnectionAction::ClCommOpenConnectionAction ()
  //## begin ClCommOpenConnectionAction::ClCommOpenConnectionAction%955537525.hasinit preserve=no
  //## end ClCommOpenConnectionAction::ClCommOpenConnectionAction%955537525.hasinit
  //## begin ClCommOpenConnectionAction::ClCommOpenConnectionAction%955537525.initialization preserve=yes
  //## end ClCommOpenConnectionAction::ClCommOpenConnectionAction%955537525.initialization
{
  //## begin ClCommOpenConnectionAction::ClCommOpenConnectionAction%955537525.body preserve=yes
  //## end ClCommOpenConnectionAction::ClCommOpenConnectionAction%955537525.body
}


ClCommOpenConnectionAction::~ClCommOpenConnectionAction ()
{
  //## begin ClCommOpenConnectionAction::~ClCommOpenConnectionAction%955537526.body preserve=yes
  //## end ClCommOpenConnectionAction::~ClCommOpenConnectionAction%955537526.body
}



//## Other Operations (implementation)
m4return_t ClCommOpenConnectionAction::Execute ()
{
  //## begin ClCommOpenConnectionAction::Execute%955537527.body preserve=yes
	m4TraceLevel(2,cout << "O "<<flush);

	M4_ASSERT(NULL!=m_poTask);

	ClCommOpeningSocketTask *thetask=(ClCommOpeningSocketTask *)m_poTask;
	m4return_t iRet;

	m4pchar_t pcSSL_CACERT=thetask->GetSSLCert();

	_PassReturnInfo();

	// m4return_t Connect (char *ai_Host, int ai_iPort, ClIOEventHandler *ai_pIOEventHandler);
	if (NULL!=pcSSL_CACERT)
		iRet=m_poCommImplementation ->GetCTL().ConnectSSL(thetask->GetHost(),thetask->GetPort(),m_poTask->GetCommEventHandler(), pcSSL_CACERT);
	else
		iRet=m_poCommImplementation ->GetCTL().Connect(thetask->GetHost(),thetask->GetPort(),m_poTask->GetCommEventHandler());

	return M4_SUCCESS;
  //## end ClCommOpenConnectionAction::Execute%955537527.body
}

m4return_t ClCommOpenConnectionAction::PrepareTask (m4handle_t ai_hConnectionID, m4tliid_t  ai_TLIID, ClCommProtocol *  ai_pProtocol)
{
  //## begin ClCommOpenConnectionAction::PrepareTask%965977803.body preserve=yes
	
	ClCommOpeningSocketTask *thetask=(ClCommOpeningSocketTask *)m_poCommImplementation->m_oBin.Get(M4_COMM_OPENSOCKET_TASK);
	m_poTask=thetask;

	int iConnID;
	m_pParamList->GetInt(M4_PARAM_CONNECTIONID,iConnID);
	m_poTask->SetConnectionID(iConnID);

	int iPort;
	m_pParamList->GetInt(M4_PARAM_PORT,iPort);
	thetask->SetPort(iPort);

	m4pchar_t pcHost;
	m_pParamList->GetString(M4_PARAM_HOST,pcHost);
	thetask->SetHost(pcHost);

	int iCommProtocolType=0;
	m_pParamList->GetInt(M4_PARAM_COMM_PROTOCOL_TYPE,iCommProtocolType);
	thetask->SetCommProtocolType(iCommProtocolType);

	m4pchar_t pcSSLCert;
	m4return_t iRet=m_pParamList->GetString(M4_PARAM_SSL_CACERT,pcSSLCert);
	if(M4_ERROR!=iRet)
		thetask->SetSSLCert(pcSSLCert);

	m_poTask->SetParams(m_pParamList);
	RemoveParams();
	
	return M4_SUCCESS;
  //## end ClCommOpenConnectionAction::PrepareTask%965977803.body
}

// Additional Declarations
  //## begin ClCommOpenConnectionAction%38F45B6B0248.declarations preserve=yes
  //## end ClCommOpenConnectionAction%38F45B6B0248.declarations

// Class ClCommCloseConnectionAction 

ClCommCloseConnectionAction::ClCommCloseConnectionAction ()
  //## begin ClCommCloseConnectionAction::ClCommCloseConnectionAction%955613876.hasinit preserve=no
  //## end ClCommCloseConnectionAction::ClCommCloseConnectionAction%955613876.hasinit
  //## begin ClCommCloseConnectionAction::ClCommCloseConnectionAction%955613876.initialization preserve=yes
  //## end ClCommCloseConnectionAction::ClCommCloseConnectionAction%955613876.initialization
{
  //## begin ClCommCloseConnectionAction::ClCommCloseConnectionAction%955613876.body preserve=yes
  //## end ClCommCloseConnectionAction::ClCommCloseConnectionAction%955613876.body
}


ClCommCloseConnectionAction::~ClCommCloseConnectionAction ()
{
  //## begin ClCommCloseConnectionAction::~ClCommCloseConnectionAction%955613877.body preserve=yes
  //## end ClCommCloseConnectionAction::~ClCommCloseConnectionAction%955613877.body
}



//## Other Operations (implementation)
m4return_t ClCommCloseConnectionAction::Execute ()
{
  //## begin ClCommCloseConnectionAction::Execute%955613878.body preserve=yes
	m4TraceLevel(2,cout << "C "<<flush);

	M4_ASSERT(NULL!=m_poTask);
	if((NULL==m_poTask->GetParams()) || m_pParamList)
	{
		m_poTask->SetParams(m_pParamList);
		RemoveParams();
	}
	_PassReturnInfo();

	m_poCommImplementation->GetCTL().StopConnection(m_poTask->GetTLIID (), m_poTask->GetCommEventHandler());

	return M4_SUCCESS;
  //## end ClCommCloseConnectionAction::Execute%955613878.body
}

m4return_t ClCommCloseConnectionAction::PrepareTask (m4handle_t ai_hConnectionID, m4tliid_t  ai_TLIID, ClCommProtocol *  ai_pProtocol)
{
  //## begin ClCommCloseConnectionAction::PrepareTask%966496756.body preserve=yes
	ClCommCloseSocketTask *theTask;
	theTask=(ClCommCloseSocketTask *)m_poCommImplementation->m_oBin.Get(M4_COMM_CLOSESOCKET_TASK);
	theTask->SetTLIID(ai_TLIID);		
	theTask->SetConnectionID(ai_hConnectionID);
	if( ((int)ai_pProtocol==1) || ((int)ai_pProtocol==0))
		theTask->SetCommProtocol(ai_pProtocol);
	m_poTask=theTask;
	if(NULL!=m_pParamList)
	{
		m_poTask->SetParams(m_pParamList);
		RemoveParams();
	}
	return M4_SUCCESS;
  //## end ClCommCloseConnectionAction::PrepareTask%966496756.body
}

// Additional Declarations
  //## begin ClCommCloseConnectionAction%38F45B8F01C8.declarations preserve=yes
  //## end ClCommCloseConnectionAction%38F45B8F01C8.declarations

// Class ClCommShutDownAction 

ClCommShutDownAction::ClCommShutDownAction ()
  //## begin ClCommShutDownAction::ClCommShutDownAction%958646099.hasinit preserve=no
  //## end ClCommShutDownAction::ClCommShutDownAction%958646099.hasinit
  //## begin ClCommShutDownAction::ClCommShutDownAction%958646099.initialization preserve=yes
  : m_bFirstShutdownStep(M4_TRUE)
  //## end ClCommShutDownAction::ClCommShutDownAction%958646099.initialization
{
  //## begin ClCommShutDownAction::ClCommShutDownAction%958646099.body preserve=yes
  //## end ClCommShutDownAction::ClCommShutDownAction%958646099.body
}


ClCommShutDownAction::~ClCommShutDownAction ()
{
  //## begin ClCommShutDownAction::~ClCommShutDownAction%958646100.body preserve=yes
  //## end ClCommShutDownAction::~ClCommShutDownAction%958646100.body
}



//## Other Operations (implementation)
m4return_t ClCommShutDownAction::Execute ()
{
  //## begin ClCommShutDownAction::Execute%958646101.body preserve=yes
	m4TraceLevel(2,cout<< "SD "<<flush);
	if(m_bFirstShutdownStep)
	{
		ClCommShutDownTask *theTask=(ClCommShutDownTask *)m_poCommImplementation->m_oBin.Get(M4_COMM_SHUTDOWN_TASK);
		theTask->SetParams(m_pParamList);
		m_poTask=theTask;
		RemoveParams();
		_PassReturnInfo();

		m_poCommImplementation->GetCTL().StopTransportLayer(theTask->GetCommEventHandler());
		return M4_SUCCESS;
	}
	else
	{
		ClCommunicationImplementation *poImp=ClCommunicationImplementation::GetInstance();
		if(poImp==NULL)
			return M4_ERROR;
		else
		{
			m4return_t iRet=poImp->StopCommunicationLayer();
//			delete poImp;
			if(iRet==M4_SUCCESS)
				return M4_COMM_SHUTDOWN_WORKER;
			else
				return iRet;
		}
	}
  //## end ClCommShutDownAction::Execute%958646101.body
}


ClCommSetLangAction::ClCommSetLangAction()
{
}

ClCommSetLangAction::~ClCommSetLangAction()
{
}

m4return_t ClCommSetLangAction::Execute ()
{

#ifndef _UNIX
	m4return_t		iRet = M4_SUCCESS ;
	m4bool_t		bRet = M4_FALSE ;
	m4int_t			iLanguage = 0 ;
	m4LCID_t		iLCID = LOGSYS_INVALID_M4UINT32 ;

	m_pParamList->GetInt( M4_PARAM_COMM_LANG_ID, iLanguage ) ;

	iLCID = M4ClLangInfo::Instance()->GetErrorLCID( iLanguage ) ;

	if( iLCID != LOGSYS_INVALID_M4UINT32 )
	{
		bRet = SetThreadLocale( iLCID ) ;

		if( !bRet )
		{
			iRet = M4_ERROR ;
		}
	}
	else
	{
		iRet = M4_ERROR ;
	}
	
	return iRet ;

#else

	return M4_SUCCESS ;

#endif

}

// Additional Declarations
  //## begin ClCommShutDownAction%3923F7E3002B.declarations preserve=yes
  //## end ClCommShutDownAction%3923F7E3002B.declarations

// Class ClCommExternalClock 






//## Other Operations (implementation)
m4return_t ClCommExternalClock::Start ()
{
  //## begin ClCommExternalClock::Start%967474877.body preserve=yes
	M4_ASSERT(NULL!=m_poCrono);
	m_poCrono->Start();
	return M4_SUCCESS;
  //## end ClCommExternalClock::Start%967474877.body
}

m4return_t ClCommExternalClock::Stop ()
{
  //## begin ClCommExternalClock::Stop%967474878.body preserve=yes
	M4_ASSERT(NULL!=m_poCrono);
	M4_ASSERT(NULL!=m_pmsExternalCounter);
	*m_pmsExternalCounter+=m_poCrono->Stop();
	return M4_SUCCESS;
  //## end ClCommExternalClock::Stop%967474878.body
}

m4return_t ClCommExternalClock::SetCounter (m4millisec_t &ai_msExternalCounter)
{
  //## begin ClCommExternalClock::SetCounter%967474879.body preserve=yes
	m_pmsExternalCounter=&ai_msExternalCounter;
	return M4_SUCCESS;
  //## end ClCommExternalClock::SetCounter%967474879.body
}

m4return_t ClCommExternalClock::SetClock (ClCrono *ai_poCrono)
{
  //## begin ClCommExternalClock::SetClock%967474880.body preserve=yes
	m_poCrono=ai_poCrono;
	return M4_SUCCESS;
  //## end ClCommExternalClock::SetClock%967474880.body
}

// Additional Declarations
  //## begin ClCommExternalClock%39AA857A00D0.declarations preserve=yes
  //## end ClCommExternalClock%39AA857A00D0.declarations

//## begin module%38DB631E018E.epilog preserve=yes
ClCommRemainReadedAction ::ClCommRemainReadedAction ()
{}

m4return_t ClCommRemainReadedAction ::Execute ()
{
	M4_ASSERT(m_poTask!=NULL);
	if(m_poTask->GetState ()==REQUEST_PENDING)
	{
		ClCommRemainReadedAction *poAction =new ClCommRemainReadedAction ;
		poAction ->m_poTask=m_poTask;
		ClCommunicationImplementation::GetInstance()->Push(poAction);
		return M4_ERROR;
	}
	// tengo que usar el buffer y decirle que siga leyendo
	return M4_SUCCESS;
}

//## end module%38DB631E018E.epilog
