//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             channel.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:08/08/1997
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Manuel Lazcano (28/05/98) - TIPO CURRENCY
//												ClMIT_ChnItemValue::ResetChannel()
//												ClMIT_ChnItemValue::GetCurrencyExchange()				
// Definition:
//
//    This module defines...
//
//==============================================================================

#include "apichni.h"

/*
#include "dump_vst.hpp"	//Para chequeos
*/

#include "cldefine.hpp"
//#include "chvmenv.hpp"
#include "channel.hpp"
#include "access.hpp"
#include "node.hpp"
#include "register.hpp"
#include "item.hpp"
#include "recorset.hpp"
#include "executor.hpp"
//#include "ldb.hpp"
// rollete nuevo del Logon()
#include "cllgadap.hpp"
//Para el OrderBy
#include "handles.hpp"
//#include "cachefac.hpp"


#include "dm.hpp"
#include "level2.hpp"
#include "node_knl.hpp"
#include "colitemdefit.hpp"
#undef TYPE		//Pendiente de que lo quiten en server (salvajada descomunal)
#include "chan_knl.hpp"
#undef TYPE		//Pendiente de que lo quiten en server (salvajada descomunal)

#include "m4mdrt.hpp"

#include "m4objglb.hpp"

// Nuevo y definitivo? API

#include "clm4objservice.hpp"
#include "clfactm4objservice.hpp"


#include "xapichn.h"

#include "cltimer.h"
#include "mitdates.h"
#include "sysinfo.h"
#include "sesschn.h"

#include "xapichn.hmg"


//=============================================================== ClMIT_MVC

ClMutex ClMIT_MVC::m_oMutexCreatedCounter(M4_TRUE) ;
m4uint32_t ClMIT_MVC::m_uiCreatedMVCCount=0 ;

ClMIT_MVC::ClMIT_MVC() 
{
	m_poInsp=GET_INSP();
	m_poTaskEnv=GET_APICHN_TASK_ENV() ;
	m_poManager=NULL;	
	m_poSessChn=NULL ;		
	m_poService=NULL ;
	m_bSwExternVM=M4_TRUE ;
	ItemV_poDeclareExtFileChn=NULL ;
}

void ClMIT_MVC::End()
{
	m4return_t swSuccess;

	UpdateCancel() ;
	
	if (m_bSwExternVM ) {

		#ifdef APICHN_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()) {
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, 
				"ClMIT_MVC::End() ShutDown EXTERNAL MVC") << SEND_MSG   ;
		} ;
		#endif

		M4_DELETE_ITEM(m_poInsp->m_poMemProf, MIT_MGI_APICHN, m_poSessChn, ClMIT_SessionChn) ;
		m_poManager=NULL;	
		m_poService=NULL ;

	} else {

		#ifdef APICHN_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, 
				"ClMIT_MVC::End() ShutDown LOCAL MVC"
				"(all attached channels will be destroyed)" ) << SEND_MSG   ;
		} ;
		#endif

		M4_DELETE_ITEM(m_poInsp->m_poMemProf, MIT_MGI_APICHN, m_poSessChn, ClMIT_SessionChn) ;
		m_poManager=NULL ;

		if (m_poService) { 

			m_oMutexCreatedCounter.Lock() ;

			//Temporizacion
			m_poTaskEnv->Chn_m_oTimerStartVM.Start();

			swSuccess=ClFactM4ObjService::DestroyObjService(m_poService);
			m_poService=NULL ;

			m_poTaskEnv->Chn_m_oTimerStartVM.Stop();
			//Fin Temporizacion

			if ( swSuccess!= M4_SUCCESS ) {
				*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_160 ] ) << SEND_MSG ;
				//m_poInsp->m_poTrace->ImmediateError() ;
			} ;

			--m_uiCreatedMVCCount ;

			m_oMutexCreatedCounter.Unlock() ;
		} ;

	} ;
	ItemV_poDeclareExtFileChn=NULL ;
}

m4bool_t ClMIT_MVC::Init( ClChannelManager *ai_poManager, ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t swSuccess, Return=M4_TRUE ;

	ClMIT_MVC::End() ;

	m_bSwExternVM = M4_TRUE ;

	#ifdef APICHN_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "ClMIT_MVC::Init() MVC StartUp with EXTERNAL MVC") << SEND_MSG   ;
	} ;
	#endif

	m_poService=NULL;
	m_poManager=ai_poManager ;
	
	M4_NEW_ITEM(m_poInsp->m_poMemProf, MIT_MGI_APICHN, m_poSessChn, ClMIT_SessionChn) ;
	if ( ! m_poSessChn->Init(m_poManager,ClMIT_Trace::PROPAGATE_ERROR) ) {
		M4_DELETE_ITEM(m_poInsp->m_poMemProf, MIT_MGI_APICHN, m_poSessChn, ClMIT_SessionChn) ;
		Return=M4_FALSE ;
	} ;

	m_poInsp->m_poTrace->TestEffect( Return, ai_ErrorEffect ) ;

	return M4_BOOL(! UpdateCancel()  && Return);
}


m4bool_t ClMIT_MVC::Init( ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t swSuccess, Return=M4_TRUE ;

	m_oMutexCreatedCounter.Lock() ;

	ClMIT_MVC::End() ;

	m_bSwExternVM = M4_FALSE ;

	#ifdef APICHN_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "ClMIT_MVC::Init() MVC StartUp with LOCAL MVC") << SEND_MSG   ; 
	} ;
	#endif


	//Version SM con emulación, test de solo una MVC por PROCESO (no thread)

	if ( ClFactM4ObjService::GetM4ObjConfig()==ClFactM4ObjService::M4ObjEmulType ) {

		if ( m_uiCreatedMVCCount!=0 )  {	//No usar función, Mutex puesto
			// Error !!!
			*m_poInsp->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 4325472, 
				"No es posible tener mas de un Meta4 Object Engine en versiones single machine con SMOLTP a 1. Pruebe a poner este valor a 0.", ClMIT_Msg::MIXED_OUT) 
				<< SEND_MSG ;
			Return=M4_FALSE ;
			goto exit ;
		} ;
	} ;


	//Temporizacion
	m_poTaskEnv->Chn_m_oTimerStartVM.Start();
		
	m_poService=ClFactM4ObjService::CreateObjService( NULL, NULL, NULL, NULL, NULL, M4_FALSE ) ;
		
	m_poTaskEnv->Chn_m_oTimerStartVM.Stop();
	//Fin Temporizacion

	if ( ! m_poService ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_150 ] ) << SEND_MSG ;
		Return=M4_FALSE ;
		goto exit ;
	} ; 

	m_poManager = m_poService->GetpChannelManager() ;

	if ( ! m_poManager ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_150 ] ) << SEND_MSG ;
		Return=M4_FALSE ;
		goto exit ;
	} ; 

exit:

	m_poInsp->m_poTrace->TestEffect( Return, ai_ErrorEffect ) ;

	if ( Return ) ++m_uiCreatedMVCCount ;
	
	m_oMutexCreatedCounter.Unlock() ;

	return M4_BOOL(! UpdateCancel()  && Return );
}

m4return_t ClMIT_MVC::GetLanguage(m4language_t *ai_pLanguage) 
{
	m4return_t Return;

	Return = m_poManager->GetLogonAdaptor()->GetLanguage(*ai_pLanguage);

	return Return;
}

m4bool_t ClMIT_MVC::LogOn(
	m4char_t *ai_pcUser, m4char_t *ai_pcPassword,m4int16_t ai_Language,
	m4char_t *ai_pcServer, m4uint32_t ai_uiPort, m4bool_t ai_bSSLType)
{
	ClLogonAdaptor *poM4LogonAdaptor;
	m4return_t RetVal;

	#ifdef APICHN_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"Logging On User %0:s Password %1:s") 
			<< ai_pcUser << "Not allowed for security" << SEND_MSG   ;
	} ;
	#endif

	M4_DELETE_ITEM(m_poInsp->m_poMemProf, MIT_MGI_APICHN, m_poSessChn, ClMIT_SessionChn) ;

	if ( UpdateCancel() ) {
		RetVal=M4_FALSE ;
		goto exit ;
	} ;

	//Temporizacion
	#ifdef APICHN_TRACE
	m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	poM4LogonAdaptor = m_poManager->GetLogonAdaptor();
	RetVal = poM4LogonAdaptor ? M4_SUCCESS: M4_ERROR;
	
	if (RetVal==M4_SUCCESS) {
		RetVal = poM4LogonAdaptor->Logon(ai_pcUser, ai_pcPassword, ai_Language,
           ai_pcServer ? ai_pcServer : "??",    // Sin sentido en cliente
		   ai_uiPort,							// Sin sentido en cliente (Port)
           M4_TRUE,                 // Uso del Role por defecto
           "", ai_bSSLType );
    }
	RetVal = RetVal == M4_SUCCESS ? M4_TRUE: M4_FALSE;
	#ifdef APICHN_TRACE
	m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

	if (RetVal) {	
		M4_NEW_ITEM(m_poInsp->m_poMemProf, MIT_MGI_APICHN, m_poSessChn, ClMIT_SessionChn) ;
		if ( ! m_poSessChn->Init(m_poManager,ClMIT_Trace::PROPAGATE_ERROR) ) {
			M4_DELETE_ITEM(m_poInsp->m_poMemProf, MIT_MGI_APICHN, m_poSessChn, ClMIT_SessionChn) ;
			RetVal=M4_FALSE ;
		} ;
	} ;

exit:
	if (!RetVal) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_390 ] ) << SEND_MSG ;
	} ;
	return M4_BOOL(RetVal);
}

void ClMIT_MVC::LogOff() 
{
	ClLogonAdaptor *poM4LogonAdaptor;

	poM4LogonAdaptor = m_poManager->GetLogonAdaptor();

	if (poM4LogonAdaptor) poM4LogonAdaptor->LogOff() ;

	M4_DELETE_ITEM(m_poInsp->m_poMemProf, MIT_MGI_APICHN, m_poSessChn, ClMIT_SessionChn) ;

}


m4bool_t ClMIT_MVC::UpdateCancel() 
{
	if ( m_poService ) {
		if ( ! m_poInsp->m_poTrace->IsCancelNotified() ) {
			if ( m_poService->GetpExecutor()->GetRequestToAbortWithClStatus() ) {
				m_poInsp->m_poTrace->NotifyCancel() ;
			} ;
		} ;
	} ;
	return m_poInsp->m_poTrace->IsCancelNotified() ;
}

m4bool_t ClMIT_MVC::UpdateCancel(ClChannelManager * ai_poManager) 
{
	if ( ai_poManager ) { 
		if ( ! m_poInsp->m_poTrace->IsCancelNotified() ) {
			if ( ai_poManager->GetpExecutor()->GetRequestToAbortWithClStatus() ) {
					m_poInsp->m_poTrace->NotifyCancel() ;
			} ;
		} ;
	} ;
	return m_poInsp->m_poTrace->IsCancelNotified() ;
}


ClChannelManager *ClMIT_MVC::GetChannelManagerFromRunContext(ClVMRunContext * ai_poRunContext) 
{
	return ai_poRunContext->m_pAccess->GetpChannel()->GetpChannelManager() ;
}


ClAccess *ClMIT_MVC::GetAccessFromRunContext(ClVMRunContext * ai_poRunContext) 
{
	return ai_poRunContext->m_pAccess ;
}

m4uint32_t ClMIT_MVC::GetCreatedMVCCount() 
{
	m4uint32_t  R ;

	m_oMutexCreatedCounter.Lock() ;
	R = m_uiCreatedMVCCount ;
	m_oMutexCreatedCounter.Unlock() ;

	return R ;
}

m4bool_t ClMIT_MVC::StartUpDeclareExtFile()
{	
	ClChannelDirectory* pChanDir;
	ClChannel* pChanAux;
	m4pcchar_t pcAuxChanId=NULL;
	m4uint32_t iNumChan, iCount;
	m4bool_t SwSuccess=M4_TRUE;
	ClChannel* pChannelExecution=NULL;

	StMIT_APIChn_TaskEnv *ai_poTaskEnv = TM_GET_APICHN_TASK_ENV(GET_TM()) ;


	if(!  GetChannelManager()) {
		//Error
		SwSuccess=M4_FALSE;
		goto exit;
	};

	//Temporizacion
	#ifdef RPT_ENGINE_DEBUG
		ai_poTaskEnv->Chn_m_oTimerChannelAccess.Start();;
	#endif

	pChanDir = &(GetChannelManager()->Channel);
	iNumChan = pChanDir->Count();

	for(iCount=0; (iCount<iNumChan) && (!pChannelExecution); iCount++){
		
		pChanAux = (*pChanDir)[iCount];

		if(pChanAux->IsBuilt()){
			pcAuxChanId = pChanAux->GetpChannelDef()->Id();
		} else {
			pcAuxChanId = NULL;
		};

		if((pcAuxChanId) && (stricmp(pcAuxChanId, "C4_JS_EXE_JIT")==0)){
			//Se ha encontrado el canal
			pChannelExecution = pChanAux;
		};
	};

	if(pChannelExecution){
		//Se ha encontrado el canal, por lo que podemos crear un acceso
		ItemV_poDeclareExtFileChn = pChannelExecution->CreateAccess();
		
		if( ItemV_poDeclareExtFileChn){
			SwSuccess=M4_TRUE;
			goto exit;
		} else {
			//ERROR: Al crear el acceso
			SwSuccess=M4_FALSE;
			goto exit;
		};
	} else {
		//ERROR: No se ha encontrado el canal
		SwSuccess=M4_FALSE;
		goto exit;
	};

exit:

	#ifdef RPT_ENGINE_DEBUG
		ai_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin temporizacion

	return SwSuccess;
}

