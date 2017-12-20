//## begin module.cm preserve=no
//## end module.cm

//## begin module.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              adminmoncommands.cpp
//	 Project:           M4AdminClient
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
//## end module.cp

//## Module: MonitorCommands; Package body
//## Subsystem: CPM4AdminClient
//## Source file: d:\marcoscp\M4AdminClient\version\src\adminmoncommands.cpp

//## begin module.additionalIncludes preserve=no
//## end module.additionalIncludes

//## begin module.includes preserve=yes
#include "adminmoncmdsid.hpp"
//## end module.includes

// MonitorCommands
#include <adminmoncommands.hpp>
// AdminMonClient
#include <adminmonclient.hpp>
//## begin module.declarations preserve=no
//## end module.declarations

//## begin module.additionalDeclarations preserve=yes
#define M4_MAX_NUM_ITEM	256
//## end module.additionalDeclarations


// Class ClAdminMonitorCommand 


ClAdminMonitorCommand::ClAdminMonitorCommand (const char* ai_pcCommandId)
  //## begin ClAdminMonitorCommand::ClAdminMonitorCommand%917274541.hasinit preserve=no
  //## end ClAdminMonitorCommand::ClAdminMonitorCommand%917274541.hasinit
  //## begin ClAdminMonitorCommand::ClAdminMonitorCommand%917274541.initialization preserve=yes
  //## end ClAdminMonitorCommand::ClAdminMonitorCommand%917274541.initialization
{
  //## begin ClAdminMonitorCommand::ClAdminMonitorCommand%917274541.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminMonitorCommand::ClAdminMonitorCommand");

	//---------------------------------------------------------------------------
	// initialize the command type.
	//---------------------------------------------------------------------------
	m4TraceLevel(3, cout << "Registering command: " << ai_pcCommandId << endl);
	if (NULL != ai_pcCommandId)
	{
		m_pCommandId = new m4char_t[strlen(ai_pcCommandId) + 1];
		strcpy(m_pCommandId, ai_pcCommandId);
	}
	else
	{
		m_pCommandId = NULL;
	}
  //## end ClAdminMonitorCommand::ClAdminMonitorCommand%917274541.body
}


ClAdminMonitorCommand::~ClAdminMonitorCommand ()
{
  //## begin ClAdminMonitorCommand::~ClAdminMonitorCommand%917274542.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminMonitorCommand::~ClAdminMonitorCommand");
	if (NULL != m_pCommandId)
	{
		delete m_pCommandId;
		m_pCommandId = NULL;
	}
  //## end ClAdminMonitorCommand::~ClAdminMonitorCommand%917274542.body
}



//## Other Operations (implementation)
m4return_t ClAdminMonitorCommand::Execute (ClAdminMonClient *ai_MonClient, const m4parsercommandargs_t &ai_InputData, M4ClString &ao_Answer)
{
  //## begin ClAdminMonitorCommand::Execute%917274544.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminMonitorCommand::Execute(...)");

	m4return_t retCode = M4_ERROR;

	ao_Answer = "";

	m4TraceHeaderLevel(4, cout << "Execute command type: " << m_pCommandId << endl);

	// create the executor for this command.
	ClAdminCommandExecutor *poCommandExecutor;
	poCommandExecutor = m_AdminCommandExecutorCreator.Create(m_pCommandId);
	if (NULL == poCommandExecutor)
	{
		return	M4_ERROR;
	}

	// execute the command.
	retCode = poCommandExecutor->Execute(ai_MonClient, ai_InputData, ao_Answer);
	delete poCommandExecutor;
	return	retCode;
  //## end ClAdminMonitorCommand::Execute%917274544.body
}

ClPDU* ClAdminMonitorCommand::Execute (m4parsercommandargs_t ai_pInput)
{
  //## begin ClAdminMonitorCommand::Execute%917339811.body preserve=yes
 	//-----------------------------------------------------------------------
	// this method is needed due to the base class is a abstract class.
	//-----------------------------------------------------------------------
	return (ClPDU*)NULL;
  //## end ClAdminMonitorCommand::Execute%917339811.body
}

m4return_t ClAdminMonitorCommand::Execute (void*& exit, ClCmdArguments *args)
{
  //## begin ClAdminMonitorCommand::Execute%917426672.body preserve=yes
 	//-----------------------------------------------------------------------
	// this method is needed due dependencies with ClCommand class.
	//-----------------------------------------------------------------------
	return M4_SUCCESS;
  //## end ClAdminMonitorCommand::Execute%917426672.body
}

// Additional Declarations
  //## begin ClAdminMonitorCommand.declarations preserve=yes
  //## end ClAdminMonitorCommand.declarations



// Class ClAdminCommandExecutorCreator 



//## Other Operations (implementation)
ClAdminCommandExecutor * ClAdminCommandExecutorCreator::Create (const char *ai_pcCommandId)
{
  //## begin ClAdminCommandExecutorCreator::Create%917339812.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminCommandExecutorCreator::Create(...)");

	ClAdminCommandExecutor * theCommand=NULL;

	if (NULL != ai_pcCommandId) 
	{
	
		if (!strcmp(ai_pcCommandId, M4_ADM_GET_SYSTEM_INFO))
		{
			theCommand= new ClACGetSystemInfo(ai_pcCommandId);
		}
		else if (!strcmp(ai_pcCommandId, M4_ADM_GET_SUBSYSTEM_INFO))
		{
			theCommand= new ClACGetSubsystemInfo(ai_pcCommandId);
		}
		else if (!strcmp(ai_pcCommandId, M4_ADM_GET_SUBSYSTEM_INFO_VALUES))
		{
			theCommand= new ClACGetSubsystemInfoValues(ai_pcCommandId);
		}
		else if (!strcmp(ai_pcCommandId, M4_ADM_GET_COMMANDS_INFO))
		{
			theCommand= new ClACGetCommandsInfo(ai_pcCommandId);
		}
		else if (!strcmp(ai_pcCommandId, M4_ADM_SHUTDOWN))
		{
			theCommand= new ClACShutdown(ai_pcCommandId);
		}
		else if (!strcmp(ai_pcCommandId, M4_ADM_EXECUTE_COMMAND))
		{
			theCommand= new ClACExecuteCommand(ai_pcCommandId);
		}
	}

	return theCommand;
  //## end ClAdminCommandExecutorCreator::Create%917339812.body
}

// Additional Declarations
  //## begin ClAdminCommandExecutorCreator.declarations preserve=yes
  //## end ClAdminCommandExecutorCreator.declarations



// Class ClAdminCommandExecutor 


ClAdminCommandExecutor::ClAdminCommandExecutor ()
  //## begin ClAdminCommandExecutor::ClAdminCommandExecutor%917513511.hasinit preserve=no
      : m_AdminCommandId(NULL)
  //## end ClAdminCommandExecutor::ClAdminCommandExecutor%917513511.hasinit
  //## begin ClAdminCommandExecutor::ClAdminCommandExecutor%917513511.initialization preserve=yes
  //## end ClAdminCommandExecutor::ClAdminCommandExecutor%917513511.initialization
{
  //## begin ClAdminCommandExecutor::ClAdminCommandExecutor%917513511.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminCommandExecutor::ClAdminCommandExecutor ()");
  //## end ClAdminCommandExecutor::ClAdminCommandExecutor%917513511.body
}


ClAdminCommandExecutor::~ClAdminCommandExecutor ()
{
  //## begin ClAdminCommandExecutor::~ClAdminCommandExecutor%917513492.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminCommandExecutor::~ClAdminCommandExecutor ()");
	if (NULL != m_AdminCommandId)
	{
		delete m_AdminCommandId;
		m_AdminCommandId = NULL;
	}
  //## end ClAdminCommandExecutor::~ClAdminCommandExecutor%917513492.body
}



