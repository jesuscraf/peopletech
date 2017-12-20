//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             analex.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:17/02/98
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Manuel Lazcano Perez (14/05/98 - 22/05/98 )
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================


#include "eng.h"
#include "engdf.inl"
#include "mitdates.h"

#include "trace.h"
#include "clargs.h"
#include "core.h"
#include "clformat.h"
#include "sysinfo.h"
#include "xengval.h"
#include "clrepreq.h"
#include "physfrp.h"

#include "ddlf_chn.h"

#include "analex.hmg"
#include "analex.h"

#include "replaceliterals.hpp"
#include "m4unicode.hpp"

#include "sesschn.h"

//==================================================================>Macros para analizador

#define ANALEX_BLANK_STR " \t"
#define ANALEX_INTRA_TOKEN "."
#define ANALEX_ARG_SEPARATOR ','
#define ANALEX_FUNCTIONC '%'
#define ANALEX_RESERVED_WC '$'
#define ANALEX_RESERVED_WS "$"
#define ANALEX_OPEN '('
#define ANALEX_CLOSE ')'

#define ANALEX_OPERATOR "<<=>&|!+-*/[]"

#define ANALEX_DIGIT    "0123456789"
#define ANALEX_LETTER   "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"

#define ANALEX_LITERAL_NUMERIC ANALEX_DIGIT "."
#define ANALEX_LITERAL_NUM_BEG ANALEX_DIGIT
#define ANALEX_LITERAL_STRC '"'
#define ANALEX_LITERAL_STRS "\""
#define ANALEX_LITERAL_NTRANSC '\''
#define ANALEX_LITERAL_NTRANSS "'"
#define ANALEX_LITERAL_DATEO_C '{'
#define ANALEX_LITERAL_DATEO_S "{"
#define ANALEX_LITERAL_DATEC_C '}'
#define ANALEX_LITERAL_DATEC_S "}"
#define ANALEX_LITERAL_TRUE  "TtYy"
#define ANALEX_LITERAL_FALSE "FfNn"

#define ANALEX_COMMENT_1S  "/"
#define ANALEX_COMMENT_2S  "*"
#define ANALEX_COMMENT_1C  '/'
#define ANALEX_COMMENT_2C  '*'

#define ANALEX_LITERAL_NULL_BOOLEAN  "NULL_B"
#define ANALEX_LITERAL_NULL_STRING   "NULL_S"
#define ANALEX_LITERAL_NULL_DATE     "NULL_D"
#define ANALEX_LITERAL_NULL_NUMERIC  "NULL_N"
#define ANALEX_LITERAL_NULL_CURRENCY "NULL_C"

#define ANALEX_LITERAL_CURRENCY "CUR" 

#define ANALEX_CHN_SCAN ANALEX_LETTER ANALEX_DIGIT "_" ANALEX_INTRA_TOKEN ANALEX_RESERVED_WS
#define ANALEX_CHN_BEG ANALEX_LETTER "_"
#define ANALEX_FUNCTION_SCAN ANALEX_LETTER ANALEX_DIGIT ANALEX_INTRA_TOKEN ANALEX_RESERVED_WS "_" "+-"
#define ANALEX_LONG_EXPR_SCAN ANALEX_LETTER ANALEX_DIGIT "_"

static const ClMIT_Args::EnumTrans_t AnalexLoadMode [] = 
{
	{ "$NULL", ClMIT_ChnItem::NULL_LOAD },
	{ "$ITEM_LOAD", ClMIT_ChnItem::ITEM_LOAD },
	{ "$GUIDE_LOAD", ClMIT_ChnItem::GUIDE_LOAD },

	{ "$TOTALIZE_COUNT", (ClMIT_ChnItem::LOAD_MODE)-1000},
	{ "$TOTALIZE_SUM", ClMIT_ChnItem::TOTALIZE_SUM },
	{ "$TOTALIZE_AVG", ClMIT_ChnItem::TOTALIZE_AVG },
	{ "$TOTALIZE_MAX", ClMIT_ChnItem::TOTALIZE_MAX },
	{ "$TOTALIZE_MIN", ClMIT_ChnItem::TOTALIZE_MIN },
	{ "$TOTALIZE_FIRST", ClMIT_ChnItem::TOTALIZE_FIRST },
	{ "$TOTALIZE_LAST", ClMIT_ChnItem::TOTALIZE_LAST },
	{ "$TOTALIZE_DESIGN", ClMIT_ChnItem::TOTALIZE_DESIGN  },

	{ "$TOTALIZE_SLICE_COUNT", (ClMIT_ChnItem::LOAD_MODE)-2000 },
	{ "$TOTALIZE_SLICE_SUM", ClMIT_ChnItem::TOTALIZE_SLICE_SUM },
	{ "$TOTALIZE_SLICE_AVG", ClMIT_ChnItem::TOTALIZE_SLICE_AVG },
	{ "$TOTALIZE_SLICE_MAX", ClMIT_ChnItem::TOTALIZE_SLICE_MAX },
	{ "$TOTALIZE_SLICE_MIN", ClMIT_ChnItem::TOTALIZE_SLICE_MIN },
	{ "$TOTALIZE_SLICE_FIRST", ClMIT_ChnItem::TOTALIZE_SLICE_FIRST },
	{ "$TOTALIZE_SLICE_LAST", ClMIT_ChnItem::TOTALIZE_SLICE_LAST },
	{ "$TOTALIZE_SLICE_DESIGN", ClMIT_ChnItem::TOTALIZE_SLICE_DESIGN  },

	{NULL,0}
} ;

enum ANALEX_IV_TYPE { ANALEX_IV_ACU, ANALEX_IV_CT, ANALEX_IV_NODE, ANALEX_IV_GET_STRING, 
					  ANALEX_IV_GET_CURRENT_DATE, ANALEX_IV_GET_GMT_DATE, ANALEX_IV_TODAY, 
					  ANALEX_IV_GET_CURRENT_TIME, ANALEX_IV_GET_CURRENT_TIME_STAMP,
					  ANALEX_IV_GET_GMT_TIME, ANALEX_IV_GET_GMT_TIME_STAMP,
					  ANALEX_IV_TO_DATE, ANALEX_IV_TO_TIME, 
					  ANALEX_IV_TO_TIME_STAMP, ANALEX_IV_SECTION, 
					  ANALEX_IV_IIF, ANALEX_IV_NVL, ANALEX_IV_STRING, ANALEX_IV_SPACE, 
					  ANALEX_IV_STRIN, ANALEX_IV_STRNIN, 
					  ANALEX_IV_MID, ANALEX_IV_TO_LONG, ANALEX_IV_TO_DOUBLE,
					  ANALEX_IV_TO_PRECISION_DOUBLE, ANALEX_IV_CHAR, ANALEX_IV_EXCHANGE_ITEM_CURRENCY, 
					  ANALEX_IV_EXCHANGE_CURRENCY, ANALEX_IV_DEFAULT_CURRENCY,
					  ANALEX_IV_DEFAULT_EXCHANGE, ANALEX_IV_GET_DEF_CUR, ANALEX_IV_IS_NULL,
					  ANALEX_IV_STRLEN, ANALEX_IV_CSTR, ANALEX_IV_ALIGN, 
					  ANALEX_IV_FORMAT_EXPRESSION, ANALEX_IV_FORMAT_STRING, ANALEX_IV_FORMAT_DEF_CUR, 
					  ANALEX_IV_PI_FUN, ANALEX_IV_REPORT_ID, ANALEX_IV_REPORT_NAME, ANALEX_IV_PAGE, 
					  ANALEX_IV_GROUP_PAGE, ANALEX_IV_IS_REPRINT, ANALEX_IV_GET_FILE_PATH, 
					  ANALEX_IV_GET_FILE_NAME, ANALEX_IV_GET_FULL_PATH, ANALEX_IV_PI, 
					  ANALEX_IV_SAVE_ITEM, ANALEX_IV_GET_DEFAULT_DATE, ANALEX_IV_GET_DEFAULT_TYPE,
					  ANALEX_IV_DEFAULT_EXCHANGE2, ANALEX_IV_EXCHANGE_ITEM_CUR2, 
					  ANALEX_IV_GET_CURRENCY_VALUE,
					  ANALEX_IV_SET_DEL_BREAK,  ANALEX_IV_IS_DEL_BREAK,
					  ANALEX_IV_ORGANIZATION_TYPE, ANALEX_IV_ORGANIZATION
					} ; 
	

static const ClMIT_Args::EnumTrans_t AnalexIVType[] = 
{
	{ "TOT",					ANALEX_IV_ACU },
	{ "CT",						ANALEX_IV_CT },
	{ "NODE",					ANALEX_IV_NODE },
	{ "GETSTRING",				ANALEX_IV_GET_STRING },
	{ "GetCurrentDate",			ANALEX_IV_GET_CURRENT_DATE },
	{ "GetGMT_Date",			ANALEX_IV_GET_GMT_DATE },
	{ "Today",					ANALEX_IV_TODAY },
	{ "GetCurrentTime",			ANALEX_IV_GET_CURRENT_TIME },
	{ "GetCurrentTimeStamp",	ANALEX_IV_GET_CURRENT_TIME_STAMP },
	{ "GetGMT_Time",			ANALEX_IV_GET_GMT_TIME },
	{ "GetGMT_TimeStamp",		ANALEX_IV_GET_GMT_TIME_STAMP },
	{ "ToDate",					ANALEX_IV_TO_DATE },
	{ "ToTime",					ANALEX_IV_TO_TIME },
	{ "ToTimeStamp",			ANALEX_IV_TO_TIME_STAMP },
	{ "SECTION",				ANALEX_IV_SECTION },
	{ "IIF",					ANALEX_IV_IIF },
	{ "NVL",					ANALEX_IV_NVL },
	{ "String",					ANALEX_IV_STRING },
	{ "Space",					ANALEX_IV_SPACE },
	{ "StrIn",					ANALEX_IV_STRIN },
	{ "StrNIn",					ANALEX_IV_STRNIN },
	{ "Mid",					ANALEX_IV_MID },
	{ "ToLong",					ANALEX_IV_TO_LONG },
	{ "ToDouble",				ANALEX_IV_TO_DOUBLE },
	{ "ToPrecisionDouble",		ANALEX_IV_TO_PRECISION_DOUBLE },
	{ "Char",					ANALEX_IV_CHAR },
	{ "ExchangeItemCurrency",	ANALEX_IV_EXCHANGE_ITEM_CURRENCY },
	{ "ExchangeCurrency",	    ANALEX_IV_EXCHANGE_CURRENCY },
	{ "ExchangeDefaultCurrency",ANALEX_IV_DEFAULT_CURRENCY },
	{ "DefaultExchange",        ANALEX_IV_DEFAULT_EXCHANGE },
	{ "GetDefaultCurrency",     ANALEX_IV_GET_DEF_CUR },
	{ "IsNull",					ANALEX_IV_IS_NULL },
	{ "StrLen",                 ANALEX_IV_STRLEN },
	{ "CStr",					ANALEX_IV_CSTR },
	{ "Align",					ANALEX_IV_ALIGN },
	{ "FormatExpression",		ANALEX_IV_FORMAT_EXPRESSION},
	{ "FormatString",			ANALEX_IV_FORMAT_STRING },
	{ "FormatDefaultCurrency",  ANALEX_IV_FORMAT_DEF_CUR },
	{ "PI",						ANALEX_IV_PI_FUN },
	{ "Report_Id",				ANALEX_IV_REPORT_ID },
	{ "Report_Name",			ANALEX_IV_REPORT_NAME },
	{ "Page",					ANALEX_IV_PAGE },
	{ "Group_Page",				ANALEX_IV_GROUP_PAGE },
	{ "IsReprint",			    ANALEX_IV_IS_REPRINT},
	{ "GetFilePath",			ANALEX_IV_GET_FILE_PATH },
	{ "GetFileName",			ANALEX_IV_GET_FILE_NAME },
	{ "GetFullPath",			ANALEX_IV_GET_FULL_PATH },
	{ "PRINT_PI",				ANALEX_IV_PI },
	{ "SaveItem",				ANALEX_IV_SAVE_ITEM }, 
	{ "GetDefaultDate",			ANALEX_IV_GET_DEFAULT_DATE},
	{ "GetDefaultType",			ANALEX_IV_GET_DEFAULT_TYPE},
	{ "DefaultExchange2",		ANALEX_IV_DEFAULT_EXCHANGE2},
	{ "ExchangeItemCurrency2",	ANALEX_IV_EXCHANGE_ITEM_CUR2},
	{ "GetCurrencyValue",		ANALEX_IV_GET_CURRENCY_VALUE },
	{ "SetDelBreak",			ANALEX_IV_SET_DEL_BREAK },
	{ "IsDelBreak",				ANALEX_IV_IS_DEL_BREAK },
	{ "OrgType",				ANALEX_IV_ORGANIZATION_TYPE },
	{ "Org",					ANALEX_IV_ORGANIZATION },

	{NULL,0}
} ;


static const ClMIT_Args::EnumTrans_t AnalexSectionAcuFunc[] = 
{
	{ "COUNT",  ClENG_TkReportTot::COUNT },
	{ "TCOUNT", ClENG_TkReportTot::TRUE_COUNT },
	{ "FCOUNT", ClENG_TkReportTot::FALSE_COUNT },
	{ "NCOUNT", ClENG_TkReportTot::NULL_COUNT },
	{ "ACOUNT", ClENG_TkReportTot::ALL_COUNT },
	{ "SUM",    ClENG_TkReportTot::SUM },
	{ "AVG",    ClENG_TkReportTot::AVG },
	{ "MAX",    ClENG_TkReportTot::MAX },
	{ "MIN",    ClENG_TkReportTot::MIN },
	{ "FIRST",  ClENG_TkReportTot::FIRST },
	{ "LAST",   ClENG_TkReportTot::LAST },
	{ "RESET",  ClENG_TkReportTot::RESET },
	{ "ENTRY",  ClENG_TkReportTot::ENTRY },

	{NULL,0}
} ;

static const ClMIT_Args::EnumTrans_t AnalexCTAcuFunc[] =
{
	{ "COUNT",	ClENG_TkCTTot::COUNT },
	{ "TCOUNT", ClENG_TkCTTot::TRUE_COUNT },
	{ "FCOUNT", ClENG_TkCTTot::FALSE_COUNT },
	{ "NCOUNT", ClENG_TkCTTot::NULL_COUNT },
	{ "ACOUNT", ClENG_TkCTTot::ALL_COUNT },
	{ "SUM",	ClENG_TkCTTot::SUM },
	{ "AVG",	ClENG_TkCTTot::AVG },
	{ "MAX",	ClENG_TkCTTot::MAX },
	{ "MIN",	ClENG_TkCTTot::MIN },
	{ "FIRST",	ClENG_TkCTTot::FIRST },
	{ "LAST",	ClENG_TkCTTot::LAST },
	{ "ENTRY",	ClENG_TkCTTot::ENTRY },
	{ "RESET",	ClENG_TkCTTot::RESET },
	{ "HEADER",	ClENG_TkCTTot::HEADER },

	{ NULL, 0 }
};

static const ClMIT_Args::EnumTrans_t AnalexSectionInfoFunc[] = 
{
	{ "RV_PRINTED",   ClENG_TkSectionInfo::RV_COUNT },
	{ "FIRST_DATE",   ClENG_TkSectionInfo::FIRST_DATE },
	{ "LAST_DATE",    ClENG_TkSectionInfo::LAST_DATE },
	{ "START_DATE",   ClENG_TkSectionInfo::START_DATE },
	{ "END_DATE",     ClENG_TkSectionInfo::END_DATE },
	{ "SLICE_INDEX",  ClENG_TkSectionInfo::SLICE_INDEX },
	{ "SLICE_COUNT",  ClENG_TkSectionInfo::SLICE_COUNT },
	{ "GET_PAGE",	  ClENG_TkSectionInfo::GET_PAGE },
	{ "IS_EOF",		  ClENG_TkSectionInfo::IS_EOF },

	{NULL,0}
} ;

static const ClMIT_Args::EnumTrans_t AnalexNODEInfoFunc[] =
{
	{ "IS_EOF",	ClENG_TkNODEInfo::IS_EOF },
	{ "NVL",    ClENG_TkNODEInfo::NVL },

	{ NULL, 0 }
};

//=================================================================================>ClENG_Token

ClENG_Token::ClENG_Token() 
{
	m_poInsp=GET_INSP();
	m_poSons=NULL ;
}

void ClENG_Token::End() 
{
	m4int16_t B ;
	
	if (m_poSons) {
		for (B=0; B<m_iMaxSons; ++B) {
			M4_DELETE_VIRTUAL_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, m_poSons[B].m_poTk ) ;
		} ;
		M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, m_poSons, StSon, m_iMaxSons ) ;
	} ;
}

void ClENG_Token::AddSon( ClENG_Token * ai_poSon ) 
{
	if (m_iCountSons==m_iMaxSons) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_50 ] ) << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
	
	m_poSons[m_iCountSons].m_poTk = ai_poSon ;
	++m_iCountSons ;
}

void ClENG_Token::Init( 
	TYPE ai_Type, m4int16_t ai_iMaxSons, ClMIT_Flag::Value_t ai_Flags,
	ClMIT_ChnValue::TYPE ai_NULLReturnType) 
{
	m4int16_t B ;

	ClENG_Token::End() ;

	m_Type=ai_Type ;
	m_iMaxSons = ai_iMaxSons ;
	m_iCountSons = 0 ;
	m_NULLReturnType = ai_NULLReturnType ;
	m_oFlags.Reset(ai_Flags);
	if ( m_oFlags.True(NOT_EVALUATE_SON) ) m_oFlags.On(DEAL_WITH_NULLS)  ;


	if (m_iMaxSons>0) {
		M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, m_poSons, StSon, m_iMaxSons ) ;
		for (B=0; B<m_iMaxSons; ++B) {
			m_poSons[B].m_poTk=NULL ;
		} ;
	} ;
}

void ClENG_Token::Trace() 
{
	#ifdef RPT_ENGINE_TRACE

	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		m4int16_t B ;
		ClMIT_Str strAux ;

		GetTraceString(&strAux) ;
	
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "%0:s") 
			<< strAux.InChar(0) << CRAZY_MSG << SEND_MSG   ;

		if (m_poSons) {
			m_poInsp->m_poTrace->Indent() ;
			for (B=0; B<m_iCountSons; ++B) {
				m_poSons[B].m_poTk->Trace() ;
			} ;
			m_poInsp->m_poTrace->Unindent() ;
		} ;
	};

	#endif
} ;

void ClENG_Token::GetTraceString(ClMIT_Str *ao_Str) 
{
	ao_Str->StrCpy("What Token is ???") ;
}

void ClENG_Token::Evaluate(ClMIT_ChnValue *ao_poValue) 
{
	m4int16_t B ;
	m4bool_t SwParNULL=M4_FALSE ;
	m4bool_t swShowTrace=M4_FALSE ;
	ClMIT_Str strDebug ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped() 
		&& m_poInsp->m_poTrace->GetGroupDetail(GROUP_ID)>=ClMIT_Msg::CRAZY )
	{
		swShowTrace=M4_TRUE ;
	};
	#endif


	if(swShowTrace){
		GetTraceString(&strDebug) ;
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "%0:s", ClMIT_Msg::ENTRY_FORMAT )
			<< strDebug.InChar(0) << CRAZY_MSG << SEND_MSG   ;
	};

	ao_poValue->End() ;
	if (IsEvaluateSons())  {
		for (B=0; B<GetCountSons(); ++B) {
			if ( EvaluateSon(B) ) SwParNULL=M4_TRUE ;
		} ;
	} ;

	if ( SwParNULL && !IsDealWithNULLs() ) {
		ao_poValue->SetNULL(m_NULLReturnType) ;
	} else {
		EvaluateThis(ao_poValue) ;
	} ;

	if(swShowTrace){
		ClMIT_Str strAux;
		ao_poValue->GetTraceValue(&strAux);

		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "%0:s, result is %1:s", ClMIT_Msg::EXIT_FORMAT  )
			<< strDebug.InChar(0) << strAux.InChar(0) << CRAZY_MSG << SEND_MSG   ;
	};
}

m4bool_t ClENG_Token::EvaluateSon(m4int16_t B) 
{

	m_poSons[B].m_oValue.End() ;
	m_poSons[B].m_poTk->Evaluate( &m_poSons[B].m_oValue ) ;

	return m_poSons[B].m_oValue.IsNULL() ;
}

void ClENG_Token::ResetExternProp( ClMIT_ItemExternProp * ai_poExternProp ) 
{
	m4int16_t B ;

	for (B=0; B<GetCountSons(); ++B) {
		m_poSons[B].m_poTk->ResetExternProp(ai_poExternProp) ;
	} ;
	ResetExternPropThis(ai_poExternProp) ;
}

void ClENG_Token::EvaluateThis(ClMIT_ChnValue *ao_Value) 
{
	ao_Value->End() ;
}

void ClENG_Token::ResetExternPropThis( ClMIT_ItemExternProp * ai_poExternProp ) 
{
}


//==============================================================================>ClENG_TkLiteral

ClENG_TkLiteral::ClENG_TkLiteral() 
{
}

void ClENG_TkLiteral::End() 
{
	ClENG_Token::End() ;
}

