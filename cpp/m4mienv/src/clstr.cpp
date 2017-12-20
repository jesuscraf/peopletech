//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             clstr.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:27/01/98
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
#include "clstr.hmg"

#include "clstr.h"


ClMIT_Str::ClMIT_Str() 
{
	m_poInsp=GET_INSP();
	m_pcDynStr=NULL;
	m_szDynSize=0;
	m_Interblock=DEFAULT_INTERBLOCK;
	m_LastAccommodate=0 ;
	m_bSwStatic=M4_TRUE;
	m_pcStcStr[0]=M4_END_STR ;
}

ClMIT_Str::ClMIT_Str(const ClMIT_Str& ai_oSource) 
{
	m_poInsp=GET_INSP();
	m_pcDynStr=NULL;
	m_szDynSize=0;
	m_Interblock=DEFAULT_INTERBLOCK;
	m_LastAccommodate=0 ;
	m_bSwStatic=M4_TRUE;
	m_pcStcStr[0]=M4_END_STR ;

	Init(&ai_oSource); 
}

ClMIT_Str::~ClMIT_Str() 
{
	ClMIT_Str::Accommodate(0); 
}

ClMIT_Str::ClMIT_Str(const m4char_t *ai_pcStr) 
{ 
	m_poInsp=GET_INSP();
	m_pcDynStr=NULL;
	m_szDynSize=0;
	m_Interblock=DEFAULT_INTERBLOCK;
	m_LastAccommodate=0 ;
	m_bSwStatic=M4_TRUE;
	m_pcStcStr[0]=M4_END_STR ;
	StrCpy(ai_pcStr); 
}


void ClMIT_Str::Init(const ClMIT_Str * ai_poSource)
{
	StrNCpy(ai_poSource->InChar(0), ai_poSource->GetMaxSize()) ;
}

void ClMIT_Str::Accommodate(size_t ai_szSize, m4bool_t ai_bSwCopy )
{

	m_LastAccommodate=ai_szSize ;

	if (ai_szSize!=0) {
		if ( ai_szSize <= SIZE_STRING ) {
			m_bSwStatic = (M4_BOOL(m_pcDynStr==NULL));
		} else {
			if ( ai_szSize > m_szDynSize ) {
				m4char_t * Str ;
				size_t Size ;

				Size = (ai_szSize/m_Interblock)*m_Interblock + 
					((ai_szSize%m_Interblock) ? m_Interblock : 0 ) ;
				M4_NEW_ARRAY( m_poInsp->m_poMemProf, MIT_MGI_CL_STR, 
					Str, m4char_t, Size ) ;
				if (ai_bSwCopy) {
					strcpy(Str,InChar(0)) ;
				} ;
				M4_DELETE_ARRAY(m_poInsp->m_poMemProf, MIT_MGI_CL_STR, 
					m_pcDynStr, m4char_t, m_szDynSize ) ;
				m_pcDynStr=Str ;
				m_szDynSize=Size ;
			} ;
			m_bSwStatic=M4_FALSE;	//Ojo, no ponerlo antes, o la copia puede ir mal.
		} ;
	} else {

		M4_DELETE_ARRAY( m_poInsp->m_poMemProf, MIT_MGI_CL_STR, 
			m_pcDynStr, m4char_t, m_szDynSize ) ;
		m_szDynSize=0;
		m_bSwStatic=M4_TRUE;
		m_pcStcStr[0]=M4_END_STR ;
	} ;
}

void ClMIT_Str::StrCpy(const m4char_t *ai_pcStr, size_t ai_AddSize) 
{
	StrNCpy( ai_pcStr, strlen(ai_pcStr)+1+ai_AddSize ) ;
}

void ClMIT_Str::StrNCpy(const m4char_t *ai_pcStr, size_t ai_szSize, size_t ai_AddSize) 
{
	Accommodate(ai_szSize+ai_AddSize, M4_FALSE) ;
	strncpy( (char *)(*this), ai_pcStr, ai_szSize ) ;	//No se testea, cabe seguro
}

void ClMIT_Str::ToASCIZ(const m4char_t *ai_pcStr, size_t ai_szSize, size_t ai_AddBlanks) 
{
	StrNCpy(ai_pcStr,ai_szSize,ai_AddBlanks+1) ;	//+1 para END_STR
	*InChar(ai_szSize) = M4_END_STR ;
	StrInsertChar(STR_CAT,ai_AddBlanks,' ') ;
}

void ClMIT_Str::FullTrim() 
{
	size_t Len ;

	Trim() ;
	Len=strspn(InChar(0)," ") ;
	if ( Len>0 ) {
		StrDeleteSegment(0,Len) ;
	} ;
}


