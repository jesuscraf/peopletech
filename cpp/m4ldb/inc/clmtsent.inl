//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                clmtsent.inl
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




m4return_t ClMetaSentence::AddInfoColumns( PVcClMtColumn ai_pVcClMtCol,PVcClMtParam ai_pvcParamCol,PVcClMtField ai_pvcFieldCol,string *ai_stColumns,m4uint32_t ai_iColumnStart)
{
	return _AddInfoColumns(ai_pVcClMtCol,ai_pvcParamCol,ai_pvcFieldCol,ai_stColumns,M4_FALSE, ai_iColumnStart);
}


m4return_t ClMetaSentence::AddInfoLongColumns( PVcClMtColumn ai_pVcClMtCol,PVcClMtParam ai_pvcParamCol,PVcClMtField ai_pvcFieldCol,string *ai_stColumns,m4uint32_t ai_iColumnStart)
{
	return _AddInfoColumns(ai_pVcClMtCol,ai_pvcParamCol,ai_pvcFieldCol,ai_stColumns,M4_TRUE, ai_iColumnStart);
}


m4id_t ClMetaSentence::GetAliasByIdx( m4int32_t ai_indAlias )
{
	return ( (*m_pvcFrmAls)[ai_indAlias]->m_stAlias.c_str());
}


m4bool_t ClMetaSentence::IsReal (void)
{
	return (m_bReal);
}


m4bool_t ClMetaSentence::IsAPISQL (void)
{
	return (m_bAPISQL);
}


m4bool_t ClMetaSentence::GetIsInsertIntoSelect (void) const
{
	return m_bIsInsertIntoSelect ;
}

void ClMetaSentence::SetIsInsertIntoSelect (m4bool_t ai_bIsInsertIntoSelect)
{
	m_bIsInsertIntoSelect = ai_bIsInsertIntoSelect;
}

m4bool_t ClMetaSentence::GetIsUpdateSelect (void) const
{
	return m_bIsUpdateSelect;
}

void ClMetaSentence::SetIsUpdateSelect (m4bool_t ai_bIsUpdateSelect)
{
	m_bIsUpdateSelect = ai_bIsUpdateSelect;
}

ClDinArr * ClMetaSentence::GetParam (void)
{
	return (m_pParam);
}


void ClMetaSentence::SetParam (ClDinArr *ai_pParam)
{
	m_pParam = ai_pParam;
}


eStmType_t ClMetaSentence::GetStmType (void)
{
	return (m_eStmType);
}


//Bug 0071661.
void ClMetaSentence::SetNoWait (m4bool_t ai_bNoWait)
{
	m_bNoWait = ai_bNoWait;
}


m4bool_t ClMetaSentence::GetNoWait	(void)
{
	return m_bNoWait;
}


void ClMetaSentence::SetForUpdate (m4bool_t ai_bForUpdate)
{
	m_bForUpdate = ai_bForUpdate;
}


m4bool_t ClMetaSentence::GetForUpdate	(void)
{
	return m_bForUpdate;
}


void ClMetaSentence::SetDistinct		(m4bool_t ai_bIsDistinct)
{
	m_bIsDistinct= ai_bIsDistinct;
}


m4bool_t ClMetaSentence::GetDistinct		(void)
{
	return m_bIsDistinct;
}


m4int32_t ClMetaSentence::GetNumObject (void)
{
	if (m_pvcFrmObj) {
		return (m_pvcFrmObj->size());
	}
	return (0);
}


m4id_t ClMetaSentence::GetIdObjectBasis (void)
{
	m4int32_t indBasis = GetPosBasis ();

	if (indBasis != M4LDB_NULL_INDEX)
	{
		return (GetIdObjectByPos (indBasis));
	}

	// Por defecto siempre la primera.
	return (GetIdObjectByPos (0));
}


void ClMetaSentence::SetIndDtLastUpdate(m4int32_t ai_indDtLU)
{
	m_indDtLastUpdate = ai_indDtLU;
}


