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


#include "m4mdrt.hpp"
#include "cldates.hpp"
#include "node.hpp"
#include "medvarin.hpp"
#include "cllgadap.hpp"
#include "cllgadap.hpp"
#include "regpipe.hpp"

#include "defmain.hpp"


#include "clldb.hpp"
#include "clload.hpp"
#include "cltblmem.hpp"
#include "clmtsent.hpp"
#include "clpersis.hpp"
#include "cldinarr.hpp"
#include "ldb_log.hpp"
#include "ldbglobals.hpp"

#include "arecset.hpp"



m4return_t ClPersist::Execute2 (void)
{
	m4return_t result = M4_ERROR;
	m4double_t dValue = 0;
	m4int32_t indParam;
	m4bool_t bOperation = M4_FALSE;
	m4bool_t bFirst = M4_TRUE;
	m4bool_t bPersist;
	VersionMark oLastUpdate;
		
	ClHandle hItem(0);
	
	//=======================================================================================
	//Bug 0072475. Registro bloqueado en herencia.
	//-----------------------------------------------------
	//Antes de lanzar la ejecución comprobamos la herencia...
	//Al insertar aquí el código, el orden de busqueda es padre, hijo, nieto...
	//La razon es porque Execute2 es recursivo: Entra con el nieto, pero como hay herencia
	//se llama Execute2 para hijo y este llama a Execute2 para padre.
	//Por lo tanto el primero en llegar a este punto es padre, es decir la tabla
	//padre de todas y despues se va retornando de la recursión al hijo y por ultimo al nieto.
	//La ejecucion Execute (pReg, &bPersist); produce la validación de update...
	//Por lo tanto la persistencia se efectua por registro. Por lo tanto, las comprobaciones de cambio
	//se realizan a nivel de registro.!!!
	//-----------------------------------------------------
	//---------------------------------------------------
	//Lo importante es que la ultima ClPersist, (la que tiene DT_LAST_UPDATE) se entere que alguna de sus descendientes
	//intenta persistir.

	if (m_pInherit)
	{
		//-----------------------------------
		//Antes de bajar en la herencia reseteamos flags. Es lo que haría el constructor.
		//Lo que no tocamos es el flag de raiz que se setea fuera de la ejecución recursiva.
		m_bExecuteChangesIntoInheritance = M4_FALSE;
		SetChangesIntoMainLeaf (M4_FALSE);
		SetForcePersistInUpdate (M4_FALSE);
		//-----------------------------------
		result = m_pInherit->Execute2 ();

		if (result == M4_ERROR)
		{
			return (M4_ERROR);
		}
	}

	//---------------------------------------------------
	//Cuando volvemos de la recursión:
	//---------------------------------------------------
	
	//Bug 0081643
	//Si m_pInherit == NULL es pq estamos en la tabla padre de la persistencia. Es la primera que se persistirá,
	//nos la guardamos.
	//Se trata de la hoja de la recursión y es la primera en persistirse. Es la que tiene DT_LAST_UPDATE.
	//En principio indicamos que no hay cambios, pero se decidirá más tarde.
	if (m_pInherit == NULL)
	{
		m_pMainLeaf = this;
		SetChangesIntoMainLeaf (M4_FALSE);
	}
	else
	{
		//Si no estamos en la hoja es pq ya estamos subiendo de la recursión, tengo que propagar flags hacia arriba
		//De esta forma, cada persist sabe quien es la hoja y si ha habido cambios en ella.
		SetpMainLeaf (m_pInherit->GetpMainLeaf ());
		SetChangesIntoMainLeaf (m_pInherit->GetChangesIntoMainLeaf ());
	}
	//---------------------------------------------------

	m_oMtLgcTbl.m_bInvRecordSet = M4_FALSE;

	// Obtenemos el puntero al objeto register.
	PClRegister pReg = &m_pGlobVars->GetpAccessRecordSet ()->Register;

	// Obtenemos el número de registros el bloque.
	m4int32_t numReg = m_pGlobVars->GetpAccessRecordSet ()->Count ();

	m4int32_t indReg = 0;
	//-------------------------------------------------------------------------
	//Vamos a quedarnos con el último registro que habrá que tocar!.
	m4int32_t indRegLastWithOperation = 0;
	for (indReg = 0; indReg < numReg; indReg++)
	{
		m_pGlobVars->GetpAccessRecordSet ()->Register[(ClRegisterIndex) indReg];

		bOperation = M4_FALSE;

		if (pReg->Operation.IsValid () && pReg->Operation.IsModifiedByDDBB())
		{
			switch (m_eStmType)
			{
				case M4LDB_INSERT:
					if (pReg->Operation.IsNewByDDBB ())
					{
						//Bug 0083508
						//El Copy-Paste de la wrapper serializa la fecha de última actualización, está dando problemas
						//que tenga valor. Le metemos nosotros un 0.
						VersionMark oLastUpdateInNewRecord;
						oLastUpdateInNewRecord.Date = 0.0;
						pReg->SetLastUpdated (oLastUpdateInNewRecord);
						bOperation = M4_TRUE;
					}
					break;

				case M4LDB_UPDATE:
					if (pReg->Operation.IsUpdatedByDDBB () && 
						!pReg->Operation.IsDeletedByDDBB () && 
						!pReg->Operation.IsNewByDDBB ())
					{
						bOperation = M4_TRUE;
					}
					break;

				case M4LDB_DELETE:
					// Si es borrado y no es nuevo y borrado.
					if (pReg->Operation.IsDeletedByDDBB () && pReg->Operation.IsNewAndDeleted() == M4_FALSE)
					{
						bOperation = M4_TRUE;
					}
					break;
			}
		}
		if (bOperation)
		{
			indRegLastWithOperation = indReg;
		}
	}
	//-------------------------------------------------------------------------

	for (indReg = 0; indReg < numReg; indReg++)
	{
		m_pGlobVars->GetpAccessRecordSet ()->Register[(ClRegisterIndex) indReg];

		bOperation = M4_FALSE;

		if (pReg->Operation.IsValid () && pReg->Operation.IsModifiedByDDBB())
		{
			switch (m_eStmType)
			{
				case M4LDB_INSERT:
					if (pReg->Operation.IsNewByDDBB ())
					{
						//Bug 0083508
						//El Copy-Paste de la wrapper serializa la fecha de última actualización, está dando problemas
						//que tenga valor. Le metemos nosotros un 0.
						VersionMark oLastUpdateInNewRecord;
						oLastUpdateInNewRecord.Date = 0.0;
						pReg->SetLastUpdated (oLastUpdateInNewRecord);
						bOperation = M4_TRUE;
					}
					break;

				case M4LDB_UPDATE:
					if (pReg->Operation.IsUpdatedByDDBB () && 
						!pReg->Operation.IsDeletedByDDBB () && 
						!pReg->Operation.IsNewByDDBB ())
					{
						bOperation = M4_TRUE;
					}
					break;

				case M4LDB_DELETE:
					// Si es borrado y no es nuevo y borrado.
					if (pReg->Operation.IsDeletedByDDBB () && pReg->Operation.IsNewAndDeleted() == M4_FALSE)
					{
						bOperation = M4_TRUE;
					}
					break;
			}
		}

		if (!bOperation)
		{
			continue;
		}

		m4bool_t bRetry = M4_FALSE;

		m_pGlobVars->SetNumberOfAttempts (M4LDB_NULL_INDEX);

		m4int32_t numParam = m_pParam->GetNumData ();

		do
		{
			#ifdef _M4LDB_DEBUG
				M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\nRegister [%ld].", (m4int32_t) pReg->Index);
			#endif

			//Refactorización.
			//Mostramos información en fichero de traza.
			SetDataValueInRecord (pReg, M4_TRUE);

			pReg->GetFlagPersisted (bPersist);
			
			if (!bPersist)
			{
				// Esto es para igualar siempre el Version y el OldVersion.
				pReg->GetLastUpdated (oLastUpdate);

				pReg->SetLastUpdated (oLastUpdate);
			}

			bPersist = M4_FALSE;

			result = Execute (pReg, &bPersist);

			m_pParam->ClearEncryptedBlobs();

			bRetry = M4_FALSE;

			if (m_pGlobVars->IsRetry ())
			{
				m_pGlobVars->SetIsRetry (M4_FALSE);
				if (m_pGlobVars->GetNumberOfAttempts () != M4LDB_NULL_INDEX)
				{
					if (m_pGlobVars->GetNumberOfAttempts ())
					{
						bRetry = M4_TRUE;
					}
					m_pGlobVars->SetNumberOfAttempts (m_pGlobVars->GetNumberOfAttempts ()-1);
				}
			}

		} while (bRetry);
		

		if (result == M4_ERROR)
		{
			return (M4_ERROR);
		}

		//---------------------------------------------------
		//Bug 0081643
		//Lo ponemos ANTES de establecer la fecha de registro!!.
		//Para controlar si tenemos que actualizar la fecha del registro pq necesitamos saber si la hoja finalmente se persistirá.
		//Si se ha guardado algo nos lo apuntamos para estar seguros que el padre se persistirá!!.
		//Si m_pInherit == NULL es pq estamos en la tabla padre de la persistencia.
		//Si la tabla base ya se persiste, no hara falta persistir por herencia.
		//Solo se persiste en herencia si no se ha detectado cambios en la base pero sí se han detectado cambios en
		//las otras.
		if ((bPersist) && (m_pInherit == NULL) )
		{
			m_bExecuteChangesIntoInheritance = M4_FALSE;
			m_bChangesIntoMainLeaf = M4_TRUE;
		}
		else
		{
			/* Bug 0171503
			Si el registro no tiene cambios hay que poner la variable a false para que no actualice la fecha
			De momento se hacha para atrás por el impacto
			m_bChangesIntoMainLeaf = M4_FALSE;
			*/
		}

		//La hoja es la persist de la tabla princìpal. Si no estamos en hoja, ha habido cambios y no ha habido
		//cambios en la hoja es pq se han encontrados cambios en la herencia que la tabla base no se enteraría.
		//Por lo tanto tendremos que persistir en la recursion!!.
		if ((bPersist) && (m_pInherit != NULL) && (m_bChangesIntoMainLeaf == M4_FALSE) )
		{
			m_bExecuteChangesIntoInheritance = M4_TRUE;
		}
		//---------------------------------------------------

		//Bug 0072475. Registro bloqueado en herencia.
		//Se incluye otra opcion:
		//Siempre que haya persistencia debemos grabar en la tabla que tiene DT_LAST_UPDATE, independientemente
		//de que sea owner. La tabla que tiene la fecha de actualizacion, en este Execute2 siempre es
		//la ultima en el recorrido por herencia. Por lo tanto debo obtener el momento de la transaccion.		
		//Escenarios para ver si se actualiza la fecha:
		//El último tiene que poner la fecha al registro!!. Veamos como identificar al último:
		//1.- Si estamos en la hoja y estamos forzando persist de hoja. Si estamos forzando
		//		la persistencia en hoja es el momento de marcar la fecha de actualizacion del registro.
		//		Este caso incluye el escenario en que en herencia sólo se modifica la root, impepinablemente
		//		se activará el update forzoso.
		//2.- Si ha habido cambios en la hoja pero no estamos forzando update y ya nos encontramos en el raiz.
		if ( ((!m_pInherit) && GetForcePersistInUpdate () == M4_TRUE)
			|| ((GetChangesIntoMainLeaf () == M4_TRUE) 
				&& (GetForcePersistInUpdate () == M4_FALSE) 				
				&& (m_bMtSentOwner) 
				&& (GetIsRootInRecursion () == M4_TRUE)) )
		{
			// La fecha de última actualización sólo se actualiza en la principal.
			oLastUpdate.Date = m_pGlobVars->GetpLConn ()->m_dDTLstUpdTrn;
			
			pReg->SetLastUpdated (oLastUpdate);
		}

		//Bug 0081643
		//Lo ponemos DESPUES de establecer la fecha de registro!!.
		//Si estamos en root y se detecta sí ha habido cambios en la herencia, persistimos la hoja!!:
		//Cuando volvemos de la recursion, el root de la recursion, que es la última que se persiste lanzara la persistencia
		//de la base, si ha habido algun cambio en la herencia y no ha habido cambios en la principal (principal es la hoja).
		//Comprobamos que no haya habido cambios en principal para evitar persistir dos veces la principal.

		//Lo podemos resetear pq en caso de que haya sido forzada la persistencia ya se habrá hecho la persistencia.
		SetForcePersistInUpdate (M4_FALSE);

		if (indReg == indRegLastWithOperation && GetIsRootInRecursion() == M4_TRUE)
		{
			SetIsRootInRecursion (M4_FALSE);
			if (m_pMainLeaf
				&& m_bExecuteChangesIntoInheritance == M4_TRUE
				&& GetChangesIntoMainLeaf () == M4_FALSE)
			{
					m4return_t resultMainLeaf = M4_SUCCESS;
					
					//Llegamos aqui si hay que persistir la hoja pq ha habido cambios en herencia y
					//todavía no se ha intentado persistir la hoja.
					//Forzamos la persistencia en Update.
					m_pMainLeaf->SetForcePersistInUpdate (M4_TRUE);

					//Mostramos información de que se va a reejecutar la persistencia en la tabla base para actualizar
					//la fecha de ultima modificacion.
					#ifdef _M4LDB_DEBUG
						M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n -- Start: Update date last update in base.");
					#endif

					//Bug 0100781
					//Me guardo el indice del registro para poder restaurar el registro al volver de la recursion.
					ClRegisterIndex OldIndex = pReg->Index;

					resultMainLeaf = m_pMainLeaf->Execute2 ();

					//Restauramos el iterador que ha podido ser cambiado a la vuelta de la recursión.
					pReg->MoveTo(OldIndex);

					#ifdef _M4LDB_DEBUG
						M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n -- Finish: Update date last update in base.\n");
					#endif

					//Al volver nos aseguramos de poner el flag de forzado a falso.
					m_pMainLeaf->SetForcePersistInUpdate (M4_FALSE);

					if (resultMainLeaf == M4_ERROR)
					{
						return M4_ERROR;
					}
			}
		}
		//---------------------------------------------------

		//Ojo. Esto tiene que ir despues de que se haya persistido!!.
		pReg->SetFlagPersisted (M4_TRUE);

		if (!m_oMtLgcTbl.m_bInvRecordSet)
		{
			if (m_eStmType != M4LDB_DELETE)
			{
				for (indParam=0; indParam<numParam; indParam++)
				{
					m4int8_t m4Type = m_pParam->GetInfoRealM4Type (indParam);

					if (m4Type == M4CL_M4_TYPE_LONG || 
						m4Type == M4CL_M4_TYPE_UNI_LONG || 
						m4Type == M4CL_M4_TYPE_BLOB || 
						m4Type == M4CL_M4_TYPE_BINARY_STRING)
					{
						// Un campo LONG no tiene valor por defecto, ¿verdad?
						continue;
					}
					else
					{
						eParamType_t eParamType = m_pParam->GetInfoParamType (indParam);

						if ((eParamType == M4LDB_PAR_ITEM) && 
							((m_eStmType == M4LDB_INSERT) ||
							(m_pParam->Difference (indParam))))
						{
							hItem = m_pParam->GetItemHandle (indParam);

							// Comprobamos que el valor persistido no es NULL
							if (m_pParam->GetLength(indParam)!=M4LDB_SQL_NULL_DATA)
							{
								m4pcchar_t	pccData = m_pParam->GetData (indParam);

								if (M4LDB_COMPARE_VALUES (pccData, (m4pcchar_t) pReg->Item[hItem].Value.GetWithOutSec (), m4Type))
								{
									m4VariantType vValue;

									if (m4Type == M4CL_M4_TYPE_DATE || m4Type == M4CL_M4_TYPE_TIMESTAMP)
									{
										vValue.Type = M4CL_CPP_TYPE_DATE;
										vValue.Data.DoubleData = *((m4double_t *)pccData);
									}
									else if(m4Type == M4CL_M4_TYPE_NUMBER)
									{
										vValue.Type = M4CL_CPP_TYPE_NUMBER;
										vValue.Data.DoubleData = *((m4double_t *)pccData);
									}
									else
									{
										vValue.Type = M4CL_CPP_TYPE_STRING_VAR;
										vValue.Data.PointerChar = (m4pchar_t)pccData;
									}

									pReg->Item[hItem].Value.SetWithOutSec (vValue, M4_TRUE);
								}
							}
							else
							{
								// Si hemos persistido un Null, hacemos un SetNull.
								pReg->Item[hItem].Value.SetNULLWithOutSec ();
							}
						}
					}
				}
			}
		}
	}

	return (result);
}


