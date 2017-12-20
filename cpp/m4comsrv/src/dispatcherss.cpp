//## begin module%38F5A2790024.cm preserve=no
//## end module%38F5A2790024.cm

//## begin module%38F5A2790024.cp preserve=no
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
//## end module%38F5A2790024.cp

//## Module: dispatcherss%38F5A2790024; Package body
//## Subsystem: version::src%379ECBD7029A
//## Source file: D:\Work\v600\m4comsrv\src\dispatcherss.cpp

//## begin module%38F5A2790024.additionalIncludes preserve=no
//## end module%38F5A2790024.additionalIncludes

//## begin module%38F5A2790024.includes preserve=yes
#include <request.hpp>
#include <clssinvoker.hpp>
#include <pdus.hpp>
#include <usinterface.hpp>
#include <m4srvres.hpp>
#include <cloblconfiguration.hpp>
#include <subsystems.hpp>
#include <clservicebasess.hpp>
#include <m4credentials.hpp>
#include <m4actionconstants.hpp>
#include <connects.hpp>
#include "m4regimpl.hpp"
//## end module%38F5A2790024.includes

// clssaction
#include <clssaction.hpp>
// ClConfiguration
#include <clconfiguration.hpp>
// dispatcherss
#include <dispatcherss.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>


//## begin module%38F5A2790024.declarations preserve=no
//## end module%38F5A2790024.declarations

//## begin module%38F5A2790024.additionalDeclarations preserve=yes
//## end module%38F5A2790024.additionalDeclarations


// Class ClDispatcherSS 


ClDispatcherSS::ClDispatcherSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator)
  //## begin ClDispatcherSS::ClDispatcherSS%955610152.hasinit preserve=no
  //## end ClDispatcherSS::ClDispatcherSS%955610152.hasinit
  //## begin ClDispatcherSS::ClDispatcherSS%955610152.initialization preserve=yes
  :ClSSImplementation (ai_poConfiguratoin, ai_poTables, ai_poNavigator )
  //## end ClDispatcherSS::ClDispatcherSS%955610152.initialization
{
  //## begin ClDispatcherSS::ClDispatcherSS%955610152.body preserve=yes
	ClSSInvoker * poInvoker = ClSSInvoker::GetInstance ( ) ;
	
	poInvoker -> RegisterAction ( DISPATCH_REQUEST_ACTION , DISPATCH_REQUEST_ACTION_ID , this ) ;
  //## end ClDispatcherSS::ClDispatcherSS%955610152.body
}



//## Other Operations (implementation)
m4return_t ClDispatcherSS::CreateAction (m4pchar_t ai_szActionType, ClBaseAction *&ao_poAction)
{
  //## begin ClDispatcherSS::CreateAction%955610153.body preserve=yes
	if ( ! stricmp ( ai_szActionType , DISPATCH_REQUEST_ACTION ) )
	{
		ao_poAction = new ClDispatchRequestAction ( this ) ;

		return M4_SUCCESS ;
	}

	return M4_ERROR ;
  //## end ClDispatcherSS::CreateAction%955610153.body
}

ClSSImplementation * ClDispatcherSS::FindService (ClRequest *ai_poRequest)
{
  //## begin ClDispatcherSS::FindService%955610154.body preserve=yes
	return NULL ;
  //## end ClDispatcherSS::FindService%955610154.body
}

