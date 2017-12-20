//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                cachedir.cpp   
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
//    This module defines the functions of the class CacheDirectory
//
//
//==============================================================================

#include "cachedirbase.hpp"
#include "diskpers.hpp"
#include "diskpers.hpp"
#include "mdupdate.hpp"
#include "m4magicn.hpp"
#include "m4objreg.hpp"
#include "cldates.hpp"
#include "chlog.hpp"
#include "m4cachedirres.hpp"

ClCacheDirectoryBase::ClCacheDirectoryBase(void)
{
	m_dLastModificationDate = M4CL_MINUS_INFINITE_IN_JULIAN;
	m_bInit = M4_FALSE;
	m_poSttWrapper = 0;

}

ClCacheDirectoryBase::~ClCacheDirectoryBase(void)
{
	_End();
}

m4return_t ClCacheDirectoryBase::_Init (ClVMBaseEnv * ai_poRegistry)
{
	if (M4_TRUE == m_bInit)
		return M4_SUCCESS;

	if ( (ai_poRegistry) && (ai_poRegistry->GetMDDiskCache() != M4CL_CACHE_NONE) )
	{
		ClTypedId oTypeID( "CacheVersion" );
		ClUniqueKey oUniqueKey("");
		ClEnvironmentId oEnvId( ai_poRegistry->GetCacheDirectory() );
 
		ClObjectId * poObjID = new ClObjectId (CacheDirectoryObjectType, &oEnvId, &oTypeID, &oUniqueKey);

		SetObjId(poObjID);
	}
	else
	{
		m_dLastModificationDate = M4CL_PLUS_INFINITE_IN_JULIAN ;
	}

	return M4_SUCCESS;
}

m4return_t ClCacheDirectoryBase::_End ()
{
	if (M4_FALSE == m_bInit)
		return M4_SUCCESS;

	m_dLastModificationDate = M4CL_MINUS_INFINITE_IN_JULIAN;
	m_poSttWrapper = 0;
	m_oCacheList.clear();

	return M4_SUCCESS;
}

m4return_t ClCacheDirectoryBase::PersistDir  (void)
{
	if (GetObjId())
	{
		ClDiskPersistor oPersistor;
		return oPersistor.Persist(*this);
	}

	return M4_SUCCESS;
}

m4return_t ClCacheDirectoryBase::DePersistDir(void)
{	
	if (GetObjId())
	{
		ClDiskPersistor oPersistor;
		return oPersistor.DePersist(*this);
	}

	return M4_SUCCESS;
}

m4return_t ClCacheDirectoryBase::PersistCaches  (m4uint16_t ai_iCacheType)
{
	CACHELIST::iterator oCacheListIt;

	for (oCacheListIt = m_oCacheList.begin();
		oCacheListIt != m_oCacheList.end();
		++oCacheListIt)
	{
		if ( ai_iCacheType & (*oCacheListIt)->GetType() )
		{
			//Persistimos la cache
			(*oCacheListIt)->PersistCache();
		}
	}

	return M4_SUCCESS;
}

m4return_t ClCacheDirectoryBase::CleanCaches  (m4uint16_t ai_iCacheType)
{
	CACHELIST::iterator oCacheListIt;

	for (oCacheListIt = m_oCacheList.begin();
		oCacheListIt != m_oCacheList.end();
		++oCacheListIt)
	{
		if ( ai_iCacheType & (*oCacheListIt)->GetType() )
		{
			//Limpiamos la cache y persistimos
			(*oCacheListIt)->RemoveAll();
			(*oCacheListIt)->PersistCache();
		}
	}

	return M4_SUCCESS;
}

m4return_t ClCacheDirectoryBase::Serialize(ClGenericIODriver& IOD)
{
	m4return_t ret;

	ret = IOD.Write((m4uint32_t)M4_CH_MAGIC_NUMBER_VER);

	if (ret == M4_ERROR)
		return M4_ERROR;

	ret = IOD.Write(m_dLastModificationDate);

	if (ret == M4_ERROR)
		return M4_ERROR;

	ret = m_vRepositoryKey.Serialize(IOD);

	return ret;
}

