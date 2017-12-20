//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb
// File:                clpersb2.cpp
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
#include "m4lgadapdef.hpp"
#include "defmain.hpp"
#include "clldb.hpp"
#include "clload.hpp"
#include "cltblmem.hpp"
#include "clmtsent.hpp"
#include "clpersis.hpp"
#include "cldinarr.hpp"
#include "ldb_log.hpp"
#include "traductor.hpp"
#include "tfres.hpp"
#include "m4win2ux.hpp"
#include "iexecutor.hpp"
#include "m4cryptc.hpp"

extern	M4_DECL_M4OBJGLB m4date_t    g_dTimeCorrection ;



int NewOrderByApp (const void *plhc, const void *prhc) {
	struct stHistory &lhc = *((struct stHistory *) plhc);
	struct stHistory &rhc = *((struct stHistory *) prhc);

	if (M4LDB_COMPARE_DATES (lhc.m_dDTStart, rhc.m_dDTStart) < 0) {
		return (-1);
	}
	else {
		if (!M4LDB_COMPARE_DATES (lhc.m_dDTStart, rhc.m_dDTStart)) {
			if (M4LDB_COMPARE_DATES (lhc.m_dDTStartCorr, rhc.m_dDTStartCorr) < 0) {
				return (-1);
			}
		}
	}

	return (1);
}


m4return_t ClPersistBase::Execute (m4bool_t &ao_bPersist, ClRegister * ai_pReg)
{
	m4return_t result = M4_SUCCESS;

	#ifdef _M4LDB_DEBUG
		if (m_pGlobVars->GetPostValidate ())
		{
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n#-#-----------Begin %s-----------#-#", g_aPersist[m_eStmType]);
		}
	#endif

	if (m_pMtLgcTbl->m_bValidate)
	{
		m_dCompleteDTStart = M4LDB_MINUS_INF_DATE;
		m_dCompleteDTEnd = M4LDB_PLUS_INF_DATE;

		switch (m_eStmType)
		{
			case M4LDB_INSERT:
			case M4LDB_DUPLICATE:
				ao_bPersist = M4_TRUE;
				result = ValidateInsert (ai_pReg);
				break;
	
			case M4LDB_UPDATE:
				result = ValidateUpdate (ao_bPersist, ai_pReg);				
				//Bug 0072475. Registro bloqueado en herencia.
				//Se persistira si se ha producido algún cambio en la rama de herencia.
				//De esta forma se fuerza a que se actualize la fecha de última modificacion.
				//El problema esta en un update de un campo de SOLO hijo. El padre no se entera
				//y no se actualiza la fecha.
				//Independientemente de que en este persist base se hayan producido
				//cambios, si en la herencia ha habido tendré que persistir.
				//Si no hay error comprobaré la persistencia.
				//En caso de error salimos del bucle.
				if (result == M4_SUCCESS && ao_bPersist == M4_FALSE)
				{
					if (m_pSourcePersist && m_pSourcePersist->GetForcePersistInUpdate () == M4_TRUE )
					{
						//Me pongo a validar la fecha de actualizacion.
						result = ValidateDTLastUpdate (ai_pReg);

						if (result == M4_ERROR)
						{
							//Si hay error salimos del bucle
							break;
						}
						ao_bPersist = M4_TRUE;
					}
				}
				break;
		
			case M4LDB_DELETE:
				ao_bPersist = M4_TRUE;
				result = ValidateDelete (ai_pReg);
				break;

			default:
				ao_bPersist = M4_TRUE;
				break;
		}
	}
	else
	{
		/* Bug 0151673
		En el caso de sentencias reales faltaba validar los valores
		De momento se echa para atrás por impacto
		result = ValidateValues(ai_pReg);
		*/
		ao_bPersist = M4_TRUE;
	}

	if (result == M4_SUCCESS && m_pGlobVars->GetPostValidate ())
	{
		if (ao_bPersist)
		{
			#ifdef _M4LDB_DEBUG
				M4LDB_BEGIN_DEBUG;
				M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n%s.", g_aPersist[m_eStmType]);

				switch (m_eStmType)
				{
					case M4LDB_INSERT:
					case M4LDB_DUPLICATE:
						m_pGlobVars->GetpLConn ()->m_lInsInfo++;
						break;
			
					case M4LDB_UPDATE:
						m_pGlobVars->GetpLConn ()->m_lUpdInfo++;
						break;
				
					case M4LDB_DELETE:
						m_pGlobVars->GetpLConn ()->m_lDelInfo++;
						break;
				}
				M4LDB_END_DEBUG;
			#endif

			if (result != M4_ERROR)
			{
				result = ClStmt::Execute(0);
				ClStmt::Close ();

				if( result != M4_ERROR )
				{
					if( m_indRealTbl == 0 && m_pMtLgcTbl->m_bIsCacheable == M4_TRUE )
					{
						result = ExecuteUpdateVersion() ;
					}
				}
			}
		}

		/* Bug 0155225
		En tramitación en borrado hay que hacer el insert después del execute
		*/
		if( result != M4_ERROR )
		{
			result = ValidatePost( ao_bPersist ) ;
		}
	}

	#ifdef _M4LDB_DEBUG
		if (m_pGlobVars->GetPostValidate ())
		{
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n#-#-----------End %s-----------#-#\n", g_aPersist[m_eStmType]);
		}
	#endif

 	return (result);
}


m4return_t ClPersistBase::ValidateInsert (ClRegister * ai_pReg)
{
	m4return_t	result;
	m4int32_t	indPar;
	m4int32_t	indParam;
	m4double_t	dOperation = 0;
	ClParam*	pParamTramitOp = NULL;
	ClParam*	pParamTramitDTLastUPdate = NULL;
	
	if (m_pGlobVars->GetPostValidate ())
	{
		if (m_eStmType == M4LDB_DUPLICATE) {
			result = ValidatePK (M4_TRUE);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
		}


		m4int32_t numParam = m_oVParam.size ();

		for (indPar=0; indPar<numParam; indPar++)
		{
			indParam = m_oVParam[indPar];

			if (!m_pMtLgcTbl->m_bAudStmt && 
				m_pParam->GetInfoHasSecurity (indParam) && 
				!m_pParam->IsNull (indParam))
			{
				m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Index);
				m4id_t iLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
				m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, indParam);
				m4id_t iField = m_pParam->GetInfoIdField (indParam);
				
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRITE_ACCESS_FIELD_REFUSE, LOG_CAT_AND (iField, nField) << LOG_CAT_AND (iLgcTbl, nLgcTbl));
				return (M4_ERROR);
			}
									
			result = ValidateValue (indParam, ai_pReg);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}

			result = CheckLanguage (indParam, ai_pReg);
			
			if (result == M4_ERROR) {
				return (M4_ERROR);
			}

			m4bool_t bIsPK = m_pParam->GetInfoIsPK (indParam);

			if (bIsPK && 
				(m_pParam->IsNull (indParam) || 
				m_pParam->IsEmptyString (indParam)))
			{
				m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
				m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
				m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, indParam);
				m4id_t iField = m_pParam->GetInfoIdField (indParam);

				// Para ver si vuelco el path o no .
				if (m_pParam->GetInfoParamType(indParam)==M4LDB_PAR_ITEM){
					// Me muevo al item nulo.
					m4int32_t hNode = m_pGlobVars->GetpLConn()->m_hLogNode;
					m_pGlobVars->GetpLConn ()->m_pLogAccess->Node[(ClHandle)hNode].RecordSet.Register.Item.MoveTo((ClHandle)m_pParam->GetItemHandle(indParam));

					DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_NULL_PK_VALUE, 
					LOG_CAT_AND (iLgcTbl, nLgcTbl) <<
					LOG_CAT_AND (iField, nField)
					);
				}
				else{
					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_NULL_PK_VALUE, LOG_CAT_AND (iLgcTbl, nLgcTbl) << LOG_CAT_AND (iField, nField));
				}
				return (M4_ERROR);
			}
		}


		#ifdef _M4LDB_DEBUG
			M4LDB_BEGIN_DEBUG;
			for (indPar=0; indPar<numParam; indPar++)
			{
				indParam = m_oVParam[indPar];
				M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\tData[%ld] Fld[%s] = ", indParam + 1, m_pParam->GetInfoIdField (indParam));
				DisplayData (indParam);
			}
			M4LDB_END_DEBUG;
		#endif


		if (!m_indRealTbl)
		{
			result = ValidateDates ();
				
			if (result == M4_ERROR) {
				return (M4_ERROR);
			}

			result = ValidateOrganization ();
			
			if (result == M4_ERROR) {
				return (M4_ERROR);
			}

			//Si GetPostValidate() es M4_TRUE significa que queda pendiente la postvalidacion.
			//La validación de seguridad la estamos haciendo antes de las postvalidaciones.
			//Si modificaramos el comportamiento, haciendo que se ejecutara en las postvalidaciones es decir, 
			//cuando GetPostValidate() es M4_FALSE, tendríamos solucionaríamos el siguiente escenario:
			//Tenemos filtro en tabla padre que comprueba valor en tabla hija. Si se hace antes de la postvalidacion
			//este caso da un error pq todavía no se ha insertado el registro en la tabla hija.
			//Si se hiciera en las postvalidaciones sí nos permitiría la insercion en la tabla padre pq
			//en el momento de postvalidaciones ya se han insertado los registros del padre y del hijo.

			result = ValidateSecurity ();
			
			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
		}


		result = ValidateNoPK ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}			


		m4uint32_t	iIsTranslation = 0;

		if( m_indRealTbl != 0 )
		{
			iIsTranslation = M4LDB_NUMBER( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_IS_TRANSLATION].Value.GetWithOutSec() );
		}

		if( iIsTranslation == 0 )
		{
			// Tramitación
			if( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
			{
				m4bool_t bExists = M4_FALSE ;
				m4date_t dTramDtLastUpdate = 0 ;
				m4date_t* pdtTramitDtLastUpdate = NULL; 

				if( m_indRealTbl == 0 )
				{
					// La fecha de última actualización sólo va en la tabla principal
					pdtTramitDtLastUpdate = &dTramDtLastUpdate ;
				}
				
				result = ExecuteTramitExist( bExists, pdtTramitDtLastUpdate ) ;
				
				if( result == M4_ERROR )
				{
					return( M4_ERROR ) ;
				}

				if( bExists == M4_TRUE )
				{
					/* Bug 0155225
					Si existe el registro se borra
					*/
					result = ExecuteTramitDelete() ;

					if( result == M4_ERROR )
					{
						return( M4_ERROR ) ;
					}

					//Ya existía el registro en el mundo real.
					dOperation = 2;
					pdtTramitDtLastUpdate = &dTramDtLastUpdate ; 
				}
				else
				{	
					//Nuevo de verdad en tramitación.
					dOperation = 1;
					pdtTramitDtLastUpdate = NULL ;
				}

				//Sólo para la tabla principal
				if( m_indRealTbl == 0 )
				{
					//Estamos insertando un registro en tramitación por defecto es
					// nuevo entonces TRAMIT_OPERATION = 1 pero si ya existía 
					// TRAMIT_OPERATION = 2 lo marcamos como modificado
					pParamTramitOp = GetClParamByType(M4LDB_TRAMIT_OPERATION) ;
					pParamTramitOp->SetBindParam(M4LDB_TRAMIT_OPERATION, &dOperation, M4CL_M4_TYPE_NUMBER, 1, 0);

					//0165456 se añade el TRAMIT_DT_LAST_UPDATE
					pParamTramitDTLastUPdate = GetClParamByType ( M4LDB_TRAMIT_DT_LAST_UPDATE ) ;
					pParamTramitDTLastUPdate->SetBindParam(M4LDB_TRAMIT_DT_LAST_UPDATE, pdtTramitDtLastUpdate, M4CL_M4_TYPE_TIMESTAMP, M4LDB_SIZE_TIMESTAMP, 0);

					/* Bug 0167923
					Los parámetros si que deben rellenarse para todas las tablas, no solo la padre
					*/
					//Sólo si es la padre de herencia
					if( m_idMainTable != NULL )
					{
						result = ExecuteTramitInsertObject() ;

						if( result == M4_ERROR )
						{
							return( M4_ERROR ) ;
						}
					}
				}
			}

			// Herencia de datos canales mono y multi
			if( m_bUseInheritance == M4_TRUE && m_pMtLgcTbl->m_bInheritsData == M4_TRUE )
			{
				m4bool_t bExists = M4_FALSE ;
				result = ExecuteInheritExist( bExists ) ;
				
				if( result == M4_ERROR )
				{
					return( M4_ERROR ) ;
				}

				if( bExists == M4_TRUE )
				{
					// Si existe el registro se borra
					result = ExecuteInheritDelete() ;

					if( result == M4_ERROR )
					{
						return( M4_ERROR ) ;
					}
				}
			}
		}

	
		if (m_pGlobVars->GetpLConn ()->m_bPreVal) {
			result = ValidateFKs ();

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
		}

		// Auditoría de traducciones
		if( m_pGlobVars->GetpLConn()->GetpLdb()->GetTranslationAuditory() == M4_TRUE )
		{
			result = InsertTranslationAuditory( "I" ) ;

			if( result == M4_ERROR )
			{
				return( M4_ERROR ) ;
			}
		}
	}
	else
	{
		// Esto se ejecuta en las postvalidaciones.
		if (!m_indRealTbl && m_pGlobVars->GetPostValidateOperations())
		{
			result = GetHistories ();
				
			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}

			result = ValidateTemporalityAndCorrection ();

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}

			result = ValidateCompleteness ();

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}
		else if( !m_indRealTbl )
		{
			result = GetHistory ();
				
			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}

		if (!m_pGlobVars->GetpLConn ()->m_bPreVal)
		{
			result = ValidateFKs ();

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}

		if (m_pMtLgcTbl->m_bCorrected && m_pMtLgcTbl->m_bInvComp && m_pGlobVars->GetPostValidateOperations())
		{
			// Con esto validamos que las nuevas correcciones sigan siendo validas en los hijos.
			result = ValidateFRs ();

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ValidateUpdate (m4bool_t &ao_bPersist, ClRegister * ai_pReg)
{
	m4return_t result;
	m4int32_t indPar;
	m4int32_t indParam;
	m4bool_t bPKDiff = M4_FALSE;


	m4uint32_t iIsTranslation = 0;

	if( m_indRealTbl != 0 )
	{
		iIsTranslation = M4LDB_NUMBER( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_IS_TRANSLATION].Value.GetWithOutSec() );
	}

	if( iIsTranslation == 1 && m_bUseTranslation == M4_TRUE && m_pMtLgcTbl->m_bUpdatePK == M4_TRUE )
	{
		if( m_pGlobVars->GetpLConn()->IsDynamicLanguage( m_pGlobVars->GetLanguage() ) == M4_FALSE )
		{
			return( M4_SUCCESS ) ;
		}
	}

	if (m_pGlobVars->GetPostValidate ())
	{
		m4int32_t numParam = m_oVParam.size ();

		for (indPar=0; indPar<numParam; indPar++)
		{
			indParam = m_oVParam[indPar];
			m4bool_t bIsPK = m_pParam->GetInfoIsPK (indParam);

			if (bIsPK)
			{
				if (m_pParam->IsNull (indParam))
				{
					// En tramitación hay que rellenar el ID_DATASET, que es PK, pero no tiene valor
					if( indParam == m_pMtLgcTbl->m_indDataSet && m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
					{
						if( ValidateValue( indParam, ai_pReg ) != M4_SUCCESS )
						{
							return( M4_ERROR ) ;
						}
					}

					// Para multisociedad.
					// Si el canal es mono, no tiene el campo id_organization y hay que darle valor porque es PK.
					// Lo hacemos si el parametro es UNDEF o INTERNO.
					
					eParamType_t ParType = m_pParam->GetInfoParamType(indParam);
					
					if (ParType==M4LDB_PAR_UNDEF || ParType==M4LDB_PAR_INTERN){
						// Comprobamos que sea el id_organization.
						if (m_pParam->GetInfoIdInternalField(indParam)==M4LDB_ID_ORGANIZATION){

							if (ValidateValue(indParam, ai_pReg)!=M4_SUCCESS){
								return M4_ERROR;
							}
						}
					}

					// Vuelvo a comprobar si es nulo, y ahora si damos error.
					if (m_pParam->IsNull (indParam) ||
						m_pParam->IsEmptyString (indParam)) {
						m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
						m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
						m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, indParam);
						m4id_t iField = m_pParam->GetInfoIdField (indParam);

						// Para ver si vuelco el path o no .
						if (m_pParam->GetInfoParamType(indParam)==M4LDB_PAR_ITEM){
							// Me muevo al item nulo.
							m4int32_t hNode = m_pGlobVars->GetpLConn()->m_hLogNode;
							m_pGlobVars->GetpLConn ()->m_pLogAccess->Node[(ClHandle)hNode].RecordSet.Register.Item.MoveTo((ClHandle)m_pParam->GetItemHandle(indParam));

							DUMP_LDBINSP_ERROR_AND_PATHF(m_pGlobVars->GetpLConn (), M4_CH_LDB_NULL_PK_VALUE, 
							LOG_CAT_AND (iLgcTbl, nLgcTbl) <<
							LOG_CAT_AND (iField, nField)
							);
						}
						else{
							DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_NULL_PK_VALUE, LOG_CAT_AND (iLgcTbl, nLgcTbl) << LOG_CAT_AND (iField, nField));
						}
						return (M4_ERROR);
					}
				}

				if (m_pMtLgcTbl->m_bUpdatePK && m_pParam->Difference (indParam))
				{
					m_pMtLgcTbl->m_bPKUpdated = M4_TRUE;

					if (indParam != m_pMtLgcTbl->m_indDTStart && indParam != m_pMtLgcTbl->m_indDTStartCorr)
					{
						bPKDiff = M4_TRUE;
					}
				}
			}
		}

		result = ValidatePK (M4_TRUE);

		if (result == M4_ERROR)
		{
			return (M4_ERROR);
		}

		if (result == M4_WARNING)
		{
			// Si es warning de update es por traducciones
			ao_bPersist = M4_FALSE;
			return (M4_SUCCESS);
		}

		for (indPar=0; indPar<numParam; indPar++)
		{
			m4bool_t bDiffLang = M4_FALSE;
			indParam = m_oVParam[indPar];
			m4bool_t bDifferenceCausedBySecurityFieldInLongs = M4_FALSE;

			if ((!m_pMtLgcTbl->m_bAudStmt) && 
				(m_pParam->GetInfoHasSecurity (indParam)) && 
				(m_pParam->Difference (indParam)))
			{
				//Bug 0087478
				//Si sobre un campo no se tiene permisos de acceso en persistencia, se queda marcado un flag.
				//Es decir, si el flag está a 1 significa que no tenemos permiso de acceso (en este caso de Update).
				//Si no había permisos de select, el data se habrá quedado a null, pero el olddata se queda con el 
				//valor que devuelve la select de validación de PK, por lo tanto se detecta la diferencia!.
				//Solucion:
				//Si estamos aquí es pq no tenemos permisos de escritura, vamos a preguntar si NO teníamos permisos de lectura.
				//La funcion devuelve true si NO teníamos permisos de lectura, en cuyo caso restauramos el valor original del campo.
				if ( m_pParam->GetInfoHasSecurityReadInPersist (indParam) == M4_TRUE )
				{
						//DUMP_LDBINSP_WARNINGF (m_pGlobVars->GetpLConn (), M4_CH_TF_READ_ACCESS_FIELD_REFUSE, iField <<	LOG_CAT_AND (iLgcTbl, nLgcTbl)	);
						//Si el tipo es Long debo hacerlo de otra forma.
					if (   m_pParam->GetInfoM4Type( indParam ) == M4CL_M4_TYPE_LONG
						|| m_pParam->GetInfoM4Type( indParam ) == M4CL_M4_TYPE_UNI_LONG
						|| m_pParam->GetInfoM4Type( indParam ) == M4CL_M4_TYPE_BLOB
						|| m_pParam->GetInfoM4Type( indParam ) == M4CL_M4_TYPE_BINARY_STRING )
					{
						bDifferenceCausedBySecurityFieldInLongs = M4_TRUE;
						
					}
					else 
					{
						/* Bug 0201898
						Si el old value tiene longitud -1 el valor se debe quedar a nulo
						*/
						m4int32_t lNewLength = m_pParam->GetOldLength( indParam );

						if (lNewLength != M4LDB_SQL_NULL_DATA)
						{
							m_pParam->SetData( indParam, m_pParam->GetOldData( indParam ), lNewLength, m_pParam->GetInfoM4Type( indParam ) );
						}
						else
						{
							m_pParam->SetData( indParam );
						}
					}
				}
				else
				{
					m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Index);
					m4id_t iLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
					m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, indParam);
					m4id_t iField = m_pParam->GetInfoIdField (indParam);

					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRITE_ACCESS_FIELD_REFUSE, LOG_CAT_AND (iField, nField) << LOG_CAT_AND (iLgcTbl, nLgcTbl));
					return (M4_ERROR);
				}			
			}

			m4bool_t bIsPK = m_pParam->GetInfoIsPK (indParam);
			m4int32_t hItem = m_pParam->GetItemHandle (indParam);

			//Haya diferencia pero que no sea causada por seguridad en campos long.
			if (((!bIsPK) || 
				(m_pMtLgcTbl->m_bUpdatePK)) && 
				((hItem) && 
				(m_pParam->Difference (indParam)) && (bDifferenceCausedBySecurityFieldInLongs == M4_FALSE)) || 
				(m_pParam->GetCurrentLanguageIndex (indParam) != M4LDB_NULL_INDEX)) {
				result = ValidateValue (indParam, ai_pReg);

				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}

				result = CheckLanguage (indParam, ai_pReg, &bDiffLang);

				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}

				// Solo hay que marcar para persistir si despues de chequear el lenguaje hay diferencias.
				if ((bDiffLang || m_pParam->Difference (indParam))
					&& !m_pParam->GetInfoIsReadOnly (indParam))
				{
					ao_bPersist = M4_TRUE;
				}
			}

			#ifdef _M4LDB_DEBUG
				M4LDB_BEGIN_DEBUG;
				M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\tData[%ld] Fld[%s] = ", indParam + 1, m_pParam->GetInfoIdField (indParam));

				DisplayData (indParam);

				if ((!bIsPK || m_pMtLgcTbl->m_bUpdatePK) && hItem && 
					m_pParam->Difference (indParam) && bDifferenceCausedBySecurityFieldInLongs == M4_FALSE && 
					!m_pParam->GetInfoIsReadOnly (indParam))
				{
					M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "*");
				}

				M4LDB_END_DEBUG;
			#endif
		}


		if (!ao_bPersist)
		{
			#ifdef _M4LDB_DEBUG
				M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\t No changes found.");
			#endif
			return (M4_SUCCESS);
		}

		if( iIsTranslation == 0 )
		{
			// Tramitación
			if( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
			{
				m4bool_t bExists = M4_FALSE ;
				result = ExecuteTramitExist( bExists, NULL ) ;
				
				if( result == M4_ERROR )
				{
					return( M4_ERROR ) ;
				}

				if( bExists != M4_TRUE )
				{
					// Si no existe el registro se inserta
					result = ExecuteTramitInsert() ;

					if( result == M4_ERROR )
					{
						return( M4_ERROR ) ;
					}
				}

				//Sólo para la tabla principal y si es la padre de herencia
				if( m_indRealTbl == 0 && m_idMainTable != NULL )
				{
					result = ExecuteTramitInsertObject() ;

					if( result == M4_ERROR )
					{
						return( M4_ERROR ) ;
					}
				}
			}

			// Herencia de datos canales mono y multi
			if( m_bUseInheritance == M4_TRUE && m_pMtLgcTbl->m_bInheritsData == M4_TRUE )
			{
				m4bool_t bExists = M4_FALSE ;
				result = ExecuteInheritExist( bExists ) ;
				
				if( result == M4_ERROR )
				{
					return( M4_ERROR ) ;
				}

				if( bExists != M4_TRUE )
				{
					// Si no existe el registro se inserta
					result = ExecuteInheritInsert() ;

					if( result == M4_ERROR )
					{
						return( M4_ERROR ) ;
					}
				}
			}
		}

		if (!m_indRealTbl)
		{
			result = ValidateDTLastUpdate (ai_pReg);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}

			result = ValidateDates ();
				
			if (result == M4_ERROR) {
				return (M4_ERROR);
			}

			result = ValidateOrganization ();
			
			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}

			result = ValidateSecurity ();

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}

		if (bPKDiff)
		{
			result = ValidateNoPK ();

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}

		if (m_pGlobVars->GetpLConn ()->m_bPreVal)
		{
			result = ValidateFKs ();

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}

		// Auditoría de traducciones
		if( m_pGlobVars->GetpLConn()->GetpLdb()->GetTranslationAuditory() == M4_TRUE )
		{
			if( m_pGlobVars->HasNotValidation( M4LDB_MULTITRANSLATING ) )
			{
				result = InsertTranslationAuditory( "T" ) ;
			}
			else
			{
				result = InsertTranslationAuditory( "U" ) ;
			}

			if( result == M4_ERROR )
			{
				return( M4_ERROR ) ;
			}
		}
	}
	else
	{
		// Esto se ejecuta en las postvalidaciones.
		if (!m_indRealTbl && m_pGlobVars->GetPostValidateOperations())
		{
			result = GetHistories ();

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}

			result = ValidateTemporalityAndCorrection ();

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}

			result = ValidateCompleteness ();

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}
		else if( !m_indRealTbl )
		{
			result = GetHistory ();
				
			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}

		if (!m_pGlobVars->GetpLConn ()->m_bPreVal)
		{
			result = ValidateFKs ();

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}

		if (m_pGlobVars->GetPostValidateOperations())
		{
			result = ValidateFRs ();

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ValidateDelete (ClRegister * ai_pReg)
{
	m4return_t result;
	m4int32_t indPar;
	m4int32_t indParam;

	if (m_pGlobVars->GetPostValidate ())
	{
		m4int32_t numParam = m_oVParam.size ();

		for (indPar=0; indPar<numParam; indPar++)
		{
			indParam = m_oVParam[indPar];
			m4bool_t bIsPK = m_pParam->GetInfoIsPK (indParam);

			if (bIsPK && m_pParam->IsNull (indParam))
			{
				// En tramitación hay que rellenar el ID_DATASET, que es PK, pero no tiene valor
				if( indParam == m_pMtLgcTbl->m_indDataSet && m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
				{
					if( ValidateValue( indParam, ai_pReg ) != M4_SUCCESS )
					{
						return( M4_ERROR ) ;
					}
				}

				// Para multisociedad.
				// Si el canal es mono, no tiene el campo id_organization y hay que darle valor porque es PK.
				// Lo hacemos si el parametro es UNDEF o INTERNO.
				eParamType_t ParType = m_pParam->GetInfoParamType(indParam);
				
				if (ParType==M4LDB_PAR_UNDEF || ParType==M4LDB_PAR_INTERN)
				{
					// Comprobamos que sea el id_organization.
					if (m_pParam->GetInfoIdInternalField(indParam)==M4LDB_ID_ORGANIZATION){

						if (ValidateValue(indParam, ai_pReg)!=M4_SUCCESS){
							return M4_ERROR;
						}
					}
				}

				// Vuelvo a comprobar si es nulo, y ahora si damos error.
				if (m_pParam->IsNull (indParam) || m_pParam->IsEmptyString (indParam))
				{

					m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
					m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
					m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, indParam);
					m4id_t iField = m_pParam->GetInfoIdField (indParam);

					// Para ver si vuelco el path o no .
					if (m_pParam->GetInfoParamType(indParam)==M4LDB_PAR_ITEM)
					{
						// Me muevo al item nulo.
						m4int32_t hNode = m_pGlobVars->GetpLConn()->m_hLogNode;
						m_pGlobVars->GetpLConn ()->m_pLogAccess->Node[(ClHandle)hNode].RecordSet.Register.Item.MoveTo((ClHandle)m_pParam->GetItemHandle(indParam));

						DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_NULL_PK_VALUE, 
						LOG_CAT_AND (iLgcTbl, nLgcTbl) <<
						LOG_CAT_AND (iField, nField)
						);
					}
					else
					{
						DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_NULL_PK_VALUE, LOG_CAT_AND (iLgcTbl, nLgcTbl) << LOG_CAT_AND (iField, nField));
					}
					return (M4_ERROR);
				}
			}
		}

		result = ValidatePK (M4_TRUE);

		if (result != M4_SUCCESS)
		{
			return (result);
		}

		if (!m_indRealTbl)
		{
			result = ValidateDTLastUpdate (ai_pReg);

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}

		#ifdef _M4LDB_DEBUG
			M4LDB_BEGIN_DEBUG;
			for (indPar=0; indPar<numParam; indPar++)
			{
				indParam = m_oVParam[indPar];

				M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\tData[%ld] Fld[%s] = ", indParam + 1, m_pParam->GetInfoIdField (indParam));
				DisplayData (indParam);
			}

			M4LDB_END_DEBUG;
		#endif

		if (!m_indRealTbl)
		{
			// Bug 0283881 En borrado no se validan las fechas de históricos, pero se deja la de corrección
			if (m_pMtLgcTbl->m_bCorrected)
			{
				result = ValidateCorrectedDates ();
				
				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}
			}

			result = ValidateOrganization ();
			
			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}

			result = ValidateSecurity ();

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}
	}
	else
	{
		m4uint32_t	iIsTranslation = 0;

		if( m_indRealTbl != 0 )
		{
			iIsTranslation = M4LDB_NUMBER( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_IS_TRANSLATION].Value.GetWithOutSec() );
		}

		if( iIsTranslation == 0 )
		{
			// Herencia de datos canales mono y multi
			if( m_bUseInheritance == M4_TRUE && m_pMtLgcTbl->m_bInheritsData == M4_TRUE )
			{
				/* Bug 0165767
				Si no existe el registro en el padre se borra el registro de borrado
				*/
				m4bool_t bExists = M4_FALSE ;
				result = ExecuteInheritExist( bExists ) ;
				
				if( result == M4_ERROR )
				{
					return( M4_ERROR ) ;
				}

				if( bExists != M4_TRUE )
				{
					result = ExecuteInheritDelete() ;

					if( result == M4_ERROR )
					{
						return( M4_ERROR ) ;
					}
				}
			}
		}

		if (!m_indRealTbl && m_pGlobVars->GetPostValidateOperations())
		{
			result = GetHistories ();

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}

			result = ValidateTemporalityAndCorrection ();

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}

			result = ValidateCompleteness ();

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}
		else if( !m_indRealTbl )
		{
			result = GetHistory ();
				
			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}

		if (m_pGlobVars->GetPostValidateOperations())
		{
			result = ValidateFRs ();

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}

		if( m_indRealTbl == 0 )
		{
			// Herencia de datos canales mono y multi
			// La validación de hijos se hace después de las foráneas
			if( m_bUseInheritance == M4_TRUE && m_pMtLgcTbl->m_bInheritsData == M4_TRUE )
			{
				m4bool_t bExists = M4_FALSE ;
				result = ExecuteInheritValidate( bExists ) ;
				
				if( result == M4_ERROR )
				{
					return( M4_ERROR ) ;
				}
			}
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ValidatePost (m4bool_t ai_bPersisted)
{
	m4return_t result;

	if( m_pGlobVars->GetPostValidate () )
	{
		m4uint32_t	iIsTranslation = 0;

		if( m_indRealTbl != 0 )
		{
			iIsTranslation = M4LDB_NUMBER( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_IS_TRANSLATION].Value.GetWithOutSec() );
		}

		if( iIsTranslation == 0 )
		{
			if( m_eStmType == M4LDB_DELETE && ai_bPersisted == M4_TRUE )
			{
				// Tramitación
				if( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
				{
					m4bool_t bExists = M4_FALSE ;
					result = ExecuteTramitExist( bExists, NULL ) ;
					
					if( result == M4_ERROR )
					{
						return( M4_ERROR ) ;
					}

					if( bExists == M4_TRUE )
					{
						// Si existe el registro en el padre se inserta el borrado
						result = ExecuteTramitInsert() ;

						if( result == M4_ERROR )
						{
							return( M4_ERROR ) ;
						}
					}

					//Sólo para la tabla principal y si es la padre de herencia
					if( m_indRealTbl == 0 && m_idMainTable != NULL )
					{
						result = ExecuteTramitInsertObject() ;

						if( result == M4_ERROR )
						{
							return( M4_ERROR ) ;
						}
					}
				}

				// Herencia de datos canales mono y multi
				if( m_bUseInheritance == M4_TRUE && m_pMtLgcTbl->m_bInheritsData == M4_TRUE )
				{
					/* Bug 0165767
					El borrado se inserta siempre si no es de traducción
					*/
					result = ExecuteInheritInsert() ;

					if( result == M4_ERROR )
					{
						return( M4_ERROR ) ;
					}
				}
			}
		}
		else if( m_bUseTranslation == M4_TRUE )
		{
			if( m_eStmType == M4LDB_UPDATE )
			{
				if( m_pMtLgcTbl->m_bUpdatePK && m_pMtLgcTbl->m_bPKUpdated == M4_TRUE )
				{
					m4bool_t bExists = M4_FALSE ;
					result = ExecuteTranslationValidatePK( bExists ) ;

					if( result == M4_ERROR )
					{
						return( M4_ERROR ) ;
					}

					if( bExists == M4_TRUE )
					{
						if( m_pGlobVars->GetpLConn()->IsDynamicLanguage( m_pGlobVars->GetLanguage() ) == M4_FALSE )
						{
							result = ValidateNoPK() ;

							if (result == M4_ERROR)
							{
								return (M4_ERROR);
							}
						}

						// Si cambia la pk se cambian todos los idiomas
						result = ExecuteTranslationUpdatePK() ;

						if (result == M4_ERROR)
						{
							return (M4_ERROR);
						}
					}
				}
				else if( ai_bPersisted == M4_FALSE )
				{
					#ifdef _M4LDB_DEBUG
						M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\t No Data Found");
						M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_DATA, m_pGlobVars->GetpLConn (), "\n\t No Data Found");
					#endif
				}
			}
		}
	}

	return (M4_SUCCESS);
}

	
m4return_t ClPersistBase::DefaultFunc (m4int32_t ai_indParam, ClRegister * ai_pReg, eFuncType_t ai_idDefaultFunc)
{
	m4return_t result;
	m4double_t dValue = 0;
	m4date_t dDate;
	ClLoad *pCursor = NULL;

	m_dOrdinal = 0;
	m_lOrdinal = 0;

	eFuncType_t idDefaultFunc = ai_idDefaultFunc;

	if (idDefaultFunc == M4LDB_FTN_NONE) {
		idDefaultFunc = m_pParam->GetInfoIdDefaultFunc (ai_indParam);
	}

	m4pcchar_t pArgs = m_pParam->GetInfoDefaultArgs (ai_indParam);

	switch (idDefaultFunc) {

		case M4LDB_DEF_GMT_DATE:

			ClActualDate (dDate);
			m_pParam->SetData (ai_indParam, (m4pcchar_t) &dDate, 0, M4CL_M4_TYPE_TIMESTAMP);
			return (M4_SUCCESS);
			break;

		case M4LDB_DEF_TRANSACTION_DATE:

			dDate = m_pGlobVars->GetpLConn ()->m_dDTLstUpdTrn;
			m_pParam->SetData (ai_indParam, (m4pcchar_t) &dDate, 0, M4CL_M4_TYPE_TIMESTAMP);
			return (M4_SUCCESS);
			break;

		case M4LDB_DEF_CLOSING_DATE:

			dDate = m_pMtLgcTbl->m_dClosingDate + m_pMtLgcTbl->m_dDateUnit;
			m_pParam->SetData (ai_indParam, (m4pcchar_t) &dDate, 0, M4CL_M4_TYPE_TIMESTAMP);
			return (M4_SUCCESS);
			break;

		case M4LDB_DEF_CURRENT_DATE:
			{
				ClActualDate (dDate);
				ClAccessRecordSet *pARS = m_pGlobVars->GetpLConn ()->m_pLogonAdaptor->GetSessionRecordSet ();
				// Si no hay canalsesion cogemos el de por defecto.
				if (pARS){
					dDate += M4LDB_DOUBLE (pARS->Current.Item[M4_ITEM_GMT_DELAY].Value.GetWithOutSec ());
				}
				else{
					dDate +=g_dTimeCorrection;
				}

				m_pParam->SetData (ai_indParam, (m4pcchar_t) &dDate, 0, M4CL_M4_TYPE_TIMESTAMP);
				return (M4_SUCCESS);
			}
			break;

		case M4LDB_DEF_NUMBER:
		case M4LDB_DEF_STRING:
		case M4LDB_DEF_DATE:

			m_pParam->SetData (ai_indParam, (m4pcchar_t) pArgs, 0, M4LDB_M4_UNICODE_TYPE(M4CL_M4_TYPE_FIXED_STRING));
			return (M4_SUCCESS);
			break;

		case M4LDB_DEF_ORDINAL:

			//--- Bloqueo. ---
			result = ExecuteOrdinalSelect (ai_indParam);
			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
			//----------------

			pCursor = (ClLoad *) FindAuxCursor (M4LDB_CURSOR_ORDINAL + ai_indParam);

			if (!pCursor) {
				result = PrepareOrdinal (ai_indParam);

				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}

				pCursor = (ClLoad *) FindAuxCursor (M4LDB_CURSOR_ORDINAL + ai_indParam);
			}

			#ifdef _M4LDB_DEBUG
				M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Ordinal [%ld].", ai_indParam);
			#endif

			if (!pCursor) {
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::DefaultFunc" << "The cursor M4LDB_CURSOR_ORDINAL " << LogCat << ai_indParam << LogCat << " doesn´t exist");
				return (M4_ERROR);
			}

			result = pCursor->Execute ();

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}

			result = pCursor->Fetch ();

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}

			#ifdef _M4LDB_DEBUG
				M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), "\n\t %.20G", m_dOrdinal);
			#endif

			m_dOrdinal++;

			m_pParam->SetData (ai_indParam, (m4pcchar_t) &m_dOrdinal, 0, M4CL_M4_TYPE_NUMBER);

			result = pCursor->Close ();

			return (result);
			break;

		case M4LDB_DEF_ORDINAL_PK:
		case M4LDB_DEF_ORDINAL_PK_NO_DATES:

			//--- Bloqueo. ---
			/* Bug 0235510
			Si es el campo ID_ORDER de auditoría no se bloquea, porque ya se bloquea en el ID_EXECUTION
			*/
			if( m_pParam->GetInfoIdInternalField( ai_indParam ) != M4LDB_ID_ORDER )
			{
				result = ExecuteOrdinalPK (ai_indParam, idDefaultFunc == M4LDB_DEF_ORDINAL_PK_NO_DATES);
				if (result == M4_ERROR) {
					return (M4_ERROR);
				}
			}
			//----------------

			pCursor = (ClLoad *) FindAuxCursor (M4LDB_CURSOR_ORDINAL_PK + ai_indParam);

			if (!pCursor) {
				result = PrepareOrdinalPK (ai_indParam, idDefaultFunc == M4LDB_DEF_ORDINAL_PK_NO_DATES);

				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}

				result = BindParamOrdinalPK (ai_indParam, idDefaultFunc == M4LDB_DEF_ORDINAL_PK_NO_DATES);

				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}

				pCursor = (ClLoad *) FindAuxCursor (M4LDB_CURSOR_ORDINAL_PK + ai_indParam);
			}

			#ifdef _M4LDB_DEBUG
				M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Ordinal Primary Key [%ld].", ai_indParam);
			#endif

			if (!pCursor) {
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::DefaultFunc" << "The cursor M4LDB_CURSOR_ORDINAL_PK " << LogCat << ai_indParam << LogCat << " does not exist");
				return (M4_ERROR);
			}

			result = pCursor->Execute ();

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}

			result = pCursor->Fetch ();

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}

			#ifdef _M4LDB_DEBUG
				M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), "\n\t %.20G", m_dOrdinal);
			#endif

			m_dOrdinal++;

			m_pParam->SetData (ai_indParam, (m4pcchar_t) &m_dOrdinal, 0, M4CL_M4_TYPE_NUMBER);

			result = pCursor->Close ();

			return (result);
			break;

		case M4LDB_DEF_SESSION_ITEM:
			{
				/* Bug 0167958
				Se busca primero en el canal
				*/
				m4VariantType vValue;
				ClChannel* 	pChannel = m_pGlobVars->GetpAccessRecordSet()->GetpAccess()->GetpChannel();

				if( pChannel->Property.GetPropValue( (m4pchar_t)pArgs, vValue ) == M4_SUCCESS && vValue.Type != M4CL_CPP_TYPE_NULL )
				{
					if( vValue.Type == M4CL_CPP_TYPE_NUMBER )
					{
						m_pParam->SetData (ai_indParam, (m4pcchar_t) &vValue.Data.DoubleData, 0, M4CL_M4_TYPE_NUMBER);
					}
					else if( vValue.Type == M4CL_CPP_TYPE_DATE )
					{
						m_pParam->SetData (ai_indParam, (m4pcchar_t) &vValue.Data.DoubleData, 0, M4CL_M4_TYPE_DATE);
					}
					else
					{
						m_pParam->SetData (ai_indParam, (m4pcchar_t) vValue.Data.PointerChar, 0, M4LDB_M4_UNICODE_TYPE(M4CL_M4_TYPE_FIXED_STRING));
					}

					return (M4_SUCCESS);
				}

				// Si no está en el canal se busca en el canal sesión
				ClAccessRecordSet *pARS = m_pGlobVars->GetpLConn ()->m_pLogonAdaptor->GetSessionRecordSet ();
				
				// Si no hay canalsesion no hacemos nada
				if (pARS)
				{
					m4bool_t bIsNotEof = pARS->Current.Item.MoveTo(pArgs);

					if( bIsNotEof == M4_TRUE )
					{
						m4uint8_t iType = pARS->Current.Item.ItemDef.M4Type() ;
						void*	pValue = pARS->Current.Item.Value.GetWithOutSec ();

						m_pParam->SetData (ai_indParam, (m4pcchar_t) pValue, 0, iType);
					}
				}
				return (M4_SUCCESS);
			}
			break;

		case M4LDB_DEF_PARENT_ORG_LEVEL:
			{
				m4pcchar_t		pcOrgItem = NULL;
				m4pcchar_t		pcOrganization = NULL;
				m4VariantType	vValue;
				

				pcOrgItem = strchr( pArgs, '|' );
				
				// Primero miramos si por argumento nos han indicado el item que tiene la sociedad
				if( pcOrgItem != NULL )
				{
					ClAccessRecordSet	*pARS = m_pGlobVars->GetpAccessRecordSet();

					if( pARS )
					{
						m4bool_t bIsNotEof = ai_pReg->Item.MoveTo( ++pcOrgItem );

						if( bIsNotEof == M4_TRUE )
						{
							pcOrganization = (m4pcchar_t)ai_pReg->Item.Value.GetWithOutSec();
						}
					}
				}
				
				if( pcOrganization == NULL || *pcOrganization == '\0' )
				{
					// No se ha indicado ningún item, así que buscamos la propiedad sociedad en el canal
					ClChannel* 	pChannel = m_pGlobVars->GetpAccessRecordSet()->GetpAccess()->GetpChannel();

					if( pChannel->Property.GetPropValue( "ID_ORGANIZATION", vValue ) == M4_SUCCESS && vValue.Type != M4CL_CPP_TYPE_NULL )
					{
						pcOrganization = vValue.Data.PointerChar;
					}
					else
					{
						// Si no está en el canal se busca en el canal sesión
						pcOrganization = m_pGlobVars->GetpLConn()->m_pLogonAdaptor->GetOrganization();
					}
				}
				
				if( pcOrganization != NULL && *pcOrganization != '\0' )
				{
					m4pcchar_t pcParentOrganization = m_pGlobVars->GetpLConn()->FindParentOrg( pcOrganization, atoi( pArgs ), M4_FALSE ) ;

					m_pParam->SetData( ai_indParam, pcParentOrganization, 0, M4LDB_M4_UNICODE_TYPE(M4CL_M4_TYPE_FIXED_STRING) );
				}

				return (M4_SUCCESS);
			}
			break;
	}

	DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_DEFAULT_FUNCTION_NOT_FOUND, idDefaultFunc << m_pParam->GetInfoIdField (ai_indParam));
	return (M4_ERROR);
}


