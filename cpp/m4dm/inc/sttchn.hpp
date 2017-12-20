//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:                sttchn.cpp
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
//    Definiciones del canal de estadísticas
////
//==============================================================================
// COMMON...
#ifndef _STTCHN_HPP
#define _STTCHN_HPP

#include "m4dm_dll.hpp"
#include "m4string.hpp"
#include "m4define.hpp"
#include "m4types.hpp"

#include "cldefine.hpp"
#include "cltypes.hpp"

#include "handles.hpp"
#include "index.hpp"
#include "sttstate.hpp"


#define M4_LABEL_EMPTY_LABEL							"__NO_LABEL__"


#define M4_INSTANCE_STATISTICS							"STATISTICS"

#define M4_CHANNEL_STATISTICS							"SYS_STATISTICS"


#define M4_NODE_SCHSTT_LABELS							"SYS_STT_LABELS"

#define M4_ITEM_SCHSTT_LABELS_LABEL						"LABEL"
#define M4_ITEM_SCHSTT_LABELS_DATE						"DATE"


#define M4_NODE_SCHSTT_ITEMS							"SYS_STT_ITEMS"
	
#define M4_ITEM_SCHSTT_ITEMS_ID_NODE					"ID_NODE"
#define M4_ITEM_SCHSTT_ITEMS_ID_T3						"ID_T3"
#define M4_ITEM_SCHSTT_ITEMS_EXECUTIONS					"EXECUTIONS"
#define M4_ITEM_SCHSTT_ITEMS_DATE						"DATE"
#define M4_ITEM_SCHSTT_ITEMS_ID_ITEM					"ID_ITEM"
#define M4_ITEM_SCHSTT_ITEMS_TOTAL_TIME					"TOTAL_TIME"
#define M4_ITEM_SCHSTT_ITEMS_DB_TIME					"DB_TIME"
#define M4_ITEM_SCHSTT_ITEMS_COM_TIME					"COM_TIME"
#define M4_ITEM_SCHSTT_ITEMS_EXEC_TIME					"EXEC_TIME"
#define M4_ITEM_SCHSTT_ITEMS_STT_TIME					"STT_TIME"
#define M4_ITEM_SCHSTT_ITEMS_SELF_TOTAL_TIME			"SELF_TOTAL_TIME"
#define M4_ITEM_SCHSTT_ITEMS_SELF_DB_TIME				"SELF_DB_TIME"
#define M4_ITEM_SCHSTT_ITEMS_SELF_COM_TIME				"SELF_COM_TIME"
#define M4_ITEM_SCHSTT_ITEMS_SELF_EXEC_TIME				"SELF_EXEC_TIME"
#define M4_ITEM_SCHSTT_ITEMS_AVG_TOTAL_TIME				"AVG_TOTAL_TIME" //*
#define M4_ITEM_SCHSTT_ITEMS_AVG_DB_TIME				"AVG_DB_TIME"
#define M4_ITEM_SCHSTT_ITEMS_AVG_COM_TIME				"AVG_COM_TIME"
#define M4_ITEM_SCHSTT_ITEMS_AVG_EXEC_TIME				"AVG_EXEC_TIME"
#define M4_ITEM_SCHSTT_ITEMS_AVG_SELF_TOTAL_TIME		"AVG_SELF_TOTAL_TIME" //*
#define M4_ITEM_SCHSTT_ITEMS_AVG_SELF_DB_TIME			"AVG_SELF_DB_TIME"
#define M4_ITEM_SCHSTT_ITEMS_AVG_SELF_COM_TIME			"AVG_SELF_COM_TIME"
#define M4_ITEM_SCHSTT_ITEMS_AVG_SELF_EXEC_TIME			"AVG_SELF_EXEC_TIME"
#define M4_ITEM_SCHSTT_ITEMS_LAST_TOTAL_TIME			"LAST_TOTAL_TIME" //*
#define M4_ITEM_SCHSTT_ITEMS_LAST_DB_TIME				"LAST_DB_TIME"
#define M4_ITEM_SCHSTT_ITEMS_LAST_COM_TIME				"LAST_COM_TIME"
#define M4_ITEM_SCHSTT_ITEMS_LAST_EXEC_TIME				"LAST_EXEC_TIME"
#define M4_ITEM_SCHSTT_ITEMS_LAST_SELF_TOTAL_TIME		"LAST_SELF_TOTAL_TIME" //*
#define M4_ITEM_SCHSTT_ITEMS_LAST_SELF_DB_TIME			"LAST_SELF_DB_TIME"
#define M4_ITEM_SCHSTT_ITEMS_LAST_SELF_COM_TIME			"LAST_SELF_COM_TIME"
#define M4_ITEM_SCHSTT_ITEMS_LAST_SELF_EXEC_TIME		"LAST_SELF_EXEC_TIME"


