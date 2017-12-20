//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             sesschn.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:27/01/98
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

#include "dm.hpp"
#include "cllgadap.hpp"

#include "apichni.h"

#include "sesschn.hmg"
#include "sesschn.h"

//===========================================================================ClMIT_SessionChn

ClMIT_SessionChn::ClMIT_SessionChn() 
{
}

void ClMIT_SessionChn::End() 
{
	ClMIT_Chn::End() ;
}

m4bool_t ClMIT_SessionChn::Init( ClChannelManager *ai_poManager, ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect )
{
	m4bool_t SwOk=M4_TRUE ;
	ClLogonAdaptor *poM4LogonAdaptor;
	ClChannel      *poSessChn ;

	ClMIT_SessionChn::End() ;


	if ( !ai_poManager ) {
		ai_poManager=m_poTaskEnv->m_poMVC->GetChannelManager() ;
	} ;

	poM4LogonAdaptor = ai_poManager->GetLogonAdaptor();
	if (!poM4LogonAdaptor) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	poSessChn=poM4LogonAdaptor->GetSessionChannel() ;
	if (!poSessChn) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	SwOk=InitPrivateAccess(poSessChn,ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!SwOk ) goto exit ;

	m_poSessNode = GetNode( "ROOT_SESSION",ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!m_poSessNode ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	if ( m_oLanguage.Init(m_poSessNode,"LANGUAGE",ClMIT_Trace::PROPAGATE_ERROR)!=ClMIT_ChnItem::ITEM_OK ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	// lenguaje Right To Left (Soporte arabe)
	if ( m_poIsRTL_Language.Init(m_poSessNode, "IS_RTL_LANGUAGE",ClMIT_Trace::PROPAGATE_ERROR)!=ClMIT_ChnItem::ITEM_OK ) {
		SwOk=M4_FALSE ;
		goto exit ;
	}

	// Establecer si el fichero PDF es editable.
	if ( m_poReadOnlyPDF.Init(m_poSessNode, "READ_ONLY_PDF",ClMIT_Trace::PROPAGATE_ERROR) != ClMIT_ChnItem::ITEM_OK ) {
		SwOk=M4_FALSE ;
		goto exit ;
	}

	// Establecer si se debe restringir el directorio de salida de los informes.
	if (m_poRestrictOutputDir.Init(m_poSessNode, "RESTRICT_REPORTS_OUTPUT_DIR", ClMIT_Trace::PROPAGATE_ERROR) != ClMIT_ChnItem::ITEM_OK) 
	{
		SwOk = M4_FALSE;
		goto exit;
	}

	// Se está en pruebas unitarias (se forza al uso de una fecha específica).
	if (m_poM4UnitActive.Init(m_poSessNode, "M4UNIT_ACTIVE", ClMIT_Trace::PROPAGATE_ERROR) != ClMIT_ChnItem::ITEM_OK) 
	{
		SwOk = M4_FALSE;
		goto exit;
	}

exit:
	if (!SwOk) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ SESSCHN_MSG_10 ] ) << SEND_MSG ;
	} ;

	m_poInsp->m_poTrace->TestEffect( SwOk, ai_ErrorEffect ) ; //Aborta si efecto inmediato

	return SwOk ;
}

m4int16_t ClMIT_SessionChn::GetLanguage() 
{
	m_oLanguage.Load() ;
	return (m4int16_t)m_oLanguage.m_oValue.GetDouble() ;
}

m4bool_t ClMIT_SessionChn::IsRTL_Language()
{
	m_poIsRTL_Language.Load() ;
	return M4_BOOL(m_poIsRTL_Language.m_oValue.GetDouble()) ;
}

m4bool_t ClMIT_SessionChn::IsReadOnlyPDF()
{
	m_poReadOnlyPDF.Load() ;
	// Mejora 0301458. Con pruebas unitarias nunca se debe generar un PDF de sólo lectura
	return M4_BOOL(m_poReadOnlyPDF.m_oValue.GetDouble()) == M4_TRUE && M4UnitActive() == M4_FALSE ;
}

m4bool_t ClMIT_SessionChn::GetRestrictOutputDir()
{
	m_poRestrictOutputDir.Load() ;
	return M4_BOOL(m_poRestrictOutputDir.m_oValue.GetDouble()) ;
}

m4bool_t ClMIT_SessionChn::M4UnitActive()
{
	m_poM4UnitActive.Load() ;
	return M4_BOOL(m_poM4UnitActive.m_oValue.GetDouble()) ;
}





