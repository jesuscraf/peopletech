//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             corevr.cpp
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

#include "core.h"

#include "corevr.hmg"
#include "corevr.h"

//============================================================================ClENG_VR

ClENG_VR::ClENG_VR() 
{
	m_poInsp=GET_INSP();
	m_poVRFather=NULL ;
	m_poOwnerSection=NULL ;
	m_poRecordNode=NULL;
} ;

void ClENG_VR::End() 
{
	StGroup::GroupKey_t::iterator It ;

	m_poRecordNode=NULL;
	m_oGuideItem.End() ;

	for (It=m_oGParam.m_oGroup.begin(); It!=m_oGParam.m_oGroup.end(); ++It) {
		M4_DELETE_VIRTUAL_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CORE, *It) ;
	} ;
	m_oGParam.m_oGroup.clear() ;

	for (It=m_oGParam.m_oAuxCompare.begin(); It!=m_oGParam.m_oAuxCompare.end(); ++It) {
		M4_DELETE_VIRTUAL_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CORE, *It) ;
	} ;
	m_oGParam.m_oAuxCompare.clear() ;

	m_oOneOne.clear() ;
	m_oPrepareRecordset.End() ;
	m_oCancelRecordset.End() ;
	m_oCancelVR.End() ;
}


m4bool_t ClENG_VR::Init( 
	ClENG_Section * ai_poOwnerSection,
	ClENG_VR *ai_poVRFather,
	ClMIT_ItemExternProp *ai_poItemMod,

	GO_THROUGH_MODE ai_GoT, RANGE_MODE ai_RangeMode,
	ClMIT_ChnNode * ai_poRecordNode, ClMIT_ChnItem * ai_poGuideItem,

	StGroup::GroupKey_t * ai_poGroupKey,

	OneOneList_t *ai_poOneOne,

	const m4char_t * ai_pcPrepareRecordset,
	const m4char_t * ai_pcCancelRecordset,
	const m4char_t * ai_pcCancelVR,

	NULL_IF_EMPTY 	ai_NullIfEmpty,
	m4bool_t    ai_bNullOnSons,

	ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect)  
{
	m4bool_t Return=M4_TRUE ;
	StGroup::GroupKey_t::iterator It ;
	OneOneList_t::iterator OneIt ;
	ClMIT_ChnItem *ItemCopy ;
	
	ClENG_VR::End() ;

	//Copiar la lista antes de cualquier testeo de error, para que se hereden los punteros
	//	y End borre los items en todos los casos.

	for (It=ai_poGroupKey->begin(); It!=ai_poGroupKey->end(); ++It) {
		m_oGParam.m_oGroup.push_back(*It) ;
		M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_CORE, ItemCopy, ClMIT_ChnItem ) ;
		ItemCopy->Init(*It) ;
		m_oGParam.m_oAuxCompare.push_back(ItemCopy) ;
	} ;

	for (OneIt=ai_poOneOne->begin(); OneIt!=ai_poOneOne->end(); ++OneIt) {
		m_oOneOne.insert(ClENG_VR::OneOneList_t::value_type(*OneIt)) ;
	} ;

	m_oPrepareRecordset.Init( ai_pcPrepareRecordset, "PrepareRecordset" ) ;
	m_oCancelRecordset.Init( ai_pcCancelRecordset, "CancelRecordset" ) ;
	m_oCancelVR.Init( ai_pcCancelVR, "CancelVR" ) ;
	
	m_poVRFather=ai_poVRFather;
	m_poOwnerSection=ai_poOwnerSection ;
	m_poItemMod = ai_poItemMod ;

	m_GoT=ai_GoT ;
	m_Range=ai_RangeMode ;

	m_ActualRecord=ClMIT_ChnNode::RECORD_EOF ; 
	m_ActualSlice=ClMIT_ChnItem::SLICE_EOF ;
	m_NullIfEmpty=ai_NullIfEmpty;
	m_bNullOnSons=ai_bNullOnSons;

	if ( ! (m_GoT==RECORD_GOT && m_Range==ALL_VR) && 
		 ! (m_GoT==RECORD_GOT && m_Range==SUBGROUP_VR) &&
		 ! (m_GoT==SLICE_GOT && m_Range==ALL_VR)  ) 
	{
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CORE_VR_MSG_10 ] ) << SEND_MSG ;
		Return=M4_FALSE ;
		goto exit ;
	} ;


	if (m_GoT==SLICE_GOT || m_Range==SUBGROUP_VR ) {
		//Copiar RecordNode de padre. PADRE requerido. Si SUBGROUP, debe ser GoT=Record
		if (!m_poVRFather) {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CORE_VR_MSG_20 ] ) << SEND_MSG ;
			Return=M4_FALSE ;
			goto exit ;
		} ;
		if ( m_Range==SUBGROUP_VR && m_poVRFather->m_GoT != RECORD_GOT ) {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CORE_VR_MSG_50 ] ) << SEND_MSG ;
			Return=M4_FALSE ;
			goto exit ;
		} ;
		m_poRecordNode=m_poVRFather->m_poRecordNode ;

	} else {
		//Linkar RecordNode
		m_poRecordNode=ai_poRecordNode ;
	} ;

	if (m_GoT==SLICE_GOT) {
		if (!ai_poGuideItem) {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CORE_VR_MSG_30 ] ) << SEND_MSG ;
			Return=M4_FALSE ;
			goto exit ;
		} ;
		m_oGuideItem.Init(ai_poGuideItem) ;
	} ;

	if ( IsGrouped() && m_GoT!=RECORD_GOT ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CORE_VR_MSG_40 ] ) << SEND_MSG ;
		Return=M4_FALSE ;
		goto exit ;
	} ;

