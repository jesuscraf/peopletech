//## begin module%35580A66034C.cm preserve=no
//## end module%35580A66034C.cm

//## begin module%35580A66034C.cp preserve=no
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
//## end module%35580A66034C.cp

//## Module: NavigatorSS%35580A66034C; Package body
//## Subsystem: version::src%379ECBD7029A
//## Source file: F:\integration\m4comsrv\src\navigatorss.cpp

//## begin module%35580A66034C.additionalIncludes preserve=no
//## end module%35580A66034C.additionalIncludes

//## begin module%35580A66034C.includes preserve=yes
#include <m4srvres.hpp>
#include <subsystemids.hpp>
#include <cachesubsystem.hpp>
#include <clsubsystem.hpp>
#include <m4log.hpp>
#include <cllocalmonitor.hpp>
//## end module%35580A66034C.includes

// NavigatorF
#include <navigatorf.hpp>
// NavigatorSS
#include <navigatorss.hpp>


//## begin module%35580A66034C.declarations preserve=no
//## end module%35580A66034C.declarations

//## begin module%35580A66034C.additionalDeclarations preserve=yes
//## end module%35580A66034C.additionalDeclarations


// Class ClNavigatorSS 


//## Other Operations (implementation)
ClSSInterfazFirewall * ClNavigatorSS::GetSubsystemByType (m4uint32_t ai_iID)
{
  //## begin ClNavigatorSS::GetSubsystemByType%894962375.body preserve=yes
	ClSSInterfaz * poSubsystem = NULL ;
	ClSystem * poSystem ;


	poSystem = ClSystem::Instance ( ) ;

	poSubsystem = poSystem -> FindSubsystemByType  ( ai_iID ) ;

	if ( ! poSubsystem )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , DEBUGINFOLOG , "Navigator couldn't find subsystem with ID %d" , ai_iID ) ;

		return NULL ;
	}


	return poSubsystem ; 
	
  //## end ClNavigatorSS::GetSubsystemByType%894962375.body
}

ClSSInterfazFirewall * ClNavigatorSS::GetSubsystemByName (m4pchar_t ai_szName)
{
  //## begin ClNavigatorSS::GetSubsystemByName%895825800.body preserve=yes
	ClSystem * poSystem ;
	ClSSInterfaz* poSubsystem = NULL ;


	poSystem = ClSystem::Instance ( ) ;

	if ( ! ( poSubsystem = poSystem -> FindSubsystemByName ( ai_szName ) ) )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , DEBUGINFOLOG , "Navigator couldn't find subsystem named %s" , ai_szName ) ;

		return NULL ;
	}

	


	return poSubsystem ;
  //## end ClNavigatorSS::GetSubsystemByName%895825800.body
}

void * ClNavigatorSS::GetSubsystemElement (m4pchar_t ai_szSubsystemName, m4pchar_t ai_szElementName)
{
  //## begin ClNavigatorSS::GetSubsystemElement%896526471.body preserve=yes
	ClSSInterfaz * poAtribSubsystem = NULL ;
	
	poAtribSubsystem =  (ClSSInterfaz *) GetSubsystemByName (ai_szSubsystemName) ;

	if (! poAtribSubsystem )
	{
		return NULL ;
	}


	return poAtribSubsystem -> GetElement ( ai_szElementName ) ;
  //## end ClNavigatorSS::GetSubsystemElement%896526471.body
}

m4return_t ClNavigatorSS::PersistConfiguration (m4pchar_t ai_szProperty, m4pchar_t ai_szValue, UINT ai_hBlock)
{
  //## begin ClNavigatorSS::PersistConfiguration%901796091.body preserve=yes
	ClSystem * poSystem ;


	poSystem = ClSystem::Instance ( ) ;


	return poSystem -> PersistConfiguration ( ai_szProperty, ai_szValue, ai_hBlock ) ;

  //## end ClNavigatorSS::PersistConfiguration%901796091.body
}

