//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             ENGdf.cpp
// Project:			 engindll.dll
// Author:           Meta Software M.S. , S.A
// Date:             14/10/99
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

#ifndef _ENGDF_INCLUDED_
#define _ENGDF_INCLUDED_

/*
Defs para renombran los nombres de las clases de canal
----------------------------------------------------------------------------------------------
*/

#include "apichndf.inl"	
#include "apichnen.h"
#include "m4res.hpp"

//#include <stdlib.h>

//#include "chvmenv.hpp"	//Para flag de Cancel

// *.H externos al ENGINE

#include "m4define.hpp"
#include "m4types.hpp"
#include "m4string.hpp"

// *.H del ENGINE

#define ENG_ID 4
#define ENG_STR_ID "ENG"


//Clave del Registry para el path de los RFNFiles
enum {ENG_RK_TRACE_FILE=0x400, ENG_RK_WORKDIR=0x500, ENG_RK_LOG_INI_FILE=0x600,
	  ENG_RK_RFN_PATH=0x700, ENG_RK_GRAPHICS_PATH=0x800, ENG_RK_GRAPHICS_PATH_BW=0x900,
	  APISQL_RK_SMOLTP=0x1000, ENG_RK_GRAPHICS_PATH_HTML=0x1100, ENG_RK_TEMP_WEB_IMAGES=0x1200,
	  ENG_RK_UNICODE_FONT_FILENAME=0x1300 };


//---------------------------------Switch modo ejecucion---------------------------------

/*
	Definir o no estos switchs segun se desee. NO definirlos aqui: es mucho mejor hacerlo 
en :
	Project - Settings - C/C++ - Category:General - Preprocessor definitions

	El motivo es que asi el codigo es el mismo para las versiones Debug o Release, al no
tener que tocar este *.H para nada.


#define RPT_ENGINE_DEBUG	

	Si esta definido, se entiende que se esta en modo debug. En este modo hay informacion
de debug y trace que es incluida, con la penalizacion de ralentizar el programa. 
	Tambien afecta al comportamiento de ciertas funciones.

#define RPT_ENGINE_TRACE
  
	Indica que se quieren mensajes trace. El sistema trace siempre se inicializa, otra cosa
es que se haga uso de esta macro para llamar a los metodos ClMIT_Trace o no.
	Actualmente, siempre se utiliza el trace, los errores y warnings siempre estan 
hard-coded, pero los mensajes debug solo lo estan si se define RPT_ENGINE_TRACE 
(excepto algunos muy genericos que no repercuten en la velocidad).

#define M4_MEM_PROF 

	Si esta definido, se activa el profiler de memoria. Lo mas normal es que se defina o 
no paralelamente con RPT_ENGINE_DEBUG.

#define M4_API_CHN_FORMAT

	Si esta definida, el modulo APICHN.CPP interactua con la DLL de formateos. Afecta al
modo de como se compila dicho modulo.

*/

//---------------------------------Macros de uso general---------------------------------

#define M4_MIN(a,b) ( (a)<(b)?(a):(b) )
#define M4_MAX(a,b) ( (a)>(b)?(a):(b) )
#define M4_IN_RANGE(c,a,b) ( (c)>=(a) && (c)<=(b) ) 

#define M4_INIT_ALLOC_OK   1
#define M4_INIT_ALLOC_FAIL -1

#define M4_END_STR 0

#define M4_H_ACCESS_PUBLIC    1
#define M4_H_ACCESS_PROTECTED 2
#define M4_H_ACCESS_PRIVATE   3


//---------------------Grupos de Traza y de Memoria----------------------------------------
enum ENG_TRACE_GROUPS {
	ENG_TGI_APICHN=MIT_TGI_NOT_SHARED_FIRST,
	ENG_TGI_CHN_VALUE,
	ENG_TGI_LOADERCN,
	ENG_TGI_CORE,
	ENG_TGI_CORE_VR,
	ENG_TGI_CROSSTAB,
	ENG_TGI_CLARGS,
	ENG_TGI_CLFORMAT,
	ENG_TGI_FONT,
	ENG_TGI_M4CLBUF,
	ENG_TGI_FIELD,
	ENG_TGI_RASTERG,
	
