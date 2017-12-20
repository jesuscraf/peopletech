//	==============================================================================
//
//	 (c) Copyright  1991-1999 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            Executive
//	 File:              ClExecutorCache.hpp
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

#ifndef _CLEXECUTORCACHE_HPP_
#define _CLEXECUTORCACHE_HPP_


// Redefinimos para evitar conflictos con los nombres de M4ObjEngine
//#ifndef ClExecutorStateCache
//#define ClExecutorStateCache ClExecutorStateCache2
//#endif


#include "m4stl.hpp"
#include "m4exeforchannel_dll.hpp"
#include "clexecutorstate.hpp"		//m4exeforchannel
#include "clcache.hpp"
#include "clgeniod.hpp"	// m4serial
#include "diskpers.hpp"	// m4serial





class M4_DECL_M4EXEFORCHANNEL ClExecutorStateCacheTree : public ClCacheTree
{
public:

	ClExecutorStateCacheTree(m4uint32_t ai_ulDefaultPolicy = 0, m4uint32_t ai_ulMaxSizeCache = 10000000, m4uint32_t ai_ulMaxNOfObjs = 1000, m4uint32_t ai_ulRefreshRatio = 50);

	virtual m4return_t NumOfObjectsControl ();

	// Deshabilitamos el refresh
	virtual m4return_t Refresh (ClSelfPersistiableObject *   ai_poSelfPersistor) { return M4_SUCCESS; }

    virtual m4return_t PutObject (ClCacheableObject *ai_pObject);

};









// Comentado ==============================================
#if 0

// Key		= string
// Object	= ClExecutorState *
typedef multimap< string, ClExecutorState *,  less<string> > ExecutorStateMap;

// Clase que representa la cache, pero basada en un multimapa simple.
class M4_DECL_M4EXEFORCHANNEL ClExecutorStateCacheMap : public ClBaseCacheInterface
{
private:
	// Mapa de ExecutorState (simulacion de Cache)
	ExecutorStateMap *		m_poCacheMap;

	// Mutex para secciones criticas
	ClMutex					m_oMutex;

public:
	ClExecutorStateCacheMap();

	~ClExecutorStateCacheMap();

	void Init();

	// Get a reference to an object (not in use) in Cache
	m4return_t GetObject(m4pchar_t ai_pcIdSession, ClExecutorState* &ao_poObject, m4bool_t &ao_bFound);

	// Insert a new object in Cache
	m4return_t PutObject(m4pchar_t ai_pcIdSession, ClExecutorState* ai_poObject);

	// Replace an object in Cache with another object
	m4return_t ReplaceObject(m4pchar_t ai_pcIdSession, ClExecutorState* ai_poObjectOld, ClExecutorState* ai_poObjectNew);

	// Free a reference to an object in Cache (obtains via GetObject)
	m4return_t FreeObject(ClExecutorState* aio_poObject);

	// Remove an object in Cache
	m4return_t RemoveObject(ClExecutorState* aio_poObject);

	// Remove all objects in Cache with this Id
	m4return_t RemoveObjectById(m4pchar_t ai_pcIdSession, m4puint32_t ao_piRemovedCount);

	// Remove objects from cache for a given Id and Version
	m4return_t RemoveObjectByIdAndVersion( m4pchar_t ai_pcId, m4date_t ai_dVersion, ClBaseCacheInterface::eRemoveType_t ai_iRemoveType );

	// Remove all objects in Cache
	m4return_t RemoveAllObjects();

	// Remove all objects in Cache for the Date
	m4return_t RemoveAll(m4date_t ai_dDate = 0);

	// Persist Cache
	m4return_t PersistCache();

	// DePersist Cache
	m4return_t DePersistCache();

	// Delete Cache
	m4return_t DeleteCache();

	// Number Objects in Cache
	m4return_t GetNumObjects(m4uint32_t & ao_iNumObjects);

	// Next element in Cache with this Id
	m4return_t GetNextId(m4pchar_t & ao_pcId, void * &  ai_poIterator);

	// Get Cache Type
	m4uint16_t GetType(void);

