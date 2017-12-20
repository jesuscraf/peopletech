//	==============================================================================
//
//	 (c) Copyright  1991-1999 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            Executive
//	 File:              ClExecutorStateManager.cpp
//	 Project:           M4ExeForChannel
//	 Author:            Meta Software M.S. , S.A
//	 Date:				980112
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	==============================================================================

// Process to do CACHE :
//	-	Detach SESSION_CHANNEL
//	-	Create object IODriver:	ClDataStoreIODriver oDSIOD;
//	-	Create buffer DataStorage to reuse :	M4DataStorage* poOutputDataStorage = M4DataStorage::GetNewDataStorage();
//	-	Create ObjectDirectory
//		-	Add Channel
//		-	Add Access
//	-	oObjDir.Serialize(oDataStorage)
//	-	Create or Get CacheableObject (si no existe)
//	-	poCacheableObject->set(oDataStorage)

//	-	De esta forma tenemos en el objeto CacheableObject - el estado (object directory) serializado.

#include "blocksynchronization.hpp"
#include "clexecutorstatemanager.hpp"
#include "clexecutorstate.hpp"
#include "m4string.hpp"
#include "dm.hpp"
#include "channel.hpp"
#include "m4srvres.hpp"
#include "cldatstoiod.hpp"
#include "cllgadap.hpp"
#include "objdir.hpp"
#include "datastorages.hpp"

#include "clexecutorcache.hpp"
#include "clm4objservice.hpp"	//M4OEINIT
#include "executor.hpp"			//M4VM


// Para poder volcar los estados a disco, y depurar.
// En este caso necesitamos linkar con: m4serial y m4filemanager.
#if	_M4_DEBUG > 10
#include "clfileiod.hpp"
#endif


//#include "cldefine.hpp"

#include <xercesc/parsers/SAXParser.hpp>
#include "serverm4xml.hpp"
#include "m4xmlutil.hpp"

// Redefinicion de Macros para volcado de errores, en DEBUG.
// reutilizamos los errores de EFV
#include "m4srvefcres.hpp"
#ifdef _DEBUG
#define DUMP_CHLOG_STATE_ERRORF(x1, x2)		DUMP_CHLOG_ERRORF(x1, x2);
#else
#define DUMP_CHLOG_STATE_ERRORF(x1, x2)
#endif


//--------------------------------------------
//	ClCacheConfigInfo
//--------------------------------------------

// Default Constructor
ClCacheConfigInfo::ClCacheConfigInfo() 
{
	SetDefaultPath();
	//m_iMode			= M4CL_CACHE_READ_WRITE;
	m_iMaxMemory		= 1048576;	// 1 Mb de Cache
	m_iMaxNumObjects	= 0;		// No. infinito de objetos
	m_iRefreshRatio		= 5;		// Cada 5 PutObjects en Cache, se persiste el objeto, si no lo estaba.

	m_poCMCRCache			= NULL;
	m_poCSCRCache			= NULL;
	m_poPresentationCache	= NULL;
	m_poMapCache			= NULL;
	m_poDataCache			= NULL;
}

// Constructor with Params
ClCacheConfigInfo::ClCacheConfigInfo(m4pcchar_t ai_pPath, m4uint32_t ai_iMaxMemory, m4uint32_t ai_iNumObjects, m4uint32_t ai_iRefreshRatio)
{
	if (ai_pPath) {
		m_sCacheDirectory.assign(ai_pPath);
	} else {
		SetDefaultPath();
	}
	//m_iMode				= M4CL_CACHE_READ_WRITE;	// Solo tiene sentido READ_WRITE
	m_iMaxMemory		= ai_iMaxMemory;
	m_iMaxNumObjects	= ai_iNumObjects;
	m_iRefreshRatio		= ai_iRefreshRatio;


	m_poCMCRCache			= NULL;
	m_poCSCRCache			= NULL;
	m_poPresentationCache	= NULL;
	m_poMapCache			= NULL;
	m_poDataCache			= NULL;
}

// Set Cache subsystems
void ClCacheConfigInfo::SetCaches(	ClCache *ai_poCMCRCache, ClCache *ai_poCSCRCache, 
			    ClCache *ai_poPresentationCache, ClCache *ai_poMapCache, ClCache *ai_poDataCache)
{
	m_poCMCRCache			= ai_poCMCRCache;
	m_poCSCRCache			= ai_poCSCRCache;
	m_poPresentationCache	= ai_poPresentationCache;
	m_poMapCache			= ai_poMapCache;
	m_poDataCache			= ai_poDataCache;
}



//--------------------------------------------
//	ClExecutorStateManager
//--------------------------------------------

// Teoricamente solo va a haber un objeto de esta clase. 
// Sera el Gestor de Estados, que controla el acceso a la Cache de Estados.


// Constructor
ClExecutorStateManager::ClExecutorStateManager() {
	m_oMutex.Init();
	m_oSessionMutex.Init();
	m_poExecutorStateCache = NULL;
	m_poCache = NULL;
}

// Destructor
ClExecutorStateManager::~ClExecutorStateManager()
{
	if (m_poExecutorStateCache) {
		// Persistimos la Cache
		//PersistCache();	// Por defecto no lo vamos a hacer... si se quiere que se haga a capon
		// Destruimos el Objeto
		delete m_poExecutorStateCache;
		m_poExecutorStateCache = NULL;
	}

	if (m_poCache) {
		delete m_poCache;
		m_poCache = NULL;
	}
}

// Initialize ExecutorState Cache
// Entrada:	ai_oConfig	= Objeto con la configuración de la Cache
// Salida:	M4_SUCCESS (ok) o M4_ERROR
m4return_t ClExecutorStateManager::InitCache(const ClCacheConfigInfo& ai_oConfig)
{
	// Seccion critica -> Inicializacion de la Cache
	ClMutBlock oBlock(&m_oMutex);

	// Indicamos las Caches que emplearán todos los Estados.
	// ClExecutorState::SetCaches(ai_oConfig.m_poCMCRCache, ai_oConfig.m_poCSCRCache, ai_oConfig.m_poPresentationCache, ai_oConfig.m_poDataCache );

	//Ya esta inicializada
	if ( IsInitialized() )
		return M4_SUCCESS;

	// Verificamos la cadena de Path
	if ( ai_oConfig.m_sCacheDirectory.c_str() == NULL || *(ai_oConfig.m_sCacheDirectory.c_str()) == '\0' ) {
		return M4_ERROR;
	}
	
	// Creamos la nueva base de cache
	ClExecutorStateCacheTree *poClCacheTree;
	poClCacheTree = new ClExecutorStateCacheTree(0, ai_oConfig.m_iMaxMemory, ai_oConfig.m_iMaxNumObjects,  ai_oConfig.m_iRefreshRatio);
	m_poCache = new ClCacheTest();
	m_poCache->SetCacheStyle( poClCacheTree );

	// Creamos la Cache con los valores de configuracion apropiados
	m_poExecutorStateCache = new ClExecutorStateCache(m_poCache, 
		ai_oConfig.m_sCacheDirectory.c_str(), 
		M4CL_CACHE_READ_WRITE, //ai_oConfig.m_iMode
		ai_oConfig.m_iMaxMemory, 
		ai_oConfig.m_iMaxNumObjects, 
		ai_oConfig.m_iRefreshRatio );

	/*
	// Creamos la Cache basada en el multimap.
	m_poExecutorStateCache = new ClExecutorStateCacheMap();
	*/

	if (!m_poExecutorStateCache)
		return M4_ERROR;

	// Indicamos las Caches que emplearán todos los Estados.
	ClExecutorState::SetCaches(ai_oConfig.m_poCMCRCache, ai_oConfig.m_poCSCRCache, ai_oConfig.m_poPresentationCache, ai_oConfig.m_poMapCache, ai_oConfig.m_poDataCache );

	// Inicializamos la Cache de Estados
	m_poExecutorStateCache->Init();

	// Nosotros no enlazamos la Cache al Directorio de Caches, para que sea independiente
	// del resto de caches.
	//GetCacheDirectory()->Attach(m_poDataCache);
	
	return M4_SUCCESS;
}


