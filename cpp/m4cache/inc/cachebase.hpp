//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                cachebase.hpp
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                16-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//
//
//
//==============================================================================


#ifndef __CACHEBASE_HPP__
#define __CACHEBASE_HPP__
//## begin module.includes preserve=yes

#include "m4cacheres.hpp"
#include "chlog.hpp"

#include "cacheitfz.hpp"
#include "clcache.hpp"
#include "dumpcache.hpp"
#include "clprsstb.hpp"
#include "datedco.hpp"
#include "clcofac.hpp"
#include "cldefine.hpp"
#include <string.h>


/*
Tenplate base para las caches de metadatos. T es el objeto contenido y S el contenedor.
- T debe derivar de la clase ClPersistiableObject y un interfaz con:
	m4uint32_t	GetSize( void ) const
	m4date_t    GetStartDate( void ) const
	m4date_t    GetEndDate( void ) const
	m4date_t    GetCorStartDate( void ) const
	m4date_t    GetCorEndDate( void ) const
	void SetCO(ClCacheableObject *)
	ClCacheableObject *GetCO(void)

- S debe derivar de la clase ClDatedContainer<T> (siendo T la clase anterior)
*/

//## end module.includes preserve=yes
template <class T, class S>
class ClBaseCache : public ClBaseCacheInterface
{
public:
	ClCache * m_poCache;

	ClPersistor * m_pPersistor;

	ClCacheableObjectFactory * m_pCOFactory;

	m4bool_t m_bCacheOwner;

	m4pchar_t m_pPath;

	m4char_t   m_acId[30];

	m4uint8_t m_iMode;

protected:
	m4return_t _GetObject (m4pchar_t ai_pId, m4date_t ai_roStartDate, m4date_t ai_roEndDate,
		             m4date_t ai_roCorDate, m4uint16_t &ao_iVerCounter, T* &ao_poObject, ClCacheInterface::eGetMode ai_eGetMode = ClCacheInterface::GET_SHARED, m4int32_t ai_iTimeOut = -1);

	m4return_t _PutObject (m4pchar_t ai_pId, m4uint16_t ai_iVerCounter, T* ai_poObject, m4date_t ai_dMaxPeriod, m4date_t ai_dMaxUnusedPeriod, m4bool_t ai_bSaveToDisk = M4_TRUE);

	m4return_t _ReplaceObject (T* ai_poOldObject, m4pchar_t ai_pId, m4uint16_t ai_iVerCounter, T* ai_poNewObject, m4date_t ai_dMaxPeriod, m4date_t ai_dMaxUnusedPeriod, m4bool_t ai_bSaveToDisk = M4_TRUE);
public:
	ClBaseCache(ClCache * ai_poCache, m4pcchar_t ai_pPath, m4pcchar_t ai_pId, m4uint8_t ai_iMode, m4uint32_t ai_iMaxMemory, m4uint32_t ai_iNumObjects, m4uint32_t ai_iRefreshRatio, m4double_t ai_dMaxPeriod, m4double_t ai_dMaxUnusedPeriod);

	void Init (void);

	~ClBaseCache ();

    virtual m4return_t PersistCache  (void);

	virtual m4return_t DePersistCache(void);
	
	virtual m4return_t DeleteCache(void);

	m4return_t FreeObject (T* ai_poObject);	

	m4return_t RemoveObject (T* ai_poObject);	
	
	m4return_t RemoveAll(m4date_t ai_dDate = 0);

	m4return_t GetNumObjects (m4uint32_t & ao_iNumObjects);

	m4return_t GetNextId (m4pchar_t & ao_pcId, void * &  ai_poIterator);

	virtual m4return_t Dump(ClDumpCacheVisitor &ai_oVisitor);

	m4return_t SetPersistor(ClPersistor * ai_pPersistor);

	m4return_t SetCOFactory(ClCacheableObjectFactory * ai_pCOFactory);

	ClCacheableObjectFactory * GetCOFactory(void);

	void Reload (void);
};


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
	
