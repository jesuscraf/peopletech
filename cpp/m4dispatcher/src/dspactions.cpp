//## begin module%3948844F002B.cm preserve=no
//## end module%3948844F002B.cm

//## begin module%3948844F002B.cp preserve=no
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
//## end module%3948844F002B.cp

//## Module: dspactions%3948844F002B; Package body
//## Subsystem: M4Dispatcher::src%3923BBFB0332
//## Source file: F:\integration\m4dispatcher\src\dspactions.cpp

//## begin module%3948844F002B.additionalIncludes preserve=no
//## end module%3948844F002B.additionalIncludes

//## begin module%3948844F002B.includes preserve=yes
#include <commadapter.hpp>
#include <adminagent.hpp>
#include <dspagent.hpp>
#include <clientagent.hpp>
#include <m4xmlstreamgen.hpp>
#include <dspinvoker.hpp>
#include <actionidentifiers.hpp>
#include <serveragent.hpp>
#include <basiclog.hpp>
#include <m4dpchres.hpp>
#include <sitenodeinfo.hpp>
#include <m4memdbg.h>
#include <m4srvtrace.h>

#define M4_SRV_TRACE_COMP_ID "DPCH"

//## end module%3948844F002B.includes

// dspactions
#include <dspactions.hpp>
// clbaseaction
#include <clbaseaction.hpp>
//## begin module%3948844F002B.declarations preserve=no
//## end module%3948844F002B.declarations

//## begin module%3948844F002B.additionalDeclarations preserve=yes
extern m4uint32_t g_iDspTrace ; 

#define M4_ERR_DPCH_CONFIG_PROPERTY 	0x006D + M4_DPCH_ERROR_BASE

//## end module%3948844F002B.additionalDeclarations


// Class ClPostReadPDUAction 

ClPostReadPDUAction::ClPostReadPDUAction (ClBaseActionExecutive *ai_poExecutive)
  //## begin ClPostReadPDUAction::ClPostReadPDUAction%961054339.hasinit preserve=no
  //## end ClPostReadPDUAction::ClPostReadPDUAction%961054339.hasinit
  //## begin ClPostReadPDUAction::ClPostReadPDUAction%961054339.initialization preserve=yes
  :ClBaseAction ( ai_poExecutive, M4_TRUE )
  //## end ClPostReadPDUAction::ClPostReadPDUAction%961054339.initialization
{
  //## begin ClPostReadPDUAction::ClPostReadPDUAction%961054339.body preserve=yes
  //## end ClPostReadPDUAction::ClPostReadPDUAction%961054339.body
}



//## Other Operations (implementation)
m4return_t ClPostReadPDUAction::Execute ()
{
  //## begin ClPostReadPDUAction::Execute%961054340.body preserve=yes

	M4_SRV_DEBUG_BLOCK( Blk, "Action %0:s", POST_READ_PDU_PROTOCOL_CONNECTION ) ;

	if (m_poExecutive)
	{
		( ( ClDptcherCommsAdaptor * ) m_poExecutive) ->PostReadClientPDU (m_pParamList);

		return M4_SUCCESS ;
	}
	else
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOPOSTREADPDUACTIONEXECUTOR,
			"Error executing post read PDU action.\nNo executor assigned to the action." );

		return M4_ERROR ;
	}
  //## end ClPostReadPDUAction::Execute%961054340.body
}

// Additional Declarations
  //## begin ClPostReadPDUAction%3947BCFC0324.declarations preserve=yes
  //## end ClPostReadPDUAction%3947BCFC0324.declarations

// Class ClCloseConnectionAction 

ClCloseConnectionAction::ClCloseConnectionAction (ClBaseActionExecutive *ai_poExecutive)
  //## begin ClCloseConnectionAction::ClCloseConnectionAction%963417631.hasinit preserve=no
  //## end ClCloseConnectionAction::ClCloseConnectionAction%963417631.hasinit
  //## begin ClCloseConnectionAction::ClCloseConnectionAction%963417631.initialization preserve=yes
  :ClBaseAction ( ai_poExecutive, M4_TRUE ) 
  //## end ClCloseConnectionAction::ClCloseConnectionAction%963417631.initialization
{
  //## begin ClCloseConnectionAction::ClCloseConnectionAction%963417631.body preserve=yes
  //## end ClCloseConnectionAction::ClCloseConnectionAction%963417631.body
}



//## Other Operations (implementation)
m4return_t ClCloseConnectionAction::Execute ()
{
  //## begin ClCloseConnectionAction::Execute%963417632.body preserve=yes

	M4_SRV_DEBUG_BLOCK( Blk, "Action %0:s", CLOSE_CONNECTION ) ;

	return ( (ClDptcherCommsAdaptor*) m_poExecutive ) -> CloseConnection (m_pParamList) ;
  //## end ClCloseConnectionAction::Execute%963417632.body
}

// Additional Declarations
  //## begin ClCloseConnectionAction%396C95CF03A6.declarations preserve=yes
  //## end ClCloseConnectionAction%396C95CF03A6.declarations

// Class ClUpdateConfigAction 

ClUpdateConfigAction::ClUpdateConfigAction (ClBaseActionExecutive *ai_poExecutive)
  //## begin ClUpdateConfigAction::ClUpdateConfigAction%962811787.hasinit preserve=no
  //## end ClUpdateConfigAction::ClUpdateConfigAction%962811787.hasinit
  //## begin ClUpdateConfigAction::ClUpdateConfigAction%962811787.initialization preserve=yes
  :ClBaseAction ( ai_poExecutive, M4_TRUE ) 
  //## end ClUpdateConfigAction::ClUpdateConfigAction%962811787.initialization
{
  //## begin ClUpdateConfigAction::ClUpdateConfigAction%962811787.body preserve=yes
  //## end ClUpdateConfigAction::ClUpdateConfigAction%962811787.body
}



