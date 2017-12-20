//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                cldinarr.inl
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

#include "regpipe.hpp"
#include "m4cryptc.hpp"



ClDinArr::ClDinArr (ClGlobVars *ai_pGlobVars)
{
	m_pGlobVars = ai_pGlobVars;
	m_poPDContainer = NULL;
	m_pmpIdData = NULL;
	m_lNumData = M4LDB_NULL_VALUE;
	m_lMaxNumData = M4LDB_NULL_VALUE;
	m_lReferences = M4LDB_NULL_VALUE;
    m_bHasLongItem = M4_FALSE;
	m_RegPipe = NULL;

	_CreateVcClParam();
}

VcClParam * ClDinArr::GetVcClParam (void)
{
	return m_pVcClParam;
}

void ClDinArr::_CreateVcClParam (void)
{
	m_pVcClParam = new VcClParam;
	ClParam * pParamIdUser = new ClParam();
	pParamIdUser->SetBindParam(M4LDB_SEC_USER, NULL, M4LDB_M4_UNICODE_TYPE(M4CL_M4_TYPE_FIXED_STRING), M4LDB_ID_MAX_SIZE, 0);
	m_pVcClParam->push_back( pParamIdUser );

	ClParam * pParamIdRole = new ClParam();
	pParamIdRole->SetBindParam(M4LDB_APP_ROLE, NULL, M4LDB_M4_UNICODE_TYPE(M4CL_M4_TYPE_FIXED_STRING), M4LDB_ID_MAX_SIZE, 0);
	m_pVcClParam->push_back( pParamIdRole );

	//Para el insert en tramitación necesitamos dos nuevos parámetros:
	// 1. Indica si es insert (1) o modificación (2) (antes se habría borrado en tramitación)
	ClParam * pParamTramitOp = new ClParam();
	pParamTramitOp->SetBindParam(M4LDB_TRAMIT_OPERATION, NULL, M4CL_M4_TYPE_NUMBER, 1, 0);
	m_pVcClParam->push_back( pParamTramitOp );

	//0165456 se añael el TRAMIT_DT_LAST_UPDATE
	//2.- DtLasUpdate del registro borrado que ahora se está insertando.
	ClParam * pParamTramitDtLastUpdate = new ClParam();
	pParamTramitDtLastUpdate->SetBindParam(M4LDB_TRAMIT_DT_LAST_UPDATE, NULL, M4CL_M4_TYPE_TIMESTAMP, M4LDB_SIZE_TIMESTAMP, 0);
	m_pVcClParam->push_back( pParamTramitDtLastUpdate );

	ClParam * pParamDtLastUpdate = new ClParam();
	pParamDtLastUpdate->SetBindParam(M4LDB_DT_LAST_UPDATE, NULL, M4CL_M4_TYPE_TIMESTAMP, M4LDB_SIZE_TIMESTAMP, 0);
	m_pVcClParam->push_back( pParamDtLastUpdate );

	ClParam * pParamPK = new ClParam();
	pParamPK->SetBindParam(M4LDB_ODINAL_PK, NULL, M4LDB_M4_UNICODE_TYPE(M4CL_M4_TYPE_FIXED_STRING), M4_MAX_LEN_MD5 * 2, 0);
	m_pVcClParam->push_back( pParamPK );
}

ClPDContainer *ClDinArr::GetRefData (m4int32_t ai_lIndData)
{
	return (m_poPDContainer[ai_lIndData]);
}

ClDinArr::ClDinArr (ClDinArr *ai_pParam)
{
	m_pGlobVars = ai_pParam->m_pGlobVars;
	m_lNumData = ai_pParam->m_lNumData;
	m_lMaxNumData = ai_pParam->m_lMaxNumData;
	m_poPDContainer = new PClPDContainer[m_lMaxNumData];

	for (m4int32_t indData=0; indData<m_lNumData; indData++)
	{
		m_poPDContainer[indData] = new ClPDContainer (ai_pParam->GetRefData (indData));
	}

	if (ai_pParam->m_pmpIdData)
	{
		m_pmpIdData = new MpIdData (*ai_pParam->m_pmpIdData);
	}

	m_lReferences = M4LDB_NULL_VALUE;
    m_bHasLongItem = M4_FALSE;
	m_RegPipe = NULL;

	_CreateVcClParam();
}

ClDinArr::~ClDinArr (void)
{ 
	Close ();
}
	
ClRegister_Pipe * ClDinArr::GetPipe(void)
{
	if (!m_RegPipe)
	{
		m_RegPipe = new ClRegister_Pipe;
	}
	return (m_RegPipe);
}

