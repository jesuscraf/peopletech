#include "sqltrans_db.hpp"
#include "clmtsent.hpp"
#include "sqlojtrans.hpp"
#include "ldb_log.hpp"
#include "access.hpp"

//Para que muestre informacion de las metarelaciones.
//La clase Translator sí tiene m_pGlobVars, pero es necesario darle ese puntero
//a la clase que vaya ahora a volcar informacion en el ldbinsp.
//Pedro:
#ifdef _M4LDB_DEBUG
	#include "clldb.hpp"
	
	#undef M4LDB_PRINT_DEBUG
	#define M4LDB_PRINT_DEBUG	if (m_pTranslator->GetGlobVars()->GetpLConn()->m_lSysDebugLevel <= m_pTranslator->GetGlobVars()->GetpLConn()->m_pLdb->GetSysDebugLevel ()) m_pTranslator->GetGlobVars()->GetpLConn()->m_pInspector->PrintFileDebug	
	#undef M4LDB_PRINT_DEBUG_TRANSLATOR
	#define M4LDB_PRINT_DEBUG_TRANSLATOR	if (GetGlobVars()->GetpLConn()->m_lSysDebugLevel <= GetGlobVars()->GetpLConn()->m_pLdb->GetSysDebugLevel ()) GetGlobVars()->GetpLConn()->m_pInspector->PrintFileDebug

	#undef M4LDB_PRINT_DEBUG_DETAIL
	#define M4LDB_PRINT_DEBUG_DETAIL	if (m_pTranslator->GetGlobVars()->GetpLConn()->m_lSysDebugLevel <= m_pTranslator->GetGlobVars()->GetpLConn()->m_pLdb->GetSysDebugLevel ()) m_pTranslator->GetGlobVars()->GetpLConn()->m_pInspector->PrintFileDebugDetail
	#undef M4LDB_PRINT_DEBUG_TRANSLATOR_DETAIL
	#define M4LDB_PRINT_DEBUG_TRANSLATOR_DETAIL	if (GetGlobVars()->GetpLConn()->m_lSysDebugLevel <= GetGlobVars()->GetpLConn()->m_pLdb->GetSysDebugLevel ()) GetGlobVars()->GetpLConn()->m_pInspector->PrintFileDebugDetail

#endif
//*******************************

extern m4bool_t str_compare (const char* a_, const char* b_);
extern m4uint8_t			M4LDB_MD_LENGTH_CLAUSE;

m4cint8_t *g_aKywAftWhere[] = { "AND","FOR","FROM","GROUP","HAVING","OR","ORDER","UNION" };
m4cint32_t g_lKywAftWhere = sizeof (g_aKywAftWhere) / sizeof (g_aKywAftWhere[0]);



m4bool_t IsKywAftWhere (m4pcchar_t ai_pToken)
{
	if (!((*ai_pToken >= 'A') && 
		(*ai_pToken <= 'Z'))) {
		// Un atajo. Si el primer carácter no es una letra, seguro que no es un keyword.
		return (M4_FALSE);
	}
	
	return (binary_search( g_aKywAftWhere, g_aKywAftWhere + g_lKywAftWhere, ai_pToken, str_compare ));
}


m4int32_t FindNotInQuote(string &ai_stString, string &ai_stFind, m4uint32_t aio_iStartPos)
{
	m4pcchar_t pcString = ai_stString.c_str() + aio_iStartPos;
	m4pcchar_t pcFind;
	m4bool_t bQuote = M4_FALSE;

	if (aio_iStartPos < ai_stString.size())
	{
		do
		{
			if (M4_FALSE == bQuote)
			{
				pcFind=ai_stFind.c_str();
				do 
				{
					if (*pcFind == '\0')
						return pcString-ai_stString.c_str()-ai_stFind.size();
					if (*pcString == '\0')
						return M4_STR_NPOS;

					if ( (*pcString == '\'') && ((pcString == ai_stString.c_str()) || (*(pcString-1)!='\\')) )
						bQuote = (!bQuote) ? M4_TRUE : M4_FALSE;
					
				} while (*pcString++ == *pcFind++);
			}
			else
			{
				if ( (*pcString == '\'') && ((pcString == ai_stString.c_str()) || (*(pcString-1)!='\\')) )
					bQuote = (!bQuote) ? M4_TRUE : M4_FALSE;

				pcString++;
			}
			
		} while (*pcString != '\0');
	}

	return M4_STR_NPOS;
}


void GetSubToken (m4pcchar_t ai_pcString, m4pcchar_t &ao_pcToken, m4int32_t &ao_lenToken, m4char_t ai_cAditionalSeparator)
{
	//Devuelve todo lo que haya hasta el siguiente '(' ',' ')' '\0' o blanco. 
	//Si el primer caracter es blanco, continua, y devuelve " XXXX". 
	m4char_t ccc;
	ao_lenToken = 0;

	if (*ai_pcString == '\0')
	{
		ao_pcToken = NULL;
		return;
	}

	ao_pcToken = ai_pcString;
	ao_lenToken++;

	ccc = *ai_pcString++;
	if ( (ccc == '(') || (ccc == ')') || (ccc == '\'') || (ccc == ai_cAditionalSeparator) )
	{
		return;
	}

	while (*ai_pcString != '\0')
	{
		ccc = *ai_pcString++;
		if ( (ccc == '(') || (ccc == ')') || (ccc == ' ') || ((ccc == '\'') && (*(ai_pcString-2)!='\\')) || (ccc == ai_cAditionalSeparator) )
		{
			return;
		}
		ao_lenToken++;
	}

	return;
}


m4return_t GetNextCompleteToken(m4pcchar_t ai_pcString, m4int32_t &ao_lenToken, m4char_t ai_cAditionalSeparator)
{
	//Buscamos el final de la cadena.
	ao_lenToken = 0;
	m4bool_t bFound = M4_FALSE, bIgnoreTokens = M4_FALSE;
	m4int32_t iTokenSize=0, iParenthesisOpen=0, iQuoteOpen=0;
	m4pcchar_t pcToken = NULL;
	do
	{
		GetSubToken(ai_pcString+ao_lenToken, pcToken, iTokenSize, ai_cAditionalSeparator);

		if (pcToken == NULL)
		{
			//Fin
			bFound = M4_TRUE;
			break;
		}

		switch (*pcToken)
		{
			case '\'':
				if (iQuoteOpen == 1)
				{
					iQuoteOpen = 0;
				}
				else
				{
					iQuoteOpen = 1;
				}
				break;

			case '(':
				if (iQuoteOpen == 0)
				{
					iParenthesisOpen++;
				}
				break;

			case ')':
				if (iQuoteOpen == 0)
				{
					if (iParenthesisOpen == 0)
					{
						//Fin
						bFound = M4_TRUE;
					}
					else
					{
						iParenthesisOpen--;
					}
				}
				break;

			case ' ':
				if ((M4_FALSE == bIgnoreTokens) && (iParenthesisOpen == 0) && (iQuoteOpen == 0) && (iTokenSize > 1))
				{
					string auxToken(pcToken+1, iTokenSize-1);
					//No solo tenemos un blanco. Vemos si el resto es palabra reservada.
					if (IsKywAftWhere(auxToken.c_str()))
					{
						bFound = M4_TRUE;
					}
					else if (auxToken.compare("SELECT") == 0)
					{
						//Empieza una Select aninadada... Cuerpo a tierra!!
						bIgnoreTokens = M4_TRUE;
					}
				}
				break;
			
			default:
				if ((M4_FALSE == bIgnoreTokens) && (iParenthesisOpen == 0) && (iQuoteOpen == 0))
				{
					string auxToken(pcToken, iTokenSize);
					if (IsKywAftWhere(auxToken.c_str()))
					{
						bFound = M4_TRUE;
					}
					else if (auxToken.compare("SELECT") == 0)
					{
						//Empieza una Select aninadada... Cuerpo a tierra!!
						bIgnoreTokens = M4_TRUE;
					}
					else if (*pcToken == ai_cAditionalSeparator)
					{
						bFound = M4_TRUE;
					}
				}
				break;

		}

		if (M4_FALSE == bFound)
		{
			ao_lenToken+=iTokenSize;
		}
	} 
	while ((pcToken != NULL) && (bFound == M4_FALSE));

	return M4_SUCCESS;
}


m4return_t	ClSQLTranslator_Oracle::TranslateAutomaticSysHints( eClauseType_t ai_eClause, m4int32_t ai_iLevel, string & aio_sTrans )
{
	m4int32_t i;

	//Traducimos el hints automáticos.
	if ((m_pMtSent == NULL) || (m_pMtSent->GetpGlobVars() == NULL)) {
		return M4_ERROR;
	}
	
	//Los hints automáticos SOLO son aplicables a la select de nivel 0.
	if ((ai_eClause != M4LDB_CLAUSE_BEGIN_STATEM) || (ai_iLevel != 0)) {
		return M4_SUCCESS;
	}
	
	//Cogemos los hints automáticos y los metemos.
	string sAutomaticHints("");
	m4bool_t bAdded = M4_FALSE;

	m4int32_t iNumSysHints = m_pMtSent->GetNumSysHint();
	for (i=0; i<iNumSysHints; i++) {
		ClSysHint * pClSysHint = m_pMtSent->GetSysHintByPos(i);
		if (pClSysHint != NULL) {
			//Comprobamos si el tipo de base de datos del hint es como el de la metasentencia para enchufarlo.
			if (pClSysHint->IsEqualByDDBB(m_pMtSent->GetpGlobVars()->GetDDBBType()) == M4_FALSE) {
				continue;
			}
			//Si es un hint automático se enchufa.
			if (pClSysHint->GetIsAutomatic() == M4_TRUE) {
				if (bAdded == M4_TRUE) {
					sAutomaticHints.append(" ");
				}
				else {
					if (TranslateSysHintsBegin(sAutomaticHints) == M4_ERROR) {
						return M4_ERROR;
					}
					bAdded = M4_TRUE;
				}

				// Se pone el nivel de la tabla principal
				if (_TranslateHintText(m_pMtSent->GetMainFilter(), pClSysHint, sAutomaticHints) == M4_ERROR) {
					return M4_ERROR;
				}
			}
		}
	}

	// Se añaden los hints dinámicos
	m4int32_t iNumDynamicHints = m_pMtSent->GetNumDynamicHint();
	for (i=0; i<iNumDynamicHints; i++) {
		ClDynamicHint * pClDynamicHint = m_pMtSent->GetDynamicHintByPos(i);
		if (pClDynamicHint != NULL) {
			if (bAdded == M4_TRUE) {
				sAutomaticHints.append(" ");
			}
			else {
				if (TranslateSysHintsBegin(sAutomaticHints) == M4_ERROR) {
					return M4_ERROR;
				}
				bAdded = M4_TRUE;
			}

			if (TranslateHintBySysHintId(pClDynamicHint->GetFilter(), pClDynamicHint->GetId(), sAutomaticHints) == M4_ERROR) {
				return M4_ERROR;
			}
		}
	}


	if (bAdded == M4_TRUE) {
		if (TranslateSysHintsEnd(sAutomaticHints) == M4_ERROR) {
			return M4_ERROR;
		}
		if (ArrangeSysHintsBeginEnd(aio_sTrans, sAutomaticHints) == M4_ERROR) {
			return M4_ERROR;
		}
	}

	aio_sTrans.append(sAutomaticHints);
	return M4_SUCCESS;
}


m4return_t ClSQLTranslator_Oracle::TranslateHintBySysHintId( m4int32_t ai_iFilter, const string & ai_sSysHintId, string & aio_sTrans )
{
	//Traducimos el hint.

	//Solo tengo en cuenta hints con id!!.
	if (ai_sSysHintId.size() == 0) {
		return M4_SUCCESS;
	}

	//Buscamos el hint para Oracle y con un determinado identificador.
	ClSysHintPk oClSysHintPk( M4LDB_SYSHINT_DB_ORACLE, ai_sSysHintId );

	//Buscamos los hints que tienen que enchufarse!.
	string sRequestHints("");
	m4bool_t bAdded = M4_FALSE;

	m4int32_t iNumSysHints = m_pMtSent->GetNumSysHint();
	for (m4int32_t i=0; i<iNumSysHints; i++) {
		ClSysHint * pClSysHint = m_pMtSent->GetSysHintByPos(i);
		if (pClSysHint != NULL) {
			//Comprobamos si coinciden con pk.
			if (pClSysHint->IsEqualByPk(oClSysHintPk) == M4_FALSE) {
				continue;
			}

			bAdded = M4_TRUE;

			if (_TranslateHintText(ai_iFilter, pClSysHint, sRequestHints) == M4_ERROR) {
				return M4_ERROR;
			}

			//No deberían haber dos hints con misma pk, hubiera saltado la validacion.
			break;
		}
	}
	
	if (bAdded == M4_FALSE) {
		//Se ha especificado un id que no existe.
		DUMP_CHLOG_ERRORF ( M4_CH_LDB_SYSHINT_IDHINT_NO_EXISTS, oClSysHintPk.GetComposedSysHintPk().c_str() );		
		return M4_ERROR;
	}

	aio_sTrans.append(sRequestHints);
	return M4_SUCCESS;	
}


m4return_t ClSQLTranslator_Oracle::TranslateSysHintsBegin( string & aio_sTrans )
{	
	aio_sTrans.append(M4LDB_SYSHINT_ORACLE_BEGIN);
	aio_sTrans.append(" ");
	return M4_SUCCESS;
}


m4return_t ClSQLTranslator_Oracle::TranslateSysHintsEnd( string & aio_sTrans )
{
	aio_sTrans.append(" ");
	aio_sTrans.append(M4LDB_SYSHINT_ORACLE_END);	
	return M4_SUCCESS;
}


m4return_t ClSQLTranslator_Oracle::ArrangeSysHintsBeginEnd( string & aio_sTrans, string & aio_sRealHint )
{
	m4bool_t bRemoved = M4_FALSE;

	//------------------------------------------------------------
	//Si no nos pasan un hint no hace falta hacer nada!.
	m4int32_t iPos = aio_sRealHint.find_first_not_of(" ");
	
	if ((iPos != -1) && ((iPos + strlen(M4LDB_SYSHINT_ORACLE_BEGIN) - 1) < aio_sRealHint.size())) {
		//Se ha encontrado un caracter que no es blanco.
		if (aio_sRealHint.compare(iPos, strlen(M4LDB_SYSHINT_ORACLE_BEGIN), M4LDB_SYSHINT_ORACLE_BEGIN) != 0) {
			//El segundo argumento no es un hint, nos podemos ir!.
			return M4_SUCCESS;
		}
	}
	else {
		//Eran todo blancos.
		return M4_SUCCESS;
	}
	//------------------------------------------------------------

	//Elimina el fin de la izquierda y el inicio de la derecha.

	//Quitamos los de cierre pq se van a meter al final!.
	if (_RemoveSysHintsEnd(aio_sTrans, bRemoved) == M4_ERROR) {
		return M4_ERROR;
	}
	if (bRemoved == M4_TRUE) {
		//Quitamos los de apertura del hint actual!.
		if (_RemoveSysHintsBegin(aio_sRealHint, bRemoved) == M4_ERROR) {
			return M4_ERROR;
		}
		//Si no se quita inicio derecha es pq no había hint a la derecha, necesito insertar fin derecha pq
		//van a concatenarse cadenas. El primer paso elimino el hint fin de la izquierda.
		if (bRemoved == M4_FALSE) {
			//Ojito, en realidad este código no se ejecutará pq al principio se controla que la cadena
			//derecha sea un hint!.
			if (TranslateSysHintsEnd(aio_sRealHint) == M4_ERROR) {
				return M4_ERROR;
			}
		}
	}

	return M4_SUCCESS;
}


m4return_t	ClSQLTranslator_Oracle::MergeSysHints( const string & ai_sSourceString, string & aio_sSysHintTranslated, m4int32_t & aio_iPosBegin)
{
	if (aio_iPosBegin < 0) {
		return M4_ERROR;
	}

	m4int32_t iPosMatch = -1;
	m4int32_t iPos = ai_sSourceString.find_last_not_of(" ", aio_iPosBegin);
	if ((iPos != -1) && (iPos >= strlen(M4LDB_SYSHINT_ORACLE_END))) {
		//Se ha encontrado un caracter que no es blanco.
		if (ai_sSourceString.compare(iPos - strlen(M4LDB_SYSHINT_ORACLE_END) + 1, strlen(M4LDB_SYSHINT_ORACLE_END), M4LDB_SYSHINT_ORACLE_END) == 0) {
			//Se ha encontrado el cierre.
			iPosMatch = iPos - 1;
		}
	}

	if (iPosMatch != -1) {
		//Ojito pq modificamos el argumento de salida!.
		aio_iPosBegin = iPosMatch;
		//Necesitamos quitar el begin del hint.
		m4int32_t iPosBeginTranslated = aio_sSysHintTranslated.find_first_not_of(" ", 0);
		if ((iPosBeginTranslated != -1) && ((iPosBeginTranslated + strlen(M4LDB_SYSHINT_ORACLE_BEGIN)) < aio_sSysHintTranslated.size())) {
			if (aio_sSysHintTranslated.compare(iPosBeginTranslated, strlen(M4LDB_SYSHINT_ORACLE_BEGIN), M4LDB_SYSHINT_ORACLE_BEGIN) == 0) {
				//Se ha encontrado el principio.
				aio_sSysHintTranslated.replace(iPosBeginTranslated, iPosBeginTranslated + strlen(M4LDB_SYSHINT_ORACLE_BEGIN), "");
			}
		}
	}
	return M4_SUCCESS;
}


