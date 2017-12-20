//## begin module%3912A6B70398.cm preserve=no
//## end module%3912A6B70398.cm

//## begin module%3912A6B70398.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.hpp
//	 Project:            M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================
//	=
//## end module%3912A6B70398.cp

//## Module: cldefine%3912A6B70398; Package specification
//## Subsystem: m4objglb::inc%3912A6EC01C8
//## Source file: d:\new\m4objglb\inc\cldefine.hpp

#ifndef _CLDEFINE_HPP_
#define _CLDEFINE_HPP_ 1

//## begin module%3912A6B70398.additionalIncludes preserve=no
//## end module%3912A6B70398.additionalIncludes

//## begin module%3912A6B70398.includes preserve=yes



#include "m4types.hpp"
#include "cldefinebase.hpp"
//## end module%3912A6B70398.includes

//## begin module%3912A6B70398.declarations preserve=no
//## end module%3912A6B70398.declarations

//## begin module%3912A6B70398.additionalDeclarations preserve=yes
//## end module%3912A6B70398.additionalDeclarations


//## begin module%3912A6B70398.epilog preserve=yes

//general

const m4date_t M4_MINUS_INF_DATE				= 657061.0;
const m4date_t M4_PLUS_INF_DATE					= 1460594.0;





// backslash
#ifdef _UNIX
#define M4_BACK_SLASH	"/"
#else
#define M4_BACK_SLASH	"\\"
#endif



#ifndef TRUE
#define TRUE		M4_TRUE
#endif

#ifndef FALSE
#define FALSE		M4_FALSE
#endif



#define M4CH_MAX_FILE                                   500


// Para el pool de cadenas

#define	M4CL_PS_INFO_SIZE								4	//2
#define	M4CL_PS_PARTIAL_INFO_SIZE						8	//4
#define	M4CL_PS_TOTAL_INFO_SIZE							12	//6

#define	M4CL_PS_DEFAULT_SIZE							32	//32 Kb

// Para la pila de fechas
#define  M4CL_DS_DEFAULT_SIZE							10
#define  M4CL_DS_DEFAULT_DELTA							1


// general

#define	M4CL_BYTES_PER_K			                    1024

#define	M4_SIZE_OF_INT8				                    1
#define	M4_SIZE_OF_INT16			                    2
#define	M4_SIZE_OF_INT32			                    4
#define	M4_SIZE_OF_DATE				                    8
#define	M4_SIZE_OF_DOUBLE   		                    8
#define M4_SIZE_OF_VARIANT_DATA		                    8
#define M4_SIZE_OF_POINTER			                    8
#define	M4_SIZE_OF_FLAGS   		                        0



#define	M4_ONE_DAY					                    1.0
#define	M4_ONE_HOUR					                    ( 1 / M4_HOURS_DAY )
#define	M4_ONE_MINUTE				                    ( 1 / M4_MINUTES_DAY )
#define	M4_ONE_SECOND				                    ( 1 / M4_SECONDS_DAY )

#define	M4_UNIT_ONE_DAY									1
#define	M4_UNIT_ONE_HOUR								2
#define	M4_UNIT_ONE_MINUTE				                3
#define	M4_UNIT_ONE_SECOND				                4


// Para los lenguajes

// If not default language found in the registry, it uses these three defines.
#define M4_LANG_DEFAULT_ID								2
#define M4_LANG_DEFAULT_LCID							0x00000409
#define M4_LANG_DEFAULT_NAME							"English"
#define M4_LANG_DEFAULT_ERR_DATE						"%Y-%m-%d"
#define M4_LANG_DEFAULT_ERR_TIME						"%Y-%m-%d %H:%M:%S"

#define M4_LANG_NONE_ID									0	// This id is not allowed in any language of the registry
#define M4_LANG_UNDEFINED_NAME							"Undefined"
#define M4CL_LANGUAGE_NONE								M4_LANG_NONE_ID

// M4CL_LANGUAGE_DEFAULT should be only used for initializing variables,
//  because it doesn't really have the default language
#define M4CL_LANGUAGE_DEFAULT							M4_LANG_DEFAULT_ID
#define M4CL_DEV_LANGUAGE_DEFAULT						2

// This should be removed to depend only from the Registry
#define	M4CL_LANGUAGE_NUMBER							9



//=============================================================================
// Defines de los modos de codificación de texto
//=============================================================================

#define	M4MDRT_CODE_MODE_PLAIN							0
#define	M4MDRT_CODE_MODE_XML							1



#define M4CH_LOGSYS_FILE                                "logsys.ini"
#define M4CH_LOGMSG_FILE                                "logmsg.ini"



// tipos de caches
#define M4CL_CACHE_NONE									0x00
#define M4CL_CACHE_READ									0x01
#define M4CL_CACHE_WRITE								0x02
#define M4CL_CACHE_READ_WRITE							0x03
#define M4CL_CACHE_MEM_ONLY								0x04


// propiedades de canal ===================================

#define M4CMPROP_CHANNEL_META_IDENTIFIER				"META_IDENTIFIER"

#define M4CMPROP_CHANNEL_WANTS_AUTOLOAD					"AUTOLOAD"
#define M4CMPROP_CHANNEL_CS_TYPE						"CS_TYPE"

#define M4CMPROP_CHANNEL_DATA_CORRECTION_DATE			"DATA_CORRECTION_DATE"
#define M4CMPROP_CHANNEL_DATA_END_DATE					"DATA_END_DATE"
#define M4CMPROP_CHANNEL_DATA_START_DATE				"DATA_START_DATE"

#define M4CMPROP_CHANNEL_APPLY_MAPPINGS					"APPLY_MAPPINGS"
#define M4CMPROP_CHANNEL_DATA_LANGUAGE					"DATA_LANGUAGE"
#define M4CMPROP_CHANNEL_META_LANGUAGE					"META_LANGUAGE"

