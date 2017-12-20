//## begin module%40A9D1BA031D.cm preserve=no
//## end module%40A9D1BA031D.cm

//## begin module%40A9D1BA031D.cp preserve=no
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
//## end module%40A9D1BA031D.cp

//## Module: m4dspapi4snmp%40A9D1BA031D; Package body
//## Subsystem: M4Dispatcher::src%3923BBFB0332
//## Source file: z:\m4dispatcher\src\m4dspapi4snmp.cpp

//## begin module%40A9D1BA031D.additionalIncludes preserve=no
//## end module%40A9D1BA031D.additionalIncludes

//## begin module%40A9D1BA031D.includes preserve=yes
#include "adminagent.hpp"
#include "adminactions.hpp"
#include "clarraylibres.hpp"
#include "dspxmlreq.hpp"
#include "snmpres.hpp"
//## end module%40A9D1BA031D.includes

// snmpinterfaces
#include <snmpinterfaces.hpp>
// SiteNodeInfo
#include <sitenodeinfo.hpp>
// m4dspapi4snmp
#include <m4dspapi4snmp.hpp>
// m4dispatchermain
#include <m4dispatchermain.hpp>
// ParamList
#include <paramlist.hpp>
//## begin module%40A9D1BA031D.declarations preserve=no
//## end module%40A9D1BA031D.declarations

//## begin module%40A9D1BA031D.additionalDeclarations preserve=yes
#define DSP_COMMAND_SET_NODE_STATE          0

#define DSP_COMMAND_STATE_INACTIVE          0
#define DSP_COMMAND_STATE_READY             1
#define DSP_COMMAND_STATE_STARTING          2
#define DSP_COMMAND_STATE_SHUTING_DOWN      3
#define DSP_COMMAND_STATE_SHUTING_DOWN_MC   4
#define DSP_COMMAND_STATE_STATE_HALTED      5

//## end module%40A9D1BA031D.additionalDeclarations


// Class M4DspApi4Snmp 



M4DspApi4Snmp::M4DspApi4Snmp (const ClSiteNodeInformation& ai_node, ClSiteNodeRepository* ai_poRepository)
  //## begin M4DspApi4Snmp::M4DspApi4Snmp%1084868909.hasinit preserve=no
  //## end M4DspApi4Snmp::M4DspApi4Snmp%1084868909.hasinit
  //## begin M4DspApi4Snmp::M4DspApi4Snmp%1084868909.initialization preserve=yes
  //## end M4DspApi4Snmp::M4DspApi4Snmp%1084868909.initialization
{
  //## begin M4DspApi4Snmp::M4DspApi4Snmp%1084868909.body preserve=yes
    m_node = ai_node;
    m_repository = ai_poRepository;
  //## end M4DspApi4Snmp::M4DspApi4Snmp%1084868909.body
}



//## Other Operations (implementation)
m4return_t M4DspApi4Snmp::ExecuteCommand (m4pcchar_t ai_commandID, m4pcchar_t ai_paramID, m4pcchar_t ai_paramValue) const
{
  //## begin M4DspApi4Snmp::ExecuteCommand%1084868906.body preserve=yes
    m4return_t retCode = M4_ERROR;
    
    // read command id.
    m4int32_t nParams = 0;
    m4int32_t iParam = -1;
    nParams = sscanf(ai_commandID, "%d", &iParam);
    if (1 != nParams)
    {
        DUMP_CHLOG_ERROR(M4_SNMP_CANNOT_GET_COMMAND_ID_IN_SNMP_COMMAND);
        return M4_ERROR;
    }

    switch(iParam)
    {
    case DSP_COMMAND_SET_NODE_STATE:
        nParams = sscanf(ai_paramValue, "%d", &iParam);
        if (1 != nParams)
        {
            DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_PARAM_IN_SNMP_COMMAND, DSP_COMMAND_SET_NODE_STATE);
            return M4_ERROR;
        }
        retCode = SetNodeState(iParam);
        break;
    default:
        DUMP_CHLOG_ERRORF(M4_SNMP_INVALID_COMMAND_ID_IN_SNMP_COMMAND, iParam);
        retCode = M4_ERROR;
        break;
    }

    return retCode;
  //## end M4DspApi4Snmp::ExecuteCommand%1084868906.body
}

m4return_t M4DspApi4Snmp::GetPropertyValue (const m4string_t& ai_name, m4int32_t ai_index, m4string_t& ao_value) const
{
  //## begin M4DspApi4Snmp::GetPropertyValue%1084868907.body preserve=yes
    m4return_t retCode = M4_ERROR;
    if (NULL == m_repository)
    {
        return M4_ERROR;
    }

    // find this node in the repository.
    ClSiteNodeInformation sni = m_node;
    retCode = m_repository->GetNodeInfo(sni);
    if (retCode != M4_SUCCESS)
    {
        return M4_ERROR;
    }

    // find property value.
    retCode = sni.GetProperty(ai_name, ao_value);
    if ((M4_SUCCESS == retCode) && (!stricmp(ai_name.c_str(), "node_state")))
    {
        // property 'node_state' is a special case.
        m4char_t pcValue[8];
        if (!stricmp(STATE_INACTIVE, ao_value.c_str()))
        {
            sprintf(pcValue, "%d", DSP_COMMAND_STATE_INACTIVE);
        }
        else if (!stricmp(STATE_READY, ao_value.c_str()))
        {
            sprintf(pcValue, "%d", DSP_COMMAND_STATE_READY);
        }
        else if (!stricmp(STATE_STARTING, ao_value.c_str()))
        {
            sprintf(pcValue, "%d", DSP_COMMAND_STATE_STARTING);
        }
        else if (!stricmp(STATE_SHUTING_DOWN, ao_value.c_str()))
        {
            sprintf(pcValue, "%d", DSP_COMMAND_STATE_SHUTING_DOWN);
        }
        else if (!stricmp(STATE_SHUTING_DOWN_MC, ao_value.c_str()))
        {
            sprintf(pcValue, "%d", DSP_COMMAND_STATE_SHUTING_DOWN_MC);
        }
        else if (!stricmp(STATE_HALTED, ao_value.c_str()))
        {
            sprintf(pcValue, "%d", DSP_COMMAND_STATE_STATE_HALTED);
        }
        else
        {
            DUMP_CHLOG_ERRORF(M4_SNMP_INVALID_VALUE, "node_state" << pcValue);
            retCode = M4_ERROR;
        }

        ao_value = pcValue;
    }

    return retCode;
  //## end M4DspApi4Snmp::GetPropertyValue%1084868907.body
}

