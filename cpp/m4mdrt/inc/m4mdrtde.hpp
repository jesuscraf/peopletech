
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdrt.dll
// File:                m4mdrtde.hpp   
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                01-03-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene los defines del run time de metadatos
//
//
//==============================================================================



#ifndef __M4MDRTDE_HPP__
#define __M4MDRTDE_HPP__




//=============================================================================
// Defines de las posiciones de memoria
//=============================================================================


// para el canal ==============================================================

#define	M4CL_MCR_LANE_INFO_SIZE						8

#define	M4CL_MCR_LANE_INFO_SCOPE					0
#define	M4CL_MCR_LANE_INFO_INVERSE_CONNECTOR		4


#define	M4CL_MCR_INT_32_STRING_AREA					10


#define	M4CL_MCR_CHANNEL_HANDLE_AREA_SIZE		    8
#define	M4CL_MCR_CHANNEL_HANDLE_AREA		        0
#define	M4CL_MCR_CHANNEL_STRING_AREA		        8


#define	M4CL_MCR_CHANNEL_SIZE		                0
#define	M4CL_MCR_CHANNEL_BUILD		                4   // numero de build
#define	M4CL_MCR_CHANNEL_TIME_UNIT		            6   // time unit
#define	M4CL_MCR_CHANNEL_CS_TYPE		            7   // cs type
#define	M4CL_MCR_CHANNEL_LANGUAGE		            8   // language

#define	M4CL_MCR_CHANNEL_START_DATE		            16
#define	M4CL_MCR_CHANNEL_END_DATE		            24
#define	M4CL_MCR_CHANNEL_COR_START_DATE	            32
#define	M4CL_MCR_CHANNEL_COR_END_DATE		        40
#define	M4CL_MCR_CHANNEL_VERSION		            48
#define	M4CL_MCR_CHANNEL_CREATION_DATE		        56
#define	M4CL_MCR_CHANNEL_CACHE_MAX_PERIOD	        64
#define	M4CL_MCR_CHANNEL_ID	                        72
#define	M4CL_MCR_CHANNEL_NAME	                    76
#define	M4CL_MCR_CHANNEL_LICENSE_ID                 80	// Licencia. Deprecado
#define	M4CL_MCR_CHANNEL_POOL	                    84
#define	M4CL_MCR_CHANNEL_OLE_CLASS_ID				88
#define	M4CL_MCR_CHANNEL_SERVICE_ID					92
#define	M4CL_MCR_CHANNEL_SERVICE_ALT_ID				96
#define	M4CL_MCR_CHANNEL_ENVIRONMENT_VARS			100
#define	M4CL_MCR_CHANNEL_OWNER_FLAG					104
#define	M4CL_MCR_CHANNEL_SYS_LOAD_ITEM				108
#define	M4CL_MCR_CHANNEL_SYS_PERSIST_ITEM			112
#define	M4CL_MCR_CHANNEL_ROOTS_START	            116
#define	M4CL_MCR_CHANNEL_SENTENCES_START	        120
#define	M4CL_MCR_CHANNEL_INSTANCES_START	        124
#define	M4CL_MCR_CHANNEL_ITEMS_START	            128
#define	M4CL_MCR_CHANNEL_TIS_START					132
#define	M4CL_MCR_CHANNEL_PARAMETERS_START			136
#define	M4CL_MCR_CHANNEL_RSMS_START					140
#define	M4CL_MCR_CHANNEL_BUSINESS_METHOD_START		144
#define	M4CL_MCR_CHANNEL_TI_SYSTEM					148
#define	M4CL_MCR_CHANNEL_SYS_LOAD_CLIENT			152
#define	M4CL_MCR_CHANNEL_SYS_PERSIST_CLIENT			156
#define	M4CL_MCR_CHANNEL_SYS_LOAD_SERVER			160
#define	M4CL_MCR_CHANNEL_SYS_PERSIST_SERVER			164
#define	M4CL_MCR_CHANNEL_EXECUTEREPORT				168
#define	M4CL_MCR_CHANNEL_DYN_FILTER					172

#define	M4CL_MCR_CHANNEL_SECURITY					176
#define	M4CL_MCR_CHANNEL_VM_CAPACITY_REQ            178

#define	M4CL_MCR_CHANNEL_TYPE	                    180  // de 1b
#define	M4CL_MCR_CHANNEL_IS_SEPARABLE               181
#define	M4CL_MCR_CHANNEL_CS_EXE_TYPE				182
#define	M4CL_MCR_CHANNEL_IS_EXTERNAL				183
#define	M4CL_MCR_CHANNEL_HAS_SECURITY				184
#define	M4CL_MCR_CHANNEL_IS_CACHEABLE				185
#define	M4CL_MCR_CHANNEL_IS_MD_CACHEABLE			186
#define	M4CL_MCR_CHANNEL_CHECK_CONCURRENCY			187
#define	M4CL_MCR_CHANNEL_ORGANIZATION_TYPE			188
#define	M4CL_MCR_CHANNEL_INHERIT_LEVEL				189
#define	M4CL_MCR_CHANNEL_DATA_LANGUAGE				190
#define	M4CL_MCR_CHANNEL_META_LANGUAGE				191
#define	M4CL_MCR_CHANNEL_CREATION_TYPE				192
#define	M4CL_MCR_CHANNEL_STYLEUSE					193
#define	M4CL_MCR_CHANNEL_WSS_SECURITY				194

