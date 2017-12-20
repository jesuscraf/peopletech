//## begin module%38D88438001C.cm preserve=no
//## end module%38D88438001C.cm

//## begin module%38D88438001C.cp preserve=no
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
//## end module%38D88438001C.cp

//## Module: TransportLayerSynchronous%38D88438001C; Package body
//## Subsystem: m4transport::M4CTL::src%38D1FCC40071
//## Source file: c:\Componentes\m4transport\src\transportlayersynchronous.cpp

//## begin module%38D88438001C.additionalIncludes preserve=no
//## end module%38D88438001C.additionalIncludes

//## begin module%38D88438001C.includes preserve=yes
//## end module%38D88438001C.includes

// TransportLayerContainer
#include <transportlayercontainer.hpp>
// TransportLayerSynchronous
#include <transportlayersynchronous.hpp>
// TransportLayer
#include <transportlayer.hpp>
// tsap
#include <tsap.hpp>
//## begin module%38D88438001C.declarations preserve=no
//## end module%38D88438001C.declarations

//## begin module%38D88438001C.additionalDeclarations preserve=yes
#include "controlpipesmkr.hpp"
#include "m4trace.hpp"
#define	MAX_RETRIES	50

extern m4uint32_t g_iTSAPId ;
//## end module%38D88438001C.additionalDeclarations


// Class ClTransportLayerSynchronous 



ClTransportLayerSynchronous::ClTransportLayerSynchronous (int ai_iControlPort)
  //## begin ClTransportLayerSynchronous::ClTransportLayerSynchronous%954835554.hasinit preserve=no
  //## end ClTransportLayerSynchronous::ClTransportLayerSynchronous%954835554.hasinit
  //## begin ClTransportLayerSynchronous::ClTransportLayerSynchronous%954835554.initialization preserve=yes
  //## end ClTransportLayerSynchronous::ClTransportLayerSynchronous%954835554.initialization
{
  //## begin ClTransportLayerSynchronous::ClTransportLayerSynchronous%954835554.body preserve=yes

	m_Container = NULL;
	m_ControlTSAP = NULL;
	ClTSAP *pTSAP;
	ClControlPipesMaker ControlPipesMaker;

	// Crear TransportLayerIdentifier de control, añadirlo a la lista, 
	// y añadir su TSAP  al select 
	int i = 0;
	while (ControlPipesMaker.Init(ai_iControlPort + i++) != M4_SUCCESS)
		if (i > MAX_RETRIES)
		{
			m4Trace(fprintf(stderr,"ControlPipesMaker.Init != M4_SUCCESS\n"));
			m_iError = M4_TRANSPORT_CONTROL_PIPES_MAKER;
			
			return;

		}
	if (ControlPipesMaker.GetPipes(pTSAP, m_ControlTSAP) != M4_SUCCESS)
	{
		m4Trace(fprintf(stderr,"ControlPipesMaker.GetPipes != M4_SUCCESS\n"));
		m_iError = M4_TRANSPORT_CONTROL_PIPES_MAKER;

		return;
		//Error grave, abortar todo
	}
	pTSAP -> SetId ( 1 ) ;
//	ControlPipesMaker.Close();
#ifdef _WINDOWS
	m_ControlTSAP->GetSystemDescriptor();
	BOOL optval = true;
	if (0 != setsockopt(m_ControlTSAP->GetSystemDescriptor(),IPPROTO_TCP,TCP_NODELAY,(const char *)&optval,sizeof(optval)))
	{

		m4Trace(fprintf(stderr,"setsockopt on control pipe != 0\n"));
		m_iError = M4_TRANSPORT_CONTROL_PIPES_SETSOCKOPT;

		return;
		//Error grave, abortar todo
	}
#endif
	// m_ControlTSAP es el extremo en el que se escribe algo cuando se añade una acción
	// El otro extremo, pTSAP, lo añadimos al select como lectura, y creamos un TransportLayerIdentifier
	// con tipo CONTROLPIPE
	ClTransportLayerIdentifier *pTransportLayerIdentifier 
		= new ClTransportLayerIdentifier(pTSAP, CONTROL_PIPE); 
	m_Container = new ClTransportLayerContainer;
	m_Container->AddTransportLayerIdentifier(pTransportLayerIdentifier);
	m_Container->AddSelect(pTransportLayerIdentifier, M4_TRUE, M4_FALSE);

	Start();
  //## end ClTransportLayerSynchronous::ClTransportLayerSynchronous%954835554.body
}


