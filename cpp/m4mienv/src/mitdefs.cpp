
//SOLO DEBE SER INCLUIDO DESDE CPPs, PARA NO EXPORTAR NUNCA ESTOS MACROS A OTROS COMPONENTES 

/*
Grupos de traza y memoria del componente MIT
----------------------------------------------------------------------------------------------
*/

#include "m4types.hpp"
#include "m4res.hpp"
#include "mit.h"

#define MIT_ID 0
#define MIT_STR_ID "MIT"

//m4uint32_t g_uiBaseInit = M4_RPT_MIENV_ERROR_BASE ;	//Definido en m4res.hpp
//m4char_t   g_pcINI_Range[] = "MIEnvDLL" ;	//Nombre de este componente para rango en el INI 


enum MIT_TRACE_GROUPS {
	MIT_TGI_COUNT=MIT_TGI_NOT_SHARED_FIRST
} ;

//Falta por hacer el append.ini de estas bases
enum MIT_TRACE_BASES {
	MIT_TGB_CL_STR=4259840,
	MIT_TGB_TRACE=4259841,
	MIT_TGB_MEMPROF=4259857,
	MIT_TGB_MIT=4259866,
	MIT_TGB_TEMPORAL=4259877,
	MIT_TGB_FILE=4259878,
	MIT_TGB_CL_ARGS=4259887
} ;

enum MIT_MEM_GROUPS {
	MIT_MGI_GUARRO=MIT_MGI_NOT_SHARED_FIRST,
	MIT_MGI_COUNT
} ;

#ifdef _INCLUDING_FROM_MIT_
//Ojo, los indices deben ser los enum de MIT_MEM_GROUPS
const m4char_t * g_MIT_pcSharedMG[MIT_MGI_SHARED_COUNT] = { 
	"Clase ClMIT_Str",
	"Varios",
	"Redirecciones debug"
} ;
#endif

#ifdef _INCLUDING_FROM_MIT_
static const m4char_t * g_MIT_pcNotSharedMG[] = { 
	"Guarreria, quitame"
} ;
#endif

/*
MIT Thread Environment
----------------------------------------------------------------------------------------------
*/

struct StMIT_ThreadEnv {

	//-------------->Propietaria ClMIT_File 

	//Para comparar ficheros.

	enum { File_COMPARE_SIZE=512*50 } ;	//512 es un tamaño usual de sector
	m4char_t File_pcBuffA[File_COMPARE_SIZE],  
		File_pcBuffB[File_COMPARE_SIZE];

} ;

/*
Macros de acceso a MIT
----------------------------------------------------------------------------------------------
*/

#define GET_INSP() g_oMIT_P.GetInsp(MIT_ID)
#define GET_TM()   g_oMIT_P.GetTM()
#define TM_GET_INSP(TM) (TM)->GetInsp(MIT_ID)
#define SPAWN_TASK(p1)  g_oMIT_P.SpawnTask((p1))
#define KILL_TASK(p1)  g_oMIT_P.KillTask((p1))

#define GET_THREAD_ENV() (StMIT_ThreadEnv*)GET_TM()->GetThreadEnv(MIT_ID)