#define M4_ITEM_SCHSTT_ITEMS_TOTAL_PDU_SIZE				"TOTAL_PDU_SIZE"
#define M4_ITEM_SCHSTT_ITEMS_OUTPUT_PDU_SIZE			"OUTPUT_PDU_SIZE"
#define M4_ITEM_SCHSTT_ITEMS_INPUT_PDU_SIZE				"INPUT_PDU_SIZE"
#define M4_ITEM_SCHSTT_ITEMS_SELF_TOTAL_PDU_SIZE		"SELF_TOTAL_PDU_SIZE"
#define M4_ITEM_SCHSTT_ITEMS_SELF_OUTPUT_PDU_SIZE		"SELF_OUTPUT_PDU_SIZE"
#define M4_ITEM_SCHSTT_ITEMS_SELF_INPUT_PDU_SIZE		"SELF_INPUT_PDU_SIZE"
#define M4_ITEM_SCHSTT_ITEMS_AVG_TOTAL_PDU_SIZE			"AVG_TOTAL_PDU_SIZE" 
#define M4_ITEM_SCHSTT_ITEMS_AVG_OUTPUT_PDU_SIZE		"AVG_OUTPUT_PDU_SIZE" 
#define M4_ITEM_SCHSTT_ITEMS_AVG_INPUT_PDU_SIZE			"AVG_INPUT_PDU_SIZE"
#define M4_ITEM_SCHSTT_ITEMS_AVG_SELF_TOTAL_PDU_SIZE	"AVG_SELF_TOTAL_PDU_SIZE" 
#define M4_ITEM_SCHSTT_ITEMS_AVG_SELF_OUTPUT_PDU_SIZE	"AVG_SELF_OUTPUT_PDU_SIZE" 
#define M4_ITEM_SCHSTT_ITEMS_AVG_SELF_INPUT_PDU_SIZE	"AVG_SELF_INPUT_PDU_SIZE"
#define M4_ITEM_SCHSTT_ITEMS_LAST_TOTAL_PDU_SIZE		"LAST_TOTAL_PDU_SIZE" 
#define M4_ITEM_SCHSTT_ITEMS_LAST_OUTPUT_PDU_SIZE		"LAST_OUTPUT_PDU_SIZE" 
#define M4_ITEM_SCHSTT_ITEMS_LAST_INPUT_PDU_SIZE		"LAST_INPUT_PDU_SIZE"
#define M4_ITEM_SCHSTT_ITEMS_LAST_SELF_TOTAL_PDU_SIZE	"LAST_SELF_TOTAL_PDU_SIZE" 
#define M4_ITEM_SCHSTT_ITEMS_LAST_SELF_OUTPUT_PDU_SIZE	"LAST_SELF_OUTPUT_PDU_SIZE" 
#define M4_ITEM_SCHSTT_ITEMS_LAST_SELF_INPUT_PDU_SIZE	"LAST_SELF_INPUT_PDU_SIZE"
#define M4_ITEM_SCHSTT_ITEMS_IS_CACHEABLE				"IS_CACHEABLE"
#define M4_ITEM_SCHSTT_ITEMS_CACHE_HIT_PERC				"CACHE_HIT_PERC"
#define M4_ITEM_SCHSTT_ITEMS_CS_TYPE					"CS_TYPE"


#define	M4_NODE_SCHSTT_VALUES							"SYS_STT_VALUES"

