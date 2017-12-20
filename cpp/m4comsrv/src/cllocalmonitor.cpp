//## begin module%3426894B01AE.cm preserve=no
//## end module%3426894B01AE.cm

//## begin module%3426894B01AE.cp preserve=no
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
//## end module%3426894B01AE.cp

//## Module: ClLocalMonitor%3426894B01AE; Package body
//## Subsystem: version::src%379ECBD7029A
//## Source file: F:\integration\m4comsrv\src\cllocalmonitor.cpp

//## begin module%3426894B01AE.additionalIncludes preserve=no
//## end module%3426894B01AE.additionalIncludes

//## begin module%3426894B01AE.includes preserve=yes
#include <m4srvres.hpp>
//Rose 98 Includes
#include <subsystemids.hpp>
#include <connects.hpp>
#include <subsystems.hpp>
#include <clsubsystem.hpp>
#include <clssinvoker.hpp>
#include <lmstats.hpp>
#include <ussession.hpp>
#include <m4actionconstants.hpp>
#include <commactionsname.h>
//## end module%3426894B01AE.includes

// cllminterface
#include <cllminterface.hpp>
// NavigatorF
#include <navigatorf.hpp>
// FIFOQ
#include <fifoq.hpp>
// m4log
#include <m4log.hpp>
// sessiondic
#include <sessiondic.hpp>
// invoker
#include <invoker.hpp>
// cllist
#include <cllist.hpp>
// ClLocalMonitor
#include <cllocalmonitor.hpp>
// clservicedicc
#include <clservicedicc.hpp>
// clcondictionary
#include <clcondictionary.hpp>

class ClConfiguration;
class ClCSSessManager;
class ClSSFactory;
class ClUsersSS;
class ClRequestSS;
class ClRequest;
class ClLauncherInterface;
class ClSSInterfaz;

//## begin module%3426894B01AE.declarations preserve=no
//## end module%3426894B01AE.declarations

//## begin module%3426894B01AE.additionalDeclarations preserve=yes
#ifdef _DEBUG
	m4uint32_t		g_uiPDUSInside = 0;
#endif
//## end module%3426894B01AE.additionalDeclarations


// Class ClLocalMonitor 


















//## begin ClLocalMonitor::m_poLocalMonitor%3563E11E0309.role preserve=no  public: static ClLocalMonitor { -> RHAN}
ClLocalMonitor *ClLocalMonitor::m_poLocalMonitor = NULL;
//## end ClLocalMonitor::m_poLocalMonitor%3563E11E0309.role



ClLocalMonitor::ClLocalMonitor ()
  //## begin ClLocalMonitor::ClLocalMonitor%874936018.hasinit preserve=no
  //## end ClLocalMonitor::ClLocalMonitor%874936018.hasinit
  //## begin ClLocalMonitor::ClLocalMonitor%874936018.initialization preserve=yes
  //## end ClLocalMonitor::ClLocalMonitor%874936018.initialization
{
  //## begin ClLocalMonitor::ClLocalMonitor%874936018.body preserve=yes
//	INITIALIZELOGMANAGER ( "Logsys.ini" ) ;
  m_poLMStats = NULL ;
  m_oAdministratorList.SetActiveList (  & (this -> m_oActiveList) ) ;
  m_oSessionManagerList.SetActiveList ( & this -> m_oActiveList ) ;

//  m_oUserDic.SetSessionDic ( & m_oSessionDic ) ;
//  m_oUserDic.SetConDictionary ( & m_oConDictionary ) ;
  //m_oActiveList.SetLocalMonitor ( this ) ;


	m_oInvoker.SetManager ( this ) ;

	BuildCommands () ;

	m_iCommunicationType = M4_WORKER_BASE_COMM ;
//	ClAutoGarbageable::SetBin (&m_oGarbage);
  //## end ClLocalMonitor::ClLocalMonitor%874936018.body
}


ClLocalMonitor::~ClLocalMonitor ()
{
  //## begin ClLocalMonitor::~ClLocalMonitor%874936019.body preserve=yes
	//  DESTROYLOGMANAGER ( ) ;
  //## end ClLocalMonitor::~ClLocalMonitor%874936019.body
}



