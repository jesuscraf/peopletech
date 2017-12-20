//## begin module%396C358F0215.cm preserve=no
//## end module%396C358F0215.cm

//## begin module%396C358F0215.cp preserve=no
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
//## end module%396C358F0215.cp

//## Module: adminactions%396C358F0215; Package body
//## Subsystem: M4Dispatcher::src%3923BBFB0332
//## Source file: F:\integration\m4dispatcher\src\adminactions.cpp

//## begin module%396C358F0215.additionalIncludes preserve=no
//## end module%396C358F0215.additionalIncludes

//## begin module%396C358F0215.includes preserve=yes
#include <adminagent.hpp>
#include <dspinvoker.hpp>
#include <m4xmlstreamgen.hpp>
#include <actionidentifiers.hpp>
#include <dspxmlreq.hpp>
#include <basiclog.hpp>
#include <m4dpchres.hpp>
#include <m4dispatchermain.hpp>
#include <m4cryptc.hpp>

#include <m4memdbg.h>
#include <m4srvtrace.h>

#define M4_SRV_TRACE_COMP_ID "DPCH"

//## end module%396C358F0215.includes

// adminactions
#include <adminactions.hpp>
// m4condition
#include <m4condition.hpp>
// clbaseaction
#include <clbaseaction.hpp>
//## begin module%396C358F0215.declarations preserve=no
//## end module%396C358F0215.declarations

//## begin module%396C358F0215.additionalDeclarations preserve=yes
extern m4uint32_t g_iDspTrace ; 
//## end module%396C358F0215.additionalDeclarations


#define	M4CH_DUMMY_B5	"\x8\xA\x5\xB\xE\xF\x2\xD\xE\x5\x4\x4\xC\x8\xB\xE\xA\xD\xF\xE\xD\xC\x8\xD\xB\xC\x4\x8\x4\x3\x4\xD\xF\xE\xB\xB\x4\xB\x5\xB\xF\x1\xA\xD\x3\xB\x6\xA\x3\xD\xF\x3\x6\x1\xD\x6\x2\x8\xB\x3\x6\x2\xF\x0\xD\x5\x8\xD\xB\xA\x3\x3\x7\x9\x1\xE\x4\x5\x2\xF\xC\x2\xD\xA\x7\xD\xB\x8\x5\xA\x2\x7\xA\xF\x1\xF\xD\xA\x6\x6\x4\xD\xE\x8\x0\x3\x5\xF\x1\xB\x5\x7\x9\x7\xC\x7\xB\xB\xF\x8\xF\xF\xC\x5\x2\xA\xF\xB\x9"

// Class ClStartConfigAction 

ClStartConfigAction::ClStartConfigAction (ClBaseActionExecutive *ai_poExecutive)
  //## begin ClStartConfigAction::ClStartConfigAction%962179778.hasinit preserve=no
  //## end ClStartConfigAction::ClStartConfigAction%962179778.hasinit
  //## begin ClStartConfigAction::ClStartConfigAction%962179778.initialization preserve=yes
  :ClDspAuthenticatedAction ( ai_poExecutive ) 
  //## end ClStartConfigAction::ClStartConfigAction%962179778.initialization
{
  //## begin ClStartConfigAction::ClStartConfigAction%962179778.body preserve=yes
  //## end ClStartConfigAction::ClStartConfigAction%962179778.body
}


