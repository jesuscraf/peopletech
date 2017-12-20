//## begin module%406BE8630090.cm preserve=no
//## end module%406BE8630090.cm

//## begin module%406BE8630090.cp preserve=no
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
//## end module%406BE8630090.cp

//## Module: snmpaloneadmin%406BE8630090; Package body
//## Subsystem: m4snmpadmin::src%37EA676703E6
//## Source file: z:\m4snmpadmin\src\snmpaloneadmin.cpp

//## begin module%406BE8630090.additionalIncludes preserve=no
//## end module%406BE8630090.additionalIncludes

#include "agent_pp/mib.h"

#include "snmpres.hpp"
#include <snmpadmin.hpp>


//## begin module%406BE8630090.declarations preserve=no
//## end module%406BE8630090.declarations

//## begin module%406BE8630090.additionalDeclarations preserve=yes
//## end module%406BE8630090.additionalDeclarations


// Class M4SnmpStandAloneAdministrator 





M4SnmpStandAloneAdministrator::M4SnmpStandAloneAdministrator()
  //## begin M4SnmpStandAloneAdministrator::M4SnmpStandAloneAdministrator%.hasinit preserve=no
  //## end M4SnmpStandAloneAdministrator::M4SnmpStandAloneAdministrator%.hasinit
  //## begin M4SnmpStandAloneAdministrator::M4SnmpStandAloneAdministrator%.initialization preserve=yes
  //## end M4SnmpStandAloneAdministrator::M4SnmpStandAloneAdministrator%.initialization
{
  //## begin M4SnmpStandAloneAdministrator::M4SnmpStandAloneAdministrator%.body preserve=yes
    m_pSnmpx = NULL;
  //## end M4SnmpStandAloneAdministrator::M4SnmpStandAloneAdministrator%.body
}


M4SnmpStandAloneAdministrator::~M4SnmpStandAloneAdministrator()
{
  //## begin M4SnmpStandAloneAdministrator::~M4SnmpStandAloneAdministrator%.body preserve=yes
  //## end M4SnmpStandAloneAdministrator::~M4SnmpStandAloneAdministrator%.body
}



//## Other Operations (implementation)
m4void_t M4SnmpStandAloneAdministrator::_Run ()
{
  //## begin M4SnmpStandAloneAdministrator::_Run%1080808687.body preserve=yes
	Agentpp::Request* req = m_pRequestList->receive(2);
    if (req) 
    {
        m_pMib->process_request(req);
    }
    else 
    {
        m_pMib->cleanup();
    }
  //## end M4SnmpStandAloneAdministrator::_Run%1080808687.body
}

m4return_t M4SnmpStandAloneAdministrator::_Destroy ()
{
  //## begin M4SnmpStandAloneAdministrator::_Destroy%1082032851.body preserve=yes
    if (NULL != m_pSnmpx) delete m_pSnmpx;
    m_pSnmpx = NULL;

    return M4_SUCCESS;
  //## end M4SnmpStandAloneAdministrator::_Destroy%1082032851.body
}

m4return_t M4SnmpStandAloneAdministrator::_Init (const DOMNode* ai_config)
{
  //## begin M4SnmpStandAloneAdministrator::_Init%1084791689.body preserve=yes
    m4return_t retCode = M4_ERROR;

    // creates the SNMP++ administrator.
    int status = SNMP_CLASS_SUCCESS;
    if (NULL == m_pSnmpx) m_pSnmpx = new Agentpp::Snmpx(status, m_snmpPort);
    if ((NULL == m_pSnmpx) || (SNMP_CLASS_SUCCESS != status))
    {
		// bg 178793
		if( status == SNMP_CLASS_TL_FAILED )
		{
			DUMP_CHLOG_ERRORF( M4_SNMP_TRANSPORT_OPERATION_FAILED, m_snmpPort ) ;
		}
		else
		{
			DUMP_CHLOG_ERRORF(M4_SNMP_SNMP_PP_LIBRARY_ERROR, Snmp::error_msg(status) << status);
		}
        return M4_ERROR;
    }

    // set the administrator to the request list.
    m_pRequestList->set_snmp(m_pSnmpx);

    return M4_SUCCESS;
  //## end M4SnmpStandAloneAdministrator::_Init%1084791689.body
}

// Additional Declarations
  //## begin M4SnmpStandAloneAdministrator%406BE15F01A6.declarations preserve=yes
  //## end M4SnmpStandAloneAdministrator%406BE15F01A6.declarations

//## begin module%406BE8630090.epilog preserve=yes
//## end module%406BE8630090.epilog
