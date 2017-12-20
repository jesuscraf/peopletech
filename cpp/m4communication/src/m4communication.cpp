//## begin module%38F1F1C2020C.cm preserve=no
//## end module%38F1F1C2020C.cm

//## begin module%38F1F1C2020C.cp preserve=no
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
//## end module%38F1F1C2020C.cp

//## Module: m4communication%38F1F1C2020C; Package body
//## Subsystem: M4Communication::M4COML::src%38D1FCE80326
//## Source file: e:\mybuild\m4communication\src\m4communication.cpp

//## begin module%38F1F1C2020C.additionalIncludes preserve=no
//## end module%38F1F1C2020C.additionalIncludes

//## begin module%38F1F1C2020C.includes preserve=yes
#include <communicationlayer.hpp>
#include <commactionsname.h>

//## end module%38F1F1C2020C.includes

// m4types
#include <m4types.hpp>
#include <basiclog.hpp>
// m4string
#include <m4string.hpp>
// m4communication
#include <m4communication.hpp>
// iexecutable
#include <iexecutable.hpp>


//## begin module%38F1F1C2020C.declarations preserve=no
//## end module%38F1F1C2020C.declarations

//## begin module%38F1F1C2020C.additionalDeclarations preserve=yes
#define M4_COMM_INTERNAL_TIMEOUT 600
m4char_t *M4_COMM_PROTOCOL_URL_PREFIX[]={"null","pdu","ascii","pduhttp"};
#define M4_SRV_TRACE_COMP_ID "COMM"
#include <m4srvtrace.h>
//## end module%38F1F1C2020C.additionalDeclarations


// Class ClCommunication 


ClCommunication::ClCommunication (m4int_t ai_iControlPort, m4int_t ai_iNWorker)
  //## begin ClCommunication::ClCommunication%955469190.hasinit preserve=no
      : m_poImplementation(NULL)
  //## end ClCommunication::ClCommunication%955469190.hasinit
  //## begin ClCommunication::ClCommunication%955469190.initialization preserve=yes
  //## end ClCommunication::ClCommunication%955469190.initialization
{
  //## begin ClCommunication::ClCommunication%955469190.body preserve=yes
	m_poImplementation=ClCommunicationImplementation::GetInstance();
	if(NULL==m_poImplementation)
		m_poImplementation=new ClCommunicationImplementation(ai_iControlPort,ai_iNWorker);
  //## end ClCommunication::ClCommunication%955469190.body
}


ClCommunication::~ClCommunication ()
{
  //## begin ClCommunication::~ClCommunication%955469191.body preserve=yes
	if(m_poImplementation)
	{	
		m_poImplementation=ClCommunicationImplementation::GetInstance();
		if(m_poImplementation)
			delete m_poImplementation;
		m_poImplementation=NULL;
	}
  //## end ClCommunication::~ClCommunication%955469191.body
}



//## Other Operations (implementation)
m4return_t ClCommunication::StartAccepting (m4int_t ai_iPort, m4pchar_t ai_pcActionName, ClExecutableInterface *ai_poActionExecutor, m4int_t ai_iCommProtocol, m4string_t ai_strCert, m4string_t ai_strPrivKey, m4string_t ai_strPreferedCipher)
{
  //## begin ClCommunication::StartAccepting%953574268.body preserve=yes
	if(_AnyErrors())
		return M4_ERROR;

	if(M4_SUCCESS==m_poImplementation->IsPortInUse(ai_iPort))
	{
		M4_SRV_DEBUG("The Port %0:s is in use",ai_iPort);
		return M4_ERROR;
	}

	ClCommStartAcceptingAction *theAction= new ClCommStartAcceptingAction() ;
	ClParamList * theParamList=new ClParamList ;

	theParamList->SetInt(M4_PARAM_PORT,ai_iPort);
	
	if( (!ai_strCert.empty()) && (!ai_strPrivKey.empty() ) )
	{
		theParamList->SetString(M4_PARAM_SSL_CERT,ai_strCert.c_str());
		theParamList->SetString(M4_PARAM_SSL_PRIVKEY,ai_strPrivKey.c_str());
	}

	if(!ai_strPreferedCipher.empty())
		theParamList->SetString(M4_PARAM_SSL_PREFERED_CIPHER,ai_strPreferedCipher.c_str());

	theParamList->SetInt(M4_PARAM_COMM_PROTOCOL_TYPE,ai_iCommProtocol);
	theAction->SetParams(theParamList);

	theAction->SetActionName(ai_pcActionName);
	theAction->SetExecutable(ai_poActionExecutor);

	
	theAction->PrepareTask(0,NULL,NULL);

	m4return_t retvalue= _PrepareReturn(NULL,NULL,theAction,theParamList);

	if(M4_ERROR==retvalue)
	{
		m_poImplementation->RemovePort(ai_iPort);
	}
	return retvalue;
  //## end ClCommunication::StartAccepting%953574268.body
}

