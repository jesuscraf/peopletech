//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             dllmain.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:09/07/1997
// Language:         C++
// Operating System: WINDOWS (Entry Points de la DLL)
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


#include "eng.h"
#define _INCLUDING_FROM_ENGINDLL_
#include "engdf.inl"

#include "clstr.h"

#include "xapichn.h"
#include "clreparg.h"
#include "cldvcfi.h"
#include "dm.hpp"
//#include "vmdefine.hpp"
#include "access.hpp"
//#include "vmachin1.h"
#include "cldates.hpp"

#include "ddlf_chn.h"
#include "designdd.h"
#include "designdt.h"
#include "sess_rd.h"
#include "rasterg.h"

#include "m4fmt.hpp"	//Para el Attach y Dettach

#include "engindll.h"

#include "m4unicode.hpp"

#define GROUP_ID ENG_TGI_WRAPVB


//----------------------------------------Auxiliares

static m4bool_t LoadParamsLoadMVC(
	StMIT_Inspector* ai_poInsp, StENG_TaskEnv * ai_TaskEnv,
	ClENG_ReportArgs *ao_poExeParams,
	const m4char_t * ai_pcCommandString, 
	m4int16_t ai_iLinkManagerMode, m4int16_t ai_iLinkDesignMode, m4int16_t ai_iLinkDataMode,
	ClAccess *ai_poVMAccess, ClAccess *ai_poDesignAccess, ClAccess *ai_poDataAccess
							 )
{
	m4bool_t SwOk=M4_TRUE ;

	//---------------------Actualización TaskEnv

	//Conexion con el environment de la M4Fmt. 
	//Ojo, no debe de hacerse en la inicializacion del objeto global.

	M4FMTAttachM4Fmt(&(ai_TaskEnv->m_uiFmtTaskId) ) ;	//Fijarse que las asignaciones pertenecen al Manager

	M4_NEW_ITEM(ai_poInsp->m_poMemProf, MIT_MGI_APICHN, ai_TaskEnv->m_APIChn.m_poMVC, ClMIT_MVC) ;

	//---------------------Carga ExeParams, parte que viene por cadena

	SwOk = ao_poExeParams->Init( ai_pcCommandString, ClMIT_Trace::PROPAGATE_ERROR ) ;
	if (!SwOk) goto exit ;

	//---------------------Carga ExeParams, parte que viene por parametro

	switch (ai_iLinkDataMode)
	{
	case 0:
		ao_poExeParams->m_LinkData=ClENG_ReportRequest::BUILD_DATA ;
		break ;

	case 1:
		ao_poExeParams->m_LinkData=ClENG_ReportRequest::LINK_LOAD_DATA ;
		ao_poExeParams->m_oDataContext.m_pAccess=ai_poDataAccess ;
		break ;

	case 2:
		ao_poExeParams->m_LinkData=ClENG_ReportRequest::LINK_DATA ;
		ao_poExeParams->m_oDataContext.m_pAccess=ai_poDataAccess ;
		break ;

	case 3:
		ao_poExeParams->m_LinkData=ClENG_ReportRequest::LINK_LOAD_UNLOAD_DATA ;
		ao_poExeParams->m_oDataContext.m_pAccess=ai_poDataAccess ;
		break ;

	case 4:
		ao_poExeParams->m_LinkData=ClENG_ReportRequest::LINK_UNLOAD_DATA ;
		ao_poExeParams->m_oDataContext.m_pAccess=ai_poDataAccess ;
		break ;
	} ;

	switch (ai_iLinkDesignMode)
	{
	case 0:
		ao_poExeParams->m_LinkDesign=ENG_BUILD_DESIGN ;
		break ;

	case 1:
		ao_poExeParams->m_LinkDesign=ENG_LINK_LOAD_DESIGN ;
		ao_poExeParams->m_oDesignContext.m_pAccess=ai_poDesignAccess;
		break ;

	case 2:
		ao_poExeParams->m_LinkDesign=ENG_LINK_DESIGN ;
		ao_poExeParams->m_oDesignContext.m_pAccess=ai_poDesignAccess;
		break ;

	case 3:
		ao_poExeParams->m_LinkDesign=ENG_LINK_LOAD_UNLOAD_DESIGN ;
		ao_poExeParams->m_oDesignContext.m_pAccess=ai_poDesignAccess;
	break ;

	case 4:
		ao_poExeParams->m_LinkDesign=ENG_LINK_UNLOAD_DESIGN ;
		ao_poExeParams->m_oDesignContext.m_pAccess=ai_poDesignAccess;
		break ;
	} ;

	switch (ai_iLinkManagerMode)
	{
	case 0:
		ao_poExeParams->m_LinkManager=ClENG_ReportRequest::BUILD_MANAGER ;
		break ;

	case 1:
		ao_poExeParams->m_LinkManager=ClENG_ReportRequest::LINK_MANAGER ;
		ao_poExeParams->m_oManagerContext.m_pAccess=ai_poVMAccess ;
		break ;
	} ;

	//------------Enganche con MVC

	switch(ao_poExeParams->m_LinkManager) {
	case ClENG_ReportRequest::BUILD_MANAGER:
		SwOk = M4_BOOL( SwOk && ai_TaskEnv->m_APIChn.m_poMVC->Init( ClMIT_Trace::PROPAGATE_ERROR ));
		SwOk = M4_BOOL( SwOk && ai_TaskEnv->m_APIChn.m_poMVC->LogOn("M3CPT","M3CPT",2));			
		break;
	case ClENG_ReportRequest::LINK_MANAGER:
		SwOk = M4_BOOL( SwOk && ai_TaskEnv->m_APIChn.m_poMVC->Init( ClMIT_MVC::GetChannelManagerFromRunContext(
						&ao_poExeParams->m_oManagerContext), ClMIT_Trace::PROPAGATE_ERROR ));
		break;
	} ;
exit:
	return SwOk ;
}



//Da false si error. 
//Llamar tras SPAW_IN. 
//Engancha MVC y con DLL de formateos. No actualiza SessionId, es cosa nuestra.  
//Carga diseño Report, lanza PrepareExecution.

static m4bool_t PrepareExecution(
	ClENG_ReportArgs *ai_poExeParams,
	ClENG_Report *ao_poReport )
{
	ClENG_DDLF_Chn   DDLF_Chn ;
	ClENG_DesignData DD ;
	ClENG_DesignTree DT ;

	m4bool_t SwOk=M4_TRUE ;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV( GET_TM() ) ;

	//------------Ejecuta el report

	//Limpia el DD

	DD.Init() ;	

	//Inicia DDLF_Chn
	
	SwOk = M4_BOOL( SwOk && DDLF_Chn.Init(
					ai_poExeParams->m_oReportId, ai_poExeParams->m_iExecutionType, 
					ai_poExeParams->m_LinkDesign, ai_poExeParams->m_iMetadataLang,
					ai_poExeParams->m_oIDChannel,
					ClMIT_MVC::GetAccessFromRunContext(&ai_poExeParams->m_oDesignContext), 
					ai_poExeParams->m_DesignAutoload, 
					ai_poExeParams->m_bPackDesign,
					ClMIT_Trace::PROPAGATE_ERROR) 
				  ); 
	if (!SwOk) goto exit ;

	// Conexion con el environment de la M4Fmt. 
	// Este es el punto correcto donde se puede proporcionar el acceso a REPORT_CHANNEL_PT
	// a la librería de formateos
	// Si el LinkDesign es ENG_BUILD_DESIGN la inicialización del canal REPORT_CHANNEL_PT no se ha
	// realizado hasta la llamada a DDLF_Chn.Init

	M4FMTSetDesignAccess ( TaskEnv->m_uiFmtTaskId,  DDLF_Chn.GetDefChn()->GetAccess() ) ;

	//Carga DD via DDLF_Chn

	DDLF_Chn.LoadAll(&DD) ;

	//Descarga DDLF_Chn, no hace falta. SÍ HACE FALTA.
	// Al tener la fmt un acceso al REPORT_CHANNEL_PT es necesario asegurar que el canal sigue vivo
	//DDLF_Chn.End() ;

	//Inicializa DT con DD

	SwOk = M4_BOOL( SwOk && DT.Init( &DD, ClMIT_Trace::PROPAGATE_ERROR ));
	if (!SwOk) goto exit ;

	//Descarga DD. Ya no sirve para nada

	DD.End() ;

	//Inicializa Report con DT
	
	SwOk = M4_BOOL( SwOk && ao_poReport->PrepareExecution( &DT, ai_poExeParams, ClMIT_Trace::PROPAGATE_ERROR) );
	if (!SwOk) goto exit ;

	//Descarga DT. Ya no sirve para nada

	DT.End() ;

exit:
	DDLF_Chn.End();
	M4FMTSetDesignAccess ( TaskEnv->m_uiFmtTaskId,  NULL ) ;
	return SwOk ;
}


