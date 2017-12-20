//## begin module%396B5C6A010E.cm preserve=no
//## end module%396B5C6A010E.cm

//## begin module%396B5C6A010E.cp preserve=no
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
//## end module%396B5C6A010E.cp

//## Module: adminagent%396B5C6A010E; Package body
//## Subsystem: M4Dispatcher::src%3923BBFB0332
//## Source file: F:\integration\m4dispatcher\src\adminagent.cpp

//## begin module%396B5C6A010E.additionalIncludes preserve=no
//## end module%396B5C6A010E.additionalIncludes

//## begin module%396B5C6A010E.includes preserve=yes
#pragma warning (disable : 4284)

#include <executiveagent.hpp>
#include <dspinvoker.hpp>
#include <dspparams.hpp>
#include <m4xmlstreamgen.hpp>
#include <actionidentifiers.hpp>
#include <dspactions.hpp>
#include <adminactions.hpp>
#include <saxactioncaller.hpp>
#include <basiclog.hpp>
#include <m4dpchres.hpp>
#include <clarraylibres.hpp>
#include <dspxmlreq.hpp>
#include <fileutil.hpp>
#include <dspappctlreq.hpp>

#ifdef _WINDOWS
#include <direct.h>
#else
#include <unistd.h>
#endif

#include <m4memdbg.h>
#include <m4srvtrace.h>

#define M4_SRV_TRACE_COMP_ID "DPCH"

#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
//## end module%396B5C6A010E.includes

// cltimer
#include <cltimer.hpp>
// dspagent
#include <dspagent.hpp>
// adminagent
#include <adminagent.hpp>
// m4condition
#include <m4condition.hpp>
// appctlnodeinfo
#include <appctlnodeinfo.hpp>
#include "m4unicode.hpp"

//## begin module%396B5C6A010E.declarations preserve=no
//## end module%396B5C6A010E.declarations

//## begin module%396B5C6A010E.additionalDeclarations preserve=yes

m4uint32_t g_iDspTrace = M4_FALSE; 

#ifdef _WINDOWS
#define TARGETINIFILE	"\\..\\conf\\m4dsptarget.xml"
#else
#define TARGETINIFILE	"../conf/m4dsptarget.xml"
#endif

#define TARGETAPPCTLFILE "../conf/m4dspini.xml"
#define ADMINHOSTSFILE "../conf/m4dspadmhosts.ini"
#define DSP_RELATIVE_CONFIG_DIR "../conf/"
#define DSP_PASS_FILE_NAME "m4dspsec.dat"


//## end module%396B5C6A010E.additionalDeclarations


// Class ClAdminAgent 








ClAdminAgent::ClAdminAgent (ClSiteNodeRepository *ai_poServerRepository, ClDspExecutiveAgent *ai_poExecutiveAgent)
  //## begin ClAdminAgent::ClAdminAgent%960967021.hasinit preserve=no
      : m_bStarting(M4_TRUE)
  //## end ClAdminAgent::ClAdminAgent%960967021.hasinit
  //## begin ClAdminAgent::ClAdminAgent%960967021.initialization preserve=yes
  ,ClDspAgent ( ai_poServerRepository , ai_poExecutiveAgent)
  //## end ClAdminAgent::ClAdminAgent%960967021.initialization
{
  //## begin ClAdminAgent::ClAdminAgent%960967021.body preserve=yes
	const char * szDspTrace = getenv("M4_DSP_TRACE");
	g_iDspTrace = szDspTrace == NULL  ? 0 : atoi ( szDspTrace ) ;

	m_poTimer=NULL ;

	m_bHasAuthentication = M4_FALSE;

	m_bIsInitializing = M4_TRUE;

	m_pcDspConfigDirPath = GetDspConfigDirPath();

	LoadPasswordFromFile();

  //## end ClAdminAgent::ClAdminAgent%960967021.body
}


ClAdminAgent::~ClAdminAgent ()
{
  //## begin ClAdminAgent::~ClAdminAgent%960967022.body preserve=yes
	if (m_pcDspConfigDirPath != NULL) 
	{
		delete [] m_pcDspConfigDirPath;
	}

	if (m_pcAuthPass != NULL) 
	{
		delete [] m_pcAuthPass;
	}

  //## end ClAdminAgent::~ClAdminAgent%960967022.body
}



//## Other Operations (implementation)
m4return_t ClAdminAgent::ProcessAction (ClActionIdentifier ai_oActionIdentifier, ClParamList *ai_poParamList, ClBaseAction *&ao_poAction)
{
  //## begin ClAdminAgent::ProcessAction%960967038.body preserve=yes

    ClBaseAction * poAction = NULL ;

	switch ( ai_oActionIdentifier )
	{
	case PARSE_XML_ID :
		M4_MDBG_NEW(  (ClBaseAction *)poAction, poAction = new ClParseXMLAction (this ) ) ;
			break ;

	case CONFIGURATIONS_QUERY_ACTION_ID :
		M4_MDBG_NEW(  (ClBaseAction *)poAction, poAction = new ClConfigurationsQueryAction ( this ) ) ;
			break ;
	case START_INITIAL_CONFIG_ACTION_ID :
	case START_SERVER_QUERY_ACTION_ID :
		M4_MDBG_NEW(  (ClBaseAction *)poAction, poAction = new ClStartConfigAction ( this ) ) ;
		break ;
	case REGISTER_APPCTL_ACTION_ID :
		return AddAppctlInfo (ai_poParamList) ;
	case STOP_SERVER_QUERY_ACTION_ID :
		M4_MDBG_NEW(  (ClBaseAction *)poAction,  poAction = new ClStopConfigAction ( this ) ) ;
		break ;
	case CHECK_CONSISTENCY_ACTION_ID :
		CheckConsistency() ;
		return M4_SUCCESS ;

	case STOP_DPCH_ACTION_ID :
		{
		M4_MDBG_NEW(  (ClBaseAction *)poAction, poAction = new ClStopDpchAction ( this ) ) ;
		}
		break ;
	case SIGNAL_DPCH_ID :
		{
		M4_MDBG_NEW(  (ClBaseAction *)poAction, poAction = new ClSingalAction ( this ) ) ;
		}
		break ;
	case REGISTER_TARGET_CONFIG_ID :
		{
		M4_MDBG_NEW(  (ClBaseAction *)poAction, poAction = new ClRegisterTargetAction ( this ) ) ;
		}
		break ;
	case UPDATE_APPCTL_CONFIG_ID :
		{
		M4_MDBG_NEW(  (ClBaseAction *)poAction, poAction = new ClUpdateAppCtlConfigAction ( this ) ) ;
		}
		break ;

//		M4_MDBG_NEW(  (ClBaseAction *)poAction,  poAction = new ClCheckConsistencyAction ( this ) ) ;
//			return poAction -> Execute () ;

	case REMOVE_SERVER_QUERY_ACTION_ID :
		{
		M4_MDBG_NEW(  (ClBaseAction *)poAction,  poAction = new ClRemoveConfigAction ( this ) ) ;
		}
		break ;
	
	case CLOSE_CONNECTION_ADMIN_ID :
		{
		M4_MDBG_NEW(  (ClBaseAction *)poAction,  poAction = new ClCloseConnectionAdminAction ( this ) ) ;
		}
		break ;

	default :

		// M4_ERR_DPCH_UNKNOWNADMINAGENTACTION = "Error proccessing action of the Agent for Administration Management.\nUnknown Action Identifier "%0:s".";	
		M4_SRV_ERROR(M4_ERR_DPCH_UNKNOWNADMINAGENTACTION, 
			"Error proccessing action of the Agent for Administration Management. Unknown Action Identifier %0:s.",
			ai_oActionIdentifier);			

		return M4_ERROR ;
	}


	poAction -> SetParams ( ai_poParamList ) ;

	m_poExecutiveAgent ->PushAction ( poAction ) ;


	return M4_SUCCESS ;
  //## end ClAdminAgent::ProcessAction%960967038.body
}