void ClMIT_Str::StrInsertChar( 
	size_t ai_szFromChar, size_t ai_szNumChars, m4char_t ai_cFillChar )
{
	Accommodate(strlen(InChar(0))+ai_szNumChars+1,M4_TRUE) ;
	StrInsertChar( InChar(0), ai_szFromChar, ai_szNumChars, ai_cFillChar, 
		GetMaxSize() ) ;
}

void ClMIT_Str::StrDuplicateChar( 
	size_t ai_szFromChar, size_t ai_szNumChars, m4char_t ai_cFillChar ) 
{
	size_t Len=strlen(InChar(0)) ;

	if (ai_szFromChar==STR_CAT) {
		Accommodate(Len+ai_szNumChars+1,M4_TRUE) ;
	} else {
		Accommodate(M4_MAX(Len,ai_szFromChar+ai_szNumChars)+1,M4_TRUE) ;
	} ;
	StrDuplicateChar( InChar(0), ai_szFromChar, ai_szNumChars, ai_cFillChar, 
		GetMaxSize() ) ;
}

void ClMIT_Str::StrDeleteSegment( size_t ai_szFromChar, size_t ai_szNumChars )
{
	StrDeleteSegment(InChar(0), ai_szFromChar, ai_szNumChars, GetMaxSize() );
}


size_t ClMIT_Str::GetString( MODE_GET_STRING ai_Mode, const m4char_t *ai_pcStr, m4char_t *ai_pcDelimit ) 
{
	size_t Long ;
	
	if ( ai_Mode == ClMIT_Str::TO_LIMIT ) {
		Long = strcspn(ai_pcStr, ai_pcDelimit) ;
	} else {
		Long = strspn(ai_pcStr, ai_pcDelimit) ;
	} ;
	StrNCpy( ai_pcStr, Long, 1 ) ;
	*(InChar(Long))=M4_END_STR ;

	//return &ai_pcStr[Long] ;
	return Long ;
}

size_t ClMIT_Str::StrLen() const
{
	return strlen( InChar(0) ) ;
}

void ClMIT_Str::Trim(size_t ai_szLen) 
{
	*(InChar(ai_szLen))=M4_END_STR ;
	if ( ai_szLen > GetMaxSize()-1  ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLSTR_OVERSTRING ] ) 
			<< InChar(0) << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
}

size_t ClMIT_Str::StrLenTrim() const
{
	return StrLenTrim( InChar(0) ) ;
}

void ClMIT_Str::OverStringValid() 
{
	OverStringValid( (char *)(*this), GetMaxSize() ) ;	
}

void  ClMIT_Str::StrCat(const m4char_t *ai_pcStr) 
{
	Accommodate(StrLen()+strlen(ai_pcStr)+1,M4_TRUE) ;
	strcat(InChar(0),ai_pcStr) ;

}

void  ClMIT_Str::StrCatChar(const m4char_t ai_pcChar) 
{
	size_t l=StrLen() ;
	Accommodate(l+1+1,M4_TRUE) ;
	*InChar(l)=ai_pcChar ;
	*InChar(l+1)=M4_END_STR ;
}


void ClMIT_Str::OverStringValid(size_t ai_szSize) 
{
	OverStringValid( (char *)(*this), GetMaxSize(), ai_szSize ) ;
}


void ClMIT_Str::OverStringValid( const m4char_t * ai_pcStr, size_t ai_szMaxSize )
{
	if ( strlen(ai_pcStr) >= ai_szMaxSize ) {
		StMIT_Inspector *Insp=GET_INSP();
		*Insp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLSTR_OVERSTRING ] ) 
			<< ai_pcStr << SEND_MSG ;
		Insp->m_poTrace->ImmediateError() ;
	}

}

void ClMIT_Str::OverStringValid( const m4char_t * ai_pcStr, size_t ai_szMaxSize, 
		size_t ai_szBytesWritten )
{
	if ( ai_szBytesWritten > ai_szMaxSize ) {
		StMIT_Inspector *Insp=GET_INSP();
		*Insp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLSTR_OVERSTRING ] ) 
			<< ClMIT_StrNLink(ai_pcStr,ai_szBytesWritten) << SEND_MSG ;
		Insp->m_poTrace->ImmediateError() ;
	}

}



void ClMIT_Str::SafeStrCpy( m4char_t *ao_pcDest, size_t ai_szMaxSizeDest, const m4char_t * ai_pccSource)
{
	OverStringValid(ai_pccSource, ai_szMaxSizeDest);
	strcpy(ao_pcDest, ai_pccSource);
}

