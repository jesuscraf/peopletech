//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                cldinarr.cpp
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

#include <sys/types.h>
#include <sys/stat.h>

#include "m4stl.hpp"
#include "m4mdrt.hpp"
#include "register.hpp"
#include "regpipe.hpp"

#include "cltblmem.hpp"
#include "cllconn.hpp"
#include "cldinarr.hpp"
#include "clglbvar.hpp"
#include "file_manager.hpp"
#include "clldb.hpp"
#include "metainfo.hpp"
#include "ldb_log.hpp"

#include "m4win2ux.hpp"
#include "m4sysalloc.hpp"
#include "m4objglb.hpp"
#include "ldbglobals.hpp"
#include "m4unicode.hpp"

#include "cldinarr.inl"



void	SwapDouble( m4pchar_t ai_pcBuffer, m4uint32_t ai_iOffset )
{

	m4char_t	c ;
	m4pchar_t	pcNumber ;


	pcNumber = ai_pcBuffer + ai_iOffset ;
	c = *pcNumber ;
	*pcNumber = *( pcNumber + 7 ) ;
	*( pcNumber + 7 ) = c ;

	pcNumber++ ;
	c = *pcNumber ;
	*pcNumber = *( pcNumber + 5 ) ;
	*( pcNumber + 5 ) = c ;

	pcNumber++ ;
	c = *pcNumber ;
	*pcNumber = *( pcNumber + 3 ) ;
	*( pcNumber + 3 ) = c ;

	pcNumber++ ;
	c = *pcNumber ;
	*pcNumber = *( pcNumber + 1 ) ;
	*( pcNumber + 1 ) = c ;

}

ClParam::ClParam( void ) 
{
	m_eFldType = M4LDB_NORMAL;	
	m_pData = NULL;
	m_lPrec = 0;
	m_m4Type = M4CL_M4_TYPE_NULL;
	m_lScale = 0;
	m_plLength = new m4int32_t;
	m_lPrecFld = 0;
}

ClParam::~ClParam() 
{
	delete m_plLength;
	m_plLength = NULL;
	if (m_pData != NULL)
	{
		delete [] m_pData;
		m_pData = NULL;
	}	
}

m4return_t ClParam::SetBindParam( eFldType_t ai_eFldType, m4pvoid_t ai_pValue, m4int32_t ai_m4Type, m4int32_t ai_lPrec, m4int32_t ai_lScale ) 
{

	if (m_pData == NULL)
	{
		if ( ai_m4Type == M4CL_M4_TYPE_NUMBER)
		{
			m_pData = (m4pchar_t) new m4double_t;
		}
		else
		{
			m_pData = new m4char_t[ M4LDB_ID_MAX_SIZE + 1];
			memset(m_pData, '\0', M4LDB_ID_MAX_SIZE + 1);
		}
	}

	if (ai_pValue != NULL) 
	{
		m4uint32_t iSize = 0;

		if ( ai_m4Type == M4CL_M4_TYPE_NUMBER )
		{
			iSize = sizeof( m4double_t ) ;
			*m_plLength = iSize;
		}
		else if ( ai_m4Type == M4CL_M4_TYPE_TIMESTAMP )
		{
			iSize = sizeof( m4date_t ) ;
			*m_plLength = iSize;
		}
		else
		{
			iSize = strlen( (m4pchar_t) ai_pValue ) + 1 ;

			if (M4LDB_ID_MAX_SIZE + 1 < iSize)
			{
				return M4_ERROR;
			}
			/* Bug 0144564
			Es más correcto inicializar los parámetros a cadenas null terminadas
			*/
			*m_plLength = M4LDB_SQL_NTS;
		}

		memcpy( m_pData, ai_pValue, iSize ) ;
	}
	else
	{
		//Es null pero queremos dejarlo preparado para cuando se sepa el valor.
		*m_plLength = M4LDB_SQL_NULL_DATA;		
	}
	
	m_eFldType = ai_eFldType;	

	m_m4Type = ai_m4Type;
	m_lPrec = ai_lPrec;
	m_lScale = ai_lScale;

	return M4_SUCCESS;
}	

m4return_t ClDinArr::Close (void)
{
	m4int32_t indData;

	if (m_pmpIdData)
	{
		delete m_pmpIdData;
	}
	m_pmpIdData = NULL;

	if (m_pVcClParam != NULL)
	{
		for (itVcClParam it = m_pVcClParam->begin(); it != m_pVcClParam->end(); it++)
		{
			delete (*it);
		}
		delete m_pVcClParam;		
	}
	m_pVcClParam = NULL;

	if (m_poPDContainer)
	{
		for (indData=0; indData<m_lNumData; indData++)
		{
			if (m_poPDContainer[indData])
			{
				delete m_poPDContainer[indData];
			}
		}
		delete [] m_poPDContainer;
	}
	m_poPDContainer = NULL;

	if (m_RegPipe)
	{
		m_RegPipe->DestroyPipe();
		delete m_RegPipe;
	}
	m_RegPipe = NULL;

	return (M4_SUCCESS);
}


