//## begin module.cm preserve=no
//## end module.cm

//## begin module.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
// 
//	 Module:            
//	 File:              cladministrator.cpp
//	 Project:           m4adminsrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines the administrator interface in the
//		server side.
//
//	==============================================================================
//## end module.cp

//## Module: ClAdministrator; Package body
//## Subsystem: CPM4AdminSrv
//## Source file: d:\marcoscp\M4AdminSrv\version\src\cladministrator.cpp

//## begin module.additionalIncludes preserve=no
//## end module.additionalIncludes

//## begin module.includes preserve=yes
#include "controlpdus.hpp"
#include "m4trace.hpp"
#include "m4log.hpp"
#include <clssinterfaz.hpp>
//## end module.includes

// ClAdministrator
#include <cladministrator.hpp>
//## begin module.declarations preserve=no
//## end module.declarations

//## begin module.additionalDeclarations preserve=yes
//## end module.additionalDeclarations


// Class ClCSAdministrator 











ClCSAdministrator::ClCSAdministrator (ClTSAP *ai_poTSAP, ClNavigatorFirewall *ai_poNavigator, ClSystemInterface *ai_poSystem)
  //## begin ClCSAdministrator::ClCSAdministrator%878114676.hasinit preserve=no
      : m_poLogonInterface(NULL), m_poUSSessionInterface(NULL)
  //## end ClCSAdministrator::ClCSAdministrator%878114676.hasinit
  //## begin ClCSAdministrator::ClCSAdministrator%878114676.initialization preserve=yes
      ,m_poNavigator(ai_poNavigator)
  //## end ClCSAdministrator::ClCSAdministrator%878114676.initialization
{
  //## begin ClCSAdministrator::ClCSAdministrator%878114676.body preserve=yes
	m4AutoTraceLevel(5, "ClCSAdministrator::ClCSAdministrator");

	m_poTSAP = ai_poTSAP ;

	//
	// the parameter ai_poSystem is obsolete;
	m_poSystem = ai_poSystem;

	//
	// set the administrator role.
	m_Role = M4_DEFAULT_ADMINISTRATOR_ROLE;

	//
	// initialize the logon interface
	if (m_poNavigator != NULL)
	{
		m_poLogonInterface =
			(ClLogonInterface *)m_poNavigator->GetSubsystemElement("USERS", "LOGON_INTERFACE");
	}
	else
	{
		m4TraceHeaderLevel(1, cout << "NULL = m_poNavigator" << endl);
		m_poLogonInterface=NULL; 
	}

	IEventHandler * TheEH=NULL;
	TheEH=ClLogBasedEventHandler::GetEHInstance();
	if(TheEH)
		TheEH->SubscribeTopic(*this,M4AdminEventTopic);
  //## end ClCSAdministrator::ClCSAdministrator%878114676.body
}


ClCSAdministrator::~ClCSAdministrator ()
{
  //## begin ClCSAdministrator::~ClCSAdministrator%878114677.body preserve=yes
	m4AutoTraceLevel(5, "ClCSAdministrator::~ClCSAdministrator");

	// Auditoría de sesión
	// Se llama a cancelar la sesión si la sesión está auditada
	// Debe ir aquí antes de que se borre el usuario y la sesión
	if( m_poUSSessionInterface != NULL && m_poUSSessionInterface->GetAuditSession() == M4_TRUE )
	{
		if( m_poLogonInterface != NULL )
		{
			m4return_t	iResult = m_poLogonInterface->CloseAuditory( m_poUSSessionInterface ) ;
		}
	}

	//ADD OSCAR 17-12-999
	//Se pone un retardo para evitar que se corte la conexión antes de que 
	//llegue la respuesta al cliente
	m4sleep(2) ;
	// END OSCAR 17-12-999
//	m_poTSAP -> Close () ;
//	delete m_poTSAP ;

	if ( m_poUSSessionInterface )
	{
		ClUSUserInterface * poUser = m_poUSSessionInterface->GetUser () ;
		if ( poUser )
		{
			if ( poUser -> GetStats () )
			{
				delete poUser -> GetStats () ;
			}
			delete  poUser ;
		}

		if ( m_poUSSessionInterface -> GetStats () )
		{
			delete m_poUSSessionInterface -> GetStats () ;
		}

		delete m_poUSSessionInterface ;
	}

	if ( m_poLogonInterface )
	{
		delete m_poLogonInterface ;

		m_poLogonInterface = NULL ;
	}

	if ( m_poNavigator )
	{
		delete m_poNavigator ;

		m_poNavigator = NULL ;
	}

  //## end ClCSAdministrator::~ClCSAdministrator%878114677.body
}



