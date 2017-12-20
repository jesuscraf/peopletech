//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             metachn.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:27/01/98
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================


#include "apichni.h"

#include "metachn.hmg"
#include "metachn.h"

const m4char_t * ClMIT_MetaChn::m_pcNodeNames[ClMIT_MetaChn::NODE_COUNT] =
{
	"SYS_T3S",
	"SYS_NODES",
	"SYS_CONNECTORS",
	"SYS_CONNECTOR_ITEM",
	"SYS_CONNECTOR_ARGS",
	"SYS_T3_CONNECTORS",
	"SYS_T3_CONNECT_ITEM",
	"SYS_TIS",
	"SYS_ITEMS",
	"SYS_ITEM_CSDESC",
	"SYS_ITEM_ARGS",
	"SYS_RULES",
	"SYS_SENTENCES",
	"SYS_RSMS",
	"SYS_ALIAS_RES",
	"SYS_T3_ALIAS_RES",
	"SYS_INDEXES",
	"SYS_INDEX_ITEMS",
	"SYS_FILTERS",
	"SYS_ITEM_TRIGGERS"
} ;

const ClMIT_MetaChn::StIndexName ClMIT_MetaChn::m_pstT3S_Names[ClMIT_MetaChn::T3S_COUNT] = 
{
	{ ClMIT_MetaChn::T3S_DT_START, "DT_START" },
	{ ClMIT_MetaChn::T3S_DT_END,   "DT_END" },
	{ ClMIT_MetaChn::T3S_DT_CORR,  "DT_CORR" },
	{ ClMIT_MetaChn::T3S_ID_T3,  "ID_T3" },
	{ ClMIT_MetaChn::T3S_N_T3,  "N_T3*" },
	{ ClMIT_MetaChn::T3S_OLE_CLASS_ID,  "OLE_CLASS_ID" },
	{ ClMIT_MetaChn::T3S_ID_SERVICE,  "ID_SERVICE" },
	{ ClMIT_MetaChn::T3S_ID_SERVICE_ALT,  "ID_SERVICE_ALT" },
	{ ClMIT_MetaChn::T3S_ID_STREAM_TYPE,  "ID_STREAM_TYPE" },
	{ ClMIT_MetaChn::T3S_IS_SEPARABLE,  "IS_SEPARABLE" },
	{ ClMIT_MetaChn::T3S_CS_EXE_TYPE,  "CS_EXE_TYPE" },
	{ ClMIT_MetaChn::T3S_VM_CAPACITY_REQ,  "VM_CAPACITY_REQ" },
	{ ClMIT_MetaChn::T3S_ENVIRONMENT_VARS,  "ENVIRONMENT_VARS" },
	{ ClMIT_MetaChn::T3S_IS_EXTERNAL,  "IS_EXTERNAL" },
	{ ClMIT_MetaChn::T3S_HAS_SECURITY,  "HAS_SECURITY" },
	{ ClMIT_MetaChn::T3S_IS_CACHEABLE,  "IS_CACHEABLE" },
	{ ClMIT_MetaChn::T3S_IS_MD_CACHEABLE,  "IS_MD_CACHEABLE" },
	{ ClMIT_MetaChn::T3S_CACHE_MAX_PERIOD,  "CACHE_MAX_PERIOD" },
	{ ClMIT_MetaChn::T3S_VERSION,  "VERSION" }
} ;


const ClMIT_MetaChn::StIndexName ClMIT_MetaChn::m_pstSYS_NODES_Names[ClMIT_MetaChn::SYS_NODES_COUNT] = 
{
	{ ClMIT_MetaChn::NODES_ID_T3, "ID_T3" },
	{ ClMIT_MetaChn::NODES_ID_TI, "ID_TI" },
	{ ClMIT_MetaChn::NODES_ID_NODE, "ID_NODE" },
	{ ClMIT_MetaChn::NODES_N_NODE, "N_NODE*" },
	{ ClMIT_MetaChn::NODES_OLE_NODE_INTER_ID, "OLE_NODE_INTER_ID" },
	{ ClMIT_MetaChn::NODES_OLE_DISP_ID, "OLE_DISP_ID" },
	{ ClMIT_MetaChn::NODES_ID_CAPACITY_TYPE, "ID_CAPACITY_TYPE" },
	{ ClMIT_MetaChn::NODES_IS_ROOT, "IS_ROOT" },
	{ ClMIT_MetaChn::NODES_AUTOLOAD, "AUTOLOAD" },
	{ ClMIT_MetaChn::NODES_NUM_ROWS, "NUM_ROWS" },
	{ ClMIT_MetaChn::NODES_NUM_ROWS_DB, "NUM_ROWS_DB" },
	{ ClMIT_MetaChn::NODES_NUM_KEEP_ROWS, "NUM_KEEP_ROWS" },
	{ ClMIT_MetaChn::NODES_IS_VISIBLE, "IS_VISIBLE" },
	{ ClMIT_MetaChn::NODES_DYN_FILTER, "DYN_FILTER" },
	{ ClMIT_MetaChn::NODES_ID_AUTO_FILTER, "ID_AUTO_FILTER" },
	{ ClMIT_MetaChn::NODES_ID_CSTYPE, "ID_CSTYPE" },
	{ ClMIT_MetaChn::NODES_ID_SYNC_TYPE, "ID_SYNC_TYPE" },
	{ ClMIT_MetaChn::NODES_IS_EXTERNAL, "IS_EXTERNAL" }
} ;

