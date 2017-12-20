//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             metachn.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/31/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//
// Definition:
//
//
//==============================================================================

#ifndef __METACHN__H__
#define __METACHN__H__

// VER SI HACEMOS ESTE MODULO GENERICO.

//=========================================================================

#include "xapichn.h"


//============================================================================ClMIT_MetaChn


class ClMIT_MetaChn : public ClMIT_Chn
{

MIT_PUBLIC:

	enum METACHN_NODES {  
		SYS_T3S,
		SYS_NODES,
		SYS_CONNECTORS,
		SYS_CONNECTOR_ITEM,
		SYS_CONNECTOR_ARGS,
		SYS_T3_CONNECTORS,
		SYS_T3_CONNECTOR_ITEM,
		SYS_TIS,
		SYS_ITEMS,
		SYS_ITEM_CSDESC,
		SYS_ITEM_ARGS,
		SYS_RULES,
		SYS_SENTENCES,
		SYS_RSMS,
		SYS_ALIAS_RES,
		SYS_T3_ALIAS_RES,
		SYS_INDEXES,
		SYS_INDEX_ITEMS,
		SYS_FILTERS,
		SYS_ITEM_TRIGGERS,
		NODE_COUNT
	} ;
	ClMIT_ChnNode *m_poNode[NODE_COUNT] ;

MIT_PRIVATE:

	static const m4char_t * m_pcNodeNames[NODE_COUNT] ;	//Indice METACHN_NODES

	//Indice METACHN_NODES. Lo rellena las llamadas a InitItemArray

	struct StNodeItemInfo {
		ClMIT_ChnItem *m_poItemArr ;
		m4int16_t      m_szItemArry ;
	} m_pstNodeItemInfo[NODE_COUNT] ;	
	
	struct StIndexName {		//Para poder reordenar la lista de indices (ver luego)
		m4uint32_t m_Id ;
		const m4char_t *m_pcName ;
	} ;

	m4int16_t m_iLanguage ;	//defines en MVC

MIT_PUBLIC:


	// ----------------------------- Inicialización---------------------------------------	

	ClMIT_MetaChn();
	virtual ~ClMIT_MetaChn() { ClMIT_MetaChn::End(); }
	virtual size_t GetSizeof() { return sizeof(ClMIT_MetaChn) ; }

	virtual void End();

	// True si ejecucion tiene exito.  Language hace falta para formar nombres de items 
	//	traducibles.
	// ai_poAccess solo puede ser NULL para un OWN_METACHN mode.

	enum METACHN_MODE { OWN_METACHN, COMMON_ACCESS, PRIVATE_ACCESS  } ;
	m4bool_t Init( METACHN_MODE ai_Mode, m4int16_t ai_iLanguage, ClAccess *ai_poAccess=NULL,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR   ) ;

	//------------------------------------------------------------------------------------

	// Todas las instrucciones para rellenar registros en el metacanal se han construido 
	//teniendo en mente que el diseño del metacanal puede y suele cambiar, con items que
	//se añaden de version a version, y con muchos de ellos que no hacen falta rellenar
	//y son particulares a otros componentes.
	// Por ello, se gestiona en estructuras los items mas comunes y genericos a todo el mundo,
	//siendo posible tambien acceder al regsitro en cuestion y meter a pelo items nuevos.
	// La gestion de registro la llevamos a cabo nosotros. Cuando un nodo solo debe tener 
	//un registro, esta clase se ocupa de ello.
	// La politica es grabar SOLO los items que vamos a utilizar. La capa canal se ocupa de
	//los no indicados. Por eso, solo se salvan los items cuyo m_oValue tiene el flag de 
	//Updated. (El Save vuelve a desactivar el flag).
	// Repecto a items con idioma, al especificar el nombre interno del item en el array
	//correspondiente, un * al final indica que es de idioma, y el nombre se formara 
	//concantenando el Id de idioma.


	//------------------------------------ SYS_T3S ----------------------------------------

MIT_PUBLIC:

	//Debe haber 1 y solo 1 registro.

