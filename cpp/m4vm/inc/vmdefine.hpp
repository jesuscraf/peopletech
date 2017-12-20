//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                vmdefine.hpp   
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
//    This module set the defines for all the VM
//
//
//==============================================================================

#ifndef __VMDEFINE_HPP__
#define __VMDEFINE_HPP__


#include "cldefine.hpp"
#include "chlog.hpp"
#include "clres.hpp"
#include "cltypes.hpp"
// Defines del DM
#include "iteminfo.hpp"

#include "vmmcrdef.hpp"
#include "math.h"


//general
//Solo comprueba y existe en DEBUG
#ifdef _DEBUG
	#define DEBUG_CHECK_RES(x,code, range, msg)	if (!(x)) { if (STARTCODE( code ,range )==M4_SUCCESS) { BeginLog(g_oChLog,code,range)<< msg<<EndLog;} StopIfWanted(); return M4_ERROR; }									
#else
    #define DEBUG_CHECK_RES(x,code, range, msg)	 
#endif

//esta muestra y compureba en debug y release
#ifdef _DEBUG
	#define M4CHECK_RES(x,code, range, msg)	if (!(x)){ if (STARTCODE( code ,range )==M4_SUCCESS) { BeginLog(g_oChLog,code,range)<< msg<<EndLog;} StopIfWanted();return M4_ERROR; }
#else
	#define M4CHECK_RES(x,code, range, msg)	if (!(x)){ if (STARTCODE( code ,range )==M4_SUCCESS) { BeginLog(g_oChLog,code,range)<< msg<<EndLog;} return M4_ERROR; }
#endif

//esta macro comprueba en ambas y muestra en debug
#ifdef _DEBUG
	#define M4CHECK_DEBUGINFO(x,code, range, msg)	if (!(x)){ if (STARTCODE( code ,range )==M4_SUCCESS) { BeginLog(g_oChLog,code,range)<< msg<<EndLog;} StopIfWanted();return M4_ERROR; }
#else
	#define M4CHECK_DEBUGINFO(x,code, range, msg)	if (!(x)){ return M4_ERROR; }
#endif


#define M4CL_RUNTIME_STACK_SIZE		1024
#define M4CL_RUNTIME_MAX_STACK_SIZE 262144 // (resize al doble 8 veces)

#define M4CL_STATE_STACK_SIZE		128
#define M4CL_STATE_STACK_MAX_SIZE	65536 // (resize al doble 9 veces)

#define M4CL_DYNAMIC_POOL_SIZE		32 //va en kb
#define M4CL_MAX_STRING				256 
#define M4CL_MAX_STRING_ERROR		1024 

#define CL_VM_DEFAULT_PRIOR_SLICE_STACK_SIZE 5


#define M4CL_MAX_REENTER			16

#define M4CL_VM_MAX_FILTER_ARGS		20

#define M4CL_IS_EOL(x)				((*x)=='\n')

#define M4CL_ORIGIN_INSTALLED		0
#define M4CL_ORIGIN_RT_GENERATED	1

#define M4CL_RTGEN_NUM_INSTRUCTIONS	150

#define M4_SHORT_DATE_FORMAT        1

//Numero inicial de tramos en las pilas redimensionables
#define	 M4CL_INITIAL_STACK_SLICES_NUM		32

//numero de atributos definidos
#define M4CL_NUM_ATT_RUNNABLE   8
#define M4CL_NUM_ATT_GET        9


//transacciones de VM
#define M4CL_NO_VM_TRANSACTION_DEFINED  -2


//Items para la persist




#define M4CL_VM_COMPILE_ITEM_LANGUAGE       "ID_CODE_TYPE"


#define M4CL_VM_ID_M4_TYPE_ITEM             "ID_M4_TYPE"


#define M4CL_VM_TRIGGER_ALL                       0
#define M4CL_VM_TRIGGER_INSERTED                  1
#define M4CL_VM_TRIGGER_UPDATEDDB                 2
#define M4CL_VM_TRIGGER_DELETED                   4
#define M4CL_VM_TRIGGER_CURRENT                   8


#define M4CL_CM_DEFAULT_PRIOR                       3


#define	M4CL_MAX_PIVOT_PK_ELEMENTS					20
#define	M4CL_MAX_CROSSTAB_COLUMNS					10
#define	M4CL_MAX_CROSSTAB_COLUMN_ID					255
#define	M4CL_MAX_LOOKUP_ELEMENTS					10


//////////// Para las TIs de Acumulado.

#define M4_CL_VM_PAYROLL_SEARCH_TYPE_IMPUTATION		0x1
#define M4_CL_VM_PAYROLL_SEARCH_TYPE_PAY			0x2

#define M4_CL_VM_PAYROLL_SELECT_PAY_MANY_PAYS			0x1
#define M4_CL_VM_PAYROLL_SELECT_PAY_BEFORE				0x2
#define M4_CL_VM_PAYROLL_SELECT_PAY_ACTUAL				0x3
#define M4_CL_VM_PAYROLL_SELECT_PAY_ACTUAL_FOR_PATRO_I	0x4
#define M4_CL_VM_PAYROLL_SELECT_PAY_ACTUAL_FOR_PATRO_II	0x5