//## Other Operations (implementation)
ClConnInstance * ClLocalMonitor::GetConnection (m4int32_t ai_IdConnection)
{
  //## begin ClLocalMonitor::GetConnection%874936020.body preserve=yes
	ClConnInstance * poConnection ;



	poConnection = (ClConnInstance * ) m_oConDictionary.GetItem ( ai_IdConnection ) ;


	if ( ! poConnection )
	{
		SETCODEF ( M4_SRV_LM_COULD_NOT_GET_CONNECTION , ERRORLOG , "Couldn't get the connection from the dictionary" ) ;

		return NULL;
	}


	return poConnection ;
  //## end ClLocalMonitor::GetConnection%874936020.body
}

m4return_t ClLocalMonitor::BuildCommands ()
{
  //## begin ClLocalMonitor::BuildCommands%875000089.body preserve=yes

	m4return_t iRet ;


	iRet = m_oInvoker.CreateCommand ( CREATESERVICE ) ;

	if (iRet == M4_ERROR )
	{
		SETCODEF ( M4_SRV_LM_COMMAND_DOES_NOT_EXIST , ERRORLOG , "Command CREATESERVICE wasn't been able to be created" ) ;
	}

	/* Este commando no está en la tabla de comandos (ClCommandTable::CreateCommand). Siempre da error */
    /*
    iRet = m_oInvoker.CreateCommand ( CREATEACCEPTOR ) ;

	if (iRet == M4_ERROR )
	{
		SETCODEF ( M4_SRV_LM_COMMAND_DOES_NOT_EXIST , ERRORLOG , "Command CREATEACCEPTOR wasn't been able to be created" ) ;
	}
    */
	
	iRet = m_oInvoker.CreateCommand ( CREATECONNMANAGER ) ;

	if (iRet == M4_ERROR )
	{
		SETCODEF ( M4_SRV_LM_COMMAND_DOES_NOT_EXIST , ERRORLOG , "Command CREATECONNMANAGER wasn't been able to be created" ) ;
	}

	iRet = m_oInvoker.CreateCommand ( CREATELAUNCHER ) ;

	if (iRet == M4_ERROR )
	{
		SETCODEF ( M4_SRV_LM_COMMAND_DOES_NOT_EXIST , ERRORLOG , "Command CREATELAUNCHER wasn't been able to be created" ) ;
	}

	iRet = m_oInvoker.CreateCommand ( CREATEADMINISTRATOR ) ;

	if (iRet == M4_ERROR )
	{
		SETCODEF ( M4_SRV_LM_COMMAND_DOES_NOT_EXIST , ERRORLOG , "Command CREATEADMINISTRATOR wasn't been able to be created" ) ;
	}

	iRet = m_oInvoker.CreateCommand ( SHUTDOWNSERVICE ) ;

	if (iRet == M4_ERROR )
	{
		SETCODEF ( M4_SRV_LM_COMMAND_DOES_NOT_EXIST , ERRORLOG , "Command SHUTDOWNSERVICE wasn't been able to be created" ) ;
	}

	/* ídem. CREATEACCEPTOR
    iRet = m_oInvoker.CreateCommand ( SHUTDOWNACCEPTOR ) ;

	if (iRet == M4_ERROR )
	{
		SETCODEF ( M4_SRV_LM_COMMAND_DOES_NOT_EXIST , ERRORLOG , "Command SHUTDOWNACCEPTOR wasn't been able to be created" ) ;
	}
    */
	
	iRet = m_oInvoker.CreateCommand ( SHUTDOWNCONNMANAGER ) ;

	if (iRet == M4_ERROR )
	{
		SETCODEF ( M4_SRV_LM_COMMAND_DOES_NOT_EXIST , ERRORLOG , "Command SHUTDOWNCONNMANAGER wasn't been able to be created" ) ;
	}

	iRet = m_oInvoker.CreateCommand ( SHUTDOWNLAUNCHER ) ;

	if (iRet == M4_ERROR )
	{
		SETCODEF ( M4_SRV_LM_COMMAND_DOES_NOT_EXIST , ERRORLOG , "Command SHUTDOWNLAUNCHER wasn't been able to be created" ) ;
	}

	iRet = m_oInvoker.CreateCommand ( NEWCONNECTION ) ;

	if (iRet == M4_ERROR )
	{
		SETCODEF ( M4_SRV_LM_COMMAND_DOES_NOT_EXIST , ERRORLOG , "Command NEWCONNECTION wasn't been able to be created" ) ;
	}
	
	iRet = m_oInvoker.CreateCommand ( DOWNCONNECTION ) ;

	if (iRet == M4_ERROR )
	{
		SETCODEF ( M4_SRV_LM_COMMAND_DOES_NOT_EXIST , ERRORLOG , "Command DOWNCONNECTION wasn't been able to be created" ) ;
	}
	
	iRet = m_oInvoker.CreateCommand ( SHUTDOWNADMINISTRATOR ) ;

	if (iRet == M4_ERROR )
	{
		SETCODEF ( M4_SRV_LM_COMMAND_DOES_NOT_EXIST , ERRORLOG , "Command SHUTDOWNADMINISTRATOR wasn't been able to be created" ) ;
	}

	/* ídem. CREATEACCEPTOR
	iRet = m_oInvoker.CreateCommand ( CREATEADMINACCEPTOR ) ;

	if (iRet == M4_ERROR )
	{
		SETCODEF ( M4_SRV_LM_COMMAND_DOES_NOT_EXIST , ERRORLOG , "Command CREATEADMINACCEPTOR wasn't been able to be created" ) ;
	}
    */
	

	return M4_SUCCESS;
  //## end ClLocalMonitor::BuildCommands%875000089.body
}

