//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                cachedir.hpp   
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
//    This module defines the interface of the channeldirectory
//
//
//==============================================================================

#ifndef __CACHEDIR_HPP__
#define __CACHEDIR_HPP__

#include "m4objcachedirectory_dll.hpp"
#include "m4stl.hpp"
#include "m4define.hpp"
#include "clgeniod.hpp"
#include "cacheitfz.hpp"
#include "clprsstb.hpp"
#include "m4var.hpp"
#include "m4cachetypes.hpp"

class ClChannelManager;
class ClBaseCacheInterface;
class ClMDUpdateDeltasList;
class ClMDUpdateList;
class ClVMStatisticsWrapper;
class ClVMBaseEnv;

class M4_DECL_M4OBJCACHEDIRECTORY ClCacheDirectory : public ClPersistiableObject
{
public:
	ClCacheDirectory() : ClPersistiableObject ( 0 ){};
	virtual ~ClCacheDirectory(){};

	virtual void Attach (ClBaseCacheInterface* ai_pCache) = 0;
	virtual void Detach (ClBaseCacheInterface* ai_pCache) = 0;

	virtual m4return_t Serialize(ClGenericIODriver& IOD) = 0;
	virtual m4return_t DeSerialize(ClGenericIODriver& IOD) = 0;

    virtual m4return_t PersistDir  (void) = 0;
    virtual m4return_t DePersistDir(void) = 0;

	virtual m4return_t PersistCaches (m4uint16_t ai_iCacheType = M4_CACHE_ALL_CACHE_TYPE) = 0;
	virtual m4return_t CleanCaches (m4uint16_t ai_iCacheType = M4_CACHE_ALL_CACHE_TYPE) = 0;

	virtual m4return_t ActivateCaches() = 0;

	virtual m4return_t RemoveObjectById (m4uint16_t ai_iCacheType, m4pchar_t ai_pcId, m4bool_t ai_bLocalOnly) = 0;

	virtual m4return_t RemoveObjectByIdAndVersion (m4uint16_t ai_iCacheType, m4pchar_t ai_pcId, m4date_t ai_dVersion, ClBaseCacheInterface::eRemoveType_t ai_iRemoveType) = 0;

	virtual m4return_t GetUpdatedList(ClMDUpdateDeltasList *& ao_poUpdatedDeltasList, ClMDUpdateList * ai_poUpdateList = 0) = 0;

	virtual m4return_t Refresh (ClMDUpdateList * ai_poUpdateList = 0) = 0;

	virtual m4return_t ActualizeCachesByUpdatedList(ClMDUpdateDeltasList * ai_poUpdatedDeltasList) = 0;

	virtual m4return_t ActualizeCachesByDate(m4date_t ai_dDate) = 0;

	virtual m4date_t GetLastModificationDate() = 0;

	virtual m4VariantType& GetRepositoryKey() = 0;

	virtual m4return_t ReloadCaches() = 0;
};

#endif