m4return_t ClCommunication::OpenConnection (m4handle_t ai_hConnectionID, m4string_t &ai_strHost, m4int_t ai_iPort, m4pchar_t ai_pcActionName, ClExecutableInterface *ai_poActionExecutor, m4int_t ai_iCommProtocol, m4string_t  ai_strSSLCert, m4bool_t ai_bIsSocketPool)
{
  //## begin ClCommunication::OpenConnection%953574271.body preserve=yes
    m4Trace(cerr<<"OpenConnection(...)"<<endl);
	if(_AnyErrors())
    {
        m4Trace(cerr<<"_AnyErrors(): error"<<endl);

		return M4_ERROR;
    }

	ClCommOpenConnectionAction *theAction = new ClCommOpenConnectionAction();
	
	ClParamList *theParamList=new ClParamList ;

	theParamList->SetInt(M4_PARAM_SESSIONID,ai_hConnectionID);	
	theParamList->SetInt(M4_PARAM_CONNECTIONID,ai_hConnectionID);	
	theParamList->SetInt(M4_PARAM_PORT,ai_iPort);
	theParamList->SetString(M4_PARAM_HOST,ai_strHost.c_str());
	theParamList->SetInt(M4_PARAM_COMM_PROTOCOL_TYPE,ai_iCommProtocol);

	if(!ai_strSSLCert.empty())
		theParamList->SetString(M4_PARAM_SSL_CACERT,ai_strSSLCert.c_str());

	m4char_t theURL[256];
	sprintf(theURL,"%s://%s:%d",M4_COMM_PROTOCOL_URL_PREFIX[ai_iCommProtocol],ai_strHost.c_str(),ai_iPort);
	theParamList->SetString(M4_PARAM_COMM_URL,theURL);

	theAction->SetParams(theParamList);
	theAction ->PrepareTask(ai_hConnectionID,NULL,NULL);
	m4return_t retvalue=_PrepareReturn(ai_pcActionName,ai_poActionExecutor,theAction,theParamList);

	//delete theParamList;
	return retvalue;
  //## end ClCommunication::OpenConnection%953574271.body
}

m4return_t ClCommunication::OpenConnection (m4handle_t ai_hConnectionID, m4string_t &ai_strHost, m4int_t ai_iPort, m4int_t ai_iCommProtocol, m4string_t  ai_strSSLCert, m4bool_t ai_bIsSocketPool)
{
  //## begin ClCommunication::OpenConnection%958463332.body preserve=yes
	m4string_t strAux;
	return OpenConnection (ai_hConnectionID, ai_strHost, ai_iPort,NULL ,NULL, ai_iCommProtocol, ai_strSSLCert,ai_bIsSocketPool);
  //## end ClCommunication::OpenConnection%958463332.body
}