	// Dump Cache
	m4return_t Dump(ClDumpCacheVisitor &ai_oVisitor);

	// Get a reference to an object (in use) in Cache
	m4return_t GetThisObject(m4pchar_t ai_pcIdSession, ClExecutorState* &ao_poObject, m4bool_t &ao_bFound);

protected:
	enum FindAction { FindFirst=1, FindInUse, FindNotInUse, FindThis };

	ClExecutorState * Find(const string &ai_stKey, ClExecutorStateCacheMap::FindAction ai_eAction, ClExecutorState *ai_poObjectFind = 0);

};

//--------------------------------------------
//	ClExecutorStateCacheMap
//--------------------------------------------
ClExecutorStateCacheMap::ClExecutorStateCacheMap()
{
	m_oMutex.Init();
	m_poCacheMap = NULL;
}

ClExecutorStateCacheMap::~ClExecutorStateCacheMap()
{
	if (m_poCacheMap) {
		delete m_poCacheMap;
		m_poCacheMap = NULL;
	}
}

void ClExecutorStateCacheMap::Init()
{
	m_poCacheMap = new ExecutorStateMap();
}

// Find an object in Cache by Key
ClExecutorState *ClExecutorStateCacheMap::Find(const string &ai_stKey, ClExecutorStateCacheMap::FindAction ai_eAction, ClExecutorState *ai_poObjectFind /*=0*/ )
{
	m4return_t ret = M4_SUCCESS;

	// Valores de salida por defecto
	ClExecutorState* poExecutorState = NULL;

	// Iterador
	ExecutorStateMap::iterator iterMap;
	
	// Busqueda
	iterMap = m_poCacheMap->find( ai_stKey );

	if (iterMap == m_poCacheMap->end())
		return NULL;

	// Busqueda secuencial entre los que tienen la misma clave
	m4uint32_t ulNumOfObjSameKey = m_poCacheMap->count(ai_stKey);
	
	for (m4uint32_t Counter=0 ; Counter < ulNumOfObjSameKey ; Counter++) {
	
		poExecutorState = (*iterMap).second;
		
		switch (ai_eAction) {
		case FindInUse:
			{
				// El primero que esté en uso
				if ( poExecutorState->IsInUse() ) {
					return poExecutorState;
				}
			}
			break;

		case FindNotInUse:
			{
				// El primero que NO esté en uso
				if ( !poExecutorState->IsInUse() ) {
					return poExecutorState;
				}
			}
			break;

		case FindThis:
			{
				// Este objeto concreto
				if ( ai_poObjectFind && (ai_poObjectFind==poExecutorState) )
					return poExecutorState;
			}

		case FindFirst:
		default:
			{
				// El primero que encuentre
				return poExecutorState;
			}
		}
		
		// checks for the next same key 
		iterMap++;
	}

	return NULL;
}

// Get a reference to an object (not in use) in Cache
m4return_t ClExecutorStateCacheMap::GetObject(m4pchar_t ai_pcIdSession, ClExecutorState* &ao_poObject, m4bool_t &ao_bFound)
{
	ClMutBlock oBlock(&m_oMutex);

	// Valores de salida por defecto
	ao_bFound = M4_FALSE;
	ao_poObject = NULL;

	if (!ai_pcIdSession) {
		return M4_ERROR;
	}

	string stKey(ai_pcIdSession);

	ao_poObject = Find(stKey, ClExecutorStateCacheMap::FindNotInUse );
	
	if (ao_poObject) {
		// Marcamos uso
		ao_bFound = M4_TRUE;
		ao_poObject->SetUsing(M4_TRUE);
	}
	
	return M4_SUCCESS;
}