#define	M4CL_MCR_CHANNEL_NODE_NUMBER                196
#define	M4CL_MCR_CHANNEL_FIXED_SIZE		            200

#define	M4CL_MCR_CHANNEL_NODES                      200

// nodes de un canal
#define	M4CL_MCR_CHANNEL_NODE_ID                    0
#define	M4CL_MCR_CHANNEL_NODE_HANDLE                4

#define	M4CL_MCR_CHANNEL_NODE_SIZE                  8

// roots de un canal
#define	M4CL_MCR_CHANNEL_ROOT_NUMBER                0
#define	M4CL_MCR_CHANNEL_ROOTS                      4

#define	M4CL_MCR_CHANNEL_ROOT_ID                    0
#define	M4CL_MCR_CHANNEL_ROOT_HANDLE                4

#define	M4CL_MCR_CHANNEL_ROOT_SIZE                  8

// sentencias de un canal
#define	M4CL_MCR_CHANNEL_SENTENCE_NUMBER            0
#define	M4CL_MCR_CHANNEL_SENTENCES                  4

#define	M4CL_MCR_CHANNEL_SENTENCE_ID                0
#define	M4CL_MCR_CHANNEL_SENTENCE_HANDLE            4

#define	M4CL_MCR_CHANNEL_SENTENCE_SIZE              8


// t3 alias de un canal
#define	M4CL_MCR_CHANNEL_INSTANCE_NUMBER            0
#define	M4CL_MCR_CHANNEL_INSTANCES                  4

#define	M4CL_MCR_CHANNEL_INSTANCE_ID                0
#define	M4CL_MCR_CHANNEL_INSTANCE_HANDLE            4

#define	M4CL_MCR_CHANNEL_INSTANCE_SIZE              8


// items de un canal
#define	M4CL_MCR_CHANNEL_ITEMS_NUMBER               0
#define	M4CL_MCR_CHANNEL_ITEMS                      4

#define	M4CL_MCR_CHANNEL_ITEM_ID                    0
#define	M4CL_MCR_CHANNEL_ITEM_HANDLE                4

#define	M4CL_MCR_CHANNEL_ITEM_SIZE                  8


// tis de un canal
#define	M4CL_MCR_CHANNEL_TIS_NUMBER					0
#define	M4CL_MCR_CHANNEL_TIS						4

#define	M4CL_MCR_CHANNEL_TI_ID						0
#define	M4CL_MCR_CHANNEL_TI_HANDLE					4

#define	M4CL_MCR_CHANNEL_TI_SIZE					8


// parameters de un canal
#define	M4CL_MCR_CHANNEL_PARAMETERS_NUMBER			0
#define	M4CL_MCR_CHANNEL_PARAMETERS					4

#define	M4CL_MCR_CHANNEL_PARAMETER_HANDLE			0

#define	M4CL_MCR_CHANNEL_PARAMETER_SIZE				4


// RSMs de un canal
#define	M4CL_MCR_CHANNEL_RSMS_NUMBER				0
#define	M4CL_MCR_CHANNEL_RSMS						4

#define	M4CL_MCR_CHANNEL_RSMS_ID					0
#define	M4CL_MCR_CHANNEL_RSMS_MASK_ID				4

#define	M4CL_MCR_CHANNEL_RSMS_SIZE					8


// Business Methods de un canal
#define	M4CL_MCR_CHANNEL_BUSINESS_METHOD_NUMBER		0
#define	M4CL_MCR_CHANNEL_BUSINESS_METHODS			4

#define	M4CL_MCR_CHANNEL_BUSINESS_METHOD_ID			0
#define	M4CL_MCR_CHANNEL_BUSINESS_METHOD_HANDLE		4

#define	M4CL_MCR_CHANNEL_BUSINESS_METHOD_SIZE		8


// de un nodo =================================================================

#define M4CL_MCR_NODE_TI                            0
#define M4CL_MCR_NODE_ID                            4
#define M4CL_MCR_NODE_M4OBJ_ID                      8
#define M4CL_MCR_NODE_NAME                          12
#define M4CL_MCR_NODE_TI_ID                         16
#define M4CL_MCR_NODE_GROUP_OBJECTS                 20
#define M4CL_MCR_NODE_START_DATE_ITEM               24
#define M4CL_MCR_NODE_END_DATE_ITEM                 28
#define M4CL_MCR_NODE_START_COR_DATE_ITEM           32
#define M4CL_MCR_NODE_END_COR_DATE_ITEM             36
#define M4CL_MCR_NODE_VIRTUAL_FLAG_ITEM             40
#define M4CL_MCR_NODE_FILTER_START_DATE_ITEM        44
#define M4CL_MCR_NODE_FILTER_END_DATE_ITEM          48
#define M4CL_MCR_NODE_FILTER_START_COR_DATE_ITEM    52
#define M4CL_MCR_NODE_FILTER_END_COR_DATE_ITEM      56
#define M4CL_MCR_NODE_CURRENCY_TYPE_ITEM            60
#define M4CL_MCR_NODE_CURRENCY_EXCH_DATE_ITEM       64
#define M4CL_MCR_NODE_CHANGE_REASON_ITEM            68
#define M4CL_MCR_NODE_DMD_COMPONENT_ITEM            72
#define M4CL_MCR_NODE_DMD_VALUE_ITEM                76
#define M4CL_MCR_NODE_LOAD_SQL_ITEM                 80
#define M4CL_MCR_NODE_PRIORITY_ITEM                 84
#define M4CL_MCR_NODE_IMPUTE_DATE_ITEM              88
#define M4CL_MCR_NODE_PAY_DATE_ITEM                 92
#define M4CL_MCR_NODE_PAY_TYPE_ITEM                 96
#define M4CL_MCR_NODE_PAY_FREQUENCY_ITEM            100
#define M4CL_MCR_NODE_IMPUTE_PAY_TYPE_ITEM			104
#define M4CL_MCR_NODE_IMPUTE_PAY_FREQUENCY_ITEM		108
#define M4CL_MCR_NODE_CLOSING_DATE_ITEM				112
#define M4CL_MCR_NODE_SYS_PARAMS_ITEM				116
#define M4CL_MCR_NODE_BDL_CHECKS_ITEM				120
#define M4CL_MCR_NODE_RAM_ORDER_BY_ITEM				124
#define M4CL_MCR_NODE_ID_ORGANIZATION				128