//## Other Operations (implementation)
m4return_t ClStartConfigAction::Execute ()
{
  //## begin ClStartConfigAction::Execute%962179779.body preserve=yes

	// first of all check security
	if (CheckAuthentication() != M4_SUCCESS)
	{
		return M4_SUCCESS;
	}

//	m4pchar_t szBuffer ;
	ClSiteNodeInformation	* poNodeInfo ;
	ClDispatcherInvoker		* poInvoker = ClDispatcherInvoker::Instance () ;
	ClSiteNodeContainer		oSiteNodeContainer ;
	ClStarXMLAnswerGen		oXMLGen  ;
	void					* poRequest ;
	m4return_t				iRet = M4_SUCCESS ;
	m4pchar_t				pcCrypt = NULL;
	m4int_t					iLenOut ;
	m4uint32_t				iBasePort = 0 ;
	m4pchar_t				strhost =NULL, baseport =NULL, configuration =NULL, user =NULL, password =NULL;




	m4bool_t bFileOut = g_iDspTrace > 0 ? M4_TRUE : M4_FALSE ;

	M4_SRV_GENERAL_BLOCK( Blk, "Action %0:s", START_SERVER ) ;



	if (M4_ERROR == m_pParamList -> GetString ( "host" , strhost ) )
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOADMINAGENTSTARTCONFIGPARAM, 
			"Error starting Server Configuration. Cannot get the attribute %1:s.", 
			"Host Name");			
		return M4_ERROR ;
	}

	ToLower ( strhost) ;


	if (M4_ERROR == m_pParamList -> GetString ( "user" , user ) )
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOADMINAGENTSTARTCONFIGPARAM, 
			"Error starting Server Configuration. Cannot get the attribute %1:s.", 
			"User Name");			

		return M4_ERROR ;
	}

	if ( M4_ERROR == m_pParamList -> GetPointer ( "REQUEST" , poRequest ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOSTARTADMONREQUEST, 
			"Error starting server configuration. Cannot get the administration request.");			

		return M4_ERROR ;
	}

	if (M4_ERROR == m_pParamList -> GetString ( "configuration" , configuration ) )
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOADMINAGENTSTARTCONFIGPARAM, 
			"Error starting Server Configuration. Cannot get the attribute %1:s.", 
			"Configuration");			
		return M4_ERROR ;
	}

	ToLower ( configuration ) ;

	if (M4_SUCCESS == m_pParamList -> GetString ( "baseport" , baseport) )
	{
		iBasePort = atoi ( baseport ) ;
	}

	if (M4_ERROR == m_pParamList -> GetString ( "password" , password ) )
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOADMINAGENTSTARTCONFIGPARAM, 
			"Error starting Server Configuration. Cannot get the attribute %1:s.", 
			"Password");			

		return M4_ERROR ;
	}

	if ( M4_SUCCESS == EncryptTwoWaySecondKey ( password, strlen ( password), M4CH_DUMMY_B5 + 23, strlen ( M4CH_DUMMY_B5 + 23 ), pcCrypt , iLenOut ) )
	{
		m_pParamList -> DeleteParam ( "password" ) ;
		m_pParamList -> SetString ( "password" , pcCrypt ) ;
	}

	M4_MDBG_NEW( poNodeInfo, poNodeInfo = new ClSiteNodeInformation (configuration , strhost, iBasePort , user , pcCrypt ) );

	// Borrar el buffer de la password encriptada.
	delete pcCrypt;

	if (0 == m_poExecutive)
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOSTARTCONFIGACTIONEXECUTOR,
			"Error executing start configuration action. No executor assigned to the action.");

		iRet = M4_ERROR;
	}
	else
	{

		if ( M4_ERROR == ( ( ClAdminAgent *) m_poExecutive) -> StartConfiguration (m_pParamList , *poNodeInfo) )
		{
			iRet = M4_ERROR ;
		}
	}


	oSiteNodeContainer.AddNode ( *poNodeInfo ) ;

	( (ClDspXMLRequest *) poRequest )  ->SetResult ( iRet ) ;

	if ( M4_ERROR == ( (ClDspXMLRequest *) poRequest ) -> BuildReqReply ( oSiteNodeContainer , &oXMLGen ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOSTARTADMONREQUESTREPLY,
			"Error starting server configuration. Cannot make administration request reply.");			

		return M4_ERROR ;
	}


	//----- Asignación de parámetros-----------------
	ClParamList * poParamList = NULL ;

	M4_MDBG_NEW(  poParamList, poParamList = new ClParamList ) ;

	if ( ! ( (ClDspXMLRequest *) poRequest ) ->GetConnectionID () )
	{
		return M4_ERROR ;
	}

	if ( M4_ERROR == poParamList -> SetPointer ( "REQUEST" , poRequest ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOEXECUTESTARTADMONREQUEST,
			"Error starting server configuration. Cannot execute the administration request.");			
		
		return	M4_ERROR ;
	}



	return poInvoker -> ExecuteAction ( REPLY_XML , poParamList ) ;
  //## end ClStartConfigAction::Execute%962179779.body
}

// Additional Declarations
  //## begin ClStartConfigAction%3959C00100E8.declarations preserve=yes
  //## end ClStartConfigAction%3959C00100E8.declarations

// Class ClStopConfigAction 

ClStopConfigAction::ClStopConfigAction (ClBaseActionExecutive *ai_poExecutive)
  //## begin ClStopConfigAction::ClStopConfigAction%962179776.hasinit preserve=no
  //## end ClStopConfigAction::ClStopConfigAction%962179776.hasinit
  //## begin ClStopConfigAction::ClStopConfigAction%962179776.initialization preserve=yes
  :ClDspAuthenticatedAction ( ai_poExecutive ) 
  //## end ClStopConfigAction::ClStopConfigAction%962179776.initialization
{
  //## begin ClStopConfigAction::ClStopConfigAction%962179776.body preserve=yes
  //## end ClStopConfigAction::ClStopConfigAction%962179776.body
}



