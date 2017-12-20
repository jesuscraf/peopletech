
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                m4facdef.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                15-05-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module set the defines for the metachannel
//
//
//==============================================================================



#ifndef __M4FACDEF_HPP__
#define __M4FACDEF_HPP__



//=============================================================================
// Identificadores de los ficheros
//=============================================================================

#define M4MDFAC_FILE_M4LOAD_M2							"m4load"
#define M4MDFAC_FILE_M4SYSTEM_M2						"m4sys"
#define M4MDFAC_FILE_M4SEC_M2							"m4sec"
#define M4MDFAC_FILE_M4DICT_M2							"m4dict"
#define M4MDFAC_FILE_M4TRANS_M2							"m4trans"



//=============================================================================
// Argumentos de items generados
//=============================================================================

#define	M4_MDFAC_SYSTEM_ARGUMENTS_EXECUTEREPORT				0
#define	M4_MDFAC_SYSTEM_ARGUMENTS_EXECUTEREPORT_NUMBER		5
#define	M4_MDFAC_SYSTEM_ARGUMENTS_NUMBER					5


//=============================================================================
// Nombres de items
//=============================================================================


// Ti del sistema
#define M4MDFAC_TI_TI_SYSTEM							"TI_SYSTEM"


// Item de filtro dinámico
#define M4MDFAC_SYSTEM_ITEM_DYN_FILTER					"DYN_FILTER"

// Items de carga y grabación de canal
#define M4MDFAC_SYSTEM_ITEM_SYS_LOAD_CLIENT				"SYS_LOAD"
#define M4MDFAC_SYSTEM_ITEM_SYS_PERSIST_CLIENT			"SYS_PERSIST"
#define M4MDFAC_SYSTEM_ITEM_SYS_LOAD_SERVER				"SYS_LOAD_SERVER"
#define M4MDFAC_SYSTEM_ITEM_SYS_PERSIST_SERVER			"SYS_PERSIST_SERVER"

// Item execute report
#define M4MDFAC_SYSTEM_ITEM_EXECUTEREPORT				"EXECUTEREPORT"


// identificadores de los nodos
#define M4MDFAC_NODE_SYS_T3S							"SYS_T3S"
#define M4MDFAC_NODE_SYS_T3S_INHERIT					"SYS_T3S_INHERIT"
#define M4MDFAC_NODE_SYS_SENTENCES						"SYS_SENTENCES"
#define M4MDFAC_NODE_SYS_RSMS							"SYS_RSMS"
#define M4MDFAC_NODE_SYS_TIS_INHERIT					"SYS_TIS_INHERIT"
#define M4MDFAC_NODE_SYS_TIS							"SYS_TIS"
#define M4MDFAC_NODE_SYS_ITEMS							"SYS_ITEMS"
#define M4MDFAC_NODE_SYS_ITEMS_INHERIT					"SYS_ITEMS_INHERIT"
#define M4MDFAC_NODE_SYS_CONCEPTS						"SYS_CONCEPTS"
#define M4MDFAC_NODE_SYS_ITEM_ARGS						"SYS_ITEM_ARGS"
#define M4MDFAC_NODE_SYS_RULES							"SYS_RULES"
#define M4MDFAC_NODE_SYS_TAGS							"SYS_TAGS"
#define M4MDFAC_NODE_SYS_ITEM_TRIGGERS					"SYS_ITEM_TRIGGERS"
#define M4MDFAC_NODE_SYS_TOTALS							"SYS_TOTALS"
#define M4MDFAC_NODE_SYS_TOTAL_TAGS						"SYS_TOTAL_TAGS"
#define M4MDFAC_NODE_SYS_NODES							"SYS_NODES"
#define M4MDFAC_NODE_SYS_NODES_INHERIT					"SYS_NODES_INHERIT"
#define M4MDFAC_NODE_SYS_NODES_READ						"SYS_NODES_READ"
#define M4MDFAC_NODE_SYS_CONNECTORS						"SYS_CONNECTORS"
#define M4MDFAC_NODE_SYS_CONNECTOR_ITEM					"SYS_CONNECTOR_ITEM"
#define M4MDFAC_NODE_SYS_CONNECTOR_ARGS					"SYS_CONNECTOR_ARGS"
#define M4MDFAC_NODE_SYS_INDEXES						"SYS_INDEXES"
#define M4MDFAC_NODE_SYS_INDEX_ITEMS					"SYS_INDEX_ITEMS"
#define M4MDFAC_NODE_SYS_T3_CONNECTORS					"SYS_T3_CONNECTORS"
#define M4MDFAC_NODE_SYS_T3_CONNECT_ITEM				"SYS_T3_CONNECT_ITEM"
#define M4MDFAC_NODE_SYS_ALIAS_RES						"SYS_ALIAS_RES"
#define M4MDFAC_NODE_SYS_T3_ALIAS_RES					"SYS_T3_ALIAS_RES"
#define M4MDFAC_NODE_SYS_FILTERS						"SYS_FILTERS"
#define M4MDFAC_NODE_SYS_ITEM_CSDESC					"SYS_ITEM_CSDESC"

// Para el xml
#define M4MDFAC_NODE_SYS_BUSINESS_SERVICES				"SYS_BUSINESS_SERVICES"
#define M4MDFAC_NODE_SYS_BUSINESS_METHODS				"SYS_BUSINESS_METHODS"
#define M4MDFAC_NODE_SYS_BUSINESS_ARGS					"SYS_BUSINESS_ARGS"

// Para los lenguajes
#define M4MDFAC_NODE_SYS_T3S_TRANS						"SYS_T3S_TRANS"
#define M4MDFAC_NODE_SYS_ITEMS_INHERIT_TRANS			"SYS_ITEMS_INHERIT_TRANS"
#define M4MDFAC_NODE_SYS_CONCEPTS_TRANS					"SYS_CONCEPTS_TRANS"
#define M4MDFAC_NODE_SYS_NODES_TRANS					"SYS_NODES_TRANS"
#define M4MDFAC_NODE_SYS_T3_CONNECTORS_TRANS			"SYS_T3_CONNECTORS_TRANS"
#define M4MDFAC_NODE_SYS_BUSINESS_METHODS_TRANS			"SYS_BUSINESS_METHODS_TRANS"


// Para la seguridad ==========================================================

#define M4MDFAC_NODE_SYS_T3_MASKS						"SYS_T3_MASK"
#define M4MDFAC_NODE_SYS_NODE_MASK						"SYS_NODE_MASK"
#define M4MDFAC_NODE_SYS_ITEM_MASK						"SYS_ITEM_MASK"
#define M4MDFAC_NODE_SYS_FILTER_MASK					"SYS_FILTER_MASK"
#define M4MDFAC_NODE_SYS_T3_CONNECTOR_MASK				"SYS_T3_CONNECTOR_MASK"


// Para el mapeo
#define M4MDFAC_NODE_MAP_VERSION						"VERSION"
#define M4MDFAC_NODE_MAP_ORGANIZATIONS					"ORGANIZATIONS"
#define M4MDFAC_NODE_MAP_MAPPINGS						"MAPPINGS"



// Identificadores de los items de los nodos ==================================


// para el fichero
#define M4MDFAC_BUILD									"BUILD"



// para las fechas
#define M4MDFAC_ITEM_DT_START							"DT_START"
#define M4MDFAC_ITEM_DT_END								"DT_END"
#define M4MDFAC_ITEM_DT_START_CORR						"DT_START_CORR"
#define M4MDFAC_ITEM_DT_END_CORR						"DT_END_CORR"
#define M4MDFAC_ITEM_DT_CORR							"DT_CORR"
#define M4MDFAC_ITEM_VERSION							"VERSION"

// para T3
#define M4MDFAC_ITEM_ID_T3								"ID_T3"
#define M4MDFAC_ITEM_ID_T3_USED							"ID_T3_USED"
#define M4MDFAC_ITEM_N_T3								"N_T3"
#define M4MDFAC_ITEM_ID_STREAM_TYPE						"ID_STREAM_TYPE"
#define M4MDFAC_ITEM_OLE_CLASS_ID						"OLE_CLASS_ID"
#define M4MDFAC_ITEM_IS_EXTERNAL						"IS_EXTERNAL"
#define M4MDFAC_ITEM_IS_SEPARABLE						"IS_SEPARABLE"
#define M4MDFAC_ITEM_CS_EXE_TYPE						"CS_EXE_TYPE"
#define M4MDFAC_ITEM_VM_CAPACITY_REQ					"VM_CAPACITY_REQ"
#define M4MDFAC_ITEM_HAS_SECURITY						"HAS_SECURITY"
#define M4MDFAC_ITEM_ENVIRONMENT_VARS					"ENVIRONMENT_VARS"
#define M4MDFAC_ITEM_IS_CACHEABLE						"IS_CACHEABLE"
#define M4MDFAC_ITEM_IS_MD_CACHEABLE					"IS_MD_CACHEABLE"
#define M4MDFAC_ITEM_CHECK_CONCURRENCY					"CHECK_CONCURRENCY"
#define M4MDFAC_ITEM_CACHE_MAX_PERIOD					"CACHE_MAX_PERIOD"
#define M4MDFAC_ITEM_OWNER_FLAG							"OWNER_FLAG"
#define M4MDFAC_ITEM_ORGANIZATION_TYPE					"ORGANIZATION_TYPE"
#define M4MDFAC_ITEM_CREATION_TYPE						"CREATION_TYPE"
#define M4MDFAC_ITEM_DATA_LANGUAGE						"DATA_LANGUAGE"
#define M4MDFAC_ITEM_META_LANGUAGE						"META_LANGUAGE"

