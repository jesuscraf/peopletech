//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                defmain.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                01-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module...
//
//
//==============================================================================

#ifndef __DEFMAIN_HPP__
#define __DEFMAIN_HPP__

#include "m4stl.hpp"
#include "cldefine.hpp"
#include "ldb_def.hpp"
#include <math.h>

extern m4uint8_t g_iEncodingMode;

typedef string*		PString;


#ifdef _M4LDB_DEBUG
class ClLDBInspector;
extern ClLDBInspector *g_pInspector;


typedef map<string, ClLDBInspector*, less<string> >	MpPFile;
typedef MpPFile*									PMpPFile;
typedef MpPFile::const_iterator						itMpPFile;
#endif

typedef vector<m4int32_t>						VcVirtualParam;
typedef VcVirtualParam*							PVcVirtualParam;
typedef VcVirtualParam::const_iterator			itVcVirtualParam;

//<<<Pedro. Indicaremos si una relacion proviene de seguridad.
//En principio solo tenemos estas dos posibilidades.
//Distinguimos entre sentencia API que venga por SYS_SENTENCE...
//y sentencia API que provenga de seguridad.
const m4uint8_t M4LDB_APISQL_FROM_APISQL		= 0;	
const m4uint8_t M4LDB_APISQL_FROM_SECURITY		= 1;
const m4uint8_t M4LDB_APISQL_FROM_SECURITY_XJOIN	= 2;
//>>>Pedro


// Numero maximo de tablas fisicas por tabla logica.
// Si cambia hay que cambiar la mascara (StMetaObject)
m4cint8_t M4_LDB_MAX_REAL_TABLES			= 32;

// Manejador interno.
m4cint8_t M4LDB_INTERNAL_HANDLE				= -1;

// Fechas de menos y mas infinito.
const m4date_t M4LDB_MINUS_INF_DATE				= M4_MINUS_INF_DATE;
const m4date_t M4LDB_PLUS_INF_DATE				= M4_PLUS_INF_DATE;


const m4int32_t M4LDB_DEFAULT_LANGUAGE			= 1;

const m4int32_t M4LDB_MAX_NUM_CURSOR			= 240;

// Estados.
enum eStateType_t
{
	M4LDB_OPEN = 1,
	M4LDB_CLOSE
};
   

// Modo de trabajo unicode  
enum eEncodingMode_t
{
	M4LDB_ENCODING_ANSI = 0,
	M4LDB_ENCODING_MIXED = 1,
	M4LDB_ENCODING_UNICODE = 2
};


// Identificadores de las funciones.
enum eFuncType_t
{
	M4LDB_FTN_NONE = 1,
	// Identificadores de las funciones de valor por defecto.
	M4LDB_DEF_GMT_DATE,
	M4LDB_DEF_TRANSACTION_DATE,
	M4LDB_DEF_CLOSING_DATE,
	M4LDB_DEF_CURRENT_DATE,
	M4LDB_DEF_NUMBER,
	M4LDB_DEF_STRING,
	M4LDB_DEF_DATE,
	M4LDB_DEF_ORDINAL,
	M4LDB_DEF_ORDINAL_PK,
	// Identificadores de las funciones de validación.
	M4LDB_CHK_NUMBER,
	M4LDB_CHK_STRING,
	M4LDB_CHK_DATE,
	M4LDB_CHK_INTERVAL_NUMBER,
	M4LDB_CHK_INTERVAL_DATE,
	// Más funciones
	M4LDB_DEF_SESSION_ITEM,
	M4LDB_DEF_PARENT_ORG_LEVEL,
	M4LDB_DEF_ORDINAL_PK_NO_DATES
};


// Identificadores de funciones de conversion.
enum eConvFunc_t 
{
	M4LDB_CONV_FUNC_NONE = 1,
	M4LDB_CONV_FUNC_TO_UPPER, 
	M4LDB_CONV_FUNC_TO_LOWER,
	M4LDB_CONV_FUNC_FIRST_CAP_LETTER
};


// Tipos de temporalidad.
enum eTempType_t
{
	M4LDB_ETHERN = 1,
	M4LDB_1_PERIOD,
	M4LDB_N_PERIOD
};


// Tipos de historicos.
enum eHistType_t
{
	M4LDB_N_PERIOD_WITHOUT_AUTO = 0,
	M4LDB_N_PERIOD_WITH_OVERLAPED,
	M4LDB_N_PERIOD_AUTOMATIC_ADAPTATION,
	M4LDB_N_PERIOD_CLOSE_LAST_REGISTER
};


// Tipos de validación.
enum eCondValidType_t
{
	M4LDB_NO_TESTED = 1,
	M4LDB_VALID_INCLUSION,
	M4LDB_VALID_PERIOD,
	M4LDB_VALID_COMPLETENESS
};


// Tipos de cascade para la fecha de fin.
enum eMoveDTEndOnCascType_t
{
	M4LDB_NO_MOVE_DT_END_ON_CASC = 0,
	M4LDB_MOVE_DT_END_ON_CASC,
	M4LDB_CLOSE_DT_END_ON_CASC
};


