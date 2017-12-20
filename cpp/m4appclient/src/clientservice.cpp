//## begin module%34951237024C.cm preserve=no
//## end module%34951237024C.cm

//## begin module%34951237024C.cp preserve=no
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
//## end module%34951237024C.cp

//## Module: clientservice%34951237024C; Package body
//## Subsystem: M4AppClient::src%3796F8B303AD
//## Source file: F:\integration\m4appclient\src\clientservice.cpp

//## begin module%34951237024C.additionalIncludes preserve=no
//## end module%34951237024C.additionalIncludes

//## begin module%34951237024C.includes preserve=yes
//#include <compressdatastorage.hpp>
#include <m4props.hpp>
#include <basiclog.hpp>
#include <clientapi.hpp>
// no funciona el body reference de rose
#include <clientconnection.hpp>
#include <clientsession.hpp>
//## end module%34951237024C.includes

// Clienttag
#include <clienttag.hpp>
// m4types
#include <m4types.hpp>
// m4date
#include <m4date.hpp>
// m4log
#include <m4log.hpp>
// controlpdus
#include <controlpdus.hpp>
// Protocol
#include <protocol.hpp>
// clientbasicdef
#include <clientbasicdef.hpp>
// clientservice
#include <clientservice.hpp>
// clientrequest
#include <clientrequest.hpp>
// reqdescriptor
#include <reqdescriptor.hpp>


//## begin module%34951237024C.declarations preserve=no
//## end module%34951237024C.declarations

//## begin module%34951237024C.additionalDeclarations preserve=yes

#include <m4eventhandlers.hpp>
//## end module%34951237024C.additionalDeclarations

// Class ClCCAdminService 

ClCCAdminService::ClCCAdminService (m4pchar_t ai_szServiceId, ClCCSession *ai_pSession)
  //## begin ClCCAdminService::ClCCAdminService%885839988.hasinit preserve=no
  //## end ClCCAdminService::ClCCAdminService%885839988.hasinit
  //## begin ClCCAdminService::ClCCAdminService%885839988.initialization preserve=yes
    : ClCCService(ai_szServiceId, ai_pSession)
  //## end ClCCAdminService::ClCCAdminService%885839988.initialization
{
  //## begin ClCCAdminService::ClCCAdminService%885839988.body preserve=yes
  //## end ClCCAdminService::ClCCAdminService%885839988.body
}


ClCCAdminService::~ClCCAdminService ()
{
  //## begin ClCCAdminService::~ClCCAdminService%885839989.body preserve=yes
  //## end ClCCAdminService::~ClCCAdminService%885839989.body
}



//## Other Operations (implementation)
m4return_t ClCCAdminService::ConnectRequest (m4handle_t& ao_hRID, ClTagList *ao_oTag, m4uint32_t ai_uiTimeout, ClTimeoutFunction ai_pTimeoutFunction)
{
  //## begin ClCCAdminService::ConnectRequest%885839990.body preserve=yes
	ClCCRequest *pRequest;
	m4return_t iRet;

	iRet=_PrepareRequest(ai_uiTimeout,pRequest,ao_hRID);
	if(M4_ERROR!=iRet)
	{
		iRet=CreateConnectPDU(pRequest,ao_oTag);
		if(M4_ERROR!=iRet)
		{
			iRet=_GetAnswer(pRequest,NULL,"ConnectRequest","Error arrived from server in ConnectRequest",ai_pTimeoutFunction,ao_oTag);

			if((iRet!=M4_ERROR) && (M4_ERROR == FullRemoveRequest (ao_hRID)))
			{
				m4Trace(cerr << "Couldn't remove ConnectRequest" <<endl);
				SETCODE(M4_ERR_REQ_NOT_DEL_FROM_DICT,ERRORLOG,"Couldn't remove ConnectRequest");
			}
		}
	}
	return iRet;
  //## end ClCCAdminService::ConnectRequest%885839990.body
}

m4return_t ClCCAdminService::DisconnectRequest (m4handle_t& ao_hRID, m4uint32_t ai_uiTimeout, ClTimeoutFunction ai_pTimeoutFunction)
{
  //## begin ClCCAdminService::DisconnectRequest%890127474.body preserve=yes
	m4return_t iRet;
	ClCCRequest *pRequest;

	iRet=_PrepareRequest(ai_uiTimeout,pRequest,ao_hRID);
	if(M4_ERROR!=iRet)
	{
		iRet=CreateDisconnectPDU(pRequest);
		if(M4_ERROR!=iRet)
		{
			iRet=_GetAnswer(pRequest,NULL,"DisconnectRequest","Error arrived from server in DisconnectRequest",ai_pTimeoutFunction);

			if((iRet!=M4_ERROR) && (M4_ERROR == FullRemoveRequest (ao_hRID)))
			{
				SETCODE(M4_ERR_REQ_NOT_DELETED,ERRORLOG,"Couldn't remove DisconnectRequest");
				m4Trace(cerr << "Couldn't remove DisconnectRequest" <<endl);
			}
		}
	}
	return iRet;
  //## end ClCCAdminService::DisconnectRequest%890127474.body
}

m4return_t ClCCAdminService::SetRoleRequest (m4handle_t& ao_hRID, m4pchar_t ai_pRoleId, m4bool_t ai_bIsDef, m4uint32_t ai_uiTimeout, ClTimeoutFunction ai_pTimeoutFunction)
{
  //## begin ClCCAdminService::SetRoleRequest%890127475.body preserve=yes
	m4return_t iRet;
	ClCCRequest *pRequest;

	iRet=_PrepareRequest(ai_uiTimeout,pRequest,ao_hRID);
	if(M4_ERROR!=iRet)
	{
		iRet=CreateSetRolePDU(pRequest,ai_pRoleId,ai_bIsDef);
		if(M4_ERROR!=iRet)
		{
			iRet=_GetAnswer(pRequest,NULL,"SetRoleRequest","Error arrived from server in SetRoleRequestRequest",ai_pTimeoutFunction);
	
			if((iRet!=M4_ERROR) && (M4_ERROR == FullRemoveRequest (ao_hRID)))
			{
				SETCODE(M4_ERR_REQ_NOT_DELETED,ERRORLOG,"Couldn't remove SetRoleRequest");
				m4Trace(cerr << "Couldn't remove SetRoleRequest" <<endl);
			}
		}
	}
	return iRet;
  //## end ClCCAdminService::SetRoleRequest%890127475.body
}

