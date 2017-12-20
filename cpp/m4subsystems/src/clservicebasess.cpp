//## begin module%37D75C610364.cm preserve=no
//## end module%37D75C610364.cm

//## begin module%37D75C610364.cp preserve=no
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
//## end module%37D75C610364.cp

//## Module: clservicebasess%37D75C610364; Package body
//## Subsystem: M4Subsystems::src%37D3A22003E4
//## Source file: F:\integration\m4subsystems\src\clservicebasess.cpp

//## begin module%37D75C610364.additionalIncludes preserve=no
//## end module%37D75C610364.additionalIncludes

//## begin module%37D75C610364.includes preserve=yes
#include <cllminterface.hpp>
#include <cloblconfiguration.hpp>
#include <ssnames.hpp>
#include <m4log.hpp>
#include <m4srvres.hpp>
#include <clexecutivebasess.hpp>
#include <clssinterfaz.hpp>
#include <configtree.hpp>
#include <clarraylibres.hpp>
#include <request.hpp>
#include <servicestats.hpp>
#include <clserviceinfo.hpp>
#include <subsystemids.hpp>
#include <clssinvoker.hpp>
#include <m4actionconstants.hpp>
#include <m4srvtrace.h>
//## end module%37D75C610364.includes

// clserviceinterface
#include <clserviceinterface.hpp>
// clssaction
#include <clssaction.hpp>
// queuebasedss
#include <queuebasedss.hpp>
// clservicebasess
#include <clservicebasess.hpp>
//## begin module%37D75C610364.declarations preserve=no
//## end module%37D75C610364.declarations

//## begin module%37D75C610364.additionalDeclarations preserve=yes
#define M4_SRV_SS_REGEN_MSG_ERROR					0x0010	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_SS_REGEN_FAIL_ERROR					0x0011	+ M4_SRV_SS_ERROR_BASE
#define M4_SRV_SS_REGEN_EXHAUST_ERROR				0x0012	+ M4_SRV_SS_ERROR_BASE

const m4uint32_t	MAX_M4_EXECUTOR_MAX_INSTANCES = 50 ;
const m4pchar_t		NUM_EXECUTOR_MAX_INSTANCES	= "MAX_INSTANCES" ;
const m4pchar_t		NUM_EXECUTOR_MIN_INSTANCES  = "MIN_INSTANCES" ;
const m4pchar_t		NUM_EXECUTOR_INSTANCES		= "NUM_INSTANCES" ;

#define M4_SRV_TRACE_COMP_ID	"SUBSYS"
//## end module%37D75C610364.additionalDeclarations


// Class ClServiceBaseSS 







ClServiceBaseSS::ClServiceBaseSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
  //## begin ClServiceBaseSS::ClServiceBaseSS%936860987.hasinit preserve=no
  //## end ClServiceBaseSS::ClServiceBaseSS%936860987.hasinit
  //## begin ClServiceBaseSS::ClServiceBaseSS%936860987.initialization preserve=yes
  :ClQueueBasedSS ( ai_poConfiguratoin, ai_poTables , ai_poNavigator), m_uActiveInstances(0)
  //## end ClServiceBaseSS::ClServiceBaseSS%936860987.initialization
{
  //## begin ClServiceBaseSS::ClServiceBaseSS%936860987.body preserve=yes
	m_bUseExceptions = M4_FALSE ;
  //## end ClServiceBaseSS::ClServiceBaseSS%936860987.body
}


ClServiceBaseSS::~ClServiceBaseSS ()
{
  //## begin ClServiceBaseSS::~ClServiceBaseSS%936860988.body preserve=yes
  //## end ClServiceBaseSS::~ClServiceBaseSS%936860988.body
}



