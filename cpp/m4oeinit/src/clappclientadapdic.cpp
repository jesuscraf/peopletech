#include "clappclientadapdic.hpp"

ClAppClientAdapDIC::ClAppClientAdapDIC() {
	m_hSessionId=0;
	m_pClClientAPI = new ClClientAPI(NULL);
}


ClAppClientAdapDIC::ClAppClientAdapDIC(m4int_t ai_iCommsType, m4int32_t ai_iControlPort, m4int32_t ai_nCommWorkers)
{
    m_hSessionId=0;
	m_pClClientAPI = new ClClientAPI(NULL, ai_iCommsType, ai_iControlPort, ai_nCommWorkers);
}

ClAppClientAdapDIC::~ClAppClientAdapDIC() {
	CloseSession();
	delete m_pClClientAPI;
}

m4return_t ClAppClientAdapDIC::CloseServices() {
	if (0 == m_hSessionId) //Tenemos que tener abierta una sesion
		return M4_ERROR;

	m4handle_t hServiceContext;
	//Cerramos todos los servicios
	ClientServiceMapIt it = m_oClientServiceMap.begin();
	while (it != m_oClientServiceMap.end()) {
		hServiceContext = (*it).second;
		m_pClClientAPI->CloseService (m_hSessionId, hServiceContext);
		it++;
	}
	m_oClientServiceMap.clear();

	return M4_SUCCESS;
}

m4return_t	ClAppClientAdapDIC::CloseSession( void ) {
	if (0 == m_hSessionId) //Tenemos que tener abierta una sesion
		return M4_ERROR;
	//Cerramos todos los servicios
	CloseServices();

	//Cerramos la sesión
	m4return_t iRet = m_pClClientAPI->CloseSession( m_hSessionId );
	m_hSessionId = 0;
	return iRet;
}

m4return_t	ClAppClientAdapDIC::SetCommsLanguage( m4language_t ai_iLang ) 
{
	m4return_t iRet = M4_ERROR ;

	iRet = m_pClClientAPI->SetCommsLanguage( ai_iLang ) ;

	return iRet ;
}

m4return_t ClAppClientAdapDIC::OpenService (m4pchar_t ai_szServiceId, m4handle_t &ao_hContext) {
	if (0 == m_hSessionId) //Tenemos que tener abierta una sesion
		return M4_ERROR;

	string sServiceName = ai_szServiceId;
	//Buscamos el nombre del servicio
	ClientServiceMapIt it = m_oClientServiceMap.find(sServiceName);
	if (it != m_oClientServiceMap.end()) {
		ao_hContext = (*it).second;
	}
	else {
		//No lo encontramos, luego lo abrimos y lo insertamos en el mapa
		if (m_pClClientAPI->OpenService (m_hSessionId, ai_szServiceId, ao_hContext) != M4_SUCCESS)
			return M4_ERROR;
		m_oClientServiceMap.insert(ClientServiceMap::value_type(sServiceName,ao_hContext));
	}
	return M4_SUCCESS;
}

m4return_t ClAppClientAdapDIC::SyncRequest (m4handle_t ai_hServiceContext, m4uint32_t ai_uiStateFlags, m4uint32_t ai_uiOperFlags, M4DataStorage *ai_pInDataStore, M4DataStorage *&ao_pOutDataStore, m4handle_t &ao_hRequestId, m4uint32_t ai_uiTimeout /*= 00*/, m4bool_t ai_bLogInfo /*= M4_FALSE*/, ClLogSystem *ai_poLog /*=NULL*/, ClTimeoutFunction ai_pTimeoutFunction /*=NULL*/) {
	if (0 == m_hSessionId) //Tenemos que tener abierta una sesion
		return M4_ERROR;

	return( m_pClClientAPI->SyncRequest (m_hSessionId, ai_hServiceContext, ai_uiStateFlags, ai_uiOperFlags, ai_pInDataStore, ao_pOutDataStore, ao_hRequestId, ai_uiTimeout, ai_bLogInfo, ai_poLog, ai_pTimeoutFunction));
}


