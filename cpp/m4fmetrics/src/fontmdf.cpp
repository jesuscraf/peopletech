//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             FONTMdf.cpp
// Project:			 fmetrics.dll
// Author:           Meta Software M.S. , S.A
// Date:             6/05/99
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Manuel Lazcano Perez
//	Modifications:	Who && Date
//
// Definition:
//
//
//
//==============================================================================

/*
Grupos de traza y memoria del componente MIT
----------------------------------------------------------------------------------------------
*/

#include "font.h"
#include "sysinfo.h"


#define FONT_M_ID 2
#define FONT_M_STR_ID "FNT_MTRC"


//Clave del Registry para el path de los RFNFiles
enum {FONT_M_RK_RFN_PATH=0x200};

enum FONT_M_TRACE_GROUPS {
	FONT_M_TGI_FONT=MIT_TGI_NOT_SHARED_FIRST,
	FONT_M_TGI_READ_RFN,
	FONT_M_TGI_RFN_LIST,	//Especial para deshabilitar toda traza excepto el listado RFN
	FONT_M_TGI_COUNT_TOTAL,
	FONT_M_TGI_PARTIAL_COUNT=FONT_M_TGI_COUNT_TOTAL-MIT_TGI_NOT_SHARED_FIRST
} ;

enum FONT_M_TRACE_BASES {
	FONT_M_TGB_FONT=4194304,
	FONT_M_TGB_READ_RFN=4194309,
	FONT_M_TGB_RFN_LIST=4194309
} ;

enum FONT_M_MEM_GROUPS {
	FONT_M_MGI_FONT=MIT_MGI_NOT_SHARED_FIRST,
	FONT_M_MGI_READ_RFN,
	FONT_M_MGI_COUNT
} ;

#ifdef _INCLUDING_FROM_FONT_M_
//Ojo, los indices deben ser los enum de MIT_MEM_GROUPS
static const m4char_t * g_FONT_M_pcMG[] = { 
	"Fonts",
	"RFN File"
} ;

//Ojo, los indices deben ser los enum de FONT_M_TRACE_GROUPS
static  const m4char_t * g_pc_FontTraceGroupNames[] =
{
	"FONT",
	"READRFN",
	"RFNLIST"
} ;

#endif

/*
Macros de acceso a MIT
----------------------------------------------------------------------------------------------
*/

#define GET_INSP() g_oMIT_P.GetInsp(FONT_M_ID)
#define GET_TM()   g_oMIT_P.GetTM()
#define TM_GET_INSP(TM) (TM)->GetInsp(FONT_M_ID)
#define SPAWN_TASK(p1)  g_oMIT_P.SpawnTask((p1))
#define KILL_TASK(p1)  g_oMIT_P.KillTask((p1))

#define TM_GET_TASK_ENV(TM) (StFONT_M_TaskEnv*) (TM)->GetTaskEnv(FONT_M_ID)

/*
Environments
----------------------------------------------------------------------------------------------
*/

struct StFONT_M_ThreadEnv {
	m4uint32_t ENV ;
} ;

struct StFONT_M_TaskEnv {
	ClFONT_M_DBFont* m_poDBFont;
} ;


#ifdef _INCLUDING_FROM_FONT_M_

/*
PlugIn
----------------------------------------------------------------------------------------------
*/

static StMIT_CompPlugIn * g_FONT_M_GetSonPlugIn(m4int16_t ai_Index) 
{
	if (ai_Index==0) {
		return g_MIT_pstPlugIn ;
	} else {
		return NULL ;
	} ;
}

static void g_FONT_M_InitMemProf( ClMIT_MemProf *ai_poMemProf) 
{
	ai_poMemProf->Init(FONT_M_MGI_COUNT, MIT_MGI_SHARED_COUNT,g_MIT_pcSharedMG,g_FONT_M_pcMG) ;
	ai_poMemProf->SetCheckPointAtEnd(M4_TRUE) ;
}

static void g_FONT_M_InitSysInfo( ClMIT_SysInfo *ai_poSysInfo)
{
	ClMIT_Str Path;

	ai_poSysInfo->Init();
	ai_poSysInfo->PrepareRegistryPath(&Path);
	ai_poSysInfo->NewValue(FONT_M_RK_RFN_PATH, Path.InChar(0), "RFNPath");
	Path.End();
}


static void g_FONT_M_InitTrace( ClMIT_Trace   *ai_poTrace, 
		const m4char_t *ai_pcLocalTraceFile, const m4char_t *ai_pcM4INIFile) 
{

	ai_poTrace->Init(
		"RPT", FONT_M_STR_ID,
		ai_pcLocalTraceFile,ai_pcM4INIFile, 
		g_pc_FontTraceGroupNames, FONT_M_TGI_PARTIAL_COUNT ) ;
}

static void * g_FONT_M_NewThreadEnv() 
{
	return new StFONT_M_ThreadEnv ;
}

static  void g_FONT_M_DeleteThreadEnv(void *ai_poEnv) 
{
	StFONT_M_ThreadEnv * R = (StFONT_M_ThreadEnv *)ai_poEnv ; 

	delete R ;
}

static void * g_FONT_M_NewTaskEnv() 
{
	StFONT_M_TaskEnv* R ;

	M4_NEW_ITEM( GET_INSP()->m_poMemProf, MIT_MGI_OTHERS, R, StFONT_M_TaskEnv ) ;

	return R ;
}

static  void g_FONT_M_DeleteTaskEnv(void *ai_poEnv) 
{
	StFONT_M_TaskEnv * R = (StFONT_M_TaskEnv *)ai_poEnv ; 

	M4_DELETE_ITEM( GET_INSP()->m_poMemProf, MIT_MGI_OTHERS, R, StFONT_M_TaskEnv ) ;
}

StMIT_CompPlugIn g_FONT_M_pstPlugIn(  
	FONT_M_ID,
	FONT_M_STR_ID,
	g_FONT_M_GetSonPlugIn,
	g_FONT_M_InitSysInfo,
	g_FONT_M_InitMemProf,
	g_FONT_M_InitTrace,
	g_FONT_M_NewTaskEnv,
	g_FONT_M_DeleteTaskEnv,
	g_FONT_M_NewThreadEnv,
	g_FONT_M_DeleteThreadEnv
	
) ;

#endif

