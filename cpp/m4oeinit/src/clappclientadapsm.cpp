

class IObjDataCache;


#include "clappclientadapsm.hpp"
#include "clm4objserviceclientsm.hpp"
#include "clm4objserviceserversm.hpp"
#include "exechannel.hpp"
#include "m4mdadaptor.hpp"
#include "genproxy.hpp"
#include "clfactm4objservice.hpp"
#include "dm.hpp"
#include "exeproxy.hpp"
#include "channel.hpp"
#include "chmngauthent.hpp"
#include "cllgadap.hpp"
#include "certmngr.hpp"
#include "sessioncontext.hpp"
#include "clseschnexport.hpp"
#include "datastorages.hpp"



ClAppClientAdapSM::ClAppClientAdapSM() 
{
	m_hSessionId = 0;
	m_bOpenOLTP = M4_FALSE;
	m_bOpenProxy = M4_FALSE;
	m_bOpenMD = M4_FALSE;
	m_pObjServiceSMClient = 0;
	m_pCacheCMCR = 0;
	m_pCacheCSCR = 0;
	m_pCachePres = 0;
	m_pCacheMap = 0;
	m_pCacheData = 0;
	m_pObjServiceSMServer = 0;
	m_bInit = M4_FALSE;
}

ClAppClientAdapSM::~ClAppClientAdapSM() 
{
	End();
}

m4return_t ClAppClientAdapSM::Init(ClM4ObjServiceClientSM* ai_ObjServiceSM, ClCache *ai_pCacheCMCR, ClCache *ai_pCacheCSCR, ClCache *ai_pCachePres, ClCache *ai_pCacheMap, ClCache *ai_pCacheData)
{
	m4return_t iResult = M4_SUCCESS;

	if (!ai_ObjServiceSM)
		return M4_ERROR;

	m_pObjServiceSMClient = ai_ObjServiceSM;

	if (!ai_pCacheCMCR)
		return M4_ERROR;

	m_pCacheCMCR = ai_pCacheCMCR;

	if (!ai_pCacheCSCR)
		return M4_ERROR;

	m_pCacheCSCR = ai_pCacheCSCR;

	if (!ai_pCachePres)
		return M4_ERROR;

	m_pCachePres = ai_pCachePres;

	if (!ai_pCacheMap)
		return M4_ERROR;

	m_pCacheMap = ai_pCacheMap;

	if (!ai_pCacheData)
		return M4_ERROR;

	m_pCacheData = ai_pCacheData;

	m_bInit = M4_TRUE;

	return M4_SUCCESS;
}

m4return_t ClAppClientAdapSM::End()
{
	//CloseServices();

	m_pObjServiceSMClient = 0;
	m_pObjServiceSMServer = 0;
	m_pCacheCMCR = 0;
	m_pCacheCSCR = 0;
	m_pCachePres = 0;
	m_pCacheMap  = 0;
	m_pCacheData = 0;

	m_bInit = M4_FALSE;

	return M4_SUCCESS;
}

m4return_t ClAppClientAdapSM::CloseServices() 
{
	if (m_hSessionId == 0) //Tenemos que tener abierta una sesion
		return M4_ERROR;

	ClFactM4ObjService::DestroyObjService(m_pObjServiceSMServer);
	m_pObjServiceSMServer = 0;

	return M4_SUCCESS;
}

m4return_t	ClAppClientAdapSM::CloseSession( void ) {
	if (m_hSessionId == 0) //Tenemos que tener abierta una sesion
		return M4_ERROR;

	//Cerramos todos los servicios
	CloseServices();

	//Cerramos la sesión
	m_hSessionId = 0;
	
	//Borrar el contenido de los servicios¿?¿?

	return M4_SUCCESS;
}

