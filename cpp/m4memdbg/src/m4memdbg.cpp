//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             m4memdbg.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/23/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//
//==============================================================================

#include <stdlib.h>
#include <stdio.h>

#include "m4define.hpp"
#include "m4types.hpp"

#include "m4memdbg.h"
#include "m4srvtrace.h"
#include "m4unicode.hpp"



ClM4_MemDbg g_oM4MemDbg ; 

m4bool_t g_bM4SwMemDbgOn=M4_FALSE ; 


#define M4_SRV_DEBUG_X_LEVEL_MEM 100

StM4_MemDbgInfo::StM4_MemDbgInfo() 
{
	m_pcCompId[0] = 0 ;
	m_pcSrcId[0] = 0 ;
	m_uiLocalId = 0 ;
}

StM4_MemDbgInfo::StM4_MemDbgInfo(const StM4_MemDbgInfo& ai_oSource) 
{
	strcpy( m_pcCompId, ai_oSource.m_pcCompId ) ;
	strcpy( m_pcSrcId, ai_oSource.m_pcSrcId  ) ;
	m_uiLocalId		= ai_oSource.m_uiLocalId ;
}

StM4_MemDbgInfo & StM4_MemDbgInfo::operator = (const StM4_MemDbgInfo &ai_oOpe)
{
	strcpy( m_pcCompId, ai_oOpe.m_pcCompId ) ;
	strcpy( m_pcSrcId,  ai_oOpe.m_pcSrcId  ) ;
	m_uiLocalId		= ai_oOpe.m_uiLocalId ;
	return *this ;
}


StM4_MemDbgInfo::StM4_MemDbgInfo( m4char_t *	ai_pcCompId, m4char_t *ai_pcSrcId, m4uint16_t	ai_uiLocalId  ) 
{
	size_t L ;
	m4char_t *PAux ;
	
	m_uiLocalId		= ai_uiLocalId ;

	PAux = strrchr( ai_pcSrcId, '/' ) ;
	if ( PAux ) {
		ai_pcSrcId = PAux+1 ;	
	} else {
		PAux = strrchr( ai_pcSrcId, '\\' ) ;
		if ( PAux ) {
			ai_pcSrcId = PAux+1 ;	
		} ;
	} ;

	L = strlen(ai_pcSrcId) ;
	if ( L>M4_MEM_DBG_LOCAL_ID_SIZE-1 ) {
		strcpy(m_pcSrcId,"...") ;
		strcat(m_pcSrcId, &ai_pcSrcId[L-M4_MEM_DBG_LOCAL_ID_SIZE+1+3] ) ;
	} else {
		strcpy(m_pcSrcId,ai_pcSrcId) ;
	} ;

	L = strlen(ai_pcCompId) ;
	L = (L > M4_MEM_DBG_COMP_ID_SIZE-1) ?  M4_MEM_DBG_COMP_ID_SIZE-1 : L ;
	strcpy(m_pcCompId,ai_pcCompId) ;
}




m4bool_t StM4_MemDbgInfo::operator == (const StM4_MemDbgInfo &ai_oOpe) const
{
	return M4_BOOL( strcmp(m_pcCompId,ai_oOpe.m_pcCompId)==0 && 
		m_uiLocalId==ai_oOpe.m_uiLocalId &&
		strcmp(m_pcSrcId,ai_oOpe.m_pcSrcId)==0 )  ;
}

m4bool_t StM4_MemDbgInfo::operator < (const StM4_MemDbgInfo &ai_oOpe) const
{
	if ( strcmp(m_pcCompId, ai_oOpe.m_pcCompId) < 0 ) {
		return M4_TRUE ;
	} else if (strcmp(m_pcCompId, ai_oOpe.m_pcCompId) > 0 ) {
		return M4_FALSE ;
	} else {
		if ( strcmp(m_pcSrcId, ai_oOpe.m_pcSrcId) < 0 ) {
			return M4_TRUE ;
		} else if (strcmp(m_pcSrcId, ai_oOpe.m_pcSrcId) > 0) {
			return M4_FALSE ;
		} else {
			return M4_BOOL( m_uiLocalId<ai_oOpe.m_uiLocalId ) ;
		} ;

	} ;
}