//## Other Operations (implementation)
m4return_t ClStopConfigAction::Execute ()
{
  //## begin ClStopConfigAction::Execute%962179777.body preserve=yes

	// first of all check security
	if (CheckAuthentication() != M4_SUCCESS)
	{
		return M4_SUCCESS;
	}

//	m4pchar_t szBuffer ;
	m4int_t					iParam ;
	ClSiteNodeInformation	oNodeInfo ;
	ClSiteNodeContainer		oNodeContainer ;
	void					* poRequest ;
	m4return_t				iRet = M4_SUCCESS ;
	ClDispatcherInvoker		* poInvoker = ClDispatcherInvoker::Instance () ;
	m4pchar_t				configuration = NULL, password =NULL;
	m4pchar_t				pcCrypt = NULL;
	m4int_t					iLenOut ;


	m4bool_t bFileOut = g_iDspTrace > 0 ? M4_TRUE : M4_FALSE ;

	ClStopXMLAnswerGen oXMLGen  ;

	M4_SRV_GENERAL_BLOCK( Blk, "Action %0:s", STOP_SERVER ) ;


	if ( M4_ERROR == m_pParamList -> GetPointer ( "REQUEST" , poRequest ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOSTOPADMONREQUEST,
			"Error stopping server configuration. Cannot get the administration request." );			

		return M4_ERROR ;
	}

	if (M4_ERROR == m_pParamList -> GetString ( "configuration" , configuration ) )
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOADMINAGENTSTARTCONFIGPARAM, 
			"Error starting Server Configuration. Cannot get the attribute %1:s.", 
			"Configuration");			
		return M4_ERROR ;
	}

	ToLower ( configuration ) ;

	if (M4_ERROR == m_pParamList -> GetString ( "password" , password ) )
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOADMINAGENTSTARTCONFIGPARAM, 
			"Error starting Server Configuration. Cannot get the attribute %1:s.", 
			"Password");			

		return M4_ERROR ;
	}

	if ( M4_SUCCESS == EncryptTwoWaySecondKey ( password, strlen ( password), M4CH_DUMMY_B5 + 23, strlen ( M4CH_DUMMY_B5 + 23 ), pcCrypt , iLenOut ) )
	{
		m_pParamList -> DeleteParam ( "password" ) ;
		m_pParamList -> SetString ( "password" , pcCrypt ) ;
		delete pcCrypt;
	}

	if(0 == m_poExecutive)
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOSTOPCONFIGACTIONEXECUTOR,
			"Error executing stop configuration action. No executor assigned to the action." );

		iRet = M4_ERROR;
	}
	else
	{
		if ( M4_ERROR == ( ( ClAdminAgent *) m_poExecutive) -> StopConfiguration (m_pParamList , oNodeInfo) )
		{
			iRet = M4_ERROR ;
		}
	}

	oNodeContainer.AddNode ( oNodeInfo ) ;

	( (ClDspXMLRequest *) poRequest ) -> SetResult ( iRet ) ;

	if ( M4_ERROR == ( (ClDspXMLRequest *) poRequest ) -> BuildReqReply (  oNodeContainer , &oXMLGen  ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOSTOPADMONREQUESTREPLY,
			"Error stopping server configuration. Cannot make administration request reply.");			
		
		return M4_ERROR ;
	}


	//----- Asignación de parámetros-----------------
	ClParamList * poParamList = NULL ;

	M4_MDBG_NEW(  poParamList, poParamList = new ClParamList ) ;

	if ( M4_SUCCESS == m_pParamList -> GetInt ( "SESSIONID" , iParam ) )
	{
		poParamList -> SetInt ( "SESSIONID" , iParam) ;
	}

	if ( M4_SUCCESS == m_pParamList -> GetInt ( "XMLID" ,  iParam ) )
	{
		poParamList -> SetInt ( "XMLID" ,  iParam ) ;
	}

	if ( M4_ERROR == poParamList -> SetPointer ( "REQUEST" , poRequest ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOEXCUTESTOPADMONREQUEST,
			"Error stopping server configuration. Cannot execute the administration request.");			

		return	M4_ERROR ;
	}



	return poInvoker -> ExecuteAction ( REPLY_XML , poParamList ) ;

  //## end ClStopConfigAction::Execute%962179777.body
}

// Additional Declarations
  //## begin ClStopConfigAction%3959C00D029E.declarations preserve=yes
  //## end ClStopConfigAction%3959C00D029E.declarations

ClRemoveConfigAction::ClRemoveConfigAction (ClBaseActionExecutive *ai_poExecutive)  
  :ClDspAuthenticatedAction ( ai_poExecutive )   
{
}