// Tipos de campos.
enum eFldType_t
{
	M4LDB_ODINAL_PK = -1,
	M4LDB_NORMAL = M4CL_INTERNAL_TYPE_NORMAL,
	M4LDB_DT_START = M4CL_INTERNAL_TYPE_START_DATE,
	M4LDB_DT_END = M4CL_INTERNAL_TYPE_END_DATE,
	M4LDB_DT_START_CORR = M4CL_INTERNAL_TYPE_COR_START_DATE,
	M4LDB_DT_END_CORR = M4CL_INTERNAL_TYPE_COR_END_DATE,
	M4LDB_DT_LAST_UPDATE = M4CL_INTERNAL_TYPE_T_DATE,
	M4LDB_TYPE_REG = M4CL_INTERNAL_TYPE_PROJECTION,
	M4LDB_TRANS_PTR = M4CL_INTERNAL_TYPE_TRANSLATION_PTR,
	M4LDB_TRANS_FLD = M4CL_INTERNAL_TYPE_TRANSLATION_FIELD,
	M4LDB_SEC_USER = M4CL_INTERNAL_TYPE_SECURITY_USER,
	M4LDB_APP_ROLE = M4CL_INTERNAL_TYPE_APP_ROLE,
	M4LDB_ID_EXECUTION = M4CL_INTERNAL_TYPE_ID_EXECUTION,
	M4LDB_ID_ORDER = M4CL_INTERNAL_TYPE_ID_ORDER,
	M4LDB_ID_OPERATION = M4CL_INTERNAL_TYPE_ID_OPERATION,
	M4LDB_ID_TYPE_UPDATE = M4CL_INTERNAL_TYPE_ID_TYPE_UPDATE,
	M4LDB_DT_OPERATION = M4CL_INTERNAL_TYPE_DT_OPERATION,
	M4LDB_ADD_INFO = M4CL_INTERNAL_TYPE_ADD_INFO,
	M4LDB_ID_ORGANIZATION = M4CL_INTERNAL_TYPE_ID_ORGANIZATION,
	M4LDB_ID_ORG_LEVEL = M4CL_INTERNAL_TYPE_ID_ORGANIZATION_LEVEL,
	M4LDB_ID_INHERIT_OPERATION = M4CL_INTERNAL_TYPE_ID_INHERITANCE_OPERATION,
	M4LDB_ID_DATASET = M4CL_INTERNAL_TYPE_ID_DATASET,
	M4LDB_TRAMIT_OPERATION = M4CL_INTERNAL_TYPE_TRAMIT_OPERATION,
	M4LDB_TRAMIT_DT_LAST_UPDATE = M4CL_INTERNAL_TYPE_TRAMIT_DT_LAST_UPDATE,
	M4LDB_RANGE_START = M4CL_INTERNAL_TYPE_RANGE_START,
	M4LDB_RANGE_END = M4CL_INTERNAL_TYPE_RANGE_END
};


// Tipos de orden en la relacion.
enum eRelType_t
{
	M4LDB_PARENT_RELATION = 1,
	M4LDB_CHILD_RELATION
};


// Tipos especiales de registros corregidos.
enum eRegType_t
{
	M4LDB_NORMAL_REG = 1,
	M4LDB_CLOSED_REG,
	M4LDB_PROJECTION_REG,
	M4LDB_NORMAL_LOCK_REG,
	M4LDB_CLOSED_LOCK_REG,
	M4LDB_PROJECTION_LOCK_REG
};


// Tipos de relación entre valores.
enum eDiffType_t
{
	M4LDB_LESS = -1,
	M4LDB_IDENTICAL = 0,
	M4LDB_GREATER = 1
};


// Tipos de sentencias de API SQL.
enum eApiSqlType_t
{
	M4LDB_APISQL_NONE = 0,
	M4LDB_APISQL_DELETE,
	M4LDB_APISQL_INSERT,
	M4LDB_APISQL_UPDATE,
	M4LDB_APISQL_SELECT,
	M4LDB_APISQL_REAL_SELECT,
	M4LDB_APISQL_REAL_STMT
};


// Clausulas de la sentencia API SQL.
enum eClauseType_t
{
	M4LDB_CLAUSE_BEGIN_STATEM = 1,
	M4LDB_CLAUSE_COLUMN ,
	M4LDB_CLAUSE_LONG_COLUMN,
	M4LDB_CLAUSE_FROM,
	M4LDB_CLAUSE_WHERE,
	M4LDB_CLAUSE_REST,
	M4LDB_CLAUSE_ORDER_BY,
	M4LDB_CLAUSE_END_STATEM
};


// Tipos de parámetros.
enum eParamType_t
{
	M4LDB_PAR_UNDEF = 0,
	M4LDB_PAR_ITEM,
	M4LDB_PAR_EXTERN,
	M4LDB_PAR_SESSION,
	M4LDB_PAR_INTERN,
	M4LDB_PAR_MINUS_INF,
	M4LDB_PAR_PLUS_INF,
	M4LDB_PAR_AUTO_FILTER
};
// A extinguir

enum eOuterJoin_t
{
	M4LDB_NO_OUTER_JOIN = 1,
	M4LDB_LEFT_OUTER_JOIN ,
	M4LDB_RIGHT_OUTER_JOIN 
};


enum eJoinType_t
{
	M4LDB_NORMAL_JOIN = 1,
	M4LDB_OUTER_JOIN
};


enum eOrgType_t
{
	M4LDB_UNDEF_ORGANIZATION = 0,
	M4LDB_MONO_ORGANIZATION,
	M4LDB_MULTI_ORGANIZATION,
	M4LDB_ORGANIZATION_GROUP
};

enum eState_t
{
	M4DBW_CLOSE,
	M4DBW_WAIT,
	M4DBW_OPEN
};


enum eRealSqlExecutionMode_t
{
	M4LDB_EXEC_REALSQL_NO_AUDIT					= 0,
	M4LDB_EXEC_REALSQL_AUDIT,
	M4LDB_EXEC_REALSQL_EXECUTION_NOT_ALLOWDED
};

// Acciones a realizar por la LDB en base al error ocurrido.
m4cint32_t M4LDB_ACT_GPF								= -1;
m4cint32_t M4LDB_ACT_UNDEFINED							= 0;
m4cint32_t M4LDB_ACT_SHUTDOWN							= 1;
m4cint32_t M4LDB_ACT_CHANGE_ERROR						= 2;
m4cint32_t M4LDB_ACT_RETRY								= 4;
m4cint32_t M4LDB_ACT_CHANGE_ERROR_IF_ORDINAL			= 8;
m4cint32_t M4LDB_ACT_RETRY_IF_ORDINAL					= 16;
m4cint32_t M4LDB_ACT_DESTROY_CONNECTION					= 32;
m4cint32_t M4LDB_ACT_DESTROY_STMT						= 64;
m4cint32_t M4LDB_ACT_DESTROY_CURSOR						= 128;
m4cint32_t M4LDB_ACT_NOTIF_RETRY_CONNECTION				= 256;
m4cint32_t M4LDB_ACT_DESTROY_ALL_CONNECTIONS			= 512;
m4cint32_t M4LDB_ACT_FLUSH_DICTIONARY					= 1024;


