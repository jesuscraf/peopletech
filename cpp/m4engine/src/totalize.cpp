//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             totalize.cpp
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

#include <stdarg.h>

#include "eng.h"
#include "engdf.inl"

#include "clargs.h"
#include "totalize.h"
#include "totalize.hmg"


//============================================================================ClENG_Tot

ClENG_Tot::ClENG_Tot()
{
	m_poInsp=GET_INSP();

	m_Type=NO_INIT ;
}

void ClENG_Tot::End() 
{
}

void ClENG_Tot::Init() 
{
	ClENG_Tot::End() ;

	Reset() ;
}

void ClENG_Tot::Reset() 
{
	m_Type=NO_INIT ;
	m_oFirst.End() ; m_oLast.End() ; m_oMin.End() ; m_oMax.End() ;
	m_iEntryCount=m_iTrueCount=m_iNullCount=0 ;
	m_dEntrySum=0 ;
}

void ClENG_Tot::Entry(ClMIT_ChnValue *ai_poValue) 
{
	ClMIT_ChnValue R ;

	if (ai_poValue->IsNULL()) {
		++m_iNullCount;
		return ;
	} ;

	if (m_Type==NO_INIT) {
		switch (ai_poValue->GetType() ) {
		case ClMIT_ChnValue::NUMERIC:
			m_Type=NUMERIC ;
			break ;
		case ClMIT_ChnValue::DATE:
			m_Type=DATE ;
			break ;
		case ClMIT_ChnValue::BOOLEAN:
			m_Type=BOOLEAN ;
			break ;
		default:
			m_Type=VARIANT ;
			break ;
		} ;
		m_oFirst.Init(ai_poValue) ;  
		m_oMin.Init(ai_poValue) ; m_oMax.Init(ai_poValue) ;
	} ;

	if (  ( m_Type==NUMERIC && ai_poValue->GetType()!=ClMIT_ChnValue::NUMERIC ) || 
		  ( m_Type==BOOLEAN && ai_poValue->GetType()!=ClMIT_ChnValue::BOOLEAN ) ||
		  ( m_Type==DATE    && ai_poValue->GetType()!=ClMIT_ChnValue::DATE ) ) 
	{
		m_Type=VARIANT ; 
	} ;

	++m_iEntryCount ;
	m_oLast.Init(ai_poValue) ;

	if (m_Type==NUMERIC) m_dEntrySum += ai_poValue->GetDouble() ;

	if (m_Type==NUMERIC || m_Type==DATE ) {

		ai_poValue->Evaluate(ClMIT_ChnValue::LESS, &m_oMin, &R ) ;		
		if ( R.GetBoolean() ) m_oMin.Init(ai_poValue) ;

		ai_poValue->Evaluate(ClMIT_ChnValue::GREATER, &m_oMax, &R ) ;		
		if ( R.GetBoolean() ) m_oMax.Init(ai_poValue) ;
	} ;

	if (m_Type==BOOLEAN) {
		if  ( ai_poValue->GetBoolean() ) ++m_iTrueCount ;
	} ;

}

void ClENG_Tot::Entry(ClENG_Tot *ai_poValue) 
{
	ClMIT_ChnValue R ;

	if (ai_poValue->GetType()==NO_INIT) {
		m_iEntryCount += ai_poValue->m_iEntryCount ;
		return ;
	} ;
	
	if (m_Type==NO_INIT) {
		m_Type = ai_poValue->GetType() ;
		m_oFirst.Init(&ai_poValue->m_oFirst) ;  
		m_oMin.Init(&ai_poValue->m_oMin) ; m_oMax.Init(&ai_poValue->m_oMax) ;
	} ;

	if (  ( m_Type==NUMERIC && ai_poValue->GetType()!=NUMERIC ) || 
		  ( m_Type==BOOLEAN && ai_poValue->GetType()!=BOOLEAN ) ||
		  ( m_Type==DATE    && ai_poValue->GetType()!=DATE ) ) 
	{
		m_Type=VARIANT ; 
	} ;

	m_iEntryCount += ai_poValue->m_iEntryCount;
	m_iTrueCount += ai_poValue->m_iTrueCount ;
	m_oLast.Init(&ai_poValue->m_oLast) ;

	if (m_Type==NUMERIC) m_dEntrySum += ai_poValue->m_dEntrySum ;

	if (m_Type==NUMERIC || m_Type==DATE ) {

		ai_poValue->m_oMin.Evaluate(ClMIT_ChnValue::LESS, &m_oMin, &R ) ;		
		if ( R.GetBoolean() ) m_oMin.Init(&ai_poValue->m_oMin) ;

		ai_poValue->m_oMax.Evaluate(ClMIT_ChnValue::GREATER, &m_oMax, &R ) ;		
		if ( R.GetBoolean() ) m_oMax.Init(&ai_poValue->m_oMax) ;
	} ;

}

m4double_t ClENG_Tot::GetSum() 
{
	if (m_Type==NO_INIT) {
		return 0 ;
	} else if (m_Type!=NUMERIC) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ TOTALIZE_MSG_10 ] ) 
			<< "SUM()" << SEND_MSG ;
		//Error local
		return 0 ;
	} else {
		return m_dEntrySum ;
	} ;
}

m4double_t ClENG_Tot::GetAvg() 
{
	if (m_Type==NO_INIT) {
		return 0 ;
	} else if (m_Type!=NUMERIC) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ TOTALIZE_MSG_10 ] ) 
			<< "AVG()" << SEND_MSG ;
		//Error local
		return 0 ;
	} else {
		//Evita el divide by zero
		return m_iEntryCount != 0 ? m_dEntrySum/m_iEntryCount : 0 ;
	} ;
}

void ClENG_Tot::GetMax(ClMIT_ChnValue *ao_poValue) 
{
	if (m_Type==NO_INIT) {
		ao_poValue->End() ;
	} else if (m_Type!=NUMERIC && m_Type!=DATE ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ TOTALIZE_MSG_10 ] ) 
			<< "MAX()" << SEND_MSG ;
		//Error local
		ao_poValue->End() ;
	} else {
		ao_poValue->Init(&m_oMax) ;
	} ;
}

void ClENG_Tot::GetMin(ClMIT_ChnValue *ao_poValue) 
{
	if (m_Type==NO_INIT) {
		ao_poValue->End() ;
	} else if (m_Type!=NUMERIC && m_Type!=DATE ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ TOTALIZE_MSG_10 ] ) 
			<< "MIN()" << SEND_MSG ;
		//Error local
	} else {
		ao_poValue->Init(&m_oMin) ;
	} ;
} 

m4uint32_t ClENG_Tot::GetTrueCount() 
{
	if (m_Type==NO_INIT) {
		return 0 ;
	} else if (m_Type!=BOOLEAN) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ TOTALIZE_MSG_10 ] ) 
			<< "TCOUNT()" << SEND_MSG ;
		//Error local
		return 0 ;
	} else {
		return m_iTrueCount ;
	} ;
}