static m4bool_t LaunchDependences(ClENG_ReportArgs *ai_poExeParams)
{
	ClENG_DDLF_Chn   DDLF_Chn ;
	ClENG_DesignData DD ;
	ClENG_DesignTree DT ;
	ClMIT_Chn		 DataChn;
	ClENG_StorageDep *poDep;
	m4bool_t SwOk=M4_TRUE ;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV( GET_TM() ) ;

	//------------Ejecuta las dependencias

	//Limpia el DD

	DD.Init() ;	

	//Inicia DDLF_Chn
	
	SwOk = M4_BOOL( SwOk && DDLF_Chn.Init(
					ai_poExeParams->m_oReportId, ai_poExeParams->m_iExecutionType, 
					ai_poExeParams->m_LinkDesign, ai_poExeParams->m_iMetadataLang,
					ai_poExeParams->m_oIDChannel,
					ClMIT_MVC::GetAccessFromRunContext(&ai_poExeParams->m_oDesignContext), 
					ai_poExeParams->m_DesignAutoload, 
					ai_poExeParams->m_bPackDesign,
					ClMIT_Trace::PROPAGATE_ERROR) 
				  ); 
	if (!SwOk) goto exit ;

	// Conexion con el environment de la M4Fmt. 
	// Este es el punto correcto donde se puede proporcionar el acceso a REPORT_CHANNEL_PT
	// a la librería de formateos
	// Si el LinkDesign es ENG_BUILD_DESIGN la inicialización del canal REPORT_CHANNEL_PT no se ha
	// realizado hasta la llamada a DDLF_Chn.Init

	M4FMTSetDesignAccess ( TaskEnv->m_uiFmtTaskId,  DDLF_Chn.GetDefChn()->GetAccess() ) ;

	//Carga DD via DDLF_Chn

	DDLF_Chn.LoadAll(&DD) ;

	//Inicializa DT con DD

	SwOk = M4_BOOL( SwOk && DT.Init( &DD, ClMIT_Trace::PROPAGATE_ERROR ));
	if (!SwOk) goto exit ;

	//Descarga DD. Ya no sirve para nada

	DD.End() ;

	//Inicializa el canal de datos
	SwOk = M4_BOOL( SwOk && ClENG_Report::InitChnData(ai_poExeParams, &DataChn, &DT));
	if (!SwOk) goto exit ;

	//Crea e inicializa el gestor de las dependencias
	M4_NEW_ITEM(m_poInsp->m_poMemProf, MEM_GRP_DVC, poDep, ClENG_StorageDepChn) ;
	SwOk = M4_VIRTUAL_OBJECT(poDep, ClENG_StorageDepChn)->Init(DDLF_Chn.GetDefChn());
	if(!SwOk){
		M4_DELETE_VIRTUAL_ITEM(m_poInsp->m_poMemProf, MEM_GRP_DVC, poDep) ;
	};
	
	//Llama al proceso de dependencia
	SwOk = M4_BOOL( SwOk && DT.ManageDependencies(&SwOk, ai_poExeParams->m_bRebuildDependencies,
												  poDep, &DataChn));
	if (!SwOk) goto exit ;	
	
	if(poDep) {
		M4_DELETE_VIRTUAL_ITEM(m_poInsp->m_poMemProf, MEM_GRP_DVC, poDep) ;
	};

	//Descarga DT. Ya no sirve para nada

	DT.End() ;

exit:
	DDLF_Chn.End();
	M4FMTSetDesignAccess ( TaskEnv->m_uiFmtTaskId,  NULL ) ;
	return SwOk ;
}


//----------------------------------------Llamada atómica

m4int16_t ExecuteReport2(
	const m4char_t * ai_pcCommandString, 
	m4int16_t ai_iLinkManagerMode, m4int16_t ai_iLinkDesignMode, m4int16_t ai_iLinkDataMode,
	ClAccess *ai_poVMAccess, ClAccess *ai_poDesignAccess, ClAccess *ai_poDataAccess,
	m4bool_t *ai_bSwCancel,
	m4char_t *ai_pcImagePath, size_t ai_szImagePath,	
	m4char_t *ao_pcOutputFile, size_t ai_szOutputFile,
	m4bool_t ai_bFromWrapper) 
{
	m4int16_t ReturnValue;
	MIT_Id_t NewTask ; 

	SPAWN_IN(NewTask,g_ENG_pstPlugIn) ;

	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;
	TaskEnv->m_APIChn.bFromWrapper = ai_bFromWrapper;

	ClMIT_TraceCheck Check ;
	ClENG_ReportArgs ExeParams ;
	m4bool_t SwOk=M4_TRUE ;
	ClMIT_Str strAux ;

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"ExecuteReport2()", ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG   ;
	};
	#endif

	//---------------------Actualización m_SessionType

	TaskEnv->m_SessionType = StENG_TaskEnv::EXECUTE_REPORT ;

	//---------------------Preparar ejecucion

	SwOk = M4_BOOL( SwOk && LoadParamsLoadMVC(
					Insp, TaskEnv, &ExeParams, ai_pcCommandString, 
					ai_iLinkManagerMode, ai_iLinkDesignMode, ai_iLinkDataMode,
					ai_poVMAccess, ai_poDesignAccess, ai_poDataAccess ) 
				  );
	if (!SwOk) goto exit ;
	
	//---------------------Modos de ejecucion

	if ( ExeParams.m_bExtractStrings ) {
		
		//EJECUCION: extraccion de literales

		ClENG_DDLF_Chn DDLF_Chn ;
		m4int16_t iMetadataLang=-1;

		SwOk = M4_BOOL( SwOk && DDLF_Chn.Init(
						ExeParams.m_oReportId, ExeParams.m_iExecutionType, 
						ExeParams.m_LinkDesign, iMetadataLang,
						ExeParams.m_oIDChannel,
						ClMIT_MVC::GetAccessFromRunContext(&ExeParams.m_oDesignContext), 
						ExeParams.m_DesignAutoload, 
						ExeParams.m_bPackDesign,
						ClMIT_Trace::PROPAGATE_ERROR) 
					  ); 
		if (!SwOk) goto exit ;

		SwOk = M4_BOOL( SwOk && DDLF_Chn.ExtractLiterals() );
		if (!SwOk) goto exit ;
	
	} else if ( ExeParams.m_bUpdateDependencies || ExeParams.m_bRebuildDependencies ) {

		//EJECUCION: Dependencies

		SwOk = M4_BOOL(SwOk && LaunchDependences(&ExeParams));
		if (!SwOk) goto exit ;

	} else {

		//EJECUCION: ejecucion de reports

		ClENG_Report Report ;

		if ( ai_pcImagePath ) {
			ExeParams.m_oImagePath.StrCpy(ai_pcImagePath) ;
			ClMIT_Str::OverStringValid(ai_pcImagePath,ai_szImagePath) ;
		} ;

		SwOk = M4_BOOL(SwOk && PrepareExecution(&ExeParams,	&Report )) ;
		if (!SwOk) goto exit ;

		Report.ExecuteBreak(M4_TRUE) ;
		if ( ! Report.TerminateExecution( &strAux ) ) SwOk=M4_FALSE  ;

		//------------Fin

		if (ao_pcOutputFile) {
			strcpy(ao_pcOutputFile, strAux );
			ClMIT_Str::OverStringValid(ao_pcOutputFile, ai_szOutputFile );
		};
	} ;
 
	Insp->m_poTrace->Flush() ;		

exit:

	if ( ai_bSwCancel ) *ai_bSwCancel=Insp->m_poTrace->IsCancelNotified() ;

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"ExecuteReport2()", ClMIT_Msg::EXIT_FORMAT) << SEND_MSG   ;
	};
	#endif
	;	//Para que en release no cante por falta de sentencia tras el exit:

	Insp->m_poTrace->StartCheck(&Check) ;
	ReturnValue = Check.AllOk() ? 1 : ( Check.OnlyWarnings() ? 2 : 0 );

	SPAWN_OUT();

	//------------Zona KILL

	KILL_IN(NewTask) ;	
	
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	//Dexconexion con el environment de la M4Fmt 
	M4FMTDettachM4Fmt(TaskEnv->m_uiFmtTaskId)	;	//Asignaciones pertenecen al manager

	M4_DELETE_ITEM(Insp->m_poMemProf, MIT_MGI_APICHN, TaskEnv->m_APIChn.m_poMVC, ClMIT_MVC) ;

	KILL_OUT(NewTask) ;

	//------------Fin Zona KILL

	return ReturnValue ; 
}


m4return_t ExecuteReport(
	m4VariantType * ai_poParam, m4int32_t ai_iNParam, 
	m4VariantType * ao_poReturn, ClVMRunContext * ai_poRunContext ) 
{
	m4return_t iResult;
	m4int16_t R;
	m4bool_t SwCancel ;
	m4char_t OutputFile[500];

	ClAccess *poVMAccess = NULL;
	ClAccess *poDesignAccess = NULL;
	ClAccess *poDataAccess = NULL;
	ClChannel *poChannel = NULL;

	if( ai_poRunContext != NULL )
	{
		poVMAccess = ai_poRunContext->m_pAccess ;
		poDataAccess = ai_poRunContext->m_pAccess ;

		poChannel = poDataAccess->GetpChannel() ;

		if( ai_poParam[ENGINE_DLL_INDEX_DESIGN_HANDLER].Type == M4CL_CPP_TYPE_NUMBER )
		{
			// Si es número es el handle físico del acceso
			poDesignAccess = poChannel->GetpChannelManager()->GetAccessByHandle( ai_poParam[ENGINE_DLL_INDEX_DESIGN_HANDLER].Data.DoubleData ) ;
		}
		else if( ai_poParam[ENGINE_DLL_INDEX_DESIGN_HANDLER].Type == M4CL_CPP_TYPE_STRING_VAR )
		{
			// Si es cadena es el alias de nivel 2
			iResult = poChannel->Level2.m_oInstanceMap.Find( ai_poParam[ENGINE_DLL_INDEX_DESIGN_HANDLER].Data.PointerChar, &poDesignAccess ) ;

			if( iResult != M4_SUCCESS )
			{
				poDesignAccess = NULL ;
			}
		}
	}

	R=ExecuteReport2(
		ai_poParam[ENGINE_DLL_INDEX_COMMAND_STRING].Data.PointerChar,
		(m4int16_t)ai_poParam[ENGINE_DLL_INDEX_LINK_MANAGER].Data.DoubleData,
		(m4int16_t)ai_poParam[ENGINE_DLL_INDEX_LINK_DESIGN].Data.DoubleData,
		(m4int16_t)ai_poParam[ENGINE_DLL_INDEX_LINK_DATA].Data.DoubleData,
		poVMAccess, poDesignAccess, poDataAccess,
		&SwCancel, NULL, 0, OutputFile, 500, M4_FALSE
	) ;

	ao_poReturn->Type = M4CL_CPP_TYPE_NUMBER;
	ao_poReturn->Data.DoubleData = R ;

	{
	//Fichero de salida - Item Concepto
	m4bool_t SwAux ;
	m4VariantType VarAux, vIsSliceMode;

	VarAux.Type = M4CL_CPP_TYPE_STRING_VAR; 
	VarAux.Data.PointerChar=OutputFile ;

	//Le pregunto al acceso si estamos ejecutando en modo tramos
	// Devuelve 0 o 1
	ai_poRunContext->m_pAccess->RunSliceMode.Get(vIsSliceMode) ;
	
    if(vIsSliceMode.Type!=M4CL_CPP_TYPE_NULL && vIsSliceMode.Data.DoubleData!=0.0)
    {
		//Si el valor de retorno no es tipo nulo y es true creamos un slice desde
		// menos infinito a mas infinito (en m4date_t).
        ai_poRunContext->m_pRecordSet->Current.Item[ClHandle(ai_poRunContext->m_hItem)].Value.AddSlice(M4CL_MINUS_INFINITE_IN_JULIAN, M4CL_PLUS_INFINITE_IN_JULIAN) ;
		ai_poRunContext->m_pRecordSet->Current.Item[ClHandle(ai_poRunContext->m_hItem)].Value.Set(VarAux,M4CL_RULE_MAX_PRIORITY,SwAux) ;
	} else {
		ai_poRunContext->m_pRecordSet->Current.Item[ClHandle(ai_poRunContext->m_hItem)].Value.Set(VarAux,M4CL_RULE_MAX_PRIORITY,SwAux) ;
	};

	} ;

	return !SwCancel ? M4_SUCCESS : M4CL_USER_ABORT_ALL_REQUEST ;
} 