// Desactivación de validaciones desde el canal.
m4cint32_t M4LDB_INV_PK									= 1;
m4cint32_t M4LDB_INV_NO_PK								= 2;
m4cint32_t M4LDB_INV_NOT_NULL							= 4;
m4cint32_t M4LDB_INV_CONSTRAINT_FUNC					= 8;
m4cint32_t M4LDB_INV_CONCURRENCY						= 16;
m4cint32_t M4LDB_INV_FK_WITHOUT_PERIOD					= 32;
m4cint32_t M4LDB_INV_FK_WITH_PERIOD						= 64;
m4cint32_t M4LDB_INV_FR_WITHOUT_PERIOD					= 128;
m4cint32_t M4LDB_INV_FR_WITH_PERIOD						= 256;
m4cint32_t M4LDB_INV_UNIQUE_INDEX						= 512;
m4cint32_t M4LDB_INV_COMPLETENESS						= 1024;
m4cint32_t M4LDB_INV_AUDITORY							= 2048;
m4cint32_t M4LDB_INV_SECURITY							= 4096;
m4cint32_t M4LDB_PREVALIDATE_FK							= 8192;
m4cint32_t M4LDB_ALLOW_PREVIOUS_INI						= 16384;
m4cint32_t M4LDB_MULTITRANSLATING						= 32768;
m4cint32_t M4LDB_INV_WRITE_DT_LAST_UPDATE				= 65536;
m4cint32_t M4LDB_USE_SECURITY_VIEW						= 131072;
m4cint32_t M4LDB_NO_USE_SECURITY_VIEW					= 262144;


m4cint32_t M4LDB_SEC_MASK_DELETE						= 1;
m4cint32_t M4LDB_SEC_MASK_INSERT						= 2;
m4cint32_t M4LDB_SEC_MASK_UPDATE						= 4;
m4cint32_t M4LDB_SEC_MASK_SELECT						= 8;
m4cint32_t M4LDB_SEC_MASK_CORR_DELETE					= 16;
m4cint32_t M4LDB_SEC_MASK_CORR_INSERT					= 32;
m4cint32_t M4LDB_SEC_MASK_CORR_UPDATE					= 64;
m4cint32_t M4LDB_SEC_MASK_DEFUSE_CLOSING_DATE			= 128;
m4cint32_t M4LDB_SEC_MASK_HIST_AUTO_ADAPT				= 256;
m4cint32_t M4LDB_SEC_MASK_DELETE_ON_CASCADE				= 512;
m4cint32_t M4LDB_SEC_MASK_UPDATE_PK_ON_CASCADE			= 1024;
m4cint32_t M4LDB_SEC_MASK_UPDATE_DT_START_ON_CASCADE	= 2048;
m4cint32_t M4LDB_SEC_MASK_UPDATE_DT_END_ON_CASCADE		= 4096;
m4cint32_t M4LDB_SEC_MASK_CLOSE_DT_END_ON_CASCADE		= 8192;
m4cint32_t M4LDB_SEC_MASK_CLOSE_PREV_REGISTER			= 16384;

m4cint32_t M4LDB_DEFAULT_SEC_MASK						= 32639; //111111101111111
m4cint32_t M4LDB_SUPERROLE_SEC_MASK						= 32767; //111111111111111


m4cint32_t M4LDB_AUD_MASK_DELETE			= 1;
m4cint32_t M4LDB_AUD_MASK_INSERT			= 2;
m4cint32_t M4LDB_AUD_MASK_UPDATE			= 4;
m4cint32_t M4LDB_AUD_MASK_SELECT			= 8;
m4cint32_t M4LDB_AUD_MASK_DTL_DELETE		= 16;
m4cint32_t M4LDB_AUD_MASK_DTL_INSERT		= 32;
m4cint32_t M4LDB_AUD_MASK_DTL_UPDATE		= 64;
m4cint32_t M4LDB_AUD_MASK_AUDIT_SQL			= 128;

m4cint32_t M4LDB_DEFAULT_AUD_MASK			= 1023; //1111111111
m4cint32_t M4LDB_SUPERROLE_AUD_MASK			= 0;

// ¡Atención!, el limite máximo de relaciones entre tablas lógicas está el 99999.
// Para cambiar este límite modificar las constantes que siguen:

// Nombres de cursores auxiliares.
m4cint32_t M4LDB_VALIDATE_PK							= 1;
m4cint32_t M4LDB_VALIDATE_NO_PK							= 2;
m4cint32_t M4LDB_GET_HISTORY							= 3;
m4cint32_t M4LDB_GET_CORR_HISTORY						= 4;
m4cint32_t M4LDB_MOVE_ON_HISTORY						= 5;
m4cint32_t M4LDB_VALIDATE_COMP							= 6;
m4cint32_t M4LDB_CLOSE_CORR_REGISTER					= 7;
m4cint32_t M4LDB_DELETE_PROJECTION						= 8;
m4cint32_t M4LDB_INSERT_PROJECTION						= 9;
m4cint32_t M4LDB_VALIDATE_SECURITY						= 10;
m4cint32_t M4LDB_CURSOR_QUERY							= 11;
m4cint32_t M4LDB_CURSOR_AUDITORY						= 12;	
m4cint32_t M4LDB_CURSOR_AUDITORY_2						= 13;
m4cint32_t M4LDB_VALIDATE_ORGANIZATION					= 14;
m4cint32_t M4LDB_INHERIT_EXIST							= 15;
m4cint32_t M4LDB_INHERIT_DELETE							= 16;
m4cint32_t M4LDB_INHERIT_INSERT							= 17;
m4cint32_t M4LDB_INHERIT_VALIDATE						= 18;
m4cint32_t M4LDB_TRAMIT_EXIST							= 19;
m4cint32_t M4LDB_TRAMIT_DELETE							= 20;
m4cint32_t M4LDB_TRAMIT_INSERT							= 21;
m4cint32_t M4LDB_TRAMIT_INSERT_OBJECT					= 22;
m4cint32_t M4LDB_TRANSLATION_INSERT						= 23;
m4cint32_t M4LDB_TRANSLATION_VALIDATE_PK				= 24;
m4cint32_t M4LDB_TRANSLATION_UPDATE_PK					= 25;
m4cint32_t M4LDB_UPDATE_VERSION							= 26;
m4cint32_t M4LDB_VALIDATE_SECURITY_OTHER_CONNECTION		= 27;
m4cint32_t M4LDB_CURSOR_ORDINAL							= 1000;// Hasta 1999
m4cint32_t M4LDB_CURSOR_ORDINAL_PK						= 2000;// Hasta 2999
m4cint32_t M4LDB_SELECT_ORDINAL							= 3000;// Hasta 3999
m4cint32_t M4LDB_INSERT_ORDINAL_PK						= 4000;// Hasta 4999
m4cint32_t M4LDB_DELETE_ORDINAL_PK						= 5000;// Hasta 5999
m4cint32_t M4LDB_VALIDATE_FK							= 100000;// Hasta 199999
m4cint32_t M4LDB_VALIDATE_FR							= 200000;// Hasta 299999
m4cint32_t M4LDB_MOVE_ON_CASCADE						= 300000;// Hasta 399999
m4cint32_t M4LDB_DELETE_ON_CASCADE						= 400000;// Hasta 499999
m4cint32_t M4LDB_UPDATEPK_ON_CASCADE					= 500000;// Hasta 599999