exit:

	m_poInsp->m_poTrace->TestEffect( Return, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return Return ;		
}

m4bool_t ClENG_VR::PreAnalex( ClENG_Analex::StExpressionEnv *ai_pstEnv ) 
{
	m4bool_t SwOk=M4_TRUE ;
	ClENG_Analex Analex ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( ENG_TGI_ANALEX,"ClENG_VR::PreAnalex()", ClMIT_Msg::ENTRY_FORMAT) 
			<< SEND_MSG  ;
	};
	#endif

	SwOk = M4_BOOL(SwOk && Analex.Init( ai_pstEnv, & m_oPrepareRecordset, 
				   ClMIT_Trace::PROPAGATE_ERROR ));
	SwOk = M4_BOOL(SwOk && Analex.Init( ai_pstEnv, & m_oCancelRecordset, 
				   ClMIT_Trace::PROPAGATE_ERROR ));
	SwOk = M4_BOOL(SwOk && Analex.Init( ai_pstEnv, & m_oCancelVR, 
				   ClMIT_Trace::PROPAGATE_ERROR ));

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( ENG_TGI_ANALEX,"ClENG_VR::PreAnalex() Returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	return SwOk ;
}


void ClENG_VR::GetTraceStr( GO_THROUGH_MODE ai_Enum, ClMIT_Str *ao_poStr ) 
{
	switch (ai_Enum) {
	case ClENG_VR::RECORD_GOT:
		ao_poStr->StrCpy("GoT by Record") ;
		break ;
	case ClENG_VR::SLICE_GOT:
		ao_poStr->StrCpy("GoT by Slice") ;
		break ;
	default:
		ao_poStr->StrCpy("GoT by Error!!!") ;
		break ;
	} ;
}

void ClENG_VR::GetTraceStr( RANGE_MODE ai_Enum, ClMIT_Str *ao_poStr ) 
{
	switch (ai_Enum) {
	case ClENG_VR::ALL_VR:
		ao_poStr->StrCpy("Range AllVR") ;
		break ;
	case ClENG_VR::SUBGROUP_VR:
		ao_poStr->StrCpy("Range Subgroup") ;
		break ;
	default:
		ao_poStr->StrCpy("Range Error!!!") ;
		break ;
	} ;
}

m4bool_t ClENG_VR::LoadGroupKey(m4bool_t ai_LoadAuxCompare) 
{
	m4bool_t R=M4_TRUE, KeyNULL=M4_FALSE, NextKeyNULL=M4_FALSE ;
	StGroup::GroupKey_t::iterator It,It2, End ;
	
	if (!ai_LoadAuxCompare) {

		for ( It=m_oGParam.m_oGroup.begin(), End=m_oGParam.m_oGroup.end(); It != End; It++ ) {
			(*It)->Load() ;
		} ;

	} else {

		ClMIT_ChnValue LocalResult ;

		for ( 
			It=m_oGParam.m_oAuxCompare.begin(), It2=m_oGParam.m_oGroup.begin(), 
			End=m_oGParam.m_oAuxCompare.end() ; It != End; It++, It2++ ) 
		{
			if ( (*It2)->m_oValue.IsNULL() ) KeyNULL=M4_TRUE ;
			(*It)->Load() ; if ( (*It)->m_oValue.IsNULL() ) NextKeyNULL=M4_TRUE ;
			//Si hay NULLs, no hace falta ningun test
			if ( !KeyNULL && !NextKeyNULL ) {	
				(*It)->m_oValue.Evaluate( ClMIT_ChnValue::NOT_EQUAL, &( (*It2)->m_oValue ), &LocalResult ) ;
				if ( LocalResult.GetBoolean() ) R=M4_FALSE ;
			} ;
		} ;
		if ( KeyNULL && NextKeyNULL  ) {
			R=M4_TRUE ;	//Si hay NULLs en ambas claves, mismo grupo incondicionalmente
		} else  if ( KeyNULL || NextKeyNULL ) {
			R=M4_FALSE ;	//Hay NULL en alguna clave, pero no en ambas, diferente grupo
		} ;
	} ;

	return R  ;
}

