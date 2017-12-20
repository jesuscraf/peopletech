//#include "vmres.hpp"
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:           sttlib.cpp
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

#include "sttchn.hpp"
#include "dm.hpp"
#include "channel.hpp"
#include "access.hpp"
#include "level2.hpp"
#include "medvarin.hpp"
#include "nodedef.hpp"
#include "chan_knl.hpp"
#include "m4dmcommon.hpp"

m4return_t GetItemIndex(ClCompiledMCR *ai_poCMCR, m4uint32_t ai_iNode, m4pchar_t ai_pcNode, m4pchar_t ai_pcItem, ClItemIndex &ao_iItemIndex)
{
	m4uint32_t hItem;

	hItem = ai_poCMCR->GetNodeItemHandleById( ai_iNode, ai_pcItem, M4_TRUE ) ;

	if( hItem == 0 )
	{
		g_oChLog<<BeginError(M4_CH_DM_ITEM_NOT_FOUND)<<ai_pcItem<<M4ObjNodeN(ai_poCMCR,ai_iNode)<<"Statistics"<<EndLog;

		return M4_ERROR;
	}

	ao_iItemIndex = ai_poCMCR->GetItemIndex( hItem ) ;

	return M4_SUCCESS;
}


ClVMStatisticsChannel::ClVMStatisticsChannel(void)
{
	m_bInitialized = M4_FALSE ;
	m_bChannelOwner = M4_FALSE ;
	m_poInitialChannelManager = NULL ;
	m_poInitialChannel = NULL ;
	m_poStatAccess = NULL ;
}


ClVMStatisticsChannel::~ClVMStatisticsChannel(void)
{
	if (m_poStatAccess != NULL)
	{
		ClChannel * pChan = m_poStatAccess->GetpChannel();
		m_poStatAccess->Destroy();

		if( m_bChannelOwner == M4_TRUE )
		{
			pChan->Destroy();
		}

		m_poStatAccess = NULL;	
	}
}


m4return_t ClVMStatisticsChannel::sttChnInit( ClChannelManager* ai_poChannelManager )
{
	m_poInitialChannelManager = ai_poChannelManager ;
	return( M4_SUCCESS ) ;
}


m4return_t ClVMStatisticsChannel::sttChnInit( ClChannel* ai_poChannel )
{
	m_poInitialChannel = ai_poChannel ;
	return( M4_SUCCESS ) ;
}


ClAccess*	ClVMStatisticsChannel::_GetAccess(void)
{
	if( m_bInitialized == M4_FALSE )
	{
		m_bInitialized = M4_TRUE ;

		if( m_poInitialChannelManager != NULL )
		{
			_Init( m_poInitialChannelManager ) ;
		}
		else if( m_poInitialChannel != NULL )
		{
			_Init( m_poInitialChannel ) ;
		}

		m_poInitialChannelManager = NULL ;
		m_poInitialChannel = NULL ;
	}

	return( m_poStatAccess ) ;
}


m4return_t ClVMStatisticsChannel::_Init( ClChannelManager* ai_poInitialChannelManager )
{
	m4return_t res = M4_SUCCESS;
	ClChannel * pSttChan = NULL;

	ai_poInitialChannelManager->CreateChannel(pSttChan);

	res = pSttChan->BuildFromId(M4_CHANNEL_STATISTICS);

	if (M4_ERROR == res)
	{
		pSttChan->Destroy();
		return M4_ERROR;
	}

	m_poStatAccess = pSttChan->CreateAccess();

	m_bChannelOwner = M4_TRUE ;

	res = _Init();

	if (M4_ERROR == res)
	{
		m_poStatAccess->Destroy();
		pSttChan->Destroy();
		m_bChannelOwner = M4_FALSE;
		m_poStatAccess = NULL;
	}
	
	return res;
}


m4return_t ClVMStatisticsChannel::_Init( ClChannel* ai_poInitialChannel )
{
	m4return_t res = M4_SUCCESS;


	m_poStatAccess = ai_poInitialChannel->CreateAccess();

	res = _Init();

	if (M4_ERROR == res)
	{
		m_poStatAccess->Destroy();
		m_poStatAccess = NULL;
	}
	
	m_bChannelOwner = M4_FALSE ;

	return res;
}


