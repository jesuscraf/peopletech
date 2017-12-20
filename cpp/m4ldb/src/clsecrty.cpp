//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb
// File:                clsecrty.cpp
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


#include "dm.hpp"
#include "dm_types.hpp"

#include "clldb.hpp"
#include "cllconn.hpp"
#include "apisql.hpp"
#include "cldinarr.hpp"
#include "px_block.hpp"
#include "tfres.hpp"
#include "ldb_log.hpp"
#include "m4win2ux.hpp"


#ifdef _M4LDB_DEBUG
	#include "clldb.hpp"
	#undef M4LDB_PRINT_DEBUG
	#define M4LDB_PRINT_DEBUG	if (m_lSysDebugLevel <= m_pLdb->GetSysDebugLevel ()) m_pInspector->PrintFileDebug

	#undef M4LDB_PRINT_DEBUG_DETAIL
	#define M4LDB_PRINT_DEBUG_DETAIL	if (m_lSysDebugLevel <= m_pLdb->GetSysDebugLevel ()) m_pInspector->PrintFileDebugDetail
#endif


void ConvertToUpper (m4pchar_t &ao_pStm)
{
	m4pchar_t pCurrent = ao_pStm;
	m4bool_t bString = M4_FALSE;

	while (*pCurrent)
	{
		if (*pCurrent== '\'' && *(pCurrent-1) != '\\')
		{
			bString = M4_BOOL(!bString);
		}

		if (!bString && *pCurrent >= 'a' && *pCurrent <= 'z')
		{
			// Convertimos a mayusculas.
			*pCurrent = *pCurrent - 32;
		}

		pCurrent++;
	}
}