m4return_t ClCCAdminService::CreateAuthPDU (ClCCRequest *ai_poRequest, m4bool_t ai_bDefaultRole, ClTagList *ao_oTag, m4uint16_t ai_iFlags )
{
  //## begin ClCCAdminService::CreateAuthPDU%889606015.body preserve=yes
	m4return_t iReturn;
	
	ClPDU * pPDU;

	_CreatePDU(M4_ID_PDU_AUTHENTICATION,ai_poRequest,pPDU);
	ClAuthenticationPDU *pAuPDU=(ClAuthenticationPDU *)pPDU; 

	ClCCUser	*pUser;
	m_pSession->GetUser(pUser);
	m4pchar_t	pUsername;
	pUser->GetName(pUsername);
	if(pUsername)
		pAuPDU -> SetUserId(pUsername, strlen(pUsername)+1);
	else
		pAuPDU -> SetUserId("",0);				

	m4pchar_t	pPasswd;
	m4uint32_t iPassLen;
	pUser->GetPasswd(pPasswd,iPassLen);
	if(pPasswd)
		pAuPDU -> SetPassword(pPasswd, iPassLen);
	
	pAuPDU -> SetDefaultRole(ai_bDefaultRole);

	pAuPDU -> SetFlags ( ai_iFlags ) ;

	if(ao_oTag)
	{
		if(ao_oTag->GetSize())
		{
			M4DataStorage* pInputData=pAuPDU->GetDataStorage();
			if(NULL==pInputData)
			{
				pInputData=M4DataStorage::GetNewDataStorage();
				pAuPDU->SetDataStorage(pInputData);
			}
			iReturn=ao_oTag->TagList2DataStorage(pInputData);
			m4DebugExecute(if(iReturn==M4_ERROR) cerr<<"Error en TagList2DataStorage"<<endl);
		}
	}

	return M4_SUCCESS;
  //## end ClCCAdminService::CreateAuthPDU%889606015.body
}

m4return_t ClCCAdminService::CreateDisconnectPDU (ClCCRequest *ai_poRequest)
{
  //## begin ClCCAdminService::CreateDisconnectPDU%889606016.body preserve=yes
	
	ClPDU *pPDU;
	
	_CreatePDU(M4_ID_PDU_NEWDISCONNECT,ai_poRequest,pPDU);
	
	return M4_SUCCESS;

  //## end ClCCAdminService::CreateDisconnectPDU%889606016.body
}

m4return_t ClCCAdminService::CreateSetRolePDU (ClCCRequest *ai_poRequest, m4pchar_t ai_pRoleId, m4bool_t ai_bIsDef)
{
  //## begin ClCCAdminService::CreateSetRolePDU%890127477.body preserve=yes
	
	ClPDU *pPDU;
	_CreatePDU(M4_ID_PDU_NEWSETROLE,ai_poRequest,pPDU);
	ClNewSetRolePDU *pSRPDU=(ClNewSetRolePDU *)pPDU;

	if (NULL == ai_pRoleId)
		pSRPDU->SetRole(NULL, 0);
	else
	{
		pSRPDU->SetRole(ai_pRoleId, strlen(ai_pRoleId)+1);
		pSRPDU->SetDefaultRole(ai_bIsDef);
	}

	return M4_SUCCESS;

  //## end ClCCAdminService::CreateSetRolePDU%890127477.body
}

m4return_t ClCCAdminService::ChangePasswordRequest (m4handle_t &ao_hRID, M4ClString &ai_oOldPasswd, M4ClString &ai_oNewPasswd, m4uint32_t ai_uiTimeout, ClTimeoutFunction ai_pTimeoutFunction)
{
  //## begin ClCCAdminService::ChangePasswordRequest%898848503.body preserve=yes

	m4return_t iRet;
	ClCCRequest *pRequest;

	iRet=_PrepareRequest(ai_uiTimeout,pRequest,ao_hRID);
	if(M4_ERROR!=iRet)
	{
		iRet=CreateChangePasswordPDU(pRequest,ai_oOldPasswd, ai_oNewPasswd);
		if(M4_ERROR!=iRet)
		{
			iRet=_GetAnswer(pRequest,NULL,"ChangePasswordRequest","Error arrived from server in ChangePasswordRequest",ai_pTimeoutFunction);

			if((iRet!=M4_ERROR) && (M4_ERROR == FullRemoveRequest (ao_hRID)))
			{
				SETCODE(M4_ERR_REQ_NOT_DELETED,ERRORLOG,"Couldn't remove ChangePasswordRequest");
				m4Trace(cerr << "Couldn't remove ChangePasswordRequest" <<endl);
			}
		}
	}
	return iRet;
  //## end ClCCAdminService::ChangePasswordRequest%898848503.body
}

m4return_t ClCCAdminService::CreateChangePasswordPDU (ClCCRequest *ai_poRequest, M4ClString &ai_oOldPasswd, M4ClString &ai_oNewPasswd)
{
  //## begin ClCCAdminService::CreateChangePasswordPDU%898848504.body preserve=yes

	ClPDU *pPDU;
	_CreatePDU(M4_ID_PDU_NEWCHANGEPASSWORD,ai_poRequest,pPDU);
	ClNewChangePasswordPDU *pDhPPDU=(ClNewChangePasswordPDU  *)pPDU;
	pDhPPDU->SetCurrentPassword(ai_oOldPasswd,ai_oOldPasswd.size()+1);
	pDhPPDU->SetNewPassword(ai_oNewPasswd,ai_oNewPasswd.size()+1);

	return M4_SUCCESS;
  //## end ClCCAdminService::CreateChangePasswordPDU%898848504.body
}

m4return_t ClCCAdminService::PingRequest (m4handle_t&ao_hRID, m4uint32_t ai_uiElapsedTime, m4uint32_t ai_uiTimeout, ClTimeoutFunction ai_pTimeoutFunction)
{
	m4return_t iRet;
	ClCCRequest *pRequest;

	iRet=_PrepareRequest(ai_uiTimeout,pRequest,ao_hRID);
	if(M4_ERROR!=iRet)
	{
		iRet=CreatePingPDU(pRequest, ai_uiElapsedTime);

		if(M4_ERROR!=iRet)
		{
			iRet=_GetAnswer(pRequest,NULL,"PingRequest","Error arrived from server in PingRequest",ai_pTimeoutFunction);

			if((iRet!=M4_ERROR) && (M4_ERROR == FullRemoveRequest (ao_hRID)))
			{
				SETCODE(M4_ERR_REQ_NOT_DELETED,ERRORLOG,"Couldn't remove PingRequest");
				m4Trace(cerr << "Couldn't remove PingRequest" <<endl);
			}
		}
	}
	return iRet;
}

m4return_t ClCCAdminService::CreatePingPDU (ClCCRequest *ai_poRequest, m4uint32_t ai_uiElapsedTime)
{
	ClPDU *pPDU;
	_CreatePDU(M4_ID_PDU_PING, ai_poRequest, pPDU);
	ClPingPDU *pPingPDU=(ClPingPDU *)pPDU;
	pPingPDU->SetElapsedTime(ai_uiElapsedTime);

	return M4_SUCCESS;
}