const ClMIT_MetaChn::StIndexName ClMIT_MetaChn::m_pstSYS_CONN_Names[ClMIT_MetaChn::SYS_CONN_COUNT] = 
{
	{ ClMIT_MetaChn::CONN_ID_T3, "ID_T3" },
	{ ClMIT_MetaChn::CONN_ID_NODE_USED, "ID_NODE_USED" },
	{ ClMIT_MetaChn::CONN_ID_NODE, "ID_NODE" },
	{ ClMIT_MetaChn::CONN_ID_CONNECTION_TYPE, "ID_CONNECTION_TYPE" },
	{ ClMIT_MetaChn::CONN_EXPAND_DELETE, "EXPAND_DELETE" },
	{ ClMIT_MetaChn::CONN_ID_SENTENCE, "ID_SENTENCE" }
} ;

const ClMIT_MetaChn::StIndexName ClMIT_MetaChn::m_pstSYS_CONN_ITEM_Names[ClMIT_MetaChn::SYS_CONN_ITEM_COUNT] = 
{
	{ ClMIT_MetaChn::CONN_ITEM_ID_T3, "ID_T3" },
	{ ClMIT_MetaChn::CONN_ITEM_ID_NODE_USED, "ID_NODE_USED" },
	{ ClMIT_MetaChn::CONN_ITEM_ID_NODE, "ID_NODE" },
	{ ClMIT_MetaChn::CONN_ITEM_ID_ITEM_USED, "ID_ITEM_USED" },
	{ ClMIT_MetaChn::CONN_ITEM_ID_ITEM, "ID_ITEM" },
	{ ClMIT_MetaChn::CONN_ITEM_ID_PRECEDENCE_TYPE, "ID_PRECEDENCE_TYPE" },
	{ ClMIT_MetaChn::CONN_ITEM_ID_SPIN_TYPE, "ID_SPIN_TYPE" },
	{ ClMIT_MetaChn::CONN_ITEM_CALL_ORDER, "CALL_ORDER" },
	{ ClMIT_MetaChn::CONN_ITEM_ID_RELSHIP_TYPE, "ID_RELSHIP_TYPE" },
	{ ClMIT_MetaChn::CONN_ITEM_ID_CONTEXT_TYPE, "ID_CONTEXT_TYPE" },
	{ ClMIT_MetaChn::CONN_ITEM_ID_CSTYPE, "ID_CSTYPE" }
} ;

const ClMIT_MetaChn::StIndexName ClMIT_MetaChn::m_pstSYS_CONN_ARGS_Names[ClMIT_MetaChn::SYS_CONN_ARGS_COUNT] = 
{
	{ ClMIT_MetaChn::CONN_ARGS_ID_T3, "ID_T3" },
	{ ClMIT_MetaChn::CONN_ARGS_ID_NODE_USED, "ID_NODE_USED" },
	{ ClMIT_MetaChn::CONN_ARGS_ID_NODE, "ID_NODE" },
	{ ClMIT_MetaChn::CONN_ARGS_ID_ITEM_USED, "ID_ITEM_USED" },
	{ ClMIT_MetaChn::CONN_ARGS_ID_ITEM, "ID_ITEM" },
	{ ClMIT_MetaChn::CONN_ARGS_ID_ARG_USED, "ID_ARG_USED" },
	{ ClMIT_MetaChn::CONN_ARGS_ID_ARG, "ID_ARG" },
	{ ClMIT_MetaChn::CONN_ARGS_ID_ARG_VAL_USED_TYPE, "ID_ARG_VAL_USED_TYPE" }
} ;

const ClMIT_MetaChn::StIndexName ClMIT_MetaChn::m_pstSYS_T3_CONN_Names[ClMIT_MetaChn::SYS_T3_CONN_COUNT] = 
{
	{ ClMIT_MetaChn::T3_CONN_ID_T3_USED, "ID_T3_USED" },
	{ ClMIT_MetaChn::T3_CONN_ID_T3, "ID_T3" },
	{ ClMIT_MetaChn::T3_CONN_ID_NODE_USED, "ID_NODE_USED" },
	{ ClMIT_MetaChn::T3_CONN_ID_NODE, "ID_NODE" },
	{ ClMIT_MetaChn::T3_CONN_ID_CONNECTION_TYPE, "ID_CONNECTION_TYPE" },
	{ ClMIT_MetaChn::T3_CONN_IS_FILTER, "IS_FILTER" },
	{ ClMIT_MetaChn::T3_CONN_ID_RELATION_TYPE, "ID_RELATION_TYPE" }
} ;