//---------------------------->Ciclo de llamadas especiales para secuenciar el visor.

m4int16_t WINAPI ViewerInitReport(
	m4uint32_t* ao_TaskId, m4char_t * ai_pcCommandStr, 
	m4int16_t ai_LinkManager, m4int16_t ai_LinkDesign, m4int16_t ai_LinkData,
	m4uint32_t ai_DesignHandler, m4uint32_t ai_DataHandler, 

	m4char_t *ao_pcOutputName, m4int16_t * ao_OutputSize, 
	m4int16_t *ao_ErrorCount, m4int16_t *ao_WarningCount,
	m4uint32_t *ao_uiBreakCount, m4int16_t *ao_SwMoreBreaks) 

{
	m4bool_t SwOk=M4_TRUE ;
	m4bool_t DesignError=M4_FALSE ;

	MIT_Id_t NewTask ; 

	SPAWN_IN(NewTask,g_ENG_pstPlugIn) ;

	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;
	ClENG_ReportArgs ExeParams ;
	ClMIT_Str path ;

	ClAccess * poDesignAccess = NULL;
	ClAccess * pDataAccess = NULL;

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"ViewerInitReport()", ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG   ;
	};
	#endif

	//---------------------Actualización m_SessionType, Dll_g_poReport, flags ...

	TaskEnv->m_SessionType = StENG_TaskEnv::RVW_LOOP ;
	M4_NEW_ITEM(Insp->m_poMemProf, ENG_MGI_OTHERS, TaskEnv->g_RVW.m_poReport, ClENG_Report) ;
	TaskEnv->g_RVW.m_poFile=NULL ;

	*ao_TaskId = NewTask;	

	// Obtener el acceso a partir del handle.
	poDesignAccess = TaskEnv->m_APIChn.m_poMVC->GetChannelManager()->GetAccessByHandle(ai_DesignHandler);
	pDataAccess = TaskEnv->m_APIChn.m_poMVC->GetChannelManager()->GetAccessByHandle(ai_DataHandler);

	//---------------------Preparar ejecucion

	SwOk = M4_BOOL( SwOk &&  LoadParamsLoadMVC(
					Insp, TaskEnv, &ExeParams, ai_pcCommandStr, 
					ai_LinkManager, ai_LinkDesign, ai_LinkData,
					pDataAccess, poDesignAccess, pDataAccess )
				  );
	if (!SwOk) {
		DesignError=M4_TRUE ;
		goto exit ;
	} ;
	SwOk = M4_BOOL(SwOk && PrepareExecution(&ExeParams,	TaskEnv->g_RVW.m_poReport ));
	if (!SwOk) goto exit ;

	//---------------------Ejecuta un break, para que haya algo que mostrar
	
	//Ejecuta un break

	*ao_SwMoreBreaks=TaskEnv->g_RVW.m_poReport->ExecuteBreak(M4_FALSE, ao_uiBreakCount,
		&TaskEnv->g_RVW.m_poFile ) ;
	*ao_SwMoreBreaks = *ao_SwMoreBreaks ? 1 : 0 ;

	if ( TaskEnv->g_RVW.m_poFile ) {
	
		if ( TaskEnv->g_RVW.m_poFile->IsOpen() ) {
			TaskEnv->g_RVW.m_uiFilePos = TaskEnv->g_RVW.m_poFile->GetPos() ;
		} else {
			//Tratamos que no haya fichero como un error de diseño (aunque no lo es)
			//DesignError=M4_TRUE ;
			;
		} ;

		TaskEnv->g_RVW.m_poFile->GetAbsolutePathFile(&path) ;
		// Bug 161143
		// Se estaba devolviendo el directorio en codificación UTF-8 cuando estamos
		// en un entorno Unicode. Esto provacaba que el visor no fuera capaz de leer
		// el archivo puesto que considera dicha cadena como ANSI (con la página de código
		// correspondiente a la máquina).
		// Se realiza la conversión CPP-ANSI que:
		// - En entorno ANSI no realiza ninguna variación sobre la cadena
		// - En entorno Unicode convierte UTF8 a ANSI CP. De esta forma el visor es capaz
		//    de leer correctamente la ruta si hay un ASCII superior a 127
		int iLength = -1;
		m4pchar_t pszTempANSI = M4CppToANSI( path.InChar(0), iLength );
		if (iLength < *ao_OutputSize)
		{
			strcpy(ao_pcOutputName, pszTempANSI);
			*ao_OutputSize = iLength;
		}
		else
		{
			strncpy(ao_pcOutputName, pszTempANSI, *ao_OutputSize - 1);
			ao_pcOutputName[*ao_OutputSize - 1] = '\0';
		}
	}  else {
		strcpy(ao_pcOutputName,"") ;
		*ao_OutputSize = strlen(ao_pcOutputName) ;
	} ;
	

	if (!SwOk) goto exit ;	//Antes de salir, debo copiar el nombre de fichero bien

	//---------------------Hasta luego, Lucarrlll
exit:

	*ao_ErrorCount = Insp->m_poTrace->GetErrorCount() ;
	*ao_WarningCount = Insp->m_poTrace->GetWarningCount() ;

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"ViewerInitReport()", ClMIT_Msg::EXIT_FORMAT) << SEND_MSG   ;
	};
	#endif

	//SwOk= SwOk && ai_Env->g_poTrace->IsAllOK() ;
	
	SPAWN_OUT() ;

	return DesignError ? 0 : 1 ;
}

m4int16_t WINAPI ViewerBreakLoop( m4uint32_t ai_TaskId, m4uint32_t *ao_uiBreakCount,
											 m4int16_t *ao_bSwFuncError) 
{
	m4bool_t Continue ;
	m4int16_t ContinueInt ;
	ClMIT_TraceCheck Check ;

	SWAP_IN(ai_TaskId);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"ViewerBreakLoop()", ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG   ;
	};
	#endif
	
	Insp->m_poTrace->StartCheck(&Check) ;

	if ( TaskEnv->g_RVW.m_poFile->IsOpen() ) {
		TaskEnv->g_RVW.m_poFile->SetPos(TaskEnv->g_RVW.m_uiFilePos) ;
	} ;

	Continue=TaskEnv->g_RVW.m_poReport->ExecuteBreak(M4_FALSE, ao_uiBreakCount) ;

	if ( TaskEnv->g_RVW.m_poFile->IsOpen() ) {
		TaskEnv->g_RVW.m_uiFilePos = TaskEnv->g_RVW.m_poFile->GetPos() ;
		TaskEnv->g_RVW.m_poFile->Flush() ;
	} ;

	Insp->m_poTrace->StopCheck(&Check) ;

exit:

	*ao_bSwFuncError = Check.AllOk() ? 1 : 0 ;
	ContinueInt = *ao_bSwFuncError ? 0 : (Continue ? 1 : 0) ;

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"ViewerBreakLoop() returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< Continue << SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;
	
	return Continue ;
}


m4int16_t WINAPI ViewerEnd(
	m4uint32_t ai_TaskId, m4int16_t *ao_ErrorCount, m4int16_t *ao_WarningCount, 
	m4char_t *ao_pcTraceName, m4int16_t * ao_TraceSize) 
{
	m4bool_t SwError=M4_FALSE ;
	ClMIT_TraceCheck Check ;
		
	KILL_IN(ai_TaskId) ;

	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	
	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"ViewerEnd()", ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG   ;
	};
	#endif

	if (TaskEnv->g_RVW.m_poFile) {
		if ( TaskEnv->g_RVW.m_poFile->IsOpen() ) {
			TaskEnv->g_RVW.m_poFile->SetPos(TaskEnv->g_RVW.m_uiFilePos) ;
		} ;
	} ;

	// Se carga, entre otras cosas, el device

	TaskEnv->g_RVW.m_poReport->TerminateExecution() ;

	Insp->m_poTrace->Flush() ;		

	SwError= M4_BOOL(SwError || !Insp->m_poTrace->IsAllOK());

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"ViewerEnd()", ClMIT_Msg::EXIT_FORMAT) << SEND_MSG   ;
	};
	#endif

	*ao_ErrorCount = Insp->m_poTrace->GetErrorCount() ;
	*ao_WarningCount = Insp->m_poTrace->GetWarningCount() ;

	*ao_TraceSize = strlen(ao_pcTraceName) ;

	//Dexconexion con el environment de la M4Fmt 
	M4FMTDettachM4Fmt(TaskEnv->m_uiFmtTaskId)	;	//Asignaciones pertenecen al manager

	M4_DELETE_ITEM(Insp->m_poMemProf, ENG_MGI_OTHERS, TaskEnv->g_RVW.m_poReport, ClENG_Report) ;
	TaskEnv->g_RVW.m_poFile=NULL ;
	M4_DELETE_ITEM(Insp->m_poMemProf, MIT_MGI_APICHN, TaskEnv->m_APIChn.m_poMVC, ClMIT_MVC) ;

	Insp->m_poTrace->StartCheck(&Check) ;

	KILL_OUT(ai_TaskId) ;

	return ! Check.AllOk() ? 1 : 0 ;
}