m4return_t ClDispatcherSS::ProcessItem (ClBaseRequest *ai_oItem)
{
  //## begin ClDispatcherSS::ProcessItem%955610156.body preserve=yes
	m4uint32_t iTypePDU =  ( ((ClRequest*)ai_oItem ) -> GetInputPDU () ) -> GetTypePDU ();
	ClRequestStatistics	* pReqStats ;
	ClUSSessionInterface *pCurrentSession ;
	ClUSUserInterface *poUser ;
	m4return_t ret = M4_SUCCESS ;
//	ClSSImplementation * poService ;
	ClSSInvoker * poInvoker = ClSSInvoker::GetInstance () ;
	m4char_t szPhisicalService [ OBL_PROPERTY_MAX_LEN ] ;
	ClParamList * poParamList  ;
//	ClConnInstance * poConnection ;
	m4string_t strUserID = "";
	ClLogSystem * poLog ;
	m4string_t szServiceID;


	pReqStats = (ClRequestStatistics	*)ai_oItem ->GetStats();

	if ( M4_ERROR != RecoverSessionFromRequest ((ClRequest * )ai_oItem , pCurrentSession ) )
	{
		((ClRequest * )ai_oItem ) -> SetSession ( pCurrentSession );

		if ( poUser = pCurrentSession -> GetUser () )
		{
			poUser -> GetUserId ( strUserID ) ;
		}
	}

	((ClRequest * )ai_oItem ) -> Initialize ( strUserID ) ;

	pReqStats->Sett ( ClRequestStatistics::Registering );

	poInvoker -> GetParamList ( poParamList  ) ;
	poParamList -> SetPointer ( "REQUEST" , ai_oItem ) ;

	poLog = ((ClRequest*)ai_oItem ) ->GetLog();

	ret = poInvoker ->ExecuteAction ( REGISTER_REQUEST_ACTION , poParamList ) ;

	if ( ret == M4_ERROR )
	{
		m4char_t szError [100];
		sprintf ( szError , "Error Registering Request ID: %d" , (m4uint32_t) ai_oItem -> GetId () );
		poLog -> SetCode ( M4_SRV_LM_REGISTER_REQUEST , ERRORLOG , szError ) ;
		ret = poInvoker ->ExecuteAction ( DEREGISTER_REQUEST_ACTION , poParamList ) ;
		poInvoker -> SetParamList ( *poParamList  ) ;
		return M4_ERROR ;
	}

	poInvoker -> SetParamList ( *poParamList  ) ;

/* AL RECUPERAR LA SESIÓN YA SE LE HA AÑADIDO LA REQUEST
	if ( pCurrentSession  )
	{
		pCurrentSession -> AddRequest () ;
	}
*/
	switch ( iTypePDU ) 
	{

			case M4_TYPE_PDU_SERVICE:

				// Acabamos a leer una PDU de tipo Servicio , Actualizamos las estad.

				pReqStats->Sett(ClRequestStatistics::GettingService);

				szServiceID = ((ClRequest*) ai_oItem  ) ->GetServiceId() ;

				if ( NULL != pCurrentSession )
				{
					if ( M4_SUCCESS == (ret = GetService ( (char*)szServiceID.c_str() , szPhisicalService , OBL_PROPERTY_MAX_LEN ) ) )
					{	

						poInvoker -> GetParamList ( poParamList  ) ;
						poParamList -> SetPointer ( "REQUEST" , ai_oItem ) ;
						poParamList -> SetDouble ( "SESSIONID" , (m4int64_t) pCurrentSession -> GetId ()  );

						ret = poInvoker ->ExecuteAction ( szPhisicalService , poParamList ) ;

						poInvoker -> SetParamList ( *poParamList  ) ;

						if ( M4_ERROR == ret)
						{
							m4string_t str = "Service PDU (" + szServiceID +") did not found service " + szPhisicalService ;
							
							poLog -> SetCode ( M4_SRV_LM_SERVICE_REQUEST , ERRORLOG , str.c_str() ) ;
							
							((ClRequest*) ai_oItem  ) -> SetError ( M4_ERROR ) ;
							
							
							ret = M4_ERROR ;
						}
					}
					else
					{
						m4string_t str = "Service PDU (" + szServiceID +"). Not service in server (in SERVICES_TABLE section) defined for that client service identifier " ;
						poLog -> SetCode ( M4_SRV_LM_SERVICE_REQUEST , ERRORLOG , str.c_str() ) ;
						ret = M4_ERROR ;
					}
				}
				else 
				{
					// Bug 0108041.
					/* Bug 0147755
					Hay que unificar el error con el de pérdida de conexión
					*/
					
					// bg 168309
					m4string_t str = "Sesión de usuario desconectada.\n \nEs posible que la sesión haya sido cerrada por la expiración del tiempo máximo de inactividad." ;
					poLog -> SetCode( M4_SRV_LM_NO_SESSION_AVAILABLE_ERROR, ERRORLOG, str.c_str() ) ;

					ret = M4_ERROR ;
				}


				break ;

			case M4_TYPE_PDU_CONTROL:

				pReqStats->Sett(ClRequestStatistics::InServiceQ);

				poInvoker -> GetParamList ( poParamList  ) ;
				poParamList -> SetPointer ( "REQUEST" , ai_oItem ) ;

				ret = poInvoker -> ExecuteAction ( CONTROL_REQUEST_ACTION , poParamList) ;

				poInvoker -> SetParamList ( *poParamList  ) ;

				break;

			default:

				// XXX. PDU Incorrecta
				SETCODEF ( M4_SRV_ERROR_IN_PDU , ERRORLOG , "%s" , ""/*pActiveTSAP -> Dump ()*/ ); 

				ret = M4_ERROR;

				break;
			}

	return ret ;
  //## end ClDispatcherSS::ProcessItem%955610156.body
}