m4return_t ClPersist::Execute2 (m4VariantType *ai_pParam, m4int32_t ai_numParam, ClTblMem *ai_poTbl) //, m4int32_t ai_lMask)
{
	m4return_t result = M4_ERROR;
	ClHandle hItem(0);
	m4double_t dValue = 0;
	m4int32_t indParam;
	m4int32_t indRow;
	m4int32_t numRow = 0;
	PClRegister pReg = &m_pGlobVars->GetpAccessRecordSet ()->Register;
	m4int32_t indExtPar = 0;
	ClTblMem oTbl;
	ClTblMem *poTbl = (ai_poTbl) ? ai_poTbl : &oTbl;
	m4VariantType vAux;

	m_oMtLgcTbl.m_bValidate = m_pMtSent->HasValidation();

	m4bool_t bNoOptimization = M4_TRUE;
	if (m_pMtSent && m_pMtSent->HasValidation() == M4_FALSE)
	{
		// Para que haya optimización en la no validación tiene que ser un objeto logico con una 
		// sola tabla real y no tener herencia.
		if (m_pvcPersist->size() == 1 && !m_pMtSent->ExistsInheritance() )
		{
			bNoOptimization = M4_FALSE;
		}
	}

	/* Bug 0154924
	Si estamos en tramitación o herencia se desactiva la optimización
	*/
	if( bNoOptimization == M4_FALSE )
	{
		if(
			( m_bUseTramitation == M4_TRUE && m_oMtLgcTbl.m_iMirrorType == 1 )
			||
			( m_bUseInheritance == M4_TRUE && m_oMtLgcTbl.m_bInheritsData == M4_TRUE )
			)
		{
			bNoOptimization = M4_TRUE ;
		}
	}
	
	m4int32_t numParam = m_pParam->GetNumData ();

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\nRegister [%ld].", (m4int32_t) pReg->Index);
	#endif

	if (m_eStmType == M4LDB_INSERT && !m_pMtSent->GetIsInsertIntoSelect())
	{
		m4bool_t bRetry = M4_FALSE;

		do
		{
			// Limpiamos los parámetros.
			m_pParam->Clear ();

			for (indParam=0; indParam<numParam; indParam++)
			{
				hItem = m_pParam->GetItemHandle (indParam);

				void *pValue = NULL;
				m4int32_t lLength = 0;

				/* Bug 0261298
				Hay que saber el tipo real cuando es encriptado
				*/
				m4int8_t m4Type = m_pParam->GetInfoRealM4Type (indParam);
				m4int8_t cppType = M4CL_CPP_TYPE_NULL;

				eParamType_t eParamType = m_pParam->GetInfoParamType (indParam);

				switch (eParamType)
				{
					case M4LDB_PAR_EXTERN:
						if (indExtPar >= ai_numParam || (m4int32_t) hItem-1 >= ai_numParam)
						{
							DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_ARGUMENT_NUMBER);
							return (M4_ERROR);
						}
						// El indice del parametro viene dado por el handle del Item.
						pValue = ConvertValue (&ai_pParam[hItem-1], m4Type);

						/* Bug 0126267
						Hay que pasar la longitud del campo, como se hace cuando es un item
						*/
						cppType = ai_pParam[hItem-1].GetType() ;

						if( cppType == M4CL_CPP_TYPE_STRING_VAR )
						{
							lLength = strlen( (m4pchar_t) pValue ) ;
						}

						indExtPar++;
						break;

					case M4LDB_PAR_SESSION:
						// Lo intento leer del canal, y si no del canal sesion
						if (_ReadChannelPropertyOrSessionItem(hItem,vAux)!=M4_SUCCESS)
						{
							return (M4_ERROR);
						}
						pValue = ConvertValue (&vAux, m4Type);
						break;

					case M4LDB_PAR_ITEM:
						if (pReg->Index == M4LDB_NULL_INDEX && 
							m_pGlobVars->GetpMCR ()->GetItemScope (hItem) == M4CL_ITEM_SCOPE_REGISTER)
						{
							DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_CURRENT_IS_EOF);
							return (M4_ERROR);
						}

						pValue = pReg->Item[hItem].Value.Get ();

						lLength = pReg->Item[hItem].Value.Size ();
						break;

					default:
						continue;
				}

				if (pValue)
				{
					switch (m4Type)
					{
						default:
							m_pParam->SetData (indParam, (m4pcchar_t) pValue, lLength);
							break;

						case M4CL_M4_TYPE_BLOB:
							{ //-- Iniciamos Bloque
							//---------------------------------
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
							// -- Fin de bloque
							//---------------------------------
							break;

						case M4CL_M4_TYPE_LONG:
						case M4CL_M4_TYPE_UNI_LONG:
						case M4CL_M4_TYPE_BINARY_STRING:
							m_pParam->SetData (indParam, (m4pcchar_t) pValue, lLength);
							break;

						case M4CL_M4_TYPE_DATE:
							dValue = (m4date_t) M4LDB_NUMBER (pValue);
							m_pParam->SetData (indParam, (m4pchar_t) &dValue);
							break;

						case M4CL_M4_TYPE_TIMESTAMP:
						case M4CL_M4_TYPE_NUMBER:
							dValue = M4LDB_DOUBLE (pValue);
							m_pParam->SetData (indParam, (m4pchar_t) &dValue);
							break;
					}
				}

				#ifdef _M4LDB_DEBUG
					if (eParamType == M4LDB_PAR_ITEM || eParamType == M4LDB_PAR_SESSION)
					{
						M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\tData[%ld] Itm[%ld] = ", indParam + 1, (m4uint32_t)hItem);
						DisplayData (indParam);
					}
				#endif
			}

			result = Execute ();

			bRetry = M4_FALSE;

			if (m_pGlobVars->IsRetry ())
			{
				m_pGlobVars->SetIsRetry (M4_FALSE);
				if (m_pGlobVars->GetNumberOfAttempts () != M4LDB_NULL_INDEX)
				{
					if (m_pGlobVars->GetNumberOfAttempts ())
					{
						bRetry = M4_TRUE;
						indExtPar = 0;
					}
					m_pGlobVars->SetNumberOfAttempts (m_pGlobVars->GetNumberOfAttempts ()-1);
				}
			}
		} while (bRetry);
	}
	else
	{
		// Limpiamos los parámetros.
		m_pParam->Clear ();

		if (m_eStmType == M4LDB_UPDATE || m_eStmType == M4LDB_INSERT)
		{
			for (indParam=0; indParam<numParam; indParam++)
			{
				hItem = m_pParam->GetItemHandle (indParam);

				void *pValue = NULL;
				m4int32_t lLength = 0;

				/* Bug 0261298
				Hay que saber el tipo real cuando es encriptado
				*/
				m4int8_t m4Type = m_pParam->GetInfoRealM4Type (indParam);

				eParamType_t eParamType = m_pParam->GetInfoParamType (indParam);

				switch (eParamType)
				{
					case M4LDB_PAR_EXTERN:
						if (indExtPar >= ai_numParam || (m4int32_t) hItem-1 >= ai_numParam)
						{
							DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_ARGUMENT_NUMBER);
							return (M4_ERROR);
						}
						// El indice del parametro viene dado por el handle del Item.
						pValue = ConvertValue (&ai_pParam[hItem-1], m4Type);

						indExtPar++;
						break;

					case M4LDB_PAR_SESSION:
						// Lo intento leer del canal, y si no del canal sesion
						if (_ReadChannelPropertyOrSessionItem(hItem,vAux)!=M4_SUCCESS)
						{
							return (M4_ERROR);
						}
						pValue = ConvertValue (&vAux, m4Type);
						break;

					case M4LDB_PAR_ITEM:
						if (pReg->Index == M4LDB_NULL_INDEX && 
							m_pGlobVars->GetpMCR ()->GetItemScope (hItem) == M4CL_ITEM_SCOPE_REGISTER)
						{
							DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_CURRENT_IS_EOF);
							return (M4_ERROR);
						}

						pValue = pReg->Item[hItem].Value.Get ();

						lLength = pReg->Item[hItem].Value.Size ();
						break;

					default:
						continue;
				}

				if (pValue)
				{
					switch (m4Type)
					{
						default:
							m_pParam->SetData (indParam, (m4pcchar_t) pValue, lLength);
							break;

						case M4CL_M4_TYPE_BLOB:
							{ //-- Iniciamos Bloque
							//---------------------------------
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
							// -- Fin de bloque
							//---------------------------------
							break;

						case M4CL_M4_TYPE_LONG:
						case M4CL_M4_TYPE_UNI_LONG:
						case M4CL_M4_TYPE_BINARY_STRING:
							m_pParam->SetData (indParam, (m4pcchar_t) pValue, lLength);
							break;

						case M4CL_M4_TYPE_DATE:
							dValue = (m4date_t) M4LDB_NUMBER (pValue);
							m_pParam->SetData (indParam, (m4pchar_t) &dValue);
							break;

						case M4CL_M4_TYPE_TIMESTAMP:
						case M4CL_M4_TYPE_NUMBER:
							dValue = M4LDB_DOUBLE (pValue);
							m_pParam->SetData (indParam, (m4pchar_t) &dValue);
							break;
					}
				}

				#ifdef _M4LDB_DEBUG
					if (eParamType == M4LDB_PAR_ITEM || eParamType == M4LDB_PAR_SESSION)
					{
						M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\tData[%ld] Itm[%ld] = ", indParam + 1, (m4uint32_t)hItem);
						DisplayData (indParam);
					}
				#endif
			}
		}

		/* Bug 0123525
		Si es insert tampoco se debe hacer la optimización
		*/
		if( bNoOptimization || m_eStmType == M4LDB_UPDATE || m_eStmType == M4LDB_INSERT )
		{
			if (!ai_poTbl)
			{
				// Buscamos el cursor en el principal.
				ClLoad *pCursor = (ClLoad *) m_oMtLgcTbl.m_pMaster->FindAuxCursor (M4LDB_CURSOR_QUERY);

				if (!pCursor)
				{
					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersist::Execute" << "The cursor M4LDB_CURSOR_QUERY does not exist.");
					return (M4_ERROR);
				}

				#ifdef _M4LDB_DEBUG
					M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\nCursorQuery.");
				#endif

				result = pCursor->Execute (ai_pParam, ai_numParam - indExtPar, pReg);

				if (result != M4_SUCCESS)
				{
					return (M4_ERROR);
				}

				result = pCursor->GetData (poTbl);

				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}
			}

			numRow = poTbl->GetNumRow ();

			if (!numRow)
			{
				DUMP_LDBINSP_WARNING (m_pGlobVars->GetpLConn (), M4_CH_LDB_NO_REGISTERS_AFFECTED);
				return (M4_WARNING);
			}

			m4int32_t indCol = 0;

			//Esto hay que hacerlo para el update select, pero tb para el delete select ¡¡¡¡
			if (m_eStmType != M4LDB_INSERT)
			{
				// Esto lo hacemos sobre el vector virtual del master para obtener la pk limpia en caso de auditorias.
				m4int32_t numPar = m_oMtLgcTbl.m_pMaster->m_oVParam.size();

				for (m4int32_t indPar=0; indPar<numPar; indPar++)
				{
					m4int32_t indFld = m_oMtLgcTbl.m_pMaster->m_oVParam[indPar];

					m4bool_t bIsPK = m_pParam->GetInfoIsPK (indFld);

					if (bIsPK)
					{
						poTbl->SetIdent (indCol, indFld);

						indCol++;
					}
				}
			}

			if (m_eStmType == M4LDB_INSERT || (m_pMtSent != NULL && m_pMtSent->GetIsUpdateSelect() == M4_TRUE ))
			{
				// Esto es para vincular los campos de la subconsulta con las columnas del insert.
				m4int32_t numFld = m_pMtSent->GetNumFields ();

				m4int32_t	indFld = 0 ;
				m4int32_t	indParam = M4LDB_NULL_INDEX ;
				ClMetaField	*pFld = NULL ;

				/* Bug 0115678
				Se ponen los campos long al final
				Para ello se hacen 2 pasados
				*/
				m4int8_t	iM4Type = 0 ;

				for( indFld = 0 ; indFld < numFld ; indFld++ )
				{
					pFld = m_pMtSent->GetFieldByPos( indFld ) ;
					
					if( pFld->IsMetaData() == M4_TRUE && pFld->m_eClause == M4LDB_CLAUSE_COLUMN )
					{
						indParam = m_pParam->FindIndexByIdField( pFld->m_stField.c_str() ) ;

						/* Bug 0130948
						Si el campo no existe no se trata como long
						*/
						if( indParam == M4LDB_NULL_INDEX )
						{
							iM4Type = M4LDB_M4_UNICODE_TYPE( M4CL_M4_TYPE_FIXED_STRING ) ;
						}
						else
						{
							iM4Type = m_pParam->GetInfoM4Type( indParam ) ;
						}

						if( iM4Type != M4CL_M4_TYPE_LONG && iM4Type != M4CL_M4_TYPE_UNI_LONG && iM4Type != M4CL_M4_TYPE_BINARY_STRING )
						{
							poTbl->SetIdent( indCol, indParam ) ;
							indCol++ ;
						}
					}
				}

				for( indFld = 0 ; indFld < numFld ; indFld++ )
				{
					pFld = m_pMtSent->GetFieldByPos( indFld ) ;
					
					if( pFld->IsMetaData() == M4_TRUE && pFld->m_eClause == M4LDB_CLAUSE_COLUMN )
					{
						indParam = m_pParam->FindIndexByIdField( pFld->m_stField.c_str() ) ;

						/* Bug 0130948
						Si el campo no existe no se trata como long
						*/
						if( indParam == M4LDB_NULL_INDEX )
						{
							iM4Type = M4LDB_M4_UNICODE_TYPE( M4CL_M4_TYPE_FIXED_STRING ) ;
						}
						else
						{
							iM4Type = m_pParam->GetInfoM4Type( indParam ) ;
						}

						if( iM4Type == M4CL_M4_TYPE_LONG || iM4Type == M4CL_M4_TYPE_UNI_LONG || iM4Type == M4CL_M4_TYPE_BINARY_STRING )
						{
							poTbl->SetIdent( indCol, indParam ) ;
							indCol++ ;
						}
					}
				}
			}

			m4pchar_t pcTramitation = NULL ;

			// Si es update select se cambia la tramitación del cursor query por la actual
			if( m_eStmType == M4LDB_UPDATE && m_pMtSent != NULL && m_pMtSent->GetIsUpdateSelect() == M4_TRUE )
			{
				m_pGlobVars->GetpAccessRecordSet()->GetpAccess()->GetpChannel()->Tramitation.Get( pcTramitation );
			}

			for (indRow=0; indRow<numRow; indRow++)
			{
				m4bool_t bRetry = M4_FALSE;

				m_pGlobVars->SetNumberOfAttempts (M4LDB_NULL_INDEX);

				do
				{
					result = m_pParam->Merge (poTbl, indRow, pcTramitation);

					if (result == M4_ERROR)
					{
						return (M4_ERROR);
					}

					result = Execute ();

					bRetry = M4_FALSE;

					if (m_pGlobVars->IsRetry ())
					{
						m_pGlobVars->SetIsRetry (M4_FALSE);
						if (m_pGlobVars->GetNumberOfAttempts () != M4LDB_NULL_INDEX)
						{
							if (m_pGlobVars->GetNumberOfAttempts ())
							{
								bRetry = M4_TRUE;
								indExtPar = 0;
							}
							m_pGlobVars->SetNumberOfAttempts (m_pGlobVars->GetNumberOfAttempts ()-1);
						}
					}
				} while (bRetry);

				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}
			}
		}
		else
		{
			// Este es el delete sin validación.
			ClRegisterIndex indRealTbl(0);
			m4uint32_t numRealTbl = m_pvcPersist->size ();

			for (indRealTbl=0; indRealTbl<numRealTbl; indRealTbl++)
			{
				
				m4bool_t bAuxPersist = M4_FALSE;

				// Situamos el current del nodo SYS_DICT_REAL_OBJECTS en dicho registro.
				m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[indRealTbl];

  				#ifdef _M4LDB_DEBUG
					M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\n # Executing Real Table [%ld].", (m4int32_t) indRealTbl);
				#endif

				ClLoad *pCursor = (ClLoad *) (*m_pvcPersist)[indRealTbl]->FindAuxCursor (M4LDB_CURSOR_QUERY);

				if (!pCursor)
				{
					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersist::Execute" << "The cursor M4LDB_CURSOR_QUERY does not exist.");
					return (M4_ERROR);
				}

				result = pCursor->Execute (ai_pParam, ai_numParam - indExtPar, pReg);

				if (result != M4_SUCCESS)
				{
					return (M4_ERROR);
				}

				result = pCursor->Close ();

				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}
			}
		}
	}

	if (result != M4_ERROR)
	{
		if (m_pInherit)
		{
			result = m_pInherit->Execute2 (ai_pParam, ai_numParam, poTbl);

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}
	}

	return (result);
}