void ClENG_TkLiteral::Init( ClMIT_ChnValue::TYPE ai_Type, const m4char_t * ai_pcAnonymousStr, m4bool_t ai_bNullValue ) 
{

	m4return_t			iResult = M4_ERROR;
	m4bool_t			bModified = M4_FALSE;
	m4pchar_t			pcAnonymousStr = NULL;
	m4pchar_t			pcOutput = NULL;
	ClChannelManager *	poChannelManager = NULL;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "ClENG_TkLiteral::Init() Creating ClENG_TkLiteral of type %0:s with %1:s") 
			<< ClMIT_ChnValue::GetTypeString(ai_Type) 
			<< ai_pcAnonymousStr << CRAZY_MSG << SEND_MSG   ;
	};
	#endif

	ClENG_TkLiteral::End() ;

	ClENG_Token::Init( ATOMIC_VALUE, 0 ) ;

	pcAnonymousStr = (m4pchar_t) ai_pcAnonymousStr;

	switch(ai_Type) {
	case ClMIT_ChnValue::NUMERIC:
		m_oValue.InitDouble(0,0,0) ;		
		if(ai_bNullValue) {
			m_oValue.SetNULL();
		} else {
			m_oValue.InitAnonymous(ai_pcAnonymousStr) ;
		};
		break ;
	case ClMIT_ChnValue::STRING:
		// Enh 0164603. Cambiar literales de las expresiones de tipo cadena.		
		// Obtener channel manager.
		poChannelManager = GET_TASK_ENV()->m_APIChn.m_poMVC->GetChannelManager();
		
		// Sustitución de literales.
		iResult = M4ReplaceLiterals(poChannelManager, ai_pcAnonymousStr, pcOutput, NULL, bModified);
		if (iResult == M4_SUCCESS && bModified == M4_TRUE && pcOutput != NULL)
		{
			m_oValue.InitString(pcOutput);
			delete pcOutput;
		}
		else
		{
			m_oValue.InitString(pcAnonymousStr);
		}

		if(ai_bNullValue) {
			m_oValue.SetNULL();
		};
		break ;
	case ClMIT_ChnValue::DATE:
		m_oValue.InitDate(0) ;		
		if(ai_bNullValue) {
			m_oValue.SetNULL();
		} else {
			m_oValue.InitAnonymous(ai_pcAnonymousStr) ;		
		};
		break ;
	case ClMIT_ChnValue::BOOLEAN:
		m_oValue.InitBoolean(M4_FALSE) ;
		if(ai_bNullValue) {
			m_oValue.SetNULL();
		} else {
			m_oValue.InitAnonymous(ai_pcAnonymousStr) ;		
		};
		break ;
	} ;
}

void ClENG_TkLiteral::GetTraceString(ClMIT_Str *ao_Str)  
{
	m_oValue.GetTraceValue(ao_Str) ;
}

void ClENG_TkLiteral::EvaluateThis(ClMIT_ChnValue *ao_Value) 
{
	ao_Value->Init(&m_oValue) ;	
}

//==============================================================================>ClENG_TkItem

ClENG_TkItem::ClENG_TkItem()
{
}

void ClENG_TkItem::End() 
{
	ClENG_Token::End() ;

	m_oItem.End() ;
}

m4bool_t ClENG_TkItem::Init( 
		ClMIT_Chn *ai_poChn, 
		ClMIT_ChnNode * ai_poNodeDefault, 
		const m4char_t *ai_pcNodeName, const m4char_t *ai_pcItemName, 
		LOAD_MODE ai_TkLoadMode, ClMIT_ChnItem::LOAD_MODE ai_LoadMode,
		ClENG_StorageDep *ai_poDep, ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t SwOk=M4_TRUE ;
	ClMIT_ChnNode *Node ;

	if (ai_pcNodeName) Node = ai_poChn->GetNode(ai_pcNodeName, ClMIT_Trace::PROPAGATE_ERROR);

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, 
			"ClENG_TkItem::Init() Creating ClENG_TkItem, channel %0:s node %1:s item %2:s loadmode %3:s" )
			<< ( ai_poChn ? ai_poChn->GetIdDesc() : ClMIT_IdDesc("?","?") )  
			<< ( ai_pcNodeName ? ( Node ? Node->GetIdDesc() : ClMIT_IdDesc("?","?") ) : 
				( ai_poNodeDefault ? ai_poNodeDefault->GetIdDesc() : ClMIT_IdDesc("?","?") )) 
			<< ai_pcItemName << (m4int16_t)ai_LoadMode
			<< CRAZY_MSG << SEND_MSG   ;
	};
	#endif

	ClENG_TkItem::End() ;

	ClENG_Token::Init( ATOMIC_VALUE, 0 ) ;

	m_LoadMode = ai_TkLoadMode ;

	if (!ai_poChn) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_40 ] ) << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	if (!ai_pcNodeName) {
		if (ai_poNodeDefault) {
			Node=ai_poNodeDefault ;
		} else {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_20 ] ) << SEND_MSG ;
			SwOk=M4_FALSE ;
			goto exit ;
		} ;
	} else {
		if ( ! Node ) {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_20 ] ) << SEND_MSG ;
			SwOk=M4_FALSE ;
			goto exit ;
		} ;
	} ;

	if (ai_LoadMode==ClMIT_ChnItem::GUIDE_LOAD && ai_pcNodeName ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_200 ] ) << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;


	SwOk = M4_BOOL(( m_oItem.Init( Node, ai_pcItemName, ClMIT_Trace::PROPAGATE_ERROR ) )==ClMIT_ChnItem::ITEM_OK);	
	if (!SwOk) goto exit ;

	//Salvamos la dependencia antes de comprobar la existencia del Item. Si no existe
	// luego generamos error local, pero la dependencia siempre se salva.
	if (ai_poDep) {
		ai_poDep->StoreItemDependency(m_oItem.GetTiId(), ai_pcItemName);
	};

	m_oItem.SetLoadMode(ai_LoadMode) ;

exit:
	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; 
	return SwOk ;		
}

void ClENG_TkItem::GetTraceString(ClMIT_Str *ao_Str) 
{
	ao_Str->StrCpy("Channel <") ;
	ao_Str->StrCat( m_oItem.GetChannelIdDesc().GetId() ) ;
	ao_Str->StrCat("> Node <") ;
	ao_Str->StrCat( m_oItem.GetNodeIdDesc().GetId() ) ;
	ao_Str->StrCat("> Item <") ;
	ao_Str->StrCat( m_oItem.GetIdDesc().GetId() ) ;
	ao_Str->StrCat(">") ;
}

void ClENG_TkItem::EvaluateThis(ClMIT_ChnValue *ao_Value) 
{
	if ( m_oItem.GetType()==ClMIT_ChnItem::METHOD || m_oItem.GetType()==ClMIT_ChnItem::CONCEPT ) {
		m_oItem.Execute() ;
	} ;
	switch (m_LoadMode) {
	case ITEM_MODE:
		m_oItem.Load() ;	
		ao_Value->Init(&m_oItem.m_oValue) ;
		break ;
	case TOTALIZE_COUNT:
		ao_Value->InitDouble(m_oItem.GetTotalizeCount(),20,0) ;
		break ;
	case TOTALIZE_SLICE_COUNT:
		ao_Value->InitDouble(m_oItem.GetNumSlices(),20,0) ;
		break ;
	} ;
}

void ClENG_TkItem::ResetExternPropThis( ClMIT_ItemExternProp * ai_poExternProp ) 
{
	m_oItem.ResetExternProp(ai_poExternProp) ;
}

//=========================================================================>ClENG_TkPrecedence

ClENG_TkPrecedence::ClENG_TkPrecedence()
{
}

void ClENG_TkPrecedence::End() 
{
	ClENG_Token::End() ;
}

void ClENG_TkPrecedence::Init( m4bool_t ai_bSwOpen ) 
{
	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, 
			"ClENG_TkPrecedence::Init() Creating ClENG_TkPrecedence %0:s") 
			<< (ai_bSwOpen ? '(' : ')') << CRAZY_MSG << SEND_MSG   ;
	};
	#endif

	ClENG_TkPrecedence::End() ;

	ClENG_Token::Init( PRECEDENCE, 0 ) ;

	m_bSwOpen = ai_bSwOpen ;
}

void ClENG_TkPrecedence::GetTraceString(ClMIT_Str *ao_Str) 
{
	ao_Str->StrCpy( "ClENG_TkPrecedence " ) ;
	ao_Str->StrCat( m_bSwOpen ? "(" : ")" ) ;
}

//=========================================================================>ClENG_TkExpSep

ClENG_TkExpSep::ClENG_TkExpSep()
{
}

void ClENG_TkExpSep::End() 
{
	ClENG_Token::End() ;
}

void ClENG_TkExpSep::Init() 
{
	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "ClENG_TkExpSep::Init() Creating ClENG_TkExpSep") 
			<< CRAZY_MSG << SEND_MSG   ;
	};
	#endif

	ClENG_TkExpSep::End() ;

	ClENG_Token::Init( ARG_SEPARATOR, 0 ) ;
}

void ClENG_TkExpSep::GetTraceString(ClMIT_Str *ao_Str)  
{
	ao_Str->StrCpy( "Separator ,") ;
}

//=========================================================================>ClENG_TkOperator

ClENG_TkOperator::ClENG_TkOperator()
{
}

void ClENG_TkOperator::End() 
{
	ClENG_Token::End() ;
}

void ClENG_TkOperator::Init( GROUP_LEVEL ai_Group, m4int16_t ai_iOpLevel, m4int16_t ai_iArgs,
	ClMIT_Flag::Value_t ai_TokenFlags, 
	ClMIT_ChnValue::TYPE ai_NULLReturnType,
	ClMIT_Flag::Value_t ai_OperatorFlags ) 
{
	ClENG_TkOperator::End() ;

	ClENG_Token::Init( OPERATOR, ai_iArgs, ai_TokenFlags, ai_NULLReturnType ) ;

	m_iOpLevel = ai_Group+ai_iOpLevel ;
	m_iArgs    = ai_iArgs ;
	m_oFlags.Reset(ai_OperatorFlags) ;
	if ( m_oFlags.True(IS_FUNCTION) ) m_oFlags.On(TO_RIGHT) ;
}

//=========================================================================>ClENG_TkNormalOperator

ClENG_TkNormalOperator::ClENG_TkNormalOperator() 
{
}

void ClENG_TkNormalOperator::End() 
{
	ClENG_TkOperator::End() ;
}

m4char_t *ClENG_TkNormalOperator::GetOpeString( NORMAL_OPERATOR ai_Ope ) 
{
	m4char_t *Str ;
	
	switch (ai_Ope) {

	case ABSORB_A:
		Str="]  (prec 0, binary)" ;
		break ;
	case ABSORB_B:
		Str="[  (prec 0, binary)" ;
		break ;
	case MINUS_SIGN:
		Str="-- (prec 0, unary)" ;
		break ;
	case DIV:
		Str="/  (prec 1, binary)";
		break ;
	case MUL:
		Str="*  (prec 1, binary)";
		break ;
	case ADD:
		Str="+  (prec 2, binary)";
		break ;
	case SUBSTRACT:
		Str="-  (prec 2, binary)";
		break ;
	case LESS:
		Str="<  (prec 0, binary)";
		break ;
	case LESS_EQUAL:
		Str="<= (prec 0, binary)";
		break ;
	case EQUAL:
		Str="=  (prec 0, binary)";
		break ;
	case NOT_EQUAL:
		Str="<> (prec 0, binary)";
		break ;
	case GREATER:
		Str=">  (prec 0, binary)";
		break ;
	case GREATER_EQUAL:
		Str=">= (prec 0, binary)";
		break ;
	case AND:
		Str="&  (prec 1, binary)";
		break ;
	case OR:
		Str="|  (prec 1, binary)";
		break ;
	case NOT:
		Str="!  (prec 1, unary)";
		break ;
	case STR_CAT:
		Str="&& (prec 0, binary)";
		break ;
	} ;
	return Str ;
} ;	

void ClENG_TkNormalOperator::Init( NORMAL_OPERATOR ai_Operator  ) 
{
	m4int16_t ArgCount ;
	ClMIT_Flag::Value_t Mask ;

	ClENG_TkNormalOperator::End() ;

	m_Operator = ai_Operator ;
	ArgCount = ClMIT_ChnValue::GetOpeArgs((ClMIT_ChnValue::OPERATOR)m_Operator) ; 

	if ( ClMIT_ChnValue::IsOpeRight((ClMIT_ChnValue::OPERATOR)m_Operator) ) {
		Mask = ClENG_TkOperator::TO_RIGHT ;
	} else {
		Mask =ClMIT_Flag::NONE_BITS  ;
	} ;

	switch (m_Operator) {

	case ABSORB_A: case ABSORB_B:
		ClENG_TkOperator::Init(  ClENG_TkOperator::ABSORPTION,0, ArgCount, 
			ClENG_Token::DEAL_WITH_NULLS, ClMIT_ChnValue::NO_INIT, Mask ) ;
		break ;

	case MINUS_SIGN:
		ClENG_TkOperator::Init(  ClENG_TkOperator::ARITHMETICAL,0,ArgCount, 
			ClENG_Token::DEAL_WITH_NULLS, ClMIT_ChnValue::NO_INIT, Mask ) ;
		break ;

	case DIV: case MUL:
		ClENG_TkOperator::Init(  ClENG_TkOperator::ARITHMETICAL,1,ArgCount, 
			ClENG_Token::DEAL_WITH_NULLS, ClMIT_ChnValue::NO_INIT, Mask ) ;
		break ;

	case ADD: case SUBSTRACT:
		ClENG_TkOperator::Init(  ClENG_TkOperator::ARITHMETICAL,2,ArgCount, 
			ClENG_Token::DEAL_WITH_NULLS, ClMIT_ChnValue::NO_INIT, Mask ) ;
		break ;

	case LESS: case LESS_EQUAL: case EQUAL: case NOT_EQUAL: case GREATER: case GREATER_EQUAL:
		ClENG_TkOperator::Init(  ClENG_TkOperator::LOGICAL, 0, ArgCount, 
			ClENG_Token::DEAL_WITH_NULLS, ClMIT_ChnValue::NO_INIT, Mask ) ;
		break ;

	case AND: case OR:	case NOT:

		ClENG_TkOperator::Init(  ClENG_TkOperator::LOGICAL,1,ArgCount, 
			ClENG_Token::DEAL_WITH_NULLS, ClMIT_ChnValue::NO_INIT, Mask ) ;
		break ;

	case STR_CAT:

		ClENG_TkOperator::Init(  ClENG_TkOperator::STRING,0,ArgCount, 
			ClENG_Token::DEAL_WITH_NULLS, ClMIT_ChnValue::NO_INIT, Mask ) ;
		break ;

	} ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, 
			"ClENG_TkNormalOperator::Init() Creating ClTkNormalOperator %0:s") 
			<< GetOpeString() << CRAZY_MSG << SEND_MSG ;
	};
	#endif
}

void ClENG_TkNormalOperator::GetTraceString(ClMIT_Str *ao_Str) 
{
	ao_Str->StrCpy(  "ClENG_TkNormalOperator <") ;
	ao_Str->StrCat( GetOpeString() ) ;
	ao_Str->StrCat( ">" ) ;
}

void ClENG_TkNormalOperator::EvaluateThis(ClMIT_ChnValue *ao_Value) 
{
	if ( GetArgCount()==2 ) {
		(GetPointerToSonValue(0))->Evaluate( (ClMIT_ChnValue::OPERATOR)m_Operator,
			GetPointerToSonValue(1), ao_Value ) ; 
	} else {
		(GetPointerToSonValue(0))->Evaluate( 
			(ClMIT_ChnValue::OPERATOR)m_Operator, NULL, ao_Value ) ; 
	} ;
}

//=========================================================================>ClENG_TkReportTot

const struct ClENG_TkReportTot::StProp ClENG_TkReportTot::m_Prop[] = 
{
	{ "COUNT"		, 0,ClMIT_ChnValue::NO_INIT},
	{ "TRUE_COUNT"	, 0,ClMIT_ChnValue::NO_INIT},
	{ "FALSE_COUNT"	, 0,ClMIT_ChnValue::NO_INIT},
	{ "NULL_COUNT"	, 0,ClMIT_ChnValue::NO_INIT},
	{ "ALL_COUNT"	, 0,ClMIT_ChnValue::NO_INIT},
	{ "SUM"			, 0,ClMIT_ChnValue::NO_INIT},
	{ "AVG"			, 0,ClMIT_ChnValue::NO_INIT},
	{ "MAX"			, 0,ClMIT_ChnValue::NO_INIT},
	{ "MIN"			, 0,ClMIT_ChnValue::NO_INIT},
	{ "FIRST"		, 0,ClMIT_ChnValue::NO_INIT},
	{ "LAST"		, 0,ClMIT_ChnValue::NO_INIT},
	{ "RESET"		, 0,ClMIT_ChnValue::NO_INIT},
	{ "ENTRY"		, 1,ClMIT_ChnValue::NO_INIT}	//Irrelevante, DEAL_NULL
} ;

ClENG_TkReportTot::ClENG_TkReportTot() 
{
	m_poTot=NULL ;
}

void ClENG_TkReportTot::End() 
{
	ClENG_TkOperator::End() ;
}


m4bool_t ClENG_TkReportTot::Init(
	ClENG_Report *ai_poReport, const m4char_t * ai_pcIdAcu, ClENG_TkReportTot::ACTION ai_Action,
	ClENG_StorageDep *ai_poDep, ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t SwOk=M4_TRUE ;
	ClMIT_Flag::Value_t Mask ;
	
	ClENG_TkReportTot::End() ;

	if (ai_poDep){
		//No hay dependencias
		return SwOk;
	};

	m_Action=ai_Action ;
	m_oName.StrCpy(ai_pcIdAcu) ;

	if (m_Action==ENTRY) {
		Mask=ClENG_Token::DEAL_WITH_NULLS ;
	} else {
		Mask=ClMIT_Flag::NONE_BITS ;
	} ;

	ClENG_TkOperator::Init(	ClENG_TkOperator::FUNCTION, 0, 
		m_Prop[m_Action].m_iNArgs, Mask, 
		m_Prop[m_Action].m_NULLReturnType, ClENG_TkOperator::IS_FUNCTION ) ;

	if ( !ai_poReport ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_190 ] ) << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	m_poTot=ai_poReport->GetTotalize(ai_pcIdAcu,ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!m_poTot) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

exit:
	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; 

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, 
			"ClENG_TkReportTot::Init() Creating ClENG_TkReportTot, Acu %0:s Action %1:s") 
			<< m_oName.InChar(0) << m_Prop[m_Action].m_pcTraceName << CRAZY_MSG << SEND_MSG ;
	};
	#endif

	return SwOk ;
}

void ClENG_TkReportTot::GetTraceString(ClMIT_Str *ao_Str) 
{
	ao_Str->StrCpy("ClENG_TkReportTot Acu <") ;
	ao_Str->StrCat( m_oName.InChar(0) ) ;
	ao_Str->StrCat("> Action <") ;
	ao_Str->StrCat( m_Prop[m_Action].m_pcTraceName ) ;
	ao_Str->StrCat(">") ;
}

void ClENG_TkReportTot::EvaluateThis(ClMIT_ChnValue *ao_Value) 
{
	switch (m_Action) {

	case ENTRY:
		m_poTot->Entry( GetPointerToSonValue(0) ) ;
		ao_Value->End() ;
		break ;

	case RESET:
		m_poTot->Reset() ;
		ao_Value->End() ;
		break ;

	case COUNT:
		ao_Value->InitDouble( m_poTot->GetCount(),20,0 );
		break ;

	case TRUE_COUNT:
		ao_Value->InitDouble( m_poTot->GetTrueCount(),20,0 );
		break ;

	case FALSE_COUNT:
		ao_Value->InitDouble( m_poTot->GetFalseCount(),20,0 );
		break ;

	case NULL_COUNT:
		ao_Value->InitDouble( m_poTot->GetNULLCount(),20,0 );
		break ;

	case ALL_COUNT:
		ao_Value->InitDouble( m_poTot->GetCount()+m_poTot->GetNULLCount(),20,0 );
		break ;

	case SUM:
		ao_Value->InitDouble( m_poTot->GetSum(),20,4 );
		break ;
	
	case AVG:
		ao_Value->InitDouble( m_poTot->GetAvg(),20,4 );
		break ;
	
	case MAX:
		m_poTot->GetMax(ao_Value) ;
		break ;

	case MIN:
		m_poTot->GetMin(ao_Value) ;
		break ;

	case FIRST:
		m_poTot->GetFirst(ao_Value) ;
		break ;

	case LAST:
		m_poTot->GetLast(ao_Value) ;
		break ;
	} ;
}


//=========================================================================>ClENG_TkSectionInfo

const struct ClENG_TkSectionInfo::StProp ClENG_TkSectionInfo::m_Prop[] = 
{
	{ "RV_ACTUAL"	, 0,ClMIT_ChnValue::NO_INIT},
	{ "FIRST_DATE"	, 0,ClMIT_ChnValue::NO_INIT},
	{ "LAST_DATE"	, 0,ClMIT_ChnValue::NO_INIT},
	{ "START_DATE"	, 1,ClMIT_ChnValue::DATE},
	{ "END_DATE"	, 1,ClMIT_ChnValue::DATE},
	{ "SLICE_INDEX"	, 0,ClMIT_ChnValue::NO_INIT},
	{ "SLICE_COUNT"	, 0,ClMIT_ChnValue::NO_INIT},
	{ "GET_PAGE"	, 0,ClMIT_ChnValue::NO_INIT},
	{ "IS_EOF"      , 0,ClMIT_ChnValue::NO_INIT},

} ;


ClENG_TkSectionInfo::ClENG_TkSectionInfo() 
{
	m_poSect=NULL ;
}

void ClENG_TkSectionInfo::End() 
{
	ClENG_TkOperator::End() ;
}