#define M4_CL_VM_PAYROLL_REVISION_BEH_SUM			0x1
#define M4_CL_VM_PAYROLL_REVISION_BEH_FIRST			0x2
#define M4_CL_VM_PAYROLL_REVISION_BEH_LAST			0x3


#define M4_CL_VM_YTD_KEEP_SLICES					1
#define M4_CL_VM_YTD_KEEP_RECORDS					2
#define M4_CL_VM_YTD_ONE_RECORD						0

#define M4_CL_VM_YTD_KEEP_NUMBER					2 //numero de posibilidades de keep que tenemos, para poder hacer el control de argumento válido

//////////////////////////////////////////

// Maxima longitud que se puede leer de un fichero.
#define M4_MAX_READ_CHARS_FROM_FILE					10240

// para messagebox
#define M4CL_MAX_LONG_STRING_NUMBER		256 // Long.Max. de un Numero formateado a cadena
#define M4CL_MAX_LONG_STRING_DATE		50	// Long.Max. de una Fecha formateada a cadena
#define M4CL_MAX_LONG_STRING_NULL		8	// Long.Max. de la cadena representando a NULL
#define M4CL_STRING_NULL				" <NULL> "	// Cadena que representa a NULL

//argumento ficticio al compilar items items de ln4 con arg. variables
#define M4CL_ARG_NAME_VAR_ARGS			"SYS_NUMBER_OF_ARGUMENTS"

#define M4CL_VM_OP_DELETED				4		
#define M4CL_VM_OP_MODIFIED				7
#define M4CL_VM_OP_NEW					1
#define M4CL_VM_OP_UPDATED				2


// LN4 ========================================

// tipos de Trim en cadenas en LN4
#define M4CL_TRIM_LEFT					-1
#define M4CL_TRIM_ALL					0
#define M4CL_TRIM_RIGHT					1

//dateadd
#define M4CL_VM_DATEADD_DAY				1
#define M4CL_VM_DATEADD_MONTH			2
#define M4CL_VM_DATEADD_YEAR			3
#define M4CL_VM_DATEADD_HOUR			4
#define M4CL_VM_DATEADD_MINUTE			5
#define M4CL_VM_DATEADD_SECOND			6
#define M4CL_VM_DATEADD_WEEK			7
#define M4CL_VM_DATEADD_COMPLETE_WEEK	8

#define M4CL_VM_DATEADD_MAX_VALUE		8
#define M4CL_VM_DATEADD_MIN_VALUE		1


//EndTransaction
#define M4CL_VM_ET_ROLLBACK_RESUME		-1		// No entra dentro del mínimo porque se trata especialmente
#define M4CL_VM_ET_ROLLBACK				0
#define M4CL_VM_ET_COMMIT				1
#define M4CL_VM_ET_VALIDATE				2
#define M4CL_VM_ET_OPERATIONS			3

#define M4CL_VM_ET_MAX_VALUE			3
#define M4CL_VM_ET_MIN_VALUE			0

// Scope y Type en LN4
// 1 byte (8 bits) = Item Scope(4 bits) + Item Type (3 bits)
#define M4CL_ITEM_SCOPE_MASK_BITS			(m4uint8_t (M4DM_SCOPE_ALL))			// 0x07	// 0000 0111
#define M4CL_ITEM_TYPE_MASK_BITS			(m4uint8_t (M4DM_TYPE_ALL << 3)	)	// 0x78	// 0111 1000

																		//				bits--76543 210
#define M4CL_ITEM_SCOPE_REGISTER_FLAG		(M4DM_SCOPE_REGISTER)		// 0x04	//4 = bit 2	= 00000 100
#define M4CL_ITEM_SCOPE_BLOCK_FLAG			(M4DM_SCOPE_BLOCK)			// 0x02	//2 = bit 1 = 00000 010
#define M4CL_ITEM_SCOPE_NODE_FLAG			(M4DM_SCOPE_NODE)			// 0x01	//1 = bit 0 = 00000 001
#define M4CL_ITEM_SCOPE_ALL_FLAG			(M4DM_SCOPE_ALL)			// Todos = 00000 111

																		//				bits--76543 210
#define M4CL_ITEM_TYPE_METHOD_FLAG			(m4uint8_t (M4DM_TYPE_METHOD	<< 3))	// 0x08	//8 = bit 3 = 00001 000
#define M4CL_ITEM_TYPE_PROPERTY_FLAG		(m4uint8_t (M4DM_TYPE_PROPERTY	<< 3))	// 0x10	//16= bit 4 = 00010 000
#define M4CL_ITEM_TYPE_FIELD_FLAG			(m4uint8_t (M4DM_TYPE_FIELD		<< 3))	// 0x20	//32= bit 5 = 00100 000
#define M4CL_ITEM_TYPE_CONCEPT_FLAG			(m4uint8_t (M4DM_TYPE_CONCEPT	<< 3))	// 0x40	//64= bit 6 = 01000 000
#define M4CL_ITEM_TYPE_ALL_FLAG				(m4uint8_t (M4DM_TYPE_ALL		<< 3))	// Todos = 01111 000