#define M4CL_MCR_NODE_LOAD_BLK_ITEM					132
#define M4CL_MCR_NODE_DELETE_BLK_ITEM				136
#define M4CL_MCR_NODE_UPDATE_BLK_ITEM				140
#define M4CL_MCR_NODE_INSERT_BLK_ITEM				144

#define M4CL_MCR_NODE_DEFAULT_INDEX                 148
#define M4CL_MCR_NODE_NUM_ROWS                      152
#define M4CL_MCR_NODE_NUM_KEEP_ROWS					156
#define M4CL_MCR_NODE_NUM_ROWS_DB                   160
#define M4CL_MCR_NODE_OLE_DISP_ID					164
#define M4CL_MCR_NODE_OLE_NODE_INTER_ID				168
#define M4CL_MCR_NODE_MAX_NUM_BLOCKS				172
#define M4CL_MCR_NODE_MAX_NUM_RECORDS				176
#define M4CL_MCR_NODE_CONNECTORS_START              180
#define M4CL_MCR_NODE_INV_CONNECTORS_START          184
#define M4CL_MCR_NODE_GROUPS_START                  188
#define M4CL_MCR_NODE_PLUGS_START                   192
#define M4CL_MCR_NODE_INDEXES_START                 196
#define M4CL_MCR_NODE_ALIAS_START                   200
#define M4CL_MCR_NODE_T3_ALIAS_START                204
#define M4CL_MCR_NODE_DMDS_START					208
#define M4CL_MCR_NODE_QBF_FILTERS_START				212
#define M4CL_MCR_NODE_FIND_FILTERS_START			216
#define M4CL_MCR_NODE_SYS_SENTENCES_START			220
#define M4CL_MCR_NODE_SYS_HINTS_START				224

#define M4CL_MCR_NODE_SCR_START						228
#define M4CL_MCR_NODE_INDEX                         230
#define M4CL_MCR_NODE_TYPE							232
#define M4CL_MCR_NODE_ORDER							234

#define M4CL_MCR_NODE_CAPACITY                      236
#define M4CL_MCR_NODE_IS_ROOT                       237
#define M4CL_MCR_NODE_IS_REAL_ROOT                  238
#define M4CL_MCR_NODE_AUTOLOAD                      239
#define M4CL_MCR_NODE_IS_ORDERED                    240
#define M4CL_MCR_NODE_IS_VISIBLE                    241
#define M4CL_MCR_NODE_DYN_FILTER                    242
#define M4CL_MCR_NODE_ID_AUTO_FILTER                243
#define M4CL_MCR_NODE_CS_TYPE						244
#define M4CL_MCR_NODE_SYNC_TYPE						245
#define M4CL_MCR_NODE_IS_EXTERNAL					246
#define M4CL_MCR_NODE_IS_QBF_FILTER					247
#define M4CL_MCR_NODE_AFFECTS_DB					248
#define M4CL_MCR_NODE_DB_RELOAD						249
#define M4CL_MCR_NODE_TI_INHERIT_LEVEL				250
#define M4CL_MCR_NODE_IS_OVERWRITTEN				251

#define M4CL_MCR_NODE_NITEM_NUMBER                  252
#define M4CL_MCR_NODE_NODE_NITEM_NUMBER             254
#define M4CL_MCR_NODE_BLOCK_NITEM_NUMBER            256
#define M4CL_MCR_NODE_REGISTER_NITEM_NUMBER         258

#define M4CL_MCR_NODE_FIXED_SIZE                    260

#define M4CL_MCR_NODE_NITEMS                        260

#define M4CL_MCR_NODE_NITEM_SIZE                    8

#define	M4CL_MCR_NODE_NITEM_ID                      0
#define	M4CL_MCR_NODE_NITEM_HANDLE                  4


