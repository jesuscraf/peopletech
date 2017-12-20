//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb
// File:                clpersbs.cpp
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


#include "m4types.hpp"
#include "m4mdrt.hpp"
#include "cldates.hpp"
#include "node.hpp"
#include "cllgadap.hpp"
#include "ldb_log.hpp"

#include "defmain.hpp"

#include "clldb.hpp"
#include "clload.hpp"
#include "cltblmem.hpp"
#include "clpersbs.hpp"
#include "clpersis.hpp"
#include "clmtsent.hpp"
#include "cldinarr.hpp"

#include "sqlojtrans.hpp"
#include "traductor.hpp"


m4pchar_t	g_ppcLangCodes[ M4CL_LANGUAGE_NUMBER ] = { "ENG", "ENG", "ENG", "ESP", "FRA", "GER", "BRA", "ITA", "GEN" } ;


ClPersistBase::ClPersistBase (ClGlobVars *ai_pGlobVars, ClPersist * ai_pSourcePersist, m4bool_t ai_bIsInternal, m4bool_t ai_bUseTramitation, m4bool_t ai_bUseInheritance, m4bool_t ai_bUseTranslation)

: ClStmt (ai_pGlobVars, ai_bIsInternal)

{
	m_pmpValCur = NULL;
	m_pHistTbl = NULL;
	m_pCorrHistTbl = NULL;
	m_pVcHistory = NULL;
	m_pMtLgcTbl = NULL;
	m_indRealTbl = M4LDB_NULL_INDEX;

	m_dOrdinal = 0;
	m_lOrdinal = 0;

	m_pMtSecSent = NULL;
	
	m_oVParam.reserve(10);

	m_dCompleteDTStart = M4LDB_MINUS_INF_DATE;
	m_dCompleteDTEnd = M4LDB_PLUS_INF_DATE;
	m_dProjection = 0;

	m_lCompleteDTStart = 0;
	m_lCompleteDTEnd = 0;
	m_lProjection = 0;

	// UNICODE DB
	m_pcOrdinal = NULL;
	m_pcProjection = NULL;
	m_pcCompleteDTStart = NULL;
	m_pcCompleteDTEnd = NULL;
	m_iOrdinalMaxLen = 0;
	m_iProjectionMaxLen = 0;
	m_iCompleteDTStart = 0;
	m_iCompleteDTEnd = 0;

	//Es un puntero al persist al que pertenece.
	m_pSourcePersist = ai_pSourcePersist;

	m_bUseTramitation = ai_bUseTramitation;
	m_bUseInheritance = ai_bUseInheritance;
	m_bUseTranslation = ai_bUseTranslation;
	m_idMainTable = NULL;
}

	
ClPersistBase::~ClPersistBase (void)
{
	Destroy ();
}


void ClPersistBase::InitStt (void)
{
	ClStmt * pAuxStm = 0;

	if (m_pmpValCur) 
	{
		for (itMpPValCursor it=m_pmpValCur->begin (); it!=m_pmpValCur->end(); it++) 
		{

			pAuxStm = ((ClStmt *)(*it).second);
			pAuxStm->InitStt();
		}
	}

	ClStmt::InitStt();
}


m4return_t ClPersistBase::Open (eStmType_t ai_eType, m4int32_t ai_idRConn, ClMetaSentence *ai_pMtSent, ClDinArr *ai_pParam, struct stMtLgcTbl *ai_pMtLgcTbl)
{
	m4return_t result;

	m_pmpValCur = new MpPValCursor;

	m_pMtLgcTbl = ai_pMtLgcTbl;

	result = ClStmt::Open (ai_eType, ai_idRConn, ai_pMtSent, M4_FALSE, ai_pParam);

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	m_indRealTbl = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Index;

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::Defuse (ClLConn *ai_pLConn)
{
	m4return_t result;

	if (m_pmpValCur) {
		for (itMpPValCursor it=m_pmpValCur->begin (); it!=m_pmpValCur->end(); it++) {

			result = (*it).second->Defuse (ai_pLConn);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
		}
	}

	result = ClStmt::Defuse (ai_pLConn);

	return (result);
}


m4return_t ClPersistBase::Release (void)
{
	if (m_pHistTbl) {
		m_pHistTbl->Release ();
	}

	if (m_pCorrHistTbl) {
		m_pCorrHistTbl->Release ();
	}

	if (m_pVcHistory) {
		m_pVcHistory->clear ();
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::Destroy (void)
{
	m4return_t result;

	if (m_pSourcePersist) {
		m_pSourcePersist = NULL;
	}

	if (m_pHistTbl) {
		delete m_pHistTbl;
	}
	m_pHistTbl = NULL;

	if (m_pCorrHistTbl) {
		delete m_pCorrHistTbl;
	}
	m_pCorrHistTbl = NULL;

	if (m_pmpValCur) {
		for (itMpPValCursor it=m_pmpValCur->begin (); it!=m_pmpValCur->end(); it++) {
			delete ((*it).second);
		}
		delete m_pmpValCur;
	}
	m_pmpValCur = NULL;

	if (m_pVcHistory) {
		delete m_pVcHistory;
	}
	m_pVcHistory = NULL;

	if (m_pMtSecSent) {
		delete m_pMtSecSent;
	}
	m_pMtSecSent = NULL;

	result = ClStmt::Destroy ();

	return (result);
}


m4return_t ClPersistBase::Close (void)
{
	return ClStmt::Close ();
}


m4return_t ClPersistBase::Prepare (void)
{
	m4return_t result = M4_ERROR;

	switch (m_eStmType)
	{
		case M4LDB_INSERT:
		case M4LDB_DUPLICATE:
			result = PrepareInsert();
			break;

		case M4LDB_UPDATE:
			result = PrepareUpdate();
			break;

		case M4LDB_DELETE:
			result = PrepareDelete();
			break;
	}

	if( result == M4_ERROR )
	{
		return( M4_ERROR ) ;
	}

	if( m_indRealTbl == 0 && m_pMtLgcTbl->m_bIsCacheable == M4_TRUE )
	{
		result = PrepareUpdateVersion() ;

		if( result == M4_ERROR )
		{
			return( M4_ERROR ) ;
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::PrepareOrdinalSelect (m4int32_t ai_indParam)
{
	m4return_t result;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Ordinal Select Lock Row [%ld].", ai_indParam);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Ordinal Select Lock Row [%ld].", ai_indParam);
	#endif


	ClLConn *pLConn = m_pGlobVars->GetpLConn ();
	eDDBBType_t eDDBB = M4DBW_ORACLE_DDBB;
	if (pLConn->GetConnManager()->GetDDBBType( m_idRConn, pLConn, eDDBB ) != M4_SUCCESS){
		return M4_ERROR;
	}
	_ClLockerTranslator *pLock = ClTraductor::s_ppTrUtils[eDDBB-1]->m_pLockTrans;

	m4pcchar_t pIdObject = m_pMtLgcTbl->m_idLgcTbl;
	m4pcchar_t pIdField = m_pParam->GetInfoIdField( ai_indParam );

	string stSelect( "SELECT 1 FROM M4RDC_AUTONUM_LOCK" );
	pLock->AddLockBetweenJoins( stSelect );
	stSelect.append( " WHERE ID_OBJECT = '" );
	stSelect.append( pIdObject );
	stSelect.append( "'" );
	stSelect.append( " AND ID_FIELD = '" );
	stSelect.append( pIdField );
	stSelect.append( "'" );
	pLock->AddLockForUpdate( stSelect );

	ClLoad * pCursor = new ClLoad( m_pGlobVars, M4_TRUE, this );
	
	result = pCursor->Open( m_idRConn, NULL, M4_FALSE, NULL );
	if (result == M4_ERROR) {
		delete pCursor;
		return M4_ERROR;
	}

	result = pCursor->Prepare( (m4pchar_t) stSelect.c_str() );
	if (result == M4_ERROR) {
		delete pCursor;
		return M4_ERROR;
	}

	m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_SELECT_ORDINAL + ai_indParam, pCursor));

	return (result);
}


m4return_t ClPersistBase::PrepareOrdinal (m4int32_t ai_indParam)
{
	m4return_t result;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Ordinal [%ld].", ai_indParam);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Ordinal [%ld].", ai_indParam);
	#endif


	string stSelect;
	string stView;

	stSelect.reserve( 200 );

	m4pcchar_t pRealTable = m_pParam->GetInfoRealTable (ai_indParam);
	m4pcchar_t pRealField = m_pParam->GetInfoRealField (ai_indParam);

	// Si la tabla es tramitable hay que calcular el ordinal entre la tabla tramitable y la de tramitación
	if( m_pMtLgcTbl->m_iMirrorType == 1 )
	{
		stView.reserve( 200 ) ;
		stSelect.reserve( 500 ) ;

		m4pcchar_t pRealObject = M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_REAL_OBJECT].Value.GetWithOutSec ());
		m4pcchar_t pMirrorRealObject = M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_MIRROR_REAL_OBJECT].Value.GetWithOutSec ());

		stView = "(SELECT " ;
		stView.append (pRealField);
		stView.append (" FROM ");
		stView.append (pRealObject);
		stView.append (" UNION ALL SELECT ");
		stView.append (pRealField);
		stView.append (" FROM ");
		stView.append (pMirrorRealObject);
		stView.append (")");

		pRealTable = stView.c_str();
	}

	stSelect = "SELECT MAX(";
	stSelect.append ("T.");
	stSelect.append (pRealField);
	stSelect.append (") FROM ");
	stSelect.append (pRealTable);
	stSelect.append (" T");

	ClLoad *pCursor = new ClLoad (m_pGlobVars, M4_TRUE, this);
	result = pCursor->Open (m_idRConn, NULL, M4_FALSE, m_pParam);

	if (result == M4_ERROR)
	{
		delete pCursor;
		return (M4_ERROR);
	}

	result = pCursor->Prepare ((m4pchar_t) stSelect.c_str ());

	if (result == M4_ERROR)
	{
		delete pCursor;
		return (M4_ERROR);
	}

	// UNIOCODE DB
	m_pcOrdinal = (m4pchar_t) &m_dOrdinal;
	m_iOrdinalMaxLen = M4LDB_SIZE_DOUBLE;

	result = pCursor->BindResult (1, &m_pcOrdinal, &m_iOrdinalMaxLen, M4CL_M4_TYPE_NUMBER, &m_lOrdinal);

	if (result == M4_ERROR)
	{
		delete pCursor;
		return (M4_ERROR);
	}

	m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_CURSOR_ORDINAL + ai_indParam, pCursor));

	return (result);
}


m4return_t ClPersistBase::PrepareOrdinalPKInsert (m4int32_t ai_indParam)
{
	m4return_t result;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Ordinal Primary Key Insert Lock Row [%ld].", ai_indParam);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Ordinal Primary Key Insert Lock Row [%ld].", ai_indParam);
	#endif


	m4pcchar_t pIdObject = m_pMtLgcTbl->m_idLgcTbl;
	m4pcchar_t pIdField = m_pParam->GetInfoIdField( ai_indParam );

	string stInsert( "INSERT INTO M4RDC_AUTOPK_LOCK (ID_OBJECT, ID_FIELD, PK) VALUES ('");
	stInsert.append( pIdObject );
	stInsert.append( "', '" );
	stInsert.append( pIdField );
	stInsert.append( "', ?) " );

	ClLoad * pCursor = new ClLoad( m_pGlobVars, M4_TRUE, this );
	
	result = pCursor->Open( m_idRConn, NULL, M4_FALSE, NULL );
	if (result == M4_ERROR) {
		delete pCursor;
		return M4_ERROR;
	}

	result = pCursor->Prepare( (m4pchar_t) stInsert.c_str() );
	if (result == M4_ERROR) {
		delete pCursor;
		return M4_ERROR;
	}

	m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_INSERT_ORDINAL_PK + ai_indParam, pCursor));

	return (result);
}


m4return_t ClPersistBase::PrepareOrdinalPKDelete (m4int32_t ai_indParam)
{
	m4return_t result;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Ordinal Primary Key Delete Lock Row [%ld].", ai_indParam);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Ordinal Primary Key Delete Lock Row [%ld].", ai_indParam);
	#endif


	m4pcchar_t pIdObject = m_pMtLgcTbl->m_idLgcTbl;
	m4pcchar_t pIdField = m_pParam->GetInfoIdField( ai_indParam );

	string stDelete( "DELETE FROM M4RDC_AUTOPK_LOCK" );
	stDelete.append( " WHERE ID_OBJECT = '" );
	stDelete.append( pIdObject );
	stDelete.append( "'" );
	stDelete.append( " AND ID_FIELD = '" );
	stDelete.append( pIdField );
	stDelete.append( "'" );
	stDelete.append( " AND PK = ?" );

	ClLoad * pCursor = new ClLoad( m_pGlobVars, M4_TRUE, this );
	
	result = pCursor->Open( m_idRConn, NULL, M4_FALSE, NULL );
	if (result == M4_ERROR) {
		delete pCursor;
		return M4_ERROR;
	}

	result = pCursor->Prepare( (m4pchar_t) stDelete.c_str() );
	if (result == M4_ERROR) {
		delete pCursor;
		return M4_ERROR;
	}

	m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_DELETE_ORDINAL_PK + ai_indParam, pCursor));

	return (result);
}


m4return_t ClPersistBase::PrepareOrdinalPK (m4int32_t ai_indParam, m4bool_t ai_bExcludeStartDates)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Ordinal Primary Key [%ld].", ai_indParam);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Ordinal Primary Key [%ld].", ai_indParam);
	#endif


	string stSelect;
	string stFields;
	string stView;

	stSelect.reserve( 500 );

	m4pcchar_t pRealTable = m_pParam->GetInfoRealTable (ai_indParam);
	m4pcchar_t pRealField = m_pParam->GetInfoRealField (ai_indParam);

	// Si la tabla es tramitable y no de auditoría
	// hay que calcular el ordinal entre la tabla tramitable y la de tramitación
	if( !m_pMtLgcTbl->m_bAudStmt && m_pMtLgcTbl->m_iMirrorType == 1 )
	{
		stFields.reserve( 200 ) ;
		stView.reserve( 500 ) ;
		stSelect.reserve( 1000 ) ;

		stFields = "";

		m4bool_t bFirst = M4_TRUE;
		m4int32_t numPar = m_oVParam.size ();

		for (indPar=0; indPar<numPar; indPar++)
		{
			indFld = m_oVParam[indPar];
			m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

			/* Bug 0266748
			Las fechas de inicio no se excluyen
			*/
			if (indFld == ai_indParam || (bIsPK && indFld != m_pMtLgcTbl->m_indDataSet))
			{
				// Bug 0302129 Se excluyen las fechas si se especifica
				if (ai_bExcludeStartDates == M4_TRUE && bIsPK && (indFld == m_pMtLgcTbl->m_indDTStart || indFld == m_pMtLgcTbl->m_indDTStartCorr))
				{
					continue;
				}

				// Es PK y no es ni el dataset o es el ordinal y no es ni una de las fechas si se excluyen
				if (!bFirst)
				{
					stFields.append (", ");
				}
				else
				{
					bFirst = M4_FALSE;
				}

				m4pcchar_t pRealField = m_pParam->GetInfoRealField (indFld);
				stFields.append (pRealField);
			}
		}

		m4pcchar_t pRealObject = M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_REAL_OBJECT].Value.GetWithOutSec ());
		m4pcchar_t pMirrorRealObject = M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_MIRROR_REAL_OBJECT].Value.GetWithOutSec ());

		stView = "(SELECT " ;
		stView.append (stFields);
		stView.append (" FROM ");
		stView.append (pRealObject);
		stView.append (" UNION ALL SELECT ");
		stView.append (stFields);
		stView.append (" FROM ");
		stView.append (pMirrorRealObject);
		stView.append (")");

		pRealTable = stView.c_str();
	}

	stSelect = "SELECT MAX(T.";
	stSelect.append (pRealField);
	stSelect.append (") FROM ");
	stSelect.append (pRealTable);
	stSelect.append (" T WHERE ");

	if (!m_pMtLgcTbl->m_bAudStmt)
	{
		m4bool_t bFirst = M4_TRUE;
		m4int32_t numPar = m_oVParam.size ();

		for (indPar=0; indPar<numPar; indPar++)
		{
			indFld = m_oVParam[indPar];
			m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

			/* Bug 0266748
			Las fechas de inicio no se excluyen
			*/
			if (bIsPK && indFld != ai_indParam && indFld != m_pMtLgcTbl->m_indDataSet)
			{
				// Bug 0302129 Se excluyen las fechas si se especifica
				if (ai_bExcludeStartDates == M4_TRUE && (indFld == m_pMtLgcTbl->m_indDTStart || indFld == m_pMtLgcTbl->m_indDTStartCorr))
				{
					continue;
				}

				// Es PK y no es ni el dataset o es el ordinal y no es ni una de las fechas si se excluyen

				/* Bug 0173558
				En herencia la organización se ignora para el cálculo de ordinal
				*/
				if( m_pMtLgcTbl->m_bInheritsData == M4_TRUE && m_pParam->GetInfoIdInternalField( indFld ) == M4LDB_ID_ORGANIZATION )
				{
					continue;
				}

				if (!bFirst)
				{
					stSelect.append (" AND ");
				}
				else
				{
					bFirst = M4_FALSE;
				}

				m4pcchar_t pRealField = m_pParam->GetInfoRealField (indFld);
				stSelect.append ("T.");
				stSelect.append (pRealField);
				stSelect.append (" = ?");
			}
		}

		if (bFirst)
		{
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_NO_PK_FIELDS, m_pMtLgcTbl->m_idLgcTbl);
			return (M4_ERROR);
		}
	}
	else
	{
		m4int32_t indIdExec = m_pParam->FindIndexByIdInternalField (M4LDB_ID_EXECUTION);

		if (indIdExec == M4LDB_NULL_INDEX)
		{
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, M4LDB_ID_EXECUTION << m_pMtLgcTbl->m_idLgcTbl);
			return (M4_ERROR);
		}

		m4pcchar_t pRealField = m_pParam->GetInfoRealField (indIdExec);
		stSelect.append ("T.");
		stSelect.append (pRealField);
		stSelect.append (" = ?");
	}

	ClLoad *pCursor = new ClLoad (m_pGlobVars, M4_TRUE, this);
	result = pCursor->Open (m_idRConn, NULL, M4_FALSE, m_pParam);

	if (result == M4_ERROR)
	{
		delete pCursor;
		return (M4_ERROR);
	}

	result = pCursor->Prepare ((m4pchar_t) stSelect.c_str ());

	if (result == M4_ERROR)
	{
		delete pCursor;
		return (M4_ERROR);
	}

	// UNIOCODE DB
	m_pcOrdinal = (m4pchar_t) &m_dOrdinal;
	m_iOrdinalMaxLen = M4LDB_SIZE_DOUBLE;

	result = pCursor->BindResult (1, &m_pcOrdinal, &m_iOrdinalMaxLen, M4CL_M4_TYPE_NUMBER, &m_lOrdinal);

	if (result == M4_ERROR)
	{
		delete pCursor;
		return (M4_ERROR);
	}

	m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_CURSOR_ORDINAL_PK + ai_indParam, pCursor));

	return (result);
}


m4return_t ClPersistBase::PrepareValidatePK (void)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;
	m4bool_t bVerifyData = M4_TRUE;
	m4int32_t numCol = 0;
	ClParam* pParamTramitDTLastUPdate = NULL;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Validate Primary Key.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Validate Primary Key.");
	#endif

	string stSelect ("SELECT ");

	m4bool_t bFirst = M4_TRUE;
	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];

		// Comprobar que no es ni LONG ni BLOB
		m4int8_t m4type = m_pParam->GetInfoM4Type (indFld);
		
		if ((m4type == M4CL_M4_TYPE_LONG ||
			 m4type == M4CL_M4_TYPE_UNI_LONG ||
			 m4type == M4CL_M4_TYPE_BLOB ||
			 m4type == M4CL_M4_TYPE_BINARY_STRING) && 
			!m_pMtLgcTbl->m_bHasAudit && 
			 m_eStmType != M4LDB_DUPLICATE && 
			(m_eStmType == M4LDB_DELETE || 
			 m_pParam->GetCurrentLanguageIndex (indFld) == M4LDB_NULL_INDEX))
		{
			// Si no tiene auditoria y no es DUPLICATE los campos LONG no son necesarios,
			// salvo para el campo del lenguaje por defecto.

			/* Bug 0155070
			Salvo que sea update con tramitación o auditoría
			*/
			m4bool_t bContinue = M4_TRUE ;

			if( m_eStmType == M4LDB_UPDATE )
			{
				if(
					( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
					||
					( m_bUseInheritance == M4_TRUE && m_pMtLgcTbl->m_bInheritsData == M4_TRUE )
					)
				{
					bContinue = M4_FALSE ;
				}
			}

			if( bContinue == M4_TRUE )
			{
				continue;
			}
		}
		
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);
		
		if (!bIsPK)
		{
			if (!bFirst)
			{
				stSelect.append (", ");
			}
			else
			{
				bFirst = M4_FALSE;
				bVerifyData = M4_FALSE;
			}
			
			m4pcchar_t pRealField = m_pParam->GetInfoRealField (indFld);
			
			stSelect.append ("T.");
			stSelect.append (pRealField);

			numCol++;

			/* Bug 0166440
			En tramitación hay que leer tanmbién la fecha del registro original
			*/
			if( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
			{
				if( m_pParam->GetInfoIdInternalField( indFld ) == M4LDB_DT_LAST_UPDATE )
				{
					stSelect.append (", T.TRAMIT_DT_LAST_UPDATE");
					numCol++;
				}
			}
		}
	}

	if (bVerifyData)
	{
		stSelect.append ("1");
	}

	stSelect.append (" FROM ");

	m4bool_t	bAddForUpdate = M4_TRUE ;
	ClItemIndex iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_REAL_OBJECT ;

	if( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
	{
		// Si es de traducción se usa la tabla de tramitación
		iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_MIRROR_REAL_OBJECT ;
	}

	m4uint32_t iIsTranslation = 0;

	if( m_indRealTbl != 0 )
	{
		iIsTranslation = M4LDB_NUMBER( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_IS_TRANSLATION].Value.GetWithOutSec() );
	}

	if( iIsTranslation == 0 )
	{
		if( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
		{
			// En tablas con tramitación y sin traduccion se debe utilizar la vista
			bAddForUpdate = M4_FALSE ;
			iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_TRAMIT_VIEW ;
		}

		// En tablas con herencia y sin traduccion se debe utilizar la vista
		if( m_bUseInheritance == M4_TRUE && m_pMtLgcTbl->m_bInheritsData == M4_TRUE )
		{
			bAddForUpdate = M4_FALSE ;
			iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_INHERIT_VIEW ;
		}
	}

	stSelect.append (M4LDB_PSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[iTableName].Value.GetWithOutSec ()));
	stSelect.append (" T");

	// Obtenemos el tipo de base de datos.
	ClLConn *pLConn = m_pGlobVars->GetpLConn ();
	eDDBBType_t eDDBB = M4DBW_ORACLE_DDBB;
	
	if (pLConn->GetConnManager()->GetDDBBType(m_idRConn,pLConn,eDDBB)!=M4_SUCCESS){
		return M4_ERROR;
	}

	_ClLockerTranslator *pLock = ClTraductor::s_ppTrUtils[eDDBB-1]->m_pLockTrans;

	// En la vista no se puede hacer un for update
	if( bAddForUpdate == M4_TRUE )
	{
		pLock->AddLockBetweenJoins(stSelect);
	}

	stSelect.append (" WHERE ");

	if( m_eStmType == M4LDB_UPDATE && m_bUseTranslation == M4_TRUE && iIsTranslation == 1 )
	{
		// Si es de traducciones hay que filtrar por el lenguaje en update, pero no en delete
		m4char_t acBuffer[ 128 + 1 ];
		sprintf(acBuffer, "T.ID_LANGUAGE# = %d AND ", m_pGlobVars->GetLanguage()); 
		stSelect.append (acBuffer);
	}

	bFirst = M4_TRUE;
	m4bool_t bExistsPK = M4_FALSE;

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

		if (bIsPK)
		{
			// Es PK.

			if (indFld != m_pMtLgcTbl->m_indDTStart && indFld != m_pMtLgcTbl->m_indDTStartCorr)
			{
				bExistsPK = M4_TRUE;
			}

			if (!bFirst)
			{
				stSelect.append (" AND ");
			}
			else
			{
				bFirst = M4_FALSE;
			}

			m4pcchar_t pRealField = m_pParam->GetInfoRealField (indFld);

			stSelect.append ("T.");
			stSelect.append (pRealField);
			stSelect.append (" = ?");
		}
	}

	if (!bExistsPK) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_NO_PK_FIELDS, m_pMtLgcTbl->m_idLgcTbl);
		return (M4_ERROR);
	}

	// En la vista no se puede hacer un for update
	if( bAddForUpdate == M4_TRUE )
	{
		pLock->AddLockForUpdate(stSelect);
	}

	ClLoad *pCursor = new ClLoad (m_pGlobVars, M4_TRUE, this);
	result = pCursor->Open (m_idRConn, NULL, M4_FALSE, m_pParam);

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	result = pCursor->Prepare ((m4pchar_t) stSelect.c_str ());

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	if (!bVerifyData)
	{
		pCursor->m_pColumns = new ClDinArr (m_pGlobVars);
		pCursor->m_pColumns->Init (numCol, M4_TRUE);
		pCursor->m_pColumns->Attach ();

		m4int32_t indCol = 0;

		for (indPar=0; indPar<numPar; indPar++)
		{
			indFld = m_oVParam[indPar];

			// Comprobar que no es ni LONG ni BLOB
			m4int8_t m4type = m_pParam->GetInfoM4Type (indFld);
				
			if ((m4type == M4CL_M4_TYPE_LONG ||
				 m4type == M4CL_M4_TYPE_UNI_LONG ||
				 m4type == M4CL_M4_TYPE_BLOB ||
				 m4type == M4CL_M4_TYPE_BINARY_STRING) && 
				!m_pMtLgcTbl->m_bHasAudit && 
				 m_eStmType != M4LDB_DUPLICATE && 
				(m_eStmType == M4LDB_DELETE || 
				 m_pParam->GetCurrentLanguageIndex (indFld) == M4LDB_NULL_INDEX))
			{
				// Si no tiene auditoria y no es DUPLICATE los campos LONG no son necesarios,
				// salvo para el campo del lenguaje por defecto.

				/* Bug 0155070
				Salvo que sea update con tramitación o auditoría
				*/
				m4bool_t bContinue = M4_TRUE ;

				if( m_eStmType == M4LDB_UPDATE )
				{
					if(
						( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
						||
						( m_bUseInheritance == M4_TRUE && m_pMtLgcTbl->m_bInheritsData == M4_TRUE )
						)
					{
						bContinue = M4_FALSE ;
					}
				}

				if( bContinue == M4_TRUE )
				{
					continue;
				}
			}
			
			m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);
			
			if (!bIsPK)
			{
				result = pCursor->m_pColumns->CreateData (indCol, m_pParam->GetRefData (indFld));

				if (result == M4_ERROR) {
					delete pCursor;
					return (M4_ERROR);
				}

				// UNICODE DB
				result = pCursor->BindResult (indCol + 1, pCursor->m_pColumns->GetDataPointerPointer(indCol), pCursor->m_pColumns->GetMaxLenPointer(indCol), pCursor->m_pColumns->GetInfoM4Type (indCol), pCursor->m_pColumns->GetLengthPointer (indCol));

				if (result == M4_ERROR) {
					delete pCursor;
					return (M4_ERROR);
				}

				indCol++;

				/* Bug 0166440
				La fecha TRAMIT_DT_LAST_UPDATE se pone en el parámatro especial
				*/
				if( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
				{
					if( m_pParam->GetInfoIdInternalField( indFld ) == M4LDB_DT_LAST_UPDATE )
					{
						pParamTramitDTLastUPdate = GetClParamByType( M4LDB_TRAMIT_DT_LAST_UPDATE ) ;

						result = pCursor->m_pColumns->CreateData (indCol, NULL, M4LDB_INTERNAL_HANDLE, M4CL_M4_TYPE_TIMESTAMP, M4LDB_SIZE_TIMESTAMP, 0, pParamTramitDTLastUPdate->m_pData, NULL, M4LDB_PAR_INTERN, M4_FALSE);

						if (result == M4_ERROR) {
							delete pCursor;
							return (M4_ERROR);
						}

						result = pCursor->BindResult (indCol + 1, pCursor->m_pColumns->GetDataPointerPointer(indCol), pCursor->m_pColumns->GetMaxLenPointer(indCol), pCursor->m_pColumns->GetInfoM4Type (indCol), pCursor->m_pColumns->GetLengthPointer (indCol));

						if (result == M4_ERROR) {
							delete pCursor;
							return (M4_ERROR);
						}

						indCol++;
					}
				}
			}
		}
	}

	m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_VALIDATE_PK, pCursor));

	return (result);
}


