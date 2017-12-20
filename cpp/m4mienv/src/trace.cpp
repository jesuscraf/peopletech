//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             trace.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:15/09/97
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

#include <stdarg.h>

#include "m4string.hpp"
#include "m4clock.hpp"

#include "mit.h"
#include "m4log.hpp"
#include "basiclog.hpp"

#include "mitdefs.cpp"

//#include "trcgroup.h"
#include "trace.hmg"
#include "trace.h"

#include "clstr.h"
#include "sysinfo.h"

#define LF_SEQUENCE "\r\n"


//================================================================ClMIT_TraceCheck

ClMIT_TraceCheck::ClMIT_TraceCheck(const ClMIT_TraceCheck &ai_Ope) 
{
	m_uiErrorCount = ai_Ope.m_uiErrorCount ;
	m_uiWarningCount = ai_Ope.m_uiWarningCount ;
	m_uiEC_ErrorCount = ai_Ope.m_uiEC_ErrorCount ;
	m_uiEC_WarningCount = ai_Ope.m_uiEC_WarningCount ;
	m_uiDummyObjects = ai_Ope.m_uiDummyObjects ;
}

ClMIT_TraceCheck & ClMIT_TraceCheck::operator = (const ClMIT_TraceCheck & ai_Ope) 
{
	m_uiErrorCount = ai_Ope.m_uiErrorCount ;
	m_uiWarningCount = ai_Ope.m_uiWarningCount ;
	m_uiEC_ErrorCount = ai_Ope.m_uiEC_ErrorCount ;
	m_uiEC_WarningCount = ai_Ope.m_uiEC_WarningCount ;
	m_uiDummyObjects = ai_Ope.m_uiDummyObjects ;
	return *this ;
}

ClMIT_TraceCheck & ClMIT_TraceCheck::operator - (const ClMIT_TraceCheck & ai_Ope) 
{
	m_uiErrorCount -= ai_Ope.m_uiErrorCount ;
	m_uiWarningCount -= ai_Ope.m_uiWarningCount ;
	m_uiEC_ErrorCount -= ai_Ope.m_uiEC_ErrorCount ;
	m_uiEC_WarningCount -= ai_Ope.m_uiEC_WarningCount ;
	m_uiDummyObjects -= ai_Ope.m_uiDummyObjects ;
	return *this ;
}

ClMIT_TraceCheck & ClMIT_TraceCheck::operator + (const ClMIT_TraceCheck & ai_Ope) 
{
	m_uiErrorCount += ai_Ope.m_uiErrorCount ;
	m_uiWarningCount += ai_Ope.m_uiWarningCount ;
	m_uiDummyObjects += ai_Ope.m_uiDummyObjects ;
	m_uiEC_ErrorCount += ai_Ope.m_uiEC_ErrorCount ;
	m_uiEC_WarningCount += ai_Ope.m_uiEC_WarningCount ;
	return *this ;
}

//================================================================ClMIT_IdNumDesc

ClMIT_IdNumDesc::ClMIT_IdNumDesc( m4uint32_t ai_uiId, const m4char_t *ai_pcDesc ) 
{
	sprintf(m_IdNum, "%lu", ai_uiId) ;
	if ( strlen(m_IdNum) > ID_STR_SIZE-1 ) {
		StMIT_Inspector *Insp=GET_INSP() ;
		Insp->m_poTrace->Unindent(10000) ;	//Por si indentacion provoca error, no caer en bucle infinito		
		*Insp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ TRACE_MSG_10 ] ) << SEND_MSG ;
		Insp->m_poTrace->ImmediateError() ;
	} ;
	ClMIT_IdDesc::Init( m_IdNum, ai_pcDesc) ;
}

void ClMIT_IdNumDesc::Init( m4uint32_t ai_uiId, const  m4char_t *ai_pcDesc ) 
{
	sprintf(m_IdNum, "%lu", ai_uiId) ;
	if ( strlen(m_IdNum) > ID_STR_SIZE-1 ) {
		StMIT_Inspector *Insp=GET_INSP() ;
		Insp->m_poTrace->Unindent(10000) ;	//Por si indentacion provoca error, no caer en bucle infinito		
		*Insp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ TRACE_MSG_10 ] ) << SEND_MSG ;
		Insp->m_poTrace->ImmediateError() ;
	} ;
	ClMIT_IdDesc::Init( m_IdNum, ai_pcDesc) ;
}

void ClMIT_IdNumDesc::Init( ClMIT_IdNumDesc * ai_poFrom )  
{
	strcpy(m_IdNum, ai_poFrom->m_IdNum) ;
	ClMIT_IdDesc::Init( m_IdNum, ai_poFrom->GetDesc()) ;
}

m4uint32_t ClMIT_IdNumDesc::GetIdNum() const 
{
	return atol(m_IdNum) ;
} 

//================================================================ClMIT_Msg

void ClMIT_Msg::Init( const ClMIT_Msg *ai_From ) 
{
	m_Group    = ai_From->m_Group ;
	m_Code     = ai_From->m_Code ;
	m_Type     = ai_From->m_Type ;
	m_Out      = ai_From->m_Out ;	
	m_MsgLevel = ai_From->m_MsgLevel ;
	m_pcMsg = ai_From->m_pcMsg ;	
	m_Flag.Reset( ai_From->m_Flag.Value() ) ;
	if ( m_Flag.TrueOR(ENTRY_FORMAT|EXIT_FORMAT) ) m_Flag.On(NO_WRAP) ;
}


ClMIT_Msg::ClMIT_Msg(
	MSG_TYPE ai_Type, MsgGroup_t ai_Group, const m4char_t  *ai_pcMsg,
	MSG_OUT ai_Out, MSG_LEVEL ai_MsgLevel, ClMIT_Flag::Value_t ai_Flags)
{
	m_Group = ai_Group ;
	m_Code  = TRACE_MSG_20+MIT_TGB_TRACE ;
	m_Type  = ai_Type ;
	m_Out   = ai_Out ;	
	m_MsgLevel = ai_MsgLevel ;
	m_pcMsg = ai_pcMsg ;	
	m_Flag.Reset( ai_Flags ) ;
	if ( m_Flag.TrueOR(ENTRY_FORMAT|EXIT_FORMAT) ) m_Flag.On(NO_WRAP) ;
}


ClMIT_Msg::ClMIT_Msg(
	MSG_TYPE ai_Type, MsgGroup_t ai_Group, MsgCode_t ai_Code, const m4char_t  *ai_pcMsg,
	MSG_OUT ai_Out, MSG_LEVEL ai_MsgLevel, ClMIT_Flag::Value_t ai_Flags)
{
	m_Group = ai_Group ;
	m_Code  = ai_Code ;
	m_Type  = ai_Type ;
	m_Out   = ai_Out ;	
	m_MsgLevel = ai_MsgLevel ;
	m_pcMsg = ai_pcMsg ;	
	m_Flag.Reset( ai_Flags ) ;
	if ( m_Flag.TrueOR(ENTRY_FORMAT|EXIT_FORMAT) ) m_Flag.On(NO_WRAP) ;
}

ClMIT_Msg::ClMIT_Msg(
	MsgGroup_t ai_Group, const m4char_t  *ai_pcMsg, 
	ClMIT_Flag::Value_t ai_Flags) 
{
	m_Group = ai_Group ;
	m_Code  =  TRACE_MSG_20+MIT_TGB_TRACE ;
	m_Type  = MSG_DEBUG ;
	m_Out   = BE_OUT ;	
	m_MsgLevel = GENERAL ;
	m_pcMsg = ai_pcMsg ;	
	m_Flag.Reset( ai_Flags ) ;
	if ( m_Flag.TrueOR(ENTRY_FORMAT|EXIT_FORMAT) ) m_Flag.On(NO_WRAP) ;
}
	
