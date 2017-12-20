//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb
// File:                clpersis.cpp
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

#include "defmain.hpp"

#include "clldb.hpp"
#include "clload.hpp"
#include "cltblmem.hpp"
#include "clpersis.hpp"
#include "clmtsent.hpp"
#include "cldinarr.hpp"
#include "ldb_log.hpp"


ClPersist::ClPersist (ClGlobVars *ai_pGlobVars, m4bool_t ai_bIsInternal, m4bool_t ai_bUseTramitation, m4bool_t ai_bUseInheritance, m4bool_t ai_bUseTranslation)

 : ClStmt (ai_pGlobVars, ai_bIsInternal)
{
	m_pvcPersist = NULL;

	m_pInherit = NULL;

	//Es un puntero a la persist donde está la tabla base. Execute2 es recursivo y se recorre persist hasta
	//que se encuentra la hoja que es la tabla principal.
	m_pMainLeaf = NULL;
	m_bIsRootInRecursion = M4_FALSE;
	m_bChangesIntoMainLeaf = M4_FALSE;
	m_bExecuteChangesIntoInheritance = M4_FALSE;
	m_bForcePersistInUpdate = M4_FALSE;
	m_bUseTramitation = ai_bUseTramitation;
	m_bUseInheritance = ai_bUseInheritance;
	m_bUseTranslation = ai_bUseTranslation;
}

	
ClPersist::~ClPersist (void)
{
	Destroy ();
}

void ClPersist::SetDataValueInRecord (PClRegister ai_pReg, m4bool_t ai_bShowInfo)
{

	m4int32_t	indParam;
	m4int32_t	numParam;
	PClRegister	pReg = ai_pReg;

	m4double_t dValue = 0;
	ClHandle hItem(0);

	//Obtenemos el numero de parametros analogia con columnas.
	numParam = m_pParam->GetNumData ();

	// Limpiamos los parámetros.
	m_pParam->Clear ();

	for (indParam=0; indParam<numParam; indParam++) {

		m4bool_t bIsPK = m_pParam->GetInfoIsPK (indParam);

		m4bool_t bIsReadOnly = m_pParam->GetInfoIsReadOnly (indParam);

		eFldType_t eFldType = m_pParam->GetInfoIdInternalField (indParam);

		//Para depuración.		
		m4id_t iLgcTbl = m_oMtLgcTbl.m_idLgcTbl;
		m4id_t iField = m_pParam->GetInfoIdField (indParam);
		//Para depuración.

		hItem = m_pParam->GetItemHandle (indParam);

		//Este bloque es importante para tener m_pParams correcto!!.
		//+-+-+-+-+-+-+-+-+-+-+-+-+-
		if ( (hItem) && 
				( ((m_eStmType == M4LDB_DELETE) && (bIsPK)) || 	((m_eStmType != M4LDB_DELETE) && (!bIsReadOnly)) )	) {

			void *pValue = pReg->Item[hItem].Value.GetWithOutSec ();
			
			void *pOldValue = NULL;

			m4int32_t lLenValue = pReg->Item[hItem].Value.Size ();

			m4int32_t lLenOldValue = 0;

			if ((bIsPK) && 
				(m_pParam->GetHasOldValue (indParam))) {
				// Tenemos una PK modificable.
				pOldValue = pReg->Item[hItem].OldValue.GetWithOutSec ();

				if (!pOldValue) {
					// Como una PK no puede ser nula le damos el valor nuevo.
					pOldValue = pValue;
					lLenOldValue = lLenValue;
				}
				else {
					lLenOldValue = pReg->Item[hItem].OldValue.Size ();
				}
			}

			//---------------------------
			if (pValue) {
				m4int8_t m4Type = m_pParam->GetInfoM4Type (indParam);

				switch (m4Type) {
					default:
						m_pParam->SetData (indParam, (m4pcchar_t) pValue, lLenValue);
						if (pOldValue) {
							m_pParam->SetOldData (indParam, (m4pcchar_t) pOldValue, lLenOldValue);
						}
						break;

					case M4CL_M4_TYPE_LONG :
					case M4CL_M4_TYPE_UNI_LONG :
					case M4CL_M4_TYPE_BINARY_STRING :
					{
						if (lLenValue > 0 && m4pcchar_t(pValue)[lLenValue-1] == '\0') {
							m_pParam->SetData (indParam, (m4pcchar_t) pValue, lLenValue-1);
						}
						// Esto no debería suceder, pero por si las moscas conservo
						// comportamiento anterior
						else {
							m_pParam->SetData (indParam, (m4pcchar_t) pValue, lLenValue);
						}
						break;
					}
					case M4CL_M4_TYPE_BLOB :
						{
							// Evaluamos si tenemos que persistirlo
							m4bool_t bDoPersist = M4_TRUE;
							ClHandle oHandle(pReg->Item[hItem].ItemDef.GetHandle());
							ClCompiledMCR *pCMCR = pReg->Item[hItem].ItemDef.GetpCMCR();
							ClHandle oAuxHandle(pCMCR->GetItemAuxItem( oHandle ) );

							if (oAuxHandle != 0)
							{
								m4return_t iResult = M4_SUCCESS;
								m4VariantType   vValue ;
								
								// Comprueba el tipo
								m4uint8_t iType = pCMCR->GetItemType( oAuxHandle );
								if (iType == M4CL_ITEM_TYPE_METHOD || iType == M4CL_ITEM_TYPE_CONCEPT)
								{
									// procesa los métodos
									ClItem &ItemToCall = pReg->Item[ oAuxHandle ];
									iResult = ItemToCall.Call() ;
									if (iResult == M4_SUCCESS)
									{
										iResult = ItemToCall.StackPop( vValue ) ;
										if (vValue.Type == M4CL_CPP_TYPE_NUMBER && vValue.Data.DoubleData)
											bDoPersist = M4_FALSE;
									}
								}
								else
								{
									// procesa los valores
									if (pReg->Item[hItem].ItemDef.CppType() == M4CL_CPP_TYPE_NUMBER)
									{
										pReg->Item[hItem].Value.GetWithOutSec(vValue);
										if (vValue.Data.DoubleData)
											bDoPersist = M4_FALSE;
									}
								}
							}

							if (bDoPersist)
							{
								// Formato: filename\0HEADER+descriptor\0extensión\0
								void *pValueDescr = pReg->Item[hItem].Value.GetDescr();
								if (pValueDescr == NULL)
									pValueDescr = "";

								m4char_t pTempBlob[M4LDB_SIZE_BLOB + 1];

								strcpy(pTempBlob,(m4pcchar_t) pValue);
								m4int32_t iSize = strlen(pTempBlob) + 1;

								sprintf(pTempBlob+iSize,"%s%s",M4LDB_BLOB_DESCR_PREFIX,pValueDescr);
								iSize += strlen(pTempBlob+iSize) + 1;

								m4pchar_t pExt = (m4pchar_t) pValue + strlen((m4pchar_t) pValue);
								while ((pExt != (m4pchar_t)pValue) && (*pExt != '.'))
									pExt--;
								if (pExt != (m4pchar_t)pValue)
									pExt++;
								else
									pExt = "";

								strcpy(pTempBlob+iSize,pExt);
								iSize += strlen(pTempBlob+iSize) + 1;
								
								m_pParam->SetData (indParam, (m4pcchar_t) pTempBlob, iSize+strlen((m4pcchar_t) pValueDescr));
							}
						}
						break;

					case M4CL_M4_TYPE_DATE:
						dValue = (m4date_t) M4LDB_NUMBER (pValue);
						m_pParam->SetData (indParam, (m4pchar_t) &dValue);
						if (pOldValue) {
							m4date_t dOldValue = M4LDB_DOUBLE (pOldValue);
							m_pParam->SetOldData (indParam, (m4pchar_t) &dOldValue);
						}
						break;

					case M4CL_M4_TYPE_TIMESTAMP:
					case M4CL_M4_TYPE_NUMBER:
						dValue = M4LDB_DOUBLE (pValue);
						m_pParam->SetData (indParam, (m4pchar_t) &dValue);
						if (pOldValue) {
							m4date_t dOldValue = M4LDB_DOUBLE (pOldValue);
							m_pParam->SetOldData (indParam, (m4pchar_t) &dOldValue);
						}
						break;
				}
			}

			else {
				m_pParam->SetNull(indParam);
			} // if (pValue) {
			//---------------------------

			if (ai_bShowInfo == M4_TRUE) {
				#ifdef _M4LDB_DEBUG
					M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\tData[%ld] Itm[%ld] = ", indParam + 1, (m4uint32_t)hItem);
					DisplayData (indParam);
				#endif
			}

		} // if ( (hItem) && ( ((m_eStmType == M4LDB_DELETE) && (bIsPK)) || 	((m_eStmType != M4LDB_DELETE) && (!bIsReadOnly)) )	) {
		//+-+-+-+-+-+-+-+-+-+-+-+-+-

	} // for (indParam=0; indParam<numParam; indParam++) {
}

