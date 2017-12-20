//## begin module%34795AAC007A.cm preserve=no
//## end module%34795AAC007A.cm

//## begin module%34795AAC007A.cp preserve=yes
//	=========================================================
//	=====================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              ClReceiverC.cpp
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
//	=========================================================
//	=====================
//## end module%34795AAC007A.cp

//## Module: clientreceiver%34795AAC007A; Package body
//## Subsystem: M4AppClient::src%3796F8B303AD
//## Source file: e:\source\m4appclient\src\clientreceiver.cpp

//## begin module%34795AAC007A.additionalIncludes preserve=no
//## end module%34795AAC007A.additionalIncludes

//## begin module%34795AAC007A.includes preserve=yes
#include <http_errors.hpp>

#ifdef PDUSTATISTIC
#include "pdusize.h"
#endif // PDUSTATISTIC
#include <clientmonitor.hpp>
//## end module%34795AAC007A.includes

// clientbasicdef
#include <clientbasicdef.hpp>
// eventdisplayer
#include <eventdisplayer.hpp>
// pduherram
#include <pduherram.hpp>
// clientreceiverq
#include <clientreceiverq.hpp>
// clientreceiver
#include <clientreceiver.hpp>
// CSUtil
#include <csutil.hpp>
// syncro
#include <syncro.hpp>
// tsap
#include <tsap.hpp>
// servicepdus
#include <servicepdus.hpp>
// m4eventhandlers
#include <m4eventhandlers.hpp>

class ClCCConnection;

//## begin module%34795AAC007A.declarations preserve=no
//## end module%34795AAC007A.declarations

//## begin module%34795AAC007A.additionalDeclarations preserve=yes
//## end module%34795AAC007A.additionalDeclarations


// Class ClCCReceiver 









ClCCReceiver::ClCCReceiver (ClCCReceiverQ* ai_pReceiverQueue)
  //## begin ClCCReceiver::ClCCReceiver%879959079.hasinit preserve=no
      : m_iNumeroTSAP(0)
  //## end ClCCReceiver::ClCCReceiver%879959079.hasinit
  //## begin ClCCReceiver::ClCCReceiver%879959079.initialization preserve=yes
  //## end ClCCReceiver::ClCCReceiver%879959079.initialization
{
  //## begin ClCCReceiver::ClCCReceiver%879959079.body preserve=yes
	
	m_pClCCReceiverQ = ai_pReceiverQueue;

//	m_pSemaphore   = new ClSemaphore(1,1);
	m_pMutex = new ClMutex;
	if(M4_ERROR == m_pMutex->Init())
	{m4Trace(cerr<<"Error in Receiver Mutex Init"<<endl);}
	m_pTSAPSelect   = new ClTSAPSelect();

  //## end ClCCReceiver::ClCCReceiver%879959079.body
}


ClCCReceiver::~ClCCReceiver ()
{
  //## begin ClCCReceiver::~ClCCReceiver%880965773.body preserve=yes

	delete m_pTSAPSelect;
	m_pTSAPSelect = NULL;

	delete m_pMutex;
	m_pMutex=NULL;
  //## end ClCCReceiver::~ClCCReceiver%880965773.body
}



//## Other Operations (implementation)
m4return_t ClCCReceiver::AddConnection (ClCCConnection* ai_pConnection)
{
  //## begin ClCCReceiver::AddConnection%880965774.body preserve=yes
	if(M4_CLASSIC_COMMS_TYPE!=ClCCMonitor::GetCommsType())
		return M4_SUCCESS;
	m4return_t iRet;
	m_pMutex ->Lock();

	ClTSAP* pTSAP;
	iRet=ai_pConnection -> GetTSAP(pTSAP);
	if( (M4_ERROR!=iRet) && (pTSAP))
	{

		iRet=AddTSAP(pTSAP);
		if(iRet!=M4_SUCCESS)
		{
			m_pMutex-> Unlock();
			return iRet;
		}
		m_dicTSAP[(m4handle_t)pTSAP]=ai_pConnection;
		m_pMutex-> Unlock();
		return M4_SUCCESS;
	}
	else
	{
		m_pMutex -> Unlock();
		return M4_ERROR;
	}
  //## end ClCCReceiver::AddConnection%880965774.body
}

