
#include "conn_provider_server.hpp"
#include "channel.hpp"
#include "cllgadap.hpp"
#include "m4lgadapdef.hpp"
#include "access.hpp"
#include "clldb_wrapper.hpp"
#include "m4mdrt.hpp"


ClLConn_Interface*
ClLConn_Provider_Server::GetConnection(ClChannel *ao_pChannel, m4bool_t ai_NewConn){
	if (ao_pChannel != NULL && ao_pChannel->GetMainLConn() != NULL)
	{
		return ao_pChannel->GetMainLConn();
	}
	if (!ai_NewConn) 
	{
		return m_pConnection;
	}
	m4VariantType vLConn;

	if (m_pConnection == NULL)
	{
		// tries to use the channel connection index
		if (ao_pChannel != NULL)
		{
			ao_pChannel->LDB_LastLogicConn.Get (vLConn);
			m_indConnection = (m4int32_t) vLConn.Data.DoubleData;
		}
		m_pConnection = m_pLdb->GetConnection (m_indConnection);

	}

	// priority to environment connection
	if (ao_pChannel != NULL) 
	{
		vLConn.Data.DoubleData = m_indConnection;	
		ao_pChannel->LDB_LastLogicConn.Set (vLConn);
	}			
    return( m_pConnection ) ;
}

m4return_t
ClLConn_Provider_Server::SetConnection  (ClLConn_Interface	 *ai_poClConn, m4bool_t ai_bIgnoreReferences){
	m4return_t iResult = M4_SUCCESS;
	// checks the incomming connection
	if (ai_bIgnoreReferences == M4_FALSE && ai_poClConn == m_pConnection){
		return M4_SUCCESS;
	}

	// have we to free the connection?
	if (m_pConnection != NULL && m_indConnection > M4LDB_NULL_INDEX)
	{
		// RTZ:
		// Nuevo. Paso la conexion, para que dentro se obtenga el Ususario del Kit de migración.
		iResult = m_pLdb->FreeConnection(m_indConnection,ai_bIgnoreReferences);

		// checks the error status
		if (iResult != M4_SUCCESS)
			return iResult;
	}

	m_pConnection = ai_poClConn;
	m_indConnection = M4LDB_NULL_INDEX;

	if (ai_bIgnoreReferences == M4_TRUE)
		return M4_SUCCESS;

	return M4_SUCCESS;
	
}
m4return_t
ClLConn_Provider_Server::End(void){
	SetConnection();
	ClLConn_Provider::End();
	return M4_SUCCESS;
}


