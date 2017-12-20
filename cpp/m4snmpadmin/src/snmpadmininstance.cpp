//## begin module%407D085802A7.cm preserve=no
//## end module%407D085802A7.cm

//## begin module%407D085802A7.cp preserve=no
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
//## end module%407D085802A7.cp

//## Module: snmpadmininstance%407D085802A7; Package body
//## Subsystem: m4snmpadmin::src%37EA676703E6
//## Source file: z:\m4snmpadmin\src\snmpadmininstance.cpp

//## begin module%407D085802A7.additionalIncludes preserve=no
//## end module%407D085802A7.additionalIncludes

#include "chlog.hpp"
#include <snmpadminfactory.hpp>
#include "snmpres.hpp"
#include <snmpadmininstance.hpp>
#include <snmpinterfaces.hpp>
#include "snmpxmlutil.hpp"
#include "m4unicode.hpp"
#include <cloblconfiguration.hpp>

#include <xercesc/dom/DOMNode.hpp> 

//## end module%407D085802A7.additionalDeclarations


// Class M4SnmpAdminInstance 

//## begin M4SnmpAdminInstance::<m_pM4ISnmpAdministratorFactory>%40729FDE039C.role preserve=no  private: static M4ISnmpAdministratorFactory { -> RHAN}
M4ISnmpAdministratorFactory *M4SnmpAdminInstance::m_pM4ISnmpAdministratorFactory = NULL;
//## end M4SnmpAdminInstance::<m_pM4ISnmpAdministratorFactory>%40729FDE039C.role

//## begin M4SnmpAdminInstance::<m_pM4ISnmpAdministrator>%4072CB7E014C.role preserve=no  private: static M4ISnmpAdministrator { -> RHAN}
M4ISnmpAdministrator *M4SnmpAdminInstance::m_pM4ISnmpAdministrator = NULL;
//## end M4SnmpAdminInstance::<m_pM4ISnmpAdministrator>%4072CB7E014C.role


//## Other Operations (implementation)
void M4SnmpAdminInstance::M4SnmpAdminFactory ()
{
  //## begin M4SnmpAdminInstance::M4SnmpAdminFactory%1080808688.body preserve=yes
  //## end M4SnmpAdminInstance::M4SnmpAdminFactory%1080808688.body
}

m4return_t M4SnmpAdminInstance::Create (ClConfiguration* ai_config)
{
  //## begin M4SnmpAdminInstance::Create%1080808689.body preserve=yes
    m4return_t retCode = M4_ERROR;

    // creates the factory.
    if (NULL == m_pM4ISnmpAdministratorFactory)
    {
        // initializes the factory.
        m_pM4ISnmpAdministratorFactory = new M4SnmpAdministratorFactory();

        if (NULL == m_pM4ISnmpAdministratorFactory) return M4_ERROR;

    }

    // creates the administrator.
    m4size_t iLen;
    m4char_t szType[OBL_PROPVALUE_MAX_LEN];
    retCode = ai_config->GetStringValue("", "SNMP_AGENT_TYPE", szType, OBL_PROPVALUE_MAX_LEN, iLen);
 	if(retCode == M4_ERROR)
	{
		return M4_ERROR;
	}
    
	if(!stricmp(szType, "SNMP_STANDALONE"))
    {
        m_pM4ISnmpAdministrator = m_pM4ISnmpAdministratorFactory->getStandAloneAdmin();
    }
    else if(!stricmp(szType, "SNMP_MASTER"))
    {
        m_pM4ISnmpAdministrator = m_pM4ISnmpAdministratorFactory->getMasterAdmin();
    }
    else if(!stricmp(szType, "SNMP_SLAVE"))
    {
        m_pM4ISnmpAdministrator = m_pM4ISnmpAdministratorFactory->getSlaveAdmin();
    }
    else
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_INVALID_VALUE, "SNMP_AGENT_TYPE" << szType);
        m_pM4ISnmpAdministrator = NULL;
    }
    if (NULL == m_pM4ISnmpAdministrator)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_MEMORY_ALLOCATION_ERROR, "m_pM4ISnmpAdministrator");
        return M4_ERROR;
    }

    // initializes the administrator.
    retCode = m_pM4ISnmpAdministrator->Init(ai_config);
    if (retCode == M4_ERROR)
    {
        delete m_pM4ISnmpAdministrator;
        m_pM4ISnmpAdministrator = NULL;
    }

    return retCode;
  //## end M4SnmpAdminInstance::Create%1080808689.body
}

