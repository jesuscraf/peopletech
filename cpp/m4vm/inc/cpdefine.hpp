//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:                 cpdefine.hpp
// Project:        compiler
// Author:         Meta Software M.S. , S.A
// Date:            Create Date
// Language:        C++ 
// Operating System: WINDOWS
// Design Document:  EXTCONCI.DOC
//
//
// Definition:
//
//    This module contiene los defines del compiler
////
//==============================================================================

#ifndef _CPDEFINE_HPP
#define _CPDEFINE_HPP
// defines



// identificadores de TIS
#define M4_TIS_SCH_TI_SYSTEM	                    "TI_SYSTEM"

// identificadores de los nodos
#define M4_NODE_SCHTIS_T3S								"TIST3S"
#define M4_NODE_SCHTIS_T3S_INHERITED					"T3_INHERIT"
#define M4_NODE_SCHTIS_NODES							"TISNODES"
#define M4_NODE_SCHTIS_NODES_INHERIT					"OVERWRITE_NODES"
#define M4_NODE_SCHTIS_TIS								"TIS"
#define M4_NODE_SCHTIS_TIS_INHERITED					"TIS_INHERIT"
#define M4_NODE_SCHTIS_ITEMS							"ITEMS"
#define M4_NODE_SCHTIS_TI_ALIAS							"TI_ALIAS"
#define M4_NODE_SCHTIS_TI_ALIAS_RES						"ALIAS_RESOLUTION"
#define M4_NODE_SCHTIS_T3_ALIAS							"T3_ALIAS"
#define M4_NODE_SCHTIS_RULES							"RULES"
#define M4_NODE_SCHTIS_ITEM_ARGS						"ITEM_ARGUMENTS"
#define M4_NODE_SCHTIS_INTERNAL_DEP						"INTERNAL_DEPENDENCIES"
#define M4_NODE_SCHTIS_EXTERNAL_DEP						"EXTERNAL_DEPENDENCIES"
#define M4_NODE_SCHTIS_CHANNEL_DEP						"CHANNEL_DEPENDENCIES"
#define M4_NODE_SCHTIS_ALIAS_RES						"ALIAS_RESOLUTION"



#define M4_NODE_SCHEXRATES_CURRENCY						"CURRENCY"
#define M4_NODE_SCHEXRATES_RATES						"RATES"
#define M4_NODE_SCHEXRATES_AUX_CURR_EX					"AUX_CURR_EX"
#define M4_NODE_SCHEXRATES_ITEM_GET_EXCHANGE			"CURGETEXCHANGE"
#define M4_NODE_SCHEXRATES_ITEM_GET_EXCHANGE_ROUNDED	"CURGETEXCHANGEROUNDED"

#define M4_NODE_SCHT3SYS_NODE_SYSTEM					"NODE_SYSTEM"

// identificadores de los items de los nodos

// para T3S
#define M4_ITEM_SCHTIS_T3S_ID_T3					"ID_T3"

// para T3S_INHERITED
#define M4_ITEM_SCHTIS_T3S_ID_LEVEL					"ID_LEVEL"
#define M4_ITEM_SCHTIS_T3S_ID_T3_BASE				"ID_T3_BASE"

// Para NODES
#define M4_ITEM_SCHTIS_NODES_ID_TI					"ID_TI"
#define M4_ITEM_SCHTIS_NODES_ID_NODE				"ID_NODE"
#define M4_ITEM_SCHTIS_NODES_ID_T3					"ID_T3"

// Para NODES_INHERITED
#define M4_ITEM_SCHTIS_NODES_INHERIT_ID_TI			"ID_TI"
#define M4_ITEM_SCHTIS_NODES_INHERIT_ID_NODE		"ID_NODE"
#define M4_ITEM_SCHTIS_NODES_INHERIT_ID_T3			"ID_T3"
#define M4_ITEM_SCHTIS_NODES_INHERIT_ID_NODE_T3		"ID_NODE_T3"

//para ALIAS_RES
#define M4_ITEM_SCHTIS_ALIAS_RES_ID_NODE			"ID_NODE"
#define M4_ITEM_SCHTIS_ALIAS_RES_ID_NODE_USED		"ID_NODE_USED"
#define M4_ITEM_SCHTIS_ALIAS_RES_ID_ALIAS			"ALIAS"
// para TIs
#define M4_ITEM_SCHTIS_TIS_ID_TI					"ID_TI"
#define M4_ITEM_SCHTIS_TIS_ID_TI_BASE				"ID_TI_BASE"
#define M4_ITEM_SCHTIS_TIS_ID_DEFAULT_DMD			"ID_DEFAULT_DMD"
#define M4_ITEM_SCHTIS_TIS_ID_DEFAULT_DMD_FIELD		"ID_DEFAULT_DMD_FIELD"
#define M4_ITEM_SCHTIS_TIS_ID_TI_OLD				"ID_TI_OLD"