// Mascaras de compatibilidad
m4cint32_t M4LDB_CLTY_UNDEFINED							= 0;
//m4cint32_t M4LDB_CLTY_NEW_REPOSITORY					= 1;
m4cint32_t M4LDB_CLTY_SECURITY_OPTIMIZATION				= 2;



// Indices globales de los diccionarios de la LDB.
// jcr Ahora se ordenan por tamano y nombre
const m4uint8_t M4LDB_SYS_DICT_DDBBS				= 0;
const m4uint8_t M4LDB_SYS_DICT_TRANS				= 1;
const m4uint8_t M4LDB_SYS_DICT_ERRORS				= 2;
const m4uint8_t M4LDB_SYS_DICT_FIELDS				= 3;
const m4uint8_t M4LDB_SYS_DICT_AUD_LOBJ				= 4;
const m4uint8_t M4LDB_SYS_DICT_CLS_DATE				= 5;
const m4uint8_t M4LDB_SYS_DICT_FOREIGNS				= 6;
const m4uint8_t M4LDB_SYS_DICT_ORG_TREE				= 7;
const m4uint8_t M4LDB_SYS_DICT_SEC_LOBJ				= 8;
const m4uint8_t M4LDB_SYS_DICT_FUNCTIONS			= 9;
const m4uint8_t M4LDB_SYS_DICT_ORG_FIELDS			= 10;
const m4uint8_t M4LDB_SYS_DICT_ORG_LEVELS			= 11;
const m4uint8_t M4LDB_SYS_DICT_SEC_FIELDS			= 12;
const m4uint8_t M4LDB_SYS_DICT_CONNECTIONS			= 13;
const m4uint8_t M4LDB_SYS_DICT_INHERITANCE			= 14;
const m4uint8_t M4LDB_SYS_DICT_REAL_OBJECTS			= 15;
const m4uint8_t M4LDB_SYS_DICT_SEC_LOBJ_FIL			= 16;
const m4uint8_t M4LDB_SYS_DICT_TRANS_FIELDS			= 17;
const m4uint8_t M4LDB_SYS_DICT_GLOBAL_PARAMS		= 18;
const m4uint8_t M4LDB_SYS_DICT_INSERT_PARAMS		= 19;
const m4uint8_t M4LDB_SYS_DICT_LOGIC_OBJECTS		= 20;
const m4uint8_t M4LDB_SYS_DICT_EXTENDED_TYPES		= 21;
const m4uint8_t M4LDB_SYS_DICT_FOREIGN_PARAMS		= 22;
const m4uint8_t M4LDB_SYS_DICT_MIRROR_OBJECTS		= 23;
const m4uint8_t M4LDB_SYS_DICT_CHILD_ORG_FIELDS		= 24;
const m4uint8_t M4LDB_SYS_DICT_DEFAULT_LANGUAGE		= 25;
const m4uint8_t M4LDB_SYS_DICT_ORG_LEVELS_TRANS		= 26;
const m4uint8_t M4LDB_SYS_DICT_DYNAMIC_LANGUAGES	= 27;
const m4uint8_t M4LDB_SYS_DICT_NEXT_ID_EXECUTION	= 28;
const m4uint8_t M4LDB_SYS_DICT_APPLICATION_PARAMS	= 29;

const m4uint8_t M4LDB_SYS_NODES_TOTAL				= 30;


#define  M4LDB_ITEM_SYS_DICT_APPLICATION_PARAMS_ID_KEY				(ClItemIndex) 0
#define  M4LDB_ITEM_SYS_DICT_APPLICATION_PARAMS_APP_VALUE			(ClItemIndex) 1

#define  M4LDB_ITEM_SYS_DICT_AUD_LOBJ_MASK							(ClItemIndex) 0
#define  M4LDB_ITEM_SYS_DICT_AUD_LOBJ_ID_RSM						(ClItemIndex) 1
#define  M4LDB_ITEM_SYS_DICT_AUD_LOBJ_ID_OBJECT						(ClItemIndex) 2

#define  M4LDB_ITEM_SYS_DICT_CLS_DATE_DT_CLOSING					(ClItemIndex) 0
#define  M4LDB_ITEM_SYS_DICT_CLS_DATE_ID_CLS_DATE					(ClItemIndex) 1

//Recordar que los literales deben estar ordenados segun el tamaño y segundo alfabeticamente.
#define  M4LDB_ITEM_SYS_DICT_CONNECTIONS_ID_CONN					(ClItemIndex) 0
#define  M4LDB_ITEM_SYS_DICT_CONNECTIONS_ID_DDBB					(ClItemIndex) 1
#define  M4LDB_ITEM_SYS_DICT_CONNECTIONS_CONN_STR					(ClItemIndex) 2
#define  M4LDB_ITEM_SYS_DICT_CONNECTIONS_CON_EXEC					(ClItemIndex) 3
#define  M4LDB_ITEM_SYS_DICT_CONNECTIONS_PASSWORD					(ClItemIndex) 4
#define  M4LDB_ITEM_SYS_DICT_CONNECTIONS_IS_CRITICAL				(ClItemIndex) 5
#define  M4LDB_ITEM_SYS_DICT_CONNECTIONS_ID_CONN_TYPE				(ClItemIndex) 6
#define  M4LDB_ITEM_SYS_DICT_CONNECTIONS_CURSOR_OPENED				(ClItemIndex) 7
#define  M4LDB_ITEM_SYS_DICT_CONNECTIONS_MAX_NUM_CURSOR				(ClItemIndex) 8
#define  M4LDB_ITEM_SYS_DICT_CONNECTIONS_ISOLATION_LEVEL			(ClItemIndex) 9