m4return_t ClPersistBase::ConstraintFunc (m4int32_t ai_indParam)
{
	string stPKRegister(M4LDB_EMPTY);
	m4return_t result;
	m4pchar_t pToken = NULL;
	m4char_t aArgs[M4LDB_SIZE_DEFAULT_STRING];
	m4double_t dAux = 0;
	m4double_t dAux1 = 0, dAux2 = 0;

	eFuncType_t idConstraintFunc = m_pParam->GetInfoIdConstraintFunc (ai_indParam);
	m4pcchar_t pArgs = m_pParam->GetInfoConstraintArgs (ai_indParam);
	m4pcchar_t pValue = m_pParam->GetData (ai_indParam);
	m4double_t dValue = 0;

	if (pArgs[0] == '\0') {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_ARGUMENT, pArgs << idConstraintFunc << m_pParam->GetInfoIdField (ai_indParam));
		return (M4_ERROR);
	}
	else {

		// Hacemos una copia de los argumentos para la función strtok.
		strcpy (aArgs, pArgs);
		m4char_t *pszLasts;

		switch (idConstraintFunc) {

			case M4LDB_CHK_NUMBER:
				dValue = *((m4double_t *) pValue);

				// Buscamos el primer token.
				pToken = M4_strtok (aArgs, "|", &pszLasts);

				while (pToken != NULL) {

					// Vamos a leer el valor.
					result = sscanf (pToken, "%lG", &dAux);

					if (result != 1) {
						strcpy (aArgs, pArgs);						
						for (m4pchar_t p = aArgs;*p;p++) {
							if (*p == '|') {
								*p = ',';
							}
						}
						pArgs = aArgs;
						DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_ARGUMENT, pArgs << idConstraintFunc << m_pParam->GetInfoIdField (ai_indParam));
						return (M4_ERROR);
					}

					// Hacemos la validación.
					if (dValue == dAux) {
						return (M4_SUCCESS);
					}

					// Obtenemos el siguiente token.
					pToken = M4_strtok (NULL, "|", &pszLasts);
				}

				break;

			case M4LDB_CHK_STRING:
				// Buscamos el primer token.
				pToken = M4_strtok (aArgs, "|", &pszLasts);
				while (pToken != NULL) {

					// Hacemos la validación.
					if (!strcmp (pValue, pToken)) {
						return (M4_SUCCESS);
					}

					// Obtenemos el siguiente token.
					pToken = M4_strtok (NULL, "|", &pszLasts);
				}

				break;

			case M4LDB_CHK_DATE:
				dValue = M4LDB_DOUBLE (pValue);

				// Buscamos el primer token.
				pToken = M4_strtok (aArgs, "|", &pszLasts);
				while (pToken != NULL) {

					dAux = M4LDB_DOUBLE (pToken);

					// Hacemos la validación.
					if (!M4LDB_COMPARE_DATES (dValue, dAux)) {
						return (M4_SUCCESS);
					}

					// Obtenemos el siguiente token.
					pToken = M4_strtok (NULL, "|", &pszLasts);
				}

				break;

			case M4LDB_CHK_INTERVAL_NUMBER:
				dValue = *((m4double_t *) pValue);

				// Leemos los dos valores.
				result = sscanf (aArgs, "%lG|%lG", &dAux1, &dAux2);

				if (result != 2) {
					strcpy (aArgs, pArgs);						
					for (m4pchar_t p = aArgs;*p;p++) {
						if (*p == '|') {
							*p = ',';
						}
					}
					pArgs = aArgs;
					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_ARGUMENT, pArgs << idConstraintFunc << m_pParam->GetInfoIdField (ai_indParam));
					return (M4_ERROR);
				}

				// Hacemos la validación.
				if ((dValue >= dAux1) && 
					(dValue <= dAux2)) {
					return (M4_SUCCESS);
				}

				break;

			case M4LDB_CHK_INTERVAL_DATE:
				dValue = M4LDB_DOUBLE (pValue);

				// Buscamos el primer token.
				pToken = M4_strtok (aArgs, "|", &pszLasts);

				if (!pToken) {
					strcpy (aArgs, pArgs);						
					for (m4pchar_t p = aArgs;*p;p++) {
						if (*p == '|') {
							*p = ',';
						}
					}
					pArgs = aArgs;
					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_ARGUMENT, pArgs << idConstraintFunc << m_pParam->GetInfoIdField (ai_indParam));
					return (M4_ERROR);
				}

				ClStringToDate (pToken, dAux1);

				// Obtenemos el siguiente token.
				pToken = M4_strtok (NULL, "|", &pszLasts);

				if (!pToken) {
					strcpy (aArgs, pArgs);						
					for (m4pchar_t p = aArgs;*p;p++) {
						if (*p == '|') {
							*p = ',';
						}
					}
					pArgs = aArgs;
					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_ARGUMENT, pArgs << idConstraintFunc << m_pParam->GetInfoIdField (ai_indParam));
					return (M4_ERROR);
				}

				ClStringToDate (pToken, dAux2);

				// Hacemos la validación.
				if ((M4LDB_COMPARE_DATES (dValue, dAux1) >= 0) && 
					(M4LDB_COMPARE_DATES (dValue, dAux2) <= 0)) {
					return (M4_SUCCESS);
				}

				break;

			default:
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_CONSTRAINT_FUNCTION_NOT_FOUND, idConstraintFunc << m_pParam->GetInfoIdField (ai_indParam));
				return (M4_ERROR);
				break;

		}
	}

	strcpy (aArgs, pArgs);						
	for (m4pchar_t p = aArgs;*p;p++) {
		if (*p == '|') {
			*p = ',';
		}
	}
	pArgs = aArgs;
	string stnPKRegister(M4LDB_EMPTY);
	string stiPKRegister(M4LDB_EMPTY);
	ComposePK (stnPKRegister, stiPKRegister);
	m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
	m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
	m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, ai_indParam);
	m4id_t iField = m_pParam->GetInfoIdField (ai_indParam);

	DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTEGRITY_CONSTRAINT_VIOLATION, 
		LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << 
		LOG_CAT_AND (iLgcTbl, nLgcTbl) <<
		LOG_CAT_AND (iField, nField) << 
		pArgs
		);
	return (M4_ERROR);
}


m4return_t ClPersistBase::ValidateDTLastUpdate (ClRegister * ai_pReg)
{
	if( m_pMtLgcTbl->m_indDTLstUpd != M4LDB_NULL_INDEX )
	{
		if( ai_pReg != NULL )
		{
			VersionMark dDate;
			ai_pReg->GetLastUpdated (dDate);

			m4date_t dDTLstUpdRAM = dDate.Date;
			m4date_t dDTLstUpdRegDB = M4LDB_DOUBLE (m_pParam->GetOldData (m_pMtLgcTbl->m_indDTLstUpd));

			if( dDTLstUpdRAM > 0 && dDTLstUpdRegDB > 0 )
			{
				if( M4LDB_COMPARE_DATES( dDTLstUpdRegDB, dDTLstUpdRAM ) > 0 )
				{
					// Actualizamos la fecha de última actualización para que pueda volverse a grabar.
					m4bool_t bWarning = M4_FALSE;
					if( ai_pReg->GetpAccess()->GetpCMCR()->GetChannelCheckConcurrency() )
					{
						m4uint32_t iRecordID = 0;
						bWarning = M4_TRUE;

						//Pedro
						//Bug 0059536, 0059483
						//El problema está en que devolvemos error, por lo tanto, la transacción
						//C/S se abortaría, pero si el M4O no es separable, toda la información
						//está a mano y a pesar de que se aborte la transacción, la fecha ya habrá 
						//sido modificada.
						//Si la fecha la actualizamos aquí, garantizamos que sólo se actualiza
						//si el chequeo de concurrencia es M4_TRUE.
						//Si el flag es M4_TRUE estamos indicando que, en caso de concurrencia, 
						//si dos usuarios modifican datos en un M4O levantado, la primera vez
						//dará un warning, pero la segunda vez te permite guardar.
						//Si el flag es M4_FALSE no nos permitirá guardar, dando siempre error.
						dDate.Date = dDTLstUpdRegDB;
						// Ahora sí que vuelve, pero se lo notifico al canal.
						ai_pReg->SetLastUpdated (dDate);
						//Fin Pedro

						if( ai_pReg->GetRecordID(iRecordID) == M4_SUCCESS )
						{
							// Si no tenemos RecordID no podemos hacerlo.
							ai_pReg->GetpAccess()->GetpChannel()->SetNewRecordVersion(iRecordID,dDTLstUpdRegDB);
						}
					}

					if( m_pGlobVars->HasNotValidation( M4LDB_INV_CONCURRENCY ) )
					{
						#ifdef _M4LDB_DEBUG
							M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Validate concurrency desactivated.");
						#endif
					}
					else
					{
						string stnPKRegister(M4LDB_EMPTY);
						string stiPKRegister(M4LDB_EMPTY);
						ComposePK (stnPKRegister, stiPKRegister);
						m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
						m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;

						if (bWarning)
						{
							DUMP_LDBINSP_WARNING_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_TF_CONCURRENCY_USERS, LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << LOG_CAT_AND (iLgcTbl, nLgcTbl) << dDTLstUpdRegDB << dDTLstUpdRAM);
						}
						else
						{
							DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_CONCURRENCY_USERS, LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << LOG_CAT_AND (iLgcTbl, nLgcTbl) << dDTLstUpdRegDB << dDTLstUpdRAM);
						}

						return (M4_ERROR);
					}
				}
			}
		}

		if( m_pGlobVars->HasNotValidation( M4LDB_INV_WRITE_DT_LAST_UPDATE ) || m_pGlobVars->HasNotValidation( M4LDB_MULTITRANSLATING ) )
		{
			#ifdef _M4LDB_DEBUG
				M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Write Last Update Date desactivated.");
			#endif

			if( m_pParam->IsOldNull(m_pMtLgcTbl->m_indDTLstUpd) == M4_TRUE )
			{
				m_pParam->SetNull(m_pMtLgcTbl->m_indDTLstUpd);
			}
			else
			{
				m_pParam->SetData (m_pMtLgcTbl->m_indDTLstUpd, m_pParam->GetOldData(m_pMtLgcTbl->m_indDTLstUpd));
			}
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ValidateDates (void)
{
	m4return_t result;

	if ((m_pMtLgcTbl->m_eCurTemp == M4LDB_1_PERIOD) || 
		(m_pMtLgcTbl->m_eCurTemp == M4LDB_N_PERIOD)) {

		m4date_t dDTStart = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTStart));
			
		m4date_t dDTEnd = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTEnd));

		if ((M4LDB_COMPARE_DATES (dDTStart, M4LDB_MINUS_INF_DATE) < 0) || 
			(M4LDB_COMPARE_DATES (dDTStart, M4LDB_PLUS_INF_DATE) > 0)) {
			string stnPKRegister(M4LDB_EMPTY);
			string stiPKRegister(M4LDB_EMPTY);
			ComposePK (stnPKRegister, stiPKRegister);
			m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
			m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
			m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTStart);
			m4id_t iField = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTStart);

			DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_DATE,
				LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << 
				LOG_CAT_AND (iLgcTbl, nLgcTbl) <<
				LOG_CAT_AND (iField, nField)
				);
			return (M4_ERROR);
		}

		if ((M4LDB_COMPARE_DATES (dDTEnd, M4LDB_MINUS_INF_DATE) < 0) || 
			(M4LDB_COMPARE_DATES (dDTEnd, M4LDB_PLUS_INF_DATE) > 0)) {
			string stnPKRegister(M4LDB_EMPTY);
			string stiPKRegister(M4LDB_EMPTY);
			ComposePK (stnPKRegister, stiPKRegister);
			m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
			m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
			m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTEnd);
			m4id_t iField = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTEnd);

			DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_DATE,
				LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << 
				LOG_CAT_AND (iLgcTbl, nLgcTbl) <<
				LOG_CAT_AND (iField, nField)
				);
			return (M4_ERROR);
		}

		if (M4LDB_COMPARE_DATES (dDTStart, dDTEnd) > 0) {
			string stnPKRegister(M4LDB_EMPTY);
			string stiPKRegister(M4LDB_EMPTY);
			ComposePK (stnPKRegister, stiPKRegister);
			m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
			m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
			m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTEnd);
			m4id_t iField = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTEnd);
			m4id_t nField2 = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTStart);
			m4id_t iField2 = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTStart);

			DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_PERIOD_DATES, 
				LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << 
				LOG_CAT_AND (iLgcTbl, nLgcTbl) << 
				LOG_CAT_AND (iField, nField) <<
				LOG_CAT_AND (iField2, nField2)
				);
			return (M4_ERROR);
		}
	}

	if (m_pMtLgcTbl->m_bCorrected) {		
		result = ValidateCorrectedDates ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ValidateValue (m4int32_t ai_indParam, ClRegister * ai_pReg)
{
	m4return_t result = M4_SUCCESS;

	// Si estamos en auditoría no validamos nada (salvo el ID_ORDER) puesto que suponemos
	// que en la DDBB está todo correcto.
	if (m_pMtLgcTbl->m_bAudStmt)
	{
		if (!m_indRealTbl)
		{
			eFldType_t eIntFld = m_pParam->GetInfoIdInternalField (ai_indParam);

			if (eIntFld == M4LDB_ID_ORDER)
			{
				result = DefaultFunc (ai_indParam, ai_pReg, M4LDB_DEF_ORDINAL_PK);
			}
			else
			{
				if (eIntFld == M4LDB_DT_OPERATION)
				{
					// Dejamos que tome por defecto la fecha que tenga.
					result = DefaultFunc (ai_indParam, ai_pReg);
				}
			}
		}
		return (result);
	}

	m4int8_t m4type = m_pParam->GetInfoM4Type (ai_indParam);
	
	if (m4type == M4CL_M4_TYPE_LONG || m4type == M4CL_M4_TYPE_UNI_LONG || m4type == M4CL_M4_TYPE_BLOB)
	{
		return (M4_SUCCESS);
	}

	// ¿Es nulo el parámetro?
	if (m_pParam->IsNull (ai_indParam))
	{
		// Es nulo.
		eFuncType_t idDefaultFunc = m_pParam->GetInfoIdDefaultFunc (ai_indParam);

		if( ai_indParam == m_pMtLgcTbl->m_indDataSet && m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
		{
			m4pchar_t pcTramitation = NULL;
			m_pGlobVars->GetpAccessRecordSet()->GetpAccess()->GetpChannel()->Tramitation.Get( pcTramitation ) ;

			if( pcTramitation != NULL )
			{
				m_pParam->SetData (ai_indParam, pcTramitation, 0, M4LDB_M4_UNICODE_TYPE( M4CL_M4_TYPE_FIXED_STRING ));

				// Ya no nos hace falta la función por defecto.
				idDefaultFunc = M4LDB_FTN_NONE;
			}
		}

		if (ai_indParam == m_pMtLgcTbl->m_indOrgType && m_pMtLgcTbl->m_eCurOrg == M4LDB_MULTI_ORGANIZATION)
		{
			m4double_t dOrgType;
			m_pGlobVars->GetpAccessRecordSet ()->GetpAccess ()->GetpChannel ()->OrganizationType.Get (dOrgType);

			if ((m4int32_t) dOrgType != M4LDB_UNDEF_ORGANIZATION)
			{
				m4VariantType vOrganization;
				ClChannel *pChannel = m_pGlobVars->GetpAccessRecordSet()->GetpAccess()->GetpChannel();

				pChannel->Organization.Get (vOrganization);
				m4pcchar_t	pccOrg = vOrganization.Data.PointerChar;

				if( pccOrg != NULL )
				{
					m4int32_t iTableLevel = m_pMtLgcTbl->m_iOrgLevel ;

					if( m_pMtLgcTbl->m_bInheritsData == M4_FALSE )
					{
						// Si la tabla no es de herencia se aplica la organización del nivel de la tabla
						pccOrg = m_pGlobVars->GetpLConn()->FindParentOrg( pccOrg, iTableLevel, M4_TRUE ) ;
					}
					else
					{
						m4int32_t iOrgLevel = m_pGlobVars->GetpLConn()->FindOrgLevel( pccOrg ) ;

						// Si es de herencia y la sociedad es menor que la de la tabla también se aplica la organización del nivel de la tabla
						if( iTableLevel > iOrgLevel )
						{
							pccOrg = m_pGlobVars->GetpLConn()->FindParentOrg( pccOrg, iTableLevel, M4_TRUE ) ;
						}
					}

					if( pccOrg == NULL )
					{
						return M4_ERROR;
					}
				}

				m_pParam->SetData (ai_indParam, pccOrg, 0, M4LDB_M4_UNICODE_TYPE( M4CL_M4_TYPE_FIXED_STRING ));

				// Ya no nos hace falta la función por defecto.
				idDefaultFunc = M4LDB_FTN_NONE;
			}
		}

		// ¿Tiene función por defecto?
		if (idDefaultFunc != M4LDB_FTN_NONE)
		{
			// El parámetro tiene función por defecto.

			result = DefaultFunc (ai_indParam, ai_pReg);

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}

		// ¿Sigue siendo nulo?
		if (m_pParam->IsNull (ai_indParam))
		{
			// Es nulo.
			m4bool_t bIsNotNull = m_pParam->GetInfoIsNotNull (ai_indParam);
		 
			// ¿Es no nulo?
			if (bIsNotNull)
			{
				// El parámetro es no nulo.
				if (m_pGlobVars->HasNotValidation (M4LDB_INV_NOT_NULL)) {
					#ifdef _M4LDB_DEBUG
						M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Validate Not null desactivated.");
					#endif
				}
				else
				{
					if (m_pParam->GetInfoIsPK (ai_indParam))
					{
						// Es PK.
						m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
						m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
						m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, ai_indParam);
						m4id_t iField = m_pParam->GetInfoIdField (ai_indParam);
						// Para ver si vuelco el path o no .
						if (m_pParam->GetInfoParamType(ai_indParam)==M4LDB_PAR_ITEM){
							// Me muevo al item nulo.
							m4int32_t hNode = m_pGlobVars->GetpLConn()->m_hLogNode;
							m_pGlobVars->GetpLConn ()->m_pLogAccess->Node[(ClHandle)hNode].RecordSet.Register.Item.MoveTo((ClHandle)m_pParam->GetItemHandle(ai_indParam));

							DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_NULL_PK_VALUE, 
								LOG_CAT_AND (iLgcTbl, nLgcTbl) <<
								LOG_CAT_AND (iField, nField)
								);
						}
						else
						{
							DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_NULL_PK_VALUE, LOG_CAT_AND (iLgcTbl, nLgcTbl) << LOG_CAT_AND (iField, nField));
						}
						return (M4_ERROR);
					}

					if(	m_pParam->GetInfoIdInternalField( ai_indParam ) != M4LDB_TRANS_FLD
						|| 
						(
							m_pParam->GetItemHandle( ai_indParam ) != 0
							&&
							(	// Bug 0287700 En multitraducción sólo se chequea para el idioma por defecto
								m_pGlobVars->HasNotValidation( M4LDB_MULTITRANSLATING ) == M4_FALSE
								||
								m_pGlobVars->GetLanguage() == m_pGlobVars->GetpLConn()->GetpLdb()->GetDefaultLanguage()
							)
						))
					{
						// No es uno de traducción.
						string stnPKRegister(M4LDB_EMPTY);
						string stiPKRegister(M4LDB_EMPTY);
						ComposePK (stnPKRegister, stiPKRegister);
						m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
						m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
						m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, ai_indParam);
						m4id_t iField = m_pParam->GetInfoIdField (ai_indParam);

						DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_NOT_NULL_CONSTRAINT_VIOLATION, 
							LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << 
							LOG_CAT_AND (iLgcTbl, nLgcTbl) <<
							LOG_CAT_AND (iField, nField)
							);
						return (M4_ERROR);
					}
				}
			}
		}
	}

	if (!m_pParam->IsNull (ai_indParam) && 
		(m_pParam->GetInfoM4Type (ai_indParam) == M4CL_M4_TYPE_DATE || 
		m_pParam->GetInfoM4Type (ai_indParam) == M4CL_M4_TYPE_TIMESTAMP))
	{
		m4date_t dDate = M4LDB_DOUBLE (m_pParam->GetData (ai_indParam));

		if ((M4LDB_COMPARE_DATES (dDate, M4LDB_MINUS_INF_DATE) < 0) || 
			(M4LDB_COMPARE_DATES (dDate, M4LDB_PLUS_INF_DATE) > 0)) {
			string stnPKRegister(M4LDB_EMPTY);
			string stiPKRegister(M4LDB_EMPTY);
			ComposePK (stnPKRegister, stiPKRegister);
			m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
			m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
			m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, ai_indParam);
			m4id_t iField = m_pParam->GetInfoIdField (ai_indParam);

			DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_DATE,
				LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << 
				LOG_CAT_AND (iLgcTbl, nLgcTbl) <<
				LOG_CAT_AND (iField, nField)
				);
			return (M4_ERROR);
		}
	}

	
	result = m_pParam->ConvertData (ai_indParam);

	if (result == M4_ERROR)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_CONVERT_FUNCTION_NOT_FOUND, m_pParam->GetInfoIdConvertFunc (ai_indParam) << m_pParam->GetInfoIdField (ai_indParam));
		return (M4_ERROR);
	}

	eFuncType_t idConstraintFunc = m_pParam->GetInfoIdConstraintFunc (ai_indParam);

	// ¿Tiene función de validación?
	if (idConstraintFunc != M4LDB_FTN_NONE && !m_pParam->IsNull (ai_indParam))
	{
		result = ConstraintFunc (ai_indParam);

		if (result == M4_ERROR)
		{
			return (M4_ERROR);
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ValidateValues (ClRegister * ai_pReg)
{
	m4return_t result;
	m4int32_t indPar;
	m4int32_t indParam;
	
	m4int32_t numParam = m_oVParam.size ();

	for (indPar=0; indPar<numParam; indPar++) {

		indParam = m_oVParam[indPar];

		result = ValidateValue (indParam, ai_pReg);

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}
	}

	return (M4_SUCCESS);
}

m4return_t ClPersistBase::ValidatePK (m4bool_t ai_bFirstTime)
{
	m4return_t result;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Validate Primary Key.");
	#endif

	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_VALIDATE_PK);

	if (!pCursor)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::ValidatePK" << "The cursor M4LDB_VALIDATE_PK does not exist");
		return (M4_ERROR);
	}

	result = pCursor->Execute ();

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	m4bool_t bNoData = M4_FALSE;

	if (!pCursor->m_bVerifyData)
	{
		m4int32_t numRow = 0;

		result = pCursor->GetData ((ClTblMem *) NULL, &numRow);

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		if (!numRow)
		{
			bNoData = M4_TRUE;
		}
	}
	else
	{
		result = pCursor->Fetch ();

		if (result == M4LDB_SQL_NO_DATA_FOUND)
		{
			bNoData = M4_TRUE;
		}
		else
		{
			result = pCursor->Close ();
		}
	}

	if( bNoData == M4_TRUE)
	{
		m4uint32_t	iIsTranslation = 0;

		if( ai_bFirstTime == M4_TRUE )
		{
			if( m_indRealTbl != 0 )
			{
				iIsTranslation = M4LDB_NUMBER( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_IS_TRANSLATION].Value.GetWithOutSec() );
			}

			if( iIsTranslation == 1 )
			{
				if( m_eStmType == M4LDB_UPDATE && m_bUseTranslation == M4_TRUE )
				{
					// Si no existe el registro se inserta y se vuelve a validar
					result = ExecuteTranslationInsert();

					if( result == M4_ERROR )
					{
						return( M4_ERROR );
					}

					return ValidatePK( M4_FALSE );
				}
				return (M4_WARNING);
			}
		}

		string stnPKRegister(M4LDB_EMPTY);
		string stiPKRegister(M4LDB_EMPTY);
		ComposePK (stnPKRegister, stiPKRegister);
		m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
		m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;

		if (m_eStmType == M4LDB_DELETE)
		{
			DUMP_LDBINSP_WARNING_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_PRIMARY_KEY_NOT_FOUND, LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << LOG_CAT_AND (iLgcTbl, nLgcTbl));
			return (M4_WARNING);
		}

		DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_PRIMARY_KEY_NOT_FOUND, LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << LOG_CAT_AND (iLgcTbl, nLgcTbl));
		return (M4_ERROR);
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ValidateNoPK (void)
{
	m4return_t result;

	if (m_eStmType == M4LDB_INSERT && m_pGlobVars->HasNotValidation (M4LDB_INV_NO_PK))
	{
		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Validate No Primary Key desactivated.");
		#endif
		return (M4_SUCCESS);
	}

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Validate No Primary Key.");
	#endif

	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_VALIDATE_NO_PK);

	if (!pCursor) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::ValidateNoPK" << "The cursor M4LDB_VALIDATE_NO_PK does not exist");
		return (M4_ERROR);
	}

	m_dProjection = 1;

	result = pCursor->Execute ();

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	result = pCursor->Fetch ();

	if (result == M4_ERROR) {
		result = pCursor->Close ();
		return (M4_ERROR);
	}

	if (result != M4LDB_SQL_NO_DATA_FOUND) {
		pCursor->Close ();
		if ((m_pMtLgcTbl->m_indRegType != M4LDB_NULL_INDEX) && 
			(!m_indRealTbl) && 
			(m_dProjection == 3)) {
//			pCursor->Close ();
			// El registro es una projección por lo que procedemos a borrarla.
			m4date_t dDTStart = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTStart));

			m4date_t dDTStartCorr = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTStartCorr));

			struct stHistory oReg;
			oReg.m_dDTStart = dDTStart;
			oReg.m_dDTStartCorr = dDTStartCorr;
			if (m_pMtLgcTbl->m_indDTEnd != M4LDB_NULL_INDEX) {
				oReg.m_dDTEnd = M4LDB_DOUBLE (m_pParam->GetData(m_pMtLgcTbl->m_indDTEnd));
			}			
			oReg.m_eRegType = (eRegType_t) M4LDB_NUMBER (m_pParam->GetData(m_pMtLgcTbl->m_indRegType));

			result = DeleteProjection (oReg);
			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
		}
		else {
			string stnPKRegister(M4LDB_EMPTY);
			string stiPKRegister(M4LDB_EMPTY);
			ComposePK (stnPKRegister, stiPKRegister);
			m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
			m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;

			DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_PRIMARY_KEY_CONSTRAINT_VIOLATION, 
				LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << 
				LOG_CAT_AND (iLgcTbl, nLgcTbl)
				);