m4return_t ClCommunication::StartListening (m4handle_t ai_hConnectionID, m4pchar_t ai_pcActionName, ClExecutableInterface *ai_poActionExecutor)
{
  //## begin ClCommunication::StartListening%953574273.body preserve=yes
	if(_AnyErrors())
		return M4_ERROR;

	ClCommStartReadingAction *theAction=new ClCommStartReadingAction();
	ClParamList *theParams;
	
	m4return_t iRet=m_poImplementation->VerifyConnection(ai_hConnectionID,theParams,theAction);
	if(M4_ERROR==iRet)
	{
		delete theAction;
		return M4_ERROR;
	}
	M4_ASSERT(!((M4_WARNING==iRet) && !ai_pcActionName));
	return _PrepareReturn(ai_pcActionName,ai_poActionExecutor,theAction,theParams);
  //## end ClCommunication::StartListening%953574273.body
}

m4return_t ClCommunication::StartListening (m4handle_t ai_hConnectionID, ClParamList &ao_Params)
{
  //## begin ClCommunication::StartListening%958483733.body preserve=yes
	if(_AnyErrors())
		return M4_ERROR;

	ClCommStartReadingAction *theAction=new ClCommStartReadingAction();
	ClParamList *theParamList;

	if(M4_ERROR==m_poImplementation->VerifyConnection(ai_hConnectionID,theParamList,theAction))
	{
		delete theAction;
		return M4_ERROR;
	}

	m4return_t retvalue=_PrepareReturn(NULL,NULL,theAction,NULL);
	
	ao_Params.SetM4Return(M4_PARAM_RETURN_RESULT,retvalue);

	ao_Params.SetInt(M4_PARAM_SESSIONID,ai_hConnectionID);
	ao_Params.SetInt(M4_PARAM_CONNECTIONID,ai_hConnectionID);

	void *thepointer;
	if(M4_ERROR!=theParamList->GetPointer(M4_PARAM_COMM_PROTOCOL,thepointer))
	{
		ClCommProtocol *theprotocol=(ClCommProtocol *)thepointer;

		switch (theprotocol->GetCommProtocolType())
		{
		case M4_COMM_PDU_HTTP_PROTOCOL:
			if(M4_ERROR!=theParamList->GetPointer(M4_PARAM_READED_PDU,thepointer))
			{
				ao_Params.SetPointer(M4_PARAM_READED_PDU,thepointer);
				m4pchar_t theHeaders;
				if(M4_ERROR!=theParamList->GetString(M4_PARAM_READED_PDU_HEADERS,theHeaders))
				{
					retvalue=M4_SUCCESS;
					ao_Params.SetString(M4_PARAM_READED_PDU_HEADERS,theHeaders);
				}
			}
			break;
		case M4_COMM_PDU_PROTOCOL:
			if(M4_ERROR!=theParamList->GetPointer(M4_PARAM_READED_PDU,thepointer))
			{
				retvalue=M4_SUCCESS;
				ao_Params.SetPointer(M4_PARAM_READED_PDU,thepointer);
			}
			break;
		case M4_COMM_ASCII_PROTOCOL:
			if(M4_ERROR!=theParamList->GetPointer(M4_PARAM_WRITE_BUFFER,thepointer))
			{
				retvalue=M4_SUCCESS;
				ao_Params.SetPointer(M4_PARAM_WRITE_BUFFER,thepointer);
			}
			break;
		default:
			m4Trace(cerr << "Error de logica en ClCommReadedAction"<<endl);
			return M4_ERROR;
			break;
		}
	}
	delete theParamList;
	return retvalue;
  //## end ClCommunication::StartListening%958483733.body
}

m4return_t ClCommunication::Write (m4handle_t ai_hConnectionID, ClPDU *ai_poNetObject, m4pchar_t ai_pcActionName, ClExecutableInterface *ai_poActionExecutor)
{
  //## begin ClCommunication::Write%953574269.body preserve=yes
	return Write(ai_hConnectionID,0,ai_poNetObject,ai_pcActionName,ai_poActionExecutor);
  //## end ClCommunication::Write%953574269.body
}