m4return_t ClRemoveConfigAction::Execute ()
{
	// first of all check security
	if (CheckAuthentication() != M4_SUCCESS)
	{
		return M4_SUCCESS;
	}

	m4return_t iRet = M4_SUCCESS ;
	void * poRequest = NULL ;
	m4pchar_t pcConfig =NULL, pcHost =NULL ;	
	ClSiteNodeInformation oNodeInfo ;
	ClSiteNodeContainer oNodeContainer ;
	ClParamList * poParamList = NULL ;
	m4int_t iParam ;
	
	ClDispatcherInvoker * poInvoker = ClDispatcherInvoker::Instance ( ) ;

	m4bool_t bFileOut = g_iDspTrace > 0 ? M4_TRUE : M4_FALSE ;

	ClRemoveXMLAnswerGen oXMLGen ;

	M4_SRV_GENERAL_BLOCK ( Blk, "Action %0:s", REMOVE_SERVER ) ;

	if ( M4_ERROR == m_pParamList -> GetPointer ( "REQUEST" , poRequest ) )
	{
		M4_SRV_ERROR ( M4_ERR_DPCH_REMOVECFGNOPROPERTY, "Error removing server configuration. Couln't get the attribute %0:s.", "Request" ) ;		

		return M4_ERROR ;
	}

	if ( M4_ERROR == m_pParamList -> GetString ( "configuration" , pcConfig ) )
	{
		M4_SRV_ERROR ( M4_ERR_DPCH_REMOVECFGNOPROPERTY,  "Error removing server configuration. Couln't get the attribute %0:s.",  "Configuration ID" ) ;		

		return M4_ERROR ;
	}

	ToLower ( pcConfig ) ;

	if ( M4_ERROR == m_pParamList -> GetString ( "host" , pcHost ) )
	{
		M4_SRV_ERROR (M4_ERR_DPCH_REMOVECFGNOPROPERTY, "Error removing server configuration. Couln't get the attribute %0:s.",  "Host Name" ) ;		

		return M4_ERROR ;
	}

	if ( 0 == m_poExecutive )
	{
		M4_SRV_ERROR_N ( M4_ERR_DPCH_REMOVECFGNOEXECUTOR, "Error executing remove configuration action. No executor assigned to the action." ) ;		

		iRet = M4_ERROR ;
	}
	else
	{
		if ( M4_ERROR == ( ( ClAdminAgent *) m_poExecutive) -> RemoveConfiguration ( m_pParamList , oNodeInfo ) )
		{
			iRet = M4_ERROR ;
		}
	}

	oNodeContainer.AddNode ( oNodeInfo ) ;

	( (ClDspXMLRequest *) poRequest ) -> SetResult ( iRet ) ;

	if ( M4_ERROR == ( (ClDspXMLRequest *) poRequest ) -> BuildReqReply (  oNodeContainer , &oXMLGen  ) )
	{
		M4_SRV_ERROR_N ( M4_ERR_DPCH_REMOVECFGNOREPLYREQUEST,  "Error removing server configuration. Cannot make administration request reply." ) ;		
		
		return M4_ERROR ;
	}
		
	M4_MDBG_NEW (  poParamList, poParamList = new ClParamList );

	if ( M4_SUCCESS == m_pParamList -> GetInt ( "SESSIONID" , iParam ) )
	{
		poParamList -> SetInt ( "SESSIONID" , iParam) ;
	}

	if ( M4_SUCCESS == m_pParamList -> GetInt ( "XMLID" ,  iParam ) )
	{
		poParamList -> SetInt ( "XMLID" ,  iParam ) ;
	}

	if ( M4_ERROR == poParamList -> SetPointer ( "REQUEST" , poRequest ) )
	{
		M4_SRV_ERROR_N ( M4_ERR_DPCH_NOEXCUTESTOPADMONREQUEST, "Error removing server configuration. Cannot execute the administration request.") ;

		return	M4_ERROR ;
	}

	iRet = poInvoker -> ExecuteAction ( REPLY_XML , poParamList ) ;

	return iRet ;
}


// Class ClCloseConnectionAdminAction 


ClCloseConnectionAdminAction::ClCloseConnectionAdminAction (ClBaseActionExecutive *ai_poExecutive)  
  :ClBaseAction ( ai_poExecutive )   
{
}

m4return_t ClCloseConnectionAdminAction::Execute ()
{
	ClAdminAgent *poAgent = ( ClAdminAgent *) m_poExecutive;

	// retrieves the connection id
	m4int_t iConnectionId = -1;
	if ( M4_ERROR == m_pParamList -> GetInt ( "SESSIONID" ,  iConnectionId ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOADMONREQUEST,
			"Error getting connection id." );			
		return M4_ERROR ;
	}

	// ok, lets free entry related to this connection
	poAgent->ReleaseConnection(iConnectionId);

	return M4_SUCCESS;
}

// Class ClCheckConsistencyAction 