void ClPersist::InitStt (void)
{
	ClStmt *pAuxStm = 0;
	ClRegisterIndex indRealTbl(0);
	m4uint32_t numRealTbl = m_pvcPersist->size ();

	for (indRealTbl=0; indRealTbl<numRealTbl; indRealTbl++)	
	{
		pAuxStm = ((ClStmt *)(*m_pvcPersist)[indRealTbl]);
		pAuxStm->InitStt();
	}

	ClStmt::InitStt();
}
	
m4return_t ClPersist::CreateParam (void)
{
	m4return_t result;
	ClRegisterIndex indFld(0);
	ClRegisterIndex indRealTbl(0);
	PClRegister pReg;
	ClCompiledMCR *pMCR;
	m4id_t idDefFld = NULL;
	m4id_t idDefLDBField = NULL;
	m4int32_t indPar = 0;
	m4pcchar_t pccIdType = NULL;
	PMpIdData pmpIdFld = NULL;


	m4uint32_t numRealTbl = m_pvcPersist->size ();

	if (!m_oMtLgcTbl.m_bAudStmt) {

		m4int32_t numParam = 0;

		// Si estamos en corrección añadimos uno para el parámetro auxiliar de fecha de fin del registro anterior.
		numParam += (m_oMtLgcTbl.m_bCorrected) ? 1 : 0;

		// Seis parámetros mas de auditoría.
		//Bug 0084837 Metemos dos más.
		numParam += (m_oMtLgcTbl.m_bHasAudit) ? 8 : 0;

		for (indRealTbl=0; indRealTbl<numRealTbl; indRealTbl++)	{

			// Situamos el current del nodo SYS_DICT_REAL_OBJECTS en dicho registro.
			m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[indRealTbl];

			// Obtenemos el número de registros (fields) que tiene el nodo SYS_DICT_FIELDS.
			numParam += m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Count ();
		}

		if( m_bUseTramitation == M4_FALSE && m_oMtLgcTbl.m_iMirrorType == 1 )
		{
			// Uno menos en tramitación (sin activar) por cada tabla por el ID_DATASET
			numParam -= numRealTbl;
		}

		// Uno menos en herencia de datos por cada tabla por el ID_ORG_LEVEL
		numParam -= (m_oMtLgcTbl.m_bInheritsData) ? numRealTbl : 0;

		m_pParam->Init (numParam);
	}
	else {
		indPar = m_pParam->GetNumData ();
	}

	for (indRealTbl=0; indRealTbl<numRealTbl; indRealTbl++)	{

		// Situamos el current del nodo SYS_DICT_REAL_OBJECTS en dicho registro.
		m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[indRealTbl];

		// Obtenemos el puntero al objeto register.
		pReg = &m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register;

		m4uint32_t numFld = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Count ();

		for (indFld=0; indFld<numFld; indFld++) {

			// Situamos el objeto register.
			m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indFld];

			eFldType_t eIntFld = (eFldType_t) M4LDB_NUMBER (pReg->Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_INTERNAL_FIELD].Value.GetWithOutSec ());

			// Si es el ID_DATASET no se persiste sin tramitación porque es virtual
			if( eIntFld == M4CL_INTERNAL_TYPE_ID_DATASET && m_bUseTramitation == M4_FALSE && m_oMtLgcTbl.m_iMirrorType == 1 )
			{
				continue;
			}

			// Si es el ID_ORG_LEVEL no se persiste porque es virtual
			if( eIntFld == M4CL_INTERNAL_TYPE_ID_ORGANIZATION_LEVEL )
			{
				continue;
			}

			m4id_t idField = M4LDB_ID (pReg->Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_FIELD].Value.GetWithOutSec ());

			pccIdType = (m4pcchar_t) pReg->Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_TYPE].Value.GetWithOutSec ();

			m4int32_t indAuxPar = m_pParam->FindIndexByIdField (idField);

			//Bug 0084837
			//Si estamos en auditoría y el tipo de auditoría es de DELETE tendremos que forzar el bindado
			//tanto del role como de usuario.
			//Si no se binda, se guardará el valor que traiga la select de validación de PK, es decir, en la auditoría
			//queda grabado el usuario y role que hizo la última modificación, en vez de quedar el que hace la operacion
			//de borrado.
			//Bug 0103234
			//Añadimos, si es auditoria de INSERT, UPDATE o DUPLICATE tambien forzamos el bindado!!.

			m4bool_t bMakeBindAuditoryUserRole = M4_FALSE;
			if ((indAuxPar != M4LDB_NULL_INDEX) &&
					((m_oMtLgcTbl.m_bAudStmt) 
					&& ((m_oMtLgcTbl.m_eStmTypeAuditory == M4LDB_DELETE) || (m_oMtLgcTbl.m_eStmTypeAuditory == M4LDB_INSERT) || (m_oMtLgcTbl.m_eStmTypeAuditory == M4LDB_UPDATE) || (m_oMtLgcTbl.m_eStmTypeAuditory == M4LDB_DUPLICATE))
					&& ((eIntFld == M4LDB_SEC_USER) || (eIntFld == M4LDB_APP_ROLE)) ) ) {
					
					bMakeBindAuditoryUserRole = M4_TRUE;
			}
				
			if ((indAuxPar != M4LDB_NULL_INDEX) && (bMakeBindAuditoryUserRole == M4_FALSE) ) {
				// Ya lo tenemos.
				(*m_pvcPersist)[indRealTbl]->m_oVParam.push_back (indAuxPar);
				continue;
			}
			else {
				(*m_pvcPersist)[indRealTbl]->m_oVParam.push_back (indPar);

				if (m_oMtLgcTbl.m_bAudStmt) {
					switch (eIntFld) {
						
						case M4LDB_ID_EXECUTION:
							result = m_pParam->CreateData (indPar, pReg, M4LDB_INTERNAL_HANDLE, M4CL_M4_TYPE_NUMBER, 10, 0, m_pGlobVars->GetpIdExecution (), NULL, M4LDB_PAR_INTERN, M4_FALSE);
							break;					
						
						case M4LDB_ID_ORDER:
							result = m_pParam->CreateData (indPar, pReg, M4LDB_INTERNAL_HANDLE, M4CL_M4_TYPE_NUMBER, 10, 0, m_pGlobVars->GetpIdOrder (), NULL, M4LDB_PAR_INTERN, M4_FALSE);
							break;					
						
						case M4LDB_DT_OPERATION:
							result = m_pParam->CreateData (indPar, pReg, M4LDB_INTERNAL_HANDLE, M4CL_M4_TYPE_TIMESTAMP, -1, 0, m_pGlobVars->GetpDtOper (), NULL, M4LDB_PAR_INTERN, M4_FALSE);
							break;					

						case M4LDB_ID_OPERATION:
							// UNICODE DB
							result = m_pParam->CreateData (indPar, pReg, M4LDB_INTERNAL_HANDLE, M4LDB_M4_UNICODE_TYPE(M4CL_M4_TYPE_FIXED_STRING), 2, 0, m_pGlobVars->GetpIdTypeOper(), NULL, M4LDB_PAR_INTERN, M4_FALSE);
							break;					

						case M4LDB_ID_TYPE_UPDATE:
							// UNICODE DB
							result = m_pParam->CreateData (indPar, pReg, M4LDB_INTERNAL_HANDLE, M4LDB_M4_UNICODE_TYPE(M4CL_M4_TYPE_FIXED_STRING), 2, 0, m_pGlobVars->GetpIdTypeUpd(), NULL, M4LDB_PAR_INTERN, M4_FALSE);
							break;					
						
						case M4LDB_ADD_INFO:
							// UNICODE DB
							result = m_pParam->CreateData (indPar, pReg, M4LDB_INTERNAL_HANDLE, M4CL_M4_TYPE_LONG, -1, 0, m_pGlobVars->GetpAddInfo (), NULL, M4LDB_PAR_INTERN, M4_FALSE);
							break;					

						//Bug 0084837
						case M4LDB_SEC_USER:
							m_oMtLgcTbl.m_indSecUser = indPar;
							//UNICODE DB
							result = m_pParam->CreateData (indPar, pReg, M4LDB_INTERNAL_HANDLE, M4LDB_M4_UNICODE_TYPE(M4CL_M4_TYPE_FIXED_STRING), M4LDB_ID_MAX_SIZE, 0, NULL, m_pGlobVars->GetpIdUserAuditoryPointer(), M4LDB_PAR_INTERN, M4_FALSE);
							break;

						case M4LDB_APP_ROLE:
							m_oMtLgcTbl.m_indAppRole = indPar;
							//UNICODE DB
							result = m_pParam->CreateData (indPar, pReg, M4LDB_INTERNAL_HANDLE, M4LDB_M4_UNICODE_TYPE(M4CL_M4_TYPE_FIXED_STRING), M4LDB_ID_MAX_SIZE, 0, NULL, m_pGlobVars->GetpIdRoleAuditoryPointer(), M4LDB_PAR_INTERN, M4_FALSE);
							break;

						default:
							DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_AUDITORY_LOGIC_FIELD, idField << m_oMtLgcTbl.m_idLgcTbl);
							if (pmpIdFld) {
								delete pmpIdFld;
							}
							return (M4_ERROR);
							break;

					}

					if (result == M4_ERROR) {
						if (pmpIdFld) {
							delete pmpIdFld;
						}
						return (M4_ERROR);
					}

					m_pParam->SetInfoIsReadOnly (indPar, M4_TRUE);

  					#ifdef _M4LDB_DEBUG
						M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\tData[%ld]: Type[%s]. Size[%ld]. Fld[%s].", (m4int32_t) indPar,  g_aM4Type[m_pParam->GetInfoM4Type (indPar)], m_pParam->GetInfoPrec (indPar), idField);
					#endif

					indPar++;

					continue;
				}
			}

			m4bool_t bExists = M4_FALSE;

			switch (eIntFld) {
				
				case M4LDB_DT_LAST_UPDATE:
					m_oMtLgcTbl.m_indDTLstUpd = indPar;
					result = m_pParam->CreateData (m_oMtLgcTbl.m_indDTLstUpd, pReg, M4LDB_INTERNAL_HANDLE, M4CL_M4_TYPE_TIMESTAMP, M4LDB_SIZE_TIMESTAMP, 0, NULL, NULL, M4LDB_PAR_INTERN, M4_FALSE);
					bExists = M4_TRUE;
					break;
				
				case M4LDB_SEC_USER:
					m_oMtLgcTbl.m_indSecUser = indPar;
					//UNICODE DB
					result = m_pParam->CreateData (m_oMtLgcTbl.m_indSecUser, pReg, M4LDB_INTERNAL_HANDLE, M4LDB_M4_UNICODE_TYPE(M4CL_M4_TYPE_FIXED_STRING), M4LDB_ID_MAX_SIZE, 0, NULL, m_pGlobVars->GetpIdUserPointer(), M4LDB_PAR_INTERN, M4_FALSE);
					bExists = M4_TRUE;
					break;

				case M4LDB_APP_ROLE:
					m_oMtLgcTbl.m_indAppRole = indPar;
					//UNICODE DB
					result = m_pParam->CreateData (m_oMtLgcTbl.m_indAppRole, pReg, M4LDB_INTERNAL_HANDLE, M4LDB_M4_UNICODE_TYPE(M4CL_M4_TYPE_FIXED_STRING), M4LDB_ID_MAX_SIZE, 0, NULL, m_pGlobVars->GetpIdRolePointer(), M4LDB_PAR_INTERN, M4_FALSE);
					bExists = M4_TRUE;
					break;

				case M4LDB_ID_DATASET:

					if( m_bUseTramitation == M4_TRUE && m_oMtLgcTbl.m_iMirrorType == 1 )
					{
						m_oMtLgcTbl.m_indDataSet = indPar;
						/* Bug 0154923
						El tipo del parámetro no puede ser sessión, ya que no existe como tal.
						Lo ponemos interno y handle 0
						*/
						result = m_pParam->CreateData (m_oMtLgcTbl.m_indDataSet, pReg, 0, M4LDB_M4_UNICODE_TYPE(M4CL_M4_TYPE_FIXED_STRING), M4LDB_ID_MAX_SIZE, 0, NULL, NULL, M4LDB_PAR_INTERN, M4_FALSE);

						// En tramitación el ID_DATASET es PK
						m_pParam->SetInfoIsPK(m_oMtLgcTbl.m_indDataSet, M4_TRUE);
						bExists = M4_TRUE;
					}
					break;
			}

			if (bExists) {		
				if (result == M4_ERROR) {
					if (pmpIdFld) {
						delete pmpIdFld;
					}
					return (M4_ERROR);
				}

				m_pParam->SetInfoIsReadOnly (indPar, M4_TRUE);

  				#ifdef _M4LDB_DEBUG
					M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\tData[%ld] -> Type[%s]. Size[%ld]. Fld[%s].", (m4int32_t) indPar,  g_aM4Type[m_pParam->GetInfoM4Type (indPar)], m_pParam->GetInfoPrec (indPar), idField);
				#endif

				indPar++;
	
				continue;
			}

			if (m_pMtSent) {
				m4bool_t bFound = M4_FALSE;
				ClMetaParam *pPar = NULL;
				m4int32_t hItem = 0;
				m4int8_t m4Type = 0;
				m4int32_t lPrec = 0;
				m4int32_t lScale = 0;
				eParamType_t eParamType = M4LDB_PAR_ITEM;
				
				if (m_pMtSent->IsAPISQL ()) {
					pPar = m_pMtSent->GetMetaParamByField (idField);

					if (pPar) {
						hItem = pPar->GetHandle (m_pGlobVars);
						m4Type = pPar->GetM4Type (m_pGlobVars);
						lPrec = pPar->GetPrecision (m_pGlobVars);
						lScale = pPar->GetScale (m_pGlobVars);
						eParamType = pPar->GetType ();
						bFound = M4_TRUE;
					}
				}
				else {
					hItem = m_pGlobVars->GetItemHandleByIdField (idField);
					if (hItem)
					{
						/* Bug 0118474
						Si la tabla es mono y el campo es de tipo extendido ID_ORG_MONO no se asocia.
						Es decir, si no es mono o el campo no es ID_ORG_MONO no se asocia
						*/
						/* Bug 0132416
						Si el item es de tipo interno ID_ORGANIZATION_MONO_VALUE (65) si que se debe
						asociar aunque sea tabla mono y campo ID_ORG_MONO
						Es decir, si es 65, o no es mono o el campo no es ID_ORG_MONO no se asocia
						*/

						m4int8_t iInternalType = m_pGlobVars->GetItemInternalType (hItem);

						if(
								iInternalType == M4CL_INTERNAL_TYPE_ID_ORGANIZATION_MONO_VALUE
							||	m_oMtLgcTbl.m_eCurOrg != M4LDB_MONO_ORGANIZATION
							||	strcmpi( pccIdType, "ID_ORG_MONO" ) != 0
							)
						{
							m4Type = m_pGlobVars->GetItemM4Type (hItem);
							lPrec = m_pGlobVars->GetItemDBPrecision (hItem);
							lScale = m_pGlobVars->GetItemScale (hItem);
							eParamType = M4LDB_PAR_ITEM;
							bFound = M4_TRUE;
						}
					}
				}

				if (bFound) {

					switch (eParamType) {
						case M4LDB_PAR_EXTERN:
							result = m_pParam->CreateData (indPar, pReg, hItem, m4Type, lPrec, lScale, NULL, NULL, M4LDB_PAR_EXTERN, M4_FALSE);
							break;

						case M4LDB_PAR_SESSION:
							pMCR = m_pGlobVars->GetpLConn ()->m_pLogonAdaptor->GetSessionRecordSet ()->GetpNodeDef ()->_GetpCMCR ();

							if (pMCR->GetItemType (hItem) == M4CL_ITEM_TYPE_METHOD) {
								m4id_t idField = pMCR->GetItemReadFieldId (hItem);
								DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_ITEM_IS_METHOD, hItem << idField);
								if (pmpIdFld) {
									delete pmpIdFld;
								}
								return (M4_ERROR);
							}

							result = m_pParam->CreateData (indPar, pReg, hItem, m4Type, lPrec, lScale, NULL, NULL, M4LDB_PAR_SESSION, M4_FALSE);
							break;

						case M4LDB_PAR_ITEM:
							if ((m_eStmType == M4LDB_DELETE) && 
								(!IsNeeded (pReg, pmpIdFld))) {
								(*m_pvcPersist)[indRealTbl]->m_oVParam.pop_back ();
								continue;
							}

							if (m_pGlobVars->GetItemType (hItem) == M4CL_ITEM_TYPE_METHOD) {
								m4id_t idField = m_pGlobVars->GetItemReadIdField (hItem);
								DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_ITEM_IS_METHOD, hItem << idField);
								if (pmpIdFld) {
									delete pmpIdFld;
								}
								return (M4_ERROR);
							}

							pMCR = m_pGlobVars->GetpMCR ();

							result = m_pParam->CreateData (indPar, pReg, hItem, m4Type, lPrec, lScale, NULL, NULL, M4LDB_PAR_ITEM, M4_FALSE);
							break;

						case M4LDB_PAR_INTERN:
							DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersist::CreateParam" << "Wrong internal parameter = " << LogCat << indPar);
							if (pmpIdFld) {
								delete pmpIdFld;
							}
							return (M4_ERROR);
							break;

						case M4LDB_PAR_MINUS_INF:
							result = m_pParam->CreateData (indPar, pReg, M4LDB_INTERNAL_HANDLE, M4CL_M4_TYPE_TIMESTAMP, M4LDB_SIZE_TIMESTAMP, 0, (m4pchar_t) &M4LDB_MINUS_INF_DATE, NULL, M4LDB_PAR_MINUS_INF, M4_FALSE);
							break;

						case M4LDB_PAR_PLUS_INF:
							result = m_pParam->CreateData (indPar, pReg, M4LDB_INTERNAL_HANDLE, M4CL_M4_TYPE_TIMESTAMP, M4LDB_SIZE_TIMESTAMP, 0, (m4pchar_t) &M4LDB_PLUS_INF_DATE, NULL, M4LDB_PAR_PLUS_INF, M4_FALSE);
							break;
					}

					if (result == M4_ERROR) {
						if (pmpIdFld) {
							delete pmpIdFld;
						}
						return (M4_ERROR);
					}

  					#ifdef _M4LDB_DEBUG
						M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\tData[%ld] -> Type[%s]. Size[%ld]. Fld[%s].", (m4int32_t) indPar,  g_aM4Type[m_pParam->GetInfoM4Type (indPar)], m_pParam->GetInfoPrec (indPar), idField);
						if ((eParamType == M4LDB_PAR_SESSION) || 
							(eParamType == M4LDB_PAR_ITEM)) {
							M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), " Itm[%s - %ld].", pMCR->GetItemId (hItem), hItem);
						}
					#endif

					indPar++;
			
					continue;
				}
			}

			if( IsNeeded(pReg, pmpIdFld) )
			{
				/* Bug 0150086
				Cuando se está en una insert into select y el campo que se está copiando es
				un blob se está leyendo como binario y se está intentando asignar a blob que
				es cadena.
				Para evitarlo, se cambia artificialmente el tipo de campo del destino de
				blob a binario.
				*/

				m4int8_t	iNewM4Type = 0 ;
				m4int32_t	hNewItem = 0 ;

				m4int8_t	iM4Type = 0 ;
				m4int32_t	iIndex = 0 ;
				m4id_t		sType = 0 ;
				ClRegister	*poRegister = NULL ;
				m4bool_t	bIsPK = M4_FALSE ;

				if( (m_eStmType == M4LDB_INSERT && m_pMtSent != NULL && m_pMtSent->GetIsInsertIntoSelect() == M4_TRUE )
					||
					(m_eStmType == M4LDB_UPDATE && m_pMtSent != NULL && m_pMtSent->GetIsUpdateSelect() == M4_TRUE ) )
				{
					sType = M4LDB_ID (pReg->Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_TYPE].Value.GetWithOutSec ());
					iIndex = m_pGlobVars->GetpLConn ()->FindIndexById (M4LDB_SYS_DICT_EXTENDED_TYPES, sType);

					if( iIndex != M4LDB_NULL_INDEX )
					{
						poRegister = &m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_EXTENDED_TYPES]->Register[iIndex];
						iM4Type = M4LDB_M4_TYPE ((m4int8_t) M4LDB_NUMBER (poRegister->Item[M4LDB_ITEM_SYS_DICT_EXTENDED_TYPES_ID_M4_TYPE].Value.GetWithOutSec ()));

						if( iM4Type == M4CL_M4_TYPE_BLOB )
						{
							hNewItem = M4LDB_INTERNAL_HANDLE ;
							iNewM4Type = M4CL_M4_TYPE_BINARY_STRING ;
						}
						else if ( m_eStmType == M4LDB_UPDATE )
						{	//Para un update select, y los campos que no sean PKs
							// es necesario asignarle un handle para que el prepare, bind y execute
							// tengan en cuenta estos campos.
							bIsPK = ((M4LDB_NUMBER (pReg->Item[M4LDB_ITEM_SYS_DICT_FIELDS_POS_PK].Value.GetWithOutSec ())) ? M4_TRUE : M4_FALSE);
							if ( bIsPK == M4_FALSE )
							{
								hNewItem = M4LDB_INTERNAL_HANDLE ;
								iNewM4Type = iM4Type ;
							}
						}
					}
				}

				result = m_pParam->CreateData (indPar, pReg, hNewItem, iNewM4Type, 0, 0, NULL, NULL, M4LDB_PAR_INTERN, M4_FALSE);

				if (result == M4_ERROR) {
					if (pmpIdFld) {
						delete pmpIdFld;
					}
					return (M4_ERROR);
				}

				if ((m_pMtSent) &&
					(m_oMtLgcTbl.m_bCorrected) && 
					((eIntFld == M4LDB_DT_END_CORR) ||
					(eIntFld == M4LDB_TYPE_REG))) {
					// Esto es necesario por si queremos modificar estos campo y no hay item asociado.
					m_pParam->SetItemHandle (indPar, M4LDB_INTERNAL_HANDLE);
				}

  				#ifdef _M4LDB_DEBUG
					M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\tData[%ld] -> Type[%s]. Size[%ld]. Fld[%s].", (m4int32_t) indPar,  g_aM4Type[m_pParam->GetInfoM4Type (indPar)], m_pParam->GetInfoPrec (indPar), idField);
				#endif

				indPar++;
			}
			else {
				(*m_pvcPersist)[indRealTbl]->m_oVParam.pop_back ();
			}
		}
	}

	if (pmpIdFld) {
		delete pmpIdFld;
	}

	// Creamos el parámetro auxiliar de fecha de fin del registro anterior (se usa en GetCorrectedHistory).
	if (m_oMtLgcTbl.m_bCorrected) {
		m_oMtLgcTbl.m_indBefCorrReg = indPar;
		result = m_pParam->CreateData (m_oMtLgcTbl.m_indBefCorrReg, NULL, 0, M4CL_M4_TYPE_TIMESTAMP, M4LDB_SIZE_TIMESTAMP, 0, NULL, NULL, M4LDB_PAR_INTERN, M4_FALSE);
	}

	if (m_pMtSent && m_eStmType != M4LDB_DELETE)
	{
		m4uint32_t lEnvLang = m_pGlobVars->GetLanguage();
		m4uint32_t lDefLDBLang = m_pGlobVars->GetpLConn()->m_pLdb->GetDefaultLanguage();

		m4uint32_t numTrans = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_TRANS]->Count ();

		pReg = &m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_TRANS_FIELDS]->Register;

		ClRegisterIndex indTrans(0);
		ClRegisterIndex indTrnFld(0);
		m4int32_t indDefFld = M4LDB_NULL_INDEX;
		m4int32_t indTrn	= M4LDB_NULL_INDEX;
		m4int32_t indLDBDef = M4LDB_NULL_INDEX;

		// ¿Existe algún campo traducido?
		for (indTrans=0; indTrans<numTrans; indTrans++)
		{
			m4int32_t hItem = 0;

			m4id_t idField = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_TRANS]->Current[indTrans].Item[M4LDB_ITEM_SYS_DICT_TRANS_ID_FIELD].Value.GetWithOutSec ());

			//Bug 0092565.
			//Ocurre cuando tenemos un API que hace un insert en un campo multilenguaje.
			//Si el tipo de elemento es M4LDB_PAR_ITEM se comprueba el scope para el handle, pero el handle
			//en campos traducibles es una posicion de columna, lo cual no es correcto. Cuando se intenta acceder al 
			//scope da el error.
			//La funcion  SetHandleItem establece el tipo a M4LDB_PAR_ITEM, es el valor que vamos a dejar por defecto aqui.
			eParamType_t eParamType = M4LDB_PAR_ITEM;

			if (m_pMtSent->IsAPISQL ()) {
				ClMetaParam *pPar = m_pMtSent->GetMetaParamByField (idField);

				if (pPar) {
					hItem = pPar->GetHandle (m_pGlobVars);					
					//Se trata de un parametro externo!.
					eParamType = pPar->GetType ();
				}
				else {
					ClMetaField *pFld = m_pMtSent->GetColumnByIdField (idField);
					
					if (pFld) {
						hItem = pFld->m_iColumn + 1;						
						eParamType = M4LDB_PAR_INTERN;
					}
				}
			}
			else {
				hItem = m_pGlobVars->GetItemHandleByIdField (idField);
			}

			if (hItem) {

				indDefFld	= M4LDB_NULL_INDEX;
				indTrn		= M4LDB_NULL_INDEX;
				indLDBDef	= M4LDB_NULL_INDEX;

				m4uint32_t numTrnFld = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_TRANS_FIELDS]->Count ();

				// Buscamos en los campos traducidos del diccionario, hasta que encontremos el lenguaje por defecto y el que estamos utilizando.
				// Si no encontramos el DefLang en el diccionario, buscamos el lenguaje por defecto de la LDB, y utilizamos ese.
				// Siempre que haya lenguaje por defecto en el diccionario se le hace caso antes que al lenguaje del sistema.

				for (indTrnFld=0; ( (indTrnFld<numTrnFld) && (indTrn == M4LDB_NULL_INDEX || indDefFld==M4LDB_NULL_INDEX)  ); indTrnFld++) {

					m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_TRANS_FIELDS]->Register[indTrnFld];

					m4uint32_t lLanguage = M4LDB_NUMBER (pReg->Item[M4LDB_ITEM_SYS_DICT_TRANS_FIELDS_ID_LANG].Value.GetWithOutSec ());

					if (lLanguage == lEnvLang) {

						m4id_t idTrnField = M4LDB_ID (pReg->Item[M4LDB_ITEM_SYS_DICT_TRANS_FIELDS_ID_TRANS_FIELD].Value.GetWithOutSec ());

						indTrn = m_pParam->FindIndexByIdField (idTrnField);

						if (indTrn != M4LDB_NULL_INDEX) {
							m_pParam->SetItemHandle (indTrn, hItem);														
							//Los traducidos seran internos o externos. Estamos sobreescribiendo el valor que ha metido la funcion SetItemHandle!.
							m_pParam->SetParamType( indTrn, eParamType );
							m_pParam->InsertIdData (idField, indTrn);
							m_pParam->SetInfoTranslation (indTrn, idField);
  							#ifdef _M4LDB_DEBUG
								M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n Translation: Fld[%s] --> Fld[%s]. Itm[%ld].", idField, idTrnField, hItem);
							#endif
						}
					}
					if (lLanguage == M4LDB_DEFAULT_LANGUAGE) {
						idDefFld =  M4LDB_ID (pReg->Item[M4LDB_ITEM_SYS_DICT_TRANS_FIELDS_ID_TRANS_FIELD].Value.GetWithOutSec ());
						indDefFld = m_pParam->FindIndexByIdField (idDefFld);

					}
					else 
						if ( lLanguage == lDefLDBLang )
						{
							idDefLDBField = M4LDB_ID (pReg->Item[M4LDB_ITEM_SYS_DICT_TRANS_FIELDS_ID_TRANS_FIELD].Value.GetWithOutSec ());
							indLDBDef = m_pParam->FindIndexByIdField (idDefLDBField );
						}
				}

				// Tenemos que decidir que campo por defecto utilizamos.
				// Si hay alguno en el diccionario, lo dejamos todo igual y seguimos.
				// Si no, miramos a ver si hemos encontrado el de por defecto segun la LDB
				// y utilizamos ese.
				if (indDefFld == M4LDB_NULL_INDEX && indLDBDef!=M4LDB_NULL_INDEX)
				{
					indDefFld = indLDBDef;
					idDefFld  = idDefLDBField;
				}

				// Miramos a ver lo que hemos encontrado.
				
				if ( indTrn != M4LDB_NULL_INDEX && indDefFld != M4LDB_NULL_INDEX )
				{
					// Hemos encontrado los dos.
					if (indDefFld!=indTrn)
					{
						// No estamos trabajando con el lenguaje por defecto.
						// Marcamos el campo del lenguaje por defecto, apuntando al actual.
						m_pParam->SetCurrentLanguageIndex (indDefFld,indTrn);
					}
//						 else 
//							Estamos con el lenguaje por defecto. No tendremos que hacer nada especial.
				}
				else
				{
					if (indTrn == M4LDB_NULL_INDEX )
					{
						// No hemos encontrado el campo traducido. Probamos con el de por defecto.
						if (indDefFld != M4LDB_NULL_INDEX) {
							m_pParam->SetItemHandle (indDefFld, hItem);							
							//Los traducidos seran internos o externos. Estamos sobreescribiendo el valor que ha metido la funcion SetItemHandle!.
							//Bug 0102652
							m_pParam->SetParamType( indDefFld, eParamType );
							m_pParam->InsertIdData (idField, indDefFld);
  							#ifdef _M4LDB_DEBUG
								M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n Translation: Fld[%s] --> Fld[%s]. Itm[%ld].", idField, idDefFld, hItem);
							#endif
						}
						else {
							// No hemos encontrado el campo de traducción.
							DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_TRANS_LOGIC_FIELD_NOT_FOUND, idField << m_oMtLgcTbl.m_idLgcTbl);
							return (M4_ERROR);
						}
					}
