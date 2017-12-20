//## begin module%42931E110136.cm preserve=no
//## end module%42931E110136.cm

//## begin module%42931E110136.cp preserve=no
//	=====================================================================
//
//	 (c) Copyright 2004 Meta4 Spain S.A.
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              <file>.cpp
//	 Project:           <project>
//	 Author:            Meta4 Spain S.A.
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS, UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=====================================================================
//## end module%42931E110136.cp

//## Module: clientconnectionhttp%42931E110136; Package body
//## Subsystem: M4AppClient::src%3796F8B303AD
//## Source file: C:\m4server\m4appclient\src\clientconnectionhttp.cpp

//## begin module%42931E110136.additionalIncludes preserve=no
//## end module%42931E110136.additionalIncludes

//## begin module%42931E110136.includes preserve=yes
#include <http_errors.hpp>
#include <m4props.hpp>
//## end module%42931E110136.includes

// clientconnectionhttp
#include <clientconnectionhttp.hpp>
// clientconnection
#include <clientconnection.hpp>
// parsehttp
#include <parsehttp.hpp>
// m4string
#include <m4string.hpp>
//## begin module%42931E110136.declarations preserve=no
//## end module%42931E110136.declarations

//## begin module%42931E110136.additionalDeclarations preserve=yes
//## end module%42931E110136.additionalDeclarations


// Class ClCCConnectionHTTP 







ClCCConnectionHTTP::ClCCConnectionHTTP (M4ClString ai_strHost, m4int_t ai_iHostPort, M4ClString ai_strServer, m4int_t ai_iServerPort, M4ClString ai_strServlet, m4bool_t ai_bSSL, M4ClString ai_strCACertPath)
  //## begin ClCCConnectionHTTP::ClCCConnectionHTTP%921676533.hasinit preserve=no
  //## end ClCCConnectionHTTP::ClCCConnectionHTTP%921676533.hasinit
  //## begin ClCCConnectionHTTP::ClCCConnectionHTTP%921676533.initialization preserve=yes
    :  ClCCConnection(ai_strHost,ai_iHostPort,ai_bSSL,ai_strCACertPath,M4_RAW_COMMS_TYPE)
  //## end ClCCConnectionHTTP::ClCCConnectionHTTP%921676533.initialization
{
  //## begin ClCCConnectionHTTP::ClCCConnectionHTTP%921676533.body preserve=yes
	m_strServer=ai_strServer;
	m_iServerPort=ai_iServerPort;

	m_strServlet=ai_strServlet;

	m_cType=M4_TRUE;

	m_Parser.SetHeadersSizeLimit(MAX_POST_REQUEST_SIZE);
	m_Parser.AddNewTokenToLookFor("Result");
	m_Parser.AddNewTokenToLookFor(PRIV_KEY_HEADER);
	m_Parser.AddNewTokenToLookFor("inactivityTimeout");
	m_Parser.AddNewTokenToLookFor("Set-Cookie");

	ReleaseTSAP();
  //## end ClCCConnectionHTTP::ClCCConnectionHTTP%921676533.body
}


ClCCConnectionHTTP::~ClCCConnectionHTTP ()
{
  //## begin ClCCConnectionHTTP::~ClCCConnectionHTTP%926436108.body preserve=yes
  //## end ClCCConnectionHTTP::~ClCCConnectionHTTP%926436108.body
}