m4return_t ClAppClientAdapSM::OpenService (m4pchar_t ai_szServiceId, m4handle_t &ao_hContext) 
{
	//m4return_t iResult;

	if (m_hSessionId == 0) //Tenemos que tener abierta una sesion
		return NULL;

	ao_hContext = M4_CLIENTAPADPTSM_SERVICE_NULL;

	if (strcmpi(ai_szServiceId,M4CL_EXECUTION_SERVICE) == 0)
	{
		//OLTP
		if (!m_pObjServiceSMServer)
			m_pObjServiceSMServer = ClFactM4ObjService::CreateObjService(m_pCacheCMCR,m_pCacheCSCR,m_pCachePres,m_pCacheMap,m_pCacheData, M4_TRUE);

		if (m_pObjServiceSMServer == NULL) {
			return M4_ERROR;
		}

		if (m_pObjServiceSMServer)
			ao_hContext = M4_CLIENTAPADPTSM_SERVICE_CONTEXT_OLTP;
	}
	else if (strcmpi(ai_szServiceId, M4CL_PROXY_SERVICE) == 0)
	{
		//PROXY
		ao_hContext = M4_CLIENTAPADPTSM_SERVICE_CONTEXT_PROXY;
	}
	else if (strcmpi(ai_szServiceId, MD_SERVICE_ID) == 0)
	{
		//MD
		if (!m_pObjServiceSMServer)
			m_pObjServiceSMServer = ClFactM4ObjService::CreateObjService(m_pCacheCMCR,m_pCacheCSCR,m_pCachePres,m_pCacheMap,m_pCacheData, M4_TRUE);

		if (m_pObjServiceSMServer == NULL) {
			return M4_ERROR;
		}

		if (m_pObjServiceSMServer)
			ao_hContext = M4_CLIENTAPADPTSM_SERVICE_CONTEXT_MD;
	}
	else
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
}