ClM4_MemDbg::ClM4_MemDbg()
{
	//No hace falta sincronizacion: constructor de un unico objeto global.

	m_oMutex.Init() ;

	m_uiNewFailed  = 0 ;
	m_oDeleteFailed.clear() ;
	m_iAutoSnapShot = 50 ;	

	strcpy(m_pcSeedName,"UNKNOW" ) ;
	m_uiSeed = 0 ;

	m_oWindowList.clear() ;

	m_iWindowSNS = 10 ;

} 


ClM4_MemDbg::~ClM4_MemDbg()
{
	//No hace falta sincronizacion: destructor de un unico objeto global.

	m4char_t Name[M4_MEM_DBG_SEED_NAME_SIZE+40] ;


	if ( g_bM4SwMemDbgOn ) {
		sprintf( Name, "%s_FINAL.SNS", m_pcSeedName );
		_SnapShotToFile(	Name, REDUCE_BY_LINE) ;
	} ;

	m_oPtrs.clear() ;

}

void ClM4_MemDbg::NotifyNew( void *ai_Ptr, m4char_t *	ai_pcCompId, m4char_t * ai_pcSrcId, m4uint16_t ai_uiLocalId ) 
{
	m_oMutex.Lock() ;

	{
		StM4_MemDbgInfo Info(ai_pcCompId, ai_pcSrcId, ai_uiLocalId ) ;
		pair< AllocList_t::iterator, bool> Pair ;
		static m4uint32_t AuxCount=0 ;
		
		m4char_t * M4_SRV_TRACE_COMP_ID = ai_pcCompId ;	//Para engañar a la macro 

		M4_SRV_MSG_AUX( "NEW : Src %0:s Line %1:s Pointer %2:s", 
			ai_pcSrcId << ai_uiLocalId << (uintptr_t)ai_Ptr, M4_SRV_DEBUG_X_LEVEL_MEM )  ;
		
		Pair = m_oPtrs.insert( AllocList_t::value_type( (uintptr_t)ai_Ptr, Info ) ) ;
		
		if ( ! Pair.second ) {
			//Error ??
			M4_SRV_MSG_AUX( "NEW failed!!!!: Src %0:s Line %1:s Pointer %2:s", 
				ai_pcSrcId << ai_uiLocalId << (uintptr_t)ai_Ptr, M4_SRV_DEBUG_X_LEVEL_MEM )  ;
			++m_uiNewFailed ;
		} ;

		if ( m_iAutoSnapShot > 0 ) {
			++AuxCount ;
			if (AuxCount%m_iAutoSnapShot==0) _SnapShotToFile() ;
		} ;
	
	} ;

	m_oMutex.Unlock() ;
}

void ClM4_MemDbg::NotifyDelete( void *ai_Ptr, m4char_t *	ai_pcCompId, m4char_t * ai_pcSrcId, m4uint16_t ai_uiLocalId ) 
{
	m_oMutex.Lock() ;
	{
		AllocList_t::iterator It ;

		m4char_t * M4_SRV_TRACE_COMP_ID = ai_pcCompId ;	//Para engañar a la macro 

		M4_SRV_MSG_AUX( "DELETE : Src %0:s Line %1:s Pointer %2:s", 
			ai_pcSrcId << ai_uiLocalId << (uintptr_t)ai_Ptr , M4_SRV_DEBUG_X_LEVEL_MEM )  ;

		It = m_oPtrs.find((uintptr_t)ai_Ptr) ;
		if ( It!=m_oPtrs.end() ) {
			m_oPtrs.erase(It) ;
		} else {
			//Error ??
			StM4_MemDbgInfo Data(ai_pcCompId, ai_pcSrcId, ai_uiLocalId) ;

			m_oDeleteFailed.insert( AllocList_t::value_type( (uintptr_t)ai_Ptr,Data) ) ;
		};
	} ;
	m_oMutex.Unlock() ;
}