//## Other Operations (implementation)
m4return_t ClUpdateConfigAction::Execute ()
{
  //## begin ClUpdateConfigAction::Execute%962811788.body preserve=yes
	ClDspRequest * poRequest ;
	void * pointer ;
	m4pchar_t szBuffer ;
	m4double_t dcpu = 0 , dsysmem = 0 , dsermem =0 ;
	m4uint32_t iBasePort = 0 , isessions = 0 ;
	m4int_t iConnectionID = 0 ;
	m4string_t szState ;
	m4string_t szHost ;
	m4string_t szConfig ;
	m4string_t strUserName ;
	m4string_t strPassword ;
	ClSiteNodeRepository *pSiteNodeRepository = NULL;
	ClSiteNodeInformation *pSNITmp = NULL;
	m4double_t dPrivateMB = 0, dMaxProcSize = 0, dFreeMem = 0;

	M4_SRV_DETAILED_BLOCK( Blk, "Action %0:s", UPDATE_CONFIGURATION ) ;

	if ( M4_SUCCESS == m_pParamList -> GetString ( "cpu" , szBuffer ) )
	{
		dcpu = atof ( szBuffer ) ;
	}

	if ( M4_SUCCESS == m_pParamList -> GetString ( "system_memory" , szBuffer ) )
	{
		dsysmem = atof ( szBuffer ) ;
	}
	if ( M4_SUCCESS == m_pParamList -> GetString ( "server_memory" , szBuffer ) )
	{
		dsermem = atof ( szBuffer ) ;
	}

	if ( M4_SUCCESS == m_pParamList -> GetString ( "sessions" , szBuffer ) )
	{
		isessions = atoi ( szBuffer ) ;
	}

	// eh 178555
	if ( M4_SUCCESS == m_pParamList -> GetString ( "server_size" , szBuffer ) )
	{
		dPrivateMB = atof ( szBuffer ) ;
	}

	if ( M4_SUCCESS == m_pParamList -> GetString ( "max_proc_size" , szBuffer ) )
	{
		dMaxProcSize = atof ( szBuffer ) ;
	}

	if ( M4_SUCCESS == m_pParamList -> GetString ( "free_ram_mem" , szBuffer ) )
	{
		dFreeMem = atof ( szBuffer ) ;
	}


	if ( M4_SUCCESS == m_pParamList -> GetString ( "servername" , szBuffer ) )
	{
		ToLower ( szBuffer ) ;
		szConfig = szBuffer ;
	}
	else
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOUPDATESERVERCONFIGNAME,
			"Error updating server configuration. Cannot get the server configuration name." );			

		return M4_ERROR ;
	}

	if ( M4_SUCCESS == m_pParamList -> GetString ( "state" , szBuffer ) )
	{		
		szState = szBuffer ;
	}
	else
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOUPDATESERVERCONFIGSTATUS,
			"Error updating server configuration. Cannot get the server configuration status." );			

		return M4_ERROR ;
	}

	if ( M4_SUCCESS == m_pParamList -> GetString ( "user" , szBuffer ) )
	{		
		strUserName = szBuffer ;
	}

	if ( M4_SUCCESS == m_pParamList -> GetString ( "password" , szBuffer ) )
	{		
		strPassword = szBuffer ;
	}

	if ( M4_SUCCESS == m_pParamList -> GetString ( "host" , szBuffer ) )
	{
		ToLower ( szBuffer ) ;
		szHost = szBuffer ;
	}

	if ( M4_SUCCESS == m_pParamList -> GetString ( "baseport" , szBuffer ) )
	{
		sscanf  (szBuffer , "%d" , &iBasePort) ;
	}

	if ( M4_SUCCESS == m_pParamList -> GetPointer ( "REQUEST" , pointer) )
	{
		poRequest = (ClDspRequest * )pointer ;
	
		iConnectionID = poRequest->GetConnectionID ( ) ;

	}
	else
	{
		return M4_ERROR ;
	}

	// Get the Connection ID (afterwards, it is used to check connection is open or not)
	//	m_pParamList -> GetInt ( "SESSIONID" , iConnectionID );


	// bg 84086. 
	// Recuperamos el valor de "enabled" de la configuración actual para no pisarlo con
	// el valor por defecto del constructor ClSiteNodeInformation (M4_TRUE).

	pSiteNodeRepository = ClSiteRepositorySingleton::GetInstance ( );

	// Nuevo estado de la configuración
	ClSiteNodeInformation oSiteNode ( szConfig, szHost , iBasePort , strUserName, strPassword, szState , dcpu , isessions , dsermem , dsysmem , iConnectionID, dPrivateMB, dMaxProcSize, dFreeMem  ) ;

	pSNITmp = new ClSiteNodeInformation ( oSiteNode );

	if ( pSNITmp == NULL )
	{
		return M4_ERROR;
	}

	// Estado actual del nodo
	pSiteNodeRepository->GetNodeInfo ( *pSNITmp );
	
	oSiteNode.SetIsEnabled ( pSNITmp->GetIsEnabled ( ) );

	// eh 197951
	oSiteNode.SetUserName( pSNITmp->GetUserName() );
	oSiteNode.SetPassword( pSNITmp->GetPassword() );

	delete pSNITmp;

	m4return_t iRet = ( (ClServerAgent*) m_poExecutive ) -> UpdateConfiguration (oSiteNode) ;

	M4_MDBG_DELETE( (ClDspRequest *)poRequest, delete poRequest );

	return iRet ;
  //## end ClUpdateConfigAction::Execute%962811788.body
}

