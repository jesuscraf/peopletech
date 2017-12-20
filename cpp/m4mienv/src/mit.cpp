//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             MIT.cpp
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


// *.H Meta4

#include "m4define.hpp"
#include "m4types.hpp"
#include "m4string.hpp"

//Cabecera

#define _INCLUDING_FROM_MIT_
#include "mit.h"

#include "mitdefs.cpp"

//#include "trcgroup.h"
#include "mit.hmg"

#include "sysinfo.h"

ClMIT_PM M4_DECL_M4MIENV g_oMIT_P ; 
StMIT_CompPlugIn M4_DECL_M4MIENV *g_MIT_pstPlugIn ;


//=========================================================================== g_MIT_pstPlugIn

static StMIT_CompPlugIn * g_MIT_GetSonPlugIn(m4int16_t ai_Index) 
{
	return NULL ;
}

static void g_MIT_InitSysInfo( ClMIT_SysInfo *ai_poSysInfo) 
{
	ClMIT_Str Path;

	ai_poSysInfo->Init() ;
	ai_poSysInfo->PrepareRegistryPath(&Path);
	ai_poSysInfo->NewValue( MIT_RK_TRACE_FILE, Path.InChar(0), "TraceFile" ) ;
	Path.End();

	ai_poSysInfo->PrepareRegistryPath(&Path);
	ai_poSysInfo->NewValue(MIT_RK_SYSTEM_DEBUG_ENABLE, Path.InChar(0), "SystemDebugEnable");
	Path.End();

	ai_poSysInfo->PrepareRegistryPath(&Path);
	ai_poSysInfo->NewValue(MIT_RK_WORKDIR, Path.InChar(0), "WorkDir");
	Path.End();
}

static void g_MIT_InitMemProf( ClMIT_MemProf *ai_poMemProf) 
{
	ai_poMemProf->Init(MIT_MGI_COUNT, MIT_MGI_SHARED_COUNT, g_MIT_pcSharedMG,g_MIT_pcNotSharedMG) ;
	ai_poMemProf->SetCheckPointAtEnd(M4_TRUE) ;
}

static void g_MIT_InitTrace( ClMIT_Trace   *ai_poTrace, 
		const m4char_t *ai_pcLocalTraceFile, const m4char_t *ai_pcM4INIFile) 
{
	//#ifdef MIT_TRACE
	ai_poTrace->Init("RPT", MIT_STR_ID, ai_pcLocalTraceFile,ai_pcM4INIFile, NULL, 0 ) ;
	//ai_poTrace->Init(NULL,				ai_pcM4INIFile, MIT_TGI_COUNT ) ;
	//#endif
}

static void * g_MIT_NewThreadEnv() 
{
	return new StMIT_ThreadEnv ;
}

static  void g_MIT_DeleteThreadEnv(void *ai_poEnv) 
{
	StMIT_ThreadEnv * R = (StMIT_ThreadEnv *)ai_poEnv ; 

	delete R ;
}

//============================================================================== ClMIT_PM

ClMIT_PM::ClMIT_PM() 
{
	m4bool_t SwSemOk=M4_TRUE ;

	//Sin mutex; constructor de un objeto estatico comun a todos threads, solo se 
	//llama una vez al levantar el primer thread.

	//-------->Inicializacion de miembros

	SwSemOk = M4_BOOL( SwSemOk & ( m_oSpawnKillMutex.Init()== M4_SUCCESS )) ;	
	SwSemOk = M4_BOOL( SwSemOk & ( m_oUpdatingTList.Init()== M4_SUCCESS )) ;	
	SwSemOk = M4_BOOL( SwSemOk & ( m_stGetSynchro.m_oDoingFixUndo.Init()== M4_SUCCESS )) ;	


	PInspForTT_Fix() ;		//No hacerlo antes de inicializar los semaforos

	m_oTList.clear() ;
	m_uiTasksRequested = 0 ;
	m_uiMaxThreads=0 ;

	//------->Creacion PlugIn

	g_MIT_pstPlugIn = new StMIT_CompPlugIn(  
		MIT_ID,
		MIT_STR_ID,
		g_MIT_GetSonPlugIn,
		g_MIT_InitSysInfo,
		g_MIT_InitMemProf,
		g_MIT_InitTrace,
		NULL,
		NULL,
		g_MIT_NewThreadEnv,
		g_MIT_DeleteThreadEnv
	) ;

	//-------->Creacion de PInsp (a partir de aqui, Insp por defecto).

	m_stInsp.m_poEnv = &m_oInspEnv ;
	m_stInsp.m_poMemProf = new ClMIT_MemProf ;
	m_stInsp.m_poSysInfo = new ClMIT_SysInfo ;
	m_stInsp.m_poTrace   = new ClMIT_Trace ;
	
	InitInspector( &m_stInsp, MIT_NULL_ID, MIT_NULL_ID, g_MIT_pstPlugIn ) ;

	m_stInsp.m_poTrace->SetGroupDetail( MIT_TGI_MIT,ClMIT_Msg::GENERAL) ;
	m_stInsp.m_poTrace->m_oOutputFlags[ClMIT_Msg::BE_OUT].On(ClMIT_Trace::FLUSH_SBS|ClMIT_Trace::ID_DESC_OUT) ;
	m_stInsp.m_poTrace->m_oOutputFlags[ClMIT_Msg::FE_OUT].On(ClMIT_Trace::FLUSH_SBS|ClMIT_Trace::ID_DESC_OUT) ;
	
	#ifdef MIT_TRACE
	if(!m_stInsp.m_poTrace->IsTraceCodeRipped()){
		*m_stInsp.m_poTrace << ClMIT_Msg( GROUP_ID,"ClMIT_PM::ClMIT_PM()",ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG ;
		*m_stInsp.m_poTrace << ClMIT_Msg( GROUP_ID,"Creating PInpector" ) << DETAILED_MSG << SEND_MSG ;
	};
	#endif

	//-------->Testeo semaforos

	if (!SwSemOk) {
		*m_stInsp.m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"ClMIT_PM::ClMIT_PM() Error creating Mutex objects !!!!" ) << SEND_MSG ;
		m_stInsp.m_poTrace->ImmediateError() ;
	} ;

	//-------->
	
	#ifdef MIT_TRACE
	if(!m_stInsp.m_poTrace->IsTraceCodeRipped()){
		*m_stInsp.m_poTrace << ClMIT_Msg( GROUP_ID,"ClMIT_PM::ClMIT_PM()",ClMIT_Msg::EXIT_FORMAT) << SEND_MSG ;
	};
	#endif

	PInspForTT_Undo() ;

}