void ClENG_VR::DoOneOne() 
{
	OneOneList_t::iterator Item ;

	for ( Item=m_oOneOne.begin(); Item != m_oOneOne.end(); Item++ ) {
		(*Item).second->GoFirst() ;
	} ;
}

void ClENG_VR::ExecuteAction( ClENG_Expression *ai_poExpr ) 
{
	if ( ! ai_poExpr->IsEmpty() ) {
		ClMIT_ChnValue V; 
		ai_poExpr->Evaluate(&V,ClMIT_Trace::PROPAGATE_ERROR) ;  
	} ;
}

m4bool_t ClENG_VR::ExecuteFilter( ClENG_Expression *ai_poExpr ) 
{
	m4bool_t R=M4_FALSE ;

	if ( ! ai_poExpr->IsEmpty() ) {
		ClMIT_ChnValue V; 

		ai_poExpr->Evaluate(&V,ClMIT_Trace::PROPAGATE_ERROR) ;  
		R=V.GetBoolean() ;
	} ;
	return R ;
}


m4bool_t ClENG_VR::IsEOF() 
{
	return  M4_BOOL(m_GoT==RECORD_GOT ? m_ActualRecord==ClMIT_ChnNode::RECORD_EOF : 
					m_ActualSlice==ClMIT_ChnItem::SLICE_EOF);
}

m4bool_t ClENG_VR::IsNextEOF() 
{
	return  M4_BOOL(m_GoT==RECORD_GOT ? 
					m_ActualRecord==ClMIT_ChnNode::RECORD_EOF || m_NextRecord==ClMIT_ChnNode::RECORD_EOF : 
					m_ActualSlice==ClMIT_ChnItem::SLICE_EOF || m_NextSlice==ClMIT_ChnItem::SLICE_EOF);
}

m4bool_t ClENG_VR::EntryInRecordSet(NULL_IF_EMPTY ai_NullIfEmpty) 
{
	m4bool_t R=M4_TRUE ;
	m_bRewindVRCalculated=M4_FALSE ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_VR::EntryInRecordSet()", ClMIT_Msg::ENTRY_FORMAT) 
			<< CRAZY_MSG << SEND_MSG ;
	};
	#endif

	if ( ExecuteFilter(&m_oCancelRecordset) ) {
		R=M4_FALSE ;
	} ; 
	
	if (R) {
		ExecuteAction(&m_oPrepareRecordset) ;
		if (ai_NullIfEmpty!=NEVER_NULL ) {
			R = M4_BOOL(! IsNullIfEmpty(ai_NullIfEmpty)); 
			//Da igual restaurar o no posiciones: hay que hacer un Rewind.
		} ;
	} ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_VR::EntryInRecordSet() Return %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< R << CRAZY_MSG << SEND_MSG ;
	};
	#endif

	return R ;
}


