//## begin module.cm preserve=no
//## end module.cm

//## begin module.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            
//	 File:              adminpduexec.cpp
//	 Project:           m4adminsrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines the objects to process the administration PDUs
//
//
//	==============================================================================
//## end module.cp

//## Module: adminPDUExecutor; Package body
//## Subsystem: CPM4AdminSrv
//## Source file: d:\marcoscp\M4AdminSrv\version\src\adminpduexec.cpp

//## begin module.additionalIncludes preserve=no
//## end module.additionalIncludes

//## begin module.includes preserve=yes
#include "m4stl.hpp"
#include <stdlib.h>
#include "m4trace.hpp"
#include "cladministrator.hpp"
#include "adminmoncmdsid.hpp"
#include "clienttag.hpp"
#include "adminlogerr.hpp"
#include <clssimplementacion.hpp>
#include "m4servenv.hpp"
#include "m4props.hpp"
#include <systeminteface.hpp>
#include <adminpduexec.hpp>
#include "m4buildversion.hpp"

extern int	M4IsUnicode( void ) ;


//## begin module.declarations preserve=no
//## end module.declarations

//## begin module.additionalDeclarations preserve=yes
//## end module.additionalDeclarations


// Class ClAdminSetrolePDU 

ClAdminSetrolePDU::ClAdminSetrolePDU (ClCSAdministrator *ai_poAdministrator)
  //## begin ClAdminSetrolePDU::ClAdminSetrolePDU%918150018.hasinit preserve=no
  //## end ClAdminSetrolePDU::ClAdminSetrolePDU%918150018.hasinit
  //## begin ClAdminSetrolePDU::ClAdminSetrolePDU%918150018.initialization preserve=yes
  :ClAdminPDUExecutor(ai_poAdministrator)
  //## end ClAdminSetrolePDU::ClAdminSetrolePDU%918150018.initialization
{
  //## begin ClAdminSetrolePDU::ClAdminSetrolePDU%918150018.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminSetrolePDU::ClAdminSetrolePDU");
  //## end ClAdminSetrolePDU::ClAdminSetrolePDU%918150018.body
}



//## Other Operations (implementation)
m4return_t ClAdminSetrolePDU::Execute (ClPDU *ai_poInputPDU)
{
  //## begin ClAdminSetrolePDU::Execute%897639337.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminSetrolePDU::Execute");
	m4return_t retCode = M4_SUCCESS;

	m4TraceLevel(1, cout << "ClAdminSetrolePDU::Execute() " << endl);

	/*
	 * Actually we check the role in the authentication PDU, due to
	 * the role is hardcoded in the administrator object.
	 */
	  
	//------------------------------------------------------------------------
	// answer with an ACK.
	//------------------------------------------------------------------------
	return SendAckPDU (ai_poInputPDU->GetRequestId());

  //## end ClAdminSetrolePDU::Execute%897639337.body
}

// Additional Declarations
  //## begin ClAdminSetrolePDU.declarations preserve=yes
  //## end ClAdminSetrolePDU.declarations



// Class ClAdminDisconnectPDU 

ClAdminDisconnectPDU::ClAdminDisconnectPDU (ClCSAdministrator *ai_poAdministrator)
  //## begin ClAdminDisconnectPDU::ClAdminDisconnectPDU%918150017.hasinit preserve=no
  //## end ClAdminDisconnectPDU::ClAdminDisconnectPDU%918150017.hasinit
  //## begin ClAdminDisconnectPDU::ClAdminDisconnectPDU%918150017.initialization preserve=yes
  :ClAdminPDUExecutor(ai_poAdministrator)
  //## end ClAdminDisconnectPDU::ClAdminDisconnectPDU%918150017.initialization
{
  //## begin ClAdminDisconnectPDU::ClAdminDisconnectPDU%918150017.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminDisconnectPDU::ClAdminDisconnectPDU");
  //## end ClAdminDisconnectPDU::ClAdminDisconnectPDU%918150017.body
}



//## Other Operations (implementation)
m4return_t ClAdminDisconnectPDU::Execute (ClPDU *ai_poInputPDU)
{
  //## begin ClAdminDisconnectPDU::Execute%898072222.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminDisconnectPDU::Execute");
	m4return_t retCode = M4_SUCCESS;
	
	m4TraceLevel(1, cout << "ClAdminDisconnectPDU::Execute() " << endl);

	//------------------------------------------------------------------------
	// check administrator.
	//------------------------------------------------------------------------
	if (NULL == m_poAdministrator)
	{
		return M4_ERROR;
	}

	//------------------------------------------------------------------------
	// answer with an ACK.
	//------------------------------------------------------------------------
	SendAckPDU (ai_poInputPDU->GetRequestId());

	//-------------------------------------------------------------------
	// close conexion and terminate the thread.
	//-------------------------------------------------------------------
	//m_poAdministrator->m_poTSAP->Close();
	m_poAdministrator->Terminate(M4_TRUE);

	return retCode;
  //## end ClAdminDisconnectPDU::Execute%898072222.body
}

// Additional Declarations
  //## begin ClAdminDisconnectPDU.declarations preserve=yes
  //## end ClAdminDisconnectPDU.declarations



// Class ClAdminUnknownPDU 

ClAdminUnknownPDU::ClAdminUnknownPDU (ClCSAdministrator *ai_poAdministrator)
  //## begin ClAdminUnknownPDU::ClAdminUnknownPDU%918150016.hasinit preserve=no
  //## end ClAdminUnknownPDU::ClAdminUnknownPDU%918150016.hasinit
  //## begin ClAdminUnknownPDU::ClAdminUnknownPDU%918150016.initialization preserve=yes
  :ClAdminPDUExecutor(ai_poAdministrator)
  //## end ClAdminUnknownPDU::ClAdminUnknownPDU%918150016.initialization
{
  //## begin ClAdminUnknownPDU::ClAdminUnknownPDU%918150016.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminUnknownPDU::ClAdminUnknownPDU");
  //## end ClAdminUnknownPDU::ClAdminUnknownPDU%918150016.body
}



//## Other Operations (implementation)
m4return_t ClAdminUnknownPDU::Execute (ClPDU *ai_poInputPDU)
{
  //## begin ClAdminUnknownPDU::Execute%897639338.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminDefaultPDU::Execute");

	m4TraceLevel(1, cout << "ClAdminUnknownPDU::Execute() " << endl);
		
	//------------------------------------------------------------------------
	// answer with an ERROR.
	//------------------------------------------------------------------------
	return SendErrorPDU (ai_poInputPDU->GetRequestId());
  //## end ClAdminUnknownPDU::Execute%897639338.body
}

// Additional Declarations
  //## begin ClAdminUnknownPDU.declarations preserve=yes
  //## end ClAdminUnknownPDU.declarations



// Class ClAdminRequestPDU 

ClAdminRequestPDU::ClAdminRequestPDU (ClCSAdministrator *ai_poAdministrator)
  //## begin ClAdminRequestPDU::ClAdminRequestPDU%918150015.hasinit preserve=no
  //## end ClAdminRequestPDU::ClAdminRequestPDU%918150015.hasinit
  //## begin ClAdminRequestPDU::ClAdminRequestPDU%918150015.initialization preserve=yes
  :ClAdminPDUExecutor(ai_poAdministrator)
  //## end ClAdminRequestPDU::ClAdminRequestPDU%918150015.initialization
{
  //## begin ClAdminRequestPDU::ClAdminRequestPDU%918150015.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminRequestPDU::ClAdminRequestPDU");
  //## end ClAdminRequestPDU::ClAdminRequestPDU%918150015.body
}