#define M4CMPROP_CHANNEL_EXECUTION_DATE					"EXECUTION_DATE"
#define M4CMPROP_CHANNEL_EXECUTION_END_DATE				"EXECUTION_END_DATE"
#define M4CMPROP_CHANNEL_EXECUTION_START_DATE			"EXECUTION_START_DATE"

#define M4CMPROP_CHANNEL_ID_APP_ROLE					"ID_APP_ROLE"
#define M4CMPROP_CHANNEL_ID_PROJECT						"ID_PROJECT"
#define M4CMPROP_CHANNEL_ID_ORGANIZATION				"ID_ORGANIZATION"
#define M4CMPROP_CHANNEL_ID_ORGANIZATION_TYPE			"ID_ORGANIZATION_TYPE"
#define M4CMPROP_CHANNEL_ID_RSM							"ID_RSM"

#define M4CMPROP_CHANNEL_ID_TRAMITATION					"ID_TRAMITATION"

#define M4CMPROP_CHANNEL_DB_CONN_ID						"DB_CONNECTION_ID"

#define M4CMPROP_CHANNEL_CORRECTION_DATE				"META_DATA_CORRECTION_DATE"
#define M4CMPROP_CHANNEL_END_DATE						"META_DATA_END_DATE"
#define M4CMPROP_CHANNEL_START_DATE						"META_DATA_START_DATE"

#define M4CMPROP_CHANNEL_NEW_RECORD_VERSION				"RECORD_VERSION"

#define M4CMPROP_CHANNEL_UNIT_DATE						"UNIT_DATE"

// para carriles
#define M4CMPROP_CHANNEL_LANE_BLOCK_NUM					"LANE_BLOCK_N"
#define M4CMPROP_CHANNEL_LANE_REG_NUM					"LANE_REG_N"

#define M4CMPROP_CHANNEL_ROUND_CURRENCY					"ROUND_CURRENCY"
#define M4CMPROP_CHANNEL_ROUND_NUMBER					"ROUND_NUMBER"
#define M4CMPROP_CHANNEL_CURRENCY_PRECISION				"CURRENCY_PRECISION"
#define M4CMPROP_CHANNEL_NUMBER_PRECISION				"NUMBER_PRECISION"

#define M4CMPROP_CHANNEL_EXECUTE_REALSQL_MODE			"EXECUTE_REALSQL_MODE"


// para la factoria =====================================

#define	M4CL_MCR_STATIC_POOL_SIZE		                32
#define	M4CL_MCR_FILE_POOL_SIZE		                    10
#define	M4CL_MCR_ACCESS_UPPER_POOL_SIZE					32
#define	M4CL_MCR_ACCESS_NO_UPPER_POOL_SIZE				32
#define	M4CL_MCR_SYSTEM_UPPER_POOL_SIZE					5
#define	M4CL_MCR_SYSTEM_NO_UPPER_POOL_SIZE				5
#define	M4CL_MCR_TEMP_POOL_SIZE		                    5
#define	M4CL_MAP_UPPER_POOL_SIZE	                    32

#define	M4CL_MCR_MAX_LINE_SIZE		                    200
#define	M4CL_MCR_MAX_TEMP_STRING		                50

#define	M4CL_MAX_ENVIRONMENT_VAR		                250



// Para canales ==============================================

// Tipos
#define M4CL_CHANNEL_TYPE_NORMAL	                    1
#define M4CL_CHANNEL_TYPE_CACHE			                2
#define M4CL_CHANNEL_TYPE_SYSTEM			            3
#define M4CL_CHANNEL_TYPE_META			                4
#define M4CL_CHANNEL_TYPE_PAYROLL		                5
#define M4CL_CHANNEL_TYPE_APPLY_VALUE	                6

// Tipos de creación
#define M4CL_CHANNEL_CREATION_TYPE_NONE					0
#define M4CL_CHANNEL_CREATION_TYPE_PAYROLL				129

// Tipos de lenguajes
#define M4CL_LANGUAGE_TYPE_NONE							0
#define M4CL_LANGUAGE_TYPE_LOGON						1
#define M4CL_LANGUAGE_TYPE_DEVELOPMENT					2


// Par las Tis ===============================================

#define M4CL_TI_INHERIT_TYPE_NONE		                0x00
#define M4CL_TI_INHERIT_TYPE_LOAD		                0x01
#define M4CL_TI_INHERIT_TYPE_PERSIST		            0x02
#define M4CL_TI_INHERIT_TYPE_NAVIGATION		            0x04
#define M4CL_TI_INHERIT_TYPE_GENERAL		            0x08
#define M4CL_TI_INHERIT_TYPE_ALL		                0x0F


// Par las T3is Alias===============================================

#define M4CL_T3ALIAS_OPEN_MODE_NO_LOAD					0
#define M4CL_T3ALIAS_OPEN_MODE_LOAD						1
#define M4CL_T3ALIAS_OPEN_MODE_AUTOLOAD_BLOCK			2
#define M4CL_T3ALIAS_OPEN_MODE_AUTOLOAD_PRG				3
#define M4CL_T3ALIAS_OPEN_MODE_AUTOLOAD_NODESAYS		4

#define M4CL_T3ALIAS_OPEN_MODE_MIN						0	//para chequeo de rangos
#define M4CL_T3ALIAS_OPEN_MODE_MAX						4

#define M4CL_T3ALIAS_OPEN_SCOPE_NOT_SHARED              1
#define M4CL_T3ALIAS_OPEN_SCOPE_LOCAL_SHARED            2
#define M4CL_T3ALIAS_OPEN_SCOPE_GLOBAL_SHARED           3

#define M4CL_T3ALIAS_OPEN_SCOPE_MIN						1	//para chequeo de rangos
#define M4CL_T3ALIAS_OPEN_SCOPE_MAX						3
// Par los Index===============================================