ClTransportLayerSynchronous::~ClTransportLayerSynchronous ()
{
  //## begin ClTransportLayerSynchronous::~ClTransportLayerSynchronous%958576518.body preserve=yes
	if (m_Container != NULL)
		delete m_Container;
	if (m_ControlTSAP!= NULL)
		delete m_ControlTSAP;
  //## end ClTransportLayerSynchronous::~ClTransportLayerSynchronous%958576518.body
}



//## Other Operations (implementation)
m4return_t ClTransportLayerSynchronous::OpenPort (int ai_iPort, ClIOEventHandler *ai_pIOEventHandler)
{
  //## begin ClTransportLayerSynchronous::OpenPort%954835546.body preserve=yes
	ClTransportActionOpenPort *pAction;

	if (m_iError != M4_TRANSPORT_OK)
		return M4_ERROR;

	if ((pAction = (ClTransportActionOpenPort *)m_Container->RecoverAction(TRANSPORT_ACTION_OPEN_PORT)) == NULL)
		pAction = new ClTransportActionOpenPort();

	if (pAction == NULL)
	{
		m4Trace(fprintf(stderr, "Error, ClTransportActionOpenPort es NULL en OpenPort\n"));

		return M4_ERROR;
	}
	(ClTransportActionOpenPort *)pAction->Init(ai_iPort, NULL, ai_pIOEventHandler);
	m_Container->AddAction(pAction);
	((ClControlWriteTSAP*)m_ControlTSAP)->Write("a",1);

	return M4_SUCCESS;
  //## end ClTransportLayerSynchronous::OpenPort%954835546.body
}

m4return_t ClTransportLayerSynchronous::OpenSSLPort (int ai_iPort, ClIOEventHandler *ai_pIOEventHandler, char *ai_pCertificateFile, char *ai_pPrivateKey, char *ai_pCipher)
{
  //## begin ClTransportLayerSynchronous::OpenSSLPort%956570164.body preserve=yes
	ClTransportActionOpenSSLPort *pAction;
	if (m_iError != M4_TRANSPORT_OK)
		return M4_ERROR;

	int	iMode = 0;
	if (ai_pCipher != NULL)
		iMode = M4_SSL_SET_PREFERED_CIPHER;
	UseSSL (M4_SSL_SERVER, ai_pCertificateFile, ai_pPrivateKey);
	if ((pAction = (ClTransportActionOpenSSLPort *)
				m_Container->RecoverAction(TRANSPORT_ACTION_OPEN_SSL_PORT)) == NULL)
		pAction = new ClTransportActionOpenSSLPort();

	(ClTransportActionOpenSSLPort *)pAction->Init(iMode, ai_pCipher, ai_iPort, 
													NULL, ai_pIOEventHandler);
	m_Container->AddAction(pAction);
	((ClControlWriteTSAP*)m_ControlTSAP)->Write("a",1);

	return M4_SUCCESS;
  //## end ClTransportLayerSynchronous::OpenSSLPort%956570164.body
}

