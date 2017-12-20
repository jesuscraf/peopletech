//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                clstatem.inl
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                01-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module...
//
//
//==============================================================================


#include "cursor.hpp"
#include "cllconn.hpp"
#include "sttlib.hpp"
#include "clmtsent.hpp"

//Mejora: 0079820
//Para tener acceso a GetCancelStatus del ejecutor.
//Se ha modificado en .dsp y el .prj para acceder a m4vm/inc
#include "executor.hpp"
#include "ldb_log.hpp"


ClStmt::ClStmt (ClGlobVars *ai_pGlobVars, m4bool_t ai_bIsInternal)
{
	m_pGlobVars = ai_pGlobVars;

	if (m_pGlobVars) {
		m_pGlobVars->Attach ();
	}

	m_pParam = NULL;

	m_pVcClParam = NULL;

	m_pCursor = NULL;

	m_idRConn = 0;

	m_eStmType = M4LDB_SELECT;

	m_pMtSent = NULL;

	m_bIsInternal = ai_bIsInternal;
}


void ClStmt::InitStt (void)
{
	if (m_pGlobVars->GetpLConn ()->m_poStatistics) {
		m_bSttInit = ((m_pGlobVars->GetpLConn ()->m_poStatistics->sttGetInitState() & M4_VM_STATISTICS_ENABLED_TIME) ? M4_TRUE : M4_FALSE);
	}
	else {
		m_bSttInit = M4_FALSE;		
	}
}


m4return_t ClStmt::GetData (m4int32_t ai_lCol, m4pchar_t *aio_ppResult, m4int32_t *aio_plMaxLen, m4int32_t ai_lM4Type, m4pint32_t ao_lLength,m4bool_t ai_bByPass)
{
	m4return_t result;
   
	//Bug 0072356. Comprobamos el puntero.
	if ((m_bSttInit) && 
		(m_pGlobVars->GetpLConn ()->m_poStatistics)) {
		m_pGlobVars->GetpLConn ()->m_poStatistics->sttStartDBClock ();
	}

	result = m_pCursor->GetData (ai_lCol, aio_ppResult, aio_plMaxLen, ai_lM4Type, ao_lLength,ai_bByPass);

	//Bug 0072356. Comprobamos el puntero.
	if ((m_bSttInit) && 
		(m_pGlobVars->GetpLConn ()->m_poStatistics)) {
		m_pGlobVars->GetpLConn ()->m_poStatistics->sttEndDBClock ((m4uint32_t)this, M4_VM_STT_DBFUNCTION_FETCH);
	}

    return (DisplayError(result, M4_TRUE));
}


m4return_t ClStmt::Fetch (void)
{
	m4return_t result = M4_SUCCESS;
	m4bool_t bInterrupted = M4_FALSE;

	//Bug 0072356. Comprobamos el puntero.
	if ((m_bSttInit) && 
		(m_pGlobVars->GetpLConn ()->m_poStatistics)) {
		m_pGlobVars->GetpLConn ()->m_poStatistics->sttStartDBClock ();
	}

	//Mejora: 0079820
	//Interrupcion de la BDL
	if( GetCancelStatus() == M4_TRUE )
	{
		DUMP_LDBINSP_ERRORF( m_pGlobVars->GetpLConn (), M4_CH_LDB_CANCELED, "Fetch" );
		bInterrupted = M4_TRUE;
	}

	if (bInterrupted == M4_FALSE) {
		result = m_pCursor->Fetch ();
	}

	//Bug 0072356. Comprobamos el puntero.
	if ((m_bSttInit) && 
		(m_pGlobVars->GetpLConn ()->m_poStatistics)) {
		m_pGlobVars->GetpLConn ()->m_poStatistics->sttEndDBClock ((m4uint32_t)this, M4_VM_STT_DBFUNCTION_FETCH);
		/*if (result == M4LDB_SQL_NO_DATA_FOUND) {
			m_pGlobVars->GetpLConn ()->m_poStatistics->sttUnlockDBClock ();
		}*/
	}
	
	if (bInterrupted == M4_TRUE) {
		return M4_ERROR;
	}
	
	return (DisplayError(result));
}


m4return_t ClStmt::DescribeCol (m4int32_t ai_lCol, m4pint32_t ao_lPrec, m4pint8_t ao_lM4Type)
{
    m4return_t result;

	//Bug 0072356. Comprobamos el puntero.
	if ((m_bSttInit) && 
		(m_pGlobVars->GetpLConn ()->m_poStatistics)) {
		m_pGlobVars->GetpLConn ()->m_poStatistics->sttStartDBClock ();
	}

	result = m_pCursor->DescribeCol (ai_lCol, ao_lPrec, ao_lM4Type);

	//Bug 0072356. Comprobamos el puntero.
	if ((m_bSttInit) && 
		(m_pGlobVars->GetpLConn ()->m_poStatistics)) {
		m4pcchar_t pSQL = 0;
		m_pCursor->GetStmt (pSQL);
		m_pGlobVars->GetpLConn ()->m_poStatistics->sttEndDBClock ((m4int32_t) this, M4_VM_STT_DBFUNCTION_PREPARE, m_eStmType, m_bIsInternal, pSQL);
	}

	return (DisplayError(result));
}


m4int32_t ClStmt::GetNumCol (void)
{
	m4int32_t result;

	//Bug 0072356. Comprobamos el puntero.
	if ((m_bSttInit) && 
		(m_pGlobVars->GetpLConn ()->m_poStatistics)) {
		m_pGlobVars->GetpLConn ()->m_poStatistics->sttStartDBClock ();
	}

	result = m_pCursor->GetNumCol ();

	//Bug 0072356. Comprobamos el puntero.
	if ((m_bSttInit) && 
		(m_pGlobVars->GetpLConn ()->m_poStatistics)) {
		m4pcchar_t pSQL = 0;
		m_pCursor->GetStmt (pSQL);
		m_pGlobVars->GetpLConn ()->m_poStatistics->sttEndDBClock ((m4int32_t) this, M4_VM_STT_DBFUNCTION_PREPARE, m_eStmType, m_bIsInternal, pSQL);
	}

	return (result);
}


ClDinArr *ClStmt::GetpParams (void)
{
	return (m_pParam);
}


ClMetaSentence *ClStmt::GetMetaSentence (void)
{
	return (m_pMtSent);
}


m4int32_t ClStmt::GetReferences (void)
{
	return (m_mpCursor.size ());
}


m4bool_t ClStmt::AllowCursorOpened (void)
{
	if (m_pCursor) {
		return (m_pCursor->AllowCursorOpened ());
	}

	return (M4_FALSE);
}


ClGlobVars *ClStmt::GetpGlobVars (void)
{
	return (m_pGlobVars);
}


ClCursor *ClStmt::FindCursor (m4int32_t ai_idLConn)
{
	itMpPClCursor it = m_mpCursor.find (ai_idLConn);

	if (it == m_mpCursor.end ()) {
		return (NULL);
	}

	return ((*it).second);
}


ClCursor *ClStmt::GetFirstCursor (void)
{
	return ((*m_mpCursor.begin ()).second);
}


void ClStmt::EraseCursor (m4int32_t ai_idLConn)
{
	m_mpCursor.erase (ai_idLConn);
}


void ClStmt::InsertCursor (m4int32_t ai_idLConn, ClCursor *ai_pCursor)
{
	m_mpCursor.insert (MpPClCursor::value_type (ai_idLConn, ai_pCursor));
}
