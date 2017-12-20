//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:                sttwrap.hpp
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
//    Wrapper de estadisticas para el ejecutor.
////
//==============================================================================
// COMMON...


#ifndef _STTWRAP_HPP
#define _STTWRAP_HPP
#include "m4dm_dll.hpp"
#include "m4string.hpp"
#include "m4define.hpp"
#include "m4types.hpp"

#include "cldefine.hpp"
#include "cltypes.hpp"

#include "clgeniod.hpp"

class ClChannelManager;
class ClVMStatistics;

class M4_DECL_M4DM ClVMStatisticsWrapper{
public:
	ClVMStatisticsWrapper(m4bool_t ai_bIsClient);
	~ClVMStatisticsWrapper(void);

	m4return_t	CreateStatistics (ClChannelManager * ai_pChannelManager);
	m4return_t	DestroyStatistics (void);

	//Level
	m4return_t	SetStatisticsLevel (m4uint16_t ai_iInit, ClChannelManager * ai_pChannelManager, m4bool_t ai_bGLobal = M4_FALSE);
	m4uint16_t	GetStatisticsLevel (void)
	{
		return m_iSttInit;
	}
    
	//Label
    m4return_t SetStatisticsLabel(m4pchar_t ai_pcLabel, m4bool_t ai_bGLobal = M4_FALSE);
	m4pchar_t  GetStatisticsLabel(void);
	
	//Pause
	m4return_t SetStatisticsPause(m4bool_t ai_bPause);
	m4bool_t GetStatisticsPause(void)
	{
		return m_bPause;
	}

	//Objeto Statistics
	ClVMStatistics* GetStatistics (void)
	{
		return m_pStatistics;
	}
	
	//Referencias
	void IncStatisticsRef(void);
	void DecStatisticsRef(void);

	void SetStatistics (ClVMStatistics *ai_pStatistics, m4uint16_t ai_iSttInit)
	{
		m_pStatistics = ai_pStatistics;
		m_iSttInit =  ai_iSttInit;
	}

	m4return_t Serialize(ClGenericIODriver& IOD);
	m4return_t DeSerialize(ClGenericIODriver& IOD, ClChannelManager * ai_pChannelManager);

private:
	//Statistics
	ClVMStatistics		*m_pStatistics;
	m4uint16_t			m_iSttInit;
	m4bool_t			m_bSttGlobal;
	m4uint32_t			m_iSttRef;
	m4bool_t			m_bPause;
	m4bool_t			m_bIsClient;
};

#endif


