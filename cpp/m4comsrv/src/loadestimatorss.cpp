//## begin module%37E661C801B1.cm preserve=no
//## end module%37E661C801B1.cm

//## begin module%37E661C801B1.cp preserve=no
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
//## end module%37E661C801B1.cp

//## Module: loadestimatorss%37E661C801B1; Package body
//## Subsystem: version::src%379ECBD7029A
//## Source file: D:\Work\v600\m4comsrv\src\loadestimatorss.cpp

//## begin module%37E661C801B1.additionalIncludes preserve=no
//## end module%37E661C801B1.additionalIncludes

//## begin module%37E661C801B1.includes preserve=yes
#include <m4servenv.hpp>
#include <cloblconfiguration.hpp>
#include <clsubsystem.hpp>
#include <servenv.hpp>
#include <m4log.hpp>
#include <m4srvres.hpp>
#include <controlpipesmkr.hpp>
#include <subsystemids.hpp>
#include <xmlstreamgenerator.hpp>
#include <clssinvoker.hpp>
#include <m4actionconstants.hpp>
#include <commactionsname.h>
//## end module%37E661C801B1.includes

// ClConfiguration
#include <clconfiguration.hpp>
// loadrule
#include <loadrule.hpp>
// loadestimatorss
#include <loadestimatorss.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>

class ClXMLMaker;

//## begin module%37E661C801B1.declarations preserve=no
//## end module%37E661C801B1.declarations

//## begin module%37E661C801B1.additionalDeclarations preserve=yes
const m4pchar_t  SZ_STATE = "STATE" ;
const m4pchar_t  SZ_LOAD = "LOAD" ;

const m4pchar_t  SZ_STARTING = "STARTING" ;
const m4pchar_t  SZ_READY = "READY" ;
const m4pchar_t  SZ_SHUTING_DOWN = "SHUTTING DOWN" ;
const m4pchar_t  SZ_INACTIVE = "INACTIVE" ;
//## end module%37E661C801B1.additionalDeclarations


// Class ClLoadEstimatorSS 














ClLoadEstimatorSS::ClLoadEstimatorSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
  //## begin ClLoadEstimatorSS::ClLoadEstimatorSS%937841128.hasinit preserve=no
  //## end ClLoadEstimatorSS::ClLoadEstimatorSS%937841128.hasinit
  //## begin ClLoadEstimatorSS::ClLoadEstimatorSS%937841128.initialization preserve=yes
  :ClSSImplementation ( ai_poConfiguratoin, ai_poTables, ai_poNavigator )
  //## end ClLoadEstimatorSS::ClLoadEstimatorSS%937841128.initialization
{
  //## begin ClLoadEstimatorSS::ClLoadEstimatorSS%937841128.body preserve=yes
	m_bConnected = M4_FALSE ;
	m_iDspConnectionId = 0;
	SetState ( SS_ACTIVE , SS_STR_ACTIVE ) ;
  //## end ClLoadEstimatorSS::ClLoadEstimatorSS%937841128.body
}


ClLoadEstimatorSS::~ClLoadEstimatorSS ()
{
  //## begin ClLoadEstimatorSS::~ClLoadEstimatorSS%937841129.body preserve=yes
  //## end ClLoadEstimatorSS::~ClLoadEstimatorSS%937841129.body
}



