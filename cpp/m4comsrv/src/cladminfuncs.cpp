//## begin module%34267D2502BB.cm preserve=no
//## end module%34267D2502BB.cm

//## begin module%34267D2502BB.cp preserve=no
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
//## end module%34267D2502BB.cp

//## Module: ClAdminFuncs%34267D2502BB; Package body
//## Subsystem: version::src%379ECBD7029A
//## Source file: F:\integration\m4comsrv\src\cladminfuncs.cpp

//## begin module%34267D2502BB.additionalIncludes preserve=no
//## end module%34267D2502BB.additionalIncludes

//## begin module%34267D2502BB.includes preserve=yes
#include <m4win.hpp>
#include <gettsap.hpp>
#include <m4srvres.hpp>
#include <m4trace.hpp>
#include <tsap.hpp>
#include <clserviceinfo.hpp>
#include <usinterface.hpp>
#include <sessmanager.hpp>
#include <navigatorss.hpp>
#include <controlpipesmkr.hpp>
#include <clsubsystem.hpp>
#include <cladministrator.hpp>
#include <connects.hpp>
#include <clcommand.hpp>
#include <fifoq.hpp>
#include <cllocalmonitor.hpp>
#include <cladminfuncs.hpp>

class ClUSSession;
class ClRequest;
class ClLauncherInterface;

//## begin module%34267D2502BB.declarations preserve=no
//## end module%34267D2502BB.declarations

//## begin module%34267D2502BB.additionalDeclarations preserve=yes
//## end module%34267D2502BB.additionalDeclarations


// Class ClCreateService 

ClCreateService::ClCreateService (void* ai_manager)
  //## begin ClCreateService::ClCreateService%875000095.hasinit preserve=no
  //## end ClCreateService::ClCreateService%875000095.hasinit
  //## begin ClCreateService::ClCreateService%875000095.initialization preserve=yes
							: ClCommand( ai_manager )
  //## end ClCreateService::ClCreateService%875000095.initialization
{
  //## begin ClCreateService::ClCreateService%875000095.body preserve=yes
  //## end ClCreateService::ClCreateService%875000095.body
}


ClCreateService::~ClCreateService ()
{
  //## begin ClCreateService::~ClCreateService%875000096.body preserve=yes
  //## end ClCreateService::~ClCreateService%875000096.body
}



//## Other Operations (implementation)
m4return_t ClCreateService::Execute (void*& ao_oReturn, ClCmdArguments* ai_Arguments)
{
  //## begin ClCreateService::Execute%878114635.body preserve=yes
	m4int32_t lIdService = 1 ;
	m4int16_t iItemQ ;
	ClLocalMonitor * poLocalMonitor ;
	ClServiceInfo * poService ;
//	ADD	EMN	990524
	
	poLocalMonitor = ClLocalMonitor::Instance ( ) ;

	if ( M4_ERROR == ( lIdService = ai_Arguments -> GetNArgument( ) ) )
	{
		SETCODEF ( M4_SRV_LM_ADD_SERVICE_ON_LINE , ERRORLOG , "Could not create a new service. Bad argument for service ID" ) ;

		return M4_ERROR ;
	}

	iItemQ = ( m4int16_t ) ai_Arguments -> GetNArgument( ) ;

	if  ( ( ! iItemQ ) || ( iItemQ == M4_ERROR ) )
	{
		poService = poLocalMonitor -> m_oServiceDictionary.AddServiceOnLine ( lIdService ,  poLocalMonitor  ) ;
	}
	else
	{
		poService = poLocalMonitor -> m_oServiceDictionary.AddServiceOnLine ( lIdService ,  poLocalMonitor  , iItemQ ) ;
	}

	if (poService == NULL )
	{
		SETCODEF ( M4_SRV_LM_ADD_SERVICE_ON_LINE , ERRORLOG ,"Was not possible to atartup the service %d" , lIdService ) ;

		 return M4_ERROR ;
	}


	 ao_oReturn = poService ;


	return M4_SUCCESS;
  //## end ClCreateService::Execute%878114635.body
}

