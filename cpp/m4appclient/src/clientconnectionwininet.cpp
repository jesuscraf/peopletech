//## begin module%42931ED80052.cm preserve=no
//## end module%42931ED80052.cm

//## begin module%42931ED80052.cp preserve=no
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
//## end module%42931ED80052.cp

//## Module: clientconnectionwininet%42931ED80052; Package body
//## Subsystem: M4AppClient::src%3796F8B303AD
//## Source file: C:\m4server\m4appclient\src\clientconnectionwininet.cpp

//## begin module%42931ED80052.additionalIncludes preserve=no
//## end module%42931ED80052.additionalIncludes

//## begin module%42931ED80052.includes preserve=yes
#include <clientapi.hpp>
//## end module%42931ED80052.includes

// clientconnectionwininet
#include <clientconnectionwininet.hpp>
// clientconnection
#include <clientconnection.hpp>
// m4types
#include <m4types.hpp>
// m4condition
#include <m4condition.hpp>
//## begin module%42931ED80052.declarations preserve=no
//## end module%42931ED80052.declarations

//## begin module%42931ED80052.additionalDeclarations preserve=yes
#define M4GATEWAY_ERROR_SOCKET_TIMEOUT "SocketTimeoutException"
const m4int32_t BUFFSIZE = 262144;
//## end module%42931ED80052.additionalDeclarations

// Class ClWInetHandlesCloser
ClWInetHandlesCloser::ClWInetHandlesCloser(HINTERNET ai_hSession, HINTERNET ai_hConnect, HINTERNET ai_hRequest)
{
	m_hSession = ai_hSession;
	m_hConnect = ai_hConnect;
	m_hRequest = ai_hRequest;
}

void ClWInetHandlesCloser::Run(void)
{
	string sErrorMsg;
	m4int32_t iErrorCode;

	if (InternetSetStatusCallback(m_hRequest, NULL) == INTERNET_INVALID_STATUS_CALLBACK)
	{
		GetErrorMessage(iErrorCode, sErrorMsg);
		m4Trace(cerr << "\nFailed to unset internet callback function %s\n." << sErrorMsg.c_str() << endl);
		SETCODEF(M4_ERR_INTERNET_SET_STATUS_CALLBACK, ERRORLOG, "#*s1*#%s#", sErrorMsg.c_str());
	}

	if (m_hRequest != NULL)
	{	
		if(InternetCloseHandle(m_hRequest) == M4_FALSE)
		{
			GetErrorMessage(iErrorCode, sErrorMsg);
			m4Trace(cerr << "\nFailed to close request handle %s\n." << sErrorMsg.c_str() << endl);
			SETCODEF(M4_ERR_INTERNET_CLOSE_HANDLE, ERRORLOG, "#*s1*#%s#", sErrorMsg.c_str());
		}
	}

	if (m_hConnect != NULL)
	{
		if(InternetCloseHandle(m_hConnect) == M4_FALSE)
		{
			GetErrorMessage(iErrorCode, sErrorMsg);
			m4Trace(cerr << "\nFailed to close connect handle %s\n." << sErrorMsg.c_str() << endl);
			SETCODEF(M4_ERR_INTERNET_CLOSE_HANDLE, ERRORLOG, "#*s1*#%s#", sErrorMsg.c_str());
		}
	}

	if (m_hSession != NULL)
	{
		if(InternetCloseHandle(m_hSession) == M4_FALSE)
		{
			GetErrorMessage(iErrorCode, sErrorMsg);
			m4Trace(cerr << "\nFailed to close session handle %s\n." << sErrorMsg.c_str() << endl);
			SETCODEF(M4_ERR_INTERNET_CLOSE_HANDLE, ERRORLOG, "#*s1*#%s#", sErrorMsg.c_str());
		}
	}
}
// Class ClWInetHandlesCloser


// Class ClWinInetConnection
ClWinInetConnection::ClWinInetConnection (M4ClString &ai_sHost, m4int32_t ai_iHostPort, const M4ClString &ai_sServerName, m4int32_t ai_iServerPort, const string &ai_sServletName, m4bool_t ai_bSSL, M4ClString &ai_strCACertPath)
  //## begin ClWinInetConnection::ClWinInetConnection%1111399686.hasinit preserve=no
  //## end ClWinInetConnection::ClWinInetConnection%1111399686.hasinit
  //## begin ClWinInetConnection::ClWinInetConnection%1111399686.initialization preserve=yes
    :  ClCCConnection(ai_sHost, ai_iHostPort, ai_bSSL, ai_strCACertPath)
  //## end ClWinInetConnection::ClWinInetConnection%1111399686.initialization
{
  //## begin ClWinInetConnection::ClWinInetConnection%1111399686.body preserve=yes
	string sErrorMsg;
	m4int32_t iErrorCode;

	// Save SSL parameters.
	m_bSSL = ai_bSSL;
	m_sCACertPath = ai_strCACertPath;

	// Initialize an application's use of the WinINet functions.

	// Bug 112310: Read the proxy option from the registry (INTERNET_OPEN_TYPE_PRECONFIG)
	
	// Pasamos INTERNET_FLAG_ASYNC que provoca que la funcion de callback se ejecute 
	// en el working thread de la WinInet.
	m_hSession = InternetOpen("M4HttpSendRequestEx", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_ASYNC);

	if (!m_hSession)
	{
		GetErrorMessage(iErrorCode, sErrorMsg);
		m4Trace(cerr << "\nFailed calling InternetOpen %s\n." << sErrorMsg.c_str() << endl);
		SETCODEF(M4_ERR_INTERNET_OPEN, ERRORLOG, "#*s1*#%s#", sErrorMsg.c_str());
	}

	// Open an HTTP session for a given site.
	m_hConnect = InternetConnect(m_hSession, ai_sHost.c_str(), ai_iHostPort, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);
	if (!m_hConnect)
	{
		GetErrorMessage(iErrorCode, sErrorMsg);
		m4Trace(cerr << "\nFailed calling InternetConnect %s\n." << sErrorMsg.c_str() << endl);
		SETCODEF(M4_ERR_INTERNET_CONNECT, ERRORLOG, "#*s1*#%s#%d#%s#", ai_sHost.c_str(), ai_iHostPort, sErrorMsg.c_str());
	}

	// Save servlet name.
	m_sServletName = ai_sServletName;

	// Build URL with params (server app name, server app port, SSL).
	string sURL;
	if (BuildURL(ai_sServerName, ai_iServerPort, ai_bSSL, sURL) != M4_SUCCESS)
	{
		m4Trace(cerr << "\nError calling BuildURL." << endl);
		SETCODEF(M4_ERR_BUILD_URL, ERRORLOG, "#*s1*#%s#%d#%d#", ai_sServerName.c_str(), ai_iServerPort, ai_bSSL);
	}

	m_sURL = sURL;

	// bg 117797
	m_strAuthServer = ai_sServerName ;


	m_sHost = ai_sHost;
	m_iHostPort = ai_iHostPort;

	// Objetos para sincronización en llamadas asincronas a WinInet.
	m_pReqCondition = new ClCondition();	
	m_pReqCondition->Init();

	m_pCloseCondition = new ClCondition();
	m_pCloseCondition->Init();

	m_hRequest = (void*)NULL;

	m_pWInetHds = NULL;

	m_bIgnoreSSLErrors = M4_FALSE;

  //## end ClWinInetConnection::ClWinInetConnection%1111399686.body
}