m4return_t ClAdminAgent::Start ()
{
  //## begin ClAdminAgent::Start%960967039.body preserve=yes
	ClDispatcherInvoker * poInvoker = ClDispatcherInvoker::Instance () ;
	m4char_t *Aux=NULL ;

    try
    {
         XMLPlatformUtils::Initialize();
    }

    catch (const XMLException& /*toCatch*/)
    {
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOINITIALIZEADMINAGENTXMLPARSE,
			"Error initializing Admin. Agent. Cannot initialize the XML parse." );					

		return M4_ERROR;
    }


	Aux= NULL ;

	if ( M4_ERROR == poInvoker -> RegisterAction (  PARSE_XML , PARSE_XML_ID  , this ) )
	{
		Aux =  PARSE_XML ;					
	} else if ( M4_ERROR == poInvoker -> RegisterAction (  CONFIGURATIONS_QUERY ,  CONFIGURATIONS_QUERY_ACTION_ID , this ) ) {
		Aux = CONFIGURATIONS_QUERY ;
	} else if ( M4_ERROR == poInvoker -> RegisterAction ( START_SERVER , START_SERVER_QUERY_ACTION_ID , this ) ) {
		Aux = START_SERVER ;
	} else if ( M4_ERROR == poInvoker -> RegisterAction ( STOP_SERVER , STOP_SERVER_QUERY_ACTION_ID , this ) ) {
		Aux = STOP_SERVER;
	} else if ( M4_ERROR == poInvoker -> RegisterAction ( CHECK_CONSISTENCY , CHECK_CONSISTENCY_ACTION_ID, this ) ) {
		Aux = CHECK_CONSISTENCY ;
	} else if ( M4_ERROR == poInvoker -> RegisterAction ( START_INI_CONFIG, START_INITIAL_CONFIG_ACTION_ID , this ) ) {
		Aux = START_INI_CONFIG;
	} else if ( M4_ERROR == poInvoker -> RegisterAction ( REGISTER_APPCTL, REGISTER_APPCTL_ACTION_ID , this ) ) {
		Aux = REGISTER_APPCTL ;
	} else if ( M4_ERROR == poInvoker -> RegisterAction ( STOP_DPCH, STOP_DPCH_ACTION_ID , this ) ) {
		Aux = STOP_DPCH ;
	} else if ( M4_ERROR == poInvoker -> RegisterAction ( SIGNAL_DPCH , SIGNAL_DPCH_ID , this ) ) {
		Aux = SIGNAL_DPCH ;
	} else if ( M4_ERROR == poInvoker -> RegisterAction ( TARGET_CONFIGURATION, REGISTER_TARGET_CONFIG_ID  , this ) ) {
		Aux = TARGET_CONFIGURATION ;
	} else if ( M4_ERROR == poInvoker -> RegisterAction ( UPDATE_APPCTL_CONFIG, UPDATE_APPCTL_CONFIG_ID  , this ) ) {
		Aux = UPDATE_APPCTL_CONFIG ;
	} else if ( M4_ERROR == poInvoker -> RegisterAction ( REMOVE_SERVER , REMOVE_SERVER_QUERY_ACTION_ID , this ) ) {
		Aux = REMOVE_SERVER;
	} else if ( M4_ERROR == poInvoker -> RegisterAction ( CLOSE_CONNECTION_ADMIN , CLOSE_CONNECTION_ADMIN_ID , this ) ) {
		Aux = CLOSE_CONNECTION_ADMIN;
	}

	if ( Aux ) {
		M4_SRV_ERROR(M4_ERR_DPCH_NOINITIALIZEAGENTACTION, 
			"Error starting Agent for %0:s. Action %1:s not registered.",	
			"Administration Management" << Aux );					
		return M4_ERROR ;
	} ;

//-----------Inicialización del Timer--------------------
	eActionNameList * vActionList = NULL ;

	M4_MDBG_NEW( vActionList, vActionList = new eActionNameList ) ;

	vActionList -> push_back (  CHECK_CONSISTENCY  ) ;
	vActionList -> push_back (  GARBAGE_COLLECTION  ) ;

	M4_MDBG_NEW( m_poTimer, m_poTimer = new ClTimer ( poInvoker , SERVER_UPDATE_SLAPSED_TIME /4  , vActionList ) ) ;

	return m_poTimer -> Start ("Dsp Timer") ;

//	return ClDptcherCommsAdaptor::Start () ;
  //## end ClAdminAgent::Start%960967039.body
}

m4return_t ClAdminAgent::GetConfigurationList (ClSiteNodeContainer &ao_poConfigurationList)
{
  //## begin ClAdminAgent::GetConfigurationList%961054359.body preserve=yes
	return m_poServerRepository -> GetNodeList ( ao_poConfigurationList ) ;
  //## end ClAdminAgent::GetConfigurationList%961054359.body
}

