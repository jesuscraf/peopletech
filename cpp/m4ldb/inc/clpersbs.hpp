//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                clpersbs.hpp
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


#ifndef __CLPERSBS_HPP__
#define __CLPERSBS_HPP__


#include "defmain.hpp"


class ClRecordSet;
class ClRegister;
class ClTblMem;
class ClPersistBase;


struct stMtLgcTbl
{
	m4id_t m_idLgcTbl;
	eTempType_t m_eCurTemp;
	eHistType_t m_eCurHist;
	eOrgType_t m_eCurOrg;
	m4int32_t m_iOrgLevel;
	m4bool_t m_bInheritsData;
	m4uint8_t m_iMirrorType;
	m4bool_t m_bIsCacheable;
	m4bool_t m_bComplete;
	m4bool_t m_bCorrected;
	m4bool_t m_bInvRecordSet;
	m4bool_t m_bInvComp;
	m4bool_t m_bUpdatePK;
	m4bool_t m_bPKUpdated;
	m4bool_t m_bValidate;
	m4bool_t m_bHasAudit;
	m4bool_t m_bAudStmt;
	//Bug 0084837
	eStmType_t m_eStmTypeAuditory;
	m4int32_t m_indDTStart;
	m4int32_t m_indDTEnd;
	m4int32_t m_indDTStartCorr;
	m4int32_t m_indDTEndCorr;
	m4int32_t m_indRegType;
	m4int32_t m_indDTLstUpd;
	m4int32_t m_indSecUser;
	m4int32_t m_indAppRole;
	m4int32_t m_indOrgType;
	m4int32_t m_indDataSet;
	m4int32_t m_indBefCorrReg;
	m4int32_t m_lSecMask;
	m4date_t m_dClosingDate;
	m4date_t m_dDateUnit;
	ClPersistBase *m_pMaster;
	ClRegisterIndex m_indLgcTbl;

	stMtLgcTbl (void)
	{
		m_idLgcTbl = M4LDB_EMPTY;
		m_eCurTemp = M4LDB_ETHERN;
		m_eCurHist = M4LDB_N_PERIOD_WITHOUT_AUTO;
		m_eCurOrg = M4LDB_UNDEF_ORGANIZATION;
		m_iOrgLevel = 0;
		m_bInheritsData = M4_FALSE;
		m_iMirrorType = 0;
		m_bIsCacheable = M4_FALSE;
		m_bComplete = M4_FALSE;
		m_bCorrected = M4_FALSE;
		m_bInvRecordSet = M4_FALSE;
		m_bInvComp = M4_FALSE;
		m_bValidate = M4_TRUE;
		m_bHasAudit = M4_FALSE;
		m_bAudStmt = M4_FALSE;
		m_eStmTypeAuditory = M4LDB_SELECT;
		m_bUpdatePK = M4_FALSE;
		m_bPKUpdated = M4_FALSE;
		m_indDTStart = M4LDB_NULL_INDEX;
		m_indDTEnd = M4LDB_NULL_INDEX;
		m_indDTStartCorr = M4LDB_NULL_INDEX;
		m_indDTEndCorr = M4LDB_NULL_INDEX;
		m_indRegType = M4LDB_NULL_INDEX;
		m_indDTLstUpd = M4LDB_NULL_INDEX;
		m_indSecUser = M4LDB_NULL_INDEX;
		m_indAppRole = M4LDB_NULL_INDEX;
		m_indOrgType = M4LDB_NULL_INDEX;
		m_indDataSet = M4LDB_NULL_INDEX;
		m_indBefCorrReg = M4LDB_NULL_INDEX;
		m_lSecMask = M4LDB_SUPERROLE_SEC_MASK;
		m_dClosingDate = M4LDB_MINUS_INF_DATE;
		m_dDateUnit = M4_ONE_DAY;
		m_pMaster = NULL;
		m_indLgcTbl = M4LDB_NULL_INDEX;
	}
};


struct stHistory
{
	m4int32_t m_indRow;	// Indice del registro en la tabla virtual de resultados.
	m4int32_t m_indReg;	// Indice del registro en el histórico corregido de registros.
	m4date_t m_dDTStart;
	m4date_t m_dDTStartCorr;
	m4date_t m_dDTEnd;
	m4date_t m_dDTEndCorr;
	m4date_t m_dDTLstUpd;
	eStmType_t m_eOperation;
	eRegType_t m_eRegType;
	// Esto es para saber si realmente se ha cambiado el registro.
	m4date_t m_dOldDTEndCorr;
	eRegType_t m_eOldRegType;