m4return_t ClPersist::Execute (ClRegister * ai_pReg, m4bool_t *ao_pbPersist, m4uint32_t ai_indOperation, m4bool_t *ao_pbInvRS)
{
	m4return_t result = M4_SUCCESS;
	ClRegisterIndex indRealTbl(0);
	m4bool_t bPersist = M4_FALSE;
	m4bool_t bNoChangesInBase = M4_FALSE;

	m4uint32_t nOperation = (ai_indOperation) ? ai_indOperation : m_pGlobVars->GetpLConn ()->m_pLdb->GetCounterOperation ();

	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[m_oMtLgcTbl.m_indLgcTbl];

	if (m_pGlobVars->GetPostValidate ())
	{
		if (m_oMtLgcTbl.m_indDTLstUpd != M4LDB_NULL_INDEX)
		{
			// Actualizamos el valor del campo fecha de última actualización.
			m_pParam->SetData (m_oMtLgcTbl.m_indDTLstUpd, (m4pchar_t) &m_pGlobVars->GetpLConn ()->m_dDTLstUpdTrn);
		}
	}

	m_oMtLgcTbl.m_bPKUpdated = M4_FALSE;

	m4uint32_t i = 0;
	m4uint32_t numRealTbl = m_pvcPersist->size ();

	m4bool_t bReverse = M4_FALSE ;

	m4int32_t iTranslationTable = -1;

	if( numRealTbl > 1 )
	{
		/*
		En herencia o tramitación con más de una tabla hay que primero primero las no base
		porque sino se puede perder el campo OPERATION que sólo está en la base
		*/
		if( ( m_bUseTramitation == M4_TRUE && m_oMtLgcTbl.m_iMirrorType == 1 )
			||
			( m_bUseInheritance == M4_TRUE && m_oMtLgcTbl.m_bInheritsData == M4_TRUE ) )
		{
			bReverse = M4_TRUE ;
		}

		for( i = 0; i < numRealTbl - 1; i++ )
		{
			indRealTbl = numRealTbl - i - 1 ;

			if( M4LDB_NUMBER( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Register[indRealTbl].Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_IS_TRANSLATION].Value.GetWithOutSec() ) == 1 )
			{
				// Se calcula la tabla de traducciones
				iTranslationTable = indRealTbl;
				break;
			}
		}
	}

	for( i = 0; i < numRealTbl; i++ )
	{
		if( bReverse == M4_FALSE )
		{
			indRealTbl = i ;
		}
		else
		{
			indRealTbl = numRealTbl - i - 1 ;

			if( iTranslationTable != -1 )
			{
				if( indRealTbl == 0 )
				{
					// La tabla de traducciones siempre es la última
					indRealTbl = iTranslationTable;
				}
				else if( indRealTbl <= iTranslationTable )
				{
					// Si es una tabla anterior o igual a la de traducciones se salta uno
					indRealTbl--;
				}
			}
		}

		m4bool_t bAuxPersist = M4_FALSE;
		m4bool_t bRetry = M4_FALSE;

		m4int32_t numParam = m_pParam->GetNumData();

		do
		{
			// Situamos el current del nodo SYS_DICT_REAL_OBJECTS en dicho registro.
			m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[indRealTbl];

			m4bool_t	bIgnore = M4_FALSE;
			m4pchar_t	pccTranslation = "";

			if( indRealTbl == iTranslationTable )
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

			if( bIgnore == M4_FALSE )
			{
  				#ifdef _M4LDB_DEBUG
					if (m_pGlobVars->GetPostValidate ())
					{
						M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\n # Executing Operation [%ld]. Real Table [%ld].%s", nOperation, (m4int32_t) indRealTbl, pccTranslation);
					}
					else
					{
						M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\n # PostValidating Operation [%ld]. Real Table [%ld].%s", nOperation, (m4int32_t) indRealTbl, pccTranslation);
					}
				#endif

				result = (*m_pvcPersist)[indRealTbl]->Execute (bAuxPersist, ai_pReg);

				bRetry = M4_FALSE;

				if( (indRealTbl || m_oMtLgcTbl.m_bAudStmt) && m_pGlobVars->IsRetry() )
				{
					// Sólo para las tablas distintas de la principal y las de auditoría.
					m_pGlobVars->SetIsRetry (M4_FALSE);
					if (m_pGlobVars->GetNumberOfAttempts () != M4LDB_NULL_INDEX)
					{
						if (m_pGlobVars->GetNumberOfAttempts ())
						{
							bRetry = M4_TRUE;
						}
						m_pGlobVars->SetNumberOfAttempts (m_pGlobVars->GetNumberOfAttempts ()-1);
					}
				}
			}

		} while (bRetry);

		if (result == M4_ERROR)
		{
			Release ();
			return (M4_ERROR);
		}

		if (!indRealTbl && m_eStmType == M4LDB_UPDATE && !bAuxPersist)
		{
			bNoChangesInBase = M4_TRUE;
		}

		if (bAuxPersist)
		{
			bPersist = M4_TRUE;
		}
	}

	if (m_pGlobVars->GetPostValidate ())
	{
		if (bPersist)
		{
			if (bNoChangesInBase)
			{
				// Ha habido un cambio en una tabla auxiliar. 
				// Tenemos que volver a validar el UPDATE en la tabla maestra.
				m4bool_t bAuxPersist = M4_TRUE;

  				#ifdef _M4LDB_DEBUG
					M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\n # Rexecuting Real Table [0].");
				#endif

				result = m_oMtLgcTbl.m_pMaster->Execute (bAuxPersist, ai_pReg);

				if (result == M4_ERROR)
				{
					Release ();
					return (M4_ERROR);
				}
			}
			
			if (m_oMtLgcTbl.m_bHasAudit)
			{
				result = m_oMtLgcTbl.m_pMaster->InsertAuditory ();
			}

			if (result == M4_ERROR)
			{
				Release ();
				return (M4_ERROR);
			}

			// PostValidamos.
			StPostValidation *pPostValidation = new StPostValidation ();

			pPostValidation->m_pAccess = m_pGlobVars->GetpAccessRecordSet ()->GetpAccess ();
			pPostValidation->m_hNode = m_pGlobVars->GethNode ();
			pPostValidation->m_indLgcTbl = m_oMtLgcTbl.m_indLgcTbl;
			pPostValidation->m_indBlock = m_pGlobVars->GetpAccessRecordSet ()->GetCurrentBlock ()->ID.BlockUniqueKey;
			pPostValidation->m_indReg = m_pGlobVars->GetpAccessRecordSet ()->Register.Index;

			result = m_pParam->Backup (&(pPostValidation->m_oParam));

			if (result == M4_ERROR)
			{
				delete pPostValidation;
				return (M4_ERROR);
			}
			
			pPostValidation->m_pStm = this;
			pPostValidation->m_indOperation = nOperation;

			m_pGlobVars->GetpLConn ()->m_plsPostValidation->push_back (pPostValidation);

			if ((m_oMtLgcTbl.m_eCurTemp == M4LDB_N_PERIOD) && 
				((m_eStmType == M4LDB_INSERT) || 
				((m_eStmType == M4LDB_UPDATE) && 
				((m_pParam->Difference (m_oMtLgcTbl.m_indDTStart)) || 
				(m_pParam->Difference (m_oMtLgcTbl.m_indDTEnd))))))
			{
				// Si tenemos un histórico, guardamos la operación en la cache de la transacción.

				m_oMtLgcTbl.m_pMaster->InsertInTransaction ();
			}
		}
	}

	if (ao_pbPersist && bPersist)
	{
		*ao_pbPersist = M4_TRUE;
	}

	if (ao_pbInvRS)
	{
		*ao_pbInvRS = m_oMtLgcTbl.m_bInvRecordSet;
	}

	if (!m_oMtLgcTbl.m_bAudStmt)
	{
		// Liberamos memoria.
		Release ();
	}

	return (M4_SUCCESS);
}