m4return_t ClCCReceiver::RemoveConnection (ClCCConnection* ai_pConnection)
{
  //## begin ClCCReceiver::RemoveConnection%881084615.body preserve=yes
	if(M4_CLASSIC_COMMS_TYPE!=ClCCMonitor::GetCommsType())
		return M4_SUCCESS;
	
	return FullRemoveConn(ai_pConnection);
  //## end ClCCReceiver::RemoveConnection%881084615.body
}

void ClCCReceiver::Run ()
{
  //## begin ClCCReceiver::Run%879852434.body preserve=yes

	m4int16_t  iAuxNumeroTSAP;
	ClTSAP*    pTSAP;
	ClPDU*     pPDU;
	m4return_t iRet;
	m_oCondEmptyTSAP.Init();
	
	map<m4handle_t, ClCCConnection *, less<m4handle_t> >::iterator iteradorTSAP;
	
	ClCCMonitor* theMonitor=NULL;
	iRet=ClCCMonitor::GetClientMonitor(theMonitor);
	while(!theMonitor)
	{
		m4TraceLevel(2,cerr<<"Monitor not responding.Still trying"<<endl)
			m4sleep(1);
		iRet=ClCCMonitor::GetClientMonitor(theMonitor);
	}
	
	ClCCReceiverQ* TheReceiverQ;
	TheReceiverQ=theMonitor -> GetReceiverQueue();
	m4pchar_t pcBuffer=NULL;

	m4char_t cConnType;
	while ( !IsTerminate() ) 
	{
		m_pMutex -> Lock();
		iAuxNumeroTSAP = m_iNumeroTSAP;
		m_pMutex -> Unlock();
		
		if (iAuxNumeroTSAP != 0)
		{
			// Se hace un select con timeout para evitar que se 
			// quede dormido esperando que lleguen datos, y asi 
			// nos aseguramos de que siempre se hace un select 
			// despues de un add o de un remove.
			m_pMutex ->Lock();
			iRet=m_pTSAPSelect -> Select(1L, 0L, M4_TRUE, M4_FALSE, M4_FALSE);
			//			m_pMutex -> Unlock();
			if (iRet==-1)
			{
				m_pMutex -> Unlock();
				m4sleep(1);
				m4Trace(cerr <<"TSAP->Select ha devuelto -1"<<endl);
#ifndef _UNIX
				m4Trace(cerr <<":WSAGetLastError:"<<WSAGetLastError()<<endl);
#endif 
			}
			else
			{
				//				m_pMutex -> Lock();
				pTSAP = m_pTSAPSelect -> NextRead();
				m_pMutex -> Unlock();
				
				while ( pTSAP != NULL )
				{
					// Me creo una factoría de PDU dandole el TSAP y el protocolo
					ClCCConnection * pConnection=NULL;	
					m_pMutex->Lock();
					iteradorTSAP=m_dicTSAP.find((m4handle_t)pTSAP);
					if(iteradorTSAP==m_dicTSAP.end())
					{
						SETCODE(M4_ERR_TSAPNOTFOUND_IN_MAP,ERRORLOG,"TSAP not found in map.");
						m4TraceLevel(2,cerr <<"TSAP already deleted\n");
						m_pMutex->Unlock();
						break;
					}
					else
					{
						pConnection=(*iteradorTSAP).second;
						cConnType=pConnection->GetConnType();
					}
					m_pMutex->Unlock();
					if(cConnType)
					{
						ClCCConnectionHTTP * pConnectionHTTP=(ClCCConnectionHTTP *)pConnection;
						if(pConnectionHTTP->GetConnStatus()==M4_CONNECTION_CANCELING)
						{
							m_pMutex->Lock();
							iRet=_FullRemoveTSAP(pConnection);
							m_pMutex ->Unlock();
						}
					}
					m4bool_t bMustReSend;
					iRet=pConnection->_GetAnswerPDU(pPDU, bMustReSend);

					switch(iRet)
					{
					case M4_SUCCESS:
					{
							if(pConnection->GetConnStatus()==M4_CONNECTION_FINISHING)
							{
								m4TraceLevel(2,cout<<"Connection Finishing. TSAP removed from Select"<<endl);
								m_pMutex -> Lock();
								iRet=_FullRemoveTSAP(pConnection);
								m_pMutex -> Unlock();
							}
							TheReceiverQ -> Push(pPDU);
						}
						break;
						// TIMEOUT DE HTTP 
					case M4_ERROR_HTTP_301 :
						m4Trace(cerr<<"Timeout de inactividad"<<endl);
						break;
					case M4_ERROR_HTTP_307 :
						m4Trace(cerr<<"Timeout de actividad"<<endl);
						break;
					case M4_ERROR_HTTP_PARSE_ERROR:
						SETCODE(M4_ERR_CONNECTION_ERROR,ERRORLOG,"HTTP Parse Internal ERROR");
						m4Trace(cerr<<"HTTP_PARSE_ERROR"<<endl);
						break;
						// ERRORES DE CONEXION CON EL SERVER
					case M4_ERROR_HTTP_306 :
						m4Trace(cerr<<"Error in comunication with server"<<endl);
						break;
					case M4_ERROR_HTTP_303 :
						m4Trace(cerr<<"Can't create TCPIP connection to server"<<endl);
						break;
						// INTERNAL CODE ERROR
					case M4_ERROR_HTTP_302 :
						m4Trace(cerr<<"INTERNAL CODE ERROR.Invalid Command in header"<<endl);
						break;
					case M4_ERROR_HTTP_304 :
						m4Trace(cerr<<"INTERNAL CODE ERROR.Request length error"<<endl);
						break;
					case M4_ERROR_HTTP_305 :
						m4Trace(cerr<<"INTERNAL CODE ERROR.Wrong size.No puede ser menor de 16"<<endl);
						break;
					case M4_ERROR_HTTP_308 :
						m4Trace(cerr<< "INTERNAL CODE ERROR.HTTP header error" <<endl);
						break;
					case M4_ERROR_HTTP_309 :
						m4Trace(cerr<<"INTERNAL CODE ERROR.Error in request"<<endl);
						break;
					case HTTP_PARSE_BUFFER_SIZE_ERROR:
						// No se que hacer
						SETCODE(M4_ERR_CONNECTION_ERROR,ERRORLOG,"INTERNAL CODE ERROR.HTTP_PARSE_BUFFER_SIZE_ERROR");
						m4Trace(cerr<<"INTERNAL CODE ERROR.HTTP_PARSE_BUFFER_SIZE_ERROR!!!!"<<endl);
						break;
					case M4_ERROR_HTTP_310:
					case M4_ERROR_HTTP_NO_COOKIE:
						m4Trace(cerr<<"Error in private key!!"<<endl);
						break;
					case M4_ERROR_HTTP_312:
						m4Trace(cerr<<"INTERNAL HTTP ERROR."<<endl);
						break;
					case HTTP_PARSE_HTTP_ERROR:
						SETCODE(M4_ERR_CONNECTION_ERROR,ERRORLOG,"Couldn't connect to HTTP server");
						// Es un error HTTP, no del servlet.
						//m4Trace(cerr<<"Couldn't connec to to HTTP server"<<endl);
						//m4Trace(cerr<<"HTTP_PARSE_HTTP_ERROR"<<endl);
						m_pMutex -> Lock();
						iRet=_FullRemoveTSAP(pConnection);
						m_pMutex -> Unlock();
						break;
					case M4_ERROR:
						{
							// En este caso no se que hacer para recuperar la conexion y la elimino
							eConnectionStatus eStatusTSAP=pConnection->GetTSAPStatus();
#ifdef TEST_CLOSE_TSAP
							if((eStatusTSAP!=M4_CONNECTION_CLOSING_TSAP)||(pConnection->GetConnStatus()!=M4_CONNECTION_CLOSING)||(pConnection->GetConnStatus()!=M4_CONNECTION_FINISHING))
#else
							if((eStatusTSAP!=M4_CONNECTION_CLOSING_TSAP) && (eStatusTSAP!=M4_CONNECTION_NOTSAP))
#endif // TEST_CLOSE_TSAP
							{
								if( (eStatusTSAP==M4_CONNECTION_CANCELING) || (eStatusTSAP==M4_CONNECTION_RECONNECTING) )
								{
									m_pMutex->Lock();
									iRet=_FullRemoveTSAP(pConnection);
									m_pMutex ->Unlock();
									pConnection->SetTSAPStatus(M4_CONNECTION_NOTSAP);
									pConnection->SetConnStatus(M4_CONNECTION_NOTSAP);
									break;
								}
								else
								{
									ClServerEventFunction pServerEventFunction;
									theMonitor->m_poDistributor->GetServerEventFunction(pServerEventFunction);
																		
									if(pServerEventFunction)
									{
										
										m4Trace(cerr << "Calling BrokenConnFunction"<<endl);
										m4char_t buffer[2048];

										if((!pConnection->m_strHost.empty()) && (2048>pConnection->m_strHost.size()+10))
										{
											sprintf(buffer,"%s:%d",pConnection->m_strHost.c_str(),pConnection->m_iHostPort);
#ifdef _WINDOWS
											//Lanza un thread para simular que la ventana que se abre es modal
											ClServerEventDisplayer * poDisplayer = new ClServerEventDisplayer ( pServerEventFunction , 2 , buffer ) ;
											poDisplayer-> Start () ;
#else
											cout <<"Broken connection: "<< buffer<<endl;
#endif
										}
									}
									m4Trace(cerr <<"Detectado Corte de la comunicacion con el servidor"<<endl);
#ifndef _UNIX
									m4Trace(cerr <<"WSAGetLastError:"<<WSAGetLastError()<<endl);
#endif 
									SETCODE(M4_ERR_CONNECTION_ERROR,ERRORLOG,"Connection interrupted.");
									m_pMutex -> Lock();
									iRet=_FullRemoveTSAP(pConnection);
									m_pMutex -> Unlock();
									pConnection->NotifyIncidence(BROKEN_CONNECTION);
								}
							}
							else
							{m4Trace(cerr<<"se ha detectado un error y estamos en M4_CONNECTION_CLOSING_TSAP"<<endl);}
						}
						break;
					case M4_CANCELING_HTTP_REQUEST:
						m4Trace(cerr<< "Canceling Request"<<endl);
						break;
					default:
						m4Trace(cerr<<"Caso no controlado!!!!:"<<(m4int_t)iRet<<endl);
						break;
					}
					pTSAP=NULL;
					m_pMutex -> Lock();
					pTSAP = m_pTSAPSelect -> NextRead();
					m_pMutex -> Unlock();
				} // while hasta que todas las TSAP hayan salido del select
			} //Si la select no devuelve error
		}
		else
			m_oCondEmptyTSAP.Wait();
	}

  //## end ClCCReceiver::Run%879852434.body
}