m4int16_t WINAPI FlipInt(m4int16_t ai_iInt)
{
	ClMIT_BinaryAdapter::Flip(&ai_iInt,sizeof(ai_iInt)) ;

	return ai_iInt;
}

m4int32_t WINAPI FlipLong(m4int32_t ai_iLong)
{
	ClMIT_BinaryAdapter::Flip(&ai_iLong,sizeof(ai_iLong)) ;

	return ai_iLong ;
}


//----------------------------------------TestAuxiliar interno


//extern m4bool_t TestDeviceAux() ;

m4int16_t  WINAPI TestAux( m4uint32_t ai_TaskId, m4char_t *ai_pcStr, m4char_t *ao_pcTraceName, m4int16_t * ao_TraceSize )
{
	m4bool_t SwOk=M4_TRUE ;

	MIT_Id_t NewTask ; 

	SPAWN_IN(NewTask,g_ENG_pstPlugIn) ;

	if (!ai_pcStr) ai_pcStr="" ;

//	SwOk = TestDeviceAux() ;

	SPAWN_OUT();

	KILL_IN(NewTask) ;	
	KILL_OUT(NewTask) ;	


	return SwOk ? 1 : 0 ;
}



//----------------------------------------Session Type: REPORT_DESIGNER

static void RD_BreakPoint() {
	return ;
} ;

m4int16_t RD_OpenSession(m4int16_t ai_iVersion, m4char_t * ai_pcCommandStr, m4int16_t ai_LinkManager, ClAccess * ai_poAccess, m4int32_t * ao_SessionHandler)
{
	m4bool_t SwOk = M4_TRUE;

	MIT_Id_t NewTask;

	SPAWN_IN(NewTask, g_ENG_pstPlugIn);

	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM);
	ClENG_ReportArgs ExeParams;

#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint();
	if (!Insp->m_poTrace->IsTraceCodeRipped()) {
		*Insp->m_poTrace << ClMIT_Msg(GROUP_ID, "OpenSessionReportDesigner()", ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG;
	};
#endif

	//---------------------Actualización m_SessionType, crear OBJ

	TaskEnv->m_SessionType = StENG_TaskEnv::REPORT_DESIGNER;
	M4_NEW_ITEM(Insp->m_poMemProf, ENG_MGI_OTHERS, TaskEnv->g_poRD, ClENG_SessReportDesigner);

	*ao_SessionHandler = NewTask;

	//---------------------Test version

	if (ai_iVersion != 0) {
		// ##Cambiar num msg
		*Insp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID,
			"Wrong version number %0:s in OpenSessionReportDesigner", ClMIT_Msg::MIXED_OUT)
			<< ai_iVersion << SEND_MSG;
		SwOk = M4_FALSE;
		goto exit;
	};

	//---------------------Preparar ejecucion, levantar MVC

	SwOk = M4_BOOL(SwOk &&  LoadParamsLoadMVC(
		Insp, TaskEnv, &ExeParams, ai_pcCommandStr,
		ai_LinkManager, ENG_LINK_DESIGN, ClENG_ReportRequest::LINK_DATA,
		ai_poAccess, ai_poAccess, ai_poAccess));
	if (!SwOk) goto exit;

exit:
#ifdef RPT_ENGINE_TRACE
	if (!Insp->m_poTrace->IsTraceCodeRipped()) {
		*Insp->m_poTrace << ClMIT_Msg(GROUP_ID, "OpenSessionReportDesigner()", ClMIT_Msg::EXIT_FORMAT) << SEND_MSG;
	};
#endif
	;

	SPAWN_OUT();

	return SwOk ? 1 : 0;
}

void WINAPI RD_CloseSession(m4uint32_t ai_SessionHandler)
{
	KILL_IN(ai_SessionHandler) ;

	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;
	
	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"CloseSession()", ClMIT_Msg::ENTRY_FORMAT) << SEND_MSG   ;
	};
	#endif

	TaskEnv->g_poRD->End() ;

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"CloseSessionReportDesigner()", ClMIT_Msg::EXIT_FORMAT) << SEND_MSG   ;
	};
	#endif

	//Dexconexion con el environment de la M4Fmt 
	M4FMTDettachM4Fmt(TaskEnv->m_uiFmtTaskId)	;	//Asignaciones pertenecen al manager

	M4_DELETE_ITEM(Insp->m_poMemProf, ENG_MGI_OTHERS, TaskEnv->g_poRD, ClENG_SessReportDesigner) ;
	M4_DELETE_ITEM(Insp->m_poMemProf, MIT_MGI_APICHN, TaskEnv->m_APIChn.m_poMVC, ClMIT_MVC) ;

	KILL_OUT(ai_SessionHandler) ;
}

m4int16_t WINAPI RD_LoadReportDesign( m4uint32_t ai_SessionHandler, m4uint32_t ai_DesignHandler ) 
{
	m4bool_t SwOk=M4_TRUE ;
	
	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;
	ClAccess * poDesignAccess = NULL;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"LoadReportDesign()", ClMIT_Msg::ENTRY_FORMAT) 
			<< SEND_MSG   ;
	};
	#endif

	// Obtener el acceso a partir del handle.
	poDesignAccess = TaskEnv->m_APIChn.m_poMVC->GetChannelManager()->GetAccessByHandle(ai_DesignHandler);

	SwOk = M4_BOOL( SwOk && TaskEnv->g_poRD->Init(
		"REPORT_DESIGNER_SESSION", 1,
		ENG_LINK_DESIGN, poDesignAccess,
		ClMIT_Chn::AUTOLOAD_NODESAYS, M4_FALSE, ClMIT_Trace::PROPAGATE_ERROR ) ) ;
	if (!SwOk) goto exit ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"LoadReportDesign() returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;

	return SwOk ? 1 : 0 ;
}

void WINAPI RD_UnloadReportDesign(m4uint32_t ai_SessionHandler)
{
	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"UnloadReportDesign()", ClMIT_Msg::ENTRY_FORMAT) 
			<< SEND_MSG   ;
	};
	#endif

	TaskEnv->g_poRD->End() ;

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"UnloadReportDesign()", ClMIT_Msg::EXIT_FORMAT) 
			<< SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;
}

m4int16_t WINAPI RD_GetCT_Props(
	m4uint32_t ai_SessionHandler,
	m4char_t *ai_pcCTId,
	m4char_t *ao_pcName,		m4uint32_t * ao_NameSize, 
	m4char_t *ao_pcTotList,		m4uint32_t * ao_TotListSize, 
	m4char_t *ao_pcTreeList,	m4uint32_t * ao_TreeListSize, 
	m4char_t *ao_pcFillTree,	m4uint32_t * ao_FillTreeSize, 
	m4int16_t *ao_piLevelCount, m4int16_t *ao_piMergerPage, 
	m4int32_t *ao_piColumnLeft, m4int32_t *ao_piColumnRight,
	m4int16_t *ao_poTogLevel,	m4int16_t *ao_poBreakLevel, 
	m4uint32_t *ao_piIdParamSec,m4int32_t *ao_piIdFirstCrossRow,
	m4char_t *ao_pcWizardData,		m4uint32_t * ao_WizardDataSize
	) 
{
	m4bool_t SwOk=M4_TRUE ;
	
	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_GetCT_Props()", ClMIT_Msg::ENTRY_FORMAT) 
			<< SEND_MSG   ;
	};
	#endif

	SwOk = M4_BOOL( SwOk && TaskEnv->g_poRD->GetCT_Props(
		ai_pcCTId, ao_pcName, ao_NameSize, ao_pcTotList, ao_TotListSize, ao_pcTreeList,
		ao_TreeListSize, ao_pcFillTree, ao_FillTreeSize, ao_piLevelCount, ao_piMergerPage, 
		ao_piColumnLeft, ao_piColumnRight, ao_poTogLevel, ao_poBreakLevel, 
		ao_piIdParamSec, ao_piIdFirstCrossRow,
		ao_pcWizardData,ao_WizardDataSize) 
	) ;
	if (!SwOk) goto exit ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_GetCT_Props() returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;

	return SwOk ? 1 : 0 ;
}