//						else
//							No hemos encontrado el lenguaje por defecto
				}

			}
		}
	}


	if (m_eStmType == M4LDB_INSERT) {
		// Reordenamos para que los ordinales PK queden al final.
		for (indRealTbl=0; indRealTbl<numRealTbl; indRealTbl++)	{

			VcVirtualParam &oVParam = (*m_pvcPersist)[indRealTbl]->m_oVParam;

			m4int32_t indLastPar = oVParam.size();

			for (indPar=0; indPar<indLastPar; indPar++) {

				m4int32_t indAux = oVParam[indPar];

				m4int8_t m4type = m_pParam->GetInfoM4Type (indAux);
				
				eFuncType_t idDefaultFunc = m_pParam->GetInfoIdDefaultFunc (indAux);

				if (idDefaultFunc == M4LDB_DEF_ORDINAL_PK || idDefaultFunc == M4LDB_DEF_ORDINAL_PK_NO_DATES) {

					/* Bug 0167708
					Se ordenan, pero sacando la burbuja hacia arriba en vez de hacer swap
					De este modo no cambiamos el orden de los demás campos
					*/
					for( m4int32_t i = indPar ; i < indLastPar - 1; i++ )
					{
						// Se pasan los valores hacia atrás
						oVParam[ i ] = oVParam[ i + 1 ] ;
					}

					oVParam[indLastPar - 1] = indAux;

					/* Bug 0170866
					Se reajustan los índices para no saltarse ningún campo
					*/
					indPar--;
					indLastPar--;
				}
			}
		}
	}


	// Reordenamos para que los parámetros LONG queden al final.
	for (indRealTbl=0; indRealTbl<numRealTbl; indRealTbl++)	{

		VcVirtualParam &oVParam = (*m_pvcPersist)[indRealTbl]->m_oVParam;

		m4int32_t indLastPar = oVParam.size();

		for (indPar=0; indPar<indLastPar; indPar++) {

			m4int32_t indAux = oVParam[indPar];

			m4int8_t m4type = m_pParam->GetInfoM4Type (indAux);
			
			if (m4type == M4CL_M4_TYPE_LONG ||
				m4type == M4CL_M4_TYPE_UNI_LONG ||
				m4type == M4CL_M4_TYPE_BLOB ||
				m4type == M4CL_M4_TYPE_BINARY_STRING) {

				/* Bug 0167708
				Se ordenan, pero sacando la burbuja hacia arriba en vez de hacer swap
				De este modo no cambiamos el orden de los demás campos
				*/
				for( m4int32_t i = indPar ; i < indLastPar - 1; i++ )
				{
					// Se pasan los valores hacia atrás
					oVParam[ i ] = oVParam[ i + 1 ] ;
				}

				oVParam[indLastPar - 1] = indAux;

				/* Bug 0170866
				Se reajustan los índices para no saltarse ningún campo
				*/
				indPar--;
				indLastPar--;
			}
		}
	}


	return (M4_SUCCESS);
}