// Additional Declarations
  //## begin ClUpdateConfigAction%396358090068.declarations preserve=yes
  //## end ClUpdateConfigAction%396358090068.declarations

// Class ClPostReadXMLAction 

ClPostReadXMLAction::ClPostReadXMLAction (ClBaseActionExecutive *ai_poExecutive)
  //## begin ClPostReadXMLAction::ClPostReadXMLAction%961054341.hasinit preserve=no
  //## end ClPostReadXMLAction::ClPostReadXMLAction%961054341.hasinit
  //## begin ClPostReadXMLAction::ClPostReadXMLAction%961054341.initialization preserve=yes
  :ClBaseAction ( ai_poExecutive, M4_TRUE )
  //## end ClPostReadXMLAction::ClPostReadXMLAction%961054341.initialization
{
  //## begin ClPostReadXMLAction::ClPostReadXMLAction%961054341.body preserve=yes
  //## end ClPostReadXMLAction::ClPostReadXMLAction%961054341.body
}



//## Other Operations (implementation)
m4return_t ClPostReadXMLAction::Execute ()
{
  //## begin ClPostReadXMLAction::Execute%961054342.body preserve=yes

	M4_SRV_DEBUG_BLOCK( Blk, "Action %0:s", POST_READ_XML_PROTOCOL_CONNECTION ) ;

    if (m_poExecutive)
	{
		( ( ClDptcherCommsAdaptor * ) m_poExecutive) ->PostReadXML (m_pParamList);

		return M4_SUCCESS ;
	}
	else
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOPOSTREADXMLACTIONEXECUTOR,
			"Error executing post read XML action. No executor assigned to the action." );

		return M4_ERROR ;
	}
  //## end ClPostReadXMLAction::Execute%961054342.body
}

// Additional Declarations
  //## begin ClPostReadXMLAction%3947BD14001B.declarations preserve=yes
  //## end ClPostReadXMLAction%3947BD14001B.declarations

// Class ClPostAcceptPDUAction 

ClPostAcceptPDUAction::ClPostAcceptPDUAction (ClBaseActionExecutive *ai_poExecutive)
  //## begin ClPostAcceptPDUAction::ClPostAcceptPDUAction%961054343.hasinit preserve=no
  //## end ClPostAcceptPDUAction::ClPostAcceptPDUAction%961054343.hasinit
  //## begin ClPostAcceptPDUAction::ClPostAcceptPDUAction%961054343.initialization preserve=yes
  :ClBaseAction ( ai_poExecutive, M4_TRUE )
  //## end ClPostAcceptPDUAction::ClPostAcceptPDUAction%961054343.initialization
{
  //## begin ClPostAcceptPDUAction::ClPostAcceptPDUAction%961054343.body preserve=yes
  //## end ClPostAcceptPDUAction::ClPostAcceptPDUAction%961054343.body
}



//## Other Operations (implementation)
m4return_t ClPostAcceptPDUAction::Execute ()
{
  //## begin ClPostAcceptPDUAction::Execute%961054344.body preserve=yes

	M4_SRV_DEBUG_BLOCK( Blk, "Action %0:s", POST_ACCEPT_PDU_PROTOCOL_CONNECTION ) ;

    if (m_poExecutive)
	{
		( ( ClDptcherCommsAdaptor * ) m_poExecutive) ->PostAcceptClientPDU (m_pParamList);
		return M4_SUCCESS ;
	}
	else
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOPOSTACCEPTPDUACTIONEXECUTOR,
			"Error executing post accept PDU action. No executor assigned to the action." );
		return M4_ERROR ;
	}

  //## end ClPostAcceptPDUAction::Execute%961054344.body
}

// Additional Declarations
  //## begin ClPostAcceptPDUAction%3947BD2301B8.declarations preserve=yes
  //## end ClPostAcceptPDUAction%3947BD2301B8.declarations

// Class ClPostAcceptXMLAction 

ClPostAcceptXMLAction::ClPostAcceptXMLAction (ClBaseActionExecutive *ai_poExecutive)
  //## begin ClPostAcceptXMLAction::ClPostAcceptXMLAction%961054345.hasinit preserve=no
  //## end ClPostAcceptXMLAction::ClPostAcceptXMLAction%961054345.hasinit
  //## begin ClPostAcceptXMLAction::ClPostAcceptXMLAction%961054345.initialization preserve=yes
  :ClBaseAction ( ai_poExecutive, M4_TRUE )
  //## end ClPostAcceptXMLAction::ClPostAcceptXMLAction%961054345.initialization
{
  //## begin ClPostAcceptXMLAction::ClPostAcceptXMLAction%961054345.body preserve=yes
  //## end ClPostAcceptXMLAction::ClPostAcceptXMLAction%961054345.body
}



//## Other Operations (implementation)
m4return_t ClPostAcceptXMLAction::Execute ()
{
  //## begin ClPostAcceptXMLAction::Execute%961054346.body preserve=yes

	M4_SRV_DEBUG_BLOCK( Blk, "Action %0:s", POST_ACCEPT_XML_PROTOCOL_CONNECTION ) ;

    if (m_poExecutive)
	{
		( ( ClDptcherCommsAdaptor * ) m_poExecutive) ->PostAccepXML (m_pParamList);
		return M4_SUCCESS ;
	}
	else
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOPOSTACCEPTXMLACTIONEXECUTOR,
			"Error executing post accept XML action. No executor assigned to the action.");

		return M4_ERROR ;
	}

  //## end ClPostAcceptXMLAction::Execute%961054346.body
}