template <class T, class S>
ClBaseCache<T,S>::ClBaseCache(ClCache * ai_poCache, m4pcchar_t ai_pPath, m4pcchar_t ai_pId, m4uint8_t ai_iMode, m4uint32_t ai_iMaxMemory, m4uint32_t ai_iNumObjects, m4uint32_t ai_iRefreshRatio, m4double_t ai_dMaxPeriod, m4double_t ai_dMaxUnusedPeriod)
{
//## begin ClBaseCache::ClBaseCache%-564599218.body preserve=yes
	if (!ai_pPath)
		ai_pPath = "";

	m_pPath = new m4char_t[strlen(ai_pPath) + 1];
	strcpy(m_pPath, ai_pPath);

	m_pPersistor = 0;

	m_pCOFactory = 0;

	strcpy(m_acId, ai_pId);

	m_iMode = ai_iMode;

	if (ai_poCache)
	{
		m_poCache = ai_poCache;

		m_bCacheOwner = M4_FALSE;
	}
	else
	{
		M4ClTimeStamp Start;
		Start.now();

		ClCacheTree *poClCacheTree;

		poClCacheTree = new ClCacheTree(ai_dMaxPeriod, ai_dMaxUnusedPeriod, ai_iMaxMemory, ai_iNumObjects,  ai_iRefreshRatio);

		m_poCache = new ClCacheTest();

		m_poCache->SetCacheStyle(poClCacheTree);

		m_bCacheOwner = M4_TRUE;
	}
//## end ClBaseCache::ClBaseCache%-564599218.body
}


template <class T, class S>
void ClBaseCache<T,S>::Init(void)
{
//## begin ClBaseCache::Init%129420952.body preserve=yes
	//Bloqueamos para que no lo hagan dos a la vez.
	//oCOFactory.m_oMutex.Lock();
	
	if (!m_poCache->GetObjId())
	{		
		//Obj ID
		ClTypedId oTypeID( m_acId );
		ClUniqueKey oUniqueKey("");
		ClEnvironmentId oEnvId( m_pPath );

		ClObjectId * poObjID = new ClObjectId (CacheObjectType, &oEnvId, &oTypeID, &oUniqueKey);

		m_poCache->SetObjId(poObjID);

		m_poCache->SetCOFactory(m_pCOFactory);

		if (m_iMode & M4CL_CACHE_WRITE )
			m_poCache->Setm_poPersistor(m_pPersistor);

		if (m_iMode & M4CL_CACHE_READ )
			DePersistCache();		
	}	

	//oCOFactory.m_oMutex.Unlock();
//## end ClBaseCache::Init%129420952.body
}
	
template <class T, class S>
ClBaseCache<T,S>::~ClBaseCache ()
{
//## begin ClBaseCache::~ClBaseCache%96594523.body preserve=yes
	if (m_bCacheOwner == M4_TRUE)
	{
		delete m_poCache;
	}

	if (m_pPath)
		delete [] m_pPath;
//## end ClBaseCache::~ClBaseCache%96594523.body
}


template <class T, class S>
m4return_t ClBaseCache<T,S>::PersistCache(void)
{
//## begin ClBaseCache::PersistCache%-860663218.body preserve=yes
	//En server no persistimos la caché para evitar que cada ejecutor persista.
	//Además la ClVMCacheableObjectFactory, que es un objeto global, fallaría al persistir la caché
	//ya que el subsistema habría borrado la caché anteriormente.
	if (m_iMode & M4CL_CACHE_WRITE )
	{
		if ((m_poCache) && (m_poCache->GetObjId()))
		{
			if (m_pPersistor)
				return m_pPersistor->Persist(*m_poCache);
			else
				return M4_ERROR;
		}
	}

	return M4_SUCCESS;
//## end ClBaseCache::PersistCache%-860663218.body
}


template <class T, class S>
m4return_t ClBaseCache<T,S>::DePersistCache(void)
{
//## begin ClBaseCache::DePersistCache%440761423.body preserve=yes
	if (m_pPersistor)
		return m_pPersistor->DePersist(*m_poCache);
	else
		return M4_ERROR;
//## end ClBaseCache::DePersistCache%440761423.body
}


template <class T, class S>
m4return_t ClBaseCache<T,S>::DeleteCache(void)
{
//## begin ClBaseCache::DeleteCache%-1240407994.body preserve=yes
	if (m_pPersistor)
		return m_pPersistor->Delete(*m_poCache);
	else
		return M4_ERROR;
//## end ClBaseCache::DeleteCache%-1240407994.body
}