m4return_t ClSQLTranslator_Oracle::_RemoveSysHintsBegin( string & aio_sTrans, m4bool_t & ao_bRemoved )
{
	ao_bRemoved = M4_FALSE;

	m4int32_t iPosMatch = -1;
	m4int32_t iPos = aio_sTrans.find_first_not_of(" ");
	
	if ((iPos != -1) && ((iPos + strlen(M4LDB_SYSHINT_ORACLE_BEGIN) - 1) < aio_sTrans.size())) {
		//Se ha encontrado un caracter que no es blanco.
		if (aio_sTrans.compare(iPos, strlen(M4LDB_SYSHINT_ORACLE_BEGIN), M4LDB_SYSHINT_ORACLE_BEGIN) == 0) {
			//Se ha encontrado el cierre.
			iPosMatch = iPos;
		}
	}

	//Si se encontro el cierre hay que quitar desde el principio.
	//El espacio despues del begin no se toca pq sirve de separador con el hint anterior.
	if (iPosMatch != -1) {
		aio_sTrans.replace(0, iPosMatch + strlen(M4LDB_SYSHINT_ORACLE_BEGIN), "");
		ao_bRemoved = M4_TRUE;
	}

	return M4_SUCCESS;
}


m4return_t ClSQLTranslator_Oracle::_RemoveSysHintsEnd( string & aio_sTrans, m4bool_t & ao_bRemoved )
{
	ao_bRemoved = M4_FALSE;

	m4int32_t iPosMatch = -1;
	m4int32_t iPos = aio_sTrans.find_last_not_of(" ");
	
	if ((iPos != -1) && (iPos >= strlen(M4LDB_SYSHINT_ORACLE_END))) {
		//Se ha encontrado un caracter que no es blanco.
		if (aio_sTrans.compare(iPos - strlen(M4LDB_SYSHINT_ORACLE_END) + 1, strlen(M4LDB_SYSHINT_ORACLE_END), M4LDB_SYSHINT_ORACLE_END) == 0) {
			//Se ha encontrado el cierre.
			iPosMatch = iPos - 1;
		}
	}

	//Si se encontro el cierre hay que quitarlo todo desde donde se ha encontrado el string de fin.
	//Antes del match siempre hay por lo menos un espacio, lo quitamos tambien.
	if (iPosMatch != -1) {
		aio_sTrans.replace(iPosMatch - 1 , iPosMatch - aio_sTrans.size() + 1, "");
		ao_bRemoved = M4_TRUE;
	}

	return M4_SUCCESS;
}


m4return_t		
ClSQLTranslator_SQLServer::TranslateSentenceAfter(string &ao_stTrans)
{
	// Buscamos la cadena "?=" y la cambiamos por "? =".
	// Para esta version lo hago a lo bruto.
	m4int32_t iPos =M4_STR_NPOS;
	do
	{
		iPos =ao_stTrans.find("?=");
		if (iPos!=M4_STR_NPOS){
			ao_stTrans.replace(iPos,2,"? =");
		}
	}while (iPos!=M4_STR_NPOS);

	if (_RemoveOuterFromWhere(ao_stTrans)!=M4_SUCCESS){
		// Sacamos error.
		DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_ERROR_TRANSLATING_OUTER_JOINS_IN_FIELDS);
		return M4_ERROR;
	}
	
	if (M4_TRUE == _MainSelectHasOrderBy())
	{
		return m_pTrUtils->m_pOrderByTrans->MoveLocalFieldsToBegining(ao_stTrans, "ORDER BY");
	}

	return M4_SUCCESS;
}


m4return_t		
ClSQLTranslator_SQLServer::_RemoveOuterFromWhere(string &ao_stTrans)
{
	//vamos a buscar los OuterJoin del estilo (+) y ponerlos en la parte FROM
	m4return_t ret;
	string stFindPattern("(+)");

	//--------
	//Pedro. bug 0068168.
	//Si estamos validando seguridad, eliminamos los OJ sin más.
	//Funcion para eliminar los OJ. Solo tiene sentido cuando estamos validando seguridad,
	//es decir cuando está implicada la tabla de control SDC_VAL_SEC_LDB.
	if (m_pMtSent->GetValidatingSecurity() == M4_TRUE) {
		#ifdef _M4LDB_DEBUG		
			M4LDB_PRINT_DEBUG_TRANSLATOR (m_pGlobVars->GetpLConn (), "\n\tPossible Outer Join validating security filter.");
			M4LDB_PRINT_DEBUG_TRANSLATOR (m_pGlobVars->GetpLConn (), "\n\t\tFilter before: %s.", ao_stTrans.c_str());
			
			M4LDB_PRINT_DEBUG_TRANSLATOR_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_LOGIC_SENTENCES, m_pGlobVars->GetpLConn (), "\n\tPossible Outer Join validating security filter.");
			M4LDB_PRINT_DEBUG_TRANSLATOR_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_LOGIC_SENTENCES, m_pGlobVars->GetpLConn (), "\n\t\tFilter before: %s.", ao_stTrans.c_str());
		#endif
		ret = _RemoveOuterSigns(ao_stTrans);

		#ifdef _M4LDB_DEBUG			
			M4LDB_PRINT_DEBUG_TRANSLATOR (m_pGlobVars->GetpLConn (), "\n\t\tFilter after: %s.", ao_stTrans.c_str());
			M4LDB_PRINT_DEBUG_TRANSLATOR_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_LOGIC_SENTENCES, m_pGlobVars->GetpLConn (), "\n\t\tFilter after: %s.", ao_stTrans.c_str());
		#endif

		return ret;
	}
	//--------

	m4int32_t iPos = FindNotInQuote(ao_stTrans,stFindPattern);

	while (iPos != M4_STR_NPOS)
	{
		//hemos encontrado lo que buscamos. Ahora tenemos que acotar el principio y el fin de la expresion.
		
		//Antes quitamos el (+)
		ao_stTrans.erase(iPos, 3);

		m4int32_t iSize = ao_stTrans.size();
		m4pcchar_t pcAux = ao_stTrans.c_str();
		m4char_t ccc;
		m4int32_t iBegin=0, iEnd=0, iAliasDot=0;
		
		//Buscamos el principio: pasado el . el principio estará después de blanco o (
		//Es decir ALIAS.FIELD (+)
		iBegin = iPos;
		do
		{
			if (iBegin <= 0){
				//Algo muy malo pasa
				return M4_ERROR;
			}
			ccc = pcAux[--iBegin];
			if (ccc == '.')
			{
				iAliasDot = iBegin;
			}
		}
		while ( ((ccc != ' ') && (ccc != '(')) || (iAliasDot < iBegin) );

		//Ya tenemos el principio en iBegin+1
		iBegin++;

		//Buscamos el final: 
		ret = GetNextCompleteToken(pcAux+iPos, iEnd);

		if (M4_SUCCESS != ret)
			return ret;

		iEnd = iPos + iEnd;
		//Ya tenemos el final en iEnd
		
		//Guardamos la cadena a mover de sitio
		string stMove;
		stMove.assign("(");
		stMove.append(ao_stTrans, iBegin, iEnd-iBegin);
		stMove.append(") AND ");

		//Buscamos el lugar para insertar: despues de ALIAS ON (
		string stFind;
		stFind.assign(stMove, 1, iAliasDot-iBegin);
		stFind.append(" ON (");
		m4int32_t iFindDeltaSize = stFind.size();

		iPos = FindNotInQuote(ao_stTrans,stFind);

		if (iPos==M4_STR_NPOS)
		{
			//No se ha encontrado donde meterlo, se quedará inner, buscamos el siguiente.
			iPos = FindNotInQuote(ao_stTrans,stFindPattern);
			continue;

			/*
			//No hemos encontrado el lugar donde mover la cadena!!
			//Pero el motivo es pq no estaba ya como un OJ. Tendremos que meterlo nosotros.
			
			stMove.assign("(");
			stMove.append(ao_stTrans, iBegin, iEnd-iBegin);
			stMove.append(") ");

			string stFindAlias;
			stFindAlias.assign(stMove, 1, iAliasDot-iBegin);			
			stFindAlias.insert(0, " ");
			stFindAlias.append(" ");

			m4int32_t iPosAlias = FindNotInQuote(ao_stTrans, stFindAlias);
			if (iPosAlias == M4_STR_NPOS) {
				//Malito malito, esto es muuuu raro, no se ha encontrado el alias!.
				return M4_ERROR;
			}

			//Hemos encontrado el alias (franqueado con espacios) pero queremos poner:
			//LEFT JOIN Tabla Alias ON
			//Buscamos el espacio antes de la tabla.
			m4int32_t iPosTable = ao_stTrans.find_last_not_of(" ", iPosAlias);
			if (iPosTable == M4_STR_NPOS) {
				//Un alias sin tabla.
				return M4_ERROR;
			}

			iPosTable = ao_stTrans.find_last_of(" ", iPosTable);
			if (iPosTable == M4_STR_NPOS) {
				//Antes de tabla por lo menos hay un espacio pq es necesario el from.
				return M4_ERROR;
			}

			//Si hay una coma antes tambien la sustituimos por un blanco.
			m4int32_t iPosComma = ao_stTrans.find_last_not_of(" ", iPosTable);
			if ((iPosComma != M4_STR_NPOS) && (ao_stTrans[iPosComma] == ',')) {
				ao_stTrans.replace(iPosComma, 1, " ");
			}

			//Tenemos el espacio antes de tabla. Nos situamos en la primera letra de Tabla.
			iPosTable++;
			
			string sLeftJoin("LEFT JOIN ");
			string sOn(" ON ");

			iPos = iPosAlias + stFindAlias.size();

			//Insertamos ON despues del Alias. Ya está franqueado por blancos.
			ao_stTrans.insert(iPos, sOn);
			//Insertamos despues para no recalcular posicion del la insercion del ON.
			ao_stTrans.insert(iPosTable, sLeftJoin);
			
			//Recalculamos posiciones que se utilizan afuera.
			iBegin = iBegin + sLeftJoin.size() + sOn.size();
			iEnd = iEnd + sLeftJoin.size() + sOn.size();
			iPos = iPos + sLeftJoin.size() + sOn.size();
			iFindDeltaSize = 0;
			*/
		}

		//Bug 0083152.
		//Hacemos la sustitución ahora.
		//La sustituimos por 1=1
		ao_stTrans.replace(iBegin, iEnd-iBegin, "1=1");

		// Antes de insertar hay que mover los parametros para dejarlos en el lugar correcto.
		// Contamos el numero de parametros en la cadena a mover :
		m4int32_t iPosPar = -1;
		// Numero de parametros a mover.
		// Posicion a la que hay que mover.
		// Posicion desde la que mover.
		m4int32_t iNumParToMove = -1,iParamPosTarget = -1,iParamRelPosSource=-1; 
		string	stQQQQ("?");
		
		do
		{
			iPosPar ++;
			iPosPar = FindNotInQuote(stMove,stQQQQ,iPosPar);
			iNumParToMove++;
		}while (iPosPar!=M4_STR_NPOS);

		if (iNumParToMove){

			// Contamos el numero de parametros hasta la posicion iPos ( donde vamos a sustituir )
			iPosPar=-1;
			do
			{
				iPosPar ++;
				iPosPar = FindNotInQuote(ao_stTrans,stQQQQ,iPosPar);
				iParamPosTarget ++;
			}while (iPosPar!=M4_STR_NPOS&&iPosPar<iPos);

			// Contamos el numero hasta la posiciones donde nos vamos a mover.
			iPosPar=iPos;
			
			do
			{
				iPosPar ++;
				iPosPar = FindNotInQuote(ao_stTrans,stQQQQ,iPosPar);
				iParamRelPosSource ++;
			}while (iPosPar!=M4_STR_NPOS&&iPosPar<iBegin);

			
			if (iParamRelPosSource){
				// Si hay que mover los parametros.
				// Contamos los parametros que haya en las columnas.
				m4int32_t nParCol=0;
				itVcClMtParam itPar = m_pvcParamCol->begin();
				itVcClMtParam itParend = m_pvcParamCol->end();
				for (;itPar!=itParend;++itPar ){
					if ((*itPar)->IsMetaData()==M4_TRUE){
						++nParCol;
					}
				}
				// Decrementamos el numero de parametros en los que tengan las columnas;
				if (nParCol){
					iParamPosTarget-=nParCol;
				}
				// Nos colocamos en la posicion a insertar.
				itPar = m_pvcParamWhr->begin();
				itParend = m_pvcParamWhr->end();
				if (iParamPosTarget){
					do{
						if ((*itPar)->IsMetaData()==M4_TRUE){
							iParamPosTarget--;
						}
						++itPar;
					}while (iParamPosTarget);
				}
				else{
					// Nos movemos al primero que no sea separador.
					while ((*itPar)->IsSeparator()==M4_TRUE)
					{
						++itPar;
					}
				}
				// Tenemos itPar la posicion a insertar.
				itVcClMtParam itParSource=itPar;
				do{
					if ((*itParSource)->IsMetaData()==M4_TRUE){
						iParamRelPosSource--;
					}
					++itParSource;
				}while (iParamRelPosSource);

				///////////
				// RTZ.(Bug 40671).
				// El parametro en el que me he quedado puede ser separador.
				// Me muevo hasta el primero que no sea separador.
				while ((*itParSource)->IsMetaData()==M4_FALSE){
					++itParSource;
				}
				///////////

				//Bug 0081719
				//Tendremos un Flag que permitira la ordenacion de parametros en el traductor.
				if( m_pMtSent != NULL && m_pMtSent->GetSortParamsColumnsLimitation() == M4_TRUE )
				{
					// Tenemos itParSource en el punto a mover.
					itVcClMtParam itParSourceEnd = itParSource;

					// Bug 0310422 Se mueve el número de parámetros por tipo
					do
					{
						if( (*itParSourceEnd)->IsMetaData() == M4_TRUE )
						{
							iNumParToMove--;
						}
						itParSourceEnd++;
					}
					while( iNumParToMove > 0 );

					// Tenemos itParSourceEnd en el punto final a mover.
					// Me hago copia de lo que voy a mover.
					VcClMtParam vTemp( itParSource, itParSourceEnd );

					// Borro las que voy a mover.
					m_pvcParamWhr->erase( itParSource, itParSourceEnd );

					// Las inserto en el lugar adecuado.
					m_pvcParamWhr->insert( itPar, vTemp.begin(), vTemp.end() );
				}
			}
		}

		//Añadimos la cadena en el sitio adecuado
		ao_stTrans.insert(iPos + iFindDeltaSize, stMove);

		iPos = FindNotInQuote(ao_stTrans,stFindPattern);
	}	

	return M4_SUCCESS;
}


m4return_t
ClSQLTranslator_SQLServer::_TranslateFrom(string &ai_stTrans,m4int32_t ai_iLevel)
{
	// Tenemos que sacar las relaciones entre tablas a partir del vector de relaciones.
	
	if (m_bOuterJoins){
		// Traducimos los Outer Joins.
		return _TranslateFrom_SQLServer (ai_stTrans,ai_iLevel);
	}
	else{
		// Traducimos igual que en Oracle.
		return _AClSQLTranslator::_TranslateFrom(ai_stTrans,ai_iLevel);
	}

}


m4return_t
ClSQLTranslator_SQLServer::_TranslateRelations		(string &ai_stTrans,m4int32_t ai_iLevel,m4bool_t &bAddedWhere)
{
	if (m_bOuterJoins){
		// Ya se han traducido en el From.
		return M4_SUCCESS;
	}
	else{
		return _AClSQLTranslator::_TranslateRelations	(ai_stTrans,ai_iLevel,bAddedWhere);
	}

}


m4return_t
ClSQLTranslator_SQLServer::_TranslateRealJoins		(string &ai_stTrans,m4int32_t ai_iLevel,m4bool_t &bAddedWhere)
{
	if (m_bOuterJoins){
		// Ya se han traducido en el From.
		return M4_SUCCESS;
	}
	else{
		return _AClSQLTranslator::_TranslateRealJoins	(ai_stTrans,ai_iLevel,bAddedWhere);
	}
}


m4return_t
ClSQLTranslator_SQLServer::_TranslateRealJoins		(string &ai_stTrans,ClMetaObject *ai_pObj,m4bool_t &bAddedWhere)
{
	if (m_bOuterJoins){
		// Ya se han traducido en el From.
		return M4_SUCCESS;
	}
	else{
		return _AClSQLTranslator::_TranslateRealJoins	(ai_stTrans,ai_pObj,bAddedWhere);
	}
}


m4return_t
ClSQLTranslator_SQLServer::_TranslateFrom_SQLServer(string &ai_stTrans,m4int32_t ai_iLevel)
{
	ai_stTrans.append(" FROM ");
	TableAnalyzer oTableAnalyzer(this,m_pvcFrmObj,m_pvcRel,m_pMtSent,ai_iLevel);

	if (oTableAnalyzer.Analyze(ai_iLevel)!=M4_SUCCESS){
		DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_CIRCULAR_REFERENCE_IN_OUTER_JOINS);
		return M4_ERROR;
	}
	// Tengo las relaciones entre tablas guardadas en oTableAnalyzer.
	if (oTableAnalyzer.DumpResults(ai_stTrans)!=M4_SUCCESS){
		DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_ERROR_TRANSLATING_TABLE_REL);
		return M4_ERROR;
	}

	return M4_SUCCESS;
}