m4return_t M4DspApi4Snmp::GetStatisticValue (const m4string_t& ai_name, m4int32_t ai_index, m4string_t& ao_value) const
{
  //## begin M4DspApi4Snmp::GetStatisticValue%1084868908.body preserve=yes
    return M4_ERROR;
  //## end M4DspApi4Snmp::GetStatisticValue%1084868908.body
}

m4return_t M4DspApi4Snmp::StopNode () const
{
  //## begin M4DspApi4Snmp::StopNode%1085040984.body preserve=yes
    m4return_t retCode = M4_ERROR;

	ClDpchMainSingleton* poDpchMainSingleton = ClDpchMainSingleton::Instance();
    if (NULL == poDpchMainSingleton)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_MEMORY_ALLOCATION_ERROR, "ClDpchMainSingleton::Instance()");
        return M4_ERROR;
    }

    ClDpchMain* poDpchMain = poDpchMainSingleton->GetDpchMain();
    if (NULL == poDpchMain)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_MEMORY_ALLOCATION_ERROR, "poDpchMainSingleton->GetDpchMain()");
        return M4_ERROR;
    }

    ClDspXMLRequest* poRequest = new ClDspXMLRequest(NULL, ClIdGenerator::GetNewId(), 0);
    if (NULL == poDpchMain)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_MEMORY_ALLOCATION_ERROR, "new ClDspXMLRequest(NULL, ClIdGenerator::GetNewId(), 0)");
        return M4_ERROR;
    }
    
    ClStopConfigAction *poAction = new ClStopConfigAction(poDpchMain->m_poDispatcherAgent);
    if (NULL == poDpchMain)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_MEMORY_ALLOCATION_ERROR, "new ClStopConfigAction(poDpchMain->m_poDispatcherAgent)");
        return M4_ERROR;
    }

    ClParamList paramList;
    paramList.SetPointer("REQUEST", poRequest);
    paramList.SetString("host", m_node.GetNodeHost().c_str()) ; 
    paramList.SetString("configuration", m_node.GetSiteNodeID().c_str()); 
    paramList.SetString("user", m_node.GetUserName().c_str()); 
    paramList.SetString("password", m_node.GetPassword().c_str()); 
    paramList.SetString("load", "0"); 

    poAction->SetParams(&paramList);

    retCode = poAction->Execute();

    return retCode;
  //## end M4DspApi4Snmp::StopNode%1085040984.body
}

m4return_t M4DspApi4Snmp::StartNode () const
{
  //## begin M4DspApi4Snmp::StartNode%1085478477.body preserve=yes
    // update from the repository, the node info.
    ClSiteNodeInformation tmpNode = m_node;
    m_repository->GetNodeInfo(tmpNode);

    // set the status to enable. The CheckConsistency thread will start the server.
    tmpNode.SetIsEnabled(M4_TRUE);
    m_repository->AddNode(tmpNode);

    return M4_SUCCESS;
  //## end M4DspApi4Snmp::StartNode%1085478477.body
}

m4return_t M4DspApi4Snmp::SetNodeState (m4int32_t ai_state) const
{
  //## begin M4DspApi4Snmp::SetNodeState%1085478478.body preserve=yes
    m4return_t retCode = M4_ERROR;

    // update from the repository, the node info.
    ClSiteNodeInformation tmpNode = m_node;
    m_repository->GetNodeInfo(tmpNode);

    switch(ai_state)
    {
    case DSP_COMMAND_STATE_SHUTING_DOWN:
    case DSP_COMMAND_STATE_INACTIVE:
        // only can stop a server it its state is STATE_READY
        if (!stricmp(tmpNode.GetNodeState().c_str(), STATE_READY))
        {
            retCode = StopNode();
        }
        else
        {
            retCode = M4_ERROR;
        }
        break;
    case DSP_COMMAND_STATE_STARTING:
        // only can start a server it its state is INACTIVE or HALTED
        if (!stricmp(tmpNode.GetNodeState().c_str(), STATE_INACTIVE) ||
            !stricmp(tmpNode.GetNodeState().c_str(), STATE_HALTED))
        {
            retCode = StartNode();
        }
        else
        {
            retCode = M4_ERROR;
        }
        break;
    default:
        DUMP_CHLOG_ERRORF(M4_SNMP_INVALID_PARAM_ID_IN_SNMP_COMMAND, DSP_COMMAND_SET_NODE_STATE);
        retCode = M4_ERROR;
        break;
    }

    return retCode;
  //## end M4DspApi4Snmp::SetNodeState%1085478478.body
}

// Additional Declarations
  //## begin M4DspApi4Snmp%40A9CFA10139.declarations preserve=yes
  //## end M4DspApi4Snmp%40A9CFA10139.declarations

//## begin module%40A9D1BA031D.epilog preserve=yes
//## end module%40A9D1BA031D.epilog
