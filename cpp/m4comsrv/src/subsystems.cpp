//## begin module%349293A5024D.cm preserve=no
//## end module%349293A5024D.cm

//## begin module%349293A5024D.cp preserve=no
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
//## end module%349293A5024D.cp

//## Module: subsystems%349293A5024D; Package body
//## Subsystem: version::src%379ECBD7029A
//## Source file: F:\integration\m4comsrv\src\subsystems.cpp

//## begin module%349293A5024D.additionalIncludes preserve=no
//## end module%349293A5024D.additionalIncludes

//## begin module%349293A5024D.includes preserve=yes
#include <m4srvres.hpp>
#include <subsystemids.hpp>
#include <navigatorss.hpp>
#include <cloblconfiguration.hpp>
#include <propertiestable.hpp>
#include <m4servenv.hpp>
#include <request.hpp>
#include <tsap.hpp>
#include <ssnames.hpp>
#include <connbase.hpp>
#include <ssvisitors.hpp>
#include <cllocalmonitor.hpp>
#include <ssstats.hpp>
#include <clserviceinfo.hpp>
#include <configtree.hpp>
#include <lmstats.hpp>
#include <clsupervisor.hpp>
#include <boomrequest.hpp>
#include <commandrequest.hpp>
#include <cladministrator.hpp>
#include <clssinterfaz.hpp>
#include <servicestats.hpp>
#include <launcher.hpp>
#include <clssinvoker.hpp>
#include <clsubsystem.hpp>
#include <m4srvtrace.h>
//## end module%349293A5024D.includes

// ClConfiguration
#include <clconfiguration.hpp>
// subsystems
#include <subsystems.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>


//## begin module%349293A5024D.declarations preserve=no
//## end module%349293A5024D.declarations

//## begin module%349293A5024D.additionalDeclarations preserve=yes
#define MAXBLOCKS 100


#define M4_SRV_TRACE_COMP_ID	"COMSRV"
//## end module%349293A5024D.additionalDeclarations


// Class ClSupervisorSS 