//## Other Operations (implementation)
m4return_t ClAdminRequestPDU::Execute (ClPDU *ai_poInputPDU)
{
  //## begin ClAdminRequestPDU::Execute%897639333.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminRequestPDU::Execute");
	m4return_t retCode;

	//-----------------------------------------------------------------------
	// check input arguments.
	//-----------------------------------------------------------------------
	if (ai_poInputPDU == NULL)
	{
		return M4_ERROR;
	}

	//------------------------------------------------------------------------
	// check administrator.
	//------------------------------------------------------------------------
	if (NULL == m_poAdministrator)
	{
		return M4_ERROR;
	}

	//-----------------------------------------------------------------------
	// Get the admin command id.
	//-----------------------------------------------------------------------
	m4pchar_t commandId = GetAdminCommandId(ai_poInputPDU);
	if (NULL == commandId)
	{
		//------------------------------------------------------------------------
		// answer with an ERROR.
		//------------------------------------------------------------------------
		return SendErrorPDU(ai_poInputPDU->GetRequestId());
	}
	
	//-----------------------------------------------------------------------
	// parse the command type and get the answer for it.
	//-----------------------------------------------------------------------
	if (!strcmp(commandId, M4_ADM_GET_SYSTEM_INFO))
	{
		retCode = GetSystemInfo(ai_poInputPDU);
	//	return retCode;
	}
	else if (!strcmp(commandId, M4_ADM_GET_SUBSYSTEM_INFO))
	{
		retCode = GetSubsystemInfo(ai_poInputPDU);
	//	return retCode;
	}
	else if (!strcmp(commandId, M4_ADM_GET_SUBSYSTEM_INFO_VALUES))
	{
		retCode = GetSubsystemInfoValues(ai_poInputPDU);
	//	return retCode;
	}
	else if (!strcmp(commandId, M4_ADM_GET_COMMANDS_INFO))
	{
		retCode = GetCommandsInfo(ai_poInputPDU);
	//	return retCode;
	}
	else if (!strcmp(commandId, M4_ADM_EXECUTE_COMMAND))
	{
		retCode = ExecuteCommand(ai_poInputPDU);
	//	return retCode;
	}
	else if (!strcmp(commandId, M4_ADM_SHUTDOWN))
	{
		ShutDown(ai_poInputPDU);
		retCode= M4_SUCCESS;
	}
	if(retCode==M4_ERROR)
	{
		//------------------------------------------------------------------------
		// answer with an ERROR.
		//------------------------------------------------------------------------
		m4TraceHeaderLevel(2, cout << "Command type not recognized." << endl);
		retCode = SendErrorPDU(ai_poInputPDU->GetRequestId());
	}
	return retCode;
  //## end ClAdminRequestPDU::Execute%897639333.body
}

const m4pchar_t ClAdminRequestPDU::GetAdminCommandId (ClPDU *ai_poInputPDU)
{
  //## begin ClAdminRequestPDU::GetAdminCommandId%899816501.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminRequestPDU::GetAdminCommandId");

	//-----------------------------------------------------------------------
	// check input argument.
	//-----------------------------------------------------------------------
	if (ai_poInputPDU == NULL)
	{
		return NULL;
	}

	//-----------------------------------------------------------------------
	// get input dataStorage.
	//-----------------------------------------------------------------------
	M4DataStorage *poInputDataStorage = ai_poInputPDU->GetDataStorage();
	if (poInputDataStorage == NULL)
	{
		return NULL;
	}

	//-----------------------------------------------------------------------
	// the first dataUnit is the command id.
	//-----------------------------------------------------------------------
	int iDataSize;
	m4pchar_t commandId;
	poInputDataStorage->InitializeIteration();
	m4return_t retCode = GetNextDataUnit(poInputDataStorage, commandId, iDataSize);
	if ((M4_ERROR == retCode) || (NULL == commandId))
	{
		m4TraceHeaderLevel(2, cout << "No dataunits in input dataStorage." << endl);
		return NULL;
	}

	return commandId;
  //## end ClAdminRequestPDU::GetAdminCommandId%899816501.body
}

m4return_t ClAdminRequestPDU::SerializeCommand (ClSystemInterface *ai_posystem, ClAdminTagList &ai_oCommandList, M4ClString &ao_oSerializedString)
{
  //## begin ClAdminRequestPDU::SerializeCommand%900950039.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminRequestPDU::SerializeCommand");

	m4return_t retCode;

	//-----------------------------------------------------------------------
	// get the system tables and the subsystem path in the obl.
	//-----------------------------------------------------------------------
	//const m4pchar_t pcOblPath = ai_oCommandList.m_subsystemPath;
	m4char_t pcOblPath[1024];
	sprintf (pcOblPath, "M4ADMINISTRATOR.ACCESS-PERMISSIONS.%s.COMMANDS", ai_oCommandList.m_subsystemPath);

	//-----------------------------------------------------------------------
	// serialize the answer in a M4ClString.
	//-----------------------------------------------------------------------
	m4char_t buffer[1024];
	ClAdminTag *poAdminTag;
	ClAdminTagList::iterator it;
	m4char_t cEnable[32];
	size_t iEnableLenght;
	m4pchar_t pcCommandLiteral;

	// initialize the output string.
	ao_oSerializedString = "";

	for (it=ai_oCommandList.begin(); it!=ai_oCommandList.end(); it++)
	{
		poAdminTag = *it;
//		retCode = ai_posystem->m_poTables->m_oBook->GetStringValue(pcOblPath, poAdminTag->m_tagStringId, cEnable, 32, iEnableLenght);
		retCode = ai_posystem->GetStringSpecificationValue (pcOblPath, poAdminTag->m_tagStringId, cEnable, 32, iEnableLenght);

		// check if the command is enable.
		if ((retCode==M4_SUCCESS) && (!strcmp(cEnable, "NOT-ACCESSIBLE")))
			continue;

		// get the literal for the command.
		pcCommandLiteral = ai_posystem->GetLiteral(poAdminTag->m_tagStringId);
		if (pcCommandLiteral == NULL)
		{
			pcCommandLiteral = poAdminTag->m_tagStringId;
		}
		
		sprintf(buffer, "%d,%s;", poAdminTag->m_tagId, pcCommandLiteral);

		// in this version is disable the m_tagWithParameters
		//sprintf(buffer, "%d,%s,%d;", poAdminTag->m_tagId, pcCommandLiteral, poAdminTag->m_tagWithParameters);
		ao_oSerializedString += buffer;	
		//delete poAdminTag;
	}
		
	return M4_SUCCESS;
  //## end ClAdminRequestPDU::SerializeCommand%900950039.body
}

m4return_t ClAdminRequestPDU::SerializeCommandParams (ClSystemInterface *ai_posystem, ClAdminTagList &ai_oCommandList, M4ClString &ao_oSerializedString)
{
  //## begin ClAdminRequestPDU::SerializeCommandParams%901092590.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminRequestPDU::SerializeCommandParams");
	//-----------------------------------------------------------------------
	// get the system tables and the subsystem path in the obl.
	//-----------------------------------------------------------------------
	const m4pchar_t pcOblPath = ai_oCommandList.m_subsystemPath;

	//-----------------------------------------------------------------------
	// serialize the answer in a M4ClString.
	//-----------------------------------------------------------------------
	m4char_t buffer[1024];
	ClAdminTag *poAdminTag;
	ClAdminTagList::iterator it;
	m4pchar_t pcParamLiteral;
	m4pchar_t tagValue = "0";

	// initialize the output string.
	ao_oSerializedString = "";

	for (it=ai_oCommandList.begin(); it!=ai_oCommandList.end(); it++)
	{
		poAdminTag = *it;

		// get the literal for the command.
		//pcParamLiteral = ai_posystem->m_poTables->GetLiteral(poAdminTag->m_tagStringId);
		pcParamLiteral = ai_posystem->GetLiteral(poAdminTag->m_tagStringId);
		if (pcParamLiteral == NULL)
		{
			pcParamLiteral = poAdminTag->m_tagStringId;
		}

		if (poAdminTag->m_tagValue != NULL)
		{
			tagValue = poAdminTag->m_tagValue;
		}
		
		sprintf(buffer, "%d,%d,%s,%s,%d;", poAdminTag->m_tagId, poAdminTag->m_tagType, pcParamLiteral, tagValue,poAdminTag->m_tagRetValue?1:0);
		ao_oSerializedString += buffer;	
		
		delete poAdminTag;
	}
	
	return M4_SUCCESS;
  //## end ClAdminRequestPDU::SerializeCommandParams%901092590.body
}