// Para T3s inherit
#define M4MDFAC_ITEM_ID_T3_BASE							"ID_T3_BASE"


// Para Tis
#define M4MDFAC_ITEM_ID_TI								"ID_TI"
#define M4MDFAC_ITEM_ID_TI_USED							"ID_TI_USED"
#define M4MDFAC_ITEM_ID_TI_BASE							"ID_TI_BASE"
#define M4MDFAC_ITEM_ID_INHERIT_TYPE					"ID_INHERIT_TYPE"
#define M4MDFAC_ITEM_ID_READ_SENTENCE					"ID_READ_SENTENCE"
#define M4MDFAC_ITEM_ID_AUX_READ_SENTENCE				"ID_AUX_READ_SENTENCE"
#define M4MDFAC_ITEM_ID_WRITE_SENTENCE					"ID_WRITE_SENTENCE"
#define M4MDFAC_ITEM_ID_READ_OBJECT						"ID_READ_OBJECT"
#define M4MDFAC_ITEM_ID_WRITE_OBJECT					"ID_WRITE_OBJECT"
#define M4MDFAC_ITEM_ID_TEMPORARY						"ID_TEMPORARY"
#define M4MDFAC_ITEM_COMPLETE							"COMPLETE"
#define M4MDFAC_ITEM_CORRECTED							"CORRECTED"
#define M4MDFAC_ITEM_IS_PUBLIC							"IS_PUBLIC"
#define M4MDFAC_ITEM_IS_PARAMETER						"IS_PARAMETER"
#define M4MDFAC_ITEM_CHECK_NOT_NULL						"CHECK_NOT_NULL"
#define M4MDFAC_ITEM_CLIENT_NOT_NULL					"CLIENT_NOT_NULL"
#define M4MDFAC_ITEM_NOT_NULL							"NOT_NULL"
#define M4MDFAC_ITEM_GENERATE_SQL						"GENERATE_SQL"

// para nodos
#define M4MDFAC_ITEM_ID_NODE							"ID_NODE"
#define M4MDFAC_ITEM_N_NODE								"N_NODE"
#define M4MDFAC_ITEM_ID_NODE_USED						"ID_NODE_USED"
#define M4MDFAC_ITEM_ID_CAPACITY_TYPE					"ID_CAPACITY_TYPE"
#define M4MDFAC_ITEM_IS_ROOT							"IS_ROOT"
#define M4MDFAC_ITEM_AUTOLOAD							"AUTOLOAD"
#define M4MDFAC_ITEM_IS_ORDERED							"IS_ORDERED"
#define M4MDFAC_ITEM_NUM_ROWS							"NUM_ROWS"
#define M4MDFAC_ITEM_NUM_KEEP_ROWS						"NUM_KEEP_ROWS"
#define M4MDFAC_ITEM_NUM_ROWS_DB						"NUM_ROWS_DB"
#define M4MDFAC_ITEM_DYN_FILTER							"DYN_FILTER"
#define M4MDFAC_ITEM_ID_AUTO_FILTER						"ID_AUTO_FILTER"
#define M4MDFAC_ITEM_OLE_DISP_ID						"OLE_DISP_ID"
#define M4MDFAC_ITEM_OLE_NODE_INTER_ID					"OLE_NODE_INTER_ID"
#define M4MDFAC_ITEM_MAX_NUM_BLOCKS						"MAX_NUM_BLOCKS"
#define M4MDFAC_ITEM_MAX_NUM_RECORDS					"MAX_NUM_RECORDS"
#define M4MDFAC_ITEM_ID_CSTYPE							"ID_CSTYPE"
#define M4MDFAC_ITEM_NODES_TYPE							"NODES_TYPE"
#define M4MDFAC_ITEM_POS_NODO							"POS_NODO"
#define M4MDFAC_ITEM_AFFECTS_DB							"AFFECTS_DB"
#define M4MDFAC_ITEM_DB_RELOAD							"DB_RELOAD"


// para alias
#define M4MDFAC_ITEM_ALIAS								"ALIAS"

// para t3s alias
#define M4MDFAC_ITEM_ID_OPEN_MODE_TP					"ID_OPEN_MODE_TP"
#define M4MDFAC_ITEM_ID_SHARED_POLICY					"ID_SHARED_POLICY"
#define M4MDFAC_ITEM_INSTANCE							"INSTANCE"
#define M4MDFAC_ITEM_CS_ACCESS							"CS_ACCESS"
#define M4MDFAC_ITEM_ID_RSM								"ID_RSM"
#define M4MDFAC_ITEM_ID_ROLE							"ID_ROLE"
#define M4MDFAC_ITEM_ID_ORGANIZATION					"ID_ORGANIZATION"

// para conectores
#define M4MDFAC_ITEM_ID_CONNECTION_TYPE					"ID_CONNECTION_TYPE"
#define M4MDFAC_ITEM_EXPAND_DELETE						"EXPAND_DELETE"
#define M4MDFAC_ITEM_EXPAND_UNDO						"EXPAND_UNDO"

// para conectores de t3
#define M4MDFAC_ITEM_IS_FILTER							"IS_FILTER"
#define M4MDFAC_ITEM_ID_RELATION_TYPE					"ID_RELATION_TYPE"
#define M4MDFAC_ITEM_IS_MULTISELECTION					"IS_MULTISELECTION"

// para items
#define M4MDFAC_ITEM_ID_ITEM							"ID_ITEM"
#define M4MDFAC_ITEM_ID_ITEM_USED						"ID_ITEM_USED"
#define M4MDFAC_ITEM_SYNONYM_ID							"SYNONYM_ID"
#define M4MDFAC_ITEM_N_ITEM								"N_ITEM"
#define M4MDFAC_ITEM_ID_ITEM_TYPE						"ID_ITEM_TYPE"
#define M4MDFAC_ITEM_ID_SCOPE_TYPE						"ID_SCOPE_TYPE"
#define M4MDFAC_ITEM_ID_M4_TYPE							"ID_M4_TYPE"
#define M4MDFAC_ITEM_PREC								"PREC"
#define M4MDFAC_ITEM_SCALE								"SCALE"
#define M4MDFAC_ITEM_ID_DMD								"ID_DMD"
#define M4MDFAC_ITEM_ID_DMD_COMPONENT					"ID_DMD_COMPONENT"
#define M4MDFAC_ITEM_ID_DMD_CROSSTAB					"ID_DMD_CROSSTAB"
#define M4MDFAC_ITEM_ID_DMD_FIELD						"ID_DMD_FIELD"
#define M4MDFAC_ITEM_REFRESH_SLICES						"REFRESH_SLICES"
#define M4MDFAC_ITEM_ID_SLICE_BHVR_TYPE					"ID_SLICE_BHVR_TYPE"
#define M4MDFAC_ITEM_ID_SLICE_SPLIT_TP					"ID_SLICE_SPLIT_TP"
#define M4MDFAC_ITEM_ID_SLICE_ITEM						"ID_SLICE_ITEM"
#define M4MDFAC_ITEM_ID_ITEM_AUX						"ID_ITEM_AUX"
#define M4MDFAC_ITEM_ID_ITEM_AUX_PRORAT					"ID_ITEM_AUX_PRORAT"
#define M4MDFAC_ITEM_ID_TOTALIZE_TYPE					"ID_TOTALIZE_TYPE"
#define M4MDFAC_ITEM_ID_SLICE_TOT_TYPE					"ID_SLICE_TOT_TYPE"
#define M4MDFAC_ITEM_ID_SYNC_TYPE						"ID_SYNC_TYPE"
#define M4MDFAC_ITEM_ID_TRANSACT_TYPE					"ID_TRANSACT_TYPE"
#define M4MDFAC_ITEM_EXE_GROUP							"EXE_GROUP"
#define M4MDFAC_ITEM_ID_EXE_LEVEL						"ID_EXE_LEVEL"
#define M4MDFAC_ITEM_ALIAS_READ_OBJECT					"ALIAS_READ_OBJECT"
#define M4MDFAC_ITEM_ALIAS_WRITE_OBJECT					"ALIAS_WRITE_OBJECT"
#define M4MDFAC_ITEM_ID_READ_FIELD						"ID_READ_FIELD"
#define M4MDFAC_ITEM_ID_WRITE_FIELD						"ID_WRITE_FIELD"
#define M4MDFAC_ITEM_ALIAS_READ_ID						"ID_READ_OBJECT"
#define M4MDFAC_ITEM_ALIAS_WRITE_ID						"ID_WRITE_OBJECT"
#define M4MDFAC_ITEM_ID_SENT_TOT_TYPE					"ID_SENT_TOT_TYPE"
#define M4MDFAC_ITEM_VARIABLE_ARGUMENTS					"VARIABLE_ARGUMENTS"
#define M4MDFAC_ITEM_IS_VISIBLE							"IS_VISIBLE"
#define M4MDFAC_ITEM_MODIFY_OTHER_ITEMS					"MODIFY_OTHER_ITEMS"
#define M4MDFAC_ITEM_INTERNAL_TYPE						"ID_INTERNAL_TYPE"
#define M4MDFAC_ITEM_ID_CONSTRAINT_FUNC					"ID_CONSTRAINT_FUNC"
#define M4MDFAC_ITEM_CONSTRAINT_ARGS					"CONSTRAINT_ARGS"
#define M4MDFAC_ITEM_DEFAULT_VALUE						"DEFAULT_VALUE"
#define M4MDFAC_ITEM_ORDER								"ITEM_ORDER"
#define M4MDFAC_ITEM_IS_EVENT_ITEM						"IS_EVENT_ITEM"
#define M4MDFAC_ITEM_ITEM_ID_T3							"ITEM_ID_T3"
#define M4MDFAC_ITEM_ITEM_ID_NODE						"ITEM_ID_NODE"
#define M4MDFAC_ITEM_ID_CONV_FUNCTION					"ID_CONV_FUNCTION"
#define M4MDFAC_ITEM_IS_PK								"IS_PK"