ClSupervisorSS::ClSupervisorSS (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
  //## begin ClSupervisorSS::ClSupervisorSS%897935265.hasinit preserve=no
  //## end ClSupervisorSS::ClSupervisorSS%897935265.hasinit
  //## begin ClSupervisorSS::ClSupervisorSS%897935265.initialization preserve=yes
  : ClSSImplementation ( ai_poConfiguration, ai_poTables , ai_poNavigator ),m_poSupervisor (NULL)
  //## end ClSupervisorSS::ClSupervisorSS%897935265.initialization
{
  //## begin ClSupervisorSS::ClSupervisorSS%897935265.body preserve=yes
	m_bReadyToClose = M4_FALSE ;

	m_poCommandsConfig = NULL ;
  //## end ClSupervisorSS::ClSupervisorSS%897935265.body
}


ClSupervisorSS::~ClSupervisorSS ()
{
  //## begin ClSupervisorSS::~ClSupervisorSS%897935266.body preserve=yes


	if ( m_poSupervisor )
	{
		delete m_poSupervisor ;

		m_poSupervisor = NULL ;
	}
  //## end ClSupervisorSS::~ClSupervisorSS%897935266.body
}



//## Other Operations (implementation)
m4return_t ClSupervisorSS::ShutDown ()
{
  //## begin ClSupervisorSS::ShutDown%897935268.body preserve=yes
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
	{
		return M4_ERROR ;
	}
	else
	{
		SetState ( SS_NOT_ACCESIBLE , SS_STR_NOT_ACCESIBLE) ;
	}


	if ( m_poSupervisor )
	{
		m_poSupervisor -> Terminate ( ) ;

		if ( m_poSupervisor -> IsRunning ( ) )
		{
			m4sleep(1);
			SetState ( SS_ACTIVE , SS_STR_ACTIVE);
			return M4_ERROR ;
		}

		delete m_poSupervisor ;

		m_poSupervisor = NULL ;
	}

	return M4_SUCCESS ;
  //## end ClSupervisorSS::ShutDown%897935268.body
}

m4return_t ClSupervisorSS::Synchronize ()
{
  //## begin ClSupervisorSS::Synchronize%897935269.body preserve=yes
	m4char_t szHinst [OBL_PROPVALUE_MAX_LEN] = "";
	size_t len ;

	if ( m_poSupervisor )
	{

		if ( m_poSupervisor -> IsExit () )
		{
//			(( ClM4ConfigTree*) m_poConfiguration ) -> GetInstance ( szHinst , OBL_PROPVALUE_MAX_LEN , len ) ;
			GetInstance ( szHinst , OBL_PROPVALUE_MAX_LEN , len ) ;

			// bugid: 0087079
            SETCODEF (M4_EXCEPTION_CAUGHT, ERRORLOG, "#*s1*#%d#%s#%s#%s#", 0, szHinst, "Meta4Server Supervisor", "Starting again");

			m_poSupervisor -> Start () ;
		}	
	}
	return M4_SUCCESS ;
  //## end ClSupervisorSS::Synchronize%897935269.body
}

m4return_t ClSupervisorSS::Initialize ()
{
  //## begin ClSupervisorSS::Initialize%897935270.body preserve=yes
	ClSystem * poSystem = ClSystem::Instance ( ) ;
	m4char_t szName [OBL_PROPVALUE_MAX_LEN];
	m4char_t szActive [OBL_PROPVALUE_MAX_LEN];
	m4size_t iLen ;


	GetInstance ( szName , OBL_PROPVALUE_MAX_LEN , iLen);

	SetName ( szName ) ;


	if ( M4_SUCCESS == GetConfigStringValue ( "ACTIVE" , szActive , OBL_PROPVALUE_MAX_LEN , iLen ) )
	{
		if ( ! stricmp ( szActive , "false" ) )
		{
			SetState ( SS_INACTIVE , SS_STR_INACTIVE ) ;
		}
		else 
		{
			if (  stricmp ( szActive , "true" ) )
			{
				M4_SRV_WARNING( M4_SRV_CONFIG_PROP_VALUEWARNING , "Configuration property: %0:s in Subsystem: %1:s, was asigned a no valid value of %2:s. Only accepted values in range: %3:s - %4:s", "ACTIVE" << "SUPERVISOR" << szActive  << "TRUE" << "FALSE"<< "TRUE");
//				SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "ACTIVE property in supervisor %s. Wrong Value %s. Only accepted TRUE or FALSE", szName, szActive ) ;
			}
		}
	}
	else
	{
		M4_SRV_WARNING ( M4_SRV_CONFIG_PROP_WARNING , "Configuration property: %0:s not found in Subsystem %1:s","ACTIVE" << "SUPERVISOR" << "TRUE" );
//		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , DEBUGINFOLOG , "ACTIVE property not found in supervisor %s. Default TRUE (ACTIVE)", szName) ;
	}


	return M4_SUCCESS ;
  //## end ClSupervisorSS::Initialize%897935270.body
}

ClCSSupervisor * ClSupervisorSS::GetSupervisor ()
{
  //## begin ClSupervisorSS::GetSupervisor%897935272.body preserve=yes
	return m_poSupervisor ;
  //## end ClSupervisorSS::GetSupervisor%897935272.body
}

void ClSupervisorSS::SetSupervisor (ClCSSupervisor *ai_poSupervisor)
{
  //## begin ClSupervisorSS::SetSupervisor%897935273.body preserve=yes
	m_poSupervisor = ai_poSupervisor ;
  //## end ClSupervisorSS::SetSupervisor%897935273.body
}

m4return_t ClSupervisorSS::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClSupervisorSS::ExecuteCommand%901709476.body preserve=yes
	return M4_SUCCESS ;
  //## end ClSupervisorSS::ExecuteCommand%901709476.body
}

m4return_t ClSupervisorSS::GetCommandsId (ClAdminTagList &aio_oCommandList)
{
  //## begin ClSupervisorSS::GetCommandsId%901709477.body preserve=yes
	return M4_SUCCESS ;
  //## end ClSupervisorSS::GetCommandsId%901709477.body
}

m4return_t ClSupervisorSS::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList)
{
  //## begin ClSupervisorSS::GetCommandParams%901709478.body preserve=yes
	return M4_SUCCESS ;
  //## end ClSupervisorSS::GetCommandParams%901709478.body
}