	enum ID_T3S { 
		T3S_DT_START, T3S_DT_END, T3S_DT_CORR, //Fechas +Infinto, -Inf, correcion (hoy)
		T3S_ID_T3, T3S_N_T3, //N_T3 con idioma
		T3S_OLE_CLASS_ID, T3S_ID_SERVICE, T3S_ID_SERVICE_ALT, 
		T3S_ID_STREAM_TYPE,
		T3S_IS_SEPARABLE,		
		T3S_CS_EXE_TYPE, 
		T3S_VM_CAPACITY_REQ, T3S_ENVIRONMENT_VARS, 
		T3S_IS_EXTERNAL, T3S_HAS_SECURITY, 
		T3S_IS_CACHEABLE, T3S_IS_MD_CACHEABLE, T3S_CACHE_MAX_PERIOD,
		T3S_VERSION,	//Heredado de SYS_BASE
		T3S_COUNT		
	} ;
	ClMIT_ChnItem m_poT3S[T3S_COUNT] ;

MIT_PRIVATE:

	static const struct StIndexName m_pstT3S_Names[T3S_COUNT] ;

	//------------------------------------ SYS_NODES ----------------------------------------

MIT_PUBLIC:

	//------SYS_NODES solo debe haber un record

	enum ID_SYS_NODES_ITEMS { 
		NODES_ID_T3,  NODES_ID_TI, NODES_ID_NODE, NODES_N_NODE, NODES_OLE_NODE_INTER_ID, 
		NODES_OLE_DISP_ID, NODES_ID_CAPACITY_TYPE, NODES_IS_ROOT, NODES_AUTOLOAD, 
		NODES_NUM_ROWS, NODES_NUM_ROWS_DB, 
		NODES_NUM_KEEP_ROWS, NODES_IS_VISIBLE, NODES_DYN_FILTER, NODES_ID_AUTO_FILTER,
		NODES_ID_CSTYPE, NODES_ID_SYNC_TYPE, NODES_IS_EXTERNAL,
		SYS_NODES_COUNT		
	} ;
	ClMIT_ChnItem m_poSYS_NODES[SYS_NODES_COUNT] ;

MIT_PRIVATE:

	static const struct StIndexName m_pstSYS_NODES_Names[SYS_NODES_COUNT] ;

	//---------------------------------SYS_CONNECTORS ----------------------------------------

MIT_PUBLIC:
	enum ID_SYS_CONNECTORS { 
		CONN_ID_T3, CONN_ID_NODE_USED, CONN_ID_NODE, CONN_ID_CONNECTION_TYPE, 
		CONN_EXPAND_DELETE, CONN_ID_SENTENCE,
		SYS_CONN_COUNT		
	} ;
	ClMIT_ChnItem m_poSYS_CONN[SYS_CONN_COUNT] ;

MIT_PRIVATE:

	static const struct StIndexName m_pstSYS_CONN_Names[SYS_CONN_COUNT] ;

	//---------------------------------SYS_CONNECTOR_ITEM-------------------------------------

MIT_PUBLIC:
	enum ID_SYS_CONNECTOR_ITEM { 
		CONN_ITEM_ID_T3, CONN_ITEM_ID_NODE_USED, CONN_ITEM_ID_NODE, CONN_ITEM_ID_ITEM_USED, 
		CONN_ITEM_ID_ITEM, CONN_ITEM_ID_PRECEDENCE_TYPE, CONN_ITEM_ID_SPIN_TYPE, 
		CONN_ITEM_CALL_ORDER, CONN_ITEM_ID_RELSHIP_TYPE, CONN_ITEM_ID_CONTEXT_TYPE, 
		CONN_ITEM_ID_CSTYPE,
		SYS_CONN_ITEM_COUNT		
	} ;
	ClMIT_ChnItem m_poSYS_CONN_ITEM[SYS_CONN_ITEM_COUNT] ;

MIT_PRIVATE:

	static const struct StIndexName m_pstSYS_CONN_ITEM_Names[SYS_CONN_ITEM_COUNT] ;