template <class T, class S>
m4return_t ClBaseCache<T,S>::_GetObject (m4pchar_t ai_pId, m4date_t ai_roStartDate, m4date_t ai_roEndDate, m4date_t ai_roCorDate, m4uint16_t &ao_iVerCounter, T* &ao_poObject, ClCacheInterface::eGetMode ai_eGetMode, m4int32_t ai_iTimeOut)
{
//## begin ClBaseCache::_GetObject%-1141536043.body preserve=yes
	//Devuelve M4_SUCCESS y en ao_poObject el objeto de la caché si lo encuentra.
	//ao_iVerCounter contiene un numérico que sólo se utliza cuando los objetos cacheados pueden tener la misma
	//clave y distintas fechas (Start End Corr). Se embpieza buscando el Id@000, si existe y sus fechas no
	//son las que buscamos, se busca el Id@001, etc. Si al final no se encuentra, ao_iVerCounter contiene el último ordinal
	//buscado, para que el PutObject pueda insertar el objecto como Id@ao_iVerCounter.
	if (!m_poCache->GetObjId())
	{
		g_oChLog<<BeginWarning(M4_CACHE_NOT_INITIALIZED)<<EndLog;			
		return M4_ERROR;
	}

	m4return_t ret = M4_SUCCESS;
	ClCacheableObject * poCacheableObject = 0;
	S * poContainer;
	m4bool_t bFound = M4_FALSE;

	m4uint32_t iLen = strlen(ai_pId);

	m4pchar_t pcIdNum = new m4char_t[ iLen + 1 + 10];

	ClCacheDatedKey *poKey, *poInternalKey;
	ClCacheKey *poAuxKey;

	ao_poObject = 0;

	ao_iVerCounter = 0;

	// Busca los objetos en la cache
	do
	{
		M4_ASSERT (ao_iVerCounter < 1000);

		sprintf(pcIdNum, "%s@%03d", ai_pId, ao_iVerCounter);

		poKey = new ClCacheDatedKey(pcIdNum, ai_roStartDate, ai_roEndDate,
									ai_roCorDate, ai_roCorDate);	

		// busca en la caché GET_SHARED sin TIME_OUT !!
		//m_poCache->GetObject(poKey, poCacheableObject);

		// Ahora es configurable el tipo: GET_SHARED, GET_EXNBLOCK, GET_EXBLOCK, y el TimeOut (por defecto deshabilitado=-1)
		m_poCache->GetObject(poKey, poCacheableObject, ai_eGetMode, ai_iTimeOut);

		poContainer = (S*)poCacheableObject;
	
		//Comprueba las fechas
		if (poContainer)
		{
			poContainer->GetKey(poAuxKey);

			poInternalKey = (ClCacheDatedKey *)poAuxKey;

			if (poInternalKey->Contains(*poKey))
			{
				ao_poObject = poContainer->GetValue();

				if (!poContainer->GetCache())
					poContainer->SetCache(m_poCache);

				bFound = M4_TRUE;
			}

			//delete poKey;
		}

		ao_iVerCounter++;
	}
	while ((poCacheableObject) && (!bFound));

	ao_iVerCounter--;
	
	if (!poCacheableObject)
	{
		ret = M4_ERROR;
	}
	else
	{
		//Si existe. Todo OK.
		ret = M4_SUCCESS;
	}

	delete poKey;

	delete [] pcIdNum;

	return ret;
//## end ClBaseCache::_GetObject%-1141536043.body
}

template <class T, class S>
m4return_t ClBaseCache<T,S>::_PutObject (m4pchar_t ai_pId, m4uint16_t ai_iVerCounter, T* ai_poObject, m4date_t ai_dMaxPeriod, m4date_t ai_dMaxUnusedPeriod, m4bool_t ai_bSaveToDisk)
{
//## begin ClBaseCache::_PutObject%-1431578528.body preserve=yes
	m4return_t ret;
	ClCacheDatedKey *poKey;

	if (!m_poCache->GetObjId())
	{
		g_oChLog<<BeginWarning(M4_CACHE_NOT_INITIALIZED)<<EndLog;			
		return M4_ERROR;
	}

	m4uint32_t iLen = strlen(ai_pId);

	m4pchar_t pcIdNum = new m4char_t[ iLen + 1 + 10];

	sprintf(pcIdNum, "%s@%03d", ai_pId, ai_iVerCounter);

	poKey = new ClCacheDatedKey(pcIdNum, ai_poObject->GetStartDate(), ai_poObject->GetEndDate(),
						  ai_poObject->GetCorStartDate(), ai_poObject->GetCorEndDate());	

	//Crea el contenedor
	ClCachePolicy *poPolicy = 0;

	if (ai_dMaxPeriod)
	{
		poPolicy = new ClCachePolicy(ai_dMaxPeriod, ai_dMaxUnusedPeriod);
	}
	
	S * poContainer = new S (poPolicy, 0, poKey);

	M4_ASSERT(m_poCache->GetObjId());

	poContainer->SetPersistor(m_pPersistor);

	poContainer->SetEnvId(m_poCache->GetObjId()->GetEnvironmentId());

	poContainer->SetValue(ai_poObject);

	poContainer->SetSize( ai_poObject->GetSize() );

	//le cambiamos el Id.
	ai_poObject->SetObjId(poContainer->CreateObjectIdFromKey());

	ai_poObject->SetCO(poContainer);

	//Graba el contenido a disco
	if ( (m_iMode & M4CL_CACHE_WRITE ) && (ai_bSaveToDisk == M4_TRUE) )
		poContainer->SaveValueToDisk();

	poContainer->SetCache(m_poCache);

	ret = m_poCache->UpdateObject(poContainer);

	if (M4_ERROR == ret)
	{
		//Error, desconectamos el container del object y destruimos el container.
		//A todos los efectos el objeto no está en la caché. espero que el UpdateObject
		//no se haya quedado con nada!!
		if (m_iMode & M4CL_CACHE_WRITE )
			poContainer->DeleteValueFromDisk();
		ai_poObject->SetCO(0);
		poContainer->SetValue(0);
		delete poContainer;
	}

	delete [] pcIdNum;

	return ret;
//## end ClBaseCache::_PutObject%-1431578528.body
}