m4return_t ClCommunication::Write (m4handle_t ai_hConnectionID, m4pchar_t ai_pcChunk, m4size_t ai_iChunkSize, m4pchar_t ai_pcActionName, ClExecutableInterface *ai_poActionExecutor)
{
  //## begin ClCommunication::Write%955613882.body preserve=yes
	return Write(ai_hConnectionID,0,ai_pcChunk, ai_iChunkSize, ai_pcActionName,ai_poActionExecutor);
  //## end ClCommunication::Write%955613882.body
}

m4return_t ClCommunication::Write (m4handle_t ai_hConnectionID, m4uint64_t ai_ui64RequestID, ClPDU *ai_poNetObject, m4pchar_t ai_pcActionName, ClExecutableInterface *ai_poActionExecutor)
{
  //## begin ClCommunication::Write%960977890.body preserve=yes
	if(_AnyErrors())
		return M4_ERROR;

	ClCommWriteAction *theAction = new ClCommWriteAction ();
	ClParamList *theParams;

	m4return_t iRet=m_poImplementation->VerifyConnection(ai_hConnectionID,theParams,theAction);
	if(M4_ERROR==iRet)
	{
		delete theAction;
		return M4_ERROR;
	}

	M4_ASSERT(!((M4_WARNING==iRet) && !ai_pcActionName));

	ClCommWrittingTask * theWTask=(ClCommWrittingTask *)(theAction->m_poTask);
	theWTask->SetPDU(ai_poNetObject );
	theWTask->SetRequestID((m4int64_t)ai_ui64RequestID );

	return _PrepareReturn(ai_pcActionName,ai_poActionExecutor,theAction,theParams);
  //## end ClCommunication::Write%960977890.body
}

m4return_t ClCommunication::Write (m4handle_t ai_hConnectionID, m4uint64_t ai_ui64RequestID, m4pchar_t ai_pcChunk, m4size_t ai_iChunkSize, m4pchar_t ai_pcActionName, ClExecutableInterface *ai_poActionExecutor)
{
  //## begin ClCommunication::Write%960977891.body preserve=yes
	if(_AnyErrors())
		return M4_ERROR;

	ClCommWriteAction *theAction = new ClCommWriteAction ();
	ClParamList *theParams;
	
	m4return_t iRet=m_poImplementation->VerifyConnection(ai_hConnectionID,theParams,theAction);
	if(M4_ERROR==iRet)
	{
		delete theAction;
		return M4_ERROR;
	}

	M4_ASSERT(!((M4_WARNING==iRet) && !ai_pcActionName));

	ClCommWrittingTask * theWTask=(ClCommWrittingTask *)(theAction->m_poTask);
	theWTask->SetBufferToWrite(ai_pcChunk,ai_iChunkSize);
	theWTask->SetRequestID(ai_ui64RequestID );
	
	return _PrepareReturn(ai_pcActionName,ai_poActionExecutor,theAction,theParams);
  //## end ClCommunication::Write%960977891.body
}

m4return_t ClCommunication::StopAccepting (m4int_t ai_iPort)
{
  //## begin ClCommunication::StopAccepting%953574270.body preserve=yes
	if(_AnyErrors())
		return M4_ERROR;

	ClCommTask *thetask=	m_poImplementation ->GetPortTask(ai_iPort);
	if(thetask==NULL)
	{
		M4_SRV_ERROR(0,"Invalid port %0:s", ai_iPort );
		return M4_ERROR;
	}
	ClCommStopAcceptingAction *theAction=new ClCommStopAcceptingAction() ;
	
	theAction->m_poTask=thetask;

	m4return_t retvalue=_PrepareReturn(NULL,NULL,theAction,thetask->GetParams ());

	m_poImplementation->RemovePort(ai_iPort);
	return retvalue;

  //## end ClCommunication::StopAccepting%953574270.body
}

