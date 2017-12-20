//## begin module%39479DC90225.cm preserve=no
//## end module%39479DC90225.cm

//## begin module%39479DC90225.cp preserve=no
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
//## end module%39479DC90225.cp

//## Module: commadapter%39479DC90225; Package body
//## Subsystem: M4Dispatcher::src%3923BBFB0332
//## Source file: z:\m4dispatcher\src\commadapter.cpp

//## begin module%39479DC90225.additionalIncludes preserve=no
//## end module%39479DC90225.additionalIncludes

//## begin module%39479DC90225.includes preserve=yes
#pragma warning (disable : 4284)

#ifdef _WINDOWS
#include <winsock2.h>
#endif

#include <dspinvoker.hpp>
#include <dspactions.hpp>
#include <actionidentifiers.hpp>
#include <m4log.hpp>
#include <m4srvres.hpp>
#include <clarraylibres.hpp>
#include <executiveagent.hpp>
#include <dspclientreqpdu.hpp>
#include <dspxmlreq.hpp>
#include <basiclog.hpp>
#include <m4dpchres.hpp>
#include <dspappctlreq.hpp>


#ifdef _UNIX
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif

#include <m4memdbg.h>
#include <m4srvtrace.h>

#define M4_SRV_TRACE_COMP_ID "DPCH"

//## end module%39479DC90225.includes

// m4communication
#include <m4communication.hpp>
// dspagent
#include <dspagent.hpp>
// commadapter
#include <commadapter.hpp>
// dspreqcontainer
#include <dspreqcontainer.hpp>
// ClAdminAgent
#include <adminagent.hpp>


//## begin module%39479DC90225.declarations preserve=no
//## end module%39479DC90225.declarations

//## begin module%39479DC90225.additionalDeclarations preserve=yes
#define CERTF	"/conf/servercert.pem"
#define KEYF	"/conf/serverpriv.pem"
//## end module%39479DC90225.additionalDeclarations


// Class ClDptcherCommsAdaptor 







ClDptcherCommsAdaptor::ClDptcherCommsAdaptor (ClSiteNodeRepository *ai_poServerRepository, ClDspExecutiveAgent *ai_poExecutiveAgent)
  //## begin ClDptcherCommsAdaptor::ClDptcherCommsAdaptor%960967024.hasinit preserve=no
      : m_bStarted(M4_FALSE)
  //## end ClDptcherCommsAdaptor::ClDptcherCommsAdaptor%960967024.hasinit
  //## begin ClDptcherCommsAdaptor::ClDptcherCommsAdaptor%960967024.initialization preserve=yes
  , ClDspAgent ( ai_poServerRepository , ai_poExecutiveAgent)
  //## end ClDptcherCommsAdaptor::ClDptcherCommsAdaptor%960967024.initialization
{
  //## begin ClDptcherCommsAdaptor::ClDptcherCommsAdaptor%960967024.body preserve=yes
	m_szHost=NULL ;
	m_poCommunications = NULL ;
  //## end ClDptcherCommsAdaptor::ClDptcherCommsAdaptor%960967024.body
}


ClDptcherCommsAdaptor::~ClDptcherCommsAdaptor ()
{
  //## begin ClDptcherCommsAdaptor::~ClDptcherCommsAdaptor%960967025.body preserve=yes
	M4_MDBG_DELETE(  m_szHost, delete m_szHost ) ;
	M4_MDBG_DELETE(  m_poCommunications , delete m_poCommunications ) ;
  //## end ClDptcherCommsAdaptor::~ClDptcherCommsAdaptor%960967025.body
}



//## Other Operations (implementation)
m4return_t ClDptcherCommsAdaptor::PostReadClientPDU (ClParamList *ai_pParamList)
{
  //## begin ClDptcherCommsAdaptor::PostReadClientPDU%960967026.body preserve=yes
	void * thepointer = NULL;
	m4return_t RetValue ;
	m4int_t iConnectionID ;
	ClParamList * poParamList = NULL;
	ClDispatcherInvoker * poInvoker = ClDispatcherInvoker::Instance () ;

	if(M4_ERROR==ai_pParamList->GetM4Return(M4_PARAM_RETURN_RESULT,RetValue))
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONLAYERPROTOCOLERROR,
			"Error running communication layer protocol." );		
		return M4_ERROR ;
	}
	else
	{
		if(M4_ERROR==ai_pParamList->GetInt(M4_PARAM_CONNECTIONID, iConnectionID))
		{
			M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONLAYERSESSIONIDERROR,
				"Error getting Session Identifier." );		

			return M4_ERROR ;
		}
		if(M4_ERROR==RetValue )
		{
			//NO es error: depende de quien cierre antes, el otro encontrara la conexion cerrada.
			//M4_SRV_ERROR(M4_ERR_DPCH_CLIENTCONNECTIONBROKEN, 
			//	"Error: Client Connection %0:s broken.",
			//	iConnectionID);		

			//SETCODEF ( M4_SRV_DISCONNECTION , DEBUGINFOLOG  , "Connection %d disconnected" ,iConnectionID);
			
			M4_MDBG_NEW(  poParamList, poParamList = new ClParamList ) ;

			poParamList -> SetInt ( "SESSIONID" , iConnectionID ) ;
			poParamList -> SetInt ( "STOP_LISTENING_FALG" , 1 ) ;

			return poInvoker -> ExecuteAction ( CLOSE_CONNECTION , poParamList ) ;
		}
		else
		{
			if(M4_ERROR==ai_pParamList->GetPointer(M4_PARAM_READED_PDU,thepointer))
			{
				M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONLAYERPROTOCOLERROR,
					"Error running communication layer protocol. Error recovering BUFFER." );		
			}
			else
			{
/*				if (! ( poPDU = (ClPDU *) thepointer ) )
				{
					return M4_ERROR ;
				}
*/
				m4int64_t iPetitionID = ClIdGenerator::GetNewId () ;
				ClDspClientRequestPDU * poClReqPDU = NULL ;
				
				M4_MDBG_NEW(  (ClDspRequest *)poClReqPDU, poClReqPDU = new 
					ClDspClientRequestPDU ( thepointer , iPetitionID , iConnectionID ) ) ;

				M4_MDBG_NEW(  poParamList, poParamList = new ClParamList ) ;

				poParamList -> SetPointer ( "CLIENT_BUFFER" , poClReqPDU ) ;
				poParamList -> SetInt64 ( "PDUID" , iPetitionID  ) ;
				poParamList -> SetInt ( "SESSIONID" , iConnectionID ) ;

				if ( M4_ERROR == poInvoker -> ExecuteAction ( PROCESS_PDU , poParamList ) )
				{
					M4_SRV_ERROR(M4_ERR_DPCH_NOCOMMUNICATIONEXECUTEACTION, 
						"Error in Client communication. Cannot execute action %0:s.",
						PROCESS_PDU);
					
					return M4_ERROR ;
				}

			}
		}
	}
	
	return M4_SUCCESS ;
  //## end ClDptcherCommsAdaptor::PostReadClientPDU%960967026.body
}

