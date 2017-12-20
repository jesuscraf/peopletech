//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             memprof.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:11-Sep-97
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

#include <stdio.h>

#include "mit.h"

#include "clstr.h"

#include "mitdefs.cpp"

//#include "trcgroup.h"
#include "memprof.hmg"
#include "memprof.h"

ClMIT_MemInfo::ClMIT_MemInfo() 
{
	ClMIT_Str::SafeStrCpy( m_pcNameGroup, SIZE_STR_GROUP, "NAME EMPTY" ) ;
		
	m_iNews=m_iDeletes=m_iNullDeletes=m_iItemsAllocated=m_iItemsDeleted=m_iMemAllocated=
		m_iMemDeleted=m_iMaxItemsMem=m_iMaxSizeof=m_iMaxMem=m_iMaxAlloc=m_iMaxItemAlloc=0 ;
}


ClMIT_MemProf::ClMIT_MemProf() 
{
	m_poInsp=GET_INSP();
	m_poInfo=NULL ;
	m_iUnInitNews = m_iUnInitDeletes = 0 ;
	m_bCheckPointAtEnd=M4_FALSE ;
}

ClMIT_MemProf::ClMIT_MemProf( 
		m4int16_t ai_iNItems, m4int16_t ai_iNSharedItems, 
		const m4char_t *const ai_pcArrNameA[], 
		const m4char_t *const ai_pcArrNameB[]) 
{
	m_poInsp=GET_INSP();
	m_poInfo=NULL ;
	ClMIT_MemProf::Init(ai_iNItems,ai_iNSharedItems,ai_pcArrNameA,ai_pcArrNameB) ;
}

void ClMIT_MemProf::End() 
{
	m4bool_t Error=M4_FALSE ;

	#ifdef MIT_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		if ( m_iUnInitNews!=0 || m_iUnInitDeletes!=0 ){
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MEMPROF_MSG_100 ] ) 
				<< m_iUnInitNews << m_iUnInitDeletes << SEND_MSG ;
		} ;
	};
	#endif

	if (m_poInfo) {

		#ifdef MIT_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			
			m4int16_t B ;

			CalculateTotal() ;

			for (B=0; B<m_iNItems; ++B ) {
				if ( m_poInfo[B].m_iNews != m_poInfo[B].m_iDeletes) {
					*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MEMPROF_MSG_50 ] ) 
						<< m_poInfo[B].m_pcNameGroup << m_poInfo[B].m_iNews 
						<< m_poInfo[B].m_iDeletes << SEND_MSG ;
					Error=M4_TRUE ;
				} ;
				if ( m_poInfo[B].m_iItemsAllocated != m_poInfo[B].m_iItemsDeleted ) {
					*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MEMPROF_MSG_70 ] ) 
						<< m_poInfo[B].m_pcNameGroup << m_poInfo[B].m_iItemsAllocated 
						<< m_poInfo[B].m_iItemsDeleted << SEND_MSG ;
					Error=M4_TRUE ;
				} ;
				if ( m_poInfo[B].m_iMemAllocated != m_poInfo[B].m_iMemDeleted ) {
					*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MEMPROF_MSG_90 ] ) 
						<< m_poInfo[B].m_pcNameGroup << m_poInfo[B].m_iMemAllocated 
						<< m_poInfo[B].m_iMemDeleted << SEND_MSG ;
					Error=M4_TRUE ;
				} ;
			} ;

			if (m_bCheckPointAtEnd||Error) CheckPoint("ClMIT_MemProf::End()") ;
		};
		#endif
		
		delete [] m_poInfo ;
		m_poInfo=NULL ;

		#ifdef MIT_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			if (!Error)	{
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MEMPROF_MSG_120 ] ) << SEND_MSG ;
			} ;
		};
		#endif
	} ;
	
	if (Error) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MEMPROF_MSG_110 ] ) << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;

	m_bCheckPointAtEnd=M4_FALSE ;
}


void ClMIT_MemProf::Init( 
	m4int16_t ai_iNItems, m4int16_t ai_iNSharedItems, 
	const m4char_t *const ai_pcArrNameA[], 
	const m4char_t *const ai_pcArrNameB[]) 
{
	m4int16_t B,Quedan;

	ClMIT_MemProf::Init(ai_iNItems) ;

	if (ai_iNSharedItems) {
		for ( B=0; B<ai_iNSharedItems; ++B ) ResetName( B, ai_pcArrNameA[B] )  ;
		Quedan=ai_iNItems-ai_iNSharedItems ;
	} ;
	if ( Quedan ) {
		for ( B=0; B<Quedan; ++B ) ResetName( ai_iNSharedItems+B, ai_pcArrNameB[B] )  ;
	} ;
}