m4bool_t ClMIT_MVC::DeclareExternalFile(DECLARE_EXTERNAL_FILE ai_DeclareExtFile, m4char_t* ai_pcFilePath)
{
	ClNode* poNode;
	m4bool_t SwSuccess=M4_TRUE;
	StMIT_APIChn_TaskEnv *ai_poTaskEnv = TM_GET_APICHN_TASK_ENV(GET_TM()) ;

	//Temporizacion
	#ifdef RPT_ENGINE_DEBUG
		ai_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	
	if(ItemV_poDeclareExtFileChn){
		
		poNode = &(ItemV_poDeclareExtFileChn->Node["N4_JS_EXE_JIT"]);
		if(poNode){
			
			//Metemos los parametros en la pila
			m4VariantType ItemValue;

			//Path del fichero
			ItemValue.Type=M4CL_CPP_TYPE_STRING_VAR;
			ItemValue.Data.PointerChar = ai_pcFilePath;

			ItemV_poDeclareExtFileChn->GetpExecutor()->Stack.Push(ItemValue);

			//Tipo de accion
			ItemValue.Type=M4CL_CPP_TYPE_STRING_VAR;

			switch(ai_DeclareExtFile){
			case TEMPORARY:
				ItemValue.Data.PointerChar="TEMPORARY";
				break;
			
			case REFERENCE:
				ItemValue.Data.PointerChar="REFERENCE";
				break;
			
			case COPY:
				ItemValue.Data.PointerChar="COPY";
				break;
			
			case REFERENCE_AND_COPY:
				ItemValue.Data.PointerChar="REFERENCE_AND_COPY";
				break;
			};

			ItemV_poDeclareExtFileChn->GetpExecutor()->Stack.Push(ItemValue);

			//Dias de expiracion
			ItemValue.Type=M4CL_CPP_TYPE_NUMBER;
			ItemValue.Data.DoubleData=30; //Dias para que expire el fichero (chapucilla de UNIX que no se acaba de entender)
			ItemV_poDeclareExtFileChn->GetpExecutor()->Stack.Push(ItemValue);

			//Parámetro-flag para que funcione la declaración como antes de arreglar el 'bug?'.
			//Si se pasa un 1, borra el fichero siempre que pueda.

			ItemValue.Type=M4CL_CPP_TYPE_NUMBER;
			ItemValue.Data.DoubleData=1; 
			ItemV_poDeclareExtFileChn->GetpExecutor()->Stack.Push(ItemValue);

			//Ejecutamos el metodo LN4
			if(poNode->RecordSet.Current.Item["DECLARE_EXTERNAL_FILE"].Call(NULL, 4, m_poTaskEnv->bFromWrapper)==M4_ERROR){
				//Error en la ejecucion del metodo
				SwSuccess=M4_FALSE;
				goto exit;
			} else {
				//Sacamos el resultado de la pila
				ItemV_poDeclareExtFileChn->GetpExecutor()->Stack.Pop(ItemValue);

				if(ItemValue.Data.DoubleData!=0){
					//Error en la ejecucion del metodo
					SwSuccess=M4_FALSE;
					goto exit;
				};
			};

		} else {
			//ERROR: de posicionamiento en el Nodo
			SwSuccess=M4_FALSE;
			goto exit;
		};

	} else {
		//ERROR: Acceso no creado
		SwSuccess=M4_FALSE;
		goto exit;
	};

exit:

	#ifdef RPT_ENGINE_DEBUG
		ai_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin temporizacion

	return SwSuccess;
}


m4bool_t ClMIT_MVC::DestroyDeclareExtFileAccess()
{
	m4bool_t SwOk=M4_TRUE;

	StMIT_APIChn_TaskEnv *ai_poTaskEnv = TM_GET_APICHN_TASK_ENV(GET_TM()) ;

	//Temporizacion
	#ifdef RPT_ENGINE_DEBUG
		ai_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	
	if(ItemV_poDeclareExtFileChn) {
		SwOk = ! M4_BOOL(ItemV_poDeclareExtFileChn->Destroy());
		ItemV_poDeclareExtFileChn = NULL;
	};

exit:

	#ifdef RPT_ENGINE_DEBUG
		ai_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin temporizacion

	return SwOk;
}


//=============================================================== ClMIT_Chn

m4bool_t ClMIT_Chn::StartUpCurrency()
{
	m4bool_t swSuccess ;
	
	if ( UpdateCancel() ) return M4_FALSE ;
		
	*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"Starting up currency access") << SEND_MSG   ;
	
	//Temporizacion
	m_poTaskEnv->Chn_m_oTimerStartUpCurrency.Start();
	
	swSuccess = M4_BOOL(m_poChannel->Level2.GetL2Access(M4_INSTANCE_EXCHANGE_RATES, &m_poTaskEnv->ItemV_m_poCurrChn, M4_FALSE));
	
	m_poTaskEnv->Chn_m_oTimerStartUpCurrency.Stop();
	//Fin temporizacion

	if(swSuccess!=M4_SUCCESS){
		*(GET_INSP()->m_poTrace) << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_360 ] ) //Definirlo !!!
			<< GetIdDesc() << SEND_MSG ;
		m_poTaskEnv->ItemV_m_poCurrChn=NULL ;
		return M4_FALSE;
	}
	else{
		return M4_TRUE;
	};
}

m4bool_t ClMIT_Chn::ExchangeCurrency( ClMIT_Str * ao_poCur, const m4char_t* ai_pcDestCur,
		m4date_t ai_ExchDate, const m4char_t* ai_pcExchType, m4double_t *ao_dCurValue  )
{
	m4VariantType var;
	m4VariantType Args[5];
	m4return_t R=M4_TRUE;
	
	ClMIT_TM *TM = GET_TM() ;
	StMIT_Inspector *ai_poInsp = TM_GET_INSP(TM) ;
	StMIT_APIChn_TaskEnv *ai_poTaskEnv = TM_GET_APICHN_TASK_ENV(TM) ;

	if ( ai_poTaskEnv->m_poMVC->UpdateCancel() ) {
		R = M4_FALSE ;
		goto exit;
	} ;

	if(!ai_poTaskEnv->ItemV_m_poCurrChn){
		if ( !ai_poTaskEnv->Chn_m_poChnCurrency ) {
			*ai_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_340 ] ) << SEND_MSG ;
			R = M4_FALSE;
			goto exit;
		} ;
		if ( !ClMIT_ChnValue::StartUpCurrency(ai_poTaskEnv->Chn_m_poChnCurrency) ) {
			*ai_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_350 ] ) << SEND_MSG ;
			R = M4_FALSE;
			goto exit;
		} ;
	}

	*ai_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
		"Exchanging currency, from currency %0:s to currency %1:s, exchng date %2:s "
		"and type %3:s, value %4:s ") 
		
		<< ao_poCur->InChar(0)
		<< ai_pcDestCur
		<< ai_ExchDate
		<< ai_pcExchType
		<< *ao_dCurValue
		
		<< SEND_MSG   ;

	
	//Temporizacion
	#ifdef APICHN_TRACE
		ai_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	Args[0].Type=M4CL_CPP_TYPE_STRING_VAR;
	Args[0].Data.PointerChar=ao_poCur->InChar(0);
	ai_poTaskEnv->ItemV_m_poCurrChn->GetpExecutor()->Stack.Push(Args[0]);

	Args[1].Type=M4CL_CPP_TYPE_STRING_VAR;
	Args[1].Data.PointerChar=(m4char_t *)ai_pcDestCur;
	ai_poTaskEnv->ItemV_m_poCurrChn->GetpExecutor()->Stack.Push(Args[1]);

	Args[2].Type=M4CL_CPP_TYPE_DATE;
	Args[2].Data.DoubleData=ai_ExchDate ;
	ai_poTaskEnv->ItemV_m_poCurrChn->GetpExecutor()->Stack.Push(Args[2]);

	Args[3].Type=M4CL_CPP_TYPE_STRING_VAR;
	Args[3].Data.PointerChar=(m4char_t *)ai_pcExchType;
	ai_poTaskEnv->ItemV_m_poCurrChn->GetpExecutor()->Stack.Push(Args[3]);

	Args[4].Type=M4CL_CPP_TYPE_NUMBER;
	Args[4].Data.DoubleData=*ao_dCurValue;
	ai_poTaskEnv->ItemV_m_poCurrChn->GetpExecutor()->Stack.Push(Args[4]);
	
	R = ai_poTaskEnv->ItemV_m_poCurrChn->Node["CURRENCY"].RecordSet.Current.Item["curGetExchange"].Call(NULL, 5, ai_poTaskEnv->bFromWrapper);

	ai_poTaskEnv->ItemV_m_poCurrChn->GetpExecutor()->Stack.Pop(var);
	#ifdef APICHN_TRACE
		ai_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin temporizacion

	if(R!=M4_SUCCESS) {

		*ai_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"Executing currency channel") 
			<< CRAZY_MSG << SEND_MSG   ;
		goto exit;
	}	

	*ao_dCurValue=var.Data.DoubleData;
	ao_poCur->StrCpy(ai_pcDestCur);

exit:

	return M4_BOOL(R);
}

ClMIT_Chn::ClMIT_Chn() 
{
	m_poInsp=GET_INSP();
	m_poTaskEnv=GET_APICHN_TASK_ENV() ;
	m_poManager=NULL ;
	m_oFlag.Off(LINKED) ;
}

void ClMIT_Chn::End() 
{
	ClMIT_TM *TM = GET_TM() ;
	StMIT_Inspector *ai_poInsp = TM_GET_INSP(TM) ;
	StMIT_APIChn_TaskEnv *ai_poTaskEnv = TM_GET_APICHN_TASK_ENV(TM) ;

	//Destruction of all the nodes
	NodeList_t::iterator itListNode ;

	for (itListNode = m_oNodeList.begin (); itListNode!=m_oNodeList.end(); itListNode++)
	{
		if (((*itListNode).second) != NULL)	{
			M4_DELETE_ITEM(m_poInsp->m_poMemProf, MIT_MGI_APICHN, 
				(*itListNode).second, ClMIT_ChnNode 
			) ;
		} ;
	} ;

	m_oNodeList.clear();

	if ( IsLinked() ) {
		if ( !IsExtern() ) {

			#ifdef APICHN_TRACE
			if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
				*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
					"ClMIT_Chn::End() Destroying LOCAL channel %0:s") << 
					GetIdDesc() << SEND_MSG   ;
			} ;
			#endif

			Unload() ;

			//Temporizacion
			ai_poTaskEnv->Chn_m_oTimerChannelDestroy.Start();

			m_poAccess->Destroy() ;	 
			m_poChannel->Destroy() ;	 

			ai_poTaskEnv->Chn_m_oTimerChannelDestroy.Stop();
			//Fin Temporizacion

		} else {

			if ( IsPrivateAccess() ) {
				#ifdef APICHN_TRACE
				if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
					*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
						"ClMIT_Chn::End() EXTERNAL channel with PRIAVTE ACCESS %0:s") 
						<< GetIdDesc()<< SEND_MSG   ;
				} ;
				#endif

				m_poAccess->Destroy() ;	 

			} else {
				#ifdef APICHN_TRACE
				if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
					*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
						"ClMIT_Chn::End() EXTERNAL channel with COMMON ACCESS %0:s") 
						<< GetIdDesc()<< SEND_MSG   ;
				} ;
				#endif
				;
			} ;

		};
	} ;
	m_oFlag.Off(LINKED);

	m_oIdDesc.Init("","") ;
}

m4bool_t ClMIT_Chn::InitOwn(const m4char_t * ai_pcNameChn, m4int16_t ai_iMetadataLang, 
							ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect, ClAccess *ai_poMetaChnAccess, m4bool_t ai_bBuildAsMetaChannel) 
{
	m4bool_t swSuccess, Return=M4_TRUE ;
	
	ClMIT_Chn::End();

	#ifdef APICHN_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
			!ai_bBuildAsMetaChannel ? 
			"ClMIT_Chn::Init() Building LOCAL channel %0:s": "ClMIT_Chn::Init() Building LOCAL META CHANNEL %0:s") 
			<< ai_pcNameChn << SEND_MSG   ;
	};
	#endif

	//Temporizacion
	m_poTaskEnv->Chn_m_oTimerBuildFromId.Start();

	m_oFlag.Off(EXTERN_CHANNEL) ;
	m_oFlag.On( PRIVATE_ACCESS) ;
	m_poManager=m_poTaskEnv->m_poMVC->GetChannelManager();

	//ACTUALIZAR m_poManager ANTES  de llamar a Chn::UpadteCancel()
	if ( UpdateCancel() ) {
		Return = M4_FALSE ;
		goto exit ;
	} ;
	
	m_poManager->CreateChannel(m_poChannel) ;

	if (ai_iMetadataLang!=-1 && ai_iMetadataLang!=0) {
		m_poChannel->Property.SetPropValue("META_LANGUAGE", ai_iMetadataLang);
		m_poChannel->Property.SetPropValue("DATA_LANGUAGE", ai_iMetadataLang);
	};

	if ( ai_poMetaChnAccess ) {
		swSuccess =  M4_BOOL(m_poChannel->BuildFromAccess(ai_poMetaChnAccess));
	} else if (ai_bBuildAsMetaChannel) {
		swSuccess =  M4_BOOL(m_poChannel->BuildAsMetachannel());
	} else {
		swSuccess =  M4_BOOL(m_poChannel->BuildFromId(ai_pcNameChn));
	} ;

	m_poTaskEnv->Chn_m_oTimerBuildFromId.Stop();
	//Fin Temporizacion

	if ( swSuccess!= M4_SUCCESS ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_170 ] ) 
			<< ai_pcNameChn << SEND_MSG ;
		Return=M4_FALSE ;

		// Para que End no de error

		m_oFlag.Off( LINKED );

		//Temporizacion
		m_poTaskEnv->Chn_m_oTimerChannelDestroy.Start();

		m_poChannel->Destroy() ;	 

		m_poTaskEnv->Chn_m_oTimerChannelDestroy.Stop();
		//Fin Temporizacion

	} else {

		//Temporizacion
		#ifdef APICHN_TRACE
			m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
		#endif
		m_poAccess=m_poChannel->CreateAccess();
		m_oFlag.On( LINKED );
		#ifdef APICHN_TRACE
			m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
		#endif
		//Fin Temporizacion

	} ;

	if (Return) {
		m_oIdDesc.Init(m_poChannel->GetpChannelDef()->Id(),m_poChannel->GetpChannelDef()->Name()) ;
		SetAutoLoad(AUTOLOAD_OFF) ;
	} ;

