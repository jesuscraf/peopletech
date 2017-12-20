#include "sqlojtrans.hpp"
#include "clmtsent.hpp"
#include "sqltrans.hpp"
#include "ldb_log.hpp"
#include "access.hpp"

extern m4uint8_t			M4LDB_MD_LENGTH_CLAUSE			;
extern m4uint8_t			M4LDB_FIELD_LENGTH				;
extern m4pcchar_t			M4LDB_POSIBLE_CLAUSES			;

//Pedro Bug 0069267
//La función no está en ningún .h, se encuentra en clmtsent.cpp
//Porque dentro de este fichero se utiliza.
extern string GetStringM4LDB_MD_LENGTH_CLAUSE (m4int32_t ai_Level);
//fin Pedro

_AClSQLTranslator::_AClSQLTranslator(ClMetaSentence *ai_pMtSent,ClGlobVars *ai_pGlobVars,ClTranslatorUtils * ai_pTrUtils, m4bool_t ai_bIsSecurityView, string* ai_psTranslatedSQL)
{
	m_pMtSent = ai_pMtSent;
	m_pGlobVars = ai_pGlobVars;
	// Referencias a objetos del ClMetaSentence
	m_pstStatemCol = ai_pMtSent->m_pstStatemCol;
	m_pvcColumns=ai_pMtSent->m_pvcColumns;
	m_pvcParamCol=ai_pMtSent->m_pvcParamCol;

	m_pstStatemWhr=ai_pMtSent->m_pstStatemWhr;
	m_pvcParamWhr=ai_pMtSent->m_pvcParamWhr;
	m_pvcRel=ai_pMtSent->m_pvcRel;

	m_pvcFld=ai_pMtSent->m_pvcFld;

	m_pvcFrmObj=ai_pMtSent->m_pvcFrmObj;
	m_pvcFrmAls=ai_pMtSent->m_pvcFrmAls;

	m_pmpFieldAndValue=ai_pMtSent->m_pmpFieldAndValue;
	
	m_CurrentFld = 0;

	m_bUsingAlias = M4_FALSE;
	m_bOuterJoins = M4_FALSE;
	m_bIsSecurityView = ai_bIsSecurityView;

	m_pTrUtils = ai_pTrUtils;
	m_psTranslatedSQL = ai_psTranslatedSQL;
}

_AClSQLTranslator::~_AClSQLTranslator(void)
{
}

m4return_t _AClSQLTranslator::TranslateSentence( string &ai_stTrans,m4int32_t ai_indColIni,m4int32_t ai_indColEnd )
{
	ClMetaObject *pTableBase = NULL; 

	// Por si acaso.
	m_CurrentFld = 0;
	m_bUsingAlias = _CheckToUseAlias();
	m_bOuterJoins = _CheckForOuterJoins();

	if (m_pMtSent->ClearObjectsBasis()!=M4_SUCCESS){
		return M4_ERROR;
	}

	// Primero las columnas.
	switch (m_pMtSent->GetStmType()) {

		case M4LDB_SELECT:
			if (ai_indColIni!=-1){
				if (_TranslateSomeColumns(m_pstStatemCol,ai_stTrans,ai_indColIni,ai_indColEnd)!=M4_SUCCESS){
					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (),M4_CH_LDB_INTERNAL_ERROR,"_TranslateSomeColumns"<<"");			
					return M4_ERROR;
				}
			}
			else{
				// Traducimos todas.
				if (_Translate(m_pstStatemCol,ai_stTrans)!=M4_SUCCESS){
					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (),M4_CH_LDB_INTERNAL_ERROR,"_TranslateColumns"<<"");			
					return M4_ERROR;
				}
			}
			break;

		case M4LDB_UPDATE:
		case M4LDB_DELETE:
			ai_stTrans.append ("SELECT ");

			// Como estamos en el Delete o en el Update, el objeto base es siempre el primero del vector.
			pTableBase = (*m_pvcFrmObj)[0];

			if (m_pMtSent != NULL && m_pMtSent->GetIsUpdateSelect() == M4_TRUE)
			{
				if (_CheckTableBaseInSubselect (pTableBase) != M4_SUCCESS)
				{
					return M4_ERROR;
				}
			}

			if (_TranslatePKs(ai_stTrans, pTableBase) != M4_SUCCESS){
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (),M4_CH_LDB_INTERNAL_ERROR,"_TranslatePKs"<<"");			
				return M4_ERROR;
			}

			if (m_pMtSent != NULL && m_pMtSent->GetIsUpdateSelect() == M4_TRUE)
			{
				//Se han añadido las PKs ahora el resto de campos de la subselect
				ai_stTrans.append( "," );
				return _TranslateInsertSubSelect(ai_stTrans);
			}
			break;

		case M4LDB_INSERT:

			ai_stTrans.append ("SELECT ");
			return _TranslateInsertSubSelect(ai_stTrans);

			break;
	}


	// Luego los filtros ( y los objetos logicos ).
	if (_Translate(m_pstStatemWhr,ai_stTrans)!=M4_SUCCESS){
		//Comentamos el mensaje de error, lo habremos dado o daremos otro por arriba.
		//DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (),M4_CH_LDB_INTERNAL_ERROR,"_TranslateWhere"<<"");			
		return M4_ERROR;
	}

	string stOrderBy;

	if (_ArrangeOrderBy(ai_stTrans, stOrderBy)!=M4_SUCCESS){
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (),M4_CH_LDB_INTERNAL_ERROR,"_ArrangeOrderBy"<<"");			
		return M4_ERROR;
	}

	if (_TranslateRanges(ai_stTrans, stOrderBy)!=M4_SUCCESS){
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (),M4_CH_LDB_INTERNAL_ERROR,"_TranslateRanges"<<"");			
		return M4_ERROR;
	}

	// Añadimos el For Update
	if (m_pMtSent->GetStmType()==M4LDB_SELECT && m_pMtSent->GetForUpdate()){
		m_pTrUtils->m_pLockTrans->AddLockForUpdate(ai_stTrans);
	}
	return M4_SUCCESS;
}

//Comprueba que:
// - Exista tabla base en la subselect
// - la tabla base en una subselect sea la misma que la de la sentencia pricipal.
m4return_t _AClSQLTranslator::_CheckTableBaseInSubselect(ClMetaObject* ai_pTableBase)
{ 
	ClMetaObject* pAuxTable = NULL;
	ClMetaObject* pBaseTableInSS = NULL;

	itVcClMtObject itObj = m_pvcFrmObj->begin();
	itVcClMtObject itObjend = m_pvcFrmObj->end();

	//Ahora iteramos buscando otro objeto base
	while (itObj!=itObjend){

		pAuxTable = (*itObj) ;
		if (pAuxTable->m_bIsBasis == M4_TRUE 
			&& ai_pTableBase != pAuxTable
			&& pAuxTable->m_iLevel != ai_pTableBase->m_iLevel )
		{
			pBaseTableInSS = pAuxTable ;
			break;
		}

		itObj++;
	}

	//Si no hay: ERROR
	if (pBaseTableInSS == NULL)
	{
		//No hay tabla base en subselect
		DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_SUBSELECT_NEEDS_TABLE_BASE );
		return M4_ERROR;
	}

	//Si son diferentes: ERROR
	if (strcmpi(pBaseTableInSS->m_stObject.c_str(), ai_pTableBase->m_stObject.c_str()) != 0)
	{
		//1246500="La tabla base de la sentencia principal "%0:s" no coincide con la tabla de la subselect "%1:s"."
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_BASE_DIFFERS_SUBSELECT, ai_pTableBase->m_stObject.c_str() << pBaseTableInSS->m_stObject.c_str() );
		return M4_ERROR;
	}

	return (M4_SUCCESS);
}

m4return_t _AClSQLTranslator::_Translate( string *ai_stSource,string &ai_stTrans )
{
	if (!ai_stSource) {
		return (M4_SUCCESS);
	}

	m4int32_t Pos = 0,LastPos=M4_STR_NPOS;
	eClauseType_t eClause=M4LDB_CLAUSE_END_STATEM;
	m4int32_t iLevel=0;

	//Sustituimos todos los hints.
	if (TranslateHintsByReplacement( *ai_stSource ) == M4_ERROR) {
		return M4_ERROR;
	}

	LastPos = Pos;
	eClauseType_t LastClause  = eClause;
	m4int32_t LastLevel = 0;
	m4bool_t bExit = M4_FALSE;
	while (bExit == M4_FALSE){
		// Sacamos lo siguiente a procesar
		Pos = _GetNextField(*ai_stSource,LastPos,eClause,iLevel);

		if (Pos!=M4_STR_NPOS){			
			//Veamos la subcadena.
			m4bool_t bArranged = M4_FALSE;
			string sSourceSubString("");
			sSourceSubString.append( *ai_stSource, LastPos, Pos - LastPos );
			//Si lo que vamos a meter es un hint tenemos que arreglar inicios y fin.
			if (ArrangeSysHintsBeginEnd( ai_stTrans, sSourceSubString ) == M4_ERROR) {
				return M4_ERROR;
			}

			//Añado a la cadena resultado.
			ai_stTrans.append( sSourceSubString );
		
			if (eClause!=LastClause || iLevel!=LastLevel){
				// Cambio de Clausula.
				m4int32_t Incr = 0;// La clausula puede ocupar un tamaño variable.
				if (_TranslateClausule(ai_stTrans,eClause,iLevel,Incr)!=M4_SUCCESS){
					return M4_ERROR;
				}
				Pos+=Incr;
			}
			else{
				if (_TranslateField(ai_stTrans,eClause)!=M4_SUCCESS){
					return M4_ERROR;
				}
				Pos+=M4LDB_FIELD_LENGTH; //Saltamos el ~F
			}
			LastLevel = iLevel;
			LastClause=eClause;
			LastPos = Pos;
		}
		else{
			ai_stTrans.append(*ai_stSource,LastPos,ai_stSource->size()-LastPos);
			bExit = M4_TRUE;
		}
	}

	return M4_SUCCESS;
}

