//## begin module%36E4F5D20371.cm preserve=no
//## end module%36E4F5D20371.cm

//## begin module%36E4F5D20371.cp preserve=no
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
//## end module%36E4F5D20371.cp

//## Module: OLTP_SubSystem%36E4F5D20371; Package body
//## Subsystem: M4ExeForChannel::OLTP::src%3922763C01B6
//## Source file: F:\integration\m4exeforchannel\src\oltpsubsys.cpp

//## begin module%36E4F5D20371.additionalIncludes preserve=no
//## end module%36E4F5D20371.additionalIncludes

//## begin module%36E4F5D20371.includes preserve=yes
#include <subsystemids.hpp>
#include <exeforchannel.hpp>
#include <oltplauncher.hpp>
#include <cloblconfiguration.hpp>
#include "objdatacacheimpl.hpp"
#include <oltpsubsys.hpp>
#include <executiveforoess.hpp>
#include <clservicebasess.hpp>



//## begin module%36E4F5D20371.declarations preserve=no
//## end module%36E4F5D20371.declarations

//## begin module%36E4F5D20371.additionalDeclarations preserve=yes
//## end module%36E4F5D20371.additionalDeclarations


// Class ClOLTPSubsystem 

//## begin ClOLTPSubsystem::M4OLTPSubsystemName%3912D6AA020C.attr preserve=no  protected: static m4pchar_t {VAC} "OLTP_SERVICE"
const m4pchar_t  ClOLTPSubsystem::M4OLTPSubsystemName = "OLTP_SERVICE";
//## end ClOLTPSubsystem::M4OLTPSubsystemName%3912D6AA020C.attr

ClOLTPSubsystem::ClOLTPSubsystem (ClConfiguration *  ai_poConfiguration, ClTables *  ai_poTables, ClNavigatorFirewall *  ai_pNavigator)
  //## begin ClOLTPSubsystem::ClOLTPSubsystem%920975044.hasinit preserve=no
  //## end ClOLTPSubsystem::ClOLTPSubsystem%920975044.hasinit
  //## begin ClOLTPSubsystem::ClOLTPSubsystem%920975044.initialization preserve=yes
  : ClServiceBaseSS (ai_poConfiguration, ai_poTables, ai_pNavigator )
  //## end ClOLTPSubsystem::ClOLTPSubsystem%920975044.initialization
{
  //## begin ClOLTPSubsystem::ClOLTPSubsystem%920975044.body preserve=yes
  //## end ClOLTPSubsystem::ClOLTPSubsystem%920975044.body
}


ClOLTPSubsystem::~ClOLTPSubsystem ()
{
  //## begin ClOLTPSubsystem::~ClOLTPSubsystem%920975045.body preserve=yes
  //## end ClOLTPSubsystem::~ClOLTPSubsystem%920975045.body
}



//## Other Operations (implementation)
m4return_t ClOLTPSubsystem::Initialize ()
{
  //## begin ClOLTPSubsystem::Initialize%920975040.body preserve=yes
	m4char_t	szType [ OBL_PROPVALUE_MAX_LEN ] ;
	m4size_t	iLen;
		
	if ( GetConfigStringValue ( "TYPE" , szType , OBL_PROPVALUE_MAX_LEN , iLen ) == M4_ERROR )
	{
		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "Cannot find the property 'Type' in subsystem configuration" ) ;
	
		return M4_ERROR ;
	}
/*
	if (strcmp(M4OLTPSubsystemName, szType))
	{
		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "%s found when %s was expected for 'TYPE' property", szType, M4OLTPSubsystemName ) ;
		return M4_ERROR ;
	}
*/
	return ClServiceBaseSS::Initialize () ;
  //## end ClOLTPSubsystem::Initialize%920975040.body
}

