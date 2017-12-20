//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                clglbvar.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                09-03-2011
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


#include "clglbvar.hpp"


ClGlobVars::ClGlobVars (void)
{
	m_lReferences = M4LDB_NULL_VALUE;
	m_pLConn = NULL;
	m_idRConn = 0;
	m_iLanguage = M4CL_LANGUAGE_DEFAULT;
	m_pIdUser = NULL;
	m_pIdRole = NULL;
	m_pIdUserAuditory = NULL;
	m_pIdRoleAuditory = NULL;
	m_pIdUserLogged = NULL;
	m_pIdRoleLogged = NULL;

	m_pIdSecRole = NULL;
	m_pIdRSM = NULL;
	m_pMCR = NULL;
	m_hNode = 0;
	m_bInvStmt = M4_FALSE;
	m_bRetry = M4_FALSE;
	m_lNumberOfAttempts = M4LDB_NULL_INDEX;
	m_pRecordSet = NULL;
	m_bPostValidate = M4_FALSE;
	m_bPostValidateOperations = M4_TRUE;
	m_bPostValidateFKs = M4_TRUE;

	m_eDDBB = M4DBW_ORACLE_DDBB; 

	m_idExecution = M4LDB_NULL_VALUE;
	m_idOrder = M4LDB_NULL_VALUE;
	m_dDTOper = 0.0;
	m_idTypeOper[0] = '\0';
	m_idTypeOper[1] = '\0';
	m_idTypeUpd[0] = '\0';
	m_idTypeUpd[1] = '\0';
	m_pAddInfo = NULL;
	m_iStatementSubType = M4LDB_NULL_VALUE;
}


ClGlobVars::~ClGlobVars (void)
{
	if (m_pIdUser)
	{
		delete [] m_pIdUser;
	}
	m_pIdUser = NULL;

	if (m_pIdRole)
	{
		delete [] m_pIdRole;
	}
	m_pIdRole = NULL;

	if (m_pIdUserAuditory)
	{
		delete [] m_pIdUserAuditory;
	}

	m_pIdUserAuditory = NULL;

	if (m_pIdRoleAuditory)
	{
		delete [] m_pIdRoleAuditory;
	}
	m_pIdRoleAuditory = NULL;

	if (m_pIdUserLogged)
	{
		delete [] m_pIdUserLogged;
	}
	m_pIdUserLogged = NULL;

	if (m_pIdRoleLogged)
	{
		delete [] m_pIdRoleLogged;
	}
	m_pIdRoleLogged = NULL;
}


void ClGlobVars::Clear (void)
{
	m_pLConn = NULL;
	m_idRConn = 0;
	m_iLanguage = M4CL_LANGUAGE_DEFAULT;
	m_pIdSecRole = NULL;
	m_pIdRSM = NULL;
	m_pMCR = NULL;
	m_hNode = 0;
	m_pRecordSet = NULL;
	m_bRetry = M4_FALSE;
	m_lNumberOfAttempts = M4LDB_NULL_INDEX;
	m_bInvStmt = M4_FALSE;
	m_bPostValidate = M4_FALSE;
	m_bPostValidateOperations = M4_TRUE;
	m_bPostValidateFKs = M4_TRUE;
	m_eDDBB = M4DBW_ORACLE_DDBB; 
	m_iStatementSubType = M4LDB_NULL_VALUE;
}


void ClGlobVars::Set (ClLConn *ai_pLConn, m4int32_t ai_idRConn, m4uint32_t ai_iLanguage, m4id_t ai_pIdRole, m4id_t ai_pIdRSM, ClCompiledMCR *ai_pMCR, m4int32_t ai_hNode, ClAccessRecordSet *ai_pRecordSet, m4id_t ai_pIdUser, m4id_t ai_pIdDebugUser)
{
	m_pLConn = ai_pLConn;
	m_idRConn = ai_idRConn;
	m_iLanguage = ai_iLanguage;
	m_pIdSecRole = ai_pIdRole;
	m_pIdRSM = ai_pIdRSM;
	m_pMCR = ai_pMCR;
	m_hNode = ai_hNode;
	m_pRecordSet = ai_pRecordSet;
	m_bRetry = M4_FALSE;
	m_lNumberOfAttempts = M4LDB_NULL_INDEX;
	m_bInvStmt = M4_FALSE;
	m_bPostValidate = M4_FALSE;
	m_bPostValidateOperations = M4_TRUE;
	m_bPostValidateFKs = M4_TRUE;
	m_eDDBB = M4DBW_ORACLE_DDBB; 

	/* Bug 0178469
	El usuario depurador es el que se graba en persistencia
	*/
	if (ai_pIdDebugUser != NULL)
	{
		if (m_pIdUser == NULL)
		{
			m_pIdUser = new m4char_t [M4LDB_ID_MAX_SIZE + 1];
		}
		strcpy (m_pIdUser, ai_pIdDebugUser);

		if (m_pIdUserAuditory == NULL)
		{
			m_pIdUserAuditory = new m4char_t [M4LDB_ID_MAX_SIZE + 1];
		}
		strcpy (m_pIdUserAuditory, ai_pIdDebugUser);
	}

	/* Bug 0178469
	El usuario logado que es el de validación de organizaciones debe ser el original
	*/
	if (ai_pIdUser != NULL)
	{
		if (m_pIdUserLogged == NULL)
		{
			m_pIdUserLogged = new m4char_t [M4LDB_ID_MAX_SIZE + 1];
		}
		strcpy (m_pIdUserLogged, ai_pIdUser);
	}

	if (ai_pIdRole != NULL)
	{
		if (m_pIdRole == NULL)
		{
			m_pIdRole = new m4char_t [M4LDB_ID_MAX_SIZE + 1];
		}
		strcpy (m_pIdRole, ai_pIdRole);

		if (m_pIdRoleAuditory == NULL)
		{
			m_pIdRoleAuditory = new m4char_t [M4LDB_ID_MAX_SIZE + 1];
		}
		strcpy (m_pIdRoleAuditory, ai_pIdRole);
		
		if (m_pIdRoleLogged == NULL)
		{
			m_pIdRoleLogged = new m4char_t [M4LDB_ID_MAX_SIZE + 1];
		}
		strcpy (m_pIdRoleLogged, ai_pIdRole);
	}
}