m4return_t ClCommunication::CloseConnection (m4handle_t ai_hConnectionID, m4pchar_t ai_pcActionName, ClExecutableInterface *ai_poActionExecutor)
{
  //## begin ClCommunication::CloseConnection%953574272.body preserve=yes
	if(_AnyErrors())
		return M4_ERROR;

	ClCommCloseConnectionAction *theAction=new ClCommCloseConnectionAction();
	ClParamList *theParams;
	m4return_t iRet=m_poImplementation->VerifyConnection(ai_hConnectionID,theParams,theAction);
	if(M4_ERROR==iRet)
	{
		delete theAction;
		return M4_ERROR;
	}
	return _PrepareReturn(ai_pcActionName,ai_poActionExecutor,theAction,theParams);
  //## end ClCommunication::CloseConnection%953574272.body
}

m4return_t ClCommunication::StopListening (m4handle_t ai_hConnectionID, m4pchar_t ai_pcActionName, ClExecutableInterface *ai_poActionExecutor)
{
  //## begin ClCommunication::StopListening%953574274.body preserve=yes
	if(_AnyErrors())
		return M4_ERROR;

	ClCommStopReadingAction *theAction= new ClCommStopReadingAction();
	ClParamList *theParams;

	if(M4_ERROR==m_poImplementation->VerifyConnection(ai_hConnectionID,theParams,theAction))
	{
		m4Trace(cerr << "error stoplistening "<<ai_hConnectionID<< endl);
		delete theAction;
		return M4_ERROR;
	}

	return _PrepareReturn(ai_pcActionName,ai_poActionExecutor,theAction,theParams);
  //## end ClCommunication::StopListening%953574274.body
}

m4return_t ClCommunication::StopWritting (m4handle_t ai_hConnectionID, m4pchar_t ai_pcActionName, ClExecutableInterface *ai_poActionExecutor)
{
  //## begin ClCommunication::StopWritting%953574275.body preserve=yes
	if(_AnyErrors())
		return M4_ERROR;

	ClCommStopWrittingAction *theAction= new ClCommStopWrittingAction();
	ClParamList *theParams;

	if(M4_ERROR==m_poImplementation->VerifyConnection(ai_hConnectionID,theParams,theAction))
	{
		delete theAction;
		return M4_ERROR;
	}

	m_poImplementation->Push(theAction);
	return M4_SUCCESS;
  //## end ClCommunication::StopWritting%953574275.body
}

m4return_t ClCommunication::StopCommunicationLayer ()
{
  //## begin ClCommunication::StopCommunicationLayer%958646097.body preserve=yes
	if(_AnyErrors())
		return M4_ERROR;

	m4return_t iRet;
	ClCommShutDownAction *theAction=new ClCommShutDownAction ;
	iRet=_PrepareReturn(NULL,NULL,theAction,NULL);

	if(M4_SUCCESS!=iRet)
		{m4Trace(cerr << "Error haciendo shutdown de comunicaciones"<<endl);}

	return ClCommunicationImplementation::GetInstance()->StopCommunicationLayer();
	
  //## end ClCommunication::StopCommunicationLayer%958646097.body
}

m4return_t ClCommunication::GetCommStat (m4handle_t ai_hConnectionID, m4uint_t &ao_uiSentBytes, m4uint_t &ao_uiReadedBytes, m4uint_t &ao_uiSentRequests, m4uint_t &ao_uiReceivedRequests)
{
  //## begin ClCommunication::GetCommStat%959072019.body preserve=yes
	if(_AnyErrors())
		return M4_ERROR;

	ClCommProtocol *theprotocol;
	m4tliid_t theTLI;
	if(M4_ERROR==m_poImplementation->GetTLIandProtocol(ai_hConnectionID,theTLI,theprotocol))
	{
		M4_SRV_ERROR( 0, "Connection %0:s not found", ai_hConnectionID );
		return M4_ERROR;
	}
	ao_uiSentBytes=theprotocol->GetSentBytes();
	ao_uiReadedBytes=theprotocol->GetReadedBytes();
	ao_uiSentRequests=theprotocol->GetNSentRequests();
	ao_uiReceivedRequests=theprotocol->GetNReceivedRequests();
	return M4_SUCCESS;
  //## end ClCommunication::GetCommStat%959072019.body
}