//## Other Operations (implementation)
m4return_t ClAdminCommandExecutor::Execute (ClAdminMonClient *ai_MonClient, const m4parsercommandargs_t &ai_InputData, M4ClString &ao_Answer)
{
  //## begin ClAdminCommandExecutor::Execute%917339813.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminCommandExecutor::Execute(...)");
	return	M4_ERROR;
  //## end ClAdminCommandExecutor::Execute%917339813.body
}

m4return_t ClAdminCommandExecutor::GetNextDataUnit (M4DataStorage *ai_dataStorage, char *&ao_data, int &ao_dataSize)
{
  //## begin ClAdminCommandExecutor::GetNextDataUnit%917522116.body preserve=yes
	ClDataUnitInterface *pDataUnitInterface;
	
	// get the dataUnitInterface.
	if (M4_SUCCESS != ai_dataStorage->GetNext(pDataUnitInterface))
	{
		return M4_ERROR;
	}

	// check the dataUnitInterface type.
	// The administrator only accept M4_DATA_UNIT.
	if (M4_DATA_UNIT != pDataUnitInterface->GetType())
	{
		return M4_ERROR;
	}
	
	ao_data = pDataUnitInterface->GetBuffer();
	ao_dataSize = pDataUnitInterface->GetSize();
	return M4_SUCCESS;			
  //## end ClAdminCommandExecutor::GetNextDataUnit%917522116.body
}

// Additional Declarations
  //## begin ClAdminCommandExecutor.declarations preserve=yes
  //## end ClAdminCommandExecutor.declarations



// Class ClACGetSubsystemInfo 

//## begin ClACGetSubsystemInfo::m_Path.attr preserve=no  private: static M4ClString {UA} 
M4ClString ClACGetSubsystemInfo::m_Path;
//## end ClACGetSubsystemInfo::m_Path.attr

//## begin ClACGetSubsystemInfo::m_Ids.attr preserve=no  private: static ClAdminInfoTagCache {UA} 
ClAdminInfoTagCache ClACGetSubsystemInfo::m_Ids;
//## end ClACGetSubsystemInfo::m_Ids.attr

//## begin ClACGetSubsystemInfo::m_Attributes.attr preserve=no  private: static ClAdminInfoTagCache {UA} 
ClAdminInfoTagCache ClACGetSubsystemInfo::m_Attributes;
//## end ClACGetSubsystemInfo::m_Attributes.attr

//## begin ClACGetSubsystemInfo::m_Statistics.attr preserve=no  private: static ClAdminInfoTagCache {UA} 
ClAdminInfoTagCache ClACGetSubsystemInfo::m_Statistics;
//## end ClACGetSubsystemInfo::m_Statistics.attr

ClACGetSubsystemInfo::ClACGetSubsystemInfo (const char *ai_AdminCommandId)
  //## begin ClACGetSubsystemInfo::ClACGetSubsystemInfo%917513493.hasinit preserve=no
  //## end ClACGetSubsystemInfo::ClACGetSubsystemInfo%917513493.hasinit
  //## begin ClACGetSubsystemInfo::ClACGetSubsystemInfo%917513493.initialization preserve=yes
  //## end ClACGetSubsystemInfo::ClACGetSubsystemInfo%917513493.initialization
{
  //## begin ClACGetSubsystemInfo::ClACGetSubsystemInfo%917513493.body preserve=yes
	m4AutoTraceLevel(5, "ClACGetSubsystemInfo::ClACGetSubsystemInfo");
	if (NULL != ai_AdminCommandId)
	{
		m_AdminCommandId = new char [strlen(ai_AdminCommandId)+1];
		if (NULL != m_AdminCommandId)
		{
			strcpy(m_AdminCommandId, ai_AdminCommandId);
		}
	}
  //## end ClACGetSubsystemInfo::ClACGetSubsystemInfo%917513493.body
}



//## Other Operations (implementation)
m4return_t ClACGetSubsystemInfo::Execute (ClAdminMonClient *ai_MonClient, const m4parsercommandargs_t &ai_InputData, M4ClString &ao_Answer)
{
  //## begin ClACGetSubsystemInfo::Execute%917339814.body preserve=yes
	m4AutoTraceLevel(5, "ClACGetSubSystemInfo::Execute(...)");
	m4return_t retCode = M4_SUCCESS;

	//------------------------------------------------------------------------
	// parse the command arguments.
	//------------------------------------------------------------------------
	m4parsercommandargs_t::const_iterator oIterator = ai_InputData.begin();
	m4TraceHeaderLevel(4, cout << "Execute - input arguments: " << endl);

	// the first item is SUBSYSTEM or STATISTIC.
	if (ai_InputData.end() == oIterator)
	{
		return M4_ERROR;
	}
	const char* firstItem = *oIterator;
	m4TraceLevel(4, cout << "-" << firstItem);
	oIterator++;

	// the second item is IDS or ATTRIBS.
	if (ai_InputData.end() == oIterator)
	{
		return M4_ERROR;
	}
	const char *secondItem = *oIterator;
	m4TraceLevel(4, cout << "-" << secondItem);
	oIterator++;

	// the third item is the subsystem path.
	if (ai_InputData.end() == oIterator)
	{
		return M4_ERROR;
	}
	const char *subsystemPath = *oIterator;
	m4TraceLevel(4, cout << "-" << subsystemPath);
	oIterator++;

	m4TraceLevel(4, cout << "-" << endl);

	//------------------------------------------------------------
	// Get answer from cache.
	//------------------------------------------------------------
	m4bool_t isValid;
	isValid = GetAnswerFromCache (subsystemPath, firstItem, secondItem, ao_Answer);

	if (M4_FALSE == isValid)
	{
		//----------------------------------------------------------
		// Get answer from server and set the cache.
		//----------------------------------------------------------
		retCode = GetAnswerFromServer(ai_MonClient, subsystemPath);

		if (M4_SUCCESS == retCode)
		{
			//------------------------------------------------------
			// Get answer from cache.
			//------------------------------------------------------
			isValid = GetAnswerFromCache (subsystemPath, firstItem, secondItem, ao_Answer);			
			if (M4_FALSE == isValid)
			{
				retCode = M4_ERROR;
			}
			else
			{
				retCode = M4_SUCCESS;
			}
		}		
	}
	return retCode;
  //## end ClACGetSubsystemInfo::Execute%917339814.body
}

m4bool_t ClACGetSubsystemInfo::GetAnswerFromCache (const char *ai_path, const char *ai_firstItem, const char *ai_secondItem, M4ClString &ao_Answer)
{
  //## begin ClACGetSubsystemInfo::GetAnswerFromCache%917513494.body preserve=yes
	m4AutoTraceLevel(5, "ClACGetSubSystemInfo::GetAnswerFromCache(...)");

	m4TraceHeaderLevel(3, cout << "ClACGetSubsystemInfo::GetAnswerFromCache" << endl);
	if (M4ClString(ai_path) != m_Path)
	{
		ResetCache(ai_path);
		return M4_FALSE;
	}

	if (!strcmp(ai_firstItem, "SUBSYSTEM") && !strcmp(ai_secondItem, "IDS"))
	{
		return m_Ids.Get(ao_Answer);
	}
	else if (!strcmp(ai_firstItem, "SUBSYSTEM") && !strcmp(ai_secondItem, "ATTRIBS"))
	{
		return m_Attributes.Get(ao_Answer);
	}
	else
	{
		return m_Statistics.Get(ao_Answer);
	}
  //## end ClACGetSubsystemInfo::GetAnswerFromCache%917513494.body
}