ClWinInetConnection::~ClWinInetConnection ()
{
  //## begin ClWinInetConnection::~ClWinInetConnection%1111399687.body preserve=yes
	// Close all handles.
	string sErrorMsg;
	m4int32_t iErrorCode;
	
	if (m_hRequest != NULL)
	{	
		if (InternetCloseHandle(m_hRequest) == M4_FALSE)
		{
			GetErrorMessage(iErrorCode, sErrorMsg);
			m4Trace(cerr << "\nFailed to close request handle %s\n." << sErrorMsg.c_str() << endl);
			SETCODEF(M4_ERR_INTERNET_CLOSE_HANDLE, ERRORLOG, "#*s1*#%s#", sErrorMsg.c_str());
		}

		// Esperamos a que se termine de liberar el handle, no vayan a liberarse los semaforos
		// antes de las llamadas a la funcion de callback.
		m_pCloseCondition->Wait( -1 );
		m_hRequest = (void*)NULL;
	}
	
	if (m_hConnect != NULL)
	{
		if (InternetCloseHandle(m_hConnect) == M4_FALSE)
		{
			GetErrorMessage(iErrorCode, sErrorMsg);
			m4Trace(cerr << "\nFailed to close connect handle %s\n." << sErrorMsg.c_str() << endl);
			SETCODEF(M4_ERR_INTERNET_CLOSE_HANDLE, ERRORLOG, "#*s1*#%s#", sErrorMsg.c_str());
		}
	}

	if (m_hSession != NULL)
	{
		if (InternetCloseHandle(m_hSession) == M4_FALSE)
		{
			GetErrorMessage(iErrorCode, sErrorMsg);
			m4Trace(cerr << "\nFailed to close session handle %s\n." << sErrorMsg.c_str() << endl);
			SETCODEF(M4_ERR_INTERNET_CLOSE_HANDLE, ERRORLOG, "#*s1*#%s#", sErrorMsg.c_str());
		}
	}
	
	if( m_pReqCondition != NULL )
	{
		delete m_pReqCondition;
		m_pReqCondition = NULL;
	}

	if( m_pCloseCondition != NULL )
	{
		delete m_pCloseCondition;
		m_pCloseCondition = NULL;
	}

	if(m_pWInetHds != NULL)
	{
		delete m_pWInetHds;
	}
	//## end ClWinInetConnection::~ClWinInetConnection%1111399687.body
}



//## Other Operations (implementation)
m4return_t ClWinInetConnection::_GetAnswerData (ClCCRequest *ai_pRequest, m4bool_t &ao_bReSend)
{
  //## begin ClWinInetConnection::_GetAnswerData%1111399711.body preserve=yes
	ClPDU * poPDU = NULL;
	m4return_t iRet = M4_SUCCESS;
	m4bool_t bRetry = M4_TRUE;
	string sErrorMsg;
	m4int32_t iErrorCode;

	m4Trace(cout << "\rReading PDU     "<<flush);

	while (bRetry)
	{
		iRet = _GetAnswerPDU(poPDU, ao_bReSend, ai_pRequest->m_uiTimeout, ai_pRequest->m_bRetrying);

		// bg 0136877
		if(ao_bReSend == M4_TRUE)
		{
			if (InternetCloseHandle(m_hRequest) == M4_FALSE)
			{				
				GetErrorMessage(iErrorCode, sErrorMsg);
				m4Trace(cerr << "\nFailed to close request handle %s\n." << sErrorMsg.c_str() << endl);
				SETCODEF(M4_ERR_INTERNET_CLOSE_HANDLE, ERRORLOG, "#*s1*#%s#", sErrorMsg.c_str());
				return M4_ERROR;
			}
			
			m_pCloseCondition->Wait( -1 );
			m_hRequest = (void*)NULL;

			return M4_SUCCESS;
		}

        // bugid: 0109718
        if (NULL == poPDU && M4_SUCCESS == iRet)
        {
            iRet = M4_ERROR;
        }
		
		// Se ha superado el timeout pero el handle puede ser necesario si el usuario pulsa Retry.
		if(iRet != M4_WARNING)
		{
			if (InternetCloseHandle(m_hRequest) == M4_FALSE)
			{				
				GetErrorMessage(iErrorCode, sErrorMsg);
				m4Trace(cerr << "\nFailed to close request handle %s\n." << sErrorMsg.c_str() << endl);
				SETCODEF(M4_ERR_INTERNET_CLOSE_HANDLE, ERRORLOG, "#*s1*#%s#", sErrorMsg.c_str());
				return M4_ERROR;
			}
			
			// La llamada es asincrona, asi que esperamos a que termine.
			m_pCloseCondition->Wait( -1 );
			m_hRequest = (void*)NULL;
		}
		

		switch (iRet)
		{
		case M4_SUCCESS:
		{
			if (poPDU->GetVersion() == 0)
			{
				m_oProtocol.SetVersion(0);
			}

			m4int_t	requestIdfromPDU = poPDU->GetRequestId();
			m4int_t	requestIdfromRequest = ai_pRequest->GetRequestId();
			if (requestIdfromPDU == 0)
			{
				// Por aqui no va a pasar nunca, ya se ha procesado en ProccessPDU
				GetEvent(poPDU);
				return M4_SUCCESS;
			}
			else
			{
				if (requestIdfromPDU != requestIdfromRequest)
				{
					m4Trace(cerr << "\nPOP RequestIDfromPDU=" << requestIdfromPDU);
					m4Trace(cerr << " RequestIDfromRequest=" << requestIdfromRequest);
					m4Trace(cerr << " RequestIDfromInputPDU=" << ai_pRequest->GetInputPDU()->GetRequestId () << endl);
					iRet = M4_ERROR;
					continue;
				}
				else
				{
					bRetry=M4_FALSE;
				}

				m4uint16_t pduType = poPDU->GetTypePDU();
				m4uint16_t pduId = poPDU->GetIdPDU();
				ai_pRequest->SetPDUOutput(poPDU);
						
				switch(pduId)
				{
				case M4_ID_PDU_ACK:
					m4Trace(cout << "\rReaded  OK !                 \n" << flush);
					ai_pRequest->SetState(REPLY_ARRIVED);
					break;
				case M4_ID_PDU_ERROR:
				case M4_ID_PDU_NEWERROR:
					m4Trace(cout << "\rReaded   Error!                 \n" << flush);
					ai_pRequest->SetState(ERROR_ARRIVED);
					break;
				default:
					if (M4_TYPE_PDU_CONTROL != pduType)
					{
						ai_pRequest->SetState(REPLY_ARRIVED);
						m4Trace(cout << "\r Readed  OK !                 \n" << flush);
						m4uint32_t uiCompressionType = GetCompressionType();

                        iRet = _DecompressData(poPDU, uiCompressionType);
						if(iRet == M4_ERROR)
						{
							m4Trace(cerr << "Error decompressing Data" << endl);
							ai_pRequest->SetState(BROKEN_CONNECTION);
							bRetry = M4_FALSE;
						}
					}
					else
					{
						m4Trace(cerr << "\rERROR invalid PDU: Id:" << pduId <<endl);
						ai_pRequest->SetState(BROKEN_CONNECTION);
						bRetry = M4_FALSE;
					}
					break;
				}
			}
		}
		break;
		case M4_ERROR:
			ai_pRequest->SetState(BROKEN_CONNECTION);
			bRetry = M4_FALSE;
		break;
		case M4_WARNING:
			bRetry = M4_FALSE;
			break;
		default:
			bRetry = M4_FALSE;
			break;
		}
	}

	return iRet;
  //## end ClWinInetConnection::_GetAnswerData%1111399711.body
}

