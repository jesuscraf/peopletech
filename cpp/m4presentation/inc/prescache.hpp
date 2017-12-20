//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                prescache.hpp   
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


#ifndef __PRESCACHE_HPP__
#define __PRESCACHE_HPP__

#include "cachebase.hpp"
#include "datedco.hpp"
#include "clcofac.hpp"
#include "diskpers.hpp"
#include "m4cachetypes.hpp"
#include "clpresen.hpp"
#include "attributes.hpp"

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
class ClPresentation;

class ClPresentationContainer : public ClDatedContainer <ClPresentation>					   
{
private:
	//VersionT3
	m4uint32_t m_iNumT3;
	m4pchar_t * m_ppcIdT3;
	m4date_t * m_pdVersionT3;
	m4uint32_t m_iSizeIncludes;
	m4pchar_t * m_ppcIdInclude;
	m4date_t * m_pdVersionInclude;

public:
	ClPresentationContainer(ClCachePolicy *ai_poPolicy = 0, m4uint32_t  ai_iSizeObject = 0, ClCacheDatedKey *ai_poClCacheKey = 0) : ClDatedContainer <ClPresentation> (ai_poPolicy, ai_iSizeObject, ai_poClCacheKey)
	{
		m_iNumT3 = 0;
		m_ppcIdT3 = 0;
		m_pdVersionT3 = 0;	
		m_iSizeIncludes = 0;
		m_ppcIdInclude = NULL;
		m_pdVersionInclude = NULL;
	};

	~ClPresentationContainer(void)
	{
		m4uint32_t i;

		for (i = 0; i < m_iNumT3; i++)
		{
			delete [] m_ppcIdT3[i];
		}

		if (m_ppcIdT3)
			delete [] m_ppcIdT3;
	
		if (m_pdVersionT3)
			delete [] m_pdVersionT3;

		for (i = 0; i < m_iSizeIncludes; i++)
		{
			delete [] m_ppcIdInclude[i];
		}

		if (m_ppcIdInclude != NULL) {
			delete [] m_ppcIdInclude;
		}
		if (m_pdVersionInclude != NULL) {
			delete [] m_pdVersionInclude;
		}
	}

	virtual m4return_t Serialize(ClGenericIODriver& IOD);

	virtual m4return_t DeSerialize(ClGenericIODriver& IOD);

	virtual m4return_t SetValue ( ClPresentation * ai_pObject );

	virtual m4uint8_t GetType(void)
	{
		return M4_CO_FACTORY_PRESENTATION_CONTAINER;
	}

	virtual ClObjectId * CreateObjectIdFromKey(void)
	{
		m4pchar_t pcId;

		pcId = m_poClCacheKey->Getm_pszKey();
		ClTypedId oTypeID(pcId);
		ClUniqueKey oUniqueKey("");
		ClEnvironmentId oEnvId( GetEnvId() );
		ClObjectId * poObjID = new ClObjectId (PresentationObjectType, &oEnvId, &oTypeID, &oUniqueKey);
	
		return poObjID;
	}

	virtual m4bool_t MatchCondition (void * ai_pCondition);
};

typedef ClBaseCache<ClPresentation, ClPresentationContainer> ClPresentationCacheBase_t;

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

class ClPresentationCache : public ClPresentationCacheBase_t
{
public:
	ClPresentationCache(ClCache * ai_poCache, m4pcchar_t ai_pPath, m4uint8_t ai_iMode, m4uint32_t ai_iMaxMemory, m4uint32_t ai_iNumObjects, m4uint32_t ai_iRefreshRatio, m4double_t ai_dMaxPeriod, m4double_t ai_dMaxUnusedPeriod);

	~ClPresentationCache ();

	virtual m4uint16_t GetType(void)
	{
		return M4_CACHE_PRES_CACHE_TYPE;
	}

	m4return_t GetObject (const ClPresentationAttributes & ai_ClPresentationAttributes, ClPresentation* &ao_rpPresentation);

	m4return_t PutObject (const ClPresentationAttributes & ai_ClPresentationAttributes, ClPresentation* ai_pPresentation, m4date_t ai_dMaxPeriod = 0);

	virtual m4return_t RemoveObjectById (m4pchar_t ai_pcIdPresentation, m4puint32_t ai_piRemovedCount = NULL);

	virtual m4return_t RemoveObjectByIdAndVersion (m4pchar_t ai_pcIdPresentation, m4date_t ai_dVersion, ClBaseCacheInterface::eRemoveType_t ai_iRemoveType);

private:
	m4pchar_t ComposeStringKey(const ClPresentationAttributes & ai_ClPresentationAttributes);
};

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

class ClPresentationCacheableObjectFactory : public ClBaseCacheableObjectFactory, public ClDiskPersistor
{
public:
	ClPresentationCacheableObjectFactory(void)
	{
	};

	~ClPresentationCacheableObjectFactory(void)
	{
	};

private:
	virtual m4return_t CreateObject(m4uint8_t ai_iType, ClCacheableObject * &aio_pObject)
	{
		aio_pObject = new ClPresentationContainer();
		((ClPresentationContainer*)aio_pObject)->SetPersistor(this);
		return M4_SUCCESS;
	}
};


#endif

