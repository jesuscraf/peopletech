//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             ClMIT_Args.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:20/10/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#include "mit.h"

#include "m4string.hpp"

#include "mitdefs.cpp"

#include "clargs.hmg"
#include "clargs.h"

const m4char_t ClMIT_Args::m_pcClosingChar[3]="##" ;	

ClMIT_Args::ClMIT_Args()
{
	m_poInsp=GET_INSP() ;
	m_poArg=NULL ;
}

void ClMIT_Args::End() 
{
	m4int16_t B ;

	if (m_poArg) {
		for (B=0; B<m_iArgCount; ++B ) {
			m_poArg[B].Accommodate(0) ;
		} ;
		M4_DELETE_ARRAY( m_poInsp->m_poMemProf, MIT_MGI_OTHERS, 
			m_poArg, ClMIT_Str, m_iArgCount)  ;
	} ;
}

void ClMIT_Args::Init( const m4char_t ** ai_pcArgV, m4int16_t ai_iArgC, 
	m4char_t ai_cSeparator, const m4char_t * ArgSep ) 
{
	m4int16_t B ;

	ClMIT_Args::End() ;

	m_oArgSep.StrCpy(ArgSep) ;
	m_iArgCount =ai_iArgC ;
	m_cSeparator=ai_cSeparator ;
	if (m_cSeparator==M4_END_STR) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLARGS_MSG_80 ] ) 
			<< "Inicializating from array" << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;

	if (m_iArgCount>0) {
		M4_NEW_ARRAY( m_poInsp->m_poMemProf, MIT_MGI_OTHERS, 
			m_poArg, ClMIT_Str, m_iArgCount)  ;
		for (B=0; B<m_iArgCount; ++B ) {
			m_poArg[B].StrCpy(ai_pcArgV[B]) ;
		} ;
	} else m_poArg=NULL ;
}

void ClMIT_Args::Init( const m4char_t *ai_pcCommand, size_t ai_szNChar, m4char_t ai_cSeparator, 
	const m4char_t * ArgSep, const m4char_t * ClosingChar ) 
{
	ClMIT_Str Aux ;

	Aux.StrNCpy(ai_pcCommand,ai_szNChar,1) ;
	*(Aux.InChar(ai_szNChar))=M4_END_STR ;
	Init( Aux, ai_cSeparator, ArgSep, ClosingChar ) ;
}

void ClMIT_Args::Init( const m4char_t *ai_pcCommand, m4char_t ai_cSeparator, 
	const m4char_t * ai_ArgSep, const m4char_t * ai_ClosingChar ) 
{
	ClMIT_Args::End() ;

	m_oArgSep.StrCpy(ai_ArgSep) ;
	m_cSeparator=ai_cSeparator ;
	if (m_cSeparator==M4_END_STR) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLARGS_MSG_80 ] ) 
			<< ai_pcCommand << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;


	m_iArgCount = ParseCommand( ai_pcCommand, m_cSeparator, 
		ai_ArgSep, ai_ClosingChar, NULL ) ;

	//Y ahora los carga

	if (m_iArgCount>0) {
	
		M4_NEW_ARRAY( m_poInsp->m_poMemProf, MIT_MGI_OTHERS, 
			m_poArg, ClMIT_Str, m_iArgCount)  ;
		ParseCommand( ai_pcCommand, m_cSeparator, 
			ai_ArgSep, ai_ClosingChar, m_poArg ) ;
	} ;
}


