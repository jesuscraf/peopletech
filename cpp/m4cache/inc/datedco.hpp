//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                dated.hpp
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
//    This module defines the class of the factory of compiled metachannels
//
//
//==============================================================================


#ifndef __DATEDCO_HPP__
#define __DATEDCO_HPP__
//## begin module.includes preserve=yes

#include "m4types.hpp"
#include "clcache.hpp"
#include "dumpcache.hpp"
#include "clgeniod.hpp"
#include "clprsstb.hpp"
#include "m4cacheres.hpp"
#include "chlog.hpp"

#define M4_CO_COND_CONTAINED_IN_KEY							0x1
#define M4_CO_COND_OLDER_VERSION							0x2
#define M4_CO_COND_OLDER_PARENT_VERSION						0x3
#define M4_CO_COND_OLDER_DEPENDENCY_VERSION					0x4

//## end module.includes preserve=yes
class M4_DECL_M4CACHE ClCOCondition
{
  public:
    ClCOCondition ()
	{
//## begin ClCOCondition::ClCOCondition%-1623154902.body preserve=yes
		m_pNext = 0;
		m_iAnd = 1;
		m_iConditionMask = 0;
		m_pValue = 0;
	
//## end ClCOCondition::ClCOCondition%-1623154902.body
	}

    ~ClCOCondition ()
	{
//## begin ClCOCondition::~ClCOCondition%-605533701.body preserve=yes
	
//## end ClCOCondition::~ClCOCondition%-605533701.body
	}

	m4uint16_t		m_iConditionMask;
	void *			m_pValue;
	m4uint8_t		m_iAnd;
	ClCOCondition * m_pNext;
};

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
class M4_DECL_M4CACHE ClCacheDatedKey : public ClCacheKey
{

  public:
	  //Deberia ser virtual en la clbase
      ClCacheDatedKey (m4pchar_t ai_pszKey = 0);

      ClCacheDatedKey (m4pchar_t ai_pszKey,
					   m4date_t ai_StartWorkDate, m4date_t ai_EndWorkDate,
					   m4date_t ai_StartCorrDate, m4date_t ai_EndCorrDate);

      ~ClCacheDatedKey ();

	  virtual m4return_t Serialize(ClGenericIODriver& IOD);

	  virtual m4return_t DeSerialize(ClGenericIODriver& IOD);

	  void SetDates ( m4date_t ai_StartWorkDate, m4date_t ai_EndWorkDate,
					  m4date_t ai_StartCorrDate, m4date_t ai_EndCorrDate);

	  m4bool_t Contains (ClCacheDatedKey &ai_oDatedKey);

	  virtual m4return_t Dump(ClDumpCacheVisitor &ai_oVisitor);

  private:
	m4date_t m_StartWorkDate;
	m4date_t m_EndWorkDate;
	m4date_t m_StartCorrDate;
	m4date_t m_EndCorrDate;
};

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
template <class T>
class ClDatedContainer : public ClCacheableObject
{
protected:
	ClCache * m_poCache;
	
	ClPersistor * m_pPersistor;

	T * m_pObject;

	m4date_t m_dVersion;

public:
	ClDatedContainer(ClCachePolicy *ai_poPolicy = 0, m4uint32_t  ai_iSizeObject = 0, ClCacheDatedKey *ai_poClCacheKey = 0);

	virtual ~ClDatedContainer ();

	virtual m4return_t Serialize(ClGenericIODriver& IOD);

	virtual m4return_t DeSerialize(ClGenericIODriver& IOD);

	virtual m4return_t SetValue ( T * ai_pObject );

	T * GetValue ( void );

	virtual m4return_t LoadValueFromDisk ( void );

	virtual m4return_t SaveValueToDisk ( void );

	virtual m4return_t DeleteValueFromDisk ( void );

    virtual m4return_t DeleteValueFromRam ( void );

	virtual m4uint8_t GetPersistianceLevel ( void );

	virtual ClObjectId * CreateObjectIdFromKey(void) = 0;

	//Interacción con la cache
	void SetCache ( ClCache * ai_poCache );

	ClCache * GetCache ( void );
	
	m4return_t FreeObjectFromCache ( void );

	m4return_t RemoveObjectFromCache ( void );

	virtual m4return_t NotifyCacheDestroy ( void );

	virtual m4return_t Dump(ClDumpCacheVisitor &ai_oVisitor);