m4return_t ClLConn::_X_AnalizeSecurity (ClAPISQL *ai_pAPISQL, ClMetaSentence *&aio_pMtSent, ClGlobVars *ai_pGlobVars, ClAccessRecordSet *ai_pRecordSet, m4int32_t *ao_plSecMask, eStmType_t ai_eStmType, ClDinArr *ai_pParam)
{
	// Se analizan la seguridad a estos niveles :
	//    * Se analiza el acceso a los objetos logicos
	//    * Se crean los filtros de seguridad que se necesitan por cada objeto logico
	//    * Se analiza el acceso a los campos
	if (!strcmpi (ai_pGlobVars->GetIdRSM (), M4CL_SEC_SUPER_USER))
	{
		// SuperRol.
		if (ao_plSecMask)
		{
			*ao_plSecMask = M4LDB_SUPERROLE_SEC_MASK;
		}

		return (M4_SUCCESS);
	}

	eStmType_t eStmType = aio_pMtSent != NULL ? aio_pMtSent->GetStmType() : ai_eStmType;

	m4bool_t bHasSecurityOptimization = ((aio_pMtSent) && (aio_pMtSent->HasOr ())) ? M4_FALSE : m_pLdb->HasSecurityOptimization ();
	m4bool_t bUseSecurityView = M4_FALSE;

	if( eStmType == M4LDB_SELECT )
	{
		if( ai_pGlobVars->HasNotValidation( M4LDB_NO_USE_SECURITY_VIEW ) == M4_FALSE )
		{
			if( m_pLdb->UseSecurityView() == M4_TRUE || ai_pGlobVars->HasNotValidation( M4LDB_USE_SECURITY_VIEW ) == M4_TRUE )
			{
				// Bug 0301311 Con FOR UPDATE no se aplican vistas de seguridad
				if( aio_pMtSent == NULL || aio_pMtSent->GetForUpdate() == M4_FALSE )
				{
					bUseSecurityView = M4_TRUE ;
				}
			}
		}
	}

	/* Bug 0265804
	Si se desactiva la reducción de relaciones de seguridad se va por el modelo antiguoa para no agrupar xjoins
	*/
	// Sólo podemos aplicar la optimización si no tenemos ORs.
	if( bHasSecurityOptimization && bUseSecurityView == M4_FALSE )
	{
		return (_X_X_AnalizeSecurity (ai_pAPISQL, aio_pMtSent, ai_pGlobVars, ai_pRecordSet, ao_plSecMask, ai_eStmType, ai_pParam));
	}

	m4return_t result;
	m4bool_t bBase = M4_FALSE;
	string stFilterSecurity(M4LDB_EMPTY);
	string stAux(M4LDB_EMPTY);
	ClRegisterIndex indRSM(M4LDB_NULL_INDEX);
	ClMetaSentence *pMtSent = aio_pMtSent;
	ClMetaSentence *pMtSecSent = aio_pMtSent;

	m4int32_t iMask = 1 << (((m4int32_t) eStmType) - 1);

	// Recordar que la mascara se contruye:
	// (1*MASK_DEL)+(2*MASK_INS)+(4*MASK_UPD)+(8*MASK_SEL)+
	// (16*MASK_COR_DEL)+(32*MASK_COR_INS)+(64*MASK_COR_UPD)+
	// (128*MASK_DFS_CLS_DATE)+(256*MASK_HIST_ADAPT)+(512*MASK_DEL_CSC)+
	// (1024*MASK_UPD_PK_CSC)+(2048*MASK_DT_START_CSC)+(4096*MASK_DT_END_CSC)

	m4int32_t numObj = 1;

	if (pMtSent)
	{		
		if( eStmType != M4LDB_SELECT && !pMtSent->IsAPISQL() )
		{
			pMtSent = NULL;
		}
		else
		{
			numObj = pMtSent->GetNumObject ();
		}
	}
	
	for (m4int32_t indObj=0; indObj<numObj; indObj++)
	{
		m4bool_t bApplyFilter = M4_TRUE;

		ClMetaObject *pLgcTbl = NULL;

		// Para las siguientes tablas comprobamos que se puedan leer.
		eStmType_t eStmTypeAux = (indObj) ? M4LDB_SELECT : eStmType;

		// Si no entramos es porque ya estamos situados.
		if (pMtSent)
		{
			pLgcTbl = pMtSent->GetObjectByPos (indObj);

			if (pLgcTbl->m_bIsBasis)
			{
				if (bBase)
				{
					// La tabla base ya ha sido añadida por lo que no es necesario volver a añadir el filtro.
					bApplyFilter = M4_FALSE;
				}
				else
				{
					bBase = M4_TRUE;
				}
			}

			m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[(ClRegisterIndex) pLgcTbl->m_indLogicTable];
		}

		m4bool_t bHasSecurity = M4LDB_BOOL (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_HAS_SECURITY].Value.GetWithOutSec ());

		if (!bHasSecurity)
		{
			continue;
		}

		//Si estamos en persistencia no tendremos en cuenta las subselects.
		if (eStmType != M4LDB_SELECT && indObj > 0)
		{
			m4id_t iLgcTbl = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
			DUMP_LDBINSP_WARNINGF (this, M4_CH_LDB_NOT_APPLY_SUBSELECT_SECURITY, iLgcTbl);

			continue;
		}

		if (!*ai_pGlobVars->GetIdRSM ())
		{
			m4id_t nLgcTbl = GetnLgcTbl (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Index);
			m4id_t iLgcTbl = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());

			// Aqui usamos el eStmType genérico.
			if (eStmType == M4LDB_SELECT)
			{
				if( M4ChLogIsErrorIn( M4_CH_LDB_NO_SECURITY_DEFINED ) == 0 )
				{
					DUMP_LDBINSP_WARNINGF (this, M4_CH_LDB_NO_SECURITY_DEFINED, LOG_CAT_AND (iLgcTbl, nLgcTbl) << "<null>");
				}
				return (M4_WARNING);
			}
			else
			{
				DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_NO_SECURITY_DEFINED, LOG_CAT_AND (iLgcTbl, nLgcTbl) << "<null>");
				return (M4_ERROR);
			}
		}

		indRSM = FindIndexById (M4LDB_SYS_DICT_SEC_LOBJ, ai_pGlobVars->GetIdRSM ());

		if (indRSM == M4LDB_NULL_INDEX)
		{
			m4id_t nLgcTbl = GetnLgcTbl (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Index);
			m4id_t iLgcTbl = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());

			// Aqui usamos el eStmType genérico.
			if (eStmType == M4LDB_SELECT)
			{
				if( M4ChLogIsErrorIn( M4_CH_LDB_NO_SECURITY_DEFINED ) == 0 )
				{
					DUMP_LDBINSP_WARNINGF (this, M4_CH_LDB_NO_SECURITY_DEFINED, LOG_CAT_AND (iLgcTbl, nLgcTbl) << ai_pGlobVars->GetIdRSM ());
				}
				return (M4_WARNING);
			}
			else
			{
				DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_NO_SECURITY_DEFINED, LOG_CAT_AND (iLgcTbl, nLgcTbl) << ai_pGlobVars->GetIdRSM ());
				return (M4_ERROR);
			}
		}

		// Situamos el current del nodo M4LDB_SYS_DICT_SEC_LOBJ en el registro actual.
		m_paRS[M4LDB_SYS_DICT_SEC_LOBJ]->Current[indRSM];

		m4int32_t iMaskObj = M4LDB_NUMBER (m_paRS[M4LDB_SYS_DICT_SEC_LOBJ]->Current.Item[M4LDB_ITEM_SYS_DICT_SEC_LOBJ_MASK].Value.GetWithOutSec ());

		if (!(iMaskObj & iMask))
		{
			m4id_t nLgcTbl = GetnLgcTbl (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Index);
			m4id_t iLgcTbl = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());

			switch (eStmTypeAux)
			{
				case M4LDB_SELECT:
					if( M4ChLogIsErrorIn( M4_CH_LDB_READ_ACCESS_REFUSE ) == 0 )
					{
						DUMP_LDBINSP_WARNINGF (this, M4_CH_LDB_READ_ACCESS_REFUSE, LOG_CAT_AND (iLgcTbl, nLgcTbl));
					}
					return (M4_WARNING);
					break;
				case M4LDB_INSERT:
					DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_INSERT_ACCESS_REFUSE, LOG_CAT_AND (iLgcTbl, nLgcTbl));
					break;
				case M4LDB_UPDATE:
					DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_UPDATE_ACCESS_REFUSE, LOG_CAT_AND (iLgcTbl, nLgcTbl));
					break;
				case M4LDB_DELETE:
					DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_DELETE_ACCESS_REFUSE, LOG_CAT_AND (iLgcTbl, nLgcTbl));
					break;
			}

			return (M4_ERROR);
		}

		result = LoadDictionaryChannelRSM ();
			
		if (result == M4_ERROR)
		{
			return (M4_ERROR);
		}

		if (eStmTypeAux != M4LDB_SELECT && ao_plSecMask)
		{
			*ao_plSecMask = iMaskObj;
		}

		if (bApplyFilter)
		{
			stAux = M4LDB_EMPTY;

			m4pcchar_t pAlias =	NULL;
			
			// Aqui usamos el eStmType genérico.
			if (eStmType == M4LDB_SELECT && bUseSecurityView == M4_FALSE)
			{
				pAlias = pMtSent->GetAliasByIdx (pLgcTbl->m_indAlias);
			}
			
			// Extraer todos los filtros de los objetos logicos
			result = _X_FindFilterByIdObject (eStmTypeAux, stAux, pAlias);
			
			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
			
			if (!stAux.empty ())
			{
				#ifdef _M4LDB_DEBUG
					M4LDB_PRINT_DEBUG (this, "\n\n APISQL Statement in Security Filter for %s = %s.\n", pLgcTbl->m_stObject.c_str(), stAux.c_str ());
					M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_LOGIC_SENTENCES, this, "\n\n APISQL Statement in Security Filter for %s = %s.\n", pLgcTbl->m_stObject.c_str(), stAux.c_str ());
				#endif

				if( eStmTypeAux != M4LDB_SELECT || bUseSecurityView == M4_TRUE)
				{
					// Cambiamos la metasentencia para escritura o vistas de seguridad
					pMtSecSent = new ClMetaSentence;
					pMtSecSent->Create (ai_pGlobVars, M4LDB_SELECT, M4_TRUE, M4_FALSE, M4_FALSE, M4_FALSE, M4_TRUE);
					pMtSecSent->SetParam (ai_pParam);
					stFilterSecurity.append ("SELECT 1 ");
				}

				stFilterSecurity.append (stAux);

				if( bUseSecurityView == M4_TRUE )
				{
					// Si hay que usar vista de seguridad se genera la vista real y se ignora el filtro de seguridad
					ClAPISQL o_X_APISQL( M4LDB_APISQL_SELECT );
					o_X_APISQL.SetIsSecurityView();
					result = o_X_APISQL.AnalizeSQL( stFilterSecurity, pMtSecSent, M4_FALSE, M4_FALSE );
					stFilterSecurity.clear();

					if( result != M4_SUCCESS )
					{
						delete pMtSecSent;
						return( M4_ERROR );
					}

					/* Bug 0267418
					Hay que decir que deben ordenarse los parámetros y tomarlos después de la ordenación
					*/
					pMtSecSent->SetSortParamsColumnsLimitation( M4_TRUE );
					result = pMtSecSent->TranslateSentence( pLgcTbl->m_sSecurityView, -1, -1, M4_TRUE );
					pLgcTbl->SetSecurityParams( pMtSecSent->m_pvcParamWhr );
					delete pMtSecSent;

					if( result != M4_SUCCESS )
					{
						return( M4_ERROR );
					}

					size_t	iOne = pLgcTbl->m_sSecurityView.find( "1" );
					pLgcTbl->m_sSecurityView.replace( 0, iOne + 1, "(SELECT B0.*" );
					pLgcTbl->m_sSecurityView.append (")");
				}
			}
		}

		m4bool_t bHasSecurityFld = M4LDB_BOOL (m_paRS[M4LDB_SYS_DICT_SEC_LOBJ]->Current.Item[M4LDB_ITEM_SYS_DICT_SEC_LOBJ_HAS_SECURITY_FLD].Value.GetWithOutSec ());

		if (!bHasSecurityFld)
		{
			continue;
		}

		if (pMtSent)
		{
			m4int32_t numFld = pMtSent->GetNumFields ();

			for (m4int32_t indFld=numFld-1; indFld>=0; indFld--)
			{
				ClMetaField *pFld = pMtSent->GetFieldByPos (indFld);

				if (pFld->IsMetaData ()==M4_TRUE)
				{
					if ((pFld->m_eClause == M4LDB_CLAUSE_COLUMN) || 
						(pFld->m_eClause == M4LDB_CLAUSE_LONG_COLUMN)) {

						// Aqui usamos el eStmType genérico.
						if (eStmType == M4LDB_SELECT)
						{
							if (pLgcTbl->m_indAlias == pFld->m_indAlias)
							{
								m4bool_t bCanReadField = _X_CanAccessByField (pFld->m_stField.c_str (), M4_TRUE);

								m4bool_t bCanReadItem = pMtSent->CanReadByPos (pFld->m_iColumn);

								if (!bCanReadField)
								{
									if (bCanReadItem)
									{
										// Warning funcional.
										m4id_t nLgcTbl = GetnLgcTbl (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Index);
										m4id_t iLgcTbl = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
										
										DUMP_LDBINSP_WARNINGF (this, M4_CH_TF_READ_ACCESS_FIELD_REFUSE, pFld->m_stField.c_str () <<	LOG_CAT_AND (iLgcTbl, nLgcTbl));
									}

									pMtSent->InvalidateColumn (pFld->m_iColumn);
								}
								else
								{
									if (!bCanReadItem)
									{
										// Warning tecnológico.
										m4id_t nLgcTbl = GetnLgcTbl (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Index);
										m4id_t iLgcTbl = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
										
										DUMP_LDBINSP_WARNINGF (this, M4_CH_LDB_READ_ACCESS_FIELD_REFUSE, 
											pFld->m_stField.c_str () <<
											LOG_CAT_AND (iLgcTbl, nLgcTbl)
											);

										pMtSent->InvalidateColumn (pFld->m_iColumn);
									}
								}
							}
						}
						else
						{
							// Nada que validar.
						}
					}
					else
					{
						if (pLgcTbl->m_indAlias == pFld->m_indAlias)
						{
							// Para el WHERE y demás siempre de lectura.
							m4bool_t bCanReadField = _X_CanAccessByField (pFld->m_stField.c_str (), M4_TRUE);

							if (!bCanReadField)
							{
								m4id_t nLgcTbl = GetnLgcTbl (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Index);
								m4id_t iLgcTbl = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
								
								// Aqui usamos el eStmType genérico.
								if (eStmType == M4LDB_SELECT)
								{
									if( M4ChLogIsErrorIn( M4_CH_LDB_READ_ACCESS_FIELD_REFUSE ) == 0 )
									{
										DUMP_LDBINSP_WARNINGF (this, M4_CH_LDB_READ_ACCESS_FIELD_REFUSE, pFld->m_stField.c_str () << LOG_CAT_AND (iLgcTbl, nLgcTbl));
									}
									return (M4_WARNING);
								}

								DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_READ_ACCESS_FIELD_REFUSE, pFld->m_stField.c_str () << LOG_CAT_AND (iLgcTbl, nLgcTbl));
								return (M4_ERROR);
							}
						}
					}
				}
			}
		}

		if (eStmTypeAux == M4LDB_INSERT || eStmTypeAux == M4LDB_UPDATE)
		{
			m4int32_t numParam = ai_pParam->GetNumData ();

			m4int32_t indParam;

			for (indParam=0; indParam<numParam; indParam++)
			{
				m4int32_t hItem = ai_pParam->GetItemHandle (indParam);

				if (hItem)
				{
					m4id_t idField = ai_pParam->GetInfoIdField (indParam);

					m4bool_t bCanWriteField = _X_CanAccessByField (idField, M4_FALSE);
					m4bool_t bCanReadField = _X_CanAccessByField (idField, M4_TRUE);

					if (!bCanWriteField)
					{
						ai_pParam->SetInfoHasSecurity (indParam);
					}
					if (!bCanReadField)
					{
						ai_pParam->SetInfoHasSecurityReadInPersist (indParam);
					}
				}
			}
		}
	}
	
	if (!stFilterSecurity.empty ())
	{
		if (ai_pAPISQL)
		{
			//Pedro: Nos ponemos a analizar la seguridad.
			ai_pAPISQL->SetAPISQLSource(M4LDB_APISQL_FROM_SECURITY);
			result = ai_pAPISQL->AnalizeSQL (stFilterSecurity, pMtSecSent, M4_TRUE);
			//Pedro: Continuamos con el API.
			ai_pAPISQL->SetAPISQLSource(M4LDB_APISQL_FROM_APISQL);

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}
		else
		{
			aio_pMtSent = pMtSecSent;

			ClAPISQL o_X_APISQL (M4LDB_APISQL_SELECT);

			//Pedro: Nos ponemos a analizar la seguridad.
			o_X_APISQL.SetAPISQLSource(M4LDB_APISQL_FROM_SECURITY);	
			result = o_X_APISQL.AnalizeSQL (stFilterSecurity, pMtSecSent, M4_FALSE);
			//Pedro: Continuamos con el API.			
			o_X_APISQL.SetAPISQLSource(M4LDB_APISQL_FROM_APISQL);

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}
	}
	
	return (M4_SUCCESS);  
}