m4return_t ClDptcherCommsAdaptor::PostReadXML (ClParamList *ai_pParamList)
{
  //## begin ClDptcherCommsAdaptor::PostReadXML%960967027.body preserve=yes
	void * thepointer ;
	m4return_t RetValue ;
	m4int_t iConnectionID ;
	m4int64_t iPetitionID;
	ClParamList * poParamList = NULL;
	ClDispatcherInvoker * poInvoker = ClDispatcherInvoker::Instance () ;

	if(M4_ERROR==ai_pParamList->GetM4Return(M4_PARAM_RETURN_RESULT,RetValue))
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONXMLPROTOCOLERROR,
			"Error running communication layer XML protocol. Error recovering RetValue." );
	}
	else
	{
		if(M4_ERROR==ai_pParamList->GetInt(M4_PARAM_CONNECTIONID, iConnectionID))
		{
			M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONXMLSESSIONIDERROR,
				"Error getting Client XML Session Identifier.");		
			return M4_ERROR ;
		}
		if(M4_ERROR==RetValue )
		{

			M4_MDBG_NEW(  poParamList, poParamList = new ClParamList ) ;

			//NO es error: depende de quien cierre antes, el otro encontrara la conexion cerrada.
			//M4_SRV_ERROR(M4_ERR_DPCH_CLIENTXMLCONNECTIONBROKEN, 
			//	"Error: Client XML Connection %0:s broken.", iConnectionID);		

			poParamList -> SetInt ( "SESSIONID" , iConnectionID ) ;

			// OK, the first thing is to remove the admin client connection, it it is the case
			if ( M4_ERROR == poInvoker -> ExecuteAction ( CLOSE_CONNECTION_ADMIN , poParamList ) )
			{
				M4_SRV_ERROR(M4_ERR_DPCH_NOCOMMUNICATIONXMLEXECUTEACTION, 
					"Error in Client XML communication.\nCannot execute action %0:s.",
					PARSE_XML);

				// we don't return en error here, since it is not critical
			}

			//
			// Check the Connection ID (Compare the Connection ID of the Connection
			// ID of the Server nodes
			//

			if ( M4_ERROR == poInvoker -> ExecuteAction ( CHECK_SERVER_CONNECTION , poParamList ) )
			{
				M4_SRV_ERROR(M4_ERR_DPCH_NOCOMMUNICATIONXMLEXECUTEACTION, 
					"Error in Client XML communication.\nCannot execute action %0:s.",
					CHECK_SERVER_CONNECTION);

				return M4_ERROR ;
			}

			//SETCODEF ( M4_SRV_DISCONNECTION , DEBUGINFOLOG  , "Connection %d disconnected" ,iConnectionID);
			m_poCommunications ->StopListening(iConnectionID) ;

			m_poCommunications -> CloseConnection(iConnectionID) ;
		}
		else
		{
			if(M4_ERROR==ai_pParamList->GetPointer(M4_PARAM_READED_BUFFER,thepointer))
			{
				M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONXMLPROTOCOLERROR,
					"Error running communication layer XML protocol. Error recovering BUFFER." );
			}
			else
			{
				M4_MDBG_NEW(  poParamList, poParamList = new ClParamList ) ;

				iPetitionID = ClIdGenerator::GetNewId () ;

				ClDspXMLRequest * poRequest = NULL ;
				
				M4_MDBG_NEW(  (ClDspRequest *)poRequest, poRequest = new 
					ClDspXMLRequest ( thepointer , iPetitionID , iConnectionID ) ) ;

				poParamList -> SetPointer ( "REQUEST" , poRequest ) ;
//				poParamList -> SetInt ( "XMLID" , iPetitionID  ) ;				
//				poParamList -> SetInt ( "SESSIONID" , iConnectionID ) ;

				if ( M4_ERROR == poInvoker -> ExecuteAction ( PARSE_XML , poParamList ) )
				{
					M4_SRV_ERROR(M4_ERR_DPCH_NOCOMMUNICATIONXMLEXECUTEACTION, 
						"Error in Client XML communication.\nCannot execute action %0:s.",
						PARSE_XML);

					return M4_ERROR ;
				}

			}
		}
	}
				
	return M4_SUCCESS ;
  //## end ClDptcherCommsAdaptor::PostReadXML%960967027.body
}

m4return_t ClDptcherCommsAdaptor::PostAcceptClientPDU (ClParamList *ai_pParamList)
{
  //## begin ClDptcherCommsAdaptor::PostAcceptClientPDU%960967028.body preserve=yes
	m4return_t RetValue ;
	m4int_t iConnectionID;
	ClDispatcherInvoker * poInvoker = ClDispatcherInvoker::Instance () ;


	if(M4_ERROR==ai_pParamList->GetM4Return(M4_PARAM_RETURN_RESULT,RetValue))
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONLAYERPROTOCOLERROR,
			"Error running communication layer protocol. Error recovering RetValue." );		
	}
	else
	{
		if(M4_ERROR==RetValue)
		{
			M4_SRV_ERROR_N(M4_ERR_DPCH_CONNECTIONESTABLISHEDERROR,
				"Error establishing connection. Error acpeting PDU." );		
		}
		else
		{
			if(M4_ERROR!=ai_pParamList->GetInt(M4_PARAM_CONNECTIONID, iConnectionID))
			{
				m_poCommunications -> StartListening(iConnectionID, POST_READ_PDU_PROTOCOL_CONNECTION, poInvoker  );
			}
			else
			{
				M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONLAYERSESSIONIDERROR,
					"Error getting Session Identifier." );		
			}
		}
	}

	return M4_SUCCESS ;
  //## end ClDptcherCommsAdaptor::PostAcceptClientPDU%960967028.body
}

m4return_t ClDptcherCommsAdaptor::PostAccepXML (ClParamList *ai_pParamList)
{
  //## begin ClDptcherCommsAdaptor::PostAccepXML%960967029.body preserve=yes
	m4return_t RetValue ;
	m4int_t iConnectionID;
	ClDispatcherInvoker * poInvoker = ClDispatcherInvoker::Instance () ;


	if(M4_ERROR==ai_pParamList->GetM4Return(M4_PARAM_RETURN_RESULT,RetValue))
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONXMLPROTOCOLERROR,
			"Error running communication layer XML protocol. Error recovering RetValue." );

	}
	else
	{
		if(M4_ERROR==RetValue)
		{
			M4_SRV_ERROR_N(M4_ERR_DPCH_XMLCONNECTIONESTABLISHEDERROR,
				"Error establishing XML connection. Error acepting ASCII." );		

		}
		else
		{
			if(M4_ERROR!=ai_pParamList->GetInt(M4_PARAM_CONNECTIONID, iConnectionID))
			{
				RetValue = m_poCommunications -> StartListening(iConnectionID, POST_READ_XML_PROTOCOL_CONNECTION, poInvoker );
			}
			else
			{
				M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONXMLSESSIONIDERROR,
					"Error getting Client XML Session Identifier." );		
				return M4_ERROR ;
			}
		}
	}

	return RetValue ;
  //## end ClDptcherCommsAdaptor::PostAccepXML%960967029.body
}