// En SQLServer cada tabla traduce sus Joins con la precedente y los joins de sus tablas reales
m4return_t
ClSQLTranslator_SQLServer::TranslateTable(string &ao_stTrans,m4int32_t ai_TableIndex,m4int32_t ai_precIndex)
{
	m4bool_t bAdded = M4_FALSE;

	ClMetaObject *pObj=(*m_pvcFrmObj)[ai_TableIndex];

	// Se calcula el tipo de canal para la herencia de datos y la tramitación
	ClItemIndex iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_REAL_OBJECT ;

	if( pObj->m_iMirrorType == 1 )
	{
		m4pchar_t pcTramitation = NULL;
		m_pGlobVars->GetpAccessRecordSet()->GetpAccess()->GetpChannel()->Tramitation.Get( pcTramitation ) ;

		if( pcTramitation != NULL )
		{
			// En tablas con tramitación hay que ver si se debe utilizar la vista
			iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_TRAMIT_VIEW ;
		}
	}

	if( pObj->m_bInheritsData == M4_TRUE )
	{
		m4double_t	dOrgType ;
		m_pGlobVars->GetpAccessRecordSet()->GetpAccess()->GetpChannel()->OrganizationType.Get( dOrgType ) ;
		m4uint32_t iOrgType = (m4uint32_t) dOrgType ;

		if( iOrgType != M4CL_ORGANIZATION_TYPE_NONE )
		{
			// En tablas con herencia hay que ver si se debe usar la vista
			iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_INHERIT_VIEW ;
		}
	}
	
	// Nos posicionamos en el diccionario.
	m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.MoveTo(pObj->m_indLogicTable);
	// Vamos a las tablas reales.
	m4int32_t nReal = m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Count();
	m4bool_t bAddedObject = M4_FALSE;
	m4int32_t indLastTable =-1;
	m4int32_t indTableJoined = -1;// Indice de la tabla real ligada con el objeto precedente.
	
	if (ai_precIndex!=M4LDB_NULL_INDEX){
		// Tenemos que traducir las relaciones con la tabla anterior.
		// Buscamos la tabla fisica sobre la que se haya establecido la relacion. (Normalmente sera la 0).
		indTableJoined = _FindFisicTableWithRelation(ai_TableIndex,ai_precIndex);
		if (indTableJoined!=-1){
			m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.MoveTo(indTableJoined);
			// De momento, no se comprueba.
			// El problema está en que puede venir de ExecuteDirect y en ese caso no se debería comprobar.
			// Si realemnte tenemos objetos de BBDD distintos, dejaos que de error el gestor.
/*
			m4int32_t idRConn = M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_CONN].Value.GetWithOutSec ());
			if (m_pMtSent->GetIdRConn() != idRConn) {
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_JOIN, m_pMtSent->GetIdObjectBasis () << pObj->m_stObject.c_str());
				return (M4_ERROR);
			}
*/

			m4pcchar_t pTable = NULL;

			if( indTableJoined == 0 && pObj->m_sSecurityView.size() > 0 )
			{
				pTable = pObj->m_sSecurityView.c_str();
				_AddSecurityParams( pObj->m_pvcSecurityParams, &ao_stTrans );
			}
			else
			{
				pTable = M4LDB_PCSTR( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[iTableName].Value.GetWithOutSec() );
			}

			ao_stTrans.append(pTable);
			ao_stTrans.append(" ");

			if( indTableJoined == 0 && pObj->m_sSecurityView.size() > 0 )
			{
				pObj->CleanSecurity();
			}

			// bg 147470
			if( !pObj->m_sFTTParams.empty() )
			{
				ao_stTrans.append (pObj->m_sFTTParams.c_str()) ;
				ao_stTrans.append (" ");
			}
			
			if (_TranslateAlias(ao_stTrans,(*m_pvcFrmAls)[pObj->m_indAlias],indTableJoined,bAdded)!=M4_SUCCESS){
				return M4_ERROR;
			}

			if (m_pMtSent->GetForUpdate()){
				m_pTrUtils->m_pLockTrans->AddLockBetweenJoins(ao_stTrans);
			}

			ao_stTrans.append (" ");

			ao_stTrans.append("ON (");
			if (_TranslateRelations(ao_stTrans,ai_precIndex,ai_TableIndex)!=M4_SUCCESS){
				return M4_ERROR;
			}
			ao_stTrans.append(") ");
			bAddedObject = M4_TRUE;
			indLastTable = indTableJoined;
		}
		else{
			// No hay relacion con la tabla anterior.
			return M4_ERROR;
		}

	}

	m4uint32_t iIsTranslation = 0;

	// Iteramos para ver cuales tenemos que añadir.
	for( m4int32_t indRealTable = 0; indRealTable < nReal; ++indRealTable )
	{
		if( indTableJoined!=indRealTable && pObj->IsUsingRealTable( indRealTable ) )// Ya hemos añadido el objeto de indice indTableJoined.
		{
			m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.MoveTo(indRealTable);
			// De momento, no se comprueba.
			// El problema está en que puede venir de ExecuteDirect y en ese caso no se debería comprobar.
			// Si realemnte tenemos objetos de BBDD distintos, dejaos que de error el gestor.
/*
			m4int32_t idRConn = M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_CONN].Value.GetWithOutSec ());
			if (m_pMtSent->GetIdRConn() != idRConn) {
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_JOIN, m_pMtSent->GetIdObjectBasis () << pObj->m_stObject.c_str());
				return (M4_ERROR);
			}
*/

			m4pcchar_t pTable = NULL;

			if( indRealTable == 0 && pObj->m_sSecurityView.size() > 0 )
			{
				pTable = pObj->m_sSecurityView.c_str();
				_AddSecurityParams( pObj->m_pvcSecurityParams, &ao_stTrans );
			}
			else
			{
				pTable = M4LDB_PCSTR( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[iTableName].Value.GetWithOutSec() );
			}

			if( bAddedObject )
			{
				iIsTranslation = M4LDB_NUMBER( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_IS_TRANSLATION].Value.GetWithOutSec() );

				if (pObj->m_eJoinType == M4LDB_OUTER_JOIN || iIsTranslation == 1 )
				{
					ao_stTrans.append("LEFT JOIN ");
				}
				else
				{
					ao_stTrans.append("INNER JOIN ");
				}
			}

			ao_stTrans.append(pTable);
			ao_stTrans.append(" ");

			if( indRealTable == 0 && pObj->m_sSecurityView.size() > 0 )
			{
				pObj->CleanSecurity();
			}

			// bg 147470
			if( !pObj->m_sFTTParams.empty() )
			{
				ao_stTrans.append(pObj->m_sFTTParams.c_str()) ;
				ao_stTrans.append(" ");
			}

			if( _TranslateAlias(ao_stTrans, (*m_pvcFrmAls)[pObj->m_indAlias], indRealTable, bAdded) != M4_SUCCESS )
			{
				return M4_ERROR;
			}
			ao_stTrans.append(" ");
			

			if( m_pMtSent->GetForUpdate() )
			{
				m_pTrUtils->m_pLockTrans->AddLockBetweenJoins(ao_stTrans);
			}

			if( bAddedObject )
			{
				ao_stTrans.append("ON (");

				if( _TranslateRealJoins(ao_stTrans, ai_TableIndex, indRealTable, indLastTable, iIsTranslation) != M4_SUCCESS )
				{
					return M4_ERROR;
				}
				ao_stTrans.append(") ");
			}
			
			bAddedObject = M4_TRUE;
			indLastTable = indRealTable;
		}
	}
	return M4_SUCCESS;
}


m4return_t
ClSQLTranslator_SQLServer::_TranslateRealJoins(string &ai_stTrans, m4int32_t ai_ObjectIndex, m4int32_t ai_RealTable1, m4int32_t ai_RealTable2, m4uint32_t ai_iIsTranslation)
{
	m4char_t Buffer1[M4LDB_SIZE_DEFAULT_STRING];
	m4char_t Buffer2[M4LDB_SIZE_DEFAULT_STRING];
	m4char_t Buffer[M4LDB_SIZE_DEFAULT_STRING*2];
	ClMetaObject *pObj = (*m_pvcFrmObj)[ai_ObjectIndex];

	if (pObj->m_bIsBasis)
	{
		if( m_bIsSecurityView == M4_FALSE )
		{
			sprintf (Buffer1, "BASE_%X", (m4int32_t) ai_RealTable1);
			sprintf (Buffer2, "BASE_%X", (m4int32_t) ai_RealTable2);
		}
		else
		{
			sprintf (Buffer1, "B%X", (m4int32_t) ai_RealTable1);
			sprintf (Buffer2, "B%X", (m4int32_t) ai_RealTable2);
		}
	}
	else
	{
		if( m_bIsSecurityView == M4_FALSE )
		{
			sprintf (Buffer1, "ALIAS_%X_%X", ai_ObjectIndex, ai_RealTable1);
			sprintf (Buffer2, "ALIAS_%X_%X", ai_ObjectIndex, ai_RealTable2);
		}
		else
		{
			sprintf (Buffer1, "A%X_%X", ai_ObjectIndex, ai_RealTable1);
			sprintf (Buffer2, "A%X_%X", ai_ObjectIndex, ai_RealTable2);
		}
	}

	// Iteramos por los fields para ver cual es PK.
	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.MoveTo(pObj->m_indLogicTable);
	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.MoveTo(0);

	if( ai_iIsTranslation == 1 )
	{
		sprintf(Buffer,"%s.ID_LANGUAGE#=%d AND ",Buffer1, m_pGlobVars->GetLanguage());
		ai_stTrans.append(Buffer);
	}

	m4bool_t bAdded = M4_FALSE;
	m4int32_t numFld = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Count ();

	for (m4int32_t indFld=0; indFld<numFld; indFld++)
	{
		m4int32_t lIsPK = M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Current[indFld].Item[M4LDB_ITEM_SYS_DICT_FIELDS_POS_PK].Value.GetWithOutSec ());

		if (lIsPK)
		{
			// Obtenemos el campo real.
			m4pcchar_t pRealField = M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Current[indFld].Item[M4LDB_ITEM_SYS_DICT_FIELDS_N_REAL_FIELD].Value.GetWithOutSec ());
			if (bAdded)
			{
				ai_stTrans.append(" AND ");
			}
			sprintf(Buffer, "%s.%s=%s.%s", Buffer1, pRealField, Buffer2, pRealField);
			ai_stTrans.append(Buffer);
			bAdded = M4_TRUE;
		}
	}

	return M4_SUCCESS;
}


m4return_t
ClSQLTranslator_SQLServer::_TranslateRelations		(string &ao_stTrans,m4int32_t ai_TableIndex1,m4int32_t ai_TableIndex2)
{

	// Traduccion de relaciones entre dos objetos.
	// Hallamos los indices de los alias.
	m4int32_t iAlias1 = (*m_pvcFrmObj)[ai_TableIndex1]->m_indAlias;
	m4int32_t iAlias2 = (*m_pvcFrmObj)[ai_TableIndex2]->m_indAlias;
	m4int32_t iLevel = (*m_pvcFrmObj)[ai_TableIndex1]->m_iLevel;

	// Buscamos las relaciones que afectan a estos alias, que esten en el mismo nivel.
	itVcClMtRelat itRel = m_pvcRel->begin();
	itVcClMtRelat itRelend = m_pvcRel->end();
	m4bool_t bFirst = M4_TRUE;

	// Hallamos el verdadero indice del Alias de los campos.
	m4int32_t AliasField1 = -1;
	m4int32_t AliasField2 = -1;

	// me guardo el indice sobre el nodo LogicObjects. Luego lo dejo posicionado en el mismo sitio donde estaba.
	m4int32_t IndexLogObj=m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Index;

	for (;itRel!=itRelend;++itRel)
	{
		ClMetaRelat *pRel = (*itRel);
		if (pRel->m_iLevel==iLevel)
		{
			AliasField1=m_pMtSent->GetNewAliasPosByAliasPos(pRel->m_pMtField1->m_indAlias);
			AliasField2=m_pMtSent->GetNewAliasPosByAliasPos(pRel->m_pMtField2->m_indAlias);

			if ( (AliasField1==iAlias1 && AliasField2==iAlias2) ||
				 (AliasField1==iAlias2 && AliasField2==iAlias1) )
			{
				// Hemos encontrado un join a traducir.
				// Lo traducimos.
				if (bFirst){
					bFirst = M4_FALSE;
				}
				else{
					ao_stTrans.append(" AND ");
				}
				if (_TranslateField(ao_stTrans,pRel->m_pMtField1)!=M4_SUCCESS){
					return M4_ERROR;
				}
				ao_stTrans.append("=");
				if (_TranslateField(ao_stTrans,pRel->m_pMtField2)!=M4_SUCCESS){
					return M4_ERROR;
				}

			}

		}
	}

	// Lo dejo posicionado donde estuviera.
	m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.MoveTo(IndexLogObj);

	return M4_SUCCESS;
}


m4return_t		ClSQLTranslator_SQLServer::TranslateCycleRelations		(string &ai_stTrans,m4int32_t ai_TableIndex1,m4int32_t ai_TableIndex2){

	return _TranslateRelations	(ai_stTrans,ai_TableIndex1,ai_TableIndex2);

}
	

m4return_t 
TableRel::Init(m4int32_t ai_NumTables,m4int32_t ai_Tableindex)
{
	m_TableIndex =ai_Tableindex;
	m_bAdded = M4_FALSE;
	
	if (m_ppInner){
		delete []m_ppInner;
	}
	
	m_ppInner = new TableRel *[ai_NumTables-1];

	if (m_ppInner == 0){
		return M4_ERROR;
	}

	if (m_ppOuter){
		delete []m_ppOuter;
	}

	m_ppOuter = new TableRel *[ai_NumTables-1];
	
	if (m_ppOuter == 0){
		return M4_ERROR;
	}
	
	for (m4int32_t i =0 ;i<ai_NumTables-1;++i){
		m_ppInner[i]=0;
		m_ppOuter[i]=0;
	}

	return M4_SUCCESS;
}


TableRel::~TableRel()
{
	if (m_ppInner){
		delete []m_ppInner;
		m_ppInner=0;
	}
	if (m_ppOuter){
		delete []m_ppOuter;
		m_ppOuter=0;
	}

	if (m_psetTableRelSet) {
		m_psetTableRelSet->erase (m_psetTableRelSet->begin (), m_psetTableRelSet->end());
		delete m_psetTableRelSet;
	}
	m_psetTableRelSet = NULL;
}


TableAnalyzer::TableAnalyzer(_AClSQLTranslator	*ai_pTranslator,VcClMtObject	*ai_pObjects,VcClMtRelat	*ai_pRelat,ClMetaSentence	*ai_pMtSent,m4int32_t ai_Level){
	
	m_pRelat = ai_pRelat;
	m_pObjects = ai_pObjects;
	m_pMtSent = ai_pMtSent;
	m_pTranslator = ai_pTranslator;
	m_pTableRel = 0;
	m_nInitTable=0;
	m_nTables=0;
}


TableAnalyzer::~TableAnalyzer()	
{
	if (m_pTableRel){
		delete [] m_pTableRel;
		m_pTableRel=0;
	}
}



//*******************
//Pedro
//Volcamos la informacion de las relaciones.
m4return_t	TableAnalyzer::DumpInfoRelations(m4int32_t ai_iLevel)
{
	m4int32_t Level1 = M4LDB_NULL_INDEX;
	m4int32_t Level2 = M4LDB_NULL_INDEX;
	string sOuter;
	string sRelationSource;
	string sDummyRelation;

	//Recorremos las relaciones y las metemos en un vector.
	itVcClMtRelat itRel = m_pRelat->begin();	
	itVcClMtRelat itRelend = m_pRelat->end();	
	for (;itRel!=itRelend;++itRel)
	{
		ClMetaRelat	*pRel =(*itRel);
		if (pRel->m_iLevel==ai_iLevel){
			////////////////////////////////////////////////
			// Ahora las uniones entre subselects me vienen como relaciones. No las debo tratar aqui.
			Level1 = m_pMtSent->GetAliasByPos(pRel->m_pMtField1->m_indAlias)->m_iLevel;
			Level2 = m_pMtSent->GetAliasByPos(pRel->m_pMtField2->m_indAlias)->m_iLevel;
			if (Level1!=Level2){
				// union entre subselects
				continue;
			}
			//Obtenemos el objeto izquierdo de la relacion.
			ClMetaObject	*pLgcObjLeft = m_pMtSent->GetObjectByPos(m_pMtSent->GetObjectPosByAliasPos(pRel->m_pMtField1->m_indAlias));
			//Obtenemos el objeto derecho de la relacion.
			ClMetaObject	*pLgcObjRight = m_pMtSent->GetObjectByPos(m_pMtSent->GetObjectPosByAliasPos(pRel->m_pMtField2->m_indAlias));
			sOuter = (pRel->m_eJoinType == M4LDB_OUTER_JOIN)? "(+)" : M4LDB_EMPTY;
			sRelationSource = (pRel->m_nRelationSource == M4LDB_APISQL_FROM_SECURITY)? "[Secur.]" : "[ApiSql]";
			sDummyRelation = (pRel->m_bIsDummy == M4_TRUE)? " Dummy" : "";
			#ifdef _M4LDB_DEBUG
				M4LDB_PRINT_DEBUG (m_pTranslator->GetGlobVars()->GetpLConn(), "\n\t\tRelation%s: %s (%s)->%d.%s = (%s)->%d.%s %s", 
																					sDummyRelation.c_str(), 
																					sRelationSource.c_str(), 
																					pLgcObjLeft->m_stObject.c_str(), 
																					pRel->m_pMtField1->m_indAlias, 
																					pRel->m_pMtField1->m_stField.c_str(), 
																					pLgcObjRight->m_stObject.c_str(), 
																					pRel->m_pMtField2->m_indAlias, 
																					pRel->m_pMtField2->m_stField.c_str(), 
																					sOuter.c_str());
				M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_LOGIC_SENTENCES, m_pTranslator->GetGlobVars()->GetpLConn(), "\n\t\tRelation%s: %s (%s)->%d.%s = (%s)->%d.%s %s", 
																					sDummyRelation.c_str(), 
																					sRelationSource.c_str(), 
																					pLgcObjLeft->m_stObject.c_str(), 
																					pRel->m_pMtField1->m_indAlias, 
																					pRel->m_pMtField1->m_stField.c_str(), 
																					pLgcObjRight->m_stObject.c_str(), 
																					pRel->m_pMtField2->m_indAlias, 
																					pRel->m_pMtField2->m_stField.c_str(), 
																					sOuter.c_str());
			#endif
		}		
	}
	return M4_SUCCESS;
}


