//## begin module%37D75ED70297.cm preserve=no
//## end module%37D75ED70297.cm

//## begin module%37D75ED70297.cp preserve=no
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
//## end module%37D75ED70297.cp

//## Module: clexecutivebasess%37D75ED70297; Package body
//## Subsystem: M4Subsystems::src%37D3A22003E4
//## Source file: F:\integration\m4subsystems\src\clexecutivebasess.cpp

//## begin module%37D75ED70297.additionalIncludes preserve=no
//## end module%37D75ED70297.additionalIncludes

//## begin module%37D75ED70297.includes preserve=yes
#include <cloblconfiguration.hpp>
#include <logsys.hpp>
#include <m4srvtrace.h>
//## end module%37D75ED70297.includes

// m4string
#include <m4string.hpp>
// clexecutivebasess
#include <clexecutivebasess.hpp>
// LauncherInterface
#include <launcher.hpp>
// queuebasedss
#include <queuebasedss.hpp>
// clservicebasess
#include <clservicebasess.hpp>
//## begin module%37D75ED70297.declarations preserve=no
//## end module%37D75ED70297.declarations

//## begin module%37D75ED70297.additionalDeclarations preserve=yes
#define M4_SRV_TRACE_COMP_ID	"SUBSYS"
//## end module%37D75ED70297.additionalDeclarations


// Class ClExecutiveBaseSS 






ClExecutiveBaseSS::ClExecutiveBaseSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator, ClServiceBaseSS *ai_poServiceSS)
  //## begin ClExecutiveBaseSS::ClExecutiveBaseSS%936860991.hasinit preserve=no
      : m_szInstanceOrder(0)
  //## end ClExecutiveBaseSS::ClExecutiveBaseSS%936860991.hasinit
  //## begin ClExecutiveBaseSS::ClExecutiveBaseSS%936860991.initialization preserve=yes
	,	ClQueueBasedSS ( ai_poConfiguratoin, ai_poTables , ai_poNavigator)
  //## end ClExecutiveBaseSS::ClExecutiveBaseSS%936860991.initialization
{
  //## begin ClExecutiveBaseSS::ClExecutiveBaseSS%936860991.body preserve=yes
	m_bUseExceptions = M4_FALSE ;
	m_poLauncher = NULL ;
	m_poServiceSS = ai_poServiceSS ;
  //## end ClExecutiveBaseSS::ClExecutiveBaseSS%936860991.body
}


ClExecutiveBaseSS::~ClExecutiveBaseSS ()
{
  //## begin ClExecutiveBaseSS::~ClExecutiveBaseSS%936860992.body preserve=yes
  //## end ClExecutiveBaseSS::~ClExecutiveBaseSS%936860992.body
}



//## Other Operations (implementation)
ClLauncherInterface * ClExecutiveBaseSS::GetLauncher ()
{
  //## begin ClExecutiveBaseSS::GetLauncher%938596862.body preserve=yes
	return m_poLauncher ;
  //## end ClExecutiveBaseSS::GetLauncher%938596862.body
}

void ClExecutiveBaseSS::SetUseExceptions (m4bool_t ai_bException)
{
  //## begin ClExecutiveBaseSS::SetUseExceptions%940264617.body preserve=yes
	m_bUseExceptions = ai_bException ;

	if ( m_poLauncher )
	{
		m_poLauncher -> SetUseException (ai_bException ) ;
	}

  //## end ClExecutiveBaseSS::SetUseExceptions%940264617.body
}

m4bool_t ClExecutiveBaseSS::GetUseExceptions ()
{
  //## begin ClExecutiveBaseSS::GetUseExceptions%940264618.body preserve=yes
	return m_bUseExceptions ;
  //## end ClExecutiveBaseSS::GetUseExceptions%940264618.body
}