//## Other Operations (implementation)
m4return_t ClServiceBaseSS::Initialize ()
{
  //## begin ClServiceBaseSS::Initialize%936860989.body preserve=yes
	ClLocalMonitorInterface * poLocalMonitor ;
	m4char_t szServiceId [OBL_PROPVALUE_MAX_LEN] ;
	m4int32_t lServiceId;
	m4return_t iRet ;
	m4double_t		uiInstances , uiMinInstances , uiMaxInstances;
	m4double_t exceptions = 0;
	m4bool_t bExceptions = M4_FALSE;
	m4char_t szExceptions [ OBL_PROPVALUE_MAX_LEN ] ;
	m4char_t szCritical [ OBL_PROPVALUE_MAX_LEN ] ;
	m4size_t iLen ;

	m_poVisitor = new ClServiceVisitor ;
	poLocalMonitor = (ClLocalMonitorInterface*)m_poNavigator -> GetLocalMonitor () ;

	iRet = GetConfigStringValue ( SERVICE_ID , szServiceId , OBL_PROPVALUE_MAX_LEN , iLen ) ;

	if ( iRet == M4_ERROR )
	{
//		M4_SRV_DEBUG ( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s","CERTIFICATE_PATH" << GetName ( ) );
		lServiceId = M4_INTERNAL_SERVICE ;
	}
	else
	{
		lServiceId = m_poTables -> GetId ( szServiceId ) ;
	}

	if ( lServiceId == M4_ERROR )
	{
		M4_SRV_ERROR ( M4_SRV_TABLES_PROP_ERROR, "No %0:s value, specified for property: '%1:s' in obl '%2:s'","ID" << szServiceId << "tables" );
		return M4_ERROR ;
	}

	m_poService = poLocalMonitor -> CreateService (lServiceId) ;

	m_poStatistics = m_poService -> GetStats ();
	m_poServiceQ = m_poService -> GetServiceQ () ;


	
	if(M4_SUCCESS == GetConfigStringValue ( "EXCEPTION_HANDLING" , szExceptions , OBL_PROPVALUE_MAX_LEN , iLen ) )
	{
		if (  strcmp ( szExceptions , DISABLED ) )
		{
			if ( strcmp ( szExceptions , ENABLED ) )
			{
				M4_SRV_WARNING( M4_SRV_CONFIG_PROP_VALUEWARNING , "Configuration property: %0:s in Subsystem: %1:s, was asigned a no valid value of %2:s. Only accepted values in range: %3:s - %4:s","EXCEPTION_HENDLING" << szServiceId << szExceptions << ENABLED << DISABLED << DISABLED );
			}
			else
			{
				bExceptions = M4_TRUE ;
			}
		}	
	}

		//Evaluar la propiedad CRITICAL SERVICE
	if ( M4_SUCCESS == GetConfigStringValue ( CRITICAL_SERVICE , szCritical , OBL_PROPVALUE_MAX_LEN , iLen ) )
	{
		if ( (strcmp ( szCritical , SS_STR_TRUE) ) && ( stricmp ( szCritical , SS_STR_FALSE ) ) )
		{
			M4_SRV_WARNING( M4_SRV_CONFIG_PROP_VALUEWARNING , "Configuration property: %0:s in Subsystem: %1:s, was asigned a no valid value of %2:s. Only accepted values in range: %3:s - %4:s",CRITICAL_SERVICE << szServiceId << szCritical<< SS_STR_TRUE << SS_STR_FALSE << SS_STR_FALSE );
		}
	}


	SetUseExceptions ( bExceptions );

	//MAX_INSTANCES
	iRet = GetConfigNumericValue (NUM_EXECUTOR_MAX_INSTANCES ,   uiMaxInstances) ;
	if ( M4_ERROR == iRet )
	{
		M4_SRV_ERROR ( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s",NUM_EXECUTOR_MAX_INSTANCES << szServiceId );
		return M4_ERROR ;
	}

	if (uiMaxInstances <1)
	{
		M4_SRV_WARNING( M4_SRV_CONFIG_PROP_VALUEWARNING , "Configuration property: %0:s in Subsystem: %1:s, was asigned a no valid value of %2:s. Only accepted values in range: %3:s - %4:s",NUM_EXECUTOR_MAX_INSTANCES << szServiceId << (m4uint32_t) uiMaxInstances << 1<< MAX_M4_EXECUTOR_MAX_INSTANCES<< 1);
		uiMaxInstances = 1 ;
	}
	
	if ( uiMaxInstances > MAX_M4_EXECUTOR_MAX_INSTANCES )
	{
		M4_SRV_WARNING( M4_SRV_CONFIG_PROP_VALUEWARNING , "Configuration property: %0:s in Subsystem: %1:s, was asigned a no valid value of %2:s. Only accepted values in range: %3:s - %4:s",NUM_EXECUTOR_MAX_INSTANCES << szServiceId << (m4uint32_t) uiMaxInstances   << 1<< MAX_M4_EXECUTOR_MAX_INSTANCES<< MAX_M4_EXECUTOR_MAX_INSTANCES );
//		return M4_ERROR ;
		uiMaxInstances = MAX_M4_EXECUTOR_MAX_INSTANCES ;
	}
	m_uMaxInstances = uiMaxInstances ;

	//MIN_INSTANCES
	iRet = GetConfigNumericValue ( NUM_EXECUTOR_MIN_INSTANCES ,   uiMinInstances) ;
	if ( M4_ERROR == iRet )
	{
		M4_SRV_ERROR ( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s",NUM_EXECUTOR_MIN_INSTANCES  << szServiceId );
		return M4_ERROR ;
	}

	if ( uiMinInstances <1)
	{
		M4_SRV_WARNING( M4_SRV_CONFIG_PROP_VALUEWARNING , "Configuration property: %0:s in Subsystem: %1:s, was asigned a no valid value of %2:s. Only accepted values in range: %3:s - %4:s",NUM_EXECUTOR_MIN_INSTANCES<< szServiceId << (m4uint32_t) uiMinInstances << 1 << (m4uint32_t) uiMaxInstances << 1 );
		uiMinInstances = 1 ;
	}

	if ( uiMinInstances > uiMaxInstances)
	{
		M4_SRV_WARNING( M4_SRV_CONFIG_PROP_VALUEWARNING , "Configuration property: %0:s in Subsystem: %1:s, was asigned a no valid value of %2:s. Only accepted values in range: %3:s - %4:s",
			NUM_EXECUTOR_MIN_INSTANCES<< szServiceId << (m4uint32_t) uiMinInstances << 1 << (m4uint32_t) uiMaxInstances << (m4uint32_t) uiMaxInstances );
		uiMinInstances = uiMaxInstances ;
//		return M4_ERROR ;
	}
	m_uMinActiveInstances = uiMinInstances ;

	//NUM_INSTANCES	
	iRet = GetConfigNumericValue ( NUM_EXECUTOR_INSTANCES ,   uiInstances) ;
	if (M4_ERROR == iRet  ) 
	{
		/* Bug 0113174
		Si no se encuentra el número de instancias se pone el mínimo y no 1 porque
		sino la siguiente condición va a dar error
		uiInstances = 1 ;
		*/
		uiInstances = uiMinInstances ;
	}

	if (uiInstances<uiMinInstances  )
	{
		M4_SRV_WARNING( M4_SRV_CONFIG_PROP_VALUEWARNING , "Configuration property: %0:s in Subsystem: %1:s, was asigned a no valid value of %2:s. Only accepted values in range: %3:s - %4:s",
			NUM_EXECUTOR_MIN_INSTANCES<< szServiceId <<  (m4uint32_t) uiInstances << (m4uint32_t) uiMinInstances  << (m4uint32_t) uiMaxInstances << (m4uint32_t) uiMinInstances);
		uiInstances = uiMinInstances  ;
	}

	if (uiInstances > uiMaxInstances ) 
	{
		M4_SRV_WARNING( M4_SRV_CONFIG_PROP_VALUEWARNING , "Configuration property: %0:s in Subsystem: %1:s, was asigned a no valid value of %2:s. Only accepted values in range: %3:s - %4:s",
			NUM_EXECUTOR_MIN_INSTANCES<< szServiceId <<  (m4uint32_t) uiInstances << (m4uint32_t) uiMinInstances  << (m4uint32_t) uiMaxInstances << (m4uint32_t) uiMaxInstances );
		uiInstances = uiMaxInstances ;
	}
//	m_uActiveInstances = uiInstances ;
	

	SetConfigNumericProperty ( "EXECUTOR_COUNT", 0);
	SetConfigNumericProperty ( "EXITED_EXECUTORS", 0);

	return M4_SUCCESS ;
		
//## end ClServiceBaseSS::Initialize%936860989.body
}

ClSSInterfaz * ClServiceBaseSS::GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables)
{
  //## begin ClServiceBaseSS::GenerateSubsystem%936860990.body preserve=yes
//	ClSSInterfaz * poInterfaz ;
	ClExecutiveBaseSS * poExecutiveSS =NULL;

	switch ( ai_lType )
	{
	case M4_EXECUTOR_TEST_SUBSYSTEM	:
	case M4_EXECUTOR_CHANNEL_SUBSYSTEM :
	case M4_EXECUTOR_ECHO_SUBSYSTEM	:
	case M4_EXECUTOR_JOB_SUBSYSTEM:
	case M4_EXECUTOR_METHOD_SUBSYSTEM:
	case M4_EXECUTOR_PERSIST_SUBSYSTEM :
	case M4_EXECUTOR_LOAD_SUBSYSTEM	:
	case M4_EXECUTOR_SESSION_SUBSYSTEM:
	case M4_EXECUTOR_JOB_GET_SUBSYSTEM	:
	case M4_EXECUTOR_JOB_EXECUTE_SUBSYSTEM	:
	case M4_EXECUTOR_PROXY_SUBSYSTEM	:
	case M4_EXECUTOR_MD_SUBSYSTEM	:
	case M4_EXECUTOR_ENV_SUBSYSTEM	:
	case M4_LAUNCHER_SUBSYSTEM :
	case M4_EXECUTOR_JOB_CANCEL_SUBSYSTEM:

		poExecutiveSS = new ClExecutiveBaseSS ( ai_poConfiguration , ai_poTables , m_poNavigator , this) ;

//		poInterfaz = new ClSSInterfaz ( poExecutiveSS   );
//		poExecutiveSS  -> SetInterfaz ( poInterfaz ) ;

		poExecutiveSS  -> SetUseExceptions ( m_bUseExceptions ) ;

		break ;
	default :
		return NULL ;
	}
	
	return poExecutiveSS -> GetInterfaz ();
		
//## end ClServiceBaseSS::GenerateSubsystem%936860990.body
}

void ClServiceBaseSS::SetService (ClServiceInterface *ai_poService)
{
  //## begin ClServiceBaseSS::SetService%936948254.body preserve=yes
	m_poService = ai_poService ;
  //## end ClServiceBaseSS::SetService%936948254.body
}

m4return_t ClServiceBaseSS::Synchronize ()
{
  //## begin ClServiceBaseSS::Synchronize%936948255.body preserve=yes
	
	if ( m_poVisitor )
	{
		( ( ClServiceVisitor * ) m_poVisitor ) ->VisitService ( (*m_poInterfaz) , (*m_poService) ) ;
	}


	ChekExecutorExceptions ()  ;

	return ClSSImplementation::Synchronize () ;
  //## end ClServiceBaseSS::Synchronize%936948255.body
}

m4return_t ClServiceBaseSS::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClServiceBaseSS::ExecuteCommand%936948256.body preserve=yes
	m4return_t iRet ;

	switch ( ai_cCommand )
	{
	case ADD_NEW_EXECUTIVE :
		iRet = AddNewExecutive () ;
		break;
	case ENABLE_EXCEPTIONS :
		iRet = CaptureExceptions( M4_TRUE );
		break ;

	case DISABLE_EXCEPTIONS  :
		iRet = CaptureExceptions ( M4_FALSE );
		break ;
	default :
		return M4_ERROR ;

	}
	
	return iRet ;
//## end ClServiceBaseSS::ExecuteCommand%936948256.body
}

