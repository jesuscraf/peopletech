//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             cacherep.hpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             28/01/14
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//    Declaration of the CacheReplacer
//
//==============================================================================

#include "dm.hpp"


// -----------------------------------------------------------------------------
// Class ClCachePack
//
// Cache package container.
// -----------------------------------------------------------------------------

class ClCachePack
{
	public:

		ClCachePack();
		~ClCachePack();

		// Resets the cache package.
		void Reset();

		// Getter members.
		m4language_t	GetLanguage();
		int				GetUnicode();
		m4pchar_t		GetOrganization();
		m4pchar_t		GetChangeDate();
		m4pchar_t		GetPack();
		m4pchar_t		GetTechVer();

		// Setter members.
		void SetLanguage	( m4language_t ai_iLanguage );
		void SetUnicode		( int ai_iUnicode );
		void SetOrganization( m4pcchar_t ai_pcOrganization );
		void SetChangeDate	( m4pcchar_t ai_pcChangeDate );
		void SetPack		( m4pcchar_t ai_pcPack );
		void SetTechVer		( m4pcchar_t ai_pcTechVer );

	private:

		m4language_t	m_iLanguage;
		int				m_bUnicode;
		m4pchar_t		m_pcOrganization;
		m4pchar_t		m_pcChangeDate;
		m4pchar_t		m_pcPack;
		m4pchar_t		m_pcTechVer;

		void			dispose();
		void			copyString( m4pchar_t& ai_pcTarget, m4pcchar_t ai_pcSource );
};


// -----------------------------------------------------------------------------
// Class ClCacheReplacer
//
// Application context cache replacer.
// -----------------------------------------------------------------------------

class ClCacheReplacer
{
	public:

		ClCacheReplacer( ClChannelManager* ai_poChManager );
		~ClCacheReplacer();

		// Replace the local cache with the more recent published cache package.
		m4return_t	ReplaceCache();

	private:

		ClChannelManager*	m_poChManager;
		ClChannel*			m_poChannel;
		ClAccess*			m_poAccess;
		ClNode*				m_poNode;

		m4language_t		m_iEnvLang;
		int					m_bEnvUnicode;
		m4pchar_t			m_pcEnvTechVer;
		m4pchar_t			m_pcEnvCachePath;

		m4return_t	Initialize( ClChannelManager* ai_poChManager );
		m4return_t	getLocalPack( ClCachePack& ao_oLocalPack );
		m4return_t	buildChannel();
		m4return_t	checkForNewPack( ClCachePack& ai_oLocalPack, ClCachePack& ao_oNewPack, m4bool_t& ao_bAvailable );
		m4return_t	clearCache();
		m4return_t	installCachePack( ClCachePack& ai_oNewPack );
		m4return_t	setLocalPack( ClCachePack& ai_oLocalPack );
		m4return_t	removeDirTree( m4pcchar_t ai_pcPath, m4bool_t ai_bSkip );
};


// -----------------------------------------------------------------------------
// Class ClFinalizer
//
// Helper for objects finalization.
// -----------------------------------------------------------------------------

class ClFinalizer
{
	public:

		ClFinalizer();
		~ClFinalizer();

		// Setter members.
		void setPackage( ClCachePack* ai_pPack );

	private:

		ClCachePack*	m_pPack;
};