const ClMIT_MetaChn::StIndexName ClMIT_MetaChn::m_pstSYS_T3_CONN_ITEM_Names[ClMIT_MetaChn::SYS_T3_CONN_ITEM_COUNT] = 
{
	{ ClMIT_MetaChn::T3_CONN_ITEM_ID_T3_USED, "ID_T3_USED" },
	{ ClMIT_MetaChn::T3_CONN_ITEM_ID_T3, "ID_T3" },
	{ ClMIT_MetaChn::T3_CONN_ITEM_ID_NODE_USED, "ID_NODE_USED" },
	{ ClMIT_MetaChn::T3_CONN_ITEM_ID_NODE, "ID_NODE" },
	{ ClMIT_MetaChn::T3_CONN_ITEM_ID_ITEM_USED, "ID_ITEM_USED" },
	{ ClMIT_MetaChn::T3_CONN_ITEM_ID_ITEM, "ID_ITEM" },
	{ ClMIT_MetaChn::T3_CONN_ITEM_ID_RELSHIP_TYPE, "ID_RELSHIP_TYPE" }
} ;

const ClMIT_MetaChn::StIndexName ClMIT_MetaChn::m_pstSYS_TIS_Names[ClMIT_MetaChn::SYS_TIS_COUNT] = 
{
	{ ClMIT_MetaChn::TIS_ID_T3, "ID_T3" },
	{ ClMIT_MetaChn::TIS_ID_TI, "ID_TI" },
	/*{ ClMIT_MetaChn::TIS_ID_TI_BASE, "ID_TI_BASE" },*/
	{ ClMIT_MetaChn::TIS_ID_INHERIT_TYPE, "ID_INHERIT_TYPE" },
	{ ClMIT_MetaChn::TIS_ID_READ_SENTENCE, "ID_READ_SENTENCE" },
	{ ClMIT_MetaChn::TIS_ID_WRITE_SENTENCE, "ID_WRITE_SENTENCE" },
	{ ClMIT_MetaChn::TIS_ID_READ_OBJECT, "ID_READ_OBJECT" },
	{ ClMIT_MetaChn::TIS_ID_WRITE_OBJECT, "ID_WRITE_OBJECT" },
	{ ClMIT_MetaChn::TIS_ID_TEMPORARY, "ID_TEMPORARY" },
	{ ClMIT_MetaChn::TIS_COMPLETE, "COMPLETE" },
	{ ClMIT_MetaChn::TIS_CORRECTED, "CORRECTED" }
} ;