m4return_t _AClSQLTranslator::_TranslateField(string &ai_stTrans ,eClauseType_t ai_eClause){

	// 
	//
	ClMetaField *pField=0;
	m4int32_t size = m_pvcFld->size();

	m4bool_t bGo = M4_TRUE;
	// Nos colocamos en el siguiente campo. ( Nos saltamos los separadores )
	while (bGo){
		if (m_CurrentFld>=size){
			return M4_ERROR;
		}
		pField = (*m_pvcFld)[m_CurrentFld++];
		if (pField->IsMetaData()==M4_TRUE){
			// En el caso de columnas separadas la clausula puede ser distinta.
//			M4_ASSERT (pField->m_eClause == ai_eClause);
			bGo = M4_FALSE;
		}
	}

	// Vamos a buscar el nombre del Real Field.
	if (_TranslateField(ai_stTrans,pField)!=M4_SUCCESS){
		return M4_ERROR;
	}
 
	return M4_SUCCESS;
}

m4return_t	_AClSQLTranslator::TranslateAutomaticSysHints( eClauseType_t ai_eClause, m4int32_t ai_iLevel, string & aio_sTrans )
{
	//Cada gestor se especializará.
	return M4_SUCCESS;
}

m4return_t _AClSQLTranslator::TranslateHintBySysHintId( m4int32_t ai_iFilter, const string & ai_sSysHintId, string & aio_sTrans )
{
	//Cada gestor se especializará.
	return M4_SUCCESS;
}

m4return_t _AClSQLTranslator::TranslateSysHintsBegin( string & aio_sTrans )
{
	//Cada gestor se especializará.
	return M4_SUCCESS;
}

m4return_t _AClSQLTranslator::TranslateSysHintsEnd( string & aio_sTrans )
{
	//Cada gestor se especializará.
	return M4_SUCCESS;
}

m4return_t _AClSQLTranslator::ArrangeSysHintsBeginEnd( string & aio_sTrans, string & aio_sRealHint )
{
	//Cada gestor se especializará.
	return M4_SUCCESS;
}

m4return_t	_AClSQLTranslator::MergeSysHints( const string & ai_sSourceString, string & aio_sSysHintTranslated, m4int32_t & aio_iPosBegin)
{
	//Cada gestor se especializará.
	return M4_SUCCESS;
}

m4return_t _AClSQLTranslator::TranslateHintsByReplacement( string & aio_sSourceString )
{
	m4bool_t bIsInsideLiteral = M4_FALSE;
	m4int32_t iPosBeginLiteral = 0;
	m4int32_t iPosEndLiteral = aio_sSourceString.size() - 1;
	//Se analiza la cadena de entrada, sustituyendo hints.
	m4int32_t iPosBegin = aio_sSourceString.find( M4LDB_SYSHINT_APISQL_WORD, 0 );
	while (iPosBegin != -1)
	{
		//Ojito pq hay que controlar que no me hayan puesto de literal.
		if (ClLConn::_IsInsideLiteral( m_pGlobVars->GetpLConn (), iPosBegin, aio_sSourceString, bIsInsideLiteral, iPosBeginLiteral, iPosEndLiteral ) == M4_ERROR) {
			return M4_ERROR;
		}

		//Reposicionamos a la cadena completa.
		iPosBeginLiteral = 0;
		iPosEndLiteral = aio_sSourceString.size() - 1;

		if (bIsInsideLiteral == M4_TRUE) {
			//Si sí pertenece a un literal, necesitamos buscar el siguiente hint.
			iPosBegin = aio_sSourceString.find( M4LDB_SYSHINT_APISQL_WORD, iPosBegin + 1 );
			continue;
		}

		string sSysHintId("");
		m4int32_t iNumChrParsed = 0;

		m4pcchar_t pBeginHint = &(aio_sSourceString[iPosBegin]);
		m4int32_t iPosEnd  = iPosBegin;
		if (ClLConn::_GetSysHintIdParsed( m_pGlobVars->GetpLConn (), pBeginHint + strlen(M4LDB_SYSHINT_APISQL_WORD), sSysHintId, iNumChrParsed) == M4_ERROR) {
			return M4_ERROR;
		}

		//Ya tenemos el id del hint. Avanzamos el fin del hint.
		iPosEnd = iPosEnd + strlen(M4LDB_SYSHINT_APISQL_WORD) + iNumChrParsed;

		//Ahora traducimos el hint.
		string sSysHintTranslated("");

		if (TranslateSysHintsBegin(sSysHintTranslated) == M4_ERROR) {
			return M4_ERROR;
		}

		// Se pone el nivel de la tabla principal
		if (TranslateHintBySysHintId(m_pMtSent->GetMainFilter(), sSysHintId, sSysHintTranslated) == M4_ERROR) {
			return M4_ERROR;
		}

		if (TranslateSysHintsEnd(sSysHintTranslated) == M4_ERROR) {
			return M4_ERROR;
		}

		//Quitamos la posible coma que pudiera haber despues del hint.
		m4int32_t iPosComma = aio_sSourceString.find_first_not_of(" ", iPosEnd);
		if ((iPosComma != -1) && (aio_sSourceString[iPosComma] == ',')) {
			aio_sSourceString.replace(iPosComma, 1, "");
		}

		//Puede ser que tengamos que fusionar hints.		
		if (MergeSysHints(aio_sSourceString, sSysHintTranslated, iPosBegin) == M4_ERROR) {
			return M4_ERROR;
		}
		
		//Sustituimos con el hint traducido.
		aio_sSourceString.replace(iPosBegin, iPosEnd - iPosBegin, sSysHintTranslated);

		//Comprimimos espacios innecesarios.
		if (ClLConn::_ComprimeixBlancs(aio_sSourceString, iPosBegin + sSysHintTranslated.size()) == M4_ERROR) {
			return M4_ERROR;
		}

		//Buscamos el siguiente hint.
		iPosBegin = aio_sSourceString.find( M4LDB_SYSHINT_APISQL_WORD, iPosBegin );
	}	
	return M4_SUCCESS;
}

m4return_t _AClSQLTranslator::_TranslateClausule(string &ai_stTrans,eClauseType_t ai_eClause,m4int32_t ai_iLevel,m4int32_t &ai_Incr)
{
	//Bug 0069267.
	string sFillString;

	switch (ai_eClause){
	case M4LDB_CLAUSE_BEGIN_STATEM:
		if (_TranslateBeginStatement(ai_stTrans,ai_iLevel)!=M4_SUCCESS){
			return M4_ERROR;
		}
		ai_Incr = M4LDB_MD_LENGTH_CLAUSE;		// B0x

		//Se meten, si hay que meter, los hints automáticos.
		if (TranslateAutomaticSysHints( ai_eClause, ai_iLevel, ai_stTrans ) == M4_ERROR) {
			return M4_ERROR;
		}

		break;
	
	case M4LDB_CLAUSE_COLUMN:
		// Puede ser |C00 o |C00,
		//Pedro
		//Bug 0069267.	
		sFillString.append("|C");
		sFillString.append(GetStringM4LDB_MD_LENGTH_CLAUSE(0));
		sFillString.append(",");
		if (m_pstStatemCol->find(sFillString)!=M4_STR_NPOS){
			ai_Incr = M4LDB_MD_LENGTH_CLAUSE+1 ;	 // |C00,
		}
		else{
			ai_Incr = M4LDB_MD_LENGTH_CLAUSE;	 // |C00
		}
		break;
	
	case M4LDB_CLAUSE_LONG_COLUMN:
		sFillString.append("|C");
		sFillString.append(GetStringM4LDB_MD_LENGTH_CLAUSE(0));
		sFillString.append(",");
		if (m_pstStatemCol->find(sFillString)!=M4_STR_NPOS){		
			ai_Incr = M4LDB_MD_LENGTH_CLAUSE;	 // |L00 Dejamos la coma.
		}
		else{
			//Sustituimos caracter de posición 1, 1 caracter, por L.
			//Es decir, la C por L.
			sFillString.replace(1,1,"L");
			if (m_pstStatemCol->find(sFillString)!=M4_STR_NPOS){		
				ai_Incr = M4LDB_MD_LENGTH_CLAUSE+1;	 // |L00, Quitamos la coma.
			}
			else{
				ai_Incr = M4LDB_MD_LENGTH_CLAUSE;	 // |L00|F00 No quitamos nada
			}
		}

		break;
	
	case M4LDB_CLAUSE_FROM:
		if (!m_pMtSent->IsReal()) {
			if (_TranslateFrom(ai_stTrans,ai_iLevel)!=M4_SUCCESS){
				return M4_ERROR;
			}
		}
		ai_Incr = M4LDB_MD_LENGTH_CLAUSE;	 // |F00
		break;
	
	case M4LDB_CLAUSE_WHERE:
		if (!m_pMtSent->IsReal()) {
			// Puede ser que venga |W00|R00 o |W00 AND ... 
			if (_TranslateWhere(ai_stTrans,ai_iLevel,ai_Incr)!=M4_SUCCESS){
				return M4_ERROR;
			}
		}
		else{
			ai_Incr = M4LDB_MD_LENGTH_CLAUSE;
		}

		break;

	case M4LDB_CLAUSE_REST:
		ai_Incr = M4LDB_MD_LENGTH_CLAUSE;	 // |R00
		break;
	
	case M4LDB_CLAUSE_ORDER_BY:
		if (_TranslateOrderBy(ai_stTrans,ai_iLevel,ai_Incr)!=M4_SUCCESS){
			return M4_ERROR;
		}
		break;

	case M4LDB_CLAUSE_END_STATEM:
		ai_Incr = M4LDB_MD_LENGTH_CLAUSE;	//E00
		break;
	default:

		return M4_ERROR;
	}
	return M4_SUCCESS;
}


