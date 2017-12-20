//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                clpersis.hpp
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

#ifndef __CLPERSIS_HPP__
#define __CLPERSIS_HPP__


#include "clpersbs.hpp"
#include "cldinarr.hpp"


typedef vector<PClPersistBase>							VcPClPersistBase;
typedef VcPClPersistBase*								PVcPClPersistBase;
typedef VcPClPersistBase::const_iterator				itVcPClPersistBase;


class ClPersist : public ClStmt
{
protected:

	PVcPClPersistBase m_pvcPersist;

	struct stMtLgcTbl m_oMtLgcTbl;

	ClPersist *m_pInherit;
	
	ClPersist *m_pMainLeaf;
	m4bool_t m_bIsRootInRecursion;
	m4bool_t m_bChangesIntoMainLeaf;
	m4bool_t m_bExecuteChangesIntoInheritance;
	m4bool_t m_bForcePersistInUpdate;
	m4bool_t m_bUseTramitation;
	m4bool_t m_bUseInheritance;
	m4bool_t m_bUseTranslation;

	void		SetDataValueInRecord (PClRegister ai_pReg, m4bool_t ai_bShowInfo);

	m4bool_t IsNeeded					(ClRegister* ai_pReg, PMpIdData &ao_pmpIdFld);
	m4bool_t IsNeededInUpdateSelect		(ClRegister* ai_pReg);

	m4return_t AnalizeSecurity (void);

	virtual m4id_t GetIdLgcTbl (void) {
		return (m_oMtLgcTbl.m_idLgcTbl);
	}

	virtual m4int32_t GetIndLgcTbl (void) {
		return (m_oMtLgcTbl.m_indLgcTbl);
	}

	virtual m4return_t ComposePK (string &ai_onPKString, string &ai_oiPKString) {
		if (m_oMtLgcTbl.m_pMaster) {
			m_oMtLgcTbl.m_pMaster->ComposePK (ai_onPKString, ai_oiPKString);
		}

		return (M4_SUCCESS);
	}

public:

	ClPersist (ClGlobVars *ai_pGlobVars, m4bool_t ai_bIsInternal, m4bool_t ai_bUseTramitation, m4bool_t ai_bUseInheritance, m4bool_t m_bUseTranslation);
	virtual ~ClPersist (void);

	ClPersist * GetpMainLeaf () {
		return m_pMainLeaf;
	}

	//Bug 0084837
	void SeteStmTypeAuditory (eStmType_t ai_eStmTypeAuditory) {
		m_oMtLgcTbl.m_eStmTypeAuditory = ai_eStmTypeAuditory;
	}

	void SetpMainLeaf (ClPersist * ai_pMainLeaf) {
		m_pMainLeaf = ai_pMainLeaf;
	}
	m4bool_t GetIsRootInRecursion (void) {
		return m_bIsRootInRecursion;
	}
	void SetIsRootInRecursion (m4bool_t ai_bIsRootInRecursion) { 
		m_bIsRootInRecursion = ai_bIsRootInRecursion;
	}
	m4bool_t GetChangesIntoMainLeaf (void) {
		return m_bChangesIntoMainLeaf;
	}
	void SetChangesIntoMainLeaf (m4bool_t ai_bChangesIntoMainLeaf) {
		m_bChangesIntoMainLeaf = ai_bChangesIntoMainLeaf;
	}
	m4bool_t GetForcePersistInUpdate (void) {
		return m_bForcePersistInUpdate;
	}
	void SetForcePersistInUpdate (m4bool_t ai_bForcePersistInUpdate) {
		m_bForcePersistInUpdate = ai_bForcePersistInUpdate;
	}

	m4return_t CreateParam (void);

	m4return_t Open (eStmType_t ai_eType, m4int32_t ai_idRConn, ClMetaSentence *ai_pMtSent = NULL, m4bool_t ai_bMtSentOwner = M4_TRUE, m4id_t ai_idMainTable = NULL);
	m4return_t Open (ClDinArr *ai_pParam);
	m4return_t Destroy (void);
	m4return_t Close (void);
	m4return_t Prepare (void);
	m4return_t BindParam (m4bool_t ai_bBlobAsBinary = M4_FALSE);
	m4return_t Execute2 (void);
	m4return_t Execute2 (m4VariantType *ai_pParam, m4int32_t ai_numParam, ClTblMem *ai_poTbl = NULL); //, m4int32_t ai_lMask);
	m4return_t Execute (ClRegister * ai_pReg = NULL, m4bool_t *ao_pbPersist = NULL, m4uint32_t ai_indOperation = 0, m4bool_t *ao_pbInvRS = NULL);

	virtual void InitStt (void);

	virtual m4return_t Defuse (ClLConn *ai_pLConn);
	m4return_t Release (void);

	ClPersistBase *GetMaster (void)
	{
		return (m_oMtLgcTbl.m_pMaster);
	}
};


#endif