// Me lo llevo arriba; En SQLServer, si al volcar el error, se carga el m4trans, da error de resultados pendientes.
//			result = pCursor->Close ();
			return (M4_ERROR);
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ValidateFKs (void)
{
	m4return_t result;
	m4int32_t indForFld;
	ClRegisterIndex indFor(0);

	m4uint32_t numFor = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Count ();

	for (indFor=0; indFor<numFor; indFor++)
	{
		ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_VALIDATE_FK + (m4int32_t) indFor);

		if (!pCursor)
		{
			continue;
		}

		/*
		Bug 0297516 La validación de FK de inclusión de fecha de inicio y de periodo
		se hace con el PostValidateOperations
		*/
		if (!m_pGlobVars->GetPostValidate())
		{
			if (pCursor->m_bVerifyData)
			{
				if (!m_pGlobVars->GetPostValidateFKs())
				{
					continue;
				}
			}
			else
			{
				if (!m_pGlobVars->GetPostValidateOperations())
				{
					continue;
				}
			}
		}

		//Escenario:
		//Tenemos tablas padre (P) e hija (H). Queremos insertar en padre, pero saltarnos la necesidad de que exista
		//registro en hijo.
		//Util en importaciones.
		if (pCursor->m_bVerifyData && m_pGlobVars->HasNotValidation (M4LDB_INV_FK_WITHOUT_PERIOD))
		{
			#ifdef _M4LDB_DEBUG
				M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Validate Foreign Key [%ld] desactivated.", (m4int32_t) indFor);
			#endif
			continue;
		}

		m4bool_t bIsNull = M4_FALSE;
		m4bool_t bThereIsNoNull = M4_FALSE;
		m4bool_t bIsModified = M4_FALSE;

		m4int32_t numForFld = pCursor->m_pvcIndParVal->size ();

		for (indForFld=0; indForFld<numForFld; indForFld++)
		{
			PStIndPar pIndPar = (*pCursor->m_pvcIndParVal)[indForFld];

			if (m_pParam->IsNull (pIndPar->m_indPar))
			{
				bIsNull = M4_TRUE;
			}
			else
			{
				// El campo de organización no entra en la validación.
				/* Bug 0155428
				El ID_DATASET tampoco debe entrar
				*/
				eFldType_t eFldType = m_pParam->GetInfoIdInternalField(pIndPar->m_indPar) ;

				if( eFldType != M4LDB_ID_ORGANIZATION && eFldType != M4LDB_ID_DATASET )
				{
					bThereIsNoNull = M4_TRUE;
				}
			}

			if (m_eStmType == M4LDB_INSERT || 
				m_eStmType == M4LDB_DUPLICATE || 
				m_pParam->Difference (pIndPar->m_indPar))
			{
				bIsModified = M4_TRUE;
			}
		}

		if (bIsNull && bThereIsNoNull)
		{
			m4bool_t bComMandat = M4LDB_BOOL (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current[indFor].Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_IS_COM_MANDAT_VAL].Value.GetWithOutSec ());
			
			if (bComMandat)
			{
				string stnPKRegister(M4LDB_EMPTY);
				string stiPKRegister(M4LDB_EMPTY);
				ComposePK (stnPKRegister, stiPKRegister);
				string stnFKRegister(M4LDB_EMPTY);
				string stiFKRegister(M4LDB_EMPTY);
				ComposeFK (stnFKRegister, stiFKRegister, pCursor);
				m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
				m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
				m4id_t iForLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Register[pCursor->m_indForLgcTbl].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
				m4id_t nForLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (pCursor->m_indForLgcTbl);

				DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_FOREIGN_CONSTRAINT_VIOLATION, 
					LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << 
					LOG_CAT_AND (iLgcTbl, nLgcTbl) <<
					LOG_CAT_AND (stiFKRegister.c_str (), stnFKRegister.c_str ()) << 
					LOG_CAT_AND (iForLgcTbl, nForLgcTbl) 
					);
				return (M4_ERROR);
			}
		}

		m4pchar_t	pcType = "";

		if (pCursor->m_bVerifyDTApp)
		{
			/* Bug 0257729
			Hay que leer posicionando
			*/
			eCondValidType_t eCondValid = (eCondValidType_t) M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current[indFor].Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_CONDUCT_VALID].Value.GetWithOutSec ());

			if( eCondValid == M4LDB_VALID_INCLUSION )
			{
				/* Bug 210569
				Si es inclusión de fecha de inicio hay que validar en las siguientes condiciones
				Si no se permite fecha inicial anterior a la del padre
					Si se modifica la fecha de inicio
				Si se permite fecha inicial anterior a la del padre
					Si se modifica la fecha de inicio y se hace mayor
					Si se modifica la fecha de fin y se hace menor
				*/
				if( pCursor->m_bAllowPreviousIni == M4_FALSE )
				{
					if( m_pParam->Difference( m_pMtLgcTbl->m_indDTStart ) != M4LDB_IDENTICAL )
					{
						bIsModified = M4_TRUE;
					}
				}
				else
				{
					if( m_pParam->Difference( m_pMtLgcTbl->m_indDTStart ) == M4LDB_GREATER ||
						m_pParam->Difference( m_pMtLgcTbl->m_indDTEnd ) == M4LDB_LESS )
					{
						bIsModified = M4_TRUE;
					}
				}

				pcType = " Start Date Inclusion";
			}
			else
			{
				/* Bug 211468
				Si es inclusión de periodo hay que validar en las siguientes condiciones
				Si no se permite fecha inicial anterior a la del padre (era el caso que había contemplado)
					Si se modifica la fecha de inicio y se hace memor
					Si se modifica la fecha de fin y se hace mayor
				Si se permite fecha inicial anterior a la del padre
					Si se modifica la fecha de fin
				*/
				if( pCursor->m_bAllowPreviousIni == M4_FALSE )
				{
					if( m_pParam->Difference( m_pMtLgcTbl->m_indDTStart ) == M4LDB_LESS ||
						m_pParam->Difference( m_pMtLgcTbl->m_indDTEnd ) == M4LDB_GREATER )
					{
						bIsModified = M4_TRUE;
					}
				}
				else
				{
					if( m_pParam->Difference( m_pMtLgcTbl->m_indDTEnd ) != M4LDB_IDENTICAL )
					{
						bIsModified = M4_TRUE;
					}
				}

				pcType = " Period Inclusion";
			}
		}

		if (!bIsNull && bIsModified)
		{
			result = ValidateFK (pCursor, indFor, pcType);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ValidateFK (ClLoad *ai_pCursor, m4int32_t ai_indFor, m4pcchar_t ai_pccType)
{
	m4int32_t indRow;
	m4return_t result;
	m4return_t resultFetch;
	ClRegisterIndex indFor(ai_indFor);

	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current[indFor];

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Validate Foreign Key [%ld]%s.", (m4int32_t) indFor, ai_pccType);
	#endif

	if (!ai_pCursor->m_bVerifyData) {

		result = ai_pCursor->Execute ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		ClTblMem oTbl;

		result = ai_pCursor->GetData (&oTbl);
		
		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		m4int32_t numRow = oTbl.GetNumRow ();

		if (!numRow) {
			string stnPKRegister(M4LDB_EMPTY);
			string stiPKRegister(M4LDB_EMPTY);
			ComposePK (stnPKRegister, stiPKRegister);
			string stnFKRegister(M4LDB_EMPTY);
			string stiFKRegister(M4LDB_EMPTY);
			ComposeFK (stnFKRegister, stiFKRegister, ai_pCursor);
			m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
			m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
			m4id_t iForLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Register[ai_pCursor->m_indForLgcTbl].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
			m4id_t nForLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (ai_pCursor->m_indForLgcTbl);

			DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_FOREIGN_CONSTRAINT_VIOLATION, 
				LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << 
				LOG_CAT_AND (iLgcTbl, nLgcTbl) <<
				LOG_CAT_AND (stiFKRegister.c_str (), stnFKRegister.c_str ()) << 
				LOG_CAT_AND (iForLgcTbl, nForLgcTbl) 
				);
			return (M4_ERROR);
		}

		m4date_t dMinForDTStartCorr = M4LDB_PLUS_INF_DATE;
		m4date_t dMaxForDTEndCorr = M4LDB_MINUS_INF_DATE;
		m4date_t dCurDTStart = (m_pMtLgcTbl->m_indDTStart != M4LDB_NULL_INDEX) ? M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTStart)) : M4LDB_MINUS_INF_DATE;
		m4date_t dCurDTEnd = (m_pMtLgcTbl->m_indDTEnd != M4LDB_NULL_INDEX) ? M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTEnd)) : M4LDB_PLUS_INF_DATE;
		eCondValidType_t eCondValid = (eCondValidType_t) M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current.Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_CONDUCT_VALID].Value.GetWithOutSec ());

		for (indRow=0; indRow<numRow; indRow++) {

			if (ai_pCursor->m_bVerifyDTApp) {

				// Sabemos que la DTStart está en la columna 0.
				m4date_t dForDTStart = M4LDB_DOUBLE (oTbl.GetData (indRow, 0));
				
				// Sabemos que la DTEnd está en la columna 1.
				m4date_t dForDTEnd = M4LDB_DOUBLE (oTbl.GetData (indRow, 1));

				if (eCondValid == M4LDB_VALID_INCLUSION) {
					// Validamos inclusion.

					/* Bug 0205463
					Si la fecha de inicio es anterior a la fecha de inicio de la FK sólo se da error si
					o no se permite fecha inicio anterior a la de incio de la FK
					o la fecha de fin es anterior a la de inicio de la FK
					*/
					if(    M4LDB_COMPARE_DATES( dCurDTStart, dForDTEnd ) > 0
						|| M4LDB_COMPARE_DATES( dCurDTStart, dForDTStart ) < 0
						   && (  ai_pCursor->m_bAllowPreviousIni == M4_FALSE
							  || M4LDB_COMPARE_DATES( dCurDTEnd, dForDTStart ) < 0 ) )
					{
						string stnPKRegister(M4LDB_EMPTY);
						string stiPKRegister(M4LDB_EMPTY);
						ComposePK (stnPKRegister, stiPKRegister);
						string stnFKRegister(M4LDB_EMPTY);
						string stiFKRegister(M4LDB_EMPTY);
						ComposeFK (stnFKRegister, stiFKRegister, ai_pCursor);
						m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
						m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
						m4id_t iForLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Register[ai_pCursor->m_indForLgcTbl].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
						m4id_t nForLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (ai_pCursor->m_indForLgcTbl);

						m4id_t iField;
						m4id_t nField;

						if( ai_pCursor->m_bAllowPreviousIni == M4_FALSE )
						{
							iField = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTStart);
							nField = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTStart);
						}
						else
						{
							// Si se permite fecha de inicio anterior se muestra mensaje de error de la fecha fin
							iField = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTEnd);
							nField = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTEnd);
						}

						DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_FOREIGN_INCLUSION, 
							LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << 
							LOG_CAT_AND (iLgcTbl, nLgcTbl) <<
							LOG_CAT_AND (iField, nField) <<
							DateLog (dForDTStart) << 
							DateLog (dForDTEnd) <<
							LOG_CAT_AND (stiFKRegister.c_str (), stnFKRegister.c_str ()) << 
							LOG_CAT_AND (iForLgcTbl, nForLgcTbl) 
							);
						return (M4_ERROR);
					}
				}
				else {
					// Validamos period o completitud.

					/* Bug 0205463
					Si la fecha de inicio es anterior a la fecha de inicio de la FK sólo se da error si
					o no se permite fecha inicio anterior a la de incio de la FK
					o la fecha de fin es anterior a la de inicio de la FK
					*/
					if(    M4LDB_COMPARE_DATES( dCurDTEnd, dForDTEnd ) > 0
						|| M4LDB_COMPARE_DATES( dCurDTStart, dForDTStart ) < 0
						   && (  ai_pCursor->m_bAllowPreviousIni == M4_FALSE
							  || M4LDB_COMPARE_DATES( dCurDTEnd, dForDTStart ) < 0 ) )
					{
						string stnPKRegister(M4LDB_EMPTY);
						string stiPKRegister(M4LDB_EMPTY);
						ComposePK (stnPKRegister, stiPKRegister);
						string stnFKRegister(M4LDB_EMPTY);
						string stiFKRegister(M4LDB_EMPTY);
						ComposeFK (stnFKRegister, stiFKRegister, ai_pCursor);
						m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
						m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
						m4id_t iForLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Register[ai_pCursor->m_indForLgcTbl].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
						m4id_t nForLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (ai_pCursor->m_indForLgcTbl);
						m4id_t iField = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTStart);
						m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTStart);
						m4id_t iField2 = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTEnd);
						m4id_t nField2 = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTEnd);

						m4uint32_t iError;

						if( ai_pCursor->m_bAllowPreviousIni == M4_FALSE )
						{
							iError = M4_CH_LDB_INVALID_FOREIGN_PERIOD;
						}
						else
						{
							// Si se permite fecha de inicio anterior se muestra mensaje de error solo en fecha fin
							iError = M4_CH_LDB_INVALID_FOREIGN_PERIOD_END;
						}

						DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), iError, 
							LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << 
							LOG_CAT_AND (iLgcTbl, nLgcTbl) <<
							LOG_CAT_AND (iField, nField) <<
							LOG_CAT_AND (iField2, nField2) <<
							DateLog (dForDTStart) << 
							DateLog (dForDTEnd) <<
							LOG_CAT_AND (stiFKRegister.c_str (), stnFKRegister.c_str ()) << 
							LOG_CAT_AND (iForLgcTbl, nForLgcTbl) 
							);
						return (M4_ERROR);
					}
				}
			}

			if (ai_pCursor->m_bVerifyDTCorr) {
				m4int32_t indDT = (ai_pCursor->m_bVerifyDTApp) ? 2 : 0;

				// Sabemos que la DTStartCorr está en la columna 0 o 2.
				m4date_t dForDTStartCorr = M4LDB_DOUBLE (oTbl.GetData (indRow, indDT));
				
				dMinForDTStartCorr = (M4LDB_COMPARE_DATES (dForDTStartCorr, dMinForDTStartCorr) < 0) ? dForDTStartCorr : dMinForDTStartCorr;

				// Sabemos que la DTEndCorr está en la columna 1 o 3.
				m4date_t dForDTEndCorr = M4LDB_DOUBLE (oTbl.GetData (indRow, indDT + 1));

				dMaxForDTEndCorr = (M4LDB_COMPARE_DATES (dForDTEndCorr, dMaxForDTEndCorr) > 0) ? dForDTEndCorr : dMaxForDTEndCorr;
			}
		}

		if (ai_pCursor->m_bVerifyDTCorr) {

			m4date_t dCurDTStartCorr = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTStartCorr));

			m4date_t dCurDTEndCorr = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTEndCorr));

			if ((M4LDB_COMPARE_DATES (dCurDTStartCorr, dMinForDTStartCorr) < 0) ||
				(M4LDB_COMPARE_DATES (dCurDTEndCorr, dMaxForDTEndCorr) > 0)) {
				string stnPKRegister(M4LDB_EMPTY);
				string stiPKRegister(M4LDB_EMPTY);
				ComposePK (stnPKRegister, stiPKRegister);
				string stnFKRegister(M4LDB_EMPTY);
				string stiFKRegister(M4LDB_EMPTY);
				ComposeFK (stnFKRegister, stiFKRegister, ai_pCursor);
				m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
				m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
				m4id_t iForLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Register[ai_pCursor->m_indForLgcTbl].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
				m4id_t nForLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (ai_pCursor->m_indForLgcTbl);
				m4id_t iField = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTStartCorr);
				m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTStartCorr);
				m4id_t iField2 = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTEndCorr);
				m4id_t nField2 = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTEndCorr);

				DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_FOREIGN_CORRECTION_PERIOD, 
					LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << 
					LOG_CAT_AND (iLgcTbl, nLgcTbl) <<
					LOG_CAT_AND (iField, nField) <<
					LOG_CAT_AND (iField2, nField2) <<
					DateLog (dMinForDTStartCorr) << 
					DateLog (dMaxForDTEndCorr) <<
					LOG_CAT_AND (stiFKRegister.c_str (), stnFKRegister.c_str ()) << 
					LOG_CAT_AND (iForLgcTbl, nForLgcTbl) 
					);
				return (M4_ERROR);
			}
/*
// Estudiar que hacemos con esto...
			if (m_pMtLgcTbl->m_eCurTemp == M4LDB_N_PERIOD) {

				m4int32_t numCorr = (m_pVcHistory) ? m_pVcHistory->size () : 0;

				if ((!numCorr) &&
					((eForComp == M4LDB_COMPLETE_AND_NULL) ||
					(eForComp == M4LDB_COMPLETE_AND_NO_NULL)) &&
					((M4LDB_COMPARE_DATES (dCurDTStartCorr, dMinForDTStartCorr)) ||
					(M4LDB_COMPARE_DATES (dCurDTEndCorr, dMaxForDTEndCorr)))) {
					string stnPKRegister(M4LDB_EMPTY);
					string stiPKRegister(M4LDB_EMPTY);
					ComposePK (stnPKRegister, stiPKRegister);
					string stnFKRegister(M4LDB_EMPTY);
					string stiFKRegister(M4LDB_EMPTY);
					ComposeFK (stnFKRegister, stiFKRegister, ai_pCursor);
					m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
					m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
					m4id_t iForLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Register[ai_pCursor->m_indForLgcTbl].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
					m4id_t nForLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (ai_pCursor->m_indForLgcTbl);
					m4id_t iField = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTStartCorr);
					m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTStartCorr);
					m4id_t iField2 = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTEndCorr);
					m4id_t nField2 = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTEndCorr);

					DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_COMPLETENESS_CORRECTION_UNSATISFIED, 
						LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << 
						LOG_CAT_AND (iLgcTbl, nLgcTbl) <<
						LOG_CAT_AND (iField, nField) <<
						LOG_CAT_AND (iField2, nField2) <<
						DateLog (dMinForDTStartCorr) << 
						DateLog (dMaxForDTEndCorr) <<
						LOG_CAT_AND (stiFKRegister.c_str (), stnFKRegister.c_str ()) << 
						LOG_CAT_AND (iForLgcTbl, nForLgcTbl) 
						);
					return (M4_ERROR);
				}
			}
*/
		}
	}
	else {

		result = ai_pCursor->Execute ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		result = ai_pCursor->Fetch ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		resultFetch = result;

		if (resultFetch == M4_SUCCESS) {
			result = ai_pCursor->Close ();
		}

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		if (resultFetch == M4LDB_SQL_NO_DATA_FOUND) {
			string stnPKRegister(M4LDB_EMPTY);
			string stiPKRegister(M4LDB_EMPTY);
			ComposePK (stnPKRegister, stiPKRegister);
			string stnFKRegister(M4LDB_EMPTY);
			string stiFKRegister(M4LDB_EMPTY);
			ComposeFK (stnFKRegister, stiFKRegister, ai_pCursor);
			m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
			m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
			m4id_t iForLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Register[ai_pCursor->m_indForLgcTbl].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
			m4id_t nForLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (ai_pCursor->m_indForLgcTbl);

			eRelType_t eRelType = (eRelType_t) M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current.Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_RELATION_ORDER].Value.GetWithOutSec ());

			if (eRelType == M4LDB_CHILD_RELATION) {
				DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_CHILD_NOT_FOUND, 
					LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << 
					LOG_CAT_AND (iLgcTbl, nLgcTbl) <<
					LOG_CAT_AND (stiFKRegister.c_str (), stnFKRegister.c_str ()) << 
					LOG_CAT_AND (iForLgcTbl, nForLgcTbl) 
					);
			}
			else {
				DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_FOREIGN_CONSTRAINT_VIOLATION, 
					LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << 
					LOG_CAT_AND (iLgcTbl, nLgcTbl) <<
					LOG_CAT_AND (stiFKRegister.c_str (), stnFKRegister.c_str ()) << 
					LOG_CAT_AND (iForLgcTbl, nForLgcTbl) 
					);
			}
			return (M4_ERROR);
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ValidateFRs (void)
{
	m4return_t result;
	m4int32_t indForFld;
	ClRegisterIndex indFor(0);

	m4uint32_t numFor = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Count ();

	for (indFor=0; indFor<numFor; indFor++) {
		
		ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_VALIDATE_FR + (m4int32_t) indFor);

		if (!pCursor) {
			continue;
		}

		if (pCursor->m_bVerifyData && m_pGlobVars->HasNotValidation (M4LDB_INV_FR_WITHOUT_PERIOD))
		{
			#ifdef _M4LDB_DEBUG
				M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Validate Foreign Reference [%ld] desactivated.", (m4int32_t) indFor);
			#endif
			continue;
		}

		m4bool_t bIsModified = M4_FALSE;

		if (m_eStmType == M4LDB_UPDATE) {

			if (pCursor->m_bVerifyDTApp) {	
		
				if (((m_pParam->Difference (m_pMtLgcTbl->m_indDTStart) == M4LDB_GREATER)) || 
					(m_pParam->Difference (m_pMtLgcTbl->m_indDTEnd) == M4LDB_LESS)) {
					bIsModified = M4_TRUE;
				}

				m4bool_t bComplete = M4_FALSE;

				eCondValidType_t eAuxCondValid = (eCondValidType_t) M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Register[indFor].Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_CONDUCT_VALID].Value.GetWithOutSec ());

				if (eAuxCondValid == M4LDB_VALID_COMPLETENESS) {
					bComplete = M4_TRUE;
				}

				if ((bComplete) && 
					(((m_pParam->Difference (m_pMtLgcTbl->m_indDTStart) == M4LDB_LESS)) || 
					(m_pParam->Difference (m_pMtLgcTbl->m_indDTEnd) == M4LDB_GREATER))) {
					bIsModified = M4_TRUE;
				}
			}

			if (!bIsModified) {

				m4int32_t numForFld = pCursor->m_pvcIndParVal->size ();

				for (indForFld=0; indForFld<numForFld; indForFld++) {

					PStIndPar pIndPar = (*pCursor->m_pvcIndParVal)[indForFld];

					if (m_pParam->Difference (pIndPar->m_indPar)) {
						bIsModified = M4_TRUE;
						break;
					}
				}
			}
		}
		else {
			bIsModified = M4_TRUE;
		}

		if (bIsModified) {

			result = ValidateFR (pCursor, indFor);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ValidateFR (ClLoad *ai_pCursor, m4int32_t ai_indFor)
{
	m4return_t result;
	m4return_t resultFetch;
	ClRegisterIndex indFor(ai_indFor);
	m4bool_t bDeleteOnCascade = M4_FALSE;
	m4bool_t bAll = M4_TRUE;
	m4bool_t bExit = M4_TRUE;

	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current[indFor];

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Validate Foreign Reference [%ld].", (m4int32_t) indFor);
	#endif

	if (!ai_pCursor->m_bVerifyData)
	{
		if (m_eStmType == M4LDB_DELETE)
		{
			bDeleteOnCascade = M4LDB_BOOL (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current.Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_DELETE_CASCADE].Value.GetWithOutSec ());

			if (bDeleteOnCascade == M4_FALSE && m_pGlobVars->HasNotValidation (M4LDB_INV_FR_WITHOUT_PERIOD))
			{
				#ifdef _M4LDB_DEBUG
					M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Validate Foreign Reference [%ld] desactivated.", (m4int32_t) indFor);
				#endif
				return (M4_SUCCESS);
			}
		}

		result = ai_pCursor->Execute ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		bAll = M4_TRUE;

		if (m_eStmType == M4LDB_DELETE)
		{
			if (bDeleteOnCascade)
			{
				bAll = M4_FALSE;
			}
		}
		else
		{
			if (m_pMtLgcTbl->m_bUpdatePK)
			{
				bAll = M4_FALSE;
			}
		}

		do
		{
			bExit = M4_TRUE;

			ClTblMem oTbl;

			if (bAll)
			{
				result = ai_pCursor->GetData (&oTbl);
			}
			else
			{
				//Bug 0076358
				//Tiene que ver con la carga de bloques de 100.
				//En SQLServer, si tenemos DeleteCascade da un error de Connection busy with another hstm.
				//Tiene pinta de que la limitacion a 100 se haya realizado para no tener excesivos registros cargados,
				//es como una carga parcial.
				//Opcion 1:
				//Modificar el valor de 100 por el de 0, esto nos permite realizar el Fetch completo.
				//De la misma forma eliminaríamos el if (más abajo).
				//Opcion 2:
				//Modificar el valor de 100 a un valor más elevado. El if tb se modificaria.				

				m4int32_t numRowRequired = 0;

				result = ai_pCursor->GetData (&oTbl, &numRowRequired);
				
				//Caso de soporte: 25730
				//Bug 0079536. Parecido a 0048726
				//El caso de soporte hace referencia a este bug. Parece ser que estaba ya controlado pero no es así.
				//El problema viene cuando tenemos exactamente 100 validaciones FR.
				//Escenario:
				//Tenemos T1 y una T2, existe una relación con delete cascade.
				//ValidateFR supone comprobar que los datos de T1 no están siendo utilizados en T2.				
				//Si borramos un registro de T1, se lanza una select para comprobar si existe algún registro en T2
				//que cumpla el join de la relación. Si hay exactamente 100 registros en T2 que la cumplen entonces
				//da un error ODBC.				
				//Acción: Podemos comentar la limitación de 100!!. Pero no está claro. Tal vez sería mejor incrementar
				//el valor.
			}
				
			if (result == M4_ERROR) {
				return (M4_ERROR);
			}

			m4int32_t numRow = oTbl.GetNumRow ();

			if (numRow)
			{
				m4int32_t indRow;
				m4date_t dCurDTStart = (m_pMtLgcTbl->m_indDTStart != M4LDB_NULL_INDEX) ? M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTStart)) : M4LDB_MINUS_INF_DATE;
				m4date_t dCurDTEnd = (m_pMtLgcTbl->m_indDTEnd != M4LDB_NULL_INDEX) ? M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTEnd)) : M4LDB_PLUS_INF_DATE;
				m4bool_t bDTStartCascade = M4_FALSE;
				eMoveDTEndOnCascType_t eMoveDTEndOnCasc = M4LDB_NO_MOVE_DT_END_ON_CASC;
				eCondValidType_t eCondValid = M4LDB_NO_TESTED;
				m4bool_t bComplete = M4_FALSE;
				bDTStartCascade = M4LDB_BOOL (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current.Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_DT_START_CASCADE].Value.GetWithOutSec ());
				eMoveDTEndOnCasc = (eMoveDTEndOnCascType_t) M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current.Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_DT_END_CASCADE].Value.GetWithOutSec ());
				eCondValid = (eCondValidType_t) M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current.Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_ID_CONDUCT_VALID].Value.GetWithOutSec ());
				
				for (indRow=0; indRow<numRow; indRow++)
				{
					if (ai_pCursor->m_bVerifyDTCorr)
					{
						m4int32_t indForDTEndCorr = (ai_pCursor->m_bVerifyDTApp) ? 3 : 1;
						m4date_t dForDTEndCorr = M4LDB_DOUBLE (oTbl.GetData (indRow, indForDTEndCorr));
						m4date_t dCurDTEndCorr = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTEndCorr));
						
						if (M4LDB_COMPARE_DATES (dForDTEndCorr, dCurDTEndCorr) > 0)
						{
							string stnPKRegister(M4LDB_EMPTY);
							string stiPKRegister(M4LDB_EMPTY);
							ComposePK (stnPKRegister, stiPKRegister);
							string stnFKRegister(M4LDB_EMPTY);
							string stiFKRegister(M4LDB_EMPTY);
							ComposeFK (stnFKRegister, stiFKRegister, ai_pCursor);
							m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
							m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
							m4id_t iForLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Register[ai_pCursor->m_indForLgcTbl].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
							m4id_t nForLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (ai_pCursor->m_indForLgcTbl);
							m4id_t iField = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTEndCorr);
							m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTEndCorr);
							m4int32_t indForDTStartCorr = (ai_pCursor->m_bVerifyDTApp) ? 2 : 0;	
							m4date_t dForDTStartCorr = M4LDB_DOUBLE (oTbl.GetData (indRow, indForDTStartCorr));

							DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_REFERENTIAL_CORRECTION_PERIOD, 
								LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << 
								LOG_CAT_AND (iLgcTbl, nLgcTbl) <<
								LOG_CAT_AND (iField, nField) <<
								DateLog (dForDTStartCorr) << 
								DateLog (dForDTEndCorr) <<
								LOG_CAT_AND (stiFKRegister.c_str (), stnFKRegister.c_str ()) << 
								LOG_CAT_AND (iForLgcTbl, nForLgcTbl) 
								);
							ai_pCursor->Close ();
							return (M4_ERROR);
						}
					}
					
					if (m_eStmType != M4LDB_DELETE)
					{
						if (ai_pCursor->m_bVerifyDTApp)
						{
							// Sabemos que la DTStart está en la columna 0.
							m4date_t dForDTStart = M4LDB_DOUBLE (oTbl.GetData (indRow, 0));
							// Sabemos que la DTEnd está en la columna 1.
							m4date_t dForDTEnd = M4LDB_DOUBLE (oTbl.GetData (indRow, 1));

							if (eCondValid == M4LDB_VALID_INCLUSION)
							{
								// Validamos inclusion.

								/* Bug 0205463
								Si la fecha de inicio es anterior a la fecha de inicio de la FK sólo se da error si
								o no se permite fecha inicio anterior a la de incio de la FK
								o la fecha de fin es anterior a la de inicio de la FK
								*/
								if(    M4LDB_COMPARE_DATES( dForDTStart, dCurDTEnd ) > 0
									|| M4LDB_COMPARE_DATES( dForDTStart, dCurDTStart ) < 0
									   && (  ai_pCursor->m_bAllowPreviousIni == M4_FALSE
										  || M4LDB_COMPARE_DATES( dForDTEnd, dCurDTStart ) < 0 ) )
								{
									string stnPKRegister(M4LDB_EMPTY);
									string stiPKRegister(M4LDB_EMPTY);
									ComposePK (stnPKRegister, stiPKRegister);
									string stnFKRegister(M4LDB_EMPTY);
									string stiFKRegister(M4LDB_EMPTY);
									ComposeFK (stnFKRegister, stiFKRegister, ai_pCursor);
									m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
									m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
									m4id_t iForLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Register[ai_pCursor->m_indForLgcTbl].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
									m4id_t nForLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (ai_pCursor->m_indForLgcTbl);
									m4id_t iField = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTStart);
									m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTStart);

									DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_REFERENTIAL_INCLUSION, 
										LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << 
										LOG_CAT_AND (iLgcTbl, nLgcTbl) <<
										LOG_CAT_AND (iField, nField) <<
										DateLog (dForDTStart) << 
										DateLog (dForDTEnd) <<
										LOG_CAT_AND (stiFKRegister.c_str (), stnFKRegister.c_str ()) << 
										LOG_CAT_AND (iForLgcTbl, nForLgcTbl) 
										);
									ai_pCursor->Close ();
									return (M4_ERROR);
								}
							}
							else
							{
								// Validamos period o completitud.

								if ((M4LDB_COMPARE_DATES (dForDTStart, dCurDTEnd) > 0) || 
									(M4LDB_COMPARE_DATES (dForDTEnd, dCurDTStart) < 0))
								{
									string stnPKRegister(M4LDB_EMPTY);
									string stiPKRegister(M4LDB_EMPTY);
									ComposePK (stnPKRegister, stiPKRegister);
									string stnFKRegister(M4LDB_EMPTY);
									string stiFKRegister(M4LDB_EMPTY);
									ComposeFK (stnFKRegister, stiFKRegister, ai_pCursor);
									m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
									m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
									m4id_t iForLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Register[ai_pCursor->m_indForLgcTbl].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
									m4id_t nForLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (ai_pCursor->m_indForLgcTbl);
									m4id_t iField = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTStart);
									m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTStart);
									m4id_t iField2 = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTEnd);
									m4id_t nField2 = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTEnd);

									DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_REFERENTIAL_PERIOD, 
										LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << 
										LOG_CAT_AND (iLgcTbl, nLgcTbl) <<
										LOG_CAT_AND (iField, nField) <<
										LOG_CAT_AND (iField2, nField2) <<
										DateLog (dForDTStart) << 
										DateLog (dForDTEnd) <<
										LOG_CAT_AND (stiFKRegister.c_str (), stnFKRegister.c_str ()) << 
										LOG_CAT_AND (iForLgcTbl, nForLgcTbl) 
										);
									ai_pCursor->Close ();
									return (M4_ERROR);
								}

								m4bool_t bMoveDTStart = M4_FALSE;
								m4bool_t bMoveDTEnd = M4_FALSE;
								m4bool_t bCloseDTEnd = M4_FALSE;
								m4date_t dAuxDTStart = dForDTStart;
								m4date_t dAuxDTEnd = dForDTEnd;

								if (eCondValid == M4LDB_VALID_PERIOD)
								{
									if (M4LDB_COMPARE_DATES (dForDTEnd, dCurDTEnd) > 0)
									{
										if (!M4LDB_COMPARE_DATES (dForDTEnd, M4LDB_PLUS_INF_DATE))
										{
											bCloseDTEnd = M4_TRUE;
										}
										else
										{
											bMoveDTEnd = M4_TRUE;
										}
										dAuxDTEnd = dCurDTEnd;
									}

									/* Bug 0205463
									Si la fecha de inicio es anterior a la fecha de inicio de la FK sólo se da intenta mover en cascada si
									o no se permite fecha inicio anterior a la de incio de la FK
									o hay movimiento en cascada
									*/
									if( M4LDB_COMPARE_DATES( dForDTStart, dCurDTStart ) < 0
										&& ( ai_pCursor->m_bAllowPreviousIni == M4_FALSE || bDTStartCascade == M4_TRUE ) )
									{
										bMoveDTStart = M4_TRUE;
										dAuxDTStart = dCurDTStart;
									}
								}
								else
								{
									if (indRow == numRow - 1)
									{
										if (M4LDB_COMPARE_DATES (dForDTEnd, dCurDTEnd))
										{
											if (!M4LDB_COMPARE_DATES (dForDTEnd, M4LDB_PLUS_INF_DATE))
											{
												bCloseDTEnd = M4_TRUE;
											}
											else
											{
												bMoveDTEnd = M4_TRUE;
											}
											dAuxDTEnd = dCurDTEnd;
										}
									}

									if (indRow == 0)
									{
										/* Bug 0205463
										Si la fecha de inicio es diferente a la fecha de inicio de la FK sólo se da intenta mover en cascada si
										o no se permite fecha inicio anterior a la de incio de la FK
										o hay movimiento en cascada
										o la fecha de inicio es posterior a la fecha de inicio de la FK
										*/
										if( M4LDB_COMPARE_DATES( dForDTStart, dCurDTStart )
											&& ( ai_pCursor->m_bAllowPreviousIni == M4_FALSE || bDTStartCascade == M4_TRUE || M4LDB_COMPARE_DATES( dForDTStart, dCurDTStart ) > 0 ) )
										{
											bMoveDTStart = M4_TRUE;
											dAuxDTStart = dCurDTStart;
										}
									}
								}

								if (bCloseDTEnd || bMoveDTEnd || bMoveDTStart)
								{
									if (((bCloseDTEnd) && (eMoveDTEndOnCasc == M4LDB_NO_MOVE_DT_END_ON_CASC)) || 
										((bMoveDTEnd) && (eMoveDTEndOnCasc != M4LDB_MOVE_DT_END_ON_CASC)) || 
										((bMoveDTStart) && (!bDTStartCascade)))
									{
										string stnPKRegister(M4LDB_EMPTY);
										string stiPKRegister(M4LDB_EMPTY);
										ComposePK (stnPKRegister, stiPKRegister);
										string stnFKRegister(M4LDB_EMPTY);
										string stiFKRegister(M4LDB_EMPTY);
										ComposeFK (stnFKRegister, stiFKRegister, ai_pCursor);
										m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
										m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
										m4id_t iForLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Register[ai_pCursor->m_indForLgcTbl].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
										m4id_t nForLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (ai_pCursor->m_indForLgcTbl);
										m4id_t iField = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTStart);
										m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTStart);
										m4id_t iField2 = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTEnd);
										m4id_t nField2 = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTEnd);

										DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_REFERENTIAL_PERIOD, 
											LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << 
											LOG_CAT_AND (iLgcTbl, nLgcTbl) <<
											LOG_CAT_AND (iField, nField) <<
											LOG_CAT_AND (iField2, nField2) <<
											DateLog (dForDTStart) << 
											DateLog (dForDTEnd) <<
											LOG_CAT_AND (stiFKRegister.c_str (), stnFKRegister.c_str ()) << 
											LOG_CAT_AND (iForLgcTbl, nForLgcTbl) 
											);
										ai_pCursor->Close ();
										return (M4_ERROR);
									}

									result = MoveOnCascade (&oTbl, ai_pCursor->m_indForLgcTbl, indRow, dAuxDTStart, dAuxDTEnd, ai_pCursor->m_bVerifyDTCorr, ai_pCursor->m_bVerifyOperation);

									if (result == M4_ERROR)
									{
										ai_pCursor->Close ();
										return (M4_ERROR);
									}
								}
							}
						}
					}
				}
						
				if (m_pMtLgcTbl->m_bUpdatePK)
				{
					result = UpdatePKOnCascade (&oTbl, ai_pCursor->m_indForLgcTbl, ai_pCursor);
					
					if (result == M4_ERROR)
					{
						ai_pCursor->Close ();
						return (M4_ERROR);
					}
				}

				if (m_eStmType == M4LDB_DELETE)
				{
					m4int32_t numCol = oTbl.GetNumCol ();
					m4int32_t indPK = (ai_pCursor->m_bVerifyOperation) ? 1 : 0;
					
					indPK = (ai_pCursor->m_bVerifyDTCorr) ? indPK + 2 : indPK;
					indPK = (ai_pCursor->m_bVerifyDTApp) ? indPK + 2 : indPK;

					// Marcamos la PK.
					for (m4int32_t indCol=indPK; indCol<numCol; indCol++)
					{
						oTbl.SetIsPK (indCol, M4_TRUE);
					}

					if (bDeleteOnCascade)
					{
						result = DeleteOnCascade (&oTbl, ai_pCursor->m_indForLgcTbl, ai_pCursor->m_bVerifyDTApp, ai_pCursor->m_bVerifyDTCorr, ai_pCursor->m_bVerifyOperation);
						
						if (result == M4_ERROR)
						{
							ai_pCursor->Close ();
							return (M4_ERROR);
						}
					}
					else
					{
						for (indRow=0; indRow<numRow; indRow++)
						{
							// Si el registro es de herencia borrado no hay que tenerlo en cuenta
							if( ai_pCursor->m_bVerifyOperation )
							{
								// Sabemos que la operation es la anterior a la pk
								m4int32_t iOperation = M4LDB_NUMBER( oTbl.GetData( indRow, indPK - 1 ) ) ;

								if( iOperation == 0 )
								{
									continue ;
								}
							}

							if (!FindPersistInPost (&oTbl, indRow, ai_pCursor->m_indForLgcTbl))
							{
								string stnPKRegister(M4LDB_EMPTY);
								string stiPKRegister(M4LDB_EMPTY);
								ComposePK (stnPKRegister, stiPKRegister);
								string stnFKRegister(M4LDB_EMPTY);
								string stiFKRegister(M4LDB_EMPTY);
								ComposeFK (stnFKRegister, stiFKRegister, ai_pCursor);
								m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
								m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
								m4id_t iForLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Register[ai_pCursor->m_indForLgcTbl].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
								m4id_t nForLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (ai_pCursor->m_indForLgcTbl);

								DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_REFERENTIAL_CONSTRAINT_VIOLATION, 
									LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << 
									LOG_CAT_AND (iLgcTbl, nLgcTbl) <<
									LOG_CAT_AND (stiFKRegister.c_str (), stnFKRegister.c_str ()) << 
									LOG_CAT_AND (iForLgcTbl, nForLgcTbl) 
									);
								ai_pCursor->Close ();
								return (M4_ERROR);
							}
						}
					}
				}
			}
		}
		while (!bExit);
	}
	else
	{
		result = ai_pCursor->Execute ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		result = ai_pCursor->Fetch ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		resultFetch = result;

		if (resultFetch == M4_SUCCESS) {
			result = ai_pCursor->Close ();
		}

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		if (resultFetch != M4LDB_SQL_NO_DATA_FOUND)
		{
			string stnPKRegister(M4LDB_EMPTY);
			string stiPKRegister(M4LDB_EMPTY);
			ComposePK (stnPKRegister, stiPKRegister);
			string stnFKRegister(M4LDB_EMPTY);
			string stiFKRegister(M4LDB_EMPTY);
			ComposeFK (stnFKRegister, stiFKRegister, ai_pCursor);
			m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
			m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
			m4id_t iForLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Register[ai_pCursor->m_indForLgcTbl].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
			m4id_t nForLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (ai_pCursor->m_indForLgcTbl);

			DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_REFERENTIAL_CONSTRAINT_VIOLATION, 
				LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << 
				LOG_CAT_AND (iLgcTbl, nLgcTbl) <<
				LOG_CAT_AND (stiFKRegister.c_str (), stnFKRegister.c_str ()) << 
				LOG_CAT_AND (iForLgcTbl, nForLgcTbl) 
				);
			return (M4_ERROR);
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ValidateSecurity (void)
{
	m4return_t result;

	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_VALIDATE_SECURITY);

	if (!pCursor) {
		return (M4_SUCCESS);
	}

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Validate Security.");
	#endif

	result = pCursor->Execute (NULL, 0);

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	result = pCursor->Fetch ();

	if (result == M4_ERROR) {
		result = pCursor->Close ();
		return (M4_ERROR);
	}

	if (result == M4LDB_SQL_NO_DATA_FOUND) {		
		//Bug 0080564
		//Tenemos DC y seguridad en la tabla hija de tal forma que hace una select sobre la padre.
		//El error se produce pq la select de validación de seguridad que se lanza no encuentra registros en el padre
		//que satisfaga la select por lo tanto da un error por falta de permisos.		
		//No hemos encontrado la validacion de seguridad, pero puede que exista en los registros que se han borrado
		//dentro de la misma conexión, por lo tanto podemos buscar en otra conexión.
		//--------------------------------------------------------------
		//Importante:
		//Solo compruebo en la otra conexion si la sentencia es delete o update!!.
		//Escenario:
		//Supongamos dos tablas: Padre (T1), Hija (T2).
		//En T1 tenemos un filtro: T2.C1 >= 22.
		//En T2 tenemos PK <= 88 AND T1.PK >= 5.
		//Es decir, si se cumplen los filtros nos permitira hacer las operaciones.
		//En el caso de Update de T1 y T2, si estamos modificando un registro cuyos nuevos valores no
		//cumplen el filtro, NO daremos error.
		//Todavía no se ha hecho commit, por lo tanto esta segunda conexion siempre obtendrá
		//el valor antiguo.
		//Por lo tanto, segun esto podremos modificar a un valor nuevo que no podrá ser modificado (pq 
		//puede no cumplir la sentencia de validacion de seguridad), pero eso sera otra película.

		//Acción: Permitir únicamente la comprobación en casos de borrado y update!!!.
		//--------------------------------------------------------------

		//--------------------------------------------------------------
		//NOTA ACLARATORIA Importante:
		//El par formado por: Conexion real (conexiones a BD) + Conexión lógica obtenemos un handle de conexión.
		//El handle de conexión es donde se realiza la petición a BD.
		//Veremos cómo definir el par en funcion de si estamos en SM o en Server!!.		
		//El objetivo es obtener un par único que no afecte a la conexión/es que hayan en curso!!.
		//--------------------------------------------------------------

		//En principio es error!!!;		
		result = M4_ERROR;

		if ((m_eStmType == M4LDB_DELETE) || (m_eStmType == M4LDB_UPDATE) ){
			//Si tampoco encontramos registros entonces es que verdaderamente no hay permisos.			
			// FUNCIONA result = _ValidateSecurityExecutingAuxiliarSelect (ClConnectionManager::m_sOtherDBConn, pCursor);			
			//Buscamos el cursor, si no lo encontramos lo preparamos...
			ClLoad *pCursorInOtherConnection = (ClLoad *) FindAuxCursor (M4LDB_VALIDATE_SECURITY_OTHER_CONNECTION);
			if (!pCursorInOtherConnection) {
				
				m4return_t resultInOtherConnection = M4_SUCCESS;

				resultInOtherConnection = PrepareValidateSecurityInOtherConnection ();
				if (resultInOtherConnection == M4_ERROR) {
					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (),M4_CH_LDB_INTERNAL_ERROR,"PrepareValidateSecurityInOtherConnection"<<"Preparing Security validation in other connection.");
					return (M4_ERROR);
				}

				resultInOtherConnection = BindParamValidateSecurityInOtherConnection ();
				if (resultInOtherConnection == M4_ERROR) {
					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (),M4_CH_LDB_INTERNAL_ERROR,"BindParamValidateSecurityInOtherConnection"<<"Binding Security validation in other connection.");
					return (M4_ERROR);
				}
			}

			result = ValidateSecurityInOtherConnection ();
		}

		//Comprobamos las variables de resultado.
		if ((result != M4_SUCCESS) || (result == M4LDB_SQL_NO_DATA_FOUND) ) {

			string stnPKRegister(M4LDB_EMPTY);
			string stiPKRegister(M4LDB_EMPTY);
			ComposePK (stnPKRegister, stiPKRegister);
			m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
			m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;

			DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_SECURITY_CONSTRAINT_VIOLATION, 
				LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << 
				LOG_CAT_AND (iLgcTbl, nLgcTbl)
				);
			return (M4_ERROR);
		}
	}

	result = pCursor->Close ();

	return (result);
}