ClMIT_Msg::ClMIT_Msg( const StMsgInfo *ai_Info ) 
{

	m_Group = ai_Info->m_Group ;
	m_Code  = ai_Info->m_Code ;
	m_Type  = ai_Info->m_Type ;
	m_Out   = ai_Info->m_Out ;	
	m_MsgLevel = GENERAL ;
	m_pcMsg = ai_Info->m_pcMsg ;	
	m_Flag.Reset( ai_Info->m_Flags ) ;
	if ( m_Flag.TrueOR(ENTRY_FORMAT|EXIT_FORMAT) ) m_Flag.On(NO_WRAP) ;
}


	
//================================================================ClMIT_Trace


ClMIT_Trace::ClMIT_Trace()
{
	m_poInsp=GET_INSP();
	m_oObjFlags.Off(INIT_TRACE) ;
}

void ClMIT_Trace::End(ClMIT_TraceCheck *ao_poCheck, m4char_t *ao_pcTraceFile, size_t ao_TraceSize)
{
	m4bool_t SwMove, SwAux ;

	m_oObjFlags.On(LOCK_EC_PROPAGATE) ;

	if ( IsInicialized() ) {
	
		CheckPoint("ClMIT_Trace::End()") ;
	
		if ( m_oObjFlags.True(LOCAL_TRACE) ) {
	
			//---------------------------->Mueve y cierra el fichero

			switch (m_MoveMode) {
				case NEVER_MOVE:
					SwMove=M4_FALSE ;
				break;
				case MOVE_ALWAYS:
					SwMove=M4_TRUE ;
				break;
				case MOVE_ON_SUCCESS:
					SwMove=IsAllOK() ;
				break;
				case MOVE_ON_PROBLEM:
					SwMove=M4_BOOL( !IsAllOK() );
				break;
				case MOVE_ON_ERROR:
					SwMove=M4_BOOL( GetErrorCount() != 0 );
				break;
			} ;

			if (SwMove) {
				#ifdef MIT_TRACE
				if(!this->IsTraceCodeRipped()){
					*this << ClMIT_Msg(GROUP_ID, "Performing file move from %0:s to %1:s") <<
						m_oFile.GetName() << m_oMoveFile << SEND_MSG ;
				};
				#endif
				if ( ao_pcTraceFile ) strcpy(ao_pcTraceFile,m_oMoveFile) ;
			} else {
				if ( ao_pcTraceFile ) strcpy(ao_pcTraceFile,m_oFile.GetName()) ;
			} ;
			if ( ao_pcTraceFile ) ClMIT_Str::OverStringValid(ao_pcTraceFile,ao_TraceSize) ;
		
			//A partir del close no pueden sacarse mensajes a fichero ni usar objetos ClStr !!!
			strcpy(m_poInsp->m_poEnv->ClMIT_Trace_pcTraceMoveAux,m_oFile.GetName()) ;
			ClMIT_Str::OverStringValid(m_poInsp->m_poEnv->ClMIT_Trace_pcTraceMoveAux,StMIT_InspEnv::ClMIT_Trace_SIZE_MOVE_AUX) ;

			//Cierra el fichero
			m_oFile.Close() ;	

			if (SwMove) {
				if ( ClMIT_File::Exist(m_oMoveFile) ) {
					SwAux=ClMIT_File::Remove(m_oMoveFile, M4_FALSE) ;
				} else SwAux=M4_TRUE;
				if (SwAux) SwAux=ClMIT_File::Move(m_poInsp->m_poEnv->ClMIT_Trace_pcTraceMoveAux,m_oMoveFile,M4_FALSE);
				if ( !SwAux ) {
					*this << ClMIT_Msg( &TRACE_ARRAY[ TRACE_MSG_70 ] ) << 
						m_poInsp->m_poEnv->ClMIT_Trace_pcTraceMoveAux << m_oMoveFile << SEND_MSG ;
					//Error local. IMPORTANTE: FE_OUT porque ya no hay fichero!!!!

					//No puede moverme: el nombre de fichero es el original
					if (ao_pcTraceFile) strcpy(ao_pcTraceFile,m_poInsp->m_poEnv->ClMIT_Trace_pcTraceMoveAux) ;
				} ;
			} ;
			//---------------------------->Acabo de mover y cerrar el fichero

		} ;


		//---------------------------->Cierra la sesion con la M4Log 

		if ( m_oObjFlags.True(M4LOG_TRACE) ) {
			DESTROYLOGMANAGER();
		} ;

		if ( ao_poCheck ) *ao_poCheck = m_oOkCount ;

		m_oObjFlags.Off(INIT_TRACE) ;
	} ;
	m_iLocationCount=0;
}