//Inicializa los handles de nodos e items y obtiene el acceso al canal de estadisticas.
//Para ello necesita un puntero al access del canal desde el que tomamos las estadisticas (VMachine->m_oState1->m_poAccess)
//Solo hay que hacerlo una sola vez.
m4return_t ClVMStatisticsChannel::_Init(void)
{
	ClCompiledMCR   *poCMCR;
	m4return_t res = M4_SUCCESS;

	if (m_poStatAccess == NULL)
	{
		return M4_ERROR;
	}

	m_poStatAccess->GetpChannel()->GetpChannel_Data()->SetCheckPointMode(M4_FALSE);

	poCMCR = m_poStatAccess-> GetpCMCR() ;

	//Ya tenemos el access al canal STATISTICS


	//LABELS
  	//Obtenemos los handles del LABELS.
	m_iNodeLabels = poCMCR->GetChannelNodeHandleById( M4_NODE_SCHSTT_LABELS, M4_TRUE ) ;

	if( m_iNodeLabels == NULL )
	{
		g_oChLog<<BeginError(M4_CH_DM_NO_SUCH_NODE_STT)<<M4_NODE_SCHSTT_LABELS<<M4Obj(poCMCR)<<EndLog;
		return( M4_ERROR ) ;
	}

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeLabels, M4_NODE_SCHSTT_LABELS, M4_ITEM_SCHSTT_LABELS_LABEL, m_iItemLabelsLabel) )
		return M4_ERROR;
	
	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeLabels, M4_NODE_SCHSTT_LABELS, M4_ITEM_SCHSTT_LABELS_DATE, m_iItemLabelsDate) )
		return M4_ERROR;

	
	//ITEMS
	m_iNodeItems = poCMCR->GetChannelNodeHandleById( M4_NODE_SCHSTT_ITEMS, M4_TRUE ) ;

	if( m_iNodeItems == NULL )
	{
		g_oChLog<<BeginError(M4_CH_DM_NO_SUCH_NODE_STT)<<M4_NODE_SCHSTT_ITEMS<<M4Obj(poCMCR)<<EndLog;
		return( M4_ERROR ) ;
	}

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_ID_T3, m_iItemItemsIdT3) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_ID_NODE, m_iItemItemsIdNode) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_ID_ITEM, m_iItemItemsIdItem) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_EXECUTIONS, m_iItemItemsExecutions) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_TOTAL_TIME, m_iItemItemsTotalTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_DB_TIME, m_iItemItemsDBTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_COM_TIME, m_iItemItemsComTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_EXEC_TIME, m_iItemItemsExecTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_STT_TIME, m_iItemItemsSttTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_SELF_TOTAL_TIME, m_iItemItemsSelfTotalTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_SELF_DB_TIME, m_iItemItemsSelfDBTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_SELF_COM_TIME, m_iItemItemsSelfComTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_SELF_EXEC_TIME, m_iItemItemsSelfExecTime) )
		return M4_ERROR;
					
	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_AVG_TOTAL_TIME, m_iItemItemsAvgTotalTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_AVG_DB_TIME, m_iItemItemsAvgDBTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_AVG_COM_TIME, m_iItemItemsAvgComTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_AVG_EXEC_TIME, m_iItemItemsAvgExecTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_AVG_SELF_TOTAL_TIME, m_iItemItemsAvgSelfTotalTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_AVG_SELF_DB_TIME, m_iItemItemsAvgSelfDBTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_AVG_SELF_COM_TIME, m_iItemItemsAvgSelfComTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_AVG_SELF_EXEC_TIME, m_iItemItemsAvgSelfExecTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_LAST_TOTAL_TIME, m_iItemItemsLastTotalTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_LAST_DB_TIME, m_iItemItemsLastDBTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_LAST_COM_TIME, m_iItemItemsLastComTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_LAST_EXEC_TIME, m_iItemItemsLastExecTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_LAST_SELF_TOTAL_TIME, m_iItemItemsLastSelfTotalTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_LAST_SELF_DB_TIME, m_iItemItemsLastSelfDBTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_LAST_SELF_COM_TIME, m_iItemItemsLastSelfComTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_LAST_SELF_EXEC_TIME, m_iItemItemsLastSelfExecTime) )
		return M4_ERROR;

	//PDUS
	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_TOTAL_PDU_SIZE, m_iItemItemsTotalPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_OUTPUT_PDU_SIZE, m_iItemItemsOutputPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_INPUT_PDU_SIZE, m_iItemItemsInputPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_SELF_TOTAL_PDU_SIZE, m_iItemItemsSelfTotalPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_SELF_OUTPUT_PDU_SIZE, m_iItemItemsSelfOutputPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_SELF_INPUT_PDU_SIZE, m_iItemItemsSelfInputPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_AVG_TOTAL_PDU_SIZE, m_iItemItemsAvgTotalPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_AVG_OUTPUT_PDU_SIZE, m_iItemItemsAvgOutputPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_AVG_INPUT_PDU_SIZE, m_iItemItemsAvgInputPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_AVG_SELF_TOTAL_PDU_SIZE, m_iItemItemsAvgSelfTotalPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_AVG_SELF_OUTPUT_PDU_SIZE, m_iItemItemsAvgSelfOutputPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_AVG_SELF_INPUT_PDU_SIZE, m_iItemItemsAvgSelfInputPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_LAST_TOTAL_PDU_SIZE, m_iItemItemsLastTotalPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_LAST_OUTPUT_PDU_SIZE, m_iItemItemsLastOutputPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_LAST_INPUT_PDU_SIZE, m_iItemItemsLastInputPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_LAST_SELF_TOTAL_PDU_SIZE, m_iItemItemsLastSelfTotalPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_LAST_SELF_OUTPUT_PDU_SIZE, m_iItemItemsLastSelfOutputPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_LAST_SELF_INPUT_PDU_SIZE, m_iItemItemsLastSelfInputPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_IS_CACHEABLE, m_iItemIsCacheable) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_CACHE_HIT_PERC, m_iItemCacheHitPerc) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_CS_TYPE, m_iItemItemsCSType) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeItems, M4_NODE_SCHSTT_ITEMS, M4_ITEM_SCHSTT_ITEMS_DATE, m_iItemItemsDate) )
		return M4_ERROR;


	//TIMES
	m_iNodeTimes = poCMCR->GetChannelNodeHandleById( M4_NODE_SCHSTT_TIMES, M4_TRUE ) ;

	if( m_iNodeTimes == NULL )
	{
		g_oChLog<<BeginError(M4_CH_DM_NO_SUCH_NODE_STT)<<M4_NODE_SCHSTT_TIMES<<M4Obj(poCMCR)<<EndLog;		
		return( M4_ERROR ) ;
	}

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeTimes, M4_NODE_SCHSTT_TIMES, M4_ITEM_SCHSTT_TIMES_DATE, m_iItemTimesDate) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeTimes, M4_NODE_SCHSTT_TIMES, M4_ITEM_SCHSTT_TIMES_ID_RULE, m_iItemTimesIdRule) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeTimes, M4_NODE_SCHSTT_TIMES, M4_ITEM_SCHSTT_TIMES_NORM, m_iItemTimesNorm) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeTimes, M4_NODE_SCHSTT_TIMES, M4_ITEM_SCHSTT_TIMES_TOTAL_TIME, m_iItemTimesTotalTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeTimes, M4_NODE_SCHSTT_TIMES, M4_ITEM_SCHSTT_TIMES_DB_TIME, m_iItemTimesDBTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeTimes, M4_NODE_SCHSTT_TIMES, M4_ITEM_SCHSTT_TIMES_COM_TIME, m_iItemTimesComTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeTimes, M4_NODE_SCHSTT_TIMES, M4_ITEM_SCHSTT_TIMES_EXEC_TIME, m_iItemTimesExecTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeTimes, M4_NODE_SCHSTT_TIMES, M4_ITEM_SCHSTT_TIMES_STT_TIME, m_iItemTimesSttTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeTimes, M4_NODE_SCHSTT_TIMES, M4_ITEM_SCHSTT_TIMES_SELF_TOTAL_TIME, m_iItemTimesSelfTotalTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeTimes, M4_NODE_SCHSTT_TIMES, M4_ITEM_SCHSTT_TIMES_SELF_DB_TIME, m_iItemTimesSelfDBTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeTimes, M4_NODE_SCHSTT_TIMES, M4_ITEM_SCHSTT_TIMES_SELF_COM_TIME, m_iItemTimesSelfComTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeTimes, M4_NODE_SCHSTT_TIMES, M4_ITEM_SCHSTT_TIMES_SELF_EXEC_TIME, m_iItemTimesSelfExecTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeTimes, M4_NODE_SCHSTT_TIMES, M4_ITEM_SCHSTT_TIMES_TOTAL_PDU_SIZE, m_iItemTimesTotalPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeTimes, M4_NODE_SCHSTT_TIMES, M4_ITEM_SCHSTT_TIMES_OUTPUT_PDU_SIZE, m_iItemTimesOutputPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeTimes, M4_NODE_SCHSTT_TIMES, M4_ITEM_SCHSTT_TIMES_INPUT_PDU_SIZE, m_iItemTimesInputPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeTimes, M4_NODE_SCHSTT_TIMES, M4_ITEM_SCHSTT_TIMES_SELF_TOTAL_PDU_SIZE, m_iItemTimesSelfTotalPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeTimes, M4_NODE_SCHSTT_TIMES, M4_ITEM_SCHSTT_TIMES_SELF_OUTPUT_PDU_SIZE, m_iItemTimesSelfOutputPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeTimes, M4_NODE_SCHSTT_TIMES, M4_ITEM_SCHSTT_TIMES_SELF_INPUT_PDU_SIZE, m_iItemTimesSelfInputPDUSize) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeTimes, M4_NODE_SCHSTT_TIMES, M4_ITEM_SCHSTT_TIMES_GENERAL_DATA, m_iItemTimesGeneralData) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeTimes, M4_NODE_SCHSTT_TIMES, M4_ITEM_SCHSTT_TIMES_SERVICE, m_iItemTimesService) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeTimes, M4_NODE_SCHSTT_TIMES, M4_ITEM_SCHSTT_TIMES_CACHE_HIT, m_iItemTimesCacheHit) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeTimes, M4_NODE_SCHSTT_TIMES, M4_ITEM_SCHSTT_TIMES_REAL_CS_TYPE, m_iItemTimesRealCSType) )
		return M4_ERROR;

	//Obtenemos Parents
	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeTimes, M4_NODE_SCHSTT_TIMES, M4_ITEM_SCHSTT_TIMES_PARENT_ID_T3, m_iItemTimesParentIdT3) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeTimes, M4_NODE_SCHSTT_TIMES, M4_ITEM_SCHSTT_TIMES_PARENT_ID_NODE, m_iItemTimesParentIdNode) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeTimes, M4_NODE_SCHSTT_TIMES, M4_ITEM_SCHSTT_TIMES_PARENT_ID_ITEM, m_iItemTimesParentIdItem) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeTimes, M4_NODE_SCHSTT_TIMES, M4_ITEM_SCHSTT_TIMES_PARENT_ID_RULE, m_iItemTimesParentIdRule) )
		return M4_ERROR;

	
	//VALUES
	m_iNodeValues = poCMCR->GetChannelNodeHandleById( M4_NODE_SCHSTT_VALUES, M4_TRUE ) ;

	if( m_iNodeValues == NULL )
	{
		g_oChLog<<BeginError(M4_CH_DM_NO_SUCH_NODE_STT)<<M4_NODE_SCHSTT_VALUES<<M4Obj(poCMCR)<<EndLog;
		return( M4_ERROR ) ;
	}

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeValues, M4_NODE_SCHSTT_VALUES, M4_ITEM_SCHSTT_VALUES_DATE, m_iItemValuesDate) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeValues, M4_NODE_SCHSTT_VALUES, M4_ITEM_SCHSTT_VALUES_VALUE, m_iItemValuesValue) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeValues, M4_NODE_SCHSTT_VALUES, M4_ITEM_SCHSTT_VALUES_SLICE_START, m_iItemValuesSliceStart) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeValues, M4_NODE_SCHSTT_VALUES, M4_ITEM_SCHSTT_VALUES_SLICE_END, m_iItemValuesSliceEnd) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeValues, M4_NODE_SCHSTT_VALUES, M4_ITEM_SCHSTT_VALUES_PRIORITY, m_iItemValuesPriority) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeValues, M4_NODE_SCHSTT_VALUES, M4_ITEM_SCHSTT_VALUES_ASSIGNED, m_iItemValuesAssigned) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeValues, M4_NODE_SCHSTT_VALUES, M4_ITEM_SCHSTT_VALUES_SOURCE, m_iItemValuesSource) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeValues, M4_NODE_SCHSTT_VALUES, M4_ITEM_SCHSTT_VALUES_ITEM_TYPE, m_iItemValuesItemType) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeValues, M4_NODE_SCHSTT_VALUES, M4_ITEM_SCHSTT_VALUES_ID_EXEC_T3, m_iItemValuesIdExecT3) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeValues, M4_NODE_SCHSTT_VALUES, M4_ITEM_SCHSTT_VALUES_ID_EXEC_NODE, m_iItemValuesIdExecNode) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeValues, M4_NODE_SCHSTT_VALUES, M4_ITEM_SCHSTT_VALUES_ID_EXEC_ITEM, m_iItemValuesIdExecItem) )
		return M4_ERROR;
	
	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeValues, M4_NODE_SCHSTT_VALUES, M4_ITEM_SCHSTT_VALUES_ID_EXEC_RULE, m_iItemValuesIdExecRule) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeValues, M4_NODE_SCHSTT_VALUES, M4_ITEM_SCHSTT_VALUES_EXEC_NORM, m_iItemValuesExecNorm) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeValues, M4_NODE_SCHSTT_VALUES, M4_ITEM_SCHSTT_VALUES_EXEC_LINE, m_iItemValuesExecLine) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeValues, M4_NODE_SCHSTT_VALUES, M4_ITEM_SCHSTT_VALUES_CALL_STACK, m_iItemValuesCallStack) )
		return M4_ERROR;


	//ASSINGS
	m_iNodeAssigns = poCMCR->GetChannelNodeHandleById( M4_NODE_SCHSTT_ASSIGNS, M4_TRUE ) ;

	if( m_iNodeAssigns == NULL )
	{
		g_oChLog<<BeginError(M4_CH_DM_NO_SUCH_NODE_STT)<<M4_NODE_SCHSTT_ASSIGNS<<M4Obj(poCMCR)<<EndLog;
		return( M4_ERROR ) ;
	}

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeAssigns, M4_NODE_SCHSTT_ASSIGNS, M4_ITEM_SCHSTT_ASSIGNS_DATE, m_iItemAssignsDate) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeAssigns, M4_NODE_SCHSTT_ASSIGNS, M4_ITEM_SCHSTT_ASSIGNS_VALUE, m_iItemAssignsValue) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeAssigns, M4_NODE_SCHSTT_ASSIGNS, M4_ITEM_SCHSTT_ASSIGNS_SLICE_START, m_iItemAssignsSliceStart) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeAssigns, M4_NODE_SCHSTT_ASSIGNS, M4_ITEM_SCHSTT_ASSIGNS_SLICE_END, m_iItemAssignsSliceEnd) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeAssigns, M4_NODE_SCHSTT_ASSIGNS, M4_ITEM_SCHSTT_ASSIGNS_PRIORITY, m_iItemAssignsPriority) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeAssigns, M4_NODE_SCHSTT_ASSIGNS, M4_ITEM_SCHSTT_ASSIGNS_ASSIGNED, m_iItemAssignsAssigned) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeAssigns, M4_NODE_SCHSTT_ASSIGNS, M4_ITEM_SCHSTT_ASSIGNS_SOURCE, m_iItemAssignsSource) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeAssigns, M4_NODE_SCHSTT_ASSIGNS, M4_ITEM_SCHSTT_ASSIGNS_ITEM_TYPE, m_iItemAssignsItemType) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeAssigns, M4_NODE_SCHSTT_ASSIGNS, M4_ITEM_SCHSTT_ASSIGNS_ID_T3, m_iItemAssignsIdT3) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeAssigns, M4_NODE_SCHSTT_ASSIGNS, M4_ITEM_SCHSTT_ASSIGNS_ID_NODE, m_iItemAssignsIdNode) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeAssigns, M4_NODE_SCHSTT_ASSIGNS, M4_ITEM_SCHSTT_ASSIGNS_ID_ITEM, m_iItemAssignsIdItem) )
		return M4_ERROR;
	
	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeAssigns, M4_NODE_SCHSTT_ASSIGNS, M4_ITEM_SCHSTT_ASSIGNS_ID_EXEC_RULE, m_iItemAssignsIdExecRule) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeAssigns, M4_NODE_SCHSTT_ASSIGNS, M4_ITEM_SCHSTT_ASSIGNS_EXEC_NORM, m_iItemAssignsExecNorm) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeAssigns, M4_NODE_SCHSTT_ASSIGNS, M4_ITEM_SCHSTT_ASSIGNS_EXEC_LINE, m_iItemAssignsExecLine) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeAssigns, M4_NODE_SCHSTT_ASSIGNS, M4_ITEM_SCHSTT_ASSIGNS_CALL_STACK, m_iItemAssignsCallStack) )
		return M4_ERROR;


	//READS
	m_iNodeReads = poCMCR->GetChannelNodeHandleById( M4_NODE_SCHSTT_READS, M4_TRUE ) ;

	if( m_iNodeReads == NULL )
	{
		g_oChLog<<BeginError(M4_CH_DM_NO_SUCH_NODE_STT)<<M4_NODE_SCHSTT_READS<<M4Obj(poCMCR)<<EndLog;
		return( M4_ERROR ) ;
	}

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeReads, M4_NODE_SCHSTT_READS, M4_ITEM_SCHSTT_READS_DATE, m_iItemReadsDate) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeReads, M4_NODE_SCHSTT_READS, M4_ITEM_SCHSTT_READS_VALUE, m_iItemReadsValue) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeReads, M4_NODE_SCHSTT_READS, M4_ITEM_SCHSTT_READS_SLICE_START, m_iItemReadsSliceStart) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeReads, M4_NODE_SCHSTT_READS, M4_ITEM_SCHSTT_READS_SLICE_END, m_iItemReadsSliceEnd) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeReads, M4_NODE_SCHSTT_READS, M4_ITEM_SCHSTT_READS_ITEM_TYPE, m_iItemReadsItemType) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeReads, M4_NODE_SCHSTT_READS, M4_ITEM_SCHSTT_READS_ID_EXEC_T3, m_iItemReadsIdExecT3) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeReads, M4_NODE_SCHSTT_READS, M4_ITEM_SCHSTT_READS_ID_EXEC_NODE, m_iItemReadsIdExecNode) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeReads, M4_NODE_SCHSTT_READS, M4_ITEM_SCHSTT_READS_ID_EXEC_ITEM, m_iItemReadsIdExecItem) )
		return M4_ERROR;
	
	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeReads, M4_NODE_SCHSTT_READS, M4_ITEM_SCHSTT_READS_ID_EXEC_RULE, m_iItemReadsIdExecRule) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeReads, M4_NODE_SCHSTT_READS, M4_ITEM_SCHSTT_READS_EXEC_NORM, m_iItemReadsExecNorm) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeReads, M4_NODE_SCHSTT_READS, M4_ITEM_SCHSTT_READS_EXEC_LINE, m_iItemReadsExecLine) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeReads, M4_NODE_SCHSTT_READS, M4_ITEM_SCHSTT_READS_CALL_STACK, m_iItemReadsCallStack) )
		return M4_ERROR;


	//USAGES
	m_iNodeUsages = poCMCR->GetChannelNodeHandleById( M4_NODE_SCHSTT_USAGES, M4_TRUE ) ;

	if( m_iNodeUsages == NULL )
	{
		g_oChLog<<BeginError(M4_CH_DM_NO_SUCH_NODE_STT)<<M4_NODE_SCHSTT_USAGES<<M4Obj(poCMCR)<<EndLog;
		return( M4_ERROR ) ;
	}

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeUsages, M4_NODE_SCHSTT_USAGES, M4_ITEM_SCHSTT_USAGES_DATE, m_iItemUsagesDate) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeUsages, M4_NODE_SCHSTT_USAGES, M4_ITEM_SCHSTT_USAGES_VALUE, m_iItemUsagesValue) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeUsages, M4_NODE_SCHSTT_USAGES, M4_ITEM_SCHSTT_USAGES_SLICE_START, m_iItemUsagesSliceStart) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeUsages, M4_NODE_SCHSTT_USAGES, M4_ITEM_SCHSTT_USAGES_SLICE_END, m_iItemUsagesSliceEnd) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeUsages, M4_NODE_SCHSTT_USAGES, M4_ITEM_SCHSTT_USAGES_ITEM_TYPE, m_iItemUsagesItemType) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeUsages, M4_NODE_SCHSTT_USAGES, M4_ITEM_SCHSTT_USAGES_ID_T3, m_iItemUsagesIdT3) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeUsages, M4_NODE_SCHSTT_USAGES, M4_ITEM_SCHSTT_USAGES_ID_NODE, m_iItemUsagesIdNode) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeUsages, M4_NODE_SCHSTT_USAGES, M4_ITEM_SCHSTT_USAGES_ID_ITEM, m_iItemUsagesIdItem) )
		return M4_ERROR;
	
	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeUsages, M4_NODE_SCHSTT_USAGES, M4_ITEM_SCHSTT_USAGES_ID_EXEC_RULE, m_iItemUsagesIdExecRule) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeUsages, M4_NODE_SCHSTT_USAGES, M4_ITEM_SCHSTT_USAGES_EXEC_NORM, m_iItemUsagesExecNorm) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeUsages, M4_NODE_SCHSTT_USAGES, M4_ITEM_SCHSTT_USAGES_EXEC_LINE, m_iItemUsagesExecLine) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeUsages, M4_NODE_SCHSTT_USAGES, M4_ITEM_SCHSTT_USAGES_CALL_STACK, m_iItemUsagesCallStack) )
		return M4_ERROR;


	//DBTIMES
	m_iNodeDBTimes = poCMCR->GetChannelNodeHandleById( M4_NODE_SCHSTT_DBTIMES, M4_TRUE ) ;

	if( m_iNodeDBTimes == NULL )
	{
		g_oChLog<<BeginError(M4_CH_DM_NO_SUCH_NODE_STT)<<M4_NODE_SCHSTT_DBTIMES<<M4Obj(poCMCR)<<EndLog;
		return( M4_ERROR ) ;
	}

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeDBTimes, M4_NODE_SCHSTT_DBTIMES, M4_ITEM_SCHSTT_DBTIMES_OPERATION, m_iItemDBTimesOperation) )
		return M4_ERROR;
	
	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeDBTimes, M4_NODE_SCHSTT_DBTIMES, M4_ITEM_SCHSTT_DBTIMES_IS_INTERNAL, m_iItemDBTimesIsInternal) )
		return M4_ERROR;
	
	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeDBTimes, M4_NODE_SCHSTT_DBTIMES, M4_ITEM_SCHSTT_DBTIMES_STATEMENT, m_iItemDBTimesStatement) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeDBTimes, M4_NODE_SCHSTT_DBTIMES, M4_ITEM_SCHSTT_DBTIMES_PREPARE_TIME, m_iItemDBTimesPrepareTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeDBTimes, M4_NODE_SCHSTT_DBTIMES, M4_ITEM_SCHSTT_DBTIMES_EXECUTE_TIME, m_iItemDBTimesExecuteTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeDBTimes, M4_NODE_SCHSTT_DBTIMES, M4_ITEM_SCHSTT_DBTIMES_FETCH_TIME, m_iItemDBTimesFetchTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeDBTimes, M4_NODE_SCHSTT_DBTIMES, M4_ITEM_SCHSTT_DBTIMES_NUM_DATA, m_iItemDBTimesNumData) )
		return M4_ERROR;


	//SQL
	m_iNodeSQL = poCMCR->GetChannelNodeHandleById( M4_NODE_SCHSTT_SQL, M4_TRUE ) ;

	if( m_iNodeSQL == NULL )
	{
		g_oChLog<<BeginError(M4_CH_DM_NO_SUCH_NODE_STT)<<M4_NODE_SCHSTT_SQL<<M4Obj(poCMCR)<<EndLog;
		return( M4_ERROR ) ;
	}

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeSQL, M4_NODE_SCHSTT_SQL, M4_ITEM_SCHSTT_SQL_STATEMENT, m_iItemSQLStatement) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeSQL, M4_NODE_SCHSTT_SQL, M4_ITEM_SCHSTT_SQL_STM_LENGTH, m_iItemSQLStmLength) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeSQL, M4_NODE_SCHSTT_SQL, M4_ITEM_SCHSTT_SQL_OPERATION, m_iItemSQLOperation) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeSQL, M4_NODE_SCHSTT_SQL, M4_ITEM_SCHSTT_SQL_IS_INTERNAL, m_iItemSQLIsInternal) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeSQL, M4_NODE_SCHSTT_SQL, M4_ITEM_SCHSTT_SQL_NUM_PREPARE, m_iItemSQLNumPrepare) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeSQL, M4_NODE_SCHSTT_SQL, M4_ITEM_SCHSTT_SQL_NUM_EXECUTE, m_iItemSQLNumExecute) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeSQL, M4_NODE_SCHSTT_SQL, M4_ITEM_SCHSTT_SQL_NUM_FETCH, m_iItemSQLNumFetch) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeSQL, M4_NODE_SCHSTT_SQL, M4_ITEM_SCHSTT_SQL_TOTAL_TIME, m_iItemSQLTotalTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeSQL, M4_NODE_SCHSTT_SQL, M4_ITEM_SCHSTT_SQL_PREPARE_TIME, m_iItemSQLPrepareTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeSQL, M4_NODE_SCHSTT_SQL, M4_ITEM_SCHSTT_SQL_EXECUTE_TIME, m_iItemSQLExecuteTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeSQL, M4_NODE_SCHSTT_SQL, M4_ITEM_SCHSTT_SQL_FETCH_TIME, m_iItemSQLFetchTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeSQL, M4_NODE_SCHSTT_SQL, M4_ITEM_SCHSTT_SQL_AVG_TOTAL_TIME, m_iItemSQLAvgTotalTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeSQL, M4_NODE_SCHSTT_SQL, M4_ITEM_SCHSTT_SQL_AVG_PREPARE_TIME, m_iItemSQLAvgPrepareTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeSQL, M4_NODE_SCHSTT_SQL, M4_ITEM_SCHSTT_SQL_AVG_EXECUTE_TIME, m_iItemSQLAvgExecuteTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeSQL, M4_NODE_SCHSTT_SQL, M4_ITEM_SCHSTT_SQL_AVG_FETCH_TIME, m_iItemSQLAvgFetchTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeSQL, M4_NODE_SCHSTT_SQL, M4_ITEM_SCHSTT_SQL_LAST_TOTAL_TIME, m_iItemSQLLastTotalTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeSQL, M4_NODE_SCHSTT_SQL, M4_ITEM_SCHSTT_SQL_LAST_PREPARE_TIME, m_iItemSQLLastPrepareTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeSQL, M4_NODE_SCHSTT_SQL, M4_ITEM_SCHSTT_SQL_LAST_EXECUTE_TIME, m_iItemSQLLastExecuteTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeSQL, M4_NODE_SCHSTT_SQL, M4_ITEM_SCHSTT_SQL_LAST_FETCH_TIME, m_iItemSQLLastFetchTime) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeSQL, M4_NODE_SCHSTT_SQL, M4_ITEM_SCHSTT_SQL_DATE, m_iItemSQLDate) )
		return M4_ERROR;


	//HISTORY
	m_iNodeHistory = poCMCR->GetChannelNodeHandleById( M4_NODE_SCHSTT_HISTORY, M4_TRUE ) ;

	if( m_iNodeHistory == NULL )
	{
		g_oChLog<<BeginError(M4_CH_DM_NO_SUCH_NODE_STT)<<M4_NODE_SCHSTT_HISTORY<<M4Obj(poCMCR)<<EndLog;
		return( M4_ERROR ) ;
	}

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeHistory, M4_NODE_SCHSTT_HISTORY, M4_ITEM_SCHSTT_HISTORY_ID_T3, m_iItemHistoryIdT3) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeHistory, M4_NODE_SCHSTT_HISTORY, M4_ITEM_SCHSTT_HISTORY_ID_NODE, m_iItemHistoryIdNode) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeHistory, M4_NODE_SCHSTT_HISTORY, M4_ITEM_SCHSTT_HISTORY_ID_ITEM, m_iItemHistoryIdItem) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeHistory, M4_NODE_SCHSTT_HISTORY, M4_ITEM_SCHSTT_HISTORY_ID_RULE, m_iItemHistoryIdRule) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeHistory, M4_NODE_SCHSTT_HISTORY, M4_ITEM_SCHSTT_HISTORY_DATE, m_iItemHistoryDate) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeHistory, M4_NODE_SCHSTT_HISTORY, M4_ITEM_SCHSTT_HISTORY_REAL_CS_TYPE, m_iItemHistoryRealCSType) )
		return M4_ERROR;

	//Obtenemos Parents
	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeHistory, M4_NODE_SCHSTT_HISTORY, M4_ITEM_SCHSTT_HISTORY_PARENT_ID_T3, m_iItemHistoryParentIdT3) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeHistory, M4_NODE_SCHSTT_HISTORY, M4_ITEM_SCHSTT_HISTORY_PARENT_ID_NODE, m_iItemHistoryParentIdNode) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeHistory, M4_NODE_SCHSTT_HISTORY, M4_ITEM_SCHSTT_HISTORY_PARENT_ID_ITEM, m_iItemHistoryParentIdItem) )
		return M4_ERROR;

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeHistory, M4_NODE_SCHSTT_HISTORY, M4_ITEM_SCHSTT_HISTORY_PARENT_ID_RULE, m_iItemHistoryParentIdRule) )
		return M4_ERROR;

	//HISTORY PARAMS
	m_iNodeHistoryParams = poCMCR->GetChannelNodeHandleById( M4_NODE_SCHSTT_HISTORY_PARAMS, M4_TRUE ) ;

	if( m_iNodeHistoryParams == NULL )
	{
		g_oChLog<<BeginError(M4_CH_DM_NO_SUCH_NODE_STT)<<M4_NODE_SCHSTT_HISTORY_PARAMS<<M4Obj(poCMCR)<<EndLog;
		return( M4_ERROR ) ;
	}

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeHistoryParams, M4_NODE_SCHSTT_HISTORY_PARAMS, M4_ITEM_SCHSTT_HISTORY_PARAMS_PARAM, m_iItemHistoryParamsParam) )
		return M4_ERROR;

	//TIMES PARAMS
	m_iNodeTimesParams = poCMCR->GetChannelNodeHandleById( M4_NODE_SCHSTT_TIMES_PARAMS, M4_TRUE ) ;

	if( m_iNodeTimesParams == NULL )
	{
		g_oChLog<<BeginError(M4_CH_DM_NO_SUCH_NODE_STT)<<M4_NODE_SCHSTT_TIMES_PARAMS<<M4Obj(poCMCR)<<EndLog;
		return( M4_ERROR ) ;
	}

	if ( M4_ERROR == GetItemIndex(poCMCR, m_iNodeTimesParams, M4_NODE_SCHSTT_TIMES_PARAMS, M4_ITEM_SCHSTT_TIMES_PARAMS_PARAM, m_iItemTimesParamsParam) )
		return M4_ERROR;


	////
	if (m_poStatAccess->Node[ (ClHandle)m_iNodeItems ].RecordSet.Register.Filter.Count() == 0)
	{
		//Añadimos un filtro de ordenación en el iterador Register del node ITEMS
		m4pchar_t apcNames1[3] = { M4_ITEM_SCHSTT_ITEMS_ID_ITEM, 
								   M4_ITEM_SCHSTT_ITEMS_ID_NODE, 
								   M4_ITEM_SCHSTT_ITEMS_ID_T3 };
		m4uint8_t aiOrder1[3] =	{ M4CL_INDEX_SEARCH_WAY_ASC, 
								  M4CL_INDEX_SEARCH_WAY_ASC, 
								  M4CL_INDEX_SEARCH_WAY_ASC };

		if ( M4_ERROR == m_poStatAccess->Node[ (ClHandle)m_iNodeItems ].RecordSet.Register.Filter.AddSort ( 3, apcNames1, aiOrder1) )
			return M4_ERROR;		
	}

	if (m_poStatAccess->Node[ (ClHandle)m_iNodeSQL ].RecordSet.Register.Filter.Count() == 0)
	{
		//Añadimos un filtro de ordenación en el iterador Register del node ITEMS
		m4pchar_t apcNames2[2] = { M4_ITEM_SCHSTT_SQL_STM_LENGTH, 
								  M4_ITEM_SCHSTT_SQL_STATEMENT };
		m4uint8_t aiOrder2[2] =	{ M4CL_INDEX_SEARCH_WAY_ASC, 
								  M4CL_INDEX_SEARCH_WAY_ASC };

		if ( M4_ERROR == m_poStatAccess->Node[ (ClHandle)m_iNodeSQL ].RecordSet.Register.Filter.AddSort ( 2, apcNames2, aiOrder2) )
			return M4_ERROR;		
	}

	return M4_SUCCESS;

}