// para conectores de item
#define M4MDFAC_ITEM_ID_PRECEDENCE_TYPE					"ID_PRECEDENCE_TYPE"
#define M4MDFAC_ITEM_ID_SPIN_TYPE						"ID_SPIN_TYPE"
#define M4MDFAC_ITEM_CALL_ORDER							"CALL_ORDER"
#define M4MDFAC_ITEM_ID_RELSHIP_TYPE					"ID_RELSHIP_TYPE"
#define M4MDFAC_ITEM_ID_CONTEXT_TYPE					"ID_CONTEXT_TYPE"

// para los argumentos
#define M4MDFAC_ITEM_ID_ARGUMENT						"ID_ARGUMENT"
#define M4MDFAC_ITEM_N_ARGUMENT							"N_ARGUMENT"
#define M4MDFAC_ITEM_ID_ARGUMENT_TYPE					"ID_ARGUMENT_TYPE"
#define M4MDFAC_ITEM_POSITION							"POSITION"

// para el mapping de argumentos
#define M4MDFAC_ITEM_ID_ARG								"ID_ARG"
#define M4MDFAC_ITEM_ID_ARG_USED						"ID_ARG_USED"
#define M4MDFAC_ITEM_ID_ARG_VAL_USED_TYPE				"ID_ARG_VAL_USED_TYPE"

// para las reglas
#define M4MDFAC_ITEM_ID_RULE							"ID_RULE"
#define M4MDFAC_ITEM_ID_CODE							"ID_CODE"
#define M4MDFAC_ITEM_IS_METARULE						"IS_METARULE"
#define M4MDFAC_ITEM_IS_CORRECT_RULE					"IS_CORRECT_RULE"
#define M4MDFAC_ITEM_IS_EVENT_RULE						"IS_EVENT_RULE"
#define M4MDFAC_ITEM_IS_RETROACTIVITY					"IS_RETROACTIVITY"
#define M4MDFAC_ITEM_RULE_ORDER							"RULE_ORDER"
#define M4MDFAC_ITEM_BATCH_TYPE							"BATCH_TYPE"
#define M4MDFAC_ITEM_IS_BATCH_METARULE					"IS_BATCH_METARULE"

// para los tags
#define M4MDFAC_ID_TAG									"ID_TAG"
#define M4MDFAC_ID_DIMENSION_ITEM						"ID_DIMENSION_ITEM"
#define M4MDFAC_ITEM_OPERATOR							"OPERATOR"
#define M4MDFAC_ITEM_ZVALUE								"ZVALUE"

// para los codigos
#define M4MDFAC_ITEM_ID_CODE							"ID_CODE"
#define M4MDFAC_ITEM_POLISH_CODE						"POLISH_CODE"
#define M4MDFAC_ITEM_ID_CODE_TYPE						"ID_CODE_TYPE"
#define M4MDFAC_ITEM_ID_RESOURCE						"ID_RESOURCE"
#define M4MDFAC_ITEM_ID_METHOD							"ID_METHOD"
#define M4MDFAC_ITEM_ID_PRIORITY						"ID_PRIORITY"

// para sentencias
#define M4MDFAC_ITEM_ID_SENTENCE						"ID_SENTENCE"
#define M4MDFAC_ITEM_APISQL								"APISQL"
#define M4MDFAC_ITEM_APISQL_1							"APISQL_1"
#define M4MDFAC_ITEM_APISQL_2							"APISQL_2"
#define M4MDFAC_ITEM_APISQL_3							"APISQL_3"
#define M4MDFAC_ITEM_APISQL_4							"APISQL_4"
#define M4MDFAC_ITEM_MAX_NUM_COL						"MAX_NUM_COL"
#define M4MDFAC_ITEM_ID_GROUP_OBJECTS					"ID_GROUP_OBJECTS"


#define M4MDFAC_ITEM_ID_WHERE_TYPE						"ID_WHERE_TYPE"
#define M4MDFAC_ITEM_ID_FUNCTION						"ID_FUNCTION"


// para los indices
#define M4MDFAC_ITEM_ID_INDEX							"ID_INDEX"
#define M4MDFAC_ITEM_IS_DEFAULT_INDEX					"IS_DEFAULT_INDEX"
#define M4MDFAC_ITEM_SEARCH_ORDER						"SEARCH_ORDER"
#define M4MDFAC_ITEM_WAY								"WAY"


// para los filters
#define M4MDFAC_ITEM_ID_FILTER							"ID_FILTER"
#define M4MDFAC_ITEM_ID_T3_FILTER						"ID_T3_FILTER"
#define M4MDFAC_ITEM_ID_NODE_FILTER						"ID_NODE_FILTER"
#define M4MDFAC_ITEM_FILTER_TYPE						"FILTER_TYPE"
#define M4MDFAC_ITEM_ID_BEHAVIOUR_TYPE					"ID_BEHAVIOUR_TYPE"
#define M4MDFAC_ITEM_IS_OWN_NODE						"IS_OWN_NODE"
#define M4MDFAC_ITEM_SYS_SENTENCE_ITEM					"SYS_SENTENCE_ITEM"
#define M4MDFAC_ITEM_SYS_PARAM_ITEM						"SYS_PARAM_ITEM"

// para la herencia
#define M4MDFAC_ITEM_ID_LEVEL							"ID_LEVEL"

// para los conceptos
#define M4MDFAC_ITEM_HAS_BACKPAY						"HAS_BACKPAY"

// para los node reads
#define M4MDFAC_ITEM_ID_BACKPAY_ITEM					"HAS_BACKPAY"
#define M4MDFAC_ITEM_ID_REV_ITEM						"ID_REV_ITEM"
#define M4MDFAC_ITEM_ID_NODE_DATES						"ID_NODE_DATES"

// para los totales
#define M4MDFAC_ITEM_IS_SUM								"IS_SUM"



// Para la seguridad ==========================================================

#define M4MDFAC_ITEM_ID_MASK							"ID_MASK"
#define M4MDFAC_ITEM_SEC								"SECURITY"
#define M4MDFAC_ITEM_T3_SEC_READ						"T3_SEC_READ"
#define M4MDFAC_ITEM_T3_SEC_WRITE						"T3_SEC_WRITE"
#define M4MDFAC_ITEM_T3_SEC_DELETE						"T3_SEC_DELETE"
#define M4MDFAC_ITEM_T3_SEC_UPDATE						"T3_SEC_UPDATE"
#define M4MDFAC_ITEM_T3_SEC_EXE							"T3_SEC_EXE"
#define M4MDFAC_ITEM_IS_APPLICABLE						"IS_APPLICABLE"
#define M4MDFAC_ITEM_SEC_FILTER							"SEC_FILTER"
#define M4MDFAC_ITEM_SEC_DYN_FILTER						"SEC_DYN_FILTER"


// Para el xml ================================================================
#define M4MDFAC_ITEM_ID_STYLEUSE						"ID_STYLEUSE"
#define M4MDFAC_ITEM_ID_WSS_SECURITY					"ID_WSS_SECURITY"
#define M4MDFAC_ITEM_XML_METHOD_NAME					"METHOD_NAME"
#define M4MDFAC_ITEM_XML_DESCRIPTION					"DESCRIPTION"
#define M4MDFAC_ITEM_XML_ID_ARGUMENT_NODE				"ID_ARGUMENT_NODE"


// Para el mapeo ==============================================================
#define M4MDFAC_ITEM_MAP_DT_MD_VERSION					"DT_MD_VERSION"
#define M4MDFAC_ITEM_MAP_ID_ORGANIZATION				"ID_ORGANIZATION"
#define M4MDFAC_ITEM_MAP_ID_ORGANIZATION_FILTER			"ID_ORGANIZATION_FILTER"
#define M4MDFAC_ITEM_MAP_SYS_SENTENCE					"SYS_SENTENCE"
#define M4MDFAC_NODE_MAP_SOURCE							"SOURCE"
#define M4MDFAC_NODE_MAP_DESTINATION					"DESTINATION"






//=============================================================================
// Indices de los items del metacanal
//=============================================================================

// Alias resolution
#define M4MDFAC_ALIAS_RES_LOAD							ClItemIndex( 0 )
#define M4MDFAC_ALIAS_RES_ALIAS							ClItemIndex( 1 )
#define M4MDFAC_ALIAS_RES_ID_T3							ClItemIndex( 2 )
#define M4MDFAC_ALIAS_RES_ID_NODE						ClItemIndex( 3 )
#define M4MDFAC_ALIAS_RES_VERSION						ClItemIndex( 4 )
#define M4MDFAC_ALIAS_RES_ID_NODE_USED					ClItemIndex( 5 )

// Concepts
#define M4MDFAC_CONCEPTS_LOAD							ClItemIndex( 0 )
#define M4MDFAC_CONCEPTS_ID_T3							ClItemIndex( 1 )
#define M4MDFAC_CONCEPTS_ID_TI							ClItemIndex( 2 )
#define M4MDFAC_CONCEPTS_ID_ITEM						ClItemIndex( 3 )
#define M4MDFAC_CONCEPTS_N_ITEM2									 4
#define M4MDFAC_CONCEPTS_N_ITEM3									 5
#define M4MDFAC_CONCEPTS_N_ITEM4									 6
#define M4MDFAC_CONCEPTS_N_ITEM5									 7
#define M4MDFAC_CONCEPTS_N_ITEM6									 8
#define M4MDFAC_CONCEPTS_N_ITEM7									 9
#define M4MDFAC_CONCEPTS_N_ITEM8									 10
#define M4MDFAC_CONCEPTS_VERSION						ClItemIndex( 11 )
#define M4MDFAC_CONCEPTS_HAS_BACKPAY					ClItemIndex( 12 )

