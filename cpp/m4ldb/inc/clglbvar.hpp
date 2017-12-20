//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                clglbvar.hpp   
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

#ifndef __CLGLOBVARS_HPP__
#define __CLGLOBVARS_HPP__


#include "defmain.hpp"
#include "cllconn.hpp"
#include "cldinarr.hpp"
#include "node_knl.hpp"
#include "m4mdrt.hpp"
#include "recorset.hpp"
#include "connmanager.hpp"


class ClLConn;
class ClCompiledMCR;
class ClAccessRecordSet;


class ClGlobVars
{
	m4int32_t m_lReferences;
	ClLConn *m_pLConn;

	m4int32_t m_idRConn;
	m4uint32_t m_iLanguage;

	m4id_t m_pIdSecRole;
	m4id_t m_pIdRSM;
	ClCompiledMCR *m_pMCR;
	m4int32_t m_hNode;
	m4bool_t m_bInvStmt;
	m4bool_t m_bRetry;
	m4int32_t m_lNumberOfAttempts;
	ClAccessRecordSet *m_pRecordSet;
	m4bool_t m_bPostValidate;
	m4bool_t m_bPostValidateOperations;
	m4bool_t m_bPostValidateFKs;

	eDDBBType_t m_eDDBB;
		
	// De estos necesitamos su dirección.
	m4pchar_t m_pIdUser;
	m4pchar_t m_pIdRole;
	//Bug 0084837
	m4pchar_t m_pIdUserAuditory;
	m4pchar_t m_pIdRoleAuditory;

	m4pchar_t m_pIdUserLogged;
	m4pchar_t m_pIdRoleLogged;

	// Auditoría.
	m4double_t m_idExecution;
	m4double_t m_idOrder;
	m4date_t m_dDTOper;
	// UNICODE DB
	m4char_t m_idTypeOper[2+1];
	m4char_t m_idTypeUpd[2+1];
	m4pchar_t m_pAddInfo;

	//Subtipo de statement.
	m4int32_t m_iStatementSubType;
	
public:

	ClGlobVars (void);

	~ClGlobVars (void);

	void Attach (void)
	{
		m_lReferences++;
	}

	void Detach (void)
	{
		m_lReferences--;
	}

	m4int32_t GetIdRConn (void)
	{
		return (m_idRConn);
	}

	m4uint32_t GetLanguage (void)
	{
		return (m_iLanguage);
	}

	m4int32_t GetReferences (void)
	{
		return (m_lReferences);
	}
	
	void Clear (void);

	void Set (ClLConn *ai_pLConn, m4int32_t ai_idRConn = 0, m4uint32_t ai_iLanguage = M4CL_LANGUAGE_DEFAULT, m4id_t ai_pIdRole = NULL, m4id_t ai_pIdRSM = NULL, ClCompiledMCR *ai_pMCR = NULL, m4int32_t ai_hNode = 0, ClAccessRecordSet *ai_pRecordSet = NULL, m4id_t ai_pIdUser = NULL, m4id_t ai_pIdDebugUser = NULL);

	void SetAuditory (m4char_t ai_idTypeOper, m4char_t ai_idTypeUpd, m4pchar_t ai_pAddInfo = NULL);

	m4return_t SetDDBBType (m4int32_t ai_idRConn)
	{
		m_idRConn = ai_idRConn;
		return (m_pLConn->GetConnManager()->GetDDBBType(ai_idRConn, m_pLConn, m_eDDBB));
	}

	eDDBBType_t GetDDBBType(void)
	{
		return (m_eDDBB);
	}

	ClLConn *GetpLConn (void)
	{
		return (m_pLConn);
	}

	m4pchar_t* GetpIdUserPointer (void)
	{
		return (&m_pIdUser);
	}

	m4pchar_t* GetpIdRolePointer (void)
	{
		return (&m_pIdRole);
	}

	m4pchar_t* GetpIdUserAuditoryPointer (void)
	{
		return (&m_pIdUserAuditory);
	}

	m4pchar_t* GetpIdRoleAuditoryPointer (void)
	{
		return (&m_pIdRoleAuditory);
	}

	m4pchar_t GetpIdUserLogged (void)
	{
		return (m_pIdUserLogged);
	}

	m4pchar_t GetpIdRoleLogged (void)
	{
		return (m_pIdRoleLogged);
	}

	m4pchar_t GetpIdExecution (void)
	{
		return ((m4pchar_t) &m_idExecution);
	}

