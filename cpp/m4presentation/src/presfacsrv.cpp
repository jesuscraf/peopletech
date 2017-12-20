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


#include "presfacsrv.hpp"
#include "cldates.hpp"
#include "clpresen.hpp"
#include "asndef.h"
#include "prescache.hpp"
#include "m4presres.hpp"
#include "chlog.hpp"

// factoria ===================================================

ClPresentationFactoryServer::ClPresentationFactoryServer (void)
{
	m_poInternalPresFactory = 0;
}

ClPresentationFactoryServer::~ClPresentationFactoryServer (void)
{
	End();
}


m4return_t ClPresentationFactoryServer::Init (ClVMBaseEnv * ai_poRegistry, ClCsTrace * ai_poCsTrace, ClVMStatisticsWrapper * ai_poSttWrapper, ClChannelManager * ai_poChannelManager, ClCacheDirectory * ai_poCacheDir, ClCache * ai_poPresCache)
{
	if (m_bInit == M4_TRUE)
		return M4_SUCCESS;

	if (!ai_poChannelManager)
		return M4_ERROR;

	m_poChannelManager = ai_poChannelManager;

	if (!ai_poCsTrace)
		return M4_ERROR;

	m_poCsTrace = ai_poCsTrace;
	
	if (!ai_poSttWrapper)
		return M4_ERROR;

	m_poSttWrapper = ai_poSttWrapper;

	if (!ai_poCacheDir)
		return M4_ERROR;

	m_poCacheDir = ai_poCacheDir;

	if (M4_ERROR == InitCache(ai_poPresCache, ai_poRegistry))
		return M4_ERROR;

	/* m_poInternalPresFactory 
	La creamos al realizar la primera operación, ya que hay que crear canales y todavía en
	este punto no tenemos canal sesion*/

	m_bInit = M4_TRUE;

	return M4_SUCCESS;
}

m4return_t ClPresentationFactoryServer::End()
{
	if (M4_FALSE == m_bInit)
		return M4_SUCCESS;

	m_poChannelManager = 0;

	if (m_poInternalPresFactory)
	{
		delete m_poInternalPresFactory;
		m_poInternalPresFactory = 0;
	}

	_End();

	m_bInit = M4_FALSE;

	return M4_SUCCESS;
}

m4return_t ClPresentationFactoryServer::_InternalBuildPresentationFromId(const ClPresentationAttributes & ai_ClPresentationAttributes, m4bool_t ai_bDefinition, ClPresentation* &ao_rpPresentation) 
{
	if (!m_poInternalPresFactory)
	{
		/*En la primera operación se crea la factoria interna.
		no se crea en la inicialización, porque se necesita crear canales y para ello un
		canal session, y en la inicializacion de la factoria server todavía no hay canal 
		sesion*/
		m_poInternalPresFactory = new CInternalPresentationFactory(this, m_poChannelManager);
	}

	// Construye la presentación
	m4return_t iResult;
	ao_rpPresentation = 0;

	// Construye la Presentation Basica
	ClPresentation * pBasicPresentation = 0;
	ClPresentationAttributes oClPresentationAttributesBasic(ai_ClPresentationAttributes.m_pIdPresentation, ai_ClPresentationAttributes.m_pOrganization, "", ai_ClPresentationAttributes.m_iLanguage);

	iResult = M4_ERROR;

	// Buscamos en la cache del server la presentación básica
	// En tiempo de diseño no la buscamos. 
	if (ai_bDefinition == M4_FALSE && m_poPresCache)
	{
		iResult = m_poPresCache->GetObject (oClPresentationAttributesBasic, pBasicPresentation);
	}

	if (iResult == M4_ERROR)
	{
		// Construye la presentacion basica
		CBufferASN * pBasicBufferASN = NULL;

		iResult = m_poInternalPresFactory->BuildBasicPresentationFromId(oClPresentationAttributesBasic, ai_bDefinition, pBasicBufferASN);

		if (iResult == M4_ERROR)
		{
			g_oChLog<<BeginError(M4_CH_PRES_BUILD_BASIC_PRESENTATION)<<ai_ClPresentationAttributes.m_pIdPresentation<<ai_ClPresentationAttributes.m_pRole<<EndLog;
			return M4_ERROR;
		}
		
		pBasicPresentation = new ClPresentation(pBasicBufferASN);

		// Lo metemos en la cache del server la presentación básica
		// Pero no, si estamos en tiempo de diseño
		if (ai_bDefinition == M4_FALSE && m_poPresCache)
		{
			//Borramos de la caché todas las presentaciones con el mismo ID y versión anterior.
			m_poPresCache->RemoveObjectByIdAndVersion( (m4pchar_t) ai_ClPresentationAttributes.m_pIdPresentation, pBasicPresentation->GetVersion(), ClBaseCacheInterface::MAIN);

			//Hacemos el Put de la caché.
			iResult = m_poPresCache->PutObject (oClPresentationAttributesBasic, pBasicPresentation);

			if (iResult == M4_ERROR)
			{
				g_oChLog<<BeginWarning(M4_CH_PRES_CACHE_PUT_PRESENTATION)<<ai_ClPresentationAttributes.m_pIdPresentation<<ai_ClPresentationAttributes.m_pRole<<EndLog;
				iResult = M4_SUCCESS;
			}
		}
	}

	
	CBufferASN * pBufferASN = NULL;

	// Construye la presentation especializada. En runtime, resolvemos includes, grants, etc.
	// En tiempo de diseño se llama a la función que construye la presentación especializada, 
	// pero en esta función no resolvemos includes, grants, etc. Implementado para bug id 0128589
	// -------------------------------------------------------

	iResult = m_poInternalPresFactory->BuildPresentation( ai_ClPresentationAttributes.m_pIdPresentation, ai_ClPresentationAttributes, pBasicPresentation->GetBufferASN(), ai_bDefinition, pBufferASN );

	if (iResult == M4_ERROR)
	{
		g_oChLog<<BeginError(M4_CH_PRES_BUILD_PRESENTATION)<<ai_ClPresentationAttributes.m_pIdPresentation<<ai_ClPresentationAttributes.m_pRole<<EndLog;
	}
	else
	{
		ao_rpPresentation = new ClPresentation(pBufferASN);
		// El pBufferASN solo contiene los includes de la presentacion si estamos en runtime
	}

	//Liberamos la pres basica de la cache del server, si estaba.
	if (ai_bDefinition == M4_FALSE && m_poPresCache)
	{
		m_poPresCache->FreeObject (pBasicPresentation);
	}
	else
	{
		delete pBasicPresentation;
	}
 
	return iResult;
}