m4int16_t ClMIT_Args::ParseCommand( const m4char_t *ai_pcCommand, m4char_t ai_cSeparator, 
	const m4char_t * ai_ArgSep, const m4char_t * ai_ClosingChar,ClMIT_Str *ai_poArg )
{
	m4int16_t count, B ;
	size_t LenBlankStr, LenNoQuote, LenOpenQuote, LenArg, LenCloseQuote, LenAux, LenAux2 ;
	m4char_t OpenQuote[2], CloseQuote[2];
	const m4char_t *pos, *begArg ;

	OpenQuote[0]  = ai_ClosingChar[0] ; OpenQuote[1]=M4_END_STR ;
	CloseQuote[0] = ai_ClosingChar[1] ; CloseQuote[1]=M4_END_STR ;

	for ( pos=ai_pcCommand, count=0, B=0 ; *pos; ++B ) {
		
		//Calcula pos para que apunte a ... (ver if)

		LenBlankStr = strspn(pos, ai_ArgSep) ;
		LenNoQuote  = strcspn(pos,OpenQuote ) ;
		pos += M4_MIN( LenBlankStr, LenNoQuote ) ;

		if ( *pos==M4_END_STR ) {
		
			// *** END_STR: salir ***
			break ;

		} else if (*pos == OpenQuote[0] ) {

			// *** Primer OpenQuoteChar, argumento delimitado por quote ***

			++count ;

			//LenOpenQuote contien la len del OpenQuote, Hay que buscar el proximo
			//	de cierre con la misma longitud.

			LenOpenQuote = strspn(pos, OpenQuote) ;
			pos += LenOpenQuote;	//Saltamos el OpenQuote

			//begArg es el comienzo del arg a procesar
			//LenArg, len arg incluido quotechar sobrantes
			for ( begArg=pos, LenArg=0 ; ;  ) {
				//LenAux es la leng del arg sin close quote
				LenAux = strcspn(pos, CloseQuote) ;	
				//Saltamos el arg, contabilizamos para LenArg
				pos += LenAux ;	 LenArg += LenAux ;
				//LenCloseQuote es el numero de closeQuote
				LenCloseQuote = strspn(pos, CloseQuote) ; 
				//Saltamos close quote, este es el comienzo de la sig pasada
				pos += LenCloseQuote  ;	
				if (LenCloseQuote >= LenOpenQuote) {
					//Fin arg, incluimos quote sobrantes
					LenArg += LenCloseQuote-LenOpenQuote ;
					break ;
				} else if ( *pos==M4_END_STR ) {
					//No hay char cierre, no incluimos estos quote char
					*GET_INSP()->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLARGS_MSG_70 ] ) 
						<< CloseQuote[0] << pos << SEND_MSG ;
					//Error local, no se cerro char apertura
					break ;
				} else {
					//Si la len es menor que la de open, forma parte del Arg
					LenArg += LenCloseQuote ;
				} ;
			} ;
			
			//Inserto nuevo param
			if ( ai_poArg ) {
				ai_poArg[B].StrNCpy(begArg,LenArg,1) ;
				*(ai_poArg[B].InChar(LenArg))=M4_END_STR ;
			} ;
			
		} else {

			// *** Primer caracter de argumento sin quote ***

			++count ;
	
			begArg = pos ;
			LenAux   = strcspn(pos,ai_ArgSep) ;
			LenAux2  = strcspn(pos,OpenQuote ) ;
			LenArg = M4_MIN( LenAux, LenAux2 ) ;
			pos += LenArg ;

			//Inserto nuevo param
			if ( ai_poArg ) {
				ai_poArg[B].StrNCpy(begArg,LenArg,1) ;
				*(ai_poArg[B].InChar(LenArg))=M4_END_STR ;
			} ;
		} ;

	} ;


	return count ;
}

const m4char_t * ClMIT_Args::GetArg(m4int16_t ai_iArg) 
{
	if (!M4_IN_RANGE(ai_iArg, 0, m_iArgCount-1)  ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLARGS_MSG_30 ] ) << ai_iArg << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
	return m_poArg[ai_iArg] ;
}

m4bool_t ClMIT_Args::IsValidSubArg(m4int16_t ai_iArg, m4int16_t ai_iSubArg, 
	const m4char_t *ai_pcNameArg) 
{
	m4bool_t SwOK ;
	size_t Len ;

	SwOK=M4_BOOL( M4_IN_RANGE(ai_iArg, 0, m_iArgCount-1) ) ;
	if (SwOK) {
		Len=GetSubArgCount(ai_iArg);
		SwOK= M4_BOOL( M4_IN_RANGE(ai_iSubArg, 0, Len-1) ) ;
	} ;
	if (!SwOK && ai_pcNameArg) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLARGS_MSG_50 ] ) 
			<< ai_iArg << ai_iSubArg << ai_pcNameArg << SEND_MSG ;
	} ;
	return SwOK ;
}

m4bool_t ClMIT_Args::GetStringFromStr( 
	const m4char_t *ai_pcArg, m4int16_t ai_iSubArg,
	ClMIT_Str * ao_poStr, m4bool_t ai_bSwOblig) 
{
	m4bool_t Return ;
	const m4char_t *ao_ppcBeginStr ;
	size_t ao_pszLen ;

	Return=GetStringFromStr(ai_pcArg, ai_iSubArg,&ao_ppcBeginStr, &ao_pszLen, ai_bSwOblig) ;
	ao_poStr->StrNCpy(ao_ppcBeginStr,ao_pszLen,1) ;
	*(ao_poStr->InChar(ao_pszLen))=M4_END_STR ;

	return Return ;
}

m4int16_t ClMIT_Args::GetSubArgCount(m4int16_t ai_iArg) 
{
	m4int16_t Count ;
	const m4char_t * Pos ;

	Pos = GetArg(ai_iArg) ;
	Count=0 ; 
	do {
		++Count ;
		Pos=strchr(Pos+1,m_cSeparator) ;
	} while (Pos) ;

	return Count ;
}