m4return_t ClAdminRequestPDU::GetSystemInfo (ClPDU *ai_poInputPDU)
{
  //## begin ClAdminRequestPDU::GetSystemInfo%898002523.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminRequestPDU::GetSystemInfo");
	m4return_t retCode = M4_ERROR;

	m4TraceLevel(1, cout << "ClAdminRequestPDU::GetSystemInfo() " << endl);

	// allocate a tmp datastorage.
	ClDataUnitInterface *dataUnitInterface;
	M4DataStorage *idsDS = M4DataStorage::GetNewDataStorage();
	if (NULL == idsDS)
	{
		return M4_ERROR;
	}

	//-----------------------------------------------------------------------
	// Actually there is only a command for the global system. It's is the 
	// system ID. It's not necesary parse the input dataStorage.
	//-----------------------------------------------------------------------
	
	//-----------------------------------------------------------------------
	// get the system ID.
	//-----------------------------------------------------------------------
	m4int64_t iSystemId = m_poAdministrator->m_poSystem->GetId();
	char childPath[32];
	sprintf(childPath, "%ld", iSystemId);
	
	// get the system type.
	M4ClString ssType = "";
	retCode = m_poAdministrator->m_poSystem ->GetSubsystemAttribValue(childPath, 0, idsDS);
	if (M4_ERROR != retCode)
	{
		idsDS->InitializeIteration();
		retCode = idsDS->GetNext(dataUnitInterface);
		if (M4_ERROR != retCode)
		{
			m4pchar_t pcType = dataUnitInterface->GetBuffer();
			if (NULL != pcType)
			{
				ssType = pcType;
			}
		}			
	}
	else
	{
		M4DataStorage::DeleteDataStorage(idsDS);
		return M4_ERROR;
	}
	idsDS->Clear();

	// get the system name.
	M4ClString ssName = "";
	retCode = m_poAdministrator->m_poSystem->GetSubsystemAttribValue(childPath, 1, idsDS);
	if (M4_ERROR != retCode)
	{
		idsDS->InitializeIteration();
		retCode = idsDS->GetNext(dataUnitInterface);
		if (M4_ERROR != retCode)
		{
			m4pchar_t pcName = dataUnitInterface->GetBuffer();
			if (NULL != pcName)
			{
				ssName = pcName;
			}
		}			
	}
	else
	{
		M4DataStorage::DeleteDataStorage(idsDS);
		return M4_ERROR;
	}
	idsDS->Clear();

	// delete temporal datastorage.
	M4DataStorage::DeleteDataStorage(idsDS);
		
	//------------------------------------------------------------
	// Build the output dataStorage.
	//------------------------------------------------------------
	M4DataStorage *poOutputDataStorage;
	ClDataUnit *poDataUnit;
	poOutputDataStorage = M4DataStorage::GetNewDataStorage();
	if (NULL == poOutputDataStorage) 
	{
		retCode = SendErrorPDU(ai_poInputPDU->GetRequestId());
		return retCode;
	}
	
	// build full answer.
	M4ClString sSystemId = M4ClString(childPath);
	sSystemId += M4ClString(",");
	sSystemId += ssType;
	sSystemId += M4ClString(",");
	sSystemId += ssName;
	sSystemId += M4ClString(",1");
	

	poOutputDataStorage->GetNewDataUnit(poDataUnit);
	if (NULL != poDataUnit)
	{		
		retCode = poDataUnit->SetSize(sSystemId.size()+1);		
		if (M4_SUCCESS == retCode)
		{
			m4pchar_t pcBuffer = poDataUnit->GetBuffer();
			strcpy(pcBuffer, sSystemId.c_str());
		}
		else
		{
			retCode = SendErrorPDU(ai_poInputPDU->GetRequestId());
			M4DataStorage::DeleteDataStorage(poOutputDataStorage);
			return retCode;
		}
	}
	else
	{
		retCode = SendErrorPDU(ai_poInputPDU->GetRequestId());
		M4DataStorage::DeleteDataStorage(poOutputDataStorage);
		return retCode;
	}

	//------------------------------------------------------------
	// Send the reply PDU.
	//------------------------------------------------------------
	retCode = SendReplyPDU(ai_poInputPDU->GetRequestId(), poOutputDataStorage);
//	M4DataStorage::DeleteDataStorage(poOutputDataStorage) ;
	return retCode;
  //## end ClAdminRequestPDU::GetSystemInfo%898002523.body
}

m4return_t ClAdminRequestPDU::GetSubsystemInfo (ClPDU *ai_poInputPDU)
{
  //## begin ClAdminRequestPDU::GetSubsystemInfo%898002524.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminRequestPDU::GetSubsystemInfo");
	int iDataSize;
	m4return_t retCode = M4_SUCCESS;

	//-----------------------------------------------------------------------
	// get input dataStorage.
	// the first dataUnit is the command id. Skip it.
	//-----------------------------------------------------------------------
	m4pchar_t commandId;
	M4DataStorage *poInputDataStorage = ai_poInputPDU->GetDataStorage();
	poInputDataStorage->InitializeIteration();
	GetNextDataUnit(poInputDataStorage, commandId, iDataSize);
	
	//-----------------------------------------------------------------------
	// the next dataUnit is the subsystem path.
	//-----------------------------------------------------------------------
	m4pchar_t subsystemPath;
	retCode = GetNextDataUnit(poInputDataStorage, subsystemPath, iDataSize);
	if (M4_ERROR == retCode)
	{
		m4TraceHeaderLevel(2, cout << "No dataunits in input dataStorage." << endl);
		retCode = SendErrorPDU(ai_poInputPDU->GetRequestId());
		return retCode;
	}
	m4TraceLevel(1, cout << "ClAdminRequestPDU::GetSubsystemInfo() - " << subsystemPath << endl);

	//-----------------------------------------------------------------------
	// get the subsystem children.
	//-----------------------------------------------------------------------
	vector<m4objid_t> children;
	m4uint32_t id;
	M4ClString stringIds = "";
	char childPath[1024];
	char childId[1024];
	
	retCode = m_poAdministrator->m_poSystem->GetSubsystemsId(subsystemPath, children);	
	if (M4_SUCCESS != retCode)
	{
		retCode = SendErrorPDU(ai_poInputPDU->GetRequestId());
		return retCode;
	}

	
	// data storages for temporal variables.
	M4DataStorage *idsDS = M4DataStorage::GetNewDataStorage();
	ClDataUnitInterface *dataUnitInterface;
	for (unsigned int i=0; i<children.size(); i++)
	{
		id = children[i] ;
		// childs path.
		sprintf(childPath, "%s.%u", subsystemPath, id ); 

		// get the child's type.
		M4ClString ssType = "";
		retCode = m_poAdministrator->m_poSystem->GetSubsystemAttribValue(childPath, 0, idsDS);
		if (M4_ERROR != retCode)
		{
			idsDS->InitializeIteration();
			retCode = idsDS->GetNext(dataUnitInterface);
			if (M4_ERROR != retCode)
			{
				m4pchar_t pcType = dataUnitInterface->GetBuffer();
				if (NULL != pcType)
				{
					ssType = pcType;
				}
			}			
		}
		else
		{
			continue;
		}
		idsDS->Clear();

		// check if this type of subsystem is accessible.
		m4char_t cEnable[32] = "";
		size_t iEnableLenght;
		
		retCode = m_poAdministrator->m_poSystem->GetStringSpecificationValue (
			"M4ADMINISTRATOR.ACCESS-PERMISSIONS.SUBSYSTEM.ACCESS-PERMISSIONS", 
			(char*)ssType.c_str(), 
			cEnable, 32, iEnableLenght);
		
		if ((retCode==M4_SUCCESS) && (!strcmp(cEnable, "NOT-ACCESSIBLE")))
		{
			m4TraceHeaderLevel(4, cout << "GetSubsystemInfo - child type: " << ssType << " - child access: " << cEnable << endl);		
			continue;
		}
						
		// get the child's name.
		M4ClString ssName = "";
		retCode = m_poAdministrator->m_poSystem->GetSubsystemAttribValue(childPath, 1, idsDS);
		if (M4_ERROR != retCode)
		{
			idsDS->InitializeIteration();
			retCode = idsDS->GetNext(dataUnitInterface);
			if (M4_ERROR != retCode)
			{
				m4pchar_t pcName = dataUnitInterface->GetBuffer();
				if (NULL != pcName)
				{
					ssName = pcName;
				}
			}			
		}
		else
		{
			continue;
		}
		idsDS->Clear();

		sprintf(childId, "%u", id ); 
		stringIds += childId;
		stringIds += "," + ssType + "," + ssName + ";";
	}

	//------------------------------------------------------------
	// Build the output dataStorage.
	//------------------------------------------------------------
	M4DataStorage *poOutputDataStorage;
	poOutputDataStorage = M4DataStorage::GetNewDataStorage();
	if (NULL == poOutputDataStorage) 
	{
		retCode = SendErrorPDU(ai_poInputPDU->GetRequestId());
		M4DataStorage::DeleteDataStorage(idsDS);
		return retCode;
	}

	
	ClDataUnit *poDataUnit = NULL;
	poOutputDataStorage->GetNewDataUnit(poDataUnit);
	poDataUnit->SetSize(strlen(stringIds.c_str()) + 1);	
	strcpy(poDataUnit->GetBuffer(), stringIds.c_str());	

	//-----------------------------------------------------------------------
	// get the subsystem attribs.
	//-----------------------------------------------------------------------
	m_poAdministrator->m_poSystem->GetSubsystemAttribsId(subsystemPath, poOutputDataStorage);	

	//-----------------------------------------------------------------------
	// get the subsystem statistics.
	//-----------------------------------------------------------------------
	m_poAdministrator->m_poSystem->GetStatisticAttribsId(subsystemPath, poOutputDataStorage);	

	//------------------------------------------------------------
	// Send the reply PDU.
	//------------------------------------------------------------
	retCode = SendReplyPDU(ai_poInputPDU->GetRequestId(), poOutputDataStorage);
//	M4DataStorage::DeleteDataStorage(poOutputDataStorage);
	M4DataStorage::DeleteDataStorage(idsDS);
	return retCode;
  //## end ClAdminRequestPDU::GetSubsystemInfo%898002524.body
}

