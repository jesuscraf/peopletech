#include "connmanager.hpp"
#include "cllconn.hpp"
#include "access.hpp"
#include "ldb_log.hpp"
#include "m4ldb_notif.hpp"
#include "conex.hpp"
#include "ldbcontext.hpp"	// ILdbContext

#include "clstatem.hpp"	// Para M4LDB_BEGIN_DEBUG

#include "m4event.hpp"
#include "cllgadap.hpp"


extern m4cint8_t *g_aDDBBs[];

// Identificador de la conexion fisica principal.
const m4int32_t ClConnectionManager::m_sSystemDBConn=0;
//Bug 0080564
const m4int32_t	ClConnectionManager::m_sValidateSecurity = -3;

/*
ClDBConn_cont::~ClDBConn_cont(){
	if (m_pDBConn){
		m_pDBConn->End();
		delete m_pDBConn;
		m_pDBConn = 0;
	}
}
*/
m4return_t 
ClDBConn_cont::CreateConnection(void){
	//OJO. No destruye lo que hubiera.
	if (m_pDBConn==0){
		m_pDBConn = new ClDBConnection;
	}
	return M4_SUCCESS;
}
m4return_t
ClDBConn_cont::DestroyConnection(void){
	if (m_pDBConn){
		delete m_pDBConn;
		m_pDBConn=0;
	}
	return M4_SUCCESS;
}

m4void_t
ClDBConn_cont::MarkToDisconnect(void){
	m_bDoDisconnect = M4_TRUE; 
}

m4return_t
ClDBConn_cont::CreateConnection(m4int32_t ai_idRConn, stConexionProperties *ai_pstConProp, ClLConn *ai_pLConn)
{
	// Bug 0281817 Sólo se puede desconectra sio no estamos dentro de una transacción
	if (m_bDoDisconnect && ai_pLConn->GetNumTrans() <= 0)
	{
		DestroyConnection();
		m_bDoDisconnect = M4_FALSE;
	}

	if (m_pDBConn==0){
		// La creamos.
		if (CreateConnection()!=M4_SUCCESS){
			return M4_ERROR;
		}
		if (m_pDBConn->Init(ai_idRConn,ai_pstConProp)!=M4_SUCCESS){
			delete m_pDBConn;
			m_pDBConn=0;
			return M4_ERROR;
		}
	}

	return M4_SUCCESS;
}

ClConnectionManager::ClConnectionManager( void )
{
	m_pMpClKeyDynamicConnection = NULL;
}

m4return_t
ClConnectionManager::InitGenericMap(ClAccess *ai_pDictAccess,ClLConn *ai_pLConn){
	
	m4int32_t numRConn = m_oConnInfo.GetNumRConn(ai_pDictAccess);
	stConexionProperties	stConProp;
	m4int32_t idRConn=0;
	// Para cada una de las conexiones fisicas pedimos el Id y sus propiedades.
	// Las creamos pero no las abrimos.
	// Cuando se quiera insertar una conexion logica nueva, hay que dar como segunda parte del mapa el mapa estatico
	// que contiene los objetos conexion creados, cada uno con la informacion de la BD a la que tiene que conectarse, pero sin conexion.
	for (m4int32_t pos = 0;pos<numRConn;++pos){
		// Pedimos el Id y las caracteristicas de la conexion.
		if (m_oConnInfo.GetInfoByPos(ai_pDictAccess,pos,idRConn,stConProp)!=M4_SUCCESS){
			// Mal empezamos.
			return M4_ERROR;
		}
		if (ValidateConnectionProperties(idRConn,stConProp,ai_pLConn)==M4_SUCCESS){
			// Insertamos en el mapa.
			if (InsertOneEntryInGenericMap(idRConn,stConProp)!=M4_SUCCESS){
				return M4_ERROR;
			}
		}
		else{
			// Sacamos el error y seguimos. No la metemos en al mapa.
			DUMP_LDBINSP_ERRORF (ai_pLConn,M4_CH_LDB_INVALID_DB_CONNECTION,idRConn);
		}
	}

	//Conexion de validacion de seguridad.
	stConexionProperties *pstConPropAuxSec;
	if (_GetConnectionInfo (0, ai_pLConn, pstConPropAuxSec)!=M4_SUCCESS){
		return M4_ERROR;
	}
	if (InsertOneEntryInGenericMap(m_sValidateSecurity,*pstConPropAuxSec)!=M4_SUCCESS){
		return M4_ERROR;
	}

	return M4_SUCCESS;
}
m4return_t
ClConnectionManager::InsertOneEntryInGenericMap(m4int32_t ai_iIdRConn,stConexionProperties &ai_stConProp){

	pair<itMapDBConnection, bool> oDBpair;
	pair<itMapConnProp, bool> oConProppair;
	// Insertamos en el mapa.
	oDBpair = m_mGenericMapDBConn.insert (MapDBConnection::value_type(ai_iIdRConn,m_oDBConn_Nihil));
	if (oDBpair.second!=M4_TRUE){
		// Mal rollo. Había un Id repetido.
		return M4_ERROR;
	}
	oConProppair = m_mGenericProperies.insert (MapConnProp::value_type(ai_iIdRConn,ai_stConProp));
	if (oConProppair.second!=M4_TRUE){
		// Mas mal rollo. Había un Id repetido.
		return M4_ERROR;
	}
/*	
	// Inicializamos la conexion. (oDBpair.first es el iterador que apunta a la que acabamos de insertar)
	if ((*oDBpair.first).second->Init(ai_iIdRConn,ai_stConProp)!=M4_SUCCESS){
		return M4_ERROR;
	}
*/
	return M4_SUCCESS;
}

m4return_t
ClConnectionManager::Init(ClVMBaseEnv *ai_pObjReg,ClAccess *ai_pDictAccess,ClLConn *ai_pLConn,ILdbContext *ai_pContext){

	m_pMpClKeyDynamicConnection = new MpClKeyDynamicConnection;

	// Tenemos que conectarnos a BD para poder obtener informacion sobre el número de conexiones reales que vamos a tener.
	// Nos conectamos y la conexión física que utilicamos se la damos a 'ai_pLConn'.
	// Esta primera deberá ser la del sistema. Si no lo fuera no pasaría nada, simplemente que le damos el objeto conexión inicializado.
	
	// Leemos la información de la conexion del sistema del Registry.
	stConexionProperties	stSysConProp;
	if (m_oConnInfo.Init(ai_pObjReg,ai_pContext)!=M4_SUCCESS){
		return M4_ERROR;
	}

	if (m_oConnInfo.GetInfoSystemConnection(stSysConProp)!=M4_SUCCESS){
		return M4_ERROR;
	}
	if (ValidateConnectionProperties(m_sSystemDBConn,stSysConProp,ai_pLConn)!=M4_SUCCESS){
		return M4_ERROR;
	}

// Nos conectamos a BD y cargamos los nodos de conexiones.
	
	// Metemos la primera entrada en el mapa de conexiones.
	if (InsertOneEntryInGenericMap(m_sSystemDBConn,stSysConProp)!=M4_SUCCESS){
		return M4_ERROR;
	}
	// Creamos una entrada en el mapa de conexiones logicas.
	if (InsertLogicalConnection(ai_pLConn->GetIdLConn())!=M4_SUCCESS){
		return M4_ERROR;
	}
	// Intentamos conectarnos.
	// Dentro del InitConnection ya se reintentará si hubiera bases de datos copia.
	if (InitConnection(m_sSystemDBConn,ai_pLConn)==0){
		return M4_ERROR;
	}
	// Y por fin podemos cargar :
	ClAccessRecordSet *pRSConnections = &(ai_pDictAccess->Node[(ClNodeIndex) M4LDB_SYS_DICT_CONNECTIONS].RecordSet);
	if (ai_pLConn->SysLoad	(pRSConnections,0,M4LDB_NULL_INDEX )!=M4_SUCCESS){
		return M4_ERROR;
	}

// Ya tenemos la información necesaria para crear las conexiones físicas.

	// Terminamos de rellenar el mapa de conexiones físicas( el que nos va a servir para insertar después el resto de las conexiones lógicas )
	if (InitGenericMap(ai_pDictAccess,ai_pLConn)!=M4_SUCCESS){
		return M4_ERROR;
	}
	// Actualizamos la conexión del sistema. Hasta ahora solo tenía una conexion fisica.
	// Añadimos el resto de las conexiones.
	if (UpdateSystemConnection(ai_pLConn->GetIdLConn())!=M4_SUCCESS){
		return M4_ERROR;
	}

	return M4_SUCCESS;
}
m4return_t
ClConnectionManager::UpdateSystemConnection(m4int32_t ai_indLConn){
	// Tenemos la entrada en el mapa con una sola conexion física.
	// La sacamos y volvemos a meterla con todas las demas.
	// La conexion fisica que tenemos creada la reaprovechamos. No la destruimos y volvemos a crearla.

// Cacheamos la conexion fisica.
	ClDBConnection *pConn = FindDBConnection(ai_indLConn,m_sSystemDBConn);
// Borramos la entrada de la conexion ai_indLConn.
	m_mLogConn2DBConn.erase(ai_indLConn);
// La volvemos a insertar. Ahora ya va con todas las conexiones fisicas.
	if (InsertLogicalConnection(ai_indLConn)!=M4_SUCCESS){
		return M4_ERROR;
	}
// Actualizamos la conexion fisica correspondiente a la del sistema.
	if (SetDBConnection(pConn,ai_indLConn,m_sSystemDBConn)!=M4_SUCCESS){
		return M4_ERROR;
	}

	return M4_SUCCESS;
}

m4return_t
ClConnectionManager::GetConnectionInfo(m4int32_t ai_idRConn,ClLConn *ai_pLConn,stConexionProperties &ao_stConProp){
	// Se lo pedimos a la conexion fisica.
	stConexionProperties *pstConProp = 0;
	if (_GetConnectionInfo(ai_idRConn,ai_pLConn,pstConProp)!=M4_SUCCESS){
		return M4_ERROR;
	}
	ao_stConProp = *pstConProp;
	return M4_SUCCESS;
}

m4return_t
ClConnectionManager::_GetConnectionInfo(m4int32_t ai_idRConn,ClLConn *ai_pLConn,stConexionProperties *&ao_pstConProp){
	ao_pstConProp = 0;
	itMapConnProp	itConnProp;
	itConnProp=m_mGenericProperies.find(ai_idRConn);
	if (itConnProp==m_mGenericProperies.end()){
		DUMP_LDBINSP_ERRORF (ai_pLConn, M4_CH_LDB_NO_DB_CONNECTION, ai_idRConn);
		return M4_ERROR;
	}
	ao_pstConProp = &(*itConnProp).second;
	return M4_SUCCESS;
}
m4return_t
ClConnectionManager::GetDDBBType (m4int32_t ai_idRConn,ClLConn *ai_pLConn,eDDBBType_t &ao_eDDBB){
	stConexionProperties *pstConProp = 0;
	if (_GetConnectionInfo(ai_idRConn,ai_pLConn,pstConProp)!=M4_SUCCESS){
		return M4_ERROR;
	}
	ao_eDDBB=pstConProp->m_eDBW;
	return M4_SUCCESS;
}

m4return_t
ClConnectionManager::InsertLogicalConnection(m4int32_t ai_indLConn){
	// Insertamos la conexion con el mapa por defecto.
	return m_mLogConn2DBConn.insert(
				MapLogConn2DBConn::value_type(ai_indLConn,m_mGenericMapDBConn)
			).second ? M4_SUCCESS : M4_ERROR;
}
ClDBConnection *
ClConnectionManager::InitConnection(m4int32_t ai_idRConn,ClLConn *ai_pLConn){
	ClDBConn_cont	&oDBConn = _InitConnection(ai_idRConn,ai_pLConn);
	if (&oDBConn == &m_oDBConn_Nihil){
		return 0;
	}
	return oDBConn.m_pDBConn;
}