void ClENG_VR::Rewind() 
{
	ClMIT_ChnNode::Record_t RecordFound ;
	ClMIT_ChnItem::SliceIndex_t SliceFound ;

	m_bNextVRCalculated = M4_FALSE ;
	m_VRProcessed = 0 ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_VR::Rewind()", ClMIT_Msg::ENTRY_FORMAT) 
			<< CRAZY_MSG << SEND_MSG ;
	};
	#endif

	if ( m_GoT==RECORD_GOT ) {

		//El primer registro encontrado que pasa el Cancel, es el primero de grupo.
		if ( !m_bRewindVRCalculated ) {
			m_bRewindVRCalculated=M4_TRUE ;
			RecordFound=ClMIT_ChnNode::RECORD_EOF ;
			if (m_Range==ALL_VR) {
				for( m_poRecordNode->GoFirst() ; !m_poRecordNode->IsEOF() ; m_poRecordNode->Next() ) {
					DoOneOne() ;
					if ( ! ExecuteFilter(&m_oCancelVR) ) {
						RecordFound=m_poRecordNode->GetNumRecord() ;
						break ;
					} ;
				} ;
			} else {
				for( m_poRecordNode->MoveTo(m_poVRFather->m_oGParam.m_FirstInG) ; 
					!m_poRecordNode->IsEOF() ; m_poRecordNode->Next() ) 
				{
					if ( m_poVRFather->SubGroupInRange() ) {
						DoOneOne() ;
						if ( ! ExecuteFilter(&m_oCancelVR) ) {
							RecordFound=m_poRecordNode->GetNumRecord() ;
							break ;
						} ;
					} else {
						//Fuera de grupo, no hay ninguno valido
						break ;
					} ;
				} ;
			} ;
		} else {
			RecordFound=m_RewindRecord ;
			MoveToRec(RecordFound) ;
			if (RecordFound!=ClMIT_ChnNode::RECORD_EOF) {
				DoOneOne() ;
			} ;
		} ;

		if ( RecordFound!=ClMIT_ChnNode::RECORD_EOF )  {
			//Hay registro valido ..
			m_ActualRecord=m_RewindRecord=RecordFound ;
			if ( IsGrouped() ) {
				m_oGParam.m_FirstInG=m_oGParam.m_NearestToLast=m_ActualRecord ;
				m_oGParam.m_LastInG=ClMIT_ChnNode::RECORD_EOF ;
				LoadGroupKey() ;	//Carga claves del grupo
			} else {
				m_oGParam.m_FirstInG=ClMIT_ChnNode::RECORD_EOF ;
			} ;
		} else {
			//No hay registro valido ..
			m_ActualRecord=m_RewindRecord=ClMIT_ChnNode::RECORD_EOF ; 
			m_oGParam.m_FirstInG=m_oGParam.m_NearestToLast=ClMIT_ChnNode::RECORD_EOF ;
			m_oGParam.m_LastInG=ClMIT_ChnNode::RECORD_EOF ;
		} ;


	} else { 
		//Modo Slice, range all y sin grupos a la fuerza

		m_ActualRecord=m_poVRFather->m_ActualRecord ;
		MoveToRec(m_ActualRecord) ;

		if ( !m_bRewindVRCalculated ) {
			m_bRewindVRCalculated=M4_TRUE ;

			SliceFound=ClMIT_ChnItem::SLICE_EOF ;
			for( m_oGuideItem.GoFirstSlice() ; !m_oGuideItem.IsSliceEOF() ; m_oGuideItem.GoNextSlice() ) {
				m_poItemMod->SetStartDate(m_oGuideItem.GetStartDate()) ;
				m_poItemMod->SetEndDate(  m_oGuideItem.GetEndDate()) ;
				DoOneOne() ;
				if ( ! ExecuteFilter(&m_oCancelVR) ) {
					SliceFound=m_oGuideItem.GetSliceIndex() ;
					break ;
				} ;
			} ;
		} else {
			SliceFound=m_RewindSlice ;
			m_oGuideItem.MoveToSlice(SliceFound) ;
			if (SliceFound!=ClMIT_ChnItem::SLICE_EOF) {
				m_poItemMod->SetStartDate(m_oGuideItem.GetStartDate()) ;
				m_poItemMod->SetEndDate(  m_oGuideItem.GetEndDate()) ;
				DoOneOne() ;
			} ;
		} ;

		if ( SliceFound!=ClMIT_ChnItem::SLICE_EOF )  {
			//Hay slice valido ..
			m_ActualSlice=SliceFound ;
		} else {
			//No hay registro valido ..
			m_ActualSlice=ClMIT_ChnItem::SLICE_EOF ; 
		} ;
	} ;

	//Estamos posicionados en el primer registro, si hay NullOnSons, se busca el Next. Next 
	//	a su vez pasara el NullOnSons tantas veces como haga falta.

	if ( m_bNullOnSons && !IsEOF() ) {
		if ( IsNullOnSons() ) {	//Pasa NullOnSons en primero, y si es nulo ...
			Next() ;				//... Next pasara el filtro en el resto.
		} ;
	} ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_VR::Rewind()", ClMIT_Msg::EXIT_FORMAT) 
			<< CRAZY_MSG << SEND_MSG ;
	};
	#endif

}