m4bool_t ClLConn::_X_CanAccessByField (m4id_t ai_idField, m4bool_t ai_bIsSelect)
{
	ClRegisterIndex indSecFld(0);

	indSecFld = FindIndexById (M4LDB_SYS_DICT_SEC_FIELDS, ai_idField);

	if (indSecFld == M4LDB_NULL_INDEX)
	{
		// No hemos encontrado el campo por lo que suponemos que no tiene seguridad definida
		// y que se puede acceder a el libremente.
		return (M4_TRUE);
	}

	// Situamos el current del nodo M4LDB_SYS_DICT_SEC_FIELDS en el registro actual.
	m_paRS[M4LDB_SYS_DICT_SEC_FIELDS]->Current[indSecFld];

	if (ai_bIsSelect)
	{
		return (M4LDB_BOOL (m_paRS[M4LDB_SYS_DICT_SEC_FIELDS]->Current.Item[M4LDB_ITEM_SYS_DICT_SEC_FIELDS_IS_READ].Value.GetWithOutSec ()));
	}

	return (M4LDB_BOOL (m_paRS[M4LDB_SYS_DICT_SEC_FIELDS]->Current.Item[M4LDB_ITEM_SYS_DICT_SEC_FIELDS_IS_WRITE].Value.GetWithOutSec ()));
}