m4return_t ClCCAdminService::AuthenticateRequest (m4handle_t& ao_hRID, m4pchar_t & ao_strX509Cert, m4uint32_t & aoi_iX509CertBufLen, m4pchar_t & ao_strSessionContext, m4uint32_t & aoi_iSessContextBufLen , ClTagList *ao_oTag, m4bool_t ai_bDefaultRole, m4uint32_t ai_uiTimeout, ClTimeoutFunction ai_pTimeoutFunction, m4uint16_t ai_iFlags)
{
  //## begin ClCCAdminService::AuthenticateRequest%899807018.body preserve=yes
	m4return_t iRet,iGetAnswerRet;
	ClCCRequest *pRequest;
	m4pchar_t	szCertificate, szSessContext ;
	ClPDU *pOutputPDU=NULL;
	m4bool_t copy = M4_FALSE ;
	m4uint32_t iCertLen , iSessContextLen ;


	iRet=_PrepareRequest(ai_uiTimeout,pRequest,ao_hRID);
	if(M4_ERROR!=iRet)
	{
		iRet=CreateAuthPDU(pRequest,ai_bDefaultRole,ao_oTag, ai_iFlags);
		if(M4_ERROR!=iRet)
		{
			iRet = iGetAnswerRet=_GetAnswer(pRequest,NULL,"AuthenticateRequest","Logon failed.Wrong login or password.Contact with your administrator.",ai_pTimeoutFunction,ao_oTag);


			if ( iGetAnswerRet != M4_ERROR )
			{
				pOutputPDU = pRequest -> GetOutputPDU ( ) ;


				iRet = ((ClPDUWithData*)  pOutputPDU  ) ->GetCertificate ( szCertificate , iCertLen ) ;

				if (  (iRet == M4_SUCCESS ) && ( iCertLen  > 0 ) )
				{
					if ( ao_strX509Cert )
					{
						if ( aoi_iX509CertBufLen > iCertLen )
						{
							copy = M4_TRUE ;
							memcpy ( ao_strX509Cert, szCertificate , iCertLen ) ;
							ao_strX509Cert [iCertLen] = 0 ;
							delete [] szCertificate ;
						}
					}

					if ( ! copy )
					{
						ao_strX509Cert = szCertificate ;
					}

					aoi_iX509CertBufLen = iCertLen ;

				}
				
				copy = M4_FALSE ;

				iRet = ((ClPDUWithData*)  pOutputPDU  ) ->GetSessContext ( szSessContext , iSessContextLen ) ;

				if ( (iRet == M4_SUCCESS ) && ( iSessContextLen > 0 ) )
				{
					if ( ao_strSessionContext )
					{
						if ( aoi_iSessContextBufLen  > iSessContextLen )
						{
							copy = M4_TRUE ;
							memcpy ( ao_strSessionContext,  szSessContext , iSessContextLen ) ;
							ao_strSessionContext [iSessContextLen ] = 0 ;
							delete [] szSessContext ;
						}
					}

					if ( ! copy )
					{
						ao_strSessionContext = szSessContext ;
					}

					aoi_iSessContextBufLen = iSessContextLen  +1;

				}
			}

			iRet = iGetAnswerRet ;

			if((iRet!=M4_ERROR) && (M4_ERROR == FullRemoveRequest (ao_hRID)))
			{
				SETCODE(M4_ERR_REQ_NOT_DELETED,ERRORLOG,"Couldn't remove AuthenticateRequest");
				m4Trace(cerr << "Couldn't Remove AuthenticateRequest" <<endl);
			}

		}
	}
	return iRet;
  //## end ClCCAdminService::AuthenticateRequest%899807018.body
}

m4return_t ClCCAdminService::CreateConnectPDU (ClCCRequest *ai_poRequest, ClTagList *ao_oTag)
{
  //## begin ClCCAdminService::CreateConnectPDU%899807019.body preserve=yes
	m4return_t iReturn;

	ClPDU *pPDU;
	iReturn=_CreatePDU(M4_ID_PDU_CONNECT,ai_poRequest,pPDU);
	if(M4_ERROR!=iReturn)
	{
		ClConnectPDU *pCPDU=(ClConnectPDU *)pPDU;
		if(ao_oTag)
		{

			// adds the server version for compatibility mode
			ao_oTag->SetTag(M4_PROP_SERVER_INTERNAL_VERSION);

			M4DataStorage* pInputData;
			pInputData=pCPDU->GetDataStorage();
			if(NULL==pInputData)
			{
				pInputData=M4DataStorage::GetNewDataStorage();
				pCPDU->SetDataStorage(pInputData);
			}
			iReturn=ao_oTag->TagList2DataStorage(pInputData);
			m4DebugExecute(if(iReturn==M4_ERROR) cerr<<"Error en TagList2DataStorage"<<endl);
		}
	}
	return iReturn;
  //## end ClCCAdminService::CreateConnectPDU%899807019.body
}

// Additional Declarations
  //## begin ClCCAdminService%34CCD67A002B.declarations preserve=yes
m4return_t ClCCAdminService::AuthenticateRequest (m4handle_t& ao_hRID, m4pchar_t ai_strX509Cert, m4uint32_t ai_iX509CertLen,  m4pchar_t ai_strSessionContext , m4pchar_t & ao_strX509Cert, m4uint32_t & aoi_iX509CertBufLen,  m4pchar_t & ao_strSessionContext, m4uint32_t & aoi_iSessContextBufLen, ClTagList *ao_oTag , m4bool_t ai_bDefaultRole , m4uint32_t ai_uiTimeout , ClTimeoutFunction ai_pTimeoutFunction , m4uint16_t ai_iFlags )
{
  //## begin ClCCAdminService::AuthenticateRequest%899807018.body preserve=yes
	m4return_t iRet,iGetAnswerRet;
	ClCCRequest *pRequest;
	m4pchar_t	szCertificate, szSessContext ;
	ClPDU *pOutputPDU=NULL , *inputPDU = NULL ;
	m4uint32_t iCertLen , iSessContextLen ;
	m4bool_t copy = M4_FALSE ;


	iRet=_PrepareRequest(ai_uiTimeout,pRequest,ao_hRID);
	if(M4_ERROR!=iRet)
	{
		iRet=CreateAuthPDU(pRequest,ai_bDefaultRole,ao_oTag, ai_iFlags);
		if(M4_ERROR!=iRet)
		{
			if ( inputPDU = pRequest -> GetInputPDU ( ) )
			{
				if (ai_strX509Cert && ai_iX509CertLen )
					inputPDU -> SetCertificate ( ai_strX509Cert , ai_iX509CertLen ) ;
				
				if ( ai_strSessionContext )
					inputPDU -> SetSessContext ( ai_strSessionContext , strlen ( ai_strSessionContext ) ) ;
			}

			iRet = iGetAnswerRet=_GetAnswer(pRequest,NULL,"AuthenticateRequest","Logon failed.Wrong login or password.Contact with your administrator.",ai_pTimeoutFunction,ao_oTag);

			if ( iRet == M4_SUCCESS )
			{
				pOutputPDU = pRequest -> GetOutputPDU ( ) ;

				iRet = ((ClPDUWithData*)  pOutputPDU  ) ->GetCertificate ( szCertificate , iCertLen ) ;

				if (  (iRet == M4_SUCCESS ) && ( iCertLen  > 0 ) )
				{
					if ( ao_strX509Cert )
					{
						if ( aoi_iX509CertBufLen > iCertLen )
						{
							copy = M4_TRUE ;
							memcpy ( ao_strX509Cert, szCertificate , iCertLen ) ;
							ao_strX509Cert [iCertLen] = 0 ;
							delete [] szCertificate ;
						}
					}

					if ( ! copy )
					{
						ao_strX509Cert = szCertificate ;
					}

					aoi_iX509CertBufLen = iCertLen +1;

				}
				
				copy = M4_FALSE ;

				iRet = ((ClPDUWithData*)  pOutputPDU  ) ->GetSessContext ( szSessContext , iSessContextLen ) ;

				if ( (iRet == M4_SUCCESS ) && ( iSessContextLen > 0 ) )
				{
					if ( ao_strSessionContext )
					{
						if ( aoi_iSessContextBufLen  > iSessContextLen )
						{
							copy = M4_TRUE ;
							memcpy ( ao_strSessionContext,  szSessContext , iSessContextLen ) ;
							ao_strSessionContext [iSessContextLen ] = 0 ;
							delete [] szSessContext ;
						}
					}

					if ( ! copy )
					{
						ao_strSessionContext = szSessContext ;
					}

					aoi_iSessContextBufLen = iCertLen +1;

				}
			}

			iRet = iGetAnswerRet ;

			if((iRet!=M4_ERROR) && (M4_ERROR == FullRemoveRequest (ao_hRID)))
			{
				SETCODE(M4_ERR_REQ_NOT_DELETED,ERRORLOG,"Couldn't remove AuthenticateRequest");
				m4Trace(cerr << "Couldn't Remove AuthenticateRequest" <<endl);
			}

		}
	}
	return iRet;
  //## end ClCCAdminService::AuthenticateRequest%899807018.body
}



  //## end ClCCAdminService%34CCD67A002B.declarations