ClChannel*	ClVMStatisticsChannel::sttGetChannel (void)
{
	ClAccess*	poAccess = _GetAccess() ;

	if( poAccess == NULL )
	{
		return( NULL ) ;
	}

	return poAccess->GetpChannel();
}


m4return_t ClVMStatisticsChannel::sttAddTime (ClSttState * ai_pState, m4pchar_t ai_ParentIdT3, m4pchar_t ai_ParentIdNode, m4pchar_t ai_ParentIdItem, m4pchar_t ai_ParentIdRule)
{
	ClNode * poNodeTimes;
	_ClRegister_Base * pRegTemp;
	m4return_t res = M4_SUCCESS;
	m4VariantType vVariant;


	ClAccess*	poAccess = _GetAccess() ;

	if( poAccess == NULL )
	{
		return( M4_ERROR ) ;
	}

	//TIMES
	//Nos posicionamos en el node Times
	poNodeTimes = &(poAccess->Node[ (ClHandle)m_iNodeTimes ]);
	poNodeTimes->RecordSet.Current.Add();
	pRegTemp = (_ClRegister_Base*)&(poNodeTimes->RecordSet.Current);

	pRegTemp->Item[m_iItemTimesDate].Value.Set(ai_pState->m_vDate);

	pRegTemp->Item[m_iItemTimesIdRule].Value.Set(ai_pState->m_vIdRule);

	vVariant.SetDate(ai_pState->m_dNorm);
	pRegTemp->Item[m_iItemTimesNorm].Value.Set(vVariant);

	vVariant.SetStringRef(ai_ParentIdT3);
	pRegTemp->Item[m_iItemTimesParentIdT3].Value.Set(vVariant);

	vVariant.SetStringRef(ai_ParentIdNode);
	pRegTemp->Item[m_iItemTimesParentIdNode].Value.Set(vVariant);

	vVariant.SetStringRef(ai_ParentIdItem);
	pRegTemp->Item[m_iItemTimesParentIdItem].Value.Set(vVariant);

	vVariant.SetStringRef(ai_ParentIdRule);
	pRegTemp->Item[m_iItemTimesParentIdRule].Value.Set(vVariant);

	vVariant.SetDouble(ai_pState->m_uiChannelCSType);
	pRegTemp->Item[m_iItemTimesRealCSType].Value.Set(vVariant);

	res = sttFillTimeTimes(ai_pState->m_oExecData, *pRegTemp);

	if (M4_ERROR == res)
		return res;

	res = sttFillTimePDUs(ai_pState->m_oExecData, *pRegTemp);

	return res;
}