m4return_t ClLocalMonitor::NewConnection (ClConnection *ai_poConnection)
{
  //## begin ClLocalMonitor::NewConnection%875696747.body preserve=yes
		ClCmdArguments oArguments ;
		m4return_t iRet ;


		oArguments.SetPArgument ( ai_poConnection ) ;

		iRet = m_oInvoker.ExecuteCommand ( NEWCONNECTION  , ( void *& ) ai_poConnection , &oArguments ) ;

		return iRet ;
  //## end ClLocalMonitor::NewConnection%875696747.body
}

m4return_t ClLocalMonitor::RemoveConnection (ClConnInstance* ai_poConnection)
{
  //## begin ClLocalMonitor::RemoveConnection%875696750.body preserve=yes
	m4int64_t IdConnection ;
	m4return_t iRet = M4_SUCCESS ;
	ClParamList * poParamList  ;


	if (! ai_poConnection )
	{
		return M4_ERROR ;
	}

	ai_poConnection -> SetStatus ( CLOSSING ) ;

	IdConnection = ai_poConnection -> GetId ( ) ;

	ClSSInvoker * poInvoker = ClSSInvoker::GetInstance () ;

	poInvoker -> GetParamList ( poParamList ) ;

	poParamList -> SetInt ( M4_PARAM_CONNECTIONID , IdConnection ) ;

	if ( M4_ERROR == ( iRet = poInvoker ->ExecuteAction ( REMOVE_CONNECTION_ACTION , poParamList ) ) )
	{
		return M4_ERROR ;
	}


	return iRet ;


  //## end ClLocalMonitor::RemoveConnection%875696750.body
}

m4return_t ClLocalMonitor::DestroyConnection (ClConnInstance* ai_poConnection)
{
  //## begin ClLocalMonitor::DestroyConnection%876753079.body preserve=yes
	ClSystem * poSystem = NULL ;
	ClUSSessionInterface * poSession = NULL ;
	ClUSUserInterface * poUser ;
	ClParamList * poParamList ;
	


	ai_poConnection -> SetStatus ( BROKEN  ) ;

	if ( M4_CONNECTION_ORIENTED_PROTOCOL == ai_poConnection -> GetConnProtocol () )
	{
		poSession = ((ClConnection*)ai_poConnection ) -> GetMainSession () ;
	}

	if ( poSession )
	{

		poSystem = ClSystem::Instance () ;



		// Conseguir el ID a partir de la implementación
		m4int64_t lIdSession , lIdUser = 0 ;

		lIdSession = poSession -> GetId ();

		if ( poUser = poSession -> GetUser () )
		{
			lIdUser = poUser -> GetId () ;
		}
		// Fin obtención id session

		ClSSInvoker * poInvoker = ClSSInvoker::GetInstance() ;

		poInvoker -> GetParamList ( poParamList ) ;

		poParamList -> SetDouble ( "SESSIONID" , (m4double_t) lIdSession ) ;
		poParamList -> SetDouble ( "USERID" , (m4double_t) lIdUser ) ;

		if ( M4_ERROR == poInvoker -> ExecuteAction ( END_USER_SESSION_ACTION , poParamList ) )
		{
			return M4_ERROR ;
		}
	}
	else 
	{
		return RemoveConnection ( ai_poConnection ) ;
	}

	return M4_SUCCESS ;
  //## end ClLocalMonitor::DestroyConnection%876753079.body
}