m4return_t ClPersist::Open (eStmType_t ai_eType, m4int32_t ai_idRConn, ClMetaSentence *ai_pMtSent, m4bool_t ai_bMtSentOwner, m4id_t ai_idMainTable)
{
	m4return_t result;
	ClRegisterIndex indRealTbl(0);
	m4int32_t idRConn = ai_idRConn;

	if (!m_pvcPersist) {

		m_oMtLgcTbl.m_indLgcTbl = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Index;

		m_oMtLgcTbl.m_idLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());

  		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\n # Data of %s.", m_oMtLgcTbl.m_idLgcTbl);
		#endif

		result = m_pGlobVars->GetpLConn ()->LoadDictionaryChannelForeigns ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		m_pvcPersist = new VcPClPersistBase;
		m_pvcPersist->reserve (4);

		result = ClStmt::Open (ai_eType, 0, ai_pMtSent, ai_bMtSentOwner, NULL);

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		m4uint32_t numRealTbl = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Count ();

		for (indRealTbl=0; indRealTbl<numRealTbl; indRealTbl++)	{

			// Situamos el current del nodo SYS_DICT_REAL_OBJECTS en dicho registro.
			m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[indRealTbl];

			//No es Interna solo si m_pMtSent!=NULL y es la primera indRealTbl==0
			ClPersistBase *pPBase = new ClPersistBase (m_pGlobVars, this, ((m_pMtSent==NULL)||(indRealTbl!=0)) ? M4_TRUE : M4_FALSE, m_bUseTramitation, m_bUseInheritance, m_bUseTranslation);

			m_pvcPersist->push_back (pPBase);

			if (idRConn == M4LDB_NULL_INDEX) {
				idRConn = M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_CONN].Value.GetWithOutSec ());
			}

			result = pPBase->Open (ai_eType, idRConn, m_pMtSent, m_pParam, &m_oMtLgcTbl);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
		}

		// Vamos a ver si tiene auditoría de detalle.
		m4int32_t lAudMask;

		result = m_pGlobVars->GetpLConn ()->AnalizeAuditoryObject (m_pGlobVars->GetIdRSM (), m_oMtLgcTbl.m_indLgcTbl, lAudMask);

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		m4int32_t iMask = 1 << (((m4int32_t) m_eStmType) + 3);

		if (m_eStmType == M4LDB_DUPLICATE) {
			iMask = M4LDB_AUD_MASK_DTL_INSERT;
		}

		if (lAudMask & iMask) {
			m_oMtLgcTbl.m_bHasAudit = M4_TRUE;
		}

		m_oMtLgcTbl.m_pMaster = (*m_pvcPersist)[0];
		
		m4bool_t bHasInheritance = M4LDB_BOOL (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_HAS_INHERITANCE].Value.GetWithOutSec ());

		if ((bHasInheritance) && 
			(m_pParam->FindIndexByIdInternalField (M4LDB_DT_START) == M4LDB_NULL_INDEX)) {
			// Si heredamos de otra seremos podemos no tener fechas para la temporalidad por lo que seremos eternas y la base será la que tenga la temporalidad.
			m_oMtLgcTbl.m_eCurTemp = M4LDB_ETHERN;

			m_oMtLgcTbl.m_bCorrected = M4_FALSE;
		}
		else {
			m4int32_t iCurTemp = M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_TEMPORARY].Value.GetWithOutSec ());

			m_oMtLgcTbl.m_eCurTemp = (eTempType_t) iCurTemp;
	
			m_oMtLgcTbl.m_bCorrected = M4LDB_BOOL (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_CORRECTED].Value.GetWithOutSec ());
		}
		
		if (m_oMtLgcTbl.m_eCurTemp == M4LDB_N_PERIOD) { 
			m_oMtLgcTbl.m_eCurHist = (eHistType_t) M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_NP_TYPE].Value.GetWithOutSec ());
			
			if (m_oMtLgcTbl.m_eCurHist == M4LDB_N_PERIOD_WITH_OVERLAPED) {
				m_oMtLgcTbl.m_eCurTemp = M4LDB_1_PERIOD;
			}
		}

		m_oMtLgcTbl.m_bValidate = M4_TRUE;


		if ((m_oMtLgcTbl.m_eCurTemp == M4LDB_1_PERIOD) || 
			(m_oMtLgcTbl.m_eCurTemp == M4LDB_N_PERIOD)) {

			ClRegisterIndex indFor(0);

			m4uint32_t numFor = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Count ();

			for (indFor=0; indFor<numFor; indFor++) {

				ClRegister *pRegFor = &(m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Register[indFor]);

				eRelType_t eRelType = (eRelType_t) M4LDB_NUMBER (pRegFor->Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_RELATION_ORDER].Value.GetWithOutSec ());

				if (eRelType == M4LDB_PARENT_RELATION) {
					eCondValidType_t eAuxCondValid = (eCondValidType_t) M4LDB_NUMBER (pRegFor->Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_CONDUCT_VALID].Value.GetWithOutSec ());

					if (eAuxCondValid == M4LDB_VALID_COMPLETENESS) {
						m_oMtLgcTbl.m_bComplete = M4_TRUE;
						break;
					}
				}
			}
		}

		m_oMtLgcTbl.m_eCurOrg = (eOrgType_t) M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_ORG_TYPE].Value.GetWithOutSec ());
		m_oMtLgcTbl.m_iOrgLevel = M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_ORG_LEVEL].Value.GetWithOutSec ());
		m_oMtLgcTbl.m_bInheritsData = M4LDB_BOOL (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_INHERITS_DATA].Value.GetWithOutSec ());
		m_oMtLgcTbl.m_iMirrorType = M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_MIRROR_TYPE].Value.GetWithOutSec ());
		m_oMtLgcTbl.m_bIsCacheable = M4LDB_BOOL (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_IS_CACHEABLE].Value.GetWithOutSec ());

		result = CreateParam ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}


		if ((m_oMtLgcTbl.m_eCurTemp == M4LDB_1_PERIOD) || 
			(m_oMtLgcTbl.m_eCurTemp == M4LDB_N_PERIOD)) {

			m_oMtLgcTbl.m_indDTStart = m_pParam->FindIndexByIdInternalField (M4LDB_DT_START);

			if (m_oMtLgcTbl.m_indDTStart == M4LDB_NULL_INDEX) {
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, M4LDB_DT_START << m_oMtLgcTbl.m_idLgcTbl);
				return (M4_ERROR);
			}

			m_oMtLgcTbl.m_indDTEnd = m_pParam->FindIndexByIdInternalField (M4LDB_DT_END);

			if (m_oMtLgcTbl.m_indDTEnd == M4LDB_NULL_INDEX) {
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, M4LDB_DT_END << m_oMtLgcTbl.m_idLgcTbl);
				return (M4_ERROR);
			}
		}

		if (m_oMtLgcTbl.m_bCorrected) {
			m4date_t dClosingDate = 0;
				
			m4id_t idClsDate = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_CLS_DATE].Value.GetWithOutSec ());

			if ((*idClsDate) && 
				(m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_CLS_DATE]->Register.Count ())) {
				dClosingDate = M4LDB_DOUBLE (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_CLS_DATE]->Register[0].Item[M4LDB_ITEM_SYS_DICT_CLS_DATE_DT_CLOSING].Value.GetWithOutSec ());
			}

			m_oMtLgcTbl.m_dClosingDate = (dClosingDate) ? dClosingDate : M4LDB_MINUS_INF_DATE;

			m_oMtLgcTbl.m_indDTStartCorr = m_pParam->FindIndexByIdInternalField (M4LDB_DT_START_CORR);

			if (m_oMtLgcTbl.m_indDTStartCorr == M4LDB_NULL_INDEX) {
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, M4LDB_DT_START_CORR << m_oMtLgcTbl.m_idLgcTbl);
				return (M4_ERROR);
			}

			m_oMtLgcTbl.m_indDTEndCorr = m_pParam->FindIndexByIdInternalField (M4LDB_DT_END_CORR);

			if (m_oMtLgcTbl.m_indDTEndCorr == M4LDB_NULL_INDEX) {
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, M4LDB_DT_END_CORR << m_oMtLgcTbl.m_idLgcTbl);
				return (M4_ERROR);
			}

			if (m_oMtLgcTbl.m_eCurTemp == M4LDB_N_PERIOD) {

				m_oMtLgcTbl.m_indRegType = m_pParam->FindIndexByIdInternalField (M4LDB_TYPE_REG);

				if (m_oMtLgcTbl.m_indRegType == M4LDB_NULL_INDEX) {
					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, M4LDB_TYPE_REG << m_oMtLgcTbl.m_idLgcTbl);
					return (M4_ERROR);
				}

				if (m_pMtSent) {
					m_pParam->SetInfoIsReadOnly (m_oMtLgcTbl.m_indRegType, M4_TRUE);
				}
			}
		}

		// Utilizamos el tipo m4 de la fecha de inicio de aplicación o correccion para establecer la unidad de fecha.
		m_oMtLgcTbl.m_dDateUnit = M4_ONE_DAY;

		m4int32_t indDate = m_oMtLgcTbl.m_indDTStart;

		if (indDate == M4LDB_NULL_INDEX)
		{
			indDate = m_oMtLgcTbl.m_indDTStartCorr;
		}

		if (indDate != M4LDB_NULL_INDEX && m_pParam->GetInfoM4Type (indDate) == M4CL_M4_TYPE_TIMESTAMP)
		{
			m_oMtLgcTbl.m_dDateUnit = M4_ONE_SECOND;
		}

		if (m_oMtLgcTbl.m_eCurOrg == M4LDB_MULTI_ORGANIZATION)
		{
			m_oMtLgcTbl.m_indOrgType = m_pParam->FindIndexByIdInternalField (M4LDB_ID_ORGANIZATION);

			if (m_oMtLgcTbl.m_indOrgType == M4LDB_NULL_INDEX)
			{
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, M4LDB_ID_ORGANIZATION << m_oMtLgcTbl.m_idLgcTbl);
				return (M4_ERROR);
			}
		}

		// Limpiamos para los campos long.
		m_pParam->ClearLongs ();

		/* Bug 0167666
		Si no hay tabla principal en herencia es la primera
		*/
		if( ai_idMainTable == NULL )
		{
			ai_idMainTable = m_oMtLgcTbl.m_idLgcTbl;
		}

		if (m_pMtSent && bHasInheritance)
		{
			// Heredamos de otra tabla.
			// Nos situamos en la tabla padre. Como tenemos recursividad no es necesario crear cursor para todas (ya lo haremos cuando estemos en la padre).
			m4int32_t indParentLgcTbl = M4LDB_NULL_INDEX;

			m4id_t idParentLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_INHERITANCE]->Current[(ClRegisterIndex) 0].Item[M4LDB_ITEM_SYS_DICT_INHERITANCE_ID_AUX_OBJECT].Value.GetWithOutSec ());

			result = m_pGlobVars->GetpLConn ()->LoadDictionaryChannel (idParentLgcTbl, &indParentLgcTbl, M4_TRUE);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}

			if (m_oMtLgcTbl.m_indLgcTbl != (m4uint32_t) indParentLgcTbl) {
				// Sabemos que no es la misma.

				m_pMtSent->SetInheritance ();

				m_pInherit = new ClPersist (m_pGlobVars, M4_TRUE, m_bUseTramitation, m_bUseInheritance, m_bUseTranslation);

				result = m_pInherit->Open (ai_eType, M4LDB_NULL_INDEX, m_pMtSent, M4_FALSE, ai_idMainTable);

				if (result == M4_ERROR) {
					return (M4_ERROR);
				}

				// Resituamos el current en la tabla principal.
				m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[m_oMtLgcTbl.m_indLgcTbl];
			}
		}
		else
		{
			m_oMtLgcTbl.m_pMaster->SetMainTable(ai_idMainTable);
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersist::Defuse (ClLConn *ai_pLConn)
{
	m4return_t result = M4_SUCCESS;

	if (m_pInherit) {
		result = m_pInherit->Defuse (ai_pLConn);

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}
	}

	if (m_pvcPersist) {
		for (itVcPClPersistBase it=m_pvcPersist->begin (); it!=m_pvcPersist->end(); it++) {

			result = (*it)->Defuse (ai_pLConn);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
		}
	}

	return (result);
}