m4return_t ClAdminAgent::StartConfiguration (ClParamList *ai_poParamList, ClSiteNodeInformation &ai_oConfigurationInfo)
{
  //## begin ClAdminAgent::StartConfiguration%962122458.body preserve=yes
	m4uint32_t iBasePort = 0 ;
	m4uint64_t xmlid ;
	m4return_t iRet = M4_SUCCESS ;
	m4string_t user , password  , strhost , configuration;
	ClDispatcherInvoker * poInvoker = ClDispatcherInvoker::Instance () ;
	ClDspAppCtlRequest * poACRequest ;
	ClDspRequest * poHARequest = NULL ;
	ClSiteNodeContainer oNodeContainer ;
	ClLogSystem * poLog = NULL ;


	
	xmlid = ClIdGenerator::GetNewId () ;


	m4bool_t bFileOut = g_iDspTrace > 0 ? M4_TRUE : M4_FALSE ;
	

	password = ai_oConfigurationInfo.GetPassword ( ) ;

	user = ai_oConfigurationInfo.GetUserName () ;

	ClCommandCtlGen oXMLGen ( "server_startup" , xmlid , user , password ) ;

	if (M4_ERROR == ai_poParamList -> GetPointer ( "REQUEST" , (void *&)poHARequest ) )
	{
//Si no hay request de administración es que es el fail over el que levanta la config
/*		M4_SRV_ERROR(M4_ERR_DPCH_NOADMINAGENTSTARTCONFIGPARAM, 
			"Error starting Server Configuration. Cannot get the attribute %1:s.", 
			"REQUEST");			

		return M4_ERROR ;
*/
	}

	configuration = ai_oConfigurationInfo.GetSiteNodeID() ;

	strhost = ai_oConfigurationInfo.GetNodeHost ( ) ;

	ClAppctlNodeInfo oAppctlInfo ( strhost , 0 ) ;

	if ( M4_ERROR == m_oAppctlRepository.GetNodeInfo( oAppctlInfo ) )
	{
		M4_SRV_ERROR ( M4_ERR_DPCH_NOAPPCTLINHOST , 
			"Error starting configuration '%0:s' on host '%1:s'. No M4 Application Controler registered on that host." , configuration.c_str() << oAppctlInfo.GetSiteNodeHost().c_str() ) ;
		return M4_ERROR ;
	}

	ai_oConfigurationInfo.Start () ;

	oNodeContainer.AddNode ( ai_oConfigurationInfo ) ;


	if( poHARequest )
	{
		poLog = poHARequest ->GetLog () ;
	}

	M4_MDBG_NEW(  (ClDspRequest *)poACRequest, poACRequest = new ClDspAppCtlRequest ( xmlid , poLog ) ) ; 

	if ( M4_ERROR == poACRequest->BuildReqReply (oNodeContainer, &oXMLGen ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOADMINAGENTSTARTXMLANSWER,
			"Error starting Server Configuration. Cannot generate XML answer." );			
		return M4_ERROR ;
	}


	ClParamList * poParamList = NULL ;

	ai_oConfigurationInfo.SetNodeState ( STATE_STARTING ) ;

	M4_MDBG_NEW( poParamList, poParamList = new ClParamList ) ;

	// fix bug 0088880
	// works now with controller definitions
	poParamList -> SetString ( "HOST" , oAppctlInfo.GetSiteNodeHost().c_str() ) ;
	poParamList -> SetInt ( "PORT" , oAppctlInfo.GetAppctlPort() ) ;
	poParamList -> SetPointer ( "REQUEST" , poACRequest ) ;

	if ( M4_ERROR == m_poServerRepository  -> AddNode ( ai_oConfigurationInfo  ) )
	{
		return M4_ERROR ;
	}

	if ( M4_ERROR == poInvoker -> ExecuteAction ( SEND_XML_TO_APPCTL , poParamList) )
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOADMINAGENTSTARTEXECUTEACTION, 
			"Error starting Server Configuration '%0:s'. Error executing action %1:s. Petition Id: %2:s",
			configuration.c_str() << SEND_XML_TO_APPCTL << (m4uint32_t) xmlid);			

		ai_oConfigurationInfo.SetNodeState ( STATE_HALTED ) ;

		m_poServerRepository  -> RemoveNode ( ai_oConfigurationInfo  ) ;

		iRet = M4_ERROR ;
	}

	PersistTarget () ;

	cout << "Starting configuration '" << configuration << "' in host '" << oAppctlInfo.GetSiteNodeHost() <<"'" << endl ;

	return iRet ;
  //## end ClAdminAgent::StartConfiguration%962122458.body
}

m4return_t ClAdminAgent::StopConfiguration (ClParamList *ai_poParamList, ClSiteNodeInformation &ai_oConfigurationInfo)
{
  //## begin ClAdminAgent::StopConfiguration%962122459.body preserve=yes
	//ClSiteNodeInformation * poNodeInfo ;
	m4uint32_t iBasePort = 0 ;
	m4uint64_t xmlid ;
	m4return_t iRet = M4_SUCCESS ;
	m4pchar_t strhost , configuration , user , password;
	m4double_t dLoad = 0;
	ClDspAppCtlRequest * poACRequest ;
	ClDspRequest * poHARequest ;
	ClDispatcherInvoker * poInvoker = ClDispatcherInvoker::Instance () ;



	m4bool_t bFileOut = g_iDspTrace > 0 ? M4_TRUE : M4_FALSE ;

	xmlid = ClIdGenerator::GetNewId () ;


	
	if (M4_ERROR == ai_poParamList -> GetPointer ( "REQUEST" , (void *&) poHARequest ) )
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOADMINAGENTSTOPCONFIGPARAM, 
			"Error stopping Server Configuration. Couln't get the attribute %0:s.",
			"REQUEST");			
		return M4_ERROR ;
	}

	if (M4_ERROR == ai_poParamList -> GetString ( "host" , strhost ) )
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOADMINAGENTSTOPCONFIGPARAM, 
			"Error stopping Server Configuration. Couln't get the attribute %0:s.",
			"Host Name");			
		return M4_ERROR ;
	}

	ClAppctlNodeInfo oAppctlInfo ( strhost , 0 ) ;

	if ( M4_ERROR == m_oAppctlRepository.GetNodeInfo( oAppctlInfo ) )
	{
		return M4_ERROR ;
	}	

	if ( 0>= oAppctlInfo.GetAppctlPort ( ) )
	{
		return M4_ERROR ;
	}
	
	if (M4_ERROR == ai_poParamList -> GetString ( "configuration" , configuration ) )
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOADMINAGENTSTOPCONFIGPARAM, 
			"Error stopping Server Configuration. Couln't get the attribute %0:s.",
			"Server Name");			
		return M4_ERROR ;
	}

	if (M4_ERROR == ai_poParamList -> GetString ( "user" , user ) )
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOADMINAGENTSTOPCONFIGPARAM, 
			"Error stopping Server Configuration. Couln't get the attribute %0:s.",
			"User Name");			
		return M4_ERROR ;
	}

	if (M4_ERROR == ai_poParamList -> GetString ( "password" , password ) )
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOADMINAGENTSTOPCONFIGPARAM, 
			"Error stopping Server Configuration. Couln't get the attribute %0:s.",
			"Password");			
		return M4_ERROR ;
	}

	ClCommandCtlGen oXMLGen ( "server_shutdown" , xmlid , user , password ) ;