m4return_t ClCacheDirectoryBase::DeSerialize(ClGenericIODriver& IOD)
{
	m4return_t ret;
	m4uint32_t iMagicNumber;

	ret = IOD.Read(iMagicNumber);

	if (ret == M4_ERROR)
		return M4_ERROR;

	if (iMagicNumber != M4_CH_MAGIC_NUMBER_VER)
	{
		g_oChLog<<BeginError(M4_CH_CACHEDIR_BAD_VER_MAGIC_NUMBER)<<iMagicNumber<<m4uint32_t(M4_CH_MAGIC_NUMBER_VER)<<EndLog;
		return M4_ERROR;
	}

	ret = IOD.Read(m_dLastModificationDate);

	if (ret == M4_ERROR)
		return M4_ERROR;

	ret = m_vRepositoryKey.DeSerialize(IOD);

	return ret;
}

void ClCacheDirectoryBase::Attach (ClBaseCacheInterface* ai_pCache)
{
	m_oCacheList.push_back(ai_pCache);
}

void ClCacheDirectoryBase::Detach (ClBaseCacheInterface* ai_pCache)
{
	m_oCacheList.remove(ai_pCache);
}

m4return_t ClCacheDirectoryBase::ActivateCaches	 (void)
{
	CACHELIST::iterator oCacheListIt;

	for (oCacheListIt = m_oCacheList.begin();
		oCacheListIt != m_oCacheList.end();
		++oCacheListIt)
	{
		(*oCacheListIt)->Init();
	}

	return M4_SUCCESS;
}


m4return_t ClCacheDirectoryBase::_RemoveObjectById (m4uint16_t ai_iCacheType, m4pchar_t ai_pcId, m4bool_t ai_bLocalOnly)
{
	//Limpia el objeto de la cache local
	CACHELIST::iterator oCacheListIt;

	for (oCacheListIt = m_oCacheList.begin();
		oCacheListIt != m_oCacheList.end();
		++oCacheListIt)
	{
		if ( ai_iCacheType & (*oCacheListIt)->GetType() )
		{
			(*oCacheListIt)->RemoveObjectById(ai_pcId);
		}
	}

	return M4_SUCCESS;
}


m4return_t ClCacheDirectoryBase::RemoveObjectByIdAndVersion (m4uint16_t ai_iCacheType, m4pchar_t ai_pcId, m4date_t ai_dVersion, ClBaseCacheInterface::eRemoveType_t ai_iRemoveType)
{
	//Limpia el objeto de la cache local
	CACHELIST::iterator oCacheListIt;

	for (oCacheListIt = m_oCacheList.begin();
		oCacheListIt != m_oCacheList.end();
		++oCacheListIt)
	{
		if ( ai_iCacheType & (*oCacheListIt)->GetType() )
		{
			(*oCacheListIt)->RemoveObjectByIdAndVersion(ai_pcId, ai_dVersion, ai_iRemoveType);
		}
	}

	return M4_SUCCESS;
}