ClDBConn_cont	&
ClConnectionManager::_InitConnection(m4int32_t ai_idRConn,ClLConn *ai_pLConn){
//	Buscamos la conexión que tenemos que iniciar.
	m4return_t ret = M4_SUCCESS;
	m4int32_t indLConn = ai_pLConn->GetIdLConn();
	ClDBConn_cont * pDBConn = NULL;

	ClDBConn_cont &oDBConn=_FindDBConnection(indLConn,ai_idRConn);
	stConexionProperties *pStConProp = 0;
	if (&oDBConn == &m_oDBConn_Nihil){
		return m_oDBConn_Nihil;
	}
	if (_GetConnectionInfo(ai_idRConn,ai_pLConn,pStConProp)!=M4_SUCCESS){
		// No hay informacion de esa conexion.
		return m_oDBConn_Nihil;
	}

	string sUser("");
	if ((ai_pLConn) && (ai_pLConn->GetIdUserTrace() != NULL) && (pStConProp != NULL)) {

		sUser.append(ai_pLConn->GetIdUserTrace());

		if (sUser.empty() != M4_TRUE) {
			//Puede ser que el usuario tenga un tipo especial de conexión!!. LO DEJAMOS PREPARADO.
			//eConnectionType_t eCTUser = (eConnectionType_t) ai_pLConn->GetConnectionTypeUser();
			eConnectionType_t eCTUser = M4LDB_CONN_TYPE_NORMAL;
			eConnectionType_t eCTConnection = pStConProp->m_eConnectionType;

			//--------------------------------------------------------------
			if ((IsDynamicConnection(eCTUser) == M4_TRUE) || (IsDynamicConnection (eCTConnection) == M4_TRUE)) {
				 //Ojo pq habría que establecer una prioridad a los tipos!.
				//En principio SOLO tengo en cuenta el tipo de conexion que viene de las propiedades de la conexion.
				eConnectionType_t eCTEffective = eCTConnection;

				m4uint64_t iIdSession = ai_pLConn->GetIdSession();

				ClValueDynamicConnection * pValueDynamicConnection = NULL;
				//La sesion se la preguntamos al contexto.			
				ClKeyDynamicConnection oKeyDynamicConnection(iIdSession, sUser, ai_idRConn);

				//---------------------- Obteniendo contenedor de conexion dinamica ----------------------------
				//Dada una clave identificativa, el siguiente bloque nos devuelve un contenedor con una conexion
				//física dinamica!!.
				
				pValueDynamicConnection = FindIntoMapDynamicConnectionByKey( oKeyDynamicConnection );
				
				//Si se ha encontrado conexion fisica tambien tendremos en el segundo argumento un puntero a la clave
				//que cumple la busqueda.
				//Importante!!: Si no se ha encontrado entonces ambos punteros serán nulos.

				//--------------------------
				if (pValueDynamicConnection == NULL) {
					//Las propiedades de la conexion han podido modificarse: cambiando el usuario y pwd!.
					//Esto se efectua dentro del contexto de una conexion logica.
					//Escenario:
					//Usu1 pide conexion logica1 y aqui modificaríamos la cadena de conexion!. Una vez liberada la conexion
					//logica puede haber otro usuario que obtenga la conexion logica1 y volveríamos a cambiar la cadena de conexion.
					//Lo que tenemos cacheado es la conexion física, pero la cadena se calcula cada vez!! y será coherente.
					//Obtenemos la password de un determinado usuario y se rellenara la cadena de conexion con informacion de la password.
					//Le pasamos el objeto creado que se insertara en el mapa, del objeto puedo sacar informacion del indice
					//de la conexion real y el usuario!.
					if (GetDynamicConnectionStringByConnectionType( eCTEffective, ai_pLConn, oKeyDynamicConnection, pStConProp ) != M4_SUCCESS) {
						return m_oDBConn_Nihil;
					}

					pDBConn = new ClDBConn_cont;
					//También nos guardamos la conexion lógica.
					pValueDynamicConnection = new ClValueDynamicConnection(pDBConn, ai_pLConn);

					//Nos cacheamos la password y se lo indicamos a la clave del mapa!.	
					if (pStConProp != NULL)  {
						pValueDynamicConnection->SetConnStr( pStConProp->m_stConnStr );
					}

					//No es muy elegante, pero nos vamos a cachear el formato con que se muestra la cadena de conexion
					//para no tener que calcularla cada vez al mostrar la traza.
					//-----				
					#ifdef _M4LDB_DEBUG
						if (ai_pLConn->GetSystemDebugEnable() <= ai_pLConn->GetpLdb()->GetSysDebugLevel ()) {
							string sFormatedString("");
							MpParseStringConnection oTokenMap;
							ParseConnectionString(pStConProp->m_stConnStr, oTokenMap);
							FormatConnectionString(oTokenMap, sFormatedString);
							pValueDynamicConnection->SetConnStrFormated(sFormatedString);
						}
					#endif
					//-----

					if (InsertIntoMapDynamicConnection( oKeyDynamicConnection, pValueDynamicConnection ) != M4_SUCCESS) {
						//Si la insercion falla es pq ya estaba en el mapa, pero es imposible pq si estamos aqui
						//es pq no se encontro en el mapa. De todas formas lo comprobamos!.
						delete pDBConn;
						delete pValueDynamicConnection;
						return m_oDBConn_Nihil;
					}
				}
				else {
					pDBConn = pValueDynamicConnection->GetDBConn();
					if (pDBConn == NULL) {
						//Esto no puede producirse pero lo comprobamos.
						return m_oDBConn_Nihil;
					}
					//Se ha encontrado valor, pero tenemos que actualizar en qué conexion lógica estamos!.
					pValueDynamicConnection->SetLogicalConnection(ai_pLConn);

					//Se ha encontrado valor, obtenemos la cadena de conexion que esta cacheada.
					pStConProp->m_stConnStr = pValueDynamicConnection->GetConnStr( );
				}
				//--------------------------

				//En este punto tenemos una conexion fisica dinamica, pq se ha encontrado o pq se ha creado nueva.
				//Si es dinamica sí se habrá modificado la cadena de conexion.
				//---------------------- Fin Obteniendo contenedor de conexion dinamica ----------------------------

				//-----	
				//Mostramos el formato en la traza, esta cacheado.
				#ifdef _M4LDB_DEBUG
					if (ai_pLConn->GetSystemDebugEnable() <= ai_pLConn->GetpLdb()->GetSysDebugLevel ()) {
						//El puntero sera el objeto local o el objeto encontrado en el mapa, pero de cualquier
						//forma el formateo es el mismo. No puede ser nulo pero lo comprobamos.
						if (pValueDynamicConnection != NULL) {
							ai_pLConn->m_pInspector->PrintFileDebug( ai_pLConn, "\n\t\t- User application: '%s' executing statement in database user: '%s'.", sUser.c_str(), pValueDynamicConnection->GetConnStrFormated().c_str() );
							ai_pLConn->m_pInspector->PrintFileDebugDetail( M4LDB_DETAIL_LEVEL_SHOW_ALWAYS, ai_pLConn, "\n\t\t- User application: '%s' executing statement in database user: '%s'.", sUser.c_str(), pValueDynamicConnection->GetConnStrFormated().c_str() );
						}
					}
				#endif
				//-----

			} //if ((IsDynamicConnection(eCTUser) == M4_TRUE) || (IsDynamicConnection (eCTConnection) == M4_TRUE)) {
			else {
				//Si la conexion no es dinamica (ni la del usuario ni la que me viene son dinamicas entonces ...)
				pDBConn = &oDBConn;
				//La conexion será normal.
			}
			//--------------------------------------------------------------

		} //if (sUser.empty() != M4_TRUE) {
		else {
			//Si no hay usuario definido...
			pDBConn = &oDBConn;
			//La conexion será normal.
		}
	} //if ((ai_pLConn) && (ai_pLConn->GetIdUserTrace() != NULL) && (pStConProp != NULL)) {
	else {
		//Si no hay conexion logica, no me lo creo pero por si acaso...
		pDBConn = &oDBConn;
		//La conexion será normal.
	}
	//---------------------------------------------

	// eh 202503
	ret=pDBConn->CreateConnection(ai_idRConn, pStConProp, ai_pLConn);
	if(ret!=M4_SUCCESS)
	{
		return m_oDBConn_Nihil;
	}
	// Intentamos inicializarla ( si ya estamos conectados, no pasará nada ).

	ret=pDBConn->m_pDBConn->InitConnection();
	if (ret!= M4_SUCCESS){
		DisplayError(ret,pDBConn->m_pDBConn,ai_pLConn);
		if (ret== M4_ERROR){
			pDBConn->m_pDBConn->Disconnect();
			// Miramos a ver la criticidad.
			if (pStConProp->m_bIsCritical){
				DisableDDBB();
				
				// eh 202503
				/*
				ClEventIntInt oEventII(this,M4_NOTIF_DB_RYPTURE_CONNECTION,ai_idRConn,indLConn );
				DBRypture_Notifier.Signal(oEventII);
				*/
			}
			return m_oDBConn_Nihil;
		}
	}
	// OK. Estamos conectados.
	return *pDBConn;
}

m4return_t
ClConnectionManager::EndConnection(m4int32_t ai_idRConn,ClLConn *ai_pLConn){
	ClDBConn_cont &oDBConn=_FindDBConnection(ai_pLConn->GetIdLConn(),ai_idRConn);
	if (&oDBConn == &m_oDBConn_Nihil){
		return M4_ERROR;
	}
	if (oDBConn.m_pDBConn==0){
		// No hay nada que cerrar.
		return M4_SUCCESS;
	}
	if (oDBConn.m_pDBConn->Disconnect()!=M4_SUCCESS){
		return M4_ERROR;
	}
	return M4_SUCCESS;
}

ClDBConnection *
ClConnectionManager::FindDBConnection(m4int32_t ai_indLogConn,m4int32_t ai_idDBConn){
	ClDBConn_cont &oDBConn= _FindDBConnection(ai_indLogConn,ai_idDBConn);
	return oDBConn.m_pDBConn;
}

ClDBConn_cont &
ClConnectionManager::_FindDBConnection(m4int32_t ai_indLogicConn,m4int32_t ai_indRealConn){

// Buscamos en el mapa
	// Buscamos las conexiones físicas de la lógica que nos dan.
	itMapLogConn2DBConn itLc2DBc;
	itLc2DBc = m_mLogConn2DBConn.find(ai_indLogicConn);
	if (itLc2DBc==m_mLogConn2DBConn.end()){
		// No encontrada la conexion logica.
//		DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_INVALID_CONNECTION, ai_idDBConn);
		return m_oDBConn_Nihil;
	}
	
	// Buscamos en el mapa de conexiones fisicas.
	MapDBConnection &mapDBConn_tmp = (*itLc2DBc).second;
	
	itMapDBConnection itDBConn;
	itDBConn = mapDBConn_tmp.find(ai_indRealConn);
	if (itDBConn==mapDBConn_tmp.end()){
		// No existe ese indice de conexion fisica.
//		DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_INVALID_CONNECTION, ai_idDBConn);
		return m_oDBConn_Nihil;
	}
	// OK. Encontrada.
	return (*itDBConn).second;
}