m4return_t ClACGetSubsystemInfo::GetAnswerFromServer (ClAdminMonClient *ai_MonClient, const char *ai_path)
{
  //## begin ClACGetSubsystemInfo::GetAnswerFromServer%917513495.body preserve=yes
	m4AutoTraceLevel(5, "ClACGetSubSystemInfo::GetAnswerFromServer(...)");
	m4return_t retCode = M4_ERROR;

	m4TraceHeaderLevel(3, cout << "ClACGetSubsystemInfo::GetAnswerFromServer" << endl);
	//------------------------------------------------------------
	// Create the dataStorages.
	//------------------------------------------------------------
	M4DataStorage *pInputData = M4DataStorage::GetNewDataStorage();
	if (NULL == pInputData)
	{
		return M4_ERROR;
	}
	M4DataStorage *pOutputData = M4DataStorage::GetNewDataStorage();
	if (NULL == pOutputData) 
	{
		M4DataStorage::DeleteDataStorage(pInputData);
		return M4_ERROR;
	}
	
	//------------------------------------------------------------
	// Build the input dataStorage.
	//------------------------------------------------------------
	ClDataUnit *poInputDataUnit;

	// the first dataUnit is the command id.
	pInputData->GetNewDataUnit(poInputDataUnit);
	if (NULL != poInputDataUnit)
	{
		retCode = poInputDataUnit->SetSize(strlen(m_AdminCommandId)+1);
		if (M4_SUCCESS == retCode)
		{
			strcpy(poInputDataUnit->GetBuffer(), m_AdminCommandId);
		}
	}
	else
	{
		retCode = M4_ERROR;
	}

	// the second dataUnit is the subsystem path.
	pInputData->GetNewDataUnit(poInputDataUnit);
	if (NULL != poInputDataUnit)
	{
		retCode = poInputDataUnit->SetSize(strlen(ai_path)+1);
		if (M4_SUCCESS == retCode)
		{
			strcpy(poInputDataUnit->GetBuffer(), ai_path);
		}
	}
	else
	{
		retCode = M4_ERROR;
	}

	//------------------------------------------------------------
	// Send the command to the server.
	//------------------------------------------------------------
	if (M4_SUCCESS == retCode)
	{
		retCode = ai_MonClient->SendSyncRequest (pInputData, pOutputData);
	}
	if (M4_SUCCESS == retCode)
	{
		// The dataUnits are:
		//		1 - subsystem ids.
		//		2 - subsystem attribs.
		//		3 - subsystem statistics.
		
		//-----------------------------------------------------------
		// iter over the dataStorage and get the dataUnits.
		//-----------------------------------------------------------
		m4pchar_t pBuffer;
		int iBufferSize;
		pOutputData->InitializeIteration();
		
		//-----------------------------------------------------------
		// the first dataUnit are the subsystem ids.
		//-----------------------------------------------------------
		retCode = GetNextDataUnit(pOutputData, pBuffer, iBufferSize);
		if (retCode == M4_SUCCESS)
		{
			m4TraceLevel(3, cout << "-- subsystem ids: " << pBuffer << endl);
			m_Ids.Set(M4ClString(pBuffer));
		}
		else
		{
			m4TraceLevel(3, cout << "-- subsystem ids: NULL value."  << endl);
			m_Ids.Set(M4ClString(""));
		}
		
		//-----------------------------------------------------------
		// the second dataUnit are the subsystem attribs.
		//-----------------------------------------------------------
		retCode = GetNextDataUnit(pOutputData, pBuffer, iBufferSize);
		if (retCode == M4_SUCCESS)
		{
			m4TraceLevel(3, cout << "-- subsystem attribs: " << pBuffer << endl);
			m_Attributes.Set(M4ClString(pBuffer));
		}
		else
		{
			m4TraceLevel(3, cout << "-- subsystem ids: NULL value."  << endl);
			m_Attributes.Set(M4ClString(""));
		}
		
		//-----------------------------------------------------------
		// the third dataUnit are the subsystem statistics.
		//-----------------------------------------------------------
		retCode = GetNextDataUnit(pOutputData, pBuffer, iBufferSize);
		if (retCode == M4_SUCCESS)
		{
			m4TraceHeaderLevel(3, cout << "-- subsystem statistics: " << pBuffer << endl);
			m_Statistics.Set(M4ClString(pBuffer));
		}
		else
		{
			m4TraceLevel(3, cout << "-- subsystem ids: NULL value."  << endl);
			m_Statistics.Set(M4ClString(""));
		}
		
		retCode = M4_SUCCESS;
	}
	
	//------------------------------------------------------------
	// free de dataStorage.
	//------------------------------------------------------------
	M4DataStorage::DeleteDataStorage(pInputData);
	M4DataStorage::DeleteDataStorage(pOutputData);
	return retCode;
  //## end ClACGetSubsystemInfo::GetAnswerFromServer%917513495.body
}

void ClACGetSubsystemInfo::ResetCache (const char *ai_newPath)
{
  //## begin ClACGetSubsystemInfo::ResetCache%917513496.body preserve=yes
	m4AutoTraceLevel(5, "ClACGetSubSystemInfo::ResetCache(...)");

	m_Path = ai_newPath;
	m_Ids.Reset();
	m_Attributes.Reset();
	m_Statistics.Reset();
  //## end ClACGetSubsystemInfo::ResetCache%917513496.body
}

// Additional Declarations
  //## begin ClACGetSubsystemInfo.declarations preserve=yes
  //## end ClACGetSubsystemInfo.declarations



// Class ClACGetSystemInfo 

ClACGetSystemInfo::ClACGetSystemInfo (const char *ai_AdminCommandId)
  //## begin ClACGetSystemInfo::ClACGetSystemInfo%917513497.hasinit preserve=no
  //## end ClACGetSystemInfo::ClACGetSystemInfo%917513497.hasinit
  //## begin ClACGetSystemInfo::ClACGetSystemInfo%917513497.initialization preserve=yes
  //## end ClACGetSystemInfo::ClACGetSystemInfo%917513497.initialization
{
  //## begin ClACGetSystemInfo::ClACGetSystemInfo%917513497.body preserve=yes
	m4AutoTraceLevel(5, "ClACGetSystemInfo::ClACGetSystemInfo");
	if (NULL != ai_AdminCommandId)
	{
		m_AdminCommandId = new char [strlen(ai_AdminCommandId)+1];
		if (NULL != m_AdminCommandId)
		{
			strcpy(m_AdminCommandId, ai_AdminCommandId);
		}
	}
  //## end ClACGetSystemInfo::ClACGetSystemInfo%917513497.body
}



//## Other Operations (implementation)
m4return_t ClACGetSystemInfo::Execute (ClAdminMonClient *ai_MonClient, const m4parsercommandargs_t &ai_InputData, M4ClString &ao_Answer)
{
  //## begin ClACGetSystemInfo::Execute%917339826.body preserve=yes
	m4AutoTraceLevel(5, "ClACGetSystemInfo::Execute(...)");

	//------------------------------------------------------------------------
	// parse the command arguments.
	// there is not needed parse this commands, due there is a unique command
	// for system.
	//------------------------------------------------------------------------

	//-------------------------------------------------------------------
	// This command is always sent to the server. It's not cacheable.
	//-------------------------------------------------------------------
	return GetAnswerFromServer (ai_MonClient, ao_Answer);
  //## end ClACGetSystemInfo::Execute%917339826.body
}