m4uint32_t ClENG_Tot::GetFalseCount() 
{
	if (m_Type==NO_INIT) {
		return 0 ;
	} else if (m_Type!=BOOLEAN) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ TOTALIZE_MSG_10 ] ) 
			<< "FCOUNT()" << SEND_MSG ;
		//Error local
		return 0 ;
	} else {
		return m_iEntryCount-m_iTrueCount ;
	} ;
}

//============================================================================ClENG_KeyTot

ClENG_KeyTotNode::ClENG_KeyTotNode()
{
	m_SortBy=SEQUENTIAL ;
	m_poTot=NULL;
}

ClENG_KeyTotNode::~ClENG_KeyTotNode() 
{
	//NO destruir m_poTot, puede ser 'heredado' en otras asignaciones, ya que no se copia.
	//ClENG_KeyTot  se encarga de todo esto cuando deba.
}

ClENG_KeyTotNode::ClENG_KeyTotNode(const ClENG_KeyTotNode& ai_oSource) 
{
	*this = ai_oSource ;
}

ClENG_KeyTotNode & ClENG_KeyTotNode::operator = (const ClENG_KeyTotNode &ai_oOpe)
{
	m_SortBy=ai_oOpe.m_SortBy ;
	m_uiSeqOrder=ai_oOpe.m_uiSeqOrder ;
	m_oKey.Init(&ai_oOpe.m_oKey);
	m_oHeader.Init(&ai_oOpe.m_oHeader);
	m_poTot=ai_oOpe.m_poTot;
	m_poSon=ai_oOpe.m_poSon;
	return *this ;
}

m4bool_t ClENG_KeyTotNode::operator == (const ClENG_KeyTotNode &ai_oOpe) const
{
	if ( m_SortBy==SEQUENTIAL) {
		return M4_BOOL(m_uiSeqOrder==ai_oOpe.m_uiSeqOrder);
	} else {
		ClMIT_ChnValue R ;
		m_oKey.Evaluate(ClMIT_ChnValue::EQUAL,&ai_oOpe.m_oKey,&R) ;
		return R.GetBoolean() ;
	} ;
}

m4bool_t ClENG_KeyTotNode::operator < (const ClENG_KeyTotNode &ai_oOpe) const
{
	switch( m_SortBy ) {
	case SEQUENTIAL:
		return M4_BOOL(m_uiSeqOrder<ai_oOpe.m_uiSeqOrder);
		break ;
	case ASCENDING:
		{
			ClMIT_ChnValue R ;
			m_oKey.Evaluate(ClMIT_ChnValue::LESS,&ai_oOpe.m_oKey,&R) ;
			return R.GetBoolean() ;
		} ;
		break;
	default:
		{
			ClMIT_ChnValue R ;
			m_oKey.Evaluate(ClMIT_ChnValue::GREATER,&ai_oOpe.m_oKey,&R) ;
			return R.GetBoolean() ;
		} ;
		break;
	} ;
}

//============================================================================ClENG_KeyTot

#define TOTALIZE_INDEX(Tree,Sub) (Tree)*(m_iTotCount)+(Sub)

ClENG_KeyTot::ClENG_KeyTot()
{
	m_poInsp=GET_INSP();
	m_piIndexKey=NULL ;
	m_poValueKey=NULL ;
	m_poDerived=NULL ;
	m_poMaster=NULL;
	m_piDerivedLevel=NULL;
}

void ClENG_KeyTot::EndDerived() 
{
	m4int16_t B ;

	M4_DELETE_ARRAY(m_poInsp->m_poMemProf,ENG_MGI_TOT, m_poDerived, ClENG_KeyTot,m_oDerivedIndex.GetCount()  ) ;
	M4_DELETE_ARRAY(m_poInsp->m_poMemProf,ENG_MGI_TOT, m_piDerivedLevel, m4int16_t, m_iLevelCount ) ;
	m_oDerivedIndex.End() ;
	m_poMaster=NULL ;
} ;

void ClENG_KeyTot::End() 
{
	EndDerived() ;

	Remove(&m_RootLevel) ;
	m_iTotCount=m_iTreeCount=0 ;
	m_oTreeIndex.End() ;
	m_oTotIndex.End() ;
	M4_DELETE_ARRAY(m_poInsp->m_poMemProf,ENG_MGI_TOT, m_piIndexKey, m4int16_t, m_iLevelCount ) ;
	M4_DELETE_ARRAY(m_poInsp->m_poMemProf,ENG_MGI_TOT, m_poValueKey, ClMIT_ChnValue, m_iLevelCount ) ;
	m_iLevelCount=0 ;
}

void ClENG_KeyTot::Remove(KeyTotList_t * ao_poList, KeyTotList_t::iterator & ai_poIt) 
{
	if (ai_poIt==ao_poList->end()) return ;	
	Remove(&(*ai_poIt).m_poSon->m_oSon) ;
	M4_DELETE_ARRAY(m_poInsp->m_poMemProf,ENG_MGI_TOT, (*ai_poIt).m_poTot, ClENG_Tot, 
		GetTreeCount()*GetTotCount() ) ;
	M4_DELETE_ITEM(m_poInsp->m_poMemProf, ENG_MGI_TOT, (*ai_poIt).m_poSon, StKeyTotSon ) ;
	ao_poList->erase(ai_poIt) ;
}

void ClENG_KeyTot::Remove(KeyTotList_t * ao_poList) 
{
	KeyTotList_t::iterator It ;

	if (!ao_poList) return ;
	for (It=ao_poList->begin();It!=ao_poList->end();++It) {
		Remove(&(*It).m_poSon->m_oSon) ;
		M4_DELETE_ARRAY(m_poInsp->m_poMemProf,ENG_MGI_TOT, (*It).m_poTot, ClENG_Tot, 
			GetTreeCount()*GetTotCount() ) ;
		M4_DELETE_ITEM(m_poInsp->m_poMemProf, ENG_MGI_TOT, (*It).m_poSon, StKeyTotSon ) ;
	} ;
	ao_poList->clear() ;
}

m4bool_t ClENG_KeyTot::Init(m4int16_t ai_LevelCount, 
		const m4char_t * ai_pcIdTree, const m4char_t * ai_pcIdTot, 
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t B ;

	ClENG_KeyTot::End() ;

	m_iLevelCount=ai_LevelCount;
	if (m_iLevelCount<1) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ TOTALIZE_MSG_90 ] ) << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	M4_NEW_ARRAY(m_poInsp->m_poMemProf,ENG_MGI_TOT, m_piIndexKey, m4int16_t, m_iLevelCount ) ;
	M4_NEW_ARRAY(m_poInsp->m_poMemProf,ENG_MGI_TOT, m_poValueKey, ClMIT_ChnValue, m_iLevelCount ) ;
	
	SwOk = M4_BOOL(SwOk && m_oTreeIndex.Init(ai_pcIdTree, ClMIT_Trace::PROPAGATE_ERROR));
	SwOk = M4_BOOL(SwOk && m_oTotIndex.Init(ai_pcIdTot, ClMIT_Trace::PROPAGATE_ERROR));
	if (!SwOk) goto exit ;

	m_iTotCount = m_oTotIndex.GetCount() ;
	m_iTreeCount = m_oTreeIndex.GetCount() ;