/* Dato no disponible al hacer shutdown de un server
	if (M4_ERROR == ai_poParamList -> GetString ( "load" , load ) )
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOADMINAGENTSTOPCONFIGPARAM, 
			"Error stopping Server Configuration. Couln't get the attribute %0:s.",
			"Load");			
	}
	else
	{
		if ( 0 == sscanf(load , "%d" , &dLoad))
		{	
			M4_SRV_ERROR(M4_ERR_DPCH_ADMINAGENTINVALIDLOAD, 
				"Error stopping Server Configuration. Invalid load value.", 
				"Load");			
		}
	}
*/
	
	ClSiteNodeInformation oNodeInfo (configuration , oAppctlInfo.GetSiteNodeHost().c_str(), 0, user, password);


	if ( M4_ERROR == m_poServerRepository -> GetNodeInfo ( oNodeInfo ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOADMINAGENTNODEINFO,
		"Error stopping Server Configuration. Cannot get Server Information." );
		return M4_ERROR ;
	}
	
	// now, all nodes are persistables. Do not delete nodes from target file.
    /*
    oNodeInfo.SetPersistable ( M4_FALSE ) ;
    */

	iBasePort = oNodeInfo.GetBasePort ( ) ;

	ai_oConfigurationInfo = oNodeInfo ;
	
	ClSiteNodeContainer oNodeContainer ;

	oNodeContainer.AddNode ( ai_oConfigurationInfo  ) ;

	M4_MDBG_NEW(  (ClDspRequest *)poACRequest, poACRequest = new ClDspAppCtlRequest ( xmlid , poHARequest ->GetLog() ) ) ; 


	if ( M4_ERROR == poACRequest->BuildReqReply (oNodeContainer, &oXMLGen ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOADMINAGENTSTARTXMLANSWER,
			"Error stopping Server Configuration. Cannot generate XML answer." );			
		return M4_ERROR ;
	}


	ClParamList * poParamList = NULL ;

	M4_MDBG_NEW( poParamList, poParamList = new ClParamList ) ;

	// fix bug 0088880
	// works now with controller definitions
	poParamList -> SetString ( "HOST" , oAppctlInfo.GetSiteNodeHost().c_str()) ;
	poParamList -> SetInt ( "PORT" , oAppctlInfo.GetAppctlPort() ) ;
	poParamList -> SetPointer ( "REQUEST" , poACRequest ) ;


	if ( M4_ERROR == poInvoker -> ExecuteAction ( SEND_XML_TO_APPCTL , poParamList) )
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOADMINAGENTSTOPEXECUTEACTION, 
			"Error stopping Server Configuration '%0:s'. Error executing action %1:s. Petition Id: %2:s",
			configuration << SEND_XML_TO_APPCTL << (m4uint32_t) xmlid );			

		return M4_ERROR ;
	}


    oNodeInfo.SetIsEnabled(M4_FALSE);
	if ( M4_ERROR == m_poServerRepository -> UpdateNode ( oNodeInfo ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOADMINAGENTNODEINFO,
			"Error stopping Server Configuration. Cannot update Server Information.");
	}

	PersistTarget () ;

	cout << "Shutting down configuration '" << configuration << "' in host '" << oAppctlInfo.GetSiteNodeHost() << "'" << endl ;

	return M4_SUCCESS  ;
  //## end ClAdminAgent::StopConfiguration%962122459.body
}

m4return_t ClAdminAgent::RemoveConfiguration (ClParamList *ai_poParamList, ClSiteNodeInformation &ai_oConfigurationInfo)
{	
	m4return_t iRet = M4_SUCCESS ;
	m4pchar_t pcHost = NULL, pcConfig = NULL ;
	
	m4bool_t bFileOut = g_iDspTrace > 0 ? M4_TRUE : M4_FALSE ;

	if (M4_ERROR == ai_poParamList -> GetString ( "host" , pcHost ) )
	{
		M4_SRV_ERROR ( M4_ERR_DPCH_REMOVECFGNOPROPERTY, "Error removing server configuration. Couln't get the attribute %0:s.", "Host Name" ) ;
		return M4_ERROR ;
	}

	if (M4_ERROR == ai_poParamList -> GetString ( "configuration" , pcConfig ) )
	{
		M4_SRV_ERROR ( M4_ERR_DPCH_REMOVECFGNOPROPERTY, "Error removing server configuration. Couln't get the attribute %0:s.", "Configuration ID") ;
		return M4_ERROR ;
	}

	M4_SRV_GENERAL (  "Removing Site Node ==>  node: %0:s, host: %1:s", pcConfig << pcHost ) ;
	SETCODEF ( M4_INF_DPCH_REMOVECFGPROCESSING, DEBUGINFOLOG, "#*s1*#%s#%s#", pcConfig, pcHost ) ;
														                                       
	ClSiteNodeInformation oNodeInfo ( pcConfig , pcHost , 0, "", "" ) ;

	if ( M4_ERROR == m_poServerRepository -> GetNodeInfo ( oNodeInfo ) )
	{
		M4_SRV_ERROR_N ( M4_ERR_DPCH_REMOVECFGNONODE, "Error removing server configuration. Cannot get server node." ) ;		
		return M4_ERROR ;
	}

	ai_oConfigurationInfo = oNodeInfo ;

	iRet = m_poServerRepository->EraseNode ( oNodeInfo ) ;

	if ( iRet == M4_SUCCESS )
	{		
		iRet = PersistTarget ( ) ;
		
		M4_SRV_GENERAL (  "Removed Site Node ==>  node: %0:s, host: %1:s", pcConfig << pcHost ) ;
		SETCODEF ( M4_INF_DPCH_REMOVECFGSUCCESS, DEBUGINFOLOG, "#*s1*#%s#%s#", pcConfig, pcHost ) ;
	}
	else
	{
		M4_SRV_ERROR_N ( M4_ERR_DPCH_REMOVECFGNONODE, "Error removing server configuration. Cannot get server node." ) ;		
	}

	return iRet ;
}

m4return_t ClAdminAgent::UpdateConfiguration (m4string_t ai_szConfigId, m4string_t ai_strHost, m4uint32_t ai_iBasePort, m4string_t ai_strUserName, m4string_t ai_strPassword, m4string_t ai_szState, m4double_t ai_dLoad)
{
  //## begin ClAdminAgent::UpdateConfiguration%962811789.body preserve=yes
	ClSiteNodeInformation * poNodeInfo = NULL ;

	M4_MDBG_NEW( poNodeInfo, poNodeInfo = new ClSiteNodeInformation (ai_szConfigId, ai_strHost, ai_iBasePort , ai_strUserName, ai_strPassword, ai_szState , ai_dLoad) );


	if ( M4_ERROR == m_poServerRepository  -> UpdateNode ( *poNodeInfo ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOADMINAGENTUPDATENODEINFO,
			"Error updating Server Configuration from Administration Agent. Cannot update Server Information.");

		return M4_ERROR ;
	}

	return M4_SUCCESS ;
  //## end ClAdminAgent::UpdateConfiguration%962811789.body
}

void ClAdminAgent::CheckConsistency ()
{
  //## begin ClAdminAgent::CheckConsistency%963302393.body preserve=yes
	ClSiteNodeContainer oRemovableNodes;


	if (! m_poServerRepository  -> IsConsistent (oRemovableNodes) )
	{		
		//m_poServerRepository -> RemoveNodeList ( oRemovableNodes ) ;

		// Fail-Over: Start the halted server nodes and the time-out 
		// surpased starting server nodes
		StartHaltedNodes(oRemovableNodes);
	}

  //## end ClAdminAgent::CheckConsistency%963302393.body
}