	stHistory (void)
	{
		m_indRow = M4LDB_NULL_INDEX;
		m_indReg = M4LDB_NULL_INDEX;
		m_dDTStart = M4LDB_MINUS_INF_DATE;
		m_dDTStartCorr = M4LDB_MINUS_INF_DATE;
		m_dDTEnd = M4LDB_PLUS_INF_DATE;
		m_dDTEndCorr = M4LDB_PLUS_INF_DATE;
		m_dDTLstUpd = M4LDB_MINUS_INF_DATE;
		m_eOperation = M4LDB_SELECT;
		m_eRegType = M4LDB_NORMAL_REG;
		m_dOldDTEndCorr = M4LDB_PLUS_INF_DATE;
		m_eOldRegType = M4LDB_NORMAL_REG;
	}

	stHistory (const struct stHistory &ai_stCopy)
	{
		m_indRow = ai_stCopy.m_indRow;
		m_indReg = ai_stCopy.m_indReg;
		m_dDTStart = ai_stCopy.m_dDTStart;
		m_dDTStartCorr = ai_stCopy.m_dDTStartCorr;
		m_dDTEnd = ai_stCopy.m_dDTEnd;
		m_dDTEndCorr = ai_stCopy.m_dDTEndCorr;
		m_dDTLstUpd = ai_stCopy.m_dDTLstUpd;
		m_eOperation = ai_stCopy.m_eOperation;
		m_eRegType = ai_stCopy.m_eRegType;
		m_dOldDTEndCorr = ai_stCopy.m_dOldDTEndCorr;
		m_eOldRegType = ai_stCopy.m_eOldRegType;
	}
	
	friend m4bool_t OrderByCorr (const struct stHistory &lhc, const struct stHistory &rhc) {
		if (M4LDB_COMPARE_DATES (lhc.m_dDTStartCorr, rhc.m_dDTStartCorr) < 0) {
			return (M4_TRUE);
		}
		else {
			if (!M4LDB_COMPARE_DATES (lhc.m_dDTStartCorr, rhc.m_dDTStartCorr)) {
				if (M4LDB_COMPARE_DATES (lhc.m_dDTStart, rhc.m_dDTStart) < 0) {
					return (M4_TRUE);
				}
			}
		}

		return (M4_FALSE);
	}

	friend m4bool_t OrderByApp (const struct stHistory &lhc, const struct stHistory &rhc) {
		if (M4LDB_COMPARE_DATES (lhc.m_dDTStart, rhc.m_dDTStart) < 0) {
			return (M4_TRUE);
		}
		else {
			if (!M4LDB_COMPARE_DATES (lhc.m_dDTStart, rhc.m_dDTStart)) {
				if (M4LDB_COMPARE_DATES (lhc.m_dDTStartCorr, rhc.m_dDTStartCorr) < 0) {
					return (M4_TRUE);
				}
			}
		}

		return (M4_FALSE);
	}

	// Por defecto.
	friend m4bool_t operator < (const struct stHistory &lhc, const struct stHistory &rhc) {
		return (OrderByApp (lhc, rhc));
	}

	friend m4bool_t operator == (const struct stHistory &lhc, const struct stHistory &rhc) {
		return (((!M4LDB_COMPARE_DATES (lhc.m_dDTStart, rhc.m_dDTStart)) && (!M4LDB_COMPARE_DATES (lhc.m_dDTStartCorr, rhc.m_dDTStartCorr))) ? M4_TRUE : M4_FALSE);
	}

};


typedef vector<struct stHistory>				VcHistory;
typedef VcHistory*								PVcHistory;
typedef VcHistory::iterator						itVcHistory;




typedef map<m4int32_t, ClStmt*, less<m4int32_t> >	MpPValCursor;
typedef MpPValCursor*								PMpPValCursor;
typedef MpPValCursor::const_iterator				itMpPValCursor;


class ClPersistBase : public ClStmt
{
protected:

	ClPersist * m_pSourcePersist;

	struct stMtLgcTbl *m_pMtLgcTbl;

	VcVirtualParam m_oVParam;

	// Cursores auxiliares.
	PMpPValCursor m_pmpValCur;

	ClRegisterIndex m_indRealTbl;

	// Tabla de registros históricos.
	PClTblMem m_pHistTbl;

	// Tabla de registros históricos con corrección.
	PClTblMem m_pCorrHistTbl;