// conectados
#define	M4CL_MCR_NODE_CONNECTOR_NUMBER              0
#define	M4CL_MCR_NODE_RB_CONNECTOR_NUMBER           2
#define	M4CL_MCR_NODE_BB_CONNECTOR_NUMBER           4
#define	M4CL_MCR_NODE_NR_CONNECTOR_NUMBER           6
#define	M4CL_MCR_NODE_RB_FIRST_CONNECTOR            8
#define	M4CL_MCR_NODE_BB_FIRST_CONNECTOR            10
#define	M4CL_MCR_NODE_NR_FIRST_CONNECTOR            12
#define	M4CL_MCR_NODE_AUTO_CONNECTOR_NUMBER			14
#define	M4CL_MCR_NODE_NODE_CONNECTORS_FIXED_SIZE    16

#define	M4CL_MCR_NODE_CONNECTORS                    16

#define	M4CL_MCR_NODE_CONNECTOR_HANDLE              0


// connectados inversos
#define	M4CL_MCR_NODE_INV_CONNECTOR_NUMBER          0
#define	M4CL_MCR_NODE_INV_CONNECTORS                4

#define	M4CL_MCR_NODE_INV_CONNECTOR_HANDLE          0


// grupos de nodo
#define	M4CL_MCR_NODE_GROUP_NUMBER                  0
#define	M4CL_MCR_NODE_GROUPS                        4

#define	M4CL_MCR_NODE_GROUP_ID                      0
#define	M4CL_MCR_NODE_GROUP_HANDLE                  4

#define	M4CL_MCR_NODE_GROUP_SIZE                    8


// plugs de nodo
#define	M4CL_MCR_NODE_PLUG_NUMBER                   0
#define	M4CL_MCR_NODE_PLUGS                         4

#define	M4CL_MCR_NODE_PLUG_CHANNEL_ID               0
#define	M4CL_MCR_NODE_PLUG_NODE_ID                  4
#define	M4CL_MCR_NODE_PLUG_HANDLE                   8

#define	M4CL_MCR_NODE_PLUG_SIZE                     12


// index de nodo
#define	M4CL_MCR_NODE_INDEX_NUMBER                  0
#define	M4CL_MCR_NODE_INDEXES                       4

#define	M4CL_MCR_NODE_INDEX_ID                      0
#define	M4CL_MCR_NODE_INDEX_HANDLE                  4

#define	M4CL_MCR_NODE_INDEX_SIZE                    8


// alias de nodo
#define	M4CL_MCR_NODE_ALIAS_NUMBER                  0
#define	M4CL_MCR_NODE_ALIAS                         4

#define	M4CL_MCR_NODE_ALIAS_ID                      0
#define	M4CL_MCR_NODE_ALIAS_NODE_HANDLE             4

#define	M4CL_MCR_NODE_ALIAS_SIZE                    8


// t3 alias de nodo
#define	M4CL_MCR_NODE_T3_ALIAS_NUMBER               0
#define	M4CL_MCR_NODE_T3_ALIAS                      4

#define	M4CL_MCR_NODE_T3_ALIAS_ID                   0
#define	M4CL_MCR_NODE_T3_ALIAS_INSTANCE_ID          4

#define	M4CL_MCR_NODE_T3_ALIAS_SIZE                 8



// dmds de nodo
#define	M4CL_MCR_NODE_DMDS_NUMBER					0
#define	M4CL_MCR_NODE_DMDS							4

#define	M4CL_MCR_NODE_DMDS_DMD_COMPONENT			0
#define	M4CL_MCR_NODE_DMDS_DMD_FIELD				4
#define	M4CL_MCR_NODE_DMDS_DMD						8
#define	M4CL_MCR_NODE_DMDS_HANDLE					12

#define	M4CL_MCR_NODE_DMD_SIZE						16


// filters de nodo
#define	M4CL_MCR_NODE_FILTERS_NUMBER			    0
#define	M4CL_MCR_NODE_FILTERS						4

#define	M4CL_MCR_NODE_FILTER_ID						0
#define	M4CL_MCR_NODE_FILTER_HANDLE					4

#define	M4CL_MCR_NODE_FILTER_SIZE					8


// sys sentences
#define	M4CL_MCR_NODE_SYS_SENTENCES_NUMBER			0
#define	M4CL_MCR_NODE_SYS_SENTENCES					4

#define	M4CL_MCR_NODE_SYS_SENTENCE_HANDLE			0

#define	M4CL_MCR_NODE_SYS_SENTENCE_SIZE				4


// sys hints
#define	M4CL_MCR_NODE_SYS_HINTS_NUMBER				0
#define	M4CL_MCR_NODE_SYS_HINTS						4

#define	M4CL_MCR_NODE_SYS_HINT_HANDLE				0

#define	M4CL_MCR_NODE_SYS_HINT_SIZE					4



// conectores de nodo =========================================================

#define	M4CL_MCR_NODE_CONNECTOR_FATHER_NODE         0
#define	M4CL_MCR_NODE_CONNECTOR_SON_NODE            4
#define	M4CL_MCR_NODE_CONNECTOR_SENTENCE            8
#define	M4CL_MCR_NODE_CONNECTOR_TYPE                12
#define	M4CL_MCR_NODE_CONNECTOR_EXPAND_DELETE       13
#define	M4CL_MCR_NODE_CONNECTOR_EXPAND_UNDO			14

#define M4CL_MCR_NODE_CONNECTOR_FIXED_SIZE          16



// de una definicion de item ==================================================

#define M4CL_MCR_ITEM_DEFAULT_VALUE                 0