// Para TIS_INHERITED
#define M4_ITEM_SCHTIS_TIS_INHERIT_ID_TI			"ID_TI"
#define M4_ITEM_SCHTIS_TIS_INHERIT_ID_TI_BASE		"ID_TI_BASE"
#define M4_ITEM_SCHTIS_TIS_INHERIT_ID_LEVEL			"ID_LEVEL"


// para ITEMS
#define M4_ITEM_SCHTIS_ITEMS_ID_TI					"ID_TI"
#define M4_ITEM_SCHTIS_ITEMS_ID_ITEM	            "ID_ITEM"
#define M4_ITEM_SCHTIS_ITEMS_ID_ITEM_TYPE           "ID_ITEM_TYPE"
#define M4_ITEM_SCHTIS_ITEMS_ID_EXE_LEVEL           "ID_EXE_LEVEL"
#define M4_ITEM_SCHTIS_ITEMS_ID_DMD					"ID_DMD"
#define M4_ITEM_SCHTIS_ITEMS_ID_DMD_COMPONENT       "ID_DMD_COMPONENT"
#define M4_ITEM_SCHTIS_ITEMS_ID_DMD_FIELD           "ID_DMD_FIELD"
#define M4_ITEM_SCHTIS_ITEMS_ID_SYNONYM             "ID_SYNONYM"
#define M4_ITEM_SCHTIS_ITEMS_N_SYNONYM              "N_SYNONYM"
#define M4_ITEM_SCHTIS_ITEMS_ID_ITEM_OLD            "ID_ITEM_OLD"
#define M4_ITEM_SCHTIS_VARIABLE_ARGS                "VARIABLE_ARGUMENTS"
#define M4_ITEM_SCHTIS_ITEMS_IS_INHERIT_ITEM		"IS_INHERIT_ITEM"
#define M4_ITEM_SCHTIS_ITEMS_ID_NODE	            "ID_NODE"
#define M4_ITEM_SCHTIS_ITEMS_ID_T3					"ID_T3"

//para ITEM_ARGS
#define M4_ITEM_SCHTIS_ITEM_ARGS_ID_ARGUMENT        "ID_ARGUMENT"
#define M4_ITEM_SCHTIS_ITEM_ARGS_ID_ARGUMENT_TYPE   "ID_ARGUMENT_TYPE"
#define M4_ITEM_SCHTIS_ITEM_POSITION				"POSITION"

//#define M4_ITEM_SCHTIS_ITEM_ARGS_INOUT_ARGUMENT     1

// para ALIAS
#define M4_ITEM_SCHTIS_TI_ALIAS_ID_T3				"ID_T3"
#define M4_ITEM_SCHTIS_TI_ALIAS_ID_TI				"ID_TI"
#define M4_ITEM_SCHTIS_TI_ALIAS_ID_NODE				"ID_NODE"
#define M4_ITEM_SCHTIS_TI_ALIAS_ID_ALIAS			"ALIAS"
#define M4_ITEM_SCHTIS_TI_ALIAS_ID_TI_USED			"ID_TI_USED"

// para ALIAS RESOLUTION
#define M4_ITEM_SCHTIS_TI_ALIAS_RES_ID_NODE_USED	"ID_NODE_USED"

// para T3 ALIAS
#define M4_ITEM_SCHTIS_T3_ALIAS_ID_ALIAS			"ALIAS"
#define M4_ITEM_SCHTIS_T3_ALIAS_ID_T3_USED			"ID_T3_USED"

// para RULES
#define M4_ITEM_SCHTIS_RULES_ID_TI					"ID_TI"
#define M4_ITEM_SCHTIS_RULES_ID_ITEM				"ID_ITEM"
#define M4_ITEM_SCHTIS_RULES_ID_RULE				"ID_RULE"
#define M4_ITEM_SCHTIS_RULES_IS_CORRECT_RULE		"IS_CORRECT_RULE"
#define M4_ITEM_SCHTIS_RULES_HAS_SYNONYM			"HAS_SYNONYM"
#define M4_ITEM_SCHTIS_RULES_RESOLVE_ALIAS			"RESOLVE_ALIAS"