m4return_t ClLConn::_X_FindFilterByIdObject (eStmType_t ai_eStmType, string& ao_stFilter, m4pcchar_t ai_pAlias)
{
	ClRegisterIndex indFilSec(0);
	m4bool_t bFirst = M4_TRUE;
	m4return_t result;
	m4char_t aAux[M4LDB_SIZE_DEFAULT_STRING];
	m4char_t aLgcTbl[M4LDB_SIZE_DEFAULT_STRING];
	m4char_t aAlias[M4LDB_SIZE_DEFAULT_STRING];
	m4char_t cSeparator;
	m4bool_t bIsWord;
	m4int32_t lenLgcTbl;
	m4int32_t lenAlias;

	m4id_t idLgcTbl = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());

	m4uint32_t numFilSec = m_paRS[M4LDB_SYS_DICT_SEC_LOBJ_FIL]->Count ();

	for (indFilSec=0; indFilSec<numFilSec; indFilSec++)
	{
		m_paRS[M4LDB_SYS_DICT_SEC_LOBJ_FIL]->Current[indFilSec];

		m4id_t idUse = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_SEC_LOBJ_FIL]->Current.Item[M4LDB_ITEM_SYS_DICT_SEC_LOBJ_FIL_ID_USE].Value.GetWithOutSec ());

		m4bool_t bAddFilter = M4_FALSE;

		switch (ai_eStmType)
		{
			case M4LDB_SELECT:
				if (*idUse == 'L' || *idUse == 'A')
				{
					bAddFilter = M4_TRUE;
				}
				break;

			case M4LDB_INSERT:
				if (*idUse == 'I' || *idUse == 'P' || *idUse == 'A')
				{
					bAddFilter = M4_TRUE;
				}
				break;

			case M4LDB_UPDATE:
				if (*idUse == 'U' || *idUse == 'P' || *idUse == 'A')
				{
					bAddFilter = M4_TRUE;
				}
				break;

			case M4LDB_DELETE:
				if (*idUse == 'D' || *idUse == 'P' || *idUse == 'A')
				{
					bAddFilter = M4_TRUE;
				}
				break;
		}

		if (bAddFilter)
		{
			m4int32_t posWhere = M4_STR_NPOS; 
			m4int32_t posBasis = M4_STR_NPOS;
			m4int32_t lenBasis = M4LDB_NULL_VALUE;
			m4pchar_t pBasis = NULL;

			if (bFirst && ai_pAlias)
			{
				bFirst = M4_FALSE;
				ao_stFilter.append (ai_pAlias);
				ao_stFilter.append (" ");
			}

			m4pchar_t pFilter = M4LDB_PSTR (m_paRS[M4LDB_SYS_DICT_SEC_LOBJ_FIL]->Current.Item[M4LDB_ITEM_SYS_DICT_SEC_LOBJ_FIL_FILTER].Value.GetWithOutSec ());

			ConvertToUpper (pFilter);
			
			string stFilter (pFilter);

			m4pchar_t pJoin = M4LDB_PSTR (m_paRS[M4LDB_SYS_DICT_SEC_LOBJ_FIL]->Current.Item[M4LDB_ITEM_SYS_DICT_SEC_LOBJ_FIL_JOIN].Value.GetWithOutSec ());

			ConvertToUpper (pJoin);

			//Pedro :Bug 0064098 -> DC; 0074680 -> UC
			//--------------------------------------------------------------------
			//Pedro: Bug 0064098
			//Modificado para evitar que se aplique el filtro de seguridad de tabla padre
			//en la tabla hija cuando se está haciendo un Delete Cascade.
			//--------------------------------------------------------------------

			//Caso 2.
			//Hay pJoin y estamos en DC --> No tenemos en cuenta filtros.
			//Hay pJoin y estamos en UC --> No tenemos en cuenta los filtros.
			//Por lo tanto, si hay pJoin y (estamos en DC o UC)
			//Bug 0074680. Update en cascada no aplicamos filtros heredados.
			if ( (*pJoin) && 
				( ((m_iStatus & M4LDB_CONEX_STATUS_DELETE_ON_CASCADE) != M4LDB_CONEX_STATUS_NONE)
				|| ((m_iStatus & M4LDB_CONEX_STATUS_UPDATE_ON_CASCADE) != M4LDB_CONEX_STATUS_NONE) )	
				)
			{
				continue;
			}

			//Línea sustituida por una que además de *pJoin, comprueba si vengo de un Delete Cascade.
			//Si hay pJoin y no estamos ni en DC ni en UC entraremos en el bloque.
			//Bug 0074680. Update en cascada no aplicamos filtros heredados.
			if ( (*pJoin) && 
				( ((m_iStatus & M4LDB_CONEX_STATUS_DELETE_ON_CASCADE) == M4LDB_CONEX_STATUS_NONE)
				&& ((m_iStatus & M4LDB_CONEX_STATUS_UPDATE_ON_CASCADE) == M4LDB_CONEX_STATUS_NONE) )
				)
			{
			//Fin Pedro
			//--------------------------------------------------------------------

				// Hay join. Esto significa que la tabla base del filtro no es la tabla base de la sentencia.
				// El join que tenemos es de la forma &O1 A1, O2 A2 WHERE A1.F1 = A2.F1.
				// Y el filtro es de la forma FROM &O2 A2 WHERE A2.F2 = 1.
				// Sustituyendo en el filtro la tabla base con lo que hay antes del WHERE en el join y
				// añadiendo el resto del join más un AND queda: 
				// FROM &O1 A1, O2 A2 WHERE A1.F1 = A2.F1 AND A2.F2 = 1.

				m4pchar_t pWhere = strstr (pJoin, " WHERE");

				if (!pWhere)
				{
					DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, "'WHERE'");
					return (M4_ERROR);
				}

				posBasis = stFilter.find ("&");

				if (posBasis == M4_STR_NPOS)
				{
					DUMP_LDBINSP_ERROR (this, M4_CH_LDB_MISSING_BASIS_LOGIC_OBJECT);
					return (M4_ERROR);
				}

				pBasis = pFilter + posBasis + 1;

				// Primero el objeto lógico en aLgcTbl.
				result = _X_GetToken (pBasis, aLgcTbl, lenLgcTbl, cSeparator, bIsWord, M4_FALSE);

				// Ahora tenemos el alias en aAlias.
				result = _X_GetToken (pBasis, aAlias, lenAlias, cSeparator, bIsWord, M4_FALSE);

				lenBasis = pBasis - pFilter - posBasis - 1;

				stFilter.replace (posBasis, lenBasis, pJoin, pWhere - pJoin);

				stFilter.append (" AND");

				stFilter.append (pWhere + 6);

				pFilter = (m4pchar_t) stFilter.c_str ();
			}

			if (ai_pAlias)
			{
				posBasis = stFilter.find ("&");

				if (posBasis == M4_STR_NPOS)
				{
					DUMP_LDBINSP_ERROR (this, M4_CH_LDB_MISSING_BASIS_LOGIC_OBJECT);
					return (M4_ERROR);
				}

				pBasis = pFilter + posBasis + 1;

				m4int32_t lNewAlias = strlen (ai_pAlias);

				// Primero el objeto lógico en aLgcTbl.
				result = _X_GetToken (pBasis, aLgcTbl, lenLgcTbl, cSeparator, bIsWord, M4_FALSE);

				// Ahora tenemos el alias en aAlias.
				result = _X_GetToken (pBasis, aAlias, lenAlias, cSeparator, bIsWord, M4_FALSE);

				sprintf (aAux, " %s%c", aAlias, cSeparator);

				// Buscamos la posición el alias obtenido anteriormente.
				m4int32_t posAlias = stFilter.find (aAux) + 1;

				sprintf (aAux, "%s.", aAlias);

				while (posAlias != M4_STR_NPOS)
				{
					// Reemplazamos el alias antiguo por el nuevo.
					stFilter.replace (posAlias, lenAlias, ai_pAlias);

					// Buscamos el siguiente alias.
					posAlias = stFilter.find (aAux, posAlias + lNewAlias);
				}
			}

			ao_stFilter.append (stFilter);

			ao_stFilter.append(";");
		}
	}

	return (M4_SUCCESS);
}



///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////