#define M4CL_MCR_ITEM_ID                            8
#define M4CL_MCR_ITEM_TI_ID                         12
#define M4CL_MCR_ITEM_DMD                           16
#define M4CL_MCR_ITEM_DMD_COMPONENT                 20
#define M4CL_MCR_ITEM_DMD_CROSSTAB                  24
#define M4CL_MCR_ITEM_DMD_FIELD                     28
#define M4CL_MCR_ITEM_DB_PRECISION                  32
#define M4CL_MCR_ITEM_PRECISION                     36
#define M4CL_MCR_ITEM_MAX_SIZE                      40
#define M4CL_MCR_ITEM_READ_OBJECT_ALIAS             44
#define M4CL_MCR_ITEM_WRITE_OBJECT_ALIAS            48
#define M4CL_MCR_ITEM_READ_FIELD_ID                 52
#define M4CL_MCR_ITEM_WRITE_FIELD_ID                56
#define M4CL_MCR_ITEM_READ_OBJECT_ID                60
#define M4CL_MCR_ITEM_WRITE_OBJECT_ID               64
#define M4CL_MCR_ITEM_SYNONYM_ID					68

#define M4CL_MCR_ITEM_ORDER                         72
#define M4CL_MCR_ITEM_FLAGS                         74

#define M4CL_MCR_ITEM_TYPE                          76
#define M4CL_MCR_ITEM_SCOPE                         77
#define M4CL_MCR_ITEM_M4_TYPE                       78
#define M4CL_MCR_ITEM_CPP_TYPE                      79
#define M4CL_MCR_ITEM_SCALE                         80
#define M4CL_MCR_ITEM_SLICE_BHRV                    81
#define M4CL_MCR_ITEM_SLICE_SPLIT                   82
#define M4CL_MCR_ITEM_TOTALIZE                      83
#define M4CL_MCR_ITEM_SLICE_TOT_TYPE                84
#define M4CL_MCR_ITEM_TRANSACTION_MODE              85
#define M4CL_MCR_ITEM_METHOD_LEVEL                  86
#define M4CL_MCR_ITEM_SENT_TOT_TYPE                 87
#define M4CL_MCR_ITEM_DEFAULT_VALUE_FLAG            88
#define M4CL_MCR_ITEM_INTERNAL_TYPE                 89
#define M4CL_MCR_ITEM_CONVERT_FUNCTION				90
#define M4CL_MCR_ITEM_MODIFY_DATA                   91

#define M4CL_MCR_ITEM_ARGUMENT_NUMBER               92
#define M4CL_MCR_ITEM_ARGUMENT_REFERENCES           94
#define	M4CL_MCR_ITEM_FIXED_SIZE		            96

#define M4CL_MCR_ITEM_ARGUMENTS                     96


// Para los argumentos
#define M4CL_MCR_ARGUMENT_ID                        0
#define M4CL_MCR_ARGUMENT_NAME						4
#define M4CL_MCR_ARGUMENT_PREC						8
#define M4CL_MCR_ARGUMENT_TYPE                      10
#define M4CL_MCR_ARGUMENT_M4_TYPE                   11

#define	M4CL_MCR_ITEM_ARGUMENT_SIZE	                12

//Para los flags
#define	M4CL_MCR_ITEM_FLAG_NODAL_ITEM               0x0001
#define	M4CL_MCR_ITEM_NOT_USED						0x0002
#define	M4CL_MCR_ITEM_FLAG_IS_VARIABLE_LENGTH       0x0004
#define	M4CL_MCR_ITEM_FLAG_HAS_VARIABLE_ARGS		0x0008
#define	M4CL_MCR_ITEM_FLAG_IS_VISIBLE				0x0010
#define	M4CL_MCR_ITEM_FLAG_IS_PK					0x0020
#define	M4CL_MCR_ITEM_FLAG_IS_INHERITED				0x0040
#define	M4CL_MCR_ITEM_FLAG_IS_PUBLIC				0x0080
#define	M4CL_MCR_ITEM_FLAG_IS_PARAMETER				0x0100
#define	M4CL_MCR_ITEM_FLAG_CHECK_NOT_NULL			0x0200
#define	M4CL_MCR_ITEM_FLAG_CLIENT_NOT_NULL			0x0400
#define	M4CL_MCR_ITEM_FLAG_NOT_NULL					0x0800
#define	M4CL_MCR_ITEM_FLAG_AFFECTS_DB				0x1000
#define	M4CL_MCR_ITEM_FLAG_GENERATE_SLICES			0x2000


// de un item de nodo =========================================================

#define M4CL_MCR_NITEM_ITEM                         0
#define M4CL_MCR_NITEM_RITEM						4
#define M4CL_MCR_NITEM_NODE                         8

#define M4CL_MCR_NITEM_SCR_START					12
#define M4CL_MCR_NITEM_INDEX                        14

#define M4CL_MCR_NITEM_FLAGS						16
#define M4CL_MCR_NITEM_CS_TYPE						17
#define M4CL_MCR_NITEM_SYNC_TYPE					18
#define M4CL_MCR_NITEM_LEVEL						19

#define M4CL_MCR_NITEM_FIXED_SIZE                   20


//Para los flags
#define	M4CL_MCR_ITEM_FLAG_IS_MY_RITEM              0x01
#define	M4CL_MCR_ITEM_FLAG_IS_EVENT					0x02