#define M4CL_INDEX_FUNCTION_EQUAL						1
#define M4CL_INDEX_FUNCTION_BIGGER						2
#define M4CL_INDEX_FUNCTION_BIGGER_OR_EQUAL				3
#define M4CL_INDEX_FUNCTION_SMALLER						4
#define M4CL_INDEX_FUNCTION_SMALLER_OR_EQUAL			5

#define M4CL_INDEX_SEARCH_WAY_ASC			            1
#define M4CL_INDEX_SEARCH_WAY_DESC			            2


// Para los items de Ti ========================================

// tipo
#define M4CL_ITEM_TYPE_METHOD		                    1
#define M4CL_ITEM_TYPE_PROPERTY		                    2
#define M4CL_ITEM_TYPE_FIELD		                    3
#define M4CL_ITEM_TYPE_CONCEPT		                    4

// Scope del item
#define	M4CL_ITEM_SCOPE_NODE			                1
#define	M4CL_ITEM_SCOPE_BLOCK			                2
#define	M4CL_ITEM_SCOPE_REGISTER			            3

#define M4CL_ITEM_MAX_SCOPE                             3

// Comportamiento del item al calcularse tramos
#define	M4CL_ITEM_REFRESH_SLICES_NEW				    1
#define	M4CL_ITEM_REFRESH_SLICES_OLD			        2

// Comportamiento del item al aglutinar tramos
#define	M4CL_ITEM_SLICE_BHRV_BASE_VALUE				    1
#define	M4CL_ITEM_SLICE_BHRV_FINAL_VALUE			    2
#define	M4CL_ITEM_SLICE_BHRV_UNIT					    3
#define	M4CL_ITEM_SLICE_BHRV_INCIDENCE				    4
#define	M4CL_ITEM_SLICE_BHRV_PERIOD_FINAL_VALUE		    5

// Comportamiento del generar tramos
#define	M4CL_ITEM_SLICE_SPLIT_LINEAR			        1
#define	M4CL_ITEM_SLICE_SPLIT_NON_LINEAR		        2
#define	M4CL_ITEM_SLICE_SPLIT_NO_SLICES				    3

// Funciones de totalización al pie
#define	M4CL_ITEM_TOTALIZE_NONE							0
#define	M4CL_ITEM_TOTALIZE_COUNT			            1
#define	M4CL_ITEM_TOTALIZE_SUM			                2
#define	M4CL_ITEM_TOTALIZE_AVG			                3
#define	M4CL_ITEM_TOTALIZE_MAX			                4
#define	M4CL_ITEM_TOTALIZE_MIN			                5
#define	M4CL_ITEM_TOTALIZE_FIRST			            6
#define	M4CL_ITEM_TOTALIZE_LAST			                7
#define	M4CL_ITEM_TOTALIZE_WEIGHTED_SUM	                8

#define	M4CL_ITEM_TOTALIZE_MIN_TYPE					    1
#define	M4CL_ITEM_TOTALIZE_MAX_TYPE					    8


// Transacciones
#define	M4CL_ITEM_TRAN_MODE_NEED_TRANSACTION	        1
#define	M4CL_ITEM_TRAN_MODE_NEW_TRANSACTION	            2
#define	M4CL_ITEM_TRAN_MODE_TRANSACTION_SUPPORTED	    3
#define	M4CL_ITEM_TRAN_MODE_TRANSACTION_NO_SUPPORTED	4

// Argumentos
#define	M4CL_ITEM_ARGUMENT_TYPE_INPUT				    1
#define	M4CL_ITEM_ARGUMENT_TYPE_REFERENCE				2


// conectores de nodo =========================================
#define	M4CL_NODE_CONNECTOR_TYPE_RB			            1
#define	M4CL_NODE_CONNECTOR_TYPE_BB			            2
#define	M4CL_NODE_CONNECTOR_TYPE_FREE			        3
#define	M4CL_NODE_CONNECTOR_TYPE_AUTO			        4

#define M4CL_NODE_CONNECTOR_MAX_TYPE                    4

// tipo m4 ======================================================

#define	M4CL_M4_TYPE_NULL					            0
#define	M4CL_M4_TYPE_FIXED_STRING			            1
#define	M4CL_M4_TYPE_VAR_STRING			                2
#define	M4CL_M4_TYPE_LONG			                    3
#define	M4CL_M4_TYPE_DATE			                    4
#define	M4CL_M4_TYPE_TIMESTAMP			                5
#define	M4CL_M4_TYPE_NUMBER			                    6
#define	M4CL_M4_TYPE_VARIANT    	                    7
#define	M4CL_M4_TYPE_CURRENCY    	                    8
#define	M4CL_M4_TYPE_NUM_VARIANT    					9
#define	M4CL_M4_TYPE_BLOB    							10
#define	M4CL_M4_TYPE_BINARY_STRING						11
#define	M4CL_M4_TYPE_HOUR								12
#define	M4CL_M4_TYPE_UNI_VAR_STRING			            13
#define	M4CL_M4_TYPE_UNI_LONG			                14
#define	M4CL_M4_TYPE_UNI_VARIANT		                15
#define	M4CL_M4_TYPE_UNI_FIXED_STRING	                16
#define	M4CL_M4_TYPE_TIME_SPAN			                17

#define	M4CL_M4_TYPE_MAX_TYPE			                17

#define	M4CL_MAX_VAR_STRING_PRECISION			        255
#define	M4CL_MAX_LONG_PRECISION			                -1
#define	M4CL_MAX_BLOB_PRECISION			                -1
#define	M4CL_MAX_DATE_PRECISION			                10
#define	M4CL_MAX_TIMESTAMP_PRECISION			        19
#define	M4CL_MAX_VARIANT_PRECISION			            255
#define	M4CL_MAX_NUM_VARIANT_PRECISION					20

// tipo interno ======================================================

// modify data ======================================================