	// Vector de identificadores de registros históricos con corrección.
	PVcHistory m_pVcHistory;

	// Esto es para mantener el contenedor del ordinal bindado en memoria.
	m4double_t m_dOrdinal;
	m4int32_t m_lOrdinal;

	m4date_t m_dCompleteDTStart;
	m4int32_t m_lCompleteDTStart;

	m4date_t m_dCompleteDTEnd;
	m4int32_t m_lCompleteDTEnd;

	m4double_t m_dProjection;
	m4int32_t m_lProjection;

	// UNICODE DB
	m4pchar_t m_pcOrdinal;
	m4pchar_t m_pcProjection;
	m4pchar_t m_pcCompleteDTStart;
	m4pchar_t m_pcCompleteDTEnd;
	m4int32_t m_iOrdinalMaxLen;
	m4int32_t m_iProjectionMaxLen;
	m4int32_t m_iCompleteDTStart;
	m4int32_t m_iCompleteDTEnd;

	ClMetaSentence *m_pMtSecSent;

	m4bool_t m_bUseTramitation;
	m4bool_t m_bUseInheritance;
	m4bool_t m_bUseTranslation;

	m4id_t m_idMainTable;
	
	m4return_t PrepareOrdinalSelect   (m4int32_t ai_indParam);
	m4return_t PrepareOrdinal         (m4int32_t ai_indParam);
	m4return_t PrepareOrdinalPKInsert (m4int32_t ai_indParam);
	m4return_t PrepareOrdinalPKDelete (m4int32_t ai_indParam);
	m4return_t PrepareOrdinalPK       (m4int32_t ai_indParam, m4bool_t ai_bExcludeStartDates);
	m4return_t PrepareValidatePK (void);
	m4return_t PrepareValidateNoPK (void);
	m4return_t PrepareValidateFKs (void);
	m4return_t PrepareValidateFRs (void);
	m4return_t PrepareGetHistory (void);
	m4return_t PrepareGetCorrectedHistory (void);
	m4return_t PrepareCursorQuery (m4pcchar_t ai_pStmt = NULL);
	m4return_t PrepareValidateSecurity (void);
	m4return_t PrepareValidateSecurityInOtherConnection (void);
	m4return_t PrepareAuditory (void);
	m4return_t PrepareValidateOrganization (void);
	m4return_t PrepareTramitExist (m4bool_t ai_bParent, m4bool_t ai_bLock, m4bool_t ai_bDtLastUpdate);
	m4return_t PrepareTramitDelete (void);
	m4return_t PrepareTramitInsert (m4pcchar_t ai_pccOperation);
	m4return_t PrepareTramitInsertObject (void);
	m4return_t PrepareInheritExist (m4bool_t ai_bParent, m4bool_t ai_bLock);
	m4return_t PrepareInheritDelete (void);
	m4return_t PrepareInheritInsert (m4pcchar_t ai_pccOperation);
	m4return_t PrepareInheritValidate (void);
	m4return_t PrepareTranslationInsert (void);
	m4return_t PrepareTranslationValidatePK (void);
	m4return_t PrepareTranslationUpdatePK (void);
	m4return_t PrepareUpdateVersion (void);
	m4return_t PrepareSingleInsert (m4pcchar_t &ao_pccRealObject);
	m4return_t PrepareSingleUpdate (void);
	m4return_t PrepareSingleDelete (void);
	m4return_t PrepareInsert (void);
	m4return_t PrepareUpdate (void);
	m4return_t PrepareDelete (void);

	m4return_t BindParamOrdinalPKInsert (m4int32_t ai_indParam);
	m4return_t BindParamOrdinalPKDelete (m4int32_t ai_indParam);
	m4return_t BindParamOrdinalPK       (m4int32_t ai_indParam, m4bool_t ai_bExcludeStartDates);
	m4return_t BindParamValidatePK (void);
	m4return_t BindParamValidateNoPK (void);
	m4return_t BindParamValidateFKs (void);
	m4return_t BindParamValidateFRs (void);
	m4return_t BindParamValidateComp (void);
	m4return_t BindParamGetHistory (void);
	m4return_t BindParamGetCorrectedHistory (void);
	m4return_t BindParamCursorQuery (void);
	m4return_t BindParamValidateSecurity (void);
	m4return_t BindParamValidateSecurityInOtherConnection (void);
	m4return_t BindParamAuditory (void);
	m4return_t BindParamValidateOrganization (void);
	m4return_t BindParamTramitExist (m4bool_t ai_bParent);
	m4return_t BindParamTramitDelete (void);
	m4return_t BindParamTramitInsert (void);
	m4return_t BindParamTramitInsertObject (void);
	m4return_t BindParamInheritExist (void);
	m4return_t BindParamInheritDelete (void);
	m4return_t BindParamInheritInsert (void);
	m4return_t BindParamInheritValidate (void);
	m4return_t BindParamTranslationInsert (void);
	m4return_t BindParamTranslationValidatePK (void);
	m4return_t BindParamTranslationUpdatePK (void);
	m4return_t BindParamUpdateVersion (void);
	m4return_t BindParamSingleInsert (m4bool_t ai_bBlobAsBinary);
	m4return_t BindParamSingleUpdate (void);
	m4return_t BindParamSingleDelete (void);
	m4return_t BindParamInsert (m4bool_t ai_bBlobAsBinary);
	m4return_t BindParamUpdate (void);
	m4return_t BindParamDelete (void);

