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

#include "cachedirsrv.hpp"
#include "mdupdate.hpp"
#include "sttlib.hpp"
#include "sttwrap.hpp"
#include "cldates.hpp"


ClCacheDirectoryServer::ClCacheDirectoryServer(void)
{
	m_poChannelManager = 0;
}

ClCacheDirectoryServer::~ClCacheDirectoryServer(void)
{
	End();
}

m4return_t ClCacheDirectoryServer::Init (ClVMBaseEnv * ai_poRegistry, ClVMStatisticsWrapper * ai_poSttWrapper, ClChannelManager * ai_poChannelManager)
{
	if (M4_TRUE == m_bInit)
		return M4_SUCCESS;

	if (M4_ERROR == _Init(ai_poRegistry))
		return M4_ERROR;

	if (!ai_poSttWrapper)
		return M4_ERROR;

	m_poSttWrapper = ai_poSttWrapper;

	if (!ai_poChannelManager)
		return M4_ERROR;

	m_poChannelManager = ai_poChannelManager;

	m_bInit = M4_TRUE;

	return M4_SUCCESS;
}

m4return_t ClCacheDirectoryServer::End ()
{
	if (M4_FALSE == m_bInit)
		return M4_SUCCESS;

	m_poChannelManager = 0;

	_End();

	m_bInit = M4_FALSE;

	return M4_SUCCESS;
}

m4return_t ClCacheDirectoryServer::RemoveObjectById (m4uint16_t ai_iCacheType, m4pchar_t ai_pcId, m4bool_t ai_bLocalOnly)
{
	if (M4_FALSE == m_bInit)
		return M4_ERROR;

	//Para tomar el tiempo desde que entra hasta que sale de esta funcion
	ClSttStateTimer oSttStateTimer(m_poSttWrapper->GetStatistics(), 
								   m_poSttWrapper->GetStatisticsLevel(), 
								   M4_VM_STT_SPECIAL_ITEM_CACHE_REMOVE_OBJECT);

	m4return_t iResult;
	
	//Lo borramos en local.
	iResult = _RemoveObjectById(ai_iCacheType, ai_pcId, ai_bLocalOnly);

	return iResult;
}

m4return_t ClCacheDirectoryServer::GetUpdatedList(ClMDUpdateDeltasList *& ao_poUpdatedDeltasList, ClMDUpdateList * ai_poUpdateList)
{
	if (M4_FALSE == m_bInit)
		return M4_ERROR;

	//Para tomar el tiempo desde que entra hasta que sale de esta funcion
	ClSttStateTimer oSttStateTimer(m_poSttWrapper->GetStatistics(), 
								   m_poSttWrapper->GetStatisticsLevel(), 
								   M4_VM_STT_SPECIAL_ITEM_METADATA_GET_UPDATE_LIST);

	m4return_t iResult = M4_SUCCESS;
	ClMDUpdateList * pAuxList = 0;
	ao_poUpdatedDeltasList = 0;
	
	if (ai_poUpdateList == 0)
	{
		//No hay lista de cambios. La creamos.
		pAuxList = new ClMDUpdateList(m_dLastModificationDate, 1000);

		if (m_dLastModificationDate == M4CL_MINUS_INFINITE_IN_JULIAN)
		{
			//Si es MinusInfiniteDate solo nos interesa LastModificationDate y RepositoryKey
			iResult = pAuxList->RefreshLastModificationDate(m_poChannelManager);
		}
		else if (m_dLastModificationDate != M4CL_PLUS_INFINITE_IN_JULIAN)
		{
			//Si es PlusInfiniteDate no refrescamos la lista. No queremos nada.
			iResult = pAuxList->RefreshList(m_poChannelManager);

			//iResult = pAuxList->CutList();
			
			//para que en SM no ocurra que LastModDate < FirstModDate
			pAuxList->SetFirstModificationDate(m_dLastModificationDate);
		}
		else
		{
			//Si es PlusInfiniteDate no refrescamos la lista. No queremos nada.
			iResult = M4_SUCCESS;
		}

		if (M4_ERROR == iResult)
		{
			delete pAuxList;
			return M4_ERROR;
		}

		ai_poUpdateList = pAuxList;
	}
	
	iResult = ai_poUpdateList->CreateUpdateDeltasList( m_dLastModificationDate, m_vRepositoryKey, ao_poUpdatedDeltasList);

	if (pAuxList)
		delete pAuxList;

	if (!ao_poUpdatedDeltasList)
	{
		return M4_ERROR;
	}

	return iResult;
}