m4return_t ClPersistBase::PrepareValidateNoPK (void)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Validate No Primary Key.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Validate No Primary Key.");
	#endif

	ClLConn *pLConn = m_pGlobVars->GetpLConn ();
	eDDBBType_t eDDBB = M4DBW_ORACLE_DDBB;
	
	if (pLConn->GetConnManager()->GetDDBBType(m_idRConn,pLConn,eDDBB)!=M4_SUCCESS){
		return M4_ERROR;
	}

	_ClLockerTranslator *pLock = ClTraductor::s_ppTrUtils[eDDBB-1]->m_pLockTrans;

	string stSelect ("SELECT ");

	if (m_pMtLgcTbl->m_indRegType != M4LDB_NULL_INDEX && !m_indRealTbl)
	{
		m4pcchar_t pRealProjection = m_pParam->GetInfoRealField (m_pMtLgcTbl->m_indRegType);

		stSelect.append (pRealProjection);
	}
	else
	{
		stSelect.append ("1");
	}		

	stSelect.append (" FROM ");


	m4bool_t	bAddForUpdate = M4_TRUE ;

	// Se calcula el tipo de canal para la herencia de datos y la tramitación
	ClItemIndex iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_REAL_OBJECT ;

	if( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
	{
		// Si es de traducción se usa la tabla de tramitación
		iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_MIRROR_REAL_OBJECT ;
	}

	m4uint32_t iIsTranslation = 0;

	if( m_indRealTbl != 0 )
	{
		iIsTranslation = M4LDB_NUMBER( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_IS_TRANSLATION].Value.GetWithOutSec() );
	}

	if( iIsTranslation == 0 )
	{
		if( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
		{
			// En tablas con tramitación y sin traduccion se debe utilizar la vista
			bAddForUpdate = M4_FALSE ;
			iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_TRAMIT_VIEW ;
		}

		// En tablas con herencia y sin traduccion se debe utilizar la vista
		if( m_bUseInheritance == M4_TRUE && m_pMtLgcTbl->m_bInheritsData == M4_TRUE )
		{
			bAddForUpdate = M4_FALSE ;
			iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_INHERIT_VIEW ;
		}
	}

	stSelect.append (M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[iTableName].Value.GetWithOutSec ()));
	stSelect.append (" T");

	if	(m_pMtLgcTbl->m_indRegType != M4LDB_NULL_INDEX && !m_indRealTbl) 
	{
		// En la vista no se puede hacer un for update
		if( bAddForUpdate == M4_TRUE )
		{
			pLock->AddLockBetweenJoins(stSelect);
		}
	}

	stSelect.append (" WHERE ");

	if (!m_pMtLgcTbl->m_bAudStmt)
	{
		m4bool_t bFirst = M4_TRUE;
		m4bool_t bExistsPK = M4_FALSE;
		m4int32_t numPar = m_oVParam.size ();

		for (indPar=0; indPar<numPar; indPar++)
		{
			indFld = m_oVParam[indPar];
			m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

			if (bIsPK)
			{
				// Es PK.
				if (indFld != m_pMtLgcTbl->m_indDTStart && indFld != m_pMtLgcTbl->m_indDTStartCorr)
				{
					bExistsPK = M4_TRUE;
				}

				if (!bFirst)
				{
					stSelect.append (" AND ");
				}
				else
				{
					bFirst = M4_FALSE;
				}

				m4pcchar_t pRealField = m_pParam->GetInfoRealField (indFld);

				stSelect.append ("T.");
				stSelect.append (pRealField);
				stSelect.append (" = ?");
			}
		}

		if (!bExistsPK) {
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_NO_PK_FIELDS, m_pMtLgcTbl->m_idLgcTbl);
			return (M4_ERROR);
		}

		if	(m_pMtLgcTbl->m_indRegType != M4LDB_NULL_INDEX && !m_indRealTbl)
		{
			// En la vista no se puede hacer un for update
			if( bAddForUpdate == M4_TRUE )
			{
				pLock->AddLockForUpdate(stSelect);
			}
		}
	}
	else
	{
		m4int32_t indIdExec = m_pParam->FindIndexByIdInternalField (M4LDB_ID_EXECUTION);

		if (indIdExec == M4LDB_NULL_INDEX) {
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, M4LDB_ID_EXECUTION << m_pMtLgcTbl->m_idLgcTbl);
			return (M4_ERROR);
		}

		m4pcchar_t pRealField = m_pParam->GetInfoRealField (indIdExec);

		stSelect.append ("T.");
		stSelect.append (pRealField);
		stSelect.append (" = ? AND ");

		m4int32_t indIdOrder = m_pParam->FindIndexByIdInternalField (M4LDB_ID_ORDER);

		if (indIdOrder == M4LDB_NULL_INDEX) {
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, M4LDB_ID_ORDER << m_pMtLgcTbl->m_idLgcTbl);
			return (M4_ERROR);
		}

		pRealField = m_pParam->GetInfoRealField (indIdOrder);

		stSelect.append ("T.");
		stSelect.append (pRealField);
		stSelect.append (" = ?");
	}

	ClLoad *pCursor = new ClLoad (m_pGlobVars, M4_TRUE, this);
	result = pCursor->Open (m_idRConn, NULL, M4_FALSE, m_pParam);

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	result = pCursor->Prepare ((m4pchar_t) stSelect.c_str ());

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	if (m_pMtLgcTbl->m_indRegType != M4LDB_NULL_INDEX && !m_indRealTbl)
	{
		// UNIOCODE DB
		m_pcProjection = (m4pchar_t) &m_dProjection;
		m_iProjectionMaxLen = M4LDB_SIZE_DOUBLE;
		result = pCursor->BindResult (1, &m_pcProjection, &m_iProjectionMaxLen, M4CL_M4_TYPE_NUMBER, &m_lProjection);

		if (result == M4_ERROR) {
			delete pCursor;
			return (M4_ERROR);
		}
	}

	m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_VALIDATE_NO_PK, pCursor));

	return (result);
}