	ENG_TGI_ANALEX,
	ENG_TGI_TOTALIZE,
	ENG_TGI_STRINDEX,
	ENG_TGI_CLDEVICE,
	ENG_TGI_CLDVCFI,
	ENG_TGI_CLDVCTXT,
	ENG_TGI_CLDVCPCL,
	ENG_TGI_CLDVCPSC,
	ENG_TGI_CLDVCESC,
	ENG_TGI_CLDVCRVW,
	ENG_TGI_DVCHTML,
	ENG_TGI_CLREPARG,
	ENG_TGI_LOADER,
	ENG_TGI_READ_RFN,
	ENG_TGI_COLOUR,
	ENG_TGI_CLDVCPDF,
	ENG_TGI_METACHN,
	ENG_TGI_SESSCHN,
	ENG_TGI_FILEREP,
	ENG_TGI_CLDVCHTML,

	ENG_TGI_WRAPVB,

	ENG_TGI_COUNT_TOTAL,
	ENG_TGI_PARTIAL_COUNT=ENG_TGI_COUNT_TOTAL-MIT_TGI_NOT_SHARED_FIRST,

	ENG_TGI_ALL_GROUP	//Flag de proceso, no es un grupo
} ;



enum ENG_TRACE_BASES {
	ENG_TGB_TRACE=2162688,
	ENG_TGB_APICHN=2162699,
	ENG_TGB_CHN_VALUE=2162724,
	ENG_TGB_LOADERCN=2162736,
	ENG_TGB_CORE=2162739,
	ENG_TGB_CORE_VR=2162773,
	ENG_TGB_CROSSTAB=2162779,
	ENG_TGB_CLARGS=2162781,
	ENG_TGB_CLFORMAT=2162789,
	ENG_TGB_FONT=2162796,
	ENG_TGB_M4CLBUF=2162799,
	ENG_TGB_MEMPROF=2162801,
	ENG_TGB_MIDEFINE=2162810,
	ENG_TGB_FIELD=2162814,
	ENG_TGB_RASTERG=2162820,
	ENG_TGB_CLSTR=2162827,
	ENG_TGB_ANALEX=2162828,
	ENG_TGB_TOTALIZE=2162876,
	ENG_TGB_STRINDEX=2162887,
	ENG_TGB_CLDEVICE=2162889,
	ENG_TGB_CLDVCFI=2162890,
	ENG_TGB_CLDVCTXT=2162899,
	ENG_TGB_CLDVCPCL=2162905,
	ENG_TGB_CLDVCPSC=2162909,
	ENG_TGB_CLDVCESC=2162913,
	ENG_TGB_CLDVCRVW=2162916,
	ENG_TGB_DVCHTML=2162918,
	ENG_TGB_CLFILE=2162921,
	ENG_TGB_CLREPARG=2162930,
	ENG_TGB_LOADER=2162934,
	ENG_TGB_READ_RFN=2162975,
	ENG_TGB_COLOUR=2162979,
	ENG_TGB_CLDVCPDF,
	ENG_TGB_METACHN,
	ENG_TGB_SESSCHN,
	ENG_TGB_FILEREP,
	ENG_TGB_CLDVCHTML,

	ENG_TGB_WRAPVB=2162986,
	ENG_TGB_CLDVCPDF_UNICODE = 2163439,

	ENG_TGB_CLREPARG_EX = 2163500,
	ENG_TGB_APICHN_EX=2163520
} ;

enum ENG_MEM_GROUPS {
	ENG_MGI_LOADER=MIT_MGI_NOT_SHARED_FIRST, 
	ENG_MGI_CORE, ENG_MGI_FIELD, ENG_MGI_RASTERG, ENG_MGI_APICHN, 
	ENG_MGI_CLDEVICE, ENG_MGI_FONT, ENG_MGI_FORMATS,  ENG_MGI_CLBUFFER, ENG_MGI_REP_REQ,
	ENG_MGI_ANALEX,
	ENG_MGI_TOT,
	ENG_MGI_OTHERS,
	ENG_MGI_TEST,	//Sirve para redirigir subgrupos de news en debug
	ENG_MGI_COUNT
} ;

#ifdef _INCLUDING_FROM_ENGINDLL_
//Ojo, los indices deben ser los enum de MIT_MEM_GROUPS
static const m4char_t * g_ENG_pcMG[] = { 
	"Loader", 
	"Core", 
	"Fields en general",
	"Fields tipo raster graphic",
	"API channel",
	"Devices",
	"Fonts",
	"Formats",
	"Clase M4ClBuffer",
	"Clases ClReportRequest",
	"Modulo Analex",
	"Gestion totalizadores",
	"Varios",
	"Redirecciones debug"
} ;

