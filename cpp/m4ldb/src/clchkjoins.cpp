
#include "m4stl.hpp"
#include "clchkjoins.hpp"
#include "defmain.hpp"
#include "clglbvar.hpp"
#include "ldb_log.hpp"
#include "clldb.hpp"


ClCheckJoins::ClCheckJoins	(PVcClMtObject	ai_pvcObject,PVcClMtAlias	ai_pvcAlias,PVcClMtRelat	ai_pvcRelat,PVcClMtField	ai_pvcField,ClGlobVars		*ai_pGlobVars, string *ai_pstWhere)
{
	m_pGlobVars	= ai_pGlobVars;
	m_pvcObject	= ai_pvcObject;
	m_pvcAlias	= ai_pvcAlias;
	m_pvcRelat	= ai_pvcRelat;
	m_pvcField	= ai_pvcField;
	m_pstWhere	= ai_pstWhere;
	m_bDoChange = M4_FALSE;
	m_bError = M4_FALSE;

}

m4return_t
ClCheckJoins::CheckConsistency(eDDBBType_t	eDDBB)
{
	// Hay que mirar:
	// -	Miramos los objetos logicos, fijandonos solo en los que tnegan Outer Join.
	//		Comprobamos que esten correctas los campos y relaciones utilizados.
	// -	Segun la base de datos en la que estemos, hay que actualizar o no los Outer Joins.
	//		En caso contrario actualizamos este objeto y los que se vean afectados por la actualizacion.
	m_bError = M4_FALSE;
	switch (eDDBB){
	case M4DBW_INFORMIX_DDBB:
	case M4DBW_SYBASE_DDBB:
		//Bug 0080499
		//En el caso de informix es posible que se pongan a inner algunos objetos, lo vamos a controlar en este
		//algoritmo es algo que se controla mas tarde en SQLServer y también para informix, pero es en la traduccion 
		//de la sentencia.
		//Lo que vamos a hacer es el control antes de modificar el objeto a inner.
		//Aplicamos solo a informix para evitar cualquier impacto que se pudiera derivar.
		//Intentaremos detectar solo el caso A=C(+) and B=C(+).		
		if (_CheckOneDominatedTwoDominator () == M4_ERROR) {
			DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_CIRCULAR_REFERENCE_IN_OUTER_JOINS);			
			return M4_ERROR;
		}

		m_bDoChange = M4_TRUE;
		break;
	case M4DBW_ORACLE_DDBB:
	case M4DBW_SQL_SERVER_DDBB:
	case M4DBW_DB2_DDBB:
	default:
		m_bDoChange = M4_FALSE;
		break;
	}
	return _CheckObjects();
}

m4return_t ClCheckJoins::_CheckOneDominatedTwoDominator (void)
{
	itVcClMtRelat	itRel = m_pvcRelat->begin();
	itVcClMtRelat	itRelend = m_pvcRelat->end();
	ClMetaRelat *pRel = 0;
	itVcClMtRelat	itRel2 = m_pvcRelat->begin();
	itVcClMtRelat	itRelend2 = m_pvcRelat->end();
	ClMetaRelat *pRel2 = 0;

	for (; itRel != itRelend; ++itRel) {
		pRel = (*itRel);

		if (pRel == NULL) {
			return M4_ERROR;
		}

		//-----------------------------------------------------------
		//Se supone que la relacion ya esta ordenada A=B(+). Es decir el alias con OJ esta a la derecha.
		//Si la relacion es OJ o el objeto derecho es OJ empezamos la recursion.
		if ((pRel->m_eJoinType == M4LDB_OUTER_JOIN) 
			|| (pRel->m_pMtField2->m_bIsOuter == M4_TRUE) )
		{
			if ((pRel == NULL) 
				|| (pRel->m_pMtField1 == NULL) 
				|| (pRel->m_pMtField2 == NULL)) {
				return M4_ERROR;
			}
			
			//Empiezo en la siguiente al bucle exterior.
			for (itRel2 = itRel + 1; itRel2 != itRelend2; ++itRel2) {
				pRel2 = (*itRel2);

				if ((pRel2 == NULL) 
					|| (pRel2->m_pMtField1 == NULL) 
					|| (pRel2->m_pMtField2 == NULL)) {
					return M4_ERROR;
				}

				if ((pRel2->m_eJoinType == M4LDB_OUTER_JOIN) 
					|| (pRel2->m_pMtField2->m_bIsOuter == M4_TRUE) )
				{
					
					//Si el alias izquierdo es diferente pero el alias derecho es el mismo... hemos encontrado el error.
					if ((pRel->m_pMtField1->m_indAlias != M4LDB_NULL_INDEX)
						&& (pRel->m_pMtField2->m_indAlias != M4LDB_NULL_INDEX)
						&& (pRel2->m_pMtField1->m_indAlias != M4LDB_NULL_INDEX)
						&& (pRel2->m_pMtField2->m_indAlias != M4LDB_NULL_INDEX)
						&& (pRel->m_pMtField1->m_indAlias != pRel2->m_pMtField1->m_indAlias) 
						&& (pRel->m_pMtField2->m_indAlias == pRel2->m_pMtField2->m_indAlias) ) {
						return M4_ERROR;
					}
				}
			}
		}
		//-----------------------------------------------------------
	}
	return M4_SUCCESS;
}