exit:

	m_poInsp->m_poTrace->TestEffect( Return, ai_ErrorEffect ) ; 
	return Return ;		
}


m4bool_t  ClMIT_Chn::InitCommonAccess( ClAccess *ai_poAccess,ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t Return=M4_TRUE ;
	
	ClMIT_Chn::End();

	m_oFlag.On(EXTERN_CHANNEL) ;
	m_oFlag.Off(PRIVATE_ACCESS) ;

	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif

	// Bug 0220106. Cuando el parámetro ENG_LINK_DESIGN = 2, al acceso viene a nulo..
	if (ai_poAccess == NULL)
	{
		Return = M4_FALSE ;
		goto exit ;
	}

	m_poAccess  =ai_poAccess ;
	m_poChannel =m_poAccess->GetpChannel() ; 
	m_poManager= m_poChannel->GetpChannelManager() ;

	//ACTUALIZAR m_poManager ANTES  de llamar a Chn::UpadteCancel()
	if ( UpdateCancel() ) {
		Return = M4_FALSE ;
		goto exit ;
	} ;

	m_oIdDesc.Init(m_poChannel->GetpChannelDef()->Id(),m_poChannel->GetpChannelDef()->Name()) ;
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

	m_oFlag.On( LINKED );

	SetAutoLoad(AUTOLOAD_OFF) ;

	#ifdef APICHN_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "ClMIT_Chn::Init() Building EXTERNAL channel %0:s") 
			<< GetIdDesc() << SEND_MSG   ;
	};
	#endif

exit:
	m_poInsp->m_poTrace->TestEffect( Return, ai_ErrorEffect ) ; 
	return Return ;		
}

m4bool_t ClMIT_Chn::InitPrivateAccess( ClAccess *ai_poAccess, 
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect ) 
{
	return InitPrivateAccess(ai_poAccess->GetpChannel(),ai_ErrorEffect) ;
}


m4bool_t  ClMIT_Chn::InitPrivateAccess( ClChannel *ai_poChn,ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t Return=M4_TRUE ;
	
	ClMIT_Chn::End();

	m_oFlag.On(EXTERN_CHANNEL) ;
	m_oFlag.On(PRIVATE_ACCESS) ;

	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	m_poChannel =ai_poChn ; 
	m_poManager= m_poChannel->GetpChannelManager() ;

	//ACTUALIZAR m_poManager ANTES  de llamar a Chn::UpadteCancel()
	if ( UpdateCancel() ) {
		Return = M4_FALSE ;
		goto exit ;
	} ;

	m_poAccess= m_poChannel->CreateAccess();

	m_oIdDesc.Init(m_poChannel->GetpChannelDef()->Id(),m_poChannel->GetpChannelDef()->Name()) ;
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

	m_oFlag.On( LINKED );

	SetAutoLoad(AUTOLOAD_OFF) ;

	#ifdef APICHN_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "ClMIT_Chn::Init() Building EXTERNAL channel %0:s") 
			<< GetIdDesc() << SEND_MSG   ;
	} ;
	#endif

exit:
	m_poInsp->m_poTrace->TestEffect( Return, ai_ErrorEffect ) ; 
	return Return ;		
}

m4bool_t ClMIT_Chn::Load() 
{
	m4bool_t SwOk=M4_TRUE ;

	if ( UpdateCancel() ) return M4_FALSE;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "ClChn::Load() Loading channel  %0:s") 
			<< GetIdDesc() << SEND_MSG   ;
	};
	#endif

	if ( !IsAutoLoad() ) {
		//Temporizacion
		m_poTaskEnv->Chn_m_oTimerLoad.Start();
		
		if ( m_poChannel->Load(NULL, m_poTaskEnv->bFromWrapper) != M4_SUCCESS ) SwOk=M4_FALSE ;
		
		m_poTaskEnv->Chn_m_oTimerLoad.Stop();
		//Fin Temporizacion

		if ( !SwOk ) {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_310 ] ) 
				<< GetIdDesc() << SEND_MSG ;
			//Error local
		} ;
	} ;
	return SwOk ;
}

m4bool_t ClMIT_Chn::Unload() 
{
	m4bool_t SwOk=M4_TRUE ;

	#ifdef RPT_ENGINE_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, "ClChn::Unload() Unloading channel %0:s") 
			<< GetIdDesc() << SEND_MSG   ;
	};
	#endif

	//Temporizacion
	m_poTaskEnv->Chn_m_oTimerLoad.Start();
	
	if ( m_poChannel->ReleaseAll() != M4_SUCCESS) SwOk=M4_FALSE ;
	
	m_poTaskEnv->Chn_m_oTimerLoad.Stop();
	//Fin Temporizacion

	if ( !SwOk ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_320 ] ) 
			<< GetIdDesc() << SEND_MSG ;
		//Error local
	} ;
	return SwOk ;
}


ClMIT_Chn::AUTOLOAD_MODE ClMIT_Chn::GetAutoLoad() 
{
	m4VariantType v ;

	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	m_poChannel->WantsAutoLoad.Get(v) ;
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

	return (AUTOLOAD_MODE)(m4int16_t)v.Data.DoubleData ;
}

m4char_t * ClMIT_Chn::AutoloadModeToTraceStr(AUTOLOAD_MODE ai_Mode) 
{
	m4char_t *Str ;

	switch (ai_Mode) {
	case AUTOLOAD_OFF:
		Str="AUTOLOAD_OFF (ClMIT_Chn class will load the data)" ;
		break ;
	case AUTOLOAD_BLOCK:
		Str="AUTOLOAD_BLOCK (ClMIT_Chn class  will NOT!!! load the data)" ;
		break ;
	case AUTOLOAD_PRG:
		Str="AUTOLOAD_PRG (ClMIT_Chn class  will NOT!!! load the data)" ;
		break ;
	case AUTOLOAD_NODESAYS:
		Str="AUTOLOAD_NODESAYS (ClMIT_Chn class  will NOT!!! load the data)" ;
		break ;
	} ;
	return Str ;
}

void ClMIT_Chn::SetAutoLoad(ClMIT_Chn::AUTOLOAD_MODE ai_AutoLoadMode) 
{
	m4VariantType v ;

	#ifdef APICHN_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClMIT_Chn::SetAutoLoadLoad() AutoLoad is %0:s") 
			<< AutoloadModeToTraceStr(ai_AutoLoadMode) << SEND_MSG   ;
	} ;
	#endif

	v.Data.DoubleData = ai_AutoLoadMode ;
	v.Type = M4CL_CPP_TYPE_NUMBER ;

	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	m_poChannel->WantsAutoLoad.Set(v) ;
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion
}

m4bool_t ClMIT_Chn::IsAutoLoad() 
{
	return M4_BOOL(GetAutoLoad()!=AUTOLOAD_OFF);
}

void ClMIT_Chn::CheckPoint() 
{
	if ( UpdateCancel() ) return ;

	#ifdef APICHN_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"ClMIT_Chn::CheckPoint() Channel %0:s") 
			<< GetIdDesc() << SEND_MSG   ;
	};
	#endif

	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	m_poChannel->GetpChannel_Data()->CheckPoint() ;
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion
}


   
ClMIT_ChnNode * ClMIT_Chn::GetNode( const m4char_t* ai_pcNameNode, ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	ClMIT_ChnNode * Return ; 
	NodeList_t::iterator itListNode;
	ClMIT_Str NName ;

	NName.StrCpy(ai_pcNameNode) ;
	
	if ( UpdateCancel() ) {
		Return=NULL ;
		goto exit ;
	} ;

	#ifdef APICHN_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,"Searching node %0:s in channel %1:s") 
			<< ai_pcNameNode << GetIdDesc() << DETAILED_MSG << SEND_MSG   ;
	};
	#endif

	// Looking for the Node
		
	itListNode=m_oNodeList.find(NName);
	
	if (itListNode == m_oNodeList.end() ) 
	{	
		M4_NEW_ITEM(m_poInsp->m_poMemProf, MIT_MGI_APICHN, 
			Return, ClMIT_ChnNode 
		) ;
		if ( Return->Init( this, ai_pcNameNode, ClMIT_Trace::PROPAGATE_ERROR ) ) {
			//Add the Connection to the ArrayConnect
			m_oNodeList.insert(NodeList_t::value_type(NName,Return)); 	
		} else {
			M4_DELETE_ITEM(m_poInsp->m_poMemProf, MIT_MGI_APICHN, 
				Return, ClMIT_ChnNode 
			) ;
		} ;
	}
	else  // return the Node
	{	
		Return=((*itListNode).second);	
	}

exit:
	m_poInsp->m_poTrace->TestEffect( M4_BOOL(Return!=NULL), ai_ErrorEffect ) ; 
	return Return ;		
}


ClMIT_Chn::ORGANIZATION_TYPE ClMIT_Chn::GetOrganizationType() 
{
	m4double_t D ;

	m_poChannel->OrganizationType.Get(D) ;
	return (ORGANIZATION_TYPE)(m4int16_t)D ;
}


m4char_t *  ClMIT_Chn::GetOrganization() 
{
	m4char_t * R ;

	if (GetOrganizationType()==ORGANIZATION_MONO) {
		m_poChannel->Organization.Get(R) ;
		if ( ! R ) R = "" ;
	} else {
		R="" ;
	} ;
	return R ;
}


#define PUBLISH_NODE_NAME "PUB_DOC_IMPLEMENTS"
#define PUBLISH_NEW_METHOD_NAME "NEW_DOC"
#define PUBLISH_SAVE_FILE_METHOD_NAME "SAVE_FILE"
#define PUBLISH_END_METHOD_NAME "END_DOC"

m4uint32_t ClMIT_Chn::NewPublishGroup( const m4char_t *ai_ReportId, const m4char_t *ai_RootPath ) 
{
	m4uint32_t R=0 ;
	m4int16_t RAux ;
	ClMIT_ChnNode *node ;
	ClItem * item ;
	m4VariantType value ;
	IStackInterface * stack = &m_poAccess->GetpExecutor()->Stack  ;
	ClMIT_Str reportId, rootPath ;

	//Pillo nodo ....

	node = GetNode(PUBLISH_NODE_NAME, ClMIT_Trace::PROPAGATE_ERROR) ;
	if ( ! node ) goto exit ;

	//Pillo item para hacer el call ...

	item = &(node->pNode->Item[PUBLISH_NEW_METHOD_NAME]) ;
	if ( ! item ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_180 ] ) 
			<< GetIdDesc() << node->GetIdDesc() << PUBLISH_NEW_METHOD_NAME << SEND_MSG ;
		goto exit ;
	} ;
	

	//Empila ReportId
	value.Type = M4CL_CPP_TYPE_STRING_VAR ;
	value.Data.PointerChar = (m4char_t *)ai_ReportId ;
	stack->Push(value) ;

	//Empila RootPath
	value.Type = M4CL_CPP_TYPE_STRING_VAR ;
	value.Data.PointerChar = (m4char_t *)ai_RootPath ;
	stack->Push(value) ;

	//Llama
	RAux = item->Call(NULL, 2, m_poTaskEnv->bFromWrapper) ;

	//Pop valor retorno
	stack->Pop(value) ;

	//Cargo valor devuelto
	R = value.Data.DoubleData ;

	//Test todo OK
	if ( RAux != M4_SUCCESS || R==0 ) {
	
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_280 ] ) 
			<< GetIdDesc() << node->GetIdDesc() << PUBLISH_NEW_METHOD_NAME << SEND_MSG ;
		goto exit ;
	} ;

exit:
	return R ;
} ;

m4bool_t ClMIT_Chn::NotifyFileToPublish( 
		m4uint32_t ai_PublishGroupId, const m4char_t *ai_FilePath, m4bool_t ai_bIsStartFile ) 
{
	m4bool_t R=0 ;
	m4int16_t RAux ;
	ClMIT_ChnNode *node ;
	ClItem * item ;
	m4VariantType value ;
	IStackInterface * stack = &m_poAccess->GetpExecutor()->Stack  ;

	//Pillo nodo ....

	node = GetNode(PUBLISH_NODE_NAME, ClMIT_Trace::PROPAGATE_ERROR) ;
	if ( ! node ) goto exit ;

	//Pillo item para hacer el call ...

	item = &(node->pNode->Item[PUBLISH_SAVE_FILE_METHOD_NAME]) ;
	if ( ! item ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_180 ] ) 
			<< GetIdDesc() << node->GetIdDesc() << PUBLISH_SAVE_FILE_METHOD_NAME << SEND_MSG ;
		goto exit ;
	} ;
	
	//Empila ai_PublishGroupId
	value.Type = M4CL_CPP_TYPE_NUMBER ;
	value.Data.DoubleData = ai_PublishGroupId ;
	stack->Push(value) ;

	//Empila ai_FilePath
	value.Type = M4CL_CPP_TYPE_STRING_VAR ;
	value.Data.PointerChar = (m4char_t *)ai_FilePath ;
	stack->Push(value) ;

	//Empila ai_bIsStartFile
	value.Type = M4CL_CPP_TYPE_NUMBER ;
	value.Data.DoubleData = ai_bIsStartFile ? M4_TRUE : M4_FALSE ;
	stack->Push(value) ;

	//Llama
	RAux = item->Call(NULL, 3, m_poTaskEnv->bFromWrapper) ;

	//Pop valor retorno
	stack->Pop(value) ;

	//Cargo valor devuelto
	R = value.Data.DoubleData ;

	//Test todo OK
	if ( RAux != M4_SUCCESS  || ! R ) {
	
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_280 ] ) 
			<< GetIdDesc() << node->GetIdDesc() << PUBLISH_SAVE_FILE_METHOD_NAME << SEND_MSG ;
		goto exit ;
	} ;

exit:
	return R ;
}