m4return_t ClWinInetConnection::_SendRawData (ClCCRequest *ai_pRequest)
{
  //## begin ClWinInetConnection::_SendRawData%1111399713.body preserve=yes
	INTERNET_BUFFERS BufferIn;
	DWORD dwBytesWritten = 0;
	string sErrorMsg;
	m4uint_t uBytesRead = 0;
	m4pchar_t pcBlock = NULL;
    m4int32_t iErrorCode = 0;
	m4int16_t iSynchRet = M4_COND_SIGNALED;
	DWORD dwLError = 0; 

	// Get timeout from request.
	m4int32_t iTimeout = ai_pRequest->m_uiTimeout;
	
	// Bg 0139158
	// m4int32_t iIETimeout = 3600000; // IE6 default value: 60 minutes
	
	// bg 194447
	m4uint32_t iNoTimeout = 0xFFFFFFFF;
	
	// Bg 123545. Comentamos las opciones de timeout, ahora los implementamos nosotros.
	/* --

	// Bug 108391. Set timeouts.
	// Set time-out value to use for Internet connection requests. 
	
	if (InternetSetOption(m_hConnect, INTERNET_OPTION_CONNECT_TIMEOUT, &iTimeout, sizeof(iTimeout)) != M4_TRUE)
	{
		GetErrorMessage(iErrorCode, sErrorMsg);
		m4Trace(cerr << "\nFailed to set INTERNET_OPTION_CONNECT_TIMEOUT internet option %s\n." << sErrorMsg.c_str() << endl);
		SETCODEF(M4_ERR_INTERNET_SET_OPTION, ERRORLOG, "#*s1*#%s#%s#", "INTERNET_OPTION_CONNECT_TIMEOUT" , sErrorMsg.c_str());
		return M4_ERROR;
	}

	// Set time-out value to receive a response to a request.
	if (InternetSetOption(m_hConnect, INTERNET_OPTION_SEND_TIMEOUT, &iTimeout, sizeof(iTimeout)) != M4_TRUE)
	{
		GetErrorMessage(iErrorCode, sErrorMsg);
		m4Trace(cerr << "\nFailed to set INTERNET_OPTION_SEND_TIMEOUT internet option %s\n." << sErrorMsg.c_str() << endl);
		SETCODEF(M4_ERR_INTERNET_SET_OPTION, ERRORLOG, "#*s1*#%s#%s#", "INTERNET_OPTION_SEND_TIMEOUT" , sErrorMsg.c_str());
		return M4_ERROR;
	}

	-- */

	if (m_hSession == NULL || m_hConnect == NULL)
	{
		// Bug 0142416: No reanuda comunicación CS tras caida de Web server.
		// Creamos una nueva sesion/conexión para enviar la pdu de cancelación
		// ya que la anterior se cerró por un error
		m_hSession = InternetOpen("M4HttpSendRequestEx", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_ASYNC);

		if (!m_hSession)
		{
			GetErrorMessage(iErrorCode, sErrorMsg);
			m4Trace(cerr << "\nFailed calling InternetOpen %s\n." << sErrorMsg.c_str() << endl);
			SETCODEF(M4_ERR_INTERNET_OPEN, ERRORLOG, "#*s1*#%s#", sErrorMsg.c_str());
			return M4_ERROR;
		}
		
		m_hConnect = InternetConnect(m_hSession, m_sHost.c_str(), m_iHostPort, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);

		if (!m_hConnect)
		{
			GetErrorMessage(iErrorCode, sErrorMsg);
			m4Trace(cerr << "\nFailed calling InternetConnect %s\n." << sErrorMsg.c_str() << endl);
			SETCODEF(M4_ERR_INTERNET_CONNECT, ERRORLOG, "#*s1*#%s#%d#%s#", m_sHost.c_str(), m_iHostPort, sErrorMsg.c_str());
			return M4_ERROR;
		}
	}

	// Set time-out value to send a request.
	if (InternetSetOption(m_hConnect, INTERNET_OPTION_RECEIVE_TIMEOUT, &iNoTimeout, sizeof(iNoTimeout)) != M4_TRUE)
	{
		GetErrorMessage(iErrorCode, sErrorMsg);
		m4Trace(cerr << "\nFailed to set INTERNET_OPTION_RECEIVE_TIMEOUT internet option %s\n." << sErrorMsg.c_str() << endl);
		SETCODEF(M4_ERR_INTERNET_SET_OPTION, ERRORLOG, "#*s1*#%s#%s#", "INTERNET_OPTION_RECEIVE_TIMEOUT" , sErrorMsg.c_str());
		return M4_ERROR;
	}
	
	// Create an HTTP request handle.
	// HTTP_VERSION = TEXT("HTTP/1.0")

	// bg 122211.	
	DWORD dwFlags = INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_KEEP_CONNECTION ;	

	if (m_bSSL == M4_TRUE)
	{		
		dwFlags |= INTERNET_FLAG_SECURE ;		
	}

	DWORD dwBuffLen = sizeof(dwFlags) ;

	
	ClPDU* pPDU = ai_pRequest->GetInputPDU();

	// Solo se da cuando en la peticion anterior espiro el timeout.
	if( m_hRequest != NULL)	
	{			
		// No es Retry, por lo que ya no necesitamos la request anterior.
		if( pPDU->GetIdPDU() == M4_ID_PDU_NEWCANCEL )
		{
			// Bg 0125975
			if(m_pWInetHds != NULL)
			{
				delete m_pWInetHds;
			}
			
			// Cerramos todos los handles de la wininet en otro thread y desde este
			// enviamos la pdu de cancelación. Una vez cancelada, los handles podrán
			// cerrarse correctamente.
			m_pWInetHds = new ClWInetHandlesCloser(m_hSession, m_hConnect, m_hRequest);

			m_pWInetHds->Start("ClWInetHandlesCloser");

			m_hSession = (void*)NULL;
			m_hConnect = (void*)NULL;
			m_hRequest = (void*)NULL;

			// Creamos una nueva sesion/conexión para enviar la pdu de cancelación
			// ya que la actual la estamos cerrando.
			m_hSession = InternetOpen("M4HttpSendRequestEx", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_ASYNC);

			if (!m_hSession)
			{
				GetErrorMessage(iErrorCode, sErrorMsg);
				m4Trace(cerr << "\nFailed calling InternetOpen %s\n." << sErrorMsg.c_str() << endl);
				SETCODEF(M4_ERR_INTERNET_OPEN, ERRORLOG, "#*s1*#%s#", sErrorMsg.c_str());
				return M4_ERROR;
			}
			
			m_hConnect = InternetConnect(m_hSession, m_sHost.c_str(), m_iHostPort, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);

			if (!m_hConnect)
			{
				GetErrorMessage(iErrorCode, sErrorMsg);
				m4Trace(cerr << "\nFailed calling InternetConnect %s\n." << sErrorMsg.c_str() << endl);
				SETCODEF(M4_ERR_INTERNET_CONNECT, ERRORLOG, "#*s1*#%s#%d#%s#", m_sHost.c_str(), m_iHostPort, sErrorMsg.c_str());
				return M4_ERROR;
			}
		}
	}

	// Este metodo no es asincrono, pero necesitamos pasarle un contexto si queremos 
	// que se invoque a la función de callback de aqui en adelante.
	m_hRequest = HttpOpenRequest(m_hConnect, "POST", m_sURL.c_str(), HTTP_VERSION, NULL, NULL, dwFlags, (DWORD)this);

	if (!m_hRequest)
	{
		GetErrorMessage(iErrorCode, sErrorMsg);
		m4Trace(cerr << "\nFailed to open request handle %s\n." << sErrorMsg.c_str() << endl);
		SETCODEF(M4_ERR_HTTP_OPEN_REQUEST, ERRORLOG, "#*s1*#%s#%s#", m_sURL.c_str(), sErrorMsg.c_str());
		return M4_ERROR;
	}

	// Registramos la funcion de callback a nivel de handle de request.
	if (InternetSetStatusCallback(m_hRequest, InetCallbackFunc) == INTERNET_INVALID_STATUS_CALLBACK)
	{
		GetErrorMessage(iErrorCode, sErrorMsg);
		m4Trace(cerr << "\nFailed to set internet callback function %s\n." << sErrorMsg.c_str() << endl);
		SETCODEF(M4_ERR_INTERNET_SET_STATUS_CALLBACK, ERRORLOG, "#*s1*#%s#", sErrorMsg.c_str());
		return M4_ERROR;
	}

	
	m4int32_t iPDUSize = pPDU->GetPDUSize();

    // insert the PDU in the protocol to serialize it.
    m_protocol.Initialize();
    m_protocol.ReleaseContents();
    m_protocol.SetWritePDU(pPDU);

    // bugid: 0110109
    // checks if PDU has files.
	M4DataStorage* dataStorage = ((ClPDUWithDataForZDelta*)pPDU)->GetDataStorageWrapper();
    m4bool_t bHasFiles = dataStorage->HasFileUnits();

    // checks for zdelta compression.
    m4uint32_t iCompressionType = pPDU->GetCompressionType();
    m4uint32_t iSessionId = pPDU->GetObjectId();
    m4bool_t doZdelta = (M4_COMPRESSION_TYPE_ZDELTA == iCompressionType) && (0 != iSessionId) && (M4_FALSE == bHasFiles);

	BufferIn.dwStructSize = sizeof(INTERNET_BUFFERS); // Must be set or error will occur
    BufferIn.Next = NULL;
    BufferIn.lpcszHeader = NULL;
    BufferIn.dwHeadersLength = 0;
    BufferIn.dwHeadersTotal = 0;
    BufferIn.lpvBuffer = NULL;
    BufferIn.dwBufferLength = 0;
    BufferIn.dwOffsetLow = 0;
    BufferIn.dwOffsetHigh = 0;
    // This is the only member used other than dwStructSize. Set below.
	BufferIn.dwBufferTotal = 0;

    if (doZdelta)
    {
        // necesito serializar la PDU para saber el tamaño que voy a enviar.
        m_protocol.GetNextChunk(pcBlock, uBytesRead);
        BufferIn.dwBufferTotal = uBytesRead;
    }
    else
    {
	    BufferIn.dwBufferTotal = pPDU->GetPDUSize();
    }

	
	// bg 283279.
	// Ponemos la marca de ignorar errores SSL sólo si es necesario. Si la CA y el certificado 
	// son buenos no hay que poner los flags, ya que esto implica que la wininet cree una conexión  
	// nueva por cada request y el correspondiente SSL handshake.
	m4bool_t	bIgnoreSSLErrorsPrev;

	do
	{
		bIgnoreSSLErrorsPrev = m_bIgnoreSSLErrors;

		// bg 122211.
		if (m_bSSL == M4_TRUE && m_bIgnoreSSLErrors == M4_TRUE )
		{
			if(!InternetQueryOption( m_hRequest, INTERNET_OPTION_SECURITY_FLAGS, (LPVOID)&dwFlags, &dwBuffLen ))
			{
				GetErrorMessage(iErrorCode, sErrorMsg);
				m4Trace(cerr << "Error on InternetQueryOption: %s\n." << sErrorMsg.c_str() << endl);
				SETCODEF(M4_ERR_HTTP_SEND_REQUEST, ERRORLOG, "#*s1*#%s#", sErrorMsg.c_str());
				_EndRequest(iTimeout,M4_FALSE);
				return M4_ERROR;
			}

			dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | SECURITY_FLAG_IGNORE_CERT_DATE_INVALID ;
			dwBuffLen = sizeof( dwFlags ) ;

			if(!InternetSetOption( m_hRequest, INTERNET_OPTION_SECURITY_FLAGS, &dwFlags, dwBuffLen ))
			{
				GetErrorMessage(iErrorCode, sErrorMsg);
				m4Trace(cerr << "Error on InternetSetOption: %s\n." << sErrorMsg.c_str() << endl);
				SETCODEF(M4_ERR_HTTP_SEND_REQUEST, ERRORLOG, "#*s1*#%s#", sErrorMsg.c_str());
				_EndRequest(iTimeout,M4_FALSE);
				return M4_ERROR;
			}
		}

		// Send the specified request to the HTTP server.
		// Modo asincrono, enviamos el contexto para la función de callback.
		if(!HttpSendRequestEx(m_hRequest, &BufferIn, NULL, HSR_ASYNC | HSR_INITIATE | HSR_CHUNKED, (DWORD)this))
		{
			// Comprobamos si se trata de ejecucion asincrona pendiente.
			dwLError = GetLastError();

			if( dwLError == ERROR_IO_PENDING )
			{
				iSynchRet = m_pReqCondition->Wait( iTimeout );
			}

			if( dwLError != ERROR_IO_PENDING || iSynchRet != M4_COND_SIGNALED )
			{
				GetErrorMessage(iErrorCode, sErrorMsg);
				m4Trace(cerr << "Error on HttpSendRequestEx: %s\n." << sErrorMsg.c_str() << endl);
				SETCODEF(M4_ERR_HTTP_SEND_REQUEST, ERRORLOG, "#*s1*#%s#", sErrorMsg.c_str());
				_EndRequest(iTimeout,M4_FALSE);
				return M4_ERROR;
			}
		}
		
		// Si hubo algún error de CA o certificado, la función de callback activó el atributo.
	}
	while( bIgnoreSSLErrorsPrev == M4_FALSE && m_bIgnoreSSLErrors == M4_TRUE );

	
	
    if (doZdelta)
    {
		// Se ejecuta en el mismo thread que estamos por lo que no bloqueamos
		// Internamente llama a la funcion de callback con INTERNET_STATUS_SENDING_REQUEST,
		// INTERNET_STATUS_REQUEST_SENT.
		BOOL bRet = InternetWriteFile(m_hRequest, pcBlock, uBytesRead, &dwBytesWritten);
		if (!bRet)
		{
            GetErrorMessage(iErrorCode, sErrorMsg);

			// Bug 0127729. Bloqueamos en la función InternetWriteFile aunque esta se 
			// ejecuta en el mismo thread.
			if (iErrorCode == ERROR_IO_PENDING)
			{
				iSynchRet = m_pReqCondition->Wait(iTimeout);
			}
			
			if (iErrorCode != ERROR_IO_PENDING || iSynchRet != M4_COND_SIGNALED)
			{
				m4Trace(cerr << "\nError on InternetWriteFile: %s\n: " << sErrorMsg.c_str() << endl);
				SETCODEF(M4_ERR_INTERNET_WRITE_FILE, ERRORLOG, "#*s1*#%d#%s#", iPDUSize, sErrorMsg.c_str());
				_EndRequest(iTimeout,M4_FALSE);

				// bg 0150392
				if( m_bSSL == M4_TRUE )
				{
					SETCODEF( M4_ERR_SENDING_TO_GATEWAY_SSL, ERRORLOG, "#*s1*#%s#%d#", m_sHost.c_str(), m_iHostPort ) ;
				}
				else
				{
					SETCODEF( M4_ERR_SENDING_TO_GATEWAY_NO_SSL, ERRORLOG, "#*s1*#%s#%d#", m_sHost.c_str(), m_iHostPort ) ;
				}

				// Bug 0142416: No reanuda comunicación CS tras caida de Web server.
				// Se resetea totalmente la Session (en otro thread) para quedar un estado consistente 
				if (iErrorCode == ERROR_INTERNET_INCORRECT_HANDLE_STATE)
				{
					ClWInetHandlesCloser* pWInetHds = new ClWInetHandlesCloser(m_hSession, m_hConnect, m_hRequest);
					// Interesa llamarse en el propio Thread. Por ello no se llama al Start
					pWInetHds->Run();

					m_hSession = (void*)NULL;
					m_hConnect = (void*)NULL;
					m_hRequest = (void*)NULL;
				}

				return M4_ERROR;
			}
		}
    }
    else
    {        
	    // Serialize PDU.
	    for (;;)
	    {
		    // Serialize PDU.
		    BOOL bRet = TRUE;
		    m4uint_t uBytesRead = 0;
		    m4pchar_t pcBlock = NULL;
		    while ((m_protocol.GetNextChunk(pcBlock, uBytesRead)) == M4_SUCCESS) 
		    {
			    // Writes data for HTTP request.
				bRet = InternetWriteFile(m_hRequest, pcBlock, uBytesRead, &dwBytesWritten);
				if (!bRet)
				{
					GetErrorMessage(iErrorCode, sErrorMsg);

			
					// Bug 0127729. Bloqueamos en la función InternetWriteFile aunque esta se 
					// ejecuta en el mismo thread. Cuando tenemos PDUs con ficheros,
					// que suelen tener varios chunks, esta función da un ERROR_IO_PENDING.
					if (iErrorCode == ERROR_IO_PENDING)
					{
						iSynchRet = m_pReqCondition->Wait(iTimeout);
					}
					
					if (iErrorCode != ERROR_IO_PENDING || iSynchRet != M4_COND_SIGNALED)
					{
						m4Trace(cerr << "\nError on InternetWriteFile: %s\n: " << sErrorMsg.c_str() << endl);
						SETCODEF(M4_ERR_INTERNET_WRITE_FILE, ERRORLOG, "#*s1*#%d#%s#", iPDUSize, sErrorMsg.c_str());
						_EndRequest(iTimeout,M4_FALSE);

						// bg 0150392
						if( m_bSSL == M4_TRUE )
						{
							SETCODEF( M4_ERR_SENDING_TO_GATEWAY_SSL, ERRORLOG, "#*s1*#%s#%d#", m_sHost.c_str(), m_iHostPort ) ;
						}
						else
						{
							SETCODEF( M4_ERR_SENDING_TO_GATEWAY_NO_SSL, ERRORLOG, "#*s1*#%s#%d#", m_sHost.c_str(), m_iHostPort ) ;
						}

						// Bug 0142416: No reanuda comunicación CS tras caida de Web server.
						// Se resetea totalmente la Session (en otro thread) para quedar un estado consistente 
						if (iErrorCode == ERROR_INTERNET_INCORRECT_HANDLE_STATE)
						{
							ClWInetHandlesCloser* pWInetHds = new ClWInetHandlesCloser(m_hSession, m_hConnect, m_hRequest);
							// Interesa llamarse en el propio Thread. Por ello no se llama al Start
							pWInetHds->Run();

							m_hSession = (void*)NULL;
							m_hConnect = (void*)NULL;
							m_hRequest = (void*)NULL;
						}

						return M4_ERROR;
					}
				}
				
                pcBlock = NULL;
		    }
	    
		    break;
	    }
    }

	return M4_SUCCESS;
  //## end ClWinInetConnection::_SendRawData%1111399713.body
}