m4int16_t WINAPI RD_GetCT_CellProps(
	m4uint32_t ai_SessionHandler,
	m4char_t *ai_pcCTId, m4int16_t ai_Level, 
	m4int16_t ai_CellType,
	m4int16_t *ao_CellAlignY, 
	m4int16_t *ao_CellAlignX,
	m4int16_t *ao_piFieldCount,
	m4int32_t *ao_piWidth,  m4int32_t *ao_piHeight, 
	m4uint32_t *ao_puiIdFillColour,
	m4char_t *ao_pcFrame, m4uint32_t *aoi_puiFrameSize ) 
{
	m4bool_t SwOk=M4_TRUE ;
	
	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_GetCT_CellProps()", ClMIT_Msg::ENTRY_FORMAT) 
			<< SEND_MSG   ;
	};
	#endif

	SwOk = M4_BOOL( SwOk && TaskEnv->g_poRD->GetCT_CellProps(
		ai_pcCTId, ai_Level, ai_CellType, ao_CellAlignY, ao_CellAlignX, ao_piFieldCount,
		ao_piWidth, ao_piHeight, ao_puiIdFillColour, ao_pcFrame, aoi_puiFrameSize ) 
	) ;
	if (!SwOk) goto exit ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID, 
			"CT %0:s Level %1:s CellType %2:s CellAlignY %3:s CellAlignX %4:s "
			"Width %5:s Height %5:s FillColour %6:s Frame %7:s" ) 
			<< ai_pcCTId << ai_Level 
			<< ClENG_DesignTree::GetCellTypeTraceStr( (ENG_CT_CELL_TYPE)ai_CellType )
			<< *ao_CellAlignY << *ao_CellAlignX
			<< *ao_piWidth << *ao_piHeight << *ao_puiIdFillColour << ao_pcFrame
			<< SEND_MSG   ;
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_GetCT_CellProps() returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;

	return SwOk ? 1 : 0 ;
}

m4int16_t WINAPI RD_GetCntProps(
	m4uint32_t ai_SessionHandler,
		m4uint32_t ai_uiIdSection, m4int16_t ai_iCntType,
		m4int16_t *ao_piPage,
		m4int32_t *ao_piTop, m4int32_t *ao_piLeft, m4int32_t *ao_piWidth, m4int32_t *ao_piHeight,
		m4int16_t *ao_piSwFixed, m4int16_t *ao_piSwTogether,
		m4int32_t *ao_piLowerLimit,
		m4char_t *ao_pcCancelCnt,	m4uint32_t * ao_CancelCntSize, 
		m4char_t *ao_pcCTMerge,		m4uint32_t * ao_CTMergeSize, 
		m4int16_t *ao_piFieldCount,
		m4uint32_t *ao_puiIdFillColour,
		m4char_t *ao_pcFrame, m4uint32_t *aoi_puiFrameSize,

		m4uint32_t *ao_puiTop, m4uint32_t *ao_puiLeft, 
		m4uint32_t *ao_puiWidth, m4uint32_t *ao_puiHeight,
		m4uint32_t *ao_puiLowerLimit,
		m4int16_t *ao_piSwTopLoaded, m4int16_t *ao_piSwLeftLoaded,
		m4int16_t *ao_piSwWidthLoaded, m4int16_t *ao_piSwHeightLoaded,
		m4int16_t *ao_piSwLowerLimitLoaded,
		
		m4char_t *ao_pcDefaultSerialOut, m4uint32_t *aoi_puiDefaultSerialOutSize )
{

	m4bool_t SwOk=M4_TRUE ;
	
	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_GetCntProps() "
			"IdSection %0:s CntType %1:s", ClMIT_Msg::ENTRY_FORMAT) 
			<< ai_uiIdSection << ClENG_DesignTree::GetCntTraceStr( (ENG_TYPE_CONTAINER)ai_iCntType) 
			<< SEND_MSG   ;
	};
	#endif

	SwOk = M4_BOOL( SwOk && TaskEnv->g_poRD->GetCntProps(
		ai_uiIdSection, (ENG_TYPE_CONTAINER)ai_iCntType, ao_piPage, ao_piTop, ao_piLeft, ao_piWidth, ao_piHeight,
		ao_piSwFixed, ao_piSwTogether, ao_piLowerLimit, ao_pcCancelCnt,	ao_CancelCntSize, 
		ao_pcCTMerge, ao_CTMergeSize, ao_piFieldCount, ao_puiIdFillColour, ao_pcFrame, 
		aoi_puiFrameSize, 
		ao_puiTop, ao_puiLeft, ao_puiWidth, ao_puiHeight, ao_puiLowerLimit,
		ao_piSwTopLoaded, ao_piSwLeftLoaded, ao_piSwWidthLoaded, ao_piSwHeightLoaded,
		ao_piSwLowerLimitLoaded,

		ao_pcDefaultSerialOut, aoi_puiDefaultSerialOutSize )
	) ;
	if (!SwOk) goto exit ;
 
exit:

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_GetCntProps() "

			"Page %0:s ParamTop %1:s %2:s ParamLeft %3:s %4:s ParamWidth %5:s %6:s "
			"ParamHeight %7:s %8:s SwFixed %9:s SwTogether %10:s "
			"ParamLowerLimit %11:s %12:s CancelCnt %13:s CtMerge %14:s FieldCount %15:s "
			"FillColour %16:s Frame %17:s  Serial %18:s") 

			<< *ao_piPage 
			<< *ao_puiTop  << m4bool_t(*ao_piSwTopLoaded) 
			<< *ao_puiLeft << m4bool_t(*ao_piSwLeftLoaded)
			<< *ao_puiWidth << m4bool_t(*ao_piSwWidthLoaded)
			<< *ao_puiHeight << m4bool_t(*ao_piSwHeightLoaded)
			<< *ao_piSwFixed << *ao_piSwTogether << *ao_puiLowerLimit << *ao_piSwLowerLimitLoaded
			<< ao_pcCancelCnt << ao_pcCTMerge << *ao_piFieldCount
			<< *ao_puiIdFillColour << ao_pcFrame << ao_pcDefaultSerialOut
			<< SEND_MSG   ;
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_GetCntProps() returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;

	return SwOk ? 1 : 0 ;
}


m4int16_t WINAPI RD_GetFieldProps(
	m4uint32_t ai_SessionHandler,
		m4int16_t ai_Owner, m4char_t *ai_pcCTId, m4uint32_t ai_uiLoc1, m4int16_t ai_iLoc2, 
		m4int16_t ai_iFieldNumber, 
		m4char_t *ao_pcFieldName, m4uint32_t *aoi_puiFieldNameSize,
		m4int16_t *ao_piFieldType,
		m4int32_t *ao_piTop, m4int32_t *ao_piLeft, m4int32_t *ao_piWidth, m4int32_t *ao_piHeight,
		m4char_t *ao_pcSaveNode, m4uint32_t *aoi_puiSaveNodeSize,
		m4char_t *ao_pcSaveItem, m4uint32_t *aoi_puiSaveItemSize,
		m4int16_t *ao_piSwStatic, 
		m4char_t *ao_pcIdPI, m4uint32_t *aoi_puiIdPISize,
		m4int16_t *ao_piSwOptimize, 
		m4char_t *ao_pcExpr, m4uint32_t *aoi_puiExprSize,
		m4char_t *ao_pcLiteralValue, m4uint32_t *aoi_puiLiteralValueSize,

		m4int16_t *ao_piSwEmbebedRaster, 
		m4char_t *ao_pcBNRaster, m4uint32_t *aoi_puiBNRasterSize,
		m4char_t *ao_pcColourRaster, m4uint32_t *aoi_puiColourRasterSize,
		m4int32_t *ao_piInterline, m4int16_t *ao_piSwVarHeight, m4int16_t *ao_piSwJustified,

		m4int16_t *ao_piMaxBoxes, m4int16_t *ao_piFillMode, m4int16_t *ao_piFillChar,
		m4int16_t *ao_piSwDuplicated,
		m4char_t *ao_pcIdCTField, m4uint32_t *aoi_puiIdCTFieldSize,
		m4int16_t *ao_piBarCodeType,
		m4uint32_t *ao_uiIdFillColour, m4char_t *ao_pcFrame, m4uint32_t *ao_puiFrameSize,
		m4char_t *ao_pcIndexExpr, m4uint32_t *ao_puiIndexExprSize,
		m4int32_t *ao_piIndexLevel,
		m4char_t *ai_pcHyperlink, m4uint32_t *ao_puiHyperlinkSize,
		m4int16_t *ao_piTempWebImage,

		m4uint32_t *ao_puiTop, m4uint32_t *ao_puiLeft, 
		m4uint32_t *ao_puiWidth, m4uint32_t *ao_puiHeight,
		m4int16_t *ao_piSwTopLoaded, m4int16_t *ao_piSwLeftLoaded,
		m4int16_t *ao_piSwWidthLoaded, m4int16_t *ao_piSwHeightLoaded,

		m4char_t *ao_pcDefaultSerialOut, m4uint32_t *aoi_puiDefaultSerialOutSize )
{
	m4bool_t SwOk=M4_TRUE ;
	
	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_GetFieldProps() "
			"Owner %0:s CtID %1:s Loc1 %2:s Loc2 %3:s FieldNumber %4:s", ClMIT_Msg::ENTRY_FORMAT) 
			<< m4char_t(ai_Owner) << ai_pcCTId <<  ai_uiLoc1 << ai_iLoc2 << ai_iFieldNumber
			<< SEND_MSG   ;
	};
	#endif

	SwOk = M4_BOOL( SwOk && TaskEnv->g_poRD->GetFieldProps(
		ai_Owner, ai_pcCTId, ai_uiLoc1, ai_iLoc2, ai_iFieldNumber, 
		ao_pcFieldName, aoi_puiFieldNameSize, ao_piFieldType, ao_piTop, ao_piLeft, 
		ao_piWidth, ao_piHeight, ao_pcSaveNode, aoi_puiSaveNodeSize, 
		ao_pcSaveItem, aoi_puiSaveItemSize, ao_piSwStatic, ao_pcIdPI, aoi_puiIdPISize,
		ao_piSwOptimize, ao_pcExpr, aoi_puiExprSize, ao_pcLiteralValue, aoi_puiLiteralValueSize,
		ao_piSwEmbebedRaster, ao_pcBNRaster, aoi_puiBNRasterSize, ao_pcColourRaster, 
		aoi_puiColourRasterSize, ao_piInterline, ao_piSwVarHeight, ao_piSwJustified,
		ao_piMaxBoxes, ao_piFillMode, ao_piFillChar, ao_piSwDuplicated, ao_pcIdCTField, 
		aoi_puiIdCTFieldSize, ao_piBarCodeType, ao_uiIdFillColour, 
		ao_pcFrame, ao_puiFrameSize, 
		ao_pcIndexExpr, ao_puiIndexExprSize, ao_piIndexLevel, ai_pcHyperlink, 
		ao_puiHyperlinkSize, ao_piTempWebImage,

		ao_puiTop, ao_puiLeft, ao_puiWidth, ao_puiHeight,
		ao_piSwTopLoaded, ao_piSwLeftLoaded, ao_piSwWidthLoaded, ao_piSwHeightLoaded,

		ao_pcDefaultSerialOut, aoi_puiDefaultSerialOutSize )
	) ;
	if (!SwOk) goto exit ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID, 
			"FieldName %0:s FieldType %1:s ParamTop %2:s ParamLeft %3:s ParamWidth %4:s "
			"ParamHeight %5:s IdPI %6:s Expr %7:s LiteralValue %8:s Frame %9:s "
			"FillColour %10:s SerialOut %11:s") 
			<< ao_pcFieldName << *ao_piFieldType << *ao_puiTop << *ao_puiLeft 
			<< *ao_puiWidth << *ao_puiHeight << ao_pcIdPI << ao_pcExpr << ao_pcLiteralValue
			<< ao_pcFrame << *ao_uiIdFillColour << ao_pcDefaultSerialOut
			<< SwOk << SEND_MSG   ;
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_GetFieldProps() returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;

	return SwOk ? 1 : 0 ;
}