m4return_t ClACGetSystemInfo::GetAnswerFromServer (ClAdminMonClient *ai_MonClient, M4ClString &ao_Answer)
{
  //## begin ClACGetSystemInfo::GetAnswerFromServer%917513498.body preserve=yes
	m4AutoTraceLevel(5, "ClACGetSystemInfo::GetAnswerFromServer(...)");

	m4TraceHeaderLevel(3, cout << "ClACGetSystemInfo::GetAnswerFromServer" << endl);
	m4return_t retCode = M4_ERROR;
	//------------------------------------------------------------
	// Create the dataStorages.
	//------------------------------------------------------------
	M4DataStorage *pInputData = M4DataStorage::GetNewDataStorage();
	if (NULL == pInputData)
	{
		return M4_ERROR;
	}
	M4DataStorage *pOutputData = M4DataStorage::GetNewDataStorage();
	if (NULL == pOutputData) 
	{
		M4DataStorage::DeleteDataStorage(pInputData);
		return M4_ERROR;
	}
	
	//------------------------------------------------------------
	// Build the input dataStorage.
	//------------------------------------------------------------
	ClDataUnit *poInputDataUnit;
	pInputData->GetNewDataUnit(poInputDataUnit);
	if (NULL != poInputDataUnit)
	{
		retCode = poInputDataUnit->SetSize(strlen(m_AdminCommandId)+1);
		if (M4_SUCCESS == retCode)
		{
			strcpy(poInputDataUnit->GetBuffer(), m_AdminCommandId);
		}
	}
	else
	{
		retCode = M4_ERROR;
	}

	//------------------------------------------------------------
	// Send the command to the server.
	//------------------------------------------------------------
	if (M4_SUCCESS == retCode)
	{
		retCode = ai_MonClient->SendSyncRequest (pInputData, pOutputData);
	}

	if (M4_SUCCESS == retCode)
	{
		// Get how many dataUnits there are in the dataStorage.
		// The dataUnits are:
		//		1 - system id.
		m4uint32_t uiUnits = pOutputData->Blocks();
		
		if (1 == uiUnits)
		{			
			//-----------------------------------------------------------
			// iter over the dataStorage and get the dataUnits.
			//-----------------------------------------------------------
			m4pchar_t pBuffer;
			m4uint32_t uiBufferSize;
			m4bool_t bEncrypted;
			
			//-----------------------------------------------------------
			// the first data unit is the system id.
			//-----------------------------------------------------------
			retCode = pOutputData->Get(pBuffer, uiBufferSize, bEncrypted, 0);
			if (retCode == M4_SUCCESS)
			{
				ao_Answer = pBuffer;
			}
		}		
		else
		{
			retCode = M4_ERROR;
		}
	}
	
	//------------------------------------------------------------
	// free de dataStorage.
	//------------------------------------------------------------
	M4DataStorage::DeleteDataStorage(pInputData);
	M4DataStorage::DeleteDataStorage(pOutputData);

	return	retCode;
  //## end ClACGetSystemInfo::GetAnswerFromServer%917513498.body
}

// Additional Declarations
  //## begin ClACGetSystemInfo.declarations preserve=yes
  //## end ClACGetSystemInfo.declarations



// Class ClACGetSubsystemInfoValues 

//## begin ClACGetSubsystemInfoValues::m_Path.attr preserve=no  private: static M4ClString {UA} 
M4ClString ClACGetSubsystemInfoValues::m_Path;
//## end ClACGetSubsystemInfoValues::m_Path.attr

//## begin ClACGetSubsystemInfoValues::m_AttributeValues.role preserve=no  private: static ClAdminInfoTagCache { -> nUHAN}
deque<ClAdminInfoTagCache> ClACGetSubsystemInfoValues::m_AttributeValues;
//## end ClACGetSubsystemInfoValues::m_AttributeValues.role

//## begin ClACGetSubsystemInfoValues::m_StatisticValues.role preserve=no  private: static ClAdminInfoTagCache { -> nUHAN}
deque<ClAdminInfoTagCache> ClACGetSubsystemInfoValues::m_StatisticValues;
//## end ClACGetSubsystemInfoValues::m_StatisticValues.role

ClACGetSubsystemInfoValues::ClACGetSubsystemInfoValues (const char *ai_AdminCommandId)
  //## begin ClACGetSubsystemInfoValues::ClACGetSubsystemInfoValues%917513499.hasinit preserve=no
  //## end ClACGetSubsystemInfoValues::ClACGetSubsystemInfoValues%917513499.hasinit
  //## begin ClACGetSubsystemInfoValues::ClACGetSubsystemInfoValues%917513499.initialization preserve=yes
  //## end ClACGetSubsystemInfoValues::ClACGetSubsystemInfoValues%917513499.initialization
{
  //## begin ClACGetSubsystemInfoValues::ClACGetSubsystemInfoValues%917513499.body preserve=yes
	m4AutoTraceLevel(5, "ClACGetSubsystemInfoValues::ClACGetSubsystemInfoValues");
	if (NULL != ai_AdminCommandId)
	{
		m_AdminCommandId = new char [strlen(ai_AdminCommandId)+1];
		if (NULL != m_AdminCommandId)
		{
			strcpy(m_AdminCommandId, ai_AdminCommandId);
		}
	}
  //## end ClACGetSubsystemInfoValues::ClACGetSubsystemInfoValues%917513499.body
}



//## Other Operations (implementation)
m4return_t ClACGetSubsystemInfoValues::Execute (ClAdminMonClient *ai_MonClient, const m4parsercommandargs_t &ai_InputData, M4ClString &ao_Answer)
{
  //## begin ClACGetSubsystemInfoValues::Execute%917339827.body preserve=yes
	m4AutoTraceLevel(5, "ClACGetSubsystemInfoValues::Execute(...)");
	m4return_t retCode = M4_SUCCESS;

	//------------------------------------------------------------------------
	// parse the command arguments.
	//------------------------------------------------------------------------
	m4parsercommandargs_t::const_iterator oIterator = ai_InputData.begin();
	m4TraceHeaderLevel(4, cout << "Execute - input arguments: " << endl);

	// the first item is SUBSYSTEM or STATISTIC.
	if (ai_InputData.end() == oIterator)
	{
		return M4_ERROR;
	}
	const char* firstItem = *oIterator;
	m4TraceLevel(4, cout << "-" << firstItem);
	oIterator++;

	// the second item is always ATTRIBVALUE.
	if (ai_InputData.end() == oIterator)
	{
		return M4_ERROR;
	}
	const char *secondItem = *oIterator;
	m4TraceLevel(4, cout << "-" << secondItem);
	oIterator++;

	// the third item is the subsystem path.
	if (ai_InputData.end() == oIterator)
	{
		return M4_ERROR;
	}
	const char *subsystemPath = *oIterator;
	m4TraceLevel(4, cout << "-" << subsystemPath);
	oIterator++;

	// the fourth item is the attribute id.
	if (ai_InputData.end() == oIterator)
	{
		return M4_ERROR;
	}
	const char *attributeId = *oIterator;
	m4TraceLevel(4, cout << "-" << attributeId);
	oIterator++;

	m4TraceLevel(4, cout << "-" << endl);

	//------------------------------------------------------------
	// Get answer from cache.
	//------------------------------------------------------------
	m4bool_t isValid;
	isValid = GetAnswerFromCache (subsystemPath, firstItem, attributeId, ao_Answer);

	if (M4_FALSE == isValid)
	{
		//----------------------------------------------------------
		// Get answer from server and set the cache.
		//----------------------------------------------------------
		retCode = GetAnswerFromServer(ai_MonClient, subsystemPath);

		if (M4_SUCCESS == retCode)
		{
			//------------------------------------------------------
			// Get answer from cache.
			//------------------------------------------------------
			isValid = GetAnswerFromCache (subsystemPath, firstItem, attributeId, ao_Answer);
			if (M4_FALSE == isValid)
			{
				retCode = M4_ERROR;
			}
			else
			{
				retCode = M4_SUCCESS;
			}
		}		
	}
	return retCode;
  //## end ClACGetSubsystemInfoValues::Execute%917339827.body
}