m4bool_t ClDinArr::HasLongItem(void)
{
    return (m_bHasLongItem);
}

m4return_t ClDinArr::Init (m4int32_t ai_lMaxNumData, m4bool_t ai_bIsOnlySel)
{
	if (!m_poPDContainer)
	{
		m_lMaxNumData = ai_lMaxNumData;
		m_poPDContainer = new PClPDContainer[m_lMaxNumData];
		memset (m_poPDContainer, '\0', m_lMaxNumData * sizeof (PClPDContainer));

		if (!ai_bIsOnlySel)
		{
			m_pmpIdData = new MpIdData;
		}
	}

	return (M4_SUCCESS);
}


void ClDinArr::Attach (void)
{
	m_lReferences++;
}


void ClDinArr::Detach (void)
{
	m_lReferences--;
}


m4int32_t ClDinArr::GetReferences (void)
{
	return (m_lReferences);
}


m4int32_t ClDinArr::GetNumData (void)
{
	return (m_lNumData);
}


m4pcchar_t ClDinArr::GetData (m4int32_t ai_lIndData)
{
	return ( *(m_poPDContainer[ai_lIndData]->m_pPDValue->m_ppDataPointer));
}


m4int32_t ClDinArr::GetLength (m4int32_t ai_lIndData)
{
	return (m_poPDContainer[ai_lIndData]->m_pPDValue->m_lLength);
}


m4pchar_t ClDinArr::GetDataPointer (m4int32_t ai_lIndData)
{
	if (m_poPDContainer[ai_lIndData]->m_pPDValue->m_pPDEncryptedValue)
	{
		return (m_poPDContainer[ai_lIndData]->m_pPDValue->m_pPDEncryptedValue->GetDataPointer ());
	}
	return ( *(m_poPDContainer[ai_lIndData]->m_pPDValue->m_ppDataPointer) );
}


// UNICODE DB
m4pchar_t* ClDinArr::GetDataPointerPointer (m4int32_t ai_lIndData)
{
	// Se devuelve la dirección del buffer para almacenar los resultados
	if (m_poPDContainer[ai_lIndData]->m_pPDValue->m_pPDEncryptedValue)
	{
		return (m_poPDContainer[ai_lIndData]->m_pPDValue->m_pPDEncryptedValue->GetDataPointerPointer ());
	}
	return (m_poPDContainer[ai_lIndData]->m_pPDValue->m_ppDataPointer);
}


m4pint32_t ClDinArr::GetMaxLenPointer (m4int32_t ai_lIndData)
{
	// UNICODE DB
	/* Bug 0130721
	Hay que tener 2 punteros de máxima longitud del buffer
	Uno para los datos normales y otro para los encriptados
	*/
	// Se devuelve un buffer para almacenar la longitud de los resultados
	if( m_poPDContainer[ai_lIndData]->m_pPDValue->m_pPDEncryptedValue )
	{
		return( m_poPDContainer[ai_lIndData]->m_pPDValue->m_pPDEncryptedValue->GetMaxLenPointer() ) ;
	}

	return( GetMaxLenDataPointer(ai_lIndData) ) ;
}


m4pint32_t ClDinArr::GetMaxLenDataPointer (m4int32_t ai_lIndData)
{
	return( m_poPDContainer[ai_lIndData]->m_pPDValue->GetMaxLenPointer( m_poPDContainer[ai_lIndData]->m_pPDMTData->m_lPrec ) ) ;
}


m4pint32_t ClDinArr::GetLengthPointer (m4int32_t ai_lIndData)
{
	if (m_poPDContainer[ai_lIndData]->m_pPDValue->m_pPDEncryptedValue)
	{
		return (m_poPDContainer[ai_lIndData]->m_pPDValue->m_pPDEncryptedValue->GetpLength ());
	}
	return (&m_poPDContainer[ai_lIndData]->m_pPDValue->m_lLength);
}


m4pcchar_t ClDinArr::GetOldData (m4int32_t ai_lIndData)
{
	if (m_poPDContainer[ai_lIndData]->m_pPDOldValue)
	{
		return ( *(m_poPDContainer[ai_lIndData]->m_pPDOldValue->m_ppDataPointer) );
	}
	return ( *(m_poPDContainer[ai_lIndData]->m_pPDValue->m_ppDataPointer) );
}


m4int32_t ClDinArr::GetOldLength (m4int32_t ai_lIndData)
{
	if (m_poPDContainer[ai_lIndData]->m_pPDOldValue)
	{
		return (m_poPDContainer[ai_lIndData]->m_pPDOldValue->m_lLength);
	}
	return (m_poPDContainer[ai_lIndData]->m_pPDValue->m_lLength);
}


