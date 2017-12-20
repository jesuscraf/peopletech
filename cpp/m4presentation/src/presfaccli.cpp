//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                presfac.cpp   
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
//    
//
//
//==============================================================================

#include "presfaccli.hpp"
#include "cldates.hpp"
#include "clpresen.hpp"
#include "prescache.hpp"
#include "m4mdadaptor.hpp"

// factoria ===================================================

ClPresentationFactoryClient::ClPresentationFactoryClient (void)
{
	m_poMDAdaptor = 0;
}

ClPresentationFactoryClient::~ClPresentationFactoryClient (void)
{
	End();
}


m4return_t ClPresentationFactoryClient::Init (ClVMBaseEnv * ai_poRegistry, ClCsTrace * ai_poCsTrace, ClVMStatisticsWrapper * ai_poSttWrapper, ClChannelManager * ai_poChannelManager, ClMDAdaptor * ai_poMDAdaptor, ClCacheDirectory * ai_poCacheDir, ClCache * ai_poPresCache)
{
	if (m_bInit == M4_TRUE)
		return M4_SUCCESS;
	
	if (!ai_poMDAdaptor)
		return M4_ERROR;

	m_poMDAdaptor = ai_poMDAdaptor;

	if (!ai_poCsTrace)
		return M4_ERROR;

	m_poCsTrace = ai_poCsTrace;
	
	if (!ai_poSttWrapper)
		return M4_ERROR;

	m_poSttWrapper = ai_poSttWrapper;

	if (ai_poChannelManager == NULL) {
		return M4_ERROR;
	}
	m_poChannelManager = ai_poChannelManager;

	if (!ai_poCacheDir)
		return M4_ERROR;

	m_poCacheDir = ai_poCacheDir;

	if (M4_ERROR == InitCache(ai_poPresCache, ai_poRegistry))
		return M4_ERROR;

	m_bInit = M4_TRUE;

	return M4_SUCCESS;
}

m4return_t ClPresentationFactoryClient::End()
{
	if (M4_FALSE == m_bInit)
		return M4_SUCCESS;

	m_poMDAdaptor = 0;

	_End();

	m_bInit = M4_FALSE;

	return M4_SUCCESS;
}

m4return_t ClPresentationFactoryClient::_InternalBuildPresentationFromId(const ClPresentationAttributes & ai_ClPresentationAttributes, m4bool_t ai_bDefinition, ClPresentation* &ao_rpPresentation) 
{
	// Pide la presentación al MDAdaptor
	m4return_t iResult;
	ao_rpPresentation = 0;

	iResult = m_poMDAdaptor->RemoteGetPresentation(ai_ClPresentationAttributes.m_pIdPresentation,
												   ai_ClPresentationAttributes,
												   ai_bDefinition,
												   ao_rpPresentation,
												   m_poSttWrapper);

	return iResult;
}