m4return_t ClNavigatorSS::RegisterSubsystem (m4uint32_t ai_lIdParent, m4uint32_t ai_lSubsystemType, ClConfiguration *ai_poConfiguration, ClSSInterfaz * &ao_poSubsystem)
{
  //## begin ClNavigatorSS::RegisterSubsystem%901796092.body preserve=yes
	ClSystem * poSystem ;
	ClSSInterfaz * poSubsystem = NULL ;
	ClSSInterfaz * poContainer = NULL ;


	poSystem = ClSystem::Instance () ;

	if ( ! ( poSubsystem = poSystem -> GenerateSubsystem ( (m4uint16_t) ai_lSubsystemType , ai_poConfiguration  ) ) )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , DEBUGINFOLOG , "Navigator couldn't Generate subsystem of type %d" , ai_lSubsystemType  ) ;

		return M4_ERROR; 
	}

	if ( ! ( poContainer = poSystem -> FindSubsystemById (  ai_lIdParent  ) ) )
	{
		return M4_ERROR ;
	}

	if ( M4_ERROR == poContainer -> AddSubsystem ( poSubsystem ) )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Navigator couldn't add subsystem into subsystem with ID %d" , ai_lIdParent   ) ;

		return M4_ERROR; 
	}

	ao_poSubsystem = poSubsystem ;

	return M4_SUCCESS ;
  //## end ClNavigatorSS::RegisterSubsystem%901796092.body
}

ClSSInterfazFirewall * ClNavigatorSS::GetSubsystemById (m4uint32_t ai_lId)
{
  //## begin ClNavigatorSS::GetSubsystemById%901966069.body preserve=yes
	ClSSInterfaz * poSubsystem = NULL ;
	ClSSInterfaz* poSubsystemBase = NULL ;
	ClSystem * poSystem ;


	poSystem = ClSystem::Instance ( ) ;

	poSubsystem = poSystem ->FindSubsystemById ( ai_lId ) ;

	if ( ! poSubsystem )
	{
		SETCODEF ( M4_SRV_SS_TREE_ERROR , DEBUGINFOLOG , "Navigator couldn't find subsystem with ID %d" , ai_lId ) ;

		return NULL ;
	}

	poSubsystemBase = poSubsystem -> GetSubsystem ( ) ;

	return poSubsystemBase ;
	
  //## end ClNavigatorSS::GetSubsystemById%901966069.body
}

m4return_t ClNavigatorSS::RegisterSubsystem (ClSSInterfaz *  ai_poContainer, ClSSInterfaz *  ai_poNewNode)
{
  //## begin ClNavigatorSS::RegisterSubsystem%918583518.body preserve=yes
	m4objid_t iContainerId  ;
	ClSSInterfaz * poContainer ;
	ClSystem * poSystem ;


	poSystem = ClSystem::Instance ( ) ;
	
	iContainerId = ai_poContainer -> GetId () ;

	if ( ! ( poContainer = poSystem -> FindSubsystemById ( iContainerId  )) )
	{
		return M4_ERROR ;
	}


	poContainer -> AddSubsystem ( ai_poNewNode ) ;


//	poNewSubsystem -> SetType ( ai_poNewNode -> GetType ( ) ) ;
	

	return  M4_SUCCESS ;
  //## end ClNavigatorSS::RegisterSubsystem%918583518.body
}

ClLocalMonitorInterface * ClNavigatorSS::GetLocalMonitor ()
{
  //## begin ClNavigatorSS::GetLocalMonitor%958638813.body preserve=yes
	ClLocalMonitor * poLocalMonitor = ClLocalMonitor::Instance();

	return poLocalMonitor ;
  //## end ClNavigatorSS::GetLocalMonitor%958638813.body
}

// Additional Declarations
  //## begin ClNavigatorSS%3558080F0318.declarations preserve=yes
  //## end ClNavigatorSS%3558080F0318.declarations

//## begin module%35580A66034C.epilog preserve=yes
//## end module%35580A66034C.epilog