//## Other Operations (implementation)
void ClCSAdministrator::Run ()
{
  //## begin ClCSAdministrator::Run%878114678.body preserve=yes
	m4AutoTraceLevel(5, "ClCSAdministrator::Run");
	
	ClPDU *poInputPDU=NULL;
	m4int16_t iPDUId;
	m4return_t retCode;
	
	ClTSAPSelect m_oTSAPSelect;
	m_oTSAPSelect.Add(m_poTSAP, M4_TRUE, M4_FALSE, M4_FALSE);

	while (!IsTerminate())
	{
		m4TraceHeaderLevel(3, cout << "ClCSAdministrator::Run -- New PDU has arrived." << endl);
		
		
		retCode=m_oTSAPSelect.Select(5L, 0L, M4_TRUE, M4_FALSE, M4_FALSE);

		if ( IsTerminate() )
			return ;
		
		if(retCode==0)
		{
			m4return_t iRet;
			IEventHandler * TheEH=NULL;
			TheEH=ClLogBasedEventHandler::GetEHInstance();
			m4int_t Defaultbuffersize=20000;
			m4int_t buffersize=Defaultbuffersize;
			m4pchar_t buffer=new m4char_t [buffersize];
			buffer[0]='\0';
			if(TheEH)
				iRet=TheEH->GetEventMsg(*this,buffer,buffersize,M4UniversalEventTopic);
			while((iRet==M4_ERROR) && (buffersize>Defaultbuffersize))
			{
				delete buffer;
				buffer=new m4char_t[buffersize+1];
				iRet=TheEH->GetEventMsg(*this,buffer,buffersize,M4UniversalEventTopic);
			}
			if(iRet==M4_SUCCESS && buffer[0] && buffersize)
			{
 				ClProtocol oProtocol;
	
				ClReplyPDU* poReplyPDU = (ClReplyPDU *)oProtocol.GetPDU(M4_ID_PDU_REPLY);	
				if (NULL == poReplyPDU)
				{
					m4Trace(cerr << "Error creating ReplyPDU"<<endl);
				}
				M4DataStorage *poDataStorage=M4DataStorage::GetNewDataStorage();

				ClDataUnit *TheDU;
				
				poDataStorage->GetNewDataUnit(TheDU);
				TheDU->SetSize(buffersize+9);
				m4pchar_t buffer2=TheDU->GetBuffer();
				memcpy(buffer2,"@@EVENT@@",9);
				memcpy(buffer2+9,buffer,buffersize);
				poReplyPDU->SetRequestId(0);
				poReplyPDU->SetDataStorage(poDataStorage);
				SendPDU(poReplyPDU);
				M4DataStorage::DeleteDataStorage(poDataStorage);
			}
			delete buffer;
		}
		else
		{
			//-------------------------------------------------------------------
			// read the input PDU.
			//-------------------------------------------------------------------
			retCode = ReadPDU(poInputPDU, iPDUId);
			if (retCode != M4_SUCCESS)
			{
				m4TraceHeaderLevel(1, cout << "ClCSAdministrator::Run -- Error in reading input PDU." << endl);
				m4TraceLevel(1, cout << "Close session and Terminate the thread." << endl);

				// fix bug 0190803
				// The Terminate() method will close the socket using an action later
				// If we do this here the transport layer will select a socket already closed,
				// generating an error.
				// m_poTSAP->Close();

				// old style, obsolete
				// m_poLogonInterface->CloseSession(m_poUSSessionInterface);
				
				Terminate(M4_TRUE);
				
				continue;
			}
		
		
			//-------------------------------------------------------------------
			// Create the request executor.
			//-------------------------------------------------------------------
			ClAdminPDUExecutor *poPduExecutor = m_oPduExecutorCreator.Create(this, iPDUId);
			if (poPduExecutor == NULL)
			{
				continue;
			}
			
			//-------------------------------------------------------------------
			// Dump the input PDU. Only for debug.
			//-------------------------------------------------------------------
			m4TraceLevel(4, poPduExecutor->DumpInputPDU(poInputPDU));
			
			//-------------------------------------------------------------------
			// Execute the input PDU and get the answer PDU.
			//-------------------------------------------------------------------
			retCode = poPduExecutor->Execute(poInputPDU);
			
			//-------------------------------------------------------------------
			// Delete the PDU executor.
			//-------------------------------------------------------------------
			if (poPduExecutor != NULL)
			{
				delete poPduExecutor;
				poPduExecutor = NULL;
			}
			
			//-------------------------------------------------------------------
			// Delete input PDU.
			//-------------------------------------------------------------------
			if (poInputPDU != NULL) 
			{
				delete poInputPDU;
				poInputPDU = NULL;
			}
		}
	}

//	Cableo para desregistrarse del ML y así corregir un bug importante. 
	ClSSInterfaz * poLocalMonitorSS = (ClSSInterfaz *) m_poNavigator -> GetSubsystemByName ( "COMMUNICATION_SERVER" ) ;

	if ( poLocalMonitorSS )
	{
		ClAdminCommandTag *newParam;
		ClAdminTagList oParamList, oResultList ;
		
		// bg 152688
		m4char_t szPointer [21];
		sprintf ( szPointer , M4_PRIdPTR, this ) ;

		newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "ADMINISTRATOR POINTER", adminLong, szPointer );
		oParamList.push_back(newParam);

		poLocalMonitorSS -> ExecuteCommand ( poLocalMonitorSS -> GetssId () , 111, oParamList, oResultList );

		delete newParam ;
	}

  //## end ClCSAdministrator::Run%878114678.body
}