m4return_t ClVMStatisticsChannel::sttAddValue (ClSttValues *ai_pValue, m4pchar_t ai_IdExecT3, m4pchar_t ai_IdExecNode, m4pchar_t ai_IdExecItem, m4pchar_t ai_IdExecRule, m4uint8_t ai_uiExecCSType)
{
	ClNode * poNodeValues;
	ClNode * poNodeAssigns;
	_ClRegister_Base * pRegTemp;
	m4return_t res = M4_SUCCESS;
	m4VariantType vVariant;


	ClAccess*	poAccess = _GetAccess() ;

	if( poAccess == NULL )
	{
		return( M4_ERROR ) ;
	}

	res = sttAddItem( ai_pValue->m_vIdT3, ai_pValue->m_vIdNode, ai_pValue->m_vIdItem, ai_pValue->m_vDate, ai_pValue->m_iCSType, NULL );

	if (res == M4_ERROR)
		return M4_ERROR;

	//VALUES
	//Nos posicionamos en el bloque hijo (en NODE) del iRegister
	poNodeValues = &(poAccess->Node[ (ClHandle)m_iNodeValues ]);
	poNodeValues->RecordSet.Current.Add();
	pRegTemp = (_ClRegister_Base*)&(poNodeValues->RecordSet.Current);

	pRegTemp->Item[m_iItemValuesDate].Value.Set(ai_pValue->m_vDate);

	if (ai_pValue->m_vValue.Type == M4CL_CPP_TYPE_STRING_VAR)
	{
		pRegTemp->Item[m_iItemValuesValue].Value.Set(ai_pValue->m_vValue);
	}
	else
	{
		m4VariantType vAux(ai_pValue->m_vValue);
		ClVariantConversor oVariantMediator;
        res = oVariantMediator.ToCharString (vAux);

		pRegTemp->Item[m_iItemValuesValue].Value.Set(vAux);
	}

	if (ai_pValue->m_dSliceStart > 0)
	{
		vVariant.SetDate(ai_pValue->m_dSliceStart);
		pRegTemp->Item[m_iItemValuesSliceStart].Value.Set(vVariant);
	}

	if (ai_pValue->m_dSliceEnd > 0)
	{
		vVariant.SetDate(ai_pValue->m_dSliceEnd);
		pRegTemp->Item[m_iItemValuesSliceEnd].Value.Set(vVariant);
	}

	vVariant.SetDouble(ai_pValue->m_iPriority);
	pRegTemp->Item[m_iItemValuesPriority].Value.Set(vVariant);

	vVariant.SetDouble(ai_pValue->m_bAssigned);
	pRegTemp->Item[m_iItemValuesAssigned].Value.Set(vVariant);

	vVariant.SetDouble(ai_pValue->m_iSource);
	pRegTemp->Item[m_iItemValuesSource].Value.Set(vVariant);

	vVariant.SetDouble(ai_pValue->m_iM4Type);
	pRegTemp->Item[m_iItemValuesItemType].Value.Set(vVariant);

	vVariant.SetStringRef(ai_IdExecT3);
	pRegTemp->Item[m_iItemValuesIdExecT3].Value.Set(vVariant);

	vVariant.SetStringRef(ai_IdExecNode);
	pRegTemp->Item[m_iItemValuesIdExecNode].Value.Set(vVariant);

	vVariant.SetStringRef(ai_IdExecItem);
	pRegTemp->Item[m_iItemValuesIdExecItem].Value.Set(vVariant);

	vVariant.SetStringRef(ai_IdExecRule);
	pRegTemp->Item[m_iItemValuesIdExecRule].Value.Set(vVariant);

	vVariant.SetDate(ai_pValue->m_dNorm);
	pRegTemp->Item[m_iItemValuesExecNorm].Value.Set(vVariant);

	vVariant.SetDouble(ai_pValue->m_iLine);
	pRegTemp->Item[m_iItemValuesExecLine].Value.Set(vVariant);

	pRegTemp->Item[m_iItemValuesCallStack].Value.Set(ai_pValue->m_vCallStack);


	res = sttAddItem( ai_IdExecT3, ai_IdExecNode, ai_IdExecItem, ai_pValue->m_vDate, ai_uiExecCSType, NULL );

	if (res == M4_ERROR)
		return M4_ERROR;

	//ASSIGNS
	//Nos posicionamos en el bloque hijo (en NODE) del iRegister
	poNodeAssigns = &(poAccess->Node[ (ClHandle)m_iNodeAssigns ]);
	poNodeAssigns->RecordSet.Current.Add();
	pRegTemp = (_ClRegister_Base*)&(poNodeAssigns->RecordSet.Current);

	pRegTemp->Item[m_iItemAssignsDate].Value.Set(ai_pValue->m_vDate);

	if (ai_pValue->m_vValue.Type == M4CL_CPP_TYPE_STRING_VAR)
	{
		pRegTemp->Item[m_iItemAssignsValue].Value.Set(ai_pValue->m_vValue);
	}
	else
	{
		m4VariantType vAux(ai_pValue->m_vValue);
		ClVariantConversor oVariantMediator;
        res = oVariantMediator.ToCharString (vAux);

		pRegTemp->Item[m_iItemAssignsValue].Value.Set(vAux);
	}

	if (ai_pValue->m_dSliceStart > 0)
	{
		vVariant.SetDate(ai_pValue->m_dSliceStart);
		pRegTemp->Item[m_iItemAssignsSliceStart].Value.Set(vVariant);
	}

	if (ai_pValue->m_dSliceEnd > 0)
	{
		vVariant.SetDate(ai_pValue->m_dSliceEnd);
		pRegTemp->Item[m_iItemAssignsSliceEnd].Value.Set(vVariant);
	}

	vVariant.SetDouble(ai_pValue->m_iPriority);
	pRegTemp->Item[m_iItemAssignsPriority].Value.Set(vVariant);

	vVariant.SetDouble(ai_pValue->m_bAssigned);
	pRegTemp->Item[m_iItemAssignsAssigned].Value.Set(vVariant);

	vVariant.SetDouble(ai_pValue->m_iSource);
	pRegTemp->Item[m_iItemAssignsSource].Value.Set(vVariant);

	vVariant.SetDouble(ai_pValue->m_iM4Type);
	pRegTemp->Item[m_iItemAssignsItemType].Value.Set(vVariant);

	pRegTemp->Item[m_iItemAssignsIdT3].Value.Set(ai_pValue->m_vIdT3);
	pRegTemp->Item[m_iItemAssignsIdNode].Value.Set(ai_pValue->m_vIdNode);
	pRegTemp->Item[m_iItemAssignsIdItem].Value.Set(ai_pValue->m_vIdItem);

	vVariant.SetStringRef(ai_IdExecRule);
	pRegTemp->Item[m_iItemAssignsIdExecRule].Value.Set(vVariant);

	vVariant.SetDate(ai_pValue->m_dNorm);
	pRegTemp->Item[m_iItemAssignsExecNorm].Value.Set(vVariant);

	vVariant.SetDouble(ai_pValue->m_iLine);
	pRegTemp->Item[m_iItemAssignsExecLine].Value.Set(vVariant);

	pRegTemp->Item[m_iItemAssignsCallStack].Value.Set(ai_pValue->m_vCallStack);

	return M4_SUCCESS;
}