m4bool_t ClENG_TkSectionInfo::Init(
		ClENG_Report *ai_poReport, ClENG_Section *ai_poSect, m4uint32_t ai_iIdSec, ACTION ai_Action,
		ClENG_StorageDep *ai_poDep, ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t SwOk=M4_TRUE ;
	
	ClENG_TkSectionInfo::End() ;

	if (ai_poDep) {
		//No hay dependencias
		return SwOk;
	};

	m_Action=ai_Action ;

	ClENG_TkOperator::Init(	ClENG_TkOperator::FUNCTION, 0, 
		m_Prop[m_Action].m_iNArgs, ClMIT_Flag::NONE_BITS, m_Prop[m_Action].m_NULLReturnType,
		ClENG_TkOperator::IS_FUNCTION ) ;

	if ( ai_iIdSec==NULL_SEC ) {
		if ( !ai_poSect ) {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_210 ] ) << SEND_MSG ;
			SwOk=M4_FALSE ;
			goto exit ;
		} ;
	} else {
		if ( !ai_poReport ) {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_210 ] ) << SEND_MSG ;
			SwOk=M4_FALSE ;
			goto exit ;
		} ;
		if ( !(ai_poSect=ai_poReport->FindSec(ai_iIdSec,ClMIT_Trace::PROPAGATE_ERROR) ) ) {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_210 ] ) << SEND_MSG ;
			SwOk=M4_FALSE ;
			goto exit ;
		} ;
	} ;

	m_poSect = ai_poSect ;

	switch ( m_Action ) {
	case FIRST_DATE : case LAST_DATE : case START_DATE : case END_DATE :
		if ( m_poSect->m_oVR.GetGoT()!=ClENG_VR::SLICE_GOT ) {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_215 ] ) << SEND_MSG ;
			SwOk=M4_FALSE ;
			goto exit ;
		} ;
		break ;
	} ;

exit:
	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; 

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, 
			"ClENG_TkSectionInfo::Init() Creating ClENG_TkSectionInfo, Action %0:s") 
			<< m_Prop[m_Action].m_pcTraceName << CRAZY_MSG << SEND_MSG ;
	};
	#endif

	return SwOk ;
}

void ClENG_TkSectionInfo::GetTraceString(ClMIT_Str *ao_Str)  
{
	ao_Str->StrCpy("ClENG_TkSectionInfo Action <") ;
	ao_Str->StrCat( m_Prop[m_Action].m_pcTraceName ) ;
	ao_Str->StrCat(">") ;
}

void ClENG_TkSectionInfo::EvaluateThis(ClMIT_ChnValue *ao_Value) 
{
	switch (m_Action) {

	case RV_COUNT:
		ao_Value->InitDouble( m_poSect->m_oVR.GetVRProcessed()+1,20,0 );
		break ;

	case FIRST_DATE:
		ao_Value->InitDate( m_poSect->m_oVR.GetFirstDate() );
		break ;

	case LAST_DATE:
		ao_Value->InitDate( m_poSect->m_oVR.GetLastDate() );
		break ;

	case START_DATE:
		ao_Value->InitDate( m_poSect->m_oVR.GetStartDate(GetPointerToSonValue(0)->GetDouble()) );
		break ;
	
	case END_DATE:
		ao_Value->InitDate( m_poSect->m_oVR.GetEndDate(GetPointerToSonValue(0)->GetDouble()) );
		break ;
	
	case SLICE_INDEX:
		ao_Value->InitDouble( m_poSect->m_oVR.GetSliceIndex(), 20, 0 );
		break ;

	case SLICE_COUNT:
		ao_Value->InitDouble( m_poSect->m_oVR.GetSliceCount(), 20, 0 );
		break ;

	case GET_PAGE:
		ao_Value->InitDouble( m_poSect->GetNumPageSec(0), 20, 0 );
		break ;

	case IS_EOF:
		ao_Value->InitBoolean( m_poSect->m_oVR.IsEOF() );
		break ;
	} ;
}


//=========================================================================>ClENG_TkCTTot

const struct ClENG_TkCTTot::StProp ClENG_TkCTTot::m_Prop[] = 
{
	{ "COUNT"		, 0,ClMIT_ChnValue::NO_INIT},
	{ "TRUE_COUNT"	, 0,ClMIT_ChnValue::NO_INIT},
	{ "FALSE_COUNT"	, 0,ClMIT_ChnValue::NO_INIT},
	{ "NULL_COUNT"	, 0,ClMIT_ChnValue::NO_INIT},
	{ "ALL_COUNT"	, 0,ClMIT_ChnValue::NO_INIT},
	{ "SUM"			, 0,ClMIT_ChnValue::NO_INIT},
	{ "AVG"			, 0,ClMIT_ChnValue::NO_INIT},
	{ "MAX"			, 0,ClMIT_ChnValue::NO_INIT},
	{ "MIN"			, 0,ClMIT_ChnValue::NO_INIT},
	{ "FIRST"		, 0,ClMIT_ChnValue::NO_INIT},
	{ "LAST"		, 0,ClMIT_ChnValue::NO_INIT},
	{ "ENTRY"		, 1,ClMIT_ChnValue::NO_INIT}, //Da igual, trata con NULLs  (par 1 variable)
	{ "RESET"		, 0,ClMIT_ChnValue::NO_INIT},
	{ "HEADER"		, 0,ClMIT_ChnValue::NO_INIT},
	{ "KEY"			, 0,ClMIT_ChnValue::NO_INIT},
} ;

ClENG_TkCTTot::ClENG_TkCTTot()
{
	m_poKeyTot = NULL;
	m_piLevelIndex=NULL ;	
}

void ClENG_TkCTTot::End()
{
	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, m_piLevelIndex, 
		m4int16_t, m_iArrCount);
	ClENG_TkOperator::End();
}

//Init para las funciones ENTRY y RESET
m4bool_t ClENG_TkCTTot::Init( 
	ACTION ai_Action, ClENG_Report* ai_poReport, ClMIT_Args *ai_poArg, 
	ClENG_StorageDep *ai_poDep, ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect)
{
	m4bool_t SwOk = M4_TRUE ;
	ClMIT_Str Str ;
	m4int16_t B, Min, ArgIndexTree, ArgIndexTot, ArgIndexLevel  ;
	ClMIT_Flag::Value_t TkFlags, OpeFlags ;
	
	if(ai_poDep){
		//No hay dependencias
		return SwOk;
	};

	if (m_Action==ENTRY) {
		TkFlags=ClENG_Token::DEAL_WITH_NULLS ;
	} else {
		TkFlags=ClMIT_Flag::NONE_BITS ;
	} ;
	OpeFlags =ClENG_TkOperator::IS_FUNCTION ;


	ClENG_TkCTTot::End();

	m_Action=ai_Action;

	//Debe existir report
	if(!ai_poReport) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_190 ] ) << SEND_MSG ;
		SwOk=M4_FALSE;
		goto exit;
	};

	//Testeo numero minimo de argumentos
	ArgIndexTree = ArgIndexTot = ArgIndexLevel =  -1 ;
	switch( m_Action ) {
	case ENTRY:
		ArgIndexTot=2 ;
		Min=2 ;
		break ;
	case RESET:
		ArgIndexTree=2 ;
		Min=2 ;
		break ;
	case HEADER:
	case KEY:
		ArgIndexLevel=2 ;
		Min=2 ;
		break ;
	default:
		ArgIndexTree=2 ;
		ArgIndexTot=3 ;
		ArgIndexLevel=4 ;
		Min=4 ;
		break ;
	} ;
	Min += 1 ;	//Minimo anterior es relativo. Le sumo 1 por CT
	if ( ai_poArg->GetArgCount() < 2 ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_130 ] ) << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	//Enganche CT y KeyTot. Todas expresiones lo necesitan
	ai_poArg->GetString(1,0,&Str) ;
	m_poKeyTot=ai_poReport->GetKeyTotalize( Str, ClMIT_Trace::PROPAGATE_ERROR) ;
	if(! m_poKeyTot ) {
		//GetKeyTotalize() ya genero error 
		SwOk=M4_FALSE;
		goto exit;
	} ; 

	//Enganche IndexTree
	if(ArgIndexTree!=-1) {
		ai_poArg->GetString(ArgIndexTree,0,&Str) ;
		m_iIndexTree = m_poKeyTot->GetTreeIndex(Str,ClMIT_Trace::PROPAGATE_ERROR) ;
		if ( m_iIndexTree == ClENG_StrIndex::INDEX_EOF  ) {
			//GetIndex() ya genero error 
			SwOk=M4_FALSE;
			goto exit;
		} ;
	} ;

	//Enganche IndexTot
	if(ArgIndexTot!=-1) {
		ai_poArg->GetString(ArgIndexTot,0,&Str) ;
		m_iIndexTot = m_poKeyTot->GetTotIndex(Str,ClMIT_Trace::PROPAGATE_ERROR) ;
		if ( m_iIndexTot == ClENG_StrIndex::INDEX_EOF  ) {
			//GetIndex() ya genero error 
			SwOk=M4_FALSE;
			goto exit;
		} ;
	} ;

	//Carga de array de indices level

	if (ArgIndexLevel!=-1) {
		m_iArrCount = m_poKeyTot->GetLevelCount() ;
		M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, m_piLevelIndex, 
			m4int16_t, m_iArrCount);
		for (B=0;B<m_iArrCount; ++B) {
			if ( !ai_poArg->IsValidSubArg(ArgIndexLevel+B,0,"ClENG_TkCTTot::Init()") ) {
				//Ya se lanzo mensaje
				SwOk=M4_FALSE ;
				goto exit ;
			} ;
			m_piLevelIndex[B]= ai_poArg->GetLong(ArgIndexLevel+B,0) ;
			m_poKeyTot->SetIndexKey(B,m_piLevelIndex[B]) ;	
		} ;
		m_LoadLevel = m_poKeyTot->GetConsLevelByIndex(m_iArrCount) ;
		m_LoadMode =  m_poKeyTot->IsPartialLoad(m_LoadLevel) ? DYNAMIC_PARTIAL : DYNAMIC_NORMAL ;
	} ;

	switch( m_Action ) {
	case ENTRY:
		ClENG_TkOperator::Init(ClENG_TkOperator::FUNCTION, 0, 
			m_poKeyTot->GetLevelCount()+1, TkFlags, m_Prop[m_Action].m_NULLReturnType, OpeFlags);
		break ;
	case RESET:
		ClENG_TkOperator::Init(ClENG_TkOperator::FUNCTION, 0, 
			m_Prop[m_Action].m_iNArgs, TkFlags, m_Prop[m_Action].m_NULLReturnType, OpeFlags);
		break ;
	case HEADER:
	case KEY:
		for (B=0;B<m_LoadLevel; ++B) {
			m_poKeyTot->SetIndexKey(B,m_piLevelIndex[B]) ;
		} ;
		SwOk = M4_BOOL( SwOk && m_poKeyTot->GetInfoByIndex( ClENG_KeyTot::FIND_REQ,m_LoadLevel,0,0,&m_oInfo ));
		if (!SwOk) goto exit ;

		if (!m_oInfo.m_poTot) {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_240 ] ) << SEND_MSG ;
			SwOk=M4_FALSE ;
			goto exit ;
		} ;

		ClENG_TkOperator::Init(ClENG_TkOperator::FUNCTION, 0, 
			m_Prop[m_Action].m_iNArgs, TkFlags, m_Prop[m_Action].m_NULLReturnType, OpeFlags);
		break ;

	default:

		if (m_LoadMode==DYNAMIC_NORMAL) {
			for (B=0;B<m_LoadLevel; ++B) {
				m_poKeyTot->SetIndexKey(B,m_piLevelIndex[B]) ;
			} ;
			SwOk = M4_BOOL( SwOk && m_poKeyTot->GetInfoByIndex( ClENG_KeyTot::FIND_REQ,m_LoadLevel,
							m_iIndexTree,m_iIndexTot,&m_oInfo )) ;
			if (!SwOk) goto exit ;

			if (!m_oInfo.m_poTot) {
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_240 ] ) << SEND_MSG ;
				SwOk=M4_FALSE ;
				goto exit ;
			} ;
		} ;

		ClENG_TkOperator::Init(ClENG_TkOperator::FUNCTION, 0, 
			m_Prop[m_Action].m_iNArgs, TkFlags, m_Prop[m_Action].m_NULLReturnType, OpeFlags);
		break ;
	} ;

exit:
	m_poInsp->m_poTrace->TestEffect(SwOk,ai_ErrorEffect);

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, 
			"ClENG_TkCTTot::Init() ClENG_TkCTTot::Init() Creating ClENG_TkCTTot") 
			<< CRAZY_MSG << SEND_MSG ;
	};
	#endif

	return SwOk;
}

m4uint32_t ClENG_TkCTTot::GetNextCTInfo(m4char_t *ai_pcExpr, 
		ClMIT_Str *ao_IdCT, ClMIT_Str *ao_IdTree, ClMIT_Str *ao_IdTot, ClMIT_Str *ao_Func ) 
{
	m4uint32_t  R=0, Long ;
	m4char_t *CTPos, *PosAux ;
	ClMIT_Args Arg ;

	ao_IdCT->StrCpy("") ;
	ao_IdTree->StrCpy("") ;
	ao_IdTot->StrCpy("") ;
	ao_Func->StrCpy("") ;

	CTPos = ClMIT_Str::StrIStr(ai_pcExpr, "%CT" ) ;
	if (CTPos) {

		R =  CTPos - ai_pcExpr + 3 /*Len %CT*/ ;

		Long = strspn(CTPos+1, ANALEX_FUNCTION_SCAN) ;
		Arg.Init( CTPos+1, Long, ClMIT_Args::NO_SUBARG, ANALEX_INTRA_TOKEN ) ;

		//Arg[0] es CT, Arg[1] IdCT, etc

		if ( Arg.GetArgCount()>=2 ) {

			Arg.GetString(1,0,ao_IdCT) ;

			Arg.GetString(Arg.GetArgCount()-1,0,ao_Func) ;
			//Recorto por ( a ao_Func
			PosAux = strchr(ao_Func->InChar(0),'(') ;
			if (PosAux) {
				*PosAux = M4_END_STR ;
			} ;
			
			//Casos en que debe leerse IdTot

			if (  ClMIT_Str::StrICmpTrim(ao_Func->InChar(0),"ENTRY")==0  ) {
				if ( Arg.GetArgCount()>=3 ) {
					Arg.GetString(2,0,ao_IdTot) ;
				} ;
			} else if ( 
				ClMIT_Str::StrICmpTrim(ao_Func->InChar(0),"RESET")!=0 &&
				ClMIT_Str::StrICmpTrim(ao_Func->InChar(0),"HEADER")!=0 && 
				ClMIT_Str::StrICmpTrim(ao_Func->InChar(0),"KEY")!=0 ) 
			{
				if ( Arg.GetArgCount()>=4 ) {
					Arg.GetString(3,0,ao_IdTot) ;
				} ;
			} ;

			//Casos en que debe leerse IdTree

			if (  ClMIT_Str::StrICmpTrim(ao_Func->InChar(0),"RESET")==0  ) {
				if ( Arg.GetArgCount()>=3 ) {
					Arg.GetString(2,0,ao_IdTree) ;
				} ;
			} else if ( 
				ClMIT_Str::StrICmpTrim(ao_Func->InChar(0),"ENTRY")!=0 &&
				ClMIT_Str::StrICmpTrim(ao_Func->InChar(0),"HEADER")!=0 && 
				ClMIT_Str::StrICmpTrim(ao_Func->InChar(0),"KEY")!=0 ) 
			{
				if ( Arg.GetArgCount()>=3 ) {
					Arg.GetString(2,0,ao_IdTree) ;
				} ;
			} ;

		} ;

	} else {
		R=0 ;
	} ;

	return R ;
}

void ClENG_TkCTTot::GetTraceString(ClMIT_Str *ao_Str) 
{
	ao_Str->StrCpy("ClENG_TkCtTot Action <") ;
	ao_Str->StrCat( m_Prop[m_Action].m_pcTraceName ) ;
	ao_Str->StrCat(">") ;
}


void ClENG_TkCTTot::EvaluateThis(ClMIT_ChnValue* ao_Value)
{
	m4int16_t B ;

	switch(m_Action) {

	case RESET:
		m_poKeyTot->TotReset(m_iIndexTree);
		ao_Value->End();
		break;

	case HEADER:
		ao_Value->Init(m_oInfo.m_poHeader);
		break;

	case KEY:
		ao_Value->Init(m_oInfo.m_poKey);
		break;

	case ENTRY:

		for (B=0;B<m_poKeyTot->GetLevelCount(); ++B) {
			m_poKeyTot->SetValueKey(B, GetPointerToSonValue(B+1)) ;
		} ;
		m_poKeyTot->TotEntryByValue( 
			m_poKeyTot->GetLevelCount(), ClENG_KeyTot::ON_ALL_TREES,
			m_iIndexTot, GetPointerToSonValue( 0 ) );
		ao_Value->End();
		break;

	default : 
		{
		ClENG_Tot AuxTot ;

		if (m_LoadMode==DYNAMIC_PARTIAL) {
			for (B=0;B<m_poKeyTot->GetLevelCount(); ++B) {
				m_poKeyTot->SetIndexKey(B, m_piLevelIndex[B]) ;
			} ;
			m_poKeyTot->GetPartialInfoByIndex( m_LoadLevel, m_iIndexTree, m_iIndexTot, &AuxTot ) ;
			m_oInfo.m_poTot = &AuxTot ;
		} ;

		switch(m_Action) {

		case COUNT:
			ao_Value->InitDouble( m_oInfo.m_poTot->GetCount(),20,0 );
			break;

		case TRUE_COUNT:
			ao_Value->InitDouble( m_oInfo.m_poTot->GetTrueCount(),20,0 );
			break ;

		case FALSE_COUNT:
			ao_Value->InitDouble( m_oInfo.m_poTot->GetFalseCount(),20,0 );
			break ;

		case NULL_COUNT:
			ao_Value->InitDouble( m_oInfo.m_poTot->GetNULLCount(),20,0 );
			break ;

		case ALL_COUNT:
			ao_Value->InitDouble( m_oInfo.m_poTot->GetCount()+m_oInfo.m_poTot->GetNULLCount(),20,0 );
			break ;

		case SUM:
			ao_Value->InitDouble( m_oInfo.m_poTot->GetSum(), 20, 4 );
			break;

		case AVG:
			ao_Value->InitDouble( m_oInfo.m_poTot->GetAvg(), 20, 4 );
			break;

		case MAX:
			m_oInfo.m_poTot->GetMax(ao_Value);
			break;

		case MIN:
			m_oInfo.m_poTot->GetMin(ao_Value);
			break;

		case FIRST:
			m_oInfo.m_poTot->GetFirst(ao_Value);
			break;

		case LAST:
			m_oInfo.m_poTot->GetLast(ao_Value);
			break;
		} ;

		} ;
		break ;

	} ;

}

//=========================================================================>ClENG_TkNODEInfo

const struct ClENG_TkNODEInfo::StProp ClENG_TkNODEInfo::m_Prop[] = 
{
	{ "IS_EOF"	, 0,ClMIT_ChnValue::NO_INIT},
	{ "NVL"     , 2,ClMIT_ChnValue::NO_INIT},
	
} ;


ClENG_TkNODEInfo::ClENG_TkNODEInfo() 
{
	m_poNode=NULL ;
}

void ClENG_TkNODEInfo::End() 
{
	ClENG_TkOperator::End() ;
}

m4bool_t ClENG_TkNODEInfo::Init(
		ACTION ai_Action, ClENG_Report* ai_poReport, m4char_t *ai_pcIdNODE,
		ClENG_StorageDep *ai_poDep, ClMIT_Chn *ai_poChn,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t SwOk=M4_TRUE ;
	ClMIT_Flag::Value_t Mask ;
	
	ClENG_TkNODEInfo::End() ;

	m_Action=ai_Action ;

	Mask=ClMIT_Flag::NONE_BITS;
	if (m_Action==NVL ) Mask=Mask|ClENG_Token::DEAL_WITH_NULLS ;
	if (m_Action==NVL ) Mask=Mask|ClENG_Token::NOT_EVALUATE_SON ;


	ClENG_TkOperator::Init(	ClENG_TkOperator::FUNCTION, 0, 
		m_Prop[m_Action].m_iNArgs, Mask, m_Prop[m_Action].m_NULLReturnType,
		ClENG_TkOperator::IS_FUNCTION ) ;

	m_poChn = ai_poChn  ;

	if ( !( m_poNode = m_poChn->GetNode( ai_pcIdNODE, ClMIT_Trace::PROPAGATE_ERROR ) ) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	};

	//Salvamos la dependencia antes de comprobar la existencia del Nodo. Si no existe
	// generamos error y nos salimos, pero la dependencia siempre se salva.
	if (ai_poDep){
		SwOk = ai_poDep->StoreNodeDependency(m_poNode->GetM4ObjId(), ai_pcIdNODE);
		if(!SwOk) goto exit;
	};

exit:
	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; 

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, 
			"ClENG_TkNODEInfo::Init() Creating ClENG_TkNODEInfo, Action %0:s") 
			<< m_Prop[m_Action].m_pcTraceName << CRAZY_MSG << SEND_MSG ;
	};
	#endif

	return SwOk ;
}

void ClENG_TkNODEInfo::GetTraceString(ClMIT_Str *ao_Str) 
{
	ao_Str->StrCpy("ClENG_TkNODEInfo Action <") ;
	ao_Str->StrCat( m_Prop[m_Action].m_pcTraceName ) ;
	ao_Str->StrCat(">") ;
}

void ClENG_TkNODEInfo::EvaluateThis(ClMIT_ChnValue *ao_Value) 
{
	switch (m_Action) {

	case IS_EOF:
		ao_Value->InitBoolean(m_poNode->IsEOF());
		break ;

	case NVL:
		if ( !m_poNode->IsEOF() ) {
			EvaluateSon(0) ;
			ao_Value->Init(GetPointerToSonValue(0));
		} else {
			EvaluateSon(1) ;
			ao_Value->Init(GetPointerToSonValue(1));
		};
		break;

	} ;
}

//=========================================================================>ClENG_TkThreadInfo


