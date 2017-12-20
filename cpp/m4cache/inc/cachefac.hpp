//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// File:	cachefac.hpp
// Author:	
// Date:	
// Definition:
//    This module defines the object ClCacheFactory
//

#ifndef __CACHEFAC_HPP__
#define __CACHEFAC_HPP__
//## begin module.includes preserve=yes

#include "m4cache_dll.hpp"
#include "m4define.hpp"
#include "m4regdef.hpp"
//## end module.includes preserve=yes
//## begin module.additionalDeclarations preserve=yes

class ClCache;
class ClVMBaseEnv;

//## end module.additionalDeclarations
class M4_DECL_M4CACHE ClCacheFactory
{
public:
	ClCacheFactory(void);
	~ClCacheFactory(void);
	m4return_t Init(ClVMBaseEnv * ai_poRegistry);
	m4return_t CreateCaches(void);
	m4return_t DestroyCaches(void);

	//Get de las caches
	ClCache * GetCMCRCache(void)
	{
//## begin ClCacheFactory::GetCMCRCache%-585500887.body preserve=yes
		return m_poCMCRCache;
	
//## end ClCacheFactory::GetCMCRCache%-585500887.body
	}

	ClCache * GetCSCRCache(void)
	{
//## begin ClCacheFactory::GetCSCRCache%1640783963.body preserve=yes
		return m_poCSCRCache;
	
//## end ClCacheFactory::GetCSCRCache%1640783963.body
	}

	ClCache * GetPresCache(void)
	{
//## begin ClCacheFactory::GetPresCache%636245841.body preserve=yes
		return m_poPresCache;
	
//## end ClCacheFactory::GetPresCache%636245841.body
	}

	ClCache * GetMapCache(void)
	{
		return m_poMapCache;
	}

	ClCache * GetDataCache(void)
	{
//## begin ClCacheFactory::GetDataCache%-2116989404.body preserve=yes
		return m_poDataCache;
	
//## end ClCacheFactory::GetDataCache%-2116989404.body
	}


	// Caches
	m4uint32_t GetMDCacheMaxMemory (void) const
	{
//## begin ClCacheFactory::GetMDCacheMaxMemory%945170584.body preserve=yes
        return (m_iMDCacheMaxMemory) ;
	
//## end ClCacheFactory::GetMDCacheMaxMemory%945170584.body
	}

	void SetMDCacheMaxMemory (m4uint32_t ai_iMDCacheMaxMemory)
	{
//## begin ClCacheFactory::SetMDCacheMaxMemory%159163114.body preserve=yes
        m_iMDCacheMaxMemory = ai_iMDCacheMaxMemory;
	
//## end ClCacheFactory::SetMDCacheMaxMemory%159163114.body
	}

	m4uint32_t GetMDCacheMaxNumObjects (void) const
	{
//## begin ClCacheFactory::GetMDCacheMaxNumObjects%-631368701.body preserve=yes
        return (m_iMDCacheMaxNumObjects) ;
	
//## end ClCacheFactory::GetMDCacheMaxNumObjects%-631368701.body
	}

	void SetMDCacheMaxNumObjects (m4uint32_t	ai_iMDCacheMaxNumObjects)
	{
//## begin ClCacheFactory::SetMDCacheMaxNumObjects%-897027360.body preserve=yes
        m_iMDCacheMaxNumObjects = ai_iMDCacheMaxNumObjects;
	
//## end ClCacheFactory::SetMDCacheMaxNumObjects%-897027360.body
	}

	m4uint32_t GetMDCacheRefreshRatio (void) const
	{
//## begin ClCacheFactory::GetMDCacheRefreshRatio%385689862.body preserve=yes
        return (m_iMDCacheRefreshRatio) ;
	
//## end ClCacheFactory::GetMDCacheRefreshRatio%385689862.body
	}

	void SetMDCacheRefreshRatio (m4uint32_t ai_iMDCacheRefreshRatio)
	{
//## begin ClCacheFactory::SetMDCacheRefreshRatio%458885766.body preserve=yes
        m_iMDCacheRefreshRatio = ai_iMDCacheRefreshRatio;
	
//## end ClCacheFactory::SetMDCacheRefreshRatio%458885766.body
	}


	m4uint32_t	GetDataCacheMaxMemory (void) const
	{
//## begin ClCacheFactory::GetDataCacheMaxMemory%1067811821.body preserve=yes
        return(m_iDataCacheMaxMemory) ;
	
//## end ClCacheFactory::GetDataCacheMaxMemory%1067811821.body
	}

	void	SetDataCacheMaxMemory (m4uint32_t ai_iDataCacheMaxMemory)
	{
//## begin ClCacheFactory::SetDataCacheMaxMemory%-2089160035.body preserve=yes
        m_iDataCacheMaxMemory = ai_iDataCacheMaxMemory;
	
//## end ClCacheFactory::SetDataCacheMaxMemory%-2089160035.body
	}

	m4uint32_t GetDataCacheMaxNumObjects (void) const
	{
//## begin ClCacheFactory::GetDataCacheMaxNumObjects%-231197258.body preserve=yes
        return (m_iDataCacheMaxNumObjects) ;
	
//## end ClCacheFactory::GetDataCacheMaxNumObjects%-231197258.body
	}

	void SetDataCacheMaxNumObjects (m4uint32_t ai_iDataCacheMaxNumObjects)
	{
//## begin ClCacheFactory::SetDataCacheMaxNumObjects%-1636729197.body preserve=yes
        m_iDataCacheMaxNumObjects = ai_iDataCacheMaxNumObjects;
	
//## end ClCacheFactory::SetDataCacheMaxNumObjects%-1636729197.body
	}

	m4uint32_t GetDataCacheRefreshRatio (void) const
	{
//## begin ClCacheFactory::GetDataCacheRefreshRatio%-123490667.body preserve=yes
        return (m_iDataCacheRefreshRatio) ;
	
//## end ClCacheFactory::GetDataCacheRefreshRatio%-123490667.body
	}	

	void SetDataCacheRefreshRatio (m4uint32_t ai_iDataCacheRefreshRatio)
	{
//## begin ClCacheFactory::SetDataCacheRefreshRatio%-1967710531.body preserve=yes
        m_iDataCacheRefreshRatio = ai_iDataCacheRefreshRatio;
	
//## end ClCacheFactory::SetDataCacheRefreshRatio%-1967710531.body
	}

private:
	m4return_t ReadCacheRegistry(ClVMBaseEnv * ai_poRegistry);

	ClCache * m_poCMCRCache;
	ClCache * m_poCSCRCache;
	ClCache * m_poPresCache;
	ClCache * m_poMapCache;
	ClCache * m_poDataCache;

    m4char_t    m_acCacheDirectory  [ M4CH_MAX_WORKING_DIRECTORY + 1 ] ;

	//caches de MD
	m4uint8_t	m_iMDDiskCache ;
	m4uint32_t	m_iMDCacheMaxMemory;
	m4uint32_t	m_iMDCacheMaxNumObjects;
	m4uint32_t	m_iMDCacheRefreshRatio;
	m4double_t	m_iMDCacheExpiryRatio;
	m4date_t	m_iMDCacheDefaultMaxPeriod;

	//caches de Datos
	m4uint8_t	m_iDataDiskCache ;
	m4uint32_t	m_iDataCacheMaxMemory;
	m4uint32_t	m_iDataCacheMaxNumObjects;
	m4uint32_t	m_iDataCacheRefreshRatio;
	m4double_t	m_iDataCacheExpiryRatio;
	m4date_t	m_iDataCacheDefaultMaxPeriod;
};

#endif
 
