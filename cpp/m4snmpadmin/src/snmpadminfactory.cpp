//## begin module%406BF192016D.cm preserve=no
//## end module%406BF192016D.cm

//## begin module%406BF192016D.cp preserve=no
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
//## end module%406BF192016D.cp

//## Module: snmpadminfactory%406BF192016D; Package body
//## Subsystem: m4snmpadmin::src%37EA676703E6
//## Source file: z:\m4snmpadmin\src\snmpadminfactory.cpp

//## begin module%406BF192016D.additionalIncludes preserve=no
//## end module%406BF192016D.additionalIncludes

#include "agent_pp/mib.h"

#include "cloblconfiguration.hpp"
#include "snmpres.hpp"
#include <snmpadminfactory.hpp>
#include <snmpadmin.hpp>
#include <snmpinterfaces.hpp>


//## begin module%406BF192016D.additionalDeclarations preserve=yes
//## end module%406BF192016D.additionalDeclarations


// Class M4SnmpAdministratorFactory 


//## Other Operations (implementation)
M4ISnmpAdministrator* M4SnmpAdministratorFactory::getStandAloneAdmin ()
{
  //## begin M4SnmpAdministratorFactory::getStandAloneAdmin%1081250851.body preserve=yes
    M4SnmpStandAloneAdministrator* poSnmpAdmin = new M4SnmpStandAloneAdministrator();

    return poSnmpAdmin;
  //## end M4SnmpAdministratorFactory::getStandAloneAdmin%1081250851.body
}

M4ISnmpAdministrator* M4SnmpAdministratorFactory::getMasterAdmin ()
{
  //## begin M4SnmpAdministratorFactory::getMasterAdmin%1081250852.body preserve=yes
    DUMP_CHLOG_ERRORF(M4_SNMP_LIBRARY_ERROR, "Master agent configuration not supported.");
    return NULL;
  //## end M4SnmpAdministratorFactory::getMasterAdmin%1081250852.body
}

M4ISnmpAdministrator* M4SnmpAdministratorFactory::getSlaveAdmin ()
{
  //## begin M4SnmpAdministratorFactory::getSlaveAdmin%1081250853.body preserve=yes
    DUMP_CHLOG_ERRORF(M4_SNMP_LIBRARY_ERROR, "Slave agent configuration not supported.");
    return NULL;
  //## end M4SnmpAdministratorFactory::getSlaveAdmin%1081250853.body
}

// Additional Declarations
  //## begin M4SnmpAdministratorFactory%40729F95004D.declarations preserve=yes
  //## end M4SnmpAdministratorFactory%40729F95004D.declarations

//## begin module%406BF192016D.epilog preserve=yes
//## end module%406BF192016D.epilog


// Detached code regions:
// WARNING: this code will be lost if code is regenerated.
#if 0
//## begin M4SnmpAdminInstance::Create%1080808689.body preserve=no
//## end M4SnmpAdminInstance::Create%1080808689.body

//## begin M4SnmpAdminInstance::Get%1081265059.body preserve=no
//## end M4SnmpAdminInstance::Get%1081265059.body

#endif