const struct ClENG_TkThreadInfo::StProp ClENG_TkThreadInfo::m_Prop[] = 
{
	{ "Report_Id"	, 0,ClMIT_ChnValue::NO_INIT},
	{ "Report_Name"	, 0,ClMIT_ChnValue::NO_INIT},
	{ "Page"		, 0,ClMIT_ChnValue::NO_INIT},
	{ "Group_Page"	, 0,ClMIT_ChnValue::NO_INIT},
	{ "IsReprint"	, 0,ClMIT_ChnValue::NO_INIT},
	{ "GetFilePath"	, 1,ClMIT_ChnValue::STRING},
	{ "GetFileName"	, 1,ClMIT_ChnValue::STRING},
	{ "GetFullPath"	, 1,ClMIT_ChnValue::STRING},
	{ "SetDelBreak"	, 1,ClMIT_ChnValue::NO_INIT},
	{ "IsDelBreak"	, 0,ClMIT_ChnValue::BOOLEAN}
} ;

ClENG_TkThreadInfo::ClENG_TkThreadInfo()
{
	m_poReport	= NULL ;
	m_poDvc		= NULL ;
	m_poExeParams = NULL ;
}

void ClENG_TkThreadInfo::End()
{
	ClENG_TkOperator::End() ;
}


m4bool_t ClENG_TkThreadInfo::Init( 
		ClENG_Report* ai_poReport, ClENG_OutDevice *ai_poDvc, 
		ClENG_ReportRequest *ai_poExeParams,
		ACTION ai_Action, ClENG_StorageDep *ai_poDep,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect )
{
	m4bool_t SwOk=M4_TRUE ;

	ClENG_TkThreadInfo::End() ;

	if (ai_poDep) {
		//No hay dependencias
		return SwOk;
	};

	m_Action=ai_Action ;

	ClENG_TkOperator::Init( ClENG_TkOperator::FUNCTION, 0, m_Prop[m_Action].m_iNArgs, 
		ClMIT_Flag::NONE_BITS, m_Prop[m_Action].m_NULLReturnType, ClENG_TkOperator::IS_FUNCTION ) ;

	if(!ai_poReport){
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_190 ] ) << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit;
	}

	m_poReport=ai_poReport ;

	if(!ai_poDvc){
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_360 ] ) << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit;
	}

	m_poDvc = ai_poDvc ; 


	if(!ai_poExeParams){
		/*##Cambiar msg*/
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_360 ] ) << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit;
	}

	m_poExeParams = ai_poExeParams ;
exit:
	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, 
			"ClENG_TkThreadInfo::Init() ClENG_TkThreadInfo::Init() Action %0:s") 
			<< m_Prop[m_Action].m_pcTraceName << CRAZY_MSG << SEND_MSG ;
	};
	#endif

	return SwOk;
}


void ClENG_TkThreadInfo::GetTraceString(ClMIT_Str *ao_Str) 
{
	ao_Str->StrCpy("ClENG_TkThreadInfo Action <") ;
	ao_Str->StrCat( m_Prop[m_Action].m_pcTraceName ) ;
	ao_Str->StrCat(">") ;
}


void ClENG_TkThreadInfo::EvaluateThis(ClMIT_ChnValue* ao_Value)
{
	ClMIT_Str path ;
	m4uint16_t NPage ;
	ClENG_PhysicalFile * F ;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;

	switch ( m_Action ){
		
		case Report_Id: 
			ao_Value->InitString( m_poReport->GetReportId() ) ;
			break;

		case Report_Name:
			ao_Value->InitString( m_poReport->GetReportName() );
			break;

		case Page:
			ao_Value->InitDouble( m_poDvc->GetNumPageDvc(0), 20, 0);
			break;

		case Group_Page:
			ao_Value->InitDouble( m_poDvc->GetNumBreakPageDvc(0), 20, 0);
			break;

		case SET_DEL_BREAK:
			m_poDvc->SetDelBreak( GetPointerToSonValue(0)->GetBoolean() ) ;
			ao_Value->End() ;
			break;

		case IS_DEL_BREAK:
			ao_Value->InitBoolean( m_poDvc->IsDelBreak() ) ;
			break;

		case IsReprint:
			ao_Value->InitBoolean(m_poExeParams->GetReprinter());
			break;


		case GetFilePath:
			NPage=(m4int16_t)GetPointerToSonValue(0)->GetDouble()-1 ;
			/*
			//Comprobamos que el puntero a la capa device no es nulo
			if(!(TaskEnv->g_poDvc)){
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_360 ] ) << SEND_MSG ;
				ao_Value->InitString("Unable to resolve device");
				break;
			};
			*/

			//Comprobamos que el numero de pagina layout especificado esta dentro del rango de paginas
			if(! M4_IN_RANGE( NPage, 0, m_poDvc->GetNumLayoutPages()-1) ){
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_370 ] ) << SEND_MSG ;
				ao_Value->InitString("Layout Page out of range");
				break;
			};

			//Comprobamos que el puntero al fichero de salida devuelto por la capa device no es nulo
			if( (F=m_poDvc->HasFlushFile(NPage))==NULL  ){
				ao_Value->InitString("Device without output file");
				break;
			};
			
			F->GetAbsolutePath(&path) ;
			ao_Value->InitString(  path.InChar(0));

			break;		
		
		case GetFileName:
			NPage=(m4int16_t)GetPointerToSonValue(0)->GetDouble()-1 ;
			/*
			//Comprobamos que el puntero a la capa device no es nulo
			if(!(TaskEnv->g_poDvc)){
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_360 ] ) << SEND_MSG ;
				ao_Value->InitString("Unable to resolve device");
				break;
			};
			*/

			//Comprobamos que el numero de pagina layout especificado esta dentro del rango de paginas
			if(! M4_IN_RANGE( NPage, 0, m_poDvc->GetNumLayoutPages()-1)){
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_370 ] ) << SEND_MSG ;
				ao_Value->InitString("Layout Page out of range");
				break;
			};

			//Comprobamos que el puntero al fichero de salida devuelto por la capa device no es nulo
			if((F=m_poDvc->HasFlushFile(NPage))==NULL){
				ao_Value->InitString("Device without output file");
				break;
			};
			
			ao_Value->InitString( F->GetFileName() );

			break;

		case GetFullPath:
			//Comprobamos que el puntero a la capa device no es nulo
			NPage=(m4int16_t)GetPointerToSonValue(0)->GetDouble()-1 ;
			/*
			if(!(TaskEnv->g_poDvc)){
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_360 ] ) << SEND_MSG ;
				ao_Value->InitString("Unable to resolve device");
				break;
			};
			*/

			//Comprobamos que el numero de pagina layout especificado esta dentro del rango de paginas
			if(! M4_IN_RANGE( NPage, 0, m_poDvc->GetNumLayoutPages()-1)){
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_370 ] ) << SEND_MSG ;
				ao_Value->InitString("Layout Page out of range");
				break;
			};

			//Comprobamos que el puntero al fichero de salida devuelto por la capa device no es nulo
			if((F=m_poDvc->HasFlushFile(NPage))==NULL){
				ao_Value->InitString("Device without output file");
				break;
			};
			

			F->GetAbsolutePathFile(&path) ;
			ao_Value->InitString(path.InChar(0));

			break;

	}
}


//=========================================================================>ClENG_TkPI

ClENG_TkPI::ClENG_TkPI() 
{
	m_poClField=NULL; 
	m_poFieldManager=NULL;
}

m4bool_t ClENG_TkPI::Init(
	m4char_t *ai_pcIdPI,
	ClENG_FieldsManager *ai_poFieldManager, ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t SwOk=M4_TRUE ; 
	m4bool_t bPiFound = M4_TRUE;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;

	ClENG_TkPI::End() ;

	//Solo se evaluan los hijos a la hora de hacer el FlushValue, no en el PreAnalex
	ClENG_TkOperator::Init(	ClENG_TkOperator::FUNCTION, 0, 1, 
		ClENG_Token::NOT_EVALUATE_SON, ClMIT_ChnValue::NO_INIT, ClENG_TkOperator::IS_FUNCTION ) ;

	if ( !ai_poFieldManager ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_220 ] ) << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
    m_oIdPI.StrCpy(ai_pcIdPI) ;
	m_poFieldManager=ai_poFieldManager;
	
exit:
	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; 

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "ClENG_TkTi::Init() Creating ClENG_TktPI, PI %0:s") 
			<< m_oIdPI.InChar(0) << CRAZY_MSG << SEND_MSG ;
	};
	#endif

	return SwOk ;
}

void  ClENG_TkPI::GetTraceString(ClMIT_Str *ao_Str)  
{
	ao_Str->StrCpy("ClENG_TktPI PI <") ;
	ao_Str->StrCat( m_oIdPI.InChar(0) ) ;
	ao_Str->StrCat(">") ;
}


void ClENG_TkPI::EvaluateThis(ClMIT_ChnValue* ao_Value)
{
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	ClMIT_ChnValue oItemValue ;
	m4int16_t iCount=0 ;
	
	if(!m_poFieldManager){
		//ERROR LOCAL
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_400 ] ) << SEND_MSG ;
		
	}else{
		m_poFieldManager->m_bValueActivated = M4_FALSE ;

		while(m_poFieldManager->ExtractPi(&m_oIdPI,m_poClField, oItemValue)){

			m_poFieldManager->m_oActiveValue.Init(&oItemValue) ;
			m_poFieldManager->SwitchValueActivation() ;
			EvaluateSon(0);	
			m_poClField->ResolvePi(GetPointerToSonValue(0));
			m_poFieldManager->SwitchValueActivation() ;
			iCount++;
		};	

		if(iCount==0){
			//Si no se encuentra la PI generamos error local
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_390 ] ) 
				<< m_oIdPI.InChar(0) << SEND_MSG ;
		};
	} ;

}


//=========================================================================>ClENG_TkExtractString

void ClENG_TkExtractString::End() 
{
	m_poTrLit = NULL ;
}


m4bool_t ClENG_TkExtractString::Init(
	m4char_t *ai_pcIdString, StENG_ReportEnv::TrLitMap_t *ai_poTrLit, 
	ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t SwOk=M4_TRUE ; 
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;

	ClENG_TkExtractString::End() ;

	ClENG_Token::Init(ATOMIC_VALUE, 0) ;

	m_oIdString.StrCpy(ai_pcIdString) ;
	m_poTrLit = ai_poTrLit ;
	if ( ! m_poTrLit ) {
		/*##Cambiar Msg*/
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_220 ] ) << SEND_MSG ;
		SwOk = M4_FALSE ;
		goto exit ;
	} ;
	
exit:
	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; 

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "ClENG_TkTi::Init() Creating ClENG_TktPI, PI %0:s") 
			<< m_oIdString.InChar(0) << CRAZY_MSG << SEND_MSG ;
	};
	#endif

	return SwOk ;
}

void  ClENG_TkExtractString::GetTraceString(ClMIT_Str *ao_Str)  
{
	ao_Str->StrCpy("ClENG_TkExtractString StringId <") ;
	ao_Str->StrCat( m_oIdString.InChar(0) ) ;
	ao_Str->StrCat(">") ;
}


void ClENG_TkExtractString::EvaluateThis(ClMIT_ChnValue* ao_Value)
{
	ClMIT_Str oStrAux;
	
	ClENG_Analex::GetTranslatedString(m_oIdString, &oStrAux, m_poTrLit) ;
	ao_Value->InitString(oStrAux);
}


//=========================================================================>ClENG_TkSysInfo

const struct ClENG_TkSysInfo::StProp ClENG_TkSysInfo::m_Prop[] = 
{
	{ "GetCurrentDate"			, 0,ClMIT_ChnValue::NO_INIT},
	{ "GetGMT_Date"				, 0,ClMIT_ChnValue::NO_INIT},
	{ "Today"					, 0,ClMIT_ChnValue::NO_INIT},
	{ "GetCurrentTime"			, 0,ClMIT_ChnValue::NO_INIT},
	{ "GetCurrentTimeStamp"		, 0,ClMIT_ChnValue::NO_INIT},
	{ "GetGMT_Time"				, 0,ClMIT_ChnValue::NO_INIT},
	{ "GetGMT_TimeStamp"		, 0,ClMIT_ChnValue::NO_INIT},
	{ "ToDate"					, 1,ClMIT_ChnValue::STRING},
	{ "ToTime"					, 1,ClMIT_ChnValue::STRING},
	{ "ToTimeStamp"				, 1,ClMIT_ChnValue::STRING},
	{ "IIF"						, 3,ClMIT_ChnValue::NO_INIT},	//Da igual, trata con NULL
	{ "NVL"						, 2,ClMIT_ChnValue::NO_INIT},	//Da igual, trata con NULL
	{ "String"					, 2,ClMIT_ChnValue::STRING},
	{ "Space"					, 1,ClMIT_ChnValue::STRING},
	{ "StrIn"					, 2,ClMIT_ChnValue::NUMERIC},
	{ "StrNIn"					, 2,ClMIT_ChnValue::NUMERIC},
	{ "Mid"						, 3,ClMIT_ChnValue::STRING},
	{ "ToLong"					, 1,ClMIT_ChnValue::NUMERIC},
	{ "ToDouble"				, 1,ClMIT_ChnValue::NUMERIC},
	{ "ToPrecisionDouble"		, 2,ClMIT_ChnValue::NUMERIC},
	{ "Char"					, 1,ClMIT_ChnValue::STRING},
	{ "ExchangeItemCurrency"	, 4,ClMIT_ChnValue::CURRENCY},
	{ "ExchangeCurrency"		, 5,ClMIT_ChnValue::CURRENCY},
	{ "ExchangeDefaultCurrency"	, 2,ClMIT_ChnValue::CURRENCY},
	{ "DefaultExchange"			, 1,ClMIT_ChnValue::CURRENCY},
	{ "GetDefaultCurrency"		, 0,ClMIT_ChnValue::STRING},
	{ "IsNull"					, 1,ClMIT_ChnValue::NO_INIT},//Da igual, trata con NULL
	{ "StrLen"					, 1,ClMIT_ChnValue::NUMERIC},
	{ "CStr"					, 2,ClMIT_ChnValue::STRING},
	{ "Align"					, 3,ClMIT_ChnValue::STRING},
	{ "FormatExpression"		, 2,ClMIT_ChnValue::STRING},
	{ "FormatString"			, 3,ClMIT_ChnValue::STRING},
	{ "FormatDefaultCurrency"	, 1,ClMIT_ChnValue::STRING},
	{ "PI"						, 0,ClMIT_ChnValue::NO_INIT},
	{ "SaveItem"				, 3,ClMIT_ChnValue::NO_INIT},	//No se sabe que tipo
	{ "GetDefaultDate"			, 0,ClMIT_ChnValue::DATE},
	{ "GetDefaultType"			, 0,ClMIT_ChnValue::STRING},
	{ "DefaultExchange2"		, 1,ClMIT_ChnValue::CURRENCY},
	{ "ExchangeItemCurrency2"	, 4,ClMIT_ChnValue::CURRENCY},
	{ "GetCurrencyValue"		, 1,ClMIT_ChnValue::NUMERIC},
	{ "OrgType"					, 0,ClMIT_ChnValue::NUMERIC},
	{ "Org"						, 0,ClMIT_ChnValue::STRING}
} ;


ClENG_TkSysInfo::ClENG_TkSysInfo() 
{
	m_poChn=NULL ;
	m_poExeParams=NULL ;
	m_poDBFormat=NULL ;
	m_poExeParams=NULL ;
}

void ClENG_TkSysInfo::End() 
{
	ClENG_TkOperator::End() ;
}


m4bool_t ClENG_TkSysInfo::Init(
	ClENG_TkSysInfo::SYSINFO ai_Action, ClENG_StorageDep *ai_poDep,
	ClMIT_Chn *ai_poChn, ClENG_ReportRequest *ai_poExeParams,
	ClENG_DBFormat *ai_poDBFormat, ClENG_FieldsManager *ai_poFieldManager,
	ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	ClENG_TkSysInfo::End() ;
	m4bool_t SwOk=M4_TRUE;
	ClMIT_Flag::Value_t Mask ;

	//TRATAMIENTO DE DEPENDENCIAS (Solo SAVE_ITEM)
	if (ai_poDep) {
		return SwOk;
	};
	//FIN TRATAMIENTO DE DEPENDENCIAS

	m_Action=ai_Action ;

	Mask=ClMIT_Flag::NONE_BITS;
	if (m_Action==IS_NULL || m_Action==IIF || m_Action==NVL ) Mask=Mask|ClENG_Token::DEAL_WITH_NULLS ;
	if (m_Action==IIF || m_Action==NVL ) Mask=Mask|ClENG_Token::NOT_EVALUATE_SON ;

	ClENG_TkOperator::Init(	ClENG_TkOperator::FUNCTION, 0, m_Prop[m_Action].m_iNArgs, 
		Mask, m_Prop[m_Action].m_NULLReturnType, ClENG_TkOperator::IS_FUNCTION ) ;

	m_poChn = ai_poChn  ;
	if (ai_Action==SAVE_ITEM || ai_Action==ORGANIZATION_TYPE || ai_Action==ORGANIZATION ) {
		if (!m_poChn) {
			SwOk=M4_FALSE ;
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_40 ] ) << SEND_MSG ;
			goto exit ;
		} ;
	} ;

	m_poExeParams = ai_poExeParams ;
	if (   ai_Action==EXCH_DEF_CURRENCY || ai_Action==DEF_EXCHANGE 
		|| ai_Action==GET_DEFAULT_CURRENCY || ai_Action==GET_DEFAULT_DATE 
		|| ai_Action==GET_DEFAULT_TYPE || ai_Action==FORMAT_DEF_CUR 
		|| ai_Action==DEFAULT_EXCHANGE2 ) 
	{
		if (!m_poExeParams) {
			/*## Crear nuevo error*/
			SwOk=M4_FALSE ;
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_40 ] ) << SEND_MSG ;
			goto exit ;
		} ;
	} ;

	m_poDBFormat = ai_poDBFormat ;
	if ( ai_Action==FORMAT_EXPR || ai_Action==FORMAT_STR || ai_Action==FORMAT_DEF_CUR )
	{
		if (!m_poDBFormat) {
			SwOk=M4_FALSE ;
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_270 ] ) << SEND_MSG ;
			goto exit ;
		} ;
	} ;

	m_poFieldManager = ai_poFieldManager ;
	if ( ai_Action==PI )
	{
		if (!m_poFieldManager) {
			/*## Crear nuevo error*/
			SwOk=M4_FALSE ;
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_270 ] ) << SEND_MSG ;
			goto exit ;
		} ;
	} ;

exit:
	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; 

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, 
			"ClENG_TkSysInfo::Init() Creating ClENG_TkSysInfo , Action %0:s") 
			<< m_Prop[m_Action].m_pcTraceName << CRAZY_MSG << SEND_MSG ;
	};
	#endif

	return SwOk ;
}

void ClENG_TkSysInfo::GetTraceString(ClMIT_Str *ao_Str) 
{
	ao_Str->StrCpy("ClENG_TkSysInfo Action <") ;
	ao_Str->StrCat( m_Prop[m_Action].m_pcTraceName ) ;
	ao_Str->StrCat(">") ;
}

// Indica si están activas la pruebas unitarias. En tal caso se actual el valor de fecha
// que se debe utilizar
m4bool_t ClENG_TkSysInfo::DateForTesting(m4date_t *ai_Date)
{
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	m4bool_t isM4UnitActive = TaskEnv->m_APIChn.m_poMVC->GetSessChn()->M4UnitActive();
	if ( M4_TRUE == isM4UnitActive)
	{
		*ai_Date = ClStringToDate("2000-01-01 12:00:00");
	}
	return isM4UnitActive;
}