template <class T, class S>
m4return_t ClBaseCache<T,S>::_ReplaceObject (T* ai_poOldObject, m4pchar_t ai_pId, m4uint16_t ai_iVerCounter, T* ai_poNewObject, m4date_t ai_dMaxPeriod, m4date_t ai_dMaxUnusedPeriod, m4bool_t ai_bSaveToDisk)
{
//## begin ClBaseCache::_ReplaceObject%1802983298.body preserve=yes
	m4return_t ret;
	ClCacheDatedKey *poKey;

	if (!m_poCache->GetObjId())
	{
		g_oChLog<<BeginWarning(M4_CACHE_NOT_INITIALIZED)<<EndLog;			
		return M4_ERROR;
	}

	m4uint32_t iLen = strlen(ai_pId);

	m4pchar_t pcIdNum = new m4char_t[ iLen + 1 + 10];

	sprintf(pcIdNum, "%s@%03d", ai_pId, ai_iVerCounter);

	poKey = new ClCacheDatedKey(pcIdNum, ai_poNewObject->GetStartDate(), ai_poNewObject->GetEndDate(),
						  ai_poNewObject->GetCorStartDate(), ai_poNewObject->GetCorEndDate());	

	//Crea el contenedor
	ClCachePolicy *poPolicy = 0;

	if (ai_dMaxPeriod)
	{
		poPolicy = new ClCachePolicy(ai_dMaxPeriod, ai_dMaxUnusedPeriod);
	}
	
	S * poNewContainer = new S (poPolicy, 0, poKey);

	M4_ASSERT(m_poCache->GetObjId());

	poNewContainer->SetPersistor(m_pPersistor);

	poNewContainer->SetEnvId(m_poCache->GetObjId()->GetEnvironmentId());

	poNewContainer->SetValue(ai_poNewObject);

	poNewContainer->SetSize( ai_poNewObject->GetSize() );

	//le cambiamos el Id.
	ai_poNewObject->SetObjId(poNewContainer->CreateObjectIdFromKey());

	ai_poNewObject->SetCO(poNewContainer);

	//Graba el contenido a disco
	if ( (m_iMode & M4CL_CACHE_WRITE ) && (ai_bSaveToDisk == M4_TRUE) )
		poNewContainer->SaveValueToDisk();

	poNewContainer->SetCache(m_poCache);

	ClCacheableObject * poOldContainer = ai_poOldObject->GetCO();
	
	if (poOldContainer)
	{
		ret = m_poCache->ReplaceObject(poOldContainer, poNewContainer);
	}
	else
	{
		ret = M4_ERROR;
	}

	if (M4_ERROR == ret)
	{
		//Error, desconectamos el container del object y destruimos el container.
		//A todos los efectos el objeto no está en la caché. espero que el UpdateObject
		//no se haya quedado con nada!!
		if (m_iMode & M4CL_CACHE_WRITE )
			poNewContainer->DeleteValueFromDisk();
		ai_poNewObject->SetCO(0);
		poNewContainer->SetValue(0);
		delete poNewContainer;
	}

	delete [] pcIdNum;

	return ret;
//## end ClBaseCache::_ReplaceObject%1802983298.body
}



template <class T, class S>
m4return_t ClBaseCache<T,S>::FreeObject (T* ao_poObject)
{
//## begin ClBaseCache::FreeObject%-2105947699.body preserve=yes
	m4return_t ret = M4_SUCCESS;
	ClCacheableObject * poCacheableObject = 0;

	poCacheableObject = ao_poObject->GetCO();

	if (poCacheableObject)
	{

		ret = m_poCache->PutObject(poCacheableObject);
	}
	else
	{
		//Por alguna razon el MCR no esta cacheado.
		delete ao_poObject;
	}

	return ret;
//## end ClBaseCache::FreeObject%-2105947699.body
}