#define M4_ITEM_SCHSTT_VALUES_DATE						"DATE"
#define M4_ITEM_SCHSTT_VALUES_VALUE						"VALUE"
#define M4_ITEM_SCHSTT_VALUES_SLICE_START				"SLICE_START"
#define M4_ITEM_SCHSTT_VALUES_SLICE_END					"SLICE_END"
#define M4_ITEM_SCHSTT_VALUES_PRIORITY					"PRIORITY"
#define M4_ITEM_SCHSTT_VALUES_ASSIGNED					"ASSIGNED"
#define M4_ITEM_SCHSTT_VALUES_SOURCE					"SOURCE"
#define M4_ITEM_SCHSTT_VALUES_ITEM_TYPE					"ITEM_TYPE"
#define M4_ITEM_SCHSTT_VALUES_ID_EXEC_T3				"ID_EXEC_T3"
#define M4_ITEM_SCHSTT_VALUES_ID_EXEC_NODE				"ID_EXEC_NODE"
#define M4_ITEM_SCHSTT_VALUES_ID_EXEC_ITEM				"ID_EXEC_ITEM"
#define M4_ITEM_SCHSTT_VALUES_ID_EXEC_RULE				"ID_EXEC_RULE"
#define M4_ITEM_SCHSTT_VALUES_EXEC_NORM					"EXEC_NORM"
#define M4_ITEM_SCHSTT_VALUES_EXEC_LINE					"EXEC_LINE"
#define M4_ITEM_SCHSTT_VALUES_CALL_STACK				"CALL_STACK"


#define	M4_NODE_SCHSTT_ASSIGNS							"SYS_STT_ASSIGNS"

#define M4_ITEM_SCHSTT_ASSIGNS_DATE						"DATE"
#define M4_ITEM_SCHSTT_ASSIGNS_VALUE					"VALUE"
#define M4_ITEM_SCHSTT_ASSIGNS_SLICE_START				"SLICE_START"
#define M4_ITEM_SCHSTT_ASSIGNS_SLICE_END				"SLICE_END"
#define M4_ITEM_SCHSTT_ASSIGNS_PRIORITY					"PRIORITY"
#define M4_ITEM_SCHSTT_ASSIGNS_ASSIGNED					"ASSIGNED"
#define M4_ITEM_SCHSTT_ASSIGNS_SOURCE					"SOURCE"
#define M4_ITEM_SCHSTT_ASSIGNS_ITEM_TYPE				"ITEM_TYPE"
#define M4_ITEM_SCHSTT_ASSIGNS_ID_T3					"ID_T3"
#define M4_ITEM_SCHSTT_ASSIGNS_ID_NODE					"ID_NODE"
#define M4_ITEM_SCHSTT_ASSIGNS_ID_ITEM					"ID_ITEM"
#define M4_ITEM_SCHSTT_ASSIGNS_ID_EXEC_RULE				"ID_EXEC_RULE"
#define M4_ITEM_SCHSTT_ASSIGNS_EXEC_NORM				"EXEC_NORM"
#define M4_ITEM_SCHSTT_ASSIGNS_EXEC_LINE				"EXEC_LINE"
#define M4_ITEM_SCHSTT_ASSIGNS_CALL_STACK				"CALL_STACK"


#define	M4_NODE_SCHSTT_READS							"SYS_STT_READS"

#define M4_ITEM_SCHSTT_READS_DATE						"DATE"
#define M4_ITEM_SCHSTT_READS_VALUE						"VALUE"
#define M4_ITEM_SCHSTT_READS_SLICE_START				"SLICE_START"
#define M4_ITEM_SCHSTT_READS_SLICE_END					"SLICE_END"
#define M4_ITEM_SCHSTT_READS_ITEM_TYPE					"ITEM_TYPE"
#define M4_ITEM_SCHSTT_READS_ID_EXEC_T3					"ID_EXEC_T3"
#define M4_ITEM_SCHSTT_READS_ID_EXEC_NODE				"ID_EXEC_NODE"
#define M4_ITEM_SCHSTT_READS_ID_EXEC_ITEM				"ID_EXEC_ITEM"
#define M4_ITEM_SCHSTT_READS_ID_EXEC_RULE				"ID_EXEC_RULE"
#define M4_ITEM_SCHSTT_READS_EXEC_NORM					"EXEC_NORM"
#define M4_ITEM_SCHSTT_READS_EXEC_LINE					"EXEC_LINE"
#define M4_ITEM_SCHSTT_READS_CALL_STACK					"CALL_STACK"


#define	M4_NODE_SCHSTT_USAGES							"SYS_STT_USAGES"

