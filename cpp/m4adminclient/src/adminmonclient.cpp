//## begin module.cm preserve=no
//## end module.cm

//## begin module.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            AdminMonClient
//	 File:              adminmonclient.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines the administrator client for server 
//		monitoring.
//
//	=============================================================================
//## end module.cp

//## Module: AdminMonClient; Package body
//## Subsystem: CPM4AdminClient
//## Source file: d:\marcoscp\M4AdminClient\version\src\adminmonclient.cpp

//## begin module.additionalIncludes preserve=no
//## end module.additionalIncludes

//## begin module.includes preserve=yes
//## end module.includes

// AdminMonClient
#include <adminmonclient.hpp>
#include <clienttag.hpp>
#include <m4log.hpp>
//## begin module.declarations preserve=no
//## end module.declarations

//## begin module.additionalDeclarations preserve=yes
#include "m4trace.hpp"
#include "adminmoncmdsid.hpp"

#include "m4regimpl.hpp"
//## end module.additionalDeclarations


// Class ClAdminMonClient 





//## begin ClAdminMonClient::m_admServiceId.attr preserve=no  private: static M4ClString {UA} "M4_ADMINISTRATION_SERVICE"
M4ClString ClAdminMonClient::m_admServiceId = "M4_ADMINISTRATION_SERVICE";
//## end ClAdminMonClient::m_admServiceId.attr







ClAdminMonClient::ClAdminMonClient ()
  //## begin ClAdminMonClient::ClAdminMonClient%884781527.hasinit preserve=no
      : m_bConnected(M4_FALSE), m_sessionId(0), m_serviceContext(0)
  //## end ClAdminMonClient::ClAdminMonClient%884781527.hasinit
  //## begin ClAdminMonClient::ClAdminMonClient%884781527.initialization preserve=yes

  //## end ClAdminMonClient::ClAdminMonClient%884781527.initialization
{
  //## begin ClAdminMonClient::ClAdminMonClient%884781527.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminMonClient::ClAdminMonClient");

	ClPairIdCommand *poCommand;
	m4return_t RetVal;

	ClVMBaseEnv	*	poEnv = new ClM4ObjRegClient();
	
	if( M4_SUCCESS != poEnv->Init() )
	{
		m_pAdmClient = new ClClientAPI( "" );
	}
	else
	{
		m4int32_t iPort = ( ( ClM4ObjRegClient* )poEnv )->GetControlPort();
		m_pAdmClient = new ClClientAPI( "", M4_ADVANCED_COMMS_TYPE, iPort );
	}

	delete poEnv;

	//-----------------------------------------------------------------------
	// initialize the log manager.
	//-----------------------------------------------------------------------	
	m4TraceHeaderLevel(4, cout << "Initializing log..." << endl);	

	/*
	ClLogManager* poLogManager = GETLOGMANAGER();
	if (NULL != poLogManager)
	{
		m4TraceHeaderLevel(4, cout << "Destroying invalid log manager..." << endl);	
		DESTROYLOGMANAGER();
	}
	INITIALIZELOGMANAGER("./adminlog.ini");
	*/
		
	m4TraceHeaderLevel(3, cout << "ClAdminMonClient::ClAdminMonClient: Inserting command handles." << endl);

	//-----------------------------------------------------------------------
	// insert M4_ADM_GET_SYSTEM_INFO handle in the command table.
	//-----------------------------------------------------------------------	
	poCommand = new ClAdminMonitorCommand(M4_ADM_GET_SYSTEM_INFO);
	if (poCommand != NULL)
	{
		RetVal = m_oTokenCommandTable.Add(poCommand);
		M4_ASSERT(RetVal == M4_SUCCESS);
	}
	
	//-----------------------------------------------------------------------
	// insert M4_ADM_GET_SUBSYSTEM_INFO handle in the command table.
	//-----------------------------------------------------------------------	
	poCommand = new ClAdminMonitorCommand(M4_ADM_GET_SUBSYSTEM_INFO);
	if (poCommand != NULL)
	{
		RetVal = m_oTokenCommandTable.Add(poCommand);
		M4_ASSERT(RetVal == M4_SUCCESS);
	}
	
	//-----------------------------------------------------------------------
	// insert M4_ADM_GET_SUBSYSTEM_INFO_VALUES handle in the command table.
	//-----------------------------------------------------------------------	
	poCommand = new ClAdminMonitorCommand(M4_ADM_GET_SUBSYSTEM_INFO_VALUES);
	if (poCommand != NULL)
	{
		RetVal = m_oTokenCommandTable.Add(poCommand);
		M4_ASSERT(RetVal == M4_SUCCESS);
	}
	
	//-----------------------------------------------------------------------
	// insert M4_ADM_GET_COMMANDS_INFO handle in the command table.
	//-----------------------------------------------------------------------	
	poCommand = new ClAdminMonitorCommand(M4_ADM_GET_COMMANDS_INFO);
	if (poCommand != NULL)
	{
		RetVal = m_oTokenCommandTable.Add(poCommand);
		M4_ASSERT(RetVal == M4_SUCCESS);
	}

	//-----------------------------------------------------------------------
	// insert M4_ADM_SHUTDOWN handle in the command table.
	//-----------------------------------------------------------------------	
	poCommand = new ClAdminMonitorCommand(M4_ADM_SHUTDOWN);
	if (poCommand != NULL)
	{
		RetVal = m_oTokenCommandTable.Add(poCommand);
		M4_ASSERT(RetVal == M4_SUCCESS);
	}
	
	//-----------------------------------------------------------------------
	// insert M4_ADM_EXECUTE_COMMAND handle in the command table.
	//-----------------------------------------------------------------------	
	poCommand = new ClAdminMonitorCommand(M4_ADM_EXECUTE_COMMAND);
	if (poCommand != NULL)
	{
		RetVal = m_oTokenCommandTable.Add(poCommand);
		M4_ASSERT(RetVal == M4_SUCCESS);
	}
	
	//-----------------------------------------------------------------------
	// initialize the parser with the monitor commands.
	//-----------------------------------------------------------------------	
	m4TraceHeaderLevel(3, cout << "ClAdminMonClient::ClAdminMonClient: Building administrator parser." << endl);
	m_poParser = new ClParser(&m_oTokenCommandTable, "acparser.ini");
	M4_ASSERT(m_poParser != NULL);

  //## end ClAdminMonClient::ClAdminMonClient%884781527.body
}