#define M4CL_ITEM_MODIFY_NONE		                    0
#define M4CL_ITEM_MODIFY_REGISTER						1
#define M4CL_ITEM_MODIFY_BLOCK		                    2
#define M4CL_ITEM_MODIFY_CHILD		                    3
#define M4CL_ITEM_MODIFY_M4OBJECT						4
#define M4CL_ITEM_MODIFY_ALL		                    5
#define M4CL_ITEM_MODIFY_LANE		                    6

// internal id ======================================================

#define	M4CL_INTERNAL_TYPE_NULL  			            0
#define	M4CL_INTERNAL_TYPE_NORMAL  			            1
#define	M4CL_INTERNAL_TYPE_START_DATE  			        2
#define	M4CL_INTERNAL_TYPE_END_DATE  			        3
#define	M4CL_INTERNAL_TYPE_COR_START_DATE  			    4
#define	M4CL_INTERNAL_TYPE_COR_END_DATE  			    5
#define	M4CL_INTERNAL_TYPE_T_DATE  			            6
#define	M4CL_INTERNAL_TYPE_PROJECTION  			        7
#define	M4CL_INTERNAL_TYPE_TRANSLATION_PTR  		    8
#define	M4CL_INTERNAL_TYPE_TRANSLATION_FIELD  		    9
#define	M4CL_INTERNAL_TYPE_FILTER_START_DATE  		    10
#define	M4CL_INTERNAL_TYPE_FILTER_END_DATE  		    11
#define	M4CL_INTERNAL_TYPE_FILTER_COR_START_DATE  	    12
#define	M4CL_INTERNAL_TYPE_FILTER_COR_END_DATE  	    13
#define	M4CL_INTERNAL_TYPE_CURRENCY_TYPE  			    14
#define	M4CL_INTERNAL_TYPE_IS_CURRENCY  			    15
#define	M4CL_INTERNAL_TYPE_CURRENCY_EXCHANGE_DATE  	    16
#define	M4CL_INTERNAL_TYPE_DMD_CHANGE_REASON  	        17
#define	M4CL_INTERNAL_TYPE_DMD_COMPONENT  			    18
#define	M4CL_INTERNAL_TYPE_DMD_VALUE  			        19
#define	M4CL_INTERNAL_TYPE_LOAD_SQL  			        20
#define	M4CL_INTERNAL_TYPE_VARIANT_TYPE			        21
#define	M4CL_INTERNAL_TYPE_PRIORITY			            22
#define	M4CL_INTERNAL_TYPE_IS_VARIANT   	            23
#define	M4CL_INTERNAL_TYPE_IMPUTE_DATE		            24
#define	M4CL_INTERNAL_TYPE_PAY_DATE			            25
#define	M4CL_INTERNAL_TYPE_PAY_TYPE			            26
#define	M4CL_INTERNAL_TYPE_PAY_FREQUENCY	            27
#define	M4CL_INTERNAL_TYPE_CHANNEL_ITEM	                28
#define	M4CL_INTERNAL_TYPE_CURRENCY_EXCHANGE_TYPE	    29
#define	M4CL_INTERNAL_TYPE_SECURITY_USER                30
#define	M4CL_INTERNAL_TYPE_APP_ROLE						31
#define	M4CL_INTERNAL_TYPE_CLOSING_DATE                 32
#define	M4CL_INTERNAL_TYPE_SYS_PARAM					33
#define	M4CL_INTERNAL_TYPE_DYNAMIC_SYS_SENTENCE			34
#define	M4CL_INTERNAL_TYPE_EXECUTE_REPORT	            35
#define	M4CL_INTERNAL_TYPE_IMPUTE_PAY_TYPE	            36
#define	M4CL_INTERNAL_TYPE_IMPUTE_PAY_FREQUENCY			37
#define	M4CL_INTERNAL_TYPE_IS_ATTACHMENT				38
#define	M4CL_INTERNAL_TYPE_PAR_INIT_DATE				39
#define	M4CL_INTERNAL_TYPE_PAR_END_DATE					40
#define	M4CL_INTERNAL_TYPE_SYSTEM_LOAD					41
#define	M4CL_INTERNAL_TYPE_SYSTEM_PERSIST				42
#define	M4CL_INTERNAL_TYPE_SYSTEM_NAVIGATION			43
#define	M4CL_INTERNAL_TYPE_SYSTEM_GENERAL				44
#define	M4CL_INTERNAL_TYPE_ID_EXECUTION					45
#define	M4CL_INTERNAL_TYPE_ID_ORDER						46
#define	M4CL_INTERNAL_TYPE_ID_OPERATION					47
#define	M4CL_INTERNAL_TYPE_ID_TYPE_UPDATE				48
#define	M4CL_INTERNAL_TYPE_DT_OPERATION					49
#define	M4CL_INTERNAL_TYPE_ADD_INFO						50
#define	M4CL_INTERNAL_TYPE_BDL_CHECKS					51
#define	M4CL_INTERNAL_TYPE_EXECUTE_REPORT_ODBC			52
#define	M4CL_INTERNAL_TYPE_M4OBJ_ALLOCATION_DATE		53
#define	M4CL_INTERNAL_TYPE_M4OBJ_PAYMENT_DATE			54
#define	M4CL_INTERNAL_TYPE_M4OBJ_CURRENCY				55
#define	M4CL_INTERNAL_TYPE_M4OBJ_EXCHANGE_RATE			56
#define	M4CL_INTERNAL_TYPE_M4OBJ_CHANGE_DATE			57
#define	M4CL_INTERNAL_TYPE_ALLOCATION_START_DATE		58
#define	M4CL_INTERNAL_TYPE_ALLOCATION_END_DATE			59
#define	M4CL_INTERNAL_TYPE_SLICE_MODE					60
#define	M4CL_INTERNAL_TYPE_YTD_QUERY					61
#define	M4CL_INTERNAL_TYPE_RAM_ORDER_BY					62
#define	M4CL_INTERNAL_TYPE_CURRENCY_EXCHANGE			63
#define	M4CL_INTERNAL_TYPE_ID_ORGANIZATION				64
#define	M4CL_INTERNAL_TYPE_ID_ORGANIZATION_MONO_VALUE	65
#define	M4CL_INTERNAL_TYPE_ID_ORGANIZATION_LEVEL		66
#define	M4CL_INTERNAL_TYPE_ID_INHERITANCE_OPERATION		67