m4return_t ClDptcherCommsAdaptor::PostWriteClientPDU (ClParamList *ai_pParamList)
{
  //## begin ClDptcherCommsAdaptor::PostWriteClientPDU%960967030.body preserve=yes
	m4int64_t iPetitionID ;
	ClDspRequest * poRequest ;


	if ( M4_ERROR == ai_pParamList -> GetInt64 ( M4_PARAM_PETITION_ID , iPetitionID ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONLAYERPROTOCOLERROR,
			"Error running communication layer protocol." );		

		return M4_ERROR ;
	}


	if ( M4_SUCCESS == m_oReqContainer.RemoveReq ( iPetitionID , poRequest ) )
	{
		M4_MDBG_DELETE( (ClDspRequest * )poRequest, delete poRequest ) ;
	}
	else
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOREMOVEDPCHREQUEST,
			"Error removing Dispatcher request. Cannot find the request %0:s to remove.",
			(m4uint32_t)iPetitionID );			
	}

	return M4_SUCCESS ;
  //## end ClDptcherCommsAdaptor::PostWriteClientPDU%960967030.body
}

m4return_t ClDptcherCommsAdaptor::PostWriteXML (ClParamList *ai_pParamList)
{
  //## begin ClDptcherCommsAdaptor::PostWriteXML%960967031.body preserve=yes
	m4int64_t iPetitionID ;
	ClDspRequest * poRequest ;


	if ( M4_ERROR == ai_pParamList -> GetInt64 ( M4_PARAM_PETITION_ID , iPetitionID ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONXMLPROTOCOLERROR,
			"Error running communication layer XML protocol." );

		return M4_ERROR ;
	}


	if ( M4_SUCCESS == m_oReqContainer.RemoveReq ( iPetitionID , poRequest ) )
	{
		M4_MDBG_DELETE( (ClDspRequest *)poRequest, delete poRequest ) ;
	}
	else
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOREMOVEDPCHREQUEST,
			"Error removing Dispatcher request. Cannot find the request %0:s to remove.",
			(m4uint32_t)iPetitionID );			
	}

	return M4_SUCCESS ;
  //## end ClDptcherCommsAdaptor::PostWriteXML%960967031.body
}

m4return_t ClDptcherCommsAdaptor::SendXMLReply (ClParamList *ai_pParams)
{
  //## begin ClDptcherCommsAdaptor::SendXMLReply%960967032.body preserve=yes
	void * thePointer ;
	ClDspRequest * poRequest ;
	m4pchar_t pBuffer ;
	m4uint32_t idSession;
	m4uint64_t ixmlid ;
	ClDispatcherInvoker * poInvoker = ClDispatcherInvoker::Instance () ;



	if ( M4_ERROR == ai_pParams -> GetPointer ( "REQUEST" , thePointer ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONXMLPROTOCOLERROR,
			"Error running communication layer XML protocol." );

		return M4_ERROR ;
	}

	poRequest = (ClDspRequest *) thePointer ;

	idSession = poRequest ->GetConnectionID () ;

	ixmlid = poRequest ->GetPetitionID ( ) ;

	pBuffer = (char*) poRequest ->GetBufferOut ( ) ;

	m_oReqContainer.AddReq ( ixmlid , poRequest ) ;

	M4_SRV_DEBUG( "XML reply to session %0:s XMLid %1:s. Data %2:s",
		idSession << (m4uint32_t)ixmlid <<  pBuffer ) ;

	if ( M4_ERROR == m_poCommunications -> Write (idSession, ixmlid, pBuffer ,strlen ( pBuffer ) +1 , POST_WRITE_XML_PROTOCOL_CONNECTION , poInvoker ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_SENDINGXMLREPLYERROR, "Error sending XML reply." );

		if ( M4_ERROR == m_oReqContainer.RemoveReq ( ixmlid , poRequest ) )
		{
			M4_SRV_ERROR(M4_ERR_DPCH_NOREMOVEDPCHREQUEST,
				"Error removing Dispatcher request. Cannot find the request %0:s to remove.",
				(m4uint32_t)ixmlid );			
		}

		M4_MDBG_DELETE( (ClDspRequest * )poRequest, delete poRequest ) ;
		
		return M4_ERROR ;
	}
	
	return M4_SUCCESS;


  //## end ClDptcherCommsAdaptor::SendXMLReply%960967032.body
}

m4return_t ClDptcherCommsAdaptor::SendPDUReplay (ClParamList *ai_pParams)
{
  //## begin ClDptcherCommsAdaptor::SendPDUReplay%960967033.body preserve=yes
	void * thePointer ;
	ClPDU * poPDU ;
	m4uint32_t idSession;
	m4int64_t  ipetitionid ;
	m4int_t id ;
	ClDspRequest * poClientReq ;
	m4return_t iRet ;
	ClDispatcherInvoker * poInvoker = ClDispatcherInvoker::Instance () ;
    int closeConnection = 1;


	if ( M4_ERROR == ai_pParams -> GetPointer ( "CLIENT_BUFFER" , thePointer ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONLAYERPROTOCOLERROR,
			"Error running communication layer protocol." );		

		return M4_ERROR ;
	}

	if (! thePointer )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_SENDINGREPLYERROR,
			"Error sending reply. Unable to get reply to the client." );

		return M4_ERROR ;
	}


	poClientReq = ( ClDspClientRequestPDU * ) thePointer ;
	
	if( ! ( poPDU = (ClPDU*) poClientReq -> GetBufferOut () ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_SENDINGREPLYERROR,
			"Error sending reply. Unable to get reply to the client." );
		return M4_ERROR ;
	}


	if ( M4_ERROR == ai_pParams -> GetInt ( "SESSIONID" , id ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONLAYERSESSIONIDERROR,
			"Error getting Session Identifier.");		
		return M4_ERROR ;
	}

	idSession = id ;

	ipetitionid = poClientReq -> GetPetitionID () ;
/*	if ( M4_ERROR == ai_pParams -> GetInt ( "PDUID"  , id ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONLAYERPROTOCOLERROR,
			"Error running communication layer protocol." );		

		return M4_ERROR ;
	}

	ipetitionid = id ;
*/
	m_oReqContainer.AddReq ( ipetitionid  , poClientReq  ) ;

	M4_SRV_DEBUG( "PDU reply to session %0:s PetitionId %1:s",
		idSession << (m4uint32_t)ipetitionid ) ;

	
	// leo el parámetro M4_CLOSE_CONNECTION.
    // Desde el API java se manda este parámetro para que no cierre la conexión
    // después de enviar la pdu de reconexión.
    if (NULL != poPDU)
    {
        M4DataStorage* ds = poPDU->GetDataStorage();
        
        // get the taglist in the pdu.
        ClTagList tl;
        tl.DataStorage2TagList(ds);
        tl.Dump();
        
        // look for M4_CLOSE_CONNECTION tag.
        // 0 - does not close the connection
        // != 0 - close the connection.
        char* tagValue = new char[64];
        if (M4_SUCCESS == tl.GetArg("M4_CLOSE_CONNECTION", tagValue, 64))
        {
            if (!strcmp(tagValue, "0"))
            {
                closeConnection = 0;
            }
        }
        delete tagValue;
    }

        
    if ( M4_ERROR == ( iRet = m_poCommunications -> Write (idSession, ipetitionid , poPDU , POST_WRITE_PDU_PROTOCOL_CONNECTION , poInvoker ) ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_REPLYSENDINGERROR,
			"Error sending reply. Unable to send reply" );

		if ( M4_ERROR == m_oReqContainer.RemoveReq ( ipetitionid  , poClientReq  ) )
		{
			M4_SRV_ERROR(M4_ERR_DPCH_NOREMOVEDPCHREQUEST,
				"Error removing Dispatcher request. Cannot find the request %0:s to remove.",
				(m4uint32_t)ipetitionid );			
		}

		M4_MDBG_DELETE( (ClDspRequest *)poClientReq, delete poClientReq ) ;
	}

    if (closeConnection != 0)
    {
        CloseConnection (ai_pParams);
    }

	return iRet;

  //## end ClDptcherCommsAdaptor::SendPDUReplay%960967033.body
}