m4return_t ClLocalMonitor::ShutDown ()
{
  //## begin ClLocalMonitor::ShutDown%876753077.body preserve=yes
	//ClSCReceiver * poReceiver ;
	 m_oAdministratorList.ShutDown () ;

	if ( m_oActiveList.ShutDown ( ) == M4_ERROR )
	{
		return M4_ERROR ;
	}

	return M4_SUCCESS ;
  //## end ClLocalMonitor::ShutDown%876753077.body
}

ClServiceInterface * ClLocalMonitor::CreateService (m4int32_t ai_IdService, m4uint16_t ai_iNumQItems)
{
  //## begin ClLocalMonitor::CreateService%875000090.body preserve=yes
	ClCmdArguments	oArguments ;
	ClServiceInterface	*poService ;

	oArguments.SetNArgument( ai_IdService ) ;

	oArguments.SetNArgument( ai_iNumQItems ) ;

	if ( m_oInvoker.ExecuteCommand ( CREATESERVICE  , (void *& ) poService , &oArguments ) == M4_ERROR )
	{
		return NULL ;
	}

return poService;
  //## end ClLocalMonitor::CreateService%875000090.body
}

m4return_t ClLocalMonitor::ShutDownService (m4int32_t ai_IdService)
{
  //## begin ClLocalMonitor::ShutDownService%876753078.body preserve=yes
	ClCmdArguments oArguments ;
	ClServiceInfo * poService ;

	oArguments.SetNArgument( ai_IdService ) ;

	if ( m_oInvoker.ExecuteCommand ( SHUTDOWNSERVICE  , (void *& ) poService , &oArguments ) == M4_ERROR )
	{
		return M4_ERROR ;
	}

	return M4_SUCCESS ;
  //## end ClLocalMonitor::ShutDownService%876753078.body
}

ClCSSessManager * ClLocalMonitor::CreateSessionManager (ClUsersSS *ai_poUsersSS)
{
  //## begin ClLocalMonitor::CreateSessionManager%876990186.body preserve=yes
	ClCmdArguments oArguments;
	ClCSSessManager * poSessManager ;


	oArguments.SetPArgument ( ai_poUsersSS ) ;

	if ( m_oInvoker.ExecuteCommand ( CREATECONNMANAGER  , (void *& ) poSessManager , &oArguments ) == M4_ERROR )
	{
		return NULL ;
	}

return poSessManager ;
  //## end ClLocalMonitor::CreateSessionManager%876990186.body
}

m4return_t ClLocalMonitor::ShutDownSessionManager (ClCSSessManager *ai_poSessManager)
{
  //## begin ClLocalMonitor::ShutDownSessionManager%878114632.body preserve=yes
	m4return_t iRet;


	iRet = m_oInvoker.ExecuteCommand ( SHUTDOWNCONNMANAGER  , (void *& ) ai_poSessManager ) ;

return iRet ;
  //## end ClLocalMonitor::ShutDownSessionManager%878114632.body
}

ClLauncherInterface * ClLocalMonitor::CreateLauncher (m4int32_t ai_iService, m4char_t ai_iType, ClNavigatorFirewall *ai_pNavigator, ClConfiguration *ai_pConfig, m4pchar_t ai_szDllName, m4pchar_t ai_szFuncName)
{
  //## begin ClLocalMonitor::CreateLauncher%878114633.body preserve=yes
	ClCmdArguments  oArguments ;
	ClLauncherInterface * poLauncher ;


	oArguments.SetNArgument ( ai_iService ) ;

	oArguments.SetNArgument ( ai_iType ) ;
/*
	oArguments.SetPArgument ( ai_pNavigator ) ;

	oArguments.SetPArgument ( ai_pConfig ) ;

//	oArguments.SetPArgument ( ai_szDllName ) ;

//	oArguments.SetPArgument ( ai_szFuncName ) ;
	
*/
	if ( m_oInvoker.ExecuteCommand ( CREATELAUNCHER  , (void *& ) poLauncher , &oArguments ) == M4_ERROR )
	{
		return NULL ;
	}

return poLauncher ;
  //## end ClLocalMonitor::CreateLauncher%878114633.body
}