m4return_t ClACGetSubsystemInfoValues::GetAnswerFromServer (ClAdminMonClient *ai_MonClient, const char *ai_path)
{
  //## begin ClACGetSubsystemInfoValues::GetAnswerFromServer%917513500.body preserve=yes
	m4AutoTraceLevel(5, "ClACGetSubsystemInfoValues::GetAnswerFromServer(...)");
	m4return_t retCode = M4_ERROR;

	m4TraceHeaderLevel(3, cout << "ClACGetSubsystemInfoValues::GetAnswerFromServer" << endl);
	//------------------------------------------------------------
	// Create the dataStorages.
	//------------------------------------------------------------
	M4DataStorage *pInputData = M4DataStorage::GetNewDataStorage();
	if (NULL == pInputData)
	{
		return M4_ERROR;
	}
	M4DataStorage *pOutputData = M4DataStorage::GetNewDataStorage();
	if (NULL == pOutputData) 
	{
		M4DataStorage::DeleteDataStorage(pInputData);
		return M4_ERROR;
	}
	
	//------------------------------------------------------------
	// Build the input dataStorage.
	//------------------------------------------------------------
	ClDataUnit *poInputDataUnit;

	// the first dataUnit is the command id.
	pInputData->GetNewDataUnit(poInputDataUnit);
	if (NULL != poInputDataUnit)
	{
		retCode = poInputDataUnit->SetSize(strlen(m_AdminCommandId)+1);
		if (M4_SUCCESS == retCode)
		{
			strcpy(poInputDataUnit->GetBuffer(), m_AdminCommandId);
		}
	}
	else
	{
		retCode = M4_ERROR;
	}

	// the second dataUnit is the subsystem path.
	pInputData->GetNewDataUnit(poInputDataUnit);
	if (NULL != poInputDataUnit)
	{
		retCode = poInputDataUnit->SetSize(strlen(ai_path)+1);
		if (M4_SUCCESS == retCode)
		{
			strcpy(poInputDataUnit->GetBuffer(), ai_path);
		}
	}
	else
	{
		retCode = M4_ERROR;
	}

	//------------------------------------------------------------
	// Send the command to the server.
	//------------------------------------------------------------
	if (M4_SUCCESS == retCode)
	{
		retCode = ai_MonClient->SendSyncRequest (pInputData, pOutputData);
	}
	if (M4_SUCCESS == retCode)
	{
		// The dataUnits are:
		//		1		- number of attributes.
		//		2..n	-  attribs values.
		//		n+1		- number of statistics.
		//		n+2 ..m	- statistics values
		
		//-----------------------------------------------------------
		// iter over the dataStorage and get the dataUnits.
		//-----------------------------------------------------------
		m4pchar_t pBuffer;
		int iBufferSize;
		pOutputData->InitializeIteration();
		
		//-----------------------------------------------------------
		// the first dataUnit is the number of attributes.
		//-----------------------------------------------------------
		int nAttributes = 0;
		retCode = GetNextDataUnit(pOutputData, pBuffer, iBufferSize);
		if (retCode == M4_SUCCESS)
		{
			nAttributes = atoi(pBuffer);
		}
		
		//-----------------------------------------------------------
		// Now n dataUnits with attributes values.
		//-----------------------------------------------------------
		for (int i=0; i<nAttributes; i++)
		{
			retCode = GetNextDataUnit(pOutputData, pBuffer, iBufferSize);
			if (retCode == M4_SUCCESS)
			{
				m4TraceLevel(3, cout << "-- attribute: " << i << ": " << pBuffer << endl);
				ClAdminInfoTagCache item;
				item.Set(M4ClString(pBuffer), i);
				m_AttributeValues.push_back(item);
			}
			else
			{
				break;
			}
		}
		
		//-----------------------------------------------------------
		// the next dataUnit is the number of statistics.
		//-----------------------------------------------------------
		int nStatistics = 0;
		retCode = GetNextDataUnit(pOutputData, pBuffer, iBufferSize);
		if (retCode == M4_SUCCESS)
		{
			nStatistics = atoi(pBuffer);
		}
		
		//-----------------------------------------------------------
		// Now n dataUnits with attributes values.
		//-----------------------------------------------------------
		for (int j=0; j<nStatistics; j++)
		{
			retCode = GetNextDataUnit(pOutputData, pBuffer, iBufferSize);
			if (retCode == M4_SUCCESS)
			{
				m4TraceLevel(3, cout << "-- statistics: " << j << ": " << pBuffer << endl);
				ClAdminInfoTagCache item;
				item.Set(M4ClString(pBuffer), j);
				m_StatisticValues.push_back(item);
			}
			else
			{
				break;
			}
		}			
		retCode = M4_SUCCESS;
	}		
	
	//------------------------------------------------------------
	// free de dataStorage.
	//------------------------------------------------------------
	M4DataStorage::DeleteDataStorage(pInputData);
	M4DataStorage::DeleteDataStorage(pOutputData);
	return retCode;
  //## end ClACGetSubsystemInfoValues::GetAnswerFromServer%917513500.body
}

m4bool_t ClACGetSubsystemInfoValues::GetAnswerFromCache (const char *ai_path, const char *ai_firstItem, const char *ai_attId, M4ClString &ao_Answer)
{
  //## begin ClACGetSubsystemInfoValues::GetAnswerFromCache%917513501.body preserve=yes
	m4AutoTraceLevel(5, "ClACGetSubSystemInfo::GetAnswerFromCache(...)");

	m4bool_t retCode = M4_TRUE;
	m4bool_t bFound = M4_FALSE;

	m4TraceHeaderLevel(3, cout << "ClACGetSubsystemInfoValues::GetAnswerFromCache" << endl);
	if (M4ClString(ai_path) != m_Path)
	{
		retCode = M4_FALSE;
	}

	if (M4_FALSE != retCode)
	{
		deque<ClAdminInfoTagCache>::iterator it;
		if (!strcmp(ai_firstItem, "SUBSYSTEM"))
		{
			for (it = m_AttributeValues.begin(); it != m_AttributeValues.end(); it++)
			{
				ClAdminInfoTagCache& item = *it;
				if (atoi(ai_attId) == item.GetId())
				{
					retCode = item.Get(ao_Answer);
					bFound = M4_TRUE;
					break;
				}
			}
		}
		else // STATISTIC
		{
			for (it = m_StatisticValues.begin(); it != m_StatisticValues.end(); it++)
			{
				ClAdminInfoTagCache& item = *it;
				if (atoi(ai_attId) == item.GetId())
				{
					retCode = item.Get(ao_Answer);
					bFound = M4_TRUE;
					break;
				}
			}
		}
	}

	// Bug 0084794: if we have not found statistics ort attribute values, return false.
	if (bFound == M4_FALSE)
	{
		retCode = M4_FALSE;
	}

	if (M4_FALSE == retCode)
	{
		ResetCache(ai_path);
	}
	
	return retCode;
  //## end ClACGetSubsystemInfoValues::GetAnswerFromCache%917513501.body
}

void ClACGetSubsystemInfoValues::ResetCache (const char *ai_newPath)
{
  //## begin ClACGetSubsystemInfoValues::ResetCache%917513502.body preserve=yes
	m4AutoTraceLevel(5, "ClACGetSubsystemInfoValues::ResetCache(...)");

	m_Path = ai_newPath;
	m_AttributeValues.clear();
	m_StatisticValues.clear();
  //## end ClACGetSubsystemInfoValues::ResetCache%917513502.body
}

// Additional Declarations
  //## begin ClACGetSubsystemInfoValues.declarations preserve=yes
  //## end ClACGetSubsystemInfoValues.declarations



// Class ClACGetCommandsInfo 

//## begin ClACGetCommandsInfo::m_Path.attr preserve=no  private: static M4ClString {UA} 
M4ClString ClACGetCommandsInfo::m_Path;
//## end ClACGetCommandsInfo::m_Path.attr

//## begin ClACGetCommandsInfo::m_Commands.attr preserve=no  private: static ClAdminInfoTagCache {UA} 
ClAdminInfoTagCache ClACGetCommandsInfo::m_Commands;
//## end ClACGetCommandsInfo::m_Commands.attr

//## begin ClACGetCommandsInfo::m_CommandParams.role preserve=no  private: static ClAdminInfoTagCache { -> nUHAN}
deque<ClAdminInfoTagCache> ClACGetCommandsInfo::m_CommandParams;
//## end ClACGetCommandsInfo::m_CommandParams.role

