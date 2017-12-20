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

#include "cachedircli.hpp"
#include "sttlib.hpp"
#include "sttwrap.hpp"
#include "m4mdadaptor.hpp"

ClCacheDirectoryClient::ClCacheDirectoryClient(void)
{
	m_poMDAdaptor = 0;
}

ClCacheDirectoryClient::~ClCacheDirectoryClient(void)
{
	End();
}

m4return_t ClCacheDirectoryClient::Init (ClVMBaseEnv * ai_poRegistry, ClVMStatisticsWrapper * ai_poSttWrapper, ClMDAdaptor * ai_poMDAdaptor)
{
	if (M4_TRUE == m_bInit)
		return M4_SUCCESS;

	if (M4_ERROR == _Init(ai_poRegistry))
		return M4_ERROR;

	if (!ai_poSttWrapper)
		return M4_ERROR;

	m_poSttWrapper = ai_poSttWrapper;

	if (!ai_poMDAdaptor)
		return M4_ERROR;

	m_poMDAdaptor = ai_poMDAdaptor;

	m_bInit = M4_TRUE;

	return M4_SUCCESS;
}

m4return_t ClCacheDirectoryClient::End ()
{
	if (M4_FALSE == m_bInit)
		return M4_SUCCESS;

	m_poMDAdaptor = 0;

	_End();

	m_bInit = M4_FALSE;

	return M4_SUCCESS;
}


m4return_t ClCacheDirectoryClient::RemoveObjectById (m4uint16_t ai_iCacheType, m4pchar_t ai_pcId, m4bool_t ai_bLocalOnly)
{
	if (M4_FALSE == m_bInit)
		return M4_ERROR;

	//Para tomar el tiempo desde que entra hasta que sale de esta funcion
	ClSttStateTimer oSttStateTimer(m_poSttWrapper->GetStatistics(), 
								   m_poSttWrapper->GetStatisticsLevel(), 
								   M4_VM_STT_SPECIAL_ITEM_CACHE_REMOVE_OBJECT);

	m4return_t iResult;

	//Lo borramos en local...
	iResult = _RemoveObjectById(ai_iCacheType, ai_pcId, ai_bLocalOnly);

	//Y en remoto
	iResult = m_poMDAdaptor->RemoteCacheRemove(ai_iCacheType, ai_pcId, m_poSttWrapper);

	return iResult;
}

m4return_t ClCacheDirectoryClient::GetUpdatedList(ClMDUpdateDeltasList *& ao_poUpdatedDeltasList, ClMDUpdateList * ai_poUpdateList)
{
	if (M4_FALSE == m_bInit)
		return M4_ERROR;

	//Para tomar el tiempo desde que entra hasta que sale de esta funcion
	ClSttStateTimer oSttStateTimer(m_poSttWrapper->GetStatistics(), 
								   m_poSttWrapper->GetStatisticsLevel(), 
								   M4_VM_STT_SPECIAL_ITEM_METADATA_GET_UPDATE_LIST);

	m4return_t iResult;
	ao_poUpdatedDeltasList = 0;

	//Pedimos la lista al server
	iResult = m_poMDAdaptor->RemoteGetMDUpdatedList(this, ao_poUpdatedDeltasList, m_poSttWrapper);

	return iResult;
}