exit:
	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return SwOk ;		
}

m4bool_t ClENG_KeyTot::InitTest( m4int16_t ai_LevelCount, 
		const m4char_t * ai_pcIdTree, const m4char_t * ai_pcIdTot, 
		const m4char_t * ai_pcLevelInfo, m4bool_t ai_bSwIncr,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t SwOk=M4_TRUE ;
	ClMIT_Args Arg ;
	m4int16_t LastBK=-1 ;

	SwOk = M4_BOOL( SwOk && Init( ai_LevelCount, ai_pcIdTree, ai_pcIdTot, ai_ErrorEffect ) ) ;
	if (!SwOk) goto exit ;

	Arg.Init( ai_pcLevelInfo, '.', ", \t" ) ;

	ResetTestLevel(0, &Arg, ai_bSwIncr, &LastBK ) ;

exit:
	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return SwOk ;		
}

void ClENG_KeyTot::ResetTestLevel(m4int16_t ai_iLevel, ClMIT_Args *ai_poArg, m4bool_t ai_bSwIncr,
		m4int16_t *ao_piLastBK ) 
{
	m4int16_t BK, BK_Count ;
	m4char_t Str[10] ;

	if ( ai_poArg->GetArgCount() > ai_iLevel ) {
		BK_Count = ai_poArg->GetLong(ai_iLevel,0) ;
		if (BK_Count<0) BK_Count=1 ;
	} else {
		BK_Count = 1 ;
	} ;

	if ( ai_iLevel+1 == m_iLevelCount  ) {
		if ( ai_bSwIncr  ) {
			if ( *ao_piLastBK<0 ) {
				*ao_piLastBK =BK_Count ;
			} else {
				++(*ao_piLastBK) ;
				BK_Count = *ao_piLastBK ;
			} ;
		} ;
	} ;

	for ( BK=0; BK<BK_Count; ++BK ) {
		SetIndexKey( ai_iLevel, BK ) ;
		Str[0]= (ai_iLevel % 2 ? 'a' : 'A' ) + BK ;
		Str[1]=M4_END_STR ;
		GetValueKey( ai_iLevel )->InitString( Str ) ;
		NewKey(ai_iLevel+1) ;
		if ( ai_iLevel+1 != m_iLevelCount ) {
			ResetTestLevel(ai_iLevel+1, ai_poArg, ai_bSwIncr, ao_piLastBK ) ;
		} ;
	} ;

}


m4bool_t ClENG_KeyTot::InitDerived( m4char_t *ai_pcCommand,
	ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t SwOk=M4_TRUE ;
	ClMIT_Args Args ;
	m4int16_t DIndex, DLCount, DLIndex, B ;
	ClMIT_Str InitTree, InitTot ;

	EndDerived() ;	

	//Inicializa DerivedIndex, y haya Args
	SwOk=M4_BOOL(SwOk&&m_oDerivedIndex.Init(ai_pcCommand,ClMIT_Trace::PROPAGATE_ERROR,&Args));
	if (!SwOk || m_oDerivedIndex.GetCount()==0 ) goto exit ;

	//Crea array de derivados en el padre
	M4_NEW_ARRAY(m_poInsp->m_poMemProf,ENG_MGI_TOT, m_poDerived, ClENG_KeyTot, m_oDerivedIndex.GetCount()  ) ;

	for (DIndex=0; DIndex<Args.GetArgCount();++DIndex) {

		//Inicializacion de un objeto derivado, primero inicializo normal y luego datos derivado,
		//	ya que la segunda parte usa datos de la primera inicializacion.

		//Inicializo derivado de forma normal
		m_oTreeIndex.GetInitString(&InitTree) ;	m_oTotIndex.GetInitString(&InitTot) ;
		DLCount=Args.GetSubArgCount(DIndex)-1 ;
		SwOk=M4_BOOL(SwOk&&m_poDerived[DIndex].Init(DLCount,InitTree,InitTot,ClMIT_Trace::PROPAGATE_ERROR));
		//Si  error, no incializa lo de demas, pero sigue siendo un objeto a borrar por el padre
		if (!SwOk) continue ;	
		
		//Inicializo derivado con particularidades de derivacion
		
		m_poDerived[DIndex].m_poMaster=this ;
		M4_NEW_ARRAY(m_poInsp->m_poMemProf,ENG_MGI_TOT, m_poDerived[DIndex].m_piDerivedLevel, m4int16_t, 
			m_poDerived[DIndex].m_iLevelCount ) ;
		for (DLIndex=0;DLIndex<DLCount;++DLIndex) {
			m_poDerived[DIndex].m_piDerivedLevel[DLIndex]=Args.GetLong(DIndex,DLIndex) ;
			if (!M4_IN_RANGE(m_poDerived[DIndex].m_piDerivedLevel[DLIndex],1,m_iLevelCount)) {
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ TOTALIZE_MSG_100 ] ) 
					<< Args.GetArg(DIndex) << SEND_MSG ;
				//Error local
				SwOk=M4_FALSE ;
			} ;
			for (B=0; B<DLIndex; ++B) {
				if (m_poDerived[DIndex].m_piDerivedLevel[DLIndex]==m_poDerived[DIndex].m_piDerivedLevel[B]) {
					*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ TOTALIZE_MSG_110 ] ) 
						<< Args.GetArg(DIndex) << SEND_MSG ;
					SwOk=M4_FALSE ;
				} ;
			} ;

		} ;
	} ;

	if (!SwOk) goto exit ;

	//Ahora debo inicializar todos los derivados

	for (DIndex=0; DIndex<Args.GetArgCount();++DIndex) {
		m_poDerived[DIndex].InitDerivedNode(&m_poDerived[DIndex].m_RootLevel, 1) ;
	} ;

exit:
	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return SwOk ;		
}