m4return_t ClAdminAgent::StartHaltedNodes (ClSiteNodeContainer &ai_oSiteNodes)
{
  //## begin ClAdminAgent::StartHaltedNodes%969528444.body preserve=yes

	ClSiteNodeInformation oSiteNodeInfo;
	m4return_t iret = M4_ERROR;
	ClParamList *poParamList = 0;
	m4string_t strHost , strConfiguration;	
	m4char_t szBasePort[64];
	m4uint32_t iBasePort = 0;
	m4string_t strUser;
	m4string_t strPassword;
	m4return_t iRet = M4_ERROR;
	m4return_t iResult = M4_SUCCESS;


	// Loop

	while ( M4_SUCCESS == ai_oSiteNodes.GetNextNode( oSiteNodeInfo ) )
	{
	
		// Get the configuration data

		strHost = oSiteNodeInfo.GetNodeHost();
		strConfiguration = oSiteNodeInfo.GetSiteNodeID();
		iBasePort = oSiteNodeInfo.GetBasePort();
		strUser = oSiteNodeInfo.GetUserName();
		strPassword = oSiteNodeInfo.GetPassword();

		(void) sprintf(szBasePort , "%d" , iBasePort);
 				
        ClParamList * poParamList = NULL  ;

		M4_MDBG_NEW(  poParamList, poParamList = new ClParamList ) ;

		poParamList -> SetString ( "host" , strHost.c_str() ) ;
		poParamList -> SetString ( "configuration" , strConfiguration.c_str() ) ;
		poParamList -> SetString ( "user" , strUser.c_str() ) ;
		poParamList -> SetString ( "password" , strPassword.c_str() ) ;
		poParamList -> SetString ( "baseport" , szBasePort ) ;


		iRet = m_poServerRepository->RemoveNode ( oSiteNodeInfo ) ;

		// Start site node
		iRet = StartConfiguration (poParamList, oSiteNodeInfo);			
		if (M4_SUCCESS == iRet)
        {
            oSiteNodeInfo.SetIsEnabled(M4_TRUE);
			m_poServerRepository->AddNode(oSiteNodeInfo);
        }
		else
        {
			iResult = M4_ERROR;
        }

		M4_MDBG_DELETE( poParamList, delete poParamList );
	}

	PersistTarget () ;

	return iResult;

	//_FindCandidateNode(m_oCandidateIterator);

	//return M4_SUCCESS ;


  //## end ClAdminAgent::StartHaltedNodes%969528444.body
}

m4return_t ClAdminAgent::InitializeTargetConfigurations ()
{
  //## begin ClAdminAgent::InitializeTargetConfigurations%972463871.body preserve=yes
	m4uint64_t iPetitionID ;
	ClParamList * poParamList ;
	m4pchar_t buffer ;
	m4uint32_t iBufferLen ;
	m4return_t iRes = M4_ERROR;


	m_psTargetFile = GetDspTargetPath();


	iRes = ReadTargetFile ( m_psTargetFile , buffer, iBufferLen );

	if (iRes == M4_SUCCESS)
	{

		iPetitionID = ClIdGenerator::GetNewId () ;

		ClDspXMLRequest * poRequest = NULL  ;

		M4_MDBG_NEW(  (ClDspRequest *)poRequest, poRequest = new ClDspXMLRequest ( buffer, iPetitionID , 0) ) ;

		M4_MDBG_NEW(  poParamList, poParamList = new ClParamList ) ;

		poParamList -> SetPointer ( "REQUEST" , (void*) poRequest ) ;

		ParseXML ( poParamList ) ;

		// bugid: 0094322.
		delete poParamList;
	}

	// ok, now we've finished the initialization
	m_bIsInitializing = M4_FALSE;

    return iRes ;
  //## end ClAdminAgent::InitializeTargetConfigurations%972463871.body
}

m4return_t ClAdminAgent::ReadTargetFile (m4pchar_t ai_szFileName, m4pchar_t &ao_pBuffer, m4uint32_t &ao_iBufferLen)
{
  //## begin ClAdminAgent::ReadTargetFile%972463872.body preserve=yes
	FILE * fxml ;
	m4uint32_t i = 0;


	if (! ai_szFileName ) {
		return M4_ERROR ;
	} ;

	if ( M4_ERROR == GetSizeFile ( ai_szFileName, ao_iBufferLen ) )
	{
		M4_SRV_WARNING(0, "Cannot find file %0:s", ai_szFileName )  ;
		return M4_ERROR  ;
	}

	

	//alloco el buffer
	M4_MDBG_NEW( ao_pBuffer, ao_pBuffer = new m4char_t [ ao_iBufferLen  + 1] ) ;

	//abro fichero xml

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniANSI ;
    fxml = M4Fopen( ai_szFileName, M4UniReadBinary, eEncoding );

	if ( !fxml )
	{
		M4_SRV_ERROR(0, "Can not open file %0:s", ai_szFileName )  ;
        delete ao_pBuffer;
		return M4_ERROR  ;
	}

	if (ao_iBufferLen != fread(ao_pBuffer, sizeof(char), ao_iBufferLen, fxml))
	{
		M4_SRV_ERROR(0, "Error: the reading of the file %0:s is incorrect", ai_szFileName )  ;
        delete ao_pBuffer;
		return M4_ERROR  ;
	}

	//copio el fichero al buffer
/*
	while ( ! feof ( fxml ) )
	{
		ao_pBuffer [i++] = fgetc ( fxml ) ;
	}

*/

	ao_pBuffer [ao_iBufferLen] = 0 ;


	fclose ( fxml ) ;

	return M4_SUCCESS ;
  //## end ClAdminAgent::ReadTargetFile%972463872.body
}

m4return_t ClAdminAgent::WriteTargetFile (m4pchar_t ai_szFileName, m4pchar_t ai_pBuffer, m4uint32_t ai_iBufferLen)
{
  //## begin ClAdminAgent::WriteTargetFile%972463873.body preserve=yes
	FILE * fxml ;
	m4uint32_t i = 0;

	if (0 == ai_szFileName)
	{
		M4_SRV_ERROR_N(0, "Target file not specified")  ;
		return M4_ERROR  ;
	}

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniANSI ;
    fxml = M4Fopen( ai_szFileName, M4UniWrite, eEncoding );

	if ( !fxml )
	{
		M4_SRV_ERROR(0, "Can not open file %0:s", ai_szFileName )  ;
		return M4_ERROR  ;
	}
	
	fprintf ( fxml , ai_pBuffer ) ;

	fclose ( fxml) ;

	return M4_SUCCESS ;
  //## end ClAdminAgent::WriteTargetFile%972463873.body
}