m4return_t ClAppClientAdapSM::SyncRequest (m4handle_t ai_hServiceContext, m4uint32_t ai_uiStateFlags, m4uint32_t ai_uiOperFlags, M4DataStorage *ai_pInDataStore, M4DataStorage *&ao_pOutDataStore, m4handle_t &ao_hRequestId, m4uint32_t ai_uiTimeout /*= 00*/, m4bool_t ai_bLogInfo /*= M4_FALSE*/, ClLogSystem *ai_poLog /*=NULL*/, ClTimeoutFunction ai_pTimeoutFunction /*=NULL*/) 
{
	m4return_t iResult = M4_ERROR;

	if (0 == m_hSessionId) //Tenemos que tener abierta una sesion
		return M4_ERROR;

	ClChannelManager *pChManagerCliente = m_pObjServiceSMClient->GetpChannelManager();
	ClChannelManager *pChManagerServer = m_pObjServiceSMServer->GetpChannelManager();
	//Obtenemos el canal sesion de la parte cliente
	ClChannel* pSessionChannel = pChManagerCliente->GetSessionChannel();
// ADD joseavm	13-11-2000
	if(NULL!=ai_pInDataStore)
		ai_pInDataStore->m_bFinishSerialization=M4_TRUE;
// END ADD joseavm

	switch (ai_hServiceContext)
	{
		case M4_CLIENTAPADPTSM_SERVICE_CONTEXT_OLTP:

		{
			// jcr. Llamo a la nueva función que hemos hecho
			//Cambiamos el channel manager del canal sessión. Vamos a pasar a la parte server
			if( pSessionChannel != NULL ) {
				pSessionChannel->_TeleTransportTo( pChManagerServer ) ;
			}

			// En emulación se tiene una cache de autos con sesión 1
			IObjDataCache	*poObjDataCache = pChManagerServer->GetObjDataCache( "1" ) ;

			//Ejecuta

			// Se desactiva la auditoría cliente y se activa la server si lo está la cliente
			m4bool_t	bActive = pChManagerCliente->UserActionsAudit.SwitchOff( M4_FALSE ) ;

			if( bActive == M4_TRUE )
			{
				pChManagerServer->UserActionsAudit.SwitchOn() ;
			}

			if( ai_pInDataStore != NULL )
			{
				/* Bug 0195351
				Se emula la serialización de blob
				*/
				ai_pInDataStore->CloneFiles( "~server" ) ;
			}

			iResult = ExecChannel(pChManagerServer,
								  ai_pInDataStore, ao_pOutDataStore,
								  NULL, NULL, NULL,
								  m_pObjServiceSMServer->GetpConnProvider(),
								  poObjDataCache);

			if( ao_pOutDataStore != NULL )
			{
				/* Bug 0195351
				Se emula la serialización de blob
				*/
				ao_pOutDataStore->CloneFiles( "~client" ) ;
			}

			if( bActive == M4_TRUE )
			{
				// Se desactiva la auditoría server y se activa la cliente si lo estaba
				pChManagerServer->UserActionsAudit.SwitchOff( M4_TRUE ) ;
				pChManagerCliente->UserActionsAudit.SwitchOn() ;
			}

			break;
		}

		case M4_CLIENTAPADPTSM_SERVICE_CONTEXT_PROXY:
		{
			
			// En proxy la llamada al _TeleTransportTo() se hace desde el espacio proxy,
			// al llamar al SetSessionChannel(). Esto es porque aquí todavía no sabemos
			// a que channel manager tenemos que enganchar el canal sesión.

			//Ejecuta
			iResult = ExecForProxy(ai_pInDataStore, ao_pOutDataStore,
								   m_pObjServiceSMClient->GetpServerProxyManagerEmu(),
								   pSessionChannel, 1, "Emulation",
								   0, 0, 0);
			break;
		}

		case M4_CLIENTAPADPTSM_SERVICE_CONTEXT_MD:
		{
			// jcr. Llamo a la nueva función que hemos hecho
			//Cambiamos el channel manager del canal sessión. Vamos a pasar a la parte server
			if( pSessionChannel != NULL ) {
				pSessionChannel->_TeleTransportTo( pChManagerServer ) ;
			}

			//Ejecuta
			// Se desactiva la auditoría cliente y se activa la server si lo está la cliente
			m4bool_t	bActive = pChManagerCliente->UserActionsAudit.SwitchOff( M4_FALSE ) ;

			if( bActive == M4_TRUE )
			{
				pChManagerServer->UserActionsAudit.SwitchOn() ;
			}

			iResult = ExecMetaData((ClCMCRFactory*)m_pObjServiceSMServer->GetpCMCRFactory(),
								   (ClPresentationFactory*)m_pObjServiceSMServer->GetpPresFactory(),
								   m_pObjServiceSMServer->GetpCacheDirectory(),
								   pChManagerServer,
								   ai_pInDataStore, ao_pOutDataStore,
								   NULL,
								   NULL);

			if( bActive == M4_TRUE )
			{
				// Se desactiva la auditoría server y se activa la cliente si lo estaba
				pChManagerServer->UserActionsAudit.SwitchOff( M4_TRUE ) ;
				pChManagerCliente->UserActionsAudit.SwitchOn() ;
			}

			break;
		}

		default:
			iResult =  M4_ERROR;
			break;
	}


// ADD joseavm	13-11-2000
	if(NULL!=ao_pOutDataStore)
		ao_pOutDataStore->m_bFinishSerialization=M4_TRUE;
// END ADD joseavm

// jcr. Llamo a la nueva función que hemos hecho
	//Cambiamos el channelmanager del canal session. Volvemos a la parte cliente
	if( pSessionChannel != NULL )
	{
		pSessionChannel->_TeleTransportTo( pChManagerCliente ) ;
	}

	return iResult;
}