m4bool_t ClENG_VR::Next() 
{
	m4bool_t ActVRProc=M4_TRUE;
	m4bool_t SwOk=M4_TRUE ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_VR::Next()", ClMIT_Msg::ENTRY_FORMAT) 
			<< CRAZY_MSG << SEND_MSG ;
	};
	#endif

	while(M4_TRUE) {	//Bucle NullOnSons
	
		if (!m_bNextVRCalculated) FindNext();	

		if (m_GoT==RECORD_GOT ) {
			if (ActVRProc && m_ActualRecord!=ClMIT_ChnNode::RECORD_EOF) ++m_VRProcessed ;
			m_ActualRecord=m_NextRecord ;
			ReLocate() ;
			if( IsGrouped() ) {
				m_oGParam.m_FirstInG=m_oGParam.m_NearestToLast=m_ActualRecord ;
				m_oGParam.m_LastInG=ClMIT_ChnNode::RECORD_EOF ;
				if(m_ActualRecord!=ClMIT_ChnNode::RECORD_EOF) {				
					LoadGroupKey() ;
				};
			} ;
		} else {
			if (ActVRProc && m_ActualSlice!=ClMIT_ChnItem::SLICE_EOF) ++m_VRProcessed ;
			m_ActualSlice=m_NextSlice ;
			ReLocate() ;
		} ;
		m_bNextVRCalculated=M4_FALSE ;

		// ------------->Test NullOnSons
		if ( m_bNullOnSons && !IsEOF() ) {
			//Si no se anula, valido y salimos. Si se anula, bucle para pillar siguiente.
			if ( !IsNullOnSons() ) break ;	
		} else {
			//Si no hay NullOnSons, o en EOF, salimos del bucle
			break ;
		};
		ActVRProc=M4_FALSE ;
	} ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_VR::Next()", ClMIT_Msg::EXIT_FORMAT) 
			<< CRAZY_MSG << SEND_MSG ;
	};
	#endif

	return SwOk;
}

void ClENG_VR::ReLocate(RELOCATE_MODE ai_Mode) 
{
	if (m_GoT==RECORD_GOT) {
		if (ai_Mode==RELOC_ACTUAL) { 
			MoveToRec(m_ActualRecord) ;
			if (!IsEOF()) DoOneOne() ;
		} else {
			MoveToRec(m_NextRecord) ;
			if (!IsNextEOF()) DoOneOne() ;
		} ;
	} else {
		MoveToRec(m_ActualRecord) ;
		m_oGuideItem.MoveToSlice(ai_Mode==RELOC_ACTUAL ? m_ActualSlice : m_NextSlice ) ;
		if (m_oGuideItem.GetSliceIndex()!=ClMIT_ChnItem::SLICE_EOF) {
			m_poItemMod->SetStartDate(m_oGuideItem.GetStartDate()) ;
			m_poItemMod->SetEndDate(  m_oGuideItem.GetEndDate()) ;
			DoOneOne() ;
		} ;
	} ;
}