m4return_t ClDispatcherSS::ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList)
{
  //## begin ClDispatcherSS::ExecuteCommand%955610157.body preserve=yes
	if (ai_cCommand < ClSSImplementation::TOTAL_COMMANDS)
		return ClSSImplementation::ExecuteCommand(ai_cCommand, ai_oParamList, ao_oResultList);
	
    SETCODEF ( M4_SRV_SS_COMMAND_PARAM_ERROR , ERRORLOG , "DISPATCHER subsystem: Invalid Administrator List SubSystem command Id: %d", ai_cCommand );
    return M4_ERROR;

	/*
    m4return_t iResult = M4_SUCCESS;
	ClAdminTagList::iterator it;
	m4uint32_t iSessionId = 0;
	m4int_t iDebugLevel = 0;
	
    switch (ai_cCommand)
	{
	default:
		SETCODEF ( M4_SRV_SS_COMMAND_PARAM_ERROR , ERRORLOG , "DISPATCHER subsystem: Invalid Administrator List SubSystem command Id: %d", ai_cCommand );
		return M4_ERROR;
	}
	
	return iResult;
    */
  //## end ClDispatcherSS::ExecuteCommand%955610157.body
}

m4return_t ClDispatcherSS::GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList)
{
  //## begin ClDispatcherSS::GetCommandParams%955610158.body preserve=yes
	// checks if the command is a base class command.
	if(ai_cCommand < ClSSImplementation::TOTAL_COMMANDS)
		return ClSSImplementation::GetCommandParams (ai_cCommand, aio_oParamList);
	
	// builds the parameters list.
    SETCODEF ( M4_SRV_SS_COMMAND_PARAM_ERROR , ERRORLOG , "DISPATCHER subsystem: Invalid Administrator List SubSystem command Id: %d", ai_cCommand );
    return M4_ERROR;

    /*
    ClAdminCommandTag *newParam = NULL;
	switch (ai_cCommand)
	{
		
	default:
		SETCODEF ( M4_SRV_SS_COMMAND_PARAM_ERROR , ERRORLOG , "DISPATCHER subsystem: Invalid Administrator List SubSystem command Id: %d", ai_cCommand );
		return M4_ERROR;
	}
	return M4_SUCCESS;
    */
  //## end ClDispatcherSS::GetCommandParams%955610158.body
}

m4return_t ClDispatcherSS::GetCommandsId (ClAdminTagList &aio_oCommandList)
{
  //## begin ClDispatcherSS::GetCommandsId%955610159.body preserve=yes
	if (ClSSImplementation::GetCommandsId(aio_oCommandList) != M4_SUCCESS)
		return M4_ERROR;
	
	//---------------------------------------------------------------
	// insert commands for this subsystem
	//---------------------------------------------------------------

	ClAdminTag *newCommand = NULL;
	

	return  M4_SUCCESS;
  //## end ClDispatcherSS::GetCommandsId%955610159.body
}

m4return_t ClDispatcherSS::Initialize ()
{
  //## begin ClDispatcherSS::Initialize%955610160.body preserve=yes
	return M4_SUCCESS ;
  //## end ClDispatcherSS::Initialize%955610160.body
}

m4return_t ClDispatcherSS::ShutDown ()
{
  //## begin ClDispatcherSS::ShutDown%955610161.body preserve=yes
	return ClSSImplementation::ShutDown () ;
  //## end ClDispatcherSS::ShutDown%955610161.body
}

m4return_t ClDispatcherSS::Start ()
{
  //## begin ClDispatcherSS::Start%955610162.body preserve=yes
	return ClSSImplementation::Start () ;
  //## end ClDispatcherSS::Start%955610162.body
}

ClSSInterfaz * ClDispatcherSS::GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables)
{
  //## begin ClDispatcherSS::GenerateSubsystem%955610163.body preserve=yes

	switch ( ai_lType )
	{
	case M4_SERVICE_TABLE :

		m_poServicesTable = ai_poConfiguration ;

		break ;
	}

	return NULL ;
  //## end ClDispatcherSS::GenerateSubsystem%955610163.body
}

m4return_t ClDispatcherSS::GetService (m4pchar_t ai_szLogicalService, m4pchar_t ao_szPhisicalService, m4size_t ai_iLenBuffer)
{
  //## begin ClDispatcherSS::GetService%955610164.body preserve=yes
	m4size_t iLen;


	return m_poServicesTable -> GetStringValue ( "" , ai_szLogicalService, ao_szPhisicalService , ai_iLenBuffer, iLen ) ;
  //## end ClDispatcherSS::GetService%955610164.body
}

