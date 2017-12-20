//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:           sttwrap.cpp
// Project:        Calculo
// Author:         Meta Software M.S. , S.A
// Date:             21 may 97
// Language:        C++ 
// Operating System: WINDOWS 
// Design Document:  
//
//
// Definition:
// 
//   
////
//==============================================================================


#include "sttwrap.hpp"
#include "dm.hpp"
#include "sttlib.hpp"


ClVMStatisticsWrapper::ClVMStatisticsWrapper(m4bool_t ai_bIsClient)
{
	m_pStatistics = NULL;
	m_iSttInit = M4_VM_STATISTICS_DISABLED;
	m_bSttGlobal = M4_FALSE;
	m_iSttRef = 0;
	m_bPause = M4_FALSE;
	m_bIsClient = ai_bIsClient;
}

ClVMStatisticsWrapper::~ClVMStatisticsWrapper()
{
	DestroyStatistics();
}

m4return_t	ClVMStatisticsWrapper::CreateStatistics (ClChannelManager * ai_pChannelManager)
{
	if (ai_pChannelManager == NULL)
		return M4_ERROR;

	if (m_pStatistics == NULL)
	{
		m_pStatistics = new ClVMStatistics(m_bIsClient);

		if ( m_pStatistics->sttInit (ai_pChannelManager) == M4_ERROR )
		{
			delete m_pStatistics;

			m_pStatistics = NULL;

			//Error de inicialiacion de estadisticas 
			//g_oChLog<<BeginWarning(M4_CH_VM_STT_INIT)<<(IsBuilt ()? GetpChannelDef ()->Id () : "<channel not built>")<<EndLog;
		
			return M4_ERROR;
		}
	}

	return M4_SUCCESS;
}

m4return_t	ClVMStatisticsWrapper::DestroyStatistics (void)
{
	if (m_pStatistics != NULL)
	{
		delete m_pStatistics;
		m_pStatistics = NULL;
	}
	
	/* Bug 0107283
	Los valores hay que resetearlos aunque no haya estadísticas creadas
	*/
	m_iSttInit = M4_VM_STATISTICS_DISABLED;
	m_bSttGlobal = M4_FALSE;
	m_iSttRef = 0;	
	m_bPause = M4_FALSE;

	return M4_SUCCESS;
}

m4return_t	ClVMStatisticsWrapper::SetStatisticsLevel (m4uint16_t ai_iInit, ClChannelManager * ai_pChannelManager, m4bool_t ai_bGLobal)
{
	if ( (m_bSttGlobal == M4_TRUE) && (ai_bGLobal == M4_FALSE) )
	{
		//Nadie puede cambiar el level si no es el owner. (Nuevo parámetro por defecto false)
		return M4_SUCCESS;
	}

	if (m_bSttGlobal == M4_TRUE)
	{
		if ( (ai_bGLobal == M4_TRUE) && (ai_iInit == M4_VM_STATISTICS_DISABLED) && (ai_iInit != m_iSttInit) )
		{
			//Quita la referencia de las stt globales.
			m_iSttRef--;
		}
	}
	else
	{
		if ( (ai_bGLobal == M4_TRUE) && (ai_iInit != M4_VM_STATISTICS_DISABLED) )
		{
			//Añade la referencia de las stt globales.
			m_iSttRef++;
		}
	}

	m_bSttGlobal = ai_bGLobal;

	if (ai_iInit == M4_VM_STATISTICS_DISABLED)
		m_bSttGlobal = M4_FALSE;

	if (ai_iInit == m_iSttInit)
		return M4_SUCCESS;

	if (ai_iInit == M4_VM_STATISTICS_DISABLED)
	{
		if (m_iSttRef != 0)
		{
			m_pStatistics->sttSetInitState(ai_iInit);

			m_iSttInit = m_pStatistics->sttGetInitState();
		}
		else
		{
			if (M4_ERROR == DestroyStatistics())
				return M4_ERROR;
		}
	}
	else
	{
		if (m_pStatistics == NULL)
		{
			if (M4_ERROR == CreateStatistics(ai_pChannelManager))
			{
				m_bSttGlobal = M4_FALSE;
				return M4_ERROR;
			}
		}

		if (m_bPause == M4_TRUE)
		{
			m_pStatistics->sttSetInitState(M4_VM_STATISTICS_PAUSED);
			
			m_iSttInit = ai_iInit;
		}
		else
		{
			m_pStatistics->sttSetInitState(ai_iInit);

			m_iSttInit = m_pStatistics->sttGetInitState();
		}

	}

	return M4_SUCCESS;
}