m4return_t
ClConnectionManager::SetDBConnection(ClDBConnection *ai_pConn,m4int32_t ai_indLConn,m4int32_t ai_indRConn){

	ClDBConn_cont &oDBConn=_FindDBConnection(ai_indLConn,ai_indRConn);
	if (&oDBConn==&m_oDBConn_Nihil){
		return M4_ERROR;
	}
	if (oDBConn.m_pDBConn){
		// De momento retornamos error. Si alguna vez interesara podemos borrar la que hubiera y quedarnos con la nueva.
		return M4_ERROR;
	}

	oDBConn.m_pDBConn=ai_pConn;
	return M4_SUCCESS;
}
m4return_t
ClConnectionManager::End(){

	//Cuando desmontamos la paraeta nos cargamos también las conexiones físicas que han quedado per a llabor.
	if (m_pMpClKeyDynamicConnection != NULL) {
		//Se destruyen TODAS las conexiones dinamicas.
		DestroyDynamicConnection( M4_TRUE );
		delete m_pMpClKeyDynamicConnection;
		m_pMpClKeyDynamicConnection = NULL;
	}

	// Borramos todas las conexiones fisicas.
	// Iteramos por todas las conexiones logicas cerrando cada una de sus fisicas.
	itMapLogConn2DBConn itLc2DBc = m_mLogConn2DBConn.begin();
	itMapLogConn2DBConn itLc2DBc_end = m_mLogConn2DBConn.end();

	for (;itLc2DBc!=itLc2DBc_end;++itLc2DBc){
		// Para cada conexion logica cerramos sus fisicas.
		itMapDBConnection itDBConn= (*itLc2DBc).second.begin();
		itMapDBConnection itDBConn_end = (*itLc2DBc).second.end();
		for (;itDBConn!=itDBConn_end;++itDBConn){
			ClDBConn_cont &DBConn_ref = (*itDBConn).second;
			if (DBConn_ref.m_pDBConn){
				DBConn_ref.m_pDBConn->End();
				delete DBConn_ref.m_pDBConn;
				DBConn_ref.m_pDBConn = 0;
			}
		}
	}



	m_mLogConn2DBConn.clear();
	m_mGenericMapDBConn.clear();

	return M4_SUCCESS;
}
m4return_t
ClConnectionManager::Transact(eTransType_t ai_eTransType,m4int32_t ai_idRConn,ClLConn *ai_pLConn){
	// Buscamos la conexion fisica.
	m4return_t ret = M4_SUCCESS;
	ClDBConn_cont &oDBConn=_FindDBConnection(ai_pLConn->GetIdLConn(),ai_idRConn);
	if (&oDBConn == &m_oDBConn_Nihil){
		// No existe esta conexion.
		return M4_ERROR;
	}

	ret = TransactOneDBConn(oDBConn,ai_pLConn,ai_eTransType);
	if (ret!=M4_SUCCESS){
		DisplayError(ret,oDBConn.m_pDBConn,ai_pLConn);
		if (ret==M4_ERROR){
			// si habia dado error hacemos rollback, si habia sido warning seguimos.
			TransactOneDBConn(oDBConn,ai_pLConn,M4LDB_ROLLBACK);
		}
		return ret;
	}


	return ret;
}

m4return_t
ClConnectionManager::Transact(eTransType_t ai_eTransType,ClLConn *ai_pLConn){

	MapDBConnection *pmapDBConns=0;
	if (FindDBConns(ai_pLConn->GetIdLConn(),pmapDBConns)!=M4_TRUE){
		// No existia.
		return M4_ERROR;
	}
	if (pmapDBConns==0){
		// No existia.
		return M4_ERROR;
	}

	// Iteramos por todas las conexiones fisicas, que tengamos abiertas.
	if (Transact(*pmapDBConns,ai_eTransType,ai_pLConn,M4_FALSE)!=M4_SUCCESS){
		// Ha fallado. Por si acaso, hacemos Rollback en todas.
		Transact(*pmapDBConns,ai_eTransType,ai_pLConn,M4_TRUE);
		return M4_ERROR;
	}
	
	return M4_SUCCESS;
}		
m4return_t
ClConnectionManager::Transact (MapDBConnection &ai_MapDBConn,eTransType_t ai_eTransType,ClLConn *ai_pLConn,m4bool_t ai_bDoAll){
	// Iteramos por todas las conexiones fisicas, que tengamos abiertas.
	m4return_t ret = M4_SUCCESS, retDoAll = M4_SUCCESS;
	itMapDBConnection	itMapDBConn = ai_MapDBConn.begin();
	itMapDBConnection	itMapDBConn_end = ai_MapDBConn.end();

	//----------
	m4return_t retDyn = M4_SUCCESS;
	//Buscamos las conexiones dinámicas que estan asociadas a una conexion logica.
	VcClValueDynamicConnection oVectorValueDynamicConnection;
	oVectorValueDynamicConnection.reserve(10);

	//Buscamos todas las conexiones dinamicas que están asociadas a la conexion lógica que nos indican!.
	_FindKeyDynamicConnectionByLogicalConnection( ai_pLConn, oVectorValueDynamicConnection );

	//Ya tenemos el vector relleno con valores, ahora puedo obtener la conexion fisica.
	//Recorremos el vector de keys que cumplen que son de la misma sesion.
	ClValueDynamicConnection * pValueDynamicConnection = NULL;	
	for (itVcClValueDynamicConnection it = oVectorValueDynamicConnection.begin(); it != oVectorValueDynamicConnection.end(); it++) {
		//Establecemos valor.
		pValueDynamicConnection = (*it);
		ClLConn * pLConn = NULL;
		ClDBConn_cont * pDBConn = NULL;
		if ( (pValueDynamicConnection != NULL) 
			&& ((pLConn = pValueDynamicConnection->GetLogicalConnection()) != NULL)
			&& ((pDBConn = pValueDynamicConnection->GetDBConn()) != NULL) )
		{
			//pLConn deberá coincidir con ai_pLConn.
			retDyn = TransactOneDBConn(*pDBConn, pLConn, ai_eTransType);
			if (retDyn != M4_SUCCESS) {
				if (IsDDBBDisabled ()) {
					DUMP_LDBINSP_ERROR (pLConn, M4_CH_LDB_DDBB_DISABLED);
				}
				else{
					retDyn = DisplayError(retDyn, pDBConn->m_pDBConn, pLConn);
				}
				return retDyn;
			}

		}		
	}
	//----------

	for (;itMapDBConn!=itMapDBConn_end;itMapDBConn++){
		ClDBConn_cont &DBConn_ref = (*itMapDBConn).second;
		ret = TransactOneDBConn(DBConn_ref,ai_pLConn,ai_eTransType);
		if (ret!=M4_SUCCESS){
			if (IsDDBBDisabled ()) {
				DUMP_LDBINSP_ERROR (ai_pLConn, M4_CH_LDB_DDBB_DISABLED);
			}
			else{
				ret = DisplayError(ret,DBConn_ref.m_pDBConn,ai_pLConn);
			}
			if (ai_bDoAll){
				// Nos pueden pedir que hagamos rollback. Hay que hacerlo sobre todas, falle o no.
				// El Commit, lo paramos en cuanto falle una.
				retDoAll = ret;
				continue;
			}
			else{
				return ret;
			}
		}
	}
	return retDoAll;
}
m4return_t
ClConnectionManager::TransactOneDBConn(ClDBConn_cont &ai_DBConn,ClLConn *ai_pLConn,eTransType_t ai_eTransType){

	switch(ai_eTransType){
	case M4LDB_BEGIN_TRANS:
		return TransactBeginTrans(ai_DBConn,ai_pLConn);
	case M4LDB_COMMIT:
	case M4LDB_ROLLBACK:
		return TransactEndTrans(ai_DBConn,ai_pLConn,ai_eTransType);
	default:
		return M4_ERROR;
	}
}
m4return_t
ClConnectionManager::TransactBeginTrans(ClDBConn_cont &ai_DBConn,ClLConn *ai_pLConn){

	m4return_t ret = M4_SUCCESS;
	switch(ai_DBConn.m_eTransState){
	
	case M4DBW_WAIT:
	case M4DBW_CLOSE:
		// Lo marcamos para que se abra cuando haga falta.
		ai_DBConn.m_eTransState=M4DBW_WAIT;
		break;

	case M4DBW_OPEN:
	default:
		// Esta abierta. Lo dejamos.
		break;

	}
	return ret;
}
m4return_t
ClConnectionManager::TransactEndTrans(ClDBConn_cont &ai_DBConn,ClLConn *ai_pLConn,eTransType_t ai_eTransType){
	m4return_t ret = M4_SUCCESS;
	switch(ai_DBConn.m_eTransState){

	case M4DBW_OPEN:
		if (ai_DBConn.m_pDBConn){
			// Si estamos en Open cerramos la conexion.
			ret = ai_DBConn.m_pDBConn->Transact(ai_eTransType);
			if (ret!=M4_SUCCESS){
				DisplayError(ret,ai_DBConn.m_pDBConn,ai_pLConn);
				return ret;
			}
			ai_DBConn.m_eTransState=M4DBW_CLOSE;
		}
		break;
	
	case M4DBW_WAIT:
	case M4DBW_CLOSE:
	default:
		// Esta cerrada. Lo dejamos.
		ai_DBConn.m_eTransState=M4DBW_CLOSE;
		break;

	}
	return ret;
}

// Cogida tal cual de cllconn.cpp
m4return_t
ClConnectionManager::DisplayError (m4return_t ai_retcode, ClDBConnection * ai_pDBConn,ClLConn *ai_pLConn)
{
	m4pchar_t pcError = NULL;
	m4int32_t result = ai_retcode;
	m4int32_t iErrorCode;
	
	if ((ai_retcode == M4_SUCCESS) ||
		(ai_retcode == M4LDB_SQL_NO_DATA_FOUND)) {
		return ((m4return_t) ai_retcode);
	}
	
	while (result != M4LDB_SQL_NO_DATA_FOUND) {
		
		result = ai_pDBConn->GetErrorString (ai_retcode, iErrorCode, &pcError);

		if (result != M4LDB_SQL_NO_DATA_FOUND) {
			if (result == M4_ERROR) {
				DUMP_LDBINSP_ERROR (ai_pLConn, M4_CH_LDB_DBWRAP_DRIVER_ERROR);
				return (M4_ERROR);
			}
			else {
				if (ai_retcode == M4_WARNING) {
					DUMP_LDBINSP_WARNINGF (ai_pLConn, M4_CH_LDB_DDBB_WARNING_II, iErrorCode << pcError);
				}
				else {
					DUMP_LDBINSP_ERRORF (ai_pLConn, M4_CH_LDB_DDBB_ERROR_II, iErrorCode << pcError);
				}
				delete [] pcError ;
			}
		}
	} 
	
	return (ai_retcode);
}

m4bool_t
ClConnectionManager::FindDBConns(m4int32_t ai_indLConn,MapDBConnection *&ao_pmapDBConn){
	// Cogemos el mapa correspondiente a esa conexion logica.
	itMapLogConn2DBConn itMapL2D;
	itMapL2D = m_mLogConn2DBConn.find(ai_indLConn);
	if (itMapL2D == m_mLogConn2DBConn.end()){
		// No tenemos esa conexion logica.
		return M4_FALSE;
	}
	// Decolvemos la direccion del mapa de conexiones fisicas.
	ao_pmapDBConn = &((*itMapL2D).second);
	return M4_TRUE;
}

m4return_t
ClConnectionManager::DisconnectAll(ClLConn *ai_pLConn){
	MapDBConnection *pmapDBConns=0;
	if (FindDBConns(ai_pLConn->GetIdLConn(),pmapDBConns)!=M4_TRUE){
		// No existia.
		return M4_ERROR;
	}
	if (pmapDBConns==0){
		// No existia.
		return M4_ERROR;
	}
	return DisconnectAll(*pmapDBConns,ai_pLConn);
}

m4return_t
ClConnectionManager::DisconnectAll(MapDBConnection &ai_MapDBConn,ClLConn *ai_pLConn){
	itMapDBConnection	itMapDBConn = ai_MapDBConn.begin();
	itMapDBConnection	itMapDBConn_end = ai_MapDBConn.end();

	m4return_t ret = M4_SUCCESS,retAll = M4_SUCCESS;
	
	for (;itMapDBConn!=itMapDBConn_end;itMapDBConn++){
		// Desconectamos todas las conexiones.
		ClDBConn_cont &DBConn_ref = (*itMapDBConn).second;
		if (DBConn_ref.m_pDBConn){
			ret = DBConn_ref.m_pDBConn->Disconnect()!=M4_SUCCESS;
			if (ret!=M4_SUCCESS){
				ai_pLConn->m_pLogMCR = NULL;
				ai_pLConn->m_pLogAccess = NULL;
				ai_pLConn->m_hLogNode = NULL;
				ret = DisplayError(ret,DBConn_ref.m_pDBConn,ai_pLConn);
				retAll = ret;
			}
		}
	}

	return retAll;
}