m4bool_t ClMIT_Trace::Init(
	const m4char_t *ai_pcLayerId, const m4char_t *ai_pcComponentId,
	const m4char_t *ai_pcLocalTraceFile, const m4char_t *ai_pcM4INIFile, 
	const m4char_t *ai_GroupNames[ClMIT_Trace::GROUP_NAME_SIZE], m4int16_t ai_iGroupCount) 
{
	ClMIT_Msg::MsgGroup_t B ;
	m4bool_t InitLogError=M4_FALSE, InitLocalError=M4_FALSE, SwOk=M4_TRUE ;
	size_t l ;

	ClMIT_Trace::End() ;

	m_oObjFlags.On(LOCK_EC_PROPAGATE) ;

	//---------Flags y grupos

	ClMIT_Str TraceCodeRipped ;
	m4int16_t iTraceRipped;

	m_poInsp->m_poSysInfo->GetValue( MIT_RK_SYSTEM_DEBUG_ENABLE, &TraceCodeRipped) ;
	iTraceRipped = atoi(TraceCodeRipped.InChar(0));
	if(!iTraceRipped){
		m_oObjFlags.On(TRACE_CODE_RIPPED) ;
		ai_pcLocalTraceFile=NULL;
	} else if(iTraceRipped==1) {
		m_oObjFlags.Off(TRACE_CODE_RIPPED) ;
	};

	m_oObjFlags.On(INIT_TRACE|SILENT_CANCEL) ;
	m_oObjFlags.Off(CANCEL_NOTIFIED) ;
	m_oObjFlags.Switch(LOCAL_TRACE, M4_BOOL(ai_pcLocalTraceFile!=NULL)) ;
	m_oObjFlags.Switch(M4LOG_TRACE, M4_BOOL(ai_pcM4INIFile!=NULL)) ;

	m_uiErrorId=m_uiWarningId=0 ;	

	m_iParamCount=m_iLocParamCount=0 ;
	m_MoveMode=NEVER_MOVE ;

	m_oTypeMsgFlags[ ClMIT_Msg::MSG_ERROR   ][ClMIT_Msg::FE_OUT].Reset( ALWAYS_LEVEL|PUSH_LOCATION) ;
	m_oTypeMsgFlags[ ClMIT_Msg::MSG_ERROR   ][ClMIT_Msg::BE_OUT].Reset( ALWAYS_LEVEL|ALWAYS_OUT|PUSH_LOCATION|SHOW_TYPE|ADD_LF ) ;

	m_oTypeMsgFlags[ ClMIT_Msg::MSG_WARNING ][ClMIT_Msg::FE_OUT].Reset( ALWAYS_LEVEL|PUSH_LOCATION) ;
	m_oTypeMsgFlags[ ClMIT_Msg::MSG_WARNING ][ClMIT_Msg::BE_OUT].Reset( ALWAYS_LEVEL|ALWAYS_OUT|PUSH_LOCATION|SHOW_TYPE|ADD_LF ) ;

	m_oTypeMsgFlags[ ClMIT_Msg::MSG_DEBUG   ][ClMIT_Msg::FE_OUT].Reset() ;
	//m_oTypeMsgFlags[ ClMIT_Msg::MSG_DEBUG   ][ClMIT_Msg::BE_OUT].Reset(ADD_LF|PUSH_LOCATION) ;
	m_oTypeMsgFlags[ ClMIT_Msg::MSG_DEBUG   ][ClMIT_Msg::BE_OUT].Reset(ADD_LF) ;

	m_oOutputFlags[ClMIT_Msg::FE_OUT].Reset(ID_OUT) ;
	m_oOutputFlags[ClMIT_Msg::BE_OUT].Reset(ID_OUT|INDENT) ;

	m_iIndent=0 ;
	SetBlanksPerIndent( 2 ) ; 
	//No se permite wrap, interfiere con el TraceReader
	m_iWrap=0 ;
	SetGroupDetail( 0, GROUP_COUNT-1, ClMIT_Msg::DETAILED ) ;

	//Inicializacion dummy de grupos

	m_oOkCount.Reset() ;
	for (B=0; B<GROUP_COUNT; ++B) {
		strcpy(m_stGroup[B].GroupName, "???") ;
		m_stGroup[B].uiErrorCount= m_stGroup[B].uiWarningCount= 0 ;
	} ;

	//Inicializacion de shared

	for (B=0; B<MIT_TGI_NOT_SHARED_FIRST; ++B) {
		strcpy(m_stGroup[B].GroupName, m_g_pcTraceGroupsId[B] ) ;
	} ;

	//Inicializacion de particulares

	for (B=0; B<ai_iGroupCount; ++B) {
		l = strlen(ai_GroupNames[B]) ;
		l = M4_MIN(l,GROUP_NAME_SIZE-1) ;
		strncpy(m_stGroup[MIT_TGI_NOT_SHARED_FIRST+B].GroupName, ai_GroupNames[B], l ) ;
		m_stGroup[MIT_TGI_NOT_SHARED_FIRST+B].GroupName[l]=M4_END_STR ;
	} ;

	//Inicializo m4log. Si hay error, se intentara comunicar luego via BE_OUT

	if ( m_oObjFlags.True(M4LOG_TRACE) ) {
		if(INITIALIZELOGMANAGER((m4char_t *)ai_pcM4INIFile)==M4_ERROR){
			InitLogError=M4_TRUE ;
			SwOk=M4_FALSE ;
			m_oObjFlags.Off(M4LOG_TRACE) ;
		}
	} ;

	//GroupCount; se usa luego en bucle

	m_iGroupCount=MIT_TGI_NOT_SHARED_FIRST+ai_iGroupCount ;

	//Inicializo LocalTrace. Si hay error, se intenta comunicar via FE_OUT. 
	//	Si se inicializa sin problemas, saca el error de init de m4log, si lo hay.

	if ( m_oObjFlags.True(LOCAL_TRACE) ) {
		InitLocalError= M4_BOOL( ! m_oFile.Open( ai_pcLocalTraceFile, ClMIT_File::OVERWRITE, 
			ClMIT_File::WRITE, ClMIT_File::SILENT_TRY_OPEN, M4_TRUE ));
		if (InitLocalError) {
			SwOk=M4_FALSE ;
			m_oObjFlags.Off(LOCAL_TRACE) ;
			//MENSAJE SOLO FE
			*this << ClMIT_Msg( &TRACE_ARRAY[ TRACE_MSG_90 ] ) << ai_pcLocalTraceFile
				<< SEND_MSG ;

		} else {

			//Imprimir cabecera de fichero report
			
			m_oFile.WStr( "<Header>\r\nVERSION=1.0;\r\nTRACEREADER=RPT;\r\nLAYER=") ;
			m_oFile.WStr( ai_pcLayerId ) ;
			m_oFile.WStr( ";\r\nCOMPONENT=" ) ;
			m_oFile.WStr( ai_pcComponentId ) ;
			m_oFile.WStr( ";\r\n" ) ;
			
			m_oFile.WStr( "THREAD=" ) ;
			ClMIT_Str strAux ;
			strAux.Accommodate(100);
			sprintf(strAux, "%lu", (unsigned long)M4Thread::GetCurrentIdThread() ) ;
			m_oFile.WStr(strAux) ;
			/*
			m_oFile.WStr("GROUPS " ) ;
			for (B=0; B<GetGroupCount(); ++B) {
				m_oFile.WStr( m_stGroup[B].GroupName) ;
				m_oFile.WChar(' ') ;
			} ;
			*/

			m_oFile.WStr(";\r\n</Header>\r\n\r\n") ;

			if (InitLogError) { 
				//MENSAJE SOLO BE
				*this << ClMIT_Msg( &TRACE_ARRAY[ TRACE_MSG_60 ] ) << SEND_MSG ;
			} ;
		} ;

	} ;

	if ( GetGroupCount() > GROUP_COUNT ) {
		*this << ClMIT_Msg( &TRACE_ARRAY[ TRACE_MSG_110 ] ) << SEND_MSG ;
		ImmediateError() ;
	} ;

	m_oObjFlags.Off(LOCK_EC_PROPAGATE) ;

	return SwOk ;
}


void ClMIT_Trace::Indent( m4int16_t ai_iCount ) 
{
	m_iIndent += ai_iCount ;
	m_iIndent = M4_MAX( 0, m_iIndent ) ;
}

void ClMIT_Trace::SetGroupDetail( 
	ClMIT_Msg::MsgGroup_t ai_From, ClMIT_Msg::MsgGroup_t ai_To, ClMIT_Msg::MSG_LEVEL ai_Detail  ) 
{
	while ( ai_From <= ai_To ) {
		m_stGroup[ai_From].Detail=ai_Detail ;
		++ai_From ;
	} ;
}



void ClMIT_Trace::Flush()
{
	if (m_oObjFlags.True(LOCAL_TRACE)) {
		m_oFile.Flush() ;
	} ;
}

m4bool_t ClMIT_Trace::IsAllOK()
{
	return  M4_BOOL( GetErrorCount() == 0 && GetWarningCount() ==0 && GetDummyCount() ==0 ); 
}

void ClMIT_Trace::NotifyDummy()
{
	++m_oOkCount.m_uiDummyObjects;
	#ifdef MIT_TRACE
	if(!this->IsTraceCodeRipped()){
		*this << ClMIT_Msg( &TRACE_ARRAY[ TRACE_MSG_40 ] ) << SEND_MSG ;
	};
	#endif
}