// Enganchar el Canal Sesion en el Channel Manager
// Entrada: ai_pChannelManager = Channel Manager
// Salida:	return = M4_SUCCESS (Ok) o M4_ERROR (No encontrado, o fallo)
m4return_t ClExecutorStateManager::DetachSessionChannel(ClChannelManager *ai_pChannelManager)
{
	m4return_t ret=M4_SUCCESS;
	ClChannel *poSessionChannel = NULL;

	if (!ai_pChannelManager)
		return M4_ERROR;

	// Pedimos Canal Sesion al Channel Manager
	poSessionChannel = ai_pChannelManager->GetSessionChannel();

	// Desenganchamos del LogonAdaptor
	ret = ai_pChannelManager->SetSessionChannel(NULL);

	// Si encontrado => Desenganchamos el Canal del ChannelManager
	if (ret == M4_SUCCESS && poSessionChannel!=NULL) {
		ret = poSessionChannel->Detach();
		if (ret != M4_SUCCESS) {
			DUMP_CHLOG_ERRORF ( M4_SRV_SESSION_CHANNEL , "ExecutiveForChannel. Cannot detach session channel") ;
			return M4_ERROR;
		}

		return ret;
	}
	return M4_ERROR;
}

// Enganchar el Canal Sesion al Channel Manager
// Entrada: ai_pChannelManager = Channel Manager
//			ai_poSessionChannel = Canal Sesion nuevo
// Salida:	return = M4_SUCCESS (Ok) o M4_ERROR (No encontrado, o fallo)
m4return_t ClExecutorStateManager::AttachSessionChannel(ClChannelManager *ai_pChannelManager, ClChannel *ai_poSessionChannel)
{
	
	m4return_t ret=M4_SUCCESS;

	if (!ai_pChannelManager || !ai_poSessionChannel)
		return M4_ERROR;

	// Enlazamos Canal Sesion al Channel Manager
	ret = ai_poSessionChannel->Attach(ai_pChannelManager);
	if (ret != M4_SUCCESS) {
		DUMP_CHLOG_ERRORF ( M4_SRV_SESSION_CHANNEL , "ExecutiveForChannel. Cannot attach session channel") ;
		return ret;
	}
	// Ponemos en el Channel Manager el Canal Sesion
	ret = ai_pChannelManager->SetSessionChannel(ai_poSessionChannel);
	return ret;
}