//## Other Operations (implementation)
m4return_t ClLoadEstimatorSS::Initialize ()
{
  //## begin ClLoadEstimatorSS::Initialize%937841130.body preserve=yes
	m4char_t szConfigName [OBL_PROPERTY_MAX_LEN] ="";
	m4char_t szBuffer [OBL_PROPERTY_MAX_LEN] ="";
	m4return_t iRet ;
	m4double_t d ;

	 m_szState = SZ_STARTING ;



	if ( M4_SUCCESS == ( iRet = GetEnviron ( "SERVER" , SERVER_NAME , szBuffer , OBL_PROPERTY_MAX_LEN ) ))
	{
		m_szConfigName = szBuffer ;
	}

	m_iSleepTime = 0 ;
	if ( M4_SUCCESS  == (iRet = GetEnviron ( "SERVER" , M4DISPATCHER_CONTROL_RETRY_TIME, szBuffer , OBL_PROPERTY_MAX_LEN ) ) )
	{
		sscanf ( szBuffer , "%d" , &m_iSleepTime ) ;
	}

	if ( M4_SUCCESS  == (iRet = GetEnviron ( "SERVER" , M4DISPATCHER_HOST, szBuffer , OBL_PROPERTY_MAX_LEN ) ) )
	{
		m_szDispatcherHost = szBuffer ;
	}
	else
	{
		SetState ( SS_INACTIVE , SS_STR_INACTIVE ) ;
	}

	m_lDispatcherPort = 0 ;

	if ( M4_SUCCESS == GetEnvironDouble ( "SERVER" , M4DISPATCHER_ADMIN_PORT, d ) )
	{
		m_lDispatcherPort = d ;
	}
	else
	{
		SetState ( SS_INACTIVE , SS_STR_INACTIVE ) ;
	}

//	NotifyDispatcher (M4_SERVER_STOPPING_RETRY_WAIT ) ;

	ClSystem * poSystem = ClSystem::Instance () ;

	poSystem ->AttachObserver ( this ) ;

	return M4_SUCCESS ;
  //## end ClLoadEstimatorSS::Initialize%937841130.body
}

m4return_t ClLoadEstimatorSS::Start ()
{
  //## begin ClLoadEstimatorSS::Start%937841131.body preserve=yes
	m4char_t szHost [100]  = "";
	m4char_t szPort [OBL_PROPVALUE_MAX_LEN] = "";

	// fix bug 0088880
	if ( M4_ERROR == GetEnviron ( "COMMUNICATION_SERVER" , LOCAL_HOST_NAME , szHost , OBL_PROPVALUE_MAX_LEN) ||
		'\0' == *szHost )
	{
		gethostname ( szHost , 100 ) ;
	}
	
	if (M4_ERROR ==  GetEnviron ( "COMMUNICATION_SERVER" , BASE_PORT , szPort , OBL_PROPVALUE_MAX_LEN) )
	{
		return M4_ERROR ;
	}
	
	sscanf ( szPort , "%d" , &m_iBasePort ) ;
	
	m_szLocalHost = szHost ;

	if ( M4_ERROR == InitializeEstimationRules () )
		return M4_ERROR ;



	return M4_SUCCESS ;
  //## end ClLoadEstimatorSS::Start%937841131.body
}

m4return_t ClLoadEstimatorSS::Stop (m4uint32_t ai_lDeferedTime)
{
  //## begin ClLoadEstimatorSS::Stop%937841132.body preserve=yes
	static m4bool_t stop = M4_FALSE ;

	if( ! stop )
	{
		stop = M4_TRUE ;

//		m_oLoadEstimator.Stop ( M4_SERVER_STOPPING_RETRY ) ;
	}

	if (! ai_lDeferedTime )
	{
//			m_oLoadEstimator.Stop ( M4_SERVER_STOPPED) ;
	}

	return M4_SUCCESS ;
  //## end ClLoadEstimatorSS::Stop%937841132.body
}

void ClLoadEstimatorSS::Update (EVENTTYPE ai_iEvent, ClServerEvent *ai_oSubject)
{
  //## begin ClLoadEstimatorSS::Update%938714043.body preserve=yes
	eStatus eState ;

	
		switch ( ai_iEvent )
		{
		case SERVER_STOPPED :
			 m_szState =SZ_INACTIVE ;
	 		 EstimateLoad () ;
			Disconnect () ;
			SetState ( SS_NOT_ACCESIBLE , SS_STR_NOT_ACCESIBLE ) ;
			return ;
			 break ;
		case SHUTTING_DOWN :
			 m_szState = SZ_SHUTING_DOWN ;
			 break ;
		case LDB_SHUTDOWN :
			 m_szState = SZ_SHUTING_DOWN ;
			 m_szState += "/LDB SHUTDOWN" ;

			break ;
		case LDB_RYPTURE_CONNECTION :
			 m_szState = SZ_SHUTING_DOWN ;
			 m_szState += "/LDB SHUTDOWN" ;

			break ;
		case MEMORY_ALERT :
			 m_szState = SZ_SHUTING_DOWN ;
			 m_szState += "/MEMORY ALERT" ;

			break ;
		case SERVER_READY :
			 m_szState = SZ_READY;
			GetState(eState) ;
			if ( SS_INACTIVE != eState )
				SetState ( SS_ACTIVE , SS_STR_ACTIVE ) ;
			break ;
		case ADMIN_SHUTDOWN :
			 m_szState = SZ_SHUTING_DOWN ;
			 m_szState += "/ADMIN SHUTDOWN" ;

			break ;
		case TCP_PORT_BUSY :
			 m_szState = SZ_SHUTING_DOWN ;
			 m_szState += "/TCP ERROR" ;

			break ;
		case CONFIG_ERROR :
			 m_szState = SZ_SHUTING_DOWN ;
			 m_szState += "/CONFIGURATION ERROR" ;

			break ;
		case SERVICE_SHUTDOWN :
			 m_szState = SZ_SHUTING_DOWN ;
			 m_szState += "/SERVICE ERROR" ;

			break ;
		}

		EstimateLoad () ;
  //## end ClLoadEstimatorSS::Update%938714043.body
}