void ClM4_MemDbg::SetSeedName( const m4char_t *ai_Name ) 
{
	m_oMutex.Lock() ;
	{
		size_t L = strlen(ai_Name)  ;
		strcpy( m_pcSeedName, L > M4_MEM_DBG_SEED_NAME_SIZE-1 ? 
			&ai_Name[L-M4_MEM_DBG_SEED_NAME_SIZE] : ai_Name ) ;
	} ;
	m_oMutex.Unlock() ;
} 


m4uint32_t ClM4_MemDbg::GetUndeletedCount() 
{
	m4uint32_t R ;

	m_oMutex.Lock() ;
	{
		R = (m4uint32_t) m_oPtrs.size() ;
	} ;
	m_oMutex.Unlock() ;

	return R ;
}

m4bool_t ClM4_MemDbg::SnapShotToFile(
	m4char_t *ai_pcFile, TYPE_SNAP_SHOT ai_Type, CompIdList_t *ai_poCompId) 
{
	m4bool_t R ;

	m_oMutex.Lock() ;
	{
		R = _SnapShotToFile(ai_pcFile, ai_Type, ai_poCompId) ;
	} ;
	m_oMutex.Unlock() ;

	return R ;
} 

m4bool_t ClM4_MemDbg::SnapShotToFile( TYPE_SNAP_SHOT ai_Type, CompIdList_t *ai_poCompId) 
{
	m4bool_t R ;

	m_oMutex.Lock() ;
	{
		R = _SnapShotToFile( ai_Type, ai_poCompId) ;
	} ;
	m_oMutex.Unlock() ;

	return R ;
} 

m4bool_t ClM4_MemDbg::_SnapShotToFile(
	m4char_t *ai_pcFile, TYPE_SNAP_SHOT ai_Type, CompIdList_t *ai_poCompId) 
{
	m4bool_t R=M4_FALSE ;

	FILE *F=NULL ;

	ClM4_SrvTrace::CreateRecursivePath(ai_pcFile) ;
	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniNative ;

	F = M4Fopen(ai_pcFile, M4UniWrite, eEncoding) ;
	if (!F) {
		ClM4_SrvTrace::CreateRecursivePath(ai_pcFile) ;
		// UNICODE FILE
		eUniFileEncodingType_t eEncoding = M4UniNative ;

		F=M4Fopen(ai_pcFile, M4UniWrite, eEncoding)  ;
	} ;
		
	if (F) {
		if (m_uiSeed==0) {
			fprintf( F, "***>Memory SnapShot\n\n" ) ;
		} else {
			fprintf( F, "***>Memory SnapShot (Last Seq File %lu)\n\n", (unsigned long)m_uiSeed-1 ) ;
		} ;
	} ;

	if ( ai_Type==REDUCE_BY_LINE) {

		ReduceByLine_t RBL ;

		ReduceByLine(&m_oPtrs, &RBL, ai_poCompId ) ;

		R = M4_BOOL( RBL.size() > 0 ) ;

		ShowReduceList(F, &RBL ) ;


	} else {

		R = M4_BOOL( m_oPtrs.size() > 0 ) ;

		ShowAllocList(F, &m_oPtrs ) ;

	} ;

	if (F) {

		fprintf( F, "\n<***Memory SnapShot\n" ) ;

		if (m_uiNewFailed>0) {
			fprintf( F, "\n\nWARNING !!! NewFailed %lu\n\n", 
				(unsigned long)m_uiNewFailed ) ;
		} ;

		if (m_oDeleteFailed.size()>0 ) {

			fprintf( F, "\n\n***>WARNING !!! DeleteFailed\n\n" ) ;

			if ( ai_Type==REDUCE_BY_LINE ) {

				ReduceByLine_t RBL ;

				ReduceByLine(&m_oDeleteFailed, &RBL, ai_poCompId ) ;

				ShowReduceList(F, &RBL ) ;

			} else {

				ShowAllocList(F, &m_oDeleteFailed ) ;
			} ;
			fprintf( F, "\n\n<***WARNING !!! DeleteFailed\n\n" ) ;
		} ;
			
		fclose(F) ;
	} ;
	return R ;
}