//## Other Operations (implementation)
m4return_t ClCCConnectionHTTP::ProccessHeaders (m4pchar_t &ao_pcBuffer, m4uint32_t &ao_uiBufferSize)
{
  //## begin ClCCConnectionHTTP::ProccessHeaders%921676535.body preserve=yes
	m4bool_t bHeaders=M4_TRUE;
	m4int_t iParseState=HTTP_PARSE_CONTINUE;
	
	m4bool_t connected=M4_TRUE;
	m4return_t iRetSwitch=M4_SUCCESS;
	
	m4char_t pzAuxCad[MAX_POST_REQUEST_SIZE];
	
	m4pchar_t thebuffer=ao_pcBuffer;
	
	m4uint32_t uiReaded=0,uiTotalReaded=0;;
	while(bHeaders)
	{
		thebuffer+=uiReaded;
		ao_uiBufferSize-=uiReaded;
		
		uiReaded =m_poTSAP->Read(thebuffer, ao_uiBufferSize);
		m4TraceLevel(2,cout <<"\nuiReaded="<<uiReaded<<endl);
		switch((m4int32_t)uiReaded)
		{
		case -1:
			// El server httpd ha cortado y yo tengo que  procesar lo que me ha llegado
#ifndef _UNIX
			m4TraceLevel(2,cerr <<"WSAGetLastError:"<<WSAGetLastError()<<endl);
#endif 
			m4TraceLevel(2,cerr<<"read=-1\nHasta ahora ===============\n"<<ao_pcBuffer<<"\n===============\n"<<endl);
			connected=M4_FALSE;
			bHeaders=M4_FALSE;
			continue;
			break;
		case 0:
#ifndef _UNIX
			m4TraceLevel(2,cerr <<"WSAGetLastError:"<<WSAGetLastError()<<endl);
#endif 
			m4TraceLevel(2,cerr<<"read=0\nHasta ahora ===============\n"<<ao_pcBuffer<<"\n===============\n"<<endl);
			connected=M4_FALSE;
			bHeaders=M4_FALSE;
			iRetSwitch= M4_ERROR_HTTP_312;
			continue;
			break;
		default:
			break;
		}
		uiTotalReaded+=uiReaded;
		iParseState=m_Parser.Parse(thebuffer,uiReaded);
		switch(iParseState)
		{
		case HTTP_PARSE_CONTINUE :
			if(connected)
			{
				m4TraceLevel(2,cout<<"\nHay mas\nSize="<<uiReaded);
				//m4Trace(cout<<"\nMe ha llegado esto\n==================\n"<<thebuffer<<"\n==================\n");
				m4TraceLevel(2,cout<<endl);
			}
			else
			{
				m4Trace(cerr<<"HTTP_PARSE_ERROR"<<endl);
				iRetSwitch= HTTP_PARSE_ERROR;
				bHeaders=M4_FALSE;
			}
			break;
			
		case HTTP_PARSE_HTTP_ERROR :
			m4Trace(cerr<<"Error from HTTP server\n");
			iRetSwitch=HTTP_PARSE_HTTP_ERROR;
			bHeaders=M4_FALSE;
			break;
			
		case HTTP_PARSE_BUFFER_SIZE_ERROR :
			m4Trace(cerr<<"Internal error in buffer"<<endl);
			bHeaders=M4_FALSE;
			iRetSwitch= M4_ERROR_HTTP_BUFFER;
			break;
			
		case HTTP_PARSE_END_OF_HEADERS :
			m4TraceLevel(2,cout << "NO ");
		case HTTP_PARSE_ALL_THE_HEADERS_FOUND :
			m4TraceLevel(2,cout <<"All headers found"<<endl);
			m4TraceLevel(2,cout<<"Me ha llegado esto\n________________\n"<<ao_pcBuffer<<"\n________________\n"<<endl);
			bHeaders=M4_FALSE;
			break;
		case HTTP_PARSE_ERROR:
			m4Trace(cerr<<"ERROR IN PARSE"<<endl);
			iRetSwitch=M4_ERROR;
			break;
		default:
			m4Trace(cerr<<"ERROR PARSING!!!!!:"<<iParseState<<endl);
			m4DebugExecute(if(!ao_pcBuffer) cerr <<"Recieved:\n"<< ao_pcBuffer <<endl)
			iRetSwitch=M4_ERROR;
			break;
		}
	}

	if (M4_SUCCESS!=iRetSwitch)
	{
		m_Parser.ResetParseAndTokenResults();
		if(ao_pcBuffer)
		{
			SETCODEF(M4_ERR_CONNECTION_ERROR,ERRORLOG,"Servlet error:------------------------------\n%s\n------------------------",ao_pcBuffer);
			cerr<<"Servlet error:\n------------------------------\n"<< ao_pcBuffer<<"\n------------------------\n";
		}
		NotifyIncidence(HTTP_ERROR);
		return iRetSwitch;
	}


	if (m_Parser.GetTokenResult("Result",pzAuxCad,0)==M4_SUCCESS)
	{
		m4int_t iBlank=0;
		while(pzAuxCad[iBlank]==' ')
			iBlank++;

		switch(atoi(&pzAuxCad[iBlank]))
		{
		case 301:
			m4Trace(cerr<<"Timeout de inactividad"<<endl);
			SETCODE(M4_ERR_CONNECTION_ERROR,ERRORLOG,"Inactivity Timeout in Servlet");
			m_pClientMonitor->RemoveConnection((m4handle_t)this);
			NotifyIncidence(HTTP_TIMEOUT);
			iRetSwitch= M4_ERROR_HTTP_301;
			break;
		case 302:
			m4Trace(cerr<<"Invalid Command in header"<<endl);
			SETCODE(M4_ERR_CONNECTION_ERROR,ERRORLOG,"INTERNAL CODE ERROR.Invalid Command in header");
			iRetSwitch= M4_ERROR_HTTP_302;
			break;
		case 303:
			m4Trace(cerr<<"Can't create TCPIP connection to server"<<endl);
			SETCODE(M4_ERR_CONNECTION_ERROR,ERRORLOG,"Couldn't create TCPIP connection to server");
			m_pClientMonitor->RemoveConnection((m4handle_t)this);
			NotifyIncidence(NO_SERVER);
			iRetSwitch= M4_ERROR_HTTP_303;
			break;
		case 304:
			m4Trace(cerr<<"Request length error"<<endl);
			SETCODE(M4_ERR_CONNECTION_ERROR,ERRORLOG,"INTERNAL CODE ERROR.Request length error");
			iRetSwitch= M4_ERROR_HTTP_304;
			break;
		case 305:
			m4Trace(cerr<<"Wrong size.No puede ser menor de 16"<<endl);
			SETCODE(M4_ERR_CONNECTION_ERROR,ERRORLOG,"INTERNAL CODE ERROR.Wrong size.Size < 16");
			iRetSwitch= M4_ERROR_HTTP_305;
			break;
		case 306:
			m4Trace(cerr<<"Error in comunication with server"<<endl);
			SETCODE(M4_ERR_CONNECTION_ERROR,ERRORLOG,"Comunication error with error.Broken connection");
			m_pClientMonitor->RemoveConnection((m4handle_t)this);
			NotifyIncidence(BROKEN_CONNECTION);
			iRetSwitch= M4_ERROR_HTTP_306;
			break;
		case 307:
			m4Trace(cerr<<"Timeout de actividad"<<endl);
			SETCODE(M4_ERR_CONNECTION_ERROR,ERRORLOG,"Activity Timeout in Servlet");
			m_pClientMonitor->RemoveConnection((m4handle_t)this);
			NotifyIncidence(HTTP_TIMEOUT);
			iRetSwitch= M4_ERROR_HTTP_307;
			break;
		case 308:
			m4Trace(cerr<<"Error 308"<<endl);
			SETCODE(M4_ERR_CONNECTION_ERROR,ERRORLOG,"INTERNAL CODE ERROR.HTTP header error");
			iRetSwitch= M4_ERROR_HTTP_308;
			break;
		case 309:
			m4Trace(cerr<<"Error in request"<<endl);
			SETCODE(M4_ERR_CONNECTION_ERROR,ERRORLOG,"INTERNAL CODE ERROR.Error in request");
			iRetSwitch= M4_ERROR_HTTP_309;
			break;
		case 310:
			m4Trace(cerr<<"Error in private key"<<endl);
			SETCODE(M4_ERR_CONNECTION_ERROR,ERRORLOG,"Error in private key");
			m_pClientMonitor->RemoveConnection((m4handle_t)this);
			NotifyIncidence(BROKEN_CONNECTION);
			iRetSwitch= M4_ERROR_HTTP_310;
			break;
		case 312:
			m4Trace(cerr<<"Internal servlet error"<<endl);
			SETCODE(M4_ERR_CONNECTION_ERROR,ERRORLOG,"INTERNAL CODE ERROR.HTTP_PARSE_BUFFER_SIZE_ERROR");
			m_pClientMonitor->RemoveConnection((m4handle_t)this);
			NotifyIncidence(BROKEN_CONNECTION);
			iRetSwitch= M4_ERROR_HTTP_312;
			break;
		case 100:
			m4TraceLevel(2,cout<<"Headers OK"<<endl);
			break;
		default:
			SETCODE(M4_ERR_CONNECTION_ERROR,ERRORLOG,"Unkown error from Servlet");
			m4Trace(cerr<<"Caso no valido:"<<atoi(pzAuxCad)<<" "<<pzAuxCad<<endl);
			iRetSwitch= M4_ERROR;
			break;
		}
	}

	if (M4_SUCCESS!=iRetSwitch)
	{
		m_Parser.ResetParseAndTokenResults();
		if(ao_pcBuffer)
		{
			SETCODEF(M4_ERR_CONNECTION_ERROR,ERRORLOG,"Servlet error:------------------------------\n%s\n------------------------",ao_pcBuffer);
			cerr<<"Servlet error:\n------------------------------\n"<< ao_pcBuffer<<"\n------------------------\n";
		}
		return iRetSwitch;
	}

	if (GetConnStatus()==M4_CONNECTION_STARTING)
	{
		if(m_Parser.GetTokenResult("Set-Cookie",pzAuxCad,0)!=M4_SUCCESS)
		{
			m4Trace(cerr<<"Error getting Set-Cookie result\n");
			iRetSwitch=M4_ERROR_HTTP_NO_COOKIE;
			SETCODE(M4_ERR_CONNECTION_ERROR,ERRORLOG,"Error in private key");
			m_pClientMonitor->RemoveConnection((m4handle_t)this);
			NotifyIncidence(BROKEN_CONNECTION);
		}
		else
		{
			m4int_t iBlank=0;
			while(pzAuxCad[iBlank]==' ')
				iBlank++;	
			m_strPublicCookie=&pzAuxCad[iBlank];
			m4TraceLevel(2,cout<<"Using PublicCookie="<<m_strPublicCookie<<endl);
			m_Parser.DeleteTokenToLookFor("Set-Cookie");
			SetConnStatus(M4_CONNECTION_RUNNING);
			ClTagList theArg;
			theArg.SetArg(M4_HTTP_PROP_COOKIE,(m4pchar_t)m_strPublicCookie.c_str());
			m_pMainSession->SetProps(&theArg);
		}
	}			

	if (M4_SUCCESS!=iRetSwitch)
	{
		m_Parser.ResetParseAndTokenResults();
		if(ao_pcBuffer)
		{
			SETCODEF(M4_ERR_CONNECTION_ERROR,ERRORLOG,"Servlet error:------------------------------\n%s\n------------------------",ao_pcBuffer);
			cerr<<"Servlet error:\n------------------------------\n"<< ao_pcBuffer<<"\n------------------------\n";
		}
		return iRetSwitch;
	}

	if(m_Parser.GetTokenResult("M4Key",pzAuxCad,0)!=M4_SUCCESS)
	{
		m4Trace(cerr<<"Error getting M4Key result\n");
		iRetSwitch=M4_ERROR_HTTP_NO_M4KEY;
	}
	else
	{
		m4int_t iBlank=0;
		while(pzAuxCad[iBlank]==' ')
			iBlank++;	
		m_strPrivCookie=&pzAuxCad[iBlank];
		m4TraceLevel(2,cout<<"Now Using PrivCookie="<<m_strPrivCookie<<"("<<&pzAuxCad[iBlank]<<")"<<endl);
#ifdef PDUSTATISTIC
		writeM4Key(m_strPrivCookie.c_str());
#endif // PDUSTATISTIC
	}

	if (M4_SUCCESS!=iRetSwitch)
	{
		m_Parser.ResetParseAndTokenResults();
		if(ao_pcBuffer)
		{
			SETCODEF(M4_ERR_CONNECTION_ERROR,ERRORLOG,"Servlet error:------------------------------\n%s\n------------------------",ao_pcBuffer);
			cerr<<"Servlet error:\n------------------------------\n"<< ao_pcBuffer<<"\n------------------------\n";
		}
		return iRetSwitch;
	}

	if(m_Parser.GetTokenResult("inactivityTimeout",pzAuxCad,0)!=M4_SUCCESS)
	{m4TraceLevel(2,cerr<<"Error getting inactivityTimeoutresult\n");}
	else
	{m4TraceLevel(2,cout<<"Inactivity Timeout "<<pzAuxCad<<endl);}

	
	// CUADRAR TAMAÑOS

	m4pchar_t pcEndHeaders=(m4pchar_t)m_Parser.GetEndOfHeadersAddress();

	ao_uiBufferSize=uiTotalReaded-(pcEndHeaders-ao_pcBuffer);
	if(ao_uiBufferSize)
		ao_pcBuffer=pcEndHeaders;
	else
		ao_pcBuffer=NULL;
	// Si se ha leido algo de la pdu se deja en el buffer apuntado al comienzo de la PDU,
	// y pongo en ao_uiBufferSize el pedazo de PDU que he leido
	// Si no lo pongo NULL 

	m_Parser.ResetParseAndTokenResults();

	return M4_SUCCESS;
  //## end ClCCConnectionHTTP::ProccessHeaders%921676535.body
}