m4return_t ClServiceBaseSS::Start ()
{
  //## begin ClServiceBaseSS::Start%936948257.body preserve=yes
	m4double_t bExceptions = 0 ;

	ClSSInvoker * poInvoker ;


	poInvoker = ClSSInvoker::GetInstance () ;

	poInvoker -> RegisterAction ( GetName () , EXECUTE_REQUEST_ACTION, this ) ;

	if ( M4_ERROR == GetConfigNumericValue ( "EXCEPTIONS" , bExceptions ) )
	{
		bExceptions = 1 ;
	}

	return ClSSImplementation::Start () ;
//## end ClServiceBaseSS::Start%936948257.body
}

m4return_t ClServiceBaseSS::Stop (m4uint32_t ai_lDeferedTime)
{
  //## begin ClServiceBaseSS::Stop%936948258.body preserve=yes
	SetState ( SS_SHUTING_DOWN , SS_STR_SHUTTING_DOWN ) ;

	return ClSSImplementation::Stop (ai_lDeferedTime) ;
  //## end ClServiceBaseSS::Stop%936948258.body
}

m4bool_t ClServiceBaseSS::IsReadyToClose ()
{
  //## begin ClServiceBaseSS::IsReadyToClose%936948259.body preserve=yes
	return ClSSImplementation::IsReadyToClose () ;
  //## end ClServiceBaseSS::IsReadyToClose%936948259.body
}

