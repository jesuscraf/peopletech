//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             m4clbuf.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/18/97
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

#include "m4stl.hpp"
#include "m4string.hpp"

#include "eng.h"
#include "m4clbuf.h"
#include "engdf.inl"

#include "m4clbuf.hmg"

#define PTR_NEXT_BLOCK(P) ( (m4int8_t **)( P+m_szBlock ) )

M4ClBuffer::M4ClBuffer()
{
	m_poInsp=GET_INSP();
	m_uiNBlock=m_szBlock=m_szFill=0;
	m_piFirstBlock=m_piLastBlock=NULL;
}


void M4ClBuffer::Init(size_t ai_szInterBlock) 
{
	M4ClBuffer::End();
	if (ai_szInterBlock <= 0 ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ M4CLBUF_MSG_10 ] ) << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
	m_szBlock=ai_szInterBlock;
}

void M4ClBuffer::InitPreserve(m4uint32_t ai_uiPreserve) 
{
	m4int8_t * Block=m_piFirstBlock, * BlockAux ;
	m4int16_t ContaBlock=1 ;

	if ( ai_uiPreserve > GetTotal() ) ai_uiPreserve=GetTotal();
	if (ai_uiPreserve==0 || IsEmpty() ) {
		Init();
		return ;
	} ;

	m_uiNBlock=(m4uint16_t)((ai_uiPreserve-1)/m_szBlock)+1;
	m_szFill=ai_uiPreserve%m_szBlock; if (!m_szFill) m_szFill=m_szBlock;
	while (Block) {
		BlockAux=*PTR_NEXT_BLOCK(Block) ;
		if (ContaBlock>m_uiNBlock) {
			M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLBUFFER, 
				Block, m4int8_t, m_szBlock+sizeof(void *)
			) ;
		} else if (ContaBlock==m_uiNBlock) {
			m_piLastBlock=Block;
		} ;
		Block=BlockAux;
		++ContaBlock ;
	} ;
	*PTR_NEXT_BLOCK(m_piLastBlock)=NULL ;
}


void M4ClBuffer::End()
{
	m4int8_t * Block=m_piFirstBlock, * BlockAux ;
	
	while (Block) {
		BlockAux=*PTR_NEXT_BLOCK(Block) ;
		M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLBUFFER, 
			Block, m4int8_t, m_szBlock+sizeof(void *)
		) ;
		Block=BlockAux;
	} ;
	m_uiNBlock=m_szFill=m_szBlock=0;
	m_piFirstBlock=m_piLastBlock=NULL;
}

m4uint32_t M4ClBuffer::GetTotal() const  
{
	return IsEmpty() ? 0 : (m_uiNBlock-1)*m_szBlock+m_szFill ;
}

void M4ClBuffer::AppendStr(  const m4char_t * ai_cStr ) {
	Append(ai_cStr,strlen(ai_cStr));	
}

void M4ClBuffer::AppendStrNULL(  const m4char_t * ai_cStr ) {
	Append(ai_cStr,strlen(ai_cStr)+1);	
}

void M4ClBuffer::Append( const void *ai_DataPar, size_t ai_szNBytes ) {
	size_t Meter ;
	m4int8_t *BlockAux;
	m4int8_t * ai_Data = (m4int8_t *)ai_DataPar ;

	if (m_uiNBlock==0) {
		M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLBUFFER, 
			m_piLastBlock, m4int8_t, m_szBlock+sizeof(void *)
		) ;
		m_piFirstBlock=m_piLastBlock;
		m_szFill=0;
		m_uiNBlock=1;
		*PTR_NEXT_BLOCK(m_piLastBlock) = NULL ;
	}
	while (ai_szNBytes!=0) {
		Meter=M4_MIN(ai_szNBytes,(m_szBlock-m_szFill));
		if (Meter>0) {
			memcpy( m_piLastBlock+m_szFill,ai_Data,Meter);
			ai_szNBytes -= Meter ;
			ai_Data  += Meter ;
			m_szFill += Meter ;
		} ;
		if (ai_szNBytes!=0) {
			BlockAux=m_piLastBlock ;
			M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLBUFFER, 
				m_piLastBlock, m4int8_t, m_szBlock+sizeof(void *)
			) ;
			m_szFill=0;
			++m_uiNBlock;
			*PTR_NEXT_BLOCK(m_piLastBlock) = NULL ;
			*PTR_NEXT_BLOCK(BlockAux) = m_piLastBlock ;
		} ;
	} ;
}