m4int16_t WINAPI RD_LoadCT_Sample(
		m4uint32_t ai_SessionHandler, m4char_t *ai_pcCTId, 
		m4char_t * ai_pcLevelInfo, m4bool_t ai_bSwIncr,
		m4int16_t ai_iTogLevel, m4bool_t ai_bSwAllCnt,
		m4char_t *ao_pcSerialOut, m4uint32_t *aoi_puiSerialOutSize ) 
{
	m4bool_t SwOk=M4_TRUE ;

	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_LoadCT_Sample() "
			"CT %0:s LevelInfo %1:s SwIncr %2:s TogLevel %3:s SwAllCnt %4:s", ClMIT_Msg::ENTRY_FORMAT) 
			<< ai_pcCTId << ai_pcLevelInfo << ai_bSwIncr << ai_iTogLevel << ai_bSwAllCnt
			<< SEND_MSG   ;
	};
	#endif

	SwOk = M4_BOOL( SwOk && TaskEnv->g_poRD->LoadCT_Sample(
		ai_pcCTId, ai_pcLevelInfo, ai_bSwIncr, ai_iTogLevel, ai_bSwAllCnt,
		ao_pcSerialOut, aoi_puiSerialOutSize ) 
	) ;
	if (!SwOk) goto exit ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_LoadCT_Sample() returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;

	return SwOk ? 1 : 0 ;
}

m4int16_t WINAPI RD_SetCT_Props(
	m4uint32_t ai_SessionHandler,
    m4char_t *ai_pcCTId,
	m4int16_t ai_iMergePage, m4double_t ai_dColumnLeft, m4double_t ai_dColumnRight,
	m4int16_t ai_iTogLevel, m4char_t *ai_pcWizardData ) 
{
	m4bool_t SwOk=M4_TRUE ;

	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_SetCT_Props()", ClMIT_Msg::ENTRY_FORMAT) 
			<< SEND_MSG   ;
	};
	#endif

	SwOk = M4_BOOL( SwOk && TaskEnv->g_poRD->SetCT_Props(
		ai_pcCTId, ai_iMergePage, ai_dColumnLeft, ai_dColumnRight, ai_iTogLevel, ai_pcWizardData ) 
	) ;
	if (!SwOk) goto exit ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_SetCT_Props() returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;

	return SwOk ? 1 : 0 ;
}


m4int16_t WINAPI RD_SetWizardData(
	m4uint32_t ai_SessionHandler,
    m4char_t *ai_pcCTId, m4char_t *ai_pcWizardData ) 
{
	m4bool_t SwOk=M4_TRUE ;

	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_SetWizardData()", ClMIT_Msg::ENTRY_FORMAT) 
			<< SEND_MSG   ;
	};
	#endif

	SwOk = M4_BOOL( SwOk && TaskEnv->g_poRD->SetWizardData(ai_pcCTId, ai_pcWizardData) ) ;
	if (!SwOk) goto exit ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_SetWizardData() returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;

	return SwOk ? 1 : 0 ;
}


m4int16_t WINAPI RD_SetCT_CellProps(
	m4uint32_t ai_SessionHandler,
	m4char_t *ai_pcCTId, m4int16_t ai_Level, 
	/*ENG_CT_CELL_TYPE*/ m4int16_t ai_CellType,
	/*ENG_CT_CELL_Y_ALIGN*/m4int16_t ai_CellAlignY, 
	/*ENG_CT_CELL_X_ALIGN*/m4int16_t ai_CellAlignX,
	m4double_t ai_dWidth,  m4double_t ai_dHeight, 
	m4uint32_t ai_uiIdFillColour, m4char_t *ai_pcFrame ) 
{
	m4bool_t SwOk=M4_TRUE ;

	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_SetCT_CellProps()", ClMIT_Msg::ENTRY_FORMAT) 
			<< SEND_MSG   ;
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID, 
			"CT %0:s Level %1:s CellType %2:s CellAlignY %3:s CellAlignX %4:s "
			"Width %5:s Height %5:s FillColour %6:s Frame %7:s" ) 
			<< ai_pcCTId << ai_Level 
			<< ClENG_DesignTree::GetCellTypeTraceStr( (ENG_CT_CELL_TYPE)ai_CellType )
			<< (m4int16_t)ai_CellAlignY << (m4int16_t)ai_CellAlignX
			<< ai_dWidth << ai_dHeight << ai_uiIdFillColour << ai_pcFrame
			<< SEND_MSG   ;
	};
	#endif

	SwOk = M4_BOOL( SwOk && TaskEnv->g_poRD->SetCT_CellProps(
		ai_pcCTId, ai_Level, ai_CellType, ai_CellAlignY, ai_CellAlignX, ai_dWidth, ai_dHeight, 
		ai_uiIdFillColour, ai_pcFrame ) 
	) ;
	if (!SwOk) goto exit ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_SetCT_CellProps() returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;

	return SwOk ? 1 : 0 ;
}

m4int16_t WINAPI RD_SetFieldProps(
	m4uint32_t ai_SessionHandler,
	m4int16_t ai_Owner, m4char_t *ai_pcCTId, m4uint32_t ai_uiLoc1, m4int16_t ai_iLoc2, 
	m4int16_t ai_iFieldNumber, 
	m4char_t *ai_pcFieldName, m4int16_t ai_iFieldType,
	m4double_t ai_dTop, m4double_t ai_dLeft, m4double_t ai_dWidth, m4double_t ai_dHeight,
	m4char_t *ai_pcSaveNode, m4char_t *ai_pcSaveItem, m4int16_t ai_iSwStatic, 
	m4char_t *ai_pcIdPI, m4int16_t ai_iSwOptimize, 
	m4char_t *ai_pcExpr, m4char_t *ai_pcLiteralValue, 
	
	m4int16_t ai_iSwEmbebedRaster, 
	m4char_t *ai_pcBNRaster, m4char_t *ai_pcColourRaster, 
	m4double_t ai_iInterline, m4int16_t ai_iSwVarHeight, m4int16_t ai_iSwJustified,

	m4int16_t ai_iMaxBoxes, m4int16_t ai_iFillMode, m4int16_t ai_iFillChar,
	m4int16_t ai_iSwDuplicated, m4char_t *ai_pcIdCTField, m4int16_t ai_iBarCodeType,
	m4uint32_t ai_uiIdFillColour, m4char_t *ai_pcFrame, 
	m4char_t *ai_pcIndexExpr, m4int32_t ai_iIndexLevel,
	m4char_t *ai_pcHyperlink, m4int16_t ai_iTempWebImage,

	m4char_t *ai_pcDefaultSerialIn )
{
	m4bool_t SwOk=M4_TRUE ;

	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_SetFieldProps() for %0:s "
			"Owner %1:s CtID %2:s Loc1 %3:s Loc2 %4:s FieldNumber %5:s", ClMIT_Msg::ENTRY_FORMAT) 
			<< (ai_iFieldNumber<0 ? "ADD a field" : "MODIFY a field" )
			<< m4char_t(ai_Owner) << ai_pcCTId <<  ai_uiLoc1 << ai_iLoc2 << ai_iFieldNumber
			<< SEND_MSG   ;
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID, 
			"FieldName %0:s FieldType %1:s ParamTop %2:s ParamLeft %3:s ParamWidth %4:s "
			"ParamHeight %5:s IdPI %6:s Expr %7:s LiteralValue %8:s Frame %9:s "
			"FillColour %10:s SerialIn %11:s") 
			<< ai_pcFieldName << ai_iFieldType << ai_dTop << ai_dLeft 
			<< ai_dWidth << ai_dHeight << ai_pcIdPI << ai_pcExpr << ai_pcLiteralValue
			<< ai_pcFrame << ai_uiIdFillColour << ai_pcDefaultSerialIn
			<< SwOk << SEND_MSG   ;
	};
	#endif


	if ( ai_iFieldNumber >= 0 ) {
		SwOk = M4_BOOL( SwOk && TaskEnv->g_poRD->SetFieldProps(
			ai_Owner, ai_pcCTId, ai_uiLoc1, ai_iLoc2, ai_iFieldNumber, ai_pcFieldName, 
			ai_iFieldType, ai_dTop, ai_dLeft, ai_dWidth, ai_dHeight, ai_pcSaveNode, 
			ai_pcSaveItem, ai_iSwStatic, ai_pcIdPI, ai_iSwOptimize, ai_pcExpr, ai_pcLiteralValue, 
			ai_iSwEmbebedRaster, ai_pcBNRaster, ai_pcColourRaster, ai_iInterline, ai_iSwVarHeight, 
			ai_iSwJustified, 
			ai_iMaxBoxes, ai_iFillMode, ai_iFillChar, ai_iSwDuplicated, 
			ai_pcIdCTField, ai_iBarCodeType, ai_uiIdFillColour, ai_pcFrame, ai_pcIndexExpr, 
			ai_iIndexLevel, ai_pcHyperlink, ai_iTempWebImage,

			ai_pcDefaultSerialIn )
		) ;
	} else {
		SwOk = M4_BOOL( SwOk && TaskEnv->g_poRD->AddField(
			ai_Owner, ai_pcCTId, ai_uiLoc1, ai_iLoc2, ai_pcFieldName, 
			ai_iFieldType, ai_dTop, ai_dLeft, ai_dWidth, ai_dHeight, ai_pcSaveNode, 
			ai_pcSaveItem, ai_iSwStatic, ai_pcIdPI, ai_iSwOptimize, ai_pcExpr, ai_pcLiteralValue, 
			ai_iSwEmbebedRaster, ai_pcBNRaster, ai_pcColourRaster, ai_iInterline, ai_iSwVarHeight, 
			ai_iSwJustified, 
			ai_iMaxBoxes, ai_iFillMode, ai_iFillChar, ai_iSwDuplicated, 
			ai_pcIdCTField, ai_iBarCodeType, ai_uiIdFillColour, ai_pcFrame, ai_pcIndexExpr, 
			ai_iIndexLevel, ai_pcHyperlink, ai_iTempWebImage,

			ai_pcDefaultSerialIn )
		) ;
	} ;
	if (!SwOk) goto exit ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_SetFieldProps() returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;

	return SwOk ? 1 : 0 ;

}