m4return_t _AClSQLTranslator::_TranslateFrom(string &ai_stTrans,m4int32_t ai_iLevel){

	// Metemos todos los objetos de este nivel.
	ai_stTrans.append(" FROM ");
	itVcClMtObject itObj = m_pvcFrmObj->begin();
	itVcClMtObject itObjend = m_pvcFrmObj->end();
	// Nos posicionamos en el primero
	while (itObj!=itObjend){
		if ((*itObj)->m_iLevel==ai_iLevel){
			break;
		}
		++itObj;
	}
	if (itObj==itObjend){
		// No existían objetos para este nivel.
		// Si no es real es error.
		if (m_pMtSent->IsReal()){
			return M4_SUCCESS;
		}
		else{
			return M4_ERROR;
		}
	}
	// Estamos posicionados en el primer objeto.


	ClMetaObject *pObj=0;
	m4int32_t indObj=M4_STR_NPOS;
	m4bool_t bAddedAnyTable = M4_FALSE;
	m4bool_t bForceAlias = M4_FALSE;


	// Se calcula el tipo de canal para la herencia de datos y la tramitación
	ClItemIndex		iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_REAL_OBJECT ;

	m4double_t	dOrgType ;
	m_pGlobVars->GetpAccessRecordSet()->GetpAccess()->GetpChannel()->OrganizationType.Get( dOrgType ) ;
	m4uint32_t iOrgType = (m4uint32_t) dOrgType ;

	m4pchar_t pcTramitation = NULL;
	m_pGlobVars->GetpAccessRecordSet()->GetpAccess()->GetpChannel()->Tramitation.Get( pcTramitation ) ;

	while (itObj!=itObjend&&(*itObj)->m_iLevel==ai_iLevel)
	{
		pObj= (*itObj);

		bForceAlias = M4_FALSE ;
		iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_REAL_OBJECT ;

		if( pObj->m_iMirrorType == 1 )
		{
			if( pcTramitation != NULL )
			{
				// En tablas con tramitación hay que ver si se debe utilizar la vista
				bForceAlias = M4_TRUE ;
				iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_TRAMIT_VIEW ;
			}
		}

		if( pObj->m_bInheritsData == M4_TRUE )
		{
			if( iOrgType != M4CL_ORGANIZATION_TYPE_NONE )
			{
				// En tablas con herencia hay que ver si se debe usar la vista
				bForceAlias = M4_TRUE ;
				iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_INHERIT_VIEW ;
			}
		}
		
	 	indObj=m_pMtSent->GetObjectPosByAliasPos(pObj->m_indAlias);
		// Nos posicionamos en el diccionario.
		m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.MoveTo(pObj->m_indLogicTable);
		// Vamos a las tablas reales.
		m4int32_t nReal = m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Count();
		// Iteramos para ver cuales tenemos que añadir.
		for (m4int32_t indRealTable =0;indRealTable<nReal;++indRealTable)
		{
			if ( pObj->IsUsingRealTable (indRealTable) )
			{
				bAddedAnyTable = M4_TRUE;
				// De momento, no se comprueba.
				// El problema está en que puede venir de ExecuteDirect y en ese caso no se debería comprobar.
				// Si realemnte tenemos objetos de BBDD distintos, dejaos que de error el gestor.
/*
				m4int32_t idRConn = M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[indRealTable].Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_CONN].Value.GetWithOutSec ());
				if (m_pMtSent->GetIdRConn() != idRConn) {
					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_JOIN, m_pMtSent->GetIdObjectBasis () << pObj->m_stObject.c_str());
					return (M4_ERROR);
				}
*/

				m4pcchar_t pTable = NULL;

				if( indRealTable == 0 && pObj->m_sSecurityView.size() > 0 )
				{
					pTable = pObj->m_sSecurityView.c_str();
					_AddSecurityParams( pObj->m_pvcSecurityParams, NULL );
					bForceAlias = M4_TRUE;
				}
				else
				{
					pTable = M4LDB_PCSTR( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[indRealTable].Item[iTableName].Value.GetWithOutSec() );
				}

				ai_stTrans.append (pTable);
				ai_stTrans.append (" ");

				if( indRealTable == 0 && pObj->m_sSecurityView.size() > 0 )
				{
					pObj->CleanSecurity();
				}

				// bg 147470
				if( !pObj->m_sFTTParams.empty() )
				{
					ai_stTrans.append (pObj->m_sFTTParams.c_str()) ;
					ai_stTrans.append (" ");
				}

				m4bool_t bAdded = M4_FALSE;
				if (_TranslateAlias(ai_stTrans,(*m_pvcFrmAls)[pObj->m_indAlias],indRealTable,bAdded)!=M4_SUCCESS){
					return M4_ERROR;
				}

				// En herencia en SQLserver se necesita un alias para la vista dinámica
				if( bAdded == M4_FALSE && bForceAlias == M4_TRUE )
				{
					ai_stTrans.append ("T");
				}

				if (m_pMtSent->GetForUpdate()){
					m_pTrUtils->m_pLockTrans->AddLockBetweenJoins(ai_stTrans);
				}
				ai_stTrans.append (",");
			}
		}
		++itObj;
	}

	// Si no hemos añadido ninguna :
	if (bAddedAnyTable==M4_FALSE){
		// Añadimos una a capon ( el ultimo de este nivel). Se que hay alguno porque si no hubieramos salido ya.
		pObj = (*(itObj-1));

		bForceAlias = M4_FALSE ;
		iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_REAL_OBJECT ;

		if( pObj->m_iMirrorType == 1 )
		{
			if( pcTramitation != NULL )
			{
				// En tablas con tramitación hay que ver si se debe utilizar la vista
				bForceAlias = M4_TRUE ;
				iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_TRAMIT_VIEW ;
			}
		}

		// En tablas con herencia hay que ver si se debe usar la vista
		if( pObj->m_bInheritsData == M4_TRUE )
		{
			if( iOrgType != M4CL_ORGANIZATION_TYPE_NONE )
			{
				bForceAlias = M4_TRUE ;
				iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_INHERIT_VIEW ;
			}
		}

		m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.MoveTo(pObj->m_indLogicTable);


		m4pcchar_t pTable = NULL;

		if( pObj->m_sSecurityView.size() > 0 )
		{
			pTable = pObj->m_sSecurityView.c_str();
			_AddSecurityParams( pObj->m_pvcSecurityParams, NULL );
			bForceAlias = M4_TRUE;
		}
		else
		{
			pTable = M4LDB_PCSTR( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[0].Item[iTableName].Value.GetWithOutSec() );
		}

		ai_stTrans.append (pTable);

		if( pObj->m_sSecurityView.size() > 0 )
		{
			pObj->CleanSecurity();
		}

		// bg 147470
		if( !pObj->m_sFTTParams.empty() )
		{
			ai_stTrans.append (pObj->m_sFTTParams.c_str()) ;
			ai_stTrans.append (" ");
		}

		// En herencia en SQLserver se necesita un alias para la vista dinámica
		if( bForceAlias == M4_TRUE )
		{
			ai_stTrans.append ("T");
		}
	}
	else{
		// Quitamos la coma del final.
		ai_stTrans.erase(ai_stTrans.size()-1);
	}

	return M4_SUCCESS;
}

m4return_t _AClSQLTranslator::_TranslateField(string &ai_stTrans,ClMetaField *ai_pField)
{
	if (ai_pField==0){
		return M4_ERROR;
	}

	m4bool_t bNVLAdded =M4_FALSE;

	if (ai_pField->m_indRealTable2!=-1){
		if (ai_pField->m_eClause!=M4LDB_CLAUSE_LONG_COLUMN){
			m_pTrUtils->m_pNVLTrans->AddNVL(ai_stTrans);
			ai_stTrans.append("(");
			bNVLAdded = M4_TRUE;
		}
	}

	// Nos colocamos en el diccionario.
	m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.MoveTo(ai_pField->m_indLogicTable);
	m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.MoveTo(ai_pField->m_indRealTable);
	m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Current.MoveTo(ai_pField->m_indRealField);

	m4pcchar_t pRealField = (m4pcchar_t)m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Current.Item[M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD].Value.GetWithOutSec();

	// Hay que añadir el Alias.
	// Cogemos el indLgcTbl del Alias.
	// No es el indice del objeto logico en el diccionario, es el indice del objeto logico en el vector de objetos logicos.
	m4int32_t indAls=m_pMtSent->GetNewAliasPosByAliasPos(ai_pField->m_indAlias);
	m4bool_t bAdded = M4_FALSE;

	if (_TranslateAlias(ai_stTrans,(*m_pvcFrmAls)[indAls],ai_pField->m_indRealTable,bAdded)!=M4_SUCCESS){
		return M4_ERROR;
	}
	if (bAdded){
		ai_stTrans.append(".");
	}
	ai_stTrans.append(pRealField);

	if (ai_pField->m_bIsOuter){
		m_pTrUtils->m_pOJTrans->AddOuterJoinInCondition(ai_stTrans);
	}

	if (ai_pField->m_indRealTable2!=M4LDB_NULL_INDEX){
		m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.MoveTo(ai_pField->m_indRealTable2);
		m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Current.MoveTo(ai_pField->m_indRealField2);
		pRealField  = (m4pcchar_t)m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Current.Item[M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD].Value.GetWithOutSec ();
		// Hay que añadir el Alias.
		ai_stTrans.append(",");
		if (_TranslateAlias(ai_stTrans,(*m_pvcFrmAls)[indAls],ai_pField->m_indRealTable2,bAdded)!=M4_SUCCESS){
			return M4_ERROR;
		}
		if (bAdded){
			ai_stTrans.append(".");
		}
		ai_stTrans.append(pRealField);
	}
	
	if (bNVLAdded){
		ai_stTrans.append(")");
	}
	return M4_SUCCESS;
}