m4return_t ClPersistBase::PrepareValidateFKs (void)
{
	m4return_t result;
	ClRegisterIndex indForReal(0);
	ClRegisterIndex indFor(0);
	ClRegisterIndex indFor2(0);
	ClRegisterIndex indForFld(0);
	ClRegisterIndex indForDTStart(0);
	ClRegisterIndex indForDTEnd(0);
	ClRegisterIndex indForDTStartCorr(0);
	ClRegisterIndex indForDTEndCorr(0);
	ClRegisterIndex indForField(0);
	m4int32_t indForLgcTbl = M4LDB_NULL_INDEX;

	string stSelect (M4LDB_EMPTY);

	m4uint32_t numFor = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Count ();

	for (indFor=0; indFor<numFor; indFor++)
	{
		//Situamos el current.
		m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[m_pMtLgcTbl->m_indLgcTbl];
		m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[m_indRealTbl];

		ClRegister_Current *pCurFor = &(m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current[indFor]);

		m4int32_t indCursor = M4LDB_VALIDATE_FK + indFor;

		m4bool_t bValChild = M4_FALSE;
		m4bool_t bComplete = M4_FALSE;
		m4bool_t bChildRequired = M4_FALSE;

		eRelType_t eRelType = (eRelType_t) M4LDB_NUMBER (pCurFor->Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_RELATION_ORDER].Value.GetWithOutSec ());
		eCondValidType_t eAuxCondValid = (eCondValidType_t) M4LDB_NUMBER (pCurFor->Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_CONDUCT_VALID].Value.GetWithOutSec ());
		m4bool_t bAllowPreviousIni = M4LDB_BOOL (pCurFor->Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ALLOW_PREVIOUS_INI].Value.GetWithOutSec ());

		if (eAuxCondValid == M4LDB_VALID_COMPLETENESS)
		{
			bComplete = M4_TRUE;
			bChildRequired = M4LDB_BOOL (pCurFor->Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_CHILD_REQUIRED].Value.GetWithOutSec ());
		}

		if (eRelType == M4LDB_CHILD_RELATION) {
			if (((bComplete) && 
				(bChildRequired)) && 
			    ((m_eStmType == M4LDB_INSERT) || 
			    (m_eStmType == M4LDB_DUPLICATE)) && 
				(!m_indRealTbl)) {
				bValChild = M4_TRUE;
			}
			else {
				continue;
			}
		}

		// Busquemos si ya existe dicha validación en la tabla maestra.
		if (m_indRealTbl && m_pMtLgcTbl->m_pMaster->FindAuxCursor (indCursor))
		{
			continue;
		}

		m4id_t idRelation = M4LDB_ID (pCurFor->Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_RELATION].Value.GetWithOutSec ());
		m4id_t idForLgcTbl = M4LDB_ID (pCurFor->Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_FOREIGN_OBJECT].Value.GetWithOutSec ());
		m4uint32_t numForFld = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGN_PARAMS]->Count ();

		if (!numForFld) {
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::PrepareValidateFKs" << "There aren´t foreign fields for the relation = " << LogCat << idRelation);
			return (M4_ERROR);
		}

		m4bool_t bExit = M4_FALSE;

		// Vamos a buscar si los campos existen en la actual tabla real.
		for (indForFld=0; indForFld<numForFld; indForFld++)
		{
			m4id_t idField = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGN_PARAMS]->Register[indForFld].Item[M4LDB_ITEM_SYS_DICT_FOREIGN_PARAMS_ID_FIELD].Value.GetWithOutSec ());
			m4int32_t indAuxPar = m_pParam->FindIndexByIdField (idField);

			if (indAuxPar == M4LDB_NULL_INDEX) {
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_LOGIC_FIELD, idField << m_pMtLgcTbl->m_idLgcTbl);
				return (M4_ERROR);
			}

			if (find (m_oVParam.begin (), m_oVParam.end (), indAuxPar) == m_oVParam.end ()) {
				bExit = M4_TRUE;
				break;
			}
		}

		if (bExit) {
			continue;
		}

		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Validate Foreign Key [%ld].", (m4int32_t) indFor);
			M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Validate Foreign Key [%ld].", (m4int32_t) indFor);
		#endif

		// Tenemos una foreign en esta tabla real.
		// Ahora vamos a buscar la tabla real foranea.
		// Esto nos situa el logic object.
		result = m_pGlobVars->GetpLConn ()->LoadDictionaryChannel (idForLgcTbl, &indForLgcTbl, M4_TRUE);

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		// A partir de aquí, hacemos un cambio de contexto:
		// Puesto que tenemos que componer la SELECT de la tabla foranea, nos situaremos en ella
		// y utilizaremos como referencia la foreign contraria.
		// Ya sabes:   Una FK en una tabla es una FR en la foranea.

		result = m_pGlobVars->GetpLConn ()->LoadDictionaryChannelForeigns ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		// Buscamos la foreign.
		m4uint32_t numFor2 = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Count ();

		m4bool_t bFound = M4_FALSE;

		// Recorremos en order inverso para encontrar los hijos primero (esto es necesario cuando el padre y el hijo son iguales).
		/* Bug 0200513
		Se cambia la condición de salida
		*/
		for (indFor2=numFor2-1; indFor2 != M4DM_EOF_INDEX && !bFound; indFor2--)
		{
			//Situamos el current.
			pCurFor = &(m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current[indFor2]);

			m4id_t idRelAux = M4LDB_ID (pCurFor->Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_RELATION].Value.GetWithOutSec ());
			
			if (!strcmpi (idRelation, idRelAux))
			{
				// Es la relación.
				m4id_t idLgcTblAux = M4LDB_ID (pCurFor->Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_FOREIGN_OBJECT].Value.GetWithOutSec ());
				
				if (!strcmpi (m_pMtLgcTbl->m_idLgcTbl, idLgcTblAux))
				{
					bFound = M4_TRUE;
				}
			}
		}

		if (!bFound) {
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::PrepareValidateFKs" << "This FK doesn´t have FR.\nLogic object = " << LogCat << m_pMtLgcTbl->m_idLgcTbl << LogCat << "\nForeign logic object = " << LogCat << idForLgcTbl);
			return (M4_ERROR);
		}

		bFound = M4_FALSE;

		// Ahora buscamos la tabla real de los campos foraneos.
		m4uint32_t numForReal = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Count ();

		m4id_t idForField = NULL;

		//Bug 0072464, 0073442. (Pasamos cuando es insert, update).
		//--- Este código realmente estaba pensado para solucionar 0072464, lo que ocurre es que al no tener que validar
		//todas las FK desaparece el bug 0073442!!!. ---
		//Escenario:
		//Tabla 1P, relacionamos con NP1. (En la relación: Padre es la 1P, hija es NP).
		//Heredamos NP2 de la NP1 (En herencia: Padre es NP1, hija es NP2)
		//El Data Model está añadiendo una relación 1P -- NP2 (En la relación: Padre es 1P, hija es NP2). Esta relación 
		//es la que queremos obviar. El motivo es pq ya existe relación de 1P -> NP (relación) -> NP2 (herencia).
		//Sobra 1P -> NP2 (relación).
		//OJO!!: 1P -> NP2 es relación es de tipo 1 (FK). La relación entre NP1 -> NP2 es de tipo 4 (heredada)!!.
		//SOLO tengo que que desestimar son las que son FK.
		m4bool_t bExistsFKNoPK = M4_FALSE;
		m4bool_t bIsInheritTable = M4_FALSE;
		
		//En este punto, el diccionario está cargado con la información del objeto NP2, por lo tanto puedo
		//preguntarle si tiene ascendentes (es decir, si es heredado).
		//Recordar que en ese nodo sólo se cargan los ascendentes. Por lo tanto, si el count > 0 significa que
		//hay tablas padre!!. Es decir somos una tabla heredada.
		m4int32_t nParents = m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_INHERITANCE]->Current.Count();
		
		//El tipo de relación es hija, y ademas la tabla hija (en la relación) es heredada (hija en herencia) de otra.
		//Además, entre una NP -> NP2 (es herencia) la relaciones de herencia se crean M4LDB_NO_TESTED.
		//Es decir NP1 -> NP2. NP2 cumple que es hija, hay relación de hija y es M4LDB_NO_TESTED. Pero esta tengo que tenerla en cuenta.!!
		//Es decir 1P -> NP2. NP2 Cumple que es hija, hay relación de hija pero es puede ser Completa, Valida Periodo... Esta sí la tengo que desestimar en la validación!!.
		if ( nParents > 0 && eRelType == M4LDB_CHILD_RELATION && eAuxCondValid != M4LDB_NO_TESTED )
		{
			bIsInheritTable = M4_TRUE;
		}
		
		for (indForReal=0; indForReal<numForReal; indForReal++)
		{
		
			bFound = M4_TRUE;

			m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[indForReal];

			for (indForFld=0; indForFld<numForFld; indForFld++)
			{
				idForField = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGN_PARAMS]->Register[indForFld].Item[M4LDB_ITEM_SYS_DICT_FOREIGN_PARAMS_ID_FIELD].Value.GetWithOutSec ());
				indForField = m_pGlobVars->GetpLConn ()->FindIndexById (M4LDB_SYS_DICT_FIELDS, idForField);

				if (indForField == M4LDB_NULL_INDEX) {
					bFound = M4_FALSE;
					break;
				}

				//Bug 0072464, 0073442.
				//Comprobamos si el campo de la relación es PK.
				//Si no es PK es pq la relación es por campos de No PK, lo anotamos para despues comprobar si es por herencia.
				m4bool_t bIsPK = M4_FALSE;

				bIsPK = ((M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Current[indForField].Item[M4LDB_ITEM_SYS_DICT_FIELDS_POS_PK].Value.GetWithOutSec ())) ? M4_TRUE : M4_FALSE)  ;				

				if (bIsPK == M4_FALSE)
				{
					bExistsFKNoPK = M4_TRUE;
				}		
			}

			if (bFound) {
				break;
			}
		}

		if (!bFound) {
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_LOGIC_FIELD, idForField << idForLgcTbl);
			return (M4_ERROR);
		}

		//Bug 0072464, 0073442.
		//Si es una tabla heredada, y todos los campos de la relación son PK, no tenemos en cuenta la FK.
		//Un tanto liadillo pero la cuestion es que si los campos por los que se relacionan son de PK significa que
		//no son de No PK.
		if (bIsInheritTable == M4_TRUE) {
			if (bExistsFKNoPK == M4_FALSE){
				#ifdef _M4LDB_DEBUG
					M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\tValidate FK [%ld] not prepared. Object %s is inherited and Relation %s between parent %s and child %s is not NO_TESTED.", (m4int32_t) indFor, idForLgcTbl, idRelation, m_pMtLgcTbl->m_idLgcTbl, idForLgcTbl);
					M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n\tValidate FK [%ld] not prepared. Object %s is inherited and Relation %s between parent %s and child %s is not NO_TESTED.", (m4int32_t) indFor, idForLgcTbl, idRelation, m_pMtLgcTbl->m_idLgcTbl, idForLgcTbl);
				#endif

				continue;
			}
		}

		m4bool_t bDTApp = M4_FALSE;
		m4bool_t bDTCorr = M4_FALSE;
		m4bool_t bStmVerifyData = M4_FALSE;
		m4pchar_t pForDTStartCorr = NULL;
		m4pchar_t pForDTEndCorr = NULL;

		stSelect = "SELECT ";

		if (!bValChild && !m_indRealTbl)
		{
			if (bComplete)
			{
				indCursor = M4LDB_VALIDATE_COMP;

				// Vamos a buscar si existe la fecha de inicio en la tabla foranea.
				indForDTStart = m_pGlobVars->GetpLConn ()->FindIndexByIdInternalField (M4LDB_DT_START);

				if (indForDTStart == M4LDB_NULL_INDEX) {
					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, M4LDB_DT_START << idForLgcTbl);
					return (M4_ERROR);
				}
				
				stSelect.append ("T.");
				stSelect.append (M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indForDTStart].Item[M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD].Value.GetWithOutSec ()));
				stSelect.append (", ");

				// Y, ahora, la fecha de fin.
				indForDTEnd = m_pGlobVars->GetpLConn ()->FindIndexByIdInternalField (M4LDB_DT_END);

				if (indForDTEnd == M4LDB_NULL_INDEX) {
					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, M4LDB_DT_END << idForLgcTbl);
					return (M4_ERROR);
				}

				stSelect.append ("T.");
				stSelect.append (M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indForDTEnd].Item[M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD].Value.GetWithOutSec ()));

				bDTApp = M4_TRUE;
			}
			else
			{
				eCondValidType_t eCondValid = (eCondValidType_t) M4LDB_NUMBER (pCurFor->Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_CONDUCT_VALID].Value.GetWithOutSec ());

				if (eCondValid != M4LDB_NO_TESTED)
				{
					// Cojemos la temporalidad de la tabla foranea.
					eTempType_t eForTemp = (eTempType_t) M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_TEMPORARY].Value.GetWithOutSec ());

					// Vamos a comprobar las temporalidades.
					if ((m_pMtLgcTbl->m_eCurTemp != M4LDB_ETHERN) &&
						(eForTemp != M4LDB_ETHERN)) {

						// Vamos a buscar si existe la fecha de inicio en la tabla foranea.
						indForDTStart = m_pGlobVars->GetpLConn ()->FindIndexByIdInternalField (M4LDB_DT_START);

						if (indForDTStart == M4LDB_NULL_INDEX) {
							DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, M4LDB_DT_START << idForLgcTbl);
							return (M4_ERROR);
						}
						
						stSelect.append ("T.");
						stSelect.append (M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indForDTStart].Item[M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD].Value.GetWithOutSec ()));
						stSelect.append (", ");

						// Y, ahora, la fecha de fin.
						indForDTEnd = m_pGlobVars->GetpLConn ()->FindIndexByIdInternalField (M4LDB_DT_END);

						if (indForDTEnd == M4LDB_NULL_INDEX) {
							DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, M4LDB_DT_END << idForLgcTbl);
							return (M4_ERROR);
						}

						stSelect.append ("T.");
						stSelect.append (M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indForDTEnd].Item[M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD].Value.GetWithOutSec ()));

						bDTApp = M4_TRUE;
					}
				}

				m4int32_t bCorrected = M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_CORRECTED].Value.GetWithOutSec ());

				if (m_pMtLgcTbl->m_bCorrected && bCorrected)
				{
					// Vamos a buscar si existe la fecha de inicio de corrección en la tabla foranea.
					indForDTStartCorr = m_pGlobVars->GetpLConn ()->FindIndexByIdInternalField (M4LDB_DT_START_CORR);

					if (indForDTStartCorr == M4LDB_NULL_INDEX) {
						DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, M4LDB_DT_START_CORR << idForLgcTbl);
						return (M4_ERROR);
					}
					
					if (bDTApp) {
						stSelect.append (", ");
					}

					pForDTStartCorr = M4LDB_PSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indForDTStartCorr].Item[M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD].Value.GetWithOutSec ());

					stSelect.append ("T.");
					stSelect.append (pForDTStartCorr);
					stSelect.append (", ");

					// Y, ahora, la fecha de fin de corrección.
					indForDTEndCorr = m_pGlobVars->GetpLConn ()->FindIndexByIdInternalField (M4LDB_DT_END_CORR);

					if (indForDTEndCorr == M4LDB_NULL_INDEX) {
						DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, M4LDB_DT_END_CORR << idForLgcTbl);
						return (M4_ERROR);
					}

					pForDTEndCorr = M4LDB_PSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indForDTEndCorr].Item[M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD].Value.GetWithOutSec ());

					stSelect.append ("T.");
					stSelect.append (pForDTEndCorr);
				
					bDTCorr = M4_TRUE;
				}
			}
		}

		m4int32_t idRConn = M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_CONN].Value.GetWithOutSec ());

		ClLoad *pCursor = new ClLoad (m_pGlobVars, M4_TRUE, this);
		result = pCursor->Open (idRConn, NULL, M4_FALSE, m_pParam);

		if (result == M4_ERROR)
		{
			delete pCursor;
			return (M4_ERROR);
		}

		if (!bDTApp && !bDTCorr)
		{
			stSelect.append ("1");
			bStmVerifyData = M4_TRUE;
		}

		stSelect.append (" FROM ");

		ClItemIndex iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_REAL_OBJECT ;

		m4bool_t bForeignUseTramitation = M4_FALSE ;

		// La foreign entre tablas con tramitación va sobre la vista
		if( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
		{
			m4uint8_t iForeignMirrorType = M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_MIRROR_TYPE].Value.GetWithOutSec ());

			if( iForeignMirrorType == 1 )
			{
				bForeignUseTramitation = M4_TRUE ;
				iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_TRAMIT_VIEW ;
			}
		}

		eOrgType_t eVerifyOrg = M4LDB_UNDEF_ORGANIZATION;
		m4bool_t bForeignInheritsData = M4_FALSE ;

		if( m_pMtLgcTbl->m_eCurOrg == M4LDB_MULTI_ORGANIZATION )
		{
			eOrgType_t eForOrg = (eOrgType_t) M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_ORG_TYPE].Value.GetWithOutSec ());

			if( eForOrg == M4LDB_MULTI_ORGANIZATION )
			{
				eVerifyOrg = M4LDB_MULTI_ORGANIZATION;

				if( m_bUseInheritance == M4_TRUE )
				{
					bForeignInheritsData = M4LDB_BOOL (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_INHERITS_DATA].Value.GetWithOutSec ());

					if( bForeignInheritsData == M4_TRUE )
					{
						// Si la tabla foreign es de herencia hay que usar la vista
						iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_INHERIT_VIEW ;
					}
				}
			}
		}
				
		m4pcchar_t pRealObject = M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[iTableName].Value.GetWithOutSec());

		// Si la tabla de foreign es la de sociedad hay que cambiar la vista por la tabla real
		if( strcmpi( pRealObject, "M4VCH_ORGANIZATION" ) == 0 )
		{
			pRealObject = "M4RCH_ORGANIZATION" ;
		}

		stSelect.append (pRealObject);
		stSelect.append (" T");
		stSelect.append (" WHERE ");

		m4bool_t bFirst = M4_TRUE;
		
		for (indForFld=0; indForFld<numForFld; indForFld++)
		{
			idForField = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGN_PARAMS]->Register[indForFld].Item[M4LDB_ITEM_SYS_DICT_FOREIGN_PARAMS_ID_FIELD].Value.GetWithOutSec ());
			indForField = m_pGlobVars->GetpLConn ()->FindIndexById (M4LDB_SYS_DICT_FIELDS, idForField);

			if (indForField == M4LDB_NULL_INDEX) {
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::PrepareValidateFKs" << "Logic field = " << LogCat << idForField << LogCat << " not found in the logic object = " << LogCat << idForLgcTbl);
				delete pCursor;
				return (M4_ERROR);
			}


			eFldType_t eForFldType = (eFldType_t) M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indForField].Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_INTERNAL_FIELD].Value.GetWithOutSec ());

			m4id_t idField = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGN_PARAMS]->Register[indForFld].Item[M4LDB_ITEM_SYS_DICT_FOREIGN_PARAMS_ID_FOREIGN_FIELD].Value.GetWithOutSec ());
			m4int32_t indFld = m_pParam->FindIndexByIdField (idField);

			if (indFld == M4LDB_NULL_INDEX) {
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::PrepareValidateFKs" << "Logic field = " << LogCat << idField << LogCat << " not found in the logic object = " << LogCat << m_pMtLgcTbl->m_idLgcTbl);
				delete pCursor;
				return (M4_ERROR);
			}

			if (eVerifyOrg != M4LDB_MULTI_ORGANIZATION && 
				m_pParam->GetInfoIdInternalField (indFld) == M4LDB_ID_ORGANIZATION && 
				eForFldType == M4LDB_ID_ORGANIZATION && 
				numForFld > 1) {
				// Quitamos la relación de organización si ambas tablas no son multi 
				// y hay mas de un campo relacionado.
				continue;
			}

			eFuncType_t idDefaultFunc = m_pParam->GetInfoIdDefaultFunc (indFld);

			if ((idDefaultFunc == M4LDB_DEF_ORDINAL || idDefaultFunc == M4LDB_DEF_ORDINAL_PK || idDefaultFunc == M4LDB_DEF_ORDINAL_PK_NO_DATES)
				&& !bValChild) {
				// Desactivamos la generación de ordinales para los campos heredados.
				m_pParam->SetInfoIdDefaultFunc (indFld, M4LDB_FTN_NONE);
			}

			if (!pCursor->m_pvcIndParVal) {
				pCursor->m_pvcIndParVal = new VcIndParVal;
				pCursor->m_pvcIndParVal->reserve(10);

			}

			PStIndPar pIndPar = new struct StIndPar;

			pIndPar->m_indPar = indFld;
			pIndPar->m_idForFld = idForField;
			pIndPar->m_bIsPK = M4_TRUE;

			if ((((indFld == m_pMtLgcTbl->m_indDTStart) || 
				(indFld == m_pMtLgcTbl->m_indDTEnd)) && 
				((eForFldType == M4LDB_DT_START) || 
				(eForFldType == M4LDB_DT_END)) &&
				((bDTApp) || 
				(bValChild))) ||
				(((indFld == m_pMtLgcTbl->m_indDTStartCorr) || 
				(indFld == m_pMtLgcTbl->m_indDTEndCorr)) && 
				((eForFldType == M4LDB_DT_START_CORR) || 
				(eForFldType == M4LDB_DT_END_CORR)) &&
				((bDTCorr) || 
				(bValChild)))) {
				pIndPar->m_bIsPK = M4_FALSE;
			}

			pCursor->m_pvcIndParVal->push_back (pIndPar);

			if (!pIndPar->m_bIsPK) {
				continue;
			}

			if (!bFirst) {
				stSelect.append (" AND ");
			}
			else {
				bFirst = M4_FALSE;
			}

			m4pchar_t pRealField = M4LDB_PSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indForField].Item[M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD].Value.GetWithOutSec ());

			stSelect.append ("T.");
			stSelect.append (pRealField);

			// Si la relación es de organización se hacen casos especiales
			if( m_pParam->GetInfoIdInternalField( indFld ) == M4LDB_ID_ORGANIZATION && eVerifyOrg == M4LDB_MULTI_ORGANIZATION && eForFldType == M4LDB_ID_ORGANIZATION )
			{
				if( bForeignInheritsData == M4_FALSE )
				{
					// Si la tabla destino no es de herencia hay que traerse la organización padre
					// del nivel de la tabla destino (con traerse todas las padres sin filtrar por el nivel ya vale)
					stSelect.append( " IN (SELECT OT.ID_PAR_ORGANIZATIO FROM M4VCH_ORG_TREE OT WHERE OT.ID_ORGANIZATION = ?)" ) ;
				}
				else
				{
					stSelect.append( " = ?" ) ;
				}
			}
			else
			{
				stSelect.append( " = ?" ) ;
			}

			if( m_pParam->GetInfoIdInternalField( indFld ) == M4LDB_ID_ORGANIZATION
				&& m_pMtLgcTbl->m_eCurOrg == M4LDB_MULTI_ORGANIZATION
				&& m_pMtLgcTbl->m_bInheritsData == M4_FALSE
				&& strcmpi( pRealObject, "M4RCH_ORGANIZATION" ) == 0 )
			{
				// Si la tabla destino es la de organización hay que filtrar por el nivel
				// para tablas multi sin herencia
				m4char_t acBuffer[ 100 + 1 ] ;

				sprintf( acBuffer, " AND T.ID_LEVEL = %d", m_pMtLgcTbl->m_iOrgLevel ) ;
				stSelect.append( acBuffer ) ;
			}
		}

		// La fk entre tablas con tramitación va sobre la vista y hay que filtrar por el ID_DATASET que no es pk
		if( bForeignUseTramitation == M4_TRUE )
		{
			// Se calcula el ID_DATSET de la tabla origen
			m4int32_t indFld = m_pGlobVars->GetpLConn()->FindIndexByIdInternalField( M4CL_INTERNAL_TYPE_ID_DATASET ) ;

			if( indFld == M4LDB_NULL_INDEX )
			{
				DUMP_LDBINSP_ERRORF( m_pGlobVars->GetpLConn(), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, M4CL_INTERNAL_TYPE_ID_DATASET << idForLgcTbl ) ;
				delete pCursor;
				return( M4_ERROR ) ;
			}

			m4pchar_t pForDataSetId = M4LDB_PSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indFld].Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_FIELD].Value.GetWithOutSec ());
			m4pchar_t pForDataSetName = M4LDB_PSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indFld].Item[M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD].Value.GetWithOutSec ());

			//Situamos el current en la tabla origen
			m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[m_pMtLgcTbl->m_indLgcTbl];
			m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[m_indRealTbl];

			// Se calcula el ID_DATSET de la tabla fk
			indFld = m_pParam->FindIndexByIdInternalField (M4LDB_ID_DATASET);

			if (indFld == M4LDB_NULL_INDEX)
			{
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, M4LDB_ID_DATASET << m_pMtLgcTbl->m_idLgcTbl);
				delete pCursor;
				return (M4_ERROR);
			}

			// Se añade el parámetro
			PStIndPar pIndPar = new struct StIndPar;

			pIndPar->m_indPar = indFld;
			pIndPar->m_idForFld = pForDataSetId;
			pIndPar->m_bIsPK = M4_TRUE;

			pCursor->m_pvcIndParVal->push_back (pIndPar);

			if (!bFirst) {
				stSelect.append (" AND ");
			}
			else {
				bFirst = M4_FALSE;
			}

			stSelect.append ("T.");
			stSelect.append (pForDataSetName);
			stSelect.append( " = ?" ) ;
		}

		if (bDTCorr)
		{
			stSelect.append (" AND ");
			stSelect.append ("T.");
			stSelect.append (pForDTEndCorr);
			stSelect.append (" > ? AND ");
			stSelect.append ("T.");
			stSelect.append (pForDTStartCorr);
			stSelect.append (" < ?");
		}

		result = pCursor->Prepare ((m4pchar_t) stSelect.c_str ());

		if (result == M4_ERROR) {
			delete pCursor;
			return (M4_ERROR);
		}

		pCursor->m_bVerifyData = bStmVerifyData;
		pCursor->m_bVerifyDTApp = bDTApp;
		pCursor->m_bVerifyDTCorr = bDTCorr;
		pCursor->m_eVerifyOrg = eVerifyOrg;
		pCursor->m_bAllowPreviousIni = bAllowPreviousIni;
		pCursor->m_indForLgcTbl = indForLgcTbl;

		if (indCursor == M4LDB_VALIDATE_COMP)
		{
			// UNIOCODE DB
			m_pcCompleteDTStart = (m4pchar_t) &m_dCompleteDTStart;
			m_iCompleteDTStart = M4LDB_SIZE_TIMESTAMP;
			result = pCursor->BindResult (1, &m_pcCompleteDTStart, &m_iCompleteDTStart, M4CL_M4_TYPE_TIMESTAMP, &m_lCompleteDTStart);

			if (result == M4_ERROR) {
				delete pCursor;
				return (M4_ERROR);
			}

			// UNIOCODE DB
			m_pcCompleteDTEnd = (m4pchar_t) &m_dCompleteDTEnd;
			m_iCompleteDTEnd = M4LDB_SIZE_TIMESTAMP;
			result = pCursor->BindResult (2, &m_pcCompleteDTEnd, &m_iCompleteDTEnd, M4CL_M4_TYPE_TIMESTAMP, &m_lCompleteDTEnd);

			if (result == M4_ERROR) {
				delete pCursor;
				return (M4_ERROR);
			}
		}

		m_pmpValCur->insert (MpPValCursor::value_type (indCursor, pCursor));
	}

	//Situamos el current.
	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[m_pMtLgcTbl->m_indLgcTbl];
	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[m_indRealTbl];

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::PrepareValidateFRs (void)
{
	m4return_t result;
	ClRegisterIndex indForReal(0);
	ClRegisterIndex indFor(0);
	ClRegisterIndex indFor2(0);
	ClRegisterIndex indForFld(0);
	ClRegisterIndex indFld(0);
	ClRegisterIndex indForDTStart(0);
	ClRegisterIndex indForDTEnd(0);
	ClRegisterIndex indForDTStartCorr(0);
	ClRegisterIndex indForDTEndCorr(0);
	ClRegisterIndex indForOperation(0);
	ClRegisterIndex indForRegType(0);
	ClRegisterIndex indForField(0);
	m4int32_t indForLgcTbl = M4LDB_NULL_INDEX;

	map<string, string, less<string> >				mapRelatedFields;
	map<string, string, less<string> >::iterator	itMapRelatedFields;

	string stSelect (M4LDB_EMPTY);

	m4uint32_t numFor = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Count ();

	for (indFor=0; indFor<numFor; indFor++)
	{
		//Situamos el current
		m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[m_pMtLgcTbl->m_indLgcTbl];
		m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[m_indRealTbl];

		ClRegister_Current *pCurFor = &(m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current[indFor]);

		eRelType_t eRelType = (eRelType_t) M4LDB_NUMBER (pCurFor->Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_RELATION_ORDER].Value.GetWithOutSec ());
		m4int32_t indCursor = M4LDB_VALIDATE_FR + indFor;
		m4bool_t bComplete = M4_FALSE;
		eCondValidType_t eAuxCondValid = (eCondValidType_t) M4LDB_NUMBER (pCurFor->Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_CONDUCT_VALID].Value.GetWithOutSec ());
		m4bool_t bAllowPreviousIni = M4LDB_BOOL (pCurFor->Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ALLOW_PREVIOUS_INI].Value.GetWithOutSec ());

		if (eAuxCondValid == M4LDB_VALID_COMPLETENESS)
		{
			bComplete = M4_TRUE;
		}

		if (eRelType == M4LDB_PARENT_RELATION)
		{
			if (bComplete && m_eStmType == M4LDB_DELETE && !m_indRealTbl)
			{
				indCursor = M4LDB_VALIDATE_COMP;
			}
			else
			{
				continue;
			}
		}

		
		// Busquemos si ya existe dicha validación en la tabla maestra.
		if (m_indRealTbl && m_pMtLgcTbl->m_pMaster->FindAuxCursor (indCursor))
		{
			continue;
		}

		m4id_t idRelation = M4LDB_ID (pCurFor->Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_RELATION].Value.GetWithOutSec ());
		m4id_t idForLgcTbl = M4LDB_ID (pCurFor->Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_FOREIGN_OBJECT].Value.GetWithOutSec ());
		m4uint32_t numForFld = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGN_PARAMS]->Count ();

		if (!numForFld) {
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::PrepareValidateFRs" << "There aren´t foreign fields for the relation = " << LogCat << idRelation);
			return (M4_ERROR);
		}

		m4bool_t bExit = M4_FALSE;

		// Vamos a buscar si los campos existen en la actual tabla real.
		for (indForFld=0; indForFld<numForFld; indForFld++)
		{
			m4id_t idField = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGN_PARAMS]->Register[indForFld].Item[M4LDB_ITEM_SYS_DICT_FOREIGN_PARAMS_ID_FIELD].Value.GetWithOutSec ());
			m4int32_t indAuxPar = m_pParam->FindIndexByIdField (idField);

			if (indAuxPar == M4LDB_NULL_INDEX) {
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_LOGIC_FIELD, idField << m_pMtLgcTbl->m_idLgcTbl);
				return (M4_ERROR);
			}

			if (find (m_oVParam.begin (), m_oVParam.end (), indAuxPar) == m_oVParam.end ())
			{
				bExit = M4_TRUE;
				break;
			}
		}

		if (bExit)
		{
			continue;
		}

		// Tenemos una foreign en esta tabla real.
		// Ahora vamos a buscar la tabla real foranea.
		// Esto nos situa el logic object.
		result = m_pGlobVars->GetpLConn ()->LoadDictionaryChannel (idForLgcTbl, &indForLgcTbl, M4_TRUE);

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		// A partir de aquí, hacemos un cambio de contexto:
		// Puesto que tenemos que componer la SELECT de la tabla foranea, nos situaremos en ella
		// y utilizaremos como referencia la foreign contraria.
		// Ya sabes:   Una FR en una tabla es una FK en la foranea.

		result = m_pGlobVars->GetpLConn ()->LoadDictionaryChannelForeigns ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		/* Bug 0191132
		Las relaciones con vistas se ignoran
		*/
		if( M4LDB_NUMBER( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT_TYPE].Value.GetWithOutSec() ) == 4 )
		{
			continue;
		}

		// Buscamos la foreign.
		m4uint32_t numFor2 = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Count ();

		m4bool_t bFound = M4_FALSE;

		for (indFor2=0; (indFor2<numFor2) && (!bFound); indFor2++)
		{
			//Situamos el current.
			pCurFor = &(m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current[indFor2]);

			m4id_t idRelAux = M4LDB_ID (pCurFor->Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_RELATION].Value.GetWithOutSec ());
			
			if (!strcmpi (idRelation, idRelAux))
			{
				// Es la relación.
				m4id_t idLgcTblAux = M4LDB_ID (pCurFor->Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_FOREIGN_OBJECT].Value.GetWithOutSec ());
				
				if (!strcmpi (m_pMtLgcTbl->m_idLgcTbl, idLgcTblAux))
				{
					bFound = M4_TRUE;
				}
			}
		}

		if (!bFound) {
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::PrepareValidateFRs" << "This FR doesn´t have FK.\nLogic object = " << LogCat << m_pMtLgcTbl->m_idLgcTbl << LogCat << "\nForeign logic object = " << LogCat << idForLgcTbl);
			return (M4_ERROR);
		}


		bFound = M4_FALSE;

		// Ahora buscamos la tabla real de los campos foraneos.
		m4uint32_t numForReal = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Count ();

		m4id_t idForField = NULL;

		//Bug 0072464, 0073442. (Pasamos cuando es delete, update).
		//--- Este código realmente estaba pensado para solucionar 0072464, lo que ocurre es que al no tener que validar
		//todas las FK desaparece el bug 0073442!!!. ---
		//Escenario:
		//Tabla 1P, relacionamos con NP1. (En la relación: Padre es la 1P, hija es NP).
		//Heredamos NP2 de la NP1 (En herencia: Padre es NP1, hija es NP2)
		//El Data Model está añadiendo una relación 1P -- NP2 (En la relación: Padre es 1P, hija es NP2). Esta relación 
		//es la que queremos obviar. El motivo es pq ya existe relación de 1P -> NP (relación) -> NP2 (herencia).
		//Sobra 1P -> NP2 (relación).
		//OJO!!: 1P -> NP2 es relación es de tipo 1 (FK). La relación entre NP1 -> NP2 es de tipo 4 (heredada)!!.
		//SOLO tengo que que desestimar son las que son FK.
		m4bool_t bExistsFKNoPK = M4_FALSE;
		m4bool_t bIsInheritTable = M4_FALSE;

		//En este punto, el diccionario está cargado con la información del objeto NP2, por lo tanto puedo
		//preguntarle si tiene ascendentes (es decir, si es heredado).
		//Recordar que en ese nodo sólo se cargan los ascendentes. Por lo tanto, si el count > 0 significa que
		//hay tablas padre!!. Es decir somos una tabla heredada.
		m4int32_t nParents = m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_INHERITANCE]->Current.Count();
		
		//El tipo de relación es hija, y ademas la tabla hija (en la relación) es heredada (hija en herencia) de otra.
		//Además, entre una NP -> NP2 (es herencia) la relaciones de herencia se crean M4LDB_NO_TESTED.
		//Es decir NP1 -> NP2. NP2 cumple que es hija, hay relación de hija y es M4LDB_NO_TESTED. Pero esta tengo que tenerla en cuenta.!!
		//Es decir 1P -> NP2. NP2 Cumple que es hija, hay relación de hija pero es puede ser Completa, Valida Periodo... Esta sí la tengo que desestimar en la validación!!.
		if ( nParents > 0 && eRelType == M4LDB_CHILD_RELATION && eAuxCondValid != M4LDB_NO_TESTED )
		{
			bIsInheritTable = M4_TRUE;
		}

		for (indForReal=0; indForReal<numForReal; indForReal++)
		{
			bFound = M4_TRUE;

			m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[indForReal];

			for (indForFld=0; indForFld<numForFld; indForFld++)
			{
				idForField = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGN_PARAMS]->Register[indForFld].Item[M4LDB_ITEM_SYS_DICT_FOREIGN_PARAMS_ID_FIELD].Value.GetWithOutSec ());
				indForField = m_pGlobVars->GetpLConn ()->FindIndexById (M4LDB_SYS_DICT_FIELDS, idForField);

				if (indForField == M4LDB_NULL_INDEX) {
					bFound = M4_FALSE;
					break;
				}

				//Bug 0072464, 0073442.
				//Comprobamos si el campo de la relación es PK.
				//Si no es PK es pq la relación es por campos de No PK, lo anotamos para despues comprobar si es por herencia.
				m4bool_t bIsPK = M4_FALSE;

				bIsPK = ((M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Current[indForField].Item[M4LDB_ITEM_SYS_DICT_FIELDS_POS_PK].Value.GetWithOutSec ())) ? M4_TRUE : M4_FALSE)  ;				
				if (bIsPK == M4_FALSE)
				{
					bExistsFKNoPK = M4_TRUE;
				}
			}

			if (bFound)
			{
				break;
			}
		}

		if (!bFound) {
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_LOGIC_FIELD, idForField << idForLgcTbl);
			return (M4_ERROR);
		}

		//Bug 0072464, 0073442.
		//Si es una tabla heredada, y todos los campos de la relación son PK, no tenemos en cuenta la FK.
		//Un tanto liadillo pero la cuestion es que si los campos por los que se relacionan son de PK significa que
		//no son de No PK.
		if (bIsInheritTable == M4_TRUE)
		{
			if (bExistsFKNoPK == M4_FALSE)
			{
				continue;
			}
		}
		
		m4bool_t bDTApp = M4_FALSE;
		m4bool_t bDTCorr = M4_FALSE;
		m4bool_t bOperation = M4_FALSE;
		m4bool_t bStmVerifyData = M4_FALSE;
		eOrgType_t eVerifyOrg = M4LDB_UNDEF_ORGANIZATION;
		m4pchar_t pForDTStart = NULL;
		m4pchar_t pForDTEnd = NULL;
		m4pchar_t pForDTStartCorr = NULL;
		m4pchar_t pForDTEndCorr = NULL;
		m4pchar_t pForProjection = NULL;
		m4pchar_t pForOperation = NULL;
		m4bool_t bFirst = M4_TRUE;

		//Bug 0077123.
		//Informix. No acepta ORDER BY c1 FOR UPDATE.
		//Si a la sentencia de validacion le añadimos Order by lo tenemos que tener en cuenta.
		m4bool_t bWithOrderBy = M4_FALSE;

		m4bool_t bForeignUseTramitation = M4_FALSE ;

		if( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
		{
			m4uint8_t iForeignMirrorType = M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_MIRROR_TYPE].Value.GetWithOutSec ());

			if( iForeignMirrorType == 1 )
			{
				bForeignUseTramitation = M4_TRUE ;
			}
		}

		m4bool_t bForeignInheritsData = M4_FALSE ;

		if (m_pMtLgcTbl->m_eCurOrg == M4LDB_MULTI_ORGANIZATION)
		{
			eOrgType_t eForOrg = (eOrgType_t) M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_ORG_TYPE].Value.GetWithOutSec ());

			if (eForOrg == M4LDB_MULTI_ORGANIZATION)
			{
				eVerifyOrg = m_pMtLgcTbl->m_eCurOrg;

				if( m_bUseInheritance == M4_TRUE )
				{
					bForeignInheritsData = M4LDB_BOOL (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_INHERITS_DATA].Value.GetWithOutSec ());
				}
			}
		}

		stSelect = "SELECT ";

		if (!m_indRealTbl)
		{
			eCondValidType_t eCondValid = (eCondValidType_t) M4LDB_NUMBER (pCurFor->Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_CONDUCT_VALID].Value.GetWithOutSec ());

			if (indCursor == M4LDB_VALIDATE_COMP)
			{
				// Vamos a buscar si existe la fecha de inicio en la tabla foranea.
				indForDTStart = m_pGlobVars->GetpLConn ()->FindIndexByIdInternalField (M4LDB_DT_START);

				if (indForDTStart == M4LDB_NULL_INDEX)
				{
					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, M4LDB_DT_START << idForLgcTbl);
					return (M4_ERROR);
				}

				pForDTStart = M4LDB_PSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indForDTStart].Item[M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD].Value.GetWithOutSec ());

				stSelect.append ("T.");
				stSelect.append (pForDTStart);
				stSelect.append (", ");

				// Y, ahora, la fecha de fin.
				indForDTEnd = m_pGlobVars->GetpLConn ()->FindIndexByIdInternalField (M4LDB_DT_END);

				if (indForDTEnd == M4LDB_NULL_INDEX) {
					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, M4LDB_DT_END << idForLgcTbl);
					return (M4_ERROR);
				}

				pForDTEnd = M4LDB_PSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indForDTEnd].Item[M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD].Value.GetWithOutSec ());

				stSelect.append ("T.");
				stSelect.append (pForDTEnd);

				bDTApp = M4_TRUE;
			}
			else
			{
				if (eCondValid != M4LDB_NO_TESTED)
				{
					eTempType_t eForTemp = (eTempType_t) M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_TEMPORARY].Value.GetWithOutSec ());

					// Vamos a comprobar las temporalidades.
					if (m_pMtLgcTbl->m_eCurTemp != M4LDB_ETHERN && eForTemp != M4LDB_ETHERN)
					{
						// Vamos a buscar si existe la fecha de inicio en la tabla foranea.
						indForDTStart = m_pGlobVars->GetpLConn ()->FindIndexByIdInternalField (M4LDB_DT_START);

						if (indForDTStart == M4LDB_NULL_INDEX) {
							DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, M4LDB_DT_START << idForLgcTbl);
							return (M4_ERROR);
						}

						pForDTStart = M4LDB_PSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indForDTStart].Item[M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD].Value.GetWithOutSec ());

						stSelect.append ("T.");
						stSelect.append (pForDTStart);
						stSelect.append (", ");


						// Y, ahora, la fecha de fin.
						indForDTEnd = m_pGlobVars->GetpLConn ()->FindIndexByIdInternalField (M4LDB_DT_END);

						if (indForDTEnd == M4LDB_NULL_INDEX) {
							DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, M4LDB_DT_END << idForLgcTbl);
							return (M4_ERROR);
						}

						pForDTEnd = M4LDB_PSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indForDTEnd].Item[M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD].Value.GetWithOutSec ());

						stSelect.append ("T.");
						stSelect.append (pForDTEnd);

						bDTApp = M4_TRUE;
					}
				}

				m4int32_t bCorrected = M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_CORRECTED].Value.GetWithOutSec ());

				if (m_pMtLgcTbl->m_bCorrected && bCorrected)
				{
					// Vamos a buscar si existe la fecha de inicio de corrección en la tabla foranea.
					indForDTStartCorr = m_pGlobVars->GetpLConn ()->FindIndexByIdInternalField (M4LDB_DT_START_CORR);

					if (indForDTStartCorr == M4LDB_NULL_INDEX)
					{
						DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, M4LDB_DT_START_CORR << idForLgcTbl);
						return (M4_ERROR);
					}
					
					if (bDTApp)
					{
						stSelect.append (", ");
					}

					pForDTStartCorr = M4LDB_PSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indForDTStartCorr].Item[M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD].Value.GetWithOutSec ());

					stSelect.append ("T.");
					stSelect.append (pForDTStartCorr);
					stSelect.append (", ");

					
					// Y, ahora, la fecha de fin de corrección.
					indForDTEndCorr = m_pGlobVars->GetpLConn ()->FindIndexByIdInternalField (M4LDB_DT_END_CORR);

					if (indForDTEndCorr == M4LDB_NULL_INDEX)
					{
						DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, M4LDB_DT_END_CORR << idForLgcTbl);
						return (M4_ERROR);
					}

					pForDTEndCorr = M4LDB_PSTR (M4LDB_PSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indForDTEndCorr].Item[M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD].Value.GetWithOutSec ())); 

					stSelect.append ("T.");
					stSelect.append (pForDTEndCorr);

					indForRegType = m_pGlobVars->GetpLConn ()->FindIndexByIdInternalField (M4LDB_TYPE_REG);

					if (indForRegType != M4LDB_NULL_INDEX)
					{
						pForProjection = M4LDB_PSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indForRegType].Item[M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD].Value.GetWithOutSec ());
					}

					bDTCorr = M4_TRUE;
				}
				else
				{
					if (m_eStmType == M4LDB_INSERT || m_eStmType == M4LDB_DUPLICATE)
					{
						continue;
					}
				}
			}

			if (bDTApp || bDTCorr)
			{
				bFirst = M4_FALSE;
			}

			// Si la tabla destino es de herencia y la origen no, hay que traer la operación 
			if( bForeignInheritsData == M4_TRUE && m_pMtLgcTbl->m_bInheritsData == M4_FALSE )
			{
				// Vamos a buscar si existe la operación en la tabla foranea.
				indForOperation = m_pGlobVars->GetpLConn()->FindIndexByIdInternalField( M4LDB_ID_INHERIT_OPERATION ) ;

				if( indForOperation == M4LDB_NULL_INDEX )
				{
					DUMP_LDBINSP_ERRORF( m_pGlobVars->GetpLConn(), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, M4LDB_ID_INHERIT_OPERATION << idForLgcTbl ) ;
					return( M4_ERROR ) ;
				}

				if( bFirst == M4_FALSE )
				{
					stSelect.append( ", " ) ;
				}
				else
				{
					bFirst = M4_FALSE ;
				}

				pForOperation = M4LDB_PSTR( m_pGlobVars->GetpLConn()->m_paRS[ M4LDB_SYS_DICT_FIELDS ]->Register[ indForOperation ].Item[ M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD ].Value.GetWithOutSec() ) ;

				stSelect.append( "T." ) ;
				stSelect.append( pForOperation ) ;

				bOperation = M4_TRUE ;
			}