m4return_t ClAppClientAdapDIC::SyncRequest (m4pchar_t ai_szServiceId, m4uint32_t ai_uiStateFlags, m4uint32_t ai_uiOperFlags, M4DataStorage *ai_pInDataStore, M4DataStorage *&ao_pOutDataStore, m4handle_t &ao_hRequestId, m4uint32_t ai_uiTimeout, m4bool_t ai_bLogInfo, ClLogSystem *ai_poLog, ClTimeoutFunction ai_pTimeoutFunction) {
	if (0 == m_hSessionId) //Tenemos que tener abierta una sesion
		return M4_ERROR;

	m4handle_t hServiceContext;
	
	if (OpenService(ai_szServiceId, hServiceContext) == M4_SUCCESS) {
		return( m_pClClientAPI->SyncRequest (m_hSessionId, hServiceContext, ai_uiStateFlags, ai_uiOperFlags, ai_pInDataStore, ao_pOutDataStore, ao_hRequestId, ai_uiTimeout, ai_bLogInfo, ai_poLog, ai_pTimeoutFunction));
	}

	return M4_ERROR;
}

m4return_t ClAppClientAdapDIC::AsyncRequest(m4handle_t ai_hServiceContext, m4uint32_t ai_uiStateFlags, m4uint32_t ai_uiOperFlags, M4DataStorage *ai_pInDataStore, m4handle_t &ao_hRequestId, m4bool_t ai_bLogInfo /*=M4_FALSE*/){

	if (0 == m_hSessionId) //Tenemos que tener abierta una sesion
		return M4_ERROR;

	return m_pClClientAPI->AsyncRequest(m_hSessionId, ai_hServiceContext, ai_uiStateFlags, ai_uiOperFlags, ai_pInDataStore, ao_hRequestId, ai_bLogInfo);
}


m4return_t ClAppClientAdapDIC::AsyncRequest(m4pchar_t ai_szServiceId, m4uint32_t ai_uiStateFlags, m4uint32_t ai_uiOperFlags, M4DataStorage *ai_pInDataStore, m4handle_t &ao_hRequestId, m4bool_t ai_bLogInfo /*=M4_FALSE*/){
	if (0 == m_hSessionId) //Tenemos que tener abierta una sesion
		return M4_ERROR;

	m4handle_t hServiceContext;
	
	if (OpenService(ai_szServiceId, hServiceContext) == M4_SUCCESS) {
		return m_pClClientAPI->AsyncRequest(m_hSessionId, hServiceContext, ai_uiStateFlags, ai_uiOperFlags, ai_pInDataStore, ao_hRequestId, ai_bLogInfo);
	}

	return M4_ERROR;
}

m4return_t ClAppClientAdapDIC::ReceiveRequest (m4handle_t ai_hRequestId, M4DataStorage *&ao_pOutDataStore, m4int32_t ai_lTimeOut, m4int16_t ai_lReceiveFlag, ClLogSystem *ai_poLog /* = NULL */) {
	if (0 == m_hSessionId) //Tenemos que tener abierta una sesion
		return M4_ERROR;

	return( m_pClClientAPI->ReceiveRequest (m_hSessionId, ai_hRequestId, ao_pOutDataStore, ai_lTimeOut, ai_lReceiveFlag, ai_poLog));
}

m4return_t ClAppClientAdapDIC::GetSessionContext( m4pchar_t &ao_pcX509Certificate, m4uint32_t &ai_riX509CertificateLen, m4pchar_t &ao_pcSessionContext, m4uint32_t &ai_riSessionContextLen )
{

	m4return_t	iResult = M4_SUCCESS ;


	if( m_hSessionId == 0 ) //Tenemos que tener abierta una sesion
	{
		return( M4_ERROR ) ;
	}

	iResult = m_pClClientAPI->GetSessContext( m_hSessionId, ao_pcX509Certificate, ai_riX509CertificateLen, ao_pcSessionContext, ai_riSessionContextLen ) ;

	return( iResult ) ;
}

void ClAppClientAdapDIC::NotifyVMExecution(m4bool_t ai_bIsStart)
{
	m_pClClientAPI->NotifyVMExecution(m_hSessionId, ai_bIsStart);
}