// Additional Declarations
  //## begin ClPostAcceptXMLAction%3947BD330391.declarations preserve=yes
  //## end ClPostAcceptXMLAction%3947BD330391.declarations

// Class ClPostWritePDUAction 

ClPostWritePDUAction::ClPostWritePDUAction (ClBaseActionExecutive *ai_poExecutive)
  //## begin ClPostWritePDUAction::ClPostWritePDUAction%961054347.hasinit preserve=no
  //## end ClPostWritePDUAction::ClPostWritePDUAction%961054347.hasinit
  //## begin ClPostWritePDUAction::ClPostWritePDUAction%961054347.initialization preserve=yes
  :ClBaseAction ( ai_poExecutive, M4_TRUE )
  //## end ClPostWritePDUAction::ClPostWritePDUAction%961054347.initialization
{
  //## begin ClPostWritePDUAction::ClPostWritePDUAction%961054347.body preserve=yes
  //## end ClPostWritePDUAction::ClPostWritePDUAction%961054347.body
}



//## Other Operations (implementation)
m4return_t ClPostWritePDUAction::Execute ()
{
  //## begin ClPostWritePDUAction::Execute%961054348.body preserve=yes

	M4_SRV_DEBUG_BLOCK( Blk, "Action %0:s", POST_WRITE_PDU_PROTOCOL_CONNECTION ) ;

    if (m_poExecutive)
	{
		( ( ClDptcherCommsAdaptor * ) m_poExecutive) ->PostWriteClientPDU (m_pParamList);

		return M4_SUCCESS ;
	}
	else
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOPOSTWRITEPDUACTIONEXECUTOR,
			"Error executing post write PDU action. No executor assigned to the action." );

		return M4_ERROR ;
	}
  //## end ClPostWritePDUAction::Execute%961054348.body
}

// Additional Declarations
  //## begin ClPostWritePDUAction%3947BD460013.declarations preserve=yes
  //## end ClPostWritePDUAction%3947BD460013.declarations

// Class ClPostWriteXMLAction 

ClPostWriteXMLAction::ClPostWriteXMLAction (ClBaseActionExecutive *ai_poExecutive)
  //## begin ClPostWriteXMLAction::ClPostWriteXMLAction%961054349.hasinit preserve=no
  //## end ClPostWriteXMLAction::ClPostWriteXMLAction%961054349.hasinit
  //## begin ClPostWriteXMLAction::ClPostWriteXMLAction%961054349.initialization preserve=yes
  :ClBaseAction ( ai_poExecutive, M4_TRUE )
  //## end ClPostWriteXMLAction::ClPostWriteXMLAction%961054349.initialization
{
  //## begin ClPostWriteXMLAction::ClPostWriteXMLAction%961054349.body preserve=yes
  //## end ClPostWriteXMLAction::ClPostWriteXMLAction%961054349.body
}



//## Other Operations (implementation)
m4return_t ClPostWriteXMLAction::Execute ()
{
  //## begin ClPostWriteXMLAction::Execute%961054350.body preserve=yes

	M4_SRV_DEBUG_BLOCK( Blk, "Action %0:s", POST_WRITE_XML_PROTOCOL_CONNECTION ) ;

    if (m_poExecutive)
	{
		( ( ClDptcherCommsAdaptor * ) m_poExecutive) ->PostWriteXML (m_pParamList);

		return M4_SUCCESS ;
	}
	else
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOPOSTWRITEXMLACTIONEXECUTOR,
			"Error executing post write XML action. No executor assigned to the action." );

		return M4_ERROR ;
	}

  //## end ClPostWriteXMLAction::Execute%961054350.body
}

// Additional Declarations
  //## begin ClPostWriteXMLAction%3947BD5D02B5.declarations preserve=yes
  //## end ClPostWriteXMLAction%3947BD5D02B5.declarations

// Class ClParseXMLAction 

ClParseXMLAction::ClParseXMLAction (ClBaseActionExecutive *ai_poExecutive)
  //## begin ClParseXMLAction::ClParseXMLAction%961054355.hasinit preserve=no
  //## end ClParseXMLAction::ClParseXMLAction%961054355.hasinit
  //## begin ClParseXMLAction::ClParseXMLAction%961054355.initialization preserve=yes
  :ClDspAuthenticatedAction ( ai_poExecutive ) 
  //## end ClParseXMLAction::ClParseXMLAction%961054355.initialization
{
  //## begin ClParseXMLAction::ClParseXMLAction%961054355.body preserve=yes
  //## end ClParseXMLAction::ClParseXMLAction%961054355.body
}



//## Other Operations (implementation)
m4return_t ClParseXMLAction::Execute ()
{
  //## begin ClParseXMLAction::Execute%961054351.body preserve=yes

	M4_SRV_DEBUG_BLOCK( Blk, "Action %0:s", PARSE_XML ) ;

	if (m_poExecutive)
	{
	
		( ( ClAdminAgent *) m_poExecutive) -> ParseXML ( m_pParamList ) ;
		return M4_SUCCESS ;
	}
	else
	{

		M4_SRV_ERROR_N(M4_ERR_DPCH_NOXMLPARSEACTION,
			"Error executing XML parse action. No executor assigned to the action." );
		return M4_ERROR ;
	}
  //## end ClParseXMLAction::Execute%961054351.body
}

// Additional Declarations
  //## begin ClParseXMLAction%3948ACCE000A.declarations preserve=yes
  //## end ClParseXMLAction%3948ACCE000A.declarations

// Class ClProcessPDUAction 