m4return_t ClDispatcherSS::ProcessAction (ClActionIdentifier ai_oActionIdentifier, ClParamList *ai_poParamList, ClBaseAction *&ao_poAction)
{
  //## begin ClDispatcherSS::ProcessAction%957943671.body preserve=yes


	switch ( ai_oActionIdentifier ) 
	{
	case DISPATCH_REQUEST_ACTION_ID :

		ao_poAction = new ClDispatchRequestAction ( this ) ;

		break ;
	}

	ao_poAction -> SetParams ( ai_poParamList ) ;

	return ao_poAction -> Execute ( ) ;
  //## end ClDispatcherSS::ProcessAction%957943671.body
}

m4return_t ClDispatcherSS::RecoverSessionFromRequest (ClRequest *ai_poRequest, ClUSSessionInterface *&ao_poSession)
{
  //## begin ClDispatcherSS::RecoverSessionFromRequest%967533894.body preserve=yes
	M4Credentials * poCredential ;
	ClSSInvoker * poInvoker = ClSSInvoker::GetInstance () ;
	ClConnInstance * poConnection ;
	ClParamList * poParamList ;
	m4return_t ret = M4_ERROR ;
	m4int64_t idSession ;
	
	if ( ao_poSession = ai_poRequest ->GetpSession () )
	{
		return M4_SUCCESS ;
	}

	if ( poConnection = ai_poRequest ->GetpConnection () )
	{
		if ( M4_CONNECTION_ORIENTED_PROTOCOL == poConnection -> GetConnProtocol( ) )
		{
			if ( ao_poSession = ((ClConnection*)poConnection ) -> GetMainSession () )
			{
				ao_poSession -> AddRequest () ;

				return M4_SUCCESS ;
			}
		}
	}

	if ( poCredential = ai_poRequest -> GetCredential () )
	{
		poInvoker -> GetParamList ( poParamList  ) ;

		idSession = ai_poRequest -> GetSessionId () ;

		poParamList ->SetPointer ( "CREDENTIAL" , poCredential ) ;
		poParamList ->SetInt64 ( "SESSIONID" , idSession ) ;

		if ( M4_SUCCESS == poInvoker ->ExecuteAction ( RECOVER_SESSION_ACTION , poParamList ) )
		{
			ret = poParamList -> GetPointer ( "SESSION" , (void*&) ao_poSession ) ;
		}
		poInvoker -> SetParamList ( *poParamList  ) ;

	}

	return ret ;

  //## end ClDispatcherSS::RecoverSessionFromRequest%967533894.body
}

// Additional Declarations
  //## begin ClDispatcherSS%38F5A17003DA.declarations preserve=yes
  //## end ClDispatcherSS%38F5A17003DA.declarations

// Class ClDispatchRequestAction 

ClDispatchRequestAction::ClDispatchRequestAction (ClSSImplementation *ai_poExecutive)
  //## begin ClDispatchRequestAction::ClDispatchRequestAction%955440328.hasinit preserve=no
  //## end ClDispatchRequestAction::ClDispatchRequestAction%955440328.hasinit
  //## begin ClDispatchRequestAction::ClDispatchRequestAction%955440328.initialization preserve=yes
  :ClSSAction ( ai_poExecutive )
  //## end ClDispatchRequestAction::ClDispatchRequestAction%955440328.initialization
{
  //## begin ClDispatchRequestAction::ClDispatchRequestAction%955440328.body preserve=yes
  //## end ClDispatchRequestAction::ClDispatchRequestAction%955440328.body
}



//## Other Operations (implementation)
m4return_t ClDispatchRequestAction::Execute ()
{
  //## begin ClDispatchRequestAction::Execute%955440329.body preserve=yes
	ClBaseRequest * poRequest ;
	void * poItem ;

	
	if ( M4_ERROR == m_pParamList -> GetPointer ( "REQUEST" , poItem ) )
	{
		return M4_ERROR ;
	}

	poRequest = (ClBaseRequest*) poItem ;

	if ( ! poRequest ) 
	{
		return M4_ERROR ;
	}

	 return ((ClDispatcherSS * )  m_poExecutive ) -> ProcessItem ( poRequest ) ;
  //## end ClDispatchRequestAction::Execute%955440329.body
}

// Additional Declarations
  //## begin ClDispatchRequestAction%38F3505201C1.declarations preserve=yes
  //## end ClDispatchRequestAction%38F3505201C1.declarations

//## begin module%38F5A2790024.epilog preserve=yes
//## end module%38F5A2790024.epilog