m4return_t _AClSQLTranslator::_TranslateWhere(string &ai_stTrans,m4int32_t ai_iLevel,m4int32_t &ai_Incr)
{
	// La clausula Where puede venir de dos formas :
	// |W00 AND ...
	// |W00|R00...
	
	// En cualqiera de los dos casos, vamos a insertar las relaciones aqui.
	// Hay que insertar tambien relaciones entre tablas reales.

	// Buscamos la clausula en cuestion :
	m4uint32_t Pos = 0;
	if (m_pMtSent->_FindClause(*m_pstStatemWhr,M4LDB_CLAUSE_WHERE,ai_iLevel,Pos)!=M4_TRUE){
		// No habia clausula
		return M4_ERROR;
	}
	m4bool_t bAddedWhere=M4_FALSE;	// Indica si ya hemos añadido el Where.
	m4bool_t bMoreWhere=M4_FALSE;	// Indica si hay mas Where aparte de las relaciones.

	// Las relaciones se van a añadir como "RRRR AND ".
	// En caso de que no haya mas W00 que estas relaciones, hay que quitar el AND.

	// Miramos a ver que Where hay.
	if ( (*m_pstStatemWhr)[Pos+M4LDB_MD_LENGTH_CLAUSE]!='|'){
		// No hay clausula Where.
		ai_Incr = M4LDB_MD_LENGTH_CLAUSE+4;	// "|W00 AND "
		ai_stTrans.append(" WHERE ");
		bAddedWhere = M4_TRUE;
		bMoreWhere = M4_TRUE;

		//Comentamos pq es incorrecta la busqueda!!:
		/*
		//Bug 0078961. Este bug engloba varios cambios.
		//Vamos a comprobar si se encuentra la cadena " OR("
		//Y recorreremos los alias del nivel actual y veremos si el objeto asociado está como OJ
		//OJO con el cableo de la cadenita de marras.
		//Buscamos a partir de Pos + 2. El motivo es que Pos es el comienzo de "|W..."
		//por lo tanto vamos a buscar despues de |W.
		m4int32_t iOrPos = 0;
		m4int32_t iNextBeginWherePos = 0;
		iOrPos = m_pstStatemWhr->find (" OR(", Pos + 2);

		//Si se encuentra un OR
		if (iOrPos != M4_STR_NPOS) {
			//Solo tendremos en cuenta el OR encontrado si está en el nivel actual.

			//Buscamos, apartir de Pos el comienzo del siguiente nivel, si lo hubiese.
			iNextBeginWherePos = m_pstStatemWhr->find ("|W", Pos + 2);

			//Si el OR encontrado esta en el nivel siguiente o mas profundo no se comprueba nada de OJ.
			//Se comprobará en el nivel donde realmente se encuentre el OR.
			//Por lo tanto, si no hay más niveles o el OR se encuentra antes del siguiente nivel comprobaremos los
			//alias
			if ( (iNextBeginWherePos == M4_STR_NPOS) || (iOrPos < iNextBeginWherePos) ) {
				//Si existe un objeto en el nivel que tiene OJ entonces tendremos la incompatilidad.
				if (_CheckWithOJInLevel (ai_iLevel) == M4_TRUE) {
					DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_OUTER_JOIN_NO_ALLOWED_II);
					return M4_ERROR;
				}
			}

		}
		*/
	}
	else{
		ai_Incr = M4LDB_MD_LENGTH_CLAUSE;
	}


	// Insertamos los joins entre tablas reales.
	if (_TranslateRealJoins(ai_stTrans,ai_iLevel,bAddedWhere)!=M4_SUCCESS){
		return M4_ERROR;
	}

	// Insertamos las relaciones
	if (_TranslateRelations(ai_stTrans,ai_iLevel,bAddedWhere)!=M4_SUCCESS){
		return M4_ERROR;
	}

	// Insertamos las relaciones entre tablas de distintos niveles ( solo para SQLServer y DB2, en las demas esta funcion no hace nada ).
	if (_TranslateSpecialRelations(ai_stTrans,ai_iLevel,bAddedWhere)!=M4_SUCCESS){
		return M4_ERROR;
	}

	if (bMoreWhere==M4_FALSE){
		// Tenemos que quitar el AND en caso de que hayamos añadido algo.
		if (bAddedWhere){
			// Quitamos el AND.
			m4int32_t size = ai_stTrans.size();
			ai_stTrans.erase(size-5); 
		}
	}


	return M4_SUCCESS;
}

m4return_t _AClSQLTranslator::_TranslateRealJoins(string &ai_stTrans,m4int32_t ai_iLevel,m4bool_t &bAddedWhere)
{
	// Nos recorremos todos los objetos, mirando a ver de cuales hay que meter relaciones.
	// Los objetos estan ordenados por nivel.
	itVcClMtObject itObj = m_pvcFrmObj->begin();
	itVcClMtObject itObjend = m_pvcFrmObj->end();
	// Nos posicionamos en el primero
	while (itObj!=itObjend){
		if ((*itObj)->m_iLevel==ai_iLevel){
			break;
		}
		++itObj;
	}
	if (itObj==itObjend){
		// No existían objetos para este nivel.
		// Si no es real es error.
		if (m_pMtSent->IsReal()){
			return M4_SUCCESS;
		}
		else{
			return M4_ERROR;
		}
	}
	// OJO. Si fuera un INSERT hay que mirarlo.
/*	if ((!indStmObj) && 
		(m_pMtSent->GetApiSql () == M4LDB_APISQL_INSERT)) {
		continue;
	}
*/

	m4bool_t bBase = M4_FALSE;
	ClMetaObject * pObj = 0;
	while (itObj!=itObjend&&(*itObj)->m_iLevel==ai_iLevel){
		pObj = (*itObj);
		// Comprobamos que no añadimos la tabla base dos veces.
		if (ai_iLevel==0){
			if (pObj->m_bIsBasis){
				if (bBase){
					++itObj; // Ya hemos añadido la tabla base.
					continue;
				}
				else{
					bBase=M4_TRUE;
				}
			}
		}
		if (_TranslateRealJoins(ai_stTrans,pObj,bAddedWhere)!=M4_SUCCESS){
			return M4_ERROR;
		}
		++itObj;
	}

	return M4_SUCCESS;
}

m4return_t _AClSQLTranslator::_TranslateRealJoins(string &ai_stTrans,ClMetaObject *ai_pObj,m4bool_t &ai_bAddedWhere)
{
	eJoinType_t JoinType = ai_pObj->m_eJoinType;

	// Buscamos el indice del registro para el idObject dado.
	m4int32_t indLgcTbl = ai_pObj->m_indLogicTable;

	if (indLgcTbl == M4LDB_NULL_INDEX) {
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_LOGIC_OBJECT, ai_pObj->m_stObject.c_str());
		return (M4_ERROR);
	}
	m4bool_t bIsBasis = ai_pObj->m_bIsBasis;

	// Situamos el current del nodo SYS_DICT_LOGIC_OBJECTS en dicho registro.
	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[indLgcTbl];

	// Obtenemos el número de registros (tablas físicas) que tiene el nodo SYS_DICT_REAL_OBJECTS.
	m4int32_t nReal = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Count ();

	if (nReal<=1){
		return M4_SUCCESS;
	}
	m4char_t Buffer[M4LDB_SIZE_DEFAULT_STRING];
	// Hay mas de un tabla real.
	// Guardo en un vector los alias de las tablas reales para las que hay que meter Join.
	vector < string > vRealTablesUsed;
	vRealTablesUsed.reserve(nReal);

	// Vamos a por el indice del objeto en el vector al alias.
	m4int32_t indObj = m_pMtSent->GetObjectPosByAliasPos(ai_pObj->m_indAlias);
	m4pcchar_t pccTranslationAlias = NULL;

	for( m4int32_t i = 0; i < nReal; ++i )
	{
		if( ai_pObj->IsUsingRealTable(i) )
		{
			if (bIsBasis)
			{
				if( m_bIsSecurityView == M4_FALSE )
				{
					sprintf (Buffer, "BASE_%X", (m4int32_t) i);
				}
				else
				{
					sprintf (Buffer, "B%X", (m4int32_t) i);
				}
			}
			else
			{
				if( m_bIsSecurityView == M4_FALSE )
				{
					sprintf (Buffer, "ALIAS_%X_%X", indObj, i);
				}
				else
				{
					sprintf (Buffer, "A%X_%X", indObj, i);
				}
			}
			vRealTablesUsed.push_back ( Buffer );

			if( i > 0 )
			{
				if( M4LDB_NUMBER( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Register[i].Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_IS_TRANSLATION].Value.GetWithOutSec() ) )
				{
					pccTranslationAlias = vRealTablesUsed.back().c_str();
				}
			}
		}
	}
	
	if (vRealTablesUsed.size()<=1){
		// Solamente usamos una tabla real.
		return M4_SUCCESS;
	}


	// El el caso de herencia y tramitación hay que añadir el campo ID_ORG_LEVEL o ID_TRAMITATION
	m4bool_t bInheritance = M4_FALSE ;
	m4bool_t bTramitation = M4_FALSE ;

	if( ai_pObj->m_iMirrorType == 1 )
	{
		m4pchar_t pcTramitation = NULL;
		m_pGlobVars->GetpAccessRecordSet()->GetpAccess()->GetpChannel()->Tramitation.Get( pcTramitation ) ;

		if( pcTramitation != NULL )
		{
			bTramitation = M4_TRUE ;
		}
	}

	if( ai_pObj->m_bInheritsData == M4_TRUE )
	{
		m4double_t dOrgType ;
		m_pGlobVars->GetpAccessRecordSet()->GetpAccess()->GetpChannel()->OrganizationType.Get( dOrgType ) ;
		m4uint32_t iOrgType = (m4uint32_t) dOrgType ;

		if( iOrgType != M4CL_ORGANIZATION_TYPE_NONE )
		{
			bInheritance = M4_TRUE ;
		}
	}


	m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current [0];
	
	// Iteramos por todos los field insertando Joins para los que sean PK.

	m4bool_t bLanguageFilterAdded = M4_FALSE;

	m4int32_t numFld = m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Count();

	for( m4int32_t indFld = 0; indFld < numFld; indFld++ )
	{
		m4int32_t lIsPK = M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Current[indFld].Item[M4LDB_ITEM_SYS_DICT_FIELDS_POS_PK].Value.GetWithOutSec ());

		if( lIsPK == 0 && ( bInheritance == M4_TRUE || bTramitation == M4_TRUE ) )
		{
			eFldType_t eIntFld = (eFldType_t) M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Current[indFld].Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_INTERNAL_FIELD].Value.GetWithOutSec ());

			if( eIntFld == M4CL_INTERNAL_TYPE_ID_ORGANIZATION_LEVEL && bInheritance == M4_TRUE )
			{
				lIsPK = 1 ;
			}
			else if( eIntFld == M4CL_INTERNAL_TYPE_ID_DATASET && bTramitation == M4_TRUE )
			{
				lIsPK = 1 ;
			}
		}

		if( lIsPK )
		{
			// Obtenemos el campo real.
			m4pcchar_t pRealField = M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Current[indFld].Item[M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD].Value.GetWithOutSec ());

			vector < string >::iterator itRTend = vRealTablesUsed.end();
			vector < string >::iterator itRTBefore = vRealTablesUsed.begin();

			vector < string >::iterator itRT = itRTBefore;

			if (itRT!=itRTend )
			{
				++itRT;
			}
			// Añadimos el Where si hace falta.
			if (ai_bAddedWhere==M4_FALSE)
			{
				ai_stTrans.append(" WHERE ");
				ai_bAddedWhere = M4_TRUE;
			}

			// Sabemos que hay mas de un elemento.
			// Añadimos n-1 joins para las n tablas reales.
			m4uint32_t iIsTranslation = 0;

			for( ; itRT != itRTend; ++itRT, ++itRTBefore)
			{
				if( pccTranslationAlias != NULL && strcmp( pccTranslationAlias, itRT->c_str() ) == 0 )
				{
					iIsTranslation = 1;

					if( bLanguageFilterAdded == M4_FALSE )
					{
						sprintf( Buffer, "%s.ID_LANGUAGE#", pccTranslationAlias );
						ai_stTrans.append( Buffer );

						m_pTrUtils->m_pOJTrans->AddOuterJoinInCondition( ai_stTrans );

						sprintf( Buffer, "=%d AND ", m_pGlobVars->GetLanguage() );
						ai_stTrans.append( Buffer );

						bLanguageFilterAdded = M4_TRUE;
					}
				}

				// ALIAS.FIELD
				sprintf(Buffer, "%s.%s", itRTBefore->c_str(), pRealField);
				ai_stTrans.append(Buffer);
				
				// En Sybase *=, con el asterisco en la tabla dominante.
				if (JoinType == M4LDB_OUTER_JOIN || iIsTranslation == 1 )
				{
					m_pTrUtils->m_pOJTrans->AddOuterJoinInEqual(ai_stTrans);
				}

				// ALIAS.FIELD
				sprintf(Buffer, "=%s.%s", itRT->c_str(), pRealField);
				ai_stTrans.append(Buffer);

				// En Oracle (+), en la tabla dominada.
				if (JoinType == M4LDB_OUTER_JOIN || iIsTranslation == 1 )
				{
					m_pTrUtils->m_pOJTrans->AddOuterJoinAfter(ai_stTrans);
				}

				ai_stTrans.append(" AND ");
			}
		}
	}
	return M4_SUCCESS;
}