m4return_t ClDptcherCommsAdaptor::Start ()
{
  //## begin ClDptcherCommsAdaptor::Start%960967040.body preserve=yes
	ClDispatcherInvoker * poInvoker = ClDispatcherInvoker::Instance () ;
	m4char_t *StrAux=NULL ;


	if ( M4_ERROR == poInvoker -> RegisterAction (  POST_ACCEPT_PDU_PROTOCOL_CONNECTION , POST_ACCEPT_PDU_ACTION_ID , this ) ) {
		StrAux = POST_ACCEPT_PDU_PROTOCOL_CONNECTION ;					
	} else if ( M4_ERROR == poInvoker -> RegisterAction (  POST_ACCEPT_XML_PROTOCOL_CONNECTION , POST_ACCEPT_XML_ACTION_ID , this ) ) {
		StrAux =  POST_ACCEPT_XML_PROTOCOL_CONNECTION ;					
		return M4_ERROR ;
	} else if ( M4_ERROR == poInvoker -> RegisterAction (  POST_READ_PDU_PROTOCOL_CONNECTION , POST_READ_PDU_ACTION_ID , this ) ) {
		StrAux =  POST_READ_PDU_PROTOCOL_CONNECTION ;					
	}  else if ( M4_ERROR == poInvoker -> RegisterAction (  POST_READ_XML_PROTOCOL_CONNECTION , POST_READ_XML_ACTION_ID , this ) ) {
		StrAux =  POST_READ_XML_PROTOCOL_CONNECTION ;					
	}  else if ( M4_ERROR == poInvoker -> RegisterAction (  POST_WRITE_PDU_PROTOCOL_CONNECTION , POST_WRITE_PDU_ACTION_ID , this ) ) {
		StrAux =  POST_WRITE_PDU_PROTOCOL_CONNECTION ;					
	}  else if ( M4_ERROR == poInvoker -> RegisterAction (  POST_WRITE_XML_PROTOCOL_CONNECTION , POST_WRITE_XML_ACTION_ID , this ) ) {
		StrAux = POST_WRITE_XML_PROTOCOL_CONNECTION ;					
	}  else if ( M4_ERROR == poInvoker -> RegisterAction (  REPLY_XML , REPLY_XML_ID , this ) ) {
		StrAux = REPLY_XML ;					
	}  else if ( M4_ERROR == poInvoker -> RegisterAction (  REPLY_PDU , REPLY_PDU_ID , this ) ) {
		StrAux = REPLY_PDU ;					
	}  else if ( M4_ERROR == poInvoker -> RegisterAction (  SEND_XML_TO_APPCTL , SEND_XML_TO_APPCTL_ID , this ) ) {
		StrAux = SEND_XML_TO_APPCTL ;					
	}  else if ( M4_ERROR == poInvoker -> RegisterAction (  CLOSE_CONNECTION, CLOSE_CONNECTION_ID , this ) ) {
		StrAux = CLOSE_CONNECTION ;					
	}  else if ( M4_ERROR == poInvoker -> RegisterAction (  CONFIGURE_PORTS, CONFIGURE_PORTS_ACTION_ID , this ) ) {
		StrAux = CONFIGURE_PORTS ;					
	}  else if ( M4_ERROR == poInvoker -> RegisterAction (  POST_READ_APPCTL_CONNECTION, POST_READ_APPCTL_ACTION_ID, this ) ) {
		StrAux = CONFIGURE_PORTS ;					
	}  else if ( M4_ERROR == poInvoker -> RegisterAction ( APPCTL_REPLY, APPCTL_REPLY_ID , this ) ) {
		StrAux = APPCTL_REPLY ;
	}

	if ( StrAux) { 
		// M4_ERR_DPCH_NOINITIALIZEAGENTACTION = "Error starting Agent for %0:s.\n Action "%1:s" not registered.";
		M4_SRV_ERROR(M4_ERR_DPCH_NOINITIALIZEAGENTACTION, 
			"Error starting Agent for %0:s. Action %1:s not registered.",
			"Communication Management" << StrAux);					
		return M4_ERROR ;
	} else {
		return M4_SUCCESS ;
	} ;
  //## end ClDptcherCommsAdaptor::Start%960967040.body
}

m4return_t ClDptcherCommsAdaptor::SendXMLtoAppCtl (m4pchar_t ai_szHost, m4uint32_t ai_iPort, ClDspAppCtlRequest *ai_poRequest)
{
  //## begin ClDptcherCommsAdaptor::SendXMLtoAppCtl%962179780.body preserve=yes
	m4int32_t hSessionASCII;
	m4int64_t  xmlid ;
	m4return_t iRet ;
	m4string_t strHost ;
	m4char_t * pAux ;
	// ClDspRequest * poRequest ;
	ClDispatcherInvoker * poInvoker = ClDispatcherInvoker::Instance () ;


	hSessionASCII = (m4int32_t)ClIdGenerator::GetNewId () ;

	strHost = ai_szHost ;

	size_t SizeAux ;

	if (! ( pAux = (char*) ai_poRequest ->GetBufferIn () ) )
	{
		return M4_ERROR; 
	}

	if ( ! ( xmlid = ai_poRequest ->GetPetitionID() ) )
	{
		return M4_ERROR ;
	}

	SizeAux = strlen ( pAux ) + 1;

	iRet = m_poCommunications -> OpenConnection(hSessionASCII , strHost , ai_iPort ,M4_COMM_ASCII_PROTOCOL);

	if(M4_SUCCESS!=iRet)
	{
		M4_SRV_ERROR (M4_ERR_DPCH_OPENINGCTRLCONNECTIONERROR,
			"Error opening connection to the Application Controller on host '%0:s'. Petition Id %1:s" , ai_szHost << (m4uint32_t) xmlid );
	}
	else
	{
		M4_SRV_DEBUG( "XML to AppCtl , Session %0:s XMLid %1:s. Data %2:s",
			hSessionASCII << (m4uint32_t)xmlid << pAux ) ;

		ai_poRequest -> SetConnectionID ( hSessionASCII  ) ;

		m_oReqContainer.AddReq ( hSessionASCII , ai_poRequest) ;

		if ( M4_ERROR == ( iRet=m_poCommunications -> StartListening (hSessionASCII , POST_READ_APPCTL_CONNECTION,poInvoker  ) ) )
		{
			M4_SRV_ERROR_N(M4_ERR_DPCH_SENDINGCTRLREPLYERROR,
				"Error Start Listening to the App. Controller." );

		}
		if ( M4_ERROR == ( iRet=m_poCommunications -> Write(hSessionASCII , xmlid , pAux, SizeAux) ) )
		{
			M4_SRV_ERROR_N(M4_ERR_DPCH_SENDINGCTRLREPLYERROR,
				"Error sending reply to the App. Controller." );

		}
		else
		{
			ai_poRequest ->WaitRead () ;

			if ( M4_ERROR == ( iRet = ai_poRequest->GetResult ( ) ) )
			{
				M4_SRV_ERROR ( M4_ERR_DPCH_ERRORFROMAPPCTL , 
					"M4 Application Controler on host '%0:s' returned an error. Petition Id: '%1:d'." , ai_szHost << (m4uint32_t)xmlid ) ;
			}
		}

//		m_oReqContainer.RemoveReq ( xmlid , poRequest ) ;
		M4_MDBG_DELETE( (ClDspRequest * )ai_poRequest, delete ai_poRequest) ;

	}

	return iRet  ;
  //## end ClDptcherCommsAdaptor::SendXMLtoAppCtl%962179780.body
}