const m4double_t M4CL_MATH_PI=	3.141592653589793;
const m4double_t M4CL_MATH_E =	exp((double)1);

//Lenguajes de Logon y de Desarrollo (canal Sesion)
#define M4CL_LANG_LOGON					(m4language_t(-1))
#define M4CL_LANG_DEVELOPMENT			(m4language_t(-2))


#define M4CL_VM_SLICES_DEBUG_FILE			"slices.log"

#define M4CL_VM_UUID_KEY_SIZE				25 //24+1 para el null terminator

#ifdef _DEBUG
	void StopIfWanted(void);
#endif


// Defines sobre Datos de la Definicion del Item
#define M4_CH_VM_ITEMDEF_TYPE							1
#define M4_CH_VM_ITEMDEF_SCOPE							2
#define M4_CH_VM_ITEMDEF_M4TYPE							3
#define M4_CH_VM_ITEMDEF_PRECISION						4
#define M4_CH_VM_ITEMDEF_INDEX							5
#define M4_CH_VM_ITEMDEF_SCALE							6
#define M4_CH_VM_ITEMDEF_INTERNAL_TYPE					7
#define M4_CH_VM_ITEMDEF_READ_OBJECT_ALIAS				8
#define M4_CH_VM_ITEMDEF_WRITE_OBJECT_ALIAS				9
#define M4_CH_VM_ITEMDEF_READ_FIELD_ID					10
#define M4_CH_VM_ITEMDEF_WRITE_FIELD_ID					11
#define M4_CH_VM_ITEMDEF_READ_OBJECT_ID					12
#define M4_CH_VM_ITEMDEF_WRITE_OBJECT_ID				13
#define M4_CH_VM_ITEMDEF_SLICE_TOTALIZE					14
#define M4_CH_VM_ITEMDEF_NAME							15
#define M4_CH_VM_ITEMDEF_ORDER							16
#define M4_CH_VM_ITEMDEF_SYNONYM						17
#define M4_CH_VM_ITEMDEF_ID								18
#define M4_CH_VM_ITEMDEF_ITEM_NUMBER					19
#define M4_CH_VM_ITEMDEF_IS_PK							20
#define M4_CH_VM_ITEMDEF_NOT_NULL						21
#define M4_CH_VM_ITEMDEF_AUXILIAR_ITEM					22
#define M4_CH_VM_ITEMDEF_DMD							23
#define M4_CH_VM_ITEMDEF_DMD_COMPONENT					24
#define M4_CH_VM_ITEMDEF_DMD_CROSSTAB					25
#define M4_CH_VM_ITEMDEF_DMD_FIELD						26
#define M4_CH_VM_ITEMDEF_THEORIC_SCOPE					27
#define M4_CH_VM_ITEMDEF_RULE_NUMBER					28
#define M4_CH_VM_ITEMDEF_RULE_ID						29
#define M4_CH_VM_ITEMDEF_RULE_START_DATE				30
#define M4_CH_VM_ITEMDEF_RULE_END_DATE					31
#define M4_CH_VM_ITEMDEF_RULE_AUTOGENERATED				32
#define M4_CH_VM_ITEMDEF_RULE_CODE						33


//Defines sobre Datos de la Definición del Nodo
#define M4_CH_VM_NODEDEF_READ_OBJECT_ID					1
#define M4_CH_VM_NODEDEF_WRITE_OBJECT_ID				2
#define M4_CH_VM_NODEDEF_ID								3
#define M4_CH_VM_NODEDEF_INDEX							4
#define M4_CH_VM_NODEDEF_IS_ROOT						5
#define M4_CH_VM_NODEDEF_IS_REAL_ROOT					6
#define M4_CH_VM_NODEDEF_PARENT							7
#define M4_CH_VM_NODEDEF_SENTENCE						8
#define M4_CH_VM_NODEDEF_NAME							9
#define M4_CH_VM_NODEDEF_MAX_NUM_ROWS_DB				10
#define M4_CH_VM_NODEDEF_TYPE							11
#define M4_CH_VM_NODEDEF_AUTO_FILTER					12


//Defines sobre Datos de la Definición del Canal
#define M4_CH_VM_CHANNELDEF_NODE_NUMBER					1
#define M4_CH_VM_CHANNELDEF_CREATION_TYPE				2
#define M4_CH_VM_CHANNELDEF_NAME						3
#define M4_CH_VM_CHANNELDEF_ENVIRONMENT_VARS			4
#define M4_CH_VM_CHANNELDEF_ID							5
#define M4_CH_VM_CHANNELDEF_L2_INSTANCE_NUMBER			6
#define M4_CH_VM_CHANNELDEF_L2_INSTANCE_HANDLE			7



#define M4_ITEM_SCHTIS_EXT_DEP_TYPE						"DEPENDENCE_TYPE"
#define M4_ITEM_SCHTIS_CHANN_DEP_TYPE					"DEPENDENCE_TYPE"
#define M4_ITEM_SCHTIS_INT_DEP_TYPE						"DEPENDENCE_TYPE"



#endif