m4return_t
ClConnectionManager::MarkAllToDisconnect(ClLConn *ai_pLConn)
{
	itMapLogConn2DBConn itLc2DBc = m_mLogConn2DBConn.begin();

	for (; itLc2DBc != m_mLogConn2DBConn.end(); ++itLc2DBc)
	{
		MapDBConnection &mapDBConn_tmp = (*itLc2DBc).second;
		itMapDBConnection itDBConn = mapDBConn_tmp.begin();

		for (; itDBConn != mapDBConn_tmp.end(); ++itDBConn)
		{
			ClDBConn_cont &oDBConn = (*itDBConn).second;

			if (&oDBConn == &m_oDBConn_Nihil || oDBConn.m_pDBConn == NULL){
				continue;
			}

			oDBConn.MarkToDisconnect();
		}
	}


	return M4_SUCCESS;
}

// Funcion ValidateConnection de ClLConn, cogida casi tal cual.
m4return_t
ClConnectionManager::ValidateConnectionProperties(m4int32_t ai_IdRConn,stConexionProperties &ai_stConProp,ClLConn *ai_pLConn)
{
	// Bug 0112361.
	//Para DB2 no validamos el servidor.
	//En el argumento tengo datos que se han leido del diccionario.
	m4pcchar_t pServer = NULL;
	m4pcchar_t pUser = NULL;
	m4int32_t iSizeStrServer = 0;

	if (ai_stConProp.m_eDBW != M4DBW_DB2_DDBB ) {
		pServer = strstr (ai_stConProp.m_stConnStr.c_str(),"SRVR=");
		iSizeStrServer = strlen ("SRVR=");

		if (!pServer) {
			pServer = strstr (ai_stConProp.m_stConnStr.c_str(),"SERVER=");
			iSizeStrServer = strlen ("SERVER=");
			if (!pServer) {
				DUMP_LDBINSP_ERROR (ai_pLConn, M4_CH_LDB_INVALID_SERVER_NAME);
				return (M4_ERROR);
			}
		}
		pServer += iSizeStrServer;
		pUser = strstr (pServer, "UID=");
	}
	else {
		pServer = "";
		pUser = strstr (ai_stConProp.m_stConnStr.c_str(), "UID=");
	}	
	

	if (!pUser) {
		DUMP_LDBINSP_ERROR (ai_pLConn, M4_CH_LDB_INVALID_USER_NAME);
		return (M4_ERROR);
	}

	pUser += 4;

	m4pcchar_t pPassword = strstr (pUser, "PWD=");

	if (!pPassword) {
		DUMP_LDBINSP_ERROR (ai_pLConn, M4_CH_LDB_INVALID_PASSWORD);
		return (M4_ERROR);
	}

	pPassword += 4;

#ifdef _M4LDB_DEBUG
		// OJO!!!!!.
		// Paso de la macro. No tengo globvars, ni quiero incluirme el clstatem.
		if (ai_pLConn->GetSystemDebugEnable() <= ai_pLConn->GetpLdb()->GetSysDebugLevel ())
		{
			ai_pLConn->m_pInspector->PrintFileDebug(ai_pLConn, "\nPhysical Connection: %d. Database: %s. Server Name: %.*s. User Name: %.*s.", ai_IdRConn, g_aDDBBs[((m4int32_t) ai_stConProp.m_eDBW) - 1], pUser - iSizeStrServer - pServer, pServer, pPassword - iSizeStrServer - pUser, pUser);
			ai_pLConn->m_pInspector->PrintFileDebugDetail (M4LDB_DETAIL_LEVEL_SHOW_ALWAYS, ai_pLConn, "\nPhysical Connection: %d. Database: %s. Server Name: %.*s. User Name: %.*s.", ai_IdRConn, g_aDDBBs[((m4int32_t) ai_stConProp.m_eDBW) - 1], pUser - iSizeStrServer - pServer, pServer, pPassword - iSizeStrServer - pUser, pUser);

			/* Bug 0148008
			Hay que mostrar la instancia de base de datos en SQLServer
			*/

			if( ai_stConProp.m_eDBW == M4DBW_SQL_SERVER_DDBB )
			{
				m4pcchar_t pDSNStart = strstr( ai_stConProp.m_stConnStr.c_str(), "DSN=" ) ;

				if( pDSNStart != NULL )
				{
					pDSNStart += strlen( "DSN=" ) ;

					m4pcchar_t pDSNEnd = strstr( pDSNStart, ";" ) ;

					ai_pLConn->m_pInspector->PrintFileDebug(ai_pLConn, " Instance: %.*s.", pDSNEnd - pDSNStart, pDSNStart) ;
					ai_pLConn->m_pInspector->PrintFileDebugDetail (M4LDB_DETAIL_LEVEL_SHOW_ALWAYS, ai_pLConn, " Instance: %.*s.", pDSNEnd - pDSNStart, pDSNStart);
				}
			}
		}
#endif

	return (M4_SUCCESS);
}


ClDBConnection *
ClConnectionManager::InitTransaction(m4int32_t ai_idRConn,ClLConn *ai_pLConn){
	m4return_t ret=M4_SUCCESS;
	ClDBConn_cont &oDBConn = _InitConnection(ai_idRConn,ai_pLConn);
	if (&oDBConn == &m_oDBConn_Nihil){
		return 0;
	}

	if (oDBConn.m_eTransState==M4DBW_WAIT){
		ret = oDBConn.m_pDBConn->InitTransaction();
		if (ret!=M4_SUCCESS){
			DisplayError(ret,oDBConn.m_pDBConn,ai_pLConn);
			// Puede ser un Warning.
			if (ret==M4_ERROR){
				oDBConn.m_pDBConn->Disconnect();
				return 0;
			}
		}
		oDBConn.m_eTransState=M4DBW_OPEN;

	}
	return oDBConn.m_pDBConn;
}
m4return_t
ClConnectionManager::RefreshInvalidConnections(m4int32_t ai_indDBConn,ClLConn *ai_pLConn){
	m4return_t ret=M4_SUCCESS;
	// Solo intentamos recuperar la conexion que nos digan.
	ClDBConn_cont &oDBConn = _FindDBConnection(ai_pLConn->GetIdLConn(),ai_indDBConn);
	if (&oDBConn == &m_oDBConn_Nihil){
		// No la hemos encontrado.
		return M4_ERROR;
	}
	if (oDBConn.m_pDBConn==0){
		// ¿quien la ha borrado?
		return M4_ERROR;
	}

	oDBConn.m_pDBConn->AllowOperationsWhenDDBBIsDisabled (M4_TRUE);

	ret= oDBConn.m_pDBConn->InitConnection();

	if (ret==M4_ERROR){
		DisplayError(ret,oDBConn.m_pDBConn,ai_pLConn);
		// Lo hemos intentado, pero nada.
		oDBConn.m_pDBConn->Disconnect();
		oDBConn.m_pDBConn->AllowOperationsWhenDDBBIsDisabled (M4_FALSE);
		return ret;
	}

	EnableDDBB ();

	return ret;
}

m4return_t ClConnectionManager::DisconnectAllDynamicConnections( void )
{
	return _DisconnectAllDynamicConnections();
}

m4return_t ClConnectionManager::_DisconnectAllDynamicConnections( void )
{
	m4return_t ret = M4_SUCCESS,retAll = M4_SUCCESS;

	if (m_pMpClKeyDynamicConnection != NULL) {
		ClValueDynamicConnection * pValueDynamicConnection = NULL;
		ClDBConn_cont * pDBConn = NULL;

		for(itMpClKeyDynamicConnection it = m_pMpClKeyDynamicConnection->begin(); it != m_pMpClKeyDynamicConnection->end(); it++) {
			pValueDynamicConnection = (*it).second;
			if (pValueDynamicConnection != NULL) {
				pDBConn = pValueDynamicConnection->GetDBConn();
				if (pDBConn) {
					if (pDBConn->m_pDBConn) {
						ret = pDBConn->m_pDBConn->Disconnect();
						if (ret != M4_SUCCESS) {
							retAll = ret;
						}
					}
				}
			}
		}
	}
	return retAll;
}

m4return_t ClConnectionManager::DestroyDynamicConnection( m4bool_t ai_bAllDynamicConnections )
{
	//El argumento me indica si son Todas o solo las ReadyToFree.
	if (m_pMpClKeyDynamicConnection != NULL) {
		ClValueDynamicConnection * pValueDynamicConnection = NULL;
		ClDBConn_cont * pDBConn = NULL;

		itMpClKeyDynamicConnection it = m_pMpClKeyDynamicConnection->begin();
		while (it != m_pMpClKeyDynamicConnection->end()) {
			pValueDynamicConnection = (*it).second;
			if (pValueDynamicConnection != NULL) {
				if ((ai_bAllDynamicConnections == M4_TRUE) || (pValueDynamicConnection->GetReadyToFree() == M4_TRUE)) {
					pDBConn = pValueDynamicConnection->GetDBConn();
					if (pDBConn) {
						//Contexto importante:
						//Las cachés solo se eliminan cuando NADIE tiene conexiones logicas, por lo tanto cualquier operación
						//con caches puede realizarse aquí sin problemas de bloqueos!.
						//La cache tiene el statement y cada uno de sus ClCursor tiene una referencia a la conexión física.
						//Cuando se limpian caches se invoca a la destrucción de cada cursor. El miembro puntero es el mismo
						//que ahora estoy liberando, por lo tanto hace falta desvincular la conexión fisica del cursor
						//que se eliminará en la limpieza de caches!.

						//La desvinculacion de la conexion fisica con el statement la podemos realizar en el destructor
						//de la conexion fisica, recorriendo los cursores con los que está asociado y estableciendo a NULL
						//el miembro del cursor donde indica la conexion fisica.
						//Nota: Si no hiciera DestroyConnection del contenedor no liberaría la conexion física pero
						//al liberar la cache tampoco se libera la conexion fisica por lo tanto se acumularian.						
						if (pDBConn->m_pDBConn) {
							pDBConn->m_pDBConn->End();
						}
						pDBConn->DestroyConnection();
						delete pDBConn;						
						delete pValueDynamicConnection;
						//Al hacer erase se invoca al destructor de ClKeyDynamicConnection.
						//Optimizacion!:
						//Si estamos eliminando todas NO eliminamos ahora, lo haremos al final!!.
						if (ai_bAllDynamicConnections == M4_FALSE) {
							m_pMpClKeyDynamicConnection->erase(it);
							//Ojo pq el erase descuajeringa el iterador. Por lo tanto establezco el iterador a la nueva 
							//posicion. Hay un problema: si hay muchos elementos este sistema es lento.
							//Sin embargo el numero de conexiones no será alto y tampoco muy comun la destruccion.
							it = m_pMpClKeyDynamicConnection->begin();
							continue;
						}						
					}
				}
			}
			it++;
		}
		//Por la optimizacion, ahora es cuando las eliminamos todas de golpe.
		if (ai_bAllDynamicConnections == M4_TRUE) {
			m_pMpClKeyDynamicConnection->clear();
		}
	}
	return M4_SUCCESS;
}

m4return_t ClConnectionManager::InsertIntoMapDynamicConnection( const ClKeyDynamicConnection & ai_oKeyDynamicConnection, ClValueDynamicConnection * ai_pValueDynamicConnection )
{
	if (m_pMpClKeyDynamicConnection != NULL) {
		pair<itMpClKeyDynamicConnection, bool> oMap2xPair;
		oMap2xPair = m_pMpClKeyDynamicConnection->insert( MpClKeyDynamicConnection::value_type ( ai_oKeyDynamicConnection, ai_pValueDynamicConnection ) );
		if (oMap2xPair.second == M4_TRUE) {
			//No había un id repetido
			return M4_SUCCESS;
		}		
	}

	return M4_ERROR;
}