#define M4MDFAC_CONCEPTS_N_ITEM									 4

// Concepts Translations
#define M4MDFAC_CONCEPTS_TRANS_LOAD						ClItemIndex( 0 )
#define M4MDFAC_CONCEPTS_TRANS_ID_T3					ClItemIndex( 1 )
#define M4MDFAC_CONCEPTS_TRANS_ID_TI					ClItemIndex( 2 )
#define M4MDFAC_CONCEPTS_TRANS_N_ITEM					ClItemIndex( 3 )
#define M4MDFAC_CONCEPTS_TRANS_ID_ITEM					ClItemIndex( 4 )
#define M4MDFAC_CONCEPTS_TRANS_VERSION					ClItemIndex( 5 )
#define M4MDFAC_CONCEPTS_TRANS_ID_LANGUAGE				ClItemIndex( 6 )

// Connectors
#define M4MDFAC_CONNECTORS_LOAD							ClItemIndex( 0 )
#define M4MDFAC_CONNECTORS_ID_T3						ClItemIndex( 1 )
#define M4MDFAC_CONNECTORS_ID_NODE						ClItemIndex( 2 )
#define M4MDFAC_CONNECTORS_VERSION						ClItemIndex( 3 )
#define M4MDFAC_CONNECTORS_EXPAND_UNDO					ClItemIndex( 4 )
#define M4MDFAC_CONNECTORS_ID_SENTENCE					ClItemIndex( 5 )
#define M4MDFAC_CONNECTORS_ID_NODE_USED					ClItemIndex( 6 )
#define M4MDFAC_CONNECTORS_EXPAND_DELETE				ClItemIndex( 7 )
#define M4MDFAC_CONNECTORS_ID_CONNECTION_TYPE			ClItemIndex( 8 )

// Connector arguments
#define M4MDFAC_CONNECTOR_ARGS_LOAD						ClItemIndex( 0 )
#define M4MDFAC_CONNECTOR_ARGS_ID_T3					ClItemIndex( 1 )
#define M4MDFAC_CONNECTOR_ARGS_ID_ARG					ClItemIndex( 2 )
#define M4MDFAC_CONNECTOR_ARGS_ID_ITEM					ClItemIndex( 3 )
#define M4MDFAC_CONNECTOR_ARGS_ID_NODE					ClItemIndex( 4 )
#define M4MDFAC_CONNECTOR_ARGS_VERSION					ClItemIndex( 5 )
#define M4MDFAC_CONNECTOR_ARGS_ID_ARG_USED				ClItemIndex( 6 )
#define M4MDFAC_CONNECTOR_ARGS_ID_ITEM_USED				ClItemIndex( 7 )
#define M4MDFAC_CONNECTOR_ARGS_ID_NODE_USED				ClItemIndex( 8 )
#define M4MDFAC_CONNECTOR_ARGS_ID_ARG_VAL_USED_TYPE		ClItemIndex( 9 )

// Connector items
#define M4MDFAC_CONNECTOR_ITEM_LOAD						ClItemIndex( 0 )
#define M4MDFAC_CONNECTOR_ITEM_ID_T3					ClItemIndex( 1 )
#define M4MDFAC_CONNECTOR_ITEM_ID_ITEM					ClItemIndex( 2 )
#define M4MDFAC_CONNECTOR_ITEM_ID_NODE					ClItemIndex( 3 )
#define M4MDFAC_CONNECTOR_ITEM_VERSION					ClItemIndex( 4 )
#define M4MDFAC_CONNECTOR_ITEM_ID_CSTYPE				ClItemIndex( 5 )
#define M4MDFAC_CONNECTOR_ITEM_CALL_ORDER				ClItemIndex( 6 )
#define M4MDFAC_CONNECTOR_ITEM_ID_ITEM_USED				ClItemIndex( 7 )
#define M4MDFAC_CONNECTOR_ITEM_ID_NODE_USED				ClItemIndex( 8 )
#define M4MDFAC_CONNECTOR_ITEM_ID_SPIN_TYPE				ClItemIndex( 9 )
#define M4MDFAC_CONNECTOR_ITEM_ID_CONTEXT_TYPE			ClItemIndex( 10 )
#define M4MDFAC_CONNECTOR_ITEM_ID_RELSHIP_TYPE			ClItemIndex( 11 )
#define M4MDFAC_CONNECTOR_ITEM_ID_PRECEDENCE_TYPE		ClItemIndex( 12 )

// Filters
#define M4MDFAC_FILTERS_LOAD							ClItemIndex( 0 )
#define M4MDFAC_FILTERS_ID_T3							ClItemIndex( 1 )
#define M4MDFAC_FILTERS_ID_NODE							ClItemIndex( 2 )
#define M4MDFAC_FILTERS_VERSION							ClItemIndex( 3 )
#define M4MDFAC_FILTERS_ID_FILTER						ClItemIndex( 4 )
#define M4MDFAC_FILTERS_FILTER_TYPE						ClItemIndex( 5 )
#define M4MDFAC_FILTERS_IS_OWN_NODE						ClItemIndex( 6 )
#define M4MDFAC_FILTERS_ID_T3_FILTER					ClItemIndex( 7 )
#define M4MDFAC_FILTERS_ID_NODE_FILTER					ClItemIndex( 8 )
#define M4MDFAC_FILTERS_SYS_PARAM_ITEM					ClItemIndex( 9 )
#define M4MDFAC_FILTERS_ID_BEHAVIOUR_TYPE				ClItemIndex( 10 )
#define M4MDFAC_FILTERS_SYS_SENTENCE_ITEM				ClItemIndex( 11 )

// Indexes
#define M4MDFAC_INDEXES_LOAD							ClItemIndex( 0 )
#define M4MDFAC_INDEXES_ID_T3							ClItemIndex( 1 )
#define M4MDFAC_INDEXES_ID_NODE							ClItemIndex( 2 )
#define M4MDFAC_INDEXES_VERSION							ClItemIndex( 3 )
#define M4MDFAC_INDEXES_ID_INDEX						ClItemIndex( 4 )
#define M4MDFAC_INDEXES_IS_DEFAULT_INDEX				ClItemIndex( 5 )

// Index Items
#define M4MDFAC_INDEX_ITEMS_WAY							ClItemIndex( 0 )
#define M4MDFAC_INDEX_ITEMS_LOAD						ClItemIndex( 1 )
#define M4MDFAC_INDEX_ITEMS_ID_T3						ClItemIndex( 2 )
#define M4MDFAC_INDEX_ITEMS_ID_ITEM						ClItemIndex( 3 )
#define M4MDFAC_INDEX_ITEMS_ID_NODE						ClItemIndex( 4 )
#define M4MDFAC_INDEX_ITEMS_VERSION						ClItemIndex( 5 )
#define M4MDFAC_INDEX_ITEMS_ID_INDEX					ClItemIndex( 6 )
#define M4MDFAC_INDEX_ITEMS_ID_FUNCTION					ClItemIndex( 7 )
#define M4MDFAC_INDEX_ITEMS_SEARCH_ORDER				ClItemIndex( 8 )

