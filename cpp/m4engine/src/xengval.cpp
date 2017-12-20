//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             engvalue.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:08/08/1997
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Manuel Lazcano (28/05/98) - TIPO CURRENCY
//												ClMIT_ChnValue::ResetChannel()
//												ClMIT_ChnValue::GetCurrencyExchange()				
// Definition:
//
//    This module defines...
//
//==============================================================================

#include "apichni.h"

#include "cltimer.h"
#include "mitdates.h"
#include "xapichn.h"

#include "xengval.hmg"
#include "xengval.h"

const m4char_t *ClMIT_ChnValue::m_pcTypeTraceStr[ClMIT_ChnValue::TYPE_COUNT] = {
	"NUMERIC",
	"STRING",
	"DATE",
	"TIME_STAMP",
	"BOOLEAN",
	"CURRENCY",
	"VARIANT",
	"NO INIT"
} ;

// ----------------------------- Inicialización---------------------------------------	

//-----------Inicializaciones para manipular monedas

m4bool_t ClMIT_ChnValue::StartUpCurrency( ClMIT_Chn * ai_poChannel )
{
	return ai_poChannel->StartUpCurrency() ;
}

void ClMIT_ChnValue::SetDefExchCurr( m4char_t *ai_pcExchCurr ) 
{
	ClMIT_TM *TM = GET_TM() ;
	StMIT_Inspector *ai_poInsp = TM_GET_INSP(TM) ;

	m4char_t *Str ;

	*ai_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
		"ClMIT_ChnValue::SetDefExchCurr to %0:s" ) << ai_pcExchCurr << SEND_MSG   ;

	Str=GET_TASK_ENV()->m_APIChn.ItemV_m_pcExchCurr ;
	strcpy(Str,ai_pcExchCurr) ;
	ClMIT_Str::OverStringValid( Str, StMIT_APIChn_TaskEnv::ItemV_CURR_SIZE ) ;
}

void ClMIT_ChnValue::SetDefExchType( m4char_t *ai_pcExchType ) 
{
	ClMIT_TM *TM = GET_TM() ;
	StMIT_Inspector *ai_poInsp = TM_GET_INSP(TM) ;

	m4char_t *Str ;

	*ai_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
		"ClMIT_ChnValue::SetDefExchType to %0:s" ) << ai_pcExchType << SEND_MSG   ;

	Str=GET_TASK_ENV()->m_APIChn.ItemV_m_pcExchType ;
	strcpy(Str,ai_pcExchType) ;
	ClMIT_Str::OverStringValid( Str, StMIT_APIChn_TaskEnv::ItemV_TYPE_SIZE ) ;
}

void ClMIT_ChnValue::SetDefExchDate( m4date_t ai_ExchDate ) 
{ 
	ClMIT_TM *TM = GET_TM() ;
	StMIT_Inspector *ai_poInsp = TM_GET_INSP(TM) ;

	*ai_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
		"ClMIT_ChnValue::SetDefExchDate to %0:s" ) << ai_ExchDate << SEND_MSG   ;

	GET_TASK_ENV()->m_APIChn.ItemV_m_ExchDate=ai_ExchDate ; 
}

const m4char_t * ClMIT_ChnValue::GetDefExchCurr() 
{ 
	return GET_TASK_ENV()->m_APIChn.ItemV_m_pcExchCurr ; 
} 

m4date_t ClMIT_ChnValue::GetDefExchDate() 
{ 
	return GET_TASK_ENV()->m_APIChn.ItemV_m_ExchDate ; 
} 

const m4char_t *ClMIT_ChnValue::GetDefExchType( ) 
{ 
	return GET_TASK_ENV()->m_APIChn.ItemV_m_pcExchType ; 
} 

//-----------Inicializaciones basicas del objeto 

ClMIT_ChnValue::ClMIT_ChnValue() 
{
	m_poInsp=GET_INSP();
	m_Type=NO_INIT;
	m_RealType=NO_INIT;
	m_oFlags.Off(FIXED_TYPE) ; 
	ClearNULL() ;
}

//-----------Inicializaciones del objeto 


void ClMIT_ChnValue::End() 
{
	m_Type=NO_INIT;
	m_RealType=NO_INIT;
	m_oFlags.Off(FIXED_TYPE) ; 
	m_oStr.Accommodate(0) ;
	m_oExchType.Accommodate(0) ;
	ClearNULL() ;
	ClearUpdated() ;
}


ClMIT_ChnValue::TYPE ClMIT_ChnValue::GetRealType() const 
{
	return m_RealType;
}

void ClMIT_ChnValue::InitDouble( m4double_t  ai_dData, 
	m4int16_t ai_iPrecision, m4int16_t ai_iScale ) 
{
	if ( IsFixedType() && m_Type != NUMERIC && m_Type != VARIANT ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_10 ] ) 
			<< GetTypeString() << GetTypeString(NUMERIC) << SEND_MSG ;
		//Efecto local
	} else  {
		m_Type=NUMERIC ;
		m_RealType = m_Type;
		m_iPrecision=ai_iPrecision; 
		m_iScale=ai_iScale ;
		m_dDouble=ai_dData ;
		ClearNULL() ;
		m_oFlags.On(UPDATED) ;
	} ;
}

void ClMIT_ChnValue::InitBoolean( m4bool_t ai_bSw ) 
{
	if ( IsFixedType() && m_Type != BOOLEAN && m_Type != VARIANT ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_10 ] ) 
			<< GetTypeString() << GetTypeString(BOOLEAN) << SEND_MSG ;
		//Efecto local
	} else {
		m_Type=BOOLEAN;
		m_RealType = m_Type;
		m_dDouble=ai_bSw ? DOUBLE_VALID_BOOLEAN_TRUE: DOUBLE_VALID_BOOLEAN_FALSE  ;
		ClearNULL() ;
		m_oFlags.On(UPDATED) ;
	} ;
}

void ClMIT_ChnValue::InitString( const m4char_t  * ai_pcData ) 
{
	if ( IsFixedType() && m_Type != STRING && m_Type != VARIANT ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_10 ] ) 
			<< GetTypeString() << GetTypeString(STRING) << SEND_MSG ;
		//Efecto local
	} else { 

		m_Type=STRING ;
		m_RealType = m_Type;
		m_oStr.StrCpy(ai_pcData) ;
		ClearNULL() ;
		m_oFlags.On(UPDATED) ;
	} ;
}

void ClMIT_ChnValue::InitDate( m4date_t ai_Date ) 
{
	ClMIT_Dates oFecha ;

	if ( IsFixedType() && m_Type != DATE && m_Type != VARIANT ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_10 ] ) 
			<< GetTypeString() << GetTypeString(DATE) << SEND_MSG ;
		//Efecto local
	} else {

		m_Type=DATE ;
		m_RealType = m_Type;
		m_Date=ai_Date ;
		
		if ( ClMIT_Dates::IsInfinite(m_Date) ) {
			SetNULL() ;
		} else {
			ClearNULL() ;
		} ;
		m_oFlags.On(UPDATED) ;
	} ;
}

void ClMIT_ChnValue::InitTimeStamp( m4date_t ai_Date ) 
{
	ClMIT_Dates oFecha ;

	if ( IsFixedType() && m_Type != TIME_STAMP && m_Type != VARIANT ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_10 ] ) 
			<< GetTypeString() << GetTypeString(TIME_STAMP) << SEND_MSG ;
		//Efecto local
	} else {

		m_Type=TIME_STAMP ;
		m_RealType = m_Type;
		m_Date=ai_Date ;
		
		if ( ClMIT_Dates::IsInfinite(m_Date) ) {
			SetNULL() ;
		} else {
			ClearNULL() ;
		} ;
		m_oFlags.On(UPDATED) ;
	} ;
}

void ClMIT_ChnValue::InitCurrency( 
		m4double_t ai_dCurValue,	m4int16_t ai_iPrecision, m4int16_t ai_iScale,
		const m4char_t* ai_pcCurr, m4date_t ai_ExchDate, const m4char_t*  ai_pcExchType ) 
{
	if ( IsFixedType() && m_Type != CURRENCY && m_Type != VARIANT ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_10 ] ) 
			<< GetTypeString() << GetTypeString(CURRENCY) << SEND_MSG ;
		//Efecto local
	} else {

		m_Type=CURRENCY;
		m_RealType = m_Type;

		m_dDouble = ai_dCurValue ;
		m_iPrecision=ai_iPrecision ; 
		m_iScale=ai_iScale ; 
		m_oStr.StrCpy(ai_pcCurr ? ai_pcCurr : "" ) ;
		m_Date = ai_ExchDate ;
		m_oExchType.StrCpy(ai_pcExchType ? ai_pcExchType: "" ) ;

		ClearNULL() ;
		m_oFlags.On(UPDATED) ;
	} ;
}