m4bool_t ClMIT_Chn::EndPublishGroup(m4uint32_t ai_PublishGroupId) 
{
	m4bool_t R=0 ;
	m4int16_t RAux ;
	ClMIT_ChnNode *node ;
	ClItem * item ;
	m4VariantType value ;
	IStackInterface * stack = &m_poAccess->GetpExecutor()->Stack  ;

	//Pillo nodo ....

	node = GetNode(PUBLISH_NODE_NAME, ClMIT_Trace::PROPAGATE_ERROR) ;
	if ( ! node ) goto exit ;

	//Pillo item para hacer el call ...

	item = &(node->pNode->Item[PUBLISH_END_METHOD_NAME]) ;
	if ( ! item ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_180 ] ) 
			<< GetIdDesc() << node->GetIdDesc() << PUBLISH_END_METHOD_NAME << SEND_MSG ;
		goto exit ;
	} ;
	
	//Empila ai_PublishGroupId
	value.Type = M4CL_CPP_TYPE_NUMBER ;
	value.Data.DoubleData = ai_PublishGroupId ;
	stack->Push(value) ;

	//Llama
	RAux = item->Call(NULL, 1, m_poTaskEnv->bFromWrapper) ;

	//Pop valor retorno
	stack->Pop(value) ;

	//Cargo valor devuelto
	R = value.Data.DoubleData ;

	//Test todo OK
	if ( RAux != M4_SUCCESS || !R ) {
	
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_280 ] ) 
			<< GetIdDesc() << node->GetIdDesc() << PUBLISH_END_METHOD_NAME << SEND_MSG ;
		goto exit ;
	} ;

exit:
	return R ;
}



//=============================================================== ClMIT_ChnItem

ClMIT_ChnItem::ClMIT_ChnItem()
{
	m_poInsp = GET_INSP() ;
	m_poTaskEnv=GET_APICHN_TASK_ENV() ;
	m_poNode = NULL;
	m_poExternProp = NULL ;
}

void ClMIT_ChnItem::End() 
{
	m_poNode=NULL; 
	m_poExternProp = NULL ;
	m_oValue.End() ;
	m_oIdDesc.Init("","") ;
}

void ClMIT_ChnItem::Init(ClMIT_ChnItem  * ai_poSource ) 
{
	ClMIT_ChnItem::End() ;

	m_poNode=ai_poSource->m_poNode;
	if (!m_poNode) return ;
	m_oItemName.StrCpy(ai_poSource->m_oItemName);
	m_Type=ai_poSource->m_Type ;
	m_Scope=ai_poSource->m_Scope ;
	m_LoadMode=ai_poSource->m_LoadMode ;
	m_StartDate = ai_poSource->m_StartDate ;
	m_EndDate = ai_poSource->m_EndDate ;

	m_oValue.SetFixedType(M4_FALSE) ;
	m_oValue.Init( & ai_poSource->m_oValue ) ;
	m_oValue.SetFixedType(M4_TRUE) ;
	
	m_oIdDesc.Init(&ai_poSource->m_oIdDesc) ;
}

ClMIT_ChnItem::INIT_RETURN ClMIT_ChnItem::Init(ClMIT_ChnNode  * ai_poNode, const m4char_t * ai_pcNameItem,
	 ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect ) 
{
	INIT_RETURN		Return=ITEM_OK;
	m4uint8_t		iM4Type;
	
	ClMIT_ChnItem::End();

	m_poNode = ai_poNode;
	m_oItemName.StrCpy(ai_pcNameItem) ;


	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	ClItemDefIt_Random idir (m_poNode->pNode->GetpExtInfo ());
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

	ClItemDef item = idir [ai_pcNameItem];

	if ( ! item.GetHandle() )
	{
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_180 ] ) 
			<< GetChannelIdDesc() << GetNodeIdDesc() << ai_pcNameItem << SEND_MSG ;
		Return=ITEM_NOT_FOUND;
		goto exit;
	} else {
		m_oIdDesc.Init( item.Id(), item.Name() ) ;
	} ;
		
	m_Scope = (SCOPE)item.Scope() ;
	switch (m_Scope) 
	{
	case M4CL_ITEM_SCOPE_REGISTER:
		m_Scope=REGISTER ;
		break ;

	case M4CL_ITEM_SCOPE_BLOCK:
		m_Scope=BLOCK ;
		break ;

	case M4CL_ITEM_SCOPE_NODE:
		m_Scope=NODE ;
		break ;

	default:
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_190 ] ) 
			<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() << SEND_MSG ;
		Return=SCOPE_ERROR;
		goto exit;
		break ;
	} ;
	
    switch ( item.Type() )
	{
	case (M4CL_ITEM_TYPE_METHOD):
		m_Type = METHOD;
		break;		
	case (M4CL_ITEM_TYPE_PROPERTY):
		m_Type = PROPERTY;
		break;
	case (M4CL_ITEM_TYPE_FIELD):
		m_Type = FIELD;
		break;
	case (M4CL_ITEM_TYPE_CONCEPT):
		m_Type = CONCEPT;
		break;
	default:
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_200 ] ) 
			<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() << SEND_MSG ;
		Return=ITEM_TYPE_ERROR;
		goto exit;
		break;
	}
	
	m_oValue.SetFixedType(M4_FALSE) ;	//Por si Item es reinicializado
    switch ( item.CppType() )
	{
	case (M4CL_CPP_TYPE_NUMBER):

		iM4Type = item.M4Type();
		if (iM4Type == M4CL_M4_TYPE_CURRENCY) 
		{
			m_oValue.InitCurrency(0,item.Precision(), item.Scale(),"", ClMIT_Dates::MinusInfiniteDate(), "") ;
			
		}
		else if (iM4Type == M4CL_M4_TYPE_NUM_VARIANT)
		{
			m_oValue.InitVariant("", ClMIT_ChnValue::NO_INIT);
		}
		else 
		{
			m_oValue.InitDouble(0.0, item.Precision(), item.Scale() ) ;
		}

		break;

	case (M4CL_CPP_TYPE_DATE):
		if ( item.M4Type() == M4CL_M4_TYPE_DATE )  m_oValue.InitDate(0) ;
		else m_oValue.InitTimeStamp(0) ;
		break;

	case (M4CL_CPP_TYPE_STRING):
		if (item.M4Type() == M4CL_M4_TYPE_VARIANT)
		{
			m_oValue.InitVariant("", ClMIT_ChnValue::NO_INIT);
		}
		else
		{
			m_oValue.InitString("");
		}

		break;

	default:
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_210 ] ) 
			<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() << SEND_MSG ;
		m_oValue.SetFixedType(M4_TRUE) ;
		Return=VALUE_TYPE_ERROR;
		goto exit;
		break;
	}
	m_oValue.SetFixedType(M4_TRUE) ;
	m_oValue.ClearUpdated() ;
	SetLoadMode( ITEM_LOAD );
		
exit:
	m_poInsp->m_poTrace->TestEffect( M4_BOOL(Return==ITEM_OK), ai_ErrorEffect ) ; 
	return Return ;		
}

ClMIT_ChnItem::INIT_RETURN ClMIT_ChnItem::InitBoolean(ClMIT_ChnNode  * ai_poNode, const m4char_t * m_pcNameItem,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{

	INIT_RETURN Return=ClMIT_ChnItem::Init(ai_poNode, m_pcNameItem, ai_ErrorEffect ) ;
	if ( Return==ITEM_OK ) {
		if ( m_oValue.GetType() != ClMIT_ChnValue::NUMERIC ) {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_220 ] ) 
				<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() << SEND_MSG ;
			Return=ITEM_EXTEND_TYPE_ERROR ;
		} else {
			m_oValue.SetFixedType(M4_FALSE) ;
			m_oValue.InitBoolean(M4_FALSE) ;
			m_oValue.SetFixedType(M4_TRUE) ;
			m_oValue.ClearUpdated() ;
		} ;
	} ;
	m_poInsp->m_poTrace->TestEffect( M4_BOOL(Return==ITEM_OK), ai_ErrorEffect ) ; 
	return Return ;		
}

m4bool_t ClMIT_ChnItem::InitLoad(ClMIT_ChnNode  * ai_poNode, const m4char_t * m_pcNameItem, 
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect, 
		INIT_RETURN  *ao_Return)
{
	if (ai_ErrorEffect==ClMIT_Trace::PROPAGATE_ERROR) {
		*ao_Return=Init(ai_poNode,m_pcNameItem,ai_ErrorEffect) ;
		if (*ao_Return==ITEM_OK) return Load() ;
		else return M4_FALSE ;
	} else {
		Init(ai_poNode,m_pcNameItem,ai_ErrorEffect) ;
		return Load() ;
	};
}

m4bool_t ClMIT_ChnItem::InitBooleanLoad(ClMIT_ChnNode  * ai_poNode, const m4char_t * m_pcNameItem, 
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect, 
		INIT_RETURN  *ao_Return)
{
	if (ai_ErrorEffect==ClMIT_Trace::PROPAGATE_ERROR) {
		*ao_Return=InitBoolean(ai_poNode,m_pcNameItem,ai_ErrorEffect) ;
		if (*ao_Return==ITEM_OK) return Load() ;
		else return M4_FALSE ;
	} else {
		InitBoolean(ai_poNode,m_pcNameItem,ai_ErrorEffect) ;
		return Load() ;
	};
}


m4return_t ClMIT_ChnItem::ConvertStringToDouble (m4pcchar_t ai_pccItemValue, m4double_t & ao_dDoubleValue)
{
  	m4return_t	iRet = M4_SUCCESS;
	m4pchar_t	pStopStr = NULL;
	m4double_t	dItemValue = 0;

	dItemValue = strtod(ai_pccItemValue, &pStopStr);
	if (*pStopStr != '\0')
	{
		iRet = M4_ERROR;
	}
	else
	{
		ao_dDoubleValue = atof(ai_pccItemValue);
	}

	return iRet;
}

m4return_t ClMIT_ChnItem::ConvertStringToDate(m4pcchar_t ai_pDateValue, m4uint8_t ai_uiItemM4Type, m4double_t & ao_dDateValue)
{
	M4ClTimeStamp	oTimeStamp;
	ClMIT_Str		sDateConverted;
	ClMIT_Str		sTime;
	m4return_t		iRet = M4_SUCCESS;

	// Check M4 type and format date to timestamp.
	if (ai_uiItemM4Type == M4CL_M4_TYPE_TIMESTAMP)
	{
		sDateConverted = ai_pDateValue;
	}
	else if (ai_uiItemM4Type == M4CL_M4_TYPE_DATE)
	{
		ClMIT_Dates::GetStringDate(&sDateConverted, ai_pDateValue);
	}
	else if (ai_uiItemM4Type == M4CL_M4_TYPE_HOUR)
	{
		// sTimeStamp = "1970/09/25 " + ai_sDateValue;
		sDateConverted = ai_pDateValue;
	}

	ao_dDateValue = ClMIT_Dates::StringToDate(sDateConverted.InChar(0));

	return iRet;
}

void ClMIT_ChnItem::SetLoadMode( LOAD_MODE ai_LoadMode, m4date_t ai_StartDate, m4date_t ai_EndDate ) 
{
	m_LoadMode=ai_LoadMode ;
	m_StartDate=ai_StartDate ;
	m_EndDate=ai_EndDate ;
}

m4uint32_t ClMIT_ChnItem::GetTotalizeCount() 
{
	m4VariantType var;

	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	m_poNode->pNode->RecordSet.Register.Item[m_oItemName] ;
	m_poNode->pNode->RecordSet.GetFooter().Get(var,M4CL_ITEM_TOTALIZE_COUNT) ;
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

	return (m4uint32_t)var.Data.DoubleData ;
}

m4return_t ClMIT_ChnItem::GetCurrencyData(m4char_t const * ai_pItemName, m4char_t *& aio_pCurrency, m4date_t & aio_dExchDate,  m4char_t *& aio_pExchangeType)
{
	m4return_t			iRet = M4_SUCCESS;

	m4VariantType		vIDCurrency;
	m4VariantType		vExchangeDate;
	m4VariantType		vExchangeType;
	
	ClItemDef			oItemDefCurrency;
	ClItemDef			oItemDefIDCurrency;
	ClItemDef			oItemDefExchangeDate;
	
	m4pcchar_t			pItemAuxIDCurrency = NULL;
	m4pcchar_t			pItemAuxExchangeDate = NULL;
	m4pcchar_t			pItemAuxExchangeType = NULL;

	ClItemDefIt_Random	oIDir(m_poNode->pNode->GetpExtInfo());

	// Inicializar
	aio_pCurrency = NULL;
	aio_dExchDate = ClMIT_Dates::MinusInfiniteDate();
	aio_pExchangeType = NULL;


	// Definición del item de tipo moneda
	oItemDefCurrency = oIDir[ai_pItemName];

	// Obligatorio
	pItemAuxIDCurrency = oItemDefCurrency.AuxiliarItem();
	if (pItemAuxIDCurrency == NULL || ClMIT_Str::StrLenTrim(pItemAuxIDCurrency) == 0)
	{
		return M4_ERROR;
	}
	
	if (m_poNode->pNode->RecordSet.Current.Item[pItemAuxIDCurrency].Value.Get(vIDCurrency) != M4_SUCCESS)
	{
		return M4_ERROR;
	}

	if (vIDCurrency.Type != M4CL_CPP_TYPE_STRING_VAR)
	{
		return M4_ERROR;
	}

	aio_pCurrency = vIDCurrency.Data.PointerChar;

	// Opcional
	oItemDefIDCurrency = oIDir[pItemAuxIDCurrency];
	pItemAuxExchangeDate = oItemDefIDCurrency.AuxiliarItem();
	if (pItemAuxExchangeDate != NULL && ClMIT_Str::StrLenTrim(pItemAuxExchangeDate) > 0)
	{
		iRet = m_poNode->pNode->RecordSet.Current.Item[pItemAuxExchangeDate].Value.Get(vExchangeDate);
		if (iRet == M4_SUCCESS && vExchangeDate.Type == M4CL_CPP_TYPE_DATE)
		{
			aio_dExchDate = vExchangeDate.Data.DoubleData;
		}
	}
	
	// Opcional
	oItemDefExchangeDate = oIDir[pItemAuxExchangeDate];
	pItemAuxExchangeType = oItemDefExchangeDate.AuxiliarItem();
	if (pItemAuxExchangeType != NULL && ClMIT_Str::StrLenTrim(pItemAuxExchangeType) > 0)
	{
		iRet = m_poNode->pNode->RecordSet.Current.Item[pItemAuxExchangeType].Value.Get(vExchangeType);
		if (iRet == M4_SUCCESS && vExchangeType.Type == M4CL_CPP_TYPE_STRING_VAR)
		{
			aio_pExchangeType = vExchangeType.Data.PointerChar;
		}
	}
	
	return M4_SUCCESS;
}


