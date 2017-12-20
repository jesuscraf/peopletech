//## begin module%407BC94A023C.cm preserve=no
//## end module%407BC94A023C.cm

//## begin module%407BC94A023C.cp preserve=no
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
//## end module%407BC94A023C.cp

//## Module: snmpss%407BC94A023C; Package body
//## Subsystem: M4SnmpSS::src%407BC8F200EB
//## Source file: z:\m4snmpss\src\snmpss.cpp

//## begin module%407BC94A023C.additionalIncludes preserve=no
//## end module%407BC94A023C.additionalIncludes

//## begin module%407BC94A023C.includes preserve=yes
#include "snmpadmininstance.hpp"
#include "snmpres.hpp"
//## end module%407BC94A023C.includes

// clssimplementacion
#include <clssimplementacion.hpp>
// snmpss
#include <snmpss.hpp>
// ClOblConfiguration
#include <cloblconfiguration.hpp>
//## begin module%407BC94A023C.declarations preserve=no
//## end module%407BC94A023C.declarations

//## begin module%407BC94A023C.additionalDeclarations preserve=yes
//## end module%407BC94A023C.additionalDeclarations


// Class M4SnmpSS 

M4SnmpSS::M4SnmpSS (ClConfiguration *ai_poConfiguration, ClTables* ai_poTables, ClNavigatorFirewall* ai_poNavigator)
  //## begin M4SnmpSS::M4SnmpSS%1081248170.hasinit preserve=no
  //## end M4SnmpSS::M4SnmpSS%1081248170.hasinit
  //## begin M4SnmpSS::M4SnmpSS%1081248170.initialization preserve=yes
    :ClSSImplementation(ai_poConfiguration, ai_poTables, ai_poNavigator, M4_FALSE)
  //## end M4SnmpSS::M4SnmpSS%1081248170.initialization
{
  //## begin M4SnmpSS::M4SnmpSS%1081248170.body preserve=yes
  //## end M4SnmpSS::M4SnmpSS%1081248170.body
}


M4SnmpSS::~M4SnmpSS()
{
  //## begin M4SnmpSS::~M4SnmpSS%.body preserve=yes
  //## end M4SnmpSS::~M4SnmpSS%.body
}



//## Other Operations (implementation)
m4return_t M4SnmpSS::Initialize ()
{
  //## begin M4SnmpSS::Initialize%1081322813.body preserve=yes
    m4return_t retCode = M4_SUCCESS;

    // read "ACTIVE" from obl.
    m4size_t iLen;
    m4char_t szValue [OBL_PROPVALUE_MAX_LEN];
    retCode = GetConfigStringValue("ACTIVE", szValue, OBL_PROPVALUE_MAX_LEN, iLen);
    if (retCode == M4_ERROR)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_CANNOT_GET_OBL_ITEM, "ACTIVE");
        return M4_ERROR;
    }

    // if "ACTIVE" property is != true, does not create the snmp administrator
    if (stricmp(szValue, "TRUE"))
    {
        return M4_SUCCESS;
    }
    
    // creates the snmp administrator.
    retCode = M4SnmpAdminInstance::Create(this->GetConfiguration());
    if (retCode == M4_ERROR)
    {
        DUMP_CHLOG_ERRORF(M4_SNMP_MEMORY_ALLOCATION_ERROR, "M4SnmpAdminInstance::Create(this->GetConfiguration())");
        return M4_ERROR;
    }

    // starts the snmp administrator.
    M4ISnmpAdministrator* poSnmpAdmin = M4SnmpAdminInstance::Get();
    if (NULL != poSnmpAdmin)
    {
        retCode = poSnmpAdmin->Start();
    }

    return retCode;
  //## end M4SnmpSS::Initialize%1081322813.body
}

m4return_t M4SnmpSS::Stop (m4uint32_t ai_lDeferedTime)
{
  //## begin M4SnmpSS::Stop%1081862814.body preserve=yes
    // starts the snmp administrator.
    m4return_t retCode = M4_SUCCESS;
    M4ISnmpAdministrator* poSnmpAdmin = M4SnmpAdminInstance::Get();
    if (NULL != poSnmpAdmin)
    {
        retCode = poSnmpAdmin->Stop();
    }

    return retCode;
  //## end M4SnmpSS::Stop%1081862814.body
}

m4return_t M4SnmpSS::ShutDown ()
{
  //## begin M4SnmpSS::ShutDown%1081862815.body preserve=yes
    return M4_SUCCESS;
  //## end M4SnmpSS::ShutDown%1081862815.body
}

m4return_t M4SnmpSS::Start ()
{
  //## begin M4SnmpSS::Start%1081862816.body preserve=yes
    return M4_SUCCESS;
  //## end M4SnmpSS::Start%1081862816.body
}

m4bool_t M4SnmpSS::IsReadyToClose ()
{
  //## begin M4SnmpSS::IsReadyToClose%1081862817.body preserve=yes
    return M4_TRUE;
  //## end M4SnmpSS::IsReadyToClose%1081862817.body
}

// Additional Declarations
  //## begin M4SnmpSS%4072868E0271.declarations preserve=yes
  //## end M4SnmpSS%4072868E0271.declarations

//## begin module%407BC94A023C.epilog preserve=yes
//## end module%407BC94A023C.epilog
