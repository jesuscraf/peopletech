//	==============================================================================
//
//	 (c) Copyright  1991-1999 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            Executive
//	 File:              ClExecutorStateManager.hpp
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

#ifndef _CLEXECUTORSTATEMANAGER_HPP_
#define _CLEXECUTORSTATEMANAGER_HPP_

#include "m4exeforchannel_dll.hpp"
#include "m4types.hpp"
//#include "datastorages.hpp"
//#include "clcache.hpp"
#include "cachebase.hpp"
#include "clexecutorstate.hpp"
#include "m4string.hpp"

class ClChannelManager;
class ClChannel;
class ClM4XMLContext;
class ClCMInitParameters;


// Clase que almacena la informacion de Configuracion de la Cache
class M4_DECL_M4EXEFORCHANNEL ClCacheConfigInfo
{
public:
	// Cache directory path
	string m_sCacheDirectory;

	// Cache Mode:	M4CL_CACHE_NONE=0, M4CL_CACHE_READ=1, M4CL_CACHE_WRITE=2, M4CL_CACHE_READ_WRITE=3
	//m4uint8_t m_iMode;

	// Max Memory used by Cache
	m4uint32_t m_iMaxMemory;

	// Max Number of Object stored in Cache
	m4uint32_t m_iMaxNumObjects;

	// Refresh ratio for Cache to persist
	m4uint32_t m_iRefreshRatio;

	// Cache subsystems
	ClCache	 *m_poCMCRCache;
	ClCache	 *m_poCSCRCache;
	ClCache	 *m_poPresentationCache;
	ClCache	 *m_poMapCache;
	ClCache	 *m_poDataCache;


private:
	void SetDefaultPath() {
#ifdef _UNIX
		m_sCacheDirectory.assign("./temp");
#else
		m_sCacheDirectory.assign("c:\\temp");
#endif
	}

public:
	// Default Constructor
	ClCacheConfigInfo();

	// Constructor with Params
	ClCacheConfigInfo(m4pcchar_t ai_pPath, m4uint32_t ai_iMaxMemory, m4uint32_t ai_iNumObjects, m4uint32_t ai_iRefreshRatio);

	// Set Cache subsystems
	void SetCaches( ClCache *ai_poCMCRCache, ClCache *ai_poCSCRCache, ClCache *ai_poPresentationCache, ClCache *ai_poMapCache, ClCache *ai_poDataCache );
};

// counter map for session references
typedef map< string,m4uint32_t,less<string> > SMSessionCounterMap;
typedef SMSessionCounterMap::iterator SMSessionCounterIt;


// Class to represent the Manager for the ExecutorState Cache
// Is a Cacheable Object
class M4_DECL_M4EXEFORCHANNEL ClExecutorStateManager
{
private:
	// ExecutorState Cache
	ClExecutorStateCache *	m_poExecutorStateCache;

	// Mutex para secciones criticas
	ClMutex					m_oMutex;

	// Syncs internal objects
	ClMutex					m_oSessionMutex;

	// // ExecutorState Cache (basada en un mapa simple)
	//ClExecutorStateCacheMap *	m_poExecutorStateCache;

	// Cache base
	ClCache *				m_poCache;

	// Counters for sessions
	SMSessionCounterMap     m_oSessionCounters;

public:
	// Constructor
	ClExecutorStateManager();
	
	// Destructor
	~ClExecutorStateManager();

	//m4return_t InitCache( ClCache * ai_pCache = NULL, m4bool_t ai_bCreateDefaultCache = M4_FALSE);
	m4return_t InitCache(const ClCacheConfigInfo& ai_oConfig);	// Initialize the Cache

	// Returns if Cache is initialized
	m4bool_t IsInitialized() const { return (m_poExecutorStateCache != NULL) ? M4_TRUE : M4_FALSE; }
	
	// Returns the Cache
	ClExecutorStateCache * GetCache(void) const { return m_poExecutorStateCache; }
	//ClExecutorStateCacheMap * GetCache(void) const { return m_poExecutorStateCache; }

	// Persist State object into Cache
	m4return_t	PersistState(ClCMInitParameters &aio_oCMInitParameters);

	// Recover State object from Cache
	m4return_t	RecoverState(ClCMInitParameters &aio_oCMInitParameters, m4bool_t& ao_bFound, m4bool_t ai_bIsNew=M4_FALSE);

	// Remove State object from Cache
	m4return_t	RemoveState(m4pchar_t ai_pccSessionId, m4puint32_t ao_piRemovedCount=NULL);

	// Check State object from Cache
	m4return_t	CheckState(m4pchar_t ai_pccSessionId, m4bool_t& ao_bFound);

	// Load Cache from disk
	m4return_t	LoadCache();

	// Save Cache to disk
	m4return_t	PersistCache();

	// Clean Cache from Memory and Disk
	m4return_t	CleanCache();

	// Updates cache deleting old elements
	m4return_t UpdateCache(ClCMInitParameters &aio_oCMInitParameters);

	// Deletes an entry in the counter Map
	void RemoveFromSessionMap(m4pcchar_t ai_pccSessionId);

	// Updates an entry in the counter Map
	m4uint32_t UpdateSessionMap(m4pcchar_t ai_pccSessionId);

public:

	// Detach Session Channel from Channel Manager
	m4return_t	DetachSessionChannel(ClChannelManager *ai_pChannelManager);

	// Attach Session Channel to Channel Manager
	m4return_t	AttachSessionChannel(ClChannelManager *ai_pChannelManager, ClChannel *ai_poSessionChannel);

	// Dump Cache
	void DumpCache(m4pchar_t ai_pcCalledFrom, ostream *ai_pStream=NULL) const;
};


#endif	//_CLEXECUTORSTATEMANAGER_HPP_