void ClMIT_MemProf::Init( m4int16_t ai_iNItems ) 
{
	ClMIT_MemProf::End() ;

	++ai_iNItems ;

	m_iNItems = ai_iNItems ;
	m_poInfo = new ClMIT_MemInfo[m_iNItems] ;

	m_poInfo[m_iNItems-1].m_iMaxItemsMem=m_poInfo[m_iNItems-1].m_iMaxMem=0 ;

	ClMIT_Str::SafeStrCpy(m_poInfo[ai_iNItems-1].m_pcNameGroup, ClMIT_MemInfo::SIZE_STR_GROUP, "****** TOTAL ******") ;
}



void ClMIT_MemProf::ResetName( m4int16_t ai_iIndexGroup, const m4char_t *ai_pcName ) 
{
	if ( !M4_IN_RANGE(ai_iIndexGroup,0,m_iNItems-2) ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MEMPROF_MSG_10 ] ) << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
	ClMIT_Str::SafeStrCpy(m_poInfo[ai_iIndexGroup].m_pcNameGroup, ClMIT_MemInfo::SIZE_STR_GROUP, ai_pcName) ;
}


void ClMIT_MemProf::ProcessNew(  m4int16_t ai_iIndexGroup, void * ai_pvPtr,
							m4int32_t ai_iNItems, m4int32_t ai_iSizeof ) 
{
	m4int32_t LocalMemAlloc=ai_iNItems*ai_iSizeof ;
	
	if (!m_poInfo) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MEMPROF_MSG_30 ] ) << SEND_MSG ;
		++m_iUnInitNews ;
		return ;
	} ;
	
	if ( !M4_IN_RANGE(ai_iIndexGroup,0,m_iNItems-2) ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MEMPROF_MSG_10 ] ) << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;

	if ( !ai_pvPtr ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MEMPROF_MSG_20 ] ) << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;

	++m_poInfo[ai_iIndexGroup].m_iNews ;
	m_poInfo[ai_iIndexGroup].m_iItemsAllocated += ai_iNItems ;
	m_poInfo[ai_iIndexGroup].m_iMemAllocated   += LocalMemAlloc;

	m_poInfo[ai_iIndexGroup].m_iMaxMem= 
		M4_MAX( m_poInfo[ai_iIndexGroup].m_iMaxMem, m_poInfo[ai_iIndexGroup].GetSpendMem() ) ; 
	m_poInfo[ai_iIndexGroup].m_iMaxItemsMem= 
		M4_MAX( m_poInfo[ai_iIndexGroup].m_iMaxItemsMem, m_poInfo[ai_iIndexGroup].GetSpendItem() ); 
	m_poInfo[ai_iIndexGroup].m_iMaxSizeof= 
		M4_MAX( m_poInfo[ai_iIndexGroup].m_iMaxSizeof, ai_iSizeof); 
	if ( LocalMemAlloc > m_poInfo[ai_iIndexGroup].m_iMaxAlloc ) {
		m_poInfo[ai_iIndexGroup].m_iMaxAlloc = LocalMemAlloc ;
		m_poInfo[ai_iIndexGroup].m_iMaxItemAlloc=ai_iNItems ;
	} ;
	CalculateTotal() ;
}

m4bool_t ClMIT_MemProf::ProcessDelete( m4int16_t ai_iIndexGroup, void * ai_pvPtr, 
								  m4int32_t ai_iNItems, m4int32_t ai_iSizeof ) 
{
	m4bool_t R ;
	
	if (!m_poInfo) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MEMPROF_MSG_30 ] ) << SEND_MSG ;
		++m_iUnInitDeletes ;
		return M4_FALSE ;
	} ;

	if ( !M4_IN_RANGE(ai_iIndexGroup,0,m_iNItems-1) ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MEMPROF_MSG_10 ] ) << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;

	if (ai_pvPtr) {

		++ m_poInfo[ai_iIndexGroup].m_iDeletes ;		
		m_poInfo[ai_iIndexGroup].m_iItemsDeleted += ai_iNItems ;
		m_poInfo[ai_iIndexGroup].m_iMemDeleted   += ai_iNItems * ai_iSizeof ;

		R=M4_TRUE ;

	} else {
		
		++ m_poInfo[ai_iIndexGroup].m_iNullDeletes ;		

		R=M4_FALSE ;
	} ;

	CalculateTotal() ;

	return R ;
}