//Ojo, los indices deben ser los enum de FONT_M_TRACE_GROUPS
static  const m4char_t * g_pc_EngineTraceGroupNames[] =
{
	"APICHN",
	"CHNVALUE",
	"LOADERCN",
	"CORE",
	"CORE_VR",
	"CROSSTAB",
	"CLARGS",
	"FORMAT",
	"FONT",
	"M4CLBUF",
	"FIELD",
	"RASTERG",
	"ANALEX",
	"TOTALIZE",
	"STRINDEX",
	"DEVICE",
	"DVCFI",
	"DVCTXT",
	"DVCPCL",
	"DVCPSC",
	"DVCESC",
	"DVCRVW",
	"DVCHTML",
	"CLREPARG",
	"LOADER",
	"READ_RFN",
	"COLOUR",
	"DVCPDF",
	"METACHN",
	"SESSCHN",
	"FILEREP",
	"DVCHTML",
	"WRAPVB"
} ;

#endif

/*
Macros de acceso a MIT
----------------------------------------------------------------------------------------------
*/

#define GET_INSP() g_oMIT_P.GetInsp(ENG_ID)
#define GET_TM()   g_oMIT_P.GetTM()
#define TM_GET_INSP(TM) (TM)->GetInsp(ENG_ID)
#define SPAWN_TASK(p1)  g_oMIT_P.SpawnTask((p1))
#define KILL_TASK(p1)  g_oMIT_P.KillTask((p1))

#define TM_GET_TASK_ENV(TM) ((StENG_TaskEnv*) (TM)->GetTaskEnv(ENG_ID))
#define GET_TASK_ENV() ((StENG_TaskEnv*)    GET_TM()->GetTaskEnv(ENG_ID))

/*
Environments
----------------------------------------------------------------------------------------------
*/

class ClENG_SessReportDesigner ;
class ClENG_Report ;
class ClENG_PhysicalFile ;

struct StENG_ThreadEnv {
	m4uint32_t ENV ;
} ;

//-------------->SessionType = RVW_LOOP (ciclo ViewerInit, ViewerBreakLoop, ViewerEnd)

struct StENG_RVW_Session {
	ClENG_Report  * m_poReport ;
	ClENG_PhysicalFile    * m_poFile ;	//Simple link
	m4uint32_t      m_uiFilePos ;

	StENG_RVW_Session() ;
} ;

struct StENG_TaskEnv {

	StMIT_APIChn_TaskEnv m_APIChn ;

	m4uint32_t m_uiFmtTaskId;   //Id de la tarea de la DLL de formateos	
	
	//-------------->Propietaria:M4ClBuffer (para leer de fichero y escribir a buffer)

	enum { M4ClBuffer_READ_SIZE=512*50 } ;	//512 es un tamaño usual de sector
	m4char_t M4ClBuffer_g_pcRBuff[M4ClBuffer_READ_SIZE] ;
	

	//-------------->Propietaria:ClFormat
	enum { Format_SIZE_STR_FORMAT=2000 } ;
	m4char_t Format_g_pcFormStr[Format_SIZE_STR_FORMAT] ;
	//-------------->Fin Propietaria:ClFormat
	
	//-------------->Propietarias:clases device


	enum {DvcESC_MAX_SIZE = 1024 };
	enum {DvcPCL_MAX_SIZE = 1024*15 } ;
	enum {DvcPSC_MAX_SIZE = 2048 } ;
	enum {DvcRVW_MAX_SIZE = 1024 } ;
	enum {DvcTXT_MAX_SIZE = 1024 } ;
	enum {DvcHTML_MAX_SIZE = 2048 } ;
	enum {DvcPDF_MAX_SIZE = 1024 * 15 } ;
	 // Se toma el del dispositivo que tiene mayor tamaño más un margen de seguridad
	m4char_t Dvc_g_pcBuff[DvcPCL_MAX_SIZE + 512];

	//-------------->Fin propietarias:clases device

	//================================>Variables que se usan según el SessionType

	enum SESSION_TYPE { EXECUTE_REPORT, RVW_LOOP, REPORT_DESIGNER } ;
	SESSION_TYPE m_SessionType ;	//Se mete a titulo informativo

	//-------------->SessionType = RVW_LOOP (ciclo ViewerInit, ViewerBreakLoop, ViewerEnd)

	StENG_RVW_Session g_RVW ;

	//<--------------SessionType = RVW_LOOP

	//-------------->SessionType = REPORT_DESIGNER

	ClENG_SessReportDesigner *g_poRD ;

	//<--------------SessionType = REPORT_DESIGNER

	//<================================>
};


#ifdef _INCLUDING_FROM_ENGINDLL_

#include "sysinfo.h"

/*
PlugIn
----------------------------------------------------------------------------------------------
*/