m4bool_t ClMIT_Trace::CheckPoint( const m4char_t * ai_pcStr ) 
{
	m4bool_t Return=M4_TRUE; 

	if ( IsInicialized() ) {
		Return = IsAllOK() ;
		if ( !Return ) {
			if ( m_oOkCount.IsError() )  { 
				*this << ClMIT_Msg( &TRACE_ARRAY[ TRACE_MSG_30 ] ) 
					<<  GetIC_ErrorCount() << GetIC_WarningCount() 
					<<  GetEC_ErrorCount() << GetEC_WarningCount() << SEND_MSG ;
			} else if ( m_oOkCount.OnlyWarnings() ) {
				*this << ClMIT_Msg( &TRACE_ARRAY[ TRACE_MSG_35 ] ) 
					<< GetIC_WarningCount() << GetEC_WarningCount()  << SEND_MSG ;
			} ;
		} ;
	} ;
	return Return ;
}

void ClMIT_Trace::TestEffect( m4bool_t ai_SwOK,  ERROR_EFFECT ai_ErrorEffect ) 
{
	if (!ai_SwOK && ai_ErrorEffect==IMMEDIATE_ERROR) ImmediateError() ;
}

void ClMIT_Trace::ImmediateError() 
{
	*this << ClMIT_Msg( &TRACE_ARRAY[ TRACE_MSG_50 ] ) << SEND_MSG ;
	Flush() ;
	//m_poEnv->ShutDown(M4_TRUE) ;
}

const m4char_t * ClMIT_Trace::GetFileName() 
{
	return 	m_oObjFlags.True(LOCAL_TRACE) ? m_oFile.GetName() : "" ;
}


void ClMIT_Trace::SetMove( MOVE_TRACE_MODES ai_MoveMode, m4char_t *ai_pcStr )  
{
	m_MoveMode = ai_MoveMode ;
	strcpy(m_oMoveFile,ai_pcStr) ;
	ClMIT_Str::OverStringValid(m_oMoveFile,SIZE_MOVE_FILE) ;
}

void ClMIT_Trace::WrapOutput(m4char_t *ai_pcStr)
{
	m4char_t *PosB, *PosAux ;
	size_t SizeSource, IndentCount, FreeSize ;
	m4bool_t SwFirst ;
	m4char_t * Sep= " -" ;
	m4char_t * IndentStr= m_poInsp->m_poEnv->ClMIT_Trace_pcBuffAux2 ;

	if ( m_oOutputFlags[ClMIT_Msg::BE_OUT].True(INDENT) ) {
		IndentCount = (m_iIndent+1)*m_iBlanksPerIndent ;
	} else {
		IndentCount = m_iBlanksPerIndent ;
	} ;
	memset( IndentStr, ' ' , IndentCount) ;
	IndentStr[IndentCount]=M4_END_STR ;
	ClMIT_Str::OverStringValid(IndentStr,StMIT_InspEnv::ClMIT_Trace_SIZE_BUFFER) ;

	//La primera vez ya estan metidos los blancos de indentacion.
	for( PosB=ai_pcStr, SwFirst=M4_TRUE ; *PosB; PosB+=SizeSource ) {
		if ( IndentCount+5 < m_iWrap ) {
			FreeSize = SwFirst ? m_iWrap : m_iWrap - IndentCount ; 
		} else {
			FreeSize = IndentCount+m_iWrap ; 
		} ;

		//SizeSource tiene los char a imprimir de la cadena original
		SizeSource = strlen(PosB) ;
		SizeSource=M4_MIN(SizeSource,FreeSize) ;

		//Test para no partir palabras
		PosAux=PosB+SizeSource-1 ;
		if ( !strchr(Sep,PosAux[1]) && PosAux[1]!=M4_END_STR ) {	
			//Entra si siguiente char al que hay que sacar NO es un separador ni fin de cadena
			for ( ; PosAux > PosB; --PosAux )  {
				if ( strchr(Sep,*PosAux) ) break ;
			} ;
			//PosAux contiene PosB o el char que separa palabra. Solo si es separador podemos 
			//	recortar (si no hay separadores, va toda la palabra).
			if ( strchr(Sep,PosAux[0]) ) {
				SizeSource=PosAux-PosB+1 ;
			} ;
		} ;

		if ( !SwFirst ) {
			//Debe insertar blancos antes, si no blancos ya vienen en la cadena
			m_oFile.WStr(IndentStr) ;
		} ;
		m_oFile.W(PosB,SizeSource) ;
		m_oFile.WStr(LF_SEQUENCE) ;
		SwFirst=M4_FALSE ;
	} ;
}



ClMIT_Trace &ClMIT_Trace::operator << (const ClMIT_PushLoc &ai_Par) 
{
	if ( m_iLocationCount < LOCATION_ITEMS ) {
		m_stLocation[m_iLocationCount].m_MsgCode  =  ai_Par.m_Code ;
		m_stLocation[m_iLocationCount].m_pcMsgStr =  ai_Par.m_pcMsg ; 
		m_stLocation[m_iLocationCount].m_iFirstParam =  m_iParamCount ;
		m_stLocation[m_iLocationCount].m_iParamCount = 0 ;
		++m_iLocationCount ;
	} ;
	return *this ;
}

void ClMIT_Trace::RestorePushState(m4int16_t ai_State) 
{

	if ( m_iLocationCount>0 && m_iLocationCount > ai_State ) {
		while ( m_iLocationCount != ai_State ) {
			m_iLocParamCount -= m_stLocation[m_iLocationCount-1].m_iParamCount ;
			m_iParamCount    -= m_stLocation[m_iLocationCount-1].m_iParamCount ;
			--m_iLocationCount ;
		} ;
	} ;
}