#define M4_ITEM_SCHSTT_USAGES_DATE						"DATE"
#define M4_ITEM_SCHSTT_USAGES_VALUE						"VALUE"
#define M4_ITEM_SCHSTT_USAGES_SLICE_START				"SLICE_START"
#define M4_ITEM_SCHSTT_USAGES_SLICE_END					"SLICE_END"
#define M4_ITEM_SCHSTT_USAGES_ITEM_TYPE					"ITEM_TYPE"
#define M4_ITEM_SCHSTT_USAGES_ID_T3						"ID_T3"
#define M4_ITEM_SCHSTT_USAGES_ID_NODE					"ID_NODE"
#define M4_ITEM_SCHSTT_USAGES_ID_ITEM					"ID_ITEM"
#define M4_ITEM_SCHSTT_USAGES_ID_EXEC_RULE				"ID_EXEC_RULE"
#define M4_ITEM_SCHSTT_USAGES_EXEC_NORM					"EXEC_NORM"
#define M4_ITEM_SCHSTT_USAGES_EXEC_LINE					"EXEC_LINE"
#define M4_ITEM_SCHSTT_USAGES_CALL_STACK				"CALL_STACK"


#define M4_NODE_SCHSTT_TIMES							"SYS_STT_TIMES"

#define M4_ITEM_SCHSTT_TIMES_DATE						"DATE"
#define M4_ITEM_SCHSTT_TIMES_ID_RULE					"ID_RULE"
#define M4_ITEM_SCHSTT_TIMES_NORM						"NORM"
#define M4_ITEM_SCHSTT_TIMES_TOTAL_TIME					"TOTAL_TIME"
#define M4_ITEM_SCHSTT_TIMES_DB_TIME					"DB_TIME"
#define M4_ITEM_SCHSTT_TIMES_COM_TIME					"COM_TIME"
#define M4_ITEM_SCHSTT_TIMES_EXEC_TIME					"EXEC_TIME"
#define M4_ITEM_SCHSTT_TIMES_STT_TIME					"STT_TIME"
#define M4_ITEM_SCHSTT_TIMES_SELF_TOTAL_TIME			"SELF_TOTAL_TIME"
#define M4_ITEM_SCHSTT_TIMES_SELF_DB_TIME				"SELF_DB_TIME"
#define M4_ITEM_SCHSTT_TIMES_SELF_COM_TIME				"SELF_COM_TIME"
#define M4_ITEM_SCHSTT_TIMES_SELF_EXEC_TIME				"SELF_EXEC_TIME"
#define M4_ITEM_SCHSTT_TIMES_TOTAL_PDU_SIZE				"TOTAL_PDU_SIZE"
#define M4_ITEM_SCHSTT_TIMES_OUTPUT_PDU_SIZE			"OUTPUT_PDU_SIZE"
#define M4_ITEM_SCHSTT_TIMES_INPUT_PDU_SIZE				"INPUT_PDU_SIZE"
#define M4_ITEM_SCHSTT_TIMES_SELF_TOTAL_PDU_SIZE		"SELF_TOTAL_PDU_SIZE"
#define M4_ITEM_SCHSTT_TIMES_SELF_OUTPUT_PDU_SIZE		"SELF_OUTPUT_PDU_SIZE"
#define M4_ITEM_SCHSTT_TIMES_SELF_INPUT_PDU_SIZE		"SELF_INPUT_PDU_SIZE"
#define M4_ITEM_SCHSTT_TIMES_GENERAL_DATA				"GENERAL_DATA"
#define M4_ITEM_SCHSTT_TIMES_SERVICE					"SERVICE"
#define M4_ITEM_SCHSTT_TIMES_CACHE_HIT					"CACHE_HIT"
#define M4_ITEM_SCHSTT_TIMES_PARENT_ID_NODE				"PARENT_ID_NODE"
#define M4_ITEM_SCHSTT_TIMES_PARENT_ID_T3				"PARENT_ID_T3"
#define M4_ITEM_SCHSTT_TIMES_PARENT_ID_ITEM				"PARENT_ID_ITEM"
#define M4_ITEM_SCHSTT_TIMES_PARENT_ID_RULE				"PARENT_ID_RULE"
#define M4_ITEM_SCHSTT_TIMES_REAL_CS_TYPE				"REAL_CS_TYPE"

