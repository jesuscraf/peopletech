//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:                trad.cpp
// Project:        calculo
// Author:         Meta Software M.S. , S.A
// Date:            20 may 97
// Language:        C++ 
// Operating System: WINDOWS
// Design Document:  EXTCONCI.DOC
//
//
// Definition:
// 
//    Implementación de la clase traductor
////
//==============================================================================

#include "m4stl.hpp"

#include "trad.h"

#include "finder.hpp"
#include "tablfunc.h"
#include "tablatt.hpp"
#include "flexlexer.h"
#include "parser.h"
#include "vmres.hpp"


#include "cldates.hpp"
#include "access.hpp"
#include "register.hpp"
#include "channel.hpp"
#include "chlog.hpp"
#include "m4win2ux.hpp"
#include "m4unicode.hpp"




/* Bug 0090300
Función que quita los # iniciales
*/
m4pcchar_t	M4SkipHash( m4pcchar_t ai_pccString )
{
	if( ai_pccString == NULL )
	{
		return( NULL ) ;
	}

	while( *ai_pccString == '#' )
	{
		ai_pccString++ ;
	}

	return( ai_pccString ) ;
}

//////////////////////

//***Constructor***


ClTrad::ClTrad ()

: m_oSynonymStack( 16 )

{

	m4VariantType vTemp;

	m_oFuncTable=ClFuncTable::GetInstance();
	m_oAtributeMethodTable = ClAtributeFuncTable::GetInstance();
	
	m_iLastArgRefStrSize=0;
	m_iNumVar=0;
	m_iNumberOfArguments=0;
	m_iNumOfRefArguments=0;
	m_iNumEtiqueta=1;
	Parser=NULL;
	m_iNumLine=0;
	m_iNumLineParser=1;
	m_iPolishPos=1;
	
	m_ppszSymbols=0;
	m_iSymbolsTableSize=0;

	// Reservamos espacio para la polaca
	m_iTamPolish=M4_TRAD_POLISH_MAX_SIZE;
	m_pszPolish=new char[m_iTamPolish+1];
	if (m_pszPolish)
		*m_pszPolish = '\0';

    bCompileError = M4_FALSE;
	
	strcpy (m_pszPushRefArg,"");
	strcpy (m_pszPushArgRefAtReturn,"");
	
	//ponemos todas las macros
	AddMacro("TRUE",M4_TRUE);
	AddMacro("FALSE",M4_FALSE);
	AddMacro("NEXT",1);
	AddMacro("BACK",-1);
	AddMacro("BEGIN",-2); 
	AddMacro("END",2);
    AddMacro("SHORT_DATE_FORMAT" , 1);
    AddMacro("LONG_DATE_FORMAT" , 0);
    AddMacro("M4_SUCCESS" , 0);
    AddMacro("M4_ERROR" , -1);
    AddMacro("ZERO" , 0);
    AddMacro("POSITIVE" , 1);
    AddMacro("NEGATIVE" , -1);
		//new notation
	AddMacro("M4_TRUE",M4_TRUE);
	AddMacro("M4_FALSE",M4_FALSE);
	AddMacro("M4_NEXT",1);
	AddMacro("M4_BACK",-1);
	AddMacro("M4_BEGIN",-2); 
	AddMacro("M4_END",2);
    AddMacro("M4_SHORT_DATE_FORMAT" , 1);
    AddMacro("M4_LONG_DATE_FORMAT" , 0);
    AddMacro("M4_ZERO" , 0);
    AddMacro("M4_POSITIVE" , 1);
    AddMacro("M4_NEGATIVE" , -1);
	

    //macros para búsquedas
    AddMacro("EQUAL" ,							M4CL_EQUAL_FUNCTION);
    AddMacro("DISTINCT" ,        				M4CL_DISTINCT_FUNCTION);
    AddMacro("GREATER" ,         				M4CL_BIGGER_FUNCTION);
    AddMacro("SMALLER" ,         				M4CL_SMALLER_FUNCTION);
    AddMacro("GREATER_OR_EQUAL" ,				M4CL_BIGGER_OR_EQUAL_FUNCTION);
    AddMacro("SMALLER_OR_EQUAL" ,				M4CL_SMALLER_OR_EQUAL_FUNCTION);
    AddMacro("EQUAL_OR_NULL" ,           		M4CL_EQUAL_FUNCTION_OR_NULL);
    AddMacro("DISTINCT_OR_NULL" ,        		M4CL_DISTINCT_FUNCTION_OR_NULL);
    AddMacro("GREATER_OR_NULL" ,         		M4CL_BIGGER_FUNCTION_OR_NULL);
    AddMacro("SMALLER_OR_NULL" ,         		M4CL_SMALLER_FUNCTION_OR_NULL);
    AddMacro("GREATER_OR_EQUAL_OR_NULL" ,		M4CL_BIGGER_OR_EQUAL_FUNCTION_OR_NULL);
    AddMacro("SMALLER_OR_EQUAL_OR_NULL" ,		M4CL_SMALLER_OR_EQUAL_FUNCTION_OR_NULL);
    AddMacro("CASE_EQUAL" ,						M4CL_CASE_EQUAL_FUNCTION);
    AddMacro("CASE_DISTINCT" ,					M4CL_CASE_DISTINCT_FUNCTION);
    AddMacro("CASE_GREATER" ,         			M4CL_CASE_BIGGER_FUNCTION);
    AddMacro("CASE_SMALLER" ,         			M4CL_CASE_SMALLER_FUNCTION);
    AddMacro("CASE_GREATER_OR_EQUAL" ,			M4CL_CASE_BIGGER_OR_EQUAL_FUNCTION);
    AddMacro("CASE_SMALLER_OR_EQUAL" ,			M4CL_CASE_SMALLER_OR_EQUAL_FUNCTION);
    AddMacro("CASE_EQUAL_OR_NULL" ,				M4CL_CASE_EQUAL_FUNCTION_OR_NULL);
    AddMacro("CASE_DISTINCT_OR_NULL" ,			M4CL_CASE_DISTINCT_FUNCTION_OR_NULL);
    AddMacro("CASE_GREATER_OR_NULL" ,			M4CL_CASE_BIGGER_FUNCTION_OR_NULL);
    AddMacro("CASE_SMALLER_OR_NULL" ,			M4CL_CASE_SMALLER_FUNCTION_OR_NULL);
    AddMacro("CASE_GREATER_OR_EQUAL_OR_NULL" ,	M4CL_CASE_BIGGER_OR_EQUAL_FUNCTION_OR_NULL);
    AddMacro("CASE_SMALLER_OR_EQUAL_OR_NULL" ,	M4CL_CASE_SMALLER_OR_EQUAL_FUNCTION_OR_NULL);
    AddMacro("REGULAR_EXPRESSION"			,	M4CL_MATCH_REGEX);
    AddMacro("REGULAR_EXPRESSION_OR_NULL"	,	M4CL_MATCH_REGEX_OR_NULL);
    AddMacro("CASE_REGULAR_EXPRESSION"		,	M4CL_CASE_MATCH_REGEX);
    AddMacro("CASE_REGULAR_EXPRESSION_OR_NULL" ,	M4CL_CASE_MATCH_REGEX_OR_NULL);
		//new notation
    AddMacro("M4_EQUAL" ,							M4CL_EQUAL_FUNCTION);
    AddMacro("M4_DISTINCT" ,        				M4CL_DISTINCT_FUNCTION);
    AddMacro("M4_GREATER" ,         				M4CL_BIGGER_FUNCTION);
    AddMacro("M4_SMALLER" ,         				M4CL_SMALLER_FUNCTION);
    AddMacro("M4_GREATER_OR_EQUAL" ,				M4CL_BIGGER_OR_EQUAL_FUNCTION);
    AddMacro("M4_SMALLER_OR_EQUAL" ,				M4CL_SMALLER_OR_EQUAL_FUNCTION);
	
    AddMacro("M4_EQUAL_OR_NULL" ,           		M4CL_EQUAL_FUNCTION_OR_NULL);
    AddMacro("M4_DISTINCT_OR_NULL" ,        		M4CL_DISTINCT_FUNCTION_OR_NULL);
    AddMacro("M4_GREATER_OR_NULL" ,         		M4CL_BIGGER_FUNCTION_OR_NULL);
    AddMacro("M4_SMALLER_OR_NULL" ,         		M4CL_SMALLER_FUNCTION_OR_NULL);
    AddMacro("M4_GREATER_OR_EQUAL_OR_NULL" ,		M4CL_BIGGER_OR_EQUAL_FUNCTION_OR_NULL);
    AddMacro("M4_SMALLER_OR_EQUAL_OR_NULL" ,		M4CL_SMALLER_OR_EQUAL_FUNCTION_OR_NULL);
	
    AddMacro("M4_CASE_EQUAL" ,						M4CL_CASE_EQUAL_FUNCTION);
    AddMacro("M4_CASE_DISTINCT" ,					M4CL_CASE_DISTINCT_FUNCTION);
    AddMacro("M4_CASE_GREATER" ,         			M4CL_CASE_BIGGER_FUNCTION);
    AddMacro("M4_CASE_SMALLER" ,         			M4CL_CASE_SMALLER_FUNCTION);
    AddMacro("M4_CASE_GREATER_OR_EQUAL" ,			M4CL_CASE_BIGGER_OR_EQUAL_FUNCTION);
    AddMacro("M4_CASE_SMALLER_OR_EQUAL" ,			M4CL_CASE_SMALLER_OR_EQUAL_FUNCTION);
	
    AddMacro("M4_CASE_EQUAL_OR_NULL" ,				M4CL_CASE_EQUAL_FUNCTION_OR_NULL);
    AddMacro("M4_CASE_DISTINCT_OR_NULL" ,			M4CL_CASE_DISTINCT_FUNCTION_OR_NULL);
    AddMacro("M4_CASE_GREATER_OR_NULL" ,			M4CL_CASE_BIGGER_FUNCTION_OR_NULL);
    AddMacro("M4_CASE_SMALLER_OR_NULL" ,			M4CL_CASE_SMALLER_FUNCTION_OR_NULL);
    AddMacro("M4_CASE_GREATER_OR_EQUAL_OR_NULL" ,	M4CL_CASE_BIGGER_OR_EQUAL_FUNCTION_OR_NULL);
    AddMacro("M4_CASE_SMALLER_OR_EQUAL_OR_NULL" ,	M4CL_CASE_SMALLER_OR_EQUAL_FUNCTION_OR_NULL);

    AddMacro("M4_REGULAR_EXPRESSION"			,	M4CL_MATCH_REGEX);
    AddMacro("M4_REGULAR_EXPRESSION_OR_NULL"	,	M4CL_MATCH_REGEX_OR_NULL);
    AddMacro("M4_CASE_REGULAR_EXPRESSION"		,	M4CL_CASE_MATCH_REGEX);
    AddMacro("M4_CASE_REGULAR_EXPRESSION_OR_NULL" ,	M4CL_CASE_MATCH_REGEX_OR_NULL);

    //macros para los dialog box
    AddMacro("BTN_OK" ,						MB_OK); 
    AddMacro("BTN_OK_CANCEL" ,				MB_OKCANCEL);
    AddMacro("BTN_ABORT_RETRY_IGNORE" ,		MB_ABORTRETRYIGNORE);
    AddMacro("BTN_YES_NO_CANCEL" ,			MB_YESNOCANCEL);
    AddMacro("BTN_YES_NO" ,					MB_YESNO);
    AddMacro("BTN_RETRY_CANCEL" ,			MB_RETRYCANCEL);
	AddMacro("TOTAL_COUNT",					1);
	AddMacro("TOTAL_SUM",					2);
	AddMacro("TOTAL_AVG",					3);
	AddMacro("TOTAL_MAX",					4);
	AddMacro("TOTAL_MIN",					5);
	AddMacro("TOTAL_FIRST",					6);
	AddMacro("TOTAL_LAST",					7);
	
    AddMacro("M4_BTN_OK" ,					MB_OK); 
    AddMacro("M4_BTN_OK_CANCEL" ,			MB_OKCANCEL);
    AddMacro("M4_BTN_ABORT_RETRY_IGNORE" ,	MB_ABORTRETRYIGNORE);
    AddMacro("M4_BTN_YES_NO_CANCEL" ,		MB_YESNOCANCEL);
    AddMacro("M4_BTN_YES_NO" ,				MB_YESNO);
    AddMacro("M4_BTN_RETRY_CANCEL" ,		MB_RETRYCANCEL);
	AddMacro("M4_TOTAL_COUNT",				1);
	AddMacro("M4_TOTAL_SUM",				2);
	AddMacro("M4_TOTAL_AVG",				3);
	AddMacro("M4_TOTAL_MAX",				4);
	AddMacro("M4_TOTAL_MIN",				5);
	AddMacro("M4_TOTAL_FIRST",				6);
	AddMacro("M4_TOTAL_LAST",				7);
	
	
    AddMacro("OK" ,			IDOK);
    AddMacro("CANCEL" ,		IDCANCEL);
    AddMacro("ABORT" ,		IDABORT);
    AddMacro("RETRY" ,		IDRETRY);
    AddMacro("IGNORE" ,		IDIGNORE);
    AddMacro("YES" ,		IDYES);
    AddMacro("NO" ,			IDNO);

    AddMacro("M4_OK" ,      IDOK);
    AddMacro("M4_CANCEL" ,  IDCANCEL);
    AddMacro("M4_ABORT" ,   IDABORT);
    AddMacro("M4_RETRY" ,   IDRETRY);
    AddMacro("M4_IGNORE" ,  IDIGNORE);
    AddMacro("M4_YES" ,     IDYES);
    AddMacro("M4_NO" ,      IDNO);

	//macros para Trim sobre cadenas
	AddMacro("TRIM_LEFT"		, M4CL_TRIM_LEFT);
	AddMacro("TRIM_ALL"			, M4CL_TRIM_ALL);
	AddMacro("TRIM_RIGHT"		, M4CL_TRIM_RIGHT);
		//New not.
	AddMacro("M4_TRIM_LEFT"		, M4CL_TRIM_LEFT);
	AddMacro("M4_TRIM_ALL"		, M4CL_TRIM_ALL);
	AddMacro("M4_TRIM_RIGHT"	, M4CL_TRIM_RIGHT);
	
	//macros para ConvertCase sobre cadenas
	AddMacro("UNCHANGED"	, M4CL_UNCHANGED);
	AddMacro("UPPERCASE"	, M4CL_UPPERCASE);
	AddMacro("LOWERCASE"	, M4CL_LOWERCASE);
		//new notation
	AddMacro("M4_UNCHANGED"	, M4CL_UNCHANGED);
	AddMacro("M4_UPPERCASE"	, M4CL_UPPERCASE);
	AddMacro("M4_LOWERCASE"	, M4CL_LOWERCASE);


	//macros para Scope y Type en Funciones xxxNodeItem()
	AddMacro("SCOPE_ALL"			, M4CL_ITEM_SCOPE_ALL_FLAG);
	AddMacro("SCOPE_REGISTER"		, M4CL_ITEM_SCOPE_REGISTER_FLAG);
	AddMacro("SCOPE_BLOCK"			, M4CL_ITEM_SCOPE_BLOCK_FLAG);
	AddMacro("SCOPE_NODE"			, M4CL_ITEM_SCOPE_NODE_FLAG);	
	AddMacro("TYPE_ALL"				, M4CL_ITEM_TYPE_ALL_FLAG);
	AddMacro("TYPE_METHOD"			, M4CL_ITEM_TYPE_METHOD_FLAG);
	AddMacro("TYPE_PROPERTY"		, M4CL_ITEM_TYPE_PROPERTY_FLAG);
	AddMacro("TYPE_FIELD"			, M4CL_ITEM_TYPE_FIELD_FLAG);
	AddMacro("TYPE_CONCEPT"			, M4CL_ITEM_TYPE_CONCEPT_FLAG);
	
		//new notation
	AddMacro("M4_SCOPE_ALL"			, M4CL_ITEM_SCOPE_ALL_FLAG);
	AddMacro("M4_SCOPE_REGISTER"	, M4CL_ITEM_SCOPE_REGISTER_FLAG);
	AddMacro("M4_SCOPE_BLOCK"		, M4CL_ITEM_SCOPE_BLOCK_FLAG);
	AddMacro("M4_SCOPE_NODE"		, M4CL_ITEM_SCOPE_NODE_FLAG);	
	AddMacro("M4_TYPE_ALL"			, M4CL_ITEM_TYPE_ALL_FLAG);
	AddMacro("M4_TYPE_METHOD"		, M4CL_ITEM_TYPE_METHOD_FLAG);
	AddMacro("M4_TYPE_PROPERTY"		, M4CL_ITEM_TYPE_PROPERTY_FLAG);
	AddMacro("M4_TYPE_FIELD"		, M4CL_ITEM_TYPE_FIELD_FLAG);
	AddMacro("M4_TYPE_CONCEPT"		, M4CL_ITEM_TYPE_CONCEPT_FLAG);

	//autoload
	AddMacro("AUTOLOAD_OFF"			, M4CL_AUTOLOAD_OFF);
	AddMacro("AUTOLOAD_BLOCK"		, M4CL_AUTOLOAD_BLOCK);
	AddMacro("AUTOLOAD_PRG"			, M4CL_AUTOLOAD_PRG);
	AddMacro("AUTOLOAD_NODESAYS"	, M4CL_AUTOLOAD_NODESAYS);
		//new notation
	AddMacro("M4_AUTOLOAD_OFF"		, M4CL_AUTOLOAD_OFF);
	AddMacro("M4_AUTOLOAD_BLOCK"	, M4CL_AUTOLOAD_BLOCK);
	AddMacro("M4_AUTOLOAD_PRG"		, M4CL_AUTOLOAD_PRG);
	AddMacro("M4_AUTOLOAD_NODESAYS"	, M4CL_AUTOLOAD_NODESAYS);

	//para los filtros por operacion
	AddMacro("OP_DELETED"		, M4CL_VM_OP_DELETED);
	AddMacro("OP_MODIFIED"		, M4CL_VM_OP_MODIFIED);
	AddMacro("OP_INSERTED"		, M4CL_VM_OP_NEW);
	AddMacro("OP_UPDATED"		, M4CL_VM_OP_UPDATED);
		//new notation
	AddMacro("M4_OP_DELETED"	, M4CL_VM_OP_DELETED);
	AddMacro("M4_OP_MODIFIED"	, M4CL_VM_OP_MODIFIED);
	AddMacro("M4_OP_INSERTED"	, M4CL_VM_OP_NEW);
	AddMacro("M4_OP_UPDATED"	, M4CL_VM_OP_UPDATED);

	
    //para las transacciones
    AddMacro("ROLLBACK",					M4CL_VM_ET_ROLLBACK);
    AddMacro("COMMIT",						M4CL_VM_ET_COMMIT);
	AddMacro("EXECUTE_POSTVALIDATION",		M4CL_VM_ET_VALIDATE);
		//new notation
	AddMacro("M4_ROLLBACK_RESUME",			M4CL_VM_ET_ROLLBACK_RESUME);
    AddMacro("M4_ROLLBACK",					M4CL_VM_ET_ROLLBACK);
    AddMacro("M4_COMMIT",					M4CL_VM_ET_COMMIT);
	AddMacro("M4_EXECUTE_POSTVALIDATION",	M4CL_VM_ET_VALIDATE);
	AddMacro("M4_EXECUTE_OPERATIONS",		M4CL_VM_ET_OPERATIONS);
	
    //para CHR
    AddMacro("NEW_LINE"	        ,13);
    AddMacro("TAB"	            ,9);
    AddMacro("DOUBLE_QUOTE"	    ,34);
    AddMacro("CR"	            ,10);
		//new notation
    AddMacro("M4_NEW_LINE"	    ,13);
    AddMacro("M4_TAB"	        ,9);
    AddMacro("M4_DOUBLE_QUOTE"	,34);
    AddMacro("M4_CR"	        ,10);
	
	//atributos de lectura
	AddMacro("ATT_SYS_SLICE_NUMBER"			, M4CL_ATT_SYS_SLICE_NUMBER);
	AddMacro("ATT_SYS_START_DATE"			, M4CL_ATT_SYS_START_DATE);
	AddMacro("ATT_SYS_END_DATE"				, M4CL_ATT_SYS_END_DATE);
	AddMacro("ATT_SYS_FIRST_SLICE"			, M4CL_ATT_SYS_FIRST_SLICE);
	AddMacro("ATT_SYS_LAST_SLICE"			, M4CL_ATT_SYS_LAST_SLICE);
	AddMacro("ATT_SYS_OLD_VALUE"			, M4CL_ATT_SYS_OLD_VALUE);
	AddMacro("ATT_SYS_BLOB_DESCRIPTION"		, M4CL_ATT_SYS_BLOB_DESCRIP);
	AddMacro("ATT_SYS_BLOB_MASK"			, M4CL_ATT_SYS_BLOB_MASK);
	AddMacro("ATT_SYS_AUX_ITEM_ID"			, M4CL_ATT_SYS_AUX_ITEM_ID);
		//new notation
	AddMacro("M4_ATT_SYS_SLICE_NUMBER"		, M4CL_ATT_SYS_SLICE_NUMBER);
	AddMacro("M4_ATT_SYS_START_DATE"		, M4CL_ATT_SYS_START_DATE);
	AddMacro("M4_ATT_SYS_END_DATE"			, M4CL_ATT_SYS_END_DATE);
	AddMacro("M4_ATT_SYS_FIRST_SLICE"		, M4CL_ATT_SYS_FIRST_SLICE);
	AddMacro("M4_ATT_SYS_LAST_SLICE"		, M4CL_ATT_SYS_LAST_SLICE);
	AddMacro("M4_ATT_SYS_OLD_VALUE"			, M4CL_ATT_SYS_OLD_VALUE);
	AddMacro("M4_ATT_SYS_BLOB_DESCRIPTION"	, M4CL_ATT_SYS_BLOB_DESCRIP);
	AddMacro("M4_ATT_SYS_BLOB_MASK"			, M4CL_ATT_SYS_BLOB_MASK);
	AddMacro("M4_ATT_SYS_AUX_ITEM_ID"		, M4CL_ATT_SYS_AUX_ITEM_ID);


	//atributos ejecutables
	AddMacro("ATT_SYS_ADD_NEW_SLICE"			, M4CL_ATT_MTHD_SYS_ADD_NEW_SLICE);
	AddMacro("ATT_SYS_CREATE_SLICE"				, M4CL_ATT_MTHD_SYS_CREATE_SLICE);
	AddMacro("ATT_SYS_SPLIT_SLICE"				, M4CL_ATT_MTHD_SYS_SPLIT_SLICE);
	AddMacro("ATT_SYS_TOTALIZE_ITEMS"			, M4CL_ATT_MTHD_SYS_TOTALIZE_ITEMS);
	AddMacro("ATT_SYS_TOTALIZE_SLICES"			, M4CL_ATT_MTHD_SYS_TOTALIZE_SLICES);
	AddMacro("ATT_SYS_CREATE_BLOB_FILE"			, M4CL_ATT_MTHD_SYS_CREATE_BLOB_FILE);
	AddMacro("ATT_SYS_SET_BLOB_DESCRIPTION"		, M4CL_ATT_MTHD_SYS_SET_BLOB_DESCRIP);
		//new notation
	AddMacro("M4_ATT_SYS_ADD_NEW_SLICE"			, M4CL_ATT_MTHD_SYS_ADD_NEW_SLICE);
	AddMacro("M4_ATT_SYS_CREATE_SLICE"			, M4CL_ATT_MTHD_SYS_CREATE_SLICE);
	AddMacro("M4_ATT_SYS_SPLIT_SLICE"			, M4CL_ATT_MTHD_SYS_SPLIT_SLICE);
	AddMacro("M4_ATT_SYS_TOTALIZE_ITEMS"		, M4CL_ATT_MTHD_SYS_TOTALIZE_ITEMS);
	AddMacro("M4_ATT_SYS_TOTALIZE_SLICES"		, M4CL_ATT_MTHD_SYS_TOTALIZE_SLICES);
	AddMacro("M4_ATT_SYS_CREATE_BLOB_FILE"		, M4CL_ATT_MTHD_SYS_CREATE_BLOB_FILE);
	AddMacro("M4_ATT_SYS_SET_BLOB_DESCRIPTION"	, M4CL_ATT_MTHD_SYS_SET_BLOB_DESCRIP);

	//errores
	AddMacro("ERRORLOG"			, ERRORLOG);
	AddMacro("WARNINGLOG"		, WARNINGLOG);
	AddMacro("DEBUGINFOLOG"		, DEBUGINFOLOG);
		//new notation
	AddMacro("M4_ERRORLOG"		, ERRORLOG);
	AddMacro("M4_WARNINGLOG"	, WARNINGLOG);
	AddMacro("M4_DEBUGINFOLOG"	, DEBUGINFOLOG);


	//ordenaciones
	AddMacro("ASCENDING"		, M4CL_INDEX_WAY_ASCENDING);
	AddMacro("DESCENDING"		, M4CL_INDEX_WAY_DESCENDING);
		//new notation
	AddMacro("M4_ASCENDING"		, M4CL_INDEX_WAY_ASCENDING);
	AddMacro("M4_DESCENDING"	, M4CL_INDEX_WAY_DESCENDING);


	//Aavanced Serialization
	AddMacro("SEND_NO_BRANCH"		, 0);
	AddMacro("SEND_BRANCH"			, 1);
	AddMacro("SEND_RESET"			, 2);
		//New notation
	AddMacro("M4_SEND_NO_BRANCH"	, 0);
	AddMacro("M4_SEND_BRANCH"		, 1);
	AddMacro("M4_SEND_RESET"		, 2);


	//VariantTYPES
	AddMacro("VARIANT_TYPE_NULL"		, M4CL_CPP_TYPE_NULL);
	AddMacro("VARIANT_TYPE_NUMBER"		, M4CL_CPP_TYPE_NUMBER);
	AddMacro("VARIANT_TYPE_STRING"		, M4CL_CPP_TYPE_STRING);
	AddMacro("VARIANT_TYPE_DATE"		, M4CL_CPP_TYPE_DATE);
		//new notation
	AddMacro("M4_VARIANT_TYPE_NULL"		, M4CL_CPP_TYPE_NULL);
	AddMacro("M4_VARIANT_TYPE_NUMBER"	, M4CL_CPP_TYPE_NUMBER);
	AddMacro("M4_VARIANT_TYPE_STRING"	, M4CL_CPP_TYPE_STRING);
	AddMacro("M4_VARIANT_TYPE_DATE"		, M4CL_CPP_TYPE_DATE);


	// Modo de compartición de instancias
	AddMacro("INSTANCE_NOT_SHARED"  		, M4CL_T3ALIAS_OPEN_SCOPE_NOT_SHARED);
	AddMacro("INSTANCE_LOCAL_SHARED"		, M4CL_T3ALIAS_OPEN_SCOPE_LOCAL_SHARED);
	AddMacro("INSTANCE_GLOBAL_SHARED"		, M4CL_T3ALIAS_OPEN_SCOPE_GLOBAL_SHARED);
		//new notation
	AddMacro("M4_INSTANCE_NOT_SHARED"  		, M4CL_T3ALIAS_OPEN_SCOPE_NOT_SHARED);
	AddMacro("M4_INSTANCE_LOCAL_SHARED"		, M4CL_T3ALIAS_OPEN_SCOPE_LOCAL_SHARED);
	AddMacro("M4_INSTANCE_GLOBAL_SHARED"	, M4CL_T3ALIAS_OPEN_SCOPE_GLOBAL_SHARED);


	//funciones DateAdd y DateDiff
	AddMacro("M4_YEAR"			, M4CL_VM_DATEADD_YEAR);
	AddMacro("M4_MONTH"			, M4CL_VM_DATEADD_MONTH);
	AddMacro("M4_DAY"			, M4CL_VM_DATEADD_DAY);
	AddMacro("M4_WEEK"			, M4CL_VM_DATEADD_WEEK);
	//AddMacro("M4_WEEKDAY"		, M4CL_VM_DATEADD_WEEKDAY);
	AddMacro("M4_HOUR"			, M4CL_VM_DATEADD_HOUR);
	AddMacro("M4_MINUTE"		, M4CL_VM_DATEADD_MINUTE);
	AddMacro("M4_SECOND"		, M4CL_VM_DATEADD_SECOND);
	AddMacro("M4_COMPLETE_WEEK"	, M4CL_VM_DATEADD_COMPLETE_WEEK);


	//para mantener tramos o registros en el YTD
	AddMacro("M4_KEEP_SLICES"		, M4_CL_VM_YTD_KEEP_SLICES);
	AddMacro("M4_KEEP_RECORDS"		, M4_CL_VM_YTD_KEEP_RECORDS);
	AddMacro("M4_KEEP_ONE_RECORD"	, M4_CL_VM_YTD_ONE_RECORD);

	//tipos de aperturas de canal por L2
	AddMacro("M4_OPEN_MODE_NO_LOAD"				, M4CL_T3ALIAS_OPEN_MODE_NO_LOAD);
	AddMacro("M4_OPEN_MODE_LOAD"				, M4CL_T3ALIAS_OPEN_MODE_LOAD);
	AddMacro("M4_OPEN_MODE_AUTOLOAD_BLOCK"		, M4CL_T3ALIAS_OPEN_MODE_AUTOLOAD_BLOCK);
	AddMacro("M4_OPEN_MODE_AUTOLOAD_PRG"		, M4CL_T3ALIAS_OPEN_MODE_AUTOLOAD_PRG);
	AddMacro("M4_OPEN_MODE_AUTOLOAD_NODESAYS"	, M4CL_T3ALIAS_OPEN_MODE_AUTOLOAD_NODESAYS);

	//tipos de busqueda de L2 segun sociedad
	AddMacro("M4_ORGANIZATION_L2_TYPE_NONE"		, M4CL_ORGANIZATION_L2_TYPE_NONE);
	AddMacro("M4_ORGANIZATION_L2_TYPE_MONO"		, M4CL_ORGANIZATION_L2_TYPE_MONO);
	AddMacro("M4_ORGANIZATION_L2_TYPE_MULTI"	, M4CL_ORGANIZATION_L2_TYPE_MULTI);
	AddMacro("M4_ORGANIZATION_L2_TYPE_FATHER"	, M4CL_ORGANIZATION_L2_TYPE_FATHER);
	AddMacro("M4_ORGANIZATION_L2_TYPE_ANY"		, M4CL_ORGANIZATION_L2_TYPE_ANY);


	//tipos de access para L2
	AddMacro("M4_CSTYPE_FRONT"		,M4CL_CSTYPE_L2_FRONT);
	AddMacro("M4_CSTYPE_BACK"		,M4CL_CSTYPE_L2_BACK);
	AddMacro("M4_CSTYPE_BOTH"		,M4CL_CSTYPE_L2_SITE);
	AddMacro("M4_CSTYPE_REAL_BOTH"	,M4CL_CSTYPE_L2_BOTH);
	AddMacro("M4_CSTYPE_FATHER"     ,M4CL_CSTYPE_L2_FATHER);
	AddMacro("M4_CSTYPE_MAPPED"     ,M4CL_CSTYPE_MAPPED);	// Deprecada. Mantenida por compatibilidad

	//CS timeout
	AddMacro("M4_DEFAULT_CS_TIMEOUT",0);
	AddMacro("M4_NEXT_CS_TIMEOUT",1);
	AddMacro("M4_GENERIC_CS_TIMEOUT",2);

	AddMacro("M4_RESET_NEXT_TIMEOUT",0);
	AddMacro("M4_ENABLE_TIMEOUT_WINDOW",1);

	//matemáticas
	AddMacro("M4_MATH_PI",m4VariantType(M4CL_MATH_PI) );
	AddMacro("M4_MATH_E", m4VariantType(M4CL_MATH_E) );

	//Lenguajes de Logon y de Desarrollo (canal Sesion)
	AddMacro("M4_LANG_LOGON",M4CL_LANG_LOGON);
	AddMacro("M4_LANG_DEVELOPMENT",M4CL_LANG_DEVELOPMENT);

	//fechas

	vTemp.Type=M4CL_CPP_TYPE_DATE;
	ClStringToDate(M4CL_MINUS_INFINITE_DATE,vTemp.Data.DoubleData);
	AddMacro("MINUS_INF"	, vTemp);
	AddMacro("M4_MINUS_INF"	, vTemp);

	
	ClStringToDate(M4CL_PLUS_INFINITE_DATE,vTemp.Data.DoubleData);
	AddMacro("PLUS_INF"	, vTemp);
	AddMacro("M4_PLUS_INF"	, vTemp);

	// Información de metadatos de nodo
	
	AddMacro( "M4_M4OBJECT_NODE_NUMBER",		M4_CH_VM_CHANNELDEF_NODE_NUMBER ) ;
	AddMacro( "M4_M4OBJECT_CREATION_TYPE",		M4_CH_VM_CHANNELDEF_CREATION_TYPE ) ;
	AddMacro( "M4_M4OBJECT_ID",					M4_CH_VM_CHANNELDEF_ID ) ;
	AddMacro( "M4_M4OBJECT_NAME",				M4_CH_VM_CHANNELDEF_NAME ) ;
	AddMacro( "M4_M4OBJECT_ENVIRONMENT_VARS",	M4_CH_VM_CHANNELDEF_ENVIRONMENT_VARS ) ;
	AddMacro( "M4_M4OBJECT_INSTANCE_NUMBER",	M4_CH_VM_CHANNELDEF_L2_INSTANCE_NUMBER ) ;
	AddMacro( "M4_M4OBJECT_INSTANCE_HANDLE",	M4_CH_VM_CHANNELDEF_L2_INSTANCE_HANDLE ) ;

	// Información de metadatos de nodo
	AddMacro( "M4_NODE_READ_OBJECT_ID",			M4_CH_VM_NODEDEF_READ_OBJECT_ID ) ;
	AddMacro( "M4_NODE_WRITE_OBJECT_ID",		M4_CH_VM_NODEDEF_WRITE_OBJECT_ID ) ;
	AddMacro( "M4_NODE_ID",						M4_CH_VM_NODEDEF_ID ) ;
	AddMacro( "M4_NODE_NAME",					M4_CH_VM_NODEDEF_NAME ) ;
	AddMacro( "M4_NODE_INDEX",					M4_CH_VM_NODEDEF_INDEX ) ;
	AddMacro( "M4_NODE_IS_ROOT",				M4_CH_VM_NODEDEF_IS_ROOT ) ;
	AddMacro( "M4_NODE_IS_REAL_ROOT",			M4_CH_VM_NODEDEF_IS_REAL_ROOT ) ;
	AddMacro( "M4_NODE_PARENT",					M4_CH_VM_NODEDEF_PARENT ) ;
	AddMacro( "M4_NODE_SENTENCE",				M4_CH_VM_NODEDEF_SENTENCE ) ;
	AddMacro( "M4_NODE_MAX_NUM_ROWS_DB",		M4_CH_VM_NODEDEF_MAX_NUM_ROWS_DB ) ;
	AddMacro( "M4_NODE_TYPE",					M4_CH_VM_NODEDEF_TYPE ) ;
	AddMacro( "M4_NODE_AUTO_FILTER",			M4_CH_VM_NODEDEF_AUTO_FILTER ) ;

	// Información de metadatos de item
	AddMacro( "M4_ITEM_TYPE",					M4_CH_VM_ITEMDEF_TYPE ) ;
	AddMacro( "M4_ITEM_SCOPE",					M4_CH_VM_ITEMDEF_SCOPE ) ;
	AddMacro( "M4_ITEM_THEORIC_SCOPE",			M4_CH_VM_ITEMDEF_THEORIC_SCOPE ) ;
	AddMacro( "M4_ITEM_M4TYPE",					M4_CH_VM_ITEMDEF_M4TYPE ) ;
	AddMacro( "M4_ITEM_PRECISION",				M4_CH_VM_ITEMDEF_PRECISION ) ;
	AddMacro( "M4_ITEM_INDEX",					M4_CH_VM_ITEMDEF_INDEX ) ;
	AddMacro( "M4_ITEM_SCALE",					M4_CH_VM_ITEMDEF_SCALE ) ;
	AddMacro( "M4_ITEM_INTERNAL_TYPE",			M4_CH_VM_ITEMDEF_INTERNAL_TYPE ) ;
	AddMacro( "M4_ITEM_READ_OBJECT_ALIAS",		M4_CH_VM_ITEMDEF_READ_OBJECT_ALIAS ) ;
	AddMacro( "M4_ITEM_WRITE_OBJECT_ALIAS",		M4_CH_VM_ITEMDEF_WRITE_OBJECT_ALIAS ) ;
	AddMacro( "M4_ITEM_READ_FIELD_ID",			M4_CH_VM_ITEMDEF_READ_FIELD_ID ) ;
	AddMacro( "M4_ITEM_WRITE_FIELD_ID",			M4_CH_VM_ITEMDEF_WRITE_FIELD_ID ) ;
	AddMacro( "M4_ITEM_READ_OBJECT_ID",			M4_CH_VM_ITEMDEF_READ_OBJECT_ID ) ;
	AddMacro( "M4_ITEM_WRITE_OBJECT_ID",		M4_CH_VM_ITEMDEF_WRITE_OBJECT_ID ) ;
	AddMacro( "M4_ITEM_SLICE_TOTALIZE",			M4_CH_VM_ITEMDEF_SLICE_TOTALIZE ) ;
	AddMacro( "M4_ITEM_NAME",					M4_CH_VM_ITEMDEF_NAME ) ;
	AddMacro( "M4_ITEM_ORDER",					M4_CH_VM_ITEMDEF_ORDER ) ;
	AddMacro( "M4_ITEM_SYNONYM",				M4_CH_VM_ITEMDEF_SYNONYM ) ;
	AddMacro( "M4_ITEM_ID",						M4_CH_VM_ITEMDEF_ID ) ;
	AddMacro( "M4_ITEM_NUMBER",					M4_CH_VM_ITEMDEF_ITEM_NUMBER ) ;
	AddMacro( "M4_ITEM_IS_PK",					M4_CH_VM_ITEMDEF_IS_PK ) ;
	AddMacro( "M4_ITEM_NOT_NULL",				M4_CH_VM_ITEMDEF_NOT_NULL ) ;
	AddMacro( "M4_ITEM_AUXILIAR_ITEM",			M4_CH_VM_ITEMDEF_AUXILIAR_ITEM ) ;
	AddMacro( "M4_ITEM_DMD",					M4_CH_VM_ITEMDEF_DMD ) ;
	AddMacro( "M4_ITEM_DMD_COMPONENT",			M4_CH_VM_ITEMDEF_DMD_COMPONENT ) ;
	AddMacro( "M4_ITEM_DMD_CROSSTAB",			M4_CH_VM_ITEMDEF_DMD_CROSSTAB ) ;
	AddMacro( "M4_ITEM_DMD_FIELD",				M4_CH_VM_ITEMDEF_DMD_FIELD ) ;
	AddMacro( "M4_ITEM_RULE_NUMBER",			M4_CH_VM_ITEMDEF_RULE_NUMBER ) ;
	AddMacro( "M4_ITEM_RULE_ID",				M4_CH_VM_ITEMDEF_RULE_ID ) ;
	AddMacro( "M4_ITEM_RULE_START_DATE",		M4_CH_VM_ITEMDEF_RULE_START_DATE ) ;
	AddMacro( "M4_ITEM_RULE_END_DATE",			M4_CH_VM_ITEMDEF_RULE_END_DATE ) ;
	AddMacro( "M4_ITEM_RULE_AUTOGENERATED",		M4_CH_VM_ITEMDEF_RULE_AUTOGENERATED ) ;
	AddMacro( "M4_ITEM_RULE_CODE",				M4_CH_VM_ITEMDEF_RULE_CODE ) ;

	AddMacro( "M4_ENCODING_NATIVE",				M4UniNative ) ;
	AddMacro( "M4_ENCODING_ANSI",				M4UniANSI ) ;
	AddMacro( "M4_ENCODING_UTF8",				M4UniUtf8 ) ;
	
	// Bookmarks
	AddMacro( "M4_BOOKMARK_POSITION",			M4CL_BOOKMARK_BY_POSITION ) ;
	AddMacro( "M4_BOOKMARK_PK",					M4CL_BOOKMARK_BY_PK ) ;


	//añadimos los atributos!!!
	///OJO!!Deben ir en mayúsculas
	AddAtribute("SYSSLICENUMBER",		M4CL_ATT_SYS_SLICE_NUMBER);
	AddAtribute("SYSSTARTDATE",         M4CL_ATT_SYS_START_DATE);
	AddAtribute("SYSENDDATE",           M4CL_ATT_SYS_END_DATE);
	AddAtribute("SYSFIRSTSLICE",		M4CL_ATT_SYS_FIRST_SLICE);
	AddAtribute("SYSLASTSLICE",			M4CL_ATT_SYS_LAST_SLICE);
    AddAtribute("SYSOLDVALUE",			M4CL_ATT_SYS_OLD_VALUE);
	AddAtribute("SYSBLOBMASK",			M4CL_ATT_SYS_BLOB_MASK);
    AddAtribute("SYSBLOBDESCRIPTION",   M4CL_ATT_SYS_BLOB_DESCRIP);
    AddAtribute("SYSAUXILIARITEMID",    M4CL_ATT_SYS_AUX_ITEM_ID);


	//La tabla de funciones de attributos no la metemos, porque ya se inicializa al pedir la instancia
	//Sinonimos
	m_iExtraMemorySynonym = 0;
	m_eSourceTo = M4_NO_SOURCE;
	m_pszLN4FinalCode = NULL;
	m_bSynonymFound = M4_FALSE;
}