m4return_t ClCCReceiver::AddTSAP (ClTSAP *ai_pTSAP)
{
  //## begin ClCCReceiver::AddTSAP%922371775.body preserve=yes
	
	m4return_t iRet=M4_SUCCESS;

	m_pTSAPSelect -> Add(ai_pTSAP, M4_TRUE, M4_FALSE, M4_FALSE);

	m_iNumeroTSAP++;

	if(1==m_iNumeroTSAP)
		m_oCondEmptyTSAP.Signal();

	return iRet;
  //## end ClCCReceiver::AddTSAP%922371775.body
}

m4return_t ClCCReceiver::RemoveTSAP (ClTSAP *ai_pTSAP)
{
  //## begin ClCCReceiver::RemoveTSAP%922371776.body preserve=yes
	
	m_pTSAPSelect -> Remove(ai_pTSAP);

	if(m_iNumeroTSAP>0)
	{
		m_iNumeroTSAP--;
	}
	else
	{
		m4Trace(cerr<<"Numero de TSAP en Select " <<m_iNumeroTSAP<<endl);
		return M4_ERROR;
	}

	return M4_SUCCESS;
  //## end ClCCReceiver::RemoveTSAP%922371776.body
}

m4return_t ClCCReceiver::FullRemoveConn (ClCCConnection* ai_pConnection)
{
  //## begin ClCCReceiver::FullRemoveConn%948876482.body preserve=yes
	
	if(!ai_pConnection)
		return  M4_ERROR;

	m4return_t iRet;
	iRet=m_pMutex -> Lock();
	if(M4_ERROR==iRet)
		return iRet;
	
/*	if(ai_pConnection->GetConnType())
	{
		m4TraceLevel(2,cout<<"HTTP connection. TSAP is NULL"<<endl);
		m_pMutex->Unlock();
		return M4_SUCCESS;
	}
*/
	iRet=_FullRemoveTSAP(ai_pConnection);

	m_pMutex -> Unlock();

	return M4_SUCCESS;
  //## end ClCCReceiver::FullRemoveConn%948876482.body
}

