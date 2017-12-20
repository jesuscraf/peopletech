//## begin module%39376A8302AD.cm preserve=no
//## end module%39376A8302AD.cm

//## begin module%39376A8302AD.cp preserve=no
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
//## end module%39376A8302AD.cp

//## Module: lmsubsystem%39376A8302AD; Package body
//## Subsystem: ssccsubsystems::src%38D7880B0333
//## Source file: F:\integration\m4comsrv\src\lmsubsystem.cpp

//## begin module%39376A8302AD.additionalIncludes preserve=no
//## end module%39376A8302AD.additionalIncludes

//## begin module%39376A8302AD.includes preserve=yes
#include <connects40.hpp>
#include <clssinvoker.hpp>
#include <cloblconfiguration.hpp>
#include <m4communication.hpp>
#include <ssnames.hpp>
#include <m4servenv.hpp>
#include <m4srvres.hpp>
#include <cladministrator.hpp>
#include <usinterface.hpp>
#include <cllocalmonitor.hpp>
#include <clsubsystem.hpp>
#include <m4actionconstants.hpp>
#include <ussession.hpp>
#include <cancelevent.hpp>
#include <m4srvtrace.h>
#include <launcher.hpp>
#include <m4props.hpp>
//## end module%39376A8302AD.includes

// actionworker
#include <actionworker.hpp>
// iexecutable
#include <iexecutable.hpp>
// requestdic
#include <requestdic.hpp>
// LMStats
#include <lmstats.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>
// clssaction
#include <clssaction.hpp>
// xmlrequest
#include <xmlrequest.hpp>
// lmsubsystem
#include <lmsubsystem.hpp>

class ClLocalMonitorInterface;

//## begin module%39376A8302AD.declarations preserve=no
//## end module%39376A8302AD.declarations

//## begin module%39376A8302AD.additionalDeclarations preserve=yes
#define CERTF	"servercert.pem"
#define KEYF	"serverpriv.pem"


#ifdef _DEBUG
extern m4uint32_t		g_uiPDUSInside;
#endif

const m4pchar_t SRV_CONFIG_NUM_COMM_WORKERS  = "NUM_COMM_WORKERS";
const m4pchar_t SRV_CONFIG_NUM_SS_WORKERS  = "NUM_SS_WORKERS";
const m4uint32_t SRV_CONFIG_MIN_NUM_WORKERS  = 1;
const m4uint32_t SRV_CONFIG_MAX_NUM_WORKERS  = 30;


#define M4_MEM_DBG_COMP_ID		5
#define M4_SRV_TRACE_COMP_ID	"COMSRV"
//## end module%39376A8302AD.additionalDeclarations


// Class ClLocalMonitorSS 



extern M4_DECL_M4REQUEST M4LogFile g_oBenchmark;



ClLocalMonitorSS::ClLocalMonitorSS (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
  //## begin ClLocalMonitorSS::ClLocalMonitorSS%882177093.hasinit preserve=no
  //## end ClLocalMonitorSS::ClLocalMonitorSS%882177093.hasinit
  //## begin ClLocalMonitorSS::ClLocalMonitorSS%882177093.initialization preserve=yes
  : ClSSImplementation ( ai_poConfiguration, ai_poTables , ai_poNavigator) ,
   m_oActionsQ (50120) , m_oActionsRecycleBin (50120) 
  //## end ClLocalMonitorSS::ClLocalMonitorSS%882177093.initialization
{
  //## begin ClLocalMonitorSS::ClLocalMonitorSS%882177093.body preserve=yes
	
	oMutexMap.Init () ;

	g_oBenchmark.WriteLine("") ;

	m_poGBVisitor = NULL ;
	ClSSInvoker* poInvoker = ClSSInvoker::GetInstance() ;
	m_poCommunication = NULL ;
  //## end ClLocalMonitorSS::ClLocalMonitorSS%882177093.body
}


ClLocalMonitorSS::~ClLocalMonitorSS ()
{
  //## begin ClLocalMonitorSS::~ClLocalMonitorSS%882177094.body preserve=yes
  //## end ClLocalMonitorSS::~ClLocalMonitorSS%882177094.body
}



//## Other Operations (implementation)
void ClLocalMonitorSS::SetLocalMonitor (ClLocalMonitor *ai_poLocalMonitor)
{
  //## begin ClLocalMonitorSS::SetLocalMonitor%882177095.body preserve=yes

	m_poLocalMonitor = ai_poLocalMonitor ;
  //## end ClLocalMonitorSS::SetLocalMonitor%882177095.body
}

ClLocalMonitorInterface * ClLocalMonitorSS::GetLocalMonitor ()
{
  //## begin ClLocalMonitorSS::GetLocalMonitor%882177096.body preserve=yes
  return m_poLocalMonitor ;
  //## end ClLocalMonitorSS::GetLocalMonitor%882177096.body
}

void ClLocalMonitorSS::Accept (ClLMVisitor &ai_oVisitor)
{
  //## begin ClLocalMonitorSS::Accept%884777562.body preserve=yes
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return ;


	ai_oVisitor.VisitLM (  *((ClLocalMonitor*) GetLocalMonitor ( ) ) )  ;

  //## end ClLocalMonitorSS::Accept%884777562.body
}

m4return_t ClLocalMonitorSS::Synchronize ()
{
  //## begin ClLocalMonitorSS::Synchronize%884942700.body preserve=yes
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	if (  m_poVisitor )
	{
		Accept (  *( (ClLMVisitor * )  m_poVisitor ) ) ;
	}


	m_poLocalMonitor -> Synchronize ( ) ;

	return ClSSImplementation::Synchronize () ;
  //## end ClLocalMonitorSS::Synchronize%884942700.body
}

m4return_t ClLocalMonitorSS::Initialize ()
{
  //## begin ClLocalMonitorSS::Initialize%885806477.body preserve=yes
	eStatus eState ;
//	m4char_t szControlPort [100] ;
	m4uint32_t  iControlPort;
	m4double_t daux;
	ClSystem * poSystem ;
	m4uint32_t iNumCommWorkers = 5 , iNumSSWorkers = 5 , iWorker ;
	ClActionWorker * poWorker ;
	m4char_t szHostName [ OBL_PROPVALUE_MAX_LEN ] , szBasePort [ OBL_PROPVALUE_MAX_LEN ] ;



	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;



	poSystem = ClSystem::Instance () ;

	m_poVisitor = new ClLMVisitor ;

	m_poGBVisitor = new ClLMGBVisitor ;

	if ( M4_ERROR == GetEnviron ("SERVER" , BASE_PORT , szHostName , OBL_PROPVALUE_MAX_LEN ) )
	{
		if ( M4_ERROR == GetConfigNumericValue ( BASE_PORT , daux) )
		{
			M4_SRV_ERROR( M4_SRV_CONFIG_PROP_ERROR , "No found %0:s in Subsystem %1:s", "BASE_PORT" << GetName ( ) );

			return M4_ERROR ;
		}
	}
	else
	{
		if( ! ( daux = atoi ( szHostName ) ) )
		{
			M4_SRV_ERROR( M4_SRV_CONFIG_PROP_ERROR , "No found %0:s in Subsystem %1:s", "BASE_PORT" << "SERVER" );
			return M4_ERROR ;
		}
	}
				

	m_iBasePort = iControlPort = daux;
	iControlPort = iControlPort +3;

	if ( M4_ERROR == GetEnviron ("SERVER" , "SERVER_HOST_NAME" , szHostName , OBL_PROPVALUE_MAX_LEN ) )
	{
		M4_SRV_ERROR( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s", "SERVER_HOST_NAME" << "SERVER" );

		return M4_ERROR ;
	}

	sprintf ( szBasePort , "%d" , (m4uint32_t) daux ) ;

	ClIdGenerator::SetServerKey ( szHostName , szBasePort ) ;

	if ( M4_SUCCESS  == GetConfigNumericValue ( SRV_CONFIG_NUM_COMM_WORKERS , daux ) )
	{
		iNumCommWorkers = daux ;
	}

	if ( M4_SUCCESS  == GetConfigNumericValue ( SRV_CONFIG_NUM_SS_WORKERS , daux ) )
	{
		iNumSSWorkers = daux ;
	}

	if ( iNumCommWorkers  > SRV_CONFIG_MAX_NUM_WORKERS )
	{
		M4_SRV_WARNING( M4_SRV_CONFIG_PROP_VALUEWARNING 
			, "Configuration property: %0:s in Subsystem: %1:s, was asigned a no valid value of %2:s. Only accepted values in range: %3:s - %4:s"
			,SRV_CONFIG_NUM_COMM_WORKERS  << "COMMUNICATION_SUBSYSTEM" << iNumCommWorkers  << SRV_CONFIG_MIN_NUM_WORKERS << SRV_CONFIG_MAX_NUM_WORKERS << SRV_CONFIG_MAX_NUM_WORKERS);
//		M4_SRV_WARNING(M4_SRV_CONFIG_PROP_VALUE,"Invalid value of property '%s'. Value must be between %d and %d. Set to %d" , SRV_CONFIG_NUM_COMM_WORKERS << SRV_CONFIG_MIN_NUM_WORKERS << SRV_CONFIG_MAX_NUM_WORKERS << SRV_CONFIG_MAX_NUM_WORKERS);
		iNumCommWorkers  = SRV_CONFIG_MAX_NUM_WORKERS ;
	}
	
	if ( iNumCommWorkers  < SRV_CONFIG_MIN_NUM_WORKERS )
	{
		M4_SRV_WARNING( M4_SRV_CONFIG_PROP_VALUEWARNING , 
			"Configuration property: %0:s in Subsystem: %1:s, was asigned a no valid value of %2:s. Only accepted values in range: %3:s - %4:s"
			,SRV_CONFIG_NUM_COMM_WORKERS  << "COMMUNICATION_SUBSYSTEM" << iNumCommWorkers  << SRV_CONFIG_MIN_NUM_WORKERS << SRV_CONFIG_MAX_NUM_WORKERS << SRV_CONFIG_MIN_NUM_WORKERS);
//		M4_SRV_WARNING(M4_SRV_SS_PROPERTY_ERROR,"Invalid value of property '%s'. Value must be between %d and %d. Set to %d" , SRV_CONFIG_NUM_COMM_WORKERS << SRV_CONFIG_MIN_NUM_WORKERS << SRV_CONFIG_MAX_NUM_WORKERS << SRV_CONFIG_MIN_NUM_WORKERS );
		iNumCommWorkers  = SRV_CONFIG_MIN_NUM_WORKERS ;
	}

	
	if ( iNumSSWorkers  > SRV_CONFIG_MAX_NUM_WORKERS )
	{
		M4_SRV_WARNING( M4_SRV_CONFIG_PROP_VALUEWARNING , 
			"Configuration property: %0:s in Subsystem: %1:s, was asigned a no valid value of %2:s. Only accepted values in range: %3:s - %4:s"
			,SRV_CONFIG_NUM_COMM_WORKERS  << "COMMUNICATION_SUBSYSTEM" << iNumSSWorkers  << SRV_CONFIG_MIN_NUM_WORKERS << SRV_CONFIG_MAX_NUM_WORKERS << SRV_CONFIG_MAX_NUM_WORKERS);
//		M4_SRV_WARNING(M4_SRV_SS_PROPERTY_ERROR,"Invalid value of property '%s'. Value must be between %d and %d. Set to %d" , SRV_CONFIG_NUM_SS_WORKERS << SRV_CONFIG_MIN_NUM_WORKERS << SRV_CONFIG_MAX_NUM_WORKERS << SRV_CONFIG_MAX_NUM_WORKERS);
		iNumSSWorkers  = SRV_CONFIG_MAX_NUM_WORKERS ;
	}
	
	if ( iNumSSWorkers  < SRV_CONFIG_MIN_NUM_WORKERS )
	{
		M4_SRV_WARNING( M4_SRV_CONFIG_PROP_VALUEWARNING , 
			"Configuration property: %0:s in Subsystem: %1:s, was asigned a no valid value of %2:s. Only accepted values in range: %3:s - %4:s"
			,SRV_CONFIG_NUM_COMM_WORKERS  << "COMMUNICATION_SUBSYSTEM" << iNumSSWorkers  << SRV_CONFIG_MIN_NUM_WORKERS << SRV_CONFIG_MAX_NUM_WORKERS << SRV_CONFIG_MIN_NUM_WORKERS);
//		M4_SRV_WARNING(M4_SRV_SS_PROPERTY_ERROR ,"Invalid value of property '%s'. Value must be between %d and %d. Set to %d" , SRV_CONFIG_NUM_SS_WORKERS << SRV_CONFIG_MIN_NUM_WORKERS << SRV_CONFIG_MAX_NUM_WORKERS << SRV_CONFIG_MIN_NUM_WORKERS );
		iNumSSWorkers  = SRV_CONFIG_MIN_NUM_WORKERS ;
	}
	
	ClSSInvoker * poInvoker  = ClSSInvoker::GetInstance ( ) ;

	ClLauncherInterface::SetSSInvoker ( poInvoker ) ;

	m_poCommunication = new ClCommunication ( iControlPort , iNumCommWorkers ) ;
				
	poInvoker -> RegisterAction ( SEND_REQUEST_ACTION ,RETURN_REQUEST , this ) ;
	poInvoker -> RegisterAction ( SEND_BUFFER_AS_CLIENT ,SEND_BUFFER_AS_CLIENT_ACTION_ID, this ) ;
	poInvoker -> RegisterAction ( CLOSE_CONNECTION_ACTION ,CLOSE_CONNECTION_ACTION_ID , this ) ;
	poInvoker -> RegisterAction ( REMOVE_CONNECTION_ACTION ,REMOVE_CONNECTION_ID , this ) ;
	poInvoker -> RegisterAction ( OPEN_CONNECTION_ACTION ,OPEN_CONNECTION_ACTION_ID , this ) ;
	poInvoker -> RegisterAction (  REGISTER_REQUEST_ACTION , REGISTER_REQUEST_ACTION_ID , this ) ;
	poInvoker -> RegisterAction (  CANCEL_USER_REQUEST_ACTION ,CANCEL_USER_REQUEST_ACTION_ID , this ) ;
	poInvoker -> RegisterAction (  DEREGISTER_REQUEST_ACTION , DEREGISTER_REQUEST_ACTION_ID , this ) ;
	poInvoker -> RegisterAction (  PROCESS_REQUEST_ACTION, PROCESS_REQUEST_ACTION_ID , this ) ;
	poInvoker -> RegisterAction (  POST_WRITE_PDU_ACCTION, POST_WRITE_PDU_PROTOCOL_CONNECTION , this ) ;
	poInvoker -> RegisterAction (  REPLACE_REQUEST , REPLACE_REQUEST_ACTION_ID , this ) ;

	
	if ( m_poCommunication -> GetError () )
	{
		M4_SRV_ERROR( M4_SRV_LM_COMM_BEGIN_ERROR , "Error initializing Communication Layer. Initial control port: %0:d" , iControlPort) ;
		return M4_ERROR ;
	}
	
	for (iWorker = 0 ;iWorker < iNumSSWorkers ;iWorker ++ )
	{
		if ( ! ( poWorker = new ClSSActionWorker ( m_oActionsQ , &m_oActionsRecycleBin ) ) )
		{
			return M4_ERROR ;
		}
		
		m_oWorkerList.push_back ( poWorker ) ;
	}

	m_poStatistics = m_poLocalMonitor -> GetStats ( );
	
	return M4_SUCCESS ;
  //## end ClLocalMonitorSS::Initialize%885806477.body
}

m4return_t ClLocalMonitorSS::ShutDown ()
{
  //## begin ClLocalMonitorSS::ShutDown%885806522.body preserve=yes

	ClSSImplementation::ShutDown () ;


	m_poLocalMonitor -> ShutDown ( ) ;

	delete m_poLocalMonitor ;

	m_poLocalMonitor = NULL ;

	return M4_SUCCESS ;
  //## end ClLocalMonitorSS::ShutDown%885806522.body
}

m4return_t ClLocalMonitorSS::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClLocalMonitorSS::ExecuteCommand%901709449.body preserve=yes
	eStatus						eState ;
	ClAdminTagList::iterator	it;
	ClCSAdministrator *			poAdministrator ;
	ClAdminTag *				poAdminTag;
	m4handle_t					hAdministrator;

	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	switch ( ai_cCommand )
	{
	case 111 : //Cableo para desregistrar administradores de la lista del ML
		// search argument this
		it = find_if(ai_oParamList.begin(), ai_oParamList.end(), search_by_tag_id(0));
		if (it == ai_oParamList.end())
		{
			M4_SRV_ERROR ( M4_SRV_SS_COMMAND_PARAM_ERROR , "Parameter '%0:s' not found executing command '%1:s' in subsystem '%2:s'" , 0 << 111 << GetName () ) ;
//			M4_SRV_ERROR_N( M4_SRV_SS_COMMAND_PARAM_ERROR , "Error Desregistering Administrator after disconnection" ) ;
			return M4_ERROR ;
		}
		else
		{
			poAdminTag = *it;
			sscanf(poAdminTag->m_tagValue, M4_PRIdPTR, &hAdministrator);
			poAdministrator = (ClCSAdministrator *)hAdministrator;

			if ( poAdministrator )
			{
				m4uint32_t iConnId = poAdministrator -> m_poTSAP -> GetId () ;
				CloseConnection ( iConnId ) ;
				((ClLocalMonitor*)m_poLocalMonitor )-> ShutDownAdministrator ( poAdministrator ) ;
			}
		}

		break ;
	case GARBAGE_COLLECTION:
		GarbageCollection () ;
		break  ;

	case ADD_COMMUNICATION_WORKER:
		return AddCommWorker () ;

	case REMOVE_COMMUNICATION_WORKER  :
		return RemoveCommWorker () ;
	default :

		M4_SRV_ERROR ( M4_SRV_COMMAND_ID_ERROR, "Error Executing command in '%0:s' Subsystem. Unknown command id: %1:d" , GetName() << ai_cCommand ) ;
		return M4_ERROR ;
	}
	return M4_SUCCESS ;
  //## end ClLocalMonitorSS::ExecuteCommand%901709449.body
}

m4return_t ClLocalMonitorSS::GetCommandsId (ClAdminTagList &aio_oCommandList)
{
  //## begin ClLocalMonitorSS::GetCommandsId%901709450.body preserve=yes
	ClAdminTag * newCommand ;
	eStatus eState ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return M4_ERROR ;

	ClSSImplementation::GetCommandsId (aio_oCommandList ) ;

	newCommand = ClAdminTag::GetNewAdminTag(GARBAGE_COLLECTION, "GARBAGE_COLLECTION", M4_TRUE);
	if (newCommand == NULL) 
		return M4_ERROR ;
	aio_oCommandList.push_back(newCommand);

	newCommand = ClAdminTag::GetNewAdminTag(ADD_COMMUNICATION_WORKER, "ADD_COMMUNICATION_WORKER", M4_FALSE);

	if (newCommand == NULL) 
		return M4_ERROR ;
	aio_oCommandList.push_back(newCommand);

	newCommand = ClAdminTag::GetNewAdminTag(REMOVE_COMMUNICATION_WORKER  , "REMOVE_COMMUNICATION_WORKER", M4_FALSE);

	if (newCommand == NULL) 
		return M4_ERROR ;
	aio_oCommandList.push_back(newCommand);


	return M4_SUCCESS ;
  //## end ClLocalMonitorSS::GetCommandsId%901709450.body
}

m4return_t ClLocalMonitorSS::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList)
{
  //## begin ClLocalMonitorSS::GetCommandParams%901709451.body preserve=yes
	return M4_SUCCESS ;
  //## end ClLocalMonitorSS::GetCommandParams%901709451.body
}