ClMIT_PM::~ClMIT_PM() 
{
	ThreadList_t::iterator It ;

	//Sin mutex; destructor de un objeto estatico comun a todos threads, solo se 
	//llama una vez al destruir el ultimo thread

	PInspForTT_Fix() ;

	#ifdef MIT_TRACE
	if(!m_stInsp.m_poTrace->IsTraceCodeRipped()){
		*m_stInsp.m_poTrace << ClMIT_Msg( GROUP_ID,"ClMIT_PM::~ClMIT_PM()", ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG ;
	};
	#endif

	//----------->Test todos TLSs borrados

	if ( m_oTList.size() > 0 ) {

		for ( It=m_oTList.begin(); It!=m_oTList.end(); ++It ) {
			*m_stInsp.m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_WARNING, GROUP_ID, 
				"Destroying thread not deleted" ) << SEND_MSG ;
		} ;
		*m_stInsp.m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 
			"Threads not deleted when the Thread Manager is being destroyed" ) << SEND_MSG ;
		m_stInsp.m_poTrace->ImmediateError() ;

	} ;

	//----------->Destruccion PInsp

	#ifdef MIT_TRACE
	if(!m_stInsp.m_poTrace->IsTraceCodeRipped()){
		*m_stInsp.m_poTrace << ClMIT_Msg( GROUP_ID,"Destroying PInpector" ) << DETAILED_MSG << SEND_MSG ;
		*m_stInsp.m_poTrace << ClMIT_Msg( GROUP_ID,"ClMIT_PM::~ClMIT_PM()", ClMIT_Msg::EXIT_FORMAT) << SEND_MSG ;
	};
	#endif

	delete m_stInsp.m_poSysInfo ;
	delete m_stInsp.m_poMemProf ;
	delete m_stInsp.m_poTrace  ;

	//----------->Destruccion PlugIn

	delete g_MIT_pstPlugIn ;

	PInspForTT_Undo() ;
}

void ClMIT_PM::InitInspector( StMIT_Inspector *ai_poInsp, MIT_Id_t ai_TaskId, MIT_Id_t ai_CompId, StMIT_CompPlugIn *ai_poPlugIn) 
{

	//Info Owner

	ai_poInsp->m_TaskId = ai_TaskId ;
	ai_poInsp->m_CompId = ai_TaskId == MIT_NULL_ID ? MIT_NULL_ID : ai_CompId  ;


	//Init grupos de memoria

	if ( ai_poPlugIn->m_pfInitMemProf ) {
		(*ai_poPlugIn->m_pfInitMemProf)(ai_poInsp->m_poMemProf) ;
	} else {
		ai_poInsp->m_poMemProf->Init(MIT_MGI_SHARED_COUNT, MIT_MGI_SHARED_COUNT, g_MIT_pcSharedMG,NULL) ;
		ai_poInsp->m_poMemProf->SetCheckPointAtEnd(M4_TRUE) ;
	} ;

	//Init SysInfo

	if ( ai_poPlugIn->m_pfInitSysInfo ) {
		(*ai_poPlugIn->m_pfInitSysInfo)(ai_poInsp->m_poSysInfo) ;
	} else {
		ai_poInsp->m_poSysInfo->Init() ;
	} ;

	//Añado a SysInfo la clave obligatoria

	ClMIT_Str Path;

	ai_poInsp->m_poSysInfo->PrepareRegistryPath(&Path);
	ai_poInsp->m_poSysInfo->NewValue(MIT_RK_LOG_INI_FILE, Path.InChar(0), "LogIniFile") ;

	//Y MIT_RK_TRACE_FILE y MIT_RK_SYSTEM_DEBUG_ENABLE si no se metio

	if ( !ai_poInsp->m_poSysInfo->ExistValue(MIT_RK_TRACE_FILE) ) {
		ai_poInsp->m_poSysInfo->NewValue( MIT_RK_TRACE_FILE, Path.InChar(0), "TraceFile" ) ;
	} ;

	if ( !ai_poInsp->m_poSysInfo->ExistValue(MIT_RK_SYSTEM_DEBUG_ENABLE) ) {
		ai_poInsp->m_poSysInfo->NewValue( MIT_RK_SYSTEM_DEBUG_ENABLE, Path.InChar(0), "SystemDebugEnable" ) ;
	} ;

	Path.End();

	//Formo los nombres de los ficheros Trace

	{	//Llaves para crear ClStr con MemProf y SysInfo ya inicializados

		ClMIT_Str TraceFile, TraceSufijo, INIFile ;

		ai_poInsp->m_poSysInfo->GetValue( MIT_RK_TRACE_FILE, &TraceFile) ;
		ai_poInsp->m_poSysInfo->GetValue( MIT_RK_LOG_INI_FILE, &INIFile) ;

		TraceSufijo.Accommodate(300) ;
		if (  ai_TaskId != MIT_NULL_ID ) {
			sprintf(TraceSufijo,"_%lu_%s", (unsigned long)ai_TaskId, ai_poPlugIn->m_pcCompId ) ;
		} else {
			TraceSufijo.StrCpy("_PManager") ;
		} ;

		TraceSufijo.OverStringValid() ;

		ClMIT_SysInfo::AppendToFile( &TraceFile, TraceSufijo ) ;

		//Inicializo el Trace
	
		if ( ai_poPlugIn->m_pfInitTrace ) {
			(*ai_poPlugIn->m_pfInitTrace)(ai_poInsp->m_poTrace,TraceFile,INIFile) ;
		} else {
			ai_poInsp->m_poTrace->Init("RPT",MIT_STR_ID,TraceFile,INIFile, NULL, 0 ) ;
		} ;

	} ;
}


StMIT_Inspector * ClMIT_PM::GetInsp(MIT_Id_t ai_CompId) 
{
	StMIT_Inspector * R ;
	ThreadList_t::iterator It ;
	m4iden_t ThreadId ;

	m_oUpdatingTList.Lock() ;
	m_stGetSynchro.m_oDoingFixUndo.Lock() ;

	ThreadId = M4Thread::GetCurrentIdThread() ;

	if (m_stGetSynchro.m_bFixActivated && m_stGetSynchro.m_oFixTId==ThreadId ) {

		R=m_stGetSynchro.m_pstFixInsp ;

	} else {

		It = m_oTList.find(ThreadId) ;	
		if (It==m_oTList.end()) {
			*m_stInsp.m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MIDEFINE_20 ] ) 
				<< "ClMIT_PM::GetInsp()" << SEND_MSG ;
			m_stInsp.m_poTrace->ImmediateError() ;
			R=NULL ;
		} else {
			R=(*It).second->GetInsp(ai_CompId) ;
		} ;
	} ;


	m_stGetSynchro.m_oDoingFixUndo.Unlock() ;
	m_oUpdatingTList.Unlock() ;

	return R ;
}