void ClENG_VR::FindNext() 
{
	ClMIT_ChnNode::Record_t RecordFound, LastRecordInG ;
	ClMIT_ChnItem::SliceIndex_t SliceFound ;
	m4uint32_t SaveVRProc=m_VRProcessed ;

	if (m_bNextVRCalculated) goto exit;
	m_bNextVRCalculated=M4_TRUE ;


	if (m_GoT==RECORD_GOT ) {

		RecordFound=ClMIT_ChnNode::RECORD_EOF ;

		if (m_ActualRecord==ClMIT_ChnNode::RECORD_EOF) goto exit;

		++m_VRProcessed ;	//Para que sea un valor valido en el CancelVR

		//PRIMER PASO: dejar en LastRecordInG el ultimo registro que pertence al grupo
		//CancelVR no interviene, solo los limites de grupos y subagrupaciones.,

		if ( IsGrouped() ) {

			//Como hay grupo, hayo el Next actualizando m_oGParam

			if ( m_oGParam.m_LastInG==ClMIT_ChnNode::RECORD_EOF ) {
				//No esta calculado el ultimo de grupo, vamos a por el.
				if (m_Range==ALL_VR) {
					//No es subgrupo, solo estamos limitados por nosotros mismos
					for ( m_poRecordNode->MoveTo(m_oGParam.m_NearestToLast ) ;
						!m_poRecordNode->IsEOF(); m_poRecordNode->Next() ) 
					{
						if ( !LoadGroupKey(M4_TRUE) ) {
							//Ya es otro grupo
							break ;
						} else {
							//Dentro del mismo grupo
							m_oGParam.m_LastInG=m_oGParam.m_NearestToLast=
								m_poRecordNode->GetNumRecord() ;
						} ;
					} ;
				} else {
					//Hay subgrupo, estamos limitados por el padre y por nosotros mismos
					for ( m_poRecordNode->MoveTo(m_oGParam.m_NearestToLast ) ;
						!m_poRecordNode->IsEOF(); m_poRecordNode->Next() ) 
					{
						if ( m_poVRFather->SubGroupInRange() ) {
							if ( !LoadGroupKey(M4_TRUE) ) {
								//Ya es otro grupo
								break ;
							} else {
								//Dentro del mismo grupo
								m_oGParam.m_LastInG=m_oGParam.m_NearestToLast=
									m_poRecordNode->GetNumRecord() ;
							} ;
						} else {
							//Fuera de grupo en padre, no es valido
							break ;
						} ;
					} ;
				} ;
			} ;
			//Aqui m_LastInG esta calculado a la fuerza. 
			LastRecordInG=m_oGParam.m_LastInG ;

		} else {

			//Sin grupo, el ultimo valido es en el que estamos
			LastRecordInG=m_ActualRecord ;
		} ;

		//LastRecordInG	YA tiene el ultimo de grupo valido (EOF si no hay registros)

		//SEGUNDO PASO:Hayar el siguiente comienzo de grupo valido. Como en Rewind, influye
		//	CancelVR, y limites del padre. RecordFound tendra el primer registro valido.

		if (m_Range==ALL_VR ) {
			//Solo hay que encontrar siguiente comienzo de grupo valido
			for( m_poRecordNode->MoveTo(LastRecordInG), m_poRecordNode->Next() ; 
				!m_poRecordNode->IsEOF() ; m_poRecordNode->Next() ) 
			{
				DoOneOne() ;
				if ( ! ExecuteFilter(&m_oCancelVR) ) {
					RecordFound=m_poRecordNode->GetNumRecord() ;
					break ;
				} ;
			} ;
		} else {
			//Para hayar el siguinte, el padre nos limita
			for( m_poRecordNode->MoveTo(LastRecordInG), m_poRecordNode->Next() ; 
				!m_poRecordNode->IsEOF() ; m_poRecordNode->Next() ) 
			{
				if ( m_poVRFather->SubGroupInRange() ) {
					DoOneOne() ;
					if ( ! ExecuteFilter(&m_oCancelVR) ) {
						RecordFound=m_poRecordNode->GetNumRecord() ;
						break ;
					} ;
				} else {
					//Fuera de grupo, no hay ninguno valido
					break ;
				} ;
			} ;

		} ;

		//TERCER PASO:Actualizacion de valores Next. Los valores de grupo ya se actualizaron.

		if ( RecordFound!=ClMIT_ChnNode::RECORD_EOF )  {
			//Hay registro valido ..
			m_NextRecord=RecordFound ;
		} else {
			//No hay registro valido ..
			m_NextRecord=ClMIT_ChnNode::RECORD_EOF ; 
		} ;

	} else {

		if (m_ActualSlice==ClMIT_ChnItem::SLICE_EOF) goto exit ;

		++m_VRProcessed ;	//Para que sea un valor valido en el CancelVR

		//Modo Slice, range all y sin grupos a la fuerza

		MoveToRec(m_ActualRecord) ;

		SliceFound=ClMIT_ChnItem::SLICE_EOF ;
		for( m_oGuideItem.MoveToSlice(m_ActualSlice), m_oGuideItem.GoNextSlice() ; 
			!m_oGuideItem.IsSliceEOF() ; m_oGuideItem.GoNextSlice() ) 
		{
			m_poItemMod->SetStartDate(m_oGuideItem.GetStartDate()) ;
			m_poItemMod->SetEndDate(  m_oGuideItem.GetEndDate()) ;
			DoOneOne() ;
			if ( ! ExecuteFilter(&m_oCancelVR) ) {
				SliceFound=m_oGuideItem.GetSliceIndex() ;
				break ;
			} ;
		} ;
		if ( SliceFound!=ClMIT_ChnItem::SLICE_EOF )  {
			//Hay slice valido ..
			m_NextSlice=SliceFound ;
		} else {
			//No hay registro valido ..
			m_NextSlice=ClMIT_ChnItem::SLICE_EOF ; 
		} ;


	} ;
exit:
	m_VRProcessed = SaveVRProc ;
	ReLocate(RELOC_NEXT) ;
	return ;
}

m4bool_t ClENG_VR::IsLast() 
{
	if (!m_bNextVRCalculated) {
		FindNext();
		ReLocate() ;
	} ;
	return IsNextEOF() ;
}

