//## begin module%356568660042.cm preserve=no
//## end module%356568660042.cm

//## begin module%356568660042.cp preserve=no
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
//## end module%356568660042.cp

//## Module: servenv%356568660042; Package body
//## Subsystem: M4ServEnv::src%378B0D2C006C
//## Source file: F:\integration\m4servenv\src\servenv.cpp

//## begin module%356568660042.additionalIncludes preserve=no
//## end module%356568660042.additionalIncludes

//## begin module%356568660042.includes preserve=yes
#include <m4srvres.hpp>
#include <m4log.hpp>
//## end module%356568660042.includes

// NavigatorF
#include <navigatorf.hpp>
// servenv
#include <servenv.hpp>
//## begin module%356568660042.declarations preserve=no
//## end module%356568660042.declarations

//## begin module%356568660042.additionalDeclarations preserve=yes
//#include <clconfiguration.hpp>
//## end module%356568660042.additionalDeclarations


// Class ClServEnv 

//## begin ClServEnv::<m_pClNavigatorFirewall>%35656AA0018C.role preserve=no  public: static ClNavigatorFirewall { -> RHAN}
ClNavigatorFirewall *ClServEnv::m_pClNavigatorFirewall;
//## end ClServEnv::<m_pClNavigatorFirewall>%35656AA0018C.role

ClServEnv::ClServEnv ()
  //## begin ClServEnv::ClServEnv%895835641.hasinit preserve=no
  //## end ClServEnv::ClServEnv%895835641.hasinit
  //## begin ClServEnv::ClServEnv%895835641.initialization preserve=yes
  //## end ClServEnv::ClServEnv%895835641.initialization
{
  //## begin ClServEnv::ClServEnv%895835641.body preserve=yes
  //## end ClServEnv::ClServEnv%895835641.body
}


ClServEnv::~ClServEnv ()
{
  //## begin ClServEnv::~ClServEnv%895835642.body preserve=yes
  //## end ClServEnv::~ClServEnv%895835642.body
}



//## Other Operations (implementation)
m4return_t ClServEnv::GetEnviron (m4pchar_t ai_szPath, m4pchar_t ai_szProperty, m4pchar_t ao_szValue, m4uint32_t ai_iBufflen)
{
  //## begin ClServEnv::GetEnviron%895835643.body preserve=yes
	ClSSInterfazFirewall * poSubsystem = NULL ;
	double dValue = 0 ;
	size_t iLen = 0 ;
	m4return_t iRet ;
	
	if ( ! m_pClNavigatorFirewall )
	{
		return M4_ERROR ;
	}


	poSubsystem = m_pClNavigatorFirewall -> GetSubsystemByName ( ai_szPath ) ;

	if (! poSubsystem )
	{
		return M4_ERROR; 
	}


	iRet = poSubsystem -> GetConfigNumericValue ( ai_szProperty , dValue ) ;

	if (iRet == M4_ERROR )
	{
		iRet = poSubsystem -> GetConfigStringValue ( ai_szProperty , ao_szValue, ai_iBufflen, iLen ) ;

		if ( iRet == M4_ERROR )
		{
			SETCODEF ( M4_SRV_SS_OBL_ERROR , DEBUGINFOLOG , "%s Subsystem hasn't configuration property %s. Trying to get environ variable" , ai_szPath , ai_szProperty  ) ;

			return M4_ERROR ;
		}
	}
	else sprintf ( ao_szValue , "%g" , dValue ) ;


	return M4_SUCCESS ;
  //## end ClServEnv::GetEnviron%895835643.body
}

void ClServEnv::SetNavigator (ClNavigatorFirewall *ai_poNavigator)
{
  //## begin ClServEnv::SetNavigator%895835644.body preserve=yes
	m_pClNavigatorFirewall = ai_poNavigator ;
  //## end ClServEnv::SetNavigator%895835644.body
}

m4return_t ClServEnv::SetEnviron (m4pchar_t ai_szPath, m4pchar_t ai_szProperty, m4pchar_t ai_szValue, m4bool_t ai_bPersistable)
{
  //## begin ClServEnv::SetEnviron%898068452.body preserve=yes
	ClSSInterfazFirewall* poSubsystem = NULL ;
	double dValue = 0 ;
	UINT iLen = 0 ;
	m4return_t iRet ;
	
	if ( ! m_pClNavigatorFirewall )
	{
		return M4_ERROR ;
	}


	poSubsystem = m_pClNavigatorFirewall -> GetSubsystemByName ( ai_szPath ) ;

	if (! poSubsystem )
	{
		return M4_ERROR; 
	}

	iRet = poSubsystem -> SetConfigStringProperty ( ai_szProperty, ai_szValue ) ;

	if (iRet == M4_ERROR )
	{
		return M4_ERROR ;
	}

	if ( ai_bPersistable )
		return m_pClNavigatorFirewall -> PersistConfiguration ( ai_szProperty, ai_szValue , 0 /*poConfiguration -> m_hBlock */) ;


	return M4_SUCCESS ;
  //## end ClServEnv::SetEnviron%898068452.body
}

m4return_t ClServEnv::GetEnviron (m4pchar_t ai_szPath, m4pchar_t ai_szProperty, m4double_t &ao_dValue)
{
  //## begin ClServEnv::GetEnviron%962703854.body preserve=yes
	ClSSInterfazFirewall * poSubsystem = NULL ;
	size_t iLen = 0 ;
	m4return_t iRet ;
	
	if ( ! m_pClNavigatorFirewall )
	{
		return M4_ERROR ;
	}


	poSubsystem = m_pClNavigatorFirewall -> GetSubsystemByName ( ai_szPath ) ;

	if (! poSubsystem )
	{
		return M4_ERROR; 
	}


	iRet = poSubsystem -> GetConfigNumericValue ( ai_szProperty , ao_dValue ) ;

	if (iRet == M4_ERROR )
	{
		SETCODEF ( M4_SRV_SS_OBL_ERROR , DEBUGINFOLOG , "%s Subsystem hasn't configuration property %s. Trying to get environ variable" , ai_szPath , ai_szProperty  ) ;

		return M4_ERROR ;
	}

	return M4_SUCCESS ;
  //## end ClServEnv::GetEnviron%962703854.body
}

// Additional Declarations
  //## begin ClServEnv%356568E0020A.declarations preserve=yes
  //## end ClServEnv%356568E0020A.declarations

//## begin module%356568660042.epilog preserve=yes
//## end module%356568660042.epilog