m4return_t ClLoadEstimatorSS::ShutDown ()
{
  //## begin ClLoadEstimatorSS::ShutDown%940242447.body preserve=yes
//		m_AppCtlControl -> Stop ( M4_SERVER_STOPPED ) ;
//		NotifyDispatcher (M4_SERVER_STOPPED) ;


		return M4_SUCCESS ;
  //## end ClLoadEstimatorSS::ShutDown%940242447.body
}

m4bool_t ClLoadEstimatorSS::IsReadyToClose ()
{
  //## begin ClLoadEstimatorSS::IsReadyToClose%940244005.body preserve=yes

	return M4_TRUE ;
  //## end ClLoadEstimatorSS::IsReadyToClose%940244005.body
}

m4return_t ClLoadEstimatorSS::NotifyDispatcher (M4ClString ai_szMessage)
{
  //## begin ClLoadEstimatorSS::NotifyDispatcher%958053247.body preserve=yes
	void * szBuffer ;
	m4return_t iRet ;
	ClParamList * poParamList= NULL;

	
	
	if ( ! m_iDspConnectionId )
	{
		return M4_ERROR ;
	}

	ClSSInvoker * poInvoker = ClSSInvoker::GetInstance ( ) ;

	poInvoker -> GetParamList ( poParamList  ) ;

	szBuffer = new m4char_t [strlen ( ai_szMessage) +1 ] ;
	strcpy ( (m4pchar_t) szBuffer , ai_szMessage ) ;

	if ( M4_ERROR == poParamList -> SetPointer ( BUFFER , szBuffer ) )
	{
		return M4_ERROR ;
	}


	if ( M4_ERROR == poParamList -> SetInt ( M4_PARAM_CONNECTIONID , m_iDspConnectionId))
	{
		return M4_ERROR ;
	}


	if ( M4_ERROR == ( iRet = poInvoker -> ExecuteAction ( SEND_BUFFER_AS_CLIENT , poParamList ) ) )
	{
		m_bConnected = M4_FALSE ;
	}

	poInvoker -> SetParamList ( *poParamList  ) ;

	if ( iRet == M4_ERROR )
	{
		poInvoker -> GetParamList ( poParamList  ) ;

		if ( M4_ERROR == poParamList -> SetPointer ( BUFFER , szBuffer ) )
		{
			return M4_ERROR ;
		}

		if ( M4_ERROR == poParamList -> SetInt ( M4_PARAM_CONNECTIONID , m_iDspConnectionId))
		{
			return M4_ERROR ;
		}

		poInvoker -> ExecuteAction ( REMOVE_CONNECTION_ACTION , poParamList ) ;
	}


	return iRet ;
  //## end ClLoadEstimatorSS::NotifyDispatcher%958053247.body
}