m4bool_t ClENG_VR::SubGroupInRange() 
{
	m4bool_t R=M4_TRUE ;
	ClMIT_ChnNode::Record_t Record=m_poRecordNode->GetNumRecord() ;

	//CancelVR no interviene

	if ( m_Range==SUBGROUP_VR ) R=m_poVRFather->SubGroupInRange() ;
	if ( R ) R = LoadGroupKey(M4_TRUE) ;

	if (R) {
		if ( Record > m_oGParam.m_NearestToLast ) 
			m_ActualRecord=m_oGParam.m_NearestToLast=Record ;
	} else {
		m_ActualRecord=m_oGParam.m_LastInG=m_oGParam.m_NearestToLast ;
	} ;

	return R ;
}



m4bool_t ClENG_VR::IsNullIfEmpty(NULL_IF_EMPTY ai_NullIfEmpty)  
{
	m4int16_t B ;
	m4bool_t Empty ;
	
	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_VR::IsNullIfEmpty()", ClMIT_Msg::ENTRY_FORMAT) 
			<< CRAZY_MSG << SEND_MSG ;
	};
	#endif

	if (ai_NullIfEmpty==NEVER_NULL) {

		Empty=M4_FALSE ;
		goto exit ;

	} else if ( ai_NullIfEmpty==THIS_SECTION || m_poOwnerSection->IsEndSection() ) {

		Rewind() ;
		Empty=IsEOF();
		goto exit ;

	} else {

		Empty=M4_TRUE ;
		Rewind() ;
		while ( Empty && !IsEOF() ) {

			//Aqui llego si registro padre es valido. Compruebo VR hijas.
	
			Empty = ai_NullIfEmpty==SON_OR_SECTIONS ? M4_TRUE : M4_FALSE ;

			// Se mira en todas secciones hijas, sean o no del mismo thread.

			for ( B=0; B<m_poOwnerSection->m_iNSons; ++B ) {

				if ( ! m_poOwnerSection->m_pstSon[B].m_poSon->m_oVR.EntryInRecordSet( 
					m_poOwnerSection->m_pstSon[B].m_poSon->m_oVR.m_NullIfEmpty) ) 
				{
					if  (ai_NullIfEmpty==SON_AND_SECTIONS) {
						Empty = M4_TRUE ;
						break ;
					} ;
				} else {
					if  (ai_NullIfEmpty==SON_OR_SECTIONS) {
						Empty = M4_FALSE ;
						break ;
					} ;
				};
			} ;
			Next() ;
		} ;
	} ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_VR::IsNullIfEmpty() Returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< CRAZY_MSG << Empty << SEND_MSG ;
	};
	#endif

	//Da igual la posicion donde se quede, tras un NullIfEmpty debe de hacerse un Rewind.

	return Empty ;
}


m4bool_t ClENG_VR::IsNullOnSons()  
{
	m4int16_t B ;
	m4bool_t Empty ;
	
	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_VR::IsNullOnSons()", ClMIT_Msg::ENTRY_FORMAT) 
			<< CRAZY_MSG << SEND_MSG ;
	};
	#endif

	if (!m_bNullOnSons) {
		Empty=M4_FALSE ;
		goto exit ;
	} ;

	// Se mira en todas secciones hijas, sean o no del mismo thread.
	Empty=M4_TRUE ;
	for ( B=0; B<m_poOwnerSection->m_iNSons; ++B ) {
		if ( ! ! m_poOwnerSection->m_pstSon[B].m_poSon->m_oVR.EntryInRecordSet( 
			m_poOwnerSection->m_pstSon[B].m_poSon->m_oVR.m_NullIfEmpty == NEVER_NULL ? 
			THIS_SECTION : m_poOwnerSection->m_pstSon[B].m_poSon->m_oVR.m_NullIfEmpty ) ) 
		{
			Empty=M4_FALSE ;
			break ;
		} ;
	} ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClENG_VR::IsNullOnSons() Returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< CRAZY_MSG << Empty << SEND_MSG ;
	};
	#endif

	return Empty ;
}



m4date_t ClENG_VR::GetFirstDate() 
{
	m4date_t R=0 ;

	if (m_GoT!=SLICE_GOT) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CORE_VR_MSG_60 ] ) 
			<< "ClENG_VR::GetFirstDate()" << SEND_MSG ;
		//Error local
	} else {
		R=m_oGuideItem.GetFirstStartDate() ;		
	} ;
	return R ;
}

m4date_t ClENG_VR::GetLastDate() 
{
	m4date_t R=0 ;

	if (m_GoT!=SLICE_GOT) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CORE_VR_MSG_60 ] ) 
			<< "ClENG_VR::GetLastDate()" << SEND_MSG ;
		//Error local
	} else {
		R=m_oGuideItem.GetLastEndDate() ;		
	} ;
	return R ;
}

