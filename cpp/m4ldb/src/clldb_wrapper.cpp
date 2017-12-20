
#include "clldb_wrapper.hpp"
#include "clldb.hpp"
#include "cllconn.hpp"
#include "m4eventhandlers.hpp"
#include "ldb_log.hpp"
#include "cllconn.hpp"



const m4pchar_t ERROR_LDB_NO_INIT = "ERROR";

ClLdb_Wrapper::ClLdb_Wrapper (void)
: m_bInit (M4_FALSE),
m_poContext(0)
{
	m_pLdb = new ClLdb();
}
	
ClLdb_Wrapper::~ClLdb_Wrapper(void){
	m_pLdb->End();
	delete m_pLdb;
	m_pLdb=0;
}

m4return_t ClLdb_Wrapper::Init(ClVMBaseEnv *ai_poObjReg, ClCMCRFactory *ai_poFactory, ILdbContext *ai_poContext)
{
	// Esta función no es thread safe, en la primera inicialización. ( Una vez inicializada no hay problema.)
	// No pueden hacer el Init dos threads a la vez porque está protegido en la m4oeinit.
	// Si se diera el caso habría que poner un Mutex aquí.
	if (m_bInit==M4_TRUE){
		return M4_SUCCESS;
	}

	// El context se inicializa dentro de la ClLdb.
	if (m_pLdb->Init(ai_poObjReg, ai_poFactory, ai_poContext)!=M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR(M4_CH_LDB_INIT_ERROR);
		m_pLdb->End();
		return M4_ERROR;
	}

	m_poContext = ai_poContext;
	m_bInit = M4_TRUE;
	return M4_SUCCESS;
}

m4return_t ClLdb_Wrapper::End(void)
{
	if (m_bInit==M4_FALSE){
		return M4_SUCCESS;
	}
	if (m_pLdb->End()!=M4_SUCCESS){
		return M4_ERROR;
	}
	m_bInit = M4_FALSE;

	return M4_SUCCESS;
}

m4return_t ClLdb_Wrapper::SetCacheDirectory(ClCacheDirectory *ai_poCacheDirectory)
{
	if (m_bInit==M4_FALSE){
		return M4_ERROR;
	}
	return m_pLdb->SetCacheDirectory (ai_poCacheDirectory);
}

ClLConn_Interface * ClLdb_Wrapper::GetConnection(m4int32_t &ao_indConnection)
{
	if (m_bInit==M4_FALSE){
		return 0;
	}

	ClLConn *pLConn = 0;
	m4return_t ret = M4_SUCCESS;


	ret = m_poContext->Lock_LDBSWLConn(LDB_SEM_TIMEOUT);
	if (ret!=M4_SUCCESS){
		DUMP_CHLOG_WARNING (M4_CH_LDB_CONN_TIMEOUT);
		// 65550="Unable to serve logical connection"
		M4PublishEvent("65550","");
		return 0;
	}
		
	pLConn = m_pLdb->GetConnection (ao_indConnection);

	// Si no nos han dado conexión unlockeamos.
	if (pLConn==0) {
		m_poContext->UnLock_LDBSWLConn();
	}

	if (m_pLdb->GetNumConnInUse() >= (m4uint32_t)m_pLdb->GetNumConn()) {
		// 65551 = "Logical connection exhaustion"
		M4PublishEvent("65551","");
	}

	return (pLConn);
}

ClLConn_Interface * ClLdb_Wrapper::GetConnection28(m4pcchar_t ai_pConnStr, m4pcchar_t ai_pUser, m4pcchar_t ai_pPassword)
{
	if (m_bInit==M4_FALSE){
		return 0;
	}
	return m_pLdb->GetConnection28 (ai_pConnStr, ai_pUser, ai_pPassword);
}

m4return_t ClLdb_Wrapper::FreeConnection(m4int32_t ai_indConnection, m4bool_t ai_bIgnoreChannel)
{
	if (m_bInit==M4_FALSE){
		return M4_ERROR;
	}

	m_pLdb->FreeConnection (ai_indConnection,ai_bIgnoreChannel);

	m_poContext->UnLock_LDBSWLConn();

	return M4_SUCCESS;
}

m4return_t ClLdb_Wrapper::FreeConnection28(ClLConn_Interface *&ao_pLConn)
{
	if (m_bInit==M4_FALSE){
		return M4_ERROR;
	}
	return m_pLdb->FreeConnection28 ((ClLConn *&)ao_pLConn);
}

m4return_t ClLdb_Wrapper::ClearAll (void)
{
	if (m_bInit==M4_FALSE){
		return M4_ERROR;
	}
	return m_pLdb->ClearAll ();
}

m4return_t ClLdb_Wrapper::ClearTables (void)
{
	if (m_bInit==M4_FALSE){
		return M4_ERROR;
	}
	return m_pLdb->ClearTables ();
}

m4return_t ClLdb_Wrapper::ClearCache (void)
{
	if (m_bInit==M4_FALSE){
		return M4_ERROR;
	}
	return m_pLdb->ClearCache ();
}

m4return_t ClLdb_Wrapper::ClearTrans (void)
{
	if (m_bInit==M4_FALSE){
		return M4_ERROR;
	}
	return m_pLdb->ClearTrans ();
}

m4pcchar_t	ClLdb_Wrapper::GetDictConnStr()
{
	if (m_bInit==M4_FALSE){
		return ERROR_LDB_NO_INIT;
	}
	return m_pLdb->GetDictConnStr();
}