void ClMIT_ChnValue::InitVariant(const m4char_t * ai_pcData, TYPE ai_eRealType)
{
	if (IsFixedType() && m_Type != VARIANT)
	{
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_10 ] ) << GetTypeString() << GetTypeString(VARIANT) << SEND_MSG;
		//Efecto local
	}
	else
	{
		m_Type = VARIANT;
		m_RealType = ai_eRealType;
		m_oStr.StrCpy(ai_pcData);
		ClearNULL();
		m_oFlags.On(UPDATED);
	}
}

void ClMIT_ChnValue::InitVariant(m4double_t ai_dData, TYPE ai_eRealType)
{
	if (IsFixedType() && m_Type != VARIANT)
	{
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_10 ] ) << GetTypeString() << GetTypeString(VARIANT) << SEND_MSG;
		//Efecto local
	}
	else
	{
		m_Type = VARIANT;
		m_RealType = ai_eRealType;

		if (ai_eRealType == NUMERIC)
		{
			m_iPrecision = NULL_DEFAULT_VARIANT_P;
			m_iScale = NULL_DEFAULT_VARIANT_S;
			m_dDouble = ai_dData;
			ClearNULL();
		}
		else if (DATE || TIME_STAMP)
		{
			m_Date = ai_dData;
			
			if (ClMIT_Dates::IsInfinite(m_Date))
			{
				SetNULL();
			}
			else
			{
				ClearNULL();
			}
		}
		
		m_oFlags.On(UPDATED);
	}
}

void ClMIT_ChnValue::InitVariant(m4double_t ai_dCurValue, const m4char_t* ai_pcCurr, m4date_t ai_ExchDate, const m4char_t*  ai_pcExchType, TYPE ai_eRealType)
{
	if (IsFixedType() && m_Type != VARIANT)
	{
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_10 ] ) << GetTypeString() << GetTypeString(VARIANT) << SEND_MSG;
		//Efecto local
	}
	else
	{
		m_Type = VARIANT;
		m_RealType = ai_eRealType;

		m_dDouble = ai_dCurValue;
		m_iPrecision = NULL_DEFAULT_VARIANT_P;
		m_iScale = NULL_DEFAULT_VARIANT_S;
		m_oStr.StrCpy(ai_pcCurr ? ai_pcCurr : "" );
		m_Date = ai_ExchDate;
		m_oExchType.StrCpy(ai_pcExchType ? ai_pcExchType: "" );

		ClearNULL();
		m_oFlags.On(UPDATED);
	}
}

void ClMIT_ChnValue::Init(  const ClMIT_ChnValue *m_poSource ) 
{
	if (IsFixedType() && (m_Type != m_poSource->m_Type ))
	{
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_10 ] ) 
			<< GetTypeString() << m_poSource->GetTypeString() << SEND_MSG ;
		//Efecto local
	} ;

	switch (m_poSource->m_Type) 
	{
	case NUMERIC:
		InitDouble(m_poSource->GetDouble(), m_poSource->GetPrecision(), 
			m_poSource->GetScale() ) ;
		break;
	case DATE:
		InitDate(m_poSource->GetDate()) ;
		break;
	case TIME_STAMP:
		InitTimeStamp(m_poSource->GetDate()) ;
		break;
	case STRING:
		InitString(m_poSource->GetString()) ;
		break;
	case BOOLEAN:
		InitBoolean(m_poSource->GetBoolean()) ;
		break;
	case CURRENCY:
		InitCurrency(
			m_poSource->GetCurrencyValue(), 
			m_poSource->GetPrecision(), m_poSource->GetScale(), 
			m_poSource->GetCurrencyType(),
			m_poSource->GetCurrencyExchDate(), m_poSource->GetCurrencyExchType());
		break;
	case VARIANT:
		if (m_poSource->GetRealType() == ClMIT_ChnValue::STRING)
		{
			InitVariant(m_poSource->GetVariantString(), m_poSource->GetRealType());
		}
		else if (m_poSource->GetRealType() == ClMIT_ChnValue::CURRENCY)
		{
			InitVariant(m_poSource->GetVariantNumber(), m_poSource->GetCurrencyType(), m_poSource->GetCurrencyExchDate(), m_poSource->GetCurrencyExchType(), m_poSource->GetRealType());
		}
		else 
		{
			InitVariant(m_poSource->GetVariantNumber(), m_poSource->GetRealType());
		}

		break;
	case NO_INIT:
		m_Type=NO_INIT;
		m_RealType=NO_INIT;
		break;
	default:
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_20 ] ) << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
		break;
	} ;

	m_oFlags.Switch(NULL_VALUE, m_poSource->IsNULL() ) ; 
	m_oFlags.Switch(UPDATED,    m_poSource->IsUpdated()) ;

}


//-----------Manejo de flag NULL. 

void ClMIT_ChnValue::SetNULL(TYPE ai_Type) 
{
	m_oFlags.On(NULL_VALUE) ;
	m_oFlags.Off(UPDATED) ;

	switch (ai_Type) 
	{
	case NUMERIC:
		m_Type=NUMERIC ;
		if (!IsFixedType()) {
			m_iPrecision=NULL_DEFAULT_NUMERIC_P; 
			m_iScale=NULL_DEFAULT_NUMERIC_S ;
		};
		m_dDouble=NULL_DEFAULT_NUMERIC_V ;
		break;
	case DATE:
		m_Type=DATE ;
		m_Date=NULL_DEFAULT_DATE ;
		break;
	case TIME_STAMP:
		m_Type=TIME_STAMP ;
		m_Date=NULL_DEFAULT_TS ;
		break;
	case STRING:
		m_Type=STRING ;
		m_oStr.StrCpy(NULL_DEFAULT_STRING) ;
		break;
	case BOOLEAN:
		m_Type=BOOLEAN;
		m_dDouble=NULL_DEFAULT_BOOLEAN;
		break;
	case CURRENCY:
		m_Type=CURRENCY;
		m_dDouble = NULL_DEFAULT_CURRENCY_V ;
		if (!IsFixedType()) {
			m_iPrecision=NULL_DEFAULT_CURRENCY_P ; 
			m_iScale=NULL_DEFAULT_CURRENCY_S ; 
		};
		m_oStr.StrCpy(NULL_DEFAULT_CURRENCY_T) ;
		m_Date = NULL_DEFAULT_CURRENCY_ED ;
		m_oExchType.StrCpy(NULL_DEFAULT_CURRENCY_ET) ;
		break;
	case VARIANT:
		m_Type = VARIANT;
		m_oStr.StrCpy(NULL_DEFAULT_STRING);
		m_dDouble = NULL_DEFAULT_VARIANT_V ;
		m_Date = NULL_DEFAULT_TS;
		m_oExchType.StrCpy(NULL_DEFAULT_CURRENCY_ET) ;	
		if (!IsFixedType())
		{
			m_iPrecision=NULL_DEFAULT_VARIANT_P ; 
			m_iScale=NULL_DEFAULT_VARIANT_S ; 
		}
		break;
	case NO_INIT:
		m_Type=NO_INIT;
		break;
	default:
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_20 ] ) << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
		break;
	} ;
	
	m_RealType = m_Type;
} 

//-----------Sobrecargas para usar el objeto con STL


ClMIT_ChnValue & ClMIT_ChnValue::operator = (const ClMIT_ChnValue &ai_oOpe)
{
	Init(&ai_oOpe) ;
	return *this ;
}

m4bool_t ClMIT_ChnValue::operator == (const ClMIT_ChnValue &ai_oOpe) const 
{
	ClMIT_ChnValue R ;

	Evaluate( EQUAL, &ai_oOpe, &R  ) ;
	return R.GetBoolean() ;
}

m4bool_t ClMIT_ChnValue::operator < (const ClMIT_ChnValue &ai_oOpe) const
{
	ClMIT_ChnValue R ;

	Evaluate( LESS, &ai_oOpe, &R  ) ;
	return R.GetBoolean() ;
}


