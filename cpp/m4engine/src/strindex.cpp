//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             StrIndex.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/31/97
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

#include "eng.h"
#include "engdf.inl"

#include "clargs.h"

#include "strindex.hmg"
#include "strindex.h"

//============================================================================ClENG_StrIndex

ClENG_StrIndex::ClENG_StrIndex()
{
	m_poInsp=GET_INSP();
}

void ClENG_StrIndex::End() 
{
	m_oList.clear() ;
}

void ClENG_StrIndex::Init() 
{
	ClENG_StrIndex::End() ;
}

m4bool_t ClENG_StrIndex::Init( const m4char_t * ai_pcCommand, ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect,
						  ClMIT_Args *ai_poArg, m4char_t ai_cSeparator, const m4char_t * ArgSep) 
{
	ClMIT_Args Arg ;
	m4int16_t B ;
	m4bool_t SwOk=M4_TRUE ;
	ClMIT_Str Aux ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		if (ClMIT_Str::StrLenTrim(ai_pcCommand)>0) { 
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_StrIndex::Init() Inicializating from %0:s", ClMIT_Msg::ENTRY_FORMAT) 
				<< ai_pcCommand << SEND_MSG ;
		} ;
	};
	#endif

	ClENG_StrIndex::End() ;

	if (!ai_poArg) ai_poArg=&Arg ;

	ai_poArg->Init( ai_pcCommand, ai_cSeparator, ArgSep ) ;
	for (B=0;B<ai_poArg->GetArgCount();++B) {
		ai_poArg->GetString(B,0,&Aux);
		SwOk = M4_BOOL(SwOk && New( Aux.InChar(0) ));
	} ;
exit:
	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; 

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		if (ClMIT_Str::StrLenTrim(ai_pcCommand)>0) { 
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_StrIndex::Init()", ClMIT_Msg::EXIT_FORMAT) 
				<< SEND_MSG ;
		} ;
	};
	#endif
	
	return SwOk ;
}

m4bool_t ClENG_StrIndex::New( const m4char_t *ai_pcKey, m4uint32_t ai_uiData ) 
{
	ClMIT_Str Key ;
	KeyList_t::iterator FoundIt ;
	m4bool_t SwOk=M4_TRUE ;

	Key.StrCpy(ai_pcKey) ;
	FoundIt = m_oList.find(Key) ;		
	if (FoundIt != m_oList.end() ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ STRINDEX_MSG_10 ] ) << SEND_MSG ;
		//Error local
		SwOk=M4_FALSE ;
		goto exit;
	} else {
		m_oList.insert( ClENG_StrIndex::KeyList_t::value_type(Key, ai_uiData) );
	} ;
exit:
	return SwOk ;
}

m4int16_t ClENG_StrIndex::GetIndex( const m4char_t *ai_pcKey, m4uint32_t *ai_puiData,
	ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	ClMIT_Str Key ;
	KeyList_t::iterator FoundIt ;
	m4int16_t Index, Found ;

	Key.StrCpy(ai_pcKey) ;
	Index=0 ; Found=INDEX_EOF ;
	FoundIt=m_oList.begin() ;
	while ( FoundIt!=m_oList.end() ) {
		if ( (*FoundIt).first==Key ) {
			Found=Index ;
			if (ai_puiData) *ai_puiData=(*FoundIt).second ;
			break ;
		} ;
		++FoundIt ;
		++Index ;
	} ;
	
	if ( Found==INDEX_EOF ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ STRINDEX_MSG_20 ] ) 
			<< ai_pcKey << SEND_MSG ;
	} ; 

exit:
	m_poInsp->m_poTrace->TestEffect( M4_BOOL(Found!=INDEX_EOF), ai_ErrorEffect ) ; 

	return Found ;
}


m4char_t * ClENG_StrIndex::GetKey( m4int16_t ai_Index, 
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4char_t *Return=NULL ;

	KeyList_t::iterator It=m_oList.begin() ;

	while (ai_Index) { ++It ; --ai_Index; } ;
	Return =  ( It!=m_oList.end()) ? (*It).first.InChar(0) : NULL ;

exit:
	m_poInsp->m_poTrace->TestEffect( M4_BOOL(Return!=NULL), ai_ErrorEffect ) ; 

	return Return ;
}


void ClENG_StrIndex::GetInitString(ClMIT_Str *ao_poStr,const m4char_t * ArgSep) 
{
	KeyList_t::iterator It ;
	m4char_t StrSep[2] ;
	size_t L ;

	StrSep[0]=ArgSep[0] ; StrSep[1]=M4_END_STR;
	
	for (ao_poStr->StrCpy(""), It=m_oList.begin(); It!=m_oList.end(); ++It ) {
		ao_poStr->StrCat((*It).first.InChar(0)) ;
		ao_poStr->StrCat(StrSep) ;
	} ;
	L=ao_poStr->StrLen() ;
	if (L>1) *ao_poStr->InChar(L-1)=M4_END_STR ;	//Borra ultimo separador superfluo
}