void ClMIT_Str::SafeStrNCpy( m4char_t *ao_pcDest, size_t ai_szMaxSizeDest, 
		const m4char_t * ai_pccSource, size_t ai_szBytesToCopy)
{
	OverStringValid(ai_pccSource, ai_szMaxSizeDest, ai_szBytesToCopy);
	strncpy(ao_pcDest, ai_pccSource, ai_szBytesToCopy);
}

size_t ClMIT_Str::StrLenTrim( const m4char_t * ai_pcStr ) 
{
	size_t L=strlen( ai_pcStr ) ; 
	while (L>0) { 
		if (ai_pcStr[L-1]==' ') --L ;
		else break;
	} ;
	return L ;
}


m4int16_t ClMIT_Str::StrCmpTrim( const m4char_t * ai_pcStr1, const m4char_t * ai_pcStr2 ) 
{
	size_t L1=StrLenTrim(ai_pcStr1), L2=StrLenTrim(ai_pcStr2) ;

	return strncmp( ai_pcStr1, ai_pcStr2, M4_MAX(L1,L2) ) ;
}

m4int16_t ClMIT_Str::StrICmpTrim( const m4char_t * ai_pcStr1, const m4char_t * ai_pcStr2 ) 
{
	size_t L1=StrLenTrim(ai_pcStr1), L2=StrLenTrim(ai_pcStr2) ;

	return strnicmp( ai_pcStr1, ai_pcStr2, M4_MAX(L1,L2) ) ;
}

void ClMIT_Str::StrInsertChar( 
		m4char_t *ao_pcStr, size_t ai_szFromChar, size_t ai_szNumChars, 
		m4char_t ai_cFillChar, size_t ai_szMaxSize ) 
{
	size_t Len=strlen(ao_pcStr) ;
	
	if (ai_szFromChar==STR_CAT) {
		memset( &ao_pcStr[Len], ai_cFillChar, ai_szNumChars ) ;
		ao_pcStr[Len+ai_szNumChars]=M4_END_STR ;
	} else {
		memmove( &ao_pcStr[ai_szFromChar+ai_szNumChars], &ao_pcStr[ai_szFromChar], 
			Len-ai_szFromChar+1 ) ;	//+1 para pillar END_STR
		memset( &ao_pcStr[ai_szFromChar], ai_cFillChar, ai_szNumChars ) ;
	} ;
	OverStringValid(ao_pcStr,ai_szMaxSize) ;
}


void ClMIT_Str::StrDuplicateChar( 
		m4char_t *ao_pcStr, size_t ai_szFromChar, size_t ai_szNumChars, 
		m4char_t ai_cFillChar, size_t ai_szMaxSize ) 
{
	size_t Len=strlen(ao_pcStr) ;
	size_t EOSIndex ;

	if (ai_szFromChar==STR_CAT) {
		memset( &ao_pcStr[Len], ai_cFillChar, ai_szNumChars ) ;
		EOSIndex=Len+ai_szNumChars;
	} else {
		memset( &ao_pcStr[ai_szFromChar], ai_cFillChar, ai_szNumChars ) ;
		EOSIndex=M4_MAX(Len, ai_szFromChar+ai_szNumChars);
	} ;
	ao_pcStr[EOSIndex]=M4_END_STR ;
	OverStringValid(ao_pcStr,ai_szMaxSize) ;
}


//Version estatica
void ClMIT_Str::StrInsertStr( 
	m4char_t *ao_pcStr, size_t ai_szFromChar, const m4char_t *ai_pcStr2, 
	size_t ai_szLen2, size_t ai_szMaxSize ) 
{
	StrInsertChar( ao_pcStr, ai_szFromChar, ai_szLen2, ' ', ai_szMaxSize ) ;
	strncpy( &ao_pcStr[ai_szFromChar], ai_pcStr2, ai_szLen2) ;
}

//Version estatica
void ClMIT_Str::StrInsertStr( 
	m4char_t *ao_pcStr, size_t ai_szFromChar, const m4char_t *ai_pcStr2, size_t ai_szMaxSize ) 
{
	StrInsertStr(ao_pcStr,ai_szFromChar,ai_pcStr2,strlen(ai_pcStr2),ai_szMaxSize) ; 
}

void ClMIT_Str::StrInsertStr( size_t ai_szFromChar, const m4char_t *ai_pcStr2, size_t ai_szLen2 ) 
{
	Accommodate(strlen(InChar(0))+ai_szLen2+1,M4_TRUE) ;
	StrInsertStr( InChar(0), ai_szFromChar, ai_pcStr2, ai_szLen2, GetMaxSize() ) ;
}