//Los comparaciones se realizarán sobre el mismo nivel.
//Escenario1:
//	API:		Tabla1 A1, Tabla2 A2, Tabla3 A3
//	Seg:		Tabla1 A10, Tabla2 A11...
//Hay que conseguir que la unión sea coherente, es decir, si la sentencia de seguridad
//hace referencia a Tabla1, independientemente del alias en seguridad, tendremos que asignarle el
//alias que aparece en el API.
//Solución Escenario1:
//	API:		Tabla1 A1, Tabla2 A2, Tabla3 A3
//	Seg:		Tabla1 A1, Tabla2 A2...
//El alias de Tabla1, en seguridad será A1. La implementación consistirá en modificar
//el índice del alias

//Escenario2:
//	API:		Tabla1 A1, Tabla2 A2, Tabla3 A3, Tabla1 A4
//	Seg:		Tabla1 A10, Tabla2 A11...
//En este caso, la referencia que tenemos en seguridad sobre la Tabla1, no sabemos a qué
//alias asociarlo (A1, A4).
//Este escenario devolverá un error de ambigüedad.


m4return_t	TableAnalyzer::ReduceSecurityAlias(m4int32_t ai_iLevel)
{
	m4int32_t Level1 = M4LDB_NULL_INDEX;
	m4int32_t Level2 = M4LDB_NULL_INDEX;
	m4bool_t	bExistsInnerJoin = M4_FALSE;

	//Vector para incluir los Alias izquierdos y derechos.
	vector< m4int32_t >					vIndAliasLeftFound;
	vector< m4int32_t >::iterator		itIndAliasLeftFound;

	vector< m4int32_t >					vIndAliasRightFound;
	vector< m4int32_t >::iterator		itIndAliasRightFound;

	//Recorremos las relaciones y las metemos en un vector.
	itVcClMtRelat itRel = m_pRelat->begin();
	itVcClMtRelat itRelend = m_pRelat->end();
	itVcClMtRelat itLastApiRelation = m_pRelat->end();
	for (;itRel!=itRelend;++itRel)
	{
		ClMetaRelat	*pRel =(*itRel);
		//No tengo en cuenta las dummies para buscar alias.
		if ((pRel->m_iLevel==ai_iLevel) && (pRel->m_bIsDummy == M4_FALSE) ){
			////////////////////////////////////////////////
			// Ahora las uniones entre subselects me vienen como relaciones. No las debo tratar aqui.
			Level1 = m_pMtSent->GetAliasByPos(pRel->m_pMtField1->m_indAlias)->m_iLevel;
			Level2 = m_pMtSent->GetAliasByPos(pRel->m_pMtField2->m_indAlias)->m_iLevel;
			if (Level1!=Level2){
				// union entre subselects
				continue;
			}

			//Si encontramos una sola relación Inner significa que tendremos que
			//controlar los alias repetidos
			if (pRel->m_eJoinType != M4LDB_OUTER_JOIN) {
				bExistsInnerJoin = M4_TRUE;
			}

			//Solo reduzco relaciones que vienen de seguridad. Si la relación es de API
			//no la trataré.
			if (pRel->m_nRelationSource != M4LDB_APISQL_FROM_SECURITY) {
				continue;
			}
			
			//Cuidadín con esto.
			//Si el tipo de relación es OJ, y no hay relaciones INNER no toco nada.
			//Con esto solucionaríamos el problema de que tanto API como SEG son OJ.			
			if ((pRel->m_eJoinType == M4LDB_OUTER_JOIN) && (bExistsInnerJoin == M4_FALSE)) {
				continue;
			}

			//Obtenemos el objeto izquierdo de la relacion.
			ClMetaObject	*pLgcObjLeft = m_pMtSent->GetObjectByPos(m_pMtSent->GetObjectPosByAliasPos(pRel->m_pMtField1->m_indAlias));
			//Obtenemos el objeto derecho de la relacion.
			ClMetaObject	*pLgcObjRight = m_pMtSent->GetObjectByPos(m_pMtSent->GetObjectPosByAliasPos(pRel->m_pMtField2->m_indAlias));

			//Obtenemos el alias izquierdo de la relación.
			m4int32_t		IndAliasLeft = pRel->m_pMtField1->m_indAlias;
			//Obtenemos el alias derecho de la relación.
			m4int32_t		IndAliasRight = pRel->m_pMtField2->m_indAlias;

			//Obtenemos las tablas reales asociadas a cada campo.
			m4int32_t		IndRealTableLeft = pRel->m_pMtField1->m_indRealTable;
			m4int32_t		IndRealTableRight = pRel->m_pMtField2->m_indRealTable;
			
			m4int32_t		IndAliasLeftFound = M4LDB_NULL_INDEX;
			m4int32_t		IndAliasRightFound = M4LDB_NULL_INDEX;
			
			//Comprobamos si los objetos implicados en la relación ya están en el vector
			//de From y el alias asociado es diferente al que estamos reduciendo.
			//El recorrido de From chequea si un objeto está más de dos veces en el API
			//Si existe más de dos veces tenemos ambigüedad, es decir, el alias que 
			//viene de seguridad no sabemos a qué alias del API asociarlo.
			//Si el alias está una sola vez, nos quedamos con el alias del API.
			//Si no está el alias, significa que tenemos un objeto en seguridad
			//que no está en el API, seguirá el proceso normal, sin tratar.
			m4int32_t	IndObj;
			m4bool_t	bExistsLeft = M4_FALSE;
			m4bool_t	bExistsRight = M4_FALSE;
			m4bool_t	bExistsTwice = M4_FALSE;

			ClMetaObject	*pLgcObjCurrent;
			m4int32_t nNumObjects = m_pMtSent->GetNumObject();

			//Si el objeto es el mismo y el alias es diferente significa que hemos
			//encontrado un alias repetido sobre un mismo objeto lógico.
			//Tenemos en cuenta que el objeto logico sea el mismo. Además debe cumplirse que
			//el alias sea diferente para evitar el caso en que se esté comparando consigo mismo.
			//Comprobamos partes izquierda y derecha.

			//Bug 0077372.
			//Se añaden condiciones para sustituir alias, de esta forma nos aseguramos que se cambia a un alias
			//que seguro que saldra en el from.
			//Si se encuentra nos lo apuntamos para poder cambiar mas tarde.

			//Comprobamos que es el mismo objeto logico, que es alias diferente, que la tabla se va a introducir en el from,
			//que la tabla real es la misma. (Controlar el caso de que el objeto lógico tenga varias físicas)


			//-----------------------------
			//Escenario 1:
			//Relaciones 0-1, 1-2, 2-3
			//Estamos en relación de seg. por ejemplo 0-11
			//Buscamos el 11 entre las tablas y si encontramos la tabla con un alias 3 nos lo apuntamos pq despues 
			//tendremos que crear una dummy (11-3), quedando dos relaciones: 0-1, 1-2, 2-3, 0-11, 11-3

			//Escenario 2:
			//Relaciones 0-1, 1-2, 2-3, 3-4
			//Estamos en relación de seg. por ejemplo 0-11, 11-15
			//Buscamos el 11 entre las tablas y si encontramos la tabla con un alias 3 nos lo apuntamos pq despues 
			//tendremos que crear una dummy (11-3).
			//Para la segunda relacion de seguridad se insertaran dos dummys repitiendo 11-3
			//Buscamos el 15 entre las tablas y si encontramos la tabla con un alias 4 nos lo apuntamos pq despues 
			//quedando dos relaciones: 0-1, 1-2, 2-3, 3-4, 0-11, 11-3, 11-3, 15-4.
			//-----------------------------

			//-----------------------------
			bExistsTwice = M4_FALSE;

			for (IndObj = 0; IndObj<nNumObjects; IndObj++) {
				pLgcObjCurrent = m_pMtSent->GetObjectByPos(IndObj);
				if ((pLgcObjCurrent->m_indLogicTable == pLgcObjLeft->m_indLogicTable) 
					&& (pLgcObjCurrent->m_indAlias != IndAliasLeft) 
					&& (pLgcObjCurrent->IsUsingRealTable(0) == M4_TRUE)
					&& ((pLgcObjCurrent->m_MaskRealTables & (1 << IndRealTableLeft)) != 0)			
					) {
						//Si el objeto donde me situo viene de seguridad, no hace falta el dummy					
						if (pLgcObjCurrent->GetAPISQLSource () != M4LDB_APISQL_FROM_APISQL) {
							continue;
						}

						//Dado un objeto lógico la real 0 siempre se inserta, aprovecho para 
						//comprobar si la 0 se tiene que meter. Si la cero no se mete no sustituyo el alias.
						bExistsTwice = (bExistsLeft == M4_TRUE) ? M4_TRUE: M4_FALSE;

						//Si esta repetido entonces hay ambigüedad. Mostramos warning y lo metemos tambien en el vector.
						if (bExistsTwice == M4_TRUE)  {
							M4LDB_PRINT_DEBUG (m_pTranslator->GetGlobVars()->GetpLConn(), "\n\t+ Left Alias Reduction in security is ambigous. TableAnalyzer::ReduceSecurityAlias");
							M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_LOGIC_SENTENCES, m_pTranslator->GetGlobVars()->GetpLConn(), "\n\t+ Left Alias Reduction in security is ambigous. TableAnalyzer::ReduceSecurityAlias");
						}

						bExistsLeft = M4_TRUE;
						vIndAliasLeftFound.push_back (pLgcObjCurrent->m_indAlias);
						//IndAliasLeftFound = pLgcObjCurrent->m_indAlias;
				}
			}
			//-----------------------------

			//-----------------------------
			//Volvemos a dar valor falso.
			bExistsTwice = M4_FALSE;

			for (IndObj = 0; IndObj<nNumObjects; IndObj++) {
				pLgcObjCurrent = m_pMtSent->GetObjectByPos(IndObj);				
				if ((pLgcObjCurrent->m_indLogicTable == pLgcObjRight->m_indLogicTable) 
					&& (pLgcObjCurrent->m_indAlias != IndAliasRight) 
					&& (pLgcObjCurrent->IsUsingRealTable(0) == M4_TRUE)
					&& ((pLgcObjCurrent->m_MaskRealTables & (1 << IndRealTableRight)) != 0)
					) {
						//Si el objeto donde me situo viene de seguridad, no hace falta el dummy					
						if (pLgcObjCurrent->GetAPISQLSource () != M4LDB_APISQL_FROM_APISQL) {
							continue;
						}

						//Dado un objeto lógico la real 0 siempre se inserta, aprovecho para 
						//comprobar si la 0 se tiene que meter. Si la cero no se mete no sustituyo el alias.
						bExistsTwice = (bExistsRight == M4_TRUE) ? M4_TRUE: M4_FALSE;

						//Si esta repetido entonces hay ambigüedad. Mostramos warning y lo metemos tambien en el vector.
						if (bExistsTwice == M4_TRUE)  {
							M4LDB_PRINT_DEBUG (m_pTranslator->GetGlobVars()->GetpLConn(), "\n\t+ Right Alias Reduction in security is ambigous. TableAnalyzer::ReduceSecurityAlias");	
							M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_LOGIC_SENTENCES, m_pTranslator->GetGlobVars()->GetpLConn(), "\n\t+ Right Alias Reduction in security is ambigous. TableAnalyzer::ReduceSecurityAlias");
						}

						bExistsRight = M4_TRUE;
						vIndAliasRightFound.push_back (pLgcObjCurrent->m_indAlias);
						//IndAliasRightFound = pLgcObjCurrent->m_indAlias;
				}
			}
			//-------------------

			//Es la posicion de la última relacion de API. Empiezan las dummy y despues las de seguridad.
			//Si es la primera dummy que se va a insertar
			if (itLastApiRelation == m_pRelat->end()) {
					itLastApiRelation = itRel;
			}

			//------------------------------------------------
			//Bug 0077372, 0077633
			//Se encontro y tenemos el valor.
			//Vamos a cortocircuitar con una relacion outer. En las relaciones dummy no importan los campos, se eliminan
			//antes de que se traduzca la sentencia.
			//------------------------------------------------

			//------------------------
			//Escenario:
			//Relación A0 - A1 (api, outer)
			//Relación A1 - A2 (api, outer)
			//Relación A2 - A3 (api, outer)
			//Relación A2 - A4 (api, outer)
			//Relación A0 - A11 (seg, inner).	Caso tratado en buscada derecha
			//Relación A11- A15 (seg, inner).	Caso general. (se crearan dos dummy, una para la izquierda A3-A11
			//									y otra para la derecha A15-A4
			//En total, para este escenario se crearían tres relaciones dummy, una de izquieda y dos de derecha.

			//Se crea una dummy para cada alias encontrado!.
			for (itIndAliasLeftFound = vIndAliasLeftFound.begin(); itIndAliasLeftFound != vIndAliasLeftFound.end(); itIndAliasLeftFound ++)
			{
				IndAliasLeftFound = (*itIndAliasLeftFound);

				if ((bExistsLeft == M4_TRUE) && (IndAliasLeftFound != M4LDB_NULL_INDEX)) {
					//Copiamos los campos.
					//Sustituimos los alias. En la relacion de seguridad se buscó por alias izquierdo, la relacion
					//dummy se crea con el encontrado a la derecha.!!!
					//en el caso general quedara
					//Resultando A3-A11, A11-A15, A15-A4
					//En este if se añade A3-A11.
					//En el if que trata la derecha se añadira A15-A4
					ClMetaField * pMtFldLeft = new ClMetaField (*(pRel->m_pMtField2));
					ClMetaField * pMtFldRight = new ClMetaField (*(pRel->m_pMtField1));

					//Creamos la relacion fantasma. Es OJ para que no influya para reducir a inner.
					ClMetaRelat	* pDummyRelation = new ClMetaRelat (pRel->m_iLevel, M4LDB_CLAUSE_WHERE, pMtFldLeft, pMtFldRight, M4LDB_OUTER_JOIN);				
					
					pDummyRelation->m_bIsDummy = M4_TRUE;
					//Sustituimos el alias.
					pDummyRelation->m_pMtField1->m_indAlias = IndAliasLeftFound;

					//Insertamos antes de la primera relacion de seguridad.
					m_pRelat->insert (itLastApiRelation ,pDummyRelation);
					//---------------
					//Incrementamos las variables iteradoras pq estamos insertando en el vector antes de la posicion actual!!!!
					itRel ++;
					itRelend ++;
					//---------------
					//Avanzamos el puntero para la siguiente dummy.
					itLastApiRelation ++;
				}
			}	//del for
			//------------------------	

			//------------------------
			//Escenario:
			//Relación A0 - A1 (api, outer)
			//Relación A1 - A2 (api, outer)
			//Relación A2 - A3 (api, outer)
			//Relación A0 - A11 (seg, inner).
			//En la iteracion estamos en A0 - A11
				//Buscamos la tabla derecha A11 y comprobamos que es la misma que la A3. por lo tanto vamos a insertar una dummy:
			//Relación A11 - A3 (api, outer, dummy)
				//Se trata de outer para que no influya y no comvierta todo a inner. Con dummy logramos cerrar el grafo
				//necesario para el algoritmo de reducción de joins.

			//Se crea una dummy para cada alias encontrado!.
			for (itIndAliasRightFound = vIndAliasRightFound.begin(); itIndAliasRightFound != vIndAliasRightFound.end(); itIndAliasRightFound ++)
			{
				IndAliasRightFound = (*itIndAliasRightFound);

				if ((bExistsRight == M4_TRUE) && (IndAliasRightFound != M4LDB_NULL_INDEX)) {
					//Copiamos los campos.
					//Sustituimos los alias. En la relacion de seguridad se buscó por alias derecho, la relacion
					//dummy se crea con el encontrado a la izquierda.!!!
					//Resultando A0-A11, A11-A3
					ClMetaField * pMtFldLeft = new ClMetaField (*(pRel->m_pMtField2));
					ClMetaField * pMtFldRight = new ClMetaField (*(pRel->m_pMtField1));

					//Creamos la relacion fantasma. Es OJ para que no influya para reducir a inner.
					ClMetaRelat	* pDummyRelation = new ClMetaRelat (pRel->m_iLevel, M4LDB_CLAUSE_WHERE, pMtFldLeft, pMtFldRight, M4LDB_OUTER_JOIN);				
					
					pDummyRelation->m_bIsDummy = M4_TRUE;
					//Sustituimos el alias.
					pDummyRelation->m_pMtField2->m_indAlias = IndAliasRightFound;

					//Insertamos antes de la primera relacion de seguridad.
					m_pRelat->insert (itLastApiRelation ,pDummyRelation);
					//---------------
					//Incrementamos las variables iteradoras pq estamos insertando en el vector antes de la posicion actual!!!!
					itRel ++;
					itRelend ++;
					//---------------
					//Avanzamos el puntero para la siguiente dummy.
					itLastApiRelation ++;				
				}
			}	//del for
			//------------------------

			//------------------------
			//Antes de salir o volver a iterar, limpiamos los vectores para la proxima iteracion.			
			if (vIndAliasLeftFound.size () != 0) {
				vIndAliasLeftFound.erase (vIndAliasLeftFound.begin(), vIndAliasLeftFound.end());
			}
			if (vIndAliasRightFound.size () != 0) {
				vIndAliasRightFound.erase (vIndAliasRightFound.begin(), vIndAliasRightFound.end());
			}
			//------------------------

		} // if ((pRel->m_iLevel==ai_iLevel) && (pRel->m_bIsDummy == M4_FALSE) ){
	}
	
	return M4_SUCCESS;	
}