m4return_t ClMIT_ChnItem::SetCurrencyData(m4char_t const * ai_pItemName, m4char_t * ai_pCurrency, m4date_t ai_dExchDate, m4char_t * ai_pExchangeType)
{	
	m4pcchar_t			pIDCurrency = NULL;
	m4pcchar_t			pExchangeDate = NULL;
	m4pcchar_t			pExchangeType = NULL;
	
	m4VariantType		vCurrencyID;
	m4VariantType		vExchangeDate;
	m4VariantType		vExchandeType;
	
	ClItemDef			oItemDefCurrency;
	ClItemDef			oItemDefIDCurrency;
	ClItemDef			oItemDefExchangeDate;
	
	ClItemDefIt_Random	oIDir(m_poNode->pNode->GetpExtInfo());
	
	// Item auxiliar del item moneda
	// Obligatorio
	oItemDefCurrency = oIDir[ai_pItemName];
	pIDCurrency = oItemDefCurrency.AuxiliarItem();
	if (pIDCurrency == NULL || ClMIT_Str::StrLenTrim(pIDCurrency) == 0)
	{
		return M4_ERROR;
	}
	
	vCurrencyID.Type = M4CL_CPP_TYPE_STRING_VAR; 
	vCurrencyID.Data.PointerChar = ai_pCurrency;
	if (m_poNode->pNode->RecordSet.Current.Item[pIDCurrency].Value.Set(vCurrencyID) != M4_SUCCESS)
	{
		return M4_ERROR;
	} 
	
	// Opcional
	oItemDefIDCurrency = oIDir[pIDCurrency];
	pExchangeDate = oItemDefIDCurrency.AuxiliarItem();
	if (pExchangeDate == NULL || ClMIT_Str::StrLenTrim(pExchangeDate) == 0)
	{
		return M4_SUCCESS;
	}
			
	vExchangeDate.Type = M4CL_CPP_TYPE_DATE; 
	vExchangeDate.Data.DoubleData = ai_dExchDate;
	m_poNode->pNode->RecordSet.Current.Item[pExchangeDate].Value.Set(vExchangeDate);
	
	// Opcional
	oItemDefExchangeDate = oIDir[pExchangeDate];
	pExchangeType = oItemDefExchangeDate.AuxiliarItem();
	if (pExchangeType == NULL || ClMIT_Str::StrLenTrim(pExchangeType) == 0)
	{
		return M4_SUCCESS;
	}
	
	vExchandeType.Type = M4CL_CPP_TYPE_STRING_VAR; 
	vExchandeType.Data.PointerChar = ai_pExchangeType;
	m_poNode->pNode->RecordSet.Current.Item[pExchangeType].Value.Set(vExchandeType);
	
	return M4_SUCCESS;
}


m4bool_t ClMIT_ChnItem::Load() {
		
	m4VariantType var;
	m4bool_t R = M4_TRUE ;
	m4int16_t GetR = M4_SUCCESS ;
	
	ClItemDefIt_Random	idir(m_poNode->pNode->GetpExtInfo());
	ClItemDef			ItemDef = idir [m_oItemName];
	m4char_t const *	ItemAux = NULL;
	m4char_t *			pValue =  NULL;
	m4double_t			dValue = 0;
	m4double_t			dDoubleValue = 0;
	m4date_t			dDateValue = 0;
	m4VariantType		vVariantType;
	m4uint8_t			uiCppType = M4CL_CPP_TYPE_NULL;

	m4double_t			dCurrValue = 0;
	m4date_t			ExchDate = 0;
	m4char_t *			pcCurr = NULL;
	m4char_t *			pcExchType = NULL;
	m4VariantType		varauxCurr, varauxExchDate,varauxExchType ;
	m4bool_t			RCurr = M4_TRUE;
	m4return_t			iRet = M4_SUCCESS;
	m4uint8_t			iM4Type;

	if ( UpdateCancel() ) {
		m_oValue.SetNULL() ;
		return M4_FALSE ;
	} ;

	if (GetType()!=METHOD) {
		//Temporizacion
		#ifdef APICHN_TRACE
			m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
		#endif
		switch( m_LoadMode )
		{
		case NULL_LOAD :

			m_oValue.SetNULL(m_oValue.GetType());

			return R;

		case ITEM_LOAD :
		
			GetR = m_poNode->pNode->RecordSet.Current.Item[m_oItemName].Value.Get(var);

			break;

		case SLICE_LOAD :
			
			MoveToSlice(m_Slice) ;
			GetR = m_poNode->pNode->RecordSet.Current.Item.Value[(ClSliceIndex)m_Slice].Get(var);

			break;

		case RANGE_SLICE_LOAD :
		
			GetR = m_poNode->pNode->RecordSet.Current.Item[m_oItemName].Value.
				Get( m_StartDate, m_EndDate, var ) ;

			break ;

		case GUIDE_LOAD :
		
			if ( m_poExternProp ) {
				GetR = m_poNode->pNode->RecordSet.Current.Item[m_oItemName].Value.
					Get( m_poExternProp->GetStartDate(), m_poExternProp->GetEndDate(), var ) ;
			} else {
				GetR = m_poNode->pNode->RecordSet.Current.Item[m_oItemName].Value.
					Get( GetFirstStartDate(), GetLastEndDate(), var ) ;
			} ;

			break ;

		case TOTALIZE_DESIGN:

			m_poNode->pNode->RecordSet.Current.Item[m_oItemName] ;
			GetR = m_poNode->pNode->RecordSet.GetFooter().Get(var,TOTALIZE_DESIGN) ;

			break;

		case TOTALIZE_SLICE_DESIGN:

			GetR = m_poNode->pNode->RecordSet.Current.Item[m_oItemName].Value.
				GetpSliceFooter().Get(var,TOTALIZE_DESIGN) ;

			break;

		default:

			if ( m_LoadMode < TOTALIZE_SLICE_OFF ) {

				m_poNode->pNode->RecordSet.Register.Item[m_oItemName] ;
				GetR = m_poNode->pNode->RecordSet.GetFooter().Get(var,m_LoadMode) ;

			} else {

				GetR = m_poNode->pNode->RecordSet.Current.Item[m_oItemName].Value.
					GetpSliceFooter().Get(var,m_LoadMode-TOTALIZE_SLICE_OFF) ;
			} ;
			break ;
		} ;
		#ifdef APICHN_TRACE
			m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
		#endif
		//Fin Temporizacion

		if ( GetR != M4_SUCCESS ) {
			//Error de efecto local
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_290 ] ) 
				<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() << SEND_MSG ;
			R=M4_FALSE ;
		} else { 
			R = var.Type==M4CL_CPP_TYPE_NULL ? M4_FALSE : M4_TRUE ;
		} ;

		switch ( m_oValue.GetType() ){
		case ( ClMIT_ChnValue::NUMERIC ):
			m_oValue.InitDouble( R ? var.Data.DoubleData : 0 );
			break;
		case ( ClMIT_ChnValue::DATE ):
			m_oValue.InitDate(var.Data.DoubleData);
			break;
		case ( ClMIT_ChnValue::TIME_STAMP ):
			m_oValue.InitTimeStamp(var.Data.DoubleData);
			break;
		case (ClMIT_ChnValue::STRING):
			m_oValue.InitString( R ? var.Data.PointerChar : "" ); 
			break;
		case (ClMIT_ChnValue::BOOLEAN):
			m_oValue.InitBoolean( R ? (M4_BOOL(var.Data.DoubleData == DOUBLE_VALID_BOOLEAN_TRUE)) : M4_FALSE );
			break;
		case (ClMIT_ChnValue::CURRENCY):
			{
				//Temporizacion
				#ifdef APICHN_TRACE
					m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
				#endif

				if(var.Type==M4CL_CPP_TYPE_NULL){
					m_oValue.SetNULL();
					RCurr=M4_TRUE;
					R=M4_FALSE;
					goto exit;
				}else{
					dCurrValue = R ? var.Data.DoubleData : 0 ;
				};

				if (GetCurrencyData(m_oItemName, pcCurr, ExchDate, pcExchType) != M4_SUCCESS)
				{
					RCurr = M4_FALSE;
					goto exit;
				}

exit:
				#ifdef APICHN_TRACE
					m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
				#endif
				//Fin Temporizacion
		
				m_oValue.InitCurrency(dCurrValue,pcCurr, ExchDate, pcExchType);

				if (!RCurr) {
					*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_330 ] ) 
						<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() << SEND_MSG ;
				} ;
				R=M4_BOOL(R && RCurr);
			} ;
			break;

		case (ClMIT_ChnValue::VARIANT):

			//Temporizacion
			#ifdef APICHN_TRACE
				m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
			#endif

			if (var.Type == M4CL_CPP_TYPE_NULL)
			{
				m_oValue.SetNULL();
				R = M4_FALSE;
				break;
			}
			else if (var.Type == M4CL_CPP_TYPE_NUMBER || var.Type == M4CL_CPP_TYPE_DATE)
			{
				dValue = var.Data.DoubleData;
			}
			else
			{
				pValue = var.Data.PointerChar;
			}

			ItemAux =  ItemDef.AuxiliarItem();
			if (ItemAux != NULL && ClMIT_Str::StrLenTrim(ItemAux) > 0)
			{
				iRet = m_poNode->pNode->RecordSet.Current.Item[ItemAux].Value.Get(vVariantType);
				if (iRet != M4_SUCCESS || vVariantType.Type != M4CL_CPP_TYPE_NUMBER)
				{
					// No se ha podido recuperar el valor del item auxiliar.
					*m_poInsp->m_poTrace << ClMIT_Msg(&TRACE_ARRAY_EX[APICHN_MSG_1]) << GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() << SEND_MSG;
					R = M4_FALSE;
					break;
				}
				
				// Obtener el tipo cpp del item a partir del tipo meta4.
				uiCppType = ConvertM4TypeToCppType(vVariantType.Data.DoubleData);
				if (uiCppType == M4CL_CPP_TYPE_NUMBER)
				{
					if (pValue != NULL)
					{
						if (ConvertStringToDouble(pValue, dDoubleValue) != M4_SUCCESS)
						{
							*m_poInsp->m_poTrace << ClMIT_Msg(&TRACE_ARRAY_EX[APICHN_MSG_2]) << GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() << pValue << SEND_MSG;
							R = M4_FALSE;
							break;
						}
					}
					else
					{
						dDoubleValue = dValue;
					}

					if (vVariantType.Data.DoubleData == M4CL_M4_TYPE_CURRENCY)
					{
						if (GetCurrencyData(ItemAux, pcCurr, ExchDate, pcExchType) != M4_SUCCESS)
						{
							*m_poInsp->m_poTrace << ClMIT_Msg(&TRACE_ARRAY[ APICHN_MSG_330]) << GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() << SEND_MSG;
							R = M4_FALSE;
							break;
						}
						m_oValue.InitVariant(dDoubleValue, pcCurr, ExchDate, pcExchType, ClMIT_ChnValue::CURRENCY);
					}
					else
					{
						m_oValue.InitVariant(dDoubleValue, ClMIT_ChnValue::NUMERIC);
					}
				}
				else if (uiCppType == M4CL_CPP_TYPE_STRING)
				{
					m_oValue.InitVariant(pValue, ClMIT_ChnValue::STRING);
				}
				else if (uiCppType == M4CL_CPP_TYPE_DATE)
				{
					if (pValue != NULL)
					{
						if (ConvertStringToDate(pValue, vVariantType.Data.DoubleData, dDateValue) != M4_SUCCESS)
						{
							*m_poInsp->m_poTrace << ClMIT_Msg(&TRACE_ARRAY_EX[APICHN_MSG_2]) << GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() << pValue << SEND_MSG;
							R = M4_FALSE;
							break;
						}
					}
					else
					{
						dDateValue = dValue;
					}

					if (vVariantType.Data.DoubleData == M4CL_M4_TYPE_DATE)
					{
						m_oValue.InitVariant(dDateValue, ClMIT_ChnValue::DATE);
					}
					else
					{
						m_oValue.InitVariant(dDateValue, ClMIT_ChnValue::TIME_STAMP);
					}
				}
			}
			else
			{
				// Bug 0304772. Para los tipos variant y variant number que no tienen item auxiliar
				// asociado: establecer un valor por defecto.
				iM4Type = ItemDef.M4Type();
				if (ItemDef.M4Type() == M4CL_M4_TYPE_VARIANT)
				{
					m_oValue.InitVariant(pValue, ClMIT_ChnValue::STRING);
				}
				else if (iM4Type == M4CL_M4_TYPE_NUM_VARIANT)
				{
					m_oValue.InitVariant(dValue, ClMIT_ChnValue::NUMERIC);
				}
			}

			#ifdef APICHN_TRACE
					m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
			#endif

			break;

		default:
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_200 ] ) 
				<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() << SEND_MSG ;
			m_poInsp->m_poTrace->ImmediateError() ;

			break ;
		} ;

		if (!R) m_oValue.SetNULL() ;	


		#ifdef APICHN_TRACE
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()) {
		
			ClMIT_Str Str ;
			m_oValue.GetInputFormat( &Str ) ;

			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
				"ClMIT_ChnItem::Load() Channel %0:s node %1:s item %2:s mode %3:s, value is %4:s") 
				<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() << GetStringLoadMode() 
				<< Str.InChar(0) << CRAZY_MSG << SEND_MSG   ;
		} ;
		#endif
	} ;

	return R ;
}