m4return_t ClLocalMonitor::ShutDownLauncher (ClLauncherInterface *ai_poLauncher)
{
  //## begin ClLocalMonitor::ShutDownLauncher%878114634.body preserve=yes
	ClCmdArguments  oArguments ;
	void * poLauncher ;
	m4return_t iRet;

/*
	oArguments.SetNArgument ( ai_lIdService ) ;

	oArguments.SetNArgument ( ai_iType ) ;
*/

	iRet = m_oInvoker.ExecuteCommand ( SHUTDOWNLAUNCHER  , (void *& ) poLauncher , &oArguments) ;

return iRet ;
  //## end ClLocalMonitor::ShutDownLauncher%878114634.body
}

void ClLocalMonitor::Accept (ClLMVisitor &ai_oVisitor)
{
  //## begin ClLocalMonitor::Accept%878200308.body preserve=yes
	ai_oVisitor.VisitLM ( * this ) ;
  //## end ClLocalMonitor::Accept%878200308.body
}

ClCSAdministrator * ClLocalMonitor::CreateAdministrator (ClTSAP *ai_poTSAP)
{
  //## begin ClLocalMonitor::CreateAdministrator%878225915.body preserve=yes
	ClCSAdministrator * poAdministrator ;
	ClCmdArguments oArguments ;


	oArguments.SetPArgument ( ai_poTSAP ) ;

	if ( m_oInvoker.ExecuteCommand ( CREATEADMINISTRATOR  , (void *& ) poAdministrator , & oArguments ) == M4_ERROR )
	{
		return NULL ;
	}

return poAdministrator ;
  //## end ClLocalMonitor::CreateAdministrator%878225915.body
}

ClAdminAcceptor * ClLocalMonitor::CreateAdminAcceptor (m4uint16_t ai_iControlPort, m4uint32_t ai_iConnectionType)
{
  //## begin ClLocalMonitor::CreateAdminAcceptor%887707939.body preserve=yes
	ClCmdArguments oArguments ;
	ClAdminAcceptor * poAdminAcceptor ;


	oArguments.SetNArgument ( ai_iControlPort ) ;

	oArguments.SetNArgument ( ai_iConnectionType ) ;

	if ( m_oInvoker.ExecuteCommand ( CREATEADMINACCEPTOR  , (void *& ) poAdminAcceptor , & oArguments ) == M4_ERROR )
	{
		return NULL ;
	}	

return poAdminAcceptor ;
  //## end ClLocalMonitor::CreateAdminAcceptor%887707939.body
}

m4return_t ClLocalMonitor::ShutDownAdministrator (ClCSAdministrator *ai_poAdministrator)
{
  //## begin ClLocalMonitor::ShutDownAdministrator%878225916.body preserve=yes
	m4return_t iRet;


	iRet = m_oInvoker.ExecuteCommand ( SHUTDOWNADMINISTRATOR  , (void *& ) ai_poAdministrator ) ;

return iRet ;
  //## end ClLocalMonitor::ShutDownAdministrator%878225916.body
}

ClControlQ * ClLocalMonitor::GetControlQueue ()
{
  //## begin ClLocalMonitor::GetControlQueue%882177110.body preserve=yes
	return ( & m_oControlQueue ) ;
  //## end ClLocalMonitor::GetControlQueue%882177110.body
}

ClStatistic * ClLocalMonitor::GetStats ()
{
  //## begin ClLocalMonitor::GetStats%890061477.body preserve=yes

	return  m_poLMStats;

  //## end ClLocalMonitor::GetStats%890061477.body
}

m4return_t ClLocalMonitor::StartUp ()
{
  //## begin ClLocalMonitor::StartUp%894962381.body preserve=yes
	m_oLauncherList.StartUp ( ) ;
	m_oSessionManagerList.StartUp ( ) ;

	return M4_SUCCESS ;
  //## end ClLocalMonitor::StartUp%894962381.body
}

ClLocalMonitor * ClLocalMonitor::Instance ()
{
  //## begin ClLocalMonitor::Instance%895738253.body preserve=yes
	if ( ! m_poLocalMonitor )
	{
		m_poLocalMonitor = new ClLocalMonitor ;
	}

	return m_poLocalMonitor ;
  //## end ClLocalMonitor::Instance%895738253.body
}