m4return_t ClServiceBaseSS::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList)
{
  //## begin ClServiceBaseSS::GetCommandParams%936948260.body preserve=yes
	return M4_SUCCESS ;
  //## end ClServiceBaseSS::GetCommandParams%936948260.body
}

m4return_t ClServiceBaseSS::GetCommandsId (ClAdminTagList &aio_oCommandList)
{
  //## begin ClServiceBaseSS::GetCommandsId%936948261.body preserve=yes
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	ClAdminTag *newCommand;
//    typedef enum { SET_QUEUE_SIZE = ClSSStatsContainer::TOTAL_COMMANDS , EMPTY_QUEUE , SUSPEND_RESUME , ADD_NEW_EXECUTIVE , SHUT_DOWN_EXECUTIVE , ENABLE_EXCEPTIONS , CAPTURE_EXCEPTIONS , TOTAL_COMMANDS } eServiceCommands;

	newCommand = ClAdminTag::GetNewAdminTag(ADD_NEW_EXECUTIVE, "ADD_NEW_EXECUTOR", M4_FALSE);
	M4_ASSERT(newCommand != NULL);
	if (newCommand == NULL) return M4_FALSE;
	aio_oCommandList.push_back(newCommand);

	if ( GetUseExceptions () )
	{
		newCommand = ClAdminTag::GetNewAdminTag(DISABLE_EXCEPTIONS, "TURN_OFF_EXCEPTIONS", M4_FALSE);
	}
	else
	{
		newCommand = ClAdminTag::GetNewAdminTag(ENABLE_EXCEPTIONS , "TURN_ON_EXCEPTIONS", M4_FALSE);
	}

	M4_ASSERT(newCommand != NULL);
	if (newCommand == NULL) return M4_FALSE;
	aio_oCommandList.push_back(newCommand);

	return  M4_SUCCESS;
//## end ClServiceBaseSS::GetCommandsId%936948261.body
}

m4return_t ClServiceBaseSS::AddNewExecutive ()
{
  //## begin ClServiceBaseSS::AddNewExecutive%938596860.body preserve=yes
	ClConfiguration * poConfiguration =NULL , * poReplyConfiguration =NULL;
	ClSSInterfaz * poSubsystem , * poNewSubsystem =NULL ;
	m4uint32_t iNumExecutors  ;
	M4DataStorage * poDataStorage = NULL ;
	INTERFACESMAP::iterator Iterator ;
	eStatus state ;


	GetState(state) ;

	if (( SS_NOT_ACCESIBLE == state ) || (SS_INACTIVE == state ) || ( SS_SHUTING_DOWN == state ) )
	{
		return M4_ERROR ;
	}


	m4char_t	szServiceName [MAXNAMEPROP] ;
	size_t	iLen ;
	if ( M4_ERROR == GetConfigStringValue ( "TYPE" , szServiceName , MAXNAMEPROP , iLen ) )
	{
		strcpy(szServiceName, "Unknown Service!");
	}

	m_oSyncBellowLevel.EnterWriter () ;

	iNumExecutors = m_oBellowLevel.size () ;

	/* Bug 0108821
	Sólo hay que contar los hijos que realmente sean ejecutores
	Si no es ejecutor se descuenta del total
	*/
	for( Iterator = m_oBellowLevel.begin() ; Iterator != m_oBellowLevel.end() ; Iterator++ )
	{
		poSubsystem = (*Iterator).second ;

		if( poSubsystem != NULL && poSubsystem->m_poImplementation->IsExecutor() == M4_FALSE )
		{
			iNumExecutors-- ;
		}
	}

	if ( iNumExecutors >= m_uMaxInstances )
	{
		M4_SRV_ERROR (  M4_SRV_COMMAND_MAX_INSTANCES , "Max number of instances of %0:s in Subsystem %1:s reached. Executing command %2:s" , "EXECUTOR" << szServiceName << "ADD_NEW_EXECUTOR" ) ;
		m_oSyncBellowLevel.LeaveWriter () ;
		return M4_ERROR ;
	}

	Iterator = m_oBellowLevel.begin() ;

	if (Iterator == m_oBellowLevel.end()) 
	{
		m_oSyncBellowLevel.LeaveWriter () ;
		return M4_ERROR ;
	}

	if ( ! ( poSubsystem = (*Iterator).second ) )
	{
		m_oSyncBellowLevel.LeaveWriter () ;
		return M4_ERROR ;
	}

	if ( ! ( poConfiguration = poSubsystem -> GetConfiguration ( ) ) )
	{
		m_oSyncBellowLevel.LeaveWriter () ;
		return M4_ERROR ;
	}

	poReplyConfiguration = new ClM4ConfigTree ( poConfiguration ) ;

	if ( ! ( poNewSubsystem = GenerateSubsystem ( poSubsystem -> GetType ( ) , poReplyConfiguration , m_poTables) ) )
	{
		m_oSyncBellowLevel.LeaveWriter () ;
		return M4_ERROR ;
	}

	poNewSubsystem -> SetName ( poSubsystem ->GetName ( ) ) ;

	poNewSubsystem -> SetConfiguration ( poReplyConfiguration ) ;

	//primero inicializo el ss (crear un ejecutor) y luego lo añado a la lista
	poNewSubsystem -> GetSubsystem() -> Initialize ();
//OSCAR 1-12-99

		m_oSyncBellowLevel.LeaveWriter () ;

	//La función AddSubsystem hace lo mismo de lo que se hacía anteriormente,
	//pero la sincronización cambia al estar sincronizados los dos métodos
	if (M4_ERROR == AddSubsystem ( (ClSSInterfaz*)poNewSubsystem ) )
	{
		return M4_ERROR ;
	}
/*
	lSubsystemId = ClIdGenerator::GetNewId () ;

	m_oBellowLevel.insert ( INTERFACESMAP::value_type ( lSubsystemId , poNewSubsystem  ) ) ;
*/
//END OSCAR 1-12-99

	poNewSubsystem -> GetSubsystem() -> Start();


	
	return M4_SUCCESS ;
//## end ClServiceBaseSS::AddNewExecutive%938596860.body
}