#define	M4CL_INTERNAL_TYPE_DYNAMIC_SYS_OBJECTS			69
#define	M4CL_INTERNAL_TYPE_OPERATION					70
#define	M4CL_INTERNAL_TYPE_ERASE_ALL_L2_IN_SERVER		71
#define	M4CL_INTERNAL_TYPE_NONE_IN_BUFFER_FIELDS		72
#define	M4CL_INTERNAL_TYPE_CURRENCY_IN_BUFFER_FIELD		73
#define	M4CL_INTERNAL_TYPE_PAYROLL_DATE					74
#define	M4CL_INTERNAL_TYPE_DO_NOT_DELETE				75
#define	M4CL_INTERNAL_TYPE_QUERY_REPORT_NO_VIEW			76
#define	M4CL_INTERNAL_TYPE_PIVOT_VALUE					77
#define	M4CL_INTERNAL_TYPE_CROSSTAB_CONCEPT_NAME		78
#define	M4CL_INTERNAL_TYPE_CROSSTAB_CONCEPT_VALUE		79
#define	M4CL_INTERNAL_TYPE_CROSSTAB_CONCEPT_TOTAL		80
#define	M4CL_INTERNAL_TYPE_SYS_HINT						81
#define	M4CL_INTERNAL_TYPE_UNUSED_ITEM					82
#define	M4CL_INTERNAL_TYPE_SOAP_METHOD					83
#define	M4CL_INTERNAL_TYPE_LIST_ICON					84
#define	M4CL_INTERNAL_TYPE_ID_DATASET					85
#define	M4CL_INTERNAL_TYPE_TRAMIT_OPERATION				86
#define	M4CL_INTERNAL_TYPE_TRAMIT_DT_LAST_UPDATE		87
#define	M4CL_INTERNAL_TYPE_TRAMIT_START_DATE			88
#define	M4CL_INTERNAL_TYPE_TIME_SPAN					89
#define	M4CL_INTERNAL_TYPE_MULTI_SELECTION				90
#define	M4CL_INTERNAL_TYPE_RESET_M4XML_SPACE			91
#define	M4CL_INTERNAL_TYPE_HIERARCHICAL_MULTISELECTION	92
#define	M4CL_INTERNAL_TYPE_HIERARCHICAL					93
#define	M4CL_INTERNAL_TYPE_UNIT_TEST					94
#define	M4CL_INTERNAL_TYPE_UNIT_TEST_NO_TRANSACTION		95
#define	M4CL_INTERNAL_TYPE_RANGE_START					96
#define	M4CL_INTERNAL_TYPE_RANGE_END					97
#define	M4CL_INTERNAL_TYPE_SHORT_CUMULATIVE_CROSS_COL	98
#define	M4CL_INTERNAL_TYPE_HR_ID_COL					99
#define	M4CL_INTERNAL_TYPE_PERIOD_ID_COL				100
#define	M4CL_INTERNAL_TYPE_ROLE_ID_COL					101
#define	M4CL_INTERNAL_TYPE_JS_UNESCAPE_VALUE			102


#define	M4CL_INTERNAL_TYPE_MAX  			            250

#define	M4CL_INTERNAL_TYPE_SYSDB_ITEM					255


#define	M4CL_INTERNAL_TYPE_SYSTEM_START					M4CL_INTERNAL_TYPE_SYSTEM_LOAD

// funciones de conversion ======================================================

#define	M4CL_CONV_FUNCT_NONE  							1
#define	M4CL_CONV_FUNCT_UPPER_CASE						2
#define	M4CL_CONV_FUNCT_LOWER_CASE						3
#define	M4CL_CONV_FUNCT_FIRST_UPPER_CASE				4


// nodos ======================================================

// capacity
#define	M4CL_NODE_CAPACITY_TYPE_MONOBLOCK               1
#define	M4CL_NODE_CAPACITY_TYPE_MULTIBLOCK              2

// temporality
#define	M4CL_NODE_TEMPORALITY_EVERGREEN                 1
#define	M4CL_NODE_TEMPORALITY_1_P                       2
#define	M4CL_NODE_TEMPORALITY_N_P                       3
#define	M4CL_NODE_TEMPORALITY_N_P_NO_AUTOMATIC			4
#define	M4CL_NODE_TEMPORALITY_N_P_WITH_OVERLAPED        5

// completeness
#define	M4CL_NODE_COMPLETE_NOT_COMPLETE                 0
#define	M4CL_NODE_COMPLETE_COMPLETE_NULL                1
#define	M4CL_NODE_COMPLETE_COMPLETE_NOT_NULL            2

// auto filter
#define	M4CL_NODE_AUTO_FILTER_NONE						0
#define	M4CL_NODE_AUTO_FILTER_APPLICATION				1
#define	M4CL_NODE_AUTO_FILTER_CORRECTION				2
#define	M4CL_NODE_AUTO_FILTER_APP_CORR					3

// DB reload
#define	M4CL_NODE_DB_RELOAD_NONE						0
#define	M4CL_NODE_DB_RELOAD_LOAD_BLK					1
#define	M4CL_NODE_DB_RELOAD_LOAD_PRG					2


// Conectores de item ========================================

// precedence
#define	M4CL_ITEM_CONNECTOR_BEFORE			            1
#define	M4CL_ITEM_CONNECTOR_AFTER		                2

// spin
#define	M4CL_ITEM_CONNECTOR_UPWARDS			            1
#define	M4CL_ITEM_CONNECTOR_DOWNWARDS		            2

