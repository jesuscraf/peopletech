//## begin module%4084F8C4024E.cm preserve=no
//## end module%4084F8C4024E.cm

//## begin module%4084F8C4024E.cp preserve=no
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
//## end module%4084F8C4024E.cp

//## Module: m4ssapi4snmp%4084F8C4024E; Package body
//## Subsystem: M4Subsystems::src%37D3A22003E4
//## Source file: z:\m4subsystems\src\m4ssapi4snmp.cpp

//## begin module%4084F8C4024E.additionalIncludes preserve=no
//## end module%4084F8C4024E.additionalIncludes

//## begin module%4084F8C4024E.includes preserve=yes
#include <cloblconfiguration.hpp>
//## end module%4084F8C4024E.includes

// snmpinterfaces
#include <snmpinterfaces.hpp>
// m4ssapi4snmp
#include <m4ssapi4snmp.hpp>
// clssinterfaz
#include <clssinterfaz.hpp>
//## begin module%4084F8C4024E.declarations preserve=no
//## end module%4084F8C4024E.declarations

//## begin module%4084F8C4024E.additionalDeclarations preserve=yes
//## end module%4084F8C4024E.additionalDeclarations


// Class M4SSApi4Snmp 


M4SSApi4Snmp::M4SSApi4Snmp (ClSSInterfaz* ai_poSSInterfaz)
  //## begin M4SSApi4Snmp::M4SSApi4Snmp%1082361896.hasinit preserve=no
  //## end M4SSApi4Snmp::M4SSApi4Snmp%1082361896.hasinit
  //## begin M4SSApi4Snmp::M4SSApi4Snmp%1082361896.initialization preserve=yes
  //## end M4SSApi4Snmp::M4SSApi4Snmp%1082361896.initialization
{
  //## begin M4SSApi4Snmp::M4SSApi4Snmp%1082361896.body preserve=yes
    m_pClSSInterfaz = ai_poSSInterfaz;
  //## end M4SSApi4Snmp::M4SSApi4Snmp%1082361896.body
}



//## Other Operations (implementation)
m4return_t M4SSApi4Snmp::ExecuteCommand (m4pcchar_t ai_commandID, m4pcchar_t ai_paramID, m4pcchar_t ai_paramValue) const
{
  //## begin M4SSApi4Snmp::ExecuteCommand%1082361895.body preserve=yes
    m4return_t retCode = M4_ERROR;
    
    // get command ID.
    if (NULL == ai_commandID)
    {
        return M4_ERROR;
    }
    m4char_t cCommand = atoi(ai_commandID);

    // build parameters list.
    if ((NULL == ai_paramID) || (NULL == ai_paramValue))
    {
        return M4_ERROR;
    }
    ClAdminTagList paramIn, paramOut;
	ClAdminCommandTag *poParamTag = ClAdminCommandTag::GetNewAdminCommandTag(atoi(ai_paramID), "", invalidType, (m4pchar_t)ai_paramValue);
    if (NULL == poParamTag)
    {
        return M4_ERROR;
    }
    paramIn.push_back(poParamTag);


    // execute command.
    if (NULL == m_pClSSInterfaz)
    {
        return M4_ERROR;
    }
    m4char_t ssPath[32];
	
#ifdef WIN32
    sprintf (ssPath, "%I64d", m_pClSSInterfaz->GetId());
#else
	sprintf (ssPath, "%lld", m_pClSSInterfaz->GetId());
#endif

    retCode = m_pClSSInterfaz->ExecuteCommand(ssPath, cCommand, paramIn, paramOut);

	// delete input and output param list.
	ClAdminTag *poAdminTag;
	ClAdminTagList::iterator it;
	for (it=paramIn.begin(); it!=paramIn.end(); it++)
	{
		poAdminTag = *it;
		delete poAdminTag;
	}
	for (it=paramOut.begin(); it!=paramOut.end(); it++)
	{
		poAdminTag = *it;
		delete poAdminTag;
	}

    return retCode;
  //## end M4SSApi4Snmp::ExecuteCommand%1082361895.body
}

m4return_t M4SSApi4Snmp::GetPropertyValue (const m4string_t& ai_name, m4int32_t ai_index, m4string_t& ao_value) const
{
  //## begin M4SSApi4Snmp::GetPropertyValue%1082361899.body preserve=yes
    m4return_t retCode = M4_ERROR;

    // get attribute.
    m4char_t szPath[8];
    sprintf (szPath, "%ld", (m4uint32_t) m_pClSSInterfaz->GetId());
    M4DataStorage *poDataStorage = M4DataStorage::GetNewDataStorage();
    retCode = m_pClSSInterfaz->GetSubsystemAttribValue(szPath, ai_index, poDataStorage);
    if (M4_SUCCESS == retCode)
    {
        // read attribute value.
        ClDataUnitInterface *dataUnitInterface;
        poDataStorage->InitializeIteration();
        retCode = poDataStorage->GetNext(dataUnitInterface);
        if (M4_SUCCESS == retCode)
        {
            ao_value = dataUnitInterface->GetBuffer();
        }
    }   
    M4DataStorage::DeleteDataStorage(poDataStorage);
	

    // if retCode != M4_SUCCESS, try another method.
    if (M4_SUCCESS != retCode)
    {
	    m4char_t szValue[OBL_PROPVALUE_MAX_LEN];
        m4size_t iLen = 0;

        retCode = m_pClSSInterfaz->GetConfigStringValue((char*)ai_name.c_str(), szValue, OBL_PROPVALUE_MAX_LEN, iLen);
        if (M4_ERROR != retCode)
        {
            ao_value = szValue;
        }
    }

    return retCode;

  //## end M4SSApi4Snmp::GetPropertyValue%1082361899.body
}

m4return_t M4SSApi4Snmp::GetStatisticValue (const m4string_t& ai_name, m4int32_t ai_index, m4string_t& ao_value) const
{
  //## begin M4SSApi4Snmp::GetStatisticValue%1083310100.body preserve=yes
    m4return_t retCode = M4_ERROR;

    // get subsystem statistics container.
    ClStatistic* poStats = m_pClSSInterfaz->GetStats();
    if (NULL == poStats)
    {
        return M4_ERROR;
    }

    // get statistic.
    m4char_t szPath[8];
    sprintf (szPath, "%ld", (m4uint32_t) m_pClSSInterfaz->GetId());
    M4DataStorage *poDataStorage = M4DataStorage::GetNewDataStorage();
    retCode = m_pClSSInterfaz->GetStatisticAttribValue(szPath, ai_index, poDataStorage);
    if (M4_SUCCESS == retCode)
    {
        // read statistic value.
	    ClDataUnitInterface *dataUnitInterface;
        poDataStorage->InitializeIteration();
        retCode = poDataStorage->GetNext(dataUnitInterface);
        if (M4_SUCCESS == retCode)
        {
            ao_value = dataUnitInterface->GetBuffer(); 
        }       
    }
    M4DataStorage::DeleteDataStorage(poDataStorage);

    return M4_SUCCESS;
  //## end M4SSApi4Snmp::GetStatisticValue%1083310100.body
}

// Additional Declarations
  //## begin M4SSApi4Snmp%4084FBE20183.declarations preserve=yes
  //## end M4SSApi4Snmp%4084FBE20183.declarations

//## begin module%4084F8C4024E.epilog preserve=yes
//## end module%4084F8C4024E.epilog