m4return_t ClCacheDirectoryBase::ActualizeCachesByUpdatedList(ClMDUpdateDeltasList * ai_poUpdatedDeltasList)
{
	// Utiliza ClMDUpdateDeltasList para actualizar las caches:
	// - Si RK del cachedir es igual que el del updatelist:
	//	 - La lista de cambios tiene size > 0. Actualiza las cachés y la LMD. 
	//	 - La lista de cambios tiene size == 0. Borra sus cachés y actualiza la LMD.
	// - Si RK del cachedir es distinto que el del updatelist :
	//	 - Se borran las cachés y actualiza RK y LMD.

	if (GetObjId())
	{
		CACHELIST::iterator oCacheListIt;

		if ( ai_poUpdatedDeltasList->GetRepositoryKey() != m_vRepositoryKey )
		{
			//cambio de repositorio
			for (oCacheListIt = m_oCacheList.begin();
				oCacheListIt != m_oCacheList.end();
				++oCacheListIt)
			{

				(*oCacheListIt)->RemoveAll();
			}

			m_vRepositoryKey = ai_poUpdatedDeltasList->GetRepositoryKey();

			m_dLastModificationDate = ai_poUpdatedDeltasList->GetLastModificationDate();
		}
		else
		{
			if ( ai_poUpdatedDeltasList->GetLastModificationDate() > m_dLastModificationDate )
			{
				if (ai_poUpdatedDeltasList->m_oVersionList.size() == 0)
				{
					for (oCacheListIt = m_oCacheList.begin();
						oCacheListIt != m_oCacheList.end();
						++oCacheListIt)
					{
						//Borramos todas las caches
						(*oCacheListIt)->RemoveAll();
					}
				}
				else
				{
					ClMDVersionList::iterator it;
					ClMDVersion * poVersion;

					//Borra los objetos obsoletos.
					for (it = ai_poUpdatedDeltasList->m_oVersionList.begin(); it != ai_poUpdatedDeltasList->m_oVersionList.end(); it++)
					{
						poVersion = (*it);
						
						for (oCacheListIt = m_oCacheList.begin();
							oCacheListIt != m_oCacheList.end();
							++oCacheListIt)
						{
							if ( ( (poVersion->m_iMDType == M4_ITEM_MD_VERSION_ID_MD_TYPE_CMCR		) && ( (*oCacheListIt)->GetType() == M4_CACHE_CMCR_CACHE_TYPE		) ) ||
								 ( (poVersion->m_iMDType == M4_ITEM_MD_VERSION_ID_MD_TYPE_CSCR		) && ( (*oCacheListIt)->GetType() == M4_CACHE_CSCR_CACHE_TYPE		) ) ||
								 ( (poVersion->m_iMDType == M4_ITEM_MD_VERSION_ID_MD_TYPE_PRES		) && ( (*oCacheListIt)->GetType() == M4_CACHE_PRES_CACHE_TYPE		) ) ||
								 ( (poVersion->m_iMDType == M4_ITEM_MD_VERSION_ID_MD_TYPE_MAP		) && ( (*oCacheListIt)->GetType() == M4_CACHE_MAP_CACHE_TYPE		) ) ||
								 ( (poVersion->m_iMDType == M4_ITEM_MD_VERSION_ID_MD_TYPE_DATA		) && ( (*oCacheListIt)->GetType() == M4_CACHE_DATA_CACHE_TYPE		) ) ||
								 ( (poVersion->m_iMDType == M4_ITEM_MD_VERSION_ID_MD_TYPE_TABLE		) && ( (*oCacheListIt)->GetType() == M4_CACHE_TABLE_CACHE_TYPE		) ) ||
								 ( (poVersion->m_iMDType == M4_ITEM_MD_VERSION_ID_MD_TYPE_LITERALS	) && ( (*oCacheListIt)->GetType() == M4_CACHE_LITERALS_CACHE_TYPE	) ) )
							{
								(*oCacheListIt)->RemoveObjectByIdAndVersion(poVersion->m_vMDId.Data.PointerChar, poVersion->m_dMDVersion, ClBaseCacheInterface::MAIN);

								/*
								Si el objeto que se queda obsoleto es una presentación puede que otras presentaciones
								que dependan de esa se deban quedar obsoletas. El caso que tenemos de momento es una
								presentación que depende de includes. Si el include cambia se debe obsoletizar la
								presentación.
								*/
								if ( poVersion->m_iMDType == M4_ITEM_MD_VERSION_ID_MD_TYPE_PRES )
								{
									/*
									Si es un include se buscará en todas las presentaciones cacheadas si dependen de este objeto.
									*/
									(*oCacheListIt)->RemoveObjectByIdAndVersion(poVersion->m_vMDId.Data.PointerChar, poVersion->m_dMDVersion, ClBaseCacheInterface::DEPENDENCY);
								}
							}
							else if ( poVersion->m_iMDType == M4_ITEM_MD_VERSION_ID_MD_TYPE_CMCR )
							{
								//Si el objeto a invalidar es un Metacanal (CMCR) invalidamos tambien su mascara y presentacion. Canal no porque ya se invalidará el solo cuando se use.
								if ( (*oCacheListIt)->GetType() & ( M4_CACHE_CSCR_CACHE_TYPE  | M4_CACHE_PRES_CACHE_TYPE ) )
								{
									(*oCacheListIt)->RemoveObjectByIdAndVersion(poVersion->m_vMDId.Data.PointerChar, poVersion->m_dMDVersion, ClBaseCacheInterface::PARENT);
								}
							}
						}
					}
				}

				m_dLastModificationDate = ai_poUpdatedDeltasList->GetLastModificationDate();
			}
			else if ( ai_poUpdatedDeltasList->GetLastModificationDate() < m_dLastModificationDate )
			{
				//Warning. Es una situación que no se debería dar!!!
				g_oChLog<<BeginWarning(M4_CH_CACHEDIR_MD_LAST_METADATA_DATE_BIGGER)<<m_dLastModificationDate<<ai_poUpdatedDeltasList->GetLastModificationDate()<<EndLog;

				for (oCacheListIt = m_oCacheList.begin();
					oCacheListIt != m_oCacheList.end();
					++oCacheListIt)
				{
					(*oCacheListIt)->RemoveAll();
				}

				m_dLastModificationDate = ai_poUpdatedDeltasList->GetLastModificationDate();
			}
			else
			{
				//Estamos actualizados
			}
		}
	}

	return M4_SUCCESS;
}