ClCheckConsistencyAction::ClCheckConsistencyAction (ClBaseActionExecutive *ai_poExecutive)
  //## begin ClCheckConsistencyAction::ClCheckConsistencyAction%963302391.hasinit preserve=no
  //## end ClCheckConsistencyAction::ClCheckConsistencyAction%963302391.hasinit
  //## begin ClCheckConsistencyAction::ClCheckConsistencyAction%963302391.initialization preserve=yes
  :ClDspAuthenticatedAction ( ai_poExecutive ) 
  //## end ClCheckConsistencyAction::ClCheckConsistencyAction%963302391.initialization
{
  //## begin ClCheckConsistencyAction::ClCheckConsistencyAction%963302391.body preserve=yes
  //## end ClCheckConsistencyAction::ClCheckConsistencyAction%963302391.body
}



//## Other Operations (implementation)
m4return_t ClCheckConsistencyAction::Execute ()
{
  //## begin ClCheckConsistencyAction::Execute%963302392.body preserve=yes

	M4_SRV_DETAILED_BLOCK( Blk, "Action %0:s", CHECK_CONSISTENCY ) ;

	if (m_poExecutive)
	{
		((ClAdminAgent*) m_poExecutive ) -> CheckConsistency() ;
		return M4_SUCCESS ;
	}
	else
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOCONSISTENCYEXECUTORASSIGNED,
			"Error executing consistency action. No executor assigned to the action." );			

		return M4_ERROR ;
	}
  //## end ClCheckConsistencyAction::Execute%963302392.body
}

// Class ClDspAuthenticatedAction 

ClDspAuthenticatedAction::ClDspAuthenticatedAction (ClBaseActionExecutive *ai_poExecutive)
  :ClBaseAction ( ai_poExecutive, M4_TRUE ) 
{
	// normally the authentication action closes the client connection if not authenticated
	m_bAutoClose = M4_TRUE;

	// normally we have to authenticate
	m_bHaveToAuthenticate = M4_TRUE;
}

m4return_t ClDspAuthenticatedAction::CheckAuthentication()
{
	// checks if we have to authenticate
	if (m_bHaveToAuthenticate == M4_FALSE)
	{
		return M4_SUCCESS;
	}

	ClAdminAgent *poAgent = ( ClAdminAgent *) m_poExecutive;

	// checks if the environment must be secure
	if (poAgent->CheckHaveToAuthenticate() == M4_FALSE)
	{
		return M4_SUCCESS;
	}

	// retrieves the request
	void *poRef = NULL;
	ClDspXMLRequest *poRequest = NULL;
	if ( M4_ERROR == m_pParamList -> GetPointer ( "REQUEST" , poRef ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOADMONREQUEST,
			"Error getting administration request. Cannot get the request." );			
		return M4_ERROR ;
	}
	poRequest = (ClDspXMLRequest*)poRef;

	// ok, lets check if the connection is already connected
	if (poAgent->CheckAuthConnection(poRequest->GetConnectionID(), NULL))
	{
		return M4_SUCCESS;
	}

	// gets the request token attribute
	m4pchar_t pcToken = NULL;
	m4uint32_t iConnectionId = poRequest->GetConnectionID();
	if ( M4_ERROR == m_pParamList -> GetString ( "token" , pcToken ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOADMONREQUEST,
			"Error getting administration request. Cannot get the request." );			
		ReplyAuthError(iConnectionId);
		return M4_ERROR ;
	}

	// ok, lets check if the connection is already connected
	if (poAgent->CheckAuthConnection(iConnectionId, pcToken) == M4_FALSE)
	{
		// the token is not valid
		ReplyAuthError(iConnectionId);
		return M4_ERROR;
	}

	return M4_SUCCESS;
}


m4return_t ClDspAuthenticatedAction::ReplyAuthError(m4uint32_t ai_iConnectionId)
{

	// ok, lets check if we should close the connection
	ClParamList * poParamList = NULL ;

	ClDispatcherInvoker * poInvoker = ClDispatcherInvoker::Instance () ;

	M4_MDBG_NEW(  poParamList, poParamList = new ClParamList ) ;

	// closes the connection
	if (m_bAutoClose)
	{
		poParamList -> SetInt ( "SESSIONID" , ai_iConnectionId ) ;
		poParamList -> SetInt ( "STOP_LISTENING_FALG" , 0 ) ;
		
		// Cerramos la conexión.
		return poInvoker -> ExecuteAction ( CLOSE_CONNECTION , poParamList ) ;
	}

	if (! poInvoker)
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOCONFIGQUERYEXECUTORASSIGNED,
			"Error executing configuration query action. No executor assigned to the action." );			

		return M4_ERROR ;
	}

	void * poRequest  = NULL;
	if ( M4_ERROR == m_pParamList -> GetPointer ( "REQUEST" , poRequest ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOADMONREQUEST,
			"Error getting administration request. Cannot get the request." );			

		return M4_ERROR ;
	}

	ClSiteNodeContainer oConfigurationList ;
	ClAuthXMLAnswerGen oXMLGen ;

	poParamList -> SetPointer ( "REQUEST" , poRequest ) ;

	( (ClDspXMLRequest *) poRequest ) -> SetResult ( M4_SUCCESS ) ;

	if ( M4_ERROR == ( (ClDspXMLRequest *) poRequest ) -> BuildReqReply (  oConfigurationList , &oXMLGen ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOADMONREQUESTREPLY,
			"Error getting administration request reply. Cannot make the reply." );			

		return M4_ERROR ;
	}

	return poInvoker-> ExecuteAction ( REPLY_XML , poParamList ) ;
}