m4return_t ClVMStatisticsChannel::sttAddRead (ClSttReads *ai_pRead, m4pchar_t ai_IdExecT3, m4pchar_t ai_IdExecNode, m4pchar_t ai_IdExecItem, m4pchar_t ai_IdExecRule, m4uint8_t ai_uiExecCSType)
{
	ClNode * poNodeReads;
	ClNode * poNodeUsages;
	_ClRegister_Base * pRegTemp;
	m4return_t res = M4_SUCCESS;
	m4VariantType vVariant;


	ClAccess*	poAccess = _GetAccess() ;

	if( poAccess == NULL )
	{
		return( M4_ERROR ) ;
	}

	res = sttAddItem( ai_pRead->m_vIdT3, ai_pRead->m_vIdNode, ai_pRead->m_vIdItem, ai_pRead->m_vDate, ai_pRead->m_iCSType, NULL );

	if (res == M4_ERROR)
		return M4_ERROR;

	//READS
	//Nos posicionamos en el bloque hijo (en NODE) del iRegister
	poNodeReads = &(poAccess->Node[ (ClHandle)m_iNodeReads ]);
	poNodeReads->RecordSet.Current.Add();
	pRegTemp = (_ClRegister_Base*)&(poNodeReads->RecordSet.Current);

	pRegTemp->Item[m_iItemReadsDate].Value.Set(ai_pRead->m_vDate);

	if (ai_pRead->m_vValue.Type == M4CL_CPP_TYPE_STRING_VAR)
	{
		pRegTemp->Item[m_iItemReadsValue].Value.Set(ai_pRead->m_vValue);
	}
	else
	{
		m4VariantType vAux(ai_pRead->m_vValue);
		ClVariantConversor oVariantMediator;
        res = oVariantMediator.ToCharString (vAux);

		pRegTemp->Item[m_iItemReadsValue].Value.Set(vAux);
	}

	if (ai_pRead->m_dSliceStart > 0)
	{
		vVariant.SetDate(ai_pRead->m_dSliceStart);
		pRegTemp->Item[m_iItemReadsSliceStart].Value.Set(vVariant);
	}

	if (ai_pRead->m_dSliceEnd > 0)
	{
		vVariant.SetDate(ai_pRead->m_dSliceEnd);
		pRegTemp->Item[m_iItemReadsSliceEnd].Value.Set(vVariant);
	}

	vVariant.SetDouble(ai_pRead->m_iM4Type);
	pRegTemp->Item[m_iItemReadsItemType].Value.Set(vVariant);

	vVariant.SetStringRef(ai_IdExecT3);
	pRegTemp->Item[m_iItemReadsIdExecT3].Value.Set(vVariant);

	vVariant.SetStringRef(ai_IdExecNode);
	pRegTemp->Item[m_iItemReadsIdExecNode].Value.Set(vVariant);

	vVariant.SetStringRef(ai_IdExecItem);
	pRegTemp->Item[m_iItemReadsIdExecItem].Value.Set(vVariant);

	vVariant.SetStringRef(ai_IdExecRule);
	pRegTemp->Item[m_iItemReadsIdExecRule].Value.Set(vVariant);

	vVariant.SetDate(ai_pRead->m_dNorm);
	pRegTemp->Item[m_iItemReadsExecNorm].Value.Set(vVariant);

	vVariant.SetDouble(ai_pRead->m_iLine);
	pRegTemp->Item[m_iItemReadsExecLine].Value.Set(vVariant);

	pRegTemp->Item[m_iItemReadsCallStack].Value.Set(ai_pRead->m_vCallStack);


	res = sttAddItem( ai_IdExecT3, ai_IdExecNode, ai_IdExecItem, ai_pRead->m_vDate, ai_uiExecCSType, NULL );

	if (res == M4_ERROR)
		return M4_ERROR;

	//USAGES
	//Nos posicionamos en el bloque hijo (en NODE) del iRegister
	poNodeUsages = &(poAccess->Node[ (ClHandle)m_iNodeUsages ]);
	poNodeUsages->RecordSet.Current.Add();
	pRegTemp = (_ClRegister_Base*)&(poNodeUsages->RecordSet.Current);

	pRegTemp->Item[m_iItemUsagesDate].Value.Set(ai_pRead->m_vDate);

	if (ai_pRead->m_vValue.Type == M4CL_CPP_TYPE_STRING_VAR)
	{
		pRegTemp->Item[m_iItemUsagesValue].Value.Set(ai_pRead->m_vValue);
	}
	else
	{
		m4VariantType vAux(ai_pRead->m_vValue);
		ClVariantConversor oVariantMediator;
        res = oVariantMediator.ToCharString (vAux);

		pRegTemp->Item[m_iItemUsagesValue].Value.Set(vAux);
	}

	if (ai_pRead->m_dSliceStart > 0)
	{
		vVariant.SetDate(ai_pRead->m_dSliceStart);
		pRegTemp->Item[m_iItemUsagesSliceStart].Value.Set(vVariant);
	}

	if (ai_pRead->m_dSliceEnd > 0)
	{
		vVariant.SetDate(ai_pRead->m_dSliceEnd);
		pRegTemp->Item[m_iItemUsagesSliceEnd].Value.Set(vVariant);
	}

	vVariant.SetDouble(ai_pRead->m_iM4Type);
	pRegTemp->Item[m_iItemUsagesItemType].Value.Set(vVariant);

	pRegTemp->Item[m_iItemUsagesIdT3].Value.Set(ai_pRead->m_vIdT3);
	pRegTemp->Item[m_iItemUsagesIdNode].Value.Set(ai_pRead->m_vIdNode);
	pRegTemp->Item[m_iItemUsagesIdItem].Value.Set(ai_pRead->m_vIdItem);

	vVariant.SetStringRef(ai_IdExecRule);
	pRegTemp->Item[m_iItemReadsIdExecRule].Value.Set(vVariant);

	vVariant.SetDate(ai_pRead->m_dNorm);
	pRegTemp->Item[m_iItemUsagesExecNorm].Value.Set(vVariant);

	vVariant.SetDouble(ai_pRead->m_iLine);
	pRegTemp->Item[m_iItemUsagesExecLine].Value.Set(vVariant);

	pRegTemp->Item[m_iItemUsagesCallStack].Value.Set(ai_pRead->m_vCallStack);

	return M4_SUCCESS;
}


m4return_t ClVMStatisticsChannel::sttAddItem (m4pchar_t ai_IdT3, m4pchar_t ai_IdNode, m4pchar_t ai_IdItem, m4VariantType &ai_vDate, m4uint8_t ai_uiCSType, const ClSttExecData * ai_poExecData)
{
	m4VariantType	pstArg[3];
	m4int32_t		iRegister;
	m4VariantType	vTime, vExecutions, vAverage;
	ClNode *poNodeItems;		
	_ClRegister_Base * pRegTemp;
	m4return_t res = M4_SUCCESS;


	ClAccess*	poAccess = _GetAccess() ;

	if( poAccess == NULL )
	{
		return( M4_ERROR ) ;
	}

	//ITEMS
	//Nos posicionamos en el bloque hijo (en ITEM) del iRegister
	poNodeItems = &(poAccess->Node[ (ClHandle)m_iNodeItems ]);

	m4bool_t bFound;
	m4uint8_t aiFunct[3]={M4CL_INDEX_FUNCTION_EQUAL, M4CL_INDEX_FUNCTION_EQUAL, M4CL_INDEX_FUNCTION_EQUAL};
	pstArg[0].Type = pstArg[1].Type = pstArg[2].Type = M4CL_CPP_TYPE_STRING_VAR;

	pstArg[0].Data.PointerChar = ai_IdItem;
	pstArg[1].Data.PointerChar = ai_IdNode;
	pstArg[2].Data.PointerChar = ai_IdT3;

	poNodeItems->RecordSet.Register.Begin();

	bFound = poNodeItems->RecordSet.Register.Filter.Find (pstArg, aiFunct);

	if (M4_FALSE == bFound)
	{ 
		iRegister = -1;
	}
	else
	{
		//iRegister = poNodeItems->RecordSet.Register.Index;
		//MMIndex es un poco ilegal, pero como mi Current no tiene filtros deberia funcionar
		iRegister = poNodeItems->RecordSet.Register.GetMMIndex();
	}

	if (iRegister == -1)
	{
		//No está, añadimos un registro
		poNodeItems->RecordSet.Register.Add();
		pRegTemp = (_ClRegister_Base*) &(poNodeItems->RecordSet.Register);

		vTime.Type = vExecutions.Type = M4CL_CPP_TYPE_NUMBER;

		pRegTemp->Item[m_iItemItemsIdT3].Value.Set(ai_IdT3, strlen (ai_IdT3) + 1);
		pRegTemp->Item[m_iItemItemsIdNode].Value.Set(ai_IdNode, strlen (ai_IdNode) + 1);
		pRegTemp->Item[m_iItemItemsIdItem].Value.Set(ai_IdItem, strlen (ai_IdItem) + 1);
		pRegTemp->Item[m_iItemItemsDate].Value.Set(ai_vDate);
		
		vTime.Data.DoubleData = ai_uiCSType;
		pRegTemp->Item[m_iItemItemsCSType].Value.Set(vTime);

		vTime.Data.DoubleData = 0 ;		
		vExecutions.Data.DoubleData = 0;
		pRegTemp->Item[m_iItemItemsExecutions].Value.Set(vExecutions);
		pRegTemp->Item[m_iItemItemsTotalTime].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsDBTime].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsComTime].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsExecTime].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsSttTime].Value.Set(vTime);		
		pRegTemp->Item[m_iItemItemsSelfTotalTime].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsSelfDBTime].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsSelfComTime].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsSelfExecTime].Value.Set(vTime);	
		pRegTemp->Item[m_iItemItemsAvgTotalTime].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsAvgDBTime].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsAvgComTime].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsAvgExecTime].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsAvgSelfTotalTime].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsAvgSelfDBTime].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsAvgSelfComTime].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsAvgSelfExecTime].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsLastTotalTime].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsLastDBTime].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsLastComTime].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsLastExecTime].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsLastSelfTotalTime].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsLastSelfDBTime].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsLastSelfComTime].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsLastSelfExecTime].Value.Set(vTime);

		pRegTemp->Item[m_iItemItemsTotalPDUSize].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsOutputPDUSize].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsInputPDUSize].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsSelfTotalPDUSize].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsSelfOutputPDUSize].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsSelfInputPDUSize].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsAvgTotalPDUSize].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsAvgOutputPDUSize].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsAvgInputPDUSize].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsAvgSelfTotalPDUSize].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsAvgSelfOutputPDUSize].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsAvgSelfInputPDUSize].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsLastTotalPDUSize].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsLastOutputPDUSize].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsLastInputPDUSize].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsLastSelfTotalPDUSize].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsLastSelfOutputPDUSize].Value.Set(vTime);
		pRegTemp->Item[m_iItemItemsLastSelfInputPDUSize].Value.Set(vTime);

		pRegTemp->Item[m_iItemIsCacheable].Value.Set(vTime);
		pRegTemp->Item[m_iItemCacheHitPerc].Value.Set(vTime);


		//Prueba
		iRegister = poNodeItems->RecordSet.Register.Index;

		//Actuatlizamos el filtro de Orden!!
		poNodeItems->RecordSet.Register.Filter.UpdateOneIndex();
	}

	poNodeItems->RecordSet.Current[iRegister];

	if (ai_poExecData != NULL)
	{
		pRegTemp = (_ClRegister_Base*) &(poNodeItems->RecordSet.Current);
		
		res = sttFillItemTimes(*ai_poExecData, *pRegTemp);

		if (M4_ERROR == res)
			return res;

		res = sttFillItemPDUs(*ai_poExecData, *pRegTemp);
	}

	return res;
}