ClACGetCommandsInfo::ClACGetCommandsInfo (const char *ai_AdminCommandId)
  //## begin ClACGetCommandsInfo::ClACGetCommandsInfo%917513503.hasinit preserve=no
  //## end ClACGetCommandsInfo::ClACGetCommandsInfo%917513503.hasinit
  //## begin ClACGetCommandsInfo::ClACGetCommandsInfo%917513503.initialization preserve=yes
  //## end ClACGetCommandsInfo::ClACGetCommandsInfo%917513503.initialization
{
  //## begin ClACGetCommandsInfo::ClACGetCommandsInfo%917513503.body preserve=yes
	m4AutoTraceLevel(5, "ClACGetCommandsInfo::ClACGetCommandsInfo");
	if (NULL != ai_AdminCommandId)
	{
		m_AdminCommandId = new char [strlen(ai_AdminCommandId)+1];
		if (NULL != m_AdminCommandId)
		{
			strcpy(m_AdminCommandId, ai_AdminCommandId);
		}
	}
  //## end ClACGetCommandsInfo::ClACGetCommandsInfo%917513503.body
}



//## Other Operations (implementation)
m4return_t ClACGetCommandsInfo::Execute (ClAdminMonClient *ai_MonClient, const m4parsercommandargs_t &ai_InputData, M4ClString &ao_Answer)
{
  //## begin ClACGetCommandsInfo::Execute%917339828.body preserve=yes
	m4AutoTraceLevel(5, "ClACGetCommandsInfo::Execute(...)");
	m4return_t retCode = M4_SUCCESS;

	//------------------------------------------------------------------------
	// parse the command arguments.
	//------------------------------------------------------------------------
	m4parsercommandargs_t::const_iterator oIterator = ai_InputData.begin();
	m4TraceHeaderLevel(4, cout << "Execute - input arguments: " << endl);

	// the first item is COMMANDS or COMMAND.
	if (ai_InputData.end() == oIterator)
	{
		return M4_ERROR;
	}
	const char* firstItem = *oIterator;
	m4TraceLevel(4, cout << "-" << firstItem);
	oIterator++;

	// the second item is PARAMS or subsystem path.
	if (ai_InputData.end() == oIterator)
	{
		return M4_ERROR;
	}
	const char *secondItem = *oIterator;
	m4TraceLevel(4, cout << "-" << secondItem);
	oIterator++;

	// the third and fourth items depends second item.
	const char *thirdItem = NULL; 
	const char *fourthItem = NULL;
	if (ai_InputData.end() != oIterator)
	{
		thirdItem = *oIterator;
		m4TraceLevel(4, cout << "-" << thirdItem);
		oIterator++;

		if (ai_InputData.end() == oIterator)
		{
			return M4_ERROR;
		}
		fourthItem = *oIterator;
		m4TraceLevel(4, cout << "-" << fourthItem);
		oIterator++;
	}

	m4TraceLevel(4, cout << "-" << endl);

	m4bool_t isValid;

// Para informacion de comandos vamos a ir siempre a preguntar al server
/*
	//------------------------------------------------------------
	// Get answer from cache.
	//------------------------------------------------------------
	
//	isValid = GetAnswerFromCache (firstItem, secondItem, thirdItem, fourthItem, ao_Answer);

	if (M4_TRUE)
	{
*/
		// get the subsystem path
		const char *subsystemPath;
		if (!strcmp(firstItem, "COMMANDS"))
		{
			subsystemPath = secondItem;
		}
		else
		{
			subsystemPath = thirdItem;
		}
		ResetCache(subsystemPath);
		//----------------------------------------------------------
		// Get answer from server and set the cache.
		//----------------------------------------------------------
		retCode = GetAnswerFromServer(ai_MonClient, subsystemPath);

		if (M4_SUCCESS == retCode)
		{
			//------------------------------------------------------
			// Get answer from cache.
			//------------------------------------------------------
			isValid = GetAnswerFromCache (firstItem, secondItem, thirdItem, fourthItem, ao_Answer);
			if (M4_FALSE == isValid)
			{
				retCode = M4_ERROR;
			}
			else
			{
				retCode = M4_SUCCESS;
			}
	//	}		
	}
	return retCode;
  //## end ClACGetCommandsInfo::Execute%917339828.body
}

m4return_t ClACGetCommandsInfo::GetAnswerFromServer (ClAdminMonClient *ai_MonClient, const char *ai_path)
{
  //## begin ClACGetCommandsInfo::GetAnswerFromServer%917513504.body preserve=yes
	m4AutoTraceLevel(5, "ClACGetCommandsInfo::GetAnswerFromServer(...)");
	m4return_t retCode = M4_ERROR;

	m4TraceHeaderLevel(3, cout << "ClACGetCommandsInfo::GetAnswerFromServer" << endl);
	//------------------------------------------------------------
	// Create the dataStorages.
	//------------------------------------------------------------
	M4DataStorage *pInputData = M4DataStorage::GetNewDataStorage();
	if (NULL == pInputData)
	{
		return M4_ERROR;
	}
	M4DataStorage *pOutputData = M4DataStorage::GetNewDataStorage();
	if (NULL == pOutputData) 
	{
		M4DataStorage::DeleteDataStorage(pInputData);
		return M4_ERROR;
	}
	
	//------------------------------------------------------------
	// Build the input dataStorage.
	//------------------------------------------------------------
	ClDataUnit *poInputDataUnit;

	// the first dataUnit is the command id.
	pInputData->GetNewDataUnit(poInputDataUnit);
	if (NULL != poInputDataUnit)
	{
		retCode = poInputDataUnit->SetSize(strlen(m_AdminCommandId)+1);
		if (M4_SUCCESS == retCode)
		{
			strcpy(poInputDataUnit->GetBuffer(), m_AdminCommandId);
		}
	}
	else
	{
		retCode = M4_ERROR;
	}

	// the second dataUnit is the subsystem path.
	pInputData->GetNewDataUnit(poInputDataUnit);
	if (NULL != poInputDataUnit)
	{
		retCode = poInputDataUnit->SetSize(strlen(ai_path)+1);
		if (M4_SUCCESS == retCode)
		{
			strcpy(poInputDataUnit->GetBuffer(), ai_path);
		}
	}
	else
	{
		retCode = M4_ERROR;
	}

	//------------------------------------------------------------
	// Send the command to the server.
	//------------------------------------------------------------
	if (M4_SUCCESS == retCode)
	{
		retCode = ai_MonClient->SendSyncRequest (pInputData, pOutputData);
	}
	if (M4_SUCCESS == retCode)
	{
		// The dataUnits in the dataStorage are:
		//		1		- subsystem commands.
		//		n		- pairs of dataUnits (commandId, commandParams)
		//				  for each command that has parameters.
		
		//-----------------------------------------------------------
		// iter over the dataStorage and get the dataUnits.
		//-----------------------------------------------------------
		m4pchar_t pBuffer, pBuffer1;
		int iBufferSize;
		pOutputData->InitializeIteration();
		
		//-----------------------------------------------------------
		// the first dataUnit are the subsystem commands..
		//-----------------------------------------------------------
		retCode = GetNextDataUnit(pOutputData, pBuffer, iBufferSize);
		if (retCode == M4_SUCCESS)
		{
			m4TraceLevel(3, cout << "-- commandsInfo: " << pBuffer << endl);
			m_Commands.Set(M4ClString(pBuffer));
		}
		
		while (M4_SUCCESS == retCode)
		{
			//-----------------------------------------------------------
			// Now n pairs of dataUnits commandId and commandParams.
			//-----------------------------------------------------------
			ClAdminInfoTagCache item;
			retCode = GetNextDataUnit(pOutputData, pBuffer, iBufferSize);
			if (M4_SUCCESS == retCode)
			{
				retCode = GetNextDataUnit(pOutputData, pBuffer1, iBufferSize);
				if (M4_SUCCESS == retCode)
				{
					m4TraceLevel(3, cout << "-- command: " << atoi(pBuffer) << ". Params: " << pBuffer1 << endl);
					item.Set(M4ClString(pBuffer1), atoi(pBuffer));
					m_CommandParams.push_back(item);
				}
			}
		}
		retCode = M4_SUCCESS;
	}
	
	//------------------------------------------------------------
	// free de dataStorage.
	//------------------------------------------------------------
	M4DataStorage::DeleteDataStorage(pInputData);
	M4DataStorage::DeleteDataStorage(pOutputData);
	return retCode;
  //## end ClACGetCommandsInfo::GetAnswerFromServer%917513504.body
}

