
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdrt.dll
// File:                m4mdcac.cpp    
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                09-03-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este modulo implementa las cachés del metacanal compilado
//
//
//==============================================================================



#include "m4mdrt.hpp"

// local definitions
#define LOCAL_TEMP_STRING 32

//=================================================================================
//
// ClCMCRContainer
//
// Clase caché de objeto cacheable metacanal compilado
//
//=================================================================================

ClCMCRCache::ClCMCRCache(ClCache * ai_poCache, m4pcchar_t ai_pPath, m4uint8_t ai_iMode, m4uint32_t ai_iMaxMemory, m4uint32_t ai_iNumObjects, m4uint32_t ai_iRefreshRatio, m4double_t ai_dMaxPeriod, m4double_t ai_dMaxUnusedPeriod) :
	ClBaseCache<ClCMCRWrapper, ClCMCRContainer>(ai_poCache, ai_pPath, "CMCRCache", ai_iMode, ai_iMaxMemory, ai_iNumObjects, ai_iRefreshRatio, ai_dMaxPeriod, ai_dMaxUnusedPeriod)
{
	static ClCMCRCacheableObjectFactory * pCoFactory = new ClCMCRCacheableObjectFactory();
	SetPersistor(pCoFactory);
	SetCOFactory(pCoFactory);
}


ClCMCRCache::~ClCMCRCache ()
{
}

m4return_t ClCMCRCache::GetObject (m4uint8_t ai_iCsType, m4pcchar_t ai_pccChannelId, m4uint8_t ai_iLanguage, m4VariantType &ai_roStartDate, m4VariantType &ai_roEndDate, m4VariantType &ai_roCorDate, m4uint16_t &ao_iVerCounter, ClCMCRWrapper* &ao_rpCMCR)
{
	m4return_t ret = M4_SUCCESS;
	m4pchar_t pcCMCRId;

	pcCMCRId = ComposeStringKey(ai_iCsType, ai_pccChannelId, ai_iLanguage);

	ret = _GetObject(pcCMCRId, ai_roStartDate.Data.DoubleData, ai_roEndDate.Data.DoubleData, ai_roCorDate.Data.DoubleData, ao_iVerCounter, ao_rpCMCR);									 

	delete [] pcCMCRId;	

	return ret;
}

m4return_t ClCMCRCache::PutObject (m4uint8_t ai_iCsType, m4pcchar_t ai_pccChannelId, m4uint8_t ai_iLanguage, m4uint16_t ai_iVerCounter, ClCMCRWrapper* ai_pCMCR, m4date_t ai_dMaxPeriod)
{
	m4return_t ret = M4_SUCCESS;
	m4pchar_t pcCMCRId;

	pcCMCRId = ComposeStringKey(ai_iCsType, ai_pccChannelId, ai_iLanguage);

	ret = _PutObject(pcCMCRId, ai_iVerCounter, ai_pCMCR, ai_dMaxPeriod, 0);									 

	delete [] pcCMCRId;	

	return ret;
}

m4pchar_t ClCMCRCache::ComposeStringKey(m4uint8_t ai_iCsType, m4pcchar_t ai_pccChannelId, m4uint8_t ai_iLanguage)
{

	m4char_t pcTemp [LOCAL_TEMP_STRING];


	sprintf(pcTemp,"%d@%1d", ai_iLanguage, ai_iCsType);
	
	m4pchar_t pcCMCRId = new m4char_t[strlen(ai_pccChannelId) + strlen(pcTemp) + 2];

	sprintf(pcCMCRId, "%s@%s", ai_pccChannelId, pcTemp);

	strupr(pcCMCRId);

	return pcCMCRId;
}

m4return_t ClCMCRCache::RemoveObjectById (m4pchar_t ai_pcChannelId, m4puint32_t ai_piRemovedCount)
{
	m4return_t ret = M4_SUCCESS;

	//Chicle asqueroso
	m4pchar_t pcCMCRId = new m4char_t[strlen(ai_pcChannelId) + 2];

	sprintf(pcCMCRId, "%s@", ai_pcChannelId);

	strupr(pcCMCRId);

    //Hacemos remove de todos los objetos que tengan en su clave la cadena.
	ret = m_poCache->RemoveAll(pcCMCRId, ai_piRemovedCount);

	delete [] pcCMCRId;	

	return ret;
}

m4return_t ClCMCRCache::RemoveObjectByIdAndVersion (m4pchar_t ai_pcChannelId, m4date_t ai_dVersion, ClBaseCacheInterface::eRemoveType_t ai_iRemoveType)
{
	//para CMCR ai_bParentObject no sirve de nada.
	m4return_t ret = M4_SUCCESS;
	
	m4pchar_t pcCMCRId = new m4char_t[strlen(ai_pcChannelId) + 2];

	sprintf(pcCMCRId, "%s@", ai_pcChannelId);

	strupr(pcCMCRId);

	ClCOCondition oCondId, oCondVer;

	oCondId.m_iConditionMask = M4_CO_COND_CONTAINED_IN_KEY;
	oCondId.m_pValue = pcCMCRId;
	oCondId.m_iAnd = 1;
	oCondId.m_pNext = &oCondVer;

	oCondVer.m_iConditionMask = M4_CO_COND_OLDER_VERSION;
	oCondVer.m_pValue = &ai_dVersion;
	oCondVer.m_iAnd = 1;
	oCondVer.m_pNext = 0;

	ret = m_poCache->RemoveByCondition(&oCondId);

	delete [] pcCMCRId;	

	return ret;

}