m4return_t ClCCConnectionHTTP::CreateHeaders (m4pchar_t ao_pcBuffer, m4uint32_t &ao_iBufferSize, m4uint32_t ai_uiPDUSize, m4uint32_t ai_uiTimeout, m4handle_t ai_hRID)
{
  //## begin ClCCConnectionHTTP::CreateHeaders%921676539.body preserve=yes
	// Por ahora usaremos un timeout infinito en el servlet
	ai_uiTimeout=0;

	m4return_t iRet;
	switch (GetConnStatus())
	{
	case M4_CONNECTION_STARTING :
		iRet=BuildConnectRequest(m_strServer.c_str(),
									m_iServerPort,
									m_strServlet.c_str(),
									ai_uiPDUSize,
									ai_uiTimeout, //ai_iTimeout 
									m_bSSL, //ai_bUseSSL 
									ao_pcBuffer,
									ao_iBufferSize);
		break;
	case M4_CONNECTION_RECONNECTING :
		iRet=BuildReConnectRequest(m_strCurrentHost.c_str(),
									m_iCurrentHostPort,
									m_strServlet.c_str(),
									ai_uiPDUSize,
									m_strPrivCookie.c_str(),
									m_strPublicCookie.c_str(),
									ai_uiTimeout,//ai_iTimeout
									m_bSSL, //ai_bUseSSL 
									ao_pcBuffer,
									ao_iBufferSize);
		break;
	case M4_CONNECTION_RUNNING :
		iRet=BuildSendDataRequest(m_strServlet.c_str(),
									ai_uiPDUSize,
									m_strPrivCookie.c_str(),
									m_strPublicCookie.c_str(),
									ai_uiTimeout,//ai_iTimeout
									ao_pcBuffer,
									ao_iBufferSize);
		break;
	case M4_CONNECTION_CLOSING:
		iRet=BuildDisconnectRequest(m_strServlet.c_str(),
									ai_uiPDUSize,
									m_strPrivCookie.c_str(),
									m_strPublicCookie.c_str(),
									ai_uiTimeout, // ai_iTimeout
									ao_pcBuffer,
									ao_iBufferSize);
		break;
	case M4_CONNECTION_FINISHING:
		m4Trace(cerr<< "No debia de llegar aqui"<<endl);
		break;
	default:
		m4Trace(cerr<<"Error de codificacion de estados"<<endl);
		iRet=M4_ERROR;
	}
	
	return iRet;
  //## end ClCCConnectionHTTP::CreateHeaders%921676539.body
}