void ClGlobVars::SetAuditory (m4char_t ai_idTypeOper, m4char_t ai_idTypeUpd, m4pchar_t ai_pAddInfo)
{
	m_idExecution = m_pLConn->m_idExecution;
	m_idOrder = M4LDB_NULL_VALUE;
	m_dDTOper = 0.0;
	m_idTypeOper[0] = ai_idTypeOper;
	m_idTypeUpd[0] = ai_idTypeUpd;

	if (ai_pAddInfo != NULL)
	{
		m_pAddInfo = ai_pAddInfo;
	}
}


m4int8_t	ClGlobVars::GetItemM4Type	(m4int32_t ai_hItem, ClCompiledMCR *ai_pMCR)
{
	m4uint8_t	iItemM4Type = iItemM4Type = ai_pMCR->GetItemM4Type(ai_hItem);
	m4uint8_t	iNormalizedItemM4Type = M4LDB_M4_UNICODE_TYPE( iItemM4Type ) ;

	if( iNormalizedItemM4Type != iItemM4Type )
	{
		// Por defecto las cadenas se promueven al entorno en el que estén
		iItemM4Type = iNormalizedItemM4Type;

		if( g_iEncodingMode == M4LDB_ENCODING_MIXED )
		{
			// En unicode, si el item es cadena y tiene campo de lectura se busca para tomar el tipo meta4
			m4pcchar_t	pccObject = ai_pMCR->GetItemReadObjectId( ai_hItem );
			m4pcchar_t	pccField = ai_pMCR->GetItemReadFieldId( ai_hItem );

			if( pccObject != NULL && *pccObject !='\0' && pccField != NULL && *pccField !='\0' )
			{
				m4int32_t indFieldRegister = m_pLConn->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register.Index;
				m4int32_t indFieldCurrent = m_pLConn->m_paRS[M4LDB_SYS_DICT_FIELDS]->Current.Index;
				m4int32_t indRealTblCurrent = m_pLConn->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Index;
				m4int32_t indLgcTblCurrent = m_pLConn->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Index;

				m4return_t result = m_pLConn->LoadDictionaryChannel( pccObject, NULL, M4_FALSE );

				if( result != M4_ERROR )
				{
					m4int32_t indField = m_pLConn->FindIndexById( M4LDB_SYS_DICT_FIELDS, pccField );

					if( indField != M4LDB_NULL_INDEX )
					{
						m4pcchar_t pccIdType = M4LDB_ID( m_pLConn->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indField].Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_TYPE].Value.GetWithOutSec() );

						if( pccIdType != NULL && *pccIdType !='\0' )
						{
							m4int32_t indTypeRegister = m_pLConn->m_paRS[M4LDB_SYS_DICT_EXTENDED_TYPES]->Register.Index;
							m4int32_t indType = m_pLConn->FindIndexById( M4LDB_SYS_DICT_EXTENDED_TYPES, pccIdType ) ;

							if( indType != M4LDB_NULL_INDEX )
							{
								m4int8_t iFieldM4Type = M4LDB_M4_TYPE( (m4int8_t) M4LDB_NUMBER( m_pLConn->m_paRS[M4LDB_SYS_DICT_EXTENDED_TYPES]->Register[indType].Item[M4LDB_ITEM_SYS_DICT_EXTENDED_TYPES_ID_M4_TYPE].Value.GetWithOutSec() ) ) ;

								if( iFieldM4Type != iItemM4Type )
								{
									if( M4LDB_M4_UNICODE_TYPE( iFieldM4Type ) == iNormalizedItemM4Type )
									{
										// Si el tipo es el mismo salvo unicode se usa el del campo
										iItemM4Type = iFieldM4Type;
									}
								}
							}
							m_pLConn->m_paRS[M4LDB_SYS_DICT_EXTENDED_TYPES]->Register.MoveTo( indTypeRegister );
						}
					}
				}
				m_pLConn->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.MoveTo( indLgcTblCurrent );
				m_pLConn->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.MoveTo( indRealTblCurrent );
				m_pLConn->m_paRS[M4LDB_SYS_DICT_FIELDS]->Current.MoveTo( indFieldCurrent );
				m_pLConn->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register.MoveTo( indFieldRegister );
			}
		}
	}

	return iItemM4Type;
}


m4bool_t ClGlobVars::HasNotValidation (m4int32_t idVal)
{
	m4int32_t hItem = m_pMCR->GetNodeBDLChecksItem (m_hNode);

	if (hItem)
	{
		m4int32_t lMask = M4LDB_NUMBER (m_pRecordSet->Item[(ClHandle) hItem].Value.Get ());

		return ((lMask & idVal) ? M4_TRUE : M4_FALSE);
	}

	return (M4_FALSE);
}