// A partir de ahora, siempre necesitamos la PK en el delete para poder utilizar la función FindPersistInPost.
//			m4bool_t bDeleteOnCascade = M4LDB_BOOL (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current.Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_DELETE_CASCADE].Value.GetWithOutSec ());

			m4bool_t bNeedsPK = M4_FALSE;

			// Esto es para el cierre en cascada y el borrado en cascada.
			m4bool_t bDTStartCascade = M4LDB_BOOL (pCurFor->Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_DT_START_CASCADE].Value.GetWithOutSec ());

			eMoveDTEndOnCascType_t eMoveDTEndOnCasc = (eMoveDTEndOnCascType_t) M4LDB_NUMBER (pCurFor->Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_DT_END_CASCADE].Value.GetWithOutSec ());

			if (((bDTStartCascade || 
				eMoveDTEndOnCasc != M4LDB_NO_MOVE_DT_END_ON_CASC) && 
				bDTApp &&
				m_eStmType == M4LDB_UPDATE) ||
//				(bDeleteOnCascade && 
				m_eStmType == M4LDB_DELETE || 
				m_pMtLgcTbl->m_bUpdatePK)
			{
				bNeedsPK = M4_TRUE;
			}

			if (bNeedsPK)
			{
				m4uint32_t numFld = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Count ();

				for (indFld=0; indFld<numFld; indFld++)
				{
					m4bool_t bIsPK = M4LDB_BOOL (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indFld].Item[M4LDB_ITEM_SYS_DICT_FIELDS_POS_PK].Value.GetWithOutSec ());

					if( !bIsPK && bForeignUseTramitation == M4_TRUE )
					{
						eFldType_t eForFldType = (eFldType_t) M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indFld].Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_INTERNAL_FIELD].Value.GetWithOutSec ());

						// Si es el ID_DATASET también es PK en tramitación
						if( eForFldType == M4CL_INTERNAL_TYPE_ID_DATASET )
						{
							bIsPK = M4_TRUE ;
						}
					}

					if (bIsPK)
					{
						// Es PK.
						if (!bFirst)
						{
							stSelect.append (", ");
						}
						else
						{
							bFirst = M4_FALSE;
						}

						m4pcchar_t pRealField = M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indFld].Item[M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD].Value.GetWithOutSec ());

						stSelect.append ("T.");
						stSelect.append (pRealField);
					}
				}
			}
		}

		if (bFirst && m_eStmType == M4LDB_UPDATE)
		{
			// No existen columnas.
			continue;
		}

		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Validate Foreign Reference [%ld].", (m4int32_t) indFor);
		#endif

		m4int32_t idRConn = M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_CONN].Value.GetWithOutSec ());

		ClLoad *pCursor = new ClLoad (m_pGlobVars, M4_TRUE, this);
		result = pCursor->Open (idRConn, NULL, M4_FALSE, m_pParam);

		if (result == M4_ERROR) {
			delete pCursor;
			return (M4_ERROR);
		}

		if (bFirst)
		{
			stSelect.append ("1");
			bStmVerifyData = M4_TRUE;
		}

		stSelect.append (" FROM ");

		m4bool_t bAddForUpdate = M4_TRUE ;
		ClItemIndex iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_REAL_OBJECT ;

		// La fr entre tablas con tramitación va sobre la vista
		if( bForeignUseTramitation == M4_TRUE )
		{
			bAddForUpdate = M4_FALSE ;
			iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_TRAMIT_VIEW ;
		}

		// La fr entre tablas con herencia va sobre la vista
		if( bForeignInheritsData == M4_TRUE && m_pMtLgcTbl->m_bInheritsData == M4_TRUE )
		{
			bAddForUpdate = M4_FALSE ;
			iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_INHERIT_VIEW ;
		}

		m4pcchar_t pRealObject = M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[iTableName].Value.GetWithOutSec());

		// Si la tabla de foreign es la de sociedad hay que cambiar la vista por la tabla real
		if( strcmpi( pRealObject, "M4VCH_ORGANIZATION" ) == 0 )
		{
			pRealObject = "M4RCH_ORGANIZATION" ;
		}

		stSelect.append (pRealObject);
		stSelect.append(" T");

		ClLConn *pLConn = m_pGlobVars->GetpLConn ();
		eDDBBType_t eDDBB = M4DBW_ORACLE_DDBB;
		
		if (pLConn->GetConnManager()->GetDDBBType(idRConn,pLConn,eDDBB)!=M4_SUCCESS){
			delete pCursor;
			return M4_ERROR;
		}

		_ClLockerTranslator *pLock = ClTraductor::s_ppTrUtils[eDDBB-1]->m_pLockTrans;

		// En la vista no se puede hacer un for update
		if( bAddForUpdate == M4_TRUE )
		{
			pLock->AddLockBetweenJoins(stSelect);
		}

		stSelect.append (" WHERE ");

		bFirst = M4_TRUE;
		mapRelatedFields.clear();

		for (indForFld=0; indForFld<numForFld; indForFld++)
		{
			idForField = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGN_PARAMS]->Register[indForFld].Item[M4LDB_ITEM_SYS_DICT_FOREIGN_PARAMS_ID_FIELD].Value.GetWithOutSec ());
			indForField = m_pGlobVars->GetpLConn ()->FindIndexById (M4LDB_SYS_DICT_FIELDS, idForField);

			if (indForField == M4LDB_NULL_INDEX)
			{
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::PrepareValidateFRs" << "Logic field = " << LogCat << idForField << LogCat << " not found in the logic object = " << LogCat << idForLgcTbl);
				delete pCursor;
				return (M4_ERROR);
			}
			
			eFldType_t eForFldType = (eFldType_t) M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indForField].Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_INTERNAL_FIELD].Value.GetWithOutSec ());

			m4id_t idField = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGN_PARAMS]->Register[indForFld].Item[M4LDB_ITEM_SYS_DICT_FOREIGN_PARAMS_ID_FOREIGN_FIELD].Value.GetWithOutSec ());
			m4int32_t indFld = m_pParam->FindIndexByIdField (idField);

			if (indFld == M4LDB_NULL_INDEX) {
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::PrepareValidateFRs" << "Logic field = " << LogCat << idField << LogCat << " not found in the logic object = " << LogCat << m_pMtLgcTbl->m_idLgcTbl);
				delete pCursor;
				return (M4_ERROR);
			}

			if (eVerifyOrg != M4LDB_MULTI_ORGANIZATION && 
				m_pParam->GetInfoIdInternalField (indFld) == M4LDB_ID_ORGANIZATION && 
				eForFldType == M4LDB_ID_ORGANIZATION && 
				numForFld > 1) {
				// Quitamos la relación de organización si ambas tablas no son multi 
				// y hay mas de un campo relacionado.
				continue;
			}

			if (!pCursor->m_pvcIndParVal)
			{
				pCursor->m_pvcIndParVal = new VcIndParVal;
				pCursor->m_pvcIndParVal->reserve(10);
			}

			PStIndPar pIndPar = new struct StIndPar;

			pIndPar->m_indPar = indFld;
			pIndPar->m_idForFld = idForField;
			pIndPar->m_bIsPK = M4_TRUE;

			if ((((indFld == m_pMtLgcTbl->m_indDTStart) || 
				(indFld == m_pMtLgcTbl->m_indDTEnd)) && 
				((eForFldType == M4LDB_DT_START) || 
				(eForFldType == M4LDB_DT_END)) &&
				(bDTApp)) ||
				(((indFld == m_pMtLgcTbl->m_indDTStartCorr) || 
				(indFld == m_pMtLgcTbl->m_indDTEndCorr)) && 
				((eForFldType == M4LDB_DT_START_CORR) || 
				(eForFldType == M4LDB_DT_END_CORR)) &&
				(bDTCorr)))
			{
				pIndPar->m_bIsPK = M4_FALSE;
			}

			pCursor->m_pvcIndParVal->push_back (pIndPar);

			if (!pIndPar->m_bIsPK)
			{
				continue;
			}

			if (!bFirst)
			{
				stSelect.append (" AND ");
			}
			else
			{
				bFirst = M4_FALSE;
			}

			m4pchar_t pRealField = M4LDB_PSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indForField].Item[M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD].Value.GetWithOutSec ());

			stSelect.append ("T.");
			stSelect.append (pRealField);

			// Si la relación es de organización se hacen casos especiales
			if( m_pParam->GetInfoIdInternalField( indFld ) == M4LDB_ID_ORGANIZATION && eVerifyOrg == M4LDB_MULTI_ORGANIZATION && eForFldType == M4LDB_ID_ORGANIZATION )
			{
				// En la tabla destino siempre hay que traerse las organizaciones hijas
				stSelect.append( " IN (SELECT OT.ID_ORGANIZATION FROM M4VCH_ORG_TREE OT WHERE OT.ID_PAR_ORGANIZATIO = ?)" ) ;
			}
			else
			{
				stSelect.append( " = ?" ) ;
			}

			// Si la tabla original es de herencia de datos hay que guardarse los campos de la relación
			// para traerse sólo los registros que no validan
			if( m_bUseInheritance == M4_TRUE && m_pMtLgcTbl->m_bInheritsData == M4_TRUE && eVerifyOrg == M4LDB_MULTI_ORGANIZATION )
			{
				mapRelatedFields.insert(map<string, string>::value_type(idForField, pRealField));	
			}
		}

		// La fr entre tablas con tramitación va sobre la vista y hay que filtrar por el ID_DATASET que no es pk
		if( bForeignUseTramitation == M4_TRUE )
		{
			// Se calcula el ID_DATSET de la tabla origen
			m4int32_t indFld = m_pGlobVars->GetpLConn()->FindIndexByIdInternalField( M4CL_INTERNAL_TYPE_ID_DATASET ) ;

			if( indFld == M4LDB_NULL_INDEX )
			{
				DUMP_LDBINSP_ERRORF( m_pGlobVars->GetpLConn(), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, M4CL_INTERNAL_TYPE_ID_DATASET << idForLgcTbl ) ;
				delete pCursor;
				return( M4_ERROR ) ;
			}

			m4pchar_t pForDataSetId = M4LDB_PSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indFld].Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_FIELD].Value.GetWithOutSec ());
			m4pchar_t pForDataSetName = M4LDB_PSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indFld].Item[M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD].Value.GetWithOutSec ());

			//Situamos el current en la tabla origen
			m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[m_pMtLgcTbl->m_indLgcTbl];
			m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[m_indRealTbl];

			// Se calcula el ID_DATSET de la tabla fr
			indFld = m_pParam->FindIndexByIdInternalField (M4LDB_ID_DATASET);

			if( indFld == M4LDB_NULL_INDEX )
			{
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, M4LDB_ID_DATASET << m_pMtLgcTbl->m_idLgcTbl);
				delete pCursor;
				return (M4_ERROR);
			}

			// Se añade el parámetro
			PStIndPar pIndPar = new struct StIndPar;

			pIndPar->m_indPar = indFld;
			pIndPar->m_idForFld = pForDataSetId;
			pIndPar->m_bIsPK = M4_TRUE;

			pCursor->m_pvcIndParVal->push_back (pIndPar);

			if (!bFirst)
			{
				stSelect.append (" AND ");
			}
			else 
			{
				bFirst = M4_FALSE;
			}

			stSelect.append ("T.");
			stSelect.append (pForDataSetName);
			stSelect.append( " = ?" ) ;
		}

		// Si la tabla original es de herencia de datos hay que hacer la validación inversa
		if( m_bUseInheritance == M4_TRUE && m_pMtLgcTbl->m_bInheritsData == M4_TRUE && eVerifyOrg == M4LDB_MULTI_ORGANIZATION )
		{
			//Situamos el current en la tabla origen
			m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[m_pMtLgcTbl->m_indLgcTbl];
			m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[m_indRealTbl];

			stSelect.append( " AND NOT EXISTS (SELECT 1 FROM " ) ;

			m4pcchar_t pRealObject = M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_INHERIT_VIEW].Value.GetWithOutSec());

			stSelect.append( pRealObject ) ;
			stSelect.append( " V WHERE " ) ;

			m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current[indFor];

			bFirst = M4_TRUE;

			// Vamos a buscar si los campos existen en el mapa de los relacionados
			for( indForFld = 0 ; indForFld < numForFld ; indForFld++ )
			{
				idForField = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGN_PARAMS]->Register[indForFld].Item[M4LDB_ITEM_SYS_DICT_FOREIGN_PARAMS_ID_FOREIGN_FIELD].Value.GetWithOutSec ());

				itMapRelatedFields = mapRelatedFields.find( idForField ) ;

				if( itMapRelatedFields == mapRelatedFields.end() )
				{
					continue;
				}

				if( bFirst == M4_FALSE )
				{
					stSelect.append( " AND " ) ;
				}
				else
				{
					bFirst = M4_FALSE ;
				}

				m4id_t idField = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGN_PARAMS]->Register[indForFld].Item[M4LDB_ITEM_SYS_DICT_FOREIGN_PARAMS_ID_FIELD].Value.GetWithOutSec ());
				m4int32_t indFld = m_pGlobVars->GetpLConn ()->FindIndexById (M4LDB_SYS_DICT_FIELDS, idField);

				if( indFld == M4LDB_NULL_INDEX )
				{
					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::PrepareValidateFRs" << "Logic field = " << LogCat << idForField << idField << " not found in the logic object = " << LogCat << m_pMtLgcTbl->m_idLgcTbl);
					delete pCursor;
					return (M4_ERROR);
				}
				
				m4pchar_t pRealField = M4LDB_PSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indFld].Item[M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD].Value.GetWithOutSec ());
				
				stSelect.append( "V." ) ;
				stSelect.append( pRealField ) ;
				stSelect.append( " = T." ) ;
				stSelect.append( itMapRelatedFields->second ) ;
			}

			stSelect.append( ")" ) ;
		}

		if (bDTCorr)
		{
			stSelect.append (" AND ");
			stSelect.append ("T.");
			stSelect.append (pForDTEndCorr);
			stSelect.append (" > ? AND ");
			stSelect.append ("T.");
			stSelect.append (pForDTStartCorr);
			stSelect.append (" < ?");

			if (pForProjection)
			{
				stSelect.append (" AND ");
				stSelect.append ("T.");
				stSelect.append (pForProjection);
				stSelect.append (" <> 3");
				stSelect.append (" AND ");
				stSelect.append ("T.");
				stSelect.append (pForProjection);
				stSelect.append (" <> 6");
			}

			stSelect.append (" ORDER BY ");

			//Bug 0077123.
			bWithOrderBy = M4_TRUE;

			stSelect.append ("T.");
			stSelect.append (pForDTEndCorr);

// ¿Para qué servía el ordernar descendentemente?
//			stSelect.append (" DESC");

			if (bDTApp)
			{
				stSelect.append (", ");
				stSelect.append ("T.");
				stSelect.append (pForDTEnd);

//				stSelect.append (" DESC");
			}
		}
		else
		{
			if (bDTApp)
			{
				// Atención, estos order bys se hacen sobre DT_END para evitar el error de
				// ambiguedad que se produce por el hecho de estar repetido en las columnas 
				// el campo DT_START cuando este es PK (lo mismo se aplica a las correcciones)
				stSelect.append (" ORDER BY ");
				stSelect.append ("T.");
				stSelect.append (pForDTEnd);

				//Bug 0077123.
				bWithOrderBy = M4_TRUE;
//				stSelect.append (" DESC");
			}
		}

		// Si se usa la vista se hace el order por nivel para el borrado lógico
		if( bForeignInheritsData == M4_TRUE && m_pMtLgcTbl->m_bInheritsData == M4_TRUE )
		{
			if( bWithOrderBy == M4_FALSE )
			{
				stSelect.append( " ORDER BY " ) ;
				bWithOrderBy = M4_TRUE ;
			}
			else
			{
				stSelect.append( ", " ) ;
			}

			stSelect.append( "T.ID_ORG_LEVEL DESC" ) ;
		}

		//Bug 0077123.
		//Informix. No acepta ORDER BY c1 FOR UPDATE.
		//Porsiaca si no hay order igual hay que poner el For Update.
		// En la vista no se puede hacer un for update
		if( bAddForUpdate == M4_TRUE )
		{
			if (bWithOrderBy == M4_TRUE) 
			{		
				pLock->AddLockForUpdateInOrderBy(stSelect);
			}
			else
			{
				pLock->AddLockForUpdate(stSelect);
			}
		}

		result = pCursor->Prepare ((m4pchar_t) stSelect.c_str ());

		if (result == M4_ERROR) {
			delete pCursor;
			return (M4_ERROR);
		}

		pCursor->m_bVerifyData = bStmVerifyData;
		pCursor->m_bVerifyDTApp = bDTApp;
		pCursor->m_bVerifyDTCorr = bDTCorr;
		pCursor->m_bVerifyOperation = bOperation;
		pCursor->m_eVerifyOrg = eVerifyOrg;
		pCursor->m_bAllowPreviousIni = bAllowPreviousIni;
		pCursor->m_indForLgcTbl = indForLgcTbl;

		if (indCursor == M4LDB_VALIDATE_COMP)
		{
			// UNIOCODE DB
			m_pcCompleteDTStart = (m4pchar_t) &m_dCompleteDTStart;
			m_iCompleteDTStart = M4LDB_SIZE_TIMESTAMP;
			result = pCursor->BindResult (1, &m_pcCompleteDTStart, &m_iCompleteDTStart, M4CL_M4_TYPE_TIMESTAMP, &m_lCompleteDTStart);

			if (result == M4_ERROR) {
				delete pCursor;
				return (M4_ERROR);
			}

			// UNIOCODE DB
			m_pcCompleteDTEnd = (m4pchar_t) &m_dCompleteDTEnd;
			m_iCompleteDTEnd = M4LDB_SIZE_TIMESTAMP;
			result = pCursor->BindResult (2, &m_pcCompleteDTEnd, &m_iCompleteDTEnd, M4CL_M4_TYPE_TIMESTAMP, &m_lCompleteDTEnd);

			if (result == M4_ERROR) {
				delete pCursor;
				return (M4_ERROR);
			}
		}

		m_pmpValCur->insert (MpPValCursor::value_type (indCursor, pCursor));
	}

	//Situamos el current.
	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[m_pMtLgcTbl->m_indLgcTbl];
	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[m_indRealTbl];

	return (M4_SUCCESS);
}


// Queda la validación del antiguo histórico cuando hacemos UpdatePK.
m4return_t ClPersistBase::PrepareGetHistory (void)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;

	if (m_pHistTbl) {
		return (M4_SUCCESS);
	}

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Get History.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Get History.");
	#endif

	m4int32_t numCol = 0;

	string stSelect ("SELECT ");

	stSelect.append ("T.");
	stSelect.append (m_pParam->GetInfoRealField (m_pMtLgcTbl->m_indDTStart));

	numCol++;

	stSelect.append (", ");
	stSelect.append ("T.");
	stSelect.append (m_pParam->GetInfoRealField (m_pMtLgcTbl->m_indDTEnd));

	numCol++;

	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);
		m4bool_t bIsCtrlRep = m_pParam->GetInfoIsCtrlRep (indFld);

		if (!bIsPK && bIsCtrlRep)
		{
			m4pcchar_t pRealField = m_pParam->GetInfoRealField (indFld);

			stSelect.append (", ");
			stSelect.append ("T.");
			stSelect.append (pRealField);

			numCol++;
		}
	}

	m_pHistTbl = new ClTblMem;

	m_pHistTbl->Init (m_pGlobVars->GetpLConn()->m_pLdb,numCol);

	m4bool_t bAddForUpdate = M4_TRUE ;
	m4pcchar_t pRealObject = M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_REAL_OBJECT].Value.GetWithOutSec ());

	if( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
	{
		// En tablas con tramitación se debe utilizar la vista
		bAddForUpdate = M4_FALSE ;
		pRealObject = M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_TRAMIT_VIEW].Value.GetWithOutSec ());
	}

	stSelect.append (" FROM ");
	stSelect.append (pRealObject);
	stSelect.append (" T");

	ClLConn *pLConn = m_pGlobVars->GetpLConn ();
	eDDBBType_t eDDBB = M4DBW_ORACLE_DDBB;
	
	if (pLConn->GetConnManager()->GetDDBBType(m_idRConn,pLConn,eDDBB)!=M4_SUCCESS){
		return M4_ERROR;
	}
	_ClLockerTranslator *pLock = ClTraductor::s_ppTrUtils[eDDBB-1]->m_pLockTrans;

	// En la vista no se puede hacer un for update
	if( bAddForUpdate == M4_TRUE )
	{
		pLock->AddLockBetweenJoins(stSelect);
	}

	stSelect.append (" WHERE ");

	m4bool_t bFirst = M4_TRUE;

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

		if (bIsPK && indFld != m_pMtLgcTbl->m_indDTStart)
		{
			if (!bFirst)
			{
				stSelect.append (" AND ");
			}
			else
			{
				bFirst = M4_FALSE;
			}

			m4pcchar_t pRealField = m_pParam->GetInfoRealField (indFld);

			stSelect.append ("T.");
			stSelect.append (pRealField);
			stSelect.append (" = ?");
		}
	}

	if (m_pMtLgcTbl->m_indRegType != M4LDB_NULL_INDEX)
	{
		// Las proyecciones no entran.
		m4pcchar_t pRealProjection = m_pParam->GetInfoRealField (m_pMtLgcTbl->m_indRegType);

		stSelect.append (" AND ");
		stSelect.append ("T.");
		stSelect.append (pRealProjection);
		stSelect.append (" <> 3");
	}

	stSelect.append (" ORDER BY ");
	stSelect.append ("T.");
	stSelect.append (m_pParam->GetInfoRealField (m_pMtLgcTbl->m_indDTStart));
	stSelect.append (" ASC");

	//Bug 0077123.
	//Informix. No acepta ORDER BY c1 FOR UPDATE.
	// En la vista no se puede hacer un for update
	if( bAddForUpdate == M4_TRUE )
	{
		pLock->AddLockForUpdateInOrderBy(stSelect);	
	}

	ClLoad *pCursor = new ClLoad (m_pGlobVars, M4_TRUE, this);
	result = pCursor->Open (m_idRConn, NULL, M4_FALSE, m_pParam);

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	result = pCursor->Prepare ((m4pchar_t) stSelect.c_str ());

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_GET_HISTORY, pCursor));

	return (result);
}


m4return_t ClPersistBase::PrepareGetCorrectedHistory (void)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;

	if (m_pCorrHistTbl) {
		return (M4_SUCCESS);
	}

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Get Corrected History.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Get Corrected History.");
	#endif

	m4int32_t numCol = 0;

	string stSelect ("SELECT ");

	if (m_pMtLgcTbl->m_eCurTemp != M4LDB_ETHERN && m_pMtLgcTbl->m_eCurTemp != M4LDB_1_PERIOD)
	{
		if (m_pMtLgcTbl->m_indDTStart != M4LDB_NULL_INDEX)
		{
			stSelect.append ("T.");
			stSelect.append (m_pParam->GetInfoRealField (m_pMtLgcTbl->m_indDTStart));
			numCol++;
		}

		if (m_pMtLgcTbl->m_indDTEnd != M4LDB_NULL_INDEX)
		{
			stSelect.append (", ");
			stSelect.append ("T.");
			stSelect.append (m_pParam->GetInfoRealField (m_pMtLgcTbl->m_indDTEnd));

			numCol++;
		}
	}

	if (m_pMtLgcTbl->m_indDTStartCorr != M4LDB_NULL_INDEX)
	{
		if (numCol)
		{
			stSelect.append (", ");
		}

		stSelect.append ("T.");
		stSelect.append (m_pParam->GetInfoRealField (m_pMtLgcTbl->m_indDTStartCorr));

		numCol++;
	}

	if (m_pMtLgcTbl->m_indDTEndCorr != M4LDB_NULL_INDEX)
	{
		stSelect.append (", ");
		stSelect.append ("T.");
		stSelect.append (m_pParam->GetInfoRealField (m_pMtLgcTbl->m_indDTEndCorr));

		numCol++;
	}

	if (m_pMtLgcTbl->m_indRegType != M4LDB_NULL_INDEX)
	{
		stSelect.append (", ");
		stSelect.append ("T.");
		stSelect.append (m_pParam->GetInfoRealField (m_pMtLgcTbl->m_indRegType));

		numCol++;
	}

	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);
		m4bool_t bIsCtrlRep = m_pParam->GetInfoIsCtrlRep (indFld);

		if (!bIsPK && bIsCtrlRep)
		{
			m4pcchar_t pRealField = m_pParam->GetInfoRealField (indFld);

			stSelect.append (", ");
			stSelect.append ("T.");
			stSelect.append (pRealField);

			numCol++;
		}
	}

	m_pCorrHistTbl = new ClTblMem;

	m_pCorrHistTbl->Init (m_pGlobVars->GetpLConn()->m_pLdb,numCol);


	m4bool_t bAddForUpdate = M4_TRUE ;
	m4pcchar_t pRealObject = M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_REAL_OBJECT].Value.GetWithOutSec ());

	if( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
	{
		// En tablas con tramitación se debe utilizar la vista
		bAddForUpdate = M4_FALSE ;
		pRealObject = M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_TRAMIT_VIEW].Value.GetWithOutSec ());
	}

	stSelect.append (" FROM ");
	stSelect.append (pRealObject);
	stSelect.append (" T");

	ClLConn *pLConn = m_pGlobVars->GetpLConn ();
	eDDBBType_t eDDBB = M4DBW_ORACLE_DDBB;
	
	if (pLConn->GetConnManager()->GetDDBBType(m_idRConn,pLConn,eDDBB)!=M4_SUCCESS){
		return M4_ERROR;
	}
	_ClLockerTranslator *pLock = ClTraductor::s_ppTrUtils[eDDBB-1]->m_pLockTrans;

	// En la vista no se puede hacer un for update
	if( bAddForUpdate == M4_TRUE )
	{
		pLock->AddLockBetweenJoins(stSelect);
	}

	stSelect.append (" WHERE ");


	m4bool_t bFirst = M4_TRUE;

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

		if (bIsPK && indFld != m_pMtLgcTbl->m_indDTStart && indFld != m_pMtLgcTbl->m_indDTStartCorr)
		{
			if (!bFirst)
			{
				stSelect.append (" AND ");
			}
			else
			{
				bFirst = M4_FALSE;
			}

			m4pcchar_t pRealField = m_pParam->GetInfoRealField (indFld);

			stSelect.append ("T.");
			stSelect.append (pRealField);
			stSelect.append (" = ?");
		}
	}

	m4pcchar_t pRealDTEndCorr = m_pParam->GetInfoRealField (m_pMtLgcTbl->m_indDTEndCorr);

	stSelect.append (" AND ");
	stSelect.append ("T.");
	stSelect.append (pRealDTEndCorr);
	// La fecha de fin de corrección es mayor o igual al valor de la fecha de inicio de correción menos uno.
	stSelect.append (" >= ?");
	
	// En la vista no se puede hacer un for update
	if( bAddForUpdate == M4_TRUE )
	{
		pLock->AddLockForUpdate(stSelect);
	}

	ClLoad *pCursor = new ClLoad (m_pGlobVars, M4_TRUE, this);
	result = pCursor->Open (m_idRConn, NULL, M4_FALSE, m_pParam);

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	result = pCursor->Prepare ((m4pchar_t) stSelect.c_str ());

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_GET_CORR_HISTORY, pCursor));

	return (result);
}


m4return_t ClPersistBase::PrepareCursorQuery (m4pcchar_t ai_pStmt)
{
	m4return_t result;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Query.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Query.");
	#endif

	ClLoad *pCursor = new ClLoad (m_pGlobVars, M4_TRUE, this);
	result = pCursor->Open (m_idRConn, m_pMtSent, M4_FALSE);

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	// Aquí puede cambiarse el current de la tabla real.
	string stStmSQL (M4LDB_EMPTY);


	if (ai_pStmt) {
		// En el caso del Delete no podemos utilizar alias en el From principal.
		m_pMtSent->SetDontUseAlias(M4_TRUE);
	}

	result = pCursor->TranslateSelect (stStmSQL);

	if (ai_pStmt) {
		// En el caso del Delete no podemos utilizar alias en el From principal.
		m_pMtSent->SetDontUseAlias(M4_FALSE);
	}

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	if (ai_pStmt) {
		m4int32_t lPos = stStmSQL.find (" FROM ");
		stStmSQL.replace (0, lPos + 6, ai_pStmt);
	}
		
	result = pCursor->Prepare ((m4pchar_t) stStmSQL.c_str ());

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[m_pMtLgcTbl->m_indLgcTbl];
	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[m_indRealTbl];

	m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_CURSOR_QUERY, pCursor));
	
	return (result);
}


m4return_t ClPersistBase::PrepareValidateSecurity (void)
{
	m4return_t result;

	if (!m_pMtSecSent) {
		return (M4_SUCCESS);
	}

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Validate Security.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing Validate Security.");
	#endif

	ClLoad *pCursor = new ClLoad (m_pGlobVars, M4_TRUE, this);
	result = pCursor->Open (m_idRConn, m_pMtSecSent, M4_FALSE);

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	string stStmSQL;

	//Bug 0068168. Indicamos que se va a validar la seguridad. Está involucrada la tabla
	//de control.
	m_pMtSecSent->SetValidatingSecurity(M4_TRUE);
	// Aquí puede cambiarse el current de la tabla real.

	result = pCursor->Prepare (stStmSQL);

	//Bug 0068168. El prepare de la validación de seguridad ha terminado.
	m_pMtSecSent->SetValidatingSecurity(M4_FALSE);
	


	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[m_pMtLgcTbl->m_indLgcTbl];
	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[m_indRealTbl];

	m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_VALIDATE_SECURITY, pCursor));
	
	return (result);
}

m4return_t ClPersistBase::PrepareValidateSecurityInOtherConnection (void)
{
	m4return_t result;

	if (!m_pMtSecSent) {
		return (M4_SUCCESS);
	}

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Validate Security In Other Connection.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing Validate Security In Other Connection.");
	#endif

	//Lo ejecutaremos en una conexion propia.
	ClLoad *pCursor = new ClLoad (m_pGlobVars, M4_TRUE, this);
	result = pCursor->Open (ClConnectionManager::m_sValidateSecurity, m_pMtSecSent, M4_FALSE);

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	string stStmSQL;

	result = pCursor->Prepare (stStmSQL);

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[m_pMtLgcTbl->m_indLgcTbl];
	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[m_indRealTbl];

	m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_VALIDATE_SECURITY_OTHER_CONNECTION, pCursor));
	
	return (result);
}

m4return_t ClPersistBase::PrepareAuditory (void)
{
	m4return_t result;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Auditory.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Auditory.");
	#endif

	m4id_t idAuditObject = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_AUDIT_OBJECT].Value.GetWithOutSec ());

	if (!*idAuditObject) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_AUDITORY_LOGIC_OBJECT_NOT_DEFINED, m_pMtLgcTbl->m_idLgcTbl);
		return (M4_ERROR);
	}

	// Esto nos situa el logic object.
	result = m_pGlobVars->GetpLConn ()->LoadDictionaryChannel (idAuditObject, NULL, M4_TRUE);

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	ClPersist *pCursor = new ClPersist (m_pGlobVars, M4_TRUE, M4_FALSE, M4_FALSE, M4_FALSE);

	//Bug 0084837
	pCursor->SeteStmTypeAuditory (m_eStmType);

	result = pCursor->Open (m_pParam);

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	result = pCursor->Prepare ();

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_CURSOR_AUDITORY, pCursor));
	
	if (m_eStmType == M4LDB_UPDATE) {
		// Para el registro antiguo.
		ClDinArr *pParam = new ClDinArr (m_pParam);

		ClPersist *pCursor2 = new ClPersist (m_pGlobVars, M4_TRUE, M4_FALSE, M4_FALSE, M4_FALSE);

		result = pCursor2->Open (pParam);

		if (result == M4_ERROR) {
			delete pCursor2;
			return (M4_ERROR);
		}

		result = pCursor2->Prepare ();

		if (result == M4_ERROR) {
			delete pCursor2;
			return (M4_ERROR);
		}

		m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_CURSOR_AUDITORY_2,pCursor2));
	}

	// Resituamos los currents.
	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[m_pMtLgcTbl->m_indLgcTbl];
	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[m_indRealTbl];

	return (M4_SUCCESS);
}