ClMIT_TM * ClMIT_PM::GetTM() 
{
	ClMIT_TM *R ;
	ThreadList_t::iterator It ;
	m4iden_t ThreadId ;

	m_oUpdatingTList.Lock() ;
	m_stGetSynchro.m_oDoingFixUndo.Lock() ;

	ThreadId = M4Thread::GetCurrentIdThread() ;
	
	It = m_oTList.find(ThreadId) ;	
	if (It == m_oTList.end())
	{
		// *m_stInsp.m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MIDEFINE_20 ] ) << "ClMIT_PM::GetInsp()" << SEND_MSG ;
		// m_stInsp.m_poTrace->ImmediateError() ;
		cout << "Thread ID not found, ClMIT_PM::GetTM() method." << endl ;
		R=NULL ;
	}
	else
	{
		R=(*It).second ;
		
		if (m_stGetSynchro.m_bFixActivated && m_stGetSynchro.m_oFixTId == ThreadId)
		{
			// *m_stInsp.m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MIDEFINE_90 ] ) << "ClMIT_PM::GetInsp()" << SEND_MSG ;
			// m_stInsp.m_poTrace->ImmediateError() ;
			// R=NULL ;
			cout << "Attempting to get a TM in an area with limited access for the inspector." << endl ;
		}
	}
	
	m_stGetSynchro.m_oDoingFixUndo.Unlock() ;
	m_oUpdatingTList.Unlock() ;

	return R ;
}



void ClMIT_PM::PInspForTT_Fix(StMIT_Inspector *ai_pstInsp) 
{
	m_stGetSynchro.m_oDoingFixUndo.Lock() ;
	m_stGetSynchro.m_bFixActivated=M4_TRUE ;
	m_stGetSynchro.m_oFixTId=M4Thread::GetCurrentIdThread() ;
	m_stGetSynchro.m_pstFixInsp = ai_pstInsp ? ai_pstInsp : &m_stInsp ;
	m_stGetSynchro.m_oDoingFixUndo.Unlock() ;
}

void ClMIT_PM::PInspForTT_Undo() 
{
	m_stGetSynchro.m_oDoingFixUndo.Lock() ;
	m_stGetSynchro.m_bFixActivated=M4_FALSE ;
	m_stGetSynchro.m_oDoingFixUndo.Unlock() ;
}


MIT_Id_t ClMIT_PM::SpawnTask( StMIT_CompPlugIn *ai_pstPlugIn) 
{
	ClMIT_TM *TM ; 
	ThreadList_t::iterator TMIt ;
	m4iden_t ThreadId ;
	MIT_Id_t TaskId ;

	//------->Fijar secciones criticas y PInsp

	m_oSpawnKillMutex.Lock() ;	

	PInspForTT_Fix() ;

	#ifdef MIT_TRACE
	if(!m_stInsp.m_poTrace->IsTraceCodeRipped()){
		*m_stInsp.m_poTrace << ClMIT_Msg( GROUP_ID,"ClMIT_PM::SpawnTask()", ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG ;
	};
	#endif

	//------->Obtener TM, o crear uno si no existe

	ThreadId = M4Thread::GetCurrentIdThread() ;

	TMIt = m_oTList.find(ThreadId) ;	
	if (TMIt==m_oTList.end()) {

		//------>Creacion TM 

		#ifdef MIT_TRACE
		if(!m_stInsp.m_poTrace->IsTraceCodeRipped()){
			*m_stInsp.m_poTrace << ClMIT_Msg( GROUP_ID, "Creating new TM" ) << DETAILED_MSG << SEND_MSG ;
		};
		#endif

		M4_NEW_ITEM( m_stInsp.m_poMemProf, MIT_MGI_OTHERS, TM, ClMIT_TM ) ;

		m_oUpdatingTList.Lock() ;
		m_oTList.insert(ClMIT_PM::ThreadList_t::value_type(ThreadId,TM)) ;
		m_oUpdatingTList.Unlock() ;

		m_uiMaxThreads=M4_MAX(m_uiMaxThreads,m_oTList.size()) ;

		TM->Init() ;	//Se realiza con objeto ya insertado en lista

	} else {

		//------>TM ya existe

		TM = (*TMIt).second ;
	} ;
	
	//------->Creacion de la TL. Es una tarea del TM

	++m_uiTasksRequested ;

	TaskId = m_uiTasksRequested ;

	TM->SpawnTask(TaskId,ai_pstPlugIn) ;
	TM->CreateTaskEnv(TaskId) ;


	//------->Desbloquear secciones criticas y PInsp

	#ifdef MIT_TRACE
	if(!m_stInsp.m_poTrace->IsTraceCodeRipped()){
		*m_stInsp.m_poTrace << ClMIT_Msg( GROUP_ID,"ClMIT_PM::SpawnTask(), TaskId %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< TaskId << SEND_MSG ;
	};
	#endif

	PInspForTT_Undo() ;

	m_oSpawnKillMutex.Unlock() ;

	return TaskId ;
}


void ClMIT_PM::KillTask( MIT_Id_t ai_TaskId ) 
{
	ClMIT_TM *TM ; 
	ThreadList_t::iterator TMIt ;
	m4iden_t ThreadId ;

	//------->Fijar secciones criticas y PInsp

	m_oSpawnKillMutex.Lock() ;	

	PInspForTT_Fix() ;

	#ifdef MIT_TRACE
	if(!m_stInsp.m_poTrace->IsTraceCodeRipped()){
		*m_stInsp.m_poTrace << ClMIT_Msg( GROUP_ID,"ClMIT_PM::KillTask(), TaskId %0:s", ClMIT_Msg::ENTRY_FORMAT) 
			<< ai_TaskId << SEND_MSG ;
	};
	#endif

	//------->Obtener TM. Debe existir 

	ThreadId = M4Thread::GetCurrentIdThread() ;

	TMIt = m_oTList.find(ThreadId) ;	
	if (TMIt==m_oTList.end()) {
		*m_stInsp.m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MIDEFINE_20 ] ) 
			<< "ClMIT_PM::KillTask()" << SEND_MSG ;
		m_stInsp.m_poTrace->ImmediateError() ;
	} else {
		TM = (*TMIt).second ;
	} ;

	//------->Destruccion de la TL. Es una tarea del TM

	TM->DestroyTaskEnv(ai_TaskId) ;
	TM->KillTask(ai_TaskId) ;

	//----->Si la tarea borrada es la actual, pner a NULL

	if ( TM->GetActiveTaskId()==ai_TaskId ) TM->SetActiveTaskId( MIT_NULL_ID ) ; 

	//------->Si ya no hay TL en la TM, se destruye el TM

	if ( TM->GetTLCount()==0 ) {

		TM->End() ;	//Se realiza con objeto aun sin borrar de la lista

		M4_DELETE_ITEM( m_stInsp.m_poMemProf, MIT_MGI_OTHERS, TM, ClMIT_TM ) ;

		m_oUpdatingTList.Lock() ;
		m_oTList.erase(TMIt) ;
		m_oUpdatingTList.Unlock() ;
	} ;

	//------->Desbloquear secciones criticas y PInsp

	#ifdef MIT_TRACE
	if(!m_stInsp.m_poTrace->IsTraceCodeRipped()){
		*m_stInsp.m_poTrace << ClMIT_Msg( GROUP_ID,"ClMIT_PM::KillTask()", ClMIT_Msg::EXIT_FORMAT) << SEND_MSG ;
	};
	#endif

	PInspForTT_Undo() ;

	m_oSpawnKillMutex.Unlock() ;

}