// relationship
#define	M4CL_ITEM_CONNECTOR_EQUALITY			        1
#define	M4CL_ITEM_CONNECTOR_BIGGER		                2
#define	M4CL_ITEM_CONNECTOR_SMALLER		                3
#define	M4CL_ITEM_CONNECTOR_BIGGER_OR_EQUAL	            4
#define	M4CL_ITEM_CONNECTOR_SMALLER_OR_EQUAL            5
#define	M4CL_ITEM_CONNECTOR_DISTINCT                    6

// context
#define	M4CL_ITEM_CONNECTOR_JUST_CONTEXT			    1
#define	M4CL_ITEM_CONNECTOR_ALL_CONTEXT		            2


// Mapping Argumentos ==============================================

#define	M4CL_ARGUMENT_MAPPING_INPUT				        1
#define	M4CL_ARGUMENT_MAPPING_OUTPUT				    2
#define	M4CL_ARGUMENT_MAPPING_REFERENCE			        3

// Triggers ========================================================

#define M4CL_VM_TRIGGER_ALL								0
#define M4CL_VM_TRIGGER_INSERTED						1
#define M4CL_VM_TRIGGER_UPDATEDDB						2
#define M4CL_VM_TRIGGER_DELETED							4
#define M4CL_VM_TRIGGER_CURRENT							8


// Cosas de sociedades =============================================

// Tipos de sociedad
#define	M4CL_ORGANIZATION_TYPE_NONE				        0
#define	M4CL_ORGANIZATION_TYPE_MONO				        1
#define	M4CL_ORGANIZATION_TYPE_MULTI			        2

// Tipos de sociedad para niveles 2
#define	M4CL_ORGANIZATION_L2_TYPE_NONE				    0
#define	M4CL_ORGANIZATION_L2_TYPE_MONO				    1
#define	M4CL_ORGANIZATION_L2_TYPE_MULTI				    2
#define	M4CL_ORGANIZATION_L2_TYPE_FATHER		        3
#define	M4CL_ORGANIZATION_L2_TYPE_ANY				    4

#define M4CL_ORGANIZATION_L2_MIN						0	//para chequeo de rangos
#define M4CL_ORGANIZATION_L2_MAX						4

// Find relationships
#define	M4CL_EQUAL_FUNCTION			                    1
#define	M4CL_DISTINCT_FUNCTION			                2
#define	M4CL_BIGGER_FUNCTION		                    3
#define	M4CL_SMALLER_FUNCTION		                    4
#define	M4CL_BIGGER_OR_EQUAL_FUNCTION		            5
#define	M4CL_SMALLER_OR_EQUAL_FUNCTION		            6
#define	M4CL_EQUAL_FUNCTION_OR_NULL		                7
#define	M4CL_DISTINCT_FUNCTION_OR_NULL		            8
#define	M4CL_BIGGER_FUNCTION_OR_NULL		            9
#define	M4CL_SMALLER_FUNCTION_OR_NULL		            10
#define	M4CL_BIGGER_OR_EQUAL_FUNCTION_OR_NULL           11
#define	M4CL_SMALLER_OR_EQUAL_FUNCTION_OR_NULL          12

#define	M4CL_CASE_EQUAL_FUNCTION						13
#define	M4CL_CASE_DISTINCT_FUNCTION						14
#define	M4CL_CASE_BIGGER_FUNCTION						15
#define	M4CL_CASE_SMALLER_FUNCTION						16
#define	M4CL_CASE_BIGGER_OR_EQUAL_FUNCTION				17
#define	M4CL_CASE_SMALLER_OR_EQUAL_FUNCTION				18
#define	M4CL_CASE_EQUAL_FUNCTION_OR_NULL				19
#define	M4CL_CASE_DISTINCT_FUNCTION_OR_NULL				20
#define	M4CL_CASE_BIGGER_FUNCTION_OR_NULL				21
#define	M4CL_CASE_SMALLER_FUNCTION_OR_NULL				22
#define	M4CL_CASE_BIGGER_OR_EQUAL_FUNCTION_OR_NULL		23
#define	M4CL_CASE_SMALLER_OR_EQUAL_FUNCTION_OR_NULL		24

#define	M4CL_MATCH_REGEX								25
#define	M4CL_MATCH_REGEX_OR_NULL						26
#define	M4CL_CASE_MATCH_REGEX							27
#define	M4CL_CASE_MATCH_REGEX_OR_NULL					28


#define	M4CL_MAX_VARIANT_COMPARE_FUNCTION		        28

// Tipos de bookmark
#define M4CL_BOOKMARK_BY_POSITION						1
#define M4CL_BOOKMARK_BY_PK								2

// para los indices ways
#define	M4CL_INDEX_WAY_ASCENDING		                1
#define	M4CL_INDEX_WAY_DESCENDING		                2

// para prioridades
#define	M4CL_RULE_PRIORITY_ZERO		                    0
#define	M4CL_RULE_PRIORITY_ONE		                    1
#define	M4CL_RULE_PRIORITY_TWO		                    2
#define	M4CL_RULE_PRIORITY_THEE		                    3

#define	M4CL_RULE_MIN_PRIORITY		                    3
#define	M4CL_RULE_MAX_PRIORITY		                    0

// para los tipo cliente servidor
#define	M4CL_CSTYPE_UNKNOWN								0x00
#define	M4CL_CSTYPE_FRONT								0x01
#define	M4CL_CSTYPE_BACK								0x02
#define	M4CL_CSTYPE_BOTH								0x03
#define	M4CL_CSTYPE_DEBUG								0x06
#define	M4CL_CSTYPE_MAPPED								0x07

#define M4CL_CSTYPE_MIN_NUMBER							0x01 //para chequear rangos, mantener actualizado
#define M4CL_CSTYPE_MAX_NUMBER							0x07