m4return_t ClLoadEstimatorSS::CreateMessage (m4pchar_t status, M4ClString &ao_Mesage, ClParamList *ai_poParamList)
{
  //## begin ClLoadEstimatorSS::CreateMessage%958053248.body preserve=yes
	m4char_t szBuffer [256] ;
	m4pchar_t szStream = NULL;
	m4uint32_t iStreamSize ;
	M4XMLStreamGenerator oXMLGenerator ("'");
	m4uint32_t iUsers = 0 , iSessions = 0;
	m4double_t dPercentMemory = 0, dCPU = 0 , iMemory = 0, iDouble, dPrivateMB = 0, dMaxProcSizeMB = 0, dFreeRAMMemMB = 0;


	if ( M4_SUCCESS == ai_poParamList->GetDouble ( "NUM_USERS" , iDouble ) )
	{
		iUsers = iDouble ;
	}

	if ( M4_SUCCESS == ai_poParamList->GetDouble ( "NUM_SESSIONS" , iDouble ) )
	{
		iSessions = iDouble ;
	}

	if ( M4_SUCCESS == ai_poParamList->GetDouble ( "CPU_USAGE" , iDouble ) )
	{
		dCPU = iDouble ;
	}
	// bg 165010
	if ( M4_SUCCESS == ai_poParamList->GetDouble ( "M4SERVER_MEMORY_PERCENT_COUNTABILITY" , iDouble ) )
	{
		iMemory  = iDouble ;
	}

	if ( M4_SUCCESS == ai_poParamList->GetDouble ( "CONSUMED_SYSTEM_MEMORY" , iDouble ) )
	{
		dPercentMemory = iDouble ;
	}

	// eh 178555
	if ( M4_SUCCESS == ai_poParamList->GetDouble ( "M4SERVER_MEMORY_COUNTABILITY" , iDouble ) )
	{
		dPrivateMB = iDouble ;
	}

	if ( M4_SUCCESS == ai_poParamList->GetDouble ( "MAX_PROC_SIZE" , iDouble ) )
	{
		dMaxProcSizeMB = iDouble ;
	}

	if ( M4_SUCCESS == ai_poParamList->GetDouble ( "FREE_RAM_MEM" , iDouble ) )
	{
		dFreeRAMMemMB = iDouble ;
	}


	oXMLGenerator.MakeXMLDeclaration();

	oXMLGenerator.InsertComment("@version");

	oXMLGenerator.MakeElement("m4server"); 
	oXMLGenerator.EndElement() ;


	oXMLGenerator.MakeElement("command");
	oXMLGenerator.AddElementAttribute ( "name" , "updateconfiguration" );
	oXMLGenerator.EndElement( ) ;


	oXMLGenerator.MakeElement("params" ) ;


	oXMLGenerator.AddElementAttribute ( "servername" , (m4pcchar_t) m_szConfigName.c_str() ) ;

	sprintf ( szBuffer , "%d" , iUsers ) ;
	oXMLGenerator.AddElementAttribute ( "users" , (m4pcchar_t) szBuffer ) ;

	sprintf ( szBuffer , "%d" , iSessions ) ;
	oXMLGenerator.AddElementAttribute ( "sessions" , (m4pcchar_t) szBuffer ) ;

	sprintf ( szBuffer , "%.2f" , dCPU ) ;
	oXMLGenerator.AddElementAttribute ( "cpu" , (m4pcchar_t) szBuffer ) ;

	sprintf ( szBuffer , "%.2f" , iMemory) ;
	oXMLGenerator.AddElementAttribute ( "server_memory" , (m4pcchar_t) szBuffer ) ;

	sprintf ( szBuffer , "%.2f" , dPercentMemory ) ;
	oXMLGenerator.AddElementAttribute ( "system_memory" , (m4pcchar_t) szBuffer ) ;

	oXMLGenerator.AddElementAttribute ( "state" , (m4pcchar_t) status ) ;

	oXMLGenerator.AddElementAttribute ( "host" , (m4pcchar_t) m_szLocalHost.c_str() ) ;

	sprintf ( szBuffer , "%d" , m_iBasePort) ;
	oXMLGenerator.AddElementAttribute ( "baseport" , (m4pcchar_t) szBuffer ) ;

	
	// eh 178555
	sprintf ( szBuffer , "%.2f" , dPrivateMB ) ;
	oXMLGenerator.AddElementAttribute ( "server_size" , (m4pcchar_t) szBuffer ) ;

	sprintf ( szBuffer , "%.2f" , dMaxProcSizeMB ) ;
	oXMLGenerator.AddElementAttribute ( "max_proc_size" , (m4pcchar_t) szBuffer ) ;

	sprintf ( szBuffer , "%.2f" , dFreeRAMMemMB ) ;
	oXMLGenerator.AddElementAttribute ( "free_ram_mem" , (m4pcchar_t) szBuffer ) ;


	oXMLGenerator.CloseSingleClosedElement ( ) ;

	oXMLGenerator.CloseElement("command");

	oXMLGenerator.CloseElement("m4server");


	iStreamSize = oXMLGenerator.GetXMLStreamSize ( ) ;

	iStreamSize++ ;

	// bugid. 0094194.
    // No hace falta hacer new, ya que en GetXMLStream se devuelve un puntero interno.
    //szStream = new m4char_t [ iStreamSize] ;

    oXMLGenerator.GetXMLStream (szStream, iStreamSize );

	ao_Mesage = szStream ;
    
    // cuando hacemos GetXMLStream nos estamos quedando con el buffer interno del stream.
    // Una vez copiado lo borramos.
    if (szStream != NULL)
    {
        delete szStream;
    }

	return M4_SUCCESS ;
  //## end ClLoadEstimatorSS::CreateMessage%958053248.body
}