m4return_t ClAdminAgent::PersistTarget ()
{
  //## begin ClAdminAgent::PersistTarget%972463874.body preserve=yes
	ClTargetConfigGen oXMLGen  ;
	ClSiteNodeContainer oSiteNodeList;
	m4pchar_t szBuffer = NULL;



	//if (m_poServerRepository->Count() > 0 ) 
	//{
		if (M4_ERROR == m_poServerRepository->GetNodeList(oSiteNodeList))
			return M4_ERROR;

		if ( M4_ERROR == oXMLGen.GenerateAnswer (oSiteNodeList, (void*&)szBuffer) )
		{
			M4_SRV_ERROR_N(M4_ERR_DPCH_NOADMINAGENTSTOPXMLANSWER,
				"Error stopping Server Configuration. Cannot generate XML answer." );			
			return M4_ERROR ;
		}

		if ( M4_ERROR == WriteTargetFile ( m_psTargetFile , szBuffer , strlen ( szBuffer)  ) )
		{
			return M4_ERROR ;
		}
	//}

	if ( szBuffer != NULL )
	{
		delete szBuffer ;
	}


	return  M4_SUCCESS ;
  //## end ClAdminAgent::PersistTarget%972463874.body
}

m4return_t ClAdminAgent::InitializeAppctlRepository (m4pchar_t ai_pszDspConfigPath)
{
  //## begin ClAdminAgent::InitializeAppctlRepository%972463883.body preserve=yes
	m4uint64_t iPetitionID ;
	ClParamList * poParamList ;
	m4pchar_t buffer ;
	m4uint32_t iBufferLen ;
	m4pchar_t psDspConfigPath = 0;


	if (0 == ai_pszDspConfigPath)
		psDspConfigPath = GetDspConfigPath();
	else
	{
		M4_MDBG_NEW(psDspConfigPath , psDspConfigPath = new m4char_t[strlen(ai_pszDspConfigPath) + 1]);
		(void) strcpy(psDspConfigPath, ai_pszDspConfigPath);
	}

	if ( M4_ERROR == ReadTargetFile ( psDspConfigPath , buffer, iBufferLen ) )
	{
		M4_MDBG_DELETE(psDspConfigPath, delete psDspConfigPath);
		return M4_ERROR ;
	}
	M4_MDBG_DELETE(psDspConfigPath, delete psDspConfigPath);

	iPetitionID = ClIdGenerator::GetNewId () ;

	ClDspXMLRequest * poRequest = NULL ;

	M4_MDBG_NEW(  (ClDspRequest *)poRequest, poRequest = new ClDspXMLRequest ( buffer, iPetitionID , 0) ) ;

	M4_MDBG_NEW(  poParamList, poParamList = new ClParamList ) ;

	poParamList -> SetPointer ( "REQUEST" , (void*) poRequest ) ;

	ParseXML ( poParamList ) ;

    // bugid: 0094322.
    delete poParamList;

	return M4_SUCCESS ;
  //## end ClAdminAgent::InitializeAppctlRepository%972463883.body
}

m4return_t ClAdminAgent::AddAppctlInfo (ClParamList *ai_poParamList)
{
  //## begin ClAdminAgent::AddAppctlInfo%972463884.body preserve=yes
	m4pchar_t szHost , szPort ;
	m4uint32_t iPort ;



	if ( M4_ERROR == ai_poParamList -> GetString ( "host" , szHost ) )
	{
		return M4_ERROR ;
	}

	ToLower ( szHost ) ;

	if ( M4_ERROR == ai_poParamList -> GetString ( "port" , szPort ) )
	{
		return M4_ERROR ;
	}

	if ( 1 < sscanf ( szPort , "%d" , &iPort  ) )
	{
		return M4_ERROR ;
	}

	ClAppctlNodeInfo oNode ( szHost , iPort ) ;

	return m_oAppctlRepository.AddNode (oNode );
  //## end ClAdminAgent::AddAppctlInfo%972463884.body
}

m4return_t ClAdminAgent::Terminate ()
{
  //## begin ClAdminAgent::Terminate%973255784.body preserve=yes

	m_poTimer -> Terminate () ;

	while (! m_poTimer -> IsExit () )
	{};

	return M4_SUCCESS ;
  //## end ClAdminAgent::Terminate%973255784.body
}

m4pchar_t ClAdminAgent::GetDspConfigPath ()
{
  //## begin ClAdminAgent::GetDspConfigPath%976007080.body preserve=yes

	m4pchar_t psResult = 0;

	#ifdef _WINDOWS

		m4char_t szPath[2048];

		if (0 !=  GetModuleFileName( NULL, szPath, 2048 ))
		{
			psResult = ComposeDspConfigPath(szPath);
		}

	#endif


	#ifdef _UNIX

		psResult = ComposeDspConfigPath();

	#endif

	return (psResult);

  //## end ClAdminAgent::GetDspConfigPath%976007080.body
}

m4pchar_t ClAdminAgent::ComposeDspConfigPath (m4pchar_t ai_psAppCtlExePath)
{
  //## begin ClAdminAgent::ComposeDspConfigPath%976007081.body preserve=yes

	m4pchar_t psResult = TARGETAPPCTLFILE;
	m4pchar_t psCurrentLimit = 0;
	m4pchar_t psForeLimit = 0;


	#ifdef _WINDOWS

	if (0 != ai_psAppCtlExePath)
	{

		// Get the configuration file parent directory

		psCurrentLimit = ai_psAppCtlExePath;
		do
		{
			psCurrentLimit = strstr(psCurrentLimit, DISPATCHER_BIN_RELATIVE_DIR);

			if (0 != psCurrentLimit)
			{
				psForeLimit = psCurrentLimit;
				psCurrentLimit += strlen(DISPATCHER_BIN_RELATIVE_DIR);
			}
		} while (0 != psCurrentLimit);

		if (0 != psForeLimit)
		{
			M4_MDBG_NEW(psResult, psResult = new m4char_t[psForeLimit - ai_psAppCtlExePath + strlen(TARGETAPPCTLFILE_RELATIVE_PATH) + 1]);
			(void) strncpy(psResult, ai_psAppCtlExePath, psForeLimit - ai_psAppCtlExePath);
			psResult[psForeLimit - ai_psAppCtlExePath] = '\0';
			(void) strcat(psResult, TARGETAPPCTLFILE_RELATIVE_PATH);
			psResult[psForeLimit - ai_psAppCtlExePath + strlen(TARGETAPPCTLFILE_RELATIVE_PATH)] = '\0';	
		}
		else
		{
				psResult = 0;
		}
	}

	#endif

	#ifdef _UNIX

		// Suppose always that the Current Working Directory is "<some path>/bin"
		// and the configuration directory is "../conf/m4appctl.conf"

		psResult = TARGETAPPCTLFILE;

	#endif

	return (psResult);

  //## end ClAdminAgent::ComposeDspConfigPath%976007081.body
}