m4return_t ClPersistBase::PrepareTramitExist (m4bool_t ai_bParent, m4bool_t ai_bLock, m4bool_t ai_bDtLastUpdate)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Tramitation Exist.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Tramitation Exist.");
	#endif

	ClLConn *pLConn = m_pGlobVars->GetpLConn ();
	eDDBBType_t eDDBB = M4DBW_ORACLE_DDBB;
	
	if (pLConn->GetConnManager()->GetDDBBType(m_idRConn,pLConn,eDDBB)!=M4_SUCCESS){
		return M4_ERROR;
	}

	_ClLockerTranslator *pLock = ClTraductor::s_ppTrUtils[eDDBB-1]->m_pLockTrans;

	string stStmSQL;
	stStmSQL.reserve(500);

	stStmSQL = "SELECT ";

	if( ai_bDtLastUpdate == M4_TRUE) 
	{
		stStmSQL.append ("TRAMIT_DT_LAST_UPDATE") ;
	}
	else
	{
		stStmSQL.append ("1");
	}

	stStmSQL.append ( " FROM " );

	ClItemIndex iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_REAL_OBJECT ;

	if( ai_bParent == M4_FALSE )
	{
		iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_MIRROR_REAL_OBJECT;
	}

	stStmSQL.append (M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[iTableName].Value.GetWithOutSec ()));
	stStmSQL.append (" T");

	// Hacer un for update si se pide
	if( ai_bLock == M4_TRUE )
	{
		pLock->AddLockBetweenJoins(stStmSQL);
	}

	stStmSQL.append (" WHERE ");

	m4bool_t bFirst = M4_TRUE;
	m4bool_t bExistsPK = M4_FALSE;
	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

		if( !bIsPK )
		{
			continue;
		}

		eFldType_t eIntFld = m_pParam->GetInfoIdInternalField (indFld);

		if( ai_bParent == M4_TRUE )
		{
			// El dataset no existe en la tabla original
			if( eIntFld == M4LDB_ID_DATASET )
			{
				continue;
			}
		}

		// Es PK.
		bExistsPK = M4_TRUE;

		if( !bFirst )
		{
			stStmSQL.append (" AND ");
		}
		else
		{
			bFirst = M4_FALSE;
		}

		m4pcchar_t pRealField = m_pParam->GetInfoRealField (indFld);

		stStmSQL.append ("T.");
		stStmSQL.append (pRealField);
		stStmSQL.append (" = ?");
	}

	if (!bExistsPK)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_NO_PK_FIELDS, m_pMtLgcTbl->m_idLgcTbl);
		return (M4_ERROR);
	}

	// Hacer un for update si se pide
	if( ai_bLock == M4_TRUE )
	{
		pLock->AddLockForUpdate(stStmSQL);
	}

	ClLoad *pCursor = new ClLoad (m_pGlobVars, M4_TRUE, this);
	result = pCursor->Open (m_idRConn, NULL, M4_FALSE, m_pParam);

	if (result == M4_ERROR)
	{
		delete pCursor;
		return (M4_ERROR);
	}

	result = pCursor->Prepare ((m4pchar_t) stStmSQL.c_str ());

	if (result == M4_ERROR)
	{
		delete pCursor;
		return (M4_ERROR);
	}

	m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_TRAMIT_EXIST, pCursor));

	return( M4_SUCCESS ) ;
}


m4return_t ClPersistBase::PrepareTramitDelete (void)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Tramitation Delete.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Tramitation Delete.");
	#endif

	string stStmSQL;
	stStmSQL.reserve( 500 );

	stStmSQL = "DELETE FROM ";
	stStmSQL.append (M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_MIRROR_REAL_OBJECT].Value.GetWithOutSec ()));
	stStmSQL.append (" WHERE ");

	m4bool_t bFirst = M4_TRUE;
	m4bool_t bExistsPK = M4_FALSE;
	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

		if (!bIsPK)
		{
			continue;
		}

		// Es PK.
		bExistsPK = M4_TRUE;

		if (!bFirst)
		{
			stStmSQL.append (" AND ");
		}
		else
		{
			bFirst = M4_FALSE;
		}

		m4pcchar_t pRealField = m_pParam->GetInfoRealField (indFld);
		stStmSQL.append (pRealField);
		stStmSQL.append (" = ?");
	}

	if (!bExistsPK) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_NO_PK_FIELDS, m_pMtLgcTbl->m_idLgcTbl);
		return (M4_ERROR);
	}

	ClLoad *pCursor = new ClLoad (m_pGlobVars, M4_TRUE, this);

	result = pCursor->Open (m_idRConn, NULL, M4_FALSE, m_pParam);

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	result = pCursor->Prepare ((m4pchar_t) stStmSQL.c_str ());

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_TRAMIT_DELETE, pCursor));

	return( M4_SUCCESS ) ;
}

m4return_t ClPersistBase::PrepareTramitInsert(m4pcchar_t ai_pccOperation)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Tramitation Insert.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Tramitation Insert.");
	#endif

	string stStmSQL;
	string stValues;

	stStmSQL.reserve( 500 );
	stValues.reserve( 500 );

	stStmSQL = "INSERT INTO ";
	stStmSQL.append (M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_MIRROR_REAL_OBJECT].Value.GetWithOutSec ()));
	stStmSQL.append (" (");

	m4bool_t bFirst = M4_TRUE;
	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];

		if (!bFirst)
		{
			stStmSQL.append (", ");
			stValues.append (", ");
		}
		else
		{
			if( m_indRealTbl == 0 )
			{
				// Sólo para la tabla principal
				stStmSQL.append ("TRAMIT_OPERATION, ");
				stValues.append (ai_pccOperation);
				stValues.append (", ");
			}
			bFirst = M4_FALSE;
		}

		m4pcchar_t pRealField = m_pParam->GetInfoRealField (indFld);

		stStmSQL.append (pRealField);
		stValues.append ("?");

		if( m_indRealTbl == 0 )
		{
			eFldType_t eIntFld = m_pParam->GetInfoIdInternalField (indFld);

			if( eIntFld == M4LDB_DT_LAST_UPDATE )
			{
				stStmSQL.append (", TRAMIT_DT_LAST_UPDATE");
				stValues.append (", ?");
			}
			else if (eIntFld == M4LDB_DT_START )
			{
				stStmSQL.append (", TRAMIT_DT_START");
				stValues.append (", ?");
			}
		}
	}

	stStmSQL.append (") VALUES (");
	stStmSQL.append (stValues);
	stStmSQL.append (")");

	ClLoad *pCursor = new ClLoad (m_pGlobVars, M4_TRUE, this);
	result = pCursor->Open (m_idRConn, NULL, M4_FALSE, m_pParam);

	if (result == M4_ERROR)
	{
		delete pCursor;
		return (M4_ERROR);
	}

	result = pCursor->Prepare ((m4pchar_t) stStmSQL.c_str ());

	if (result == M4_ERROR)
	{
		delete pCursor;
		return (M4_ERROR);
	}

	m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_TRAMIT_INSERT, pCursor));

	return( M4_SUCCESS ) ;
}

m4return_t ClPersistBase::PrepareTramitInsertObject (void)
{
	//INSERT INTO M4RTR_TRAMIT_OBJ (ID_TRAMITATION, ID_OBJECT) 
	//SELECT T.ID_TRAMITATION, ? FROM M4RTR_TRAMITATION T 
	//WHERE T. ID_TRAMITATION = ? AND NOT EXISTS 
	//				( SELECT 1 FROM M4RTR_TRAMIT_OBJ M 
	//				WHERE M.ID_TRAMITATION = T. ID_TRAMITATION AND M.ID_OBJECT = ? )

	m4return_t result;
	string stStmSQL;
	string stValues;

	#ifdef _M4LDB_DEBUG
		//Traza Preparing Cursos
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Tramitation Insert Object.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Tramitation Insert Object.");
	#endif

	stStmSQL.reserve( 500 ); 
	
	//Creamos la sentencia es cableada
	stStmSQL = "INSERT INTO M4RTR_TRAMIT_OBJ (ID_TRAMITATION, ID_OBJECT, DT_LAST_UPDATE, ID_SECUSER, ID_APPROLE)";
	
	stStmSQL.append (" SELECT T.ID_TRAMITATION, '") ;
	stStmSQL.append (m_idMainTable) ;
	stStmSQL.append ("', ?, ?, ? FROM M4RTR_TRAMITATION T ") ;

	stStmSQL.append (" WHERE T. ID_TRAMITATION = ? AND NOT EXISTS ");
	stStmSQL.append (" ( SELECT 1 FROM M4RTR_TRAMIT_OBJ M WHERE M.ID_TRAMITATION = T. ID_TRAMITATION AND M.ID_OBJECT = '") ;
	stStmSQL.append (m_idMainTable) ;
	stStmSQL.append ("') ") ;

	ClLoad *pCursor = new ClLoad (m_pGlobVars, M4_TRUE, this);
	result = pCursor->Open (m_idRConn, NULL, M4_FALSE, m_pParam);

	if (result == M4_ERROR)
	{
		delete pCursor;
		return (M4_ERROR);
	}

	result = pCursor->Prepare ((m4pchar_t) stStmSQL.c_str ());

	if (result == M4_ERROR)
	{
		delete pCursor;
		return (M4_ERROR);
	}

	m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_TRAMIT_INSERT_OBJECT, pCursor));
	return( M4_SUCCESS ) ;
}

m4return_t ClPersistBase::PrepareValidateOrganization (void)
{
	m4return_t result;

	//Bug 0095175
	//La comprobación no se realiza por RSM (m_pGlobVars->GetIdRSM ()), se realizará por el Role del usuario.
	//Si no hay Role definido no se aplicará ningún filtro.
	m4id_t	pccRole = m_pGlobVars->GetIdSecRole();

	if (pccRole == NULL || strcmpi (pccRole, M4CL_SEC_SUPER_USER) == 0 || strcmpi (pccRole, "M4RAISE_PRIVILEGES") == 0)
	{
		// SuperRol.
		return M4_SUCCESS;
	}

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Validate Organization.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Validate Organization.");
	#endif

	//Bug 0071876
	//Si los valores de Role, Usuario, o Sociedad vienen a -1
	//En el bindado dará un GP, por lo tanto, no creamos el cursor
	//si alguno de ellos es -1.
	//Para la tabla padre sí está definido, pero para los hijos el valor es -1.
	if (!m_pMtLgcTbl) {
		return M4_ERROR;
	}
	if ((m_pMtLgcTbl->m_indAppRole == M4LDB_NULL_INDEX) || 
		(m_pMtLgcTbl->m_indOrgType == M4LDB_NULL_INDEX) ||
		(m_pMtLgcTbl->m_indSecUser == M4LDB_NULL_INDEX))
	{
		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\tIndex in logic table for User, Role, Organization not defined.");
		#endif
		return M4_SUCCESS;
	}
	//Fin bug.
		
	string stSelect ;
	//Bug 0070817
	//Hasta ahora no se había ejecutado esta select, sin embargo hay un error pq el campo org de la tabla física
	//M4RSC_APP_USR_ROLE es ID_ORGANIZATION_US.

	/* Bug 0124717
	Se cambia la select porque había un producto cartesiano
	*/
	/* De momento no hay que filtrar por las hijas en multinivel
	stSelect = "SELECT 1 FROM M4RSC_ROLE_ORG R, M4VCH_ORG_TREE T WHERE R.ID_ORGANIZATION_US = T.ID_PAR_ORGANIZATION AND R.ID_APP_ROLE_USED = ? AND = T.ID_ORGANIZATION = ? UNION SELECT 1 FROM M4RSC_APP_USR_ROLE U, M4VCH_ORG_TREE T WHERE U.ID_ORGANIZATION_US = T.ID_PAR_ORGANIZATION AND U.ID_APP_ROLE = ? AND T.ID_ORGANIZATION = ? AND U.ID_APP_USR = ?" ;
	*/
	stSelect = "SELECT 1 FROM M4RSC_ROLE_ORG A WHERE A.ID_APP_ROLE_USED = ? AND A.ID_ORGANIZATION_US = ? UNION SELECT 1 FROM M4RSC_APP_USR_ROLE B WHERE B.ID_APP_ROLE = ? AND B.ID_ORGANIZATION_US = ? AND B.ID_APP_USR = ?" ;


	/* Bug 0182590
	Se consigue la función SHORT_TODAY y se pone el filtro de fechas
	*/
	eDDBBType_t eType = M4DBW_ORACLE_DDBB;
	ClLConn *pLConn = m_pGlobVars->GetpLConn();
	pLConn->GetConnManager()->GetDDBBType(m_idRConn, pLConn, eType);
	m4int32_t indDDBB = pLConn->FindIndexById (M4LDB_SYS_DICT_DDBBS, eType);

	pLConn->m_paRS[M4LDB_SYS_DICT_DDBBS]->Current[indDDBB];

	m4int32_t indFunction = pLConn->FindIndexById (M4LDB_SYS_DICT_FUNCTIONS, "SHORT_TODAY");

	if (indFunction == M4LDB_NULL_INDEX)
	{
		DUMP_LDBINSP_ERRORF (pLConn, M4_CH_LDB_WRONG_LOGIC_FUNCTION, "SHORT_TODAY" << eType);
		return (M4_ERROR);
	}

	m4id_t idShortToday = M4LDB_ID (pLConn->m_paRS[M4LDB_SYS_DICT_FUNCTIONS]->Register[indFunction].Item[M4LDB_ITEM_SYS_DICT_FUNCTIONS_FUNCTION_HEADER].Value.GetWithOutSec ());

	stSelect.append(" AND B.DT_START <= ");
	stSelect.append(idShortToday);
	stSelect.append(" AND B.DT_END >= ");
	stSelect.append(idShortToday);


	ClLoad *pCursor = new ClLoad (m_pGlobVars, M4_TRUE, this);

	result = pCursor->Open (m_idRConn, NULL, M4_FALSE, m_pParam);

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	result = pCursor->Prepare ((m4pchar_t) stSelect.c_str ());

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_VALIDATE_ORGANIZATION, pCursor));

	return (result);
}


m4return_t ClPersistBase::PrepareInheritExist (m4bool_t ai_bParent, m4bool_t ai_bLock)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Inherit Exist.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Inherit Exist.");
	#endif

	ClLConn *pLConn = m_pGlobVars->GetpLConn ();
	eDDBBType_t eDDBB = M4DBW_ORACLE_DDBB;
	
	if (pLConn->GetConnManager()->GetDDBBType(m_idRConn,pLConn,eDDBB)!=M4_SUCCESS){
		return M4_ERROR;
	}

	_ClLockerTranslator *pLock = ClTraductor::s_ppTrUtils[eDDBB-1]->m_pLockTrans;

	string stStmSQL;
	stStmSQL.reserve( 500 );

	stStmSQL = "SELECT 1 FROM ";
	stStmSQL.append (M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_REAL_OBJECT].Value.GetWithOutSec ()));
	stStmSQL.append (" T");

	// Hacer un for update si se pide
	if( ai_bLock == M4_TRUE )
	{
		pLock->AddLockBetweenJoins(stStmSQL);
	}

	stStmSQL.append (" WHERE ");

	m4bool_t bFirst = M4_TRUE;
	m4bool_t bExistsPK = M4_FALSE;
	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

		if (!bIsPK)
		{
			continue;
		}

		// Es PK.
		bExistsPK = M4_TRUE;

		if (!bFirst)
		{
			stStmSQL.append (" AND ");
		}
		else
		{
			bFirst = M4_FALSE;
		}

		m4pcchar_t pRealField = m_pParam->GetInfoRealField (indFld);
		stStmSQL.append ("T.");
		stStmSQL.append (pRealField);

		if( ai_bParent == M4_TRUE && m_pParam->GetInfoIdInternalField( indFld ) == M4LDB_ID_ORGANIZATION )
		{
			// Si es del padre se filtra por las sociedades padre
			stStmSQL.append( " IN (SELECT OT.ID_PAR_ORGANIZATIO FROM M4VCH_ORG_TREE OT WHERE OT.ID_ORGANIZATION <> OT.ID_PAR_ORGANIZATIO AND OT.ID_ORGANIZATION = ?)" ) ;
		}
		else
		{
			stStmSQL.append (" = ?");
		}
	}

	if (!bExistsPK) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_NO_PK_FIELDS, m_pMtLgcTbl->m_idLgcTbl);
		return (M4_ERROR);
	}

	// Hacer un for update si se pide
	if( ai_bLock == M4_TRUE )
	{
		pLock->AddLockForUpdate(stStmSQL);
	}

	ClLoad *pCursor = new ClLoad (m_pGlobVars, M4_TRUE, this);

	result = pCursor->Open (m_idRConn, NULL, M4_FALSE, m_pParam);

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	result = pCursor->Prepare ((m4pchar_t) stStmSQL.c_str ());

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_INHERIT_EXIST, pCursor));

	return( M4_SUCCESS ) ;
}


m4return_t ClPersistBase::PrepareInheritDelete (void)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Inherit Delete.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Inherit Delete.");
	#endif

	/* Bug 0153092
	Se quita el alias del delete porque en SQLServer no se reconoce
	*/
	string stStmSQL;
	stStmSQL.reserve( 500 );

	stStmSQL = "DELETE FROM ";
	stStmSQL.append (M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_REAL_OBJECT].Value.GetWithOutSec ()));
	stStmSQL.append (" WHERE ");

	m4bool_t bFirst = M4_TRUE;
	m4bool_t bExistsPK = M4_FALSE;
	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

		if (!bIsPK)
		{
			continue;
		}

		// Es PK.
		bExistsPK = M4_TRUE;

		if (!bFirst)
		{
			stStmSQL.append (" AND ");
		}
		else
		{
			bFirst = M4_FALSE;
		}

		m4pcchar_t pRealField = m_pParam->GetInfoRealField (indFld);
		stStmSQL.append (pRealField);
		stStmSQL.append (" = ?");
	}

	if (!bExistsPK) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_NO_PK_FIELDS, m_pMtLgcTbl->m_idLgcTbl);
		return (M4_ERROR);
	}

	/* Bug 0170168, 0180794
	Sólo hay que borrar si el registro es borrado: OPERATION = 0
	*/
	if( m_indRealTbl == 0 )
	{
		//Si estamos en la tabla principal filtramos directamente
		stStmSQL.append (" AND OPERATION = 0");
	}
	else
	{
		//Si estamos en una secundaria hay que hacer un EXISTS con la principal
		stStmSQL.append ( " AND EXISTS ( SELECT NULL FROM " ) ;
		stStmSQL.append ( M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Register[0].Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_REAL_OBJECT].Value.GetWithOutSec ()) );
		stStmSQL.append ( " B WHERE ") ;		

		bFirst = M4_TRUE;

		for (indPar=0; indPar<numPar; indPar++)
		{
			indFld = m_oVParam[indPar];
			m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

			if (!bIsPK)
			{
				continue;
			}

			// Es PK.
			bExistsPK = M4_TRUE;

			if (!bFirst)
			{
				stStmSQL.append (" AND ");
			}
			else
			{
				bFirst = M4_FALSE;
			}

			m4pcchar_t pRealField = m_pParam->GetInfoRealField (indFld);
			stStmSQL.append ("B.");
			stStmSQL.append (pRealField);
			stStmSQL.append (" = ?");
		}

		stStmSQL.append (" AND B.OPERATION = 0 ) ");
	}



	ClLoad *pCursor = new ClLoad (m_pGlobVars, M4_TRUE, this);

	result = pCursor->Open (m_idRConn, NULL, M4_FALSE, m_pParam);

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	result = pCursor->Prepare ((m4pchar_t) stStmSQL.c_str ());

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_INHERIT_DELETE, pCursor));

	return( M4_SUCCESS ) ;
}


m4return_t ClPersistBase::PrepareInheritInsert(m4pcchar_t ai_pccOperation)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Inherit Insert.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Inherit Insert.");
	#endif

	/* Bug 0153092
	Se quita el alias del insert porque en SQLServer no se reconoce
	*/
	string stStmSQL;
	string stValues;

	stStmSQL.reserve( 500);
	stValues.reserve( 500 );

	stStmSQL = "INSERT INTO ";
	stStmSQL.append (M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_REAL_OBJECT].Value.GetWithOutSec ()));
	stStmSQL.append (" (");

	if( m_indRealTbl == 0 )
	{
		// Sólo para la tabla principal
		stStmSQL.append ("OPERATION, ");
		stValues.append (ai_pccOperation);
		stValues.append (", ");
	}

	m4bool_t bFirst = M4_TRUE;
	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];

		/* Bug 0157038
		El campo OPERATION no debe aparecer porque ya se pone manualmente
		*/
		eFldType_t eIntFld = m_pParam->GetInfoIdInternalField (indFld);

		if( eIntFld == M4LDB_ID_INHERIT_OPERATION )
		{
			continue;
		}

		if (!bFirst)
		{
			stStmSQL.append (", ");
			stValues.append (", ");
		}
		else
		{
			bFirst = M4_FALSE;
		}

		m4pcchar_t pRealField = m_pParam->GetInfoRealField (indFld);

		stStmSQL.append (pRealField);
		stValues.append ("?");
	}

	stStmSQL.append (") VALUES (");
	stStmSQL.append (stValues);
	stStmSQL.append (")");

	ClLoad *pCursor = new ClLoad (m_pGlobVars, M4_TRUE, this);

	result = pCursor->Open (m_idRConn, NULL, M4_FALSE, m_pParam);

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	result = pCursor->Prepare ((m4pchar_t) stStmSQL.c_str ());

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_INHERIT_INSERT, pCursor));

	return( M4_SUCCESS ) ;
}


m4return_t ClPersistBase::PrepareInheritValidate (void)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Inherit Validate.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Inherit Validate.");
	#endif

	string stStmSQL;
	stStmSQL.reserve( 500 );

	stStmSQL = "SELECT O.ID_ORGANIZATION, ";

	m4uint8_t iLanguage = m_pGlobVars->GetLanguage();
	m4uint8_t iDefaultLanguage = m_pGlobVars->GetpLConn()->GetpLdb()->GetDefaultLanguage();

	if( iDefaultLanguage >= M4CL_LANGUAGE_NUMBER )
	{
		iDefaultLanguage = M4_LANG_DEFAULT_ID;
	}

	if( iLanguage >= M4CL_LANGUAGE_NUMBER )
	{
		iLanguage = iDefaultLanguage;
	}

	if( iLanguage != iDefaultLanguage )
	{
		ClLConn *pLConn = m_pGlobVars->GetpLConn ();
		eDDBBType_t eDDBB = M4DBW_ORACLE_DDBB;
		
		if (pLConn->GetConnManager()->GetDDBBType(m_idRConn,pLConn,eDDBB)!=M4_SUCCESS){
			return M4_ERROR;
		}

		_ClNVLTranslator *pNvl= ClTraductor::s_ppTrUtils[eDDBB-1]->m_pNVLTrans;

		pNvl->AddNVL( stStmSQL );
		stStmSQL.append( "(O.NM_ORGANIZATION" );
		stStmSQL.append( g_ppcLangCodes[ iLanguage ] );
		stStmSQL.append( ", " );
	}

	stStmSQL.append( "O.NM_ORGANIZATION" );
	stStmSQL.append( g_ppcLangCodes[ iDefaultLanguage ] );

	if( iLanguage != iDefaultLanguage )
	{
		stStmSQL.append( ")" );
	}

	stStmSQL.append(" FROM M4RCH_ORGANIZATION O, ");
	stStmSQL.append(M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_REAL_OBJECT].Value.GetWithOutSec ()));
	stStmSQL.append(" T");
	stStmSQL.append(" WHERE T.OPERATION <> 0 AND ");

	m4bool_t bFirst = M4_TRUE;
	m4bool_t bExistsPK = M4_FALSE;

	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

		if (!bIsPK)
		{
			continue;
		}

		// Es PK.
		bExistsPK = M4_TRUE;

		if (!bFirst)
		{
			stStmSQL.append (" AND ");
		}
		else
		{
			bFirst = M4_FALSE;
		}

		m4pcchar_t pRealField = m_pParam->GetInfoRealField (indFld);
		stStmSQL.append ("T.");
		stStmSQL.append (pRealField);

		if( m_pParam->GetInfoIdInternalField( indFld ) == M4LDB_ID_ORGANIZATION )
		{
			stStmSQL.append (" IN (SELECT OT.ID_ORGANIZATION FROM M4VCH_ORG_TREE OT WHERE OT.ID_ORGANIZATION <> OT.ID_PAR_ORGANIZATIO AND OT.ID_PAR_ORGANIZATIO = ?)");
			stStmSQL.append (" AND O.ID_ORGANIZATION = T.");
			stStmSQL.append (pRealField);
		}
		else
		{
			stStmSQL.append (" = ?");
		}
	}

	if (!bExistsPK) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_NO_PK_FIELDS, m_pMtLgcTbl->m_idLgcTbl);
		return (M4_ERROR);
	}

	ClLoad *pCursor = new ClLoad (m_pGlobVars, M4_TRUE, this);

	result = pCursor->Open (m_idRConn, NULL, M4_FALSE, m_pParam);

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	result = pCursor->Prepare ((m4pchar_t) stStmSQL.c_str ());

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_INHERIT_VALIDATE, pCursor));

	return( M4_SUCCESS ) ;
}


m4return_t ClPersistBase::PrepareTranslationInsert (void)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Translation Insert.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Translation Insert.");
	#endif

	string stStmSQL;
	string stValues;

	stStmSQL.reserve( 500);
	stValues.reserve( 500 );

	ClItemIndex iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_REAL_OBJECT ;

	// El el caso de tramitación hay que usar la tabla espejo
	if( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
	{
		iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_MIRROR_REAL_OBJECT;
	}

	stStmSQL = "INSERT INTO ";
	stStmSQL.append (M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[iTableName].Value.GetWithOutSec ()));
	stStmSQL.append (" (ID_LANGUAGE#, ");

	m4char_t acBuffer[ 128 + 1 ];
	sprintf(acBuffer, "%d, ", m_pGlobVars->GetLanguage()); 
	stValues.append (acBuffer);

	m4bool_t bFirst = M4_TRUE;
	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

		if (!bIsPK)
		{
			continue;
		}

		if (!bFirst)
		{
			stStmSQL.append (", ");
			stValues.append (", ");
		}
		else
		{
			bFirst = M4_FALSE;
		}

		m4pcchar_t pRealField = m_pParam->GetInfoRealField (indFld);

		stStmSQL.append (pRealField);
		stValues.append ("?");
	}

	stStmSQL.append (") VALUES (");
	stStmSQL.append (stValues);
	stStmSQL.append (")");

	ClLoad *pCursor = new ClLoad (m_pGlobVars, M4_TRUE, this);

	result = pCursor->Open (m_idRConn, NULL, M4_FALSE, m_pParam);

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	result = pCursor->Prepare ((m4pchar_t) stStmSQL.c_str ());

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_TRANSLATION_INSERT, pCursor));

	return( M4_SUCCESS ) ;
}