	m4return_t SetPersistor(ClPersistor * ai_pPersistor);
	//virtual m4bool_t MatchCondition (void * ai_pCondition);

private:
	m4pchar_t GetErrorStringKey( void );

};

template <class T>
ClDatedContainer<T>::ClDatedContainer(ClCachePolicy *ai_poPolicy, m4uint32_t  ai_iSizeObject, ClCacheDatedKey *ai_poClCacheKey) 
//## begin ClDatedContainer::ClDatedContainer%1552279139.initialization preserve=yes
: ClCacheableObject(ai_poPolicy, ai_iSizeObject, ai_poClCacheKey)
//## end ClDatedContainer::ClDatedContainer%1552279139.initialization
{

//## begin ClDatedContainer::ClDatedContainer%1552279139.body preserve=yes
	m_pObject = 0;
	m_poCache = 0;
	m_dVersion = 0;
	m_pPersistor = 0;
//## end ClDatedContainer::ClDatedContainer%1552279139.body
}


template <class T>
ClDatedContainer<T>::~ClDatedContainer ()
{
//## begin ClDatedContainer::~ClDatedContainer%129706103.body preserve=yes
	//??
	if (m_poClCacheKey)
		delete m_poClCacheKey;	

	if (m_pObject)
		delete m_pObject;

	m_pObject = 0;
	m_poCache = 0;
	m_pPersistor = 0;
//## end ClDatedContainer::~ClDatedContainer%129706103.body
}

template <class T>
m4return_t ClDatedContainer<T>::Serialize(ClGenericIODriver& IOD)
{
//## begin ClDatedContainer::Serialize%-117390661.body preserve=yes
	m4return_t ret;
	m4bool_t bExists = M4_FALSE;
	
	//Serializo la Clase Base
	ret = ClCacheableObject::Serialize(IOD);

	//Key
	ret = IOD.Write(m_poClCacheKey, &bExists);

	if (ret == M4_ERROR)
		return M4_ERROR;

	if (!bExists)
		ret = m_poClCacheKey->Serialize(IOD);


	//m_pEnvId
	ret = IOD.Write(m_pEnvId, &bExists);

	if (ret == M4_ERROR)
		return M4_ERROR;

	if (!bExists)
		ret = m_pEnvId->Serialize(IOD);

	ret = IOD.Write(m_dVersion);

	if (ret == M4_ERROR)
		return M4_ERROR;

	if (m_bDisk == M4_FALSE)
	{	
		// Escribimos el Objeto a disco
		ret = SaveValueToDisk();
	}

	return ret;
//## end ClDatedContainer::Serialize%-117390661.body
}

template <class T>
m4return_t ClDatedContainer<T>::DeSerialize(ClGenericIODriver& IOD)
{
//## begin ClDatedContainer::DeSerialize%463842093.body preserve=yes
	m4return_t ret;
	m4bool_t bExists = M4_FALSE;
	
	//Serializo la Clase Base
	ret = ClCacheableObject::DeSerialize(IOD);

	//Key
	ret = IOD.Read((void**)&m_poClCacheKey, &bExists);

	if (ret == M4_ERROR)
		return M4_ERROR;

	if (!bExists)
	{
		m_poClCacheKey = new ClCacheDatedKey();

		ret = IOD.StoreAddress(m_poClCacheKey);

		if (ret == M4_ERROR)
			return M4_ERROR;

		ret = m_poClCacheKey->DeSerialize(IOD);
		
		if (ret == M4_ERROR)
			return M4_ERROR;

	}

	//m_pEnvId
	ret = IOD.Read((void**)&m_pEnvId, &bExists);

	if (ret == M4_ERROR)
		return M4_ERROR;

	if (!bExists)
	{
		m_pEnvId = new ClEnvironmentId();

		ret = IOD.StoreAddress(m_pEnvId);

		if (ret == M4_ERROR)
			return M4_ERROR;

		ret = m_pEnvId->DeSerialize(IOD);
		
		if (ret == M4_ERROR)
			return M4_ERROR;
	}

	ret = IOD.Read(m_dVersion);

	if (ret == M4_ERROR)
		return M4_ERROR;

	m_bDisk = M4_TRUE;

	m_bRam = M4_FALSE;

	return ret;
//## end ClDatedContainer::DeSerialize%463842093.body
}