m4return_t ClServiceBaseSS::ChekExecutorExceptions ()
{
  //## begin ClServiceBaseSS::ChekExecutorExceptions%938596861.body preserve=yes
	m4size_t				iInactive = 0;
	INTERFACESMAP::iterator Iterator ;
	m4size_t				iCount;
	m4return_t				ret;
	ClSSInterfaz			*poInterfaz ;
//	ClExecutiveBaseSS		*poLauncherSS;
	ClLauncherInterface		*poLauncher;
	eStatus state ;
	m4size_t				iSize = 0;



	GetState (state) ;

	if ( ( SS_NOT_ACCESIBLE == state )||( SS_SHUTING_DOWN == state ) )
		return M4_ERROR ;

	m_oSyncBellowLevel.EnterWriter () ;

/*  iSize es el número de ejecutores, pero puede que el servicio tenga hijos que 
	que no sean ejecutores, como es el caso del proxy

	m4size_t				iSize = m_oBellowLevel.size();
*/


	for ( Iterator = m_oBellowLevel.begin() ; Iterator != m_oBellowLevel.end() ; Iterator ++ )
	{
		poInterfaz = (*Iterator).second ;
//		poLauncherSS = (ClExecutiveBaseSS *) (*Iterator).second ;
		if ( poLauncher = (ClLauncherInterface *)poInterfaz ->GetElement( "EXECUTIVE" ) )
		{
			iSize ++ ;
			if (poLauncher->IsExit())
			{
				iInactive++;

				poInterfaz -> SetState ( (ClSSInterfaz::eStatus) SS_INACTIVE , SS_STR_INACTIVE ) ;
			}
		}
	}

	SetConfigNumericProperty ( "EXECUTOR_COUNT", iSize);

	m_oSyncBellowLevel.LeaveWriter () ;

//	Check Active value
	if (m_uActiveInstances < iInactive)
	{
		SetConfigNumericProperty ( "EXITED_EXECUTORS", iInactive);

		m4char_t	szServiceName [MAXNAMEPROP] ;
		size_t	iLen ;
		if ( M4_ERROR == GetConfigStringValue ( "TYPE" , szServiceName , MAXNAMEPROP , iLen ) )
		{
			strcpy(szServiceName, "Unknown Service!");
		}

		m4int_t		iAborted = iInactive - m_uActiveInstances;
//	Check Max value
		if (iSize < m_uMaxInstances)
		{

			m4int_t		iSpare = m_uMaxInstances - iSize;
			for (iCount = 0; (iCount < iAborted) && (iCount < iSpare); iCount++)
			{
				SETCODEF ( M4_SRV_SS_REGEN_MSG_ERROR , WARNINGLOG , szServiceName) ;
				ret = AddNewExecutive();
				if (M4_ERROR == ret)
				{
					SETCODEF ( M4_SRV_SS_REGEN_FAIL_ERROR , WARNINGLOG , szServiceName) ;
					m_uMaxInstances--;		//	No more retries
				}
				else
				{
					iSize++;
				}
			}
		}

		m_uActiveInstances = iInactive;
//	Check Min value
		if ((iSize - m_uActiveInstances) < m_uMinActiveInstances)
		{

			SetState ( SS_INACTIVE , SS_STR_INACTIVE );
			((ClServiceInterface*)m_poService) ->Disable();

			SETCODEF ( M4_SRV_SS_REGEN_EXHAUST_ERROR, WARNINGLOG , szServiceName) ;
			EmptyQueue();
			Notify( SERVICE_SHUTDOWN );

		}

	}

	return M4_SUCCESS; 
//## end ClServiceBaseSS::ChekExecutorExceptions%938596861.body
}

m4return_t ClServiceBaseSS::EmptyQueue ()
{
  //## begin ClServiceBaseSS::EmptyQueue%938596863.body preserve=yes
	static ClMutex oMutex (M4_TRUE) ;
	if (NULL == m_poService)
		return M4_ERROR ;
	
	ClServiceQ	*poServiceQ;

	poServiceQ = (ClServiceQ *) m_poServiceQ;

	oMutex.Lock() ;
	poServiceQ -> Lock();

	ClServiceItem		poItem;
	ClRequest			*oltpRequest;

	while (0 < poServiceQ ->Size())
	{
		poServiceQ->Pop(poItem,M4_FALSE,0);

		oltpRequest = (ClRequest*)poItem;

		oltpRequest -> Cancel ( ) ;

		Reply ( poItem ) ;
	}

	oMutex.Unlock() ;
	return M4_SUCCESS ;
//## end ClServiceBaseSS::EmptyQueue%938596863.body
}