ClAdminMonClient::~ClAdminMonClient ()
{
  //## begin ClAdminMonClient::~ClAdminMonClient%882296710.body preserve=yes

	m4AutoTraceLevel(5, "ClAdminMonClient::~ClAdminMonClient");

	if( m_pAdmClient != NULL )
	{
		m4TraceHeaderLevel(3, cout << "ClAdminMonClient::~ClAdminMonClient: Deleting admin client." << endl);
		delete m_pAdmClient;
	}

	//-----------------------------------------------------------------------
	// free the parser.
	//-----------------------------------------------------------------------	
	if (m_poParser != NULL)
	{
		m4TraceHeaderLevel(3, cout << "ClAdminMonClient::~ClAdminMonClient: Deleting administrator parser." << endl);
		delete m_poParser;
	}

	//-----------------------------------------------------------------------
	// Deleting command table.
	//-----------------------------------------------------------------------	
	m4TraceHeaderLevel(3, cout << "Deleting command table..." << endl);
	m_oTokenCommandTable.Clear();

	//-----------------------------------------------------------------------
	// destroy the log manager.
	//-----------------------------------------------------------------------	
//	DESTROYLOGMANAGER();

  //## end ClAdminMonClient::~ClAdminMonClient%882296710.body
}



//## Other Operations (implementation)
m4return_t ClAdminMonClient::Connect (const m4pchar_t ai_szServerAddress, const m4uint32_t ai_iServerPort, const m4pchar_t ai_szUser, const m4pchar_t ai_szPassword)
{
  //## begin ClAdminMonClient::Connect%885289654.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminMonClient::Connect");
	m4TraceHeaderLevel(2, cout << "ClAdminMonClient::Connect: " << ai_szServerAddress << ", " << ai_iServerPort << ", " << ai_szUser << ", " << ai_szPassword << endl);
	m4return_t retCode;
	
	//-----------------------------------------------------------------------
	// initialize the administrator client.
	//-----------------------------------------------------------------------
	m4TraceHeaderLevel(3, cout << "ClAdminMonClient::Connect: Initializing admin client." << endl);
	retCode = m_pAdmClient->Init(100000);
	
	// build the taglist that identifies the administrator.
	ClTagList otagList;
	otagList.SetArg (m_admServiceId, m_admServiceId);

	// preprocessing log.
	PreprocessingLogInfo();

	//-----------------------------------------------------------------------
	// open the session.
	//-----------------------------------------------------------------------
	m4TraceHeaderLevel(3, cout << "ClAdminMonClient::Connect: OpenSession." << endl);
	M4ClString strHost(ai_szServerAddress);
	M4ClString strUser(ai_szUser);
	M4ClString strPasswd(ai_szPassword);

	retCode = m_pAdmClient->OpenSession(
		strHost, 
		ai_iServerPort, 
		strUser, 
		strPasswd, 
		M4_TRUE,		// use default role.
		m_sessionId,
		&otagList);

	if (M4_SUCCESS != retCode)
	{	
		// postprocessing log.
		return PostprocessingLogInfo(retCode);
	}
	
	//-----------------------------------------------------------------------
	// enable the administrator role.
	// Actually, the administrator role is hardcoded in the server side,
	// but call this function in an API CLIENT requirement.
	//-----------------------------------------------------------------------
	m4TraceHeaderLevel(3, cout << "ClAdminMonClient::Connect: EnableRole." << endl);
	retCode = m_pAdmClient->EnableRole(m_sessionId);
	if (M4_SUCCESS != retCode)
	{
		m_pAdmClient->CloseSession(m_sessionId);

		// postprocessing log.
		return PostprocessingLogInfo(retCode);
	}
	
	//-----------------------------------------------------------------------
	// open the administration service.
	//-----------------------------------------------------------------------
	m4TraceHeaderLevel(3, cout << "ClAdminMonClient::Connect: OpenService." << endl);
	retCode = m_pAdmClient->OpenService(m_sessionId, m_admServiceId, m_serviceContext);
	if (M4_SUCCESS != retCode)
	{
		m_pAdmClient->CloseSession(m_sessionId);

		// postprocessing log.
		return PostprocessingLogInfo(retCode);
	}
	
	//-----------------------------------------------------------------------
	// normal return.
	//-----------------------------------------------------------------------
	m_bConnected = M4_TRUE;

	// postprocessing log.
	return PostprocessingLogInfo(retCode);
  //## end ClAdminMonClient::Connect%885289654.body
}