// End Class ClDspAuthenticatedAction 

// Additional Declarations
  //## begin ClCheckConsistencyAction%396AEE8D01A3.declarations preserve=yes
  //## end ClCheckConsistencyAction%396AEE8D01A3.declarations

// Class ClConfigurationsQueryAction 

ClConfigurationsQueryAction::ClConfigurationsQueryAction (ClBaseActionExecutive *ai_poExecutive)
  //## begin ClConfigurationsQueryAction::ClConfigurationsQueryAction%961054360.hasinit preserve=no
  //## end ClConfigurationsQueryAction::ClConfigurationsQueryAction%961054360.hasinit
  //## begin ClConfigurationsQueryAction::ClConfigurationsQueryAction%961054360.initialization preserve=yes
  :ClDspAuthenticatedAction ( ai_poExecutive ) 
  //## end ClConfigurationsQueryAction::ClConfigurationsQueryAction%961054360.initialization
{
  //## begin ClConfigurationsQueryAction::ClConfigurationsQueryAction%961054360.body preserve=yes
	// this action will not auto close the connection
	// all other, normally, will auto close
	m_bAutoClose = M4_FALSE;
  //## end ClConfigurationsQueryAction::ClConfigurationsQueryAction%961054360.body
}

//## Other Operations (implementation)
m4return_t ClConfigurationsQueryAction::Execute ()
{
  //## begin ClConfigurationsQueryAction::Execute%961054361.body preserve=yes

	// first of all check security
	if (CheckAuthentication() != M4_SUCCESS)
	{
		return M4_SUCCESS;
	}

	ClSiteNodeContainer oConfigurationList ;
	m4string_t xml ;
	m4string_t params = "" ;
	void * poRequest ;
	ClParamList * poParamList = NULL ;
	m4return_t iRet = M4_SUCCESS ;

	M4_SRV_GENERAL_BLOCK( Blk, "Action %0:s", CONFIGURATIONS_QUERY ) ;

	M4_MDBG_NEW(  poParamList, poParamList = new ClParamList ) ;

	ClDispatcherInvoker * poInvoker = ClDispatcherInvoker::Instance () ;


	if (! poInvoker)
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOCONFIGQUERYEXECUTORASSIGNED,
			"Error executing configuration query action. No executor assigned to the action." );			

		return M4_ERROR ;
	}

	if ( M4_ERROR == m_pParamList -> GetPointer ( "REQUEST" , poRequest ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOADMONREQUEST,
			"Error getting administration request. Cannot get the request." );			

		// CAMBIO: Eliminar este return y hacer que devuelva un XML especial
		// por defecto
		return M4_ERROR ;
	}

	m4bool_t bFileOut = g_iDspTrace > 0 ? M4_TRUE : M4_FALSE ;

	ClConfigurationsXMLAnswerGen oXMLGen ;


	if ( M4_ERROR == ( ( ClAdminAgent *) m_poExecutive) -> GetConfigurationList (oConfigurationList) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOCONFIGURATIONLIST,
			"Error getting Server configuration list. Cannot get configuration list." );			

		// CAMBIO: Eliminar este return y hacer que devuelva un XML especial
		// por defecto
		iRet = M4_ERROR ;
	}


	poParamList -> SetPointer ( "REQUEST" , poRequest ) ;

	( (ClDspXMLRequest *) poRequest ) -> SetResult ( iRet ) ;

	if ( M4_ERROR == ( (ClDspXMLRequest *) poRequest ) -> BuildReqReply (  oConfigurationList , &oXMLGen ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOADMONREQUESTREPLY,
			"Error getting administration request reply. Cannot make the reply." );			

		return M4_ERROR ;
	}

	return poInvoker-> ExecuteAction ( REPLY_XML , poParamList ) ;

  //## end ClConfigurationsQueryAction::Execute%961054361.body
}