void ClLocalMonitorSS::GarbageCollection ()
{
  //## begin ClLocalMonitorSS::GarbageCollection%936177782.body preserve=yes
    // este metodo se ejecuta por la línea
    //      COMMUNICATION_SERVER = "GARBAGE_COLLECTION"
    // del startup.obl.

	eStatus eState ;
	ClBaseAction * poAction ;


	GetState(eState) ;
	if ( SS_NOT_ACCESIBLE == eState )
		return ;

	if (  m_poGBVisitor )
	{
		Accept (  *( (ClLMVisitor * )  m_poGBVisitor ) ) ;
	}

	/*
     * esto también se hace en ClSystem::GarbageCollection()
     * por lo que es redundante. Lo comentamos.
     */
    //ClRequest::s_oGarbageCollector.Purge();

	((ClLocalMonitor *)m_poLocalMonitor ) -> GetSessionDic ( ) -> Synchronize ( ) ;

	((ClLocalMonitor *)m_poLocalMonitor ) -> m_oClosedConnection.Synchronize ( ) ;

	((ClLocalMonitor *)m_poLocalMonitor ) -> m_oActiveList.Synchronize () ;

	while ( m_oActionsRecycleBin.Size () )
	{
		m_oActionsRecycleBin.Pop (poAction) ;

		if ( poAction ) 
			delete poAction ;
	}

  //## end ClLocalMonitorSS::GarbageCollection%936177782.body
}

