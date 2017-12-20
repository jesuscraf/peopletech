#include "ldbcontext.hpp"
#include "m4objreg.hpp"


ClLdbContextServer::ClLdbContextServer (void)
{
	m_poMutexConnect		= new ClMutex(M4_TRUE);
	m_poMutexSystem			= new ClMutex(M4_TRUE);
	m_poMutexLoadDict		= new ClMutex(M4_TRUE);
	m_poMutexObjects		= new ClMutex(M4_TRUE);
	m_poMutexObjectTrans	= new ClMutex(M4_TRUE);
	m_poMutexForeigns		= new ClMutex(M4_TRUE);
	m_poMutexRSMs			= new ClMutex(M4_TRUE);
	m_poMutexOrgTree		= new ClMutex(M4_TRUE);
	m_poMutexPartialLoad	= new ClMutex(M4_TRUE);
	m_poMutexSentences		= new ClMutex(M4_TRUE);
	m_poMutexSysSentences	= new ClMutex(M4_TRUE);
	m_poMutexDebug			= new ClMutex(M4_TRUE);
	m_numConn = 0;
}

ClLdbContextServer::~ClLdbContextServer(void)
{
	delete m_poMutexConnect;
	delete m_poMutexSystem;
	delete m_poMutexLoadDict;
	delete m_poMutexObjects;
	delete m_poMutexObjectTrans;
	delete m_poMutexForeigns;
	delete m_poMutexRSMs;
	delete m_poMutexOrgTree;
	delete m_poMutexPartialLoad;
	delete m_poMutexSentences;
	delete m_poMutexSysSentences;
	delete m_poMutexDebug;
}

m4return_t ClLdbContextServer::Init (m4int32_t ai_iNumConn)
{
	// Si no me viene numero de conexiones, no hago el Init. Me van a fallar todos los Lock y no se dara ninguna conexión.
	if (ai_iNumConn)
	{
		if (m_oLDBSWLConn.Init(ai_iNumConn, ai_iNumConn)!=M4_SUCCESS)
		{
			return M4_ERROR;
		}
	}
	return M4_SUCCESS;
}

m4int16_t ClLdbContextServer::GetMaxNumConn(ClVMBaseEnv *ai_pObjReg)
{
	m_numConn = ai_pObjReg->GetMaxNumConn();
	return m_numConn;
}

m4return_t ClLdbContextServer::Lock_LDBSWLConn(m4int32_t ai_time)
{
	return m_oLDBSWLConn.Lock(ai_time);
}

void ClLdbContextServer::UnLock_LDBSWLConn(void)
{
	m_oLDBSWLConn.Unlock();
}

m4int16_t ClLdbContextServer::GetNumConn(void)
{
	// En server las que diga el Registry. Es lo mismo que GetMaxNumConn.
	return m_numConn;
}

m4bool_t ClLdbContextServer::IsCriticalDBConnection(m4bool_t ai_bIsCritical)
{
	return ai_bIsCritical;
}

m4bool_t ClLdbContextServer::MustAttachConnection(m4int32_t ai_indLogConn)
{
	// La del sistema no se atacha.
	return ai_indLogConn!=-1?M4_TRUE:M4_FALSE;
}