m4return_t ClCCConnectionHTTP::GetNewTSAP (ClTSAP *&ao_pTSAP, m4uint32_t ai_uiPDUSize, m4uint32_t ai_uiTimeout, m4handle_t ai_hRID)
{
  //## begin ClCCConnectionHTTP::GetNewTSAP%922184352.body preserve=yes
	m4return_t iRet;
	while(m_eTSAPStatus==M4_CONNECTION_CLOSING_TSAP)
	{
		m4sleep(1);
	}
	if((!m_poTSAP) && (m_eTSAPStatus==M4_CONNECTION_NOTSAP))
	{
		iRet=CreateTSAP();
		if(M4_SUCCESS!=iRet)
		{
			m4Trace(cerr<<"Error creando TSAP en ConnectionHTTP"<<endl);
			return M4_ERROR;
		}
	}

	ao_pTSAP=m_poTSAP;

	m4uint32_t buffersize=MAX_POST_REQUEST_SIZE;
	m4char_t thebuffer[MAX_POST_REQUEST_SIZE];
	iRet=CreateHeaders(thebuffer,buffersize,ai_uiPDUSize,ai_uiTimeout);
	if(M4_SUCCESS!=iRet)
	{
		m4Trace(cerr<<"Error creando Headers en ConnectionHTTP"<<endl);
		return M4_ERROR;
	}

	iRet=SendHeaders(thebuffer,buffersize);
	if(M4_SUCCESS!=iRet)
	{
		m4Trace(cerr<<"Error sending Headers en ConnectionHTTP"<<endl);
		return M4_ERROR;
	}

	return M4_SUCCESS;
  //## end ClCCConnectionHTTP::GetNewTSAP%922184352.body
}