	//---------------------------------SYS_CONNECTOR_ARGS-------------------------------------

MIT_PUBLIC:
	enum ID_SYS_CONNECTOR_ARGS { 
		CONN_ARGS_ID_T3, CONN_ARGS_ID_NODE_USED, CONN_ARGS_ID_NODE, CONN_ARGS_ID_ITEM_USED, 
		CONN_ARGS_ID_ITEM, CONN_ARGS_ID_ARG_USED, 
		CONN_ARGS_ID_ARG, CONN_ARGS_ID_ARG_VAL_USED_TYPE,
		SYS_CONN_ARGS_COUNT		
	} ;
	ClMIT_ChnItem m_poSYS_CONN_ARGS[SYS_CONN_ARGS_COUNT] ;

MIT_PRIVATE:

	static const struct StIndexName m_pstSYS_CONN_ARGS_Names[SYS_CONN_ARGS_COUNT] ;

	//---------------------------------SYS_T3_CONNECTORS-------------------------------------

MIT_PUBLIC:
	enum ID_SYS_T3_CONNECTORS { 
		T3_CONN_ID_T3_USED, T3_CONN_ID_T3, T3_CONN_ID_NODE_USED, T3_CONN_ID_NODE, 
		T3_CONN_ID_CONNECTION_TYPE, T3_CONN_IS_FILTER, T3_CONN_ID_RELATION_TYPE, 
		SYS_T3_CONN_COUNT		
	} ;
	ClMIT_ChnItem m_poSYS_T3_CONN[SYS_T3_CONN_COUNT] ;

MIT_PRIVATE:

	static const struct StIndexName m_pstSYS_T3_CONN_Names[SYS_T3_CONN_COUNT] ;

	//---------------------------------SYS_T3_CONNECT_ITEM-------------------------------------

MIT_PUBLIC:
	enum ID_SYS_T3_CONN_ITEM { 
		T3_CONN_ITEM_ID_T3_USED, T3_CONN_ITEM_ID_T3, T3_CONN_ITEM_ID_NODE_USED, 
		T3_CONN_ITEM_ID_NODE, T3_CONN_ITEM_ID_ITEM_USED, T3_CONN_ITEM_ID_ITEM, 
		T3_CONN_ITEM_ID_RELSHIP_TYPE,
		SYS_T3_CONN_ITEM_COUNT		
	} ;
	ClMIT_ChnItem m_poSYS_T3_CONN_ITEM[SYS_T3_CONN_ITEM_COUNT] ;

MIT_PRIVATE:

	static const struct StIndexName m_pstSYS_T3_CONN_ITEM_Names[SYS_T3_CONN_ITEM_COUNT] ;

	//------------------------------------SYS_TIS---------------------------------------------

MIT_PUBLIC:
	enum ID_SYS_TIS { 
		TIS_ID_T3, TIS_ID_TI, /*TIS_ID_TI_BASE,*/ TIS_ID_INHERIT_TYPE, TIS_ID_READ_SENTENCE, 
		TIS_ID_WRITE_SENTENCE, TIS_ID_READ_OBJECT, TIS_ID_WRITE_OBJECT, 
		TIS_ID_TEMPORARY, TIS_COMPLETE, TIS_CORRECTED, 
		SYS_TIS_COUNT		
	} ;
	ClMIT_ChnItem m_poSYS_TIS[SYS_TIS_COUNT] ;

MIT_PRIVATE:

	static const struct StIndexName m_pstSYS_TIS_Names[SYS_TIS_COUNT] ;