// Class ClCCService 






ClCCService::ClCCService (m4pchar_t ai_szServiceId, ClCCSession *ai_pSession)
  //## begin ClCCService::ClCCService%882789765.hasinit preserve=no
  //## end ClCCService::ClCCService%882789765.hasinit
  //## begin ClCCService::ClCCService%882789765.initialization preserve=yes
  //## end ClCCService::ClCCService%882789765.initialization
{
  //## begin ClCCService::ClCCService%882789765.body preserve=yes

	m_pSession = ai_pSession;

	if( ai_szServiceId )
	{
		m_szServiceId  = new m4char_t [ strlen ( ai_szServiceId) + 1 ] ;
		strcpy ( m_szServiceId  , ai_szServiceId ) ;
	}
		
	m_MutexDicRequests.Init();
	
  //## end ClCCService::ClCCService%882789765.body
}


ClCCService::~ClCCService ()
{
  //## begin ClCCService::~ClCCService%881313195.body preserve=yes

	CancelAllRequests();

	if(m_szServiceId)
		delete [] m_szServiceId;

  //## end ClCCService::~ClCCService%881313195.body
}



//## Other Operations (implementation)
m4return_t ClCCService::GetSession (ClCCSession *&ao_pSession)
{
  //## begin ClCCService::GetSession%881685998.body preserve=yes

	ao_pSession = m_pSession;

	return M4_SUCCESS;
  //## end ClCCService::GetSession%881685998.body
}

m4return_t ClCCService::GetRequest (m4handle_t ai_RequestId, ClCCRequest *&ao_pRequest)
{
  //## begin ClCCService::GetRequest%881833854.body preserve=yes

	m_MutexDicRequests.Lock();
	map<m4int32_t, ClCCRequest*, less<m4int32_t> >::const_iterator iterador;
//	iterador = m_dicRequests.begin();
	iterador = m_dicRequests.find(ai_RequestId);

	// Si no es cierto, significa que no existe una 
	// request con ese identificador en el mapa.
	if (iterador == m_dicRequests.end())
	{	
		m_MutexDicRequests.Unlock();
		SETCODE(M4_ERR_GETREQ_REQNOTFOUND,ERRORLOG,"Request not found.");
		return M4_ERROR;
	}

	ao_pRequest = (*iterador).second;

	m_MutexDicRequests.Unlock();
	return M4_SUCCESS;

  //## end ClCCService::GetRequest%881833854.body
}

m4return_t ClCCService::AsyncRequest (M4DataStorage& ai_oInputData, m4handle_t& ao_hRID, m4bool_t ai_bLogInfo, m4uint16_t ai_uiRequestType, m4bool_t ai_bOwner)
{
  //## begin ClCCService::AsyncRequest%884258324.body preserve=yes
return M4_ERROR;
  //## end ClCCService::AsyncRequest%884258324.body
}

m4return_t ClCCService::SyncRequest (M4DataStorage& ai_oInputData, M4DataStorage& ao_oOutputData, m4handle_t& ao_hRID, m4bool_t ai_bLogInfo, ClLogSystem *ai_poLog, ClRequestDescriptor *ao_poUniqueId, m4uint32_t ai_uiTimeout, m4uint16_t ai_uiRequestType, m4bool_t ai_bOwner, ClTimeoutFunction ai_pTimeoutFunction)
{
  //## begin ClCCService::SyncRequest%884258325.body preserve=yes
	m4return_t iRet;
	ClCCRequest *pRequest;
	iRet=_PrepareRequest(ai_uiTimeout,pRequest,ao_hRID);
//cout << ClPrecisionTicker::GetTimeTick()<<"\t";
//	TheTagList.SetArg(M4_PROP_LAST_PDU_SENT,(m4double_t)pPDU ->GetPDUSize());
//	pSession->SetProps(&TheTagList);

	if(M4_ERROR!=iRet)
	{
		iRet=CreateRequestPDU(ai_oInputData, ai_uiRequestType, ai_bOwner,pRequest);
		if(M4_ERROR!=iRet)
		{
			/* Bug 0108256
			Se pone el nivel de log del cliente
			*/
			if( ai_poLog != NULL )
			{
				ClPDU *poPDU = pRequest->GetInputPDU() ;

				if( poPDU != NULL )
				{
					m4uint8_t iOverwriteMask = ai_poLog->GetOverwriteMask() ;

					poPDU->SetHasOverwriteMask( 1 ) ;
					poPDU->SetOverwriteMask( iOverwriteMask ) ;
				}
			}

			iRet=_GetAnswer(pRequest,&ao_oOutputData,"Syncrequest","Error arrived from server in SyncRequest",ai_pTimeoutFunction,NULL);

			if ( ao_poUniqueId)
			{
				pRequest -> GetUniqueRequestID ( ao_poUniqueId -> m_strDescriptor ) ;
			}

			if((iRet!=M4_ERROR) && (M4_ERROR == FullRemoveRequest (ao_hRID)))
			{
				SETCODE(M4_ERR_REQ_NOT_DELETED,ERRORLOG,"Couldn't remove SyncRequest");
				m4Trace(cerr << "Couldn't remove SyncRequest" <<endl);
			}
		}
	}


//cout << ClPrecisionTicker::GetTimeTick()<<endl;
	return iRet;

  //## end ClCCService::SyncRequest%884258325.body
}