m4return_t ClAdminMonClient::Disconnect ()
{
  //## begin ClAdminMonClient::Disconnect%885289655.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminMonClient::Disconnect");
	m4TraceHeaderLevel(2, cout << "ClAdminMonClient::Disconnect" << endl);
	
	//-----------------------------------------------------------------------
	// reset m_bConnected flag.
	//-----------------------------------------------------------------------
	m_bConnected = M4_FALSE;
	
	//-----------------------------------------------------------------------
	// close the administration service.
	//-----------------------------------------------------------------------
	m4TraceHeaderLevel(3, cout << "ClAdminMonClient::Disconnect: CloseService." << endl);
	m_pAdmClient->CloseService(m_sessionId, m_serviceContext);
	
	//-----------------------------------------------------------------------
	// disable administrator role.
	//-----------------------------------------------------------------------
	m4TraceHeaderLevel(3, cout << "ClAdminMonClient::Disconnect: DisableRole." << endl);
	m_pAdmClient->DisableRole(m_sessionId, 0);
	
	//-----------------------------------------------------------------------
	// close session.
	//-----------------------------------------------------------------------
	m4TraceHeaderLevel(3, cout << "ClAdminMonClient::Disconnect: CloseSession." << endl);
	m_pAdmClient->CloseSession(m_sessionId);
	
	//-----------------------------------------------------------------------
	// normal return.
	//-----------------------------------------------------------------------
	return M4_SUCCESS;
  //## end ClAdminMonClient::Disconnect%885289655.body
}