m4bool_t ClMIT_Args::GetStringFromStr( 
	const m4char_t *ai_pcArg, m4int16_t ai_iSubArg,
	const m4char_t **ao_ppcBeginStr, size_t * ao_pszLen, m4bool_t ai_bSwOblig) 
{
	m4int16_t Skiped ;
	m4pcchar_t  Pos;
	m4pcchar_t PosAnt ;
	m4bool_t Found=M4_TRUE ;

	if (ai_iSubArg<0) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLARGS_MSG_10 ] ) 
			<< ai_iSubArg << ai_pcArg << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
	
	Skiped=0 ; Pos=strchr(ai_pcArg,m_cSeparator) ; PosAnt=NULL ;
 	while ( Pos && Skiped < ai_iSubArg  ) {
		++Skiped ;
		PosAnt=Pos ;
		Pos=strchr(Pos+1,m_cSeparator) ;
	} ;

	if (ai_iSubArg==0) {				//Caso especial, SubArg=0
		*ao_ppcBeginStr=ai_pcArg ;
		*ao_pszLen = Pos ? Pos-ai_pcArg : strlen(ai_pcArg) ;		
	} else if ( Skiped==ai_iSubArg || Skiped-1==ai_iSubArg) {	//Encontrado
		*ao_ppcBeginStr= PosAnt+1 ;
		*ao_pszLen = Pos ? Pos-*ao_ppcBeginStr : strlen(*ao_ppcBeginStr) ;		
	} else {							//No existe SubArg
		Found=M4_FALSE ;
		if (ai_bSwOblig ) {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLARGS_MSG_10 ] ) 
				<< ai_iSubArg << ai_pcArg << SEND_MSG ;
			m_poInsp->m_poTrace->ImmediateError() ;
		} ;
	} ;
	return Found ;
}

m4int16_t ClMIT_Args::GetEnumFromStr( 
		const m4char_t * ai_pcArg, m4int16_t ai_iSubArg,
		const EnumTrans_t * ai_Trans, m4bool_t *ao_SwFound, m4bool_t ai_SwMsg  ) 
{
	const m4char_t *Begin ;
	size_t Len ;
	m4bool_t SwOK ;

	SwOK=GetStringFromStr(ai_pcArg, ai_iSubArg, &Begin, &Len, M4_FALSE ) ;
	if (SwOK) {
		SwOK=M4_FALSE ;
		while (ai_Trans->pcStr ) {
			if ( strlen(ai_Trans->pcStr) == Len && strnicmp( Begin, ai_Trans->pcStr, Len )==0 ){
				SwOK=M4_TRUE ;
				break ;
			} else ++ai_Trans;
		} ;
	} ;
	if (!SwOK && !ao_SwFound ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLARGS_MSG_20 ] ) 
			<< ai_pcArg << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
	if (ao_SwFound) *ao_SwFound=SwOK ;
	if (!SwOK && ai_SwMsg ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLARGS_MSG_60 ] ) 
			<< ai_pcArg << SEND_MSG ;
		//Warning, efecto local
	} ;

	return SwOK ? ai_Trans->iValue : 0 ;

}

m4int32_t ClMIT_Args::GetLongFromStr( const m4char_t *ai_pcArg, m4int16_t ai_iSubArg ) 
{
	const m4char_t *Begin ;
	size_t Len ;
	m4int32_t Return ;
	ClMIT_Str StrAux ;

	GetStringFromStr(ai_pcArg, ai_iSubArg, &Begin, &Len ) ;
	StrAux.ToASCIZ(Begin,Len) ;
	Return=atol(StrAux) ;

	return Return ;
}

m4double_t ClMIT_Args::GetDoubleFromStr( const m4char_t *ai_pcArg, m4int16_t ai_iSubArg ) 
{
	const m4char_t *Begin ;
	size_t Len ;
	m4double_t Return ;
	ClMIT_Str StrAux ;

	GetStringFromStr(ai_pcArg, ai_iSubArg, &Begin, &Len ) ;
	StrAux.ToASCIZ(Begin,Len) ;
	Return=atof(StrAux) ;

	return Return ;
}

m4int16_t ClMIT_Args::Find( m4int16_t ai_iSubArg, const m4char_t *ai_pcValue, size_t ai_szNChar, 
					   m4bool_t ai_bSwOblig) 
{
	const m4char_t *Begin ;
	size_t Len ;
	m4int16_t B, Found=-1 ;

	if (ai_szNChar==0) ai_szNChar=strlen(ai_pcValue) ;

	for (B=0; B<m_iArgCount; ++B) {
		if ( GetString(B, ai_iSubArg, &Begin, &Len, M4_FALSE ) ) {
			if ( strnicmp(Begin,ai_pcValue,ai_szNChar)==0 ) {
				if (Begin[ai_szNChar]==m_cSeparator || Begin[ai_szNChar]=='\x00') {
					Found=B ;
					break ;
				} ;
			} ;
		} ;
	} ;

	if ( Found<0 && ai_bSwOblig) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLARGS_MSG_40 ] ) 
			<< ai_pcValue << (m4uint32_t)ai_szNChar << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	}
	return Found ; 
}