m4return_t ClWinInetConnection::_GetAnswerPDU (ClPDU *&ao_poPDU, m4bool_t &ao_bReSend, m4int32_t ai_iTimeout, m4bool_t ai_bRetrying, m4pchar_t ai_pcBuffer, m4uint_t ai_uiBufferSize)
{
  //## begin ClWinInetConnection::_GetAnswerPDU%1111399716.body preserve=yes
	// bugid: 0109718
	m4return_t iRet = M4_ERROR;

	DWORD dwBytesRead = 0;
	m4int32_t iHeadSize = 0;
	m4pchar_t pReadFileBuffer = NULL;
	m4uint32_t iPDUBodySize = 0;
	m4pchar_t pPDUBody = NULL;
	m4pchar_t pszRemainingBody = NULL;
	m4pchar_t pRemainingBody = NULL;
	m4int32_t iRemainingBody = 0;	
	string sErrorMsg;
    m4int32_t iErrorCode = 0;
    m4uint_t uiUsed = 0;
    m4uint_t uiNextBlockSize = 0;
    m4bool_t bInitializeProtocol = M4_TRUE;
	DWORD dwLError = NO_ERROR;	
	m4int16_t iSynchRet = M4_COND_TIMEOUT;	
	m4int32_t iStatusCode = -1;
	DWORD dSize = sizeof(iStatusCode);
	DWORD dIndex = 0;
	m4uint16_t iSkipPDUId = M4_ID_PDU_SKIP;

	
	ao_bReSend = M4_FALSE;

	// Cuando estamos haciendo retry simplemente hay que esperar.
	iRet = _EndRequest(ai_iTimeout,ai_bRetrying);

	if (iRet != M4_SUCCESS)
	{
		return iRet;
	}

	// bg 0136877
	if (!HttpQueryInfo(m_hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &iStatusCode, &dSize, &dIndex))
	{
		dwLError = GetLastError();
		
		if( dwLError == ERROR_IO_PENDING)
		{			
			iSynchRet = m_pReqCondition->Wait( ai_iTimeout );			
		}
		
		if( dwLError != ERROR_IO_PENDING || iSynchRet != M4_COND_SIGNALED )
		{
			GetErrorMessage(iErrorCode, sErrorMsg);
			m4Trace(cerr << "\nHttpQueryInfo failed for HTTP_QUERY_STATUS_CODE: %s\n: " << sErrorMsg.c_str() << endl);
			SETCODEF(M4_ERR_HTTP_QUERY_INFO, ERRORLOG, "#*s1*#%s#%s#", "HTTP_QUERY_STATUS_CODE", sErrorMsg.c_str());
			return M4_ERROR;
		}
	}

	if (iStatusCode == HTTP_STATUS_PROXY_AUTH_REQ)
	{
		// El proxy requiere autenticación por lo que avisamos hacia arriba que habrá que reenviar.
		ao_bReSend = M4_TRUE;

		// Forzamos la negociación de autenticación con el proxy enviando un pdu ficticia. 
		// Cuando se reenvie la pdu actual, ya estaremos autenticados en el proxy.
		if(!HttpSendRequest (m_hRequest, NULL, 0, &iSkipPDUId, sizeof(iSkipPDUId)))
		{
			dwLError = GetLastError();
		
			if( dwLError == ERROR_IO_PENDING)
			{			
				iSynchRet = m_pReqCondition->Wait( ai_iTimeout );			
			}
		
			if( dwLError != ERROR_IO_PENDING || iSynchRet != M4_COND_SIGNALED )
			{
				GetErrorMessage(iErrorCode, sErrorMsg);
				m4Trace(cerr << "Error on HttpSendRequest: %s\n." << sErrorMsg.c_str() << endl);
				SETCODEF(M4_ERR_HTTP_SEND_REQUEST, ERRORLOG, "#*s1*#%s#", sErrorMsg.c_str());
				_EndRequest(ai_iTimeout,M4_FALSE);
				return M4_ERROR;
			}
		}
		
		return M4_SUCCESS;
	}


	// Alloc read file buffer.
	pReadFileBuffer = new m4char_t[BUFFSIZE];
	if (pReadFileBuffer == NULL)
	{
		m4Trace(cerr << "NULL pReadFileBuffer." << endl);
		SETCODEF(M4_ERR_NO_MEMORY, ERRORLOG, "#*s1*#%d#", sizeof(m4char_t) * BUFFSIZE);
		return M4_ERROR;
	}


	DWORD dwNumberOfBytesAvailable = 0;
	if (!InternetQueryDataAvailable(m_hRequest, &dwNumberOfBytesAvailable, 0, 0))
	{
		dwLError = GetLastError();
		
		if( dwLError == ERROR_IO_PENDING)
		{			
			iSynchRet = m_pReqCondition->Wait( ai_iTimeout );			
		}
		
		if( dwLError != ERROR_IO_PENDING || iSynchRet != M4_COND_SIGNALED )
		{
			GetErrorMessage(iErrorCode, sErrorMsg);
			m4Trace(cerr << "\nInternetQueryDataAvailable failed: %s\n: " << sErrorMsg.c_str() << endl);
			SETCODEF(M4_ERR_INTERNET_QUERY_DATA_AVAILABLE, ERRORLOG, "#*s1*#%s#", sErrorMsg.c_str());
			delete [] pReadFileBuffer;
			return M4_ERROR;
		}
	}
	
	if (dwNumberOfBytesAvailable == 0)
	{
		m4Trace(cerr << "\ndwNumberOfBytesAvailable is zero." << endl);
		SETCODEF(M4_ERR_NO_INTERNET_DATA_AVAILABLE, ERRORLOG, "#*s1*#");
		delete [] pReadFileBuffer;
		return M4_ERROR;
	}
	
	// Bug 0110007.
	// Get status code and status error text from header HTTP response.
	m4int32_t iStatusErrorCode = 0;
	string sStatusErrorMessage;
	if (CheckHeaderInformation(iStatusErrorCode, sStatusErrorMessage) != M4_SUCCESS)
	{
		m4Trace(cerr << "\nCheckHeaderInformation failed." << endl);
		SETCODEF(M4_ERR_CHECK_HEADER_INFORMATION, ERRORLOG, "#*s1*#");
		delete [] pReadFileBuffer;
		return M4_ERROR;
	}

	// If we have an HTTP_STATUS_SERVER_ERROR error (500), try to recover
	// error text.
	if (iStatusErrorCode == HTTP_STATUS_SERVER_ERROR)
	{
		// Check error text. We are only interested in m4gateway errors, specifically
		// we need to know if we have a timeout.
		if (sStatusErrorMessage.compare(M4GATEWAY_ERROR_SOCKET_TIMEOUT) == 0)
		{
			// M4gateway timeout.
			iRet = M4_WARNING;
			SETCODEF(M4_ERR_M4GATEWAY_SOCKET_TIMEOUT, ERRORLOG, "#*s1*#%s#", sStatusErrorMessage.c_str());
		}
		else
		{
			// Other errors.
			iRet = M4_ERROR;
			SETCODEF(M4_ERR_HTTP_STATUS_SERVER_ERROR, ERRORLOG, "#*s1*#%s#", sStatusErrorMessage.c_str());
		}

		m4Trace(cerr << "\nError on HttpEndRequest: %s\n: " << sStatusErrorMessage.c_str() << endl);
		return iRet;
	}
	
	// Read data returned from HTTP server.
	ao_poPDU = NULL;
    m4uint32_t pduReadStatus = M4_COMM_PROTOCOL_RETURN_CONTINUE_READING;
	do
	{
		dwBytesRead = 0;
		iHeadSize = 0;

		// Se ejecuta en el mismo thread que estamos por lo que no bloqueamos
		// Internamente llama a la funcion de callback con INTERNET_STATUS_RECEIVING_RESPONSE,
		// INTERNET_STATUS_RESPONSE_RECEIVED, INTERNET_STATUS_CLOSING_CONNECTION, INTERNET_STATUS_CONNECTION_CLOSED
		if (InternetReadFile(m_hRequest, pReadFileBuffer + iRemainingBody, BUFFSIZE - 1, &dwBytesRead))
		{
			pReadFileBuffer[iRemainingBody + dwBytesRead] = 0x00; // Null-terminate buffer
			m4Trace(cout << pReadFileBuffer << endl);
		}
		else
		{
			dwLError = GetLastError();

			if( dwLError == ERROR_IO_PENDING)
			{			
				iSynchRet = m_pReqCondition->Wait( ai_iTimeout );
			}

			if( dwLError != ERROR_IO_PENDING || iSynchRet != M4_COND_SIGNALED )
			{
				GetErrorMessage(iErrorCode, sErrorMsg);
				m4Trace(cerr << "\nInternetReadFile failed: %s\n: "<< sErrorMsg.c_str() << endl);
				SETCODEF(M4_ERR_INTERNET_READ_FILE, ERRORLOG, "#*s1*#%s#", sErrorMsg.c_str());
				iRet = M4_ERROR;
				break;
			}
		}
		
		if (dwBytesRead <= 0)
        {
            break;
        }

		if (M4_TRUE == bInitializeProtocol)
        {
            bInitializeProtocol = M4_FALSE;
            m_protocol.Initialize();
            m_protocol.ReleaseContents();
        }

        pPDUBody = pReadFileBuffer;
        iPDUBodySize = iRemainingBody + dwBytesRead;
        uiUsed = 0;
        do
        {
            pduReadStatus = m_protocol.AddChunk(pPDUBody, iPDUBodySize, uiUsed, uiNextBlockSize);
            pPDUBody = pPDUBody + uiUsed;
            iPDUBodySize -= uiUsed;
        }
        while ((M4_COMM_PROTOCOL_RETURN_CONTINUE_READING == pduReadStatus)
            && (uiNextBlockSize <= iPDUBodySize));

        if (M4_COMM_PROTOCOL_RETURN_COMPLETED == pduReadStatus)
        {
            m_protocol.GetReadedPDU(ao_poPDU);
            iRet = M4_SUCCESS;
            break;
        }

		if (M4_COMM_PROTOCOL_RETURN_ERROR == pduReadStatus)
		{
			m4Trace(cerr << "\nError building PDU." << endl);
			SETCODEF(M4_ERR_BUILD_PDU, ERRORLOG, "#*s1*#");
			iRet = M4_ERROR;
            break;
		}

		iRemainingBody = iPDUBodySize;
        if (0 != iRemainingBody)
        {
			pRemainingBody = new m4char_t[iRemainingBody];
			if (pRemainingBody == NULL)
			{
				m4Trace(cerr << "NULL pRemainingBody." << endl);
				SETCODEF(M4_ERR_NO_MEMORY, ERRORLOG, "#*s1*#%d#", sizeof(m4char_t) * iRemainingBody);
				iRet = M4_ERROR;
                break;
			}

    		// Save remaining PDU body data.
    		memcpy(pRemainingBody, pPDUBody, iRemainingBody);
        }

		if (pReadFileBuffer != NULL)
		{
			delete[] pReadFileBuffer;
		}
		pReadFileBuffer = new m4char_t[iRemainingBody + BUFFSIZE];
		if (pReadFileBuffer == NULL)
		{
			m4Trace(cerr << "NULL pReadFileBuffer." << endl);
			SETCODEF(M4_ERR_NO_MEMORY, ERRORLOG, "#*s1*#%d#", sizeof(m4char_t) * (iRemainingBody + BUFFSIZE));
			iRet = M4_ERROR;
            break;
		}
		
		// Copy remaining PDU data to read file buffer.
        if (NULL != pRemainingBody)
        {
		    memcpy(pReadFileBuffer, pRemainingBody, iRemainingBody);
            delete[] pRemainingBody;
            pRemainingBody = NULL;
            // does not reset iRemainingBody. It's used in the next iteration.
        }
    }
	while (M4_COMM_PROTOCOL_RETURN_CONTINUE_READING == pduReadStatus);

	if (NULL != pRemainingBody) delete[] pRemainingBody;
	if (NULL != pReadFileBuffer) delete[] pReadFileBuffer;

	return iRet;
  //## end ClWinInetConnection::_GetAnswerPDU%1111399716.body
}