m4return_t ClCacheDirectoryBase::ActualizeCachesByDate(m4date_t ai_dDate)
{
	CACHELIST::iterator oCacheListIt;

	for (oCacheListIt = m_oCacheList.begin();
		oCacheListIt != m_oCacheList.end();
		++oCacheListIt)
	{
		if ( ai_dDate )
		{
			// Si la fecha es valida limpiamos de las caches los obj anteriores a ella.
			(*oCacheListIt)->RemoveAll(ai_dDate);
		}
	}

	m_dLastModificationDate = ai_dDate;

	return M4_SUCCESS;
}

m4return_t ClCacheDirectoryBase::Refresh (ClMDUpdateList * ai_poUpdateList)
{
	m4return_t ret = M4_SUCCESS;

	if (M4_FALSE == m_bInit)
		return M4_ERROR;

	if (GetObjId())
	{
		//Hacemos el Refresh de las caches.
		ClMDUpdateDeltasList * poUpdatedDeltasList = 0;

		ret = DePersistDir();

		ret = GetUpdatedList(poUpdatedDeltasList, ai_poUpdateList);

		if ( (ret == M4_SUCCESS) && (poUpdatedDeltasList != 0) )
		{
			ret = ActualizeCachesByUpdatedList(poUpdatedDeltasList);
			
			ret = PersistCaches();

			ret = PersistDir();

			delete poUpdatedDeltasList;
		}
		else
		{
			//Si hubo error borramos las caches.
			CleanCaches(M4_CACHE_ALL_MD_CACHE_TYPE);

			PersistCaches();

			ret = M4_ERROR;
		}
	}

	if (M4_ERROR == ret)
	{
		g_oChLog<<BeginError(M4_CH_CACHEDIR_NO_MD_CACHE_REFREH)<<EndLog;		
	}

	return ret;
}

m4return_t ClCacheDirectoryBase::ReloadCaches (void)
{
	CACHELIST::iterator oCacheListIt;

	for (oCacheListIt = m_oCacheList.begin();
		oCacheListIt != m_oCacheList.end();
		++oCacheListIt)
	{
		(*oCacheListIt)->Reload();
	}

	return M4_SUCCESS;
}