m4date_t ClENG_VR::GetStartDate(ClMIT_ChnItem::SliceIndex_t ai_Index) 
{
	m4date_t R=0 ;

	if (m_GoT!=SLICE_GOT) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CORE_VR_MSG_60 ] ) 
			<< "ClENG_VR::GetStartDate()" << SEND_MSG ;
		//Error local
	} else {
		R=m_oGuideItem.GetStartDate(ai_Index) ;		
	} ;
	return R ;
}

m4date_t ClENG_VR::GetEndDate(ClMIT_ChnItem::SliceIndex_t ai_Index) 
{
	m4date_t R=0 ;

	if (m_GoT!=SLICE_GOT) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CORE_VR_MSG_60 ] ) 
			<< "ClENG_VR::GetEndDate()" << SEND_MSG ;
		//Error local
	} else {
		R=m_oGuideItem.GetEndDate(ai_Index) ;		
	} ;
	return R ;
}

ClMIT_ChnItem::SliceIndex_t ClENG_VR::GetSliceIndex() 
{
	ClMIT_ChnItem::SliceIndex_t R=0 ;

	if (m_GoT!=SLICE_GOT) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CORE_VR_MSG_60 ] ) 
			<< "ClENG_VR::GetSliceIndex()" << SEND_MSG ;
		//Error local
	} else {
		R=m_ActualSlice ;		
	} ;
	return R ;
}

ClMIT_ChnItem::SliceIndex_t ClENG_VR::GetSliceCount() 
{
	ClMIT_ChnItem::SliceIndex_t R=0 ;

	if (m_GoT!=SLICE_GOT) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CORE_VR_MSG_60 ] ) 
			<< "ClENG_VR::GetSliceCount()" << SEND_MSG ;
		//Error local
	} else {
		R=m_oGuideItem.GetNumSlices() ;		
	} ;
	return R ;
}


void ClENG_VR::Save(StSave *ao_poSave) 
{
	ao_poSave->m_ActualRecord=m_ActualRecord ;
	ao_poSave->m_NextRecord=m_NextRecord;
	ao_poSave->m_bNextVRCalculated=m_bNextVRCalculated;
	ao_poSave->m_RewindRecord=m_RewindRecord;
	ao_poSave->m_bRewindVRCalculated=m_bRewindVRCalculated;
	ao_poSave->m_ActualSlice=m_ActualSlice;
	ao_poSave->m_NextSlice=m_NextSlice;
	ao_poSave->m_RewindSlice=m_RewindSlice;
	ao_poSave->m_GroupFirstInG=m_oGParam.m_FirstInG;
	ao_poSave->m_GroupNearestToLast=m_oGParam.m_NearestToLast;
	ao_poSave->m_GroupLastInG=m_oGParam.m_LastInG;
	ao_poSave->m_VRProcessed=m_VRProcessed;
}

void ClENG_VR::Restore(StSave *ao_poSave) 
{
	m_ActualRecord=ao_poSave->m_ActualRecord ;
	m_NextRecord=ao_poSave->m_NextRecord ;
	m_bNextVRCalculated=ao_poSave->m_bNextVRCalculated ;
	m_RewindRecord=ao_poSave->m_RewindRecord ;
	m_bRewindVRCalculated=ao_poSave->m_bRewindVRCalculated ;
	m_ActualSlice=ao_poSave->m_ActualSlice ;
	m_NextSlice=ao_poSave->m_NextSlice ;
	m_RewindSlice=ao_poSave->m_RewindSlice ;
	m_oGParam.m_FirstInG=ao_poSave->m_GroupFirstInG ;
	m_oGParam.m_NearestToLast=ao_poSave->m_GroupNearestToLast ;
	m_oGParam.m_LastInG=ao_poSave->m_GroupLastInG ;
	m_VRProcessed=ao_poSave->m_VRProcessed ;
	ReLocate() ;
	if(m_ActualRecord!=ClMIT_ChnNode::RECORD_EOF) {
		LoadGroupKey() ;
	};
}


m4bool_t ClENG_VR::TestAlmostNRecords( m4uint32_t ai_uiNRecords ) 
{
	StSave SavePos ;

	Save(&SavePos) ;

	for ( ReLocate(); !IsEOF() && ai_uiNRecords ; Next(), --ai_uiNRecords ) ;

	Restore(&SavePos) ;
	return M4_BOOL(ai_uiNRecords==0);
}

void ClENG_VR::MoveToRec(ClMIT_ChnNode::Record_t ai_Record) 
{
	if (ai_Record!=ClMIT_ChnNode::RECORD_EOF) m_poRecordNode->MoveTo(ai_Record) ;
}