m4return_t ClSupervisorSS::Start ()
{
  //## begin ClSupervisorSS::Start%927293869.body preserve=yes
	m4char_t szSupervisor [MAXNAMEPROP];
	static int lIdThread =1 ;
	m4return_t iRet = M4_SUCCESS ;
	eStatus eState ;
	m4double_t iDelay = 0 ;
	ClSystem * poSystem = ClSystem::Instance () ;


	if ( m_poSupervisor )
	{
		return M4_ERROR ;
	}

	GetState ( eState  ) ;

	if ( eState == SS_ACTIVE )
	{
		if ( M4_ERROR == GetConfigNumericValue ( "DELAY_TIME" , iDelay ) )
		{
			M4_SRV_ERROR ( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s","DELAY_TIME" << GetName () );
//			SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "DELAY TIME property not found in supervisor %s", GetName () ) ;

			return M4_ERROR ;
		}

		m_poSupervisor = new ClCSSupervisor ( (m4uint32_t)iDelay  ) ;

		poSystem -> AttachObserver ( this ) ;

		if ( m_poCommandsConfig )
		{
			if (M4_ERROR == m_poSupervisor -> RegisterCommands ( m_poCommandsConfig ) )
			{
				return M4_ERROR ;
			}
			sprintf ( szSupervisor , "SUPERVISOR %d" , lIdThread ) ;

			lIdThread ++ ;

			iRet = m_poSupervisor -> Start ( szSupervisor ) ;
		}
	}

	return 	iRet ;
  //## end ClSupervisorSS::Start%927293869.body
}

m4return_t ClSupervisorSS::Stop (m4uint32_t ai_lDeferedTime)
{
  //## begin ClSupervisorSS::Stop%928140273.body preserve=yes
	if ( ! ai_lDeferedTime)
	{
		if ( m_poSupervisor )
		{
			m_poSupervisor -> Terminate ( ) ;

			if ( m_poSupervisor -> GetStringsId () )
				cout << "Stopping " << m_poSupervisor -> GetStringsId () << "...\n" << flush ;


//			m_poSupervisor = NULL ;
		}
	}

	return M4_SUCCESS ;
  //## end ClSupervisorSS::Stop%928140273.body
}

m4bool_t ClSupervisorSS::IsReadyToClose ()
{
  //## begin ClSupervisorSS::IsReadyToClose%928140274.body preserve=yes
	if (! m_bReadyToClose )
	{
		if ( m_poSupervisor )
		{
//		m_bReadyToClose = ! m_poSupervisor -> IsRunning ( ) ;
			m_bReadyToClose = (  m_poSupervisor -> IsRunning  ( ) ) ? M4_FALSE : M4_TRUE ;

			if ( ( m_bReadyToClose ) && ( m_poSupervisor -> GetStringsId () ) )
			{

				cout << m_poSupervisor -> GetStringsId () << " Stopped\n" << flush ;
			}
		}
		else 
		{
			m_bReadyToClose = M4_TRUE ;
		}
	}

	return m_bReadyToClose;
  //## end ClSupervisorSS::IsReadyToClose%928140274.body
}

ClSSInterfaz * ClSupervisorSS::GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables)
{
  //## begin ClSupervisorSS::GenerateSubsystem%933760135.body preserve=yes

	switch ( ai_lType )
	{
	case M4_SUPERVISOR_COMMANDS :

//		iRet =m_poSupervisor -> RegisterCommands ( ai_poConfiguration ) ;
		m_poCommandsConfig = ai_poConfiguration ;

		break ;
	}

	return NULL ;
  //## end ClSupervisorSS::GenerateSubsystem%933760135.body
}

void ClSupervisorSS::Update (EVENTTYPE ai_iEvent, ClServerEvent *ai_oSubject)
{
  //## begin ClSupervisorSS::Update%944159881.body preserve=yes
	if (m_poSupervisor)
	{
		m_poSupervisor -> Update (ai_iEvent, ai_oSubject ) ;
	}
  //## end ClSupervisorSS::Update%944159881.body
}

// Additional Declarations
  //## begin ClSupervisorSS%357FFFC203E6.declarations preserve=yes
  //## end ClSupervisorSS%357FFFC203E6.declarations

// Class ClSupervisorsSS 