void ClMIT_Str::StrInsertStr( size_t ai_szFromChar, const m4char_t *ai_pcStr2)  
{
	Accommodate(strlen(InChar(0))+strlen(ai_pcStr2)+1,M4_TRUE) ;
	StrInsertStr( InChar(0), ai_szFromChar, ai_pcStr2, GetMaxSize() ) ;
}

void ClMIT_Str::StrDeleteSegment( 
		m4char_t *ao_pcStr, size_t ai_szFromChar, size_t ai_szNumChars, 
		size_t ai_szMaxSize ) 
{
	size_t Len=strlen(ao_pcStr) ;
	
	memmove( &ao_pcStr[ai_szFromChar], &ao_pcStr[ai_szFromChar+ai_szNumChars], 
	Len-ai_szFromChar-ai_szNumChars+1 ) ;	//+1 para pillar END_STR
	//memset( &ao_pcStr[ai_szFromChar], ai_cFillChar, ai_szNumChars ) ;
	
	OverStringValid(ao_pcStr,ai_szMaxSize) ;
}


void ClMIT_Str::StrAlign( 
	const m4char_t *ao_pcInputStr, size_t ai_szSizeAlign, 
	STR_ALIGN ai_AlignMode, m4char_t * ao_pcStr, size_t ai_szMaxSize ) 
{
	size_t Len ;

	Len=strlen(ao_pcInputStr);
	if (Len>=ai_szSizeAlign) {
		SafeStrCpy(ao_pcStr, ai_szMaxSize, ao_pcInputStr) ;
	} else {
		//Escribo un blanco mas para que se testee que va a caber el END_STR
		StrDuplicateChar( ao_pcStr, 0, ai_szSizeAlign+1, ' ',ai_szMaxSize ) ;
		ao_pcStr[ai_szSizeAlign]=M4_END_STR;

		switch(ai_AlignMode) {
		case STR_LEFT:
			strncpy(ao_pcStr,ao_pcInputStr,Len) ;
			break ;
		case STR_RIGHT:
			strncpy(&ao_pcStr[ai_szSizeAlign-Len],ao_pcInputStr,Len) ;
			break ;
		case STR_CENTER:
			//Nota:la division entera siempre toma el ajuste menor si no es exacto.
			//Se quiere esto, o se podria sobrescribir un caracter.
			strncpy(&ao_pcStr[(ai_szSizeAlign-Len)/2],ao_pcInputStr,Len) ;
			break ;
		} ;
	} ;
}

void ClMIT_Str::StrAlign( const m4char_t *ao_pcInputStr, size_t ai_szSizeAlign, 
		STR_ALIGN ai_AlignMode) 
{
	size_t Len;

	Len=strlen(ao_pcInputStr) ;
	Accommodate(M4_MAX(Len,ai_szSizeAlign)+1) ;
	StrAlign( ao_pcInputStr, ai_szSizeAlign, ai_AlignMode,InChar(0),GetMaxSize()) ;
}

ClMIT_Str & ClMIT_Str::operator = (const ClMIT_Str &ai_oOpe)
{
	Init(&ai_oOpe) ;
	return *this ;
}

m4bool_t ClMIT_Str::operator < (const ClMIT_Str &ai_OpeB) const
{
	return M4_BOOL( StrICmpTrim(InChar(0), ai_OpeB.InChar(0) ) < 0 );
}

m4bool_t ClMIT_Str::operator == (const ClMIT_Str &ai_OpeB) const
{
	return M4_BOOL( StrICmpTrim(InChar(0), ai_OpeB.InChar(0) ) == 0 );
}

m4char_t *ClMIT_Str::StrStr(m4char_t *ai_pcStr, m4char_t *ai_pcSubStr ) 
{
	return strstr(ai_pcStr,ai_pcSubStr) ;
}

m4char_t *ClMIT_Str::StrIStr(m4char_t *ai_pcStr, m4char_t *ai_pcSubStr ) 
{
	m4char_t *R=NULL;
	size_t LSub=strlen(ai_pcSubStr) ;
	
	if (LSub==0) return ai_pcStr ;

	for ( ; *ai_pcStr!=M4_END_STR; ++ai_pcStr) {
		if ( strnicmp(ai_pcStr,ai_pcSubStr,LSub)==0 ) {
			R=ai_pcStr ;
			break ;
		} ;
	} ;
	return R ;
}


void ClMIT_Str::ToUpper( m4char_t * ai_pcStr ) 
{
	for( ; *ai_pcStr; ++ai_pcStr) {
		*ai_pcStr=toupper(*ai_pcStr) ;
	} ;
}

void ClMIT_Str::ToLower( m4char_t * ai_pcStr ) 
{
	for( ; *ai_pcStr; ++ai_pcStr) {
		*ai_pcStr=tolower(*ai_pcStr) ;
	} ;
}