void TableAnalyzer::EraseDummyRelations (void)
{
	ClMetaRelat	* pRel = NULL;

	for (m4int32_t indRel = m_pRelat->size() - 1; indRel>=0; indRel--) {

		itVcClMtRelat itRel = m_pRelat->begin () + indRel;
		pRel = (*itRel);

		if (pRel->m_bIsDummy == M4_TRUE) {
			delete (pRel);
			m_pRelat->erase (itRel);
		}
	}
}


//Pedro. Esta función invocará la algoritmo de chequeo previo de relaciones.
m4return_t TableAnalyzer::AnalyzePreviousCheck(m4int32_t ai_iLevel)
{
	m4int32_t Level1 = M4LDB_NULL_INDEX;
	m4int32_t Level2 = M4LDB_NULL_INDEX;	

	if (ReduceSecurityAlias(ai_iLevel) != M4_SUCCESS) {
		return M4_ERROR;
	}

	//Volcamos la informacion antes del algoritmo.
	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pTranslator->GetGlobVars()->GetpLConn(), "\n\tRelations before algorithm: Level %ld.", ai_iLevel);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_LOGIC_SENTENCES, m_pTranslator->GetGlobVars()->GetpLConn(), "\n\tRelations before algorithm: Level %ld.", ai_iLevel);
		DumpInfoRelations(ai_iLevel);
	#endif

	ClPreviousCheckRelationsGroup oCheckRelationsGroup;

	//Recorremos las relaciones y las metemos en un vector.
	itVcClMtRelat itRel = m_pRelat->begin();
	itVcClMtRelat itRelend = m_pRelat->end();
	for (;itRel!=itRelend;++itRel)
	{
		ClMetaRelat	*pRel =(*itRel);
		if (pRel->m_iLevel==ai_iLevel){
			////////////////////////////////////////////////
			// Ahora las uniones entre subselects me vienen como relaciones. No las debo tratar aqui.
			Level1 = m_pMtSent->GetAliasByPos(pRel->m_pMtField1->m_indAlias)->m_iLevel;
			Level2 = m_pMtSent->GetAliasByPos(pRel->m_pMtField2->m_indAlias)->m_iLevel;
			if (Level1!=Level2){
				// union entre subselects
				continue;
			}
			//Insertamos la relación.
			oCheckRelationsGroup.AddRelation(pRel);
		}
	}

	//Una vez insertadas todas las relaciones iniciamos el chequeo.
	oCheckRelationsGroup.ReduceJoins();

	//Eliminamos las relaciones que se han añadido.
	EraseDummyRelations ();

	//Volcamos la informacion despues del algoritmo.
	#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pTranslator->GetGlobVars()->GetpLConn(), "\n\tRelations after algorithm: Level %ld.", ai_iLevel);
		M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_LOGIC_SENTENCES, m_pTranslator->GetGlobVars()->GetpLConn(), "\n\tRelations after algorithm: Level %ld.", ai_iLevel);
		DumpInfoRelations(ai_iLevel);
	#endif

	return M4_SUCCESS;
}
//********************


m4return_t 
TableAnalyzer::Analyze(m4int32_t ai_iLevel)
{
	m4int32_t Level1 = M4LDB_NULL_INDEX;
	m4int32_t Level2 = M4LDB_NULL_INDEX;
	if (_InitTableRel(ai_iLevel)!=M4_SUCCESS){
		return M4_ERROR;
	}

	/*
	//Pedro: Invocamos al algoritmo que detecta inconsistencias entre las relaciones
	//en función del origen de la relación: APISQL o de SEGURIDAD.
	if (AnalyzePreviousCheck(ai_iLevel) != M4_SUCCESS) {
		return M4_ERROR;
	}
	*/

	// Nos recorremos todas las condiciones.
	m4int32_t nConditions = m_pRelat->size();
	itVcClMtRelat itRel = m_pRelat->begin();
	itVcClMtRelat itRelend = m_pRelat->end();

	//Bug 0081510
	//Vamos a copiar las relaciones en un vector de forma ordenada, primero inner y despues outer.
	typedef list <ClMetaRelat *>		lMetaRelations_t;
	typedef lMetaRelations_t::iterator	lMetaRelationsIt_t;
	lMetaRelations_t oMetaRelations;
	
	for (;itRel != itRelend ;++itRel) {
		ClMetaRelat	*pRel =(*itRel);

		//Lo mismo que abajo!.
		if (pRel->m_iLevel==ai_iLevel){
			Level1 = m_pMtSent->GetAliasByPos(pRel->m_pMtField1->m_indAlias)->m_iLevel;
			Level2 = m_pMtSent->GetAliasByPos(pRel->m_pMtField2->m_indAlias)->m_iLevel;
			if (Level1!=Level2){
				continue;
			}
		}
		switch (pRel->m_eJoinType)	{
			case M4LDB_NORMAL_JOIN:
				//Insertamos al principio.
				oMetaRelations.push_front (pRel);
				break;

			case M4LDB_OUTER_JOIN:
				//Lo insertamos al final.
				oMetaRelations.push_back (pRel);
				break;
			default:
				break;
		}
	}

	//Iteramos por la lista ordenada!!.
	lMetaRelationsIt_t	itRelList = oMetaRelations.begin();
	lMetaRelationsIt_t	itRelendList = oMetaRelations.end();

	for (;itRelList != itRelendList ;++itRelList)
	{
		ClMetaRelat	*pRel =(*itRelList);

		if (pRel->m_iLevel==ai_iLevel){
			////////////////////////////////////////////////
			// Ahora las uniones entre subselects me vienen como relaciones. No las debo tratar aqui.
			Level1 = m_pMtSent->GetAliasByPos(pRel->m_pMtField1->m_indAlias)->m_iLevel;
			Level2 = m_pMtSent->GetAliasByPos(pRel->m_pMtField2->m_indAlias)->m_iLevel;
			if (Level1!=Level2){
				// union entre subselects
				continue;
			}
			////////////////////////////////////////////////
			switch (pRel->m_eJoinType)
			{
			case M4LDB_NORMAL_JOIN:
				// Marcamos las tablas como INNER_JOIN
				if (AddInnerJoin(pRel->m_pMtField1->m_indAlias,pRel->m_pMtField2->m_indAlias)!=M4_SUCCESS){
					return M4_ERROR;
				}
				break;
			case M4LDB_OUTER_JOIN:
				// La primera tabla es la dominante.
				if (AddOuterJoin(pRel->m_pMtField1->m_indAlias,pRel->m_pMtField2->m_indAlias)!=M4_SUCCESS){
					return M4_ERROR;
				}
				break;
			default:
				return M4_ERROR;
			}
		}
	}

	if (_PropagateOuterJoins()!=M4_SUCCESS){
		return M4_ERROR;
	}
	
	//Bug 0080087
	//Añadimos el miembro y un árbol de llamadas para detectar el error A=B(+) and B=C(+) and C=A(+)
	if (_CheckCircularOuterJoins() != M4_SUCCESS){
		return M4_ERROR;
	}

	return M4_SUCCESS;

}


m4return_t 
TableAnalyzer::_PropagateOuterJoins(void){
	// Propagamos los Outer Joins. 
	//	Es decir, A=B(+) AND B=C => nos deja A y C como dominantes y B como dominada.
	//	Nos tiene que dejar A como dominante y B y C como dominadas.
	
	// Recorremos las tablas, buscando las dominadas, y cada una lo propagará.
	for (m4int32_t i=0;i<m_nTables;++i){
		if (m_pTableRel[i].m_eType==LDB_OUTER_DOMINATED){
			// Marcamos todos sus Inner Joins como dominados.
			if (m_pTableRel[i].PropagateOuterJoins()!=M4_SUCCESS){
				return M4_ERROR;
			}
		}
	}

	return M4_SUCCESS;
}


m4return_t TableAnalyzer::_CheckCircularOuterJoins (void)
{	
	for (m4int32_t i=0;i<m_nTables;++i){
		if (m_pTableRel[i].m_eType == LDB_OUTER_DOMINATED){
			//Si la tabla es dominada iniciamos la recursión para comprobar si podemos volver a ella
			//por un grafo de dominadas!.
			if (m_pTableRel[i].CheckCircularOuterJoins() != M4_SUCCESS){
				return M4_ERROR;
			}
		}
	}

	return M4_SUCCESS;
}


m4return_t 
TableAnalyzer::AddInnerJoin(m4int32_t ai_AliasIndex1,m4int32_t ai_AliasIndex2)
{
	// Comprobaciones varias
	if (ai_AliasIndex1==-1 || ai_AliasIndex2==-1){
		return M4_SUCCESS;
	}
	m4int32_t Index1 = m_pMtSent->GetObjectPosByAliasPos(ai_AliasIndex1)-m_nInitTable;
	m4int32_t Index2 = m_pMtSent->GetObjectPosByAliasPos(ai_AliasIndex2)-m_nInitTable;
	
	if (Index1 <0 || Index2 <0){
		return M4_ERROR;
	}
	if ( Index1>=m_nTables || Index2>=m_nTables){
		return M4_ERROR;
	}

	
	////////////////////////
	// Marcamos las dos tablas.

	if (m_pTableRel[Index1].AddInnerJoin(&m_pTableRel[Index2])!=M4_SUCCESS){
		return M4_ERROR;
	}

	if (m_pTableRel[Index2].AddInnerJoin(&m_pTableRel[Index1])!=M4_SUCCESS){
		return M4_ERROR;
	}

	return M4_SUCCESS;
}


m4return_t 
TableAnalyzer::AddOuterJoin(m4int32_t ai_TabIxDominator,m4int32_t ai_TabIxDominated)
{
	// Comprobaciones varias
	if (ai_TabIxDominator==-1 || ai_TabIxDominated==-1){
		return M4_SUCCESS;
	}
	m4int32_t indDominator = m_pMtSent->GetObjectPosByAliasPos(ai_TabIxDominator)-m_nInitTable;
	m4int32_t indDominated = m_pMtSent->GetObjectPosByAliasPos(ai_TabIxDominated)-m_nInitTable;

	if (indDominator>=m_nTables || indDominated>=m_nTables ){
		return M4_ERROR;
	}
	if (indDominator<0 || indDominated<0){
		return M4_ERROR;
	}

	if (m_pTableRel == 0){
		return M4_ERROR;
	}
	////////////////////////
	// Marcamos las dos tablas.
	if (m_pTableRel[indDominator].AddOuterJoinDominator(&m_pTableRel[indDominated])!=M4_SUCCESS){
		return M4_ERROR;
	}
	if (m_pTableRel[indDominated].AddOuterJoinDominated(&m_pTableRel[indDominator])!=M4_SUCCESS){
		return M4_ERROR;
	}

	return M4_SUCCESS;
}


m4return_t 
TableAnalyzer::_InitTableRel(m4int32_t ai_iLevel)
{
	// Contamos cuantos objetos hay de este nivel.
	if (m_pTableRel){
		delete [] m_pTableRel;
		m_pTableRel=0;
	}
	m_nTables=0;
	m_nInitTable=0;
	m_iLevel = ai_iLevel;
	itVcClMtObject itObj = m_pObjects->begin();
	itVcClMtObject itObjend = m_pObjects->end();
	itVcClMtObject itbeginlevel = itObjend ;
	itVcClMtObject itendlevel = itObjend ;

	for (;itObj!=itObjend;++itObj){
		if ((*itObj)->m_iLevel == m_iLevel){
			if (itbeginlevel ==itObjend ){
				// Si no habiamos cogido el principio, lo cogemos ahora.
				itbeginlevel = itObj;
			}

			++m_nTables;
		}
		else{
		if ((*itObj)->m_iLevel > m_iLevel){
			itendlevel = itObj;
			break;
		}
			else{
				++m_nInitTable;
			}

		}
	}
	if (m_nTables==0){
		// Por lo menos tiene que haber uno
		return M4_ERROR;
	}
	
	m_pTableRel = new TableRel[m_nTables];
	
	for (m4int32_t i =0 ;i<m_nTables;++i){
		m_pTableRel[i].Init(m_nTables,m_nInitTable+i);
	}

	return M4_SUCCESS;
}


m4return_t 
TableAnalyzer::DumpResults(string &ao_stStmSQL)
{
	m4int32_t i=0;
	m4bool_t bFirst=M4_TRUE;
	
	// En un caso normal, la primera me debería desencadenar todas las demas.
	// Pueden ocurrir dos cosas.
	//	- Que haya tablas sin JOIN con ninguna otra.
	//  - Que esten unidas por OR.
	// Recorro todas y las voy volcando.
	// Cuando se haya volcado una tabla, estara marcada como añadida y no se repetira.
	
	for (i=0;i<m_nTables;++i){
		// Incluimos las tablas que no estén añadidas y sean dominantes.
		if (m_pTableRel[i].m_bAdded==M4_FALSE && m_pTableRel[i].m_eType!=LDB_OUTER_DOMINATED){
			string stNewObject;
			stNewObject.reserve( 1024 );
			if (m_pTableRel[i].DumpTable(stNewObject,m_pObjects,m_pTranslator,M4LDB_NULL_INDEX)!=M4_SUCCESS){
				return M4_ERROR;
			}
			if (stNewObject.empty()==0){
				if (bFirst==M4_FALSE){
					// Puede haber producto cartesiano.
					// Si estamos aqui, quiere decir que no estan todas las tablas unidas por joins.
					ao_stStmSQL.append(", ");
				}
				else{
					bFirst=M4_FALSE;
				}
				ao_stStmSQL.append(stNewObject);
			}
		}
	}
	return M4_SUCCESS;
}


m4return_t 
TableRel::DumpTable(string &ao_stStmSQL,VcClMtObject *ai_pObjects,_AClSQLTranslator	*ai_pTrans,m4int32_t ai_precIndex)
{
	m_bAdded = M4_TRUE; // Nos marcamos como añadidos.

	// Aqui traducimos los joins ( o outer joins ) con tablas reales.
	if (ai_pTrans->TranslateTable (ao_stStmSQL,m_TableIndex,ai_precIndex)!=M4_SUCCESS){
		return M4_ERROR;

	}

	//Bug 0077247. Una coma de mas en informix
	m4int32_t iStringSize = 0;

	m4int32_t i;
	string stCycleJoins;
	// REFERENCIAS CICLICAS.
	// Miro a ver si alguna de las tablas en Inner esta añadida ya.
	for (i=0;i<m_nInner;++i){
		if (m_ppInner[i] && m_ppInner[i]->m_bAdded){
			// Referencia circular, ya esta añadida.
			// Creamos las relaciones entre estas dos y las metemos en el join anterior.
			stCycleJoins.append(" AND ");
			if (ai_pTrans->TranslateCycleRelations (stCycleJoins,m_TableIndex,m_ppInner[i]->m_TableIndex)!=M4_SUCCESS){
				return M4_ERROR;
			}
		}
	}

	// Lo mismo para los Outer joins. En este caso el gestor puede dar error.
	for (i=0;i<m_nOuter;++i){
		if (m_ppOuter[i] && m_ppOuter[i]->m_bAdded){
			// Referencia circular, ya esta añadida.
			// Creamos las relaciones entre estas dos y las metemos en el join anterior.
			stCycleJoins.append(" AND ");
			if (ai_pTrans->TranslateCycleRelations (stCycleJoins,m_TableIndex,m_ppOuter[i]->m_TableIndex)!=M4_SUCCESS){
				return M4_ERROR;
			}
		}
	}

	// Primero los outer joins
	for (i=0;i<m_nOuter;++i)
	{
		if (m_ppOuter[i] && m_ppOuter[i]->m_bAdded==M4_FALSE){
			ai_pTrans->AppendOJinTables_begin(ao_stStmSQL);	// "LEFT JOIN" o ", OUTER (" 

			//Bug 0077247. Una coma de mas en informix
			iStringSize = ao_stStmSQL.size();

			// Borramos la entrada para que no nos vuelva la llamada.
			if (m_ppOuter[i]->ClearEntrance(this,M4_TRUE)!=M4_SUCCESS){
				return M4_ERROR;
			}
			if (m_ppOuter[i]->DumpTable(ao_stStmSQL,ai_pObjects,ai_pTrans,m_TableIndex)!=M4_SUCCESS){
					return M4_ERROR;
				}

			//Bug 0077247. Una coma de mas en informix
			//Si no se ha añadido nada, eliminamos lo que hemos hecho Append_begin
			if (iStringSize == (m4int32_t) ao_stStmSQL.size() ) {
				ai_pTrans->EraseOJinTables_begin(ao_stStmSQL);
				continue;
			}

			ai_pTrans->AppendOJinTables_end(ao_stStmSQL);
		}
	}
	
	for (i=0;i<m_nInner;++i)
	{
		if (m_ppInner[i] && m_ppInner[i]->m_bAdded==M4_FALSE){
			ai_pTrans->AppendIJinTables_begin(ao_stStmSQL);	// "INNER JOIN" o ", " 

			//Bug 0077247. Una coma de mas en informix
			iStringSize = ao_stStmSQL.size();

			// Borramos la entrada para que no nos vuelva la llamada.
			if (m_ppInner[i]->ClearEntrance(this,M4_FALSE)!=M4_SUCCESS){
				return M4_ERROR;
			}
			
			if (m_ppInner[i]->DumpTable(ao_stStmSQL,ai_pObjects,ai_pTrans,m_TableIndex)!=M4_SUCCESS){
				return M4_ERROR;
			}

			//Bug 0077247. Una coma de mas en informix
			//Si no se ha añadido nada, eliminamos lo que hemos hecho Append_begin
			if (iStringSize == (m4int32_t) ao_stStmSQL.size() ) {
				ai_pTrans->EraseIJinTables_begin(ao_stStmSQL);
				continue;
			}

			ai_pTrans->AppendIJinTables_end(ao_stStmSQL);
		}
	}

	//<<< Pedro. Bug 0051303.
	//El error consistía en que si se detectaba ciclo se insertaba un join inmediatamente
	//después de la tabla (comiéndose un caracter). Por lo tanto, primero detectamos que
	//hay ciclo y lo insertaremos al final.
	//Antes teníamos el código nada mas calcular el string.
	if (!stCycleJoins.empty()){
		// Ha habido ref. circulares.
		// La cadena que me viene acaba en : "A.F=B.F) ".
		/* Bug 0196169
		Para evitar cableos se busca el ')' final
		*/
		size_t iPosition = ao_stStmSQL.rfind( ')' ) ;

		if( iPosition != M4_STR_NPOS )
		{
			ao_stStmSQL.insert(iPosition, stCycleJoins);
		}
	}
	//>>> Pedro.

	return M4_SUCCESS;
}