m4return_t ClLocalMonitorSS::Start ()
{
  //## begin ClLocalMonitorSS::Start%954935299.body preserve=yes
	m4double_t iPDUPort = 0 ;
	m4double_t iAdminPort = 0;
	m4double_t iBasePort = 0 ;
	m4double_t iSSLPort = 0 ;
	m4char_t szCertPath [ MAX_CONFIG_PATH_LEN ] = "" ;
	m4char_t szCypherMethod [MAXNAMEPROP] = "";
	m4string_t szKeyf ;
	m4string_t szCertf ;
	m4size_t iLen ;
	ClSystem * poSystem = ClSystem::Instance () ;
	ClActionWorkerList::iterator Iterator;
	m4uint32_t iWorker = 1 ;
	m4char_t szWorker [50] ;




	if (m_poCommunication )
	{

		if ( GetConfigStringValue ( "CERTIFICATE_PATH" , szCertPath  , MAX_CONFIG_PATH_LEN, iLen ) == M4_ERROR )
		{
			
			M4_SRV_WARNING ( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s","CERTIFICATE_PATH" << GetName ( ) );
//			SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , WARNINGLOG , "No 'CERTIFICATE_PATH' property found in Communication Subsystem" ) ;
			
			cout << ": Certificate files Path not specified\n" << flush ;
			
		}
		
		szKeyf = szCertPath  ;
		szKeyf += KEYF ;
		szCertf = szCertPath  ;
		szCertf += CERTF ;
		

/*		if ( M4_ERROR == GetConfigNumericValue  ( BASE_PORT , iBasePort ) )
		{
			M4_SRV_ERROR ( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s","BASE_PORT" << GetName ( ) );
//			M4_SRV_ERROR_N ( M4_SRV_SS_PROPERTY_ERROR , "No 'BASE_PORT' property found in Communication Subsystem" ) ;
			return M4_ERROR ;
		}
*/
		if ( !m_iBasePort )
		{
			return M4_ERROR ;
		}

//			m_iBasePort = iBasePort ;
		iPDUPort = m_iBasePort;
		iSSLPort = m_iBasePort + 1 ;
		iAdminPort = m_iBasePort + 2 ;

		if ( M4_ERROR == m_poCommunication -> StartAccepting((m4int_t) iPDUPort,POST_ACCEPT_PDU_ACCTION ,this, M4_COMM_PDU_PROTOCOL) )
		{
			M4_SRV_ERROR ( M4_SRV_LM_BAD_PORT_NUMBER ,  "Error openning port number %0:d in %1:s mode" , iPDUPort << "normal") ;
			return M4_ERROR ;
		}

		cout << endl << "Accepting port: \t\t" << iPDUPort <<  endl ;


		if ( M4_ERROR == GetConfigStringValue ( SSL_CYPHER_METHOD , szCypherMethod , MAXNAMEPROP , iLen ) )
		{
			M4_SRV_WARNING ( M4_SRV_CONFIG_PROP_ERROR , "Configuration property: %0:s not found in Subsystem %1:s",SSL_CYPHER_METHOD  << GetName ( ) );
//			SETCODEF ( M4_SRV_SS_PROPERTY_ERROR , WARNINGLOG , "No %s  property found in Communication Subsystem" , SSL_CYPHER_METHOD ) ;
		}


		if ( M4_ERROR == m_poCommunication -> StartAccepting( (m4int_t )iSSLPort ,POST_ACCEPT_PDU_ACCTION ,this, M4_COMM_PDU_PROTOCOL , szCertf , szKeyf , szCypherMethod ) )
		{
			M4_SRV_ERROR ( M4_SRV_LM_BAD_PORT_NUMBER , "Error openning port number %0:d in %1:s mode" , iSSLPort << "SSL" ) ;
			return M4_ERROR ;
		}

		cout << "SSL Accepting port: \t\t" <<  iSSLPort << endl ;

		if ( M4_ERROR == m_poCommunication -> StartAccepting( (m4int_t )iAdminPort ,POST_ACCEPT_PDU_ADMIN_ACCTION ,this, M4_COMM_PDU_PROTOCOL ) )
		{
			M4_SRV_ERROR ( M4_SRV_LM_BAD_PORT_NUMBER , "Error openning port number %0:d in %1:s mode" , iSSLPort << "normal" ) ;
			return M4_ERROR ;
		}

		cout <<  "Administration Accepting port: \t" << iAdminPort << endl ;

		for ( Iterator = m_oWorkerList.begin() ; Iterator != m_oWorkerList.end() ; Iterator ++ )
		{
			sprintf ( szWorker , "LM Worker %d" , iWorker ++) ;
			(*Iterator) -> Start (szWorker) ;
		}
	}

	return ClSSImplementation::Start ( ) ;

  //## end ClLocalMonitorSS::Start%954935299.body
}

m4return_t ClLocalMonitorSS::CreateAction (m4pchar_t ai_szActionType, ClBaseAction *&ao_poAction)
{
  //## begin ClLocalMonitorSS::CreateAction%955440320.body preserve=yes
	ao_poAction= NULL ;
	return M4_ERROR ;
  //## end ClLocalMonitorSS::CreateAction%955440320.body
}

ClSSImplementation * ClLocalMonitorSS::FindService (ClRequest *ai_poRequest)
{
  //## begin ClLocalMonitorSS::FindService%955610151.body preserve=yes
	M4_ASSERT ( 0 ) ;
	//			m_poSchedulerSS -> ProcessItem ( ai_poRequest );

			return NULL ;
  //## end ClLocalMonitorSS::FindService%955610151.body
}

m4return_t ClLocalMonitorSS::ProcessAction (ClActionIdentifier ai_oActionIdentifier, ClParamList *ai_poParamList, ClBaseAction *&ao_poAction)
{
  //## begin ClLocalMonitorSS::ProcessAction%957943668.body preserve=yes
	ClSSAction * poAction = NULL ;



	switch ( ai_oActionIdentifier )
	{
    case POST_ACCEPT_PDU_PROTOCOL_CONNECTIONS_ID :
		return ConnectionAccepted ( ai_poParamList ) ;
		break ;
		
	case POST_READ_PDU_PROTOCOL_CONNECTION :
		return ConnectionReaded ( ai_poParamList ) ;
		break ;

	case POST_ACCEPT_ADMIN_CONNECTIONS :
		return AdminConnectionAccepted ( ai_poParamList ) ;
		break ;

	case POST_WRITE_PDU_PROTOCOL_CONNECTION :
		poAction = new ClRemoveRequestAction ( this ) ;
//		return RemoveRequest ( ai_poParamList ) ;
		break ;
	case RETURN_REQUEST :
		poAction = new ClReturnRequestAction ( this ) ;

//		poAction -> SetParams ( ai_poParamList ) ;

//		return ao_poAction -> Execute () ;
		break ;

	case SEND_BUFFER_AS_CLIENT_ACTION_ID :
		ao_poAction = new ClSendBufferAsClientAction( this ) ;

		ao_poAction -> SetParams ( ai_poParamList ) ;

		return ao_poAction -> Execute () ;
		break ;

	case CLOSE_CONNECTION_ACTION_ID :
		poAction = new CloseConnectionAction ( this ) ;

//		ao_poAction -> SetParams ( ai_poParamList ) ;

//		return ao_poAction -> Execute () ;
		break ;
	case REMOVE_CONNECTION_ID :
		ao_poAction = new ClRemoveConnectionAction ( this ) ;

		ao_poAction -> SetParams ( ai_poParamList ) ;

		return ao_poAction -> Execute () ;
		break ;
	case OPEN_CONNECTION_ACTION_ID :
		ao_poAction = new ClOpenConnectionAction ( this ) ;

		ao_poAction -> SetParams ( ai_poParamList ) ;

		return ao_poAction -> Execute () ;
		break ;
	case REGISTER_REQUEST_ACTION_ID :
		ao_poAction = new ClRegisterRequestAction ( this ) ;
		ao_poAction -> SetParams ( ai_poParamList ) ;
		return ao_poAction -> Execute () ;

		break ;

	case DEREGISTER_REQUEST_ACTION_ID :

		ao_poAction = new ClDeRegisterRequestAction ( this ) ;

		ao_poAction -> SetParams ( ai_poParamList ) ;
		return ao_poAction -> Execute () ;
		break ;

	case CANCEL_USER_REQUEST_ACTION_ID  :
		poAction = new ClCancelUserRequestAction ( this ) ;

//		ao_poAction -> SetParams ( ai_poParamList ) ;
//		return ao_poAction -> Execute () ;
		break ;
	case PROCESS_REQUEST_ACTION_ID :
		poAction = new ClProcessRequestAction ( this ) ;
        if (NULL != poAction)
        {
            // aquí entro en la cola de los action 
            ai_poParamList->SetInt64(M4_PARAM_COMM_EXECUTION_TIME, ClPrecisionTicker::GetTimeTick());
        }
		break ;
		
	case REPLACE_REQUEST_ACTION_ID :
		ao_poAction = new ClReplaceRequest ( this ) ;

		ao_poAction -> SetParams ( ai_poParamList ) ;
		return ao_poAction -> Execute () ;
		break ;

	}

	if ( poAction )
	{
		poAction -> SetParams ( ai_poParamList ) ;
		m_oActionsQ.Push ( poAction ) ;
		return M4_SUCCESS ;
	}


	M4_SRV_ERROR ( M4_SRV_ACTION_UNKNOWN , "Unknown Action ID= %0:d  executing action in %1:s subsystem" , ai_oActionIdentifier << GetName() ) ;
	return M4_ERROR ;
  //## end ClLocalMonitorSS::ProcessAction%957943668.body
}

void * ClLocalMonitorSS::GetElement (m4pchar_t ai_szElement)
{
  //## begin ClLocalMonitorSS::GetElement%958560329.body preserve=yes
	if (! stricmp( ai_szElement , "LOCAL MONITOR" ) )
	{
		return m_poLocalMonitor ;
	}

	if (! stricmp( ai_szElement , "COMMUNICATIONS" ) )
	{
		return m_poCommunication ;
	}

	return NULL ;
  //## end ClLocalMonitorSS::GetElement%958560329.body
}

m4return_t ClLocalMonitorSS::BuildRequestFromPDU (ClConnInstance *ai_poConnection, ClPDU *ai_poPDU, ClRequest *&ao_poRequest, m4uint64_t ai_uiStartRecordTime)
{
  //## begin ClLocalMonitorSS::BuildRequestFromPDU%959932988.body preserve=yes
		m4uint32_t				PDUType ;
		m4pchar_t				serviceId = NULL;

		if ( ! ai_poPDU )
			return M4_ERROR ;


		PDUType = ai_poPDU -> GetTypePDU();

		switch(PDUType)
		{
			case M4_TYPE_PDU_CONTROL:
				ao_poRequest = _CreateRequest(M4_CONTROL_REQ_TYPE, serviceId, ai_poConnection  , ai_poPDU , ai_uiStartRecordTime);
				break;

			case M4_TYPE_PDU_SERVICE:
				((ClRequestPDU *)ai_poPDU) -> GetServiceId(serviceId );
				ao_poRequest = _CreateRequest(M4_SERVICE_REQ_TYPE, serviceId, ai_poConnection  , ai_poPDU , ai_uiStartRecordTime);
				break;

			default:
				M4_SRV_ERROR ( M4_SRV_ERROR_PDU , "Communication SS: Unknown PDU descriptor. Type %0:d" , PDUType ); 
				return M4_ERROR ;
		}

		if (0 == ao_poRequest)
			return M4_ERROR;


	return M4_SUCCESS ;
  //## end ClLocalMonitorSS::BuildRequestFromPDU%959932988.body
}

m4return_t ClLocalMonitorSS::ConnectionReaded (ClParamList *ai_poParamList)
{
  //## begin ClLocalMonitorSS::ConnectionReaded%959932991.body preserve=yes
		m4return_t RetValue = M4_SUCCESS ;
		void *thepointer;
		m4pchar_t m_pcBuffer = NULL ;
		ClPDU * poPDU = NULL;
		int iConnectionID;
		ClParamList * poParamList ;
		ClConnInstance * poConnection ;
		ClRequest * poRequest = NULL ;
		m4millisec_t dStartTime = 0 , dReadedTime = 0 , dQueuedTime = 0;
		ClSSInvoker * poInvoker = ClSSInvoker::GetInstance () ;



	if ( ! m_poCommunication )
	{
		return M4_ERROR ;
	}


	if(M4_ERROR==ai_poParamList->GetM4Return(M4_PARAM_RETURN_RESULT,RetValue))
	{
		M4_SRV_ERROR ( M4_SRV_LM_COMM_READ_PARAM_ERROR , "Communication SS: Error getting %0:s param from action after read a new PDU" , M4_PARAM_RETURN_RESULT ); 
		return M4_ERROR ;
	}
	else
	{
		if(M4_ERROR==ai_poParamList->GetInt(M4_PARAM_CONNECTIONID, iConnectionID))
		{
			M4_SRV_ERROR ( M4_SRV_LM_COMM_READ_PARAM_ERROR , "Communication SS: Error getting %0:s param from action after read a new PDU" , M4_PARAM_CONNECTIONID  ); 
//			M4_SRV_ERROR ( M4_SRV_ERROR_PDU , "Communication SS: Error gettting %0:s param in pdu readed callback" , M4_PARAM_CONNECTIONID ); 
			return M4_ERROR ;
		}

		if(M4_ERROR==RetValue )
		{
//			SETCODEF ( M4_SRV_DISCONNECTION , DEBUGINFOLOG  , "Connection %d disconnected" ,iConnectionID);
 			
			if ( M4_ERROR == ( (ClLocalMonitor *) m_poLocalMonitor ) -> DestroyConnectionId ( iConnectionID) )
			{
				//No es un error puesto que si la connexión es orientada a sesión, se habrá borrado en el closesession

//				M4_SRV_ERROR ( M4_SRV_COMM_ERROR , "Communication SS: Error Closssing connection %0:d" , iConnectionID); 
//				M4_ASSERT ( "Error in LocalMonitor::DestroyConnectionId()" ) ;
				return M4_ERROR ;
			}
		}
		else
		{
			if(M4_ERROR== (RetValue = ai_poParamList->GetPointer(M4_PARAM_READED_PDU,thepointer) ) )
			{
				M4_SRV_ERROR ( M4_SRV_LM_COMM_READ_PARAM_ERROR , "Communication SS: Error getting %0:s param from action after read a new PDU" , M4_PARAM_READED_PDU  ); 
//				M4_SRV_ERROR ( M4_SRV_COMM_ERROR , "Communication SS: Error gettting %0:s param in pdu readed callback" , M4_PARAM_READED_PDU ); 
			}
			else
			{
				if(M4_ERROR==ai_poParamList->GetInt64(M4_PARAM_START_READING_TIME  ,dStartTime))
				{
					M4_SRV_WARNING ( M4_SRV_LM_COMM_READ_PARAM_ERROR , "Communication SS: Error getting %0:s param from action after read a new PDU" , M4_PARAM_START_READING_TIME   ); 
//					M4_SRV_WARNING ( M4_SRV_COMM_ERROR , "Communication SS: Error gettting %s param in pdu readed callback" , M4_PARAM_START_READING_TIME  ); 
					dStartTime = 0 ;
				}				

				if(M4_ERROR==ai_poParamList->GetInt64(M4_PARAM_FINISH_READING_TIME   ,dReadedTime))
				{
					M4_SRV_WARNING ( M4_SRV_LM_COMM_READ_PARAM_ERROR , "Communication SS: Error getting %0:s param from action after read a new PDU" , M4_PARAM_FINISH_READING_TIME ); 
//					M4_SRV_WARNING ( M4_SRV_COMM_ERROR , "Communication SS: Error gettting %s param in pdu readed callback" , M4_PARAM_FINISH_READING_TIME ); 
					dReadedTime = 0 ;
				}				

				if ( poConnection = (ClConnInstance*)( (ClLocalMonitor *) m_poLocalMonitor ) -> m_oConDictionary.GetAndAddRequest ( iConnectionID )  )
				{

					m4Trace(cout <<"+( " << ++g_uiPDUSInside <<  " )" << endl );


					poInvoker -> GetParamList (poParamList) ;
					
					poPDU =(ClPDU*) thepointer ;
					
					poParamList -> SetPointer ( "PDU" , poPDU ) ;
					poParamList -> SetPointer ( "CONNECTION" , poConnection ) ;
					poParamList -> SetInt64(M4_PARAM_START_READING_TIME  ,dStartTime);
					poParamList -> SetInt64(M4_PARAM_FINISH_READING_TIME   ,dReadedTime);
					
					RetValue = poInvoker ->ExecuteAction ( PROCESS_REQUEST_ACTION , poParamList ) ;
				}
				else
				{
					M4_SRV_WARNING ( M4_SRV_LM_CONN_NOT_FOUND, "Communication SS: Error Getting connection number %0:d from connections dictionary. Connection clossed jet" ,  iConnectionID  ) ;
					delete ( (ClPDU*) poPDU ) ;
				}
			}
		}
	}

	return RetValue ;
  //## end ClLocalMonitorSS::ConnectionReaded%959932991.body
}

m4return_t ClLocalMonitorSS::ConnectionAccepted (ClParamList *ai_poParamList)
{
  //## begin ClLocalMonitorSS::ConnectionAccepted%959932992.body preserve=yes
		m4return_t RetValue;
		m4pchar_t pHost ;
		ClConnInstance * poConnection ;



	if ( ! m_poCommunication )
	{
		return M4_ERROR ;
	}

	if(M4_ERROR==ai_poParamList->GetM4Return(M4_PARAM_RETURN_RESULT,RetValue))
	{
		M4_SRV_ERROR ( M4_SRV_LM_COMM_ACCEPT_PARAM_ERROR , "Communication SS: Error getting %0:s param from action after accepting a new connection" , M4_PARAM_RETURN_RESULT); 
//		M4_SRV_ERROR ( M4_SRV_COMM_ERROR , "Communication SS: Error gettting %0:s param in Connection Accepted callback" , M4_PARAM_RETURN_RESULT); 
		return M4_ERROR ;
	}
	else
	{
		if(M4_ERROR==RetValue)
		{
			M4_SRV_ERROR_N ( M4_SRV_COMM_ERROR , "Communication SS: Error in Connection Accepted callback" ); 
			return M4_ERROR ;
		}
		else
		{
			int iConnectionID;
			if(M4_ERROR!=ai_poParamList->GetInt(M4_PARAM_CONNECTIONID, iConnectionID))
			{
//	La conexión definitiva será instanciada por el SessionManager

				//Aquí veremos como instanciamos un tipo u otro de conexión

				// Las conexiones por defecto son "No orientadas a conexion".
				// El Session Manager la reemplazara por una ClConnection si no llega el tag M4_PROP_SERVER_NON_CONN_ORIENTED.
				poConnection = new ClConnection40 ( iConnectionID ) ;

				poConnection -> SetId ( iConnectionID ) ;

				if ( M4_SUCCESS == m_poCommunication -> GetCommParams (iConnectionID, *ai_poParamList ) )
				{
					if ( M4_SUCCESS == ai_poParamList -> GetString ( M4_PARAM_COMM_HOSTNAME , pHost ) )
					{
						poConnection -> SetHost (pHost ) ;
					}
					
					ai_poParamList -> GetString ( M4_PARAM_COMM_IP , pHost ) ;

					poConnection -> SetIP (pHost ) ;
					
				}


				if ( M4_ERROR == ((ClLocalMonitor*)m_poLocalMonitor ) -> m_oConDictionary.AddItem ( iConnectionID, poConnection ) )  
				{
					M4_SRV_ERROR( M4_SRV_LM_DUPPLIED_CONN_ID, "Communication SS: Connection number %0:d couldn't be registered. Or two connections with same identifier" ,  iConnectionID) ;
//					M4_SRV_ERROR_N ( M4_SRV_LM_NEW_CONNECTION_ERROR , "Cannot insert new connection into connections dictionary" ) ;
					
					return M4_ERROR ;
				}
//
				if ( M4_SUCCESS == m_poCommunication->StartListening(iConnectionID, POST_READ_PDU_ACCTION, this ) )
				{
					((ClLMStats*)m_poStatistics ) -> AddNewConnections ( 1 ) ;
				}
					
			}
			else
			{
				M4_SRV_ERROR ( M4_SRV_LM_COMM_ACCEPT_PARAM_ERROR , "Communication SS: Error getting %0:s param from action after accepting a new connection" , M4_PARAM_CONNECTIONID); 
//				M4_SRV_ERROR ( M4_SRV_COMM_ERROR , "Communication SS: Error gettting %0:s param in Connection Accepted callback" , M4_PARAM_CONNECTIONID); 
				return M4_ERROR ;
			}
		}
	}

	return M4_SUCCESS ;
  //## end ClLocalMonitorSS::ConnectionAccepted%959932992.body
}

m4return_t ClLocalMonitorSS::ExecuteAction (m4pchar_t ai_oActionString, ClParamList *ai_poParamList)
{
  //## begin ClLocalMonitorSS::ExecuteAction%959932993.body preserve=yes
	ClSSAction * poAction = NULL ;

	if ( m_poCommunication )
	{
	
		if (! stricmp ( ai_oActionString , POST_ACCEPT_PDU_ACCTION ) )
		{
			return ConnectionAccepted ( ai_poParamList ) ;
		}
		if (! stricmp ( ai_oActionString , POST_ACCEPT_PDU_ADMIN_ACCTION ) )
		{
			return AdminConnectionAccepted ( ai_poParamList ) ;
		}

		 
		if ( ! stricmp ( ai_oActionString , PROCESS_REQUEST_ACTION ) )
		{
			poAction = new ClProcessRequestAction ( this ) ;
		}
		
		if ( ! stricmp ( ai_oActionString , POST_READ_PDU_ACCTION ) )
		{
			return ConnectionReaded ( ai_poParamList ) ;
		}
		
		if (! stricmp ( ai_oActionString , POST_WRITE_PDU_ACCTION ) )
		{
			return RemoveRequest ( ai_poParamList ) ;
		}

		if (! stricmp ( ai_oActionString , CLOSE_CONNECTION_ACTION ) )
		{
//			return CloseConnection( ai_poParamList ) ;
		}

	}

	if ( poAction )
	{
		m_oActionsQ.Push ( poAction ) ;
		return M4_SUCCESS ;
	}

	M4_SRV_ERROR ( M4_SRV_ACTION_UNKNOWN , "Unknown Action ID= %0:s  executing action in %1:s subsystem" , ai_oActionString << GetName() ) ;
//	M4_SRV_ERROR ( M4_SRV_ACTION_UNKNOWN , "Communication SS: Error executting action %0:s" , ai_oActionString ); 
	return M4_ERROR ;
  //## end ClLocalMonitorSS::ExecuteAction%959932993.body
}

m4return_t ClLocalMonitorSS::AdminConnectionAccepted (ClParamList *ai_poParamList)
{
  //## begin ClLocalMonitorSS::AdminConnectionAccepted%959932994.body preserve=yes
		m4return_t RetValue;
		m4pchar_t pHost ;
		ClTSAP * pTSAP ;



	if ( ! m_poCommunication )
	{
		return M4_ERROR ;
	}

	if(M4_ERROR==ai_poParamList->GetM4Return(M4_PARAM_RETURN_RESULT,RetValue))
	{
		M4_SRV_ERROR ( M4_SRV_LM_COMM_ACCEPT_PARAM_ERROR , "Communication SS: Error getting %0:s param from action after accepting a new connection" , M4_PARAM_RETURN_RESULT); 
//		M4_SRV_ERROR ( M4_SRV_COMM_ERROR , "Communication SS: Error gettting %0:s param in Connection Accepted callback" , M4_PARAM_RETURN_RESULT); 
		return M4_ERROR ;
	}
	else
	{
		if(M4_ERROR==RetValue)
		{
			M4_SRV_ERROR_N ( M4_SRV_COMM_ERROR , "Communication SS: Error in Connection Accepted callback" ); 
			return M4_ERROR ;
		}
		else
		{
			int iConnectionID;
			if(M4_ERROR!=ai_poParamList->GetInt(M4_PARAM_CONNECTIONID, iConnectionID))
			{

			    if ( M4_SUCCESS == m_poCommunication -> GetCommParams (iConnectionID, *ai_poParamList ) )
				{
					if ( M4_SUCCESS == ai_poParamList -> GetString ( M4_PARAM_COMM_HOSTNAME , pHost ) )
					{
//						poConnection -> SetHost (pHost ) ;
					}
				}

				if ( M4_SUCCESS == ai_poParamList -> GetPointer ( M4_PARAM_TLIID, (void*&)pTSAP ) )
				{
					pTSAP -> SetId (iConnectionID) ;
					m4TraceHeaderLevel(2, cout << "ClAdminAcceptor::Run -- Create a new administrator" << endl);
					((ClLocalMonitor*)m_poLocalMonitor ) -> CreateAdministrator ( pTSAP ) ;
				}
				
			}
			else
			{
				M4_SRV_ERROR ( M4_SRV_LM_COMM_ACCEPT_PARAM_ERROR , "Communication SS: Error getting %0:s param from action after accepting a new connection" , M4_PARAM_CONNECTIONID); 
//				M4_SRV_ERROR ( M4_SRV_COMM_ERROR , "Communication SS: Error gettting %0:s param in Connection Accepted callback" , M4_PARAM_CONNECTIONID); 
				return M4_ERROR ;
			}
		}
	}

	return M4_SUCCESS ;

	
  //## end ClLocalMonitorSS::AdminConnectionAccepted%959932994.body
}

m4return_t ClLocalMonitorSS::SendRequest (ClRequest *ai_poRequest)
{
  //## begin ClLocalMonitorSS::SendRequest%959932995.body preserve=yes
	ClPDU * poPDU ;
	m4pchar_t pLogBuffer ;
	ClConnInstance * poConnection ;
	m4int32_t idConnection ;
	m4return_t ret ;
	ClLogSystem *  pLog ;
	m4uint32_t lSizeLogError , iLenout ;
	m4uint64_t iRequestId  ;
	ClRequestStatistics	*pReqStats ;		


	if ( ai_poRequest -> m_oStatus.GetValue () == REQ_CANCELING )
	{
		ai_poRequest -> Cancel ( ) ;
	}

	if ( ! m_poCommunication )
	{
		M4_SRV_ERROR_N ( M4_SRV_LM_SEND_REQUEST , "Send request action in Local Monitor Subsystem: No Communication System instantiated. Request will be lost" ) ;
		return M4_ERROR ;
	}

	if ( pReqStats = (ClRequestStatistics	*)ai_poRequest ->GetStats() )
	{
		pReqStats-> Sett( ClRequestStatistics::Writing );
	}

	if ( ! ( poConnection = (ClConnInstance*) ai_poRequest ->GetpConnection () ) )
	{
		M4_SRV_ERROR_N( M4_SRV_LM_SEND_REQUEST , "Send request action in Local Monitor Subsystem: Request without connection" ) ;
		return M4_ERROR ;
	}

	if ( ! (idConnection = poConnection -> GetId () )) 
	{
		M4_SRV_ERROR_N( M4_SRV_LM_SEND_REQUEST , "Send request action in Local Monitor Subsystem: Connection with invalid ID" ) ;
		return M4_ERROR ;
	}

	pLog = (ClLogSystem *) ai_poRequest -> GetLog() ;

	if (! (poPDU = ai_poRequest -> GetOutputPDU () ) )
	{
		if ( pLog )
		{
			pLog -> SetCode (M4_SRV_LM_SEND_REQUEST , ERRORLOG , "Send request action in Local Monitor Subsystem: NULL outputPDU" ) ;
		}
		else
		{
			M4_SRV_ERROR_N ( M4_SRV_LM_SEND_REQUEST  , "Send request action in Local Monitor Subsystem: NULL outputPDU" ) ;
		}
		ai_poRequest  -> SetError( -1 ) ;
//		return M4_ERROR ;
	}


////////////////////////////////////////////////////////////////////////
//	código para añadir la log a la PDU de salida
/////////////////////////////////////////////////////////////////////////////
	if ( pLog )
	{

		ret = pLog -> GetSerialStringSizeFromMessage(lSizeLogError);

		if ( ( lSizeLogError > 0 ) & (ret != M4_ERROR ) )
		{
			// La log está vacía.
	
			pLogBuffer = new m4char_t [ lSizeLogError ];
			
			M4_ASSERT ( pLogBuffer != NULL );
			
			ret = pLog -> GetSerialStringFromMessage( pLogBuffer, lSizeLogError , iLenout );
			
			if ( ( ret == M4_ERROR ) || ( ! lSizeLogError ) )
			{
				// Error : En la m4log hay que poner que ha habido un error al recupoerar los errores de las trazas
				delete []pLogBuffer;
				
				M4_SRV_ERROR_N( M4_SRV_PDULOG_ERROR , "Error getting serialized log information from the PDU Log" ) ;
			}
			else
			{
			
				poPDU -> EnableLogInfo ();
				
				ret = poPDU -> SetLogInfo ( pLogBuffer , lSizeLogError );
				
				if ( ret != M4_SUCCESS )
				{
					// Error : Existe un error cuando añadimos el Log en la PDU de salida
					M4_SRV_ERROR_N ( M4_SRV_PDULOG_ERROR , "Error adding log information to the PDU" ) ;
				}
				
				delete [] pLogBuffer ;
			}
		}
	}	
////////////////////////////////////////////////////////////////////
///////			Log añadida a l aPDU
/////////////////////////////////////////////////////

	iRequestId = ai_poRequest -> GetId () ;

	if ( M4_ERROR == ( ret = m_poCommunication -> Write(idConnection , iRequestId  , poPDU,POST_WRITE_PDU_ACCTION,this) ) )
	{
		M4_SRV_ERROR_N( M4_SRV_LM_SEND_REQUEST , "LM Subsystem: Error writing PDU" ) ;
		RemoveRequest ( idConnection , iRequestId , ClPrecisionTicker::GetTimeTick() ) ;
	}

	return ret ;

  //## end ClLocalMonitorSS::SendRequest%959932995.body
}

m4return_t ClLocalMonitorSS::RemoveRequest (ClParamList *ai_poParamList)
{
  //## begin ClLocalMonitorSS::RemoveRequest%959948239.body preserve=yes
	m4int_t iConnectionID ;
	m4int64_t iRequestId  ;
	m4millisec_t dWriteTime ;
	ClSSInvoker * poInvoker = ClSSInvoker::GetInstance () ;
	ClParamList * poParamList ;



	if(M4_ERROR==ai_poParamList->GetInt(M4_PARAM_CONNECTIONID, iConnectionID))
	{
		M4_SRV_ERROR ( M4_SRV_LM_COMM_WRITE_PARAM_ERROR , "Communication SS: Error getting %0:s param from action after writting a PDU" , M4_PARAM_CONNECTIONID); 
//		M4_SRV_ERROR( M4_SRV_COMM_ERROR, "Communication SS: Error getting %0:s param removing request" ,  M4_PARAM_CONNECTIONID ) ;
		return M4_ERROR ;
	}

	if(M4_ERROR==ai_poParamList->GetInt64 (M4_PARAM_PETITION_ID, iRequestId ))
	{
		M4_SRV_ERROR ( M4_SRV_LM_COMM_WRITE_PARAM_ERROR , "Communication SS: Error getting %0:s param from action after writting a PDU" , M4_PARAM_PETITION_ID ); 
//		M4_SRV_ERROR( M4_SRV_COMM_ERROR, "Communication SS: Error getting %0:s param removing request of connection %1:d" ,  M4_PARAM_PETITION_ID << iConnectionID ) ;
		return M4_ERROR ;
	}

 
	if(M4_ERROR==ai_poParamList->GetInt64 (M4_PARAM_FINISH_WRITTING_TIME, dWriteTime ))
	{
		M4_SRV_WARNING ( M4_SRV_LM_COMM_WRITE_PARAM_ERROR , "Communication SS: Error getting %0:s param from action after writting a PDU" , M4_PARAM_FINISH_WRITTING_TIME ); 
//		M4_SRV_ERROR( M4_SRV_COMM_ERROR, "Communication SS: Error getting %0:s param removing request" ,  M4_PARAM_FINISH_WRITTING_TIME) ;
		dWriteTime = 0 ;
	}

	poInvoker -> GetParamList (poParamList) ;

	poParamList -> SetInt(M4_PARAM_CONNECTIONID, iConnectionID) ;
	poParamList -> SetInt64 (M4_PARAM_PETITION_ID, iRequestId  ) ;
	poParamList -> SetInt64 (M4_PARAM_FINISH_WRITTING_TIME, dWriteTime ) ;

	return poInvoker -> ExecuteAction ( POST_WRITE_PDU_ACCTION , poParamList ) ;
  //## end ClLocalMonitorSS::RemoveRequest%959948239.body
}

ClRequest * ClLocalMonitorSS::_CreateRequest (m4uint32_t ai_uiReqType, m4pchar_t ai_szServiceId, ClConnInstance *ai_oConnection, ClPDU *ai_poInputPDU, m4uint64_t ai_uiStartRecordTime)
{
  //## begin ClLocalMonitorSS::_CreateRequest%962700383.body preserve=yes
	ClRequest	*poRequest = ClRequest::s_oGarbageCollector.CreateRequest(ai_uiReqType, ai_szServiceId , ai_uiStartRecordTime , ai_poInputPDU ->GetIdPDU() );
	if (0 == poRequest)		return 0;

	poRequest->SetPDUInput (ai_poInputPDU);
	poRequest->SetConnection (ai_oConnection) ;
//	((ClRequestStatistics*) poRequest->GetStats() )-> SettReaded () ;
	return poRequest;

  //## end ClLocalMonitorSS::_CreateRequest%962700383.body
}

m4return_t ClLocalMonitorSS::OpenConnection (m4string_t ai_szHost, m4uint32_t ai_iPort, m4objid_t &ao_iConnectionId)
{
  //## begin ClLocalMonitorSS::OpenConnection%962784427.body preserve=yes
	m4return_t iRet ;


	if ( ! m_poCommunication )
	{
		return M4_ERROR ;
	}

	ao_iConnectionId = ClIdGenerator::GetNewId () ;

	if ( M4_SUCCESS != ( iRet = m_poCommunication -> OpenConnection( ao_iConnectionId ,ai_szHost,ai_iPort,M4_COMM_ASCII_PROTOCOL) ) )
	{
		M4_SRV_ERROR (  M4_SRV_LM_OPEN_CONN_ERROR , "Communication SS: Internal error in Communication Layer openning new connection to host %0:s in port %1:d" , ai_szHost.c_str() << ai_iPort ) ;
//		M4_SRV_ERROR( M4_SRV_COMM_ERROR, "Communication SS: Error Opening connection to host %0:s in port %1:d" , ai_szHost.c_str() << ai_iPort ) ;
	}
	
	return iRet ;
  //## end ClLocalMonitorSS::OpenConnection%962784427.body
}

m4return_t ClLocalMonitorSS::SendBuffer (m4objid_t ai_iConnectionId, m4pchar_t ai_szBuffer, m4uint32_t ai_iSize)
{
  //## begin ClLocalMonitorSS::SendBuffer%962784428.body preserve=yes
	m4return_t iRet ;
	m4objid_t iBufferId = 0;
	m4objid_t iId = 0;


//	iBufferId = ClIdGenerator::GetNewId () ;
	if ( ! m_poCommunication )
	{
		return M4_ERROR ;
	}


	if ( M4_ERROR == ( iRet = m_poCommunication -> Write(ai_iConnectionId, iId  ,ai_szBuffer,ai_iSize)  ) )
	{
		M4_SRV_ERROR( M4_SRV_COMM_ERROR, "Communication SS: Error Writting connection %0:d " ,  (m4uint32_t) ai_iConnectionId) ;
	}

	delete ai_szBuffer ;

	return iRet ;
  //## end ClLocalMonitorSS::SendBuffer%962784428.body
}

m4return_t ClLocalMonitorSS::CloseConnection (m4uint32_t &ao_iConnectionId)
{
  //## begin ClLocalMonitorSS::CloseConnection%962784431.body preserve=yes
	if ( ! m_poCommunication )
	{
		return M4_ERROR ;
	}

	m_poCommunication -> CloseConnection ( ao_iConnectionId ) ;

	return M4_SUCCESS;
  //## end ClLocalMonitorSS::CloseConnection%962784431.body
}

m4return_t ClLocalMonitorSS::RemoveBuffer (ClParamList *ai_pParams)
{
  //## begin ClLocalMonitorSS::RemoveBuffer%962784432.body preserve=yes
	m4objid_t iBufferId ;
	m4int64_t iParam ;
	m4int_t iConnectionID ;
	m4return_t iRet;


	if(M4_ERROR==ai_pParams->GetInt(M4_PARAM_CONNECTIONID, iConnectionID))
	{
		M4_SRV_ERROR ( M4_SRV_LM_COMM_WRITE_PARAM_XML_ERROR , "Communication SS: Error getting %0:s param from action after writting an XML buffer" , M4_PARAM_CONNECTIONID); 
//		M4_SRV_ERROR( M4_SRV_COMM_ERROR, "Communication SS: Error Getting %0:s param in XML connection writen callback" ,  M4_PARAM_CONNECTIONID ) ;
		return M4_ERROR ;
	}

	if ( M4_ERROR == ai_pParams -> GetInt64 ( M4_PARAM_PETITION_ID , iParam ) )
	{
		M4_SRV_ERROR ( M4_SRV_LM_COMM_WRITE_PARAM_XML_ERROR , "Communication SS: Error getting %0:s param from action after writting an XML buffer" , M4_PARAM_PETITION_ID ); 
//		M4_SRV_ERROR( M4_SRV_COMM_ERROR, "Communication SS: Error Getting %0:s param in XML connection writen callback" ,  M4_PARAM_PETITION_ID ) ;
		return M4_ERROR ;
	}

	iBufferId = iParam ;


	iRet = RemoveBuffer ( iConnectionID , iBufferId ) ;

	return iRet ;
  //## end ClLocalMonitorSS::RemoveBuffer%962784432.body
}

m4return_t ClLocalMonitorSS::RegisterRequest (ClRequest *ai_poRequest)
{
  //## begin ClLocalMonitorSS::RegisterRequest%969874233.body preserve=yes
	m4return_t iRet = M4_SUCCESS ;
	m4objid_t lIdPDU = 0 ;
	m4objid_t  lIdRequest = 0;
	ClUSSessionInterface * poSession ;
	ClRequestSessionMap::iterator Iterator ;
	m4objid_t * poRequestList ;
	m4objid_t iSessionId ;
	m4uint32_t iNumRequest ;


	lIdRequest = ai_poRequest -> GetId () ;

	if ( poSession = ai_poRequest ->GetpSession ( ) )
	{
		iSessionId = poSession->GetId () ;

		oMutexMap.Lock () ;

		Iterator = m_RequestSessionMap.find ( iSessionId) ;

		if ( Iterator == m_RequestSessionMap.end () )
		{	
			poRequestList = new m4objid_t [50] ;

			memset (poRequestList , 0 , sizeof (m4objid_t) *50 ) ;

			m_RequestSessionMap.insert ( ClRequestSessionMap::value_type ( iSessionId , poRequestList ) ) ;
			
		}
		else 
		{
			poRequestList = (*Iterator).second ;
		}

		oMutexMap.Unlock () ;

		iNumRequest = ( (ClUSSessionStats*) (poSession -> GetStats () ) )-> GetnPendingReqs () ;

		poRequestList [iNumRequest -1] = lIdRequest ;

	}

	if ( M4_ERROR == (iRet = m_oRequestDic.AddItem (lIdRequest, ai_poRequest) ) )
	{
		M4_SRV_ERROR ( M4_SRV_LM_DUPPLIED_REQ_ID , "Error inserting request in the request dictionary. Another request with same ID allredy registered. ID %0:d" , (m4uint32_t) lIdRequest ) ;
	}

	return iRet ;
  //## end ClLocalMonitorSS::RegisterRequest%969874233.body
}

m4return_t ClLocalMonitorSS::CancelRequest (m4objid_t ai_iRequestID , m4objid_t ai_iSessionId )
{
  //## begin ClLocalMonitorSS::CancelRequest%970139114.body preserve=yes
	ClCancelRequestEvent * poEvent ;
	ClSSInterfaz * poProxySubsystem = NULL ;
	ClRequest				* poRequest ;
	m4uint32_t iReqId , iSessId ;
	m4objid_t  ilId;
	m4uint32_t iNumReq = 1 ;
	ClRequestSessionMap::iterator Iterator ;
	m4objid_t * poRequestList , iRequestID;
	m4uint32_t iNumRequest = 0 , i = 0;


	if ( ! poProxySubsystem )
	{
		ClSystem * poSystem = ClSystem::Instance () ;
		poProxySubsystem = poSystem -> FindSubsystemByType ( M4_PROXY_SPACES_SUBSYSTEM ) ;
	}

	if ( ! ai_iRequestID & ! ai_iSessionId  )
		return M4_ERROR ;


	if ( ! ai_iRequestID )
	{
		iSessId = ai_iSessionId ;

		oMutexMap.Lock () ;

		Iterator = m_RequestSessionMap.find ( ai_iSessionId) ;

		if ( Iterator != m_RequestSessionMap.end () )
		{	
			poRequestList = (*Iterator).second ;

			while (poRequestList[i])
			{
				iNumRequest ++ ;
				i++ ;
			}

		}

		oMutexMap.Unlock () ;

	}
	else
	{

		iNumRequest = 1 ;
		poRequestList = &ai_iRequestID;
	}

	for ( i = 0 ; i < iNumRequest ; i++ )
	{
		iRequestID = poRequestList [i] ;

		if ( poProxySubsystem )
		{
			ilId = iRequestID >> 32 ;
			iSessId = ilId & 0xffffffff ;
			iReqId = iRequestID & 0xffffffff ;

			poEvent =new ClCancelRequestEvent  ;
			
			//		poInterfaz = poImplementation -> GetInterfaz () ;
			
			poEvent -> SetRequestId ( iReqId ) ;
			
			poEvent -> SetSessionId ( iSessId ) ;
			
			
			poEvent -> AttachObserver ( poProxySubsystem ) ;
			
			poEvent -> Notify ( REQUEST_CANCELED ) ;

//			poEvent -> DetachObserver ( poProxySubsystem ) ;

			delete poEvent ;
		}
		
		
		
		if (  poRequest = (ClRequest*) m_oRequestDic.GetItem ( iRequestID) ) 
		{
			poRequest -> m_oStatus.SetValue ( REQ_CANCELING ) ;
		}
		
	}	

	return M4_SUCCESS ;
  //## end ClLocalMonitorSS::CancelRequest%970139114.body
}

m4return_t ClLocalMonitorSS::RemoveRequest (m4uint32_t ai_lIdConnection, m4objid_t ai_lIdRequest, m4millisec_t ai_dWrittenTime)
{
  //## begin ClLocalMonitorSS::RemoveRequest%970561855.body preserve=yes
	ClRequest * poRequest ;
	ClConnInstance * poConnection ;
	ClUSSessionStats * poSessionStats ;
	m4objid_t iSessionId ;
	ClRequestSessionMap::iterator Iterator ;
	m4objid_t * poRequestList ;
	m4uint32_t i ;
	ClPDU *	pOutputPDU;
	m4uint8_t opID;
	m4uint16_t uiElapsedTime;


	if ( ! ( poRequest = (ClRequest*) m_oRequestDic.GetItem ( ai_lIdRequest ) ) )
	{
		M4_SRV_ERROR ( M4_SRV_LM_REQ_DESREG_JET , "Communication SS: Error getting request %0:d form request dictionary. Request sending notified twice or duplied id %0:d" , (m4int_t)ai_lIdRequest ) ;
		
		return M4_ERROR ;
	}

	ClRequestStatistics	*pReqStats = (ClRequestStatistics	*)poRequest ->GetStats();

	if ( pReqStats )
	{
		pReqStats -> Sett ( ClRequestStatistics::Deregistering , ai_dWrittenTime);
	}


	if ( M4_ERROR == m_oRequestDic.RemoveItem( ai_lIdRequest )  )
	{
		M4_SRV_ERROR ( M4_SRV_LM_REQ_DESREG_JET , "Communication SS: Error getting request %0:d form request dictionary. Request sending notified twice or duplied id %0:d" , (m4int_t)ai_lIdRequest ) ;
//		M4_ASSERT ( "Esto quiere decir que hay un error de lógica en algún sitio" ) ;
		return M4_ERROR ;
	}

	ClUSSessionInterface * poSession = poRequest -> GetpSession () ;


	poConnection = poRequest -> GetpConnection () ;

	if ( poSession )
	{
		if ( poSessionStats = (ClUSSessionStats *) poSession ->GetStats () )
		{
			M4ClTimeStamp oCrono;
			m4double_t dTime;
			
			oCrono.now () ;
			
			oCrono.get ( dTime ) ;

			pOutputPDU = poRequest -> GetOutputPDU();

			if (pOutputPDU != NULL)
			{
				opID = pOutputPDU -> GetIdPDU() & 0x00ff;

				// Se comprueba si se trata de una request de Ping
				if (M4_ID_PDU_PING == opID)
				{
					// Tiempo en segundos que hay que restar (puesto que se trata realmente de la última acción 
					uiElapsedTime = ((ClPingPDU *)pOutputPDU) -> GetElapsedTime();
					dTime -= m4double_t(uiElapsedTime) / 86400.0;
				}
			}
			
			poSessionStats -> SettLastTransaction ( dTime );
			poSessionStats -> AddnPendingReqs ( -1 ) ;
			poSessionStats -> AddnProcessedReqs ( 1 ) ;

		}
		iSessionId = poSession->GetId () ;

		oMutexMap.Lock () ;

		Iterator = m_RequestSessionMap.find ( iSessionId) ;

		if ( Iterator != m_RequestSessionMap.end () )
		{	

			poRequestList = (*Iterator).second ;

			for ( i = 0 ; i<50 ; i++ )
			{
				if ( poRequestList [i] == ai_lIdRequest )
				{
					poRequestList [i] = 0 ;
					break;
				}
			}
		}

		oMutexMap.Unlock () ;
	}

	if ( poConnection )
	{
		-- (poConnection -> m_oNumRequest);  
	}


	if ( pReqStats )
	{
		pReqStats -> SettFinish ( );
	}

	if ( m_poStatistics )
	{
		((ClLMStats*)m_poStatistics ) -> Computer(poRequest) ;
	}

	poRequest->SendToBin();
	
	m4Trace(cout <<"-( " << --g_uiPDUSInside << " ) "  << endl ) ; //- Deleted in Sender process " << endl );
	
	return M4_SUCCESS ;
  //## end ClLocalMonitorSS::RemoveRequest%970561855.body
}

m4return_t ClLocalMonitorSS::RemoveBuffer (m4uint32_t ai_lConnectionId, m4objid_t ai_lPetitionId)
{
  //## begin ClLocalMonitorSS::RemoveBuffer%970572582.body preserve=yes
	ClXMLRequest * poXMLRequest = NULL ;



	if ( ! ( poXMLRequest = (ClXMLRequest *) m_oXMLRequestDic.GetItem ( ai_lPetitionId ) ) )
	{
		M4_SRV_ERROR ( M4_SRV_LM_XML_DESREG_JET , "Communication SS: Error getting XML buffer %0:d form dictionary. XML sent notified twice or duplied id %0:d" , (m4uint32_t)ai_lPetitionId ) ;
		
		return M4_ERROR ;
	}

	delete poXMLRequest ;

	if( M4_ERROR == m_oXMLRequestDic.RemoveItem ( ai_lPetitionId ) ) 
	{
		M4_SRV_ERROR ( M4_SRV_LM_XML_DESREG_JET , "Communication SS: Error getting XML buffer %0:d form dictionary. XML sent notified twice or duplied id %0:d" , (m4uint32_t) ai_lPetitionId) ;
		
		return M4_ERROR ;
	}


	return M4_SUCCESS;
  //## end ClLocalMonitorSS::RemoveBuffer%970572582.body
}

m4return_t ClLocalMonitorSS::AddCommWorker ()
{
  //## begin ClLocalMonitorSS::AddCommWorker%971889048.body preserve=yes
	m4return_t iRet ;
	m4double_t daux ;
	m4uint32_t iNumCommWorkers ;



	if ( ! m_poCommunication )
	{
		return M4_ERROR ;
	}

	if ( M4_SUCCESS  == GetConfigNumericValue ( SRV_CONFIG_NUM_COMM_WORKERS , daux ) )
	{
		iNumCommWorkers = daux ;
	}

	if ( SRV_CONFIG_MAX_NUM_WORKERS <= iNumCommWorkers )
	{
		M4_SRV_ERROR (  M4_SRV_COMMAND_MAX_INSTANCES , "Max number of instances of %0:s in Subsystem %1:s reached. Executing command %2:s" , "WORKER" << "COMMUNICATION_SUBSYSTEM" << "ADD_COMMUNICATION_WORKER" ) ;

		return M4_ERROR ;
	}

	if ( M4_ERROR == ( iRet = m_poCommunication -> AddWorker () ) )
	{
		M4_SRV_ERROR_N (  M4_SRV_LM_ADDCOMWORK_ERROR , "Communication SS: Internal error in Communication Layer creating new communication worker" ) ;
	}

	SetConfigNumericProperty ( SRV_CONFIG_NUM_COMM_WORKERS , ++iNumCommWorkers ) ;

	return iRet ;
  //## end ClLocalMonitorSS::AddCommWorker%971889048.body
}

m4return_t ClLocalMonitorSS::ProcessPDU (ClPDU *ai_poPDU, ClConnInstance *ai_poConnection, ClParamList* ai_poParamList)
{
  //## begin ClLocalMonitorSS::ProcessPDU%975318793.body preserve=yes
//	ClConnInstance* poConnection ;
	ClParamList * poParamList;
	ClRequest * poRequest ;
	ClSSInvoker * poInvoker = ClSSInvoker::GetInstance () ;
	m4millisec_t iCreatingTime = ClPrecisionTicker::GetTimeTick() ;
	m4objid_t iConnectionID;
	m4return_t iret ;


	if ( m_poStatistics )
	{
		if ( ai_poPDU )
		{
			((ClLMStats*)m_poStatistics ) -> AddInputBytes ( ai_poPDU -> GetSize () ) ;
		}

	}

	if (! ai_poConnection )
	{
		return M4_ERROR ;
	}

	iConnectionID = ai_poConnection -> GetId () ;
/*
	if ( ! ( poConnection = (ClConnInstance*)( (ClLocalMonitor *) m_poLocalMonitor ) -> m_oConDictionary.GetItem ( ai_iConnectionID )  ) )
	{
		poConnection = (ClConnInstance*)( (ClLocalMonitor *) m_poLocalMonitor ) -> m_oClosedConnection.GetItem ( ai_iConnectionID )  ;

		M4_ASSERT ( poConnection != NULL ) ;
	}
*/				
	poInvoker->GetParamList ( poParamList ) ;
				
	
	m4millisec_t dStartTime = 0, dReadedTime = 0 , dCommTime = 0;
	if ( M4_ERROR == ai_poParamList ->GetInt64(M4_PARAM_START_READING_TIME ,dStartTime) )
	{
		M4_SRV_ERROR (  M4_SRV_LM_COMM_PROCESS_PARAM_ERROR , "Communication SS: Error processing PDU. Param %0:s not found" , M4_PARAM_START_READING_TIME ) ;
	}
	if( M4_ERROR == ai_poParamList ->GetInt64(M4_PARAM_FINISH_READING_TIME   ,dReadedTime) )
	{
		M4_SRV_ERROR (  M4_SRV_LM_COMM_PROCESS_PARAM_ERROR , "Communication SS: Error processing PDU. Param %0:s not found" , M4_PARAM_FINISH_READING_TIME  ) ;
	}
	if( M4_ERROR == ai_poParamList ->GetInt64(M4_PARAM_COMM_EXECUTION_TIME     ,dCommTime) )
	{
		M4_SRV_ERROR (  M4_SRV_LM_COMM_PROCESS_PARAM_ERROR , "Communication SS: Error processing PDU. Param %0:s not found" , M4_PARAM_COMM_QUEUED_READED_TIME ) ;
	}
    
	if ( M4_ERROR == BuildRequestFromPDU ( ai_poConnection, ai_poPDU, poRequest , dStartTime ) )
	{
		//En principio solo da error si la pdu es nula, por tanto error de protocolo
		poParamList -> SetInt ( M4_PARAM_CONNECTIONID , iConnectionID) ;
		poInvoker -> ExecuteAction ( REMOVE_CONNECTION_ACTION , poParamList ) ;
//		M4_SRV_ERROR_N (  M4_SRV_LM_COMM_PROCESS_PARAM_ERROR , "Communication SS: Error processing PDU. Param %0:s not found" ) ;
		return M4_ERROR ;
	}
				
				

	((ClRequestStatistics*) poRequest -> GetStats () )->Sett ( ClRequestStatistics::InCommSubsystem, dReadedTime);
	((ClRequestStatistics*) poRequest -> GetStats () )->Sett ( ClRequestStatistics::InSSQueueIn, dCommTime);
	((ClRequestStatistics*) poRequest -> GetStats () )->Sett ( ClRequestStatistics::Creating, iCreatingTime);		
	((ClRequestStatistics*) poRequest -> GetStats () )->Sett ( ClRequestStatistics::GettingSession);

	/* Bug 0113962
	Se vuelca el tamaño de la pdu
	*/
	m4uint32_t iSize = ai_poPDU->GetDeltaSize() ;

	if( iSize == 0 )
	{
		// Si no es de tipo delta se toma el tamaño normal
		iSize = ai_poPDU->GetSize() ;
	}

	((ClRequestStatistics*) poRequest -> GetStats () )->SetInputSize ( iSize );

	((ClRequestStatistics*) poRequest -> GetStats () )->SetUserInfo ( ai_poPDU->GetUserInfo() );
				
	poParamList -> SetPointer ( "REQUEST" , poRequest ) ;
	poParamList -> SetDouble (M4_PARAM_CONNECTIONID, (m4int64_t) iConnectionID) ;
				
				
	if ( M4_SUCCESS > (iret = poInvoker -> ExecuteAction ( DISPATCH_REQUEST_ACTION , poParamList ) ) )
	{

		poRequest -> SetError (iret) ;

		poInvoker -> ExecuteAction ( SEND_REQUEST_ACTION , poParamList )  ;

		return M4_ERROR ;
	}

	poInvoker->SetParamList ( *poParamList ) ;

	return M4_SUCCESS ;
  //## end ClLocalMonitorSS::ProcessPDU%975318793.body
}

m4return_t ClLocalMonitorSS::Stop (m4uint32_t ai_lDeferedTime)
{
  //## begin ClLocalMonitorSS::Stop%977242948.body preserve=yes
	ClActionWorkerList::iterator Iterator;
	m4return_t iRet = M4_SUCCESS ;
	static m4bool_t bCommStopedAccepting = M4_FALSE ;
	static m4bool_t bCommStopedListening = M4_FALSE ;

	if (! bCommStopedAccepting )
	{
		if ( m_poCommunication )
		{
			m_poCommunication -> StopAccepting ( m_iBasePort ) ;
			m_poCommunication -> StopAccepting ( m_iBasePort + 1) ;
			m_poCommunication -> StopAccepting ( m_iBasePort + 2) ;
		}
		bCommStopedAccepting = M4_TRUE ;
	}

	if ( ! bCommStopedListening ) 
	{
		if ( ai_lDeferedTime <= 120 )
		{
			StopReading ( ) ;
			bCommStopedListening = M4_TRUE ;
		}
	}

	if ( ! ai_lDeferedTime )
	{
		for ( Iterator = m_oWorkerList.begin() ; Iterator != m_oWorkerList.end() ; Iterator ++ )
		{
			(*Iterator) -> Terminate () ;
		}

		for ( Iterator = m_oWorkerList.begin() ; Iterator != m_oWorkerList.end() ; Iterator ++ )
		{
			while ( (*Iterator) -> IsRunning () )
			{
				m_oActionsQ.Push ( (ClBaseAction*) NULL ) ;
				m4sleep (1) ;
			}
		}

		if ( m_poCommunication )
		{
			iRet = m_poCommunication -> StartShutdown () ;
		}

	}

	return iRet ;
  //## end ClLocalMonitorSS::Stop%977242948.body
}

m4bool_t ClLocalMonitorSS::IsReadyToClose ()
{
  //## begin ClLocalMonitorSS::IsReadyToClose%977242949.body preserve=yes
	ClActionWorkerList::iterator Iterator;

	
	for ( Iterator = m_oWorkerList.begin() ; Iterator != m_oWorkerList.end() ; Iterator ++ )
	{
		if ( (*Iterator) -> IsRunning () )
		{
			m_oActionsQ.Push ( (ClBaseAction*) NULL ) ;
			return M4_FALSE ;
		}
	}

	if ( ! ( m_poCommunication -> IsShuttedDown () ) )
	{
		return M4_FALSE ;
	}

	return M4_TRUE ;
  //## end ClLocalMonitorSS::IsReadyToClose%977242949.body
}

m4return_t ClLocalMonitorSS::StopReading ()
{
  //## begin ClLocalMonitorSS::StopReading%977301111.body preserve=yes
	IdList oIdList ;
	m4objid_t lId ;

	if( M4_ERROR == ((ClLocalMonitor*)m_poLocalMonitor ) -> m_oConDictionary.GetObjectList ( oIdList ) )
	{
		return M4_ERROR ;
	}

	while ( oIdList.size () > 0 )
	{
		lId = oIdList.back () ;

		m_poCommunication -> CloseConnection (  lId ) ;

		oIdList.pop_back () ;
	}

	return M4_SUCCESS ;

  //## end ClLocalMonitorSS::StopReading%977301111.body
}

m4return_t ClLocalMonitorSS::RemoveConnection (m4uint32_t ai_idConnection)
{
  //## begin ClLocalMonitorSS::RemoveConnection%993726127.body preserve=yes
	ClConnInstance * poConnection ;
	ClParamList * pParamList ;
	ClSSInvoker * poInvoker = ClSSInvoker::GetInstance () ;
	m4return_t iRet ;

	if ( ! m_poCommunication )
	{
		return M4_ERROR ;
	}


	if ( ! ( poConnection = (ClConnInstance*)( (ClLocalMonitor *) m_poLocalMonitor ) -> m_oConDictionary.GetItem ( ai_idConnection ) ) )
	{
		M4_SRV_ERROR( M4_SRV_LM_CONN_NOT_FOUND, "Communication SS: Error Getting connection number %0:d from connections dictionary. Connection clossed jet" ,  ai_idConnection ) ;
		M4_ASSERT ( "Connection Not Found" ) ;
		return M4_ERROR ;
	}

	if ( M4_ERROR == ( (ClLocalMonitor *) m_poLocalMonitor ) -> m_oConDictionary.RemoveItem ( ai_idConnection ) )
	{
		M4_SRV_ERROR( M4_SRV_LM_CONN_NOT_FOUND, "Communication SS: Error Getting connection number %0:d from connections dictionary. Connection clossed jet" ,  ai_idConnection ) ;
		M4_ASSERT ( "connection Not Found") ;
		return M4_ERROR ;
	}

	((ClLMStats*)m_poStatistics ) -> AddNewConnections ( -1 ) ;

	if ( M4_ERROR == ( iRet =( (ClLocalMonitor *) m_poLocalMonitor ) -> m_oClosedConnection.AddItem ( ai_idConnection , poConnection ) ) )
	{
		M4_ASSERT ( "La connexion ya se ha borrado") ;
		M4_SRV_ERROR( M4_SRV_LM_CONN_CLOSED_JET, "Communication SS: Connection number %0:d closed jet. Or two connections with same identifier" ,  ai_idConnection ) ;
	}

	poInvoker->GetParamList ( pParamList ) ;

	pParamList -> SetInt ( M4_PARAM_CONNECTIONID , (int) ai_idConnection  ) ;

	if ( M4_SUCCESS > poInvoker -> ExecuteAction ( CLOSE_CONNECTION_ACTION , pParamList ) ) 
	{
		CloseConnection ( ai_idConnection ) ;
	}

	return iRet ;

  //## end ClLocalMonitorSS::RemoveConnection%993726127.body
}

void ClLocalMonitorSS::REap ()
{
  //## begin ClLocalMonitorSS::REap%994350081.body preserve=yes
  //## end ClLocalMonitorSS::REap%994350081.body
}

m4return_t ClLocalMonitorSS::ReplaceRequest (ClRequest *ai_poOldRequest, ClRequest *ai_poNewRequest)
{
  //## begin ClLocalMonitorSS::ReplaceRequest%994350082.body preserve=yes

	if ( M4_ERROR == m_oRequestDic.RemoveItem( ai_poOldRequest -> GetId () )  )
	{
		M4_SRV_ERROR ( M4_SRV_LM_REQ_DESREG_JET , "Communication SS: Error getting request %0:d form request dictionary. Request sending notified twice or duplied id %0:d" , (m4int_t) ai_poOldRequest -> GetId () ) ;
//		M4_ASSERT ( "Esto quiere decir que hay un error de lógica en algún sitio" ) ;
		return M4_ERROR ;
	}

	if ( M4_ERROR == m_oRequestDic.AddItem( ai_poNewRequest -> GetId () , ai_poNewRequest )  )
	{
		M4_SRV_ERROR ( M4_SRV_LM_DUPPLIED_REQ_ID , "Error inserting request in the request dictionary. Another request with same ID allredy registered. ID %0:d" , (m4uint32_t) ai_poOldRequest -> GetId ()) ;
//		M4_ASSERT ( "Esto quiere decir que hay un error de lógica en algún sitio" ) ;
		return M4_ERROR ;
	}
	
	return M4_SUCCESS ;
  //## end ClLocalMonitorSS::ReplaceRequest%994350082.body
}


m4return_t ClLocalMonitorSS::RemoveCommWorker ( )
{
	m4return_t iRet ;
	m4uint32_t iNumCommWorkers ;
	m4double_t daux ;


	if ( ! m_poCommunication )
	{
		return M4_ERROR ;
	}

	if ( M4_SUCCESS  == GetConfigNumericValue ( SRV_CONFIG_NUM_COMM_WORKERS , daux ) )
	{
		iNumCommWorkers = daux ;
	}

	if ( SRV_CONFIG_MIN_NUM_WORKERS >= iNumCommWorkers )
	{
		M4_SRV_ERROR (  M4_SRV_COMMAND_MAX_INSTANCES , "Min number of instances of %0:s in Subsystem %1:s reached. Executing command %2:s" , "WORKER" << "COMMUNICATION_SUBSYSTEM" << "REMOVE_COMMUNICATION_WORKER" ) ;

		return M4_ERROR ;
	}

	if ( M4_ERROR == ( iRet = m_poCommunication -> RemoveWorker () ) )
	{
		M4_SRV_ERROR_N (  M4_SRV_LM_ADDCOMWORK_ERROR , "Communication SS: Internal error in Communication Layer removing communication worker" ) ;
	}

	SetConfigNumericProperty ( SRV_CONFIG_NUM_COMM_WORKERS , --iNumCommWorkers )  ;

	return iRet ;
}

// Class ClProcessRequestAction 

ClProcessRequestAction::ClProcessRequestAction (ClSSImplementation *ai_poExecutive)
  //## begin ClProcessRequestAction::ClProcessRequestAction%975318792.hasinit preserve=no
  //## end ClProcessRequestAction::ClProcessRequestAction%975318792.hasinit
  //## begin ClProcessRequestAction::ClProcessRequestAction%975318792.initialization preserve=yes
 :ClSSAction ( ai_poExecutive , M4_TRUE )
  //## end ClProcessRequestAction::ClProcessRequestAction%975318792.initialization
{
  //## begin ClProcessRequestAction::ClProcessRequestAction%975318792.body preserve=yes
  //## end ClProcessRequestAction::ClProcessRequestAction%975318792.body
}



//## Other Operations (implementation)
m4return_t ClProcessRequestAction::Execute ()
{
  //## begin ClProcessRequestAction::Execute%975318790.body preserve=yes
	ClPDU * poPDU ;
	ClConnInstance *  poConnection;

	if ( M4_ERROR == m_pParamList -> GetPointer ( "PDU" , (void*&)poPDU ) )
	{
		M4_SRV_ERROR (  M4_SRV_LM_COMM_PROCESS_PARAM_ERROR , "Communication SS: Error processing PDU. Param %0:s not found" , "PDU" ) ;
//		M4_SRV_ERROR_N (  M4_SRV_COMM_ERROR , "Process Request Action: Error Getting 'PDU' Param " ) ;
		return M4_ERROR ;
	}
	if ( M4_ERROR == m_pParamList -> GetPointer ( "CONNECTION" , (void*&) poConnection ) )
	{
		M4_SRV_ERROR (  M4_SRV_LM_COMM_PROCESS_PARAM_ERROR , "Communication SS: Error processing PDU. Param %0:s not found" , "CONNECTION" ) ;
//		M4_SRV_ERROR_N (  M4_SRV_COMM_ERROR , "Process Request Action: Error Getting 'CONNECTIONID' Param " ) ;
		return M4_ERROR ;
	}


	return ((ClLocalMonitorSS*)m_poExecutive )-> ProcessPDU ( poPDU , poConnection , m_pParamList ) ;
  //## end ClProcessRequestAction::Execute%975318790.body
}

// Additional Declarations
  //## begin ClProcessRequestAction%38F21D500218.declarations preserve=yes
  //## end ClProcessRequestAction%38F21D500218.declarations

// Class ClReturnRequestAction 

ClReturnRequestAction::ClReturnRequestAction (ClSSImplementation *ai_poExecutive)
  //## begin ClReturnRequestAction::ClReturnRequestAction%959932989.hasinit preserve=no
  //## end ClReturnRequestAction::ClReturnRequestAction%959932989.hasinit
  //## begin ClReturnRequestAction::ClReturnRequestAction%959932989.initialization preserve=yes
  :ClSSAction ( ai_poExecutive , M4_TRUE )
  //## end ClReturnRequestAction::ClReturnRequestAction%959932989.initialization
{
  //## begin ClReturnRequestAction::ClReturnRequestAction%959932989.body preserve=yes
  //## end ClReturnRequestAction::ClReturnRequestAction%959932989.body
}



//## Other Operations (implementation)
m4return_t ClReturnRequestAction::Execute ()
{
  //## begin ClReturnRequestAction::Execute%959932990.body preserve=yes
	ClBaseRequest * poRequest ;
	void * poItem ;



	if ( M4_ERROR == m_pParamList -> GetPointer ( "REQUEST" , poItem ) )
	{
		M4_SRV_ERROR (  M4_SRV_LM_BEFORE_WRITE_PARAM_ERROR , "Communication SS: Error writing PDU. Param %0:s not found" , "REQUEST" ) ;
		return M4_ERROR ;
	}

	if ( ! ( poRequest = (ClBaseRequest*)poItem ) ) 
	{
		M4_SRV_ERROR (  M4_SRV_LM_BEFORE_WRITE_PARAM_ERROR , "Communication SS: Error writing PDU. Param %0:s not found" , "REQUEST" ) ;
//		M4_SRV_ERROR (  M4_SRV_COMM_ERROR , "Return Request Action: Error Getting '%0:s' Param " , M4_PARAM_FINISH_READING_TIME ) ;
		return M4_ERROR ;
	}


	 return ((ClLocalMonitorSS * )  m_poExecutive ) -> SendRequest( (ClRequest*)poRequest ) ;
  //## end ClReturnRequestAction::Execute%959932990.body
}

// Additional Declarations
  //## begin ClReturnRequestAction%39376ABB0040.declarations preserve=yes
  //## end ClReturnRequestAction%39376ABB0040.declarations

// Class ClSendBufferAsClientAction 

ClSendBufferAsClientAction::ClSendBufferAsClientAction (ClSSImplementation *ai_poExecutive)
  //## begin ClSendBufferAsClientAction::ClSendBufferAsClientAction%962784429.hasinit preserve=no
  //## end ClSendBufferAsClientAction::ClSendBufferAsClientAction%962784429.hasinit
  //## begin ClSendBufferAsClientAction::ClSendBufferAsClientAction%962784429.initialization preserve=yes
  :ClSSAction ( ai_poExecutive )
  //## end ClSendBufferAsClientAction::ClSendBufferAsClientAction%962784429.initialization
{
  //## begin ClSendBufferAsClientAction::ClSendBufferAsClientAction%962784429.body preserve=yes
  //## end ClSendBufferAsClientAction::ClSendBufferAsClientAction%962784429.body
}



//## Other Operations (implementation)
m4return_t ClSendBufferAsClientAction::Execute ()
{
  //## begin ClSendBufferAsClientAction::Execute%962784430.body preserve=yes
	m4int_t iConnectionId ;
	m4string_t szHost;
	m4return_t iRet ;
	m4pchar_t szBuffer ;
	
	if ( M4_ERROR == m_pParamList ->GetPointer ( BUFFER , (void*&)szBuffer ) )
	{
		return M4_ERROR ;
	}

	if ( M4_ERROR == m_pParamList -> GetInt ( M4_PARAM_CONNECTIONID , iConnectionId) )
	{
		return M4_ERROR ;
	}

	if ( M4_ERROR == ( iRet = ( (ClLocalMonitorSS*)m_poExecutive ) -> SendBuffer (iConnectionId , szBuffer, strlen ( szBuffer) +1 ) ) )
	{
		return M4_ERROR ;
	}

	return iRet ;
  //## end ClSendBufferAsClientAction::Execute%962784430.body
}

// Additional Declarations
  //## begin ClSendBufferAsClientAction%3962E6B402A5.declarations preserve=yes
  //## end ClSendBufferAsClientAction%3962E6B402A5.declarations

// Class CloseConnectionAction 

CloseConnectionAction::CloseConnectionAction (ClSSImplementation *ai_poExecutive)
  //## begin CloseConnectionAction::CloseConnectionAction%962784434.hasinit preserve=no
  //## end CloseConnectionAction::CloseConnectionAction%962784434.hasinit
  //## begin CloseConnectionAction::CloseConnectionAction%962784434.initialization preserve=yes
  :ClSSAction ( ai_poExecutive , M4_TRUE )
  //## end CloseConnectionAction::CloseConnectionAction%962784434.initialization
{
  //## begin CloseConnectionAction::CloseConnectionAction%962784434.body preserve=yes
  //## end CloseConnectionAction::CloseConnectionAction%962784434.body
}



//## Other Operations (implementation)
m4return_t CloseConnectionAction::Execute ()
{
  //## begin CloseConnectionAction::Execute%962784435.body preserve=yes
	m4int_t iParam ;
	m4uint32_t iConnectionId ;


	if ( M4_ERROR == m_pParamList -> GetInt ( M4_PARAM_CONNECTIONID , iParam ) )
	{
		return M4_ERROR ;
	}

	iConnectionId = iParam ;

	return ( (ClLocalMonitorSS*)m_poExecutive ) -> CloseConnection (iConnectionId) ;
  //## end CloseConnectionAction::Execute%962784435.body
}

// Additional Declarations
  //## begin CloseConnectionAction%396313F60042.declarations preserve=yes
  //## end CloseConnectionAction%396313F60042.declarations

// Class ClOpenConnectionAction 

ClOpenConnectionAction::ClOpenConnectionAction (ClSSImplementation *ai_poExecutive)
  //## begin ClOpenConnectionAction::ClOpenConnectionAction%969373407.hasinit preserve=no
  //## end ClOpenConnectionAction::ClOpenConnectionAction%969373407.hasinit
  //## begin ClOpenConnectionAction::ClOpenConnectionAction%969373407.initialization preserve=yes
  :ClSSAction ( ai_poExecutive )
  //## end ClOpenConnectionAction::ClOpenConnectionAction%969373407.initialization
{
  //## begin ClOpenConnectionAction::ClOpenConnectionAction%969373407.body preserve=yes
  //## end ClOpenConnectionAction::ClOpenConnectionAction%969373407.body
}



//## Other Operations (implementation)
m4return_t ClOpenConnectionAction::Execute ()
{
  //## begin ClOpenConnectionAction::Execute%969373408.body preserve=yes
	m4return_t iRet ;
	m4objid_t iConnectionId ;
	m4int_t iPort ;
	m4pchar_t szHost;

	if ( M4_ERROR == m_pParamList -> GetString ( HOST , szHost ) )
	{
		return M4_ERROR ;
	}


	if ( M4_ERROR == m_pParamList -> GetInt ( PORT , iPort ) )
	{
		return M4_ERROR ;
	}

	if ( M4_SUCCESS == ( iRet = ( (ClLocalMonitorSS*)m_poExecutive ) -> OpenConnection (szHost, iPort, iConnectionId) ) )
	{
		m_pParamList -> SetDouble ( M4_PARAM_CONNECTIONID , (m4int64_t) iConnectionId ) ;
	}

	return iRet ;
  //## end ClOpenConnectionAction::Execute%969373408.body
}

// Additional Declarations
  //## begin ClOpenConnectionAction%39C73EF602DE.declarations preserve=yes
  //## end ClOpenConnectionAction%39C73EF602DE.declarations

// Class ClRegisterRequestAction 

ClRegisterRequestAction::ClRegisterRequestAction (ClSSImplementation *ai_poExecutive)
  //## begin ClRegisterRequestAction::ClRegisterRequestAction%955610147.hasinit preserve=no
  //## end ClRegisterRequestAction::ClRegisterRequestAction%955610147.hasinit
  //## begin ClRegisterRequestAction::ClRegisterRequestAction%955610147.initialization preserve=yes
  :ClSSAction ( ai_poExecutive )
  //## end ClRegisterRequestAction::ClRegisterRequestAction%955610147.initialization
{
  //## begin ClRegisterRequestAction::ClRegisterRequestAction%955610147.body preserve=yes
  //## end ClRegisterRequestAction::ClRegisterRequestAction%955610147.body
}



//## Other Operations (implementation)
m4return_t ClRegisterRequestAction::Execute ()
{
  //## begin ClRegisterRequestAction::Execute%955610148.body preserve=yes
	ClRequest * poRequest ;
	void * poItem ;
	m4objid_t idConnection = 0 ;


	if ( M4_ERROR == m_pParamList -> GetPointer ( "REQUEST" , poItem ) )
	{
		return M4_ERROR ;
	}

	if ( !  ( poRequest = (ClRequest*) poItem ) )
	{
		return M4_ERROR ;
	}


	return ((ClLocalMonitorSS*)m_poExecutive )-> RegisterRequest( poRequest ) ;
  //## end ClRegisterRequestAction::Execute%955610148.body
}

// Additional Declarations
  //## begin ClRegisterRequestAction%38F589BF01EF.declarations preserve=yes
  //## end ClRegisterRequestAction%38F589BF01EF.declarations

// Class ClDeRegisterRequestAction 

ClDeRegisterRequestAction::ClDeRegisterRequestAction (ClSSImplementation *ai_poExecutive)
  //## begin ClDeRegisterRequestAction::ClDeRegisterRequestAction%955610149.hasinit preserve=no
  //## end ClDeRegisterRequestAction::ClDeRegisterRequestAction%955610149.hasinit
  //## begin ClDeRegisterRequestAction::ClDeRegisterRequestAction%955610149.initialization preserve=yes
  :ClSSAction ( ai_poExecutive )
  //## end ClDeRegisterRequestAction::ClDeRegisterRequestAction%955610149.initialization
{
  //## begin ClDeRegisterRequestAction::ClDeRegisterRequestAction%955610149.body preserve=yes
  //## end ClDeRegisterRequestAction::ClDeRegisterRequestAction%955610149.body
}



//## Other Operations (implementation)
m4return_t ClDeRegisterRequestAction::Execute ()
{
  //## begin ClDeRegisterRequestAction::Execute%955610150.body preserve=yes
	ClRequest * poRequest ;
	ClConnInstance * poConnection ;
	void * poItem ;
	m4objid_t idConnection ,lIdRequest;

	if ( M4_ERROR == m_pParamList -> GetPointer ( "REQUEST" , poItem ) )
	{
		return M4_ERROR ;
	}

	poRequest = ( ClRequest* ) poItem ;

	if ( ! ( poConnection = poRequest -> GetpConnection () ) )
	{
		return M4_ERROR ;
	}

	lIdRequest = poRequest -> GetId ( ) ;

	lIdRequest &= 0x0000ffff ;
	idConnection = poConnection -> GetId () ;

	return ((ClLocalMonitorSS*)m_poExecutive )-> RemoveRequest( idConnection , lIdRequest , ClPrecisionTicker::GetTimeTick()) ;
  //## end ClDeRegisterRequestAction::Execute%955610150.body
}

// Additional Declarations
  //## begin ClDeRegisterRequestAction%38F589EC012B.declarations preserve=yes
  //## end ClDeRegisterRequestAction%38F589EC012B.declarations

// Class ClCancelUserRequestAction 

ClCancelUserRequestAction::ClCancelUserRequestAction (ClSSImplementation *ai_poExecutive)
  //## begin ClCancelUserRequestAction::ClCancelUserRequestAction%958560330.hasinit preserve=no
  //## end ClCancelUserRequestAction::ClCancelUserRequestAction%958560330.hasinit
  //## begin ClCancelUserRequestAction::ClCancelUserRequestAction%958560330.initialization preserve=yes
 :ClSSAction ( ai_poExecutive , M4_TRUE )
  //## end ClCancelUserRequestAction::ClCancelUserRequestAction%958560330.initialization
{
  //## begin ClCancelUserRequestAction::ClCancelUserRequestAction%958560330.body preserve=yes
  //## end ClCancelUserRequestAction::ClCancelUserRequestAction%958560330.body
}



//## Other Operations (implementation)
m4return_t ClCancelUserRequestAction::Execute ()
{
  //## begin ClCancelUserRequestAction::Execute%958560331.body preserve=yes
	m4objid_t lIdRequest = 0, lIdSession = 0;

	m_pParamList -> GetInt64( "REQUESTID" , (m4int64_t&) lIdRequest ) ;
	m_pParamList -> GetInt64( "SESSIONID" , (m4int64_t&) lIdSession ) ;


	return ((ClLocalMonitorSS*)m_poExecutive )-> CancelRequest ( lIdRequest , lIdSession ) ;
  //## end ClCancelUserRequestAction::Execute%958560331.body
}

// Additional Declarations
  //## begin ClCancelUserRequestAction%39227A250254.declarations preserve=yes
  //## end ClCancelUserRequestAction%39227A250254.declarations

// Class ClRemoveRequestAction 

ClRemoveRequestAction::ClRemoveRequestAction (ClSSImplementation *ai_poExecutive)
  //## begin ClRemoveRequestAction::ClRemoveRequestAction%989398207.hasinit preserve=no
  //## end ClRemoveRequestAction::ClRemoveRequestAction%989398207.hasinit
  //## begin ClRemoveRequestAction::ClRemoveRequestAction%989398207.initialization preserve=yes
 :ClSSAction ( ai_poExecutive , M4_TRUE )
  //## end ClRemoveRequestAction::ClRemoveRequestAction%989398207.initialization
{
  //## begin ClRemoveRequestAction::ClRemoveRequestAction%989398207.body preserve=yes
  //## end ClRemoveRequestAction::ClRemoveRequestAction%989398207.body
}



//## Other Operations (implementation)
m4return_t ClRemoveRequestAction::Execute ()
{
  //## begin ClRemoveRequestAction::Execute%989398208.body preserve=yes
	m4int_t iConnectionID ;
	m4objid_t iRequestId = 0  ;
	m4int64_t iParamint  ;
	m4millisec_t dWriteTime ;


	if(M4_ERROR==m_pParamList->GetInt(M4_PARAM_CONNECTIONID, iConnectionID))
	{
		M4_SRV_ERROR ( M4_SRV_LM_COMM_WRITE_PARAM_ERROR , "Communication SS: Error getting %0:s param from action after writting a PDU" , M4_PARAM_CONNECTIONID); 
		return M4_ERROR ;
	}

	if(M4_ERROR==m_pParamList->GetInt64 (M4_PARAM_PETITION_ID, iParamint ))
	{
		M4_SRV_ERROR ( M4_SRV_LM_COMM_WRITE_PARAM_ERROR , "Communication SS: Error getting %0:s param from action after writting a PDU" , M4_PARAM_PETITION_ID ); 
		return M4_ERROR ;
	}

	iRequestId = iParamint ;
 
	if(M4_ERROR==m_pParamList->GetInt64 (M4_PARAM_FINISH_WRITTING_TIME, dWriteTime ))
	{
		M4_SRV_WARNING ( M4_SRV_LM_COMM_WRITE_PARAM_ERROR , "Communication SS: Error getting %0:s param from action after writting a PDU" , M4_PARAM_FINISH_WRITTING_TIME ); 
		dWriteTime = 0 ;
	}


	return ((ClLocalMonitorSS*)m_poExecutive )-> RemoveRequest ( iConnectionID , iRequestId , dWriteTime  ) ;
  //## end ClRemoveRequestAction::Execute%989398208.body
}

// Additional Declarations
  //## begin ClRemoveRequestAction%3AF904540106.declarations preserve=yes
  //## end ClRemoveRequestAction%3AF904540106.declarations

// Class ClRemoveConnectionAction 

ClRemoveConnectionAction::ClRemoveConnectionAction (ClSSImplementation *ai_poExecutive)
  //## begin ClRemoveConnectionAction::ClRemoveConnectionAction%993726128.hasinit preserve=no
  //## end ClRemoveConnectionAction::ClRemoveConnectionAction%993726128.hasinit
  //## begin ClRemoveConnectionAction::ClRemoveConnectionAction%993726128.initialization preserve=yes
 :ClSSAction ( ai_poExecutive , M4_TRUE)
  //## end ClRemoveConnectionAction::ClRemoveConnectionAction%993726128.initialization
{
  //## begin ClRemoveConnectionAction::ClRemoveConnectionAction%993726128.body preserve=yes
  //## end ClRemoveConnectionAction::ClRemoveConnectionAction%993726128.body
}



//## Other Operations (implementation)
m4return_t ClRemoveConnectionAction::Execute ()
{
  //## begin ClRemoveConnectionAction::Execute%993726129.body preserve=yes
	m4int_t iParam ;
	m4uint32_t iConnectionId ;


	if ( M4_ERROR == m_pParamList -> GetInt ( M4_PARAM_CONNECTIONID , iParam ) )
	{
		return M4_ERROR ;
	}

	iConnectionId = iParam ;

	return ( (ClLocalMonitorSS*)m_poExecutive ) -> RemoveConnection (iConnectionId) ;
  //## end ClRemoveConnectionAction::Execute%993726129.body
}

// Additional Declarations
  //## begin ClRemoveConnectionAction%3B3B0E59022C.declarations preserve=yes
  //## end ClRemoveConnectionAction%3B3B0E59022C.declarations

// Class ClReplaceRequest 

ClReplaceRequest::ClReplaceRequest (ClSSImplementation *ai_poExecutive)
  //## begin ClReplaceRequest::ClReplaceRequest%994350079.hasinit preserve=no
  //## end ClReplaceRequest::ClReplaceRequest%994350079.hasinit
  //## begin ClReplaceRequest::ClReplaceRequest%994350079.initialization preserve=yes
 :ClSSAction ( ai_poExecutive )
  //## end ClReplaceRequest::ClReplaceRequest%994350079.initialization
{
  //## begin ClReplaceRequest::ClReplaceRequest%994350079.body preserve=yes
  //## end ClReplaceRequest::ClReplaceRequest%994350079.body
}



//## Other Operations (implementation)
m4return_t ClReplaceRequest::Execute ()
{
  //## begin ClReplaceRequest::Execute%994350080.body preserve=yes
	ClRequest * poOldRequest , *poNewRequest ;


	if ( M4_ERROR == m_pParamList -> GetPointer ( "OLD_REQUEST" , (void*&)poOldRequest) )
	{
		return M4_ERROR ;
	}

	if ( M4_ERROR == m_pParamList -> GetPointer ( "NEW_REQUEST" , (void*&)poNewRequest) )
	{
		return M4_ERROR ;
	}

	return ( (ClLocalMonitorSS*)m_poExecutive ) -> ReplaceRequest (poOldRequest , poNewRequest) ;
	
  //## end ClReplaceRequest::Execute%994350080.body
}

// Additional Declarations
  //## begin ClReplaceRequest%3B4493480089.declarations preserve=yes
  //## end ClReplaceRequest%3B4493480089.declarations

//## begin module%39376A8302AD.epilog preserve=yes
//## end module%39376A8302AD.epilog