#define M4_NODE_SCHSTT_DBTIMES							"SYS_STT_DBTIMES"

#define M4_ITEM_SCHSTT_DBTIMES_OPERATION				"OPERATION"
#define M4_ITEM_SCHSTT_DBTIMES_IS_INTERNAL				"IS_INTERNAL"
#define M4_ITEM_SCHSTT_DBTIMES_STATEMENT				"STATEMENT"
#define M4_ITEM_SCHSTT_DBTIMES_PREPARE_TIME				"PREPARE_TIME"
#define M4_ITEM_SCHSTT_DBTIMES_EXECUTE_TIME				"EXECUTE_TIME"
#define M4_ITEM_SCHSTT_DBTIMES_FETCH_TIME				"FETCH_TIME"
#define M4_ITEM_SCHSTT_DBTIMES_NUM_DATA					"NUM_DATA"


#define M4_NODE_SCHSTT_SQL								"SYS_STT_SQL"

#define M4_ITEM_SCHSTT_SQL_STATEMENT					"STATEMENT"
#define M4_ITEM_SCHSTT_SQL_STM_LENGTH					"STM_LENGTH"
#define M4_ITEM_SCHSTT_SQL_OPERATION					"OPERATION"
#define M4_ITEM_SCHSTT_SQL_IS_INTERNAL					"IS_INTERNAL"
#define M4_ITEM_SCHSTT_SQL_NUM_PREPARE					"NUM_PREPARE"
#define M4_ITEM_SCHSTT_SQL_NUM_EXECUTE					"NUM_EXECUTE"
#define M4_ITEM_SCHSTT_SQL_NUM_FETCH					"NUM_FETCH"
#define M4_ITEM_SCHSTT_SQL_TOTAL_TIME					"TOTAL_TIME"
#define M4_ITEM_SCHSTT_SQL_PREPARE_TIME					"PREPARE_TIME"
#define M4_ITEM_SCHSTT_SQL_EXECUTE_TIME					"EXECUTE_TIME"
#define M4_ITEM_SCHSTT_SQL_FETCH_TIME					"FETCH_TIME"
#define M4_ITEM_SCHSTT_SQL_AVG_TOTAL_TIME				"AVG_TOTAL_TIME"
#define M4_ITEM_SCHSTT_SQL_AVG_PREPARE_TIME				"AVG_PREPARE_TIME"
#define M4_ITEM_SCHSTT_SQL_AVG_EXECUTE_TIME				"AVG_EXECUTE_TIME"
#define M4_ITEM_SCHSTT_SQL_AVG_FETCH_TIME				"AVG_FETCH_TIME"
#define M4_ITEM_SCHSTT_SQL_LAST_TOTAL_TIME				"LAST_TOTAL_TIME"
#define M4_ITEM_SCHSTT_SQL_LAST_PREPARE_TIME			"LAST_PREPARE_TIME"
#define M4_ITEM_SCHSTT_SQL_LAST_EXECUTE_TIME			"LAST_EXECUTE_TIME"
#define M4_ITEM_SCHSTT_SQL_LAST_FETCH_TIME				"LAST_FETCH_TIME"
#define M4_ITEM_SCHSTT_SQL_DATE							"DATE"

#define M4_NODE_SCHSTT_HISTORY							"SYS_STT_HISTORY"
	
#define M4_ITEM_SCHSTT_HISTORY_ID_T3					"ID_T3"
#define M4_ITEM_SCHSTT_HISTORY_ID_NODE					"ID_NODE"
#define M4_ITEM_SCHSTT_HISTORY_ID_ITEM					"ID_ITEM"
#define M4_ITEM_SCHSTT_HISTORY_ID_RULE					"ID_RULE"
#define M4_ITEM_SCHSTT_HISTORY_REAL_CS_TYPE				"REAL_CS_TYPE"
#define M4_ITEM_SCHSTT_HISTORY_DATE						"DATE"
#define M4_ITEM_SCHSTT_HISTORY_PARENT_ID_NODE			"PARENT_ID_NODE"
#define M4_ITEM_SCHSTT_HISTORY_PARENT_ID_T3				"PARENT_ID_T3"
#define M4_ITEM_SCHSTT_HISTORY_PARENT_ID_ITEM			"PARENT_ID_ITEM"
#define M4_ITEM_SCHSTT_HISTORY_PARENT_ID_RULE			"PARENT_ID_RULE"