m4return_t ClWinInetConnection::Reconnect (m4pchar_t ai_pcServerIP, m4int_t ai_iServerPort)
{
  //## begin ClWinInetConnection::Reconnect%1115711669.body preserve=yes
	// Build new URL with params (server app name, server app port, SSL).
	string sURL;
	if ( m_bSSL )
	{
		ai_iServerPort ++ ;
	}
	if (BuildURL(ai_pcServerIP, ai_iServerPort, m_bSSL, sURL) != M4_SUCCESS)
	{
		m4Trace(cerr << "\nError calling BuildURL." << endl);
		SETCODEF(M4_ERR_BUILD_URL, ERRORLOG, "#*s1*#%s#%d#%d#", ai_pcServerIP, ai_iServerPort, m_bSSL);
		return M4_ERROR;
	}

	m_sURL = sURL;

	// bg 117797
	m_strAuthServer = ai_pcServerIP ;

	return M4_SUCCESS;
  //## end ClWinInetConnection::Reconnect%1115711669.body
}

m4return_t ClWinInetConnection::BuildPDU (ClPDU *&ao_poPDU, m4pchar_t &aio_pcBuffer, m4uint32_t &aio_uiBufferSize)
{
  //## begin ClWinInetConnection::BuildPDU%1112692646.body preserve=yes
	m4TraceLevel(2, cout << "Me ha llegado un buffer de " << aio_uiBufferSize << endl);

	m4pchar_t pszBuffer = NULL;
	m4int32_t iNextToRead = 0;
	m4return_t iRet = M4_SUCCESS;

	m4pchar_t pPDUData = aio_pcBuffer;
	m4uint32_t iPDUDataSize = aio_uiBufferSize;

	// Keep parsing input PDU data (body) to add new data blocks.
	while (1)
	{
		pszBuffer = NULL;
		iNextToRead = ao_poPDU->GetNextSizeBlock(pszBuffer);
		m4TraceLevel(2, cout << "ReadChunk:" << iNextToRead << endl);
		
		if (iNextToRead == 0)
		{
			break;
		}
		else if (iNextToRead < 0)
		{
			m4Trace(cerr << "Error creating PDU:SizeBlock=" << iNextToRead << endl);
			SETCODEF(M4_ERR_PDU_GET_NEXT_SIZE_BLOCK, ERRORLOG, "#*s1*#");
			return M4_ERROR;
		}
		
		if (!pszBuffer)
		{
			m4Trace(cerr << "NULL buffer creating pdu." << endl);
			SETCODEF(M4_ERR_PDU_GET_NEXT_SIZE_BLOCK, ERRORLOG, "#*s1*#");
			return M4_ERROR;
		}
		
		// Check if we still have enough data to add a new block.
		if (((m4uint32_t) iNextToRead) <= iPDUDataSize)
		{
			memcpy(pszBuffer, pPDUData, iNextToRead);
			iPDUDataSize -= iNextToRead;
			pPDUData += iNextToRead;
		}
		else
		{
			// We do not have enough data to set a new block. Returns the remaining data
			// to be able to set them the next time.
			aio_pcBuffer = aio_pcBuffer + aio_uiBufferSize - iPDUDataSize;
			aio_uiBufferSize = iPDUDataSize;
			return M4_SUCCESS;
		}

		iRet = ao_poPDU->AddBlock(pszBuffer);
		if (iRet != M4_SUCCESS)
		{
			SETCODEF(M4_ERR_PDU_ADD_BLOCK, ERRORLOG, "#*s1*#");
			return M4_ERROR;
		}
	}
	
	return M4_SUCCESS;
  //## end ClWinInetConnection::BuildPDU%1112692646.body
}