ClSessionDic * ClLocalMonitor::GetSessionDic ()
{
  //## begin ClLocalMonitor::GetSessionDic%901531332.body preserve=yes
	return & m_oSessionDic ;
  //## end ClLocalMonitor::GetSessionDic%901531332.body
}

void ClLocalMonitor::SetStats (ClLMStats *ai_poStats)
{
  //## begin ClLocalMonitor::SetStats%901796097.body preserve=yes
	m_poLMStats = ai_poStats ;
  //## end ClLocalMonitor::SetStats%901796097.body
}

void ClLocalMonitor::Synchronize ()
{
  //## begin ClLocalMonitor::Synchronize%936613830.body preserve=yes
  //## end ClLocalMonitor::Synchronize%936613830.body
}

m4return_t ClLocalMonitor::DestroyConnectionId (m4uint32_t ai_IdConnection)
{
  //## begin ClLocalMonitor::DestroyConnectionId%962784433.body preserve=yes
	ClSystem * poSystem = NULL ;
	ClSSInterfaz* poSubsystem ; 
	ClUSSessionInterface * poSession = NULL ;
	ClUSUserInterface * poUser = NULL ;
	ClParamList * poParamList ;
	ClConnInstance * poConnection ;
	m4return_t iRet = M4_SUCCESS ;
	m4uint64_t lIdSession = 0,lIdUser = 0;
	ClSSInvoker * poInvoker = ClSSInvoker::GetInstance () ;

	//bg 105060
	m_oConDictionary.GetUserAndSessionId ( ai_IdConnection , lIdUser , lIdSession );
	
	if ( lIdSession )
	{
		poInvoker -> GetParamList ( poParamList  ) ;
							
		poParamList -> SetInt64 ( "SESSIONID" , (m4int64_t) lIdSession) ;
							
		poInvoker -> ExecuteAction ( CANCEL_USER_REQUEST_ACTION , poParamList  ) ;;	


		poSystem = ClSystem::Instance () ;
		
		if ( ! ( poSubsystem = poSystem -> FindSubsystemByType  ( M4_USERS_SUBSYSTEM ) ) )
		{
			SETCODEF ( M4_SRV_SS_TREE_ERROR , ERRORLOG , "Cannot find Users subsystem for terminate session" ) ;
			
			return M4_ERROR ;
		}
		
		poInvoker -> GetParamList ( poParamList  ) ;
		poParamList -> SetDouble ( "SESSIONID" , (m4int64_t) lIdSession ) ;
		poParamList -> SetDouble ( "USERID" , (m4int64_t) lIdUser ) ;
		
		if ( M4_ERROR == poInvoker -> ExecuteAction ( END_USER_SESSION_ACTION , poParamList ) )
			//		if ( ( ( ClUsersSS * ) (poSubsystem -> GetSubsystem()) ) -> EndUserSession ( lIdSession ) == M4_ERROR )
		{
			iRet = M4_ERROR ;
		}
		
		poInvoker -> SetParamList ( *poParamList  ) ;

	}
	else 
	{
		if ( ! ( poConnection = (ClConnection*) m_oConDictionary.GetItem ( ai_IdConnection ) ) )
		{
			M4_ASSERT ("Connexión no encontrada en diccionario") ;
			return M4_ERROR;
		}

		//Esta comprobación es para evitar que si se está procesando una autentificación 
		//la sesión se quede huérfana de conexión
		if ( poConnection -> m_oNumRequest.GetValue() == 0 )
		{
			return RemoveConnection ( poConnection ) ;
		}
	}

	return iRet;
  //## end ClLocalMonitor::DestroyConnectionId%962784433.body
}

//## begin module%3426894B01AE.epilog preserve=yes
//## end module%3426894B01AE.epilog


// Detached code regions:
// WARNING: this code will be lost if code is regenerated.
#if 0
//## begin ClLocalMonitor::GetOutputQueue%882177111.body preserve=no
  return ( & m_oOutputQueue ) ;
//## end ClLocalMonitor::GetOutputQueue%882177111.body

//## begin ClLocalMonitor::GetReceiveQueue%882177112.body preserve=no
  return  ( & m_oReceiveQueue ) ;
//## end ClLocalMonitor::GetReceiveQueue%882177112.body

#endif