// Recuperar el estado de una sesion de la cache
// Esta funcion es thread-safe (excepto si el Id es el mismo en dos threads)
// porque sobre el mismo objeto va a haber varios threads llamando a este método.
// Entrada:	ai_pccSessionId		= Sesion ID (a buscar)
//			aio_oCMInitParameters= Parametros con el estado, y Channel Manager (nuevo) en el que engancharemos la sesion (in/out)
//			ai_bIsNew			= Flag para indicar si el estado a recuperar es nuevo o no
// Salida:	ao_bFound	= M4_TRUE (Encontrado) o M4_FALSE (no encontrado)
//			return = M4_SUCCESS (Ok) o M4_ERROR (fallo)
m4return_t	ClExecutorStateManager::RecoverState(ClCMInitParameters &aio_oCMInitParameters, m4bool_t& ao_bFound, m4bool_t ai_bIsNew /*=M4_FALSE*/)
{
	m4return_t ret = M4_SUCCESS;
	m4bool_t bClone = M4_FALSE;

	ClM4XMLInfo::SetFileInfoByThread("RecoverState-Start");

	string sSessionId = aio_oCMInitParameters.GetSubSessionId();
	m4pchar_t pccSessionId = (m4pchar_t) sSessionId.c_str();

	// Sesion ID para el clone (puede ser una peticion de "clone") o NULL (no hay que clonar)
	string sCloneSessionId = aio_oCMInitParameters.GetEndSubSessionId();
	m4pchar_t pccCloneSessionId = (m4pchar_t) sCloneSessionId.c_str();

	ao_bFound = M4_FALSE;

	if (!IsInitialized() || (*pccSessionId == '\0')) {
		DUMP_CHLOG_STATE_ERRORF( M4_ERR_XML_LOAD_STATE, aio_oCMInitParameters.GetSubSessionId().c_str() );
		return M4_ERROR;
	}

	// Si los Id's no son iguales => hay que clonar el estado:
	// antes de soltar el objeto hay que copiar al nuevo estado
	if (pccCloneSessionId && strcmp(pccCloneSessionId, pccSessionId) != 0 ) {
		// Nos piden clonar el estado de clave "ai_pccSessionId" con la clave "pccCloneSessionId"
		bClone = M4_TRUE;
	}

	ClExecutorState* poExecutorState = NULL;
	ClExecutorState* poExecutorStateNew = NULL;
	ClM4XMLContext* poXMLContext = NULL;

	// Sólo buscamos en la Cache, si no es un nuevo estado
	if ( !ai_bIsNew ) {
		ClM4XMLInfo::SetFileInfoByThread("RecoverState-GetObject-Start");

		// Buscar SessionId en la Cache, y pillarlo en exclusiva, pero sin bloquear: GET_EXNBLOCK
		// Buscar SessionId en la Cache, y pillarlo pero no en exclusiva: GET_SHARED (esto era antes)

		// efv
		// first of all we need to check if the clone session id exists
		// if it exists then we use it. This is the normal behaviour in the m4operations api.
		if (bClone) {

			// ok, lets try to find the clone object
			ret = GetCache()->GetObject(pccCloneSessionId, poExecutorState, ao_bFound, ClCacheInterface::GET_EXNBLOCK);

			ClM4XMLInfo::SetFileInfoByThread("RecoverState-GetObject-Clone-End");
			if (ret != M4_SUCCESS) {
				DUMP_CHLOG_STATE_ERRORF( M4_ERR_XML_LOAD_STATE, aio_oCMInitParameters.GetSubSessionId().c_str() );
				return M4_ERROR;	// Error en la cache
			}
		
			if (ao_bFound) {
				// hey, the clone destination is already there, so we just use it
				bClone = M4_FALSE;
			}

		}

		// have we still to find out the object?
		if (ao_bFound == M4_FALSE) {

			// ok we have to find it out. A bit boring, don't you think?
			ret = GetCache()->GetObject(pccSessionId, poExecutorState, ao_bFound, ClCacheInterface::GET_EXNBLOCK);
			//ret = GetCache()->GetObject(pccSessionId, poExecutorState, ao_bFound);
			ClM4XMLInfo::SetFileInfoByThread("RecoverState-GetObject-End");
			if (ret != M4_SUCCESS) {
				DUMP_CHLOG_STATE_ERRORF( M4_ERR_XML_LOAD_STATE, aio_oCMInitParameters.GetSubSessionId().c_str() );
				return M4_ERROR;	// Error en la cache
			}
		}
	}

	// Importante !!!!
	// Si lo encontramos en la cache, 2 threads no pueden obtener el mismo objeto <poExecutorState>
	
	if (!ao_bFound) {

		if (bClone) {
			pccSessionId = pccCloneSessionId;
		}

		// No encontrado en la cache => Creamos el estado, y lo inicializamos.
		poExecutorState = new ClExecutorState(pccSessionId);
		ret = poExecutorState->Init( aio_oCMInitParameters );
		if (ret != M4_SUCCESS) {
			delete poExecutorState;
			poExecutorState = NULL;
			DUMP_CHLOG_STATE_ERRORF( M4_ERR_XML_LOAD_STATE, aio_oCMInitParameters.GetSubSessionId().c_str() );
			return M4_ERROR;
		}

		// Insertamos nuevo objeto en la Cache
		ClM4XMLInfo::SetFileInfoByThread("RecoverState-PutObject-Start");
		poExecutorState->SetUsing(M4_TRUE);
		ret = GetCache()->PutObject(pccSessionId, poExecutorState);
		ClM4XMLInfo::SetFileInfoByThread("RecoverState-PutObject-End");
		if (ret != M4_SUCCESS) {
			poExecutorState->SetUsing(M4_FALSE);
			delete poExecutorState;
			poExecutorState = NULL;
			DUMP_CHLOG_STATE_ERRORF( M4_ERR_XML_LOAD_STATE, aio_oCMInitParameters.GetSubSessionId().c_str() );
			return M4_ERROR;
		}

		// Completamos los parametros para el Context
		//aio_oCMInitParameters.SetCM( GetChannelManager() ); // Se hace en el ClExecutor::Init( aio_oCMInitParameters );

	} else {

		// Encontrado en la cache

		// No puede estar en uso
		if ( poExecutorState->IsInUse() ) {
			ClM4XMLInfo::SetFileInfoByThread("RecoverState-FreeObject-Start");
			poExecutorState->SetUsing(M4_FALSE);
			ret = GetCache()->FreeObject(poExecutorState);
			ClM4XMLInfo::SetFileInfoByThread("RecoverState-FreeObject-End");
			DUMP_CHLOG_STATE_ERRORF( M4_ERR_XML_LOAD_STATE, aio_oCMInitParameters.GetSubSessionId().c_str() );
			return M4_ERROR;
		} else {
			// Lo vamos a usar
			poExecutorState->SetUsing(M4_TRUE);
			poExecutorState->Update(aio_oCMInitParameters);
		}

		// Si nos piden clonar un estado existente
		if ( bClone ) {
			// Clonamos: poExecutorStateNew : Es equivalente a hacer un "Save As" en la cache
			ret = poExecutorState->Clone( pccCloneSessionId, aio_oCMInitParameters, poExecutorStateNew );
			if (ret != M4_SUCCESS || !poExecutorStateNew) {
				ClM4XMLInfo::SetFileInfoByThread("RecoverState-FreeObject-Start");
				poExecutorState->SetUsing(M4_FALSE);
				ret = GetCache()->FreeObject(poExecutorState);
				ClM4XMLInfo::SetFileInfoByThread("RecoverState-FreeObject-End");
				DUMP_CHLOG_STATE_ERRORF( M4_ERR_XML_LOAD_STATE, aio_oCMInitParameters.GetSubSessionId().c_str() );
				return M4_ERROR;
			}

			// Soltamos la referencia que teniamos en la Cache del Original
			// Aqui es donde se dispara la persistencia por RefreshRatio o por Necesidades de Memoria => Ojo con el Multithreading aqui...
			ClM4XMLInfo::SetFileInfoByThread("RecoverState-FreeObject-Start");
			poExecutorState->SetUsing(M4_FALSE);
			ret = GetCache()->FreeObject(poExecutorState);
			ClM4XMLInfo::SetFileInfoByThread("RecoverState-FreeObject-End");
			if (ret != M4_SUCCESS) {
				delete poExecutorStateNew;
				poExecutorStateNew = NULL;
				DUMP_CHLOG_STATE_ERRORF( M4_ERR_XML_LOAD_STATE, aio_oCMInitParameters.GetSubSessionId().c_str() );
				return M4_ERROR;
			}

			// El estado que nos interesa ahora es el nuevo "clone"
			poExecutorState = poExecutorStateNew;

			// Insertamos nuevo objeto en la Cache (clonado)
			ClM4XMLInfo::SetFileInfoByThread("RecoverState-PutObject-Start");
			poExecutorState->SetUsing(M4_TRUE);
			ret = GetCache()->PutObject(pccCloneSessionId, poExecutorState);
			ClM4XMLInfo::SetFileInfoByThread("RecoverState-PutObject-End");
			if (ret != M4_SUCCESS) {
				poExecutorState->SetUsing(M4_FALSE);
				delete poExecutorState;
				poExecutorState = NULL;
				DUMP_CHLOG_STATE_ERRORF( M4_ERR_XML_LOAD_STATE, aio_oCMInitParameters.GetSubSessionId().c_str() );
				return M4_ERROR;
			}
		}
	
		// Si lo recuperamos de disco, el Context esta bastante limpio
		// y hay que enchufarle los valores válidos.
		poXMLContext = poExecutorState->GetXMLContext();
		
		if (poXMLContext) {
			poXMLContext->Update(aio_oCMInitParameters);
		} else {
			ClM4XMLInfo::SetFileInfoByThread("RecoverState-FreeObject-Start");
			poExecutorState->SetUsing(M4_FALSE);
			ret = GetCache()->FreeObject(poExecutorState);
			ClM4XMLInfo::SetFileInfoByThread("RecoverState-FreeObject-End");
			DUMP_CHLOG_STATE_ERRORF( M4_ERR_XML_LOAD_STATE, aio_oCMInitParameters.GetSubSessionId().c_str() );
			return M4_ERROR;
		}

	}

	if (!poExecutorState) {
		DUMP_CHLOG_STATE_ERRORF( M4_ERR_XML_LOAD_STATE, aio_oCMInitParameters.GetSubSessionId().c_str() );
		return M4_ERROR;
	}

	// Estaba en la Cache => No soltamos la referencia, hasta que se haga el PersisState
	//poExecutorState->SetUsing(M4_FALSE);
	//ret = GetCache()->FreeObject(poExecutorState);

	// Obtenemos el Contexto valido
	poXMLContext = poExecutorState->GetXMLContext();

	if (poXMLContext) {
		// resets initial members
		poXMLContext->Init();

		// Enchufamos el DataStorage para el tratamiento de Blobs
		poXMLContext->ProcInFileRefs( aio_oCMInitParameters.GetInputDataStorage() );
	} else {
		// Muy chungo
		ClM4XMLInfo::SetFileInfoByThread("RecoverState-FreeObject-Start");
		poExecutorState->SetUsing(M4_FALSE);
		ret = GetCache()->FreeObject(poExecutorState);
		ClM4XMLInfo::SetFileInfoByThread("RecoverState-FreeObject-End");
		DUMP_CHLOG_STATE_ERRORF( M4_ERR_XML_LOAD_STATE, aio_oCMInitParameters.GetSubSessionId().c_str() );
		return M4_ERROR;
	}

	// Devolvemos el Contexto y el Estado
	poExecutorState->GetM4ObjService()->GetpExecutor()->SetpCancelStatus( aio_oCMInitParameters.GetStatus() );
	aio_oCMInitParameters.SetExecutorState( poExecutorState );

	ClM4XMLInfo::SetFileInfoByThread("RecoverState-End");

	return ret;
}