m4return_t ClDinArr::CreateData (m4int32_t ai_lIndData, ClRegister *ai_pReg, m4int32_t ai_hItem, m4int8_t ai_m4Type, m4int32_t ai_lPrec, m4int32_t ai_lScale, m4pchar_t ai_pMemory, m4pchar_t* ai_ppMemoryPointer, eParamType_t ai_eParamType, m4bool_t ai_bWithPipe)
{

	m4return_t result;


	if (ai_lIndData >= m_lMaxNumData)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClDinArr::CreateData" << "Array bounds exceeded");
		return (M4_ERROR);
	}

	if (ai_lIndData >= m_lNumData)
	{
		m_lNumData = ai_lIndData + 1;
	}

	if (!m_poPDContainer[ai_lIndData])
	{
		ClRegister *pReg = NULL;
		m4bool_t bIsEncrypted = M4_FALSE;

		ClPDValue *pPDValue = new ClPDValue;
		ClPDMTData *pPDMTData = new ClPDMTData;
		ClPDMTDataExtended *pPDMTDataExtended = NULL;

		if (m_pmpIdData)
		{
			pPDMTDataExtended = new ClPDMTDataExtended;
		}

		m_poPDContainer[ai_lIndData] = new ClPDContainer (pPDValue, pPDMTData, pPDMTDataExtended);

		pPDMTData->m_hItem = ai_hItem;
		pPDMTData->m_eParamType = ai_eParamType;

		if (ai_pReg)
		{
			m4id_t idType = M4LDB_ID (ai_pReg->Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_TYPE].Value.GetWithOutSec ());
			
			m4int32_t indType = m_pGlobVars->GetpLConn ()->FindIndexById (M4LDB_SYS_DICT_EXTENDED_TYPES, idType);

			if (indType == M4LDB_NULL_INDEX)
			{
				m4id_t idField = M4LDB_ID (ai_pReg->Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_FIELD].Value.GetWithOutSec ());
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_EXTENDED_TYPE_NOT_FOUND, idType << idField);
				return (M4_ERROR);
			}
		
			pReg = &m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_EXTENDED_TYPES]->Register[indType];

			pPDMTData->m_m4Type = M4LDB_M4_TYPE ((m4int8_t) M4LDB_NUMBER (pReg->Item[M4LDB_ITEM_SYS_DICT_EXTENDED_TYPES_ID_M4_TYPE].Value.GetWithOutSec ()));
			pPDMTData->m_bIsUnicode = M4LDB_M4_IS_UNICODE ((m4int8_t) M4LDB_NUMBER (pReg->Item[M4LDB_ITEM_SYS_DICT_EXTENDED_TYPES_ID_M4_TYPE].Value.GetWithOutSec ()));

			if (ai_hItem)
			{
				m4int8_t m4TypeItm = M4LDB_M4_TYPE (ai_m4Type);

				if (m4TypeItm != pPDMTData->m_m4Type &&
					M4LDB_M4_UNICODE_TYPE(m4TypeItm) != M4LDB_M4_UNICODE_TYPE(pPDMTData->m_m4Type) &&
					ai_eParamType != M4LDB_PAR_INTERN)
				{
					// Si el tipo es distinto (sin tener en cuenta unicode) se da warning y se usa el del item
					m4id_t idField = M4LDB_ID (ai_pReg->Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_FIELD].Value.GetWithOutSec ());
					m4id_t nItem = M4LDB_EMPTY;

					if (ai_eParamType == M4LDB_PAR_ITEM && ai_hItem > 0)
					{
						nItem = M4LDB_ID (m_pGlobVars->GetpMCR ()->nItemI (ai_hItem));
			
						if (!*nItem)
						{
							nItem = M4LDB_ID (m_pGlobVars->GetpMCR ()->iItemI (ai_hItem));
						}
					}
					if (!*nItem)
					{
						DUMP_LDBINSP_WARNINGF (m_pGlobVars->GetpLConn (), M4_CH_LDB_M4TYPE_MISMATCH, idField << ai_lIndData);
					}
					else
					{
						DUMP_LDBINSP_WARNINGF (m_pGlobVars->GetpLConn (), M4_CH_LDB_M4TYPE_MISMATCH, idField << nItem);
					}

					pPDMTData->m_m4Type = m4TypeItm;
				}
			}

			pPDMTData->m_lScale = 0;

			switch (pPDMTData->m_m4Type)
			{
				case M4CL_M4_TYPE_LONG:
				case M4CL_M4_TYPE_UNI_LONG:
				case M4CL_M4_TYPE_BLOB:
				case M4CL_M4_TYPE_BINARY_STRING:
					pPDMTData->m_lPrec = M4LDB_SIZE_LONG;
					break;

				case M4CL_M4_TYPE_DATE:
				case M4CL_M4_TYPE_TIMESTAMP:
					pPDMTData->m_lPrec = M4LDB_SIZE_TIMESTAMP;
					break;

				case M4CL_M4_TYPE_NUMBER:
					pPDMTData->m_lPrec = M4LDB_NUMBER (pReg->Item[M4LDB_ITEM_SYS_DICT_EXTENDED_TYPES_PREC].Value.GetWithOutSec ());
					
					if (ai_hItem)
					{
						if ((((ai_lPrec < pPDMTData->m_lPrec) && (!m_pmpIdData)) || 
							((ai_lPrec > pPDMTData->m_lPrec) && (m_pmpIdData))) && 
							(ai_eParamType != M4LDB_PAR_INTERN))
						{
							m4id_t idField = M4LDB_ID (ai_pReg->Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_FIELD].Value.GetWithOutSec ());
							m4id_t nItem = M4LDB_EMPTY;

							if ((ai_eParamType == M4LDB_PAR_ITEM) && (ai_hItem > 0))
							{

								nItem = M4LDB_ID (m_pGlobVars->GetpMCR ()->nItemI (ai_hItem));
					
								if (!*nItem)
								{
									nItem = M4LDB_ID (m_pGlobVars->GetpMCR ()->iItemI (ai_hItem));
								}
							}
							if (!*nItem)
							{
								DUMP_LDBINSP_WARNINGF (m_pGlobVars->GetpLConn (), M4_CH_LDB_PRECISION_MISMATCH, idField << ai_lIndData);
							}
							else
							{
								DUMP_LDBINSP_WARNINGF (m_pGlobVars->GetpLConn (), M4_CH_LDB_PRECISION_MISMATCH, idField << nItem);
							}
						}

						pPDMTData->m_lPrec = ai_lPrec;
						pPDMTData->m_lScale = ai_lScale;
					}
					else
					{
						pPDMTData->m_lScale = M4LDB_NUMBER (pReg->Item[M4LDB_ITEM_SYS_DICT_EXTENDED_TYPES_SCALE].Value.GetWithOutSec ());
					}

					break;

				default:
					pPDMTData->m_lPrec = M4LDB_NUMBER (pReg->Item[M4LDB_ITEM_SYS_DICT_EXTENDED_TYPES_PREC].Value.GetWithOutSec ());

					if (ai_hItem)
					{
						if ((((ai_lPrec < pPDMTData->m_lPrec) && (!m_pmpIdData)) || 
							((ai_lPrec > pPDMTData->m_lPrec) && (m_pmpIdData))) && 
							(ai_eParamType != M4LDB_PAR_INTERN))
						{
							m4id_t idField = M4LDB_ID (ai_pReg->Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_FIELD].Value.GetWithOutSec ());
							m4id_t nItem = M4LDB_EMPTY;

							if ((ai_eParamType == M4LDB_PAR_ITEM) && (ai_hItem > 0))
							{

								if (!m_pmpIdData)
								{
									// Utilizaremos el tamaño del campo para traer valores y truncaremos con al tamaño del item.
									pPDMTData->m_lPrecFld = pPDMTData->m_lPrec;
								}

								nItem = M4LDB_ID (m_pGlobVars->GetpMCR ()->nItemI (ai_hItem));
					
								if (!*nItem)
								{
									nItem = M4LDB_ID (m_pGlobVars->GetpMCR ()->iItemI (ai_hItem));
								}
							}
							if (!*nItem)
							{
								DUMP_LDBINSP_WARNINGF (m_pGlobVars->GetpLConn (), M4_CH_LDB_PRECISION_MISMATCH, idField << ai_lIndData);
							}
							else
							{
								DUMP_LDBINSP_WARNINGF (m_pGlobVars->GetpLConn (), M4_CH_LDB_PRECISION_MISMATCH, idField << nItem);
							}
						}

						pPDMTData->m_lPrec = ai_lPrec;
					}

					if (!pPDMTData->m_lPrec)
					{
						pPDMTData->m_lPrec = M4LDB_SIZE_DEFAULT_STRING;
					}

					break;
			}

			bIsEncrypted = M4LDB_BOOL (pReg->Item[M4LDB_ITEM_SYS_DICT_EXTENDED_TYPES_IS_ENCRYPTED].Value.GetWithOutSec ());
		}
		else
		{
			pPDMTData->m_m4Type = M4LDB_M4_TYPE (ai_m4Type);
			pPDMTData->m_bIsUnicode = M4LDB_M4_IS_UNICODE (ai_m4Type);
			pPDMTData->m_lPrec = ai_lPrec;
			pPDMTData->m_lScale = 0;

			switch (pPDMTData->m_m4Type)
			{
				case M4CL_M4_TYPE_LONG:
				case M4CL_M4_TYPE_UNI_LONG:
				case M4CL_M4_TYPE_BLOB:
				case M4CL_M4_TYPE_BINARY_STRING:
					pPDMTData->m_lPrec = M4LDB_SIZE_LONG;
					break;

				case M4CL_M4_TYPE_DATE:
				case M4CL_M4_TYPE_TIMESTAMP:
					pPDMTData->m_lPrec = M4LDB_SIZE_TIMESTAMP;
					break;

				case M4CL_M4_TYPE_NUMBER:
					pPDMTData->m_lScale = ai_lScale;
					break;

				default:
					if (!pPDMTData->m_lPrec)
					{
						pPDMTData->m_lPrec = M4LDB_SIZE_DEFAULT_STRING;
					}
					break;
			}
		}

		if ((ai_bWithPipe) && (!m_RegPipe))
		{
			m_RegPipe = new ClRegister_Pipe;
			m_RegPipe->InitPipe (m_lMaxNumData);
		}
		
		if( ai_pMemory != NULL )
		{
			// UNICODE DB
			pPDValue->m_pData = ai_pMemory;
			pPDValue->m_ppDataPointer = &pPDValue->m_pData;
			pPDValue->m_lLength = M4LDB_SQL_NTS;
			pPDMTData->m_bIsExtern = M4_TRUE;
		}
		else if( ai_ppMemoryPointer != NULL )
		{
			// UNICODE DB
			pPDValue->m_pData = NULL;
			pPDValue->m_ppDataPointer = ai_ppMemoryPointer;
			pPDValue->m_lLength = M4LDB_SQL_NTS;
			pPDMTData->m_bIsExtern = M4_TRUE;
		}
		else
		{
			switch (pPDMTData->m_m4Type)
			{
				case M4CL_M4_TYPE_LONG:
				case M4CL_M4_TYPE_UNI_LONG:
				case M4CL_M4_TYPE_BINARY_STRING:
					m_bHasLongItem = M4_TRUE;
					pPDValue->m_pData = NULL;
					// UNICODE DB
					pPDValue->m_ppDataPointer = &pPDValue->m_pData;
					break;

				case M4CL_M4_TYPE_BLOB:
					m_bHasLongItem = M4_TRUE;
					pPDValue->m_pData = new m4char_t[M4LDB_SIZE_BLOB + 1];
					memset (pPDValue->m_pData, '\0', M4LDB_SIZE_BLOB + 1);
					// UNICODE DB
					pPDValue->m_ppDataPointer = &pPDValue->m_pData;
					break;

				case M4CL_M4_TYPE_DATE:
				case M4CL_M4_TYPE_TIMESTAMP:
					if ((ai_bWithPipe) && (ai_hItem > 0))
					{
						m_RegPipe->CreateItemInPipe(ai_lIndData, &pPDValue->m_lLength, M4LDB_SIZE_DOUBLE, ai_hItem, M4_TRUE);
						// UNICODE DB
						pPDValue->m_pData = NULL;
						pPDValue->m_ppDataPointer = m_RegPipe->GetItemAddressPointer(ai_lIndData);
					}
					else
					{
						pPDValue->m_pData = (m4pchar_t) new m4date_t;
						*((m4date_t*) pPDValue->m_pData) = 0.0;
						// UNICODE DB
						pPDValue->m_ppDataPointer = &pPDValue->m_pData;
					}
					break;

				case M4CL_M4_TYPE_NUMBER:
					if ((ai_bWithPipe) && (ai_hItem > 0))
					{
						m_RegPipe->CreateItemInPipe(ai_lIndData, &pPDValue->m_lLength, M4LDB_SIZE_DOUBLE, ai_hItem, M4_TRUE);
						// UNICODE DB
						pPDValue->m_pData = NULL;
						pPDValue->m_ppDataPointer = m_RegPipe->GetItemAddressPointer(ai_lIndData);
					}
					else
					{
						pPDValue->m_pData = (m4pchar_t) new m4double_t;
						*((m4double_t*) pPDValue->m_pData) = 0.0;
						// UNICODE DB
						pPDValue->m_ppDataPointer = &pPDValue->m_pData;
					}
					break;

				default:
					if ((ai_bWithPipe) && (ai_hItem > 0))
					{
						result = m_RegPipe->CreateItemInPipe(ai_lIndData, &pPDValue->m_lLength, pPDMTData->m_lPrec + 1, ai_hItem);
						//Bug 0077794
						//Nos protegemos si no hemos podido reservar memoria.
						//Modificación de DM. El miembro devolvera error y volcará el error, nosotros sólo retornamos error.
						if (result == M4_ERROR)
						{						
							return M4_ERROR;
						}
						// UNICODE DB
						pPDValue->m_pData = NULL;
						pPDValue->m_ppDataPointer = m_RegPipe->GetItemAddressPointer(ai_lIndData);
					}
					else
					{
						pPDValue->m_pData = new m4char_t[pPDMTData->m_lPrec + 1];
						//Bug 0077794
						//Nos protegemos si no hemos podido reservar memoria.
						if (!pPDValue->m_pData)
						{
							DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClDinArr::CreateData" << "Allocating for precision data");
							return M4_ERROR;
						}
						memset (pPDValue->m_pData, '\0', pPDMTData->m_lPrec + 1);

						// UNICODE DB
						pPDValue->m_ppDataPointer = &pPDValue->m_pData;
					}
					break;
			}
		}

		if (bIsEncrypted)
		{
			pPDValue->m_pPDEncryptedValue = new ClPDEncryptedValue;

			// Si el campo esta encriptado, reservamos espacio (para que se pueda bindar). 
			if (pPDValue->ReserveForEncrypted(pPDMTData->m_m4Type, pPDMTData->m_lPrec, pPDMTData->m_bIsUnicode)!=M4_SUCCESS)
			{
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (),M4_CH_LDB_INTERNAL_ERROR,"Allocating for encrypting data\n");
				return M4_ERROR;
			}
		}


		if ((pPDMTDataExtended) && (pReg))
		{
			pPDMTDataExtended->m_bIsPK = ((M4LDB_NUMBER (ai_pReg->Item[M4LDB_ITEM_SYS_DICT_FIELDS_POS_PK].Value.GetWithOutSec ())) ? M4_TRUE : M4_FALSE);
			pPDMTDataExtended->m_bIsNotNull = ((M4LDB_NUMBER (ai_pReg->Item[M4LDB_ITEM_SYS_DICT_FIELDS_NOT_NULL].Value.GetWithOutSec ())) ? M4_TRUE : M4_FALSE);
			pPDMTDataExtended->m_bIsCtrlRep = ((M4LDB_NUMBER (ai_pReg->Item[M4LDB_ITEM_SYS_DICT_FIELDS_CTRL_REPETITION].Value.GetWithOutSec ())) ? M4_TRUE : M4_FALSE);
			pPDMTDataExtended->m_idDefFunc = (eFuncType_t) M4LDB_NUMBER (pReg->Item[M4LDB_ITEM_SYS_DICT_EXTENDED_TYPES_ID_DEFAULT_FUNC].Value.GetWithOutSec ());
			pPDMTDataExtended->m_idConstFunc = (eFuncType_t) M4LDB_NUMBER (pReg->Item[M4LDB_ITEM_SYS_DICT_EXTENDED_TYPES_ID_CONSTRAINT_FUNC].Value.GetWithOutSec ());
			pPDMTDataExtended->m_idConvFunc = (eConvFunc_t) M4LDB_NUMBER (pReg->Item[M4LDB_ITEM_SYS_DICT_EXTENDED_TYPES_ID_CONVERT_FUNC].Value.GetWithOutSec ());
			pPDMTDataExtended->m_pDefArgs = M4LDB_PCSTR (pReg->Item[M4LDB_ITEM_SYS_DICT_EXTENDED_TYPES_DEFAULT_ARGS].Value.GetWithOutSec ());
			pPDMTDataExtended->m_pConstArgs = M4LDB_PCSTR (pReg->Item[M4LDB_ITEM_SYS_DICT_EXTENDED_TYPES_CONSTRAINT_ARGS].Value.GetWithOutSec ());
			pPDMTDataExtended->m_idIntFld = (eFldType_t) M4LDB_NUMBER (ai_pReg->Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_INTERNAL_FIELD].Value.GetWithOutSec ());
			pPDMTDataExtended->m_pRealTable = M4LDB_PCSTR (ai_pReg->Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_REAL_OBJECT].Value.GetWithOutSec ());
			pPDMTDataExtended->m_pRealField = M4LDB_PCSTR (ai_pReg->Item[M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD].Value.GetWithOutSec ());
			pPDMTDataExtended->m_idField = M4LDB_ID (ai_pReg->Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_FIELD].Value.GetWithOutSec ());

			InsertIdData (pPDMTDataExtended->m_idField, ai_lIndData);

			if(	pPDMTDataExtended->m_bIsNotNull == M4_TRUE && pPDMTDataExtended->m_bIsPK == M4_FALSE )
			{
				if( pPDMTDataExtended->m_idIntFld == M4LDB_TRANS_FLD && ai_eParamType == M4LDB_PAR_ITEM && ai_hItem > 0 )
				{
					/* Bug 0251548
					Los campos traducibles que se usan directamente no pueden ser not null
					*/
					pPDMTDataExtended->m_bIsNotNull = M4_FALSE ;
				}
				else if( pPDMTDataExtended->m_idIntFld != M4LDB_TRANS_FLD && pPDMTDataExtended->m_idField != NULL && *( pPDMTDataExtended->m_idField ) != '\0' && pPDMTDataExtended->m_idField[ strlen( pPDMTDataExtended->m_idField ) - 1 ] == '#' )
				{
					/* Bug 0252605
					Cualquier campo # que se use directamente no pueden ser not null
					*/
					pPDMTDataExtended->m_bIsNotNull = M4_FALSE ;
				}
			}
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClDinArr::CreateOldData (m4int32_t ai_lIndData)
{
	if ((m_poPDContainer[ai_lIndData]) && 
		(!m_poPDContainer[ai_lIndData]->m_pPDOldValue))
	{
		ClPDValue *pPDOldValue = new ClPDValue;

		m_poPDContainer[ai_lIndData]->m_pPDOldValue = pPDOldValue;

		switch (m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type)
		{
			case M4CL_M4_TYPE_LONG:
			case M4CL_M4_TYPE_UNI_LONG:
			case M4CL_M4_TYPE_BLOB:
			case M4CL_M4_TYPE_BINARY_STRING:
				pPDOldValue->m_pData = NULL;
				// UNICODE DB
				pPDOldValue->m_ppDataPointer = &pPDOldValue->m_pData;
				break;

			case M4CL_M4_TYPE_DATE:
			case M4CL_M4_TYPE_TIMESTAMP:
				pPDOldValue->m_pData = (m4pchar_t) new m4date_t;
				*((m4date_t*) pPDOldValue->m_pData) = 0.0;
				// UNICODE DB
				pPDOldValue->m_ppDataPointer = &pPDOldValue->m_pData;
				break;

			case M4CL_M4_TYPE_NUMBER:
				pPDOldValue->m_pData = (m4pchar_t) new m4double_t;
				*((m4double_t*) pPDOldValue->m_pData) = 0.0;
				// UNICODE DB
				pPDOldValue->m_ppDataPointer = &pPDOldValue->m_pData;
				break;

			default:
				pPDOldValue->m_pData = new m4char_t[m_poPDContainer[ai_lIndData]->m_pPDMTData->m_lPrec + 1];
				memset (pPDOldValue->m_pData, '\0', m_poPDContainer[ai_lIndData]->m_pPDMTData->m_lPrec + 1);
				// UNICODE DB
				pPDOldValue->m_ppDataPointer = &pPDOldValue->m_pData;
				break;
		}
		// Si el Value estaba encriptado, creamos el dato encriptado para el Old Value.
		ClPDEncryptedValue * pEncrypted = m_poPDContainer[ai_lIndData]->m_pPDValue->m_pPDEncryptedValue;
		if (pEncrypted)
		{
			pPDOldValue->m_pPDEncryptedValue = new ClPDEncryptedValue(*pEncrypted);
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClDinArr::SetData (m4int32_t ai_lIndData, m4pcchar_t ai_pcData, m4int32_t ai_lLength, m4int8_t ai_m4Type)
{
	m4return_t ret=M4_SUCCESS;
	m4pcchar_t pNewValue = ai_pcData;
	m4int32_t lLength;
	m4double_t dValue = 0;
	m4char_t aValue[M4LDB_SIZE_DEFAULT_STRING];

	if (ai_m4Type && pNewValue)
	{
		switch (ai_m4Type)
		{
			case M4CL_M4_TYPE_DATE:
			case M4CL_M4_TYPE_TIMESTAMP:
				if (m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_FIXED_STRING ||
					m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_UNI_VAR_STRING)
				{
					ClDateToString (M4LDB_DOUBLE (pNewValue), aValue, M4LDB_SIZE_TIMESTAMP + 1);
					pNewValue = aValue;
				}
				break;

			case M4CL_M4_TYPE_NUMBER:
				if (m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_FIXED_STRING ||
					m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_UNI_VAR_STRING)
				{
					sprintf (aValue, "%.20G", M4LDB_DOUBLE (pNewValue));
					pNewValue = aValue;
				}
				break;

			default:
				if (m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_DATE || 
					m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_TIMESTAMP)
				{
					ClStringToDate (pNewValue, dValue);
					pNewValue = (m4pcchar_t) &dValue;
				}
				if (m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_NUMBER)
				{
					dValue = atof (pNewValue);
					pNewValue = (m4pcchar_t) &dValue;
				}
				break;
		}
	}

	if (pNewValue)
	{
		switch (m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type) 
		{
			case M4CL_M4_TYPE_LONG:
			case M4CL_M4_TYPE_UNI_LONG:
			case M4CL_M4_TYPE_BINARY_STRING:
				// UNICODE DB
				m_poPDContainer[ai_lIndData]->m_pPDValue->m_pData = (m4pchar_t) pNewValue;
				m_poPDContainer[ai_lIndData]->m_pPDValue->m_ppDataPointer = &( m_poPDContainer[ai_lIndData]->m_pPDValue->m_pData );

				if( m_poPDContainer[ai_lIndData]->m_pPDMTData->m_eParamType == M4LDB_PAR_EXTERN )
				{
					m_poPDContainer[ai_lIndData]->m_pPDValue->m_lLength = strlen( pNewValue );
				}
				else
				{
					m_poPDContainer[ai_lIndData]->m_pPDValue->m_lLength = ai_lLength;
				}
				break;

			case M4CL_M4_TYPE_BLOB:
				{
					if (m_poPDContainer[ai_lIndData]->m_pPDValue->m_pTblMem)
					{
						// Bug 299654 Antes de asignar hay que liberar la memoria del BLOB
						if( m_poPDContainer[ai_lIndData]->m_pPDValue->m_pData != NULL )
						{
							delete( m_poPDContainer[ai_lIndData]->m_pPDValue->m_pData ) ;
						}

						// UNICODE DB
						m_poPDContainer[ai_lIndData]->m_pPDValue->m_pData = (m4pchar_t) pNewValue;
						m_poPDContainer[ai_lIndData]->m_pPDValue->m_ppDataPointer = &( m_poPDContainer[ai_lIndData]->m_pPDValue->m_pData );
						m_poPDContainer[ai_lIndData]->m_pPDValue->m_lLength = ai_lLength;
					}
					else
					{
						/* Bug 0154487
						No se puede asignar algo de más tamaño que M4LDB_SIZE_BLOB, que es lo que
						se ha reservado.
						Si se pasa más como argumento puede ser porque sea una copia de otro blob
						ya la longitud es falsa, ya que tiene metida la longitud del fichero.
						*/
						if( ai_lLength > M4LDB_SIZE_BLOB )
						{
							ai_lLength = M4LDB_SIZE_BLOB ;
						}

						memcpy( *(m_poPDContainer[ai_lIndData]->m_pPDValue->m_ppDataPointer), pNewValue, ai_lLength);
						
						// UNICODE FILE
						int iLength = -1 ;
						char* pcFileName = M4CppToANSI( pNewValue, iLength ) ;

						struct stat stFile;
						if (stat (pcFileName, &stFile) == 0)
						{
							m_poPDContainer[ai_lIndData]->m_pPDValue->m_lLength = stFile.st_size;
							
							m4int32_t lFile = strlen (pNewValue);
							m4int32_t lPos;
							for (lPos = lFile; lPos; lPos--)
							{
								if (pNewValue[lPos] == '.')
								{
									break;
								}
							}
							
							if (lPos)
							{
								m_poPDContainer[ai_lIndData]->m_pPDValue->m_lLength += (lFile - lPos);
							}
							else
							{
								m_poPDContainer[ai_lIndData]->m_pPDValue->m_lLength++;
							}

							m_poPDContainer[ai_lIndData]->m_pPDValue->m_lLength += strlen(pNewValue+strlen(pNewValue)+1) + 1;
						}
						else
						{
							m_poPDContainer[ai_lIndData]->m_pPDValue->m_lLength = M4LDB_SQL_NULL_DATA;
						}

						delete [] pcFileName ;
					}
				}
				break;

			case M4CL_M4_TYPE_DATE:
				*((m4date_t*) *(m_poPDContainer[ai_lIndData]->m_pPDValue->m_ppDataPointer)) = M4LDB_NUMBER ((m4date_t *) pNewValue);
				m_poPDContainer[ai_lIndData]->m_pPDValue->m_lLength = M4LDB_SQL_NTS;
				break;

			case M4CL_M4_TYPE_TIMESTAMP:
				*((m4date_t*) *(m_poPDContainer[ai_lIndData]->m_pPDValue->m_ppDataPointer)) = *((m4date_t*) pNewValue);
				m_poPDContainer[ai_lIndData]->m_pPDValue->m_lLength = M4LDB_SQL_NTS;
				break;

			case M4CL_M4_TYPE_NUMBER:
				*((m4double_t*) *(m_poPDContainer[ai_lIndData]->m_pPDValue->m_ppDataPointer)) = *((m4double_t*) pNewValue);
				m_poPDContainer[ai_lIndData]->m_pPDValue->m_lLength = M4LDB_SQL_NTS;
				break;

			default:
				// Si es de longitud 0 metemos un nulo.
				// Por compatibilidad entre distintas BBDD
				// ( Oracle lo pasa a nulo;SQLServer, DB2 e Informix, lo dejan igual y Sybase pone un espacio ).
				if (*pNewValue)
				{
					if (ai_lLength > 0)
					{
						lLength = ai_lLength;
					}
					else
					{
						lLength = (m4int32_t) strlen (pNewValue);
					}

					// UNICODE DB
					/* Bug 0126229
					El truncado debe hacerse por caracteres y no por bytes
					No se debe truncar el valor por que no quepa
					*/
					size_t		iChars = 0 ;
					size_t		iSize = 0 ;
					int			iPosition = 0 ;
					m4int32_t	*plMaxLen = NULL ;

					iChars = M4StrLen( pNewValue, lLength ) ;

					if( iChars > m_poPDContainer[ai_lIndData]->m_pPDMTData->m_lPrec )
					{
						iChars = m_poPDContainer[ai_lIndData]->m_pPDMTData->m_lPrec ;
					}

					iPosition = M4CharPosition( pNewValue, lLength, iChars, iSize ) ;

					plMaxLen = GetMaxLenDataPointer( ai_lIndData ) ;

					if( iPosition > *plMaxLen )
					{
						delete [] *(m_poPDContainer[ai_lIndData]->m_pPDValue->m_ppDataPointer) ;
						*plMaxLen = iPosition ;
						*(m_poPDContainer[ai_lIndData]->m_pPDValue->m_ppDataPointer) = new m4char_t[*plMaxLen + 1];
					}

					memcpy (*(m_poPDContainer[ai_lIndData]->m_pPDValue->m_ppDataPointer), pNewValue, iPosition);
					(*(m_poPDContainer[ai_lIndData]->m_pPDValue->m_ppDataPointer))[iPosition] = '\0';
					m_poPDContainer[ai_lIndData]->m_pPDValue->m_lLength = M4LDB_SQL_NTS;

					if( iPosition < lLength && iPosition < strlen( pNewValue ) )
					{
						m4bool_t	bDumpError = M4_TRUE ;

						/*
						if( lLength == iPosition +  9 && memcmp( m4pvoid_t( pNewValue + iPosition), ".00000000",  9 ) == 0
						  ||lLength == iPosition + 10 && memcmp( m4pvoid_t( pNewValue + iPosition), ".00000000", 10 ) == 0 )
						{
							bDumpError = M4_FALSE ;
						}
						*/

						if( bDumpError == M4_TRUE )
						{
							DUMP_LDBINSP_ERRORF( m_pGlobVars->GetpLConn(), M4_CH_LDB_DATA_TRUNCATED, pNewValue << iPosition ) ;
						}
					}
				}
				else
				{
					m_poPDContainer[ai_lIndData]->m_pPDValue->m_lLength = M4LDB_SQL_NULL_DATA;
				}
				break;
		}

	}
	else
	{
		m_poPDContainer[ai_lIndData]->m_pPDValue->m_lLength = M4LDB_SQL_NULL_DATA;
	}

	if (m_poPDContainer[ai_lIndData]->m_pPDValue->m_pPDEncryptedValue)
	{
		// Si es un field encriptado tenemos que encriptar  pNewValue
		// Hay que distinguir el caso de los blobs, porque me hace falta el blobdefaultdir
		
		ret = m_poPDContainer[ai_lIndData]->m_pPDValue->EncryptValue(m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type, m_poPDContainer[ai_lIndData]->m_pPDMTData->m_bIsUnicode);
		
		if (ret != M4_SUCCESS)
		{ 
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (),M4_CH_LDB_ENCRYPTING_DATA, *(m_poPDContainer[ai_lIndData]->m_pPDValue->m_ppDataPointer));
		}
	}

	return (ret);
}

m4return_t ClDinArr::SetNull (m4int32_t ai_lIndData)
{
	m_poPDContainer[ai_lIndData]->m_pPDValue->m_lLength = -1;
	if (m_poPDContainer[ai_lIndData]->m_pPDValue->m_pPDEncryptedValue)
	{
		m_poPDContainer[ai_lIndData]->m_pPDValue->m_pPDEncryptedValue->Reset();
	}

	return M4_SUCCESS;
}

m4return_t ClDinArr::SetOldData (m4int32_t ai_lIndData, m4pcchar_t ai_pcData, m4int32_t ai_lLength, m4int8_t ai_m4Type)
{
	m4pcchar_t pOldValue = ai_pcData;
	m4int32_t lLength;
	m4double_t dValue = 0;
	m4char_t aValue[M4LDB_SIZE_DEFAULT_STRING];

	if (!m_poPDContainer[ai_lIndData]->m_pPDOldValue)
	{
		return (M4_SUCCESS);
	}

	if (ai_m4Type && pOldValue)
	{
		switch (ai_m4Type)
		{
			case M4CL_M4_TYPE_DATE:
			case M4CL_M4_TYPE_TIMESTAMP:
				if (m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_FIXED_STRING ||
					m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_UNI_VAR_STRING)
				{
					ClDateToString (M4LDB_DOUBLE (pOldValue), aValue, M4LDB_SIZE_TIMESTAMP + 1);
					pOldValue = aValue;
				}
				break;

			case M4CL_M4_TYPE_NUMBER:
				if (m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_FIXED_STRING ||
					m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_UNI_VAR_STRING)
				{
					sprintf (aValue, "%.20G", M4LDB_DOUBLE (pOldValue));
					pOldValue = aValue;
				}
				break;

			default:
				if (m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_DATE || 
					m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_TIMESTAMP)
				{
					ClStringToDate (pOldValue, dValue);
					pOldValue = (m4pcchar_t) &dValue;
				}
				if (m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_NUMBER)
				{
					dValue = atof (pOldValue);
					pOldValue = (m4pcchar_t) &dValue;
				}
				break;
		}
	}

	if (pOldValue)
	{
		switch (m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type)
		{
			case M4CL_M4_TYPE_LONG:
			case M4CL_M4_TYPE_UNI_LONG:
			case M4CL_M4_TYPE_BLOB:
			case M4CL_M4_TYPE_BINARY_STRING:
				// UNICODE DB
				m_poPDContainer[ai_lIndData]->m_pPDOldValue->m_pData = (m4pchar_t) pOldValue;
				m_poPDContainer[ai_lIndData]->m_pPDOldValue->m_ppDataPointer = &( m_poPDContainer[ai_lIndData]->m_pPDOldValue->m_pData );
				m_poPDContainer[ai_lIndData]->m_pPDOldValue->m_lLength = ai_lLength;
				return (M4_SUCCESS);
				break;

			case M4CL_M4_TYPE_DATE:
				*((m4date_t*) *(m_poPDContainer[ai_lIndData]->m_pPDOldValue->m_ppDataPointer)) = M4LDB_NUMBER ((m4date_t *) pOldValue);
				break;

			case M4CL_M4_TYPE_TIMESTAMP:
				*((m4date_t*) *(m_poPDContainer[ai_lIndData]->m_pPDOldValue->m_ppDataPointer)) = *((m4date_t*) pOldValue);
				break;

			case M4CL_M4_TYPE_NUMBER:
				*((m4double_t*) *(m_poPDContainer[ai_lIndData]->m_pPDOldValue->m_ppDataPointer)) = *((m4double_t*) pOldValue);
				break;

			default:
				if (ai_lLength > 0)
				{
					lLength = ai_lLength;
				}
				else
				{
					lLength = (m4int32_t) strlen (pOldValue);
				}

				lLength = __min (lLength, m_poPDContainer[ai_lIndData]->m_pPDMTData->m_lPrec);

				memcpy (*(m_poPDContainer[ai_lIndData]->m_pPDOldValue->m_ppDataPointer), pOldValue, lLength);
				(*(m_poPDContainer[ai_lIndData]->m_pPDOldValue->m_ppDataPointer))[lLength] = '\0';
				break;
		}

		m_poPDContainer[ai_lIndData]->m_pPDOldValue->m_lLength = M4LDB_SQL_NTS;
	}
	else
	{
		m_poPDContainer[ai_lIndData]->m_pPDOldValue->m_lLength = M4LDB_SQL_NULL_DATA;
	}

	return (M4_SUCCESS);
}


m4int32_t ClDinArr::FindIndexByIdInternalField (eFldType_t ai_idIntFld)
{
	m4int32_t indData;

	for (indData=0; indData<m_lNumData; indData++)
	{
		if (ai_idIntFld == (m_poPDContainer[indData]->m_pPDMTDataExtended->m_idIntFld))
		{
			return (indData);
		}
	}

	return (M4LDB_NULL_INDEX);
}


m4int32_t ClDinArr::FindIndexByItemHandle (m4int32_t ai_hItem)
{
	m4int32_t indData;

	for (indData=0; indData<m_lNumData; indData++)
	{
		if (ai_hItem == m_poPDContainer[indData]->m_pPDMTData->m_hItem)
		{
			return (indData);
		}
	}

	return (M4LDB_NULL_INDEX);
}


m4return_t ClDinArr::Clear (void)
{
	m4int32_t indData;

	for (indData=0; indData<m_lNumData; indData++)
	{
		if (!m_poPDContainer[indData]->m_pPDMTData->m_bIsExtern)
		{
			switch (m_poPDContainer[indData]->m_pPDMTData->m_m4Type)
			{
				case M4CL_M4_TYPE_LONG:
				case M4CL_M4_TYPE_UNI_LONG:
				case M4CL_M4_TYPE_BINARY_STRING:
					if (m_poPDContainer[indData]->m_pPDValue->m_pTblMem)
					{
						m_poPDContainer[indData]->m_pPDValue->Release ();
					}
					else
					{
						*(m_poPDContainer[indData]->m_pPDValue->m_ppDataPointer) = NULL;
					}
					if ((m_poPDContainer[indData]->m_pPDOldValue) && 
						(m_poPDContainer[indData]->m_pPDOldValue->m_pTblMem))
					{
						m_poPDContainer[indData]->m_pPDOldValue->Release ();
					}
					break;

				case M4CL_M4_TYPE_BLOB:
					if (m_poPDContainer[indData]->m_pPDValue->m_pTblMem)
					{
						m_poPDContainer[indData]->m_pPDValue->Release ();

						// Bug 299654 Se debe restablecer la memoria asociada al BLOB
						m_poPDContainer[indData]->m_pPDValue->m_pData = new m4char_t[ M4LDB_SIZE_BLOB + 1 ];
						memset( *(m_poPDContainer[indData]->m_pPDValue->m_ppDataPointer), '\0',  M4LDB_SIZE_BLOB + 1 );
					}
					else
					{
						/* Bug 0165266
						Si es un BLOB puede que no tenga el dato alocado si es de oldvalue
						*/
						if( *(m_poPDContainer[indData]->m_pPDValue->m_ppDataPointer) != NULL )
						{
							memset( *(m_poPDContainer[indData]->m_pPDValue->m_ppDataPointer), '\0',  M4LDB_SIZE_BLOB + 1 );
						}
					}
					if ((m_poPDContainer[indData]->m_pPDOldValue) && 
						(m_poPDContainer[indData]->m_pPDOldValue->m_pTblMem))
					{
						m_poPDContainer[indData]->m_pPDOldValue->Release ();
					}
					break;

				case M4CL_M4_TYPE_DATE:
				case M4CL_M4_TYPE_TIMESTAMP:
					*((m4date_t*) *(m_poPDContainer[indData]->m_pPDValue->m_ppDataPointer)) = 0.0;
					break;

				case M4CL_M4_TYPE_NUMBER:
					*((m4double_t*) *(m_poPDContainer[indData]->m_pPDValue->m_ppDataPointer)) = 0.0;
					break;

				default:
					memset ( *(m_poPDContainer[indData]->m_pPDValue->m_ppDataPointer), '\0', m_poPDContainer[indData]->m_pPDMTData->m_lPrec + 1);
					break;

			}

			m_poPDContainer[indData]->m_pPDValue->m_lLength = M4LDB_SQL_NULL_DATA;

			if (m_poPDContainer[indData]->m_pPDOldValue)
			{
				m_poPDContainer[indData]->m_pPDOldValue->m_lLength = M4LDB_SQL_NULL_DATA;
			}

			if (m_poPDContainer[indData]->m_pPDValue->m_pPDEncryptedValue)
			{
				m_poPDContainer[indData]->m_pPDValue->m_pPDEncryptedValue->Reset();
			}
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClDinArr::ClearLongs (void)
{
	m4int32_t indData;

	for (indData=0; indData<m_lNumData; indData++)
	{
		if (!m_poPDContainer[indData]->m_pPDMTData->m_bIsExtern && 
			(m_poPDContainer[indData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_LONG || 
			 m_poPDContainer[indData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_UNI_LONG || 
			 m_poPDContainer[indData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_BLOB || 
			 m_poPDContainer[indData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_BINARY_STRING))
		{
			if (m_poPDContainer[indData]->m_pPDValue->m_pTblMem)
			{
				m_poPDContainer[indData]->m_pPDValue->Release ();

				// Bug 299654 Se debe restablecer la memoria asociada al BLOB
				if (m_poPDContainer[indData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_BLOB)
				{
					m_poPDContainer[indData]->m_pPDValue->m_pData = new m4char_t[ M4LDB_SIZE_BLOB + 1 ];
					memset( *(m_poPDContainer[indData]->m_pPDValue->m_ppDataPointer), '\0',  M4LDB_SIZE_BLOB + 1 );
				}
			}
			else
			{
				if (m_poPDContainer[indData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_BLOB)
				{
					/* Bug 0165266
					Si es un BLOB puede que no tenga el dato alocado si es de oldvalue
					*/
					if( *(m_poPDContainer[indData]->m_pPDValue->m_ppDataPointer) != NULL )
					{
						memset( *(m_poPDContainer[indData]->m_pPDValue->m_ppDataPointer), '\0',  M4LDB_SIZE_BLOB + 1 );
					}
				}
				else
				{
					*(m_poPDContainer[indData]->m_pPDValue->m_ppDataPointer) = NULL;
				}
			}
			m_poPDContainer[indData]->m_pPDValue->m_lLength = M4LDB_SQL_NULL_DATA;

			if ((m_poPDContainer[indData]->m_pPDOldValue) && 
				(m_poPDContainer[indData]->m_pPDOldValue->m_pTblMem))
			{
				m_poPDContainer[indData]->m_pPDOldValue->Release ();
				m_poPDContainer[indData]->m_pPDOldValue->m_lLength = M4LDB_SQL_NULL_DATA;
			}
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClDinArr::SetLengthReadOnly (void)
{
	m4int32_t indData;

	for (indData=0; indData<m_lNumData; indData++)
	{
		if (m_poPDContainer[indData]->m_pPDMTData->m_bIsExtern)
		{
			m_poPDContainer[indData]->m_pPDValue->m_lLength = M4LDB_SQL_NTS;

			if (m_poPDContainer[indData]->m_pPDOldValue)
			{
				m_poPDContainer[indData]->m_pPDOldValue->m_lLength = M4LDB_SQL_NULL_DATA;
			}

			switch (m_poPDContainer[indData]->m_pPDMTData->m_m4Type)
			{
				case M4CL_M4_TYPE_DATE:
				case M4CL_M4_TYPE_TIMESTAMP:
				case M4CL_M4_TYPE_NUMBER:
					if ((! *(m_poPDContainer[indData]->m_pPDValue->m_ppDataPointer)) || 
						(! *((m4date_t*) *(m_poPDContainer[indData]->m_pPDValue->m_ppDataPointer))))
					{
						m_poPDContainer[indData]->m_pPDValue->m_lLength = M4LDB_SQL_NULL_DATA;
					}
					break;

				default:
					if ((! *(m_poPDContainer[indData]->m_pPDValue->m_ppDataPointer)) || 
						(! **(m_poPDContainer[indData]->m_pPDValue->m_ppDataPointer)))
					{
						m_poPDContainer[indData]->m_pPDValue->m_lLength = M4LDB_SQL_NULL_DATA;
					}
					break;
			}
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClDinArr::Backup (ClTblMem *ai_pTbl)
{
	m4return_t result;
	m4int32_t indData;

	ai_pTbl->Init (m_pGlobVars->GetpLConn()->m_pLdb,m_lNumData);

	for (indData=0; indData<m_lNumData; indData++)
	{
		ai_pTbl->SetIdent (indData, indData);

		ai_pTbl->SetM4Type (indData, m_poPDContainer[indData]->m_pPDMTData->m_m4Type);

		if (m_poPDContainer[indData]->m_pPDMTDataExtended)
		{
			ai_pTbl->SetIsPK (indData, m_poPDContainer[indData]->m_pPDMTDataExtended->m_bIsPK);

			/* Bug 0249803
			Las pks de auditoría no deben ponerse en los backups
			*/
			if( m_poPDContainer[indData]->m_pPDMTDataExtended->m_idIntFld == M4LDB_ID_EXECUTION
				|| m_poPDContainer[indData]->m_pPDMTDataExtended->m_idIntFld == M4LDB_ID_ORDER )
			{
				ai_pTbl->SetIsPK (indData, M4_FALSE);
			}
		}
	}

	for (indData=0; indData<m_lNumData; indData++)
	{
		m4int32_t lLength = m_poPDContainer[indData]->m_pPDValue->m_lLength;

		if (m_poPDContainer[indData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_LONG ||
			m_poPDContainer[indData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_UNI_LONG ||
			m_poPDContainer[indData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_BLOB ||
			m_poPDContainer[indData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_BINARY_STRING)
		{
			// Para post-validaciones no es necesario guardar los LONGs.
			lLength = M4LDB_SQL_NULL_DATA;
		}	
		
		if (lLength != M4LDB_SQL_NULL_DATA)
		{
			switch (m_poPDContainer[indData]->m_pPDMTData->m_m4Type)
			{
				case M4CL_M4_TYPE_DATE:
				case M4CL_M4_TYPE_TIMESTAMP:
				case M4CL_M4_TYPE_NUMBER:
					lLength = M4LDB_SIZE_DOUBLE;
					break;

				default:
					lLength = strlen ( *(m_poPDContainer[indData]->m_pPDValue->m_ppDataPointer)) + 1;
					break;
			}

			result = ai_pTbl->SetCurrentData ( *(m_poPDContainer[indData]->m_pPDValue->m_ppDataPointer), lLength);

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}
		else
		{
			// Dejamos el puntero en la misma dirección.
			result = ai_pTbl->ReserveCurrentData (0);

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}
	}

	for (indData=0; indData<m_lNumData; indData++)
	{
		if (m_poPDContainer[indData]->m_pPDOldValue)
		{
			m4int32_t lLength = m_poPDContainer[indData]->m_pPDOldValue->m_lLength;

			if (m_poPDContainer[indData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_LONG ||
				m_poPDContainer[indData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_UNI_LONG ||
				m_poPDContainer[indData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_BLOB ||
				m_poPDContainer[indData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_BINARY_STRING)
			{
				// Para post-validaciones no es necesario guardar los LONGs.
				lLength = M4LDB_SQL_NULL_DATA;
			}	
			
			if (lLength != M4LDB_SQL_NULL_DATA)
			{
				switch (m_poPDContainer[indData]->m_pPDMTData->m_m4Type)
				{
					case M4CL_M4_TYPE_DATE:
					case M4CL_M4_TYPE_TIMESTAMP:
					case M4CL_M4_TYPE_NUMBER:
						lLength = M4LDB_SIZE_DOUBLE;
						break;

					default:
						lLength = strlen ( *(m_poPDContainer[indData]->m_pPDOldValue->m_ppDataPointer)) + 1;
						break;
				}

				result = ai_pTbl->SetCurrentData ( *(m_poPDContainer[indData]->m_pPDOldValue->m_ppDataPointer), lLength);

				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}

				continue;
			}
		}

		// Dejamos el puntero en la misma dirección.
		result = ai_pTbl->ReserveCurrentData (0);

		if (result == M4_ERROR)
		{
			return (M4_ERROR);
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClDinArr::Restore (ClTblMem *ai_pTbl)
{
	m4return_t result;
	m4int32_t indCol;

	m4int32_t numCol = ai_pTbl->GetNumCol ();

	for (indCol=0; indCol<numCol; indCol++)
	{
		m4int32_t indData = ai_pTbl->GetIdent (indCol);

		m4int8_t m4Type = ai_pTbl->GetM4Type (indCol);

		if (indData != M4LDB_NULL_INDEX)
		{
            if (m_poPDContainer[indData]->m_pPDMTData->m_bIsExtern)
			{
                continue;
            }

			m4int32_t lNewLength = ai_pTbl->GetLength (0, indCol);

			if (lNewLength != M4LDB_SQL_NULL_DATA)
			{
				m4pcchar_t pNewValue = ai_pTbl->GetData (0, indCol);

				result = SetData (indData, pNewValue, lNewLength, m4Type);
			}
			else
			{
				result = SetData (indData);
			}

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}
	}

	for (indCol=0; indCol<numCol; indCol++)
	{
		m4int32_t indData = ai_pTbl->GetIdent (indCol);

		m4int8_t m4Type = ai_pTbl->GetM4Type (indCol);

		if (indData != M4LDB_NULL_INDEX)
		{
			if (m_poPDContainer[indData]->m_pPDOldValue)
			{
				m4int32_t lNewLength = ai_pTbl->GetLength (1, indCol);

				if (lNewLength != M4LDB_SQL_NULL_DATA)
				{
					m4pcchar_t pNewValue = ai_pTbl->GetData (1, indCol);

					result = SetOldData (indData, pNewValue, lNewLength, m4Type);
				}
				else
				{
					result = SetOldData (indData);
				}

				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}
			}
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClDinArr::Merge (ClTblMem *ai_pTbl, m4int32_t ai_indRow, m4pcchar_t ai_pccTramitation)
{
	m4return_t	result;
	m4int32_t	indCol;
	m4bool_t	bIsPk = M4_FALSE;

	m4int32_t numCol = ai_pTbl->GetNumCol ();

	for (indCol=0; indCol<numCol; indCol++)
	{
		m4int32_t indData = ai_pTbl->GetIdent (indCol);

		m4int8_t m4Type = ai_pTbl->GetM4Type (indCol);

		if (indData != M4LDB_NULL_INDEX)
		{
            if ((m_poPDContainer[indData]->m_pPDMTData->m_bIsExtern) && 
				(!m_poPDContainer[indData]->m_pPDOldValue))
			{
                continue;
            }

			m4int32_t lNewLength = ai_pTbl->GetLength (ai_indRow, indCol);

			bIsPk = GetInfoIsPK(indData);

			if (lNewLength != M4LDB_SQL_NULL_DATA)
			{
				m4pcchar_t pNewValue = ai_pTbl->GetData (ai_indRow, indCol);

				// Si hay que usar una tramitación especial se cambia el ID_DATASET
				if( ai_pccTramitation != NULL && GetInfoIdInternalField(indData) == M4LDB_ID_DATASET )
				{
					pNewValue = ai_pccTramitation ;
					lNewLength = strlen( ai_pccTramitation ) ;
				}

				//En updates de campos pks hay que asignar el old value.
				//En el resto el value
				if (m_poPDContainer[indData]->m_pPDOldValue && bIsPk == M4_TRUE)
				{
					result = SetOldData (indData, pNewValue, lNewLength, m4Type);
				}
				else
				{
					result = SetData (indData, pNewValue, lNewLength, m4Type);
				}
			}
			else
			{
				if (m_poPDContainer[indData]->m_pPDOldValue && bIsPk == M4_TRUE)
				{
					result = SetOldData (indData);
				}
				else
				{
					result = SetData (indData);
				}
			}
		}	
	}

	return (M4_SUCCESS);
}


eDiffType_t ClDinArr::Difference (m4int32_t ai_lIndData)
{
	if (m_poPDContainer[ai_lIndData]->m_pPDOldValue)
	{
		if (m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_LONG ||
			m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_UNI_LONG ||
			m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_BLOB ||
			m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_BINARY_STRING)
		{
			return (M4LDB_GREATER);
		}

		if (m_poPDContainer[ai_lIndData]->m_pPDValue->m_lLength == M4LDB_SQL_NULL_DATA)
		{
			if (m_poPDContainer[ai_lIndData]->m_pPDOldValue->m_lLength == M4LDB_SQL_NULL_DATA)
			{
				return (M4LDB_IDENTICAL);
			}
			return (M4LDB_LESS);
		}

		if (m_poPDContainer[ai_lIndData]->m_pPDOldValue->m_lLength == M4LDB_SQL_NULL_DATA)
		{
			return (M4LDB_GREATER);
		}

		m4double_t dMod = M4LDB_COMPARE_VALUES ( *(m_poPDContainer[ai_lIndData]->m_pPDValue->m_ppDataPointer), *(m_poPDContainer[ai_lIndData]->m_pPDOldValue->m_ppDataPointer), m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type);

		if (dMod)
		{
			if (dMod > 0)
			{
				return (M4LDB_GREATER);
			}
			else
			{
				return (M4LDB_LESS);
			}
		}
	}

	return (M4LDB_IDENTICAL);
}


m4return_t ClDinArr::ConvertData (m4int32_t ai_lIndData)
{

	switch (m_poPDContainer[ai_lIndData]->m_pPDMTDataExtended->m_idConvFunc)
	{
	case M4LDB_CONV_FUNC_NONE:
		
		return M4_SUCCESS;
		break;
	case M4LDB_CONV_FUNC_TO_UPPER:
		
		return _ConvertToUpperLower(ai_lIndData) ;
		break;
	case M4LDB_CONV_FUNC_TO_LOWER:

		return _ConvertToUpperLower(ai_lIndData,M4_FALSE);
		break;
	case M4LDB_CONV_FUNC_FIRST_CAP_LETTER:
		
		return _ConvertFirstCapLetter ( *(m_poPDContainer[ai_lIndData]->m_pPDValue->m_ppDataPointer));	
		break;
	default :
		
		return M4_ERROR;
	}

}


m4return_t ClDinArr::_ConvertToUpperLower		(m4int32_t ai_lIndData,m4bool_t ai_ToUpper)
{
	switch (m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type)
	{
	case M4CL_M4_TYPE_FIXED_STRING:
	case M4CL_M4_TYPE_UNI_VAR_STRING:
		{
			// Me tengo que hacer copia de la cadena
			// para luego poder ver si ha cambiado.
			if (m_poPDContainer[ai_lIndData]->m_pPDValue->m_lLength != M4LDB_SQL_NULL_DATA)
			{
				m4int32_t len = strlen ( *(m_poPDContainer[ai_lIndData]->m_pPDValue->m_ppDataPointer) ) + 1;
				m4pchar_t pTemp = new m4char_t [len];
				m4return_t ret;

				strcpy (pTemp, *(m_poPDContainer[ai_lIndData]->m_pPDValue->m_ppDataPointer) );

				if( ai_ToUpper )
				{
					// UNICODE LOCALE
					M4StrUpr( pTemp );
				}
				else
				{
					// UNICODE LOCALE
					M4StrLwr( pTemp );
				}
			
				ret = SetData (ai_lIndData, pTemp, len ,m_poPDContainer[ai_lIndData]->m_pPDMTData->m_m4Type);
				delete [] pTemp;
				return ret;
			}
			else 
				return M4_SUCCESS;
			break;
		}
	default:
		return M4_SUCCESS;
		break;
	}
}


m4return_t ClDinArr::_ConvertFirstCapLetter	(m4pchar_t ai_pData)
{
	// UNICODE LOCALE
	size_t	iBytesEaten = 0 ;
	M4ChrUpr(ai_pData, iBytesEaten);

	/*
	if (NewLetter!=(*ai_pData))
	{
		// Si ha cambiado lo actualizo a mano.Lo unico que me interesa es que 
		// quede marcado como que tiene diferencias.
		// La otra posibilidad sería llamar al SetData.
		(*ai_pData) = NewLetter;
	}
	*/

	return M4_SUCCESS;
}

m4return_t ClDinArr::ClearEncryptedBlobs (void)
{
	m4int32_t indData;
	for (indData=0; indData<m_lNumData; indData++)
	{
		if (m_poPDContainer[indData]->m_pPDMTData->m_m4Type == M4CL_M4_TYPE_BLOB)
		{
			if (m_poPDContainer[indData]->m_pPDValue->m_pPDEncryptedValue)
			{
				m_poPDContainer[indData]->m_pPDValue->m_pPDEncryptedValue->Reset();
			}
		}
	}
	return M4_SUCCESS;
}


m4return_t ClDinArr::DecryptAll(void)
{
	m4int32_t i=0;


	for (i=0;i<m_lNumData;++i)
	{
		if (m_poPDContainer[i]->m_pPDValue->m_pPDEncryptedValue)
		{
			if (m_poPDContainer[i]->m_pPDValue->DecryptValue(m_poPDContainer[i]->m_pPDMTData->m_m4Type, m_poPDContainer[i]->m_pPDMTData->m_lPrec, m_poPDContainer[i]->m_pPDMTData->m_lScale)!=M4_SUCCESS)
			{
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (),M4_CH_LDB_DECRYPTING_DATA,m_poPDContainer[i]->m_pPDValue->m_pPDEncryptedValue->GetDataPointer());
				return M4_ERROR;
			}
		}
	}
	return M4_SUCCESS;
}

ClPDEncryptedValue::ClPDEncryptedValue(ClPDEncryptedValue &ai_Encrypted)
{
	m_pEncryptedData = NULL;
	m_lMaxLen = -1 ;
	m_iRealLen = -1;
	m_bIsFile = ai_Encrypted.m_bIsFile ;
	ReserveBuffer(ai_Encrypted.m_lMaxLen);
}

m4return_t	ClPDEncryptedValue::Encrypt(m4pcchar_t ai_pData,m4uint32_t ai_iLen)
{
	m4int32_t	lLen = ai_iLen;
	m4int_t		retLen= 0;
	m4pchar_t	pcEncryptedData = NULL;

	if (ai_pData==0 || lLen == M4LDB_SQL_NULL_DATA)
	{
		Reset();
		return M4_SUCCESS;
	}

	if (lLen==0 || lLen == M4LDB_SQL_NTS)
	{
		lLen = strlen (ai_pData)+1;
	}

	if (lLen<0)
	{
		// ¿Cómo es posible?
		lLen = strlen (ai_pData)+1;
	}

	if (ReserveBuffer(2 * M4MaxLenOfEnCrypted(lLen))!=M4_SUCCESS)
	{
		return M4_ERROR;
	}

	if (M4DataConvert (ai_pData, lLen, pcEncryptedData, retLen)!=M4_SUCCESS)
	{
		Reset();
		return M4_ERROR;
	}

	memcpy(m_pEncryptedData, pcEncryptedData, retLen + 1);
	
	m_iRealLen=retLen;
	
	delete pcEncryptedData;
	
	return M4_SUCCESS;
}

void	ClPDEncryptedValue::Clear(void)
{
	if (m_bIsFile)
	{
		ClFileManager::Instance()->UnSetInterestInFile(m_pEncryptedData);
	}

	if (m_pEncryptedData)
	{
		delete [] m_pEncryptedData;
		m_pEncryptedData=NULL;
	}
	m_lMaxLen = -1;
	m_iRealLen = -1;
	m_bIsFile = M4_FALSE;

}

m4return_t	ClPDEncryptedValue::ReserveBuffer(m4int32_t ai_size)
{
	if (ai_size)
	{
		if (ai_size>m_lMaxLen)
		{
			// No nos vale el buffer que tenemos.
			Clear();
			m_pEncryptedData = new m4char_t[ai_size+1];
			if (m_pEncryptedData==0)
			{
				Reset();
				return M4_ERROR;
			}
			m_lMaxLen = ai_size;
		}
		m_iRealLen = -1;
		memset (m_pEncryptedData,0,m_lMaxLen+1);
	}
	else
	{
		Reset();
	}
	return M4_SUCCESS;
}

m4return_t ClPDValue::ReserveForEncrypted(m4int8_t ai_m4Type, m4int32_t ai_lPrec, m4bool_t ai_bIsUnicode)
{
	m4return_t ret = M4_SUCCESS;
	m4uint32_t iLength = 0 ;

	switch (ai_m4Type) 
	{
		case M4CL_M4_TYPE_LONG:
		case M4CL_M4_TYPE_UNI_LONG:
		case M4CL_M4_TYPE_BINARY_STRING:
			// No hay que reservar nada.
			break;

		case M4CL_M4_TYPE_BLOB:
			if (m_pPDEncryptedValue->ReserveBuffer(M4_MAX_PATH*2)!=M4_SUCCESS)
			{
				ret = M4_ERROR;
			}
			break;

		case M4CL_M4_TYPE_DATE:
		case M4CL_M4_TYPE_TIMESTAMP:
		case M4CL_M4_TYPE_NUMBER:
			if (m_pPDEncryptedValue->ReserveBuffer(2 * M4MaxLenOfEnCrypted(sizeof(m4double_t)))!=M4_SUCCESS)
			{
				ret = M4_ERROR;
			}
			break;

		default:
			/* Bug 0131004
			UNICODE DB
			La longitud debe ser el doble más el BOM, estemos en unicode o no, ya que el campo puede estar en UTF16
			Dejamos el +1 por compatibilidad, aunque no debería ser necesario encriptar el cero final
			*/
			iLength = ai_lPrec + 1 ;

			/* Bug 0159568
			Sólo se pone el doble de precisión y el BOM si es realmente un campo unicode
			*/
			if( ai_bIsUnicode == M4_TRUE )
			{
				iLength *= sizeof( M4XMLCh ) ;
				iLength += M4UNI_UTF16_BOM_SIZE ;
			}

			if (m_pPDEncryptedValue->ReserveBuffer(2 * M4MaxLenOfEnCrypted(iLength))!=M4_SUCCESS)
			{
				ret = M4_ERROR;
			}

			break;
	}

	return ret;
}



m4pint32_t ClPDValue::GetMaxLenPointer( m4int32_t ai_lPrec )
{
	if( m_lMaxLen == -1 )
	{
		m_lMaxLen = ai_lPrec;
	}
	return( &m_lMaxLen ) ;
}


m4return_t ClPDValue::EncryptValue(m4int8_t ai_m4Type, m4bool_t ai_bIsUnicode)
{
	if (m_lLength == M4LDB_SQL_NULL_DATA)
	{
		m_pPDEncryptedValue->Reset();
		return M4_SUCCESS;
	}

	m4return_t		ret = M4_SUCCESS;
	int				iLength = -1 ;
	m4pchar_t		pcData = NULL ;
	M4XMLCh*		pcUtf16Data = NULL ;
	
	switch (ai_m4Type)
	{
		case M4CL_M4_TYPE_LONG:
		case M4CL_M4_TYPE_UNI_LONG:
		case M4CL_M4_TYPE_BINARY_STRING:
			if (m_pPDEncryptedValue->Encrypt(*m_ppDataPointer,m_lLength)!=M4_SUCCESS)
			{
				ret = M4_ERROR;
			}
			break;

		case M4CL_M4_TYPE_BLOB:
			{
				// Voy a guardar : FileName+0+CabeceraEcnriptada+0+ExtensionEncriptada+0.
				// Me llega FileName+0+Cabecera+0+Extension+0

				// Me tengo que crear un fichero nuevo, para que se pueda acceder despues al que no esta encriptado.
	
				// Extension del fichero :
				m_pPDEncryptedValue->Reset();

				m4pchar_t	pHeader=0,pExt=0;
				m4char_t	Buffer[M4_MAX_PATH*2];
				m4pchar_t	pcHeaderCrypted = NULL;
				m4pchar_t	pcExtCrypted = NULL;
				m4int_t		retLen = 0;

				pHeader = *m_ppDataPointer + strlen (*m_ppDataPointer)+1;

				pExt = pHeader + strlen (pHeader)+1;

				m4uint32_t SizeHeader = strlen (pHeader);
				m4uint32_t SizeHeader_crypted = 2 * M4MaxLenOfEnCrypted(SizeHeader);
				m4uint32_t SizeExt = strlen (pExt);
				m4uint32_t SizeExt_crypted = 2 * M4MaxLenOfEnCrypted(SizeExt);
				
				// El fichero lo desencripto a  otro fichero temporal, porque ocupan un monton los dos buffers en memoria.
				
				ClFileManager * pFM = ClFileManager::Instance();
				ClFileName oFN;
				m4char_t pFN[M4_MAX_PATH+1];

				if (pFM->CreateTempFile(oFN,0,pExt,0)!=M4_SUCCESS)
				{
					return M4_ERROR;
				}
				
				oFN.GetFileName(pFN);

				// Cutremente hasta que este la funcion de Oscar.

				if (M4ConvertFile(*m_ppDataPointer,pFN)!=M4_SUCCESS)
				{
					pFM->UnSetInterestInFile(oFN);
					return M4_ERROR;
				}

				// Copiamos el nombre del fichero al buffer.

				m4uint32_t SizeFileName = strlen (pFN);
				m4uint32_t TotalSize=0;
				m4uint32_t FileSize = 0;

				strcpy(Buffer,pFN);

				TotalSize+=SizeFileName+1;

				// Encriptamos cabecera y extension en el buffer.
				// Comprobamos que no nos pasemos

				if (SizeHeader_crypted+SizeExt_crypted+SizeFileName+3>M4_MAX_PATH*2)
				{
					// Se han pasado un poco
					pFM->UnSetInterestInFile(oFN);
					return M4_ERROR;
				}

				if (M4DataConvert(pHeader,SizeHeader,pcHeaderCrypted,retLen)!=M4_SUCCESS)
				{
					pFM->UnSetInterestInFile(oFN);
					return M4_ERROR;
				}

				memcpy(Buffer + TotalSize, pcHeaderCrypted, retLen + 1);
				delete pcHeaderCrypted;

				TotalSize+=retLen+1;

				if (M4DataConvert(pExt,SizeExt,pcExtCrypted,retLen)!=M4_SUCCESS)
				{
					m_pPDEncryptedValue->Reset();
					pFM->UnSetInterestInFile(oFN);
					return M4_ERROR;
				}

				memcpy(Buffer + TotalSize, pcExtCrypted, retLen + 1);
				delete pcExtCrypted;

				TotalSize+=retLen+1;

				// UNICODE FILE
				int iLength = -1 ;
				char* pcFileName = M4CppToANSI( Buffer, iLength ) ;

				struct stat stFile;
				if (stat (pcFileName, &stFile) == 0)
				{
					FileSize = stFile.st_size;
				}
				delete [] pcFileName ;

				m_pPDEncryptedValue->SetBlob(Buffer,TotalSize,TotalSize+FileSize);

			}
			break;

		case M4CL_M4_TYPE_DATE:
		case M4CL_M4_TYPE_TIMESTAMP:
		case M4CL_M4_TYPE_NUMBER:
			{
				m4double_t dArch = *(m4double_t*) *m_ppDataPointer;
				if (M4GetByteOrder(M4_ARCHITECTURE)==M4_BYTEORDER_NORMAL)
				{
					// Lo invertimos. Lo vamos a guardar como Little
					SwapDouble((m4pchar_t)&dArch,0);

				}
				// Convertimos el binario directamente.
				if (m_pPDEncryptedValue->Encrypt((m4pchar_t)&dArch,sizeof(m4double_t)))
				{
					return M4_ERROR;
				}
				break;
			}

			
		default:

			/* Bug 0131004
			UNICODE DB
			Si se está en unicode hay que pasar el valor a UTF16 y poner la cabecera
			Dejamos el +1 por compatibilidad, aunque no debería ser necesario encriptar el cero final
			*/
			iLength = -1 ;

			/* Bug 0147474
			Se decide pasar a unicode o no según el tipo de campo y no de entorno
			*/
			if( ai_bIsUnicode == M4_TRUE )
			{
				pcUtf16Data = M4CppToUtf16( *m_ppDataPointer, iLength ) ;

				// iLength++ ;
				iLength *= sizeof( M4XMLCh ) ;
				iLength += M4UNI_UTF16_BOM_SIZE ;

				pcData = new m4char_t[ iLength ] ;

				memcpy( pcData, acUTF16BOM, M4UNI_UTF16_BOM_SIZE ) ;
				memcpy( pcData + M4UNI_UTF16_BOM_SIZE, pcUtf16Data, iLength - M4UNI_UTF16_BOM_SIZE ) ;

				delete [] pcUtf16Data ;
			}
			else
			{
				pcData = M4CppToANSI( *m_ppDataPointer, iLength ) ;
				// iLength++ ;
			}

			if (m_pPDEncryptedValue->Encrypt(pcData, iLength)!=M4_SUCCESS)
			{
				ret = M4_ERROR;
			}
			delete [] pcData ;
			break;

	}
	return ret;
}

m4return_t ClPDValue::DecryptValue(m4int8_t ai_m4Type, m4int32_t ai_lPrec, m4int32_t ai_lScale)
{
	m4int32_t	lLenCryp=0,lLenDeCrypt=0;
	m4pchar_t	nnn;
	m4int8_t	nofdec,pointpos=0;
	m4pchar_t	ppp;
	m4double_t  number =0;
	//	El item que vamos a leer no es long ni blob, por lo que deberia tener espacio suficiente.con un LONG
	// UNICODE DB
	/* Bug 0131004
	En principio no hay que multiplicar la longitud por dos porque con 4kb debería bastar
	*/
	m4char_t	TempBuffer[M4LDB_SIZE_LONG];
	m4pchar_t	pBufferDecrypt = NULL;

	if (ai_m4Type != M4CL_M4_TYPE_BLOB && ai_m4Type != M4CL_M4_TYPE_LONG && ai_m4Type != M4CL_M4_TYPE_UNI_LONG && ai_m4Type!= M4CL_M4_TYPE_BINARY_STRING)
	{
		// Desencriptamos.
		if (m_pPDEncryptedValue->Decrypt(pBufferDecrypt,lLenDeCrypt)!=M4_SUCCESS)
		{
			return M4_ERROR;
		}

		if (lLenDeCrypt==-1)
		{
			m_lLength=-1;
			delete pBufferDecrypt;
			return M4_SUCCESS;
		}

		memcpy(TempBuffer, pBufferDecrypt, lLenDeCrypt + 1);
		delete pBufferDecrypt;

		// Convertimos al tipo del item:
		switch (ai_m4Type)
		{
			case M4CL_M4_TYPE_DATE :
			case M4CL_M4_TYPE_TIMESTAMP :

				number = *(m4double_t *)TempBuffer;
				if (M4GetByteOrder(M4_ARCHITECTURE)==M4_BYTEORDER_NORMAL)
				{
					// Lo hemos guardado en Little, invertimos.
					SwapDouble((m4pchar_t )&number,0);
				}
				// Copiamos el binario.
				memcpy(*m_ppDataPointer,(m4pchar_t)&number,sizeof(m4double_t));
				break;
			case M4CL_M4_TYPE_NUMBER :
				number = *(m4double_t *)TempBuffer;
				if (M4GetByteOrder(M4_ARCHITECTURE)==M4_BYTEORDER_NORMAL)
				{
					// Lo hemos guardado en Little, invertimos.
					SwapDouble((m4pchar_t )&number,0);
				}
				sprintf(TempBuffer,"%lf",number);
				lLenDeCrypt= strlen(TempBuffer);
				if (lLenDeCrypt>ai_lPrec){
					TempBuffer[ai_lPrec]=0;
				}
				// Busco el punto para hallar el numero de de decimales.
				ppp = TempBuffer;
				while (*ppp!='.'&&*ppp!=0)
				{
					++pointpos;
					++ppp;
				}
				nofdec = ai_lPrec - pointpos -1;
				if (nofdec>ai_lScale)
				{
					TempBuffer[pointpos+ai_lScale+1]=0;
				}
				*((m4double_t *) *m_ppDataPointer)= strtod(TempBuffer,&nnn);
				break;

			default:

				// UNICODE DB
				/* Bug 0130721
				Puede que el buffer de salida se quede pequeño
				Si es así hay que redimensionarlo hasta el número de caracteres de la precisión
				*/
				size_t			iChars = 0 ;
				size_t			iSize = 0 ;
				int				iLength = -1 ;
				int				iPosition = 0 ;
				m4int32_t		*plMaxLen = NULL ;
				m4pchar_t		pcData = NULL ;
				M4XMLCh*		pcUtf16Data = NULL ;

				// UNICODE DB
				/* Bug 0131004
				Hay que pasar el valor del formato de base de datos, que puede ser ANSI o UTF16
				al formato local, que puede ser ANSI o UTF8
				*/
				if( lLenDeCrypt >= M4UNI_UTF16_BOM_SIZE
					&&
					(
						memcmp( TempBuffer, acUTF16WinBOM, M4UNI_UTF16_BOM_SIZE ) == 0
						||
						memcmp( TempBuffer, acUTF16UnixBOM, M4UNI_UTF16_BOM_SIZE ) == 0
					)
				  )
				{
					iLength = ( lLenDeCrypt - M4UNI_UTF16_BOM_SIZE ) / sizeof( M4XMLCh ) ;
					pcUtf16Data = (M4XMLCh*) ( TempBuffer + M4UNI_UTF16_BOM_SIZE ) ;

					if( memcmp( TempBuffer, acUTF16BOM, M4UNI_UTF16_BOM_SIZE ) != 0 )
					{
						// Si la arquitectura en la que se grabó no es la misma hay que darlos la vuelta
						M4SwapUtf16( pcUtf16Data, iLength ) ;
					}

					/* Bug 0206239
					No se puede asumir que venga null terminado para utf16
					iLength = -1 ;
					*/
					pcData = M4Utf16ToCpp( pcUtf16Data, iLength ) ;
				}
				else
				{
					iLength = -1 ;
					pcData = M4ANSIToCpp( TempBuffer, iLength ) ;
				}

				iChars = M4StrLen( pcData, iLength ) ;

				if( iChars > ai_lPrec )
				{
					iChars = ai_lPrec ;
				}

				iPosition = M4CharPosition( pcData, iLength, iChars, iSize ) ;

				plMaxLen = GetMaxLenPointer( ai_lPrec ) ;

				if( iPosition > *plMaxLen )
				{
					delete [] *m_ppDataPointer ;
					*plMaxLen = iPosition ;
					*m_ppDataPointer = new m4char_t[ *plMaxLen + 1 ] ;
				}

				memcpy( *m_ppDataPointer, pcData, iPosition ) ;
				(*m_ppDataPointer)[ iPosition ] = '\0' ;

				delete [] pcData ;

				break;
		}
		m_lLength = *(m_pPDEncryptedValue->GetpLength());
	}
	return M4_SUCCESS;
}

m4return_t	ClPDEncryptedValue::Decrypt(m4pchar_t & ai_pData,m4int32_t &ao_Len)
{
	m4int_t iii = ao_Len;

	// Inicializaciones
	ai_pData = NULL;
	ao_Len = 0;


	// Cadena vacia
	if (m_iRealLen==0)
	{
		ai_pData = new m4char_t[1];
		*ai_pData = '\0';
		return M4_SUCCESS;
	}

	if (m_iRealLen==-1)
	{
		ai_pData = new m4char_t[1];
		*ai_pData = '\0';
		ao_Len = -1;
		return M4_SUCCESS;
	}

	if (M4DataDeConvert (m_pEncryptedData,m_iRealLen,ai_pData,iii)!=M4_SUCCESS)
	{
		return M4_ERROR;
	}

	ao_Len=iii;

	return M4_SUCCESS;
}

void	ClPDEncryptedValue::SetBlob(m4pchar_t ai_pBuffer,m4int32_t ai_lSize,m4int32_t ai_lFictSize)
{
	Reset();
	ReserveBuffer(ai_lSize);

	memcpy(m_pEncryptedData,ai_pBuffer,ai_lSize);
	if (ai_lFictSize == -1)
	{
		m_iRealLen = ai_lSize;
	}
	else
	{
		m_iRealLen = ai_lFictSize ;
	}
	m_bIsFile = M4_TRUE;
}

void	ClPDEncryptedValue::Reset()
{
	if (m_bIsFile)
	{
		ClFileManager::Instance()->UnSetInterestInFile(m_pEncryptedData);
	}
	if (m_pEncryptedData==NULL)
	{
		m_lMaxLen = -1;
	}
	else
	{
		memset(m_pEncryptedData, 0, m_lMaxLen+1);
	}
	m_iRealLen = -1;
	m_bIsFile = M4_FALSE;
}


m4return_t ClDinArr::CreateData (m4int32_t ai_lIndData, ClPDContainer *ai_poPDContainer)
{
	if (ai_lIndData >= m_lMaxNumData)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClDinArr::CreateData" << "Array bounds exceeded");
		return (M4_ERROR);
	}

	if (ai_lIndData >= m_lNumData)
	{
		m_lNumData = ai_lIndData + 1;
	}

	if (!m_poPDContainer[ai_lIndData])
	{
		m_poPDContainer[ai_lIndData] = new ClPDContainer (ai_poPDContainer);
	}

	return (M4_SUCCESS);
}


m4return_t ClDinArr::CreateData (m4int32_t ai_lIndData,ClMetaParam	*ai_pPar)
{
	return CreateData (ai_lIndData, NULL, ai_pPar->GetHandle(m_pGlobVars), ai_pPar->GetM4Type(m_pGlobVars), ai_pPar->GetPrecision(m_pGlobVars), ai_pPar->GetScale(m_pGlobVars), NULL, NULL, ai_pPar->GetType(), M4_FALSE);
}


m4return_t ClDinArr::InsertIdData (m4id_t ai_idField, m4int32_t ai_lIndData)
{
	struct stIdField stNew;

	stNew.m_pIdField = ai_idField;

	m_pmpIdData->insert (MpIdData::value_type (stNew, ai_lIndData));

	return (M4_SUCCESS);
}