m4return_t ClDptcherCommsAdaptor::ProcessAction (ClActionIdentifier ai_oActionIdentifier, ClParamList *ai_poParamList, ClBaseAction *&ao_poAction)
{
  //## begin ClDptcherCommsAdaptor::ProcessAction%963314074.body preserve=yes
	ClBaseAction * poAction = NULL ;
	ClParamList *poParamList = 0;
	m4return_t RetValue = M4_FALSE;
	m4int_t iConnectionID = 0;
	void * poPDUPointer = 0;
	void * poBufferPointer = 0;
	m4int64_t iPetitionID  = 0;
	void * poRequest = 0;
	m4pchar_t	pcAdmHost = NULL ;
	m4bool_t	bAllowedAdmHost ;
	ClDispatcherInvoker * poInvoker ;
	hostent* pHostInfo ;
	string sIPAddress = "" ;	

	switch ( ai_oActionIdentifier )
	{
	case POST_ACCEPT_PDU_ACTION_ID :
		M4_MDBG_NEW(  (ClBaseAction *)poAction, poAction = new ClPostAcceptPDUAction(this) );		
		//return PostAcceptClientPDU ( ai_poParamList ) ;

		if(M4_ERROR==ai_poParamList->GetM4Return(M4_PARAM_RETURN_RESULT,RetValue))
		{
			M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONLAYERPROTOCOLERROR,
				"Error running communication layer protocol. Error recovering RetValue.");		
			return M4_ERROR;
		}
			
		if(M4_ERROR==ai_poParamList->GetInt(M4_PARAM_CONNECTIONID, iConnectionID))
		{
			M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONLAYERPROTOCOLERROR,
				"Error running communication layer protocol. Error recovering Session ID.");		
			return M4_ERROR;
		}

		M4_MDBG_NEW(  poParamList, poParamList = new ClParamList ) ;
		poParamList -> SetM4Return (M4_PARAM_RETURN_RESULT,RetValue) ;
		poParamList -> SetInt (M4_PARAM_CONNECTIONID, iConnectionID) ;

		break ;
	case POST_ACCEPT_XML_ACTION_ID :

		if(M4_ERROR==ai_poParamList->GetInt(M4_PARAM_CONNECTIONID, iConnectionID))
		{
			M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONXMLPROTOCOLERROR,
				"Error running communication layer XML protocol.  Error recovering Session ID." );
			return M4_ERROR;
		}

		// eh 165963
		if(M4_ERROR == ai_poParamList->GetString( M4_PARAM_REMOTE_HOST_NAME, pcAdmHost ))
		{
			// Puesto de administración desconocido.
			M4_SRV_ERROR( M4_ERR_DPCH_DENIED_ADMIN_HOST, "Conexión de administración denegada al realizarse desde un puesto no autorizado ('%0:s'). Consulte con su administrador.", "" );
			return M4_ERROR;
		}

		// bg 166631
		pHostInfo = gethostbyname( pcAdmHost ) ;
		if( pHostInfo != NULL )
		{
			sIPAddress = inet_ntoa( *(in_addr*)pHostInfo->h_addr ) ;
		}

		// Comprobamos si la conexión desde el puesto cliente está permitida.
		CheckAdminHost( pcAdmHost, sIPAddress.c_str(), bAllowedAdmHost ) ;

		M4_MDBG_NEW(  poParamList, poParamList = new ClParamList ) ;

		if( bAllowedAdmHost == M4_TRUE )
		{
			M4_MDBG_NEW(  (ClBaseAction *)poAction, poAction = new ClPostAcceptXMLAction(this) );
			//return PostAccepXML ( ai_poParamList ) ;

			if(M4_ERROR==ai_poParamList->GetM4Return(M4_PARAM_RETURN_RESULT,RetValue))
			{
				M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONXMLPROTOCOLERROR,
					"Error running communication layer XML protocol. Error recovering RetValue." );
				return M4_ERROR;
			}

			poParamList -> SetM4Return  (M4_PARAM_RETURN_RESULT,RetValue) ;
			poParamList -> SetInt (M4_PARAM_CONNECTIONID, iConnectionID) ;
		}
		else
		{
			// Puesto de administración no autorizado.
			M4_SRV_ERROR( M4_ERR_DPCH_DENIED_ADMIN_HOST, "Conexión de administración denegada al realizarse desde un puesto no autorizado ('%0:s'/'%1:s'). Consulte con su administrador.", pcAdmHost << sIPAddress.c_str() );

			poInvoker = ClDispatcherInvoker::Instance () ;

			poParamList -> SetInt ( "SESSIONID" , iConnectionID ) ;
			poParamList -> SetInt ( "STOP_LISTENING_FALG" , 0 ) ;
			
			// Cerramos la conexión.
			return poInvoker -> ExecuteAction ( CLOSE_CONNECTION , poParamList ) ;
		}


		break ;
	case POST_READ_PDU_ACTION_ID :
		M4_MDBG_NEW(  (ClBaseAction *)poAction, poAction = new ClPostReadPDUAction(this) );

		if(M4_ERROR==ai_poParamList->GetM4Return(M4_PARAM_RETURN_RESULT,RetValue))
		{
			M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONLAYERPROTOCOLERROR,
				"Error running communication layer protocol." );
            
            delete poAction;
			return M4_ERROR ;
		}

		if(M4_ERROR==ai_poParamList->GetInt(M4_PARAM_CONNECTIONID, iConnectionID))
		{
			M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONLAYERSESSIONIDERROR,
				"Error getting Session Identifier." );		

            delete poAction;
			return M4_ERROR;
		}

		// bg 115372
		// No es un error, ya que de momento no se va a usar la PDU.
		ai_poParamList->GetPointer( M4_PARAM_READED_PDU, poPDUPointer ) ;
		/*
		if(M4_ERROR==ai_poParamList->GetPointer(M4_PARAM_READED_PDU,poPDUPointer))
		{
			M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONLAYERPROTOCOLERROR,
				"Error running communication layer protocol. Error recovering BUFFER.");		

            delete poAction;
			return M4_ERROR;
		}
		*/

		M4_MDBG_NEW(  poParamList, poParamList = new ClParamList ) ;
		poParamList -> SetM4Return  (M4_PARAM_RETURN_RESULT,RetValue) ;
		poParamList -> SetInt (M4_PARAM_CONNECTIONID, iConnectionID) ;

		// bg 115372
		if( poPDUPointer != NULL )
		{
			poParamList->SetPointer (M4_PARAM_READED_PDU,poPDUPointer) ;
		}

        /*
        // En el API comunicaciones java enviamos un tag para que no cierre la 
        // conexión después de la PDU de reconexión.
        // Si tenemos este tag ponemos un parámetro, para que nos llegue al método de 
        // ClDptcherCommsAdaptor::SendPDUReplay.
        if (NULL != poPDUPointer)
        {
            ClPDU* pdu = (ClPDU*)poPDUPointer;
            if (pdu->GetIdPDU() == M4_ID_PDU_CONNECT)
            {
                ClConnectPDU* pduConnect = (ClConnectPDU*)poPDUPointer;
                M4DataStorage* ds = pduConnect->GetDataStorage();
                
                // get the taglist in the pdu.
                ClTagList tl;
                tl.DataStorage2TagList(ds);
                
                // look for M4_CLOSE_CONNECTION tag.
                // 0 - does not close the connection
                // != 0 - close the connection.
                char* tagValue = new char[64];
                if (M4_SUCCESS == tl.GetArg("M4_CLOSE_CONNECTION", tagValue, 64))
                {
                    if (!strcmp(tagValue, "0"))
                    {
                        poParamList->SetInt("M4_CLOSE_CONNECTION", 0);
                    }
                }
                delete tagValue;
            }
        }
		*/

		break ;
	case POST_READ_XML_ACTION_ID :
		M4_MDBG_NEW(  (ClBaseAction *)poAction,  poAction = new ClPostReadXMLAction( this) ) ;

		// Important Note: It is not possible to run the
		// next line because it is in the m4communication
		// worker thread, and so, there can be errors to
		// execute m4communication methods (this was a bug)
		
		//	return PostReadXML ( ai_poParamList ) ;

		if(M4_ERROR==ai_poParamList->GetM4Return(M4_PARAM_RETURN_RESULT,RetValue))
		{
			M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONXMLPROTOCOLERROR,
				"Error running communication layer XML protocol. Error recovering RetValue." );
			return M4_ERROR;
		}

		if(M4_ERROR==ai_poParamList->GetInt(M4_PARAM_CONNECTIONID, iConnectionID))
		{
			M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONXMLSESSIONIDERROR,
				"Error getting Client XML Session Identifier." );		
			return M4_ERROR;
		}

		M4_MDBG_NEW(  poParamList, poParamList = new ClParamList ) ;


		if(M4_ERROR!=ai_poParamList->GetPointer(M4_PARAM_READED_BUFFER,poBufferPointer))
		{
			//M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONXMLPROTOCOLERROR,
			//	"Error running communication layer XML protocol. Error recovering BUFFER." );
			//return M4_ERROR;

			poParamList->SetPointer (M4_PARAM_READED_BUFFER,poBufferPointer) ;
		}

		poParamList -> SetM4Return (M4_PARAM_RETURN_RESULT,RetValue) ;
		poParamList -> SetInt (M4_PARAM_CONNECTIONID, iConnectionID) ;

		break ;

	case POST_WRITE_PDU_ACTION_ID :
		M4_MDBG_NEW(  (ClBaseAction *)poAction, poAction = new ClPostWritePDUAction(this) );
		//return PostWriteClientPDU ( ai_poParamList ) ;

		if ( M4_ERROR == ai_poParamList -> GetInt64 ( M4_PARAM_PETITION_ID , iPetitionID ) )
		{
			M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONLAYERPROTOCOLERROR,
				"Error running communication layer protocol." );		
			return M4_ERROR ;
		}

		M4_MDBG_NEW(  poParamList, poParamList = new ClParamList ) ;
		poParamList -> SetInt64 ( M4_PARAM_PETITION_ID , iPetitionID ) ;

		break ;

	case POST_WRITE_XML_ACTION_ID :
		M4_MDBG_NEW(  (ClBaseAction *)poAction,  poAction = new ClPostWriteXMLAction(this) ) ;
		//return PostWriteXML (ai_poParamList ) ;

		if ( M4_ERROR == ai_poParamList -> GetInt64 ( M4_PARAM_PETITION_ID , iPetitionID ) )
		{
			M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONXMLPROTOCOLERROR,
				"Error running communication layer XML protocol." );

			return M4_ERROR ;
		}

		M4_MDBG_NEW(  poParamList, poParamList = new ClParamList ) ;
		poParamList -> SetInt64 ( M4_PARAM_PETITION_ID , iPetitionID ) ;
		break ;
	
	case REPLY_XML_ID :
		M4_MDBG_NEW(  (ClBaseAction *)poAction, poAction = new ClSendXMLAction ( this ) ) ;
		break;
	case SEND_XML_TO_APPCTL_ID :
		M4_MDBG_NEW(  (ClBaseAction *)poAction, poAction = new ClSendXMLToAppCtl ( this ) ) ;
		poAction -> SetParams ( ai_poParamList ) ;
        // bugid: 0094322.
        RetValue = poAction -> Execute ()  ;
        delete poAction;
        return RetValue;
		break;
	case REPLY_PDU_ID :
		M4_MDBG_NEW(  (ClBaseAction *)poAction, poAction = new ClSendPDUAction ( this ) ) ;
		break ;
	case CLOSE_CONNECTION_ID :
		M4_MDBG_NEW(  (ClBaseAction *)poAction, poAction = new ClCloseConnectionAction ( this ) ) ;
		break ;
	case CONFIGURE_PORTS_ACTION_ID :
		M4_MDBG_NEW(  (ClBaseAction *)poAction, poAction = new ClConfigureComms ( this ) ) ;
		poAction -> SetParams ( ai_poParamList ) ;
        // bugid: 0094322.
		RetValue = poAction -> Execute ()  ;
        delete poAction;
        return RetValue;
	case POST_READ_APPCTL_ACTION_ID :
		return PostReadAppctl ( ai_poParamList ) ;
	case APPCTL_REPLY_ID :
		M4_MDBG_NEW(  (ClBaseAction *)poAction,  poAction = new ClAppCtlReplyAction ( this ) ) ;
		poAction -> SetParams ( ai_poParamList ) ;
        // bugid: 0094322.
		RetValue = poAction -> Execute ()  ;
        delete poAction;
        return RetValue;
		break ;
	}	

	if ( poAction )
	{

		// Get the copy of the parameter list because the m4communication
		// layer delete it after the end of the current method
		// ("ClDptcherCommsAdaptor::ProcessAction"). But do not get the
		// copy to the four first actions (these are not called by the 
		// m4communication layer)
		
		if (0 == poParamList)
			poParamList = ai_poParamList;

		poAction -> SetParams ( poParamList ) ;
		m_poExecutiveAgent ->PushAction ( poAction ) ;
	}
	return M4_SUCCESS ;
  //## end ClDptcherCommsAdaptor::ProcessAction%963314074.body
}