const ClMIT_MetaChn::StIndexName ClMIT_MetaChn::m_pstSYS_ITEMS_Names[ClMIT_MetaChn::SYS_ITEMS_COUNT] = 
{
	{ ClMIT_MetaChn::ITEMS_ID_T3, "ID_T3" },
	{ ClMIT_MetaChn::ITEMS_ID_TI, "ID_TI" },
	{ ClMIT_MetaChn::ITEMS_ID_ITEM, "ID_ITEM" },
	{ ClMIT_MetaChn::ITEMS_ID_SYNONYM, "ID_SYNONYM" },
	{ ClMIT_MetaChn::ITEMS_N_ITEM, "N_ITEM*" },
	{ ClMIT_MetaChn::ITEMS_DEFAULT_VALUE, "DEFAULT_VALUE" },
	{ ClMIT_MetaChn::ITEMS_ID_ITEM_TYPE, "ID_ITEM_TYPE" },
	{ ClMIT_MetaChn::ITEMS_ID_SCOPE_TYPE, "ID_SCOPE_TYPE" },
	{ ClMIT_MetaChn::ITEMS_ID_M4_TYPE, "ID_M4_TYPE" },
	{ ClMIT_MetaChn::ITEMS_PREC, "PREC" },
	{ ClMIT_MetaChn::ITEMS_SCALE, "SCALE" },
	{ ClMIT_MetaChn::ITEMS_ITEM_ORDER, "ITEM_ORDER" },
	{ ClMIT_MetaChn::ITEMS_ID_DMD, "ID_DMD" },
	{ ClMIT_MetaChn::ITEMS_ID_DMD_COMPONENT, "ID_DMD_COMPONENT" },
	{ ClMIT_MetaChn::ITEMS_ID_DMD_CROSSTAB, "ID_DMD_CROSSTAB" },
	{ ClMIT_MetaChn::ITEMS_ID_DMD_FIELD, "ID_DMD_FIELD" },
	{ ClMIT_MetaChn::ITEMS_ID_SLICE_BHVR_TYPE, "ID_SLICE_BHVR_TYPE" },
	{ ClMIT_MetaChn::ITEMS_ID_SLICE_SPLIT_TP, "ID_SLICE_SPLIT_TP" },
	{ ClMIT_MetaChn::ITEMS_ID_SLICE_TOT_TYPE, "ID_SLICE_TOT_TYPE" },
	{ ClMIT_MetaChn::ITEMS_ID_TOTALIZE_TYPE, "ID_TOTALIZE_TYPE" },
	{ ClMIT_MetaChn::ITEMS_ID_TRANSACT_TYPE, "ID_TRANSACT_TYPE" },
	{ ClMIT_MetaChn::ITEMS_ID_EXE_LEVEL, "ID_EXE_LEVEL" },
	{ ClMIT_MetaChn::ITEMS_EXE_GROUP, "EXE_GROUP" },
	{ ClMIT_MetaChn::ITEMS_ID_SLICE_ITEM, "ID_SLICE_ITEM" },
	{ ClMIT_MetaChn::ITEMS_ID_ITEM_AUX, "ID_ITEM_AUX" },
	{ ClMIT_MetaChn::ITEMS_ALIAS_READ_OBJECT, "ALIAS_READ_OBJECT" },
	{ ClMIT_MetaChn::ITEMS_ALIAS_WRITE_OBJECT, "ALIAS_WRITE_OBJECT" },
	{ ClMIT_MetaChn::ITEMS_ID_READ_FIELD, "ID_READ_FIELD" },
	{ ClMIT_MetaChn::ITEMS_ID_WRITE_FIELD, "ID_WRITE_FIELD" },
	{ ClMIT_MetaChn::ITEMS_ID_READ_OBJECT, "ID_READ_OBJECT" },
	{ ClMIT_MetaChn::ITEMS_ID_WRITE_OBJECT, "ID_WRITE_OBJECT" },
	{ ClMIT_MetaChn::ITEMS_ID_SENT_TOT_TYPE, "ID_SENT_TOT_TYPE" },
	{ ClMIT_MetaChn::ITEMS_VARIABLE_ARGUMENTS, "VARIABLE_ARGUMENTS" },
	{ ClMIT_MetaChn::ITEMS_IS_VISIBLE, "IS_VISIBLE" },
	{ ClMIT_MetaChn::ITEMS_MODIFY_OTHER_ITEMS, "MODIFY_OTHER_ITEMS" },
	{ ClMIT_MetaChn::ITEMS_ID_INTERNAL_TYPE, "ID_INTERNAL_TYPE" },
	{ ClMIT_MetaChn::ITEMS_ID_CSTYPE, "ID_CSTYPE" },
	{ ClMIT_MetaChn::ITEMS_ID_SYNC_TYPE, "ID_SYNC_TYPE" },
	{ ClMIT_MetaChn::ITEMS_IS_PUBLIC, "IS_PUBLIC" },
	{ ClMIT_MetaChn::ITEMS_IS_PARAMETER, "IS_PARAMETER" },
	{ ClMIT_MetaChn::ITEMS_CHECK_NOT_NULL, "CHECK_NOT_NULL" },
	{ ClMIT_MetaChn::ITEMS_CLIENT_NOT_NULL, "CLIENT_NOT_NULL" }

} ;


const ClMIT_MetaChn::StIndexName ClMIT_MetaChn::m_pstSYS_ITEM_CSDESC_Names[ClMIT_MetaChn::SYS_ITEM_CSDESC_COUNT] = 
{
	{ ClMIT_MetaChn::ITEM_CSDESC_ID_T3, "ID_T3" },
	{ ClMIT_MetaChn::ITEM_CSDESC_ID_NODE, "ID_NODE" },
	{ ClMIT_MetaChn::ITEM_CSDESC_ID_ITEM, "ID_ITEM" },
	{ ClMIT_MetaChn::ITEM_CSDESC_ID_CSTYPE, "ID_CSTYPE" },
	{ ClMIT_MetaChn::ITEM_CSDESC_ID_SYNC_TYPE, "ID_SYNC_TYPE" }
} ;