m4return_t _AClSQLTranslator::_TranslateRelations(string &ai_stTrans,m4int32_t ai_iLevel,m4bool_t &ai_bAddedWhere)
{
	// Insertamos las relaciones que tengamos en m_pvcRel.

	if (m_pvcRel==0){
		return M4_SUCCESS;
	}
	if (m_pvcRel->empty()){
		return M4_SUCCESS;
	}

	itVcClMtRelat	itRel = m_pvcRel->begin();
	itVcClMtRelat	itRelend = m_pvcRel->end();
	ClMetaRelat *pRel = 0;
	for (;itRel!=itRelend;++itRel){
		pRel=(*itRel);
		if (pRel->m_iLevel == ai_iLevel){
			if (ai_bAddedWhere==M4_FALSE){
				ai_stTrans.append(" WHERE ");
				ai_bAddedWhere=M4_TRUE;
			}

			if (_TranslateField(ai_stTrans,pRel->m_pMtField1)!=M4_SUCCESS){
				return M4_ERROR;
			}

			if (pRel->m_eJoinType==M4LDB_OUTER_JOIN){
				m_pTrUtils->m_pOJTrans->AddOuterJoinInEqual(ai_stTrans);
			}
			ai_stTrans.append("=");

			if (_TranslateField(ai_stTrans,pRel->m_pMtField2)!=M4_SUCCESS){
				return M4_ERROR;
			}
			
			//Miramos a ver si tenemos que añadir Outer Join.
			if (pRel->m_eJoinType==M4LDB_OUTER_JOIN){
				m_pTrUtils->m_pOJTrans->AddOuterJoinAfter(ai_stTrans);
			}
			ai_stTrans.append(" AND ");
		}
	}

	return M4_SUCCESS;
}

m4int32_t _AClSQLTranslator::_GetNextField(string &ai_stBuf,m4int32_t ai_IniPos,eClauseType_t &ai_eClause,m4int32_t &ai_iLevel)
{
	eClauseType_t eClause;
	m4int32_t Level;
	m4pcchar_t pTemp = ai_stBuf.c_str();
	m4pcchar_t pIni = pTemp;
	pTemp+=ai_IniPos;
	m4char_t ccc = *pTemp;
	m4int32_t nCom=0;

	while (ccc){
		switch (ccc){
			case '\'':
				++nCom;
				break;
			case '|':
				// Lo unico que controlo es que el numero de comillas sea par.
				if (nCom%2==0){
					m4char_t clause =*(pTemp+1);
					if (strchr(M4LDB_POSIBLE_CLAUSES,clause)!=0){
						if (m_pMtSent->_GetClauseAndLevel(pTemp+1,eClause,Level)==M4_SUCCESS){
							ai_eClause = eClause;
							ai_iLevel = Level;
							return pTemp - pIni;
						}
					}
				}
				break;
			case '~':
				if (nCom%2==0){
					m4char_t field =*(pTemp+1);
					if (field=='F'){
						 return  pTemp - pIni;
					}
				}
//			default:
//				break;
		}
		ccc = *(++pTemp);
	}
	return M4_STR_NPOS;
}

m4return_t _AClSQLTranslator::_TranslateOrderBy(string &ai_stTrans,m4int32_t ai_iLevel,m4int32_t &ai_Incr)
{
	m4uint32_t Pos = 0;
	// Miro a ver si tiene algo en el OrderBy
	if (m_pMtSent->_FindClause(*m_pstStatemWhr,M4LDB_CLAUSE_ORDER_BY,ai_iLevel,Pos)!=M4_TRUE){
		return M4_ERROR;
	}
	if ( (*m_pstStatemWhr)[Pos+M4LDB_MD_LENGTH_CLAUSE]!='|'){
		ai_stTrans.append(" ORDER BY ");
		//Bug 0069267.
		ai_Incr = M4LDB_MD_LENGTH_CLAUSE + 1;	// Quitamos la coma.		
	}else{
		//Bug 0069267.
		ai_Incr = M4LDB_MD_LENGTH_CLAUSE;	// No venia coma.
	}

	return M4_SUCCESS;
}

m4return_t _AClSQLTranslator::_TranslateSomeColumns( string *ai_stSource, string &ai_stTrans,m4int32_t ai_indColIni,m4int32_t ai_indColEnd )
{
	if (ai_indColEnd==-1 || ai_indColIni==-1){
		return M4_ERROR;
	}

	// Me posiciono en la primera columna que tenga que incluir.
	itVcClMtColumn	itCol = m_pvcColumns->begin();
	itVcClMtColumn	itColend = m_pvcColumns->end();
	ClMetaItemCol *pCol= 0;
	m4int32_t lPosIni =-1;
	m4int32_t lPosEnd =-1;
	m4bool_t bHasLongItems = M4_FALSE;
	m4bool_t bHasNormalItems = M4_FALSE;
	
	for (;itCol!=itColend;++itCol){
		pCol = (*itCol);
		if (pCol->IsMetaData()==M4_TRUE){
			if (pCol->m_iColumn==ai_indColIni){
				//Encontrado el principio.
				lPosIni = pCol->m_lPos;
				if (pCol->m_eClause==M4LDB_CLAUSE_COLUMN){
					bHasNormalItems =M4_TRUE;
				}

			}
			if (pCol->m_iColumn==ai_indColEnd){
				// Encontrado el final.
//				lPosEnd = pCol->m_lPos;
				break;
			}
			if (pCol->m_eClause==M4LDB_CLAUSE_LONG_COLUMN){
				bHasLongItems =M4_TRUE;
			}
		}
	}
	
	// Tenemos el vector posicionado en la siguiente columna a la ultima.
	// Retrocedemos hasta antes de un separador.
	while ((*(--itCol))->IsSeparator()==M4_TRUE);
	pCol = (*itCol);
	lPosEnd = pCol->m_lPos+pCol->m_lLength;
	

	// Hay que avanzar el vector de Fields hasta llegar a esta columna.
	itVcClMtField	itFld = m_pvcFld->begin();
	itVcClMtField	itFldend = m_pvcFld->end();
	ClMetaField *pField = 0;
	for (;itFld!=itFldend;++itFld,++m_CurrentFld)
	{
		pField = (*itFld);
		if (pField ->IsMetaData()==M4_TRUE){
			if (pField->m_iColumn==ai_indColIni){
				// Encontrado.
				break;
			}
		}
	}


	m4int32_t RealIniPos = -1;
	m4int32_t RealEndPos = -1;

	// Me creo un substring con la cadena que tenga que tratar.
	m4uint32_t ClausePos =-1;
	ai_stTrans.assign("SELECT ");
	if (bHasNormalItems){
		// Tengo que empezar a contar desde la clausula COO.
		if (m_pMtSent->_FindClause (*ai_stSource,M4LDB_CLAUSE_COLUMN,0,ClausePos)!=M4_TRUE){
			return M4_ERROR;
		}
		RealIniPos = ClausePos + M4LDB_MD_LENGTH_CLAUSE+lPosIni+1;	// C00,
		if (bHasLongItems==M4_FALSE){
			RealEndPos = ClausePos + M4LDB_MD_LENGTH_CLAUSE+lPosEnd;
		}
	}
	
	if (bHasLongItems){
		// Dos casos : 
		//	a)que hayamos incluido columnas normales
		//	b)que todas sean Long.
		if (m_pMtSent->_FindClause (*ai_stSource,M4LDB_CLAUSE_LONG_COLUMN,0,ClausePos)!=M4_TRUE){
			return M4_ERROR;
		}
		if (bHasNormalItems==M4_FALSE){
			RealIniPos = ClausePos + M4LDB_MD_LENGTH_CLAUSE+lPosIni+1;	// C00,
		}
		RealEndPos = ClausePos + M4LDB_MD_LENGTH_CLAUSE+lPosEnd;
	}

	// Tengo en RealIniPos y en RealEndPos, el comienzo y fin del trozo de select.
	
	string stColumnPart = ai_stSource->substr(RealIniPos,RealEndPos-RealIniPos);
	if (_Translate(&stColumnPart,ai_stTrans)!=M4_SUCCESS){
		return M4_ERROR;
	}

	// Movemos el vector de fields hasta el separador del where
	itFld=m_pvcFld->begin();
	itFldend = m_pvcFld->end();
	
	itFld+=m_CurrentFld;
	for (;itFld!=itFldend;++itFld,++m_CurrentFld){
		pField = (*itFld);
		if (pField->IsSeparator()==M4_TRUE && pField->m_eClause==M4LDB_CLAUSE_WHERE){
			break;
		}
	}
	return M4_SUCCESS;

}