ClMIT_ChnValue::ClMIT_ChnValue(const ClMIT_ChnValue& ai_oSource) 
{ 
	m_poInsp=GET_INSP() ;
	m_Type=NO_INIT;
	m_RealType = NO_INIT;
	m_oFlags.Off(FIXED_TYPE) ;
	Init(&ai_oSource); //Pone IsNULL segun el origen
}

//-------------------------------Gets & Sets------------------------------------------	


ClMIT_ChnValue::StOperatorData const * const ClMIT_ChnValue::GetOpeInfo( 
		ClMIT_ChnValue::TYPE ai_Type, ClMIT_ChnValue::OPERATOR ai_Ope) 
{ 
	ClMIT_ChnValue::StOperatorData const * r ;
	size_t idx1=ai_Type, idx2=ai_Ope ;
	
	//Puesto de esta forma porque si no el compiler AIX 5.1 en release se lia 
	r = & (ClMIT_ChnValue::m_stOpeData[idx1][idx2]) ;
	
	
	return r ;
} 




m4int16_t ClMIT_ChnValue::GetScale() const 
{ 
	if ( m_Type != CURRENCY && m_Type != NUMERIC && m_Type != VARIANT )
	{
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_30 ] ) 
			<< "ClMIT_ChnValue::GetScale()" << SEND_MSG ;
		//Efecto local

		if (m_Type == VARIANT)
		{
			return NULL_DEFAULT_VARIANT_S;
		}
		else
		{
			return NULL_DEFAULT_NUMERIC_S;
		}
	} 
	else
	{
		return m_iScale;
	} ;
}			

m4int16_t ClMIT_ChnValue::GetPrecision() const 
{ 
	if ( m_Type != CURRENCY && m_Type != NUMERIC  && m_Type != VARIANT )
	{
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_30 ] ) 
			<< "ClMIT_ChnValue::GetPrecision()" << SEND_MSG ;
		//Efecto local

		if (m_Type == VARIANT)
		{
			return NULL_DEFAULT_VARIANT_P;
		}
		else
		{
			return NULL_DEFAULT_NUMERIC_P ;
		}
	}
	else
	{
		return m_iPrecision;
	} ;
}	

void ClMIT_ChnValue::SetScale(m4int16_t ai_iScale)  
{ 
	if ( m_Type != CURRENCY && m_Type != NUMERIC  && m_Type != VARIANT ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_30 ] ) 
			<< "ClMIT_ChnValue::SetScale()" << SEND_MSG ;
		//Efecto local
		return ;
	} else {
		m_iScale=ai_iScale ; 
	} ;
}	

m4double_t ClMIT_ChnValue::GetCurrencyValue() const 
{ 
	if ( m_Type != CURRENCY && m_Type != VARIANT ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_30 ] ) 
			<< "ClMIT_ChnValue::GetCurrencyValue()" << SEND_MSG ;
		//Efecto local
		return NULL_DEFAULT_CURRENCY_V ;
	} else {
		return m_dDouble ; 
	} ;
}

m4char_t*  ClMIT_ChnValue::GetCurrencyType() const  
{ 
	if ( m_Type != CURRENCY && m_Type != VARIANT ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_30 ] ) 
			<< "ClMIT_ChnValue::GetCurrencyName()" << SEND_MSG ;
		//Efecto local
		return NULL_DEFAULT_CURRENCY_T ;
	} else {
		return m_oStr.InChar(0); 
	} ;
} 

m4date_t   ClMIT_ChnValue::GetCurrencyExchDate() const  
{ 
	if ( m_Type != CURRENCY && m_Type != VARIANT ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_30 ] ) 
			<< "ClMIT_ChnValue::GetExchangeDate()" << SEND_MSG ;
		//Efecto local
		return NULL_DEFAULT_CURRENCY_ED ;
	} else {
		return m_Date ; 
	} ;
}

m4char_t*  ClMIT_ChnValue::GetCurrencyExchType() const  
{ 
	if ( m_Type != CURRENCY && m_Type != VARIANT ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_30 ] ) 
			<< "ClMIT_ChnValue::GetExchangeType()" << SEND_MSG ;
		//Efecto local
		return NULL_DEFAULT_CURRENCY_ET ;
	} else {
		return m_oExchType.InChar(0); 
	} ;
}   

m4bool_t ClMIT_ChnValue::ExchangeCurrency( const m4char_t* ai_pcDestCur )
{
	if ( m_Type != CURRENCY && m_Type != VARIANT ){

		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_30 ] ) 
			<< "ClMIT_ChnValue::GetCurrencyExchange()" << SEND_MSG ;
		return M4_FALSE;

	} else {

		m_oFlags.On(UPDATED) ;

		return ClMIT_Chn::ExchangeCurrency( 
			&m_oStr, ai_pcDestCur, GetCurrencyExchDate(), GetCurrencyExchType(), 
			&m_dDouble );
	} ;
}

m4double_t ClMIT_ChnValue::GetDouble()  const 
{ 
	if ( m_Type != NUMERIC && m_Type != VARIANT )
	{
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_30 ] ) 
			<< "ClMIT_ChnValue::GetDouble()" << SEND_MSG ;
		//Efecto local
		
		if (m_Type == VARIANT)
		{
			return NULL_DEFAULT_VARIANT_V;
		}
		else
		{
			return NULL_DEFAULT_NUMERIC_V ;
		}

	}
	else
	{
		return m_dDouble;
	} ;
}

m4char_t * ClMIT_ChnValue::GetString() const 
{ 
	if ( m_Type != STRING && m_Type != VARIANT ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_30 ] ) 
			<< "ClMIT_ChnValue::GetString()" << SEND_MSG ;
		//Efecto local
		return NULL_DEFAULT_STRING ;
	} else {
		return m_oStr.InChar(0) ; 
	} ;
}	

m4date_t ClMIT_ChnValue::GetDate( ) const  
{
	if ( m_Type != DATE && m_Type != TIME_STAMP && m_Type != VARIANT ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_30 ] ) 
			<< "ClMIT_ChnValue::GetDate()" << SEND_MSG ;
		//Efecto local
		return NULL_DEFAULT_DATE ;
	} else {
		return m_Date ; 
	} ;
}

m4bool_t ClMIT_ChnValue::GetBoolean() const  
{ 
	if ( m_Type != BOOLEAN && m_Type != VARIANT ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_30 ] ) 
			<< "ClMIT_ChnValue::GetBoolean()" << SEND_MSG ;
		//Efecto local
		return NULL_DEFAULT_BOOLEAN ;
	} else {
		return M4_BOOL(m_dDouble == DOUBLE_VALID_BOOLEAN_TRUE); 
	} ;
} 

m4char_t * ClMIT_ChnValue::GetVariantString() const 
{ 
	if (m_Type != VARIANT)
	{
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_30 ] ) << "ClMIT_ChnValue::GetVariant()" << SEND_MSG;
		return NULL_DEFAULT_STRING;
	}
	else
	{
		if (m_RealType == STRING)
		{
			return m_oStr.InChar(0);
		}
		else
		{
			return NULL_DEFAULT_STRING;
		}
	}
}	

m4double_t ClMIT_ChnValue::GetVariantNumber() const 
{ 
	if (m_Type != VARIANT)
	{
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_30 ] ) << "ClMIT_ChnValue::GetVariant()" << SEND_MSG;
		return NULL_DEFAULT_NUMERIC_V;
	}
	else
	{
		if (m_RealType == NUMERIC || m_RealType == CURRENCY)
		{
			return m_dDouble;
		}
		else if (m_RealType == DATE || m_RealType == TIME_STAMP)
		{
			return m_Date;
		}
		else
		{
			return NULL_DEFAULT_NUMERIC_V;
		}
	}
}	