m4return_t ClExecutiveBaseSS::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClExecutiveBaseSS::ExecuteCommand%940321346.body preserve=yes
	eStatus state ;


	GetState(state) ;

	if ( SS_NOT_ACCESIBLE == state )
		return M4_ERROR ;


	if ( SS_INACTIVE == state )
	{
		SETCODEF ( M4_SRV_SS_INACTIVE , WARNINGLOG , "Executor inactive, exception detected. It was not possible to execute command in executor" ) ;

		return M4_ERROR ;
	}

	if(ai_cCommand < ClSSImplementation::TOTAL_COMMANDS)
		return ClSSImplementation::ExecuteCommand (ai_cCommand, ai_oParamList, ao_oResultList);

	m4return_t			iRet;
	ClAdminCommandTag	*newParam;
	switch ( ai_cCommand )
	{
	case CAPTURE_EXCEPTIONS :
		SetUseExceptions ( M4_TRUE ) ;
		break ;
	case DONT_CAPTURE_EXCEPTIONS :
		SetUseExceptions (M4_FALSE ) ;
		break ;

	case GET_USED_MEMORY:
		if (m_poLauncher)
		{
			iRet = m_poLauncher->ExecuteCommand(ClLauncherInterface::USED_MEMORY, ai_oParamList, ao_oResultList);

			if ( iRet != M4_SUCCESS )
			{
				newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "USED_MEMORY", adminLong, "0");
				M4_ASSERT(newParam != NULL);
				if (newParam == NULL) 
					return M4_ERROR;
				ao_oResultList.push_back(newParam);
			}
		}
		break ;

	default :
		return M4_ERROR ;
	}

	return M4_SUCCESS;
  //## end ClExecutiveBaseSS::ExecuteCommand%940321346.body
}

m4return_t ClExecutiveBaseSS::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList)
{
  //## begin ClExecutiveBaseSS::GetCommandParams%940321347.body preserve=yes
	ClAdminCommandTag *newParam;
	eStatus state ;


	GetState(state) ;

	if ( SS_NOT_ACCESIBLE == state )
		return M4_ERROR ;


	m4return_t retVal=M4_ERROR;

	//-----------------------------------------------------------------------
	// check if the command is a base class command.
	//-----------------------------------------------------------------------
	if(ai_cCommand < ClSSImplementation::TOTAL_COMMANDS)
		return ClSSImplementation::GetCommandParams (ai_cCommand, aio_oParamList);
	
	//-----------------------------------------------------------------------
	// build the parameters list.
	//-----------------------------------------------------------------------
	
	switch (ai_cCommand)
	{
    case GET_LOG_OF_LAST_EXECUTION:
		// There is not parameters for this command.
		newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "LOG", adminText, "", M4_TRUE );
		M4_ASSERT(newParam != NULL);
		if (newParam == NULL) 
			return M4_ERROR;
		aio_oParamList.push_back(newParam);
		retVal=M4_SUCCESS;
		break;
	
	case GET_USED_MEMORY :
		newParam = ClAdminCommandTag::GetNewAdminCommandTag(0, "USED_MEMORY", adminLong, "", M4_TRUE);
		M4_ASSERT(newParam != NULL);
		if (newParam == NULL) 
			return M4_ERROR;
		aio_oParamList.push_back(newParam);

		break ;

	case CAPTURE_EXCEPTIONS :
	case DONT_CAPTURE_EXCEPTIONS :
		break ;

	default:
		M4_ASSERT("Request command not recognized" == NULL);
		break;		
	}

	return retVal;
  //## end ClExecutiveBaseSS::GetCommandParams%940321347.body
}

m4return_t ClExecutiveBaseSS::GetCommandsId (ClAdminTagList &aio_oCommandList)
{
  //## begin ClExecutiveBaseSS::GetCommandsId%940321348.body preserve=yes
	eStatus state ;

	GetState (state) ;

	if ( SS_NOT_ACCESIBLE == state )
		return M4_ERROR ;

	ClAdminTag *newCommand;
	if (M4_FALSE == m_bUseExceptions)
	{
		newCommand = ClAdminTag::GetNewAdminTag(CAPTURE_EXCEPTIONS, "CAPTURE_EXCEPTIONS", M4_TRUE);
		M4_ASSERT(newCommand != NULL);
		if (newCommand == NULL) return M4_FALSE;
		aio_oCommandList.push_back(newCommand);
	}
	else
	{
		newCommand = ClAdminTag::GetNewAdminTag(DONT_CAPTURE_EXCEPTIONS, "DONT_CAPTURE_EXCEPTIONS", M4_TRUE);
		M4_ASSERT(newCommand != NULL);
		if (newCommand == NULL) return M4_FALSE;
		aio_oCommandList.push_back(newCommand);
	}
	
	return  M4_SUCCESS;
  //## end ClExecutiveBaseSS::GetCommandsId%940321348.body
}