//============================================================================== ClMIT_TM

ClMIT_TM::ClMIT_TM() {
	m_poInsp=GET_INSP();
} ;


void ClMIT_TM::Init() 
{
	ClMIT_TM::End() ;

	m_TL_ActiveTaskId = MIT_NULL_ID ;
}

void ClMIT_TM::End() 
{
	if ( m_oTaskList.size()>0 || m_oSonTaskList.size()>0 || m_oCompList.size()>0 ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MIDEFINE_10 ] ) << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
	m_oTaskList.clear() ;
	m_oSonTaskList.clear() ;
	m_oCompList.clear() ;
}

StMIT_Inspector * ClMIT_TM::GetInsp(MIT_Id_t ai_CompId) 
{
	TL_CompList_t::iterator It ;

	It=m_oCompList.find(ai_CompId) ;
	if ( It==m_oCompList.end() ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MIDEFINE_30 ] ) 
			<< "ClMIT_TM::GetInsp()" << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
	return (*It).second->GetInsp(m_TL_ActiveTaskId) ;
}

void *ClMIT_TM::GetThreadEnv(MIT_Id_t ai_CompId) 
{
	TL_CompList_t::iterator It ;

	It=m_oCompList.find(ai_CompId) ;
	if ( It==m_oCompList.end() ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MIDEFINE_30 ] ) 
			<< "ClMIT_TM::GetThreadEnv()" << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
	return (*It).second->GetThreadEnv() ;
}

void *ClMIT_TM::GetTaskEnv(MIT_Id_t ai_CompId) 
{
	TL_CompList_t::iterator It ;

	It=m_oCompList.find(ai_CompId) ;
	if ( It==m_oCompList.end() ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MIDEFINE_30 ] ) 
			<< "ClMIT_TM::GetTaskEnv()" << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
	return (*It).second->GetTaskEnv(m_TL_ActiveTaskId) ;
}

void ClMIT_TM::InTask(MIT_Id_t ai_TaskId, MIT_Id_t *ao_PreviousTask) 
{
	*ao_PreviousTask=m_TL_ActiveTaskId ;
	m_TL_ActiveTaskId=ai_TaskId ;
}

void ClMIT_TM::OutTask(MIT_Id_t ai_PreviousTask) 
{
	m_TL_ActiveTaskId=ai_PreviousTask ;
}


void ClMIT_TM::SpawnTask(MIT_Id_t ai_TaskId, StMIT_CompPlugIn *ai_pstPlugIn) 
{
	pair< TL_TaskList_t::iterator, bool> It ;
	StTaskD TaskD ;
	
	#ifdef MIT_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClMIT_TM::SpawnTask(), Task %0:s Comp %1:s", ClMIT_Msg::ENTRY_FORMAT) 
			<< ai_TaskId << ClMIT_IdNumDesc(ai_pstPlugIn->m_CompId, ai_pstPlugIn->m_pcCompId)  
			<< CRAZY_MSG << SEND_MSG ;
	};
	#endif

	//---------Insercion m_oTaskList y m_oSonTaskList
	
	TaskD.m_TaskId = ai_TaskId ;
	TaskD.m_RootCompId = ai_pstPlugIn->m_CompId ;
	TaskD.m_OwnerTaskId = m_TL_ActiveTaskId ;

	It = m_oTaskList.insert(TL_TaskList_t::value_type(ai_TaskId,TaskD)) ;
	if ( !It.second ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MIDEFINE_70 ] ) 
			<< "ClMIT_TM::SpawnTask()" << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
	m_oSonTaskList.insert(TL_SonTaskList_t::value_type(TaskD.m_OwnerTaskId,ai_TaskId)) ;

	//-------Procesar arbol

	SpawnSubTask(ai_TaskId,ai_pstPlugIn) ;

	#ifdef MIT_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClMIT_TM::SpawnTask(), Task %0:s Comp %1:s", ClMIT_Msg::EXIT_FORMAT) 
			<< ai_TaskId << ClMIT_IdNumDesc(ai_pstPlugIn->m_CompId, ai_pstPlugIn->m_pcCompId)  
			<< CRAZY_MSG  << SEND_MSG ;
	};
	#endif
}

void ClMIT_TM::SpawnSubTask(MIT_Id_t ai_TaskId, StMIT_CompPlugIn *ai_pstPlugIn) 
{
	TL_CompList_t::iterator TCMIt ;
	ClMIT_TCM *TCM ;
	ClMIT_TCM::StTaskD *TLC_Task, TLC_TaskAux ;
	StMIT_CompPlugIn *SonPlugIn ;
	m4int16_t SonIndex ;

	#ifdef MIT_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClMIT_TM::SpawnSubTask(), Task %0:s Comp %1:s", ClMIT_Msg::ENTRY_FORMAT) 
			<< ai_TaskId << ClMIT_IdNumDesc(ai_pstPlugIn->m_CompId, ai_pstPlugIn->m_pcCompId)  
			<< CRAZY_MSG << SEND_MSG ;
	};
	#endif

	//------------Obtencion TCM

	TCMIt=m_oCompList.find(ai_pstPlugIn->m_CompId) ;
	if ( TCMIt==m_oCompList.end() ) {

		//------------TCM no existe, crear uno

		M4_NEW_ITEM( m_poInsp->m_poMemProf, MIT_MGI_OTHERS, TCM, ClMIT_TCM ) ;
		m_oCompList.insert(TL_CompList_t::value_type(ai_pstPlugIn->m_CompId,TCM)) ;
		TCM->Init(ai_pstPlugIn) ;	//Se realiza con objeto ya insertado en lista. 
									//	Crea ThreadEnv

	} else {
		
		//------------TCM existe, usarlo

		TCM=(*TCMIt).second ;
	} ;

	//----------Testeo para cortar llamada en grafo

	TLC_Task=TCM->GetTLC_Task(ai_TaskId) ;
	if ( TLC_Task ) {
		if ( TLC_Task->m_pstPlugIn != ai_pstPlugIn ) {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MIDEFINE_60 ] ) << SEND_MSG ;
			m_poInsp->m_poTrace->ImmediateError() ;
		} ;
		goto exit ;
	} ;

	//--------Creacion de tareas en Comp hijos

	for ( SonIndex=0; (SonPlugIn=(*ai_pstPlugIn->m_pfGetSonPlugIn)(SonIndex))!=NULL ;++SonIndex ) {
		SpawnSubTask(ai_TaskId, SonPlugIn) ;
	} ;

	//--------Creacion del TLC_Task. El Inspector se crea luego

	TLC_TaskAux.m_TaskId=ai_TaskId ;
	TLC_TaskAux.m_stInsp.m_poSysInfo=NULL ; TLC_TaskAux.m_stInsp.m_poTrace=NULL ;
	TLC_TaskAux.m_stInsp.m_poTrace=NULL ; TLC_TaskAux.m_stInsp.m_poEnv=NULL ;
	TLC_TaskAux.m_poTaskEnv=NULL ;
	TLC_TaskAux.m_pstPlugIn=ai_pstPlugIn ;

	TCM->NewTLC_Task( &TLC_TaskAux ) ;