m4bool_t ClENG_KeyTot::Find( FIND_ACTION ai_Action,
	KeyTotList_t * &ao_poList, KeyTotList_t::iterator & ao_poIt, 
	ClMIT_ChnValue *ai_poSubKey, m4int16_t ai_iLevel, 
	m4int16_t ai_iTreeIndex, m4int16_t ai_iSubIndex, 
	TOT_ACTION ai_TotAction, 
	ClMIT_ChnValue *ai_poEntry, ClENG_Tot *ai_poTotEntry ) 
{
	m4bool_t SwFound ;
	m4int16_t B, T ;

	if (!ai_poSubKey || ai_iLevel==0) {

		//Sin subclave

		SwFound=M4_FALSE ;

	} else if ( 
		(!M4_IN_RANGE(ai_iTreeIndex,0,GetTreeCount()-1) && 
		!(ai_iTreeIndex==ON_ALL_TREES && ai_TotAction!=NONE_ACTION ) ) || 
			!M4_IN_RANGE(ai_iSubIndex,0,GetTotCount()-1) ) 
	{

		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ TOTALIZE_MSG_60 ] ) << SEND_MSG ;
		//Error local
		SwFound=M4_FALSE ;

	} else {

		//Lista no vacia, se busca si existe o no.

		ao_poList=&m_RootLevel ;
		SwFound=M4_TRUE ;
		for (B=0; B<ai_iLevel;++B)  {	
			GetItFromKey( ao_poList, &ai_poSubKey[B], ao_poIt) ;		
			if (ao_poIt != ao_poList->end() ) {
				//Todo OK, la lista e iterador apuntan a la busqueda correcta.
				if ( ai_TotAction!=NONE_ACTION ) {
					if (ai_iTreeIndex==ON_ALL_TREES) {
						for (T=0;T<GetTreeCount();++T) {
							switch(ai_TotAction) {
							case ENTRY_VALUE_ACTION:
								(*ao_poIt).m_poTot
									[TOTALIZE_INDEX(T,ai_iSubIndex)].Entry(ai_poEntry) ;
								break;
							case ENTRY_TOT_ACTION:
								(*ao_poIt).m_poTot
									[TOTALIZE_INDEX(T,ai_iSubIndex)].Entry(ai_poTotEntry) ;
								break;
							case RESET_ACTION:
								(*ao_poIt).m_poTot
									[TOTALIZE_INDEX(T,ai_iSubIndex)].Reset() ;
								break;
							} ;
						} ;
					} else {
						switch(ai_TotAction) {
						case ENTRY_VALUE_ACTION:
							(*ao_poIt).m_poTot
								[TOTALIZE_INDEX(ai_iTreeIndex,ai_iSubIndex)].Entry(ai_poEntry) ;
							break;
						case ENTRY_TOT_ACTION:
							(*ao_poIt).m_poTot
								[TOTALIZE_INDEX(ai_iTreeIndex,ai_iSubIndex)].Entry(ai_poTotEntry) ;
							break;
						case RESET_ACTION:
							(*ao_poIt).m_poTot
								[TOTALIZE_INDEX(ai_iTreeIndex,ai_iSubIndex)].Reset() ;
							break;
						} ;
					} ;
				} ;
			} else {
				//Fallo, la lista e iterador apuntan a la busqueda incorrecta.
				SwFound=M4_FALSE ;
				break ;
			} ;
			//Evita que en la ultima iteracion se apunte a la lista vacia del ultimo nodo
			if (B<ai_iLevel-1) ao_poList= &(*ao_poIt).m_poSon->m_oSon ;					
		} ;
	} ;

	if (!SwFound && ai_Action==FIND_REQ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ TOTALIZE_MSG_30 ] ) << SEND_MSG ;
		//Error local
	} ;
	return SwFound ;
}

m4bool_t ClENG_KeyTot::Find( FIND_ACTION ai_Action,
		KeyTotList_t * &ao_poList, KeyTotList_t::iterator & ao_poIt, 
		m4int16_t *ai_piSubKey, m4int16_t ai_iLevel, 
		m4int16_t ai_iTreeIndex, m4int16_t ai_iSubIndex, 
		TOT_ACTION ai_TotAction, 
		ClMIT_ChnValue *ai_poEntry, ClENG_Tot *ai_poTotEntry ) 
{
	m4bool_t SwFound ;
	m4int16_t B, T ;

	if (!ai_piSubKey || ai_iLevel==0) {

		//Sin subclave

		SwFound=M4_FALSE ;

	} else if (
		(!M4_IN_RANGE(ai_iTreeIndex,0,GetTreeCount()-1) && 
		!(ai_iTreeIndex==ON_ALL_TREES && ai_TotAction!=NONE_ACTION ) ) || 
			!M4_IN_RANGE(ai_iSubIndex,0,GetTotCount()-1) ) 
	{

		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ TOTALIZE_MSG_60 ] ) << SEND_MSG ;
		//Error local
		SwFound=M4_FALSE ;


	} else {

		//Lista no vacia, se busca si existe o no.

		ao_poList=&m_RootLevel ;
		SwFound=M4_TRUE ;
		for (B=0; B<ai_iLevel;++B)  {	
			GetItFromIndex(ao_poList,ai_piSubKey[B],ao_poIt) ;
			if (ao_poIt != ao_poList->end() ) {
				//Todo OK, la lista e iterador apuntan a la busqueda correcta.
				if ( ai_TotAction!=NONE_ACTION ) {
					if (ai_iTreeIndex==ON_ALL_TREES) {
						for (T=0;T<GetTreeCount();++T) {
							switch(ai_TotAction) {
							case ENTRY_VALUE_ACTION:
								(*ao_poIt).m_poTot
									[TOTALIZE_INDEX(T,ai_iSubIndex)].Entry(ai_poEntry) ;
								break;
							case ENTRY_TOT_ACTION:
								(*ao_poIt).m_poTot
									[TOTALIZE_INDEX(T,ai_iSubIndex)].Entry(ai_poTotEntry) ;
								break;
							case RESET_ACTION:
								(*ao_poIt).m_poTot
									[TOTALIZE_INDEX(T,ai_iSubIndex)].Reset() ;
								break;
							} ;
						} ;
					} else {
						switch(ai_TotAction) {
						case ENTRY_VALUE_ACTION:
							(*ao_poIt).m_poTot
								[TOTALIZE_INDEX(ai_iTreeIndex,ai_iSubIndex)].Entry(ai_poEntry) ;
							break;
						case ENTRY_TOT_ACTION:
							(*ao_poIt).m_poTot
								[TOTALIZE_INDEX(ai_iTreeIndex,ai_iSubIndex)].Entry(ai_poTotEntry) ;
							break;
						case RESET_ACTION:
							(*ao_poIt).m_poTot
								[TOTALIZE_INDEX(ai_iTreeIndex,ai_iSubIndex)].Reset() ;
							break;
						} ;
					} ;
				} ;
			} else {
				//Fallo, la lista e iterador apuntan a la busqueda incorrecta.
				SwFound=M4_FALSE ;
				break ;
			} ;
			//Evita que en la ultima iteracion se apunte a la lista vacia del ultimo nodo
			if (B<ai_iLevel-1) ao_poList= &(*ao_poIt).m_poSon->m_oSon ;					
		} ;
	} ;

	if (!SwFound && ai_Action==FIND_REQ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ TOTALIZE_MSG_30 ] ) << SEND_MSG ;
		//Error local
	} ;
	return SwFound ;
}

m4bool_t ClENG_KeyTot::TotEntryByValue( 
	m4int16_t ai_iLevel, 
	m4int16_t ai_iTreeIndex, m4int16_t ai_iSubIndex, 
	ClMIT_ChnValue *ai_poEntry, ClENG_Tot *ai_poTotEntry ) 
{
	m4bool_t SwOk=M4_TRUE ;
	KeyTotList_t *List ;
	KeyTotList_t::iterator It ;

	SwOk=Find( FIND_REQ, List, It, m_poValueKey, ai_iLevel, 
		ai_iTreeIndex==ON_ALL_TREES ? 0: ai_iTreeIndex, ai_iSubIndex,
		NONE_ACTION) ;
	if (!SwOk) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ TOTALIZE_MSG_40 ] ) << SEND_MSG ;
		//Error local
	} else {
		Find( FIND_REQ, List, It, m_poValueKey, ai_iLevel, ai_iTreeIndex, ai_iSubIndex, 
			ai_poTotEntry ? ENTRY_TOT_ACTION : ENTRY_VALUE_ACTION, ai_poEntry, ai_poTotEntry ) ;
	} ;

	return SwOk ;
}