m4return_t 
TableRel::PropagateOuterJoins(void){
	
	// Recorremos los Inner Joins, para que se propague.
	m_eType = LDB_OUTER_DOMINATED;
	for (m4int32_t i=0;i<m_nInner;++i){

		switch (m_ppInner[i]->m_eType){

		case LDB_OUTER_DOMINATOR:
			// Error. Estamos volviendo a una tabla dominante. No tienen sentido los joins.
			// Tenemos A= B(+) and B= C(+) and A=C(+).
			// El gestor canta error, pero lo damos nosotros antes.

			//La afirmación anterior es falsa!!.

			//sent = sent + "FROM &SQ3_RELACIONES_VIRTUALES ALIAS "
			//sent = sent + ", TABLA__3 ALIAS3 "
			//sent = sent + ", TABLA__4 ALIAS4 "
			//sent = sent + ", TABLA_5 ALIAS5 "
			//sent = sent + " WHERE alias.A = alias3.A(+) "
			//sent = sent + " AND alias3.A = alias4.A "
			//sent = sent + " AND alias4.A = alias5.A(+) "

			//Si aqui damos un error, se estaría dando un error en la propagación pero el gestor no daría error.
			//El escenario es que una dominada tiene un inner con una dominadora.
			//Tenemos A=B(+) And B=D And D=C(+)
			//Accion: No damos el error!!.
			//return M4_ERROR;						
			break;
		case LDB_INNER:
			// Propagamos la marca.
			if (m_ppInner[i]->PropagateOuterJoins()!=M4_SUCCESS){
				return M4_ERROR;
			}
			break;
		default: // LDB_NO_JOIN, LDB_DOMINATED
			break;

		}

	}
	
	return M4_SUCCESS;
}


m4return_t TableRel::CheckCircularOuterJoins (void)
{	
	//Si es dominada tendremos que recorrer la lista para comprobar:
	//A=B(+) and B=C(+) and C=A(+)
	if (!m_psetTableRelSet) {
		return M4_ERROR;
	}
	
	for (TableRelSetIt_t itSet = m_psetTableRelSet->begin (); itSet != m_psetTableRelSet->end(); itSet ++) {
		TableRel * pTableRel = (*itSet);

		if (pTableRel) {
			if (pTableRel->_CheckCircularOuterJoins(this) != M4_SUCCESS) {
				return M4_ERROR;
			}
		}
	}
	
	return M4_SUCCESS;
}


m4return_t TableRel::_CheckCircularOuterJoins (TableRel * ai_pTableRel)
{	
	//La recursion se inicia si hay tablas en el set. En el set solo hay tablas si se detecta un OJ.
	//Si tenemos A=B(+) and B=C(+) and C=A(+), tendremos que todas son dominadas.
	//Los set de las "dominadoras" estaran rellenos:  A tendrá a B, B tendrá a C y C tendrá a A.
	//Si iniciamos en A es dominada, recorro el set y encuentra a B que es dominada por lo tanto
	//por la recursion se cmprueba el set de B que esta C. Con C pasa lo mismo en su set está
	//A que es dominada pero entonces se cumple que he cerrado el ciclo con dominadas --> Error.
	
	//El unico caso que podríamos tener una recursion infinita es en el siguiente supuesto:
	//A=B(+) and B=C(+) and C=D(+) and D=E(+) and E=B(+) and E=A(+)
	//Al empezar en A tendríamos un blucle interno, pero este caso no es posible porque antes del
	//algoritmo en que estamos se comprueba que una tabla no pueda ser dominada por dos, es decir
	//no permitimos A=B(+) and and E=B(+).
	if (!m_psetTableRelSet) {
		return M4_ERROR;
	}

	//P.e.
	//Si tenemos A=B(+) and A=C(+), la tabla A es dominadora y en el set se introduciran B,C.
	//Supongamos que tenemos A=B(+) and A=C(+) and B=C(+) and C=A(+), entonces hay una relación
	//circular pq C no será tampoco dominadora y habré llegado a A. Esto debería dar un error.
	
	//Hemos conseguido completar un ciclo y volver a la misma.
	if (this == ai_pTableRel) {
		return M4_ERROR;
	}

	//Si en el recorrido me encuentro un dominator salimos de la recursion.
	if (m_eType == LDB_OUTER_DOMINATOR) {
		return M4_SUCCESS;
	}
	
	for (TableRelSetIt_t itSet = m_psetTableRelSet->begin (); itSet != m_psetTableRelSet->end(); itSet ++) {
		TableRel * pTableRel = (*itSet);

		if (pTableRel) {
			if (pTableRel->_CheckCircularOuterJoins(ai_pTableRel) != M4_SUCCESS) {
				return M4_ERROR;
			}
		}
	}

	return M4_SUCCESS;
}


m4return_t 
TableRel::ClearEntrance(TableRel *ai_toclean,m4bool_t ai_bIsOuter)
{
	TableRel **ppTableRel=0;
	m4int32_t nJoins=0;
	
	if (ai_bIsOuter){
		ppTableRel = m_ppOuter;
		nJoins = m_nOuter;
	}
	else{
		ppTableRel = m_ppInner;
		nJoins = m_nInner;
	}

	for (m4int32_t i=0;i<nJoins;++i){
		
		if (ppTableRel[i]==ai_toclean){
			ppTableRel[i]=0;
			return M4_SUCCESS;
		}
	}
	return M4_ERROR;
}


m4return_t 
TableRel::AddInnerJoin(TableRel *ai_InnerTable)
{
	m4int32_t i=0;
	if (m_ppInner == 0 || m_ppOuter == 0 ){
		return M4_ERROR;
	}

	// Miramos a ver si no estuviera ya entre los Outers.
	for (i =0;i<m_nOuter;++i){
		if (m_ppOuter[i] == ai_InnerTable){
			// Dos tabla no pueden tener Inner y Outer a la vez.
			return M4_ERROR;
			//Es necesario tener ordenadas las relaciones, primero las inner. El motivo es pq si encontramos
			//la inner no trataremos la outer.
			//Si no las tenemos ordenadas podría saltar este error, se habría metido la outer y cuando
			//trataramos las inner daríamos un error. Si establecemos un orden nos estamos asegurando que
			//primero se mete la inner y despues la outer no llega a meterse.
			//Por lo tanto, este error nunca puede producirse!!!.
		}
	}

	// Miramos a ver si no estuviera ya entre los Inners.
	for (i =0;i<m_nInner;++i){
		if (m_ppInner[i] == ai_InnerTable){
			return M4_SUCCESS;
		}
	}

	// OK. Lo añadimos

	// Si el tipo que tenía es distinto de NO_JOIN, dejamos el que hubiera.
	if (m_eType == LDB_NO_JOIN){
		m_eType = LDB_INNER;
	}

	m_ppInner[m_nInner++] = ai_InnerTable;
	
	return M4_SUCCESS;
}


m4return_t 
TableRel::AddOuterJoinDominator(TableRel *ai_OuterDominated)
{
	m4int32_t i=0;
	
	if (m_ppInner == 0 || m_ppOuter == 0 ){
		return M4_ERROR;
	}

	// Miramos a ver si no estuviera ya entre los Inners.
	for (i =0;i<m_nInner;++i){
		if (m_ppInner[i] == ai_OuterDominated){
			// Dos tabla no pueden tener Inner y Outer a la vez.
			//return M4_ERROR;

			//Bug 0081510
			//Si ya está como inner retornamos M4_SUCCESS.
			//Escenario:
			//FROM A, B WHERE A.C1=B.C1 AND A.C2=B.C2(+).
			//Este caso debería traducirse como Inner, pero no es un error!.
			//Si en la dominadora pasa por aquí, en la dominada también debera detectarse el caso analogo.
			return M4_SUCCESS;

		}
	}

	// Miramos a ver si no estuviera ya entre los Outers.
	for (i =0;i<m_nOuter;++i){
		if (m_ppOuter[i] == ai_OuterDominated){
			// Ya esta contada
			return M4_SUCCESS;
		}
	}

	
	//Bug 0080087
	//Controlar el caso A=B(+) and B=C(+) and C=A(+). Oracle da error.
	//Empleamos un set para meter la dominada. Sera util para recorrerlo más tarde.
	//En este set una dominadora sabrá cuantas dominadas tiene.
	//P.e.
	//Si tenemos A=B(+) and A=C(+), la tabla A es dominadora y en el set se introduciran B,C.
	if (m_psetTableRelSet) {
		m_psetTableRelSet->insert(ai_OuterDominated);
	}

	// OK. Lo añadimos
	
	// Mientras no fuera dominada la dejamos como dominante
	if (m_eType != LDB_OUTER_DOMINATED){
		m_eType = LDB_OUTER_DOMINATOR;
	}



	m_ppOuter[m_nOuter++] = ai_OuterDominated;

	return M4_SUCCESS;

}


m4return_t 
TableRel::AddOuterJoinDominated(TableRel *ai_OuterDominator)
{
	m4int32_t i=0;
	
	if (m_ppInner == 0 || m_ppOuter == 0 ){
		return M4_ERROR;
	}
	// Miramos a ver si no estuviera ya entre los Inners.
	for (i =0;i<m_nInner;++i){
		if (m_ppInner[i] == ai_OuterDominator){
			// Dos tablas no pueden tener Inner y Outer a la vez.
			//return M4_ERROR;

			//Bug 0081510
			//Si ya está como inner retornamos M4_SUCCESS.
			//Escenario:
			//FROM A, B WHERE A.C1=B.C1 AND A.C2=B.C2(+).
			//Este caso debería traducirse como Inner, pero no es un error!.
			//Si en la dominada pasa por aquí, en la dominadora también debera detectarse el caso analogo.
			return M4_SUCCESS;
		}
	}

	// Miramos que no estuviera entre los Outers
	for (i =0;i<m_nOuter;++i){
		if (m_ppOuter[i] == ai_OuterDominator){
			// Ya esta contada
			return M4_SUCCESS;
		}
	}
	
	//Controlamos el siguiente caso:	
	//Tendremos A=B(+) And B=D(+) And C=D(+). Oracle da un error
	//El caso A=B(+) and B=C(+) and A=C(+) tb daríamos un error

	//La informacion que se guarda en la estructura es:
	//			A		B		C
	//Inner		
	//Outer		C		C		A,B
	//Tipo	Domidor	  Domidor  Dominada
	//Si encontramos dos dominadoras que tiene OJ con C, daré error.
	//A, B son dominadoras de C, por lo tanto esto daría error en Oracle.

	//Dos tablas no pueden pretender ser dominadoras de una misma!!.
	if (m_bExistsDominator == M4_TRUE) {
		return M4_ERROR;
	}
	m_bExistsDominator = M4_TRUE;

	// OK. Lo añadimos
	
	m_eType = LDB_OUTER_DOMINATED;

	m_ppOuter[m_nOuter++] = ai_OuterDominator;

	return M4_SUCCESS;


}


m4return_t		
ClSQLTranslator_Informix::TranslateTable(string &ao_stTrans,m4int32_t ai_TableIndex,m4int32_t ai_precIndex)
{

	ClMetaObject *pObj=(*m_pvcFrmObj)[ai_TableIndex];

	// Se calcula el tipo de canal para la herencia de datos y la tramitación
	ClItemIndex iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_REAL_OBJECT ;

	if( pObj->m_iMirrorType == 1 )
	{
		m4pchar_t pcTramitation = NULL;
		m_pGlobVars->GetpAccessRecordSet()->GetpAccess()->GetpChannel()->Tramitation.Get( pcTramitation ) ;

		if( pcTramitation != NULL )
		{
			// En tablas con tramitación hay que ver si se debe utilizar la vista
			iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_TRAMIT_VIEW ;
		}
	}

	if( pObj->m_bInheritsData == M4_TRUE )
	{
		m4double_t	dOrgType ;
		m_pGlobVars->GetpAccessRecordSet()->GetpAccess()->GetpChannel()->OrganizationType.Get( dOrgType ) ;
		m4uint32_t iOrgType = (m4uint32_t) dOrgType ;

		if( iOrgType != M4CL_ORGANIZATION_TYPE_NONE )
		{
			// En tablas con herencia hay que ver si se debe usar la vista
			iTableName = M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_INHERIT_VIEW ;
		}
	}

	// Nos posicionamos en el diccionario.
	m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.MoveTo(pObj->m_indLogicTable);
	// Vamos a las tablas reales.
	m4int32_t nReal = m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Count();

	m4bool_t bAddedObject = M4_FALSE;
	// Iteramos para ver cuales tenemos que añadir.
	m4int32_t nPar = 0; // Contador de cuantos parentesis abrimos.
	for (m4int32_t indRealTable =0;indRealTable<nReal;++indRealTable)
	{
		if ( pObj->IsUsingRealTable (indRealTable) )
		{
			m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.MoveTo(indRealTable);
			// De momento, no se comprueba.
			// El problema está en que puede venir de ExecuteDirect y en ese caso no se debería comprobar.
			// Si realemnte tenemos objetos de BBDD distintos, dejaos que de error el gestor.
/*
			m4int32_t idRConn = M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_CONN].Value.GetWithOutSec ());
			if (m_pMtSent->GetIdRConn() != idRConn) {
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_JOIN, m_pMtSent->GetIdObjectBasis () << pObj->m_stObject.c_str());
				return (M4_ERROR);
			}
*/

			m4pcchar_t pTable = NULL;

			if( indRealTable == 0 && pObj->m_sSecurityView.size() > 0 )
			{
				pTable = pObj->m_sSecurityView.c_str();
				_AddSecurityParams( pObj->m_pvcSecurityParams, &ao_stTrans );
			}
			else
			{
				pTable = M4LDB_PCSTR( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.Item[iTableName].Value.GetWithOutSec() );
			}

			if (bAddedObject){
				ao_stTrans.append(",");
				if (pObj->m_eJoinType == M4LDB_OUTER_JOIN){
					ao_stTrans.append(" OUTER (");
					++nPar;
				}
			}
			// Añadimos nombre de tabla.
			ao_stTrans.append(pTable);
			ao_stTrans.append(" ");

			if( indRealTable == 0 && pObj->m_sSecurityView.size() > 0 )
			{
				pObj->CleanSecurity();
			}

			// Añadimos alias.
			m4bool_t bAdded = M4_FALSE;
			if (_TranslateAlias(ao_stTrans,(*m_pvcFrmAls)[pObj->m_indAlias],indRealTable,bAdded)!=M4_SUCCESS){
				return M4_ERROR;
			}
			bAddedObject = M4_TRUE;
		}
	}
	// Cerramos todos los que hemos abierto.
	while (nPar--){
		ao_stTrans.append(")");
	}

	return M4_SUCCESS;
}


m4return_t		
ClSQLTranslator_Informix::TranslateSentenceAfter(string &ao_stTrans)
{
	m4return_t ret;

	ret = _RemoveOuterSigns(ao_stTrans);

	if (ret != M4_SUCCESS)
		return ret;


	if (M4_TRUE == _MainSelectHasGroupBy())
	{
		ret = m_pTrUtils->m_pOrderByTrans->MoveLocalFieldsToBegining(ao_stTrans, "GROUP BY");

		if (ret != M4_SUCCESS)
			return ret;
	}


	if (M4_TRUE == _MainSelectHasOrderBy())
	{
		return m_pTrUtils->m_pOrderByTrans->MoveLocalFieldsToBegining(ao_stTrans, "ORDER BY");
	}

	return M4_SUCCESS;

}