// Items
#define M4MDFAC_ITEMS_LOAD								ClItemIndex( 0 )
#define M4MDFAC_ITEMS_PREC								ClItemIndex( 1 )
#define M4MDFAC_ITEMS_ID_T3								ClItemIndex( 2 )
#define M4MDFAC_ITEMS_ID_TI								ClItemIndex( 3 )
#define M4MDFAC_ITEMS_IS_PK								ClItemIndex( 4 )
#define M4MDFAC_ITEMS_SCALE								ClItemIndex( 5 )
#define M4MDFAC_ITEMS_ID_DMD							ClItemIndex( 6 )
#define M4MDFAC_ITEMS_ID_ITEM							ClItemIndex( 7 )
#define M4MDFAC_ITEMS_VERSION							ClItemIndex( 8 )
#define M4MDFAC_ITEMS_NOT_NULL							ClItemIndex( 9 )
#define M4MDFAC_ITEMS_EXE_GROUP							ClItemIndex( 10 )
#define M4MDFAC_ITEMS_ID_CSTYPE							ClItemIndex( 11 )
#define M4MDFAC_ITEMS_IS_PUBLIC							ClItemIndex( 12 )
#define M4MDFAC_ITEMS_AFFECTS_DB						ClItemIndex( 13 )
#define M4MDFAC_ITEMS_ID_M4_TYPE						ClItemIndex( 14 )
#define M4MDFAC_ITEMS_ID_SYNONYM						ClItemIndex( 15 )
#define M4MDFAC_ITEMS_IS_VISIBLE						ClItemIndex( 16 )
#define M4MDFAC_ITEMS_ITEM_ID_T3						ClItemIndex( 17 )
#define M4MDFAC_ITEMS_ITEM_ORDER						ClItemIndex( 18 )
#define M4MDFAC_ITEMS_ID_ITEM_AUX						ClItemIndex( 19 )
#define M4MDFAC_ITEMS_ID_DMD_FIELD						ClItemIndex( 20 )
#define M4MDFAC_ITEMS_ID_EXE_LEVEL						ClItemIndex( 21 )
#define M4MDFAC_ITEMS_ID_ITEM_TYPE						ClItemIndex( 22 )
#define M4MDFAC_ITEMS_ID_SYNC_TYPE						ClItemIndex( 23 )
#define M4MDFAC_ITEMS_IS_PARAMETER						ClItemIndex( 24 )
#define M4MDFAC_ITEMS_ITEM_ID_NODE						ClItemIndex( 25 )
#define M4MDFAC_ITEMS_DEFAULT_VALUE						ClItemIndex( 26 )
#define M4MDFAC_ITEMS_ID_READ_FIELD						ClItemIndex( 27 )
#define M4MDFAC_ITEMS_ID_SCOPE_TYPE						ClItemIndex( 28 )
#define M4MDFAC_ITEMS_ID_SLICE_ITEM						ClItemIndex( 29 )
#define M4MDFAC_ITEMS_IS_EVENT_ITEM						ClItemIndex( 30 )
#define M4MDFAC_ITEMS_CHECK_NOT_NULL					ClItemIndex( 31 )
#define M4MDFAC_ITEMS_ID_READ_OBJECT					ClItemIndex( 32 )
#define M4MDFAC_ITEMS_ID_WRITE_FIELD					ClItemIndex( 33 )
#define M4MDFAC_ITEMS_CLIENT_NOT_NULL					ClItemIndex( 34 )
#define M4MDFAC_ITEMS_ID_DMD_CROSSTAB					ClItemIndex( 35 )
#define M4MDFAC_ITEMS_ID_WRITE_OBJECT					ClItemIndex( 36 )
#define M4MDFAC_ITEMS_ID_CONV_FUNCTION					ClItemIndex( 37 )
#define M4MDFAC_ITEMS_ID_DMD_COMPONENT					ClItemIndex( 38 )
#define M4MDFAC_ITEMS_ID_INTERNAL_TYPE					ClItemIndex( 39 )
#define M4MDFAC_ITEMS_ID_SENT_TOT_TYPE					ClItemIndex( 40 )
#define M4MDFAC_ITEMS_ID_TOTALIZE_TYPE					ClItemIndex( 41 )
#define M4MDFAC_ITEMS_ID_TRANSACT_TYPE					ClItemIndex( 42 )
#define M4MDFAC_ITEMS_ALIAS_READ_OBJECT					ClItemIndex( 43 )
#define M4MDFAC_ITEMS_ID_SLICE_SPLIT_TP					ClItemIndex( 44 )
#define M4MDFAC_ITEMS_ID_SLICE_TOT_TYPE					ClItemIndex( 45 )
#define M4MDFAC_ITEMS_ALIAS_WRITE_OBJECT				ClItemIndex( 46 )
#define M4MDFAC_ITEMS_ID_ITEM_AUX_PRORAT				ClItemIndex( 47 )
#define M4MDFAC_ITEMS_ID_SLICE_BHVR_TYPE				ClItemIndex( 48 )
#define M4MDFAC_ITEMS_MODIFY_OTHER_ITEMS				ClItemIndex( 49 )
#define M4MDFAC_ITEMS_VARIABLE_ARGUMENTS				ClItemIndex( 50 )

// Items arguments
#define M4MDFAC_ITEM_ARGS_LOAD							ClItemIndex( 0 )
#define M4MDFAC_ITEM_ARGS_PREC							ClItemIndex( 1 )
#define M4MDFAC_ITEM_ARGS_ID_T3							ClItemIndex( 2 )
#define M4MDFAC_ITEM_ARGS_ID_TI							ClItemIndex( 3 )
#define M4MDFAC_ITEM_ARGS_ID_ITEM						ClItemIndex( 4 )
#define M4MDFAC_ITEM_ARGS_VERSION						ClItemIndex( 5 )
#define M4MDFAC_ITEM_ARGS_POSITION						ClItemIndex( 6 )
#define M4MDFAC_ITEM_ARGS_ID_M4_TYPE					ClItemIndex( 7 )
#define M4MDFAC_ITEM_ARGS_N_ARGUMENT					ClItemIndex( 8 )
#define M4MDFAC_ITEM_ARGS_ID_ARGUMENT					ClItemIndex( 9 )
#define M4MDFAC_ITEM_ARGS_ID_ARGUMENT_TYPE				ClItemIndex( 10 )

// Item CsDesc
#define M4MDFAC_ITEM_CSDESC_LOAD						ClItemIndex( 0 )
#define M4MDFAC_ITEM_CSDESC_ID_T3						ClItemIndex( 1 )
#define M4MDFAC_ITEM_CSDESC_ID_ITEM						ClItemIndex( 2 )
#define M4MDFAC_ITEM_CSDESC_ID_NODE						ClItemIndex( 3 )
#define M4MDFAC_ITEM_CSDESC_VERSION						ClItemIndex( 4 )
#define M4MDFAC_ITEM_CSDESC_ID_CSTYPE					ClItemIndex( 5 )
#define M4MDFAC_ITEM_CSDESC_ID_SYNC_TYPE				ClItemIndex( 6 )

// Item Inherits
#define M4MDFAC_ITEM_INHERIT_LOAD						ClItemIndex( 0 )
#define M4MDFAC_ITEM_INHERIT_ID_T3						ClItemIndex( 1 )
#define M4MDFAC_ITEM_INHERIT_ID_TI						ClItemIndex( 2 )
#define M4MDFAC_ITEM_INHERIT_ID_ITEM					ClItemIndex( 3 )
#define M4MDFAC_ITEM_INHERIT_N_ITEM2								 4
#define M4MDFAC_ITEM_INHERIT_N_ITEM3								 5
#define M4MDFAC_ITEM_INHERIT_N_ITEM4								 6
#define M4MDFAC_ITEM_INHERIT_N_ITEM5								 7
#define M4MDFAC_ITEM_INHERIT_N_ITEM6								 8
#define M4MDFAC_ITEM_INHERIT_N_ITEM7								 9
#define M4MDFAC_ITEM_INHERIT_N_ITEM8								 10
#define M4MDFAC_ITEM_INHERIT_VERSION					ClItemIndex( 11 )

#define M4MDFAC_ITEM_INHERIT_N_ITEM									 4

// Item Inherits Translations
#define M4MDFAC_ITEM_INHERIT_TRANS_LOAD					ClItemIndex( 0 )
#define M4MDFAC_ITEM_INHERIT_TRANS_ID_T3				ClItemIndex( 1 )
#define M4MDFAC_ITEM_INHERIT_TRANS_ID_TI				ClItemIndex( 2 )
#define M4MDFAC_ITEM_INHERIT_TRANS_N_ITEM				ClItemIndex( 3 )
#define M4MDFAC_ITEM_INHERIT_TRANS_ID_ITEM				ClItemIndex( 4 )
#define M4MDFAC_ITEM_INHERIT_TRANS_VERSION				ClItemIndex( 5 )
#define M4MDFAC_ITEM_INHERIT_TRANS_ID_LANGUAGE			ClItemIndex( 6 )

// Nodes
#define M4MDFAC_NODES_LOAD								ClItemIndex( 0 )
#define M4MDFAC_NODES_ID_T3								ClItemIndex( 1 )
#define M4MDFAC_NODES_ID_TI								ClItemIndex( 2 )
#define M4MDFAC_NODES_ID_NODE							ClItemIndex( 3 )
#define M4MDFAC_NODES_IS_ROOT							ClItemIndex( 4 )
#define M4MDFAC_NODES_N_NODE2										 5
#define M4MDFAC_NODES_N_NODE3										 6
#define M4MDFAC_NODES_N_NODE4										 7
#define M4MDFAC_NODES_N_NODE5										 8
#define M4MDFAC_NODES_N_NODE6										 9
#define M4MDFAC_NODES_N_NODE7										 10
#define M4MDFAC_NODES_N_NODE8										 11
#define M4MDFAC_NODES_VERSION							ClItemIndex( 12 )
#define M4MDFAC_NODES_AUTOLOAD							ClItemIndex( 13 )
#define M4MDFAC_NODES_ID_LEVEL							ClItemIndex( 14 )
#define M4MDFAC_NODES_NUM_ROWS							ClItemIndex( 15 )
#define M4MDFAC_NODES_POS_NODO							ClItemIndex( 16 )
#define M4MDFAC_NODES_DB_RELOAD							ClItemIndex( 17 )
#define M4MDFAC_NODES_ID_CSTYPE							ClItemIndex( 18 )
#define M4MDFAC_NODES_AFFECTS_DB						ClItemIndex( 19 )
#define M4MDFAC_NODES_DYN_FILTER						ClItemIndex( 20 )
#define M4MDFAC_NODES_IS_ORDERED						ClItemIndex( 21 )
#define M4MDFAC_NODES_IS_VISIBLE						ClItemIndex( 22 )
#define M4MDFAC_NODES_NODES_TYPE						ClItemIndex( 23 )
#define M4MDFAC_NODES_IS_EXTERNAL						ClItemIndex( 24 )
#define M4MDFAC_NODES_NUM_ROWS_DB						ClItemIndex( 25 )
#define M4MDFAC_NODES_OLE_DISP_ID						ClItemIndex( 26 )
#define M4MDFAC_NODES_ID_SYNC_TYPE						ClItemIndex( 27 )
#define M4MDFAC_NODES_NUM_KEEP_ROWS						ClItemIndex( 28 )
#define M4MDFAC_NODES_ID_AUTO_FILTER					ClItemIndex( 29 )
#define M4MDFAC_NODES_MAX_NUM_BLOCKS					ClItemIndex( 30 )
#define M4MDFAC_NODES_MAX_NUM_RECORDS					ClItemIndex( 31 )
#define M4MDFAC_NODES_ID_CAPACITY_TYPE					ClItemIndex( 32 )
#define M4MDFAC_NODES_ID_GROUP_OBJECTS					ClItemIndex( 33 )
#define M4MDFAC_NODES_OLE_NODE_INTER_ID					ClItemIndex( 34 )

#define M4MDFAC_NODES_N_NODE										 5