ClMIT_Trace &ClMIT_Trace::operator << (TRACE_MSG_COMMAND ai_Command)
{
	m4int16_t IntAux ;

	switch (ai_Command) {

	case SEND_MSG:

		if ( !IsInicialized() || IsCancelOutput() ) break ;

		if ( m_oObjFlags.True(LOCAL_TRACE) &&  
			( m_oTypeMsgFlags[m_oMsg.m_Type][ClMIT_Msg::BE_OUT].True(ALWAYS_OUT) ||  
			( m_oMsg.m_Out==ClMIT_Msg::BE_OUT || m_oMsg.m_Out==ClMIT_Msg::MIXED_OUT)) 
			&&
			( m_oTypeMsgFlags[m_oMsg.m_Type][ClMIT_Msg::BE_OUT].True(ALWAYS_LEVEL) ||  
			m_oMsg.m_MsgLevel <= m_stGroup[m_oMsg.m_Group].Detail ) 

			&& m_oFile.IsOpen()

			) 
		{
			//Salida BE_OUT debe realizarse!!

			BEOut() ;

		};

		if ( m_oObjFlags.True(M4LOG_TRACE) && 
			( m_oTypeMsgFlags[m_oMsg.m_Type][ClMIT_Msg::FE_OUT].True(ALWAYS_OUT) ||  
			( m_oMsg.m_Out==ClMIT_Msg::FE_OUT || m_oMsg.m_Out==ClMIT_Msg::MIXED_OUT)) 
			&&
			( m_oTypeMsgFlags[m_oMsg.m_Type][ClMIT_Msg::FE_OUT].True(ALWAYS_LEVEL) ||  
			m_oMsg.m_MsgLevel <= m_stGroup[m_oMsg.m_Group].Detail ) 
			) 
		{
			//Salida FE_OUT debe realizarse!!

			FEOut() ;

		};

		if ( m_oMsg.m_Type==ClMIT_Msg::MSG_ERROR ) {
			if ( m_oMsg.m_Flag.False(ClMIT_Msg::EC_MSG) ) {
				++m_uiErrorId ;
				if ( m_oMsg.m_Flag.False(ClMIT_Msg::SOFT_NOTIFY) ) {
					++m_oOkCount.m_uiErrorCount ;
					++(m_stGroup[m_oMsg.m_Group].uiErrorCount) ;
				} ;
			}  else {
				++m_oOkCount.m_uiEC_ErrorCount ;
			} ;
		} else if ( m_oMsg.m_Type==ClMIT_Msg::MSG_WARNING ) {
			if ( m_oMsg.m_Flag.False(ClMIT_Msg::EC_MSG) ) {
				++m_uiWarningId ;
				if ( m_oMsg.m_Flag.False(ClMIT_Msg::SOFT_NOTIFY)  ) {
					++m_oOkCount.m_uiWarningCount ;
					++(m_stGroup[m_oMsg.m_Group].uiWarningCount) ;
				} ;
			} else {
				++m_oOkCount.m_uiEC_WarningCount ;
			};
		} ;

		if ( m_oObjFlags.False(LOCK_EC_PROPAGATE) && 
			( m_oMsg.m_Type==ClMIT_Msg::MSG_ERROR || m_oMsg.m_Type==ClMIT_Msg::MSG_WARNING )
			&& m_oMsg.m_Flag.False(ClMIT_Msg::EC_MSG) ) 
		{
			m4char_t * Buff = m_poInsp->m_poEnv->ClMIT_Trace_pcBuffAux ;
			ClMIT_Str Str ; 

			LogPrintF(m_oMsg.m_pcMsg, Buff,  GetMsgFirstParam(), GetMsgParamCount() ) ;
			if ( strlen(Buff) >= StMIT_InspEnv::ClMIT_Trace_SIZE_BUFFER ) {
				m_iIndent=0 ;	//Por si indentacion provoca error, no caer en bucle infinito		
				*this << ClMIT_Msg( &TRACE_ARRAY[ TRACE_MSG_10 ] ) << SEND_MSG ;
				ImmediateError() ;
			} ;
			Str.StrCpy(Buff) ;	//Hay que copiarlo, Buff se reutiliza en el mismo thread

			GET_TM()->NotifyProblem(this, m_oMsg.m_Type, 
				m_oMsg.m_Type==ClMIT_Msg::MSG_ERROR ? m_uiErrorId-1 : m_uiWarningId-1, Str) ;

		} ;


		m_iParamCount=m_iLocParamCount ;

		break;

	case ENTRY_MSG:
		m_oMsg.m_Flag.On(ClMIT_Msg::ENTRY_FORMAT|ClMIT_Msg::NO_WRAP) ; 
		m_oMsg.m_Flag.Off(ClMIT_Msg::EXIT_FORMAT) ;
		break;

	case EXIT_MSG:
		m_oMsg.m_Flag.On(ClMIT_Msg::EXIT_FORMAT|ClMIT_Msg::NO_WRAP) ;
		m_oMsg.m_Flag.Off(ClMIT_Msg::ENTRY_FORMAT) ; 
		break;

	case NO_WRAP_MSG:
		m_oMsg.m_Flag.On(ClMIT_Msg::NO_WRAP) ;
		break;

	case GENERAL_MSG:
		m_oMsg.m_MsgLevel=ClMIT_Msg::GENERAL ;
		break;

	case DETAILED_MSG:
		m_oMsg.m_MsgLevel=ClMIT_Msg::DETAILED ;
		break;

	case CRAZY_MSG:
		m_oMsg.m_MsgLevel=ClMIT_Msg::CRAZY ;
		break;

	case END_LOC:
		
		IntAux = m_stLocation[m_iLocationCount-1].m_iFirstParam ;
		if (IntAux > m_iParamCount-1 ) {
			IntAux=0 ;	//No se llego a meter ningun parametro
		} else {
			IntAux= m_iParamCount - IntAux ;
		} ;
		m_stLocation[m_iLocationCount-1].m_iParamCount = IntAux ;
		m_iLocParamCount += IntAux ;

		break;

	case POP_LOC:
		if (m_iLocationCount>0) {
			m_iLocParamCount -= m_stLocation[m_iLocationCount-1].m_iParamCount ;
			m_iParamCount    -= m_stLocation[m_iLocationCount-1].m_iParamCount ;
			--m_iLocationCount ;
		} ;
		break;
	} ;
	
	return *this;
}

ClMIT_Trace &ClMIT_Trace::operator << (const ClMIT_Msg &ai_Msg) 
{
	m_oMsg.Init(&ai_Msg) ;
	return *this ; 
}


ClMIT_Trace &ClMIT_Trace::operator << (const m4char_t *ai_Par) 
{
	StParam * Param = RequestNewParam() ;
	if (Param) {
		Param->m_Type=StParam::LINK_STR ;
		Param->m_pcLinkStr =ai_Par ;
		Param->m_LinkStrLen=-1 ;
	} ;
	return *this;
}

ClMIT_Trace &ClMIT_Trace::operator << (const ClMIT_StrNLink &ai_Par) 
{
	StParam * Param = RequestNewParam() ;
	if (Param) {
		Param->m_Type=StParam::LINK_STR ;
		Param->m_pcLinkStr =ai_Par.m_pcMsg ;
		Param->m_LinkStrLen=ai_Par.m_LinkStrLen ;
	} ;
	return *this;
}

ClMIT_Trace &ClMIT_Trace::operator << (m4char_t ai_Par) 
{
	StParam * Param = RequestNewParam() ;
	if (Param) {
		Param->m_Type=StParam::SINGLE_CHAR ;
		Param->m_cChar=ai_Par ;
	} ;
	return *this;
}

ClMIT_Trace &ClMIT_Trace::operator << (m4bool_t ai_Par) 
{
	StParam * Param = RequestNewParam() ;
	if (Param) {
		Param->m_Type=StParam::BOOLEAN ;
		Param->m_bSw = ai_Par ;
	} ;
	return *this;
}

ClMIT_Trace &ClMIT_Trace::operator << (m4int32_t ai_Par) 
{
	StParam * Param = RequestNewParam() ;
	if (Param) {
		Param->m_Type=StParam::NUMERIC ;
		sprintf( Param->m_pcNumStr, "%li", ai_Par ) ;
		if ( strlen(Param->m_pcNumStr) > MAX_SIZE_PARAM-1 ) {
			m_iIndent=0 ;	//Por si indentacion provoca error, no caer en bucle infinito		
			*this << ClMIT_Msg( &TRACE_ARRAY[ TRACE_MSG_10 ] ) << SEND_MSG ;
			ImmediateError() ;
		} ;
	} ;
	return *this;
}


ClMIT_Trace &ClMIT_Trace::operator << (m4uint32_t ai_Par) 
{
	StParam * Param = RequestNewParam() ;
	if (Param) {
		Param->m_Type=StParam::NUMERIC ;
		sprintf( Param->m_pcNumStr, "%lu", ai_Par ) ;
		if ( strlen(Param->m_pcNumStr) > MAX_SIZE_PARAM-1 ) {
			m_iIndent=0 ;	//Por si indentacion provoca error, no caer en bucle infinito		
			*this << ClMIT_Msg( &TRACE_ARRAY[ TRACE_MSG_10 ] ) << SEND_MSG ;
			ImmediateError() ;
		} ;
	} ;
	return *this;
}