m4return_t GetErrorMessage (m4int32_t &aio_iErrorCode, string &aio_sErrorMsg)
{
  //## begin ClWinInetConnection::GetErrorMessage%1115711667.body preserve=yes
	DWORD dwError = GetLastError();
	aio_sErrorMsg = "";
	LPVOID lpMsgBuf = 0;
	if (dwError >= INTERNET_ERROR_BASE && dwError <= INTERNET_ERROR_LAST)
    {
		if (dwError == ERROR_INTERNET_EXTENDED_ERROR)
		{
			DWORD dwInetError;
			DWORD dwExtLength;
			if (! InternetGetLastResponseInfo(&dwInetError, 0, &dwExtLength)
				&& GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				dwExtLength += 1;
				lpMsgBuf = LocalAlloc(LMEM_FIXED, dwExtLength);
				if (lpMsgBuf != 0)
				{
					if (! InternetGetLastResponseInfo(&dwInetError,
						reinterpret_cast<LPTSTR>(lpMsgBuf),
						&dwExtLength))
					{
						LocalFree (lpMsgBuf);
						lpMsgBuf = 0;
					}
				}
			}
		}
		else
		{
			FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER
				| FORMAT_MESSAGE_IGNORE_INSERTS
				| FORMAT_MESSAGE_FROM_HMODULE,
				GetModuleHandle("wininet.dll"),
				dwError,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				reinterpret_cast<LPTSTR>(&lpMsgBuf),
				0,
				0);
		}
    }
	else
    {
		FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER
			| FORMAT_MESSAGE_IGNORE_INSERTS
			| FORMAT_MESSAGE_FROM_SYSTEM,
			0,
			dwError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPTSTR>(&lpMsgBuf),
			0,
			0);
    }
	if (lpMsgBuf == 0)
    {
		aio_sErrorMsg = "Unexpected error condition.";
    }
	else
    {
		aio_sErrorMsg = reinterpret_cast<LPCTSTR>(lpMsgBuf);
		LocalFree (lpMsgBuf);
    }

	aio_iErrorCode = dwError;

	return M4_SUCCESS;
  //## end ClWinInetConnection::GetErrorMessage%1115711667.body
}