// Persistir el estado de una sesion en la Cache.
// Esta funcion es thread-safe (excepto si el Id es el mismo en dos threads)
// porque sobre el mismo objeto va a haber varios threads llamando a este método.
// Entrada:	ai_pccSessionId = Sesion ID
//			aio_oCMInitParameters= Parametros con el estado, y Channel Manager (nuevo) en el que engancharemos la sesion (in/out)
// Salida:	return = M4_SUCCESS (Ok) o M4_ERROR (fallo)
m4return_t ClExecutorStateManager::PersistState(ClCMInitParameters &aio_oCMInitParameters)
{
	m4return_t ret	= M4_SUCCESS;

	ClM4XMLInfo::SetFileInfoByThread("PersistState-Start");

	ClExecutorState* poExecutorState = NULL;
	//ClExecutorState* poOldExecutorState = NULL;
	m4bool_t bFound = M4_FALSE;

	poExecutorState = aio_oCMInitParameters.GetExecutorState();
	if (!poExecutorState) {
		DUMP_CHLOG_STATE_ERRORF ( M4_ERR_XML_PERSIST_STATE, aio_oCMInitParameters.GetSubSessionId().c_str() );
		return M4_ERROR;	// No nos dan el estado a persistir
	}

	string sSessionId = aio_oCMInitParameters.GetEndSubSessionId();
	m4pchar_t pccSessionId = (m4pchar_t) sSessionId.c_str();

	if ( !IsInitialized() || (*pccSessionId == '\0') || !poExecutorState->GetSessionId() ) {
		DUMP_CHLOG_STATE_ERRORF ( M4_ERR_XML_PERSIST_STATE, aio_oCMInitParameters.GetSubSessionId().c_str() );
		return M4_ERROR;
	}

	// Si los Id's no son iguales => error
	if ( strcmp(pccSessionId, poExecutorState->GetSessionId()) != 0 ) {
		DUMP_CHLOG_STATE_ERRORF ( M4_ERR_XML_PERSIST_STATE, aio_oCMInitParameters.GetSubSessionId().c_str() );
		return M4_ERROR;	// Seria la hostia que saltase este error
	}

	//ClM4XMLInfo::SetFileInfoByThread("PersistState-GetThisObject-Start");
	// Buscar SessionId en la Cache, y pillarlo en exclusiva, sin bloquear: GET_EXNBLOCK
	// pero lo podemos pillar no en exclusiva: GET_SHARED  (esto era antes)
	//ret = GetCache()->GetObject(ai_pccSessionId, poOldExecutorState, bFound, ClCacheInterface::GET_SHARED);
	//ret = GetCache()->GetObject(poExecutorState->GetSessionId(), poOldExecutorState, bFound, ClCacheInterface::GET_SHARED);
	//ret = GetCache()->GetThisObject(poExecutorState->GetSessionId(), poExecutorState, bFound);
	//ClM4XMLInfo::SetFileInfoByThread("PersistState-GetThisObject-End");
	if (ret != M4_SUCCESS ) {
		DUMP_CHLOG_STATE_ERRORF ( M4_ERR_XML_PERSIST_STATE, aio_oCMInitParameters.GetSubSessionId().c_str() );
		return M4_ERROR;	// Error en la Cache
	}
	if ( !poExecutorState->IsInUse() ) {
		// No puede ser que esté en la cache y esté en uso
		DUMP_CHLOG_STATE_ERRORF ( M4_ERR_XML_PERSIST_STATE, aio_oCMInitParameters.GetSubSessionId().c_str() );
		return M4_ERROR;
	}

	// Importante !!!!
	// 1. Si lo encontramos en la cache, es porque nadie lo está usando, 

	// Si no esta en la Cache => Error
	//if (!bFound) {
		// No puede ser que no esté en la cache
	//	return M4_ERROR;
	//}
	//if ( poExecutorState != poOldExecutorState ) {
	//	return M4_ERROR;	// El objeto de la cache y el objeto actual no son el mismo.
	//}


	// Soltamos la referencia que teniamos de la Cache.
	// Aqui es donde se dispara la persistencia por RefreshRatio o por Necesidades de Memoria => Ojo con el Multithreading aqui...
	ClM4XMLInfo::SetFileInfoByThread("PersistState-FreeObject-Start");

	poExecutorState->SetUsing(M4_FALSE);
	ret = GetCache()->FreeObject(poExecutorState);
	ClM4XMLInfo::SetFileInfoByThread("PersistState-FreeObject-End");
	
	// Despues si es necesario persistiremos la Cache
	// ret = ai_poStateCache->PersistToDisk()

	// validate cache deleting old elements
	UpdateCache(aio_oCMInitParameters);

	ClM4XMLInfo::SetFileInfoByThread("PersistState-End");

	if (ret != M4_SUCCESS) {
		DUMP_CHLOG_STATE_ERRORF ( M4_ERR_XML_PERSIST_STATE, aio_oCMInitParameters.GetSubSessionId().c_str() );
		return M4_ERROR;
	}

	return M4_SUCCESS;
}


// Updates the cache deleting old elements
// Follows the PersistLevel and the last access date
m4return_t ClExecutorStateManager::UpdateCache(ClCMInitParameters &aio_oCMInitParameters)
{
	// syncs
	ClMutBlock oBlock(&m_oMutex);

	// scans cache objects
	m4return_t iResult = M4_SUCCESS, iStatus = M4_SUCCESS;
	ClCacheableObject *poCacheableObject = NULL;
	void *pOldRef = NULL;
	
	ClExecutorState *poCandidate = NULL;
	ClExecutorStateCache *poExecutorCache = GetCache();

	if (poExecutorCache == NULL)
	{
		DUMP_CHLOG_ERROR(M4_ERR_XML_CACHE_INTERNAL_ERROR);
		return M4_ERROR;
	}

	ClCache * poCache = GetCache()->m_poCache;

	if (poCache == NULL)
	{
		DUMP_CHLOG_ERROR(M4_ERR_XML_CACHE_INTERNAL_ERROR);
		return M4_ERROR;
	}

	// gets the objects from the cache
	m4uint32_t uBaseSessionId = aio_oCMInitParameters.GetSessionId();
	m4int32_t iNSessions = 0;
	m4uint16_t iMinPersistLevel = -1;
	m4date_t iMinDate = ClPlusInfiniteDate();

	// blocks cache
	{
		ClMutBlock oBlockCache(poCache->GetMutex());

		for (;;)
		{
			iResult = poCache->GetNext(poCacheableObject,pOldRef);
			if (iResult != M4_SUCCESS)
				break;
			
			// checks the end condition
			if (poCacheableObject == NULL)
				break;
			
			// proccesses error state
			if (iStatus != M4_SUCCESS)
				continue;
			
			ClExecutorStateDataContainer *poContainer = (ClExecutorStateDataContainer*) poCacheableObject;
			
			ClExecutorState *poExecutorState = poContainer->GetValue();
			
			// object is not available (in disk, maybe)
			if (poExecutorState == NULL)
				continue;
			
			ClM4XMLContext *poXMLContext = poExecutorState->GetXMLContext();
			
			if (poXMLContext == NULL)
			{
				DUMP_CHLOG_ERROR(M4_ERR_XML_CACHE_INTERNAL_ERROR);
				iStatus = M4_ERROR;
				continue;
			}
			
			// checks the session id
			m4uint32_t uSessionId = poXMLContext->GetSessionId();
			if (uSessionId != uBaseSessionId)
				continue;
			
			// one more session
			m4uint16_t iPersistLevel = poXMLContext->GetPersistLevel();
			if (iPersistLevel)
			{
				iNSessions++;
				
				// checks the minimun for all contexts in the session
				if (iPersistLevel < iMinPersistLevel)
				{
					iMinPersistLevel = iPersistLevel;
					iMinDate = poXMLContext->GetLastAccess();;
					poCandidate = poExecutorState;
				}
				else
				{
					if (iPersistLevel == iMinPersistLevel)
					{
						m4date_t iDate = poXMLContext->GetLastAccess();
						if (iDate < iMinDate)
						{
							iMinDate = iDate;
							poCandidate = poExecutorState;
						}
					}
				}
			}
			
			// goes to the next element
		}
	}

	if (iResult != M4_SUCCESS || iStatus != M4_SUCCESS)
		return M4_ERROR;

	// deletes the candidate
	if (poCandidate != NULL && iNSessions >= iMinPersistLevel)
	{
		if (RemoveState(poCandidate->GetSessionId()) != M4_SUCCESS)
		{
			DUMP_CHLOG_ERROR(M4_ERR_XML_CACHE_INTERNAL_ERROR);
			iResult = M4_ERROR;
		}
	}
	
	return (iResult != M4_SUCCESS || iStatus != M4_SUCCESS) ? M4_ERROR : M4_SUCCESS;
}