/* FUNCIONA 
m4return_t ClPersistBase::_ValidateSecurityExecutingAuxiliarSelect (m4int32_t ai_idRConn, ClLoad * ai_pCursorSource)
{
	//Creamos una select auxiliar en base a otra load.
	ClLoad * pCursorSecurity = NULL;
	m4return_t result = M4_ERROR;

	m4int32_t idRConn = ai_idRConn;

	m4bool_t bDataFound = M4_FALSE;

	//Indicamos en el ldbinsp
	M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Validate Security (In Other Connection).");

	pCursorSecurity = new ClLoad(m_pGlobVars);
	if (!pCursorSecurity) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (),M4_CH_LDB_INTERNAL_ERROR,"_ValidateSecurityExecutingAuxiliarSelect"<<"Impossible create Load in connection.");
		return M4_ERROR;
	}

	if ((!ai_pCursorSource) || (!(ai_pCursorSource->GetMetaSentence ())) ) {
		if (pCursorSecurity) {
			delete pCursorSecurity;
		}
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (),M4_CH_LDB_INTERNAL_ERROR,"_ValidateSecurityExecutingAuxiliarSelect"<<"No source Load or no Metasentence.");
		return M4_ERROR;
	}

	//---------------------------------------------------
	//Invocamos la ejecucion.	
	result = _ValidateSecurityExecutingAuxiliarSelectInConnection (idRConn, ai_pCursorSource, pCursorSecurity, bDataFound);
	//Esta sería la llamada para la ejecución en la conexión de auditoría a fuego.
	//result = _ValidateSecurityExecutingAuxiliarSelectInConnection (ClConnectionManager::m_sAuditoryDBConn, ai_pCursorSource, pCursorSecurity, bDataFound);
	//---------------------------------------------------

	//Tratamos de la misma forma si la ejecucion ha dado error o si no hay registros que validen seguridad.
	if ((result == M4_ERROR) || (bDataFound == M4_FALSE) ) {
		if (pCursorSecurity) {
			delete pCursorSecurity;
		}
		return M4_ERROR;
	}

	//Si estamos aqui es pq NO se ha producido un error propiamente dicho error.
	//Tambien estaremos aquí si SÍ se cumple la validacion de seguridad, es decir, hay permisos para la operación..
	//Pero de todas formas eliminamos el cursor pq no lo necesitamos. Siempre eliminamos.
	if (pCursorSecurity) {
		delete pCursorSecurity;
	}

	//Indicamos en el ldbinsp
	M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Validate Security (In Other Connection) successfull.");

	return result;
}

m4return_t ClPersistBase::_ValidateSecurityExecutingAuxiliarSelectInConnection (m4int32_t ai_idRConn, ClLoad * ai_pCursorSource, ClLoad * ai_pCursorDestination, m4bool_t &ao_bDataFound)
{
	//Creamos una select auxiliar en base a otra load.
	ClLoad * pCursorSecurity = ai_pCursorDestination;	
	string stStmSQL;
	m4return_t result = M4_SUCCESS;

	//Pensamos que no va a haber datos en el Fetch.
	ao_bDataFound = M4_FALSE;

	//El params sí puede ser nulo.
	if ( (!ai_pCursorSource)
		|| (ai_pCursorSource->GetMetaSentence ()) ) {
	}
	//Ejecutamos la select sobre la conexion que acabamos de pedir.
	//Indicamos que no somos propietarios de la metasentencia.
	result = pCursorSecurity->Open (ai_idRConn, ai_pCursorSource->GetMetaSentence (), M4_FALSE, ai_pCursorSource->GetpParams());
	if (result == M4_ERROR) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (),M4_CH_LDB_INTERNAL_ERROR,"_ValidateSecurityExecutingAuxiliarSelectInConnection"<<"Open.");
		return M4_ERROR;
	}

	result = pCursorSecurity->Prepare (stStmSQL);
	if (result == M4_ERROR) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (),M4_CH_LDB_INTERNAL_ERROR,"_ValidateSecurityExecutingAuxiliarSelectInConnection"<<"Prepare.");
		return M4_ERROR;
	}

	result = pCursorSecurity->BindParam (NULL);
	if (result == M4_ERROR) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (),M4_CH_LDB_INTERNAL_ERROR,"_ValidateSecurityExecutingAuxiliarSelectInConnection"<<"BindParam.");
		return M4_ERROR;
	}

	result = pCursorSecurity->Execute ();
	if (result == M4_ERROR) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (),M4_CH_LDB_INTERNAL_ERROR,"_ValidateSecurityExecutingAuxiliarSelectInConnection"<<"Execute.");
		return M4_ERROR;
	}

	result = pCursorSecurity->Fetch();
	if (result == M4_ERROR) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (),M4_CH_LDB_INTERNAL_ERROR,"_ValidateSecurityExecutingAuxiliarSelectInConnection"<<"Fetch.");
		return M4_ERROR;
	}

	//Si no encontramos registros en la select auxiliar damos un error.
	//Se trata de un error funcional, significa que en la select que lanzamos para validar seguridad no obtenemos
	//registros, por lo tanto no se tendrán permisos.
	if (result == M4LDB_SQL_NO_DATA_FOUND) {
		//Nos aseguramos de indicar que no hay datos.
		ao_bDataFound = M4_FALSE;
		//Retornamos exito en la ejecucion.
		return M4_SUCCESS;
	}
	
	//Si estamos en este punto es pq no ha habido errores y se han encontrado datos.
	ao_bDataFound = M4_TRUE;
	return M4_SUCCESS;
}
FIN FUNCIONA*/

m4return_t ClPersistBase::ValidateSecurityInOtherConnection (void)
{
	m4return_t result;

	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_VALIDATE_SECURITY_OTHER_CONNECTION);

	if (!pCursor) {
		return (M4_SUCCESS);
	}

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Validate Security In Other Connection.");
	#endif

	result = pCursor->Execute (NULL, 0);

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	result = pCursor->Fetch ();

	if (result == M4_ERROR) {
		result = pCursor->Close ();
		return (M4_ERROR);
	}

	if (result == M4LDB_SQL_NO_DATA_FOUND) {
		//Retornamos que no se ha encontrado registro que valide la condicion de seguridad.
		//El resultado sera analogo a devolver error.
		return (M4LDB_SQL_NO_DATA_FOUND);
	}

	result = pCursor->Close ();

	return (result);
}


m4return_t ClPersistBase::ExecuteTramitExist ( m4bool_t &ao_rbExist, m4date_t* ao_pdDtLastUpdate)
{
	m4return_t result;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Tramitation Exist.");
	#endif

	ao_rbExist = M4_FALSE ;
	if ( ao_pdDtLastUpdate != NULL ) 
	{
		*ao_pdDtLastUpdate = M4LDB_MINUS_INF_DATE ;
	}

	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_TRAMIT_EXIST);

	if (!pCursor)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::ExecuteTramitExist" << "The cursor M4LDB_TRAMIT_EXIST does not exist");
		return (M4_ERROR);
	}

	result = pCursor->Execute ();

	if (result == M4_ERROR)
	{
		return (M4_ERROR);
	}
		
	ClTblMem oTbl;

	result = pCursor->GetData (&oTbl);

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	m4int32_t numRow = oTbl.GetNumRow ();

	if( numRow != 0 )
	{
		//Si nos piden valor devolvemos la fecha
		if ( ao_pdDtLastUpdate != NULL ) 
		{
			*ao_pdDtLastUpdate = M4LDB_DOUBLE ( oTbl.GetData (0, 0) ) ;
		}
		ao_rbExist = M4_TRUE ;
	}
	
	result = pCursor->Close ();
	
	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ExecuteTramitDelete (void)
{
	m4return_t result;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Tramitation Delete.");
	#endif

	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_TRAMIT_DELETE);

	if (!pCursor) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::ExecuteTramitDelete" << "The cursor M4LDB_TRAMIT_DELETE does not exist");
		return (M4_ERROR);
	}

	result = pCursor->Execute ();

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	result = pCursor->Close ();

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ExecuteTramitInsert (void)
{
	m4return_t result;
	ClParam*	pParamTramitDTLastUPdate = NULL;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Tramitation Insert.");
	#endif

	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_TRAMIT_INSERT);

	if (!pCursor)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::ExecuteTramitInsert" << "The cursor M4LDB_TRAMIT_INSERT does not exist");
		return (M4_ERROR);
	}

	/* Bug 0166440
	Se asigna el tamaño del TRAMIT_DT_LAST_UPDATE, ya que el bindado del result de la lectura
	en el validate pk no lo hace
	Nunca debería ser nulo.
	*/
	//Sólo para la tabla principal
	if( m_indRealTbl == 0 )
	{
		pParamTramitDTLastUPdate = GetClParamByType( M4LDB_TRAMIT_DT_LAST_UPDATE ) ;
		*pParamTramitDTLastUPdate->m_plLength = sizeof( m4date_t );
	}

	result = pCursor->Execute ();

	if (result == M4_ERROR)
	{
		return (M4_ERROR);
	}

	result = pCursor->Close ();

	return (M4_SUCCESS);
}

m4return_t ClPersistBase::ExecuteTramitInsertObject (void)
{
	m4return_t result;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Tramitation Insert Object.");
	#endif

	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_TRAMIT_INSERT_OBJECT);

	if (!pCursor)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::ExecuteTramitInsertObject" << "The cursor M4LDB_TRAMIT_INSERT_OBJECT does not exist");
		return (M4_ERROR);
	}

	result = pCursor->Execute ();

	if (result == M4_ERROR)
	{
		return (M4_ERROR);
	}

	result = pCursor->Close ();

	return (M4_SUCCESS);
}

m4return_t ClPersistBase::ValidateOrganization (void)
{
	m4return_t result;

	//Bug 0075467
	//Se prepara y se binda cuando la tabla es Multisocietaria, por lo tanto parece lógico
	//que tambien se ejecute si la tabla es Multi.

	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_VALIDATE_ORGANIZATION);

	if (!pCursor) {
		return (M4_SUCCESS);
	}

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Validate Organization.");
	#endif

	if (m_pVcClParam != NULL) {		
		ClParam * pParam = NULL;
		pParam = GetClParamByType(M4LDB_SEC_USER);
		if (pParam != NULL) {
			if (pParam->SetBindParam(M4LDB_SEC_USER, m_pGlobVars->GetpIdUserLogged(), M4LDB_M4_UNICODE_TYPE(M4CL_M4_TYPE_FIXED_STRING), M4LDB_ID_MAX_SIZE, 0) == M4_ERROR) {
				return (M4_ERROR);
			}
		}

		pParam = GetClParamByType(M4LDB_APP_ROLE);
		if (pParam != NULL) {
			if (pParam->SetBindParam(M4LDB_APP_ROLE, m_pGlobVars->GetpIdRoleLogged(), M4LDB_M4_UNICODE_TYPE(M4CL_M4_TYPE_FIXED_STRING), M4LDB_ID_MAX_SIZE, 0) == M4_ERROR) {
				return (M4_ERROR);
			}
		}		
	}

	result = pCursor->Execute (NULL, 0);

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	result = pCursor->Fetch ();

	if (result == M4_ERROR) {
		result = pCursor->Close ();
		return (M4_ERROR);
	}

	if (result == M4LDB_SQL_NO_DATA_FOUND) {
		string stnPKRegister(M4LDB_EMPTY);
		string stiPKRegister(M4LDB_EMPTY);
		ComposePK (stnPKRegister, stiPKRegister);
		m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
		m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;

		DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_SECURITY_CONSTRAINT_VIOLATION, 
			LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << 
			LOG_CAT_AND (iLgcTbl, nLgcTbl)
			);
		return (M4_ERROR);
	}

	result = pCursor->Close ();

	return (result);
}


m4return_t ClPersistBase::ExecuteInheritExist (m4bool_t &ao_rbExist)
{
	m4return_t result;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Inherit Exist.");
	#endif

	ao_rbExist = M4_FALSE ;

	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_INHERIT_EXIST);

	if (!pCursor) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::ExecuteInheritExist" << "The cursor M4LDB_INHERIT_EXIST does not exist");
		return (M4_ERROR);
	}

	result = pCursor->Execute ();

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	result = pCursor->Fetch ();

	if (result == M4_ERROR) {
		result = pCursor->Close ();
		return (M4_ERROR);
	}

	if (result != M4LDB_SQL_NO_DATA_FOUND)
	{
		result = pCursor->Close ();
		ao_rbExist = M4_TRUE ;
	}
	else
	{
		ao_rbExist = M4_FALSE ;
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ExecuteInheritDelete (void)
{
	m4return_t result;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Inherit Delete.");
	#endif

	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_INHERIT_DELETE);

	if (!pCursor) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::ExecuteInheritDelete" << "The cursor M4LDB_INHERIT_DELETE does not exist");
		return (M4_ERROR);
	}

	result = pCursor->Execute ();

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	result = pCursor->Close ();

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ExecuteInheritInsert (void)
{
	m4return_t result;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Inherit Insert.");
	#endif

	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_INHERIT_INSERT);

	if (!pCursor) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::ExecuteInheritInsert" << "The cursor M4LDB_INHERIT_INSERT does not exist");
		return (M4_ERROR);
	}

	result = pCursor->Execute ();

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	result = pCursor->Close ();

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ExecuteInheritValidate (m4bool_t &ao_rbExist)
{
	m4return_t result;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Inherit Validate.");
	#endif

	ao_rbExist = M4_FALSE ;

	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_INHERIT_VALIDATE);

	if (!pCursor) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::ExecuteInheritValidate" << "The cursor M4LDB_INHERIT_VALIDATE does not exist");
		return (M4_ERROR);
	}

	result = pCursor->Execute ();

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	ClTblMem oTbl;

	result = pCursor->GetData (&oTbl);
	
	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	m4int32_t numRow = oTbl.GetNumRow ();

	if( numRow != 0 )
	{
		m4int32_t	indRow = -1 ;
		m4pchar_t	pcOrgId = NULL ;
		m4pchar_t	pcOrgName = NULL ;
		m4id_t		iLgcTbl = NULL ;
		m4id_t		nLgcTbl = NULL ;
		string		siOrganizations ;
		string		snOrganizations ;
		string		siPKRegister ;
		string		snPKRegister ;

		ao_rbExist = M4_TRUE ;

		for( indRow = 0 ; indRow < numRow ; indRow++ )
		{
			if( indRow != 0 )
			{
				siOrganizations.append( ", " ) ;
				snOrganizations.append( ", " ) ;
			}

			// El id está en la primera columna y el nombre en la segunda
			pcOrgId = M4LDB_PSTR( oTbl.GetData( indRow, 0 ) ) ;
			pcOrgName = M4LDB_PSTR( oTbl.GetData( indRow, 1 ) ) ;

			if( pcOrgName == NULL )
			{
				pcOrgName = pcOrgId ;
			}

			siOrganizations.append( pcOrgId ) ;
			snOrganizations.append( pcOrgName ) ;
		}

		iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
		nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);

		ComposePK( snPKRegister, siPKRegister ) ;

		DUMP_LDBINSP_WARNING_AND_PATHF( m_pGlobVars->GetpLConn (),
			M4_CH_TF_DATA_REFERENCED_IN_CHILDREN, 
			LOG_CAT_AND( siPKRegister.c_str(), snPKRegister.c_str() ) << 
			LOG_CAT_AND( iLgcTbl, nLgcTbl ) <<
			LOG_CAT_AND( siOrganizations.c_str(), snOrganizations.c_str() ) ) ;

	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ExecuteTranslationInsert (void)
{
	m4return_t result;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Translation Insert.");
	#endif

	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_TRANSLATION_INSERT);

	if (!pCursor) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::ExecuteTranslationInsert" << "The cursor M4LDB_TRANSLATION_INSERT does not exist");
		return (M4_ERROR);
	}

	result = pCursor->Execute ();

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	result = pCursor->Close ();

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ExecuteTranslationValidatePK (m4bool_t &ao_rbExist)
{
	m4return_t result;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Translation Validate Primary Key.");
	#endif

	ao_rbExist = M4_FALSE ;

	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_TRANSLATION_VALIDATE_PK);

	if (!pCursor) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::ExecuteTranslationValidatePK" << "The cursor M4LDB_TRANSLATION_VALIDATE_PK does not exist");
		return (M4_ERROR);
	}

	result = pCursor->Execute ();

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	result = pCursor->Fetch ();

	if (result == M4_ERROR) {
		result = pCursor->Close ();
		return (M4_ERROR);
	}

	if (result != M4LDB_SQL_NO_DATA_FOUND)
	{
		result = pCursor->Close ();
		ao_rbExist = M4_TRUE ;
	}
	else
	{
		ao_rbExist = M4_FALSE ;
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ExecuteTranslationUpdatePK (void)
{
	m4return_t result;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Translation Update Primary Key.");
	#endif

	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_TRANSLATION_UPDATE_PK);

	if (!pCursor) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::ExecuteTranslationUpdatePK" << "The cursor M4LDB_TRANSLATION_UPDATE_PK does not exist");
		return (M4_ERROR);
	}

	result = pCursor->Execute ();

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	result = pCursor->Close ();

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ExecuteUpdateVersion (void)
{
	m4return_t result;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Update Version.");
	#endif

	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_UPDATE_VERSION);

	if (!pCursor) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::ExecuteUpdateVersion" << "The cursor M4LDB_UPDATE_VERSION does not exist");
		return (M4_ERROR);
	}

	ClParam* pParamDTLastUPdate = GetClParamByType(M4LDB_DT_LAST_UPDATE) ;
	pParamDTLastUPdate->SetBindParam(M4LDB_DT_LAST_UPDATE, (m4pvoid_t) &m_pGlobVars->GetpLConn()->m_dDTLstUpdTrn, M4CL_M4_TYPE_TIMESTAMP, M4LDB_SIZE_TIMESTAMP, 0);

	result = pCursor->Execute ();

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	result = pCursor->Close ();

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::MoveOnCascade (ClTblMem *ai_pData, m4int32_t ai_indLgcTbl, m4int32_t ai_indRow, m4date_t ai_dDTStart, m4date_t ai_dDTEnd, m4bool_t ai_bCorr, m4bool_t ai_bOperation)
{
	m4return_t result;
	ClRegisterIndex indOldCurForeign(0);
	ClRegisterIndex	indLgcTbl(ai_indLgcTbl);
	m4int32_t indFld;
	ClDinArr *pData = NULL;
	m4int32_t indDtStart = M4LDB_NULL_INDEX;
	m4int32_t indDtEnd = M4LDB_NULL_INDEX;
	m4int32_t numFld;
	m4bool_t bEnd = M4_FALSE;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n [ Internal Operation ]\n # Move On Cascade.");
	#endif

	
	// Sabemos que la DTStart está en la columna 0.
	m4date_t dForDTStart = M4LDB_DOUBLE (ai_pData->GetData (ai_indRow, 0));
	
	// Sabemos que la DTEnd está en la columna 1.
	m4date_t dForDTEnd = M4LDB_DOUBLE (ai_pData->GetData (ai_indRow, 1));

	
	if ((!(m_pMtLgcTbl->m_lSecMask & M4LDB_SEC_MASK_UPDATE_DT_START_ON_CASCADE)) && 
		(M4LDB_COMPARE_DATES (dForDTStart, ai_dDTStart))) {
		m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
		m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;

		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_UPDATE_DT_START_ON_CASCADE_REFUSE, LOG_CAT_AND (iLgcTbl, nLgcTbl));
		return (M4_ERROR);
	}


	eMoveDTEndOnCascType_t eMoveDTEndOnCasc = (eMoveDTEndOnCascType_t) M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current.Item[M4LDB_ITEM_SYS_DICT_FOREIGNS_DT_END_CASCADE].Value.GetWithOutSec ());

	if ((eMoveDTEndOnCasc == M4LDB_MOVE_DT_END_ON_CASC) && 
		(M4LDB_COMPARE_DATES (dForDTEnd, ai_dDTEnd))) {
		bEnd = M4_TRUE;
		if (!(m_pMtLgcTbl->m_lSecMask & M4LDB_SEC_MASK_UPDATE_DT_END_ON_CASCADE)) {
			m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
			m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
			
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_UPDATE_DT_END_ON_CASCADE_REFUSE, LOG_CAT_AND (iLgcTbl, nLgcTbl));
			return (M4_ERROR);
		}
	}

	if ((eMoveDTEndOnCasc == M4LDB_CLOSE_DT_END_ON_CASC) && 
		(!M4LDB_COMPARE_DATES (dForDTEnd, M4LDB_PLUS_INF_DATE))) {
		bEnd = M4_TRUE;
		if (!(m_pMtLgcTbl->m_lSecMask & M4LDB_SEC_MASK_CLOSE_DT_END_ON_CASCADE)) {
			m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
			m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
			
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_CLOSE_DT_END_ON_CASCADE_REFUSE, LOG_CAT_AND (iLgcTbl, nLgcTbl));
			return (M4_ERROR);
		}
	}


	if ((bEnd == M4_FALSE) && 
		(m_pMtLgcTbl->m_bUpdatePK)) {
		// La fecha de inicio la cambiamos en la función UpdatePK.
		return (M4_SUCCESS);
	}

	// Guardamos el current.
	indOldCurForeign = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current.Index;

	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[indLgcTbl];

	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[(ClRegisterIndex) 0];

	ClPersist *pStm = (ClPersist *) FindAuxCursor (M4LDB_MOVE_ON_CASCADE + (m4int32_t) indLgcTbl);

	if (!pStm) {

		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Move On Cascade.");
		#endif

		/* Bug 0160492
		Se activa la tramitación en movimiento de fechas de las tablas hijas
		*/
		pStm = new ClPersist (m_pGlobVars, M4_TRUE, m_bUseTramitation, M4_FALSE, M4_FALSE);

		m4int32_t idRConn = M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_CONN].Value.GetWithOutSec ());

		result = pStm->Open (M4LDB_UPDATE, idRConn);
			
		if (result == M4_ERROR) {
			delete pStm;
			return (M4_ERROR);
		}

		pData = pStm->GetpParams ();