m4return_t ClCSAdministrator::ReadPDU (ClPDU* &ao_poInputPDU, m4int16_t &ao_iPDUId)
{
  //## begin ClCSAdministrator::ReadPDU%885830286.body preserve=yes
	
	m4AutoTraceLevel(5, "ClCSAdministrator::ReadPDU");
	m4int32_t iBytesReaded = 0;
	
	//-------------------------------------------------------------------
	// Read PDU header.
	//-------------------------------------------------------------------
	ClProtocol oProtocol;
	m4int32_t iHeaderSize = oProtocol.GetHeadSize();
	m4char_t pcPDUtype[128];
	iBytesReaded = m_poTSAP->Read(pcPDUtype, iHeaderSize);
	if (iBytesReaded <= 0)
	{
		m4TraceHeaderLevel(3, cout << "ClCSAdministrator::ReadPDU -- iBytesReaded <= 0." << endl);
		return M4_ERROR;
	}
	
	//-------------------------------------------------------------------
	// Get PDU type and PDU Id.
	//-------------------------------------------------------------------
	ao_poInputPDU = oProtocol.GetPDU(pcPDUtype);

	if ( ! ao_poInputPDU )
	{
		return M4_ERROR ;
	}

	ao_iPDUId = ao_poInputPDU->GetIdPDU();

  
	//-------------------------------------------------------------------
	// Read the input PDU.
	//-------------------------------------------------------------------
	m4uint32_t uiBlockSize;
	m4pchar_t pcDataBlock = NULL;

	while((uiBlockSize = ao_poInputPDU->GetNextSizeBlock(pcDataBlock)) != 0) 
	{
		iBytesReaded = m_poTSAP->Read(pcDataBlock, uiBlockSize);
		if (iBytesReaded <= 0)
		{
			m4TraceHeaderLevel(3, cout << "ClCSAdministrator::ReadPDU -- iBytesReaded <= 0." << endl);
			return M4_ERROR;
		}
		ao_poInputPDU -> AddBlock(pcDataBlock);
		pcDataBlock = NULL;
	}

	//-------------------------------------------------------------------
	// normal return.
	//-------------------------------------------------------------------
	return M4_SUCCESS;
  //## end ClCSAdministrator::ReadPDU%885830286.body
}

m4return_t ClCSAdministrator::SendPDU (ClPDU* ai_poPDU)
{
  //## begin ClCSAdministrator::SendPDU%885830287.body preserve=yes
	m4AutoTraceLevel(5, "ClCSAdministrator::SendPDU");

	//-------------------------------------------------------------------
	// check input PDU.
	//-------------------------------------------------------------------
	if (ai_poPDU == NULL)
	{
		return M4_ERROR;
	}


	//-------------------------------------------------------------------
	// serialize log info.
	//-------------------------------------------------------------------
	m4uint32_t logSize ;
	m4bool_t bLog=M4_FALSE;
	m4pchar_t logBuffer = NULL;
	m4uint32_t first=0;
	m4return_t iRet= GET_SERIAL_STRING_SIZE(logSize);

	if(M4_ERROR!=iRet)
	{
	//	bLog=M4_TRUE;
		if (0 < logSize)
		{
			logBuffer = new m4char_t[logSize];
			if (NULL != logBuffer)
			{
				GET_SERIALIZE_ERRORS(logBuffer, logSize,first);
				ai_poPDU->EnableLogInfo();
				ai_poPDU->SetLogInfo(logBuffer, logSize);
				CLEAN();
				delete [] logBuffer ;
			}
		}
	}		
	
	//-------------------------------------------------------------------
	// send the PDU to the network.
	//-------------------------------------------------------------------
	m4uint32_t uBytesRead, uTotalBytesRead = 0;
	m4pchar_t pcBlock = NULL;

	while ((ai_poPDU->GetNextBlock(uTotalBytesRead, pcBlock, uBytesRead)) == M4_SUCCESS) 
	{
		m_poTSAP->Write(pcBlock, uBytesRead);
		uTotalBytesRead += uBytesRead;
		pcBlock = NULL;
	}

	//-------------------------------------------------------------------
	// send log info.
	//-------------------------------------------------------------------
	if (bLog && (NULL != logBuffer) && (0 < logSize))
	{
		m_poTSAP->Write(logBuffer, logSize);
		if (NULL != logBuffer)
		{
			delete logBuffer;
			logBuffer = NULL;
		}
	}	

	return M4_SUCCESS;
  //## end ClCSAdministrator::SendPDU%885830287.body
}

// Additional Declarations
  //## begin ClCSAdministrator.declarations preserve=yes
  //## end ClCSAdministrator.declarations



//## begin module.epilog preserve=yes
//## end module.epilog