m4return_t
ClSQLTranslator_SQLServer::_TranslateSpecialRelations		(string &ai_stTrans,m4int32_t ai_iLevel,m4bool_t &ai_bAddedWhere)
{
	// Traducimos las posibles relaciones entre este nivel y alguno de los anteriores.
	// Buscamos relaciones que haya entre este nivel y alguno de los anteriores.
	if (m_bOuterJoins == M4_FALSE){
		// Si no hay Outer Joins se han traducido como relaciones normales.Retornamos.
		return M4_SUCCESS;
	}

	ClMetaRelat *pRel = 0;
	m4int32_t Level1 = M4LDB_NULL_INDEX;
	m4int32_t Level2 = M4LDB_NULL_INDEX;
	itVcClMtRelat itRel = m_pvcRel->begin();
	itVcClMtRelat itRelend = m_pvcRel->end();

	for (;itRel!=itRelend;++itRel)
	{
		pRel = (*itRel);
		if (pRel->m_iLevel == ai_iLevel){
			// Miramos los niveles de cada uno de los objetos.
			Level1 = (*m_pvcFrmAls)[pRel->m_pMtField1->m_indAlias]->m_iLevel;
			Level2 = (*m_pvcFrmAls)[pRel->m_pMtField2->m_indAlias]->m_iLevel;
			if (Level1==Level2){
				// Relacion normal.
				continue;
			}
			// Traducimos la relacion.

			if (ai_bAddedWhere==M4_FALSE){
				ai_stTrans.append(" WHERE ");
				ai_bAddedWhere = M4_TRUE;
			}
			if (_TranslateField(ai_stTrans,pRel->m_pMtField1)!=M4_SUCCESS){
				return M4_ERROR;
			}

			ai_stTrans.append("=");

			if (_TranslateField(ai_stTrans,pRel->m_pMtField2)!=M4_SUCCESS){
				return M4_ERROR;
			}
			ai_stTrans.append(" AND ");
		}
	}

	return M4_SUCCESS;
}


m4int32_t
ClSQLTranslator_SQLServer::_FindFisicTableWithRelation(m4int32_t ai_TableIndex1,m4int32_t ai_TableIndex2)
{
	// Buscamos la tabla fisica sobre la que tenemos la relacion.
	m4int32_t iAlias1 = (*m_pvcFrmObj)[ai_TableIndex1]->m_indAlias;
	m4int32_t iAlias2 = (*m_pvcFrmObj)[ai_TableIndex2]->m_indAlias;
	m4int32_t iLevel = (*m_pvcFrmObj)[ai_TableIndex1]->m_iLevel;

	// Buscamos las relaciones que afectan a estos alias, que esten en el mismo nivel.
	itVcClMtRelat itRel = m_pvcRel->begin();
	itVcClMtRelat itRelend = m_pvcRel->end();
	m4bool_t bFirst = M4_TRUE;

	// Hallamos el verdadero indice del Alias de los campos.
	m4int32_t AliasField1 = -1;
	m4int32_t AliasField2 = -1;

	for (;itRel!=itRelend;++itRel)
	{
		ClMetaRelat *pRel = (*itRel);
		if (pRel->m_iLevel==iLevel)
		{
			AliasField1=m_pMtSent->GetNewAliasPosByAliasPos(pRel->m_pMtField1->m_indAlias);
			AliasField2=m_pMtSent->GetNewAliasPosByAliasPos(pRel->m_pMtField2->m_indAlias);

			if (AliasField1==iAlias1 && AliasField2==iAlias2){
				// Devolvemos la tabla real correspondiente.
				return pRel->m_pMtField1->m_indRealTable;
			}
			else{
				if (AliasField1==iAlias2 && AliasField2==iAlias1){
					// Devolvemos la tabla real correspondiente.
					return pRel->m_pMtField2->m_indRealTable;
				}
			}
		}
	}
	return -1;
}


m4return_t		
_AClSQLTranslator::_RemoveOuterSigns(string &ao_stTrans)
{
	//vamos a buscar los (+) quitarlos
	string stFindPattern("(+)");
	m4int32_t iPos = FindNotInQuote(ao_stTrans,stFindPattern);

	while (iPos != M4_STR_NPOS)
	{
		ao_stTrans.erase(iPos, 3);
		iPos = FindNotInQuote(ao_stTrans,stFindPattern,iPos);
	}

	return M4_SUCCESS;
}


m4return_t		
ClSQLTranslator_Informix::_TranslateFrom			(string &ai_stTrans,m4int32_t ai_iLevel)
{
	if (m_bOuterJoins){
		// Traducimos los Outer Joins.
		return _TranslateFrom_Informix (ai_stTrans,ai_iLevel);
	}
	else{
		// Traducimos igual que en Oracle.
		return _AClSQLTranslator::_TranslateFrom(ai_stTrans,ai_iLevel);
	}
}


m4return_t
ClSQLTranslator_Informix::_TranslateFrom_Informix(string &ai_stTrans,m4int32_t ai_iLevel)
{
	ai_stTrans.append(" FROM ");
	TableAnalyzer oTableAnalyzer(this,m_pvcFrmObj,m_pvcRel,m_pMtSent,ai_iLevel);

	if (oTableAnalyzer.Analyze(ai_iLevel)!=M4_SUCCESS){
		DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_CIRCULAR_REFERENCE_IN_OUTER_JOINS);
		return M4_ERROR;
	}
	// Tengo las relaciones entre tablas guardadas en oTableAnalyzer.
	if (oTableAnalyzer.DumpResults(ai_stTrans)!=M4_SUCCESS){
		DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_ERROR_TRANSLATING_TABLE_REL);
		return M4_ERROR;
	}

	return M4_SUCCESS;
}


void
ClSQLTranslator_SQLServer::AppendOJinTables_begin(string &ao_stTrans)
{
	ao_stTrans.append ("LEFT JOIN ");
}


void
ClSQLTranslator_SQLServer::AppendOJinTables_end(string &ao_stTrans)
{
}


void
ClSQLTranslator_SQLServer::AppendIJinTables_begin(string &ao_stTrans)
{
	ao_stTrans.append ("INNER JOIN ");
}


void
ClSQLTranslator_SQLServer::AppendIJinTables_end(string &ao_stTrans)
{
}


//Bug 0077247. Una coma de mas en informix
void
ClSQLTranslator_SQLServer::EraseOJinTables_begin(string &ao_stTrans)
{
	//Nos aseguramos que está la cadena justo antes. Si es = 0 es que sí estaba.
	if (!strcmpi( ao_stTrans.c_str() + (ao_stTrans.size() - strlen("LEFT JOIN ")),"LEFT JOIN ")) {
		ao_stTrans.erase(ao_stTrans.size() - strlen("LEFT JOIN "));
	}
}


void
ClSQLTranslator_SQLServer::EraseOJinTables_end(string &ao_stTrans)
{
}


void
ClSQLTranslator_SQLServer::EraseIJinTables_begin(string &ao_stTrans) 
{
	//Nos aseguramos que está la cadena justo antes. Si es = 0 es que sí estaba.
	if (!strcmpi( ao_stTrans.c_str() + (ao_stTrans.size() - strlen("INNER JOIN ")),"INNER JOIN ")) {
		ao_stTrans.erase(ao_stTrans.size() - strlen("INNER JOIN "));
	}
}


void
ClSQLTranslator_SQLServer::EraseIJinTables_end(string &ao_stTrans)
{
}


//Bug 0077127
//El paso previo a la llamada se ha habrá metido append(" AND "). Ver stCycleJoins.
//En informix no se une con AND, por lo tanto hay que eliminarlo.
m4return_t	ClSQLTranslator_Informix::TranslateCycleRelations (string &ai_stTrans,m4int32_t ai_TableIndex1,m4int32_t ai_TableIndex2) {
	//Nos aseguramos que nos han puesto " AND ". Si no es damos un error!.
	if (strcmpi( ai_stTrans.c_str() + (ai_stTrans.size() - 5)," AND ")) {
		return M4_ERROR;
	}
	//Quitamos el " AND ".
	if (ai_stTrans.size() >= 5) {
		ai_stTrans.erase (ai_stTrans.size() - 5);
	}
	return M4_SUCCESS;
}


void
ClSQLTranslator_Informix::AppendOJinTables_begin(string &ao_stTrans)
{
	ao_stTrans.append (",OUTER (");
}


void
ClSQLTranslator_Informix::AppendOJinTables_end(string &ao_stTrans)
{
	ao_stTrans.append (")");
}


void
ClSQLTranslator_Informix::AppendIJinTables_begin(string &ao_stTrans)
{
	ao_stTrans.append (", ");
}


void
ClSQLTranslator_Informix::AppendIJinTables_end(string &ao_stTrans)
{
}


//Bug 0077247. Una coma de mas en informix
void
ClSQLTranslator_Informix::EraseOJinTables_begin(string &ao_stTrans)
{
	//Nos aseguramos que está la cadena justo antes. Si es = 0 es que sí estaba.
	if (!strcmpi( ao_stTrans.c_str() + (ao_stTrans.size() - strlen(",OUTER (")),",OUTER (")) {
		ao_stTrans.erase(ao_stTrans.size() - strlen(",OUTER ("));
	}
}


void
ClSQLTranslator_Informix::EraseOJinTables_end(string &ao_stTrans)
{
	//Nos aseguramos que está la cadena justo antes. Si es = 0 es que sí estaba.
	if (!strcmpi( ao_stTrans.c_str() + (ao_stTrans.size() - strlen(")")),")")) {
		ao_stTrans.erase(ao_stTrans.size() - strlen(")"));
	}

}


void
ClSQLTranslator_Informix::EraseIJinTables_begin(string &ao_stTrans)
{
	//Nos aseguramos que está la cadena justo antes. Si es = 0 es que sí estaba.
	if (!strcmpi( ao_stTrans.c_str() + (ao_stTrans.size() - strlen(", ")),", ")) {
		ao_stTrans.erase(ao_stTrans.size() - strlen(", "));
	}
}


void
ClSQLTranslator_Informix::EraseIJinTables_end(string &ao_stTrans)
{
}


//**********************************************************
//--- Pedro
//Definicion de los miembros de las clases relacionadas con la comprobacion previa de relaciones.


//********** Clase que engloba relaciones. Punto de entrada******//
ClPreviousCheckRelationsGroup::ClPreviousCheckRelationsGroup()
{
	//Creamos los vectores de punteros: de tablas y de relaciones.	
	//Reservamos 8 porque por defecto reserva 1024 (Jesús).
	m_oTables.reserve(8);
	//No se reserva espacio si es de tipo lista.
	//m_oRelations.reserve(8);
}


ClPreviousCheckRelationsGroup::~ClPreviousCheckRelationsGroup()
{
	//La liberacion del vector STL es automatica pero los punteros que almacenamos los debemos
	//liberar nosotros.		
	//Liberamos las tablas y las relaciones que se han añadido.
	//Los vectore..., son contenedores de información pero la destrucción del vector no supone la
	//liberacion de los punteros que puedan contener los vectores.
	//En este caso tenemo colecciones de punteros y es necesario eliminarlos antes de que el objeto
	//colección se destruya.
	//Recordar que los clones se eliminaran en otro sitio.

	for (itVcClPreviousCheckTable it1=m_oTables.begin (); it1!=m_oTables.end(); it1++) {
		if ((*it1) != NULL) {
			delete (*it1);
		}
	}
	for (itVcClPreviousCheckRelation it2=m_oRelations.begin (); it2!=m_oRelations.end(); it2++) {
		if ((*it2) != NULL) {
			delete (*it2);
		}
	}	
}


m4return_t ClPreviousCheckRelationsGroup::AddRelation(ClMetaRelat * ai_pMetaRel)
{
	//Puntero auxiliar.
	ClPreviousCheckTable * pTable1;
	
	//Buscamos la tabla al lado izquierdo de la MetaRelacion.
	ClPreviousCheckTable * tmp = FindTable(ai_pMetaRel, M4_TRUE);
	
	//El puntero auxiliar apuntara al encontrado o si no lo encuentra apuntará al nuevo.
	if (tmp != NULL) {		
		pTable1 = tmp;
	}
	else {
		//No lo hemos encontrado, lo creamos.
		//Al pasar M4_TRUE estamos tomamos el alias izquierdo de la metarelacion
		//para crear el objeto Tabla.
		//MetaRelacion A.id = B.id
		pTable1 = new ClPreviousCheckTable (ai_pMetaRel,M4_TRUE);
		m_oTables.push_back(pTable1);
	}


	//Tomamos el alias derecho.
	ClPreviousCheckTable * pTable2;
	tmp = FindTable(ai_pMetaRel, M4_FALSE);
	if (tmp != NULL) {
		pTable2 = tmp;
	}
	else {
		pTable2 = new ClPreviousCheckTable (ai_pMetaRel, M4_FALSE);
		m_oTables.push_back(pTable2);
	}

	//Creamos la relacion de chequeo.		
	//Al pasar M4_TRUE significara que el operando izquierdo de la igualdad sera principal y
	//etiquetaremos la relacion como principal. En realidad lo que se hará es tomar la información
	//del operando izquierdo de la metarelación.
	//Recordamos que a partir de una MetaRelacion tendremos una relacion.
	//MetaRelacion:			A.id=B.id
	//Relacion: 			A.id=B.id
	//En la creacion de la relacion, tendremos un puntero a cada una de las tablas.
	ClPreviousCheckRelation * pRelChk = new ClPreviousCheckRelation(this, ai_pMetaRel, pTable1, pTable2);

	//Se inserta en la lista de relaciones.
	m_oRelations.push_back(pRelChk);

	return M4_SUCCESS;
}


//Se inserta en la lista de relaciones.
m4return_t ClPreviousCheckRelationsGroup::AddRelation(ClPreviousCheckRelation *ai_Relation)
{
	m_oRelations.push_back(ai_Relation);
	return M4_SUCCESS;
}


//Retorna un puntero a Tabla si lo encuentra o NULL si no lo encuentra.
//El segundo argumento nos indica si la comparación se realiza sobre el operando
//izquierdo de la metarelación o sobre el derecho.
//Se consideran iguales si los alias son iguales.
ClPreviousCheckTable * ClPreviousCheckRelationsGroup::FindTable(ClMetaRelat * ai_pMetaRel, m4bool_t ai_bIsLeftAlias)
{
	//Buscaremos un alias concreto.
	//El segundo argumento me indica si debo buscar a la izquierda de la MetaRelacion o la derecha.	
	ClPreviousCheckTable * pTable = NULL;
	for (itVcClPreviousCheckTable it=m_oTables.begin (); it!=m_oTables.end(); it++) {
		pTable = (*it);
		if (ai_bIsLeftAlias == M4_TRUE) {
			if ((pTable->m_indAlias == ai_pMetaRel->m_pMtField1->m_indAlias)) {
				return (*it);
			}
		}
		if (ai_bIsLeftAlias == M4_FALSE) {
			if ((pTable->m_indAlias == ai_pMetaRel->m_pMtField2->m_indAlias)) {
				return (*it);
			}
		}
	}
	return NULL;
}


//Necesitamos la lista ordenada según la función de ordenación.
//Si alguna de las relaciones es de seguridad entonces tendrá sentido el recorrido.
//Si no tenemos seguridad no habrá reducción de joins. Si el API es incorrecto continuará
//dando error.
m4return_t ClPreviousCheckRelationsGroup::ReduceJoins()
{		
	ClPreviousCheckRelation * tmp;
	
	//Ordenamos la lista.
	m_oRelations.sort(CompareTo);

	PromoteToOuterAndSetClones();
	PromoteToInner();
	GenerateTableRelations();
	
	for (itVcClPreviousCheckRelation it=m_oRelations.begin (); it!=m_oRelations.end(); it++) {
		tmp = (*it);
		//Si la relacion es de seguridad podemos iniciar recursion.		
		if (tmp->m_bIsSecurity == M4_TRUE) {
			ReduceJoins(tmp->m_pBase);
		}
	}	
	
	//Se traspasa la informacion a los clones.
	PropagateToClones();

	return M4_SUCCESS;
}


//Dada la tabla inicial, se indica que es la origen y vamos a recorrer tablas para ir en busca
//de ciclos.
//Siempre se inicia en una tabla perteneciente a una relación de seguridad.
m4return_t ClPreviousCheckRelationsGroup::ReduceJoins(ClPreviousCheckTable * ai_pTable)
{
	ResetRelations();
	ai_pTable->m_bIsOrigin = M4_TRUE;
	FindAndReduce(ai_pTable, M4_FALSE);
	ai_pTable->m_bIsOrigin = M4_FALSE;

	return M4_SUCCESS;
}

//Este miembro se invocará para iniciar la recursion. Marcamos todas las relaciones como no
//visitadas.
m4return_t ClPreviousCheckRelationsGroup::ResetRelations()
{
	ClPreviousCheckRelation * tmp;

	for (itVcClPreviousCheckRelation it=m_oRelations.begin (); it!=m_oRelations.end(); it++) {
		tmp = (*it);
		tmp->ResetVisited();
	}
	
	return M4_SUCCESS;
}


//Si en el recorrido encontramos la tabla que origino el ciclo y es inner --> pondremos inner.
//Es recursion.
//Recordar que esta función se inicio por una tabla perteneciente a relación de seguridad!!!.
//Ejemplo:
//i = inner, o= outer, 0 = api, 1 = seg.
// 0 a 1 a o 0
// 1 a 2 a o 0
// 2 a 3 a o 0
// 3 a 4 a o 0
// 4 a 5 a o 0
// 5 a 6 a o 0
// 6 a 7 a o 0
// 2 a 6 a o 0
// 1 a 7 a o 0
// 0 a 3 a i 1
// 3 a 7 a i 1
//Obtendremos:
// 0 --> (0,3) - (1,0)
// 1 --> (1,0) - (2,1)
// 2 --> (2,1) - (3,2)
// 3 --> (0,3) - (3,2) - (3,7) - (4,3)
// 4 --> (4,3) - (5,4)
// 5 --> (5,4) - (5,6)
// 6 --> (5,6) - (7,6)
// 7 --> (3,7) - (7,6)
m4bool_t ClPreviousCheckRelationsGroup::FindAndReduce(PClPreviousCheckTable ai_pTable, m4bool_t ai_bInner) {

	ClPreviousCheckRelation * tmp;
	m4bool_t bIsReduced = M4_FALSE;
	ClPreviousCheckTable * pTable;

	for (itVcClPreviousCheckRelation it=ai_pTable->m_oRelations.begin (); it!=ai_pTable->m_oRelations.end(); it++) {
		tmp = (*it);		
		if (tmp->m_bIsVisited == M4_FALSE) {
			tmp->SetVisited(M4_TRUE);

			if (ai_bInner == M4_FALSE) {
				ai_bInner = !(tmp->m_bIsOuter);
			}

			pTable = tmp->GetNextTable(ai_pTable);
			if ((pTable->m_bIsOrigin && ai_bInner) || (FindAndReduce(pTable, ai_bInner))) {
				tmp->SetInner();
				bIsReduced = M4_TRUE;
			}
			//Con esta línea, permitimos que se pueda visitar una misma relación para dos
			//bucles distintos.
			tmp->SetVisited(M4_FALSE);
		}
	}

	return bIsReduced;
}