// Nodes Translations
#define M4MDFAC_NODES_TRANS_LOAD						ClItemIndex( 0 )
#define M4MDFAC_NODES_TRANS_ID_T3						ClItemIndex( 1 )
#define M4MDFAC_NODES_TRANS_N_NODE						ClItemIndex( 2 )
#define M4MDFAC_NODES_TRANS_ID_NODE						ClItemIndex( 3 )
#define M4MDFAC_NODES_TRANS_VERSION						ClItemIndex( 4 )
#define M4MDFAC_NODES_TRANS_ID_LANGUAGE					ClItemIndex( 5 )

// Node Inherits
#define M4MDFAC_NODE_INHERIT_LOAD						ClItemIndex( 0 )
#define M4MDFAC_NODE_INHERIT_ID_T3						ClItemIndex( 1 )
#define M4MDFAC_NODE_INHERIT_ID_TI						ClItemIndex( 2 )
#define M4MDFAC_NODE_INHERIT_ID_NODE					ClItemIndex( 3 )
#define M4MDFAC_NODE_INHERIT_VERSION					ClItemIndex( 4 )
#define M4MDFAC_NODE_INHERIT_ID_LEVEL					ClItemIndex( 5 )

// Node Reads
#define M4MDFAC_NODE_READS_LOAD							ClItemIndex( 0 )
#define M4MDFAC_NODE_READS_ID_T3						ClItemIndex( 1 )
#define M4MDFAC_NODE_READS_ID_NODE						ClItemIndex( 2 )
#define M4MDFAC_NODE_READS_VERSION						ClItemIndex( 3 )
#define M4MDFAC_NODE_READS_ID_REV_ITEM					ClItemIndex( 4 )
#define M4MDFAC_NODE_READS_ID_NODE_DATES				ClItemIndex( 5 )
#define M4MDFAC_NODE_READS_ID_BACKPAY_ITEM				ClItemIndex( 6 )

// RSMs
#define M4MDFAC_RSMS_LOAD								ClItemIndex( 0 )
#define M4MDFAC_RSMS_ID_T3								ClItemIndex( 1 )
#define M4MDFAC_RSMS_ID_RSM								ClItemIndex( 2 )
#define M4MDFAC_RSMS_ID_MASK							ClItemIndex( 3 )
#define M4MDFAC_RSMS_VERSION							ClItemIndex( 4 )


// Rules
#define M4MDFAC_RULES_LOAD								ClItemIndex( 0 )
#define M4MDFAC_RULES_ID_T3								ClItemIndex( 1 )
#define M4MDFAC_RULES_ID_TI								ClItemIndex( 2 )
#define M4MDFAC_RULES_DT_END							ClItemIndex( 3 )
#define M4MDFAC_RULES_ID_ITEM							ClItemIndex( 4 )
#define M4MDFAC_RULES_ID_RULE							ClItemIndex( 5 )
#define M4MDFAC_RULES_VERSION							ClItemIndex( 6 )
#define M4MDFAC_RULES_DT_START							ClItemIndex( 7 )
#define M4MDFAC_RULES_ID_METHOD							ClItemIndex( 8 )
#define M4MDFAC_RULES_BATCH_TYPE						ClItemIndex( 9 )
#define M4MDFAC_RULES_RULE_ORDER						ClItemIndex( 10 )
#define M4MDFAC_RULES_DT_END_CORR						ClItemIndex( 11 )
#define M4MDFAC_RULES_ID_PRIORITY						ClItemIndex( 12 )
#define M4MDFAC_RULES_ID_RESOURCE						ClItemIndex( 13 )
#define M4MDFAC_RULES_IS_METARULE						ClItemIndex( 14 )
#define M4MDFAC_RULES_POLISH_CODE						ClItemIndex( 15 )
#define M4MDFAC_RULES_ID_CODE_TYPE						ClItemIndex( 16 )
#define M4MDFAC_RULES_DT_START_CORR						ClItemIndex( 17 )
#define M4MDFAC_RULES_IS_EVENT_RULE						ClItemIndex( 18 )
#define M4MDFAC_RULES_CHANNEL_DT_END					ClItemIndex( 19 )
#define M4MDFAC_RULES_CHANNEL_DT_CORR					ClItemIndex( 20 )
#define M4MDFAC_RULES_IS_CORRECT_RULE					ClItemIndex( 21 )
#define M4MDFAC_RULES_CHANNEL_DT_START					ClItemIndex( 22 )
#define M4MDFAC_RULES_IS_RETROACTIVITY					ClItemIndex( 23 )
#define M4MDFAC_RULES_IS_BATCH_METARULE					ClItemIndex( 24 )

// Sentences
#define M4MDFAC_SENTENCES_LOAD							ClItemIndex( 0 )
#define M4MDFAC_SENTENCES_ID_T3							ClItemIndex( 1 )
#define M4MDFAC_SENTENCES_APISQL						ClItemIndex( 2 )
#define M4MDFAC_SENTENCES_VERSION						ClItemIndex( 3 )
#define M4MDFAC_SENTENCES_ID_SENTENCE					ClItemIndex( 4 )
#define M4MDFAC_SENTENCES_MAX_NUM_COL					ClItemIndex( 5 )
#define M4MDFAC_SENTENCES_ID_GROUP_OBJECTS				ClItemIndex( 6 )


// T3 Alias resolution
#define M4MDFAC_T3_ALIAS_RES_LOAD						ClItemIndex( 0 )
#define M4MDFAC_T3_ALIAS_RES_ALIAS						ClItemIndex( 1 )
#define M4MDFAC_T3_ALIAS_RES_ID_T3						ClItemIndex( 2 )
#define M4MDFAC_T3_ALIAS_RES_ID_NODE					ClItemIndex( 3 )
#define M4MDFAC_T3_ALIAS_RES_ID_ROLE					ClItemIndex( 4 )
#define M4MDFAC_T3_ALIAS_RES_VERSION					ClItemIndex( 5 )
#define M4MDFAC_T3_ALIAS_RES_ID_LEVEL					ClItemIndex( 6 )
#define M4MDFAC_T3_ALIAS_RES_INSTANCE					ClItemIndex( 7 )
#define M4MDFAC_T3_ALIAS_RES_CS_ACCESS					ClItemIndex( 8 )
#define M4MDFAC_T3_ALIAS_RES_ID_T3_USED					ClItemIndex( 9 )
#define M4MDFAC_T3_ALIAS_RES_ID_OPEN_MODE_TP			ClItemIndex( 10 )
#define M4MDFAC_T3_ALIAS_RES_ID_ORGANIZATION			ClItemIndex( 11 )
#define M4MDFAC_T3_ALIAS_RES_ID_SHARED_POLICY			ClItemIndex( 12 )
#define M4MDFAC_T3_ALIAS_RES_ID_ORGANIZATION_TYPE		ClItemIndex( 13 )


// T3s
#define M4MDFAC_T3S_LOAD								ClItemIndex( 0 )
#define M4MDFAC_T3S_ID_T3								ClItemIndex( 1 )
#define M4MDFAC_T3S_N_T32											 2
#define M4MDFAC_T3S_N_T33											 3
#define M4MDFAC_T3S_N_T34											 4
#define M4MDFAC_T3S_N_T35											 5
#define M4MDFAC_T3S_N_T36											 6
#define M4MDFAC_T3S_N_T37											 7
#define M4MDFAC_T3S_N_T38											 8
#define M4MDFAC_T3S_DT_END								ClItemIndex( 9 )
#define M4MDFAC_T3S_DT_CORR								ClItemIndex( 10 )
#define M4MDFAC_T3S_VERSION								ClItemIndex( 11 )
#define M4MDFAC_T3S_DT_START							ClItemIndex( 12 )
#define M4MDFAC_T3S_ID_SERVICE							ClItemIndex( 13 )
#define M4MDFAC_T3S_OWNER_FLAG							ClItemIndex( 14 )
#define M4MDFAC_T3S_CS_EXE_TYPE							ClItemIndex( 15 )
#define M4MDFAC_T3S_ID_LANGUAGE							ClItemIndex( 16 )
#define M4MDFAC_T3S_IS_EXTERNAL							ClItemIndex( 17 )
#define M4MDFAC_T3S_HAS_SECURITY						ClItemIndex( 18 )
#define M4MDFAC_T3S_IS_CACHEABLE						ClItemIndex( 19 )
#define M4MDFAC_T3S_IS_SEPARABLE						ClItemIndex( 20 )
#define M4MDFAC_T3S_LOAD_PAYROLL						ClItemIndex( 21 )
#define M4MDFAC_T3S_OLE_CLASS_ID						ClItemIndex( 22 )
#define M4MDFAC_T3S_CREATION_TYPE						ClItemIndex( 23 )
#define M4MDFAC_T3S_DATA_LANGUAGE						ClItemIndex( 24 )
#define M4MDFAC_T3S_LOAD_LANGUAGE						ClItemIndex( 25 )
#define M4MDFAC_T3S_META_LANGUAGE						ClItemIndex( 26 )
#define M4MDFAC_T3S_ID_SERVICE_ALT						ClItemIndex( 27 )
#define M4MDFAC_T3S_ID_STREAM_TYPE						ClItemIndex( 28 )
#define M4MDFAC_T3S_IS_MD_CACHEABLE						ClItemIndex( 29 )
#define M4MDFAC_T3S_VM_CAPACITY_REQ						ClItemIndex( 30 )
#define M4MDFAC_T3S_CACHE_MAX_PERIOD					ClItemIndex( 31 )
#define M4MDFAC_T3S_ENVIRONMENT_VARS					ClItemIndex( 32 )
#define M4MDFAC_T3S_CHECK_CONCURRENCY					ClItemIndex( 33 )
#define M4MDFAC_T3S_ID_ORGANIZATION_TYPE				ClItemIndex( 34 )