void ClConnectionManager::EraseFromMapDynamicConnection( const ClKeyDynamicConnection & ai_oKeyDynamicConnection )
{
	if (m_pMpClKeyDynamicConnection != NULL) {
		m_pMpClKeyDynamicConnection->erase( ai_oKeyDynamicConnection );
	}
}

m4return_t ClConnectionManager::EraseAnyKeyFromMapDynamicConnection( const ClKeyDynamicConnection & ai_oKeyDynamicConnection )
{
	//Elimina todas las ocurrencias que coincidan con algun valor de clave.
	if (m_pMpClKeyDynamicConnection != NULL) {
		itMpClKeyDynamicConnection it = m_pMpClKeyDynamicConnection->begin();
		while (it != m_pMpClKeyDynamicConnection->end()) {
			const ClKeyDynamicConnection & oRefKeyDynamicConnection = (*it).first;
			if (oRefKeyDynamicConnection.CompareAnyKey( ai_oKeyDynamicConnection ) == M4_TRUE)	{
				m_pMpClKeyDynamicConnection->erase( it );
				it = m_pMpClKeyDynamicConnection->begin();
				continue;
			}
			it++;
		}
	}
	return M4_SUCCESS;
}

//Retornamos en un vector los punteros de las claves que se han encontrado que cumplen cierta condicion (Id sesion).
void ClConnectionManager::_FindKeyDynamicConnectionByIdSession( m4uint64_t ai_iIdSession, VcClValueDynamicConnection & ao_oVectorValueDynamicConnection )
{
	if (m_pMpClKeyDynamicConnection != NULL) {
		ClKeyDynamicConnection oKeyDynamicConnection( ai_iIdSession, M4LDB_EMPTY, M4LDB_NULL_INDEX );

		for (itMpClKeyDynamicConnection it = m_pMpClKeyDynamicConnection->begin(); it != m_pMpClKeyDynamicConnection->end(); it++)	{
			if ((*it).first.CompareIdSessionKey( oKeyDynamicConnection ) == M4_TRUE) {
				//Lo hemos encontrado en el mapa.
				//Importante: En el vector insertamos un puntero al valor.				
				ao_oVectorValueDynamicConnection.push_back((*it).second);
			}
		}
	}
}

//Retornamos en un vector los punteros de las claves que se han encontrado que cumplen cierta condicion (Id User).
void ClConnectionManager::_FindKeyDynamicConnectionByIdUser( const string & ai_sUser, VcClValueDynamicConnection & ao_oVectorValueDynamicConnection )
{
	if (m_pMpClKeyDynamicConnection != NULL) {
		ClKeyDynamicConnection oKeyDynamicConnection( 0, ai_sUser, M4LDB_NULL_INDEX );

		for (itMpClKeyDynamicConnection it = m_pMpClKeyDynamicConnection->begin(); it != m_pMpClKeyDynamicConnection->end(); it++)	{
			if ((*it).first.CompareIdUserKey( oKeyDynamicConnection ) == M4_TRUE) {
				//Lo hemos encontrado en el mapa.
				//Importante: En el vector insertamos un puntero al valor.				
				ao_oVectorValueDynamicConnection.push_back((*it).second);
			}
		}
	}
}

//Retornamos en un vector los punteros de las claves que se han encontrado que cumplen cierta condicion (Conexion lógica).
void ClConnectionManager::_FindKeyDynamicConnectionByLogicalConnection( const ClLConn * ai_pLConn, VcClValueDynamicConnection & ao_oVectorValueDynamicConnection )
{
	if (m_pMpClKeyDynamicConnection != NULL) {

		for (itMpClKeyDynamicConnection it = m_pMpClKeyDynamicConnection->begin(); it != m_pMpClKeyDynamicConnection->end(); it++)	{
			//El second contiene el puntero al objeto value. El objeto value tienen la conexion logica.
			ClValueDynamicConnection * pValueDynamicConnection = (*it).second;
			if (pValueDynamicConnection != NULL) {
				if (pValueDynamicConnection->GetLogicalConnection() == ai_pLConn) {
					//Lo hemos encontrado en el mapa. Insertamos el puntero al valor.					
					ao_oVectorValueDynamicConnection.push_back((*it).second);
				}
			}
		}
	}
}

//-------------------------------------------------------------------------
//Busquedas en el mapa por Key. Retornando conexion física.
//-------------------------------------------------------------------------
ClValueDynamicConnection * ClConnectionManager::FindIntoMapDynamicConnectionByKey( const ClKeyDynamicConnection & ai_oKeyDynamicConnection )
{
	if (m_pMpClKeyDynamicConnection != NULL) {
		//Busca segun el operador == definido. Se supone que se ha diseñado para que la comparacion sea por la PK!.
		itMpClKeyDynamicConnection it = m_pMpClKeyDynamicConnection->find( ai_oKeyDynamicConnection );
		if (it != m_pMpClKeyDynamicConnection->end())
		{
			//Lo hemos encontrado en el mapa.
			//Rellenamos el puntero de salida con la clave encontrada (No es la misma que el primer argumento, lo que se
			//garantiza es que son iguales segun el operador ==).
			return (*it).second;
		}
	}

	//No lo hemos encontrado. Para evitar problemas es importante devolver NULL en el puntero de salida.
	return NULL;
}

//-------------------------------------------------------------------------
//FIN Busquedas en el mapa por Key, DBConn, Sesion, Usuario. Retornando conexion física.
//-------------------------------------------------------------------------

//Le indicamos a partir de que indice empezamos a buscar.
void ClConnectionManager::SetReadyToFreeDynamicConnectionByIdSession( m4uint64_t ai_iIdSession, m4bool_t ai_bReadyToFree )
{
	VcClValueDynamicConnection oVectorValueDynamicConnection;
	oVectorValueDynamicConnection.reserve(10);

	//Buscamos todas las conexiones dinamicas que puede tener una misma sesion!.
	_FindKeyDynamicConnectionByIdSession( ai_iIdSession, oVectorValueDynamicConnection );

	//Recorremos el vector de keys que cumplen que son de la misma sesion.
	ClValueDynamicConnection * pValueDynamicConnection = NULL;	
	for (itVcClValueDynamicConnection it = oVectorValueDynamicConnection.begin(); it != oVectorValueDynamicConnection.end(); it++) {
		//Establecemos valor.
		pValueDynamicConnection = (*it);
		if (pValueDynamicConnection != NULL) {
			pValueDynamicConnection->SetReadyToFree(ai_bReadyToFree);
		}		
	}
}


//-------------------------------------------------------------------------
// Funciones relacionadas con el relleno de la cadena de conexion de la conexion dinamica.
//-------------------------------------------------------------------------
//Funcion estatica que me indica si un determinado tipo se considera dinamica.
m4bool_t ClConnectionManager::IsDynamicConnection( eConnectionType_t ai_eConnectionType )
{
	if ((ai_eConnectionType == M4LDB_CONN_TYPE_DYNAMIC) || (ai_eConnectionType == M4LDB_CONN_TYPE_DYNAMIC_2X)) {
			return M4_TRUE;
	}

	return M4_FALSE;

}

m4bool_t ClConnectionManager::IsDynamicConnectionByIdRConn( m4int32_t ai_idRConn )
{
	//Obtenemos las propiedades y comprobamos si es dinamica.
	itMapConnProp	itConnProp;
	stConexionProperties * pstConProp = NULL;
	itConnProp = m_mGenericProperies.find(ai_idRConn);
	if (itConnProp != m_mGenericProperies.end()) {
		pstConProp = &(*itConnProp).second;
		if (pstConProp != NULL) {
			if (IsDynamicConnection(pstConProp->m_eConnectionType) == M4_TRUE) {
				return M4_TRUE;
			}
		}
	}

	return M4_FALSE;
}

m4return_t ClConnectionManager::GetDynamicConnectionStringByConnectionType( eConnectionType_t ai_eConnectionType, ClLConn *ai_pLConn, const ClKeyDynamicConnection & ai_oKeyDynamicConnection, stConexionProperties * ai_pStConProp )
{
	m4return_t result = M4_SUCCESS;

	if ((ai_pLConn == NULL) || (ai_pStConProp == NULL)) {
		return M4_ERROR;
	}

	//En funcion del tipo de conexion dinamica la password se obtiene de forma diferente.
	//Se invocaran a las funciones que rellenaran la cadena de conexion con la password adecuada!.
	switch (ai_eConnectionType)
	{
		case M4LDB_CONN_TYPE_NORMAL:
			break;

		case M4LDB_CONN_TYPE_DYNAMIC:
			result = _GetDynamicConnectionString( ai_pLConn, ai_oKeyDynamicConnection, ai_pStConProp );
			break;

		case M4LDB_CONN_TYPE_DYNAMIC_2X:
			result = _GetDynamicConnectionString2x( ai_pLConn, ai_oKeyDynamicConnection, ai_pStConProp );
			break;
		default:
			result = M4_ERROR;			
	}

	if (result == M4_ERROR) {
		//El usuario no ha podido obtener una conexion dinamica.
		DUMP_LDBINSP_ERRORF (ai_pLConn, M4_CH_LDB_DYN_GETTING_DYNAMIC_CONNECTION, ai_oKeyDynamicConnection.GetIdUser());
	}

	return result;
}

m4return_t ClConnectionManager::_GetDynamicConnectionString( ClLConn *ai_pLConn, const ClKeyDynamicConnection & ai_oKeyDynamicConnection, stConexionProperties * ai_pStConProp )
{
	//En principio no haremos nada en este caso!.
	return M4_SUCCESS;
}

//Explicació:
//	Tenemos una cadena de conexion especial que nos permite realizar la conexion y validar usuario y password de aplicacion.
//	A partir de la cadena de conexion y el usuario de aplicacion leemos de BD de una tabla y obtendremos el usuario y la password
//	de BD encriptada.
//	La password la desencriptamos podremos construir una cadena UsuarioBD@PasswordBD que es la que devolveremos!.

//La select que se lanza para obtener usuario, password es:
//M4LDB_USER_VALIDATE_SELECT_DYNAMIC_2X = "SELECT A.USUARIO_DB, B.PASSWORD_VALIDATE, B.PASSWORD_APP FROM M4_USUARIOS A, M4_INFO_USER_ST32 B WHERE A.ID_USUARIO_M4 = B.ID_USER_APP AND A.ID_USUARIO_M4 = '"
//Sin password de aplicacion!.
//Español (3):
//m4char_t M4LDB_USER_VALIDATE_SELECT_DYNAMIC_2X[M4LDB_SIZE_DEFAULT_STRING] = "SELECT A.USUARIO_DB, B.PASSWORD_VALIDATE, B.IS_LOCKED, B.CHANGE_PERIOD, A.FEC_ULT_CAMBIO  FROM M4_USUARIOS A, M4_INFO_USER_ST32 B WHERE A.ID_USUARIO_M4 = B.ID_USER_APP AND A.ID_USUARIO_M4 = ?";
//Portugués (5, Alemán no está):
//m4char_t M4LDB_USER_VALIDATE_SELECT_DYNAMIC_2X[M4LDB_SIZE_DEFAULT_STRING] = "SELECT A.UTILIZ_DB, B.PASSWORD_VALIDATE, B.IS_LOCKED, B.CHANGE_PERIOD, A.DAT_ULT_ALTER FROM M4_UTILIZAD A, M4_INFO_USER_ST32 B WHERE A.ID_UTILIZ_M4 = B.ID_USER_APP AND A.ID_UTILIZ_M4 = ?";
//Donde:
//	A.USUARIO_DB= Usuario de BD
//	B.PASSWORD_VALIDATE = Password de BD
//	B.PASSWORD_APP = Password de aplicacion