void ClMIT_ChnValue::GetTraceValue( ClMIT_Str *ao_poStr ) const 
{
	ClMIT_Str			strAux ;
	FORMAT_DATA_TYPE	type ;
	TYPE				eRealType = NO_INIT;

	ao_poStr->StrCpy( GetTypeString() ) ;
	ao_poStr->StrCat( ":") ;

	switch (m_Type)
	{
	case NUMERIC:

		GetInputFormatDouble(GetDouble(),GetPrecision(),GetScale(), &strAux, &type) ;
		ao_poStr->StrCat( strAux) ;

		break ;

	case DATE:

		GetInputFormatDate(GetDate(),&strAux,&type) ;
		ao_poStr->StrCat( strAux) ;

		break ;

	case TIME_STAMP:

		GetInputFormatTimeStamp(GetDate(),&strAux,&type) ;
		ao_poStr->StrCat( strAux) ;

		break ;

	case STRING:

		GetInputFormatString(GetString(),&strAux,&type) ;
		ao_poStr->StrCat( strAux) ;

		break ;

	case BOOLEAN:

		GetInputFormatBoolean(GetBoolean(),&strAux,&type) ;
		ao_poStr->StrCat( strAux) ;
		
		break ;

	case CURRENCY:

		GetInputFormatCurrency(GetCurrencyValue(),GetPrecision(),GetScale(),&strAux,&type) ;
		ao_poStr->StrCat( strAux) ;

		break ;

	case VARIANT:

		eRealType = GetRealType();
		if (eRealType == STRING)
		{
			GetInputFormatVariant(GetVariantString(), &strAux, &type);
		}
		else
		{
			GetInputFormatVariant(eRealType, GetVariantNumber(), GetPrecision(), GetScale(), &strAux, &type);
		}
		
		ao_poStr->StrCat(strAux);
		
		break ;

	case NO_INIT:
	default:
		break ;
	} ;
}


// ------------------------------Valores anonimos --------------------------------	

void ClMIT_ChnValue::InitAnonymous(const m4char_t    *ai_pcAnonymousValue ) 
{
	switch (m_Type)
	{
	case NUMERIC: {

		m4double_t V;
		m4int16_t  P, S ;
		
		V = GetAnonymousDouble(ai_pcAnonymousValue,&P,&S)  ;
		InitDouble( V, P, S ) ;

		break ;
	  } ;
	case STRING:

		InitString( GetAnonymousString(ai_pcAnonymousValue) ) ;

		break ;

	case DATE:

		InitDate( GetAnonymousDate(ai_pcAnonymousValue) );

		break ;

	case TIME_STAMP:

		InitTimeStamp( GetAnonymousTimeStamp(ai_pcAnonymousValue) );

		break ;

	case BOOLEAN:

		InitBoolean( GetAnonymousBoolean(ai_pcAnonymousValue) );
		
		break ;

	default:

		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_50 ] ) << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
		break;
	} ;
}



m4double_t ClMIT_ChnValue::GetAnonymousDouble()  const 
{
	if ( m_Type != STRING ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_60 ] ) << SEND_MSG ;
		//Efecto local
		return NULL_DEFAULT_NUMERIC_V ;
	} else {
		return GetAnonymousDouble( GetString() ) ;
	} ;
}

m4date_t ClMIT_ChnValue::GetAnonymousDate() const  
{
	if ( m_Type != STRING ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_60 ] ) << SEND_MSG ;
		//Efecto local
		return NULL_DEFAULT_DATE ;
	} else {
		return GetAnonymousDate( GetString() ) ;
	} ;
}


m4date_t ClMIT_ChnValue::GetAnonymousTimeStamp() const  
{
	if ( m_Type != STRING ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_60 ] ) << SEND_MSG ;
		//Efecto local
		return NULL_DEFAULT_TS ;
	} else {
		return GetAnonymousTimeStamp( GetString() ) ;
	} ;
}

const m4char_t * ClMIT_ChnValue::GetAnonymousString() const  
{
	if ( m_Type != STRING ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_60 ] ) << SEND_MSG ;
		//Efecto local
		return NULL_DEFAULT_STRING;
	} else {
		return GetAnonymousString( GetString() ) ;
	} ;
}

m4bool_t ClMIT_ChnValue::GetAnonymousBoolean() const  
{
	if ( m_Type != STRING ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_60 ] ) << SEND_MSG ;
		//Efecto local
		return NULL_DEFAULT_BOOLEAN ;
	}  else {
		return GetAnonymousBoolean( GetString() ) ;
	} ;
}

m4double_t ClMIT_ChnValue::GetAnonymousDouble(const m4char_t *ai_pcAnonymousValue)  
{
	return atof(ai_pcAnonymousValue) ;
}

m4double_t ClMIT_ChnValue::GetAnonymousDouble(const m4char_t *ai_pcAnonymousValue, 
	m4int16_t *ao_iPrecision, m4int16_t *ao_iScale ) 
{
	m4pcchar_t PosDec ;

	PosDec=strchr(ai_pcAnonymousValue, '.' ) ;
	if (PosDec) {
		*ao_iPrecision = PosDec-ai_pcAnonymousValue;
		*ao_iScale     = ClMIT_Str::StrLenTrim(ai_pcAnonymousValue)-(PosDec-ai_pcAnonymousValue)-1;
	} else {
		*ao_iPrecision = ClMIT_Str::StrLenTrim(ai_pcAnonymousValue) ;
		*ao_iScale	   = 0 ;  
	} ;
	return atof(ai_pcAnonymousValue) ; 

}

m4date_t ClMIT_ChnValue::GetAnonymousDate(const m4char_t *ai_pcAnonymousValue)  
{
	return ClMIT_Dates::StringToDate(ai_pcAnonymousValue)	;
}

m4date_t ClMIT_ChnValue::GetAnonymousTimeStamp(const m4char_t *ai_pcAnonymousValue)  
{
	return ClMIT_Dates::StringToDate(ai_pcAnonymousValue)	; 
}

const m4char_t * ClMIT_ChnValue::GetAnonymousString(const m4char_t *ai_pcAnonymousValue)  
{
	return ai_pcAnonymousValue ;
}

m4bool_t ClMIT_ChnValue::GetAnonymousBoolean(const m4char_t *ai_pcAnonymousValue) 
{
	if ( ClMIT_Str::StrCmpTrim(ai_pcAnonymousValue, STR_VALID_BOOLEAN_TRUE ) == 0 ) { 
		return M4_TRUE ;
	} else if ( ClMIT_Str::StrCmpTrim(ai_pcAnonymousValue, STR_VALID_BOOLEAN_FALSE ) == 0 ) { 
		return M4_FALSE ;
	} else {
		*(GET_INSP()->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_40 ] ) << SEND_MSG ;
		//Efecto local
		return NULL_DEFAULT_BOOLEAN ;
	} ;
}


void ClMIT_ChnValue::DoubleToAnonymous(m4double_t ai_dDouble, ClMIT_Str *ao_Str, 
		m4int16_t ai_iScale ) 
{
	m4char_t *Format=GET_TASK_ENV()->m_APIChn.ItemV_m_pcBuffFormat ;

	if ( ai_iScale<0 ) {
		sprintf( Format, "%f", (double)ai_dDouble ) ;
	} else {
		sprintf( Format, "%.*f", (int)ai_iScale,(double)ai_dDouble ) ;
	} ;

	ClMIT_Str::OverStringValid( Format, StMIT_APIChn_TaskEnv::ItemV_SIZE_STR_FORMAT ) ;
	ao_Str->StrCpy(Format) ;
}

void ClMIT_ChnValue::NumberToAnonymous(m4double_t ai_dDouble, ClMIT_Str *ao_Str ) 
{
	m4char_t *Format=GET_TASK_ENV()->m_APIChn.ItemV_m_pcBuffFormat ;

	sprintf( Format, "%li", (long int)ai_dDouble ) ;
	ClMIT_Str::OverStringValid( Format, StMIT_APIChn_TaskEnv::ItemV_SIZE_STR_FORMAT ) ;
	ao_Str->StrCpy(Format) ;
}

void ClMIT_ChnValue::StringToAnonymous(const m4char_t * ai_pcStr, ClMIT_Str *ao_Str)  
{
	ao_Str->StrCpy(ai_pcStr) ;
}

void ClMIT_ChnValue::BooleanToAnonymous(m4bool_t   ai_Bool, ClMIT_Str *ao_Str) 
{
	ao_Str->StrCpy( ai_Bool ? STR_VALID_BOOLEAN_TRUE : STR_VALID_BOOLEAN_FALSE ) ;
}


// ------------------------------Formateos (relacionados con str anonimas)------------	