M4ISnmpAdministrator* M4SnmpAdminInstance::Get ()
{
  //## begin M4SnmpAdminInstance::Get%1081265059.body preserve=yes
    return m_pM4ISnmpAdministrator;
  //## end M4SnmpAdminInstance::Get%1081265059.body
}

void M4SnmpAdminInstance::Destroy ()
{
  //## begin M4SnmpAdminInstance::Destroy%1081862818.body preserve=yes
    if (NULL != m_pM4ISnmpAdministrator)
    {
        delete m_pM4ISnmpAdministrator;
        m_pM4ISnmpAdministrator = NULL;
    }

  //## end M4SnmpAdminInstance::Destroy%1081862818.body
}

m4return_t M4SnmpAdminInstance::Create (const DOMNode *ai_config)
{
  //## begin M4SnmpAdminInstance::Create%1084791067.body preserve=yes
    m4return_t retCode = M4_ERROR;

    // creates the factory.
    if (NULL == m_pM4ISnmpAdministratorFactory)
    {
        // initializes the factory.
        m_pM4ISnmpAdministratorFactory = new M4SnmpAdministratorFactory();

        if (NULL == m_pM4ISnmpAdministratorFactory) return M4_ERROR;

    }

    // creates the administrator.
    const char* szType = SnmpDOMUtils::getAttVal(ai_config, M4_XERCES_LITERAL("snmp_admintype"));
    if (NULL == szType)
    {
		// UNICODE XML
		m4pchar_t	pcNodeName = M4XMLToCpp( (M4XMLCh*)ai_config->getNodeName() ) ;
        DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_XML_ATTRIBUTE, "snmp_admintype" << pcNodeName);
		delete [] pcNodeName ;
        return M4_ERROR;
    }
   
	if(!stricmp(szType, "SNMP_STANDALONE"))
    {
        m_pM4ISnmpAdministrator = m_pM4ISnmpAdministratorFactory->getStandAloneAdmin();
    }
    else if(!stricmp(szType, "SNMP_MASTER"))
    {
        m_pM4ISnmpAdministrator = m_pM4ISnmpAdministratorFactory->getMasterAdmin();
    }
    else if(!stricmp(szType, "SNMP_SLAVE"))
    {
        m_pM4ISnmpAdministrator = m_pM4ISnmpAdministratorFactory->getSlaveAdmin();
    }
    else
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_INVALID_VALUE, "SNMP_AGENT_TYPE" << szType);
        m_pM4ISnmpAdministrator = NULL;
    }
    if (NULL == m_pM4ISnmpAdministrator)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_MEMORY_ALLOCATION_ERROR, "m_pM4ISnmpAdministrator");
        return M4_ERROR;
    }

    // initializes the administrator.
    retCode = m_pM4ISnmpAdministrator->Init(ai_config);
    if (retCode == M4_ERROR)
    {
        delete m_pM4ISnmpAdministrator;
        m_pM4ISnmpAdministrator = NULL;
    }

    return retCode;
  //## end M4SnmpAdminInstance::Create%1084791067.body
}

// Additional Declarations
  //## begin M4SnmpAdminInstance%406BF093035B.declarations preserve=yes
  //## end M4SnmpAdminInstance%406BF093035B.declarations

//## begin module%407D085802A7.epilog preserve=yes
//## end module%407D085802A7.epilog