m4return_t ClTransportLayerSynchronous::ClosePort (m4tliid_t ai_Identifier, ClIOEventHandler *ai_pIOEventHandler)
{
  //## begin ClTransportLayerSynchronous::ClosePort%954835547.body preserve=yes
	if (m_iError != M4_TRANSPORT_OK)
		return M4_ERROR;

	ClTSAP *pTSAP =	(ClTSAP *)ai_Identifier;
	ClTransportActionClosePort *pAction;

	if ((pAction = (ClTransportActionClosePort*)
					m_Container->RecoverAction(TRANSPORT_ACTION_CLOSE_PORT)) == NULL)
		pAction = new ClTransportActionClosePort();

	(ClTransportActionClosePort*)pAction->Init(pTSAP, ai_pIOEventHandler);
	m_Container->AddAction(pAction);
	((ClControlWriteTSAP*)m_ControlTSAP)->Write("a",1);

	return M4_SUCCESS;
  //## end ClTransportLayerSynchronous::ClosePort%954835547.body
}

m4return_t ClTransportLayerSynchronous::BeginRead (m4tliid_t ai_Identifier, ClIOEventHandler *ai_pIOEventHandler)
{
  //## begin ClTransportLayerSynchronous::BeginRead%954835548.body preserve=yes
	if (m_iError != M4_TRANSPORT_OK)
		return M4_ERROR;

	ClTSAP *pTSAP =	(ClTSAP *)ai_Identifier;

	ClTransportActionBeginRead  *pAction;
	if ((pAction = (ClTransportActionBeginRead *)
					m_Container->RecoverAction(TRANSPORT_ACTION_BEGIN_READ)) == NULL)
		pAction = new ClTransportActionBeginRead ();

	(ClTransportActionBeginRead  *)pAction->Init(pTSAP, ai_pIOEventHandler);
	m_Container->AddAction(pAction);
	((ClControlWriteTSAP*)m_ControlTSAP)->Write("a",1);

	return M4_SUCCESS;
  //## end ClTransportLayerSynchronous::BeginRead%954835548.body
}

m4return_t ClTransportLayerSynchronous::Write (m4tliid_t ai_Identifier, ClIOEventHandler *ai_pIOEventHandler)
{
  //## begin ClTransportLayerSynchronous::Write%954835549.body preserve=yes
	//Escribir en modo no bloqueante; 
	//Si  no se ha escrito todo, crear acción de escribir con lo que falte
	if (m_iError != M4_TRANSPORT_OK)
		return M4_ERROR;


	ClTSAP *pTSAP =	(ClTSAP *)ai_Identifier;

	ClTransportActionWrite  *pAction;

	if ((pAction = (ClTransportActionWrite *)
						m_Container->RecoverAction(TRANSPORT_ACTION_WRITE)) == NULL)
		pAction = new ClTransportActionWrite();

	(ClTransportActionWrite *)pAction->Init(pTSAP, ai_pIOEventHandler);
	m_Container->AddAction(pAction);
	((ClControlWriteTSAP*)m_ControlTSAP)->Write("a",1);

	return M4_SUCCESS;
  //## end ClTransportLayerSynchronous::Write%954835549.body
}

m4return_t ClTransportLayerSynchronous::StopReading (m4tliid_t ai_Identifier, ClIOEventHandler *ai_pIOEventHandler)
{
  //## begin ClTransportLayerSynchronous::StopReading%954835550.body preserve=yes
	if (m_iError != M4_TRANSPORT_OK)
		return M4_ERROR;

	ClTSAP *pTSAP =	(ClTSAP *)ai_Identifier;

	ClTransportActionStopReading *pAction;
	if ((pAction = (ClTransportActionStopReading*)
				m_Container->RecoverAction(TRANSPORT_ACTION_STOP_READING)) == NULL)
		pAction = new ClTransportActionStopReading();

	(ClTransportActionStopReading *)pAction->Init(pTSAP, ai_pIOEventHandler);
	m_Container->AddAction(pAction);
	((ClControlWriteTSAP*)m_ControlTSAP)->Write("a",1);

	return M4_SUCCESS;
  //## end ClTransportLayerSynchronous::StopReading%954835550.body
}

