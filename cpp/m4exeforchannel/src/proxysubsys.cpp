//## begin module%359C8AAE0237.cm preserve=no
//## end module%359C8AAE0237.cm

//## begin module%359C8AAE0237.cp preserve=no
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
//## end module%359C8AAE0237.cp

//## Module: Proxy_SubSystem%359C8AAE0237; Package body
//## Subsystem: M4ExeForChannel::Proxy::src%392274340168
//## Source file: F:\integration\m4exeforchannel\src\proxysubsys.cpp

//## begin module%359C8AAE0237.additionalIncludes preserve=no
//## end module%359C8AAE0237.additionalIncludes

//## begin module%359C8AAE0237.includes preserve=yes
#include <subsystemids.hpp>
#include <m4srvres.hpp>
#include "m4log.hpp"
#include "cloblconfiguration.hpp"
#include "navigatorf.hpp"
#include "oltplauncher.hpp"
#include "exeforproxy.hpp"
#include <proxspacsss.hpp>
#include <executiveforoess.hpp>
#include <proxysubsys.hpp>
#include <clservicebasess.hpp>

//## begin module%359C8AAE0237.declarations preserve=no
//## end module%359C8AAE0237.declarations

//## begin module%359C8AAE0237.additionalDeclarations preserve=yes
//## end module%359C8AAE0237.additionalDeclarations


// Class ClProxySubSystem 

//## begin ClProxySubSystem::M4ProxySubsystemName%391BC89A00E0.attr preserve=no  protected: static m4pchar_t {UAC} "PROXY_SERVICE"
const m4pchar_t  ClProxySubSystem::M4ProxySubsystemName = "PROXY_SERVICE";
//## end ClProxySubSystem::M4ProxySubsystemName%391BC89A00E0.attr

ClProxySubSystem::ClProxySubSystem (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
  //## begin ClProxySubSystem::ClProxySubSystem%899454531.hasinit preserve=no
  //## end ClProxySubSystem::ClProxySubSystem%899454531.hasinit
  //## begin ClProxySubSystem::ClProxySubSystem%899454531.initialization preserve=yes
  : ClServiceBaseSS(ai_poConfiguratoin, ai_poTables, ai_poNavigator)
  //## end ClProxySubSystem::ClProxySubSystem%899454531.initialization
{
  //## begin ClProxySubSystem::ClProxySubSystem%899454531.body preserve=yes
  //## end ClProxySubSystem::ClProxySubSystem%899454531.body
}


ClProxySubSystem::~ClProxySubSystem ()
{
  //## begin ClProxySubSystem::~ClProxySubSystem%899454532.body preserve=yes

	// makes a shutdown
	ShutDown();
  //## end ClProxySubSystem::~ClProxySubSystem%899454532.body
}



//## Other Operations (implementation)
ClSSInterfaz * ClProxySubSystem::GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables)
{
  //## begin ClProxySubSystem::GenerateSubsystem%958122096.body preserve=yes
	ClSSImplementation * poImplementation ;
//	ClExecutiveBaseSS * poExecutiveSS ;

	switch ( ai_lType )
	{
	case M4_PROXY_SPACES_SUBSYSTEM :
		poImplementation = new ClProxySpacesSS(ai_poConfiguration , ai_poTables , m_poNavigator );

		if (poImplementation )
		{
			SETCODEF( M4_SRV_STARTUP_SUCCESS, DEBUGINFOLOG, "Proxy Spaces Subsystem created" );
			cout << "Proxy Spaces Subsystem created" << endl;
		}
		break;

	case M4_EXECUTOR_PROXY_SUBSYSTEM :

		poImplementation = new ClExecutiveForProxySS( ai_poConfiguration , ai_poTables , m_poNavigator , this ) ;

		(( ClExecutiveBaseSS * ) poImplementation ) -> SetUseExceptions ( m_bUseExceptions ) ;

		(( ClExecutiveBaseSS * ) poImplementation ) -> SetServiceQ ( m_poServiceQ ) ;

		break ;

	default :

		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR, ERRORLOG , "Proxy Service Subsystem Don't know how to make subsystem type %d" , ai_lType ) ;
		return NULL ;
	}

	return poImplementation -> GetInterfaz ();
	
  //## end ClProxySubSystem::GenerateSubsystem%958122096.body
}