m4return_t ClAdminRequestPDU::GetSubsystemInfoValues (ClPDU *ai_poInputPDU)
{
  //## begin ClAdminRequestPDU::GetSubsystemInfoValues%898002525.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminRequestPDU::GetSubsystemInfoValues");
	m4return_t retCode = M4_SUCCESS;
	int iDataSize;


	//-----------------------------------------------------------------------
	// get input dataStorage.
	// the first dataUnit is the command id. Skip it.
	//-----------------------------------------------------------------------
	m4pchar_t commandId;
	M4DataStorage *poInputDataStorage = ai_poInputPDU->GetDataStorage();
	poInputDataStorage->InitializeIteration();
	GetNextDataUnit(poInputDataStorage, commandId, iDataSize);
	
	//-----------------------------------------------------------------------
	// the next dataUnit is the subsystem path.
	//-----------------------------------------------------------------------
	m4pchar_t subsystemPath;
	retCode = GetNextDataUnit(poInputDataStorage, subsystemPath, iDataSize);
	if (M4_ERROR == retCode)
	{
		m4TraceHeaderLevel(2, cout << "No dataunits in input dataStorage." << endl);
		retCode = SendErrorPDU(ai_poInputPDU->GetRequestId());
		return retCode;
	}
	m4TraceLevel(1, cout << "ClAdminRequestPDU::GetSubsystemInfoValues() - " << subsystemPath << endl);

	//-----------------------------------------------------------------------
	// get attributes values.
	//-----------------------------------------------------------------------
	int nAttributes = 0;
	retCode = M4_SUCCESS;
	M4DataStorage *attDS = M4DataStorage::GetNewDataStorage();
	if (NULL == attDS)
	{
		retCode = SendErrorPDU(ai_poInputPDU->GetRequestId());
		return retCode;
	}

	while (M4_SUCCESS == retCode)
	{
		retCode = m_poAdministrator->m_poSystem->GetSubsystemAttribValue(subsystemPath, nAttributes, attDS);
		if (M4_SUCCESS == retCode)
		{
			nAttributes++;
		}
	}
	
	//-----------------------------------------------------------------------
	// get statistics values.
	//-----------------------------------------------------------------------
	int nStatistiscs = 0;
	retCode = M4_SUCCESS;
	M4DataStorage *statDS = M4DataStorage::GetNewDataStorage();
	if (NULL == statDS)
	{
		M4DataStorage::DeleteDataStorage(attDS);
		retCode = SendErrorPDU(ai_poInputPDU->GetRequestId());
		return retCode;
	}

	while (M4_SUCCESS == retCode)
	{
		retCode = m_poAdministrator->m_poSystem->GetStatisticAttribValue(subsystemPath, nStatistiscs, statDS);
		if (M4_SUCCESS == retCode)
		{
			nStatistiscs++;
		}
	}


	//------------------------------------------------------------
	// Build the output dataStorage.
	//------------------------------------------------------------
	M4DataStorage *poOutputDataStorage;
	poOutputDataStorage = M4DataStorage::GetNewDataStorage();
	if (NULL == poOutputDataStorage) 
	{
		M4DataStorage::DeleteDataStorage(statDS);
		M4DataStorage::DeleteDataStorage(attDS);
		retCode = SendErrorPDU(ai_poInputPDU->GetRequestId());
		return retCode;
	}

	//-----------------------------------------------------------------------
	// insert attributes info in output dataStorage.
	//-----------------------------------------------------------------------
	m4pchar_t pcBuffer;
	m4uint32_t uiBufferSize = 16;
	ClDataUnit *poOutputDataUnit;
	ClDataUnitInterface *dataUnitInterface;

	// insert number of attributes.
	poOutputDataStorage->GetNewDataUnit(poOutputDataUnit);
	if (NULL != poOutputDataUnit)
	{
		retCode = poOutputDataUnit->SetSize(uiBufferSize);
		if (M4_SUCCESS == retCode)
		{
			pcBuffer = poOutputDataUnit->GetBuffer();
			if (NULL != pcBuffer)
			{
				sprintf(pcBuffer, "%d", nAttributes);
			}
			else
			{
				M4DataStorage::DeleteDataStorage(attDS);
				M4DataStorage::DeleteDataStorage(statDS);
				M4DataStorage::DeleteDataStorage(poOutputDataStorage);
				retCode = SendErrorPDU(ai_poInputPDU->GetRequestId());
				return retCode;
			}
		}
	}
	else
	{
		M4DataStorage::DeleteDataStorage(attDS);
		M4DataStorage::DeleteDataStorage(statDS);
		M4DataStorage::DeleteDataStorage(poOutputDataStorage);
		retCode = SendErrorPDU(ai_poInputPDU->GetRequestId());
		return retCode;
	}

	// insert attributes values. 
	retCode = M4_SUCCESS;
	if(nAttributes)
	{
		attDS->InitializeIteration();
		while (M4_SUCCESS == retCode)
		{
			retCode = attDS->GetNext(dataUnitInterface);
			if ((M4_SUCCESS == retCode) && (dataUnitInterface->GetSize()))
			{
				ClDataUnit *poDU;
				m4int_t aux_size=dataUnitInterface->GetSize();
				poOutputDataStorage->GetNewDataUnit(poDU);
				poDU->SetSize(aux_size);
				memcpy(poDU->GetBuffer(),dataUnitInterface->GetBuffer(),aux_size);
			}
#ifdef _DEBUG
			else
			{
				if(dataUnitInterface && (!(dataUnitInterface->GetSize())))
				{
					cout << "Empty DataUnit"<<endl;
				}
			}
#endif
		}
	}
	//-----------------------------------------------------------------------
	// insert statistics info in output dataStorage.
	//-----------------------------------------------------------------------
	// insert number of statistics.
	poOutputDataStorage->GetNewDataUnit(poOutputDataUnit);
	if (NULL != poOutputDataUnit)
	{
		retCode = poOutputDataUnit->SetSize(uiBufferSize);
		if (M4_SUCCESS == retCode)
		{
			pcBuffer = poOutputDataUnit->GetBuffer();
			if (NULL != pcBuffer)
			{
				sprintf(pcBuffer, "%d", nStatistiscs);
			}
			else
			{
				M4DataStorage::DeleteDataStorage(attDS);
				M4DataStorage::DeleteDataStorage(statDS);
				M4DataStorage::DeleteDataStorage(poOutputDataStorage);
				retCode = SendErrorPDU(ai_poInputPDU->GetRequestId());
				return retCode;
			}
		}
	}
	else
	{
		M4DataStorage::DeleteDataStorage(attDS);
		M4DataStorage::DeleteDataStorage(statDS);
		M4DataStorage::DeleteDataStorage(poOutputDataStorage);
		retCode = SendErrorPDU(ai_poInputPDU->GetRequestId());
		return retCode;
	}

	// insert statistics values. 
	retCode = M4_SUCCESS;
	if(nStatistiscs)
	{
		statDS->InitializeIteration();
		while (M4_SUCCESS == retCode)
		{
			retCode = statDS->GetNext(dataUnitInterface);
			if ((M4_SUCCESS == retCode) && (dataUnitInterface->GetSize()))
			{
				ClDataUnit *poDU;
				poOutputDataStorage->GetNewDataUnit(poDU);
				m4int_t aux_size=dataUnitInterface->GetSize();
				poDU->SetSize(aux_size);
				memcpy(poDU->GetBuffer(),dataUnitInterface->GetBuffer(),aux_size);
			}
#ifdef _DEBUG
			else
			{
				if(dataUnitInterface && (!(dataUnitInterface->GetSize())))
				{
					cout << "Empty DataUnit"<<endl;
				}
			}
#endif
		}
	}
	//------------------------------------------------------------
	// Send the reply PDU.
	//------------------------------------------------------------
	retCode = SendReplyPDU(ai_poInputPDU->GetRequestId(), poOutputDataStorage);

//	M4DataStorage::DeleteDataStorage(poOutputDataStorage);
	M4DataStorage::DeleteDataStorage(attDS);
	M4DataStorage::DeleteDataStorage(statDS);

	return retCode;
  //## end ClAdminRequestPDU::GetSubsystemInfoValues%898002525.body
}