#define M4_NODE_SCHSTT_HISTORY_PARAMS					"SYS_STT_HISTORY_PARAMS"
	
#define M4_ITEM_SCHSTT_HISTORY_PARAMS_PARAM				"PARAM"


#define M4_NODE_SCHSTT_TIMES_PARAMS						"SYS_STT_TIMES_PARAMS"
	
#define M4_ITEM_SCHSTT_TIMES_PARAMS_PARAM				"PARAM"


class _ClRegister_Base;
class ClAccess;
class ClChannel;
class ClChannelManager;
class ClLevel2;


class M4_DECL_M4DM ClVMStatisticsChannel{
public:
	ClVMStatisticsChannel(void);
	~ClVMStatisticsChannel(void);

	m4return_t sttChnInit( ClChannelManager * ai_pChannelManager );
	m4return_t sttChnInit( ClChannel *ai_poSttChannel ) ;

	ClChannel* sttGetChannel (void);

	//----------------------------------------------------------------------------------------------------------------------------------------
	//Funciones con parametros explicitos (sin handles)	
	m4return_t sttAddLabel (m4pchar_t ai_pcLabel);

	//Función de Item generica. Añade en el current de Label y posiciona Items.
	m4return_t sttAddItem (m4pchar_t ai_IdT3, m4pchar_t ai_IdNode, m4pchar_t ai_IdItem, m4VariantType &ai_vDate, m4uint8_t ai_uiCSType, const ClSttExecData * ai_poExecData);

	//Funciones de Times. Añade en el current de Items y posiciona Times.
	m4return_t sttAddTime (ClSttState * ai_pState, m4pchar_t ai_ParentIdT3, m4pchar_t ai_ParentIdNode, m4pchar_t ai_ParentIdItem, m4pchar_t ai_ParentIdRule);

	//Función de DBTimes. Añade en el current de Times.
	m4return_t sttInsertBDTimes (ClDBCache * ai_pDBCache);

 	//Función de Values. Añade en el current de Label y posiciona Items y Values.
	m4return_t sttAddValue (ClSttValues *ai_pValue, m4pchar_t ai_IdExecT3, m4pchar_t ai_IdExecNode, m4pchar_t ai_IdExecItem, m4pchar_t ai_IdExecRule, m4uint8_t ai_uiExecCSType);

 	//Función de Reads. Añade en el current de Label y posiciona Items y Reads y Usages.
	m4return_t sttAddRead (ClSttReads *ai_pRead, m4pchar_t ai_IdExecT3, m4pchar_t ai_IdExecNode, m4pchar_t ai_IdExecItem, m4pchar_t ai_IdExecRule, m4uint8_t ai_uiExecCSType);

	//Función de History. Añade en el current de Label.
	m4return_t sttAddHistory (m4pchar_t ai_IdT3, m4pchar_t ai_IdNode, m4pchar_t ai_IdItem, m4pchar_t ai_IdRule, 
							  m4pchar_t ai_ParentIdT3, m4pchar_t ai_ParentIdNode, m4pchar_t ai_ParentIdItem, m4pchar_t ai_ParentIdRule,
							  m4VariantType &ai_vDate, m4uint8_t ai_uiChannelCSType);

	//
	m4return_t sttAddHistoryParam (m4VariantType &ai_vParam);

	//
	m4return_t sttAddTimesParam (m4VariantType &ai_vParam);
	
	//Función de SQL. Añade en el current de Items.
	m4return_t sttAddSQL (ClDBCache * ai_pDBCache);

private:
	ClAccess* _GetAccess(void);
	m4return_t _Init(ClChannelManager* ai_poInitialChannelManager);
	m4return_t _Init(ClChannel* ai_poInitialChannel);
	m4return_t _Init(void);

	m4return_t sttFillItemTimes (const ClSttExecData & ai_oExecData, _ClRegister_Base &pRegItems);
	m4return_t sttFillItemPDUs (const ClSttExecData & ai_oExecData, _ClRegister_Base &pRegItems);
	m4return_t sttFillTimeTimes (const ClSttExecData & ai_oExecData, _ClRegister_Base &pRegTimes);
	m4return_t sttFillTimePDUs (const ClSttExecData & ai_oExecData, _ClRegister_Base &pRegTimes);
	m4return_t sttFillSQLTimes (ClDBTimes *pDBTimes, _ClRegister_Base &pRegSQL);