//Argumentos:
//	(IN)	ai_pLConn: Conexion logica, para mostrar errores...
//	(IN)	ai_oKeyDynamicConnection: Tenemos el indice de la conexion fisica y el usuario de aplicacion.
//	(IN)	ai_sUserApp: Usuario de aplicacion.
//	(OUT)	ai_pStConProp: La utilizamos tanto de entrada como de salida.
//				- Como argumento de entrada tendremos las propiedades de la conexion donde se va a ejecutar 
//				la sentencia para obtener la pwd.
//				- Como argumento de salida rellenaremos el usuario y pwd definitivo para la conexion.
//				Importante!: Todo esto esta en el contexto de una conexion lógica!.

m4return_t ClConnectionManager::_GetDynamicConnectionString2x( ClLConn * ai_pLConn, const ClKeyDynamicConnection & ai_oKeyDynamicConnection, stConexionProperties * ai_pStConProp )
{
	//Si la conexion dinamica es 2x, tendremos que lanzar una select.
	//La select va filtrada por el usuario de aplicacion y obtendremos el usuario/password de BD.
	//Con esa informacion tendremos que rellenar la cadena de conexion definitiva!.

	m4return_t ret = M4_SUCCESS;
	string sUserDB ("");
	string sPasswordDB ("");	

	//---------------------------------------------------
	//Creamos una conexion física con las propiedades de la conexion ai_idRConn!.
	ClDBConn_cont * pDBConn = new ClDBConn_cont;

	// La creamos. En realidad nunca la creación nunca da error.
	if (pDBConn->CreateConnection() != M4_SUCCESS) {
		pDBConn->DestroyConnection();		
		delete pDBConn;
		#ifdef _M4LDB_DEBUG
			if (ai_pLConn->GetSystemDebugEnable() <= ai_pLConn->GetpLdb()->GetSysDebugLevel ()) {
				ai_pLConn->m_pInspector->PrintFileDebug( ai_pLConn, "ERROR: Dynamic Connection 2x (Create).\n" );
				ai_pLConn->m_pInspector->PrintFileDebugDetail( M4LDB_DETAIL_LEVEL_SHOW_ALWAYS, ai_pLConn, "ERROR: Dynamic Connection 2x (Create).\n" );
			}
		#endif
		return M4_ERROR;
	}
	//Reservamos la conexion de validacion!		
	if (pDBConn->m_pDBConn->Init(ai_oKeyDynamicConnection.GetIdRConn(), ai_pStConProp) != M4_SUCCESS){
		pDBConn->DestroyConnection();		
		delete pDBConn;		
		#ifdef _M4LDB_DEBUG
			if (ai_pLConn->GetSystemDebugEnable() <= ai_pLConn->GetpLdb()->GetSysDebugLevel ()) {
				ai_pLConn->m_pInspector->PrintFileDebug( ai_pLConn, "ERROR: Dynamic Connection 2x (Init).\n" );
				ai_pLConn->m_pInspector->PrintFileDebugDetail( M4LDB_DETAIL_LEVEL_SHOW_ALWAYS, ai_pLConn, "ERROR: Dynamic Connection 2x (Init).\n" );
			}
		#endif
		return M4_ERROR;
	}
	//---------------------------------------------------

	ret = pDBConn->m_pDBConn->InitConnection();
	if (ret == M4_ERROR) {
		//Destruimos la conexion fisica creada.
		pDBConn->DestroyConnection();
		delete pDBConn;
		#ifdef _M4LDB_DEBUG
			if (ai_pLConn->GetSystemDebugEnable() <= ai_pLConn->GetpLdb()->GetSysDebugLevel ()) {
				ai_pLConn->m_pInspector->PrintFileDebug( ai_pLConn, "ERROR: Dynamic Connection 2x (InitConnection).\n" );
				ai_pLConn->m_pInspector->PrintFileDebugDetail( M4LDB_DETAIL_LEVEL_SHOW_ALWAYS, ai_pLConn, "ERROR: Dynamic Connection 2x (InitConnection).\n" );
			}
		#endif
		return M4_ERROR;
	}

	//---------------------------------------------------
	//Ejecutamos la sentencia para que dado un identificador de usuario de aplicacion obtener usuario/password de BD.	
	ret = _ExecuteStatementInConnectionDB ( ai_pLConn, pDBConn->m_pDBConn, ai_oKeyDynamicConnection, ai_pStConProp, sUserDB, sPasswordDB );
	if (ret == M4_ERROR) {
		//Destruimos la conexion fisica creada.
		pDBConn->DestroyConnection();
		delete pDBConn;
		return ret;
	}

	//Tendremos que modificar la cadena de conexion con la informacion obtenida del usuario y password.
	ret = FillDynamicConnectionString( ai_pStConProp->m_stConnStr, &sUserDB, &sPasswordDB );
	if (ret == M4_ERROR) {
		DUMP_LDBINSP_ERRORF (ai_pLConn, M4_CH_LDB_INVALID_DB_CONNECTION, ai_oKeyDynamicConnection.GetIdRConn());
	}

	//---------------------------------------------------
	//Destruimos la conexion fisica creada.
	pDBConn->DestroyConnection();
	delete pDBConn;
	//---------------------------------------------------

	return ret;
}