m4return_t ClPersistBase::PrepareTranslationValidatePK (void)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Translation Validate Primary Key.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Translation Validate Primary Key.");
	#endif

	ClLConn *pLConn = m_pGlobVars->GetpLConn ();
	eDDBBType_t eDDBB = M4DBW_ORACLE_DDBB;
	
	if (pLConn->GetConnManager()->GetDDBBType(m_idRConn,pLConn,eDDBB)!=M4_SUCCESS){
		return M4_ERROR;
	}

	_ClLockerTranslator *pLock = ClTraductor::s_ppTrUtils[eDDBB-1]->m_pLockTrans;

	string stStmSQL;
	stStmSQL.reserve( 500 );

	ClItemIndex iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_REAL_OBJECT ;

	// El el caso de tramitación hay que usar la tabla espejo
	if( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
	{
		iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_MIRROR_REAL_OBJECT;
	}

	stStmSQL = "SELECT 1 FROM ";
	stStmSQL.append (M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[iTableName].Value.GetWithOutSec ()));
	stStmSQL.append (" T");

	// Hacer un for update
	pLock->AddLockBetweenJoins(stStmSQL);

	stStmSQL.append (" WHERE ");

	m4bool_t bFirst = M4_TRUE;
	m4bool_t bExistsPK = M4_FALSE;
	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

		if (!bIsPK)
		{
			continue;
		}

		// Es PK.
		bExistsPK = M4_TRUE;

		if (!bFirst)
		{
			stStmSQL.append (" AND ");
		}
		else
		{
			bFirst = M4_FALSE;
		}

		m4pcchar_t pRealField = m_pParam->GetInfoRealField (indFld);
		stStmSQL.append ("T.");
		stStmSQL.append (pRealField);
		stStmSQL.append (" = ?");
	}

	if (!bExistsPK) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_NO_PK_FIELDS, m_pMtLgcTbl->m_idLgcTbl);
		return (M4_ERROR);
	}

	// Hacer un for update
	pLock->AddLockForUpdate(stStmSQL);

	ClLoad *pCursor = new ClLoad (m_pGlobVars, M4_TRUE, this);

	result = pCursor->Open (m_idRConn, NULL, M4_FALSE, m_pParam);

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	result = pCursor->Prepare ((m4pchar_t) stStmSQL.c_str ());

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_TRANSLATION_VALIDATE_PK, pCursor));

	return( M4_SUCCESS ) ;
}


m4return_t ClPersistBase::PrepareTranslationUpdatePK (void)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Translation Update Primary key.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Translation Update Primary key.");
	#endif

	string stStmSQL;
	stStmSQL.reserve( 500 );

	ClItemIndex iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_REAL_OBJECT ;

	// El el caso de tramitación hay que usar la tabla espejo
	if( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
	{
		iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_MIRROR_REAL_OBJECT;
	}

	stStmSQL = "UPDATE ";
	stStmSQL.append (M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[iTableName].Value.GetWithOutSec ()));
	stStmSQL.append (" SET ");

	m4bool_t bFirst = M4_TRUE;
	m4bool_t bExistsPK = M4_FALSE;
	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

		if (!bIsPK)
		{
			continue;
		}

		if (!bFirst)
		{
			stStmSQL.append (", ");
		}
		else
		{
			bFirst = M4_FALSE;
		}

		m4pcchar_t pRealField = m_pParam->GetInfoRealField (indFld);

		stStmSQL.append (pRealField);
		stStmSQL.append (" = ?");
	}

	stStmSQL.append (" WHERE ");

	bFirst = M4_TRUE;

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

		if (!bIsPK)
		{
			continue;
		}

		// Es PK.
		bExistsPK = M4_TRUE;

		if (!bFirst)
		{
			stStmSQL.append (" AND ");
		}
		else
		{
			bFirst = M4_FALSE;
		}

		m4pcchar_t pRealField = m_pParam->GetInfoRealField (indFld);
		stStmSQL.append (pRealField);
		stStmSQL.append (" = ?");
	}

	if (!bExistsPK) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_NO_PK_FIELDS, m_pMtLgcTbl->m_idLgcTbl);
		return (M4_ERROR);
	}

	ClLoad *pCursor = new ClLoad (m_pGlobVars, M4_TRUE, this);

	result = pCursor->Open (m_idRConn, NULL, M4_FALSE, m_pParam);

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	result = pCursor->Prepare ((m4pchar_t) stStmSQL.c_str ());

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_TRANSLATION_UPDATE_PK, pCursor));

	return( M4_SUCCESS ) ;
}


m4return_t ClPersistBase::PrepareUpdateVersion (void)
{
	m4return_t result;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Update Version.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Update Version.");
	#endif

	string stStmSQL;
	stStmSQL.reserve( 125 );

	stStmSQL = "UPDATE M4RCH_MD_VERSION SET DT_MD_VERSION = ? WHERE ID_MD_TYPE = 5 AND ID_T3 = '";
	stStmSQL.append (m_pMtLgcTbl->m_idLgcTbl);
	stStmSQL.append ("'");


	ClLoad *pCursor = new ClLoad (m_pGlobVars, M4_TRUE, this);

	result = pCursor->Open (m_idRConn, NULL, M4_FALSE, m_pParam);

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	result = pCursor->Prepare ((m4pchar_t) stStmSQL.c_str ());

	if (result == M4_ERROR) {
		delete pCursor;
		return (M4_ERROR);
	}

	m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_UPDATE_VERSION, pCursor));

	return( M4_SUCCESS ) ;
}


m4return_t ClPersistBase::PrepareSingleInsert (m4pcchar_t &ao_pccRealObject)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;

	string stStmSQL;
	stStmSQL.reserve( 1024 );

	stStmSQL = "INSERT INTO ";

	// Añadimos el nombre de la tabla física.
	ClItemIndex iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_REAL_OBJECT ;

	// El el caso de tramitación hay que usar la tabla espejo
	if( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
	{
		iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_MIRROR_REAL_OBJECT;
	}

	ao_pccRealObject = M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[iTableName].Value.GetWithOutSec ());

	/* Bug 0150594
	No se puede persistir sobre la vista de sociedades y se cambia por la tabla
	*/
	if( strcmpi( ao_pccRealObject, "M4VCH_ORGANIZATION" ) == 0 )
	{
		ao_pccRealObject = "M4RCH_ORGANIZATION" ;
	}

	stStmSQL.append (ao_pccRealObject);

	// Traducimos y añadimos la lista de campos.
	stStmSQL.append (" (");

	m4uint32_t iIsTranslation = 0;

	if( m_indRealTbl != 0 )
	{
		iIsTranslation = M4LDB_NUMBER( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_IS_TRANSLATION].Value.GetWithOutSec() );

		if( iIsTranslation == 1 )
		{
			// Si es de traducciones hay que insertar el lenguaje
			stStmSQL.append ("ID_LANGUAGE#, ");
		}
	}
	else
	{
		if( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
		{
			stStmSQL.append ("TRAMIT_OPERATION, TRAMIT_DT_LAST_UPDATE, ");
		}
	}

	m4int32_t numPar = m_oVParam.size ();

	for( indPar = 0; indPar < numPar; indPar++ )
	{
		indFld = m_oVParam[indPar];

		if (indPar > 0)
		{
			stStmSQL.append (", ");
		}

		m4pcchar_t pRealField = m_pParam->GetInfoRealField (indFld);
		stStmSQL.append (pRealField);

		/* Bug 0156331
		En el insert también se pone el TRAMIT_DT_START
		*/
		if( m_indRealTbl == 0 && m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
		{
			eFldType_t eIntFld = m_pParam->GetInfoIdInternalField (indFld);

			if( eIntFld == M4LDB_DT_START )
			{
				stStmSQL.append (", TRAMIT_DT_START");
			}
		}

		if (m_eStmType == M4LDB_DUPLICATE && 
			(indFld == m_pMtLgcTbl->m_indDTStart || 
			indFld == m_pMtLgcTbl->m_indDTEnd || 
			indFld == m_pMtLgcTbl->m_indDTStartCorr || 
			indFld == m_pMtLgcTbl->m_indDTEndCorr || 
			indFld == m_pMtLgcTbl->m_indRegType))
		{
			m_pParam->CreateOldData (indFld);
		}
	}

	// Values...
	stStmSQL.append (") VALUES (");

	if( m_indRealTbl != 0 )
	{
		if( iIsTranslation == 1 )
		{
			// Si es de traducciones hay que insertar el lenguaje
			m4char_t acBuffer[ 128 + 1 ];
			sprintf(acBuffer, "%d, ", m_pGlobVars->GetLanguage());
			stStmSQL.append (acBuffer);
		}
	}
	else
	{
		if( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
		{
			//Del TRAMIT_OPERATION y el TRAMIT_DT_LAST_UPDATE
			stStmSQL.append ("?, ?, ");
		}
	}

	for( indPar = 0; indPar < numPar; indPar++ )
	{
		indFld = m_oVParam[indPar];

		if (indPar > 0)
		{
			stStmSQL.append (", ");
		}

		stStmSQL.append ("?");

		/* Bug 0156331
		En el insert también se pone el TRAMIT_DT_START
		*/
		if( m_indRealTbl == 0 && m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
		{
			eFldType_t eIntFld = m_pParam->GetInfoIdInternalField (indFld);

			if( eIntFld == M4LDB_DT_START )
			{
				stStmSQL.append (", ?");
			}
		}
	}
	stStmSQL.append (")");


	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing %s.", g_aPersist[m_eStmType]);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing %s.", g_aPersist[m_eStmType]);
	#endif

	result = ClStmt::Prepare ((m4pchar_t) stStmSQL.c_str ());

	if (result == M4_ERROR)
	{
		return (M4_ERROR);
	}

	return (result);
}


m4return_t ClPersistBase::PrepareInsert (void)
{
	m4return_t result;
	m4pcchar_t pRealObject = NULL;

	if (!m_pMtLgcTbl->m_bAudStmt)
	{
		// Esto va delante por el tema de los ordinales.
		result = PrepareValidateFKs ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}
	}

	result = PrepareSingleInsert(pRealObject);

	if( result == M4_ERROR )
	{
		return( M4_ERROR );
	}
	
	if (m_eStmType == M4LDB_DUPLICATE)
	{
		// Vamos a montar el cursor que nos servirá para traernos los datos.
		result = PrepareValidatePK ();

		if (result == M4_ERROR)
		{
			return (M4_ERROR);
		}
	}

	// Vamos a montar el cursor de validación de No PK.
	result = PrepareValidateNoPK ();

	if (result == M4_ERROR)
	{
		return (M4_ERROR);
	}

	if (!m_indRealTbl)
	{
		// Sólo la principal.
		if (!m_pMtLgcTbl->m_bAudStmt)
		{
			if (m_pMtLgcTbl->m_eCurOrg == M4LDB_MULTI_ORGANIZATION)
			{
				result = PrepareValidateOrganization ();

				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}
			}

			if (m_pMtSecSent)
			{
				result = PrepareValidateSecurity ();

				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}
			}

			if (m_pMtSent != NULL && m_pMtSent->GetIsInsertIntoSelect () == M4_TRUE)
			{
				// Vamos a montar el cursor de query.
				result = PrepareCursorQuery ();

				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}
			}

			if (m_pMtLgcTbl->m_eCurTemp != M4LDB_ETHERN)
			{
				// Vamos a montar el cursor historico.
				result = PrepareGetHistory ();
				
				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}
			}

			if (m_pMtLgcTbl->m_bCorrected)
			{
				// Vamos a montar el cursor historico con corrección.
				result = PrepareGetCorrectedHistory ();
				
				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}

				// Vamos a montar el cursor de validación de FR.
				result = PrepareValidateFRs ();

				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}
			}
		}
	}

	m4uint32_t iIsTranslation = 0;

	if( m_indRealTbl != 0 )
	{
		iIsTranslation = M4LDB_NUMBER( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_IS_TRANSLATION].Value.GetWithOutSec() );
	}

	// Tramitación
	if( iIsTranslation == 0 )
	{
		if( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
		{
			result = PrepareTramitExist( M4_FALSE, M4_FALSE, m_indRealTbl == 0 ) ;

			if( result == M4_ERROR )
			{
				return( M4_ERROR ) ;
			}

			result = PrepareTramitDelete() ;

			if( result == M4_ERROR )
			{
				return( M4_ERROR ) ;
			}

			//Sólo para la tabla principal y si es la padre de herencia
			if( m_indRealTbl == 0 && m_idMainTable != NULL )
			{
				result = PrepareTramitInsertObject() ;
		
				if( result == M4_ERROR )
				{
					return( M4_ERROR ) ;
				}
			}
		}

		// Herencia de datos canales mono y multi
		if( m_bUseInheritance == M4_TRUE && m_pMtLgcTbl->m_bInheritsData == M4_TRUE )
		{
			result = PrepareInheritExist( M4_FALSE, M4_TRUE ) ;

			if( result == M4_ERROR )
			{
				return( M4_ERROR ) ;
			}

			result = PrepareInheritDelete() ;

			if( result == M4_ERROR )
			{
				return( M4_ERROR ) ;
			}
		}
	}

	return (result);
}


m4return_t ClPersistBase::PrepareSingleUpdate (void)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;
	m4bool_t bIsAPISQL = M4_FALSE;
	ClStmt *pCursor = NULL;

	if (m_pMtSent)
	{
		bIsAPISQL = m_pMtSent->IsAPISQL ();
	}

	string stStmSQL ("UPDATE ");

	// Añadimos el nombre de la tabla física.
	ClItemIndex iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_REAL_OBJECT ;

	// El el caso de tramitación hay que usar la tabla espejo
	if( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
	{
		iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_MIRROR_REAL_OBJECT;
	}

	m4pcchar_t pRealObject = M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[iTableName].Value.GetWithOutSec ());

	/* Bug 0150594
	No se puede persistir sobre la vista de sociedades y se cambia por la tabla
	*/
	if( strcmpi( pRealObject, "M4VCH_ORGANIZATION" ) == 0 )
	{
		pRealObject = "M4RCH_ORGANIZATION" ;
	}

	stStmSQL.append (pRealObject);

	// Traducimos y añadimos la lista de campos.
	stStmSQL.append (" SET ");

	m4uint32_t iIsTranslation = 0;

	if( m_indRealTbl != 0 )
	{
		iIsTranslation = M4LDB_NUMBER( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_IS_TRANSLATION].Value.GetWithOutSec() );
	}

	m4bool_t bFirst = M4_TRUE;
	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);
		m4int32_t hItem = m_pParam->GetItemHandle (indFld);

		if ((!bIsPK || bIsAPISQL || !m_pMtSent || (indFld == m_pMtLgcTbl->m_indDTStart && m_pMtSent))
			&& 
		    (hItem || (m_pParam->GetCurrentLanguageIndex(indFld) != M4LDB_NULL_INDEX && m_pParam->GetInfoM4Type(indFld) != M4CL_M4_TYPE_BLOB)))
		{
			// Es susceptible de actualización todo lo que tenga handle y :
			// 1.- No es PK.
			// 2.- Viene de APISQL.
			// 3.- Es una operación interna.
			// 4.- La DT_START y sólo para operaciones externas.
			// 5.- Si el campo es el del lenguaje por defecto y estamos trabajando con otro lenguaje (pero no para blobs)

			if (hItem)
			{
				m_pParam->CreateOldData (indFld);
			}

			if (bIsPK)
			{
				m_pMtLgcTbl->m_bUpdatePK = M4_TRUE;
			}

			if (!bFirst)
			{
				stStmSQL.append (", ");
			}
			else
			{
				bFirst = M4_FALSE;
			}

			m4pcchar_t pRealField = m_pParam->GetInfoRealField (indFld);

			stStmSQL.append (pRealField);
			stStmSQL.append (" = ?");
		}
	}

	if (bFirst)
	{
		DUMP_LDBINSP_DEBUGF (m_pGlobVars->GetpLConn (), M4_CH_LDB_NO_FIELDS_FOR_UPDATE, m_pMtLgcTbl->m_idLgcTbl << (m4int32_t) m_indRealTbl);

		// Esto es necesario para mantener el bloqueo aunque no se hagan cambios.
		m4pcchar_t pRealField = m_pParam->GetInfoRealField (m_oVParam[0]);

		stStmSQL.append (pRealField);
		stStmSQL.append (" = ");
		stStmSQL.append (pRealField);
	}

	// Traducimos y añadimos el filtro.
	stStmSQL.append (" WHERE ");

	if( m_bUseTranslation == M4_TRUE && iIsTranslation == 1 )
	{
		// Si es de traducciones hay que insertar el lenguaje
		m4char_t acBuffer[ 128 + 1 ];
		sprintf(acBuffer, "ID_LANGUAGE# = %d AND ", m_pGlobVars->GetLanguage()); 
		stStmSQL.append (acBuffer);
	}

	bFirst = M4_TRUE;

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

		if (bIsPK)
		{
			// Es PK.
			if (!bFirst)
			{
				stStmSQL.append (" AND ");
			}
			else
			{
				bFirst = M4_FALSE;
			}

			m4pcchar_t pRealField = m_pParam->GetInfoRealField (indFld);

			stStmSQL.append (pRealField);
			stStmSQL.append (" = ?");
		}
	}

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing %s.", g_aPersist[m_eStmType]);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing %s.", g_aPersist[m_eStmType]);
	#endif

	result = ClStmt::Prepare ((m4pchar_t) stStmSQL.c_str ());

	if (result == M4_ERROR)
	{
		return (M4_ERROR);
	}

	return (result);
}


m4return_t ClPersistBase::PrepareUpdate (void)
{
	m4return_t result;
	m4bool_t bIsAPISQL = M4_FALSE;

	m4uint32_t iIsTranslation = 0;

	if( m_indRealTbl != 0 )
	{
		iIsTranslation = M4LDB_NUMBER( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_IS_TRANSLATION].Value.GetWithOutSec() );
	}

	if( iIsTranslation == 1 && m_bUseTranslation == M4_TRUE && m_pMtLgcTbl->m_bUpdatePK == M4_TRUE )
	{
		result = PrepareTranslationValidatePK() ;

		if( result == M4_ERROR )
		{
			return( M4_ERROR ) ;
		}

		result = PrepareTranslationUpdatePK() ;

		if( result == M4_ERROR )
		{
			return( M4_ERROR ) ;
		}

		if( m_pGlobVars->GetpLConn()->IsDynamicLanguage( m_pGlobVars->GetLanguage() ) == M4_FALSE )
		{
			result = PrepareValidateNoPK() ;

			if( result == M4_ERROR )
			{
				return( M4_ERROR ) ;
			}

			return( M4_SUCCESS ) ;
		}
	}

	if (m_pMtSent)
	{
		bIsAPISQL = m_pMtSent->IsAPISQL ();
	}

	// Esto va delante por el tema de los ordinales.
	result = PrepareValidateFKs ();

	if (result == M4_ERROR)
	{
		return (M4_ERROR);
	}

	result = PrepareSingleUpdate();

	if( result == M4_ERROR )
	{
		return( M4_ERROR );
	}

	// Vamos a montar el cursor de validación de PK.
	result = PrepareValidatePK ();

	if (result == M4_ERROR)
	{
		return (M4_ERROR);
	}

	if (m_pMtLgcTbl->m_bUpdatePK)
	{
		// Vamos a montar el cursor de validación No de PK.
		result = PrepareValidateNoPK ();

		if (result == M4_ERROR)
		{
			return (M4_ERROR);
		}
	}
	
	if (!m_indRealTbl)
	{
		// Sólo la principal.
		if (m_pMtLgcTbl->m_eCurOrg == M4LDB_MULTI_ORGANIZATION)
		{
			result = PrepareValidateOrganization ();

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}

		if (m_pMtSecSent)
		{
			result = PrepareValidateSecurity ();

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}

		if (bIsAPISQL)
		{
			// Vamos a montar el cursor de query.
			result = PrepareCursorQuery ();

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}

		if (m_pMtLgcTbl->m_eCurTemp != M4LDB_ETHERN)
		{
			// Vamos a montar el cursor historico.
			result = PrepareGetHistory ();
			
			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}

		if (m_pMtLgcTbl->m_bCorrected)
		{
			// Vamos a montar el cursor historico con corrección.
			result = PrepareGetCorrectedHistory ();
			
			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}
	}

	// Vamos a montar el cursor de validación de FR.
	result = PrepareValidateFRs ();

	if (result == M4_ERROR)
	{
		return (M4_ERROR);
	}

	if( iIsTranslation == 0 )
	{
		// Tramitación
		if( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
		{
			result = PrepareTramitExist( M4_FALSE, M4_TRUE, M4_FALSE ) ;

			if( result == M4_ERROR )
			{
				return( M4_ERROR ) ;
			}

			result = PrepareTramitInsert( "2" ) ;

			if( result == M4_ERROR )
			{
				return( M4_ERROR ) ;
			}

			//Sólo para la tabla principal y si es la padre de herencia
			if( m_indRealTbl == 0 && m_idMainTable != NULL )
			{
				result = PrepareTramitInsertObject() ;

				if( result == M4_ERROR )
				{
					return( M4_ERROR ) ;
				}
			}
		}

		// Herencia de datos canales mono y multi
		if( m_bUseInheritance == M4_TRUE && m_pMtLgcTbl->m_bInheritsData == M4_TRUE  )
		{
			result = PrepareInheritExist( M4_FALSE, M4_TRUE ) ;

			if( result == M4_ERROR )
			{
				return( M4_ERROR ) ;
			}

			result = PrepareInheritInsert( "1" ) ;

			if( result == M4_ERROR )
			{
				return( M4_ERROR ) ;
			}
		}
	}
	else if( m_bUseTranslation == M4_TRUE )
	{
		// Traducciones
		result = PrepareTranslationInsert() ;

		if( result == M4_ERROR )
		{
			return( M4_ERROR ) ;
		}
	}

	return (result);
}


m4return_t ClPersistBase::PrepareSingleDelete (void)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;


	string stStmSQL ("DELETE FROM ");

	// Añadimos el nombre de la tabla física.
	ClItemIndex iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_REAL_OBJECT ;

	// El el caso de tramitación hay que usar la tabla espejo
	if( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
	{
		iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_MIRROR_REAL_OBJECT;
	}

	m4pcchar_t pRealObject = M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[iTableName].Value.GetWithOutSec ());

	/* Bug 0150594
	No se puede persistir sobre la vista de sociedades y se cambia por la tabla
	*/
	if( strcmpi( pRealObject, "M4VCH_ORGANIZATION" ) == 0 )
	{
		pRealObject = "M4RCH_ORGANIZATION" ;
	}

	stStmSQL.append (pRealObject);

	// Traducimos y añadimos el filtro.
	stStmSQL.append (" WHERE ");

	m4bool_t bFirst = M4_TRUE;
	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];

		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

		if (bIsPK)
		{
			// Es PK.

			if (!bFirst)
			{
				stStmSQL.append (" AND ");
			}
			else
			{
				bFirst = M4_FALSE;
			}

			m4pcchar_t pRealField = m_pParam->GetInfoRealField (indFld);
			stStmSQL.append (pRealField);
			stStmSQL.append (" = ?");
		}
		else
		{
			if(
				( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
				||
				( m_bUseInheritance == M4_TRUE && m_pMtLgcTbl->m_bInheritsData == M4_TRUE )
				)
			{
				// En herencia y en tramitación hay que crear un old value para que la
				// sentencia de validación se traiga el valor sin machacar el actual porque luego se necesita
				// Esto aplica al role, usuario y fecha de última actualización
				eFldType_t eIntFld = m_pParam->GetInfoIdInternalField( indFld ) ;

				if( eIntFld == M4LDB_APP_ROLE || eIntFld == M4LDB_SEC_USER || eIntFld == M4LDB_DT_LAST_UPDATE )
				{
					m_pParam->CreateOldData( indFld ) ;
				}
			}
		}
	}


	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing %s.", g_aPersist[m_eStmType]);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Preparing %s.", g_aPersist[m_eStmType]);
	#endif

	result = ClStmt::Prepare ((m4pchar_t) stStmSQL.c_str ());

	if (result == M4_ERROR)
	{
		return (M4_ERROR);
	}

	return (result);
}



m4return_t ClPersistBase::PrepareDelete (void)
{
	m4return_t result;

	m4bool_t bNoOptimization = M4_TRUE;
	if (m_pMtSent && m_pMtSent->HasValidation() == M4_FALSE)
	{
		// Para que haya optimización en la no validación tiene que ser un objeto logico con una 
		// sola tabla real y no tener herencia.
		if (m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Count() == 1 && !m_pMtSent->ExistsInheritance())
		{
			bNoOptimization = M4_FALSE;
		}
	}

	/* Bug 0154924
	Si estamos en tramitación o herencia se desactiva la optimización
	*/
	if( bNoOptimization == M4_FALSE )
	{
		if(
			( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
			||
			( m_bUseInheritance == M4_TRUE && m_pMtLgcTbl->m_bInheritsData == M4_TRUE )
			)
		{
			bNoOptimization = M4_TRUE ;
		}
	}

	if (bNoOptimization == M4_TRUE)
	{
		result = PrepareSingleDelete();

		if( result == M4_ERROR )
		{
			return( M4_ERROR );
		}

		// Vamos a montar el cursor de validación de PK.
		result = PrepareValidatePK ();

		if (result == M4_ERROR)
		{
			return (M4_ERROR);
		}

		if (!m_indRealTbl)
		{
			// Sólo la principal.
			if (m_pMtLgcTbl->m_eCurOrg == M4LDB_MULTI_ORGANIZATION)
			{
				result = PrepareValidateOrganization ();

				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}
			}

			if (m_pMtSecSent)
			{
				result = PrepareValidateSecurity ();

				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}
			}

			if (m_pMtSent && m_pMtSent->IsAPISQL ())
			{
				// Vamos a montar el cursor de query.
				result = PrepareCursorQuery ();

				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}
			}

			if (m_pMtLgcTbl->m_eCurTemp != M4LDB_ETHERN && m_pMtLgcTbl->m_eCurTemp != M4LDB_1_PERIOD)
			{
				// Vamos a montar el cursor historico.
				result = PrepareGetHistory ();
				
				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}
			}

			if (m_pMtLgcTbl->m_bCorrected)
			{
				// Vamos a montar el cursor historico con corrección.
				result = PrepareGetCorrectedHistory ();
				
				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}
			}
		}

		// Vamos a montar el cursor de validación de FR.
		result = PrepareValidateFRs ();

		if (result == M4_ERROR)
		{
			return (M4_ERROR);
		}

		
		m4uint32_t	iIsTranslation = 0;

		if( m_indRealTbl != 0 )
		{
			iIsTranslation = M4LDB_NUMBER( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_IS_TRANSLATION].Value.GetWithOutSec() );
		}

		if( iIsTranslation == 0 )
		{
			// Tramitación
			if( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
			{
				result = PrepareTramitExist( M4_TRUE, M4_FALSE, M4_FALSE ) ;

				if( result == M4_ERROR )
				{
					return( M4_ERROR ) ;
				}

				result = PrepareTramitInsert( "0" ) ;

				if( result == M4_ERROR )
				{
					return( M4_ERROR ) ;
				}

				//Sólo para la tabla principal y si es la padre de herencia
				if( m_indRealTbl == 0 && m_idMainTable != NULL )
				{
					result = PrepareTramitInsertObject() ;
		
					if( result == M4_ERROR )
					{
						return( M4_ERROR ) ;
					}
				}
			}

			// Herencia de datos canales mono y multi
			if( m_bUseInheritance == M4_TRUE && m_pMtLgcTbl->m_bInheritsData == M4_TRUE )
			{
				result = PrepareInheritExist( M4_TRUE, M4_FALSE ) ;

				if( result == M4_ERROR )
				{
					return( M4_ERROR ) ;
				}

				result = PrepareInheritInsert( "0" ) ;

				if( result == M4_ERROR )
				{
					return( M4_ERROR ) ;
				}

				result = PrepareInheritDelete() ;

				if( result == M4_ERROR )
				{
					return( M4_ERROR ) ;
				}

				if( m_indRealTbl == 0 )
				{
					result = PrepareInheritValidate() ;

					if( result == M4_ERROR )
					{
						return( M4_ERROR ) ;
					}
				}
			}
		}
	}
	else
	{
		result = PrepareCursorQuery ("DELETE FROM ");

		if (result == M4_ERROR)
		{
			return (M4_ERROR);
		}
	}

	return (result);
}