ClProcessPDUAction::ClProcessPDUAction (ClBaseActionExecutive *ai_poExecutive)
  //## begin ClProcessPDUAction::ClProcessPDUAction%961054353.hasinit preserve=no
  //## end ClProcessPDUAction::ClProcessPDUAction%961054353.hasinit
  //## begin ClProcessPDUAction::ClProcessPDUAction%961054353.initialization preserve=yes
  :ClBaseAction ( ai_poExecutive, M4_TRUE ) 
  //## end ClProcessPDUAction::ClProcessPDUAction%961054353.initialization
{
  //## begin ClProcessPDUAction::ClProcessPDUAction%961054353.body preserve=yes
  //## end ClProcessPDUAction::ClProcessPDUAction%961054353.body
}



//## Other Operations (implementation)
m4return_t ClProcessPDUAction::Execute ()
{
  //## begin ClProcessPDUAction::Execute%961054354.body preserve=yes
	void * pointer ;
	ClDspClientRequestPDU * poClReqPDU ;
	ClDispatcherInvoker * poInvoker = ClDispatcherInvoker::Instance () ;
	m4return_t iRet ;

	M4_SRV_DEBUG_BLOCK( Blk, "Action %0:s", PROCESS_PDU ) ;


	if ( M4_ERROR == m_pParamList -> GetPointer ( "CLIENT_BUFFER" , pointer ) )
	{

		M4_SRV_ERROR_N(M4_ERR_DPCH_NOCLIENTBUFFER,
			"Error processing communication action. Cannot get the client buffer." );			

		return M4_ERROR ;
	}

	poClReqPDU = (ClDspClientRequestPDU *) pointer;


	if (0 == m_poExecutive)
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOPROCESSPDUACTIONEXECUTOR,
			"Error executing process PDU action. No executor assigned to the action." );

		return M4_ERROR ;
	}

	if ( M4_ERROR == ( ( ClClientAgent *) m_poExecutive) -> RedirectClientConnection (poClReqPDU ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOREDIRECTCLIENTCONNECT,
			"Error processing communication action. Cannot redirect client connection." );			
		
		return M4_ERROR ;
	}

	iRet = poInvoker ->ExecuteAction ( REPLY_PDU , m_pParamList ) ;

	m_pParamList = NULL ;

	return iRet ;
  //## end ClProcessPDUAction::Execute%961054354.body
}

// Additional Declarations
  //## begin ClProcessPDUAction%3948AD2D038C.declarations preserve=yes
  //## end ClProcessPDUAction%3948AD2D038C.declarations

// Class ClSendPDUAction 

ClSendPDUAction::ClSendPDUAction (ClBaseActionExecutive *ai_poExecutive)
  //## begin ClSendPDUAction::ClSendPDUAction%962094000.hasinit preserve=no
  //## end ClSendPDUAction::ClSendPDUAction%962094000.hasinit
  //## begin ClSendPDUAction::ClSendPDUAction%962094000.initialization preserve=yes
  :ClBaseAction ( ai_poExecutive, M4_TRUE ) 
  //## end ClSendPDUAction::ClSendPDUAction%962094000.initialization
{
  //## begin ClSendPDUAction::ClSendPDUAction%962094000.body preserve=yes
  //## end ClSendPDUAction::ClSendPDUAction%962094000.body
}



//## Other Operations (implementation)
m4return_t ClSendPDUAction::Execute ()
{
  //## begin ClSendPDUAction::Execute%962094001.body preserve=yes

	M4_SRV_DEBUG_BLOCK( Blk, "Action %0:s", REPLY_PDU ) ;

	if (m_poExecutive)
	{
		return ( ( ClDptcherCommsAdaptor *) m_poExecutive) -> SendPDUReplay (m_pParamList) ;
	}
	else
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOSENDPDUACTIONEXECUTOR,
			"Error executing send PDU action. No executor assigned to the action." );

		return M4_ERROR;
	}

  //## end ClSendPDUAction::Execute%962094001.body
}

// Additional Declarations
  //## begin ClSendPDUAction%395876230321.declarations preserve=yes
  //## end ClSendPDUAction%395876230321.declarations

// Class ClSendXMLAction 

ClSendXMLAction::ClSendXMLAction (ClBaseActionExecutive *ai_poExecutive)
  //## begin ClSendXMLAction::ClSendXMLAction%962094002.hasinit preserve=no
  //## end ClSendXMLAction::ClSendXMLAction%962094002.hasinit
  //## begin ClSendXMLAction::ClSendXMLAction%962094002.initialization preserve=yes
  :ClBaseAction ( ai_poExecutive, M4_TRUE ) 
  //## end ClSendXMLAction::ClSendXMLAction%962094002.initialization
{
  //## begin ClSendXMLAction::ClSendXMLAction%962094002.body preserve=yes
  //## end ClSendXMLAction::ClSendXMLAction%962094002.body
}



//## Other Operations (implementation)
m4return_t ClSendXMLAction::Execute ()
{
  //## begin ClSendXMLAction::Execute%962094003.body preserve=yes

	M4_SRV_DEBUG_BLOCK( Blk, "Action %0:s", REPLY_XML ) ;

	if (m_poExecutive)
	{
		return ( ( ClDptcherCommsAdaptor *) m_poExecutive) -> SendXMLReply (m_pParamList) ;
	}
	else
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOSENDXMLACTIONEXECUTOR,
			"Error executing send XML action. No executor assigned to the action." );

		return M4_ERROR;

	}
  //## end ClSendXMLAction::Execute%962094003.body
}

// Additional Declarations
  //## begin ClSendXMLAction%3958764E00FC.declarations preserve=yes
  //## end ClSendXMLAction%3958764E00FC.declarations

// Class ClSendXMLToAppCtl 