m4return_t ClAppClientAdapSM::SyncRequest (m4pchar_t ai_szServiceId, m4uint32_t ai_uiStateFlags, m4uint32_t ai_uiOperFlags, M4DataStorage *ai_pInDataStore, M4DataStorage *&ao_pOutDataStore, m4handle_t &ao_hRequestId, m4uint32_t ai_uiTimeout, m4bool_t ai_bLogInfo, ClLogSystem *ai_poLog, ClTimeoutFunction ai_pTimeoutFunction) 
{
	if (0 == m_hSessionId) //Tenemos que tener abierta una sesion
		return M4_ERROR;

	m4handle_t hServiceContext;
	
	if (OpenService(ai_szServiceId, hServiceContext) == M4_SUCCESS) 
	{
		return( SyncRequest (hServiceContext, ai_uiStateFlags, ai_uiOperFlags, ai_pInDataStore, ao_pOutDataStore, ao_hRequestId, ai_uiTimeout, ai_bLogInfo, ai_poLog, ai_pTimeoutFunction));
	}

	return M4_ERROR;
}

m4return_t ClAppClientAdapSM::AsyncRequest(m4handle_t ai_hServiceContext, m4uint32_t ai_uiStateFlags, m4uint32_t ai_uiOperFlags, M4DataStorage *ai_pInDataStore, m4handle_t &ao_hRequestId, m4bool_t ai_bLogInfo /*=M4_FALSE*/){
	return M4_ERROR;	// No tiene sentido en SM
}


m4return_t ClAppClientAdapSM::AsyncRequest(m4pchar_t ai_szServiceId, m4uint32_t ai_uiStateFlags, m4uint32_t ai_uiOperFlags, M4DataStorage *ai_pInDataStore, m4handle_t &ao_hRequestId, m4bool_t ai_bLogInfo /*= M4_FALSE*/){
	return M4_ERROR;	// No tiene sentido en SM
}

m4return_t ClAppClientAdapSM::ReceiveRequest (m4handle_t ai_hRequestId, M4DataStorage *&ao_pOutDataStore, m4int32_t ai_lTimeOut, m4int16_t ai_lReceiveFlag, ClLogSystem *ai_poLog /* = NULL */) {
	return M4_ERROR;	// No tiene sentido en SM
}

m4return_t ClAppClientAdapSM::GetSessionContext( m4pchar_t &ao_pcX509Certificate, m4uint32_t &ai_riX509CertificateLen, m4pchar_t &ao_pcSessionContext, m4uint32_t &ai_riSessionContextLen )
{
	ClChannelManager *poCM = m_pObjServiceSMClient->GetpChannelManager();

	m4return_t res = M4_SUCCESS;

	if (poCM == NULL) {
		return M4_ERROR;
	}

	ClLogonAdaptor *poLA = poCM->GetLogonAdaptor();

	if (poLA == NULL) {
		return M4_ERROR;
	}
	
	m4pchar_t pcUserId = poLA->GetUsername();

	if (pcUserId == NULL) {
		return M4_ERROR;
	}

	ClExecuteItem oItemExecutor(poCM);
	ClCertManager oCertManager(&oItemExecutor);
	
	res = oCertManager.GetCertificate (pcUserId, ao_pcX509Certificate, ai_riX509CertificateLen, ao_pcSessionContext , ai_riSessionContextLen );
	if (res != M4_SUCCESS) {
		return M4_ERROR;
	}

	ClChannel *poSession = poLA->GetSessionChannelPointer();
	if (poSession == NULL) {
		return M4_ERROR;
	}

 	ClSessionContext oSessionContext;

	ClSesChnExport oSesChnExport;
	res = oSesChnExport.Export(poSession, oSessionContext);
	if (res != M4_SUCCESS) {
		return M4_ERROR;
	}

	res = oSessionContext.Serialize (ao_pcSessionContext,ai_riSessionContextLen);
	if (res != M4_SUCCESS) {
		return M4_ERROR;
	}

	if (ao_pcSessionContext == NULL) {
		ao_pcSessionContext = new m4char_t [ai_riSessionContextLen];
		res = oSessionContext.Serialize (ao_pcSessionContext,ai_riSessionContextLen);
	}

	return res;
}