void ClENG_TkSysInfo::EvaluateThis(ClMIT_ChnValue *ao_Value) 
{
	ClMIT_Str osAux, osAux2;
	m4char_t *PChar;

	size_t iMax, iMin, i;
	size_t iCharNum = 0;
	size_t iFirstChar = 0;
	size_t iLastChar = 0;
	size_t iCharSize = 0;
	m4int32_t lVal;
	m4char_t cChar;
	ClENG_Format*	poFormatAux;
	ClENG_DBFormat* poDBFormatAux;
	FORMAT_DATA_TYPE poType;
	m4date_t Date ;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	
	switch (m_Action) {
	case GET_CURRENT_DATE:
		if ( M4_TRUE == DateForTesting(&Date) )
		{
			ao_Value->InitDate(Date);
		}
		else
		{
			ao_Value->InitDate(ClMIT_Dates::ActualDate(NULL,NULL,M4CL_TIME_LOCAL_TIME));
		}
		break ;

	case GET_GMT_DATE:
		if ( M4_TRUE == DateForTesting(&Date) )
		{
			ao_Value->InitDate(Date);
		}
		else
		{
			ao_Value->InitDate(ClMIT_Dates::ActualDate(NULL,NULL,M4CL_TIME_GM_TIME));
		}
		break ;

	case TODAY:
		if ( M4_TRUE == DateForTesting(&Date) )
		{
			ClMIT_Dates::DateToDateStr(&osAux, Date);
		}
		else
		{
			ClMIT_Dates::ActualDate(&osAux,NULL,M4CL_TIME_LOCAL_TIME) ;
		}
		ao_Value->InitString(osAux); 
		break;

	case GET_CURRENT_TIME:
		if ( M4_TRUE == DateForTesting(&Date) )
		{
			ClMIT_Dates::DateToTimeStr(&osAux, Date);
		}
		else
		{
			ClMIT_Dates::ActualTime(&osAux,NULL,M4CL_TIME_LOCAL_TIME) ;
		}
		ao_Value->InitString(osAux); 
		break;

	case GET_CURRENT_TS:
		if ( M4_TRUE == DateForTesting(&Date) )
		{
			ClMIT_Dates::DateToTimeStampStr(&osAux, Date);
		}
		else
		{
			ClMIT_Dates::ActualTimeStamp(&osAux,NULL,M4CL_TIME_LOCAL_TIME) ;
		}
		ao_Value->InitString(osAux); 
		break;

	case GET_GMT_TIME:
		if ( M4_TRUE == DateForTesting(&Date) )
		{
			ClMIT_Dates::DateToTimeStr(&osAux, Date);
		}
		else
		{
			ClMIT_Dates::ActualTime(&osAux,NULL,M4CL_TIME_GM_TIME) ;
		}
		ao_Value->InitString(osAux); 
		break;

	case GET_GMT_TS:
		if ( M4_TRUE == DateForTesting(&Date) )
		{
			ClMIT_Dates::DateToTimeStampStr(&osAux, Date);
		}
		else
		{
			ClMIT_Dates::ActualTimeStamp(&osAux,NULL,M4CL_TIME_GM_TIME) ;
		}
		ao_Value->InitString(osAux); 
		break;

	case TO_DATE:
		ClMIT_Dates::DateToDateStr(&osAux,GetPointerToSonValue(0)->GetDate()) ;
		ao_Value->InitString(osAux); 
		break;

	case TO_TIME:
		ClMIT_Dates::DateToTimeStr(&osAux,GetPointerToSonValue(0)->GetDate()) ;
		ao_Value->InitString(osAux); 
		break;

	case TO_TS:
		ClMIT_Dates::DateToTimeStampStr(&osAux,GetPointerToSonValue(0)->GetDate()) ;
		ao_Value->InitString(osAux); 
		break;
	
	case IIF:
		EvaluateSon(0) ;
		if ( (GetPointerToSonValue(0))->GetBoolean() ) {
			EvaluateSon(1) ;
			ao_Value->Init(GetPointerToSonValue(1));
		} else {
			EvaluateSon(2) ;
			ao_Value->Init(GetPointerToSonValue(2));
		} ;
		break;

	case NVL:
		EvaluateSon(0) ;
		if ( !(GetPointerToSonValue(0))->IsNULL() ) {
			ao_Value->Init(GetPointerToSonValue(0));
		} else {
			EvaluateSon(1) ;
			ao_Value->Init(GetPointerToSonValue(1));
		} ;
		break;

	case STRING_F:
		iMax = (size_t)GetPointerToSonValue(1)->GetDouble();
		osAux.StrCpy("") ;
		
		for( i=0; i<iMax; i++ )
		{
			osAux.StrCat(GetPointerToSonValue(0)->GetString());
		}
		ao_Value->InitString(osAux);
		break;

	case SPACE_F:

		osAux.StrCpy("") ;
		osAux.StrDuplicateChar(0,(m4int16_t)GetPointerToSonValue(0)->GetDouble(),' ') ;
		ao_Value->InitString(osAux);
		break;

	case STR_IN:
		PChar = ClMIT_Str::StrStr(GetPointerToSonValue(0)->GetString(),GetPointerToSonValue(1)->GetString());
		if(PChar!=NULL) {
			ao_Value->InitDouble(PChar - GetPointerToSonValue(0)->GetString()+1, 0, 0);
		} else {
			ao_Value->InitDouble(0,0,0);
		} ;
		break;

	case STR_NIN:
		PChar = ClMIT_Str::StrIStr(GetPointerToSonValue(0)->GetString(),GetPointerToSonValue(1)->GetString());
		if(PChar!=NULL) {
			ao_Value->InitDouble(PChar - GetPointerToSonValue(0)->GetString()+1, 0, 0);
		} else {
			ao_Value->InitDouble(0,0,0);
		} ;
		break;


	case MID:
		
		iMin = (size_t)GetPointerToSonValue(1)->GetDouble();
		iMax = (size_t)GetPointerToSonValue(2)->GetDouble();

		// Bug 0281724. UNICODE.
		PChar = GetPointerToSonValue(0)->GetString();
		iCharNum = M4StrLen(PChar, -1);

		// Bug 0142912.
		if (iMin > iCharNum)
		{
			// Bug 0280464. Cuando llega una cadena vacía (""), da error porque el min es mayor que
			// cero siempre. En este caso, no se devuelve error, y simplemente te devuelve una cadena 
			// vacia.
			// *m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_250 ] ) << SEND_MSG ;
			ao_Value->InitString("");
			break;
		}
		else if (iMax > iCharNum)
		{
			// En el caso en el que el Max (caracter hasta el que quieres quedarte) 
			// el Mid sea mayor que la longitud de la cadena, se devuelve la cadena
			// tal cual.
			ao_Value->InitString(PChar);
			break;
		}
		else if (iMin > iMax)
		{
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_260 ] ) << SEND_MSG ;
			break;	
		}
		else
		{
			iFirstChar = M4CharPosition(PChar, -1, iMin, iCharSize);
			iLastChar = M4CharPosition(PChar, -1, iMax, iCharSize);

			osAux.ToASCIZ(PChar + iFirstChar - 1, iLastChar - iFirstChar + 1);
			ao_Value->InitString(osAux);

			break;
		};

	case TO_LONG:

		ao_Value->InitDouble(0,0,0);
		ao_Value->InitAnonymous(GetPointerToSonValue(0)->GetString()) ;
		ao_Value->SetScale(0);

		break;

	case TO_DOUBLE:

		ao_Value->InitDouble(0,0,0);
		ao_Value->InitAnonymous(GetPointerToSonValue(0)->GetString()) ;
		ao_Value->SetScale(4);

		break;

	case TO_PRECISION_DOUBLE:

		ao_Value->InitDouble(0,0,0);
		ao_Value->InitAnonymous(GetPointerToSonValue(0)->GetString()) ;
		ao_Value->SetScale(GetPointerToSonValue(1)->GetDouble());

		break;

	case CHAR_F:

		osAux.StrCpy("") ;
		osAux.StrInsertChar(ClMIT_Str::STR_CAT,1,GetPointerToSonValue(0)->GetDouble()) ;
		ao_Value->InitString(osAux);

		break;	

	case EXCH_ITEM_CURR:
		ao_Value->InitCurrency( 
			GetPointerToSonValue(0)->GetCurrencyValue(), 
			GetPointerToSonValue(0)->GetPrecision(),
			GetPointerToSonValue(0)->GetScale(),
			GetPointerToSonValue(0)->GetCurrencyType(),
			GetPointerToSonValue(2)->GetDate(), 
			GetPointerToSonValue(3)->GetString());
		ao_Value->ExchangeCurrency( GetPointerToSonValue(1)->GetString() );
		break;


	case EXCH_CURRENCY:

		ao_Value->InitCurrency( 
			GetPointerToSonValue(0)->GetDouble(), 
			GetPointerToSonValue(0)->GetPrecision(),
			GetPointerToSonValue(0)->GetScale(),
			GetPointerToSonValue(1)->GetString(),
			GetPointerToSonValue(3)->GetDate(), 
			GetPointerToSonValue(4)->GetString() );
		ao_Value->ExchangeCurrency( GetPointerToSonValue(2)->GetString() );
		break;

	case EXCH_DEF_CURRENCY:

		ao_Value->InitCurrency( 
			GetPointerToSonValue(0)->GetDouble(), 
			GetPointerToSonValue(0)->GetPrecision(),
			GetPointerToSonValue(0)->GetScale(),
			GetPointerToSonValue(1)->GetString(), 
			ClMIT_ChnValue::GetDefExchDate(), 
			ClMIT_ChnValue::GetDefExchType() );

		//Si no se ha definido moneda por defecto o coincide con la origen, no se realiza el cambio
		if(	m_poExeParams->m_oDestCurrency.StrLen() ) {
			if ( ClMIT_Str::StrICmpTrim(m_poExeParams->m_oDestCurrency, 
				GetPointerToSonValue(1)->GetString())!=0)
			{
				ao_Value->ExchangeCurrency( ClMIT_ChnValue::GetDefExchCurr() );
			}
		} ;
		break;

	case DEF_EXCHANGE:

		//Buscamos los valores por defecto
		Date = ClMIT_ChnValue::GetDefExchDate() ;
		osAux.StrCpy(ClMIT_ChnValue::GetDefExchType());
		
		if (!osAux.StrLen()){
			osAux.StrCpy(GetPointerToSonValue(0)->GetCurrencyExchType());
		}; 

		ao_Value->InitCurrency( 
			GetPointerToSonValue(0)->GetCurrencyValue(), 
			GetPointerToSonValue(0)->GetPrecision(),
			GetPointerToSonValue(0)->GetScale(),
			GetPointerToSonValue(0)->GetCurrencyType(),
			Date, 
			osAux.InChar(0));

		if(!osAux.StrLen() || ClMIT_Dates::IsInfinite(Date) ){
			ao_Value->Init(GetPointerToSonValue(0)) ;
		} else if(m_poExeParams->m_oDestCurrency.StrLen()){
			ao_Value->ExchangeCurrency( ClMIT_ChnValue::GetDefExchCurr() );
		};

		break;

	case GET_DEFAULT_CURRENCY:

		if(m_poExeParams->m_oDestCurrency){
			ao_Value->InitString(m_poExeParams->m_oDestCurrency);
		};
		break;

	case GET_DEFAULT_DATE:

		if(m_poExeParams->m_dExchangeDate){
			ao_Value->InitDate(m_poExeParams->m_dExchangeDate);
		};
		break;
	
	case GET_DEFAULT_TYPE:

		if(m_poExeParams->m_oExchangeType){
			ao_Value->InitString(m_poExeParams->m_oExchangeType);
		};
		break;

	case IS_NULL:

		ao_Value->InitBoolean( GetPointerToSonValue(0)->IsNULL() );
		
		break;

	case STR_LEN:
		
		ao_Value->InitDouble(strlen(GetPointerToSonValue(0)->GetString()), 8, 0);
		
		break;

	case CSTR:

		ClMIT_ChnValue::GetInputFormatDouble(GetPointerToSonValue(0)->GetDouble(), 
			20, GetPointerToSonValue(1)->GetDouble(),&osAux ) ;
		ao_Value->InitString(osAux);

		break;

	case ALIGN:
		
		iMax = (size_t)GetPointerToSonValue(1)->GetDouble();
		
		if(ClMIT_Str::StrICmpTrim(GetPointerToSonValue(2)->GetString(), "RIGHT")==0){
			osAux.StrAlign(GetPointerToSonValue(0)->GetString(),iMax,ClMIT_Str::STR_RIGHT);
		} else if(ClMIT_Str::StrICmpTrim(GetPointerToSonValue(2)->GetString(), "LEFT")==0){
			osAux.StrAlign(GetPointerToSonValue(0)->GetString(),iMax,ClMIT_Str::STR_LEFT);
		} else if(ClMIT_Str::StrICmpTrim(GetPointerToSonValue(2)->GetString(), "CENTER")==0){
			osAux.StrAlign(GetPointerToSonValue(0)->GetString(),iMax,ClMIT_Str::STR_CENTER);
		} else {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_280 ] ) 
				<< GetPointerToSonValue(2)->GetString() << SEND_MSG ;
			osAux.StrCat(GetPointerToSonValue(0)->GetString());
		};
		ao_Value->InitString(osAux);

		break;

	case FORMAT_EXPR:
		{

		FORMAT_DATA_TYPE FormatType ;

		ao_Value->Init(GetPointerToSonValue(0));

		poFormatAux = m_poDBFormat->FindFormat( 
			(m4uint32_t)GetPointerToSonValue(1)->GetDouble(),ClMIT_Trace::PROPAGATE_ERROR) ;
		if (!poFormatAux) {
			/**m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_290 ] ) 
				<< GetPointerToSonValue(1)->GetDouble() << SEND_MSG ;*/
			break ;
		} ;
			
		if ( GetPointerToSonValue(0)->GetInputFormat(&osAux,&FormatType) ) {
			poFormatAux->Apply( osAux, FormatType, &osAux2 );
			ao_Value->InitString(osAux2.InChar(0));
		} else {
			ao_Value->InitString(osAux) ;
		} ;

		} ;

		break;

	case FORMAT_STR:

		ao_Value->Init(GetPointerToSonValue(0));
		
		if(!(poFormatAux = m_poDBFormat->FindFormat(
			GetPointerToSonValue(2)->GetDouble(),ClMIT_Trace::PROPAGATE_ERROR))){
			//*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_290 ] ) << SEND_MSG ;
		} else {
			if( ClMIT_Str::StrICmpTrim(GetPointerToSonValue(1)->GetString(),"STRING")==0 ){
					poFormatAux->Apply( GetPointerToSonValue(0)->GetString(), FORMAT_STRING, &osAux );
			} else if( ClMIT_Str::StrICmpTrim(GetPointerToSonValue(1)->GetString(),"DATE")==0){
					poFormatAux->Apply( GetPointerToSonValue(0)->GetString(), FORMAT_DATE, &osAux );
			} else if( ClMIT_Str::StrICmpTrim(GetPointerToSonValue(1)->GetString(),"NUMERIC")==0){
					poFormatAux->Apply( GetPointerToSonValue(0)->GetString(), FORMAT_DOUBLE, &osAux );
			} else if( ClMIT_Str::StrICmpTrim(GetPointerToSonValue(1)->GetString(),"FLOAT")==0){
					poFormatAux->Apply( GetPointerToSonValue(0)->GetString(), FORMAT_DOUBLE, &osAux );
			} else if( ClMIT_Str::StrICmpTrim(GetPointerToSonValue(1)->GetString(),"TIME")==0){
					poFormatAux->Apply( GetPointerToSonValue(0)->GetString(), FORMAT_DATE, &osAux );
			} else if( ClMIT_Str::StrICmpTrim(GetPointerToSonValue(1)->GetString(),"BINARY")==0){
					poFormatAux->Apply( GetPointerToSonValue(0)->GetString(), FORMAT_DOUBLE, &osAux );
			} else if( ClMIT_Str::StrICmpTrim(GetPointerToSonValue(1)->GetString(),"BIT")==0){
					poFormatAux->Apply( GetPointerToSonValue(0)->GetString(), FORMAT_DOUBLE, &osAux );
			} else if( ClMIT_Str::StrICmpTrim(GetPointerToSonValue(1)->GetString(),"ERROR")==0){
					poFormatAux->Apply( GetPointerToSonValue(0)->GetString(), FORMAT_DOUBLE, &osAux );
			} else if( ClMIT_Str::StrICmpTrim(GetPointerToSonValue(1)->GetString(),"TIME_STAMP")==0){
					poFormatAux->Apply( GetPointerToSonValue(0)->GetString(), FORMAT_TIME_STAMP, &osAux );
			} else {
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_310 ] ) 
					<< GetPointerToSonValue(1)->GetString() << SEND_MSG ;
				osAux.StrCpy(GetPointerToSonValue(0)->GetString());
			};
			ao_Value->InitString(osAux.InChar(0));
		};

		break;

	case FORMAT_DEF_CUR:

		if( ClMIT_Str::StrICmpTrim((m_poExeParams->m_oDestCurrency).InChar(0),"")==0) {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_320 ] ) << SEND_MSG ;
			ao_Value->Init(GetPointerToSonValue(0));
			break;
		}else{
			ao_Value->InitString(m_poExeParams->m_oDestCurrency);
		};

		
		if(!m_poDBFormat->GetCurDefaultFormat(ao_Value->GetString(), M4_TRUE, ClMIT_Trace::PROPAGATE_ERROR)){
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_330 ] ) << SEND_MSG ;
			ao_Value->Init(GetPointerToSonValue(0));
			break;
		}else{
			poFormatAux = m_poDBFormat->GetCurDefaultFormat(ao_Value->GetString());
			ao_Value->Init(GetPointerToSonValue(0));
			if(ao_Value->GetType()!=ClMIT_ChnValue::NUMERIC){
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_340 ] ) << SEND_MSG ;
				ao_Value->Init(GetPointerToSonValue(0));
				break;
			};					
			ao_Value->GetInputFormat(&osAux, &poType);
			poFormatAux->Apply( osAux, poType, &osAux2 );
			ao_Value->InitString( osAux2.InChar(0) );
		};

		break;

	case SAVE_ITEM:
		{
		ClMIT_ChnNode *Node ;
		ClMIT_ChnItem Item ;

		ao_Value->Init(GetPointerToSonValue(2)) ;
		if ( !( Node = m_poChn->GetNode(GetPointerToSonValue(0)->GetString(), ClMIT_Trace::PROPAGATE_ERROR ) ) ) {
			break ;
		} ;
		if ( Item.Init( Node, GetPointerToSonValue(1)->GetString(), ClMIT_Trace::PROPAGATE_ERROR )!=ClMIT_ChnItem::ITEM_OK ) 	{
			break ;
		} ;
		Item.m_oValue.Init(ao_Value) ;
		Item.Save() ;
		};
		break ;

	case PI:
		//----------- MANU -------------
		//Esta expresion solo puede ser llamada desde Print_Pi, que activa el m_oActiveValue con el valor adecuado
		// en cada iteracion del bucle. Al hacer la referencia a %PI(), esta carga el valor activo y se lo devuelve
		// a Print_Pi para resolver el field pendiente a nivel de Device
		if(m_poFieldManager->IsValueActivated()){
			ao_Value->Init(&(m_poFieldManager->m_oActiveValue)) ;
		} else {
			//ERROR LOCAL
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_410 ] ) << SEND_MSG ;
		};

		break;

	case DEFAULT_EXCHANGE2:
		{
		//Esta expresion es similar a la DEF_EXCHANGE, solo que la otra primero trinca los valores por defecto, y si no
		// hay trinca los del Item. Esta hace al reves, si no tiene el Item coge por defecto
		Date=GetPointerToSonValue(0)->GetCurrencyExchDate() ;
		osAux.StrCpy(GetPointerToSonValue(0)->GetCurrencyExchType());

		//Si no los hay trincamos los valores por defecto para el Report
		if(!osAux.StrLen()){
			osAux.StrCpy(ClMIT_ChnValue::GetDefExchType());	
		}; 

		if(ClMIT_Dates::IsInfinite(Date)) {
			Date = ClMIT_ChnValue::GetDefExchDate() ;
		};

		//Inicializamos la moneda a cambiar
		ao_Value->InitCurrency( 
			GetPointerToSonValue(0)->GetCurrencyValue(), 
			GetPointerToSonValue(0)->GetPrecision(), 
			GetPointerToSonValue(0)->GetScale(), 
			GetPointerToSonValue(0)->GetCurrencyType(),
			Date,
			osAux.InChar(0));

		//Si existe la moneda por defecto cambiamos, sino se queda con el valor anterior
		if(m_poExeParams->m_oDestCurrency.StrLen())
			ao_Value->ExchangeCurrency( ClMIT_ChnValue::GetDefExchCurr() );
		};
		break;

	case EXCHANGE_ITEM_CUR2:
		{
		//Se inicializan con los valores de origen y si no se toman los defectos que se introduzcan en la expresion
		Date=GetPointerToSonValue(0)->GetCurrencyExchDate() ;
		osAux.StrCpy(GetPointerToSonValue(0)->GetCurrencyExchType());
		
		//Si no los hay trincamos los valores que nos han pasado
		if(!osAux.StrLen()){
			osAux.StrCpy(GetPointerToSonValue(3)->GetString());	
		}; 

		if(ClMIT_Dates::IsInfinite(Date)){
			Date=GetPointerToSonValue(2)->GetDate() ;
		};

		ao_Value->InitCurrency( 
			GetPointerToSonValue(0)->GetCurrencyValue(), 
			GetPointerToSonValue(0)->GetPrecision(), 
			GetPointerToSonValue(0)->GetScale(), 
			GetPointerToSonValue(0)->GetCurrencyType(),
			Date,
			osAux.InChar(0));
		ao_Value->ExchangeCurrency( GetPointerToSonValue(1)->GetString() );

		};
		break;

	case GET_CURRENCY_VALUE:

		ao_Value->InitDouble(GetPointerToSonValue(0)->GetCurrencyValue(),20,2) ;

		break;

	case ORGANIZATION_TYPE:

 		ao_Value->InitDouble(m_poChn->GetOrganizationType(),2,0) ;

		break;

	case ORGANIZATION:

		ao_Value->InitString( m_poChn->GetOrganization() ) ;

		break;

	};
}

//=========================================================================>ClENG_Expression

ClENG_Expression::ClENG_Expression() 
{
	m_poInsp=GET_INSP();
	m_poRoot = NULL ;
	
}

void ClENG_Expression::End() 
{
	// Borra todo el arbol

	M4_DELETE_VIRTUAL_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, m_poRoot ) ;
	m_oExpr.End() ;
	m_oTraceId.End() ;
}

void ClENG_Expression::Init(const m4char_t *ai_pcExpr, const m4char_t *ai_TraceId) 
{
	ClENG_Expression::End() ;

	m_oExpr.StrCpy(ai_pcExpr) ;
	m_oTraceId.StrCpy(ai_TraceId) ;
}

void ClENG_Expression::ChangeExpression(const m4char_t *ai_pcExpr) 
{
	ClMIT_Str strAux ;

	strAux.StrCpy(m_oTraceId) ;
	Init(ai_pcExpr,strAux) ;
}


void ClENG_Expression::PreAnalex(ClENG_Token *ai_poRoot) 
{
	M4_DELETE_VIRTUAL_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, m_poRoot ) ;
	m_poRoot=ai_poRoot ;
}

void ClENG_Expression::Trace() 
{
	#ifdef RPT_ENGINE_TRACE
	
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){		
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Expression::Trace() type %0:s  expression %1:s", ClMIT_Msg::ENTRY_FORMAT) 
			<< GetTraceId() << GetExpressionStr() << CRAZY_MSG << SEND_MSG   ;
		if (!IsEmpty()) {
			m_poRoot->Trace() ;
		} else {
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"There aren't tokens") << CRAZY_MSG << SEND_MSG   ;
		} ;
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Expression::Trace() %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< GetExpressionStr() << CRAZY_MSG << SEND_MSG   ;
	};
	#endif
}