m4return_t ClAdminMonClient::Execute (m4pchar_t ai_szSentence, m4pchar_t ao_szAnswer, m4uint16_t &aio_uiSize)
{
  //## begin ClAdminMonClient::Execute%884261081.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminMonClient::Execute");
	m4TraceHeaderLevel(2, cout << "ClAdminMonClient::Execute: command:" << ai_szSentence << "(" << aio_uiSize << ")" << endl);
	
	m4return_t retCode;
	
	//-----------------------------------------------------------------------
	// check if the administrator is connected.
	//-----------------------------------------------------------------------		
	if (!m_bConnected)
	{
		m4TraceHeaderLevel(1, cout << "ClAdminMonClient::Execute: The administrator is not connected." << endl);
		return M4_ERROR;
	}
	
	//-----------------------------------------------------------------------
	// check the input sentence. If ai_szSentence is NULL, copy in 
	// ao_szAnswer the answer from the last call to Execute. 
	//-----------------------------------------------------------------------		
	if(!strcmp(ai_szSentence,""))
	{
		if (m_serverAnswer.size() <= aio_uiSize) 
		{
			aio_uiSize = m_serverAnswer.size();
			memcpy(ao_szAnswer, m_serverAnswer.c_str(), aio_uiSize);
			retCode =  M4_SUCCESS;
			
			//
			// reset the string for the next command
			m_serverAnswer = "";
		}	
		else
		{
			aio_uiSize = m_serverAnswer.size();
			retCode =  M4_WARNING;
		}
		
		return retCode;
	}
	
	//-----------------------------------------------------------------------
	// parse the input sentence.
	//-----------------------------------------------------------------------		
	m4TraceHeaderLevel(3, cout << "ClAdminMonClient::Execute: Parse input sentence." << endl);
	m_serverAnswer = "";
	retCode = m_poParser->Parse(ai_szSentence, &m_oSentenceTable);
	if (retCode != M4_SUCCESS)
	{
		m4TraceHeaderLevel(1, cout << "ClAdminMonClient::Execute: Invalid command." << endl);
		return M4_ERROR;
	}
	
	//---------------------------------------------------------------
	// for each element in the sentenceTable create a dataStorage.
	//---------------------------------------------------------------
	ClSentenceTable::iterator oSenTIterator;
	M4ClString partialAnswer = "";
	
	for (oSenTIterator = m_oSentenceTable.begin(); oSenTIterator != m_oSentenceTable.end(); oSenTIterator++)
	{
		//------------------------------------------------------------
		// if the next assert fail normally is due to the acparser.ini
		// file is wrong.
		//------------------------------------------------------------
		ClAdminMonitorCommand *poAdminMonitorCommand =
			(ClAdminMonitorCommand *)(*oSenTIterator).m_oPointer;
		M4_ASSERT(poAdminMonitorCommand != NULL);
		if (NULL == poAdminMonitorCommand) return M4_ERROR;

		retCode = poAdminMonitorCommand->Execute(this, (*oSenTIterator).m_oArguments, partialAnswer);

		//------------------------------------------------------------
		// build the full answer to the client with the answer 
		// returned by the server.
		//------------------------------------------------------------
		if (retCode == M4_SUCCESS) 
		{
			if (m_serverAnswer == "")
			{
				//
				// this is the first answer for the full sentence
				if (partialAnswer == "")
				{
					// insert a space in the global answer.
					m_serverAnswer = " ";
				}
				else
				{
					m_serverAnswer = partialAnswer;

					// reset the partialAnswer
					partialAnswer = "";
				}
			}
			else
			{
				m_serverAnswer += ":";
				m_serverAnswer += partialAnswer;
				
				// reset the partialAnswer
				partialAnswer = "";
			}
			
		}
		else
		{
			//
			// clear the sentence table.
			SentenceClear();
			return M4_ERROR;
		}

	} // for (...)
	
	//-----------------------------------------------------------------------
	// clear the sentence table.
	//-----------------------------------------------------------------------
	SentenceClear();
	
	//-----------------------------------------------------------------------
	// copy full answer in output buffer.
	//-----------------------------------------------------------------------
	if (m_serverAnswer.size() <= aio_uiSize) 
	{
		aio_uiSize = m_serverAnswer.size();
		memcpy(ao_szAnswer, m_serverAnswer.c_str(), aio_uiSize);
		retCode =  M4_SUCCESS;

		//
		// reset the string for the next command
		m_serverAnswer = "";
	}
	else
	{
		aio_uiSize = m_serverAnswer.size();
		retCode =  M4_WARNING;
	}
	
	//-----------------------------------------------------------------------
	// normal return.
	//-----------------------------------------------------------------------
	m4TraceHeaderLevel(2, cout << "ClAdminMonClient::Execute: answer: " << ao_szAnswer << "(" << aio_uiSize << ")" << endl);
	return retCode;
  //## end ClAdminMonClient::Execute%884261081.body
}