m4return_t ClVMStatisticsChannel::sttFillTimeTimes (const ClSttExecData & ai_oExecData, _ClRegister_Base &pRegTimes)
{
	m4VariantType vAuxVar;
	m4return_t res = M4_SUCCESS;

	vAuxVar.Type = M4CL_CPP_TYPE_NUMBER;
	
	//Total times
	vAuxVar.Data.DoubleData = ai_oExecData.m_dTotalTime;
	pRegTimes.Item[m_iItemTimesTotalTime].Value.Set(vAuxVar);		

	vAuxVar.Data.DoubleData = ai_oExecData.m_dDBTime;
	pRegTimes.Item[m_iItemTimesDBTime].Value.Set(vAuxVar);		

	vAuxVar.Data.DoubleData = ai_oExecData.m_dComTime;
	pRegTimes.Item[m_iItemTimesComTime].Value.Set(vAuxVar);		

	vAuxVar.Data.DoubleData = ai_oExecData.m_dSttTime;
	pRegTimes.Item[m_iItemTimesSttTime].Value.Set(vAuxVar);		

	vAuxVar.Data.DoubleData = ai_oExecData.m_dTotalTime - (ai_oExecData.m_dDBTime + ai_oExecData.m_dComTime /*+ ai_oExecData.m_dSttTime*/ );
	pRegTimes.Item[m_iItemTimesExecTime].Value.Set(vAuxVar);		

	//Self times
	vAuxVar.Data.DoubleData = ai_oExecData.m_dSelfTotalTime;
	pRegTimes.Item[m_iItemTimesSelfTotalTime].Value.Set(vAuxVar);		

	vAuxVar.Data.DoubleData = ai_oExecData.m_dSelfDBTime;
	pRegTimes.Item[m_iItemTimesSelfDBTime].Value.Set(vAuxVar);		

	vAuxVar.Data.DoubleData = ai_oExecData.m_dSelfComTime;
	pRegTimes.Item[m_iItemTimesSelfComTime].Value.Set(vAuxVar);		

	vAuxVar.Data.DoubleData = ai_oExecData.m_dSelfTotalTime - (ai_oExecData.m_dSelfDBTime + ai_oExecData.m_dSelfComTime /*+ ai_oExecData.m_dSelfSttTime*/);
	pRegTimes.Item[m_iItemTimesSelfExecTime].Value.Set(vAuxVar);		

	return M4_SUCCESS;
}


m4return_t ClVMStatisticsChannel::sttFillTimePDUs (const ClSttExecData & ai_oExecData, _ClRegister_Base &pRegTimes)
{
	m4VariantType vAuxVar;
	m4return_t res = M4_SUCCESS;

	vAuxVar.Type = M4CL_CPP_TYPE_STRING_VAR;
	
	//General data
	vAuxVar.Data.PointerChar = ai_oExecData.m_pcGeneralData;
	pRegTimes.Item[m_iItemTimesGeneralData].Value.Set(vAuxVar);		
	
	//Service
	vAuxVar.Data.PointerChar = ai_oExecData.m_pcService;
	pRegTimes.Item[m_iItemTimesService].Value.Set(vAuxVar);		

	//Cachehit
	vAuxVar.Type = M4CL_CPP_TYPE_NUMBER;

	if (M4_VM_STT_CACHE_FOUND == ai_oExecData.m_eCacheHit)
		vAuxVar.Data.DoubleData = 1;
	else
		vAuxVar.Data.DoubleData = 0;

	pRegTimes.Item[m_iItemTimesCacheHit].Value.Set(vAuxVar);		


	//Total 
	vAuxVar.Data.DoubleData = ai_oExecData.m_dOutputPDUSize + ai_oExecData.m_dInputPDUSize;
	pRegTimes.Item[m_iItemTimesTotalPDUSize].Value.Set(vAuxVar);		

	vAuxVar.Data.DoubleData = ai_oExecData.m_dOutputPDUSize;
	pRegTimes.Item[m_iItemTimesOutputPDUSize].Value.Set(vAuxVar);		

	vAuxVar.Data.DoubleData = ai_oExecData.m_dInputPDUSize;
	pRegTimes.Item[m_iItemTimesInputPDUSize].Value.Set(vAuxVar);		

	//Self 
	vAuxVar.Data.DoubleData = ai_oExecData.m_dSelfOutputPDUSize + ai_oExecData.m_dSelfInputPDUSize;
	pRegTimes.Item[m_iItemTimesSelfTotalPDUSize].Value.Set(vAuxVar);		

	vAuxVar.Data.DoubleData = ai_oExecData.m_dSelfOutputPDUSize;
	pRegTimes.Item[m_iItemTimesSelfOutputPDUSize].Value.Set(vAuxVar);		

	vAuxVar.Data.DoubleData = ai_oExecData.m_dSelfInputPDUSize;
	pRegTimes.Item[m_iItemTimesSelfInputPDUSize].Value.Set(vAuxVar);		

	return M4_SUCCESS;
}


//Internas
m4return_t ClVMStatisticsChannel::sttFillItemTimes (const ClSttExecData & ai_oExecData, _ClRegister_Base &pRegItems)
{
	m4VariantType vTime, vExecutions;
	m4VariantType vAverage, vExecTime, vAux;
	m4return_t res = M4_SUCCESS;

	vExecTime.Type = vAverage.Type = vAux.Type = M4CL_CPP_TYPE_NUMBER;

	//executions + 1
	pRegItems.Item[m_iItemItemsExecutions].Value.Get(vExecutions);
	vExecutions.Data.DoubleData ++;
	pRegItems.Item.Value.Set(vExecutions);

	//TotalTime
	vAux.Data.DoubleData = ai_oExecData.m_dTotalTime;

	pRegItems.Item[m_iItemItemsTotalTime].Value.Get(vTime);
	vTime.Data.DoubleData += vAux.Data.DoubleData;
	pRegItems.Item.Value.Set(vTime);

	pRegItems.Item[m_iItemItemsLastTotalTime].Value.Set(vAux);

	vAverage.Data.DoubleData = vTime.Data.DoubleData / vExecutions.Data.DoubleData;
	pRegItems.Item[m_iItemItemsAvgTotalTime].Value.Set(vAverage);

	//DBTime
	vAux.Data.DoubleData = ai_oExecData.m_dDBTime;

	pRegItems.Item[m_iItemItemsDBTime].Value.Get(vTime);
	vTime.Data.DoubleData += vAux.Data.DoubleData;
	pRegItems.Item.Value.Set(vTime);

	pRegItems.Item[m_iItemItemsLastDBTime].Value.Set(vAux);

	vAverage.Data.DoubleData = vTime.Data.DoubleData / vExecutions.Data.DoubleData;			
	pRegItems.Item[m_iItemItemsAvgDBTime].Value.Set(vAverage);

	//ComTime
	vAux.Data.DoubleData = ai_oExecData.m_dComTime;

	pRegItems.Item[m_iItemItemsComTime].Value.Get(vTime);
	vTime.Data.DoubleData += vAux.Data.DoubleData;
	pRegItems.Item.Value.Set(vTime);

	pRegItems.Item[m_iItemItemsLastComTime].Value.Set(vAux);

	vAverage.Data.DoubleData = vTime.Data.DoubleData / vExecutions.Data.DoubleData;			
	pRegItems.Item[m_iItemItemsAvgComTime].Value.Set(vAverage);

	//ExecTime
	pRegItems.Item[m_iItemItemsExecTime].Value.Get(vTime);
	vExecTime.Data.DoubleData = ai_oExecData.m_dTotalTime - 
							(ai_oExecData.m_dDBTime + ai_oExecData.m_dComTime 
							 /*+ ai_oExecData.m_dSttTime*/);
	vTime.Data.DoubleData += vExecTime.Data.DoubleData;
	pRegItems.Item.Value.Set(vTime);

	pRegItems.Item[m_iItemItemsLastExecTime].Value.Set(vExecTime);

	vAverage.Data.DoubleData = vTime.Data.DoubleData / vExecutions.Data.DoubleData;			
	pRegItems.Item[m_iItemItemsAvgExecTime].Value.Set(vAverage);


	//SelfTotalTime
	vAux.Data.DoubleData = ai_oExecData.m_dSelfTotalTime;

	pRegItems.Item[m_iItemItemsSelfTotalTime].Value.Get(vTime);
	vTime.Data.DoubleData += vAux.Data.DoubleData;
	pRegItems.Item.Value.Set(vTime);

	pRegItems.Item[m_iItemItemsLastSelfTotalTime].Value.Set(vAux);

	vAverage.Data.DoubleData = vTime.Data.DoubleData / vExecutions.Data.DoubleData;			
	pRegItems.Item[m_iItemItemsAvgSelfTotalTime].Value.Set(vAverage);

	//SelfDBTime
	vAux.Data.DoubleData = ai_oExecData.m_dSelfDBTime;

	pRegItems.Item[m_iItemItemsSelfDBTime].Value.Get(vTime);
	vTime.Data.DoubleData += vAux.Data.DoubleData;
	pRegItems.Item.Value.Set(vTime);

	pRegItems.Item[m_iItemItemsLastSelfDBTime].Value.Set(vAux);

	vAverage.Data.DoubleData = vTime.Data.DoubleData / vExecutions.Data.DoubleData;			
	pRegItems.Item[m_iItemItemsAvgSelfDBTime].Value.Set(vAverage);


	//SelfComTime
	vAux.Data.DoubleData = ai_oExecData.m_dSelfComTime;

	pRegItems.Item[m_iItemItemsSelfComTime].Value.Get(vTime);
	vTime.Data.DoubleData += vAux.Data.DoubleData;
	pRegItems.Item.Value.Set(vTime);

	pRegItems.Item[m_iItemItemsLastSelfComTime].Value.Set(vAux);

	vAverage.Data.DoubleData = vTime.Data.DoubleData / vExecutions.Data.DoubleData;			
	pRegItems.Item[m_iItemItemsAvgSelfComTime].Value.Set(vAverage);

	//SelfExecTime
	pRegItems.Item[m_iItemItemsSelfExecTime].Value.Get(vTime);
	vExecTime.Data.DoubleData = ai_oExecData.m_dSelfTotalTime - 
							(ai_oExecData.m_dSelfDBTime + ai_oExecData.m_dSelfComTime 
							 /*+ ai_oExecData.m_dSelfSttTime*/);
	vTime.Data.DoubleData += vExecTime.Data.DoubleData;
	pRegItems.Item.Value.Set(vTime);

	pRegItems.Item[m_iItemItemsLastSelfExecTime].Value.Set(vExecTime);

	vAverage.Data.DoubleData = vTime.Data.DoubleData / vExecutions.Data.DoubleData;			
	pRegItems.Item[m_iItemItemsAvgSelfExecTime].Value.Set(vAverage);

	//SttTime
	pRegItems.Item[m_iItemItemsSttTime].Value.Get(vTime);
	vTime.Data.DoubleData += ai_oExecData.m_dSttTime;
	pRegItems.Item.Value.Set(vTime);

	return M4_SUCCESS;
}