//		numFld = pData->GetNumData ();
		numFld = pStm->GetMaster ()->m_oVParam.size ();

		for (indFld=0; indFld<numFld; indFld++) {
			
			m4int32_t indParam = pStm->GetMaster ()->m_oVParam[indFld];
			
			eFldType_t eFldType = pData->GetInfoIdInternalField (indParam);
			
			if (!m_pMtLgcTbl->m_bUpdatePK) {
				if ((eFldType == M4LDB_DT_START) || 
					(eFldType == M4LDB_DT_END)) {
					pData->SetItemHandle (indParam, M4LDB_INTERNAL_HANDLE);
					
					if (eFldType == M4LDB_DT_START) {
						indDtStart = indParam;
					}
					
					if (eFldType == M4LDB_DT_END) {
						indDtEnd = indParam;
					}
				}
			}
			else {
				// Solo cambiamos la fecha de fin.
				if (eFldType == M4LDB_DT_END) {
					pData->SetItemHandle (indParam, M4LDB_INTERNAL_HANDLE);
					indDtEnd = indParam;
				}
			}
		}

		result = pStm->Prepare ();

		if (result == M4_ERROR) {
			delete pStm;
			return (M4_ERROR);
		}

		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Move On Cascade.");
		#endif

		// Bindamos los parámetros.
		result = pStm->BindParam ();
		
		if (result == M4_ERROR) {
			delete pStm;
			return (M4_ERROR);
		}

		m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_MOVE_ON_CASCADE + (m4int32_t) indLgcTbl, pStm));
	}
	else {
		pData = pStm->GetpParams ();

		if (!m_pMtLgcTbl->m_bUpdatePK) {
			indDtStart = pData->FindIndexByIdInternalField (M4LDB_DT_START);
		}
		else {
			// Solo cambiamos la fecha de fin.
		}
		
		indDtEnd = pData->FindIndexByIdInternalField (M4LDB_DT_END);

//		numFld = pData->GetNumData ();
		numFld = pStm->GetMaster ()->m_oVParam.size ();
	}

	if (!m_pMtLgcTbl->m_bUpdatePK) {
		if (indDtStart == M4LDB_NULL_INDEX) {
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::MoveOnCascade" << "Logic field DT_START not found");
			return (M4_ERROR);
		}
	}
	if (indDtEnd == M4LDB_NULL_INDEX) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::MoveOnCascade" << "Logic field DT_END not found");
		return (M4_ERROR);
	}

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Move On Cascade.");
	#endif

	// Hemos construido la tabla en memoria de la siguiente forma:
	// DT_START, DT_END, [DT_START_CORR, DT_END_CORR], PK1, PK2, PK3....
	// y sabemos que el orden de las PKs será el mismo tanto en la tabla de memoria como en el cldinarr.

	pData->Clear ();

	if (!m_pMtLgcTbl->m_bUpdatePK) {
		pData->SetData (indDtStart, (m4pcchar_t) &ai_dDTStart);
	}
	else {
		// La fecha de inicio se trata como una PK normal.
		// Si es necesario la modificaremos en el UpdatePK.
	}

	pData->SetData (indDtEnd, (m4pcchar_t) &ai_dDTEnd);

	// A partir de la columna 2 o 4 están las PKs.
	m4int32_t indPK = (ai_bOperation) ? 1 : 0;

	indPK = (ai_bCorr) ? indPK + 4 : indPK + 2;
	

	for (indFld=0; indFld<numFld; indFld++) {

		m4int32_t indParam = pStm->GetMaster ()->m_oVParam[indFld];

		m4bool_t bIsPK = pData->GetInfoIsPK (indParam);

		if (bIsPK) {
			eFldType_t eFldType = pData->GetInfoIdInternalField (indParam);

			if ((eFldType == M4LDB_DT_START) && 
				(!m_pMtLgcTbl->m_bUpdatePK)) {
				pData->SetOldData (indParam, ai_pData->GetData (ai_indRow, indPK), 0, ai_pData->GetM4Type (indPK));
			}
			else {
				pData->SetData (indParam, ai_pData->GetData (ai_indRow, indPK), 0, ai_pData->GetM4Type (indPK));
			}

			indPK++;
		}
	}

	m4bool_t bOldPostValidate = m_pGlobVars->GetPostValidate ();

	m_pGlobVars->SetPostValidate (M4_TRUE);

	result = pStm->Execute ();

	m_pGlobVars->SetPostValidate (bOldPostValidate);

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	// Reajustamos los current.
	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[m_pMtLgcTbl->m_indLgcTbl];

	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[m_indRealTbl];

	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current[indOldCurForeign];

	if (m_pMtSent) {
		m_pMtLgcTbl->m_bInvRecordSet = M4_TRUE;
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::DeleteOnCascade (ClTblMem *ai_pData, m4int32_t ai_indLgcTbl, m4bool_t ai_bApp, m4bool_t ai_bCorr, m4bool_t ai_bOperation)
{
	m4return_t result;
	ClRegisterIndex indOldCurForeign(0);
	ClRegisterIndex	indLgcTbl(ai_indLgcTbl);
	m4int32_t indFld;
	m4int32_t indRow;


	// Guardamos el current.
	indOldCurForeign = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current.Index;
	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[indLgcTbl];
	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[(ClRegisterIndex) 0];


	m4bool_t bUseTramitation = M4_FALSE;

	if( m_bUseTramitation == M4_TRUE && m_pMtLgcTbl->m_iMirrorType == 1 )
	{
		m4uint8_t iForeignMirrorType = M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_MIRROR_TYPE].Value.GetWithOutSec ());

		if( iForeignMirrorType == 1 )
		{
			// Si la tabla es de tramitación y la foreign también se hace el delete con tramitación
			bUseTramitation = M4_TRUE ;
		}
		else
		{
			// Si la tabla es de tramitación y la foreign no lo es, no se puede hacer delete on cascade
			m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[m_pMtLgcTbl->m_indLgcTbl];
			m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[m_indRealTbl];
			m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current[indOldCurForeign];
			return( M4_SUCCESS ) ;
		}
	}

	m4bool_t bUseInheritance = M4_FALSE;

	if( m_bUseInheritance == M4_TRUE && m_pMtLgcTbl->m_bInheritsData == M4_TRUE )
	{
		m4bool_t bForeignInheritsData = M4LDB_BOOL (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_INHERITS_DATA].Value.GetWithOutSec ());

		if( bForeignInheritsData == M4_TRUE )
		{
			// Si la tabla es de herencia y la foreign también se hace el delete lógico
			bUseInheritance = M4_TRUE ;
		}
	}


	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n [ Internal Operation ]\n # Delete On Cascade.");
	#endif

	/*
	if (!(m_pMtLgcTbl->m_lSecMask & M4LDB_SEC_MASK_DELETE_ON_CASCADE)) {
		m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
		m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;

		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_DELETE_ON_CASCADE_REFUSE, LOG_CAT_AND (iLgcTbl, nLgcTbl));
		return (M4_ERROR);
	}
	*/

	ClPersist *pStm = (ClPersist *) FindAuxCursor (M4LDB_DELETE_ON_CASCADE + (m4int32_t) indLgcTbl);
				
	if (!pStm)
	{
		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Delete On Cascade.");
		#endif

		pStm = new ClPersist (m_pGlobVars, M4_TRUE, bUseTramitation, bUseInheritance, M4_FALSE);

		m4int32_t idRConn = M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_CONN].Value.GetWithOutSec ());

		result = pStm->Open (M4LDB_DELETE, idRConn);

		if (result == M4_ERROR) {
			delete pStm;
			return (M4_ERROR);
		}

		//Pedro :Bug 0064098 -> DC; 0074680 -> UC
		//Indicamos que se va a producir un borrado en cascada.
		m_pGlobVars->GetpLConn ()->m_iStatus |= M4LDB_CONEX_STATUS_DELETE_ON_CASCADE;
		//Fin Pedro

		result = pStm->Prepare ();

		//Pedro :Bug 0064098 -> DC; 0074680 -> UC
		//Lo dejamos sin estado.
		m_pGlobVars->GetpLConn ()->m_iStatus &= ~M4LDB_CONEX_STATUS_DELETE_ON_CASCADE;
		//Fin Pedro

		if (result == M4_ERROR) {
			delete pStm;
			return (M4_ERROR);
		}

		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Delete On Cascade.");
		#endif

		// Bindamos los parámetros.
		result = pStm->BindParam ();
		
		if (result == M4_ERROR) {
			delete pStm;
			return (M4_ERROR);
		}

		m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_DELETE_ON_CASCADE + (m4int32_t) indLgcTbl, pStm));
	}

	ClDinArr *pData = pStm->GetpParams ();

//	m4int32_t numFld = pData->GetNumData ();
	m4int32_t numFld = pStm->GetMaster ()->m_oVParam.size ();


	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Delete On Cascade.");
	#endif

	// Hemos construido la tabla en memoria de la siguiente forma:
	// [DT_START, DT_END], [DT_START_CORR, DT_END_CORR], PK1, PK2, PK3....
	// y sabemos que el orden de las PKs será el mismo tanto en la tabla de memoria como en el cldinarr.

	pData->Clear ();

	m4int32_t numRow = ai_pData->GetNumRow ();

	for (indRow=0; indRow<numRow; indRow++)
	{
		m4int32_t indPK = (ai_bOperation) ? 1 : 0;

		indPK = (ai_bCorr) ? indPK + 2 : indPK;
		indPK = (ai_bApp) ? indPK + 2 : indPK;

		/* Bug 0152743
		Los borrados también se deben tener en cuenta en el borrado en cascada

		if( ai_bOperation )
		{
			// Sabemos que la operation es la anterior a la pk
			m4int32_t iOperation = M4LDB_NUMBER( ai_pData->GetData( indRow, indPK - 1 ) ) ;

			if( iOperation == 0 )
			{
				continue ;
			}
		}
		*/

		if (!FindPersistInPost (ai_pData, indRow, ai_indLgcTbl))
		{
			if (!(m_pMtLgcTbl->m_lSecMask & M4LDB_SEC_MASK_DELETE_ON_CASCADE))
			{
				m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
				m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;

				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_DELETE_ON_CASCADE_REFUSE, LOG_CAT_AND (iLgcTbl, nLgcTbl));
				return (M4_ERROR);
			}

			for (indFld=0; indFld<numFld; indFld++)
			{
				m4int32_t indParam = pStm->GetMaster ()->m_oVParam[indFld];
				m4bool_t bIsPK = pData->GetInfoIsPK (indParam);

				if (bIsPK)
				{
					pData->SetData (indParam, ai_pData->GetData (indRow, indPK), 0, ai_pData->GetM4Type (indPK));
					indPK++;
				}
			}

			m4bool_t bOldPostValidate = m_pGlobVars->GetPostValidate ();

			m_pGlobVars->SetPostValidate (M4_TRUE);

			result = pStm->Execute ();

			m_pGlobVars->SetPostValidate (bOldPostValidate);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
		}
	}

	// Reajustamos los current.
	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[m_pMtLgcTbl->m_indLgcTbl];
	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[m_indRealTbl];
	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current[indOldCurForeign];

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::UpdatePKOnCascade (ClTblMem *ai_pData, m4int32_t ai_indLgcTbl, ClLoad *ai_pCursor)
{
	m4return_t result;
	m4int32_t indForFld;
	ClRegisterIndex indOldCurForeign(0);
	ClRegisterIndex	indLgcTbl(ai_indLgcTbl);
	m4int32_t indFld;
	m4int32_t indRow;
	m4int32_t indDtStart = M4LDB_NULL_INDEX;
	m4bool_t bPKDiff = M4_FALSE;
	m4bool_t bDTStartDiff = M4_FALSE;
	ClDinArr *pData = NULL;

	// Veamos si hay algo que actualizar.
	m4int32_t numParam = m_oVParam.size ();

	for (m4int32_t indPar=0; indPar<numParam; indPar++) {

		m4int32_t indParam = m_oVParam[indPar];

		// Las fechas de inicio de app y corr no se consideran como PK.
		if ((indParam != m_pMtLgcTbl->m_indDTStartCorr) && 
			(m_pParam->GetInfoIsPK (indParam)) && 
			(m_pParam->Difference (indParam))) {
			bPKDiff = M4_TRUE;
			if (indParam == m_pMtLgcTbl->m_indDTStart) {
				bDTStartDiff = M4_TRUE;
				break;
			}
		}
	}

	if (!bPKDiff) {
		return (M4_SUCCESS);
	}

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n [ Internal Operation ]\n # Update Primary Key On Cascade.");
	#endif

	if (!(m_pMtLgcTbl->m_lSecMask & M4LDB_SEC_MASK_UPDATE_PK_ON_CASCADE)) {
		m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
		m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;

		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_UPDATE_PK_ON_CASCADE_REFUSE, LOG_CAT_AND (iLgcTbl, nLgcTbl));
		return (M4_ERROR);
	}

	if ((!(m_pMtLgcTbl->m_lSecMask & M4LDB_SEC_MASK_UPDATE_DT_START_ON_CASCADE)) && 
		(bDTStartDiff)) {
		m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
		m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;

		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_UPDATE_DT_START_ON_CASCADE_REFUSE, LOG_CAT_AND (iLgcTbl, nLgcTbl));
		return (M4_ERROR);
	}

	m4int32_t numRow = ai_pData->GetNumRow ();

	// Guardamos el current.
	indOldCurForeign = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current.Index;

	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[indLgcTbl];

	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[(ClRegisterIndex) 0];

	ClPersist *pStm = (ClPersist *) FindAuxCursor (M4LDB_UPDATEPK_ON_CASCADE + (m4int32_t) indLgcTbl);
				
	if (!pStm) {

		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Update Primary Key On Cascade.");
		#endif

		/* Bug 0160492
		Se activa la tramitación en actualización de las pks de las tablas hijas
		*/
		pStm = new ClPersist (m_pGlobVars, M4_TRUE, m_bUseTramitation, M4_FALSE, M4_FALSE);

		m4int32_t idRConn = M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_CONN].Value.GetWithOutSec ());

		result = pStm->Open (M4LDB_UPDATE, idRConn);
			
		if (result == M4_ERROR) {
			delete pStm;
			return (M4_ERROR);
		}

		pData = pStm->GetpParams ();

//		numFld = pData->GetNumData ();

		if (ai_pCursor->m_bVerifyDTApp == M4_TRUE) {
			m4int32_t indParam = pData->FindIndexByIdInternalField (M4LDB_DT_START);

			if (indParam != M4LDB_NULL_INDEX) {
				m4bool_t bIsPK = pData->GetInfoIsPK (indParam);
				if (bIsPK) {
					pData->SetItemHandle (indParam, M4LDB_INTERNAL_HANDLE);
					indDtStart = indParam;
				}
			}
		}

		m4int32_t numForFld = ai_pCursor->m_pvcIndParVal->size ();

		for (indForFld=0; indForFld<numForFld; indForFld++) {

			PStIndPar pIndPar = (*ai_pCursor->m_pvcIndParVal)[indForFld];

			m4bool_t bIsPK = m_pParam->GetInfoIsPK (pIndPar->m_indPar);

			if (bIsPK) {
				m4int32_t indForField = pData->FindIndexByIdField (pIndPar->m_idForFld);

				if (indForField == M4LDB_NULL_INDEX) {
					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::UpdatePKOnCascade" << "Logic field = " << LogCat << pIndPar->m_idForFld << LogCat << " not found in the logic object = " << LogCat << m_pMtLgcTbl->m_idLgcTbl);
					return (M4_ERROR);
				}

				pData->SetItemHandle (indForField, M4LDB_INTERNAL_HANDLE);
			}
		}

		//Bug 0074680
		//Marcamos que estamos en un Update en cascada.
		m_pGlobVars->GetpLConn ()->m_iStatus |= M4LDB_CONEX_STATUS_UPDATE_ON_CASCADE;
		//Fin Bug.
		
		result = pStm->Prepare ();

		//Bug 0074680
		//Desmarcamos el flag.
		m_pGlobVars->GetpLConn ()->m_iStatus &= ~M4LDB_CONEX_STATUS_UPDATE_ON_CASCADE;
		//Fin Bug.

		if (result == M4_ERROR) {
			delete pStm;
			return (M4_ERROR);
		}

		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Update Primary Key On Cascade.");
		#endif

		// Bindamos los parámetros.
		result = pStm->BindParam ();
		
		if (result == M4_ERROR) {
			delete pStm;
			return (M4_ERROR);
		}

		m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_UPDATEPK_ON_CASCADE+ (m4int32_t) indLgcTbl, pStm));
	}
	else {
		pData = pStm->GetpParams ();

//		numFld = pData->GetNumData ();

		if (ai_pCursor->m_bVerifyDTApp == M4_TRUE) {
			m4int32_t indParam = pData->FindIndexByIdInternalField (M4LDB_DT_START);

			if (indParam != M4LDB_NULL_INDEX) {
				m4bool_t bIsPK = pData->GetInfoIsPK (indParam);
				if (bIsPK) {
					indDtStart = indParam;
				}
			}
		}
	}

	m4int32_t numFld = pStm->GetMaster ()->m_oVParam.size ();

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Update Primary Key On Cascade.");
	#endif

	pData->Clear ();

	m4int32_t numForFld = ai_pCursor->m_pvcIndParVal->size ();

	for (indForFld=0; indForFld<numForFld; indForFld++) {

		PStIndPar pIndPar = (*ai_pCursor->m_pvcIndParVal)[indForFld];

		m4bool_t bIsPK = m_pParam->GetInfoIsPK (pIndPar->m_indPar);

		if (bIsPK) {
			m4int32_t indForField = pData->FindIndexByIdField (pIndPar->m_idForFld);

			if (indForField == M4LDB_NULL_INDEX) {
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::UpdatePKOnCascade" << "Logic field = " << LogCat << pIndPar->m_idForFld << LogCat << " not found in the logic object = " << LogCat << m_pMtLgcTbl->m_idLgcTbl);
				return (M4_ERROR);
			}

			pData->SetData (indForField, m_pParam->GetDataPointer (pIndPar->m_indPar), 0, m_pParam->GetInfoM4Type (pIndPar->m_indPar));
		}
	}

	for (indRow=0; indRow<numRow; indRow++) {
	
		if (indDtStart != M4LDB_NULL_INDEX) {
			// Sabemos que la DTStart está en la columna 0.
			m4date_t dForDTStart = M4LDB_DOUBLE (ai_pData->GetData (indRow, 0));

			if (bDTStartDiff) { 
				m4date_t dCurDTStart = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTStart));

				/* Bug 0208097
				Se permite también mover la fecha hacia atrás
				*/
				if (M4LDB_COMPARE_DATES (dForDTStart, dCurDTStart) != 0) {
					pData->SetData (indDtStart, (m4pcchar_t) &dCurDTStart);
					pData->SetOldData (indDtStart, (m4pcchar_t) &dForDTStart);
				}
				else {
					pData->SetData (indDtStart, (m4pcchar_t) &dForDTStart);
				}
			}
			else {
				pData->SetData (indDtStart, (m4pcchar_t) &dForDTStart);
			}
		}

		m4int32_t indPK = (ai_pCursor->m_bVerifyOperation) ? 1 : 0;
		
		indPK = (ai_pCursor->m_bVerifyDTCorr) ? indPK + 2 : indPK;
		indPK = (ai_pCursor->m_bVerifyDTApp) ? indPK + 2 : indPK;

		for (indFld=0; indFld<numFld; indFld++) {

			m4int32_t indParam = pStm->GetMaster ()->m_oVParam[indFld];

			m4bool_t bIsPK = pData->GetInfoIsPK (indParam);

			if (bIsPK) {
				if (pData->GetItemHandle (indParam)) {
					pData->SetOldData (indParam, ai_pData->GetData (indRow, indPK), 0, ai_pData->GetM4Type (indPK));
				}
				else {
					pData->SetData (indParam, ai_pData->GetData (indRow, indPK), 0, ai_pData->GetM4Type (indPK));
				}

				indPK++;
			}
		}

		m4bool_t bOldPostValidate = m_pGlobVars->GetPostValidate ();

		m_pGlobVars->SetPostValidate (M4_TRUE);

		result = pStm->Execute ();

		m_pGlobVars->SetPostValidate (bOldPostValidate);

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}
	}

	// Reajustamos los current.
	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[m_pMtLgcTbl->m_indLgcTbl];

	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[m_indRealTbl];

	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FOREIGNS]->Current[indOldCurForeign];

	if (m_pMtSent) {
		m_pMtLgcTbl->m_bInvRecordSet = M4_TRUE;
	}
		
	return (M4_SUCCESS);
}


m4return_t ClPersistBase::MoveOnHistory (m4date_t ai_dDTStart, m4date_t ai_dDTClose, m4date_t ai_dOldDTStart)
{
	m4return_t result;
	m4int32_t indFld;
	ClDinArr *pData = NULL;
	m4int32_t numFld;
	m4date_t dOldDTStart = (ai_dOldDTStart) ? ai_dOldDTStart : ai_dDTStart;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n [ Internal Operation ]\n # Move On History.");
	#endif

	ClPersist *pStm = (ClPersist *) FindAuxCursor (M4LDB_MOVE_ON_HISTORY);

	if ((m_pMtLgcTbl->m_eCurHist == M4LDB_N_PERIOD_AUTOMATIC_ADAPTATION) && 
		(!(m_pMtLgcTbl->m_lSecMask & M4LDB_SEC_MASK_HIST_AUTO_ADAPT))) {
		m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
		m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;

		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_HIST_AUTO_ADAPT_REFUSE, LOG_CAT_AND (iLgcTbl, nLgcTbl));
		return (M4_ERROR);
	}

	if ((m_pMtLgcTbl->m_eCurHist == M4LDB_N_PERIOD_CLOSE_LAST_REGISTER) && 
		(!(m_pMtLgcTbl->m_lSecMask & M4LDB_SEC_MASK_CLOSE_PREV_REGISTER))) {
		m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
		m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;

		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_CLOSE_PREV_REGISTER_REFUSE, LOG_CAT_AND (iLgcTbl, nLgcTbl));
		return (M4_ERROR);
	}

	if (!pStm) {

		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Move On History.");
		#endif

		/* Bug 0159233
		Se activa la tramitación en adaptación de fechas de tablas NP
		*/
		pStm = new ClPersist (m_pGlobVars, M4_TRUE, m_bUseTramitation, M4_FALSE, M4_FALSE);

		result = pStm->Open (M4LDB_UPDATE, m_idRConn);
			
		if (result == M4_ERROR) {
			delete pStm;
			return (M4_ERROR);
		}

		pData = pStm->GetpParams ();

//		numFld = pData->GetNumData ();
		numFld = pStm->GetMaster ()->m_oVParam.size ();

		for (indFld=0; indFld<numFld; indFld++) {

			m4int32_t indParam = pStm->GetMaster ()->m_oVParam[indFld];

			eFldType_t eFldType = pData->GetInfoIdInternalField (indParam);

			if ((eFldType == M4LDB_DT_START) || 
				(eFldType == M4LDB_DT_END)) {
				pData->SetItemHandle (indParam, M4LDB_INTERNAL_HANDLE);
			}
		}

		result = pStm->Prepare ();

		if (result == M4_ERROR) {
			delete pStm;
			return (M4_ERROR);
		}

		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Move On History.");
		#endif

		// Bindamos los parámetros.
		result = pStm->BindParam ();
		
		if (result == M4_ERROR) {
			delete pStm;
			return (M4_ERROR);
		}

		m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_MOVE_ON_HISTORY, pStm));
	}
	else {
		pData = pStm->GetpParams ();

//		numFld = pData->GetNumData ();
		numFld = pStm->GetMaster ()->m_oVParam.size ();
	}

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Move On History.");
	#endif

	pData->Clear ();

	for (indFld=0; indFld<numFld; indFld++) {

		m4int32_t indParam = pStm->GetMaster ()->m_oVParam[indFld];

		m4bool_t bIsPK = pData->GetInfoIsPK (indParam);

		eFldType_t eFldType = pData->GetInfoIdInternalField (indParam);

		if ((bIsPK) || 
			(eFldType == M4LDB_DT_START) || 
			(eFldType == M4LDB_DT_END)) {

			if (eFldType == M4LDB_DT_START) {
				pData->SetData (indParam, (m4pcchar_t) &ai_dDTStart);

				pData->SetOldData (indParam, (m4pcchar_t) &dOldDTStart);
			}
			else {
				if (eFldType == M4LDB_DT_END) {
					pData->SetData (indParam, (m4pcchar_t) &ai_dDTClose);
				}
				else {
					m4int32_t indFldCur = m_pParam->FindIndexByIdField (pData->GetInfoIdField (indParam));
					pData->SetData (indParam, m_pParam->GetData (indFldCur), 0, m_pParam->GetInfoM4Type (indFldCur));
				}
			}			
		}
	}

	m4bool_t bOldPostValidate = m_pGlobVars->GetPostValidate ();

	m_pGlobVars->SetPostValidate (M4_TRUE);

	result = pStm->Execute ();

	m_pGlobVars->SetPostValidate (bOldPostValidate);

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}
/*
	if ((m_pMtLgcTbl->m_bCorrected) && 
		(m_pVcHistory)) {
		// Actualizamos las fechas del registro en el histórico.
		m4date_t dCurDTStartCorr = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTStartCorr));

		for (itVcHistory it = m_pVcHistory->begin (); it!=m_pVcHistory->end(); it++) {

			if ((it->m_eOperation != M4LDB_DELETE) && 
				(!M4LDB_COMPARE_DATES (it->m_dDTStartCorr, dCurDTStartCorr)) &&
				(!M4LDB_COMPARE_DATES (it->m_dDTStart, dOldDTStart))) {
				it->m_dDTStart = ai_dDTStart;
				it->m_dDTEnd = ai_dDTClose;
				break;
			}
		}
	}
*/

	if (m_pMtSent) {
		m_pMtLgcTbl->m_bInvRecordSet = M4_TRUE;
	}

	return (M4_SUCCESS);
}