m4return_t ClPersistBase::BindParam (m4bool_t ai_bBlobAsBinary)
{
	m4return_t result = M4_ERROR;

	switch (m_eStmType)
	{
		case M4LDB_INSERT:
		case M4LDB_DUPLICATE:
			result = BindParamInsert(ai_bBlobAsBinary);
			break;

		case M4LDB_UPDATE:
			result = BindParamUpdate();
			break;

		case M4LDB_DELETE:
			result = BindParamDelete();
			break;
	}

	if( result == M4_ERROR )
	{
		return( M4_ERROR ) ;
	}

	if( m_indRealTbl == 0 && m_pMtLgcTbl->m_bIsCacheable == M4_TRUE )
	{
		result = BindParamUpdateVersion() ;

		if( result == M4_ERROR )
		{
			return( M4_ERROR ) ;
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::BindParamOrdinalPKInsert (m4int32_t ai_indParam)
{
	m4return_t result;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Ordinal Primary Key Insert Lock Row [%ld].", ai_indParam);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Ordinal Primary Key Insert Lock Row [%ld].", ai_indParam);
	#endif

	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_INSERT_ORDINAL_PK + ai_indParam);

	if (!pCursor)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::BindParamOrdinalPKInsert" << "The cursor M4LDB_INSERT_ORDINAL_PK " << LogCat << ai_indParam << LogCat << " does not exist");
		return (M4_ERROR);
	}

	ClParam*	pParamPK = GetClParamByType (M4LDB_ODINAL_PK);

	if (pParamPK == NULL)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::BindParamOrdinalPKInsert" << "The parameter M4LDB_ODINAL_PK does not exist");
		return (M4_ERROR);
	}

	result = pCursor->ClStmt::BindParam (1, pParamPK->m_pData, NULL, pParamPK->m_lPrec, pParamPK->m_m4Type, pParamPK->m_lScale, pParamPK->m_plLength );

	if (result == M4_ERROR)
	{
		return (M4_ERROR);
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::BindParamOrdinalPKDelete (m4int32_t ai_indParam)
{
	m4return_t result;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Ordinal Primary Key Delete Lock Row [%ld].", ai_indParam);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Ordinal Primary Key Delete Lock Row [%ld].", ai_indParam);
	#endif

	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_DELETE_ORDINAL_PK + ai_indParam);

	if (!pCursor)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::BindParamOrdinalPKDelete" << "The cursor M4LDB_DELETE_ORDINAL_PK " << LogCat << ai_indParam << LogCat << " does not exist");
		return (M4_ERROR);
	}

	ClParam*	pParamPK = GetClParamByType (M4LDB_ODINAL_PK);

	if (pParamPK == NULL)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::BindParamOrdinalPKDelete" << "The parameter M4LDB_ODINAL_PK does not exist");
		return (M4_ERROR);
	}

	result = pCursor->ClStmt::BindParam (1, pParamPK->m_pData, NULL, pParamPK->m_lPrec, pParamPK->m_m4Type, pParamPK->m_lScale, pParamPK->m_plLength ) ;

	if (result == M4_ERROR)
	{
		return (M4_ERROR);
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::BindParamOrdinalPK (m4int32_t ai_indParam, m4bool_t ai_bExcludeStartDates)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;
	m4int32_t indParam = 0;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Ordinal Primary Key [%ld].", ai_indParam);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Ordinal Primary Key [%ld].", ai_indParam);
	#endif

	// Bindado de los parámetros del cursor OrdinalPK.
	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_CURSOR_ORDINAL_PK + ai_indParam);

	if (!pCursor)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::BindParamOrdinalPK" << "The cursor M4LDB_CURSOR_ORDINAL_PK " << LogCat << ai_indParam << LogCat << " does not exist");
		return (M4_ERROR);
	}

	if (!m_pMtLgcTbl->m_bAudStmt)
	{
		m4int32_t numPar = m_oVParam.size ();

		for (indPar=0; indPar<numPar; indPar++)
		{
			indFld = m_oVParam[indPar];
			m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

			/* Bug 0266748
			Las fechas de inicio no se excluyen
			*/
			if (bIsPK && indFld != ai_indParam && indFld != m_pMtLgcTbl->m_indDataSet)
			{
				// Bug 0302129 Se excluyen las fechas si se especifica
				if (ai_bExcludeStartDates == M4_TRUE && (indFld == m_pMtLgcTbl->m_indDTStart || indFld == m_pMtLgcTbl->m_indDTStartCorr))
				{
					continue;
				}

				// Es PK y no es ni el dataset o es el ordinal y no es ni una de las fechas si se excluyen

				/* Bug 0173558
				En herencia la organización se ignora para el cálculo de ordinal
				*/
				if( m_pMtLgcTbl->m_bInheritsData == M4_TRUE && m_pParam->GetInfoIdInternalField( indFld ) == M4LDB_ID_ORGANIZATION )
				{
					continue;
				}

				result = pCursor->ClStmt::BindParam (indParam + 1, indFld);

				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}

				indParam++;
			}
		}
	}
	else
	{
		m4int32_t indIdExec = m_pParam->FindIndexByIdInternalField (M4LDB_ID_EXECUTION);

		if (indIdExec == M4LDB_NULL_INDEX)
		{
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, M4LDB_ID_EXECUTION << m_pMtLgcTbl->m_idLgcTbl);
			return (M4_ERROR);
		}

		result = pCursor->ClStmt::BindParam (indParam + 1, indIdExec);

		if (result == M4_ERROR)
		{
			return (M4_ERROR);
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::BindParamValidatePK (void)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;
	m4int32_t indParam = 0;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Validate Primary Key.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Validate Primary Key.");
	#endif

	// Bindado de los parámetros del cursor ValidatePK.
	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_VALIDATE_PK);

	if (!pCursor) {
		// El cursor tiene que existir.
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::BindParamValidatePK" << "The cursor M4LDB_VALIDATE_PK doesn't exist");
		return (M4_ERROR);
	}

	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++) {

		indFld = m_oVParam[indPar];

		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

		if (bIsPK) {
			// Es PK.

			m4bool_t bOldData = m_pMtLgcTbl->m_bUpdatePK;

			if (m_eStmType == M4LDB_DUPLICATE) {
				bOldData = M4_TRUE;
			}

			result = pCursor->ClStmt::BindParam (indParam + 1, indFld, bOldData);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}

			indParam++;
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::BindParamValidateNoPK (void)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;
	m4int32_t indParam = 0;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Validate No Primary Key.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Validate No Primary Key.");
	#endif

	// Bindado de los parámetros del cursor ValidateNoPK.
	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_VALIDATE_NO_PK);

	if (!pCursor) {
		// El cursor tiene que existir.
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::BindParamValidateNoPK" << "The cursor M4LDB_VALIDATE_NO_PK does not exist");
		return (M4_ERROR);
	}

	if (!m_pMtLgcTbl->m_bAudStmt) {
		m4int32_t numPar = m_oVParam.size ();

		for (indPar=0; indPar<numPar; indPar++) {

			indFld = m_oVParam[indPar];

			m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

			if (bIsPK) {
				// Es PK.

				result = pCursor->ClStmt::BindParam (indParam + 1, indFld);

				if (result == M4_ERROR) {
					return (M4_ERROR);
				}

				indParam++;
			}
		}
	}
	else {
		m4int32_t indIdExec = m_pParam->FindIndexByIdInternalField (M4LDB_ID_EXECUTION);

		if (indIdExec == M4LDB_NULL_INDEX) {
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, M4LDB_ID_EXECUTION << m_pMtLgcTbl->m_idLgcTbl);
			return (M4_ERROR);
		}

		result = pCursor->ClStmt::BindParam (indParam + 1, indIdExec);

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		indParam++;

		m4int32_t indIdOrder = m_pParam->FindIndexByIdInternalField (M4LDB_ID_ORDER);

		if (indIdOrder == M4LDB_NULL_INDEX) {
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, M4LDB_ID_ORDER << m_pMtLgcTbl->m_idLgcTbl);
			return (M4_ERROR);
		}

		result = pCursor->ClStmt::BindParam (indParam + 1, indIdOrder);

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::BindParamValidateFKs (void)
{
	m4return_t result;
	m4int32_t indFor = 0;
	m4int32_t indForFld = 0;

	m4int32_t numFor = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Count ();

	// Bindado de los parámetros del cursor ValidateFK.
	for (indFor=0; indFor<numFor; indFor++) {

		ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_VALIDATE_FK + (m4int32_t) indFor);

		if (!pCursor) {
			continue;
		}

		m4int32_t indParam = 0;

		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Validate Foreign Key [%ld].", (m4int32_t) indFor);
			M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Validate Foreign Key [%ld].", (m4int32_t) indFor);
		#endif

		m4int32_t numForFld = pCursor->m_pvcIndParVal->size ();

		for (indForFld=0; indForFld<numForFld; indForFld++) {

			PStIndPar pIndPar = (*pCursor->m_pvcIndParVal)[indForFld];

			if (pIndPar->m_bIsPK) {

				result = pCursor->ClStmt::BindParam (indParam + 1, pIndPar->m_indPar);

				if (result == M4_ERROR) {
					return (M4_ERROR);
				}

				indParam++;
			}
		}

		if (pCursor->m_bVerifyDTCorr) {

			result = pCursor->ClStmt::BindParam (indParam + 1, m_pMtLgcTbl->m_indDTStartCorr);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}

			indParam++;

			result = pCursor->ClStmt::BindParam (indParam + 1, m_pMtLgcTbl->m_indDTEndCorr);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::BindParamValidateFRs (void)
{
	m4return_t result;
	m4int32_t indFor;
	m4int32_t indForFld;

	m4int32_t numFor = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Count ();

	// Bindado de los parámetros del cursor ValidateFR.
	for (indFor=0; indFor<numFor; indFor++) {

		ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_VALIDATE_FR + (m4int32_t) indFor);

		if (!pCursor) {
			continue;
		}

		m4int32_t indParam = 0;

		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Validate Foreign Reference [%ld].", (m4int32_t) indFor);
			M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Validate Foreign Reference [%ld].", (m4int32_t) indFor);
		#endif

		m4int32_t numForFld = pCursor->m_pvcIndParVal->size ();

		for (indForFld=0; indForFld<numForFld; indForFld++) {

			PStIndPar pIndPar = (*pCursor->m_pvcIndParVal)[indForFld];

			if (pIndPar->m_bIsPK) {

				result = pCursor->ClStmt::BindParam (indParam + 1, pIndPar->m_indPar, m_pMtLgcTbl->m_bUpdatePK);

				if (result == M4_ERROR) {
					return (M4_ERROR);
				}

				indParam++;
			}
		}

		if (pCursor->m_bVerifyDTCorr) {

			result = pCursor->ClStmt::BindParam (indParam + 1, m_pMtLgcTbl->m_indDTStartCorr);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}

			indParam++;

			result = pCursor->ClStmt::BindParam (indParam + 1, m_pMtLgcTbl->m_indDTEndCorr);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::BindParamValidateComp (void)
{
	m4return_t result;
	m4int32_t indForFld = 0;

	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_VALIDATE_COMP);

	if (!pCursor) {
		return (M4_SUCCESS);
	}

	m4int32_t indParam = 0;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Validate Completeness.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Validate Completeness.");
	#endif

	m4int32_t numForFld = pCursor->m_pvcIndParVal->size ();

	for (indForFld=0; indForFld<numForFld; indForFld++) {

		PStIndPar pIndPar = (*pCursor->m_pvcIndParVal)[indForFld];

		if (pIndPar->m_bIsPK) {

			result = pCursor->ClStmt::BindParam (indParam + 1, pIndPar->m_indPar);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}

			indParam++;
		}
	}

	if (pCursor->m_bVerifyDTCorr) {

		result = pCursor->ClStmt::BindParam (indParam + 1, m_pMtLgcTbl->m_indDTStartCorr);

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		indParam++;

		result = pCursor->ClStmt::BindParam (indParam + 1, m_pMtLgcTbl->m_indDTEndCorr);

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::BindParamGetHistory (void)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;
	m4int32_t indParam = 0;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Get History.");
	#endif

	// Bindado de los parámetros del cursor GHS.
	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_GET_HISTORY);

	if (!pCursor) {
		// El cursor tiene que existir.
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::BindParamGetHistory" << "The cursor M4LDB_GET_HISTORY does not exist");
		return (M4_ERROR);
	}

	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++) {

		indFld = m_oVParam[indPar];

		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

		if ((bIsPK) && 
			(indFld != m_pMtLgcTbl->m_indDTStart)) {

			result = pCursor->ClStmt::BindParam (indParam + 1, indFld);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}

			indParam++;
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::BindParamGetCorrectedHistory (void)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;
	m4int32_t indParam = 0;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Get Corrected History.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Get Corrected History.");
	#endif

	// Bindado de los parámetros del cursor GCH.
	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_GET_CORR_HISTORY);

	if (!pCursor) {
		// El cursor tiene que existir.
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::BindParamGetCorrectedHistory" << "The cursor M4LDB_GET_CORR_HISTORY does not exist");
		return (M4_ERROR);
	}

	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++) {

		indFld = m_oVParam[indPar];

		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

		if ((bIsPK) &&
			(indFld != m_pMtLgcTbl->m_indDTStart) &&
			(indFld != m_pMtLgcTbl->m_indDTStartCorr)) {
			
			result = pCursor->ClStmt::BindParam (indParam + 1, indFld);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}

			indParam++;
		}
	}

	if (m_pMtLgcTbl->m_indBefCorrReg == M4LDB_NULL_INDEX) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::BindParamGetCorrectedHistory" << "The parameter BefCorrReg does not exist");
		return (M4_ERROR);
	}

	result = pCursor->ClStmt::BindParam (indParam + 1, m_pMtLgcTbl->m_indBefCorrReg);

	return (result);
}



m4return_t ClPersistBase::BindParamCursorQuery (void)
{
	m4return_t result;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Query.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Query.");
	#endif

	// Bindado de los parámetros del cursor CQY.
	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_CURSOR_QUERY);

	if (!pCursor) {
		// El cursor tiene que existir.
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::BindParamCursorQuery" << "The cursor M4LDB_CURSOR_QUERY does not exist");
		return (M4_ERROR);
	}

	result = pCursor->BindParam ();

	return (result);
}


m4return_t ClPersistBase::BindParamValidateSecurity (void)
{
	m4return_t result;

	// Bindado de los parámetros del cursor VSC.
	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_VALIDATE_SECURITY);

	if (!pCursor) {
		return (M4_SUCCESS);
	}

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Validate Security.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n + Binding Parameters Validate Security.");
	#endif

	result = pCursor->BindParam ((m_eStmType != M4LDB_UPDATE) ? M4_FALSE : M4_TRUE);

	return (result);
}

m4return_t ClPersistBase::BindParamValidateSecurityInOtherConnection (void)
{
	m4return_t result;

	// Bindado de los parámetros del cursor VSC.
	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_VALIDATE_SECURITY_OTHER_CONNECTION);

	if (!pCursor) {
		return (M4_SUCCESS);
	}

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Validate Security In Other Connection.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n + Binding Parameters Validate Security In Other Connection.");
	#endif

	result = pCursor->BindParam ((m_eStmType != M4LDB_UPDATE) ? M4_FALSE : M4_TRUE);

	return (result);
}

m4return_t ClPersistBase::BindParamAuditory (void)
{
	m4return_t result;
	m4bool_t bBlobAsBinary = M4_FALSE;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Auditory.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Auditory.");
	#endif

	ClPersist *pCursor = (ClPersist *) FindAuxCursor (M4LDB_CURSOR_AUDITORY);

	if (!pCursor) {
		// El cursor tiene que existir.
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::BindParamAuditory" << "The cursor M4LDB_CURSOR_AUDITORY does not exist");
		return (M4_ERROR);
	}

	/* Bug 0165266
	En el delete se bindan los blob como binarios porque se traen de base de datos
	*/
	if( m_eStmType == M4LDB_DELETE )
	{
		bBlobAsBinary = M4_TRUE ;
	}
	
	result = pCursor->BindParam (bBlobAsBinary);

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	if (m_eStmType == M4LDB_UPDATE) {
		ClPersist *pCursor2 = (ClPersist *) FindAuxCursor (M4LDB_CURSOR_AUDITORY_2);

		if (!pCursor2) {
			// El cursor tiene que existir.
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::BindParamAuditory" << "The cursor M4LDB_CURSOR_AUDITORY_2 does not exist");
			return (M4_ERROR);
		}
		
		/* Bug 0165266
		En el update del old se bindan los blob como binarios porque se traen de base de datos
		*/
		result = pCursor2->BindParam (M4_TRUE);
	}

	return (result);
}


m4return_t ClPersistBase::BindParamTramitExist (m4bool_t ai_bParent)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;
	m4int32_t indParam = 0;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Tramitation Exist.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Tramitation Exist.");
	#endif


	// Bindado de los parámetros del cursor Tramit Exist.
	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_TRAMIT_EXIST);

	if (!pCursor)
	{
		return (M4_SUCCESS);
	}

	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

		if( bIsPK )
		{
			// Es PK.
			eFldType_t eIntFld = m_pParam->GetInfoIdInternalField (indFld);

			if( ai_bParent == M4_TRUE )
			{
				if (eIntFld == M4LDB_ID_DATASET)
				{
					continue;
				}
			}

			/* Bug 0159233
			Se binda siempre el old value por si son actualizaciones de pk
			*/
			result = pCursor->ClStmt::BindParam (indParam + 1, indFld, M4_TRUE);

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}

			indParam++;
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::BindParamTramitDelete (void)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;
	m4int32_t indParam = 0;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Tramitation Delete.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Tramitation Delete.");
	#endif


	// Bindado de los parámetros del cursor Tramitation Delete.
	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_TRAMIT_DELETE);

	if (!pCursor) {
		return (M4_SUCCESS);
	}

	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

		if (bIsPK)
		{
			// Es PK.
			result = pCursor->ClStmt::BindParam (indParam + 1, indFld);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}

			indParam++;
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::BindParamTramitInsert (void)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;
	m4int32_t indParam = 0;
	ClParam* pParamTramitDTLastUPdate = NULL;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Tramitation Insert.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Tramitation Insert.");
	#endif


	// Bindado de los parámetros del cursor Inherit Insert.
	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_TRAMIT_INSERT);

	if (!pCursor)
	{
		return (M4_SUCCESS);
	}

	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];
		eFldType_t eIntFld = m_pParam->GetInfoIdInternalField (indFld);

		m4bool_t bOldData = M4_FALSE;
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

		if (bIsPK)
		{
			/* Bug 0159233, 0166440
			En pk se binda siempre el old value por si son actualizaciones de pk
			Los que no son pk no deben ir para que no vaya el DT_LAST_UPDATE
			*/

			bOldData = M4_TRUE;
		}

		result = pCursor->ClStmt::BindParam (indParam + 1, indFld, bOldData);

		if (result == M4_ERROR)
		{
			return (M4_ERROR);
		}

		indParam++;

		if( m_indRealTbl == 0 )
		{
			if( eIntFld == M4LDB_DT_LAST_UPDATE)
			{
				/* Bug 0166440
				La fecha TRAMIT_DT_LAST_UPDATE se coge del parámatro especial
				En el valor viejo no siempre está la fecha del registro original
				*/
				pParamTramitDTLastUPdate = GetClParamByType( M4LDB_TRAMIT_DT_LAST_UPDATE ) ;
				result = pCursor->ClStmt::BindParam( indParam + 1, pParamTramitDTLastUPdate->m_pData, NULL, pParamTramitDTLastUPdate->m_lPrec, pParamTramitDTLastUPdate->m_m4Type, pParamTramitDTLastUPdate->m_lScale, pParamTramitDTLastUPdate->m_plLength ) ;

				if (result == M4_ERROR) {
					return (M4_ERROR);
				}

				indParam++;
			}
			else if (eIntFld == M4LDB_DT_START)
			{
				// La fecha antigua TRAMIT_DT_START se coge del valor viejo
				result = pCursor->ClStmt::BindParam (indParam + 1, indFld, M4_TRUE);

				if (result == M4_ERROR) {
					return (M4_ERROR);
				}

				indParam++;
			}
		}
	}

	return (M4_SUCCESS);
}

m4return_t ClPersistBase::BindParamTramitInsertObject (void)
{
	m4return_t	result;
	m4int32_t	indParam = 0 ;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Tramitation Insert Object.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Tramitation Insert Object.");
	#endif


	// Bindado de los parámetros del cursor Inherit Insert.
	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_TRAMIT_INSERT_OBJECT);

	if (!pCursor)
	{
		return (M4_SUCCESS);
	}
		
	//Por orden de la sentencia:
	//DT_LAST_UPDATE, ID_SECUSER, ID_APPROLE, ID_TRAMITATION
	result = pCursor->ClStmt::BindParam (indParam + 1, m_pMtLgcTbl->m_indDTLstUpd);

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}
	indParam++;

	result = pCursor->ClStmt::BindParam (indParam + 1, m_pMtLgcTbl->m_indSecUser);

	if (result == M4_ERROR) 
	{
		return (M4_ERROR);
	}
	indParam++;

	result = pCursor->ClStmt::BindParam (indParam + 1, m_pMtLgcTbl->m_indAppRole);

	if (result == M4_ERROR) 
	{
		return (M4_ERROR);
	}
	indParam++;

	//Luego el ID_TRAMITATION
	result = pCursor->ClStmt::BindParam (indParam + 1, m_pMtLgcTbl->m_indDataSet);
	
	if (result == M4_ERROR)
	{
		return (M4_ERROR);
	}
	indParam++;

	return (M4_SUCCESS);
}

m4return_t ClPersistBase::BindParamValidateOrganization (void)
{
	m4return_t result;
	m4int32_t indForFld = 0;

	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_VALIDATE_ORGANIZATION);

	if (!pCursor) {
		return (M4_SUCCESS);
	}
	
	m4int32_t indParam = 0;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Validate Organization.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Validate Organization.");
	#endif

	ClParam * pParamIdRole = NULL;
	if ((m_pVcClParam == NULL) || ((pParamIdRole = GetClParamByType(M4LDB_APP_ROLE)) == NULL) ) {
		result = pCursor->ClStmt::BindParam (indParam + 1, m_pMtLgcTbl->m_indAppRole);
	}
	else {
		result = pCursor->ClStmt::BindParam (indParam + 1, pParamIdRole->m_pData, NULL, pParamIdRole->m_lPrec, pParamIdRole->m_m4Type, pParamIdRole->m_lScale, pParamIdRole->m_plLength );
	}
	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	indParam++;
	result = pCursor->ClStmt::BindParam (indParam + 1, m_pMtLgcTbl->m_indOrgType);

	//Pedro. Bug 0070817
	//Bindamos tres mas.
	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	indParam++;
	if (pParamIdRole == NULL) {
		result = pCursor->ClStmt::BindParam (indParam + 1, m_pMtLgcTbl->m_indAppRole);
	}
	else {
		result = pCursor->ClStmt::BindParam (indParam + 1, pParamIdRole->m_pData, NULL, pParamIdRole->m_lPrec, pParamIdRole->m_m4Type, pParamIdRole->m_lScale, pParamIdRole->m_plLength );
	}	
	if (result == M4_ERROR) {
		return (M4_ERROR);
	}	
	
	indParam++;
	result = pCursor->ClStmt::BindParam (indParam + 1, m_pMtLgcTbl->m_indOrgType);
	if (result == M4_ERROR) {
		return (M4_ERROR);
	}
	
	indParam++;		
	ClParam * pParamIdUser = NULL;
	if ((m_pVcClParam == NULL) || ((pParamIdUser = GetClParamByType(M4LDB_SEC_USER)) == NULL) ) {
		result = pCursor->ClStmt::BindParam (indParam + 1, m_pMtLgcTbl->m_indSecUser);
	}
	else {
		result = pCursor->ClStmt::BindParam (indParam + 1, pParamIdUser->m_pData, NULL, pParamIdUser->m_lPrec, pParamIdUser->m_m4Type, pParamIdUser->m_lScale, pParamIdUser->m_plLength );
	}		

	return (result);
}


m4return_t ClPersistBase::BindParamInheritExist (void)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;
	m4int32_t indParam = 0;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Inherit Exist.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Inherit Exist.");
	#endif


	// Bindado de los parámetros del cursor Inherit Exist.
	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_INHERIT_EXIST);

	if (!pCursor) {
		return (M4_SUCCESS);
	}

	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

		if (bIsPK)
		{
			// Es PK.
			result = pCursor->ClStmt::BindParam (indParam + 1, indFld);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}

			indParam++;
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::BindParamInheritDelete (void)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;
	m4int32_t indParam = 0;
	m4int32_t iTimes = 0 ;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Inherit Delete.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Inherit Delete.");
	#endif


	// Bindado de los parámetros del cursor Inherit Delete.
	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_INHERIT_DELETE);

	if (!pCursor) {
		return (M4_SUCCESS);
	}

	m4int32_t numPar = m_oVParam.size ();

	 
	// Bug 0180794
	iTimes = 1 ;
	if( m_indRealTbl != 0 )
	{
		iTimes = 2 ;
	}

	for (m4int32_t iTime = 0; iTime < iTimes; iTime++)
	{
		for (indPar=0; indPar<numPar; indPar++)
		{
			indFld = m_oVParam[indPar];
			m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

			if (bIsPK)
			{
				// Es PK.
				result = pCursor->ClStmt::BindParam (indParam + 1, indFld);

				if (result == M4_ERROR) {
					return (M4_ERROR);
				}

				indParam++;
			}
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::BindParamInheritInsert (void)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;
	m4int32_t indParam = 0;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Inherit Insert.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Inherit Insert.");
	#endif


	// Bindado de los parámetros del cursor Inherit Insert.
	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_INHERIT_INSERT);

	if (!pCursor) {
		return (M4_SUCCESS);
	}

	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];

		/* Bug 0157038
		El campo OPERATION no debe aparecer porque ya se pone manualmente
		*/
		eFldType_t eIntFld = m_pParam->GetInfoIdInternalField (indFld);

		if( eIntFld == M4LDB_ID_INHERIT_OPERATION )
		{
			continue;
		}

		result = pCursor->ClStmt::BindParam (indParam + 1, indFld);

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		indParam++;
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::BindParamInheritValidate (void)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;
	m4int32_t indParam = 0;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Inherit Validate.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Inherit Validate.");
	#endif


	// Bindado de los parámetros del cursor Inherit Validate.
	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_INHERIT_VALIDATE);

	if (!pCursor) {
		return (M4_SUCCESS);
	}

	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

		if (bIsPK)
		{
			// Es PK.
			result = pCursor->ClStmt::BindParam (indParam + 1, indFld);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}

			indParam++;
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::BindParamTranslationInsert (void)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;
	m4int32_t indParam = 0;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Translation Insert.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Translation Insert.");
	#endif


	// Bindado de los parámetros del cursor Translation Insert.
	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_TRANSLATION_INSERT);

	if (!pCursor) {
		return (M4_SUCCESS);
	}

	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

		if (bIsPK)
		{
			result = pCursor->ClStmt::BindParam (indParam + 1, indFld, m_pMtLgcTbl->m_bUpdatePK);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}

			indParam++;
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::BindParamTranslationValidatePK (void)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;
	m4int32_t indParam = 0;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Translation Validate Primary Key.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Translation Validate Primary Key.");
	#endif

	// Bindado de los parámetros del cursor Translation Validate PK.
	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_TRANSLATION_VALIDATE_PK);

	if (!pCursor) {
		return (M4_SUCCESS);
	}

	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

		if (bIsPK)
		{
			// Es PK.
			result = pCursor->ClStmt::BindParam (indParam + 1, indFld, m_pMtLgcTbl->m_bUpdatePK);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}

			indParam++;
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::BindParamTranslationUpdatePK (void)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;
	m4int32_t indParam = 0;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Translation Update Primary Key.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Translation Update. Primary key");
	#endif

	// Bindado de los parámetros del cursor Translation Update PK.
	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_TRANSLATION_UPDATE_PK);

	if (!pCursor) {
		return (M4_SUCCESS);
	}

	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

		if (!bIsPK)
		{
			continue;
		}

		result = pCursor->ClStmt::BindParam (indParam + 1, indFld);

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		indParam++;
	}

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

		if (bIsPK)
		{
			// Es PK.
			result = pCursor->ClStmt::BindParam (indParam + 1, indFld, m_pMtLgcTbl->m_bUpdatePK);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}

			indParam++;
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::BindParamUpdateVersion (void)
{
	m4return_t result;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Update Version.");
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Update Version.");
	#endif

	// Bindado de los parámetros del cursor Update Version.
	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_UPDATE_VERSION);

	if (!pCursor) {
		return (M4_SUCCESS);
	}

	ClParam* pParamDTLastUPdate = GetClParamByType(M4LDB_DT_LAST_UPDATE) ;
	result = pCursor->ClStmt::BindParam (1, pParamDTLastUPdate->m_pData, NULL, pParamDTLastUPdate->m_lPrec, pParamDTLastUPdate->m_m4Type, pParamDTLastUPdate->m_lScale, pParamDTLastUPdate->m_plLength);
	
	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::BindParamSingleInsert (m4bool_t ai_bBlobAsBinary)
{
	m4return_t	result;
	m4int32_t	indFld;
	m4int32_t	indPar;
	m4int32_t	indParam = 0;
	ClParam*	pParamTramitOp = NULL;
	ClParam*	pParamTramitDTLastUPdate = NULL;


	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters %s.", g_aPersist[m_eStmType]);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n + Binding Parameters %s.", g_aPersist[m_eStmType]);
	#endif

	//El primer valor a bindar si es la tabla base y estamos en tramitación 
	// es el TRAMIT_OPERATION, y el TRAMIT_DT_LAST_UPDATE
	if( m_indRealTbl == 0 && m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
	{
		//Primero va el TRAMIT_OPERATION
		pParamTramitOp = GetClParamByType(M4LDB_TRAMIT_OPERATION) ;
		result = ClStmt::BindParam (indParam + 1, pParamTramitOp->m_pData, NULL, pParamTramitOp->m_lPrec, pParamTramitOp->m_m4Type, pParamTramitOp->m_lScale, pParamTramitOp->m_plLength );
		
		if (result == M4_ERROR) 
		{
			return (M4_ERROR);
		}
		indParam++;

		//Luego el TRAMIT_DT_LAST_UPDATE
		pParamTramitDTLastUPdate = GetClParamByType(M4LDB_TRAMIT_DT_LAST_UPDATE) ;
		result = ClStmt::BindParam (indParam + 1, pParamTramitDTLastUPdate->m_pData, NULL, pParamTramitDTLastUPdate->m_lPrec, pParamTramitDTLastUPdate->m_m4Type, pParamTramitDTLastUPdate->m_lScale, pParamTramitDTLastUPdate->m_plLength );
		
		if (result == M4_ERROR) 
		{
			return (M4_ERROR);
		}
		indParam++;
	}

	m4uint32_t	iIsTranslation = 0;

	if( m_indRealTbl != 0 )
	{
		iIsTranslation = M4LDB_NUMBER( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_IS_TRANSLATION].Value.GetWithOutSec() );
	}

	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];
		result = ClStmt::BindParam (indParam + 1, indFld, M4_FALSE, ai_bBlobAsBinary);

		if (result == M4_ERROR)
		{
			return (M4_ERROR);
		}

		indParam++;

		/* Bug 0156331
		En el insert también se pone el TRAMIT_DT_START
		*/
		if( m_indRealTbl == 0 && m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
		{
			eFldType_t eIntFld = m_pParam->GetInfoIdInternalField (indFld);

			if( eIntFld == M4LDB_DT_START )
			{
				result = ClStmt::BindParam (indParam + 1, indFld);

				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}

				indParam++;
			}
		}
	}

	return (result);
}