m4int32_t ClMetaSentence::GetIndDtLastUpdate(void)
{
	return m_indDtLastUpdate;
}


m4int32_t ClMetaSentence::GetObjectPosByAliasPos	(m4int32_t ai_indAlias)
{
	m4int32_t indAlias = GetNewAliasPosByAliasPos(ai_indAlias);
	if (indAlias == -1)
	{
		return -1;
	}
	return (*m_pvcFrmAls)[indAlias]->m_indObj;
}


m4bool_t ClMetaSentence::IsObjectBasisByAliasPos(m4int32_t ai_indAlias)
{
	m4int32_t indObj = GetObjectPosByAliasPos(ai_indAlias);
	return (*m_pvcFrmObj)[indObj]->m_bIsBasis;
}


ClGlobVars	* ClMetaSentence::GetpGlobVars (void)
{
	return (m_pGlobVars);
}


m4int32_t ClMetaSentence::GetNumAlias (void)
{
	if (m_pvcFrmAls)
	{
		return (m_pvcFrmAls->size());
	}
	return (0);
}


m4int32_t ClMetaSentence::GetNumFields (void)
{
	if (m_pvcFld)
	{
		return (m_pvcFld->size());
	}
	return (0);
}


m4int32_t ClMetaSentence::GetNumRelations (void)
{
	if (m_pvcRel)
	{
		return (m_pvcRel->size());
	}
	return (0);
}


ClMetaObject * ClMetaSentence::GetObjectByPos	(m4int32_t ai_Pos)
{
	return (*m_pvcFrmObj)[ai_Pos];
}


ClMetaAlias * ClMetaSentence::GetAliasByPos	(m4int32_t ai_Pos)
{
	return (*m_pvcFrmAls)[ai_Pos];
}


ClMetaRelat * ClMetaSentence::GetRelationByPos	(m4int32_t ai_Pos)
{
	return (*m_pvcRel)[ai_Pos];
}


ClMetaField * ClMetaSentence::GetFieldByPos	(m4int32_t ai_Pos)
{
	return (*m_pvcFld)[ai_Pos];
}


m4return_t ClMetaSentence::AddAlias	(ClMetaAlias *ai_pstAlias)
{
	if (m_pvcFrmAls)
	{
		m_pvcFrmAls->push_back (ai_pstAlias);
		return (M4_SUCCESS);
	}
	return (M4_ERROR);
}


m4bool_t ClMetaSentence::HasValidation (void)
{
	return (m_bHasValidation);
}


m4bool_t ClMetaSentence::HasRange (void)
{
	return (m_bHasRange);
}


m4bool_t ClMetaSentence::IsPartialLoad (void)
{
	return (m_bPartialLoad);
}


void ClMetaSentence::SetAudMask (m4int32_t ai_lAudMask)
{
	m_lAudMask = ai_lAudMask;
}


m4int32_t ClMetaSentence::GetAudMask (void)
{
	return (m_lAudMask);
}


void ClMetaSentence::SetDontUseAlias	(m4bool_t ai_bDUA)
{
	m_bDontUseAlias = ai_bDUA;
}


m4bool_t ClMetaSentence::GetDontUseAlias	(void)
{
	return m_bDontUseAlias;
}


ofstream &operator << (ofstream &ofs,ClMetaSentence &MetaSnt)
{
	MetaSnt.Dump(ofs);
	return ofs;
}


void ClMetaSentence::SetOr (m4bool_t ai_bHasOr)
{
	m_bHasOr = ai_bHasOr;
}


m4bool_t ClMetaSentence::HasOr (void)
{
	return (m_bHasOr);
}


m4bool_t ClMetaSentence::ExistsInheritance (void)
{
	return (m_bExistsInheritance);
}

void ClMetaSentence::SetInheritance (void)
{
	m_bExistsInheritance = M4_TRUE;
}