m4return_t ClPersistBase::ValidateCompleteness (void)
{
	m4return_t result;

	// Hemos construido la tabla en memoria de la siguiente forma:
	// DT_START, DT_END, [DT_START_CORR, DT_END_CORR, PROJ], IS_CTRL_REP1, IS_CTRL_REP2, IS_CTRL_REP3....

	if ((m_pMtLgcTbl->m_bComplete) && 
		(!m_pMtLgcTbl->m_bInvComp)) {

		ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_VALIDATE_COMP);

		if (!pCursor) {
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::ValidateCompleteness" << "The cursor M4LDB_VALIDATE_COMP does not exist");
			return (M4_ERROR);
		}
		
		//Mejora: 0072936
		//Permitir saltarse la validacion de completitud.
		//Escenario:
		//Tenemos tablas padre (P) e hija (H). Con una relacion de completitud, es decir los registros en la
		//hija abarcan todo el periodo del padre.
		//Esto nos permitira insertar un registro en tabla hija sin necesidad de que exista en el padre o sin necesidad
		//de que abarque el periodo del padre.
		//Util en importaciones.
		//Bug 0075529.
		//En este caso, el check de hijo obligatorio es 0 pq estamos intentando insertar en hija.
		//En PrepareValidateFRs, si no hay verificacion de fechas bDTApp, bDTCorr entonces pCursor->m_bVerifyData se pone a 1.
		//Pero si hay verificacion de fechas el flag se queda a 0. En el supuesto en que estamos SI hay verificación de fechas
		//En FR no tiene en cuenta el flag de hijo necesario, si hay completitud sí se validan fechas, por lo tanto el flag sera 0.
		//Esto nos impide poner la condición pCursor->m_bVerifyData ponerla en el if.
		//En el caso de M4LDB_INV_FK_WITHOUT_PERIOD sí se puso pq la validacion de FK controla el insertar registro en
		//(P) independientemente del hijo, en este caso sí se controla el flag de hijo necesario que es el que decide si
		//debemos validar con hijo o no y en ultima instancia establece el flag a 1.
		if ( m_pGlobVars->HasNotValidation (M4LDB_INV_COMPLETENESS))
		{
			#ifdef _M4LDB_DEBUG
				M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Validate completness desactivated.");
			#endif
			return (M4_SUCCESS);
		}

		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Validate Completeness.");
		#endif

		result = pCursor->Execute ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		result = pCursor->Fetch ();

		pCursor->Close ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		if (result != M4LDB_SQL_NO_DATA_FOUND) {
			#ifdef _M4LDB_DEBUG
				m4char_t aValue[M4LDB_SIZE_TIMESTAMP + 1];

				ClDateToString (m_dCompleteDTStart, aValue, M4LDB_SIZE_TIMESTAMP + 1);
				M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), "\n\t \"%s\" ; ", aValue);

				ClDateToString (m_dCompleteDTEnd, aValue, M4LDB_SIZE_TIMESTAMP + 1);
				M4LDB_PRINTV_DEBUG (m_pGlobVars->GetpLConn (), " \"%s\" ; ", aValue);
			#endif

			m4int32_t numRow = 0;

			m4bool_t bCompError = M4_FALSE;

			m4bool_t bDelError = M4_FALSE;

			if (m_pMtLgcTbl->m_eCurTemp == M4LDB_1_PERIOD) {
				if ((m_eStmType == M4LDB_INSERT) || 
					(m_eStmType == M4LDB_UPDATE)) {
					numRow = 1;

					m4date_t dMinDTStart = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTStart));

					m4date_t dMaxDTEnd = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTEnd));

					if ((M4LDB_COMPARE_DATES (dMinDTStart, m_dCompleteDTStart)) ||
						(M4LDB_COMPARE_DATES (dMaxDTEnd, m_dCompleteDTEnd))) {
						bCompError = M4_TRUE;
					}
				}
			}
			else {
				
				numRow = m_pHistTbl->GetNumRow ();

				if (numRow) {
					m4date_t dDTStart = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTStart));
					m4date_t dDTEnd = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTEnd));

					if (m_eStmType == M4LDB_DELETE) {
						// Esto era un delete.

						if (!M4LDB_COMPARE_DATES (dDTStart, m_dCompleteDTStart)) {
							// Estamos en el delete del primer registro.
							m4date_t dDTStartN = M4LDB_DOUBLE (m_pHistTbl->GetData (0, 0));

							if (M4LDB_COMPARE_DATES (dDTStartN, m_dCompleteDTStart)) {
								// El nuevo primer registro no cumple la completitud.
						
								if (FindInTransaction (dDTStartN)) {
									// El nuevo primer registro ha sido modificado en esta transacción.
									// Dejamos la validación hasta que lleguemos a él.
									// Todo correcto ... por ahora.
								}
								else {
									if (m_pMtLgcTbl->m_eCurHist == M4LDB_N_PERIOD_AUTOMATIC_ADAPTATION) {
										// Se puede autoadaptar.										
										// Adaptamos la fecha de inicio.
										result = MoveOnHistory (m_dCompleteDTStart, M4LDB_DOUBLE (m_pHistTbl->GetData (0, 1)), dDTStartN);

										if (result == M4_ERROR) {
											return (M4_ERROR);
										}
									}
									else {
										// No se puede autoadaptar.
										bCompError = M4_TRUE;									
									}
								}
							}
							else {
								// El nuevo primer registro cumple la completitud.
								// Todo correcto.
							}
						}
						else {
							// No estamos en el delete del primer registro.
							// Todo correcto.
						}

						if (!M4LDB_COMPARE_DATES (dDTEnd, m_dCompleteDTEnd)) {
							// Estamos en el delete del último registro.
							m4date_t dDTEndN = M4LDB_DOUBLE (m_pHistTbl->GetData (numRow - 1, 1));

							if (M4LDB_COMPARE_DATES (dDTEndN, m_dCompleteDTEnd)) {
								// El nuevo último registro no cumple la completitud.
							
								m4date_t dDTStartN = M4LDB_DOUBLE (m_pHistTbl->GetData (numRow - 1, 0));

								if (FindInTransaction (dDTStartN)) {
									// El nuevo último registro ha sido modificado en esta transacción.
									// Dejamos la validación hasta que lleguemos a él.
									// Todo correcto ... por ahora.
								}
								else {
									if ((m_pMtLgcTbl->m_eCurHist == M4LDB_N_PERIOD_AUTOMATIC_ADAPTATION) || 
										(m_pMtLgcTbl->m_eCurHist == M4LDB_N_PERIOD_CLOSE_LAST_REGISTER)) {
										// Se puede autoadaptar o hacemos la apertura del registro anterior.
										// Adaptamos la fecha de fin.
										result = MoveOnHistory (M4LDB_DOUBLE (m_pHistTbl->GetData (numRow - 1, 0)), m_dCompleteDTEnd);

										if (result == M4_ERROR) {
											return (M4_ERROR);
										}
									}
									else {
										// No se puede autoadaptar.
										bCompError = M4_TRUE;									
									}
								}
							}
							else {
								// El nuevo último registro cumple la completitud.
								// Todo correcto.
							}
						}
						else {
							// No estamos en el delete del último registro.
							// Todo correcto.
						}
					}
					else {
						// Esto es un insert/update.
						m4date_t dDTStartN = M4LDB_DOUBLE (m_pHistTbl->GetData (0, 0));

						if (!M4LDB_COMPARE_DATES (dDTStart, dDTStartN)) {
							// Estamos en el insert/update del primer registro.

							if (M4LDB_COMPARE_DATES (dDTStart, m_dCompleteDTStart)) {
								// El registro no cumple la completitud.

								if (m_pMtLgcTbl->m_eCurHist == M4LDB_N_PERIOD_AUTOMATIC_ADAPTATION) {
									// Se puede autoadaptar.
									
									if ((m_eStmType == M4LDB_UPDATE) && 
										(!m_pParam->Difference (m_pMtLgcTbl->m_indDTStart))) {
										// La fecha de inicio no ha cambiado.

										m4date_t dDTEndN = M4LDB_DOUBLE (m_pHistTbl->GetData (0, 1));

										if (M4LDB_COMPARE_DATES (m_dCompleteDTStart, dDTEndN) <= 0){
											// Adaptamos la fecha de inicio.

											result = MoveOnHistory (m_dCompleteDTStart, dDTEndN, dDTStartN);

											if (result == M4_ERROR) {
												return (M4_ERROR);
											}

											// Actualizamos la fecha de inicio.
											m_pParam->SetData (m_pMtLgcTbl->m_indDTStart, (m4pcchar_t) &m_dCompleteDTStart);
											m_pHistTbl->SetData (0, 0, (m4pcchar_t) &m_dCompleteDTStart);
										}
										else {
											// No podemos adaptar la fecha de inicio porque sería mayor a la fecha de fin.
											bDelError = M4_TRUE;
										}
									}
									else {
										// No podemos autoadaptarlo porque ha sido decisión del usuario.
										bCompError = M4_TRUE;
									}
								}
								/* Bug 0205463, 211469
								Si la fecha de inicio es diferente a la fecha de inicio de la FK y no se autoadapta sólo se da error si
								o no está activado el permiso de fecha inicio anterior a la de incio de la FK
								o la fecha de inicio es posterior a la de inicio de la FK
								o la fecha fin es anterior a la de inicio de la FK
								*/
								else if(   pCursor->m_bAllowPreviousIni == M4_FALSE
										|| M4LDB_COMPARE_DATES( dDTStart, m_dCompleteDTStart ) > 0
										|| M4LDB_COMPARE_DATES( dDTEnd, m_dCompleteDTStart ) < 0 )
								{
									// No se puede autoadaptar.
									bCompError = M4_TRUE;
								}
							}
							else {
								// El nuevo último registro cumple la completitud.
								// Todo correcto.
							}
						}
						else {
							// No estamos en el insert/update del primer registro.
							// Todo correcto.
						}

						m4date_t dDTEndN = M4LDB_DOUBLE (m_pHistTbl->GetData (numRow - 1, 1));

						if (!M4LDB_COMPARE_DATES (dDTEnd, dDTEndN)) {
							// Estamos en el insert/update del último registro.

							if (M4LDB_COMPARE_DATES (dDTEnd, m_dCompleteDTEnd)) {
								// El registro no cumple la completitud.

								if (m_pMtLgcTbl->m_eCurHist == M4LDB_N_PERIOD_AUTOMATIC_ADAPTATION) {
									// Se puede autoadaptar.
									
									if ((m_eStmType == M4LDB_UPDATE) && 
										(!m_pParam->Difference (m_pMtLgcTbl->m_indDTEnd))) {
										// La fecha de fin no ha cambiado.

										m4date_t dDTStartN = M4LDB_DOUBLE (m_pHistTbl->GetData (numRow - 1, 0));

										if (M4LDB_COMPARE_DATES (dDTStartN, m_dCompleteDTEnd) <= 0){
											// Adaptamos la fecha de fin.

											result = MoveOnHistory (dDTStartN, m_dCompleteDTEnd);

											if (result == M4_ERROR) {
												return (M4_ERROR);
											}

											// Actualizamos la fecha de fin.
											m_pParam->SetData (m_pMtLgcTbl->m_indDTEnd, (m4pcchar_t) &m_dCompleteDTEnd);
											m_pHistTbl->SetData (numRow - 1, 1, (m4pcchar_t) &m_dCompleteDTEnd);
										}
										else {
											// No podemos adaptar la fecha de fin porque sería menor a la fecha de inicio.
											bDelError = M4_TRUE;
										}
									}
									else {
										// No podemos autoadaptarlo porque ha sido decisión del usuario.
										bCompError = M4_TRUE;
									}
								}
								else {
									// No se puede autoadaptar.
									bCompError = M4_TRUE;									
								}
							}
							else {
								// El nuevo último registro cumple la completitud.
								// Todo correcto.
							}
						}
						else {
							// No estamos en el insert/update del último registro.
							// Todo correcto.
						}
					}
				}
			}

			if (!numRow) {
				string stnPKRegister(M4LDB_EMPTY);
				string stiPKRegister(M4LDB_EMPTY);
				ComposePK (stnPKRegister, stiPKRegister);
				string stnFKRegister(M4LDB_EMPTY);
				string stiFKRegister(M4LDB_EMPTY);
				ComposeFK (stnFKRegister, stiFKRegister, pCursor);
				m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
				m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
				m4id_t iForLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Register[pCursor->m_indForLgcTbl].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
				m4id_t nForLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (pCursor->m_indForLgcTbl);

				DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_REFERENTIAL_CONSTRAINT_VIOLATION, 
					LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << 
					LOG_CAT_AND (iLgcTbl, nLgcTbl) <<
					LOG_CAT_AND (stiFKRegister.c_str (), stnFKRegister.c_str ()) << 
					LOG_CAT_AND (iForLgcTbl, nForLgcTbl) 
					);
				return (M4_ERROR);
			}

			if (bCompError) {
				string stnPKRegister(M4LDB_EMPTY);
				string stiPKRegister(M4LDB_EMPTY);
				ComposePK (stnPKRegister, stiPKRegister);
				m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
				m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
				m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTStart);
				m4id_t iField = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTStart);
				m4id_t nField2 = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTEnd);
				m4id_t iField2 = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTEnd);

				m4uint32_t iError;

				if( pCursor->m_bAllowPreviousIni == M4_FALSE )
				{
					iError = M4_CH_LDB_COMPLETENESS_FOREIGN_UNSATISFIED;
				}
				else
				{
					// Si se permite fecha de inicio anterior se muestra mensaje de error solo en fecha fin
					iError = M4_CH_LDB_COMPLETENESS_FOREIGN_UNSATISFIED_END;
				}

				DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), iError, 
					LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << 
					LOG_CAT_AND (iLgcTbl, nLgcTbl) <<
					LOG_CAT_AND (iField, nField) <<
					LOG_CAT_AND (iField2, nField2) <<
					// ¿Qué fechas pongo aquí?
					DateLog (m_dCompleteDTStart) << 
					DateLog (m_dCompleteDTEnd)
					);
				return (M4_ERROR);
			}

			if (bDelError) {
				string stnPKRegister(M4LDB_EMPTY);
				string stiPKRegister(M4LDB_EMPTY);
				ComposePK (stnPKRegister, stiPKRegister);
				string stnFKRegister(M4LDB_EMPTY);
				string stiFKRegister(M4LDB_EMPTY);
				ComposeFK (stnFKRegister, stiFKRegister, pCursor);
				m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
				m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
				m4id_t iForLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Register[pCursor->m_indForLgcTbl].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
				m4id_t nForLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (pCursor->m_indForLgcTbl);
				m4id_t iField = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTStart);
				m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTStart);
				m4id_t iField2 = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTEnd);
				m4id_t nField2 = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTEnd);

				DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_REFERENTIAL_PERIOD, 
					LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << 
					LOG_CAT_AND (iLgcTbl, nLgcTbl) <<
					LOG_CAT_AND (iField, nField) <<
					LOG_CAT_AND (iField2, nField2) <<
					// ¿Qué fechas pongo aquí?
					DateLog (m_dCompleteDTStart) << 
					DateLog (m_dCompleteDTEnd) << 
					LOG_CAT_AND (stiFKRegister.c_str (), stnFKRegister.c_str ()) << 
					LOG_CAT_AND (iForLgcTbl, nForLgcTbl) 
					);
				return (M4_ERROR);
			}
		}
		else {
			if (m_eStmType != M4LDB_DELETE) {
				string stnPKRegister(M4LDB_EMPTY);
				string stiPKRegister(M4LDB_EMPTY);
				ComposePK (stnPKRegister, stiPKRegister);
				string stnFKRegister(M4LDB_EMPTY);
				string stiFKRegister(M4LDB_EMPTY);
				ComposeFK (stnFKRegister, stiFKRegister, pCursor);
				m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
				m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
				m4id_t iForLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Register[pCursor->m_indForLgcTbl].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
				m4id_t nForLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (pCursor->m_indForLgcTbl);

				DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_FOREIGN_CONSTRAINT_VIOLATION, LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << LOG_CAT_AND (iLgcTbl, nLgcTbl) << LOG_CAT_AND (stiFKRegister.c_str (), stnFKRegister.c_str ()) << LOG_CAT_AND (iForLgcTbl, nForLgcTbl));
				return (M4_ERROR);
			}
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::GetHistory (void)
{
	m4return_t result;

	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_GET_HISTORY);

	if (!pCursor) {
		return (M4_SUCCESS);
	}

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Get History.");
	#endif

	result = pCursor->Execute ();

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	result = pCursor->GetData (m_pHistTbl);

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}


	/* Bug 0256302
	Se ajusta la fecha de fin del parámetro con la de base de datos
	*/
	m4int32_t numRow = m_pHistTbl->GetNumRow ();
	
	if (numRow) {

		// Hemos construido la tabla en memoria de la siguiente forma:
		// DT_START, DT_END, IS_CTRL_REP1, IS_CTRL_REP2, IS_CTRL_REP3....

		m4date_t dDTStart = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTStart));

		for (m4int32_t indRow=0; indRow<numRow; indRow++) {

			m4date_t dDTStartAux = M4LDB_DOUBLE (m_pHistTbl->GetData (indRow, 0));

			if (!M4LDB_COMPARE_DATES (dDTStart, dDTStartAux)) {
				// Hemos encontrado el registro. Lo validamos en el anterior y el posterior.

				m4date_t dDTEnd = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTEnd));
				m4date_t dDTEndAux = M4LDB_DOUBLE (m_pHistTbl->GetData (indRow, 1));

				if (M4LDB_COMPARE_DATES (dDTEndAux, dDTEnd)) {
					// Actualizamos la fecha de fin si no es la misma que la de base de datos

					#ifdef _M4LDB_DEBUG
						m4char_t aBefore[M4LDB_SIZE_TIMESTAMP + 1];
						m4char_t aAfter[M4LDB_SIZE_TIMESTAMP + 1];
						ClDateToString (dDTEnd, aBefore, M4LDB_SIZE_TIMESTAMP + 1);
						ClDateToString (dDTEndAux, aAfter, M4LDB_SIZE_TIMESTAMP + 1);
						M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn(), "\n\tChanging End Date \"%s\" to \"%s\"", aBefore, aAfter);
						M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_ALWAYS, m_pGlobVars->GetpLConn(), "\n\tChanging End Date \"%s\" to \"%s\"", aBefore, aAfter);
					#endif

					m_pParam->SetData (m_pMtLgcTbl->m_indDTEnd, (m4pcchar_t) &dDTEndAux);
				}

				break;
			}
		}
	}

	return (result);
}


m4return_t ClPersistBase::ValidateHistory (void)
{
	m4return_t result;

	m4int32_t numRow = m_pHistTbl->GetNumRow ();
	
	if (numRow) {

		// Hemos construido la tabla en memoria de la siguiente forma:
		// DT_START, DT_END, IS_CTRL_REP1, IS_CTRL_REP2, IS_CTRL_REP3....

		m4date_t dDTStart = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTStart));

		// Todo esto funciona bién porque está ordenado, ¿verdad?, ¿no habrás quitado el ORDER BY?
		if (m_eStmType == M4LDB_DELETE) {

			for (m4int32_t indRow=0; indRow<numRow; indRow++) {

				m4date_t dDTStartAux = M4LDB_DOUBLE (m_pHistTbl->GetData (indRow, 0));

				if (M4LDB_COMPARE_DATES (dDTStart, dDTStartAux) < 0) {
					// Este registro es el siguiente por lo que cojemos el anterior.

					if (!indRow) {
						// No tenemos anterior por lo que nos vamos.
						return (M4_SUCCESS);
					}

					if ((m_pMtLgcTbl->m_bComplete) && 
						(!m_pMtLgcTbl->m_bInvComp)) {

						result = ValidateHistoryNoLikeAdjacentRegister (indRow-1);

						if (result == M4_ERROR) {
							return (M4_ERROR);
						}
					}

					result = ValidateHistoryAutomaticAdaptation (indRow-1);

					if (result == M4_ERROR) {
						return (M4_ERROR);
					}

					return (M4_SUCCESS);
				}
			}
		}
		else {
			for (m4int32_t indRow=0; indRow<numRow; indRow++) {

				m4date_t dDTStartAux = M4LDB_DOUBLE (m_pHistTbl->GetData (indRow, 0));

				if (!M4LDB_COMPARE_DATES (dDTStart, dDTStartAux)) {
					// Hemos encontrado el registro. Lo validamos en el anterior y el posterior.

					if (indRow) {
						// Tenemos anterior.
						if ((m_pMtLgcTbl->m_bComplete) && 
							(!m_pMtLgcTbl->m_bInvComp)) {

							result = ValidateHistoryNoLikeAdjacentRegister (indRow-1);

							if (result == M4_ERROR) {
								return (M4_ERROR);
							}
						}

						result = ValidateHistoryAutomaticAdaptation (indRow-1);

						if (result == M4_ERROR) {
							return (M4_ERROR);
						}
					}

					if (indRow != numRow-1) {
						// Tenemos posterior.
						if ((m_pMtLgcTbl->m_bComplete) && 
							(!m_pMtLgcTbl->m_bInvComp)) {

							result = ValidateHistoryNoLikeAdjacentRegister (indRow);

							if (result == M4_ERROR) {
								return (M4_ERROR);
							}
						}

						result = ValidateHistoryAutomaticAdaptation (indRow);

						if (result == M4_ERROR) {
							return (M4_ERROR);
						}
					}

					return (M4_SUCCESS);
				}
			}
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ValidateHistoryNoLikeAdjacentRegister (m4int32_t ai_indRow)
{
	m4bool_t bFound = M4_FALSE;

	// Hemos construido la tabla en memoria de la siguiente forma:
	// DT_START, DT_END, IS_CTRL_REP1, IS_CTRL_REP2, IS_CTRL_REP3....
	// Por eso empezamos por la tercera columna.

	m4int32_t numCol = m_pHistTbl->GetNumCol ();

	for (m4int32_t indCol=2; indCol<numCol; indCol++) {

		bFound = M4_TRUE;

		m4int8_t m4Type = m_pHistTbl->GetM4Type (indCol);

		m4pcchar_t pCurrValue = m_pHistTbl->GetData (ai_indRow, indCol);

		m4pcchar_t pNextValue = m_pHistTbl->GetData (ai_indRow + 1, indCol);

		if (M4LDB_COMPARE_VALUES (pCurrValue, pNextValue, m4Type)) {
			return (M4_SUCCESS);
		}
	}

	if (!bFound) {
		// No se ha definido ningún campo para el control de repetición.
		return (M4_SUCCESS);
	}

	string stnPKRegister(M4LDB_EMPTY);
	string stiPKRegister(M4LDB_EMPTY);
	ComposePK (stnPKRegister, stiPKRegister);
	string stnFields(M4LDB_EMPTY);
	string stiFields(M4LDB_EMPTY);
	ComposeRPFields (stnFields, stiFields);
	m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
	m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;

	if (m_eStmType == M4LDB_DELETE) {
		DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_REPETITION_CONTROL_PREV_NEXT, LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << LOG_CAT_AND (iLgcTbl, nLgcTbl) << LOG_CAT_AND (stiFields.c_str (), stnFields.c_str ()));
	}
	else {
		m4date_t dDTStart = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTStart));

		m4date_t dCurrDTStart = M4LDB_DOUBLE (m_pHistTbl->GetData (ai_indRow, 0));

		if (!M4LDB_COMPARE_DATES (dCurrDTStart, dDTStart)) {
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_REPETITION_CONTROL_CURR_NEXT, LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << LOG_CAT_AND (iLgcTbl, nLgcTbl) << LOG_CAT_AND (stiFields.c_str (), stnFields.c_str ()));
		}
		else {
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_REPETITION_CONTROL_PREV_CURR, LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << LOG_CAT_AND (iLgcTbl, nLgcTbl) << LOG_CAT_AND (stiFields.c_str (), stnFields.c_str ()));
		}
	}

	return (M4_ERROR);
}


m4return_t ClPersistBase::ValidateHistoryAutomaticAdaptation (m4int32_t ai_indRow)
{
	m4return_t result = M4_SUCCESS;

	// Hemos construido la tabla en memoria de la siguiente forma:
	// DT_START, DT_END, IS_CTRL_REP1, IS_CTRL_REP2, IS_CTRL_REP3....

	m4date_t dCurrDTStart = M4LDB_DOUBLE (m_pHistTbl->GetData (ai_indRow, 0));

	m4date_t dCurrDTEnd = M4LDB_DOUBLE (m_pHistTbl->GetData (ai_indRow, 1));

	m4date_t dNextDTStart = M4LDB_DOUBLE (m_pHistTbl->GetData (ai_indRow + 1, 0));

	m4date_t dNextDTEnd = M4LDB_DOUBLE (m_pHistTbl->GetData (ai_indRow + 1, 1));

	m4bool_t bCompError = M4_FALSE;

	m4bool_t bDelError = M4_FALSE;

	m4bool_t bOverError = M4_FALSE;

	if (m_eStmType == M4LDB_DELETE) {
		// Esto era un delete.

		if (M4LDB_COMPARE_DATES (dCurrDTEnd, dNextDTStart) < 0) {
			// No hay solapamiento.

			if ((m_pMtLgcTbl->m_bComplete) && 
				(!m_pMtLgcTbl->m_bInvComp)) {
				// Requiere completitud.

				if (M4LDB_COMPARE_DATES (dCurrDTEnd + m_pMtLgcTbl->m_dDateUnit, dNextDTStart) < 0) {
					// Existe hueco.

					if ((!FindInTransaction (dCurrDTStart)) && 
						(!FindInTransaction (dNextDTStart))) {
						// Adaptamos la fecha de fin del current.

						if (m_pMtLgcTbl->m_eCurHist == M4LDB_N_PERIOD_AUTOMATIC_ADAPTATION) {
							// Se puede autoadaptar.
							result = MoveOnHistory (dCurrDTStart, dNextDTStart - m_pMtLgcTbl->m_dDateUnit);
						}
						else {
							// No se puede autoadaptar.
							bCompError = M4_TRUE;
						}
					}
					else {
						// Alguno de los dos registros ha sido modificado en esta transacción.
						// Dejamos la validación hasta que lleguemos a él.
						// Todo correcto ... por ahora.
					}
				}
				else {
					// No existe hueco.Todo correcto.
				}
			}
			else {
				// No requiere completitud. Todo correcto.
			}
		}
		else {
			// Hay solapamiento. 
			if ((!FindInTransaction (dCurrDTStart)) && 
				(!FindInTransaction (dNextDTStart))) {
				// Esto no debería de haberse producido.
				// Aun así damos error de solapamiento.
				bOverError = M4_TRUE;
			}
			else {
				// Alguno de los dos registros ha sido modificado en esta transacción.
				// Dejamos la validación hasta que lleguemos a él.
				// Todo correcto ... por ahora.
			}
		}
	}
	else {
		// Esto es un insert/update.
		if (M4LDB_COMPARE_DATES (dCurrDTEnd, dNextDTStart) < 0) {
			// No hay solapamiento.

			if ((m_pMtLgcTbl->m_bComplete) && 
				(!m_pMtLgcTbl->m_bInvComp)) {
				// Requiere completitud.

				if (M4LDB_COMPARE_DATES (dCurrDTEnd + m_pMtLgcTbl->m_dDateUnit, dNextDTStart) < 0) {
					// Existe hueco.

					m4date_t dDTStart = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTStart));

					if (!M4LDB_COMPARE_DATES (dCurrDTStart, dDTStart)) {
						// El current es el registro actual.

						if (m_pMtLgcTbl->m_eCurHist == M4LDB_N_PERIOD_AUTOMATIC_ADAPTATION) {
							// Se puede autoadaptar.
						
							if ((!FindInTransaction (dNextDTStart)) && 
								((m_eStmType == M4LDB_INSERT) || 
								(m_pParam->Difference (m_pMtLgcTbl->m_indDTEnd)))) {
								// La fecha de fin ha cambiado o el registro es nuevo y no hay cambios en el registro siguiente.
								// Adaptamos la fecha de inicio del siguiente
								result = MoveOnHistory (dCurrDTEnd + m_pMtLgcTbl->m_dDateUnit, dNextDTEnd, dNextDTStart);
							}
							else {
								m4date_t dDTAux = dNextDTStart - m_pMtLgcTbl->m_dDateUnit;

								// Adaptamos la fecha de fin del current.
								result = MoveOnHistory (dCurrDTStart, dDTAux);

								// Actualizamos la fecha de fin.
								m_pParam->SetData (m_pMtLgcTbl->m_indDTEnd, (m4pcchar_t) &dDTAux);
								m_pHistTbl->SetData (ai_indRow, 1, (m4pcchar_t) &dDTAux);
							}
						}
						else {
							// No se puede autoadaptar.
							bCompError = M4_TRUE;
						}
					}
					else {
						// El current es el registro anterior.

						if (!FindInTransaction (dCurrDTStart)) {
							// El registro anterior no ha sido modificado por el usuario.

							if (m_pMtLgcTbl->m_eCurHist == M4LDB_N_PERIOD_AUTOMATIC_ADAPTATION) {
								// Se puede autoadaptar.
								// Adaptamos la fecha de fin del registro anterior.
								result = MoveOnHistory (dCurrDTStart, dNextDTStart - m_pMtLgcTbl->m_dDateUnit);
							}
							else {
								// No se puede autoadaptar.
								bCompError = M4_TRUE;
							}
						}
						else {
							// El registro anterior ha sido modificado en esta transacción.
							// Dejamos la validación hasta que lleguemos a él.
							// Todo correcto ... por ahora.
						}
					}
				}
				else {
					// No existe hueco.Todo correcto.
				}
			}
			else {
				// No requiere completitud. Todo correcto.
			}
		}
		else {
			// Hay solapamiento.

			m4date_t dDTStart = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTStart));

			if (!M4LDB_COMPARE_DATES (dCurrDTStart, dDTStart)) {
				// El current es el registro actual.

				if ((!FindInTransaction (dNextDTStart)) && 
					((m_eStmType == M4LDB_INSERT) || 
					(m_pParam->Difference (m_pMtLgcTbl->m_indDTEnd)))) {
					// La fecha de fin ha cambiado y no hay cambios en el registro siguiente.

					if (M4LDB_COMPARE_DATES (dCurrDTEnd + m_pMtLgcTbl->m_dDateUnit, dNextDTEnd) > 0){
						// No podemos adaptar la fecha de inicio porque sería mayor a la fecha de fin.
						bDelError = M4_TRUE;
					}
					else {
						if (m_pMtLgcTbl->m_eCurHist == M4LDB_N_PERIOD_AUTOMATIC_ADAPTATION) {
							// Se puede autoadaptar.				
							// Adaptamos la fecha de inicio del siguiente
							result = MoveOnHistory (dCurrDTEnd + m_pMtLgcTbl->m_dDateUnit, dNextDTEnd, dNextDTStart);
						}
						else {
							// No se puede autoadaptar.
							bOverError = M4_TRUE;
						}
					}
				}
				else {
					m4date_t dDTAux = dNextDTStart - m_pMtLgcTbl->m_dDateUnit;

					if (M4LDB_COMPARE_DATES (dDTAux, dCurrDTStart) < 0){
						// No podemos adaptar la fecha de fin porque sería menor a la fecha de inicio.
						bDelError = M4_TRUE;
					}
					else {
						if ((m_pMtLgcTbl->m_eCurHist == M4LDB_N_PERIOD_AUTOMATIC_ADAPTATION) || 
							(m_pMtLgcTbl->m_eCurHist == M4LDB_N_PERIOD_CLOSE_LAST_REGISTER)) {
							// Se puede autoadaptar o hacemos cierre del registro anterior.
							// Adaptamos la fecha de fin del current.
							result = MoveOnHistory (dCurrDTStart, dDTAux);

							// Actualizamos la fecha de fin.
							m_pParam->SetData (m_pMtLgcTbl->m_indDTEnd, (m4pcchar_t) &dDTAux);
							m_pHistTbl->SetData (ai_indRow, 1, (m4pcchar_t) &dDTAux);
						}
						else {
							// No se puede autoadaptar.
							bOverError = M4_TRUE;
						}
					}
				}
			}
			else {
				// El current es el registro anterior.

				if (!FindInTransaction (dCurrDTStart)) {
					// El registro anterior no ha sido modificado por el usuario.

					if (M4LDB_COMPARE_DATES (dNextDTStart - m_pMtLgcTbl->m_dDateUnit, dCurrDTStart) < 0){
						// No podemos adaptar la fecha de fin porque sería menor a la fecha de inicio.
						bDelError = M4_TRUE;
					}
					else {
						if ((m_pMtLgcTbl->m_eCurHist == M4LDB_N_PERIOD_AUTOMATIC_ADAPTATION) || 
							(m_pMtLgcTbl->m_eCurHist == M4LDB_N_PERIOD_CLOSE_LAST_REGISTER)) {
							// Se puede autoadaptar o hacemos cierre del registro anterior.
							// Adaptamos la fecha de fin del registro anterior.
							result = MoveOnHistory (dCurrDTStart, dNextDTStart - m_pMtLgcTbl->m_dDateUnit);
						}
						else {
							// No se puede autoadaptar.
							bOverError = M4_TRUE;
						}
					}
				}
				else {
					// El registro anterior ha sido modificado en esta transacción.
					// Dejamos la validación hasta que lleguemos a él.
					// Todo correcto ... por ahora.
				}
			}
		}
	}
	
	if (bCompError) {
		string stnPKRegister(M4LDB_EMPTY);
		string stiPKRegister(M4LDB_EMPTY);
		ComposePK (stnPKRegister, stiPKRegister);
		m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
		m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
		m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTStart);
		m4id_t iField = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTStart);
		m4id_t nField2 = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTEnd);
		m4id_t iField2 = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTEnd);

		DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_COMPLETENESS_UNSATISFIED, LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << LOG_CAT_AND (iLgcTbl, nLgcTbl) << LOG_CAT_AND (iField, nField) << LOG_CAT_AND (iField2, nField2) << DateLog (M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTStart))) << DateLog (M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTEnd))));
		return (M4_ERROR);
	}

	if (bDelError) {
		string stnPKRegister(M4LDB_EMPTY);
		string stiPKRegister(M4LDB_EMPTY);
		ComposePK (stnPKRegister, stiPKRegister);
		m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
		m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;

		DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_DELETE_ON_HISTORY, LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << LOG_CAT_AND (iLgcTbl, nLgcTbl));
		return (M4_ERROR);
	}

	if (bOverError) {
		string stnPKRegister(M4LDB_EMPTY);
		string stiPKRegister(M4LDB_EMPTY);
		ComposePK (stnPKRegister, stiPKRegister);
		m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
		m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;

		// Error de solapamiento.
		DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_OVERLAPED_NO_ALLOWED, LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << LOG_CAT_AND (iLgcTbl, nLgcTbl));
		return (M4_ERROR);
	}

	return (result);
}


m4return_t ClPersistBase::GetCorrectedHistory (void)
{
	m4return_t result;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Get Corrected History.");
	#endif

	// Hemos construido la tabla en memoria de la siguiente forma:
	// DT_START, DT_END, DT_START_CORR, DT_END_CORR, IS_CTRL_REP1, IS_CTRL_REP2, IS_CTRL_REP3....

	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_GET_CORR_HISTORY);

	if (!pCursor) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::GetCorrectedHistory" << "The cursor M4LDB_GET_CORR_HISTORY does not exist");
		return (M4_ERROR);
	}

	result = pCursor->Execute ();

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	result = pCursor->GetData (m_pCorrHistTbl);

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	m_pMtLgcTbl->m_bInvComp = M4_FALSE;

	m4int32_t numRow = m_pCorrHistTbl->GetNumRow ();

	if (!numRow) {
		return (M4_SUCCESS);
	}

	if (m_pMtLgcTbl->m_eCurTemp == M4LDB_N_PERIOD) {
		if (!m_pVcHistory) {
			m_pVcHistory = new VcHistory;
			m_pVcHistory->reserve(10);
		}

		m_pVcHistory->clear ();

		m4date_t dCurDTStartCorr = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTStartCorr));

		for (m4int32_t indRow=0; indRow<numRow; indRow++) {

			struct stHistory oHist;

			oHist.m_indRow = indRow;

			oHist.m_dDTStart = M4LDB_DOUBLE (m_pCorrHistTbl->GetData (indRow, 0));

			oHist.m_dDTEnd = M4LDB_DOUBLE (m_pCorrHistTbl->GetData (indRow, 1));

			oHist.m_dDTStartCorr = M4LDB_DOUBLE (m_pCorrHistTbl->GetData (indRow, 2));

			oHist.m_dDTEndCorr = M4LDB_DOUBLE (m_pCorrHistTbl->GetData (indRow, 3));

			oHist.m_dOldDTEndCorr = oHist.m_dDTEndCorr;

			oHist.m_eRegType = (eRegType_t) M4LDB_NUMBER (m_pCorrHistTbl->GetData (indRow, 4));

			oHist.m_eOldRegType = oHist.m_eRegType;

			if ((oHist.m_eRegType != M4LDB_CLOSED_REG) && 
				(oHist.m_eRegType != M4LDB_CLOSED_LOCK_REG) && 
				(M4LDB_COMPARE_DATES (oHist.m_dDTEndCorr, M4LDB_PLUS_INF_DATE))) {
				// Actualizamos su fecha de fin de corrección a mas infinito.
				oHist.m_dDTEndCorr = M4LDB_PLUS_INF_DATE;

				oHist.m_eOperation = M4LDB_UPDATE;

				if (oHist.m_eRegType == M4LDB_NORMAL_LOCK_REG) {
					oHist.m_eRegType = M4LDB_NORMAL_REG;
				}
			}

			if (M4LDB_COMPARE_DATES (oHist.m_dDTStartCorr, dCurDTStartCorr) < 0) {
				
				if (!m_pMtLgcTbl->m_bInvComp) {
					// Hay correcciones anteriores. No estamos en el base.
					m_pMtLgcTbl->m_bInvComp = M4_TRUE;

					m4int32_t lCorrMask = 1 << (((m4int32_t) m_eStmType) + 4);		

					if (!(m_pMtLgcTbl->m_lSecMask & lCorrMask)) {
						m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
						m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;

						switch (m_eStmType) {
							case M4LDB_INSERT:
								DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INSERT_CORRECTION_REFUSE, LOG_CAT_AND (iLgcTbl, nLgcTbl)
									);
								break;
							case M4LDB_UPDATE:
								DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_UPDATE_CORRECTION_REFUSE, LOG_CAT_AND (iLgcTbl, nLgcTbl)
									);
								break;
							case M4LDB_DELETE:
								DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_DELETE_CORRECTION_REFUSE, LOG_CAT_AND (iLgcTbl, nLgcTbl)
									);
								break;
						}
						return (M4_ERROR);
					}
				}

				if (oHist.m_eRegType == M4LDB_PROJECTION_LOCK_REG) {
					oHist.m_eRegType = M4LDB_PROJECTION_REG;
				}
			}
			else {
				if ((oHist.m_eRegType == M4LDB_PROJECTION_REG) || 
					(oHist.m_eRegType == M4LDB_PROJECTION_LOCK_REG)) {
					oHist.m_eOperation = M4LDB_DELETE;
				}
			}

			m_pVcHistory->push_back (oHist);
		}
	}
	else {
		m4date_t dCurDTStartCorr = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTStartCorr));

		for (m4int32_t indRow=0; indRow<numRow; indRow++) {

			m4date_t dDTStartCorr = M4LDB_DOUBLE (m_pCorrHistTbl->GetData (indRow, 0));

			if (M4LDB_COMPARE_DATES (dDTStartCorr, dCurDTStartCorr) < 0) {

				if (!m_pMtLgcTbl->m_bInvComp) {
					// Hay correcciones anteriores. No estamos en el base.
					m_pMtLgcTbl->m_bInvComp = M4_TRUE;

					m4int32_t lCorrMask = 1 << (((m4int32_t) m_eStmType) + 3);		

					if (!(m_pMtLgcTbl->m_lSecMask & lCorrMask)) {
						m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
						m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;

						switch (m_eStmType) {
							case M4LDB_INSERT:
								DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INSERT_CORRECTION_REFUSE, LOG_CAT_AND (iLgcTbl, nLgcTbl)
									);
								break;
							case M4LDB_UPDATE:
								DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_UPDATE_CORRECTION_REFUSE, LOG_CAT_AND (iLgcTbl, nLgcTbl)
									);
								break;
							case M4LDB_DELETE:
								DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_DELETE_CORRECTION_REFUSE, LOG_CAT_AND (iLgcTbl, nLgcTbl)
									);
								break;
						}
						return (M4_ERROR);
					}
				}

				break;
			}
		}
	}


	return (M4_SUCCESS);
}