// para RULES
#define M4_ITEM_SCHTIS_RULE_CODES_SOURCE_CODE		"SOURCE_CODE"
#define M4_ITEM_SCHTIS_RULE_CODES_POLISH_CODE		"POLISH_CODE"
#define M4_ITEM_SCHTIS_RULE_CODES_TEST_PARAMETERS   "TEST_PARAMETERS"
#define M4_ITEM_SCHTIS_RULE_CODES_TEMP_CODE			"TEMP_CODE"
#define M4_ITEM_SCHTIS_RULE_CODES_LN4_RESOLVED		"LN4_RESOLVED"
// para INTERNAL_DEPENDENCES
#define M4_ITEM_SCHTIS_INT_DEP_ID_ITEM_USED			"ID_ITEM_USED"

// para	EXTERNAL_DEPENDENCES
#define M4_ITEM_SCHTIS_EXT_DEP_ID_ITEM_USED			"ID_ITEM_USED"
#define M4_ITEM_SCHTIS_EXT_DEP_ID_TI_USED			"ID_TI_USED"
#define M4_ITEM_SCHTIS_EXT_DEP_ALIAS				"ALIAS"
#define M4_ITEM_SCHTIS_EXT_DEP_REAL_ALIAS			"REAL_ALIAS"

//para CHANNEL_DEPENDENCIES
#define M4_ITEM_SCHTIS_CHANNEL_DEP_ID_ITEM_USED		"ID_ITEM_USED"
#define M4_ITEM_SCHTIS_CHANNEL_DEP_ID_T3_USED		"ID_T3_USED"
#define M4_ITEM_SCHTIS_CHANNEL_DEP_ID_TI_USED		"ID_TI_USED"
#define M4_ITEM_SCHTIS_CHANNEL_DEP_ID_NODE_USED		"ID_NODE_USED"
#define M4_ITEM_SCHTIS_CHANNEL_DEP_ID_NODE_T3_USED	"ID_NODE_T3_USED"

// para CURRENCY
#define M4_ITEM_SCHEXRATES_CURRENCY_ID_CURRENCY     "ID_CURRENCY"
#define M4_ITEM_SCHEXRATES_CURRENCY_ID_ROOT_CURRENCY			"ID_ROOT_CURRENCY"
#define M4_ITEM_SCHEXRATES_CURRENCY_DT_START_ROOT_CURRENCY		"DT_START_ROOT_CURRENCY"
#define M4_ITEM_SCHEXRATES_CURRENCY_DECIMALS		"DEC_NB"

// para RATES
#define M4_ITEM_SCHEXRATES_RATES_ID_REFER_CUR	    "ID_REFER_CUR"
#define M4_ITEM_SCHEXRATES_RATES_RATE			    "RATE"
#define M4_ITEM_SCHEXRATES_RATES_DT_START		    "DT_START"
#define M4_ITEM_SCHEXRATES_RATES_DT_END				"DT_END"
#define M4_ITEM_SCHEXRATES_RATES_EX_TYPE			"EX_TYPE"

// para AUX_CUR_EX
#define M4_ITEM_SCHEXRATES_AUX_CURR_EX_ID_CURRENCY_ORG		"ID_CURRENCY_ORG"
#define M4_ITEM_SCHEXRATES_AUX_CURR_EX_ID_CURRENCY_DES		"ID_CURRENCY_DES"
#define M4_ITEM_SCHEXRATES_AUX_CURR_EX_ID_CURREN_INTERM		"ID_CURREN_INTERM"
#define M4_ITEM_SCHEXRATES_AUX_CURR_EX_DT_START				"DT_START"
#define M4_ITEM_SCHEXRATES_AUX_CURR_EX_DT_END				"DT_END"

// para currency
#define M4_ITEM_CURGETEXCHANGE						"CURGETEXCHANGE"

// pra el LookUp
#define M4_ITEM_APPLY_FILTER						"APPLY_FILTER"
#define M4_LOOKUP_RAM_FILTER						"__SysLookUpFilter"
#define M4_LOOKUP_SQL_FILTER						"__SysLookUpSQLFilter"

//Para compilar en el Design Channel. Nodos e Items del canal auxiliar.

// T3s y Alias.
#define M4OBJECT_LEVEL2_ITEMS									"SCH_LEVEL2_ITEMS"
//#define M4_CHANNEL_EXTENDED_ITEMS								"EXTENDED_ITEMS"
#define M4_CHANNEL_ITEMS_L2_INSTANCE							"ITEMS_L2_INSTANCE"

//Nodes
#define M4NODE_LEVEL2_ITEMS_NODES_L2							"SCH_NODES_L2"
#define M4NODE_LEVEL2_ITEMS_ITEMS_L2							"SCH_ITEMS_L2"

// Items del Nodo de Nodos.
	// Bloque.Metodos de busqueda