// Get a reference to an object (in use) in Cache
m4return_t ClExecutorStateCacheMap::GetThisObject(m4pchar_t ai_pcIdSession, ClExecutorState* &ao_poObject, m4bool_t &ao_bFound)
{
	ClMutBlock oBlock(&m_oMutex);

	// Valores de salida por defecto
	ao_bFound = M4_FALSE;

	if (!ai_pcIdSession) {
		return M4_ERROR;
	}

	string stKey(ai_pcIdSession);

	ClExecutorState *poExecutorState = Find( stKey, ClExecutorStateCacheMap::FindThis, ao_poObject );
	
	if (poExecutorState) {
		// encontrada referencia, debe ser la misma
		if (poExecutorState != ao_poObject)
			return M4_ERROR;

		ao_bFound = M4_TRUE;
	}
	
	return M4_SUCCESS;
}

// Insert a new object in Cache
m4return_t ClExecutorStateCacheMap::PutObject(m4pchar_t ai_pcIdSession, ClExecutorState* ai_poObject)
{
	ClMutBlock oBlock(&m_oMutex);

	if (!ai_pcIdSession || !ai_poObject) {
		return M4_ERROR;
	}

	// Clave y Objeto no son el mismo
	if ( strcmp( ai_pcIdSession, ai_poObject->GetSessionId() ) != 0 ) {
		return M4_ERROR;
	}

	string stKey(ai_pcIdSession);

	// Iterador
	ExecutorStateMap::iterator iterMap;

	// Busqueda
	iterMap = m_poCacheMap->find( stKey );

	m4bool_t bFound = M4_TRUE;

	if (iterMap == m_poCacheMap->end()) {
		bFound = M4_FALSE;
	}

	if (bFound) {
		ClExecutorState* pExecutorStateOld = (*iterMap).second;
		if ( !pExecutorStateOld) {
			return M4_ERROR;	// No existe
		}

		if ( pExecutorStateOld == ai_poObject) {
			return M4_SUCCESS;	// son el mismo :-)
		}

		// El que vamos a reemplazar (Save As) puede estar en uso por otro thread
		if ( pExecutorStateOld->IsInUse() ) {
			// Le marcamos para borrado
			pExecutorStateOld->SetSerialized(M4_TRUE);
		} else {
			// replace
			m_poCacheMap->erase( iterMap );
			delete pExecutorStateOld;
			pExecutorStateOld = NULL;
		}
	}

	// Insert new object
	ai_poObject->SetUsing(M4_TRUE);
	m_poCacheMap->insert( ExecutorStateMap::value_type(stKey, ai_poObject) );

	return M4_SUCCESS;
}

// Replace an object in Cache with another object
m4return_t ClExecutorStateCacheMap::ReplaceObject(m4pchar_t ai_pcIdSession, ClExecutorState* ai_poObjectOld, ClExecutorState* ai_poObjectNew)
{
	ClMutBlock oBlock(&m_oMutex);

	if (!ai_pcIdSession || !ai_poObjectOld || !ai_poObjectNew ) {
		return M4_ERROR;
	}

	// Clave y Objeto no son el mismo
	if ( strcmp( ai_pcIdSession, ai_poObjectOld->GetSessionId() ) != 0 ) {
		return M4_ERROR;
	}
	if ( strcmp( ai_pcIdSession, ai_poObjectNew->GetSessionId() ) != 0 ) {
		return M4_ERROR;
	}

	string stKey(ai_pcIdSession);

	// Iterador
	ExecutorStateMap::iterator iterMap;

	// Busqueda
	iterMap = m_poCacheMap->find( stKey );

	if (iterMap == m_poCacheMap->end())
		return M4_ERROR;

	// Busqueda secuencial entre los que tienen la misma clave
	m4uint32_t ulNumOfObjSameKey = m_poCacheMap->count(stKey);
	
	for (m4uint32_t Counter=0 ; Counter < ulNumOfObjSameKey ; Counter++) {

		if ( (*iterMap).second == ai_poObjectOld ) {
			
			delete ai_poObjectOld;
			ai_poObjectOld = NULL;
			(*iterMap).second = ai_poObjectNew;
			
			// remove old
			//m_poCacheMap->erase( iterMap );
			// insert new
			//m_poCacheMap->insert( ExecutorStateMap::value_type(stKey, ai_poObjectNew) );

			return M4_SUCCESS;
		}

		// checks for the next same key 
		iterMap++;
	}

	return M4_ERROR;
}