m4bool_t ClENG_KeyTot::TotResetByValue( 
	m4int16_t ai_iLevel, 
	m4int16_t ai_iTreeIndex, m4int16_t ai_iSubIndex ) 
{
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t B ;
	KeyTotList_t *List ;
	KeyTotList_t::iterator It ;

	SwOk=Find( FIND_REQ, List, It, m_poValueKey, ai_iLevel, 
		ai_iTreeIndex==ON_ALL_TREES ? 0: ai_iTreeIndex, ai_iSubIndex,
		NONE_ACTION) ;
	if (!SwOk) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ TOTALIZE_MSG_40 ] ) << SEND_MSG ;
		//Error local
	} else {
		Find( FIND_REQ, List, It, m_poValueKey, ai_iLevel, ai_iTreeIndex, ai_iSubIndex, 
			RESET_ACTION) ;
		if ( m_poDerived ) {
			for (B=0;B<m_oDerivedIndex.GetCount();++B) {
				m_poDerived[B].TotResetByValue(ai_iLevel, ai_iTreeIndex, ai_iSubIndex ) ;
			} ;
		} ;
	} ;

	return SwOk ;
}

m4bool_t ClENG_KeyTot::TotReset( m4int16_t ai_iTreeIndex ) 
{
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t B ;
	
	if (!M4_IN_RANGE(ai_iTreeIndex,0,GetTreeCount()-1) &&  ai_iTreeIndex!=ON_ALL_TREES )  {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ TOTALIZE_MSG_60 ] ) << SEND_MSG ;
		//Error local
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	TotReset(&m_RootLevel,ai_iTreeIndex) ;

	if ( m_poDerived ) {
		for (B=0;B<m_oDerivedIndex.GetCount();++B) {
			m_poDerived[B].TotReset(ai_iTreeIndex) ;
		} ;
	} ;

exit:
	return SwOk ;
}

void ClENG_KeyTot::SynchronizeEntry() 
{
	m4int16_t B ;

	if ( m_poDerived ) {
		for (B=0;B<m_oDerivedIndex.GetCount();++B) {
			m_poDerived[B].EntryDerivedNode(&m_poDerived[B].m_RootLevel,1) ;
			m_poDerived[B].SynchronizeEntry() ;
		} ;
	} ;
}


void ClENG_KeyTot::TotReset(KeyTotList_t * ao_poList, m4int16_t ai_iTreeIndex) 
{
	KeyTotList_t::iterator It ;
	ClENG_Tot *ArrTot ;
	m4int16_t B1, B2, TreeB, TreeE ;

	if (!ao_poList) return ;

	if (ai_iTreeIndex==ON_ALL_TREES)  {
		TreeB=0;
		TreeE=GetTreeCount()-1 ;
	} else {
		TreeB=ai_iTreeIndex;
		TreeE=ai_iTreeIndex ;
	} ;
	for (It=ao_poList->begin();It!=ao_poList->end();++It) {
		TotReset(&(*It).m_poSon->m_oSon,ai_iTreeIndex) ;
		ArrTot=(*It).m_poTot ;
		for (B1=TreeB;B1<=TreeE;++B1) {
			for (B2=0;B2<GetTotCount();++B2) {
				ArrTot[TOTALIZE_INDEX(B1,B2)].Reset() ; 
			} ;
		} ;
	} ;
}

m4bool_t ClENG_KeyTot::NewKey( m4int16_t ai_iLevel, ClMIT_ChnValue *ai_poHeader ) 
{
	KeyTotList_t *FoundList ;
	KeyTotList_t::iterator FoundIt ;
	m4bool_t SwOk=M4_TRUE, SwFound ;
	ClENG_KeyTotNode Node ;

	if ( m_iTreeCount<1 || m_iTotCount<1 ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ TOTALIZE_MSG_50 ] ) << SEND_MSG ;
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	if (ai_iLevel<1) {

		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ TOTALIZE_MSG_90 ] ) << SEND_MSG ;
		//Error local
		SwOk=M4_FALSE ;
		goto exit;

	} else if (ai_iLevel==1) {

		//No hay clave, añadire a la lista raiz
		FoundList=&m_RootLevel ;

	} else {

		SwFound=Find( FIND_REQ, FoundList, FoundIt, m_poValueKey, ai_iLevel-1, 0, 0 ) ;
		if (SwFound) {
			//Subclave encontrada, añadire clave en lista hija
			FoundList=&(*FoundIt).m_poSon->m_oSon ;	
		} else {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ TOTALIZE_MSG_30 ] ) << SEND_MSG ;
			//Error local
			SwOk=M4_FALSE ;
			goto exit;
		} ;
	} ;

	//Compruebo que no es clave duplicada, e inserto
	GetItFromKey( FoundList, &m_poValueKey[ai_iLevel-1], FoundIt ) ;
	if (FoundIt != FoundList->end() ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ TOTALIZE_MSG_20 ] ) << SEND_MSG ;
		//Error local
		SwOk=M4_FALSE ;
		goto exit;
	} else {
		M4_NEW_ITEM(m_poInsp->m_poMemProf, ENG_MGI_TOT, Node.m_poSon, StKeyTotSon ) ;
		M4_NEW_ARRAY(m_poInsp->m_poMemProf,ENG_MGI_TOT, Node.m_poTot, ClENG_Tot, 
			GetTreeCount()*GetTotCount() ) ;
		Node.m_SortBy=ClENG_KeyTotNode::SEQUENTIAL ;
		Node.m_oKey.Init(&m_poValueKey[ai_iLevel-1]) ;
		Node.m_oHeader.Init( ai_poHeader ?  ai_poHeader : & Node.m_oKey )  ;
		Node.m_uiSeqOrder=(*FoundList).size() ;
		(*FoundList).push_back( Node );
	} ;
exit:
	return SwOk ;
}

m4bool_t ClENG_KeyTot::RemoveKey( m4int16_t ai_iLevel ) 
{
	m4bool_t SwOk=M4_TRUE ;
	KeyTotList_t *FoundList ;
	KeyTotList_t::iterator FoundIt ;

	SwOk=Find( FIND_REQ, FoundList, FoundIt, m_poValueKey, ai_iLevel, 0, 0 ) ;
	if (SwOk) {
		Remove(FoundList,FoundIt) ;
	} ;

	return SwOk ;
}

