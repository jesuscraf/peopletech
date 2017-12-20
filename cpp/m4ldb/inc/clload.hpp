//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                clload.hpp   
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

#ifndef _CLLOAD_HPP_
#define _CLLOAD_HPP_

#include "clstatem.hpp"
#include "clmtsent.hpp"
#include "cllconn.hpp"

class ClTblMem;
class ClRecordSet;
class ClPersist;
class ClPersistBase;
class ClItem;

typedef m4pcchar_t m4id_t;


class ClLoad : public ClStmt
{
protected:

	m4int32_t m_InitCol;
	m4int32_t m_EndCol;
	VcVirtualParam		m_oVColumns;

	// Vector para los indices de los parámetros de las select de validación.
	PVcIndParVal m_pvcIndParVal;

	ClDinArr *m_pColumns;

	ClLoad *m_pAuxLoad;

	ClRecordSet *m_pAuxRS;

	ClPersistBase *m_pMainPB;

	m4return_t TranslateSelect (string &ao_stStmSQL)
	{
		return m_pMtSent->TranslateSentence(ao_stStmSQL);
	}

	void AuditRows( m4uint32_t iTotalRows );
	m4return_t GetLongData (m4int32_t ai_indCol, m4int32_t ai_lM4Type, ClTblMem *ai_pTbl);
	m4return_t GetLongData (m4int32_t ai_indCol, m4int32_t ai_lM4Type, ClItem *ai_pItem);
	m4return_t GetBlobData (m4int32_t ai_indCol, m4int32_t ai_lM4Type, ClItem *ai_pItem);

	m4return_t GetParamValue (m4int32_t ai_hItem, m4VariantType *ao_pValue);
	m4return_t GetFatherValue (m4int32_t ai_hItem, m4VariantType *ao_pValue);
	m4return_t _BindCol ();
	m4return_t _BindParam (m4bool_t ai_bUseOldValue = M4_FALSE);
	friend class ClStmt;

	virtual m4bool_t IsPartialLoad (void) {
		if (m_pMtSent) {
			return (m_pMtSent->IsPartialLoad ());
		}

		return (M4_FALSE);
	}

	virtual m4id_t GetIdLgcTbl (void);
	virtual m4int32_t GetIndLgcTbl (void);
	virtual m4return_t ComposePK (string &ai_onPKString, string &ai_oiPKString);

protected:

	m4bool_t m_bVerifyData;
	m4bool_t m_bVerifyDTApp;
	m4bool_t m_bVerifyDTCorr;
	m4bool_t m_bVerifyOperation;
	m4bool_t m_bAllowPreviousIni;
	eOrgType_t m_eVerifyOrg;
	m4int32_t m_indForLgcTbl;

	friend class ClPersistBase;

public:
	ClLoad (ClGlobVars *ai_pGlobVars, m4bool_t ai_bIsInternal = M4_TRUE, ClPersistBase *ai_pMainPB = NULL);
	virtual ~ClLoad (void);

	m4return_t Open (m4int32_t ai_idRConn, ClMetaSentence *ai_pMtSent, m4bool_t ai_bMtSentOwner = M4_TRUE, ClDinArr *ai_pParam = NULL);
	m4return_t Destroy (void);
	m4return_t Close (void);
	m4return_t Prepare (string &ao_stStmSQL);
	m4return_t BindParam (m4bool_t ai_bUseOldValue = M4_FALSE);
	m4return_t BindCol (void);

	m4return_t Prepare (m4pchar_t ai_pStmSQL)
	{
		return (ClStmt::Prepare (ai_pStmSQL));
	}

	m4return_t Execute ()
	{
		return (ClStmt::Execute(0));
	}

	m4bool_t HasAuxLoad (void)
	{
		return ((m_pAuxLoad) ? M4_TRUE : M4_FALSE);
	}

	ClDinArr *GetpColumns()
	{
		if (m_pAuxLoad){
			return (m_pAuxLoad->GetpColumns ());
		}
		return (m_pColumns);
	}

	m4bool_t HasLongItem (void)
	{
		if ((m_pColumns) && 
			(m_pColumns->HasLongItem())) {
			return (M4_TRUE);
		}

		if (m_pAuxLoad){
			return (m_pAuxLoad->HasLongItem ());
		}

		return (M4_FALSE);
	}

	m4return_t Execute (m4VariantType *ai_pParam, m4int32_t ai_numParam, ClRegister *ai_pReg = NULL, m4int32_t ai_iFirstData = 0, m4uint32_t ai_iTimeOut = 0);
	m4return_t GetData (ClTblMem *ao_pTbl, m4int32_t *aio_piNumData = NULL);
	m4return_t GetData (m4int32_t *ao_iNumData = NULL, m4int32_t ai_iFirstData = 0, m4int32_t ai_indFirstReg = M4LDB_NULL_INDEX);

	m4bool_t NewCursor (m4int32_t ai_iFirstData);

	virtual void InitStt (void);

	virtual m4return_t Defuse (ClLConn *ai_pLConn);

	void ClearMetaSentence (void)
	{
		if (m_pMtSent) {
			m_pMtSent->Clear ();
		}	
	}
};

#endif