#define M4CL_CSTYPE_REAL_MAPPED							0x0F // Para las estadísticas. Debe estar fuera de rango

// para la lookup de nivel 2
#define	M4CL_CSTYPE_L2_UNKNOWN							0x00
#define	M4CL_CSTYPE_L2_FRONT							0x01
#define	M4CL_CSTYPE_L2_BACK								0x02
#define	M4CL_CSTYPE_L2_SITE								0x03
#define	M4CL_CSTYPE_L2_BOTH								0x04
#define	M4CL_CSTYPE_L2_FATHER							0x05

#define M4CL_CSTYPE_L2_MIN_NUMBER						0x01 //para chequear rangos, mantener actualizado
#define M4CL_CSTYPE_L2_MAX_NUMBER						0x05

// execution type
#define M4CL_CSEXETYPE_OLTP								0x00
#define M4CL_CSEXETYPE_PROXY							0x01
#define M4CL_CSEXETYPE_AUTO								0x02

// cacheable type
#define M4CL_CACHEABLE_FALSE							0
#define M4CL_CACHEABLE_TRUE								1
#define M4CL_CACHEABLE_VOLATILE							2

// tipos de servicios
#define	M4CL_EXECUTION_SERVICE							"OLTP"
#define M4CL_SESSION_SERVICE							"SESSION"
#define M4CL_PROXY_SERVICE								"PROXY"

// para el ambito de serializacion
#define M4CL_RECORD_SERIAL_ALL                          0
#define M4CL_RECORD_SERIAL_DELTA                        1
#define M4CL_RECORD_SERIAL_ADVANCED                     2

#define M4CL_ITEM_SERIAL_ALL                            0
#define M4CL_ITEM_SERIAL_MAPPED                         1

// para tipos de filtros predefinidos
#define M4CL_FILTER_TYPE_QBF                            1
#define M4CL_FILTER_TYPE_MAPI                           2
#define M4CL_FILTER_TYPE_FIND                           3

// para tipos de filtros multiselección
#define M4CL_FILTER_BEHAVIOUR_QBF                       0
#define M4CL_FILTER_BEHAVIOUR_MULTI_ALL                 1
#define M4CL_FILTER_BEHAVIOUR_ALL                       2

// para la seguridad
#define	M4CL_SEC_ENABLE									1
#define	M4CL_SEC_DISABLE								0

#define	M4CL_SEC_STRING_ENABLE							"1"
#define	M4CL_SEC_STRING_DISABLE							"0"

//para las formulas
#define M4CL_NO_LENGUAJE								0
#define M4CL_LENGUAGE_LN4								M4CL_INTERN_OBJECT_LN4 //por claridad
#define M4CL_LENGUAGE_CPP								M4CL_INTERN_OBJECT_CPP

#define M4CL_INTERN_OBJECT_LN4							1
#define M4CL_INTERN_OBJECT_CPP							2
#define M4CL_INTERN_OBJECT_DLL							3	//Dll de interfaz fijo

//para los códigos de función en L3 y upper
#define M4CL_L3_NO_OPERATION							0
#define M4CL_L3_EXECUTE_ITEM							1
#define M4CL_L3_READ_ITEM								2
#define M4CL_L3_ASSIGN_ITEM								3
#define M4CL_L3_EXECUTE_ATTRIBUTE						4
#define M4CL_L3_READ_ATTRIBUTE							5

// tipos de presentaciones
#define M4CL_PRESENTATION_DEFINITION					0
#define M4CL_PRESENTATION_OBL							1
#define M4CL_PRESENTATION_XML							2
#define M4CL_PRESENTATION_VERSION						3

// defines de los tamaños maximos
#define	M4CL_MAX_T3_ID		                            30
#define	M4CL_MAX_TI_ID		                            30
#define	M4CL_MAX_NODE_ID		                        30
#define	M4CL_MAX_ITEM_ID		                        30
#define	M4CL_MAX_GROUP_ID		                        30
#define	M4CL_MAX_ITEM_FORMULA_ID                        30
#define M4CL_MAX_RULE_ID			                    30
#define	M4CL_MAX_ARG_ID	                                30
#define	M4CL_MAX_DMD_ID	                                30
#define	M4CL_MAX_DMD_COMPONENT_ID	                    30
#define	M4CL_MAX_DMD_FIELD_ID	                        30
#define	M4CL_MAX_OBJECT_ID	                            30
#define	M4CL_MAX_METHOD_ID	                            30
#define	M4CL_MAX_CURRENCY_ID							30
#define	M4CL_MAX_SENTENCE_ID							30
#define	M4CL_MAX_FILTER_ID								30
#define	M4CL_MAX_MASK_ID								30
#define	M4CL_MAX_ROLE_ID								30
#define	M4CL_MAX_RSM_ID									30
#define	M4CL_MAX_ORG_ID									30
#define	M4CL_MAX_EXCHANGE_TYPE_ID						30
#define	M4CL_MAX_INDEX_ID								30
#define	M4CL_MAX_TAG_ID									100
#define	M4CL_MAX_TOTAL_TAG_ID							130
#define	M4CL_MAX_ZVALUE									30
#define	M4CL_MAX_BMETHOD_ID								30


#define	M4CL_MAX_NITEM_FORMULA_ID                       32

#define	M4CL_MAX_PRESENTATION_ID						40
#define	M4CL_MAX_MAPPING_ID								40

#define	M4CL_MAX_ITEM_NAME		                        50


// varios
#define	M4CL_DEFAULT_DELTA		                        1
//#define M4_DATE_SCALE                                   1000000


// LN4 ========================================

// tipos de case sensitive en LN4
#define M4CL_UNCHANGED									0
#define M4CL_UPPERCASE									1
#define M4CL_LOWERCASE									-1

//----------------------------------
//funciones y argumentos
//----------------------------------

#define M4CL_MAX_NOMBRE_FUNC							50
#define M4CL_MAX_PARAM									100

