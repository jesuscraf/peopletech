//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                datacache.hpp   
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
//    This module defines the cache wrapper for data
//
//
//==============================================================================


#ifndef __DATACACHE_HPP__
#define __DATACACHE_HPP__
#include "m4dm_dll.hpp"
#include "cachebase.hpp"
#include "datedco.hpp"
#include "clcofac.hpp"
#include "diskpers.hpp"
#include "m4var.hpp"
#include "m4cachetypes.hpp"
#include "chan_knl.hpp"

class ClChannelManager;

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

typedef ClDatedContainer <ClChannel_Data> ClChannelDataContainerBase_t;


class M4_DECL_M4DM ClChannelDataContainer : public ClChannelDataContainerBase_t				   
{
private:
	//VersionT3
	m4date_t m_dT3Version;

public:
	ClChannelDataContainer(ClCachePolicy *ai_poPolicy = 0, m4uint32_t  ai_iSizeObject = 0, ClCacheDatedKey *ai_poClCacheKey = 0) : ClChannelDataContainerBase_t (ai_poPolicy, ai_iSizeObject, ai_poClCacheKey)
	{
		m_dT3Version = 0;
	};

	virtual m4return_t Serialize(ClGenericIODriver& IOD);

	virtual m4return_t DeSerialize(ClGenericIODriver& IOD);

	virtual m4return_t SetValue ( ClChannel_Data * ai_pObject );

	virtual m4uint8_t GetType(void)
	{
		return M4_CO_FACTORY_DATA_CONTAINER;
	}

	virtual ClObjectId * CreateObjectIdFromKey(void)
	{
		m4pchar_t pcId;

		pcId = m_poClCacheKey->Getm_pszKey();
		ClTypedId oTypeID(pcId);
		ClUniqueKey oUniqueKey("");
		ClEnvironmentId oEnvId( GetEnvId() );
		ClObjectId * poObjID = new ClObjectId (ChannelDataObjectType, &oEnvId, &oTypeID, &oUniqueKey);
	
		return poObjID;
	}

	virtual m4bool_t MatchCondition (void * ai_pCondition);
};


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

typedef ClBaseCache<ClChannel_Data, ClChannelDataContainer> ClDataCacheBase_t;

class M4_DECL_M4DM ClDataCache : public ClDataCacheBase_t
{
public:
	ClDataCache(ClCache * ai_poCache, m4pcchar_t ai_pPath, m4uint8_t ai_iMode, m4uint32_t ai_iMaxMemory, m4uint32_t ai_iNumObjects, m4uint32_t ai_iRefreshRatio, m4double_t ai_dMaxPeriod, m4double_t ai_dMaxUnusedPeriod);

	~ClDataCache ();

	virtual m4uint16_t GetType(void)
	{
		return M4_CACHE_DATA_CACHE_TYPE;
	}

	m4return_t GetObject (m4uint8_t ai_iCsType, m4pcchar_t ai_pccChannelId, m4pcchar_t ai_pccRSM, m4uint8_t ai_iLanguage, m4uint8_t ai_iOrgType, m4pchar_t ai_pcOrg, m4VariantType * ai_pVarParamsId, m4uint32_t ai_iNumParams, m4VariantType &ai_roStartDate, m4VariantType &ai_roEndDate, m4VariantType &ai_roCorDate, ClChannel_Data* &ao_rpChannelData, m4bool_t &ao_bFound);

	m4return_t PutObject (m4uint8_t ai_iCsType, m4pcchar_t ai_pccChannelId, m4pcchar_t ai_pccRSM, m4uint8_t ai_iLanguage, m4uint8_t ai_iOrgType, m4pchar_t ai_pcOrg, m4VariantType * ai_pVarParamsId, m4uint32_t ai_iNumParams, ClChannel_Data* ai_pChannelData, m4date_t ai_dMaxPeriod = 0);

	virtual m4return_t RemoveObjectById (m4pchar_t ai_pccChannelId, m4puint32_t ai_piRemovedCount = NULL);

	virtual m4return_t RemoveObjectByIdAndVersion (m4pchar_t ai_pcChannelId, m4date_t ai_dVersion, ClBaseCacheInterface::eRemoveType_t ai_iRemoveType);

	m4pchar_t ComposeStringKey(m4uint8_t ai_iCsType, m4pcchar_t ai_pccChannelId, m4pcchar_t ai_pccRSM, m4uint8_t ai_iLanguage, m4uint8_t ai_iOrgType, m4pchar_t ai_pcOrg, m4VariantType * ai_pVarParamsId, m4uint32_t ai_iNumParams);
};

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

class M4_DECL_M4DM ClDataCacheableObjectFactory : public ClBaseCacheableObjectFactory, public ClDiskPersistor
{
public:
	ClDataCacheableObjectFactory(void);

	~ClDataCacheableObjectFactory(void);

	virtual m4return_t Persist  (ClPersistiableObject & ai_oPersistiableObj);

	virtual m4return_t DePersist(ClPersistiableObject & ai_oPersistiableObj);

	void SetChannelManager(ClChannelManager * ai_poChannelManager)
	{
		m_poChannelManager = ai_poChannelManager;
	}

	ClChannelManager * GetChannelManager(void)
	{
		return m_poChannelManager;
	}

	ClMutex			m_oMutex;
private:
	virtual m4return_t CreateObject(m4uint8_t ai_iType, ClCacheableObject * &aio_pObject)
	{
		aio_pObject = new ClChannelDataContainer();
		((ClChannelDataContainer*)aio_pObject)->SetPersistor(this);
		return M4_SUCCESS;
	}

	//Por ahora la fatoria solo lo usa para los Channel_Data
	ClChannelManager * m_poChannelManager;
};

#endif