ClServiceInterface * ClServiceBaseSS::GetService ()
{
  //## begin ClServiceBaseSS::GetService%939282715.body preserve=yes
	return m_poService ;
  //## end ClServiceBaseSS::GetService%939282715.body
}

m4return_t ClServiceBaseSS::CaptureExceptions (m4bool_t ai_bCapture)
{
  //## begin ClServiceBaseSS::CaptureExceptions%939806808.body preserve=yes
	ClSSInterfaz * poSubsystem =NULL ;
	INTERFACESMAP::iterator Iterator ;
//	ClLauncherInterface * poLauncher ;
	eStatus state ;
	ClAdminTagList admintagIn , admintagOut;
	m4char_t szId [10] ;

	GetState(state) ;

	if ( SS_NOT_ACCESIBLE == state )
		return M4_ERROR ;



	m_oSyncBellowLevel.EnterWriter () ;

	SetUseExceptions ( ai_bCapture ) ;

	for ( Iterator = m_oBellowLevel.begin() ; Iterator != m_oBellowLevel.end() ; Iterator ++)
	{
		if (  poSubsystem = (*Iterator).second ) 
		{
			sprintf ( szId , "%ld" , (m4uint32_t) poSubsystem -> GetId () ) ;
			if ( ai_bCapture )
				poSubsystem  -> ExecuteCommand ( szId , ClExecutiveBaseSS::CAPTURE_EXCEPTIONS , admintagIn , admintagOut) ;
				else
					poSubsystem  -> ExecuteCommand ( szId , ClExecutiveBaseSS::DONT_CAPTURE_EXCEPTIONS , admintagIn , admintagOut ) ;
				 /*SetUseExceptions ( ai_bCapture ) ;

			if ( poLauncher = (ClLauncherInterface*)poSubsystem -> GetElement () )
			{
				poLauncher -> SetUseException ( ai_bCapture ) ;
			}
			*/
		}
	}

	m_oSyncBellowLevel.LeaveWriter () ;

	return M4_SUCCESS ;
//## end ClServiceBaseSS::CaptureExceptions%939806808.body
}

void ClServiceBaseSS::SetUseExceptions (m4bool_t ai_bException)
{
  //## begin ClServiceBaseSS::SetUseExceptions%940264615.body preserve=yes
	m_bUseExceptions = ai_bException ;
	if ( ai_bException )
	{
		SetConfigStringProperty ( "EXCEPTION_HANDLING" , ENABLED ) ;
	}
	else
	{
		SetConfigStringProperty ( "EXCEPTION_HANDLING" , DISABLED ) ;
	}
  //## end ClServiceBaseSS::SetUseExceptions%940264615.body
}

m4bool_t ClServiceBaseSS::GetUseExceptions ()
{
  //## begin ClServiceBaseSS::GetUseExceptions%940264616.body preserve=yes
	return m_bUseExceptions ;
  //## end ClServiceBaseSS::GetUseExceptions%940264616.body
}

m4return_t ClServiceBaseSS::ProcessItem (ClBaseRequest *ai_oItem)
{
  //## begin ClServiceBaseSS::ProcessItem%955100221.body preserve=yes
	ClRequestStatistics * pReqStats = (ClRequestStatistics * ) ai_oItem -> GetStats () ;

	if ( pReqStats )
		pReqStats->Sett ( ClRequestStatistics::InServiceQ);

	return m_poServiceQ -> Push ( ai_oItem ) ;
  //## end ClServiceBaseSS::ProcessItem%955100221.body
}

m4return_t ClServiceBaseSS::CreateAction (m4pchar_t ai_szActionType, ClBaseAction *&ao_poAction)
{
  //## begin ClServiceBaseSS::CreateAction%955440315.body preserve=yes
	if ( ! stricmp ( ai_szActionType , GetName () ) )
	{
		ao_poAction = new ClServiceBaseSSAction ( this ) ;
		return M4_SUCCESS ;
	}

	return M4_ERROR ;
  //## end ClServiceBaseSS::CreateAction%955440315.body
}

m4return_t ClServiceBaseSS::ProcessAction (ClActionIdentifier ai_oActionIdentifier, ClParamList *ai_poParamList, ClBaseAction *&ao_poAction)
{
  //## begin ClServiceBaseSS::ProcessAction%957943669.body preserve=yes

	switch ( ai_oActionIdentifier )
	{
	case EXECUTE_REQUEST_ACTION :
	
		ao_poAction = new ClServiceBaseSSAction (this) ;
		ao_poAction -> SetParams ( ai_poParamList ) ;
	}

	return ao_poAction -> Execute () ;
  //## end ClServiceBaseSS::ProcessAction%957943669.body
}

m4return_t ClServiceBaseSS::EstimateLoad ()
{
  //## begin ClServiceBaseSS::EstimateLoad%958983595.body preserve=yes
	return M4_SUCCESS ;
  //## end ClServiceBaseSS::EstimateLoad%958983595.body
}

m4return_t ClServiceBaseSS::Reply (ClBaseRequest *ai_poRequest)
{
  //## begin ClServiceBaseSS::Reply%967113585.body preserve=yes
		ClSSInvoker * poInvoker ;
		ClParamList * poParamList ;
		ClRequestStatistics * poStats ;
		m4return_t iret ;


	if ( ! ai_poRequest )
		return M4_ERROR ;

		
	poInvoker = ClSSInvoker::GetInstance () ;

	poInvoker -> GetParamList ( poParamList ) ;
	poParamList -> SetPointer ( "REQUEST" , ai_poRequest ) ;

    poStats = (ClRequestStatistics *)ai_poRequest->GetStats();
	if (NULL != poStats)
    {
		poStats->Sett(ClRequestStatistics::InSSQueueOut);
    }

	iret = poInvoker -> ExecuteAction ( SEND_REQUEST_ACTION , poParamList) ;
//	poInvoker -> SetParamList ( *poParamList ) ;

	return iret ;
//## end ClServiceBaseSS::Reply%967113585.body
}