	m4pchar_t GetpIdOrder (void)
	{
		return ((m4pchar_t) &m_idOrder);
	}

	m4pchar_t GetpDtOper (void)
	{
		return ((m4pchar_t) &m_dDTOper);
	}

	m4pchar_t GetpIdTypeOper (void)
	{
		return (m_idTypeOper);
	}

	m4pchar_t GetpIdTypeUpd (void)
	{
		return (m_idTypeUpd);
	}

	m4pchar_t GetpAddInfo (void)
	{
		return (m_pAddInfo);
	}

	m4id_t GetIdSecRole (void)
	{
		return (m_pIdSecRole);
	}

	m4id_t GetIdRSM (void)
	{
		return (m_pIdRSM);
	}
	
	ClCompiledMCR* GetpMCR (void)
	{
		return (m_pMCR);
	}
		
	void SetpMCR (ClCompiledMCR* ai_pMCR)
	{
		m_pMCR = ai_pMCR;
	}
		
	m4int32_t GethNode (void)
	{
		return (m_hNode);
	}

	void SethNode (m4int32_t ai_hNode)
	{
		m_hNode = ai_hNode;
	}

	m4bool_t IsInvStmt (void)
	{
		return (m_bInvStmt);
	}
	
	void SetInvStmt (m4bool_t ai_bInvStmt)
	{
		m_bInvStmt = ai_bInvStmt;
	}

	ClAccessRecordSet *GetpAccessRecordSet (void)
	{
		return (m_pRecordSet);
	}

	m4bool_t GetAutoApp (void)
	{
		return ((m_pMCR->GetChannelIsCacheable ()) ? M4_FALSE : ((m_pMCR->GetNodeIdAutoFilter (m_hNode) & M4CL_NODE_AUTO_FILTER_APPLICATION) ? M4_TRUE : M4_FALSE));
	}

	m4bool_t GetAutoCorr (void)
	{
		return ((m_pMCR->GetChannelIsCacheable ()) ? M4_FALSE : ((m_pMCR->GetNodeIdAutoFilter (m_hNode) & M4CL_NODE_AUTO_FILTER_CORRECTION) ? M4_TRUE : M4_FALSE));
	}

	m4int32_t	GetItemHandleByPos		(m4int32_t ai_indItem)
	{
		return m_pMCR->GetNodeItemHandleByPosition (m_hNode, (m4int16_t) ai_indItem);
	}

	m4int32_t	GetItemInverseConnector	(m4int32_t ai_hItem)
	{
		return m_pMCR->GetItemInverseConnector (ai_hItem);
	}

	m4int32_t	GetItemConnectorRelation (m4int32_t ai_hItemInvConn)
	{
		return m_pMCR->GetItemConnectorRelation (ai_hItemInvConn);
	}

	m4int32_t	GetNumItem (void)
	{
		return m_pMCR->GetNodeNumberOfItems (m_hNode);
	}

	m4int8_t	GetItemScope	(m4int32_t ai_hItem)
	{
		return m_pMCR->GetItemScope (ai_hItem);
	}

	m4int8_t	GetItemType		(m4int32_t ai_hItem)
	{
		return m_pMCR->GetItemType (ai_hItem);
	}

	m4int8_t	GetItemM4Type	(m4int32_t ai_hItem, ClCompiledMCR *ai_pMCR);

	m4int8_t	GetItemM4Type	(m4int32_t ai_hItem)
	{
		return GetItemM4Type(ai_hItem, m_pMCR);
	}

	m4int8_t	GetItemInternalType	(m4int32_t ai_hItem)
	{
		return m_pMCR->GetItemInternalType	(ai_hItem);
	}

	m4id_t		GetItemReadIdField (m4int32_t ai_hItem)
	{
		return m_pMCR->GetItemReadFieldId (ai_hItem);
	}

	m4pcchar_t  GetItemReadAlias (m4int32_t ai_hItem)
	{
		m4uint32_t iNodeHandle = m_pMCR->GetItemNodeHandle (ai_hItem);

		if (m_pMCR->GetNodeOwnSentence (iNodeHandle) == 1)
		{
			return "";
		}
		return (m_pMCR->GetItemReadObjectAlias (ai_hItem));
	}

	m4id_t		GetWriteIdObject (void)
	{
		return m_pMCR->GetNodeWriteObjectId (m_hNode);
	}

	m4int32_t	GetItemPrecision		(m4int32_t ai_hItem)
	{
		return m_pMCR->GetItemPrecision(ai_hItem);
	}