ClSSInterfaz * ClOLTPSubsystem::GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables)
{
  //## begin ClOLTPSubsystem::GenerateSubsystem%957346232.body preserve=yes
	ClExecutiveBaseSS * poExecutiveSS;

	if ( ai_lType == M4_EXECUTOR_CHANNEL_SUBSYSTEM)
	{
		poExecutiveSS = new ClExecutiveForChannelSS( ai_poConfiguration , ai_poTables , m_poNavigator , this) ;

		poExecutiveSS  -> SetUseExceptions ( m_bUseExceptions ) ;

		poExecutiveSS  -> SetServiceQ ( m_poServiceQ ) ;

		return poExecutiveSS  -> GetInterfaz ();
	}
	else
	{
		SETCODEF ( M4_SRV_SS_FACTORY_ERROR , ERRORLOG , "OLTP Service Subsystem Don't know how to make subsystem type %d" , ai_lType ) ;
		return NULL ;
	}
	
  //## end ClOLTPSubsystem::GenerateSubsystem%957346232.body
}

// Additional Declarations
  //## begin ClOLTPSubsystem%36E4F4FE0312.declarations preserve=yes
m4return_t ClOLTPSubsystem::ShutDown()
{
    // shutdown de los subsistemas hijos.
    ClServiceBaseSS::ShutDown();

    // Destruimos la cache de autos.
    ClObjDataCacheManager::Destroy();

    return M4_SUCCESS;
}

  //## end ClOLTPSubsystem%36E4F4FE0312.declarations

// Class ClExecutiveForChannelSS 

//## begin ClExecutiveForChannelSS::s_szOLTPInstanceOrder%3912E76A023A.attr preserve=no  protected: static m4size_t {VA} 0
m4size_t ClExecutiveForChannelSS::s_szOLTPInstanceOrder = 0;
//## end ClExecutiveForChannelSS::s_szOLTPInstanceOrder%3912E76A023A.attr

ClExecutiveForChannelSS::ClExecutiveForChannelSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator, ClServiceBaseSS *ai_poServiceSS)
  //## begin ClExecutiveForChannelSS::ClExecutiveForChannelSS%957346233.hasinit preserve=no
  //## end ClExecutiveForChannelSS::ClExecutiveForChannelSS%957346233.hasinit
  //## begin ClExecutiveForChannelSS::ClExecutiveForChannelSS%957346233.initialization preserve=yes
  :ClExecutiveForOESS (ai_poConfiguratoin, ai_poTables, ai_poNavigator , ai_poServiceSS)
  //## end ClExecutiveForChannelSS::ClExecutiveForChannelSS%957346233.initialization
{
  //## begin ClExecutiveForChannelSS::ClExecutiveForChannelSS%957346233.body preserve=yes
  //## end ClExecutiveForChannelSS::ClExecutiveForChannelSS%957346233.body
}


ClExecutiveForChannelSS::~ClExecutiveForChannelSS ()
{
  //## begin ClExecutiveForChannelSS::~ClExecutiveForChannelSS%957346234.body preserve=yes
  //## end ClExecutiveForChannelSS::~ClExecutiveForChannelSS%957346234.body
}



//## Other Operations (implementation)
m4return_t ClExecutiveForChannelSS::Initialize ()
{
  //## begin ClExecutiveForChannelSS::Initialize%957346235.body preserve=yes
	m4return_t		iRet = ClExecutiveForOESS::Initialize ();
	if (M4_SUCCESS != iRet)
		return M4_ERROR;
	
	ClLauncherStats *    poStats ;
	ClExecutive	*		poExecutive;

	m_poStatistics = poStats = new ClLauncherStats ;

	poExecutive = new ClExecutiveForChannel(NULL);

	//Para que los miembros m_poOutputQ y m_poServiceQ estén inicializados se los ha tenido que dar 
	//el SS de servicio correspondiente en el Generate SS

	m_poLauncher = new ClLauncherForOLTP(  poExecutive , poStats, /*ai_poLog, m_poServiceQ*/ m_poServiceSS);

	m_szInstanceOrder = ++s_szOLTPInstanceOrder;

	return M4_SUCCESS;
  //## end ClExecutiveForChannelSS::Initialize%957346235.body
}

// Additional Declarations
  //## begin ClExecutiveForChannelSS%391049CD02DA.declarations preserve=yes
  //## end ClExecutiveForChannelSS%391049CD02DA.declarations

//## begin module%36E4F5D20371.epilog preserve=yes
//## end module%36E4F5D20371.epilog