m4pchar_t ClAdminAgent::GetDspConfigDirPath ()
{
  //## begin ClAdminAgent::ComposeDspConfigPath%976007081.body preserve=yes

	// in unix we just return the relative dir to conf. We assume that the current directory is /bin
	#ifdef _UNIX
		return DSP_RELATIVE_CONFIG_DIR;
	#endif // _UNIX

	// the exe file path. Default is current directory (UNIX)
	m4char_t pcExeDir[2048] = "";
	m4pchar_t psResult = 0;

	#ifdef _WINDOWS

		if (GetModuleFileName( NULL, pcExeDir, sizeof(pcExeDir)) == 0)
		{
			// error... defaults to current directory
			strcpy(pcExeDir,"");
		}


		// ok lets compute the parent directory in windows

		m4pchar_t psCurrentLimit = 0;
		m4pchar_t psForeLimit = 0;

		psCurrentLimit = pcExeDir;
		do
		{
			psCurrentLimit = strstr(psCurrentLimit, DISPATCHER_BIN_RELATIVE_DIR);

			if (0 != psCurrentLimit)
			{
				psForeLimit = psCurrentLimit;
				psCurrentLimit += strlen(DISPATCHER_BIN_RELATIVE_DIR);
			}
		} while (0 != psCurrentLimit);

		if (0 != psForeLimit)
		{
			M4_MDBG_NEW(psResult, psResult = new m4char_t[psForeLimit - pcExeDir + strlen(DISPATCHER_CONFIG_RELATIVE_DIR) + 1]);
			(void) strncpy(psResult, pcExeDir, psForeLimit - pcExeDir);
			psResult[psForeLimit - pcExeDir] = '\0';
			(void) strcat(psResult, DISPATCHER_CONFIG_RELATIVE_DIR);
			psResult[psForeLimit - pcExeDir + strlen(DISPATCHER_CONFIG_RELATIVE_DIR)] = '\0';	
		}

	#endif // _WINDOWS

	return (psResult);

  //## end ClAdminAgent::ComposeDspConfigPath%976007081.body
}

m4pchar_t ClAdminAgent::GetDspTargetPath ()
{
  //## begin ClAdminAgent::GetDspTargetPath%976007082.body preserve=yes

	m4pchar_t psResult = 0;

	#ifdef _WINDOWS

		m4pchar_t szPath = new m4char_t [2048];

//		if (0 !=  GetModuleFileName( NULL, szPath, 2048 ))
		if (0 !=  getcwd (  szPath, 2048 ))
		{
			psResult = ComposeDspTargetPath(szPath);
		}

	#endif


	#ifdef _UNIX

		psResult = ComposeDspTargetPath();

	#endif

	return (psResult);

  //## end ClAdminAgent::GetDspTargetPath%976007082.body
}

m4pchar_t ClAdminAgent::ComposeDspTargetPath (m4pchar_t ai_psDspExePath)
{
  //## begin ClAdminAgent::ComposeDspTargetPath%976007083.body preserve=yes

	m4pchar_t psResult = TARGETAPPCTLFILE;
	m4pchar_t psCurrentLimit = 0;
	m4pchar_t psForeLimit = 0;

	if (! ai_psDspExePath )
		return TARGETINIFILE ;


	psResult = ai_psDspExePath ;

	strcat ( psResult , TARGETINIFILE ) ;

	return psResult  ;


	#ifdef _WINDOWS

	if (0 != ai_psDspExePath)
	{

		// Get the configuration file parent directory

		psCurrentLimit = ai_psDspExePath;
		do
		{
			psCurrentLimit = strstr(psCurrentLimit, DISPATCHER_BIN_RELATIVE_DIR);

			if (0 != psCurrentLimit)
			{
				psForeLimit = psCurrentLimit;
				psCurrentLimit += strlen(DISPATCHER_BIN_RELATIVE_DIR);
			}
		} while (0 != psCurrentLimit);

		if (0 != psForeLimit)
		{
			psForeLimit += strlen(DISPATCHER_BIN_RELATIVE_DIR);
			M4_MDBG_NEW(psResult, psResult = new m4char_t[psForeLimit - ai_psDspExePath + strlen(TARGETINIFILE) + 1]);
			(void) strncpy(psResult, ai_psDspExePath, psForeLimit - ai_psDspExePath);
			psResult[psForeLimit - ai_psDspExePath] = '\0';
			(void) strcat(psResult, TARGETINIFILE);
			psResult[psForeLimit - ai_psDspExePath + strlen(TARGETINIFILE)] = '\0';	
		}
		else
		{
				psResult = 0;
		}
	}

	#endif

	#ifdef _UNIX

		// Suppose always that the Current Working Directory is "<some path>/bin"
		// and the target file is "<some path>/bin/m4dsptarget.xml"

		psResult = TARGETINIFILE;

	#endif

	return (psResult);

  //## end ClAdminAgent::ComposeDspTargetPath%976007083.body
}

m4return_t ClAdminAgent::RegisterTargetNode (ClParamList *ai_poParamList)
{
  //## begin ClAdminAgent::RegisterTargetNode%991920579.body preserve=yes
	ClSiteNodeInformation * poNodeInfo ;
	m4uint32_t iBasePort = 0 ;
	m4return_t iRet = M4_SUCCESS ;
	m4pchar_t strhost =NULL, baseport =NULL, configuration =NULL, user =NULL, password =NULL;
	ClDispatcherInvoker * poInvoker = ClDispatcherInvoker::Instance () ;
	//ClSiteNodeContainer oNodeContainer ;
	ClLogSystem * poLog = NULL ;

	
	if (M4_ERROR == ai_poParamList -> GetString ( "host" , strhost ) )
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOADMINAGENTSTARTCONFIGPARAM, 
			"Error starting Server Configuration. Cannot get the attribute %0:s.", 
			"Host Name");			
		return M4_ERROR ;
	}

	ToLower ( strhost) ;

	if (M4_ERROR == ai_poParamList -> GetString ( "configuration" , configuration ) )
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOADMINAGENTSTARTCONFIGPARAM, 
			"Error starting Server Configuration. Cannot get the attribute %0:s.", 
			"Configuration");			
		return M4_ERROR ;
	}
	ToLower ( configuration ) ;

	if (M4_ERROR == ai_poParamList -> GetString ( "user" , user ) )
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOADMINAGENTSTARTCONFIGPARAM, 
			"Error starting Server Configuration. Cannot get the attribute %0:s.", 
			"User Name");			

		return M4_ERROR ;
	}

	if (M4_ERROR == ai_poParamList -> GetString ( "password" , password ) )
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOADMINAGENTSTARTCONFIGPARAM, 
			"Error starting Server Configuration. Cannot get the attribute %0:s.", 
			"Password");			

		return M4_ERROR ;
	}

	if (M4_SUCCESS == ai_poParamList -> GetString ( "baseport" , baseport) )
	{
		iBasePort = atoi ( baseport ) ;
	}


	M4_MDBG_NEW( poNodeInfo, poNodeInfo = new ClSiteNodeInformation (configuration , strhost, iBasePort , user , password , STATE_HALTED ) );

    /* set isEnabled member */
    m4pchar_t pcIsEnabled = NULL;
	if (M4_SUCCESS == ai_poParamList->GetString("enabled", pcIsEnabled))
	{
        m4bool_t bIsEnabled = stricmp(pcIsEnabled, "true") ? M4_FALSE : M4_TRUE;
		poNodeInfo->SetIsEnabled(bIsEnabled);
	}


	/* oNodeContainer.AddNode ( *poNodeInfo ) ; */

	if ( M4_ERROR == m_poServerRepository  -> AddNode ( *poNodeInfo ) )
	{
		M4_MDBG_DELETE( poNodeInfo, delete poNodeInfo ) ;

		return M4_ERROR ;
	}


	M4_MDBG_DELETE( poNodeInfo, delete poNodeInfo ) ;

	return M4_SUCCESS ;
  //## end ClAdminAgent::RegisterTargetNode%991920579.body
}


