#ifndef _RFNDF_INCLUDED_
#define _RFNDF_INCLUDED_


//#include "m4res.hpp"

/*
Grupos de traza y memoria del componente MIT
----------------------------------------------------------------------------------------------
*/

#include "mit.h"

#define RFN_ID 4
#define RFN_STR_ID "RFN"

enum RFN_TRACE_GROUPS {
	RFN_TGI_OTHERS=MIT_TGI_NOT_SHARED_FIRST,
	RFN_M_TGI_COUNT_TOTAL,
	RFN_M_TGI_PARTIAL_COUNT=RFN_M_TGI_COUNT_TOTAL-MIT_TGI_NOT_SHARED_FIRST
} ;

enum RFN_TRACE_BASES {
	RFN_TGB_OTHERS=4325376
} ;

enum RFN_MEM_GROUPS {
	RFN_MGI_SYMBOL=MIT_MGI_NOT_SHARED_FIRST,
	RFN_MGI_COUNT
} ;

#ifdef _INCLUDING_FROM_RFN_
//Ojo, los indices deben ser los enum de RFN_MEM_GROUPS
static const m4char_t * g_RFN_pcMG[] = { 
	"KK",
} ;

//Ojo, los indices deben ser los enum de FONT_M_TRACE_GROUPS
static  const m4char_t * g_pc_RFNTraceGroupNames[] =
{
	"OTHERS"
} ;

#endif

/*
Macros de acceso a MIT
----------------------------------------------------------------------------------------------
*/

#define GET_INSP() g_oMIT_P.GetInsp(RFN_ID)
#define GET_TM()   g_oMIT_P.GetTM()
#define TM_GET_INSP(TM) (TM)->GetInsp(RFN_ID)
#define SPAWN_TASK(p1)  g_oMIT_P.SpawnTask((p1))
#define KILL_TASK(p1)  g_oMIT_P.KillTask((p1))

#define TM_GET_TASK_ENV(TM) ((StRFN_TaskEnv*) (TM)->GetTaskEnv(RFN_ID))
#define GET_TASK_ENV() ((StRFN_TaskEnv*)    GET_TM()->GetTaskEnv(RFN_ID))
/*
Environments
----------------------------------------------------------------------------------------------
*/

struct StRFN_ThreadEnv {
	m4int16_t KK ;
} ;


struct StRFN_TaskEnv {
	m4int16_t KK ;
} ;


#ifdef _INCLUDING_FROM_RFN_

#include "sysinfo.h"

/*
PlugIn
----------------------------------------------------------------------------------------------
*/

#include "fontm.h"

static StMIT_CompPlugIn * g_RFN_GetSonPlugIn(m4int16_t ai_Index) 
{
	if (ai_Index==0) {
//		return g_MIT_pstPlugIn ;
//	} else if (ai_Index==1) {
		return &g_FONT_M_pstPlugIn ;
	} else {
		return NULL ;
	} ;
}

static void g_RFN_InitMemProf( ClMIT_MemProf *ai_poMemProf) 
{
	ai_poMemProf->Init(RFN_MGI_COUNT, MIT_MGI_SHARED_COUNT,g_MIT_pcSharedMG,g_RFN_pcMG) ;
	ai_poMemProf->SetCheckPointAtEnd(M4_TRUE) ;
}

static void g_RFN_InitTrace( ClMIT_Trace   *ai_poTrace, 
		const m4char_t *ai_pcLocalTraceFile, const m4char_t *ai_pcM4INIFile) 
{
	#ifdef RFN_TRACE
	ai_poTrace->Init("RPT", RFN_STR_ID, ai_pcLocalTraceFile,ai_pcM4INIFile, 
		g_pc_RFNTraceGroupNames, RFN_M_TGI_PARTIAL_COUNT ) ;
	ai_poTrace->m_oOutputFlags[ClMIT_Msg::BE_OUT].On(ClMIT_Trace::FLUSH_SBS) ;
	#else
	ai_poTrace->Init("RPT", RFN_STR_ID, NULL, ai_pcM4INIFile, 
		g_pc_RFNTraceGroupNames, RFN_M_TGI_PARTIAL_COUNT ) ;
	#endif
}

static void g_RFN_InitSysInfo( ClMIT_SysInfo *ai_poSysInfo) 
{
	ai_poSysInfo->Init() ;
}


static void * g_RFN_NewThreadEnv() 
{
	return new StRFN_ThreadEnv ;
}

static  void g_RFN_DeleteThreadEnv(void *ai_poEnv) 
{
	StRFN_ThreadEnv * R = (StRFN_ThreadEnv *)ai_poEnv ; 

	delete R ;
}

static void * g_RFN_NewTaskEnv() 
{
	StRFN_TaskEnv* R ;

	M4_NEW_ITEM( GET_INSP()->m_poMemProf, MIT_MGI_OTHERS, R, StRFN_TaskEnv ) ;
	
	return R ; ;
}

static  void g_RFN_DeleteTaskEnv(void *ai_poEnv) 
{
	StRFN_TaskEnv * R = (StRFN_TaskEnv *)ai_poEnv ; 

	M4_DELETE_ITEM( GET_INSP()->m_poMemProf, MIT_MGI_OTHERS, R, StRFN_TaskEnv ) ;
}

StMIT_CompPlugIn g_RFN_pstPlugIn(  
	RFN_ID,
	RFN_STR_ID,
	g_RFN_GetSonPlugIn,
	g_RFN_InitSysInfo,
	g_RFN_InitMemProf,
	g_RFN_InitTrace,
	g_RFN_NewTaskEnv,
	g_RFN_DeleteTaskEnv,
	g_RFN_NewThreadEnv,
	g_RFN_DeleteThreadEnv
	
) ;

#endif


#endif