m4return_t ClDptcherCommsAdaptor::CloseConnection (ClParamList *ai_pParamList)
{
  //## begin ClDptcherCommsAdaptor::CloseConnection%963417633.body preserve=yes
	m4int_t iConnectionID , iStopListnFalg = 0;


    ai_pParamList->Dump();

	if ( M4_ERROR == ai_pParamList ->GetInt ( "SESSIONID" , iConnectionID ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONLAYERSESSIONIDERROR,
			"Error getting Session Identifier.");		
		return M4_ERROR ;
	}

	ai_pParamList -> GetInt ( "STOP_LISTENING_FALG" , iStopListnFalg ) ;

	if ( iStopListnFalg )
	{
		if( M4_ERROR == m_poCommunications -> StopListening(iConnectionID) )
		{
			// bg 115372
			m4Trace	(cerr <<"Connection already stopped."<<endl);
			/*
			M4_SRV_ERROR_N(M4_ERR_DPCH_NOSTOPLISTENING,
				"Error stopping connection. Unable to stop listening" );		
			*/
			return M4_ERROR ;
		}
	}

	if ( M4_ERROR == m_poCommunications -> CloseConnection(iConnectionID) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_CLOSINGCONNECTIONERROR,
			"Error closing connection." );		

		return M4_ERROR ;
	}

	return M4_SUCCESS ;
  //## end ClDptcherCommsAdaptor::CloseConnection%963417633.body
}