m4int16_t WINAPI RD_DeleteField(
	m4uint32_t ai_SessionHandler,
	m4int16_t ai_Owner, m4char_t *ai_pcCTId, m4uint32_t ai_uiLoc1, m4int16_t ai_iLoc2, 
	m4int16_t ai_iFieldNumber ) 
{
	m4bool_t SwOk=M4_TRUE ;

	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_DeleteField()", ClMIT_Msg::ENTRY_FORMAT) 
			<< SEND_MSG   ;
	};
	#endif


	SwOk = M4_BOOL( SwOk && TaskEnv->g_poRD->DeleteField(
		ai_Owner, ai_pcCTId, ai_uiLoc1, ai_iLoc2, ai_iFieldNumber ) 
	) ;
	if (!SwOk) goto exit ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_DeleteField() returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;

	return SwOk ? 1 : 0 ;
}


m4int16_t WINAPI RD_SetCntProps(
	m4uint32_t ai_SessionHandler,
	m4uint32_t ai_uiIdSection, m4int16_t ai_CntType,
	m4int16_t ai_iPage,
	m4double_t ai_dTop, m4double_t ai_dLeft, m4double_t ai_dWidth, m4double_t ai_dHeight,
	m4int16_t ai_iSwFixed, m4int16_t ai_iSwTogether,
	m4double_t ai_dLowerLimit,
	m4char_t *ai_pcCancelCnt,	 
	m4char_t *ai_pcCTMerge,
	m4uint32_t ai_uiIdFillColour,
	m4char_t *ai_pcFrame, 

	m4char_t *ai_pcDefaultSerialIn )
{
	m4bool_t SwOk=M4_TRUE ;

	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_SetCntProps()", ClMIT_Msg::ENTRY_FORMAT) 
			<< SEND_MSG   ;
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_SetCntProps() "

			"IdSection %0:s CntType %1:s Page %2:s Top %3:s Left %4:s Width %5:s  Height %6:s "
			"SwFixed %7:s SwTogether %8:s "
			"ParamLowerLimit %9:s CancelCnt %10:s CtMerge %11:s "
			"FillColour %12:s Frame %13:s  Serial %14:s") 

			<< ai_uiIdSection << ClENG_DesignTree::GetCntTraceStr( (ENG_TYPE_CONTAINER)ai_CntType) 
			<< ai_iPage << ai_dTop  << ai_dLeft << ai_dWidth << ai_dHeight 
			<< ai_iSwFixed << ai_iSwTogether << ai_dLowerLimit 
			<< ai_pcCancelCnt << ai_pcCTMerge 
			<< ai_uiIdFillColour << ai_pcFrame << ai_pcDefaultSerialIn
			<< SEND_MSG   ;
	};
	#endif


	SwOk = M4_BOOL( SwOk && TaskEnv->g_poRD->SetCntProps(
		ai_uiIdSection, ai_CntType, ai_iPage, ai_dTop, ai_dLeft, ai_dWidth, ai_dHeight,
		ai_iSwFixed, ai_iSwTogether, ai_dLowerLimit, ai_pcCancelCnt, ai_pcCTMerge,
		ai_uiIdFillColour, ai_pcFrame, 

		ai_pcDefaultSerialIn )
	) ;
	if (!SwOk) goto exit ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_SetCntProps() returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;

	return SwOk ? 1 : 0 ;
}


m4uint32_t WINAPI RD_CountFontInstances(
	m4uint32_t ai_SessionHandler, m4uint32_t ai_uiIdFont ) 
{
	m4uint32_t R ;

	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_CountFontInstances()", ClMIT_Msg::ENTRY_FORMAT) 
			<< SEND_MSG   ;
	};
	#endif

	R = TaskEnv->g_poRD->CountFontInstances(ai_uiIdFont) ;

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_CountFontInstances() returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< R << SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;
	
	return R  ;
}

m4uint32_t WINAPI RD_CountFormatInstances(
	m4uint32_t ai_SessionHandler, m4uint32_t ai_uiIdFormat ) 
{
	m4uint32_t R=0 ;

	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_CountFormatInstances()", ClMIT_Msg::ENTRY_FORMAT) 
			<< SEND_MSG   ;
	};
	#endif

	R = TaskEnv->g_poRD->CountFormatInstances(ai_uiIdFormat) ;

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_CountFormatInstances() returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< R << SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;
	
	return R ;
}



m4int16_t WINAPI RD_DeleteCnt(
	m4uint32_t ai_SessionHandler,
	m4uint32_t ai_uiIdSection, m4int16_t ai_CntType )
{
	m4bool_t SwOk=M4_TRUE ;

	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_DeleteCnt()", ClMIT_Msg::ENTRY_FORMAT) 
			<< SEND_MSG   ;
	};
	#endif

	SwOk = M4_BOOL( SwOk && TaskEnv->g_poRD->DeleteCnt( ai_uiIdSection, ai_CntType) ) ;
	if (!SwOk) goto exit ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_DeleteCnt() returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;

	return SwOk ? 1 : 0 ;
}

m4int16_t WINAPI RD_DeleteCT_Cell(
	m4uint32_t ai_SessionHandler, m4char_t *ai_pcCTId, m4int16_t ai_Level, m4int16_t ai_CellType ) 
{
	m4bool_t SwOk=M4_TRUE ;

	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_DeleteCT_Cell()", ClMIT_Msg::ENTRY_FORMAT) 
			<< SEND_MSG   ;
	};
	#endif

	SwOk = M4_BOOL( SwOk && TaskEnv->g_poRD->DeleteCT_Cell( ai_pcCTId, ai_Level, ai_CellType) ) ;
	if (!SwOk) goto exit ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_DeleteCT_Cell() returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;

	return SwOk ? 1 : 0 ;
}


m4int16_t WINAPI RD_AddCT_Cell(
	m4uint32_t ai_SessionHandler,
	m4char_t *ai_pcCTId, m4int16_t ai_Level, 
	/*ENG_CT_CELL_TYPE*/ m4int16_t ai_CellType,
	/*ENG_CT_CELL_Y_ALIGN*/m4int16_t ai_CellAlignY, 
	/*ENG_CT_CELL_X_ALIGN*/m4int16_t ai_CellAlignX,
	m4double_t ai_dWidth,  m4double_t ai_dHeight, 
	m4uint32_t ai_uiIdFillColour, m4char_t *ai_pcFrame ) 
{
	m4bool_t SwOk=M4_TRUE ;

	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_AddCT_Cell()", ClMIT_Msg::ENTRY_FORMAT) 
			<< SEND_MSG   ;
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID, 
			"CT %0:s Level %1:s CellType %2:s CellAlignY %3:s CellAlignX %4:s "
			"Width %5:s Height %5:s FillColour %6:s Frame %7:s" ) 
			<< ai_pcCTId << ai_Level 
			<< ClENG_DesignTree::GetCellTypeTraceStr( (ENG_CT_CELL_TYPE)ai_CellType )
			<< (m4int16_t)ai_CellAlignY << (m4int16_t)ai_CellAlignX
			<< ai_dWidth << ai_dHeight << ai_uiIdFillColour << ai_pcFrame
			<< SEND_MSG   ;
	};
	#endif

	SwOk = M4_BOOL( SwOk && TaskEnv->g_poRD->AddCT_Cell( 
		ai_pcCTId, ai_Level, ai_CellType, ai_CellAlignY, ai_CellAlignX,
		ai_dWidth,  ai_dHeight, ai_uiIdFillColour, ai_pcFrame ) 
	) ;
	if (!SwOk) goto exit ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_AddCT_Cell() returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;

	return SwOk ? 1 : 0 ;
}


m4int16_t WINAPI RD_SynchronizeCT( m4uint32_t ai_SessionHandler, m4char_t *ai_pcCTId )
{
	m4bool_t SwOk=M4_TRUE ;

	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_SynchronizeCT() Ct %0:s", ClMIT_Msg::ENTRY_FORMAT) 
			<< ai_pcCTId << SEND_MSG ;
	};
	#endif

	SwOk = M4_BOOL( SwOk && TaskEnv->g_poRD->SynchronizeCT( ai_pcCTId ) ) ;
	if (!SwOk) goto exit ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_SynchronizeCT() returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;

	return SwOk ? 1 : 0 ;
}