m4bool_t _AClSQLTranslator::_CheckToUseAlias(void)
{
	if (m_bIsSecurityView == M4_TRUE)
	{
		return M4_TRUE;
	}

	if (m_pvcFrmObj==0){
		return M4_FALSE;
	}

	if (m_pvcFrmObj->empty()){
		return M4_FALSE;
	}

	// Con que haya uno que no sea base o que tenga filtro societario hay que usar alias.
	for (itVcClMtObject itObj=m_pvcFrmObj->begin();itObj!=m_pvcFrmObj->end();++itObj){
		if ((*itObj)->m_bIsBasis == M4_FALSE || (*itObj)->m_bHasSocFilter == M4_TRUE){
			return M4_TRUE;
		}
		else{
			if ((*itObj)->m_MaskRealTables!=1){
				return M4_TRUE;
			}
		}
	}
	return M4_FALSE;
}

m4bool_t _AClSQLTranslator::_CheckForOuterJoins(void)
{
	itVcClMtObject	it = m_pvcFrmObj->begin();
	for(;it != m_pvcFrmObj->end(); ++it)
	{
		if ((*it)->m_eJoinType == M4LDB_OUTER_JOIN)
		{
			return M4_TRUE;
		}
	}
	// No habia. Por si acaso los quitamos.
	_RemoveOuterSigns(*m_pstStatemWhr);

	it = m_pvcFrmObj->begin();
	for(;it != m_pvcFrmObj->end(); ++it)
	{
		m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[(*it)->m_indLogicTable];

		m4int32_t nReal = m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Count();

		for( m4int32_t i = 1; i < nReal; i++ )
		{
			if( (*it)->IsUsingRealTable( i ) == M4_TRUE )
			{
				if( M4LDB_NUMBER( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Register[i].Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_IS_TRANSLATION].Value.GetWithOutSec() ) )
				{
					// Si es de traducciones hay que poner outer
					return M4_TRUE;
				}
			}
		}
	}

	return M4_FALSE;
}

m4bool_t _AClSQLTranslator::_CheckWithOJInLevel(m4int32_t ai_iLevel)
{
	if (_CheckRelationsWithOJInLevel (ai_iLevel) == M4_TRUE) {
		return M4_TRUE;
	}

	if (_CheckAliasWithOJInLevel (ai_iLevel) == M4_TRUE) {
		return M4_TRUE;
	}

	if (_CheckFieldsWithOJInLevel (ai_iLevel) == M4_TRUE) {
		return M4_TRUE;
	}

	return M4_FALSE;
}

m4bool_t _AClSQLTranslator::_CheckRelationsWithOJInLevel(m4int32_t ai_iLevel)
{
	//Comprueba si hay alguna relación marcada como OJ y no se reduce.
	if (m_pvcRel) {

		ClMetaRelat * pRel = NULL;
		itVcClMtRelat	it = m_pvcRel->begin();

		for (;it != m_pvcRel->end(); ++it) {

			pRel = (*it);			
			if ( (pRel) && (pRel->m_eJoinType == M4LDB_OUTER_JOIN) && (pRel->m_bReduce == M4_FALSE) )	{
				//Se ha encontrado una relacion marcada como OJ.
				//Pero debo comprobar que sus alias están en el nivel adecuado.
				//------------------------------------------------------------
				if ((pRel->m_pMtField1) && (pRel->m_pMtField2))	{
					
					if ( (pRel->m_pMtField1->m_indAlias != M4LDB_NULL_INDEX) 
						&& (pRel->m_pMtField1->m_indAlias != M4LDB_NULL_INDEX) ) {
						//---------
							if (m_pvcFrmAls) {
								ClMetaAlias * pAlias1 = NULL;
								ClMetaAlias * pAlias2 = NULL;

								pAlias1 = (*m_pvcFrmAls)[pRel->m_pMtField1->m_indAlias];
								pAlias2 = (*m_pvcFrmAls)[pRel->m_pMtField2->m_indAlias];
								if ( ((pAlias1) && (pAlias1->m_iLevel == ai_iLevel))
									|| ((pAlias1) && (pAlias1->m_iLevel == ai_iLevel)) ) {
										return M4_TRUE;
								}
							}
						//---------
					}					
				}
				//------------------------------------------------------------
			}
		}
	}

	return M4_FALSE;	
}

m4bool_t _AClSQLTranslator::_CheckAliasWithOJInLevel(m4int32_t ai_iLevel)
{
	//Comprobamos los objetos.
	if ((m_pvcFrmObj) && (m_pvcFrmAls) ) {
		
		ClMetaAlias * pAlias = NULL;
		ClMetaObject * pObj = NULL;
		itVcClMtObject	it = m_pvcFrmObj->begin();

		for (;it != m_pvcFrmObj->end(); ++it) {

			pObj = (*it);

			if ( (pObj) && (pObj->m_eJoinType == M4LDB_OUTER_JOIN) 
				&& (pObj->m_indAlias != M4LDB_NULL_INDEX) ) {

					//Se ha encontrado un objeto, pero vamos a ver si el alias es del nivel.
					pAlias = (*m_pvcFrmAls)[pObj->m_indAlias];
					if ((pAlias) && (pAlias->m_iLevel == ai_iLevel) ) {
						return M4_TRUE;
					}
			}
		}
	}

	return M4_FALSE;	
}

m4bool_t _AClSQLTranslator::_CheckFieldsWithOJInLevel(m4int32_t ai_iLevel)
{
	//Comprueba si hay algun campo como OJ.		
	if (m_pvcFld) {
		
		ClMetaField * pFld = NULL;
		itVcClMtField	it = m_pvcFld->begin();

		for (;it != m_pvcFld->end(); ++it) {

			pFld = (*it);			
			if ( (pFld) && (pFld->m_bIsOuter == M4_TRUE) && (pFld->m_iLevel == ai_iLevel) )	{
				//Se ha encontrado un campo marcado como OJ.
				return M4_TRUE;
			}
		}
	}

	return M4_FALSE;	
}

m4return_t _AClSQLTranslator::_TranslatePKs(string &ai_stTrans, ClMetaObject* ai_pTable)
{
	m4return_t result;
	m4bool_t bFirst = M4_TRUE;
	ClRegisterIndex indFld (0);

	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[ai_pTable->m_indLogicTable];
	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[(ClRegisterIndex) 0];

	m4uint32_t numFld = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Count ();


	/* Bug 0155070
	El el caso de tramitación hay que añadir el campo ID_TRAMITATION
	*/
	m4bool_t bTramitation = M4_FALSE ;

	if( ai_pTable->m_iMirrorType == 1 )
	{
		m4pchar_t pcTramitation = NULL;
		m_pGlobVars->GetpAccessRecordSet()->GetpAccess()->GetpChannel()->Tramitation.Get( pcTramitation ) ;

		if( pcTramitation != NULL )
		{
			bTramitation = M4_TRUE ;
		}
	}

	for (indFld=0; indFld<numFld; indFld++)
	{
		m4int32_t lIsPK = M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indFld].Item[M4LDB_ITEM_SYS_DICT_FIELDS_POS_PK].Value.GetWithOutSec ());

		if( lIsPK == 0 && bTramitation == M4_TRUE )
		{
			eFldType_t eIntFld = (eFldType_t) M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Current[indFld].Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_INTERNAL_FIELD].Value.GetWithOutSec ());

			if( eIntFld == M4CL_INTERNAL_TYPE_ID_DATASET && bTramitation == M4_TRUE  )
			{
				lIsPK = 1 ;
			}
		}

		if (lIsPK)
		{
			if (!bFirst) {
				ai_stTrans.append (", ");
			}
			else {
				bFirst = M4_FALSE;
			}

			ClMetaField oFld (0, M4LDB_CLAUSE_COLUMN, M4LDB_EMPTY, 0, 0);

			//Tb definimos el índice del alias ya que como tramtamos el update select, para 
			// este caso la select del cursor query tiene campos de la tabla base y campos de otra 
			// tabla. 
			oFld.m_indAlias = ai_pTable->m_indAlias;
			oFld.m_indRealTable = 0;
			oFld.m_indRealField = indFld;
			oFld.m_indLogicTable = ai_pTable->m_indLogicTable;

			result = _TranslateField (ai_stTrans, &oFld);

			if (result == M4_ERROR) {
				return (M4_ERROR);
			}
		}
	}

	if (bFirst) { //No encontró ninguno
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_NO_PK_FIELDS, ai_pTable->m_stObject.c_str ());
		return (M4_ERROR);
	}

	return (M4_SUCCESS);
}

m4return_t _AClSQLTranslator::_TranslateInsertSubSelect(string &ai_stTrans) 
{

	// Tenemos un filtro de la forma |F00|W00|R00|B01......|E01|O00|E00.
	// Le quitamos las clausulas |F00|W00|R00|B01 y |O00|E00 y lo traducimos. ( nos dara la select que buscamos).
	m_pstStatemWhr->erase(0, 4*M4LDB_MD_LENGTH_CLAUSE);
	m_pstStatemWhr->erase(m_pstStatemWhr->size()-2*M4LDB_MD_LENGTH_CLAUSE, 2*M4LDB_MD_LENGTH_CLAUSE);

	// Moveoms el vector de Fields hasta el nivel 1
	if (m_pvcFld){
		itVcClMtField	itFld = m_pvcFld->begin();
		itVcClMtField	itFldend = m_pvcFld->end();

		ClMetaField * pFld=0;
		for (;itFld !=itFldend;++itFld,++m_CurrentFld){
			pFld = (*itFld);
			// Me salto los fields de las columnas.
			if (pFld->m_eClause == M4LDB_CLAUSE_FROM){
				break;
			}
		}
	}

	if (_Translate(m_pstStatemWhr,ai_stTrans)!=M4_SUCCESS){
		return M4_ERROR;
	}
	
	return M4_SUCCESS;
}