m4return_t ClTransportLayerSynchronous::StopWriting (m4tliid_t ai_Identifier, ClIOEventHandler *ai_pIOEventHandler)
{
  //## begin ClTransportLayerSynchronous::StopWriting%954835551.body preserve=yes
	if (m_iError != M4_TRANSPORT_OK)
		return M4_ERROR;

	ClTSAP *pTSAP =	(ClTSAP *)ai_Identifier;


	ClTransportActionStopWriting *pAction;
	if ((pAction = (ClTransportActionStopWriting*)
					m_Container->RecoverAction(TRANSPORT_ACTION_STOP_WRITING)) == NULL)
		pAction = new ClTransportActionStopWriting();

	(ClTransportActionStopWriting*)pAction->Init(pTSAP, ai_pIOEventHandler);
	m_Container->AddAction(pAction);
	((ClControlWriteTSAP*)m_ControlTSAP)->Write("a",1);

	return M4_SUCCESS;
  //## end ClTransportLayerSynchronous::StopWriting%954835551.body
}

m4return_t ClTransportLayerSynchronous::ConnectSSL (char *ai_Host, int ai_iPort, ClIOEventHandler *ai_pIOEventHandler, char *ai_pCertificateFile, char *ai_Cipher)
{
  //## begin ClTransportLayerSynchronous::ConnectSSL%956570158.body preserve=yes
	if (m_iError != M4_TRANSPORT_OK)
		return M4_ERROR;

	int	iMode = 0;
	if (ai_Cipher != NULL)
		iMode = M4_SSL_SET_PREFERED_CIPHER;
	UseSSL (M4_SSL_CLIENT_CERTIFICATE, NULL, ai_pCertificateFile );
	ClTransportActionSSLConnect *pAction;
	if ((pAction = (ClTransportActionSSLConnect *)
				m_Container->RecoverAction(TRANSPORT_ACTION_SSL_CONNECT)) == NULL)
		pAction = new ClTransportActionSSLConnect();

	(ClTransportActionSSLConnect*)pAction->Init(iMode, ai_Cipher, ai_Host, ai_iPort, NULL, ai_pIOEventHandler);

	m_Container->AddAction(pAction);
	((ClControlWriteTSAP*)m_ControlTSAP)->Write("a",1);

	return M4_SUCCESS;
  //## end ClTransportLayerSynchronous::ConnectSSL%956570158.body
}

m4return_t ClTransportLayerSynchronous::Connect (char *ai_Host, int ai_iPort, ClIOEventHandler *ai_pIOEventHandler)
{
  //## begin ClTransportLayerSynchronous::Connect%954835552.body preserve=yes
	if (m_iError != M4_TRANSPORT_OK)
		return M4_ERROR;

	ClTransportActionConnect *pAction;
	if ((pAction = (ClTransportActionConnect*)
				m_Container->RecoverAction(TRANSPORT_ACTION_CONNECT)) == NULL)
		pAction = new ClTransportActionConnect();

	(ClTransportActionConnect*)pAction->Init(ai_Host, ai_iPort, 
												NULL, ai_pIOEventHandler);
	m_Container->AddAction(pAction);
	((ClControlWriteTSAP*)m_ControlTSAP)->Write("a",1);

	return M4_SUCCESS;
  //## end ClTransportLayerSynchronous::Connect%954835552.body
}

m4return_t ClTransportLayerSynchronous::StopConnection (m4tliid_t ai_Identifier, ClIOEventHandler *ai_pIOEventHandler)
{
  //## begin ClTransportLayerSynchronous::StopConnection%954835553.body preserve=yes
	if (m_iError != M4_TRANSPORT_OK)
		return M4_ERROR;

	ClTSAP *pTSAP =	(ClTSAP *)ai_Identifier;


	ClTransportActionCloseConnection *pAction;
	if ((pAction = (ClTransportActionCloseConnection *)
				m_Container->RecoverAction(TRANSPORT_ACTION_CLOSE_CONNECTION)) == NULL)
		pAction = new ClTransportActionCloseConnection();

	(ClTransportActionCloseConnection*)pAction->Init(pTSAP, ai_pIOEventHandler);
	m_Container->AddAction(pAction);
	((ClControlWriteTSAP*)m_ControlTSAP)->Write("a",1);

	return M4_SUCCESS;
  //## end ClTransportLayerSynchronous::StopConnection%954835553.body
}