#define  M4LDB_ITEM_SYS_DICT_DDBBS_ID_DDBB							(ClItemIndex) 0
#define  M4LDB_ITEM_SYS_DICT_DDBBS_COMPLETE_LOAD					(ClItemIndex) 1

#define  M4LDB_ITEM_SYS_DICT_ERRORS_ID_DDBB							(ClItemIndex) 0
#define  M4LDB_ITEM_SYS_DICT_ERRORS_ID_ERROR						(ClItemIndex) 1
#define  M4LDB_ITEM_SYS_DICT_ERRORS_ARGUMENT1						(ClItemIndex) 2
#define  M4LDB_ITEM_SYS_DICT_ERRORS_ID_ACTION						(ClItemIndex) 3
#define  M4LDB_ITEM_SYS_DICT_ERRORS_ID_ERROR2						(ClItemIndex) 4

#define  M4LDB_ITEM_SYS_DICT_EXTENDED_TYPES_PREC					(ClItemIndex) 0
#define  M4LDB_ITEM_SYS_DICT_EXTENDED_TYPES_SCALE					(ClItemIndex) 1
#define  M4LDB_ITEM_SYS_DICT_EXTENDED_TYPES_ID_TYPE					(ClItemIndex) 2
#define  M4LDB_ITEM_SYS_DICT_EXTENDED_TYPES_ID_M4_TYPE				(ClItemIndex) 3
#define  M4LDB_ITEM_SYS_DICT_EXTENDED_TYPES_DEFAULT_ARGS			(ClItemIndex) 4
#define  M4LDB_ITEM_SYS_DICT_EXTENDED_TYPES_IS_ENCRYPTED			(ClItemIndex) 5
#define  M4LDB_ITEM_SYS_DICT_EXTENDED_TYPES_CONSTRAINT_ARGS			(ClItemIndex) 6
#define  M4LDB_ITEM_SYS_DICT_EXTENDED_TYPES_ID_CONVERT_FUNC			(ClItemIndex) 7
#define  M4LDB_ITEM_SYS_DICT_EXTENDED_TYPES_ID_DEFAULT_FUNC			(ClItemIndex) 8
#define  M4LDB_ITEM_SYS_DICT_EXTENDED_TYPES_ID_CONSTRAINT_FUNC		(ClItemIndex) 9

#define  M4LDB_ITEM_SYS_DICT_FIELDS_POS_PK							(ClItemIndex) 0
#define  M4LDB_ITEM_SYS_DICT_FIELDS_ID_TYPE							(ClItemIndex) 1
#define  M4LDB_ITEM_SYS_DICT_FIELDS_ID_FIELD						(ClItemIndex) 2
#define  M4LDB_ITEM_SYS_DICT_FIELDS_NOT_NULL						(ClItemIndex) 3
#define  M4LDB_ITEM_SYS_DICT_FIELDS_ID_OBJECT						(ClItemIndex) 4
#define  M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD					(ClItemIndex) 5
#define  M4LDB_ITEM_SYS_DICT_FIELDS_ID_REAL_OBJECT					(ClItemIndex) 6
#define  M4LDB_ITEM_SYS_DICT_FIELDS_CTRL_REPETITION					(ClItemIndex) 7
#define  M4LDB_ITEM_SYS_DICT_FIELDS_ID_INTERNAL_FIELD				(ClItemIndex) 8

#define  M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_OBJECT						(ClItemIndex) 0
#define  M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_COMPLETE					(ClItemIndex) 1
#define  M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_RELATION					(ClItemIndex) 2
#define  M4LDB_ITEM_SYS_DICT_FOREIGNS_CHILD_REQUIRED				(ClItemIndex) 3
#define  M4LDB_ITEM_SYS_DICT_FOREIGNS_DELETE_CASCADE				(ClItemIndex) 4
#define  M4LDB_ITEM_SYS_DICT_FOREIGNS_DT_END_CASCADE				(ClItemIndex) 5
#define  M4LDB_ITEM_SYS_DICT_FOREIGNS_DT_START_CASCADE				(ClItemIndex) 6
#define  M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_CONDUCT_VALID				(ClItemIndex) 7
#define  M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_FOREIGN_OBJECT				(ClItemIndex) 8
#define  M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_RELATION_ORDER				(ClItemIndex) 9
#define  M4LDB_ITEM_SYS_DICT_FOREIGNS_IS_COM_MANDAT_VAL				(ClItemIndex) 10
#define  M4LDB_ITEM_SYS_DICT_FOREIGNS_ALLOW_PREVIOUS_INI			(ClItemIndex) 11

#define  M4LDB_ITEM_SYS_DICT_FOREIGN_PARAMS_ID_FIELD				(ClItemIndex) 0
#define  M4LDB_ITEM_SYS_DICT_FOREIGN_PARAMS_ID_OBJECT				(ClItemIndex) 1
#define  M4LDB_ITEM_SYS_DICT_FOREIGN_PARAMS_ID_RELATION				(ClItemIndex) 2
#define  M4LDB_ITEM_SYS_DICT_FOREIGN_PARAMS_ID_FOREIGN_FIELD		(ClItemIndex) 3
#define  M4LDB_ITEM_SYS_DICT_FOREIGN_PARAMS_ID_FOREIGN_OBJECT		(ClItemIndex) 4

#define  M4LDB_ITEM_SYS_DICT_FUNCTIONS_ID_DDBB						(ClItemIndex) 0
#define  M4LDB_ITEM_SYS_DICT_FUNCTIONS_ID_FUNCTION					(ClItemIndex) 1
#define  M4LDB_ITEM_SYS_DICT_FUNCTIONS_FUNCTION_HEADER				(ClItemIndex) 2