m4return_t ClPersist::Release (void)
{
	m4return_t result = M4_SUCCESS;

	if (m_pInherit) {
		result = m_pInherit->Release ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}
	}

	if (m_pvcPersist) {
		for (itVcPClPersistBase it=m_pvcPersist->begin (); it!=m_pvcPersist->end(); it++) {

			result = (*it)->Release ();

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
		}
	}

	m_pParam->ClearLongs ();

	return (result);
}


m4return_t ClPersist::Destroy (void)
{
	m4return_t result;

	if (m_pInherit) {
		result = m_pInherit->Destroy ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		delete m_pInherit;
		m_pInherit = NULL;
	}

	if (m_pMainLeaf) {
		m_pMainLeaf = NULL;
	}

	if (m_pvcPersist) {
		for (itVcPClPersistBase it=m_pvcPersist->begin (); it!=m_pvcPersist->end(); it++) {

			result = (*it)->Destroy ();

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}

			delete (*it);
		}
		
		delete m_pvcPersist;
		m_pvcPersist = NULL;
	}

	return (M4_SUCCESS);
}


m4return_t ClPersist::Close (void)
{
	m4return_t result;

	if (m_pInherit) {
		result = m_pInherit->Close ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}
	}

	if (m_pvcPersist) {
		for (itVcPClPersistBase it=m_pvcPersist->begin (); it!=m_pvcPersist->end(); it++) {

			result = (*it)->Close ();

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
		}
		
		if (m_pParam){
			m_pParam->ClearEncryptedBlobs();
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersist::Prepare (void)
{
	m4return_t result;
	ClRegisterIndex indRealTbl(0);

	if (m_pInherit) {
		result = m_pInherit->Prepare ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

	}

	// Nos situamos, por si acaso (mas vale prevenir...).
	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[m_oMtLgcTbl.m_indLgcTbl];

	if (m_pMtSent) {
		// Esto sirve para los parámetros internos.
		m_pMtSent->SetParam (m_pParam);
	}

	ClMetaSentence *pMtSent = m_pMtSent;

	result = m_pGlobVars->GetpLConn ()->_X_AnalizeSecurity (NULL, pMtSent, m_pGlobVars, NULL, &m_oMtLgcTbl.m_lSecMask, m_eStmType, m_pParam);

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	if ((pMtSent) && 
		(pMtSent != m_pMtSent)) {
		m_oMtLgcTbl.m_pMaster->m_pMtSecSent = pMtSent;
	}

	m_oMtLgcTbl.m_dClosingDate = (!(m_oMtLgcTbl.m_lSecMask & M4LDB_SEC_MASK_DEFUSE_CLOSING_DATE)) ? m_oMtLgcTbl.m_dClosingDate : M4LDB_MINUS_INF_DATE;

	if (!(m_oMtLgcTbl.m_lSecMask & M4LDB_SEC_MASK_UPDATE_DT_END_ON_CASCADE)) {
		// Si no puedo actualizar la fecha de fin, tampoco puedo actualizar la fecha de inicio.
		m_oMtLgcTbl.m_lSecMask &= ~M4LDB_SEC_MASK_UPDATE_DT_START_ON_CASCADE;
	}

	m4uint32_t numRealTbl = m_pvcPersist->size ();

	for( indRealTbl = 0; indRealTbl < numRealTbl; indRealTbl++ )
	{
		// Por si nos lo cambian dentro del Prepare (que seguro que se hace) o nos viene cambiado del AnalizeSecurity.
		m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[m_oMtLgcTbl.m_indLgcTbl];

		// Situamos el current del nodo SYS_DICT_REAL_OBJECTS en dicho registro.
		m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[indRealTbl];

		m4bool_t	bIgnore = M4_FALSE;
		m4pchar_t	pccTranslation = "";

		if( indRealTbl != 0 )
		{
			m4uint32_t	iIsTranslation = M4LDB_NUMBER( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_IS_TRANSLATION].Value.GetWithOutSec() );

			if( iIsTranslation == 1 )
			{
				pccTranslation = " Translation.";

				if( (		m_eStmType == M4LDB_INSERT
						||	m_eStmType == M4LDB_DUPLICATE
						|| ( m_eStmType == M4LDB_UPDATE && m_bUseTranslation == M4_TRUE && (*m_pvcPersist)[indRealTbl]->m_pMtLgcTbl->m_bUpdatePK == M4_FALSE )
					)
					&& m_pGlobVars->GetpLConn()->IsDynamicLanguage( m_pGlobVars->GetLanguage() ) == M4_FALSE )
				{
					// Si no es un lenguaje dinámico no se inserta ni modifica en la tabla de traducciones
					// Salvo actualizaciones en cascada o pk
					bIgnore = M4_TRUE;
				}
			}
		}

		if( bIgnore == M4_FALSE )
		{
  			#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\n # Preparing Real Table [%ld].%s", (m4int32_t) indRealTbl, pccTranslation);
				M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n\n # Preparing Real Table [%ld].%s", (m4int32_t) indRealTbl, pccTranslation);
			#endif

			result = (*m_pvcPersist)[indRealTbl]->Prepare ();

			if( result == M4_ERROR )
			{
				return (M4_ERROR);
			}
		}
	}

	/* Bug 0165266
	La auditoría se debe preparar después de todas las principales
	*/
	if (m_oMtLgcTbl.m_bHasAudit)
	{
		// Vamos a montar el cursor para auditoría de detalle.
		result = m_oMtLgcTbl.m_pMaster->PrepareAuditory ();

		if (result == M4_ERROR)
		{
			return (M4_ERROR);
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersist::BindParam (m4bool_t ai_bBlobAsBinary)
{
	m4return_t result;
	ClRegisterIndex indRealTbl(0);

	if (m_pInherit) {
		result = m_pInherit->BindParam ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}
	}

	m4uint32_t numRealTbl = m_pvcPersist->size ();

	for( indRealTbl = 0; indRealTbl < numRealTbl; indRealTbl++ )
	{
		// Por si nos lo cambian dentro del BindParam (que seguro que se hace).
		m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[m_oMtLgcTbl.m_indLgcTbl];

		// Situamos el current del nodo SYS_DICT_REAL_OBJECTS en dicho registro.
		m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[indRealTbl];

		m4bool_t	bIgnore = M4_FALSE;
		m4pchar_t	pccTranslation = "";

		if( indRealTbl != 0 )
		{
			m4uint32_t	iIsTranslation = M4LDB_NUMBER( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_IS_TRANSLATION].Value.GetWithOutSec() );

			if( iIsTranslation == 1 )
			{
				pccTranslation = " Translation.";

				if( (		m_eStmType == M4LDB_INSERT
						||	m_eStmType == M4LDB_DUPLICATE
						|| ( m_eStmType == M4LDB_UPDATE && m_bUseTranslation == M4_TRUE && (*m_pvcPersist)[indRealTbl]->m_pMtLgcTbl->m_bUpdatePK == M4_FALSE )
					)
					&& m_pGlobVars->GetpLConn()->IsDynamicLanguage( m_pGlobVars->GetLanguage() ) == M4_FALSE )
				{
					// Si no es un lenguaje dinámico no se inserta ni modifica en la tabla de traducciones
					// Salvo actualizaciones en cascada o pk
					bIgnore = M4_TRUE;
				}
			}
		}

		if( bIgnore == M4_FALSE )
		{
  			#ifdef _M4LDB_DEBUG
				M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\n # Binding Parameters Real Table [%ld].%s", (m4int32_t) indRealTbl, pccTranslation);
				M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_BIND, m_pGlobVars->GetpLConn (), "\n\n # Binding Parameters Real Table [%ld].%s", (m4int32_t) indRealTbl, pccTranslation);
			#endif

			result = (*m_pvcPersist)[indRealTbl]->BindParam (ai_bBlobAsBinary);

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}
	}

	/* Bug 0165266
	La auditoría se debe bindar después de todas las principales
	*/
	if (m_oMtLgcTbl.m_bHasAudit)
	{
		// Vamos a montar el cursor para auditoría de detalle.
		result = m_oMtLgcTbl.m_pMaster->BindParamAuditory ();

		if (result == M4_ERROR)
		{
			return (M4_ERROR);
		}
	}

	return (M4_SUCCESS);
}


// Auditoría.
m4return_t ClPersist::Open (ClDinArr *ai_pParam)
{
	m4return_t result;
	ClRegisterIndex indRealTbl(0);

	if (!m_pvcPersist) {

		m_oMtLgcTbl.m_indLgcTbl = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Index;

		m_pvcPersist = new VcPClPersistBase;
		m_pvcPersist->reserve (4);

		result = ClStmt::Open (M4LDB_INSERT, 0, NULL, M4_FALSE, ai_pParam);

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		m4uint32_t numRealTbl = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Count ();

		for (indRealTbl=0; indRealTbl<numRealTbl; indRealTbl++)	{

			// Situamos el current del nodo SYS_DICT_REAL_OBJECTS en dicho registro.
			m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[indRealTbl];

			ClPersistBase *pPBase = new ClPersistBase (m_pGlobVars, this, M4_TRUE, m_bUseTramitation, m_bUseInheritance, m_bUseTranslation);

			m4int32_t idRConn = M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_CONN].Value.GetWithOutSec ());

			result = pPBase->Open (M4LDB_INSERT, idRConn, m_pMtSent, m_pParam, &m_oMtLgcTbl);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}

			m_pvcPersist->push_back (pPBase);
		}
		
		m_oMtLgcTbl.m_bAudStmt = M4_TRUE;

		m_oMtLgcTbl.m_pMaster = (*m_pvcPersist)[0];

		m_oMtLgcTbl.m_idLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());

		result = CreateParam ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		// Limpiamos para los campos long.
		m_pParam->ClearLongs ();
	}

	return (M4_SUCCESS);
}