/*
// Persistir el estado de una sesion en la Cache.
// Esta funcion es thread-safe (excepto si el Id es el mismo en dos threads)
// porque sobre el mismo objeto va a haber varios threads llamando a este método.
// Entrada:	ai_pccSessionId = Sesion ID
//			ai_pChannelManager = Channel Manager
//			ai_pXMLContext	= XML Context to persist
// Salida:	return = M4_SUCCESS (Ok) o M4_ERROR (fallo)
m4return_t ClExecutorStateManager::PersistState(m4pchar_t ai_pccSessionId, ClChannelManager* &ai_pChannelManager, ClM4XMLContext *ai_pXMLContext)
{
	m4return_t ret	= M4_SUCCESS;
	m4return_t ret2 = M4_SUCCESS;
	m4return_t ret3 = M4_SUCCESS;

	if (!ai_pccSessionId || !ai_pChannelManager || !IsInitialized() )
		return M4_ERROR;

	M4DataStorage* poOutputDataStorage = NULL;
	ClExecutorState* poOldExecutorState = NULL;
	ClExecutorState* poExecutorState = NULL;
	m4bool_t bFound = M4_FALSE;

	// Buscar SessionId en la Cache, y pillarlo en exclusiva, sin bloquear: GET_EXNBLOCK (esto era antes)
	// pero ahora lo pillamos no en exclusiva: GET_SHARED
	//ret = GetCache()->GetObject(ai_pccSessionId, poOldExecutorState, bFound, ClCacheInterface::GET_SHARED);
	ret = GetCache()->GetObject(ai_pccSessionId, poOldExecutorState, bFound, ClCacheInterface::GET_EXNBLOCK);

	if (ret != M4_SUCCESS ) {
		return M4_ERROR;	// Error en la Cache
	}
	// Importante !!!!
	// 1. Si lo encontramos en la cache, y obtenemos el mismo objeto <poExecutorState>
	// en las llamadas de dos threads => nos vamos al garete...
	// Esto en teoría no va a suceder nunca, pero además como el GetObject es GET_EXNBLOCK,
	// el segundo thread no obtendrá el objeto sino NULL... con lo cual creerá que no está en la Cache...
	// y esto puede ser peor... porque podemos meter dos objetos con el mismo Id en la cache....
	// Así que a tener muy encuenta !!! ESTO ERA ANTES !!!

	// 2. Ahora, si lo encontramos en la cache, y obtenemos el mismo objeto <poExecutorState>
	// en las llamadas de dos threads => NO nos vamos al garete...
	// Esto en teoría puede suceder, pero además como el GetObject es GET_SHARED,
	// el segundo thread obtendrá el mismo objeto... A partir de ahí... mucho cuidado
	// Así que a tener muy encuenta !!!

	// Ahora siempre creamos un objeto nuevo... y luego pegamos el cambiazo si existia otro ya en la cache !!!
	// Pero esa operacion va a ser atómica.

	// Si no esta en la Cache => Creamos el objeto: Sesion nueva
	poExecutorState = new ClExecutorState();
	if (!poExecutorState)
		return M4_ERROR;
	// Le ponemos el Id
	if ( !poExecutorState->SetSessionId(ai_pccSessionId) ) {
		delete poExecutorState;
		return M4_ERROR;
	}


	// Inicializamos el DataStorage: le creamos o le reseteamos segun haga falta
	ret = poExecutorState->InitDataStorage();	// En la forma antigua
	if (ret != M4_SUCCESS) {
		// Salida limpia
		delete poExecutorState;
		if (bFound) {
			// Estaba en la Cache => Soltamos la referencia al antiguo objeto
			ClM4XMLInfo::SetFileInfoByThread("PersistState-FreeObject-Start");
			ret = GetCache()->FreeObject(poOldExecutorState);
			ClM4XMLInfo::SetFileInfoByThread("PersistState-FreeObject-End");
		}
		return M4_ERROR;
	}
	
	// Obtenemos un puntero al Nuevo DataStorage
	poOutputDataStorage = poExecutorState->GetDataStorage();	// Antigua forma

	// Creamos el IODriver que sabe como serializar.
	ClDataStoreIODriver oDSIOD;

	// Inicializamos IODriver, usando el DataStorage (salida de datos)
	ret = oDSIOD.Init(GEN_IO_MODE_WRITE, poOutputDataStorage);

#if _M4_DEBUG > 10
	// CheckSum de comprobación
	if (ret == M4_SUCCESS) {
		ret = oDSIOD.InitCheckSum();
	}
#endif
	
	if (ret == M4_SUCCESS) {
		//	Creamos ObjectDirectory temporal: ClCMObjectDirectory	- gestor de objetos a serializar
		ClCMObjectDirectory oObjDir(ai_pChannelManager);
		// Activamos Serialización de los Niveles 2, para este Channel Manager
		oObjDir.SetForceL2RelationSerialize(M4_TRUE);
		
		ClChannel*	pChanAux = NULL;
		m4pcchar_t	pcAuxChanId = 0;
		m4uint32_t	i;
		ClChannelDirectory* pChanDir = &( ai_pChannelManager->Channel );
		m4uint32_t iNumChan = pChanDir->Count ();	//No. de canales
	
		// Recorremos todos los Canales del ChannelManager
		for ( i = 0; i < iNumChan; i++ ) {
			pChanAux = (*pChanDir)[i];

			if (pChanAux->IsBuilt())
				pcAuxChanId = pChanAux->GetpChannelDef ()->Id ();
			else
				pcAuxChanId = 0;

			// Deberiamos saltar los canales del sistema - pero ya estan detachados del Channel Manager
				
			// Marcamos el Canal para serializar todos los registros
			pChanAux->SetRecordSerializationMode(M4CL_RECORD_SERIAL_ALL);
			
			// Marcamos el Canal para serializar todos los Items (mapped, etc.)
			pChanAux->SetItemSerializationMode(M4CL_ITEM_SERIAL_ALL);
			
			// Añadimos el Canal
			ret = ( oObjDir.IncludeChannelAndCreateOID(pChanAux) == -1 ) ? M4_ERROR : M4_SUCCESS;		// Incluimos el Channel
			if (ret == M4_SUCCESS) {
				// Añadimos todos los Access del Canal
				ret = oObjDir.IncludeAllAccessAndCreateOID(*pChanAux);	// Incluimos todos los Access
			}
			
			if (ret != M4_SUCCESS) {
				break;
			}
		}
		
		// Serializamos el ObjectDirectory
		if (ret == M4_SUCCESS) {
			oObjDir.SetKeepChannelCsTypeOnRemote(M4_TRUE);  // keeps CS types, para que no se borren al destruir el ObjectDirectory
			string sTemp("PersistState-ObjdirSerialize-Start");
			char pcTemp[64];
			sprintf(pcTemp,"\t%d",oObjDir.GetCount());
			sTemp += pcTemp;
			ClM4XMLInfo::SetFileInfoByThread(sTemp.c_str());
			ret = oObjDir.Serialize(oDSIOD);				// Sesion
			ClM4XMLInfo::SetFileInfoByThread("PersistState-ObjdirSerialize-End");
		}

		// Serializamos el Contexto de XML
		if ( ai_pXMLContext && ret==M4_SUCCESS ) {
			ClM4XMLInfo::SetFileInfoByThread("PersistState-ContextSerialize-Start");
			ret = ai_pXMLContext->Serialize(oDSIOD);
			ClM4XMLInfo::SetFileInfoByThread("PersistState-ContextSerialize-End");
		}

#if _M4_DEBUG > 10
		// CheckSum de comprobación
		if (ret == M4_SUCCESS)
			ret = oDSIOD.CreateCheckSum(poOutputDataStorage);
#endif

		// Cerramos el IODriver
		oDSIOD.End();

		// Deberiamos tambien restaurar el tipo de Serializacion (de registro y de item) para cada Canal
		//m4uint8_t GetRecordSerializationMode(void) { return m_iRecSerMod; }
		//m4uint8_t GetItemSerializationMode(void) { return m_iIteSerMod; }
		//oObjDir.SetForceL2RelationSerialize(M4_FALSE);	//Desactivamos Serialización de los Niveles 2
	}

	// De esta forma tenemos en el objeto ExecutorState - el ID y el Estado (object directory) serializado.

	//	Metemos el Objeto en la Cache
	if (ret == M4_SUCCESS ) {
		if (!bFound) {
			// No estaba en la Cache => Lo metemos (sin persistir)
			ClM4XMLInfo::SetFileInfoByThread("PersistState-PutObject-Start");
			ret2 = GetCache()->PutObject(ai_pccSessionId, poExecutorState);
			ClM4XMLInfo::SetFileInfoByThread("PersistState-PutObject-End");
			
		} else {
			// Estaba en la Cache => Lo hemos actualizado
			// Habra que decidir si persistirlo siempre o no. En principio no.
			//ret = poExecutorState->GetCO()->SaveValueToDisk();

			// Invalidamos la imagen del disco, por si ya habia sido persistido.
			//ClCacheableObject * poCacheableObject = poOldExecutorState->GetCO();
			//if (poCacheableObject) {
			//	ret = poCacheableObject->Setm_bDisk(M4_FALSE);
			//} else {
			//	// M4_ERROR
			//	if (!bFound)
			//		delete poExecutorState;
			//	return M4_ERROR;
			//}

			// Reemplazamos el objeto antiguo por el nuevo
			ClM4XMLInfo::SetFileInfoByThread("PersistState-ReplaceObject-Start");
			ret2 = GetCache()->ReplaceObject(poOldExecutorState, ai_pccSessionId, poExecutorState);
			ClM4XMLInfo::SetFileInfoByThread("PersistState-ReplaceObject-End");
		}
	}

	// Soltamos la referencia que teniamos para el objeto antiguo si lo tuviesemos
	if (bFound && poOldExecutorState != NULL) {
		ClM4XMLInfo::SetFileInfoByThread("PersistState-FreeObject-Start");
		ret3 = GetCache()->FreeObject(poOldExecutorState);
		ClM4XMLInfo::SetFileInfoByThread("PersistState-FreeObject-End");
	}

	if (ret != M4_SUCCESS) {
		// Posiblemente Error en la Serializacion: M4_ERROR
		// Salida limpia
		delete poExecutorState;
		poExecutorState = NULL;
		return M4_ERROR;
	}


	// Soltamos la referencia que teniamos de la Cache para el nuevo objeto recien insertado, y para el objeto antiguo si lo tuviesemos
	// Aqui es donde se dispara la persistencia por RefreshRatio o por Necesidades de Memoria => Ojo con el Multithreading aqui...
	ClM4XMLInfo::SetFileInfoByThread("PersistState-FreeObject-Start");
	ret3 = GetCache()->FreeObject(poExecutorState);
	ClM4XMLInfo::SetFileInfoByThread("PersistState-FreeObject-End");
	
	// Despues si es necesario persistiremos la Cache
	// ret = ai_poStateCache->PersistToDisk()
	
	ClM4XMLInfo::SetFileInfoByThread("PersistState-End");

	if (ret2 != M4_SUCCESS || ret != M4_SUCCESS || ret3 != M4_SUCCESS) {
		return M4_ERROR;
	}

	return M4_SUCCESS;
}
*/