m4pchar_t ClDinArr::GetOldDataPointer (m4int32_t ai_lIndData)
{
	if (m_poPDContainer[ai_lIndData]->m_pPDOldValue)
	{
		return ( *(m_poPDContainer[ai_lIndData]->m_pPDOldValue->m_ppDataPointer) );
	}
	return (NULL);
}


m4pchar_t* ClDinArr::GetOldDataPointerPointer (m4int32_t ai_lIndData)
{
	if (m_poPDContainer[ai_lIndData]->m_pPDOldValue)
	{
		return ( m_poPDContainer[ai_lIndData]->m_pPDOldValue->m_ppDataPointer );
	}
	return (NULL);
}


m4pint32_t ClDinArr::GetOldLengthPointer (m4int32_t ai_lIndData)
{
	if (m_poPDContainer[ai_lIndData]->m_pPDOldValue)
	{
		return (&m_poPDContainer[ai_lIndData]->m_pPDOldValue->m_lLength);
	}
	return (NULL);
}


m4int32_t ClDinArr::GetItemHandle (m4int32_t ai_lIndData)
{
	return (m_poPDContainer[ai_lIndData]->m_pPDMTData->m_hItem);
}


void ClDinArr::SetItemHandle (m4int32_t ai_lIndData, m4int32_t ai_hItem)
{
	if (ai_hItem == M4LDB_INTERNAL_HANDLE || ai_hItem)
	{
		m_poPDContainer[ai_lIndData]->m_pPDMTData->m_eParamType = M4LDB_PAR_INTERN;
	}
	else
	{
		m_poPDContainer[ai_lIndData]->m_pPDMTData->m_eParamType = M4LDB_PAR_ITEM;
	}

	m_poPDContainer[ai_lIndData]->m_pPDMTData->m_hItem = ai_hItem;
}


void ClDinArr::SetParamType (m4int32_t ai_lIndData, eParamType_t ai_eParamType)
{
	m_poPDContainer[ai_lIndData]->m_pPDMTData->m_eParamType = ai_eParamType;
}


m4id_t ClDinArr::GetInfoIdField (m4int32_t ai_lIndData)
{
	if (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended)
	{
		return (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended->m_idField);
	}
	return (NULL);
}


m4pcchar_t ClDinArr::GetInfoRealTable (m4int32_t ai_lIndData)
{
	if (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended)
	{
		return (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended->m_pRealTable);
	}
	return (NULL);
}


m4pcchar_t ClDinArr::GetInfoRealField (m4int32_t ai_lIndData)
{
	if (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended)
	{
		return (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended->m_pRealField);
	}
	return (NULL);
}


m4int8_t ClDinArr::GetInfoM4Type (m4int32_t ai_lIndData)
{
	if( m_poPDContainer[ai_lIndData]->m_pPDValue->m_pPDEncryptedValue != NULL )
	{
		if( m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_LONG || 
			m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_UNI_LONG || 
			m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_BLOB || 
			m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_BINARY_STRING )
		{
			return (m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type);
		}
		else
		{
			/* Bug 0177318
			Si es encriptado no se convierte a unicode
			return M4LDB_M4_UNICODE_TYPE( M4CL_M4_TYPE_FIXED_STRING );
			*/
			return M4CL_M4_TYPE_FIXED_STRING;
		}
	}

	return (m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type);
}


void ClDinArr::SetInfoM4Type (m4int32_t ai_lIndData, m4int8_t ai_m4Type)
{
	m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type = ai_m4Type;
}


m4int8_t ClDinArr::GetInfoRealM4Type (m4int32_t ai_lIndData)
{
	return (m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type);
}


m4int32_t ClDinArr::GetInfoPrec (m4int32_t ai_lIndData)
{
	if (m_poPDContainer[ai_lIndData]->m_pPDValue->m_pPDEncryptedValue)
	{
		return (m_poPDContainer[ai_lIndData]->m_pPDValue->m_pPDEncryptedValue->GetMaxLen ());
	}
	return m_poPDContainer[ai_lIndData]->m_pPDMTData->m_lPrec;
}


m4int32_t ClDinArr::GetInfoPrecFld (m4int32_t ai_lIndData)
{
	return m_poPDContainer[ai_lIndData]->m_pPDMTData->m_lPrecFld;
}

m4int32_t ClDinArr::GetInfoOrgLevel (m4int32_t ai_lIndData)
{
	return m_poPDContainer[ai_lIndData]->m_pPDMTData->m_iOrgLevel;
}