#define  M4LDB_ITEM_SYS_DICT_GLOBAL_PARAMS_ID_PROPERTY				(ClItemIndex) 0
#define  M4LDB_ITEM_SYS_DICT_GLOBAL_PARAMS_PROPERTY_VALUE			(ClItemIndex) 1

#define  M4LDB_ITEM_SYS_DICT_INHERITANCE_ID_OBJECT					(ClItemIndex) 0
#define  M4LDB_ITEM_SYS_DICT_INHERITANCE_ID_AUX_OBJECT				(ClItemIndex) 1

#define  M4LDB_ITEM_SYS_DICT_INSERT_PARAMS_ID_EXECUTION				(ClItemIndex) 0

#define  M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_CORRECTED				(ClItemIndex) 0
#define  M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT				(ClItemIndex) 1
#define  M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_NP_TYPE				(ClItemIndex) 2
#define  M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_CLS_DATE				(ClItemIndex) 3
#define  M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_ORG_TYPE				(ClItemIndex) 4
#define  M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_MIRROR_TYPE				(ClItemIndex) 5
#define  M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_HAS_AUDITORY				(ClItemIndex) 6
#define  M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_HAS_SECURITY				(ClItemIndex) 7
#define  M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_ORG_LEVEL				(ClItemIndex) 8
#define  M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_TEMPORARY				(ClItemIndex) 9
#define  M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_IS_CACHEABLE				(ClItemIndex) 10
#define  M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_COMPATIBILITY			(ClItemIndex) 11
#define  M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_COMPLETE_LOAD			(ClItemIndex) 12
#define  M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT_AUX			(ClItemIndex) 13
#define  M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_INHERITS_DATA			(ClItemIndex) 14
#define  M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT_TYPE			(ClItemIndex) 15
#define  M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_HAS_INHERITANCE			(ClItemIndex) 16
#define  M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_AUDIT_OBJECT			(ClItemIndex) 17
#define  M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_TRANS_REAL_OBJECT		(ClItemIndex) 18

#define  M4LDB_ITEM_SYS_DICT_MIRROR_OBJECTS_ID_OBJECT					(ClItemIndex) 0
#define  M4LDB_ITEM_SYS_DICT_MIRROR_OBJECTS_ID_REAL_OBJECT				(ClItemIndex) 1
#define  M4LDB_ITEM_SYS_DICT_MIRROR_OBJECTS_ID_MIRROR_REAL_OBJECT		(ClItemIndex) 2
#define  M4LDB_ITEM_SYS_DICT_MIRROR_OBJECTS_ID_MIRROR_TRANS_REAL_OBJECT	(ClItemIndex) 3

#define  M4LDB_ITEM_SYS_DICT_NEXT_ID_EXECUTION_ID_EXECUTION			(ClItemIndex) 0

#define  M4LDB_ITEM_SYS_DICT_ORG_FIELDS_POS_PK						(ClItemIndex) 0
#define  M4LDB_ITEM_SYS_DICT_ORG_FIELDS_ID_FIELD					(ClItemIndex) 1
#define  M4LDB_ITEM_SYS_DICT_ORG_FIELDS_ID_OBJECT					(ClItemIndex) 2
#define  M4LDB_ITEM_SYS_DICT_ORG_FIELDS_HAS_PARENT					(ClItemIndex) 3
#define  M4LDB_ITEM_SYS_DICT_ORG_FIELDS_ID_INTERNAL_FIELD			(ClItemIndex) 4

#define  M4LDB_ITEM_SYS_DICT_CHILD_ORG_FIELDS_ID_FIELD				(ClItemIndex) 0
#define  M4LDB_ITEM_SYS_DICT_CHILD_ORG_FIELDS_ID_OBJECT				(ClItemIndex) 1
#define  M4LDB_ITEM_SYS_DICT_CHILD_ORG_FIELDS_ID_CHILD_FIELD		(ClItemIndex) 2

#define  M4LDB_ITEM_SYS_DICT_ORG_LEVELS_DESC2									  0
#define  M4LDB_ITEM_SYS_DICT_ORG_LEVELS_DESC3									  1
#define  M4LDB_ITEM_SYS_DICT_ORG_LEVELS_DESC4									  2
#define  M4LDB_ITEM_SYS_DICT_ORG_LEVELS_DESC5									  3
#define  M4LDB_ITEM_SYS_DICT_ORG_LEVELS_DESC6									  4
#define  M4LDB_ITEM_SYS_DICT_ORG_LEVELS_DESC7									  5
#define  M4LDB_ITEM_SYS_DICT_ORG_LEVELS_DESC8									  6
#define  M4LDB_ITEM_SYS_DICT_ORG_LEVELS_ID_LEVEL					(ClItemIndex) 7

#define  M4LDB_ITEM_SYS_DICT_ORG_LEVELS_DESC									  0

#define  M4LDB_ITEM_SYS_DICT_ORG_LEVELS_TRANS_DESC					(ClItemIndex) 0
#define  M4LDB_ITEM_SYS_DICT_ORG_LEVELS_TRANS_ID_LEVEL				(ClItemIndex) 1
#define  M4LDB_ITEM_SYS_DICT_ORG_LEVELS_TRANS_ID_LANGUAGE			(ClItemIndex) 2

#define  M4LDB_ITEM_SYS_DICT_ORG_TREE_ID_LEVEL						(ClItemIndex) 0
#define  M4LDB_ITEM_SYS_DICT_ORG_TREE_IS_DUPLICATED					(ClItemIndex) 1
#define  M4LDB_ITEM_SYS_DICT_ORG_TREE_ID_ORGANIZATION				(ClItemIndex) 2
#define  M4LDB_ITEM_SYS_DICT_ORG_TREE_ID_PAR_ORGANIZATION			(ClItemIndex) 3

#define  M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_CONN					(ClItemIndex) 0
#define  M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_OBJECT					(ClItemIndex) 1
#define  M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_REAL_OBJECT			(ClItemIndex) 2
#define  M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_TRAMIT_VIEW			(ClItemIndex) 3
#define  M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_IS_TRANSLATION			(ClItemIndex) 4
#define  M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_INHERIT_VIEW			(ClItemIndex) 5
#define  M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_MIRROR_REAL_OBJECT		(ClItemIndex) 6