m4bool_t ClACGetCommandsInfo::GetAnswerFromCache (const char *ai_firstItem, const char *ai_secondItem, const char *ai_thirdItem, const char *ai_fourthItem, M4ClString &ao_Answer)
{
  //## begin ClACGetCommandsInfo::GetAnswerFromCache%917513505.body preserve=yes
	m4AutoTraceLevel(5, "ClACGetCommandsInfo::GetAnswerFromCache(...)");

	m4TraceHeaderLevel(3, cout << "ClACGetCommandsInfo::GetAnswerFromCache" << endl);
	// get subsystem path.
	const char* subsystemPath;
	if (!strcmp(ai_firstItem, "COMMANDS"))
	{
		subsystemPath = ai_secondItem;
	}
	else
	{
		subsystemPath = ai_thirdItem;
	}

	if (M4ClString(subsystemPath) != m_Path)
	{
		ResetCache(subsystemPath);
		return M4_FALSE;
	}

	if (!strcmp(ai_firstItem, "COMMANDS"))
	{
		return m_Commands.Get(ao_Answer);
	}
	else
	{
		int commandId = atoi(ai_fourthItem);
		deque<ClAdminInfoTagCache>::iterator it;
		for (it = m_CommandParams.begin(); it != m_CommandParams.end(); it++)
		{
			ClAdminInfoTagCache& item = *it;
			if (item.GetId() == commandId)
			{
				//MOD FJSM 
				if (item.Get(ao_Answer)==M4_FALSE) {
					ResetCache(subsystemPath);
					return M4_FALSE;
				}
				else
					return M4_TRUE;
				//END MOD FJSM			
			}
		}
	}

	return M4_TRUE;
  //## end ClACGetCommandsInfo::GetAnswerFromCache%917513505.body
}

void ClACGetCommandsInfo::ResetCache (const char *ai_newPath)
{
  //## begin ClACGetCommandsInfo::ResetCache%917513506.body preserve=yes
	m4AutoTraceLevel(5, "ClACGetCommandsInfo::ResetCache(...)");

	m_Path = ai_newPath;
	m_Commands.Reset();
	m_CommandParams.clear();
  //## end ClACGetCommandsInfo::ResetCache%917513506.body
}

// Additional Declarations
  //## begin ClACGetCommandsInfo.declarations preserve=yes
  //## end ClACGetCommandsInfo.declarations



// Class ClACShutdown 

ClACShutdown::ClACShutdown (const char *ai_AdminCommandId)
  //## begin ClACShutdown::ClACShutdown%917513507.hasinit preserve=no
  //## end ClACShutdown::ClACShutdown%917513507.hasinit
  //## begin ClACShutdown::ClACShutdown%917513507.initialization preserve=yes
  //## end ClACShutdown::ClACShutdown%917513507.initialization
{
  //## begin ClACShutdown::ClACShutdown%917513507.body preserve=yes
	m4AutoTraceLevel(5, "ClACShutdown::ClACShutdown");
	if (NULL != ai_AdminCommandId)
	{
		m_AdminCommandId = new char [strlen(ai_AdminCommandId)+1];
		if (NULL != m_AdminCommandId)
		{
			strcpy(m_AdminCommandId, ai_AdminCommandId);
		}
	}
  //## end ClACShutdown::ClACShutdown%917513507.body
}



//## Other Operations (implementation)
m4return_t ClACShutdown::Execute (ClAdminMonClient *ai_MonClient, const m4parsercommandargs_t &ai_InputData, M4ClString &ao_Answer)
{
  //## begin ClACShutdown::Execute%917339829.body preserve=yes
	m4AutoTraceLevel(5, "ClACShutdown::Execute(...)");
	//------------------------------------------------------------------------
	// parse the command arguments.
	// there is not needed parse this commands, due there is a unique command
	// for system.
	//------------------------------------------------------------------------

	//-------------------------------------------------------------------
	// This command is always sent to the server. It's not cacheable.
	//-------------------------------------------------------------------
	return GetAnswerFromServer (ai_MonClient, ao_Answer);
  //## end ClACShutdown::Execute%917339829.body
}

m4return_t ClACShutdown::GetAnswerFromServer (ClAdminMonClient *ai_MonClient, M4ClString &ao_Answer)
{
  //## begin ClACShutdown::GetAnswerFromServer%917513508.body preserve=yes
	m4AutoTraceLevel(5, "ClACShutdown::GetAnswerFromServer(...)");

	m4TraceHeaderLevel(3, cout << "ClACShutdown::GetAnswerFromServer" << endl);
	m4return_t retCode = M4_ERROR;
	//------------------------------------------------------------
	// Create the dataStorages.
	//------------------------------------------------------------
	M4DataStorage *pInputData = M4DataStorage::GetNewDataStorage();
	if (NULL == pInputData)
	{
		return M4_ERROR;
	}
	M4DataStorage *pOutputData = M4DataStorage::GetNewDataStorage();
	if (NULL == pOutputData) 
	{
		M4DataStorage::DeleteDataStorage(pInputData);
		return M4_ERROR;
	}
	
	//------------------------------------------------------------
	// Build the input dataStorage.
	//------------------------------------------------------------
	ClDataUnit *poInputDataUnit;
	pInputData->GetNewDataUnit(poInputDataUnit);
	if (NULL != poInputDataUnit)
	{
		retCode = poInputDataUnit->SetSize(strlen(m_AdminCommandId)+1);
		if (M4_SUCCESS == retCode)
		{
			strcpy(poInputDataUnit->GetBuffer(), m_AdminCommandId);
		}
	}
	else
	{
		retCode = M4_ERROR;
	}

	//------------------------------------------------------------
	// Send the command to the server.
	//------------------------------------------------------------
	if (M4_SUCCESS == retCode)
	{
		retCode = ai_MonClient->SendSyncRequest (pInputData, pOutputData);
	}
	
	//------------------------------------------------------------
	// free de dataStorage.
	//------------------------------------------------------------
	M4DataStorage::DeleteDataStorage(pInputData);
	M4DataStorage::DeleteDataStorage(pOutputData);

	return	retCode;
  //## end ClACShutdown::GetAnswerFromServer%917513508.body
}

// Additional Declarations
  //## begin ClACShutdown.declarations preserve=yes
  //## end ClACShutdown.declarations



// Class ClACExecuteCommand 

ClACExecuteCommand::ClACExecuteCommand (const char *ai_AdminCommandId)
  //## begin ClACExecuteCommand::ClACExecuteCommand%917513509.hasinit preserve=no
  //## end ClACExecuteCommand::ClACExecuteCommand%917513509.hasinit
  //## begin ClACExecuteCommand::ClACExecuteCommand%917513509.initialization preserve=yes
  //## end ClACExecuteCommand::ClACExecuteCommand%917513509.initialization
{
  //## begin ClACExecuteCommand::ClACExecuteCommand%917513509.body preserve=yes
	m4AutoTraceLevel(5, "ClACExecuteCommand::ClACExecuteCommand");
	if (NULL != ai_AdminCommandId)
	{
		m_AdminCommandId = new char [strlen(ai_AdminCommandId)+1];
		if (NULL != m_AdminCommandId)
		{
			strcpy(m_AdminCommandId, ai_AdminCommandId);
		}
	}
  //## end ClACExecuteCommand::ClACExecuteCommand%917513509.body
}