m4return_t ClLConn::_X_X_AnalizeSecurity (ClAPISQL *ai_pAPISQL, ClMetaSentence *&aio_pMtSent, ClGlobVars *ai_pGlobVars, ClAccessRecordSet *ai_pRecordSet, m4int32_t *ao_plSecMask, eStmType_t ai_eStmType, ClDinArr *ai_pParam)
{
	// Se analizan la seguridad a estos niveles :
	//    * Se analiza el acceso a los objetos logicos
	//    * Se crean los filtros de seguridad que se necesitan por cada objeto logico
	//    * Se analiza el acceso a los campos
	if (!strcmpi (ai_pGlobVars->GetIdRSM (), M4CL_SEC_SUPER_USER))
	{
		// SuperRol.
		if (ao_plSecMask)
		{
			*ao_plSecMask = M4LDB_SUPERROLE_SEC_MASK;
		}

		return (M4_SUCCESS);
	}

	m4return_t result = M4_SUCCESS;
	m4bool_t bBase = M4_FALSE;
	string stAux(M4LDB_EMPTY);
	ClRegisterIndex indRSM(M4LDB_NULL_INDEX);
	ClMetaSentence *pMtSent = aio_pMtSent;
	ClMetaSentence *pMtSecSent = aio_pMtSent;
	eStmType_t eStmType = (aio_pMtSent) ? aio_pMtSent->GetStmType () : ai_eStmType;
	VcPStSecurityFilter vcSecFil;

	m4int32_t iMask = 1 << (((m4int32_t) eStmType) - 1);

	// Recordar que la mascara se contruye:
	// (1*MASK_DEL)+(2*MASK_INS)+(4*MASK_UPD)+(8*MASK_SEL)+
	// (16*MASK_COR_DEL)+(32*MASK_COR_INS)+(64*MASK_COR_UPD)+
	// (128*MASK_DFS_CLS_DATE)+(256*MASK_HIST_ADAPT)+(512*MASK_DEL_CSC)+
	// (1024*MASK_UPD_PK_CSC)+(2048*MASK_DT_START_CSC)+(4096*MASK_DT_END_CSC)

	m4int32_t numObj = 1;

	if (pMtSent)
	{		
		if (eStmType != M4LDB_SELECT && !pMtSent->IsAPISQL ())
		{
			pMtSent = NULL;
		}
		else
		{
			numObj = pMtSent->GetNumObject ();
		}
	}
	
	for (m4int32_t indObj=0; indObj<numObj; indObj++)
	{
		m4bool_t bApplyFilter = M4_TRUE;

		ClMetaObject *pLgcTbl = NULL;

		// Para las siguientes tablas comprobamos que se puedan leer.
		eStmType_t eStmTypeAux = (indObj) ? M4LDB_SELECT : eStmType;

		// Si no entramos es porque ya estamos situados.
		if (pMtSent)
		{
			pLgcTbl = pMtSent->GetObjectByPos (indObj);

			if (pLgcTbl->m_bIsBasis)
			{
				if (bBase)
				{
					// La tabla base ya ha sido añadida por lo que no es necesario volver a añadir el filtro.
					bApplyFilter = M4_FALSE;
				}
				else
				{
					bBase = M4_TRUE;
				}
			}

			m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[(ClRegisterIndex) pLgcTbl->m_indLogicTable];
		}

		m4bool_t bHasSecurity = M4LDB_BOOL (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_HAS_SECURITY].Value.GetWithOutSec ());

		if (!bHasSecurity)
		{
			continue;
		}

		//Si estamos en persistencia no tendremos en cuenta las subselects.
		if (eStmType != M4LDB_SELECT && indObj > 0)
		{			
			m4id_t iLgcTbl = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());			
			DUMP_LDBINSP_WARNINGF (this, M4_CH_LDB_NOT_APPLY_SUBSELECT_SECURITY, iLgcTbl);
			continue;
		}

		if (!*ai_pGlobVars->GetIdRSM ())
		{
			m4id_t nLgcTbl = GetnLgcTbl (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Index);
			m4id_t iLgcTbl = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());

			// Aqui usamos el eStmType genérico.
			if (eStmType == M4LDB_SELECT)
			{
				if( M4ChLogIsErrorIn( M4_CH_LDB_NO_SECURITY_DEFINED ) == 0 )
				{
					DUMP_LDBINSP_WARNINGF (this, M4_CH_LDB_NO_SECURITY_DEFINED, LOG_CAT_AND (iLgcTbl, nLgcTbl) << "<null>");
				}
				return (M4_WARNING);
			}
			else
			{
				DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_NO_SECURITY_DEFINED, LOG_CAT_AND (iLgcTbl, nLgcTbl) << "<null>");
				return (M4_ERROR);
			}
		}

		indRSM = FindIndexById (M4LDB_SYS_DICT_SEC_LOBJ, ai_pGlobVars->GetIdRSM ());

		if (indRSM == M4LDB_NULL_INDEX)
		{
			m4id_t nLgcTbl = GetnLgcTbl (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Index);
			m4id_t iLgcTbl = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());

			// Aqui usamos el eStmType genérico.
			if (eStmType == M4LDB_SELECT)
			{
				if( M4ChLogIsErrorIn( M4_CH_LDB_NO_SECURITY_DEFINED ) == 0 )
				{
					DUMP_LDBINSP_WARNINGF (this, M4_CH_LDB_NO_SECURITY_DEFINED, LOG_CAT_AND (iLgcTbl, nLgcTbl) << ai_pGlobVars->GetIdRSM ());
				}
				return (M4_WARNING);
			}
			else
			{
				DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_NO_SECURITY_DEFINED, LOG_CAT_AND (iLgcTbl, nLgcTbl) << ai_pGlobVars->GetIdRSM ());
				return (M4_ERROR);
			}
		}

		// Situamos el current del nodo M4LDB_SYS_DICT_SEC_LOBJ en el registro actual.
		m_paRS[M4LDB_SYS_DICT_SEC_LOBJ]->Current[indRSM];

		m4int32_t iMaskObj = M4LDB_NUMBER (m_paRS[M4LDB_SYS_DICT_SEC_LOBJ]->Current.Item[M4LDB_ITEM_SYS_DICT_SEC_LOBJ_MASK].Value.GetWithOutSec ());

		if (!(iMaskObj & iMask))
		{
			m4id_t nLgcTbl = GetnLgcTbl (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Index);
			m4id_t iLgcTbl = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());

			switch (eStmTypeAux)
			{
				case M4LDB_SELECT:
					if( M4ChLogIsErrorIn( M4_CH_LDB_READ_ACCESS_REFUSE ) == 0 )
					{
						DUMP_LDBINSP_WARNINGF (this, M4_CH_LDB_READ_ACCESS_REFUSE, LOG_CAT_AND (iLgcTbl, nLgcTbl));
					}
					return (M4_WARNING);
					break;
				case M4LDB_INSERT:
					DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_INSERT_ACCESS_REFUSE, LOG_CAT_AND (iLgcTbl, nLgcTbl));
					break;
				case M4LDB_UPDATE:
					DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_UPDATE_ACCESS_REFUSE, LOG_CAT_AND (iLgcTbl, nLgcTbl));
					break;
				case M4LDB_DELETE:
					DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_DELETE_ACCESS_REFUSE, LOG_CAT_AND (iLgcTbl, nLgcTbl));
					break;
			}

			return (M4_ERROR);
		}

		result = LoadDictionaryChannelRSM ();
			
		if (result == M4_ERROR)
		{
			return (M4_ERROR);
		}

		if (eStmTypeAux != M4LDB_SELECT && ao_plSecMask)
		{
			*ao_plSecMask = iMaskObj;
		}

		if (bApplyFilter)
		{
			stAux = M4LDB_EMPTY;

			m4pcchar_t pAlias =	NULL;
			
			if (eStmType == M4LDB_SELECT)
			{
				pAlias = pMtSent->GetAliasByIdx (pLgcTbl->m_indAlias);
			}
			
			// Extraer todos los filtros de los objetos logicos
			result = _X_X_FindFilterByIdObject (vcSecFil, eStmTypeAux, stAux, pAlias);
			
			if (result == M4_ERROR)
			{
				for (itPStSecurityFilter it = vcSecFil.begin (); it!=vcSecFil.end (); it++)
				{
					delete (*it);
				}
				return (M4_ERROR);
			}
		}
		
		m4bool_t bHasSecurityFld = M4LDB_BOOL (m_paRS[M4LDB_SYS_DICT_SEC_LOBJ]->Current.Item[M4LDB_ITEM_SYS_DICT_SEC_LOBJ_HAS_SECURITY_FLD].Value.GetWithOutSec ());

		if (!bHasSecurityFld)
		{
			continue;
		}

		if (pMtSent)
		{
			m4int32_t numFld = pMtSent->GetNumFields ();

			for (m4int32_t indFld=numFld-1; indFld>=0; indFld--)
			{
				ClMetaField *pFld = pMtSent->GetFieldByPos (indFld);

				if (pFld->IsMetaData ()==M4_TRUE)
				{
					if ((pFld->m_eClause == M4LDB_CLAUSE_COLUMN) || 
						(pFld->m_eClause == M4LDB_CLAUSE_LONG_COLUMN))
					{
						// Aqui usamos el eStmType genérico.
						if (eStmType == M4LDB_SELECT)
						{
							if (pLgcTbl->m_indAlias == pFld->m_indAlias)
							{
								m4bool_t bCanReadField = _X_CanAccessByField (pFld->m_stField.c_str (), M4_TRUE);

								m4bool_t bCanReadItem = pMtSent->CanReadByPos (pFld->m_iColumn);

								if (!bCanReadField)
								{
									if (bCanReadItem)
									{
										// Warning funcional.
										m4id_t nLgcTbl = GetnLgcTbl (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Index);
										m4id_t iLgcTbl = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
										
										DUMP_LDBINSP_WARNINGF (this, M4_CH_TF_READ_ACCESS_FIELD_REFUSE, pFld->m_stField.c_str () <<	LOG_CAT_AND (iLgcTbl, nLgcTbl));
									}

									pMtSent->InvalidateColumn (pFld->m_iColumn);
								}
								else
								{
									if (!bCanReadItem)
									{
										// Warning tecnológico.
										m4id_t nLgcTbl = GetnLgcTbl (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Index);
										m4id_t iLgcTbl = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
										
										DUMP_LDBINSP_WARNINGF (this, M4_CH_LDB_READ_ACCESS_FIELD_REFUSE, pFld->m_stField.c_str () << LOG_CAT_AND (iLgcTbl, nLgcTbl));

										pMtSent->InvalidateColumn (pFld->m_iColumn);
									}
								}
							}
						}
						else
						{
							// Nada que validar.
						}
					}
					else
					{
						if (pLgcTbl->m_indAlias == pFld->m_indAlias)
						{
							// Para el WHERE y demás siempre de lectura.
							m4bool_t bCanReadField = _X_CanAccessByField (pFld->m_stField.c_str (), M4_TRUE);

							if (!bCanReadField)
							{
								for (itPStSecurityFilter it = vcSecFil.begin (); it!=vcSecFil.end (); it++)
								{
									delete (*it);
								}

								m4id_t nLgcTbl = GetnLgcTbl (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Index);
								m4id_t iLgcTbl = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
								
								// Aqui usamos el eStmType genérico.
								if (eStmType == M4LDB_SELECT)
								{
									if( M4ChLogIsErrorIn( M4_CH_LDB_READ_ACCESS_FIELD_REFUSE ) == 0 )
									{
										DUMP_LDBINSP_WARNINGF (this, M4_CH_LDB_READ_ACCESS_FIELD_REFUSE, pFld->m_stField.c_str () << LOG_CAT_AND (iLgcTbl, nLgcTbl));
									}
									return (M4_WARNING);
								}

								DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_READ_ACCESS_FIELD_REFUSE, 
									pFld->m_stField.c_str ()  <<
									LOG_CAT_AND (iLgcTbl, nLgcTbl)
									);
								return (M4_ERROR);
							}
						}
					}
				}
			}
		}

		if (eStmTypeAux == M4LDB_INSERT || eStmTypeAux == M4LDB_UPDATE)
		{
			m4int32_t numParam = ai_pParam->GetNumData ();

			m4int32_t indParam;

			for (indParam=0; indParam<numParam; indParam++)
			{
				m4int32_t hItem = ai_pParam->GetItemHandle (indParam);

				if (hItem)
				{
					m4id_t idField = ai_pParam->GetInfoIdField (indParam);

					m4bool_t bCanWriteField = _X_CanAccessByField (idField, M4_FALSE);
					m4bool_t bCanReadField = _X_CanAccessByField (idField, M4_TRUE);

					if (!bCanWriteField)
					{
						ai_pParam->SetInfoHasSecurity (indParam);
					}
					if (!bCanReadField)
					{
						ai_pParam->SetInfoHasSecurityReadInPersist (indParam);
					}
				}
			}
		}
	}
	
	if (!vcSecFil.empty ())
	{
		string stFilterSecurity (M4LDB_EMPTY);

		if (eStmType != M4LDB_SELECT)
		{
			// Cambiamos la metasentencia para escritura.
			pMtSecSent = new ClMetaSentence;

			pMtSecSent->Create (ai_pGlobVars, M4LDB_SELECT, M4_TRUE, M4_FALSE, M4_FALSE, M4_FALSE, M4_TRUE);

			pMtSecSent->SetParam (ai_pParam);

			stFilterSecurity.append ("SELECT 1 ");
		}

		for (itPStSecurityFilter it = vcSecFil.begin (); it!=vcSecFil.end (); it++)
		{
			// Aqui usamos el eStmType genérico.
			if (eStmType == M4LDB_SELECT)
			{
				m4char_t aAux[30];
				sprintf (aAux, "^%ld ", (*it)->m_lLevel);			
				stFilterSecurity.append (aAux);
			}

			m4int32_t lNextFilter = stFilterSecurity.size ();
				
			stFilterSecurity.append ((*it)->m_pFilter);

			if (!(*it)->m_sJoin.empty ())
			{
				m4int32_t posWhere = lNextFilter;
				
				// El Where puede venir como \nWHERE o \tWHERE, no vale con buscar ' WHERE'.
				// Buscamos Where y nos aseguramos que el caracter anterior es separador.
				
				posWhere = stFilterSecurity.find ("WHERE", posWhere);
				
				while (posWhere!=M4_STR_NPOS)
				{
					m4char_t cBeforeWhere = stFilterSecurity[posWhere-1];

					if ((cBeforeWhere  == 0x20) ||	//SPACE
					((cBeforeWhere  >= 0x09) &&  
					(cBeforeWhere  <= 0x0D)))
					{  //HT, LF, VT, FF, CR				}
						// OK. Es un separador.
						break;
					}
					posWhere = stFilterSecurity.find ("WHERE", posWhere+1);
				}


				if (posWhere == M4_STR_NPOS)
				{
					for (itPStSecurityFilter it = vcSecFil.begin (); it!=vcSecFil.end (); it++)
					{
						delete (*it);
					}
					DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, "'WHERE'");
					return (M4_ERROR);
				}


				stFilterSecurity.replace (posWhere-1, 6, (*it)->m_sJoin);
			}

			stFilterSecurity.append (";");

			if (eStmType == M4LDB_SELECT && (*it)->m_pAlias)
			{
				result = _X_ReplaceAlias (stFilterSecurity, lNextFilter, (*it)->m_pAlias);

				if (result == M4_ERROR)
				{
					for (itPStSecurityFilter it = vcSecFil.begin (); it!=vcSecFil.end (); it++)
					{
						delete (*it);
					}
					return (M4_ERROR);
				}
			}

			if (!(*it)->m_sJoin.empty ())
			{
				m4int32_t posBasis = stFilterSecurity.find ("&", lNextFilter);

				if (posBasis == M4_STR_NPOS)
				{
					DUMP_LDBINSP_ERROR (this, M4_CH_LDB_MISSING_BASIS_LOGIC_OBJECT);
					return (M4_ERROR);
				}

				stFilterSecurity.at (posBasis) = ' ';
			}
		}

		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (this, "\n\n APISQL Statement in Security Filter = %s.\n", stFilterSecurity.c_str ());
			M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_LOGIC_SENTENCES, this, "\n\n APISQL Statement in Security Filter = %s.\n", stFilterSecurity.c_str ());
		#endif

		if (ai_pAPISQL)
		{
			//Pedro: Nos ponemos a analizar la seguridad.
			ai_pAPISQL->SetAPISQLSource(M4LDB_APISQL_FROM_SECURITY);
			result = ai_pAPISQL->AnalizeSQL (stFilterSecurity, pMtSecSent, M4_TRUE);
			//Pedro: Continuamos con el API.
			ai_pAPISQL->SetAPISQLSource(M4LDB_APISQL_FROM_APISQL);			

		}
		else
		{
			aio_pMtSent = pMtSecSent;

			ClAPISQL o_X_APISQL (M4LDB_APISQL_SELECT);

			//Pedro: Nos ponemos a analizar la seguridad.
			o_X_APISQL.SetAPISQLSource(M4LDB_APISQL_FROM_SECURITY);
			result = o_X_APISQL.AnalizeSQL (stFilterSecurity, pMtSecSent, M4_FALSE);
			//Pedro: Continuamos con el API.			
			o_X_APISQL.SetAPISQLSource(M4LDB_APISQL_FROM_APISQL);			

		}
	}
	
	for (itPStSecurityFilter it = vcSecFil.begin (); it!=vcSecFil.end (); it++)
	{
		delete (*it);
	}

	return (result);  
}