m4bool_t ClPersist::IsNeeded (ClRegister *ai_pReg, PMpIdData &ao_pmpIdFld)
{
	if ((m_eStmType == M4LDB_INSERT) || 
		(m_eStmType == M4LDB_DUPLICATE) || 
		(m_oMtLgcTbl.m_bHasAudit)) {
		// El campo siempre es necesitado en el insert y si tiene auditoria.
		return (M4_TRUE);
	}

	m4bool_t bHasSecurity = M4LDB_BOOL (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_HAS_SECURITY].Value.GetWithOutSec ());

	if (bHasSecurity) {
		return (M4_TRUE);
	}

	m4bool_t bIsPK = ((M4LDB_NUMBER (ai_pReg->Item[M4LDB_ITEM_SYS_DICT_FIELDS_POS_PK].Value.GetWithOutSec ())) ? M4_TRUE : M4_FALSE);

	if (bIsPK) {
		return (M4_TRUE);
	}

	eFldType_t eIntFld = (eFldType_t) M4LDB_NUMBER (ai_pReg->Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_INTERNAL_FIELD].Value.GetWithOutSec ());
	
	if (((eIntFld == M4LDB_DT_START) ||
		(eIntFld == M4LDB_DT_END)) && 
		(m_oMtLgcTbl.m_eCurTemp != M4LDB_ETHERN)) {
		return (M4_TRUE);
	}
	
	if (((eIntFld == M4LDB_DT_START_CORR) ||
		(eIntFld == M4LDB_DT_END_CORR) ||
		(eIntFld == M4LDB_TYPE_REG)) && 
		(m_oMtLgcTbl.m_bCorrected)) {
		return (M4_TRUE);
	}

	//Es needed si estamos en un update select 
	if( m_eStmType == M4LDB_UPDATE && m_pMtSent != NULL && m_pMtSent->GetIsUpdateSelect() == M4_TRUE )
	{
		if ( IsNeededInUpdateSelect ( ai_pReg ) == M4_TRUE )
		{
			return ( M4_TRUE ); 
		}
	}

	/* Bug 0155070
	En tramitación y herencia se necesitan todos los campos en el update
	por si hay que hacer un insert de modificación
	*/
	if( m_eStmType == M4LDB_UPDATE )
	{
		if( m_bUseTramitation == M4_TRUE && m_oMtLgcTbl.m_iMirrorType == 1 )
		{
			return (M4_TRUE);
		}

		if( m_bUseInheritance == M4_TRUE && m_oMtLgcTbl.m_bInheritsData == M4_TRUE )
		{
			/* Bug 0157038
			El campo OPERATION no debe aparecer porque ya se pone manualmente
			*/
			if( eIntFld != M4LDB_ID_INHERIT_OPERATION )
			{
				return (M4_TRUE);
			}
		}
	}

	m4bool_t bIsCtrlRep = ((M4LDB_NUMBER (ai_pReg->Item[M4LDB_ITEM_SYS_DICT_FIELDS_CTRL_REPETITION].Value.GetWithOutSec ())) ? M4_TRUE : M4_FALSE);

	if (bIsCtrlRep) {
		return (M4_TRUE);
	}

	if (!ao_pmpIdFld) {

		ao_pmpIdFld = new MpIdData;

		ClRegisterIndex indFor(0);
		ClRegisterIndex indForFld(0);

		m4uint32_t numFor = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Count ();

		if (m_eStmType == M4LDB_DELETE) {

			for (indFor=0; indFor<numFor; indFor++) {

				ClRegister_Current *pCurFor = &(m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current[indFor]);

				eRelType_t eRelType = (eRelType_t) M4LDB_NUMBER (pCurFor->Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_RELATION_ORDER].Value.GetWithOutSec ());

				if (eRelType == M4LDB_PARENT_RELATION) {
					eCondValidType_t eAuxCondValid = (eCondValidType_t) M4LDB_NUMBER (pCurFor->Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_CONDUCT_VALID].Value.GetWithOutSec ());

					if (eAuxCondValid != M4LDB_VALID_COMPLETENESS) {
						continue;
					}
				}

				m4uint32_t numForFld = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGN_PARAMS]->Count ();

				for (indForFld=0; indForFld<numForFld; indForFld++) {

					m4id_t idField = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGN_PARAMS]->Register[indForFld].Item[M4LDB_ITEM_SYS_DICT_FOREIGN_PARAMS_ID_FIELD].Value.GetWithOutSec ());

					struct stIdField stNew;

					stNew.m_pIdField = idField;

					ao_pmpIdFld->insert (MpIdData::value_type (stNew, 0));
				}
			}
		}
		else {
			for (indFor=0; indFor<numFor; indFor++) {

				m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current[indFor];

				m4uint32_t numForFld = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGN_PARAMS]->Count ();

				for (indForFld=0; indForFld<numForFld; indForFld++) {

					m4id_t idField = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGN_PARAMS]->Register[indForFld].Item[M4LDB_ITEM_SYS_DICT_FOREIGN_PARAMS_ID_FIELD].Value.GetWithOutSec ());

					struct stIdField stNew;

					stNew.m_pIdField = idField;

					ao_pmpIdFld->insert (MpIdData::value_type (stNew, 0));
				}
			}
		
			if (m_pMtSent) {
				ClRegisterIndex indTrans(0);
				ClRegisterIndex indTrnFld(0);

				m4uint32_t numTrans = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_TRANS]->Count ();

				for (indTrans=0; indTrans<numTrans; indTrans++) {

					m4int32_t hItem = 0;

					m4id_t idField = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_TRANS]->Current[indTrans].Item[M4LDB_ITEM_SYS_DICT_TRANS_ID_FIELD].Value.GetWithOutSec ());

					if (m_pMtSent->IsAPISQL ()) {
						ClMetaParam *pPar = m_pMtSent->GetMetaParamByField (idField);

						if (pPar) {
							hItem = pPar->GetHandle (m_pGlobVars);
						}
					}
					else {
						hItem = m_pGlobVars->GetItemHandleByIdField (idField);
					}

					if (hItem) {

						struct stIdField stNew;

						stNew.m_pIdField = idField;

						ao_pmpIdFld->insert (MpIdData::value_type (stNew, 0));
			
						m4uint32_t numTrnFld = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_TRANS_FIELDS]->Count ();

						for (indTrnFld=0; indTrnFld<numTrnFld; indTrnFld++) {

							m4id_t idTrnField = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_TRANS_FIELDS]->Register[indTrnFld].Item[M4LDB_ITEM_SYS_DICT_TRANS_FIELDS_ID_TRANS_FIELD].Value.GetWithOutSec ());

							struct stIdField stNew;

							stNew.m_pIdField = idTrnField;

							ao_pmpIdFld->insert (MpIdData::value_type (stNew, 0));
						}
					}
				}
			}
		}
	}

	m4id_t idField = M4LDB_ID (ai_pReg->Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_FIELD].Value.GetWithOutSec ());

	struct stIdField stAux;

	stAux.m_pIdField = idField;

	itMpIdData it = ao_pmpIdFld->find (stAux);

	if (it != ao_pmpIdFld->end ()) {
		return (M4_TRUE);
	}

	return (M4_FALSE);
}

//Comprobamos si el campo de la tabla IsNeeded en un update select
// si está en la metasentencia en el vector de campos m_pvcFld
m4bool_t ClPersist::IsNeededInUpdateSelect (ClRegister* ai_pReg)
{
	m4id_t idField = 0; 
	m4int32_t numFld = 0;
	ClMetaField* pFld = NULL;

	idField = M4LDB_ID (ai_pReg->Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_FIELD].Value.GetWithOutSec ());

	numFld = m_pMtSent->GetNumFields ();

	for (m4int32_t indFld=numFld-1; indFld>=0; indFld--)
	{
		pFld = m_pMtSent->GetFieldByPos (indFld);

		if ( !strcmpi(pFld->m_stField.c_str(), idField) )
		{
			return (M4_TRUE);
		}
	}
	
	return (M4_FALSE) ;
}
	