m4return_t ClLoadEstimatorSS::EstimateServerLoad (ClParamList &ao_oParamList)
{
  //## begin ClLoadEstimatorSS::EstimateServerLoad%958053249.body preserve=yes
	ClLoadRulesTable::iterator Iterator;
	m4double_t dLoad = 0 , dSSLoad = 0;


	for ( Iterator = m_oLoadRulesTable.begin () ; 
			Iterator != m_oLoadRulesTable.end () ; 
			Iterator ++ )
			{
				if ( M4_ERROR == (*Iterator) -> GetSubsystemLoad ( ao_oParamList ) )
				{
					return M4_ERROR ;
				}
			}

	return M4_SUCCESS ;
  //## end ClLoadEstimatorSS::EstimateServerLoad%958053249.body
}

m4return_t ClLoadEstimatorSS::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClLoadEstimatorSS::ExecuteCommand%958118079.body preserve=yes
	m4return_t iRet = M4_ERROR;
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	if (ai_cCommand < ClSSImplementation::TOTAL_COMMANDS)
		return ClSSImplementation::ExecuteCommand (ai_cCommand, ai_oParamList, ao_oResultList);
	
	switch (ai_cCommand)
	{
	case ESTIMATE_LOAD:		
		iRet = EstimateLoad();		
		break;
		
	default:
		M4_ASSERT("Distributor command not recognized" == NULL);
		break;		
	}

	return iRet;
  //## end ClLoadEstimatorSS::ExecuteCommand%958118079.body
}

m4return_t ClLoadEstimatorSS::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList)
{
  //## begin ClLoadEstimatorSS::GetCommandParams%958118080.body preserve=yes
	m4return_t retVal=M4_ERROR;
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

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
    case ESTIMATE_LOAD:
		// There is not parameters for this command.
		retVal=M4_SUCCESS;
		break;
		
	default:
		M4_ASSERT("Distributor command not recognized" == NULL);
		break;		
	}

	return retVal;
  //## end ClLoadEstimatorSS::GetCommandParams%958118080.body
}

m4return_t ClLoadEstimatorSS::GetCommandsId (ClAdminTagList &aio_oCommandList)
{
  //## begin ClLoadEstimatorSS::GetCommandsId%958118081.body preserve=yes
	eStatus eState ;
	ClAdminTag *newCommand;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	//---------------------------------------------------------------
	// call the base class to get its commands
	//---------------------------------------------------------------
	if (ClSSImplementation::GetCommandsId(aio_oCommandList) != M4_SUCCESS)
		return M4_ERROR;
	
	//---------------------------------------------------------------
	// insert commands for this subsystem
	//---------------------------------------------------------------

	newCommand = ClAdminTag::GetNewAdminTag(ESTIMATE_LOAD, "ESTIMATE_LOAD", M4_FALSE);

	if (newCommand == NULL) 
		return M4_ERROR;
	aio_oCommandList.push_back(newCommand);

	
	return  M4_SUCCESS;
  //## end ClLoadEstimatorSS::GetCommandsId%958118081.body
}