m4bool_t ClENG_KeyTot::GetInfoByValue( 
		FIND_ACTION ai_Action, m4int16_t ai_iLevel,
		m4int16_t ai_iTreeIndex, m4int16_t ai_iSubIndex, 
		StInfo *ao_pstInfo ) 
{
	KeyTotList_t *FoundList ;
	KeyTotList_t::iterator FoundIt ;
	m4bool_t SwOk=M4_TRUE ;

	ao_pstInfo->m_poTot=NULL ;
	ao_pstInfo->m_poKey=NULL ;
	ao_pstInfo->m_oSonCount=0 ;

	if (ai_iLevel==0) {
		SwOk=M4_TRUE ;
		ao_pstInfo->m_oSonCount=m_RootLevel.size() ;
	} else {
		SwOk=Find( ai_Action, FoundList, FoundIt, m_poValueKey, ai_iLevel, 
			ai_iTreeIndex, ai_iSubIndex ) ;
		if (SwOk) {
			ao_pstInfo->m_poTot=&(*FoundIt).m_poTot[TOTALIZE_INDEX(ai_iTreeIndex,ai_iSubIndex)] ;
			ao_pstInfo->m_poKey=&(*FoundIt).m_oKey ;
			ao_pstInfo->m_poHeader=&(*FoundIt).m_oHeader ;
			ao_pstInfo->m_oSonCount=(*FoundIt).m_poSon->m_oSon.size() ;
		} ;
	} ;
	return SwOk ;
}

m4bool_t ClENG_KeyTot::GetInfoByIndex( 
	FIND_ACTION ai_Action, m4int16_t ai_iLevel,
	m4int16_t ai_iTreeIndex, m4int16_t ai_iSubIndex, 
	StInfo *ao_pstInfo ) 
{
	KeyTotList_t *FoundList ;
	KeyTotList_t::iterator FoundIt ;
	m4bool_t SwOk=M4_TRUE ;

	ao_pstInfo->m_poTot=NULL ;
	ao_pstInfo->m_poKey=NULL ;
	ao_pstInfo->m_oSonCount=0 ;

	if (ai_iLevel==0) {
		SwOk=M4_TRUE ;
		ao_pstInfo->m_oSonCount=m_RootLevel.size() ;
	} else {
		SwOk=Find( ai_Action, FoundList, FoundIt, m_piIndexKey, ai_iLevel, 
			ai_iTreeIndex, ai_iSubIndex ) ;
		if (SwOk) {
			ao_pstInfo->m_poTot=&(*FoundIt).m_poTot[TOTALIZE_INDEX(ai_iTreeIndex,ai_iSubIndex)] ;
			ao_pstInfo->m_poKey=&(*FoundIt).m_oKey ;
			ao_pstInfo->m_poHeader=&(*FoundIt).m_oHeader ;
			ao_pstInfo->m_oSonCount=(*FoundIt).m_poSon->m_oSon.size() ;
		} ;
	} ;
	return SwOk ;
}


m4bool_t ClENG_KeyTot::GetItFromIndex(KeyTotList_t * ao_poList, m4int16_t ai_iIndex, 
		KeyTotList_t::iterator & ao_poIt) 
{
	if ( !M4_IN_RANGE(ai_iIndex,0,ao_poList->size()-1) ) {
		ao_poIt=ao_poList->end() ;
		return M4_FALSE ;
	} else {
		ao_poIt=ao_poList->begin() ;
		while (ai_iIndex>0) {
			--ai_iIndex ;
			++ao_poIt	;
		} ;
		return M4_TRUE ;
	} ;
}

m4bool_t ClENG_KeyTot::GetItFromKey(KeyTotList_t * ao_poList, ClMIT_ChnValue * ai_poKey, 
		KeyTotList_t::iterator & ao_poIt) 
{
	ClMIT_ChnValue R ;

	ao_poIt=ao_poList->begin() ;
	while (ao_poIt!=ao_poList->end()) {
		(*ao_poIt).m_oKey.Evaluate( ClMIT_ChnValue::EQUAL, ai_poKey, &R ) ;
		if ( R.GetBoolean() ) break ;
		else ++ao_poIt ;
	} ;
	return M4_BOOL(ao_poIt!=ao_poList->end());
}

m4int16_t ClENG_KeyTot::GetConsLevelByIndex(m4int16_t ai_iLevel) 
{
	m4int16_t *ai_piSubKey ;
	
	for ( ai_piSubKey=&m_piIndexKey[ai_iLevel-1]; ai_iLevel>0; --ai_iLevel, --ai_piSubKey ) {
		if (*ai_piSubKey != TOTALIZE_LEVEL) break ;
	} ;
	return ai_iLevel ;
}

m4bool_t ClENG_KeyTot::IsPartialLoad(m4int16_t ai_iLevel) 
{
	m4int16_t *ai_piSubKey ; 
	m4bool_t R ;

	ai_iLevel=GetConsLevelByIndex(ai_iLevel) ;
	if (ai_iLevel==0) {
		R=M4_TRUE ;
	} else {
		R=M4_FALSE ;
		for (ai_piSubKey=m_piIndexKey; ai_iLevel>0; --ai_iLevel, ++ai_piSubKey ) {
			if (*ai_piSubKey == TOTALIZE_LEVEL) {
				R=M4_TRUE ;
				break ;
			} ;
		} ;

	} ;
	return R ;
}


void ClENG_KeyTot::Trace() 
{
	#ifdef RPT_ENGINE_TRACE
	
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){

		m4int16_t B ;

		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "ClENG_KeyTot::Trace()", ClMIT_Msg::ENTRY_FORMAT ) << SEND_MSG ;
	
		TraceThis(&m_RootLevel) ;

		if ( m_poDerived ) {
			for (B=0;B<m_oDerivedIndex.GetCount();++B) {
				*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "Derived ClENG_KeyTot %0:s") 
					<< m_oDerivedIndex.GetKey(B) << SEND_MSG ;
				m_poInsp->m_poTrace->Indent() ;
				m_poDerived[B].Trace() ;
				m_poInsp->m_poTrace->Unindent() ;
			} ;
		} ;

		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "ClENG_KeyTot::Trace()", ClMIT_Msg::EXIT_FORMAT ) << SEND_MSG ;
	};

	#endif
}

void ClENG_KeyTot::TraceThis(KeyTotList_t * ao_poList) 
{
	#ifdef RPT_ENGINE_TRACE
	
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		
		KeyTotList_t::iterator It ;
		ClMIT_Str Str, Str2 ;

		if (!ao_poList) return ;
		for (It=ao_poList->begin();It!=ao_poList->end();++It) {
			(*It).m_oKey.GetInputFormat(&Str) ;
			(*It).m_oHeader.GetInputFormat(&Str2) ;

			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "%0:s Header %1:s") 
				<< Str.InChar(0) << Str2.InChar(0) << SEND_MSG ;
			m_poInsp->m_poTrace->Indent() ;
			TraceThis(&(*It).m_poSon->m_oSon) ;
			m_poInsp->m_poTrace->Unindent() ;
		} ;
	};

	#endif
}


m4bool_t ClENG_KeyTot::GetPartialInfoByIndex( 
	m4int16_t ai_iLevel,
	m4int16_t ai_iTreeIndex, m4int16_t ai_iSubIndex, 
	ClENG_Tot * ai_poTot ) 
{
	m4int16_t TopLevel ;
	m4bool_t SwOk=M4_TRUE ;

	TopLevel=GetConsLevelByIndex(ai_iLevel) ;
	if ( TopLevel==0 ) TopLevel=1 ;

	ai_poTot->Reset() ;
	
	SwOk = M4_BOOL(SwOk && GetPartialInfo(&m_RootLevel, m_piIndexKey, TopLevel, 
				   ai_iTreeIndex, ai_iSubIndex, ai_poTot )); 
	
	if (!SwOk) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ TOTALIZE_MSG_70 ] ) << SEND_MSG ;
		//Error local
	} ;

	return SwOk ;
} 