m4return_t	_AClSQLTranslator::_TranslateHintText( m4int32_t ai_iFilter, ClSysHint* ai_poHint, string &aio_sHint )
{

	m4return_t	iResult = M4_ERROR ;
	m4bool_t	bFound = M4_FALSE ;
	m4bool_t	bAdded = M4_FALSE ;
	m4int32_t	i = 0 ;
	m4int32_t	iLength = 0 ;
	m4pcchar_t	pccText = NULL ;
	m4pcchar_t	pccStart = NULL ;
	m4pcchar_t	pccEnd = NULL ;
	m4char_t	aAlias[ M4LDB_SIZE_DEFAULT_STRING + 1 ] ;
	string		sAlias ;
	ClMetaAlias	*poAlias = NULL ;


	pccText = ai_poHint->GetText().c_str() ;

	bFound = M4_TRUE ;
	while( bFound == M4_TRUE )
	{
		if( ( pccStart = strchr( pccText, '&' ) ) != NULL )
		{
			pccEnd = pccStart + 1 ;

			while( isalnum( *pccEnd ) != 0 || *pccEnd == '_' )
			{
				pccEnd++ ;
			}

			aio_sHint.append( pccText, pccStart - pccText ) ;

			sAlias = "" ;
			sAlias.append( pccStart + 1, pccEnd - pccStart - 1 ) ;

			// Si construye el alias y se busca a ver si existe
			sprintf( aAlias, "^%ld_%ld_%s", 0, ai_iFilter, sAlias.c_str() ) ;

			iLength = m_pMtSent->GetNumAlias() ;

			for( i = 0 ; i < iLength ; i++ )
			{
				poAlias =  m_pMtSent->GetAliasByPos( i ) ;

				if( strcmp( poAlias->m_stAlias.c_str(), aAlias ) == 0 )
				{
					if( poAlias->m_bReduce == M4_TRUE )
					{
						// Si el alias está reducido se pone el que han cambiado
						poAlias = m_pMtSent->GetAliasByPos( poAlias->m_indNewAlias ) ;
					}

					iResult = _TranslateAlias( aio_sHint, poAlias , 0, bAdded ) ;

					if( iResult != M4_SUCCESS || bAdded == M4_FALSE )
					{
						return( M4_ERROR ) ;
					}

					break ;
				}
			}

			if( i == iLength )
			{
				DUMP_LDBINSP_ERRORF( m_pGlobVars->GetpLConn(), M4_CH_LDB_WRONG_ALIAS, sAlias.c_str() ) ;
			}

			pccText = pccEnd ;
		}
		else
		{
			aio_sHint.append( pccText ) ;
			bFound = M4_FALSE ;
		}
	}

	return( M4_SUCCESS ) ;
}


m4return_t _AClSQLTranslator::_TranslateBeginStatement(string &ai_stTrans,m4int32_t ai_iLevel){

	if (ai_iLevel!=0){
		ai_stTrans.append(" SELECT ");
		return M4_SUCCESS;
	}
	else{
		ai_stTrans.append("SELECT ");
	}

	if (m_pMtSent->GetStmType() == M4LDB_SELECT && ai_iLevel==0){
		if (m_pMtSent->GetDistinct()){
			ai_stTrans.append(" DISTINCT");
		}

	}
	return M4_SUCCESS;
}

m4return_t _AClSQLTranslator::TranslateSentenceAfter(string &ao_stTrans){
	return M4_SUCCESS;
}

m4bool_t _AClSQLTranslator::_MainSelectHasGroupBy			(void)
{
	m4uint32_t Pos=0;
	if (m_pstStatemWhr==0){
		return M4_FALSE;
	}
	if (m_pMtSent->_FindClause(*m_pstStatemWhr,M4LDB_CLAUSE_REST,0,Pos)==M4_FALSE){
		return M4_FALSE;
	}

	// Me salto la clausula.
	eClauseType_t eClause;

	m4int32_t Level = 0;
	m4int32_t size = m_pstStatemWhr->size();

	Pos+=M4LDB_MD_LENGTH_CLAUSE;
	do{
		Pos = m_pstStatemWhr->find("GROUP BY",Pos);
		if (Pos!=M4_STR_NPOS){
			Pos = m_pMtSent->_GetNextClause(*m_pstStatemWhr,Pos,eClause,Level);
			if (Pos!=M4_STR_NPOS && Level == 0){
				return M4_TRUE;
			}
		}
	}
	while (Pos!=M4_STR_NPOS);

	return M4_FALSE;
}

m4bool_t _AClSQLTranslator::_MainSelectHasOrderBy(void)
{
	m4uint32_t OBPos=0;

	if (m_pstStatemWhr==0){
		return M4_FALSE;
	}
	if (m_pMtSent->_FindClause(*m_pstStatemWhr,M4LDB_CLAUSE_ORDER_BY,0,OBPos)==M4_FALSE){
		return M4_FALSE;
	}
	if ( (*m_pstStatemWhr)[OBPos+M4LDB_MD_LENGTH_CLAUSE]=='|'){
		return M4_FALSE;
	}

	return M4_TRUE;
}

m4return_t _AClSQLTranslator::_TranslateAlias(string &ao_stTrans,ClMetaAlias * ai_pAlias,m4int32_t ai_indRealTable,m4bool_t &ai_bAdded)
{
	ai_bAdded = M4_FALSE;
	if (m_bUsingAlias==M4_FALSE){
		return M4_SUCCESS;
	}

	// Si estamos en nivel 0, y nos handicho que no usemos alias (caso de DELETE sin validaciones).
	if (ai_pAlias->m_iLevel == 0 && m_pMtSent->m_bDontUseAlias){
		return M4_SUCCESS;
	}

	m4int32_t indObj = ai_pAlias->m_indObj;
	if (indObj==-1){
		return M4_ERROR;
	}

	ClMetaObject *pObj = (*m_pvcFrmObj)[indObj ];
	m4char_t aTable [M4LDB_SIZE_DEFAULT_STRING];

	if (pObj->m_bIsBasis)
	{
		if( m_bIsSecurityView == M4_FALSE )
		{
			sprintf(aTable,"BASE_%X",ai_indRealTable);
		}
		else
		{
			sprintf(aTable,"B%X",ai_indRealTable);
		}
	}
	else
	{
		if( m_bIsSecurityView == M4_FALSE )
		{
			sprintf(aTable,"ALIAS_%X_%X", indObj, (m4int32_t) ai_indRealTable);
		}
		else
		{
			sprintf(aTable,"A%X_%X", indObj, (m4int32_t) ai_indRealTable);
		}
	}
	ai_bAdded = M4_TRUE;
	ao_stTrans.append(aTable);

	return M4_SUCCESS;
}

enum eOrientation_t{
	M4LDB_ORIENTATION_ASC = 1,
	M4LDB_ORIENTATION_DESC
};

struct stOrderByField{
	string			m_stField;
	eOrientation_t	m_eOrient;
	stOrderByField(m4pcchar_t ai_pString,int ai_size,eOrientation_t	ai_eOrient){
		m_stField.reserve(50);
		m_stField.assign(ai_pString,ai_size);
		m_eOrient = ai_eOrient;
	}
};