#define  M4LDB_ITEM_SYS_DICT_SEC_FIELDS_ID_RSM						(ClItemIndex) 0
#define  M4LDB_ITEM_SYS_DICT_SEC_FIELDS_IS_READ						(ClItemIndex) 1
#define  M4LDB_ITEM_SYS_DICT_SEC_FIELDS_ID_FIELD					(ClItemIndex) 2
#define  M4LDB_ITEM_SYS_DICT_SEC_FIELDS_IS_WRITE					(ClItemIndex) 3
#define  M4LDB_ITEM_SYS_DICT_SEC_FIELDS_ID_OBJECT					(ClItemIndex) 4

#define  M4LDB_ITEM_SYS_DICT_SEC_LOBJ_MASK							(ClItemIndex) 0
#define  M4LDB_ITEM_SYS_DICT_SEC_LOBJ_ID_RSM						(ClItemIndex) 1
#define  M4LDB_ITEM_SYS_DICT_SEC_LOBJ_ID_OBJECT						(ClItemIndex) 2
#define  M4LDB_ITEM_SYS_DICT_SEC_LOBJ_COMPLETE_LOAD					(ClItemIndex) 3
#define  M4LDB_ITEM_SYS_DICT_SEC_LOBJ_HAS_SECURITY_FLD				(ClItemIndex) 4

#define  M4LDB_ITEM_SYS_DICT_SEC_LOBJ_FIL_JOIN						(ClItemIndex) 0
#define  M4LDB_ITEM_SYS_DICT_SEC_LOBJ_FIL_FILTER					(ClItemIndex) 1
#define  M4LDB_ITEM_SYS_DICT_SEC_LOBJ_FIL_ID_RSM					(ClItemIndex) 2
#define  M4LDB_ITEM_SYS_DICT_SEC_LOBJ_FIL_ID_USE					(ClItemIndex) 3
#define  M4LDB_ITEM_SYS_DICT_SEC_LOBJ_FIL_ID_FILTER					(ClItemIndex) 4
#define  M4LDB_ITEM_SYS_DICT_SEC_LOBJ_FIL_ID_OBJECT					(ClItemIndex) 5

#define  M4LDB_ITEM_SYS_DICT_TRANS_ID_FIELD							(ClItemIndex) 0
#define  M4LDB_ITEM_SYS_DICT_TRANS_ID_OBJECT						(ClItemIndex) 1
#define  M4LDB_ITEM_SYS_DICT_TRANS_ID_FIELD_AUX						(ClItemIndex) 2

#define  M4LDB_ITEM_SYS_DICT_TRANS_FIELDS_ID_LANG					(ClItemIndex) 0
#define  M4LDB_ITEM_SYS_DICT_TRANS_FIELDS_ID_FIELD					(ClItemIndex) 1
#define  M4LDB_ITEM_SYS_DICT_TRANS_FIELDS_ID_OBJECT					(ClItemIndex) 2
#define  M4LDB_ITEM_SYS_DICT_TRANS_FIELDS_ID_TRANS_FIELD			(ClItemIndex) 3

#define  M4LDB_ITEM_SYS_DICT_DEFAULT_LANGUAGE_ID_LANGUAGE			(ClItemIndex) 0

#define  M4LDB_ITEM_SYS_DICT_DYNAMIC_LANGUAGES_LANG_CODE			(ClItemIndex) 0
#define  M4LDB_ITEM_SYS_DICT_DYNAMIC_LANGUAGES_ID_LANGUAGE			(ClItemIndex) 1


// Indices globales de el canal de nombres traducidos.
// jcr Ahora se ordenan por tamano y nombre

const m4uint8_t M4LDB_SYS_TRANS_FIELDS					= 0;
const m4uint8_t M4LDB_SYS_TRANS_FIELDS_TRANS			= 1;
const m4uint8_t M4LDB_SYS_TRANS_LOGIC_OBJECTS			= 2;
const m4uint8_t M4LDB_SYS_TRANS_LOGIC_OBJECTS_TRANS		= 3;
const m4uint8_t M4LDB_SYS_TRANS_NODE_NUMBER				= 4;

#define  M4LDB_ITEM_SYS_TRANS_FIELDS_ID_FIELD					(ClItemIndex)0
#define  M4LDB_ITEM_SYS_TRANS_FIELDS_ID_OBJECT					(ClItemIndex)1
#define  M4LDB_ITEM_SYS_TRANS_FIELDS_FIRST_LANGUAGE				(ClItemIndex)2

#define  M4LDB_ITEM_SYS_TRANS_FIELDS_TRANS_ID_FIELD				(ClItemIndex)0
#define  M4LDB_ITEM_SYS_TRANS_FIELDS_TRANS_ID_OBJECT			(ClItemIndex)1
#define  M4LDB_ITEM_SYS_TRANS_FIELDS_TRANS_ID_LANGUAGE			(ClItemIndex)2
#define  M4LDB_ITEM_SYS_TRANS_FIELDS_TRANS_ID_TRANS_FLD			(ClItemIndex)3

#define  M4LDB_ITEM_SYS_TRANS_LOGIC_OBJECTS_ID_OBJECT			(ClItemIndex)0
#define  M4LDB_ITEM_SYS_TRANS_LOGIC_OBJECTS_ID_OBJECT_AUX		(ClItemIndex)1
#define  M4LDB_ITEM_SYS_TRANS_LOGIC_OBJECTS_FIRST_LANGUAGE		(ClItemIndex)2

#define  M4LDB_ITEM_SYS_TRANS_LOGIC_OBJECTS_TRANS_ID_OBJECT		(ClItemIndex)0
#define  M4LDB_ITEM_SYS_TRANS_LOGIC_OBJECTS_TRANS_ID_LANGUAGE	(ClItemIndex)1
#define  M4LDB_ITEM_SYS_TRANS_LOGIC_OBJECTS_TRANS_ID_TRANS_OBJ	(ClItemIndex)2


// Funciones.