// Additional Declarations
  //## begin ClCreateService%3426525D0230.declarations preserve=yes
  //## end ClCreateService%3426525D0230.declarations

// Class ClNewConnection 

ClNewConnection::ClNewConnection (void* ai_manager)
  //## begin ClNewConnection::ClNewConnection%875095490.hasinit preserve=no
  //## end ClNewConnection::ClNewConnection%875095490.hasinit
  //## begin ClNewConnection::ClNewConnection%875095490.initialization preserve=yes
							: ClCommand( ai_manager )
  //## end ClNewConnection::ClNewConnection%875095490.initialization
{
  //## begin ClNewConnection::ClNewConnection%875095490.body preserve=yes
  //## end ClNewConnection::ClNewConnection%875095490.body
}


ClNewConnection::~ClNewConnection ()
{
  //## begin ClNewConnection::~ClNewConnection%875095491.body preserve=yes
  //## end ClNewConnection::~ClNewConnection%875095491.body
}



//## Other Operations (implementation)
m4return_t ClNewConnection::Execute (void*& ao_oReturn, ClCmdArguments* ai_Arguments)
{
  //## begin ClNewConnection::Execute%878114640.body preserve=yes
	ClConnInstance * poConnection = NULL; 
	m4uint64_t  lIdConnection ;


	//Get the arguments	
	poConnection = ( ClConnInstance * ) (ai_Arguments -> GetPArgument ( ) ) ;

	if ( ! poConnection )
	{
		SETCODEF ( M4_SRV_LM_NEW_CONNECTION_ERROR , ERRORLOG , "New connection detected but no connection created" ) ;
		return M4_ERROR ;
	}
	//if the connection user propietary is not set
		//add a new entry in the users dictionary

	lIdConnection = poConnection -> GetId ( ) ;

	if ( M4_ERROR == ( ( ClLocalMonitor * ) m_manager ) -> m_oConDictionary.AddItem ( lIdConnection , poConnection ) )  
	{
		SETCODEF ( M4_SRV_LM_NEW_CONNECTION_ERROR , ERRORLOG , "Cannot insert new connection into connections dictionary" ) ;

		return M4_ERROR ;
	}


	return M4_SUCCESS;
  //## end ClNewConnection::Execute%878114640.body
}

// Additional Declarations
  //## begin ClNewConnection%3428E1EF032F.declarations preserve=yes
  //## end ClNewConnection%3428E1EF032F.declarations

// Class ClDownConnection 

ClDownConnection::ClDownConnection (void* ai_manager)
  //## begin ClDownConnection::ClDownConnection%875095494.hasinit preserve=no
  //## end ClDownConnection::ClDownConnection%875095494.hasinit
  //## begin ClDownConnection::ClDownConnection%875095494.initialization preserve=yes
 							: ClCommand( ai_manager )
  //## end ClDownConnection::ClDownConnection%875095494.initialization
{
  //## begin ClDownConnection::ClDownConnection%875095494.body preserve=yes
  //## end ClDownConnection::ClDownConnection%875095494.body
}


ClDownConnection::~ClDownConnection ()
{
  //## begin ClDownConnection::~ClDownConnection%875095495.body preserve=yes
  //## end ClDownConnection::~ClDownConnection%875095495.body
}



//## Other Operations (implementation)
m4return_t ClDownConnection::Execute (void*& ao_oReturn, ClCmdArguments* ai_Arguments)
{
  //## begin ClDownConnection::Execute%878114641.body preserve=yes
 	ClConnection * poConnection = NULL ;
	m4int64_t lIdConnection = 0, lIdUser = 0;
	m4return_t iRet = 0;




	poConnection =  ( ClConnection * ) ( ai_Arguments -> GetPArgument ( ) ) ;

	if ( iRet == M4_ERROR ) 
	{
		SETCODEF( M4_SRV_LM_BAD_ARGUMENT , ERRORLOG , " connectiton pointer argument is not corrected passed to ClDownConnection " ) ;

		return M4_ERROR ;
	}

	lIdConnection = poConnection -> GetId ( ) ;

	lIdUser = poConnection -> GetMainSession ()  -> GetUser ( ) ->GetId ( ) ;


	iRet = ( ( ClLocalMonitor * ) m_manager ) -> m_oConDictionary.RemoveConnection ( lIdConnection ) ;
	if ( iRet == M4_ERROR ) 
	{
		SETCODEF( M4_SRV_LM_DOWN_CONNECTION , ERRORLOG , "The dictionary down connection method failed" ) ;

		return M4_ERROR ;
	}

	return M4_SUCCESS;
  //## end ClDownConnection::Execute%878114641.body
}