m4return_t ClLConn::_X_X_FindFilterByIdObject (VcPStSecurityFilter &ai_vcSecFil, eStmType_t ai_eStmType, string& ao_stFilter, m4pcchar_t ai_pAlias)
{
	// El join que tenemos es de la forma &O1 A1, O2 A2 WHERE A1.F1 = A2.F1.
	// Y el filtro es de la forma FROM &O2 A2 WHERE A2.F2 = 1.
	// Sustituyendo en el filtro la tabla base con lo que hay antes del WHERE en el join y
	// añadiendo el resto del join más un AND queda: 
	// FROM &O1 A1, O2 A2 WHERE A1.F1 = A2.F1 AND A2.F2 = 1.

	ClRegisterIndex indFilSec(0);
	m4return_t result;

	m4id_t idLgcTbl = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());

	m4uint32_t numFilSec = m_paRS[M4LDB_SYS_DICT_SEC_LOBJ_FIL]->Count ();

	for (indFilSec=0; indFilSec<numFilSec; indFilSec++)
	{
		m_paRS[M4LDB_SYS_DICT_SEC_LOBJ_FIL]->Current[indFilSec];

		m4id_t idUse = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_SEC_LOBJ_FIL]->Current.Item[M4LDB_ITEM_SYS_DICT_SEC_LOBJ_FIL_ID_USE].Value.GetWithOutSec ());

		m4bool_t bAddFilter = M4_FALSE;

		switch (ai_eStmType)
		{
			case M4LDB_SELECT:
				if (*idUse == 'L' || *idUse == 'A')
				{
					bAddFilter = M4_TRUE;
				}
				break;

			case M4LDB_INSERT:
				if (*idUse == 'I' || *idUse == 'P' || *idUse == 'A')
				{
					bAddFilter = M4_TRUE;
				}
				break;

			case M4LDB_UPDATE:
				if (*idUse == 'U' || *idUse == 'P' || *idUse == 'A')
				{
					bAddFilter = M4_TRUE;
				}
				break;

			case M4LDB_DELETE:
				if (*idUse == 'D' || *idUse == 'P' || *idUse == 'A')
				{
					bAddFilter = M4_TRUE;
				}
				break;
		}

		if (bAddFilter)
		{
			m4int32_t lLevel = (ai_pAlias) ? atol (&ai_pAlias[1]) : ai_vcSecFil.size ();

			m4id_t idFilter = M4LDB_ID (m_paRS[M4LDB_SYS_DICT_SEC_LOBJ_FIL]->Current.Item[M4LDB_ITEM_SYS_DICT_SEC_LOBJ_FIL_ID_FILTER].Value.GetWithOutSec ());

			m4pchar_t pFilter = M4LDB_PSTR (m_paRS[M4LDB_SYS_DICT_SEC_LOBJ_FIL]->Current.Item[M4LDB_ITEM_SYS_DICT_SEC_LOBJ_FIL_FILTER].Value.GetWithOutSec ());

			ConvertToUpper (pFilter);

			if (!*idFilter)
			{
				idFilter = pFilter;
			}
			
			m4pchar_t pJoin = M4LDB_PSTR (m_paRS[M4LDB_SYS_DICT_SEC_LOBJ_FIL]->Current.Item[M4LDB_ITEM_SYS_DICT_SEC_LOBJ_FIL_JOIN].Value.GetWithOutSec ());

			ConvertToUpper (pJoin);

			//Pedro :Bug 0064098 -> DC; 0074680 -> UC
			//--------------------------------------------------------------------
			//Pedro: Bug 0064098
			//Modificado para evitar que se aplique el filtro de seguridad de tabla padre
			//en la tabla hija cuando se está haciendo un Delete Cascade.						
			//--------------------------------------------------------------------			
			//Caso 1:
			//Si hay filtro join y NO estamos en DC --> sí construiremos el filtro.
			//Es decir, tendremos en cuenta la columna que nos indica el filtro
			//heredado a aplicar == pFilter.
			//Caso 2:
			//Si hay pJoin y estamos en DC --> NO debemos aplicar filtros. Buscaremos en
			//el siguiente filtro (for)
			//Caso 3:
			//Si NO hay pJoin --> Tambien necesito aplicar el filtro.
			//Bug 0074680. Update en cascada no aplicamos filtros heredados.

			//Caso 2
			if ( (*pJoin) && 
				( ((m_iStatus & M4LDB_CONEX_STATUS_DELETE_ON_CASCADE) != M4LDB_CONEX_STATUS_NONE)
				|| ((m_iStatus & M4LDB_CONEX_STATUS_UPDATE_ON_CASCADE) != M4LDB_CONEX_STATUS_NONE) )
				)
			{
				continue;
			}

			//Si no estamos ni en DC ni en UP entonces podemos aplicar filtro.			
			//Caso 1
			if ( (*pJoin) && 						
				( ((m_iStatus & M4LDB_CONEX_STATUS_DELETE_ON_CASCADE) == M4LDB_CONEX_STATUS_NONE)
				&& ((m_iStatus & M4LDB_CONEX_STATUS_UPDATE_ON_CASCADE) == M4LDB_CONEX_STATUS_NONE) )
				)
			{
			//Fin Pedro: Bug 0064098
					
				StSecurityFilter *poSF = NULL;
	
				for (itPStSecurityFilter it = ai_vcSecFil.begin (); it!=ai_vcSecFil.end (); it++)
				{
					if(
						!strcmp( (*it)->m_idFilter, idFilter)
						&& 
						(*it)->m_lLevel == lLevel
						)
					{
						poSF = (*it);
						break;
					}
				}

				if (!poSF)
				{
					poSF = new struct StSecurityFilter (idFilter, lLevel, pFilter, NULL);
					ai_vcSecFil.push_back (poSF);
				}

				//Pedro: Bug 0064098			
				//Línea sustituida por una que además de *pJoin, comprueba si vengo de un Delete Cascade.
				//pero se hace más arriba
				//1 --- if (*pJoin) {
				//if ((*pJoin) && (m_iStatus != M4LDB_CONEX_STATUS_DELETE_ON_CASCADE)){
				//Fin Pedro
				string stJoin;

				result = _X_ComposeJoin (stJoin, pFilter, pJoin);

				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}

				if (ai_pAlias)
				{
					result = _X_ReplaceAlias (stJoin, 0, ai_pAlias);

					if (result == M4_ERROR)
					{
						return (M4_ERROR);
					}
				}

				m4int32_t posWhere = stJoin.find (" WHERE");

				if (posWhere == M4_STR_NPOS)
				{
					DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, "'WHERE'");
					return (M4_ERROR);
				}

				if (poSF->m_sJoin.empty ())
				{
					poSF->m_sJoin = stJoin;
				}
				else
				{
					m4int32_t posBasis = stJoin.find ("&");

					if (posBasis == M4_STR_NPOS)
					{
						DUMP_LDBINSP_ERROR (this, M4_CH_LDB_MISSING_BASIS_LOGIC_OBJECT);
						return (M4_ERROR);
					}

					stJoin.at (posBasis) = ' ';

					posWhere = poSF->m_sJoin.find (" WHERE");

					if (posWhere == M4_STR_NPOS)
					{
						DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, "'WHERE'");
						return (M4_ERROR);
					}

					poSF->m_sJoin.replace (posWhere, 6, stJoin);
				}
			}