m4return_t ClCommunication::GetCommParams (m4handle_t ai_hConnectionID, m4string_t &ao_strURL)
{
  //## begin ClCommunication::GetCommParams%959072025.body preserve=yes
	if(_AnyErrors())
		return M4_ERROR;

	if(M4_ERROR==m_poImplementation->GetURL(ai_hConnectionID,ao_strURL))
	{
		M4_SRV_ERROR( 0, "Connection %0:s not found", ai_hConnectionID );
		return M4_ERROR;
	}
	return M4_SUCCESS;
  //## end ClCommunication::GetCommParams%959072025.body
}

m4int_t ClCommunication::GetError ()
{
  //## begin ClCommunication::GetError%959072028.body preserve=yes
	if(m_poImplementation==NULL)
		return M4_ERROR;
	return m_poImplementation->GetError();
  //## end ClCommunication::GetError%959072028.body
}

m4return_t ClCommunication::GetWorkerStat (m4int_t ai_iWorkerID, m4uint_t &ao_uiExecutedActions, m4uint_t &ao_uiExecutingTime, m4uint_t &ao_uiWaitingTime, m4uint_t &ao_uiExternalExecutingTime)
{
  //## begin ClCommunication::GetWorkerStat%964423557.body preserve=yes
	if(_AnyErrors())
		return M4_ERROR;
	return m_poImplementation ->GetWorkerStats (ai_iWorkerID,ao_uiExecutedActions, ao_uiExecutingTime, ao_uiWaitingTime,ao_uiExternalExecutingTime);
  //## end ClCommunication::GetWorkerStat%964423557.body
}

m4return_t ClCommunication::_PrepareReturn (m4pchar_t ai_pcActionName, ClExecutableInterface *ai_poActionExecutor, ClCommAction *ai_poCommAction, ClParamList *ai_poParams)
{
  //## begin ClCommunication::_PrepareReturn%965219978.body preserve=yes
	if(_AnyErrors())
    {
        m4Trace(cerr<<"ClCommunication::_PrepareReturn::_AnyErrors(): error" << endl);
        if (NULL != ai_pcActionName)
        {
            m4Trace(cerr<<">>>ActionName: "<< ai_pcActionName << endl);
        }
		return M4_ERROR;
    }
	m4return_t retvalue = M4_SUCCESS;

	if ( (NULL!=ai_pcActionName) && (NULL!=ai_poActionExecutor))
	{
		ai_poCommAction->SetActionName(ai_pcActionName);
		ai_poCommAction->SetExecutable(ai_poActionExecutor);
		ai_poCommAction->SetCondition(NULL);
		m_poImplementation->Push(ai_poCommAction);
		retvalue=M4_SUCCESS;
	}
	else
	{
		ClCommCondition *theCondition=new ClCommCondition;

		if(NULL==ai_poParams)
		{
			if(NULL==ai_poCommAction->m_poTask)
			{
				ai_poParams=new ClParamList;
				ai_poCommAction->SetParams(ai_poParams);
			}
			else
			{
				ai_poParams=ai_poCommAction->m_poTask->GetParams();
				if(NULL==ai_poParams)
				{
					ai_poParams=new ClParamList;
					ai_poCommAction->m_poTask->SetParams(ai_poParams);
				}
			}
		}
		
		ai_poCommAction->SetCondition(theCondition);
		ai_poCommAction->SetActionName(NULL);
		ai_poCommAction->SetExecutable(NULL);
		
		m_poImplementation->Push(ai_poCommAction);
		m4return_t retCond =theCondition->Wait(M4_COMM_INTERNAL_TIMEOUT);
		
		// bg 304443
		if(retCond!=M4_WARNING)
		{
			ai_poParams->GetM4Return(M4_PARAM_RETURN_RESULT,retvalue);
			int auxInt;
			if(M4_ERROR!=ai_poParams->GetInt("DeleteParam",auxInt))
            {
				if(auxInt==1)
                {
					delete ai_poParams;
                }
            }
			
		    // bugid: 0095013
            // Si ha habido un timeout, estamos borrando este objeto y lo tenemos asignado
            // al ai_poCommAction. Cuando ai_poCommAction acceda theCondition se va a encontrar
            // un puntero a no se sabe que.
            // Borramos sólo en caso de que haya ido bien.
            // En caso contrario tenemos un memory leak, pero no es tan grave porque esto se da poco.
            delete theCondition;
		}
		else
		{
			m4Trace(cerr << "M4_COMM_INTERNAL_TIMEOUT"<<endl);
			retvalue=M4_ERROR;		// Se va a quedar la condition sin borrar
		}
		
        //delete theCondition;
	}
	return retvalue;
  //## end ClCommunication::_PrepareReturn%965219978.body
}