m4return_t ClWinInetConnection::BuildURL (const string &ai_sAppServerName, m4int32_t ai_iAppServerPort, m4bool_t ai_bUseSSL, string &aio_sURL)
{
  //## begin ClWinInetConnection::BuildURL%1115711668.body preserve=yes
	string sURL;
	m4char_t pszAux[8];

	sURL = m_sServletName;
	sURL.append("?server=");
	sURL.append(ai_sAppServerName);
	sURL.append("&port=");
	sprintf(pszAux, "%d", ai_iAppServerPort);
	sURL.append(pszAux);
	sURL.append("&useSSL=");
	sprintf(pszAux, "%d", ai_bUseSSL);
	sURL.append(pszAux);

	aio_sURL = sURL;

	return M4_SUCCESS;
  //## end ClWinInetConnection::BuildURL%1115711668.body
}

m4return_t ClWinInetConnection::CheckHeaderInformation (m4int32_t &aio_iStatusCode, string &aio_sStatusMessage)
{
  //## begin ClWinInetConnection::CheckHeaderInformation%1131442403.body preserve=yes
	// Get status code.
	string sErrorMsg;
    m4int32_t iErrorCode = 0;
	m4int32_t iStatusCode = -1;
	DWORD dSize = sizeof(iStatusCode);
	DWORD dIndex = 0;
	if (!HttpQueryInfo(m_hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &iStatusCode, &dSize, &dIndex))
	{
		GetErrorMessage(iErrorCode, sErrorMsg);
		m4Trace(cerr << "\nHttpQueryInfo failed for HTTP_QUERY_STATUS_CODE: %s\n: " << sErrorMsg.c_str() << endl);
		SETCODEF(M4_ERR_HTTP_QUERY_INFO, ERRORLOG, "#*s1*#%s#%s#", "HTTP_QUERY_STATUS_CODE", sErrorMsg.c_str());
		return M4_ERROR;
	}
	
	// If we have an HTTP_STATUS_SERVER_ERROR error (500) from m4gateway, try to recover
	// error text.
	if (iStatusCode != HTTP_STATUS_OK)
	{
		// Get status message.
		m4bool_t bQueryInfoError = M4_FALSE;
		m4uint32_t iErrorInfoSize = BUFFSIZE;
		m4pchar_t pcErrorInfo = new m4char_t[iErrorInfoSize];
		if (!HttpQueryInfo(m_hRequest, HTTP_QUERY_STATUS_TEXT, pcErrorInfo, &iErrorInfoSize, NULL))
		{
			GetErrorMessage(iErrorCode, sErrorMsg);
			if (iErrorCode == ERROR_INSUFFICIENT_BUFFER)
			{
				delete[] pcErrorInfo;
				pcErrorInfo = new m4char_t[iErrorInfoSize];
				if (HttpQueryInfo(m_hRequest, HTTP_QUERY_STATUS_TEXT, pcErrorInfo, &iErrorInfoSize, NULL))
				{
					aio_sStatusMessage.append(pcErrorInfo);
				}
				else
				{
					bQueryInfoError = M4_TRUE;
				}
			}
			else
			{
				bQueryInfoError = M4_TRUE;
			}
		}
		else
		{
			aio_sStatusMessage.append(pcErrorInfo);
		}

		// Release memory.
		delete[] pcErrorInfo;

		if (bQueryInfoError == M4_TRUE)
		{
			m4Trace(cerr << "\nHttpQueryInfo failed for HTTP_QUERY_STATUS_TEXT: %s\n: " << sErrorMsg.c_str() << endl);
			SETCODEF(M4_ERR_HTTP_QUERY_INFO, ERRORLOG, "#*s1*#%s#%s#", "HTTP_QUERY_STATUS_TEXT", sErrorMsg.c_str());
			return M4_ERROR;
		}
	}

	aio_iStatusCode = iStatusCode;
	
	return M4_SUCCESS;
  //## end ClWinInetConnection::CheckHeaderInformation%1131442403.body
}