void ClDinArr::SetInfoOrgLevel (m4int32_t ai_lIndData, m4int32_t ai_iOrgLevel)
{
	m_poPDContainer[ai_lIndData]->m_pPDMTData->m_iOrgLevel = ai_iOrgLevel;
}

m4bool_t ClDinArr::GetInfoInheritsData (m4int32_t ai_lIndData)
{
	return m_poPDContainer[ai_lIndData]->m_pPDMTData->m_bInheritsData;
}

void ClDinArr::SetInfoInheritsData (m4int32_t ai_lIndData, m4bool_t ai_bInheritsData)
{
	m_poPDContainer[ai_lIndData]->m_pPDMTData->m_bInheritsData = ai_bInheritsData;
}

m4int32_t ClDinArr::GetInfoScale (m4int32_t ai_lIndData)
{
	return (m_poPDContainer[ai_lIndData]->m_pPDMTData->m_lScale);
}


void ClDinArr::SetEmulateNVL (m4int32_t ai_lIndData,eEmulateNVL_t ai_eEmulateNVL)
{
	m_poPDContainer[ai_lIndData]->m_pPDMTData->m_eEmulateNVL = ai_eEmulateNVL;
}


eEmulateNVL_t  ClDinArr::GetEmulateNVL (m4int32_t ai_lIndData)
{
	return (m_poPDContainer[ai_lIndData]->m_pPDMTData->m_eEmulateNVL);
}


m4bool_t ClDinArr::GetInfoIsPK (m4int32_t ai_lIndData)
{
	if (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended)
	{
		return (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended->m_bIsPK);
	}
	return (M4_FALSE);
}


void ClDinArr::SetInfoIsPK (m4int32_t ai_lIndData, m4bool_t ai_bIsPK)
{
	if (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended)
	{
		m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended->m_bIsPK = ai_bIsPK;
	}
}


m4bool_t ClDinArr::GetInfoIsNotNull (m4int32_t ai_lIndData)
{
	if (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended)
	{
		return (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended->m_bIsNotNull);
	}
	return (M4_FALSE);
}


eFuncType_t ClDinArr::GetInfoIdDefaultFunc (m4int32_t ai_lIndData)
{
	if (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended)
	{
		return (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended->m_idDefFunc);
	}
	return (M4LDB_FTN_NONE);
}


eFuncType_t ClDinArr::GetInfoIdConstraintFunc (m4int32_t ai_lIndData)
{
	if (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended)
	{
		return (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended->m_idConstFunc);
	}
	return (M4LDB_FTN_NONE);
}


eFldType_t ClDinArr::GetInfoIdInternalField (m4int32_t ai_lIndData)
{
	if (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended)
	{
		return (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended->m_idIntFld);
	}
	return (M4LDB_NORMAL);
}


eParamType_t ClDinArr::GetInfoParamType (m4int32_t ai_lIndData)
{
	return (m_poPDContainer[ai_lIndData]->m_pPDMTData->m_eParamType);
}


m4pcchar_t ClDinArr::GetInfoDefaultArgs (m4int32_t ai_lIndData)
{
	if (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended)
	{
		return (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended->m_pDefArgs);
	}
	return (NULL);
}


m4pcchar_t ClDinArr::GetInfoConstraintArgs (m4int32_t ai_lIndData)
{
	if (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended)
	{
		return (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended->m_pConstArgs);
	}
	return (NULL);
}


m4bool_t ClDinArr::GetInfoIsCtrlRep (m4int32_t ai_lIndData)
{
	if (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended)
	{
		return (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended->m_bIsCtrlRep);
	}
	return (M4_FALSE);
}


m4int32_t ClDinArr::FindIndexByIdField (m4id_t ai_idField)
{
	struct stIdField stAux;

	stAux.m_pIdField = ai_idField;
	itMpIdData it = m_pmpIdData->find (stAux);

	if (it == m_pmpIdData->end ())
	{
		return (M4LDB_NULL_INDEX);
	}

	return ((*it).second);
}


m4bool_t ClDinArr::IsNull (m4int32_t ai_lIndData)
{
	return ((m_poPDContainer[ai_lIndData]->m_pPDValue->m_lLength == M4LDB_SQL_NULL_DATA) ? M4_TRUE : M4_FALSE);
}