m4bool_t ClM4_MemDbg::_SnapShotToFile( TYPE_SNAP_SHOT ai_Type, CompIdList_t *ai_poCompId) 
{
	m4bool_t SwOk ;

	m4char_t Name[M4_MEM_DBG_SEED_NAME_SIZE+20] ;

	sprintf( Name, "%s_%lu.SNS", m_pcSeedName ,(unsigned long)m_uiSeed );
	SwOk = _SnapShotToFile( Name, ai_Type, ai_poCompId) ;
	++m_uiSeed ;
	if ( m_iWindowSNS>0 ) {
		m_oWindowList.push_back( Name ) ;
		while ( m_oWindowList.size() > m_iWindowSNS ) {
			remove( m_oWindowList.front().c_str() ) ;
			m_oWindowList.pop_front() ;
		}  ;
	} ;

	return SwOk ;
}

void ClM4_MemDbg::ReduceByLine(AllocList_t *ai_poList, ReduceByLine_t *ao_poReduceList, CompIdList_t *ai_poCompId  ) 
{
	AllocList_t::iterator It ;
	CompIdList_t::iterator CompIt ;
	ReduceByLine_t::iterator RBLIt ;

	ao_poReduceList->clear() ;

	for ( It=ai_poList->begin(); It!= ai_poList->end(); ++It ) {

		if (ai_poCompId) {
			CompIt= ai_poCompId->find( (*It).second.GetCompId() ); 
			if (CompIt==ai_poCompId->end()) continue ;
		} ;
			
		RBLIt = ao_poReduceList->find( (*It).second ) ;
		if ( RBLIt==ao_poReduceList->end() ) {
			ao_poReduceList->insert( ReduceByLine_t::value_type((*It).second,1) ) ;
		} else {
			++ ( (*RBLIt).second ) ;  
		} ;
	} ;
}

void ClM4_MemDbg::ShowAllocList(FILE *ai_poF, AllocList_t *ai_poList) 
{
	AllocList_t::iterator It ;

	if (! ai_poF ) return ;

	for ( It=ai_poList->begin(); It!= ai_poList->end(); ++It ) {
		fprintf(ai_poF, "    CompId <%-10s> SrcId <%-10s> LineId <%6lu> Ptr <%10lu>\n",
			(uintptr_t)		(*It).second.GetCompId(),
			(char *)		(*It).second.GetSrcId(), 
			(uintptr_t)		(*It).second.GetLocalId(),
			(uintptr_t)		(*It).first ) ;
	} ;
}

void ClM4_MemDbg::ShowReduceList(FILE *ai_poF, ReduceByLine_t *ai_poList) 
{
	ReduceByLine_t::iterator It ;

	if (! ai_poF ) return ;

	for ( It=ai_poList->begin(); It!= ai_poList->end(); ++It ) {
		fprintf(ai_poF, "    CompId <%-10s> SrcId <%-20s> LineId <%6lu> Count <%6lu>\n",
			(uintptr_t)		(*It).first.GetCompId(),
			(char *)		(*It).first.GetSrcId(), 
			(uintptr_t)		(*It).first.GetLocalId(),
			(uintptr_t)		(*It).second ) ;
	} ;
}


