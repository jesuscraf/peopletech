//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             FMTdf.cpp
// Project:			 M4FMT32.dll
// Author:           Meta Software M.S. , S.A
// Date:             05/06/99
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

#include "sysinfo.h"
#include "plugin.hpp"
#include "access.hpp"

#define FMT_ID 5
#define FMT_STR_ID "FMT"

class FMT_ClFormato;
class FMT_ClContainer;
class CLM4FMT;

struct StSuposiciones
{ 
   m4int16_t valor;
   m4char_t tipo;
   m4char_t cuenta;
};

struct StDatosDesformateados
{ 
   ClCadena sCadenaOriginal;
      
   m4char_t sAnio[256];
   m4char_t sMes[64];
   m4char_t sDiaSemana[32];
   m4char_t sDia[64];
   m4char_t sHora[64];
   m4char_t sMinuto[64];
   m4char_t sSegundo[64];   
   
   ClCadena sParteEntera;
   ClCadena sParteDecimal;
   
   ClCadena sThousand;
   ClCadena sDecimal;
   ClCadena sCharsFill; 
   ClCadena sCurrency;
   
    m4bool_t bNumNeg;      
};

//Clave del Registry para el path de los RFNFiles
enum {FMT_RK_LANGUAGE_PATH=0x500, FMT_RK_PARAMS_DIR=0x505};

enum FMT_TRACE_GROUPS {
	FMT_TGI_M4FMT=MIT_TGI_NOT_SHARED_FIRST,
	FMT_TGI_CLM4FMT,
	FMT_M_TGI_COUNT_TOTAL,
	FMT_M_TGI_PARTIAL_COUNT=FMT_M_TGI_COUNT_TOTAL-MIT_TGI_NOT_SHARED_FIRST
} ;


enum FMT_TRACE_BASES {
	FMT_TGB_M4FMT=100,
	FMT_TGB_CLM4FMT,
	FMT_TGB_FONT
} ;

enum FMT_MEM_GROUPS {
	FMT_MGI_M4FMT=MIT_MGI_NOT_SHARED_FIRST,
	FMT_MGI_CLM4FMT,
	FMT_MGI_COUNT
} ;

#ifdef _INCLUDING_FROM_M4FMT_
//Ojo, los indices deben ser los enum de MIT_MEM_GROUPS
static const m4char_t * g_FMT_pcMG[] = { 
	"M4Fmt",
	"ClM4Fmt"
} ;

//Ojo, los indices deben ser los enum de FMT_M_TRACE_GROUPS
static  const m4char_t * g_pc_FormatTraceGroupNames[] =
{
	"M4FMT",
	"CLM4FMT"
} ;

#endif

/*
Macros de acceso a MIT
----------------------------------------------------------------------------------------------
*/

#define GET_INSP() g_oMIT_P.GetInsp(FMT_ID)
#define GET_TM()   g_oMIT_P.GetTM()
#define TM_GET_INSP(TM) (TM)->GetInsp(FMT_ID)
#define SPAWN_TASK(p1)  g_oMIT_P.SpawnTask((p1))
#define KILL_TASK(p1)  g_oMIT_P.KillTask((p1))

#define TM_GET_TASK_ENV(TM) (StFMT_TaskEnv*) (TM)->GetTaskEnv(FMT_ID)

/*
Environments
----------------------------------------------------------------------------------------------
*/

struct StFMT_ThreadEnv {
	m4uint32_t ENV ;
} ;

struct StFMT_TaskEnv {
	//----->Propietaria dll
	CLM4FMT*  g_pClM4Fmt;

	//Fichero para tracear la aplicacion
	FILE* g_fTraceFile ;
	//----->Fin Propietaria dll

	//Acceso del canal que proporciona los parámetros Windows ;
	ClAccess* g_AccessDesign;

	//----->Propietaria FMT_ClElement o derivadas
	m4bool_t Format_g_bHay;
	m4bool_t Format_g_bTio;
	//----->Fin Propietaria FMT_ClElement

	//----->Propietaria CLM4FMT
	m4uint32_t             ClM4Fmt_g_uCount;
	m4bool_t               ClM4Fmt_g_bInitialize;
	FMT_ClFormato*         ClM4Fmt_g_pFormatoWINDOWS;
	FMT_ClFormato*         ClM4Fmt_g_pFormatoISO;
	StDatosDesformateados* ClM4Fmt_g_pDD;
	m4char_t*              g_LightString[12+7];
	//----->Fin Propietaria CLM4FMT