const ClMIT_MetaChn::StIndexName ClMIT_MetaChn::m_pstSYS_ITEM_ARGS_Names[ClMIT_MetaChn::SYS_ITEM_ARGS_COUNT] = 
{
	{ ClMIT_MetaChn::ITEM_ARGS_ID_T3, "ID_T3" },
	{ ClMIT_MetaChn::ITEM_ARGS_ID_TI, "ID_TI" },
	{ ClMIT_MetaChn::ITEM_ARGS_ID_ITEM, "ID_ITEM" },
	{ ClMIT_MetaChn::ITEM_ARGS_ID_ARGUMENT, "ID_ARGUMENT" },
	{ ClMIT_MetaChn::ITEM_ARGS_ID_ARGUMENT_TYPE, "ID_ARGUMENT_TYPE" },
	{ ClMIT_MetaChn::ITEM_ARGS_ID_M4_TYPE, "ID_M4_TYPE" },
	{ ClMIT_MetaChn::ITEM_ARGS_PREC, "PREC" },
	{ ClMIT_MetaChn::ITEM_ARGS_SCALE, "SCALE" },
	{ ClMIT_MetaChn::ITEM_ARGS_POSITION, "POSITION" }
} ;



const ClMIT_MetaChn::StIndexName ClMIT_MetaChn::m_pstSYS_RULES_Names[ClMIT_MetaChn::SYS_RULES_COUNT] = 
{
	{ ClMIT_MetaChn::SYS_RULES_CHANNEL_DT_START, "CHANNEL_DT_START" },
	{ ClMIT_MetaChn::SYS_RULES_CHANNEL_DT_END, "CHANNEL_DT_END" },
	{ ClMIT_MetaChn::SYS_RULES_CHANNEL_DT_CORR, "CHANNEL_DT_CORR" },
	{ ClMIT_MetaChn::SYS_RULES_ID_T3, "ID_T3" },
	{ ClMIT_MetaChn::SYS_RULES_ID_TI, "ID_TI" },
	{ ClMIT_MetaChn::SYS_RULES_ID_ITEM, "ID_ITEM" },
	{ ClMIT_MetaChn::SYS_RULES_ID_RULE, "ID_RULE" },
	{ ClMIT_MetaChn::SYS_RULES_DT_START, "DT_START" },
	{ ClMIT_MetaChn::SYS_RULES_DT_END, "DT_END" },
	{ ClMIT_MetaChn::SYS_RULES_DT_START_CORR, "DT_START_CORR" },
	{ ClMIT_MetaChn::SYS_RULES_DT_END_CORR, "DT_END_CORR" },
	{ ClMIT_MetaChn::SYS_RULES_IS_METARULE, "IS_METARULE" },
	{ ClMIT_MetaChn::SYS_RULES_POLISH_CODE, "POLISH_CODE" },
	{ ClMIT_MetaChn::SYS_RULES_ID_CODE_TYPE, "ID_CODE_TYPE" },
	{ ClMIT_MetaChn::SYS_RULES_ID_RESOURCE, "ID_RESOURCE" },
	{ ClMIT_MetaChn::SYS_RULES_ID_METHOD, "ID_METHOD" },
	{ ClMIT_MetaChn::SYS_RULES_ID_PRIORITY, "ID_PRIORITY" },
	{ ClMIT_MetaChn::SYS_RULES_IS_CORRECT_RULE, "IS_CORRECT_RULE" }
} ;

const ClMIT_MetaChn::StIndexName ClMIT_MetaChn::m_pstSYS_SENTENCES_Names[ClMIT_MetaChn::SYS_SENTENCES_COUNT] = 
{
	{ ClMIT_MetaChn::SYS_SENTENCES_ID_T3, "ID_T3" },
	{ ClMIT_MetaChn::SYS_SENTENCES_ID_SENTENCE, "ID_SENTENCE" },
	{ ClMIT_MetaChn::SYS_SENTENCES_APISQL, "APISQL" },
	{ ClMIT_MetaChn::SYS_SENTENCES_MAX_NUM_COL, "MAX_NUM_COL" }
} ;


const ClMIT_MetaChn::StIndexName ClMIT_MetaChn::m_pstSYS_RSMS_Names[ClMIT_MetaChn::SYS_RSMS_COUNT] = 
{
	{ ClMIT_MetaChn::SYS_RSMS_ID_T3, "ID_T3" },
	{ ClMIT_MetaChn::SYS_RSMS_ID_RSM, "ID_RSM" },
	{ ClMIT_MetaChn::SYS_RSMS_ID_MASK, "ID_MASK" }
} ;

const ClMIT_MetaChn::StIndexName ClMIT_MetaChn::m_pstSYS_ALIAS_RES_Names[ClMIT_MetaChn::SYS_ALIAS_RES_COUNT] = 
{
	{ ClMIT_MetaChn::SYS_ALIAS_RES_ID_T3, "ID_T3" },
	{ ClMIT_MetaChn::SYS_ALIAS_RES_ID_NODE, "ID_NODE" },
	{ ClMIT_MetaChn::SYS_ALIAS_RES_ALIAS, "ALIAS" },
	{ ClMIT_MetaChn::SYS_ALIAS_RES_ID_NODE_USED, "ID_NODE_USED" },
} ;