void M4SrvConfigDevelopEnvByEnvVble( m4char_t *ai_pcTraceId ) 
{
	m4bool_t SwMemDbgOn  ;
	m4char_t *Path, *Content ;
	m4uint32_t AutoSNS=0 ;
	m4int16_t TraceLevel=1 ;
	m4bool_t SwM4Log=M4_TRUE ;
	m4bool_t SwCout ;
	m4string_t SeedName ;
	m4char_t PSep[2] ;
	size_t L ;
	m4int16_t CoutFile=0 ;
	m4string_t  CoutFileName ;
	const m4char_t    *CoutFilePtr ;
	m4bool_t    CoutFileFlush ;
	m4int16_t WindowSNS=10 ;

	SwMemDbgOn=M4_FALSE ;
	SwCout	  =M4_FALSE ;

	#ifdef _UNIX
	strcpy(PSep,"/") ;
	Path = "./" ;
	#else
	strcpy(PSep,"\\") ;
	Path = ".\\" ;
	#endif

	//Altera defectos

	Content=getenv("M4_SRV_MDBG") ;
	if (Content) {
		SwMemDbgOn = M4_BOOL( atoi(Content) ) ;
	} ;

	Content=getenv("M4_SRV_SNS_PATH") ;
	if (Content) {
		if(strlen(Content)>0) Path = Content ;
	} ;

	Content=getenv("M4_SRV_AUTO_SNS") ;
	if (Content) {
		AutoSNS = atoi(Content) ;
	} ;

	Content=getenv("M4_SRV_TRACE_LEVEL") ;
	if (Content) {
		TraceLevel = atoi(Content) ;
	} ;

	Content=getenv("M4_SRV_M4LOG") ;
	if (Content) {
		SwM4Log = M4_BOOL( atoi(Content) ) ;
	} ;

	Content=getenv("M4_SRV_COUT") ;
	if (Content) {
		SwCout = M4_BOOL( atoi(Content) ) ;
	} ;

	Content=getenv("M4_SRV_COUT_FILE") ;
	if (Content) {
		CoutFile = M4_BOOL( atoi(Content) ) ;
	} ;

	Content=getenv("M4_SRV_WINDOW_SNS") ;
	if (Content) {
		WindowSNS = atoi(Content) ;
	} ;

	//Haya SeedName y CoutFileName, por si este se usa

	SeedName = Path ;
	L = strlen(SeedName.c_str() ) ;
	if ( !L ) {
		SeedName += PSep ;
	} else if ( SeedName[L-1] != PSep[0] ) {
		SeedName += PSep ;
	} ;
	SeedName += ai_pcTraceId ;
	SeedName += PSep ;
	SeedName += ai_pcTraceId ;
	CoutFileName = SeedName ;
	CoutFileName += ".LOG" ;

	//Haya CoutFile y flag Flush

	if ( CoutFile==0 || CoutFile==0 ) {
		CoutFileFlush=M4_FALSE ;
	} else {
		CoutFileFlush=M4_TRUE ;
	} ;

	if ( CoutFile==0  ) {
		CoutFilePtr=NULL ;
	} else {
		CoutFilePtr=CoutFileName.c_str() ;
	} ;

	//Resetea defectos

	g_bM4SwMemDbgOn=SwMemDbgOn ;
	g_oM4MemDbg.SetSeedName( SeedName.c_str() ) ;
	g_oM4MemDbg.SetAutoSnapShot(AutoSNS) ;
	g_oM4SrvTrace.SetCurrentLevel(TraceLevel) ;
	g_bM4SwSrvCoutOn=SwCout ;
	g_bM4SwSrvM4LogOn=SwM4Log ;
	g_oM4SrvTrace.SetTraceFile( CoutFilePtr ) ;
	g_oM4SrvTrace.SetFlush( CoutFileFlush ) ;
	g_oM4MemDbg.SetWindowSNS(WindowSNS) ;

}