template <class T>
m4return_t ClDatedContainer<T>::SetValue ( T * ai_pObject )
{
//## begin ClDatedContainer::SetValue%1391979137.body preserve=yes
	m_pObject = ai_pObject;

	m_bRam = M4_TRUE;

	if (ai_pObject)
	{
		m_dVersion = ai_pObject->GetVersion();
	}

	return M4_SUCCESS;
//## end ClDatedContainer::SetValue%1391979137.body
}

template <class T>
T * ClDatedContainer<T>::GetValue ( void )
{
//## begin ClDatedContainer::GetValue%-1842182442.body preserve=yes
	return m_pObject;
//## end ClDatedContainer::GetValue%-1842182442.body
}

template <class T>
m4return_t ClDatedContainer<T>::SaveValueToDisk ( void )
{
//## begin ClDatedContainer::SaveValueToDisk%-525735631.body preserve=yes
	m4return_t ret;

	if (!m_pObject)
	{
		g_oChLog<<BeginWarning(M4_CACHE_SAVEVALUETODISK_EMPTY)<<GetErrorStringKey()<<EndLog;
		return M4_ERROR;
	}

	if (m_pPersistor)
	{
		ret = m_pPersistor->Persist(*m_pObject);

		if (ret == M4_ERROR)
		{
			//Si se ha creado el fichero lo borramos.
			m_pPersistor->Delete(*m_pObject);
			g_oChLog<<BeginWarning(M4_CACHE_SAVEVALUETODISK_PERSIST)<<GetErrorStringKey()<<EndLog;
			return M4_ERROR;
		}
		
		m_bDisk = M4_TRUE;
	}
	else
	{
		//Error
		ret = M4_ERROR;
	}

	return ret;
//## end ClDatedContainer::SaveValueToDisk%-525735631.body
}


template <class T>
m4return_t ClDatedContainer<T>::LoadValueFromDisk ( void )
{
//## begin ClDatedContainer::LoadValueFromDisk%28383483.body preserve=yes
	m4return_t ret;

	if (m_pObject)
	{
		g_oChLog<<BeginWarning(M4_CACHE_LOADVALUEFROMDISK_NOTEMPTY)<<GetErrorStringKey()<<EndLog;
		return M4_ERROR;
	}

	m_pObject = new T();

	m_pObject->SetObjId(CreateObjectIdFromKey());

	if (m_pPersistor)
	{
		ret = m_pPersistor->DePersist(*m_pObject);

		if  (ret == M4_ERROR)
		{
			//Si existe el fichero lo borramos.
			m_pPersistor->Delete(*m_pObject);
			delete m_pObject;
			m_pObject = 0;
			g_oChLog<<BeginWarning(M4_CACHE_LOADVALUEFROMDISK_DEPERSIST)<<GetErrorStringKey()<<EndLog;
			return M4_ERROR;
		}

		m_pObject->SetCO(this);

		SetValue(m_pObject);

		m_bRam = M4_TRUE;
	}
	else
	{
		//Error
		ret = M4_ERROR;
	}

	return M4_SUCCESS;
//## end ClDatedContainer::LoadValueFromDisk%28383483.body
}

template <class T>
m4return_t ClDatedContainer<T>::DeleteValueFromDisk ( void )
{
//## begin ClDatedContainer::DeleteValueFromDisk%445981600.body preserve=yes
	m4return_t ret;

	if (m_pPersistor)
	{
		if (!m_pObject)
		{
			//Creamos un objeto temporal para poder usar el persistor
			T * pObject = new T();

			pObject->SetObjId(CreateObjectIdFromKey());

			ret = m_pPersistor->Delete(*pObject);

			delete pObject;
		}
		else
		{
			ret = m_pPersistor->Delete(*m_pObject);
		}
		
		if (ret == M4_ERROR)
		{
			g_oChLog<<BeginWarning(M4_CACHE_DELETEVALUEFROMDISK)<<GetErrorStringKey()<<EndLog;
		}

		m_bDisk = M4_FALSE;
	}
	else
	{
		ret = M4_ERROR;
	}

	return M4_SUCCESS;
//## end ClDatedContainer::DeleteValueFromDisk%445981600.body
}

template <class T>
m4return_t ClDatedContainer<T>::DeleteValueFromRam ( void )
{
//## begin ClDatedContainer::DeleteValueFromRam%697354252.body preserve=yes
	if (!m_pObject)
	{
		g_oChLog<<BeginWarning(M4_CACHE_DELETEVALUEFROMRAM)<<GetErrorStringKey()<<EndLog;
	}
	else
	{
		delete m_pObject;
	
		m_pObject = 0;
	}

	m_bRam = M4_FALSE;

	return M4_SUCCESS;
//## end ClDatedContainer::DeleteValueFromRam%697354252.body
}