m4return_t ClCCReceiver::_FullRemoveTSAP (ClCCConnection *ai_pConnection)
{
  //## begin ClCCReceiver::_FullRemoveTSAP%951384407.body preserve=yes
	m4return_t iRet;

	ClTSAP* pTSAP;
	iRet=ai_pConnection -> GetTSAP(pTSAP);
	if(!pTSAP  || (M4_ERROR==iRet))
		return  M4_ERROR;

	m4int32_t borrado=m_dicTSAP.erase((m4handle_t)pTSAP);
	if (!borrado) 
	{
		m4TraceLevel(2,cerr<<"TSAP already deleted from receiver map\n");
	}
	else
		iRet=RemoveTSAP(pTSAP);
	return iRet;
  //## end ClCCReceiver::_FullRemoveTSAP%951384407.body
}

// Additional Declarations
  //## begin ClCCReceiver%3447381B00B1.declarations preserve=yes
  //## end ClCCReceiver%3447381B00B1.declarations

// Class ClCCDistributor 




ClCCDistributor::ClCCDistributor (ClCCReceiverQ* ai_pReceiverQueue)
  //## begin ClCCDistributor::ClCCDistributor%881686003.hasinit preserve=no
  //## end ClCCDistributor::ClCCDistributor%881686003.hasinit
  //## begin ClCCDistributor::ClCCDistributor%881686003.initialization preserve=yes
  : m_iPDUCount(0)
  //## end ClCCDistributor::ClCCDistributor%881686003.initialization
{
  //## begin ClCCDistributor::ClCCDistributor%881686003.body preserve=yes
 
	m_pClCCReceiverQ = ai_pReceiverQueue;
	m_pEventFunction=NULL;

  //## end ClCCDistributor::ClCCDistributor%881686003.body
}