// de una referencia a un item de nodo ========================================

#define M4CL_MCR_RITEM_NAME				            0
#define M4CL_MCR_RITEM_INVERSE_CONNECTOR            4
#define M4CL_MCR_RITEM_SLICE_NITEM                  8
#define M4CL_MCR_RITEM_AUX_NITEM                    12
#define M4CL_MCR_RITEM_AUX_NITEM_PRORAT             16
#define M4CL_MCR_RITEM_CONDITIONANTS_START          20
#define M4CL_MCR_RITEM_ASSIGNED_START               24
#define M4CL_MCR_RITEM_EXECUTED_START               28
#define M4CL_MCR_RITEM_FORM_IDS_START               32
#define M4CL_MCR_RITEM_FORMULAS_START               36

#define M4CL_MCR_RITEM_SCOPE                        40

#define M4CL_MCR_RITEM_CONNECTED_ITEM_NUMBER        44
#define M4CL_MCR_RITEM_FIXED_SIZE                   48

#define M4CL_MCR_RITEM_CONNECTED_ITEMS              48


// Para los condicionantes
#define M4CL_MCR_RITEM_CONDITIONANT_NUMBER          0
#define M4CL_MCR_RITEM_CONDITIONANTS                4

#define M4CL_MCR_RITEM_CONDITIONANT_SIZE            4

#define M4CL_MCR_RITEM_CONDITIONANT_NITEM           0

// Para los asignados
#define M4CL_MCR_RITEM_ASSIGNED_NUMBER              0
#define M4CL_MCR_RITEM_ASSIGNED                     4

#define M4CL_MCR_RITEM_ASSIGNED_SIZE                4

#define M4CL_MCR_RITEM_ASSIGNED_NITEM               0

// Para los ejecutados
#define M4CL_MCR_RITEM_EXECUTED_NUMBER              0
#define M4CL_MCR_RITEM_EXECUTED                     4

#define M4CL_MCR_RITEM_EXECUTED_SIZE                4

#define M4CL_MCR_RITEM_EXECUTED_NITEM               0

// Los ids de formulas
#define M4CL_MCR_RITEM_FORM_IDS_NUMBER              0
#define M4CL_MCR_RITEM_FORM_IDS                     4

#define M4CL_MCR_RITEM_FORM_IDS_SIZE                4

#define M4CL_MCR_RITEM_FORM_IDS_HANDLE              0

// Para las fórmulas
#define M4CL_MCR_FORMULA_SLICE_NUMBER               0
#define M4CL_MCR_FORMULA_FORMULA_NUMBER             4
#define M4CL_MCR_FORMULA_FORMULA_START              8

#define M4CL_MCR_FORMULA_START_DATE                 0
#define M4CL_MCR_FORMULA_END_DATE                   8
#define M4CL_MCR_FORMULA_CODE_START                 16



// los conectados
#define M4CL_MCR_NITEM_CONNECTOR_NODE_CONNECTOR     0
#define M4CL_MCR_NITEM_CONNECTOR_FATHER_NITEM       4
#define M4CL_MCR_NITEM_CONNECTOR_SON_NITEM          8
#define M4CL_MCR_NITEM_CONNECTOR_PRECEDENCE         12
#define M4CL_MCR_NITEM_CONNECTOR_SPIN               13
#define M4CL_MCR_NITEM_CONNECTOR_RELATION           14
#define M4CL_MCR_NITEM_CONNECTOR_CONTEXT            15
#define M4CL_MCR_NITEM_CONNECTOR_CS_TYPE            16

#define M4CL_MCR_NITEM_CONNECTOR_FIXED_SIZE         20  // 17 redondeado



// para la TI =================================================================

#define M4CL_MCR_TI_ID_TI							0
#define M4CL_MCR_TI_READ_SENTENCE                   4
#define M4CL_MCR_TI_WRITE_SENTENCE                  8
#define M4CL_MCR_TI_READ_OBJECT_ID                  12
#define M4CL_MCR_TI_WRITE_OBJECT_ID                 16
#define M4CL_MCR_TI_REFERENCES_START                20

#define M4CL_MCR_TI_TEMPORARY_ID                    24
#define M4CL_MCR_TI_COMPLETE                        25
#define M4CL_MCR_TI_CORRECTED                       26
#define M4CL_MCR_TI_OWN_SENTENCE					27

#define M4CL_MCR_TI_ITEM_NUMBER						28
#define M4CL_MCR_TI_FIXED_SIZE                      32

#define M4CL_MCR_TI_ITEMS							32


// items de ti
#define	M4CL_MCR_TI_ITEM_HANDLE						0

#define	M4CL_MCR_TI_ITEM_SIZE						4

// Referencias de una Ti
#define	M4CL_MCR_REFERENCE_NODE						0
#define	M4CL_MCR_REFERENCE_NUMBER					4

#define	M4CL_MCR_REFERENCE_SIZE						8


// para los plugs =============================================================

#define M4CL_MCR_PLUG_FATHER_CHANNEL_ID             0
#define M4CL_MCR_PLUG_FATHER_CHANNEL_NAME           4
#define M4CL_MCR_PLUG_FATHER_NODE_ID                8