// Free a reference to an object in Cache (obtains via GetObject)
m4return_t ClExecutorStateCacheMap::FreeObject(ClExecutorState* aio_poObject)
{
	ClMutBlock oBlock(&m_oMutex);

	if (!aio_poObject) {
		return M4_ERROR;
	}

	string stKey( aio_poObject->GetSessionId() );

	// Valores por defecto
	ClExecutorState* poExecutorState = NULL;

	// Iterador
	ExecutorStateMap::iterator iterMap;
	
	// Busqueda
	iterMap = m_poCacheMap->find( stKey );

	if (iterMap == m_poCacheMap->end())
		return NULL;

	// Busqueda secuencial entre los que tienen la misma clave
	m4uint32_t ulNumOfObjSameKey = m_poCacheMap->count(stKey);
	
	for (m4uint32_t Counter=0 ; Counter < ulNumOfObjSameKey ; Counter++) {
	
		poExecutorState = (*iterMap).second;
		
		// Este objeto concreto
		if ( poExecutorState && (aio_poObject==poExecutorState) ) {
			// encontrado

			// Soltamos referencia
			poExecutorState->SetUsing(M4_FALSE);

			// Si esta marcado para borrado => lo borramos (o lo pasamos a la cola de serializados)
			if ( poExecutorState->IsSerialized() ) {
				// remove
				poExecutorState->SetSerialized(M4_FALSE);

				m_poCacheMap->erase( iterMap );
				
				delete poExecutorState;
			}

			return M4_SUCCESS;
		}
		
		// checks for the next same key 
		iterMap++;
	}

	// no encontrado
	return M4_ERROR;
}

// Remove an object in Cache
m4return_t ClExecutorStateCacheMap::RemoveObject(ClExecutorState* aio_poObject)
{
	ClMutBlock oBlock(&m_oMutex);

	if (!aio_poObject) {
		return M4_ERROR;
	}

	string stKey(aio_poObject->GetSessionId());

	// Iterador
	ExecutorStateMap::iterator iterMap;

	ClExecutorState *poExecutorState = NULL;

	// Busqueda
	iterMap = m_poCacheMap->find( stKey );

	if (iterMap == m_poCacheMap->end())
		return M4_ERROR;

	// Busqueda secuencial entre los que tienen la misma clave
	m4uint32_t ulNumOfObjSameKey = m_poCacheMap->count(stKey);
	
	for (m4uint32_t Counter=0 ; Counter < ulNumOfObjSameKey ; Counter++) {

		poExecutorState = (*iterMap).second;

		// encontrado
		if ( poExecutorState == aio_poObject ) {
			
			// si esta en uso => no se puede borrar
			if ( poExecutorState->IsInUse() ) {
				//return M4_ERROR;

				// Marcamos para borrado => serializar
				poExecutorState->SetSerialized(M4_TRUE);
			
			} else {
				// remove
				m_poCacheMap->erase( iterMap );

				delete poExecutorState;
			}

			return M4_SUCCESS;
		}

		// checks for the next same key 
		iterMap++;
	}
	
	return M4_ERROR;
}

// Remove all objects in Cache with this Id
m4return_t ClExecutorStateCacheMap::RemoveObjectById(m4pchar_t ai_pcPortionKey, m4puint32_t ao_piRemovedCount)
{
	ClMutBlock oBlock(&m_oMutex);

	m4uint32_t iRemovedCount;
	if (!ao_piRemovedCount) {
		ao_piRemovedCount = &iRemovedCount;
	}

	*ao_piRemovedCount = 0;

	if (!ai_pcPortionKey) {
		return M4_ERROR;
	}

	// Iterador
	ExecutorStateMap::iterator iterMap;
	ExecutorStateMap::iterator iterMapNext;

	ClExecutorState *poExecutorState = NULL;
	
	// Busqueda secuencial entre todos los elementos
	m4uint32_t ulNumOfElemInCache = m_poCacheMap->size();

	for (iterMap = m_poCacheMap->begin(); iterMap != m_poCacheMap->end(); iterMap = iterMapNext ) {
		iterMapNext = iterMap;
		iterMapNext++;

		poExecutorState = (*iterMap).second;

		//Si se cumple ai_pcPortionKey se encuentra al principio de pObjectKey->Getm_pszKey()!!
		if( strstr( poExecutorState->GetSessionId(), ai_pcPortionKey ) == poExecutorState->GetSessionId() ) {
			//Lo hemos encontrado.

			// si esta en uso => no se puede borrar
			if ( poExecutorState->IsInUse() ) {
				//return M4_ERROR;

				// Marcamos para borrado => serializar
				poExecutorState->SetSerialized(M4_TRUE);
			
			} else {
				// Contamos
				if (ao_piRemovedCount) {
					(*ao_piRemovedCount)++;
				}

				m_poCacheMap->erase(iterMap);

				delete poExecutorState;
			}

		}
	}
	
	return M4_SUCCESS;
}