const ClMIT_MetaChn::StIndexName ClMIT_MetaChn::m_pstSYS_T3_ALIAS_RES_Names[ClMIT_MetaChn::SYS_T3_ALIAS_RES_COUNT] = 
{
	{ ClMIT_MetaChn::SYS_T3_ALIAS_RES_ID_T3, "ID_T3" },
	{ ClMIT_MetaChn::SYS_T3_ALIAS_RES_ID_NODE, "ID_NODE" },
	{ ClMIT_MetaChn::SYS_T3_ALIAS_RES_ALIAS, "ALIAS" },
	{ ClMIT_MetaChn::SYS_T3_ALIAS_RES_ID_T3_USED, "ID_T3_USED" },
	{ ClMIT_MetaChn::SYS_T3_ALIAS_RES_ID_RSM, "ID_ROLE" },
	{ ClMIT_MetaChn::SYS_T3_ALIAS_RES_INSTANCE, "INSTANCE" },
	{ ClMIT_MetaChn::SYS_T3_ALIAS_RES_ID_SHARED_POLICY, "ID_SHARED_POLICY" },
	{ ClMIT_MetaChn::SYS_T3_ALIAS_RES_ID_OPEN_MODE_TP, "ID_OPEN_MODE_TP" },
	{ ClMIT_MetaChn::SYS_T3_ALIAS_RES_CS_ACCESS, "CS_ACCESS" }
} ;


const ClMIT_MetaChn::StIndexName ClMIT_MetaChn::m_pstSYS_INDEXES_Names[ClMIT_MetaChn::SYS_INDEXES_COUNT] = 
{
	{ ClMIT_MetaChn::SYS_INDEXES_ID_T3, "ID_T3" },
	{ ClMIT_MetaChn::SYS_INDEXES_ID_NODE, "ID_NODE" },
	{ ClMIT_MetaChn::SYS_INDEXES_ID_INDEX, "ID_INDEX" },
	{ ClMIT_MetaChn::SYS_INDEXES_IS_DEFAULT_INDEX, "IS_DEFAULT_INDEX" }
} ;	


const ClMIT_MetaChn::StIndexName ClMIT_MetaChn::m_pstSYS_INDEX_ITEMS_Names[ClMIT_MetaChn::SYS_INDEX_ITEMS_COUNT] = 
{
	{ ClMIT_MetaChn::SYS_INDEX_ITEMS_ID_T3, "ID_T3" },
	{ ClMIT_MetaChn::SYS_INDEX_ITEMS_ID_NODE, "ID_NODE" },
	{ ClMIT_MetaChn::SYS_INDEX_ITEMS_ID_INDEX, "ID_INDEX" },
	{ ClMIT_MetaChn::SYS_INDEX_ITEMS_ID_ITEM, "ID_ITEM" },
	{ ClMIT_MetaChn::SYS_INDEX_ITEMS_ID_FUNCTION, "ID_FUNCTION" },
	{ ClMIT_MetaChn::SYS_INDEX_ITEMS_WAY, "WAY" }
} ;	

const ClMIT_MetaChn::StIndexName ClMIT_MetaChn::m_pstSYS_FILTERS_Names[ClMIT_MetaChn::SYS_FILTERS_COUNT] = 
{
	{ ClMIT_MetaChn::SYS_FILTERS_ID_T3, "ID_T3" },
	{ ClMIT_MetaChn::SYS_FILTERS_ID_NODE, "ID_NODE" },
	{ ClMIT_MetaChn::SYS_FILTERS_ID_FILTER, "ID_FILTER" },
	{ ClMIT_MetaChn::SYS_FILTERS_ID_T3_FILTER, "ID_T3_FILTER" },
	{ ClMIT_MetaChn::SYS_FILTERS_ID_NODE_FILTER, "ID_NODE_FILTER" },
	{ ClMIT_MetaChn::SYS_FILTERS_FILTER_TYPE, "FILTER_TYPE" }
} ;

const ClMIT_MetaChn::StIndexName ClMIT_MetaChn::m_pstSYS_ITEM_TRIGGERS_Names[ClMIT_MetaChn::SYS_ITEM_TRIGGERS_COUNT] = 
{
	{ ClMIT_MetaChn::SYS_ITEM_TRIGGERS_ID_T3, "ID_T3" },
	{ ClMIT_MetaChn::SYS_ITEM_TRIGGERS_ID_TI, "ID_TI" },
	{ ClMIT_MetaChn::SYS_ITEM_TRIGGERS_ID_ITEM_USED, "ID_ITEM_USED" },
	{ ClMIT_MetaChn::SYS_ITEM_TRIGGERS_ID_ITEM, "ID_ITEM" },
	{ ClMIT_MetaChn::SYS_ITEM_TRIGGERS_ID_PRECEDENCE_TYPE, "ID_PRECEDENCE_TYPE" },
	{ ClMIT_MetaChn::SYS_ITEM_TRIGGERS_ID_SPIN_TYPE, "ID_SPIN_TYPE" },
	{ ClMIT_MetaChn::SYS_ITEM_TRIGGERS_CALL_ORDER, "CALL_ORDER" },
	{ ClMIT_MetaChn::SYS_ITEM_TRIGGERS_ID_RELSHIP_TYPE, "ID_RELSHIP_TYPE" },
	{ ClMIT_MetaChn::SYS_ITEM_TRIGGERS_ID_CONTEXT_TYPE, "ID_CONTEXT_TYPE" },
	{ ClMIT_MetaChn::SYS_ITEM_TRIGGERS_ID_CSTYPE, "ID_CSTYPE" },
} ;