	m4bool_t			m_bInitialized;
	m4bool_t			m_bChannelOwner;
	ClChannelManager*	m_poInitialChannelManager;
	ClChannel*			m_poInitialChannel;
	ClAccess*			m_poStatAccess;

	m4uint32_t		m_iNodeLabels,
					m_iNodeItems, 
					m_iNodeTimes, 
					m_iNodeValues,
					m_iNodeAssigns,
					m_iNodeReads,
					m_iNodeUsages,
					m_iNodeDBTimes,
					m_iNodeSQL,
					m_iNodeHistory,
					m_iNodeHistoryParams,
					m_iNodeTimesParams;

	ClItemIndex		m_iItemLabelsLabel;

	
	ClItemIndex		m_iItemLabelsDate, 
					m_iItemItemsDate, 
					m_iItemTimesDate, 
					m_iItemValuesDate,
					m_iItemAssignsDate,
					m_iItemReadsDate,
					m_iItemUsagesDate,
					m_iItemSQLDate,
					m_iItemHistoryDate;

	ClItemIndex		m_iItemValuesValue,
					m_iItemValuesSliceStart,
					m_iItemValuesSliceEnd,
					m_iItemValuesPriority,
					m_iItemValuesAssigned,
					m_iItemValuesSource,
					m_iItemValuesItemType,
					m_iItemValuesIdExecT3,
					m_iItemValuesIdExecNode,
					m_iItemValuesIdExecItem,
					m_iItemValuesIdExecRule,
					m_iItemValuesExecNorm,
					m_iItemValuesExecLine,
					m_iItemValuesCallStack;

	ClItemIndex		m_iItemAssignsValue,
					m_iItemAssignsSliceStart,
					m_iItemAssignsSliceEnd,
					m_iItemAssignsPriority,
					m_iItemAssignsAssigned,
					m_iItemAssignsSource,
					m_iItemAssignsItemType,
					m_iItemAssignsIdT3,
					m_iItemAssignsIdNode,
					m_iItemAssignsIdItem,
					m_iItemAssignsIdExecRule,
					m_iItemAssignsExecNorm,
					m_iItemAssignsExecLine,
					m_iItemAssignsCallStack;

	ClItemIndex		m_iItemReadsValue,
					m_iItemReadsSliceStart,
					m_iItemReadsSliceEnd,
					m_iItemReadsItemType,
					m_iItemReadsIdExecT3,
					m_iItemReadsIdExecNode,
					m_iItemReadsIdExecItem,
					m_iItemReadsIdExecRule,
					m_iItemReadsExecNorm,
					m_iItemReadsExecLine,
					m_iItemReadsCallStack;

	ClItemIndex		m_iItemUsagesValue,
					m_iItemUsagesSliceStart,
					m_iItemUsagesSliceEnd,
					m_iItemUsagesItemType,
					m_iItemUsagesIdT3,
					m_iItemUsagesIdNode,
					m_iItemUsagesIdItem,
					m_iItemUsagesIdExecRule,
					m_iItemUsagesExecNorm,
					m_iItemUsagesExecLine,
					m_iItemUsagesCallStack;