m4bool_t ClENG_Expression::Evaluate(ClMIT_ChnValue *ao_poValue, 
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t SwOk=M4_TRUE ;
	m4uint32_t OkCount ;
	m4bool_t swTrace=M4_FALSE ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped() && 
		m_poInsp->m_poTrace->GetGroupDetail(GROUP_ID)>=ClMIT_Msg::DETAILED ) 
	{
		swTrace=M4_TRUE ;
	} 
	#endif

	*m_poInsp->m_poTrace << ClMIT_PushLoc( &TRACE_ARRAY[ ANALEX_MSG_5000 ] ) 
		<< GetTraceId() << GetExpressionStr() << END_LOC ;

	if(swTrace){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "ClENG_Expression::Evaluate() type %0:s expression %1:s", ClMIT_Msg::ENTRY_FORMAT) 
			<< GetTraceId() << GetExpressionStr()
			<< DETAILED_MSG << SEND_MSG   ;
	};

	if (!IsEmpty()) {
		OkCount=m_poInsp->m_poTrace->GetOkCount() ;	
		m_poRoot->Evaluate(ao_poValue) ;
		SwOk = M4_BOOL( OkCount==m_poInsp->m_poTrace->GetOkCount() ) ;	
	} else {
		//m_poInsp->m_poTrace->PutMsg( &TRACE_ARRAY[ ANALEX_MSG_180 ], "ClENG_Expression::Evaluate()") ;
		SwOk=M4_TRUE ;
		//goto exit ;
	} ;

exit:
	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; 


	if (!SwOk) {

		if(swTrace){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "ClENG_Expression::Evaluate() Evaluate failed", 
				ClMIT_Msg::EXIT_FORMAT) << DETAILED_MSG << SEND_MSG   ;
		};
		//m_poInsp->m_poTrace->PutExtendMsg( &TRACE_ARRAY[ ANALEX_MSG_430 ], "ClENG_Expression::Evaluate()",
		//	NULL, GetExpressionStr() ) ;
		;
	
	} else {

		if(swTrace){
			if ( !IsEmpty() )
			{
				ClMIT_Str Str ;
				ao_poValue->GetTraceValue( &Str) ; 
				*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "ClENG_Expression::Evaluate() "
					"Result is %0:s for type %1:s expression %2:s", ClMIT_Msg::EXIT_FORMAT) 
					<< Str.InChar(0) 
					<< GetTraceId() << GetExpressionStr()
					<< DETAILED_MSG << SEND_MSG   ;
			} else {
				*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "ClENG_Expression::Evaluate() "
					"%0:s There isn't return value", ClMIT_Msg::EXIT_FORMAT) 
					<< GetExpressionStr() << DETAILED_MSG << SEND_MSG   ;
			};
		};
	} ;

	*m_poInsp->m_poTrace << POP_LOC ;
	
	return SwOk ;
}

void ClENG_Expression::ResetExternProp( ClMIT_ItemExternProp * ai_poExternProp ) 
{
	if (!IsEmpty()) {
		m_poRoot->ResetExternProp( ai_poExternProp ) ;
	} ;
}

m4bool_t ClENG_Expression::IsExpressionStrEmpty()  
{
	return M4_BOOL(m_oExpr.StrLenTrim()==0);
}


//=========================================================================>ClENG_Analex

ClENG_Analex::StExpressionEnv::StExpressionEnv()
{
	m_poChn = NULL ;
	m_poDefaultNode = NULL ;	
	m_LoadMode = ClMIT_ChnItem::ITEM_LOAD ;
	m_poItemExternProp = NULL ;
	m_poFieldExternProp = NULL ; 
	m_poFieldManager = NULL ;
	m_poReport = NULL ;
	m_poSect = NULL ;
	m_poDependencies=NULL;
	m_poDvc = NULL ;
	m_poExeParams = NULL ;
	m_poDBFormat = NULL ;
	m_poTrLit=NULL ;
}


ClENG_Analex::ClENG_Analex() 
{
	m_poInsp=GET_INSP();
	m_poExpr=NULL ;
}

void ClENG_Analex::End() 
{
	TokenList_t::iterator Item, End ;

	m_poExpr=NULL ;

	End=m_oTokenList.end() ;	
	for ( Item=m_oTokenList.begin(); Item != End; Item++ ) {
		M4_DELETE_VIRTUAL_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, *Item ) ;
	} ;
	m_oTokenList.clear() ;
}

m4bool_t ClENG_Analex::Init(
	StExpressionEnv * ai_pstEnv, ClENG_Expression *ai_poExpr,
	ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t SwOk=M4_TRUE ;
	
	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		if (!ai_poExpr->IsExpressionStrEmpty()) {
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "ClENG_Analex::Init()", ClMIT_Msg::ENTRY_FORMAT) 
				<< DETAILED_MSG << SEND_MSG   ;
		};
	} ;
	#endif

	ClENG_Analex::End() ;

	*m_poInsp->m_poTrace << ClMIT_PushLoc( &TRACE_ARRAY[ ANALEX_MSG_5010 ] ) 
		<< ai_poExpr->GetExpressionStr() << END_LOC ;

	m_poExpr = ai_poExpr ;

	if ( !m_poExpr->IsExpressionStrEmpty() ) {

		SwOk = AnaLex(m_poExpr->GetExpressionStr(),ai_pstEnv) ;
		if (!SwOk) goto exit ;

		if(!ai_pstEnv->m_poDependencies) {
			SwOk = AnaSintax() ;	//Inicializa m_poExpr
			if (!SwOk) goto exit ;

			m_poExpr->ResetExternProp( ai_pstEnv->m_poItemExternProp ) ;

			#ifdef RPT_ENGINE_TRACE
			if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
				m_poExpr->Trace() ;
			};
			#endif
		};

	} else {

		m_poExpr->PreAnalex(NULL) ;

	};
exit:
	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; 

	//if (!SwOk) {
	//	m_poInsp->m_poTrace->PutExtendMsg( &TRACE_ARRAY[ ANALEX_MSG_440 ], 
	//		"ClENG_Analex::Init()", NULL, m_poExpr->GetExpressionStr() ) ;
	//} ;
	
	ClENG_Analex::End() ;	//Vacia lista auxiliar

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		if (!ai_poExpr->IsExpressionStrEmpty()) {
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "ClENG_Analex::Init()", ClMIT_Msg::EXIT_FORMAT) 
				<< DETAILED_MSG << SEND_MSG   ;
		};
	} ;
	#endif

	*m_poInsp->m_poTrace << POP_LOC ;
		
	return SwOk ;		
}

inline const m4char_t * ClENG_Analex::NextString(const m4char_t *ai_pcStr) 
{
	return ai_pcStr+strspn(ai_pcStr, ANALEX_BLANK_STR) ;
}

ClENG_Analex::CHARACTER_TYPE ClENG_Analex::GetCharType( m4char_t ai_cChar ) 
{
	CHARACTER_TYPE Return ;

	if (ai_cChar==M4_END_STR) {
		Return=END_STRING ;
	} else if (ai_cChar==*ANALEX_INTRA_TOKEN) {
		Return=INTRA_TOKEN_SEPARATOR ;
	} else {
		if ( strchr(ANALEX_BLANK_STR,ai_cChar)  ) Return=TOKEN_SEPARATOR ;
		else Return=TOKEN_CHAR ;
	} ;
	return Return ;
}



void ClENG_Analex::PrepareForAnalexCT( 
	m4char_t *ai_pcExpr, m4char_t *ai_pcIndexInfo , 
	ClMIT_Str* ai_poBuffer, m4bool_t ai_bSwDependencies )
{
	m4bool_t SwOk = M4_TRUE;
	ClMIT_Str oStrAux, StrCTId;
	m4char_t  *Pos, *Beg, *End, *Expr, *BegCT, *BegPI ;
	StMIT_Inspector* ai_poInsp = GET_INSP() ;
	m4int16_t LevelIndex ;
	m4bool_t DoSomething=M4_FALSE ;
	ClMIT_Args IndexArg ;

	IndexArg.Init(ai_pcIndexInfo,'#',"_") ;

	Expr=ai_pcExpr;	//ai_pcExpr no crece ni decrece en este metodo!!

	ai_poBuffer->StrCpy("") ;

	for (Pos=Expr; *Pos!=M4_END_STR;) {
		//Poner . para que no se confunda con $ct_page
		BegCT= ClMIT_Str::StrIStr(Pos,"$CT.") ;
		BegPI= ClMIT_Str::StrIStr(Pos,"%Print_pi.")  ;

		if (BegCT && BegPI) {
			//Debe quedarse con el que sale antes en la cadena, o se comera al otro
			if (BegCT>BegPI) {
				BegCT=NULL ;
			} else {
				BegPI=NULL ;
			};
		} ;
		
		if ( (BegCT) ) {

			Beg=BegCT ;

			DoSomething=M4_TRUE ;
			oStrAux.ToASCIZ(Pos,Beg-Pos) ;
			ai_poBuffer->StrCat(oStrAux) ;	

			//Obtiene IdCT
			Beg+=strlen("$CT")+1 ;	//+1 por '.'
			End=strchr(Beg,*ANALEX_INTRA_TOKEN) ;
			if (!End) {
				*ai_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_230 ] ) << SEND_MSG ;
				SwOk=M4_FALSE;
				goto exit;
			} ;
			oStrAux.ToASCIZ(Beg,End-Beg) ;
			StrCTId.StrCpy(oStrAux) ;

			Beg +=End-Beg+1 ;	//+1, salta el '.'

			//Obtiene IdLevel
			Beg += oStrAux.GetString(ClMIT_Str::WHILE_CHAR, Beg, ANALEX_LITERAL_NUMERIC ) ;
			if (!ai_bSwDependencies) {

				LevelIndex = atoi(oStrAux)-1 ; //Ajuste level 1-n a 0-n
				if ( IndexArg.GetArgCount() < LevelIndex+1 ) {
					/*##Num error*/
					*ai_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID,
						"Level %0:s for CT %1:s out of range", ClMIT_Msg::MIXED_OUT) 
						<< LevelIndex << (char*)StrCTId << SEND_MSG ;
					SwOk=M4_FALSE;
					goto exit;
				} ;
				LevelIndex = IndexArg.GetLong(LevelIndex,0) ;

				oStrAux.Accommodate(20) ;	//Mas que suficiente para un entero
				sprintf(oStrAux.InChar(0),"%i", (int)LevelIndex ) ;
				oStrAux.OverStringValid() ;	

			} else {
			
				oStrAux.StrCpy("00") ;

			} ;

			ai_poBuffer->StrCat(oStrAux);
			ai_poBuffer->StrCat(ANALEX_INTRA_TOKEN);

			Pos=Beg ;

		} else  if ( ( BegPI ) ) {

			Beg=BegPI ;

			DoSomething=M4_TRUE ;
			oStrAux.ToASCIZ(Pos,Beg-Pos) ;
			ai_poBuffer->StrCat(oStrAux) ;

			//Obtiene IdPi
			Beg+=strlen("%Print_pi.") ;	
			End=strchr(Beg,ANALEX_OPEN) ;
			if (!End) {
				*ai_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_240 ] ) << SEND_MSG ;
				SwOk=M4_FALSE;
				goto exit;
			} ;
			oStrAux.ToASCIZ(Beg,End-Beg) ;
			Beg +=End-Beg ;	//Seguira en el (

			ai_poBuffer->StrCat("%PRINT_PI.");
			ai_poBuffer->StrCat(oStrAux);
			ai_poBuffer->StrCat("_");
			ai_poBuffer->StrCat(ai_pcIndexInfo);

			Pos=Beg ;

		} else Beg=NULL ;

		if (!Beg) {
			ai_poBuffer->StrCat(Pos) ;
			Pos += strlen(Pos) ;
		} ;

	} ;

exit:
	if (!SwOk) {

		//Error local

		ai_poBuffer->StrCpy("Error in ClENG_Analex::PrepareForAnalexCT()!!!") ;

		#ifdef RPT_ENGINE_TRACE
		if(!ai_poInsp->m_poTrace->IsTraceCodeRipped()){
			*ai_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
				"ClENG_Analex::PrepareForAnalexCT() Unable To Prepare this expression" ) 
				<< CRAZY_MSG << SEND_MSG   ;
		};
		#endif

	} ;

	#ifdef RPT_ENGINE_TRACE
	if(!ai_poInsp->m_poTrace->IsTraceCodeRipped()){
		if ( DoSomething ) {
			*ai_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
				"ClENG_Analex::PrepareForAnalexCT() Input expression %0:s, ouput expression %1:s" ) 
				<< ai_pcExpr << ai_poBuffer->InChar(0) << DETAILED_MSG << SEND_MSG   ;
		};
	} ;
	#endif
}


void ClENG_Analex::PrepareForAnalexField( ClMIT_Str *ai_poBuffer, StExpressionEnv* ai_pstEnv)
{
	StMIT_Inspector* ai_poInsp = GET_INSP() ;
	ClMIT_Str ai_oExpr ;
	m4bool_t SwOk = M4_TRUE;
	ClMIT_Str oStrAux;
	m4char_t  *Pos, *Beg, *End, *Expr;
	m4bool_t DoSomething=M4_FALSE ;

	ai_oExpr.StrCpy(ai_poBuffer) ;

	Expr=ai_oExpr.InChar(0);	//ai_poExpr no crece ni decrece en este metodo!!

	ai_poBuffer->StrCpy("") ;

	for (Pos=Expr; *Pos!=M4_END_STR;) {
		if ( ( Beg=ClMIT_Str::StrIStr(Pos,"$CT_PAGE") ) ) {

			DoSomething=M4_TRUE ;
			oStrAux.ToASCIZ(Pos,Beg-Pos) ;
			ai_poBuffer->StrCat(oStrAux) ;
			Beg+=strlen("$CT_PAGE") ;	

			oStrAux.Accommodate(20) ;	//Mas que suficiente para un entero
			sprintf( oStrAux.InChar(0),"%i",(int)ai_pstEnv->m_iLayoutPage) ;
			oStrAux.OverStringValid() ;
			ai_poBuffer->StrCat(oStrAux);

			Pos=Beg ;
		} ;

		if (!Beg) {
			ai_poBuffer->StrCat(Pos) ;
			Pos += strlen(Pos) ;
		} ;

	} ;

exit:
	if (!SwOk) {

		//Error local

		ai_poBuffer->StrCpy("Error in ClENG_Analex::PrepareForAnalexField()!!!") ;

		#ifdef RPT_ENGINE_TRACE
		if(!ai_poInsp->m_poTrace->IsTraceCodeRipped()){
			*ai_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
				"ClENG_Analex::PrepareForAnalexField() Unable To Prepare this expression" ) 
				<< CRAZY_MSG << SEND_MSG   ;
		};
		#endif

	} ;

	#ifdef RPT_ENGINE_TRACE
	if(!ai_poInsp->m_poTrace->IsTraceCodeRipped()){
		if ( DoSomething ) {
			*ai_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
				"ClENG_Analex::PrepareForAnalexField() Input expression %0:s, ouput expression %1:s" ) 
				<< ai_oExpr.InChar(0) << ai_poBuffer->InChar(0) << DETAILED_MSG << SEND_MSG   ;
		};
	} ;
	#endif
}

void ClENG_Analex::PrepareForAnalexCTAndField( 
	m4char_t *ai_pcExpr, m4char_t *ai_pcIndexInfo , 
	StExpressionEnv* ai_pstEnv,
	ClMIT_Str* ai_poBuffer,
	m4bool_t ai_bSwDependencies) 
{
	ClMIT_Str strAux ;
	
	strAux.StrCpy(ai_pcExpr) ;
	PrepareForAnalexField( &strAux, ai_pstEnv); 
	PrepareForAnalexCT( strAux.InChar(0), ai_pcIndexInfo , ai_poBuffer, ai_bSwDependencies); 
}

// fix bug 0088808
typedef enum {REP_LOOP_OK,REP_LOOP_FOUND,REP_LOOP_ERROR} LoopRetType_t;
typedef set< ClMIT_Str, less<ClMIT_Str> > IdSetType;

// checks large expressions loops

/* ret values: 
	REP_LOOP_OK OK, 
	REP_LOOP_FOUND loop detectect, 
	REP_LOOP_ERROR sintax error
*/

static m4int16_t CheckLoop(m4pcchar_t ai_pcBuffer, ClENG_Analex::LEMap_t * ai_poLE, 
						   IdSetType &aio_oSet, IdSetType &aio_oValidatedSet, m4uint32_t ai_iLevel,
						   ClMIT_Str &ao_oFailedId) {

	if (ai_pcBuffer == NULL || *ai_pcBuffer == '\0') {
		return REP_LOOP_OK;
	}
	
	// check level
	if (++ai_iLevel > ClENG_Analex::MAX_LE_INCLUSIONS) {
		return REP_LOOP_ERROR;
	}

	// scans expressions
	m4pcchar_t pcBeg= NULL, pcPos = ai_pcBuffer;
	ClMIT_Str oStrAux;
	ClENG_Analex::LEMap_t::iterator oIt ;

	for (;;)  {
		pcBeg = ClMIT_Str::StrIStr((m4pchar_t)pcPos,"$LE") ;
		
		if (pcBeg == NULL) {
			return REP_LOOP_OK;
		}

		// gets the expression name
		pcBeg += 4; // "$LE" + 1 
		m4uint32_t iLong = strspn(pcBeg,ANALEX_LONG_EXPR_SCAN) ;
		m4pcchar_t pcEnd = NULL;

		if(iLong>0) {
			pcEnd = pcBeg + iLong;
		}
		else if(strchr(pcBeg, M4_END_STR)) {
			pcEnd = strchr(pcBeg, M4_END_STR);
		}
		else {
			return REP_LOOP_ERROR;
		}

		oStrAux.ToASCIZ(pcBeg,pcEnd-pcBeg) ;

		// prepares next execution
		pcPos = pcBeg + strlen(oStrAux);


		// is already a valid expresion ?
		if (aio_oValidatedSet.find(oStrAux) != aio_oValidatedSet.end()) {
			// skips validation
			continue;
		}

		if (aio_oSet.insert(oStrAux).second == 0) {
			// already in map. Loop found!
			ao_oFailedId = oStrAux;
			return REP_LOOP_FOUND;
		}

		oIt = ai_poLE->find(oStrAux) ;
		if ( oIt == ai_poLE->end() ) {
			return REP_LOOP_ERROR;
		}

		// scans sublevel
		m4int16_t iRes = CheckLoop((*oIt).second,ai_poLE,aio_oSet,aio_oValidatedSet,ai_iLevel,ao_oFailedId);
		if (iRes != REP_LOOP_OK) {
			ClMIT_Str oTemp (oStrAux);
			oTemp.StrCat(" -> ");
			oTemp.StrCat(ao_oFailedId);
			ao_oFailedId = oTemp;
			return iRes;
		}

		// removes from the active search tree
		aio_oSet.erase(oStrAux);

		// inserts a new validated expression
		aio_oValidatedSet.insert(oStrAux);
	}

	return REP_LOOP_OK;
}

m4bool_t ClENG_Analex::PrepareForLongExpr( ClMIT_Str *ai_poBuffer, ClENG_Analex::LEMap_t * ai_poLE )
{ 
	m4bool_t SwOk = M4_TRUE;
	ClMIT_Str oStrAux, oLongExpr;
	m4char_t  *Pos, *Beg, *End, *Expr;
	StMIT_Inspector* ai_poInsp = GET_INSP() ;
	ClMIT_Str ai_oExpr ;
	m4bool_t DoSomething, DoSomethingTotal=M4_FALSE ;
	size_t Long ;
	LEMap_t::iterator It ;
	IdSetType oIdSet, oIdValidatedSet;
	
	m4uint32_t Inclusions=0 ;

	// fix bug 0088808
	// validate loops
	m4int16_t iResLoop = CheckLoop((m4pcchar_t)ai_poBuffer,ai_poLE,oIdSet,oIdValidatedSet,0,oStrAux);
	if (iResLoop == REP_LOOP_FOUND) {
		*ai_poInsp->m_poTrace << ClMIT_Msg(
			ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163038,
			"Se ha detectado una referencia circular en el siguiente camino: \"%0:s\".", ClMIT_Msg::MIXED_OUT) 
			<< (char*)oStrAux << SEND_MSG ;
		SwOk=M4_FALSE;
		goto exit;
	}
	
	do {	//Bucle para procesar recursiones de LongExpressions

		DoSomething=M4_FALSE ;	

		ai_oExpr.StrCpy(ai_poBuffer) ;
		//Puntero Expr no varia porque ai_poExpr no crece ni decrece en este metodo!!	
		Expr=ai_oExpr.InChar(0);	

		ai_poBuffer->StrCpy("") ;

		for (Pos=Expr; *Pos!=M4_END_STR;) {
			Beg=ClMIT_Str::StrIStr(Pos,"$LE") ;
			if (Beg) {

				DoSomething=DoSomethingTotal=M4_TRUE ;
				oStrAux.ToASCIZ(Pos,Beg-Pos) ;
				ai_poBuffer->StrCat(oStrAux) ;

				//Obtiene ID_EXPRESSION
				Beg+=strlen("$LE")+1 ;	//+1 por '.'
				Long = strspn(Beg,ANALEX_LONG_EXPR_SCAN) ;
				if(Long>0) {
					End = Beg+Long;
				}else if(strchr(Beg, M4_END_STR)) {
					End = strchr(Beg, M4_END_STR);
				}else{
					*ai_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_230 ] ) << SEND_MSG ;
					SwOk=M4_FALSE;
					goto exit;
				} ;
				oStrAux.ToASCIZ(Beg,End-Beg) ;
			
				It = ai_poLE->find(oStrAux) ;
				if ( It == ai_poLE->end() ) {
					//Cambiar por mensaje correcto!
					*ai_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_230 ] ) << SEND_MSG ;
					SwOk=M4_FALSE ;
					goto exit ;
				} ;

				ai_poBuffer->StrCat((*It).second );

				Beg += strlen( oStrAux );
				Pos=Beg ;

			} else {
				ai_poBuffer->StrCat(Pos) ;
				Pos += strlen(Pos) ;
			} ;
		} ;
		Inclusions++ ;
		if (Inclusions>MAX_LE_INCLUSIONS) {
			*ai_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_380 ] ) 
				<< (m4int16_t)MAX_LE_INCLUSIONS << Expr << SEND_MSG ;
			SwOk=M4_FALSE ;
			//Error local
			break ;
		} ;
	} while (DoSomething)  ;