//----------------------------------
//funciones y argumentos
//----------------------------------
#define M4CL_DEFAULT_STACK_SIZE							256
#define M4CL_DEFAULT_STACK_MAX_SIZE						65536 // (resize al doble 8 veces)



// para las del sistema
#define M4CL_ITEM_ROOT_LOAD								"ROOTLOAD"
#define M4CL_ITEM_EXTENDED_LOAD							"EXTENDEDLOAD"
#define M4CL_ITEM_BEGIN_TRANSACTION						"BEGINTRANSACTION"
#define M4CL_ITEM_END_TRANSACTION						"ENDTRANSACTION"
// Only for Jesus
#define M4CL_ITEM_LOAD									"LOAD"
#define M4CL_ITEM_LOAD_PRG								"LOAD_PRG"
#define M4CL_ITEM_LOAD_BLK								"LOAD_BLK"
#define M4CL_ITEM_INSERT_PRG							"INSERT_PRG"
#define M4CL_ITEM_INSERT_BLK							"INSERT_BLK"
#define M4CL_ITEM_DELETE_PRG							"DELETE_PRG"
#define M4CL_ITEM_DELETE_BLK							"DELETE_BLK"
#define M4CL_ITEM_UPDATE_PRG							"UPDATE_PRG"
#define M4CL_ITEM_UPDATE_BLK							"UPDATE_BLK"
#define M4CL_ITEM_PERSIST_TREE							"PERSIST_TREE"

//Item del sistema para el borrado de L2 en canales proxy
#define M4CL_ITEM_ERASE_ALL_L2_INSTANCES				"ERASE_ALL_L2_INSTANCES"

// Item del canal sessión que guarda el nombre del canal moneda
#define M4_SCH_SESSION_T3_EXCHANGE_RATES				"T3_EXCHANGE_RATES"

// identificadores de instancias de canal
#define M4_INSTANCE_EXCHANGE_RATES						"EXCHANGE_RATES"
#define M4_INSTANCE_SYS_STATISTICS						"STATISTICS"
#define M4_INSTANCE_M4_ERRORS							"M4_ERRORS"

#define M4_T3_EXCHANGE_RATES							"EXCHANGE_RATES"
#define M4_T3_SYS_STATISTICS							"SYS_STATISTICS"
#define M4_T3_M4_ERRORS									"M4_ERRORS"

// identificadores de canales
#define M4_CHANNEL_SCHT3S_TI_CHANNEL					"TI_CHANNEL"
#define M4_CHANNEL_SCHT3S_DESIGN_CHANNEL				"DESIGN_CHANNEL"
#define M4_CHANNEL_SCHT3S_DESIGN_CHANNEL_PROXY			"DESIGN_CHANNEL_PROXY"
#define M4_CHANNEL_SCHT3S_T3_SYSTEM						"T3_SYSTEM"



//Autoload:
#define M4CL_AUTOLOAD_OFF								0
#define M4CL_AUTOLOAD_BLOCK								1
#define M4CL_AUTOLOAD_PRG								2
#define M4CL_AUTOLOAD_NODESAYS							3

// seguridad
#define M4CL_SEC_SUPER_USER								"M4ADM"

// advanced serialization modes : SYNC_TYPE
#define M4_SYNC_NONE									0
#define M4_SYNC_DOCUMENT								1
#define M4_SYNC_DOCUMENT_PERSIST						2
#define M4_SYNC_BLOCK									3
#define M4_SYNC_BLOCK_BRANCH							4
#define M4_SYNC_REGISTER								5
#define M4_SYNC_REGISTER_BRANCH							6
#define M4_SYNC_NULL									M4_SYNC_DOCUMENT

// Precision en la comparacion de Doubles y Fechas: if ( ABS(A-B)<precision ) then A==B
#define M4_LN4_MATH_PRECISION_MODF						0.5e-9	//Error añadido a un valor para evitar el error del 'modf'
#define M4_LN4_MATH_PRECISION							10e-9
#define M4_LN4_DATE_PRECISION							10e-7

// Character used to start the extension
#define M4CL_START_EXTENSION_OF_FILE_NAME				'.'


//codigos de error además de los de M4define

#define M4CL_USER_ERROR_ABORT_TRANSACTION				-2
#define M4CL_USER_ABORT_ALL_REQUEST						-3
#define M4CL_EXECUTION_INTERRUPTION						-4

// Meta4Object de chequeo de versión de metadatos

#define M4_CHANNEL_MD_VERSION							"SCH_MD_VERSION"

#define M4_NODE_MD_VERSION								"SCH_MD_VERSION"
#define M4_NODE_MD_GENERAL_DATA							"SCH_MD_GENERAL_DATA"

#define M4_ITEM_MD_VERSION_DT_MD_VERSION				"DT_MD_VERSION"
#define M4_ITEM_MD_VERSION_ID_MD						"ID_T3"
#define M4_ITEM_MD_VERSION_ID_MD_TYPE					"ID_MD_TYPE"
#define M4_ITEM_MD_VERSION_PARAM						"PARAM"
#define M4_METHOD_MD_VERSION_LOADBYDATE					"LOADBYDATE"
#define M4_METHOD_MD_VERSION_CHECK_M4OBJ_VERSION		"CHECK_M4OBJ_VERSION"

#define M4_ITEM_MD_GENERAL_MAX_DT_MD_VERSION			"MAX_DT_MD_VERSION"
#define M4_ITEM_MD_GENERAL_REPOSITORY_KEY				"REPOSITORY_KEY"
#define M4_METHOD_MD_GENERAL_GET_MAX_DT_MD_VERSION		"GET_MAX_DT_MD_VERSION"
#define M4_METHOD_MD_GENERAL_GET_REPOSITORY_KEY			"GET_REPOSITORY_KEY"


//## end module%3912A6B70398.epilog

#endif