m4return_t ClPersistBase::CompleteHistoryCorrectionDate (void)
{
	m4return_t result;

	m4int32_t indReg;

	if ((m_pVcHistory) && 
		(m_pMtSent)) {
		m4date_t dCurDTStartCorr = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTStartCorr));

		m4int32_t numReg;

		// Vamos a validar que el cambio no afecte a ninguna corrección futura.

		m4date_t dDTStart = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTStart));

		m4date_t dDTEnd = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTEnd));

		m4date_t dDTCurStart = M4LDB_DOUBLE (m_pParam->GetOldData (m_pMtLgcTbl->m_indDTStart));

		m4date_t dDTCurEnd = M4LDB_DOUBLE (m_pParam->GetOldData (m_pMtLgcTbl->m_indDTEnd));

		for (itVcHistory it = m_pVcHistory->begin (); it!=m_pVcHistory->end(); it++) {

			if ((it->m_eOperation != M4LDB_DELETE) && 
				(!FindInTransaction (it->m_dDTStart, it->m_dDTStartCorr)) && 
				(M4LDB_COMPARE_DATES (it->m_dDTStartCorr, dCurDTStartCorr) > 0) &&
				((M4LDB_COMPARE_DATES (it->m_dDTEnd, dDTStart) >= 0) || 
				(M4LDB_COMPARE_DATES (it->m_dDTEnd, dDTCurStart) >= 0)) && 
				((M4LDB_COMPARE_DATES (it->m_dDTStart, dDTEnd) <= 0) || 
				(M4LDB_COMPARE_DATES (it->m_dDTStart, dDTCurEnd) <= 0))) {
				string stnPKRegister(M4LDB_EMPTY);
				string stiPKRegister(M4LDB_EMPTY);
				ComposePK (stnPKRegister, stiPKRegister);
				m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
				m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;

				DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_NO_LATEST_CORRECTION, LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << LOG_CAT_AND (iLgcTbl, nLgcTbl));
				return (M4_ERROR);
			}
		}

		while (dCurDTStartCorr) {

			numReg = m_pVcHistory->size ();

			struct stHistory *pBefHist = new stHistory [numReg];

			m4int32_t numBefReg = 0;

			// Leemos todos los registros del histórico anterior que se propagan después de la 
			// actual fecha de inicio de corrección.
			for (indReg=0; indReg<numReg; indReg++) {

				struct stHistory &oReg = (*m_pVcHistory)[indReg];

				if ((oReg.m_eOperation != M4LDB_DELETE) && 
					(M4LDB_COMPARE_DATES (oReg.m_dDTStartCorr, dCurDTStartCorr) < 0) &&
					(M4LDB_COMPARE_DATES (oReg.m_dDTEndCorr, dCurDTStartCorr) > 0)) {
					// Actualizamos el índice del registro.
					oReg.m_indReg = indReg;

					pBefHist[numBefReg].m_indRow = oReg.m_indRow;
					pBefHist[numBefReg].m_indReg = oReg.m_indReg;
					pBefHist[numBefReg].m_dDTStart = oReg.m_dDTStart;
					pBefHist[numBefReg].m_dDTStartCorr = oReg.m_dDTStartCorr;
					pBefHist[numBefReg].m_dDTEnd = oReg.m_dDTEnd;
					pBefHist[numBefReg].m_dDTEndCorr = oReg.m_dDTEndCorr;
					pBefHist[numBefReg].m_eOperation = oReg.m_eOperation;
					pBefHist[numBefReg].m_eRegType = oReg.m_eRegType;
					pBefHist[numBefReg].m_dOldDTEndCorr = oReg.m_dOldDTEndCorr;
					pBefHist[numBefReg].m_eOldRegType = oReg.m_eOldRegType;

					numBefReg++;
				}
			}

			qsort( (void *)pBefHist, (size_t)numBefReg, sizeof( struct stHistory ), NewOrderByApp );

			m4bool_t bInvHist = M4_TRUE;

			struct stHistory *pCurHist = NULL;

			m4int32_t numCurReg = 0;

			m4int32_t indBefReg;

			for (indBefReg=0; indBefReg<numBefReg; indBefReg++) {

				// Fecha de inicio de aplicación del registro actual del histórico anterior.
				m4date_t dBefS = pBefHist[indBefReg].m_dDTStart;

				// Fecha de fin de aplicación del registro actual del histórico anterior.
				m4date_t dBefE = pBefHist[indBefReg].m_dDTEnd;
				
				if (bInvHist) {

					numReg = m_pVcHistory->size ();

					if (pCurHist) delete [] pCurHist;

					pCurHist = new stHistory [numReg];

					numCurReg = 0;

					// Leemos todos los registros del histórico actual.
					for (indReg=0; indReg<numReg; indReg++) {

						struct stHistory &oReg = (*m_pVcHistory)[indReg];

						if ((oReg.m_eOperation != M4LDB_DELETE) && 
							(!M4LDB_COMPARE_DATES (oReg.m_dDTStartCorr, dCurDTStartCorr)) && 
							(M4LDB_COMPARE_DATES (oReg.m_dDTEnd, dBefS) >= 0)) {

							pCurHist[numCurReg].m_indRow = oReg.m_indRow;
							pCurHist[numCurReg].m_indReg = oReg.m_indReg;
							pCurHist[numCurReg].m_dDTStart = oReg.m_dDTStart;
							pCurHist[numCurReg].m_dDTStartCorr = oReg.m_dDTStartCorr;
							pCurHist[numCurReg].m_dDTEnd = oReg.m_dDTEnd;
							pCurHist[numCurReg].m_dDTEndCorr = oReg.m_dDTEndCorr;
							pCurHist[numCurReg].m_eOperation = oReg.m_eOperation;
							pCurHist[numCurReg].m_eRegType = oReg.m_eRegType;
							pCurHist[numCurReg].m_dOldDTEndCorr = oReg.m_dOldDTEndCorr;
							pCurHist[numCurReg].m_eOldRegType = oReg.m_eOldRegType;

							numCurReg++;
						}
					}

					qsort( (void *)pCurHist, (size_t)numCurReg, sizeof( struct stHistory ), NewOrderByApp );

					bInvHist = M4_FALSE;
				}

				m4bool_t bInvNext = M4_FALSE;

				m4int32_t indCurReg;

				for (indCurReg=0; indCurReg<numCurReg; indCurReg++) {

					if (indCurReg > 0) {
						// Esto es para no generar mas proyecciones de la cuenta.
						dBefS = pCurHist[indCurReg-1].m_dDTEnd + m_pMtLgcTbl->m_dDateUnit;
						if (M4LDB_COMPARE_DATES (pBefHist[indBefReg].m_dDTStart, dBefS) > 0){
							dBefS = pBefHist[indBefReg].m_dDTStart;
						}
					}
					else {
						dBefS = pBefHist[indBefReg].m_dDTStart;
					}
					
					if (indCurReg + 1 < numCurReg) {
						// Esto es para no generar mas proyecciones de la cuenta.
						dBefE = pCurHist[indCurReg+1].m_dDTStart - m_pMtLgcTbl->m_dDateUnit;
						if (M4LDB_COMPARE_DATES (pBefHist[indBefReg].m_dDTEnd, dBefE) < 0){
							dBefE = pBefHist[indBefReg].m_dDTEnd;
						}
					}
					else {
						dBefE = pBefHist[indBefReg].m_dDTEnd;
					}

					// Fecha de inicio de aplicación del registro actual del histórico actual.
					m4date_t dCurS = pCurHist[indCurReg].m_dDTStart;

					// Fecha de fin de aplicación del registro actual del histórico actual.
					m4date_t dCurE = pCurHist[indCurReg].m_dDTEnd;
					
					if ((M4LDB_COMPARE_DATES (dBefS, dCurE) > 0) || 
						(M4LDB_COMPARE_DATES (dBefE, dCurS) < 0)) {
						// No existe proyección.
						continue;
					}

					m4bool_t bLock = (pCurHist[indCurReg].m_eRegType != M4LDB_PROJECTION_REG) ? M4_TRUE : M4_FALSE;

					if (M4LDB_COMPARE_DATES (dBefS, dCurS) < 0) {
						// Existe proyección de la parte inicial del registro actual del histórico anterior.

						bLock = M4_TRUE;

						if (!bInvNext) {
							// No ha sido invalidada anteriormente.

							m4bool_t bInvProj = M4_FALSE;

							// Averiguamos si son iguales el registro actual del histórico anterior y el 
							// registro actual del histórico actual.
							if (CompareRegisters (pBefHist[indBefReg].m_indRow, pCurHist[indCurReg].m_indRow) == M4_TRUE) {
								// Tenemos que proyectar el registro anterior del histórico anterior.

								if (!m_pMtLgcTbl->m_bComplete) {
									bInvProj = M4_TRUE;
								}
								else {
									if (!indBefReg) {
										// No existe un registro anterior en el histórico anterior.
										string stnPKRegister(M4LDB_EMPTY);
										string stiPKRegister(M4LDB_EMPTY);
										ComposePK (stnPKRegister, stiPKRegister);
										string stnFields(M4LDB_EMPTY);
										string stiFields(M4LDB_EMPTY);
										ComposeRPFields (stnFields, stiFields);
										m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
										m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;

										DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_PREVIOUS_PROJECTION, LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << LOG_CAT_AND (iLgcTbl, nLgcTbl) << LOG_CAT_AND (stiFields.c_str (), stnFields.c_str ()));
										if (pBefHist) delete [] pBefHist;
										if (pCurHist) delete [] pCurHist;
										return (M4_ERROR);
									}

									// Nos vamos al registro anterior.
									indBefReg--;
								}
							}

							if (!bInvProj) {

								struct stHistory oHist (pBefHist[indBefReg]);

								// Insertamos el registro proyección en el histórico corregido de registros.
								oHist.m_dDTStartCorr = pCurHist[indCurReg].m_dDTStartCorr;

								oHist.m_dDTEnd = dCurS - m_pMtLgcTbl->m_dDateUnit;

/*								if (M4LDB_COMPARE_DATES (oHist.m_dDTStart, oHist.m_dDTEnd) > 0) {
									string stPKRegister(M4LDB_EMPTY);
									ComposePK (stPKRegister);
									DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_PROJECTION, stPKRegister.c_str ());
									return (M4_ERROR);
								}*/

								oHist.m_eOperation = M4LDB_INSERT;

								oHist.m_eRegType = M4LDB_PROJECTION_REG;

								m_pVcHistory->push_back (oHist);
							}
						}

						bInvNext = M4_FALSE;
					}


					if (M4LDB_COMPARE_DATES (dBefE, dCurE) > 0) {
						// Existe proyección de la parte final del registro actual del histórico anterior.

						bLock = M4_TRUE;
						
						 // Esto debería de ser un bool, pero por si acaso...
						m4char_t bAft = 0;

						m4bool_t bInvProj = M4_FALSE;

						// Averiguamos si son iguales el registro actual del histórico anterior y el 
						// registro actual del histórico actual.
						if ((indBefReg + 1 <= numBefReg) && 
							(CompareRegisters (pBefHist[indBefReg].m_indRow, pCurHist[indCurReg].m_indRow) == M4_TRUE)) {
							// Tenemos que proyectar el registro siguiente del histórico anterior.

							if (m_pMtLgcTbl->m_bComplete) {
								bInvProj = M4_TRUE;
							}
							else {
								bAft = 1;
							}
						}

						if (!bInvProj) {

							struct stHistory oHist (pBefHist[indBefReg + bAft]);

							m4date_t dNCurS = 0;

							if (indCurReg + 1 < numCurReg) {
								// Existe un registro siguiente.

								// Fecha de inicio de aplicación del registro siguiente del histórico actual.
								dNCurS = pCurHist[indCurReg + 1].m_dDTStart;
							}

							if ((dNCurS) && 
								(M4LDB_COMPARE_DATES (dBefE, dNCurS - m_pMtLgcTbl->m_dDateUnit) >= 0)) {
								// Existe adyacencia del registro siguiente del histórico actual.

								if (M4LDB_COMPARE_DATES (dCurE, dNCurS - m_pMtLgcTbl->m_dDateUnit)) {
									// Existe hueco para la projección.

									// Averiguamos si son iguales el registro actual del histórico anterior y el 
									// registro siguiente del histórico actual.
									if (CompareRegisters (pBefHist[indBefReg].m_indRow, pCurHist[indCurReg + 1].m_indRow) == M4_TRUE) {
										string stnPKRegister(M4LDB_EMPTY);
										string stiPKRegister(M4LDB_EMPTY);
										ComposePK (stnPKRegister, stiPKRegister);
										string stnFields(M4LDB_EMPTY);
										string stiFields(M4LDB_EMPTY);
										ComposeRPFields (stnFields, stiFields);
										m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
										m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;

										DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_PROJECTION, LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << LOG_CAT_AND (iLgcTbl, nLgcTbl) << LOG_CAT_AND (stiFields.c_str (), stnFields.c_str ()));
										if (pBefHist) delete [] pBefHist;
										if (pCurHist) delete [] pCurHist;
										return (M4_ERROR);
									}

									oHist.m_dDTEnd = dNCurS - m_pMtLgcTbl->m_dDateUnit;
								}
								else {
									// No existe hueco, con lo cual no proyectamos.
									bInvProj = M4_TRUE;
								}

								bInvNext = M4_TRUE;
							}

							if (!bInvProj) {
								if ((bAft) && 
									(indBefReg + 1 == numBefReg)) {
									// No existe un registro siguiente en el histórico anterior.
									string stnPKRegister(M4LDB_EMPTY);
									string stiPKRegister(M4LDB_EMPTY);
									ComposePK (stnPKRegister, stiPKRegister);
									string stnFields(M4LDB_EMPTY);
									string stiFields(M4LDB_EMPTY);
									ComposeRPFields (stnFields, stiFields);
									m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
									m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;

									DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_NEXT_PROJECTION, LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << LOG_CAT_AND (iLgcTbl, nLgcTbl) << LOG_CAT_AND (stiFields.c_str (), stnFields.c_str ()));
									if (pBefHist) delete [] pBefHist;
									if (pCurHist) delete [] pCurHist;
									return (M4_ERROR);
								}

								// Insertamos el registro proyección en el histórico corregido de registros.
								oHist.m_dDTStartCorr = pCurHist[indCurReg].m_dDTStartCorr;

								oHist.m_dDTStart = dCurE + m_pMtLgcTbl->m_dDateUnit;

/*								if (M4LDB_COMPARE_DATES (oHist.m_dDTStart, oHist.m_dDTEnd) > 0) {
									string stPKRegister(M4LDB_EMPTY);
									ComposePK (stPKRegister);
									DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_PROJECTION, stPKRegister.c_str ());
									return (M4_ERROR);
								}*/

								oHist.m_eOperation = M4LDB_INSERT;

								oHist.m_eRegType = M4LDB_PROJECTION_REG;

								m_pVcHistory->push_back (oHist);
							}
						}
					}

					// Actualizamos la fecha de fin de corrección del registro proyectado en el histórico corregido de registros.
					(*m_pVcHistory)[pBefHist[indBefReg].m_indReg].m_dDTEndCorr = pCurHist[indCurReg].m_dDTStartCorr - m_pMtLgcTbl->m_dDateUnit;

					if ((*m_pVcHistory)[pBefHist[indBefReg].m_indReg].m_eOperation == M4LDB_SELECT) {
						(*m_pVcHistory)[pBefHist[indBefReg].m_indReg].m_eOperation = M4LDB_UPDATE;
					}

					eRegType_t eRegType = (*m_pVcHistory)[pBefHist[indBefReg].m_indReg].m_eRegType;

					if (eRegType == M4LDB_NORMAL_REG) {
						(*m_pVcHistory)[pBefHist[indBefReg].m_indReg].m_eRegType = M4LDB_NORMAL_LOCK_REG;
					}

					if ((bLock) && 
						(eRegType == M4LDB_PROJECTION_REG)) {
						(*m_pVcHistory)[pBefHist[indBefReg].m_indReg].m_eRegType = M4LDB_PROJECTION_LOCK_REG;
					}

					bInvHist = M4_TRUE;
				}
			}

			m4date_t dNextDTStartCorr = 0;

			numReg = m_pVcHistory->size ();

			// Buscamos la fecha de inicio de corrección del siguiente histórico.
			for (indReg=0; indReg<numReg; indReg++) {

				struct stHistory &oReg = (*m_pVcHistory)[indReg];

				if ((oReg.m_eOperation != M4LDB_DELETE) && 
					((!dNextDTStartCorr) || 
					(M4LDB_COMPARE_DATES (dNextDTStartCorr, oReg.m_dDTStartCorr) > 0)) && 
					(M4LDB_COMPARE_DATES (oReg.m_dDTStartCorr, dCurDTStartCorr) > 0)) {
					dNextDTStartCorr = oReg.m_dDTStartCorr;
				}
			}

			dCurDTStartCorr = dNextDTStartCorr;

			if (pBefHist) delete [] pBefHist;
			if (pCurHist) delete [] pCurHist;
		}
/*
		if ((m_pMtLgcTbl->m_eCurComp != M4LDB_NO_COMPLETE) && 
			(m_eStmType == M4LDB_UPDATE) && 
			(!m_pParam->Difference (m_pMtLgcTbl->m_indDTStartCorr)) && 
			(!m_pParam->Difference (m_pMtLgcTbl->m_indDTEndCorr)) && 
			(!m_pParam->Difference (m_pMtLgcTbl->m_indDTStart)) && 
			(!m_pParam->Difference (m_pMtLgcTbl->m_indDTEnd))) {
			// Hemos validado todo lo que queriamos.
			m_pVcHistory->clear ();
			return (M4_SUCCESS);
		}
*/
		//Nos guardamos la posicion pq puede descuajeringarse al ir recorriendo registros.
		m4int32_t indLgcTblCurrent = m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Index;

		numReg = m_pVcHistory->size ();

		for (indReg=0; indReg<numReg; indReg++) {

			struct stHistory &oReg = (*m_pVcHistory)[indReg];

			switch (oReg.m_eOperation) {

			case M4LDB_DELETE: 
				result = DeleteProjection (oReg);
				if (result == M4_ERROR) {
					return (M4_ERROR);
				}
				break;

			case M4LDB_UPDATE:
				if ((oReg.m_eRegType != oReg.m_eOldRegType) || 
					(M4LDB_COMPARE_DATES (oReg.m_dDTEndCorr, oReg.m_dOldDTEndCorr))) {
					result = CloseCorrectedRegister (oReg.m_dDTStart, oReg.m_dDTStartCorr, oReg.m_dDTEndCorr, oReg.m_eRegType);
					if (result == M4_ERROR) {
						return (M4_ERROR);
					}
				}
				break;
			case M4LDB_INSERT:
				result = InsertProjection (indReg);
				if (result == M4_ERROR) {
					return (M4_ERROR);
				}
				break;
			}

			m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.MoveTo(indLgcTblCurrent);
		}
	}

	return (M4_SUCCESS);
}


m4bool_t ClPersistBase::CompareRegisters (m4int32_t ai_indReg1, m4int32_t ai_indReg2)
{
	m4int32_t indCol;
	m4pcchar_t pValue1;
	m4pcchar_t pValue2;
		
	m4bool_t bFound = M4_FALSE;

	// La columna empieza en 5.
	m4int32_t numCol = m_pCorrHistTbl->GetNumCol ();

	for (indCol=5; indCol<numCol; indCol++) {

		bFound = M4_TRUE;

		m4int8_t m4Type = m_pCorrHistTbl->GetM4Type (indCol);

		pValue1 = m_pCorrHistTbl->GetData (ai_indReg1, indCol);

		pValue2 = m_pCorrHistTbl->GetData (ai_indReg2, indCol);

		if (M4LDB_COMPARE_VALUES (pValue1, pValue2, m4Type)) {
			return (M4_FALSE);
		}
	}

	if (!bFound) {
		return (M4_FALSE);
	}

	return (M4_TRUE);
}


m4return_t ClPersistBase::ValidateCorrectedDates (void)
{
	m4date_t dDTStartCorr = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTStartCorr));

	m4date_t dDTEndCorr = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTEndCorr));

	if (m_eStmType == M4LDB_DELETE) {
		if (m_pMtLgcTbl->m_indRegType != M4LDB_NULL_INDEX) {
			eRegType_t eRegType = (eRegType_t) M4LDB_NUMBER (m_pParam->GetData (m_pMtLgcTbl->m_indRegType));

			//Caso normal: Si intentamos borrar un registro distinto de normal damos error.	
			//Excepcion: Si estamos borrando projeccion de forma interna (no damos error).
			if ((eRegType != M4LDB_NORMAL_REG) && (m_pGlobVars->GetStatementSubType() != M4LDB_DELETE_PROJECTION)) {
				string stnPKRegister(M4LDB_EMPTY);
				string stiPKRegister(M4LDB_EMPTY);
				ComposePK (stnPKRegister, stiPKRegister);
				m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
				m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;

				DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_DELETE_CORRECTION, LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << LOG_CAT_AND (iLgcTbl, nLgcTbl));
				return (M4_ERROR);
			}
		}
	}

	if ((m_pMtLgcTbl->m_indRegType != M4LDB_NULL_INDEX) && 
		(m_pMtSent)) {
		m4double_t dValue = M4LDB_NORMAL_REG;
		if (M4LDB_COMPARE_DATES (dDTEndCorr, M4LDB_PLUS_INF_DATE) < 0) {
			dValue = M4LDB_CLOSED_REG;
		}
		m_pParam->SetData (m_pMtLgcTbl->m_indRegType, (m4pcchar_t) &dValue);
	}

	if ((M4LDB_COMPARE_DATES (dDTStartCorr, M4LDB_MINUS_INF_DATE) < 0) || 
		(M4LDB_COMPARE_DATES (dDTStartCorr, M4LDB_PLUS_INF_DATE) > 0)) {
		string stnPKRegister(M4LDB_EMPTY);
		string stiPKRegister(M4LDB_EMPTY);
		ComposePK (stnPKRegister, stiPKRegister);
		m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
		m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
		m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTStartCorr);
		m4id_t iField = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTStartCorr);

		DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_DATE, LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << LOG_CAT_AND (iLgcTbl, nLgcTbl) << LOG_CAT_AND (iField, nField));
		return (M4_ERROR);
	}

	if ((M4LDB_COMPARE_DATES (dDTEndCorr, M4LDB_MINUS_INF_DATE) < 0) || 
		(M4LDB_COMPARE_DATES (dDTEndCorr, M4LDB_PLUS_INF_DATE) > 0)) {
		string stnPKRegister(M4LDB_EMPTY);
		string stiPKRegister(M4LDB_EMPTY);
		ComposePK (stnPKRegister, stiPKRegister);
		m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
		m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
		m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTEndCorr);
		m4id_t iField = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTEndCorr);

		DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_DATE, LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << LOG_CAT_AND (iLgcTbl, nLgcTbl) << LOG_CAT_AND (iField, nField));
		return (M4_ERROR);
	}

	if (M4LDB_COMPARE_DATES (dDTStartCorr, dDTEndCorr) > 0) {
		string stnPKRegister(M4LDB_EMPTY);
		string stiPKRegister(M4LDB_EMPTY);
		ComposePK (stnPKRegister, stiPKRegister);
		m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
		m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
		m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTEndCorr);
		m4id_t iField = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTEndCorr);
		m4id_t nField2 = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTStartCorr);
		m4id_t iField2 = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTStartCorr);

		DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_PERIOD_DATES, LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << LOG_CAT_AND (iLgcTbl, nLgcTbl) << LOG_CAT_AND (iField, nField) << LOG_CAT_AND (iField2, nField2));
		return (M4_ERROR);
	}

	m4date_t dDTStart = (m_pMtLgcTbl->m_indDTStart != M4LDB_NULL_INDEX) ? M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTStart)) : M4LDB_MINUS_INF_DATE;

	m4date_t dDTEnd = (m_pMtLgcTbl->m_indDTEnd != M4LDB_NULL_INDEX) ? M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTEnd)) : M4LDB_PLUS_INF_DATE;

	if ((M4LDB_COMPARE_DATES (dDTStartCorr, m_pMtLgcTbl->m_dClosingDate) <= 0) && 
		(M4LDB_COMPARE_DATES (dDTStart, m_pMtLgcTbl->m_dClosingDate) <= 0) && 
		(M4LDB_COMPARE_DATES (m_pMtLgcTbl->m_dClosingDate, M4LDB_MINUS_INF_DATE) > 0)) {
		// Existe un caso en el que esto no es error: Cuando lo que cambiamos es sólo la FFA o la FFC.
		m4bool_t bError = M4_TRUE;

		if (m_eStmType == M4LDB_UPDATE) {
			if (m_pParam->Difference (m_pMtLgcTbl->m_indDTEndCorr)) {
				bError = M4_FALSE;
			}

			if (m_pMtLgcTbl->m_indDTEnd != M4LDB_NULL_INDEX) {
				if (m_pParam->Difference (m_pMtLgcTbl->m_indDTEnd)) {
					bError = M4_FALSE;
				}
			}

			m4int32_t numParam = m_pParam->GetNumData ();

			for (m4int32_t indParam=0; indParam<numParam; indParam++) {
				if ((indParam != m_pMtLgcTbl->m_indDTEnd) && 
					(indParam != m_pMtLgcTbl->m_indDTEndCorr) && 
					(m_pParam->Difference (indParam)) && 
					(!m_pParam->GetInfoIsReadOnly (indParam))) {
					bError = M4_TRUE;
				}
			}
		}

		if (bError) {
			string stnPKRegister(M4LDB_EMPTY);
			string stiPKRegister(M4LDB_EMPTY);
			ComposePK (stnPKRegister, stiPKRegister);
			m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
			m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
			m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTStartCorr);
			m4id_t iField = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTStartCorr);

			if (m_pMtLgcTbl->m_indDTStart != M4LDB_NULL_INDEX) {
				m4id_t nField2 = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTStart);
				m4id_t iField2 = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTStart);

				DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_DATES_SMALLER_THAN_CLOSING_DATE, LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << LOG_CAT_AND (iLgcTbl, nLgcTbl) << LOG_CAT_AND (iField, nField) << LOG_CAT_AND (iField2, nField2));
			}
			else {
				DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_CORR_DATE_SMALLER_THAN_CLOSING_DATE, LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << LOG_CAT_AND (iLgcTbl, nLgcTbl) << LOG_CAT_AND (iField, nField));
			}
			return (M4_ERROR);
		}
	}

	if ((M4LDB_COMPARE_DATES (dDTEndCorr, m_pMtLgcTbl->m_dClosingDate) <= 0) && 
		(M4LDB_COMPARE_DATES (dDTStart, m_pMtLgcTbl->m_dClosingDate) <= 0)) {
		string stnPKRegister(M4LDB_EMPTY);
		string stiPKRegister(M4LDB_EMPTY);
		ComposePK (stnPKRegister, stiPKRegister);
		m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
		m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
		m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTEndCorr);
		m4id_t iField = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTEndCorr);

		if (m_pMtLgcTbl->m_indDTStart != M4LDB_NULL_INDEX) {
			m4id_t nField2 = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTStart);
			m4id_t iField2 = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTStart);

			DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_DATES_SMALLER_THAN_CLOSING_DATE, LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << LOG_CAT_AND (iLgcTbl, nLgcTbl) << LOG_CAT_AND (iField, nField) << LOG_CAT_AND (iField2, nField2));
		}
		else {
			DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_CORR_DATE_SMALLER_THAN_CLOSING_DATE, LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << LOG_CAT_AND (iLgcTbl, nLgcTbl) << LOG_CAT_AND (iField, nField));
		}
		return (M4_ERROR);
	}

	if ((M4LDB_COMPARE_DATES (dDTEnd, m_pMtLgcTbl->m_dClosingDate) <= 0) && 
		(M4LDB_COMPARE_DATES (dDTStartCorr, m_pMtLgcTbl->m_dClosingDate) <= 0)) {
		string stnPKRegister(M4LDB_EMPTY);
		string stiPKRegister(M4LDB_EMPTY);
		ComposePK (stnPKRegister, stiPKRegister);
		m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
		m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;
		m4id_t nField = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTStartCorr);
		m4id_t iField = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTStartCorr);

		if (m_pMtLgcTbl->m_indDTEnd != M4LDB_NULL_INDEX) {
			m4id_t nField2 = m_pGlobVars->GetpLConn ()->GetnField (this, m_pMtLgcTbl->m_indDTEnd);
			m4id_t iField2 = m_pParam->GetInfoIdField (m_pMtLgcTbl->m_indDTEnd);

			DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_DATES_SMALLER_THAN_CLOSING_DATE, LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << LOG_CAT_AND (iLgcTbl, nLgcTbl) << LOG_CAT_AND (iField, nField) << LOG_CAT_AND (iField2, nField2));
		}
		else {
			DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_CORR_DATE_SMALLER_THAN_CLOSING_DATE, LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << LOG_CAT_AND (iLgcTbl, nLgcTbl) << LOG_CAT_AND (iField, nField));
		}
		return (M4_ERROR);
	}

	// Ponemos el valor del parámetro auxiliar para el registro corrección anterior.
	m4date_t dBefDTEndCorr = dDTStartCorr - m_pMtLgcTbl->m_dDateUnit;

	m_pParam->SetData (m_pMtLgcTbl->m_indBefCorrReg, (m4pcchar_t) &dBefDTEndCorr);

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ValidateCorrection (void)
{
	if (!m_pMtSent) {
		return (M4_SUCCESS);
	}

	m4return_t result;

	m4int32_t numRow = m_pCorrHistTbl->GetNumRow ();

	if (!numRow) {
		return (M4_SUCCESS);
	}

	m4date_t dBefDTStartCorr = 0;

	m4date_t dCurDTStartCorr = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTStartCorr));

	m4date_t dCurDTEndCorr = M4LDB_DOUBLE (m_pParam->GetData (m_pMtLgcTbl->m_indDTEndCorr));

	m4int32_t indBefRow = M4LDB_NULL_INDEX;

	// Buscamos el registro corregido anterior al actual.
	for (m4int32_t indRow=0; indRow<numRow; indRow++) {

		m4date_t dAuxDTStartCorr = M4LDB_DOUBLE (m_pCorrHistTbl->GetData (indRow, 0));

		if (M4LDB_COMPARE_DATES (dCurDTStartCorr, dAuxDTStartCorr) > 0) {
			if ((!dBefDTStartCorr) ||
				(M4LDB_COMPARE_DATES (dBefDTStartCorr, dAuxDTStartCorr) > 0)) {

				if (!FindInTransaction (M4LDB_MINUS_INF_DATE, dAuxDTStartCorr)) {
					dBefDTStartCorr = dAuxDTStartCorr;

					indBefRow = indRow;
				}
				else {
					// El registro anterior ha sido modificado por el usuario.
					// Dejamos el cierre del registro para cuando lleguemos a él
				}
			}
		}
		else {
			if (M4LDB_COMPARE_DATES (dCurDTStartCorr, dAuxDTStartCorr) < 0) {
				if (!FindInTransaction (M4LDB_MINUS_INF_DATE, dAuxDTStartCorr)) {
					string stnPKRegister(M4LDB_EMPTY);
					string stiPKRegister(M4LDB_EMPTY);
					ComposePK (stnPKRegister, stiPKRegister);
					m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
					m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;

					DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_NO_LATEST_CORRECTION, LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << LOG_CAT_AND (iLgcTbl, nLgcTbl));
					return (M4_ERROR);
				}
				else {
					// El registro siguiente ha sido modificado por otro usuario.
					if (m_eStmType != M4LDB_DELETE) {
						// Cerramos el registro actual.
						m4date_t dAuxDTEndCorr = dAuxDTStartCorr - m_pMtLgcTbl->m_dDateUnit;

						result = CloseCorrectedRegister (M4LDB_MINUS_INF_DATE, dCurDTStartCorr, dAuxDTEndCorr, M4LDB_NORMAL_REG);

						if (result == M4_ERROR) {
							return (M4_ERROR);
						}

						m_pParam->SetData (m_pMtLgcTbl->m_indDTEndCorr, (m4pchar_t) &dAuxDTEndCorr);
					}
				}
				break;
			}
		}
	}

	if (indBefRow != M4LDB_NULL_INDEX) {
		result = CloseCorrectedRegister (M4LDB_MINUS_INF_DATE, dBefDTStartCorr, dCurDTEndCorr, M4LDB_NORMAL_REG);

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::CloseCorrectedRegister (m4date_t ai_dDTStart, m4date_t ai_dDTStartCorr, m4date_t ai_dDTEndCorr, eRegType_t ai_eRegType)
{
	m4return_t result;
	m4int32_t indFld;
	ClDinArr *pData = NULL;
	m4int32_t numFld;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n [ Internal Operation ]\n # Close Corrected Register.");
	#endif

	ClPersist *pStm = (ClPersist *) FindAuxCursor (M4LDB_CLOSE_CORR_REGISTER);

	if (!pStm) {

		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Close Corrected Register.");
		#endif

		pStm = new ClPersist (m_pGlobVars, M4_TRUE, M4_FALSE, M4_FALSE, M4_FALSE);

		result = pStm->Open (M4LDB_UPDATE, m_idRConn);
			
		if (result == M4_ERROR) {
			delete pStm;
			return (M4_ERROR);
		}

		pData = pStm->GetpParams ();

//		numFld = pData->GetNumData ();
		numFld = pStm->GetMaster ()->m_oVParam.size ();

		for (indFld=0; indFld<numFld; indFld++) {

			m4int32_t indParam = pStm->GetMaster ()->m_oVParam[indFld];

			eFldType_t eFldType = pData->GetInfoIdInternalField (indParam);

			if ((eFldType == M4LDB_DT_END_CORR) || 
				(eFldType == M4LDB_TYPE_REG)) {
				pData->SetItemHandle (indParam, M4LDB_INTERNAL_HANDLE);
			}
		}

		result = pStm->Prepare ();

		if (result == M4_ERROR) {
			delete pStm;
			return (M4_ERROR);
		}

		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Close Corrected Register.");
		#endif

		// Bindamos los parámetros.
		result = pStm->BindParam ();
		
		if (result == M4_ERROR) {
			delete pStm;
			return (M4_ERROR);
		}

		m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_CLOSE_CORR_REGISTER, pStm));
	}
	else {
		pData = pStm->GetpParams ();

//		numFld = pData->GetNumData ();
		numFld = pStm->GetMaster ()->m_oVParam.size ();
	}

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Close Corrected Register.");
	#endif

	pData->Clear ();

	for (indFld=0; indFld<numFld; indFld++) {

		m4int32_t indParam = pStm->GetMaster ()->m_oVParam[indFld];

		m4bool_t bIsPK = pData->GetInfoIsPK (indParam);

		eFldType_t eFldType = pData->GetInfoIdInternalField (indParam);

		if ((bIsPK) || 
			(eFldType == M4LDB_DT_END_CORR) || 
			(eFldType == M4LDB_TYPE_REG)) {

			switch (eFldType) {
				case M4LDB_TYPE_REG:
					{
						m4double_t dAux = (m4double_t) ai_eRegType;
						pData->SetData (indParam, (m4pchar_t) &dAux);
						break;
					}

				case M4LDB_DT_START:
					pData->SetData (indParam, (m4pchar_t) &ai_dDTStart);
					break;

				case M4LDB_DT_START_CORR:
					pData->SetData (indParam, (m4pchar_t) &ai_dDTStartCorr);
					break;

				case M4LDB_DT_END_CORR:
					pData->SetData (indParam, (m4pchar_t) &ai_dDTEndCorr);
					break;

				default:
					{
						m4int32_t indData = m_pParam->FindIndexByIdField (pData->GetInfoIdField (indParam));
						pData->SetData (indParam, m_pParam->GetData (indData), 0, m_pParam->GetInfoM4Type (indData));
					}
			}
		}
	}

	m4bool_t bOldPostValidate = m_pGlobVars->GetPostValidate ();

	m_pGlobVars->SetPostValidate (M4_TRUE);

	result = pStm->Execute ();

	m_pGlobVars->SetPostValidate (bOldPostValidate);

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	if (m_pMtSent) {
		m_pMtLgcTbl->m_bInvRecordSet = M4_TRUE;
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::DeleteProjection (const struct stHistory & ai_oReg)
{
	m4return_t result;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n [ Internal Operation ]\n # Delete Projection.");
	#endif

	ClPersist *pStm = (ClPersist *) FindAuxCursor (M4LDB_DELETE_PROJECTION);

	if (!pStm) {

		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Delete Projection.");
		#endif

		pStm = new ClPersist (m_pGlobVars, M4_TRUE, M4_FALSE, M4_FALSE, M4_FALSE);

		result = pStm->Open (M4LDB_DELETE, m_idRConn);
			
		if (result == M4_ERROR) {
			delete pStm;
			return (M4_ERROR);
		}

		result = pStm->Prepare ();

		if (result == M4_ERROR) {
			delete pStm;
			return (M4_ERROR);
		}

		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Delete Projection.");
		#endif

		// Bindamos los parámetros.
		result = pStm->BindParam ();
		
		if (result == M4_ERROR) {
			delete pStm;
			return (M4_ERROR);
		}

		m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_DELETE_PROJECTION, pStm));
	}

	ClDinArr *pData = pStm->GetpParams ();