//***Destructor***

ClTrad::~ClTrad(){

	// Limpiamos la polaca
	if (m_pszPolish)
		delete [] m_pszPolish ;
	m_iTamPolish=0;
	
}

//EDU:
m4uint32_t ClTrad::GetSize (void) const
{
    m4uint32_t length = 0;
	
   
    // estimación cutre de la memoria de los mapas
	length += m_oSymbolTable.size () * (sizeof (M4ClString) + sizeof (m4int32_t));
	length += m_oAtributeTable.size () * (sizeof (M4ClString) + sizeof (m4int32_t));
    length += m_oMacroTable.size () * (sizeof (M4ClString) + sizeof (m4int32_t));
    
	//ClAtributeFuncTable
	length += m_oAtributeMethodTable->GetSize () - sizeof (ClAtributeFuncTable); // como el clfunctable es miembro, mi sizeof ya incluye el sizeof (ClAtributeFuncTable)
	
    // estimación de la memoria de las pilas de enteros
	length += m_oArgList.size () * sizeof (int);
	length += m_oLabelStack.size () * sizeof (int);
    length += m_oArgCallStack.size () * sizeof (int);
	
    // estimación cutre de la memoria del set
	length += m_oInternalDepend.size () * sizeof (string);    
	length += m_oExternalDepend.size () * sizeof (string);
	length += m_oChannelDepend.size () * sizeof (string);
	
    // idem del MapSynonym:
	length += m_oSynonymStack.size () * (sizeof (StSynonym));
	length += m_oSynonymMap.size () * (sizeof (m4int32_t) + sizeof (StSynonym));

	// estimación del tamaño de la Polaca
	length += (m_iTamPolish + 1);

    return length;
}

//**Obtener Argumantos
m4return_t ClTrad::SetArguments( void ){
	m4return_t ret;
	m4uint8_t iAutoLoad;
	m_poFinder->GetAccess()->GetpChannel()->WantsAutoLoad.Get (iAutoLoad); 
	m_poFinder->GetAccess()->GetpChannel()->WantsAutoLoad.Set (M4CL_AUTOLOAD_BLOCK);     

	ret = ((ClFinder_TICH *)m_poFinder)->SetArguments();

	m_poFinder->GetAccess()->GetpChannel()->WantsAutoLoad.Set (iAutoLoad);     

	return ret;
	
}



//***Establecer argumentos

m4return_t  ClTrad::SetArguments(m4char_t **ai_ppszArguments, m4bool_t *ai_pbReference, m4int8_t ai_iNumArgs, m4bool_t ai_bVar){
	
	m4int8_t i;
	m4return_t ret=M4_SUCCESS;

	if (ai_iNumArgs > M4CL_MAX_PARAM) {		
		DUMP_CHLOG_ERROR( M4_CH_VM_COMP_IE_TOO_PARAMS );
		bCompileError=M4_TRUE;
		return M4_ERROR;
	}
	
	//Los guardamos en orden inverso 
	if (ai_bVar){ //truqui argumentos variables: añadimos un argumentos más, para que la numeración sea correcta
		//y con el nombre SYS_NUMBER_OF_ARGUMENTS
		m4char_t acArgument[] = "M4CL_ARG_NAME_VAR_ARGS";
		ret=AddArgument(acArgument, M4_FALSE);
        if (ret!=M4_SUCCESS) {            
			DUMP_CHLOG_ERROR( M4_CH_VM_COMP_IE_SETTING_ARGS );
            bCompileError=M4_TRUE;
        }
	}
	
	for (i=ai_iNumArgs-1;i>=0;i--){
		ret=AddArgument(ai_ppszArguments[i] , ai_pbReference[i]);
        if (ret!=M4_SUCCESS) {            
			DUMP_CHLOG_ERROR( M4_CH_VM_COMP_IE_SETTING_ARGS );
            bCompileError=M4_TRUE;
            break;
        }
	}
	
	return ret;
}


m4return_t ClTrad::AddArgument(m4char_t *ai_argument,m4bool_t ai_bReference){
	
	m4return_t ret=M4_SUCCESS;
	m4char_t pszTemp[6*M4CL_MAX_PARAM];
	
	ret=AddSymbol(ai_argument,M4_FALSE);
	//si es por ref. guardamos el número de argumento
	//para poder poner un push de ese arg, antes de retornar
	//(el número de argumento lo guardamos despues de añadirlo,
	//que es cuando está bien(a dif. de las var.) )
	if (ai_bReference){
		sprintf(pszTemp,"%sR%d\n",m_pszPushArgRefAtReturn,m_iNumberOfArguments);
		strcpy(m_pszPushArgRefAtReturn,pszTemp);
		m_iNumOfRefArguments++;
	}
	
	return ret;
}