/*
// Recuperar el estado de una sesion de la cache
// Esta funcion es thread-safe (excepto si el Id es el mismo en dos threads)
// porque sobre el mismo objeto va a haber varios threads llamando a este método.
// Entrada:	ai_pccSessionId		= Sesion ID
//			ai_pChannelManager	= Channel Manager (nuevo) en el que engancharemos la sesion
//			ai_pXMLContext		= XML Context to recover (in/out)
// Salida:	ao_bFound	= M4_TRUE (Encontrado) o M4_FALSE (no encontrado)
//			return = M4_SUCCESS (Ok) o M4_ERROR (fallo)
m4return_t	ClExecutorStateManager::RecoverState(m4pchar_t ai_pccSessionId, ClChannelManager * &ai_pChannelManager, ClM4XMLContext *ai_pXMLContext, m4bool_t& ao_bFound)
{
	m4return_t ret = M4_SUCCESS;
	ao_bFound = M4_FALSE;

	if (!ai_pccSessionId || !ai_pChannelManager || !IsInitialized() )
		return M4_ERROR;
	
	ClM4XMLInfo::SetFileInfoByThread("RecoverState-Start");

	M4DataStorage* poInputDataStorage = NULL;
	ClExecutorState* poExecutorState = NULL;

	// Buscar SessionId en la Cache, y pillarlo pero en exclusiva, sin bloquear: GET_EXNBLOCK (esto era antes)
	// Buscar SessionId en la Cache, y pillarlo pero no en exclusiva: GET_SHARED
	ClM4XMLInfo::SetFileInfoByThread("RecoverState-GetObject-Start");
	ret = GetCache()->GetObject(ai_pccSessionId, poExecutorState, ao_bFound, ClCacheInterface::GET_SHARED);
	ClM4XMLInfo::SetFileInfoByThread("RecoverState-GetObject-End");

	if (ret != M4_SUCCESS) {
		return M4_ERROR;	// Error en la cache
	}
	// Importante !!!!
	// 1. Si lo encontramos en la cache, y obtenemos el mismo objeto <poExecutorState>
	// en las llamadas de dos threads => nos vamos al garete...
	// Esto en teoría no va a suceder nunca, pero además como el GetObject es GET_EXNBLOCK,
	// el segundo thread no obtendrá el objeto sino NULL... con lo cual creerá que no está en la Cache...
	// y esto puede ser peor... porque podemos meter dos objetos con el mismo Id en la cache....
	// Así que a tener muy encuenta !!! ESTO ERA ANTES !!!!

	// 2. Ahora, si lo encontramos en la cache, y obtenemos el mismo objeto <poExecutorState>
	// en las llamadas de dos threads => NO nos vamos al garete...
	// Esto en teoría puede suceder, pero además como el GetObject es GET_SHARED,
	// el segundo thread obtendrá el mismo objeto... A partir de ahí... mucho cuidado
	// Así que a tener muy encuenta !!!

	
	if (!ao_bFound) {
		return M4_SUCCESS;	// No encontrado en la cache
	}
	// Objeto en la Cache => Obtenemos referencia al DataStorage
	//poInputDataStorage = poExecutorState->GetDataStorage();	// Antigua forma
	// Creamos un DataStorage temporal como iterador del bloque de unidades del DataStorage matriz.
	poInputDataStorage = M4DataStorage::GetNewDataStorage( poExecutorState->GetDataStorage()->GetDataUnitInterfaceQueue() );


	// Attach SESSION_CHANNEL - Esto no hay que hacerlo, se hace fuera
	//ret = AttachSessionChannel(ai_pChannelManager, ai_poSessionChannel);
	//if (ret != M4_SUCCESS) {
	//	return M4_ERROR;
	//}

	// Creamos el IODriver que sabe como serializar.
	ClDataStoreIODriver oDSIOD;	// Sabe como serializar

	// Inicializamos IODriver, usando el DataStorage (salida de datos)
	ret = oDSIOD.Init(GEN_IO_MODE_READ, poInputDataStorage);

#if _M4_DEBUG > 10
	// CheckSum de comprobación
	// checks the check sum
	M4_ASSERT( oDSIOD.CheckSum(poInputDataStorage) == M4_TRUE );
#endif

	// Creamos ObjectDirectory temporal: ClCMObjectDirectory	- gestor de objetos a deserializar
	ClCMObjectDirectory oObjDir(ai_pChannelManager);

	if (ret == M4_SUCCESS) {
		// Deserializamos el Estado
		ClM4XMLInfo::SetFileInfoByThread("RecoverState-ObjdirDeserialize-Start");
		ret = oObjDir.DeSerialize(oDSIOD);		// Sesion

		string sTemp("RecoverState-ObjdirDeserialize-End");
		char pcTemp[64];
		sprintf(pcTemp,"\t%d",oObjDir.GetCount());
		sTemp += pcTemp;
		ClM4XMLInfo::SetFileInfoByThread(sTemp.c_str());
		
#if _M4_DEBUG > 10
		// Volcado de los estados a disco... para depuracion (ver comentario en el inicio)
		if ( ret != M4_SUCCESS ) {
			// Error en Deserializacion
			static int iCountError = 0;
			{
				ClMutBlock oMut(&m_oMutex);
				iCountError++;
			}
			m4char_t pszFileName[100];
			sprintf(pszFileName, "c:\\temp\\state_%d_%s.sal", iCountError, ai_pccSessionId );

			cerr << "Error " << iCountError << " deserialize state: " << ai_pccSessionId << endl;
			cerr.flush();

			// Volcamos a disco el estado que dio problemas
			ClFileIODriver oFileIOD;
			oFileIOD.Init(GEN_IO_MODE_WRITE, pszFileName);
			ClM4XMLInfo::SetFileInfoByThread("RecoverState-ExecutorSerialize-Start");
			poExecutorState->Serialize(oFileIOD);	// Ojo con este serialize que no es reentrante para el mismo objeto
			ClM4XMLInfo::SetFileInfoByThread("RecoverState-ExecutorSerialize-End");
			oFileIOD.End(M4_TRUE);
		} else {
			// OK en Serializacion
			static int iCountSucces = 0;
			{
				ClMutBlock oMut(&m_oMutex);
				iCountSucces++;
			}
			m4char_t pszFileName[100];
			sprintf(pszFileName, "c:\\temp\\state_ok_%d_%s.sal", iCountSucces, ai_pccSessionId );

			cerr << "OK " << iCountSucces << " deserialize state: " << ai_pccSessionId << endl;
			cerr.flush();

			// Volcamos a disco el estado que dio problemas
			ClFileIODriver oFileIOD;
			oFileIOD.Init(GEN_IO_MODE_WRITE, pszFileName);
			ClM4XMLInfo::SetFileInfoByThread("RecoverState-ExecutorSerialize-Start");
			poExecutorState->Serialize(oFileIOD);	// Ojo con este serialize que no es reentrante para el mismo objeto
			ClM4XMLInfo::SetFileInfoByThread("RecoverState-ExecutorSerialize-End");
			oFileIOD.End(M4_TRUE);
		}
#endif

		// Deserializamos el Contexto
		if ( ai_pXMLContext && ret==M4_SUCCESS ) {
			ClM4XMLInfo::SetFileInfoByThread("RecoverState-ContextDeserialize-Start");
			ret = ai_pXMLContext->Deserialize(oDSIOD);
			ClM4XMLInfo::SetFileInfoByThread("RecoverState-ContextDeserialize-End");
		}
		//	-	Cerramos el IODriver
		oDSIOD.End();
	}

	// Borramos el DataStorage Temporal
	if (poInputDataStorage) {
		M4DataStorage::DeleteDataStorage( poInputDataStorage );
		poInputDataStorage = NULL;
	}

	// Una vez deserializado el estado, se supone que se ha enganchado al ChannelManager
	if (ret != M4_SUCCESS) {
		// Estaba en la Cache => Soltamos la referencia que teniamos de la Cache.
		ClM4XMLInfo::SetFileInfoByThread("RecoverState-FreeObject-Start");
		GetCache()->FreeObject(poExecutorState);
		ClM4XMLInfo::SetFileInfoByThread("RecoverState-FreeObject-End");
		return M4_ERROR;
	}

	// Marcamos todos los objetos, para poder destruir el ObjectDirectory sin destruirlos
	oObjDir.SetKeepAliveAll(M4_TRUE);

	// Estaba en la Cache => Lo soltamos (soltamos la referencia)
	ClM4XMLInfo::SetFileInfoByThread("RecoverState-FreeObject-Start");
	ret = GetCache()->FreeObject(poExecutorState);
	ClM4XMLInfo::SetFileInfoByThread("RecoverState-FreeObject-End");
	
	ClM4XMLInfo::SetFileInfoByThread("RecoverState-End");

	return ret;
}
*/