void ClMIT_MemProf::CalculateTotal() 
{
	m4int16_t B ;

	m_poInfo[m_iNItems-1].m_iNews=m_poInfo[m_iNItems-1].m_iDeletes=
	m_poInfo[m_iNItems-1].m_iNullDeletes=m_poInfo[m_iNItems-1].m_iItemsAllocated=
	m_poInfo[m_iNItems-1].m_iItemsDeleted=m_poInfo[m_iNItems-1].m_iMemAllocated=
	m_poInfo[m_iNItems-1].m_iMemDeleted=
	m_poInfo[m_iNItems-1].m_iMaxAlloc=m_poInfo[m_iNItems-1].m_iMaxItemAlloc=
	m_poInfo[m_iNItems-1].m_iMaxSizeof=0 ;
	for (B=0; B<m_iNItems-1; ++B ) {
		m_poInfo[m_iNItems-1].m_iNews			+=	m_poInfo[B].m_iNews ;	
		m_poInfo[m_iNItems-1].m_iDeletes		+=	m_poInfo[B].m_iDeletes ;
		m_poInfo[m_iNItems-1].m_iNullDeletes	+=	m_poInfo[B].m_iNullDeletes ;
		m_poInfo[m_iNItems-1].m_iItemsAllocated	+=	m_poInfo[B].m_iItemsAllocated ;
		m_poInfo[m_iNItems-1].m_iItemsDeleted	+=	m_poInfo[B].m_iItemsDeleted ;
		m_poInfo[m_iNItems-1].m_iMemAllocated	+=	m_poInfo[B].m_iMemAllocated ;
		m_poInfo[m_iNItems-1].m_iMemDeleted		+=	m_poInfo[B].m_iMemDeleted ;	
		m_poInfo[m_iNItems-1].m_iMaxSizeof		=	M4_MAX(
			m_poInfo[m_iNItems-1].m_iMaxSizeof, m_poInfo[B].m_iMaxSizeof);
		if ( m_poInfo[B].m_iMaxAlloc > m_poInfo[m_iNItems-1].m_iMaxAlloc ) {
			m_poInfo[m_iNItems-1].m_iMaxAlloc = m_poInfo[B].m_iMaxAlloc ;
			m_poInfo[m_iNItems-1].m_iMaxItemAlloc=m_poInfo[B].m_iMaxItemAlloc ;
		} ;		
	} ;
	m_poInfo[m_iNItems-1].m_iMaxMem		= 	
		M4_MAX(	m_poInfo[m_iNItems-1].m_iMaxMem, m_poInfo[m_iNItems-1].GetSpendMem() )  ;
	m_poInfo[m_iNItems-1].m_iMaxItemsMem = 	
		M4_MAX(	m_poInfo[m_iNItems-1].m_iMaxItemsMem, m_poInfo[m_iNItems-1].GetSpendItem() )  ;

}

void ClMIT_MemProf::CheckPoint( const m4char_t * ai_pcStr ) 
{
	if (!m_poInfo) return ;

	#ifdef MIT_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		
		m4int16_t B ;

		CalculateTotal() ;

		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "Memory CheckPoint in %0:s") 
			<<  ai_pcStr << SEND_MSG ;
		m_poInsp->m_poTrace->Indent(1) ;
		
		for (B=0; B<m_iNItems; ++B ) {

			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, 
				"Group %0:s : Undeleted news = %1:s (%2:s-%3:s) "
				"Mem = %4:s (%5:s-%6:s), Items %7:s (%8:s-%9:s). "
				"MaxMem=%10:s MaxItemsMem=%11:s MaxSizeof=%12:s MaxAlloc=%13:s "
				"MaxItemAlloc=%14:s NullDeletes=%15:s", ClMIT_Msg::NO_WRAP ) 

				<< m_poInfo[B].m_pcNameGroup

				<< m_poInfo[B].m_iNews -m_poInfo[B].m_iDeletes 
				<< m_poInfo[B].m_iNews << m_poInfo[B].m_iDeletes

				<< m_poInfo[B].m_iMemAllocated -m_poInfo[B].m_iMemDeleted 
				<< m_poInfo[B].m_iMemAllocated << m_poInfo[B].m_iMemDeleted

				<< m_poInfo[B].m_iItemsAllocated -m_poInfo[B].m_iItemsDeleted 
				<< m_poInfo[B].m_iItemsAllocated << m_poInfo[B].m_iItemsDeleted
				<< m_poInfo[B].m_iMaxMem << m_poInfo[B].m_iMaxItemsMem
				<< m_poInfo[B].m_iMaxSizeof << m_poInfo[B].m_iMaxAlloc
				<< m_poInfo[B].m_iMaxItemAlloc << m_poInfo[B].m_iNullDeletes

				<<  SEND_MSG ;

		} ;

		m_poInsp->m_poTrace->Unindent(1) ;
	};
	#endif
		
}