ClMIT_Trace &ClMIT_Trace::operator << (m4int16_t ai_Par) 
{
	StParam * Param = RequestNewParam() ;
	if (Param) {
		Param->m_Type=StParam::NUMERIC ;
		sprintf( Param->m_pcNumStr, "%i", ai_Par ) ;
		if ( strlen(Param->m_pcNumStr) > MAX_SIZE_PARAM-1 ) {
			m_iIndent=0 ;	//Por si indentacion provoca error, no caer en bucle infinito		
			*this << ClMIT_Msg( &TRACE_ARRAY[ TRACE_MSG_10 ] ) << SEND_MSG ;
			ImmediateError() ;
		} ;
	} ;
	return *this;
}


ClMIT_Trace &ClMIT_Trace::operator << (m4uint16_t ai_Par) 
{
	StParam * Param = RequestNewParam() ;
	if (Param) {
		Param->m_Type=StParam::NUMERIC ;
		sprintf( Param->m_pcNumStr, "%u", ai_Par ) ;
		if ( strlen(Param->m_pcNumStr) > MAX_SIZE_PARAM-1 ) {
			m_iIndent=0 ;	//Por si indentacion provoca error, no caer en bucle infinito		
			*this << ClMIT_Msg( &TRACE_ARRAY[ TRACE_MSG_10 ] ) << SEND_MSG ;
			ImmediateError() ;
		} ;
	} ;
	return *this;
}



ClMIT_Trace &ClMIT_Trace::operator << (m4double_t ai_Par) 
{
	StParam * Param = RequestNewParam() ;
	if (Param) {
		Param->m_Type=StParam::NUMERIC ;

		sprintf( Param->m_pcNumStr, "%.*f", (int)2, ai_Par ) ;
		if ( strlen(Param->m_pcNumStr) > MAX_SIZE_PARAM-1 ) {
			m_iIndent=0 ;	//Por si indentacion provoca error, no caer en bucle infinito		
			*this << ClMIT_Msg( &TRACE_ARRAY[ TRACE_MSG_10 ] ) << SEND_MSG ;
			ImmediateError() ;
		} ;
	} ;
	return *this;
}

ClMIT_Trace &ClMIT_Trace::operator << (const ClMIT_IdDesc &ai_Par) 
{
	StParam * Param = RequestNewParam() ;
	if (Param) {
		Param->m_Type=StParam::ID_DESC ;
		Param->m_pcIdDesc[0]=ai_Par.GetId();
		Param->m_pcIdDesc[1]=ai_Par.GetDesc();
	} ;
	return *this;
}

ClMIT_Trace::StParam * ClMIT_Trace::RequestNewParam() 
{
	if (m_iParamCount==MAX_ITEMS_PARAM) {
		*this << ClMIT_Msg( &TRACE_ARRAY[ TRACE_MSG_80 ] ) << SEND_MSG ;
		return NULL ;
	} else {
		++m_iParamCount ;
		return &m_stParamArray[m_iParamCount-1] ;
	} ;
}

void ClMIT_Trace::LogPrintF(const m4char_t *ai_pcFormatStr, m4char_t *ai_pcTarget,
						m4int16_t ai_iFirstParam, m4int16_t ai_iParamCount) 
{
	const m4char_t  *Pos, *Beg, *End ;
	m4int16_t ArgIndex ;
	m4bool_t SwOk ;
	StParam *Param=NULL ;

	ai_pcTarget[0]=M4_END_STR ;

	for (Pos=ai_pcFormatStr; *Pos!=M4_END_STR;) {
		if ( ( Beg=strchr(Pos,'%') ) ) {
			
			strncat(ai_pcTarget,Pos,Beg-Pos) ;	//Copia lo que hay hasta el momento (% excluido)
	
			SwOk=M4_TRUE ;

			End=strchr(Beg,':') ;			//Ahora a hayar el ordinal de parametro
			if (End) {	
				ArgIndex = atoi(Beg+1) ; //El : para la conversion, es correcto. +1 salta %
											
				switch (*(End+1)) {
				case 's':
					//El valor es correcto, se actualiza Beg conforme a eso !!
					Beg=End+2 ;	//+2 para saltarse el s
					if ( M4_IN_RANGE(ArgIndex,0,ai_iParamCount-1) ) {
						Param=&m_stParamArray[ai_iFirstParam+ArgIndex] ;
						strcat(ai_pcTarget,"<") ;
						switch(Param->m_Type) {
						case StParam::LINK_STR:
							if (Param->m_LinkStrLen==-1) strcat(ai_pcTarget,Param->m_pcLinkStr) ;
							else strncat(ai_pcTarget,Param->m_pcLinkStr,Param->m_LinkStrLen) ;
							break ;
						case StParam::NUMERIC:
							strcat(ai_pcTarget,Param->m_pcNumStr) ;
							break ;
						case StParam::BOOLEAN:
							strcat(ai_pcTarget,Param->m_bSw ? "Y": "N" ) ;
							break ;
						case StParam::ID_DESC:
							if ( m_oOutputFlags[ClMIT_Msg::BE_OUT].True(ID_OUT) ) {
								strcat(ai_pcTarget,Param->m_pcIdDesc[0]) ;
								if ( m_oOutputFlags[ClMIT_Msg::BE_OUT].True(DESC_OUT) ) {
									strcat(ai_pcTarget,"::") ;
								} ;
							} ;
							if ( m_oOutputFlags[ClMIT_Msg::BE_OUT].True(DESC_OUT) ) {
								strcat(ai_pcTarget,Param->m_pcIdDesc[1]) ;
							} ;
							break ;
						case StParam::SINGLE_CHAR:
							m4char_t Str[2] ;
							Str[0]=Param->m_cChar ; Str[1]=M4_END_STR ;
							strcat(ai_pcTarget,Str) ;
							break ;
						} ;
						strcat(ai_pcTarget,">") ;
					} ;		//Si arg no existe, no copiara nada
					break;
				default:
					SwOk=M4_FALSE ;	//Tipo no tratado
					break ;
				} ;
			} else SwOk=M4_FALSE ;	//No puede hayar ordinal
			
			
			if (!SwOk) {
				//Si hay problemas, copia un % para saltarse el parametro. La siguiente pasada
				//	copiara lo de demas.
				strcat(ai_pcTarget,"%");
				Pos=Beg+1;
			} else Pos=Beg ;

		} ;

		if (!Beg) {
			strcat(ai_pcTarget,Pos) ;
			Pos += strlen(Pos) ;
		} ;
	} ;
}


void ClMIT_Trace::LogParams(m4char_t *ai_pcTarget, StParam* ai_Param) 
{
	 
	switch(ai_Param->m_Type) {
	case StParam::LINK_STR:
		if (ai_Param->m_LinkStrLen==-1) strcat(ai_pcTarget,ai_Param->m_pcLinkStr) ;
		else strncat(ai_pcTarget,ai_Param->m_pcLinkStr,ai_Param->m_LinkStrLen) ;
		break ;
	case StParam::NUMERIC:
		strcat(ai_pcTarget,ai_Param->m_pcNumStr) ;
		break ;
	case StParam::BOOLEAN:
		strcat(ai_pcTarget,ai_Param->m_bSw ? "Y": "N" ) ;
		break ;
	case StParam::ID_DESC:
		strcat(ai_pcTarget,ai_Param->m_pcIdDesc[0]) ;
		strcat(ai_pcTarget,"&&") ;
		strcat(ai_pcTarget,ai_Param->m_pcIdDesc[1]) ;
		break ;
	case StParam::SINGLE_CHAR:
		m4char_t Str[2] ;
		Str[0]=ai_Param->m_cChar ; Str[1]=M4_END_STR ;
		strcat(ai_pcTarget,Str) ;
		break ;
	} ;
}