m4return_t ClAdminMonClient::SendSyncRequest (M4DataStorage *ai_inputDS, M4DataStorage *ao_outputDS)
{
  //## begin ClAdminMonClient::SendSyncRequest%901526255.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminMonClient::SendSyncRequest");
	m4TraceHeaderLevel(4, cout << "Sending a synRequest to the server..." << endl);

	//------------------------------------------------------------
	// send the dataStorage to the Server.
	//------------------------------------------------------------
	m4return_t retCode = M4_ERROR;
	m4int32_t iStateFlags = 0;
	m4int32_t iFlags = 0;
	m4handle_t hRequestId;
	const m4uint32_t uiTimeout = 0x1FFFFFFF;
	m4bool_t bLogInfo = M4_TRUE; 

	// preprocessing log.
	PreprocessingLogInfo();

	retCode = m_pAdmClient->SyncRequest(m_sessionId, m_serviceContext, 
		iStateFlags, iFlags, 
		ai_inputDS, ao_outputDS, 
		hRequestId, uiTimeout,
		bLogInfo);

	// postprocessing log.
	return PostprocessingLogInfo(retCode);
  //## end ClAdminMonClient::SendSyncRequest%901526255.body
}

m4bool_t ClAdminMonClient::CheckConnection ()
{
  //## begin ClAdminMonClient::CheckConnection%911389909.body preserve=yes
	return m_bConnected;
  //## end ClAdminMonClient::CheckConnection%911389909.body
}

m4return_t ClAdminMonClient::GetLogError (M4ClString &ao_type, M4ClString &ao_Code, M4ClString &ao_Text)
{
  //## begin ClAdminMonClient::GetLogError%923332969.body preserve=yes
	ClLogManager *poLogManager = NULL;
	m4return_t retCode = M4_ERROR;

	// get the log object.
	poLogManager = GETLOGMANAGER();
	if (NULL == poLogManager)
	{
		m4TraceHeaderLevel(3, cout << "Invalid log object" << endl);
		return M4_ERROR;
	}

	// temporal buffer for log error.
	const m4int32_t bufferSize = 4096;
	m4int32_t bufferLength = 0;
	m4char_t buffer[bufferSize];
		
	// get the error type.
	retCode = GETH_ERRORTYPE(buffer, bufferSize, bufferLength);
	if (M4_SUCCESS != retCode)
	{
		m4TraceHeaderLevel(4, cout << "error in GETH_ERRORTYPE" << endl);
		return M4_ERROR;
	}
	ao_type = buffer;
	
	// get the error code.
	retCode = GETH_ERRORCODE(buffer, bufferSize, bufferLength);
	if (M4_SUCCESS != retCode)
	{
		m4TraceHeaderLevel(4, cout << "error in GETH_ERRORCODE" << endl);
		return M4_ERROR;
	}
	ao_Code = buffer;

	// get the error text.
	retCode = GETH_ERRORTEXT(buffer, bufferSize, bufferLength);
	if (M4_SUCCESS != retCode)
	{
		m4TraceHeaderLevel(4, cout << "error in GETH_ERRORTEXT" << endl);
		return M4_ERROR;
	}
	ao_Text = buffer;

	// clear this error from the queue.
	POP();

	return M4_SUCCESS;
  //## end ClAdminMonClient::GetLogError%923332969.body
}

m4void_t ClAdminMonClient::SentenceClear ()
{
  //## begin ClAdminMonClient::SentenceClear%884781526.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminMonClient::SentenceClear");
	
	m_poParser->SentenceClear(&m_oSentenceTable);
  //## end ClAdminMonClient::SentenceClear%884781526.body
}