void ClMIT_ChnItem::Save() {
	
	m4VariantType			var;
	m4bool_t				swSuccess;
	ClMIT_ChnValue::TYPE	eRealType = ClMIT_ChnValue::NO_INIT;
	m4double_t				dValue = 0;
	
	ClItemDefIt_Random		idir (m_poNode->pNode->GetpExtInfo());
	ClItemDef				ItemDef;
	m4char_t const *		ItemAux;
		
	if ( UpdateCancel() ) return ;

	switch( m_oValue.GetType() ) 
	{
	case ClMIT_ChnValue::NUMERIC:
		var.Type = M4CL_CPP_TYPE_NUMBER;
		var.Data.DoubleData = m_oValue.GetDouble();

		//Temporizacion
		#ifdef APICHN_TRACE
			m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
		#endif
		swSuccess = M4_BOOL(m_poNode->pNode->RecordSet.Current.Item[m_oItemName].Value.Set(var));
		#ifdef APICHN_TRACE
			m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
		#endif
		//Fin Temporizacion

		if ( swSuccess != M4_SUCCESS ) {
			//Error de efecto local
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_300 ] ) 
				<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() << SEND_MSG ;
		} ;
		break ;

	case ClMIT_ChnValue::TIME_STAMP:
	case ClMIT_ChnValue::DATE:
		var.Type = M4CL_CPP_TYPE_DATE;
		var.Data.DoubleData = m_oValue.GetDate();

		//Temporizacion
		#ifdef APICHN_TRACE
			m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
		#endif
		swSuccess = M4_BOOL(m_poNode->pNode->RecordSet.Current.Item[m_oItemName].Value.Set(var));
		#ifdef APICHN_TRACE
			m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
		#endif
		//Fin Temporizacion

		if ( swSuccess != M4_SUCCESS ) {
			//Error de efecto local
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_300 ] ) 
				<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() << SEND_MSG ;
		} ;
		break ;

	case ClMIT_ChnValue::STRING:
		var.Type = M4CL_CPP_TYPE_STRING_VAR; 
		var.Data.PointerChar=m_oValue.GetString() ;

		//Temporizacion
		#ifdef APICHN_TRACE
			m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
		#endif

		swSuccess = M4_BOOL(m_poNode->pNode->RecordSet.Current.Item[m_oItemName].Value.Set(var));
		#ifdef APICHN_TRACE
			m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
		#endif
		//Fin Temporizacion

		if ( swSuccess != M4_SUCCESS ) {
			//Error de efecto local
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_300 ] ) 
				<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() << SEND_MSG ;
		} ;
		break ;
	case ClMIT_ChnValue::BOOLEAN:
		var.Type = M4CL_CPP_TYPE_NUMBER;
		var.Data.DoubleData = m_oValue.GetBoolean() ? DOUBLE_VALID_BOOLEAN_TRUE : DOUBLE_VALID_BOOLEAN_FALSE ;
		
		//Temporizacion
		#ifdef APICHN_TRACE
			m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
		#endif
		swSuccess = M4_BOOL(m_poNode->pNode->RecordSet.Current.Item[m_oItemName].Value.Set(var));
		#ifdef APICHN_TRACE
			m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
		#endif
		//Fin Temporizacion

		if ( swSuccess != M4_SUCCESS ) {
			//Error de efecto local
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_300 ] ) 
				<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() << SEND_MSG ;
		} ;
		break ;

	case (ClMIT_ChnValue::CURRENCY):
	{
		m4VariantType varaux ;
		m4bool_t RCurr=M4_TRUE;

		//Temporizacion
		#ifdef APICHN_TRACE
			m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
		#endif
		
		ItemDef = idir [m_oItemName];

		var.Type = M4CL_CPP_TYPE_NUMBER;
		var.Data.DoubleData = m_oValue.GetCurrencyValue();
		if ( m_poNode->pNode->RecordSet.Current.Item[m_oItemName].Value.Set(var) != M4_SUCCESS ) {
			RCurr=M4_FALSE ;
		} ;
		
		if (SetCurrencyData(m_oItemName, m_oValue.GetCurrencyType(), m_oValue.GetCurrencyExchDate(), m_oValue.GetCurrencyExchType()) != M4_SUCCESS)
		{
			RCurr=M4_FALSE ;
		}

		#ifdef APICHN_TRACE
			m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
		#endif
		//Fin Temporizacion

		if (!RCurr) {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_330 ] ) 
				<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() << SEND_MSG ;
		} ;
	} ;

	break;
	
	case (ClMIT_ChnValue::VARIANT):
	{
		//Temporizacion
		#ifdef APICHN_TRACE
			m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
		#endif
		
		eRealType = m_oValue.GetRealType();
		if (eRealType == ClMIT_ChnValue::NUMERIC || eRealType == ClMIT_ChnValue::CURRENCY ||
			eRealType == ClMIT_ChnValue::DATE || eRealType == ClMIT_ChnValue::TIME_STAMP)
		{	
			var.Data.DoubleData = m_oValue.GetVariantNumber();				
			if (eRealType == ClMIT_ChnValue::NUMERIC)
			{
				var.Type = M4CL_CPP_TYPE_NUMBER;
			}
			else if (eRealType == ClMIT_ChnValue::DATE || eRealType == ClMIT_ChnValue::TIME_STAMP)
			{
				var.Type = M4CL_CPP_TYPE_DATE;
			}
			else if (eRealType == ClMIT_ChnValue::CURRENCY)
			{
				var.Type = M4CL_CPP_TYPE_NUMBER;

				// Obtener el item auxiliar del tipo variant (el item que indica su tipo real).
				ItemDef = idir[m_oItemName];
				ItemAux = ItemDef.AuxiliarItem();
				if (ItemAux != NULL && ClMIT_Str::StrLenTrim(ItemAux) > 0)
				{
					if (SetCurrencyData(ItemAux, m_oValue.GetCurrencyType(), m_oValue.GetCurrencyExchDate(), m_oValue.GetCurrencyExchType()) != M4_SUCCESS)
					{
						swSuccess = M4_FALSE;
					}
				}
			}
		}
		else
		{
			var.Type = M4CL_CPP_TYPE_STRING_VAR; 
			var.Data.PointerChar = m_oValue.GetVariantString();
		}
		
		swSuccess = M4_BOOL(m_poNode->pNode->RecordSet.Current.Item[m_oItemName].Value.Set(var));
		
		#ifdef APICHN_TRACE
			m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
		#endif
		//Fin Temporizacion

		if (swSuccess != M4_SUCCESS)
		{
			*m_poInsp->m_poTrace << ClMIT_Msg(&TRACE_ARRAY_EX[APICHN_MSG_1]) << GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() << SEND_MSG;
		}
	}
	
	break;

	default:
		//Error local
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_200 ] ) 
			<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() << SEND_MSG ;
		break ;
	} ;
    
	m_oValue.ClearUpdated() ;

	#ifdef APICHN_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){

		ClMIT_Str Str ;

		m_oValue.GetInputFormat( &Str ) ;

		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
			"ClMIT_ChnItem::Save() Channel %0:s node %1:s item %2:s mode %3:s, value is %4:s") 
			<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() << GetStringLoadMode() 
			<< Str.InChar(0) << CRAZY_MSG << SEND_MSG   ;
	} ;
	#endif
}

void ClMIT_ChnItem::Execute()  
{	
	m4VariantType var;
	m4int16_t R ;

	if ( UpdateCancel() ) return ;

	#ifdef APICHN_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
			"ClMIT_ChnItem::ExecuteMethod() Executing channel %0:s node %1:s item %2:s") 
			<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() << CRAZY_MSG << SEND_MSG   ;
	};
	#endif

	if ( !( GetType()==METHOD || GetType()==CONCEPT ) ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_281 ] ) 
			<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() << SEND_MSG ;
		//Efecto local
	} else {

		//Temporizacion
	    #ifdef APICHN_TRACE
			m_poTaskEnv->Chn_m_oTimerExecuteMethod.Start();
		#endif
		R=m_poNode->pNode->RecordSet.Current.Item[m_oItemName].Call(NULL, 0, m_poTaskEnv->bFromWrapper);
		#ifdef APICHN_TRACE
			m_poTaskEnv->Chn_m_oTimerExecuteMethod.Stop();
		#endif
		//Fin Temporizacion

		//Temporizacion
	    #ifdef APICHN_TRACE
			m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
		#endif
		m_poNode->m_poChn->m_poAccess->GetpExecutor()->Stack.Pop(var) ;
		#ifdef APICHN_TRACE
			m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
		#endif
		//Fin Temporizacion

		if ( R != M4_SUCCESS ) {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_280 ] ) 
				<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() << SEND_MSG ;
			//Efecto local
		} else {
			// si es método guardar el valor en m_oValue
			if (GetType()==METHOD) {
				//------------------------------
				switch ( m_oValue.GetType() ){
				case ( ClMIT_ChnValue::NUMERIC ):
					m_oValue.InitDouble( R == M4_SUCCESS ? var.Data.DoubleData : 0 );
					break;
				case ( ClMIT_ChnValue::DATE ):
					m_oValue.InitDate( R == M4_SUCCESS ? var.Data.DoubleData : 0 );
					break;
				case ( ClMIT_ChnValue::TIME_STAMP ):
					m_oValue.InitTimeStamp( R == M4_SUCCESS ? var.Data.DoubleData : 0 );
					break;
				case (ClMIT_ChnValue::STRING):
					if (var.Type != M4CL_CPP_TYPE_STRING_VAR ) R=M4_ERROR ;
					m_oValue.InitString( R == M4_SUCCESS ? var.Data.PointerChar : "" ); 
					break;
				case (ClMIT_ChnValue::BOOLEAN):
					m_oValue.InitBoolean( R == M4_SUCCESS ? (M4_BOOL(var.Data.DoubleData == DOUBLE_VALID_BOOLEAN_TRUE)) : M4_FALSE );
					break;
				default:
					*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_200 ] ) 
						<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() << SEND_MSG ;
					m_poInsp->m_poTrace->ImmediateError() ;

					break ;
				} ;
				if (R != M4_SUCCESS) m_oValue.SetNULL() ;	
				//------------------------------
			}
		} ;
		
	} ;
}


m4bool_t ClMIT_ChnItem::FindDate(  m4date_t ai_dDate, m4bool_t ai_bFromFirst, 
							 m4bool_t ai_bSwOblig) 
{
	m4bool_t bFound = M4_FALSE;
		
	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif

	if (ai_bFromFirst) m_poNode->GoFirst() ;
	
	while (!bFound && !m_poNode->IsEOF() )
	{
		Load();
		if (ClMIT_Dates::CmpDate(m_oValue.GetDate(),ai_dDate)!=0)
		{
			m_poNode->Next(); 
		}
		else  bFound = M4_TRUE;
	}

	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

	if ( ai_bSwOblig && !bFound ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_240 ] ) 
			<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() 
			<< m_oValue.GetPrecision() << m_oValue.GetScale() << ai_dDate << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
	return bFound;

}

m4bool_t ClMIT_ChnItem::FindTimeStamp(  m4date_t ai_dDate, m4bool_t ai_bFromFirst, 
							 m4bool_t ai_bSwOblig) 
{
	m4bool_t bFound = M4_FALSE;
		
	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif

	if (ai_bFromFirst) m_poNode->GoFirst() ;
	
	while (!bFound && !m_poNode->IsEOF() )
	{
		Load();
		if (ClMIT_Dates::CmpTimeStamp(m_oValue.GetDate(),ai_dDate)!=0)
		{
			m_poNode->Next(); 
		}
		else  bFound = M4_TRUE;
	}

	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

	if ( ai_bSwOblig && !bFound ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_240 ] ) 
			<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() 
			<< m_oValue.GetPrecision() << m_oValue.GetScale() << ai_dDate << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
	return bFound;

}

m4bool_t ClMIT_ChnItem::FindDouble(m4double_t ai_dValue, m4bool_t ai_bFromFirst, 
							   m4bool_t ai_bSwOblig ) 
{
	
	m4bool_t bFound = M4_FALSE;

	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif

	if (ai_bFromFirst) m_poNode->GoFirst() ;
	
	while (!bFound && !m_poNode->IsEOF() )
	{
		Load();
		if (m_oValue.GetDouble() != ai_dValue)
		{
			m_poNode->Next(); 
		}
		else  bFound = M4_TRUE;
	}

	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

	if ( ai_bSwOblig && !bFound ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_250 ] ) 
			<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() 
			<< m_oValue.GetPrecision() << m_oValue.GetScale() << ai_dValue << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
	return bFound;
}
	
m4bool_t ClMIT_ChnItem::FindString(const m4char_t *ai_pcValue, m4bool_t ai_bFromFirst,
							   m4bool_t ai_bSwOblig ) 
{
  	m4bool_t bFound = M4_FALSE;

	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif

	if (ai_bFromFirst) m_poNode->GoFirst() ;
	
	while (!bFound && !m_poNode->IsEOF() )
	{
		Load();
		if ( ClMIT_Str::StrCmpTrim(m_oValue.GetString(), ai_pcValue) != 0 )
		{
			m_poNode->Next();
				
		}
		else  bFound = M4_TRUE;
	}

	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

	if ( ai_bSwOblig && !bFound ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_260 ] ) 
			<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() << ai_pcValue << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	};
	return bFound;
}

m4bool_t ClMIT_ChnItem::FindIString(const m4char_t *ai_pcValue, m4bool_t ai_bFromFirst,
							    m4bool_t ai_bSwOblig ) 
{
  	m4bool_t bFound = M4_FALSE;
	
	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif

	if (ai_bFromFirst) m_poNode->GoFirst() ;
	
	while (!bFound && !m_poNode->IsEOF() )
	{
		Load();
		if ( ClMIT_Str::StrICmpTrim(m_oValue.GetString(), ai_pcValue) != 0 )
		{
			m_poNode->Next();
				
		}
		else  bFound = M4_TRUE;
	}

	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

	if ( ai_bSwOblig && !bFound ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_260 ] ) 
			<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() << ai_pcValue << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	};
	return bFound;
}