	ClItemIndex		m_iItemItemsIdT3, 
					m_iItemItemsIdNode, 
					m_iItemItemsIdItem,
					m_iItemItemsExecutions, 
					m_iItemItemsTotalTime, 
					m_iItemItemsDBTime, 
					m_iItemItemsComTime, 
					m_iItemItemsExecTime, 
					m_iItemItemsSttTime,
					m_iItemItemsSelfTotalTime, 
					m_iItemItemsSelfDBTime, 
					m_iItemItemsSelfComTime, 
					m_iItemItemsSelfExecTime, 
					m_iItemItemsAvgTotalTime,
					m_iItemItemsAvgDBTime,
					m_iItemItemsAvgComTime,
					m_iItemItemsAvgExecTime,
					m_iItemItemsAvgSelfTotalTime,
					m_iItemItemsAvgSelfDBTime,
					m_iItemItemsAvgSelfComTime,
					m_iItemItemsAvgSelfExecTime,
					m_iItemItemsLastTotalTime, 
					m_iItemItemsLastDBTime, 
					m_iItemItemsLastComTime, 
					m_iItemItemsLastExecTime, 
					m_iItemItemsLastSelfTotalTime, 
					m_iItemItemsLastSelfDBTime, 
					m_iItemItemsLastSelfComTime, 
					m_iItemItemsLastSelfExecTime,
					m_iItemItemsTotalPDUSize, 
					m_iItemItemsOutputPDUSize, 
					m_iItemItemsInputPDUSize,
					m_iItemItemsSelfTotalPDUSize, 
					m_iItemItemsSelfOutputPDUSize, 
					m_iItemItemsSelfInputPDUSize,
					m_iItemItemsAvgTotalPDUSize, 
					m_iItemItemsAvgOutputPDUSize, 
					m_iItemItemsAvgInputPDUSize,
					m_iItemItemsAvgSelfTotalPDUSize, 
					m_iItemItemsAvgSelfOutputPDUSize, 
					m_iItemItemsAvgSelfInputPDUSize,
					m_iItemItemsLastTotalPDUSize, 
					m_iItemItemsLastOutputPDUSize, 
					m_iItemItemsLastInputPDUSize,
					m_iItemItemsLastSelfTotalPDUSize, 
					m_iItemItemsLastSelfOutputPDUSize, 
					m_iItemItemsLastSelfInputPDUSize,
					m_iItemIsCacheable,
					m_iItemCacheHitPerc,
					m_iItemItemsCSType;

	ClItemIndex 	m_iItemTimesIdRule,
					m_iItemTimesNorm,
					m_iItemTimesParentIdT3, 
					m_iItemTimesParentIdNode, 
					m_iItemTimesParentIdItem,
					m_iItemTimesParentIdRule,
					m_iItemTimesTotalTime, 
					m_iItemTimesDBTime,
					m_iItemTimesComTime,
					m_iItemTimesExecTime,
					m_iItemTimesSttTime,
					m_iItemTimesSelfTotalTime, 
					m_iItemTimesSelfDBTime,
					m_iItemTimesSelfComTime,
					m_iItemTimesSelfExecTime,
					m_iItemTimesTotalPDUSize,
					m_iItemTimesOutputPDUSize,
					m_iItemTimesInputPDUSize,
					m_iItemTimesSelfTotalPDUSize,
					m_iItemTimesSelfOutputPDUSize,
					m_iItemTimesSelfInputPDUSize,
					m_iItemTimesGeneralData,
					m_iItemTimesService,
					m_iItemTimesCacheHit,
					m_iItemTimesRealCSType;

	ClItemIndex		m_iItemDBTimesOperation,
					m_iItemDBTimesIsInternal,
					m_iItemDBTimesStatement,
					m_iItemDBTimesPrepareTime,
					m_iItemDBTimesExecuteTime,
					m_iItemDBTimesFetchTime,
					m_iItemDBTimesNumData;

	ClItemIndex		m_iItemSQLStatement,
					m_iItemSQLStmLength,
					m_iItemSQLOperation,
					m_iItemSQLIsInternal,
					m_iItemSQLNumPrepare,
					m_iItemSQLNumExecute,
					m_iItemSQLNumFetch,
					m_iItemSQLTotalTime,
					m_iItemSQLPrepareTime,
					m_iItemSQLExecuteTime,
					m_iItemSQLFetchTime,
					m_iItemSQLAvgTotalTime,
					m_iItemSQLAvgPrepareTime,
					m_iItemSQLAvgExecuteTime,
					m_iItemSQLAvgFetchTime,
					m_iItemSQLLastTotalTime,
					m_iItemSQLLastPrepareTime,
					m_iItemSQLLastExecuteTime,
					m_iItemSQLLastFetchTime;

	ClItemIndex		m_iItemHistoryIdT3, 
					m_iItemHistoryIdNode, 
					m_iItemHistoryIdItem,
					m_iItemHistoryIdRule,
					m_iItemHistoryRealCSType,
					m_iItemHistoryParentIdT3, 
					m_iItemHistoryParentIdNode, 
					m_iItemHistoryParentIdItem,
					m_iItemHistoryParentIdRule;

	ClItemIndex		m_iItemHistoryParamsParam, 
					m_iItemTimesParamsParam; 
};

#endif


