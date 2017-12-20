//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                cachedirbase.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                16-06-1997
// Language:               C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the class of the channeldirectorybase
//
//
//==============================================================================

#ifndef __CACHEDIRBASE_HPP__
#define __CACHEDIRBASE_HPP__

#include "m4objcachedirectory_dll.hpp"
#include "m4stl.hpp"
#include "cachedir.hpp"
#include "m4define.hpp"
#include "clgeniod.hpp"
#include "cacheitfz.hpp"
#include "clprsstb.hpp"
#include "m4var.hpp"

class M4_DECL_M4OBJCACHEDIRECTORY ClCacheDirectoryBase : public ClCacheDirectory
{
public:
	ClCacheDirectoryBase(void);
	~ClCacheDirectoryBase(void);

	virtual void Attach (ClBaseCacheInterface* ai_pCache);
	virtual void Detach (ClBaseCacheInterface* ai_pCache);

	virtual m4return_t Serialize(ClGenericIODriver& IOD);
	virtual m4return_t DeSerialize(ClGenericIODriver& IOD);

    virtual m4return_t PersistDir  (void);
    virtual m4return_t DePersistDir(void);

	virtual m4return_t PersistCaches (m4uint16_t ai_iCacheType = M4_CACHE_ALL_CACHE_TYPE);
	virtual m4return_t CleanCaches (m4uint16_t ai_iCacheType = M4_CACHE_ALL_CACHE_TYPE);

	virtual m4return_t ActivateCaches();

	virtual m4return_t RemoveObjectById (m4uint16_t ai_iCacheType, m4pchar_t ai_pcId, m4bool_t ai_bLocalOnly) = 0;

	virtual m4return_t RemoveObjectByIdAndVersion (m4uint16_t ai_iCacheType, m4pchar_t ai_pcId, m4date_t ai_dVersion, ClBaseCacheInterface::eRemoveType_t ai_iRemoveType);

	virtual m4return_t GetUpdatedList(ClMDUpdateDeltasList *& ao_poUpdatedDeltasList, ClMDUpdateList * ai_poUpdateList = 0) = 0;

	virtual m4return_t Refresh (ClMDUpdateList * ai_poUpdateList = 0);

	virtual m4return_t ActualizeCachesByUpdatedList(ClMDUpdateDeltasList * ai_poUpdatedDeltasList);

	virtual m4return_t ActualizeCachesByDate(m4date_t ai_dDate);

	virtual m4date_t GetLastModificationDate()
	{
		return m_dLastModificationDate;
	}

	virtual m4VariantType& GetRepositoryKey()
	{
		return m_vRepositoryKey;
	}

	virtual m4return_t ReloadCaches();

protected:
	typedef list<ClBaseCacheInterface*> CACHELIST;
	CACHELIST		m_oCacheList;

	m4date_t		m_dLastModificationDate;
	m4VariantType	m_vRepositoryKey;
	m4bool_t		m_bInit;

	ClVMStatisticsWrapper			*m_poSttWrapper;

	m4return_t _Init (ClVMBaseEnv * ai_poRegistry);
	m4return_t _End ();
	m4return_t _RemoveObjectById (m4uint16_t ai_iCacheType, m4pchar_t ai_pcId, m4bool_t ai_bLocalOnly);
};

#endif