//==============================================================================ClMIT_MetaChn

ClMIT_MetaChn::ClMIT_MetaChn() 
{
}

void ClMIT_MetaChn::End() 
{
	ClMIT_Chn::End() ;
}

m4bool_t ClMIT_MetaChn::Init( METACHN_MODE ai_Mode, m4int16_t ai_iLanguage, ClAccess *ai_poAccess,  ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect )
{
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t iMetadataLanguage=-1;
	
	ClMIT_MetaChn::End() ;

	m_iLanguage = ai_iLanguage ;
	
	switch (ai_Mode) {
	case OWN_METACHN:
		SwOk=InitOwn("MetaChn", iMetadataLanguage, ClMIT_Trace::PROPAGATE_ERROR, NULL, M4_TRUE) ;
		break ;
	case COMMON_ACCESS:
		SwOk=InitCommonAccess(ai_poAccess,ClMIT_Trace::PROPAGATE_ERROR) ;
		break ;
	case PRIVATE_ACCESS:
		SwOk=InitPrivateAccess(ai_poAccess,ClMIT_Trace::PROPAGATE_ERROR) ;
		break ;
	} ;
	if (!SwOk ) goto exit ;

	SwOk= M4_BOOL( SwOk && InitNodes() );
	if (!SwOk ) goto exit ;

	SwOk = M4_BOOL( SwOk && InitItemArray(SYS_T3S,m_poT3S,m_pstT3S_Names,T3S_COUNT));
	SwOk = M4_BOOL( SwOk && InitItemArray(SYS_NODES,m_poSYS_NODES,m_pstSYS_NODES_Names,SYS_NODES_COUNT));
	SwOk = M4_BOOL( SwOk && InitItemArray(SYS_CONNECTORS,m_poSYS_CONN,m_pstSYS_CONN_Names,SYS_CONN_COUNT));
	SwOk = M4_BOOL( SwOk && InitItemArray(SYS_CONNECTOR_ITEM,m_poSYS_CONN_ITEM,m_pstSYS_CONN_ITEM_Names,SYS_CONN_ITEM_COUNT));
	SwOk = M4_BOOL( SwOk && InitItemArray(SYS_CONNECTOR_ARGS,m_poSYS_CONN_ARGS,m_pstSYS_CONN_ARGS_Names,SYS_CONN_ARGS_COUNT));
	SwOk = M4_BOOL( SwOk && InitItemArray(SYS_T3_CONNECTORS,m_poSYS_T3_CONN,m_pstSYS_T3_CONN_Names,SYS_T3_CONN_COUNT));
	SwOk = M4_BOOL( SwOk && InitItemArray(SYS_T3_CONNECTOR_ITEM,m_poSYS_T3_CONN_ITEM,m_pstSYS_T3_CONN_ITEM_Names,SYS_T3_CONN_ITEM_COUNT));
	SwOk = M4_BOOL( SwOk && InitItemArray(SYS_TIS,m_poSYS_TIS,m_pstSYS_TIS_Names,SYS_TIS_COUNT));
	SwOk = M4_BOOL( SwOk && InitItemArray(SYS_ITEMS,m_poSYS_ITEMS,m_pstSYS_ITEMS_Names,SYS_ITEMS_COUNT));
	SwOk = M4_BOOL( SwOk && InitItemArray(SYS_ITEM_CSDESC,m_poSYS_ITEM_CSDESC,m_pstSYS_ITEM_CSDESC_Names,SYS_ITEM_CSDESC_COUNT));
	SwOk = M4_BOOL( SwOk && InitItemArray(SYS_ITEM_ARGS,m_poSYS_ITEM_ARGS,m_pstSYS_ITEM_ARGS_Names,SYS_ITEM_ARGS_COUNT));
	SwOk = M4_BOOL( SwOk && InitItemArray(SYS_RULES,m_poSYS_RULES,m_pstSYS_RULES_Names,SYS_RULES_COUNT));
	SwOk = M4_BOOL( SwOk && InitItemArray(SYS_SENTENCES,m_poSYS_SENTENCES,m_pstSYS_SENTENCES_Names,SYS_SENTENCES_COUNT));
	SwOk = M4_BOOL( SwOk && InitItemArray(SYS_RSMS,m_poSYS_RSMS,m_pstSYS_RSMS_Names,SYS_RSMS_COUNT));
	SwOk = M4_BOOL( SwOk && InitItemArray(SYS_ALIAS_RES,m_poSYS_ALIAS_RES,m_pstSYS_ALIAS_RES_Names,SYS_ALIAS_RES_COUNT));
	SwOk = M4_BOOL( SwOk && InitItemArray(SYS_T3_ALIAS_RES,m_poSYS_T3_ALIAS_RES,m_pstSYS_T3_ALIAS_RES_Names,SYS_T3_ALIAS_RES_COUNT));
	SwOk = M4_BOOL( SwOk && InitItemArray(SYS_INDEXES,m_poSYS_INDEXES,m_pstSYS_INDEXES_Names,SYS_INDEXES_COUNT));
	SwOk = M4_BOOL( SwOk && InitItemArray(SYS_INDEX_ITEMS,m_poSYS_INDEX_ITEMS,m_pstSYS_INDEX_ITEMS_Names,SYS_INDEX_ITEMS_COUNT));
	//SwOk = M4_BOOL( SwOk && InitItemArray(SYS_FILTERS,m_poSYS_FILTERS,m_pstSYS_FILTERS_Names,SYS_FILTERS_COUNT));
	SwOk = M4_BOOL( SwOk && InitItemArray(SYS_ITEM_TRIGGERS,m_poSYS_ITEM_TRIGGERS,m_pstSYS_ITEM_TRIGGERS_Names,SYS_ITEM_TRIGGERS_COUNT));


exit:
	if (!SwOk) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ METACHN_MSG_10 ] ) << SEND_MSG ;
	} ;

	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; //Aborta si efecto inmediato

	return SwOk ;
}