ClSendXMLToAppCtl::ClSendXMLToAppCtl (ClBaseActionExecutive *ai_poExecutive)
  //## begin ClSendXMLToAppCtl::ClSendXMLToAppCtl%963388997.hasinit preserve=no
  //## end ClSendXMLToAppCtl::ClSendXMLToAppCtl%963388997.hasinit
  //## begin ClSendXMLToAppCtl::ClSendXMLToAppCtl%963388997.initialization preserve=yes
  :ClBaseAction ( ai_poExecutive, M4_TRUE ) 
  //## end ClSendXMLToAppCtl::ClSendXMLToAppCtl%963388997.initialization
{
  //## begin ClSendXMLToAppCtl::ClSendXMLToAppCtl%963388997.body preserve=yes
  //## end ClSendXMLToAppCtl::ClSendXMLToAppCtl%963388997.body
}



//## Other Operations (implementation)
m4return_t ClSendXMLToAppCtl::Execute ()
{
  //## begin ClSendXMLToAppCtl::Execute%963388998.body preserve=yes
	m4pchar_t szHost ;
	m4uint32_t iPort ;
	m4int_t iParam ;
	ClDspAppCtlRequest * poRequest ;


	M4_SRV_DEBUG_BLOCK( Blk, "Action %0:s", SEND_XML_TO_APPCTL ) ;

	if ( M4_ERROR == m_pParamList -> GetInt ( "PORT" , iParam ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOXMLAPPCTLPORT,
			"Error sending XML reply to the App. Controller. Cannot get the port." );

		return M4_ERROR ;
	}

	iPort = iParam ;

	if ( M4_ERROR == m_pParamList -> GetPointer ( "REQUEST" , (void*&) poRequest ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOXMLAPPCTLBUFFER,
			"Error sending XML reply to the App. Controller. Cannot get the communication buffer." );
		
		return M4_ERROR ;
	}

	if ( M4_ERROR == m_pParamList -> GetString ( "HOST" , szHost ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOXMLAPPCTLHOST,
			"Error sending XML reply to the App. Controller. Cannot get the App. Controller host." );

		return M4_ERROR ;
	}

	return ( (ClDptcherCommsAdaptor *) m_poExecutive ) -> SendXMLtoAppCtl (szHost, iPort, poRequest );
  //## end ClSendXMLToAppCtl::Execute%963388998.body
}

// Additional Declarations
  //## begin ClSendXMLToAppCtl%396C38470194.declarations preserve=yes
  //## end ClSendXMLToAppCtl%396C38470194.declarations

// Class ClConfigureComms 

ClConfigureComms::ClConfigureComms (ClBaseActionExecutive *ai_poExecutive)
  //## begin ClConfigureComms::ClConfigureComms%972986735.hasinit preserve=no
  //## end ClConfigureComms::ClConfigureComms%972986735.hasinit
  //## begin ClConfigureComms::ClConfigureComms%972986735.initialization preserve=yes
  :ClBaseAction ( ai_poExecutive, M4_TRUE ) 
  //## end ClConfigureComms::ClConfigureComms%972986735.initialization
{
  //## begin ClConfigureComms::ClConfigureComms%972986735.body preserve=yes
  //## end ClConfigureComms::ClConfigureComms%972986735.body
}