m4char_t * ClMIT_ChnItem::GetStringLoadMode( LOAD_MODE ai_LoadMode ) 
{
	switch (ai_LoadMode) 
	{
	case NULL_LOAD:
		return "NULL_LOAD" ;
		break;

	case ITEM_LOAD:
		return "ITEM_LOAD" ;	
		break ;

	case SLICE_LOAD:
		return "SLICE_LOAD" ;	
		break ;
	
	case RANGE_SLICE_LOAD:
		return "RANGE_SLICE_LOAD" ;	
		break ;
	
	case TOTALIZE_SUM:
		return "TOTALIZE_SUM" ;	
		break ;
	case TOTALIZE_AVG:
		return "TOTALIZE_AVG" ;	
		break ;
	case TOTALIZE_MAX:
		return "TOTALIZE_MAX" ;	
		break ;
	case TOTALIZE_MIN:
		return "TOTALIZE_MIN" ;	
		break ;
	case TOTALIZE_FIRST:
		return "TOTALIZE_FIRST" ;	
		break ;
	case TOTALIZE_LAST:
		return "TOTALIZE_LAST" ;	
		break ;
	case TOTALIZE_DESIGN:
		return "TOTALIZE_DESIGN" ;	
		break ;

	case TOTALIZE_SLICE_SUM:
		return "TOTALIZE_SLICE_SUM" ;	
		break ;
	case TOTALIZE_SLICE_AVG:
		return "TOTALIZE_SLICE_AVG" ;	
		break ;
	case TOTALIZE_SLICE_MAX:
		return "TOTALIZE_SLICE_MAX" ;	
		break ;
	case TOTALIZE_SLICE_MIN:
		return "TOTALIZE_SLICE_MIN" ;	
		break ;
	case TOTALIZE_SLICE_FIRST:
		return "TOTALIZE_SLICE_FIRST" ;	
		break ;
	case TOTALIZE_SLICE_LAST:
		return "TOTALIZE_SLICE_LAST" ;	
		break ;
	case TOTALIZE_SLICE_DESIGN:
		return "TOTALIZE_SLICE_DESIGN" ;	
		break ;

	default:
		return "String for LoadMode not found" ;	
		break ;

	} ;
}

//-----------------------------SLICES

m4bool_t ClMIT_ChnItem::IsSliceEOF() 
{
    return M4_BOOL(m_Slice == SLICE_EOF);
}

ClMIT_ChnItem::SliceIndex_t ClMIT_ChnItem::GetSliceIndex() 
{
    return m_Slice ;
}

ClMIT_ChnItem::SliceIndex_t ClMIT_ChnItem::GetNumSlices() 
{
	ClMIT_ChnItem::SliceIndex_t SliceIndex ;

	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	SliceIndex = m_poNode->pNode->RecordSet.Current.Item[m_oItemName].Value.Count(); 
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion
    
	return SliceIndex ;
}

m4date_t   ClMIT_ChnItem::GetFirstStartDate() 
{
	m4date_t Date;

	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	Date = m_poNode->pNode->RecordSet.Current.Item[m_oItemName].Value.GetFirstStartDate();
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

    return Date; 
}

m4date_t   ClMIT_ChnItem::GetLastEndDate() 
{
	m4date_t Date;

	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	Date = m_poNode->pNode->RecordSet.Current.Item[m_oItemName].Value.GetLastEndDate();
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

    return Date;
}

void  ClMIT_ChnItem::GoFirstSlice() 
{
	//Temporizacion
    #ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	m_Slice = m_poNode->pNode->RecordSet.Current.Item[m_oItemName].Value.Begin() ?
		0 : SLICE_EOF ;
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin temporizacion

	#ifdef APICHN_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
			"ClMIT_ChnItem::GoFirstSlice() Channel %0:s node %1:s item %2:s, SliceIndex %3:s "
			"NumSlices %4:s IsEof %5:s" ) 
			<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() 
			<< GetSliceIndex() << GetNumSlices() << IsSliceEOF()
			<< CRAZY_MSG << SEND_MSG   ;
	};
	#endif
}

void  ClMIT_ChnItem::GoLastSlice() 
{
	//Temporizacion
    #ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	m_Slice = m_poNode->pNode->RecordSet.Current.Item[m_oItemName].Value.End() ?
		(m4uint32_t)m_poNode->pNode->RecordSet.Current.Item.Value.Index : (m4uint32_t)SLICE_EOF ;
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

	#ifdef APICHN_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
			"ClMIT_ChnItem::GoLastSlice() Channel %0:s node %1:s item %2:s, SliceIndex %3:s "
			"NumSlices %4:s IsEof %5:s" ) 
			<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() 
			<< GetSliceIndex() << GetNumSlices() << IsSliceEOF()
			<< CRAZY_MSG << SEND_MSG   ;
	};
	#endif
}

void  ClMIT_ChnItem::GoNextSlice() 
{
	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	m_Slice = m_Slice==SLICE_EOF ? 0 : ++m_Slice ;
    if ( ! m_poNode->pNode->RecordSet.Current.Item[m_oItemName].Value.
		MoveTo((ClSliceIndex)m_Slice) ) 
	{
		m_Slice=SLICE_EOF ;
	} ;
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

	#ifdef APICHN_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
			"ClMIT_ChnItem::GoNextSlice() Channel %0:s node %1:s item %2:s, SliceIndex %3:s "
			"NumSlices %4:s IsEof %5:s" ) 
			<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() 
			<< GetSliceIndex() << GetNumSlices() << IsSliceEOF()
			<< CRAZY_MSG << SEND_MSG   ;
	};
	#endif
}

void  ClMIT_ChnItem::GoPrevSlice() 
{
	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	if ( m_Slice==0 ) {
		m_Slice=SLICE_EOF ;
	} else { 
		--m_Slice ;
	    if ( ! m_poNode->pNode->RecordSet.Current.Item[m_oItemName].Value.
			MoveTo((ClSliceIndex)m_Slice) )
				m_Slice=SLICE_EOF ;
	} ;
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

	#ifdef APICHN_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
			"ClMIT_ChnItem::GoPrevSlice() Channel %0:s node %1:s item %2:s, SliceIndex %3:s "
			"NumSlices %4:s IsEof %5:s" ) 
			<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() 
			<< GetSliceIndex() << GetNumSlices() << IsSliceEOF()
			<< CRAZY_MSG << SEND_MSG   ;
	};
	#endif
}

void  ClMIT_ChnItem::MoveToSlice( SliceIndex_t ai_Index ) 
{
	//Temporizacion
    #ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	if (ai_Index==SLICE_EOF)  {
		m_Slice=SLICE_EOF ;
	} else {
		m_Slice=ai_Index ;
	    if ( ! m_poNode->pNode->RecordSet.Current.Item[m_oItemName].Value.
			MoveTo((ClSliceIndex)m_Slice) )
				m_Slice=SLICE_EOF ;
	} ;
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

	#ifdef APICHN_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
			"ClMIT_ChnItem::MoveToSlice() Channel %0:s node %1:s item %2:s, SliceIndex %3:s "
			"NumSlices %4:s IsEof %5:s" ) 
			<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() 
			<< GetSliceIndex() << GetNumSlices() << IsSliceEOF()
			<< CRAZY_MSG << SEND_MSG   ;
	};
	#endif
}

void  ClMIT_ChnItem::MoveToSlice( m4date_t ai_Date ) 
{
	//Temporizacion
    #ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	m_Slice = m_poNode->pNode->RecordSet.Current.Item[m_oItemName].Value.MoveTo(ai_Date) ?
		(m4uint32_t)m_poNode->pNode->RecordSet.Current.Item.Value.Index : (m4uint32_t)SLICE_EOF ;
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion


	#ifdef APICHN_TRACE

	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
	
		ClMIT_Str StrAux ;

		ClMIT_Dates::DateToDateStr(&StrAux,ai_Date) ;
	
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
			"ClMIT_ChnItem::MoveToSlice(Date) Date %0:s Channel %1:s node %2:s item %3:s, "
			"SliceIndex %4:s NumSlices %5:s IsEof %6:s" ) 
			<< StrAux.InChar(0) << GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() 
			<< GetSliceIndex() << GetNumSlices() << IsSliceEOF()
			<< CRAZY_MSG << SEND_MSG   ;
	};
	#endif
}

void  ClMIT_ChnItem::MoveToSlice( m4date_t ai_StartDate, m4date_t ai_EndDate ) 
{
	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	m_Slice = m_poNode->pNode->RecordSet.Current.Item[m_oItemName].Value.
		MoveTo(ai_StartDate, ai_EndDate) ?  
		(m4uint32_t)m_poNode->pNode->RecordSet.Current.Item.Value.Index : (m4uint32_t)SLICE_EOF ;
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion


	#ifdef APICHN_TRACE

	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
	
		ClMIT_Str StrAux1, StrAux2 ;

		ClMIT_Dates::DateToDateStr(&StrAux1,ai_StartDate) ;
		ClMIT_Dates::DateToDateStr(&StrAux2,ai_EndDate) ;
	
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
			"ClMIT_ChnItem::MoveToSlice(Date1,Date2) Date1 %0:s Date2 %1:s Channel %2:s node %3:s "
			"item %4:s, SliceIndex %5:s NumSlices %6:s IsEof %7:s" ) 
			<< StrAux1.InChar(0) << StrAux2.InChar(0) << GetChannelIdDesc() 
			<< GetNodeIdDesc() << GetIdDesc() << GetSliceIndex() << GetNumSlices() 
			<< IsSliceEOF()
			<< CRAZY_MSG << SEND_MSG   ;
	};

	#endif
}

m4date_t   ClMIT_ChnItem::GetStartDate(SliceIndex_t ai_Index) 
{
	m4date_t Date;

	MoveToSlice(ai_Index) ;

	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	Date = m_poNode->pNode->RecordSet.Current.Item.Value[(ClSliceIndex)m_Slice].GetStartDate();
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

	return m_Slice == SLICE_EOF ? 0 : Date ;
}

m4date_t   ClMIT_ChnItem::GetEndDate(SliceIndex_t ai_Index) 
{
	m4date_t Date;

	MoveToSlice(ai_Index) ;

	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	Date = m_poNode->pNode->RecordSet.Current.Item.Value[(ClSliceIndex)m_Slice].GetEndDate();
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

    return m_Slice == SLICE_EOF ? 0 : Date ;
}


m4pcchar_t ClMIT_ChnItem::GetTiId()
{
	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif

	ClItemDefIt_Random idir (m_poNode->pNode->GetpExtInfo ());
	ClItemDef ItemDef = idir [m_oItemName];

	return ItemDef.TiId();
	
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion
}

//====================================================================== ClMIT_ChnNode
ClMIT_ChnNode::ClMIT_ChnNode()
{
	m_poInsp = GET_INSP() ;
	m_poTaskEnv=GET_APICHN_TASK_ENV() ;
	pNode = NULL;
}

void ClMIT_ChnNode::End() 
{
	pNode = NULL;
	m_oIdDesc.Init("","") ;
}

m4bool_t ClMIT_ChnNode::Init( ClMIT_Chn * ai_poChn, const m4char_t* ai_pcNameNode, ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect  )
{
	m4bool_t Return=M4_TRUE ;

	ClMIT_ChnNode::End();

	m_poChn = ai_poChn ;
	
	SetIgnoreDeleted(M4_TRUE) ;

	// Conseguimos puntero al nodo
	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
    pNode = &(m_poChn->m_poAccess->Node[ai_pcNameNode]);
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

	if (!pNode) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_270 ] ) 
			<< ai_pcNameNode << m_poChn->GetIdDesc() << SEND_MSG ;
		Return=M4_FALSE ;
	} else {
		m_oIdDesc.Init( pNode->GetpNodeDef()->Id(), pNode->GetpNodeDef()->Name() ) ;

		//Obtiene el Handle para el OrderBy
		m_oHandleOrderBy = m_poChn->m_poAccess->GetpCMCR()->GetNodeRAMOrderByItem(pNode->GetpNodeDef()->GetHandle());
	} ;

	m_poInsp->m_poTrace->TestEffect( Return, ai_ErrorEffect ) ; 
	return Return ;		
}


m4bool_t ClMIT_ChnNode::GetOrderByItem(ClMIT_Str *ao_poStrOrderBy)
{
	m4bool_t SwOk = M4_TRUE;
	m4char_t *PAux ;
	StMIT_APIChn_TaskEnv *ai_poTaskEnv = TM_GET_APICHN_TASK_ENV(GET_TM()) ;

	//Temporizacion
	#ifdef RPT_ENGINE_DEBUG
		ai_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif

	PAux = (m4char_t *) pNode->RecordSet.Current.Item[m_oHandleOrderBy].Value.Get() ;

	//Si el handle es distinto de cero es que hay Item
	// Entonces leemos su valor (cadena)
	if(m4uint32_t(m_oHandleOrderBy)!=0){
		ao_poStrOrderBy->StrCpy( PAux ? PAux : "" );
	} else {
		SwOk = M4_FALSE;
	};

	//Temporizacion
	#ifdef RPT_ENGINE_DEBUG
		ai_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif

	return SwOk;
}

m4return_t ClMIT_ChnNode::SetOrderByItem(m4char_t* ai_pcStrOrderBy)
{
	m4VariantType var;
	m4return_t swSuccess ;	
	StMIT_APIChn_TaskEnv *ai_poTaskEnv = TM_GET_APICHN_TASK_ENV(GET_TM()) ;

	var.Type = M4CL_CPP_TYPE_STRING_VAR; 
	var.Data.PointerChar=ai_pcStrOrderBy;

	//Temporizacion
	#ifdef RPT_ENGINE_DEBUG
		ai_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	if(m4uint32_t(m_oHandleOrderBy)!=0){
		swSuccess = pNode->RecordSet.Current.Item[m_oHandleOrderBy].Value.Set(var) ;
	} else {
		swSuccess = M4_ERROR;
	}
	#ifdef RPT_ENGINE_DEBUG
		ai_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif

	*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID, 
		"ClMIT_ChnNode::SetOrderByItem() in node %0:s of %1:s, sentence is %2:s") 
		<< GetIdDesc() << m_poChn->GetIdDesc() << ai_pcStrOrderBy
		<< SEND_MSG   ;

	//Fin Temporizacion

	/*if ( swSuccess != M4_SUCCESS ) {
		//Error de efecto local
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_300 ] ) 
			<< GetChannelIdDesc() << GetNodeIdDesc() << GetIdDesc() << SEND_MSG ;
	} ;*/

	return swSuccess;
}