m4return_t
ClCheckJoins::_CheckObjects	()
{

	// Buscamos objetos con la marca de Outer Join.
	itVcClMtObject	itObj = m_pvcObject->begin();
	itVcClMtObject	itObjend = m_pvcObject->end();
	ClMetaObject *pObj = 0;
	m4bool_t bMustBeOuter = M4_FALSE;


	for (;itObj!=itObjend;++itObj){
		if (_CheckOneObject( (*itObj) )!=M4_SUCCESS){
			return M4_ERROR;
		}
	}
	return M4_SUCCESS;

}

m4return_t
ClCheckJoins::_CheckOneObject(ClMetaObject *ai_pObj)
{
	// Buscamos las relaciones y los fields que hacen referencia a este objeto.
	m4bool_t bMustBeOuter = M4_TRUE;

	if (ai_pObj->m_eJoinType!=M4LDB_OUTER_JOIN){
		bMustBeOuter = M4_FALSE;
	}


	m4int32_t indAlias = ai_pObj->m_indAlias;

	// Primero los Fields.
	itVcClMtField	itFld = m_pvcField->begin();
	itVcClMtField	itFldend = m_pvcField->end();
	ClMetaField *pFld = 0;
	for (;itFld!=itFldend;++itFld){
		pFld = (*itFld);
		if (pFld->m_indAlias==indAlias ){
			// Aunque sean de distinto nivel, si está apuntando al Alias debe ir en Outer Join
			// Si no esta en el Where, no me afecta al tipo de Outer join.
			if (pFld->m_eClause==M4LDB_CLAUSE_WHERE){
				if (pFld->m_bIsOuter!=bMustBeOuter){

					//Para multisociedad. En caso de que la tabla sea multi, hemos añadido un filtro para poder ver solo las sociedades de este rol.
					// El filtro es de la forma : (ID_ORG IN (SELECT ...) OR ID_ORG IS NULL.
					// En este caso no quitamos el Outer Join. !!!!!!!!!!!!
					// Los campos que hemos añadido llevanla marca de multisoc.
					if (pFld->m_bIsMultiSoc==M4_FALSE){

						// Tenemos que actualizar el objeto y todo lo que tenga que ver con él.
						// OJO!!!. En cualquier caso, pasamos a utilizar Inner Joins para ese objeto.
						if (m_bError==M4_FALSE){
							m_bError = M4_TRUE;
							DUMP_LDBINSP_DEBUGF(m_pGlobVars->GetpLConn(),M4_CH_LDB_OUTER_JOIN_TO_INNER_JOIN_I,ai_pObj->m_stObject.c_str()<<pFld->m_stField.c_str());
						}
						return (_SetObjectJoinToInnerJoin(ai_pObj));
					}
				}
			}
		}
	}

	// Ahora las relaciones.
	itVcClMtRelat	itRel = m_pvcRelat->begin();
	itVcClMtRelat	itRelend = m_pvcRelat->end();
	ClMetaRelat *pRel = 0;
	m4bool_t bSetInnerJoin = M4_FALSE;
	eJoinType_t eJTDesired = (bMustBeOuter?M4LDB_OUTER_JOIN:M4LDB_NORMAL_JOIN);
	for (;itRel!=itRelend;++itRel){
		
		pRel = (*itRel);
		
		// Para cada relacion comprobamos :
		//	- Si el objeto es Inner, no puede estar en una relacion de tipo Outer en el segundo elemento. (Dominado).
		//	- Si el objeto es Outer, debe tener todas sus relaciones en Outer.

		if (eJTDesired==M4LDB_OUTER_JOIN){
			// Si la relacion no es Outer y el objeto esta incluido en ella, hay que modificarlo.
			if (pRel->m_eJoinType!=M4LDB_OUTER_JOIN){
				if (pRel->m_pMtField1->m_indAlias==indAlias || pRel->m_pMtField2->m_indAlias==indAlias){
					bSetInnerJoin = M4_TRUE;
				}
			}
		}
		else{
			// Si la relacion no es Inner y el objeto es la parte dominada de la relación, hay que modificarlo.
			if (pRel->m_eJoinType!=M4LDB_NORMAL_JOIN){
				if (pRel->m_pMtField2->m_indAlias==indAlias){
					bSetInnerJoin = M4_TRUE;
				}
			}
		}

		if (bSetInnerJoin){
			// OJO!!!. En cualquier caso, pasamos a utilizar Inner Joins para ese objeto.
			if (m_bError==M4_FALSE){
				m_bError = M4_TRUE;
				DUMP_LDBINSP_DEBUGF(m_pGlobVars->GetpLConn(),M4_CH_LDB_OUTER_JOIN_TO_INNER_JOIN_II,ai_pObj->m_stObject.c_str());
			}
			return (_SetObjectJoinToInnerJoin(ai_pObj));
		}
	}

	return M4_SUCCESS;
}