void M4ClBuffer::AppendFile( ClMIT_File *ai_poF, m4uint32_t ai_From, m4uint32_t ai_Count ) 
{
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	size_t Read, ToWrite;
	size_t  Written ;	//Info debug

	ai_poF->SetPos(ai_From);  
	
	
	Written=0 ;
	while ( !ai_poF->IsEOF() ) {
		Read=ai_poF->R( TaskEnv->M4ClBuffer_g_pcRBuff, StENG_TaskEnv::M4ClBuffer_READ_SIZE ) ;
		if ( ai_Count==READ_UNTIL_EOF) {
			Append(TaskEnv->M4ClBuffer_g_pcRBuff,Read) ;
			Written += Read ;
		} else {
			ToWrite = M4_MIN(Read,ai_Count) ;
			Append(TaskEnv->M4ClBuffer_g_pcRBuff,ToWrite) ;
			ai_Count -=ToWrite ;
			Written += ToWrite ;
			if (ai_Count==0) break ;
		} ;
		
	} ;
}


void M4ClBuffer::Flush( FlushFunc_t * Process, void *ai_pvParam ) {
	m4int8_t * Block=m_piFirstBlock; 
	m4uint16_t NBlock=1 ;

	while (Block) {
		Process( Block, NBlock!=m_uiNBlock ? m_szBlock:m_szFill, ai_pvParam );
		Block=*PTR_NEXT_BLOCK(Block) ;
		++NBlock;
	} ;
}

void M4ClBuffer::Flush( FlushFunc_t *Process, void *ai_pvParam, 
		   m4uint32_t ai_uiFrom, m4uint32_t ai_uiNBytes) 
{
	m4int8_t * Block=m_piFirstBlock; 
	m4uint16_t NBlock=1, BlockBegin=(m4uint16_t)(ai_uiFrom/m_szBlock)+1 ;
	m4uint32_t Aux, Aux2 ;

	if (ai_uiNBytes==0) goto exit ;
	
	if ( !M4_IN_RANGE(ai_uiFrom,0,GetTotal()-1) ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ M4CLBUF_MSG_20 ] ) << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;

	while (Block && ai_uiNBytes>0) {
		if (NBlock>BlockBegin) {
			Aux2=M4_MIN(m_szBlock,ai_uiNBytes);
			if (NBlock==m_uiNBlock) Aux2=M4_MIN(Aux2,m_szFill);
			Process( Block, Aux2, ai_pvParam );
			ai_uiNBytes -= Aux2 ;			
		} else if (NBlock==BlockBegin) {
			Aux=(ai_uiFrom+1)%m_szBlock; if (!Aux) Aux=m_szBlock;
			Aux2=m_szBlock-Aux+1;
			Aux2=M4_MIN(Aux2,ai_uiNBytes);
			if (NBlock==m_uiNBlock) Aux2=M4_MIN(Aux2,m_szFill);
			Process( Block+Aux-1, Aux2, ai_pvParam );
			ai_uiNBytes -= Aux2 ;			
		} ;
		Block=*PTR_NEXT_BLOCK(Block) ;
		++NBlock;
	} ;
exit:
	return ;
}

void M4ClBuffer::WFileManip(m4int8_t *ai_Data, size_t ai_szNBytes, void *ai_pvParam) {
	((ClMIT_File *)ai_pvParam)->W( ai_Data, sizeof(m4int8_t)*ai_szNBytes  );
}


void M4ClBuffer::MemToBuffManip(m4int8_t *ao_Data, size_t ai_szNBytes, void *ao_pvParam) {
	StCopy * Copy = (StCopy *)ao_pvParam  ;
	m4int8_t * Source = (m4int8_t *)Copy->Data ;
	size_t Aux ;

	if ( ( Aux=M4_MIN(Copy->Count,ai_szNBytes) ) > 0  ) {
		memcpy(ao_Data, Source,Aux) ;
		Source += Aux ;
		Copy->Data = Source ;
		Copy->Count -= Aux ;
	} ;
} 

void M4ClBuffer::BuffToMemManip(m4int8_t *ao_Data, size_t ai_szNBytes, void *ao_pvParam) {
	StCopy * Copy = (StCopy *)ao_pvParam  ;
	m4int8_t * Target = (m4int8_t *)Copy->Data ;
	size_t Aux ;

	if ( ( Aux=M4_MIN(Copy->Count,ai_szNBytes) ) > 0  ) {
		memcpy(Target, ao_Data,Aux) ;
		Target += Aux ;
		Copy->Data = Target ;
		Copy->Count -= Aux ;
	} ;
} 

void M4ClBuffer::ToM4ClBuffManip(m4int8_t *ao_Data, size_t ai_szNBytes, void *ao_pvParam) 
{
	((M4ClBuffer *)ao_pvParam)->Append( ao_Data, ai_szNBytes  );
}