//	m4int32_t numFld = pData->GetNumData ();
	m4int32_t numFld = pStm->GetMaster ()->m_oVParam.size ();


	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Delete Projection.");
	#endif

	pData->Clear ();

	for (m4int32_t indFld=0; indFld<numFld; indFld++) {

		m4int32_t indParam = pStm->GetMaster ()->m_oVParam[indFld];

		m4bool_t bIsPK = pData->GetInfoIsPK (indParam);

		eFldType_t eFldType = pData->GetInfoIdInternalField (indParam);

		//Problemas auditando tablas con proyecciones.
		//Los campos No pk no se están añadiendo a la tabla de auditoría.

		switch (eFldType) {
			case M4LDB_TYPE_REG:
				{
					m4double_t dValue;
					dValue = ai_oReg.m_eRegType;
					pData->SetData (indParam, (m4pcchar_t) &dValue);
					break;
				}

			case M4LDB_DT_START:
				pData->SetData (indParam, (m4pchar_t) &(ai_oReg.m_dDTStart));
				break;

			case M4LDB_DT_START_CORR:
				pData->SetData (indParam, (m4pchar_t) &(ai_oReg.m_dDTStartCorr));
				break;

			case M4LDB_DT_END:
				pData->SetData (indParam, (m4pcchar_t) &(ai_oReg.m_dDTEnd));
				break;

			default:
				m4int32_t indData = m_pParam->FindIndexByIdField (pData->GetInfoIdField (indParam));				
				if (indData != M4LDB_NULL_INDEX) {				
					pData->SetData (indParam, m_pParam->GetData (indData), 0, m_pParam->GetInfoM4Type (indData));
				}
				break;
		}			
/*
		if (bIsPK) {
			m4int32_t indData = m_pParam->FindIndexByIdField (pData->GetInfoIdField (indParam));

			if (indData == m_pMtLgcTbl->m_indDTStart) {
				pData->SetData (indParam, (m4pchar_t) &ai_dDTStart);
			}
			else {
				if (indData == m_pMtLgcTbl->m_indDTStartCorr) {
					pData->SetData (indParam, (m4pchar_t) &ai_dDTStartCorr);
				}
				else {
					pData->SetData (indParam, m_pParam->GetData (indData), 0, m_pParam->GetInfoM4Type (indData));
				}
			}
		}
*/
	}

	//pStm->SetValidate (M4_FALSE);
	m4bool_t bOldPostValidate = m_pGlobVars->GetPostValidate ();
	m4int32_t iOldStatementSubType = m_pGlobVars->GetStatementSubType();

	m_pGlobVars->SetPostValidate (M4_TRUE);
	m_pGlobVars->SetStatementSubType(M4LDB_DELETE_PROJECTION);
	
	result = pStm->Execute ();

	m_pGlobVars->SetPostValidate (bOldPostValidate);
	m_pGlobVars->SetStatementSubType(iOldStatementSubType);

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	if (m_pMtSent) {
		m_pMtLgcTbl->m_bInvRecordSet = M4_TRUE;
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::InsertProjection (m4int32_t ai_indReg)
{
	m4return_t result;
	m4int32_t indFld;
	m4double_t dValue;
	m4int32_t indFldCur;

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n [ Internal Operation ]\n # Insert Projection.");
	#endif


	if (m_pMtLgcTbl->m_eCurHist == M4LDB_N_PERIOD_WITHOUT_AUTO) {
		string stnPKRegister(M4LDB_EMPTY);
		string stiPKRegister(M4LDB_EMPTY);
		ComposePK (stnPKRegister, stiPKRegister);
		m4id_t nLgcTbl = m_pGlobVars->GetpLConn ()->GetnLgcTbl (m_pMtLgcTbl->m_indLgcTbl);
		m4id_t iLgcTbl = m_pMtLgcTbl->m_idLgcTbl;

		// Error de solapamiento.
		DUMP_LDBINSP_ERROR_AND_PATHF (m_pGlobVars->GetpLConn (), M4_CH_LDB_OVERLAPED_NO_ALLOWED, LOG_CAT_AND (stiPKRegister.c_str (), stnPKRegister.c_str ()) << LOG_CAT_AND (iLgcTbl, nLgcTbl));
		return (M4_ERROR);
	}

	ClPersist *pStm = (ClPersist *) FindAuxCursor (M4LDB_INSERT_PROJECTION);

	if (!pStm) {

		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Preparing Cursor Insert Projection.");
		#endif

		pStm = new ClPersist (m_pGlobVars, M4_TRUE, M4_FALSE, M4_FALSE, M4_FALSE);

		result = pStm->Open (M4LDB_DUPLICATE, m_idRConn);
			
		if (result == M4_ERROR) {
			delete pStm;
			return (M4_ERROR);
		}

		result = pStm->Prepare ();

		if (result == M4_ERROR) {
			delete pStm;
			return (M4_ERROR);
		}

		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Binding Parameters Cursor Insert Projection.");
		#endif

		// Bindamos los parámetros.
		result = pStm->BindParam ();
		
		if (result == M4_ERROR) {
			delete pStm;
			return (M4_ERROR);
		}

		m_pmpValCur->insert (MpPValCursor::value_type (M4LDB_INSERT_PROJECTION, pStm));
	}

	ClDinArr *pData = pStm->GetpParams ();

//	m4int32_t numFld = pData->GetNumData ();
	m4int32_t numFld = pStm->GetMaster ()->m_oVParam.size ();


	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Insert Projection.");
	#endif

	pData->Clear ();

	m4int32_t indRow = (*m_pVcHistory)[ai_indReg].m_indRow;

	for (indFld=0; indFld<numFld; indFld++) {

		m4int32_t indParam = pStm->GetMaster ()->m_oVParam[indFld];

		m4bool_t bIsPK = pData->GetInfoIsPK (indParam);

		eFldType_t eFldType = pData->GetInfoIdInternalField (indParam);

		//if ((bIsPK) || 
		//	(eFldType == M4LDB_DT_END) ||
		//	(eFldType == M4LDB_DT_END_CORR) || 
		//	(eFldType == M4LDB_TYPE_REG)) {

			switch (eFldType) {

				case M4LDB_TYPE_REG:
					dValue = (*m_pVcHistory)[ai_indReg].m_eRegType;
					pData->SetData (indParam, (m4pcchar_t) &dValue);
					break;

				case M4LDB_DT_START:
					pData->SetData (indParam, (m4pcchar_t) & (*m_pVcHistory)[ai_indReg].m_dDTStart);
					pData->SetOldData (indParam, m_pCorrHistTbl->GetData (indRow, 0));
					break;

				case M4LDB_DT_START_CORR:
					pData->SetData (indParam, (m4pcchar_t) & (*m_pVcHistory)[ai_indReg].m_dDTStartCorr);
					pData->SetOldData (indParam, m_pCorrHistTbl->GetData (indRow, 2));
					break;

				case M4LDB_DT_END:
					pData->SetData (indParam, (m4pcchar_t) &(*m_pVcHistory)[ai_indReg].m_dDTEnd);
					break;

				case M4LDB_DT_END_CORR:
					pData->SetData (indParam, (m4pcchar_t) &(*m_pVcHistory)[ai_indReg].m_dDTEndCorr);
					break;

				default:
					indFldCur = m_pParam->FindIndexByIdField (pData->GetInfoIdField (indParam));
					if (indFldCur != M4LDB_NULL_INDEX) {
						pData->SetData (indParam, m_pParam->GetData (indFldCur), 0, m_pParam->GetInfoM4Type (indFldCur));
					}
					break;
			}			
		//}
	}

	m4bool_t bOldPostValidate = m_pGlobVars->GetPostValidate ();

	m_pGlobVars->SetPostValidate (M4_TRUE);

	result = pStm->Execute ();

	m_pGlobVars->SetPostValidate (bOldPostValidate);

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	if (m_pMtSent) {
		m_pMtLgcTbl->m_bInvRecordSet = M4_TRUE;
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::GetHistories (void)
{
	m4return_t result;

	if (m_pMtLgcTbl->m_bCorrected) {
		
		// El GetCorrectedHistory debe de ir antes que el ValidateHistory.
		result = GetCorrectedHistory ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}
	}

	result = GetHistory ();

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ValidateTemporalityAndCorrection (void)
{
	m4return_t result;

	if (m_pMtLgcTbl->m_eCurTemp == M4LDB_N_PERIOD) {

		result = ValidateHistory ();

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		if (m_pMtLgcTbl->m_bCorrected) {
		
			result = CompleteHistoryCorrectionDate ();

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
		}
	}
	else {
		if (m_pMtLgcTbl->m_bCorrected) {
		
			result = ValidateCorrection ();

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
		}
	}

	return (M4_SUCCESS);
}


m4return_t  ClPersistBase::CheckLanguage (m4int32_t ai_indParam, ClRegister * ai_pReg, m4bool_t *ao_pbDiff)
{
	m4return_t ret = M4_SUCCESS;
	m4int32_t indCurrLang = m_pParam->GetCurrentLanguageIndex(ai_indParam);

	if (indCurrLang != M4LDB_NULL_INDEX)
	{
		// Estamos en el lenguaje por defecto, trabajando con otro lenguaje.
		// Comprobamos que no sea nulo el campo traducido del lenguaje actual
		// y que sea nulo el campo del lenguaje por defecto.
		if (m_pParam->IsNull (ai_indParam) && !m_pParam->IsNull (indCurrLang) )
		{
			if (ao_pbDiff) {
				*ao_pbDiff = M4_TRUE;
			}

			// Actualizamos el valor del campo con el lenguaje por defecto.
			m4pcchar_t pVal = m_pParam->GetData(indCurrLang);
			m4int8_t m4type = m_pParam->GetInfoM4Type(indCurrLang);

			m4int32_t len = 0;

			if( m4type != M4CL_M4_TYPE_BLOB || pVal == NULL )
			{
				len = m_pParam->GetLength(indCurrLang);
			}
			else
			{
				// En caso de blobs se calcula el tamaño con el fichero, la descripción y la extensión
				len += strlen( pVal ) + 1;
				len += strlen( pVal + len ) + 1;
				len += strlen( pVal + len ) + 1;
			}

			ret = m_pParam -> SetData (ai_indParam, pVal,len, m4type);
			// Por si no se hubiera hecho ya, en el campo traducido, lo hacemos ahora.
			// OJO !!!!. Aqui puede haber un problema en caso de que al validar dos veces seguidas, cambie el valor del item las dos veces.
			// Ahora mismo no puede ocurrir, pero se podría añadir alguna función que si lo hiciera.
			if (ret == M4_SUCCESS)
			{
				ret = ValidateValue(ai_indParam, ai_pReg);
			}
		}
	}

	return ret;

}


m4return_t ClPersistBase::InsertAuditory (void)
{
	m4return_t result = M4_SUCCESS;

	/* Bug 0291500
	Se permite desactivar la auditoría si el RSM es M4ADM
	*/
	if (m_pGlobVars->HasNotValidation (M4LDB_INV_AUDITORY) && strcmpi (m_pGlobVars->GetIdRSM (), M4CL_SEC_SUPER_USER) == 0)
	{
		#ifdef _M4LDB_DEBUG
			M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Auditory desactivated.");
		#endif
		return (M4_SUCCESS);
	}

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Auditory.");
	#endif

	ClPersist *pCursor = (ClPersist *) FindAuxCursor (M4LDB_CURSOR_AUDITORY);

	if (!pCursor) {
		// El cursor tiene que existir.
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::InsertAuditory" << "The cursor M4LDB_CURSOR_AUDITORY does not exist");
		return (M4_ERROR);
	}

	result = m_pGlobVars->GetpLConn ()->InitAuditory ();

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	if (m_eStmType == M4LDB_UPDATE) {
		
		m_pGlobVars->SetAuditory ('U', 'N');

		pCursor->GetpParams ()->SetLengthReadOnly ();

		m4bool_t bOldPostValidate = m_pGlobVars->GetPostValidate ();

		m_pGlobVars->SetPostValidate (M4_TRUE);

		result = pCursor->Execute ();

		m_pGlobVars->SetPostValidate (bOldPostValidate);

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		pCursor = (ClPersist *) FindAuxCursor (M4LDB_CURSOR_AUDITORY_2);

		if (!pCursor) {
			// El cursor tiene que existir.
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::InsertAuditory" << "The cursor M4LDB_CURSOR_AUDITORY_2 does not exist");
			return (M4_ERROR);
		}

		if (result == M4_ERROR) {
			return (M4_ERROR);
		}

		m_pGlobVars->SetAuditory ('U', 'O');

		pCursor->GetpParams ()->SetLengthReadOnly ();

		bOldPostValidate = m_pGlobVars->GetPostValidate ();

		m_pGlobVars->SetPostValidate (M4_TRUE);

		result = pCursor->Execute ();

		m_pGlobVars->SetPostValidate (bOldPostValidate);
	}
	else {
		m4char_t cStm = 'I';
		
		if (m_eStmType == M4LDB_DELETE) {
			cStm = 'D';
		}

		m_pGlobVars->SetAuditory (cStm, '\0');

		pCursor->GetpParams ()->SetLengthReadOnly ();

		m4bool_t bOldPostValidate = m_pGlobVars->GetPostValidate ();

		m_pGlobVars->SetPostValidate (M4_TRUE);

		result = pCursor->Execute ();

		m_pGlobVars->SetPostValidate (bOldPostValidate);
	}

	/* Bug 0268230
	Hay que reposicionar la tabla en la que se estaba.
	*/
	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[m_pMtLgcTbl->m_indLgcTbl];
	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[m_indRealTbl];

	return (result);
}


m4bool_t ClPersistBase::FindPersistInPost (ClTblMem *ai_pTblMem, m4int32_t ai_lRow, m4int32_t ai_indLgcTbl)
{
	for (itLsPStPostValidation it=m_pGlobVars->GetpLConn ()->m_plsPostValidation->begin (); it!=m_pGlobVars->GetpLConn ()->m_plsPostValidation->end (); it++) {
		if (((*it)->m_pStm->m_eStmType == M4LDB_INSERT) && 
			((*it)->m_indLgcTbl == ai_indLgcTbl) && 
			((*it)->m_oParam.ComparePKs (ai_pTblMem, ai_lRow))) {
  			#ifdef _M4LDB_DEBUG
				M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\t This FR is validated in operation [%ld].", (*it)->m_indOperation);
			#endif
			return (M4_TRUE);
		}
	}
	return (M4_FALSE);
}


m4return_t ClPersistBase::InsertTranslationAuditory( m4pcchar_t ai_pccOperation )
{

	m4return_t		iResult = M4_ERROR ;
	m4int8_t		iM4Type = 0 ;
	m4uint32_t		i = 0 ;
	m4uint32_t		iLength = 0 ;
	m4uint32_t		indParam = 0 ;
	m4uint32_t		iTranslationFields = 0 ;
	m4uint32_t		iArguments = 0 ;
	m4pcchar_t		pValue = NULL ;
	m4pcchar_t		pOldValue = NULL ;
	m4id_t			idTranslation = NULL ;
	ClAccess		*poAccess = NULL ;
	ClChannel		*poChannel = NULL ;
	ClNode			*poNode = NULL ;
	IStackInterface	*poStack = NULL ;
	m4VariantType	vVariant ;


	// Se cuentan los campos traducibles no nulos y se busca el campo sociedad
	iLength = m_oVParam.size() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		indParam = m_oVParam[ i ] ;
		idTranslation = m_pParam->GetInfoTranslation( indParam ) ;

		if( idTranslation != NULL )
		{
			if( m_pParam->IsNull( indParam ) == M4_FALSE )
			{
				pValue = m_pParam->GetDataPointer( indParam ) ;

				if( pValue != NULL && *pValue != '\0' )
				{
					pOldValue = m_pParam->GetOldDataPointer( indParam ) ;

					if( pOldValue == NULL || strcmp( pOldValue, pValue ) != 0 )
					{
						iTranslationFields++ ;
					}
				}
			}
		}
	}

	if( iTranslationFields == 0 )
	{
		// Si no hay no nulos no se inserta
		return( M4_SUCCESS ) ;
	}

	poAccess = m_pGlobVars->GetpAccessRecordSet()->GetpAccess() ;

	if( poAccess == NULL )
	{
		return( M4_ERROR ) ;
	}

	poChannel = poAccess->GetpChannel() ;

	if( poChannel == NULL )
	{
		return( M4_ERROR ) ;
	}

	iResult = m_pGlobVars->GetpLConn()->GetAuditTranslationNode( poChannel->GetpChannelManager(), poNode ) ;

	if( iResult != M4_SUCCESS || poNode == NULL )
	{
		return( M4_ERROR ) ;
	}

	poStack = &( poAccess->GetpExecutor()->Stack ) ;

	if( poStack == NULL )
	{
		return( M4_ERROR ) ;
	}

	// Tipo de operación
	vVariant.SetStringRef( (const m4pchar_t) ai_pccOperation ) ;
	iResult = poStack->Push( vVariant ) ;
	iArguments++ ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// Lenguaje
	vVariant.SetDouble( m_pGlobVars->GetLanguage() ) ;
	iResult = poStack->Push( vVariant ) ;
	iArguments++ ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// Organización
	poChannel->Organization.Get( vVariant ) ;

	for( i = 0 ; i < iLength; i++ )
	{
		indParam = m_oVParam[ i ] ;

		if( m_pParam->GetInfoIsPK( indParam ) == M4_TRUE && m_pParam->GetInfoIdInternalField( indParam ) == M4LDB_ID_ORGANIZATION )
		{
			if( m_pParam->IsNull( indParam ) == M4_FALSE )
			{
				pValue = m_pParam->GetDataPointer( indParam ) ;

				if( pValue != NULL && *pValue != '\0' && strcmp( pValue, "0000" ) != 0 )
				{
					vVariant.SetStringRef( (const m4pchar_t) pValue ) ;
				}
			}
			break ;
		}
	}

	iResult = poStack->Push( vVariant ) ;
	iArguments++ ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}
		
	// Tabla
	vVariant.SetStringRef( (const m4pchar_t) m_pMtLgcTbl->m_idLgcTbl ) ;
	iResult = poStack->Push( vVariant ) ;
	iArguments++ ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// Número de campos traducibles
	vVariant.SetDouble( iTranslationFields ) ;
	iResult = poStack->Push( vVariant ) ;
	iArguments++ ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	// Trios de campos traducibles
	for( i = 0 ; i < iLength ; i++ )
	{
		indParam = m_oVParam[ i ] ;
		idTranslation = m_pParam->GetInfoTranslation( indParam ) ;

		if( idTranslation != NULL )
		{
			if( m_pParam->IsNull( indParam ) == M4_FALSE )
			{
				pValue = m_pParam->GetDataPointer( indParam ) ;

				if( pValue != NULL && *pValue != '\0' )
				{
					pOldValue = m_pParam->GetOldDataPointer( indParam ) ;

					if( pOldValue == NULL || strcmp( pOldValue, pValue ) != 0 )
					{
						vVariant.SetStringRef( (const m4pchar_t) idTranslation ) ;
						iResult = poStack->Push( vVariant ) ;
						iArguments++ ;

						if( iResult != M4_SUCCESS )
						{
							return( M4_ERROR ) ;
						}

						vVariant.SetStringRef( (const m4pchar_t) pValue ) ;
						iResult = poStack->Push( vVariant ) ;
						iArguments++ ;

						if( iResult != M4_SUCCESS )
						{
							return( M4_ERROR ) ;
						}

						if( pOldValue == NULL )
						{
							vVariant.SetNull() ;
						}
						else
						{
							vVariant.SetStringRef( (const m4pchar_t) pOldValue ) ;
						}
						iResult = poStack->Push( vVariant ) ;
						iArguments++ ;

						if( iResult != M4_SUCCESS )
						{
							return( M4_ERROR ) ;
						}
					}
				}
			}
		}
	}

	// Parejas de pks
	for( i = 0 ; i < iLength; i++ )
	{
		indParam = m_oVParam[ i ] ;

		if( m_pParam->GetInfoIsPK( indParam ) == M4_TRUE && m_pParam->GetInfoIdInternalField( indParam ) != M4LDB_ID_ORGANIZATION )
		{
			vVariant.SetStringRef( (const m4pchar_t) m_pParam->GetInfoIdField( indParam ) ) ;
			iResult = poStack->Push( vVariant ) ;
			iArguments++ ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}

			vVariant.SetNull() ;

			if( m_pParam->IsNull( indParam ) == M4_FALSE )
			{
				iM4Type = m_pParam->GetInfoM4Type( indParam ) ;
				pValue = m_pParam->GetDataPointer( indParam ) ;

				switch( iM4Type )
				{
					case M4CL_M4_TYPE_FIXED_STRING:
					case M4CL_M4_TYPE_VAR_STRING:
					case M4CL_M4_TYPE_LONG:
					case M4CL_M4_TYPE_VARIANT:
					case M4CL_M4_TYPE_UNI_VAR_STRING:
					case M4CL_M4_TYPE_UNI_LONG:
					case M4CL_M4_TYPE_UNI_VARIANT:
					case M4CL_M4_TYPE_UNI_FIXED_STRING:

						vVariant.SetStringRef( (const m4pchar_t) pValue ) ;
						break;

					case M4CL_M4_TYPE_NUMBER:
					case M4CL_M4_TYPE_CURRENCY:
					case M4CL_M4_TYPE_NUM_VARIANT:
					case M4CL_M4_TYPE_TIME_SPAN:

						vVariant.SetDouble( M4LDB_DOUBLE( pValue ) ) ;
						break;

					case M4CL_M4_TYPE_DATE:
					case M4CL_M4_TYPE_TIMESTAMP:
					case M4CL_M4_TYPE_HOUR:

						vVariant.SetDouble( M4LDB_DOUBLE( pValue ) ) ;
						vVariant.Type = M4CL_CPP_TYPE_DATE ;
						break;

					case M4CL_M4_TYPE_BLOB:
					case M4CL_M4_TYPE_BINARY_STRING:
					default:
						break;
				}
			}

			iResult = poStack->Push( vVariant ) ;
			iArguments++ ;

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}

	// Ejecución del item
	iResult = poNode->RecordSet.Item[ "ADD" ].Call( NULL, iArguments ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = poStack->Pop( vVariant ) ;

    if( iResult != M4_SUCCESS || vVariant.Type != M4CL_CPP_TYPE_NUMBER || vVariant.Data.DoubleData != M4_SUCCESS )
    {
		return( M4_ERROR ) ;
    }

	return( M4_SUCCESS ) ;
}


m4return_t ClPersistBase::ExecuteOrdinalSelect (m4int32_t ai_indParam)
{
	m4return_t	result = M4_ERROR;

	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_SELECT_ORDINAL + ai_indParam);

	if (!pCursor) {
		result = PrepareOrdinalSelect (ai_indParam);

		if (result != M4_SUCCESS)
		{
			return (M4_ERROR);
		}

		pCursor = (ClLoad *) FindAuxCursor (M4LDB_SELECT_ORDINAL + ai_indParam);
	}

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Ordinal Select Lock Row [%ld].", ai_indParam);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Executing Cursor Ordinal Select Lock Row [%ld].", ai_indParam);
	#endif

	if (!pCursor) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::ExecuteOrdinalSelect" << "The cursor M4LDB_SELECT_ORDINAL " << LogCat << ai_indParam << LogCat << " does not exist");
		return (M4_ERROR);
	}

	result = pCursor->Execute ();

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	result = pCursor->Fetch ();
	result = pCursor->Close ();

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ExecuteOrdinalPKInsert (m4int32_t ai_indParam)
{
	m4return_t	result = M4_ERROR;

	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_INSERT_ORDINAL_PK + ai_indParam);

	if (!pCursor) {
		result = PrepareOrdinalPKInsert (ai_indParam);

		if (result != M4_SUCCESS)
		{
			return (M4_ERROR);
		}

		result = BindParamOrdinalPKInsert (ai_indParam);

		if (result != M4_SUCCESS)
		{
			return (M4_ERROR);
		}

		pCursor = (ClLoad *) FindAuxCursor (M4LDB_INSERT_ORDINAL_PK + ai_indParam);
	}

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Ordinal Primary Key Insert Lock Row [%ld].", ai_indParam);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Executing Cursor Ordinal Primary Key Insert Lock Row [%ld].", ai_indParam);
	#endif

	if (!pCursor) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::ExecuteOrdinalPKInsert" << "The cursor M4LDB_INSERT_ORDINAL_PK " << LogCat << ai_indParam << LogCat << " does not exist");
		return (M4_ERROR);
	}

	result = pCursor->Execute ();

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	result = pCursor->Close ();

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ExecuteOrdinalPKDelete (m4int32_t ai_indParam)
{
	m4return_t	result = M4_ERROR;

	ClLoad *pCursor = (ClLoad *) FindAuxCursor (M4LDB_DELETE_ORDINAL_PK + ai_indParam);

	if (!pCursor) {
		result = PrepareOrdinalPKDelete (ai_indParam);

		if (result != M4_SUCCESS)
		{
			return (M4_ERROR);
		}

		result = BindParamOrdinalPKDelete (ai_indParam);

		if (result != M4_SUCCESS)
		{
			return (M4_ERROR);
		}

		pCursor = (ClLoad *) FindAuxCursor (M4LDB_DELETE_ORDINAL_PK + ai_indParam);
	}

	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n + Executing Cursor Ordinal Primary Key Delete Lock Row [%ld].", ai_indParam);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn (), "\n + Executing Cursor Ordinal Primary Key Delete Lock Row [%ld].", ai_indParam);
	#endif

	if (!pCursor) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::ExecuteOrdinalPKDelete" << "The cursor M4LDB_DELETE_ORDINAL_PK " << LogCat << ai_indParam << LogCat << " does not exist");
		return (M4_ERROR);
	}

	result = pCursor->Execute ();

	if (result == M4_ERROR) {
		return (M4_ERROR);
	}

	result = pCursor->Close ();

	return (M4_SUCCESS);
}


m4return_t ClPersistBase::ExecuteOrdinalPK( m4int32_t ai_indParam, m4bool_t ai_bExcludeStartDates )
{

	m4return_t	iResult = M4_ERROR ;
	m4char_t	acPK[ M4_MAX_LEN_MD5 * 2 + 1 ] ;
	ClParam*	pParamPK = NULL;


	iResult = ComposeOrdinalPK( ai_indParam, ai_bExcludeStartDates, acPK, M4_MAX_LEN_MD5 * 2 ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	pParamPK = GetClParamByType( M4LDB_ODINAL_PK ) ;

	if( pParamPK == NULL )
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClPersistBase::ExecuteOrdinalPK" << "The parameter M4LDB_ODINAL_PK does not exist");
		return( M4_ERROR ) ;
	}

	iResult = pParamPK->SetBindParam( M4LDB_ODINAL_PK, acPK, M4LDB_M4_UNICODE_TYPE(M4CL_M4_TYPE_FIXED_STRING), M4_MAX_LEN_MD5 * 2, 0 ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = ExecuteOrdinalPKInsert( ai_indParam ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	iResult = ExecuteOrdinalPKDelete( ai_indParam ) ;

	if( iResult != M4_SUCCESS )
	{
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}


m4return_t ClPersistBase::ComposeOrdinalPK( m4int32_t ai_indParam, m4bool_t ai_bExcludeStartDates, m4pchar_t ao_pcPkMD5, m4int_t ai_PkMD5Length )
{

	m4return_t	iResult = M4_ERROR ;
	m4int8_t	iType = 0 ;
	m4uint32_t	i = 0 ;
	m4uint32_t	iParams = 0 ;
	m4int32_t	iLength = 0 ;
	m4int_t		iSize = 0 ;
	m4pcchar_t	pValue = NULL ;
	m4char_t	acValue[ M4LDB_SIZE_DEFAULT_STRING + 1 ] ;
	string		sPkString ;


	/* Bug 0277602
	Se recorre directamente el array de datos
	*/
	iParams = m_pParam->GetNumData() ;

	for( i = 0 ; i < iParams ; i++ )
	{
		if( i == ai_indParam )
		{
			continue ;
		}

		// Bug 0302129 Se excluyen las fechas si se especifica
		if( ai_bExcludeStartDates == M4_TRUE && ( i == m_pMtLgcTbl->m_indDTStart || i == m_pMtLgcTbl->m_indDTStartCorr ) )
		{
			continue ;
		}

		if( m_pParam->GetInfoIsPK( i ) == M4_TRUE )
		{
			sPkString.append( "<" ) ;

			iLength = m_pParam->GetLength( i ) ;

			if( iLength != M4LDB_SQL_NULL_DATA )
			{
				pValue = m_pParam->GetData( i ) ;

				if( pValue != NULL )
				{
					iType = m_pParam->GetInfoM4Type( i ) ;

					switch( iType )
					{
						default:

							sPkString.append( pValue ) ;
							break;

						case M4CL_M4_TYPE_DATE:
						case M4CL_M4_TYPE_TIMESTAMP:
						case M4CL_M4_TYPE_NUMBER:
						case M4CL_M4_TYPE_CURRENCY:
						case M4CL_M4_TYPE_NUM_VARIANT:
						case M4CL_M4_TYPE_HOUR:
						case M4CL_M4_TYPE_TIME_SPAN:

							sprintf( acValue, "%.20G", M4LDB_DOUBLE( pValue ) ) ;
							sPkString.append( acValue ) ;
							break;
					}
				}
			}

			sPkString.append( ">" ) ;
		}
	}

	iResult = EncryptOneWayMD5( sPkString.c_str(), sPkString.size(), ao_pcPkMD5, ai_PkMD5Length, iSize ) ;

	return( iResult ) ;
}