m4return_t ClTrad::Compile (m4char_t *ai_pszFormula)
{
	
	m4return_t ret=M4_SUCCESS;
	m4int32_t	iParserResult;
	m4char_t    *pszSourceCode, *pszInCode;
    m4uint32_t  iLong;
		
	m_iLastArgRefStrSize=0;
	m_iPolishPos=1;					//iniciamos la posición actual para poder concatenar
	m_pszPolish[0]=0;				//cortamos la polaca
	m_iNumLine=0;
	m_iNumLineParser=1;
	bCompileError = M4_FALSE;
	
    if (ai_pszFormula==0 ) ai_pszFormula="";
	
	m4uint8_t iAutoLoad;
	m_poFinder->GetAccess()->GetpChannel()->WantsAutoLoad.Get (iAutoLoad); 
	m_poFinder->GetAccess()->GetpChannel()->WantsAutoLoad.Set (M4CL_AUTOLOAD_BLOCK);     
    
    iLong=strlen(ai_pszFormula);
    pszSourceCode=NULL;
	
    //si el codigo no acaba en \r\n lo metemos a pelo, para que no falle
    if (iLong<3){
        pszSourceCode= new m4char_t [iLong+3];
		memcpy(pszSourceCode, ai_pszFormula, iLong+1);
		strcat (pszSourceCode+iLong, "\r\n");
        pszInCode=pszSourceCode;
    }
    else if (ai_pszFormula[iLong-1]!='\n' && ai_pszFormula[iLong-2]!='\r'){
		
        pszSourceCode= new m4char_t [iLong+3];
		memcpy(pszSourceCode, ai_pszFormula, iLong+1);
		strcat (pszSourceCode+iLong, "\r\n");
        pszInCode=pszSourceCode;
    }
    else{ //si el código acaba en \r\n lo utilizamos tal cual
        pszInCode=ai_pszFormula;
    }
	m_iLenLN4=strlen(pszInCode);
	
	istrstream oFuente(pszInCode);					//creamos e iniciamos stream del scanner
	yyFlexLexer oScanner(this,&oFuente);			//un scanner creado
	Parser=new ClParser (oScanner,this);			//un parser
	iParserResult = Parser->Parse();				//compilamos
	
    
	ret=(iParserResult == 0 ? M4_SUCCESS :M4_ERROR); //de momento no hacemos caso a lo que retorne PARSE
    if (bCompileError ){    //por si hubo algún error de compialción que no salte por el parser
		//sino por los mensajes (existencia de items, definición devar...)        
		//DUMP_CHLOG_ERROR( M4_CH_VM_COMP_END_WITH_ERRORS );
        ret=M4_ERROR;
    }
	
	//ponemos la versíon de polaca
	SetPolishInfo();
	
	
	//para que no pueda haber una formula sin retorno 
	//añadimos siempre "a pelo" que retorne 0 al final. si ya
	//ha retornado nunca llegará a eso
	
	AddPolish("N0.0\n");
	AddPolish(m_pszPushArgRefAtReturn);
	AddPolish("j0\nL0\nE\n"); //salto al sitio donde está el return


	/* Bug 0104638
	El cambio de las TIs hay que hacerlo siempre aunque falle
	*/
	if( m_eSourceTo == M4_OLD_TI_TO_ID_TI || m_eSourceTo == M4_OLD_ITEM_TO_ID_ITEM )
	{
		CheckToUpdateSynonyms(pszInCode,M4_FALSE);
		if (m_pszLN4FinalCode){
			m_poFinder->SetValue( M4_ITEM_SCHTIS_RULE_CODES_SOURCE_CODE, m_pszLN4FinalCode, M4_TRUE);
			delete []m_pszLN4FinalCode;
			m_pszLN4FinalCode=NULL;
		}
	}


	if ( bCompileError == M4_FALSE ){

		//DEPENDENCIAS
		if (m_eSourceTo==M4_SOURCE_SAVE||m_eSourceTo==M4_GET_POLISH){
			UpdateInternalDepend();
			UpdateExternalDepend();
			UpdateChannelDepend();
		}
		
		//SINONIMOS
		switch( m_eSourceTo ){

		case M4_GET_POLISH:

			m_poFinder->SetValue(M4_ITEM_SCHTIS_RULE_CODES_POLISH_CODE,m_pszPolish, M4_TRUE);
			break;
			
		case M4_RESTORE_SOURCE:

			CheckToUpdateSynonyms (pszInCode,M4_FALSE);
			if ( ret == M4_SUCCESS ){
				if (m_pszLN4FinalCode){
					/* Bug 0084826 No se debe modificar el estado */
					m_poFinder->SetValue( M4_ITEM_SCHTIS_RULE_CODES_SOURCE_CODE, m_pszLN4FinalCode, M4_FALSE );
					delete []m_pszLN4FinalCode;
					m_pszLN4FinalCode=NULL;
				}
				else{
					// Pasamos el codigo de Temp_Code a Source_Code
					/* Bug 0084826 No se debe modificar el estado */
					m_poFinder->SetValue( M4_ITEM_SCHTIS_RULE_CODES_SOURCE_CODE, pszInCode, M4_FALSE );
				}

			}
			break;

		case M4_SOURCE_TO_NUMBER:
		case M4_SOURCE_TO_STRING:
		case M4_SOURCE_TO_ID:
			
			CheckToUpdateSynonyms (pszInCode,M4_FALSE);
			if ( ret == M4_SUCCESS && m_pszLN4FinalCode != NULL ){
				/* Bug 0084826 No se debe modificar el estado */
				m_poFinder->SetValue( M4_ITEM_SCHTIS_RULE_CODES_SOURCE_CODE, m_pszLN4FinalCode, M4_FALSE );
				delete []m_pszLN4FinalCode;
				m_pszLN4FinalCode=NULL;
			}
			break;
			
		case M4_SOURCE_SAVE:
			
			CheckToUpdateSynonyms (pszInCode,M4_TRUE);
			if ( ret == M4_SUCCESS && m_pszLN4FinalCode != NULL ){
				// Temporal. Hasta que este lo del WriteField de TempCode.
				// Luego se escribe directamente en Temp_Code
				ClCompiledMCR *pCMCR = m_poFinder->GetAccess()->GetpCMCR();
				
				ClHandle hTempCode,hRules = pCMCR->GetChannelNodeHandleById( M4_NODE_SCHTIS_RULES, M4_TRUE );
				hTempCode = pCMCR->GetNodeItemHandleById(hRules, M4_ITEM_SCHTIS_RULE_CODES_TEMP_CODE, M4_TRUE );

				// Segun donde esté el Write Field metemos el codigo en Temp_Code o en Source_Code.
				if ( stricmp ( pCMCR->GetItemWriteFieldId( hTempCode ), "" ) == 0 ) {
					/* Bug 0084826 No se debe modificar el estado */
					m_poFinder->SetValue( M4_ITEM_SCHTIS_RULE_CODES_SOURCE_CODE, m_pszLN4FinalCode, M4_FALSE );
				}
				else {
					/* Bug 0084826 No se debe modificar el estado */
					m_poFinder->SetValue( M4_ITEM_SCHTIS_RULE_CODES_TEMP_CODE, m_pszLN4FinalCode, M4_FALSE );
				}

				delete []m_pszLN4FinalCode;
				m_pszLN4FinalCode=NULL;
			}			
			m_poFinder->SetValue(M4_ITEM_SCHTIS_RULE_CODES_POLISH_CODE,m_pszPolish, M4_TRUE);
			break;

		case M4_GET_SYMBOLS:

			if ((m4int32_t)m_iSymbolsTableSize<m_iNumVar ){
				ret=M4_ERROR;
			}
			else{
				itMapaSimbolos it;
				it=m_oSymbolTable.begin();
				while (it!=m_oSymbolTable.end()){
					if ( (*it).second>=0){
						strncpy(m_ppszSymbols[(*it).second], (M4ClString)((*it).first), m_iSymbolSize );
						m_ppszSymbols[(*it).second][m_iSymbolSize-1]=0;
					}
					++it;
				}
			}

			break;

		case M4_TI_BASE_TO_INHERITED:

			CheckToUpdateSynonyms(pszInCode,M4_FALSE);
			if (m_pszLN4FinalCode){
				// Lo tengo que devolver donde me digan.
				/* Bug 0084826 No se debe modificar el estado */
				m_poFinder->SetValue( M4_ITEM_SCHTIS_RULE_CODES_LN4_RESOLVED, m_pszLN4FinalCode, M4_FALSE );
				delete []m_pszLN4FinalCode;
				m_pszLN4FinalCode=0;
			}
			else{
				// Lo pongo a cadena vacía, por si acaso
				/* Bug 0084826 No se debe modificar el estado */
				m_poFinder->SetValue( M4_ITEM_SCHTIS_RULE_CODES_LN4_RESOLVED, pszInCode, M4_FALSE );
			}
			break;
		
		case M4_NO_SOURCE:
		default:
			break;
		}
	}

	while (!m_oSynonymStack.empty()){
		m_oSynonymStack.pop();
	}
	
	m_oSynonymMap.clear();
	
	//LO  HE CAMBIADO DE SITIO PARA SINONIMOS.
    if (pszSourceCode !=NULL){              //si hemos tennido que añadir el \r\n borramos la copia
        delete [] pszSourceCode;
        pszSourceCode=NULL;
	}
	
	
	//Dejamos todo lista para otra compilación
	//DEPENDENCIAS
	
	//vaciar los sets.
	if (!m_oInternalDepend.empty())
	{
		m_oInternalDepend.clear();
	}
	if (!m_oExternalDepend.empty())
	{
		m_oExternalDepend.clear();
	}
	if (!m_oChannelDepend.empty())
	{
		m_oChannelDepend.clear();
	}
	
	
	m_iNumVar=0; 
	m_iNumberOfArguments=0;
	m_iNumOfRefArguments=0;
	m_iNumEtiqueta=1;
	m_iNumLine=0;
	m_iNumLineParser=1;
	delete Parser;
	Parser=NULL;
    bCompileError = M4_FALSE;
	m_bSynonymFound	= M4_FALSE;
	m_bSynonymError=M4_FALSE;
	m_eSourceTo = M4_NO_SOURCE;
	m_iExtraMemorySynonym=0;
	//vaciamos la tabla de símbolos
	m_oSymbolTable.clear();
	
    //vaciamos la listade argumentos
    m_oArgList.clear();
	
    //vaciamos la pila de etiquetas
	while (!m_oLabelStack.empty()) { 
		m_oLabelStack.pop();
	}
    
	
    //vaciamos la pila de contadores de argumentos
    while (!m_oArgCallStack.empty()) { 
		m_oArgCallStack.pop();
	}
    
	
	//vaciamos el buffer de asignaciones y el de arg. de ref.
	strcpy (m_pszPushRefArg,"");
	strcpy(m_pszPushArgRefAtReturn,"");

	m_poFinder->GetAccess()->GetpChannel()->WantsAutoLoad.Set (iAutoLoad);   
	
	return ret;								//hasta luego
}




m4return_t ClTrad::CompileRule (Compile_Type  ai_Type )
{
    m4return_t ret = M4_SUCCESS;	
	m4pchar_t pszFormula = 0;

	ret = ((ClFinder_TICH *)m_poFinder)->GetLN4Code(pszFormula);
		// un poco chapucilla, deveuelve Warning cuando no hay codigo LN4 ( la regla no es de tipo LN4 )
	switch (ret)
	{
	case M4_WARNING:
		ret = M4_SUCCESS;
		break;
	case M4_ERROR:        
		DUMP_CHLOG_ERROR( M4_CH_VM_COMP_UNABLE_GET_LN4_CODE );
        ret = M4_ERROR;
		break;
	default:
		m_eSourceTo = ai_Type ;
		ret = Compile ( pszFormula );
		break;
	}

	return ret;

}

//********************
//Almacenar constantes
//********************

m4return_t ClTrad::StoreNumber (char *ai_number){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->Almacena numero:%s\n",ai_number);
#endif
	m4uint32_t iLong;
    
    iLong=strlen (ai_number);
    while (ai_number [iLong-1] == '0' && ai_number[iLong-1] !='.')
        --iLong;
	
    sprintf(m_BufferTemp,"N%.*s\n",iLong+1,ai_number);
	return AddPolish(m_BufferTemp);
	
}



m4return_t ClTrad::StoreString (char *ai_string){
	
	m4return_t ret=M4_SUCCESS;
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->Almacena cadena:%s\n",ai_string);
#endif
	
	m4uint32_t iLen=strlen(ai_string);
	m4uint32_t i=0;
	
	if (iLen > 0)
	{
		while (ai_string[iLen - i - 1] == '\\') {
			++i;
		}
	}

	ret = AddPolish("S");
	if (i){
		//volcamos la cadena, excepto los i últimos caracteres, incluido el \n
		m4char_t *pszTemp = new m4char_t [iLen-i+1];
		m4char_t acOpCode[50];//Cadena donde montamos el código 'ñapas'. Dejamos margen para 10000000 "\"
		
		strncpy(pszTemp, ai_string, iLen-i);
		
		pszTemp[iLen-i]=0;

		if (ret!=M4_ERROR){
			ret=AddPolish (pszTemp);
		}

		if (ret!=M4_ERROR){
			ret=AddPolish ("\n");
		}

		delete[]pszTemp ;
		//hacemos la multiplicacación y la suma: N92, chr(), i , *, +
		sprintf(acOpCode,"N92\nF201\nN%d\nF11\nF9",i);
		if (ret!=M4_ERROR){
			ret=AddPolish (acOpCode);
		}


	}
	else{
		if ( ret != M4_ERROR ){
			ret = AddPolish(ai_string);
		}
	}


	if (ret!= M4_ERROR){
		ret = AddPolish("\n");
	}
		
	return ret;
}


m4return_t ClTrad::StoreDate (char *ai_date){
	
	m4date_t lFecha;
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->Almacena fecha:%s\n",ai_date);
#endif
	
	ClStringToDate(ai_date,lFecha);
	sprintf(m_BufferTemp,"D%.8lf\n",lFecha);
	
	return AddPolish(m_BufferTemp);
}

//******************
//Funciones  Logicas
//******************

//***AND

m4return_t ClTrad::LogOpAnd(void){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->Operación Lógica AND\n");
#endif
	
	return GenerateBasicFunctionCall ("LOGICALAND");
}

//******************OR

m4return_t ClTrad::LogOpOr(void){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->Operación Lógica OR\n");
#endif
	
	return GenerateBasicFunctionCall ("LOGICALOR");
	
}

//****************NOT
m4return_t ClTrad::LogOpNot(void){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->Operación Lógica NOT\n");
#endif
	
	return GenerateBasicFunctionCall ("LOGICALNOT");
}


//*******************==
m4return_t ClTrad::LogOpEqual(void){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->Operación Lógica EQUAL\n");
#endif
	
	return GenerateBasicFunctionCall ("LOGICALEQUAL");
}


//****************** <>
m4return_t ClTrad::LogOpNotEqual(void){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->Operación Lógica NOT EQUAL\n");
#endif
	
	return GenerateBasicFunctionCall ("LOGICALNOTEQUAL");
}


//********************** >
m4return_t ClTrad::LogOpGreater(void){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->Operación Lógica GREATER\n");
#endif
	
	return GenerateBasicFunctionCall ("LOGICALGREATER");
}

//************ >=
m4return_t ClTrad::LogOpGreaterEqual(void){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->Operación Lógica GREATER_EQUAL\n");
#endif
	
	return GenerateBasicFunctionCall ("LOGICALGREATEREQUAL");
}

//************** <
m4return_t ClTrad::LogOpLess(void){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->Operación Lógica LESS\n");
#endif
	
	return GenerateBasicFunctionCall ("LOGICALLESS");
}


//********************** <=

m4return_t ClTrad::LogOpLessEqual(void){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->Operación Lógica LESS_EQUAL\n");
#endif
	
	return GenerateBasicFunctionCall ("LOGICALLESSEQUAL");
}	



//*******************
//Assign, return
//*******************

m4return_t ClTrad::Assign(void){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->ASSIGN:\n");
#endif
	/*
	1.- comprobamos si es un item
	si no comprobamos si es una variable
	que existe y sino la definimos
	2.- sEgún lo que sea A o OA
	*/
	
	m4int32_t iNumVar;
	m4char_t pszTemp[M4CL_MAX_LINEA_POLACA];
	m4pcchar_t pcRealItem=0;
	
	if ( (strcmp(m_oTargetItem.m_acNode,"")==0) && (m_oTargetItem.m_bTrame==M4_FALSE) ) {//No tiene nodo,ni tramo puede ser variable
		
		//si es un nombre de macro ERROR-No puede asignarse
		if (IsMacro(m_oTargetItem.m_acItem)) {
			DumpError(M4_CH_VM_COMP_ASSIGN_TO_CONST, m_oTargetItem.m_acItem);
			return M4_ERROR;
		}
		
		//miramos si está el la tabla de símbolos
		if (m_oTargetItem.m_acItem[0]=='@' || m_oTargetItem.m_acItem[0]=='#' || m_oTargetItem.m_acItem[0]=='$' || (iNumVar=GetSymbol(m_oTargetItem.m_acItem))==M4CL_NO_SYMBOL) { //no es variable definida
			if (IsItem(m_oTargetItem.m_acItem , m_oTargetItem.m_acNode, m_oTargetItem.m_acChannel, pcRealItem)){
				//escribir  en polaca item
				sprintf(m_BufferTemp,"OA I%s 0\n",pcRealItem);
				
				if (AddDepend(pcRealItem,m_oTargetItem,DependentAssign)!=M4_SUCCESS){
					DumpError(M4_CH_VM_COMP_UNABLE_CREATE_DEPEND, pcRealItem);
				}

				
			}
			else if (m_oTargetItem.m_acItem[0]=='@' || m_oTargetItem.m_acItem[0]=='#' || m_oTargetItem.m_acItem[0]=='$' ) { //una variable no puede empezar por #@$
				/* Bug 0090300 Se quitan los # iniciales */
				DumpError(M4_CH_VM_COMP_ITEM_NOT_EXIST, M4SkipHash( m_oTargetItem.m_acItem ));
				return M4_ERROR;
				
			}
			else{
				//añadir símbolo a tabla
				if (AddSymbol(m_oTargetItem.m_acItem)!=M4_SUCCESS) {
					DumpError(M4_CH_VM_COMP_IE_ADD_SYMBOL, m_oTargetItem.m_acItem);
                    return M4_ERROR;
				}
				//escribir en la polaca
				iNumVar=GetSymbol(m_oTargetItem.m_acItem);
				if (iNumVar!=M4CL_NO_SYMBOL) {
					sprintf(m_BufferTemp,"A%d\n",iNumVar);
				}
				else {
					DumpError(M4_CH_VM_COMP_IE_READ_SYMBOL, m_oTargetItem.m_acItem);
					return M4_ERROR;
				}
			}
		}
		else{	//es una variable definida
			//escribir en la polaca
			sprintf(m_BufferTemp,"A%d\n",iNumVar);
			// Borramos la entrada en el mapa de posibles sinonimos, porque sabemos que es una variable.
			DeleteItemSynonym(m_oTargetItem.m_acItem,M4_FALSE);
		}
	}
	else{
		if (IsItem(m_oTargetItem.m_acItem , m_oTargetItem.m_acNode , m_oTargetItem.m_acChannel, pcRealItem)){ //debe ser item
			// para que se escriba el RealItem en la polaca :
			BasicItem2String(m_oTargetItem,pcRealItem,pszTemp,M4CL_MAX_LINEA_POLACA);
			sprintf(m_BufferTemp,"OA %s\n",pszTemp);
			if (AddDepend(pcRealItem,m_oTargetItem,DependentAssign)!=M4_SUCCESS){
				DumpError(M4_CH_VM_COMP_UNABLE_CREATE_DEPEND, pcRealItem);
			}

		}
		else{
			/* Bug 0090300 Se quitan los # iniciales */
			DumpError(M4_CH_VM_COMP_ITEM_NODE_NOT_EXIST, M4SkipHash( m_oTargetItem.m_acItem ), M4SkipHash( m_oTargetItem.m_acNode ));
			return M4_ERROR;
		}
	}
	return AddPolish(m_BufferTemp);	
}



m4return_t ClTrad::Return(void){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->RETURN\n");
#endif
	
	AddPolish(m_pszPushArgRefAtReturn);
	//return AddPolish("E\n");
	return AddPolish("j0\n");
}


//*******************
//ARITMETICAS
//*******************

//**************** Add

m4return_t ClTrad::AritmOpAdd(void){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->Operación Aritmetica ADD\n");
#endif
	
	return GenerateBasicFunctionCall ("ARITMETICALADD");
}

//*******Sub
m4return_t ClTrad::AritmOpSub(void){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->Operación Aritmetica SUB\n");
#endif
	
	return GenerateBasicFunctionCall ("ARITMETICALSUB");
}

//******************* Mul

m4return_t ClTrad::AritmOpMul(void){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->Operación Aritmetica MUL\n");
#endif
	
	return GenerateBasicFunctionCall ("ARITMETICALMUL");
}

//**************** Div

m4return_t ClTrad::AritmOpDiv(void){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->Operación Aritmetica DIV\n");
#endif
	
	return GenerateBasicFunctionCall ("ARITMETICALDIV");
}


//***************** Signo
m4return_t ClTrad::AritmOpSigno(void){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->Operación Aritmetica CHANGE_SIGN\n");
#endif
	
	return GenerateBasicFunctionCall ("ARITMETICALCHANGESIGN");
}

//*********************
//Argumentos
//*********************


//**********Argum,ento
m4return_t ClTrad::Arg(void){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->Hemos reducido un argumento\n");
#endif
	//para lso tipos guardamos las posiciones
	m_oArgList.push_back(m_iPolishPos-m_iLastArgRefStrSize);  //guardamos la posición del final del argumento que reducimos
	m_iLastArgRefStrSize=0;
    //para el contador de argumentos
    (m_oArgCallStack.top())++;
	
    return M4_SUCCESS;
	
}


m4return_t ClTrad::InitArgList(void){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->InitArgList\n");
#endif
	m_iLastArgRefStrSize=0;
	m_oArgCallStack.push(0);  //guardamos el número de argumentos. De momento 0.
    return M4_SUCCESS;
	
}

m4uint32_t ClTrad::EndArgList(void){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->EndArgList\n");
#endif
	m4uint32_t  iNumArgs;
	
	iNumArgs=m_oArgCallStack.top();  //guardamos el número de argumentos. De momento 0.
	m_oArgCallStack.pop();
    
    return iNumArgs;
	
}

//*********************
//Nodos, items...
//*********************