m4return_t ClAdminRequestPDU::GetCommandsInfo (ClPDU *ai_poInputPDU)
{
  //## begin ClAdminRequestPDU::GetCommandsInfo%898002526.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminRequestPDU::GetCommandsInfo");
	int iDataSize;
	ClAdminTagList oCommandList, oCommandParamList;
	M4ClString oSerializedString;
	ClDataUnit *poDataUnit;
	m4char_t buffer[32];

	//-----------------------------------------------------------------------
	// get input dataStorage.
	// the first dataUnit is the command id. Skip it.
	//-----------------------------------------------------------------------
	m4pchar_t commandId;
	M4DataStorage *poInputDataStorage = ai_poInputPDU->GetDataStorage();
	poInputDataStorage->InitializeIteration();
	GetNextDataUnit(poInputDataStorage, commandId, iDataSize);
	
	//-------------------------------------------------------------------
	// the next dataUnit is the subsystem path.
	//-------------------------------------------------------------------
	m4pchar_t subsystemPath;
	m4return_t retCode = GetNextDataUnit(poInputDataStorage, subsystemPath, iDataSize);
	if (M4_ERROR == retCode)
	{
		m4TraceHeaderLevel(2, cout << "No dataunits in input dataStorage." << endl);
		return M4_ERROR;
	}
	m4TraceLevel(1, cout << "ClAdminRequestPDU::GetCommandsInfo() - " << subsystemPath << endl);
	
	//------------------------------------------------------------
	// Build the output dataStorage.
	//------------------------------------------------------------
	M4DataStorage *poOutputDataStorage;
	poOutputDataStorage = M4DataStorage::GetNewDataStorage();
	if (NULL == poOutputDataStorage) 
	{
		retCode = SendErrorPDU(ai_poInputPDU->GetRequestId());
		return retCode;
	}

	//-------------------------------------------------------------------
	// if (path == "0"), this is the system. The system doesn't has
	// commands.
	//-------------------------------------------------------------------
	if (!strcmp(subsystemPath, "0"))
	{
		oSerializedString = " ";
	}
	else
	{		
		//-------------------------------------------------------------------
		// get subsystem commands.
		//-------------------------------------------------------------------
		m_poAdministrator->m_poSystem->GetCommandsIdRoot(subsystemPath, oCommandList);			
		SerializeCommand (m_poAdministrator->m_poSystem, oCommandList, oSerializedString);
	}
	poOutputDataStorage->GetNewDataUnit(poDataUnit);
	poDataUnit->SetSize(strlen(oSerializedString.c_str()) + 1);	
	strcpy(poDataUnit->GetBuffer(), oSerializedString.c_str());	
	
	//-------------------------------------------------------------------
	// get subsystem command parameters.
	//-------------------------------------------------------------------	
	ClAdminTagList::iterator it;
	for (it=oCommandList.begin(); it!=oCommandList.end(); it++) 
	{
		ClAdminTag *poAdminTag = *it;
		if (M4_TRUE == poAdminTag->m_tagWithParameters)
		{
			// insert command Id in the output DataStorage.
			sprintf (buffer, "%d", poAdminTag->m_tagId);
			poOutputDataStorage->GetNewDataUnit(poDataUnit);
			poDataUnit->SetSize(strlen(buffer) + 1);	
			strcpy(poDataUnit->GetBuffer(), buffer);	
			
			m_poAdministrator->m_poSystem->GetCommandParamsRoot(subsystemPath, (char)poAdminTag->m_tagId, oCommandParamList);	
			m4TraceLevel(4, cout << "Number of parameters: " << oCommandParamList.size());
			
			SerializeCommandParams (m_poAdministrator->m_poSystem, oCommandParamList, oSerializedString);		
			oCommandParamList.clear();

			poOutputDataStorage->GetNewDataUnit(poDataUnit);
			poDataUnit->SetSize(strlen(oSerializedString.c_str()) + 1);	
			strcpy(poDataUnit->GetBuffer(), oSerializedString.c_str());
		}
		
		delete poAdminTag;
	}
	
	//-----------------------------------------------------------------------
	// normal return.
	//-----------------------------------------------------------------------
	retCode = SendReplyPDU(ai_poInputPDU->GetRequestId(), poOutputDataStorage);
//	M4DataStorage::DeleteDataStorage(poOutputDataStorage);
	return retCode;
  //## end ClAdminRequestPDU::GetCommandsInfo%898002526.body
}

m4return_t ClAdminRequestPDU::ExecuteCommand (ClPDU *ai_poInputPDU)
{
  //## begin ClAdminRequestPDU::ExecuteCommand%898002529.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminRequestPDU::ExecuteCommand");
	m4return_t retCode;
	int iDataSize;

	m4TraceLevel(1, cout << "ClAdminRequestPDU::ExecuteCommand() " << endl);

	//-----------------------------------------------------------------------
	// check the administrator role. Only the M4_DEFAULT_ADMINISTRATOR_ROLE
	// has permission to execute commands.
	//-----------------------------------------------------------------------
	if (m_poAdministrator->m_Role.empty())
	{
		m4TraceHeaderLevel(1, cout << "This user has not M4_DEFAULT_ADMINISTRATOR_ROLE." << endl);
		
		// write error in log system.
		M4AdminLogError::Setcodef(M4_SRV_ADM_UserHasNotAdministratorRole);
		
		retCode = SendErrorPDU(ai_poInputPDU->GetRequestId());
		return retCode;
	}
	
	//-----------------------------------------------------------------------
	// get input dataStorage.
	// the first dataUnit is the command id. Skip it.
	//-----------------------------------------------------------------------
	m4pchar_t commandId;
	M4DataStorage *poInputDataStorage = ai_poInputPDU->GetDataStorage();
	poInputDataStorage->InitializeIteration();
	GetNextDataUnit(poInputDataStorage, commandId, iDataSize);
	
	//-----------------------------------------------------------------------
	// the next dataUnit is the system path.
	//-----------------------------------------------------------------------
	m4pchar_t pcSystemPath;
	retCode = GetNextDataUnit(poInputDataStorage, pcSystemPath, iDataSize);
	if (M4_ERROR == retCode)
	{
		retCode = SendErrorPDU(ai_poInputPDU->GetRequestId());
		return retCode;
	}

	//-----------------------------------------------------------------------
	// the next dataUnit is the command id.
	//-----------------------------------------------------------------------
	m4pchar_t pcCommandId;
	retCode = GetNextDataUnit(poInputDataStorage, pcCommandId, iDataSize);
	if (M4_ERROR == retCode)
	{
		retCode = SendErrorPDU(ai_poInputPDU->GetRequestId());
		return retCode;
	}
	m4char_t cCommandId = atoi(pcCommandId);
	
	//-----------------------------------------------------------------------
	// create a new param list with the parameters for this command.
	//-----------------------------------------------------------------------
	ClAdminTagList oInputParamList, oOutputParamList;
	ClAdminCommandTag *poParamTag;
	m4pchar_t paramId;
	m4pchar_t paramValue;
	retCode = M4_SUCCESS;
	while (M4_SUCCESS == retCode)
	{
		retCode = GetNextDataUnit(poInputDataStorage, paramId, iDataSize);
		if (M4_ERROR == retCode)
		{
			break;
		}
		retCode = GetNextDataUnit(poInputDataStorage, paramValue, iDataSize);
		if (M4_ERROR == retCode)
		{
			break;
		}
		poParamTag = ClAdminCommandTag::GetNewAdminCommandTag(atoi(paramId), "", invalidType, paramValue);
		if (poParamTag != NULL)
		{
			oInputParamList.push_back(poParamTag);
		}
	}
	
	//-----------------------------------------------------------------------
	// execute the command.
	//-----------------------------------------------------------------------
	m4TraceHeaderLevel(2, cout << "Command: EXECUTE COMMAND " << pcSystemPath << " " << (int)cCommandId << endl);
	retCode = m_poAdministrator->m_poSystem->ExecuteCommand(pcSystemPath, cCommandId, oInputParamList, oOutputParamList);
	
	//-----------------------------------------------------------------------
	// check if the command is a shutdown command, then removes the subsystem
	// from the server tree.
	// This is the shutdown for a subsystem not for the whole server.
	//-----------------------------------------------------------------------
	if ((ClSSImplementation::SHUT_DOWN == cCommandId) && (M4_SUCCESS == retCode))
	{
		m4pchar_t pcDot;
		m4char_t pcSubsytemPath[256];

		strcpy(pcSubsytemPath, pcSystemPath);
		
		// Search backward '.'. Search the subsystem id.
		pcDot = strrchr(pcSubsytemPath, '.');		
		if (pcDot != NULL)
		{
			m4uint32_t subsystemId = atoi(pcDot+1);
			*pcDot = '\0';
			
			// Search backward '.'. Search the subsystem parent id.
			pcDot = strrchr(pcSubsytemPath, '.');		
			if (pcDot != NULL)
			{
				m4uint32_t subsystemParentId = atoi(pcDot+1);
				*pcDot = '\0';
				m_poAdministrator->m_poSystem->RemoveSubsystem(subsystemParentId, subsystemId);	
				
			}
			else
			{
				// the subsystem parent is the root (0).
				m_poAdministrator->m_poSystem->RemoveSubsystem(0, subsystemId);	
			}
		}
	}

	//-----------------------------------------------------------------------
	// delete input and output param list.
	//-----------------------------------------------------------------------
	ClAdminTag *poAdminTag;
	ClAdminTagList::iterator it;
	for (it=oInputParamList.begin(); it!=oInputParamList.end(); it++)
	{
		poAdminTag = *it;
		delete poAdminTag;
	}

	if(oOutputParamList.begin()!=oOutputParamList.end())
	{
		M4DataStorage* poOutputDataStorage=M4DataStorage::GetNewDataStorage();
		m4pchar_t buffer;
		for (it=oOutputParamList.begin(); it!=oOutputParamList.end(); it++)
		{
			poAdminTag = *it;
			if(poAdminTag->m_tagValue)
			{
				ClDataUnit *theDU;
				retCode=poOutputDataStorage->GetNewDataUnit(theDU);
				m4char_t tempbuffer[32];
				sprintf(tempbuffer,"%d",poAdminTag->m_tagId);
				theDU->SetSize(strlen(tempbuffer)+1);
				buffer=theDU->GetBuffer();
				strcpy(buffer,tempbuffer);
				retCode=poOutputDataStorage->GetNewDataUnit(theDU);
				theDU->SetSize(strlen(poAdminTag->m_tagValue)+1);
				buffer=theDU->GetBuffer();
				strcpy(buffer,poAdminTag->m_tagValue);
			}
			delete poAdminTag;
		}
		retCode = SendReplyPDU(ai_poInputPDU->GetRequestId(),poOutputDataStorage);
	//	M4DataStorage::DeleteDataStorage(poOutputDataStorage);
	}
	else
	{
		if (M4_SUCCESS == retCode)
		{
			retCode = SendAckPDU (ai_poInputPDU->GetRequestId());
		}
		else
		{
			retCode = SendErrorPDU (ai_poInputPDU->GetRequestId());
		}
	}

	return retCode;
  //## end ClAdminRequestPDU::ExecuteCommand%898002529.body
}