// Additional Declarations
  //## begin ClConfigurationsQueryAction%3948E5F50347.declarations preserve=yes
  //## end ClConfigurationsQueryAction%3948E5F50347.declarations

// Class ClStopDpchAction 


ClStopDpchAction::ClStopDpchAction (ClBaseActionExecutive *ai_poExecutive)
  //## begin ClStopDpchAction::ClStopDpchAction%973007655.hasinit preserve=no
  //## end ClStopDpchAction::ClStopDpchAction%973007655.hasinit
  //## begin ClStopDpchAction::ClStopDpchAction%973007655.initialization preserve=yes
  :ClDspAuthenticatedAction ( ai_poExecutive ) 
  //## end ClStopDpchAction::ClStopDpchAction%973007655.initialization
{
  //## begin ClStopDpchAction::ClStopDpchAction%973007655.body preserve=yes
  //## end ClStopDpchAction::ClStopDpchAction%973007655.body
}



//## Other Operations (implementation)
m4return_t ClStopDpchAction::Execute ()
{
  //## begin ClStopDpchAction::Execute%973007656.body preserve=yes

	// first of all check security
	if (CheckAuthentication() != M4_SUCCESS)
	{
		return M4_SUCCESS;
	}

	ClStopDpchAnswerGen oXMLGen ;
	//m4return_t iRet ;
	ClDspXMLRequest * poRequest ;
	ClSiteNodeContainer oSiteNodeContainer ;
	ClParamList * poParamList ;
	ClDispatcherInvoker * poInvoker = ClDispatcherInvoker::Instance () ;
	m4char_t *StopSrv=NULL ;
	m4bool_t SwStopSrv=M4_FALSE ;

	if ( M4_ERROR == m_pParamList->GetString ( "stopservers" , StopSrv ) ) {
		M4_SRV_WARNING(0, "Action %0:s, missing parameter %1:s, setting to default value : false",
			 STOP_DPCH << "stopservers"  ) ;
		SwStopSrv	= M4_FALSE ;
	} 
	else 
	{
		SwStopSrv	= !(StopSrv[0]=='N'||StopSrv[0]=='n' ) ;
	} ;

	if (SwStopSrv) 
	{
	
		ClSiteNodeRepository *Rep = ClSiteRepositorySingleton::GetInstance() ;
		ClSiteNodeContainer List ;
		ClSiteNodeInformation Info, InfoAux ;
		ClParamList ParamList ;	//No alocar, StopConfiguration solo lo usa

		Rep->GetNodeList(List) ;

		while (  M4_ERROR != List.GetNextNode(Info) ) 
		{

			ParamList.Clear() ;
			ParamList.SetString( "host"				, Info.GetNodeHost().c_str() ) ; 
			ParamList.SetString( "configuration"	, Info.GetSiteNodeID().c_str() ) ; 
			ParamList.SetString( "user"				, Info.GetUserName().c_str() ) ; 
			ParamList.SetString( "password"			, Info.GetPassword().c_str() ) ; 
			ParamList.SetString( "load"				, "0"  ) ; 
			( ( ClAdminAgent *) m_poExecutive) -> StopConfiguration(&ParamList, InfoAux) ;
		} ;

	} ;



	M4_SRV_GENERAL_BLOCK( Blk, "Action %0:s", STOP_DPCH ) ;


	if ( M4_ERROR == m_pParamList -> GetPointer ( "REQUEST" , (void *&) poRequest ) )
	{
		return M4_ERROR ;
	}

	if ( M4_ERROR == poRequest -> BuildReqReply ( oSiteNodeContainer , &oXMLGen ) )
	{
		return M4_ERROR ;
	}

	M4_MDBG_NEW(  poParamList, poParamList = new ClParamList ) ;

	poParamList -> SetPointer ( "REQUEST" , poRequest ) ;

	poInvoker -> ExecuteAction ( REPLY_XML , poParamList ) ;

	poInvoker -> ExecuteAction ( SIGNAL_DPCH , m_pParamList ) ;

	return M4_SUCCESS;
	
  //## end ClStopDpchAction::Execute%973007656.body
}

// Additional Declarations
  //## begin ClStopDpchAction%39FEE8550055.declarations preserve=yes
  //## end ClStopDpchAction%39FEE8550055.declarations

// Class ClSingalAction 

ClSingalAction::ClSingalAction (ClBaseActionExecutive *ai_poExecutive)
  //## begin ClSingalAction::ClSingalAction%991756196.hasinit preserve=no
  //## end ClSingalAction::ClSingalAction%991756196.hasinit
  //## begin ClSingalAction::ClSingalAction%991756196.initialization preserve=yes
  :ClDspAuthenticatedAction ( ai_poExecutive ) 
  //## end ClSingalAction::ClSingalAction%991756196.initialization
{
  //## begin ClSingalAction::ClSingalAction%991756196.body preserve=yes
  //## end ClSingalAction::ClSingalAction%991756196.body
}