//Antes de Bug 0064098 no estaban comentadas estas 3 lineas
//1 ---			else {
//2 ---				poSF->m_pAlias = ai_pAlias;
//3 ---			}
//Fin del comentario
			//Continuación de Bug 0064098
			//Esta parte se ha añadido para que se aplique el filtro sólo si NO tenemos
			//campos de join. Si No hay campos de join significa que el filtro NO es heredado
			//por lo tanto habrá que aplicarlo. Creamos la estructura si es necesario.
			//Caso 3
			if (!(*pJoin))
			{	
				StSecurityFilter *poSF = NULL;

				for (itPStSecurityFilter it = ai_vcSecFil.begin (); it!=ai_vcSecFil.end (); it++)
				{
					if (
						!strcmp ((*it)->m_idFilter, idFilter)
						&& 
						(*it)->m_lLevel == lLevel
						&&
						/* Bug 0171934
						Si el alias no es el mismo hay que añadirlo también
						*/
						(*it)->m_pAlias != NULL
						&&
						!strcmp ((*it)->m_pAlias, ai_pAlias)
						)
					{
						poSF = (*it);
						break;
					}
				}

				if (!poSF)
				{
					poSF = new struct StSecurityFilter (idFilter, lLevel, pFilter, ai_pAlias);
					ai_vcSecFil.push_back (poSF);
				}
			}