m4return_t ClDptcherCommsAdaptor::StopCommunicationLayer ()
{
  //## begin ClDptcherCommsAdaptor::StopCommunicationLayer%972919262.body preserve=yes

	if ( m_poCommunications ) {
		return m_poCommunications -> StopCommunicationLayer() ;
	} else {
		return M4_ERROR ;
	} ;
  //## end ClDptcherCommsAdaptor::StopCommunicationLayer%972919262.body
}

m4bool_t ClDptcherCommsAdaptor::IsStarted ()
{
  //## begin ClDptcherCommsAdaptor::IsStarted%972986737.body preserve=yes
	return m_bStarted ;
  //## end ClDptcherCommsAdaptor::IsStarted%972986737.body
}

m4return_t ClDptcherCommsAdaptor::PostReadAppctl (ClParamList *ai_pParamList)
{
  //## begin ClDptcherCommsAdaptor::PostReadAppctl%981385469.body preserve=yes
	void * thepointer ;
	m4return_t RetValue = M4_ERROR ;
	m4int_t iConnectionID ;
	m4int64_t iPetitionID;
	ClParamList * poParamList = NULL , * poCloseParamList ;
	ClDispatcherInvoker * poInvoker = ClDispatcherInvoker::Instance () ;
	ClDspAppCtlRequest * poRequest ;


	if(M4_ERROR==ai_pParamList->GetM4Return(M4_PARAM_RETURN_RESULT,RetValue))
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONXMLPROTOCOLERROR,
			"Error running communication layer XML protocol. Error recovering RetValue." );
	}
	else
	{
		if(M4_ERROR==ai_pParamList->GetInt(M4_PARAM_CONNECTIONID, iConnectionID))
		{
			M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONXMLSESSIONIDERROR,
				"Error getting Appctl Session Identifier.");		
			return M4_ERROR ;
		}
		if(M4_ERROR!=RetValue )
		{
			if(M4_ERROR==ai_pParamList->GetPointer(M4_PARAM_READED_BUFFER,thepointer))
			{
				M4_SRV_ERROR_N(M4_ERR_DPCH_COMMUNICATIONXMLPROTOCOLERROR,
					"Error running communication layer XML protocol. Error recovering BUFFER." );
			}
			else
			{
				M4_MDBG_NEW(  poParamList, poParamList = new ClParamList ) ;

				iPetitionID = ClIdGenerator::GetNewId () ;

				ClDspXMLRequest * poRequest = NULL ;
				
				M4_MDBG_NEW(  (ClDspRequest *)poRequest, poRequest = new 
					ClDspXMLRequest ( thepointer , iPetitionID , iConnectionID ) ) ;

				poParamList -> SetPointer ( "REQUEST" , poRequest ) ;

				if ( M4_ERROR == ParseXML ( poParamList ) )
				{
					RetValue = M4_ERROR ;
				}
                // bugid: 0094322.
                delete poParamList;
                delete poRequest;
			}
		}
	}

	if ( M4_ERROR == RetValue )
	{
		M4_MDBG_NEW(  poCloseParamList, poCloseParamList = new ClParamList ) ;

		poCloseParamList -> SetInt ( "SESSIONID" , iConnectionID ) ;

		poCloseParamList -> SetInt ( "STOP_LISTENING_FALG" , 1 ) ;

		poInvoker -> ExecuteAction ( CLOSE_CONNECTION , poCloseParamList ) ;

		if ( M4_ERROR == m_oReqContainer.RemoveReq ( iConnectionID , (ClDspRequest *&) poRequest ) )
		{
			return M4_ERROR ;
		}

		poRequest -> SetResult ( M4_ERROR ) ;

		poRequest -> ReadReady ( ) ;
	}

	return M4_SUCCESS ;
  //## end ClDptcherCommsAdaptor::PostReadAppctl%981385469.body
}

m4return_t ClDptcherCommsAdaptor::ProcessAppctlReply (m4return_t ai_iResult, m4uint64_t ai_iRequestID, ClDspRequest *ai_poRequest, m4pchar_t ai_szAppCtlLog)
{
  //## begin ClDptcherCommsAdaptor::ProcessAppctlReply%981535909.body preserve=yes
	ClDspAppCtlRequest * poRequest ;
	ClLogSystem * poLog ;
	m4uint32_t iConnectionID ;
	ClParamList * poCloseParamList ;
	ClDispatcherInvoker * poInvoker = ClDispatcherInvoker::Instance () ;


	iConnectionID = ai_poRequest->GetConnectionID () ;

	if ( M4_ERROR == m_oReqContainer.RemoveReq ( iConnectionID , (ClDspRequest *&) poRequest ) )
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOREMOVEDPCHREQUEST,
			"Error removing Dispatcher request. Cannot find the request %0:s to remove.",
			(m4uint32_t)iConnectionID );			
		return M4_ERROR ;
	}

	if ( ai_iRequestID != poRequest ->GetPetitionID () )
	{
		M4_ASSERT ( "Error en el protocolo con el Controler" ) ;

		M4_MDBG_NEW(  poCloseParamList, poCloseParamList = new ClParamList ) ;

		poCloseParamList -> SetInt ( "SESSIONID" , iConnectionID ) ;

		poCloseParamList -> SetInt ( "STOP_LISTENING_FALG" , 1 ) ;

		poInvoker -> ExecuteAction ( CLOSE_CONNECTION , poCloseParamList ) ;


		return M4_ERROR ;
	}

	if ( ( ai_szAppCtlLog  ) && ( poLog = poRequest -> GetLog () ) )
	{
		poLog -> SetSerialCodeString ( ai_szAppCtlLog ) ;
	}

	poRequest -> SetResult ( ai_iResult ) ;

	poRequest -> ReadReady ( ) ;

	ai_poRequest ->SetResult ( M4_SUCCESS ) ;


	return M4_SUCCESS ;
  //## end ClDptcherCommsAdaptor::ProcessAppctlReply%981535909.body
}