m4return_t ClCommunication::AddWorker ()
{
  //## begin ClCommunication::AddWorker%967794011.body preserve=yes
	if(_AnyErrors())
		return M4_ERROR;
	return m_poImplementation->AddWorker ();
  //## end ClCommunication::AddWorker%967794011.body
}

m4return_t ClCommunication::RemoveWorker ()
{
  //## begin ClCommunication::RemoveWorker%967794012.body preserve=yes
	if(_AnyErrors())
		return M4_ERROR;
	return m_poImplementation->RemoveWorker ();
  //## end ClCommunication::RemoveWorker%967794012.body
}

m4int_t ClCommunication::GetNWorker ()
{
  //## begin ClCommunication::GetNWorker%967794013.body preserve=yes
	if(_AnyErrors())
		return M4_ERROR;
	return m_poImplementation->GetNWorker ();
  //## end ClCommunication::GetNWorker%967794013.body
}

m4int_t ClCommunication::GetNPendingActions ()
{
  //## begin ClCommunication::GetNPendingActions%967794014.body preserve=yes
	if(_AnyErrors())
		return M4_ERROR;
	return m_poImplementation->GetNPendingActions();
  //## end ClCommunication::GetNPendingActions%967794014.body
}

m4return_t ClCommunication::SetNSocketPool (m4string_t &ai_strURL, m4int_t ai_iNSocket)
{
  //## begin ClCommunication::SetNSocketPool%971164983.body preserve=yes
	if(_AnyErrors())
		return M4_ERROR;
	return m_poImplementation->SetNSocketPool(ai_strURL,ai_iNSocket);
  //## end ClCommunication::SetNSocketPool%971164983.body
}

m4return_t ClCommunication::Write (m4handle_t ai_hConnectionID, m4uint32_t ai_iRequestID, ClPDU *ai_poNetObject, m4pchar_t ai_pcHTTPHeaders, m4pchar_t ai_pcActionName, ClExecutableInterface *ai_poActionExecutor)
{
  //## begin ClCommunication::Write%972554299.body preserve=yes
	if(_AnyErrors())
		return M4_ERROR;

	ClCommWriteAction *theAction = new ClCommWriteAction ();
	ClParamList *theParams;

	m4return_t iRet=m_poImplementation->VerifyConnection(ai_hConnectionID,theParams,theAction);
	if(M4_ERROR==iRet)
	{
		delete theAction;
		return M4_ERROR;
	}

	M4_ASSERT(!((M4_WARNING==iRet) && !ai_pcActionName));

	ClCommWrittingTask * theWTask=(ClCommWrittingTask *)(theAction->m_poTask);
	theWTask->SetPDU(ai_poNetObject );
	theWTask->SetHeaders(ai_pcHTTPHeaders);
	theWTask->SetRequestID(ai_iRequestID );

	return _PrepareReturn(ai_pcActionName,ai_poActionExecutor,theAction,theParams);
  //## end ClCommunication::Write%972554299.body
}