m4return_t ClCCService::Receive (m4int32_t ai_RequestId, M4DataStorage& ao_oOutputData, m4int32_t ai_lTimeOut, m4int16_t ai_lReceiveFlag, ClLogSystem *ai_poLog)
{
  //## begin ClCCService::Receive%884258326.body preserve=yes
return M4_ERROR;
  //## end ClCCService::Receive%884258326.body
}

m4return_t ClCCService::CreateRequestPDU (M4DataStorage& ai_oInputData, m4uint16_t ai_uiRequestType, m4bool_t ai_bOwner, ClCCRequest *ai_pRequest)
{
  //## begin ClCCService::CreateRequestPDU%884258327.body preserve=yes

	ClCCConnection *pConnection=NULL;

	m_pSession->GetConnection(pConnection);
	
	m4return_t iRet;
	ClPDU* pPDU;
	iRet=_CreatePDU(ai_uiRequestType,ai_pRequest,pPDU);
	
	pPDU -> SetDataStorage(&ai_oInputData, ai_bOwner);

	// El servicio ha pasado a ser una cadena de caracteres
	( ( ClRequestPDU* ) pPDU ) -> SetServiceId(m_szServiceId , strlen (m_szServiceId) + 1 );

	return M4_SUCCESS;

  //## end ClCCService::CreateRequestPDU%884258327.body
}

m4return_t ClCCService::_PrepareRequest (m4uint32_t &aio_uiTimeout, ClCCRequest * &ao_pRequest, m4handle_t& ao_hRID)
{
  //## begin ClCCService::_PrepareRequest%947519641.body preserve=yes
	m4return_t iRet=M4_SUCCESS;
// Register Request in the monitor
	ClCCMonitor *theMonitor;
	ClCCMonitor::GetClientMonitor(theMonitor);

	ao_pRequest = new ClCCRequest(this);
	ao_pRequest->m_uiTimeout=aio_uiTimeout;

		// Se marca en la request el servicio al que 
	// corresponde.
	ao_pRequest -> SetServiceId( m_szServiceId);

	if (NULL != m_pSession->m_poDebugFile) 
		ao_pRequest->m_poDebugFile=m_pSession->m_poDebugFile;

	iRet=theMonitor->GetRequestNumber(ao_hRID );

	ao_pRequest->SetRequestId(ao_hRID );
	m_dicRequests[ao_hRID] = ao_pRequest;

	theMonitor->AddRequest( ao_hRID, ao_pRequest);
	
	return M4_SUCCESS;
  //## end ClCCService::_PrepareRequest%947519641.body
}

m4pchar_t ClCCService::GetService () const
{
  //## begin ClCCService::GetService%884854699.body preserve=yes
	return m_szServiceId;
  //## end ClCCService::GetService%884854699.body
}

m4return_t ClCCService::RemoveRequest (m4handle_t ai_RequestId)
{
  //## begin ClCCService::RemoveRequest%899225540.body preserve=yes

	// Antes de nada compruebo que el diccionario 
	// contiene una request con ese identificador.
	// Si no, devuelvo error.
	ClCCRequest* pRequest;

	eRequestStatus iStatus ;
	ClCCMonitor *theMonitor;
	m4return_t iRet;
	
	ClCCMonitor::GetClientMonitor(theMonitor);


	iRet=GetRequest(ai_RequestId, pRequest);
	if ((pRequest == NULL) || (M4_ERROR== iRet))
		return M4_ERROR;

	//Decide que hacer dependiendo del estado de la request
	
	iStatus = pRequest -> GetState () ;
	while(iStatus==SENDING)
		m4sleep(1);

	m4int16_t ret;
	iRet=M4_SUCCESS;

	m_MutexDicRequests.Lock();
	ret = m_dicRequests.erase(ai_RequestId);
	m_MutexDicRequests.Unlock();
	if( ret != 1)
	{
		m4Trace(cerr <<"Request not deleted from dict" <<endl);
		SETCODE(M4_ERR_REQ_NOT_DEL_FROM_DICT,ERRORLOG,"Request not deleted from Service dictionary");
		iRet=M4_ERROR;
	}

	delete pRequest;

	return iRet;

  //## end ClCCService::RemoveRequest%899225540.body
}

m4return_t ClCCService::CancelRequest (m4handle_t ai_RequestId, m4uint32_t ai_uiTimeout, m4bool_t ai_bSeverity)
{
  //## begin ClCCService::CancelRequest%884854700.body preserve=yes
	m4return_t iReturn;
	m4uint32_t iStatus  ;
	ClCCRequest * pRequest2Cancel = NULL ;

	iReturn= GetRequest(ai_RequestId,pRequest2Cancel);
	if ((pRequest2Cancel == NULL) || (M4_ERROR== iReturn))
		return M4_ERROR;

	iStatus = pRequest2Cancel -> GetState () ;

	switch ( iStatus )
	{
		case SENDING :
		case BUILDING :
		case WAITING_FOR_REPLY :
		{
			pRequest2Cancel->SetState(CANCELING);

			ClCCConnection *pConnection;
			m_pSession->GetConnection(pConnection);
			if(!pConnection)
			{
				m4Trace(cerr<< "NULL Connection."<<endl);
				return M4_ERROR;
			}
			if(pConnection->GetConnType())
			{
				eConnectionStatus elStatus=pConnection->GetConnStatus();

				pConnection->SetConnStatus(M4_CONNECTION_CANCELING);
				ClTSAP *pTSAP;
				pConnection->GetTSAP(pTSAP);
				if(pTSAP)
					pTSAP->Close(); // con esto el receiver saldra de la mascara y borrara la TSAP
				while(pConnection->GetConnStatus()==M4_CONNECTION_CANCELING)
					m4sleep(1);

				pConnection->SetConnStatus(elStatus);
			}
// Ahora mando la peticion de cancelacion			
		
			ClCCRequest *pRequest;
			m4handle_t hRID ;
			m4return_t iRet=_PrepareRequest(ai_uiTimeout,pRequest,hRID);

			if(M4_ERROR!=iRet)
			{
				iRet=CreateCancelRequestPDU(ai_RequestId , pRequest);
				if(M4_ERROR!=iRet)
				{
					iRet=_GetAnswer(pRequest,NULL,"CancelRequest","Error arrived from server in CancelRequest",NULL,NULL);
					if((iRet!=M4_ERROR) && (M4_ERROR == FullRemoveRequest (hRID)))
					{
						m4Trace(cerr << "Couldn't remove ConnectRequest from monitor" <<endl);
						SETCODE(M4_ERR_REQ_NOT_DEL_FROM_DICT,ERRORLOG,"Couldn't remove ConnectRequest from monitor");
					}
				}
			}
			return iRet;
		}
		break ;

		case REPLY_ARRIVED :
			m4Trace(cerr<<"Request REPLY_ARRIVED"<<endl);
			return M4_ERROR ;
			break;

		case ERROR_ARRIVED :
			m4Trace(cerr<<"Request ERROR_ARRIVED "<<endl);
			return M4_ERROR ;
			break;

		case CANCELED :
			m4Trace(cerr<<"Request CANCELED"<<endl);
			return M4_ERROR ;
			break;

		case CANCELING :
			m4Trace(cerr<<"Request CANCELING"<<endl);
			return M4_ERROR ;
			break;

		default :
			m4Trace(cerr<<"Default"<<endl);
			return M4_ERROR ;
			break;
	} 

  //## end ClCCService::CancelRequest%884854700.body
}