//## Other Operations (implementation)
m4return_t ClConfigureComms::Execute ()
{
  //## begin ClConfigureComms::Execute%972986736.body preserve=yes
	m4int_t iBasePort , iMaxRetries , iNumWorkers ;
	m4pchar_t szBasePort;
	m4pchar_t szCypherMethod ;
	m4pchar_t szMaxRetries;
	m4pchar_t szNumWorkers ;
	m4pchar_t szTimeout ;
	m4uint32_t iTimeout = 0;
	m4uint32_t iSnmpPort = 0 ;
	m4pchar_t szDumpTrace ;
	m4uint32_t iDumpTrace = 0 ;

	m4pchar_t szSessionsWeight, szSrvMemWeight, szSysMemWeight, szCPUWeight, szExecTimeWeight, szExecTimeThreshold;
	m4uint32_t iSessionsWeight = 0, iSrvMemWeight = 0, iSysMemWeight = 0, iCPUWeight = 0, iExecTimeWeight = 0, iExecTimeThreshold = 0;

	M4_SRV_GENERAL_BLOCK( Blk, "Action %0:s", CONFIGURE_PORTS ) ;

	if ( M4_ERROR == m_pParamList -> GetString ( "baseport" , szBasePort ) )
	{
		M4_SRV_ERROR(M4_ERR_DPCH_CONFIG_PROPERTY,
			"Error inicializando M4 Site Dispatcher. No se ha encontrado la propiedad '%0:s'" , "baseport");

		return M4_ERROR ;
	}

	if (1 > sscanf (szBasePort , "%d" , &iBasePort ) )
	{
		M4_SRV_ERROR(M4_ERR_DPCH_CONFIG_PROPERTY,
			"Error inicializando M4 Site Dispatcher. No se ha encontrado la propiedad '%0:s'" , "baseport" );
		return M4_ERROR ;
	}

	if ( M4_ERROR == m_pParamList -> GetString ( "CypherMethod" , szCypherMethod ) )
	{
		M4_SRV_ERROR(M4_ERR_DPCH_CONFIG_PROPERTY,
			"Error inicializando M4 Site Dispatcher. No se ha encontrado la propiedad '%0:s'" , "CypherMethod" );

		return M4_ERROR ;
	}

	if ( M4_ERROR == m_pParamList -> GetString ( "MaxStartRetries" , szMaxRetries ) )
	{
		M4_SRV_ERROR (M4_ERR_DPCH_CONFIG_PROPERTY,
			"Error inicializando M4 Site Dispatcher. No se ha encontrado la propiedad '%0:s'" , "MaxStartRetries");

		return M4_ERROR ;
	}

	if( 1 > sscanf ( szMaxRetries , "%d" , &iMaxRetries )  )
	{
		M4_SRV_ERROR (M4_ERR_DPCH_CONFIG_PROPERTY,
			"Error inicializando M4 Site Dispatcher. No se ha encontrado la propiedad '%0:s'" , "MaxStartRetries");
		return M4_ERROR ;
	}

	if ( M4_ERROR == m_pParamList -> GetString ( "ServerStartTimeout" , szTimeout ) )
	{
		M4_SRV_WARNING (M4_ERR_DPCH_CONFIG_PROPERTY,
			"Error inicializando M4 Site Dispatcher. No se ha encontrado la propiedad '%0:s'" , "ServerStartTimeout");

//		return M4_ERROR ;
	}
	else
	{
		sscanf ( szTimeout , "%d" , &iTimeout )  ;
	}

	if ( M4_ERROR == m_pParamList -> GetString ( "NumWorkers" , szNumWorkers ) )
	{
		M4_SRV_ERROR (M4_ERR_DPCH_CONFIG_PROPERTY,
			"Error inicializando M4 Site Dispatcher. No se ha encontrado la propiedad '%0:s'" , "NumWorkers");

		return M4_ERROR ;
	}

	if( 1 > sscanf ( szNumWorkers , "%d" , &iNumWorkers )  )
	{
		M4_SRV_ERROR (M4_ERR_DPCH_CONFIG_PROPERTY,
			"Error inicializando M4 Site Dispatcher. No se ha encontrado la propiedad '%0:s'" , "NumWorkers");
		return M4_ERROR ;
	}

	// eh 178555
	if ( M4_ERROR == m_pParamList -> GetString ( "dump_trace" , szDumpTrace ) )
	{
		M4_SRV_ERROR(M4_ERR_DPCH_CONFIG_PROPERTY,
			"Error inicializando M4 Site Dispatcher. No se ha encontrado la propiedad '%0:s'" , "dump_trace" );

		return M4_ERROR ;
	}

	if( !stricmp(szDumpTrace, "Enabled") )
	{
		iDumpTrace = 1 ;
	}
	else
	{
		iDumpTrace = 0 ;
	}

	
	if ( M4_ERROR == m_pParamList -> GetString ( "LoadSessionsWeight" , szSessionsWeight ) )
	{
		M4_SRV_ERROR (M4_ERR_DPCH_CONFIG_PROPERTY,
			"Error inicializando M4 Site Dispatcher. No se ha encontrado la propiedad '%0:s'" , "LoadSessionsWeight");

		return M4_ERROR ;
	}

	if( 1 > sscanf ( szSessionsWeight , "%d" , &iSessionsWeight )  )
	{
		M4_SRV_ERROR (M4_ERR_DPCH_CONFIG_PROPERTY,
			"Error inicializando M4 Site Dispatcher. No se ha encontrado la propiedad '%0:s'" , "LoadSessionsWeight");
		return M4_ERROR ;
	}

	if ( M4_ERROR == m_pParamList -> GetString ( "LoadSrvMemWeight" , szSrvMemWeight ) )
	{
		M4_SRV_ERROR (M4_ERR_DPCH_CONFIG_PROPERTY,
			"Error inicializando M4 Site Dispatcher. No se ha encontrado la propiedad '%0:s'" , "LoadSrvMemWeight");

		return M4_ERROR ;
	}

	if( 1 > sscanf ( szSrvMemWeight, "%d" , &iSrvMemWeight )  )
	{
		M4_SRV_ERROR (M4_ERR_DPCH_CONFIG_PROPERTY,
			"Error inicializando M4 Site Dispatcher. No se ha encontrado la propiedad '%0:s'" , "LoadSrvMemWeight");
		return M4_ERROR ;
	}

	if ( M4_ERROR == m_pParamList -> GetString ( "LoadSysMemWeight" , szSysMemWeight ) )
	{
		M4_SRV_ERROR (M4_ERR_DPCH_CONFIG_PROPERTY,
			"Error inicializando M4 Site Dispatcher. No se ha encontrado la propiedad '%0:s'" , "LoadSysMemWeight");

		return M4_ERROR ;
	}

	if( 1 > sscanf ( szSysMemWeight, "%d" , &iSysMemWeight )  )
	{
		M4_SRV_ERROR (M4_ERR_DPCH_CONFIG_PROPERTY,
			"Error inicializando M4 Site Dispatcher. No se ha encontrado la propiedad '%0:s'" , "LoadSysMemWeight");
		return M4_ERROR ;
	}

	if ( M4_ERROR == m_pParamList -> GetString ( "LoadCPUWeight" , szCPUWeight ) )
	{
		M4_SRV_ERROR (M4_ERR_DPCH_CONFIG_PROPERTY,
			"Error inicializando M4 Site Dispatcher. No se ha encontrado la propiedad '%0:s'" , "LoadCPUWeight");

		return M4_ERROR ;
	}

	if( 1 > sscanf ( szCPUWeight, "%d" , &iCPUWeight )  )
	{
		M4_SRV_ERROR (M4_ERR_DPCH_CONFIG_PROPERTY,
			"Error inicializando M4 Site Dispatcher. No se ha encontrado la propiedad '%0:s'" , "LoadCPUWeight");
		return M4_ERROR ;
	}

	if ( M4_ERROR == m_pParamList -> GetString ( "LoadExecTimeWeight" , szExecTimeWeight ) )
	{
		M4_SRV_ERROR (M4_ERR_DPCH_CONFIG_PROPERTY,
			"Error inicializando M4 Site Dispatcher. No se ha encontrado la propiedad '%0:s'" , "LoadExecTimeWeight");

		return M4_ERROR ;
	}

	if( 1 > sscanf ( szExecTimeWeight, "%d" , &iExecTimeWeight )  )
	{
		M4_SRV_ERROR (M4_ERR_DPCH_CONFIG_PROPERTY,
			"Error inicializando M4 Site Dispatcher. No se ha encontrado la propiedad '%0:s'" , "LoadExecTimeWeight");
		return M4_ERROR ;
	}

	if ( M4_ERROR == m_pParamList -> GetString ( "LoadExecTimeThresholdMsecs" , szExecTimeThreshold ) )
	{
		M4_SRV_ERROR (M4_ERR_DPCH_CONFIG_PROPERTY,
			"Error inicializando M4 Site Dispatcher. No se ha encontrado la propiedad '%0:s'" , "LoadExecTimeThresholdMsecs");

		return M4_ERROR ;
	}

	if( 1 > sscanf ( szExecTimeThreshold, "%d" , &iExecTimeThreshold )  )
	{
		M4_SRV_ERROR (M4_ERR_DPCH_CONFIG_PROPERTY,
			"Error inicializando M4 Site Dispatcher. No se ha encontrado la propiedad '%0:s'" , "LoadExecTimeThresholdMsecs");
		return M4_ERROR ;
	}

	ClDspAgent::SetParams ( iBasePort, iMaxRetries, iTimeout, iDumpTrace, iSessionsWeight, iSrvMemWeight, iSysMemWeight, iCPUWeight, iExecTimeWeight, iExecTimeThreshold);

	M4_SRV_GENERAL("Configure comms with BasePort %0:s ", iBasePort ) ;

	return ( (ClDptcherCommsAdaptor *) m_poExecutive ) -> InitComms ( szCypherMethod , iNumWorkers );
//	return M4_SUCCESS ;

  //## end ClConfigureComms::Execute%972986736.body
}