//## Other Operations (implementation)
m4return_t ClACExecuteCommand::Execute (ClAdminMonClient *ai_MonClient, const m4parsercommandargs_t &ai_InputData, M4ClString &ao_Answer)
{
  //## begin ClACExecuteCommand::Execute%917339830.body preserve=yes
	m4AutoTraceLevel(5, "ClACExecuteCommand::Execute(...)");

	m4parsercommandargs_t::const_iterator oIterator = ai_InputData.begin();
	m4TraceHeaderLevel(4, cout << "Execute - input arguments: " << endl);

	// the first item is COMMAND.
	if (ai_InputData.end() == oIterator)
	{
		return M4_ERROR;
	}
	const char* firstItem = *oIterator;
	m4TraceLevel(4, cout << "-" << firstItem);
	oIterator++;

	// the second item is subsystemPath.
	if (ai_InputData.end() == oIterator)
	{
		return M4_ERROR;
	}
	const char *subsystemPath = *oIterator;
	m4TraceLevel(4, cout << "-" << subsystemPath);
	oIterator++;

	// the third item is the commandId.
	if (ai_InputData.end() == oIterator)
	{
		return M4_ERROR;
	}
	const char *commandId = *oIterator;
	m4TraceLevel(4, cout << "-" << commandId);
	oIterator++;

	// now, all items are pairs paramId-paramValue.
	int nItems=0;
	const char* items[M4_MAX_NUM_ITEM];
	while (ai_InputData.end() != oIterator)
	{
		if(nItems<M4_MAX_NUM_ITEM)
		{
			items[nItems] = *oIterator;
			m4TraceLevel(4, cout << "-" << items[nItems]);
			nItems++;
			oIterator++;
		}
		else
		{
			m4Trace(cerr << "Sobrepasado M4_MAX_NUM_ITEM"<<endl);
			return M4_ERROR;
			break;
		}
	}
	m4TraceLevel(4, cout << "-" << endl);

	//-------------------------------------------------------------------
	// This command is always sent to the server. It's not cacheable.
	//-------------------------------------------------------------------
	return GetAnswerFromServer (ai_MonClient, subsystemPath, commandId, items, nItems, ao_Answer);
  //## end ClACExecuteCommand::Execute%917339830.body
}

m4return_t ClACExecuteCommand::GetAnswerFromServer (ClAdminMonClient *ai_MonClient, const char *ai_path, const char *ai_commandId, const char **ai_items, int ai_nItems, M4ClString &ao_Answer)
{
  //## begin ClACExecuteCommand::GetAnswerFromServer%917513510.body preserve=yes
	m4AutoTraceLevel(5, "ClACExecuteCommand::GetAnswerFromServer(...)");
	m4return_t retCode = M4_ERROR;

	//------------------------------------------------------------
	// Create the dataStorages.
	//------------------------------------------------------------
	M4DataStorage *pInputData = M4DataStorage::GetNewDataStorage();
	if (NULL == pInputData)
	{
		return M4_ERROR;
	}
	M4DataStorage *pOutputData = M4DataStorage::GetNewDataStorage();
	if (NULL == pOutputData) 
	{
		M4DataStorage::DeleteDataStorage(pInputData);
		return M4_ERROR;
	}
	
	//------------------------------------------------------------
	// Build the input dataStorage.
	//------------------------------------------------------------
	ClDataUnit *poInputDataUnit;

	// the first dataUnit is the command id.
	pInputData->GetNewDataUnit(poInputDataUnit);
	if (NULL != poInputDataUnit)
	{
		retCode = poInputDataUnit->SetSize(strlen(m_AdminCommandId)+1);
		if (M4_SUCCESS == retCode)
		{
			strcpy(poInputDataUnit->GetBuffer(), m_AdminCommandId);
		}
	}
	else
	{
		retCode = M4_ERROR;
	}

	// the second dataUnit is the subsystem path.
	pInputData->GetNewDataUnit(poInputDataUnit);
	if (NULL != poInputDataUnit)
	{
		retCode = poInputDataUnit->SetSize(strlen(ai_path)+1);
		if (M4_SUCCESS == retCode)
		{
			strcpy(poInputDataUnit->GetBuffer(), ai_path);
		}
	}
	else
	{
		retCode = M4_ERROR;
	}

	// the third dataUnit is the commandId.
	pInputData->GetNewDataUnit(poInputDataUnit);
	if (NULL != poInputDataUnit)
	{
		retCode = poInputDataUnit->SetSize(strlen(ai_commandId)+1);
		if (M4_SUCCESS == retCode)
		{
			strcpy(poInputDataUnit->GetBuffer(), ai_commandId);
		}
	}
	else
	{
		retCode = M4_ERROR;
	}

	// the next dataUnits are the command parameters.
	M4ClString auxStr;
	for (int i=0; i<ai_nItems; i++)
	{
		m4pchar_t item = (m4pchar_t)ai_items[i];
		if(item[0]==1) 
		{
			if(!auxStr.empty())
			{
				pInputData->GetNewDataUnit(poInputDataUnit);
				if (NULL != poInputDataUnit)
				{
					retCode = poInputDataUnit->SetSize(auxStr.size()+1);
					if (M4_SUCCESS == retCode)
					{
						strcpy(poInputDataUnit->GetBuffer(), auxStr.c_str());
					}
					auxStr="";
				}
				else
				{
					retCode = M4_ERROR;
					break;
				}
			}			
			
			
			pInputData->GetNewDataUnit(poInputDataUnit);
			if (NULL != poInputDataUnit)
			{
				retCode = poInputDataUnit->SetSize(strlen(item));
				if (M4_SUCCESS == retCode)
				{
					strcpy(poInputDataUnit->GetBuffer(), item+1);
				}
			}
			else
			{
				retCode = M4_ERROR;
				break;
			}
		}
		else
		{
			if(!auxStr.empty())
				auxStr+=" ";
			auxStr+=item;
		}
	}
	if(!auxStr.empty())
	{
		pInputData->GetNewDataUnit(poInputDataUnit);
		if (NULL != poInputDataUnit)
		{
			retCode = poInputDataUnit->SetSize(auxStr.size()+1);
			if (M4_SUCCESS == retCode)
			{
				strcpy(poInputDataUnit->GetBuffer(), auxStr.c_str());
			}
			auxStr="";
		}
		else
		{
			retCode = M4_ERROR;

		}
	}

	//------------------------------------------------------------
	// Send the command to the server.
	//------------------------------------------------------------
	if (M4_SUCCESS == retCode)
	{
		retCode = ai_MonClient->SendSyncRequest (pInputData, pOutputData);
	}
	
	//------------------------------------------------------------
	// free de dataStorage.
	//------------------------------------------------------------
	M4DataStorage::DeleteDataStorage(pInputData);

	ClDataUnitInterface *TheDUI;
	m4int_t counter=0;
	m4bool_t bAnyAnswer=M4_FALSE;
	m4pchar_t thebuffer;

	m4char_t pzCommandSeparator[2];
	sprintf(pzCommandSeparator,"%c",M4_ADMIN_COMMAND_SEPARATOR);
	m4char_t pzParamSeparator[2];
	sprintf(pzParamSeparator,"%c",M4_ADMIN_COMMAND_PARAM_SEPARATOR);
	while(M4_ERROR!=pOutputData->GetNext(TheDUI))
	{
		if(bAnyAnswer)
			ao_Answer+=pzParamSeparator;
		bAnyAnswer=M4_TRUE;
		ClDataUnit *TheDU=(ClDataUnit *)TheDUI;
		thebuffer =TheDU->GetBuffer();
		if(TheDU->GetSize() && thebuffer)
		{
			ao_Answer+=thebuffer;
			ao_Answer+=pzParamSeparator;
		}
		if(M4_ERROR!=pOutputData->GetNext(TheDUI))
		{
			TheDU=(ClDataUnit *)TheDUI;
			thebuffer =TheDU->GetBuffer();
			
			if(TheDU->GetSize() && thebuffer)
			{
				ao_Answer+=thebuffer;
				counter++;
			}
		}
		else
			retCode=M4_ERROR;
	}
	if(bAnyAnswer)
		ao_Answer+=pzCommandSeparator;
	M4DataStorage::DeleteDataStorage(pOutputData);
	return retCode;
  //## end ClACExecuteCommand::GetAnswerFromServer%917513510.body
}

// Additional Declarations
  //## begin ClACExecuteCommand.declarations preserve=yes
  //## end ClACExecuteCommand.declarations



//## begin module.epilog preserve=yes
//## end module.epilog