// Additional Declarations
  //## begin ClDownConnection%3428E1FF0075.declarations preserve=yes
  //## end ClDownConnection%3428E1FF0075.declarations

// Class ClShutDownService 

ClShutDownService::ClShutDownService (void* ai_manager)
  //## begin ClShutDownService::ClShutDownService%877008205.hasinit preserve=no
  //## end ClShutDownService::ClShutDownService%877008205.hasinit
  //## begin ClShutDownService::ClShutDownService%877008205.initialization preserve=yes
							: ClCommand( ai_manager )
  //## end ClShutDownService::ClShutDownService%877008205.initialization
{
  //## begin ClShutDownService::ClShutDownService%877008205.body preserve=yes
  //## end ClShutDownService::ClShutDownService%877008205.body
}


ClShutDownService::~ClShutDownService ()
{
  //## begin ClShutDownService::~ClShutDownService%877008206.body preserve=yes
  //## end ClShutDownService::~ClShutDownService%877008206.body
}



//## Other Operations (implementation)
m4return_t ClShutDownService::Execute (void*& ao_oReturn, ClCmdArguments* ai_Arguments)
{
  //## begin ClShutDownService::Execute%878114643.body preserve=yes
	ClServiceInfo * poService ;
	m4int32_t lIdService ;


	lIdService = ai_Arguments -> GetNArgument ( ) ;

	poService = ( ClServiceInfo * ) ( (ClLocalMonitor * ) m_manager ) -> m_oServiceDictionary.GetItem ( lIdService ) ;
	
	poService -> Disable (  ) ;

	cout << "\nThe service " << lIdService  << " has been closed\n" << flush ;

return M4_SUCCESS ;
  //## end ClShutDownService::Execute%878114643.body
}

// Additional Declarations
  //## begin ClShutDownService%343B72EB03C3.declarations preserve=yes
  //## end ClShutDownService%343B72EB03C3.declarations

// Class ClCreateCM 

ClCreateCM::ClCreateCM (void* ai_manager)
  //## begin ClCreateCM::ClCreateCM%877008208.hasinit preserve=no
  //## end ClCreateCM::ClCreateCM%877008208.hasinit
  //## begin ClCreateCM::ClCreateCM%877008208.initialization preserve=yes
							: ClCommand( ai_manager )
  //## end ClCreateCM::ClCreateCM%877008208.initialization
{
  //## begin ClCreateCM::ClCreateCM%877008208.body preserve=yes
  //## end ClCreateCM::ClCreateCM%877008208.body
}


ClCreateCM::~ClCreateCM ()
{
  //## begin ClCreateCM::~ClCreateCM%877008209.body preserve=yes
  //## end ClCreateCM::~ClCreateCM%877008209.body
}



//## Other Operations (implementation)
m4return_t ClCreateCM::Execute (void*& ao_oReturn, ClCmdArguments* ai_Arguments)
{
  //## begin ClCreateCM::Execute%878114644.body preserve=yes
	ClCSSessManager * poSessManager = NULL ;
	ClSessManagerStats * poStats = NULL ;
//	ClSendQ * poSendQueue = NULL ;
	ClUsersSS		*pUsersSS = NULL;		// Como se inicializa?
	
	/*
	pUsersSS = ( ClUsersSS * ) ai_Arguments -> GetPArgument ( ) ; 

	poStats = new ClSessManagerStats ;

//	MOD	EMN	980508	poSessManager = new  ClCSSessManager ( (ClLocalMonitor*) m_manager ,( (ClLocalMonitor*) m_manager ) -> GetControlQueue ( ) , poSendQueue , poStats );
	poSessManager = new  ClCSSessManager ( (ClLocalMonitor*) m_manager ,( (ClLocalMonitor*) m_manager ) -> GetControlQueue ( ) , poStats );

	iRet = ( (ClLocalMonitor*) m_manager ) -> m_oSessionManagerList.AddItem ( poSessManager ) ; 
	
	if (iRet == M4_ERROR)
	{
		return M4_ERROR;
	}

	ao_oReturn = poSessManager ;

//	( ( ClLocalMonitor * ) m_manager ) -> GetStats() -> AddNumConnManagers (1 ) ;
*/
	return M4_SUCCESS;
  //## end ClCreateCM::Execute%878114644.body
}