m4return_t ClAdminRequestPDU::ShutDown (ClPDU *ai_poInputPDU)
{
  //## begin ClAdminRequestPDU::ShutDown%898002530.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminRequestPDU::ShutDown");
	m4return_t retCode;

	//-----------------------------------------------------------------------
	// there is not parameters in shutdown command. It's not necesary parser 
	// the input dataStorage.
	//-----------------------------------------------------------------------
	m4TraceLevel(1, cout << "ClAdminRequestPDU::ShutDown() " << endl);
	
	//-----------------------------------------------------------------------
	// check the administrator role. Only the M4_DEFAULT_ADMINISTRATOR_ROLE
	// has permission to execute commands.
	//-----------------------------------------------------------------------
	if ( m_poAdministrator->m_Role.empty())
	{
		m4TraceHeaderLevel(1, cout << "This user has not M4_DEFAULT_ADMINISTRATOR_ROLE." << endl);

		// Put the error code (error list in logmsg.ini) in the
		// log system
		M4AdminLogError::Setcodef(M4_SRV_ADM_UserHasNotAdministratorRole);
		
		retCode = SendErrorPDU(ai_poInputPDU->GetRequestId());
		return retCode;
	}

	//-----------------------------------------------------------------------
	// before the shutdown, close the administrator session.
	//-----------------------------------------------------------------------
	retCode = SendAckPDU (ai_poInputPDU->GetRequestId());
	//m_poAdministrator->m_poTSAP->Close();

	//-----------------------------------------------------------------------
	// shutdown.
	//-----------------------------------------------------------------------
	//OSCAR 22-6-99
	M4PublishEvent("65557","") ;

	m_poAdministrator->m_poSystem->Exit(3);
//	m_poAdministrator->m_poSystem->ShutDown();

	m_poAdministrator -> Terminate () ;			
	return retCode;
  //## end ClAdminRequestPDU::ShutDown%898002530.body
}

// Additional Declarations
  //## begin ClAdminRequestPDU.declarations preserve=yes
  //## end ClAdminRequestPDU.declarations



// Class ClAdminPDUExecutor 


ClAdminPDUExecutor::ClAdminPDUExecutor (ClCSAdministrator *ai_poAdministrator)
  //## begin ClAdminPDUExecutor::ClAdminPDUExecutor%918150009.hasinit preserve=no
  //## end ClAdminPDUExecutor::ClAdminPDUExecutor%918150009.hasinit
  //## begin ClAdminPDUExecutor::ClAdminPDUExecutor%918150009.initialization preserve=yes
  :m_poAdministrator(ai_poAdministrator)
  //## end ClAdminPDUExecutor::ClAdminPDUExecutor%918150009.initialization
{
  //## begin ClAdminPDUExecutor::ClAdminPDUExecutor%918150009.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminPDUExecutor::ClAdminPDUExecutor");
  //## end ClAdminPDUExecutor::ClAdminPDUExecutor%918150009.body
}



//## Other Operations (implementation)
void ClAdminPDUExecutor::DumpInputPDU (ClPDU *ai_poInputPDU)
{
  //## begin ClAdminPDUExecutor::DumpInputPDU%897907989.body preserve=yes
	m4AutoTraceLevel(5, "ClCSAdministrator::DumpInputPDU");
	if (ai_poInputPDU == NULL)
	{
		m4Trace(cout << "   Invalid PDU." << endl);
		return;
	}

	int iBlockSize;
	m4pchar_t szPDUData;
	M4DataStorage *poInputDataStorage = ai_poInputPDU->GetDataStorage();
	if (poInputDataStorage == NULL)
	{
		m4Trace(cout << "   end PDU." << endl);
		return;
	}
	poInputDataStorage->InitializeIteration();
	m4Trace(cout << "   #");

	while (GetNextDataUnit(poInputDataStorage, szPDUData, iBlockSize) == M4_SUCCESS)
	{
		m4Trace(cout << szPDUData << '#');
	}
	m4Trace(cout << endl << "   end PDU." << endl);
  //## end ClAdminPDUExecutor::DumpInputPDU%897907989.body
}

m4return_t ClAdminPDUExecutor::GetNextDataUnit (M4DataStorage *ai_dataStorage, char *&ao_data, int &ao_dataSize)
{
  //## begin ClAdminPDUExecutor::GetNextDataUnit%917522117.body preserve=yes
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
  //## end ClAdminPDUExecutor::GetNextDataUnit%917522117.body
}

m4return_t ClAdminPDUExecutor::SendAckPDU (m4uint32_t ai_iRequestId, ClTagList *ai_poTagList)
{
  //## begin ClAdminPDUExecutor::SendAckPDU%918150010.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminPDUExecutor::SendAckPDU");
	m4return_t retCode = M4_SUCCESS;

	//------------------------------------------------------------------------
	// check administrator.
	//------------------------------------------------------------------------
	if (NULL == m_poAdministrator)
	{
		return M4_ERROR;
	}

	//------------------------------------------------------------------------
	// build the answer PDU.
	//------------------------------------------------------------------------
	ClProtocol oProtocol;
	
	ClAckPDU *poAckPDU = (ClAckPDU *) oProtocol.GetPDU(M4_ID_PDU_ACK);
	if (NULL == poAckPDU)
	{
		return M4_ERROR;
	}
	poAckPDU->SetRequestId(ai_iRequestId);

	// sets the output tags
	if (ai_poTagList != NULL) {

		// serializes to output
		M4DataStorage *poOutput = poAckPDU->GetDataStorage();
		if (poOutput != NULL) {
			if (ai_poTagList->TagList2DataStorage(poOutput) != M4_SUCCESS) {
				return M4_ERROR;
			}
		}
	}

	//-------------------------------------------------------------------
	// Send the PDU to the network.
	//-------------------------------------------------------------------
	retCode = m_poAdministrator->SendPDU(poAckPDU);	

	//-------------------------------------------------------------------
	// delete the output PDU.
	//-------------------------------------------------------------------
	if (NULL != poAckPDU)
	{
		delete poAckPDU;
	}

	return retCode;
  //## end ClAdminPDUExecutor::SendAckPDU%918150010.body
}

m4return_t ClAdminPDUExecutor::SendErrorPDU (m4uint32_t ai_iRequestId)
{
  //## begin ClAdminPDUExecutor::SendErrorPDU%918150011.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminPDUExecutor::SendErrorPDU");
	m4return_t retCode = M4_SUCCESS;

	//------------------------------------------------------------------------
	// check administrator.
	//------------------------------------------------------------------------
	if (NULL == m_poAdministrator)
	{
		return M4_ERROR;
	}

	//------------------------------------------------------------------------
	// build the answer PDU.
	//------------------------------------------------------------------------
	ClProtocol oProtocol;
	
	ClNewErrorPDU *poErrorPDU = (ClNewErrorPDU *) oProtocol.GetPDU(M4_ID_PDU_ERROR);
	if (NULL == poErrorPDU)
	{
		return M4_ERROR;
	}
	poErrorPDU->SetRequestId(ai_iRequestId);
	poErrorPDU->SetErrorCode(M4_ERROR);

	//-------------------------------------------------------------------
	// Send the PDU to the network.
	//-------------------------------------------------------------------
	retCode = m_poAdministrator->SendPDU(poErrorPDU);	

	//-------------------------------------------------------------------
	// delete the output PDU.
	//-------------------------------------------------------------------
	if (NULL != poErrorPDU)
	{
		delete poErrorPDU;
	}

	return retCode;
  //## end ClAdminPDUExecutor::SendErrorPDU%918150011.body
}