m4return_t ClLoadEstimatorSS::EstimateLoad ()
{
  //## begin ClLoadEstimatorSS::EstimateLoad%958118082.body preserve=yes
	M4ClString szBuffer ;
//	ClAdminTagList oParamTag ;
	ClParamList oParamTag ;
	eStatus eState ;
    m4return_t retCode = M4_ERROR;


	GetState  ( eState ) ;

	if ( eState != SS_ACTIVE )
	{
		return M4_ERROR ;
	}

	if (! m_bConnected )
	{
        retCode = Connect();
		if ( M4_ERROR == retCode )
		{
			return M4_ERROR ;
		}
	}


	if ( M4_SUCCESS == EstimateServerLoad ( oParamTag ) )
	{
		if ( M4_SUCCESS == CreateMessage ( m_szState , szBuffer , &oParamTag ) )
		{
			if( M4_SUCCESS == NotifyDispatcher ( szBuffer ) )
			{
				return M4_SUCCESS ;
			}
		}
	}

	return M4_ERROR ;
  //## end ClLoadEstimatorSS::EstimateLoad%958118082.body
}

m4return_t ClLoadEstimatorSS::InitializeEstimationRules ()
{
  //## begin ClLoadEstimatorSS::InitializeEstimationRules%960220701.body preserve=yes
	UINT auiBlockHandles [100] ;
	m4uint32_t iLenParam,iLenFactor;
	m4size_t iNumBlocks , i , iLenOut;
	m4char_t szSSName [100] ;
	m4char_t szCommandName [100] ;
	m4char_t szParamName [100] ;
	m4char_t szActive [100] ;
	ClConfiguration * poConfiguration ;
	m4uint32_t iHndProp ;
	m4void_t * iHndMatch ;
	ClLoadRule * poLoadRule ;
	m4return_t iRet ;


	if ( M4_ERROR == m_poLoadRulesConfig -> GetAllMatchingBlocks ("", auiBlockHandles ,100 , iNumBlocks ) )
	{
		return M4_ERROR ;
	}

	//Cada objeto configuración corresponde a un SS que intervicene en la estimación de la carga del server
	for ( i = 0 ; i < iNumBlocks ; i ++ )
	{
		if ( ! (poConfiguration = m_poLoadRulesConfig -> GetBloqConfiguration ( auiBlockHandles [ i ] ) ) )
		{
			return  M4_ERROR ;
		}

		//La campo "instancia" de la configuración es el nombre del SS
		if ( M4_ERROR == poConfiguration -> GetInstance (szSSName , 100 , iLenOut ) )
		{
			return M4_ERROR ;
		}

		//El campo "clase" de la configuración es el nombre del comando
		if ( M4_ERROR == poConfiguration -> GetClass (szCommandName , 100 , iLenOut ) )
		{
			return M4_ERROR ;
		}

		poLoadRule = new ClLoadRule ( szCommandName ) ;

		if ( M4_ERROR == poLoadRule -> Initialize (m_poNavigator, szSSName ) )
		{
			return M4_ERROR ;
		}

		//Cada propiedad es un par NOMBRE_PARAMETRO_DE_SALIDA = "Factor asociado a ese parámetro"
		if ( M4_SUCCESS == poConfiguration -> GetFirstProp ("", szParamName, 100 , iLenParam, szActive, 100, iLenFactor, iHndProp, iHndMatch) )
		{
			do 
			{
				if ( ! stricmp ( szActive , ENABLED ) )
				{
					poLoadRule ->SetParamName ( szParamName ) ;
				}

				iRet = poConfiguration -> GetNextProp ("", szParamName, 100 , iLenParam, szActive, 100, iLenFactor, iHndProp, iHndMatch)  ;

			}while ( iRet == M4_SUCCESS ) ;
		}

		m_oLoadRulesTable.push_back ( poLoadRule ) ;

	}

	return M4_SUCCESS ;
  //## end ClLoadEstimatorSS::InitializeEstimationRules%960220701.body
}

ClSSInterfaz * ClLoadEstimatorSS::GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables)
{
  //## begin ClLoadEstimatorSS::GenerateSubsystem%960220702.body preserve=yes
	switch ( ai_lType )
	{
	case M4_LOAD_ESTIMATION_RULES :

		m_poLoadRulesConfig = ai_poConfiguration ;

	}

	return NULL ;
  //## end ClLoadEstimatorSS::GenerateSubsystem%960220702.body
}