	//------------------------------------SYS_ITEMS-------------------------------------------

MIT_PUBLIC:
	enum ID_SYS_ITEMS { 
		ITEMS_ID_T3, ITEMS_ID_TI, ITEMS_ID_ITEM, ITEMS_ID_SYNONYM, ITEMS_N_ITEM, 
		ITEMS_DEFAULT_VALUE, ITEMS_ID_ITEM_TYPE, ITEMS_ID_SCOPE_TYPE, 
		ITEMS_ID_M4_TYPE, ITEMS_PREC, ITEMS_SCALE, ITEMS_ITEM_ORDER, ITEMS_ID_DMD, 
		ITEMS_ID_DMD_COMPONENT, ITEMS_ID_DMD_CROSSTAB, ITEMS_ID_DMD_FIELD, 
		ITEMS_ID_SLICE_BHVR_TYPE, ITEMS_ID_SLICE_SPLIT_TP, 
		ITEMS_ID_SLICE_TOT_TYPE, ITEMS_ID_TOTALIZE_TYPE, 
		ITEMS_ID_TRANSACT_TYPE, ITEMS_ID_EXE_LEVEL, 
		ITEMS_EXE_GROUP, ITEMS_ID_SLICE_ITEM, ITEMS_ID_ITEM_AUX, ITEMS_ALIAS_READ_OBJECT, 
		ITEMS_ALIAS_WRITE_OBJECT, ITEMS_ID_READ_FIELD, ITEMS_ID_WRITE_FIELD, 
		ITEMS_ID_READ_OBJECT, ITEMS_ID_WRITE_OBJECT, 
		ITEMS_ID_SENT_TOT_TYPE, ITEMS_VARIABLE_ARGUMENTS, ITEMS_IS_VISIBLE, 
		ITEMS_MODIFY_OTHER_ITEMS, ITEMS_ID_INTERNAL_TYPE, ITEMS_ID_CSTYPE, 
		ITEMS_ID_SYNC_TYPE, ITEMS_IS_PUBLIC, ITEMS_IS_PARAMETER, ITEMS_CHECK_NOT_NULL, 
		ITEMS_CLIENT_NOT_NULL,
		SYS_ITEMS_COUNT		
	} ;
	ClMIT_ChnItem m_poSYS_ITEMS[SYS_ITEMS_COUNT] ;

MIT_PRIVATE:

	static const struct StIndexName m_pstSYS_ITEMS_Names[SYS_ITEMS_COUNT] ;

	//------------------------------------SYS_ITEM_CSDESC-------------------------------------

MIT_PUBLIC:
	enum ID_SYS_ITEM_CSDESC { 
		ITEM_CSDESC_ID_T3, ITEM_CSDESC_ID_NODE, ITEM_CSDESC_ID_ITEM, ITEM_CSDESC_ID_CSTYPE, 
		ITEM_CSDESC_ID_SYNC_TYPE, 
		SYS_ITEM_CSDESC_COUNT		
	} ;
	ClMIT_ChnItem m_poSYS_ITEM_CSDESC[SYS_ITEM_CSDESC_COUNT] ;

MIT_PRIVATE:

	static const struct StIndexName m_pstSYS_ITEM_CSDESC_Names[SYS_ITEM_CSDESC_COUNT] ;


	//---------------------------------SYS_ITEM_ARGS-------------------------------------

MIT_PUBLIC:
	enum ID_SYS_ITEM_ARGS { 
		ITEM_ARGS_ID_T3, ITEM_ARGS_ID_TI, ITEM_ARGS_ID_ITEM, ITEM_ARGS_ID_ARGUMENT, 
		ITEM_ARGS_ID_ARGUMENT_TYPE, ITEM_ARGS_ID_M4_TYPE, 
		ITEM_ARGS_PREC, ITEM_ARGS_SCALE, ITEM_ARGS_POSITION, 
		SYS_ITEM_ARGS_COUNT		
	} ;
	ClMIT_ChnItem m_poSYS_ITEM_ARGS[SYS_ITEM_ARGS_COUNT] ;

MIT_PRIVATE:

	static const struct StIndexName m_pstSYS_ITEM_ARGS_Names[SYS_ITEM_ARGS_COUNT] ;

	//------------------------------------SYS_RULES----------------------------------------

MIT_PUBLIC:
	enum ID_SYS_RULES { 
		SYS_RULES_CHANNEL_DT_START, SYS_RULES_CHANNEL_DT_END, SYS_RULES_CHANNEL_DT_CORR, 
		SYS_RULES_ID_T3, SYS_RULES_ID_TI, 
		SYS_RULES_ID_ITEM, SYS_RULES_ID_RULE, 
		SYS_RULES_DT_START, SYS_RULES_DT_END, SYS_RULES_DT_START_CORR, SYS_RULES_DT_END_CORR, 
		SYS_RULES_IS_METARULE, SYS_RULES_POLISH_CODE, SYS_RULES_ID_CODE_TYPE, 
		SYS_RULES_ID_RESOURCE, SYS_RULES_ID_METHOD, SYS_RULES_ID_PRIORITY, 
		SYS_RULES_IS_CORRECT_RULE,
		SYS_RULES_COUNT		
	} ;
	ClMIT_ChnItem m_poSYS_RULES[SYS_RULES_COUNT] ;

MIT_PRIVATE:

	static const struct StIndexName m_pstSYS_RULES_Names[SYS_RULES_COUNT] ;

	//------------------------------------SYS_SENTENCES----------------------------------------

MIT_PUBLIC:
	enum ID_SYS_SENTENCES { 
		SYS_SENTENCES_ID_T3, SYS_SENTENCES_ID_SENTENCE, SYS_SENTENCES_APISQL, 
		SYS_SENTENCES_MAX_NUM_COL, 
		SYS_SENTENCES_COUNT		
	} ;
	ClMIT_ChnItem m_poSYS_SENTENCES[SYS_SENTENCES_COUNT] ;

MIT_PRIVATE:

	static const struct StIndexName m_pstSYS_SENTENCES_Names[SYS_SENTENCES_COUNT] ;


	//------------------------------------SYS_RSMS----------------------------------------

MIT_PUBLIC:
	enum ID_SYS_RSMS { 
		SYS_RSMS_ID_T3, SYS_RSMS_ID_RSM, SYS_RSMS_ID_MASK,
		SYS_RSMS_COUNT		
	} ;
	ClMIT_ChnItem m_poSYS_RSMS[SYS_RSMS_COUNT] ;

MIT_PRIVATE:

	static const struct StIndexName m_pstSYS_RSMS_Names[SYS_RSMS_COUNT] ;


	//------------------------------------SYS_ALIAS_RES-------------------------------------

MIT_PUBLIC:
	enum ID_SYS_ALIAS_RES { 
		SYS_ALIAS_RES_ID_T3, SYS_ALIAS_RES_ID_NODE, SYS_ALIAS_RES_ALIAS, 
		SYS_ALIAS_RES_ID_NODE_USED, 
		SYS_ALIAS_RES_COUNT		
	} ;
	ClMIT_ChnItem m_poSYS_ALIAS_RES[SYS_ALIAS_RES_COUNT] ;

MIT_PRIVATE:

	static const struct StIndexName m_pstSYS_ALIAS_RES_Names[SYS_ALIAS_RES_COUNT] ;


	//------------------------------------SYS_T3_ALIAS_RES-------------------------------------

MIT_PUBLIC:
	enum ID_SYS_T3_ALIAS_RES { 
		SYS_T3_ALIAS_RES_ID_T3, SYS_T3_ALIAS_RES_ID_NODE, SYS_T3_ALIAS_RES_ALIAS, 
		SYS_T3_ALIAS_RES_ID_T3_USED, SYS_T3_ALIAS_RES_ID_RSM, SYS_T3_ALIAS_RES_INSTANCE, 
		SYS_T3_ALIAS_RES_ID_SHARED_POLICY, SYS_T3_ALIAS_RES_ID_OPEN_MODE_TP, SYS_T3_ALIAS_RES_CS_ACCESS, 
		SYS_T3_ALIAS_RES_COUNT		
	} ;
	ClMIT_ChnItem m_poSYS_T3_ALIAS_RES[SYS_T3_ALIAS_RES_COUNT] ;

MIT_PRIVATE:

	static const struct StIndexName m_pstSYS_T3_ALIAS_RES_Names[SYS_T3_ALIAS_RES_COUNT] ;


	//------------------------------------SYS_INDEXES-----------------------------------------

MIT_PUBLIC:
	enum ID_SYS_INDEXES { 
		SYS_INDEXES_ID_T3, SYS_INDEXES_ID_NODE, SYS_INDEXES_ID_INDEX, 
		SYS_INDEXES_IS_DEFAULT_INDEX,
		SYS_INDEXES_COUNT		
	} ;
	ClMIT_ChnItem m_poSYS_INDEXES[SYS_INDEXES_COUNT] ;

MIT_PRIVATE:

	static const struct StIndexName m_pstSYS_INDEXES_Names[SYS_INDEXES_COUNT] ;
	
	//------------------------------------SYS_INDEX_ITEMS-----------------------------------------

MIT_PUBLIC:
	enum ID_SYS_INDEX_ITEMS { 
		SYS_INDEX_ITEMS_ID_T3, SYS_INDEX_ITEMS_ID_NODE, SYS_INDEX_ITEMS_ID_INDEX, 
		SYS_INDEX_ITEMS_ID_ITEM, SYS_INDEX_ITEMS_ID_FUNCTION, SYS_INDEX_ITEMS_WAY, 
		SYS_INDEX_ITEMS_COUNT		
	} ;
	ClMIT_ChnItem m_poSYS_INDEX_ITEMS[SYS_INDEX_ITEMS_COUNT] ;

MIT_PRIVATE:

	static const struct StIndexName m_pstSYS_INDEX_ITEMS_Names[SYS_INDEX_ITEMS_COUNT] ;

	//------------------------------------SYS_FILTERS-----------------------------------------

MIT_PUBLIC:
	enum ID_SYS_FILTERS { 
		SYS_FILTERS_ID_T3, SYS_FILTERS_ID_NODE, SYS_FILTERS_ID_FILTER, SYS_FILTERS_ID_T3_FILTER,
		SYS_FILTERS_ID_NODE_FILTER, SYS_FILTERS_FILTER_TYPE, 
		SYS_FILTERS_COUNT		
	} ;
	ClMIT_ChnItem m_poSYS_FILTERS[SYS_FILTERS_COUNT] ;

MIT_PRIVATE:

	static const struct StIndexName m_pstSYS_FILTERS_Names[SYS_FILTERS_COUNT] ;

	//---------------------------------SYS_ITEM_TRIGGERS-----------------------------------

MIT_PUBLIC:
	enum ID_SYS_ITEM_TRIGGERS { 
		SYS_ITEM_TRIGGERS_ID_T3, SYS_ITEM_TRIGGERS_ID_TI, SYS_ITEM_TRIGGERS_ID_ITEM_USED, 
		SYS_ITEM_TRIGGERS_ID_ITEM, SYS_ITEM_TRIGGERS_ID_PRECEDENCE_TYPE, 
		SYS_ITEM_TRIGGERS_ID_SPIN_TYPE, SYS_ITEM_TRIGGERS_CALL_ORDER, 
		SYS_ITEM_TRIGGERS_ID_RELSHIP_TYPE, 
		SYS_ITEM_TRIGGERS_ID_CONTEXT_TYPE, SYS_ITEM_TRIGGERS_ID_CSTYPE, 
		SYS_ITEM_TRIGGERS_COUNT		
	} ;
	ClMIT_ChnItem m_poSYS_ITEM_TRIGGERS[SYS_ITEM_TRIGGERS_COUNT] ;

MIT_PRIVATE:

	static const struct StIndexName m_pstSYS_ITEM_TRIGGERS_Names[SYS_ITEM_TRIGGERS_COUNT] ;

	//------------------------------------ Auxiliares ----------------------------------------

MIT_PUBLIC:

	// Hace Save solo de los Updated. Por precacucion, limpia el flag Updated de todos.

	void SaveItems( METACHN_NODES ai_Node ) ;

MIT_PRIVATE:

	//Incializa todos los nodos, true si todos bien

	m4bool_t InitNodes( ) ;

	//Hace Init de los items, todos del nodo ai_poNode. Si algun nombre acaba en *, el nombre 
	//	se forma concatenando el Id del idioma.
	//Tambien rellena m_pstNodeItemInfo
	//Da true si no tuvo problemas

	m4bool_t InitItemArray( 
		METACHN_NODES ai_Node, ClMIT_ChnItem *ai_poItemArr, const StIndexName *ai_pstNameArr, m4int16_t ai_iNItems ) ;

};


#endif