m4return_t ClAdminPDUExecutor::SendReplyPDU (m4uint32_t ai_iRequestId, M4DataStorage *ai_poDataStorage)
{
  //## begin ClAdminPDUExecutor::SendReplyPDU%918150012.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminPDUExecutor::SendErrorPDU");
	m4return_t retCode = M4_SUCCESS;

	//------------------------------------------------------------------------
	// check administrator.
	//------------------------------------------------------------------------
	if (NULL == m_poAdministrator)
	{
		return M4_ERROR;
	}
	//------------------------------------------------------------------------
	// añadimos los eventos a la respuesta
	//------------------------------------------------------------------------

	IEventHandler * TheEH=NULL;
	TheEH=ClLogBasedEventHandler::GetEHInstance();
	if(!TheEH)
	{m4Trace(cerr<< "Error obteniendo EH Instance"<<endl);}
	else
	{
		m4return_t iRet=M4_ERROR;
		m4int_t Defaultbuffersize=10000;
		m4int_t buffersize=Defaultbuffersize;
		m4pchar_t buffer=new m4char_t[buffersize];

		iRet=TheEH->GetEventMsg(*m_poAdministrator,buffer,buffersize,M4AdminEventTopic);
		while((iRet==M4_ERROR) && (buffersize>Defaultbuffersize))
		{
			delete buffer;
			buffer=new m4char_t[buffersize+1];
			iRet=TheEH->GetEventMsg(*m_poAdministrator,buffer,buffersize,M4AdminEventTopic);
		}
		if((iRet!=M4_ERROR) && buffersize)
		{
			ClDataUnit *pDU=NULL;
			ai_poDataStorage->GetNewDataUnit(pDU);
// OJO SI SE CAMBIA LA SERIALIZACION
			iRet=pDU->SetSize(buffersize+9);
			m4pchar_t outBuffer=pDU->GetBuffer();
			memcpy(outBuffer, "@@EVENT@@",9);
			memcpy(outBuffer+9, buffer,buffersize);
// OJO SI SE CAMBIA LA SERIALIZACION
		}
		delete buffer;
	}

	//------------------------------------------------------------------------
	// build the answer PDU.
	//------------------------------------------------------------------------
	ClProtocol oProtocol;
	
	ClReplyPDU* poReplyPDU = (ClReplyPDU *)oProtocol.GetPDU(M4_ID_PDU_REPLY);	
	if (NULL == poReplyPDU)
	{
		return M4_ERROR;
	}
	poReplyPDU->SetRequestId(ai_iRequestId);
	poReplyPDU->SetDataStorage(ai_poDataStorage);

	//-------------------------------------------------------------------
	// Send the PDU to the network.
	//-------------------------------------------------------------------
	retCode = m_poAdministrator->SendPDU(poReplyPDU);	

	//-------------------------------------------------------------------
	// delete the output PDU.
	//-------------------------------------------------------------------
	if (NULL != poReplyPDU)
	{
		delete poReplyPDU;
	}

	return retCode;
  //## end ClAdminPDUExecutor::SendReplyPDU%918150012.body
}

// Additional Declarations
  //## begin ClAdminPDUExecutor.declarations preserve=yes
  //## end ClAdminPDUExecutor.declarations



// Class ClAdminConnectPDU 

ClAdminConnectPDU::ClAdminConnectPDU (ClCSAdministrator *ai_poAdministrator)
  //## begin ClAdminConnectPDU::ClAdminConnectPDU%918150014.hasinit preserve=no
  //## end ClAdminConnectPDU::ClAdminConnectPDU%918150014.hasinit
  //## begin ClAdminConnectPDU::ClAdminConnectPDU%918150014.initialization preserve=yes
  :ClAdminPDUExecutor(ai_poAdministrator)
  //## end ClAdminConnectPDU::ClAdminConnectPDU%918150014.initialization
{
  //## begin ClAdminConnectPDU::ClAdminConnectPDU%918150014.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminConnectPDU::ClAdminConnectPDU");
  //## end ClAdminConnectPDU::ClAdminConnectPDU%918150014.body
}



//## Other Operations (implementation)
m4return_t ClAdminConnectPDU::Execute (ClPDU *ai_poInputPDU)
{
  //## begin ClAdminConnectPDU::Execute%899977157.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminConnectPDU::Execute");

	m4TraceLevel(1, cout << "ClAdminConnectPDU::Execute() " << endl);

	// get the DataStorage from the input PDU.
	M4DataStorage *poInputDataStorage = ai_poInputPDU->GetDataStorage();
	if (NULL == poInputDataStorage)
	{
		m4TraceHeaderLevel(2, cout << "Can't get DataStorage in connect PDU. Close connection." << endl);
		
		// write error in log system.
		M4AdminLogError::Setcodef(M4_SRV_ADM_InvalidAdministratorClient);
		
		SendErrorPDU (ai_poInputPDU->GetRequestId());
		
		// fix bug 0190803
		// The Terminate() method will close the socket using an action later
		// If we do this here the transport layer will select a socket already closed,
		// generating an error.
		// m_poAdministrator->m_poTSAP->Close();

		m_poAdministrator->Terminate();
		return M4_ERROR;
	}

	// get the tagList from the input DataStorage.
	ClTagList oTagList;
	char* admTag = "M4_ADMINISTRATION_SERVICE";
	int argSize = 128;
	m4pchar_t pcArg = new char [128];;
	if (NULL == pcArg)
	{
		m4TraceHeaderLevel(2, cout << "Can't get AdminTag in connect PDU. Close connection." << endl);
		
		// write error in log system.
		M4AdminLogError::Setcodef(M4_SRV_ADM_InvalidAdministratorClient);
		
		SendErrorPDU (ai_poInputPDU->GetRequestId());

		// fix bug 0190803
		// The Terminate() method will close the socket using an action later
		// If we do this here the transport layer will select a socket already closed,
		// generating an error.
		// m_poAdministrator->m_poTSAP->Close();

		m_poAdministrator->Terminate();
		return M4_ERROR;
	}

	oTagList.DataStorage2TagList(poInputDataStorage);
	if (M4_SUCCESS != oTagList.GetArg(admTag, pcArg, argSize))
	{
		m4TraceHeaderLevel(2, cout << "Can't get AdminTag in connect PDU. Close connection." << endl);
		delete pcArg;
		
		// write error in log system.
		M4AdminLogError::Setcodef(M4_SRV_ADM_InvalidAdministratorClient);
		
		SendErrorPDU (ai_poInputPDU->GetRequestId());

		// fix bug 0190803
		// The Terminate() method will close the socket using an action later
		// If we do this here the transport layer will select a socket already closed,
		// generating an error.
		// m_poAdministrator->m_poTSAP->Close();
		
		m_poAdministrator->Terminate();
		return M4_ERROR;
	}

	// fix bug 0085965
	// checks for client new version
	static m4pchar_t refName = "M4_PROP_SERVER_";
	static m4pchar_t refPath = "SERVER";
	static m4int_t refSize = strlen(refName);
	m4char_t pcBuffer[32];

	if (oTagList.StartIteration() == M4_SUCCESS) {
		m4char_t pcTagName[64];
		m4ServerVT oVT;
		
		while(oTagList.GetNextTag(pcTagName,64,oVT) != M4_ERROR) {

			// Enh 163151.
			if( !strcmp( pcTagName, M4_TECH_VERSION ) )
			{
				oTagList.SetArg( pcTagName, (m4pchar_t) GetMindDBVersion() ) ;
				continue;
			}

			if( !strcmp( pcTagName, M4_TECH_ENCODING ) )
			{
				oTagList.SetArg( pcTagName, (m4double_t) M4IsUnicode() ) ;
				continue;
			}

			// check pattern
			if (strncmp(pcTagName,refName,refSize) != 0) {
				continue;
			}

			// gets the environmente name
			m4pchar_t name = pcTagName + refSize;
			if (*name == '\0') {
				continue;
			}

			// gets the value
			if (GetEnviron(refPath,name,pcBuffer,32) != M4_SUCCESS) {
				continue;
			}

			// updates the result
			if (oTagList.SetArg(pcTagName,pcBuffer) != M4_SUCCESS) {
				continue;
			}
		}
	}

	delete pcArg;

	// answer with an ACK.
	return SendAckPDU (ai_poInputPDU->GetRequestId(), &oTagList);
//## end ClAdminConnectPDU::Execute%899977157.body
}

// Additional Declarations
  //## begin ClAdminConnectPDU.declarations preserve=yes
  //## end ClAdminConnectPDU.declarations



// Class ClAdminPDUExecutorCreator 