// Additional Declarations
  //## begin ClCreateCM%344614680321.declarations preserve=yes
  //## end ClCreateCM%344614680321.declarations

// Class ClCreateLauncher 

ClCreateLauncher::ClCreateLauncher (void* ai_manager)
  //## begin ClCreateLauncher::ClCreateLauncher%878029383.hasinit preserve=no
  //## end ClCreateLauncher::ClCreateLauncher%878029383.hasinit
  //## begin ClCreateLauncher::ClCreateLauncher%878029383.initialization preserve=yes
							: ClCommand( ai_manager )
  //## end ClCreateLauncher::ClCreateLauncher%878029383.initialization
{
  //## begin ClCreateLauncher::ClCreateLauncher%878029383.body preserve=yes
  //## end ClCreateLauncher::ClCreateLauncher%878029383.body
}


ClCreateLauncher::~ClCreateLauncher ()
{
  //## begin ClCreateLauncher::~ClCreateLauncher%878029384.body preserve=yes
  //## end ClCreateLauncher::~ClCreateLauncher%878029384.body
}



//## Other Operations (implementation)
m4return_t ClCreateLauncher::Execute (void*& ao_oReturn, ClCmdArguments* ai_Arguments)
{
  //## begin ClCreateLauncher::Execute%878114645.body preserve=yes
	ClLauncherInterface * poLauncher = NULL;
	ClNavigatorFirewall * poNavigator = NULL ;
	ClConfiguration * poConfiguration = NULL ;
	ClLauncherStats * poStats = NULL ;
	ClServiceInfo * poService = NULL ;
	m4int32_t lIdService ;
//	static m4uint16_t iNumLauncher = 0 ;
	m4uint32_t iType ;
	ClServiceQ * poServiceQueue = NULL ;
	ClOutputQ * poOutputQ = NULL ;
	ClLog * poLog = NULL ;
	m4return_t iRet;
//	m4pchar_t szDllName ;
//	m4pchar_t szFuncName ;
//	M4ClLoaDll oLoadDll ;
//	m4typefunreturn pFunc ;
//	void ( * Create)(m4uint32_t, ClEngine *,ClExecutive *& ) ; 


	if ( M4_ERROR == ( lIdService = ai_Arguments -> GetNArgument ( ) ) )
	{
		SETCODEF ( M4_SRV_CREATE_LAUNCHER , ERRORLOG , "Bad argument as service id creting a new executor" ) ;

		return M4_ERROR ;
	}

	if ( M4_ERROR == ( iType = ( m4uint16_t ) ai_Arguments -> GetNArgument ( ) ) )
	{
		SETCODEF ( M4_SRV_CREATE_LAUNCHER , ERRORLOG , "Bad argument as executor type creating a new executor" ) ;

		return M4_ERROR ;
	}
/*
	if ( ! ( poNavigator = ( ClNavigatorFirewall * ) ai_Arguments -> GetPArgument ( ) ) )
	{
		SETCODEF ( M4_SRV_CREATE_LAUNCHER , ERRORLOG , "Bad argument as navigator creating a new executor" ) ;

		return M4_ERROR ;
	}

	if ( ! ( poConfiguration =  (ClConfiguration*) ai_Arguments -> GetPArgument ( ) ) )
	{
		SETCODEF ( M4_SRV_CREATE_LAUNCHER , ERRORLOG , "Bad argument as configuration, creating a new executor" ) ;

		return M4_ERROR ;
	}
*/
//	szDllName = ( m4pchar_t ) ai_Arguments -> GetPArgument ( ) ; 

//	szFuncName = ( m4pchar_t ) ai_Arguments -> GetPArgument ( ) ;


	if ( ! ( poService = (ClServiceInfo *)( ( ClLocalMonitor * ) m_manager )-> m_oServiceDictionary.GetItem ( lIdService ) ) )
	{
		SETCODEF ( M4_SRV_CREATE_LAUNCHER , ERRORLOG , "cannot find service %d, creating a new executor for that service" , lIdService) ;

		return M4_ERROR ;	
	}

	if ( ! ( poServiceQueue = (poService -> GetServiceQ ( ) ) ) )
	{
		SETCODEF ( M4_SRV_CREATE_LAUNCHER , ERRORLOG , "Service %d queue is NULL, creating a new executor for that service" , lIdService) ;

		return M4_ERROR ;
	}

/*	if ( ! ( poOutputQ  = ( ( ClLocalMonitor * ) m_manager ) -> GetOutputQueue ( ) ) )
	{
		SETCODEF ( M4_SRV_CREATE_LAUNCHER , ERRORLOG , "Server output queue is NULL, creating a new executor" ) ;

		return M4_ERROR ;
	}
*/
	poLog = & ( poService -> m_oLog ) ;

	poStats = new ClLauncherStats ;

	ClExeFactory::Create ( iType , poLauncher , poStats, poServiceQueue , NULL) ;

	if ( ! poLauncher )
	{
		SETCODEF ( M4_SRV_CREATE_LAUNCHER , ERRORLOG , "cannot find service %d, creating a new executor for that service" , lIdService) ;

		return M4_ERROR ;
	}

	iRet = ( poService -> m_oLauncherList ).AddItem ( poLauncher ) ;

	if  ( iRet == M4_ERROR ) 
	{
		return M4_ERROR ;
	}

	//El startup del ejecutor lo hace el new del launcher
	//poExecutive -> StartUp ( ) ;


//	if ( ( ( ClLocalMonitor * ) m_manager ) -> GetStats() ) 
//		( ( ClLocalMonitor * ) m_manager ) -> GetStats() -> AddNumLaunchers (1 ) ;

	ao_oReturn = poLauncher ;


	return M4_SUCCESS ;
  //## end ClCreateLauncher::Execute%878114645.body
}