m4return_t	_AClSQLTranslator::_ArrangeOrderBy(string &ao_stTrans, string &ao_stOrderBy)
{
	if (_MainSelectHasOrderBy()==M4_FALSE){
		return M4_SUCCESS;
	}
	// Tenemos que colocar el Order By. Hay dos criterios a tener en cuenta.
	// 1. El orden de los campos es el que me venga. Hay que quitar los repetidos.
	// 2. La orientación de los campos es en el orden inverso al que me vengan. ( El ultimo que me encuentre es el que me da la orientacion)

	// Me defino una lista, donde voy poniendo los elementos.

	list<stOrderByField *>	OrderList;

	// Busco el principio del Order By.

	m4int32_t OBPos = ao_stTrans.rfind(" ORDER BY ");
	m4int32_t Pos = OBPos + 10; //strlen (" ORDER BY ") = 10
	m4int32_t PosEnd = 0;


	m4return_t ret = M4_SUCCESS;
	m4pcchar_t pBegin =0;
	m4pcchar_t pEnd =0;
	m4pcchar_t pTemp =0;
	eOrientation_t eOrient = M4LDB_ORIENTATION_ASC;

	do{
		pBegin = ao_stTrans.c_str()+Pos;
		eOrient = M4LDB_ORIENTATION_ASC;	// Por defecto la orientacion es ascendente
		// Me aseguro de que no he llegado al final.
		if (GetNextCompleteToken(pBegin,PosEnd,',')!=M4_SUCCESS){
			ret = M4_ERROR;
			break;
		}
		if (PosEnd){
			pEnd = pBegin+PosEnd;
			// Buscamos la primera posicion distinta de espacio.
			while (*pBegin==' '){
				pBegin++;
			}
			// Buscamos la ultima posicion distinta de espacio.
			while (*(pEnd-1)==' '){
				pEnd--;
			}
			// Miro cutre pero rapidamente, a ver si hay ASC o DESC.
			// Tengo pEnd al final ( puede ser al final del ASC o en la coma )
			pTemp = pEnd;
			if (*(pTemp-1)=='C'){
				pTemp--;
				if (*(pTemp-1)=='S'){
					pTemp--;
					switch (*(pTemp-1)){
					case 'A':
						pTemp--;
						if (*(pTemp-1) == ' '){
							// Encontrado ASC.
							pEnd = pTemp;
							eOrient = M4LDB_ORIENTATION_ASC;
						}
						break;
					case 'E':
						pTemp--;
						if (*(pTemp-1) == 'D'){
							pTemp--;
							if (*(pTemp-1)==' '){
								// Encontrado DESC.
								pEnd = pTemp-1;
								eOrient = M4LDB_ORIENTATION_DESC;
							}
						}
						break;
					default:
						break;
					}
				}
			}

			// Vuelvo a quitar blancos del final por si acaso.
			while (*(pEnd-1)==' '){
				pEnd--;
			}
			// Tengo entre pBegin y pEnd, la cadena a comparar.
			// Miro a ver si estuviera ya metida en la lista.
			list<stOrderByField *>::iterator	it = OrderList.begin();
			list<stOrderByField *>::iterator	itend = OrderList.end();
			stOrderByField *pstOrder;
			for (;it!=itend;++it){
				pstOrder = (*it);
				// comparamos priemro el tamaño.
				if ((pEnd-pBegin)==(m4int32_t)pstOrder->m_stField.size()){
					if (strncmp(pstOrder->m_stField.c_str(),pBegin,pEnd-pBegin)==0){
						// Encontrado. No lo añadimos. Actualizamos la orientacion, con al que haymaos encontrado ahora.
						pstOrder->m_eOrient = eOrient;
						break;
					}
				}
			}
			if (it==itend){
				// Tengo que insertarlo.
				pstOrder = new stOrderByField(pBegin,pEnd-pBegin,eOrient);
				OrderList.push_back(pstOrder);
			}
			// Si la cadena acaba en 0, PosEnd esta en el caracter siguiente al 0.
			// Si acaba en coma, PosEnd esta en la coma.
			if ((Pos+PosEnd)<(m4int32_t)ao_stTrans.size()){
				// Borramos uno mas.
				PosEnd++;
			}
			// Borramos el trozo de cadena en cualquier caso.
			ao_stTrans.erase(Pos,PosEnd);
		}
	}while (PosEnd);

	if( ret == M4_SUCCESS )
	{
		//Copiamos todos los elementos al Order By.
		// me guardo lo que quede de la Select.
		
		string stFinal( ao_stTrans.c_str() + Pos ) ;
		ao_stTrans.erase( Pos,ao_stTrans.size() - Pos ) ;

		list<stOrderByField *>::iterator	itdump = OrderList.begin() ;
		list<stOrderByField *>::iterator	itdumpend = OrderList.end() ;

		m4bool_t	bFirst = M4_TRUE ;
		m4bool_t	bChanged = M4_FALSE ;

		for( ; itdump != itdumpend ; ++itdump )
		{
			stOrderByField*	pst = *itdump ;

			if( bFirst )
			{
				bFirst = M4_FALSE ;
			}
			else
			{
				ao_stOrderBy.append( ", " ) ;
			}

			m4pcchar_t	pccField = pst->m_stField.c_str() ;

			string		stField ;

			if( m_pMtSent->m_bHasRange == M4_TRUE && m_pGlobVars->GetDDBBType() != M4DBW_ORACLE_DDBB )
			{
				// Solo en oracle se soporta por posición. En el resto se pone el texto de la columna
				m4int32_t	iPosition = atol( pccField ) ;

				if( iPosition > 0 )
				{
					m4char_t	acBuffer1[ 128 + 1 ] ;
					m4char_t	acBuffer2[ 128 + 1 ] ;

					sprintf( acBuffer2, " C%d$", iPosition ) ;

					if( iPosition == 1 )
					{
						sprintf( acBuffer1, "SELECT" ) ;

					}
					else
					{
						sprintf( acBuffer1, " C%d$", iPosition - 1 ) ;
					}

					m4int32_t	iStart = ao_stTrans.find( acBuffer1 ) ;
					m4int32_t	iEnd = ao_stTrans.find( acBuffer2 ) ;

					if( iStart != M4_STR_NPOS && iEnd != M4_STR_NPOS )
					{
						iStart += strlen( acBuffer1 ) + 1 ;
						stField = ao_stTrans.substr( iStart, iEnd - iStart ) ;
						pccField = stField.c_str() ;
						bChanged = M4_TRUE ;
					}
				}
			}

			ao_stOrderBy.append( pccField ) ;

			if( pst->m_eOrient == M4LDB_ORIENTATION_ASC )
			{
				ao_stOrderBy.append( " ASC" ) ;
			}
			else if( pst->m_eOrient == M4LDB_ORIENTATION_DESC )
			{
				ao_stOrderBy.append( " DESC" ) ;
			}
		}

		if( m_pMtSent->m_bHasRange == M4_TRUE )
		{
			// Si es por rangos se quita el order by de la sentencia
			ao_stTrans.erase( Pos - 10, Pos ) ;

			if( bChanged = M4_TRUE )
			{
				m4int32_t	iDistinct = M4_STR_NPOS;
				while( ( iDistinct = ao_stOrderBy.find( "DISTINCT" ) ) != M4_STR_NPOS )
				{
					ao_stOrderBy.erase( iDistinct, 8 ) ;
				}
			}
		}
		else
		{
			ao_stTrans.append( ao_stOrderBy ) ;
		}

		if( stFinal.size() > 0 )
		{
			ao_stTrans.append( " " ) ;
			ao_stTrans.append( stFinal ) ;
		}
	}

	// Vaciamos la lista.
	list<stOrderByField *>::iterator	iterase;
	while( OrderList.size() > 0 )
	{
		iterase = OrderList.begin() ;
		delete *iterase;
		OrderList.erase( iterase ) ;
	}

	return ret;
}


m4return_t	_AClSQLTranslator::_TranslateRanges( string &ao_stTrans, string &ai_stOrderBy )
{
	if( m_pMtSent->m_bHasRange == M4_FALSE )
	{
		return M4_SUCCESS ;
	}

	m4int32_t	iFrom = ao_stTrans.find( " FROM " ) ;

	if( iFrom == M4_STR_NPOS )
	{
		DUMP_LDBINSP_ERRORF( m_pGlobVars->GetpLConn(), M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, "FROM" ) ;
		return M4_ERROR;
	}

	if( ai_stOrderBy.length() == 0 )
	{
		// Si no hay order by se pone la primera columna
		if( m_pGlobVars->GetDDBBType() == M4DBW_ORACLE_DDBB )
		{
			// En oracle se pone la primera columna por posición
			ai_stOrderBy = "1" ;
		}
		else
		{
			// En sqlserver no vale la notación de posición. Se pone el texto de la primera columna sin el distinct
			m4int32_t	iStart = ao_stTrans.find( "SELECT " ) ;
			m4int32_t	iEnd = ao_stTrans.find( " C1$" ) ;

			if( iStart != M4_STR_NPOS && iEnd != M4_STR_NPOS )
			{
				iStart += 7 ;
				ai_stOrderBy = ao_stTrans.substr( iStart, iEnd - iStart ) ;

				m4int32_t	iDistinct = M4_STR_NPOS;
				while( ( iDistinct = ai_stOrderBy.find( "DISTINCT" ) ) != M4_STR_NPOS )
				{
					ai_stOrderBy.erase( iDistinct, 8 ) ;
				}
			}
		}
	}

	ao_stTrans.insert( iFrom, ") ROW_NUMBER$" ) ;
	ao_stTrans.insert( iFrom, ai_stOrderBy ) ;
	ao_stTrans.insert( iFrom, ", ROW_NUMBER() OVER (ORDER BY " ) ;
	ao_stTrans.insert( 0, "SELECT * FROM (" ) ;
	ao_stTrans.append( ") T WHERE T.ROW_NUMBER$ BETWEEN ? AND ? ORDER BY T.ROW_NUMBER$" ) ;

	m_pMtSent->m_pvcParamWhr->push_back( new ClMetaAutoFilter( 0, M4LDB_CLAUSE_WHERE, M4CL_INTERNAL_TYPE_RANGE_START, M4CL_M4_TYPE_NUMBER, 1, M4_FALSE) ) ;
	m_pMtSent->m_pvcParamWhr->push_back( new ClMetaAutoFilter( 0, M4LDB_CLAUSE_WHERE, M4CL_INTERNAL_TYPE_RANGE_END, M4CL_M4_TYPE_NUMBER, 1, M4_FALSE) ) ;

	return M4_SUCCESS ;
}


void	_AClSQLTranslator::_AddSecurityParams( PVcClMtParam ai_pcvParams, string* ai_psObjectsSQL )
{
	m4int32_t		iPosition = -1 ;
	m4int32_t		iParams = 0 ;
	string			stQQQQ( "?" ) ;
	VcClMtParam		vParams ;
	itVcClMtParam	it ;

	if( ai_pcvParams == NULL || m_pvcParamWhr == NULL )
	{
		return;
	}

	/* Bug 0273746
	Los parámetros de las vistas de seguridad deben insertarse en la posción correcta.
	Para ello se busca cuantos parámetros se han volcado ya en la sentencia.
	*/

	// Primero se hace una copia de los parámetros
	vParams.reserve( ai_pcvParams->size() ) ;

	for( it = ai_pcvParams->begin() ; it != ai_pcvParams->end() ; it++ )
	{
		vParams.push_back( (ClMetaParam*) (*it)->Clone() ) ;
	}

	iParams = 0 ;

	// Se buscan parámetros en la cadena total traducida hasta ahora
	if( m_psTranslatedSQL != NULL )
	{
		iPosition = -1 ;
		iParams-- ; 

		do
		{
			iPosition++ ;
			iPosition = FindNotInQuote( *m_psTranslatedSQL, stQQQQ, iPosition ) ;
			iParams++ ;
		}
		while( iPosition != M4_STR_NPOS ) ;
	}

	// Se buscan parámetros en la cadena parcial traducida hasta ahora (tablas con join en SQLServer e Informix)
	if( ai_psObjectsSQL != NULL )
	{
		iPosition = -1 ;
		iParams-- ; 

		do
		{
			iPosition++ ;
			iPosition = FindNotInQuote( *ai_psObjectsSQL, stQQQQ, iPosition ) ;
			iParams++ ;
		}
		while( iPosition != M4_STR_NPOS ) ;
	}

	// Bug 0288761 Hay que descontar los parámetros que están en la parte select
	if( iParams > 0 && m_pvcParamCol != NULL )
	{
		it = m_pvcParamCol->begin();

		do
		{
			if( (*it)->IsMetaData() == M4_TRUE )
			{
				iParams-- ;
			}
			it++ ;
		}
		while( iParams > 0 && it != m_pvcParamCol->end() ) ;
	}

	// Se posiciona en el parámetro correspondiente
	it = m_pvcParamWhr->begin();

	if( iParams > 0 )
	{
		do
		{
			if( (*it)->IsMetaData() == M4_TRUE )
			{
				iParams-- ;
			}
			it++ ;
		}
		while( iParams > 0 && it != m_pvcParamWhr->end() ) ;
	}

	// Se añaden los parámetros a partir del correspondiente
	m_pvcParamWhr->insert( it, vParams.begin(), vParams.end() ) ;
}