m4return_t ClWinInetConnection::_EndRequest(m4int32_t ai_iTimeout, m4bool_t ai_bRetrying)
{
	m4return_t iRet = M4_ERROR;
	DWORD dwLError = NO_ERROR;	
	m4int16_t iSynchRet = M4_COND_TIMEOUT;	
	string sErrorMsg;
	m4int32_t iErrorCode;

	// Cuando estamos haciendo retry simplemente hay que esperar.
	if( !ai_bRetrying )
	{
		// End an HTTP request that was initiated by HttpSendRequestEx.
		if (!HttpEndRequest(m_hRequest, NULL, 0, HSR_ASYNC | HSR_INITIATE | HSR_CHUNKED))
		{
			dwLError = GetLastError();

			if(dwLError != ERROR_IO_PENDING)
			{
				GetErrorMessage(iErrorCode, sErrorMsg);
				m4Trace(cerr << "\nError on HttpEndRequest: %s\n: " << sErrorMsg.c_str() << endl);
				SETCODEF(M4_ERR_HTTP_END_REQUEST, ERRORLOG, "#*s1*#%s#", sErrorMsg.c_str());
			
				return M4_ERROR;
			}
		}
	}

	if(ai_bRetrying || dwLError == ERROR_IO_PENDING)
	{
		iSynchRet = m_pReqCondition->Wait( ai_iTimeout );

		if(iSynchRet == M4_COND_TIMEOUT)
		{
			return M4_WARNING;
		}
		else if(iSynchRet == M4_COND_ERROR)
		{
			m4Trace(cerr << "\nError on asynchronous HttpEndRequest: Condition error\n " << endl);

			return M4_ERROR;
		}
	}

	// si llegamos aquí estamos en el estado normal
	return M4_SUCCESS;
}

// Additional Declarations
  //## begin ClWinInetConnection%423E92D70341.declarations preserve=yes
// Bg 123545. Funcion de callback basica para las funciones asincronas de la wininet.
void CALLBACK InetCallbackFunc( HINTERNET hInternet, DWORD_PTR dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength )
{	
	ClCondition* pSynchObj = NULL;
	INTERNET_ASYNC_RESULT *pRes = ( INTERNET_ASYNC_RESULT* )lpvStatusInformation;


	// Trazas
	// m4char_t szTrace[1024];
	// int iThreadID = GetCurrentThreadId();
	// sprintf(szTrace, "Thread ID : %d, Status: %d.\n", iThreadID, dwInternetStatus);
	// OutputDebugString(szTrace);

	if(((ClWinInetConnection*)dwContext)->getRequestHandle() != hInternet)
	{
		return;
	}
	
	// Operación asíncrona finalizada.
	if( dwInternetStatus == INTERNET_STATUS_REQUEST_COMPLETE )
	{
		if( pRes->dwError == ERROR_INTERNET_INVALID_CA || pRes->dwError == ERROR_INTERNET_SEC_CERT_DATE_INVALID || pRes->dwError == ERROR_INTERNET_SEC_CERT_CN_INVALID )
		{
			( ( ClWinInetConnection* )dwContext )->setIgnoreSSLErrors( M4_TRUE );
		}

		pSynchObj = ((ClWinInetConnection*)dwContext)->m_pReqCondition;
		
		if( pSynchObj != NULL )
		{
			pSynchObj->Signal();
		}
	}
	// Cierre de handle finalizado.
	else if( dwInternetStatus == INTERNET_STATUS_HANDLE_CLOSING )
	{

		pSynchObj = ((ClWinInetConnection*)dwContext)->m_pCloseCondition;
		
		if( pSynchObj != NULL )
		{
			pSynchObj->Signal();
		}
	}
}
  //## end ClWinInetConnection%423E92D70341.declarations
//## begin module%42931ED80052.epilog preserve=yes
//## end module%42931ED80052.epilog