// Additional Declarations
  //## begin ClCreateLauncher%34562B45017A.declarations preserve=yes
  //## end ClCreateLauncher%34562B45017A.declarations

// Class ClShutDownCM 

ClShutDownCM::ClShutDownCM (void* ai_manager)
  //## begin ClShutDownCM::ClShutDownCM%878114646.hasinit preserve=no
  //## end ClShutDownCM::ClShutDownCM%878114646.hasinit
  //## begin ClShutDownCM::ClShutDownCM%878114646.initialization preserve=yes
							: ClCommand( ai_manager )
  //## end ClShutDownCM::ClShutDownCM%878114646.initialization
{
  //## begin ClShutDownCM::ClShutDownCM%878114646.body preserve=yes
  //## end ClShutDownCM::ClShutDownCM%878114646.body
}


ClShutDownCM::~ClShutDownCM ()
{
  //## begin ClShutDownCM::~ClShutDownCM%878114647.body preserve=yes
  //## end ClShutDownCM::~ClShutDownCM%878114647.body
}



//## Other Operations (implementation)
m4return_t ClShutDownCM::Execute (void*& ao_oReturn, ClCmdArguments* ai_Arguments)
{
  //## begin ClShutDownCM::Execute%878114648.body preserve=yes
	m4return_t iRet ;



	iRet = ( ( ClLocalMonitor * ) m_manager ) -> m_oSessionManagerList.ShutDownItem ( ( M4Thread *) ao_oReturn ) ;

	if ( iRet == M4_ERROR ) 
	{
		SETCODEF ( M4_SRV_LM_SHUTING_DOWN_CONNMANAGER , ERRORLOG , "The Connection Manager couldn't be shut down" ) ;

		return M4_ERROR ;
	}

	cout << "\nOne Session Manager shut down\n" << flush ;

//	( ( ClLocalMonitor * ) m_manager ) -> GetStats() -> AddNumConnManagers ( -1 ) ;

	return M4_SUCCESS ;
  //## end ClShutDownCM::Execute%878114648.body
}