m4return_t ClCCConnectionHTTP::SendHeaders (m4pchar_t ai_pcBuffer, m4uint32_t ai_uiHeaderSize)
{
  //## begin ClCCConnectionHTTP::SendHeaders%922184353.body preserve=yes
	
	m4uint32_t iSent=0;
	m4uint32_t iToSend=ai_uiHeaderSize;
	m4TraceLevel(2,cout<<"Header (size="<<iToSend<<")::\n_________________\n"<<ai_pcBuffer<<"\n_________________\n"<<endl);
	if(NULL==m_poTSAP)
	{
		m4Trace(cerr << "NULL TSAP sending HTTP headers"<<endl);
		return M4_ERROR;
	}

	iSent=m_poTSAP->Write(ai_pcBuffer,iToSend);
	if(iSent!=iToSend)
	{
		SETCODE(M4_ERR_COMUNIC_ERROR_IN_SENDER,ERRORLOG,"Comunnication error sender http headers");
		m4Trace(cerr<<"Error sending headers:"<<(m4int32_t)iSent<<" bytes sent.It sould be "<<iToSend<<endl);
		return M4_ERROR;
	}
	return M4_SUCCESS;
  //## end ClCCConnectionHTTP::SendHeaders%922184353.body
}

m4char_t ClCCConnectionHTTP::GetConnType ()
{
  //## begin ClCCConnectionHTTP::GetConnType%923902843.body preserve=yes
	return (m4char_t)M4_TRUE;
  //## end ClCCConnectionHTTP::GetConnType%923902843.body
}