m4bool_t ClMIT_ChnValue::GetInputFormat( ClMIT_Str *ao_poStr, FORMAT_DATA_TYPE *ao_poType ) const  
{
	m4bool_t	R ;
	TYPE		eRealType = NO_INIT;

	switch (m_Type)
	{
	case NUMERIC:

		R=GetInputFormatDouble(GetDouble(),GetPrecision(),GetScale(),ao_poStr,ao_poType) ;

		break ;

	case DATE:

		R=GetInputFormatDate(GetDate(),ao_poStr,ao_poType) ;

		break ;

	case TIME_STAMP:

		R=GetInputFormatTimeStamp(GetDate(),ao_poStr,ao_poType) ;

		break ;

	case STRING:

		R=GetInputFormatString(GetString(),ao_poStr,ao_poType) ;

		break ;

	case BOOLEAN:

		R=GetInputFormatBoolean(GetBoolean(),ao_poStr,ao_poType) ;
		
		break ;

	case CURRENCY:

		R=GetInputFormatCurrency(GetCurrencyValue(),GetPrecision(),GetScale(),ao_poStr,ao_poType) ;

		break ;

		
	case VARIANT:

		eRealType = GetRealType();
		if (eRealType == STRING)
		{
			R=GetInputFormatVariant(GetVariantString(),  ao_poStr, ao_poType);
		}
		else
		{
			R=GetInputFormatVariant(eRealType, GetVariantNumber(), GetPrecision(), GetScale(), ao_poStr, ao_poType);
		}
		
		break;

	case NO_INIT:
	default:
		ao_poStr->StrCpy( "" ) ; 
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_140 ] ) 
			<< GetTypeString() << SEND_MSG ;
		m_poInsp->m_poTrace->NotifyDummy() ;
		R=M4_FALSE ;

		break ;
	} ;
	return R ;
}

m4bool_t ClMIT_ChnValue::GetInputFormatDouble( 
		m4double_t  ai_dDouble , m4int16_t ai_iPrecision, m4int16_t ai_iScale,
		ClMIT_Str *ao_poStr, FORMAT_DATA_TYPE *ao_pType )  
{
	m4char_t *Format=GET_TASK_ENV()->m_APIChn.ItemV_m_pcBuffFormat ;

	// Bug 0151080. Para los valores inferiores a la precisión M4_LN4_MATH_PRECISION (-10e-9)
	// se toma su valor absoluto.
	if (ai_dDouble >= -(M4_LN4_MATH_PRECISION))
	{
		ai_dDouble = fabs(ai_dDouble);
	}

	sprintf( Format, "%.*f", (int)ai_iScale,(double)ai_dDouble ) ;
	ClMIT_Str::OverStringValid( Format, StMIT_APIChn_TaskEnv::ItemV_SIZE_STR_FORMAT ) ;
	if (ao_pType) *ao_pType=FORMAT_DOUBLE ;
	ao_poStr->StrCpy(Format) ;
	return M4_TRUE ;
}

m4bool_t ClMIT_ChnValue::GetInputFormatDate( 
		m4date_t  ai_Date, 
		ClMIT_Str *ao_poStr, FORMAT_DATA_TYPE *ao_pType )  
{
	m4char_t *Format=GET_TASK_ENV()->m_APIChn.ItemV_m_pcBuffFormat ;
	ClMIT_Str  Str ;

	ClMIT_Dates::DateToDateStr(ao_poStr, ai_Date);
	if (ao_pType) *ao_pType=FORMAT_DATE ;
	return M4_TRUE ;
}

m4bool_t ClMIT_ChnValue::GetInputFormatTimeStamp( 
		m4date_t  ai_Date, 
		ClMIT_Str *ao_poStr, FORMAT_DATA_TYPE *ao_pType )  
{
	m4char_t *Format=GET_TASK_ENV()->m_APIChn.ItemV_m_pcBuffFormat ;
	ClMIT_Str  Str ;

	ClMIT_Dates::DateToTimeStampStr(ao_poStr, ai_Date); 
	if (ao_pType) *ao_pType=FORMAT_TIME_STAMP ;
	return M4_TRUE ;
}

m4bool_t ClMIT_ChnValue::GetInputFormatString( 
		m4char_t  *ai_pcStr,
		ClMIT_Str *ao_poStr, FORMAT_DATA_TYPE *ao_pType )  
{
	ao_poStr->StrCpy(ai_pcStr) ;
	if (ao_pType) *ao_pType=FORMAT_STRING ;
	return M4_TRUE ;
}

m4bool_t ClMIT_ChnValue::GetInputFormatBoolean( 
		m4bool_t ai_bSw,
		ClMIT_Str *ao_poStr, FORMAT_DATA_TYPE *ao_pType )  
{
	if ( ai_bSw ) ao_poStr->StrCpy( STR_VALID_BOOLEAN_TRUE ) ;
	else ao_poStr->StrCpy( STR_VALID_BOOLEAN_FALSE ) ; 
	return M4_FALSE ;
}

m4bool_t ClMIT_ChnValue::GetInputFormatCurrency( 
		m4double_t  ai_dDouble , m4int16_t ai_iPrecision, m4int16_t ai_iScale,
		ClMIT_Str *ao_poStr, FORMAT_DATA_TYPE *ao_pType )  
{
	m4char_t *Format=GET_TASK_ENV()->m_APIChn.ItemV_m_pcBuffFormat ;

	// Bug 0151080. Para los valores inferiores a la precisión M4_LN4_MATH_PRECISION (-10e-9)
	// se toma su valor absoluto.
	if (ai_dDouble >= -(M4_LN4_MATH_PRECISION))
	{
		ai_dDouble = fabs(ai_dDouble);
	}

	sprintf( Format, "%.*f", (int)ai_iScale,(double)ai_dDouble ) ;
	ClMIT_Str::OverStringValid( Format, StMIT_APIChn_TaskEnv::ItemV_SIZE_STR_FORMAT ) ;
	if (ao_pType) *ao_pType=FORMAT_DOUBLE ;
	ao_poStr->StrCpy(Format) ;
	return M4_TRUE ;
}

m4bool_t ClMIT_ChnValue::GetInputFormatVariant(m4char_t  *ai_pcValue, ClMIT_Str *ao_poStr, FORMAT_DATA_TYPE *ao_pType)
{
	return GetInputFormatString(ai_pcValue, ao_poStr, ao_pType);
}

m4bool_t ClMIT_ChnValue::GetInputFormatVariant(TYPE ai_eRealType, m4double_t ai_dValue, m4int16_t ai_iPrecision, m4int16_t ai_iScale, ClMIT_Str *ao_poStr, FORMAT_DATA_TYPE *ao_pType)
{
	m4return_t		iRet = M4_SUCCESS;
	
	if (ai_eRealType == NUMERIC)
	{
		iRet = GetInputFormatDouble(ai_dValue, ai_iPrecision, ai_iScale, ao_poStr, ao_pType);
	}
	else if (ai_eRealType == CURRENCY)
	{
		iRet = GetInputFormatCurrency(ai_dValue, ai_iPrecision, ai_iScale, ao_poStr, ao_pType);
	}
	else if (ai_eRealType == DATE)
	{
		iRet = GetInputFormatDate(ai_dValue, ao_poStr, ao_pType);
	}
	else if (ai_eRealType == TIME_STAMP)
	{
		iRet = GetInputFormatTimeStamp(ai_dValue, ao_poStr, ao_pType);
	}

	return iRet;
}


// ------------------------------Comparaciones logicas--------------------------------	


m4char_t const * ClMIT_ChnValue::m_pcOpeTraceStr[ClMIT_ChnValue::OPERATOR_COUNT]  = 
{
	"< LESS", "<= LESS_EQUAL", "= EQUAL", "<> NOT_EQUAL",
	"> GREATER", ">= GREATER_EQUAL",

	"& AND", "| OR", "! NOT", 

	"- MINUS_SIGN", "/ DIVIDE", "* MULTIPLY", "+ ADD", "- SUBSTRACT",

	"] ABSORB_A", "[ ABSORB_B",

	"&& STR_CAT"
} ;



m4int16_t const ClMIT_ChnValue::m_iOpeArgs[ClMIT_ChnValue::OPERATOR_COUNT]  = 
{
	/*LESS*/2, /*LESS_EQUAL*/2, /*EQUAL*/2, /*NOT_EQUAL*/2,
	/*GREATER*/2, /*GREATER_EQUAL*/2,

	/*AND*/2, /*OR*/2, /*NOT*/1, 

	/* MINUS_SIGN*/1, /*DIV*/2, /*MUL*/2, /*ADD*/2, /*SUBSTRACT*/2,

	/*ABSORB_A*/2, /*ABSORB_B*/2,

	/*STR_CAT*/2
} ;