template <class T>
m4pchar_t M4LDB_PSTR(T *X)
{
	m4pchar_t pTemp = (m4pchar_t) X; 

	if (pTemp)
	{
		// Quitamos los blancos del principio.
		while (*pTemp == ' ')
		{
			pTemp++;
		}
	}
	else
	{		
		#if defined(_M4SOLX86_) || defined(_M4SOLSPARC_)
			pTemp = "";
		#else
			pTemp = (m4pchar_t) M4LDB_EMPTY;
		#endif // (_M4SOLX86_) || defined(_M4SOLSPARC_)
	}

	return (pTemp);
}


//=================================================================================
// Convierte un tipo meta4 de unicode a no unicode y viceversa
//=================================================================================
template <class T>
m4uint8_t	M4LDB_M4_UNICODE_TYPE( T ai_iType )
{
	if( g_iEncodingMode == M4LDB_ENCODING_ANSI )
	{
		if( ai_iType == M4CL_M4_TYPE_UNI_FIXED_STRING || ai_iType == M4CL_M4_TYPE_UNI_VAR_STRING )
		{
			ai_iType = M4CL_M4_TYPE_FIXED_STRING;
		}
		else if( ai_iType == M4CL_M4_TYPE_UNI_VARIANT )
		{
			ai_iType = M4CL_M4_TYPE_VARIANT;
		}
		else if( ai_iType == M4CL_M4_TYPE_UNI_LONG )
		{
			ai_iType = M4CL_M4_TYPE_LONG;
		}
	}
	else
	{
		if( ai_iType == M4CL_M4_TYPE_FIXED_STRING || ai_iType == M4CL_M4_TYPE_VAR_STRING )
		{
			ai_iType = M4CL_M4_TYPE_UNI_VAR_STRING;
		}
		else if( ai_iType == M4CL_M4_TYPE_VARIANT )
		{
			ai_iType = M4CL_M4_TYPE_UNI_VARIANT;
		}
		else if( ai_iType == M4CL_M4_TYPE_LONG )
		{
			ai_iType = M4CL_M4_TYPE_UNI_LONG;
		}
	}

	return( ai_iType ) ;
}

template <class T>
m4int8_t M4LDB_M4_TYPE(T ai_iType)
{
	m4uint8_t iResult = ai_iType ;

	// Primero se controla que se esté en el rango permitido
	if( iResult < M4CL_M4_TYPE_FIXED_STRING || iResult > M4CL_M4_TYPE_MAX_TYPE )
	{
		iResult = M4CL_M4_TYPE_FIXED_STRING;
	}

	// Segundo se reducen los tipos a su tipo básico
	switch( iResult )
	{
		case M4CL_M4_TYPE_VAR_STRING:
		case M4CL_M4_TYPE_VARIANT:
			iResult = M4CL_M4_TYPE_FIXED_STRING;
		break;

		case M4CL_M4_TYPE_UNI_VARIANT:
			iResult = M4CL_M4_TYPE_UNI_VAR_STRING;
		break;

		case M4CL_M4_TYPE_CURRENCY:
		case M4CL_M4_TYPE_NUM_VARIANT:
		case M4CL_M4_TYPE_TIME_SPAN:
			iResult = M4CL_M4_TYPE_NUMBER;
		break;

		case M4CL_M4_TYPE_HOUR:
			iResult = M4CL_M4_TYPE_TIMESTAMP;
		break;
	}

	// Tercero se reducen los tipos unicode si es full ansi o full unicode
	if( g_iEncodingMode != M4LDB_ENCODING_MIXED )
	{
		iResult = M4LDB_M4_UNICODE_TYPE( iResult ) ;
	}

	return( iResult );
}

template <class T>
m4bool_t M4LDB_M4_IS_UNICODE(T ai_iType)
{
	if( ai_iType == M4CL_M4_TYPE_UNI_FIXED_STRING || ai_iType == M4CL_M4_TYPE_UNI_VAR_STRING || ai_iType == M4CL_M4_TYPE_UNI_LONG || ai_iType == M4CL_M4_TYPE_UNI_VARIANT )
	{
		return( M4_TRUE );
	}

	return( M4_FALSE );
}


inline m4date_t M4LDB_COMPARE_DATES (m4date_t ai_dDate1, m4date_t ai_dDate2)
{
	return ((fabs (ai_dDate1 - ai_dDate2) > M4_LN4_DATE_PRECISION) ? (ai_dDate1 - ai_dDate2) : 0.0);
}



template <class T, class U, class V>
m4double_t M4LDB_COMPARE_VALUES (T X, U Y, V Z)
{
	if (X && Y)
	{
		if (Z == M4CL_M4_TYPE_DATE || 
			Z == M4CL_M4_TYPE_TIMESTAMP || 
			Z == M4CL_M4_TYPE_NUMBER)
		{
			m4double_t xx = M4LDB_DOUBLE(X);
			m4double_t yy = M4LDB_DOUBLE(Y);
			return ((fabs (xx - yy) > M4_LN4_DATE_PRECISION) ? (xx - yy) : 0.0);
		}
		else
		{
			return (strcmp (X, Y));
		}
	}

	if (!X) 
	{
		if (!Y)
		{
			return (0.0);
		}
		return (-1.0);
	}
	return (1.0);
}


#define M4LDB_ID(X)			(m4pcchar_t) M4LDB_PSTR (X)

#define M4LDB_PCSTR(X)		(m4pcchar_t) M4LDB_PSTR (X)


extern m4pcchar_t g_aPersist[];
extern m4pcchar_t g_aM4Type[];


#define _M4LDB_DEBUG_BASE_NAME	"ldbinsp"
#define M4LDB_BLOB_PREFIX		"~ldb"
#define M4LDB_BLOB_DESCR_PREFIX		"~BLOBD"
#define M4LDB_BLOB_DESCR_LEN		6


// Security.
#define M4LDB_SEC_LOGIC_OBJECT	"SDC_VAL_SEC_LDB"
#define M4LDB_SEC_LOGIC_FIELD	"CONTROL"


// Niveles
#define	M4LDB_ORG_LEVEL_TECHNOLOGY			-999999


#endif	//__DEFMAIN_HPP__