#define M4CL_MCR_PLUG_SCR_START						12

#define M4CL_MCR_PLUG_TYPE                          14
#define M4CL_MCR_PLUG_IS_FILTER                     15
#define M4CL_MCR_PLUG_ID_RELATION_TYPE              16
#define M4CL_MCR_PLUG_IS_MULTISELECTION             17

#define M4CL_MCR_PLUG_ITEMS_NUMBER                  20

#define M4CL_MCR_PLUG_FIXED_SIZE                    24

#define M4CL_MCR_PLUG_ITEMS                         24


#define M4CL_MCR_PLUG_ITEM_HANDLE                   0


// para los plug items ========================================================

#define M4CL_MCR_PLUG_FATHER_ITEM_ID                0
#define M4CL_MCR_PLUG_SON_ITEM_HANDLE               4
#define M4CL_MCR_PLUG_RELATION                      8

#define M4CL_MCR_PLUG_ITEM_FIXED_SIZE               12 // redondeado


// para los indices ===========================================================

#define M4CL_MCR_INDEX_ID                           0
#define M4CL_MCR_INDEX_ITEM_NUMBER                  4
#define M4CL_MCR_INDEX_FIXED_SIZE                   8

#define M4CL_MCR_INDEX_ITEMS                        8


#define M4CL_MCR_INDEX_ITEM_ITEM_HANDLE             0
#define M4CL_MCR_INDEX_ITEM_FUNCTION_ID             4
#define M4CL_MCR_INDEX_ITEM_WAY                     5

#define M4CL_MCR_INDEX_ITEM_SIZE                    8


// para los filters ===========================================================

#define M4CL_MCR_FILTER_ID							0
#define M4CL_MCR_FILTER_T3_ID						4
#define M4CL_MCR_FILTER_NODE_ID						8
#define M4CL_MCR_FILTER_SYS_SENTENCE_ITEM			12
#define M4CL_MCR_FILTER_SYS_PARAM_ITEM				16

#define M4CL_MCR_FILTER_SCR_START					20

#define M4CL_MCR_FILTER_BEHAVIOUR_TYPE				22
#define M4CL_MCR_FILTER_IS_OWN_NODE					23

#define M4CL_MCR_FILTER_FIXED_SIZE					24


// para los business methods ==================================================

#define M4CL_MCR_BUSINESS_METHOD_ID					0
#define M4CL_MCR_BUSINESS_METHOD_NAME				4
#define M4CL_MCR_BUSINESS_METHOD_DESCRIPTION		8
#define M4CL_MCR_BUSINESS_METHOD_ITEM_HANDLE		12

#define M4CL_MCR_BUSINESS_METHOD_ARGUMENT_NUMBER	16

#define	M4CL_MCR_BUSINESS_METHOD_FIXED_SIZE			20
#define M4CL_MCR_BUSINESS_METHOD_ARGUMENTS			20


// para los business arguments ================================================

#define	M4CL_MCR_BUSINESS_ARG_NODE_ID               0
#define	M4CL_MCR_BUSINESS_ARG_TYPE                  4

#define	M4CL_MCR_BUSINESS_ARG_SIZE                  8


// para el codigo =============================================================

#define M4CL_MCR_CODE_ASSIGN_FLAG                   0x80
#define M4CL_MCR_CODE_AUTOGENERATED_FLAG            0x40
#define M4CL_MCR_CODE_PRIOR_MASK					0x3f            //0011 1111
#define M4CL_MCR_CODE_INHERIT_LEVEL_MASK			0xff000000      //1111 1111 0000 0000 0000 0000 0000 0000
#define M4CL_MCR_CODE_INSTRUCTION_NUMBER_MASK		0x00ffffff      //0000 0000 1111 1111 1111 1111 1111 1111
#define M4CL_MCR_CODE_VARIABLE_NUMBER				0
#define M4CL_MCR_CODE_PRIOR_ASSIGN                  2
#define M4CL_MCR_CODE_TYPE							3
#define M4CL_MCR_CODE_NUMBER_INSTRUCTIONS			4
#define M4CL_MCR_CODE_FORMULA_SIZE					8
#define M4CL_MCR_CODE_RULE_TI                       12

#define M4CL_MCR_CODE_FIXED_SIZE					16	//Tamaño de la Cabecera de la Formula
#define M4CL_MCR_CODE_INSTRUCTION_START				16

#define M4CL_MCR_CODE_INSTRUCTION_OFFSET_SIZE		4	//32 bits = Tamaño de cada Offset de Instruccion: Ojo !!! M4_ROUND_TO_8( )


// para las instrucciones:

//	Tipos de Instrucciones
#define M4CL_MCR_CODE_INSTRUCTION_TYPE_SIMPLE		0	// Tipos de Instruccion
#define M4CL_MCR_CODE_INSTRUCTION_TYPE_VARIANT		1
#define M4CL_MCR_CODE_INSTRUCTION_TYPE_CONTROL		2
#define M4CL_MCR_CODE_INSTRUCTION_TYPE_ITEM			3
#define M4CL_MCR_CODE_INSTRUCTION_MAX_TYPES			4	// Numero de Tipos de Instruccion

#if	_M4_DEBUG > 1

