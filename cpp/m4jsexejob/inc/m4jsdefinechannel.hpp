//==============================================================================
//
// (c) Copyright  1991-1999 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             jsexecutors_dll.hpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:9/8/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================



#ifndef M4_JSDEFINECHANEL_HPP
#define M4_JSDEFINECHANEL_HPP

//----------------------------------------------------------------------------//
//                 NOMBRES DE CANALES                                         //
//----------------------------------------------------------------------------//

#define NAME_CHANNEL_JIT		"CH_JS_EXE_JIT"
#define NAME_CHANNEL_DEFS		"CH_ALL_JOB_DEFS"  
#define NAME_CHANNEL_EXECS		"CH_JS_EXECUTIONS"
#define NAME_CHANNEL_GROUP_DEF	"CH_ALL_JOB_GROUP_DEFS"
#define CHANNEL_JIT				"CH_JS_EXE_JIT"
#define CHANNEL_EXECUTIONS		"CH_JS_EXECUTIONS"



//---------------------------------------------------------------------------//
//                 NOMBRES DE NODOS                                           //
//---------------------------------------------------------------------------//

#define NAME_NODE_JOB_DEFS			"ALL_JOB_DEFS"
#define NAME_NODE_GROUP_DEFS		"ALL_JOB_GROUP_DEFS"
#define NAME_NODE_EXEC_JIT			"N_JS_EXE_JIT"
#define NAME_NODE_GROUP_EXEC		"N_JOB_GROUP_EXECS" 
#define NAME_NODE_JOB_EXECS			"N_JOB_EXECS"
#define NODE_JIT_PARAMS				"N_JS_OUT_PARAMS"
#define NODE_RESOURCE_EXTERNAL_JIT	"N_JS_EXTERNAL_FILES"
#define NODE_RESOURCE_EXTERNAL_EXEC	"N_RES_EXECS"
#define	NODE_PARAMS_JIT				"N_JS_OUT_PARAMS"
#define NODE_PARAM_EXEC				"N_JOB_PARAMETERS"


//------------------------------------------------------------------------//
//              NOMBRES DE CAMPOS                                         //
//------------------------------------------------------------------------//

#define M4_ID_JOB_GROUP_DEF       "ID_JOB_GROUP_DEF"
#define M4_ID_JOB_DEF             "ID_JOB_DEF"
#define M4_ID_JOB_GROUP_EXEC      "ID_JOB_GROUP_EXEC"
#define M4_ID_JOB_EXEC            "ID_JOB_EXEC"

//***************Nombre de campos que definen el job********************//

#define  M4_CODE_LN4_JIT          "APDU"
#define  M4_JOB_STRING_SECURITY   "CREDENTIAL"
#define  M4_IS_LAST_JOB_IN_BOX    "IS_LAST_IN_BOX"
#define  M4_ORDER_JOB_IN_BOX      "ORDER_IN_BOX"
#define  M4_JOB_NUM_PARAMETER     "NUM_PARAMETER"
#define  M4_NUM_EXECUTION         "NUM_EXECS"

//**********Nombre de campos que definen la ejecución de un job*********//

#define M4_DATE_SCHEC_EXECUTION   "DATE_SCHEDULED"
#define M4_DATE_START_TASK        "DATE_SUBMIT"
#define M4_DATE_END_TASK          "DATE_END"
#define M4_STATUS_EXECUTION       "ID_EXEC_STATUS"
#define M4_DATA_OUT_EXECUTION     "OUT"
#define M4_CODE_RETURN_EXECUTION  "RETURN_CODE"
#define M4_LOG_EXECUTION          "LOG"
#define M4_RESULT_EXECUTION_JIT   "JIT_RESULT"

//**********Nombres de campos de la definición de parámetros************//

#define M4_PARAMETER_NAME       "ARG_NAME"
#define M4_PARAMETER_TYPE       "ARG_TYPE"
#define M4_PARAMETER_VALUE      "PARAM_VALUE"
#define M4_PARAMETER_POSISITION "POSITION"


//*****************Nombres de campos de parámetros en ejecución**********//

#define ITEM_PARAM_VALUE_OUT_JIT "PARAM_VALUE"
#define ITEM_PARAM_NAME_OUT_JIT  "PARAM_NAME"


//****************Nombres de campos relacionados con ficheros************//