// Additional Declarations
  //## begin ClShutDownCM%3457003A0003.declarations preserve=yes
  //## end ClShutDownCM%3457003A0003.declarations

// Class ClShutDownLauncher 

ClShutDownLauncher::ClShutDownLauncher (void* ai_manager)
  //## begin ClShutDownLauncher::ClShutDownLauncher%878114658.hasinit preserve=no
  //## end ClShutDownLauncher::ClShutDownLauncher%878114658.hasinit
  //## begin ClShutDownLauncher::ClShutDownLauncher%878114658.initialization preserve=yes
							: ClCommand( ai_manager )
  //## end ClShutDownLauncher::ClShutDownLauncher%878114658.initialization
{
  //## begin ClShutDownLauncher::ClShutDownLauncher%878114658.body preserve=yes
  //## end ClShutDownLauncher::ClShutDownLauncher%878114658.body
}


ClShutDownLauncher::~ClShutDownLauncher ()
{
  //## begin ClShutDownLauncher::~ClShutDownLauncher%878114659.body preserve=yes
  //## end ClShutDownLauncher::~ClShutDownLauncher%878114659.body
}



//## Other Operations (implementation)
m4return_t ClShutDownLauncher::Execute (void*& ao_oReturn, ClCmdArguments* ai_Arguments)
{
  //## begin ClShutDownLauncher::Execute%878114660.body preserve=yes
	ClServiceInfo * poService = NULL ;
	m4int32_t lIdService ;
	m4bool_t bType = M4_FALSE ;
	m4return_t iRet; 


	lIdService = ai_Arguments -> GetNArgument ( ) ;

	if (  M4_TRUE && ( ai_Arguments -> GetNArgument ( ) ) )
		bType = M4_TRUE ;

	poService = ( ClServiceInfo *)( ( ClLocalMonitor * ) m_manager ) -> m_oServiceDictionary.GetItem( lIdService ) ;

	iRet = poService -> m_oLauncherList.ShutDownItem ( 0 ) ;

	if ( iRet == M4_ERROR ) 
	{
		SETCODEF ( M4_SRV_LM_SHUTING_DOWN_LAUNCHER , ERRORLOG , "The Launcher couldn't be shut down" ) ;

		return M4_ERROR ;
	}

	cout << "\nOne Launcher attached to service " << lIdService << " shut down\n" << flush ;
	
//	( ( ClLocalMonitor * ) m_manager ) -> GetStats() -> AddNumLaunchers ( -1 ) ;


	return M4_SUCCESS ;

  //## end ClShutDownLauncher::Execute%878114660.body
}

// Additional Declarations
  //## begin ClShutDownLauncher%345701E201A8.declarations preserve=yes
  //## end ClShutDownLauncher%345701E201A8.declarations

// Class ClCreateAdministrator 

ClCreateAdministrator::ClCreateAdministrator (void* ai_manager)
  //## begin ClCreateAdministrator::ClCreateAdministrator%878114670.hasinit preserve=no
  //## end ClCreateAdministrator::ClCreateAdministrator%878114670.hasinit
  //## begin ClCreateAdministrator::ClCreateAdministrator%878114670.initialization preserve=yes
							: ClCommand( ai_manager )
  //## end ClCreateAdministrator::ClCreateAdministrator%878114670.initialization
{
  //## begin ClCreateAdministrator::ClCreateAdministrator%878114670.body preserve=yes
  //## end ClCreateAdministrator::ClCreateAdministrator%878114670.body
}


ClCreateAdministrator::~ClCreateAdministrator ()
{
  //## begin ClCreateAdministrator::~ClCreateAdministrator%878114671.body preserve=yes
  //## end ClCreateAdministrator::~ClCreateAdministrator%878114671.body
}