//Fin Pedro Bug 0064098
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClLConn::_X_ComposeJoin (string &ai_stJoin, m4pchar_t ai_pFilter, m4pchar_t ai_pJoin)
{
	m4return_t result;
	m4char_t aAux[M4LDB_SIZE_DEFAULT_STRING];
	m4char_t aLgcTbl[M4LDB_SIZE_DEFAULT_STRING];
	m4char_t aAlias[M4LDB_SIZE_DEFAULT_STRING];
	m4char_t cSeparator;
	m4bool_t bIsWord;
	m4int32_t lenAux;
	m4int32_t lenLgcTbl;
	m4int32_t lenAlias;
	m4bool_t bExit = M4_FALSE;

	m4pchar_t pBasis = strchr (ai_pFilter, '&');

	if (!pBasis)
	{
		DUMP_LDBINSP_ERROR (this, M4_CH_LDB_MISSING_BASIS_LOGIC_OBJECT);
		return (M4_ERROR);
	}

	pBasis++;

	// Primero el objeto lógico en aLgcTbl.
	result = _X_GetToken (pBasis, aLgcTbl, lenLgcTbl, cSeparator, bIsWord, M4_FALSE);

	// Ahora tenemos el alias en aAlias.
	result = _X_GetToken (pBasis, aAlias, lenAlias, cSeparator, bIsWord, M4_FALSE);

	m4pchar_t pCurrent = ai_pJoin;

	while (!bExit)
	{
		m4bool_t bEqual = M4_FALSE;

		result = _X_GetToken (pCurrent, aAux, lenAux, cSeparator, bIsWord, M4_FALSE);
		
		if (cSeparator == '\0')
		{
			DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, "WHERE");
			return (M4_ERROR);
		}

		if (strcmp (aAux, aLgcTbl))
		{
			ai_stJoin.append (", ");
			ai_stJoin.append (aAux);
		}
		else
		{
			bEqual = M4_TRUE;
		}

		result = _X_GetToken (pCurrent, aAux, lenAux, cSeparator, bIsWord, M4_FALSE);
		
		if (cSeparator == '\0')
		{
			DUMP_LDBINSP_ERRORF (this, M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, "WHERE");
			return (M4_ERROR);
		}

		if (strcmp (aAux, aAlias))
		{
			if (bEqual)
			{
				ai_stJoin.append (", ");
				ai_stJoin.append (aLgcTbl);
			}
			ai_stJoin.append (" ");
			ai_stJoin.append (aAux);
		}

		pCurrent = M4LDB_PSTR (pCurrent);

		//Bug 0087773
		//Vamos a marcar de alguna forma la parte de xjoin!!. Delimitamos el xjoin con asteriscos!.		
		if (!strncmp (pCurrent, "WHERE", 5))
		{
			ai_stJoin.append (" ");			
			ai_stJoin.append ("WHERE");
			ai_stJoin.append (" ¿ ");
			ai_stJoin.append (pCurrent + 5);
			ai_stJoin.append (" ¿ ");
			ai_stJoin.append (" AND");
			bExit = M4_TRUE;
		}
	}

	return (M4_SUCCESS);

}


m4return_t ClLConn::_X_ReplaceAlias (string &ai_stFilter, m4int32_t ai_lOffset, m4pcchar_t ai_pAlias)
{
	m4return_t result;
	m4char_t aAlias[M4LDB_SIZE_DEFAULT_STRING];
	m4char_t cSeparator;
	m4bool_t bIsWord;
	m4int32_t lenAux;
	m4int32_t lenAlias = 0;
	
	m4bool_t bExit = M4_FALSE;

	string stAux (ai_stFilter.c_str (), ai_lOffset);

	m4pchar_t pAux = new m4char_t [ai_stFilter.size () + 1 - ai_lOffset];

	m4pchar_t pCurrent = (m4pchar_t) ai_stFilter.c_str () + ai_lOffset;

	while (!bExit)
	{
		result = _X_GetToken (pCurrent, pAux, lenAux, cSeparator, bIsWord, M4_FALSE);

		if (result != M4_ERROR)
		{

			if (!lenAlias && pAux[0] == '&')
			{
				stAux.append (pAux);
				stAux.append (&cSeparator, 0, 1);
				result = _X_GetToken (pCurrent, aAlias, lenAlias, cSeparator, bIsWord, M4_FALSE);
				stAux.append (ai_pAlias);
			}
			else
			{
				if (lenAux > lenAlias && 
					pAux[lenAlias] == '.' && 
					!strncmp (pAux, aAlias, lenAlias))
				{
					stAux.append (ai_pAlias);
					stAux.append (&pAux[lenAlias]);
				}
				else
				{
					stAux.append (pAux);
				}
			}
		}

		if (cSeparator == '\0')
		{
			bExit = M4_TRUE;
		}
		else
		{
			stAux.append (&cSeparator, 0, 1);
		}
	}

	ai_stFilter = stAux;

	delete pAux;

	return (M4_SUCCESS);
}