m4return_t ClPersistBase::BindParamInsert (m4bool_t ai_bBlobAsBinary)
{
	m4return_t	result;

	// Esto va delante por el tema de los ordinales.
	result = BindParamValidateFKs ();

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	result = BindParamValidateComp ();

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	result = BindParamSingleInsert( ai_bBlobAsBinary );

	if( result == M4_ERROR )
	{
		return( M4_ERROR );
	}

	if (m_eStmType == M4LDB_DUPLICATE)
	{
		result = BindParamValidatePK ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}
	}

	result = BindParamValidateNoPK ();

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	if (!m_indRealTbl)
	{
		// Sólo la principal.
		if (!m_pMtLgcTbl->m_bAudStmt)
		{
			if (m_pMtLgcTbl->m_eCurOrg == M4LDB_MULTI_ORGANIZATION)
			{
				result = BindParamValidateOrganization ();

				if (result == M4_ERROR) {
					return (M4_ERROR);
				}
			}

			if (m_pMtSecSent)
			{
				result = BindParamValidateSecurity ();

				if (result == M4_ERROR) {
					return (M4_ERROR);
				}
			}

			if (m_pMtSent != NULL && m_pMtSent->GetIsInsertIntoSelect () == M4_TRUE )
			{
				result = BindParamCursorQuery ();

				if (result == M4_ERROR) {
					return (M4_ERROR);
				}
			}
			
			if (m_pMtLgcTbl->m_eCurTemp != M4LDB_ETHERN)
			{
				result = BindParamGetHistory ();
				
				if (result == M4_ERROR) {
					return (M4_ERROR);
				}
			}

			if (m_pMtLgcTbl->m_bCorrected)
			{
				result = BindParamGetCorrectedHistory ();
				
				if (result == M4_ERROR) {
					return (M4_ERROR);
				}

				result = BindParamValidateFRs ();

				if (result == M4_ERROR) {
					return (M4_ERROR);
				}
			}
		}
	}

	m4uint32_t	iIsTranslation = 0;

	if( m_indRealTbl != 0 )
	{
		iIsTranslation = M4LDB_NUMBER( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_IS_TRANSLATION].Value.GetWithOutSec() );
	}

	if( iIsTranslation == 0 )
	{
		// Tramitación
		if( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
		{
			result = BindParamTramitExist( M4_FALSE ) ;

			if( result == M4_ERROR )
			{
				return( M4_ERROR ) ;
			}

			result = BindParamTramitDelete() ;

			if( result == M4_ERROR )
			{
				return( M4_ERROR ) ;
			}

			//Sólo para la tabla principal y si es la padre de herencia
			if( m_indRealTbl == 0 && m_idMainTable != NULL )
			{
				result = BindParamTramitInsertObject() ;

				if( result == M4_ERROR )
				{
					return( M4_ERROR ) ;
				}
			}
		}

		// Herencia de datos canales mono y multi
		if( m_bUseInheritance == M4_TRUE && m_pMtLgcTbl->m_bInheritsData == M4_TRUE )
		{
			result = BindParamInheritExist() ;

			if( result == M4_ERROR )
			{
				return( M4_ERROR ) ;
			}

			result = BindParamInheritDelete() ;

			if( result == M4_ERROR )
			{
				return( M4_ERROR ) ;
			}
		}
	}

	return (result);
}


m4return_t ClPersistBase::BindParamSingleUpdate (void)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;
	m4int32_t indParam = 0;
	m4bool_t bIsAPISQL = M4_FALSE;

	if (m_pMtSent) {
		bIsAPISQL = m_pMtSent->IsAPISQL ();
	}

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters %s.", g_aPersist[m_eStmType]);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n + Binding Parameters %s.", g_aPersist[m_eStmType]);
	#endif

	m4uint32_t	iIsTranslation = 0;

	if( m_indRealTbl != 0 )
	{
		iIsTranslation = M4LDB_NUMBER( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_IS_TRANSLATION].Value.GetWithOutSec() );
	}

	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);
		m4int32_t hItem = m_pParam->GetItemHandle (indFld);

		if ((!bIsPK || bIsAPISQL || !m_pMtSent || (indFld == m_pMtLgcTbl->m_indDTStart && m_pMtSent))
			&& 
		    (hItem || (m_pParam->GetCurrentLanguageIndex(indFld) != M4LDB_NULL_INDEX && m_pParam->GetInfoM4Type(indFld) != M4CL_M4_TYPE_BLOB)))
		{
			// Es susceptible de actualización todo lo que tenga handle y :
			// 1.- No es PK.
			// 2.- Viene de APISQL.
			// 3.- Es una operación interna.
			// 4.- La DT_START y sólo para operaciones externas.
			// 5.- Si el campo es el del lenguaje por defecto y estamos trabajando con otro lenguaje (pero no para blobs)

			result = ClStmt::BindParam (indParam + 1, indFld);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
			indParam++;
		}
	}

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

		if (bIsPK)
		{
			// Es PK.
			result = ClStmt::BindParam (indParam + 1, indFld, m_pMtLgcTbl->m_bUpdatePK);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}

			indParam++;
		}
	}

	return (result);
}


m4return_t ClPersistBase::BindParamUpdate (void)
{
	m4return_t result;
	m4bool_t bIsAPISQL = M4_FALSE;

	m4uint32_t iIsTranslation = 0;

	if( m_indRealTbl != 0 )
	{
		iIsTranslation = M4LDB_NUMBER( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_IS_TRANSLATION].Value.GetWithOutSec() );
	}

	if( iIsTranslation == 1 && m_bUseTranslation == M4_TRUE && m_pMtLgcTbl->m_bUpdatePK == M4_TRUE )
	{
		result = BindParamTranslationValidatePK() ;

		if( result == M4_ERROR )
		{
			return( M4_ERROR ) ;
		}

		result = BindParamTranslationUpdatePK() ;

		if( result == M4_ERROR )
		{
			return( M4_ERROR ) ;
		}

		if( m_pGlobVars->GetpLConn()->IsDynamicLanguage( m_pGlobVars->GetLanguage() ) == M4_FALSE )
		{
			result = BindParamValidateNoPK() ;

			if( result == M4_ERROR )
			{
				return( M4_ERROR ) ;
			}

			return( M4_SUCCESS ) ;
		}
	}

	if (m_pMtSent) {
		bIsAPISQL = m_pMtSent->IsAPISQL ();
	}

	// Esto va delante por el tema de los ordinales.
	result = BindParamValidateFKs ();

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	result = BindParamValidateComp ();

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	result = BindParamSingleUpdate();

	if( result == M4_ERROR )
	{
		return( M4_ERROR );
	}

	result = BindParamValidatePK ();

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	if (m_pMtLgcTbl->m_bUpdatePK) {
		result = BindParamValidateNoPK ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}
	}
		
	if (!m_indRealTbl)
	{
		// Sólo la principal.
		if (m_pMtLgcTbl->m_eCurOrg == M4LDB_MULTI_ORGANIZATION)
		{
			result = BindParamValidateOrganization ();

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
		}

		if (m_pMtSecSent)
		{
			result = BindParamValidateSecurity ();

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
		}
		
		if (bIsAPISQL)
		{
			result = BindParamCursorQuery ();

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
		}

		if (m_pMtLgcTbl->m_eCurTemp != M4LDB_ETHERN)
		{
			result = BindParamGetHistory ();
				
			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
		}

		if (m_pMtLgcTbl->m_bCorrected)
		{
			result = BindParamGetCorrectedHistory ();
			
			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
		}
	}

	result = BindParamValidateFRs ();

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	if( iIsTranslation == 0 )
	{
		// Tramitación
		if( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
		{
			result = BindParamTramitExist( M4_FALSE ) ;

			if( result == M4_ERROR )
			{
				return( M4_ERROR ) ;
			}

			result = BindParamTramitInsert() ;

			if( result == M4_ERROR )
			{
				return( M4_ERROR ) ;
			}

			//Sólo para la tabla principal y si es la padre de herencia
			if( m_indRealTbl == 0 && m_idMainTable != NULL )
			{
				result = BindParamTramitInsertObject() ;

				if( result == M4_ERROR )
				{
					return( M4_ERROR ) ;
				}
			}
		}

		// Herencia de datos canales mono y multi
		if( m_bUseInheritance == M4_TRUE && m_pMtLgcTbl->m_bInheritsData == M4_TRUE )
		{
			result = BindParamInheritExist() ;

			if( result == M4_ERROR )
			{
				return( M4_ERROR ) ;
			}

			result = BindParamInheritInsert() ;

			if( result == M4_ERROR )
			{
				return( M4_ERROR ) ;
			}
		}
	}
	else if( m_bUseTranslation == M4_TRUE )
	{
		result = BindParamTranslationInsert() ;

		if( result == M4_ERROR )
		{
			return( M4_ERROR ) ;
		}
	}

	return (result);
}


m4return_t ClPersistBase::BindParamSingleDelete (void)
{
	m4return_t result;
	m4int32_t indFld;
	m4int32_t indPar;
	m4int32_t indParam = 0;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters %s.", g_aPersist[m_eStmType]);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n + Binding Parameters %s.", g_aPersist[m_eStmType]);
	#endif

	m4int32_t numPar = m_oVParam.size ();

	for (indPar=0; indPar<numPar; indPar++)
	{
		indFld = m_oVParam[indPar];
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

		if (bIsPK)
		{
			// Es PK.
			result = ClStmt::BindParam (indParam + 1, indFld);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}

			indParam++;
		}
	}

	return (result);
}


m4return_t ClPersistBase::BindParamDelete (void)
{
	m4return_t result;

	m4bool_t bNoOptimization = M4_TRUE;
	if (m_pMtSent && m_pMtSent->HasValidation() == M4_FALSE)
	{
		// Para que haya optimización en la no validación tiene que ser un objeto logico con una 
		// sola tabla real y no tener herencia.
		if (m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Count() == 1 && !m_pMtSent->ExistsInheritance())
		{
			bNoOptimization = M4_FALSE;
		}
	}

	/* Bug 0154924
	Si estamos en tramitación o herencia se desactiva la optimización
	*/
	if( bNoOptimization == M4_FALSE )
	{
		if(
			( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
			||
			( m_bUseInheritance == M4_TRUE && m_pMtLgcTbl->m_bInheritsData == M4_TRUE )
			)
		{
			bNoOptimization = M4_TRUE ;
		}
	}

	if (bNoOptimization == M4_TRUE)
	{
		result = BindParamSingleDelete();

		if( result == M4_ERROR )
		{
			return( M4_ERROR );
		}

		result = BindParamValidatePK ();

		if (result == M4_ERROR)
		{
			return (M4_ERROR);
		}

		if (!m_indRealTbl)
		{
			// Sólo la principal.
			if (m_pMtLgcTbl->m_eCurOrg == M4LDB_MULTI_ORGANIZATION)
			{
				result = BindParamValidateOrganization ();

				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}
			}

			if (m_pMtSecSent)
			{
				result = BindParamValidateSecurity ();

				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}
			}

			if (m_pMtSent && m_pMtSent->IsAPISQL ())
			{
				result = BindParamCursorQuery ();

				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}
			}

			if (m_pMtLgcTbl->m_eCurTemp != M4LDB_ETHERN && m_pMtLgcTbl->m_eCurTemp != M4LDB_1_PERIOD)
			{
				result = BindParamGetHistory ();
					
				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}
			}

			if (m_pMtLgcTbl->m_bCorrected)
			{
				result = BindParamGetCorrectedHistory ();
				
				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}
			}
		}

		result = BindParamValidateFRs ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		result = BindParamValidateComp ();


		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		m4uint32_t	iIsTranslation = 0;

		if( m_indRealTbl != 0 )
		{
			iIsTranslation = M4LDB_NUMBER( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_IS_TRANSLATION].Value.GetWithOutSec() );
		}

		if( iIsTranslation == 0 )
		{
			// Tramitación
			if( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
			{
				result = BindParamTramitExist( M4_TRUE ) ;

				if( result == M4_ERROR )
				{
					return( M4_ERROR ) ;
				}

				result = BindParamTramitInsert() ;

				if( result == M4_ERROR )
				{
					return( M4_ERROR ) ;
				}

				//Sólo para la tabla principal y si es la padre de herencia
				if( m_indRealTbl == 0 && m_idMainTable != NULL )
				{
					result = BindParamTramitInsertObject() ;

					if( result == M4_ERROR )
					{
						return( M4_ERROR ) ;
					}
				}
			}

			// Herencia de datos canales mono y multi
			if( m_bUseInheritance == M4_TRUE && m_pMtLgcTbl->m_bInheritsData == M4_TRUE )
			{
				result = BindParamInheritExist() ;

				if( result == M4_ERROR )
				{
					return( M4_ERROR ) ;
				}

				result = BindParamInheritInsert() ;

				if( result == M4_ERROR )
				{
					return( M4_ERROR ) ;
				}

				result = BindParamInheritDelete() ;

				if( result == M4_ERROR )
				{
					return( M4_ERROR ) ;
				}

				if( m_indRealTbl == 0 )
				{
					result = BindParamInheritValidate() ;

					if( result == M4_ERROR )
					{
						return( M4_ERROR ) ;
					}
				}
			}
		}
	}
	else
	{
		result = BindParamCursorQuery ();

		if( result == M4_ERROR )
		{
			return( M4_ERROR ) ;
		}
	}

	return (result);
}


m4return_t ClPersistBase::ComposePK (string &ai_onPKString, string &ai_oiPKString)
{
	m4char_t aValue[M4LDB_SIZE_DEFAULT_STRING + 1];
	m4char_t aDate[M4LDB_SIZE_TIMESTAMP + 1];

	m4bool_t bFirst = M4_TRUE;
	m4bool_t bFirstItemPk = M4_TRUE;
	m4bool_t bShowIdOrg = M4_FALSE;


	// el campo ID_ORGANIZATION es PK siempre, pero en tablas mono no debe salir en el mensaje de error.
	if (m_pMtLgcTbl->m_eCurOrg == M4LDB_MULTI_ORGANIZATION){
		bShowIdOrg = M4_TRUE;
	}

	m4int32_t numPar = m_oVParam.size();

	for (m4int32_t indPar=0; indPar<numPar; indPar++)
	{
		m4int32_t indParam = m_oVParam[indPar];
		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indParam);

		if (bIsPK)
		{
			if( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
			{
				// El el caso de tramitación el ID_DATASET no debe salir
				if( m_pParam->GetInfoIdInternalField(indParam) == M4LDB_ID_DATASET )
				{
					continue;
				}
			}

			if (bShowIdOrg == M4_FALSE)
			{
				// Si no hay que mostrarlo y es el IdOrganization vamos al siguiente.
				if (m_pParam->GetInfoIdInternalField(indParam)==M4LDB_ID_ORGANIZATION)
				{
					continue;
				}
			}

			if (!bFirst)
			{
				ai_onPKString.append (", ");
				ai_oiPKString.append (", ");
			}
			else
			{
				bFirst = M4_FALSE;
			}

			// Nos movemos al primer item que veamos de la PK.
			if (bFirstItemPk && m_pParam->GetInfoParamType(indParam)==M4LDB_PAR_ITEM)
			{
				m4int32_t hItem = m_pParam->GetItemHandle(indParam);

				if (hItem)
				{
					ClNode *pNode = m_pGlobVars->GetpLConn ()->m_pLogAccess->Node.Get((ClHandle)m_pGlobVars->GetpLConn ()->m_hLogNode);

					if (pNode->RecordSet.Register.Item.MoveTo((ClHandle)hItem))
					{
						// OK. Nos hemos podido mover
						bFirstItemPk = M4_FALSE;
					}
				}
			}

			m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, indParam);

			ai_onPKString.append (nField);
			ai_onPKString.append ("=");
			m4id_t iField = m_pParam->GetInfoIdField (indParam);
			ai_oiPKString.append (iField);
			ai_oiPKString.append ("=");

			m4int32_t lLength = m_pParam->GetOldLength (indParam);

			if (lLength != M4LDB_SQL_NULL_DATA)
			{
				m4pcchar_t pValue = m_pParam->GetOldData (indParam);
				m4int8_t m4Type = m_pParam->GetInfoM4Type (indParam);

				switch (m4Type) {

					default:

						lLength = strlen( pValue ) ;

						if (lLength > M4LDB_SIZE_DEFAULT_STRING)
						{
							/* Bug 0207884
							Se evita el desbordamiento del buffer
							*/
							sprintf (aValue, "%.32s...%.32s", pValue, pValue + lLength - 32);
						}
						else
						{
							sprintf (aValue, "%s", pValue);
						}

						break;

					case M4CL_M4_TYPE_DATE:
						ClDateToString (M4LDB_DOUBLE (pValue), aDate, M4LDB_SIZE_DATE + 1);
						sprintf (aValue, "%s", aDate);
						break;

					case M4CL_M4_TYPE_TIMESTAMP:
						ClDateToString (M4LDB_DOUBLE (pValue), aDate, M4LDB_SIZE_TIMESTAMP + 1);
						sprintf (aValue, "%s", aDate);
						break;

					case M4CL_M4_TYPE_NUMBER:
						sprintf (aValue, "%.20G", M4LDB_DOUBLE (pValue));
						break;
				}

				ai_onPKString.append (aValue);
				ai_oiPKString.append (aValue);
			}
			else
			{
				ai_onPKString.append ("<null>");
				ai_oiPKString.append ("<null>");
			}
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ComposeFK (string &ai_onFKString, string &ai_oiFKString, ClLoad *ai_pCursor)
{
/*
	m4char_t aValue[M4LDB_SIZE_DEFAULT_STRING];
	m4char_t aDate[M4LDB_SIZE_TIMESTAMP + 1];
*/
	m4bool_t bFirst = M4_TRUE;
	
	m4bool_t bShowIdOrg = M4_FALSE;

	// el campo ID_ORGANIZATION es PK siempre, pero en tablas mono no debe salir en el mensaje de error.
	if (ai_pCursor->m_eVerifyOrg == M4LDB_MULTI_ORGANIZATION){
		bShowIdOrg = M4_TRUE;
	}
	
	m4int32_t numForFld = ai_pCursor->m_pvcIndParVal->size ();

	for (m4int32_t indForFld=0; indForFld<numForFld; indForFld++) {

		PStIndPar pIndPar = (*ai_pCursor->m_pvcIndParVal)[indForFld];

		if (pIndPar->m_bIsPK) {

			if (bShowIdOrg==M4_FALSE){
				// Si no hay que mostrarlo y es el IdOrganization vamos al siguiente.
				if (m_pParam->GetInfoIdInternalField(pIndPar->m_indPar)==M4LDB_ID_ORGANIZATION){
					continue;
				}
			}

			if (!bFirst) {
				ai_onFKString.append (", ");

				ai_oiFKString.append (", ");
			}
			else {
				bFirst = M4_FALSE;
			}

			m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, pIndPar->m_indPar);

			ai_onFKString.append (nField);

//			ai_onFKString.append ("=");

			m4id_t iField = m_pParam->GetInfoIdField (pIndPar->m_indPar);

			ai_oiFKString.append (iField);
/*
			ai_oIFKString.append ("=");

			m4int32_t lLength = m_pParam->GetLength (pIndPar->m_indPar);

			if (lLength != M4LDB_SQL_NULL_DATA) {
				m4int8_t m4Type = m_pParam->GetInfoM4Type (pIndPar->m_indPar);

				switch (m4Type) {

					default:
						sprintf (aValue, "%s", m_pParam->GetData (pIndPar->m_indPar));
						break;

					case M4CL_M4_TYPE_DATE:
						ClDateToString (M4LDB_DOUBLE (m_pParam->GetData (pIndPar->m_indPar)), aDate, M4LDB_SIZE_DATE + 1);
						sprintf (aValue, "%s", aDate);
						break;

					case M4CL_M4_TYPE_TIMESTAMP:
						ClDateToString (M4LDB_DOUBLE (m_pParam->GetData (pIndPar->m_indPar)), aDate, M4LDB_SIZE_TIMESTAMP + 1);
						sprintf (aValue, "%s", aDate);
						break;

					case M4CL_M4_TYPE_NUMBER:
						sprintf (aValue, "%.20G", M4LDB_DOUBLE (m_pParam->GetData (pIndPar->m_indPar)));
						break;
				}

				ai_onFKString.append (aValue);

				ai_oiFKString.append (aValue);
			}
			else {
				ai_onFKString.append ("<null>");

				ai_oiFKString.append ("<null>");
			}
			*/
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ComposeRPFields (string &ai_onFields, string &ai_oiFields)
{
	m4bool_t bFirst = M4_TRUE;

	m4bool_t bShowIdOrg = M4_FALSE;

	// el campo ID_ORGANIZATION es PK siempre, pero en tablas mono no debe salir en el mensaje de error.
	if (m_pMtLgcTbl->m_eCurOrg == M4LDB_MULTI_ORGANIZATION){
		bShowIdOrg = M4_TRUE;
	}
	
	m4int32_t numPar = m_oVParam.size();

	for (m4int32_t indPar=0; indPar<numPar; indPar++) {

		m4int32_t indParam = m_oVParam[indPar];

		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indParam);

		m4bool_t bIsCtrlRep = m_pParam->GetInfoIsCtrlRep (indParam);

		if ((!bIsPK) &&
			(bIsCtrlRep)) {

			if (bShowIdOrg==M4_FALSE){
				// Si no hay que mostrarlo y es el IdOrganization vamos al siguiente.
				if (m_pParam->GetInfoIdInternalField(indParam)==M4LDB_ID_ORGANIZATION){
					continue;
				}
			}

			if (!bFirst) {
				ai_onFields.append (", ");

				ai_oiFields.append (", ");
			}
			else {
				bFirst = M4_FALSE;
			}

			ai_onFields.append (m_pGlobVars->GetpLConn ()->GetnField (this, indParam));
			
			ai_oiFields.append (m_pParam->GetInfoIdField (indParam));
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ComposePKWithoutNames (string &ai_oiPKString)
{
	m4char_t aValue[M4LDB_SIZE_DEFAULT_STRING];

	m4bool_t bFirst = M4_TRUE;
	
	m4int32_t numPar = m_oVParam.size();

	for (m4int32_t indPar=0; indPar<numPar; indPar++) {

		m4int32_t indParam = m_oVParam[indPar];

		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indParam);

		if ((bIsPK) && 
			(indParam != m_pMtLgcTbl->m_indDTStart) && 
			(indParam != m_pMtLgcTbl->m_indDTStartCorr)) {

			if (!bFirst) {
				ai_oiPKString.append ("_");
			}
			else {
				bFirst = M4_FALSE;
			}

			m4int32_t lLength = m_pParam->GetLength (indParam);

			if (lLength != M4LDB_SQL_NULL_DATA) {

				m4pcchar_t pValue = m_pParam->GetData (indParam);

				m4int8_t m4Type = m_pParam->GetInfoM4Type (indParam);

				switch (m4Type) {

					default:
						sprintf (aValue, "%s", pValue);
						break;

					case M4CL_M4_TYPE_DATE:
					case M4CL_M4_TYPE_TIMESTAMP:
					case M4CL_M4_TYPE_NUMBER:
						sprintf (aValue, "%.20G", M4LDB_DOUBLE (pValue));
						break;
				}

				ai_oiPKString.append (aValue);
			}
			else {
				ai_oiPKString.append ("<null>");
			}
		}
	}

	return (M4_SUCCESS);
}


void ClPersistBase::InsertInTransaction (void)
{
	if (!m_pGlobVars->GetpLConn ()->m_pmpTransCache) {
		m_pGlobVars->GetpLConn ()->m_pmpTransCache = new MpTransCache;
	}

	struct StTransaction stAux;

	if (m_pMtLgcTbl->m_indDTStart != M4LDB_NULL_INDEX) {
		stAux.m_dDTStart = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTStart));
	}

	if (m_pMtLgcTbl->m_indDTStartCorr != M4LDB_NULL_INDEX) {
		stAux.m_dDTStartCorr = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTStartCorr));
	}

	stAux.m_indLgcTbl = m_pMtLgcTbl->m_indLgcTbl;

	ComposePKWithoutNames (stAux.m_stPK);

	m_pGlobVars->GetpLConn ()->m_pmpTransCache->insert (MpTransCache::value_type (stAux, this));
}


m4bool_t ClPersistBase::FindInTransaction (m4date_t ai_dDTStart, m4date_t ai_dDTStartCorr)
{
	if (!m_pGlobVars->GetpLConn ()->m_pmpTransCache) {
		return (M4_FALSE);
	}

	struct StTransaction stAux;

	stAux.m_dDTStart = ai_dDTStart;

	stAux.m_dDTStartCorr = ai_dDTStartCorr;

	if ((!ai_dDTStartCorr) && 
		(m_pMtLgcTbl->m_indDTStartCorr != M4LDB_NULL_INDEX)) {
		stAux.m_dDTStartCorr = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTStartCorr));
	}

	stAux.m_indLgcTbl = m_pMtLgcTbl->m_indLgcTbl;

	ComposePKWithoutNames (stAux.m_stPK);

	itMpTransCache it = m_pGlobVars->GetpLConn ()->m_pmpTransCache->find (stAux);

	if (it == m_pGlobVars->GetpLConn ()->m_pmpTransCache->end ()) {
		return (M4_FALSE);
	}

	return (M4_TRUE);
}



