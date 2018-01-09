//## begin module.includes preserve=yes
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                cachefacr.cpp
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
//    This module defines the functions of the class ClCacheFactory
//
//
//==============================================================================

#include "cachefac.hpp"
#include "clcache.hpp"
#include "m4objreg.hpp"


//## end module.includes preserve=yes
ClCacheFactory::ClCacheFactory (void)
{
//## begin ClCacheFactory::ClCacheFactory%1803347439.body preserve=yes
	m_poCMCRCache = 0;
	m_poCSCRCache = 0;
	m_poPresCache = 0;
	m_poMapCache  = 0;
	m_poDataCache = 0;

	*m_acCacheDirectory = '\0';

	//caches de MD
	m_iMDDiskCache = 0;
	m_iMDCacheMaxMemory = 2000000;
	m_iMDCacheMaxNumObjects = 100;
	m_iMDCacheRefreshRatio = 5;
	m_iMDCacheExpiryRatio = 0;
	m_iMDCacheDefaultMaxPeriod = 0;

	//caches de Datos
	m_iDataDiskCache = 0;
	m_iDataCacheMaxMemory = 1000000;
	m_iDataCacheMaxNumObjects = 100;
	m_iDataCacheRefreshRatio = 5;
	m_iDataCacheExpiryRatio = 0;
	m_iDataCacheDefaultMaxPeriod = 0;
//## end ClCacheFactory::ClCacheFactory%1803347439.body
}

ClCacheFactory::~ClCacheFactory (void)
{
//## begin ClCacheFactory::~ClCacheFactory%-1872047883.body preserve=yes
	DestroyCaches();
//## end ClCacheFactory::~ClCacheFactory%-1872047883.body
}

m4return_t ClCacheFactory::Init(ClVMBaseEnv * ai_poRegistry)
{
//## begin ClCacheFactory::Init%-316074507.body preserve=yes
	return ReadCacheRegistry(ai_poRegistry);
//## end ClCacheFactory::Init%-316074507.body
}

m4return_t ClCacheFactory::CreateCaches(void)
{
//## begin ClCacheFactory::CreateCaches%1916795273.body preserve=yes
	M4ClTimeStamp Start;
	Start.now();

	if (m_poCMCRCache)
		return M4_ERROR;

	ClCacheTree *poClCacheTree;

	poClCacheTree = new ClCacheTree(0, 0, m_iMDCacheMaxMemory, m_iMDCacheMaxNumObjects,  m_iMDCacheRefreshRatio);
	m_poCMCRCache = new ClCacheTest();
	m_poCMCRCache->SetCacheStyle(poClCacheTree);

	poClCacheTree = new ClCacheTree(0, 0, m_iMDCacheMaxMemory, m_iMDCacheMaxNumObjects,  m_iMDCacheRefreshRatio);
	m_poCSCRCache = new ClCacheTest();
	m_poCSCRCache->SetCacheStyle(poClCacheTree);

	poClCacheTree = new ClCacheTree(0, 0, m_iMDCacheMaxMemory, m_iMDCacheMaxNumObjects,  m_iMDCacheRefreshRatio);
	m_poPresCache = new ClCacheTest();
	m_poPresCache->SetCacheStyle(poClCacheTree);

	poClCacheTree = new ClCacheTree(0, 0, m_iMDCacheMaxMemory, m_iMDCacheMaxNumObjects,  m_iMDCacheRefreshRatio);
	m_poMapCache = new ClCacheTest();
	m_poMapCache->SetCacheStyle(poClCacheTree);

	poClCacheTree = new ClCacheTree(0, 0, m_iDataCacheMaxMemory, m_iDataCacheMaxNumObjects,  m_iDataCacheRefreshRatio);
	m_poDataCache = new ClCacheTest();
	m_poDataCache->SetCacheStyle(poClCacheTree);

	return M4_SUCCESS;
//## end ClCacheFactory::CreateCaches%1916795273.body
}

m4return_t ClCacheFactory::DestroyCaches(void)
{
//## begin ClCacheFactory::DestroyCaches%1229165199.body preserve=yes
	if (m_poDataCache)
	{
		delete m_poDataCache;
		m_poDataCache = 0;
	}

	if (m_poCSCRCache)
	{
		delete m_poCSCRCache;
		m_poCSCRCache = 0;
	}

	if (m_poPresCache)
	{
		delete m_poPresCache;
		m_poPresCache = 0;
	}

	if (m_poMapCache)
	{
		delete m_poMapCache;
		m_poMapCache = 0;
	}

	if (m_poCMCRCache)
	{
		delete m_poCMCRCache;
		m_poCMCRCache = 0;
	}
	
	return M4_SUCCESS;
//## end ClCacheFactory::DestroyCaches%1229165199.body
}


m4return_t  ClCacheFactory::ReadCacheRegistry(ClVMBaseEnv * ai_poRegistry)
{
//## begin ClCacheFactory::ReadCacheRegistry%-981600420.body preserve=yes
	if ( (ai_poRegistry->GetCacheDirectory() != 0) && (strlen(ai_poRegistry->GetCacheDirectory()) < M4CH_MAX_WORKING_DIRECTORY) )
	{
		strcpy(m_acCacheDirectory, ai_poRegistry->GetCacheDirectory());
	}

	m_iDataDiskCache = ai_poRegistry->GetDataDiskCache();
	
	m_iDataCacheMaxMemory = ai_poRegistry->GetDataCacheMaxMemory();

	m_iDataCacheMaxNumObjects = ai_poRegistry->GetDataCacheMaxNumObjects();

	m_iDataCacheRefreshRatio = ai_poRegistry->GetDataCacheRefreshRatio();

	m_iDataCacheExpiryRatio = ai_poRegistry->GetDataCacheExpiryRatio();

	m_iDataCacheDefaultMaxPeriod = ai_poRegistry->GetDataCacheDefaultMaxPeriod();

	
	m_iMDDiskCache = ai_poRegistry->GetMDDiskCache();

	m_iMDCacheMaxMemory = ai_poRegistry->GetMDCacheMaxMemory();

	m_iMDCacheMaxNumObjects = ai_poRegistry->GetMDCacheMaxNumObjects();

	m_iMDCacheRefreshRatio = ai_poRegistry->GetMDCacheRefreshRatio();

	m_iMDCacheExpiryRatio = ai_poRegistry->GetMDCacheExpiryRatio();

	m_iMDCacheDefaultMaxPeriod = ai_poRegistry->GetMDCacheDefaultMaxPeriod();


	return (M4_SUCCESS);
//## end ClCacheFactory::ReadCacheRegistry%-981600420.body
}