m4return_t ClProxySubSystem::Initialize ()
{
  //## begin ClProxySubSystem::Initialize%958122097.body preserve=yes
	m4char_t	szType [ OBL_PROPVALUE_MAX_LEN ] ;
	m4size_t	iLen;
		
	if ( GetConfigStringValue ( "TYPE" , szType , OBL_PROPVALUE_MAX_LEN , iLen ) == M4_ERROR )
	{
		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "Cannot find the property 'Type' in subsystem configuration" ) ;
	
		return M4_ERROR ;
	}
/*
	if (strcmp(M4ProxySubsystemName, szType))
	{
		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "%s found when %s was expected for 'TYPE' property", szType, M4ProxySubsystemName ) ;
		return M4_ERROR ;
	}
*/
	return ClServiceBaseSS::Initialize () ;

  //## end ClProxySubSystem::Initialize%958122097.body
}

// Additional Declarations
  //## begin ClProxySubSystem%359C8B9E011A.declarations preserve=yes
  //## end ClProxySubSystem%359C8B9E011A.declarations

// Class ClExecutiveForProxySS 

//## begin ClExecutiveForProxySS::s_szProxyInstanceOrder%391BD23002C9.attr preserve=no  protected: static m4size_t {UA} 0
m4size_t ClExecutiveForProxySS::s_szProxyInstanceOrder = 0;
//## end ClExecutiveForProxySS::s_szProxyInstanceOrder%391BD23002C9.attr

ClExecutiveForProxySS::ClExecutiveForProxySS (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator, ClServiceBaseSS *ai_poServiceSS)
  //## begin ClExecutiveForProxySS::ClExecutiveForProxySS%958122098.hasinit preserve=no
  //## end ClExecutiveForProxySS::ClExecutiveForProxySS%958122098.hasinit
  //## begin ClExecutiveForProxySS::ClExecutiveForProxySS%958122098.initialization preserve=yes
  :ClExecutiveForOESS (ai_poConfiguration, ai_poTables, ai_poNavigator , ai_poServiceSS )
  //## end ClExecutiveForProxySS::ClExecutiveForProxySS%958122098.initialization
{
  //## begin ClExecutiveForProxySS::ClExecutiveForProxySS%958122098.body preserve=yes
  //## end ClExecutiveForProxySS::ClExecutiveForProxySS%958122098.body
}


ClExecutiveForProxySS::~ClExecutiveForProxySS ()
{
  //## begin ClExecutiveForProxySS::~ClExecutiveForProxySS%958122099.body preserve=yes
  //## end ClExecutiveForProxySS::~ClExecutiveForProxySS%958122099.body
}



//## Other Operations (implementation)
m4return_t ClExecutiveForProxySS::Initialize ()
{
  //## begin ClExecutiveForProxySS::Initialize%958122100.body preserve=yes
	m4return_t		iRet = ClExecutiveForOESS::Initialize ();
	if (M4_SUCCESS != iRet)
		return M4_ERROR;
	
	ClLauncherStats *    poStats ;
	ClExecutive	*		poExecutive;

	m_poStatistics = poStats = new ClLauncherStats ;

	poExecutive = new ClExecutiveForProxy(NULL);

	//Para que los miembros m_poOutputQ y m_poServiceQ estén inicializados se los ha tenido que dar 
	//el SS de servicio correspondiente en el Generate SS

	m_poLauncher = new ClLauncherForOLTP(  poExecutive , poStats, /*ai_poLog, m_poServiceQ*/m_poServiceSS);

	m_szInstanceOrder = ++s_szProxyInstanceOrder;

	return M4_SUCCESS;
  //## end ClExecutiveForProxySS::Initialize%958122100.body
}

// Additional Declarations
  //## begin ClExecutiveForProxySS%391BD1D80223.declarations preserve=yes
  //## end ClExecutiveForProxySS%391BD1D80223.declarations

//## begin module%359C8AAE0237.epilog preserve=yes
//## end module%359C8AAE0237.epilog