#define ITEM_TYPE_ACTION_RESOURCE         "FILE_STORAGE_TYPE"
#define ITEM_PATH_JIT_RESOURCE_EXTERNAL   "FILE_PATH"
#define ITEM_MAX_DELAY                    "DAY_TO_EXPIRE"
#define ITEM_EXISTS_FILE                  "EXISTS_FILE"

#define ITEM_TYPE_ACTION_EXEC             "RES_STORAGE_TYPE"
#define ITEM_NAME_RESOURCE				  "RES_NAME"
#define ITEM_PATH_RESOURCE		          "SEARCH_PATH"
#define ITEM_SIZE_RESOURCE                "CONTENT_SIZE"
#define ITEM_DATE_CREATED_RESOURCE        "CREATION_DATE"
#define ITEM_DATE_MAX_DELAY               "EXPIRATION_DATE"
#define ITEM_STATUS_RESOURCE              "STATUS"
#define ITEM_IS_ERASABLE                  "IS_ERASABLE"

//---------------------------------------------------------------------//
//          NOMBRE DE MÉTODOS                                         //
//---------------------------------------------------------------------//

#define M4_METHOD_FIND					"FINDREGISTER"
#define M4_METHOD_MOVE					"MOVETO"
#define M4_METHOD_EXECUTION_CODE_LN4	"EXE_JIT_WITH_PARAMS"
#define METHOD_CREATE_PARAM_JIT			"ADD_PARAM_OUT"
#define METHOD_LOAD_CHANNEL_EXEC		"READ_EXEC_WITH_DATE"
#define METHOD_COUNT_JOBS				"GET_NUM_JOBS"

#define METHOD_SAVE_FILE_EXTERNAL		"SAVE_BLOB_CONTENTS"

#define METHOD_LOAD					"LOAD_PARAMS"
#define METHOD_FIRST_OUT_PARAM		"GET_FIRST_OUTPUT_PARAM"
#define METHOD_NEXT_OUT_PARAM		"GET_NEXT_OUTPUT_PARAM"
#define METHOD_FIRST_IN_PARAM		"GET_FIRST_INPUT_PARAM"
#define METHOD_NEXT_IN_PARAM		"GET_NEXT_INPUT_PARAM"
#define METHOD_JIT					"EXE_JIT_WITH_PARAMS"
#define METHOD_ADD_PARAM_OUT		"ADD_PARAM_OUT"
#define METHOD_SET_OUT_PARAM		"SET_OUTPUT_VALUE"
#define METHOD_SET_PARAM			"SET_PARAM_VALUE"


//----------------------------------------------------------------------//
//            FILTROS PARA CARGAR CANALES                               //
//----------------------------------------------------------------------//


#define M4_FILTER_ID_BOX                   "BOX_ID"
#define M4_FILTER_ID_JOB                   "JOB_ID"
#define M4_FILTER_DATE_SCHEC               "BOX_DATE"


//----------------------------------------------------------------------//
//                 DEFINICIONES COMUNES                                 //
//---------------------------------------------------------------------//



//***********TIPO DE FLUJO DE LOS PARAMETROS****************************//

#define M4_FLOW_TYPE_INPUT       "1"
#define M4_FLOW_TYPE_OUTPUT      "2"


//************TIPO DE ACCIONES CON LOS RECURSOS***********************//

#define M4_TEMP_TYPE_ACTION_RESOURCE   "TEMPORARY"
#define M4_REF_TYPE_ACTION_RESOURCE    "R"
#define M4_OWN_TYPE_ACTION_RESOURCE    "I"
#define M4_OWNREF_TYPE_ACTION_RESOURCE "RI"


//**************ESTADO DE EJECUCION*************************************//


#define STATUS_WAITING         1
#define STATUS_EXECUTE         2
#define STATUS_EXECUTED        0
#define STATUS_CANCEL          3
#define STATUS_EXPIRED         4
#define STATUS_CANCELED_TOTAL  7
#define STATUS_ABORTED         10
#define STATUS_ABORTED_TOTAL   11
#define STATUS_NO_ASIGNADO      4
#define STATUS_INTERRUPTED      6


//*********************************Varios******************************//

#define M4_NO_IS_LAST_IN_BOX          0
#define FIRST_JOB_IN_BOX              1
#define M4_PARAM_REPEAT               -1
#define COND_ERROR                    -1
#define COND_EOF                       1


//*********************MENSAJES DE ERROR PARA EL JOB SCHEDULER***********//
#define M4_ABORT_AND_ERROR         -10
#define M4_VIOLATION_SECURITY_JS   -11
#define M4_ABORT_TASK              -12

#endif