m4return_t ClVMStatisticsWrapper::SetStatisticsLabel(m4pchar_t ai_pcLabel, m4bool_t ai_bGLobal)
{
	if (m_iSttInit != M4_VM_STATISTICS_DISABLED)
	{
		if ( (m_bSttGlobal == M4_TRUE) && (ai_bGLobal == M4_FALSE) )
		{
			//Nadie puede cambiar la label si no es el owner. (Nuevo parámetro por defecto false)
			return M4_SUCCESS;
		}

		return ( m_pStatistics->sttSetLabel(ai_pcLabel) );
	}
	else 
		return M4_ERROR;
}

m4pchar_t  ClVMStatisticsWrapper::GetStatisticsLabel(void)
{
	if (m_pStatistics != NULL)
	{
		return( m_pStatistics->sttGetLabel() );
	}

	return NULL;
}

//Pause
m4return_t ClVMStatisticsWrapper::SetStatisticsPause(m4bool_t ai_bPause)
{
	m4return_t ret = M4_SUCCESS;

	if (m_bPause == ai_bPause)
		return M4_SUCCESS;

	m_bPause = ai_bPause;

	if (m_pStatistics)
	{
		if (ai_bPause == M4_TRUE)
		{
			m_pStatistics->sttSetInitState(M4_VM_STATISTICS_PAUSED);
		}
		else
		{
			m_pStatistics->sttSetInitState(m_iSttInit);
		}
	}

	return M4_SUCCESS;
}


//Refs
void ClVMStatisticsWrapper::IncStatisticsRef(void)
{
	m_iSttRef++;
}

void ClVMStatisticsWrapper::DecStatisticsRef(void)
{
	if (m_iSttRef == 0)
	{
		//Debug
		//M4_ASSERT(0);
	}
	else
	{
		m_iSttRef--;

		if ((m_iSttRef <= 0) /*&& (m_bSttGlobal == M4_FALSE)*/)
		{
			DestroyStatistics();
		}
	}
}

m4return_t ClVMStatisticsWrapper::Serialize(ClGenericIODriver& IOD)
{
	m4return_t ret;

	ret = IOD.Write(m_iSttInit);

	if (m_iSttInit != M4_VM_STATISTICS_DISABLED)
	{
		/* Bug 0107283
		Sólo se mandan las estadísticas si están activadas
		*/
		ret = IOD.Write(m_bPause);
		ret = IOD.Write(m_bSttGlobal);
		
		ret = m_pStatistics->Serialize(IOD);
	}

	return ret;
}


m4return_t ClVMStatisticsWrapper::DeSerialize(ClGenericIODriver& IOD, ClChannelManager * ai_pChannelManager)
{
	m4return_t ret;
	m4uint16_t iSttInit;

	ret = IOD.Read(iSttInit);
	SetStatisticsLevel(iSttInit, ai_pChannelManager);

	if (iSttInit != M4_VM_STATISTICS_DISABLED)
	{
		/* Bug 0107283
		Sólo se mandan las estadísticas si están activadas
		*/
		ret = IOD.Read(m_bPause);
		ret = IOD.Read(m_bSttGlobal);

		ret = m_pStatistics->DeSerialize(IOD);
	}

	return ret;
}