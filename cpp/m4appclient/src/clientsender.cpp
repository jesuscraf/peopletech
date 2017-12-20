//## begin module%34795A450085.cm preserve=no
//## end module%34795A450085.cm

//## begin module%34795A450085.cp preserve=yes
//	=========================================================
//	=====================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              ClSenderC.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:		ClCCSender
//
//	    This module defines...
//
//
//	=========================================================
//	=====================
//## end module%34795A450085.cp

//## Module: clientsender%34795A450085; Package body
//## Subsystem: M4AppClient::src%3796F8B303AD
//## Source file: e:\source\m4appclient\src\clientsender.cpp

//## begin module%34795A450085.additionalIncludes preserve=no
//## end module%34795A450085.additionalIncludes

//## begin module%34795A450085.includes preserve=yes
#ifdef   PDUSTATISTIC
#include "pdusize.h"
#endif // PDUSTATISTIC
#include <clientservice.hpp>
//## end module%34795A450085.includes

// clientconnection
#include <clientconnection.hpp>
// clientsenderq
#include <clientsenderq.hpp>
// clientsender
#include <clientsender.hpp>
// clientsession
#include <clientsession.hpp>
// CSUtil
#include <csutil.hpp>
// tsap
#include <tsap.hpp>
//## begin module%34795A450085.declarations preserve=no
//## end module%34795A450085.declarations

//## begin module%34795A450085.additionalDeclarations preserve=yes
class ClCCSession;
#define M4_MAX_CHUNK_SIZE 1000000
//## end module%34795A450085.additionalDeclarations


// Class ClCCSender 





ClCCSender::ClCCSender (ClCCSendQ* ai_pRequestQueue)
  //## begin ClCCSender::ClCCSender%880446326.hasinit preserve=no
      : m_iCount(0), m_iRequestCount(0)
  //## end ClCCSender::ClCCSender%880446326.hasinit
  //## begin ClCCSender::ClCCSender%880446326.initialization preserve=yes
  //## end ClCCSender::ClCCSender%880446326.initialization
{
  //## begin ClCCSender::ClCCSender%880446326.body preserve=yes
	m_pSenderQ = ai_pRequestQueue;
  //## end ClCCSender::ClCCSender%880446326.body
}


ClCCSender::~ClCCSender ()
{
  //## begin ClCCSender::~ClCCSender%880368623.body preserve=yes
  //## end ClCCSender::~ClCCSender%880368623.body
}



//## Other Operations (implementation)
void ClCCSender::Run ()
{
  //## begin ClCCSender::Run%879852436.body preserve=yes
	m4bool_t bBroken;
	ClCCRequest* pRequest;
	ClCCSession * pSession;
	ClCCConnection	*pConnection;
	ClCCService		*pService;
	ClPDU*  pPDU ; 
	m4return_t iRet;
	eRequestStatus eRState;

	while( !IsTerminate() )
	{
		pRequest = GetRequestFromQueue();
		bBroken=M4_FALSE;
		
		if (pRequest == NULL)
		{
			m4Trace(cerr <<"NULL Request from Sender queue." <<endl);
			SETCODE(M4_ERR_NULL_REQ_FROM_QUEUE,ERRORLOG,"NULL Request from Sender queue.");
		}
		else
		{
			eRState=pRequest->GetState();
			if(SENDING==eRState)
			{
				pPDU= pRequest -> GetInputPDU();
				if (!pPDU)
				{
					m4Trace(cerr <<"NULL PDU from Sender queue." <<endl);
					SETCODE(M4_ERR_NULL_PDU_FROM_QUEUE,ERRORLOG,"NULL PDU from Sender queue.");
					bBroken=M4_TRUE;
				}
				else
				{
					iRet=pRequest -> GetService(pService); //Siempre devuelven M4_SUCCESS
					pService->GetSession(pSession);
					pSession->GetConnection(pConnection);

					iRet = pConnection ->_SendRawData(pRequest);
					if (M4_ERROR==iRet)
					{
						m4Trace(cerr <<"Error sending PDU" <<endl);
						SETCODE(M4_ERR_PDU_ERROR,ERRORLOG,"Error sending PDU");
						bBroken=M4_TRUE;
					}
					else
						pRequest -> SetState(WAITING_FOR_REPLY);
#ifdef _DEBUG		
					if(!bBroken)
					{

						m4uint16_t pduType = pPDU -> GetTypePDU();
						
						if (M4_TYPE_PDU_CONTROL != pduType)
						{
							m4pchar_t pService ;
							((ClRequestPDU*) pPDU ) -> GetServiceId(pService) ;
							m4Trace(cout << "(-)Request (" << pService << ", " << (  (ClRequestPDU*) pPDU ) -> GetRequestId() << ")"<< endl);
						}
						else
						{
							m4Trace(cout << endl << "Sending Command..." << flush );
						}
					}
#endif
				}
			}
			else
			{
				if(pRequest -> GetState()==CANCELING)
					pRequest -> SetState(BROKEN_CONNECTION);
				else
					m4Trace(cerr<<"Request in state "<< eRState<<endl);
			}
		}
		if(bBroken)
		{
				ClCondition	*pCond = pRequest -> GetCondition();
				pRequest -> SetState(BROKEN_CONNECTION);
				if (pCond != NULL)
					pCond -> Signal();
		}
	}
  //## end ClCCSender::Run%879852436.body
}

m4return_t ClCCSender::SendPDU (ClCCRequest *ai_pRequest, ClTSAP* ai_pTSAP)
{
  //## begin ClCCSender::SendPDU%880368624.body preserve=yes
	M4_ASSERT(1==2);
	return M4_ERROR;
  //## end ClCCSender::SendPDU%880368624.body
}

ClCCRequest* ClCCSender::GetRequestFromQueue ()
{
  //## begin ClCCSender::GetRequestFromQueue%880368622.body preserve=yes

	ClCCRequest* pRequest = NULL;
	m_pSenderQ -> Pop(pRequest);

	if (pRequest == NULL)
	{
		m4Trace(cerr <<"NULL Request from Sender queue." <<endl);
		SETCODE(M4_ERR_NULL_REQ_FROM_QUEUE,ERRORLOG,"NULL Request from Sender queue.");
	}

	return pRequest;

  //## end ClCCSender::GetRequestFromQueue%880368622.body
}

// Additional Declarations
  //## begin ClCCSender%344737DF03AD.declarations preserve=yes
  //## end ClCCSender%344737DF03AD.declarations

//## begin module%34795A450085.epilog preserve=yes
//## end module%34795A450085.epilog