void ClTransportLayerSynchronous::Run ()
{
  //## begin ClTransportLayerSynchronous::Run%954835555.body preserve=yes
	int							ret; 
	int							writed;
	m4bool_t					bExistRead, 
								bExistWrite;
	ClTSAP						*pTSAP;
	ClTSAP						*pNewTSAP;
	ClTransportLayerIdentifier	*pTransportLayerIdentifier;
	ClTransportLayerIdentifier	*pNewTransportLayerIdentifier;
	ClIOEventHandler			*pIOEventHandler;
	ClTransportAction *					pAction;
	char						buffer[1024];

	while (M4_TRUE)
	{

		bExistRead = bExistWrite = M4_TRUE;
		//Leer y ejecutar Lista acciones pendientes
		while ((pAction = m_Container->GetAction()) != NULL)
		{
			pAction->Execute(m_Container);
			m_Container->KeepAction(*pAction);
//			delete pAction;
		}
		if(m_Container->WantStop() == M4_TRUE)
		{
			pIOEventHandler = m_Container->GetWantStopIOEventHandler();
			if (pIOEventHandler)
				pIOEventHandler->ProccessStop(M4_SUCCESS);

			return;
		}
		ret = m_Container->Select(0,0);
		if (ret <= 0)
		{
			//Error
			m4Trace(printf("Select devuelve error\n"));
#ifdef _WINDOWS
			m4Trace(cout <<GetLastError()<<endl);
#endif
			m_iError = M4_TRANSPORT_SELECT;
			break;
		}
		while (bExistRead || bExistWrite)
		{
			//if ((bExistRead) && (ret>0))
			if (bExistRead) 
			{
				pTransportLayerIdentifier = m_Container->NextRead();
				if (pTransportLayerIdentifier != NULL)
				{
				//	ret--;
				//	if(!ret)
				//		bExistRead=M4_FALSE;
					//Buscar TSAP en mapa de ClTransportLayerIdentifier
					//m_MapTransportLayerIdentifier
					//Si el tipo nos indica que es el socket de control, no hacer nada
					pTSAP = pTransportLayerIdentifier->GetTSAP();
					//FALTA TRATAMIENTO DE ERRORES
					switch (pTransportLayerIdentifier->GetTSAPType())
					{
					default:
						m4Trace(fprintf(stderr, "Error crítico de lógica: TSAP de tipo desconocido en NextRead\n"));
						m_iError = M4_TRANSPORT_SELECT;
						break;

					case CONNECTION_SOCKET:
						pIOEventHandler = pTransportLayerIdentifier->GetIOEventHandlerRead();
						ret = pTSAP->Read(pIOEventHandler->GetBuffer(),
										pIOEventHandler->GetBufferSize() + M4_COMM_DEFAULT_BUFFER_MARGIN_SIZE);
				
						// dump transport for debugging
						DumpData(pTransportLayerIdentifier->GetId(),pIOEventHandler->GetBuffer(),ret);
						
						if (ret > 0)
						{
							pTransportLayerIdentifier->m_ulReaded += ret;
							if (pIOEventHandler->ProccessRead(pTSAP, ret) != M4_SUCCESS)
								m_Container->RemoveSelect(pTransportLayerIdentifier, M4_TRUE, M4_FALSE);
						}
						else
						{
							if( ret || (!pTSAP->IsSSL()) )
							{
								if (pIOEventHandler->ProccessRead(pTSAP, ret) != M4_SUCCESS)
									m_Container->RemoveSelect(pTransportLayerIdentifier, M4_TRUE, M4_FALSE);
							}
						}
						break;
					case CONTROL_PIPE:
						((ClControlReadTSAP*)pTSAP)->Read(buffer,sizeof(buffer));
						break;
					case LISTEN_SOCKET:
						pIOEventHandler = pTransportLayerIdentifier->GetIOEventHandlerRead();
						pNewTSAP = ((ClServerTSAPSelectable *)pTSAP)->Accept();
						if( pNewTSAP )
						{
							pNewTSAP -> SetId ( g_iTSAPId ++ ) ;

							pNewTransportLayerIdentifier = 
								new ClTransportLayerIdentifier(pNewTSAP, 
								CONNECTION_SOCKET, 
								pIOEventHandler);
							m_Container->AddTransportLayerIdentifier(pNewTransportLayerIdentifier);
							pIOEventHandler->ProccessNewConnection((m4tliid_t)pNewTSAP, M4_SUCCESS);
						}
						break;
					}
				}
				else
					bExistRead = M4_FALSE;
			}
			//if ((bExistWrite) && (ret>0))
			if (bExistWrite)
			{
				pTransportLayerIdentifier = m_Container->NextWrite();
				if (pTransportLayerIdentifier != NULL)
				{
				//	ret--;
				//	if(!ret)
				//		bExistWrite=M4_FALSE;
					//Buscar TSAP en mapa de ClTransportLayerIdentifier
					//m_MapTransportLayerIdentifier
					pIOEventHandler = pTransportLayerIdentifier->GetIOEventHandlerWrite();
					pTSAP = pTransportLayerIdentifier->GetTSAP();
					writed = pTSAP->Write(pIOEventHandler->GetBuffer(),
						pIOEventHandler->GetBufferSize());
					if (writed > 0)
						pTransportLayerIdentifier->m_ulWrited += writed;
					
					switch (writed)
					{
					case -1:
						m_Container->RemoveSelect(pTransportLayerIdentifier, M4_FALSE, M4_TRUE); //Remove nuevo, que especifique tipo de remove:lectura, escritura o especial
						pIOEventHandler->ProccessWrite(pTSAP, M4_ERROR);
						break;

					case -2:
						// Se ha llenado el buffer del sistema, esperar a que select nos vuelva a despertar
						break;

					default:
						if (writed == (int)pIOEventHandler->GetBufferSize()) 
						{
							//Hemos terminado
							m_Container->RemoveSelect(pTransportLayerIdentifier, M4_FALSE, M4_TRUE); //Remove nuevo, que especifique tipo de remove:lectura, escritura o especial
							pIOEventHandler->ProccessWrite(pTSAP, M4_SUCCESS);
						}
						else
						{
							pIOEventHandler->SetBuffer(pIOEventHandler->GetBuffer() + writed);
							pIOEventHandler->SetBufferSize(pIOEventHandler->GetBufferSize() - writed);
						}
					}
				}
				else
					bExistWrite = M4_FALSE;
			}
		}
	}
  //## end ClTransportLayerSynchronous::Run%954835555.body
}

