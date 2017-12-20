//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             clformat.cpp
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

#include "m4string.hpp"

#include "eng.h"
#include "engdf.inl"

#include "xengval.h"
#include "clformat.hmg"
#include "clformat.h"

#include "m4fmt.hpp"

#include "m4unicode.hpp"

//=========================================================================ClENG_Format

void ClENG_Format::Init( ClENG_DBFormat *ai_poOwner, ClMIT_IdNumDesc *ai_poIdDesc ) 
{

	ClENG_Format::End(); 

	m_oDesc.StrCpy(ai_poIdDesc->GetDesc()) ;
	m_oLocation.Init( ai_poIdDesc->GetIdNum(), m_oDesc ) ;

	m_iIdFormat = ai_poIdDesc->GetIdNum();
	m_poOwner = ai_poOwner ;

	if ( M4FMTCreateNewFormat( m_iTaskId, m_oDesc, &m_iHandle ) !=1 ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLFORMAT_MSG_10 ] ) 
			<< m_oLocation << SEND_MSG ;
		//Efecto local
	} ;

}

ClENG_Format::ClENG_Format() 
{ 
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;
	
	m_poInsp=GET_INSP();
	m_iTaskId=TaskEnv->m_uiFmtTaskId;
	m_iHandle=NULL_HANDLE ; 
}

void ClENG_Format::End()
{
	if ( m_iHandle != NULL_HANDLE ) {
		if ( M4FMTDestroyFormat(m_iTaskId, m_iHandle) != 1 ) {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLFORMAT_MSG_20 ] ) 
				<< m_oLocation << SEND_MSG ;
			//Efecto local
		} ;
		m_iHandle = NULL_HANDLE ;
	} ;
	m_oLocation.Init(0,"?");
}

void ClENG_Format::SetProperty (m4pchar_t ai_pcProperty, m4pchar_t ai_pcValue )
{
	if ( M4FMTSetProperty(m_iTaskId, m_iHandle, ai_pcProperty, ai_pcValue) != 1 ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLFORMAT_MSG_30 ] ) 
			<< ai_pcProperty << ai_pcValue  << m_oLocation << SEND_MSG ;
		//Efecto local
	} ;
}

void ClENG_Format::Apply( const m4char_t *ai_pcInStr, FORMAT_DATA_TYPE ai_Type, ClMIT_Str *ao_poOutStr )
{
	m4int16_t Size2 ;
	m4pchar_t pcString = NULL;
	StENG_TaskEnv * TaskEnv = TM_GET_TASK_ENV(GET_TM()) ;

	m4char_t *Format = TaskEnv->Format_g_pcFormStr ;

	// UNICODE REPORTS.
	// Bug 0132793. Antes de pasarle las cadenas a la dll de formateos, hay 
	// que convertirlas a ANSI porque si no, en unicode, se pasarían en UTF8,
	// con lo que los formatos no se aplicarían.
	int iLen = strlen(ai_pcInStr);
	pcString = M4CppToANSI(ai_pcInStr, iLen);

	if ( M4FMTApplyFormat(m_iTaskId, m_iHandle, (m4char_t *)pcString, ai_Type, 1, Format, 
		StENG_TaskEnv::Format_SIZE_STR_FORMAT, &Size2 ) != 1 ) 
	{
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLFORMAT_MSG_40 ] ) 
			<< m_oLocation << pcString << SEND_MSG ;
		//Efecto local
		ao_poOutStr->StrCpy(pcString) ;
	} else {
		Format[Size2]=M4_END_STR ;
		ClMIT_Str::OverStringValid(Format,StENG_TaskEnv::Format_SIZE_STR_FORMAT) ;
		ao_poOutStr->StrCpy(Format) ;
	} ;

	delete [] pcString;

	// UNICODE REPORTS.
	// Bug 0128779. Convertir los formatos a UTF8 o ANSI. La dll de formateos
	// escribe los formatos en ASCII de toda la vida, pero ahora, en los dispositivos
	// se espera trabajar en unicode o en ansi.
	char *	pcResult = NULL;
	int iLength = strlen(ao_poOutStr->InChar(0));
	pcResult = M4ANSIToCpp(ao_poOutStr->InChar(0), iLength);
	ao_poOutStr->StrCpy(pcResult);

	delete [] pcResult;
}

//=========================================================================ClENG_DBFormat

ClENG_DBFormat::ClENG_DBFormat()
{
	m_poInsp=GET_INSP();
	m_poFormat = NULL;
	m_iNFormats = 0;
}

void ClENG_DBFormat::Init(m4int16_t ai_iNFormats)
{
	ClENG_DBFormat::End();
	m_iNFormats = ai_iNFormats;
	if ( m_iNFormats )
		M4_NEW_ARRAY( m_poInsp->m_poMemProf, ENG_MGI_FORMATS, m_poFormat, ClENG_Format, m_iNFormats );
}

void ClENG_DBFormat::End()
{
	M4_DELETE_ARRAY( m_poInsp->m_poMemProf,ENG_MGI_FORMATS, m_poFormat, ClENG_Format, m_iNFormats );
	m_iNFormats = 0;
}

void ClENG_DBFormat::Save(	
	m4int16_t ai_iIndex, ClMIT_IdNumDesc *ai_poIdDesc, m4pchar_t ai_pcIdCurrency )
{
	ClMIT_Str oTemp;
	
	if (! M4_IN_RANGE( ai_iIndex, 0,  m_iNFormats-1 ) ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLFORMAT_MSG_50 ] ) 
			<< ai_iIndex << 0L << (m4int16_t)(m_iNFormats-1) << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;
	M4_VIRTUAL_OBJECT( &(m_poFormat[ai_iIndex]),ClENG_Format)->
		Init( this, ai_poIdDesc) ;
	oTemp.StrCpy(ai_pcIdCurrency);
	m_oMap.insert(KeyMap_t::value_type (oTemp,&(m_poFormat[ai_iIndex])));
}

ClENG_Format* ClENG_DBFormat::FindFormat( m4uint32_t ai_uiIdFormat, ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect ) 
{
	m4int16_t B, Index=-1;
	
	for (B=0;B<m_iNFormats; ++B) {
		if (m_poFormat[B].GetId() == ai_uiIdFormat){
			Index=B;
			break;
		} ;
	} ;
	
	if (Index==-1) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLFORMAT_MSG_60 ] ) 
			<< ai_uiIdFormat << SEND_MSG ;
		goto exit ;
	} ;

exit:
	m_poInsp->m_poTrace->TestEffect( M4_BOOL(Index!=-1), ai_ErrorEffect ) ; 

	return Index!=-1 ? &m_poFormat[Index] : NULL  ;
}


ClENG_Format* ClENG_DBFormat::GetCurDefaultFormat( m4pchar_t ai_pcIdCurrency, 
		m4bool_t ai_bSwOblig, ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect )
{
	m4int16_t iIndex=-1;
	itKeyMap  itListCur;
	ClENG_Format* poFormato=NULL;
	ClMIT_Str oTemp;

	oTemp.StrCpy( ai_pcIdCurrency );

	itListCur = m_oMap.find( oTemp );
	
	if(itListCur!=m_oMap.end()){
		poFormato=((*itListCur).second);
	}

	if (!poFormato && ai_bSwOblig) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLFORMAT_MSG_70 ] ) 
			<< ai_pcIdCurrency << SEND_MSG ;
		goto exit ;
	} ;
	
exit:
	m_poInsp->m_poTrace->TestEffect( M4_BOOL(poFormato!=NULL), ai_ErrorEffect ) ; 

	return poFormato;

}