//Internas
m4return_t ClVMStatisticsChannel::sttFillItemPDUs (const ClSttExecData & ai_oExecData, _ClRegister_Base &pRegItems)
{
	m4VariantType vTime, vExecutions;
	m4VariantType vAverage, vAux;
	m4return_t res = M4_SUCCESS;

	vAverage.Type = vAux.Type = M4CL_CPP_TYPE_NUMBER;

	//executions
	pRegItems.Item[m_iItemItemsExecutions].Value.Get(vExecutions);

	//IsCacheable
	if (M4_VM_STT_CACHE_NO_CACHEABLE != ai_oExecData.m_eCacheHit)
	{
		vAux.Data.DoubleData = 1;
		pRegItems.Item[m_iItemIsCacheable].Value.Set(vAux);
	}

	//PacheHitPerc
	pRegItems.Item[m_iItemCacheHitPerc].Value.Get(vAux);
	vAux.Data.DoubleData *= ((vExecutions.Data.DoubleData - 1) / 100);

	if (M4_VM_STT_CACHE_FOUND == ai_oExecData.m_eCacheHit)
	{
		vAux.Data.DoubleData++;
	}

	vAux.Data.DoubleData = (vAux.Data.DoubleData / vExecutions.Data.DoubleData) * 100;
	pRegItems.Item.Value.Set(vAux);

	//Si no ha datos de PDUS salir
	if ( (ai_oExecData.m_dOutputPDUSize == 0) && (ai_oExecData.m_dInputPDUSize == 0))
		return M4_SUCCESS;

	vAux.Type = M4CL_CPP_TYPE_NUMBER;

	//TotalSize
	vAux.Data.DoubleData = ai_oExecData.m_dOutputPDUSize + ai_oExecData.m_dInputPDUSize;

	pRegItems.Item[m_iItemItemsTotalPDUSize].Value.Get(vTime);
	vTime.Data.DoubleData += vAux.Data.DoubleData;
	pRegItems.Item.Value.Set(vTime);

	pRegItems.Item[m_iItemItemsLastTotalPDUSize].Value.Set(vAux);

	vAverage.Data.DoubleData = vTime.Data.DoubleData / vExecutions.Data.DoubleData;
	pRegItems.Item[m_iItemItemsAvgTotalPDUSize].Value.Set(vAverage);

	//Output
	vAux.Data.DoubleData = ai_oExecData.m_dOutputPDUSize;

	pRegItems.Item[m_iItemItemsOutputPDUSize].Value.Get(vTime);
	vTime.Data.DoubleData += vAux.Data.DoubleData;
	pRegItems.Item.Value.Set(vTime);

	pRegItems.Item[m_iItemItemsLastOutputPDUSize].Value.Set(vAux);

	vAverage.Data.DoubleData = vTime.Data.DoubleData / vExecutions.Data.DoubleData;			
	pRegItems.Item[m_iItemItemsAvgOutputPDUSize].Value.Set(vAverage);

	//Input
	vAux.Data.DoubleData = ai_oExecData.m_dInputPDUSize;

	pRegItems.Item[m_iItemItemsInputPDUSize].Value.Get(vTime);
	vTime.Data.DoubleData += vAux.Data.DoubleData;
	pRegItems.Item.Value.Set(vTime);

	pRegItems.Item[m_iItemItemsLastInputPDUSize].Value.Set(vAux);

	vAverage.Data.DoubleData = vTime.Data.DoubleData / vExecutions.Data.DoubleData;			
	pRegItems.Item[m_iItemItemsAvgInputPDUSize].Value.Set(vAverage);


	//SelfTotalSize
	vAux.Data.DoubleData = ai_oExecData.m_dSelfOutputPDUSize + ai_oExecData.m_dSelfInputPDUSize;

	pRegItems.Item[m_iItemItemsSelfTotalPDUSize].Value.Get(vTime);
	vTime.Data.DoubleData += vAux.Data.DoubleData;
	pRegItems.Item.Value.Set(vTime);

	pRegItems.Item[m_iItemItemsLastSelfTotalPDUSize].Value.Set(vAux);

	vAverage.Data.DoubleData = vTime.Data.DoubleData / vExecutions.Data.DoubleData;
	pRegItems.Item[m_iItemItemsAvgSelfTotalPDUSize].Value.Set(vAverage);

	//Output
	vAux.Data.DoubleData = ai_oExecData.m_dSelfOutputPDUSize;

	pRegItems.Item[m_iItemItemsSelfOutputPDUSize].Value.Get(vTime);
	vTime.Data.DoubleData += vAux.Data.DoubleData;
	pRegItems.Item.Value.Set(vTime);

	pRegItems.Item[m_iItemItemsLastSelfOutputPDUSize].Value.Set(vAux);

	vAverage.Data.DoubleData = vTime.Data.DoubleData / vExecutions.Data.DoubleData;			
	pRegItems.Item[m_iItemItemsAvgSelfOutputPDUSize].Value.Set(vAverage);

	//Input
	vAux.Data.DoubleData = ai_oExecData.m_dSelfInputPDUSize;

	pRegItems.Item[m_iItemItemsSelfInputPDUSize].Value.Get(vTime);
	vTime.Data.DoubleData += vAux.Data.DoubleData;
	pRegItems.Item.Value.Set(vTime);

	pRegItems.Item[m_iItemItemsLastSelfInputPDUSize].Value.Set(vAux);

	vAverage.Data.DoubleData = vTime.Data.DoubleData / vExecutions.Data.DoubleData;			
	pRegItems.Item[m_iItemItemsAvgSelfInputPDUSize].Value.Set(vAverage);

	return M4_SUCCESS;
}


m4return_t ClVMStatisticsChannel::sttAddLabel (m4pchar_t ai_pcLabel)
{
	m4int32_t iRegister;
	ClNode	*poNodeLabels;		
	_ClRegister_Base * pRegTemp;


	ClAccess*	poAccess = _GetAccess() ;

	if( poAccess == NULL )
	{
		return( M4_ERROR ) ;
	}

	//LABELS
	poNodeLabels = &(poAccess->Node[ (ClHandle)m_iNodeLabels ]) ; 
	
	//Buscamos nuestro LABEL
	__SSearch( poNodeLabels->RecordSet.Register, ai_pcLabel, m_iItemLabelsLabel);
	iRegister = poNodeLabels->RecordSet.Register.Index;

	if (iRegister == -1)
	{
		//No está, añadimos un registro
		poNodeLabels->RecordSet.Current.Add();

		m4VariantType vDate;
		vDate.Type = M4CL_CPP_TYPE_DATE;
		vDate.Data.DoubleData = ClActualDate((m4uint8_t)M4CL_TIME_LOCAL_TIME);

		pRegTemp = (_ClRegister_Base*)&(poNodeLabels->RecordSet.Current);

		pRegTemp->Item[m_iItemLabelsLabel].Value.Set(ai_pcLabel, strlen (ai_pcLabel) + 1);		

		pRegTemp->Item[m_iItemLabelsDate].Value.Set(vDate);

	}
	else
	{
		poNodeLabels->RecordSet.Current[iRegister];
	}

	return M4_SUCCESS;
}


m4return_t ClVMStatisticsChannel::sttAddSQL (ClDBCache * ai_pDBCache)
{
	M4_ASSERT(ai_pDBCache);

	m4VariantType	pstArg[2];
	m4int32_t		iRegister;
	m4VariantType	vAux, vDate;
	ClNode	*poNodeSQL;	
	_ClRegister_Base * pRegTemp;
	m4bool_t bFound;
	m4uint32_t iStmLength;
	m4return_t res = M4_SUCCESS;


	ClAccess*	poAccess = _GetAccess() ;

	if( poAccess == NULL )
	{
		return( M4_ERROR ) ;
	}

	m4uint8_t aiFunct[2]={M4CL_INDEX_FUNCTION_EQUAL, M4CL_INDEX_FUNCTION_EQUAL};
	pstArg[0].Type = M4CL_CPP_TYPE_NUMBER;
	pstArg[1].Type = M4CL_CPP_TYPE_STRING_VAR;
	

	vDate.Type = M4CL_CPP_TYPE_DATE;
	vDate.Data.DoubleData = ClActualDate((m4uint8_t)M4CL_TIME_LOCAL_TIME);

	vAux.Type = M4CL_CPP_TYPE_NUMBER;
	vAux.Data.DoubleData = 0 ;		

	ClDBTimes *pAuxDBTimes = 0;
	ClDBTimesList::iterator it;

	poNodeSQL = &(poAccess->Node[ (ClHandle)m_iNodeSQL ]) ; 

	//Borra los objetos creados.
	for (it = ai_pDBCache->m_DBTimesList.begin (); it != ai_pDBCache->m_DBTimesList.end(); it++) 
	{		
		pAuxDBTimes = (*it);

		M4_ASSERT(pAuxDBTimes->m_pcStatement);

		//Sirve de algo buscar primero por strlen??
		iStmLength = strlen(pAuxDBTimes->m_pcStatement);
		pstArg[0].Data.DoubleData = iStmLength;
		pstArg[1].Data.PointerChar = pAuxDBTimes->m_pcStatement;

		poNodeSQL->RecordSet.Register.Begin();
		bFound = poNodeSQL->RecordSet.Register.Filter.Find (pstArg, aiFunct);

		if (M4_FALSE == bFound)
		{ 
			iRegister = -1;
		}
		else
		{
			iRegister = poNodeSQL->RecordSet.Register.GetMMIndex();
		}

		if (iRegister == -1)
		{
			//No está, añadimos un registro
			poNodeSQL->RecordSet.Register.Add();
			pRegTemp = (_ClRegister_Base*) &(poNodeSQL->RecordSet.Register);		

			vAux.Data.DoubleData = iStmLength;
			pRegTemp->Item[m_iItemSQLStmLength].Value.Set(vAux.Data.DoubleData);
			pRegTemp->Item[m_iItemSQLStatement].Value.Set(pAuxDBTimes->m_pcStatement, iStmLength + 1);
			pRegTemp->Item[m_iItemSQLDate].Value.Set(vDate);

			vAux.Data.DoubleData = pAuxDBTimes->m_eOp;			
			pRegTemp->Item[m_iItemSQLOperation].Value.Set(vAux);		
			vAux.Data.DoubleData = pAuxDBTimes->m_IsInternal;
			pRegTemp->Item[m_iItemSQLIsInternal].Value.Set(vAux);	

			vAux.Data.DoubleData = 0;
			pRegTemp->Item[m_iItemSQLNumPrepare].Value.Set(vAux);
			pRegTemp->Item[m_iItemSQLNumExecute].Value.Set(vAux);
			pRegTemp->Item[m_iItemSQLNumFetch].Value.Set(vAux);
			pRegTemp->Item[m_iItemSQLTotalTime].Value.Set(vAux);
			pRegTemp->Item[m_iItemSQLAvgTotalTime].Value.Set(vAux);
			pRegTemp->Item[m_iItemSQLLastTotalTime].Value.Set(vAux);
			pRegTemp->Item[m_iItemSQLPrepareTime].Value.Set(vAux);
			pRegTemp->Item[m_iItemSQLLastPrepareTime].Value.Set(vAux);
			pRegTemp->Item[m_iItemSQLAvgPrepareTime].Value.Set(vAux);		
			pRegTemp->Item[m_iItemSQLExecuteTime].Value.Set(vAux);
			pRegTemp->Item[m_iItemSQLLastExecuteTime].Value.Set(vAux);
			pRegTemp->Item[m_iItemSQLAvgExecuteTime].Value.Set(vAux);
			pRegTemp->Item[m_iItemSQLFetchTime].Value.Set(vAux);
			pRegTemp->Item[m_iItemSQLLastFetchTime].Value.Set(vAux);
			pRegTemp->Item[m_iItemSQLAvgFetchTime].Value.Set(vAux);

			//Prueba
			iRegister = poNodeSQL->RecordSet.Register.Index;
			//poNodeSQL->RecordSet.Current[poNodeSQL->RecordSet.Register.Index];

			//Actuatlizamos el filtro de Orden!!
			poNodeSQL->RecordSet.Register.Filter.UpdateOneIndex();
		}

		poNodeSQL->RecordSet.Current[iRegister];
		pRegTemp = (_ClRegister_Base*) &(poNodeSQL->RecordSet.Current);

		res = sttFillSQLTimes(pAuxDBTimes, *pRegTemp);			
	}
	
	return M4_SUCCESS;
}