ClSupervisorsSS::ClSupervisorsSS (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
  //## begin ClSupervisorsSS::ClSupervisorsSS%897983838.hasinit preserve=no
  //## end ClSupervisorsSS::ClSupervisorsSS%897983838.hasinit
  //## begin ClSupervisorsSS::ClSupervisorsSS%897983838.initialization preserve=yes
  : ClSSImplementation ( ai_poConfiguration,  ai_poTables , ai_poNavigator)
  //## end ClSupervisorsSS::ClSupervisorsSS%897983838.initialization
{
  //## begin ClSupervisorsSS::ClSupervisorsSS%897983838.body preserve=yes
  //## end ClSupervisorsSS::ClSupervisorsSS%897983838.body
}


ClSupervisorsSS::~ClSupervisorsSS ()
{
  //## begin ClSupervisorsSS::~ClSupervisorsSS%897983839.body preserve=yes
  //## end ClSupervisorsSS::~ClSupervisorsSS%897983839.body
}



//## Other Operations (implementation)
m4return_t ClSupervisorsSS::GetCommandsId (M4DataStorage *ao_poDataStorage)
{
  //## begin ClSupervisorsSS::GetCommandsId%897983840.body preserve=yes
	return TOTAL_COMMANDS ;
  //## end ClSupervisorsSS::GetCommandsId%897983840.body
}

m4return_t ClSupervisorsSS::ShutDown ()
{
  //## begin ClSupervisorsSS::ShutDown%897983841.body preserve=yes
	return M4_SUCCESS ;
  //## end ClSupervisorsSS::ShutDown%897983841.body
}

m4return_t ClSupervisorsSS::Synchronize ()
{
  //## begin ClSupervisorsSS::Synchronize%897983842.body preserve=yes
	return M4_SUCCESS ;
  //## end ClSupervisorsSS::Synchronize%897983842.body
}

m4return_t ClSupervisorsSS::Initialize ()
{
  //## begin ClSupervisorsSS::Initialize%897983843.body preserve=yes
	return M4_SUCCESS ;
  //## end ClSupervisorsSS::Initialize%897983843.body
}

m4return_t ClSupervisorsSS::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClSupervisorsSS::ExecuteCommand%901709479.body preserve=yes
	return M4_SUCCESS ;
  //## end ClSupervisorsSS::ExecuteCommand%901709479.body
}

m4return_t ClSupervisorsSS::GetCommandsId (ClAdminTagList &aio_oCommandList)
{
  //## begin ClSupervisorsSS::GetCommandsId%901709480.body preserve=yes
	return M4_SUCCESS ;
  //## end ClSupervisorsSS::GetCommandsId%901709480.body
}

m4return_t ClSupervisorsSS::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList)
{
  //## begin ClSupervisorsSS::GetCommandParams%901709481.body preserve=yes
	return M4_SUCCESS ;
  //## end ClSupervisorsSS::GetCommandParams%901709481.body
}

m4return_t ClSupervisorsSS::GetCommandParams (m4char_t ai_cCommand, M4DataStorage *ao_poDataStorage)
{
  //## begin ClSupervisorsSS::GetCommandParams%910862285.body preserve=yes
	return M4_SUCCESS;
  //## end ClSupervisorsSS::GetCommandParams%910862285.body
}

ClSSInterfaz * ClSupervisorsSS::GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables)
{
  //## begin ClSupervisorsSS::GenerateSubsystem%933665014.body preserve=yes
	ClSSInterfaz * poInterfaz ;
	ClSupervisorSS * poSupervisorSS ;

	switch ( ai_lType )
	{
	case M4_SUPERVISOR_SUBSYSTEM :

		poSupervisorSS = new ClSupervisorSS ( ai_poConfiguration , ai_poTables , m_poNavigator ) ;

		poInterfaz = new ClSSInterfaz ( poSupervisorSS  );

		poSupervisorSS -> SetInterfaz ( poInterfaz ) ;


		break ;
	}
	
	return poInterfaz ;
  //## end ClSupervisorsSS::GenerateSubsystem%933665014.body
}

// Additional Declarations
  //## begin ClSupervisorsSS%3581533C035C.declarations preserve=yes
  //## end ClSupervisorsSS%3581533C035C.declarations

//## begin module%349293A5024D.epilog preserve=yes
//## end module%349293A5024D.epilog