m4return_t ClCCService::CancelAllRequests ()
{
  //## begin ClCCService::CancelAllRequests%884854702.body preserve=yes
	
	// Recorre la lista de requests pendientes y las 
	// destruye. Antes las borra del diccionario del 
	// monitor.

	m4return_t iReturn;
	
	typedef map<m4int32_t, ClCCRequest*, less<m4int32_t> >::iterator CI;
	CI p = m_dicRequests.begin();
	ClCCRequest *pRequest; 
	
	m4int32_t requestId;
	
	m4bool_t  bDeleteIt;
	
	for (; p != m_dicRequests.end();)
	{
		requestId = (*p).first;
		pRequest = (*p).second;
		eRequestStatus RequestState=pRequest->GetState();
		
		bDeleteIt=M4_TRUE;
		
		switch (RequestState)
		{
		case TIMEOUT:
			m4TraceLevel(2,cerr << "Request abandonada en TIMEOUT"<<endl);
			break;
		case BROKEN_CONNECTION:
			{	
				ClCondition	*pCond = pRequest -> GetCondition();
				if (NULL != pCond)
				{
					m4int32_t contador=0;
					pCond ->Signal();
					ClCCMonitor *elMonitor;
					ClCCMonitor::GetClientMonitor(elMonitor);
					ClCCRequest *pTempRequest; 
					while((elMonitor->GetRequest(requestId,pTempRequest)==M4_SUCCESS)&&(contador<1))
					{
						contador++;
						m4sleep(1);
					}
					if(contador<1)
						bDeleteIt=M4_FALSE;
				}	
				else
				{m4TraceLevel(2,cerr << "Request abandonada"<<endl);}
			}	
			break;
		default:
			m4Trace(cerr<< "Canceling request in " << RequestState<< " state"<<endl);
			break;
		}
		if(bDeleteIt)
		{
			iReturn=FullRemoveRequest (requestId);
			if (M4_ERROR == iReturn)
			{
				SETCODE(M4_ERR_REQ_NOT_DELETED,ERRORLOG,"Couldn't remove Request in CancelAllRequest");
				m4Trace(cerr << "Couldn't remove CancelRequest " <<endl);
			}
		}
		p=m_dicRequests.begin();
	}


return M4_SUCCESS;

  //## end ClCCService::CancelAllRequests%884854702.body
}

m4return_t ClCCService::CreateCancelRequestPDU (m4handle_t ai_hToBeCancelID, ClCCRequest *ai_pRequest)
{
  //## begin ClCCService::CreateCancelRequestPDU%898788004.body preserve=yes
	ClPDU *pPDU;
	m4return_t iRet;

	iRet=_CreatePDU(M4_ID_PDU_NEWCANCEL,ai_pRequest,pPDU);
	if(M4_SUCCESS==iRet)
	{
		ClNewCancelPDU* pCPDU=(ClNewCancelPDU*) pPDU;

		pCPDU -> SetCancelRequestId(ai_hToBeCancelID);
	}

	return iRet;
  //## end ClCCService::CreateCancelRequestPDU%898788004.body
}

m4return_t ClCCService::NotifyAllRequest (eRequestStatus ai_eIncidence)
{
  //## begin ClCCService::NotifyAllRequest%923902840.body preserve=yes
	ClCCMonitor *elMonitor;
	ClCCMonitor::GetClientMonitor(elMonitor);

	typedef map<m4int32_t, ClCCRequest*, less<m4int32_t> >::iterator CI;
m_MutexDicRequests.Lock();
	CI p = m_dicRequests.begin();
	ClCCRequest *pRequest; 
	m4int32_t requestId;

	for (; p != m_dicRequests.end();p++)
	{
		requestId = (*p).first;
		pRequest = (*p).second;
		if(pRequest->GetState()==SENDING)
		{
			pRequest->SetState(CANCELING);
		}
		else
		{
			pRequest->SetState(ai_eIncidence);
			ClCondition	*pCond = pRequest -> GetCondition();
			if (NULL != pCond)
				pCond ->Signal();
		}
	}
m_MutexDicRequests.Unlock();
	return M4_SUCCESS;
  //## end ClCCService::NotifyAllRequest%923902840.body
}

void ClCCService::GetDataStorage (ClPDU*  ai_poPDU, M4DataStorage& ao_oData)
{
  //## begin ClCCService::GetDataStorage%927714163.body preserve=yes
	m4return_t res;
	m4uint32_t size;
	m4pchar_t  pBuffer,pBuffersal;
	m4bool_t   encriptado = M4_FALSE;

	ao_oData.Clear();

	M4DataStorage* pData = ai_poPDU -> GetDataStorage(M4_TRUE);
	if(NULL!=pData )
	{
		pData -> InitializeIteration();

		m4pchar_t bufferData = 0;

		ClDataUnitInterface *pDI;

		while( (res = pData -> GetNext(pDI)) != M4_ERROR )
		{
			switch(pDI->GetType())
			{
			case M4_DATA_UNIT:

				ClDataUnit		*poMemDataUnit;

				poMemDataUnit = (ClDataUnit *)pDI;
				size=poMemDataUnit->GetSize();
				pBuffer=poMemDataUnit->GetBuffer();
// Vamos a mirar si son eventos o son log!!
				if(size>=10)
				{
					if(!strncmp(pBuffer,"@@EVENT@@",9))
					{
						m4Trace(cout<< "EVENTOS!!!!!"<<endl);
						IEventHandler *theEH=ClLogBasedEventHandler::GetEHInstance();
						if(theEH)
							((ClLogBasedEventHandler*)theEH)->Deserialize(pBuffer+9,size-9);
					}
					else
						if(!strncmp(pBuffer,"@@LOGSY@@",9))
						{
							m4Trace(cout<< "LOGS!!!!!"<<endl);
							SETSERIALCODESTRING ( pBuffer+9 );	
						}
						else
						{
							ao_oData.GetNewDataUnit(poMemDataUnit);
							poMemDataUnit->SetSize(size);
							pBuffersal=poMemDataUnit->GetBuffer();
							memcpy(pBuffersal,pBuffer,size);
						}
				}
				else
				{
					ao_oData.GetNewDataUnit(poMemDataUnit);
					poMemDataUnit->SetSize(size);
					pBuffersal=poMemDataUnit->GetBuffer();
					memcpy(pBuffersal,pBuffer,size);
				}
// Fin de evento o de log
			break;
			case M4_FILE_UNIT:
				ClFileUnit	*poFileUnit;
				m4pchar_t	pszFilename;

				poFileUnit = (ClFileUnit *)pDI;
				pszFilename = poFileUnit->GetFileName();

				ao_oData.GetNewFileUnit(poFileUnit);
				poFileUnit->SetFile(pszFilename);
				poFileUnit->SetSerialState(M4_TRUE);
				
			break;

			case M4_CHECK_UNIT:
				// Completitud. No se contempla recibir una check unit como respuesta a una petición.
				break;

			default:
				break;
						
			}
		}
		ao_oData.InitializeIteration();
		ao_oData.m_bFinishSerialization=M4_TRUE;
	}
//cout << ClPrecisionTicker::GetTimeTick()<<"\t";
  //## end ClCCService::GetDataStorage%927714163.body
}