m4bool_t ClCMCRContainer::MatchCondition (void * ai_pCondition)
{
	m4bool_t bValue = M4_FALSE;
	ClCOCondition * pCondition = (ClCOCondition*)ai_pCondition;

	if (!pCondition)
		return M4_FALSE;

	if (!pCondition->m_pValue)
		return M4_FALSE;

	bValue = M4_FALSE;

	M4_ASSERT(m_dVersion);

	switch (pCondition->m_iConditionMask)
	{
		case M4_CO_COND_CONTAINED_IN_KEY:
			//Se cumple si ai_pcPortionKey se encuentra al principio de Getm_pszKey()!!
			if(strstr(m_poClCacheKey->Getm_pszKey(),(m4pchar_t)pCondition->m_pValue) == m_poClCacheKey->Getm_pszKey())		
			{
				bValue = M4_TRUE;
			}

			break;

		case M4_CO_COND_OLDER_VERSION:
			if ( m_dVersion < *(m4date_t*)(pCondition->m_pValue) )
			{
				bValue = M4_TRUE;
			}
			break;

		default:
			break;
	};

	if (pCondition->m_pNext)
	{
		if ( ((pCondition->m_iAnd) && (M4_TRUE == bValue)) || ((!pCondition->m_iAnd) && (M4_FALSE == bValue)) )
		{
			bValue = MatchCondition(pCondition->m_pNext);
		}
	}

	return bValue;
}



//=================================================================================
//
// ClCSCRCache
//
// Clase caché de objeto cacheable seguridad compilada
//
//=================================================================================

ClCSCRCache::ClCSCRCache(ClCache * ai_poCache, m4pcchar_t ai_pPath, m4uint8_t ai_iMode, m4uint32_t ai_iMaxMemory, m4uint32_t ai_iNumObjects, m4uint32_t ai_iRefreshRatio, m4double_t ai_dMaxPeriod, m4double_t ai_dMaxUnusedPeriod) :
	ClBaseCache<ClCSCRWrapper, ClCSCRContainer>(ai_poCache, ai_pPath, "CSCRCache", ai_iMode,ai_iMaxMemory, ai_iNumObjects, ai_iRefreshRatio, ai_dMaxPeriod, ai_dMaxUnusedPeriod)
{
	static ClCSCRCacheableObjectFactory * pCoFactory = new ClCSCRCacheableObjectFactory();
	SetPersistor(pCoFactory);
	SetCOFactory(pCoFactory);
}


ClCSCRCache::~ClCSCRCache ()
{
}

m4return_t ClCSCRCache::GetObject (m4uint8_t ai_iCsType, m4pcchar_t ai_pccChannelId, m4pcchar_t ai_pccMask, ClCSCRWrapper* &ao_rpCSCR)
{
	m4return_t ret = M4_SUCCESS;
	m4pchar_t pcCSCRId;
	m4uint16_t iVerCounter; //No utilizamos su valor de vuelta
	
	pcCSCRId = ComposeStringKey(ai_iCsType, ai_pccChannelId, ai_pccMask);

	ret = _GetObject(pcCSCRId, ClMinusInfiniteDate(), ClPlusInfiniteDate(), ClActualDate(), iVerCounter, ao_rpCSCR);									 

	delete [] pcCSCRId;	

	return ret;
}

m4return_t ClCSCRCache::PutObject (m4uint8_t ai_iCsType, m4pcchar_t ai_pccChannelId, m4pcchar_t ai_pccMask, ClCSCRWrapper* ai_pCMCR, m4date_t ai_dMaxPeriod)
{
	m4return_t ret = M4_SUCCESS;
	m4pchar_t pcCSCRId;
	
	pcCSCRId = ComposeStringKey(ai_iCsType, ai_pccChannelId, ai_pccMask);

	ret = _PutObject(pcCSCRId, 0, ai_pCMCR, ai_dMaxPeriod, 0);									 

	delete [] pcCSCRId;	

	return ret;
}

m4pchar_t ClCSCRCache::ComposeStringKey(m4uint8_t ai_iCsType, m4pcchar_t ai_pccChannelId, m4pcchar_t ai_pccMask)
{
	m4pchar_t pcCSCRId = new m4char_t[strlen(ai_pccChannelId) + strlen(ai_pccMask) + 3 + 1];

	sprintf(pcCSCRId, "%s@%s@%1d", ai_pccChannelId, ai_pccMask, ai_iCsType);

	return pcCSCRId;
}