m4return_t ClServiceBaseSS::Init (ClConfiguration *ai_poConfiguration)
{
  //## begin ClServiceBaseSS::Init%993464192.body preserve=yes
	ClSSInterfaz * poSubsystem ;
	UINT * aiHandles = new UINT [1000] ;
	m4size_t iNumBlocks = 0 ;
	m4uint16_t  iBlocks , iInst ;
	m4double_t iNumInstances ;
	m4return_t iRet ;
	ClConfiguration * poConfiguration ;
	m4objid_t  lId ;
	m4int16_t lType ;
	m4char_t szType [OBL_PROPVALUE_MAX_LEN] ;
	m4pchar_t szNumInstances  = "NUM_INSTANCES_";
	m4char_t szNumInsPropName  [OBL_PROPVALUE_MAX_LEN*2] ;
	m4char_t szNameId [OBL_PROPVALUE_MAX_LEN] ;
	size_t iLen ;
	m4double_t iMinNumInstancesBlock;

	iRet = GetConfiguration() -> GetType ( szType , OBL_PROPVALUE_MAX_LEN ) ;

	if ( iRet == M4_ERROR )
	{
		M4_SRV_ERROR ( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s","TYPE" << "??" );
		return M4_ERROR ;
	}

	iRet = GetConfiguration() -> GetStringValue ( "" , "STRINGID" , szNameId , OBL_PROPVALUE_MAX_LEN, iLen )  ;

	if (iRet == M4_ERROR )
	{
		iRet = GetConfiguration() -> SetStringValue ( "" , "STRINGID", szType ) ;
	}


	if ( ! GetName())
		SetName ( szType ) ;

	iRet = GetConfiguration() -> GetAllMatchingBlocks ( "*.*" , aiHandles , 1000, iNumBlocks ) ;

	if ( iRet == M4_ERROR ) 
	{
		SETCODEF ( M4_SRV_STARTUPOBL_ERROR, ERRORLOG , "Cannot find blocks into the subsystem %s" , szType ) ;
		return M4_ERROR ;
	}

	for ( iBlocks = 0 ; iBlocks < iNumBlocks ; iBlocks ++ ) 
	{
		if ( ! (poConfiguration = GetConfiguration()  -> GetBloqConfiguration ( aiHandles [ iBlocks ] ) ) )
		{
			SETCODEF ( M4_SRV_STARTUPOBL_ERROR, ERRORLOG , "Cannot generate a configuration object from block handle %l into subsystem %s" , aiHandles [ iBlocks ] , szType ) ;
			return M4_ERROR ;
		}

		//Asigna el bloque de la obl que se trata
		poConfiguration -> m_hBlock = aiHandles [ iBlocks ] ;

		if ( poConfiguration -> GetType ( szType , OBL_PROPVALUE_MAX_LEN) == M4_ERROR )
		{
			M4_SRV_ERROR ( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s","TYPE" << "??" );
			return M4_ERROR ;
		}

		if ( M4_ERROR == (lType = m_poTables -> GetId ( szType ) ) )
		{
			SETCODEF ( M4_SRV_SS_UNKNOWN , ERRORLOG , "Subsystem %s, not defined in 'tables.obl'" , szType ) ;
			return M4_ERROR ;
		}

		//----------------------------------
		//Bug 0094830
		//Obtenemos el minimo numero de instancias para el bloque que se está anilizando.
		//Si hay error significa que no está, por lo tanto se establece al del miembro.
		if ( poConfiguration -> GetNumericValue ( "" , "MIN_INSTANCES"  , iMinNumInstancesBlock ) == M4_SUCCESS )
		{
			if ( iMinNumInstancesBlock <= 0)
			{
				SETCODEF ( M4_SRV_STARTUPOBL_ERROR, ERRORLOG , "MIN_INSTANCES property of subsystem %s is asigned to 0, and it is and illegal value", szType ) ;
				return M4_ERROR ;
			}
			//Si estamos aqui es pq la propiedad existe en el fichero .obl y establecemos el valor minimo.
			m_uMinActiveInstances = iMinNumInstancesBlock;
		}
		//----------------------------------
		
		sprintf ( szNumInsPropName , "%s%s" , szNumInstances , szType ) ;
		//La propiedad NUM_INSTANCES la miramos ahora en el subsystema padre, para saber el nº de instancias de un SS
		if ( GetConfiguration() -> GetNumericValue ( "" , szNumInsPropName  , iNumInstances ) == M4_ERROR )
		{
			iNumInstances = 1 ;
		}

		if ( iNumInstances == 0)
		{
			SETCODEF ( M4_SRV_STARTUPOBL_ERROR, ERRORLOG , "NUM_INSTANCES property of subsystem %s is asigned to 0, and it is and illegal value", szType ) ;
			return M4_ERROR ;
		}

		if ( m_uMaxInstances < iNumInstances )
		{
			M4_SRV_WARNING( M4_SRV_CONFIG_PROP_VALUEWARNING , "Configuration property: %0:s in Subsystem: %1:s, was asigned a no valid value of %2:s. Only accepted values in range: %3:d - %4:d. Value set to %5:d",szNumInsPropName << GetName () << (m4uint32_t) iNumInstances << (m4uint32_t) m_uMinActiveInstances <<  (m4uint32_t) m_uMaxInstances  <<  (m4uint32_t) m_uMaxInstances );
			iNumInstances = m_uMaxInstances ;
		}

		if ( m_uMinActiveInstances  > iNumInstances )
		{
			M4_SRV_WARNING( M4_SRV_CONFIG_PROP_VALUEWARNING , "Configuration property: %0:s in Subsystem: %1:s, was asigned a no valid value of %2:s. Only accepted values in range: %3:d - %4:d. Value set to %5:d",szNumInsPropName << GetName () << (m4uint32_t) iNumInstances << (m4uint32_t) m_uMinActiveInstances <<  (m4uint32_t) m_uMaxInstances  <<  (m4uint32_t) m_uMinActiveInstances );
			iNumInstances = m_uMinActiveInstances ;
		}


		for ( iInst = 0 ; iInst < iNumInstances ; iInst ++ )
		{
			if ( iInst > 0 )
			{
//Aquí se debe de implementar la creación de un objeto con copia
				poConfiguration = new ClM4ConfigTree ( poConfiguration ) ;
			}

			if (! (  poSubsystem = GenerateSubsystem ( lType , poConfiguration , m_poTables ) ) )
			{
				poSubsystem = m_poSSFactory -> GenerateSubsystem ( lType , poConfiguration , m_poTables )  ;
			}

			if ( poSubsystem )
			{
//				{
					lId = ClIdGenerator::GetNewId ( ) ;

					m_oBellowLevel.insert ( INTERFACESMAP::value_type ( lId , poSubsystem ) ) ;

					//Asignar el id al eobjeto clsubsystem y al derivado de clssstatscontainer
					poSubsystem -> SetId ( lId , GetPath ( ) ) ;

					if ( M4_ERROR == poSubsystem -> Initialize () )
					{
						return M4_ERROR ;
					}

					if ( poSubsystem -> Init ( poConfiguration ) == M4_ERROR ) 
					{
						return M4_ERROR ;
					}
//				}
			}
		}
	}

	if  ( iRet == M4_ERROR )
	{
		return M4_ERROR ;
	}

	delete [] aiHandles ;



	return M4_SUCCESS ;
	
//## end ClServiceBaseSS::Init%993464192.body
}

m4return_t ClServiceBaseSS::RegisterSnmpCommands ()
{
  //## begin ClServiceBaseSS::RegisterSnmpCommands%1066298189.body preserve=yes
	/*
	m4return_t iRet ;
	ClAdminTagList oParamList ;
	m4size_t iLen ;
	m4char_t szType [100] ;
	
    ClSNMPAdminInterface * poSnmpAdmin = ClSNMPAdminInterface::GetInstance () ;

	if ( ! poSnmpAdmin )
		return M4_ERROR ;


  if ( M4_ERROR == GetConfigStringValue ("TYPE", szType , 100, iLen) )
		return M4_ERROR ;


	iRet = poSnmpAdmin ->RegisterCommand ("ADD_NEW_EXECUTOR", ADD_NEW_EXECUTIVE, oParamList, this , szType , m_strSnmpOidx) ;
	iRet = poSnmpAdmin ->RegisterCommand ("TURN_OFF_EXCEPTIONS", DISABLE_EXCEPTIONS, oParamList, this, szType , m_strSnmpOidx) ;
	iRet = poSnmpAdmin ->RegisterCommand ("TURN_ON_EXCEPTIONS", ENABLE_EXCEPTIONS, oParamList, this, szType , m_strSnmpOidx) ;

	*/
    return M4_SUCCESS ;
  //## end ClServiceBaseSS::RegisterSnmpCommands%1066298189.body
}

// Additional Declarations
  //## begin ClServiceBaseSS%37D75C11000B.declarations preserve=yes
  //## end ClServiceBaseSS%37D75C11000B.declarations

// Class ClServiceBaseSSAction 

ClServiceBaseSSAction::ClServiceBaseSSAction (ClSSImplementation *ai_poExecutive)
  //## begin ClServiceBaseSSAction::ClServiceBaseSSAction%955440317.hasinit preserve=no
  //## end ClServiceBaseSSAction::ClServiceBaseSSAction%955440317.hasinit
  //## begin ClServiceBaseSSAction::ClServiceBaseSSAction%955440317.initialization preserve=yes
  :ClSSAction ( ai_poExecutive )
  //## end ClServiceBaseSSAction::ClServiceBaseSSAction%955440317.initialization
{
  //## begin ClServiceBaseSSAction::ClServiceBaseSSAction%955440317.body preserve=yes
  //## end ClServiceBaseSSAction::ClServiceBaseSSAction%955440317.body
}



//## Other Operations (implementation)
m4return_t ClServiceBaseSSAction::Execute ()
{
  //## begin ClServiceBaseSSAction::Execute%955440318.body preserve=yes
	ClBaseRequest * poRequest ;
	void * poItem ;

	if (M4_ERROR == m_pParamList -> GetPointer ( "REQUEST" , poItem ) )
	{
		return M4_ERROR ;
	}

	if (! ( poRequest = (ClBaseRequest*) poItem  ) )
	{
		return M4_ERROR ;
	}

	return ((ClServiceBaseSS*)m_poExecutive ) -> ProcessItem ( poRequest ) ;
//## end ClServiceBaseSSAction::Execute%955440318.body
}

// Additional Declarations
  //## begin ClServiceBaseSSAction%38F335A30196.declarations preserve=yes
  //## end ClServiceBaseSSAction%38F335A30196.declarations

//## begin module%37D75C610364.epilog preserve=yes
//## end module%37D75C610364.epilog