#define M4MDFAC_T3S_N_T3											 2

// T3s Translations
#define M4MDFAC_T3S_TRANS_LOAD							ClItemIndex( 0 )
#define M4MDFAC_T3S_TRANS_N_T3							ClItemIndex( 1 )
#define M4MDFAC_T3S_TRANS_ID_T3							ClItemIndex( 2 )
#define M4MDFAC_T3S_TRANS_VERSION						ClItemIndex( 3 )
#define M4MDFAC_T3S_TRANS_ID_LANGUAGE					ClItemIndex( 4 )

// T3 Connectors
#define M4MDFAC_T3_CONNECTORS_LOAD						ClItemIndex( 0 )
#define M4MDFAC_T3_CONNECTORS_ID_T3						ClItemIndex( 1 )
#define M4MDFAC_T3_CONNECTORS_N_T32									 2
#define M4MDFAC_T3_CONNECTORS_N_T33									 3
#define M4MDFAC_T3_CONNECTORS_N_T34									 4
#define M4MDFAC_T3_CONNECTORS_N_T35									 5
#define M4MDFAC_T3_CONNECTORS_N_T36									 6
#define M4MDFAC_T3_CONNECTORS_N_T37									 7
#define M4MDFAC_T3_CONNECTORS_N_T38									 8
#define M4MDFAC_T3_CONNECTORS_ID_NODE					ClItemIndex( 9 )
#define M4MDFAC_T3_CONNECTORS_VERSION					ClItemIndex( 10 )
#define M4MDFAC_T3_CONNECTORS_IS_FILTER					ClItemIndex( 11 )
#define M4MDFAC_T3_CONNECTORS_ID_T3_USED				ClItemIndex( 12 )
#define M4MDFAC_T3_CONNECTORS_ID_NODE_USED				ClItemIndex( 13 )
#define M4MDFAC_T3_CONNECTORS_ID_RELATION_TYPE			ClItemIndex( 14 )
#define M4MDFAC_T3_CONNECTORS_IS_MULTISELECTION			ClItemIndex( 15 )
#define M4MDFAC_T3_CONNECTORS_ID_CONNECTION_TYPE		ClItemIndex( 16 )

#define M4MDFAC_T3_CONNECTORS_N_T3									 2

// T3 Connectors Translations
#define M4MDFAC_T3_CONNECTORS_TRANS_LOAD				ClItemIndex( 0 )
#define M4MDFAC_T3_CONNECTORS_TRANS_N_T3				ClItemIndex( 1 )
#define M4MDFAC_T3_CONNECTORS_TRANS_ID_T3				ClItemIndex( 2 )
#define M4MDFAC_T3_CONNECTORS_TRANS_VERSION				ClItemIndex( 3 )
#define M4MDFAC_T3_CONNECTORS_TRANS_ID_T3_USED			ClItemIndex( 4 )
#define M4MDFAC_T3_CONNECTORS_TRANSS_ID_LANGUAGE		ClItemIndex( 5 )

// T3 Connector Items
#define M4MDFAC_T3_CONNECT_ITEM_LOAD					ClItemIndex( 0 )
#define M4MDFAC_T3_CONNECT_ITEM_ID_T3					ClItemIndex( 1 )
#define M4MDFAC_T3_CONNECT_ITEM_ID_ITEM					ClItemIndex( 2 )
#define M4MDFAC_T3_CONNECT_ITEM_ID_NODE					ClItemIndex( 3 )
#define M4MDFAC_T3_CONNECT_ITEM_VERSION					ClItemIndex( 4 )
#define M4MDFAC_T3_CONNECT_ITEM_ID_T3_USED				ClItemIndex( 5 )
#define M4MDFAC_T3_CONNECT_ITEM_ID_ITEM_USED			ClItemIndex( 6 )
#define M4MDFAC_T3_CONNECT_ITEM_ID_NODE_USED			ClItemIndex( 7 )
#define M4MDFAC_T3_CONNECT_ITEM_ID_RELSHIP_TYPE			ClItemIndex( 8 )

// T3 Inheritance
#define M4MDFAC_T3_INHERIT_LOAD							ClItemIndex( 0 )
#define M4MDFAC_T3_INHERIT_ID_T3						ClItemIndex( 1 )
#define M4MDFAC_T3_INHERIT_VERSION						ClItemIndex( 2 )
#define M4MDFAC_T3_INHERIT_ID_LEVEL						ClItemIndex( 3 )
#define M4MDFAC_T3_INHERIT_ID_T3_BASE					ClItemIndex( 4 )

// Tags
#define M4MDFAC_TAGS_LOAD								ClItemIndex( 0 )
#define M4MDFAC_TAGS_ID_T3								ClItemIndex( 1 )
#define M4MDFAC_TAGS_ID_TI								ClItemIndex( 2 )
#define M4MDFAC_TAGS_DT_END								ClItemIndex( 3 )
#define M4MDFAC_TAGS_ID_TAG								ClItemIndex( 4 )
#define M4MDFAC_TAGS_ZVALUE								ClItemIndex( 5 )
#define M4MDFAC_TAGS_ID_ITEM							ClItemIndex( 6 )
#define M4MDFAC_TAGS_ID_RULE							ClItemIndex( 7 )
#define M4MDFAC_TAGS_VERSION							ClItemIndex( 8 )
#define M4MDFAC_TAGS_DT_START							ClItemIndex( 9 )
#define M4MDFAC_TAGS_OPERATOR							ClItemIndex( 10 )
#define M4MDFAC_TAGS_DT_END_CORR						ClItemIndex( 11 )
#define M4MDFAC_TAGS_DT_START_CORR						ClItemIndex( 12 )
#define M4MDFAC_TAGS_CHANNEL_DT_END						ClItemIndex( 13 )
#define M4MDFAC_TAGS_CHANNEL_DT_CORR					ClItemIndex( 14 )
#define M4MDFAC_TAGS_CHANNEL_DT_START					ClItemIndex( 15 )
#define M4MDFAC_TAGS_ID_DIMENSION_ITEM					ClItemIndex( 16 )

// Tis
#define M4MDFAC_TIS_LOAD								ClItemIndex( 0 )
#define M4MDFAC_TIS_ID_T3								ClItemIndex( 1 )
#define M4MDFAC_TIS_ID_TI								ClItemIndex( 2 )
#define M4MDFAC_TIS_VERSION								ClItemIndex( 3 )
#define M4MDFAC_TIS_COMPLETE							ClItemIndex( 4 )
#define M4MDFAC_TIS_CORRECTED							ClItemIndex( 5 )
#define M4MDFAC_TIS_GENERATE_SQL						ClItemIndex( 6 )
#define M4MDFAC_TIS_ID_TEMPORARY						ClItemIndex( 7 )
#define M4MDFAC_TIS_ID_READ_OBJECT						ClItemIndex( 8 )
#define M4MDFAC_TIS_ID_INHERIT_TYPE						ClItemIndex( 9 )
#define M4MDFAC_TIS_ID_WRITE_OBJECT						ClItemIndex( 10 )
#define M4MDFAC_TIS_ID_READ_SENTENCE					ClItemIndex( 11 )
#define M4MDFAC_TIS_ID_WRITE_SENTENCE					ClItemIndex( 12 )

// Tis inheritance
#define M4MDFAC_TIS_INHERIT_LOAD						ClItemIndex( 0 )
#define M4MDFAC_TIS_INHERIT_ID_T3						ClItemIndex( 1 )
#define M4MDFAC_TIS_INHERIT_ID_TI						ClItemIndex( 2 )
#define M4MDFAC_TIS_INHERIT_VERSION						ClItemIndex( 3 )
#define M4MDFAC_TIS_INHERIT_ID_LEVEL					ClItemIndex( 4 )
#define M4MDFAC_TIS_INHERIT_ID_TI_BASE					ClItemIndex( 5 )

// Totals
#define M4MDFAC_TOTALS_LOAD								ClItemIndex( 0 )
#define M4MDFAC_TOTALS_ID_T3							ClItemIndex( 1 )
#define M4MDFAC_TOTALS_ID_TI							ClItemIndex( 2 )
#define M4MDFAC_TOTALS_DT_END							ClItemIndex( 3 )
#define M4MDFAC_TOTALS_IS_SUM							ClItemIndex( 4 )
#define M4MDFAC_TOTALS_ID_ITEM							ClItemIndex( 5 )
#define M4MDFAC_TOTALS_VERSION							ClItemIndex( 6 )
#define M4MDFAC_TOTALS_DT_START							ClItemIndex( 7 )
#define M4MDFAC_TOTALS_HAS_BACKPAY						ClItemIndex( 8 )
#define M4MDFAC_TOTALS_ID_ITEM_USED						ClItemIndex( 9 )
#define M4MDFAC_TOTALS_TOTAL_DT_END						ClItemIndex( 10 )
#define M4MDFAC_TOTALS_TOTAL_VERSION					ClItemIndex( 11 )
#define M4MDFAC_TOTALS_CHANNEL_DT_END					ClItemIndex( 12 )
#define M4MDFAC_TOTALS_TOTAL_DT_START					ClItemIndex( 13 )
#define M4MDFAC_TOTALS_CHANNEL_DT_START					ClItemIndex( 14 )