exit:

	#ifdef MIT_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClMIT_TM::SpawnSubTask(), Task %0:s Comp %1:s", ClMIT_Msg::EXIT_FORMAT) 
			<< ai_TaskId << ClMIT_IdNumDesc(ai_pstPlugIn->m_CompId, ai_pstPlugIn->m_pcCompId)  
			<< CRAZY_MSG << SEND_MSG ;
	};
	#endif
	;
}

void ClMIT_TM::CreateTaskEnv(MIT_Id_t ai_TaskId) 
{
	TL_TaskList_t::iterator TaskIt ; 

	#ifdef MIT_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClMIT_TM::CreateTaskEnv(), Task %0:s", ClMIT_Msg::ENTRY_FORMAT) 
			<< ai_TaskId << CRAZY_MSG << SEND_MSG ;
	};
	#endif

	//------Localiza task para saber Comp Root, etc

	TaskIt=m_oTaskList.find(ai_TaskId) ;
	if ( TaskIt==m_oTaskList.end() ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MIDEFINE_40 ] ) 
			<<  "ClMIT_TCM::CreateTaskEnv()" << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;

	//-----Creo TaskEnv

	CreateSubTaskEnv( ai_TaskId, (*TaskIt).second.m_RootCompId ) ;

	#ifdef MIT_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClMIT_TM::CreateTaskEnv(), Task %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< ai_TaskId << CRAZY_MSG << SEND_MSG ;
	};
	#endif

}


void ClMIT_TM::CreateSubTaskEnv( MIT_Id_t ai_TaskId, MIT_Id_t ai_CompId ) 
{
	TL_CompList_t::iterator TCMIt ;
	ClMIT_TCM::StTaskD *TLC_Task ;
	m4int16_t SonIndex ;
	StMIT_CompPlugIn *SonPlugIn ;
	MIT_Id_t PrevTask ;

	ClMIT_TCM *TCM ;

	#ifdef MIT_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClMIT_TM::CreateSubTaskEnv(), Task %0:s Comp %1:s", ClMIT_Msg::ENTRY_FORMAT) 
			<< ai_TaskId << ai_CompId << CRAZY_MSG << SEND_MSG ;
	};
	#endif

	//-----Localizo TCM

	TCMIt=m_oCompList.find(ai_CompId) ;
	if ( TCMIt==m_oCompList.end() ) { 
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MIDEFINE_30 ] ) 
			<< "ClMIT_TM::CreateSubTaskEnv()" << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
	TCM=(*TCMIt).second ;

	//-----Test llamada en grafo

	TLC_Task=TCM->GetTLC_Task(ai_TaskId) ;
	if ( TLC_Task->m_stInsp.m_poEnv ) goto exit ;

	//-----m_stInsp.m_poEnv hace ademas de flag de 'pase por aqui'

	TLC_Task->m_stInsp.m_poEnv = &m_oInspEnv ;	
	
	//----Poceso hijos

	for ( SonIndex=0; (SonPlugIn=(*TLC_Task->m_pstPlugIn->m_pfGetSonPlugIn)(SonIndex))!=NULL ;++SonIndex ) {
		CreateSubTaskEnv(ai_TaskId, SonPlugIn->m_CompId) ;
	} ;

	//----Creacion de TaskInsp

	#ifdef MIT_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"Creating TaskInsp, TaskId %0:s Component %1:s") 
			<< ai_TaskId << ClMIT_IdNumDesc(
				TLC_Task->m_pstPlugIn->m_CompId, TLC_Task->m_pstPlugIn->m_pcCompId)  << DETAILED_MSG << SEND_MSG ;
	};
	#endif

	g_oMIT_P.PInspForTT_Undo() ;
	g_oMIT_P.PInspForTT_Fix(&TLC_Task->m_stInsp) ;
	InTask(ai_TaskId,&PrevTask) ;	//Cambia a Task creada

	TLC_Task->m_stInsp.m_poMemProf = new ClMIT_MemProf ;	
	TLC_Task->m_stInsp.m_poSysInfo = new ClMIT_SysInfo ;
	TLC_Task->m_stInsp.m_poTrace   = new ClMIT_Trace ;

	g_oMIT_P.InitInspector( &TLC_Task->m_stInsp, ai_TaskId, ai_CompId, TLC_Task->m_pstPlugIn ) ;

	OutTask(PrevTask) ;			//Retorna a la Task previa
	g_oMIT_P.PInspForTT_Undo() ;
	g_oMIT_P.PInspForTT_Fix() ;

	//----Creacion TaskEnv

	if ( ! TLC_Task->m_poTaskEnv && TLC_Task->m_pstPlugIn->m_pfNewTaskEnv ) {
	
		#ifdef MIT_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"Creating TaskEnv, TaskId %0:s Component %1:s") 
				<< ai_TaskId << ClMIT_IdNumDesc(
				TLC_Task->m_pstPlugIn->m_CompId, TLC_Task->m_pstPlugIn->m_pcCompId)  << DETAILED_MSG << SEND_MSG ;
		};
		#endif

		g_oMIT_P.PInspForTT_Undo() ;
		g_oMIT_P.PInspForTT_Fix(&TLC_Task->m_stInsp) ;
		InTask(ai_TaskId,&PrevTask) ;	//Cambia a Task creada

		TLC_Task->m_poTaskEnv = (*TLC_Task->m_pstPlugIn->m_pfNewTaskEnv)() ;
	
		OutTask(PrevTask) ;			//Retorna a la Task previa
		g_oMIT_P.PInspForTT_Undo() ;
		g_oMIT_P.PInspForTT_Fix() ;
	} ;