//En el offset de 32 bits, los dos bits altos contienen el tipo de Instruccion: en DEBUG
#define M4CL_MCR_CODE_INSTRUCTION_TYPE_HEX_SIMPLE	0x00000000	// Tipos de Instruccion Hexadecimal
#define M4CL_MCR_CODE_INSTRUCTION_TYPE_HEX_VARIANT	0x40000000
#define M4CL_MCR_CODE_INSTRUCTION_TYPE_HEX_CONTROL	0x80000000
#define M4CL_MCR_CODE_INSTRUCTION_TYPE_HEX_ITEM		0xC0000000

#endif

//	Tamaño de cada Tipo de Instruccion: Ojo !!! M4_ROUND_TO_8( )
#define M4CL_MCR_CODE_INSTRUCTION_SIZE_VARIANT			16	//12	redondeado
#define M4CL_MCR_CODE_INSTRUCTION_SIZE_VARIANT_AS_INT	8	
#define M4CL_MCR_CODE_INSTRUCTION_SIZE_ITEM				24	//21	redondeado


//	Offsets de los datos de cada Instruccion

#define M4CL_MCR_CODE_INSTRUCTION_FLAGS				0
#define M4CL_MCR_CODE_INSTRUCTION_EXEFUNC			2

#define M4CL_MCR_CODE_INSTRUCTION_VARIANT			8
#define M4CL_MCR_CODE_INSTRUCTION_VARIANT_AS_INT	4

#define M4CL_MCR_CODE_INSTRUCTION_ITEMVAR			4

#define M4CL_MCR_CODE_INSTRUCTION_RULE				8
#define M4CL_MCR_CODE_INSTRUCTION_NODE				12
#define M4CL_MCR_CODE_INSTRUCTION_CHANNEL			16
#define M4CL_MCR_CODE_INSTRUCTION_ATTR				20

#define M4CL_MCR_CODE_INSTRUCTION_ARG_NUMBER		21



// de un grupo ================================================================

#define	M4CL_MCR_GROUP_FIXED_SIZE                   0

#define	M4CL_MCR_GROUP_ITEM_NUMBER                  0
#define	M4CL_MCR_GROUP_ITEMS                        4


// para las sentencias ========================================================

#define M4CL_MCR_SENTENCE_ID                        0
#define M4CL_MCR_SENTENCE_GROUP_OBJECTS				4
#define M4CL_MCR_SENTENCE_APISQL_1					8
#define M4CL_MCR_SENTENCE_APISQL_2					12
#define M4CL_MCR_SENTENCE_APISQL_3					16
#define M4CL_MCR_SENTENCE_APISQL_4					20
#define M4CL_MCR_SENTENCE_MAX_NUM_COL				24

#define M4CL_MCR_SENTENCE_FIXED_SIZE                28



// para las t3 alias ==========================================================

#define M4CL_MCR_INSTANCE_ID                        0
#define M4CL_MCR_INSTANCE_T3_ID                     4
#define M4CL_MCR_INSTANCE_ROLE_ID                   8
#define M4CL_MCR_INSTANCE_ORGANIZATION              12
#define M4CL_MCR_INSTANCE_SCOPE                     16
#define M4CL_MCR_INSTANCE_OPEN_MODE                 17
#define M4CL_MCR_INSTANCE_CS_ACCESS                 18
#define M4CL_MCR_INSTANCE_USE_ROLE					19
#define M4CL_MCR_INSTANCE_ORGANIZATION_TYPE			20

#define M4CL_MCR_INSTANCE_FIXED_SIZE                24  // 21 redondeado



// para la seguridad ==========================================================

#define	M4CL_SCR_SECURITY_SIZE		                0
#define	M4CL_SCR_SECURITY_BUILD		                4
#define	M4CL_SCR_SECURITY_CS_TYPE					6
#define	M4CL_SCR_SECURITY_T3_VERSION				8
#define	M4CL_SCR_SECURITY_OWN_VERSION				16
#define	M4CL_SCR_SECURITY_NODE_NUMBER				24
#define	M4CL_SCR_SECURITY_MASK_ID					28
#define	M4CL_SCR_SECURITY_T3_ID						32
#define M4CL_SCR_T3_MASK_START						36

#define M4CL_SCR_SECURITY_FIXED_SIZE				40	// 37 redondeado

#define M4CL_SCR_T3_MASK_SIZE						1
#define M4CL_SCR_NODE_MASK_SIZE						8
#define M4CL_SCR_ITEM_MASK_SIZE						1
#define M4CL_SCR_FILTER_MASK_SIZE					1
#define M4CL_SCR_T3_CON_MASK_SIZE					1


#define M4CL_SCR_READ								0x01
#define M4CL_SCR_WRITE								0x02
#define M4CL_SCR_DELETE								0x04
#define M4CL_SCR_UPDATE								0x08
#define M4CL_SCR_EXECUTE							0x10

#define M4CL_SCR_DYN_FILTER							0x20
#define M4CL_SCR_CONTINUE							0x20

#define M4CL_SCR_IS_APPLICABLE						0x01


#define M4CL_SCR_T3_MASK							0
#define M4CL_SCR_NODE_MASK							0
#define M4CL_SCR_NODE_SEC_FILTER					4
#define M4CL_SCR_ITEM_MASK							0
#define M4CL_SCR_FILTER_MASK						0
#define M4CL_SCR_T3_CON_MASK						0


#endif