m4return_t ClDptcherCommsAdaptor::InitComms (m4pchar_t ai_szCypherMethod, m4uint32_t ai_iNumWorkers)
{
  //## begin ClDptcherCommsAdaptor::InitComms%1000797470.body preserve=yes
	m4string_t 	szKeyf ;
	m4string_t	szCertf ;
	m4pchar_t	szCertPath ;
	m4string_t strOidx ;

	m_poServerRepository -> SetParams ( m_iAdminPort , m_iMaxStartRetries , m_iServerStartTimeout, m_iDumpTrace, m_iSessionsWeight, m_iSrvMemWeight, m_iSysMemWeight, m_iCPUWeight, m_iExecTimeWeight, m_iExecTimeThreshold );

	ClDispatcherInvoker * poInvoker = ClDispatcherInvoker::Instance () ;
	
	m_szCypherMethod = ai_szCypherMethod ;

	M4_MDBG_NEW(  m_poCommunications , m_poCommunications = new ClCommunication ( m_iControlPort ) ) ;

	M4_MDBG_DELETE(  m_szHost, delete m_szHost ) ;
	M4_MDBG_NEW(  m_szHost, m_szHost = new m4char_t [100] ) ;

	if ( M4_ERROR == gethostname ( m_szHost , 100 ) )
	{
		M4_SRV_ERROR_N(M4_ERR_DPCH_NOGETHOSTNAME,
			"Error starting Agent for Communication Management. Cannot get the Dispatcher Host Name." );					
		return M4_ERROR ;
	}

	
	if ( M4_ERROR == m_poCommunications -> StartAccepting (m_iAdminPort, 
	POST_ACCEPT_XML_PROTOCOL_CONNECTION , poInvoker , M4_COMM_ASCII_PROTOCOL ) )
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOSTARTACCEPTING, 
			"Error starting Agent for Communication Management. Couln't initialize communication layer to listen to %0:s.",
			"Administration Client");					
		return M4_ERROR ;
	}


	if ( M4_ERROR == m_poCommunications -> StartAccepting (m_iClientPort, 
	POST_ACCEPT_PDU_PROTOCOL_CONNECTION , poInvoker , M4_COMM_PDU_PROTOCOL ) )
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOSTARTACCEPTING, 
			"Error starting Agent for Communication Management. Couln't initialize communication layer to listen to %0:s.",
			"Application Client");					
		return M4_ERROR ;
	}

	//En Windows el script pone la variable M4_APP_INSTALLATION_ROOT
	if ( ! ( szCertPath = getenv ( "M4_APP_INSTALLATION_ROOT" ) ) )
	{
		//En unix la variable es M4_APP_INSTALL_ROOTDIR
		szCertPath = getenv ( "M4_APP_INSTALL_ROOTDIR" ) ;
	}

	if ( szCertPath )
	{

		szKeyf = szCertPath ;
		szCertf = szCertPath ;
	}
	else
	{	// Bg 122204.
		// Asumimos que se levantó como servicio, ya que la variable de entorno la establece el batch.
		szKeyf = "..";
		szCertf = "..";
	}

	szKeyf += KEYF ;
	szCertf += CERTF ;

	if ( M4_ERROR == m_poCommunications -> StartAccepting (m_iClientSSLPort, 
	POST_ACCEPT_PDU_PROTOCOL_CONNECTION , poInvoker , M4_COMM_PDU_PROTOCOL , szCertf , szKeyf , m_szCypherMethod ) )
	{
		M4_SRV_ERROR(M4_ERR_DPCH_NOSTARTACCEPTING, 
			"Error starting Agent for Communication Management. Couln't initialize communication layer to listen to %0:s.",
			"Application Client");					
		return M4_ERROR ;
	}


	if (0 == m_poExecutiveAgent || M4_ERROR == m_poExecutiveAgent->Start ( ai_iNumWorkers ) )
	{
		cout <<	"Error initializing Dispatcher. Agent for Executing Actions not initilized." << endl ;			
		return M4_ERROR ;
	}

	m_bStarted = M4_TRUE ;

	return M4_SUCCESS ;

  //## end ClDptcherCommsAdaptor::InitComms%1000797470.body
}


m4return_t	ClDptcherCommsAdaptor::CheckAdminHost( m4pcchar_t ai_pccHostName, m4pcchar_t ai_pccHostIP, m4bool_t& ao_bAllowed )
{
	m4pchar_t	pcCfgPath ;
	FILE*		pfAdmHosts = NULL ;
	m4char_t	acLine[ 512 ] ;
	m4pchar_t	pcLine ;
	int			iLen ;
	int			iNumLines = 0 ;
	hostent*	pHostInfo ;
	string		sIPAddress = "" ;	


	ao_bAllowed = M4_FALSE ;

	pcCfgPath = ClAdminAgent::GetDspAdminHostsConfigPath() ;
	
	
	pfAdmHosts = fopen( pcCfgPath, "rt" ) ;

	if( pfAdmHosts == NULL )
	{
		// No existe fichero, todos los puestos están autorizados.
		ao_bAllowed = M4_TRUE ;
		return M4_SUCCESS ;
	}

	do
	{
		*acLine = '\0' ;

		pcLine = fgets( (m4pchar_t) acLine, 512, pfAdmHosts ) ;

		if( pcLine != NULL )
		{
			iLen = strlen( pcLine ) ;

			// Quitamos salto de linea.
			if( iLen > 0 && pcLine[ iLen - 1 ] == '\n' )
			{
				iLen-- ;
				pcLine[ iLen ] = '\0' ;
			}

			if( iLen > 0 && pcLine[ iLen - 1 ] == '\r' )
			{
				iLen-- ;
				pcLine[ iLen ] = '\0' ;
			}

			// Quitamos espacios al final.
			while( iLen > 0 && ( pcLine[ iLen - 1 ] == 32 || pcLine[ iLen - 1 ] == '\t' ) )
			{
				iLen-- ;
				pcLine[ iLen ] = '\0' ;
			}

			// Quitamos espacios al comienzo.
			while( iLen > 0 && ( *pcLine == 32 || *pcLine == '\t' ) )
			{
				iLen-- ;
				pcLine++ ;
			}

			// Ignoramos líneas en blanco.
			if( iLen == 0 )
			{
				continue ;
			}

			// Ignoramos líneas de comentario.
			if( pcLine[ 0 ] == ';' )
			{
				continue ;
			}
			
			iNumLines++ ;

			// Comodín, todos los puestos están autorizados.
			if( iLen == 1 && pcLine[ iLen - 1 ] == '*' )
			{
				ao_bAllowed = M4_TRUE ;
				break ;
			}
			else if( strcmpi( pcLine, ai_pccHostName ) == 0 )
			{
				// Host name específico autorizado
				ao_bAllowed = M4_TRUE ;
				break ;
			}
			else if( strcmpi( pcLine, ai_pccHostIP ) == 0 )
			{
				// Host IP específica autorizada
				ao_bAllowed = M4_TRUE ;
				break ;
			}
			else
			{
				pHostInfo = gethostbyname( pcLine ) ;
				if( pHostInfo != NULL )
				{
					sIPAddress = inet_ntoa( *(in_addr*)pHostInfo->h_addr ) ;
					
					if( strcmpi( sIPAddress.c_str(), ai_pccHostIP ) == 0 )
					{
						// Host IP resuelta autorizada
						ao_bAllowed = M4_TRUE ;
						break ;
					}
				}
			}
		}
	}
	while( pcLine != NULL ) ;
	
	fclose( pfAdmHosts ) ;

	if( iNumLines == 0 )
	{
		// fichero sin datos, todos los puestos están autorizados.
		ao_bAllowed = M4_TRUE ;
	}

	return M4_SUCCESS ;
}
// Additional Declarations
  //## begin ClDptcherCommsAdaptor%39476AC903E4.declarations preserve=yes
  //## end ClDptcherCommsAdaptor%39476AC903E4.declarations

//## begin module%39479DC90225.epilog preserve=yes
//## end module%39479DC90225.epilog