m4return_t ClTransportLayerSynchronous::UseSSL (int ai_iMode, char *ai_pCertificateFile, char *ai_pPrivateKey)
{
  //## begin ClTransportLayerSynchronous::UseSSL%956570156.body preserve=yes
	struct 
	{
		char *PrivateKeyFile;
		char *CertificateFile;
	}SSLData;

	if(NULL!=ai_pPrivateKey)
	{
		SSLData.PrivateKeyFile = new char [strlen(ai_pPrivateKey) +1];
		strcpy(SSLData.PrivateKeyFile, ai_pPrivateKey);
	}
	else 
		SSLData.PrivateKeyFile =NULL;

	if(NULL!=ai_pCertificateFile)
	{
		SSLData.CertificateFile = new char [strlen(ai_pCertificateFile) +1];
		strcpy(SSLData.CertificateFile, ai_pCertificateFile);
	}
	else
		SSLData.CertificateFile =NULL;

	m4return_t ret = (SocketInit(ai_iMode, &SSLData) == 0)?M4_SUCCESS:M4_ERROR;

	if(NULL!=ai_pPrivateKey)
		delete SSLData.PrivateKeyFile;

	if(NULL!=ai_pCertificateFile)
		delete SSLData.CertificateFile;
	
	return ret;


  //## end ClTransportLayerSynchronous::UseSSL%956570156.body
}