m4pchar_t ClAdminAgent::GetDspAdminHostsConfigPath()
{
	m4pchar_t psResult = 0 ;

	#ifdef _WINDOWS

		m4char_t szPath[2048] ;

		if (0 != GetModuleFileName( NULL, szPath, 2048 ))
		{
			psResult = ComposeDspAdminHostsConfigPath( szPath ) ;
		}

	#endif


	#ifdef _UNIX

		psResult = ComposeDspAdminHostsConfigPath() ;

	#endif

	return (psResult) ;
}

m4pchar_t ClAdminAgent::ComposeDspAdminHostsConfigPath( m4pchar_t ai_pcExePath )
{
	m4pchar_t psResult = ADMINHOSTSFILE;
	m4pchar_t psCurrentLimit = 0;
	m4pchar_t psForeLimit = 0;


	#ifdef _WINDOWS

	if (0 != ai_pcExePath)
	{

		// Get the configuration file parent directory

		psCurrentLimit = ai_pcExePath;
		do
		{
			psCurrentLimit = strstr(psCurrentLimit, DISPATCHER_BIN_RELATIVE_DIR);

			if (0 != psCurrentLimit)
			{
				psForeLimit = psCurrentLimit;
				psCurrentLimit += strlen(DISPATCHER_BIN_RELATIVE_DIR);
			}
		} while (0 != psCurrentLimit);

		if (0 != psForeLimit)
		{
			M4_MDBG_NEW(psResult, psResult = new m4char_t[psForeLimit - ai_pcExePath + strlen(ADMHOSTSFILE_RELATIVE_PATH) + 1]);
			(void) strncpy(psResult, ai_pcExePath, psForeLimit - ai_pcExePath);
			psResult[psForeLimit - ai_pcExePath] = '\0';
			(void) strcat(psResult, ADMHOSTSFILE_RELATIVE_PATH);
			psResult[psForeLimit - ai_pcExePath + strlen(ADMHOSTSFILE_RELATIVE_PATH)] = '\0';	
		}
		else
		{
				psResult = 0;
		}
	}

	#endif

	#ifdef _UNIX

		// Suppose always that the Current Working Directory is "<some path>/bin"
		// and the configuration directory is "../conf/m4appctl.conf"

		psResult = ADMINHOSTSFILE;

	#endif

	return (psResult);
}

m4bool_t ClAdminAgent::CheckAuthConnection(m4uint32_t ai_uiConnectionId, m4pcchar_t ai_pccToken)
{
	AuthConnectionsIt_t oIt = m_oAuthConnections.find(ai_uiConnectionId);
	
	if (oIt != m_oAuthConnections.end())
	{
		// the user was already authenticated
		return M4_TRUE;
	}

	if (ai_pccToken == NULL)
	{
		return M4_FALSE;
	}

	// ok, this connection was not authenticated before, lets check the password
	if (strcmp(m_pcAuthPass, ai_pccToken) != 0)
	{
		// the input pass doesn't match
		return M4_FALSE;
	}

	// hey, authentication was ok!
	m_oAuthConnections.insert(ai_uiConnectionId);
	return M4_TRUE;
}

m4void_t ClAdminAgent::ReleaseConnection(m4uint32_t ai_uiConnectionId)
{
	m_oAuthConnections.erase(ai_uiConnectionId);
}

m4bool_t ClAdminAgent::IsInitializing()
{
	return m_bIsInitializing;
}
	  
m4bool_t ClAdminAgent::CheckHaveToAuthenticate()
{
	return m_bHasAuthentication;
}

m4void_t ClAdminAgent::LoadPasswordFromFile()
{
	m_pcAuthPass = NULL;

	// generates the password file path
	m4pchar_t pcPassFilePath;
	M4_MDBG_NEW( pcPassFilePath, pcPassFilePath = new m4char_t[strlen(m_pcDspConfigDirPath) + strlen(DSP_PASS_FILE_NAME) + 1] ) ;

	m4char_t pcPass[2048];

	// init
	strcpy(pcPassFilePath, m_pcDspConfigDirPath);
	strcat(pcPassFilePath, DSP_PASS_FILE_NAME);

	// opens the input file
	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniANSI ;
    FILE *fPass = M4Fopen( pcPassFilePath, M4UniReadBinary, eEncoding );

	if ( !fPass )
	{
		M4_SRV_ERROR(0, "Can not open file %0:s", pcPassFilePath )  ;
		delete [] pcPassFilePath;
		return;
	}

	// we have authentication file so we must require the authentication token
	m_bHasAuthentication = M4_TRUE;
	
	m4int32_t iLen = fscanf(fPass, "%2047s", pcPass);
	if (iLen <= 0)
	{
		M4_SRV_ERROR(0, "Error: the reading of the file %0:s is incorrect", pcPassFilePath )  ;
		delete [] pcPassFilePath;
		fclose(fPass);
		return;
	}

	delete[] pcPassFilePath;
	fclose(fPass);

	iLen = strlen(pcPass);
	M4_MDBG_NEW( m_pcAuthPass, m_pcAuthPass = new m4char_t[ iLen + 1 ] ) ;
	strcpy(m_pcAuthPass, pcPass);
}

// Additional Declarations
  //## begin ClAdminAgent%3947972F0207.declarations preserve=yes
  //## end ClAdminAgent%3947972F0207.declarations

//## begin module%396B5C6A010E.epilog preserve=yes
//## end module%396B5C6A010E.epilog


// Detached code regions:
// WARNING: this code will be lost if code is regenerated.
#if 0
//## begin ClAdminAgent::ToLower%972543784.body preserve=no
	m4pchar_t szString ;


	szString = aio_string ;
	for( ; *aio_string; ++aio_string) 
	{
		*aio_string=tolower(*aio_string) ;
	} ;

	return szString ;
//## end ClAdminAgent::ToLower%972543784.body

#endif