m4bool_t ClENG_KeyTot::GetPartialInfo( 
		KeyTotList_t * ao_poList, m4int16_t *ai_piSubKey, m4int16_t ai_iLevel,
		m4int16_t ai_iTreeIndex, m4int16_t ai_iSubIndex, 
		ClENG_Tot * ai_poTot ) 
{
	m4bool_t SwOk=M4_TRUE ;
	KeyTotList_t::iterator It ;
	m4int16_t Index ;

	if (ai_iLevel==1) {
		Index=TOTALIZE_INDEX(ai_iTreeIndex,ai_iSubIndex) ;
		if (*ai_piSubKey!=TOTALIZE_LEVEL) {
			SwOk = M4_BOOL(SwOk && GetItFromIndex(ao_poList,*ai_piSubKey,It));
			if (SwOk) {
				ai_poTot->Entry( &(*It).m_poTot[Index] ) ;
			} ;
		} else {
			for (It=ao_poList->begin();It!=ao_poList->end();++It) {
				ai_poTot->Entry( &(*It).m_poTot[Index] ) ;
			} ;
		} ;
	} else {
		if (*ai_piSubKey!=TOTALIZE_LEVEL) {
			SwOk = M4_BOOL(SwOk && GetItFromIndex(ao_poList,*ai_piSubKey,It));
			if (SwOk) {
				SwOk=M4_BOOL(SwOk && GetPartialInfo( &(*It).m_poSon->m_oSon, ai_piSubKey+1, ai_iLevel-1,
							 ai_iTreeIndex, ai_iSubIndex, ai_poTot));
			} ;
		} else {
			for (It=ao_poList->begin();It!=ao_poList->end();++It) {
				SwOk=M4_BOOL(SwOk && GetPartialInfo( &(*It).m_poSon->m_oSon, ai_piSubKey+1, ai_iLevel-1,
							 ai_iTreeIndex, ai_iSubIndex, ai_poTot));
			} ;
		} ;
	} ;

	return SwOk ;
}


m4int16_t ClENG_KeyTot::GetIndexKey(m4int16_t ai_iIndex) 
{
	if (!M4_IN_RANGE(ai_iIndex, 0, GetLevelCount()-1)  ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ TOTALIZE_MSG_80 ] ) << SEND_MSG ;
		//Error local
		return 0 ;
	} else return m_piIndexKey[ai_iIndex] ;
}

void ClENG_KeyTot::SetIndexKey(m4int16_t ai_iIndex, m4int16_t ai_iValue) 
{
	if (!M4_IN_RANGE(ai_iIndex, 0, GetLevelCount()-1) ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ TOTALIZE_MSG_80 ] ) << SEND_MSG ;
		//Error local
	} else {
		m_piIndexKey[ai_iIndex]=ai_iValue ;
	} ;
}

ClMIT_ChnValue * ClENG_KeyTot::GetValueKey(m4int16_t ai_iIndex) 
{
	if (!M4_IN_RANGE(ai_iIndex, 0, GetLevelCount()-1)  ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ TOTALIZE_MSG_80 ] ) << SEND_MSG ;
		//Error local
		return NULL ;
	} else return &m_poValueKey[ai_iIndex] ;
}

void ClENG_KeyTot::SetValueKey(m4int16_t ai_iIndex, ClMIT_ChnValue * ai_iValue) 
{
	if (!M4_IN_RANGE(ai_iIndex, 0, GetLevelCount()-1) ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ TOTALIZE_MSG_80 ] ) << SEND_MSG ;
		//Error local
	} else {
		m_poValueKey[ai_iIndex].Init(ai_iValue) ;
	} ;
}

m4int16_t ClENG_KeyTot::LevelFatherToDerived(m4int16_t ai_iFatherLevel) 
{
	m4int16_t R ;
	m4int16_t B, *Pos ;

	if (!m_piDerivedLevel) {
		R=-1 ;
	} else {
		for ( R=-1, B=0, Pos=m_piDerivedLevel; B<m_iLevelCount; ++B, ++Pos ) {
			if ( *m_piDerivedLevel == ai_iFatherLevel ) {
				R=B+1 ;
				break ;
			} ;
		} ;
	} ;
	return R ;
}

m4int16_t ClENG_KeyTot::DerivedLevelToFather(m4int16_t ai_iDerivedLevel) 
{
	m4int16_t R ;

	if (!m_piDerivedLevel || !M4_IN_RANGE(ai_iDerivedLevel,1,m_iLevelCount) ) {
		R=-1 ;
	} else {
		R=m_piDerivedLevel[ai_iDerivedLevel-1] ;
	} ;
	return R ;
}

void ClENG_KeyTot::Sort( KeyTotList_t * ao_poList, ClENG_KeyTotNode::SORT_BY ai_Sort,
	m4int16_t ai_CountLevel, m4bool_t ai_bOnLastLevel,  
	KeyList_t *ao_KeyList) 
{
	KeyTotList_t::iterator It ;
	m4bool_t SortThis ;

	if (!ao_poList || ai_CountLevel==0 ) return ;

	if ( !ai_bOnLastLevel ) {
		SortThis=M4_TRUE ;
	} else {
		if (ai_CountLevel<0)  {
			SortThis = M4_BOOL(ao_poList->size()==0); 
		} else {
			SortThis = M4_BOOL(ai_CountLevel==1); 
		} ;
	} ;
	for (It=ao_poList->begin();It!=ao_poList->end();++It) {
		if ( ai_CountLevel>1 || ai_CountLevel<0 ) {	//Optimiza, evitando entradas superfluas 
			Sort( &(*It).m_poSon->m_oSon, ai_Sort, 
				ai_CountLevel<0 ? ai_CountLevel : ai_CountLevel-1, 
				ai_bOnLastLevel, ao_KeyList ) ;
		} ;
		if ( SortThis ) (*It).m_SortBy=ai_Sort ;
	} ;
	if (SortThis) {
		ao_poList->sort() ;
		if (ao_KeyList) {
			ao_KeyList->clear() ;
			for (It=ao_poList->begin();It!=ao_poList->end();++It) {
				ao_KeyList->push_back( (*It).m_oKey ) ;
			} ;
		} ;
	} ;
}

m4bool_t ClENG_KeyTot::SortKey( ClENG_KeyTotNode::SORT_BY ai_Sort, m4int16_t ai_Level, KeyList_t *ao_KeyList) 
{
	m4bool_t R ;
	KeyTotList_t *FoundList ;
	KeyTotList_t::iterator FoundIt ;

	if (ai_Level==0) {
		Sort( &m_RootLevel, ai_Sort, 1, M4_TRUE, ao_KeyList) ;
		R=M4_TRUE ;
	} else {
		R=Find( FIND_REQ, FoundList, FoundIt, m_poValueKey, ai_Level, 0,0 ) ;
		if (R) {
			Sort( FoundList, ai_Sort, 1, M4_TRUE, ao_KeyList) ;
		} ;
	} ;
	return R ;
}