	m4return_t ValidateInsert (ClRegister * ai_pReg = NULL);
	m4return_t ValidateUpdate (m4bool_t &ao_bPersist, ClRegister *ai_pReg = NULL);
	m4return_t ValidateDelete (ClRegister *ai_pReg = NULL);
	m4return_t ValidatePost (m4bool_t ai_bPersisted);

	m4return_t DefaultFunc (m4int32_t ai_indParam, ClRegister * ai_pReg, eFuncType_t ai_idDefaultFunc = M4LDB_FTN_NONE);
	m4return_t ConstraintFunc (m4int32_t ai_indParam);

	m4return_t ValidateDTLastUpdate (ClRegister *ai_pReg = NULL);
	m4return_t ValidateDates (void);
	m4return_t ValidateValue (m4int32_t ai_indParam, ClRegister * ai_pReg);
	m4return_t ValidateValues (ClRegister * ai_pReg);
	m4return_t ValidateDefaultLanguage (m4int32_t ai_indParam);
	m4return_t ValidatePK (m4bool_t ai_bFirstTime);
	m4return_t ValidateNoPK (void);
	m4return_t ValidateFK (ClLoad *ai_pCursor, m4int32_t ai_indFor, m4pcchar_t ai_pccType);
	m4return_t ValidateFR (ClLoad *ai_pCursor, m4int32_t ai_indFor);
	m4return_t ValidateFKs (void);
	m4return_t ValidateFRs (void);

	m4return_t ValidateSecurity (void);
	m4return_t ValidateSecurityInOtherConnection (void);
	m4return_t ValidateOrganization (void);

	m4return_t ExecuteTramitExist (m4bool_t &ao_rbExist, m4date_t* ao_pdDtLastUpdate);
	m4return_t ExecuteTramitDelete (void);
	m4return_t ExecuteTramitInsert (void);
	m4return_t ExecuteTramitInsertObject (void);
	m4return_t ExecuteInheritExist (m4bool_t &ao_rbExist);
	m4return_t ExecuteInheritDelete (void);
	m4return_t ExecuteInheritInsert (void);
	m4return_t ExecuteInheritValidate (m4bool_t &ao_rbExist);
	m4return_t ExecuteTranslationInsert (void);
	m4return_t ExecuteTranslationValidatePK (m4bool_t &ao_rbExist);
	m4return_t ExecuteTranslationUpdatePK (void);
	m4return_t ExecuteUpdateVersion (void);

	m4return_t CheckLanguage (m4int32_t ai_indParam, ClRegister * ai_pReg, m4bool_t *ao_pbDiff = NULL);
	m4bool_t   CompareRegisters (m4int32_t ai_indReg1, m4int32_t ai_indReg2);

