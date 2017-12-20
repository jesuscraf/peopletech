//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                cachedircli.hpp   
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
//    This module defines the class of the channeldirectory server
//
//
//==============================================================================

#ifndef __CACHEDIRSRV_HPP__
#define __CACHEDIRSRV_HPP__

#include "m4objcachedirectory_dll.hpp"
#include "cachedirbase.hpp"

class M4_DECL_M4OBJCACHEDIRECTORY ClCacheDirectoryServer : public ClCacheDirectoryBase
{
public:
	ClCacheDirectoryServer(void);
	~ClCacheDirectoryServer(void);

	m4return_t Init (ClVMBaseEnv * ai_poRegistry, ClVMStatisticsWrapper * ai_poSttWrapper, ClChannelManager * ai_poChannelManager);
	m4return_t End ();
	
	virtual m4return_t RemoveObjectById (m4uint16_t ai_iCacheType, m4pchar_t ai_pcId, m4bool_t ai_bLocalOnly);
	virtual m4return_t GetUpdatedList(ClMDUpdateDeltasList *& ao_poUpdatedDeltasList, ClMDUpdateList * ai_poUpdateList = 0);

private:
	
	ClChannelManager * m_poChannelManager;
};

#endif