exit:
	#ifdef MIT_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClMIT_TM::CreateSubTaskEnv(), Task %0:s Comp %1:s", ClMIT_Msg::EXIT_FORMAT) 
			<< ai_TaskId << ai_CompId << CRAZY_MSG << SEND_MSG ;
	};
	#endif
	;
}


void ClMIT_TM::KillTask(MIT_Id_t ai_TaskId) 
{
	TL_TaskList_t::iterator TaskIt ; 
	TL_SonTaskList_t::iterator SonTaskIt ;
	
	#ifdef MIT_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClMIT_TM::KillTask(), Task %0:s", ClMIT_Msg::ENTRY_FORMAT) 
			<< ai_TaskId << CRAZY_MSG << SEND_MSG ;
	};
	#endif

	//---------Busca el IdTask

	TaskIt=m_oTaskList.find(ai_TaskId) ;
	if ( TaskIt==m_oTaskList.end() ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MIDEFINE_40 ] ) 
			<<  "ClMIT_TCM::KillTask()" << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;

	//--------Testea que no haya subtareas immediatas (si no las hay, tampoco puede haberlas
	//	no immediatas)
	
	SonTaskIt = m_oSonTaskList.find(ai_TaskId) ;
	if ( SonTaskIt!=m_oSonTaskList.end() ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MIDEFINE_80 ] ) << ai_TaskId << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;

	//-------Procesar arbol

	KillSubTask(ai_TaskId,(*TaskIt).second.m_RootCompId) ;

	//-------Borrar en TaskIt y referencia en tareas padre de las listas

	SonTaskIt = m_oSonTaskList.find((*TaskIt).second.m_OwnerTaskId) ;
	if ( SonTaskIt != m_oSonTaskList.end() ) m_oSonTaskList.erase(SonTaskIt) ;

	m_oTaskList.erase(TaskIt) ;

	#ifdef MIT_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClMIT_TM::KillTask(), Task %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< ai_TaskId << CRAZY_MSG << SEND_MSG ;
	};
	#endif
}

void ClMIT_TM::KillSubTask(MIT_Id_t ai_TaskId, MIT_Id_t ai_CompId) 
{
	TL_CompList_t::iterator TCMIt ;
	ClMIT_TCM *TCM ;
	ClMIT_TCM::StTaskD *TLC_Task ;
	StMIT_CompPlugIn *SonPlugIn, *AuxPlugIn ;
	m4int16_t SonIndex ;

	#ifdef MIT_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClMIT_TM::KillSubTask(), Task %0:s Comp %1:s", ClMIT_Msg::ENTRY_FORMAT) 
			<< ai_TaskId << ai_CompId << CRAZY_MSG << SEND_MSG ;
	};
	#endif

	//-----Localizo TCM

	TCMIt=m_oCompList.find(ai_CompId) ;
	if ( TCMIt==m_oCompList.end() ) { 
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MIDEFINE_30 ] ) 
			<< "ClMIT_TM::CreateSubTaskEnv()" << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
	TCM=(*TCMIt).second ;

	//-----Test llamada en grafo

	TLC_Task=TCM->GetTLC_Task(ai_TaskId) ;
	if ( !TLC_Task ) goto exit ;
	
	//----Poceso hijos

	for ( SonIndex=0; (SonPlugIn=(*TLC_Task->m_pstPlugIn->m_pfGetSonPlugIn)(SonIndex))!=NULL ;++SonIndex ) {
		KillSubTask(ai_TaskId, SonPlugIn->m_CompId) ;
	} ;

	//---Destruyo TLC

	AuxPlugIn=TLC_Task->m_pstPlugIn ;
	TCM->DeleteTLC_Task(ai_TaskId) ;	//Invalida TLC_Task

	//---Si ya no quedan TLCs, se borra el TCM (lo que provocara el borrado del ThreadEnv).

	if ( TCM->GetTLC_Count()==0 ) {
		TCM->End( AuxPlugIn  ) ;	
		M4_DELETE_ITEM( m_poInsp->m_poMemProf, MIT_MGI_OTHERS, TCM, ClMIT_TCM ) ;
		m_oCompList.erase(TCMIt) ;
	} ;

exit:

	#ifdef MIT_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClMIT_TM::KillSubTask(), Task %0:s Comp %1:s", ClMIT_Msg::EXIT_FORMAT) 
			<< ai_TaskId << ai_CompId << CRAZY_MSG << SEND_MSG ;
	};
	#endif
	;
}


void ClMIT_TM::DestroyTaskEnv(MIT_Id_t ai_TaskId) 
{
	TL_TaskList_t::iterator TaskIt ; 

	#ifdef MIT_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClMIT_TM::DestroyTaskEnv(), Task %0:s", ClMIT_Msg::ENTRY_FORMAT) 
			<< ai_TaskId << CRAZY_MSG << SEND_MSG ;
	};
	#endif

	//------Localiza task para saber Comp Root, etc

	TaskIt=m_oTaskList.find(ai_TaskId) ;
	if ( TaskIt==m_oTaskList.end() ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MIDEFINE_40 ] ) 
			<<  "ClMIT_TCM::CreateTaskEnv()" << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;

	//-----Destruyo TaskEnv

	DestroySubTaskEnv( ai_TaskId, (*TaskIt).second.m_RootCompId ) ;

	#ifdef MIT_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClMIT_TM::DestroyTaskEnv(), Task %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< ai_TaskId << CRAZY_MSG << SEND_MSG ;
	};
	#endif
}