ClCCDistributor::~ClCCDistributor ()
{
  //## begin ClCCDistributor::~ClCCDistributor%881686004.body preserve=yes
  //## end ClCCDistributor::~ClCCDistributor%881686004.body
}



//## Other Operations (implementation)
ClPDU* ClCCDistributor::GetPDUFromQueue ()
{
  //## begin ClCCDistributor::GetPDUFromQueue%881686005.body preserve=yes

	ClPDU* pPDU = NULL;
	m_pClCCReceiverQ -> Pop(pPDU);

//	M4_ASSERT(pPDU != NULL); //  Veremos si es NULL en el Run del Distributor

	return pPDU;

  //## end ClCCDistributor::GetPDUFromQueue%881686005.body
}

void ClCCDistributor::Run ()
{
  //## begin ClCCDistributor::Run%881686006.body preserve=yes

	while ( !IsTerminate() ) 
	{
		ClPDU* pPDU = GetPDUFromQueue();
		if (pPDU != NULL)
		{
			m4return_t iRet;
			iRet=ReturnRequest(pPDU);
		}		
		else
		{
			m4Trace(cerr <<"NULL PDU in receiver from queue");
			SETCODE(M4_ERR_NULL_PDU_FROM_QUEUE,ERRORLOG,"NULL PDU in receiver from queue");
		}
	}

  //## end ClCCDistributor::Run%881686006.body
}