m4return_t ClLoadEstimatorSS::CalculateLoad (ClParamList &ai_pParams)
{
  //## begin ClLoadEstimatorSS::CalculateLoad%962640310.body preserve=yes
	m4uint32_t iUsers = 0 , iSessions = 0, iMemory = 0;
	m4double_t dPercentMemory = 0, dCPU = 0 , dLoad , iDouble , dloadaux;
	m4return_t iRet ;

	if ( M4_SUCCESS == ai_pParams.GetDouble ( "NUM_USERS" , iDouble ) )
	{
		iUsers = iDouble ;
	}

	if ( M4_SUCCESS == ai_pParams.GetDouble ( "NUM_SESSIONS" , iDouble ) )
	{
		iSessions = iDouble ;
	}

	if ( M4_SUCCESS == ai_pParams.GetDouble ( "CPU_USAGE" , iDouble ) )
	{
		dCPU = iDouble ;
	}

	if ( M4_SUCCESS == ai_pParams.GetDouble ( "M4SERVER_MEMORY_COUNTABILITY" , iDouble ) )
	{
		iMemory  = iDouble ;
	}

	if ( M4_SUCCESS == ai_pParams.GetDouble ( "CONSUMED_SYSTEM_MEMORY" , iDouble ) )
	{
		dPercentMemory = iDouble ;
	}

	dloadaux = iUsers ;
	dloadaux /= 100 ;  //1 si hay 100 usuarios
	dLoad = dloadaux ;

	dloadaux = iSessions ;
	dloadaux /= 100 ;  //1 si hay 100 sessiones 
	dLoad += dloadaux ;

	dloadaux = iMemory ;
	dloadaux /= 100 ;  //1 si tiene 100Mb 
	dLoad += dloadaux ;

	dloadaux = 3*dPercentMemory ;
	dloadaux /= 100 ;  //3 si está al 100 de memoria
	dLoad += dloadaux ;

	dloadaux = 2*dCPU ;
	dloadaux /= 100  ; //2 si está al 100 de CPU
	dLoad += dloadaux ;

	iRet = ai_pParams.SetDouble ( "LOAD" , dLoad ) ;
	
	return iRet ;

  //## end ClLoadEstimatorSS::CalculateLoad%962640310.body
}

m4return_t ClLoadEstimatorSS::Connect ()
{
  //## begin ClLoadEstimatorSS::Connect%969382660.body preserve=yes
	m4return_t iRet ;
	m4double_t dValue  ;
	ClSSInvoker * poInvoker ;
	ClParamList * poParamList ;



	poInvoker = ClSSInvoker::GetInstance ( ) ;

	poInvoker -> GetParamList ( poParamList  ) ;

	if ( M4_ERROR == poParamList -> SetInt ( PORT , m_lDispatcherPort ) )
	{
        poParamList->Clear();
        delete poParamList;
		return M4_ERROR ;
	}

	if ( M4_ERROR == poParamList -> SetString ( HOST , m_szDispatcherHost ) )
	{
        poParamList->Clear();
        delete poParamList;
		return M4_ERROR ;
	}

	if ( M4_SUCCESS == ( iRet = poInvoker -> ExecuteAction ( OPEN_CONNECTION_ACTION , poParamList ) ) )
	{
		if ( M4_ERROR == poParamList -> GetDouble ( M4_PARAM_CONNECTIONID , dValue ) )
		{
            poParamList->Clear();
            delete poParamList;
			return M4_ERROR ;
		}

		m_iDspConnectionId = dValue ;
		
		m_bConnected = M4_TRUE ;
	}


    poParamList->Clear();
	delete poParamList;
    return iRet ;
  //## end ClLoadEstimatorSS::Connect%969382660.body
}

m4return_t ClLoadEstimatorSS::Disconnect ()
{
  //## begin ClLoadEstimatorSS::Disconnect%995012991.body preserve=yes
	ClParamList * poParamList   ;



	ClSSInvoker * poInvoker = ClSSInvoker::GetInstance ( ) ;

	poInvoker -> GetParamList ( poParamList  ) ;

	poParamList -> SetInt ( M4_PARAM_CONNECTIONID , m_iDspConnectionId  ) ;
	

	return poInvoker -> ExecuteAction ( CLOSE_CONNECTION_ACTION , poParamList ) ;
  //## end ClLoadEstimatorSS::Disconnect%995012991.body
}

// Additional Declarations
  //## begin ClLoadEstimatorSS%37E65D0D006F.declarations preserve=yes
  //## end ClLoadEstimatorSS%37E65D0D006F.declarations

//## begin module%37E661C801B1.epilog preserve=yes
//## end module%37E661C801B1.epilog