//## Other Operations (implementation)
ClAdminPDUExecutor * ClAdminPDUExecutorCreator::Create (ClCSAdministrator *ai_poAdministrator, m4int16_t ai_iPDUId)
{
  //## begin ClAdminPDUExecutorCreator::Create%897569279.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminPDUExecutorCreator::Create");

	switch (ai_iPDUId)
	{
	case M4_ID_PDU_CONNECT:
		m4TraceHeaderLevel(4, cout << "ClAdminPDUExecutorCreator::Create(). PDU id: M4_ID_PDU_CONNECT" << endl);
		return new ClAdminConnectPDU(ai_poAdministrator);
		break;
	case M4_ID_PDU_AUTHENTICATION:
		m4TraceHeaderLevel(4, cout << "ClAdminPDUExecutorCreator::Create(). PDU id: M4_ID_PDU_AUTHENTICATION" << endl);
		return new ClAdminAuthenticationPDU(ai_poAdministrator);
		break;
	case M4_ID_PDU_SETROLE:
	case M4_ID_PDU_NEWSETROLE:
		m4TraceHeaderLevel(4, cout << "ClAdminPDUExecutorCreator::Create(). PDU id: M4_ID_PDU_SETROLE" << endl);
		return new ClAdminSetrolePDU(ai_poAdministrator);
		break;
	case M4_ID_PDU_REQUEST:
		m4TraceHeaderLevel(4, cout << "ClAdminPDUExecutorCreator::Create(). PDU id: M4_ID_PDU_REQUEST" << endl);
		return new ClAdminRequestPDU(ai_poAdministrator);
		break;
	case M4_ID_PDU_DISCONNECT:
	case M4_ID_PDU_NEWDISCONNECT:
		m4TraceHeaderLevel(4, cout << "ClAdminPDUExecutorCreator::Create(). PDU id: M4_ID_PDU_DISCONNECT" << endl);
		return new ClAdminDisconnectPDU(ai_poAdministrator);
		break;
	default:
		m4TraceHeaderLevel(4, cout << "ClAdminPDUExecutorCreator::Create(). PDU not supported. PDU id (hex): "<< hex << ai_iPDUId << endl);
		return new ClAdminUnknownPDU(ai_poAdministrator);
		break;
	}
	return (ClAdminPDUExecutor *)NULL;
  //## end ClAdminPDUExecutorCreator::Create%897569279.body
}

// Additional Declarations
  //## begin ClAdminPDUExecutorCreator.declarations preserve=yes
  //## end ClAdminPDUExecutorCreator.declarations



// Class ClAdminAuthenticationPDU 

ClAdminAuthenticationPDU::ClAdminAuthenticationPDU (ClCSAdministrator *ai_poAdministrator)
  //## begin ClAdminAuthenticationPDU::ClAdminAuthenticationPDU%918150013.hasinit preserve=no
  //## end ClAdminAuthenticationPDU::ClAdminAuthenticationPDU%918150013.hasinit
  //## begin ClAdminAuthenticationPDU::ClAdminAuthenticationPDU%918150013.initialization preserve=yes
  :ClAdminPDUExecutor(ai_poAdministrator)
  //## end ClAdminAuthenticationPDU::ClAdminAuthenticationPDU%918150013.initialization
{
  //## begin ClAdminAuthenticationPDU::ClAdminAuthenticationPDU%918150013.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminAuthenticationPDU::ClAdminAuthenticationPDU");
  //## end ClAdminAuthenticationPDU::ClAdminAuthenticationPDU%918150013.body
}



//## Other Operations (implementation)
m4return_t ClAdminAuthenticationPDU::Execute (ClPDU *ai_poInputPDU)
{
  //## begin ClAdminAuthenticationPDU::Execute%897639336.body preserve=yes
	m4AutoTraceLevel(5, "ClAdminAuthenticationPDU::Execute");
	m4return_t retCode = M4_SUCCESS;
	
	m4TraceLevel(1, cout << "ClAdminAuthenticationPDU::Execute(): " << endl);
	
	//------------------------------------------------------------------------
	// check administrator.
	//------------------------------------------------------------------------
	if (NULL == m_poAdministrator)
	{
		return M4_ERROR;
	}

	//------------------------------------------------------------------------
	// get user and password from input PDU.
	//------------------------------------------------------------------------
	m4pchar_t pcUser;
	m4pchar_t pcPassword;
	ClAuthenticationPDU *authenticationPDU = (ClAuthenticationPDU *)ai_poInputPDU;
	authenticationPDU->GetUserId(pcUser);
	m_poAdministrator->m_User = pcUser;
	authenticationPDU->GetPassword(pcPassword);
	m_poAdministrator->m_Password = pcPassword;
	
	m4TraceLevel(1, cout << "(user, password): " << pcUser << "," << pcPassword << endl);
	
	//------------------------------------------------------------------------
	// Authenticate the user.
	//------------------------------------------------------------------------
	// check logon interface.
	if (NULL == m_poAdministrator->m_poLogonInterface)
	{
		m4TraceHeaderLevel(1, cout << "NULL = ai_poAdministrator->m_poLogonInterface" << endl);
		SendErrorPDU (ai_poInputPDU->GetRequestId());

		// fix bug 0190803
		// The Terminate() method will close the socket using an action later
		// If we do this here the transport layer will select a socket already closed,
		// generating an error.
		// m_poAdministrator->m_poTSAP->Close();
		
		m_poAdministrator->Terminate();
		return M4_ERROR;
	}

	// Fix bug 0088238
	ClTagList oTagList;
	m4bool_t bDo2Way = M4_FALSE;

	m4pchar_t pcClientMachine = NULL;
	m4ServerVT	ao_vArgClientMachine;

	M4DataStorage *pDataStorage = ai_poInputPDU->GetDataStorage();
	if( pDataStorage )
	{
		oTagList.DataStorage2TagList(pDataStorage);

		// gets the client tag
		m4ServerVT	ao_vArg	;
		if ( M4_SUCCESS == oTagList.GetArg (M4_SESS_USER_CRYPT2WAY, ao_vArg) )
		{
			bDo2Way = M4_TRUE;
		}

		if ( M4_SUCCESS == oTagList.GetArg (M4_CLIENT_ID, ao_vArgClientMachine ) )
		{
			if (ao_vArgClientMachine.Data.PointerChar != NULL)
				pcClientMachine = ao_vArgClientMachine.Data.PointerChar;
		}
	}
	
	// open the session
	retCode = m_poAdministrator->m_poLogonInterface->OpenSession(pcUser, pcPassword, m_poAdministrator->m_poUSSessionInterface, bDo2Way, pcClientMachine);
	if (M4_SUCCESS == retCode)
	{
		// get the default administration role.
		char administratorRole[1024];
		strcpy(administratorRole, m_poAdministrator->m_Role.c_str());

		// check if this user has M4_DEFAULT_ADMINISTRATOR_ROLE role.
		retCode = m_poAdministrator->m_poLogonInterface->EnableRole(m_poAdministrator->m_poUSSessionInterface, administratorRole);
		if (M4_SUCCESS == retCode)
		{
			m4TraceHeaderLevel(1, cout << "(rol): " << m_poAdministrator->m_Role << ", connected with full access." << endl);
		}
		else
		{
			m_poAdministrator->m_Role=M4_DEFAULT_ADMINISTRATOR_DEFAULT_ROLE;
			strcpy(administratorRole, m_poAdministrator->m_Role.c_str());
			retCode = m_poAdministrator->m_poLogonInterface->EnableRole(m_poAdministrator->m_poUSSessionInterface,administratorRole);
			if(M4_SUCCESS==retCode)
			{
				m4TraceHeaderLevel(1, cout << "(rol): " << m_poAdministrator->m_Role << ", connected with full access." << endl);
			}
			else
			{
				// reset the administration role. Ther user only can get information.
				// He can't execute commands.
				m_poAdministrator->m_Role = "";
				m4TraceHeaderLevel(1, cout << "(rol): " << "---" << ", connected with limited access." << endl);
			}
		}

		//
		// Close the session. Now the administrator doesn't appears in the
		// users subsystem tree.
		m_poAdministrator->m_poLogonInterface->CloseSession(m_poAdministrator->m_poUSSessionInterface);

		M4PublishEvent("65558","") ;

	}
	else
	{
		m4TraceHeaderLevel(1, cout << "ClAdminAuthenticationPDU::Execute: OpenSession failed." << endl);
		m4TraceHeaderLevel(1, cout << "Terminating administration thread..." << endl);
		SendErrorPDU (ai_poInputPDU->GetRequestId());

		// fix bug 0190803
		// The Terminate() method will close the socket using an action later
		// If we do this here the transport layer will select a socket already closed,
		// generating an error.
		// m4sleep(1);
		// m_poAdministrator->m_poTSAP->Close();
		// m4sleep(1);
		
		m_poAdministrator->Terminate();
		return M4_ERROR;
	}	
		
	//------------------------------------------------------------------------
	// answer with an ACK.
	//------------------------------------------------------------------------
	return SendAckPDU (ai_poInputPDU->GetRequestId());
  //## end ClAdminAuthenticationPDU::Execute%897639336.body
}

// Additional Declarations
  //## begin ClAdminAuthenticationPDU.declarations preserve=yes
  //## end ClAdminAuthenticationPDU.declarations



//## begin module.epilog preserve=yes
//## end module.epilog