m4return_t ClCSCRCache::RemoveObjectById (m4pchar_t ai_pcChannelId, m4puint32_t ai_piRemovedCount)
{
	m4return_t ret = M4_SUCCESS;

	//Chicle asqueroso
	m4pchar_t pcCSCRId = new m4char_t[strlen(ai_pcChannelId) + 2];

	sprintf(pcCSCRId, "%s@", ai_pcChannelId);

	strupr(pcCSCRId);

    //Hacemos remove de todos los objetos que tengan en su clave la cadena.
	ret = m_poCache->RemoveAll(pcCSCRId, ai_piRemovedCount);

	delete [] pcCSCRId;	

	return ret;
}

m4return_t ClCSCRCache::RemoveObjectByIdAndVersion (m4pchar_t ai_pcChannelId, m4date_t ai_dVersion, ClBaseCacheInterface::eRemoveType_t ai_iRemoveType)
{
	m4return_t ret = M4_SUCCESS;
	
	m4pchar_t pcCSCRId = new m4char_t[strlen(ai_pcChannelId) + 2];

	sprintf(pcCSCRId, "%s@", ai_pcChannelId);

	strupr(pcCSCRId);

	ClCOCondition oCondId, oCondVer;

	oCondId.m_iConditionMask = M4_CO_COND_CONTAINED_IN_KEY;
	oCondId.m_pValue = pcCSCRId;
	oCondId.m_iAnd = 1;
	oCondId.m_pNext = &oCondVer;

	// Si ai_bParentObject hay que quitar la CSCR cuando su version de CMCR (T3) sea distinta.
	if (ai_iRemoveType == ClBaseCacheInterface::MAIN)
	{
		oCondVer.m_iConditionMask = M4_CO_COND_OLDER_VERSION;
	}
	else
	{
		oCondVer.m_iConditionMask = M4_CO_COND_OLDER_PARENT_VERSION;
	}

	oCondVer.m_pValue = &ai_dVersion;
	oCondVer.m_iAnd = 1;
	oCondVer.m_pNext = 0;

	ret = m_poCache->RemoveByCondition(&oCondId);

	delete [] pcCSCRId;	

	return ret;

}


m4return_t ClCSCRContainer::Serialize(ClGenericIODriver& IOD)
{
	m4return_t ret;

	ret = ClDatedContainer <ClCSCRWrapper>::Serialize(IOD);

	if (M4_ERROR == ret)
		return M4_ERROR;

	ret = IOD.Write(m_dT3Version);

	if (ret == M4_ERROR)
		return M4_ERROR;

	return M4_SUCCESS;
}

m4return_t ClCSCRContainer::DeSerialize(ClGenericIODriver& IOD)
{
	m4return_t ret;

	ret = ClDatedContainer <ClCSCRWrapper>::DeSerialize(IOD);

	if (M4_ERROR == ret)
		return M4_ERROR;

	ret = IOD.Read(m_dT3Version);

	if (ret == M4_ERROR)
		return M4_ERROR;

	return M4_SUCCESS;
}

m4return_t ClCSCRContainer::SetValue ( ClCSCRWrapper * ai_pObject )
{
	ClDatedContainer <ClCSCRWrapper>::SetValue(ai_pObject);

	if (ai_pObject)
	{
		m_dT3Version = ((ClCSCRWrapper*)GetValue())->GetT3Version();
	}

	return M4_SUCCESS;
}


m4bool_t ClCSCRContainer::MatchCondition (void * ai_pCondition)
{
	m4bool_t bValue = M4_FALSE;
	ClCOCondition * pCondition = (ClCOCondition*)ai_pCondition;

	if (!pCondition)
		return M4_FALSE;

	if (!pCondition->m_pValue)
		return M4_FALSE;

	bValue = M4_FALSE;

	M4_ASSERT(m_dVersion);

	switch (pCondition->m_iConditionMask)
	{
		case M4_CO_COND_CONTAINED_IN_KEY:
			//Se cumple si ai_pcPortionKey se encuentra al principio de Getm_pszKey()!!
			if(strstr(m_poClCacheKey->Getm_pszKey(),(m4pchar_t)pCondition->m_pValue) == m_poClCacheKey->Getm_pszKey())		
			{
				bValue = M4_TRUE;
			}

			break;

		case M4_CO_COND_OLDER_VERSION:
			if ( m_dVersion < *(m4date_t*)(pCondition->m_pValue) )
			{
				bValue = M4_TRUE;
			}
			break;

		case M4_CO_COND_OLDER_PARENT_VERSION:
			if ( m_dT3Version < *(m4date_t*)(pCondition->m_pValue) )
			{
				bValue = M4_TRUE;
			}
			break;

		default:
			break;
	};

	if (pCondition->m_pNext)
	{
		if ( ((pCondition->m_iAnd) && (M4_TRUE == bValue)) || ((!pCondition->m_iAnd) && (M4_FALSE == bValue)) )
		{
			bValue = MatchCondition(pCondition->m_pNext);
		}
	}

	return bValue;
}