m4return_t ClTransportLayerSynchronous::StopTransportLayer (ClIOEventHandler *ai_pIOEventHandler)
{
  //## begin ClTransportLayerSynchronous::StopTransportLayer%956912718.body preserve=yes
	
	ClTransportActionStop *pAction; 
	if ((pAction = (ClTransportActionStop*)
				m_Container->RecoverAction(TRANSPORT_ACTION_STOP)) == NULL)
		pAction = new ClTransportActionStop();

	(ClTransportActionStop *)pAction->Init(NULL, ai_pIOEventHandler);
	m_Container->AddAction(pAction);
	((ClControlWriteTSAP*)m_ControlTSAP)->Write("a",1);

	return M4_SUCCESS;
  //## end ClTransportLayerSynchronous::StopTransportLayer%956912718.body
}

int ClTransportLayerSynchronous::GetTSAPError (m4tliid_t ai_Identifier)
{
  //## begin ClTransportLayerSynchronous::GetTSAPError%957367977.body preserve=yes
	if (m_Container->GetTransportLayerIdentifier((ClTSAP *)ai_Identifier) != NULL)
	{
		return ((ClTSAP *)ai_Identifier)->Error();
	}

	return -1;
  //## end ClTransportLayerSynchronous::GetTSAPError%957367977.body
}

m4return_t ClTransportLayerSynchronous::GetTransferedBytes (m4tliid_t ai_Identifier, ClIOEventHandler *ai_pIOEventHandler)
{
  //## begin ClTransportLayerSynchronous::GetTransferedBytes%958059749.body preserve=yes
	ClTransportActionGetTransferedBytes *pAction;
	if ((pAction = (ClTransportActionGetTransferedBytes*)
				m_Container->RecoverAction(TRANSPORT_ACTION_GET_TRANSFERED_BYTES)) == NULL)
		pAction = new ClTransportActionGetTransferedBytes();

	(ClTransportActionGetTransferedBytes *)pAction->Init((ClTSAP *)ai_Identifier, ai_pIOEventHandler);
	m_Container->AddAction(pAction);
	((ClControlWriteTSAP*)m_ControlTSAP)->Write("a",1);

	return M4_SUCCESS;
  //## end ClTransportLayerSynchronous::GetTransferedBytes%958059749.body
}

// Additional Declarations
  //## begin ClTransportLayerSynchronous%38D0B6390241.declarations preserve=yes
m4return_t ClTransportLayerSynchronous::GetTLIHostname (m4tliid_t ai_Identifier,m4pchar_t ai_szAddress, int ai_iMaxLen)
{
	if((NULL==ai_Identifier) || (NULL==ai_szAddress) || (ai_iMaxLen==0))
	{
		m4Trace(cerr<<"ClTransportLayerSynchronous::GetTLIHostname Invalid paramms"<<endl);
		return M4_ERROR;
	}

	ClTSAP *theTSAP=(ClTSAP *)ai_Identifier;
	if(theTSAP->GetRemoteHostname (ai_szAddress, ai_iMaxLen))
		return M4_SUCCESS;
	else
		return M4_ERROR;
}
  //## end ClTransportLayerSynchronous%38D0B6390241.declarations

//## begin module%38D88438001C.epilog preserve=yes
//## end module%38D88438001C.epilog