m4return_t ClCCDistributor::ReturnRequest (ClPDU* ai_pPDU)
{
  //## begin ClCCDistributor::ReturnRequest%881742134.body preserve=yes

	m4int32_t	requestId;

	requestId = ((ClRequestPDU *)ai_pPDU) -> GetRequestId();

	if ( ! requestId )
	{
		return GetEvent(ai_pPDU);
	}


	m4uint16_t pduType = ai_pPDU -> GetTypePDU();
	m4uint16_t pduId = ai_pPDU -> GetIdPDU();


	ClCCMonitor		*elMonitor;
	ClCCMonitor::GetClientMonitor(elMonitor);

	ClCCRequest* pRequest;
	m4return_t ret = elMonitor-> GetRequest(requestId, pRequest);

	if (ret != M4_SUCCESS) 
	{
		SETCODE(M4_ERR_GETREQ_REQNOTFOUND,ERRORLOG,"Nobody waiting for this request.");
		m4Trace(cerr << "Nobody waiting for this request.\n");
		if (ai_pPDU != NULL)
			delete ai_pPDU;
		return M4_ERROR;
	}

	eRequestStatus iStatusNow=pRequest -> GetState();
	for(m4uint32_t counter=0;iStatusNow==SENDING && counter<5;counter++)
	{
		m4Trace(cerr<<"Still Sending"<<endl);
		m4sleep(1);
		iStatusNow=pRequest -> GetState();
	}
	
	ClCondition	*pCond = pRequest -> GetCondition();
	
	if (M4_TYPE_PDU_CONTROL != pduType)
	{
		m_iPDUCount++;
		m4Trace(cout << "(+)Request (" << ( (ClReplyPDU*) ai_pPDU ) -> GetServiceId());
		m4Trace(cout << ", " << ( (ClReplyPDU*) ai_pPDU ) -> GetRequestId());
		m4Trace(cout << ", " << m_iPDUCount << "): ");
		if(pduId==M4_ID_PDU_ERROR)
		{
			m4Trace(cout << "Error in ServicePDU" << endl);
			pRequest -> SetState(ERROR_ARRIVED);
		}			
		else
			pRequest -> SetState(REPLY_ARRIVED);

		if (pCond != NULL)
		{
			pRequest -> SetPDUOutput(ai_pPDU);
			pCond -> Signal();
			m4Trace(cout << "Sync" << endl);
		}
		else 
		{
			m4Trace(cout << "Async" << endl);
		}
	}
	else	// Es de control
	{
		if(NULL == pCond)
		{
			m4Trace(cerr << "pCond == NULL en PDU de control" <<endl);
			SETCODE(M4_ERR_PCOND_NULL_IN_CONTROL_PDU,ERRORLOG,"pCond == NULL in Control PDU.");

			switch(pduId)
			{
			case M4_ID_PDU_ACK:
				m4Trace(cout << "OK !" << endl);
				pRequest -> SetState(REPLY_ARRIVED);
				break;

			case M4_ID_PDU_ERROR:
				m4Trace(cout << "Error!" << endl);
				pRequest -> SetState(ERROR_ARRIVED);
				break;
			default:
				m4Trace(cerr << "ERROR he llegado a default con " << pduId <<"\n");
				break;
			}
			pRequest -> SetPDUOutput(ai_pPDU);
		}
		else
		{
			m4uint32_t counter=0;
			switch(pduId)
			{
			case M4_ID_PDU_ACK:
				switch(pRequest->GetState())
				{
					case SENDING :
						for( counter=0;pRequest->GetState()==SENDING && counter<5;counter++)
						{
							m4Trace(cerr<<"Still Sending"<<endl);
							m4sleep(1);
						}
						break;
					default:
						break;
				}
				m4Trace(cout << "OK !" << endl);
				pRequest -> SetState(REPLY_ARRIVED);
				break;
			case M4_ID_PDU_ERROR:
				m4Trace(cout << "Error!" << endl);
				pRequest -> SetState(ERROR_ARRIVED);
				break;
			default:
				m4Trace(cerr << "ERROR he llegado a default con " << pduId <<"\n");
				break;
			}
			pRequest -> SetPDUOutput(ai_pPDU);
			pCond -> Signal();
		}
	}


	return M4_SUCCESS;

  //## end ClCCDistributor::ReturnRequest%881742134.body
}