m4return_t
ClCheckJoins::_SetObjectJoinToInnerJoin	(ClMetaObject *ai_pObj)
{
	// miramos a ver si hubiera incompatibilidad.
	_CheckForIncompatibilityWarning(ai_pObj);
	
	// Si no tenemos que hacer cambios, salimos. ya hemos soltado un warning antes de entrar en la funcion.
	if (m_bDoChange == M4_FALSE){
		return M4_SUCCESS;
	}

	ai_pObj->m_eJoinType = M4LDB_NORMAL_JOIN;

	m4int32_t indAlias = ai_pObj->m_indAlias;
	
	set <m4int32_t, less <m4int32_t> >	setMtObject;	// set para incluir los objetos que voy a modificar.
    // Actualizamos :
	//	-	Relaciones.
	//	-	Campos.
	// Primero los Campos.
	
	itVcClMtField	itFld = m_pvcField->begin();
	itVcClMtField	itFldend = m_pvcField->end();
	ClMetaField *pFld = 0;
	
	for (;itFld!=itFldend;++itFld){
		pFld = (*itFld);
		if (pFld->m_indAlias==indAlias ){
			pFld->m_bIsOuter=M4_FALSE;
		}
	}

	itVcClMtRelat	itRel = m_pvcRelat->begin();
	itVcClMtRelat	itRelend = m_pvcRelat->end();
	ClMetaRelat *pRel = 0;
	m4int32_t indObj = M4LDB_NULL_INDEX;

	for (;itRel!=itRelend;++itRel){
		pRel = (*itRel);
		// Para cada relacion comprobamos :
		//	-	Si está en m_pMtField1, lo dejamos. ( Es la tabla dominante del Join ).
		//	-	Si está en m_pMtField2, quitamos el Outer Join, y se modifica el objeto que era dominante. 

		if (pRel->m_pMtField2->m_indAlias==indAlias){
			if (pRel->m_eJoinType==M4LDB_OUTER_JOIN){

				pRel->m_eJoinType=M4LDB_NORMAL_JOIN;
				
				// Obtenemos el Objeto.
				indObj = (*m_pvcAlias)[pRel->m_pMtField1->m_indAlias]->m_indObj;
				
				if (indObj!=M4LDB_NULL_INDEX){
					// Lo guardamos para revisarlo más tarde.
					setMtObject.insert(indObj);	// Si falla la insercion es porque ya estaba. No hay problema.
				}
				else{
					DUMP_LDBINSP_ERRORF(m_pGlobVars->GetpLConn(),M4_CH_LDB_INTERNAL_ERROR,"ClCheckJoins::_SetObjectJoinToInnerJoin"<<"ObjectIndex=-1");
					return M4_ERROR;
				}
			}
		}
	}

	// Repasamos los objetos que tengamos en el set, a ver si se han producido mas incoherencias al modificar este objeto.

	set <m4int32_t, less <m4int32_t> >::iterator itsetMtObject;
	ClMetaObject *pObj = 0;

	while (!setMtObject.empty()){
		
		itsetMtObject = setMtObject.begin();	
		pObj = (*m_pvcObject)[ (*itsetMtObject) ];

		if (m_bError==M4_FALSE){
			m_bError = M4_TRUE;
			DUMP_LDBINSP_DEBUGF(m_pGlobVars->GetpLConn(),M4_CH_LDB_OUTER_JOIN_TO_INNER_JOIN_III,pObj->m_stObject.c_str()<<ai_pObj->m_stObject.c_str());
		}
		
		if (_CheckOneObject(pObj)!=M4_SUCCESS){
			return M4_ERROR;
		}
		setMtObject.erase(itsetMtObject);
	}
	
	return M4_SUCCESS;
}
// El parametro no lo utilizo, es por si en un fuituro me hiciera falta, para no modificar el hpp.
void
ClCheckJoins::_CheckForIncompatibilityWarning(ClMetaObject *ai_pObj)
{
	// Se ha encontrado una posible incompatibilidad.
	// Tenemos Outer join entre tablas y una condicion (sin Outer Join) sobre la tabla dominada.

	// Las situaciones de incompatibilidad que concocemos son :
	//	- A=B(+) AND (A=? OR B=?)
	//	- A=B(+) AND (B=? OR B IS NULL). Puede haber incompatibilidades en general con el IS NULL.

	// Condiciones para sacar el Warning :
	//	- Que haya OR (Actualmente no podemos detectar si es entre una tabla que este en Outer join o no).
	//	- Que haya IS NULL (Lo correcto seria comprobar que el IS NULL afecta a un campo de la tabla dominada, pero tampoco podemos hacerlo).

	// Sacamos el Warning solo en caso de que se den las dos cosas.
	//OR
	if (m_pstWhere){
		if (m_pstWhere->find(" OR ")!=M4_STR_NPOS){
			// Tiene OR.
			if (m_pstWhere->find(" IS NULL")!=M4_STR_NPOS){
				// Tiene IS NULL.
				//if (g_OuterJoinsWarning){
					DUMP_LDBINSP_WARNING(m_pGlobVars->GetpLConn(),M4_CH_LDB_OUTER_JOIN_INCOMPATIBILITY);					

				//}
			}

		}
	}


}