// Additional Declarations
  //## begin ClConfigureComms%39FE9E5703AE.declarations preserve=yes
  //## end ClConfigureComms%39FE9E5703AE.declarations

// Class ClAppCtlReplyAction 

ClAppCtlReplyAction::ClAppCtlReplyAction (ClBaseActionExecutive *ai_poExecutive)
  //## begin ClAppCtlReplyAction::ClAppCtlReplyAction%981535907.hasinit preserve=no
  //## end ClAppCtlReplyAction::ClAppCtlReplyAction%981535907.hasinit
  //## begin ClAppCtlReplyAction::ClAppCtlReplyAction%981535907.initialization preserve=yes
  :ClBaseAction ( ai_poExecutive, M4_TRUE ) 
  //## end ClAppCtlReplyAction::ClAppCtlReplyAction%981535907.initialization
{
  //## begin ClAppCtlReplyAction::ClAppCtlReplyAction%981535907.body preserve=yes
  //## end ClAppCtlReplyAction::ClAppCtlReplyAction%981535907.body
}



//## Other Operations (implementation)
m4return_t ClAppCtlReplyAction::Execute ()
{
  //## begin ClAppCtlReplyAction::Execute%981535908.body preserve=yes
	m4pchar_t pRequestID , pResult , szLog = NULL ;
	m4uint32_t iRequestID ;
	m4return_t iResult = M4_ERROR ;
	ClDspRequest * poRequest ;
	// ClLogSystem * poLog ;
	// m4return_t iRet ;




	if ( M4_ERROR == m_pParamList -> GetString ( "RequestID" , pRequestID ) )
	{
		return M4_ERROR ;
	}

	m_pParamList -> GetString ( "log" , szLog ) ;


	if ( 1 > sscanf ( pRequestID , "%d" , &iRequestID ) )
	{
		return M4_ERROR ;
	}

	if ( M4_ERROR != m_pParamList -> GetString ( "Result" , pResult ) )
	{
		if (! stricmp ( "SUCCESS" , pResult ) )
		{
			iResult = M4_SUCCESS ;
		}
	}

	if ( M4_ERROR == m_pParamList -> GetPointer ( "REQUEST" , (void *&) poRequest ) )
	{
		return M4_ERROR ;
	}

/*	if ( poLog = poRequest -> GetLog () )
	{
		iRet = poLog-> SetSerialCodeString (szLog   ) ;
	}
*/
	return ( ( ClDptcherCommsAdaptor *) m_poExecutive) -> ProcessAppctlReply(iResult, iRequestID , poRequest , szLog )  ;
  //## end ClAppCtlReplyAction::Execute%981535908.body
}

// Additional Declarations
  //## begin ClAppCtlReplyAction%3A810C29038C.declarations preserve=yes
  //## end ClAppCtlReplyAction%3A810C29038C.declarations

//## begin module%3948844F002B.epilog preserve=yes
//## end module%3948844F002B.epilog


// Detached code regions:
// WARNING: this code will be lost if code is regenerated.
#if 0
//## begin PostReadAppCtlReply::PostReadAppCtlReply%983293166.initialization preserve=no
  :ClBaseAction ( ai_poExecutive, M4_TRUE ) 
//## end PostReadAppCtlReply::PostReadAppCtlReply%983293166.initialization

//## begin PostReadAppCtlReply::Execute%983293167.body preserve=no
	return ( ( ClDptcherCommsAdaptor *) m_poExecutive) -> PostReadAppctl (m_pParamList ) ;
//## end PostReadAppCtlReply::Execute%983293167.body

#endif