exit:
	if (!SwOk) {

		//Error local

		ai_poBuffer->StrCpy("Error in ClENG_Analex::PrepareForLongExpr()!!!") ;

		#ifdef RPT_ENGINE_TRACE
		if(!ai_poInsp->m_poTrace->IsTraceCodeRipped()){
			*ai_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
				"ClENG_Analex::PrepareForLongExpr() Unable To Prepare this expression" ) 
				<< CRAZY_MSG << SEND_MSG   ;
		};
		#endif

	} ;

	#ifdef RPT_ENGINE_TRACE
	if(!ai_poInsp->m_poTrace->IsTraceCodeRipped()){
		if ( DoSomethingTotal ) {
			// Bug 0308237. Para que no se desborde la traza (máximo 13000 caracteres), se divide el mensaje.
			*ai_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "ClENG_Analex::PrepareForLongExpr() Input expression %0:s" ) 
				<< ai_oExpr.InChar(0) << DETAILED_MSG << SEND_MSG   ;

			*ai_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "ClENG_Analex::PrepareForLongExpr() Output expression %0:s" ) 
				<< ai_poBuffer->InChar(0) << DETAILED_MSG << SEND_MSG   ;
		};
	} ;
	#endif

	return SwOk ;
}


m4bool_t ClENG_Analex::GetTranslatedString(const m4char_t* ai_pcString, ClMIT_Str* ao_pTranslated, StENG_ReportEnv::TrLitMap_t * ai_poTrLit)
{
	ClMIT_Str Key ;
	StENG_ReportEnv::TrLitMap_t::iterator FoundIt ;
	m4bool_t SwOk=M4_TRUE ;

	Key.StrCpy(ai_pcString) ;
	FoundIt=ai_poTrLit->find( Key ) ;
	if (FoundIt!=ai_poTrLit->end()) {
		ao_pTranslated->StrCpy( (*FoundIt).second );
	} else {
		ao_pTranslated->StrCpy("") ;
		SwOk=M4_FALSE ;
	} ;
	return SwOk ;
}

m4bool_t ClENG_Analex::AnaLex(
	const m4char_t *ai_pcStr, StExpressionEnv * ai_pstEnv) 
{
	m4bool_t SwOk=M4_TRUE;
	m4bool_t ShowMessage=M4_FALSE ; //Por defecto no se muestra el mensaje de error final
	const m4char_t *Pos, *PosOk;
	ClMIT_Str StrA ;
	ClENG_Token *Tk=NULL ;
	ClENG_TkNormalOperator::NORMAL_OPERATOR Ope ;
	m4bool_t SwComment;

	ClMIT_Args Arg ;
	size_t Long ;
	m4int16_t Index ;

	ANALEX_IV_TYPE IVType ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Analex::AnaLex() Expression %0:s", ClMIT_Msg::ENTRY_FORMAT) 
			<< ai_pcStr << DETAILED_MSG << SEND_MSG   ;
	};
	#endif

	Pos = PosOk= NextString(ai_pcStr) ;

	
	while ( GetCharType(*Pos) != END_STRING ) {

		//Solo se inserta token en la lista si es correcto. Por ello, si no lo es, hay que 
		//	borrarlo.

		SwComment=M4_FALSE;
		Tk=NULL ;

		switch (*Pos) {

		case ANALEX_LITERAL_STRC:

			Pos= Pos+1+StrA.GetString( ClMIT_Str::TO_LIMIT, Pos+1, ANALEX_LITERAL_STRS) ;
			if ( *Pos != ANALEX_LITERAL_STRC ) {
				SwOk=M4_FALSE ;
				ShowMessage=M4_TRUE;
				goto exit ;
			} ;
			++Pos ;

			M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkLiteral ) ;
			M4_VIRTUAL_OBJECT(Tk,ClENG_TkLiteral)->Init(ClMIT_ChnValue::STRING, StrA) ;

			break;

		case ANALEX_LITERAL_NTRANSC:

			Pos= Pos+1+StrA.GetString( ClMIT_Str::TO_LIMIT, Pos+1, ANALEX_LITERAL_NTRANSS) ;
			if ( *Pos != ANALEX_LITERAL_NTRANSC ) {
				SwOk=M4_FALSE ;
				ShowMessage=M4_TRUE;
				goto exit ;
			} ;
			++Pos ;

			M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkLiteral ) ;
			M4_VIRTUAL_OBJECT(Tk,ClENG_TkLiteral)->Init(ClMIT_ChnValue::STRING, StrA) ;

			break;

		case ANALEX_LITERAL_DATEO_C:

			Pos= Pos+1+StrA.GetString( ClMIT_Str::TO_LIMIT, Pos+1, ANALEX_LITERAL_DATEC_S) ;
			if ( *Pos != ANALEX_LITERAL_DATEC_C ) {
				SwOk=M4_FALSE ;
				ShowMessage=M4_TRUE;
				goto exit ;
			} ;
			++Pos ;

			if(strpbrk(StrA.InChar(0),ANALEX_LETTER) /*|| strcspn(StrA.InChar(0),ANALEX_DIGIT)>0*/){
				SwOk=M4_FALSE ;
				ShowMessage=M4_TRUE;
				goto exit ;
			};

			M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkLiteral ) ;
			M4_VIRTUAL_OBJECT(Tk,ClENG_TkLiteral)->Init(ClMIT_ChnValue::DATE, StrA) ;

			break;

		case ANALEX_RESERVED_WC:

			++Pos ;
			
			//El orden de analizacion es importante. Se analizan primero los literales mas largos
			// Primero los nulos y luego las constantes Booleanas
			//NULO BOOLEANO
			if ( *Pos && !strcmp(ANALEX_LITERAL_NULL_BOOLEAN, Pos) ) {

				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkLiteral ) ;
				M4_VIRTUAL_OBJECT(Tk,ClENG_TkLiteral)->Init(ClMIT_ChnValue::BOOLEAN, "0", M4_TRUE) ;
				
				Pos+=strlen(Pos);
				
			//NULO NUMERICO
			} else if ( *Pos && !strcmp(ANALEX_LITERAL_NULL_NUMERIC, Pos) ) {

				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkLiteral ) ;
				M4_VIRTUAL_OBJECT(Tk,ClENG_TkLiteral)->Init(ClMIT_ChnValue::NUMERIC, "0", M4_TRUE) ;
				
				Pos+=strlen(Pos);
				
			//NULO FECHA
			} else if ( *Pos && !strcmp(ANALEX_LITERAL_NULL_DATE, Pos) ) {

				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkLiteral ) ;
				M4_VIRTUAL_OBJECT(Tk,ClENG_TkLiteral)->Init(ClMIT_ChnValue::DATE, "0", M4_TRUE) ;
				
				Pos+=strlen(Pos);
				
			//NULO STRING
			} else if ( *Pos && !strcmp(ANALEX_LITERAL_NULL_STRING, Pos) ) {

				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkLiteral ) ;
				M4_VIRTUAL_OBJECT(Tk,ClENG_TkLiteral)->Init(ClMIT_ChnValue::STRING, "0", M4_TRUE) ;
				
				Pos+=strlen(Pos);
				
			/*//CONSTANTE DE TIPO MONEDA
			  // No terminado
			} else if ( *Pos && !strcmp(ANALEX_LITERAL_CURRENCY, Pos) ) {

				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkLiteral ) ;
				M4_VIRTUAL_OBJECT(Tk,ClENG_TkLiteral)->Init(ClMIT_ChnValue::CURRENCY, "0") ;
				
				Pos+=strlen(Pos);*/

			//CONSTANTE BOOLEANA
			} else if ( *Pos && strchr(ANALEX_LITERAL_TRUE,*Pos) ) {

				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkLiteral ) ;
				M4_VIRTUAL_OBJECT(Tk,ClENG_TkLiteral)->Init(ClMIT_ChnValue::BOOLEAN, "1") ;

				++Pos ;

			} else if ( *Pos && strchr(ANALEX_LITERAL_FALSE,*Pos) ) {

				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkLiteral ) ; 
				M4_VIRTUAL_OBJECT(Tk,ClENG_TkLiteral)->Init(ClMIT_ChnValue::BOOLEAN, "0") ;

				++Pos ;

			} else {

				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_160 ] ) << SEND_MSG ;
				SwOk=M4_FALSE ;
				goto exit ;

			} ;

			break ;

		case ANALEX_OPEN:
		case ANALEX_CLOSE:

			M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkPrecedence ) ;
			M4_VIRTUAL_OBJECT(Tk,ClENG_TkPrecedence)->Init( M4_BOOL(*Pos==ANALEX_OPEN) ) ;

			++Pos ;

			break ;
		
		case ANALEX_ARG_SEPARATOR:

			M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkExpSep ) ;
			M4_VIRTUAL_OBJECT(Tk,ClENG_TkExpSep)->Init() ;

			++Pos ;

			break ;

		case ANALEX_FUNCTIONC:

			//Es alguna funcion

			++Pos ;	//Salta el %, que pertenece a ANALEX_CHARSET_OUT_TOKEN
			Long = strspn(Pos, ANALEX_FUNCTION_SCAN) ;
			Arg.Init( Pos, Long, ClMIT_Args::NO_SUBARG, ANALEX_INTRA_TOKEN ) ;
			Pos += Long ;

			if ( Arg.GetArgCount() == 0 ) {
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_130 ] ) << SEND_MSG ;
				SwOk=M4_FALSE ;
				goto exit ;
			} ;
			IVType = (ANALEX_IV_TYPE)Arg.GetEnum(0,0, AnalexIVType, &SwOk, M4_TRUE )  ;
			if (!SwOk) {
				ShowMessage=M4_TRUE ;
				goto exit ;
			} ;

			switch ( IVType ) {

			case ANALEX_IV_ACU:

				{
				//Acumuladores de seccion 

				ClENG_TkReportTot::ACTION IVAction ;
				ClMIT_Str IdTot ;

				if ( Arg.GetArgCount() != 3 ) {
					*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_130 ] ) << SEND_MSG ;
					SwOk=M4_FALSE ;
					goto exit ;
				} ;
				
				IVAction = (ClENG_TkReportTot::ACTION)Arg.GetEnum(2,0, AnalexSectionAcuFunc, &SwOk, M4_TRUE )  ;
				if (!SwOk) {
					ShowMessage=M4_TRUE ;
					goto exit ;
				} ;
				Arg.GetString(1,0,&IdTot) ;

				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkReportTot ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkReportTot)->Init( 
					ai_pstEnv->m_poReport, IdTot, IVAction, 
				 	ai_pstEnv->m_poDependencies, ClMIT_Trace::PROPAGATE_ERROR ) ;
				if (!SwOk) goto exit ;
				} ;

				break ;

			case ANALEX_IV_PI:
				{

				ClENG_TkReportTot::ACTION IVAction ;
				ClMIT_Str IdPi ;

				if ( Arg.GetArgCount() != 2 ) {
					*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_130 ] ) << SEND_MSG ;
					SwOk=M4_FALSE ;
					goto exit ;
				} ;
				
				Arg.GetString(1,0,&IdPi) ;
				
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkPI ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkPI)->Init(
					IdPi,ai_pstEnv->m_poFieldManager,ClMIT_Trace::PROPAGATE_ERROR ) ;
				if (!SwOk) goto exit ;

				break ;

				} ;


			case ANALEX_IV_CT:
				
				{

					ClENG_TkCTTot::ACTION IVAction;

					IVAction = (ClENG_TkCTTot::ACTION)Arg.GetEnum(Arg.GetArgCount()-1,0, 
						AnalexCTAcuFunc, &SwOk, M4_TRUE );
					if (!SwOk) {
						ShowMessage=M4_TRUE ;
						SwOk=M4_FALSE ;
						goto exit ;
					} ;
					M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkCTTot ) ;
					SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkCTTot)->Init(
						IVAction, ai_pstEnv->m_poReport, &Arg, 
						ai_pstEnv->m_poDependencies, ClMIT_Trace::PROPAGATE_ERROR ) ;
					if (!SwOk) {
						SwOk=M4_FALSE ;
						goto exit ;
					} ;
				};

				break;
		
			case ANALEX_IV_GET_STRING:
				
				{
					ClMIT_Str IdString;

					if ( Arg.GetArgCount() != 2 ) {
						*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_130 ] ) << SEND_MSG ;
						SwOk=M4_FALSE ;
						goto exit ;
					} ;
				
					Arg.GetString(1,0,&IdString) ;
				
					M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkExtractString ) ;
					SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkExtractString)->Init(
						IdString, ai_pstEnv->m_poTrLit ,ClMIT_Trace::PROPAGATE_ERROR ) ;
					if (!SwOk) goto exit ;

					break ;
				};
			case ANALEX_IV_NODE:
				
				{
					ClENG_TkNODEInfo::ACTION IVAction;
					ClMIT_Str IdNode;

					if ( Arg.GetArgCount() != 3 ) {
						*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_130 ] ) << SEND_MSG ;
						SwOk=M4_FALSE ;
						goto exit ;
					} ;

					IVAction = (ClENG_TkNODEInfo::ACTION)Arg.GetEnum(Arg.GetArgCount()-1,0, 
						AnalexNODEInfoFunc, &SwOk, M4_TRUE );
					if (!SwOk) {
						ShowMessage=M4_TRUE ;
						SwOk=M4_FALSE ;
						goto exit ;
					} ;
					
					Arg.GetString(1,0,&IdNode) ;

					M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkNODEInfo ) ;
					SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkNODEInfo)->Init(
						IVAction,  ai_pstEnv->m_poReport, IdNode, ai_pstEnv->m_poDependencies,
						ai_pstEnv->m_poChn, ClMIT_Trace::PROPAGATE_ERROR ) ;
					if (!SwOk) {
						SwOk=M4_FALSE ;
						goto exit ;
					} ;
				};

				break;
			
			case ANALEX_IV_GET_CURRENT_DATE:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::GET_CURRENT_DATE, 
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR ) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_GET_GMT_DATE:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::GET_GMT_DATE, 
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR  ) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_TODAY:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::TODAY,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR ) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_GET_CURRENT_TIME:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::GET_CURRENT_TIME,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_GET_CURRENT_TIME_STAMP:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::GET_CURRENT_TS,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_GET_GMT_TIME:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::GET_GMT_TIME,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_GET_GMT_TIME_STAMP:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::GET_GMT_TS,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_TO_DATE:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::TO_DATE,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_TO_TIME:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::TO_TIME,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_TO_TIME_STAMP:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::TO_TS,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_STRING:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::STRING_F,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_SPACE:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::SPACE_F,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_STRIN:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::STR_IN,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_STRNIN:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::STR_NIN,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_MID:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::MID,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_TO_LONG:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::TO_LONG,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_TO_DOUBLE:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::TO_DOUBLE,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_TO_PRECISION_DOUBLE:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init(
					ClENG_TkSysInfo::TO_PRECISION_DOUBLE,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_CHAR:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::CHAR_F,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;

				break ;

			case ANALEX_IV_EXCHANGE_ITEM_CURRENCY:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::EXCH_ITEM_CURR,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_EXCHANGE_CURRENCY:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::EXCH_CURRENCY,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_DEFAULT_EXCHANGE:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::DEF_EXCHANGE,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_DEFAULT_CURRENCY:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::EXCH_DEF_CURRENCY,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_DEFAULT_EXCHANGE2:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::DEFAULT_EXCHANGE2,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_EXCHANGE_ITEM_CUR2:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::EXCHANGE_ITEM_CUR2,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_GET_DEF_CUR:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::GET_DEFAULT_CURRENCY,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_IS_NULL:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::IS_NULL,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_STRLEN:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::STR_LEN,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_CSTR:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::CSTR,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_ALIGN:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::ALIGN,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_FORMAT_EXPRESSION:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::FORMAT_EXPR,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_FORMAT_STRING:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::FORMAT_STR,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;
			
			case ANALEX_IV_FORMAT_DEF_CUR:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::FORMAT_DEF_CUR,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_GET_CURRENCY_VALUE:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::GET_CURRENCY_VALUE,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_PI_FUN:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::PI,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_REPORT_ID:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkThreadInfo) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkThreadInfo)->Init( 
					ai_pstEnv->m_poReport, ai_pstEnv->m_poDvc, ai_pstEnv->m_poExeParams,
					ClENG_TkThreadInfo::Report_Id, ai_pstEnv->m_poDependencies, 
					ClMIT_Trace::PROPAGATE_ERROR ) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_REPORT_NAME:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkThreadInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkThreadInfo)->Init( 
					ai_pstEnv->m_poReport, ai_pstEnv->m_poDvc, ai_pstEnv->m_poExeParams,
					ClENG_TkThreadInfo::Report_Name, ai_pstEnv->m_poDependencies, 
					ClMIT_Trace::PROPAGATE_ERROR ) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_PAGE:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkThreadInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkThreadInfo)->Init( 
					ai_pstEnv->m_poReport, ai_pstEnv->m_poDvc, ai_pstEnv->m_poExeParams,
					ClENG_TkThreadInfo::Page, ai_pstEnv->m_poDependencies, ClMIT_Trace::PROPAGATE_ERROR ) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_GROUP_PAGE:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkThreadInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkThreadInfo)->Init( 
					ai_pstEnv->m_poReport, ai_pstEnv->m_poDvc, ai_pstEnv->m_poExeParams,
					ClENG_TkThreadInfo::Group_Page, 
					ai_pstEnv->m_poDependencies, ClMIT_Trace::PROPAGATE_ERROR ) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_SET_DEL_BREAK:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkThreadInfo) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkThreadInfo)->Init( 
					ai_pstEnv->m_poReport, ai_pstEnv->m_poDvc, ai_pstEnv->m_poExeParams,
					ClENG_TkThreadInfo::SET_DEL_BREAK, ai_pstEnv->m_poDependencies, 
					ClMIT_Trace::PROPAGATE_ERROR ) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_IS_DEL_BREAK:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkThreadInfo) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkThreadInfo)->Init( 
					ai_pstEnv->m_poReport, ai_pstEnv->m_poDvc, ai_pstEnv->m_poExeParams,
					ClENG_TkThreadInfo::IS_DEL_BREAK, ai_pstEnv->m_poDependencies, 
					ClMIT_Trace::PROPAGATE_ERROR ) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_IS_REPRINT:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkThreadInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkThreadInfo)->Init( 
					ai_pstEnv->m_poReport, ai_pstEnv->m_poDvc, ai_pstEnv->m_poExeParams,
					ClENG_TkThreadInfo::IsReprint, ai_pstEnv->m_poDependencies, ClMIT_Trace::PROPAGATE_ERROR ) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_GET_FILE_PATH:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkThreadInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkThreadInfo)->Init( 
					ai_pstEnv->m_poReport, ai_pstEnv->m_poDvc, ai_pstEnv->m_poExeParams,
					ClENG_TkThreadInfo::GetFilePath, ai_pstEnv->m_poDependencies, ClMIT_Trace::PROPAGATE_ERROR ) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_GET_FILE_NAME:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkThreadInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkThreadInfo)->Init( 
					ai_pstEnv->m_poReport, ai_pstEnv->m_poDvc, ai_pstEnv->m_poExeParams,
					ClENG_TkThreadInfo::GetFileName, ai_pstEnv->m_poDependencies, ClMIT_Trace::PROPAGATE_ERROR ) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_GET_FULL_PATH:
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkThreadInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkThreadInfo)->Init( 
					ai_pstEnv->m_poReport, ai_pstEnv->m_poDvc, ai_pstEnv->m_poExeParams,
					ClENG_TkThreadInfo::GetFullPath, ai_pstEnv->m_poDependencies, ClMIT_Trace::PROPAGATE_ERROR ) ;
				if (!SwOk) goto exit ;
				
				break ;

			case ANALEX_IV_SECTION:

				//Datos de seccion

				ClENG_TkSectionInfo::ACTION SecAction ;
				m4uint32_t IdSec ;

				if ( Arg.GetArgCount() == 3 ) {
					SecAction = (ClENG_TkSectionInfo::ACTION)Arg.GetEnum(2,0, AnalexSectionInfoFunc, &SwOk, M4_TRUE )  ;
					if (!SwOk) {
						ShowMessage=M4_TRUE ;
						goto exit ;
					} ;
					IdSec=Arg.GetLong(1,0) ;
				} else if (Arg.GetArgCount() == 2) {
					SecAction = (ClENG_TkSectionInfo::ACTION)Arg.GetEnum(1,0, AnalexSectionInfoFunc, &SwOk, M4_TRUE )  ;
					if (!SwOk) {
						ShowMessage=M4_TRUE ;
						goto exit ;
					} ;
					IdSec=ClENG_TkSectionInfo::NULL_SEC ;
				} else {
					*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_130 ] ) << SEND_MSG ;
					SwOk=M4_FALSE ;
					goto exit ;
				} ;
				
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSectionInfo ) ;
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSectionInfo)->Init( 
					ai_pstEnv->m_poReport, ai_pstEnv->m_poSect, IdSec,
					SecAction, ai_pstEnv->m_poDependencies, ClMIT_Trace::PROPAGATE_ERROR ) ;
				if (!SwOk) goto exit ;

				break ;

			case ANALEX_IV_IIF:

				M4_NEW_ITEM( m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo );
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::IIF,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR);
				if (!SwOk) goto exit ;

				break;

			case ANALEX_IV_NVL:

				M4_NEW_ITEM( m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo );
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::NVL,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR);
				if (!SwOk) goto exit ;

				break;

			case ANALEX_IV_SAVE_ITEM:
				M4_NEW_ITEM( m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo );
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::SAVE_ITEM,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR);
				if (!SwOk) goto exit ;
				break ;

			case ANALEX_IV_GET_DEFAULT_DATE:
				M4_NEW_ITEM( m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo );
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::GET_DEFAULT_DATE,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR);
				if (!SwOk) goto exit ;
				break ;

			case ANALEX_IV_GET_DEFAULT_TYPE:
				M4_NEW_ITEM( m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo );
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::GET_DEFAULT_TYPE,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR);
				if (!SwOk) goto exit ;
				break ;

			case ANALEX_IV_ORGANIZATION_TYPE:
				M4_NEW_ITEM( m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo );
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::ORGANIZATION_TYPE,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR);
				if (!SwOk) goto exit ;
				break ;

			case ANALEX_IV_ORGANIZATION:
				M4_NEW_ITEM( m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkSysInfo );
				SwOk=M4_VIRTUAL_OBJECT(Tk,ClENG_TkSysInfo)->Init( 
					ClENG_TkSysInfo::ORGANIZATION,
					ai_pstEnv->m_poDependencies, ai_pstEnv->m_poChn,  ai_pstEnv->m_poExeParams,
					ai_pstEnv->m_poDBFormat, ai_pstEnv->m_poFieldManager,
					ClMIT_Trace::PROPAGATE_ERROR);
				if (!SwOk) goto exit ;
				break ;

			} ;


			break ;

		default:

			if ( *Pos && strchr( ANALEX_OPERATOR, *Pos ) ) {

				//Es algún operador

				switch(*Pos) {

				case ']':	
					Ope=ClENG_TkNormalOperator::ABSORB_A ;
					break ;

				case '[':	
					Ope=ClENG_TkNormalOperator::ABSORB_B ;
					break ;

				case '+':	
					Ope=ClENG_TkNormalOperator::ADD ;
					break ;

				case '-':
					if ( *(Pos+1)=='-' ) {
						Ope=ClENG_TkNormalOperator::MINUS_SIGN ;
						++Pos ;
					} else {
						Ope=ClENG_TkNormalOperator::SUBSTRACT ;
					} ;
					break ;

				case '*':
					Ope=ClENG_TkNormalOperator::MUL ;
					break ;

				case '/':
					if(*(Pos+1)==ANALEX_COMMENT_2C) {
						Pos++;
						Pos= Pos+1+StrA.GetString( ClMIT_Str::TO_LIMIT, Pos+1, ANALEX_COMMENT_2S) ;
						if ( *Pos != ANALEX_COMMENT_2C ) {
							SwOk=M4_FALSE ;
							ShowMessage=M4_TRUE;
							goto exit ;
						} ;

						Pos++;
						if(*Pos!=ANALEX_COMMENT_1C) {
							SwOk=M4_FALSE ;
							ShowMessage=M4_TRUE;
							goto exit ;
						};

						StrA.StrCpy("");
						++Pos;
	
						SwComment=M4_TRUE;

						break;

					} else {
						Ope=ClENG_TkNormalOperator::DIV ;

						break;
					};

				case '=':
					Ope=ClENG_TkNormalOperator::EQUAL ;
					break ;

				case '<':
					if ( *(Pos+1)=='=' ) {
						Ope=ClENG_TkNormalOperator::LESS_EQUAL ;
						++Pos ;
					} else if ( *(Pos+1)=='>' ) {
						Ope=ClENG_TkNormalOperator::NOT_EQUAL ;
						++Pos ;
					} else {
						Ope=ClENG_TkNormalOperator::LESS ;
					} ;
					break ;

				case '>':
					if ( *(Pos+1)=='=' ) {
						Ope=ClENG_TkNormalOperator::GREATER_EQUAL ;
						++Pos ;
					} else {
						Ope=ClENG_TkNormalOperator::GREATER ;
					} ;
					break ;

				case '&':
					if ( *(Pos+1)=='&' ) {
						Ope=ClENG_TkNormalOperator::STR_CAT ;
						++Pos ;
					} else {
						Ope=ClENG_TkNormalOperator::AND ;
					} ;
					break ;

				case '|':
					Ope=ClENG_TkNormalOperator::OR ;
					break ;

				case '!':
					Ope=ClENG_TkNormalOperator::NOT ;
					break ;
				} ;

				if(!SwComment){
					M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkNormalOperator ) ;
					M4_VIRTUAL_OBJECT(Tk,ClENG_TkNormalOperator)->Init(Ope) ;

					++Pos ;
				};

			} else if ( *Pos && strchr(ANALEX_CHN_BEG,*Pos) ) {

				ClMIT_ChnItem::LOAD_MODE LoadMode ;
				ClENG_TkItem::LOAD_MODE TkLoadMode ;
				const m4char_t *StrItem ;
				const m4char_t *StrNode ;

				//Es un item de canal

				Long = strspn(Pos, ANALEX_CHN_SCAN) ;
				Arg.Init( Pos, Long, ClMIT_Args::NO_SUBARG, ANALEX_INTRA_TOKEN ) ;
				Pos += Long ;
				

				Index=Arg.GetArgCount()-1 ;	
				if ( *Arg.GetArg(Index) == ANALEX_RESERVED_WC ) {
					LoadMode = (ClMIT_ChnItem::LOAD_MODE)Arg.GetEnum( 
						Index, 0, AnalexLoadMode, &SwOk, M4_TRUE )  ;
					if (!SwOk) {
						ShowMessage=M4_TRUE ;
						goto exit ;
					} ;
					Index = Arg.GetArgCount()-1 ;
				} else {
					LoadMode = ai_pstEnv->m_LoadMode ;
					Index= Arg.GetArgCount() ;
				} ;

				switch(LoadMode) {
				case -1000:
					TkLoadMode=ClENG_TkItem::TOTALIZE_COUNT ;
					LoadMode = ClMIT_ChnItem::ITEM_LOAD ;
					break ;
				case -2000:
					TkLoadMode=ClENG_TkItem::TOTALIZE_SLICE_COUNT;
					LoadMode = ClMIT_ChnItem::ITEM_LOAD ;
					break ;
				default:
					TkLoadMode=ClENG_TkItem::ITEM_MODE;
					break ;
				} ;


				// Ahora Index es el numero de subargumentos a procesar.

				if (Index==1) {
					StrNode=NULL ;
					StrItem = Arg.GetArg(0) ;
				} else if (Index==2) {
					StrNode= Arg.GetArg(0) ;
					StrItem = Arg.GetArg(1) ;
				} else {
					SwOk=M4_FALSE ;
					ShowMessage=M4_TRUE;
					goto exit ;
				} ;

				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkItem ) ;
				SwOk = M4_VIRTUAL_OBJECT(Tk,ClENG_TkItem)->Init(
					ai_pstEnv->m_poChn, ai_pstEnv->m_poDefaultNode, StrNode, StrItem, 
					TkLoadMode,LoadMode, ai_pstEnv->m_poDependencies, ClMIT_Trace::PROPAGATE_ERROR ) ;
				if (!SwOk) goto exit ;
			
			} else if ( *Pos && strchr(ANALEX_LITERAL_NUM_BEG ,*Pos) ) {

				Pos=Pos+StrA.GetString(ClMIT_Str::WHILE_CHAR, Pos, ANALEX_LITERAL_NUMERIC ) ;
				
				M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk, ClENG_TkLiteral ) ;
				M4_VIRTUAL_OBJECT(Tk,ClENG_TkLiteral)->Init(ClMIT_ChnValue::NUMERIC, StrA) ;

			} else {

				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_170 ] ) << SEND_MSG ;
				SwOk=M4_FALSE ;
				goto exit ;
			} ;

			break ;
		} ;

		if(!SwComment){
			m_oTokenList.push_back(Tk) ;
		};
		
		Pos = PosOk = NextString(Pos) ;
	} ;