template <class T, class S>
m4return_t ClBaseCache<T,S>::RemoveAll(m4date_t ai_dDate)
{
//## begin ClBaseCache::RemoveAll%-1857830224.body preserve=yes
	if (!ai_dDate)
	{
		return m_poCache->RemoveAll();
	}
	else
	{
		M4ClTimeStamp tsRemoveDate;

		tsRemoveDate = (m4time_t)ai_dDate;

		return m_poCache->RemoveAll(&tsRemoveDate);
	}
//## end ClBaseCache::RemoveAll%-1857830224.body
}

template <class T, class S>
m4return_t ClBaseCache<T,S>::RemoveObject (T* ao_poObject)
{
//## begin ClBaseCache::RemoveObject%194671757.body preserve=yes
	m4return_t ret = M4_SUCCESS;
	ClCacheableObject * poCacheableObject = 0;

	poCacheableObject = ao_poObject->GetCO();

	if (poCacheableObject)
	{
		ret = m_poCache->RemoveObject(poCacheableObject);
	}

	return ret;
//## end ClBaseCache::RemoveObject%194671757.body
}

template <class T, class S>
m4return_t ClBaseCache<T,S>::GetNumObjects (m4uint32_t & ao_iNumObjects)
{
//## begin ClBaseCache::GetNumObjects%-2001397908.body preserve=yes
	ao_iNumObjects = m_poCache->GetNObjInCache();

	return M4_SUCCESS;
//## end ClBaseCache::GetNumObjects%-2001397908.body
}


template <class T, class S>
m4return_t ClBaseCache<T,S>::GetNextId (m4pchar_t & ao_pcId, void * &  ai_poIterator)
{
//## begin ClBaseCache::GetNextId%-1129089689.body preserve=yes
	m4return_t ret = M4_SUCCESS;
	ClCacheableObject * poCacheableObject = (ClCacheableObject *)ai_poIterator;
	ao_pcId = 0;
	ClCacheKey * poKey = 0;

	//Si poCacheableObject (ai_poIterator) es 0 empieza por el primero.
	//Si no, utiliza ai_poIterator como iterador de su pos actual.
	ret = m_poCache->GetNext(poCacheableObject, ai_poIterator);

	if (ret == M4_ERROR)
		return M4_ERROR;

	if (!poCacheableObject)
		return M4_ERROR;

	poCacheableObject->GetKey(poKey);

	if (!poKey)
		return M4_ERROR;

	ao_pcId = poKey->Getm_pszKey();

	return M4_SUCCESS;
//## end ClBaseCache::GetNextId%-1129089689.body
}

template <class T, class S>
m4return_t ClBaseCache<T,S>::Dump(ClDumpCacheVisitor &ai_oVisitor)
{
//## begin ClBaseCache::Dump%484459575.body preserve=yes
	ai_oVisitor << "\tOwner: " << m_bCacheOwner << EndDumpCacheLn;
	ai_oVisitor << "\tMode: " << m_iMode << EndDumpCacheLn;

	m_poCache->Dump(ai_oVisitor);
/*
	m4pcchar_t m_pPath;
	m4char_t   m_acId[30];
*/
	return M4_SUCCESS;
//## end ClBaseCache::Dump%484459575.body
}

template <class T, class S>
m4return_t ClBaseCache<T,S>::SetPersistor(ClPersistor * ai_pPersistor)
{
//## begin ClBaseCache::SetPersistor%-1348044169.body preserve=yes
	m_pPersistor = ai_pPersistor;
	return M4_SUCCESS;
//## end ClBaseCache::SetPersistor%-1348044169.body
}

template <class T, class S>
m4return_t ClBaseCache<T,S>::SetCOFactory(ClCacheableObjectFactory * ai_pCOFactory)
{
//## begin ClBaseCache::SetCOFactory%-2088628645.body preserve=yes
	m_pCOFactory = ai_pCOFactory;
	return M4_SUCCESS;
//## end ClBaseCache::SetCOFactory%-2088628645.body
}

template <class T, class S>
ClCacheableObjectFactory *  ClBaseCache<T,S>::GetCOFactory(void)
{
//## begin ClBaseCache::GetCOFactory%1831275396.body preserve=yes
	return m_pCOFactory;
//## end ClBaseCache::GetCOFactory%1831275396.body
}

template <class T, class S>
void ClBaseCache<T,S>::Reload(void)
{
	if (m_iMode & M4CL_CACHE_READ )
		DePersistCache();
}

#endif
 