template <class T>
m4uint8_t ClDatedContainer<T>::GetPersistianceLevel ( void )
{
//## begin ClDatedContainer::GetPersistianceLevel%-1968448437.body preserve=yes
	return 2;
//## end ClDatedContainer::GetPersistianceLevel%-1968448437.body
}


template <class T>
void ClDatedContainer<T>::SetCache ( ClCache * ai_poCache )
{
//## begin ClDatedContainer::SetCache%-560696806.body preserve=yes
	m_poCache = ai_poCache;
//## end ClDatedContainer::SetCache%-560696806.body
}

template <class T>
ClCache *  ClDatedContainer<T>::GetCache ( void )
{
//## begin ClDatedContainer::GetCache%1735029312.body preserve=yes
	return m_poCache;
//## end ClDatedContainer::GetCache%1735029312.body
}
	
template <class T>
m4return_t ClDatedContainer<T>::FreeObjectFromCache(void)
{
//## begin ClDatedContainer::FreeObjectFromCache%-1368661475.body preserve=yes
	if (m_poCache)
	{
		return m_poCache->PutObject(this);
	}
	else
	{
		g_oChLog<<BeginWarning(M4_CACHE_VALUE_ACCESS_TO_CACHE)<<GetErrorStringKey()<<EndLog;
		return M4_ERROR;
	}
//## end ClDatedContainer::FreeObjectFromCache%-1368661475.body
}

template <class T>
m4return_t ClDatedContainer<T>::RemoveObjectFromCache(void)
{
//## begin ClDatedContainer::RemoveObjectFromCache%-423430046.body preserve=yes
	if (m_poCache)
	{
		return m_poCache->RemoveObject(this);
	}
	else
	{
		g_oChLog<<BeginWarning(M4_CACHE_VALUE_ACCESS_TO_CACHE)<<GetErrorStringKey()<<EndLog;
		return M4_ERROR;
	}
//## end ClDatedContainer::RemoveObjectFromCache%-423430046.body
}

template <class T>
m4return_t ClDatedContainer<T>::NotifyCacheDestroy ()
{
//## begin ClDatedContainer::NotifyCacheDestroy%75757299.body preserve=yes
	//Pone a Null el miembro m_poCache pq la caché ya no existe.
	m_poCache = 0;
	return M4_TRUE;
//## end ClDatedContainer::NotifyCacheDestroy%75757299.body
}

template <class T>
m4return_t ClDatedContainer<T>::Dump(ClDumpCacheVisitor &ai_oVisitor)
{
//## begin ClDatedContainer::Dump%363930122.body preserve=yes
	m4pchar_t pcEnv = 0;

	if (m_pEnvId)
	{
		pcEnv = m_pEnvId->GetId();

		if (!pcEnv)
			pcEnv = "NULL";
	}
	else
	{
		pcEnv = "NULL";
	}

	ClCacheableObject::Dump(ai_oVisitor);

	ai_oVisitor << "\t\tDated CO Type:\t " << GetType() << EndDumpCacheLn;

	if (M4_TRUE == ai_oVisitor.DumpDirs())
	{
		ai_oVisitor << "\t\tDated CO EnvId:\t " << pcEnv << EndDumpCacheLn;
	}

	return M4_SUCCESS;
//## end ClDatedContainer::Dump%363930122.body
}

template <class T>
m4pchar_t ClDatedContainer<T>::GetErrorStringKey( void )
{
//## begin ClDatedContainer::GetErrorStringKey%1174887441.body preserve=yes
	m4pchar_t pKey = 0;

	if (m_poClCacheKey)
	{
		pKey = m_poClCacheKey->Getm_pszKey();
	}

	if (!pKey)
	{
		pKey = "NULL";
	}

	return pKey;
//## end ClDatedContainer::GetErrorStringKey%1174887441.body
}

template <class T>
m4return_t ClDatedContainer<T>::SetPersistor(ClPersistor * ai_pPersistor)
{
//## begin ClDatedContainer::SetPersistor%1622311098.body preserve=yes
	m_pPersistor = ai_pPersistor;
	return M4_SUCCESS;
//## end ClDatedContainer::SetPersistor%1622311098.body
}
#endif 