// Check State object from Cache
// Entrada: ai_pccSessionId = Session ID
// Salida: ao_bFound = encontrado o no
m4return_t	ClExecutorStateManager::CheckState(m4pchar_t ai_pccSessionId, m4bool_t& ao_bFound)
{
	m4return_t ret = M4_SUCCESS;

	ClM4XMLInfo::SetFileInfoByThread("CheckState-Start");
	
	if (!ai_pccSessionId || !IsInitialized() )
		return M4_ERROR;

	ClExecutorState* poExecutorState = NULL;

	ClM4XMLInfo::SetFileInfoByThread("CheckState-GetObject-Start");
	ret = GetCache()->GetObject(ai_pccSessionId, poExecutorState, ao_bFound, ClCacheInterface::GET_SHARED);
	//ret = GetCache()->GetObject(ai_pccSessionId, poExecutorState, ao_bFound);
	ClM4XMLInfo::SetFileInfoByThread("CheckState-GetObject-End");

	if (ret==M4_SUCCESS && ao_bFound == M4_TRUE)
	{
		ClM4XMLInfo::SetFileInfoByThread("CheckState-FreeObject-Start");
		GetCache()->FreeObject(poExecutorState);
		ClM4XMLInfo::SetFileInfoByThread("CheckState-FreeObject-End");
	}
	
	ClM4XMLInfo::SetFileInfoByThread("CheckState-End");

	return ret;

}

// Eliminar un estado de una sesion de la cache
// Esta funcion debe ser thread-safe... 
// porque sobre el mismo objeto va a haber varios threads llamando a este método.
// En principio lo es.... porque lo es la Cache.
// Entrada:	ai_pccSessionId = Sesion ID
// Salida:	ao_piRemovedCount = Puntero a variable donde dejar el numero de estados borrados
//			return = M4_SUCCESS (Ok) o M4_ERROR (fallo)
m4return_t	ClExecutorStateManager::RemoveState(m4pchar_t ai_pccSessionId, m4puint32_t ao_piRemovedCount)
{
	m4return_t ret = M4_SUCCESS;

	if (!ai_pccSessionId || !IsInitialized() )
		return M4_ERROR;

	ClM4XMLInfo::SetFileInfoByThread("RemoveState-Start");
	
	ClM4XMLInfo::SetFileInfoByThread("RemoveState-RemoveObjectById-Start");
	RemoveFromSessionMap(ai_pccSessionId);
	ret = GetCache()->RemoveObjectById( ai_pccSessionId, ao_piRemovedCount );
	ClM4XMLInfo::SetFileInfoByThread("RemoveState-RemoveObjectById-End");
	if (ret != M4_SUCCESS) {
		return M4_ERROR;
	}

	
	ClM4XMLInfo::SetFileInfoByThread("RemoveState-End");

	return ret;
}