m4return_t ClCCDistributor::SetServerEventFunction (ClServerEventFunction ai_EventFunction)
{
  //## begin ClCCDistributor::SetServerEventFunction%929691987.body preserve=yes
	m4return_t iRet=M4_SUCCESS;

	if(m_pEventFunction)
		iRet=M4_WARNING;

	m_pEventFunction=ai_EventFunction;
		return iRet;
  //## end ClCCDistributor::SetServerEventFunction%929691987.body
}

m4return_t ClCCDistributor::GetServerEventFunction (ClServerEventFunction &ao_EventFunction)
{
  //## begin ClCCDistributor::GetServerEventFunction%941097607.body preserve=yes
	m4return_t iRet=M4_SUCCESS;
	if(!m_pEventFunction)
	{
		iRet=M4_ERROR;
		ao_EventFunction=NULL;
	}

	ao_EventFunction=m_pEventFunction;
	return iRet;
  //## end ClCCDistributor::GetServerEventFunction%941097607.body
}

m4return_t ClCCDistributor::GetEvent (ClPDU *ai_pPDU)
{
  //## begin ClCCDistributor::GetEvent%948366550.body preserve=yes
		M4DataStorage * poInputDataStorage;
		ClDataUnit     *poDataUnit;
		//Si el ID de request es 0 se trata de una PDU de Evento del server
		poInputDataStorage = ai_pPDU ->GetDataStorage();
		
		poInputDataStorage ->InitializeIteration();//Esto es para empezar a leer 
		//del data storage de entrada
		if (poInputDataStorage->GetNext((ClDataUnitInterface*&)poDataUnit)==M4_SUCCESS )
		{
			m4pchar_t buffer=NULL;
			m4int_t size=0;
			if ((buffer=poDataUnit->GetBuffer())!=NULL)
			{
				size=poDataUnit->GetSize();
				if((size>9) && (!strncmp(buffer,"@@EVENT@@",9)))
				{
					m4Trace(cout<< "EVENTOS!!!!!"<<endl);
					
					IEventHandler *theEH=ClLogBasedEventHandler::GetEHInstance();
					if(theEH)
						((ClLogBasedEventHandler*)theEH)->Deserialize(buffer+9,size-9);
#ifdef _DEBUG
					ClEventMsgHistory theMsgHis;
					theMsgHis.Deserialize(buffer+9,size);
					m4int_t nEvents=theMsgHis.GetSize();
					if(nEvents)
					{
						m4Trace(cout << "Han llegado "<<nEvents<<" eventos" <<endl);
					}
#endif
				}
				else
				{
					SETCODEF ( M4_ERR_CONNECTION_ERROR , DEBUGINFOLOG , "Server Shuting down in %s minutes" , buffer  ) ;
					
					if ( m_pEventFunction )		//Si se ha asignado la función para 
					{							//mostrar eventos recibidos del server, se procesan
#ifdef _WINDOWS
						//Lanza un thread para simular que la ventana que se abre es modal
						ClServerEventDisplayer * poDisplayer = new ClServerEventDisplayer ( m_pEventFunction , buffer [0] , buffer +1 ) ;
						poDisplayer-> Start () ;
#else
						cout <<"Server Shuting down in "<< buffer<<  " minutes"<<endl;
#endif
					}
				}
			
			}
		}
	return M4_SUCCESS;
  //## end ClCCDistributor::GetEvent%948366550.body
}

// Additional Declarations
  //## begin ClCCDistributor%348DBC810140.declarations preserve=yes
  //## end ClCCDistributor%348DBC810140.declarations

//## begin module%34795AAC007A.epilog preserve=yes
//## end module%34795AAC007A.epilog