//## Other Operations (implementation)
m4return_t ClSingalAction::Execute ()
{
  //## begin ClSingalAction::Execute%991756197.body preserve=yes

	// first of all check security
	if (CheckAuthentication() != M4_SUCCESS)
	{
		return M4_SUCCESS;
	}

	ClDpchMainSingleton * poDpchMainSingleton = ClDpchMainSingleton::Instance();
	ClDpchMain * poDpchMain = poDpchMainSingleton->GetDpchMain();

	

	poDpchMain->StopDispatcher();
	
	return M4_SUCCESS ;
  //## end ClSingalAction::Execute%991756197.body
}

// Additional Declarations
  //## begin ClSingalAction%3B1CFF3301A9.declarations preserve=yes
  //## end ClSingalAction%3B1CFF3301A9.declarations

// Class ClRegisterTargetAction 

ClRegisterTargetAction::ClRegisterTargetAction (ClBaseActionExecutive *ai_poExecutive)
  //## begin ClRegisterTargetAction::ClRegisterTargetAction%991912264.hasinit preserve=no
  //## end ClRegisterTargetAction::ClRegisterTargetAction%991912264.hasinit
  //## begin ClRegisterTargetAction::ClRegisterTargetAction%991912264.initialization preserve=yes
  :ClDspAuthenticatedAction ( ai_poExecutive ) 
  //## end ClRegisterTargetAction::ClRegisterTargetAction%991912264.initialization
{
  //## begin ClRegisterTargetAction::ClRegisterTargetAction%991912264.body preserve=yes
	
	ClAdminAgent *poExecutive = ( ClAdminAgent *) ai_poExecutive;

	// we have to authenticate if we are NOT in initializing mode
	m_bHaveToAuthenticate = (poExecutive->IsInitializing() == M4_FALSE);
  //## end ClRegisterTargetAction::ClRegisterTargetAction%991912264.body
}



//## Other Operations (implementation)
m4return_t ClRegisterTargetAction::Execute ()
{
  //## begin ClRegisterTargetAction::Execute%991912265.body preserve=yes

	// first of all check security
	if (CheckAuthentication() != M4_SUCCESS)
	{
		return M4_SUCCESS;
	}

	ClSiteNodeInformation oNodeInfo ;
	ClDispatcherInvoker * poInvoker = ClDispatcherInvoker::Instance () ;
	ClSiteNodeContainer oSiteNodeContainer ;
	ClStarXMLAnswerGen oXMLGen  ;
	//void * poRequest ;
	m4return_t iRet = M4_SUCCESS ;


	M4_SRV_GENERAL_BLOCK( Blk, "Action %0:s", START_SERVER ) ;

/*
	if ( M4_ERROR == m_pParamList -> GetPointer ( "REQUEST" , poRequest ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOSTARTADMONREQUEST, 
			"Error starting server configuration. Cannot get the administration request.");			

		return M4_ERROR ;
	}
*/
	if (0 == m_poExecutive)
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOSTARTCONFIGACTIONEXECUTOR,
			"Error executing start configuration action. No executor assigned to the action.");

		iRet = M4_ERROR;
	}
	else
	{

		if ( M4_ERROR == ( ( ClAdminAgent *) m_poExecutive) -> RegisterTargetNode (m_pParamList ) )
		{
			iRet = M4_ERROR ;
		}
	}



	return iRet ;
  //## end ClRegisterTargetAction::Execute%991912265.body
}

// Additional Declarations
  //## begin ClRegisterTargetAction%3B1F60F60138.declarations preserve=yes
ClUpdateAppCtlConfigAction ::ClUpdateAppCtlConfigAction (ClBaseActionExecutive *ai_poExecutive)
  :ClDspAuthenticatedAction ( ai_poExecutive ) 
{
}


m4return_t ClUpdateAppCtlConfigAction::Execute ()
{

	m4pchar_t szConfig = NULL ;
	m4pchar_t szhost = NULL ;
	m4int_t iPort ;

	if ( M4_ERROR == m_pParamList -> GetString  ( "configuration", szConfig ) )
	{
		return M4_ERROR ;
	}
	if ( M4_ERROR == m_pParamList -> GetString  ( "M4AppCtl" , szhost ) )
	{
		return M4_ERROR ;
	}
	if ( M4_ERROR == m_pParamList -> GetInt  ( "port" , iPort) )
	{
		return M4_ERROR ;
	}

	 
	return M4_SUCCESS ;
	
}


  //## end ClRegisterTargetAction%3B1F60F60138.declarations

//## begin module%396C358F0215.epilog preserve=yes
//## end module%396C358F0215.epilog