m4int16_t	ClLdb_Wrapper::GetNumConn()
{
	if (m_bInit==M4_FALSE){
		return 0;
	}
	return m_pLdb->GetNumConn();
}

m4int16_t	ClLdb_Wrapper::GetNumCursor()
{
	if (m_bInit==M4_FALSE){
		return 0;
	}
	return m_pLdb->GetNumCursor();
}

m4pcchar_t	ClLdb_Wrapper::GetConExec()
{
	if (m_bInit==M4_FALSE){
		return ERROR_LDB_NO_INIT;
	}
	return m_pLdb->GetConExec();
}

m4int16_t	ClLdb_Wrapper::GetNumStmtCache()
{
	if (m_bInit==M4_FALSE){
		return 0;
	}
	return m_pLdb->GetNumStmtCache();
}

m4int16_t	ClLdb_Wrapper::GetNumObject()
{
	if (m_bInit==M4_FALSE){
		return 0;
	}
	return m_pLdb->GetNumObject();
}

m4pcchar_t	ClLdb_Wrapper::GetIsolationLevel()
{
	if (m_bInit==M4_FALSE){
		return ERROR_LDB_NO_INIT;
	}
	return m_pLdb->GetIsolationLevel();
}

m4pcchar_t	ClLdb_Wrapper::GetDataBaseType()
{
	if (m_bInit==M4_FALSE){
		return ERROR_LDB_NO_INIT;
	}
	return m_pLdb->GetDataBaseType();
}

m4pcchar_t	ClLdb_Wrapper::GetDebugLevel()
{
	if (m_bInit==M4_FALSE){
		return ERROR_LDB_NO_INIT;
	}
	return m_pLdb->GetDebugLevel();
}

m4uint32_t	ClLdb_Wrapper::GetNumConnInUse()
{
	if (m_bInit==M4_FALSE){
		return 0;
	}
	return m_pLdb->GetNumConnInUse();
}

m4uint32_t	ClLdb_Wrapper::GetNumLogicObjectInUse()
{
	if (m_bInit==M4_FALSE){
		return 0;
	}
	return m_pLdb->GetNumLogicObjectInUse();
}

m4uint32_t	ClLdb_Wrapper::GetNumStatmntCacheInUse()
{
	if (m_bInit==M4_FALSE){
		return 0;
	}
	return m_pLdb->GetNumStatmntCacheInUse();
}

m4return_t	ClLdb_Wrapper::SetSysDebugLevel(m4uint8_t ai_uiSysDebugLevel)
{
	if (m_bInit==M4_FALSE){
		return M4_ERROR;
	}
	return m_pLdb->SetSysDebugLevel(ai_uiSysDebugLevel);
}

//Detalle de trazas en ldbinsp.
m4return_t	ClLdb_Wrapper::SetSystemDebugDetailLevel(m4uint32_t ai_uiSystemDebugDetailLevel)
{
	if (m_bInit==M4_FALSE){
		return M4_ERROR;
	}
	return m_pLdb->SetSystemDebugDetailLevel(ai_uiSystemDebugDetailLevel);
}

m4pcchar_t	ClLdb_Wrapper::GetStrSystemDebugDetailLevel (void)
{
	if (m_bInit==M4_FALSE){
		return ERROR_LDB_NO_INIT;
	}
	return m_pLdb->GetStrSystemDebugDetailLevel();
}


m4uint32_t	ClLdb_Wrapper::GetSystemDebugDetailLevel (void)
{
	if (m_bInit==M4_FALSE){
		return M4LDB_DETAIL_LEVEL_SHOW_NONE;
	}
	return m_pLdb->GetSystemDebugDetailLevel();
}

void ClLdb_Wrapper::Attach_DBRyptureConnection(ClNotifiable *ai_pNotifiable)
{
	m_pLdb->Attach_DBRyptureConnection(ai_pNotifiable);
}

m4return_t ClLdb_Wrapper::RefreshInvalidConnections(m4int32_t ai_indDBConn,m4int32_t ai_indLConn)
{
	return m_pLdb->RefreshInvalidConnections(ai_indDBConn,ai_indLConn);
}

//Posible orden: m4objreg, m4dm, m4ldb, m4hsparser, m4mdfac, m4vm, m4oeinit, m4ldbsubsy, m4exeforchannel, m4comsrv.

//Implicadas en modificacion del interfaz de bdl (buscando *.hpp, *.cpp) ILdb (24/02/2004):
//	m4ldb, m4oeinit, m4ldbsubsy, m4exeforchannel, m4comsrv
//Implicadas en mensajes de error:
//	m4log
//Implicadas en cambio de m2 (version de m2):
//	m4buildversion, m4mdfac, m4objreg
//Implicadas en set de la session:
//	m4executive, m4jsexejob
//Implicadas en el cambio de interfaz de la estructura stConexionProperties:
//	m4dbwrap

//Orden definitivo:
//m4buildversion, m4log, m4objreg, m4dbwrap, m4ldb, m4mdfac, m4oeinit, m4executive, m4ldbsubsy, m4exeforchannel, m4jsexejob, m4comsrv

void ClLdb_Wrapper::UserSessionClosedByIdSession( m4uint64_t ai_iIdSession )
{
	if (m_pLdb != NULL) {
		m_pLdb->UserSessionClosedByIdSession( ai_iIdSession );
	}
}