m4return_t ClExecutiveBaseSS::GetLogOfLastExecution (m4pchar_t &ao_pBuffer)
{
  //## begin ClExecutiveBaseSS::GetLogOfLastExecution%941014432.body preserve=yes
	return M4_SUCCESS ;
  //## end ClExecutiveBaseSS::GetLogOfLastExecution%941014432.body
}

void * ClExecutiveBaseSS::GetElement (m4pchar_t ai_szElement)
{
  //## begin ClExecutiveBaseSS::GetElement%942682763.body preserve=yes
	if ( ! stricmp (ai_szElement , "EXECUTIVE" ) )
		return m_poLauncher ;

	return NULL ;
  //## end ClExecutiveBaseSS::GetElement%942682763.body
}

m4return_t ClExecutiveBaseSS::Start ()
{
  //## begin ClExecutiveBaseSS::Start%955100216.body preserve=yes
	ClSSInterfaz * poLDBSubsystem ;

/*------------------------------------------------------------
Registrarse a los eventos de la LDB
---------------------------------------------------------------*/

	poLDBSubsystem = (ClSSInterfaz *) m_poNavigator -> GetSubsystemByName ( "LDB" ) ;

	if ( poLDBSubsystem )
	{
		poLDBSubsystem -> AttachObserver ( this ) ;
	}

	if ( m_poLauncher )
	{
		m_poLauncher -> SetUseException (m_bUseExceptions ) ;

		if ( M4_ERROR == m_poLauncher -> Initialize ( m_poNavigator , GetConfiguration() ) )
	
		{
			return M4_ERROR ;
		}

		m4char_t szLauncher [100];

		sprintf ( szLauncher  , "Executor for %s number %d" , m_strStringType.c_str() , m_szInstanceOrder) ;

		m_poLauncher -> Start ( szLauncher ) ;

		cout << szLauncher << " starting" << endl ;
	}

	return ClSSImplementation::Start () ;
  //## end ClExecutiveBaseSS::Start%955100216.body
}

m4return_t ClExecutiveBaseSS::Initialize ()
{
  //## begin ClExecutiveBaseSS::Initialize%957518548.body preserve=yes
	m4char_t			pszType [ OBL_PROPVALUE_MAX_LEN ] ;
	m4size_t			iLen ;

	if ( GetConfigStringValue( "TYPE", pszType , OBL_PROPVALUE_MAX_LEN , iLen) == M4_ERROR )
	{
		M4_SRV_ERROR ( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s","TYPE" << "EXECUTOR_??" );
//		SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , ERRORLOG , "Cannot find the property 'Type' in subsystem configuration" ) ;
	
		return M4_ERROR ;
	}

	m_strStringType = pszType;

	return M4_SUCCESS;
  //## end ClExecutiveBaseSS::Initialize%957518548.body
}

m4bool_t ClExecutiveBaseSS::IsReadyToClose ()
{
  //## begin ClExecutiveBaseSS::IsReadyToClose%962263775.body preserve=yes
	m4bool_t bExit = M4_TRUE ;

	if ( m_poLauncher )
		bExit = ! m_poLauncher -> IsRunning  ( )  ;

	if (! bExit )
	{
		m_poServiceQ -> Push ( (ClBaseRequest *) NULL ) ;
	}

	return bExit ;
  //## end ClExecutiveBaseSS::IsReadyToClose%962263775.body
}

m4return_t ClExecutiveBaseSS::Stop (m4uint32_t ai_lDeferedTime)
{
  //## begin ClExecutiveBaseSS::Stop%962287219.body preserve=yes
	if (  ai_lDeferedTime <= 60)
	{
		if ( m_poLauncher )
			m_poLauncher -> Terminate () ;
	}

	return M4_SUCCESS ;
  //## end ClExecutiveBaseSS::Stop%962287219.body
}

m4return_t ClExecutiveBaseSS::Reply (ClBaseRequest *ai_poRequest)
{
  //## begin ClExecutiveBaseSS::Reply%967113584.body preserve=yes
	M4_ASSERT ( "ClExecutiveBaseSS::Reply() :El SS Ejecutor no tiene la fucnionalidad implementada" ) ;

	return M4_ERROR ;
  //## end ClExecutiveBaseSS::Reply%967113584.body
}

// Additional Declarations
  //## begin ClExecutiveBaseSS%37D75E250089.declarations preserve=yes
  //## end ClExecutiveBaseSS%37D75E250089.declarations

//## begin module%37D75ED70297.epilog preserve=yes
//## end module%37D75ED70297.epilog