void ClMIT_Trace::OldLogParams(m4char_t *ai_pcTarget, m4int16_t ai_iFirstParam, m4int16_t ai_iParamCount) 
{
	m4int16_t B ;
	StParam *Param=NULL ;
	 
	for (B=0, Param=&m_stParamArray[ai_iFirstParam], ai_pcTarget[0]=M4_END_STR ; 
		B<ai_iParamCount; ++B , ++Param ) 
	{
		strcat(ai_pcTarget,"#") ;
		switch(Param->m_Type) {
		case StParam::LINK_STR:
			if (Param->m_LinkStrLen==-1) strcat(ai_pcTarget,Param->m_pcLinkStr) ;
			else strncat(ai_pcTarget,Param->m_pcLinkStr,Param->m_LinkStrLen) ;
			break ;
		case StParam::NUMERIC:
			strcat(ai_pcTarget,Param->m_pcNumStr) ;
			break ;
		case StParam::BOOLEAN:
			strcat(ai_pcTarget,Param->m_bSw ? "Y": "N" ) ;
			break ;
		case StParam::ID_DESC:
			strcat(ai_pcTarget,Param->m_pcIdDesc[0]) ;
			strcat(ai_pcTarget,"&&") ;
			strcat(ai_pcTarget,Param->m_pcIdDesc[1]) ;
			break ;
		case StParam::SINGLE_CHAR:
			m4char_t Str[2] ;
			Str[0]=Param->m_cChar ; Str[1]=M4_END_STR ;
			strcat(ai_pcTarget,Str) ;
			break ;
		} ;
	} ;
}

void ClMIT_Trace::BEOut() 
{
	m4char_t * Buff = m_poInsp->m_poEnv->ClMIT_Trace_pcBuffAux ;
	m4int16_t B ;

	Buff[0]=M4_END_STR ;
	size_t l, blanks ;

	//UnIndent de EXIT_FORMAT, antes de imprimir mensaje

	if (m_oMsg.m_Flag.True(ClMIT_Msg::EXIT_FORMAT) ) Unindent() ;
	
	//Insercion valores traza

	sprintf(Buff, "[%10.10lu%c%c%1.1i%3.3i%-8s]", 
		(long unsigned)ClPrecisionTicker::GetTimeTick(),
		(char)(m_oMsg.m_Type==ClMIT_Msg::MSG_DEBUG ? 'D' : (m_oMsg.m_Type==ClMIT_Msg::MSG_ERROR ? 'E' : 'W') ),
		(char)(m_oMsg.m_Flag.True(ClMIT_Msg::ENTRY_FORMAT) ? 'T' : (m_oMsg.m_Flag.True(ClMIT_Msg::EXIT_FORMAT) ? 'X' : 'B') ),
		(int)m_oMsg.m_MsgLevel,
		(int)m_iIndent,
		m_stGroup[m_oMsg.m_Group].GroupName
	) ;
	
	//Indenta
	
	if ( m_oOutputFlags[ClMIT_Msg::BE_OUT].True(INDENT) ) {
		l = strlen(Buff) ;
		blanks = m_iIndent*m_iBlanksPerIndent ;
		memset(&Buff[l],' ', blanks) ;
		Buff[l+blanks] = M4_END_STR ;
	} ;


	// Pre-texto de formato ENTRY o EXIT

	if ( m_oMsg.m_Flag.True(ClMIT_Msg::ENTRY_FORMAT) ) strcat( Buff,"->->->" ) ;
	else if (m_oMsg.m_Flag.True(ClMIT_Msg::EXIT_FORMAT) ) strcat( Buff,"<-<-<-" ) ;

	//Añade grupo

	if ( m_oTypeMsgFlags[m_oMsg.m_Type][ClMIT_Msg::BE_OUT].True(SHOW_GROUP) ) {
		sprintf( & Buff[ strlen(Buff) ], "<Group %4i> ", (int)m_oMsg.m_Group ) ;
	} ;

	//Añade tipo

	if ( m_oTypeMsgFlags[m_oMsg.m_Type][ClMIT_Msg::BE_OUT].True(SHOW_TYPE) ) {
		switch( m_oMsg.m_Type ) 
		{
		case ClMIT_Msg::MSG_ERROR:
			strcat(Buff,"<ERROR !!!!!!!!!> ") ;
			break;
		case ClMIT_Msg::MSG_WARNING:
			strcat(Buff,"<WARNING !!!!!!!> ") ;
			break;
		case ClMIT_Msg::MSG_DEBUG:
			strcat(Buff,"<DEBUG> ") ;
			break;
		} ;
	} ;

	//Añade error/warningId

	if ( m_oMsg.m_Flag.False(ClMIT_Msg::EC_MSG) ) {
		switch( m_oMsg.m_Type ) 
		{
		case ClMIT_Msg::MSG_ERROR:
			sprintf( & Buff[ strlen(Buff) ], "<E$%lu> ", (unsigned long)m_uiErrorId ) ;
			break;
		case ClMIT_Msg::MSG_WARNING:
			sprintf( & Buff[ strlen(Buff) ], "<W$%lu> ", (unsigned long)m_uiWarningId ) ;
			break;
		} ;
	} ;
		
	//Añade codigo

	if ( m_oTypeMsgFlags[m_oMsg.m_Type][ClMIT_Msg::BE_OUT].True(SHOW_CODE) ) {
		sprintf( & Buff[ strlen(Buff) ], "<Code %10li> ", (long int)m_oMsg.m_Code ) ;
	} ;


	//Añade mensaje con formateo e inclusion de parametros
	LogPrintF(m_oMsg.m_pcMsg, & Buff[ strlen(Buff) ],  GetMsgFirstParam(), GetMsgParamCount() ) ;

	//Añade cadena de localizacion

	if ( m_iLocationCount>0 && m_oTypeMsgFlags[m_oMsg.m_Type][ClMIT_Msg::BE_OUT].True(PUSH_LOCATION) ) {
		strcat( Buff, " <<<< Location : ") ;
		for (B=0; B<m_iLocationCount; ++B ) {
			LogPrintF(m_stLocation[B].m_pcMsgStr, & Buff[ strlen(Buff) ],  
				m_stLocation[B].m_iFirstParam, m_stLocation[B].m_iParamCount ) ;
			if (B != m_iLocationCount-1) strcat( Buff, " # ") ;
		} ;
		strcat( Buff, " >>>>") ;
	} ;

	// Post-texto de formato ENTRY o EXIT

	if ( m_oMsg.m_Flag.True(ClMIT_Msg::ENTRY_FORMAT) ) strcat( Buff,"->->->->->->->->->->->->->->->" ) ;
	else if (m_oMsg.m_Flag.True(ClMIT_Msg::EXIT_FORMAT) ) strcat( Buff,"<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-" ) ;

	// Saca a Wrap o linea simple, segun corresponda, añadiendo LF

	if ( m_oMsg.m_Flag.True(ClMIT_Msg::ENTRY_FORMAT) && 
		m_oTypeMsgFlags[m_oMsg.m_Type][ClMIT_Msg::BE_OUT].True(ADD_LF) ) 
	{
		m_oFile.WStr(LF_SEQUENCE) ;
	} ;

	if ( m_iWrap>0 && m_oTypeMsgFlags[m_oMsg.m_Type][ClMIT_Msg::BE_OUT].True(ADD_LF) &&
		m_oMsg.m_Flag.False(ClMIT_Msg::NO_WRAP) ) 
	{
		WrapOutput(Buff) ;			//Ya mete los LF
	} else {
		if ( m_oTypeMsgFlags[m_oMsg.m_Type][ClMIT_Msg::BE_OUT].True(ADD_LF) ) 
			strcat( Buff, LF_SEQUENCE ) ;
		m_oFile.WStr(Buff) ;
	} ;

	if ( m_oMsg.m_Flag.True(ClMIT_Msg::EXIT_FORMAT) && 
		m_oTypeMsgFlags[m_oMsg.m_Type][ClMIT_Msg::BE_OUT].True(ADD_LF) ) 
	{
		m_oFile.WStr(LF_SEQUENCE) ;
	} ;
	
	// FLUSH_SBS

	if (m_oOutputFlags[ClMIT_Msg::BE_OUT].True(FLUSH_SBS)) Flush() ;

	//Indent de ENTRY_FORMAT, despues de imprimir mensaje

	if (m_oMsg.m_Flag.True(ClMIT_Msg::ENTRY_FORMAT) ) Indent() ;

	if ( strlen(Buff) >= StMIT_InspEnv::ClMIT_Trace_SIZE_BUFFER ) {
		m_iIndent=0 ;	//Por si indentacion provoca error, no caer en bucle infinito		
		*this << ClMIT_Msg( &TRACE_ARRAY[ TRACE_MSG_10 ] ) << SEND_MSG ;
		ImmediateError() ;
	} ;
} 