#define M4ITEM_NODES_L2_FINDITEM								"FINDITEM"
#define M4ITEM_NODES_L2_FINDNODE								"FINDNODE"
//#define M4ITEM_NODES_L2_ID_T3_FILTER							"ID_T3_FILTER"
//#define M4ITEM_NODES_L2_ID_NODE_FILTER							"ID_NODE_FILTER"
	// Registro.
#define M4ITEM_NODES_L2_ID_T3									"ID_T3"
#define M4ITEM_NODES_L2_ID_NODE									"ID_NODE"
#define M4ITEM_NODES_L2_ID_NODE_T3								"ID_NODE_T3"

// Items del Nodo de Items.
	// Bloque.Items de Filtro
//#define M4ITEM_ITEMS_L2_ID_ITEM_FILTER							"ID_ITEM_FILTER"
//#define M4ITEM_ITEMS_L2_N_SYNONYM_FILTER						"N_SYNONYM_FILTER"
//#define M4ITEM_ITEMS_L2_ID_SYNONYM_FILTER						"ID_SYNONYM_FILTER"
	// Bloque.Para rellenar dependencias de nivel 2.
#define M4ITEM_ITEMS_L2_ID_NODE_T3								"ID_NODE_T3"
	// Registro.
#define M4ITEM_ITEMS_L2_ID_ITEM									"ID_ITEM"
#define M4ITEM_ITEMS_L2_N_SYNONYM								"N_SYNONYM"
#define M4ITEM_ITEMS_L2_ID_SYNONYM								"ID_SYNONYM"
#define M4ITEM_ITEMS_L2_ID_TI									"ID_TI"
#define M4ITEM_ITEMS_L2_ID_T3									"ID_T3"
#define M4ITEM_ITEMS_L2_ID_NODE									"ID_NODE"


//Tama�o m�ximo del buffer para la polaca.
#define M4_TRAD_POLISH_MAX_SIZE						32768

//atributos
#define M4CL_ATT_SYS_SLICE_NUMBER               0
#define M4CL_ATT_SYS_START_DATE                 1
#define M4CL_ATT_SYS_END_DATE                   2
#define M4CL_ATT_SYS_FIRST_SLICE                3
#define M4CL_ATT_SYS_LAST_SLICE                 4
#define M4CL_ATT_SYS_OLD_VALUE                  5
#define M4CL_ATT_SYS_BLOB_DESCRIP               6
#define M4CL_ATT_SYS_BLOB_MASK			        7
#define M4CL_ATT_SYS_AUX_ITEM_ID		        8

//atributos m�todo
#define M4CL_ATT_MTHD_SYS_ADD_NEW_SLICE         0
#define M4CL_ATT_MTHD_SYS_GET_ITEM_NAME         1
#define M4CL_ATT_MTHD_SYS_CREATE_SLICE          2
#define M4CL_ATT_MTHD_SYS_SPLIT_SLICE           3
#define M4CL_ATT_MTHD_SYS_TOTALIZE_ITEMS		4
#define M4CL_ATT_MTHD_SYS_TOTALIZE_SLICES		5
#define M4CL_ATT_MTHD_SYS_CREATE_BLOB_FILE		6
#define M4CL_ATT_MTHD_SYS_SET_BLOB_DESCRIP		7


//len del ALIAS
#define M4CL_MAX_LEN_ALIAS						0x22

////////////////sinonimos.Colocarlo !!!!!!!!!!
#define M4CL_MAX_LEN_SYNONYM						M4CL_MAX_LEN_ITEM

enum Compile_Type{
M4_NO_SOURCE=0,
M4_SOURCE_TO_NUMBER,
M4_SOURCE_TO_STRING,
M4_SOURCE_TO_ID,
M4_OLD_ITEM_TO_ID_ITEM,
M4_SOURCE_SAVE,
M4_RESTORE_SOURCE,
M4_GET_POLISH,
M4_OLD_TI_TO_ID_TI,
M4_GET_SYMBOLS,
M4_TI_BASE_TO_INHERITED
};


#define M4_RESTORE_AFTER_COMPILE					M4_FALSE
#define M4_RESTORE_AFTER_LOAD						M4_TRUE

////////////////////////////////////////

//Finder
#define M4CL_FINDER_JIT										0x1
#define M4CL_FINDER_TICH									0x0
////////////////////////////////////////

//Campo IS_CORRECT_RULE
#define M4CL_VM_DONT_COMPILE								0x0

////////////////////////////////////////
//Campo HAS_SYNONYM
#define M4_FORMULA_WITH_SYNONYM						M4_TRUE
#define M4_FORMULA_WITHOUT_SYNONYM					M4_FALSE


#endif