m4return_t  ClConnectionManager::_ExecuteStatementInConnectionDB( ClLConn * ai_pLConn, ClDBConnection * ai_pDBConn, const ClKeyDynamicConnection & ai_oKeyDynamicConnection, const stConexionProperties * ai_pStConProp, string & ao_sUserDB, string & ao_sPasswordDB )
{
	m4return_t ret = M4_SUCCESS;
		
	m4bool_t bReallyInitialized = M4_FALSE;

	m4char_t pParamUser[ M4LDB_SIZE_DEFAULT_STRING + 1];
	m4double_t dIsLocked = 0;
	m4double_t dChangePeriod = 0;	
	m4date_t dFecUltCambio = 0;

	// UNICODE DB
	m4pchar_t pcUserDB = NULL;
	m4pchar_t pcPwdDB = NULL;
	m4pchar_t pcIsLocked = NULL;
	m4pchar_t pcChangePeriod = NULL;
	m4pchar_t pcFecUltCambio = NULL;
	m4int32_t iUserDBMaxLen = 0;
	m4int32_t iPwdDBMaxLen = 0;
	m4int32_t iIsLockedMaxLen = 0;
	m4int32_t iChangePeriodMaxLen = 0;
	m4int32_t iFecUltCambioMaxLen = 0;

	m4int32_t lMaxLenFld = M4LDB_SQL_NTS;
	m4int32_t lLengthUserDB = M4LDB_SQL_NTS;
	m4int32_t lLengthPwdDB = M4LDB_SQL_NTS;
	m4int32_t lLengthIsLocked = 0;
	m4int32_t lLengthChangePeriod = 0;
	m4int32_t lLengthFecUltCambio = 0;

	strcpy(pParamUser, ai_oKeyDynamicConnection.GetIdUser().c_str());

	//--------------------------------------------------------------------------------
	//Obtememos la cadena select que se va a lanzar.				
	string sSelectGetDBUser ("");
	m4id_t pValue = NULL;
	ClLogonAdaptor * pLA = NULL;
	ClAccessRecordSet * pRS = NULL;
	if ( ((pLA = ai_pLConn->m_pLogonAdaptor) != NULL) 
		&& ((pRS = pLA->GetSessionRecordSet()) != NULL) ){

			pValue = M4LDB_ID (pRS->Current.Item["SELECT_TO_GET_DB_USER_2X"].Value.GetWithOutSec ());
			if (pValue != NULL) {				
				sSelectGetDBUser.append( pValue );
			}
	}
	//--------------------------------------------------------------------------------

	//----------------------------------
	//Es la select que da error.
	string sSelectError ( sSelectGetDBUser );
	m4int32_t iParamPos = sSelectError.find("?");
	if (iParamPos == M4LDB_NULL_INDEX ) {
		string sWhereIsError ("");
		if (sSelectError.size() == 0) {
			sWhereIsError.append( "<null>" );
		}
		else {
			sWhereIsError.append( sSelectError );
		}
		DUMP_LDBINSP_ERRORF ( ai_pLConn, M4_CH_LDB_DYN_INVALID_STATEMENT, sWhereIsError);
		return M4_ERROR;
	}
	sSelectError =  sSelectError.erase(iParamPos, 1);
	sSelectError.append("'");
	sSelectError.append( pParamUser );
	sSelectError.append("'");
	//----------------------------------
	
	//----------------------------------
	//Hacemos copia de la cadena de conexion.
	string sConnectionStringError( ai_pStConProp->m_stConnStr );
	//No tocamos el usuario de base de datos que tenga la cadena de conexion.
	string sPasswordDB ("******");
	ret = FillDynamicConnectionString( sConnectionStringError, NULL, &sPasswordDB );
	if (ret == M4_ERROR) {		
		DUMP_LDBINSP_ERRORF (ai_pLConn, M4_CH_LDB_INVALID_DB_CONNECTION, ai_oKeyDynamicConnection.GetIdRConn());
		return M4_ERROR;
	}
	//----------------------------------

	//----------------------------------
	//Construimos una cadena para mostrar información, tanto la select como la cadena de conexion.
	string sComunErrorArg ("User application: ");
	sComunErrorArg.append( pParamUser );
	sComunErrorArg.append(". ConnectionString: ");
	sComunErrorArg.append( sConnectionStringError );
	sComunErrorArg.append( ". Validate select: ");
	sComunErrorArg.append( sSelectError );
	sComunErrorArg.append( ". ");
	//----------------------------------

	//Creamos el cursor donde lanzar la sentencia.
	ClCursor * pClCursor = new ClCursor();	
	ret = pClCursor->Open();
	if (ret == M4_ERROR) {
		_DisplayErrorCursorDynamic( ai_pLConn, M4_CH_LDB_DYN_INTERNAL_ERROR, pClCursor, "ClConnectionManager::_ExecuteStatementInConnectionDB (Open Cursor)", sComunErrorArg);
		delete pClCursor;
		return ret;
	}

	ret = pClCursor->Prepare( (m4char_t *) sSelectGetDBUser.c_str(), ai_pStConProp->m_eDBW );
	if (ret == M4_ERROR) {
		_DisplayErrorCursorDynamic( ai_pLConn, M4_CH_LDB_DYN_INTERNAL_ERROR, pClCursor, "ClConnectionManager::_ExecuteStatementInConnectionDB (Prepare)", sComunErrorArg);
		delete pClCursor;
		return ret;
	}

	//Los argumentos son:
	//	1. (IN)		Parametro que se binda.Empieza en 1.
	//	2. (IN)		Buffer donde tenemos el valor del parametro.
	//	3. (IN)		Tamaño que se supone que tiene la columna en BD.
	//	4. (IN)		Tipo de dato que es el parametro.
	//	5. (IN)		Es la escala.
	//	6. (IN)		Puede tener varios valores:
	//					a) Un puntero al tamaño que tiene el buffer (se ignora a no ser que la columna sea texto o long).
	//					b) SQL_NTS (-3). El parametro es null terminado.
	//					c) SQL_NULl_DATA: El parametro es nulo.
	//					d) Otros (procedures, o pq sea un valor muy grande...)

	ret = pClCursor->BindParam (1, pParamUser, NULL, M4LDB_SIZE_DEFAULT_STRING + 1, M4LDB_M4_UNICODE_TYPE(M4CL_M4_TYPE_FIXED_STRING), 0, &lMaxLenFld);
	if (ret == M4_ERROR) {
		_DisplayErrorCursorDynamic( ai_pLConn, M4_CH_LDB_DYN_INTERNAL_ERROR, pClCursor, "ClConnectionManager::_ExecuteStatementInConnectionDB (BindParam User Application)", sComunErrorArg);
		delete pClCursor;
		return ret;
	}

	//Los argumentos son:
	//	1. (IN)		Columna que se binda.Empieza en 1.
	//	2. (OUT)	Buffer donde se dejará la información.
	//	3. (IN)		Tamaño máximo en bytes que se ha reservado para el buffer.
	//	4. (IN)		Tipo de dato que se trata.
	//	5. (IN)		Puede tener varios valores:
	//					a) Un puntero al tamaño que tiene el buffer (se ignora a no ser que la columna sea texto o long).
	//					b) SQL_NTS (-3). El parametro es nul terminado.
	//					c) SQL_NULl_DATA: El parametro es nulo.
	//					d) Otros (procedimiento, o pq sea un valor muy grande...)
	//	6. (IN)		Podría obviarse, es util cuando la precision del item puede ser diferente al del field.
	//Si no hay valor en la columna IsLocked entonces lLengthIsLocked será -1.
	// UNICODE DB

	iUserDBMaxLen = M4LDB_SIZE_DEFAULT_STRING;
	pcUserDB = new m4char_t[ iUserDBMaxLen + 1 ];

	ret = pClCursor->BindResult (1, &pcUserDB, &iUserDBMaxLen, M4LDB_M4_UNICODE_TYPE(M4CL_M4_TYPE_FIXED_STRING), &lLengthUserDB, lMaxLenFld);
	if (ret == M4_ERROR) {
		_DisplayErrorCursorDynamic( ai_pLConn, M4_CH_LDB_DYN_INTERNAL_ERROR, pClCursor, "ClConnectionManager::_ExecuteStatementInConnectionDB (BindColumn User Database)", sComunErrorArg);
		delete [] pcUserDB;
		delete pClCursor;
		return ret;
	}

	iPwdDBMaxLen = M4LDB_SIZE_DEFAULT_STRING;
	pcPwdDB = new m4char_t[ iPwdDBMaxLen + 1 ];

	ret = pClCursor->BindResult (2, &pcPwdDB, &iPwdDBMaxLen, M4LDB_M4_UNICODE_TYPE(M4CL_M4_TYPE_FIXED_STRING), &lLengthPwdDB, lMaxLenFld);
	if (ret == M4_ERROR) {
		_DisplayErrorCursorDynamic( ai_pLConn, M4_CH_LDB_DYN_INTERNAL_ERROR, pClCursor, "ClConnectionManager::_ExecuteStatementInConnectionDB (BindColumn Password User Database)", sComunErrorArg);
		delete [] pcUserDB;
		delete [] pcPwdDB;
		delete pClCursor;
		return ret;
	}

	pcIsLocked = (m4pchar_t) &dIsLocked;
	iIsLockedMaxLen = M4LDB_SIZE_DOUBLE;
	ret = pClCursor->BindResult (3, &pcIsLocked, &iIsLockedMaxLen, M4CL_M4_TYPE_NUMBER, &lLengthIsLocked);
	if (ret == M4_ERROR) {
		_DisplayErrorCursorDynamic( ai_pLConn, M4_CH_LDB_DYN_INTERNAL_ERROR, pClCursor, "ClConnectionManager::_ExecuteStatementInConnectionDB (BindColumn Is Locked)", sComunErrorArg);
		delete [] pcUserDB;
		delete [] pcPwdDB;
		delete pClCursor;
		return ret;
	}

	//Si no hay valor en la columna Change Period entonces lLengthChangePeriod será -1.
	pcChangePeriod = (m4pchar_t) &dChangePeriod;
	iChangePeriodMaxLen = M4LDB_SIZE_DOUBLE;
	ret = pClCursor->BindResult (4, &pcChangePeriod, &iChangePeriodMaxLen, M4CL_M4_TYPE_NUMBER, &lLengthChangePeriod);
	if (ret == M4_ERROR) {
		_DisplayErrorCursorDynamic( ai_pLConn, M4_CH_LDB_DYN_INTERNAL_ERROR, pClCursor, "ClConnectionManager::_ExecuteStatementInConnectionDB (BindColumn Change Period)", sComunErrorArg);
		delete [] pcUserDB;
		delete [] pcPwdDB;
		delete pClCursor;
		return ret;
	}

	//Si no hay valor en la columna fecha de ultimo cambio entonces lLengthFecUltCambio será -1.
	pcFecUltCambio = (m4pchar_t) &dFecUltCambio;
	iFecUltCambioMaxLen = M4LDB_SIZE_TIMESTAMP;
	ret = pClCursor->BindResult (5, &pcFecUltCambio, &iFecUltCambioMaxLen, M4CL_M4_TYPE_TIMESTAMP, &lLengthFecUltCambio);
	if (ret == M4_ERROR) {
		_DisplayErrorCursorDynamic( ai_pLConn, M4_CH_LDB_DYN_INTERNAL_ERROR, pClCursor, "ClConnectionManager::_ExecuteStatementInConnectionDB (BindColumn Fec Ult Cambio)", sComunErrorArg);
		delete [] pcUserDB;
		delete [] pcPwdDB;
		delete pClCursor;
		return ret;
	}

	ret = pClCursor->InitCursor (ai_pDBConn, bReallyInitialized);
	if (ret == M4_ERROR) {
		_DisplayErrorCursorDynamic( ai_pLConn, M4_CH_LDB_DYN_INTERNAL_ERROR, pClCursor, "ClConnectionManager::_ExecuteStatementInConnectionDB (InitCursor)", sComunErrorArg);
		delete [] pcUserDB;
		delete [] pcPwdDB;
		delete pClCursor;
		return ret;
	}

	m4return_t iCanceled = M4_SUCCESS;
	ret = pClCursor->Execute(0, NULL, iCanceled);
	if (ret == M4_ERROR) {
		_DisplayErrorCursorDynamic( ai_pLConn, M4_CH_LDB_DYN_INTERNAL_ERROR, pClCursor, "ClConnectionManager::_ExecuteStatementInConnectionDB (Execute)", sComunErrorArg);
		delete [] pcUserDB;
		delete [] pcPwdDB;
		delete pClCursor;
		return ret;
	}
	//Mostramos la select que se ha lanzado.
	#ifdef _M4LDB_DEBUG
		if (ai_pLConn->GetSystemDebugEnable() <= ai_pLConn->GetpLdb()->GetSysDebugLevel ()) {
			string sFormatedString("");
			MpParseStringConnection oTokenMap;
			ParseConnectionString(sConnectionStringError, oTokenMap);
			FormatConnectionString(oTokenMap, sFormatedString);
			
			ai_pLConn->m_pInspector->PrintFileDebug( ai_pLConn, "\n\t\t- Get database user 2x from '%s' by executing:\n\t\t%s.\n", sFormatedString.c_str(), sSelectError.c_str() );
			ai_pLConn->m_pInspector->PrintFileDebugDetail( M4LDB_DETAIL_LEVEL_SHOW_ALWAYS, ai_pLConn, "\n\t\t- Get database user 2x from '%s' by executing:\n\t\t%s.\n", sFormatedString.c_str(), sSelectError.c_str() );
		}
	#endif

	ret = pClCursor->Fetch ();
	switch (ret)
	{
		case M4_SUCCESS: 
			//El usuario/password de BD son necesarios.
			if ((lLengthUserDB == 0) || (lLengthPwdDB == 0))
			{
				//Los campos no pueden estar vacios.
				delete [] pcUserDB;
				delete [] pcPwdDB;
				delete pClCursor;
				//La select no ha encontrado parametrizados el usuario y password de BD para el usuario de aplicacion.
				DUMP_LDBINSP_ERRORF (ai_pLConn, M4_CH_LDB_DYN_DB_USER_PASSWORD_NOT_FOUND, sComunErrorArg);
				return M4_ERROR;
			}

			//Si el usuario 2x está bloqueado daremos un error.
			if (dIsLocked != 0)
			{
				delete [] pcUserDB;
				delete [] pcPwdDB;
				delete pClCursor;				
				DUMP_LDBINSP_ERRORF (ai_pLConn, M4_CH_LDB_DYN_USER_BLOCKED, sComunErrorArg);
				return M4_ERROR;
			}

			//Si se indica algo en el periodo de cambio de password...
			if ((lLengthChangePeriod != -1) && (lLengthFecUltCambio != -1)) {
				//Comprobaremos si ha expirado.
				//Ojo pq en la 2x guardan con local time, no con GMT. (El problema es que se guarda la del Gestor)!. Asumiremos
				//que van sincronizados el gestor y el server nuestro.
				//m4date_t dTodayGMT;
				//ClActualDate (dTodayGMT);
				//m4char_t aDateGMT[M4LDB_SIZE_TIMESTAMP + 1];
				//ClDateToString (dTodayGMT, aDateGMT, M4LDB_SIZE_TIMESTAMP + 1);
				m4date_t dToday;
				ClActualDate (dToday, M4CL_TIME_LOCAL_TIME);
				if ((dToday - dFecUltCambio) >= dChangePeriod)
				{
					delete [] pcUserDB;
					delete [] pcPwdDB;
					delete pClCursor;				
					DUMP_LDBINSP_ERRORF (ai_pLConn, M4_CH_LDB_DYN_PASSWORD_EXPIRED, sComunErrorArg);
					return M4_ERROR;
				}
			}

			break;

		case M4LDB_SQL_NO_DATA_FOUND:
			//No hay datos. Se considera error.
			delete [] pcUserDB;
			delete [] pcPwdDB;
			delete pClCursor;
			//La select no ha encontrado parametrizados el usuario y password de BD para el usuario de aplicacion.
			DUMP_LDBINSP_ERRORF (ai_pLConn, M4_CH_LDB_DYN_DB_USER_PASSWORD_NOT_FOUND, sComunErrorArg);
			return M4_ERROR;

		default:
			_DisplayErrorCursorDynamic( ai_pLConn, M4_CH_LDB_DYN_INTERNAL_ERROR, pClCursor, "ClConnectionManager::_ExecuteStatementInConnectionDB (Fetch)", sComunErrorArg);
			delete [] pcUserDB;
			delete [] pcPwdDB;
			delete pClCursor;
			return M4_ERROR;
	}

	ret = pClCursor->Close ();
	if (ret == M4_ERROR)
	{
		_DisplayErrorCursorDynamic( ai_pLConn, M4_CH_LDB_DYN_INTERNAL_ERROR, pClCursor, "ClConnectionManager::_ExecuteStatementInConnectionDB (Close)", sComunErrorArg);
		delete [] pcUserDB;
		delete [] pcPwdDB;
		delete pClCursor;
		return ret;
	}

	m4int_t iLongResul = 0;
	m4char_t * pBuffer = NULL;
	m4char_t * strPwdDBDesenc = NULL;

	//Importante: emind!!!
	//La clave de desencriptacion depende del usuario de aplicacion.
	m4int_t iLongUserApp = ai_oKeyDynamicConnection.GetIdUser().size();
	m4char_t * pKey = new m4char_t[iLongUserApp + 4];
	sprintf (pKey, "%s%s", "\x05""\x8""\x12", ai_oKeyDynamicConnection.GetIdUser().c_str());
	pKey[0] = pKey[0] + 60;
	pKey[1] = pKey[1] + 60;
	pKey[2] = pKey[2] + 60;
	
	//---------------------------------------------------
	//El usuario de aplicacion es el que realmente encripto la password... continuamos desencriptando
	//la password de BD.
	//Desencriptar la pwd de bbdd.				

	ret = DecryptTwoWay(pcPwdDB, strlen(pcPwdDB), pKey, strlen(pKey), pBuffer, iLongResul);
	if (ret != M4_ERROR) {
		//La password de BD tambien se encriptó con el identificador de usuario de aplicacion!.
		//Comprobamos que somos el usuario que encripto la password de BD.
		if(strncmp(pBuffer, ai_oKeyDynamicConnection.GetIdUser().c_str(), iLongUserApp) == 0) {
			iLongResul -= iLongUserApp;
			strPwdDBDesenc = new m4char_t[iLongResul + 1];
			strcpy(strPwdDBDesenc, pBuffer + iLongUserApp);

			//Devolvemos los valores.
			/* Bug 0162967
			Se asigna el usuario correctamente
			*/
			ao_sUserDB.append( pcUserDB );
			ao_sPasswordDB.append( strPwdDBDesenc );
			delete [] strPwdDBDesenc;
			ret = M4_SUCCESS;
		} else{
			//No somos el usuario que encripto la password de BD.
			DUMP_LDBINSP_ERRORF (ai_pLConn, M4_CH_LDB_DYN_USER_ACCESS_DENIED, sSelectError );
			ret = M4_ERROR;
		}
	} else{
		//Error en la desencriptacion de la password de BD.
		DUMP_LDBINSP_ERRORF (ai_pLConn, M4_CH_LDB_DYN_PASSWORD_DB_DESENCRYPT, sSelectError );
		ret = M4_ERROR;
	}
	//---------------------------------------------------

	delete [] pcUserDB;
	delete [] pcPwdDB;
	delete [] pBuffer;
	delete [] pKey;

	//Siempre nos cargamos el cursor.
	delete pClCursor;
	return ret;
}