// Remove objects from cache for a given Id and Version
m4return_t ClExecutorStateCacheMap::RemoveObjectByIdAndVersion( m4pchar_t ai_pcId, m4date_t ai_dVersion, ClBaseCacheInterface::eRemoveType_t ai_iRemoveType )
{
	// Esta funcion es reentrante... porque la Cache lo es.

	m4return_t ret = M4_SUCCESS;

	m4uint32_t uiNum;

	ret = RemoveObjectById( ai_pcId, &uiNum );

	return ret;
}


// Remove all objects in Cache
m4return_t ClExecutorStateCacheMap::RemoveAllObjects()
{
	ClMutBlock oBlock(&m_oMutex);

	// Iterador
	ExecutorStateMap::iterator iterMap;
	ExecutorStateMap::iterator iterMapNext;

	ClExecutorState *poExecutorState = NULL;

	iterMap = m_poCacheMap->begin();
	iterMapNext = iterMap;

	// Busqueda secuencial entre todos los elementos
	m4uint32_t ulNumOfElemInCache = m_poCacheMap->size();

	for (m4uint32_t Counter=0 ; Counter < ulNumOfElemInCache ; Counter++) {
		
		iterMapNext++;
		
		poExecutorState = (*iterMap).second;

		// si esta en uso => no se puede borrar
		if ( poExecutorState->IsInUse() ) {
			//return M4_ERROR;

			// Marcamos para borrado => serializar
			poExecutorState->SetSerialized(M4_TRUE);

		} else {
			// remove
			m_poCacheMap->erase(iterMap);

			delete poExecutorState;
		}

		// checks for the next same key 
		iterMap = iterMapNext;
	}
	
	return M4_SUCCESS;
}

m4return_t ClExecutorStateCacheMap::RemoveAll(m4date_t ai_dDate)
{
	return RemoveAllObjects();
}

// Persist Cache
m4return_t ClExecutorStateCacheMap::PersistCache()
{
	return M4_SUCCESS;
}

// DePersist Cache
m4return_t ClExecutorStateCacheMap::DePersistCache()
{
	return M4_SUCCESS;
}

// Delete Cache
m4return_t ClExecutorStateCacheMap::DeleteCache()
{
	return RemoveAll();
}

// Number Objects in Cache
m4return_t ClExecutorStateCacheMap::GetNumObjects(m4uint32_t & ao_iNumObjects)
{
	ao_iNumObjects = m_poCacheMap->size();
	return M4_SUCCESS;
}

// Next element in Cache with this Id
m4return_t ClExecutorStateCacheMap::GetNextId(m4pchar_t & ao_pcId, void * &  ai_poIterator)
{
	return M4_SUCCESS;
}

// Get Cache Type
m4uint16_t ClExecutorStateCacheMap::GetType(void)
{
	return M4_CACHE_SESSION_STATE_CACHE_TYPE;	//chlib\version\inc\cachedir.hpp
}

// Dump Cache
m4return_t ClExecutorStateCacheMap::Dump(ClDumpCacheVisitor &ai_oVisitor)
{
	return M4_SUCCESS;
}

#endif // 0 - Comentado ==============================================


#endif	// _CLEXECUTORCACHE_HPP_