m4return_t ClCCConnectionHTTP::_GetAnswerPDU (ClPDU *&ao_pPDU, m4bool_t &ao_bReSend, m4int32_t ai_iTimeout, m4bool_t ai_bRetrying, m4pchar_t ai_pcBuffer, m4uint_t ai_uiBufferSize)
{
  //## begin ClCCConnectionHTTP::_GetAnswerPDU%951384406.body preserve=yes
	m4return_t iRet,iRet2;

	if(GetConnStatus()==M4_CONNECTION_CANCELING)
	{
		iRet=DestroyTSAP();
		SetTSAPStatus(M4_CONNECTION_NOTSAP);
		SetConnStatus(M4_CONNECTION_NOTSAP);
		iRet=M4_CANCELING_HTTP_REQUEST;
	}
	else
	{
		m4uint32_t uibuffersize=MAX_POST_REQUEST_SIZE;
		m4char_t thebuffer[MAX_POST_REQUEST_SIZE];
		memset(thebuffer,0,MAX_POST_REQUEST_SIZE);
		m4pchar_t  pcBuffer=thebuffer; // Vamos a cambiarlo y no quiero perder el bueno
		iRet=ProccessHeaders(pcBuffer,uibuffersize);
		if(M4_SUCCESS!=iRet)
			return iRet;



		iRet=ClCCConnection::_GetAnswerPDU (ao_pPDU,ao_bReSend,ai_iTimeout,ai_bRetrying,pcBuffer,uibuffersize);
		m_pClientMonitor->RemoveConnection((m4handle_t)this);
		iRet2=DestroyTSAP();
	}

	return  iRet;
  //## end ClCCConnectionHTTP::_GetAnswerPDU%951384406.body
}

m4return_t ClCCConnectionHTTP::_SendRawData (ClCCRequest *ai_pRequest)
{
  //## begin ClCCConnectionHTTP::_SendRawData%951410088.body preserve=yes
		ClTSAP * pTSAP;
	m4return_t  iRet;
		// Ahora serializo la PDU y la mando al servidor.
	ClPDU*  pPDU  = ai_pRequest -> GetInputPDU();
	
	iRet=GetNewTSAP(pTSAP,pPDU->GetPDUSize(),ai_pRequest->m_uiTimeout,pPDU->GetRequestId());
	if(M4_ERROR==iRet)
		return M4_ERROR;
	return ClCCConnection::_SendRawData (ai_pRequest);
  //## end ClCCConnectionHTTP::_SendRawData%951410088.body
}

// Additional Declarations
  //## begin ClCCConnectionHTTP%36EFA41302E1.declarations preserve=yes
  //## end ClCCConnectionHTTP%36EFA41302E1.declarations

//## begin module%42931E110136.epilog preserve=yes
//## end module%42931E110136.epilog