	m4int32_t	GetItemDBPrecision		(m4int32_t ai_hItem)
	{
		return m_pMCR->GetItemDBPrecision(ai_hItem);
	}

	m4int8_t	GetItemScale	(m4int32_t ai_hItem)
	{
		return m_pMCR->GetItemScale	(ai_hItem);
	}

	m4int32_t GetNodeFilterStartDateItem (void)
	{
		return m_pMCR->GetNodeFilterStartDateItem (m_hNode);
	}

	m4int32_t GetNodeFilterEndDateItem (void)
	{
		return m_pMCR->GetNodeFilterEndDateItem (m_hNode);
	}

	m4int32_t GetNodeFilterStartCorDateItem (void)
	{
		return m_pMCR->GetNodeFilterStartCorDateItem (m_hNode);
	}

	m4int32_t GetNodeFilterEndCorDateItem (void)
	{
		return m_pMCR->GetNodeFilterEndCorDateItem (m_hNode);
	}

	m4int32_t GetItemNodeHandle (m4int32_t ai_hItem)
	{
		return m_pMCR->GetItemNodeHandle (ai_hItem);
	}

	m4int32_t GetItemHandleConnectorFatherItem (m4int32_t ai_hItemInvConector)
	{
		return m_pMCR->GetItemConnectorFatherItemHandle (ai_hItemInvConector);
	}

	m4int32_t GetItemHandleByIdField (m4id_t ai_idField)
	{
		m4int32_t numItem = m_pMCR->GetNodeNumberOfItems (m_hNode);
		
		for (m4int32_t indItem=0; indItem<numItem; indItem++)
		{
			m4int32_t hItemAux = m_pMCR->GetNodeItemHandleByPosition (m_hNode, (m4int16_t) indItem);

			m4id_t idField = m_pMCR->GetItemWriteFieldId (hItemAux);

			if (!strcmpi (idField, ai_idField))
			{
				return (hItemAux);
			}
		}
		return (0);
	}

	m4int32_t GetNodeFatherHandle (void)
	{
		m4int32_t hConn = m_pRecordSet->GetpNode_ExtInfo ()->Parent.GetpNotFree ()->GetHandle ();

		if (hConn)
		{
			return (m_pMCR->GetNodeConnectorFatherNodeHandle (hConn));
		}
		return (M4LDB_NULL_VALUE);
	}

	m4int32_t GetItemHandleByIdItem (m4id_t ai_idItem,m4bool_t ai_bConnActivate)
	{
		if (!ai_bConnActivate)
		{
			return (m_pMCR->GetNodeItemHandleById (m_hNode, ai_idItem, M4_TRUE));
		}
		return (m_pMCR->GetNodeItemHandleById (GetNodeFatherHandle (), ai_idItem, M4_TRUE));
	}

	m4bool_t IsRetry (void)
	{
		return (m_bRetry);
	}

	void SetIsRetry (m4bool_t ai_bRetry)
	{
		m_bRetry = ai_bRetry;
	}

	m4int32_t GetNumberOfAttempts (void)
	{
		return (m_lNumberOfAttempts);
	}

	void SetNumberOfAttempts (m4int32_t ai_lNumberOfAttempts)
	{
		m_lNumberOfAttempts = ai_lNumberOfAttempts;
	}

	m4bool_t GetPostValidate (void)
	{
		return (m_bPostValidate);
	}

	void SetPostValidate (m4bool_t ai_bPostValidate)
	{
		m_bPostValidate = ai_bPostValidate;
	}

	m4bool_t GetPostValidateOperations (void)
	{
		return (m_bPostValidateOperations);
	}

	void SetPostValidateOperations (m4bool_t ai_bPostValidateOperations)
	{
		m_bPostValidateOperations = ai_bPostValidateOperations;
	}

	m4bool_t GetPostValidateFKs (void)
	{
		return (m_bPostValidateFKs);
	}

	void SetPostValidateFKs (m4bool_t ai_bPostValidateFKs)
	{
		m_bPostValidateFKs = ai_bPostValidateFKs;
	}

	m4int32_t GetStatementSubType ( void )
	{
		return m_iStatementSubType;
	}

	void SetStatementSubType ( m4int32_t ai_iStatementSubType )
	{
		m_iStatementSubType = ai_iStatementSubType;
	}

	m4bool_t HasNotValidation (m4int32_t idVal);
};


#endif