m4bool_t ClMIT_MetaChn::InitItemArray( 
	METACHN_NODES ai_Node, ClMIT_ChnItem *ai_poItemArr, const StIndexName *ai_pstNameArr, m4int16_t ai_iNItems ) 
{
	m4int16_t B ;
	m4bool_t SwOk=M4_TRUE ;
	ClMIT_ChnItem::INIT_RETURN ItemR ;
	ClMIT_Str StrAux ;
	size_t IndexLastChar ;

	m_pstNodeItemInfo[ai_Node].m_poItemArr = ai_poItemArr ; 
	m_pstNodeItemInfo[ai_Node].m_szItemArry = ai_iNItems ; 
	
	for (B=0; B<ai_iNItems; ++B) {

		IndexLastChar = strlen(ai_pstNameArr[B].m_pcName)-1 ;
		if ( ai_pstNameArr[B].m_pcName[IndexLastChar] == '*' ) {
			StrAux.Accommodate(100) ;
			sprintf(StrAux, "%.*s%i", IndexLastChar, ai_pstNameArr[B].m_pcName, (int)m_iLanguage ) ;
			StrAux.OverStringValid() ;
			ItemR=ai_poItemArr[ ai_pstNameArr[B].m_Id ].Init(m_poNode[ai_Node],StrAux,ClMIT_Trace::PROPAGATE_ERROR) ;
		} else {
			ItemR=ai_poItemArr[ ai_pstNameArr[B].m_Id ].Init(m_poNode[ai_Node],ai_pstNameArr[B].m_pcName,ClMIT_Trace::PROPAGATE_ERROR) ;
		} ;

		if ( ItemR != ClMIT_ChnItem::ITEM_OK ) {
			SwOk=M4_FALSE ;
		} ;
	} ;
	return SwOk ;
}

void ClMIT_MetaChn::SaveItems( METACHN_NODES ai_Node ) 
{
	m4int16_t B ;

	ClMIT_ChnItem *ItemArr = m_pstNodeItemInfo[ai_Node].m_poItemArr ;
	m4int16_t      Size    =  m_pstNodeItemInfo[ai_Node].m_szItemArry ;

	for (B=0; B<Size; ++B) {
		if ( ItemArr[B].m_oValue.IsUpdated() ) ItemArr[B].Save() ;
		ItemArr[B].m_oValue.ClearUpdated() ;
	} ;
}


m4bool_t ClMIT_MetaChn::InitNodes( ) 
{
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t B ;

	for (B=0;B<NODE_COUNT;++B) {
		m_poNode[B] = GetNode(m_pcNodeNames[B],ClMIT_Trace::PROPAGATE_ERROR) ;
		if (!m_poNode[B]) SwOk=M4_FALSE;
	} ;

	return SwOk ;
} 