	m4return_t GetHistory (void);
	m4return_t GetCorrectedHistory (void);
	m4return_t ValidateCompleteness (void);
	m4return_t ValidateHistory (void);
	m4return_t ValidateHistoryNoLikeAdjacentRegister (m4int32_t ai_indRow);
	m4return_t ValidateHistoryAutomaticAdaptation (m4int32_t ai_indRow);
	m4return_t ValidateCorrection (void);
	m4return_t MoveOnCascade (ClTblMem *ai_pData, m4int32_t ai_indLgcTbl, m4int32_t ai_indRow, m4date_t ai_dDTStart, m4date_t ai_dDTEnd, m4bool_t ai_bCorr, m4bool_t ai_bOperation);
	m4return_t MoveOnHistory (m4date_t ai_dDTStart, m4date_t ai_dDTClose, m4date_t ai_dOldDTStart = 0.0);
	m4return_t DeleteOnCascade (ClTblMem *ai_pData, m4int32_t ai_indLgcTbl, m4bool_t ai_bApp, m4bool_t ai_bCorr, m4bool_t ai_bOperation);
	m4return_t UpdatePKOnCascade (ClTblMem *ai_pData, m4int32_t ai_indLgcTbl, ClLoad *ai_pCursor);
	m4return_t CompleteHistoryCorrectionDate (void);
	m4return_t CloseCorrectedRegister (m4date_t ai_dDTStart, m4date_t ai_dDTStartCorr, m4date_t ai_dDTEndCorr, eRegType_t ai_eRegType);
	m4return_t DeleteProjection ( const struct stHistory & ai_oReg );
	m4return_t InsertProjection (m4int32_t ai_indReg);
	m4return_t GetHistories (void);
	m4return_t ValidateCorrectedDates (void);
	m4return_t ValidateTemporalityAndCorrection (void);

	m4return_t InsertAuditory (void);
	m4return_t InsertTranslationAuditory (m4pcchar_t ai_pccOperation);

	virtual m4return_t ComposePK (string &ai_onPKString, string &ai_oiPKString);
	m4return_t ComposeFK (string &ai_onFKString, string &ai_oiFKString, ClLoad *ai_pCursor);
	m4return_t ComposeRPFields (string &ai_onFields, string &ai_oiFields);

	m4return_t ComposePKWithoutNames (string &ai_oiPKString);
	void InsertInTransaction (void);
	m4bool_t FindInTransaction (m4date_t ai_dDTStart, m4date_t ai_dDTStartCorr = 0);
	m4bool_t FindPersistInPost (ClTblMem *ai_pTblMem, m4int32_t ai_lRow, m4int32_t ai_indLgcTbl);

	m4return_t ExecuteOrdinalSelect  ( m4int32_t ai_indParam ) ;
	m4return_t ExecuteOrdinalPKInsert( m4int32_t ai_indParam ) ;
	m4return_t ExecuteOrdinalPKDelete( m4int32_t ai_indParam ) ;
	m4return_t ExecuteOrdinalPK      ( m4int32_t ai_indParam, m4bool_t ai_bExcludeStartDates ) ;
	m4return_t ComposeOrdinalPK      ( m4int32_t ai_indParam, m4bool_t ai_bExcludeStartDates, m4pchar_t ao_pcPkMD5, m4int_t ai_PkMD5Length ) ;

	friend class ClLConn;
	friend class ClLoad;

public:

	ClPersistBase (ClGlobVars *ai_pGlobVars, ClPersist * ai_pSourcePersist, m4bool_t ai_bIsInternal, m4bool_t ai_bUseTramitation, m4bool_t ai_bUseInheritance, m4bool_t ai_bUseTranslation);
	virtual ~ClPersistBase (void);

	m4return_t Open (eStmType_t ai_eType, m4int32_t ai_idRConn, ClMetaSentence *ai_pMtSent, ClDinArr *ai_pParam, struct stMtLgcTbl *ai_pMtLgcTbl);
	m4return_t Destroy (void);
	m4return_t Close (void);
	m4return_t Prepare (void);
	m4return_t BindParam (m4bool_t ai_bBlobAsBinary);
	m4return_t Execute (m4bool_t &ao_bPersist, ClRegister * ai_pReg = NULL);

	virtual m4return_t Defuse (ClLConn *ai_pLConn);
	m4return_t Release (void);

	virtual void InitStt (void);

	// Funciones inline.
	virtual m4id_t GetIdLgcTbl (void) {
		return (m_pMtLgcTbl->m_idLgcTbl);
	}

	void SetMainTable(m4id_t ai_idMainTable)
	{
		m_idMainTable = ai_idMainTable;
	}

protected:
	ClStmt *FindAuxCursor (m4int32_t ai_idCursor)
	{
		itMpPValCursor it = m_pmpValCur->find (ai_idCursor);
		return ((it == m_pmpValCur->end ()) ? NULL : (*it).second);
	}

	virtual m4bool_t HasOrdinal(void){
		return (m_dOrdinal)?M4_TRUE:M4_FALSE;
	}

	virtual m4int32_t GetIndLgcTbl (void) {
		return (m_pMtLgcTbl->m_indLgcTbl);
	}

	friend class ClPersist;
};

typedef ClPersistBase*		PClPersistBase;


#endif

