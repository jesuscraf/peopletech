
//==============================================================================
//
// (c) Copyright  1991-2005 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                clmapcac.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                20-06-2005
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the cache for the mappings
//
//
//==============================================================================



#include "m4mdfac_dll.hpp"

#include "cltypes.hpp"
#include "clmappings.hpp"
#include "m4cachetypes.hpp"
#include "cachebase.hpp"
#include "datedco.hpp"
#include "diskpers.hpp"



#ifndef __CLMAPCAC_HPP__
#define __CLMAPCAC_HPP__




//=================================================================================
//
// ClMapContainer
//
// Clase container de objeto cacheable de mapeos
//
//=================================================================================

class	ClMapContainer : public ClDatedContainer <ClMappings>					   
{

public:

// Funciones de inicialización ================================================

	ClMapContainer( ClCachePolicy *ai_poPolicy = NULL, m4uint32_t ai_iSizeObject = 0, ClCacheDatedKey *ai_poClCacheKey = 0 ) ;


// Interfaz de ClCacheableObject ==============================================

	virtual m4uint8_t GetType( void )
	{
		return( M4_CO_FACTORY_MAP_CONTAINER ) ;
	}

	virtual m4bool_t MatchCondition( void* ai_pCondition ) ;


// Interfaz de ClDatedContainer ===============================================

	virtual ClObjectId*	CreateObjectIdFromKey( void ) ;

};




//=================================================================================
//
// ClMapCache
//
// Clase caché de objeto cacheable metacanal compilado
//
//=================================================================================

typedef	ClBaseCache<ClMappings, ClMapContainer>	ClMapCacheBase_t ;

class	M4_DECL_M4MDFAC	ClMapCache : public ClMapCacheBase_t
{

public:

// Funciones de inicialización ================================================

	ClMapCache( ClCache* ai_poCache, m4pcchar_t ai_pPath, m4uint8_t ai_iMode, m4uint32_t ai_iMaxMemory, m4uint32_t ai_iNumObjects, m4uint32_t ai_iRefreshRatio, m4double_t ai_dMaxPeriod, m4double_t ai_dMaxUnusedPeriod ) ;
	~ClMapCache( void ) {} ;


// Interfaz de ClBaseCacheInterface ===========================================

	virtual m4uint16_t GetType( void )
	{
		return( M4_CACHE_MAP_CACHE_TYPE ) ;
	}

	virtual m4return_t RemoveObjectById( m4pchar_t ai_pccMappingId, m4puint32_t ai_piRemovedCount = NULL ) ;
	virtual m4return_t RemoveObjectByIdAndVersion( m4pchar_t ai_pccMappingId, m4date_t ai_dVersion, ClBaseCacheInterface::eRemoveType_t ai_iRemoveType ) ;


// Funciones de lectura escritura =============================================

	m4return_t GetObject( m4pcchar_t ai_pccMappingId, ClMappings* &ao_rpoMappings ) ;
	m4return_t PutObject( m4pcchar_t ai_pccMappingId, ClMappings* ai_poMappings ) ;


protected:

// Otras funciones ============================================================

	m4pchar_t ComposeStringKey( m4pcchar_t ai_pccMappingId ) ;

};





//=================================================================================
//
// ClMapCacheableObjectFactory
//
// Clase de la factoria de MapCacheableObject
//
//=================================================================================

class M4_DECL_M4MDFAC ClMapCacheableObjectFactory : public ClBaseCacheableObjectFactory, public ClDiskPersistor
{

public:

// Funciones de inicialización ================================================

	ClMapCacheableObjectFactory( void ) {} ;
	~ClMapCacheableObjectFactory(void) {} ;


protected:

// Interfaz de ClBaseCacheableObjectFactory ===================================

	virtual m4return_t CreateObject( m4uint8_t ai_iType, ClCacheableObject* &ao_rpObject ) ;

};



#endif