void ClMIT_TM::DestroySubTaskEnv( MIT_Id_t ai_TaskId, MIT_Id_t ai_CompId ) 
{
	TL_CompList_t::iterator TCMIt ;
	ClMIT_TCM::StTaskD *TLC_Task ;
	m4int16_t SonIndex ;
	StMIT_CompPlugIn *SonPlugIn ;
	MIT_Id_t PrevTask ;

	ClMIT_TCM *TCM ;

	#ifdef MIT_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClMIT_TM::DestroySubTaskEnv(), Task %0:s Comp %1:s", ClMIT_Msg::ENTRY_FORMAT) 
			<< ai_TaskId << ai_CompId << CRAZY_MSG << SEND_MSG ;
	};
	#endif

	//-----Localizo TCM

	TCMIt=m_oCompList.find(ai_CompId) ;
	if ( TCMIt==m_oCompList.end() ) { 
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MIDEFINE_30 ] ) 
			<< "ClMIT_TM::CreateSubTaskEnv()" << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
	TCM=(*TCMIt).second ;

	//-----Test llamada en grafo

	TLC_Task=TCM->GetTLC_Task(ai_TaskId) ;
	if ( ! TLC_Task->m_stInsp.m_poEnv ) goto exit ; 

	//----Destruccion TaskEnv

	if ( TLC_Task->m_poTaskEnv && TLC_Task->m_pstPlugIn->m_pfDeleteTaskEnv ) {
	
		#ifdef MIT_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"Destroying TaskEnv, TaskId %0:s Component %1:s") 
				<< ai_TaskId << ClMIT_IdNumDesc(
				TLC_Task->m_pstPlugIn->m_CompId, TLC_Task->m_pstPlugIn->m_pcCompId)  << DETAILED_MSG << SEND_MSG ;
		};
		#endif

		g_oMIT_P.PInspForTT_Undo() ;
		g_oMIT_P.PInspForTT_Fix(&TLC_Task->m_stInsp) ;
		InTask(ai_TaskId,&PrevTask) ;	//Cambia a Task 

		(*TLC_Task->m_pstPlugIn->m_pfDeleteTaskEnv)(TLC_Task->m_poTaskEnv) ;
	
		OutTask(PrevTask) ;			//Retorna a la Task previa
		g_oMIT_P.PInspForTT_Undo() ;
		g_oMIT_P.PInspForTT_Fix() ;
	} ;

	//----Destruccion de TaskInsp

	#ifdef MIT_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"Destroying TaskInsp, TaskId %0:s Component %1:s") 
			<< ai_TaskId << ClMIT_IdNumDesc(
			TLC_Task->m_pstPlugIn->m_CompId, TLC_Task->m_pstPlugIn->m_pcCompId)  << DETAILED_MSG << SEND_MSG ;
	};
	#endif

	g_oMIT_P.PInspForTT_Undo() ;
	g_oMIT_P.PInspForTT_Fix(&TLC_Task->m_stInsp) ;
	InTask(ai_TaskId,&PrevTask) ;	//Cambia a Task 

	delete TLC_Task->m_stInsp.m_poSysInfo ;
	delete TLC_Task->m_stInsp.m_poMemProf ;
	delete TLC_Task->m_stInsp.m_poTrace ;
	TLC_Task->m_stInsp.m_poEnv = NULL ;	//-m_stInsp.m_poEnv hace ademas de flag de 'pase por aqui'

	OutTask(PrevTask) ;			//Retorna a la Task previa
	g_oMIT_P.PInspForTT_Undo() ;
	g_oMIT_P.PInspForTT_Fix() ;

	//----Proceso hijos

	for ( SonIndex=0; (SonPlugIn=(*TLC_Task->m_pstPlugIn->m_pfGetSonPlugIn)(SonIndex))!=NULL ;++SonIndex ) {
		DestroySubTaskEnv(ai_TaskId, SonPlugIn->m_CompId) ;
	} ;

exit:
	#ifdef MIT_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClMIT_TM::DestroySubTaskEnv(), Task %0:s Comp %1:s", ClMIT_Msg::EXIT_FORMAT) 
			<< ai_TaskId << ai_CompId << CRAZY_MSG << SEND_MSG ;
	};
	#endif
	;
}


void  ClMIT_TM::SetCrossed(MIT_Id_t ai_TaskId, MIT_Id_t ai_CompId,m4bool_t ai_bSw) 
{
	TL_CompList_t::iterator TCMIt ;
	ClMIT_TCM::StTaskD *TLC_Task ;
	m4int16_t SonIndex ;
	StMIT_CompPlugIn *SonPlugIn ;

	ClMIT_TCM *TCM ;

	//-----Localizo TCM

	TCMIt=m_oCompList.find(ai_CompId) ;
	if ( TCMIt==m_oCompList.end() ) goto exit ;
	TCM=(*TCMIt).second ;

	//-----Y ahota TLC_Task

	TLC_Task=TCM->GetTLC_Task(ai_TaskId) ;
	if ( !TLC_Task ) goto exit ;

	//-----Pone flag

	TLC_Task->m_bSwCrossed = ai_bSw ;

	//----Pone flag en hijos

	for ( SonIndex=0; (SonPlugIn=(*TLC_Task->m_pstPlugIn->m_pfGetSonPlugIn)(SonIndex))!=NULL ;++SonIndex ) {
		SetCrossed( ai_TaskId, SonPlugIn->m_CompId, ai_bSw) ;
	} ;

exit:
	;
}


void  ClMIT_TM::NotifyProblem(
	ClMIT_Trace *ai_poCaller, ClMIT_Msg::MSG_TYPE ai_Type, 
	m4uint32_t ai_uiLocalId, m4char_t *ai_pcMsg) 
{
	TL_TaskList_t::iterator TaskIt ; 

	//------Localiza task para saber Comp Root, etc

	TaskIt=m_oTaskList.find(m_TL_ActiveTaskId) ;
	if ( TaskIt==m_oTaskList.end() ) goto exit ;

	//-----Flag para evitar incursiones en arbol

	SetCrossed(m_TL_ActiveTaskId,(*TaskIt).second.m_RootCompId) ;

	//-----Notify 

	SubNotifyProblem( m_TL_ActiveTaskId, (*TaskIt).second.m_RootCompId, ai_poCaller,
		ai_Type, ai_uiLocalId, ai_pcMsg) ;
exit:
	;
}


void  ClMIT_TM::SubNotifyProblem(
	MIT_Id_t ai_TaskId, MIT_Id_t ai_CompId,
	ClMIT_Trace *ai_poCaller, ClMIT_Msg::MSG_TYPE ai_Type, 
	m4uint32_t ai_uiLocalId, m4char_t *ai_pcMsg) 
{

	TL_CompList_t::iterator TCMIt ;
	ClMIT_TCM::StTaskD *TLC_Task ;
	m4int16_t SonIndex ;
	StMIT_CompPlugIn *SonPlugIn ;

	ClMIT_TCM *TCM ;

	//-----Localizo TCM

	TCMIt=m_oCompList.find(ai_CompId) ;
	if ( TCMIt==m_oCompList.end() ) goto exit ;
	TCM=(*TCMIt).second ;

	//-----Test llamada en grafo, o alcanzada rama generadora ->EXIT

	TLC_Task=TCM->GetTLC_Task(ai_TaskId) ;
	if ( TLC_Task->m_bSwCrossed ) goto exit ; 

	if ( TLC_Task->m_stInsp.m_poTrace ) {
		if ( TLC_Task->m_stInsp.m_poTrace == ai_poCaller  ) goto exit ;
	} ;

	TLC_Task->m_bSwCrossed = M4_TRUE ;

	//----Notificacion en el actual inspector

	if ( TLC_Task->m_stInsp.m_poTrace ) {
		if ( TLC_Task->m_stInsp.m_poTrace->IsInicialized() ) 
		{
			TLC_Task->m_stInsp.m_poTrace->NotifyEC_Problem(
				ai_Type, ai_CompId, TLC_Task->m_pstPlugIn->m_pcCompId, ai_uiLocalId, ai_pcMsg) ;
		} ;
	} ;

	//----Proceso hijos

	for ( SonIndex=0; (SonPlugIn=(*TLC_Task->m_pstPlugIn->m_pfGetSonPlugIn)(SonIndex))!=NULL ;++SonIndex ) {
		SubNotifyProblem( ai_TaskId, SonPlugIn->m_CompId, ai_poCaller, ai_Type, ai_uiLocalId, ai_pcMsg) ;
	} ;

exit:
	;
}