exit:
	if (!SwOk) {

		M4_DELETE_VIRTUAL_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, Tk ) ;
		
		if(ShowMessage) {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_30 ] ) 
				<< ai_pcStr << PosOk << SEND_MSG ;
		};
	} ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Analex::AnaLex() Expression %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< ai_pcStr << DETAILED_MSG << SEND_MSG   ;
	};
	#endif

	return SwOk ;
}

m4bool_t ClENG_Analex::AnaSintax() 
{
	m4bool_t SwOk=M4_TRUE ;
	TokenList_t::iterator Beg, End, Root ;
	ClENG_Token *Tk ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Analex::AnaSintax() Expression %0:s", ClMIT_Msg::ENTRY_FORMAT) 
			<< m_poExpr->GetExpressionStr() << DETAILED_MSG << SEND_MSG   ;
	};
	#endif

	Beg= m_oTokenList.begin() ; End=m_oTokenList.end() ;

	SwOk = M4_BOOL(SwOk && TestParenthesis(Beg,End)) ;
	if (!SwOk) goto exit ;
	
	SwOk = M4_BOOL(SwOk && GenerateTree(Beg, End, Root));
	if (!SwOk) goto exit ;

	Tk=*Root ; *Root=NULL ;
	m_poExpr->PreAnalex(Tk) ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_Analex::AnaSintax() Ok ? %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << DETAILED_MSG << SEND_MSG   ;
	};
	#endif

	return SwOk ;
}


m4bool_t ClENG_Analex::SearchLessOperator(
		TokenList_t::iterator & ai_poBeg, TokenList_t::iterator & ai_poEnd,
		TokenList_t::iterator & ai_Found) 
{
	TokenList_t::iterator B, TkAtomic, It  ;
	m4int16_t ActParLevel, SelectedParLevel, SelectedOpLevel ;
	ClENG_TkOperator *TkOp ;
	m4bool_t SwOk=M4_TRUE ;
	m4bool_t SwOnlyAtomic=M4_TRUE ;

	ai_Found=TkAtomic=m_oTokenList.end() ;

	ActParLevel=0 ; 
	SelectedParLevel=0 ; 
	SelectedOpLevel=0 ; 
	for (B=ai_poBeg; B != ai_poEnd ; ++B ) {

		switch ( (*B)->GetType() ) {

		case  ClENG_Token::PRECEDENCE:

			if ( ((ClENG_TkPrecedence *)(*B))->IsOpen() ) {
				++ActParLevel ;
			} else {
				--ActParLevel ;
			} ;
			break;

		case ClENG_Token::OPERATOR:

			TkOp=(ClENG_TkOperator *)(*B) ;
			if ( ai_Found==m_oTokenList.end() || 
				ActParLevel<SelectedParLevel || 
				( ActParLevel==SelectedParLevel && TkOp->GetOpLevel() > SelectedOpLevel ) ) 
			{
				ai_Found = B ;
				SelectedParLevel=ActParLevel ;
				SelectedOpLevel=TkOp->GetOpLevel() ;
			} ;
			break ;

		case ClENG_Token::ATOMIC_VALUE:
			if ( TkAtomic==m_oTokenList.end() ) {
				TkAtomic=B ;
			} else {
				SwOnlyAtomic=M4_FALSE ;
			} ;
			break ;

		//case ClENG_Token::ARG_SEPARATOR:
		//	m_poInsp->m_poTrace->PutMsg( &TRACE_ARRAY[ ANALEX_MSG_110 ], "ClENG_Analex::SearchLessOperator()") ;
		//	SwOk=M4_FALSE ;
		//	goto exit ;

		//	break ;
		} ;
	} ;

	if ( ai_Found==m_oTokenList.end() ) {

		// Valor atomico

		if (!SwOnlyAtomic) {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_80 ] ) << SEND_MSG ;
			SwOk=M4_FALSE ;
			goto exit ;
		} ;
		if ( TkAtomic==m_oTokenList.end() )  {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_90 ] ) << SEND_MSG ;
			SwOk=M4_FALSE ;
			goto exit ;
		} ;

		ai_Found=TkAtomic ;

	} else {
		
		//Operador, testeo de unarios. 

		TkOp=(ClENG_TkOperator *)(*ai_Found) ;
		if ( TkOp->GetArgCount()==1 ) {
			// Solo es preciso comprobar que hay (), el orden y validez ya se comprobo
			if ( TkOp->IsToRight() ) {
				for (B=ai_poBeg; B != ai_Found ; ++B ) {
					if ( (*B)->GetType() != ClENG_Token::PRECEDENCE ) {
						*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_120 ] ) << SEND_MSG ;
						SwOk=M4_FALSE ;
						goto exit ;
					} ;
				} ;
			} else {
				It = ai_poBeg ; ++It ;
				for (B=It; B != ai_poEnd ; ++B ) {
					if ( (*B)->GetType() != ClENG_Token::PRECEDENCE ) {
						*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_120 ] ) << SEND_MSG ;
						SwOk=M4_FALSE ;
						goto exit ;
					} ;
				} ;

			} ;
		} ;
	
	} ;

exit:
	if (!SwOk) ai_Found=m_oTokenList.end() ;

	return SwOk ;
}


m4bool_t ClENG_Analex::SearchFunctionArg(
	TokenList_t::iterator & ai_poBeg, TokenList_t::iterator & ai_poEnd,
	TokenList_t::iterator & ai_Found, m4bool_t *ao_bSwEmpty ) 
{
	TokenList_t::iterator B ;
	m4int16_t ActParLevel ;
	m4bool_t SwOk=M4_TRUE ;

	ai_Found=m_oTokenList.end() ;
	ActParLevel=0 ; 
	*ao_bSwEmpty=M4_TRUE ;
	for (B=ai_poBeg; B != ai_poEnd && ai_Found==m_oTokenList.end(); ++B ) {

		switch ( (*B)->GetType() ) {

		case  ClENG_Token::PRECEDENCE:

			if ( ((ClENG_TkPrecedence *)(*B))->IsOpen() ) {
				++ActParLevel ;
			} else {
				if ( ActParLevel==0 ) {
					ai_Found=B ;
				} else {
					--ActParLevel ;
				} ;
			} ;
			break;

		case ClENG_Token::ARG_SEPARATOR:
	
			if ( ActParLevel==0 ) {
				ai_Found=B ;
			} ;

			break ;

		default:

			*ao_bSwEmpty=M4_FALSE ;

			break ;
		} ;
	} ;

	if ( ai_Found==m_oTokenList.end() ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	
	if ( ! TestParenthesis(ai_poBeg,ai_Found) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	if ( (*ai_Found)->GetType()==ClENG_Token::PRECEDENCE ) {
		for (B=ai_Found; B != ai_poEnd ; ++B ) {
			if ( (*B)->GetType() != ClENG_Token::PRECEDENCE ) {
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_120 ] ) << SEND_MSG ;
				SwOk=M4_FALSE ;
				goto exit ;
			} ;
		} ;
	} ;

exit:
	if (!SwOk) ai_Found=m_oTokenList.end() ;
	
	return SwOk ;
}

m4bool_t ClENG_Analex::TestParenthesis(TokenList_t::iterator &ai_Begin, TokenList_t::iterator &ai_End ) 
{
	TokenList_t::iterator B  ;
	m4bool_t SwOk=M4_TRUE ;

	m4int16_t ActParLevel ;

	ActParLevel=0 ; 
	for (B=ai_Begin ; B != ai_End ; ++B ) {

		if ( (*B)->GetType() == ClENG_Token::PRECEDENCE ) {

			if ( ((ClENG_TkPrecedence *)(*B))->IsOpen() ) {
				++ActParLevel ;
			} else {
				--ActParLevel ;
			} ;
			if ( ActParLevel < 0 ) {
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_70 ] ) << SEND_MSG ;
				SwOk=M4_FALSE ;
				goto exit ;
			} ;
		} ;
	} ;

	if ( ActParLevel != 0 ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_60 ] ) << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

exit:
	return SwOk ;
}


inline void ClENG_Analex::TransferFromListToToken(TokenList_t::iterator & ai_poIt, ClENG_Token *ai_poTk ) 
{
	ai_poTk->AddSon( *ai_poIt ) ;
	*ai_poIt = NULL ;
}

m4bool_t ClENG_Analex::GenerateTree(
		TokenList_t::iterator & ai_poBeg, TokenList_t::iterator & ai_poEnd,
		TokenList_t::iterator & ai_poRoot ) 
{
	m4bool_t SwOk=M4_TRUE, SwEmpty ;
	TokenList_t::iterator NodeA, NodeB, NodeAux1, NodeAux2 ;
	ClENG_TkOperator *TkOp ;
	
	ai_poRoot=NodeA=NodeB=m_oTokenList.end();
	
	SwOk = M4_BOOL( SwOk && SearchLessOperator( ai_poBeg, ai_poEnd, ai_poRoot ) );
	if (!SwOk) goto exit ;

	if ( (*ai_poRoot)->GetType() == ClENG_Token::OPERATOR )  {

		//--------------->Operator, llamada recursiva.  
	
		TkOp = (ClENG_TkOperator *)(*ai_poRoot) ;

		if ( ! TkOp->IsFunction()  ) {

			//Tipo parametro 

			switch(TkOp->GetArgCount()) {
			case 2:

				//Binarios

				SwOk = M4_BOOL( SwOk && GenerateTree(ai_poBeg,ai_poRoot, NodeA ) );
				NodeAux1=ai_poRoot ; ++NodeAux1 ;
				SwOk = M4_BOOL( SwOk && GenerateTree(NodeAux1,ai_poEnd, NodeB ) );
				if (!SwOk) goto exit ;

				//Ok, al arbol

				TransferFromListToToken( NodeA, TkOp ) ;
				TransferFromListToToken( NodeB, TkOp ) ;

				break ;

			case 1:

				//Unarios a dcha o izda

				if ( TkOp->IsToRight() ) {
					NodeAux1=ai_poRoot ; ++NodeAux1 ;
					NodeAux2=ai_poEnd ;
				} else {
					NodeAux1=ai_poBeg ;
					NodeAux2=ai_poRoot ;
				} ;

				SwOk = M4_BOOL( SwOk && GenerateTree(NodeAux1,NodeAux2, NodeA) );
				if (!SwOk) goto exit ;

				//Ok, al arbol

				TransferFromListToToken( NodeA, TkOp ) ;

				break ;

			default:

				//Otro tipo, no soportado

				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_100 ] ) 
					<< TkOp->GetArgCount() << SEND_MSG ;
				m_poInsp->m_poTrace->ImmediateError() ;

				break ;
			} ;

		
		
		} else {
		
			//Tipo función, obtener parámetros por Func(...,...,...)

			TkOp = (ClENG_TkOperator *)(*ai_poRoot) ;

			// Testea que a continuacion haya un (
			NodeAux1 = ai_poRoot; ++NodeAux1 ;
			if (NodeAux1==m_oTokenList.end()) {
				SwOk=M4_FALSE ;
			} else { 
				if ( (*NodeAux1)->GetType() != ClENG_Token::PRECEDENCE ) {
					SwOk=M4_FALSE ;
				} else {
					if ( ! ( ((ClENG_TkPrecedence *)*NodeAux1)->IsOpen() ) ) {
						SwOk=M4_FALSE ;
					} ;
				} ;
			} ;
			if (!SwOk) {
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_155 ] ) << SEND_MSG ;
				goto exit ;
			} ;

			++NodeAux1 ;	//Salta el ( inicial
			NodeAux2=ai_poEnd ;
			while (M4_TRUE) {

				if ( !SearchFunctionArg(NodeAux1,NodeAux2,NodeA,&SwEmpty) ) {
					SwOk=M4_FALSE ;
					goto exit;
				} ;

				// Analizo subargumento, si hay 

				if ( !SwEmpty ) {

					if ( TkOp->GetCountSons()==TkOp->GetMaxSons() ) {
						*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_140 ] ) 
							<< TkOp->GetMaxSons() << SEND_MSG ;
						SwOk=M4_FALSE ;
						goto exit;
					} ;

					SwOk = M4_BOOL( SwOk && GenerateTree(NodeAux1,NodeA, NodeB) );
					if (!SwOk) goto exit ;

					//Ok, al arbol

					TransferFromListToToken( NodeB, TkOp ) ;
				} ;

				//A por el siguiente argumento, o fin

				if ( (*NodeA)->GetType() == ClENG_Token::PRECEDENCE )  {
					break ;
				} else {
					NodeAux1=NodeA ;	++NodeAux1 ;	//Salta el ,
				} ;

			} ;

			if ( TkOp->GetCountSons()!=TkOp->GetMaxSons() ) {
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ ANALEX_MSG_150 ] ) 
					<< TkOp->GetMaxSons() << TkOp->GetCountSons() << SEND_MSG ;
				SwOk=M4_FALSE ;
				goto exit;
			} ;

		} ;
	
	} else {

		//--------------->Atomic Value, fin de arbol recursivo.  


		//No hay que hacer nada, llamada padre se encargará de engancharlo a nodo padre y
		//	desengancharlo de la lista.
	
	} ;


exit:

	if (!SwOk) {
		if ( ai_poRoot != m_oTokenList.end() ) {
			if ( *ai_poRoot ) (*ai_poRoot)->End() ;
		} ;
		if ( NodeA != m_oTokenList.end() ) {
			//Borra también todo el subarbol
			M4_DELETE_VIRTUAL_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, *NodeA ) ;
		} ;
		if ( NodeB != m_oTokenList.end() ) {
			//Borra también todo el subarbol
			M4_DELETE_VIRTUAL_ITEM(m_poInsp->m_poMemProf, ENG_MGI_ANALEX, *NodeB ) ;
		} ;
	} ;

	return SwOk ;
}