void ClENG_KeyTot::Synchronize( ClENG_KeyTot *ao_poDerived, SYNC_ACTION ai_Action, m4int16_t ai_iMaxLevelDerived) 
{
	m4int16_t MaxLevel ;
	m4int16_t DIndex, FIndex, Aux ;

	//Monta clave de busqueda en padre, hayando MaxLevel
	for (FIndex=0;FIndex<m_iLevelCount; ++FIndex) m_poValueKey[FIndex].End() ;
	for (DIndex=0, MaxLevel=0 ;DIndex<ai_iMaxLevelDerived; ++DIndex) {
		Aux=ao_poDerived->DerivedLevelToFather(DIndex+1) ; 
		if (!(Aux<0))  {
			m_poValueKey[Aux-1].Init(&ao_poDerived->m_poValueKey[DIndex]) ;
			MaxLevel=M4_MAX(MaxLevel,Aux) ;
		} ;
	} ;
	SynchronizeAux(&m_RootLevel, 1, MaxLevel, ai_iMaxLevelDerived, ao_poDerived, ai_Action ) ;
}

void ClENG_KeyTot::SynchronizeAux( 
	KeyTotList_t * ao_poList, 
	m4int16_t ai_iActualLevel, m4int16_t ai_iMaxLevel,  m4int16_t ai_iMaxLevelDerived,
	ClENG_KeyTot *ao_poDerived, SYNC_ACTION ai_Action ) 
{
	ClMIT_ChnValue *LookForValue, *DerivedValue ;
	m4int16_t Index ;
	m4bool_t R ;
	KeyTotList_t::iterator FoundIt ;
	
	LookForValue=&m_poValueKey[ai_iActualLevel-1] ;
	//Si el nivel actual tiene correspondencia con el derivado, se obtiene el puntero para ir
	//actualizando la clave.
	Index=ao_poDerived->LevelFatherToDerived(ai_iActualLevel) ;
	DerivedValue= Index<0 ? NULL: &ao_poDerived->m_poValueKey[Index-1] ;

	if ( LookForValue->GetType()==ClMIT_ChnValue::NO_INIT  ) {

		//Se deben recorrer todos los valores de este nivel.

		for (FoundIt=ao_poList->begin();FoundIt!=ao_poList->end();++FoundIt) {
			if (DerivedValue) DerivedValue->Init(LookForValue) ;
			if ( ai_iActualLevel==ai_iMaxLevel ) {
				//Ya se llego al nivel tope, hay que procesar llamada y cortar el recorrido.
				//Procesar llamada !!!!
			} else {
				//Aun no estamos en el nivel tope, hay que seguir buscando en hijos.
				SynchronizeAux( &(*FoundIt).m_poSon->m_oSon,ai_iActualLevel+1, ai_iMaxLevel, 
					ai_iMaxLevelDerived, ao_poDerived,ai_Action);
			} ;
		} ;

	} else {

		GetItFromKey(ao_poList, LookForValue, FoundIt)  ;
		//Si no se encuentra el valor se corta el recorrido. 
		if ( FoundIt!=ao_poList->end() ) {
			if (DerivedValue) DerivedValue->Init(LookForValue) ;
			if ( ai_iActualLevel==ai_iMaxLevel ) {
				//Ya se llego al nivel tope, hay que procesar llamada y cortar el recorrido.
				//Procesar llamada !!!!
			} else {
				//Aun no estamos en el nivel tope, hay que seguir buscando en hijos.
				SynchronizeAux( &(*FoundIt).m_poSon->m_oSon,ai_iActualLevel+1, ai_iMaxLevel, 
					ai_iMaxLevelDerived, ao_poDerived,ai_Action);
			} ;
			
		} ;
	} ;
}


void ClENG_KeyTot::DoDerivedAction(
	m4int16_t ai_iMaxLevelDerived, SYNC_ACTION ai_Action, ClENG_KeyTotNode *ai_poSource) 
{
	m4bool_t R ;
	KeyTotList_t *FoundList ;
	KeyTotList_t::iterator FoundIt ;
	m4int16_t Tree, SubTot ;

	//Al llegar aqui, las claves en el arbol derivado ya deben de estar actualizadas.

	switch (ai_Action) {

	case DERIVED_INIT:
		
		//Solo crea si clave no existe
		if ( ! Find( FIND_TEST, FoundList, FoundIt, m_poValueKey, ai_iMaxLevelDerived, 0,0 ) ) {
			NewKey(ai_iMaxLevelDerived,&ai_poSource->m_oHeader) ;
		} ;

		break;

	case DERIVED_ENTRY:

		for (Tree=0;Tree<GetTreeCount();++Tree) {
			for (SubTot=0;SubTot<GetTotCount();++SubTot) {
				TotEntryByValue( ai_iMaxLevelDerived, Tree, SubTot, NULL, 
					&ai_poSource->m_poTot[TOTALIZE_INDEX(Tree,SubTot)] ) ;
			} ;
		} ;
		

		break;
	} ;
}

void ClENG_KeyTot::InitDerivedNode(KeyTotList_t * ao_poList,m4int16_t ai_iActualLevel) 
{
	KeyTotList_t::iterator It ;

	//Valor derivado actual a no Init, provoca que se recorran y llenen todos los valores.

	m_poValueKey[ai_iActualLevel-1].End() ;	
	m_poMaster->Synchronize(this,DERIVED_INIT,ai_iActualLevel) ;
	
	//Ahora, a por los hijos, si debe de tenerlos

	if (ai_iActualLevel<m_iLevelCount) {

		for (It=ao_poList->begin();It!=ao_poList->end();++It) {

			//En cada pasada se fija un valor clave fijo para Synchronize

			m_poValueKey[ai_iActualLevel-1].Init( &(*It).m_oKey ) ;	
			m_poMaster->InitDerivedNode( &(*It).m_poSon->m_oSon, ai_iActualLevel+1 ) ;
		} ;
	} ;
}


void ClENG_KeyTot::EntryDerivedNode(KeyTotList_t * ao_poList,m4int16_t ai_iActualLevel) 
{
	KeyTotList_t::iterator It ;


	for (It=ao_poList->begin();It!=ao_poList->end();++It) {

		//En cada pasada se fija un valor clave fijo para Synchronize

		m_poValueKey[ai_iActualLevel-1].Init( &(*It).m_oKey ) ;	

		if (ai_iActualLevel<m_iLevelCount) {

			//No es ultimo nivel, llama a hijos sin llamar a Synchronize.

			EntryDerivedNode( &(*It).m_poSon->m_oSon, ai_iActualLevel+1 ) ;

		} else {

			//Ultimo nivel, llamada a Synchronize.

			m_poMaster->Synchronize(this,DERIVED_ENTRY,ai_iActualLevel) ;
		} ;

	} ;
}