m4int16_t WINAPI RD_IncrCTHeightLevel(m4uint32_t ai_SessionHandler, m4char_t *ai_pcCTId, m4int16_t ai_Level, m4double_t ai_dHeightIncrement)
{
	m4bool_t SwOk=M4_TRUE ;

	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_IncrCTHeightLevel() Ct %0:s Level %1:s Increment %2:s", ClMIT_Msg::ENTRY_FORMAT) 
			<< ai_pcCTId << ai_Level << ai_dHeightIncrement << SEND_MSG ;
	};
	#endif

	SwOk = M4_BOOL( SwOk && TaskEnv->g_poRD->IncrCTHeightLevel(ai_pcCTId, ai_Level, ai_dHeightIncrement ) ) ;
	if (!SwOk) goto exit ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_IncrCTHeightLevel() returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;

	return SwOk ? 1 : 0 ;
}

m4int16_t WINAPI RD_GenerateNewCTIdField(m4uint32_t ai_SessionHandler,
	m4char_t *ai_pcCTId, m4char_t *ao_pcKey, m4uint32_t *aoi_puiKeySize) 
{
	m4bool_t SwOk=M4_TRUE ;

	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_GenerateNewCTIdField() Ct %0:s", ClMIT_Msg::ENTRY_FORMAT) 
			<< ai_pcCTId << SEND_MSG ;
	};
	#endif

	SwOk = M4_BOOL( SwOk && TaskEnv->g_poRD->GenerateNewCTIdField(ai_pcCTId,ao_pcKey,aoi_puiKeySize ) ) ;
	if (!SwOk) goto exit ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_GenerateNewCTIdField() returning %0:s %1:s", ClMIT_Msg::EXIT_FORMAT) 
			<< ai_pcCTId << SwOk << SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;

	return SwOk ? 1 : 0 ;
}

m4int16_t WINAPI RD_EnsureFont(m4uint32_t ai_SessionHandler, m4uint32_t ai_uiIdFont )
{
	m4bool_t SwOk=M4_TRUE ;

	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_EnsureFont() FOnt %0:s", ClMIT_Msg::ENTRY_FORMAT) 
			<< ai_uiIdFont << SEND_MSG ;
	};
	#endif

	SwOk = M4_BOOL( SwOk && TaskEnv->g_poRD->EnsureFont(ai_uiIdFont ) ) ;
	if (!SwOk) goto exit ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_EnsureFont() returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;

	return SwOk ? 1 : 0 ;
}


m4int16_t WINAPI RD_EnsureFormat(m4uint32_t ai_SessionHandler, m4uint32_t ai_uiIdFormat )
{
	m4bool_t SwOk=M4_TRUE ;

	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_EnsureFormat() Format %0:s", ClMIT_Msg::ENTRY_FORMAT) 
			<< ai_uiIdFormat << SEND_MSG ;
	};
	#endif

	SwOk = M4_BOOL( SwOk && TaskEnv->g_poRD->EnsureFormat(ai_uiIdFormat ) ) ;
	if (!SwOk) goto exit ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_EnsureFormat() returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;

	return SwOk ? 1 : 0 ;
}


m4int16_t WINAPI RD_EnsureColour(m4uint32_t ai_SessionHandler, m4uint32_t ai_uiIdColour )
{
	m4bool_t SwOk=M4_TRUE ;

	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_EnsureColour() Colour %0:s", ClMIT_Msg::ENTRY_FORMAT) 
			<< ai_uiIdColour << SEND_MSG ;
	};
	#endif

	SwOk = M4_BOOL( SwOk && TaskEnv->g_poRD->EnsureColour(ai_uiIdColour ) ) ;
	if (!SwOk) goto exit ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_EnsureColour() returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;

	return SwOk ? 1 : 0 ;
}


m4int16_t WINAPI RD_ClearFontsNotUsed(m4uint32_t ai_SessionHandler )
{
	m4bool_t SwOk=M4_TRUE ;

	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_ClearFontsNotUsed()", ClMIT_Msg::ENTRY_FORMAT) 
			<< SEND_MSG ;
	};
	#endif

	SwOk = M4_BOOL( SwOk && TaskEnv->g_poRD->ClearFontsNotUsed() ) ;
	if (!SwOk) goto exit ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_ClearFontsNotUsed() returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;

	return SwOk ? 1 : 0 ;
}

m4int16_t WINAPI RD_ClearFormatsNotUsed(m4uint32_t ai_SessionHandler )
{
	m4bool_t SwOk=M4_TRUE ;

	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_ClearFormatsNotUsed()", ClMIT_Msg::ENTRY_FORMAT) 
			<< SEND_MSG ;
	};
	#endif

	SwOk = M4_BOOL( SwOk && TaskEnv->g_poRD->ClearFormatsNotUsed() ) ;
	if (!SwOk) goto exit ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_ClearFormatsNotUsed() returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;

	return SwOk ? 1 : 0 ;
}

m4int16_t WINAPI RD_ClearColoursNotUsed(m4uint32_t ai_SessionHandler )
{
	m4bool_t SwOk=M4_TRUE ;

	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_ClearColoursNotUsed()", ClMIT_Msg::ENTRY_FORMAT) 
			<< SEND_MSG ;
	};
	#endif

	SwOk = M4_BOOL( SwOk && TaskEnv->g_poRD->ClearColoursNotUsed() ) ;
	if (!SwOk) goto exit ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_ClearColoursNotUsed() returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;

	return SwOk ? 1 : 0 ;
}

void WINAPI RD_SynchronizeFontsUsedByReport(m4uint32_t ai_SessionHandler )
{
	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_SynchronizeFontsUsedByReport()", ClMIT_Msg::ENTRY_FORMAT) 
			<< SEND_MSG ;
	};
	#endif

	TaskEnv->g_poRD->SynchronizeFontsUsedByReport() ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_SynchronizeFontsUsedByReport()", ClMIT_Msg::EXIT_FORMAT) 
			<< SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;

	return  ;
}

void WINAPI RD_SynchronizeFormatsUsedByReport(m4uint32_t ai_SessionHandler )
{
	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_SynchronizeFormatsUsedByReport()", ClMIT_Msg::ENTRY_FORMAT) 
			<< SEND_MSG ;
	};
	#endif

	TaskEnv->g_poRD->SynchronizeFormatsUsedByReport() ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_SynchronizeFormatsUsedByReport()", ClMIT_Msg::EXIT_FORMAT) 
			<< SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;

	return  ;
}

void WINAPI RD_SynchronizeColoursUsedByReport(m4uint32_t ai_SessionHandler )
{
	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_SynchronizeColoursUsedByReport()", ClMIT_Msg::ENTRY_FORMAT) 
			<< SEND_MSG ;
	};
	#endif

	TaskEnv->g_poRD->SynchronizeColoursUsedByReport() ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_SynchronizeColoursUsedByReport()", ClMIT_Msg::EXIT_FORMAT) 
			<< SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;

	return  ;
}


m4int16_t WINAPI RD_GetCT_WizardData(m4uint32_t ai_SessionHandler,
	m4char_t *ai_pcCTId, m4char_t *ao_pcSerialOut, m4uint32_t *aoi_puiSerialOutSize) 
{
	m4bool_t SwOk=M4_TRUE ;

	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_GetCT_WizardData() CT %0:s", ClMIT_Msg::ENTRY_FORMAT) 
			<< ai_pcCTId << SEND_MSG ;
	};
	#endif

	SwOk = M4_BOOL( SwOk && TaskEnv->g_poRD->GetCT_WizardData(ai_pcCTId, ao_pcSerialOut, aoi_puiSerialOutSize) ) ;
	if (!SwOk) goto exit ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_GetCT_WizardData() returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;

	return SwOk ? 1 : 0 ;
}

m4int16_t WINAPI RD_GetGraphicFileInfo(
	m4int32_t ai_SessionHandler,
	m4char_t *ai_pcFileName, m4int16_t *ao_FileFormat,
	m4uint32_t *ao_PixelWidth, m4uint32_t *ao_PixelHeight, m4int16_t *ao_bpp ) 
{
	m4bool_t SwOk=M4_TRUE ;

	SWAP_IN(ai_SessionHandler);
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(TM) ;

	#ifdef RPT_ENGINE_TRACE
	RD_BreakPoint() ;
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_GetGraphicFileInfo() File %0:s", ClMIT_Msg::ENTRY_FORMAT) 
			<< ai_pcFileName << SEND_MSG ;
	};
	#endif

	ClENG_RasterMap::FILE_FORMATS fileFormat ;
	SwOk = M4_BOOL( SwOk && ClENG_RasterMap::GetFileInfo(
		ai_pcFileName, &fileFormat, ao_PixelWidth, ao_PixelHeight, ao_bpp ) ) ;
	if (!SwOk) goto exit ;

	switch (fileFormat) {
	case ClENG_RasterMap::BMP :
		*ao_FileFormat=0 ;
		break ;
	case ClENG_RasterMap::GIF :
		*ao_FileFormat=1 ;
		break ;
	case ClENG_RasterMap::JPG :
		*ao_FileFormat=2 ;
		break ;
	default:
		*ao_FileFormat=200 ;
		break;
	} ;

exit:

	#ifdef RPT_ENGINE_TRACE
	if(!Insp->m_poTrace->IsTraceCodeRipped()){
		*Insp->m_poTrace << ClMIT_Msg( GROUP_ID,"RD_GetGraphicFileInfo() returning %0:s", ClMIT_Msg::EXIT_FORMAT) 
			<< SwOk << SEND_MSG   ;
	};
	#endif

	SWAP_OUT() ;

	return SwOk ? 1 : 0 ;
}