m4bool_t ClDinArr::IsOldNull (m4int32_t ai_lIndData)
{
	if (m_poPDContainer[ai_lIndData]->m_pPDOldValue)
	{
		return ( (m_poPDContainer[ai_lIndData]->m_pPDOldValue->m_lLength == M4LDB_SQL_NULL_DATA) ? M4_TRUE : M4_FALSE );
	}
	return ( (m_poPDContainer[ai_lIndData]->m_pPDValue->m_lLength == M4LDB_SQL_NULL_DATA) ? M4_TRUE : M4_FALSE );
;
}


m4bool_t ClDinArr::GetInfoIsReadOnly (m4int32_t ai_lIndData)
{
	return (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended->m_bIsReadOnly);
}


m4return_t ClDinArr::SetInfoIsReadOnly (m4int32_t ai_lIndData, m4bool_t ai_bIsReadOnly)
{
	m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended->m_bIsReadOnly = ai_bIsReadOnly;
	return (M4_SUCCESS);
}


	m4bool_t	ClDinArr::GetIsEncrypted (m4int32_t ai_lIndData)
{
	return ((m_poPDContainer[ai_lIndData]->m_pPDValue->m_pPDEncryptedValue) ? M4_TRUE : M4_FALSE);
}


eConvFunc_t	ClDinArr::GetInfoIdConvertFunc (m4int32_t ai_lIndData)
{
	return (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended->m_idConvFunc);
}

	void		ClDinArr::SetCurrentLanguageIndex (m4int32_t ai_lIndData, m4int32_t ai_lIndCurrLang)
{
	m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended->m_iCurrentLang = ai_lIndCurrLang;
}


	m4int32_t	ClDinArr::GetCurrentLanguageIndex	(m4int32_t ai_lIndData )
{
	return (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended->m_iCurrentLang);
}


void ClDinArr::SetInfoIdDefaultFunc (m4int32_t ai_lIndData, eFuncType_t ai_eFuncType)
{
	m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended->m_idDefFunc = ai_eFuncType;
}


m4bool_t ClDinArr::GetInfoHasSecurity (m4int32_t ai_lIndData)
{
	return (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended->m_bHasSecurity);
}

void ClDinArr::SetInfoHasSecurity (m4int32_t ai_lIndData)
{
	m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended->m_bHasSecurity = M4_TRUE;
}
	
m4id_t ClDinArr::GetInfoTranslation (m4int32_t ai_lIndData)
{
	return (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended->m_idTranslation);
}

void ClDinArr::SetInfoTranslation (m4int32_t ai_lIndData, m4id_t ai_idTranslation)
{
	switch( m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type )
	{
		case M4CL_M4_TYPE_FIXED_STRING:
		case M4CL_M4_TYPE_VAR_STRING:
		case M4CL_M4_TYPE_LONG:
		case M4CL_M4_TYPE_VARIANT:
		case M4CL_M4_TYPE_UNI_VAR_STRING:
		case M4CL_M4_TYPE_UNI_LONG:
		case M4CL_M4_TYPE_UNI_VARIANT:
		case M4CL_M4_TYPE_UNI_FIXED_STRING:

			m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended->m_idTranslation = ai_idTranslation;
			break;
	}
}
	
m4bool_t ClDinArr::GetInfoHasSecurityReadInPersist (m4int32_t ai_lIndData)
{
	return (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended->m_bHasSecurityReadInPersist);
}

void ClDinArr::SetInfoHasSecurityReadInPersist (m4int32_t ai_lIndData)
{
	m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended->m_bHasSecurityReadInPersist = M4_TRUE;
}

m4bool_t ClDinArr::GetInfoIsOwn (m4int32_t ai_lIndData)
{
	return (m_poPDContainer[ai_lIndData]->m_bIsOwn);
}


m4bool_t ClDinArr::GetHasOldValue (m4int32_t ai_lIndData)
{
	return ((m_poPDContainer[ai_lIndData]->m_pPDOldValue) ? M4_TRUE : M4_FALSE);
}


void ClDinArr::SetInternalLong (m4int32_t ai_lIndData, ClTblMem *ai_pTblMem)
{
	m_poPDContainer[ai_lIndData]->m_pPDValue->m_pTblMem = ai_pTblMem;
}


m4bool_t ClDinArr::IsEmptyString (m4int32_t ai_lIndData)
{
	if (m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_FIXED_STRING ||
		m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_UNI_VAR_STRING)
	{
		m4pchar_t pValue = *(m_poPDContainer[ai_lIndData]->m_pPDValue->m_ppDataPointer);

		if (pValue && *pValue == '\0')
		{
			return (M4_TRUE);
		}
	}
	return (M4_FALSE);
}