m4return_t ClAdminMonClient::DecodeDataStorage (M4DataStorage *ai_data, M4ClString &ao_string)
{
  //## begin ClAdminMonClient::DecodeDataStorage%897034497.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminMonClient::decodeDataStorage");
	//-----------------------------------------------------------------------
	// get how many dataUnits there are in the dataStorage.
	//-----------------------------------------------------------------------
	m4uint32_t uiUnits = ai_data->Blocks();
	
	//-----------------------------------------------------------------------
	// iter over the dataStorage and get the dataUnits.
	//-----------------------------------------------------------------------
	m4pchar_t pBuffer;
	m4uint32_t uiBufferSize;
	m4bool_t bEncrypted;
	m4return_t retCode;

	for (m4uint32_t iblock=0; iblock < uiUnits; iblock++)
	{
		retCode = ai_data->Get(pBuffer, uiBufferSize, bEncrypted, iblock);
		if (retCode == M4_SUCCESS)
		{
			//
			// copy pbuffer to output string.
			if (iblock == 0)
			{
				ao_string = pBuffer;
			}
			else
			{
				ao_string += ":";
				ao_string += pBuffer;
			}
		}
		else 
		{
			return M4_ERROR;
		}
	}
		
	//-----------------------------------------------------------------------
	// normal return.
	//-----------------------------------------------------------------------
	return M4_SUCCESS;
  //## end ClAdminMonClient::DecodeDataStorage%897034497.body
}

m4return_t ClAdminMonClient::PreprocessingLogInfo (m4return_t ai_lastRetCode)
{
  //## begin ClAdminMonClient::PreprocessingLogInfo%923332968.body preserve=yes
	// clear the log object.
	CLEAN();

	return ai_lastRetCode;
  //## end ClAdminMonClient::PreprocessingLogInfo%923332968.body
}

m4return_t ClAdminMonClient::PostprocessingLogInfo (m4return_t ai_lastRetCode)
{
  //## begin ClAdminMonClient::PostprocessingLogInfo%923556879.body preserve=yes
	ClLogManager *poLogManager = NULL;
	m4return_t retCode = M4_ERROR;
	
	// get the log object.
	poLogManager = GETLOGMANAGER();
	if (NULL == poLogManager)
	{
		m4TraceHeaderLevel(3, cout << "Invalid log object" << endl);
		return ai_lastRetCode;
	}

	// dump errors in log object.
	m4TraceLevel(4, DumpLogIngo());

	return ai_lastRetCode;
  //## end ClAdminMonClient::PostprocessingLogInfo%923556879.body
}

void ClAdminMonClient::DumpLogIngo ()
{
  //## begin ClAdminMonClient::DumpLogIngo%923556880.body preserve=yes
	ClLogManager *poLogManager = NULL;

	// get the log object.
	poLogManager = GETLOGMANAGER();
	if (NULL == poLogManager)
	{
		m4TraceHeaderLevel(4, cout << "Invalid log object" << endl);
		return;
	}

	// iterate over the log object.
	m4int32_t index = 0;
	const m4int32_t bufferSize = 4096;
	m4int32_t bufferLength = 0;
	m4char_t bufferCode[bufferSize];
	m4char_t bufferType[bufferSize];
	m4char_t bufferText[bufferSize];
	
	m4return_t retCode = M4_SUCCESS;
	while (M4_SUCCESS == retCode)
	{
		retCode = GETH_N_ERRORCODE(index, bufferCode, bufferSize, bufferLength);
		retCode = GETH_N_ERRORTYPE(index, bufferType, bufferSize, bufferLength);
		retCode = GETH_N_ERRORTEXT(index, bufferText, bufferSize, bufferLength);
		if (M4_SUCCESS == retCode)
		{
			m4TraceLevel(4, cout << "[LOG] " << bufferType << "(" << bufferCode << "): " << bufferText << endl);
			index++;
		}
	}
  //## end ClAdminMonClient::DumpLogIngo%923556880.body
}

// Additional Declarations
  //## begin ClAdminMonClient.declarations preserve=yes
  //## end ClAdminMonClient.declarations



//## begin module.epilog preserve=yes
//## end module.epilog