m4return_t ClCCService::FullRemoveRequest (m4handle_t ai_RequestId)
{
  //## begin ClCCService::FullRemoveRequest%947582395.body preserve=yes

	// Antes de nada compruebo que el diccionario 
	// contiene una request con ese identificador.
	// Si no, devuelvo error.
	ClCCRequest* pRequest;
	eRequestStatus iStatus;
	m4return_t iRet=M4_SUCCESS;;

	iRet=GetRequest(ai_RequestId, pRequest);
	if ((pRequest == NULL) || (M4_ERROR== iRet))
	{
		iRet= M4_ERROR;
	}
	else
	{
		//Decide que hacer dependiendo del estado de la request
		m4bool_t bRetry=M4_TRUE;
		m4int_t iRetries=5;
		while(bRetry)
		{
			iStatus = pRequest -> GetState () ;

			switch ( iStatus )
			{
			case ERROR_ARRIVED :
			case REPLY_ARRIVED :
			case WAITING_FOR_REPLY:
				bRetry=M4_FALSE;
				break;
			case SENDING :
				m4sleep(1);
				iRetries--;
				if(iRetries<0)
					bRetry=M4_FALSE;
				break;
			default :
				m4TraceLevel(2,cout<< "State:"<< iStatus <<endl);
				bRetry=M4_FALSE;
				break;
			} 
		}

		m4int16_t ret;
		m_MutexDicRequests.Lock();
		ret = m_dicRequests.erase(ai_RequestId);
		m_MutexDicRequests.Unlock();

		if( ret != 1)
		{
			m4Trace(cerr <<"Request not deleted from dict" <<endl);
			SETCODE(M4_ERR_REQ_NOT_DEL_FROM_DICT,ERRORLOG,"Request not deleted from Service dictionary");
			iRet=M4_ERROR;
		}

		if(iStatus!=WAITING_FOR_REPLY)
			delete pRequest;

		ClCCMonitor *theMonitor;	
		ClCCMonitor::GetClientMonitor(theMonitor);

		if(M4_ERROR == theMonitor->RemoveRequest(ai_RequestId))
		{
			m4Trace(cerr << "Couldn't Remove Request from monitor" <<endl);
			SETCODE(M4_ERR_REQ_NOT_DEL_FROM_DICT,ERRORLOG,"Couldn't remove Request from monitor");
		}
	}

	return iRet;
  //## end ClCCService::FullRemoveRequest%947582395.body
}