	//----->Propietaria FMT_ClContainer
	m4bool_t     Container_g_bContainerDestroyed;
	FMT_ClContainer* Container_g_pContainer;
	LONG         Container_g_lSearchs;
	LONG		 Cpntainer_g_lCacheFaults;
	//----->Fin Propietaria FMT_ClContainer
		 
	//----->Propietaria FMT_ClFormato
	ClCadena*      Cadena_g_pCad;
	ClCadena*      Cadena_g_pMask;
	ClCadena*      Cadena_g_pNum;
	StSuposiciones Formato_g_Suposiciones[8];
	m4int16_t      Formato_g_Asignados[7];
	m4int16_t      Formato_g_iSuposiciones;
	m4int16_t      Formato_g_TablaOrden[7];
	m4int16_t      Formato_g_iCuenta;
	//----->Fin Propietaria FMT_ClFormato

	//----->Link al gestor de Plug-Ins de idioma
	FMT_ClPlugInManager* m_poPlugIn;
} ;


#ifdef _INCLUDING_FROM_M4FMT_

/*
PlugIn
----------------------------------------------------------------------------------------------
*/

static StMIT_CompPlugIn * g_FMT_GetSonPlugIn(m4int16_t ai_Index) 
{
	return ai_Index==0 ? g_MIT_pstPlugIn : NULL ;
}

static void g_FMT_InitMemProf( ClMIT_MemProf *ai_poMemProf) 
{
	ai_poMemProf->Init(FMT_MGI_COUNT, MIT_MGI_SHARED_COUNT,g_MIT_pcSharedMG,g_FMT_pcMG) ;
	ai_poMemProf->SetCheckPointAtEnd(M4_TRUE) ;
}

static void g_FMT_InitSysInfo( ClMIT_SysInfo *ai_poSysInfo)
{
	ClMIT_Str Path; 

	ai_poSysInfo->Init();
	ai_poSysInfo->PrepareRegistryPath(&Path);
	ai_poSysInfo->NewValue(FMT_RK_LANGUAGE_PATH, Path.InChar(0), "Language");
	
	ai_poSysInfo->PrepareRegistryPath(&Path);
	ai_poSysInfo->NewValue(FMT_RK_PARAMS_DIR, Path.InChar(0), "ParamsDir");
	
	Path.End();
}


static void g_FMT_InitTrace( ClMIT_Trace   *ai_poTrace, 
		const m4char_t *ai_pcLocalTraceFile, const m4char_t *ai_pcM4INIFile) 
{
	#ifdef _DEBUG
	ai_poTrace->Init(
		"RPT", FMT_STR_ID,
		ai_pcLocalTraceFile,ai_pcM4INIFile, 
		g_pc_FormatTraceGroupNames, FMT_M_TGI_PARTIAL_COUNT ) ;
	#else
	ai_poTrace->Init(
		"RPT", FMT_STR_ID,
		NULL,				ai_pcM4INIFile, 
		g_pc_FormatTraceGroupNames, FMT_M_TGI_PARTIAL_COUNT ) ;
	#endif
}

static void * g_FMT_NewThreadEnv() 
{
	return new StFMT_ThreadEnv ;
}

static  void g_FMT_DeleteThreadEnv(void *ai_poEnv) 
{
	StFMT_ThreadEnv * R = (StFMT_ThreadEnv *)ai_poEnv ; 

	delete R ;
}

static void * g_FMT_NewTaskEnv() 
{
	StFMT_TaskEnv* R ;

	M4_NEW_ITEM( GET_INSP()->m_poMemProf, MIT_MGI_OTHERS, R, StFMT_TaskEnv ) ;

	return R ;
}

static  void g_FMT_DeleteTaskEnv(void *ai_poEnv) 
{
	StFMT_TaskEnv * R = (StFMT_TaskEnv *)ai_poEnv ; 

	M4_DELETE_ITEM( GET_INSP()->m_poMemProf, MIT_MGI_OTHERS, R, StFMT_TaskEnv ) ;
}

StMIT_CompPlugIn g_FMT_pstPlugIn(  
	FMT_ID,
	FMT_STR_ID,
	g_FMT_GetSonPlugIn,
	g_FMT_InitSysInfo,
	g_FMT_InitMemProf,
	g_FMT_InitTrace,
	g_FMT_NewTaskEnv,
	g_FMT_DeleteTaskEnv,
	g_FMT_NewThreadEnv,
	g_FMT_DeleteThreadEnv
	
) ;

#endif