//============================================================================== ClMIT_TCM


ClMIT_TCM::ClMIT_TCM() {
	m_poInsp=GET_INSP();
	m_poThreadEnv=NULL ;
} ;

void ClMIT_TCM::End() 
{
	if ( m_oTaskList.size()>0  ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MIDEFINE_50 ] ) << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
	m_oTaskList.clear() ;
}

void ClMIT_TCM::End(StMIT_CompPlugIn *ai_pstPlugIn) 
{
	if ( m_poThreadEnv && ai_pstPlugIn->m_pfDeleteThreadEnv ) {
		#ifdef MIT_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"Destroying ThreadEnv, Component %0:s") 
				<< ClMIT_IdNumDesc(
					ai_pstPlugIn->m_CompId, ai_pstPlugIn->m_pcCompId)  << DETAILED_MSG << SEND_MSG ;
		};
		#endif
		(*ai_pstPlugIn->m_pfDeleteThreadEnv)(m_poThreadEnv) ;
		m_poThreadEnv=NULL ;
	} ;
	ClMIT_TCM::End() ;
}


void ClMIT_TCM::Init(StMIT_CompPlugIn *ai_pstPlugIn) 
{
	ClMIT_TCM::End(ai_pstPlugIn) ;

	if ( ai_pstPlugIn->m_pfNewThreadEnv ) {
		#ifdef MIT_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"Creating ThreadEnv, Component %0:s") 
				<< ClMIT_IdNumDesc(
					ai_pstPlugIn->m_CompId, ai_pstPlugIn->m_pcCompId)  << DETAILED_MSG << SEND_MSG ;
		};
		#endif
		m_poThreadEnv = (*ai_pstPlugIn->m_pfNewThreadEnv)();
	} ;

}

StMIT_Inspector * ClMIT_TCM::GetInsp(MIT_Id_t ai_TaskId) 
{
	TLC_TaskList_t::iterator It ;

	It=m_oTaskList.find(ai_TaskId) ;
	if ( It==m_oTaskList.end() ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MIDEFINE_40 ] ) 
			<<  "ClMIT_TCM::GetInsp()" << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
	return & (*It).second.m_stInsp ;
}

void * ClMIT_TCM::GetTaskEnv(MIT_Id_t ai_TaskId) 
{
	TLC_TaskList_t::iterator It ;

	It=m_oTaskList.find(ai_TaskId) ;
	if ( It==m_oTaskList.end() ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MIDEFINE_40 ] ) 
			<<  "ClMIT_TCM::GetTaskEnv()" << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
	return (*It).second.m_poTaskEnv ;
}

ClMIT_TCM::StTaskD * ClMIT_TCM::GetTLC_Task(MIT_Id_t ai_TaskId) 
{
	TLC_TaskList_t::iterator It ;
	It=m_oTaskList.find(ai_TaskId) ;
	return It==m_oTaskList.end() ? NULL : &(*It).second ;
}

void ClMIT_TCM::NewTLC_Task( ClMIT_TCM::StTaskD * ai_poData ) 
{
	pair<TLC_TaskList_t::iterator,bool> It ;

	It = m_oTaskList.insert(TLC_TaskList_t::value_type(ai_poData->m_TaskId,*ai_poData)) ;
	if ( !It.second ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MIDEFINE_70 ] ) 
			<< "ClMIT_TCM::NewTLC_Task()" << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
}

m4uint32_t ClMIT_TCM::GetTLC_Count() 
{
	return m_oTaskList.size() ; 
}

void ClMIT_TCM::DeleteTLC_Task( MIT_Id_t ai_TaskId  ) 
{
	TLC_TaskList_t::iterator It ;

	It=m_oTaskList.find(ai_TaskId) ;
	if ( It==m_oTaskList.end() ) 	{
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ MIDEFINE_40 ] ) 
			<<  "ClMIT_TCM::DeleteTLC_Task()" << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
	m_oTaskList.erase(It) ;
}


//========================================================================== StMIT_CompPlugIn 


StMIT_CompPlugIn::StMIT_CompPlugIn(
		MIT_Id_t ai_CompId, m4char_t * ai_pcCompId,
		StMIT_CompPlugIn::GetSonPlugIn_t ai_pfGetSonPlugIn,	

		StMIT_CompPlugIn::InitSysInfo_t ai_pfInitSysInfo,
		StMIT_CompPlugIn::InitMemProf_t ai_pfInitMemProf,
		StMIT_CompPlugIn::InitTrace_t   ai_pfInitTrace,


		StMIT_CompPlugIn::NewTaskEnv_t	ai_pfNewTaskEnv, 
		StMIT_CompPlugIn::DeleteTaskEnv_t ai_pfDeleteTaskEnv,

		StMIT_CompPlugIn::NewThreadEnv_t    ai_pfNewThreadEnv,
		StMIT_CompPlugIn::DeleteThreadEnv_t ai_pfDeleteThreadEnv
) 
{
	m_CompId=ai_CompId;
	if ( strlen(ai_pcCompId)>=40 ) {
		strncpy(m_pcCompId,ai_pcCompId,39) ;
		m_pcCompId[39]=M4_END_STR ;
	} else {
		strcpy(m_pcCompId,ai_pcCompId) ;
	} ;

	m_pfInitSysInfo=ai_pfInitSysInfo ;
	m_pfInitMemProf=ai_pfInitMemProf ;
	m_pfInitTrace=ai_pfInitTrace ;

	m_pfGetSonPlugIn=ai_pfGetSonPlugIn;

	m_pfNewTaskEnv=ai_pfNewTaskEnv ;
	m_pfDeleteTaskEnv=ai_pfDeleteTaskEnv ;

	m_pfNewThreadEnv=ai_pfNewThreadEnv ;
	m_pfDeleteThreadEnv=ai_pfDeleteThreadEnv ;
}