// Total Tags
#define M4MDFAC_TOTAL_TAGS_LOAD							ClItemIndex( 0 )
#define M4MDFAC_TOTAL_TAGS_ID_T3						ClItemIndex( 1 )
#define M4MDFAC_TOTAL_TAGS_ID_TI						ClItemIndex( 2 )
#define M4MDFAC_TOTAL_TAGS_DT_END						ClItemIndex( 3 )
#define M4MDFAC_TOTAL_TAGS_ID_TAG						ClItemIndex( 4 )
#define M4MDFAC_TOTAL_TAGS_ZVALUE						ClItemIndex( 5 )
#define M4MDFAC_TOTAL_TAGS_ID_ITEM						ClItemIndex( 6 )
#define M4MDFAC_TOTAL_TAGS_VERSION						ClItemIndex( 7 )
#define M4MDFAC_TOTAL_TAGS_DT_START						ClItemIndex( 8 )
#define M4MDFAC_TOTAL_TAGS_OPERATOR						ClItemIndex( 9 )
#define M4MDFAC_TOTAL_TAGS_DT_END_USED					ClItemIndex( 10 )
#define M4MDFAC_TOTAL_TAGS_ID_ITEM_USED					ClItemIndex( 11 )
#define M4MDFAC_TOTAL_TAGS_DT_START_USED				ClItemIndex( 12 )
#define M4MDFAC_TOTAL_TAGS_CHANNEL_DT_END				ClItemIndex( 13 )
#define M4MDFAC_TOTAL_TAGS_CHANNEL_DT_START				ClItemIndex( 14 )
#define M4MDFAC_TOTAL_TAGS_ID_DIMENSION_ITEM			ClItemIndex( 15 )

// Triggers
#define M4MDFAC_TRIGGERS_LOAD							ClItemIndex( 0 )
#define M4MDFAC_TRIGGERS_ID_T3							ClItemIndex( 1 )
#define M4MDFAC_TRIGGERS_ID_TI							ClItemIndex( 2 )
#define M4MDFAC_TRIGGERS_ID_ITEM						ClItemIndex( 3 )
#define M4MDFAC_TRIGGERS_VERSION						ClItemIndex( 4 )
#define M4MDFAC_TRIGGERS_ID_CSTYPE						ClItemIndex( 5 )
#define M4MDFAC_TRIGGERS_CALL_ORDER						ClItemIndex( 6 )
#define M4MDFAC_TRIGGERS_ID_ITEM_USED					ClItemIndex( 7 )
#define M4MDFAC_TRIGGERS_ID_SPIN_TYPE					ClItemIndex( 8 )
#define M4MDFAC_TRIGGERS_ID_CONTEXT_TYPE				ClItemIndex( 9 )
#define M4MDFAC_TRIGGERS_ID_RELSHIP_TYPE				ClItemIndex( 10 )
#define M4MDFAC_TRIGGERS_ID_PRECEDENCE_TYPE				ClItemIndex( 11 )



// Seguridad ==================================================================

// Item mask
#define M4MDFAC_ITEM_MASK_LOAD							ClItemIndex( 0 )
#define M4MDFAC_ITEM_MASK_ID_T3							ClItemIndex( 1 )
#define M4MDFAC_ITEM_MASK_T3_SEC						ClItemIndex( 2 )
#define M4MDFAC_ITEM_MASK_ID_ITEM						ClItemIndex( 3 )
#define M4MDFAC_ITEM_MASK_ID_MASK						ClItemIndex( 4 )
#define M4MDFAC_ITEM_MASK_ID_NODE						ClItemIndex( 5 )

// Filter mask
#define M4MDFAC_FILTER_MASK_LOAD						ClItemIndex( 0 )
#define M4MDFAC_FILTER_MASK_ID_T3						ClItemIndex( 1 )
#define M4MDFAC_FILTER_MASK_ID_MASK						ClItemIndex( 2 )
#define M4MDFAC_FILTER_MASK_ID_NODE						ClItemIndex( 3 )
#define M4MDFAC_FILTER_MASK_ID_FILTER					ClItemIndex( 4 )
#define M4MDFAC_FILTER_MASK_IS_APPLICABLE				ClItemIndex( 5 )

// Node mask
#define M4MDFAC_NODE_MASK_LOAD							ClItemIndex( 0 )
#define M4MDFAC_NODE_MASK_ID_T3							ClItemIndex( 1 )
#define M4MDFAC_NODE_MASK_T3_SEC						ClItemIndex( 2 )
#define M4MDFAC_NODE_MASK_ID_MASK						ClItemIndex( 3 )
#define M4MDFAC_NODE_MASK_ID_NODE						ClItemIndex( 4 )
#define M4MDFAC_NODE_MASK_SEC_FILTER					ClItemIndex( 5 )

// T3 mask
#define M4MDFAC_T3_MASK_LOAD							ClItemIndex( 0 )
#define M4MDFAC_T3_MASK_ID_T3							ClItemIndex( 1 )
#define M4MDFAC_T3_MASK_T3_SEC							ClItemIndex( 2 )
#define M4MDFAC_T3_MASK_ID_MASK							ClItemIndex( 3 )
#define M4MDFAC_T3_MASK_VERSION							ClItemIndex( 4 )

// T3 connector mask
#define M4MDFAC_T3_CON_MASK_LOAD						ClItemIndex( 0 )
#define M4MDFAC_T3_CON_MASK_ID_T3						ClItemIndex( 1 )
#define M4MDFAC_T3_CON_MASK_ID_MASK						ClItemIndex( 2 )
#define M4MDFAC_T3_CON_MASK_ID_NODE						ClItemIndex( 3 )
#define M4MDFAC_T3_CON_MASK_ID_T3_USED					ClItemIndex( 4 )
#define M4MDFAC_T3_CON_MASK_ID_NODE_USED				ClItemIndex( 5 )
#define M4MDFAC_T3_CON_MASK_IS_APPLICABLE				ClItemIndex( 6 )



// XML ========================================================================

// Business Services
#define M4MDFAC_BUSINESS_SERVICES_LOAD					ClItemIndex( 0 )
#define M4MDFAC_BUSINESS_SERVICES_ID_T3					ClItemIndex( 1 )
#define M4MDFAC_BUSINESS_SERVICES_VERSION				ClItemIndex( 2 )
#define M4MDFAC_BUSINESS_SERVICES_ID_STYLEUSE			ClItemIndex( 3 )
#define M4MDFAC_BUSINESS_SERVICES_ID_WSS_SECURITY		ClItemIndex( 4 )

// Business Methods
#define M4MDFAC_BUSINESS_METHODS_LOAD					ClItemIndex( 0 )
#define M4MDFAC_BUSINESS_METHODS_ID_T3					ClItemIndex( 1 )
#define M4MDFAC_BUSINESS_METHODS_ID_ITEM				ClItemIndex( 2 )
#define M4MDFAC_BUSINESS_METHODS_ID_NODE				ClItemIndex( 3 )
#define M4MDFAC_BUSINESS_METHODS_VERSION				ClItemIndex( 4 )
#define M4MDFAC_BUSINESS_METHODS_ID_METHOD				ClItemIndex( 5 )
#define M4MDFAC_BUSINESS_METHODS_NM_METHOD2							 6
#define M4MDFAC_BUSINESS_METHODS_NM_METHOD3							 7
#define M4MDFAC_BUSINESS_METHODS_NM_METHOD4							 8
#define M4MDFAC_BUSINESS_METHODS_NM_METHOD5							 9
#define M4MDFAC_BUSINESS_METHODS_NM_METHOD6							10
#define M4MDFAC_BUSINESS_METHODS_NM_METHOD7							11
#define M4MDFAC_BUSINESS_METHODS_NM_METHOD8							12
#define M4MDFAC_BUSINESS_METHODS_DESC_METHOD2						13
#define M4MDFAC_BUSINESS_METHODS_DESC_METHOD3						14
#define M4MDFAC_BUSINESS_METHODS_DESC_METHOD4						15
#define M4MDFAC_BUSINESS_METHODS_DESC_METHOD5						16
#define M4MDFAC_BUSINESS_METHODS_DESC_METHOD6						17
#define M4MDFAC_BUSINESS_METHODS_DESC_METHOD7						18
#define M4MDFAC_BUSINESS_METHODS_DESC_METHOD8						19

#define M4MDFAC_BUSINESS_METHODS_NM_METHOD							 6
#define M4MDFAC_BUSINESS_METHODS_DESC_METHOD						13

// Business Methods Translations
#define M4MDFAC_BUSINESS_METHODS_TRANS_LOAD				ClItemIndex( 0 )
#define M4MDFAC_BUSINESS_METHODS_TRANS_ID_T3			ClItemIndex( 1 )
#define M4MDFAC_BUSINESS_METHODS_TRANS_VERSION			ClItemIndex( 2 )
#define M4MDFAC_BUSINESS_METHODS_TRANS_ID_METHOD		ClItemIndex( 3 )
#define M4MDFAC_BUSINESS_METHODS_TRANS_NM_METHOD		ClItemIndex( 4 )
#define M4MDFAC_BUSINESS_METHODS_TRANS_DESC_METHOD		ClItemIndex( 5 )
#define M4MDFAC_BUSINESS_METHODS_TRANS_ID_LANGUAGE		ClItemIndex( 6 )

// Business Arguments
#define M4MDFAC_BUSINESS_ARGS_LOAD						ClItemIndex( 0 )
#define M4MDFAC_BUSINESS_ARGS_ID_T3						ClItemIndex( 1 )
#define M4MDFAC_BUSINESS_ARGS_VERSION					ClItemIndex( 2 )
#define M4MDFAC_BUSINESS_ARGS_ID_METHOD					ClItemIndex( 3 )
#define M4MDFAC_BUSINESS_ARGS_ID_ARGUMENT_NODE			ClItemIndex( 4 )
#define M4MDFAC_BUSINESS_ARGS_ID_ARGUMENT_TYPE			ClItemIndex( 5 )


#endif