//****************AtributoMetodo
m4return_t ClTrad::AtributoMetodo(char *ai_celda, char* ai_metodo, char *ai_nArgs){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->Atributo(%s).Metodo(%s) con (%s) argumentos (Los argumentos ya los hemos metido antes (y las posiciones... )\n",ai_celda, ai_metodo, ai_nArgs);
#endif
	
	m4ItemComp_t oTempItem;
	m4return_t ret;
	m4int8_t iRealArgs;
	m4int32_t iNumAtrib;
	m4char_t pszTemp[M4CL_MAX_LINEA_POLACA];
	m4pcchar_t pcRealItem=0;
	
	ScanCell (ai_celda,oTempItem);
	
	iNumAtrib=GetAtributeMethod(ai_metodo);
	if (iNumAtrib==M4CL_NO_ATRIBUTE_METHOD){
		/* Bug 0090300 Se quitan los # iniciales */
		DumpError(M4_CH_VM_COMP_UNDEF_ATTRIB_METHOD, M4SkipHash( ai_metodo ));
		return M4_ERROR;
	}
	
	ret=CheckAtributeMethod(ai_metodo , oTempItem.m_acItem, oTempItem.m_acNode , oTempItem.m_acChannel , atoi(ai_nArgs),iRealArgs,pcRealItem );
	if (ret!=M4_SUCCESS){
		return ret;
	}
	
	ret=BasicItem2String(oTempItem, pcRealItem,pszTemp , M4CL_MAX_LINEA_POLACA);
	if (AddDepend(pcRealItem,oTempItem,DependentExecution)!=M4_SUCCESS)
	{
		DumpError(M4_CH_VM_COMP_UNABLE_CREATE_DEPEND, pcRealItem);
	}

	if (iRealArgs<0){
		sprintf(m_BufferTemp,"N%.1lf\nOT %s A%d\n", atof(ai_nArgs),pszTemp,iNumAtrib);				
	}
	else{
		sprintf(m_BufferTemp,"OT %s A%d\n",pszTemp , iNumAtrib);				
	}
	AddPolish(m_BufferTemp); //a la polaca
	ret=AddPolish(m_pszPushRefArg);//los arg.por ref a la pol
	strcpy(m_pszPushRefArg,"");
	return ret;
	
	
}


//**************Atributo

m4return_t ClTrad::Atributo(char *ai_celda, char *ai_atributo){
	
#ifdef M4_DEBUG_PARSER	
	printf("POLACA->Atributo(%s)\n",ai_celda, ai_atributo);
#endif
	
	m4ItemComp_t oTempItem;
	m4return_t ret;
    m4bool_t bExiste;
	m4int32_t iNumAtrib;
	m4char_t pszTemp[M4CL_MAX_LINEA_POLACA];
	m4pcchar_t pcRealItem=0;
	
	ScanCell (ai_celda,oTempItem);
	
	//comprobamos que exista tamto el item como el nodo
	iNumAtrib=GetAtribute(ai_atributo);
	bExiste=( (IsItem(oTempItem.m_acItem , oTempItem.m_acNode , oTempItem.m_acChannel, pcRealItem) && (iNumAtrib!=M4CL_NO_ATRIBUTE)) ? M4_TRUE : M4_FALSE );
	if (!bExiste){
		DumpError(M4_CH_VM_COMP_UNDEF_ATTRIB, ai_atributo, oTempItem.m_acItem);
		return M4_ERROR;
	}
	ret=BasicItem2String(oTempItem, pcRealItem,pszTemp , M4CL_MAX_LINEA_POLACA);
	if (AddDepend(pcRealItem,oTempItem,DependentRead)!=M4_SUCCESS)
	{
		DumpError(M4_CH_VM_COMP_UNABLE_CREATE_DEPEND, pcRealItem);
	}
	sprintf(m_BufferTemp,"OP %s A%d\n",pszTemp,iNumAtrib);				
	
	return AddPolish(m_BufferTemp); //a la polaca
	
	
}


//********************Metodo

m4return_t ClTrad::Metodo(char *ai_metodo,char* ai_nArgs){
	
#ifdef M4_DEBUG_PARSER	
	printf("POLACA->Metodo o función(%s) con (%s) argumentos (Los argumentos ya los hemos metido antes (y las posiciones)\n",ai_metodo, ai_nArgs);
#endif
	
	/*
	Aquí puede venir un item de método o una función
	En caso de coincidencia tiene preferencia las métodos del nodo.
	*/
	m4ItemComp_t oTempItem;
	m4return_t ret;
	m4int8_t iRealArgs;
	m4uint16_t iId;
	m4int32_t iLevel;
	m4char_t pszTemp[M4CL_MAX_LINEA_POLACA];
	m4pcchar_t pcRealItem=0;
	m4bool_t bItemFounded;
	m4bool_t bIsChannel;

	ret=M4_SUCCESS;
	ScanCell (ai_metodo,oTempItem);
	
	
	
	if ( (strcmp(oTempItem.m_acNode,"")==0) && (oTempItem.m_bTrame==M4_FALSE) ) {//No tiene nodo,ni tramo: puede ser una función
		
		ret=CheckMethodItem( oTempItem.m_acItem,"", oTempItem.m_acChannel , atoi(ai_nArgs),iRealArgs, iLevel,pcRealItem, bItemFounded);
		
        if (ret==M4_SUCCESS){
            if (iLevel!=0){//es un item y tiene bien los param...
                //escribir  en polaca item
                if (iRealArgs<0){
                    sprintf(m_BufferTemp,"N%.1lf\nOF L%d I%s 0\n",atof(ai_nArgs),iLevel,pcRealItem);
                }
                else{
                    sprintf(m_BufferTemp,"OF L%d I%s 0\n",iLevel,pcRealItem);
                }
				if (AddDepend(pcRealItem,oTempItem,DependentExecution)!=M4_SUCCESS)
				{
					DumpError(M4_CH_VM_COMP_UNABLE_CREATE_DEPEND, pcRealItem);
				}
                AddPolish(m_BufferTemp);
                ret=AddPolish(m_pszPushRefArg);
                strcpy(m_pszPushRefArg,"");
            }
            else{ //es un método pero de nivel 0->Error
				DumpError(M4_CH_VM_COMP_METHOD_L0_MUST_BE_L1, oTempItem.m_acItem, oTempItem.m_acNode);
            }
        }
        else { //como método no rula. probamos como función
			if(bItemFounded==M4_FALSE) {
				ret=CheckFunction(oTempItem.m_acItem,iId,atoi(ai_nArgs),iRealArgs,bIsChannel);
				if (ret==M4_SUCCESS){
					if (iRealArgs<0){
						if( bIsChannel == M4_TRUE )
						{
							sprintf(m_BufferTemp,"N%.1lf\nH%d\n",atof(ai_nArgs), iId);
						}
						else
						{
							sprintf(m_BufferTemp,"N%.1lf\nF%d\n",atof(ai_nArgs), iId);
						}
					}
					else{
						if( bIsChannel == M4_TRUE )
						{
							sprintf(m_BufferTemp,"H%d\n",iId);
						}
						else
						{
							sprintf(m_BufferTemp,"F%d\n",iId);
						}
					}
					AddPolish(m_BufferTemp);
					ret=AddPolish(m_pszPushRefArg);
					strcpy(m_pszPushRefArg,"");
				}
				else{ //el mensaje ya sale en CheckFucntion
				}
			}
		}
	}
	else{	//es un método de un item
		if (strcmp(oTempItem.m_acNode,"")==0) {//item[]
			ret=CheckMethodItem( oTempItem.m_acItem,"",oTempItem.m_acChannel ,atoi(ai_nArgs),iRealArgs,iLevel,pcRealItem, bItemFounded);
			if (ret==M4_SUCCESS){
				if (iRealArgs<0){
					sprintf(m_BufferTemp,"N%.1lf\nOF L%d I%s 1\n",atof(ai_nArgs), iLevel,pcRealItem);
				}
				else{
					sprintf(m_BufferTemp,"OF L%d I%s 1\n",iLevel,pcRealItem);
				}
				if (AddDepend(pcRealItem,oTempItem,DependentExecution)!=M4_SUCCESS)
				{
					DumpError(M4_CH_VM_COMP_UNABLE_CREATE_DEPEND, pcRealItem);
				}
				AddPolish(m_BufferTemp);
				ret=AddPolish(m_pszPushRefArg);
				strcpy(m_pszPushRefArg,"");
			}
			else{
				if (!bItemFounded){ //si no se encontro sacamos el error, si falla por argumentos ya ha salido
					/* Bug 0090300 Se quitan los # iniciales */
					DumpError(M4_CH_VM_COMP_ITEM_NOT_EXIST, M4SkipHash( oTempItem.m_acItem ));
				}
			}
		}
		else{ //nodo.item  o  nodo.item[] o con canal 
			ret=CheckMethodItem( oTempItem.m_acItem , oTempItem.m_acNode , oTempItem.m_acChannel ,atoi(ai_nArgs),iRealArgs,iLevel,pcRealItem, bItemFounded);
			if (ret==M4_SUCCESS){
				ret=BasicItem2String(oTempItem,pcRealItem, pszTemp , M4CL_MAX_LINEA_POLACA);
				if (iRealArgs<0){
					sprintf(m_BufferTemp,"N%.1lf\nOF L%d %s\n", atof(ai_nArgs) ,iLevel, pszTemp );
				}
				else{
					sprintf(m_BufferTemp,"OF L%d %s\n",iLevel,pszTemp);
				}
				if (AddDepend(pcRealItem,oTempItem,DependentExecution)!=M4_SUCCESS)
				{
					DumpError(M4_CH_VM_COMP_UNABLE_CREATE_DEPEND, pcRealItem);
				}
				AddPolish(m_BufferTemp);
				ret=AddPolish(m_pszPushRefArg);
				strcpy(m_pszPushRefArg,"");
			}
			else{
				if (!bItemFounded){ //si no se encontro sacamos el error, si falla por argumentos ya ha salido
					/* Bug 0090300 Se quitan los # iniciales */
					DumpError(M4_CH_VM_COMP_UNDEF_METHOD_NODE, M4SkipHash( oTempItem.m_acItem ), M4SkipHash( oTempItem.m_acNode ));
				}
			}
		}
	}
	return ret;
}


//***************MetodoRegla

m4return_t ClTrad::MetodoRegla(char *ai_metodo, char *ai_regla,char* ai_nArgs){
	
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->Metodo(%s).Regla(%s) con (%s) argumentos (Los argumentos ya los hemos metido antes (y las posiciones... )\n", ai_metodo, ai_regla, ai_nArgs);	
#endif
	
	m4ItemComp_t oTempItem;
	m4return_t ret;
	m4int8_t iRealArgs;
	m4int32_t iLevel;
	m4char_t pszTemp [M4CL_MAX_LINEA_POLACA];
	m4pcchar_t pcRealItem=0;
	m4char_t	acRule[M4CL_MAX_LEN_RULE+1];
	
	strncpy(acRule,ai_regla,M4CL_MAX_LEN_RULE);
	acRule[M4CL_MAX_LEN_RULE]=0;//nulterminamos

	ScanCell (ai_metodo,oTempItem);
	
	ret=CheckMethodRule(acRule , oTempItem.m_acItem, oTempItem.m_acNode ,oTempItem.m_acChannel ,atoi(ai_nArgs) ,iRealArgs, iLevel,pcRealItem);
	if (ret!=M4_SUCCESS){ 
		return ret;
	}
	
	BasicItem2String (oTempItem,pcRealItem, pszTemp, M4CL_MAX_LINEA_POLACA);
	if (iRealArgs<0){
		sprintf(m_BufferTemp,"N%.1lf\nOF L%d %s R%s\n",atof(ai_nArgs), iLevel, pszTemp,  acRule);
	}
	else{
		sprintf(m_BufferTemp,"OF L%d %s R%s\n",iLevel, pszTemp ,acRule);
	}
	if (AddDepend(pcRealItem,oTempItem,DependentExecution)!=M4_SUCCESS)
	{
		DumpError(M4_CH_VM_COMP_UNABLE_CREATE_DEPEND, pcRealItem);
	}
	
	AddPolish(m_BufferTemp); //a la polaca
	ret=AddPolish(m_pszPushRefArg);
	strcpy(m_pszPushRefArg,"");
	
	return ret;
	
	
}


//***************Metodo del padre

m4return_t ClTrad::MetodoPadre(char *ai_metodo,char* ai_nArgs){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->Metodo del nodo padre (%s) con (%s) argumentos (Los argumentos ya los hemos metido antes (y las posiciones)\n",ai_metodo, ai_nArgs);
#endif
	return M4_ERROR;
	/*de momento hemos quitado lo poder acceder a la regla del padre, lo dejo comentado por si vuelve....

	
	m4ItemComp_t oTempItem;
	m4return_t ret;
	m4int8_t iRealArgs;
	m4int32_t iLevel;
	m4char_t pszTemp [M4CL_MAX_LINEA_POLACA];
	m4pcchar_t pcRealItem=0;
	
	ScanCell (ai_metodo,oTempItem);
	
	ret=CheckFatherMethod(oTempItem.m_acItem, oTempItem.m_acNode,oTempItem.m_acChannel ,atoi(ai_nArgs),iRealArgs,iLevel,pcRealItem);
	if (ret!=M4_SUCCESS){
		return ret;
	}
				
	ret=BasicItem2String (oTempItem, pcRealItem,pszTemp, M4CL_MAX_LINEA_POLACA);
	if (iRealArgs<0){
		sprintf(m_BufferTemp,"N%.1lf\nOF F L%d %s %s\n", atof(ai_nArgs) ,iLevel, pszTemp);
	}
	else{
		sprintf(m_BufferTemp,"OF F L%d %s\n",iLevel, pszTemp );
	}
	if (AddDepend(pcRealItem,oTempItem,M4_TRUE)!=M4_SUCCESS)
	{
		DumpError(M4_CH_VM_COMP_UNABLE_CREATE_DEPEND, pcRealItem);
	}
	AddPolish(m_BufferTemp); //a la polaca
	ret=AddPolish(m_pszPushRefArg);
	strcpy(m_pszPushRefArg,"");
	
	return ret;
	
	*/	
}

//*********metodo con regla del padre

m4return_t ClTrad::MetodoReglaPadre(char *ai_metodo, char *ai_regla,char* ai_nArgs){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->Metodo(%s).Regla(%s) con (%s) argumentos (Los argumentos ya los hemos metido antes (y las posiciones... )\n", ai_metodo, ai_regla, ai_nArgs);	
#endif

	return M4_ERROR;
	/*de momento hemos quitado lo poder acceder a la regla del padre, lo dejo comentado por si vuelve....

	
	m4ItemComp_t oTempItem;
	m4return_t ret;
	m4int8_t iRealArgs;
	m4int32_t iLevel;
	m4char_t pszTemp [M4CL_MAX_LINEA_POLACA];
	m4pcchar_t pcRealItem=0;
	m4char_t	acRule[M4CL_MAX_LEN_RULE+1];
	
	strncpy(acRule,ai_regla,M4CL_MAX_LEN_RULE);
	acRule[M4CL_MAX_LEN_RULE]=0;//nulterminamos

	
	ScanCell (ai_metodo,oTempItem);
	
	ret=CheckFatherMethodRule(acRule , oTempItem.m_acItem, oTempItem.m_acNode ,oTempItem.m_acChannel , atoi(ai_nArgs),iRealArgs,iLevel,pcRealItem );
	if (ret!=M4_SUCCESS){
		return ret;
	}
	BasicItem2String (oTempItem, pcRealItem,pszTemp, M4CL_MAX_LINEA_POLACA);
	if (iRealArgs<0){
		sprintf(m_BufferTemp,"N%.1lf\nOF F L%d %s R%s\n", atof(ai_nArgs), iLevel, pszTemp, acRule);
	}
	else{
		sprintf(m_BufferTemp,"OF F L%d %s R%s\n",iLevel, pszTemp  ,acRule);
	}
	if (AddDepend(pcRealItem,oTempItem,M4_TRUE)!=M4_SUCCESS)
	{
		DumpError(M4_CH_VM_COMP_UNABLE_CREATE_DEPEND, pcRealItem);
	}
	AddPolish(m_BufferTemp); //a la polaca
	ret=AddPolish(m_pszPushRefArg);
	strcpy(m_pszPushRefArg,"");
	
	return ret;
	*/
}


//***ItemValorRead

m4return_t ClTrad::ItemValorRead (char *ai_celda){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->ItemValorRead(%s)(asignable,pero lo estamos leyendo) \n",ai_celda);
#endif
	
	/*
	1.- comprobamos si es un item
	si no comprobamos si es una variable
	que existe 
	2.- sEgún lo que sea R o OR
	*/
	m4ItemComp_t oTempItem;
	m4int32_t iNumVar;
	m4VariantType vMacro;
	m4return_t ret;
	m4char_t pszTemp [M4CL_MAX_LINEA_POLACA];
	m4pcchar_t pcRealItem=0;
	
	ret=M4_SUCCESS;
	
	ScanCell(ai_celda,oTempItem);
	
	if ( (strcmp(oTempItem.m_acNode,"")==0) && (oTempItem.m_bTrame==M4_FALSE) ) {//No tiene nodo,ni tramo puede ser variable o macro
		
		//si es un nombre de macro sustituimos por el valor
		if (GetMacro(oTempItem.m_acItem, vMacro) ) {
			switch (vMacro.Type){
			case M4CL_CPP_TYPE_NUMBER:
				sprintf(pszTemp,"%.8lf",vMacro.Data.DoubleData); //constantes numéricas enteras
				return StoreNumber(pszTemp); //para que almacene lo que deba
			case M4CL_CPP_TYPE_DATE:
				sprintf(m_BufferTemp,"D%.8lf\n",vMacro.Data.DoubleData);
				break;
			case M4CL_CPP_TYPE_STRING_VAR:
				sprintf(m_BufferTemp,"S%s\n",vMacro.Data.PointerChar);
				break;
			}
			
			return AddPolish(m_BufferTemp);	
		}
		
		//miramos si está el la tabla de símbolos
		if (oTempItem.m_acItem[0]=='@' || oTempItem.m_acItem[0]=='#' || oTempItem.m_acItem[0]=='$'  ||  (iNumVar=GetSymbol(oTempItem.m_acItem))==M4CL_NO_SYMBOL ) { //no es variable definida
			if (IsItem(oTempItem.m_acItem , oTempItem.m_acNode, oTempItem.m_acChannel, pcRealItem)){
				//escribir  en polaca item. No tiene tramo
				sprintf(m_BufferTemp,"OR I%s 0\n",pcRealItem);
				
				if (AddDepend(pcRealItem,oTempItem,DependentRead)!=M4_SUCCESS){
					//error
					DumpError(M4_CH_VM_COMP_UNABLE_CREATE_DEPEND, pcRealItem);
				}
			}
			else{
				/* Bug 0090300 Se quitan los # iniciales */
				DumpError(M4_CH_VM_COMP_UNDEF_ITEM_VARIABLE, M4SkipHash( oTempItem.m_acItem ));
				ret=M4_ERROR;
			}
		}
		else{	//es una variable definida
			//escribir en la polaca
			sprintf(m_BufferTemp,"R%d\n",iNumVar);
			// Borramos la entrada en el mapa de posibles sinonimos, porque sabemos que es una variable.
			/* Bug 0117629
			El símbolo no es el de m_oTargetItem.m_acItem sino el de oTempItem.m_acItem
			*/
			DeleteItemSynonym(oTempItem.m_acItem,M4_FALSE);
		}
	}
	else{ //debe ser item de un nodo
		if (!IsItem(oTempItem.m_acItem , oTempItem.m_acNode, oTempItem.m_acChannel, pcRealItem)){
			//si solo puede ser item y no lo es error
			/* Bug 0090300 Se quitan los # iniciales */
			DumpError(M4_CH_VM_COMP_UNDEF_ITEM, M4SkipHash( oTempItem.m_acItem ), M4SkipHash( oTempItem.m_acNode ));
			ret=M4_ERROR;
		}
		else 
		{
			BasicItem2String (oTempItem, pcRealItem,pszTemp, M4CL_MAX_LINEA_POLACA);
			sprintf(m_BufferTemp,"OR %s\n",pszTemp);
			
			if (ret!=M4_ERROR)
				if (AddDepend(pcRealItem,oTempItem,DependentRead)!=M4_SUCCESS)
				{
					DumpError(M4_CH_VM_COMP_UNABLE_CREATE_DEPEND, pcRealItem);
				}
		}
			
	}
	
	if (ret!=M4_ERROR){
		ret=AddPolish(m_BufferTemp);	
	}
	else{//metemos en la polaca una "cadena de relleno", para que si se sigue compilando no
			//falle la cuenta de argumentos y saquemos los errores bien (y podamos sacar todos los 
			//que tenga la formula)
		AddPolish("C0\n");
		//No asignamos a ret, porque hay un error, y debe seguir siendo M4_ERROR

	}
	
	return ret;
}



//*******función del canal
m4return_t ClTrad::ChannelFunction (char *ai_channel, char *ai_function, char *ai_nArgs){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->Función (%s) de un canal (%s) con %d arg\n",ai_channel, ai_function, ai_nArgs);
#endif
	
	
	m4return_t ret;
	m4int8_t iRealArgs;
	m4uint16_t iId;
	m4char_t  acChannel[M4CL_MAX_LEN_CHANNEL+1];

	strncpy(acChannel,ai_channel,M4CL_MAX_LEN_CHANNEL);
	acChannel[M4CL_MAX_LEN_CHANNEL]=0;//nulterminamos
	
	ret=CheckChannelFunction(ai_function, iId, atoi(ai_nArgs) , iRealArgs);
	
	if (ret!=M4_SUCCESS){ //el mensaje ya sale en CheckChannelFunction
		return ret;
	}
	
	if (! m_poFinder->IsAliasOrMyT3(acChannel) ){
		//el mensaje ya sale en CheckChannelFunction
		return M4_ERROR;
	}


	if (iRealArgs<0){
		sprintf(m_BufferTemp,"N%.1lf\nH%d C%s\n",atof(ai_nArgs),iId, acChannel);				
	}
	else{
		sprintf(m_BufferTemp,"H%d C%s\n",iId, acChannel);				
	}
	
	ret=AddPolish(m_BufferTemp); //a la polaca
	
	return ret;
}


//***************************
//Control de flujo
//***************************


//***If
m4return_t ClTrad::If(void){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->IF\n");
#endif
	
	m_oLabelStack.push(m_iNumEtiqueta); //metemos la etiqueta del siguiente bloque (else o elseif)
	m_iNumEtiqueta++;
	m_oLabelStack.push(m_iNumEtiqueta); //metemos la etiqueta del fin
	m_iNumEtiqueta++;
	
	return M4_SUCCESS;
}

//***Then
m4return_t ClTrad::Then (void){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->THEN\n");
#endif
	
	return M4_SUCCESS;
}


//Else
m4return_t ClTrad::Else (void){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->ELSE\n");
#endif	
	
	//salta incondicional a la etiqueta del tope y cambiamos el tope-1 por el sig
	m4int32_t iTope;
	m4int32_t iTope_1; //tope -1
	m4int32_t iPolishPosTemp,iOldLineRef;
	m4return_t ret=M4_SUCCESS;
	
    if (m_oLabelStack.size() <2){ //debe tener 2 elementos al menos        
		DUMP_CHLOG_ERROR( M4_CH_VM_COMP_IE_CANT_GET_LABELS_STACK );
        return M4_ERROR;
    }
	
	iTope=m_oLabelStack.top(); //sacamos la primera y guardamos el valor
	m_oLabelStack.pop();
	
	iTope_1=m_oLabelStack.top(); //leemos la anterior,sin sacarla


	// Si la linea anterior es un retorno hay que ponerlo después del jump
	if( m_iPolishPos < 3 )
	{
		iPolishPosTemp = 0 ;
	}
	else
	{
		iPolishPosTemp = m_iPolishPos - 3 ;
	}

	// Retrocedemos una linea
	while( iPolishPosTemp >= 0 && m_pszPolish[ iPolishPosTemp ] != '\n' )
	{
		iPolishPosTemp-- ;
	}
	iPolishPosTemp++ ;
	
	//Comprobamos si la linea anterior era C%d
	if( m_pszPolish[ iPolishPosTemp ] == 'C' )
	{
		m_iPolishPos = iPolishPosTemp + 1 ;
		sscanf( m_pszPolish + m_iPolishPos, "%d", &iOldLineRef ) ;
		sprintf(m_BufferTemp,"j%d\nL%d\nC%d\n",iTope_1,iTope,iOldLineRef); //ponemos el salto a top-1 y etiqueta top
	}
	else
	{
		sprintf(m_BufferTemp,"j%d\nL%d\n",iTope_1,iTope); //ponemos el salto a top-1 y etiqueta top
	}

	
	ret=AddPolish(m_BufferTemp);
	
	m_oLabelStack.push(iTope_1); //metemos lo que no debería ir para
	//que en el endif solo tomemos en consideración el caso
	// de que no hay ningún else, ni elseif. En el Endif se comprueba si los
	//2 valores de la pila son consecutivos, cosa que solo pasa si no ha habido ningún elseif
	// y ningún else (ya que aquí nos encargamos de que no queden consecutivos)							
	
	return ret;
}


//***Elseif
m4return_t ClTrad::Elseif (void){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->ELSEIF\n");
#endif
	
	//salta incondicional a la etiqueta del tope 
	m4int32_t iTope;
	m4int32_t iTope_1; //tope -1
	m4int32_t iPolishPosTemp,iOldLineRef;
	m4return_t ret=M4_SUCCESS;
	
    if (m_oLabelStack.size() <2){ //debe tener 2 elementos al menos        
		DUMP_CHLOG_ERROR( M4_CH_VM_COMP_IE_CANT_GET_LABELS_STACK );
        return M4_ERROR;
    }
	
	iTope=m_oLabelStack.top(); //sacamos la primera y guardamos el valor
	m_oLabelStack.pop();
	
	iTope_1=m_oLabelStack.top(); //leemos la anterior (la del endif),sin sacarla
	
	
	// Si la linea anterior es un retorno hay que ponerlo después del jump
	if( m_iPolishPos < 3 )
	{
		iPolishPosTemp = 0 ;
	}
	else
	{
		iPolishPosTemp = m_iPolishPos - 3 ;
	}

	// Retrocedemos una linea
	while( iPolishPosTemp >= 0 && m_pszPolish[ iPolishPosTemp ] != '\n' )
	{
		iPolishPosTemp-- ;
	}
	iPolishPosTemp++ ;
	
	//Comprobamos si la linea anterior era C%d
	if( m_pszPolish[ iPolishPosTemp ] == 'C' )
	{
		m_iPolishPos = iPolishPosTemp + 1 ;
		sscanf( m_pszPolish + m_iPolishPos, "%d", &iOldLineRef ) ;
		sprintf(m_BufferTemp,"j%d\nL%d\nC%d\n",iTope_1,iTope,iOldLineRef); //ponemos el salto a top-1(final) y etiqueta top
	}
	else
	{
		sprintf(m_BufferTemp,"j%d\nL%d\n",iTope_1,iTope); //ponemos el salto a top-1(final) y etiqueta top
	}

	ret=AddPolish(m_BufferTemp);
	
	//metemos en la pila la siguiente etiqueta
	m_oLabelStack.push(m_iNumEtiqueta); //metemos la sig. etiqueta 
	m_iNumEtiqueta++;
	
	
	return ret;
}


//***Endif
m4return_t ClTrad::Endif (void){
	
#ifdef M4_DEBUG_PARSER	
	printf("POLACA->ENDIF\n");
#endif
	m4return_t ret=M4_SUCCESS;
	m4int32_t iTope, iTope_1;
	m4int32_t iPolishPosTemp,iOldLineRef;
	
    if (m_oLabelStack.size() <2){ //debe tener 2 elementos al menos        
		DUMP_CHLOG_ERROR( M4_CH_VM_COMP_IE_CANT_GET_LABELS_STACK );
        return M4_ERROR;
    }
	
	iTope=m_oLabelStack.top(); //sacamos la etiqueta de endif
	m_oLabelStack.pop();
	iTope_1=m_oLabelStack.top(); //sacamos la etiqueta de endif
	m_oLabelStack.pop();


	/* Bug 0089566
	Si la linea anterior es un retorno hay que ponerlo después de las etiquetas
	*/
	// 
	if( m_iPolishPos < 3 )
	{
		iPolishPosTemp = 0 ;
	}
	else
	{
		iPolishPosTemp = m_iPolishPos - 3 ;
	}

	// Retrocedemos una linea
	while( iPolishPosTemp >= 0 && m_pszPolish[ iPolishPosTemp ] != '\n' )
	{
		iPolishPosTemp-- ;
	}
	iPolishPosTemp++ ;
	
	//Comprobamos si la linea anterior era C%d
	if( m_pszPolish[ iPolishPosTemp ] == 'C' )
	{
		m_iPolishPos = iPolishPosTemp + 1 ;
		sscanf( m_pszPolish + m_iPolishPos, "%d", &iOldLineRef ) ;
		sprintf(m_BufferTemp,"L%d\nL%d\nC%d\n",iTope_1, iTope,iOldLineRef);
	}
	else
	{
		sprintf(m_BufferTemp,"L%d\nL%d\n",iTope_1, iTope);
	}


	ret=AddPolish(m_BufferTemp);
	
	return ret;
	
}


//***Do
m4return_t ClTrad::Do (void){
	
#ifdef M4_DEBUG_PARSER		
	printf("POLACA->DO\n");
#endif
	
	m4return_t ret=M4_SUCCESS;
	
	sprintf(m_BufferTemp,"L%d\n",m_iNumEtiqueta); //ponemos etiqueta del DO
	ret=AddPolish(m_BufferTemp);
	//metemos en la pila esa etiqueta
	m_oLabelStack.push(-m_iNumEtiqueta); 
	//Metemos la etiqueta en negativo para que la condición
	//sepa que es la de un DO y la quite de la pila cuando
	//llegue a ella
	m_iNumEtiqueta++;
	
	return ret;
}


//****For
m4return_t ClTrad::For (void){
	
#ifdef M4_DEBUG_PARSER	
	printf("POLACA->FOR\n");
#endif
	return M4_SUCCESS;
}

//***To
m4return_t ClTrad::To (void){
	
#ifdef M4_DEBUG_PARSER			
	printf("POLACA->TO\n");
#endif
	return M4_SUCCESS;
}

//***Next
m4return_t ClTrad::Next (void){
#ifdef M4_DEBUG_PARSER				
	printf("POLACA->NEXT\n");
#endif
	
	m4int32_t iEtiqueta1, iEtiqueta2, iNumVar;
	m4return_t ret=M4_SUCCESS;
	m4uint16_t iId;
	
    if (m_oLabelStack.size() <3){ //debe tener 2 elementos al menos        
		DUMP_CHLOG_ERROR( M4_CH_VM_COMP_IE_CANT_GET_LABELS_STACK );
        return M4_ERROR;
    }
	
	iNumVar=m_oLabelStack.top(); 
	m_oLabelStack.pop();
	iEtiqueta1=m_oLabelStack.top(); 
	m_oLabelStack.pop();
	iEtiqueta2=m_oLabelStack.top(); 
	m_oLabelStack.pop();
	
	sprintf(m_BufferTemp,"R%d\n",iNumVar); // poner 1 y variable EN POLACA
	ret=AddPolish(m_BufferTemp);
	if (ret==M4_SUCCESS){
		iId=M4CL_VM_FT_INCREMENT;
		sprintf(m_BufferTemp,"F%d\nA%d\nj%d\nL%d\n",iId,iNumVar,iEtiqueta1,iEtiqueta2);
		ret=AddPolish(m_BufferTemp);
	}
	return ret;
	
	
}

//***While
m4return_t ClTrad::While (void){
	
#ifdef M4_DEBUG_PARSER			
	printf("POLACA->WHILE\n");
#endif
	m4return_t ret=M4_SUCCESS;
	
	//metemos en la pila la nueva etiqueta
	m_oLabelStack.push(m_iNumEtiqueta); 
	
	sprintf(m_BufferTemp,"L%d\n",m_iNumEtiqueta); //ponemos etiqueta EN POLACA de inicio de While
	ret=AddPolish(m_BufferTemp);
	
	m_iNumEtiqueta++;
	m_oLabelStack.push(m_iNumEtiqueta); 
	m_iNumEtiqueta++;
	
	return ret;
}


//***Wend
m4return_t ClTrad::Wend (void){
	
#ifdef M4_DEBUG_PARSER				
	printf("POLACA->WEND\n");
#endif
	m4int32_t iTope,iSalto;
	m4return_t ret=M4_SUCCESS;
	
    if (m_oLabelStack.size() <2){ //debe tener 2 elementos al menos        
		DUMP_CHLOG_ERROR( M4_CH_VM_COMP_IE_CANT_GET_LABELS_STACK );
        return M4_ERROR;
    }
	
	iTope=m_oLabelStack.top(); //sacamos la etiqueta de while de  la pila
	m_oLabelStack.pop();
	iSalto=m_oLabelStack.top(); //sacamos la etiqueta de while de  la pila
	m_oLabelStack.pop();
	
	sprintf(m_BufferTemp,"j%d\nL%d\n",iSalto,iTope); //ponemos etiqueta EN POLACA
	ret=AddPolish(m_BufferTemp);
	return ret;
	
}


//***Until
m4return_t ClTrad::Until (void){
	
#ifdef M4_DEBUG_PARSER				
	printf("POLACA->UNTIL\n");
#endif
	return M4_SUCCESS;
}


//***StoreFor
m4return_t ClTrad::StoreFor(m4char_t *ai_identifier){
	
#ifdef M4_DEBUG_PARSER					
	printf("POLACA->FOR\n");
#endif
	
	m4int32_t iNumVar=0;
	m4return_t ret=M4_SUCCESS;
	m4pcchar_t pcRealItem=0;
	
	//si es un nombre de macro ERROR-No puede asignarse
	if (IsMacro(ai_identifier)) {
		DumpError(M4_CH_VM_COMP_ASSIGN_TO_CONST, m_oTargetItem.m_acItem);
        //para evitar que falle el resto de la compilación y poder seguir 
        //mirando errores del Source metemos las etiquetas en la pila como
        //si hubiera ido bien, para que no falle al intantar sacarlas
		
		ret = M4_ERROR;
	}
	
	//determinar nº de variable y si no existe crearla
	if (ai_identifier[0]=='@' || ai_identifier[0]=='#' ||ai_identifier[0]=='$' || (iNumVar=GetSymbol(ai_identifier))==M4CL_NO_SYMBOL ) { //no es variable definida
		//si es un item---error
		if (ai_identifier[0]=='@' || ai_identifier[0]=='#' ||ai_identifier[0]=='$' || IsItem(ai_identifier,"","", pcRealItem)){		
			DumpError(M4_CH_VM_COMP_ITEN_CANT_BE_ITERATION_VAR, ai_identifier);
			ret = M4_ERROR;
		}
		else {//Ni variable definida, ni item, ni macro ->añadir símbolo a tabla
            if(AddSymbol(ai_identifier)!=M4_SUCCESS) {
				DumpError(M4_CH_VM_COMP_IE_ADD_SYMBOL, ai_identifier);
				ret = M4_ERROR;
            }
            else{
                //obtenemos el número de variable
                iNumVar=GetSymbol(ai_identifier);
            }
        }
		
	}
	
	m_oLabelStack.push(m_iNumEtiqueta); //metemos etiqueta2
	m_iNumEtiqueta++;
	m_oLabelStack.push(m_iNumEtiqueta); //metemos etiqueta1
	m_iNumEtiqueta++;
	m_oLabelStack.push(iNumVar); 
	
	sprintf(m_BufferTemp,"A%d\nL%d\n",iNumVar,m_iNumEtiqueta-1); //ponemos asignación y etiqueta EN POLACA
	ret=AddPolish(m_BufferTemp);
	return ret;
}


//***FinFor
m4return_t ClTrad::FinFor(void){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->FIN DE FOR\n");
#endif
	
	m4int32_t iNumVar;
	m4return_t ret=M4_SUCCESS;
	m4uint16_t iId;
	m4int32_t iEtiqueta2, iEtiqueta1;
	
    if (m_oLabelStack.size() <3){ //debe tener 3 elementos al menos        
		DUMP_CHLOG_ERROR( M4_CH_VM_COMP_IE_CANT_GET_LABELS_STACK );
        return M4_ERROR;
    }
	
	iNumVar=m_oLabelStack.top(); //sacamos el numero de var
	m_oLabelStack.pop(); //lo sacamos para leer la etiqueta1
	iEtiqueta1=m_oLabelStack.top();
	m_oLabelStack.pop(); //lo sacamos para leer la etiqueta2
	iEtiqueta2=m_oLabelStack.top();
	
	m_oLabelStack.push(iEtiqueta1); //volvemos a meter etiqueta1
	m_oLabelStack.push(iNumVar); //volvemos a meter el numvar
	
	sprintf(m_BufferTemp,"R%d\n",iNumVar); //ponemos "leer variable"
	ret=AddPolish(m_BufferTemp);
	if (ret==M4_SUCCESS){
		//metemos greater porque según el orden en que se reducen 
		//las cosas debe comprobarse > en vez de <=
		iId=M4CL_VM_FT_LOGICALGREATEREQUAL;
		sprintf(m_BufferTemp,"F%d\nJ%d\n",iId,iEtiqueta2);
		ret=AddPolish(m_BufferTemp);
	}
	
	return ret;
}


//***Purge
m4return_t ClTrad::Purge(void){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->PURGE\n");
#endif
	return AddPolish("P1\n");	//las funciones de las que 
	//no tomamos el valor deben purgar uno de la pila
	
}


//***************************
//Asignación + condiciones
//***************************


//***Target
m4return_t ClTrad::Target(char *ai_target){
	
#ifdef M4_DEBUG_PARSER
	printf("POLACA->TARGET:\n");
#endif
	return ScanCell(ai_target,m_oTargetItem);
}


//***Condition
m4return_t ClTrad::Condition(void){
	
#ifdef M4_DEBUG_PARSER	
	printf("POLACA->CONDITION\n");
#endif
	//salta si false a la etiqueta del tope 
	m4int32_t iTope;
	m4return_t ret=M4_SUCCESS;
    //m4uint16_t iId;
	
    if (m_oLabelStack.size() <1){ //debe tener 1 elementos al menos        
		DUMP_CHLOG_ERROR( M4_CH_VM_COMP_IE_CANT_GET_LABELS_STACK );
        return M4_ERROR;
    }
	
	iTope=m_oLabelStack.top(); //leemos la etiqueta
	//ATENCION:::::
	/*si el número de etiqueta es negativo hay que sacarla de la pila
	Esto se debe a que el 'DO' va a meter la etiqueta en negativo para saber
	que es suya. En este caso hay que sacar la etiqueta ya que la cond. es
	el final de la sentencia, y no se puede sacar antes para que la condición
	sepa que etiqueta debe poner*/
	if (iTope<0) {
		m_oLabelStack.pop();
		iTope=-iTope; //ponemos el valor en positivo
					  /*truco cutre y asqueroso para evitar que un NULL en 
					  una condición nos cree un bucle infinito.
					  En los final de do-until (cond) como si la condición es falsa sigue iterando
					  y como si hay un null implicado la condición dara siempre falsa, lo que hacemos es
					  un (NOT  ( NOT (---) ) ) y así si hay un null implicado quedara:
					  NOt (NULL) = falso.
					  Not (Falso) =true ->podemos salir del bucle
					  ----Finalmente este truco no se hace, pero me lo guardo pa'l futuro
					  iId=M4CL_VM_FT_LOGICALNOT;
					  //ret=m_oFuncTable->GetId("LogicalNot",iId); 
					  //if (ret!=M4_SUCCESS){					  
					  //DUMP_CHLOG_ERRORF( M4_CH_VM_COMP_IE_NO_FUNCTION, "LogicalNot" );
					  //bCompileError=M4_TRUE;
					  //}
					  //else{
					  sprintf(m_BufferTemp,"F%d\nF%d\n",iId,iId);
					  ret=AddPolish(m_BufferTemp);
	//}*/
		
	}
	
	
	sprintf(m_BufferTemp,"J%d\n",iTope); //ponemos el salto a top
	ret=AddPolish(m_BufferTemp);
	
	return ret;
}


//************************
//Escribir numero de linea
//************************
m4return_t ClTrad::WriteLine(void){
	
#ifdef M4_DEBUG_PARSER	
	printf("POLACA->CONDITION\n");
#endif
	
	m4uint32_t tam;
	m4int32_t iOldLine;
	m4return_t ret=M4_SUCCESS;
	m4char_t acNumLine[12];			// C123456 => Max. 999.999 Lineas de Polaca
	m4int32_t iPolishPosTemp,iOldLineRef;
	
	
	//gestión del número de linea
	iOldLine=m_iNumLine;
	m_iNumLine=Parser->GetNumLine();
	if (m_iNumLine-iOldLine>0){
		//miramos si justo antes hay otro C%d para unirlos
		iPolishPosTemp=m_iPolishPos;
		//RRRR
		if ( m_iPolishPos < 3 )	
			iPolishPosTemp = 0;
		else 
		{
			iPolishPosTemp-=3;

			//Bug:67798 (>=)
			// Cuando se compila una regla con varias lineas de comentarios al comienzo de la regla,
			//  el compilador generaba dos instrucciones de polaca C
			while (iPolishPosTemp>=0 && m_pszPolish[iPolishPosTemp] !='\n'){ //retrocedemos una linea
				iPolishPosTemp--;
			}
			iPolishPosTemp++;
		}
		
		//Comprobamos si la linea anterior era C%d (fusion), y preparamos Nueva Linea
		if (m_pszPolish[iPolishPosTemp]=='C'){ //antes habia otra referencia de linea de código
			sscanf(&m_pszPolish[++iPolishPosTemp],"%d",&iOldLineRef);		// Leemos el no. de linea anterior
			sprintf(acNumLine,"%d\n",m_iNumLine-iOldLine+iOldLineRef);	// Escribimos el nuevo no. en buffer temporal: %d

		}
		else{
			sprintf(acNumLine,"C%d\n",m_iNumLine-iOldLine);				// Escribimos el nuevo no. en buffer temporal: C%d
			// Preparamos para concatenar
			iPolishPosTemp = m_iPolishPos-1;
		}

		ret=M4_SUCCESS;
		tam=strlen(acNumLine);

		// Redimensionamos el Buffer de Polaca si es necesario
		while ( (tam>=unsigned(m_iTamPolish-iPolishPosTemp)) && (ret==M4_SUCCESS) )  {
			ret = ResizePolish();
		}

		// Concatenamos
		if (ret == M4_SUCCESS ) {
			memcpy(m_pszPolish+iPolishPosTemp, acNumLine, tam+1);	//escribimos y actualizamos la posición
			iPolishPosTemp += tam;
			
			m_iPolishPos=iPolishPosTemp+1;	// ajustamos la Posicion de la polaca
			ret=M4_SUCCESS;
		}
		else{
			ret=M4_ERROR;
		}
	}
	//Incrementamos el No. de linea procesada por el Parser
	++m_iNumLineParser;
	
	return ret;
}

//******************************
//Funciones auxiliares
//******************************

//***
//*********Descomponer un item y meterlo en la estructura 	m4ItemComp_t	
//***
m4return_t ClTrad::ScanCell(m4char_t *ai_item,m4ItemComp_t	&ao_oRet){
	
/*
Posibilidades: 
Channel!Nodo.item
Channel!Nodo.item[]
Channel!Nodo<>.item
Channel!Nodo<>.item[]
Nodo.item
Nodo.item[]
Nodo<>.item
Nodo<>.item[]
Item
Item[]

  Si alguna de ellas tiene '[]' indica que tiene tramo
  <> indica que lleva fila
	*/
	
	m4char_t *psTemp;
	m4char_t *pszLine;
	
	//pasamos a mayusculas
	ai_item=strupr(ai_item);
	
	//no hay tramos
	strcpy(ao_oRet.m_acRule,"");//las celdas no tiene reglas
	
	pszLine=ai_item;			//iniciamos el puntero para poder ir avanzando
	
	//miramos si viene con canal o no
	if ( (psTemp=strchr(pszLine,'!'))!=NULL){	//channel!nodo.item[]
		strncpy(ao_oRet.m_acChannel,pszLine,psTemp-pszLine); //copiamos el nodo
		ao_oRet.m_acChannel[psTemp-pszLine]=0; //terminamos en 0
		pszLine=++psTemp;		//ahora miramos a partir de canal
	}
	else{
		strcpy(ao_oRet.m_acChannel,"");
	}
	
	//Separan nodo e item, si procede
	if ( (psTemp=strchr(pszLine,'.'))!=NULL){	//nodo.item[]
		strncpy(ao_oRet.m_acNode,pszLine,psTemp-pszLine); //copiamos el nodo
		ao_oRet.m_acNode[psTemp-pszLine]=0;
		strcpy(ao_oRet.m_acItem,++psTemp);				//y el item
		
		//Miramos si tiene row o no
		if ( (psTemp=strchr(ao_oRet.m_acNode,'<')) !=NULL){	//miramos si tiene tramo
			*psTemp='\0';	//pasamos del '<>' si existe
			ao_oRet.m_bRow=M4_TRUE;
		}
		else {
			ao_oRet.m_bRow=M4_FALSE;
		}
	}
	else{	//item
		strcpy(ao_oRet.m_acItem,pszLine);				//copiamos el item
		strcpy(ao_oRet.m_acNode,"");					//no hay nodo
		ao_oRet.m_bRow=M4_FALSE;
		
	}
	
	//Miramos si tiene tramo
	if ( (psTemp=strchr(ao_oRet.m_acItem,'[')) !=NULL){	//miramos si tiene tramo
		*psTemp='\0';	//pasamos del '[]' si existe
		ao_oRet.m_bTrame=M4_TRUE;
	}
	else {
		ao_oRet.m_bTrame=M4_FALSE;
	}
	
	return M4_SUCCESS;
}


//*****************************
//***Tabla de simbolos e  items
//*****************************

m4return_t ClTrad::AddSymbol(m4char_t *ai_symbol,m4bool_t ai_bIsSymbol){
	
	m4bool_t bRes;
	
	//pasamos a mayusculas
	ai_symbol=strupr(ai_symbol);
	if (!ai_bIsSymbol){
		m_iNumberOfArguments--;
		bRes=( ((m_oSymbolTable.insert(MapaSimbolos::value_type(ai_symbol,m_iNumberOfArguments))).second) ? M4_TRUE : M4_FALSE );
	}
	else{
		bRes=( ((m_oSymbolTable.insert(MapaSimbolos::value_type(ai_symbol,m_iNumVar))).second) ? M4_TRUE : M4_FALSE );
		m_iNumVar++;
	}
	return (bRes==M4_TRUE ? M4_SUCCESS: M4_ERROR);
}


m4int32_t ClTrad::GetSymbol(m4char_t *ai_symbol){
	itMapaSimbolos it;
	
	//pasamos a mayusculas
	ai_symbol=strupr(ai_symbol);
	
	it=m_oSymbolTable.find(ai_symbol);
	if (it==m_oSymbolTable.end()) return M4CL_NO_SYMBOL;
	else return (*it).second;
	
}

//*****************
//***********Macros
//*****************

m4return_t ClTrad::AddMacro(m4char_t *ai_pszName,m4VariantType ai_iVal){
	
	
	//pasamos a mayusculas
	ai_pszName=strupr(ai_pszName);
	
	return ( ((m_oMacroTable.insert(MapaSimbolosVariant::value_type(ai_pszName,ai_iVal))).second) ? M4_SUCCESS : M4_ERROR);
	
	
}

m4return_t ClTrad::AddMacro(m4char_t *ai_pszName,m4int32_t ai_iVal){
	
	
	//pasamos a mayusculas
	ai_pszName=strupr(ai_pszName);
	
	return ( ((m_oMacroTable.insert(MapaSimbolosVariant::value_type(ai_pszName,m4VariantType(ai_iVal) ) ) ).second) ? M4_SUCCESS : M4_ERROR);
	
}

m4bool_t ClTrad::IsMacro(m4char_t *ai_pszName){
	
	itMapaSimbolosVariant it;
	
	
	//pasamos a mayusculas
	ai_pszName=strupr(ai_pszName);
	
	it=m_oMacroTable.find(ai_pszName);
	if (it==m_oMacroTable.end()) return M4_FALSE;
	
	return M4_TRUE;
}



m4bool_t	ClTrad::GetMacro        (m4char_t *ai_pszName, m4VariantType &ao_iVal){
	itMapaSimbolosVariant it;
	
	//pasamos a mayusculas
	ai_pszName=strupr(ai_pszName);
	
	it=m_oMacroTable.find(ai_pszName);
	if (it==m_oMacroTable.end()) return M4_FALSE;
	else{
		ao_iVal= (*it).second;
		return M4_TRUE;
	}
}


//*****************
//********Atributos
//*****************

m4return_t ClTrad::AddAtribute(m4char_t *ai_pszName,m4int32_t ai_iVal){
	
	//pasamos a mayusculas
	//ai_pszName=strupr(ai_pszName);
	return ( ((m_oAtributeTable.insert(MapaSimbolos::value_type(ai_pszName,ai_iVal))).second) ? M4_SUCCESS: M4_ERROR );
	
}


//compuruba que sea un nombre de atributo válido
m4bool_t ClTrad::IsAtribute(m4char_t *ai_pszName){
	
	itMapaSimbolos it;
	
	//pasamos a mayusculas
	ai_pszName=strupr(ai_pszName);
	
	it=m_oAtributeTable.find(ai_pszName);
	if (it==m_oAtributeTable.end()) return M4_FALSE;
	
	return M4_TRUE;
}



m4int32_t ClTrad::GetAtribute(m4char_t *ai_pszName){
	
	itMapaSimbolos it;
	
	//pasamos a mayusculas
	ai_pszName=strupr(ai_pszName);
	
	it=m_oAtributeTable.find(ai_pszName);
	if (it==m_oAtributeTable.end()) return M4CL_NO_ATRIBUTE;
	else return (*it).second;
	
}


//************************
//********Atributos-Metodo
//************************
m4return_t ClTrad::AddAtributeMethod(m4char_t *ai_pszName, m4int8_t ai_iNumArg, m4uint16_t ai_iId, m4char_t *ai_acParamDesc, m4bool_t ai_bGenDep){
	
	m4return_t ret;
	
	
	ret=m_oAtributeMethodTable->AddAttribute(ai_pszName,ai_iNumArg,ai_iId,ai_acParamDesc, ai_bGenDep);
	return ret;
}


//compuruba que sea un nombre de atributo-metodo válido
m4bool_t ClTrad::IsAtributeMethod(m4char_t *ai_pszName){
	
	m4return_t ret;
	m4char_t acArgDesc[M4CL_MAX_PARAM];
	m4uint16_t Id;
	m4int8_t iRealArgs, iType;
	
	//pasamos a mayusculas
	ai_pszName=strupr(ai_pszName);
	
	ret=m_oAtributeMethodTable->GetId(ai_pszName,Id,iRealArgs,acArgDesc, iType);
	return (ret==M4_SUCCESS ? M4_TRUE : M4_FALSE);
}



m4int32_t ClTrad::GetAtributeMethod(m4char_t *ai_pszName){
	
	m4return_t ret;
	m4char_t acArgDesc[M4CL_MAX_PARAM];
	m4uint16_t Id;
	m4int8_t iRealArgs, iType;
	
	//pasamos a mayusculas
	ai_pszName=strupr(ai_pszName);
	
	ret=m_oAtributeMethodTable->GetId(ai_pszName,Id,iRealArgs,acArgDesc, iType);
	if (ret==M4_SUCCESS){
		return Id;
	}
	else {
		return M4CL_NO_ATRIBUTE_METHOD;
	}
}


m4return_t ClTrad::CheckAtributeMethod(m4char_t *ai_Atribute, m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel , m4int8_t ai_nArgs, m4int8_t &ai_iRealArgs,m4pcchar_t &ao_pRealItem){
	
	m4return_t ret=M4_SUCCESS;
	m4char_t acArgDesc[M4CL_MAX_PARAM];
	m4uint16_t Id;
	m4int8_t iType;
	m4pcchar_t pcRealItem=0;
	
	//pasamos a mayusculas
	ai_Atribute=strupr(ai_Atribute);
	ai_Item=strupr(ai_Item);
	ai_Node=strupr(ai_Node);
	ai_Channel=strupr(ai_Channel);
	
	//miramos si es item
	if (IsItem(ai_Item,ai_Node, ai_Channel, pcRealItem)){
		//miramos si es atributo metodo
		ao_pRealItem = pcRealItem;
		ret=m_oAtributeMethodTable->GetId(ai_Atribute,Id,ai_iRealArgs,acArgDesc, iType);
		if (ret==M4_SUCCESS){
			ret=CheckArgs(ai_iRealArgs,ai_nArgs,acArgDesc,ai_Atribute); 

		}
		else{
			DumpError(M4_CH_VM_COMP_UNDEF_ATTRIB, ai_Atribute, ai_Item);
		}
		
	}
	else{
		DumpError(M4_CH_VM_COMP_IS_NOT_ITEM, ai_Item);
		ret=M4_ERROR;
	}
	
	return ret;
}


//********************
//Validez
//********************

m4return_t ClTrad::CheckArgs(m4int8_t ai_iNumArgsReal,m4int8_t ai_iNumArgsPassed, m4char_t * ai_acArgDesc, m4char_t * ai_pszErrorString){
	
/*	1.- Comprobar número de parámetros 
2.- Comprobar tipos (R->Valor r->Referencia)

  Codificación del número de parámetros:
  n>=0  Exactamente n parametros
  n<0   n parámetros al menos o más. Los que pasen de n sontodor por valor
  n<-M4CL_MAX_PARAM   Numero variable sin un fijo
    */
	
	int i;
	m4return_t ret=M4_SUCCESS;
	
	
	//vaciamos el buffer de asignaciones 
	strcpy (m_pszPushRefArg,"");
	
	if (ai_iNumArgsReal>0)  { //tienen que coincidir exactamente
		if (ai_iNumArgsReal==ai_iNumArgsPassed){
			for (i=ai_iNumArgsPassed-1;i>=0;--i){
				ret=CheckSimpleArg(ai_acArgDesc[i]);
				if (ret==M4_ERROR){
					sprintf (m_pszErrorString, "%d", i+1);
					if (ai_acArgDesc[i]=='r'){
						DumpError(M4_CH_VM_COMP_WRONG_ARGUMENT_TYPE_REF, m_pszErrorString, ai_pszErrorString);
					}
					else{
						DumpError(M4_CH_VM_COMP_WRONG_ARGUMENT_TYPE_VALUE, m_pszErrorString, ai_pszErrorString);
					}
					//break;  dejamos de comprobar
				}
			}
		}
		else{ //el número de argumentos no es válido
			sprintf (m_pszErrorString, "%d", ai_iNumArgsReal);
			DumpError(M4_CH_VM_COMP_WRONG_NUM_PARAM_IN_FUNCTION, m_pszErrorString, ai_pszErrorString);
			ret=M4_ERROR;
		}
	}
	
	else if (ai_iNumArgsReal<0 && ai_iNumArgsReal>-M4CL_MAX_PARAM){ //Número variable pero con un mínimo
		if (-ai_iNumArgsReal<=ai_iNumArgsPassed){

			//los opcionalaes (ultimos) , son todos por valor
			for (i=0;i<ai_iNumArgsPassed+ai_iNumArgsReal;i++){ //los opcionales son siempre por valor
				ret=CheckSimpleArg('R');
				if (ret==M4_ERROR){
					sprintf (m_pszErrorString, "%d", i+(-ai_iNumArgsReal)+1);
					DumpError(M4_CH_VM_COMP_WRONG_ARGUMENT_TYPE_VALUE, m_pszErrorString, ai_pszErrorString); 
					//break;  dejamos de comprobar
				}
			}

			for (i=(-ai_iNumArgsReal)-1;i>=0;--i){//comprobamos los obligatorios con su cadena de formato
				ret=CheckSimpleArg(ai_acArgDesc[i]);
				
				if (ret==M4_ERROR){
					sprintf (m_pszErrorString, "%d", i+1);
					if (ai_acArgDesc[i]=='r'){
						DumpError(M4_CH_VM_COMP_WRONG_ARGUMENT_TYPE_REF, m_pszErrorString, ai_pszErrorString);
					}
					else{
						DumpError(M4_CH_VM_COMP_WRONG_ARGUMENT_TYPE_VALUE, m_pszErrorString, ai_pszErrorString);
					}
					//break;  dejamos de comprobar
				}
			}
		}
		else{ //el número de argumentos no es válido
			sprintf (m_pszErrorString, "%d", (-ai_iNumArgsReal));
			DumpError(M4_CH_VM_COMP_WRONG_NUM_PARAM_IN_FUNCTION_LEAST, m_pszErrorString, ai_pszErrorString);
			ret=M4_ERROR;
		}
	}
	
	
	else if (ai_iNumArgsReal==0){ //sin argumentos
		if (ai_iNumArgsPassed==0){
			ret=M4_SUCCESS;
		}
		else{ //el número de argumentos no es válido
			DumpError(M4_CH_VM_COMP_WRONG_FUNCTION_METHOD_NOT_PARAM, ai_pszErrorString);
			ret=M4_ERROR;
		}
	}
	
	
	else { //Número variable y sin mínimo
		for (i=0;i<ai_iNumArgsPassed;i++){
			ret=CheckSimpleArg('R');
			if (ret==M4_ERROR){
				sprintf (m_pszErrorString, "%d", i+1);
				DumpError(M4_CH_VM_COMP_WRONG_ARGUMENT_TYPE_VALUE, m_pszErrorString, ai_pszErrorString);
				//break;  dejamos de comprobar
			}
		}
	}
	m_iLastArgRefStrSize=strlen(m_pszPushRefArg);
	return ret;
}



m4return_t ClTrad::CheckFunction(m4char_t *ai_acName,m4uint16_t &ai_id, m4int8_t ai_iNumArgsPassed, m4int8_t &ao_iRealArgs, m4bool_t &ao_rbIsChannel){
	
	m4return_t ret;
	m4char_t acArgDesc[M4CL_MAX_PARAM];
	

	ao_rbIsChannel = M4_FALSE ;

	//pasamos a mayusculas
	ai_acName=strupr(ai_acName);
	
	
	ret=m_oFuncTable->GetIdSemantic(ai_acName,ai_id,ao_iRealArgs,acArgDesc);

	// Ahora se permite que una función de canal se llame sin alias
	if( ret != M4_SUCCESS )
	{
		ret=m_oFuncTable->GetIdChannel(ai_acName,ai_id,ao_iRealArgs,acArgDesc);

		if( ret == M4_SUCCESS )
		{
			ao_rbIsChannel = M4_TRUE ;
		}
	}
	
	if ( ret==M4_SUCCESS ){
		ret=CheckArgs(ao_iRealArgs,ai_iNumArgsPassed,acArgDesc,ai_acName);	
	}
	else{
		/* Bug 0090300 Se quitan los # iniciales */
		DumpError(M4_CH_VM_COMP_UNDEF_FUNCTION_METHOD, M4SkipHash( ai_acName ));
	}
	return ret;
}



m4return_t ClTrad::CheckChannelFunction(m4char_t *ai_acName,m4uint16_t &ai_id, m4int8_t ai_iNumArgsPassed, m4int8_t &ao_iRealArgs){
	
	m4return_t ret;
	m4char_t acArgDesc[M4CL_MAX_PARAM];
	
	
	//pasamos a mayusculas
	ai_acName=strupr(ai_acName);
	
	
	ret=m_oFuncTable->GetIdChannel(ai_acName,ai_id,ao_iRealArgs,acArgDesc);
	
	if (ret==M4_SUCCESS){
		ret=CheckArgs(ao_iRealArgs,ai_iNumArgsPassed,acArgDesc,ai_acName);	
	}
	else{
		/* Bug 0090300 Se quitan los # iniciales */
		DumpError(M4_CH_VM_COMP_UNDEF_CHANNEL_FUNCTION, M4SkipHash( ai_acName ));
	}

	return ret;
}




m4return_t ClTrad::CheckSimpleArg(m4char_t ai_cType){
	
	m4int32_t iPosFinArg;
	m4char_t pszTemp[M4CL_MAX_LINEA_POLACA*M4CL_MAX_PARAM];
	m4return_t ret;
	
	ret=M4_ERROR;
	
	//leemos la posición final del argumento
	if (m_oArgList.empty()) {
		return M4_ERROR;
	}
	
	iPosFinArg=m_oArgList.back();
	m_oArgList.pop_back();
	
	iPosFinArg-=3;
	while (iPosFinArg>=0 && m_pszPolish[iPosFinArg] !='\n'){
		iPosFinArg--;
	}
	iPosFinArg++;
	
	//ya estamos al principio de la linea de la polaca del argumento
	if (ai_cType=='R'){ //valor
		if (strchr("NSDRFH",m_pszPolish[iPosFinArg])!=NULL){
			ret=M4_SUCCESS;
		}
		else if (m_pszPolish[iPosFinArg]=='O'    && (strchr("NSDRFPT",m_pszPolish[iPosFinArg+1])!=NULL) ){ //aqui sobraría SDN
			ret=M4_SUCCESS;
		}
	}
	else if (ai_cType=='r'){ //referencia
		if (m_pszPolish[iPosFinArg]=='R'){
			
			//copiamos a la cadena  pszPushRefArg el argumento, pero
			//en logar de poner 'R' (leer) ponemos 'A' meter en la pila
			ReadLine(&m_pszPolish[iPosFinArg],pszTemp,M4CL_MAX_LINEA_POLACA);
			pszTemp[0]='A'; //cambiamos de leer a asignar

			strcat (pszTemp,"\n");
			strcat (pszTemp,m_pszPushRefArg);
			strcpy (m_pszPushRefArg,pszTemp);
			ret=M4_SUCCESS;
		}
		else if (m_pszPolish[iPosFinArg]=='O'  &&  m_pszPolish[iPosFinArg+1]=='R'){
			
			//copiamos a la cadena  pszPushRefArg el argumento, pero
			//en logar de poner 'OR' (leer) ponemos 'OA' meter en la pila
			ReadLine(&m_pszPolish[iPosFinArg],pszTemp,M4CL_MAX_LINEA_POLACA);
			pszTemp[1]='A'; //cambiamos de leer a asignar

			strcat (pszTemp,"\n");
			strcat (pszTemp,m_pszPushRefArg);
			strcpy (m_pszPushRefArg,pszTemp);
			ret=M4_SUCCESS;
		}
	}
	return ret;
}




m4return_t ClTrad::CheckMethodItem(m4char_t *ai_Item, m4char_t *ai_Node,m4char_t *ai_Channel , m4int8_t ai_nArgs, m4int8_t &ao_iRealArgs, m4int32_t &ao_iLevel,m4pcchar_t &ao_pcRealItem, m4bool_t &ao_ItemFound){
	
	m4return_t ret;

    m4char_t    acArgDes[M4CL_MAX_PARAM];
	
	//pasamos a mayusculas
	ai_Item=strupr(ai_Item);
	ai_Node=strupr(ai_Node);
	ai_Channel=strupr(ai_Channel);	
	
	
	ao_ItemFound=IsMethodItem(ai_Item,ai_Node , ai_Channel, ao_iRealArgs,ao_iLevel,acArgDes,ao_pcRealItem);
	
	ret= (ao_ItemFound==M4_TRUE ? M4_SUCCESS:M4_ERROR);
	if (ret==M4_SUCCESS){
		ret=CheckArgs(ao_iRealArgs,ai_nArgs,acArgDes,ai_Item);
	}
	
	return ret;
}



m4return_t ClTrad::CheckMethodRule(m4char_t *ai_Rule, m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel ,m4int8_t ai_nArgs, m4int8_t &ao_iRealArgs, m4int32_t &ao_iLevel,m4pcchar_t &ao_pRealItem){ 
	
	m4return_t ret;
	m4bool_t bRet;
    m4char_t    acArgDes[M4CL_MAX_PARAM];
	
	//pasamos a mayusculas
	ai_Node=strupr(ai_Node);
	ai_Item=strupr(ai_Item);
	ai_Node=strupr(ai_Node);
	ai_Channel=strupr(ai_Channel);	
	
	bRet=IsMethodItem(ai_Item,ai_Node,ai_Channel, ao_iRealArgs,ao_iLevel,acArgDes,ao_pRealItem);
	if (bRet==M4_FALSE){
		/* Bug 0090300 Se quitan los # iniciales */
		DumpError(M4_CH_VM_COMP_ITEM_NOT_EXIST, M4SkipHash( ai_Item ));
	}
	
	ret= (bRet==M4_TRUE ? M4_SUCCESS:M4_ERROR);
	if (ret==M4_SUCCESS){
		ret=CheckArgs(ao_iRealArgs,ai_nArgs,acArgDes,ai_Item);
		if (ret==M4_SUCCESS){
			bRet=IsRule(ai_Item, ai_Node, ai_Channel, ai_Rule);
			ret= (bRet==M4_TRUE ? M4_SUCCESS:M4_ERROR);
	
			if (bRet==M4_FALSE){
				/* Bug 0090300 Se quitan los # iniciales */
				DumpError(M4_CH_VM_COMP_UNDEF_METHOD_RULE, M4SkipHash( ai_Rule ) ,M4SkipHash( ai_Item ));
			}

		}
	}
	return ret;
}



m4return_t ClTrad::CheckFatherMethod(m4char_t *ai_Item, m4char_t *ai_Node,m4char_t *ai_Channel , m4int8_t ai_nArgs, m4int8_t &ao_iRealArgs, m4int32_t &ao_iLevel,m4pcchar_t &ao_pcRealItem){
	
	return M4_ERROR;
	/*de momento hemos quitado lo poder acceder a la regla del padre, lo dejo comentado por si vuelve....

	m4return_t ret;
	m4bool_t bRet;
    m4char_t    acArgDes[M4CL_MAX_PARAM];
	
	//pasamos a mayusculas
	ai_Item=strupr(ai_Item);
	ai_Node=strupr(ai_Node);
	ai_Channel=strupr(ai_Channel);	
	
	bRet=IsFatherMethod(ai_Item , ai_Node , ai_Channel, ao_iRealArgs,ao_iLevel,acArgDes,ao_pcRealItem);
	if (bRet==M4_FALSE){
		DumpError(M4_CH_VM_COMP_UNDEF_METHOD_IN_PARENT, ai_Item);
	}

	ret= (bRet==M4_TRUE ? M4_SUCCESS:M4_ERROR);
	
	if (ret==M4_SUCCESS){
		ret=CheckArgs(ao_iRealArgs,ai_nArgs,acArgDes,ai_Item);	//suponemos que de verdad tiene 2 argumentos
	}
	
	return ret;
	*/
}



m4return_t ClTrad::CheckFatherMethodRule(m4char_t *ai_Rule, m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel, m4int8_t ai_nArgs, m4int8_t &ao_iRealArgs, m4int32_t &ao_iLevel,m4pcchar_t &ao_pcRealItem){

	return M4_ERROR;
	/*de momento hemos quitado lo poder acceder a la regla del padre, lo dejo comentado por si vuelve....

	m4return_t ret;
	m4bool_t bRet;
    m4char_t    acArgDes[M4CL_MAX_PARAM];
	
	//pasamos a mayusculas
	ai_Rule=strupr(ai_Rule);
	ai_Item=strupr(ai_Item);
	ai_Node=strupr(ai_Node);
	ai_Channel=strupr(ai_Channel);	
	
	
	bRet=IsFatherMethod(ai_Item , ai_Node, ai_Channel, ao_iRealArgs,ao_iLevel,acArgDes,ao_pcRealItem);
	if (bRet==M4_FALSE){
		DumpError(M4_CH_VM_COMP_UNDEF_METHOD_IN_PARENT, ai_Item);
	}

	ret= (bRet==M4_TRUE ? M4_SUCCESS:M4_ERROR);
	
	if (ret==M4_SUCCESS){
		ret=CheckArgs(ao_iRealArgs,ai_nArgs,acArgDes,ai_Item); //suponemos que de verdad tiene 2 argumentos
		if (ret==M4_SUCCESS){
			bRet=IsFatherRule(ai_Item, ai_Node, ai_Channel, ai_Rule);
			ret= (bRet==M4_TRUE ? M4_SUCCESS:M4_ERROR);
			if (bRet==M4_FALSE){
				DumpError(M4_CH_VM_COMP_UNDEF_METHOD_RULE_IN_PARENT, ai_Rule, ai_Item);
			}
		}
	}
	return ret;
	*/
}



//********************
//Existencia
//********************





m4bool_t ClTrad::IsItem (m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel, m4pcchar_t &ao_RealItem){
	
	
	return m_poFinder->IsItem(ai_Item,ai_Node,ai_Channel,ao_RealItem);
}



m4bool_t ClTrad::IsMethodItem(m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel, m4int8_t &ao_iRealArgs, m4int32_t &ao_iLevel, m4char_t *ao_acArgDes,m4pcchar_t &ao_pcRealItem){
	
	
	return m_poFinder->IsMethodItem(ai_Item,ai_Node,ai_Channel,ao_iRealArgs,ao_iLevel,ao_acArgDes,ao_pcRealItem);
	
	
}



m4bool_t ClTrad::IsRule(m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel, m4char_t *ai_Rule){
	
	
	return m_poFinder->IsRule(ai_Item,ai_Node,ai_Channel,ai_Rule);
	
}



m4bool_t ClTrad::IsFatherMethod(m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel, m4int8_t &ao_iRealArgs, m4int32_t &ao_iLevel, m4char_t *ao_acArgDes,m4pcchar_t &ao_pcRealItem){
	
	return M4_FALSE;
	/*de momento quitamos lo de la herencia, ya que ahora, la herencia de ti's-channel
	solo guarda la ultima regla sobreescrita (la + específica) y no vamos a poder llamar
	a la del padre en RT, porque el código no se instala.
	Lo dejo comentado en lugar de quitarlo, porque estoy seguro de que lo van a volver a pedir
	return m_poFinder->IsFatherMethod(ai_Item,ai_Node,ai_Channel,ao_iRealArgs,ao_iLevel,ao_acArgDes,ao_pcRealItem);
	*/
}



m4bool_t ClTrad::IsFatherRule(m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel, m4char_t *ai_Rule){
	
	return M4_FALSE;
	/*de momento quitamos lo de la herencia, ya que ahora, la herencia de ti's-channel
	solo guarda la ultima regla sobreescrita (la + específica) y no vamos a poder llamar
	a la del padre en RT, porque el código no se instala.
	Lo dejo comentado en lugar de quitarlo, porque estoy seguro de que lo van a volver a pedir
	return m_poFinder->IsFatherRule(ai_Item,ai_Node,ai_Channel,ai_Rule);
	*/
}


//***********************************
// Redimensiona el buffer de Polaca
//***********************************

m4return_t ClTrad::ResizePolish( void )
{
	m4uint32_t	iNewSize ;
	m4return_t ret;

	iNewSize = m_iTamPolish + M4_TRAD_POLISH_MAX_SIZE ;
	char *pszPolish = new char[iNewSize + 1];
	if (pszPolish) {
		// Redimensionamos el buffer de la Polaca y su tamaño
		memcpy( pszPolish, m_pszPolish, m_iTamPolish );	// strcpy
		delete [] m_pszPolish;
		m_pszPolish	= pszPolish;
		m_iTamPolish = iNewSize;
		ret=M4_SUCCESS;
	} 
	else{
		// No podemos redimensionar el buffer de la Polaca
		DumpError(M4_CH_VM_COMP_IE_NOT_MEMORY, "");
		ret=M4_ERROR;
	}

	return ret;
}


//***********************************
// Escribir en la polaca
//***********************************

m4return_t ClTrad::AddPolish(m4char_t *ai_buffer){
	
	m4uint32_t tam;
	m4return_t ret=M4_SUCCESS;
	
	tam=strlen(ai_buffer);
	// Redimensionamos el Buffer de Polaca si es necesario
	while ( (tam>=unsigned(m_iTamPolish-m_iPolishPos)) && (ret == M4_SUCCESS) )  {
		ret = ResizePolish();
	}
	if (ret==M4_SUCCESS) {
		// Concatenamos la Polaca, y ya sabemos a partir de donde
		memcpy( m_pszPolish+m_iPolishPos-1, ai_buffer, tam+1 );	// strcpy (con el 0 de la ultima cadena)
		m_iPolishPos+=tam;	//actualizamos la posición
		ret=M4_SUCCESS;
	}
	
	return ret;	

}

//********************************
//leer linea
//********************************

m4return_t ClTrad::ReadLine(m4char_t *ai_pszString,m4char_t *ao_pszLine, m4int32_t ai_iTam){
	
	m4char_t *pszTemp;
	m4int32_t iTam;
	
	pszTemp=ai_pszString;
	while (!M4CL_IS_EOL(pszTemp) && (*pszTemp!='\0') ) pszTemp++;
	
	iTam=min((m4int32_t)(pszTemp-ai_pszString), ai_iTam);
	strncpy(ao_pszLine,ai_pszString,iTam);
	ao_pszLine[iTam]='\0';
	
	return M4_SUCCESS;
}

//**********************
//version de polaca
//**********************

m4return_t ClTrad::SetPolishInfo(void){
	
	m4char_t pszLevel[10];
	
	sprintf(pszLevel,"V%d\n",m_iNumVar);
	
	return AddPolish(pszLevel);
}

//*********************
//GenerateBasicFC
//*********************
m4return_t  ClTrad::GenerateBasicFunctionCall (m4char_t *ai_pszString){
	
	m4return_t ret;
	m4uint16_t iId;
	
	ret=m_oFuncTable->GetId(ai_pszString,iId);
	if (ret!=M4_SUCCESS){		
		DUMP_CHLOG_ERRORF( M4_CH_VM_COMP_IE_NO_FUNCTION, ai_pszString );
        bCompileError=M4_TRUE;
		return M4_ERROR;	
	}
	sprintf(m_BufferTemp,"F%d\n",iId);
	
	return AddPolish(m_BufferTemp);
}


//*****************************

m4return_t ClTrad::BasicItem2String (m4ItemComp_t ai_oItem, m4pcchar_t ai_pcRealItem,m4char_t *ao_pszItem, m4int32_t ai_iLong ){
	
	ao_pszItem[0]='\0'; //para concatenar a gusto
	if (strcmp(ai_oItem.m_acChannel,"")!=0){
		strcat (ao_pszItem,"C");
		strcat (ao_pszItem,ai_oItem.m_acChannel);
		strcat (ao_pszItem," ");
	}
	
	if (strcmp(ai_oItem.m_acNode,"")!=0){
		strcat (ao_pszItem,"N");
		strcat (ao_pszItem,ai_oItem.m_acNode);
		strcat (ao_pszItem," ");
		
		//miramos si tiene row
		if (ai_oItem.m_bRow==M4_TRUE){
			strcat (ao_pszItem,"1 ");
		}
		else{
			strcat (ao_pszItem,"0 ");
		}
	}
	
	//item tiene que venir
	M4_ASSERT (ai_pcRealItem);
	strcat (ao_pszItem,"I");
	strcat (ao_pszItem,ai_pcRealItem);
	strcat (ao_pszItem," ");
	
	//miramos si tiene row
	if (ai_oItem.m_bTrame==M4_TRUE){
		strcat (ao_pszItem,"1");
	}
	else{
		strcat (ao_pszItem,"0");
	}
	return M4_SUCCESS;
	
}

//	DEPENDENCIAS




m4return_t	ClTrad::AddDepend(m4pcchar_t ai_pszItemId,m4ItemComp_t &ai_oItemId, m4DependentType ai_eDependentType)

{
	m4return_t ret=M4_SUCCESS;
	m4char_t	acType[3]; //para #, el digito y el terminador

	//Para compilación JIT no hay nodo de dependencias.
	if (m_poFinder->Is_TICH() != M4_TRUE)	return M4_SUCCESS;

	if (((ClFinder_TICH *)m_poFinder)->GetLastItemIs_Inherit()){
		// Es Item del sistema. No se añade como dependencia.
		return M4_SUCCESS;
	}

	if (strcmp(ai_pszItemId,"")!=0)	{
		if (strlen(ai_pszItemId)<M4CL_MAX_LEN_ITEM&&strlen(ai_oItemId.m_acNode)<M4CL_MAX_LEN_NODE){
			if (strcmp(ai_oItemId.m_acNode,"")==0) 	{
				m4char_t pszItem[M4CL_MAX_LEN_ITEM+3];
				strcpy(pszItem,ai_pszItemId);
				
				sprintf(acType,"#%d",ai_eDependentType);
				strcat(pszItem,acType);
				

				//FORMATO:	ITEM#0
				m_oInternalDepend.insert(pszItem);
			}
			else {
				
				m4pcchar_t pid_ti;
				m4pcchar_t pRealAlias;
				((ClFinder_TICH *)m_poFinder)->GetLastItemID_TI_USED(pid_ti);
				
				// Hay que rellenar un campo nuevo :Real_Alias
				// Si pid_ti es nulo, no se ha utilizado alias y : ALIAS = ID_TI = m_acNode; REAL_ALIAS=0.
				// Si no es nulo, se ha utilizado un alias : ALIAS = m_acNode;ID_TI = pid_ti;REAL_ALIAS = m_acNode.

				if (pid_ti==0){
					pRealAlias = 0;
					pid_ti=ai_oItemId.m_acNode;
				}
				else{
					pRealAlias=ai_oItemId.m_acNode;
				}

				if (strcmp(ai_oItemId.m_acChannel,"")==0){
					
					m4char_t pszTIAliasItem[M4CL_MAX_LEN_ITEM+M4CL_MAX_LEN_NODE+2*M4CL_MAX_LEN_ALIAS+6];//REAL_ALIAS#TI#ALIAS#ITEM#0

					/* Bug 0073357
					Si la TI es la misma hay que ponerlo como dependencia interna
					*/
					*pszTIAliasItem = '\0' ;

					if( strcmpi( m_acCurrentNode, pid_ti ) != 0 )
					{
						if (pRealAlias){
							strcat(pszTIAliasItem,pRealAlias);
						}

						strcat(pszTIAliasItem,"#");
						strcat(pszTIAliasItem,pid_ti);
						strcat(pszTIAliasItem,"#");
						strcat(pszTIAliasItem,ai_oItemId.m_acNode);
						strcat(pszTIAliasItem,"#");
					}

					strcat(pszTIAliasItem,ai_pszItemId);

					sprintf(acType,"#%d",ai_eDependentType);
					strcat(pszTIAliasItem, acType);
					

					if( strcmpi( m_acCurrentNode, pid_ti ) != 0 )
					{
						//FORMATO:	REAL_ALIAS#TI#ALIAS#ITEM#0
						m_oExternalDepend.insert(pszTIAliasItem);
					}
					else
					{
						//FORMATO:	ITEM#0
						m_oInternalDepend.insert(pszTIAliasItem);
					}
				}
				else{
					m4pcchar_t acT3;
					m4pcchar_t acNode_T3;
					((ClFinder_TICH *)m_poFinder)->GetLastItemID_T3_USED(acT3);
					((ClFinder_TICH *)m_poFinder)->GetLastItemID_NODE_T3_USED(acNode_T3);
					if (acT3==0)	acT3=ai_oItemId.m_acChannel;
					if (acNode_T3==0)	acNode_T3=acT3;
					m4char_t pszChannelTIItem  [M4CL_MAX_LEN_ITEM+M4CL_MAX_LEN_NODE+M4CL_MAX_LEN_CHANNEL+6];//CHANNEL#NODE#TI#ITEM#0
					
					strcpy(pszChannelTIItem,acT3);
					strcat(pszChannelTIItem,"#");
					strcat(pszChannelTIItem,acNode_T3);
					strcat(pszChannelTIItem,"#");
					strcat(pszChannelTIItem,ai_oItemId.m_acNode);
					strcat(pszChannelTIItem,"#");
					strcat(pszChannelTIItem,pid_ti);
					strcat(pszChannelTIItem,"#");
					strcat(pszChannelTIItem,ai_pszItemId);

					sprintf(acType,"#%d",ai_eDependentType);
					strcat(pszChannelTIItem, acType);
					
				
					//FORMATO:	CHANNEL#NODE_T3#NODE#TI#ITEM#0	(NODE_T3 se añade por la herencia.)
					m_oChannelDepend.insert(pszChannelTIItem);
				}
			}
		}
		else {
			DumpError(M4_CH_VM_COMP_IE_LONG_ID, "");
			ret=M4_ERROR;
		}
	}
	else {
		DumpError(M4_CH_VM_COMP_NULL_DEPENDENCY, "");
		ret=M4_ERROR;
	}
	
	return ret;
	
}


m4return_t ClTrad::UpdateInternalDepend(void)
{
	m4return_t ret=M4_SUCCESS;

	ClBlock_Channel_Proxy *poBlockDepend;
	
	m4uint32_t hInternalDep=0;
	//Para compilación JIT no hay nodo de dependencias.
	if (m_poFinder->Is_TICH() != M4_TRUE)	return M4_SUCCESS;
	
	hInternalDep=m_poFinder->GetAccess()->GetpCMCR()->GetChannelNodeHandleById(M4_NODE_SCHTIS_INTERNAL_DEP, M4_TRUE );
	if (hInternalDep==0){
		// No hay nodo de dependencias.
		return M4_SUCCESS;
	}
    ClRecordSet oRSIntDep (m_poFinder->GetAccess ());

	poBlockDepend=m_poFinder->m_poRecordSet->Register[m_poFinder->m_oRegIndex].GetChildBlock((ClHandle)hInternalDep);
	oRSIntDep.SetCurrentBlock(poBlockDepend);
	

	/*
	Optimización de las dependencias
	Se recorre el bloque de dependencias existentes. Para cada registro se concatena
	la clave que representa la dependencia dentro del set. Se busca esa dependencia
	en el set. Si existe se borra del set porque ya no hay que añadirla. Si no existe
	se marca el registro para borrar porque es una dependencia que ha dejado de serlo

	ret=oRSIntDep.Register.DeleteAll();
	*/
    m4uint16_t		i = 0 ;
    m4uint16_t		iLength = 0 ;
	string			sKey ;
    m4VariantType	vValue ;
	m4char_t		acBuffer[ 128 ] ;
    ClRegister		*poRegister = NULL ;

    iLength = m4uint16_t( oRSIntDep.Count() ) ;

	if( iLength > 0 )
	{
		sKey.reserve( M4CL_MAX_LEN_ITEM + 10 ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( oRSIntDep.Register[ i ] ) ;

			if( poRegister->Operation.IsDeleted() == M4_TRUE )
			{
				continue ;
			}

			sKey = "" ;

			ret = poRegister->Item[ M4_ITEM_SCHTIS_INT_DEP_ID_ITEM_USED ].Value.Get( vValue ) ;

			if( ret == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_STRING_VAR && vValue.Data.PointerChar != NULL )
			{
				sKey.append( vValue.Data.PointerChar ) ;
			}

			sKey.append( "#" ) ;

			ret = poRegister->Item[ M4_ITEM_SCHTIS_INT_DEP_TYPE ].Value.Get( vValue ) ;

			if( ret == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_NUMBER )
			{
				sprintf( acBuffer, "%.0f", vValue.Data.DoubleData ) ;
				sKey.append( acBuffer ) ;
			}

			itSetDepend iter = m_oInternalDepend.find( sKey.c_str() ) ;

			if( iter != m_oInternalDepend.end() )
			{
				m_oInternalDepend.erase( iter ) ;
			}
			else
			{
				poRegister->Delete() ;
			}
		}
	}

	
	if (m_oInternalDepend.size()>0){
		m4VariantType vValue;
		m4char_t pszTemp[M4CL_MAX_LEN_ITEM+3];
		
		itSetDepend iter=m_oInternalDepend.begin();

		while(iter!=m_oInternalDepend.end())
		{
			m4pchar_t pItem,pExecDep;
			ret=oRSIntDep.Register.Add();
			
			strcpy (pszTemp,(*iter).c_str());
			pItem = pszTemp;

			pExecDep=strchr(pItem,'#');
			*pExecDep=0;
			++pExecDep;

			vValue.Type = M4CL_CPP_TYPE_STRING_VAR;
			vValue.Data.PointerChar=(m4pchar_t)pItem;	

			
			if (ret==M4_SUCCESS){
				
				ClRegister * poRegister=&(oRSIntDep.Register);
				ret = poRegister->Item[M4_ITEM_SCHTIS_INT_DEP_ID_ITEM_USED].Value.Set(vValue);
				if (ret!=M4_ERROR){
					vValue.SetDouble(atof(pExecDep));
					ret = poRegister->Item[M4_ITEM_SCHTIS_INT_DEP_TYPE].Value.Set(vValue);

				}
				if (ret==M4_ERROR){
					DumpWarning(M4_CH_VM_COMP_CANT_UPDATE_DEPEND, pItem);
					ret=M4_SUCCESS;
				}
				
			}
			else{
				DumpError(M4_CH_VM_COMP_IE_CANT_ADD_REG_DEPEND, "");
				ret=M4_SUCCESS;
			}

			iter++;
		}
		
	}
	return ret;
}

m4return_t ClTrad::UpdateExternalDepend(void)
{
	m4return_t	ret=M4_SUCCESS;
	ClBlock_Channel_Proxy	*poBlockDepend;
	m4uint32_t hExternalDep = 0;
	
	//Para compilación JIT no hay nodo de dependencias.
	if (m_poFinder->Is_TICH() != M4_TRUE)	return M4_SUCCESS;
	hExternalDep=m_poFinder->GetAccess()->GetpCMCR()->GetChannelNodeHandleById(M4_NODE_SCHTIS_EXTERNAL_DEP, M4_TRUE );
	if (hExternalDep==0){
		// No tenemos nodo de dependencias.
		return M4_SUCCESS;
	}
	
    ClRecordSet	oRSExtDep (m_poFinder->GetAccess ());
	
	poBlockDepend=m_poFinder->m_poRecordSet->Register[m_poFinder->m_oRegIndex].GetChildBlock((ClHandle)hExternalDep);
	oRSExtDep.SetCurrentBlock(poBlockDepend);
	
	/*
	Optimización de las dependencias
	Se recorre el bloque de dependencias existentes. Para cada registro se concatena
	la clave que representa la dependencia dentro del set. Se busca esa dependencia
	en el set. Si existe se borra del set porque ya no hay que añadirla. Si no existe
	se marca el registro para borrar porque es una dependencia que ha dejado de serlo

	ret=oRSExtDep.Register.DeleteAll();
	*/
    m4uint16_t		i = 0 ;
    m4uint16_t		iLength = 0 ;
	string			sKey ;
    m4VariantType	vValue ;
	m4char_t		acBuffer[ 128 ] ;
    ClRegister		*poRegister = NULL ;

    iLength = m4uint16_t( oRSExtDep.Count() ) ;

	if( iLength > 0 )
	{
		sKey.reserve( M4CL_MAX_LEN_ALIAS + M4CL_MAX_LEN_NODE + M4CL_MAX_LEN_ALIAS + M4CL_MAX_LEN_ITEM + 10 ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( oRSExtDep.Register[ i ] ) ;

			if( poRegister->Operation.IsDeleted() == M4_TRUE )
			{
				continue ;
			}

			sKey = "" ;

			ret = poRegister->Item[ M4_ITEM_SCHTIS_EXT_DEP_REAL_ALIAS ].Value.Get( vValue ) ;

			if( ret == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_STRING_VAR && vValue.Data.PointerChar != NULL )
			{
				sKey.append( vValue.Data.PointerChar ) ;
			}

			sKey.append( "#" ) ;

			ret = poRegister->Item[ M4_ITEM_SCHTIS_EXT_DEP_ID_TI_USED ].Value.Get( vValue ) ;

			if( ret == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_STRING_VAR && vValue.Data.PointerChar != NULL )
			{
				sKey.append( vValue.Data.PointerChar ) ;
			}

			sKey.append( "#" ) ;

			ret = poRegister->Item[ M4_ITEM_SCHTIS_EXT_DEP_ALIAS ].Value.Get( vValue ) ;

			if( ret == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_STRING_VAR && vValue.Data.PointerChar != NULL )
			{
				sKey.append( vValue.Data.PointerChar ) ;
			}

			sKey.append( "#" ) ;

			ret = poRegister->Item[ M4_ITEM_SCHTIS_EXT_DEP_ID_ITEM_USED ].Value.Get( vValue ) ;

			if( ret == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_STRING_VAR && vValue.Data.PointerChar != NULL )
			{
				sKey.append( vValue.Data.PointerChar ) ;
			}

			sKey.append( "#" ) ;

			ret = poRegister->Item[ M4_ITEM_SCHTIS_EXT_DEP_TYPE ].Value.Get( vValue ) ;

			if( ret == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_NUMBER )
			{
				sprintf( acBuffer, "%.0f", vValue.Data.DoubleData ) ;
				sKey.append( acBuffer ) ;
			}

			itSetDepend iter = m_oExternalDepend.find( sKey.c_str() ) ;

			if( iter != m_oExternalDepend.end() )
			{
				m_oExternalDepend.erase( iter ) ;
			}
			else
			{
				poRegister->Delete() ;
			}
		}
	}


	if (m_oExternalDepend.size()>0){
		m4VariantType vValue;
		
		m4char_t psztemp[M4CL_MAX_LEN_ITEM+M4CL_MAX_LEN_NODE+2*M4CL_MAX_LEN_ALIAS+6];

		itSetDepend iter=m_oExternalDepend.begin();

		while(iter!=m_oExternalDepend.end())
		{
			ret=oRSExtDep.Register.Add();
			
			
			m4pchar_t pRealAlias,pTI,pAlias,pItem,pExecDep;
			
			strcpy(psztemp,(*iter).c_str());

			pRealAlias = psztemp;

			pTI = strchr(pRealAlias,'#');
			M4_ASSERT (pTI);
			*pTI=0;
			++pTI;

			pAlias=strchr(pTI,'#');
			M4_ASSERT (pAlias);
			*pAlias=0;
			++pAlias;

			pItem = strchr(pAlias,'#');
			M4_ASSERT (pItem);
			*pItem=0;
			++pItem;

			pExecDep=strchr(pItem,'#');
			M4_ASSERT(pExecDep);
			*pExecDep=0;
			++pExecDep;

			
			
			if (ret==M4_SUCCESS){
				
				ClRegister	*poRegister=&(oRSExtDep.Register);
				vValue.Type=M4CL_CPP_TYPE_STRING_VAR;
				vValue.Data.PointerChar=pItem;
				ret=poRegister->Item[M4_ITEM_SCHTIS_EXT_DEP_ID_ITEM_USED].Value.Set(vValue);
				if (ret!=M4_ERROR){
				
					poRegister->Item.MoveTo(M4_ITEM_SCHTIS_EXT_DEP_REAL_ALIAS);
					if (poRegister->Item.GetIndex() != M4DM_EOF_INDEX) {
						// Existe el item.
						if (*pRealAlias!=0){
							vValue.Data.PointerChar=pRealAlias;
							ret = poRegister->Item.Value.Set(vValue);
						}
						else{
							ret = poRegister->Item.Value.SetNULL();
						}
					}
				
					if (ret!=M4_ERROR){
						vValue.Data.PointerChar=pTI;
						ret=poRegister->Item[M4_ITEM_SCHTIS_EXT_DEP_ID_TI_USED].Value.Set(vValue);
						if (ret!=M4_ERROR){	//&&strcmp(acTI,acAlias)
							vValue.Data.PointerChar=pAlias;
							ret=poRegister->Item[M4_ITEM_SCHTIS_EXT_DEP_ALIAS].Value.Set(vValue);
							if (ret!=M4_ERROR){
								vValue.SetDouble(atof(pExecDep));
								ret = poRegister->Item[M4_ITEM_SCHTIS_EXT_DEP_TYPE].Value.Set(vValue);

							}
						}
						
					}
					if (ret==M4_ERROR){
						DumpWarning(M4_CH_VM_COMP_CANT_UPDATE_DEPEND, pItem);
						ret=M4_SUCCESS;
					}
				}
			}
			else{
				DumpError(M4_CH_VM_COMP_IE_CANT_ADD_REG_DEPEND, "");
				ret=M4_SUCCESS;
			}

			iter++;			
		}
		
	}
	return ret;
}	

m4return_t ClTrad::UpdateChannelDepend(void)
{
	m4return_t	ret=M4_SUCCESS;
	ClBlock_Channel_Proxy	*poBlockDepend;
	m4uint32_t hChannelDepend =0;
	
	//Para compilación JIT no hay nodo de dependencias.
	if (m_poFinder->Is_TICH() != M4_TRUE)	return M4_SUCCESS;

	hChannelDepend = m_poFinder->GetAccess()->GetpCMCR()->GetChannelNodeHandleById(M4_NODE_SCHTIS_CHANNEL_DEP);
	if (hChannelDepend==0){
		// No tenemos nodo de dependencias.
		return M4_SUCCESS;
	}
	
    ClRecordSet	oRSChannelDep (m_poFinder->GetAccess ());
	poBlockDepend=m_poFinder->m_poRecordSet->Register[m_poFinder->m_oRegIndex].GetChildBlock((ClHandle)hChannelDepend);
	oRSChannelDep.SetCurrentBlock(poBlockDepend);
	
	/*
	Optimización de las dependencias
	Se recorre el bloque de dependencias existentes. Para cada registro se concatena
	la clave que representa la dependencia dentro del set. Se busca esa dependencia
	en el set. Si existe se borra del set porque ya no hay que añadirla. Si no existe
	se marca el registro para borrar porque es una dependencia que ha dejado de serlo

	ret=oRSChannelDep.Register.DeleteAll();
	*/
    m4uint16_t		i = 0 ;
    m4uint16_t		iLength = 0 ;
	string			sKey ;
    m4VariantType	vValue ;
	m4char_t		acBuffer[ 128 ] ;
    ClRegister		*poRegister = NULL ;

    iLength = m4uint16_t( oRSChannelDep.Count() ) ;

	if( iLength > 0 )
	{
		sKey.reserve( M4CL_MAX_LEN_ALIAS + M4CL_MAX_LEN_CHANNEL + M4CL_MAX_LEN_NODE + M4CL_MAX_LEN_NODE + M4CL_MAX_LEN_ITEM + 10 ) ;

		for( i = 0 ; i < iLength ; i++ )
		{
			poRegister = &( oRSChannelDep.Register[ i ] ) ;

			if( poRegister->Operation.IsDeleted() == M4_TRUE )
			{
				continue ;
			}

			sKey = "" ;

			ret = poRegister->Item[ M4_ITEM_SCHTIS_CHANNEL_DEP_ID_T3_USED ].Value.Get( vValue ) ;

			if( ret == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_STRING_VAR && vValue.Data.PointerChar != NULL )
			{
				sKey.append( vValue.Data.PointerChar ) ;
			}

			sKey.append( "#" ) ;

			ret = poRegister->Item[ M4_ITEM_SCHTIS_CHANNEL_DEP_ID_NODE_T3_USED ].Value.Get( vValue ) ;

			if( ret == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_STRING_VAR && vValue.Data.PointerChar != NULL )
			{
				sKey.append( vValue.Data.PointerChar ) ;
			}

			sKey.append( "#" ) ;

			ret = poRegister->Item[ M4_ITEM_SCHTIS_CHANNEL_DEP_ID_NODE_USED ].Value.Get( vValue ) ;

			if( ret == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_STRING_VAR && vValue.Data.PointerChar != NULL )
			{
				sKey.append( vValue.Data.PointerChar ) ;
			}

			sKey.append( "#" ) ;

			ret = poRegister->Item[ M4_ITEM_SCHTIS_CHANNEL_DEP_ID_TI_USED ].Value.Get( vValue ) ;

			if( ret == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_STRING_VAR && vValue.Data.PointerChar != NULL )
			{
				sKey.append( vValue.Data.PointerChar ) ;
			}

			sKey.append( "#" ) ;

			ret = poRegister->Item[ M4_ITEM_SCHTIS_CHANNEL_DEP_ID_ITEM_USED ].Value.Get( vValue ) ;

			if( ret == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_STRING_VAR && vValue.Data.PointerChar != NULL )
			{
				sKey.append( vValue.Data.PointerChar ) ;
			}

			sKey.append( "#" ) ;

			ret = poRegister->Item[ M4_ITEM_SCHTIS_CHANN_DEP_TYPE ].Value.Get( vValue ) ;

			if( ret == M4_SUCCESS && vValue.Type == M4CL_CPP_TYPE_NUMBER )
			{
				sprintf( acBuffer, "%.0f", vValue.Data.DoubleData ) ;
				sKey.append( acBuffer ) ;
			}

			itSetDepend iter = m_oChannelDepend.find( sKey.c_str() ) ;

			if( iter != m_oChannelDepend.end() )
			{
				m_oChannelDepend.erase( iter ) ;
			}
			else
			{
				poRegister->Delete() ;
			}
		}
	}


	if (m_oChannelDepend.size()>0){
		m4VariantType vValue;
		m4char_t psztemp[M4CL_MAX_LEN_ITEM+M4CL_MAX_LEN_NODE+M4CL_MAX_LEN_ALIAS+6];		

		itSetDepend iter=m_oChannelDepend.begin();

		while(iter!=m_oChannelDepend.end())
		{
			ret=oRSChannelDep.Register.Add();
			
			m4pchar_t pChannel,pNode_T3,pTI,pNode,pItem,pExecDep;

			//FORMATO:	CHANNEL#NODE_T3#NODE#TI#ITEM#0	(NODE_T3 se añade por la herencia.)
			strcpy(psztemp,(*iter).c_str());
			pChannel = psztemp;


			pNode_T3=strchr(pChannel,'#');
			*pNode_T3=0;
			++pNode_T3;

			pNode=strchr(pNode_T3,'#');
			*pNode=0;
			++pNode;
				
			pTI=strchr(pNode,'#');
			*pTI=0;
			++pTI;

			pItem=strchr(pTI,'#');
			*pItem=0;
			++pItem;

			pExecDep=strchr(pItem,'#');
			M4_ASSERT(pExecDep);
			*pExecDep=0;
			++pExecDep;

			if (ret==M4_SUCCESS){
				
				ClRegister	*poRegister=&(oRSChannelDep.Register);
				vValue.Type=M4CL_CPP_TYPE_STRING_VAR;
				vValue.Data.PointerChar=pItem;
				ret=poRegister->Item[M4_ITEM_SCHTIS_CHANNEL_DEP_ID_ITEM_USED].Value.Set(vValue);
				
				if (ret!=M4_ERROR){
					vValue.Data.PointerChar=pChannel;
					ret=poRegister->Item[M4_ITEM_SCHTIS_CHANNEL_DEP_ID_T3_USED].Value.Set(vValue);
					if (ret!=M4_ERROR){		//&&strcmp(acTI,acAlias)
						vValue.Data.PointerChar=pTI;
						ret=poRegister->Item[M4_ITEM_SCHTIS_CHANNEL_DEP_ID_TI_USED].Value.Set(vValue);
						if (ret!=M4_ERROR){
							vValue.Data.PointerChar=pNode;
							ret=poRegister->Item[M4_ITEM_SCHTIS_CHANNEL_DEP_ID_NODE_USED].Value.Set(vValue);
							if (ret!=M4_ERROR){
								vValue.Data.PointerChar=pNode_T3;
								ret=poRegister->Item[M4_ITEM_SCHTIS_CHANNEL_DEP_ID_NODE_T3_USED].Value.Set(vValue);
								if (ret!=M4_ERROR){
									vValue.SetDouble(atof(pExecDep));
									ret = poRegister->Item[M4_ITEM_SCHTIS_CHANN_DEP_TYPE].Value.Set(vValue);
								}
							}
						}
					}
				}
				if (ret==M4_ERROR){
					DumpWarning(M4_CH_VM_COMP_CANT_UPDATE_DEPEND, pItem);
					ret=M4_SUCCESS;
				}
			}
			else{
				DumpError(M4_CH_VM_COMP_IE_CANT_ADD_REG_DEPEND, "");
				ret=M4_SUCCESS;
			}

			iter++;
		}
	}
	return ret;
}

m4return_t ClTrad::UpdateRealSynonym(m4pcchar_t ai_acPosSynon,m4pcchar_t ai_acSynon)
{
	
	m4return_t ret=M4_SUCCESS;
	itMapSynonym Iter;
	m4bool_t	bFound=M4_FALSE;
	m4int32_t	iExtra;
	
	Iter=m_oSynonymMap.end();
	
	while(Iter!=m_oSynonymMap.begin()&&bFound!=M4_TRUE){
		Iter--;
		if ((*Iter).second.m_bToChange==M4_FALSE && stricmp((*Iter).second.m_acOriginal,ai_acPosSynon)==0){
			strcpy((*Iter).second.m_acSynonym,ai_acSynon);
			(*Iter).second.m_bToChange=M4_TRUE;
			bFound=M4_TRUE;
			iExtra=strlen(ai_acSynon)-strlen(ai_acPosSynon);
			m_iExtraMemorySynonym+=((iExtra>0)?iExtra:0);
			m_bSynonymFound = M4_TRUE ;
		}
		
	}
	if (bFound!=M4_TRUE){
		//GRAVE ERROR;		
		//		  DUMP_CHLOG_ERROR( M4_CH_VM_COMP_UPDATE_SYNONYMS??? );
		return M4_ERROR;
	}
	
	return ret;
	
}
m4return_t ClTrad::CheckToUpdateSynonyms(m4pchar_t ai_InCode,m4bool_t ai_bSetItemHasSynonym)
{
	if (SynonymTreatment()==M4_FALSE){
		return M4_SUCCESS;
	}

	if (ai_bSetItemHasSynonym){
		/* Bug 0084826 No se debe modificar el estado */
		m_poFinder->SetValue( M4_ITEM_SCHTIS_RULES_HAS_SYNONYM, (m_bSynonymFound ? 1.0 : 0.0), M4_FALSE );
	}

	if (m_bSynonymFound){
		m4uint32_t iLongInCode = strlen(ai_InCode)+1;
		m_pszLN4FinalCode = new m4char_t [iLongInCode+(m_iExtraMemorySynonym)];		
		memcpy(m_pszLN4FinalCode,ai_InCode,iLongInCode) ;
		return UpdateSynoynms (m_pszLN4FinalCode) ;
	}
	else{
		m_pszLN4FinalCode = 0;
	}

	return M4_SUCCESS;
}

m4return_t ClTrad::UpdateSynoynms (m4pchar_t ai_pszSource)
{	
	m4return_t ret=M4_SUCCESS;
	
	itMapSynonym Iter;
	
	Iter=m_oSynonymMap.end();
	
	while (Iter!=m_oSynonymMap.begin()){
		Iter--;
		if ((*Iter).second.m_bToChange==M4_TRUE){
			ChangeToSynonym(ai_pszSource,
				(*Iter).first,
				(*Iter).second.m_acSynonym,
				(*Iter).second.m_iOffsetEnd);
		}
	}
	
	return ret;
}



m4return_t ClTrad::ChangeToSynonym(m4pchar_t ai_pszSource,m4int32_t ai_iOffset,m4pchar_t ai_pszSynonym,m4int32_t ai_iOffsetEnd)
{
	m4pchar_t pszBack=new m4char_t[strlen(ai_pszSource) + M4CL_MAX_LEN_ITEM*1];
	m4return_t ret=M4_SUCCESS;
	
	if (pszBack) {
		strncpy(pszBack,ai_pszSource,ai_iOffset);
		pszBack[ai_iOffset]='\x0';
		strcat(pszBack,ai_pszSynonym);
		strcat(pszBack,ai_pszSource+ai_iOffsetEnd);
		strcpy(ai_pszSource,pszBack);
		
		delete [] pszBack;
	} 
	else {
		DumpError(M4_CH_VM_COMP_IE_NOT_MEMORY, "");
		ret = M4_ERROR;
	}
	
	return ret;
}

m4return_t ClTrad::AddItemSynonym(ClRegister *oRegItem,m4pchar_t ai_pcItem)
{
	m4char_t acSynonym[M4CL_MAX_LEN_ITEM+1],acRealItem[M4CL_MAX_LEN_ITEM+1];
	m4VariantType vValue;
	m4return_t retSyn;
	m4double_t *pdType;

					
	//Para compilación JIT no se cambian los sinónimos.
	if (SynonymTreatment()==M4_FALSE){
		// No hay que hacer traducción.
		return M4_SUCCESS;
	}

	if (m_eSourceTo==M4_OLD_TI_TO_ID_TI||m_eSourceTo==M4_TI_BASE_TO_INHERITED){
		return M4_SUCCESS;
	}

	strcpy(acRealItem,(m4pcchar_t) oRegItem->Item[M4_ITEM_SCHTIS_ITEMS_ID_ITEM].Value.Get());
    pdType = ( ( m4double_t * ) oRegItem->Item[M4_ITEM_SCHTIS_ITEMS_ID_ITEM_TYPE].Value.Get() );
	if (pdType!=0){
		switch(m_eSourceTo)	{
		case M4_SOURCE_TO_ID:
		case M4_OLD_ITEM_TO_ID_ITEM:
			UpdateRealSynonym(ai_pcItem,acRealItem);
			break;
		case M4_RESTORE_SOURCE:
			if (*ai_pcItem=='@'){
				retSyn =oRegItem->Item[M4_ITEM_SCHTIS_ITEMS_N_SYNONYM].Value.Get(vValue);
				if (vValue.Type==M4CL_CPP_TYPE_NULL||retSyn!=M4_SUCCESS){
					//EN ESTE CASO SI QUE ES ERROR;
					m_bSynonymError=M4_TRUE;
					
					break;
				}
				strcpy(acSynonym,"@");
				strcat(acSynonym,vValue.Data.PointerChar);
				UpdateRealSynonym(ai_pcItem,acSynonym);
				break;
			}
			else if (*ai_pcItem=='#'){
				retSyn =oRegItem->Item[M4_ITEM_SCHTIS_ITEMS_ID_SYNONYM].Value.Get(vValue);
				if (vValue.Type==M4CL_CPP_TYPE_NULL||retSyn!=M4_SUCCESS){
					//ERROR
					m_bSynonymError=M4_TRUE;
					
					break;
				}
				/* Bug 0089065
				Con un 0 sale el buffer vacío
				mejor manera de no complicarse la vida es hacer un sprintf
				*/
				sprintf( acSynonym, "#%.0lf", vValue.Data.DoubleData ) ;

				UpdateRealSynonym(ai_pcItem,acSynonym);
				break;
			}
			else {
				UpdateRealSynonym(ai_pcItem,acRealItem);
				break;
			}
			
		case M4_SOURCE_TO_NUMBER:
			retSyn =oRegItem->Item[M4_ITEM_SCHTIS_ITEMS_ID_SYNONYM].Value.Get(vValue);
			if (vValue.Type==M4CL_CPP_TYPE_NULL||retSyn!=M4_SUCCESS){
				//	m_bSynonymError=M4_TRUE;
				//En caso de que no exista sinonimo, no se sustituye y ya esta.
				break;
			}
			/* Bug 0089065
			Con un 0 sale el buffer vacío
			mejor manera de no complicarse la vida es hacer un sprintf
			*/
			sprintf( acSynonym, "#%.0lf", vValue.Data.DoubleData ) ;

			UpdateRealSynonym(ai_pcItem,acSynonym);
			break;
			
		case M4_SOURCE_TO_STRING:
			
			retSyn =oRegItem->Item[M4_ITEM_SCHTIS_ITEMS_N_SYNONYM].Value.Get(vValue);
			if (vValue.Type==M4CL_CPP_TYPE_NULL||retSyn!=M4_SUCCESS){
				//	m_bSynonymError=M4_TRUE;
				//En caso de que no exista sinonimo, no se sustituye y ya esta.
				break;
			}
			strcpy(acSynonym,"@");
			strcat(acSynonym,vValue.Data.PointerChar);
			UpdateRealSynonym(ai_pcItem,acSynonym);
			break;

		case M4_SOURCE_SAVE:
			
			if (stricmp(ai_pcItem,acRealItem)!=0){
				acSynonym[0]=ai_pcItem[0];
				acSynonym[1]='\x0';
				strcat(acSynonym,acRealItem);
				UpdateRealSynonym(ai_pcItem,acSynonym);
			}
			break;
			
		}
		
		
	}
	return M4_SUCCESS;
}

void ClTrad::SetSourceMode(Compile_Type  ai_cSourceMode){
	m_eSourceTo=ai_cSourceMode;
}


m4return_t ClTrad::SetAccess (ClAccess * ai_poAccess, m4uint32_t ai_hItem, m4uint8_t ai_iRuleLevel ){
	return m_poFinder->SetAccess(ai_poAccess, ai_hItem, ai_iRuleLevel);
}
void ClTrad::Reset(){
	m_poFinder->Reset();
}

//establece el RS. Debe tener un bloque "atachado"
void ClTrad::SetRecordSet ( ClAccessRecordSet * ai_poRecordSet ){
	m_poFinder->m_poRecordSet = ai_poRecordSet;
}

void ClTrad::SetRegIndex ( ClRegisterIndex ai_oRegIndex ){
	m_poFinder->m_oRegIndex = ai_oRegIndex;
}

Compile_Type  ClTrad::GetSourceMode(void){
	return m_eSourceTo;
}

void ClTrad::SetSymbolsTableSpace(m4char_t **ai_ppszSymbols, m4uint32_t ai_iSymbolsTableSize,m4uint32_t ai_iSymbolSize ){
	m_ppszSymbols		= ai_ppszSymbols;
	m_iSymbolsTableSize	= ai_iSymbolsTableSize;
	m_iSymbolSize		= ai_iSymbolSize;
}

//**************************
//Set Error
//**************************
void ClTrad::DumpError (m4int32_t ai_iNumError, m4pcchar_t ai_ErrorString1, m4pcchar_t ai_ErrorString2) {

	if( ai_ErrorString1 == NULL )
	{
		DUMP_CHLOG_ERRORF( ai_iNumError, (m_iNumLine+1) << m_acCurrentRule << m_acCurrentItem << m_acCurrentNode << m_acCurrentChannel << M4LOGSYS_EXT_ERROR_CODE_LINE << m_iNumLine+1 );
	}
	else
	{
		if( ai_ErrorString2 == NULL )
		{
			DUMP_CHLOG_ERRORF( ai_iNumError, (m_iNumLine+1) << m_acCurrentRule << m_acCurrentItem << m_acCurrentNode << m_acCurrentChannel << ai_ErrorString1 << M4LOGSYS_EXT_ERROR_CODE_LINE << m_iNumLine+1 );
		}
		else
		{
			DUMP_CHLOG_ERRORF( ai_iNumError, (m_iNumLine+1) << m_acCurrentRule << m_acCurrentItem << m_acCurrentNode << m_acCurrentChannel << ai_ErrorString1 << ai_ErrorString2 << M4LOGSYS_EXT_ERROR_CODE_LINE << m_iNumLine+1 );
		}
	}

	bCompileError=M4_TRUE;
}

void ClTrad::DumpWarning (m4int32_t ai_iNumError, m4pcchar_t ai_ErrorString1, m4pcchar_t ai_ErrorString2) {
	
	if( ai_ErrorString1 == NULL )
	{
		DUMP_CHLOG_WARNINGF( ai_iNumError, (m_iNumLine+1) << m_acCurrentRule << m_acCurrentItem << m_acCurrentNode << m_acCurrentChannel << M4LOGSYS_EXT_ERROR_CODE_LINE << m_iNumLine+1 );
	}
	else
	{
		if( ai_ErrorString2 == NULL )
		{
			DUMP_CHLOG_WARNINGF( ai_iNumError, (m_iNumLine+1) << m_acCurrentRule << m_acCurrentItem << m_acCurrentNode << m_acCurrentChannel << ai_ErrorString1 << M4LOGSYS_EXT_ERROR_CODE_LINE << m_iNumLine+1 );
		}
		else
		{
			DUMP_CHLOG_WARNINGF( ai_iNumError, (m_iNumLine+1) << m_acCurrentRule << m_acCurrentItem << m_acCurrentNode << m_acCurrentChannel << ai_ErrorString1 << ai_ErrorString2 << M4LOGSYS_EXT_ERROR_CODE_LINE << m_iNumLine+1 );
		}
	}

	bCompileError=M4_TRUE;
}


//**************************
//GetNumLineParser - Devuelve no. de linea del Parser
//**************************
m4int32_t ClTrad::GetNumLineParser (void) {
	return m_iNumLineParser;
}

//**************************
//GetNumLine
//**************************
m4int32_t ClTrad::GetNumLine (void) {
	return m_iNumLine;
}

//**************************
//SynonymTreatment - Indica si tenemos que traducir sinonimos.
//**************************
m4bool_t ClTrad::SynonymTreatment(void){
	// Si estamos traduciendo sinonimos. Si no es JIT y no viene de GetPolish o CheckSyntax o CompileRule
	return M4_BOOL(m_poFinder->Is_TICH()&&m_eSourceTo!=M4_NO_SOURCE&&m_eSourceTo!=M4_GET_POLISH);
}
//**************************
//PopIdentifier - Saca un identificador de la pila de sinonimos.
//**************************
m4return_t ClTrad::PopIdentifier(void){
	if (SynonymTreatment()==M4_FALSE){
		// No hay que hacer traducción.
		return M4_SUCCESS;
	}
	
	m_oSynonymStack.pop();

	return M4_SUCCESS;
}
//**************************
//PushIdentifier - Mete un identificador en la pila de sinonimos.
//**************************
void	ClTrad::PushIdentifier(m4pchar_t ai_pOriginal,m4int32_t ai_iOffset,m4int32_t ai_ilen){
	if (SynonymTreatment()==M4_FALSE){
		// No hay que hacer traducción.
		return;
	}
	StSynonym Synonym;

	strncpy(Synonym.m_acOriginal,ai_pOriginal,M4CL_MAX_LEN_ITEM);
	Synonym.m_acOriginal[M4CL_MAX_LEN_ITEM]=0;
	
	Synonym.m_bToChange=M4_FALSE;
	Synonym.m_iOffsetEnd=m_iLenLN4-ai_iOffset-1;
	Synonym.m_iOffset=m_iLenLN4-ai_iOffset-ai_ilen-1;
	
	m_oSynonymStack.push(Synonym);
}
//Item->Synonym
//**************************
//NotifyPosibleItem -	Indica que hemos encontrado o bien un item o una variable. ( Lo sacamos de la pila y lo metemos en el mapa )
//**************************
m4return_t		ClTrad::NotifyPosibleItem(void){	// Puede ser item o variable.
	if (SynonymTreatment()==M4_FALSE){
		// No hay que hacer traducción.
		return M4_SUCCESS;
	}

	if (m_eSourceTo!=M4_OLD_TI_TO_ID_TI&&m_eSourceTo!=M4_TI_BASE_TO_INHERITED){
		// Si estamos traduciendo Items lo movemos de la pila al mapa.
		m_oSynonymMap.insert(MapSynonym::value_type(m_oSynonymStack.top().m_iOffset,m_oSynonymStack.top()));
	}

	// En cualquier caso lo sacamos de la pila.
	m_oSynonymStack.pop();
	return M4_SUCCESS;
}
//**************************
//DeleteItemSynonym - Indica que borremos el ultimo item que hayamos añadido.
//						Es para poder distinguir entre variables y metodos que se llamen igual.
//**************************
void	ClTrad::DeleteItemSynonym(m4pchar_t ai_pcItem,m4bool_t ai_bToChange){
	// 
	if (SynonymTreatment()==M4_FALSE){
		// No hay que hacer traducción.
		return;
	}
	if (m_eSourceTo==M4_OLD_TI_TO_ID_TI||m_eSourceTo==M4_TI_BASE_TO_INHERITED){
		return;
	}
	
	_DeleteLastSynonym(ai_pcItem,ai_bToChange);
}

// Ti_Alias -->id_ti
//**************************
//NotifyPosibleTI - Indica que hemos encontrado una TI.( Lo sacamos de la pila y lo metemos en el mapa )
//**************************
m4return_t		ClTrad::NotifyPosibleTI(void){
	if (SynonymTreatment()==M4_FALSE){
		// No hay que hacer traducción.
		return M4_SUCCESS;
	}
	if (m_eSourceTo==M4_OLD_TI_TO_ID_TI||m_eSourceTo==M4_TI_BASE_TO_INHERITED){
		// Si estamos traduciendo TIs lo movemos de la pila al mapa.
		m_oSynonymMap.insert(MapSynonym::value_type(m_oSynonymStack.top().m_iOffset,m_oSynonymStack.top()));
	}

	// En cualquier caso lo sacamos de la pila.
	m_oSynonymStack.pop();
	return M4_SUCCESS;
}

//**************************
//AddTISynonym - Verifica que lo que habia era una TI y actualiza el Old_Ti.
//**************************
m4return_t ClTrad::AddTISynonym(m4pcchar_t ai_pAlias,m4pcchar_t ai_pIdTI)
{

	if (m_eSourceTo!=M4_OLD_TI_TO_ID_TI&&m_eSourceTo!=M4_TI_BASE_TO_INHERITED){
		return M4_SUCCESS;
	}

	return UpdateRealSynonym(ai_pAlias,ai_pIdTI);
}

//**************************
//_DeleteLastSynonym - Borra el ultimo elemento encontrado en el mapa.
//**************************
void	ClTrad::_DeleteLastSynonym(m4pcchar_t ai_pitem,m4bool_t ai_bToChange){
	// Iteramos empezando por el final, hasta que encontremos el "sinonimo" en cuestion.
	itMapSynonym it = m_oSynonymMap.end();
	
	while (it !=m_oSynonymMap.begin()){
		it--;
		if ((*it).second.m_bToChange==ai_bToChange &&
			stricmp((*it).second.m_acOriginal,ai_pitem)==0){
			// Lo quitamos.
			m_oSynonymMap.erase(it);
			break;
		}
	}
}

// En AIX, es necesario definir los operadores :
//**************************
//StSynonym::operator == 
//**************************
m4bool_t StSynonym::operator == (const StSynonym &ai_left)const{
	if (m_iOffset != ai_left.m_iOffset){
		return M4_FALSE;
	}
	if (m_iOffsetEnd!= ai_left.m_iOffsetEnd){
		return M4_FALSE;
	}
	if (m_bToChange!= ai_left.m_bToChange){
		return M4_FALSE;
	}
	if (strcmp(m_acOriginal,ai_left.m_acOriginal)){
		return M4_FALSE;
	}
	if (strcmp(m_acSynonym,ai_left.m_acSynonym)){
		return M4_FALSE;
	}
	return M4_TRUE;
}
//**************************
//StSynonym::operator < 
//**************************
m4bool_t StSynonym::operator < (const StSynonym &ai_left)const{
	if (m_iOffset < ai_left.m_iOffset){
		return M4_TRUE;
	}
	if (m_iOffset != ai_left.m_iOffset){
		return M4_FALSE;
	}

	if (m_iOffsetEnd < ai_left.m_iOffsetEnd){
		return M4_TRUE;
	}
	if (m_iOffsetEnd != ai_left.m_iOffsetEnd){
		return M4_FALSE;
	}

	if (m_bToChange < ai_left.m_bToChange){
		return M4_TRUE;
	}
	if (m_bToChange != ai_left.m_bToChange){
		return M4_FALSE;
	}

	if (strcmp(m_acOriginal,ai_left.m_acOriginal)<0){
		return M4_TRUE;
	}
	if (strcmp(m_acOriginal,ai_left.m_acOriginal)){
		return M4_FALSE;
	}

	if (strcmp(m_acSynonym,ai_left.m_acSynonym)<0){
		return M4_TRUE;
	}
	return M4_FALSE;
}
 
//------------------------
//SetCurrentRuleInfo
//------------------------
m4return_t 	ClTrad::SetCurrentRuleInfo(m4bool_t ai_bTakeInfoFromTiChannel, const m4char_t *ai_pszChannel/*=0*/, const m4char_t *ai_pszTi/*=0*/,const m4char_t *ai_pszItem/*=0*/, const m4char_t *ai_pszRule/*=0*/){
	if (ai_bTakeInfoFromTiChannel){
		return	((ClFinder_TICH *)m_poFinder)->GetCurrentRuleInfo (m_acCurrentChannel, M4CL_MAX_LEN_CHANNEL, m_acCurrentNode, M4CL_MAX_LEN_NODE, m_acCurrentItem, M4CL_MAX_LEN_ITEM, m_acCurrentRule, M4CL_MAX_LEN_RULE);
	}

	//si nos pasan la info (es JIT
	strncpy(m_acCurrentItem,ai_pszItem,M4CL_MAX_LEN_ITEM);
	m_acCurrentItem[M4CL_MAX_LEN_ITEM]=0;
	
	strncpy(m_acCurrentNode,ai_pszTi,M4CL_MAX_LEN_NODE);
	m_acCurrentNode[M4CL_MAX_LEN_NODE]=0;
	
	strncpy(m_acCurrentChannel,ai_pszChannel,M4CL_MAX_LEN_CHANNEL);
	m_acCurrentChannel[M4CL_MAX_LEN_CHANNEL]=0;
	
	strncpy(m_acCurrentRule,ai_pszRule,M4CL_MAX_LEN_RULE);
	m_acCurrentRule[M4CL_MAX_LEN_RULE]=0;
	
	return M4_SUCCESS;
}