// Deletes session map entries
void ClExecutorStateManager::RemoveFromSessionMap(m4pcchar_t ai_pccSessionId)
{
	ClMutBlock oBlock(&m_oSessionMutex);

	// gets the string until '-'
	m4pcchar_t pcRef = strstr(ai_pccSessionId,"-");
	m4int16_t iSize = 0;
	
	if (pcRef == NULL)
		iSize = strlen(pcRef);
	else
		iSize = pcRef - ai_pccSessionId;

	m4pchar_t pcSession = new m4char_t [iSize+1];
	strncpy(pcSession,ai_pccSessionId,iSize);
	pcSession[iSize] = '\0';

	SMSessionCounterIt it = m_oSessionCounters.find(pcSession);

	if (it != m_oSessionCounters.end())
	{
		m_oSessionCounters.erase(it);
	}

	delete [] pcSession;
}

// Gets and updates the current session counter
m4uint32_t ClExecutorStateManager::UpdateSessionMap(m4pcchar_t ai_pccSessionId)
{
	ClMutBlock oBlock(&m_oSessionMutex);

	m4uint32_t iResult;

	SMSessionCounterIt it = m_oSessionCounters.find(ai_pccSessionId);

	if (it == m_oSessionCounters.end())
	{
		m_oSessionCounters.insert(SMSessionCounterMap::value_type(ai_pccSessionId,1));
		iResult = 0;
	}
	else
	{
		iResult = ((*it).second)++;
	}

	return iResult;
}

// Load Cache from Disk
// Esta funcion debe ser ejecutada solo si no hay ningun thread haciendo: PersistState o RecoverState
m4return_t	ClExecutorStateManager::LoadCache()
{
	
	ClM4XMLInfo::SetFileInfoByThread("LoadCache-Start");
	
	// Seccion critica
	ClMutBlock oBlock(&m_oMutex);
	
	m4return_t ret = M4_ERROR;
	if ( IsInitialized() ) {
		ClM4XMLInfo::SetFileInfoByThread("LoadCache-DePersistCache-Start");
		ret = GetCache()->DePersistCache();
		ClM4XMLInfo::SetFileInfoByThread("LoadCache-DePersistCache-End");
	}
	
	ClM4XMLInfo::SetFileInfoByThread("LoadCache-End");
	return ret;
}


// Persist Cache to Disk
// Esta funcion debe ser ejecutada solo si no hay ningun thread haciendo: PersistState o RecoverState
m4return_t	ClExecutorStateManager::PersistCache()
{
	
	ClM4XMLInfo::SetFileInfoByThread("PersistCache-Start");
	
	// Seccion critica
	ClMutBlock oBlock(&m_oMutex);

	m4return_t ret = M4_ERROR;
	if ( IsInitialized() ) {
		ClM4XMLInfo::SetFileInfoByThread("PersistCache-PersistCache-Start");
		ret = GetCache()->PersistCache();
		ClM4XMLInfo::SetFileInfoByThread("PersistCache-PersistCache-End");
	}
	
	ClM4XMLInfo::SetFileInfoByThread("PersistCache-End");
	return ret;
}

// Release objects from Cache (Memory and Disk)
// Esta funcion debe ser ejecutada solo si no hay ningun thread haciendo: PersistState o RecoverState
m4return_t	ClExecutorStateManager::CleanCache()
{
	
	ClM4XMLInfo::SetFileInfoByThread("CleanCache-Start");
	
	// Seccion critica
	ClMutBlock oBlock(&m_oMutex);
	
	m4return_t ret = M4_ERROR;
	if ( IsInitialized() ) {
		ClM4XMLInfo::SetFileInfoByThread("CleanCache-RemoveAll-Start");
		ret = GetCache()->RemoveAll();
		ClM4XMLInfo::SetFileInfoByThread("CleanCache-RemoveAll-End");
	}
	
	ClM4XMLInfo::SetFileInfoByThread("CleanCache-End");
	return ret;
}

// Dump ExecutorState Cache
void ClExecutorStateManager::DumpCache(m4pchar_t ai_pcCalledFrom, ostream *ai_pStream) const 
{
	/*
	static m4bool_t bFileCreated = M4_FALSE;
	m4uint8_t	iSystemDebug;
    m4pcchar_t	pccPath;
    m4char_t	acFile[ M4CH_MAX_FILE + 1 ];
    ostream     *pStream = ai_pStream;

	if (ai_pStream == NULL)
	{

		iSystemDebug = m_poEnvironment->GetSystemDebugEnable() ;
		
		if( iSystemDebug < 2 )
		{
			return;
		}
    
		pccPath = m_poEnvironment->GetDebugDirectory() ;
		
		if( pccPath != 0 && *pccPath != '\0' )
		{
			sprintf( acFile, "%s%s%s", pccPath, M4_BACK_SLASH, "CacheDump.txt" ) ;
		}
		else
		{
			sprintf( acFile, "%s", "CacheDump.txt" ) ;
		}
		
		if (M4_FALSE == bFileCreated)
		{
			// UNICODE FILE
			eUniFileEncodingType_t eEncoding = M4UniNative ;

			M4Open( *pStream, acFile, M4UniWrite, eEncoding ) ;
			bFileCreated = M4_TRUE;
		}
		else
		{
			// UNICODE FILE
			eUniFileEncodingType_t eEncoding = M4UniNative ;

			M4Open( *pStream, acFile, M4UniAppend, eEncoding ) ;
		}
	}

    if( pStream != NULL )
    {
		ostream &Output = *pStream;

		ClDumpCacheVisitor oCacheVisitor(pStream);

		oCacheVisitor.SetDumpDates(M4_FALSE);
		
		ClBaseCacheInterface * pCache = 0;

		if (ai_pcCalledFrom == 0)
			ai_pcCalledFrom = "";

	    Output << "============ Dumping Caches ===============================================" << endl;
		Output << "   Called from <" << ai_pcCalledFrom << ">" << endl;

		Output << endl << "Metadata Cache (CMCR)-------------" << endl;
		pCache = (ClBaseCacheInterface *) m_poFactory->GetCache();
		if (pCache)
		{
			pCache->Dump(oCacheVisitor);
		}
		else
		{
			Output << "Cache not active." << endl;
		}

		Output << endl << "Security Cache (CSCR)-------------" << endl;
		pCache = (ClBaseCacheInterface *) m_poFactory->GetCSCRFactory()->GetCache();
		if (pCache)
		{
			pCache->Dump(oCacheVisitor);
		}
		else
		{
			Output << "Cache not active." << endl;
		}

		Output << endl << "Presentation Cache-------------" << endl;
		if ((ClBaseCacheInterface *)GetPresFac())
		{
			pCache = (ClBaseCacheInterface *)GetPresFac()->GetCache();
		}
		else
		{ 
			pCache = 0;
		}
		if (pCache)
		{
			pCache->Dump(oCacheVisitor);
		}
		else
		{
			Output << "Cache not active." << endl;
		}

		Output << endl << "Data Cache-------------" << endl;
		pCache = (ClBaseCacheInterface *)GetCache();
		if (pCache)
		{
			pCache->Dump(oCacheVisitor);
		}
		else
		{
			Output << "Cache not active." << endl;
		}
		
		Output <<  endl << endl;

		if (ai_pStream == NULL)
		{
			delete ((ofstream*) pStream);
		}
	}
	*/
}