ClMIT_ChnValue::StOperatorData const ClMIT_ChnValue::m_stOpeData[ClMIT_ChnValue::TYPE_COUNT][ClMIT_ChnValue::OPERATOR_COUNT] =
{
			
	/*NUMERIC*/	
	{
		/*LESS*/			{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::BOOLEAN }, 
		/*LESS_EQUAL*/		{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::BOOLEAN },
		/*EQUAL*/			{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::BOOLEAN }, 
		/*NOT_EQUAL*/		{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::BOOLEAN },
		/*GREATER*/			{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::BOOLEAN },
		/*GREATER_EQUAL*/	{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::BOOLEAN },

		/*AND*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*OR*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT}, 
		/*NOT*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},

		/* MINUS_SIGN*/		{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::NUMERIC},
		/*DIV*/				{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::NUMERIC},
		/*MUL*/				{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::NUMERIC}, 
		/*ADD*/				{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::NUMERIC},
		/*SUBSTRACT*/		{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::NUMERIC},

		/*ABSORB_A*/		{ClMIT_ChnValue::ALLOWED|ClMIT_ChnValue::DEAL_NULLS|ClMIT_ChnValue::MULTI_TYPE, 
								ClMIT_ChnValue::NO_INIT}, 
		/*ABSORB_B*/		{ClMIT_ChnValue::ALLOWED|ClMIT_ChnValue::DEAL_NULLS|ClMIT_ChnValue::MULTI_TYPE,
								ClMIT_ChnValue::NO_INIT},
		/*STR_CAT*/			{ClMIT_ChnValue::ALLOWED|ClMIT_ChnValue::DEAL_NULLS|ClMIT_ChnValue::MULTI_TYPE,
								ClMIT_ChnValue::STRING}
	},

	/*STRING*/	
	{
		/*LESS*/			{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::BOOLEAN }, 
		/*LESS_EQUAL*/		{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::BOOLEAN },
		/*EQUAL*/			{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::BOOLEAN }, 
		/*NOT_EQUAL*/		{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::BOOLEAN },
		/*GREATER*/			{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::BOOLEAN },
		/*GREATER_EQUAL*/	{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::BOOLEAN },

		/*AND*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*OR*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT}, 
		/*NOT*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},

		/* MINUS_SIGN*/		{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*DIV*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*MUL*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*ADD*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*SUBSTRACT*/		{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},

		/*ABSORB_A*/		{ClMIT_ChnValue::ALLOWED|ClMIT_ChnValue::DEAL_NULLS|ClMIT_ChnValue::MULTI_TYPE, 
								ClMIT_ChnValue::NO_INIT}, 
		/*ABSORB_B*/		{ClMIT_ChnValue::ALLOWED|ClMIT_ChnValue::DEAL_NULLS|ClMIT_ChnValue::MULTI_TYPE,
								ClMIT_ChnValue::NO_INIT},
		/*STR_CAT*/			{ClMIT_ChnValue::ALLOWED|ClMIT_ChnValue::DEAL_NULLS|ClMIT_ChnValue::MULTI_TYPE,
								ClMIT_ChnValue::STRING}
	},

	/*DATE*/	
	{
		/*LESS*/			{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::BOOLEAN }, 
		/*LESS_EQUAL*/		{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::BOOLEAN },
		/*EQUAL*/			{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::BOOLEAN }, 
		/*NOT_EQUAL*/		{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::BOOLEAN },
		/*GREATER*/			{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::BOOLEAN },
		/*GREATER_EQUAL*/	{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::BOOLEAN },

		/*AND*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*OR*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT}, 
		/*NOT*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},

		/* MINUS_SIGN*/		{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*DIV*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*MUL*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*ADD*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*SUBSTRACT*/		{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},

		/*ABSORB_A*/		{ClMIT_ChnValue::ALLOWED|ClMIT_ChnValue::DEAL_NULLS|ClMIT_ChnValue::MULTI_TYPE, 
								ClMIT_ChnValue::NO_INIT}, 
		/*ABSORB_B*/		{ClMIT_ChnValue::ALLOWED|ClMIT_ChnValue::DEAL_NULLS|ClMIT_ChnValue::MULTI_TYPE,
								ClMIT_ChnValue::NO_INIT},
		/*STR_CAT*/			{ClMIT_ChnValue::ALLOWED|ClMIT_ChnValue::DEAL_NULLS|ClMIT_ChnValue::MULTI_TYPE,
								ClMIT_ChnValue::STRING}
	},

	/*TIME_STAMP*/	
	{
		/*LESS*/			{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::BOOLEAN }, 
		/*LESS_EQUAL*/		{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::BOOLEAN },
		/*EQUAL*/			{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::BOOLEAN }, 
		/*NOT_EQUAL*/		{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::BOOLEAN },
		/*GREATER*/			{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::BOOLEAN },
		/*GREATER_EQUAL*/	{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::BOOLEAN },

		/*AND*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*OR*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT}, 
		/*NOT*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},

		/* MINUS_SIGN*/		{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*DIV*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*MUL*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*ADD*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*SUBSTRACT*/		{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},

		/*ABSORB_A*/		{ClMIT_ChnValue::ALLOWED|ClMIT_ChnValue::DEAL_NULLS|ClMIT_ChnValue::MULTI_TYPE, 
								ClMIT_ChnValue::NO_INIT}, 
		/*ABSORB_B*/		{ClMIT_ChnValue::ALLOWED|ClMIT_ChnValue::DEAL_NULLS|ClMIT_ChnValue::MULTI_TYPE,
								ClMIT_ChnValue::NO_INIT},
		/*STR_CAT*/			{ClMIT_ChnValue::ALLOWED|ClMIT_ChnValue::DEAL_NULLS|ClMIT_ChnValue::MULTI_TYPE,
								ClMIT_ChnValue::STRING}
	},

	/*BOOLEAN*/ 
	{
		/*LESS*/			{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT}, 
		/*LESS_EQUAL*/		{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*EQUAL*/			{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::BOOLEAN }, 
		/*NOT_EQUAL*/		{ClMIT_ChnValue::ALLOWED, ClMIT_ChnValue::BOOLEAN },
		/*GREATER*/			{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*GREATER_EQUAL*/	{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},

		/*AND*/				{ClMIT_ChnValue::ALLOWED|ClMIT_ChnValue::DEAL_NULLS, ClMIT_ChnValue::BOOLEAN},
		/*OR*/				{ClMIT_ChnValue::ALLOWED|ClMIT_ChnValue::DEAL_NULLS, ClMIT_ChnValue::BOOLEAN}, 
		/*NOT*/				{ClMIT_ChnValue::ALLOWED,ClMIT_ChnValue::BOOLEAN},

		/* MINUS_SIGN*/		{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*DIV*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*MUL*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*ADD*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*SUBSTRACT*/		{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},

		/*ABSORB_A*/		{ClMIT_ChnValue::ALLOWED|ClMIT_ChnValue::DEAL_NULLS|ClMIT_ChnValue::MULTI_TYPE, 
								ClMIT_ChnValue::NO_INIT}, 
		/*ABSORB_B*/		{ClMIT_ChnValue::ALLOWED|ClMIT_ChnValue::DEAL_NULLS|ClMIT_ChnValue::MULTI_TYPE,
								ClMIT_ChnValue::NO_INIT},
		/*STR_CAT*/			{ClMIT_ChnValue::ALLOWED|ClMIT_ChnValue::DEAL_NULLS|ClMIT_ChnValue::MULTI_TYPE,
								ClMIT_ChnValue::STRING}
	},
	
	/*CURRENCY*/	
	{
		/*LESS*/			{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT}, 
		/*LESS_EQUAL*/		{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*EQUAL*/			{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*NOT_EQUAL*/		{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*GREATER*/			{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*GREATER_EQUAL*/	{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},

		/*AND*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*OR*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT}, 
		/*NOT*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},

		/* MINUS_SIGN*/		{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*DIV*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*MUL*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*ADD*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*SUBSTRACT*/		{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},

		/*ABSORB_A*/		{ClMIT_ChnValue::ALLOWED|ClMIT_ChnValue::DEAL_NULLS|ClMIT_ChnValue::MULTI_TYPE, 
								ClMIT_ChnValue::NO_INIT}, 
		/*ABSORB_B*/		{ClMIT_ChnValue::ALLOWED|ClMIT_ChnValue::DEAL_NULLS|ClMIT_ChnValue::MULTI_TYPE,
								ClMIT_ChnValue::NO_INIT},
		/*STR_CAT*/			{ClMIT_ChnValue::ALLOWED|ClMIT_ChnValue::DEAL_NULLS|ClMIT_ChnValue::MULTI_TYPE,
								ClMIT_ChnValue::STRING}

	},

	/*VARIANT*/	
	{
		/*LESS*/			{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT}, 
		/*LESS_EQUAL*/		{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*EQUAL*/			{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*NOT_EQUAL*/		{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*GREATER*/			{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*GREATER_EQUAL*/	{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},

		/*AND*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*OR*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT}, 
		/*NOT*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},

		/* MINUS_SIGN*/		{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*DIV*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*MUL*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*ADD*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*SUBSTRACT*/		{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},

		/*ABSORB_A*/		{ClMIT_ChnValue::ALLOWED|ClMIT_ChnValue::DEAL_NULLS|ClMIT_ChnValue::MULTI_TYPE, 
								ClMIT_ChnValue::NO_INIT}, 
		/*ABSORB_B*/		{ClMIT_ChnValue::ALLOWED|ClMIT_ChnValue::DEAL_NULLS|ClMIT_ChnValue::MULTI_TYPE,
								ClMIT_ChnValue::NO_INIT},
		/*STR_CAT*/			{ClMIT_ChnValue::ALLOWED|ClMIT_ChnValue::DEAL_NULLS|ClMIT_ChnValue::MULTI_TYPE,
								ClMIT_ChnValue::STRING}

	},

	/*NO_INIT*/	
	{
		/*LESS*/			{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT}, 
		/*LESS_EQUAL*/		{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*EQUAL*/			{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*NOT_EQUAL*/		{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*GREATER*/			{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*GREATER_EQUAL*/	{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},

		/*AND*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*OR*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT}, 
		/*NOT*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},

		/* MINUS_SIGN*/		{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*DIV*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*MUL*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*ADD*/				{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},
		/*SUBSTRACT*/		{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT},

		/*ABSORB_A*/		{ClMIT_ChnValue::ALLOWED|ClMIT_ChnValue::DEAL_NULLS|ClMIT_ChnValue::MULTI_TYPE, 
								ClMIT_ChnValue::NO_INIT}, 
		/*ABSORB_B*/		{ClMIT_ChnValue::ALLOWED|ClMIT_ChnValue::DEAL_NULLS|ClMIT_ChnValue::MULTI_TYPE,
								ClMIT_ChnValue::NO_INIT},
		/*STR_CAT*/			{ClMIT_Flag::NONE_BITS, ClMIT_ChnValue::NO_INIT}
	}
} ;

m4bool_t ClMIT_ChnValue::Evaluate( 
		ClMIT_ChnValue::OPERATOR ai_Op, const ClMIT_ChnValue * ai_poOpeB, ClMIT_ChnValue * ao_poResult )  const
{
	m4bool_t SwOk=M4_TRUE, SwSetNULL=M4_FALSE ;
	const ClMIT_ChnValue *NULLValidOpe=NULL ;
	m4int16_t OpeArgs = GetOpeArgs(ai_Op) ;
	StOperatorData const * const OpeInfo = GetOpeInfo(GetType(),ai_Op)  ;
	ClMIT_Flag OpeInfoFlag( OpeInfo->m_oFlag ) ;

	// Test operador valido para este tipo

	if ( !OpeInfoFlag.True(ALLOWED) ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_85 ] ) 
			<< GetOpeString(ai_Op) << GetTypeString() << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	// Test NULL. Si operador no trata con NULLs, o todos operandos son NULL, resultado NULL.

	if ( IsNULL() || (OpeArgs==2 && ai_poOpeB->IsNULL()) ) {
		if ( OpeInfoFlag.False(DEAL_NULLS) || OpeArgs==1 || 
			( OpeArgs==2 && IsNULL() && ai_poOpeB->IsNULL() ) ) 
		{
			SwSetNULL=M4_TRUE ;
			goto exit ;
		} ;
		NULLValidOpe= IsNULL() ? ai_poOpeB : this ;
	} ;


	// Test MULTI_TYPE, SOLO si !NULLValidOpe. Si no es multitype, tipos deben coincidir 

	if ( !NULLValidOpe && OpeInfoFlag.False(MULTI_TYPE) && OpeArgs==2 
		&& GetType() != ai_poOpeB->GetType()  ) 
	{
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_80 ] ) 
			<< GetOpeString(ai_Op) << GetTypeString() << ai_poOpeB->GetTypeString() 
			<< SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;


	//--------------->Ahora, a operar. Los operandos son correctos. Si operador trata con NULLs
	//		lo primero que debe tratar es el caso NULLValidOpe. Es NULL si no hay nuigun NULL, y
	//		si no apunta al operando no-nulo.		


	//----Primero me quito de encima los operadores MULTI_TYPE

	if ( ai_Op==STR_CAT ) {

		ClMIT_Str StrA, StrB ;

		if ( NULLValidOpe ) {

			NULLValidOpe->GetInputFormat( &StrA ) ;
			ao_poResult->InitString(StrA.InChar(0)) ;

		} else {

			GetInputFormat( &StrA ) ;
			ai_poOpeB->GetInputFormat( &StrB ) ;
			StrA.StrCat(StrB.InChar(0)) ;
			ao_poResult->InitString(StrA.InChar(0)) ;
		} ;

	} else if (ai_Op==ABSORB_A) {

		ao_poResult->Init(ai_poOpeB) ;

	} else if (ai_Op==ABSORB_B) {

		ao_poResult->Init(this) ;

	} else {

		//----Y ahora voy a por los operadores especificos por tipo

		switch (GetType())
		{
		case NUMERIC:

			switch (ai_Op)
			{
			case LESS:
				ao_poResult->InitBoolean( M4_BOOL(GetDouble() < ai_poOpeB->GetDouble()) ) ;
				break;
			case LESS_EQUAL:
				ao_poResult->InitBoolean( M4_BOOL(GetDouble() <= ai_poOpeB->GetDouble()) ) ;
				break;
			case EQUAL:
				ao_poResult->InitBoolean( M4_BOOL(GetDouble() == ai_poOpeB->GetDouble()) ) ;
				break;
			case NOT_EQUAL:
				ao_poResult->InitBoolean( M4_BOOL(GetDouble() != ai_poOpeB->GetDouble()) );
				break;
			case GREATER:
				ao_poResult->InitBoolean( M4_BOOL(GetDouble() > ai_poOpeB->GetDouble()) ) ;
				break;
			case GREATER_EQUAL:
				ao_poResult->InitBoolean( M4_BOOL(GetDouble() >= ai_poOpeB->GetDouble()) ) ;
				break;

			case MINUS_SIGN:
				ao_poResult->InitDouble( - GetDouble(), GetPrecision(), GetScale() ) ;
				break;
			case DIV:
				if (ai_poOpeB->GetDouble()==0) {
					*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_88 ] ) 
						<< GetDouble() << SEND_MSG ;
					SwOk=M4_FALSE ;
					goto exit ;
				} ;
				ao_poResult->InitDouble( GetDouble() / ai_poOpeB->GetDouble(),
					M4_MAX( GetPrecision(), ai_poOpeB->GetPrecision() ), 
					M4_MAX( GetScale(),     ai_poOpeB->GetScale() ) ) ;
				break;
			case MUL:
				ao_poResult->InitDouble( GetDouble() * ai_poOpeB->GetDouble(),
					M4_MAX( GetPrecision(), ai_poOpeB->GetPrecision() ), 
					M4_MAX( GetScale(),     ai_poOpeB->GetScale() ) ) ;
				break;
			case ADD:
				ao_poResult->InitDouble( GetDouble() + ai_poOpeB->GetDouble(),
					M4_MAX( GetPrecision(), ai_poOpeB->GetPrecision() ), 
					M4_MAX( GetScale(),     ai_poOpeB->GetScale() ) ) ;
				break;
			case SUBSTRACT:
				ao_poResult->InitDouble( GetDouble() - ai_poOpeB->GetDouble(),
					M4_MAX( GetPrecision(), ai_poOpeB->GetPrecision() ), 
					M4_MAX( GetScale(),     ai_poOpeB->GetScale() ) ) ;
				break;

			default:
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_90 ] ) 
					<< GetOpeString(ai_Op) << GetTypeString() << SEND_MSG ;
				m_poInsp->m_poTrace->ImmediateError() ;
				break;
			} ;

			break ;

		case DATE:

			switch (ai_Op)
			{
			case LESS:
				ao_poResult->InitBoolean( M4_BOOL(ClMIT_Dates::CmpDate(GetDate(),ai_poOpeB->GetDate())==-1) ) ;
				break;
			case LESS_EQUAL:
				ao_poResult->InitBoolean( M4_BOOL(( ClMIT_Dates::CmpDate(GetDate(),ai_poOpeB->GetDate())==-1 ) || 
					( ClMIT_Dates::CmpDate(GetDate(),ai_poOpeB->GetDate())==0 )) ) ;
				break;
			case EQUAL:
				ao_poResult->InitBoolean( M4_BOOL(ClMIT_Dates::CmpDate(GetDate(), ai_poOpeB->GetDate())==0) ) ;
				break;
			case NOT_EQUAL:
				ao_poResult->InitBoolean( M4_BOOL(ClMIT_Dates::CmpDate(GetDate(), ai_poOpeB->GetDate())!=0) );
				break;
			case GREATER:
				ao_poResult->InitBoolean( M4_BOOL(ClMIT_Dates::CmpDate(GetDate(),ai_poOpeB->GetDate())==1) );
				break;
			case GREATER_EQUAL:
				ao_poResult->InitBoolean( M4_BOOL(( ClMIT_Dates::CmpDate(GetDate(),ai_poOpeB->GetDate())==1 ) || 
					( ClMIT_Dates::CmpDate(GetDate(),ai_poOpeB->GetDate())==0 )));
				break;

			default:
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_90 ] ) 
					<< GetOpeString(ai_Op) << GetTypeString() << SEND_MSG ;
				m_poInsp->m_poTrace->ImmediateError() ;
				break;
			} ;

			break ;

		case TIME_STAMP:

			switch (ai_Op)
			{
			case LESS:
				ao_poResult->InitBoolean( M4_BOOL(ClMIT_Dates::CmpTimeStamp(GetDate(),ai_poOpeB->GetDate())==-1) ) ;
				break;
			case LESS_EQUAL:
				ao_poResult->InitBoolean( M4_BOOL(( ClMIT_Dates::CmpTimeStamp(GetDate(),ai_poOpeB->GetDate())==-1 ) || 
					( ClMIT_Dates::CmpTimeStamp(GetDate(),ai_poOpeB->GetDate())==0 )) ) ;
				break;
			case EQUAL:
				ao_poResult->InitBoolean( M4_BOOL(ClMIT_Dates::CmpTimeStamp(GetDate(), ai_poOpeB->GetDate())==0) ) ;
				break;
			case NOT_EQUAL:
				ao_poResult->InitBoolean( M4_BOOL(ClMIT_Dates::CmpTimeStamp(GetDate(), ai_poOpeB->GetDate())!=0) );
				break;
			case GREATER:
				ao_poResult->InitBoolean( M4_BOOL(ClMIT_Dates::CmpTimeStamp(GetDate(),ai_poOpeB->GetDate())==1) );
				break;
			case GREATER_EQUAL:
				ao_poResult->InitBoolean( M4_BOOL(( ClMIT_Dates::CmpTimeStamp(GetDate(),ai_poOpeB->GetDate())==1 ) || 
					( ClMIT_Dates::CmpTimeStamp(GetDate(),ai_poOpeB->GetDate())==0 )));
				break;

			default:
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_90 ] ) 
					<< GetOpeString(ai_Op) << GetTypeString() << SEND_MSG ;
				m_poInsp->m_poTrace->ImmediateError() ;
				break;
			} ;

			break ;

		case STRING:
			{
				switch (ai_Op)
				{
				case LESS:
					ao_poResult->InitBoolean( M4_BOOL(ClMIT_Str::StrCmpTrim(GetString(),ai_poOpeB->GetString())<0) ) ;
					break;
				case LESS_EQUAL:
					ao_poResult->InitBoolean( M4_BOOL(ClMIT_Str::StrCmpTrim(GetString(),ai_poOpeB->GetString())<=0)) ;
					break;
				case EQUAL:
					ao_poResult->InitBoolean( M4_BOOL(ClMIT_Str::StrCmpTrim(GetString(),ai_poOpeB->GetString())==0) );
					break;
				case NOT_EQUAL:
					ao_poResult->InitBoolean( M4_BOOL(ClMIT_Str::StrCmpTrim(GetString(),ai_poOpeB->GetString())!=0 ));
					break;
				case GREATER:
					ao_poResult->InitBoolean( M4_BOOL(ClMIT_Str::StrCmpTrim(GetString(),ai_poOpeB->GetString())>0 ));
					break;
				case GREATER_EQUAL:
					ao_poResult->InitBoolean( M4_BOOL(ClMIT_Str::StrCmpTrim(GetString(),ai_poOpeB->GetString())>=0 ));
					break;

				default:
					*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_90 ] ) 
						<< GetOpeString(ai_Op) << GetTypeString() << SEND_MSG ;
					m_poInsp->m_poTrace->ImmediateError() ;
					break;
				} ;
		
			} ;
			break ;

		case BOOLEAN:

			switch (ai_Op)
			{
			case EQUAL:
				ao_poResult->InitBoolean( M4_BOOL((GetBoolean() && ai_poOpeB->GetBoolean()) || 
					(!GetBoolean() && !ai_poOpeB->GetBoolean()) ))  ;
				break;
			case NOT_EQUAL:
				ao_poResult->InitBoolean( M4_BOOL((GetBoolean() && !ai_poOpeB->GetBoolean()) || 
					(!GetBoolean() && ai_poOpeB->GetBoolean()) ))  ;
				break;

			case VAL_AND:
				if (NULLValidOpe) {
					if (NULLValidOpe->GetBoolean()) {
						SwSetNULL=M4_TRUE ;	//TRUE & NULL = NULL
					} else {
						ao_poResult->InitBoolean( M4_FALSE ) ;	//FALSE & NULL = FALSE
					} ;
				} else {
					ao_poResult->InitBoolean( M4_BOOL(GetBoolean() && ai_poOpeB->GetBoolean()) ) ;
				} ;
				break;

			case VAL_OR:
				if (NULLValidOpe) {
					if (NULLValidOpe->GetBoolean()) {
						ao_poResult->InitBoolean( M4_TRUE );	//TRUE | NULL = TRUE
					} else {
						SwSetNULL=M4_TRUE  ;	//FALSE | NULL = NULL
					} ;
				} else {
					ao_poResult->InitBoolean( M4_BOOL(GetBoolean() || ai_poOpeB->GetBoolean()) ) ;
				} ;
				break;

			case VAL_NOT:
				ao_poResult->InitBoolean( M4_BOOL(!GetBoolean()) ) ;
				break;


			default:
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_90 ] ) 
					<< GetOpeString(ai_Op) << GetTypeString() << SEND_MSG ;
				m_poInsp->m_poTrace->ImmediateError() ;
				break;
			} ;

			break ;
	
		case CURRENCY:
		case VARIANT:

			switch (ai_Op)
			{
			
			default:
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_90 ] ) 
					<< GetOpeString(ai_Op) << GetTypeString() << SEND_MSG ;
				m_poInsp->m_poTrace->ImmediateError() ;
				break;
			} ;
			break ;
			
		case NO_INIT:

			switch (ai_Op)
			{
			default:
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_90 ] ) 
					<< GetOpeString(ai_Op) << GetTypeString() << SEND_MSG ;
				m_poInsp->m_poTrace->ImmediateError() ;
				break;
			} ;
			break ;

		default:

			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CHNVALUE_MSG_130 ] ) << SEND_MSG ;
			m_poInsp->m_poTrace->ImmediateError() ;
			break;
		} ;



	};

exit :

	//Pone valor a NULL si hay error o si no lo hay pero el resultado es NULL

	if ( !SwOk || SwSetNULL ) {

		if ( OpeInfo->m_NULLType!=NO_INIT ) {
			ao_poResult->SetNULL( OpeInfo->m_NULLType ) ;
		} else {
			if ( GetType()==NO_INIT && OpeArgs==2 ) {
				ao_poResult->SetNULL( ai_poOpeB->GetType() ) ;
			} else {
				ao_poResult->SetNULL( GetType() ) ;
			} ;
		} ;
	} ;

	return SwOk ;
}


//=========================================================================StMIT_NULLDisplayDef
// Almacenan cadenas de impresion para nulos.

void StMIT_NULLDisplayDef::End() 
{
	m4int16_t B ;

	for (B=0;B<ClMIT_ChnValue::TYPE_COUNT;++B) m_oStr[B].End() ;
}

void StMIT_NULLDisplayDef::Init(StMIT_NULLDisplayDef *ai_poSource) 
{
	m4int16_t B ;

	for (B=0;B<ClMIT_ChnValue::TYPE_COUNT;++B) m_oStr[B].StrCpy(ai_poSource->m_oStr[B]) ;
}

	