void  ClMIT_ChnNode::MoveTo( Record_t ai_Record )
{
	if ( UpdateCancel() )	{
		pNode->RecordSet.Current.MoveToEOF ();
		return ;
	} ;

	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	if (ai_Record==RECORD_EOF) {
		pNode->RecordSet.Current.MoveToEOF ();
	} else {
		pNode->RecordSet.Current.MoveTo(ai_Record);
	} ;
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

	#ifdef APICHN_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){

		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
			"ClMIT_ChnNode::MoveTo() Record %0:s channel %1:s node %2:s IndexRecord %3:s IsEof %4:s") 
			<< ai_Record  << m_poChn->GetIdDesc() << GetIdDesc() << GetNumRecord() << IsEOF()	
			<< DETAILED_MSG << SEND_MSG   ;
	} ;
	#endif
}

void ClMIT_ChnNode::ReLoad() 
{
	m4bool_t SwOk=M4_TRUE ;

	if ( UpdateCancel() )	{
		pNode->RecordSet.Current.MoveToEOF ();
		return ;
	} ;

	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerLoad.Start();
	#endif
	
	pNode->RecordSet.Destroy() ;
	pNode->RecordSet.Load_Block() ;


	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerLoad.Stop();
	#endif
	//Fin Temporizacion

	if ( !SwOk ) {
		/*##Num error*/
		*m_poInsp->m_poTrace << ClMIT_Msg( ClMIT_Msg::MSG_ERROR, GROUP_ID,
			"Channel CT %0:s Node %1:s, ReLoad failed", ClMIT_Msg::MIXED_OUT) 
			<< m_poChn->GetIdDesc() << GetIdDesc() << SEND_MSG ;
		//Error local
	} ;

	#ifdef APICHN_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
			"ClMIT_ChnNode::ReLoad() Channel %0:s node %1:s Count %2:s") 
			<< m_poChn->GetIdDesc() << GetIdDesc() << GetCount() 	
			<< DETAILED_MSG << SEND_MSG   ;
	} ;
	#endif
}


ClMIT_ChnNode::Record_t ClMIT_ChnNode::GetRealCount() 
{
	//Cuidado; los metodos aqui usados no deben llamar en mensajes trace a GetCount, 
	//	o se cae en un bucle infinito.

	ClMIT_ChnNode::Record_t Count, Pos ;

	if ( UpdateCancel() ) return 0 ;
	
	if ( !IsIgnoreDeleted() ) {
		//Temporizacion
		#ifdef APICHN_TRACE
			m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
		#endif
		//------>Parche MVC
		Count = pNode->RecordSet.Register.Count();	// Dispara autoload
		Count = pNode->RecordSet.Count();	// Da count
		if (Count==-1) Count=0 ;	//Parche para bug MVC
		//------>
		#ifdef APICHN_TRACE
			m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
		#endif
		//Fin Temporizacion
	} else {
		//Temporizacion
		#ifdef APICHN_TRACE
			m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
		#endif
		Count=0 ;
		Pos = GetNumRecord() ;
	    pNode->RecordSet.Current.Begin ();
		while ( !IsEOF() ) {
			if ( ! IsRecordDeleted() ) ++Count;
			pNode->RecordSet.Current.Next ();
		} ;
		#ifdef APICHN_TRACE
			m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
		#endif
		//Fin Temporizacion
		MoveTo(Pos) ;
		#ifdef APICHN_TRACE
	
		if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
			*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
				"ClMIT_ChnNode::GetRealCount() invoked with IsIgnoreDeleted!!!! All records in "
				"node will be tested. Channel %0:s node %1:s IndexRecord %2:s CountRecords %3:s "
				"IsEof %4:s") 
				<< m_poChn->GetIdDesc() << GetIdDesc() << GetNumRecord() << Count<< IsEOF()	
				<< DETAILED_MSG << SEND_MSG   ;
		};
		#endif
	} ;

	return Count ;
}


ClMIT_ChnNode::Record_t ClMIT_ChnNode::GetCount() 
{
	ClMIT_ChnNode::Record_t Record ;

	if ( UpdateCancel() ) return 0 ;

	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif

	//------>Parche MVC
	Record = pNode->RecordSet.Register.Count();	// Dispara autoload
	Record = pNode->RecordSet.Count();	// Da count
	if (Record==-1) Record=0 ;	//Parche para bug MVC
	//------>

	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

	return Record ;
}


ClMIT_ChnNode::Record_t ClMIT_ChnNode::GetNumRecord() 
{
	ClMIT_ChnNode::Record_t Record ;

	if ( UpdateCancel() ) return RECORD_EOF ;

	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	Record = pNode->RecordSet.Current.Index ;
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

    return IsEOF() ? RECORD_EOF : Record ;
}

m4bool_t ClMIT_ChnNode::IsRecordDeleted() 
{
	m4bool_t IsDel;

	if ( UpdateCancel() )	return M4_TRUE ;

	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	IsDel = pNode->RecordSet.Current.Operation.IsDeleted() ;
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

	return IsDel ;
}



m4bool_t ClMIT_ChnNode::IsRecordModified() 
{
	m4bool_t IsModif;

	if ( UpdateCancel() )	return M4_TRUE ;

	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	IsModif = pNode->RecordSet.Current.Operation.IsModified();
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

	return IsModif;
}


m4pcchar_t ClMIT_ChnNode::GetTiId()
{
	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	return (pNode->GetpNodeDef()->TiId());
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion
}

m4pcchar_t ClMIT_ChnNode::GetM4ObjId()
{
	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	return (pNode->GetpNodeDef()->M4ObjId());
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion
}

m4bool_t ClMIT_ChnNode::Next()	
{	
	if ( UpdateCancel() )	{
		pNode->RecordSet.Current.MoveToEOF ();
		return M4_TRUE;
	} ;
	
	//Temporizacion
    #ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	pNode->RecordSet.Current.Next ();
	if ( IsIgnoreDeleted() ) {
		while ( !IsEOF() && IsRecordDeleted() ) 
			pNode->RecordSet.Current.Next ();
	} ;
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

	#ifdef APICHN_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
			"ClMIT_ChnNode::Next() Channel %0:s node %1:s IndexRecord %2:s CountRecords %3:s IsEof %4:s" )  
			<< m_poChn->GetIdDesc() << GetIdDesc() << GetNumRecord() << GetCount() << IsEOF()
			<< DETAILED_MSG << SEND_MSG   ;
	};
	#endif

	return M4_TRUE;
}

void ClMIT_ChnNode::Previous() 
{
	if ( UpdateCancel() )	{
		pNode->RecordSet.Current.MoveToEOF ();
		return ;
	} ;

	//Temporizacion
    #ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	pNode->RecordSet.Current.Prev ();
	if ( IsIgnoreDeleted() ) {
		while ( !IsEOF() && IsRecordDeleted() ) 
			pNode->RecordSet.Current.Prev ();
	} ;
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

	#ifdef APICHN_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
			"ClMIT_ChnNode::Previous() Channel %0:s node %1:s IndexRecord %2:s CountRecords %3:s IsEof %4:s" )  
			<< m_poChn->GetIdDesc() << GetIdDesc() << GetNumRecord() << GetCount() << IsEOF()
			<< DETAILED_MSG << SEND_MSG   ;
	};
	#endif
}

void ClMIT_ChnNode::GoFirst( ) 
{
	if ( UpdateCancel() )	{
		pNode->RecordSet.Current.MoveToEOF ();
		return ;
	} ;

	//Temporizacion
    #ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	pNode->RecordSet.Current.Begin();
	if ( IsIgnoreDeleted() ) {
		while ( !IsEOF() && IsRecordDeleted() ) 
			pNode->RecordSet.Current.Next();
	} ;
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

	#ifdef APICHN_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
			"ClMIT_ChnNode::GoFirst() Channel %0:s node %1:s IndexRecord %2:s CountRecords %3:s IsEof %4:s" )  
			<< m_poChn->GetIdDesc() << GetIdDesc() << GetNumRecord() << GetCount() << IsEOF()
			<< DETAILED_MSG << SEND_MSG   ;
	};
	#endif
}

void ClMIT_ChnNode::GoLast( ) 
{
	if ( UpdateCancel() )	{
		pNode->RecordSet.Current.MoveToEOF ();
		return ;
	} ;

	//Temporizacion
    #ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif

	if ( !IsPartialLoad() ) {
		pNode->RecordSet.Current.End ();
	} else {

		Record_t Reg ;

		//PEDAZO DE CHAPUZA!!!
		if ( IsEOF() ) 	pNode->RecordSet.Current.Begin() ;	//Si no EOF(), parto de donde este
		do  {
			Reg = pNode->RecordSet.Current.Index ;
			pNode->RecordSet.Current.Next();
		} while ( !IsEOF() ) ;
		if ( !IsEOF() )  pNode->RecordSet.Current.MoveTo(Reg);
	} ;


	if ( IsIgnoreDeleted() ) {
		while ( !IsEOF() && IsRecordDeleted() ) 
			pNode->RecordSet.Current.Prev();
	} ;
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

	#ifdef APICHN_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
			"ClMIT_ChnNode::GoLast() Channel %0:s node %1:s IndexRecord %2:s CountRecords %3:s IsEof %4:s" )  
			<< m_poChn->GetIdDesc() << GetIdDesc() << GetNumRecord() << GetCount() << IsEOF()
			<< DETAILED_MSG << SEND_MSG   ;
	};
	#endif
}

void ClMIT_ChnNode::Insert() 
{
	m4return_t Return;

	if ( UpdateCancel() )	{
		pNode->RecordSet.Current.MoveToEOF ();
		return ;
	} ;

	//Temporizacion
    #ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	Return = pNode->RecordSet.Current.Insert();
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

	if(Return!=M4_SUCCESS){
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_370 ] ) 
						<< m_poChn->GetIdDesc() << GetIdDesc() << SEND_MSG ;
		return;
	};

	#ifdef APICHN_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
			"ClMIT_ChnNode::Insert() Channel %0:s node %1:s IndexRecord %2:s CountRecords %3:s IsEof %4:s" )  
			<< m_poChn->GetIdDesc() << GetIdDesc() << GetNumRecord() << GetCount() << IsEOF()
			<< DETAILED_MSG << SEND_MSG   ;
	};
	#endif
}

void ClMIT_ChnNode::Delete() 
{
	m4return_t Return;

	if ( UpdateCancel() )	{
		pNode->RecordSet.Current.MoveToEOF ();
		return ;
	} ;

	//Temporizacion
    #ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	Return = pNode->RecordSet.Current.Delete();
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

	if(Return!=M4_SUCCESS){
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_380 ] ) 
						<< m_poChn->GetIdDesc() << GetIdDesc() << SEND_MSG ;
		return;
	};

	if(IsIgnoreDeleted()){
		Next();
	};

	#ifdef APICHN_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
			"ClMIT_ChnNode::Delete() Channel %0:s node %1:s IndexRecord %2:s CountRecords %3:s IsEof %4:s" )  
			<< m_poChn->GetIdDesc() << GetIdDesc() << GetNumRecord() << GetCount() << IsEOF()
			<< DETAILED_MSG << SEND_MSG   ;
	};
	#endif
}

m4bool_t ClMIT_ChnNode::DeleteAll() 
{
	m4bool_t SwOk=M4_TRUE;
	m4return_t Return=M4_SUCCESS;
	Record_t Count;

	if ( UpdateCancel() )	{
		pNode->RecordSet.Current.MoveToEOF ();
		goto exit ;
	} ;

	//Temporizacion
    #ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif

	Count = GetRealCount();
	if(Count!=0) {
		Return = pNode->RecordSet.Current.DeleteAll();
		pNode->RecordSet.Current.MoveToEOF ();
	};
	
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

	if(Return!=M4_SUCCESS){
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ APICHN_MSG_380 ] ) 
						<< m_poChn->GetIdDesc() << GetIdDesc() << SEND_MSG ;
		SwOk=M4_FALSE;
		goto exit;
	};

	#ifdef APICHN_TRACE
	if(!m_poInsp->m_poTrace->IsTraceCodeRipped()){
		*m_poInsp->m_poTrace << ClMIT_Msg( GROUP_ID,
			"ClMIT_ChnNode::DeleteAll() Channel %0:s node %1:s IndexRecord %2:s CountRecords %3:s IsEof %4:s" )  
			<< m_poChn->GetIdDesc() << GetIdDesc() << GetNumRecord() << GetCount() << IsEOF()
			<< DETAILED_MSG << SEND_MSG   ;
	};
	#endif

exit:

	return SwOk;
}

m4bool_t ClMIT_ChnNode::IsEOF() 
{
	m4bool_t R;

	if ( UpdateCancel() ) return M4_TRUE ;

	//Temporizacion
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Start();
	#endif
	//R = pNode->RecordSet.Current.Index.IsEOF ();
	R = pNode->RecordSet.Current.Index==M4DM_EOF_INDEX ? M4_TRUE : M4_FALSE;
	#ifdef APICHN_TRACE
		m_poTaskEnv->Chn_m_oTimerChannelAccess.Stop();
	#endif
	//Fin Temporizacion

    return R ;
}

m4bool_t ClMIT_ChnNode::UpdateCancel() 
{ 
	return m_poChn->UpdateCancel() ; 
} 


m4uint32_t ClMIT_ChnNode::GetPartialLoadRows() 
{
	return pNode->GetpNodeDef()->NumRows() ;
}


m4uint32_t ClMIT_ChnNode::GetPartialLoadKeepRows() 
{
	return pNode->GetpNodeDef()->NumKeepRows() ;
}