m4return_t ClConnectionManager::FillDynamicConnectionString( string & aio_stConnStr, const string * ai_psUser, const string * ai_psPassword )
{
	//Si usuario o password son nulos significa que NO se modificarán los valores que actualmente tiene 
	//la cadena de conexion.

	//DSN=M3CPTEVO;SRVR=FRODOTCPO806;UID=usuario;PWD=password
	//Importante: La cadena de conexion puede haberse puesto como DSN=M3CPTEVO;SRVR=FRODOTCPO806;UID=usuario;PWD=%s
	//Lo que ocurre es que se habrá desencriptado en un paso previo, por lo tanto no habría problemas.
	if (aio_stConnStr.size() != 0) {

		m4int32_t iBytesReplaced = 0;
		m4int32_t iPosUidStart = aio_stConnStr.find(";UID=");
		m4int32_t iPosPwdStart = aio_stConnStr.find(";PWD=");
		m4int32_t iPosUidEnd = M4_STR_NPOS;
		m4int32_t iPosPwdEnd = M4_STR_NPOS;

		if ((iPosUidStart == M4_STR_NPOS) || (iPosPwdStart == M4_STR_NPOS)) {
			return M4_ERROR;
		}

		iPosUidStart = iPosUidStart + strlen(";UID=");
		iPosPwdStart = iPosPwdStart + strlen(";PWD=");

		//Eliminamos siempre por el final.
		if (iPosUidStart < iPosPwdStart) {
			//USUARIO ANTES QUE PASSWORD.
			//-----------------------------------------
			//Calculemos el numero de bytes a reemplazar.
			iPosPwdEnd = aio_stConnStr.find(";", iPosPwdStart);
			if (iPosPwdEnd == M4_STR_NPOS) {
				//No hay ; por lo tanto reemplazamos hasta el final.
				iBytesReplaced = aio_stConnStr.size() - iPosPwdStart;
			}
			else {
				//Hay ; por lo tanto reemplazamos hasta el ; encontrado.
				iBytesReplaced = iPosPwdEnd - iPosPwdStart;
			}
			//Reemplazamos password.
			if (ai_psPassword != NULL) {
				aio_stConnStr.replace( iPosPwdStart, iBytesReplaced, (*ai_psPassword) );
			}
			//-----------------------------------------

			//-----------------------------------------
			//Calculemos el numero de bytes a reemplazar.
			iPosUidEnd = aio_stConnStr.find(";", iPosUidStart);
			if (iPosUidEnd == M4_STR_NPOS) {
				//Imposible pq nos tenemos que encontrar con el ; de la password.
				return M4_ERROR;
			}
			else {
				//Hay ; por lo tanto reemplazamos hasta el ; encontrado.
				iBytesReplaced = iPosUidEnd - iPosUidStart;
			}
			//Reeemplazamos usuario.
			if (ai_psUser != NULL) {
				aio_stConnStr.replace( iPosUidStart, iBytesReplaced, (*ai_psUser) );
			}
			//-----------------------------------------
		}
		else {
			//PASSWORD ANTES QUE USUARIO.
			//-----------------------------------------
			//Calculemos el numero de bytes a reemplazar.
			iPosUidEnd = aio_stConnStr.find(";", iPosUidStart);
			if (iPosUidEnd == M4_STR_NPOS) {
				//No hay ; por lo tanto reemplazamos hasta el final.
				iBytesReplaced = aio_stConnStr.size() - iPosUidStart;
			}
			else {
				//Hay ; por lo tanto reemplazamos hasta el ; encontrado.
				iBytesReplaced = iPosUidEnd - iPosUidStart;
			}
			//Reeemplazamos usuario.
			if (ai_psUser != NULL) {
				aio_stConnStr.replace( iPosUidStart, iBytesReplaced, (*ai_psUser) );
			}
			//-----------------------------------------

			//-----------------------------------------
			//Calculemos el numero de bytes a reemplazar.
			iPosPwdEnd = aio_stConnStr.find(";", iPosPwdStart);
			if (iPosPwdEnd == M4_STR_NPOS) {
				//Imposible pq nos tenemos que encontrar con el ; del usuario.
				return M4_ERROR;
			}
			else {
				//Hay ; por lo tanto reemplazamos hasta el ; encontrado.
				iBytesReplaced = iPosPwdEnd - iPosPwdStart;
			}
			//Reemplazamos password.
			if (ai_psPassword != NULL) {
				aio_stConnStr.replace( iPosPwdStart, iBytesReplaced, (*ai_psPassword) );
			}
			//-----------------------------------------
		}
	}
	return M4_SUCCESS;
}

m4return_t ClConnectionManager::ParseConnectionString( const string & ai_stConnStr, MpParseStringConnection & ao_mMpParseStringConnection )
{
	//Retorna en el mapa de salida la cadena de conexión parseada.
	//P.e. DSN=M3CPTEVO;SRVR=FRODOTCPO806;UID=usuario;PWD=password devolvera tokens (clave,valor):
	//(DSN,M3CPTEVO)(SRVR,FRODOTCPO806)(UID,usuario)(PWD,password)
	
	m4int32_t iPosStart = 0;
	m4int32_t iPosEnd = 0;
	m4int32_t iPosSemicolon = 0;
	m4int32_t iNumChr = ai_stConnStr.size() - 1;

	//Inicialmente siempre entramos una vez. Tenemos que iterar mientras hayan puntos y coma pero 
	//no haya finalizado la cadena.	
	while ((iPosSemicolon != M4_STR_NPOS) && (iPosSemicolon < iNumChr) ) {
		//Buscamos el punto y coma desde el comienzo de subcadena
		iPosSemicolon = ai_stConnStr.find(";", iPosStart);
		//Si no hay mas punto y coma significa que el resto de la cadena es el token
		if (iPosSemicolon == M4_STR_NPOS) {
			iPosEnd = ai_stConnStr.size();
		}
		else {
			iPosEnd = iPosSemicolon - 1;
		}

		//Ya tenemos limitado el token con [iPosStart,iPosEnd] de la forma Clave=Valor.
		//Buscamos el igual. Debe existir y no nos podemos pasar del token!.
		//Si la cadena empieza por punto y coma o hay varios seguidos en algun sitio daremos un error. Esto es debido
		//a que se habremos pasado al siguiente token sin tener finalizado el token actual.
		m4int32_t iPosEqual = ai_stConnStr.find("=", iPosStart);
		if ((iPosEqual == M4_STR_NPOS) || (iPosEqual > iPosEnd)) {			
			ao_mMpParseStringConnection.clear();
			return M4_ERROR;
		}
		else {
			//Metemos los valores en el mapa.
			//string sKey (ai_stConnStr[iPosStart], ai_stConnStr[iPosEqual - 1]);
			//string sValue (ai_stConnStr[iPosEqual + 1], ai_stConnStr[iPosEnd]);

			//DSN=M3CPTEVO;SRVR=FRODOTCPO806;UID=usuario;PWD=password
			//Por ejemplo, para el token SRVR=FRODOTCPO806:
			//	La clave (SRVR) tenemos que [iPosStart=13,iPosEqual=17,iPosEnd=29,iPosSemicolon=30]			
			//	Por lo tanto tenemos que obtener string empezando en 13 una cadena de 4 elementos (17-13)			
			string sKey = ai_stConnStr.substr(iPosStart, iPosEqual - iPosStart);

			//	El valor (FRODOTCPO806) tenemos que [iPosStart=13,iPosEqual=17,iPosEnd=29,iPosSemicolon=30]
			//	Por lo tanto tenemos que obtener string empezando en 18 una cadena de 12 elementos (29-17)
			string sValue = ai_stConnStr.substr(iPosEqual + 1, iPosEnd - iPosEqual);
			ao_mMpParseStringConnection.insert( MpParseStringConnection::value_type(sKey, sValue) );
		}

		//Ahora incrementamos el inicio al siguiente token
		iPosStart = iPosSemicolon + 1;
	}

	return M4_SUCCESS;
}


m4return_t ClConnectionManager::FormatConnectionString( MpParseStringConnection & ai_mMpParseStringConnection, string & ao_stFormated )
{
	//Vamos a formatear la cadena de conexion para mostrarla de una determinada forma.
	//Recibimos el mapa con las claves y los valores.
	//Vamos a formatearla como usuario/passwd@servidor (sin poner la password). Es decir sólo USUARIO@SERVIDOR

	//Buscamos el usuario y el servidor.	
	itMpParseStringConnection itUid = ai_mMpParseStringConnection.find("UID");
	itMpParseStringConnection itSrvr = ai_mMpParseStringConnection.find("SRVR");
	//Puede ser la cadena SERVER!.
	itMpParseStringConnection itServer = ai_mMpParseStringConnection.find("SERVER");
	if ((itUid != ai_mMpParseStringConnection.end()) 
		&& ( (itSrvr != ai_mMpParseStringConnection.end()) || (itServer != ai_mMpParseStringConnection.end())) ) {
			ao_stFormated.append((*itUid).second);
			ao_stFormated.append("@");
			//Una de las dos modalidades debe existir!.
			if (itSrvr != ai_mMpParseStringConnection.end()) {
				ao_stFormated.append((*itSrvr).second);
			}
			else {			
				ao_stFormated.append((*itServer).second);
			}
			return M4_SUCCESS;
	}
	return M4_ERROR;	
}

void ClConnectionManager::_DisplayErrorCursorDynamic( ClLConn * ai_pLConn, m4int32_t ai_iErrorType, ClCursor * ai_pClCursor, const string & ai_sWhereIsError, const string & ai_sDescription )
{
	m4return_t result = M4_SUCCESS;
	m4pchar_t pcError = NULL;
	m4int32_t iErrorCode;
	string sSecondErrorArg("");

	//Esto nunca debería producirse pq siempre se comprueba el cursor.
	if (ai_pClCursor == NULL) {
		DUMP_LDBINSP_ERRORF ( ai_pLConn, M4_CH_LDB_DYN_INTERNAL_ERROR, "ClConnectionManager::_DisplayErrorCursorDynamic" << "Cursor is null" );
		return;
	}

	result = ai_pClCursor->GetErrorString ( M4_ERROR, iErrorCode, &pcError );

	sSecondErrorArg.append( ai_sDescription );
	sSecondErrorArg.append( " " );
	if (result != M4LDB_SQL_NO_DATA_FOUND) {
		if (result == M4_ERROR) {
			DUMP_LDBINSP_ERROR (ai_pLConn, M4_CH_LDB_DBWRAP_DRIVER_ERROR);
		}
		else {
			sSecondErrorArg.append( pcError );
			delete [] pcError ;
		}
	}

	switch (ai_iErrorType)
	{
		case M4_CH_LDB_DYN_INTERNAL_ERROR:
			DUMP_LDBINSP_ERRORF ( ai_pLConn, M4_CH_LDB_DYN_INTERNAL_ERROR, ai_sWhereIsError << sSecondErrorArg );
			break;

		default:
			break;
	}
}