//Se cogen parejas de relaciones 1-2, 2-3,...
//!Se comparan a nivel de campo, es decir, dos relaciones son iguales si coinciden alias
//y campos!. Es importante el hecho de que el vector se ordena de una determinada forma.
//Si las relaciones son iguales y el tipo de join es diferente, se promueve a OJ.
//Si la relación se añade en los clones, por lo tanto la eliminamos de la otra lista.
//Se añade en los clones cuando son iguales en la comparacion de campos y el tipo de join
//es igual, entonces tenemos un clone. Si además uno viene de seguridad y el otro no
//en clones ponemos que es de seguridad.
m4return_t ClPreviousCheckRelationsGroup::PromoteToOuterAndSetClones(void)
{	
	itVcClPreviousCheckRelation itEnd = m_oRelations.end ();
	itVcClPreviousCheckRelation itOldRelation = m_oRelations.begin();
	itVcClPreviousCheckRelation itCurrent = itOldRelation;
	
	itCurrent++;
	for (;itCurrent != itEnd; ++itCurrent)
	{
		if ((*itCurrent)->CompareToField(*itOldRelation) == M4_FALSE)
		{
			if ((*itCurrent)->m_bIsOuter != (*itOldRelation)->m_bIsOuter)
			{
				(*itCurrent)->SetOuter(M4_TRUE);
				(*itOldRelation)->SetOuter(M4_TRUE);
			}
			else
			{
				(*itOldRelation)->AddClone(*itCurrent);
				if ((*itCurrent)->m_bIsSecurity != (*itOldRelation)->m_bIsSecurity)
				{
					(*itOldRelation)->SetSecurity();
				}

				//Se elimina de la lista y nos situamos en el anterior.
				//Eliminar significa que ha desaparecido de la lista y compactar la lista.
				m_oRelations.erase(itCurrent);
				itCurrent = itOldRelation;
				continue;
			}
		}
		itOldRelation = itCurrent;
	}	
	return M4_SUCCESS;
}


//Se toman parejas 1-2, 2-3, ...
//Despues de la promoción a outer, la lista se ha modificado, se han eliminado entradas
//(insertándolas en clones).
//La promocion a inner se realiza despues de la promocion a outer.
//En esta segunda iteracion se comprueba la igualdad de tablas (no en campos), se
//considerarán iguales si las tablas implicadas en ambas relaciones son iguales.
//Si son iguales las tablas y el tipo de join es diferente lo pondremos a inner.
m4return_t ClPreviousCheckRelationsGroup::PromoteToInner()
{		

	itVcClPreviousCheckRelation itEnd = m_oRelations.end ();
	itVcClPreviousCheckRelation itOldRelation = m_oRelations.begin();
	itVcClPreviousCheckRelation itCurrent = itOldRelation;
	
	itCurrent++;
	for (;itCurrent != itEnd; ++itCurrent)		
	{				
		if ((*itCurrent)->CompareToTable(*itOldRelation) == M4_FALSE)
		{
			if ((*itCurrent)->m_bIsOuter != (*itOldRelation)->m_bIsOuter)
			{
				(*itCurrent)->SetInner();
			}
		}		
		itOldRelation = itCurrent;
	}	
	return M4_SUCCESS;
}


//El proceso de generacion de relaciones consiste en dada una relacion, a su objeto base
//se le añade la relacion.
//Es decir, el objeto base/next tendrán punteros a las relaciones. De esta forma,
//cada objeto sabe en qué relaciones está.
m4return_t ClPreviousCheckRelationsGroup::GenerateTableRelations()
{
	ClPreviousCheckRelation * tmp;
	for (itVcClPreviousCheckRelation it=m_oRelations.begin (); it!=m_oRelations.end(); it++) {
		tmp = (*it);
		tmp->GetBase()->AddRelation(tmp);
		tmp->GetNext()->AddRelation(tmp);
	}
	return M4_SUCCESS;
}

//Es el último paso en la reducción. La informacion se pasa a las relaciones que tengamos
//en clones.
m4return_t ClPreviousCheckRelationsGroup::PropagateToClones(void)
{
	ClPreviousCheckRelation * pBase;

	for (itVcClPreviousCheckRelation it=m_oRelations.begin (); it!=m_oRelations.end(); it++) {		
		pBase = (*it);	
		//Recorre los clones para la reclación inicial pBase.
		for (itVcClPreviousCheckRelation it2 = pBase->m_oClones.begin (); it2!=pBase->m_oClones.end(); it2++) {
			(*it2)->SetOuter(pBase->m_bIsOuter);
		}		
	}	
	return M4_SUCCESS;
}
	

//***************** Clase Tabla *************************************//
ClPreviousCheckTable::ClPreviousCheckTable(ClMetaRelat * ai_pMetaRel, m4bool_t ai_bIsMainLeft)
{	
	//No se crea porque es una lista.
	//m_oRelations.reserve(8);
	
	//La metarelacion no se libera aquí!!.
	m_pMetaRel = ai_pMetaRel;
	//En función de left tomaremos la informacion del primero o del segundo operando de la
	//metarelacion.
	if (ai_bIsMainLeft == M4_TRUE) {
		m_indAlias = ai_pMetaRel->m_pMtField1->m_indAlias;		
	}
	else {
		m_indAlias = ai_pMetaRel->m_pMtField2->m_indAlias;		
	}
	m_bIsOrigin = M4_FALSE;
}

//La destruccion del objeto supone la destrucción del vector y este llama a los destructores
//de sus elementos (relaciones). Pero esta destrucción se realiza en el objeto grupo.
//Aqui pondremos a NULL el elemento.
ClPreviousCheckTable::~ClPreviousCheckTable()
{
	for (itVcClPreviousCheckRelation it=m_oRelations.begin (); it!=m_oRelations.end(); it++) {
		(*it) = NULL;
	}
	m_pMetaRel = NULL;	
}


//Añadimos la relacion.
m4return_t ClPreviousCheckTable::AddRelation(ClPreviousCheckRelation * ai_pRelation)
{
	m_oRelations.push_back(ai_pRelation);
	//Mantenemos la lista siempre ordenada.
	m_oRelations.sort(CompareTo);
	return M4_SUCCESS;
}


//***************** Clase Relacion *************************************//
ClPreviousCheckRelation::ClPreviousCheckRelation(ClPreviousCheckRelationsGroup * ai_pRelationsGroup,
												 ClMetaRelat * ai_pMetaRel,
												 ClPreviousCheckTable * ai_pBase,
												 ClPreviousCheckTable * ai_pNext)
{

	//Este es el grupo donde el objeto tabla añade.
	m_pRelationsGroup = ai_pRelationsGroup;
	//Tendremos la asociación de la Metarelación con la relacion de chequeo. 
	m_pMetaRel = ai_pMetaRel;		
	
	//Base es la tabla inicial (izquierda).
	m_pBase = ai_pBase;
	//Base es la tabla final (derecha).
	m_pNext = ai_pNext;
	
	m_indAliasBase = ai_pMetaRel->m_pMtField1->m_indAlias;
	m_stFieldBase = ai_pMetaRel->m_pMtField1->m_stField;
	m_indAliasNext = ai_pMetaRel->m_pMtField2->m_indAlias;
	m_stFieldNext = ai_pMetaRel->m_pMtField2->m_stField;
		
	//Comprueba si es API o es de seguridad.
	//M4LDB_APISQL_FROM_APISQL		= 0;	
	//M4LDB_APISQL_FROM_SECURITY	= 1;
	m_bIsSecurity = (ai_pMetaRel->GetAPISQLSource() == M4LDB_APISQL_FROM_APISQL) ? M4_FALSE : M4_TRUE;
	m_bIsOuter = (ai_pMetaRel->m_eJoinType == M4LDB_OUTER_JOIN) ? M4_TRUE : M4_FALSE;	
	m_bIsVisited = M4_FALSE;
	//Se ha convertido en lista, no se hace reserve.
	//m_oClones.reserve(8);
}


//Liberamos los punteros que contiene la lista de clones.
ClPreviousCheckRelation::~ClPreviousCheckRelation()
{	
	for (itVcClPreviousCheckRelation it=m_oClones.begin (); it!=m_oClones.end(); it++) {
		if ((*it) != NULL) {
			delete (*it);
		}
	}	
	m_pRelationsGroup = NULL;
	m_pBase = NULL;
	m_pNext = NULL;
}


//Obtenemos el puntero a la next de una relación.
//Se utiliza cuando estamos en la recursión.
ClPreviousCheckTable * ClPreviousCheckRelation::GetNextTable(ClPreviousCheckTable * ai_pTable)
{
	if (m_pBase == ai_pTable) {
		return m_pNext;
	}
	else {
		return m_pBase;
	}
}


//Dada una relacion, tenemos su base y su next, con esta comparacion estamos detectando que
//dos relaciones son iguales si tablas y campos base y next coinciden con tablas y campos base
//de la relacion argumento.
m4bool_t ClPreviousCheckRelation::CompareToField(ClPreviousCheckRelation * ai_pRelation)
{
	if ((m_pBase->m_indAlias == ai_pRelation->GetBase()->m_indAlias) &&
		(m_pNext->m_indAlias == ai_pRelation->GetNext()->m_indAlias) &&
		(m_stFieldBase == ai_pRelation->m_stFieldBase) &&
		(m_stFieldNext == ai_pRelation->m_stFieldNext) )
	{
		return M4_FALSE;
	}
	return M4_TRUE;
}


//Dos tablas serán iguales si el base, next coinciden con el base y next del argumento.
m4bool_t ClPreviousCheckRelation::CompareToTable(ClPreviousCheckRelation * ai_pRelation)
{
	if ((m_pBase->m_indAlias == ai_pRelation->GetBase()->m_indAlias) &&
		(m_pNext->m_indAlias == ai_pRelation->GetNext()->m_indAlias))
	{
		return M4_FALSE;
	}
	return M4_TRUE;
}


//En este caso la comparacion tiene en cuenta que la relacion coincide en alias con los dos argumentos
//tabla, uno por base y otro por next.
//Se chequea tanto a=b como b=a, se habría encontrado la relacion.
m4bool_t ClPreviousCheckRelation::CompareToTables(ClPreviousCheckTable * ai_pBase, ClPreviousCheckTable * ai_pNext)
{
	if (((m_pBase->m_indAlias == ai_pBase->m_indAlias) && (m_pNext->m_indAlias == ai_pNext->m_indAlias)) || 
		((m_pBase->m_indAlias == ai_pNext->m_indAlias) && (m_pNext->m_indAlias == ai_pBase->m_indAlias)))
	{
		return M4_FALSE;
	}
	return M4_TRUE;
}


//Dadas dos tablas, si encontramos la relacion la ponemos a inner.
m4return_t ClPreviousCheckRelation::SetInner(void)
{
	for (itVcClPreviousCheckRelation it=m_pRelationsGroup->m_oRelations.begin(); it!=m_pRelationsGroup->m_oRelations.end(); it++) {
		//Comparo la parte izquierda con Base
		//La parte derecha con Next.
		//Comparamos Alias, Fields	
		//Pueden haber varias relaciones A.id=B.id AND A.org=B.org
		if ((*it)->CompareToTables(m_pBase, m_pNext) == M4_FALSE) {
				(*it)->m_pMetaRel->m_eJoinType = M4LDB_NORMAL_JOIN;
				(*it)->m_bIsOuter =	M4_FALSE;
		}
	}
	return M4_SUCCESS;
}


//Estamos en una relación, nos comparamos con el resto de relaciones
//Si son iguales a nivel de tabla marcaremos como visitada la relación.
//Bug 0073288.
m4return_t ClPreviousCheckRelation::SetVisited(m4bool_t ai_bIsVisited)
{
	for (itVcClPreviousCheckRelation it=m_pRelationsGroup->m_oRelations.begin (); it!=m_pRelationsGroup->m_oRelations.end(); it++) {	
		if ((*it)->CompareToTables(m_pBase, m_pNext) == M4_FALSE) {				
			(*it)->m_bIsVisited = ai_bIsVisited;
		}
	}
	return M4_SUCCESS;
}


//*********
//Esta funcion sera invocada por el objeto lista para poder ordenar la lista de relaciones.
//Tiene dos argumentos que son el par de relaciones que se comparan.
bool CompareTo(ClPreviousCheckRelation * const &ai_pRelationOrig, ClPreviousCheckRelation * const &ai_pRelationDest)
{
	//Si segundo operando es mayor que primero, devolvemos TRUE (1), en caso
	//contrario devolvemos FALSE (0).
	//La ordenación es de menor a mayor. Es decir, Si Destino > Origen devolvemos true,
	//de esta forma el orden de inserción será Origen, Destino.

	//Si iRes == -1 significa que Destino > Origen, por lo tanto el orden
	//en la inserción se quedaría: Origen, Destino.
	//Si iRes == 1 significa que Destino < Origen, por lo tanto el resultado
	//en la insercion se quedaría: Destino, Origen.

	//Es decir:
	//Si iRes == -1 devolveremos true porque el orden: Origen, Destino es correcto.
	//Si iRes == 1 devolveremos false porque el orden: Origen, Destino es incorrecto, es decir
	//el orden en que se deberá insertar en la lista es Destino, Origen
	int iRes;


	//<<<--
	//Explicación obsoleta (porque no se emplea iRes en el retorno):
	// a=b --> 0
	// a<b --> -1
	// a>b --> 1
	//Si son iguales es 0.
	//Si no son iguales, miramos si es menor el alias de m_pBase devuelvo -1.
	//Si el segundo es menor devuelvo 1
	//Por ejemplo:
	//(0-3) vs (3-7)	Devuelve -1
	//(7-1) vs (3-7)	Devuelve 1
	//(6-7) vs (6-2)	Pasa el primer if con 0 y el segundo devuelve 1	
	//>>>--

	iRes = (ai_pRelationOrig->GetBaseAlias() == ai_pRelationDest->GetBaseAlias())? 0: (ai_pRelationOrig->GetBaseAlias() < ai_pRelationDest->GetBaseAlias())? -1 : 1;
	//Si da -1 significa que Destino > Origen. Tengo que devolver true.
	//Si da 0 significa que son iguales, debo pasar a la siguiente comprobación.
	//Si da 1 significa que Destino <= Origen. Tengo que devolver false.	
	switch (iRes) {
		case 0: break;
		case 1: return false;
		case -1:return true;
	}

	iRes = (ai_pRelationOrig->GetNextAlias() == ai_pRelationDest->GetNextAlias())? 0: (ai_pRelationOrig->GetNextAlias() < ai_pRelationDest->GetNextAlias())? -1 : 1;
	switch (iRes) {
		case 0: break;
		case 1: return false;
		case -1:return true;
	}
	
	iRes = (ai_pRelationOrig->GetBaseField() == ai_pRelationDest->GetBaseField())? 0: (ai_pRelationOrig->GetBaseField() < ai_pRelationDest->GetBaseField())? -1 : 1;
	switch (iRes) {
		case 0: break;
		case 1: return false;
		case -1:return true;
	}
	
	iRes = (ai_pRelationOrig->GetNextField() == ai_pRelationDest->GetNextField())? 0: (ai_pRelationOrig->GetNextField() < ai_pRelationDest->GetNextField())? -1 : 1;
	switch (iRes) {
		case 0: break;
		case 1: return false;
		case -1:return true;
	}
	
	//Si son del mismo tipo de outer, devolvemos 0.
	//Si son de diferente tipo de join: si el base es outer devolvemos 1.
	//Si el que es outer es el segundo devolvemos -1.
	iRes = (ai_pRelationOrig->IsOuter() == ai_pRelationDest->IsOuter())? 0: (ai_pRelationOrig->IsOuter())? 1 : -1;
	switch (iRes) {
		case 0: break;
		case 1: return false;
		case -1:return true;
	}

	iRes = (ai_pRelationOrig->IsSecurity() == ai_pRelationDest->IsSecurity())? 0: (ai_pRelationOrig->IsSecurity())? 1 : -1;
	switch (iRes) {
		case 0: break;
		case 1: return false;
		case -1:return true;
	}
	
	//Si son identicos devolveremos false. Si no son identicos devolveremos bRes (true)
	//Si son iguales iRes = 0
	//Si la resta es negativa iRes = -1 por que Destino > Origen
	//Si la resta es positiva iRes = 0 por que Destino <= Origen
	iRes =  ((ai_pRelationOrig - ai_pRelationDest)) ? 0 : ((ai_pRelationOrig - ai_pRelationDest) < 0) ? -1 : 1;
	switch (iRes) {
		case 0: return false;
		case 1: return false;
		case -1:return true;
		default: return false;
	}
}
//*********

//--- Pedro
//**********************************************************