void ClMIT_Trace::FEOut()
{
	m4char_t * Buff = m_poInsp->m_poEnv->ClMIT_Trace_pcBuffAux ;
	StParam *Param=NULL ;
	m4int16_t B, C ;	

	//Traducir las macros al tipo de error del M4Logdll
	switch(m_oMsg.m_Type){
		case ClMIT_Msg::MSG_ERROR: 
			B=ERRORLOG;
			break;
		case ClMIT_Msg::MSG_WARNING: 
			B=WARNINGLOG;
			break;
		case ClMIT_Msg::MSG_DEBUG:
			B=DEBUGINFOLOG;
			break;
	}
	
	//Nueva funcionalidad para pasar mensajes a la M4LOG
	//MACRO de comienzo de volcado de Error
	BL_BEGIN(B, m_oMsg.m_Code);

	for (C=0, Param=&m_stParamArray[GetMsgFirstParam()]; 
	     C<GetMsgParamCount(); ++C , ++Param ) 
	{
		Buff[0]=M4_END_STR;
		LogParams(& Buff[ strlen(Buff) ], Param) ;
		BL_OBJ << Buff;
	}; 

	//Añado cadena de localizacion, si la hay
	if ( m_iLocationCount>0 && m_oTypeMsgFlags[m_oMsg.m_Type][ClMIT_Msg::FE_OUT].True(PUSH_LOCATION) ) {
		for (B=0; B<m_iLocationCount; ++B ) {
			BL_OBJ<<BL_NewError(m_stLocation[B].m_MsgCode);
		    for (C=0, Param=&m_stParamArray[m_stLocation[B].m_iFirstParam], Buff[0]=M4_END_STR ; 
			     C<m_stLocation[B].m_iParamCount; ++C , ++Param ) 	{
			        LogParams(& Buff[ strlen(Buff) ], Param ) ;
					if ( strlen(Buff) >= StMIT_InspEnv::ClMIT_Trace_SIZE_BUFFER ) {
						m_iIndent=0 ;	//Por si indentacion provoca error, no caer en bucle infinito		
						*this << ClMIT_Msg( &TRACE_ARRAY[ TRACE_MSG_10 ] ) << SEND_MSG ;
						ImmediateError() ;
					};
					BL_OBJ << Buff;
			};
		};
	};

	//MACRO de fin de volcado a la pila
	BL_END;
}


void ClMIT_Trace::OldFEOut()
{
	m4char_t * Buff = m_poInsp->m_poEnv->ClMIT_Trace_pcBuffAux ;
	m4int16_t B ;

	Buff[0]=M4_END_STR ;

	//No interviene ningun tipo de formateo

	//Añade codigo "pilla mensaje del *.ini"

	strcat(Buff,"#*S1*") ;
	
	//Añade parametros

	OldLogParams(& Buff[ strlen(Buff) ],  GetMsgFirstParam(), GetMsgParamCount()) ;

	//Añade # final

	strcat(Buff,"#") ;

	//Añado cadena de localizacion, si la hay

	if ( m_iLocationCount>0 && m_oTypeMsgFlags[m_oMsg.m_Type][ClMIT_Msg::FE_OUT].True(PUSH_LOCATION) ) {
		for (B=0; B<m_iLocationCount; ++B ) {
			sprintf( &Buff[strlen(Buff)], "||#*N%lu*S1*", m_stLocation[B].m_MsgCode  ) ;
			OldLogParams(& Buff[ strlen(Buff) ],  
				m_stLocation[B].m_iFirstParam, m_stLocation[B].m_iParamCount ) ;
			if (B != m_iLocationCount-1) strcat( Buff, "#") ;
		} ;
		//Añade # final
		strcat(Buff,"#") ;
	} ;

	//Traducir las macros al tipo de error del M4Logdll
	switch(m_oMsg.m_Type){
	case ClMIT_Msg::MSG_ERROR: 
		B=ERRORLOG;
		break;
	case ClMIT_Msg::MSG_WARNING: 
		B=WARNINGLOG;
		break;
	case ClMIT_Msg::MSG_DEBUG:
		B=DEBUGINFOLOG;
		break;
	}
	SETCODE(m_oMsg.m_Code, B, Buff);

	if ( strlen(Buff) >= StMIT_InspEnv::ClMIT_Trace_SIZE_BUFFER ) {
		m_iIndent=0 ;	//Por si indentacion provoca error, no caer en bucle infinito		
		*this << ClMIT_Msg( &TRACE_ARRAY[ TRACE_MSG_10 ] ) << SEND_MSG ;
		ImmediateError() ;
	} ;

}



void ClMIT_Trace::NotifyCancel() 
{
	if ( IsInicialized() && m_oObjFlags.False(CANCEL_NOTIFIED) ) {
		*this << ClMIT_Msg( &TRACE_ARRAY[ TRACE_MSG_100 ] ) << SEND_MSG ;
		m_oObjFlags.On(CANCEL_NOTIFIED) ;
	} ;
}


void ClMIT_Trace::NotifyEC_Problem(ClMIT_Msg::MSG_TYPE ai_Type, 
		m4uint32_t ai_IdComp, m4char_t *ai_pcIdComp, m4uint32_t ai_uiLocalId, m4char_t *ai_pcMsg) 
{
	ClMIT_Str Str ;

	Str.Accommodate(30) ;

	if (ai_Type==ClMIT_Msg::MSG_ERROR) {
		sprintf( Str, "E$%lu", (unsigned long)ai_uiLocalId ) ;
		*this << ClMIT_Msg( &TRACE_ARRAY[ TRACE_MSG_120] ) 
			<< ai_IdComp << ai_pcIdComp << (m4char_t*)Str << ai_pcMsg << SEND_MSG ;
	} else {
		sprintf( Str, "W$%lu", (unsigned long)ai_uiLocalId ) ;
		*this << ClMIT_Msg( &TRACE_ARRAY[ TRACE_MSG_130] ) 
			<< ai_IdComp << ai_pcIdComp << (m4char_t*)Str << ai_pcMsg << SEND_MSG ;
	} ;
	Str.OverStringValid() ;
}