//## Other Operations (implementation)
m4return_t ClCreateAdministrator::Execute (void*& ao_oReturn, ClCmdArguments* ai_Arguments)
{
  //## begin ClCreateAdministrator::Execute%878114672.body preserve=yes
	ClCSAdministrator * poAdministrator ;
	m4return_t iRet = M4_SUCCESS ;
	ClTSAP *poTSAP = NULL ;
	m4char_t szThreadName [20] ;
	static m4uint32_t IdThread = 1 ;
	ClNavigatorSS * poNavigator ;
	ClSystem * poSystem ;


	poSystem = ClSystem::Instance () ;

	poNavigator = new ClNavigatorSS ;

	poTSAP = ( ClTSAP * ) ai_Arguments -> GetPArgument ( ) ;

	poAdministrator = new ClCSAdministrator (  poTSAP , poNavigator , poSystem) ;

	iRet = ( (ClLocalMonitor * ) m_manager ) -> m_oAdministratorList.AddItem ( poAdministrator ) ;


	if ( iRet == M4_ERROR ) 
	{
		SETCODEF ( M4_SRV_LM_CREATING_ADMINISTRATOR, ERRORLOG , "The Administrator couldn't be created" ) ;

		return M4_ERROR ;
	}


	sprintf ( szThreadName , "Administrator %d" , IdThread ) ;

	poAdministrator -> Start ( szThreadName) ;

	IdThread ++ ;

	cout << "\nOne Administrator has started\n" << flush ;

//	( ( ClLocalMonitor * ) m_manager ) -> GetStats() -> AddNumAdministrators ( 1 ) ;


	return M4_SUCCESS ;
  //## end ClCreateAdministrator::Execute%878114672.body
}

// Additional Declarations
  //## begin ClCreateAdministrator%345787EE0242.declarations preserve=yes
  //## end ClCreateAdministrator%345787EE0242.declarations

// Class ClShutDownAdministrator 

ClShutDownAdministrator::ClShutDownAdministrator (void* ai_manager)
  //## begin ClShutDownAdministrator::ClShutDownAdministrator%878114673.hasinit preserve=no
  //## end ClShutDownAdministrator::ClShutDownAdministrator%878114673.hasinit
  //## begin ClShutDownAdministrator::ClShutDownAdministrator%878114673.initialization preserve=yes
							: ClCommand( ai_manager )
  //## end ClShutDownAdministrator::ClShutDownAdministrator%878114673.initialization
{
  //## begin ClShutDownAdministrator::ClShutDownAdministrator%878114673.body preserve=yes
  //## end ClShutDownAdministrator::ClShutDownAdministrator%878114673.body
}


ClShutDownAdministrator::~ClShutDownAdministrator ()
{
  //## begin ClShutDownAdministrator::~ClShutDownAdministrator%878114674.body preserve=yes
  //## end ClShutDownAdministrator::~ClShutDownAdministrator%878114674.body
}



//## Other Operations (implementation)
m4return_t ClShutDownAdministrator::Execute (void*& ao_oReturn, ClCmdArguments* ai_Arguments)
{
  //## begin ClShutDownAdministrator::Execute%878114675.body preserve=yes
	m4return_t iRet ;


	
	iRet = 	( ( ClLocalMonitor * ) m_manager ) -> m_oAdministratorList.ShutDownItem ( ( M4Thread *) ao_oReturn ) ;

	if ( iRet == M4_ERROR ) 
	{
		SETCODEF ( M4_SRV_LM_SHUTING_DOWN_ADMINISTRATOR , ERRORLOG , "The Administrator couldn't be shut down" ) ;
		return M4_ERROR ;
	}

	cout << "\nOne Administrator shut down\n" << flush ;
	
//	( ( ClLocalMonitor * ) m_manager ) -> GetStats() -> AddNumAdministrators ( -1 ) ;


	return M4_SUCCESS ;
  //## end ClShutDownAdministrator::Execute%878114675.body
}

// Additional Declarations
  //## begin ClShutDownAdministrator%34578814011A.declarations preserve=yes
  //## end ClShutDownAdministrator%34578814011A.declarations

//## begin module%34267D2502BB.epilog preserve=yes
//## end module%34267D2502BB.epilog