m4return_t ClCommunication::SetProtocolVersion (m4handle_t ai_hConnectionID, m4int_t ai_iProtocolVersion)
{
  //## begin ClCommunication::SetProtocolVersion%973247121.body preserve=yes
	if(_AnyErrors())
		return M4_ERROR;
	return m_poImplementation->SetProtocolVersion (ai_hConnectionID, ai_iProtocolVersion);
  //## end ClCommunication::SetProtocolVersion%973247121.body
}

m4return_t ClCommunication::GetCommParams (m4handle_t ai_hConnectionID, ClParamList &aio_oParamList)
{
  //## begin ClCommunication::GetCommParams%977486320.body preserve=yes
	if(_AnyErrors())
		return M4_ERROR;

	m4string_t strURL,strHostname;
	m4return_t iRet=m_poImplementation->GetURLAndHostName(ai_hConnectionID,strURL,strHostname);
	if(M4_ERROR==iRet)
	{
		m4Trace(cerr << "Error en Implementation::GetURLAndHostName"<<endl);
		return M4_ERROR;
	}
	aio_oParamList.SetString(M4_PARAM_COMM_URL,strURL.c_str());
	aio_oParamList.SetString(M4_PARAM_COMM_HOSTNAME,strHostname.c_str());

	return M4_SUCCESS;
  //## end ClCommunication::GetCommParams%977486320.body
}

m4return_t ClCommunication::StartShutdown ()
{
  //## begin ClCommunication::StartShutdown%988885055.body preserve=yes
	if(_AnyErrors())
		return M4_ERROR;

	m4return_t iRet;

	ClCommShutDownAction *theAction;

	theAction=new ClCommShutDownAction ;
	iRet=_PrepareReturn("VayaActionName",(ClExecutableInterface *)1,theAction,NULL);

	if(M4_SUCCESS!=iRet)
    {
		m4Trace(cerr << "Error haciendo shutdown de comunicaciones"<<endl);
    }

	return iRet;
//	return ClCommunicationImplementation::GetInstance()->StopCommunicationLayer();

  //## end ClCommunication::StartShutdown%988885055.body
}
m4bool_t ClCommunication::IsShuttedDown ()
{
  //## begin ClCommunication::IsShuttedDown%988885056.body preserve=yes
	if( ( ClCommunicationImplementation::GetInstance()==NULL) || (ClCommunicationImplementation::GetInstance()->IsShuttedDown()))
		return M4_TRUE;
	else
		return M4_FALSE;
  //## end ClCommunication::IsShuttedDown%988885056.body
}

m4bool_t ClCommunication::_AnyErrors ()
{
  //## begin ClCommunication::_AnyErrors%988885057.body preserve=yes
	if(GetError())
	{
		M4_SRV_ERROR_N(0,"Fatal error in ClCommunication");
		return M4_TRUE;
	}
	if(( m_poImplementation==NULL) || (m_poImplementation->IsShuttingDown()))
	{
		M4_SRV_ERROR_N(0,"ClCommunication shuttingdown");
		return M4_TRUE;
	}
	if(IsShuttedDown ())
	{
		M4_SRV_ERROR_N(0,"ClCommunication shutteddown");
		return M4_TRUE;
	}
	return M4_FALSE;
  //## end ClCommunication::_AnyErrors%988885057.body
}


m4return_t ClCommunication::SetCommsLanguage( m4language_t ai_iLang )
{
	m4return_t iRet = M4_ERROR ;

	ClParamList * theParamList=new ClParamList ;

	theParamList->SetInt( M4_PARAM_COMM_LANG_ID, (m4int_t)ai_iLang ) ;
	
	ClCommSetLangAction *theAction= new ClCommSetLangAction() ;

	theAction->SetParams( theParamList ) ;
	theAction->SetActionName( NULL ) ;

	iRet = m_poImplementation->Push( theAction ) ;

	return iRet ;
}

// Additional Declarations
  //## begin ClCommunication%38D1F37501A8.declarations preserve=yes
  //## end ClCommunication%38D1F37501A8.declarations

//## begin module%38F1F1C2020C.epilog preserve=yes
//## end module%38F1F1C2020C.epilog