StENG_RVW_Session::StENG_RVW_Session() 
{
	m_poReport=NULL ;
	m_poFile=NULL ;
}

static StMIT_CompPlugIn * g_ENG_GetSonPlugIn(m4int16_t ai_Index) 
{
	return ai_Index==0 ? &g_FONT_M_pstPlugIn : NULL ;
}

static void g_ENG_InitMemProf( ClMIT_MemProf *ai_poMemProf) 
{
	ai_poMemProf->Init(ENG_MGI_COUNT, MIT_MGI_SHARED_COUNT,g_MIT_pcSharedMG,g_ENG_pcMG) ;
	ai_poMemProf->SetCheckPointAtEnd(M4_TRUE) ;
}

static void g_ENG_InitSysInfo( ClMIT_SysInfo *ai_poSysInfo)
{
	ClMIT_Str Path;

	ai_poSysInfo->Init();
	ai_poSysInfo->PrepareRegistryPath(&Path);
	ai_poSysInfo->NewValue(ENG_RK_RFN_PATH, Path.InChar(0), "RFNPath");
	Path.End();

	ai_poSysInfo->PrepareRegistryPath(&Path);
	ai_poSysInfo->NewValue(ENG_RK_WORKDIR, Path.InChar(0), "WorkDir");
	Path.End();

	ai_poSysInfo->PrepareRegistryPath(&Path);
	ai_poSysInfo->NewValue(ENG_RK_GRAPHICS_PATH, Path.InChar(0), "GraphicsPath");
	Path.End();

	ai_poSysInfo->PrepareRegistryPath(&Path);
	ai_poSysInfo->NewValue(ENG_RK_GRAPHICS_PATH_BW, Path.InChar(0), "GraphicsPathBW");
	Path.End();

	ai_poSysInfo->PrepareRegistryPath(&Path);
	ai_poSysInfo->NewValue(APISQL_RK_SMOLTP, Path.InChar(0), "GraphicsPathBW");
	Path.End();

	ai_poSysInfo->PrepareRegistryPath(&Path);
	ai_poSysInfo->NewValue(ENG_RK_GRAPHICS_PATH_HTML, Path.InChar(0), "GraphicsPathHTML");
	Path.End();

	ai_poSysInfo->PrepareRegistryPath(&Path);
	ai_poSysInfo->NewValue(ENG_RK_TEMP_WEB_IMAGES, Path.InChar(0), "TempWebImages");
	Path.End();

	ai_poSysInfo->PrepareRegistryPath(&Path);
	ai_poSysInfo->NewValue(ENG_RK_UNICODE_FONT_FILENAME, Path.InChar(0), "UnicodeFontFileName");
	Path.End();
}


static void g_ENG_InitTrace( ClMIT_Trace   *ai_poTrace, 
		const m4char_t *ai_pcLocalTraceFile, const m4char_t *ai_pcM4INIFile) 
{

	ai_poTrace->Init(
		"RPT", ENG_STR_ID,
		ai_pcLocalTraceFile,ai_pcM4INIFile, 
		g_pc_EngineTraceGroupNames, ENG_TGI_PARTIAL_COUNT ) ;
}

static void * g_ENG_NewThreadEnv() 
{
	return new StENG_ThreadEnv ;
}

static  void g_ENG_DeleteThreadEnv(void *ai_poEnv) 
{
	StENG_ThreadEnv * R = (StENG_ThreadEnv *)ai_poEnv ; 

	delete R ;
}

static void * g_ENG_NewTaskEnv() 
{
	StENG_TaskEnv* R ;

	M4_NEW_ITEM( GET_INSP()->m_poMemProf, MIT_MGI_OTHERS, R, StENG_TaskEnv ) ;
	R->g_poRD = NULL ;

	return R ;
}

static  void g_ENG_DeleteTaskEnv(void *ai_poEnv) 
{
	StENG_TaskEnv * R = (StENG_TaskEnv *)ai_poEnv ; 

	M4_DELETE_ITEM( GET_INSP()->m_poMemProf, MIT_MGI_OTHERS, R, StENG_TaskEnv ) ;
}

StMIT_CompPlugIn g_ENG_pstPlugIn(  
	ENG_ID,
	ENG_STR_ID,
	g_ENG_GetSonPlugIn,
	g_ENG_InitSysInfo,
	g_ENG_InitMemProf,
	g_ENG_InitTrace,
	g_ENG_NewTaskEnv,
	g_ENG_DeleteTaskEnv,
	g_ENG_NewThreadEnv,
	g_ENG_DeleteThreadEnv
	
) ;

#endif

#endif