m4return_t ClCCService::_GetAnswer (ClCCRequest *ai_poRequest, M4DataStorage *ai_poOutputDataStorage, m4pchar_t ai_szRequestName, m4pchar_t ai_szDefaultError, ClTimeoutFunction ai_pTimeoutFunction, ClTagList *ao_oTag)
{
  //## begin ClCCService::_GetAnswer%947582396.body preserve=yes
	m4return_t iRet=M4_ERROR;
	ClPDU *pOutputPDU=NULL;
	m4bool_t bNoAnswerData=M4_FALSE;
	m4bool_t bMustReSend = M4_TRUE;
	
	m4uint8_t opID = ai_poRequest->GetInputPDU()->GetIdPDU() & 0x00ff;

	m_pSession->PDU_Lock(opID);

	if(!ai_szRequestName)
		ai_szRequestName="Request";

	if(!ai_szDefaultError)
		ai_szDefaultError="Error from server.";

	// Bg 136877
	while( bMustReSend == M4_TRUE )
	{
		// Bg 123545. Inicialización.
		ai_poRequest->m_bRetrying = M4_FALSE;

		iRet=_SendData(ai_poRequest);
		if(M4_ERROR==iRet)
		{
			m_pSession->PDU_Unlock(opID);
			return iRet;
		}
		while(SENDING==ai_poRequest->GetState())
		{
			Sleep(50);
		}
		m4bool_t bTry=M4_TRUE;
		
		eRequestStatus requestState;

	#ifdef TEST_CLOSE_TSAP
		
		while((requestState = ai_poRequest -> GetState())==SENDING )
		{
			m4Trace(cerr<<"."<<flush);
		}
		cerr<<"Close TSAP!"<<endl;
		iRet= M4_ERROR;
		bTry=M4_FALSE;
	#endif // TEST_CLOSE_TSAP

		
		while(bTry)
		{
			iRet=_GetAnswerData(ai_poRequest, bMustReSend);

			if( bMustReSend == M4_TRUE)
			{
				break;
			}

		//	if(M4_ERROR==iRet)
		//		return iRet;

			m4uint_t uiControlTimeout=ai_poRequest->m_uiTimeout;
			requestState = ai_poRequest -> GetState();
			switch(requestState)
			{
			case REPLY_ARRIVED:
				bTry=M4_FALSE;
				iRet=M4_SUCCESS;
				break;
			case ERROR_ARRIVED:
				bTry=M4_FALSE;
				iRet=M4_ERROR;
				break;
			case BROKEN_CONNECTION:
				m4Trace(cerr <<"Detectado Corte de la comunicacion con el servidor"<<endl);
				SETCODEF(M4_ERR_CONNECTION_ERROR,ERRORLOG,"Connection interrupted in %s.",ai_szRequestName);
				iRet=M4_ERROR;
				bTry=M4_FALSE;
				bNoAnswerData=M4_TRUE;
				break;
			case NO_SERVER:
				m4Trace(cerr <<"NO Server."<<endl);
				SETCODE(M4_ERR_CONNECTION_ERROR,ERRORLOG,"Couldn't connect to Server");
				iRet=M4_ERROR;
				bTry=M4_FALSE;
				bNoAnswerData=M4_TRUE;
				break;
			case HTTP_ERROR:
				m4Trace(cerr <<"HTTP error  in "<<ai_szRequestName<<endl);
				SETCODEF(M4_ERR_CONNECTION_ERROR,ERRORLOG,"HTTP error in %s.",ai_szRequestName);
				iRet=M4_ERROR;
				bTry=M4_FALSE;
				bNoAnswerData=M4_TRUE;
				break;
			case HTTP_TIMEOUT:
				m4Trace(cerr <<"HTTP Timeout in "<< ai_szRequestName<<endl);
				SETCODEF(M4_ERR_CONNECTION_ERROR,ERRORLOG,"HTTP Timeout in %s.",ai_szRequestName);
				iRet=M4_ERROR;
				bTry=M4_FALSE;
				bNoAnswerData=M4_TRUE;
				break;
			default:
				{
					m4Trace(cerr << "\nTIMEOUT in "<<ai_szRequestName<<"\n" <<endl);
					m4DebugExecute(if(!uiControlTimeout) cerr<<"Timeout era infinito!!!!!!"<<endl);
					m4bool_t bExit=M4_TRUE;
					if (ai_pTimeoutFunction)
					{
						m4int32_t retTimeoutFunction=(*ai_pTimeoutFunction)(uiControlTimeout);
						switch(retTimeoutFunction)
						{
						case -10: // Hay que enviar una cancelacion
							
							// Bg 123545.
							ai_poRequest->m_bRetrying = M4_FALSE;

							CancelRequest(ai_poRequest->GetRequestId() ,100000,M4_TRUE);
							bExit=M4_TRUE;
							break;
						case -1:
							break;
						case 0:
							uiControlTimeout=-1;
							bExit=M4_FALSE;

							// Bg 123545. Avisamos a la conexion wininet de que es un retry.
							ai_poRequest->m_bRetrying = M4_TRUE;

							break;
						default:
							if(retTimeoutFunction>0)
							{
								uiControlTimeout=retTimeoutFunction; //Supongo que me lo dan en milesegundos
								bExit=M4_FALSE;
							}

							// Bg 123545. Avisamos a la conexion wininet de que es un retry.
							ai_poRequest->m_bRetrying = M4_TRUE;
						}
		//Esto es necesario para que tenga efecto en la petición en curso 
		//el cambio del timeout en la ventana modal.
						ai_poRequest->m_uiTimeout = uiControlTimeout ;

					}
					if(bExit)
					{
						ai_poRequest->UnsetCondition();
					//	iRet=M4_ERROR;
						if(( ai_szRequestName) && (!strcmp(ai_szRequestName,"AuthenticateRequest")))
							iRet=M4_ERROR;
						else
							iRet=M4_WARNING;
						bTry=M4_FALSE;
						bNoAnswerData=M4_TRUE;
					}
				}
				break;
			}
		}
	}

	if(!bNoAnswerData)
	{
		pOutputPDU =ai_poRequest -> GetOutputPDU();
		if(pOutputPDU)
		{
			if ((NULL != ao_oTag) && (!ai_poOutputDataStorage))
			{
				M4DataStorage *pOutputData = ((ClAckPDU *)pOutputPDU) -> GetDataStorage();
				if(pOutputData)
				{
					m4return_t iReturn=ao_oTag->DataStorage2TagList(pOutputData);
					m4DebugExecute(if(iReturn==M4_ERROR) cerr<<"Error en DataStorage2Taglist"<<endl);
				}
			}
			
			if(ai_poOutputDataStorage && (iRet==M4_SUCCESS))
				GetDataStorage(pOutputPDU, *ai_poOutputDataStorage);
			m4bool_t bNoLog=M4_TRUE;

			if (pOutputPDU -> LogInfo () ) 
			{
				m4pchar_t szLogInfo ;
				m4uint32_t iLogSize ;

				if (M4_ERROR!=pOutputPDU  -> GetLogInfo( szLogInfo, iLogSize) && szLogInfo)
				{
					SETSERIALCODESTRING ( szLogInfo ) ;
					bNoLog=M4_FALSE;
				}
			}
			if(M4_ERROR==iRet)
			{
				if(bNoLog)	
					SETCODE(M4_ERR_ERROR_FROM_SERVER,ERRORLOG,ai_szDefaultError);
				ClCCConnection *pConnection;
				m_pSession->GetConnection(pConnection);
				if((pConnection) && ( pConnection -> m_oProtocol.GetVersion()==M4_PDU_40_VERSION))
					iRet=(m4return_t)((ClNewErrorPDU *)pOutputPDU)->GetErrorCode();
				else
					iRet=(m4return_t)((ClErrorPDU *)pOutputPDU)->GetErrorCode();
				if(M4_SUCCESS==iRet)
					iRet=M4_ERROR;
			}
			else
			{
				if(pOutputPDU->GetIdPDU()==M4_ID_PDU_ACK)
					iRet=((ClAckPDU *)pOutputPDU) ->GetWhy();
			}
		}
		else
		{
			m4Trace(cerr<<"NULL PDU!!!"<<endl);
			SETCODEF(M4_ERR_NULL_PDU_FROM_QUEUE,ERRORLOG,"NULL answer PDU for %s\n",ai_szRequestName);
			iRet=M4_ERROR;
		}
	}
	m_pSession->PDU_Unlock(opID);
	return iRet;
  //## end ClCCService::_GetAnswer%947582396.body
}

m4return_t ClCCService::_CreatePDU (m4int16_t ai_iPDUType, ClCCRequest *ai_poRequest, ClPDU * &ao_pPDU)
{
  //## begin ClCCService::_CreatePDU%948190723.body preserve=yes
	m4return_t iRet=M4_SUCCESS;

	ClCCConnection *pConnection;
	m_pSession->GetConnection(pConnection);


	ao_pPDU=pConnection -> m_oProtocol.GetPDU(ai_iPDUType);

    // le pongo a la PDU el número de sesión en el appserver.
    m4string_t unused;
    m4string_t serverSessionId;
    m4uint32_t uiObjectId = 0;
    m_pSession->GetServerSessionParams(unused, unused, serverSessionId);
    uiObjectId = atol(serverSessionId.c_str());
    ao_pPDU->SetObjectId(uiObjectId);

    // le pongo a la PDU el tipo de compresión.
    m4uint32_t uiCompressionType = pConnection->GetCompressionType();
    ao_pPDU->SetCompressionType(uiCompressionType);

	ai_poRequest->SetPDUInput(ao_pPDU);	
	
//	if (GET_REMOTE_LOG())
//	Se ha activado por defecto 10-3-99
	if(M4_TRUE)
		ao_pPDU  -> EnableLogInfo();

	ao_pPDU -> SetRequestId(ai_poRequest->GetRequestId());

	return iRet;
  //## end ClCCService::_CreatePDU%948190723.body
}

m4return_t ClCCService::_GetAnswerData (ClCCRequest *ai_pRequest, m4bool_t &ao_bReSend)
{
  //## begin ClCCService::_GetAnswerData%950783985.body preserve=yes
	return m_pSession->_GetAnswerData(ai_pRequest, ao_bReSend);
  //## end ClCCService::_GetAnswerData%950783985.body
}

m4return_t ClCCService::_SendData (ClCCRequest *ai_pRequest)
{
  //## begin ClCCService::_SendData%950783986.body preserve=yes
	return m_pSession->_SendData(ai_pRequest);
  //## end ClCCService::_SendData%950783986.body
}

//## begin module%34951237024C.epilog preserve=yes
//## end module%34951237024C.epilog
