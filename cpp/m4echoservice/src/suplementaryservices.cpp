//## begin module%38E066670136.cm preserve=no
//## end module%38E066670136.cm

//## begin module%38E066670136.cp preserve=no
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
//## end module%38E066670136.cp

//## Module: suplementaryservices%38E066670136; Package body
//## Subsystem: M4echoservice::src%38DF87440246
//## Source file: F:\integration\m4echoservice\src\suplementaryservices.cpp

//## begin module%38E066670136.additionalIncludes preserve=no
//## end module%38E066670136.additionalIncludes

//## begin module%38E066670136.includes preserve=yes
#include <echoservicess.hpp>
#include <subsystemids.hpp>
#include <cloblconfiguration.hpp>
//## end module%38E066670136.includes

// suplementaryservices
#include <suplementaryservices.hpp>
// Statistics
#include <statistics.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>


//## begin module%38E066670136.declarations preserve=no
//## end module%38E066670136.declarations

//## begin module%38E066670136.additionalDeclarations preserve=yes
const m4uint32_t MAX_SUPLEMENTARY_SERVICE_SUBSYSTEM_NUM_INSTANCES = 1 ;
//## end module%38E066670136.additionalDeclarations


// Class ClSuplementaryServicesSS 

ClSuplementaryServicesSS::ClSuplementaryServicesSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
  //## begin ClSuplementaryServicesSS::ClSuplementaryServicesSS%882177081.hasinit preserve=no
  //## end ClSuplementaryServicesSS::ClSuplementaryServicesSS%882177081.hasinit
  //## begin ClSuplementaryServicesSS::ClSuplementaryServicesSS%882177081.initialization preserve=yes
  :ClSSImplementation (ai_poConfiguratoin, ai_poTables , ai_poNavigator)
  //## end ClSuplementaryServicesSS::ClSuplementaryServicesSS%882177081.initialization
{
  //## begin ClSuplementaryServicesSS::ClSuplementaryServicesSS%882177081.body preserve=yes
  //## end ClSuplementaryServicesSS::ClSuplementaryServicesSS%882177081.body
}


ClSuplementaryServicesSS::~ClSuplementaryServicesSS ()
{
  //## begin ClSuplementaryServicesSS::~ClSuplementaryServicesSS%882177082.body preserve=yes
  //## end ClSuplementaryServicesSS::~ClSuplementaryServicesSS%882177082.body
}



//## Other Operations (implementation)
void ClSuplementaryServicesSS::Accept (ClSuplementaryServicesVisitor &ai_oVisitor)
{
  //## begin ClSuplementaryServicesSS::Accept%884777559.body preserve=yes
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return   ;

	ai_oVisitor.VisitServiceList ( * this ) ;
  //## end ClSuplementaryServicesSS::Accept%884777559.body
}

m4return_t ClSuplementaryServicesSS::Synchronize ()
{
  //## begin ClSuplementaryServicesSS::Synchronize%884942697.body preserve=yes
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	
	if ( m_poVisitor )
	{
		Accept ( * ( ( ClSuplementaryServicesVisitor * ) m_poVisitor ) ) ;
	}

	return M4_SUCCESS; 
  //## end ClSuplementaryServicesSS::Synchronize%884942697.body
}

m4return_t ClSuplementaryServicesSS::Initialize ()
{
  //## begin ClSuplementaryServicesSS::Initialize%885806474.body preserve=yes
	eStatus eState ;
	m4double_t iNumInstances ;
	m4char_t			szType [ OBL_PROPVALUE_MAX_LEN ] ;
	m4size_t iLen ;

	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	
	if ( GetConfigStringValue ( "TYPE" , szType , OBL_PROPVALUE_MAX_LEN , iLen ) == M4_ERROR )
	{
		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "Cannot find the property 'Type' in subsystem configuration" ) ;
	
		return M4_ERROR ;
	}


	if ( GetConfigNumericValue ( NUM_INSTANCES , iNumInstances ) == M4_SUCCESS )
	{

		if ( iNumInstances >= MAX_SUPLEMENTARY_SERVICE_SUBSYSTEM_NUM_INSTANCES )
		{
			SETCODEF ( M4_SRV_STARTUPOBL_ERROR, ERRORLOG , "NUM_INSTANCES property of subsystem %s is asigned to %D, and it is and illegal value. Mximun allowed is %d", szType , iNumInstances , MAX_SUPLEMENTARY_SERVICE_SUBSYSTEM_NUM_INSTANCES  ) ;
			return M4_ERROR ;
		}
	}
	
	return M4_SUCCESS ;
  //## end ClSuplementaryServicesSS::Initialize%885806474.body
}

m4return_t ClSuplementaryServicesSS::ShutDown ()
{
  //## begin ClSuplementaryServicesSS::ShutDown%885806519.body preserve=yes
	return M4_SUCCESS ;
  //## end ClSuplementaryServicesSS::ShutDown%885806519.body
}

m4return_t ClSuplementaryServicesSS::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClSuplementaryServicesSS::ExecuteCommand%901709443.body preserve=yes
	return M4_SUCCESS ;
  //## end ClSuplementaryServicesSS::ExecuteCommand%901709443.body
}

m4return_t ClSuplementaryServicesSS::GetCommandsId (ClAdminTagList &aio_oCommandList)
{
  //## begin ClSuplementaryServicesSS::GetCommandsId%901709444.body preserve=yes
	return M4_SUCCESS ;
  //## end ClSuplementaryServicesSS::GetCommandsId%901709444.body
}

m4return_t ClSuplementaryServicesSS::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList)
{
  //## begin ClSuplementaryServicesSS::GetCommandParams%901709445.body preserve=yes
	return M4_SUCCESS ;
  //## end ClSuplementaryServicesSS::GetCommandParams%901709445.body
}

ClSSInterfaz * ClSuplementaryServicesSS::GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables)
{
  //## begin ClSuplementaryServicesSS::GenerateSubsystem%954230996.body preserve=yes
	ClECHOServiceSS* poServiceSS = NULL;

	if (ai_lType == M4_ECHO_SERVICE_SUBSYSTEM)
	{
		poServiceSS = new ClECHOServiceSS ( ai_poConfiguration , ai_poTables , m_poNavigator) ;

		return poServiceSS  -> GetInterfaz ();
	}
	else
	{
		SETCODEF ( M4_SRV_SS_FACTORY_ERROR , ERRORLOG , "ECHO Service Subsystem Don't know how to make subsystem type %d" , ai_lType ) ;
		return NULL ;
	}
  //## end ClSuplementaryServicesSS::GenerateSubsystem%954230996.body
}

// Class ClSuplementaryServicesVisitor 


//## Other Operations (implementation)
void ClSuplementaryServicesVisitor::VisitServiceList (ClSuplementaryServicesSS &ai_oServiceList)
{
  //## begin ClSuplementaryServicesVisitor::VisitServiceList%885288584.body preserve=yes
  //## end ClSuplementaryServicesVisitor::VisitServiceList%885288584.body
}

//## begin module%38E066670136.epilog preserve=yes
//## end module%38E066670136.epilog