m4return_t ClVMStatisticsChannel::sttFillSQLTimes (ClDBTimes *pDBTimes, _ClRegister_Base &pRegSQL)
{
	m4double_t dNumExecutes, dTotal;
	m4VariantType vExecutions, vAverage, vTime;

	vTime.Type = vExecutions.Type = vAverage.Type = M4CL_CPP_TYPE_NUMBER;

	M4_ASSERT(pDBTimes);

	//Executions
	pRegSQL.Item[m_iItemSQLNumPrepare].Value.Get(vExecutions);
	vExecutions.Data.DoubleData += pDBTimes->m_iNumPrepare;
	//vExecutions.Data.DoubleData ++;
	pRegSQL.Item.Value.Set(vExecutions);

	pRegSQL.Item[m_iItemSQLNumExecute].Value.Get(vExecutions);
	vExecutions.Data.DoubleData += pDBTimes->m_iNumExecute;
	dNumExecutes = vExecutions.Data.DoubleData;
	pRegSQL.Item.Value.Set(vExecutions);

	if ( pDBTimes->m_iNumFetch > pDBTimes->m_iNumExecute ) 
	{
		// Se le resta un Fetch por execute (ya que el ultimo fetch no trae datos)
		pRegSQL.Item[m_iItemSQLNumFetch].Value.Get(vExecutions);
		vExecutions.Data.DoubleData += pDBTimes->m_iNumFetch - pDBTimes->m_iNumExecute;
		pRegSQL.Item.Value.Set(vExecutions);
	}


	//Total
	dTotal = pDBTimes->m_dPrepareTime + pDBTimes->m_dExecuteTime + pDBTimes->m_dFetchTime;
	pRegSQL.Item[m_iItemSQLTotalTime].Value.Get(vTime);
	vTime.Data.DoubleData += dTotal;
	pRegSQL.Item.Value.Set(vTime);

	vAverage.Data.DoubleData = vTime.Data.DoubleData / dNumExecutes;			
	pRegSQL.Item[m_iItemSQLAvgTotalTime].Value.Set(vAverage);

	vTime.Data.DoubleData = dTotal / pDBTimes->m_iNumExecute;
	pRegSQL.Item[m_iItemSQLLastTotalTime].Value.Set(vTime);

	//Prepare
	pRegSQL.Item[m_iItemSQLPrepareTime].Value.Get(vTime);
	vTime.Data.DoubleData += pDBTimes->m_dPrepareTime;
	pRegSQL.Item.Value.Set(vTime);

	vAverage.Data.DoubleData = vTime.Data.DoubleData / dNumExecutes;			
	pRegSQL.Item[m_iItemSQLAvgPrepareTime].Value.Set(vAverage);

	vTime.Data.DoubleData = pDBTimes->m_dPrepareTime / pDBTimes->m_iNumExecute;
	pRegSQL.Item[m_iItemSQLLastPrepareTime].Value.Set(vTime);

	//Execute
	pRegSQL.Item[m_iItemSQLExecuteTime].Value.Get(vTime);
	vTime.Data.DoubleData += pDBTimes->m_dExecuteTime;
	pRegSQL.Item.Value.Set(vTime);

	vAverage.Data.DoubleData = vTime.Data.DoubleData / dNumExecutes;			
	pRegSQL.Item[m_iItemSQLAvgExecuteTime].Value.Set(vAverage);

	vTime.Data.DoubleData = pDBTimes->m_dExecuteTime / pDBTimes->m_iNumExecute;
	pRegSQL.Item[m_iItemSQLLastExecuteTime].Value.Set(vTime);
	
	//Fetch
	pRegSQL.Item[m_iItemSQLFetchTime].Value.Get(vTime);
	vTime.Data.DoubleData += pDBTimes->m_dFetchTime;
	pRegSQL.Item.Value.Set(vTime);

	vAverage.Data.DoubleData = vTime.Data.DoubleData / dNumExecutes;			
	pRegSQL.Item[m_iItemSQLAvgFetchTime].Value.Set(vAverage);

	vTime.Data.DoubleData = pDBTimes->m_dFetchTime / pDBTimes->m_iNumExecute;
	pRegSQL.Item[m_iItemSQLLastFetchTime].Value.Set(vTime);

	return M4_SUCCESS;
}


m4return_t ClVMStatisticsChannel::sttInsertBDTimes (ClDBCache * ai_pDBCache)
{
	M4_ASSERT(ai_pDBCache);
	
	m4return_t res = M4_SUCCESS;
	m4VariantType vAux;
	ClDBTimes *pAuxDBTimes = 0;
	ClDBTimesList::iterator it;


	ClAccess*	poAccess = _GetAccess() ;

	if( poAccess == NULL )
	{
		return( M4_ERROR ) ;
	}

	_ClRegister_Base * pRegTimes = (_ClRegister_Base*)(&poAccess->Node[ (ClHandle)m_iNodeDBTimes ].RecordSet.Current);

	//Borra los objetos creados.
	for (it = ai_pDBCache->m_DBTimesList.begin (); it != ai_pDBCache->m_DBTimesList.end(); it++) 
	{
		pRegTimes->Add();

		pAuxDBTimes = (*it);

		// No esta bloqueada una operacion, luego metemos un nuevo reistro.
		vAux.Type = M4CL_CPP_TYPE_NUMBER;

		vAux.Data.DoubleData = pAuxDBTimes->m_eOp;			
		pRegTimes->Item[m_iItemDBTimesOperation].Value.Set(vAux);		

		vAux.Data.DoubleData = pAuxDBTimes->m_IsInternal;
		pRegTimes->Item[m_iItemDBTimesIsInternal].Value.Set(vAux);		
	
		vAux.Type = M4CL_CPP_TYPE_STRING_VAR;

		if (pAuxDBTimes->m_pcStatement)
		{
			vAux.Data.PointerChar = pAuxDBTimes->m_pcStatement;			
			pRegTimes->Item[m_iItemDBTimesStatement].Value.Set(vAux);					
		}

		//Tiempos
		vAux.Type = M4CL_CPP_TYPE_NUMBER;

		vAux.Data.DoubleData = pAuxDBTimes->m_dPrepareTime;			
		pRegTimes->Item[m_iItemDBTimesPrepareTime].Value.Set(vAux);					

		vAux.Data.DoubleData = pAuxDBTimes->m_dExecuteTime;			
		pRegTimes->Item[m_iItemDBTimesExecuteTime].Value.Set(vAux);					

		vAux.Data.DoubleData = pAuxDBTimes->m_dFetchTime;			
		pRegTimes->Item[m_iItemDBTimesFetchTime].Value.Set(vAux);					

		
		if (pAuxDBTimes->m_eOp == M4_STT_LDB_SELECT)
		{
			//El último fetch no trae datos.
			if (pAuxDBTimes->m_iNumFetch == 0)
				vAux.Data.DoubleData = 0;
			else
				vAux.Data.DoubleData = pAuxDBTimes->m_iNumFetch - 1;
		}
		else
		{
			vAux.Data.DoubleData = pAuxDBTimes->m_iNumExecute;			
		}

		pRegTimes->Item[m_iItemDBTimesNumData].Value.Set(vAux);					
	}

	return M4_SUCCESS;
}


m4return_t ClVMStatisticsChannel::sttAddHistory (m4pchar_t ai_IdT3, m4pchar_t ai_IdNode, m4pchar_t ai_IdItem, m4pchar_t ai_IdRule,
										  m4pchar_t ai_ParentIdT3, m4pchar_t ai_ParentIdNode, m4pchar_t ai_ParentIdItem, m4pchar_t ai_ParentIdRule,
									      m4VariantType &ai_vDate, m4uint8_t ai_uiChannelCSType)
{
	m4return_t res = M4_SUCCESS;

	ClNode * poNodeHistory;
	_ClRegister_Base * pRegTemp;
	

	ClAccess*	poAccess = _GetAccess() ;

	if( poAccess == NULL )
	{
		return( M4_ERROR ) ;
	}

	//HISTORY
	//Nos posicionamos en el node History
	poNodeHistory = &(poAccess->Node[ (ClHandle)m_iNodeHistory ]) ; 
        
	poNodeHistory->RecordSet.Current.Add();

	pRegTemp = (_ClRegister_Base*)&(poNodeHistory->RecordSet.Current);

	M4_ASSERT(ai_IdNode && ai_IdT3 && ai_IdItem && ai_IdRule);

	pRegTemp->Item[m_iItemHistoryIdT3].Value.Set(ai_IdT3, strlen (ai_IdT3) + 1);
	pRegTemp->Item[m_iItemHistoryIdNode].Value.Set(ai_IdNode, strlen (ai_IdNode) + 1);
	pRegTemp->Item[m_iItemHistoryIdItem].Value.Set(ai_IdItem, strlen (ai_IdItem) + 1);
	pRegTemp->Item[m_iItemHistoryIdRule].Value.Set(ai_IdRule, strlen (ai_IdRule) + 1);
	pRegTemp->Item[m_iItemHistoryDate].Value.Set(ai_vDate);

	pRegTemp->Item[m_iItemHistoryParentIdT3].Value.Set(ai_ParentIdT3, strlen (ai_ParentIdT3) + 1);
	pRegTemp->Item[m_iItemHistoryParentIdNode].Value.Set(ai_ParentIdNode, strlen (ai_ParentIdNode) + 1);
	pRegTemp->Item[m_iItemHistoryParentIdItem].Value.Set(ai_ParentIdItem, strlen (ai_ParentIdItem) + 1);
	pRegTemp->Item[m_iItemHistoryParentIdRule].Value.Set(ai_ParentIdRule, strlen (ai_ParentIdRule) + 1);

	m4VariantType vAux;
	vAux.Type = M4CL_CPP_TYPE_NUMBER;
	vAux.Data.DoubleData = ai_uiChannelCSType;
	pRegTemp->Item[m_iItemHistoryRealCSType].Value.Set(vAux);

	return M4_SUCCESS;
}


m4return_t ClVMStatisticsChannel::sttAddHistoryParam (m4VariantType &ai_vParam)
{
	m4return_t res = M4_SUCCESS;

	ClNode * poNodeHistoryParams;
	_ClRegister_Base * pRegTemp;
	

	ClAccess*	poAccess = _GetAccess() ;

	if( poAccess == NULL )
	{
		return( M4_ERROR ) ;
	}

	//HISTORY PARAMS
	//Nos posicionamos en el node History Params
	poNodeHistoryParams = &(poAccess->Node[ (ClHandle)m_iNodeHistoryParams ]) ; 
        
	poNodeHistoryParams->RecordSet.Current.Add();

	pRegTemp = (_ClRegister_Base*)&(poNodeHistoryParams->RecordSet.Current);

	pRegTemp->Item[m_iItemHistoryParamsParam].Value.Set(ai_vParam);

	return M4_SUCCESS;
}


m4return_t ClVMStatisticsChannel::sttAddTimesParam (m4VariantType &ai_vParam)
{
	m4return_t res = M4_SUCCESS;

	ClNode * poNodeTimesParams;
	_ClRegister_Base * pRegTemp;
	

	ClAccess*	poAccess = _GetAccess() ;

	if( poAccess == NULL )
	{
		return( M4_ERROR ) ;
	}

	//TIMES PARAMS
	//Nos posicionamos en el node Times Params
	poNodeTimesParams = &(poAccess->Node[ (ClHandle)m_iNodeTimesParams ]) ; 
        
	poNodeTimesParams->RecordSet.Current.Add();

	pRegTemp = (_ClRegister_Base*)&(poNodeTimesParams->RecordSet.Current);

	pRegTemp->Item[m_iItemTimesParamsParam].Value.Set(ai_vParam);

	return M4_SUCCESS;
}


