//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb
// File:                apisql.cpp
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



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cllgadap.hpp"
#include "m4lgadapdef.hpp"

#include "clldb.hpp"
#include "apisql.hpp"
#include "cldinarr.hpp"
#include "clmtsent.hpp"
#include "conex.hpp"
#include "ldb_log.hpp"
#include "access.hpp"
#include "recorset.hpp"

#ifdef _M4LDB_DEBUG
	#include "clldb.hpp"
	
	#undef M4LDB_PRINT_DEBUG
	#define M4LDB_PRINT_DEBUG	if (m_pGlobVars->GetpLConn ()->m_lSysDebugLevel <= m_pGlobVars->GetpLConn ()->m_pLdb->GetSysDebugLevel ()) m_pGlobVars->GetpLConn ()->m_pInspector->PrintFileDebug

	#undef M4LDB_PRINT_DEBUG_DETAIL
	#define M4LDB_PRINT_DEBUG_DETAIL	if (m_pGlobVars->GetpLConn ()->m_lSysDebugLevel <= m_pGlobVars->GetpLConn ()->m_pLdb->GetSysDebugLevel ()) m_pGlobVars->GetpLConn ()->m_pInspector->PrintFileDebugDetail
#endif




m4bool_t str_compare (const char* a_, const char* b_)
{
	return (strcmp (a_, b_) < 0 ? M4_TRUE : M4_FALSE);
}

//Pedro Bug 0069267
//La función no está en ningún .h, se encuentra en clmtsent.cpp
//Porque dentro de este fichero se utiliza.
extern string GetStringM4LDB_MD_LENGTH_CLAUSE (m4int32_t ai_Level);
extern m4uint8_t M4LDB_MD_LENGTH_CLAUSE;
//fin Pedro

m4cint8_t *g_aSQLReservedWords[] = { "ALL","AND","ASC","BETWEEN","BY","CFROM","DELETE","DESC","DISTINCT","ESCAPE","EXISTS","FOR","FROM","GROUP","HAVING","IN","INSERT","INTO","IS","LIKE","M4_MINUS_INF","M4_PLUS_INF","MINUS_INF","NOT","NOWAIT","NULL","OF","OR","ORDER","PLUS_INF","SELECT","SET","UNION","UPDATE","VALUES","WHERE" };
m4cint32_t g_lSQLReservedWords = sizeof (g_aSQLReservedWords) / sizeof (g_aSQLReservedWords[0]);


m4bool_t IsKeyword (m4pcchar_t ai_pToken)
{
	if( !(*ai_pToken >= 'A' && *ai_pToken <= 'Z') )
	{
		// Un atajo. Si el primer carácter no es una letra, seguro que no es un keyword.
		return (M4_FALSE);
	}

	return (binary_search( g_aSQLReservedWords, g_aSQLReservedWords + g_lSQLReservedWords, ai_pToken, str_compare ))?M4_TRUE:M4_FALSE;
}



m4cint8_t *g_aKywAftFrom[] = { "FOR","GROUP","ORDER","SET","UNION","WHERE" };
m4cint32_t g_lKywAftFrom = sizeof (g_aKywAftFrom) / sizeof (g_aKywAftFrom[0]);


m4bool_t IsKywAftFrom (m4pchar_t ai_pToken)
{
	if( !( *ai_pToken >= 'A' && *ai_pToken <= 'Z') )
	{
		// Un atajo. Si el primer carácter no es una letra, seguro que no es un keyword.
		return (M4_FALSE);
	}
	
	return (binary_search( g_aKywAftFrom, g_aKywAftFrom + g_lKywAftFrom, ai_pToken, str_compare ))?M4_TRUE:M4_FALSE;
}


// Solo se utiliza en este .cpp
m4bool_t   IsControlCharacter(m4char_t ai_char)
{
	if (ai_char == 0x20 || (ai_char>=0x09 && ai_char<=0x0D) )
	{
		return M4_TRUE;
	}
	return M4_FALSE;
}


m4char_t g_aClauses[] = { "BCLFWROE" };


m4return_t _X_GetToken (m4pchar_t &ao_pStm, m4pchar_t ao_pToken, m4int32_t &ao_lenToken, m4char_t &ao_cSeparator, m4bool_t &ao_bIsWord, m4bool_t ai_bIsReal, m4int32_t *ao_pInvRelation)
{
	m4pchar_t pTrg = ao_pToken;
	m4bool_t bExit = M4_FALSE;
	m4bool_t bRealWord = M4_FALSE;
	m4bool_t bString = M4_FALSE;
	m4bool_t bIsSpace = M4_FALSE;
	*ao_pToken = '\0';
	ao_cSeparator = '\0';
	ao_lenToken = 0;
	ao_bIsWord = M4_TRUE;

	while( *ao_pStm && !bExit )
	{
		bIsSpace = M4_FALSE;

		if (*ao_pStm == '`')
		{
			bRealWord = M4_BOOL(!bRealWord);
			ao_bIsWord = M4_FALSE;
		}

		if (!bRealWord)
		{
			if (*ao_pStm == '\'' && *(ao_pStm-1) != '\\')
			{
				bString = M4_BOOL(!bString);
				ao_bIsWord = M4_FALSE;
			}

			if (!bString)
			{
				// No forma parte de una cadena.
				if (ao_pInvRelation)
				{
					m4char_t car = *ao_pStm - 128;
					
					if (car == '(' || car == 'E')
					{ // WHERE
						*ao_pInvRelation = *ao_pInvRelation + 1;
						*ao_pStm = car;
					}
					else
					{
						if ((car == ';') || 
							(car == ')') || 
							(car == 'F') || // FOR
							(car == 'G') || // GROUP
							(car == 'O') || // ORDER
							(car == 'U'))   // UNION
						{
							*ao_pInvRelation = *ao_pInvRelation - 1;
							*ao_pStm = car;
							if (*ao_pStm == '\0')
							{
								continue;
							}
						}
					}
				}

				if ((*ao_pStm != '_') && 
					(*ao_pStm != '.') && 
					(*ao_pStm != '\'') && 
					(*ao_pStm != '@') &&	// Item
					(*ao_pStm != '$') &&	// Session Item
					(*ao_pStm != '&') &&	// Base Object
					(*ao_pStm != '?') &&	// External Parameter
					(*ao_pStm != '#') &&	// Logical Function
//					(*ao_pStm != '|') &&	// Clause Element
					(*ao_pStm != '^') &&	// Internal Alias
					(*ao_pStm != '~') &&	// Logical Element
					(((*ao_pStm >= 0x21) &&  
					(*ao_pStm <= 0x2F)) ||
					(*ao_pStm == '¿') ||	//XJoin. El simbolo '¿' es un separador. 					
					((*ao_pStm >= 0x3A) &&  
					(*ao_pStm <= 0x40)) || 
					((*ao_pStm >= 0x5B) &&  
					(*ao_pStm <= 0x60)) || 
					((*ao_pStm >= 0x7B) &&  
					(*ao_pStm <= 0x7E))))
				{
					// Es un separador.

					ao_cSeparator = *ao_pStm;
					bExit = M4_TRUE;
				}
				else
				{
					if (IsControlCharacter(*ao_pStm))
					{  //HT, LF, VT, FF, CR
						// Es un espacio.
						bIsSpace = M4_TRUE;
						if (pTrg != ao_pToken)
						{
							// Hay algo escrito y queremos que el espacio sea separador.
							ao_cSeparator = ' ';
							bExit = M4_TRUE;
						}
					}
					else
					{
						if (*ao_pStm >= 0x20 && *ao_pStm <= 0x7E)
						{
							// Es imprimible.

							if (!ai_bIsReal && *ao_pStm >= 'a' && *ao_pStm <= 'z')
							{
								// Convertimos a mayusculas.
								*ao_pStm = *ao_pStm - 32;
							}

							*pTrg = *ao_pStm;
							pTrg++;
						}
					}
				}
				if ((ao_bIsWord) && 
					(!bExit) && 
					(!bIsSpace) && 
					(*ao_pStm != '_') && 
					(!((*ao_pStm >= 'A') && 
					(*ao_pStm <= 'Z'))) && 
					((!((*ao_pStm >= '0') && 
					(*ao_pStm <= '9'))) || 
					(pTrg == ao_pToken + 1)))
				{
					// No es una palabra.
					ao_bIsWord = M4_FALSE;
				}
			}
			else
			{
				*pTrg = *ao_pStm;
				pTrg++;
			}
		}
		else
		{
			*pTrg = *ao_pStm;
			pTrg++;
		}

		ao_pStm++;
	}

//	if (!bExit) {
//RRRR<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// En caso de que salga del bucle por no cumplir la primera condicion nos vamos fuera del array si seguimos leyendo.
	if (!bExit && (*ao_pStm))
	{
//RRRR>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		ao_pStm++;
	}

	if (pTrg != ao_pToken)
	{
		// Hemos copiado algo.
		*pTrg = '\0';
		ao_lenToken = pTrg - ao_pToken;
		return (M4_SUCCESS);
	}

	return (M4_ERROR);
}

void RemoveOcurrencesOfR (string *ai_pst)
{
	if (ai_pst==0)
	{
		return;
	}

	m4bool_t bRepeat;
	m4int32_t lPos;

	do
	{
		bRepeat = M4_FALSE;

		lPos = ai_pst->find ("(~R)");

		if (lPos != M4_STR_NPOS)
		{
			bRepeat = M4_TRUE;
			ai_pst->replace (lPos, 4, "~R");
		}

		lPos = ai_pst->find (" AND~R");

		if (lPos != M4_STR_NPOS)
		{
			bRepeat = M4_TRUE;
			ai_pst->replace (lPos, 6, "");
		}

		lPos = ai_pst->find ("(~R AND");

		if (lPos != M4_STR_NPOS)
		{
			bRepeat = M4_TRUE;
			ai_pst->replace (lPos, 7, "(");
		}

		lPos = ai_pst->find (" AND ~R)");

		if (lPos != M4_STR_NPOS)
		{
			bRepeat = M4_TRUE;
			ai_pst->replace (lPos, 8, ")");
		}
	} while (bRepeat);
}



ClAPISQL::ClAPISQL (eApiSqlType_t ai_eApiSql)
{
	m_lFilter = 0;

	m_indFilterSelect = 0;

	m_pMtSent = NULL;

	m_pGlobVars = NULL;
	
	m_pCurrent = NULL;

	m_pAuxString = NULL;

	m_pvcAuxColItm = NULL;

	m_pvcAuxColFld = NULL;

	m_pvcAuxColPar = NULL;

	m_pstAuxCol = NULL;

	m_pvcAuxColLngItm = NULL;

	m_pvcAuxColLngFld = NULL;

	m_pvcAuxColLngPar = NULL;

	m_pstAuxColLng = NULL;
	
	m_pvcAuxWhrFld = NULL;

	m_pvcAuxWhrPar = NULL;

	m_pstAuxWhr = NULL;

	m_pvcAuxFrmRel = NULL;

	m_pvcAuxFrmObj = NULL;

	m_eApiSql = ai_eApiSql;

	//iInicializamos para que se considere APISQL (no es de seguridad)
	m_iAPISQLSource = M4LDB_APISQL_FROM_APISQL;
	m_bIsSecurityView = M4_FALSE;

	//Bug 0082678, 0082953
	m_bLeftOperand = M4_TRUE;	
	m_bLeftOperandIsMultiOrganization = M4_FALSE;
	m_bRightOperandIsMultiOrganization = M4_FALSE;
	m_bAddedField = M4_FALSE;
	m_bAddedParam = M4_FALSE;

	switch (m_eApiSql)
	{
		case M4LDB_APISQL_SELECT:
			m_pvcAuxColFld = new VcClMtField;
			m_pvcAuxColLngFld = new VcClMtField;
			m_pvcAuxWhrFld = new VcClMtField;

			m_pvcAuxFrmRel = new VcClMtRelat;
			m_pvcAuxFrmObj = new VcClMtObject;
			
		case M4LDB_APISQL_REAL_SELECT:
			m_pvcAuxColItm = new VcClMtColumn;
			m_pvcAuxColPar = new VcClMtParam;
			m_pstAuxCol = new string;

			m_pvcAuxColLngItm = new VcClMtColumn;
			m_pvcAuxColLngPar = new VcClMtParam;
			m_pstAuxColLng = new string;
			
			m_pvcAuxWhrPar = new VcClMtParam;
			m_pstAuxWhr = new string;
			break;

		case M4LDB_APISQL_INSERT:
		case M4LDB_APISQL_UPDATE:
			m_pvcAuxColFld = new VcClMtField;
			m_pvcAuxColPar = new VcClMtParam;

		case M4LDB_APISQL_DELETE:
			m_pvcAuxFrmRel = new VcClMtRelat;
			m_pvcAuxFrmObj = new VcClMtObject;
			
		case M4LDB_APISQL_REAL_STMT:
			m_pvcAuxWhrFld = new VcClMtField;
			m_pvcAuxWhrPar = new VcClMtParam;
			m_pstAuxWhr = new string;
			break;
	}

	m_pvcAuxItm = NULL;
	
	m_pvcAuxFld = NULL;
	
	m_pvcAuxPar = NULL;
	
	m_pstAux = NULL;
	
	m_eClause = M4LDB_CLAUSE_BEGIN_STATEM;

	m_pstAPISQLStm = NULL;

	m_lMaxLevel = 0;

	m_lLevel = 0;

	m_lInitLevel = 0;

	m_bConnActivate = M4_FALSE;

	m_indAlsBasis = M4LDB_NULL_INDEX;

	m_indAlsSec = M4LDB_NULL_INDEX;

	m_bNoAddDtLastUpdate = M4_FALSE;

	//Variables contadores de parentesis.
	m_iopen = 0;

	m_iclose = 0;
}


ClAPISQL::~ClAPISQL (void)
{
	if (m_pAuxString)
	{
		delete m_pAuxString;
	}
	m_pAuxString = NULL;

	if (m_pvcAuxColItm)
	{
		for (itVcClMtColumn it=m_pvcAuxColItm->begin (); it!=m_pvcAuxColItm->end(); it++)
		{
			delete (*it);
		}
		delete m_pvcAuxColItm;
	}
	m_pvcAuxColItm = NULL;

	if (m_pvcAuxColFld)
	{
		for (itVcClMtField it=m_pvcAuxColFld->begin (); it!=m_pvcAuxColFld->end(); it++)
		{
			delete (*it);
		}
		delete m_pvcAuxColFld;
	}
	m_pvcAuxColFld = NULL;

	if (m_pvcAuxColPar)
	{
		for (itVcClMtParam it=m_pvcAuxColPar->begin (); it!=m_pvcAuxColPar->end(); it++)
		{
			delete (*it);
		}
		delete m_pvcAuxColPar;
	}
	m_pvcAuxColPar = NULL;

	if (m_pstAuxCol)
	{
		delete m_pstAuxCol;
	}
	m_pstAuxCol = NULL;

	if (m_pvcAuxColLngItm)
	{
		for (itVcClMtColumn it=m_pvcAuxColLngItm->begin (); it!=m_pvcAuxColLngItm->end(); it++)
		{
			delete (*it);
		}
		delete m_pvcAuxColLngItm;
	}
	m_pvcAuxColLngItm = NULL;

	if (m_pvcAuxColLngFld)
	{
		for (itVcClMtField it=m_pvcAuxColLngFld->begin (); it!=m_pvcAuxColLngFld->end(); it++)
		{
			delete (*it);
		}
		delete m_pvcAuxColLngFld;
	}
	m_pvcAuxColLngFld = NULL;

	if (m_pvcAuxColLngPar)
	{
		for (itVcClMtParam it=m_pvcAuxColLngPar->begin (); it!=m_pvcAuxColLngPar->end(); it++)
		{
			delete (*it);
		}
		delete m_pvcAuxColLngPar;
	}
	m_pvcAuxColLngPar = NULL;

	if (m_pstAuxColLng)
	{
		delete m_pstAuxColLng;
	}
	m_pstAuxColLng = NULL;

	if (m_pvcAuxWhrFld)
	{
		for (itVcClMtField it=m_pvcAuxWhrFld->begin (); it!=m_pvcAuxWhrFld->end(); it++)
		{
			delete (*it);
		}
		delete m_pvcAuxWhrFld;
	}
	m_pvcAuxWhrFld = NULL;

	if (m_pvcAuxWhrPar)
	{
		for (itVcClMtParam it=m_pvcAuxWhrPar->begin (); it!=m_pvcAuxWhrPar->end(); it++)
		{
			delete (*it);
		}
		delete m_pvcAuxWhrPar;
	}
	m_pvcAuxWhrPar = NULL;

	if (m_pstAuxWhr)
	{
		delete m_pstAuxWhr;
	}
	m_pstAuxWhr = NULL;

	if (m_pvcAuxFrmRel)
	{
		for (itVcClMtRelat it=m_pvcAuxFrmRel->begin (); it!=m_pvcAuxFrmRel->end(); it++)
		{
			delete (*it);
		}
		delete m_pvcAuxFrmRel;
		m_pvcAuxFrmRel = NULL;
	}

	if (m_pvcAuxFrmObj)
	{
		for (itVcClMtObject it=m_pvcAuxFrmObj->begin (); it!=m_pvcAuxFrmObj->end(); it++)
		{
			delete (*it);
		}
		delete m_pvcAuxFrmObj;
	}
	m_pvcAuxFrmObj = NULL;
}


void ClAPISQL::ResetFlagsMultiOrganization (void) 
{
	m_bLeftOperand						= M4_TRUE;
	m_bLeftOperandIsMultiOrganization	= M4_FALSE;
	m_bRightOperandIsMultiOrganization	= M4_FALSE;
	m_bAddedField						= M4_FALSE;
	m_bAddedParam						= M4_FALSE;
}


void ClAPISQL::AddSeparators (eClauseType_t ai_eClause, m4int32_t ai_lLevel)
{
	if (m_pvcAuxWhrFld)
	{
		ClMetaField_Sep *pMtFldSep = new ClMetaField_Sep (ai_lLevel, ai_eClause);

		// Esto se necesita para la comprobación de alias en anidamientos.
		pMtFldSep->m_iFilter = m_lFilter;

		m_pvcAuxFld = m_pvcAuxWhrFld;

		m_pvcAuxFld->push_back (pMtFldSep);
	}

	ClMetaParam_Sep *pMtParSep = new ClMetaParam_Sep (ai_lLevel, ai_eClause);

	m_pvcAuxPar = m_pvcAuxWhrPar;

	m_pvcAuxPar->push_back (pMtParSep);

	m_pstAux = m_pstAuxWhr;

	if (m_pstAux)
	{
		m4char_t aAux[10];

		//Pedro
		//Bug 0069267. Caso genérico.
		sprintf (aAux, "|%c%s", g_aClauses[ai_eClause-1], GetStringM4LDB_MD_LENGTH_CLAUSE(ai_lLevel).c_str());		
		m_pstAux->append (aAux);
	}
	//Fin Pedro
}


void ClAPISQL::AddToOr (m4bool_t ai_bIsBegin)
{
	if (m_pMtSent->IsReal() || m_indAlsSec != M4LDB_NULL_INDEX)
	{
		return;
	}

	if (m_pvcAuxWhrFld)
	{
		ClMetaField_AddToOr *pMtFldAdd = new ClMetaField_AddToOr (m_lLevel, M4LDB_CLAUSE_WHERE, ai_bIsBegin);

		// Esto se necesita para la comprobación de alias en anidamientos.
		pMtFldAdd->m_iFilter = m_lFilter;

		m_pvcAuxWhrFld->push_back (pMtFldAdd);
	}

	if (ai_bIsBegin == M4_FALSE)
	{
		ClMetaParam_AddToOr *pMtParAdd = new ClMetaParam_AddToOr (m_lLevel, M4LDB_CLAUSE_WHERE, M4_FALSE);

		m_pvcAuxWhrPar->push_back (pMtParAdd);

		if (m_pstAuxWhr)
		{
			m_pstAuxWhr->append (" ~X)");
		}
	}
	else
	{
		if (m_pstAuxWhr)
		{
			m_pstAuxWhr->append ("(");
		}
	}
}


void ClAPISQL::SetAuxVectors (eClauseType_t ai_eClause, m4int32_t ai_lLevel, m4bool_t ai_bNoAdd)
{
	if (((m_eClause == M4LDB_CLAUSE_COLUMN) || 
		(m_eClause == M4LDB_CLAUSE_LONG_COLUMN)) && 
		(ai_eClause != M4LDB_CLAUSE_COLUMN) && 
		(ai_eClause != M4LDB_CLAUSE_LONG_COLUMN))
	{
		// Ya no estamos en las columnas por lo que ponemos el nivel/columna a 0.
		m_lLevel = 0;
	}
	
	if (ai_eClause == M4LDB_CLAUSE_COLUMN)
	{
		m_eClause = ai_eClause;
		m_pvcAuxItm = m_pvcAuxColItm;
		m_pvcAuxFld = m_pvcAuxColFld;
		m_pvcAuxPar = m_pvcAuxColPar;
		m_pstAux = m_pstAuxCol;
		return;
	}
	else
	{
		if (ai_eClause == M4LDB_CLAUSE_LONG_COLUMN)
		{
			m_eClause = ai_eClause;
			m_pvcAuxItm = m_pvcAuxColLngItm;
			m_pvcAuxFld = m_pvcAuxColLngFld;
			m_pvcAuxPar = m_pvcAuxColLngPar;
			m_pstAux = m_pstAuxColLng;
			return;
		}
	}

	if (ai_lLevel != M4LDB_NULL_INDEX)
	{
		m_lLevel = ai_lLevel;
		if (ai_eClause == M4LDB_CLAUSE_BEGIN_STATEM && !m_lLevel)
		{
			m_pvcAuxItm = NULL;
			m_pvcAuxFld = NULL;
			m_pvcAuxPar = NULL;
			m_pstAux = NULL;
			return;
		}
	}
	else
	{
		if (ai_eClause == M4LDB_CLAUSE_BEGIN_STATEM)
		{ 
			m_lMaxLevel++;
			m_lLevel = m_lMaxLevel;
		}
	}

	if (m_lLevel > m_lInitLevel && ai_eClause > M4LDB_CLAUSE_FROM)
	{
		eClauseType_t eAuxClause = (eClauseType_t) ((m4int32_t) m_eClause + 1);
		for (;eAuxClause < ai_eClause;)
		{
			SetAuxVectors (eAuxClause, M4LDB_NULL_INDEX, M4_TRUE);
			eAuxClause = (eClauseType_t) ((m4int32_t) eAuxClause + 1);
		}
	}

	m_eClause = ai_eClause;

	m_pvcAuxItm = NULL;

	if (m_eClause == M4LDB_CLAUSE_END_STATEM)
	{
		if (m_lLevel >= 0)
		{
			AddSeparators (m_eClause, m_lLevel);

			do
			{
				m_lLevel--;

				if (m_lLevel >= 0)
				{
					m4char_t aAux[10];

					//Pedro
					//Bug 0069267					
					sprintf (aAux, "%s", GetStringM4LDB_MD_LENGTH_CLAUSE(m_lLevel).c_str());

					// Buscamos alverres la última clausula de dicho nivel.
					m4pchar_t pCls = NULL;
					m4int32_t indCls = m_pstAuxWhr->size ();
					while (!pCls)
					{
						indCls = m_pstAuxWhr->rfind (aAux, indCls-1);

						if (indCls == M4_STR_NPOS)
						{
							// Vuelvo a intentarlo con otro nivel. Si no ha encontrado nada de este nivel, no podemos estar en él ¿no?.
							break;
//							return;
						}
						if ( (*m_pstAuxWhr)[indCls-2] == '|')
						{
							pCls = strchr (g_aClauses, (*m_pstAuxWhr)[indCls-1]);
							m_eClause = (eClauseType_t) (pCls - g_aClauses + 1);
						}
					}
				}
			} while ((m_eClause == M4LDB_CLAUSE_END_STATEM) && (m_lLevel >= 0));
		}
	}
	else
	{
		AddSeparators (m_eClause, m_lLevel);
	
		if (!ai_bNoAdd)
		{
			if (m_eClause == M4LDB_CLAUSE_WHERE)
			{
				m_pstAux->append (" AND");
			}
			else
			{
				if (m_eClause == M4LDB_CLAUSE_ORDER_BY)
				{
					m_pstAux->append (",");
				}
			}
		}
	}

	return;
}


m4bool_t ClAPISQL::FindOuterJoin (void)
{
	// Volvemos atras para situarnos en el separador;
	m4pchar_t pCurrent = m_pCurrent - 1;

	// Quitamos blancos.
	for (; ((!*pCurrent) || IsControlCharacter(*pCurrent)); )
	{
		pCurrent++;
	}

	if (pCurrent[0] == '(' && pCurrent[1] == '+' && pCurrent[2] == ')')
	{
		m_pCurrent = pCurrent + 3;
		// No se mete ahora. Lo mete el MetaSentence. Va asociado al campo.
		// Si es real si que se mete.
		if (m_pMtSent->IsReal())
		{
			m_pstAux->append ("(+)");
		}
		return (M4_TRUE);
	}

	return (M4_FALSE);
}


// Sólo cuenta los parametros .
m4return_t ClAPISQL::DescribeFilterFunct (const string &ai_stFilterFunct,
                              m4int32_t ai_iBeginPos, 
                              m4int32_t &ao_iNumPar, 
                              m4int32_t &ao_iSizeFunct)
{
	m4uint32_t iPos	= ai_iBeginPos;
	m4int32_t iAbiertos=1;
	m4char_t Current=0;
	m4bool_t bParamFound = M4_FALSE; // Nos indica si entre los parentesis hemos encontrado algun parametro.
	
	if (ai_iBeginPos == M4_STR_NPOS)
	{
		return (M4_ERROR);
	}
	
	ao_iNumPar = 0;

	// Nos quedamos posicionados despues del primer parentesis.

	iPos = ai_stFilterFunct.find ('(',iPos+1);
	if (iPos == M4_STR_NPOS)
	{
		return M4_ERROR;
	}

	//Si hay un parentesis de apertura es pq seguro que tenemos por lo menos un parámetro.	

	m4bool_t bInvertedCommasStarted = M4_FALSE;	
	while (iAbiertos != 0)
	{
		++iPos;
		
		if (iPos > ai_stFilterFunct.size())
		{
			return M4_ERROR;
		}

		Current = ai_stFilterFunct[iPos];
		if (Current == '\'')
		{
			bInvertedCommasStarted = (!bInvertedCommasStarted);	
		}

		if (iAbiertos == 1)
		{
			if ((Current == ',') && (bInvertedCommasStarted == M4_FALSE))
			{
				//Ojito, si nos ponen una coma y no había ningun parametro antes damos error.
				if (bParamFound == M4_FALSE)
				{
					return M4_ERROR;
				}
				ao_iNumPar++;
			}
		}

		//Si no había nada y hay un caracter que no es un separador entendemos que es un argumento.
		//Tampoco vale el caracter que da fin a argumentos.
		if ((ao_iNumPar == 0) && (Current != ' ') && (Current != '\t') && (Current != ')'))
		{
			bParamFound = M4_TRUE;
			ao_iNumPar++;
		}

		if (bInvertedCommasStarted == M4_FALSE)
		{
			//Tratamiento de parentesis.
			if (Current == '(')
			{
				++iAbiertos;
			}
			else
			{ 
				if (Current == ')')
				{
					--iAbiertos;
				}
			}
		}
	}
	ao_iSizeFunct = iPos - ai_iBeginPos + 1;

	return M4_SUCCESS;
}


// Cambiar el bucle de abajo para que sea esta la que busque la funcion
m4return_t ClAPISQL::DescribeParametersFilterFunct (  const string &ai_stFilterFunct,
                                          m4int32_t ai_iBeginPos,
                                          string *ao_pParameters)
{
	m4uint32_t iPar = 0, iPos = ai_iBeginPos;
	m4int32_t iAbiertos=0;
	m4char_t Current =0;

	if (ai_iBeginPos == M4_STR_NPOS)
	{
		return (M4_ERROR);
	}

	m4bool_t bInvertedCommasStarted = M4_FALSE;	
	ao_pParameters [iPar] = "";

	while (iPos < ai_stFilterFunct.size())
	{
		Current = ai_stFilterFunct[iPos];
		//Si estamos dentro de comillas no analizamos el caracter, lo enchufamos sin mas.
		if (bInvertedCommasStarted == M4_TRUE)
		{
			if (Current == '\'')
			{
				bInvertedCommasStarted = (!bInvertedCommasStarted);				
			}
			if (iAbiertos)
			{
				ao_pParameters [iPar] += Current;			
			}
			iPos++;
			continue;
		}
		
		switch (Current) 
		{
			case '\'':
				bInvertedCommasStarted = (!bInvertedCommasStarted);
				if (iAbiertos)
				{
					ao_pParameters [iPar] += '\'';
				}
				break;

			case '(':
				if (iAbiertos)
				{
					ao_pParameters [iPar] += '(';
				}
				iAbiertos++;
				break;

			case ')':
				iAbiertos--;
				if (!iAbiertos)
				{					
					if (ao_pParameters [iPar] == "")
					{						
						return (M4_SUCCESS);	//Parametro vacío.
					}
					else
					{
						return (M4_SUCCESS);	//Se ha acabado la funcion.
					}
				}
				else
				{
					if (iAbiertos < 0)
					{						
						return (M4_ERROR);		//Demasiados abiertos.
					}
					ao_pParameters [iPar] += ')';
				}
				break;

			case ',':
				//Este invento se hace pq puede ser que tengamos funciones anidadas, pero eso no significa
				//que tenga que incrementarse el número de parámetros.
				if (iAbiertos == 1)
				{				
					if (ao_pParameters [iPar] == "")
					{					
						return (M4_ERROR);		//Parametro vacío.
					}
					iPar++;
					ao_pParameters [iPar] = "";					
				}
				else
				{
					if (iAbiertos > 0)
					{
						ao_pParameters [iPar] += Current;
					}
				}
				break;				

			default:
				if (iAbiertos > 0)
				{
					ao_pParameters [iPar] += Current;
				}
				break;
		}
		iPos++;
	}

	//Not found
	return (M4_ERROR);
}


m4return_t ClAPISQL::ReplaceRealFunctParameter (string &ao_stRealFunct, m4int32_t ai_iPosPar, string ai_stPar)
{
	m4uint32_t iPosalnum = 0;
	m4int32_t iPos = -1;	// Empezamos en -1. ( La primera abusqueda se hace desde iPos+1.
	m4int32_t iLen = 2;	// longitud del identificador del parametro.

	//Bug 0093639
	//Buscamos #P y comprobaremos si despues viene un dígito o no.
	//Es importante que si llegan más parámetros que #P tenemos en la formula daremos un error pq no encontraremos
	//donde situar el argumento.
	iPos = ao_stRealFunct.find("#P", iPos + 1);
	if (iPos == M4_STR_NPOS)
	{
		return (M4_ERROR);
	}

	iPosalnum = iPos + iLen ;
	m4uint32_t LenRealFunc = ao_stRealFunct.size();
	
	// Contamos la longitud del idefntificador de parametro.( puede ser #P1,  incluso #P14)
	while (iPosalnum<LenRealFunc&&isdigit(ao_stRealFunct[iPosalnum++]))
		++iLen;

	// Si tenemos como identificador algo mas que #P, hay que sustituir todos de una vez.
	if (iLen>2)
	{
		m4char_t * siCurrentArg = new m4char_t [iLen + 1];
		sprintf (siCurrentArg, "#P%X", (m4int32_t) ai_iPosPar + 1);
		
		//Vamos a sustituir sólo el argumento que nos indican!.
		iPos = ao_stRealFunct.find(siCurrentArg, 0);

		while (iPos!=M4_STR_NPOS)
		{
			//Escenario: Pe tenemos que tenemos ao_stRealFunct = `DATEDIFF`(`DD`,#P2,#P1).
			//Cuando se invoca a la función, se busca un determinado argumento y se irán sustituyendo.
			//La primera vez se buscará #P1 y se sustituirá por el valor que se pasa como otro argumento.
			//	iPos = 20 (es la posicion P1#).
			//	iLen = 3 (es la longitud de #P1).

			ao_stRealFunct.replace(iPos,iLen,ai_stPar);

			iPos=ao_stRealFunct.find(siCurrentArg, iPos + 1);
		} 
		delete [] siCurrentArg;
		return M4_SUCCESS;
	}

	//Si estamos aqui es pq en la formula solo tenemos #P, y tendremos que sustituir en el orden de llegada de los
	//parametros.

	ao_stRealFunct.replace (iPos, iLen, ai_stPar);
	return (M4_SUCCESS);
}


m4return_t ClAPISQL::AnalizeFunction (m4pchar_t ai_pToken)
{
	m4char_t cSeparator;
	m4bool_t bIsWord;
	m4int32_t lenToken;
	m4return_t result;
	m4int32_t indDDBB = M4LDB_NULL_INDEX;
	m4int32_t indFunction = M4LDB_NULL_INDEX;
	m4char_t aFName[M4LDB_SIZE_LONG];
	strcpy (aFName, &ai_pToken[1]);

	// Buscamos donde empieza la función.
	m4int32_t iFFBeginPos = m_pstAPISQLStm->rfind ("#", m_pCurrent - m_pstAPISQLStm->c_str ()-1);	
	m4int32_t iFFNumArgs = 0;
	m4int32_t iFFSize = 0;	

	string *pFFArgs = NULL;
	
	// Nos situamos en el bloque de las funciones adecuadas.
	indDDBB = m_pGlobVars->GetpLConn ()->FindIndexById (M4LDB_SYS_DICT_DDBBS, m_pGlobVars->GetDDBBType ());
	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_DDBBS]->Current[indDDBB];

	// Buscar si existe funcion

	// UNICODE DB
	// En unicode hay que intentar ver si existe la versión 2 de la función
	if( g_iEncodingMode != M4LDB_ENCODING_ANSI )
	{
		strcat( aFName, "2" ) ;
		indFunction = m_pGlobVars->GetpLConn ()->FindIndexById (M4LDB_SYS_DICT_FUNCTIONS, aFName);
		aFName[ strlen( aFName ) - 1 ] = '\0' ;
	}
	
	if (indFunction == M4LDB_NULL_INDEX)
	{
		indFunction = m_pGlobVars->GetpLConn ()->FindIndexById (M4LDB_SYS_DICT_FUNCTIONS, aFName);

		if (indFunction == M4LDB_NULL_INDEX)
		{
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_LOGIC_FUNCTION, aFName << m_pGlobVars->GetDDBBType ());
			return (M4_ERROR);
		}
	}

	string stSentFunct (" ");

	// Se coge el nombre real de la funcion		
	stSentFunct.append (M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FUNCTIONS]->Register[indFunction].Item[M4LDB_ITEM_SYS_DICT_FUNCTIONS_FUNCTION_HEADER ].Value.GetWithOutSec ()));
			
	string stAux(M4LDB_EMPTY);
	m4pchar_t pCurrent = (m4pchar_t) stSentFunct.c_str ();

	// Buscamos los posibles nombres de funciones reales y las ponemos entre los caracteres '`' para que se reconozcan como reales.
	do
	{
		/* Bug 0141014
		La función es ya real, y no se debe pasar a mayúsculas.
		*/
		result = _X_GetToken (pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, M4_TRUE);

		if (result !=M4_ERROR)
		{
			if (bIsWord)
			{
				if (!strcmp (m_pAuxString, "MAX") || 
					!strcmp (m_pAuxString, "MIN") || 
					!strcmp (m_pAuxString, "COUNT") || 
					!strcmp (m_pAuxString, "SUM") || 
					!strcmp (m_pAuxString, "AVG"))
				{ 
					m_bNoAddDtLastUpdate = M4_TRUE;
				}
				stAux.append("`");
				stAux.append(m_pAuxString);
				stAux.append("`");
			}
			else
			{
				/* Bug 0141014
				Se debe soportar la notación usuario.función para SQLServer
				*/
				if( strchr( m_pAuxString, '.' ) != NULL )
				{
					stAux.append("`");
					stAux.append(m_pAuxString);
					stAux.append("`");
				}
				else
				{
					if( g_iEncodingMode != M4LDB_ENCODING_ANSI && m_pAuxString != NULL && *m_pAuxString == '\'' )
					{
						/* Bug 0270427
						A los literales del cuerpo de funciones no se les pone la N.
						Aquí se escapa.
						*/
						stAux.append("'´");
						stAux.append(m_pAuxString + 1);
					}
					else
					{
						stAux.append(m_pAuxString);
					}
				}
			}
		}
		if (cSeparator)
		{
			stAux.append(&cSeparator,1);
		}

	} while (cSeparator != '\0');

	stSentFunct = stAux;
	
	//Buscamos la función en el filtro.
	result = DescribeFilterFunct (*m_pstAPISQLStm, iFFBeginPos, iFFNumArgs, iFFSize);
	
	if (result == M4_ERROR)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_BAD_LOGIC_FUNCTION, aFName);
		return (M4_ERROR);
	}
	
	if (iFFNumArgs > 0)
	{
		pFFArgs = new string [iFFNumArgs];
		
		// Crear el espacio para almacenar los argumentos
		result = DescribeParametersFilterFunct (*m_pstAPISQLStm, iFFBeginPos, pFFArgs);
		
		if (result == M4_ERROR)
		{
			delete [] pFFArgs;
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_BAD_LOGIC_FUNCTION, aFName);
			return (M4_ERROR);
		}
		
		//el bucle se hace para los params del filtro, porque ahora asumimos que serán
		//iguales a los de la función. Más adelante, se recorrerá un nodo que nos casará ls pos
		// de los argumentos y nos dará args por defecto.
		m4int32_t indRealArg;
		for (indRealArg =0; indRealArg < iFFNumArgs; indRealArg++)
		{
			//Sustituimos el siguiente parámetro.
			//si en lugar de 0 pusieramos x sustituiríamos el x+1avo sig. parámetro.
			result = ReplaceRealFunctParameter (stSentFunct, indRealArg, pFFArgs [indRealArg]);
			if (result == M4_ERROR)
			{
				delete [] pFFArgs;
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_BAD_LOGIC_FUNCTION, aFName);
				return (M4_ERROR);
			}
		}
	}
	
	m_pstAPISQLStm->replace (iFFBeginPos, iFFSize, stSentFunct);

	// Añadimos el null terminado.
	m_pstAPISQLStm->append(&cSeparator,1);

	// Resituamos el current por si ha habido realocación.
	m_pCurrent = & ( (*m_pstAPISQLStm)[iFFBeginPos]);
	
	if (pFFArgs)
	{	
	   delete [] pFFArgs;
	}
	
	return (M4_SUCCESS);  
}


m4pcchar_t ClAPISQL::AnalizeParan (ClMetaParam *ai_poParam)
{
	/* Bug 0154113
	En SQLServer, los parámetros de tipo fecha se convierten a DATETIME
	si vienen seguidos por + ó -
	*/
	if( m_pGlobVars->GetDDBBType() == M4DBW_SQL_SERVER_DDBB )
	{
		m4int8_t iType = ai_poParam->GetM4Type( m_pGlobVars ) ;

		if( iType == M4CL_M4_TYPE_TIMESTAMP || iType == M4CL_M4_TYPE_DATE )
		{
			if( *m_pCurrent != '\0' )
			{
				// El separador puede ser el propio operador
				if( *(m_pCurrent - 1) == '+' || *(m_pCurrent - 1) == '-' )
				{
					return( " CAST(? AS DATETIME)" );
				}

				m4pcchar_t pccCurrent = m_pCurrent;

				while( *pccCurrent != '\0' && IsControlCharacter( *pccCurrent ) )
				{
					// Quitamos blancos.
					pccCurrent++;
				}

				if( *pccCurrent == '+' || *pccCurrent == '-' )
				{
					return( " CAST(? AS DATETIME)" );
				}
			}
		}
	}

	return( " ?" );
}


m4return_t ClAPISQL::AnalizeArgument (m4char_t ai_cSeparator)
{
	m4return_t result;
	m4char_t cSeparator = ai_cSeparator;
	m4bool_t bIsWord;
	m4int32_t lenToken;

	result = FindSeparator ('(');

	if (result == M4_ERROR)
	{
		return (M4_ERROR);
	}

	// Tipo M4.
	result = _X_GetToken (m_pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal ());

	result = FindSeparator (',');

	if (result == M4_ERROR)
	{
		return (M4_ERROR);
	}

	m4int8_t m4Type = M4LDB_M4_TYPE( (m4int8_t) atol (m_pAuxString) );
	
	// Prec.
	result = _X_GetToken (m_pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal ());

	result = FindSeparator (',');

	if (result == M4_ERROR)
	{
		return (M4_ERROR);
	}

	m4int32_t lPrec = atol (m_pAuxString);
	
	// Scale.
	result = _X_GetToken (m_pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal ());

	result = FindSeparator (')');

	if (result == M4_ERROR)
	{
		return (M4_ERROR);
	}

	m4int32_t lScale = atol (m_pAuxString);

	ClMetaExternPar *pMtPar = new ClMetaExternPar (m_lLevel, m_eClause, m4Type, lPrec, lScale);

	m_pvcAuxPar->push_back (pMtPar);

	if (m_pstAux)
	{
		m_pstAux->append( AnalizeParan(pMtPar) );
	}

	return (M4_SUCCESS);
}


m4return_t ClAPISQL::AnalizeFieldOrConst (m4pchar_t ai_pToken, m4bool_t *ai_pbIsFld)
{
	m4pchar_t pPoint = NULL;
	m4pchar_t pField = NULL;
	m4bool_t bIsFld = M4_FALSE;

	if (*ai_pToken != '\'' && !m_pMtSent->IsReal ())
	{
		pPoint = strchr (ai_pToken, '.');

		if (!pPoint)
		{
			pField = ai_pToken;
		}
		else
		{
			pField = pPoint + 1;
		}

		if (*pField >= 'A' && *pField <= 'Z')
		{
			// Posiblemente es un campo.
			bIsFld = M4_TRUE;
			
			if (ai_pbIsFld)
			{
				*ai_pbIsFld = M4_TRUE;
			}
		}
	}
	
	if (bIsFld)
	{
		m4int32_t indAls = M4_STR_NPOS;
		
		if (pField != ai_pToken)
		{
			*pPoint = '\0';

			m4int32_t lLevel = m_lLevel;
			if (m_eClause == M4LDB_CLAUSE_COLUMN || m_eClause == M4LDB_CLAUSE_LONG_COLUMN)
			{
				lLevel = 0;
			}

			indAls = _FindIdxAlias (ai_pToken, m_lFilter, lLevel, m_eClause);
		
			if (m_pMtSent->GetParam ())
			{
				//-------------------------------------------
				//Bug 0082678, 0082953
				//Escenario:				
				//Filtro heredado en seguridad, entre tabla mono y multi, se está añadiendo el join por Pk,
				//que incluye ID_ORGANIZATION, pero esto provoca un problema pq al final tenemos un join filtrando por
				//sociedad en una tabla mono y el valor de la sociedad es del registro que se está actualizando... en 
				//la tabla multi. Como resultado tenemos que puede ser que nunca se cumpla el filtro de seguridad.
				if ((m_pMtSent) && (indAls != M4LDB_NULL_INDEX) )
				{
					m4int32_t indObj = M4LDB_NULL_INDEX;
					ClMetaObject *pLgcTbl = NULL;
					ClMetaAlias	*pAlias = NULL;

					pAlias = m_pMtSent->GetAliasByPos(indAls);

					if (pAlias)
					{
						indObj = pAlias->m_indObj;

						if (indObj != M4LDB_NULL_INDEX)
						{
							pLgcTbl = GetLgcTblByPos(indObj);

							//Si el campo es de organización:
							if (pLgcTbl != NULL && pLgcTbl->m_stObject.c_str() != NULL && !strcmpi (pField, "ID_ORGANIZATION") )
							{
								//Si la tabla no es multisocietaria y el campo es el de organización, vamos a quitar el posible join con organizacion.
								//Mu feo, con el cableo, asumimos que ese es el id de un campo de organización. Pero este dato esta cableado en otros sitios.
								//Pero ya se sabe la criticidad!!.

								//En este punto todavía no se ha conseguido la sociedad del objeto, pero la obtenemos de forma temporal.
								//Tampoco tiene la posicion del objeto en el diccionario.
								m4int32_t iOrganizationType = M4LDB_UNDEF_ORGANIZATION;
								m4int32_t indLgcTbl = M4LDB_NULL_INDEX;

								//----
								//Obtenemos el id del objeto lógico current, debe ser el mismo que el que me indique pLgcTbl!!.
								m4id_t idLgcTblCurrent = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
								//----

								//-----------------------
								//Nos guardamos el current!.
								m4int32_t indLgcTblCurrent = m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Index;
								//-----------------------

								//Inicializamos al current, pero puede cambiar.
								indLgcTbl = indLgcTblCurrent;

								//Si el objeto es el de seguridad o ya estoy situado no cargo nada. Por lo 
								//tanto si no se cumple lo anterior cargamos la información del objeto lógico.
								if ( strcmpi (pLgcTbl->m_stObject.c_str(), M4LDB_SEC_LOGIC_OBJECT) 
									&& strcmpi (pLgcTbl->m_stObject.c_str(), idLgcTblCurrent) )
								{
									m4return_t result = m_pGlobVars->GetpLConn ()->LoadDictionaryChannel (pLgcTbl->m_stObject.c_str(), &indLgcTbl, M4_TRUE);
									if (result == M4_ERROR)
									{
										return M4_ERROR;
									}
								}

								//Debemos estar posicionados!.
								if (indLgcTbl == M4LDB_NULL_INDEX)
								{
									return M4_ERROR;
								}								

								iOrganizationType = M4LDB_NUMBER(m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Register[indLgcTbl].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_ORG_TYPE].Value.GetWithOutSec());

								/* Bug 0197391
								Se calcula si el campo es realmente de tipo organización
								Si no lo es no se hace la sustitución
								*/
								m4bool_t bIsOrganization = M4_FALSE ;
								m4uint32_t numRealTbl = m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Count() ;

								for( m4uint32_t i = 0 ; i < numRealTbl ; i++ )
								{
									m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[ i ] ;

									m4int32_t indOrgField = m_pGlobVars->GetpLConn()->FindIndexById( M4LDB_SYS_DICT_FIELDS, pField ) ;

									if( indOrgField != M4LDB_NULL_INDEX )
									{
										eFldType_t eFldType = (eFldType_t) M4LDB_NUMBER( m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indOrgField].Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_INTERNAL_FIELD].Value.GetWithOutSec() ) ;

										if( eFldType == M4LDB_ID_ORGANIZATION )
										{
											bIsOrganization = M4_TRUE ;
											break ;
										}
									}
								}

								//-----------------------
								//Restauramos el current.
								m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.MoveTo(indLgcTblCurrent);
								//-----------------------

								if( bIsOrganization == M4_TRUE )
								{
									//Escenarios:
									//1.- Mono.ORG = Mono.ORG
									//	No hay join, por lo tanto no hay problemas.
									//2.- Mono.ORG = Multi.ORG
									//	El primer operando se sustirtuirá por 1 y en el segundo operando que es de multi 
									//	sabre que en el primero puse un 1 y por lo tanto forzaré un 1 para que quede un 1 = 1.
									//3.- Multi.ORG = Multi.ORG
									//	No sustituyo nada.
									//4.- Multi.ORG = Mono.ORG
									//	Este caso no se dara pq la herencia no se produce de multi a mono y el join no puede
									//	venir ordenado de esta forma!!.

									m4bool_t bLeftOperand = m_bLeftOperand;
									m4bool_t bLeftOperandIsMultiOrganization = M4_FALSE;
									m4bool_t bRightOperandIsMultiOrganization = M4_FALSE;
									m4bool_t bAddedField = M4_FALSE;
									m4bool_t bAddedParam = M4_FALSE;

									m4bool_t bMakeAction =M4_FALSE;
																
									//Para solucionar el caso 4 que sí puede darse en el editor de sentencias!.
									//En este caso, el operando izquierdo no es 1 pero el tipo de organización es mono y
									//no tendríamos que meter el 1 en el operando derecho!!.
									//ES DECIR: Si el operando izquierdo es multi y el derecho es mono no tenemos que meter el 1 a la derecha.
									
									//------------------------------------------------------------------
									//Si estamos en el operando izquierdo y es multi, nos lo apuntamos.
									if (bLeftOperand == M4_TRUE && iOrganizationType == M4LDB_MULTI_ORGANIZATION)
									{
										m_bLeftOperandIsMultiOrganization = M4_TRUE; 
									}

									//Si estamos en el operando derecho y es multi nos lo apuntamos
									if (bLeftOperand == M4_FALSE && iOrganizationType == M4LDB_MULTI_ORGANIZATION)
									{
										m_bRightOperandIsMultiOrganization = M4_TRUE; 
									}

									//Establecemos las variables locales despues de poner valor a los miembros
									bLeftOperandIsMultiOrganization = m_bLeftOperandIsMultiOrganization;
									bRightOperandIsMultiOrganization = m_bRightOperandIsMultiOrganization; 
									bAddedField = m_bAddedField;
									bAddedParam = m_bAddedParam;

									//Los flags los reseteamos cuando estamos en el operando derecho.
									//En las varibles locales ya tengo la informacion util.
									if (bLeftOperand == M4_FALSE)
									{
										m_bLeftOperandIsMultiOrganization = M4_FALSE;
										m_bRightOperandIsMultiOrganization = M4_FALSE;
										m_bAddedField = M4_FALSE;
										m_bAddedParam = M4_FALSE;
									}
									//------------------------------------------------------------------

									//Invierto el operando.
									m_bLeftOperand = (m_bLeftOperand == M4_TRUE) ? M4_FALSE : M4_TRUE;

									//Ahora planteamos la casuística:
									//----------------------------------------------------------------
									//----------------------------------------------------------------
									//Mono-Mono, Mono-Multi: se metería 1 = 1.
									if ( ((bLeftOperandIsMultiOrganization == M4_FALSE) && (bRightOperandIsMultiOrganization == M4_FALSE)) 
										|| ((bLeftOperandIsMultiOrganization == M4_FALSE) && (bRightOperandIsMultiOrganization == M4_TRUE)) )
									{
											bMakeAction = M4_TRUE;
									}
									
									//---------------------------------------
									//Multi-Multi, todo sigue su curso!!.
									//No hacemos nada especial.

									//---------------------------------------
									//Multi-Mono.
									//Tendremos que deshacer todo lo que se hizo para el operando izquierdo!!.								
									if ( (bLeftOperandIsMultiOrganization == M4_TRUE) && (bRightOperandIsMultiOrganization == M4_FALSE) && (bLeftOperand == M4_FALSE) )
									{
											//Para el operando izquierdo hay dos posibles actuaciones que han podido ocurrir:
											//Caso 1:
											//	- Creación e inserción de un parámetro: Ver m_pvcAuxPar.
											//	- Se añade " ?". Ver m_pstAux.
											//Caso 2:
											//	- Creación e inserción de un campo: Ver m_pvcAuxFld.
											//	- Se añade " ~F". Ver m_pstAux.

											//Sustituiremos por 2 = 2. De esta forma podemos diferenciar el tipo de join que estoy sustituyendo.
											//-----------
											m4int32_t iPos = M4LDB_NULL_INDEX;
											if (bAddedParam == M4_TRUE)
											{
												iPos = m_pstAux->rfind (" ?");
												if (iPos != M4LDB_NULL_INDEX)
												{
													m_pstAux->erase (iPos, m_pstAux->size() - 1);
													m_pstAux->append (" 2 = 2 ");
													if (m_pvcAuxPar && m_pvcAuxPar->empty () == M4_FALSE)
													{
														ClMetaParam *pPar = (*m_pvcAuxPar)[m_pvcAuxPar->size() - 1];
														if (pPar)
														{
															delete pPar;
														}
														m_pvcAuxPar->pop_back ();
													}
												}
											}
											else
											{
												if (bAddedField == M4_TRUE)
												{
													iPos = m_pstAux->rfind (" ~F");
													if (iPos != M4LDB_NULL_INDEX)
													{
														m_pstAux->erase (iPos, m_pstAux->size() - 1);
														m_pstAux->append (" 2 = 2 ");
														if (m_pvcAuxFld && m_pvcAuxFld->empty () == M4_FALSE)
														{
															ClMetaField *pFld = (*m_pvcAuxFld)[m_pvcAuxFld->size() - 1];
															if (pFld)
															{
																delete pFld;
															}
															m_pvcAuxFld->pop_back ();
														}
													}
												}
											}
											//-----------

											if (ai_pbIsFld)
											{
												*ai_pbIsFld = M4_FALSE;
											}

											//Retornamos sin haber creado el campo!.
											return M4_SUCCESS;
									}
									//----------------------------------------------------------------
									//----------------------------------------------------------------

									//Acciones.
									//----------------------------------------------------------------
									if (bMakeAction == M4_TRUE)
									{
										if (ai_pbIsFld)
										{
											*ai_pbIsFld = M4_FALSE;
										}

										//Metemos un 1, al final el join se traducirá por 1 = 1
										if (m_pstAux)
										{
											m_pstAux->append (" 1 ");
										}
										//Retornamos sin haber creado el campo!.
										return M4_SUCCESS;
									}
									//----------------------------------------------------------------

									//Bug 0083359
									//Solo actualizamos flags si estamos en el operando izquierdo!!.
									//Si estamos en el operando derecho se han reseteado antes y no debo tocarlos!!.
									if (bLeftOperand == M4_TRUE)
									{
										//Si es la base, acabará añadiendose como parametro.
										if (indAls == m_indAlsBasis)
										{
											m_bAddedParam = M4_TRUE;
										}
										else
										{
											//Si no se añade parametro, se añadira campo.
											m_bAddedField = M4_TRUE;
										}
									}
								}
							}
						}
					}
				}
				//-------------------------------------------

				if (indAls == m_indAlsBasis)
				{	// Estamos en la base.
					if (ai_pbIsFld)
					{
						*ai_pbIsFld = M4_FALSE;
					}
					return (AnalizeInternalParam (pField));
				}
			}
		}
		
		ClMetaField *pMtFld = new ClMetaField (m_lLevel, m_eClause, pField, indAls, m_lFilter);

		m_pvcAuxFld->push_back (pMtFld);

		if (m_pstAux)
		{
			m_pstAux->append (" ~F");
		}
				
		return (M4_SUCCESS);
	}
	
	// Es algo que no se lo que es.
	if (m_pstAux)
	{
		m_pstAux->append (" ");
		m_pstAux->append (ai_pToken);
	}

	return (M4_SUCCESS);
}


m4return_t ClAPISQL::AnalizeItemCol (m4pchar_t ai_pToken, m4int32_t &ao_hItem)
{
	m4int32_t hItem = m_pGlobVars->GetItemHandleByIdItem (&ai_pToken[1], m_bConnActivate); // Así evitamos el '@'.

	if (!hItem)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_ITEM, &ai_pToken[1]);
		return (M4_ERROR);
	}
	ao_hItem = hItem;

	m4int8_t cType = m_pGlobVars->GetItemType (hItem);
	
	if (cType == M4CL_ITEM_TYPE_METHOD)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_ITEM_IS_METHOD, hItem);
		return (M4_ERROR);
	}


	m4int8_t cScope = m_pGlobVars->GetItemScope (hItem);

	//Bug 0076002.
	//Se gpa si es sentencia real y el item no es de registro.
	//Tanto si es como si no es Real devolveremos Warning.
	if (cScope != M4CL_ITEM_SCOPE_REGISTER)
	{
		// Aqui el warning significa que el item no es de tipo registro.
		return (M4_WARNING);
	}

	eClauseType_t eOldClause = m_eClause;

	m4int8_t m4Type = m_pGlobVars->GetItemM4Type (hItem);
			
	if (m4Type == M4CL_M4_TYPE_LONG || 
		m4Type == M4CL_M4_TYPE_UNI_LONG || 
		m4Type == M4CL_M4_TYPE_BLOB || 
		m4Type == M4CL_M4_TYPE_BINARY_STRING)
	{
		SetAuxVectors (M4LDB_CLAUSE_LONG_COLUMN);
	}
	else
	{
		SetAuxVectors (M4LDB_CLAUSE_COLUMN);
	}

	// Recuerda, aqui el nivel es la columna.
	m_lLevel = m_pvcAuxItm->size ();

	ClMetaItemCol *pMtItem = new ClMetaItemCol (m_lLevel, m_eClause, hItem);

	pMtItem->m_lPos = m_pstAux->size ();

	if (eOldClause == M4LDB_CLAUSE_COLUMN)
	{
		if (m_pvcAuxColItm->size () > 0)
		{
			// La longitud del anterior es la posición actual menos su posición.
			m4int32_t lLen = m_pstAuxCol->size () - m_pvcAuxColItm->back ()->m_lPos;

			if (lLen == 1)
			{
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_LOGIC_FIELD, (m4int32_t) m_pvcAuxColItm->size ());
				return (M4_ERROR);
			}

			m_pvcAuxColItm->back ()->m_lLength = lLen;
		}
	}
	else
	{
		if (m_pvcAuxColLngItm->size () > 0)
		{
			// La longitud del anterior es la posición actual menos su posición.
			m4int32_t lLen = m_pstAuxColLng->size () - m_pvcAuxColLngItm->back ()->m_lPos;

			if (lLen == 1)
			{
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_LOGIC_FIELD, (m4int32_t) m_pvcAuxColLngItm->size ());
				return (M4_ERROR);
			}

			m_pvcAuxColLngItm->back ()->m_lLength = lLen;
		}
	}

		
	m_pstAux->append (",");

	m_pvcAuxItm->push_back (pMtItem);

	return (M4_SUCCESS);
}


m4return_t ClAPISQL::AnalizeItem (m4pchar_t ai_pToken)
{
	m4int32_t hItem = m_pGlobVars->GetItemHandleByIdItem (&ai_pToken[1], m_bConnActivate); // Así evitamos el '@'.

	if (!hItem)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_ITEM, &ai_pToken[1]);
		return (M4_ERROR);
	}

	ClMetaItem *pMtPar = new ClMetaItem (m_lLevel, m_eClause, hItem);

	m_pvcAuxPar->push_back (pMtPar);

	if (m_pstAux)
	{
		m_pstAux->append( AnalizeParan(pMtPar) );
	}

	return (M4_SUCCESS);
}


m4return_t ClAPISQL::AnalizeSessionItem (m4pchar_t ai_pToken, m4bool_t ai_bAppendParam)
{
	if (!m_pGlobVars->GetpLConn ()->m_pLogonAdaptor->GetSessionRecordSet ())
	{
		DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_SESSION_NOT_FOUND);
		return (M4_ERROR);
	}

	// Aquí buscaremos el Item en el canal sesion.
	m4bool_t bExists = m_pGlobVars->GetpLConn ()->m_pLogonAdaptor->GetSessionRecordSet ()->Register.Item.MoveTo (ai_pToken);

	if (!bExists)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_ITEM, ai_pToken);
		return (M4_ERROR);
	}

	m4int32_t hSessionItem = m_pGlobVars->GetpLConn ()->m_pLogonAdaptor->GetSessionRecordSet ()->Register.Item.ItemDef.GetHandle ();

	ClMetaItemSess *pMtPar = new ClMetaItemSess (m_lLevel, m_eClause, hSessionItem);

	m_pvcAuxPar->push_back (pMtPar);

	if (m_pstAux != NULL && ai_bAppendParam == M4_TRUE)
	{
		m_pstAux->append( AnalizeParan(pMtPar) );
	}

	return (M4_SUCCESS);
}


m4return_t ClAPISQL::AnalizeInternalParam (m4pchar_t ai_pToken)
{
	ClDinArr *pParam = m_pMtSent->GetParam ();

	m4int32_t indParam = pParam->FindIndexByIdField (ai_pToken);

	if (indParam == M4LDB_NULL_INDEX)
	{
		// Lo buscamos en los campos traducidos.
		ClMetaField oFld (M4LDB_NULL_INDEX, M4LDB_CLAUSE_WHERE, ai_pToken, M4LDB_NULL_INDEX, M4LDB_NULL_INDEX);

		oFld.m_indLogicTable = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Index;

		m4return_t result = FindIndexField (&oFld, M4_FALSE, 0);

		if (result == M4_SUCCESS && 
			oFld.m_indRealTable != M4LDB_NULL_INDEX && 
			oFld.m_indRealField != M4LDB_NULL_INDEX)
		{
			m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[(ClRegisterIndex) oFld.m_indRealTable];

			m4id_t idField = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Current[(ClRegisterIndex) oFld.m_indRealField].Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_FIELD].Value.GetWithOutSec ());

			indParam = pParam->FindIndexByIdField (idField);
		}
	}

	if (indParam == M4LDB_NULL_INDEX)
	{
		m4id_t idLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_LOGIC_FIELD, ai_pToken << idLgcTbl);
		return (M4_ERROR);
	}

	ClMetaInternPar *pMtPar = new ClMetaInternPar (m_lLevel, m_eClause, indParam);

	m_pvcAuxPar->push_back (pMtPar);

	m_pstAux->append (" ?");

	return (M4_SUCCESS);
}


m4return_t ClAPISQL::AnalizeSpecialParam (m4pchar_t ai_pToken)
{
	ClMetaConst *pMtPar = NULL;

	if (ai_pToken[0] == 'P' || // PLUS_INF 
		ai_pToken[3] == 'P')   // M4_PLUS_INF
	{
		pMtPar = new ClMetaConst (m_lLevel, m_eClause, M4LDB_PLUS_INF);
	}
	else
	{
		pMtPar = new ClMetaConst (m_lLevel, m_eClause, M4LDB_MINUS_INF);
	}

	m_pvcAuxPar->push_back (pMtPar);

	if (m_pstAux)
	{
		m_pstAux->append( AnalizeParan(pMtPar) );
	}

	return (M4_SUCCESS);
}


void	ClAPISQL::AnalizeLiteral( m4pcchar_t ai_pccLiteral )
{
	m_pstAux->append( " " );

	// UNICODE DB
	if( g_iEncodingMode != M4LDB_ENCODING_ANSI )
	{
		/* Bug 0270427
		A los literales del cuerpo de funciones no se les pone la N.
		Aquí se desescapa.
		*/
		size_t	iLength = strlen( ai_pccLiteral ) ;

		if( iLength > 1 && ai_pccLiteral[ 1 ] == '´' )
		{
			m_pstAux->append( "'" );
			m_pstAux->append( ai_pccLiteral + 2 );
			return;
		}

		m4uint8_t	iType = m_pGlobVars->GetpMCR()->GetChannelType() ;

		// Sólo se pone la N si no es del sistema
		if( iType < 100 )
		{
			/* Bug 0136246
			Sólo se pone la N si es Oracle o SQLServer
			*/
			eDDBBType_t eType = m_pGlobVars->GetDDBBType();

			if( eType == M4DBW_ORACLE_DDBB || eType == M4DBW_SQL_SERVER_DDBB )
			{
				if( g_iEncodingMode == M4LDB_ENCODING_UNICODE )
				{
					/* Bug 0270427
					En entornos unicode puros se pone siempre la N
					*/
					m_pstAux->append ("N");
				}
				else
				{
					// Sólo se pone al N si hay caracteres no ASCII
					m4pcchar_t	pccChar = ai_pccLiteral;

					while( *pccChar != '\0' )
					{
						if( *pccChar & 0x80 )
						{
							m_pstAux->append( "N" );
							break;
						}
						pccChar++;
					}
				}
			}
		}
	}

	m_pstAux->append( ai_pccLiteral );
}


m4return_t ClAPISQL::AnalizeFromClause (m4bool_t ai_bWhere, m4bool_t ai_bChangeSecTable)
{
	if (m_pMtSent->IsReal ())
	{
		SetAuxVectors (M4LDB_CLAUSE_WHERE, M4LDB_NULL_INDEX, M4_TRUE);
		m_pstAux->append (" FROM");
		return (AnalizeWhereClause ());
	}

	m4return_t result;
	m4char_t cSeparator;
	m4bool_t bIsWord;
	m4int32_t lenToken;
	m4bool_t bExit1 = M4_FALSE;
	m4bool_t bExit2 = M4_FALSE;
	m4bool_t bBasis = M4_FALSE;
	m4bool_t bExistBasis = M4_FALSE;
	eStmType_t eStmTp = (ai_bWhere) ? M4LDB_SELECT : m_pMtSent->GetStmType ();	// Para una select o subconsulta ai_bWhere está a TRUE.
	m4int32_t iInvRelation = 0;
	m4int32_t iInvRelAux = 0;
	ClMetaObject *pMtObj = NULL;
	m4int32_t lLevel = m_lLevel;	// Esto lo hacemos porque el nivel puede cambiar al salir.
	m4bool_t bMissingKeyWord = M4_FALSE;
	m4int32_t iIdxLogicTable = M4LDB_NULL_INDEX;
	m4uint32_t iNumRealObjs = 0;
	m4pcchar_t pcRealTableName = NULL;

	// Vamos a recoger los ID_OBJECTs y sus alias (si es que tienen).
	while (!bExit1)
	{
		bExit2 = M4_FALSE;

		result = _X_GetToken (m_pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal ());

		if (result != M4_ERROR)
		{
			if (IsKywAftFrom (m_pAuxString))
			{
				DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_LOGIC_OBJECT);
				return (M4_ERROR);
			}

			//Veamos si es un Hint.
			m4int32_t iNumChrParsed = 0;
			if (ClLConn::_IsReservedWordSysHint(m_pAuxString, iNumChrParsed) == M4_TRUE)
			{
				//Se ha detectado el hint pero el current se ha quedado despues del separador por lo tanto
				//retrocedemos. El separador será probablemente un parentesis abierto.
				m_pCurrent--;
				string sSysHintId(M4LDB_EMPTY);
				if (ClLConn::_GetSysHintIdParsed(m_pGlobVars->GetpLConn (), m_pCurrent, sSysHintId, iNumChrParsed) == M4_SUCCESS)
				{
					m_pCurrent = m_pCurrent + iNumChrParsed;

					// Los hints de from se guardan para ponerlos en la parte select
					m_pMtSent->AddDynamicHint( m_lFilter, sSysHintId.c_str() ) ;
					continue;
				}
				else
				{
					return M4_ERROR;
				}
			}
			else
			{
				m4id_t idLgcTbl = (m4id_t) m_pAuxString;
				// Estamos en un objeto lógico.
				if (*m_pAuxString == '&')
				{
					/* Bug 0103805
					Solo cambiamos la tabla por la de seguridad si se indica
					*/
					if (ai_bChangeSecTable == M4_TRUE && m_pMtSent->GetParam ())
					{
						// Esta es la tabla de seguridad.
						//Bug 0078994						
						//Si estamos aquí es pq estamos en una sentencia de validacion de seguridad. Para que
						//funcione bien tenemos que asociar las tablas base de todos los filtros a M4LDB_SEC_LOGIC_OBJECT,
						//El problema se produce si en una persistencia (delete...) tenemos implicadas varias tablas: la tabla
						//principal (sobre la que se hace el delete...) y otras tablas pe dentro de un exists.
						//Si la tabla que tiene seguridad no es la principal entonces es cuando tenemos el problema.

						//La razon de que funcione es la siguiente:
						//Tenemos sentencia DELETE FROM T3 WHERE EXISTS T1...
						//Tenemos un filtro de seguridad FROM &T1, T2 WHERE T1.C1 = T2.C2.
						//Y la tabla de seguridad es M4LDB_SEC_LOGIC_OBJECT.
						
						//Introducimos la tabla de seguridad como tabla base pero también introducimos la tabla que viene
						//en el filtro de seguridad pero NO como tabla base.
						//Cuando se analiza el where del filtro de seguridad, se comprueba si el alias T1 es de la tabla 
						//base y si lo es se comprueba si esta el campo.
						//En este caso no sera de la tabla base por lo tanto no se buscara en la tabla base M4LDB_SEC_LOGIC_OBJECT
						//(que hubiera dado un error pq la tabla de seguridad tiene a fuego los campos de T3 (tabla principal del delete...)
						//y si se validara C1 sobre la base daria el consabido error sintactico.

						//Accion:
						//Elegimos la tabla base la de seguridad. Además, siempre añadimos la tabla base del filtro de 
						//seguridad, de forma que para cada filtro se añadira la tupla tabla de seguridad + tabla base del filtro.
						//Pero la tabla base del filtro NO se inserta como tabla base.
						
						//Hay un problema con la lo anterior: No funcionaría porque:
						//Es necesario que el where de seguridad incluya al registro que quiero borrar!!!.
						//DE MOMENTO NO TOCAMOS NADA!.

						idLgcTbl = M4LDB_SEC_LOGIC_OBJECT;
						bBasis = M4_TRUE;
						bExistBasis = M4_TRUE;

						/*****
						//Tratamiento de la tabla de seguridad.
						//Creacion del objeto y del alias.
						//---------------------------------------------------------------------------

						m4int32_t indObj = m_pvcAuxFrmObj->size ();
						pMtObj = new ClMetaObject (m_lLevel, m_eClause, idLgcTbl, bBasis, m_lFilter);

						//Vamos a forzar que aparezca en el from, aunque no tenemos campos asociados a esta tabla.
						pMtObj->NotifyUsingRealTable (0);

						//Construimos un alias para la tabla de seguridad.
						pMtObj->m_indAlias = _FindIdxAlias ("0_ALIAS_SECURITY_0", m_lFilter, m_lLevel, m_eClause);
						ClMetaAlias	*pAlias = m_pMtSent->GetAliasByPos(pMtObj->m_indAlias);
						if (pAlias)
						{
							pAlias->m_indObj = indObj;
						}

						//Insertamos el objeto de seguridad.
						m_pvcAuxFrmObj->push_back(pMtObj);
						//---------------------------------------------------------------------------

						//Creamos el objeto.
						//-------------------------						
						//Ya se ha añadido la tabla base, por lo tanto esta no es la base.
						bBasis = M4_FALSE;
						idLgcTbl = (m4id_t) &m_pAuxString[1];
						pMtObj = new ClMetaObject (m_lLevel, m_eClause, idLgcTbl, bBasis, m_lFilter);
						//-------------------------
						*****/
					}
					else
					{
						idLgcTbl = (m4id_t) &m_pAuxString[1];

						// Bug 0293862 la tabla base no viene
						if (*idLgcTbl == '\0')
						{
							return (M4_ERROR);
						}

						if (m_lLevel != m_lInitLevel)
						{
							if (m_pMtSent != NULL && m_pMtSent->GetIsUpdateSelect() == M4_TRUE)
							{
								//Estoy en un update select y es la tabla base de la subselect
								bBasis = M4_TRUE;
							}
							else
							{
								bBasis = M4_FALSE;
							}
						}
						else
						{
							if (bBasis)
							{
								// Objeto base duplicado.
								m4int32_t numObj = m_pvcAuxFrmObj->size ();
								for (m4int32_t indObj=0; indObj<numObj; indObj++)
								{
									PClMetaObject poMtObject = (*m_pvcAuxFrmObj)[indObj];
									if (poMtObject->m_bIsBasis)
									{
										m4id_t idBasisLgcTbl = poMtObject->m_stObject.c_str ();
										if (strcmp (idLgcTbl, idBasisLgcTbl))
										{
											DUMP_LDBINSP_WARNINGF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_BASIS_LOGIC_OBJECT, idBasisLgcTbl << idLgcTbl);
											bBasis = M4_FALSE;
										}
										break;
									}
								}
							}
							else
							{
								bBasis = M4_TRUE;
								bExistBasis = M4_TRUE;
							}
						}
					}
				}
				else
				{
					if (eStmTp != M4LDB_SELECT && !m_pvcAuxFrmObj->size())
					{
						bBasis = M4_TRUE;
						bExistBasis = M4_TRUE;
					}
					else
					{
						bBasis = M4_FALSE;
					}
				}

				pMtObj = new ClMetaObject (m_lLevel, m_eClause, idLgcTbl, bBasis, m_lFilter);
			}
		}
		else
		{
			DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_LOGIC_OBJECT);
			return (M4_ERROR);
		}

		while (!bExit1 && !bExit2)
		{
			switch (cSeparator)
			{
				case '(' :

					if (eStmTp != M4LDB_INSERT)
					{
						// bg 147470
						// Bug 0280428 si pMtObj es nulo no se puede hacer nada
						if(pMtObj != NULL && m_pGlobVars->GetDDBBType() == M4DBW_SQL_SERVER_DDBB)
						{
							result = m_pGlobVars->GetpLConn ()->LoadDictionaryChannel (pMtObj->m_stObject.c_str(), &iIdxLogicTable, M4_TRUE);

							if( result == M4_ERROR )
							{
								return M4_ERROR;
							}

							m_pGlobVars->GetpLConn( )->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.MoveTo( iIdxLogicTable );
							m4uint32_t iNumRealObjs = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Count();
							
							if( iNumRealObjs == 1 )
							{
								pcRealTableName = M4LDB_PCSTR (m_pGlobVars->GetpLConn( )->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[0].Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_REAL_OBJECT].Value.GetWithOutSec ( ));
								
								if( !stricmp( pcRealTableName, "FREETEXTTABLE" ) || !stricmp( pcRealTableName, "CONTAINSTABLE" ))
								{
									// -- Syntax: FREETEXTTABLE/CONTAINSTABLE(realTable, realField, searchString)

									// -- Get realTable arg
									result = _X_GetToken (m_pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal ());

									pMtObj->m_sFTTParams.append("(");
									pMtObj->m_sFTTParams.append(m_pAuxString);

									result = FindSeparator (',');

									if (result == M4_ERROR)
									{
										return (M4_ERROR);
									}

									// -- Get realField arg
									result = _X_GetToken (m_pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal ());

									pMtObj->m_sFTTParams.append(",");
									pMtObj->m_sFTTParams.append(m_pAuxString);

									result = FindSeparator (',');

									if (result == M4_ERROR)
									{
										return (M4_ERROR);
									}

									// -- Get searchString arg
									result = _X_GetToken (m_pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal ());

									pMtObj->m_sFTTParams.append(",");
									pMtObj->m_sFTTParams.append(m_pAuxString);
									pMtObj->m_sFTTParams.append(")");

									result = FindSeparator (')');

									if (result == M4_ERROR)
									{
										return (M4_ERROR);
									}

									break;
								}
							}
						}

						bMissingKeyWord = M4_TRUE;
					}

					bExit1 = M4_TRUE;
					continue;
					break;

				case ')' :
					if (eStmTp == M4LDB_SELECT && m_lLevel != m_lInitLevel)
					{
						// Esto posiblemente sea el fin de una subselect.
						SetAuxVectors (M4LDB_CLAUSE_END_STATEM);
					}
					else
					{
						bMissingKeyWord = M4_TRUE;
					}
					bExit1 = M4_TRUE;
					continue;
					break;

				case ';':
				case '\0' :
					if (eStmTp != M4LDB_SELECT && eStmTp != M4LDB_DELETE)
					{
						bMissingKeyWord = M4_TRUE;
					}
					bExit1 = M4_TRUE;
					continue;
					break;

				case ',' :
					if (eStmTp == M4LDB_SELECT)
					{
						bExit2 = M4_TRUE;
						continue;
					}
					bMissingKeyWord = M4_TRUE;
					bExit1 = M4_TRUE;
					continue;
					break;

				case ' ' :
					break;

				default :
					bMissingKeyWord = M4_TRUE;
					bExit1 = M4_TRUE;
					continue;
			}

			iInvRelAux = iInvRelation;

			result = _X_GetToken (m_pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal (), &iInvRelation);

			if (result != M4_ERROR)
			{
				if (IsKywAftFrom (m_pAuxString))
				{
					switch (*m_pAuxString) {

						case 'W':
							if (eStmTp != M4LDB_SELECT && eStmTp != M4LDB_DELETE)
							{
								bMissingKeyWord = M4_TRUE;
							}
							else
							{
								if (m_indAlsSec == M4LDB_NULL_INDEX || m_lLevel != m_lInitLevel)
								{
									SetAuxVectors (M4LDB_CLAUSE_WHERE);
								}
								else
								{
									SetAuxVectors (M4LDB_CLAUSE_WHERE, m_lInitLevel, M4_TRUE);
					
									//Bug 0075175
									//El problema es debido a la limitacion de la pila en AIX.
									//Con un número considerable de selects anidadas, se produce un
									//desbordamiento de pila que tenemos a 64Kb.
									//Hay tres miembros involucrados: AnalizeFrom, AnalizeWhere, AnalizeMultifilter.
									//Soluciones:
									//1.- Utilizamos el Heap. Creamos un vector y despues lo liberamos.
									//			m4char_t * alias = NULL;
									//			alias = new m4char_t [M4LDB_SIZE_DEFAULT_STRING + 1];
									//			.....
									//			if (aAlias) {
									//				delete [] aAlias;
									//			}
									//			aAlias = NULL;

									//2.- Limitar el tamaño a 20 + NULL dígitos para el indice de alias.
									//		Utilizamos el vector SOLO para el numero!!:
									//		Es importante que a m_pstAux acabemos haciendo un append de: "~A_%ld AND"
									//Consumo:	From 1188 bytes.
									//			Where 748 bytes.
									//			Multi 100 bytes.
									//Ahorramos 232 bytes por iteracion. Si hay 110 llamdas recursivas ahorramos aprox. 22Kb de pila.

									m4char_t aAlias[21];
									sprintf (aAlias, "%ld", m_indAlsSec);
									m_pstAux->append ("~A_");
									m_pstAux->append (aAlias);
									m_pstAux->append (" AND");
								}
							}
							break;

						case 'O':
							if (eStmTp != M4LDB_SELECT)
							{
								bMissingKeyWord = M4_TRUE;
							}
							else
							{
								result = _X_GetToken (m_pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal ());
								if (!strcmp (m_pAuxString, "BY"))
								{
									SetAuxVectors (M4LDB_CLAUSE_ORDER_BY);
								}
								else
								{
									DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, "'BY'");
									return (M4_ERROR);
								}
							}
							break;

						case 'F':
							if (eStmTp != M4LDB_SELECT)
							{
								bMissingKeyWord = M4_TRUE;
							}
							else
							{
								result = _X_GetToken (m_pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal ());
								if (!strcmp (m_pAuxString, "UPDATE"))
								{
									m_pMtSent->SetForUpdate (M4_TRUE);
									while ((cSeparator != '\0') && 
										(cSeparator != ';'))
									{
										// El resto lo descartamos pero buscamos el final de la select.
										iInvRelAux = iInvRelation;

										result = _X_GetToken (m_pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal (), &iInvRelation);

										//Bug 0071661.
										//Comprobamos si es NOWAIT.
										//En ese caso marcamos la metasentencia como NOWAIT.
										if (!strcmp (m_pAuxString, "NOWAIT"))
										{
											m_pMtSent->SetNoWait (M4_TRUE);
										}

										if (iInvRelAux > iInvRelation)
										{
											AddToOr (M4_FALSE);
										}
									}
								}
								else
								{
									DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, "'UPDATE'");
									return (M4_ERROR);
								}
							}
							break;

						case 'S':
							if (eStmTp != M4LDB_UPDATE)
							{
								bMissingKeyWord = M4_TRUE;
							}
							break;

						default:
							if (eStmTp != M4LDB_SELECT)
							{
								bMissingKeyWord = M4_TRUE;
							}
							else
							{
								m_bNoAddDtLastUpdate = M4_TRUE;
								SetAuxVectors (M4LDB_CLAUSE_REST);
								m_pstAux->append (" ");
								m_pstAux->append (m_pAuxString);
							}
							break;
					}
					bExit1 = M4_TRUE;
					continue;
				}
				else
				{
					// Estamos en un alias.
					if (pMtObj)
					{
						if (*m_pAuxString == '^')
						{
							// Este objeto lógico ya existe.
							delete pMtObj;
							bExistBasis = M4_TRUE;
							if (bBasis && m_indAlsBasis == M4LDB_NULL_INDEX)
							{
								m_indAlsBasis = _FindIdxAlias (m_pAuxString, m_lFilter, m_lLevel, m_eClause);
							}
						}
						else
						{
							if (eStmTp == M4LDB_SELECT && bBasis)
							{
								//Se comprueba que la tabla base tratada es igual
								// a la tabla base de la sentencia principal
								// Se hace cuando hay más de un filtro aplicado (dos sys_sentence)
								// o en un update select.
								m4int32_t numObj = m_pvcAuxFrmObj->size ();
								for (m4int32_t indObj=0; indObj<numObj; indObj++)
								{
									PClMetaObject poMtObject = (*m_pvcAuxFrmObj)[indObj];
									if (poMtObject ->m_bIsBasis)
									{
										m4id_t idLgcTbl = pMtObj->m_stObject.c_str ();
											
										m4id_t idBasisLgcTbl = poMtObject->m_stObject.c_str ();
										if (strcmp (idLgcTbl, idBasisLgcTbl))
										{
											DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_BASIS_LOGIC_OBJECT, idLgcTbl << idBasisLgcTbl);
											delete pMtObj;
											return (M4_ERROR);
										}
										break;
									}
								}
							}

							pMtObj->m_indAlias = _FindIdxAlias (m_pAuxString, m_lFilter, m_lLevel, m_eClause);

							m4int32_t indObj = m_pvcAuxFrmObj->size ();

							ClMetaAlias	*pAlias = m_pMtSent->GetAliasByPos(pMtObj->m_indAlias);
							if (pAlias->m_indObj!=M4_STR_NPOS)
							{
								// Ya tenía un objeto asociado.
								PClMetaObject poMtObject = (*m_pvcAuxFrmObj)[pAlias->m_indObj];
								DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_DUPLICATED_ALIAS,m_pAuxString<<poMtObject->m_stObject.c_str()<<pMtObj->m_stObject.c_str());
								delete pMtObj;
								return (M4_ERROR);
							}
							pAlias->m_indObj = indObj;
			
							m_pvcAuxFrmObj->push_back(pMtObj);

							if (bBasis && m_indAlsBasis == M4LDB_NULL_INDEX)
							{
								m_indAlsBasis = pMtObj->m_indAlias;
							}
						}
						pMtObj = NULL;
					}
					else
					{
						bMissingKeyWord = M4_TRUE;
						bExit1 = M4_TRUE;
						continue;
					}
				}
			}

		}
		if (pMtObj)
		{
			// No tenemos alias, asi que le creamos uno interno.
			if (_CreateAlias (pMtObj, bBasis)!=M4_SUCCESS)
			{
				return M4_ERROR;
			}
			pMtObj = NULL;
		}
	}

	if (bMissingKeyWord)
	{
		switch (eStmTp)
		{
			default:
			case M4LDB_SELECT:
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, "','");
				break;
			case M4LDB_INSERT:
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, "'('");
				break;
			case M4LDB_UPDATE:
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, "'SET'");
				break;
			case M4LDB_DELETE:
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, "'WHERE'");
				break;
		}
		return (M4_ERROR);
	}

	if (pMtObj)
	{
		if (_CreateAlias (pMtObj, bBasis)!=M4_SUCCESS)
		{
			return M4_ERROR;
		}
		pMtObj = NULL;
	}


	if (eStmTp == M4LDB_SELECT)
	{
		if (lLevel == m_lInitLevel && !bExistBasis)
		{
			DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_BASIS_LOGIC_OBJECT);
			return (M4_ERROR);
		}
	
		if (cSeparator == ';')
		{
			return (AnalizeMultiFilter ());
		}
	}

	if (cSeparator == ')')
	{
		return (M4_SUCCESS);
	}

	if (eStmTp == M4LDB_INSERT)
	{
		return (AnalizeFieldsClause ());
	}

	if (eStmTp == M4LDB_UPDATE)
	{

		m4pchar_t pAuxCurrent;
		pAuxCurrent = m_pCurrent;

		result = _X_GetToken (pAuxCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal ());
		
		if ( cSeparator != '(' )// Un UPDATE...VALUES clásico:
		{
			return ( AnalizeFieldsAndValuesClause () );
		}
		else
		{
			//Estamos en un UPDATE TABLA SET (DATA1,DATA2..) SELECT (DATA1,DATA2...
			//Hacemos lo mismo que el INSERT INTO SELECT con esto se analizan los campos
			// a hacer update y la select que servirá para darles valor.
			return ( AnalizeFieldsClause () );
		}
	}
	
	if (iInvRelAux < iInvRelation)
	{
		AddToOr (M4_TRUE);
	}

	return (AnalizeWhereClause (iInvRelation));
}


m4return_t ClAPISQL::AnalizeWhereClause (m4int32_t ai_iInvRelation)
{
	m4return_t result;
	m4char_t cSeparator;
	m4bool_t bIsWord;
	m4int32_t lenToken;
	m4bool_t bExit = M4_FALSE;
	m4bool_t bPossibleRel = M4_FALSE;
	m4bool_t bOuter = M4_FALSE;
	m4bool_t bOuter2 = M4_FALSE;
	m4bool_t bIsFld = M4_FALSE;
	m4bool_t bExistsEqual = M4_FALSE;
	m4int32_t iInvRelation = ai_iInvRelation;
	m4int32_t iInvRelAux = 0;
	m4int32_t lInitRelation = M4_FALSE;
	m4int32_t iInnerPar = 0;
	eStmType_t eStmTp = m_pMtSent->GetStmType ();

	ClMetaObject *pObjectAPISQLSource = 0;
	m4int32_t indAPISQLSource = M4LDB_NULL_INDEX;

	if (*m_pCurrent == '\0')
	{
		return (M4_SUCCESS);
	}

	// Para no comernos el separador.
	m_pCurrent--;

	if ((!m_pMtSent->IsReal ()) && 
		(((m_eClause == M4LDB_CLAUSE_WHERE) &&
		(m_lLevel == m_lInitLevel)) || 
		(m_eClause == M4LDB_CLAUSE_BEGIN_STATEM)))
	{	// Esto es por el insert.
		m4pchar_t pCurrent = m_pCurrent;

		FindOR (pCurrent);

		m4char_t car = *(m_pCurrent - 1) - 128;
	
		if (car == 'E')
		{	// Esta 'E' corresponde a la última letra del WHERE.
			iInvRelation++;
			AddToOr (M4_TRUE);
			*(m_pCurrent - 1) = car;
		}
	}
	
	// Vamos a analizar los elementos del Where.
	while (!bExit)
	{
		bIsFld = M4_FALSE;

		if (!bPossibleRel)
		{
			lInitRelation = M4LDB_NULL_INDEX;
			bExistsEqual = M4_FALSE;
		}

		bPossibleRel = M4_FALSE;
		
		if (iInvRelAux > iInvRelation)
		{
			AddToOr (M4_FALSE);
		}

		iInvRelAux = iInvRelation;

		result = _X_GetToken (m_pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal (), &iInvRelation);

		if (iInvRelAux < iInvRelation)
		{
			AddToOr (M4_TRUE);
			iInvRelAux = iInvRelation;
		}


		if (result != M4_ERROR)
		{
			if (bIsWord)
			{
				if (m_pMtSent->IsReal ())
				{
					if (!strcmp (m_pAuxString, "PLUS_INF") || 
						!strcmp (m_pAuxString, "MINUS_INF") || 
						!strcmp (m_pAuxString, "M4_PLUS_INF") || 
						!strcmp (m_pAuxString, "M4_MINUS_INF"))
					{
						result = AnalizeSpecialParam (m_pAuxString);

						if (result == M4_ERROR)
						{
							return (M4_ERROR);
						}
					}
					else
					{
						bIsWord = M4_FALSE;
					}
				}
				else
				{
					if (IsKeyword (m_pAuxString))
					{
						if (IsKywAftFrom (m_pAuxString))
						{
							switch (*m_pAuxString)
							{

								case 'W':
									SetAuxVectors (M4LDB_CLAUSE_WHERE);
									break;

								case 'O':
									if (iInvRelAux > iInvRelation)
									{
										AddToOr (M4_FALSE);
									}

									iInvRelAux = iInvRelation;

									result = _X_GetToken (m_pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal ());
									if (!strcmp (m_pAuxString, "BY"))
									{
										SetAuxVectors (M4LDB_CLAUSE_ORDER_BY);
									}
									else
									{
										DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, "'BY'");
										return (M4_ERROR);
									}
									break;

								case 'F':
									if (iInvRelAux > iInvRelation)
									{
										AddToOr (M4_FALSE);
									}

									iInvRelAux = iInvRelation;

									result = _X_GetToken (m_pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal (), &iInvRelation);

									if (iInvRelAux > iInvRelation)
									{
										AddToOr (M4_FALSE);
									}

									if (!strcmp (m_pAuxString, "UPDATE"))
									{
										m_pMtSent->SetForUpdate (M4_TRUE);

										while (cSeparator != '\0' && cSeparator != ';')
										{
											// El resto lo descartamos pero buscamos el final de la select.
											iInvRelAux = iInvRelation;

											result = _X_GetToken (m_pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal (), &iInvRelation);

											//Bug 0071661.
											//Comprobamos si es NOWAIT.
											//En ese caso marcamos la metasentencia como NOWAIT.
											if (!strcmp (m_pAuxString, "NOWAIT"))
											{
												m_pMtSent->SetNoWait (M4_TRUE);
											}

											if (iInvRelAux > iInvRelation)
											{
												AddToOr (M4_FALSE);
											}
										}
										bIsWord = M4_TRUE;
									}
									else
									{
										DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, "'UPDATE'");
										return (M4_ERROR);
									}

									iInvRelAux = iInvRelation;
									break;

								default:
									if (iInvRelAux > iInvRelation)
									{
										AddToOr (M4_FALSE);
									}

									iInvRelAux = iInvRelation;

									m_bNoAddDtLastUpdate = M4_TRUE;
									if (m_eClause != M4LDB_CLAUSE_REST && m_eClause != M4LDB_CLAUSE_ORDER_BY)
									{
										SetAuxVectors (M4LDB_CLAUSE_REST);
									}
									m_pstAux->append (" ");
									m_pstAux->append (m_pAuxString);
									break;
							}
						}
						else
						{
							if (!strcmp (m_pAuxString, "SELECT"))
							{
								SetAuxVectors (M4LDB_CLAUSE_BEGIN_STATEM);
							}
							else
							{
								if (!strcmp (m_pAuxString, "FROM"))
								{
									if (m_eClause != M4LDB_CLAUSE_BEGIN_STATEM)
									{
										DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, "'SELECT'");
										return (M4_ERROR);
									}
									// Estoy dentro de una subconsulta.
									SetAuxVectors (M4LDB_CLAUSE_FROM);
									/* Bug 0103805
									Si estamos dentro del where ya no hay que sustituir
									la tabla de seguridad
									*/
									result = AnalizeFromClause (M4_TRUE, M4_FALSE);
									if (result == M4_ERROR)
									{
										return (M4_ERROR);
									}
									cSeparator = *(m_pCurrent-1);
									if (cSeparator != ')' && cSeparator != '\0')
									{
										cSeparator = ' ';
									}
								}
								else
								{
									if (!strcmp (m_pAuxString, "PLUS_INF") || 
										!strcmp (m_pAuxString, "MINUS_INF") || 
										!strcmp (m_pAuxString, "M4_PLUS_INF") || 
										!strcmp (m_pAuxString, "M4_MINUS_INF"))
									{
										result = AnalizeSpecialParam (m_pAuxString);

										if (result == M4_ERROR)
										{
											return (M4_ERROR);
										}
									}
									else
									{
										if (!strcmp (m_pAuxString, "OR"))
										{
											if (m_indAlsSec == M4LDB_NULL_INDEX)
											{
												m_pMtSent->SetOr (M4_TRUE);
											}
											AddToOr (M4_FALSE);
											m_pstAux->append (" OR");
											AddToOr (M4_TRUE);
										}
										else
										{
											m_pstAux->append (" ");
											m_pstAux->append (m_pAuxString);

											// UNICODE DB
											if( g_iEncodingMode != M4LDB_ENCODING_ANSI && m_pGlobVars->GetDDBBType() == M4DBW_ORACLE_DDBB )
											{
												if( strcmp( m_pAuxString, "LIKE" ) == 0 )
												{
													/* Bug 0178961
													El LIKE 2 no funciona con campos long
													m_pstAux->append( "2" ) ;
													*/
												}
											}

											//Bug 0076011
											if (!strcmp (m_pAuxString, "HAVING"))
											{
												//En este punto el current ya ha avanzado hasta el siguiente token del Having.
												//Por lo tanto (m_pCurrent - 1) debe ser un blanco!!.
												m4pchar_t pCurrent = m_pCurrent;

												//Importante!: FindOR se queda con el pLeft - 1, por lo tanto aqui tendremos
												//que utilizar el m_pCurrent - 1.
												//Nos quedamos con el caracter pq el FindOR puede modificarlo.
												//Debería ser un blanco.
												//El tipo debe ser m4uchar_t!.
												m4uchar_t cBefore = *(m_pCurrent - 1);
												//Con esta llamada se marcara el ambito de los OR que haya.							
												FindOR (pCurrent);	
												//Comprobamos el caracter despues de pasar por FindOR.
												m4uchar_t cAfter = *(m_pCurrent - 1);

												//Si el caracter cambio en el FindOR es pq se encontro un OR.
												//En realidad, si ha encontrado OR tendremos que el caracter lo habra 
												//modificado a cBefore + 128.		

												//Por lo tanto, si se cumple la condicion es pq se encontro OR.
												if (cBefore == (cAfter - 128))
												{
													AddToOr (M4_TRUE);
													//Restauro el caracter que había inicialmente.
													*(m_pCurrent - 1) = cBefore;
												}
											}
										}
									}
								}
							}
						}
					}
					else
					{
						bIsWord = M4_FALSE;
					}
				}
			}
			
			if (!bIsWord)
			{
				//Si no se analiza campo hay que resetear valores;
				if ( (*m_pAuxString == '#')
					|| (*m_pAuxString == '@')
					|| (*m_pAuxString == '$')
					|| (*m_pAuxString == '?')
					|| (*m_pAuxString == '\'')
					|| (*m_pAuxString == '`') )					
				{
						ResetFlagsMultiOrganization ();
				}
				switch (*m_pAuxString)
				{
					case '#':
						//Solo permitimos el hint al principio de una subselect.
						if (m_eClause == M4LDB_CLAUSE_BEGIN_STATEM && m_lLevel > 0)
						{
							m4int32_t iNumChrParsed = 0;
							if (ClLConn::_IsReservedWordSysHint(m_pAuxString, iNumChrParsed) == M4_TRUE)
							{
								//Se ha detectado el hint pero el current se ha quedado despues del separador por lo tanto
								//retrocedemos. El separador será probablemente un parentesis abierto.
								m_pCurrent--;
								string sSysHintId(M4LDB_EMPTY);							
								if (ClLConn::_GetSysHintIdParsed(m_pGlobVars->GetpLConn (), m_pCurrent, sSysHintId, iNumChrParsed) == M4_SUCCESS)
								{
									m_pCurrent = m_pCurrent + iNumChrParsed;
									if (m_pstAux == NULL)
									{
										SetAuxVectors (M4LDB_CLAUSE_COLUMN);
									}									
									m_pstAux->append(" ");
									m_pstAux->append(m_pAuxString);
									m_pstAux->append("(");
									m_pstAux->append (sSysHintId);
									m_pstAux->append(")");
									m_pstAux->append(" ");
									continue;
								}
								else
								{
									return M4_ERROR;
								}
							}
						}

						//Si no era un hint estaremos aqui.
						result = AnalizeFunction (m_pAuxString);
						if (result == M4_ERROR)
						{
							return (M4_ERROR);
						}

						cSeparator = ' ';
						break;

					case '@': 
						if (m_eClause == M4LDB_CLAUSE_BEGIN_STATEM)
						{
							// Volvemos atras para situarnos en el separador;
							m4pchar_t pCurrent = m_pCurrent-1;
							// Quitamos blancos.
							for (; ((!*pCurrent) || IsControlCharacter(*pCurrent)); )
							{
								pCurrent++;
							}
							if (*pCurrent == '=')
							{
								// Hemos encontrado un '@Item=' en las columnas de la subselect.
								// Lo quitamos.
								m_pCurrent = pCurrent + 1;
								DUMP_LDBINSP_WARNINGF (m_pGlobVars->GetpLConn (), M4_CH_LDB_ITEM_IN_SUBQUERY, m_pAuxString);
								continue;
							}
						}
						result = AnalizeItem (m_pAuxString);

						if (result == M4_ERROR)
						{
							return (M4_ERROR);
						}
						break;

					case '$': 
						result = AnalizeSessionItem (m_pAuxString + 1, M4_TRUE);

						if (result == M4_ERROR)
						{
							return (M4_ERROR);
						}
						break;

					case '?': 
						result = AnalizeArgument (cSeparator);
						
						if (result == M4_ERROR)
						{
							return (M4_ERROR);
						}

						cSeparator = ' ';
						break;

					case '\'': 
						AnalizeLiteral(m_pAuxString);
						break;

					case '`': 
						m_pstAux->append (" ");
						m_pstAux->append (m_pAuxString+1);
						cSeparator = ' ';
						break;

					default:
						if (!m_pMtSent->IsReal ())
						{
							m4int32_t lPos = m_pstAux->size (); 

   							result = AnalizeFieldOrConst (m_pAuxString, &bIsFld);

							bPossibleRel = bIsFld;

							if (bPossibleRel && 
								!iInvRelation && 
								m_eClause == M4LDB_CLAUSE_WHERE)
							{

								if (lInitRelation == M4LDB_NULL_INDEX)
								{
									lInitRelation = lPos;

									bOuter = FindOuterJoin ();

									if (bOuter)
									{
										// Marcamos el campo como Outer.
										ClMetaField	*pFld =m_pvcAuxFld->back();
										pFld -> m_bIsOuter = M4_TRUE;
										cSeparator = ' ';
										//Bug 0075780
										//En el escenario de estar analizado c1 en la sentencia A.c1 (+) = B.c2
										//el campo se marca, pero tb vamos a marcar el objeto A.
										ClMetaObject *pobj = 0;
										m4int32_t indObjTmp = M4LDB_NULL_INDEX;
										indObjTmp = m_pMtSent->GetObjectPosByAliasPos(pFld->m_indAlias);
										if (indObjTmp != M4LDB_NULL_INDEX)
										{
											pobj = (*m_pvcAuxFrmObj)[indObjTmp];
											pobj->m_eJoinType = M4LDB_OUTER_JOIN;
										}
									}
								}
								else
								{
									if (bExistsEqual)
									{
										// Los últimos serán los primeros.
										m4int32_t size = m_pvcAuxFld->size();

										PClMetaField pMtFldRight = (*m_pvcAuxFld)[size-1];
										PClMetaField pMtFldLeft = (*m_pvcAuxFld)[size-2];

										if ( pMtFldRight->m_indAlias != pMtFldLeft->m_indAlias )
										{
											m_pvcAuxFld->pop_back ();
											m_pvcAuxFld->pop_back ();

											ClMetaRelat *pstRel=0;
											m4bool_t bSecondOuter = FindOuterJoin ();

											if (bOuter)
											{
												if (bSecondOuter)
												{
													// Hay un doble Outer Join.Damos Warning y nos quedamos con el primero.
													m4pcchar_t id_obj1 = "",id_obj2 = "";
													ClMetaObject *pobj = 0;
													m4int32_t indObj = M4LDB_NULL_INDEX;// ( puede que no exista el objeto )
	
													// Objeto 1.
													indObj = m_pMtSent->GetObjectPosByAliasPos(pMtFldLeft->m_indAlias);
													if (indObj != M4LDB_NULL_INDEX)
													{
														pobj = (*m_pvcAuxFrmObj)[indObj];
													}
													id_obj1 = pobj ->m_stObject.c_str();

													// Objeto 2.
													indObj = m_pMtSent->GetObjectPosByAliasPos(pMtFldRight->m_indAlias);
													if (indObj != M4LDB_NULL_INDEX)
													{
														pobj = (*m_pvcAuxFrmObj)[indObj];
													}

													id_obj2 = pobj ->m_stObject.c_str();

													DUMP_LDBINSP_WARNINGF(m_pGlobVars->GetpLConn (),M4_CH_LDB_DOUBLE_OUTER_JOIN,id_obj1<<id_obj2);
													cSeparator = ' ';

												}
												// Lo habiamos marcado como Outer. Lo quitamos porque la marca la lleva la relacion.
												pMtFldLeft->m_bIsOuter = M4_FALSE;
												pstRel = new ClMetaRelat (m_lLevel, m_eClause, pMtFldRight, pMtFldLeft, M4LDB_OUTER_JOIN);
											}
											else
											{
												bOuter2 = bSecondOuter;
												if (bOuter2)
												{
													/////////////////////////////////////////
													//Pedro: Bug 0064565
													//Solucion para que los conectores tengan outer join.													
													//Cuando se detecta que un objeto tiene OJ, se marcara ese objeto
													//El problema está en que el join de filtro se esta efectuando antes del TransferMetadata
													//Ver la función AnalizeSQL.
													//Cuando añadimos el filtro de conector, se pregunta si el objeto tiene OJ.
													ClMetaObject *pobj = 0;
													m4int32_t indObjTmp = M4LDB_NULL_INDEX;
													indObjTmp = m_pMtSent->GetObjectPosByAliasPos(pMtFldRight->m_indAlias);
													if (indObjTmp != M4LDB_NULL_INDEX)
													{
														pobj = (*m_pvcAuxFrmObj)[indObjTmp];
														pobj->m_eJoinType = M4LDB_OUTER_JOIN;
														//El campo NO lo marcamos, marcamos el objeto.
														//pMtFldRight->m_bIsOuter = M4_TRUE;
													}	
													//fin
													/////////////////////////////////////////

													pstRel = new ClMetaRelat (m_lLevel, m_eClause, pMtFldLeft, pMtFldRight, M4LDB_OUTER_JOIN);
													cSeparator = ' ';
												}
												else
												{
													pstRel = new ClMetaRelat (m_lLevel, m_eClause, pMtFldLeft, pMtFldRight, M4LDB_NORMAL_JOIN);
												}
											}

											//Pedro. Cuando introduzcamos la relación en el vector, indicamos el tipo de APISQL.
											pstRel->SetAPISQLSource(GetAPISQLSource());
											m_pvcAuxFrmRel->push_back (pstRel);

											//-------------------------------------------											
											//Marcamos los objetos tambien, es util para las relaciones dummy.
											indAPISQLSource = m_pMtSent->GetObjectPosByAliasPos(pstRel->m_pMtField1->m_indAlias);											
											if ((indAPISQLSource != M4LDB_NULL_INDEX) && (m_pvcAuxFrmObj))
											{
												pObjectAPISQLSource = (*m_pvcAuxFrmObj)[indAPISQLSource];
												if (pObjectAPISQLSource)
												{
													pObjectAPISQLSource->SetAPISQLSource(GetAPISQLSource());
												}
											}

											indAPISQLSource = m_pMtSent->GetObjectPosByAliasPos(pstRel->m_pMtField2->m_indAlias);											
											if ((indAPISQLSource != M4LDB_NULL_INDEX) && (m_pvcAuxFrmObj))
											{
												pObjectAPISQLSource = (*m_pvcAuxFrmObj)[indAPISQLSource];
												if (pObjectAPISQLSource)
												{
													pObjectAPISQLSource->SetAPISQLSource(GetAPISQLSource());
												}
											}
											//-------------------------------------------

											// Borramos del string la relación (el ~R es necesario para limpiar los AND y parentesis que queden sueltos).
											m_pstAux->replace (lInitRelation, m_pstAux->size () - lInitRelation, "~R");
										}
									}

									bPossibleRel = M4_FALSE;

									lInitRelation = M4LDB_NULL_INDEX;
	
									bOuter = M4_FALSE;
	
									bExistsEqual = M4_FALSE;
								}
							}
							else
							{
								bPossibleRel = M4_FALSE;
	
								lInitRelation = M4LDB_NULL_INDEX;
	
								bOuter = M4_FALSE;

								bExistsEqual = M4_FALSE;
							}
						}
						else
						{
							// Bug 0140959. En el caso de que estemos en SQL Server, verificamos 
							// si el último carácter de la sentencia es un corchete "[" y 
							// que el separador es un "]" y si es así, no añadimos el espacio.
							m4bool_t	bAddSpace = M4_TRUE ;

							if (m_pGlobVars->GetDDBBType() == M4DBW_SQL_SERVER_DDBB)
							{
								if( m_pstAux->empty() == M4_FALSE )
								{
									size_t iLen = m_pstAux->size();
									m4pcchar_t pccAux = m_pstAux->c_str();

									if (pccAux[iLen - 1] == '[' && cSeparator == ']')
									{
										bAddSpace = M4_FALSE;
									}
								}
							}

							if( bAddSpace == M4_TRUE )
							{
								m_pstAux->append (" ");
							}
							m_pstAux->append (m_pAuxString);
						}
						
						if (result == M4_ERROR)
						{
							return (M4_ERROR);
						}
						break;
				}
			}
		}
		
		switch (cSeparator)
		{
			case '(' :
				bOuter2 = FindOuterJoin ();
				// Si encontramos el '(+)' en algo que no sea un campo damos error.
				if (bOuter2 && !m_pMtSent->IsReal())
				{
					if (!bIsFld)
					{
						DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_OUTER_JOIN_NO_ALLOWED);
						return (M4_ERROR);
					}
					else
					{
						DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_OUTER_JOIN_NO_ALLOWED_II);
						return (M4_ERROR);
					}
				}
				lInitRelation = M4LDB_NULL_INDEX;
				if (!bOuter2)
				{
					m_pstAux->append (&cSeparator, 0, 1);
					iInnerPar++;
				}
				break;

			case ')' :
				if (iInvRelAux > iInvRelation)
				{
					AddToOr (M4_FALSE);
				}
				iInvRelAux = iInvRelation;
				lInitRelation = M4LDB_NULL_INDEX;
				iInnerPar--;
				if (iInnerPar < 0)
				{
					// Esto posiblemente sea el fin de una subselect.
					SetAuxVectors (M4LDB_CLAUSE_END_STATEM);
					if (m_lLevel >= m_lInitLevel)
					{
						return (M4_SUCCESS);
					}
				}
				m_pstAux->append (&cSeparator, 0, 1);
				break;
			
			case ' ' :
				break;
				
			case ';':
				if (m_pMtSent->IsReal ())
				{
					m_pstAux->append (&cSeparator, 0, 1);
				}
				else
				{
					if (eStmTp == M4LDB_SELECT)
					{
						if (iInvRelAux > iInvRelation)
						{
							AddToOr (M4_FALSE);
						}
						return (AnalizeMultiFilter ());
					}
					bExit = M4_TRUE;
				}
				break;
				
			case '\0' :
				bExit = M4_TRUE;
				break;
				
			case '=' :
				if (bExistsEqual)
				{
					// Doble igual, malo.
					bPossibleRel = M4_FALSE;
					bExistsEqual = M4_FALSE;
				}
				else
				{
					if (lInitRelation != M4LDB_NULL_INDEX)
					{
						bPossibleRel = M4_TRUE;
						bExistsEqual = M4_TRUE;
					}
				}
				m_pstAux->append (&cSeparator, 0, 1);
				break;

			case '¿':
				//Se esta analizando un where de de seguridad que ha sido insertado al heredar un filtro.
				if (GetAPISQLSource( ) == M4LDB_APISQL_FROM_SECURITY)
				{
					SetAPISQLSource( M4LDB_APISQL_FROM_SECURITY_XJOIN );
				}
				else
				{
					//Si es xjoin seguro que inicialmente era de seguridad.
					if (GetAPISQLSource( ) == M4LDB_APISQL_FROM_SECURITY_XJOIN)
					{
						SetAPISQLSource( M4LDB_APISQL_FROM_SECURITY );
					}
				}
				break;

			default :
				lInitRelation = M4LDB_NULL_INDEX;
				m_pstAux->append (&cSeparator, 0, 1);
				break;
		}
	}
	
	if (iInvRelAux > iInvRelation)
	{
		AddToOr (M4_FALSE);
	}

	return (M4_SUCCESS);
}


m4return_t ClAPISQL::AnalizeColumnsClause (void)
{
	m4return_t result;
	m4int32_t lenToken;
	m4char_t cSeparator;
	m4bool_t bIsWord;
	m4bool_t bFirst = M4_TRUE;
	m4bool_t bError = M4_TRUE;
	m4bool_t bEndColumn = M4_FALSE;
	m4bool_t bInvColumn = M4_FALSE;
	m4bool_t bIsFld = M4_FALSE;
	m4int32_t iInnerPar = 0;
	m4bool_t bNoAddDtLastUpdate = M4_TRUE;
	m4int32_t hItemCol = M4LDB_NULL_INDEX;


	// Se está analizando una select.
	// Si es de primer nivel se setea el filtro de la sentencia principal
	if( m_lLevel == 0 )
	{
		m_pMtSent->SetMainFilter( m_lFilter ) ;
	}

	while (1)
	{
		hItemCol = M4LDB_NULL_INDEX;

		bIsFld = M4_FALSE;
		
		bEndColumn = M4_FALSE;

		bInvColumn = M4_FALSE;
		
		bError = M4_TRUE;
		
		result = _X_GetToken (m_pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal ());
		
		if (result != M4_ERROR)
		{
			if (bIsWord)
			{
				if (bFirst)
				{
					bFirst = M4_FALSE;
					if (!strcmp (m_pAuxString, "DISTINCT"))
					{
						m_pMtSent->SetDistinct (M4_TRUE);
						m_bNoAddDtLastUpdate = M4_TRUE;
						continue;
					}
				}
			}
			else
			{
				if (*m_pAuxString == '@')
				{
					result = AnalizeItemCol (m_pAuxString, hItemCol);
					
					if (result == M4_ERROR)
					{
						return (M4_ERROR);
					}

					if (result == M4_WARNING)
					{
						bInvColumn = M4_TRUE;
					}
					else
					{
						result = FindSeparator ('=');
						if (result == M4_ERROR)
						{
							return (M4_ERROR);
						}						
					}
					bError = M4_FALSE;
				}
				else
				{
					if ((m_pMtSent->GetParam() || m_bIsSecurityView == M4_TRUE) && !strcmp(m_pAuxString, "1"))
					{
						SetAuxVectors (M4LDB_CLAUSE_COLUMN);

						ClMetaItemCol *pMtItem = new ClMetaItemCol (0, M4LDB_CLAUSE_COLUMN, 0);

						pMtItem->m_lPos = m_pstAux->size ();

						m_pstAux->append (", 1");

						m_pvcAuxItm->push_back (pMtItem);

						bError = M4_FALSE;
					}
					else
					{						
						//Veamos si es un Hint.
						m4int32_t iNumChrParsed = 0;
						if (ClLConn::_IsReservedWordSysHint(m_pAuxString, iNumChrParsed) == M4_TRUE)
						{
							//Se ha detectado el hint pero el current se ha quedado despues del separador por lo tanto
							//retrocedemos. El separador será probablemente un parentesis abierto.
							m_pCurrent--;
							string sSysHintId(M4LDB_EMPTY);							
							if (ClLConn::_GetSysHintIdParsed(m_pGlobVars->GetpLConn (), m_pCurrent, sSysHintId, iNumChrParsed) == M4_SUCCESS)
							{
								m_pCurrent = m_pCurrent + iNumChrParsed;
								if (m_pstAux == NULL)
								{
									SetAuxVectors (M4LDB_CLAUSE_COLUMN);
								}								
								m_pstAux->append(" ");
								m_pstAux->append(m_pAuxString);
								m_pstAux->append("(");
								m_pstAux->append (sSysHintId);
								m_pstAux->append(")");
								m_pstAux->append(" ");
								continue;
							}
							else
							{
								return M4_ERROR;
							}
						}
					}
				}
			}
		}
		
		bFirst = M4_FALSE;
		
		if (bError)
		{
			m4int32_t indCol = (m_pvcAuxItm) ? m_pvcAuxItm->size () + 1 : 1;
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_ITEM, indCol);
			return (M4_ERROR);
		}

		m4bool_t bValidateType = M4_TRUE;
		
		while (!bEndColumn)
		{
			result = _X_GetToken (m_pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal ());

			if (result != M4_ERROR)
			{
				if (bIsWord)
				{
					if (IsKeyword (m_pAuxString))
					{
						if (!strcmp (m_pAuxString, "FROM"))
						{
							if (m_pvcAuxColItm->size () == 0 && m_pvcAuxColLngItm->size () == 0)
							{
								DUMP_LDBINSP_WARNING (m_pGlobVars->GetpLConn (), M4_CH_LDB_NO_REGISTER_ITEM);
								return (M4_WARNING);
							}

							if (m_eClause == M4LDB_CLAUSE_COLUMN)
							{
								if (m_pvcAuxColItm->size () > 0)
								{
									// La longitud del anterior es la posición actual menos su posición.
									m4int32_t lLen = m_pstAuxCol->size () - m_pvcAuxColItm->back ()->m_lPos;

									if (lLen == 1)
									{
										DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_LOGIC_FIELD, (m4int32_t) m_pvcAuxColItm->size ());
										return (M4_ERROR);
									}

									m_pvcAuxColItm->back ()->m_lLength = lLen;

									if (bValidateType == M4_FALSE)
									{
										// Desactivamos la validación de tipos porque solo hay que realizarla cuando sólo hay un campo asociado al item.
										m_pvcAuxColItm->back ()->m_bValidateType = M4_FALSE;
									}
								}
							}
							else
							{
								if (m_pvcAuxColLngItm->size () > 0)
								{
									// La longitud del anterior es la posición actual menos su posición.
									m4int32_t lLen = m_pstAuxColLng->size () - m_pvcAuxColLngItm->back ()->m_lPos;

									if (lLen == 1)
									{
										DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_LOGIC_FIELD, (m4int32_t) m_pvcAuxColLngItm->size ());
										return (M4_ERROR);
									}

									m_pvcAuxColLngItm->back ()->m_lLength = lLen;

								}
							}
							m_bNoAddDtLastUpdate = (m_bNoAddDtLastUpdate == M4_TRUE) ? m_bNoAddDtLastUpdate : bNoAddDtLastUpdate;
							SetAuxVectors (M4LDB_CLAUSE_FROM);
							return (AnalizeFromClause (M4_TRUE, M4_TRUE));
						}
						else
						{
							if (!bInvColumn)
							{
								if (!strcmp (m_pAuxString, "PLUS_INF") || 
									!strcmp (m_pAuxString, "MINUS_INF") || 
									!strcmp (m_pAuxString, "M4_PLUS_INF") || 
									!strcmp (m_pAuxString, "M4_MINUS_INF"))
								{
									result = AnalizeSpecialParam (m_pAuxString);
									if (result == M4_ERROR)
									{
										return (M4_ERROR);
									}
									bValidateType = M4_FALSE;
								}
								else
								{
									if (!strcmp (m_pAuxString, "NULL"))
									{
										m_pstAux->append (" NULL");
										bValidateType = M4_FALSE;
									}
									else
									{
										DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, "'FROM'");
										return (M4_ERROR);
									}
								}
							}
						}
					}
					else
					{
						bIsWord = M4_FALSE;
					}
				}

				//Si la columna quedo invalida, tengo el handle del item, ahora voy a obtener el alias, todo lo metemos
				//en el mapa.

				/* Bug 0130591
				Se añaden al mapa todos los items, y no sólo los que no son de registro,
				ya que no se sabe si van a estar conectados o no
				*/
				if ( hItemCol != M4LDB_NULL_INDEX && !bIsWord )
				{
					if ( (*m_pAuxString != '#')
						&& (*m_pAuxString != '@')
						&& (*m_pAuxString != '$')
						&& (*m_pAuxString != '?')
						&& (*m_pAuxString != '\'')
						&& (*m_pAuxString != '`') 
						&& (!m_pMtSent->IsReal ()) )					
					{
						//Obtenemos la subcadena del alias!.
						//Si existe alias, buscamos la posición donde finaliza.

						m4pchar_t	pcPoint = strstr( m_pAuxString, "." ) ;

						if (pcPoint != NULL)
						{
							*pcPoint = '\0' ;
							m_oItemColAlias.insert( MapItemColAlias::value_type(hItemCol, m_pAuxString) );
							*pcPoint = '.' ;
						}
					}
				}

				if (!bIsWord && !bInvColumn)
				{
					//Si no se analiza campo hay que resetear valores;
					if ( (*m_pAuxString == '#')
						|| (*m_pAuxString == '@')
						|| (*m_pAuxString == '$')
						|| (*m_pAuxString == '?')
						|| (*m_pAuxString == '\'')
						|| (*m_pAuxString == '`') )					
					{
							ResetFlagsMultiOrganization ();
					}
					switch (*m_pAuxString)
					{
						case '#':
							result = AnalizeFunction (m_pAuxString);							
							if (result == M4_ERROR)
							{
								return (M4_ERROR);
							}
							
							cSeparator = ' ';
							bValidateType = M4_FALSE;
							break;							
							
						case '@':
							result = AnalizeItem (m_pAuxString);
							
							if (result == M4_ERROR)
							{
								return (M4_ERROR);
							}

							bValidateType = M4_FALSE;
							break;
							
						case '$':
							result = AnalizeSessionItem (m_pAuxString + 1, M4_TRUE);
							
							if (result == M4_ERROR)
							{
								return (M4_ERROR);
							}

							bValidateType = M4_FALSE;
							break;
							
						case '?':
							result = AnalizeArgument (cSeparator);
							
							if (result == M4_ERROR)
							{
								return (M4_ERROR);
							}
							
							cSeparator = ' ';
							bValidateType = M4_FALSE;
							break;
							
						case '\'': 
							AnalizeLiteral(m_pAuxString);
							bValidateType = M4_FALSE;
							break;

						case '`': 
							m_pstAux->append (" ");
							m_pstAux->append (m_pAuxString+1);
							cSeparator = ' ';
							bValidateType = M4_FALSE;
							break;

						default:
							if (!m_pMtSent->IsReal ())
							{
								result = AnalizeFieldOrConst (m_pAuxString, &bIsFld);
								
								if (result == M4_ERROR)
								{
									return (M4_ERROR);
								}

								if (bIsFld)
								{
									bNoAddDtLastUpdate = M4_FALSE;
								}
							}
							else
							{
								// Bug 0140959. En el caso de que estemos en SQL Server, verificamos 
								// si el último carácter de la sentencia es un corchete "[" y 
								// que el separador es un "]" y si es así, no añadimos el espacio.
								m4bool_t	bAddSpace = M4_TRUE ;
								
								if (m_pGlobVars->GetDDBBType() == M4DBW_SQL_SERVER_DDBB)
								{
									if( m_pstAux->empty() == M4_FALSE )
									{
										size_t iLen = m_pstAux->size();
										m4pcchar_t pccAux = m_pstAux->c_str();
										
										if (pccAux[iLen - 1] == '[' && cSeparator == ']')
										{
											bAddSpace = M4_FALSE;
										}
									}
								}
								
								if( bAddSpace == M4_TRUE )
								{
									m_pstAux->append (" ");
								}
								m_pstAux->append (m_pAuxString);
								bValidateType = M4_FALSE;
							}
							break;
					}
				}
			}

			switch (cSeparator)
			{
				case '(' :
					iInnerPar++;
					break;

				case ')' :
					iInnerPar--;
					break;
				
				case ',':
					if (!iInnerPar)
					{
						bEndColumn = M4_TRUE;
						continue;
					}
					break;
					
				case ';':
				case '\0' :
					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, "'FROM'");
					return (M4_ERROR);
					break;
					
				case ' ' :
					continue;
					break;
			}

			if (!bInvColumn)
			{
				m_pstAux->append (&cSeparator, 0, 1);
			}
		}

		if (!bInvColumn && 
			bValidateType == M4_FALSE && 
			m_eClause == M4LDB_CLAUSE_COLUMN && 
			m_pvcAuxColItm->size () > 0)
		{
			// Desactivamos la validación de tipos porque solo hay que realizarla cuando sólo hay un campo asociado al item.
			m_pvcAuxColItm->back ()->m_bValidateType = M4_FALSE;
		}
	}

	return (M4_ERROR);
}


m4return_t ClAPISQL::AnalizeFieldsClause (void)
{
	m4return_t result;
	m4char_t cSeparator;
	m4bool_t bIsWord;
	m4int32_t lenToken;
	m4bool_t bExit = M4_FALSE;

	eStmType_t iStmType = M4LDB_SELECT ;


	SetAuxVectors (M4LDB_CLAUSE_COLUMN);

	// Vamos a buscar los campos del Insert.
	while (!bExit)
	{
		result = _X_GetToken (m_pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal ());

		if (result != M4_ERROR)
		{
			if (bIsWord)
			{
				if (!strcmp (m_pAuxString, "VALUES"))
				{
					result = FindSeparator ('(');

					if (result == M4_ERROR)
					{
						return (M4_ERROR);
					}

					if (!m_pvcAuxFld->size ())
					{
						DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_LOGIC_FIELD_NUMBER);
						return (M4_ERROR);
					}

					return (AnalizeValuesClause ());
				}

				if (!strcmp (m_pAuxString, "SELECT"))
				{
					iStmType = m_pMtSent->GetStmType ();

					if ( iStmType == M4LDB_INSERT )
					{
						//Marcamos la sentencia como que es un INSERT INTO SELECT
						m_pMtSent->SetIsInsertIntoSelect(M4_TRUE);
					}
					else if ( iStmType == M4LDB_UPDATE )
					{
						//O un UPDATE SELECT
						m_pMtSent->SetIsUpdateSelect(M4_TRUE);
					}

					if (!m_pvcAuxFld->size ())
					{
						DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_LOGIC_FIELD_NUMBER);
						return (M4_ERROR);
					}

					SetAuxVectors (M4LDB_CLAUSE_REST);
					SetAuxVectors (M4LDB_CLAUSE_BEGIN_STATEM);
					return (AnalizeWhereClause ());
				}
			}

			m4bool_t bIsFld = M4_FALSE;

			result = AnalizeFieldOrConst (m_pAuxString, &bIsFld);
			ResetFlagsMultiOrganization ();

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}

			if (!bIsFld)
			{
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_LOGIC_FIELD, (m4int32_t) (m_pvcAuxFld->size () + 1));
				return (M4_ERROR);
			}
		}

		// Vamos a quitar los blancos.
		for (; ((!cSeparator) || IsControlCharacter(cSeparator)); m_pCurrent++)
		{
			cSeparator = *m_pCurrent;
		}

		switch (cSeparator)
		{
			case '(' :
			case ')' :
				break;

			case ',' :
				// Esto realmente es la columna.
				m_lLevel++;
				break;

			case ';':
			case '\0' :
				bExit = M4_TRUE;
				break;

			default :
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, "','");
				return (M4_ERROR);
				break;
		}
	}

	DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, "'VALUES'");
	return (M4_ERROR);
}


m4return_t ClAPISQL::AnalizeValuesClause (void)
{
	m4return_t result;					
	m4char_t cSeparator;
	m4bool_t bIsWord;
	m4int32_t lenToken;
	m4uint32_t numVal = 0;
	m4bool_t bExit = M4_FALSE;

	m_pstAuxWhr->erase ();

	// Vamos a buscar los valores del INSERT.
	while (!bExit)
	{
		result = _X_GetToken (m_pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal ());

		if (result != M4_ERROR)
		{
			switch (*m_pAuxString)
			{
				case '@':
					result = AnalizeItem (m_pAuxString);
					break;

				case '$':
					result = AnalizeSessionItem (m_pAuxString + 1, M4_TRUE);
					break;

				case '?':
					result = AnalizeArgument (cSeparator);
					cSeparator = ' ';
					break;

				default:
					if (!strcmp (m_pAuxString, "PLUS_INF") || 
						!strcmp (m_pAuxString, "MINUS_INF") || 
						!strcmp (m_pAuxString, "M4_PLUS_INF") || 
						!strcmp (m_pAuxString, "M4_MINUS_INF"))
					{
						result = AnalizeSpecialParam (m_pAuxString);
					}
					else
					{
						DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_ITEM, (m4int32_t) (m_pvcAuxPar->size () + 1));
						return (M4_ERROR);
					}
			}

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}

		switch (cSeparator)
		{
			case ';':
			case '\0' :
				bExit = M4_TRUE;
				break;

			case ')' :
			case ',' :
				numVal++;
				break;
		}
	}

	if (numVal != m_pvcAuxFld->size ())
	{
		DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_LOGIC_FIELD_NUMBER);
		return (M4_ERROR);
	}

	return (M4_SUCCESS);
}


m4return_t ClAPISQL::AnalizeFieldsAndValuesClause (void)
{
	m4return_t result;
	m4int32_t lenToken;
	m4char_t cSeparator;
	m4bool_t bIsWord;
	m4bool_t bExit1 = M4_FALSE;
	m4bool_t bExit2 = M4_FALSE;

	SetAuxVectors (M4LDB_CLAUSE_COLUMN);

	while (!bExit1)
	{
		bExit2 = M4_FALSE;

		m4bool_t bIsFld = M4_FALSE;

		result = _X_GetToken (m_pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal ());

		if (result != M4_ERROR)
		{
			if (*m_pAuxString != '#' && 
				*m_pAuxString != '@' && 
				*m_pAuxString != '$'&& 
				*m_pAuxString != '?')
			{
				result = AnalizeFieldOrConst (m_pAuxString, &bIsFld);
				ResetFlagsMultiOrganization ();

				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}
			}
		}

		if (!bIsFld)
		{
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_LOGIC_FIELD, (m4int32_t) (m_pvcAuxFld->size () + 1));
			return (M4_ERROR);
		}

		while ((!bExit1) && (!bExit2))
		{
			result = _X_GetToken (m_pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal ());

			if (result != M4_ERROR)
			{
				if (bIsWord)
				{
					if (!strcmp (m_pAuxString, "WHERE"))
					{
						SetAuxVectors (M4LDB_CLAUSE_WHERE);
						return (AnalizeWhereClause ());
					}
				}

				switch (*m_pAuxString)
				{
					case '@':
						result = AnalizeItem (m_pAuxString);
						break;

					case '$':
						result = AnalizeSessionItem (m_pAuxString + 1, M4_TRUE);
						break;

					case '?':
						result = AnalizeArgument (cSeparator);
						cSeparator = ' ';
						break;

					default:
						if (!strcmp (m_pAuxString, "PLUS_INF") || 
							!strcmp (m_pAuxString, "MINUS_INF") || 
							!strcmp (m_pAuxString, "M4_PLUS_INF") || 
							!strcmp (m_pAuxString, "M4_MINUS_INF"))
						{
							result = AnalizeSpecialParam (m_pAuxString);
						}
						else
						{
							DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_ITEM, (m4int32_t) (m_pvcAuxPar->size () + 1));
							return (M4_ERROR);
						}
				}

				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}
			}

			switch (cSeparator)
			{
				case ' ' :
				case '=' :
					break;

				case ';':
				case '\0' :
					bExit1 = M4_TRUE;
					break;

				case ',' :
					bExit2 = M4_TRUE;
					break;

				default :
					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, "','");
					return (M4_ERROR);
					break;

			}
		}
	}
		
	return (M4_SUCCESS);
}



m4return_t ClAPISQL::AnalizeMultiFilter (void)
{
	m4return_t result;
	m4char_t cSeparator;
	m4bool_t bIsWord;
	m4int32_t lenToken;

	if (!m_pstAuxWhr->empty ())
	{
		while (m_lLevel >= 0)
		{
			SetAuxVectors (M4LDB_CLAUSE_END_STATEM);
		}
	}

	m_lFilter++;

	//	El indice lo reseteo para cualquier filtro que no sea de tipo Order By. 
	//	( En este tipo de filtros no tengo FROM y no se cual es la base ).
	//m_indAlsBasis = M4LDB_NULL_INDEX;
	
	// Desactivamos el flag indicativo del filtro del conector.
	m_bConnActivate = M4_FALSE;

	do 
	{
		result = _X_GetToken (m_pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal ());
		if (*m_pAuxString == '^')
		{
			m_lInitLevel = atol (&m_pAuxString[1]);
			m_indAlsSec = _FindIdxAlias (m_pAuxString, m_indFilterSelect, m_lInitLevel, M4LDB_CLAUSE_WHERE);
			// Esto es para hacer que se repita.
			cSeparator = ';';
			result = M4_ERROR;
		}
	} while ((result == M4_ERROR) && (cSeparator == ';'));

	// ¿Qué es lo que es?
	if (bIsWord)
	{
		if (!strcmp (m_pAuxString, "SELECT"))
		{
			m_indFilterSelect = m_lFilter;
			m_indAlsBasis = M4LDB_NULL_INDEX;
			SetAuxVectors (M4LDB_CLAUSE_BEGIN_STATEM, m_lInitLevel);
			return (AnalizeColumnsClause ());
		} 
		else
		{
			if (!strcmp (m_pAuxString, "FROM"))
			{
				m_indAlsBasis = M4LDB_NULL_INDEX;
				SetAuxVectors (M4LDB_CLAUSE_FROM, m_lInitLevel);
				return (AnalizeFromClause (M4_TRUE, M4_TRUE));
			}
			else
			{
				if (!strcmp (m_pAuxString, "CFROM"))
				{ 
					// Estamos en un filtro del conector.
					m_bConnActivate = M4_TRUE;
					m_indAlsBasis = M4LDB_NULL_INDEX;
					SetAuxVectors (M4LDB_CLAUSE_FROM, m_lInitLevel);
					return (AnalizeFromClause (M4_TRUE, M4_TRUE));
				}
				else 
				{
					if (!strcmp (m_pAuxString, "ORDER"))
					{
						// Estamos en una sentencia de ORDER BY.
						_X_GetToken (m_pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal ());
						if (!strcmp (m_pAuxString, "BY"))
						{
							// Hay que poner la clausula FROM, para que al transferir los datos sepamos a que corresponde.
							SetAuxVectors (M4LDB_CLAUSE_FROM, 0);
							SetAuxVectors (M4LDB_CLAUSE_ORDER_BY, 0);
							return AnalizeOrderByWithItems();
						}
					}
				}
			}
		}
	}


	if (cSeparator != '\0')
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_APISQL, m_pAuxString);
		return (M4_ERROR);
	}

	return (M4_SUCCESS);
}


m4return_t ClAPISQL::AnalizeSQL (string &aio_stAPISQLStm, ClMetaSentence *ai_pMtSent, m4bool_t ai_bWithID, m4bool_t ai_bHasConnFilter, m4char_t ai_iApplyAutoFilters)
{

	m4return_t result;
	m4char_t cSeparator;
	m4bool_t bIsWord;
	m4int32_t lenToken;
	m4bool_t bAdd = M4_FALSE;
	m4bool_t bIsInsert = M4_FALSE;
	m4bool_t bExistsFilter = M4_FALSE;		// Controla si se han añadido filtros adicionales despues de haber hecho la transferencia.
	m4int32_t lLevel = 0;
	

	m_pstAPISQLStm = &aio_stAPISQLStm;

	m_pMtSent = ai_pMtSent;

	if (!m_pMtSent->IsReal ())
	{
		// Le añadimos el ';' para evitar problemas con al delimitar el ambito de los ORs.
		m_pstAPISQLStm->append (";");
	}

	m_pGlobVars = m_pMtSent->GetpGlobVars ();

	if (m_pAuxString)
	{
		delete m_pAuxString;
	}
	m_pAuxString = new char [m_pstAPISQLStm->size () * 2];

	// Esto lo hacemos antes de situar el current por si internamente volvemos a entrar en el analize (a traves del loaddictionary).
	m4return_t result2 = SetDDBBType ();
	// La validación del error la haremos más adelante.

	m_pCurrent = (m4pchar_t) m_pstAPISQLStm->c_str ();
	
	if (ai_bWithID)
	{
		// La primera palabra es el identificador de la sentencia y por tanto la descartamos.
		// Pero también puede ser el indicador del nivel inicial.
		result = _X_GetToken (m_pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal ());

		if (*m_pAuxString == '^')
		{
			m_lInitLevel = atol (&m_pAuxString[1]);
			lLevel = m_lInitLevel;
			m_indAlsSec = _FindIdxAlias (m_pAuxString, m_indFilterSelect, m_lInitLevel, M4LDB_CLAUSE_WHERE);
		}
		else
		{
			if (m_pMtSent->GetStmType () == M4LDB_SELECT)
			{
				// La única forma de llegar hasta aquí es a través de la Load, 
				// por lo que sabemos que tenemos que añadir conectores, dt_last_update, etc.
				bAdd = M4_TRUE;
			}
		}
	}

	if ((!m_pMtSent->IsReal ()) && (m_pvcAuxFrmObj))
	{
		m_pMtSent->GetObjectCopy (*m_pvcAuxFrmObj);
	}		

	m4pchar_t pStmWithoutIdSent = m_pCurrent;

	if (result2 == M4_ERROR)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_APISQL_SYNTAX_ERROR, m4uint32_t(m_pCurrent - pStmWithoutIdSent) << pStmWithoutIdSent);
		return (M4_ERROR);
	}

	if (CounterParenthesisAndQuotes (m_pCurrent) != M4_SUCCESS)
	{
		return M4_ERROR;
	}

	result = _X_GetToken (m_pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal ());
	
	if (result != M4_ERROR)
	{
		
		m_bConnActivate = M4_FALSE; 

		m_indAlsBasis = M4LDB_NULL_INDEX;

		switch (m_eApiSql)
		{
			case M4LDB_APISQL_NONE:
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_APISQL, "<null>");
				return (M4_ERROR);
				break;

			case M4LDB_APISQL_DELETE:
				if (!strcmp (m_pAuxString, "DELETE"))
				{
					SetAuxVectors (M4LDB_CLAUSE_BEGIN_STATEM, lLevel);
					SetAuxVectors (M4LDB_CLAUSE_FROM, lLevel);
					result = AnalizeFromClause (M4_FALSE, M4_TRUE);
				}
				else
				{
					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_APISQL, pStmWithoutIdSent);
					return (M4_ERROR);
				}
				break;

			case M4LDB_APISQL_INSERT:
				if (!strcmp (m_pAuxString, "INSERT"))
				{
					result = _X_GetToken (m_pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal ());
					if (result != M4_ERROR)
					{
						if (!strcmp (m_pAuxString, "INTO"))
						{
							bIsInsert = M4_TRUE;
							SetAuxVectors (M4LDB_CLAUSE_BEGIN_STATEM, lLevel);
							SetAuxVectors (M4LDB_CLAUSE_FROM, lLevel);
							result = AnalizeFromClause (M4_FALSE, M4_TRUE);
						}
						else
						{
							DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, "'INTO'");
							return (M4_ERROR);
						}
					}
					else
					{
						DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, "'INTO'");
						return (M4_ERROR);
					}
				}
				else
				{
					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_APISQL, pStmWithoutIdSent);
					return (M4_ERROR);
				}
				break;

			case M4LDB_APISQL_UPDATE:
				if (!strcmp (m_pAuxString, "UPDATE"))
				{
					SetAuxVectors (M4LDB_CLAUSE_BEGIN_STATEM, lLevel);
					SetAuxVectors (M4LDB_CLAUSE_FROM, lLevel);
					result = AnalizeFromClause (M4_FALSE, M4_TRUE);
				}
				else
				{
					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_APISQL, pStmWithoutIdSent);
					return (M4_ERROR);
				}
				break;

			case M4LDB_APISQL_SELECT:
				if (!strcmp (m_pAuxString, "SELECT"))
				{
					m_indFilterSelect = m_lFilter;
					SetAuxVectors (M4LDB_CLAUSE_BEGIN_STATEM, lLevel);
					result = AnalizeColumnsClause ();
				} 
				else
				{
					if (!strcmp (m_pAuxString, "FROM"))
					{
						SetAuxVectors (M4LDB_CLAUSE_FROM, lLevel);
						result = AnalizeFromClause (M4_TRUE, M4_TRUE);
					}
					else
					{
						if (!strcmp (m_pAuxString, "CFROM"))
						{
							// Estamos en un filtro del conector.
							SetAuxVectors (M4LDB_CLAUSE_FROM, lLevel);
							m_bConnActivate = M4_TRUE;
							result = AnalizeFromClause (M4_TRUE, M4_TRUE);
						}
						else
						{
							DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_APISQL, pStmWithoutIdSent);
							return (M4_ERROR);
						}
					}
				}
				break;

			case M4LDB_APISQL_REAL_SELECT:
				if (!strcmpi (m_pAuxString, "SELECT"))
				{
					SetAuxVectors (M4LDB_CLAUSE_BEGIN_STATEM, lLevel);
					result = AnalizeColumnsClause ();
				}
				else
				{
					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_APISQL, pStmWithoutIdSent);
					return (M4_ERROR);
				}
				break;

			case M4LDB_APISQL_REAL_STMT:
				SetAuxVectors (M4LDB_CLAUSE_FROM);
				SetAuxVectors (M4LDB_CLAUSE_WHERE, M4LDB_NULL_INDEX, M4_TRUE);

				if (strcmp (m_pAuxString, "CALL"))
				{
					// El call no lo añadimos.
					m_pstAux->append (m_pAuxString);
				}				
				result = AnalizeWhereClause ();
				break;
		}
		
		if (result == M4_WARNING)
		{
			return (M4_WARNING);
		}

		if (result == M4_ERROR)
		{
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_APISQL_SYNTAX_ERROR, m4uint32_t(m_pCurrent - pStmWithoutIdSent) << pStmWithoutIdSent);
			return (M4_ERROR);
		}
		
		if (!m_pstAuxWhr->empty ())
		{
			while (m_lLevel >= lLevel)
			{
				SetAuxVectors (M4LDB_CLAUSE_END_STATEM);
			}
		}

		if ((bAdd) && (ai_bHasConnFilter))
		{
			result = AddConnectorFilters ();
			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}

		if (!m_pMtSent->IsReal ())
		{
			result = AnalizeLogicElements (bAdd, bIsInsert);
			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}

		result = TransferMetaData (M4_TRUE, M4_TRUE);
		if (result == M4_ERROR)
		{
			return (M4_ERROR);
		}

		if ((bAdd) && (ai_iApplyAutoFilters != 0))
		{
			result = AddAutomaticFilters (ai_iApplyAutoFilters, bExistsFilter);
			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}

		result = AddSocFilters(bExistsFilter);
		if (result == M4_ERROR)
		{
			return (M4_ERROR);
		}

		result = AddTramitFilters(bExistsFilter);
		if (result == M4_ERROR)
		{
			return (M4_ERROR);
		}

		// Si hemos añadido algun filtro, volvemos a hacer la transferencia.
		if (bExistsFilter)
		{
			//Bug 0071469
			//Antes no hacía falta analizar objetos ni relaciones, pq la inserción de filtros
			//no añadía nuevas relaciones.
			//Ahora, al añadir filtros automáticos puede ocurrir que existan relaciones duplicadas
			//por lo tanto no pasamos el argumento opcional TransferMetaData (M4_FALSE).
			//Al pasar true se ajustarán objetos y relaciones.

			/* Bug 0124976
			Hay casos en los que no se quieren procesar los ors para añadir el filtro
			De momento no se tienen en cuenta para auto filtros y sociedad
			*/
			result = TransferMetaData (M4_TRUE, M4_FALSE);
			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}
		// Esto es por si tenemos que volver a entrar.
		m_lFilter++;

		return (M4_SUCCESS);
	}

	DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_APISQL, "<null>");
	return (M4_ERROR);
}


m4return_t ClAPISQL::AnalizeLogicElements (m4bool_t ai_bAdd, m4bool_t ai_bIsInsert)
{
	m4return_t result;
	m4int32_t indExtPar = 1;
	if (m_pvcAuxColPar)
	{
		for (itVcClMtParam it=m_pvcAuxColPar->begin (); it!=m_pvcAuxColPar->end(); it++)
		{
			if ((*it)->IsMetaData()==M4_TRUE && (*it)->GetType()==M4LDB_PAR_EXTERN)
			{
				((ClMetaExternPar *)(*it))->m_iParIndex=indExtPar;
				indExtPar++;
			}
		}
	}

	if (m_pvcAuxWhrPar)
	{
		for (itVcClMtParam it=m_pvcAuxWhrPar->begin (); it!=m_pvcAuxWhrPar->end(); it++)
		{
			if ((*it)->IsMetaData()==M4_TRUE && (*it)->GetType()==M4LDB_PAR_EXTERN)
			{
				((ClMetaExternPar *)(*it))->m_iParIndex=indExtPar;
				indExtPar++;
			}
		}
	}
	if (m_pvcAuxFrmRel && m_pvcAuxFrmRel->size())
	{
		RemoveOcurrencesOfR (m_pstAuxWhr);
	}

	ClMetaAlias *pMtAlias  = 0;
	m4int32_t nAlias = m_pMtSent->GetNumAlias();

	for (m4int32_t i=0; i<nAlias; ++i)
	{
		pMtAlias  = m_pMtSent->GetAliasByPos(i);
		if (pMtAlias->m_indObj == M4LDB_NULL_INDEX && !pMtAlias->m_bReduce)
		{
			m4pcchar_t pAlias = strchr (pMtAlias->m_stAlias.c_str (), '_') + 1;
			pAlias = strchr (pAlias, '_') + 1;
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_ALIAS, pAlias);
			return (M4_ERROR);
		}
	}
	// Antes de cargar el diccionario, me guardo la posicion del Current.
	m4int32_t CurrentPos = m_pCurrent - m_pstAPISQLStm->c_str();

	if (m_pvcAuxFrmObj)
	{
		ClMetaObject *pObj=0;
		for (itVcClMtObject it=m_pvcAuxFrmObj->begin (); it!=m_pvcAuxFrmObj->end(); it++)
		{
			pObj = (*it);
			result = m_pGlobVars->GetpLConn ()->LoadDictionaryChannel (pObj->m_stObject.c_str (), &(pObj->m_indLogicTable), M4_TRUE);

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
			// Se lee el OrgType y OrgLevelde cada objeto.
			pObj->m_iOrgType = M4LDB_NUMBER(m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Register[pObj->m_indLogicTable].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_ORG_TYPE].Value.GetWithOutSec());
			pObj->m_bInheritsData = M4LDB_BOOL(m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Register[pObj->m_indLogicTable].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_INHERITS_DATA].Value.GetWithOutSec());
			pObj->m_iOrgLevel = M4LDB_NUMBER(m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Register[pObj->m_indLogicTable].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_ORG_LEVEL].Value.GetWithOutSec());
			pObj->m_iMirrorType = M4LDB_NUMBER(m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Register[pObj->m_indLogicTable].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_MIRROR_TYPE].Value.GetWithOutSec());
		}
	}
	m_pCurrent = (m4pchar_t )m_pstAPISQLStm->c_str() + CurrentPos;

	if (m_pvcAuxColFld)
	{
		//Bug 0072475. Registro bloqueado en herencia.
		//Tenemos un problema a la hora de cargar una tabla hija. ClAPISQL::AddDTLastUpdate. Este miembro
		//actualiza el valor del registro al a fecha de actualizacion que encuentra en BD. De esta forma, en la carga
		//se marca el registro con la fecha de actualización.
		//En tablas hija no tenemos, de momento DT_LAST_UPDATE, por lo tanto, el registro se marca con un valor
		//de actualizacion de 0. Esto produce un error en la concurrencia.
		//Escenario:
		//Dos usuarios: U1 y U2 cargan el canal. El valor de DT_LAST_UPDATE es 0 pq no existe en la tabla heredada.
		//U1 modifica y graba, pero U2 puede modificar y tambien grabar -> Error.
		//A partir de entonces si U1 intenta grabar ya le dara el error, pq U2 tiene la tabla en propiedad
		//pero el caso inicial no sería correcto.
		//Metemos ahora DT_LAST_UPDATE, pero es puede ser necesario pasar despues por FindFieldInTheDicticionary.
		//Por ese motivo se hace ahora, pq puede haberse insertado el campo DT_LAST_UPDATE y hara falta meter
		//From y relaciones con tabla hija-padre. En realidad es hija con la ascendente que tenga DT_LAST_UPDATE.

		if (ai_bAdd && !m_bNoAddDtLastUpdate)
		{
			result = AddDTLastUpdate ();
			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		} 

		for (size_t iColFld=0; iColFld<m_pvcAuxColFld->size(); iColFld++)
		{
			result = FindFieldInTheDicticionary ((*m_pvcAuxColFld)[ iColFld ]);
			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}
	}

	if (m_pvcAuxColLngFld)
	{
		for (size_t iColLngFld=0; iColLngFld<m_pvcAuxColLngFld->size(); iColLngFld++)
		{
			result = FindFieldInTheDicticionary ((*m_pvcAuxColLngFld)[ iColLngFld ]);
			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}
	}

	if (m_pvcAuxWhrFld)
	{
		for (size_t iWhrField=0; iWhrField<m_pvcAuxWhrFld->size(); iWhrField++)
		{
			ClMetaField * pWhrFld = (*m_pvcAuxWhrFld)[ iWhrField ];
			if ((pWhrFld != NULL) && (pWhrFld->IsMetaData () == M4_TRUE))
			{
				result = FindFieldInTheDicticionary (pWhrFld);
				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}
			}
		}

		/* Bug 0115678
		Si estamos en una insert into select se reordenan los campos long para que vayan al final
		Se sacan todos los campos del vector y se guardan
		Se recorre el vector de campos y a partir de su tabla real y su campo real se obtiene
		el tipo extendido. A partir del tipo extendido se obtiene su tipo m4.
		Si es de tipo long o binary se guarda en un vector del longs y sino se va guardando
		en el definitivo
		También si es blob, ya que los blob se leen como binarios
		Luego se añaden los campos long al final a partir del vector de longs
		*/
		if( ai_bIsInsert == M4_TRUE )
		{
			m4bool_t			bFieldProcessed = M4_FALSE ;
			m4bool_t			bFieldFound = M4_FALSE ;
			m4bool_t			bIsLong = M4_FALSE ;
			size_t				i = 0 ;
			size_t				j = 0 ;
			size_t				iLength = 0 ;
			size_t				iLongs = 0 ;
			m4int8_t			iM4Type = 0 ;
			m4id_t				pccIdType = NULL ;
			m4int32_t			iIndType = M4LDB_NULL_INDEX ;
			VcClMtField			vcTemporal ;
			VcClMtField			vcLong ;
			ClMetaField			*pMtField = NULL ;
			ClMetaField			*pLongMtField = NULL ;
			ClRegister_Current	*pTableCurrent =  NULL ;
			ClRegister			*pFieldRegister =  NULL ;
			ClRegister			*pTypeRegister =  NULL ;


			pTableCurrent = &m_pGlobVars->GetpLConn()->m_paRS[ M4LDB_SYS_DICT_REAL_OBJECTS ]->Current ;
			pFieldRegister = &m_pGlobVars->GetpLConn()->m_paRS[ M4LDB_SYS_DICT_FIELDS ]->Register ;
			pTypeRegister = &m_pGlobVars->GetpLConn()->m_paRS[ M4LDB_SYS_DICT_EXTENDED_TYPES ]->Register ;

			iLength = m_pvcAuxWhrFld->size() ;

			vcTemporal = *m_pvcAuxWhrFld ;
			m_pvcAuxWhrFld->clear() ;
			m_pvcAuxWhrFld->reserve( iLength ) ;
			vcLong.reserve( iLength ) ;

			bFieldProcessed = M4_FALSE ;
			bFieldFound = M4_FALSE ;

			for( i = 0 ; i < iLength ; i++ )
			{
				pMtField = vcTemporal[ i ] ;

				if( bFieldProcessed == M4_FALSE && pMtField != NULL && pMtField->IsMetaData() == M4_TRUE )
				{
					bFieldFound = M4_TRUE ;

					pTableCurrent->MoveTo( pMtField->m_indRealTable ) ;
					pFieldRegister->MoveTo( pMtField->m_indRealField ) ;

					pccIdType = M4LDB_ID( pFieldRegister->Item[ M4LDB_ITEM_SYS_DICT_FIELDS_ID_TYPE ].Value.GetWithOutSec ());
					iIndType = m_pGlobVars->GetpLConn()->FindIndexById( M4LDB_SYS_DICT_EXTENDED_TYPES, pccIdType ) ;

					bIsLong = M4_FALSE ;
					if( iIndType != M4LDB_NULL_INDEX )
					{
						iM4Type = M4LDB_M4_TYPE( (m4int8_t) M4LDB_NUMBER( pTypeRegister->Item[ M4LDB_ITEM_SYS_DICT_EXTENDED_TYPES_ID_M4_TYPE ].Value.GetWithOutSec() ) ) ;

						if( iM4Type == M4CL_M4_TYPE_LONG || iM4Type == M4CL_M4_TYPE_UNI_LONG || iM4Type == M4CL_M4_TYPE_BLOB || iM4Type == M4CL_M4_TYPE_BINARY_STRING )
						{
							bIsLong = M4_TRUE ;
						}
					}

					if( bIsLong == M4_TRUE )
					{
						vcLong.push_back( pMtField ) ;
					}
					else
					{
						m_pvcAuxWhrFld->push_back( pMtField ) ;
					}
				}
				else
				{
					if( bFieldFound == M4_TRUE )
					{
						// Si no es un campo si hay longs pendientes se añaden y después el field que viene
						iLongs = vcLong.size() ;

						for( j = 0 ; j < iLongs ; j++ )
						{
							pLongMtField = vcLong[ j ] ;
							m_pvcAuxWhrFld->push_back( pLongMtField ) ;
						}
						vcLong.clear() ;

						bFieldFound = M4_FALSE ;
						bFieldProcessed = M4_TRUE ;
					}

					m_pvcAuxWhrFld->push_back( pMtField ) ;
				}
			}
		}
	}

	if (m_pvcAuxFrmRel)
	{
		//Bug 0099705
		for (size_t iRelation=0; iRelation<m_pvcAuxFrmRel->size(); iRelation++)
		{
			result = FindFieldInTheDicticionary ((*m_pvcAuxFrmRel)[ iRelation ]->m_pMtField1);
			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
			result = FindFieldInTheDicticionary ((*m_pvcAuxFrmRel)[ iRelation ]->m_pMtField2);
			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClAPISQL::FindIndexFieldwoAlias (ClMetaField *ai_pMtField)
{
	m4return_t result = M4_ERROR;
	m4bool_t bOk = M4_FALSE;
	ClMetaObject *pMtAuxObj=0;
	ClMetaObject *pMtObj=0;
		
	// Iteramos por todos los objetos del filtro y nivel del campo.
	m4int32_t nObjects = GetNumLgcTbl ();
	// Puede ser que hayamos pasado los objetos al MetaSentence.

	for (m4int32_t index = nObjects-1;index>=0;index--)
	{
		pMtAuxObj = GetLgcTblByPos (index);

		bOk = M4_FALSE;
		if (ai_pMtField->m_iFilter == pMtAuxObj->m_iFilter)
		{
			// Si es columna, solo comparamos los filter ( deben de ser 0, pero por si acaso ).
			// Si no deben coincidir tambien los niveles.
			if (ai_pMtField->m_eClause == M4LDB_CLAUSE_COLUMN || 
				ai_pMtField->m_eClause == M4LDB_CLAUSE_LONG_COLUMN)
			{
				bOk= M4_TRUE;
			}
			else
			{
				if (ai_pMtField->m_iLevel == pMtAuxObj->m_iLevel)
				{
					bOk= M4_TRUE;
				}
			}

			if (bOk)
			{
				ai_pMtField->m_indLogicTable = pMtAuxObj->m_indLogicTable;

				if (ai_pMtField->m_indLogicTable == M4LDB_NULL_INDEX)
				{
					return (M4_ERROR);
				}

				result = FindIndexField (ai_pMtField, M4_FALSE, index);

				if (result == M4_SUCCESS)
				{
					if (ai_pMtField->m_indLogicTable != pMtAuxObj->m_indLogicTable)
					{
						// El campo lo hemos encontrado buscando en la herencia.

						if ((ai_pMtField->m_eClause == M4LDB_CLAUSE_COLUMN) && 
							((m_pMtSent->GetStmType () == M4LDB_INSERT) || 
							(m_pMtSent->GetStmType () == M4LDB_UPDATE)))
						{
							// No es necesario seguir, asi que nos vamos.
							return (M4_SUCCESS);
						}

						// Si la tabla no estaba añadida anteriormente, ahora se habrá añadido.
						// Volvemos a buscar.
						nObjects = GetNumLgcTbl ();
						index = nObjects;
						//Bug 0081842
						//El warning se da cuando tenemos un campo y en el from hay varios objetos con ese campo.					
						//Antes estábamos dando un warning incorrecto en el siguiente supuesto:
						//		El problema es debido a que en la select primero tenemos un campo que solo esta
						//		en la padre, y eso produce que se meta un join hijo-padre (añadiendo la tabla padre al from).
						//		Despues se mete un campo que está en los dos pe la PK y dabamos el warning.

						//Solución adoptada:
						//Cada objeto tendrá dos punteros:
						//Un puntero al objeto padre. Ese objeto padre se habrá añadido para encontrar un campo.
						//Un segundo puntero que lo tendrá el objeto padre y apuntará al hijo. Con él un objeto padre
						//sabrá qué objeto hijo es el que provocó su creación.
						//Con los dos punteros tendré una lista doblemente enlazada!.

						//Como volvemos a iterar, se resetea el valor.
						pMtObj = NULL;
						continue;
					}

					if (pMtObj)
					{
						//Vamos a comprobar si son tablas que están en una misma cadena de herencia.
						//Si pertenecen a la misma cadena en la herencia significa que la hija metió a la padre.
						//Si están en la cadena de herencia y el campo es PK no tenemos que mostrar el warning pq
						//la tabla padre se ha añadido para encontrar un campo de la select y si es PK no considero
						//ambigüedad.
						m4bool_t bShowWarning = M4_TRUE;
						m4bool_t bIsPK = M4_FALSE;
						//Vamos a ver si es PK.
						if (ai_pMtField->m_indRealField != M4LDB_NULL_INDEX)
						{
							bIsPK = ((M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Current[ai_pMtField->m_indRealField].Item[M4LDB_ITEM_SYS_DICT_FIELDS_POS_PK].Value.GetWithOutSec ())) ? M4_TRUE : M4_FALSE);
						}

						//Buscamos en ascendentes y en descendentes.
						//Lo tipico es que pMtObj sea el padre y pMtAuxObj sea hijo, por lo tanto el destino (pMtAuxObj) se encontrará
						//en el camino de herencia en los descendientes del origen (pMtObj).
						//Decimos que es lo tipico pq en caso de haberse añadido por herencia estara al final
						//del vector y el ultimo es el primero que se visita.
						if ((ObjectsIntoInheritanceChain (pMtObj, pMtAuxObj, M4LDB_SEARCH_TYPE_INTO_INHERITANCE_TABLE_FIND_BOTH) == M4_TRUE) && (bIsPK == M4_TRUE))
						{
							bShowWarning = M4_FALSE;
						}

						if (bShowWarning == M4_TRUE)
						{
							DUMP_LDBINSP_WARNINGF (m_pGlobVars->GetpLConn (),M4_CH_LDB_AMBIGOUS_COLUMN_REFERENCE, ai_pMtField->m_stField.c_str () << pMtObj->m_stObject.c_str () << pMtAuxObj->m_stObject.c_str ());						
						}
					}

					pMtObj = pMtAuxObj;
				}
			}
		}
	}

	if (!pMtObj)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (),M4_CH_LDB_FIELD_NOT_FOUND, ai_pMtField->m_stField.c_str ());
		return (M4_ERROR);
	}

	ai_pMtField->m_indLogicTable = pMtObj->m_indLogicTable;

	ai_pMtField->m_indAlias = pMtObj->m_indAlias;

	if (ai_pMtField->m_indRealTable != M4LDB_NULL_INDEX)
	{
		pMtObj->NotifyUsingRealTable (ai_pMtField->m_indRealTable);
	}

	if (ai_pMtField->m_indRealTable2 != M4LDB_NULL_INDEX)
	{
		pMtObj->NotifyUsingRealTable (ai_pMtField->m_indRealTable2);
	}

	return (M4_SUCCESS);
}


m4bool_t ClAPISQL::ObjectsIntoInheritanceChain( ClMetaObject * ai_pSourceObject, ClMetaObject * ai_pDestinationObject, eSearchTypeIntoInheritanceTable_t ai_iSearchTypeIntoInheritanceTable )
{
	//Podremos buscar en herencia ascendente, en descendente o en ambas (por defecto).
	
	//Partiendo del objeto origen vamos a ver si podemos llegar al destino.
	//Bien por herencia ascendente, es decir, destination puede ser el padre, abuelo,... de source.
	//bien por herencia descendiente, es decir, destination será el hijo, nieto,... de source.
	//o bien buscamos por los dos caminos.
	if ( ai_pSourceObject == NULL || ai_pDestinationObject == NULL )
	{
		return M4_FALSE;
	}

	//Si el objeto es el mismo interpretamos que uno pertenece a su propia cadena de herencia.
	if (ai_pSourceObject == ai_pDestinationObject )
	{
		return M4_TRUE;
	}

	ClMetaObject * pCurrent = NULL;

	//------------------------------------------------
	//Busqueda en ascendentes.
	if (ai_iSearchTypeIntoInheritanceTable == M4LDB_SEARCH_TYPE_INTO_INHERITANCE_TABLE_FIND_BOTH 
		|| ai_iSearchTypeIntoInheritanceTable == M4LDB_SEARCH_TYPE_INTO_INHERITANCE_TABLE_FIND_PARENT )
	{
		//Buscamos primero de hijo a padre, es decir, si destination es padre, abuelo,... de source
		pCurrent = ai_pSourceObject;	
		while (pCurrent != NULL)
		{
			if (pCurrent == ai_pDestinationObject)
			{
				return M4_TRUE;
			}
			
			pCurrent = pCurrent->m_pParentObject;
		}
	}
	//------------------------------------------------
	
	//------------------------------------------------
	//Busqueda en descendientes.
	if (ai_iSearchTypeIntoInheritanceTable == M4LDB_SEARCH_TYPE_INTO_INHERITANCE_TABLE_FIND_BOTH 
		|| ai_iSearchTypeIntoInheritanceTable == M4LDB_SEARCH_TYPE_INTO_INHERITANCE_TABLE_FIND_CHILD )
	{

		//Buscamos padre a hijo, es decir, si destination es hijo, nieto,... de source.
		//Esto realiza una busqueda en un arbol, desencadena una recursion.!!
		return ai_pSourceObject->MetaObjectIntoInheritanceChain( ai_pDestinationObject );
	}
	//------------------------------------------------

	return M4_FALSE;
}


m4return_t ClAPISQL::FindFieldInTheDicticionary (ClMetaField *ai_pMtField)
{
	m4return_t result;

	if (ai_pMtField->m_indAlias == M4LDB_NULL_INDEX)
	{
		result = FindIndexFieldwoAlias (ai_pMtField);

		if (result == M4_ERROR)
		{
			return (M4_ERROR);
		}
	}
	else
	{
		m4int32_t indObj = m_pMtSent->GetAliasByPos(ai_pMtField->m_indAlias)->m_indObj;
		// Si viene de añadir conectores, los objetos los tiene el MetaSentence.
		ClMetaObject *pLgcTbl = GetLgcTblByPos (indObj);

		ai_pMtField->m_indLogicTable = pLgcTbl->m_indLogicTable;

		result = FindIndexField (ai_pMtField, M4_TRUE, indObj);

		if (result == M4_ERROR)
		{
			return (M4_ERROR);
		}

		if (ai_pMtField->m_indRealTable != M4LDB_NULL_INDEX)
		{
			pLgcTbl->NotifyUsingRealTable (ai_pMtField->m_indRealTable);
		}

		if (ai_pMtField->m_indRealTable2 != M4LDB_NULL_INDEX)
		{
			pLgcTbl->NotifyUsingRealTable (ai_pMtField->m_indRealTable2);
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClAPISQL::_FindIndexField (m4id_t ai_idField, m4int32_t ai_indLgcTbl, m4int32_t &ao_indFld, m4int32_t &ao_indRealTbl)
{
	m4return_t result = M4_ERROR;

	// Situamos el current del nodo SYS_DICT_LOGIC_OBJECTS en el registro indicado por el indice.
	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[(ClRegisterIndex) ai_indLgcTbl];

	// Obtenemos el número de registros (tablas físicas) que tiene el nodo SYS_DICT_REAL_OBJECTS.
	m4int32_t numRealTbl = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Count ();


	m4int32_t indFld = M4LDB_NULL_INDEX;

	for (m4int32_t i=0; i<numRealTbl; i++)
	{
		// Situamos el current del nodo SYS_DICT_REAL_OBJECTS en el registro actual.
		m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[(ClRegisterIndex) i];

		// Buscamos el indice del registro para el idField dado.
		indFld = m_pGlobVars->GetpLConn ()->FindIndexById (M4LDB_SYS_DICT_FIELDS, ai_idField);

		if (indFld != M4LDB_NULL_INDEX)
		{
			ao_indFld=indFld;
			ao_indRealTbl=i;
			return (M4_SUCCESS);
		}
	}

	return (M4_ERROR);
}


m4return_t ClAPISQL::FindIndexField (ClMetaField *ai_pMtField, m4bool_t ai_bShowError, m4int32_t ai_posLgcTbl)
{
	m4return_t result = _FindIndexField (ai_pMtField->m_stField.c_str (), ai_pMtField->m_indLogicTable, ai_pMtField->m_indRealField, ai_pMtField->m_indRealTable);

	if (result == M4_SUCCESS)
	{
		return (result);
	}

	// No lo hemos encontrado. ¿será un campo traducido?
	ClRegisterIndex indTrans = m_pGlobVars->GetpLConn ()->FindIndexById (M4LDB_SYS_DICT_TRANS, ai_pMtField->m_stField.c_str ());

	if (indTrans != M4LDB_NULL_INDEX)
	{
		ClRegisterIndex indTrnFld(0);

		m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_TRANS]->Current[indTrans];

		m4id_t idDefFld = NULL;
		m4uint32_t numTrnFld = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_TRANS_FIELDS]->Count ();

		m4uint32_t lEnvLang = m_pGlobVars->GetLanguage();
		m4uint32_t lDefLangSystem = m_pGlobVars->GetpLConn()->m_pLdb->GetDefaultLanguage();
		
		//Bug 0077224
		//El for para cada uno de los idiomas por los que se ha traducido el campo.
		m4bool_t bDefaultLanguageIsGenericLanguage = M4_FALSE;

		for (indTrnFld=0; indTrnFld<numTrnFld; indTrnFld++)
		{
			ClRegister *pReg = &m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_TRANS_FIELDS]->Register[indTrnFld];

			m4uint32_t lLanguage = M4LDB_NUMBER (pReg->Item[M4LDB_ITEM_SYS_DICT_TRANS_FIELDS_ID_LANG].Value.GetWithOutSec ());

			if (lLanguage == lEnvLang)
			{
				// Tiene traducción para ese idioma.
				m4id_t idTrnFld = M4LDB_ID (pReg->Item[M4LDB_ITEM_SYS_DICT_TRANS_FIELDS_ID_TRANS_FIELD].Value.GetWithOutSec ());

				if (!strcmpi (idTrnFld, ai_pMtField->m_stField.c_str ()))
				{
					if (ai_bShowError)
					{
						m4id_t idLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
						DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_TRANSLATION, ai_pMtField->m_stField.c_str () << idLgcTbl);
					}
					return (M4_ERROR);
				}

				result = _FindIndexField (idTrnFld, ai_pMtField->m_indLogicTable, ai_pMtField->m_indRealField, ai_pMtField->m_indRealTable);

				if (result == M4_ERROR)
				{
					if (ai_bShowError)
					{
						m4id_t idLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
						DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_LOGIC_FIELD, idTrnFld << idLgcTbl);
					}
					return (M4_ERROR);
				}
			}
			else
			{
				if (lLanguage == M4LDB_DEFAULT_LANGUAGE)
				{
					// Tiene traducción por defecto.
					idDefFld = M4LDB_ID (pReg->Item[M4LDB_ITEM_SYS_DICT_TRANS_FIELDS_ID_TRANS_FIELD].Value.GetWithOutSec ());
					//Bug 0077224
					//Si en BD tenemos registro para el idioma 1 es el de por defecto que definitivamente se queda.
					bDefaultLanguageIsGenericLanguage = M4_TRUE;
				}
				else
				{
					//Si ya se ha establecido el idioma al generico es pq tenemos un registro para el idioma 1
					//y aunque haya otro por defecto nos quedaremos con el generico.
					if (lLanguage == lDefLangSystem && bDefaultLanguageIsGenericLanguage == M4_FALSE)
					{
						// Tiene traducción por defecto del sistema.
						idDefFld = M4LDB_ID (pReg->Item[M4LDB_ITEM_SYS_DICT_TRANS_FIELDS_ID_TRANS_FIELD].Value.GetWithOutSec ());
					}
				}
			}
		}
		
		if (idDefFld && m_pGlobVars->HasNotValidation( M4LDB_MULTITRANSLATING ) == M4_FALSE)
		{
			if (!strcmpi (idDefFld, ai_pMtField->m_stField.c_str ()))
			{
				if (ai_bShowError)
				{
					m4id_t idLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INVALID_TRANSLATION, ai_pMtField->m_stField.c_str () << idLgcTbl);
				}
				return (M4_ERROR);
			}

			if (result == M4_SUCCESS)
			{
				result = _FindIndexField (idDefFld, ai_pMtField->m_indLogicTable, ai_pMtField->m_indRealField2, ai_pMtField->m_indRealTable2);

				if (result == M4_ERROR)
				{
					m4id_t idLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_LOGIC_FIELD, idDefFld << idLgcTbl);
					return (M4_ERROR);
				}

				if (ai_pMtField->m_indRealField == ai_pMtField->m_indRealField2 && 
					ai_pMtField->m_indRealTable == ai_pMtField->m_indRealTable2	)
				{
					ai_pMtField->m_indRealField2 = M4LDB_NULL_INDEX;
					ai_pMtField->m_indRealTable2 = M4LDB_NULL_INDEX;
				}
			}
			else
			{
				result = _FindIndexField (idDefFld, ai_pMtField->m_indLogicTable, ai_pMtField->m_indRealField, ai_pMtField->m_indRealTable);

				if (result == M4_ERROR)
				{
					if (ai_bShowError)
					{
						m4id_t idLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
						DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_LOGIC_FIELD, idDefFld << idLgcTbl);
					}
					return (M4_ERROR);
				}
			}
		}
	}

	if (result == M4_ERROR)
	{
		result = FindIntoInheritance (ai_pMtField, ai_bShowError, ai_posLgcTbl);
	}

	if (result == M4_ERROR)
	{
		if (ai_bShowError)
		{
			m4id_t idLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_LOGIC_FIELD, ai_pMtField->m_stField.c_str () << idLgcTbl);
		}
	}

	return (result);
}


m4return_t ClAPISQL::FindIntoInheritance (ClMetaField *ai_pMtField, m4bool_t ai_bShowError, m4int32_t ai_posLgcTbl)
{
	m4return_t result = M4_ERROR;

	m4bool_t bHasInheritance = M4LDB_BOOL (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_HAS_INHERITANCE].Value.GetWithOutSec ());

	if (bHasInheritance)
	{
		if (ai_pMtField->m_indRealField != M4LDB_NULL_INDEX)
		{
			// Ya ha sido encontrado antes (seguro que no tenía alias, verdad) así que nos vamos.
			return (M4_ERROR);
		}

		// Nos situamos en la tabla padre. Como tenemos recursividad con FindIndexField, no es necesario investigar en el resto (ya lo haremos cuando estemos en la padre).
		m4id_t idParentLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_INHERITANCE]->Current[(ClRegisterIndex) 0].Item[M4LDB_ITEM_SYS_DICT_INHERITANCE_ID_AUX_OBJECT].Value.GetWithOutSec ());

		m4int32_t indParentLogicTable = M4LDB_NULL_INDEX; 

		result = m_pGlobVars->GetpLConn ()->LoadDictionaryChannel (idParentLgcTbl, &indParentLogicTable, M4_TRUE);

		m4int32_t iOrgType = M4LDB_NUMBER(m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Register[indParentLogicTable].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_ORG_TYPE].Value.Get());
		m4bool_t bInheritsData = M4LDB_BOOL(m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Register[indParentLogicTable].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_INHERITS_DATA].Value.GetWithOutSec());
		m4int32_t iOrgLevel = M4LDB_NUMBER(m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Register[indParentLogicTable].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_ORG_LEVEL].Value.Get());
		m4uint8_t iMirrorType = M4LDB_NUMBER(m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Register[indParentLogicTable].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_MIRROR_TYPE].Value.GetWithOutSec());

		if (result == M4_ERROR)
		{
			return (M4_ERROR);
		}

		if (ai_pMtField->m_indLogicTable == indParentLogicTable)
		{
			// Es la misma tabla asi que pasamos de ella.
			return (M4_ERROR);
		}

		ai_pMtField->m_indLogicTable = indParentLogicTable;

		result = FindIndexField (ai_pMtField, ai_bShowError, ai_posLgcTbl);

		if (result == M4_SUCCESS && ai_pMtField->m_indLogicTable == indParentLogicTable)
		{
			// Lo he encontrado en el actual objeto lógico.

			if ((ai_pMtField->m_eClause == M4LDB_CLAUSE_COLUMN) && 
				((m_pMtSent->GetStmType () == M4LDB_INSERT) || 
				(m_pMtSent->GetStmType () == M4LDB_UPDATE)))
			{
				// No es necesario seguir, asi que nos vamos.
				return (M4_SUCCESS);
			}

			ClMetaObject *pLgcTbl1 = NULL;
			ClMetaObject *pLgcTbl2 = GetLgcTblByPos (ai_posLgcTbl);

			m4int32_t iAuxFilter = m_lFilter;
			
			m_lFilter = ai_pMtField->m_iFilter;

			if (ai_pMtField->m_eClause == M4LDB_CLAUSE_COLUMN || 
				ai_pMtField->m_eClause == M4LDB_CLAUSE_LONG_COLUMN)
			{
				m_lLevel = m_lInitLevel;
			}
			else
			{
				m_lLevel = ai_pMtField->m_iLevel;
			}
			
			SetAuxVectors (M4LDB_CLAUSE_FROM, m_lLevel);

			// Iteramos por todos los objetos del filtro y nivel del campo.
			m4int32_t numObj = GetNumLgcTbl ();

			// Buscamos si ya está en el filtro y nivel del campo.
			for (m4int32_t indObj=0; indObj<numObj; indObj++)
			{
				pLgcTbl1 = GetLgcTblByPos (indObj);
				if (pLgcTbl1->m_iFilter == m_lFilter &&
					pLgcTbl1->m_iLevel == m_lLevel &&
					pLgcTbl1->m_indLogicTable == ai_pMtField->m_indLogicTable)
				{
					break;
				}
				pLgcTbl1 = NULL;
			}

			if (!pLgcTbl1)
			{
				// No ha sido encontrado. Lo añadimos.

				pLgcTbl1 = new ClMetaObject (m_lLevel, m_eClause, idParentLgcTbl, M4_FALSE, m_lFilter);

				//Bug 0081842
				//Enlazamos la herencia.
				pLgcTbl1->AddChildObject(pLgcTbl2);
				pLgcTbl2->m_pParentObject = pLgcTbl1;

				pLgcTbl1->m_indLogicTable = ai_pMtField->m_indLogicTable;

				/* Bug 53433 No se estaba actualizando el idOrganization en las tablas heredadas.*/
				pLgcTbl1->m_iOrgType = iOrgType;
				pLgcTbl1->m_bInheritsData = bInheritsData;
				pLgcTbl1->m_iOrgLevel = iOrgLevel;
				pLgcTbl1->m_iMirrorType = iMirrorType;

				result = _CreateAlias (pLgcTbl1, M4_FALSE);

				if (result == M4_ERROR)
				{
					return (M4_ERROR);
				}
			}

			pLgcTbl1->NotifyUsingRealTable (ai_pMtField->m_indRealTable);

			/* Bug 0164192
			Si es traducido, la tabla del idioma por defecto también se está usando
			*/
			if (ai_pMtField->m_indRealTable2 != M4LDB_NULL_INDEX)
			{
				pLgcTbl1->NotifyUsingRealTable (ai_pMtField->m_indRealTable2);
			}

			ai_pMtField->m_indAlias = pLgcTbl1->m_indAlias;

			SetAuxVectors (M4LDB_CLAUSE_WHERE, m_lLevel, M4_TRUE);
				
			// Añadimos la relación por las PKs (da igual que esté duplicada porque en el AnalizeRelations se va ha reducir).
			m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[(ClRegisterIndex) pLgcTbl1->m_indLogicTable];
			
			m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[(ClRegisterIndex) 0];
			
			m4int32_t numFld = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Count ();

			for (m4int32_t indFld=0; indFld<numFld; indFld++)
			{
				m4bool_t bIsPK = ((M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Current[indFld].Item[M4LDB_ITEM_SYS_DICT_FIELDS_POS_PK].Value.GetWithOutSec ())) ? M4_TRUE : M4_FALSE);
				
				if (bIsPK)
				{
					// Obtenemos el nombre lógico del campo
					m4id_t idField = M4LDB_PCSTR (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Current[indFld].Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_FIELD].Value.GetWithOutSec ());

					// Buscamos que exista en la segunda.
					m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[(ClRegisterIndex) pLgcTbl2->m_indLogicTable];
					
					m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[(ClRegisterIndex) 0];

					m4int32_t indFld2 = m_pGlobVars->GetpLConn ()->FindIndexById (M4LDB_SYS_DICT_FIELDS, idField);

					if (indFld2 == M4LDB_NULL_INDEX)
					{
						DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_LOGIC_FIELD, idField << pLgcTbl2->m_stObject.c_str ());
						return (M4_ERROR);
					}

					// Nos resituamos.
					m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[(ClRegisterIndex) pLgcTbl1->m_indLogicTable];
					
					m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[(ClRegisterIndex) 0];

					ClMetaField *pMtFld1 = new ClMetaField (m_lLevel, M4LDB_CLAUSE_WHERE, idField, pLgcTbl1->m_indAlias, m_lFilter);

					ClMetaField *pMtFld2 = new ClMetaField (m_lLevel, M4LDB_CLAUSE_WHERE, idField, pLgcTbl2->m_indAlias, m_lFilter);
					
					ClMetaRelat *pstRel = new ClMetaRelat (m_lLevel, M4LDB_CLAUSE_WHERE, pMtFld1, pMtFld2, M4LDB_NORMAL_JOIN);

					//-------------------------------------------
					//Bug 0071469
					//Para herencia: El campo que se realiza por join.

					//Objeto recien creado.
					pMtFld1->m_indLogicTable = pLgcTbl1->m_indLogicTable;
					pMtFld1->m_indRealField = indFld;
					//Las PK siempre estan en la primera de las tablas == ai_pMtField->m_indRealTable
					pMtFld1->m_indRealTable = 0;

					//Objeto del que se parte a buscar.
					pMtFld2->m_indLogicTable = pLgcTbl2->m_indLogicTable;
					pMtFld2->m_indRealField = indFld2;
					//Las PK siempre estan en la primera de las tablas == ai_pMtField->m_indRealTable
					pMtFld2->m_indRealTable = 0;

					//Se acaba de construir el join A1.Pk = A2.Pk, el problema viene cuando los alias
					//por los que yo estoy haciendo el join tienen un objeto aociado que no va a salir en el
					//From, porque la máscara real del objeto es 0 -> No se considera para la composición de
					//la sentencia real.
					//Estaríamos haciendo FROM A1, A3 WHERE A1.pk=A2.pk And A1.ci=A3.cj
					//El join de Pk lo estoy haciendo con A2 que no sale en el From y al componer la sentencia
					//real me dara un error.

					//Un objeto lógico puede tener varias tablas físicas asociadas.
					//Con el código anterior, sobre el objeto creado pLgcTbl1 (por defecto tiene la máscara a 1),
					//se hace el join por PK.
					//Ojo con pLgcTbl2->m_MaskRealTables, pq si es 0 estaríamos haciendo un join
					//con algo que no esta en el from.				
					//-------------------------------------------

					//Pedro. Cuando introduzcamos la relación en el vector, indicamos el tipo de APISQL.
					pstRel->SetAPISQLSource(GetAPISQLSource());							
					m_pvcAuxFrmRel->push_back(pstRel);

					//-------------------------------------------
					//Marcamos los objetos tambien, es util para las relaciones dummy.
					ClMetaObject *pObjectAPISQLSource = 0;
					m4int32_t indAPISQLSource = M4LDB_NULL_INDEX;

					//Bug 0078863
					//En API la información está en el vector auxiliar de from.					
					indAPISQLSource = m_pMtSent->GetObjectPosByAliasPos(pstRel->m_pMtField1->m_indAlias);											
					if (indAPISQLSource != M4LDB_NULL_INDEX && m_pvcAuxFrmObj && m_pvcAuxFrmObj->empty() == M4_FALSE )
					{
						pObjectAPISQLSource = (*m_pvcAuxFrmObj)[indAPISQLSource];
						if (pObjectAPISQLSource)
						{
							pObjectAPISQLSource->SetAPISQLSource(GetAPISQLSource());
						}
					}

					indAPISQLSource = m_pMtSent->GetObjectPosByAliasPos(pstRel->m_pMtField2->m_indAlias);											
					if (indAPISQLSource != M4LDB_NULL_INDEX && m_pvcAuxFrmObj && m_pvcAuxFrmObj->empty() == M4_FALSE )
					{
						pObjectAPISQLSource = (*m_pvcAuxFrmObj)[indAPISQLSource];
						if (pObjectAPISQLSource)
						{
							pObjectAPISQLSource->SetAPISQLSource(GetAPISQLSource());
						}
					}
					//-------------------------------------------
				}
			}

			SetAuxVectors (M4LDB_CLAUSE_END_STATEM);

			m_lFilter = iAuxFilter;
		}
	}

	return (result);
}


m4return_t ClAPISQL::AddDTLastUpdate (void)
{
	// Vamos a comprobar si utilizamos fecha de última actualización.
	// Para ello es imprescindible que la tabla de escritura sea la base de la select de lectura.
	m4id_t idWriteLgcTbl = m_pGlobVars->GetWriteIdObject ();

	if (!*idWriteLgcTbl)
	{
		return (M4_SUCCESS);
	}

	ClMetaObject *pLgcTbl = (*m_pvcAuxFrmObj)[m_pMtSent->GetAliasByPos(m_indAlsBasis)->m_indObj];

	m4id_t idBasisLgcTbl = pLgcTbl->m_stObject.c_str ();

	// Situamos el current del nodo SYS_DICT_LOGIC_OBJECTS en el registro indicado por el indice.
	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[(ClRegisterIndex) pLgcTbl->m_indLogicTable];

	// Situamos el current del nodo SYS_DICT_REAL_OBJECTS en la primera tabla.
	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[(ClRegisterIndex) 0];

	// Buscamos el indice del campo DT_LAST_UPDATE.
	m4int32_t indDTLastUpdate = m_pGlobVars->GetpLConn ()->FindIndexByIdInternalField (M4LDB_DT_LAST_UPDATE);
	
	if (indDTLastUpdate != M4LDB_NULL_INDEX)
	{
		//Bug 0113872
		if (strcmpi (idWriteLgcTbl, idBasisLgcTbl))
		{
			return (M4_SUCCESS);
		}

		m4int32_t lColumn = m_pvcAuxColItm->size ();

		ClMetaItemCol *pMtItem = new ClMetaItemCol (lColumn, M4LDB_CLAUSE_COLUMN, M4LDB_NULL_INDEX);

		pMtItem->m_lPos = m_pstAuxCol->size ();

		if (lColumn > 1)
		{
			// La longitud del anterior es la posición del nuevo menos su posición.
			m_pvcAuxColItm->back ()->m_lLength = pMtItem->m_lPos - m_pvcAuxColItm->back ()->m_lPos;
		}

		m_pstAuxCol->append (", ~F");

		pMtItem->m_lLength = m_pstAuxCol->size () - pMtItem->m_lPos;

		m_pvcAuxColItm->push_back (pMtItem);

		m4id_t idField = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[(ClRegisterIndex) indDTLastUpdate].Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_FIELD].Value.GetWithOutSec ());

		ClMetaField *pMtFld = new ClMetaField (lColumn, M4LDB_CLAUSE_COLUMN, idField, m_indAlsBasis, m_indFilterSelect);

		m_pMtSent->SetIndDtLastUpdate(lColumn);

		pMtFld->m_indRealField = indDTLastUpdate;

		pMtFld->m_indRealTable = 0;

		pMtFld->m_indLogicTable = pLgcTbl->m_indLogicTable;

		pLgcTbl->NotifyUsingRealTable (pMtFld->m_indRealTable);

		m_pvcAuxColFld->push_back (pMtFld);
	}

	//-------------------------
	//Bug 0072475. Registro bloqueado en herencia.
	//Si no lo ha encontrado en la tabla base, lo buscaremos en la herencia.
	else
	{
		m4int32_t indParentLogicTable = M4LDB_NULL_INDEX;
		m4id_t idParentLgcTbl;
		m4return_t result;
		m4int32_t iInheritance;		

		//Vamos a buscar en la herencia.
		m4bool_t bHasInheritance = M4LDB_BOOL (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_HAS_INHERITANCE].Value.GetWithOutSec ());

		if (bHasInheritance)
		{
			m4int32_t nParents = m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_INHERITANCE]->Current.Count();
			for (iInheritance = 0; iInheritance < nParents; iInheritance ++)
			{				
				//Es un padre, abuelo,...
				m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_INHERITANCE]->Current[(ClRegisterIndex) iInheritance];
				
				idParentLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_INHERITANCE]->Current[(ClRegisterIndex) iInheritance].Item[M4LDB_ITEM_SYS_DICT_INHERITANCE_ID_AUX_OBJECT].Value.GetWithOutSec ());

				//Cargamos el diccionario del padre
				result = m_pGlobVars->GetpLConn ()->LoadDictionaryChannel (idParentLgcTbl, &indParentLogicTable, M4_TRUE);
				if (result == M4_ERROR || indParentLogicTable == M4LDB_NULL_INDEX)
				{
					return M4_ERROR;
				}

				//Ahora buscamos el campo en la padre!!.
				m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[(ClRegisterIndex) indParentLogicTable];
				m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[(ClRegisterIndex) 0];

				//Buscamos el campo por tipo interno, pero esta vez estoy en el padre.
				//Esta función devuelve el índice del field!!.
				indDTLastUpdate = m_pGlobVars->GetpLConn ()->FindIndexByIdInternalField (M4LDB_DT_LAST_UPDATE);

				//El campo no lo hemos encontrado en el padre, buscamos en el abuelo...
				//Iterando de nuevo en el bucle.
				if (indDTLastUpdate == M4LDB_NULL_INDEX)
				{
					//---------------------
					//Cada iteración, el diccionario debe quedar como estaba inicialmente.
					//Ademas, en la salida del bucle tambien debe dejarse todo como estaba inicialmente.
					m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[(ClRegisterIndex) pLgcTbl->m_indLogicTable];
					if (result == M4_ERROR)
					{
						return M4_ERROR;
					}						
					m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[(ClRegisterIndex) 0];
					//---------------------
					continue;
				}
				else
				{
					//El campo se ha encontrado en la herencia!!.
					break;
				}

			}	//for (iInheritance = 0; iInheritance < nParentsChildren; iInheritance++)

		}	//if (bHasInheritance) {

		//Hemos encontrado el campo en la herencia
		if (indDTLastUpdate != M4LDB_NULL_INDEX)
		{

			m4int32_t lColumn = m_pvcAuxColItm->size ();

			ClMetaItemCol *pMtItem = new ClMetaItemCol (lColumn, M4LDB_CLAUSE_COLUMN, M4LDB_NULL_INDEX);

			pMtItem->m_lPos = m_pstAuxCol->size ();

			if (lColumn > 1)
			{
				// La longitud del anterior es la posición del nuevo menos su posición.
				m_pvcAuxColItm->back ()->m_lLength = pMtItem->m_lPos - m_pvcAuxColItm->back ()->m_lPos;
			}

			m_pstAuxCol->append (", ~F");

			pMtItem->m_lLength = m_pstAuxCol->size () - pMtItem->m_lPos;

			m_pvcAuxColItm->push_back (pMtItem);

			m4id_t idField = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[(ClRegisterIndex) indDTLastUpdate].Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_FIELD].Value.GetWithOutSec ());
			
			ClMetaField *pMtFld = new ClMetaField (lColumn, M4LDB_CLAUSE_COLUMN, idField, m_indAlsBasis, m_indFilterSelect);

			/* Bug 0124491
			El índice debe ser el de la tabla base
			No hay que ponerlo a 0
			pMtFld->m_indAlias = 0;
			*/
			pMtFld->m_indLogicTable = pLgcTbl->m_indLogicTable;
			pMtFld->m_indRealField = M4LDB_NULL_INDEX;
			pMtFld->m_indRealTable = M4LDB_NULL_INDEX;
			
			m_pMtSent->SetIndDtLastUpdate(lColumn);

			pLgcTbl->NotifyUsingRealTable (pMtFld->m_indRealTable);

			m_pvcAuxColFld->push_back (pMtFld);
		}

		//Dejamos todo como estaba
		// Situamos el current del nodo SYS_DICT_LOGIC_OBJECTS en el registro indicado por el indice.
		m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[(ClRegisterIndex) pLgcTbl->m_indLogicTable];
		// Situamos el current del nodo SYS_DICT_REAL_OBJECTS en la primera tabla.
		m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[(ClRegisterIndex) 0];
	}
	//Fin Bug 0072475. Registro bloqueado en herencia.
		

	return (M4_SUCCESS);
}


m4return_t ClAPISQL::AddConnectorFilters (void)
{
	if (m_pMtSent->IsReal())
	{
		return M4_SUCCESS;
	}
	
	m4bool_t bFirst = M4_TRUE;

	if (!m_pGlobVars->GetpAccessRecordSet ())
	{
		return (M4_SUCCESS);
	}

	m4int32_t numItem = m_pGlobVars->GetNumItem ();

	for (m4int32_t indItem=0; indItem<numItem; indItem++)
	{
		m4int32_t hItem = m_pGlobVars->GetItemHandleByPos (indItem);

		if (!hItem)
		{
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClAPISQL::AddConnectorFilters" << "Null item handle in column = " << LogCat << (m4int32_t) indItem);
			return (M4_ERROR);
		}

		m4id_t idField = m_pGlobVars->GetItemReadIdField (hItem);

		m4int32_t hItemInvConector = m_pGlobVars->GetItemInverseConnector (hItem);

		m4bool_t bIsConnected = m_pGlobVars->GetpAccessRecordSet ()->Register.Item[(ClHandle)hItem].GetpConnectItemInfo ()->IsConnected ();

		if ((*idField) && 
			((hItemInvConector) || 
			(bIsConnected)))
		{
			m4int8_t cType = m_pGlobVars->GetItemType (hItem);

			if (cType == M4CL_ITEM_TYPE_METHOD)
			{
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_ITEM_IS_METHOD, hItem << idField);
				return (M4_ERROR);
			}

			if (bFirst)
			{
				SetAuxVectors (M4LDB_CLAUSE_FROM, m_lInitLevel);
				SetAuxVectors (M4LDB_CLAUSE_WHERE, m_lInitLevel, M4_TRUE);
				bFirst = M4_FALSE;
			}
			m4int32_t indAlias =M4LDB_NULL_INDEX;
			m4pcchar_t pAlias = M4LDB_PCSTR(m_pGlobVars->GetItemReadAlias (hItem));
			
			if ((*pAlias)!=0)
			{
				// Si tiene Alias en la TI, tendría que haber venido en la sentencia.
				sprintf (m_pAuxString, "^%ld_%ld_%s", m_lInitLevel, m_indFilterSelect, pAlias);

				indAlias = _FindIdxAlias (m_pAuxString, m_indFilterSelect, m_lInitLevel, M4LDB_CLAUSE_WHERE);

				if (indAlias == M4LDB_NULL_INDEX)
				{
					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_WRONG_ALIAS, pAlias);
					return (M4_ERROR);
				}
			}
			else
			{
				// Para resolver el bug 60170 le asociamos la tabla base.
				indAlias = m_indAlsBasis;	

				//Bug 0089536
				//Buscamos el handle en el mapa y obtendremos el alias.
				if (hItem != M4LDB_NULL_INDEX)
				{
					m4int32_t indAliasCol = M4LDB_NULL_INDEX;
					itMapItemColAlias it;

					it = m_oItemColAlias.find( hItem );
					if (it != m_oItemColAlias.end() )
					{
						sprintf (m_pAuxString, "^%ld_%ld_%s", m_lInitLevel, m_indFilterSelect, (it->second).c_str() );
						indAliasCol = _FindIdxAlias (m_pAuxString, m_indFilterSelect, m_lInitLevel, M4LDB_CLAUSE_WHERE);

						if (indAliasCol != M4LDB_NULL_INDEX)
						{
							indAlias = indAliasCol;
						}
					}
				}
			}

			ClMetaField *pMtFld = new ClMetaField (m_lInitLevel, M4LDB_CLAUSE_WHERE, idField, indAlias, m_indFilterSelect);

			m4int32_t indObj = m_pMtSent->GetObjectPosByAliasPos(pMtFld->m_indAlias);
			
			ClMetaObject *pLgcTbl = GetLgcTblByPos(indObj);

			if (pLgcTbl->m_eJoinType == M4LDB_OUTER_JOIN)
			{
				pMtFld->m_bIsOuter = M4_TRUE;
			}

			m_pvcAuxFld->push_back (pMtFld);
			
			ClMetaItem *pMtPar = new ClMetaItem (m_lInitLevel, M4LDB_CLAUSE_WHERE, hItem);

			m_pvcAuxPar->push_back (pMtPar);

			m4int32_t iConnRelation = M4CL_ITEM_CONNECTOR_EQUALITY;

			if (hItemInvConector != M4LDB_NULL_INDEX)
			{
				// Chapucilla para salir al paso en los carriles.
				iConnRelation = m_pGlobVars->GetItemConnectorRelation (hItemInvConector);
			}	

			m_pstAux->append (" AND ~F");

			switch (iConnRelation)
			{
				case (M4CL_ITEM_CONNECTOR_BIGGER):
					m_pstAux->append (" > ?");
					break;
				case (M4CL_ITEM_CONNECTOR_SMALLER):
					m_pstAux->append (" < ?");
					break;
				case (M4CL_ITEM_CONNECTOR_BIGGER_OR_EQUAL):
					m_pstAux->append (" >= ?");
					break;
				case (M4CL_ITEM_CONNECTOR_SMALLER_OR_EQUAL):
					m_pstAux->append (" <= ?");
					break;
				case (M4CL_ITEM_CONNECTOR_DISTINCT):
					m_pstAux->append (" <> ?");
					break;
				case (M4CL_ITEM_CONNECTOR_EQUALITY):
				default:
					m_pstAux->append (" = ?");
					break;
			}
		}
	}

	if (!bFirst)
	{
		SetAuxVectors (M4LDB_CLAUSE_END_STATEM, m_lInitLevel);
	}

	return (M4_SUCCESS);
}


m4return_t ClAPISQL::AddFilterByInternalType (eFldType_t ai_eFldType, ClMetaObject *ai_pLgcObj, m4bool_t ai_bAutomaticFilters)
{
	m4int8_t iM4Type = M4CL_M4_TYPE_NULL;

	//Bug 0071469
	//Tambien se añade un argumento opcinal donde indicaremos la insercion de autofiltros.
	if (AddFieldByInternalField(ai_eFldType,ai_pLgcObj, NULL, ai_bAutomaticFilters, &iM4Type)!=M4_SUCCESS)
	{
		return M4_ERROR;
	}

	//Bug 0069558.
	//A pesar de que el objeto tenga OJ, el campo lo dejamos a inner. La razón es pq se va a hacer
	//Campo = valor en la parte where, es decir, estamos buscando un valor concreto del campo, por lo
	//tanto el OJ no tiene sentido ya que si no es ese valor, el registro no tiene que traerse.
	//Bug 0071803.
	//Volvemos a descomentar el código. Para que no se corte el OJ.
	if (ai_pLgcObj->m_eJoinType == M4LDB_OUTER_JOIN)
	{
		m_pvcAuxFld->back()->m_bIsOuter = M4_TRUE;
	}
	
	m4int32_t	type = M4LDB_NORMAL ;

	switch (ai_eFldType)
	{
		case M4LDB_DT_START:
			type = M4CL_INTERNAL_TYPE_FILTER_END_DATE;
			break;

		case M4LDB_DT_END:
			type = M4CL_INTERNAL_TYPE_FILTER_START_DATE;
			break;

		case M4LDB_DT_START_CORR:
			type = M4CL_INTERNAL_TYPE_FILTER_COR_END_DATE;
			break;

		case M4LDB_DT_END_CORR:
			type = M4CL_INTERNAL_TYPE_FILTER_COR_START_DATE;
			break;

		case M4LDB_ID_ORGANIZATION:
			type = M4CL_INTERNAL_TYPE_ID_ORGANIZATION;
			iM4Type = M4LDB_M4_UNICODE_TYPE( M4CL_M4_TYPE_FIXED_STRING );
			break;

		case M4LDB_ID_ORG_LEVEL:
			type = M4CL_INTERNAL_TYPE_ID_ORGANIZATION_LEVEL;
			iM4Type = M4CL_M4_TYPE_NUMBER;
			break;

		case M4LDB_ID_DATASET:
			type = M4CL_INTERNAL_TYPE_ID_DATASET;
			iM4Type = M4LDB_M4_UNICODE_TYPE( M4CL_M4_TYPE_FIXED_STRING );
			break;
	}

	ClMetaAutoFilter *pMtPar = new ClMetaAutoFilter (ai_pLgcObj->m_iLevel, M4LDB_CLAUSE_WHERE, type, iM4Type, ai_pLgcObj->m_iOrgLevel, ai_pLgcObj->m_bInheritsData);

	m_pvcAuxPar->push_back (pMtPar);
	
	return (M4_SUCCESS);
}


m4return_t ClAPISQL::AddAutomaticFilters (m4char_t ai_iApplyAutoFilters, m4bool_t &ao_bExistsFilter)
{
	if (m_pMtSent->IsReal())
	{
		return M4_SUCCESS;
	}

	if (ai_iApplyAutoFilters == 0)
	{
		return M4_SUCCESS;
	}

	m4bool_t bBase = M4_FALSE;
	m4bool_t bAddBase = M4_TRUE;
	m4int32_t indObj=0;
	m4return_t result;

	m4int32_t numObj = m_pMtSent->GetNumObject ();

	// Bug 0291152 Se mira a ver si ya se ha añadido la tabla padre de la base y no hay que poner filtro a la hija
	for (indObj=0; indObj<numObj; indObj++)
	{
		ClMetaObject *pLgcTbl = m_pMtSent->GetObjectByPos (indObj);

		if (pLgcTbl->m_bIsBasis && pLgcTbl->m_pParentObject != NULL)
		{
			bAddBase = M4_FALSE;
			break;
		}
	}
	
	for (indObj=0; indObj<numObj; indObj++)
	{
		ClMetaObject *pLgcTbl = m_pMtSent->GetObjectByPos (indObj);

		if (pLgcTbl->m_bIsBasis)
		{
			if (bBase || bAddBase == M4_FALSE)
			{
				// La tabla base ya ha sido añadida o no hay que añadirla
				continue;
			}
			else
			{
				bBase = M4_TRUE;
			}
		}

		// Bug 0291152 No se añaden filtros a las tablas hijas que ya tienen tabla padre
		if (pLgcTbl->m_pParentObject != NULL)
		{
			continue;
		}

		//-----------------------------------------------------------------
		//Bug 0071469
		//Miembro modificado para que la construcción del filtro automático 
		//busque en las tablas ascendentes de la herencia.
		//-----------------------------------------------------------------

		//Si no hay tabla real asociada, no me preocupo en componer filtros!!.
		if (pLgcTbl->m_MaskRealTables == 0)
		{
			continue;
		}

		//Obtenemos el id de la tabla para mostrarla en el ldbinsp.
		// Bug 0285327 Se toma del propio objeto, porque el diccionario puede no estar posicionado (por ejemplo en tablas heredadas)
		m4id_t idLgcTbl = pLgcTbl->m_stObject.c_str();

		// Situamos el current del nodo SYS_DICT_LOGIC_OBJECTS en dicho registro.
		m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[(ClRegisterIndex) pLgcTbl->m_indLogicTable];

		eTempType_t eCurTemp = (eTempType_t) M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_TEMPORARY].Value.GetWithOutSec ());

		if ((eCurTemp != M4LDB_ETHERN) && 
			((ai_iApplyAutoFilters & M4CL_NODE_AUTO_FILTER_APPLICATION) != 0))
		{

			#ifdef _M4LDB_DEBUG
				M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\t# Adding autofilter (historic). Logic object %s.", idLgcTbl);
			#endif

			//---------------------
			//El tercer parametro indica que se añade filtro automatico, esto producirá una búsqueda del campo
			//en la herencia padre, abuelo...
			result = AddFilterByInternalType (M4LDB_DT_START, pLgcTbl, M4_TRUE);

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}

			m_pstAux->append (" AND ~F <= ? ");
			SetAuxVectors (M4LDB_CLAUSE_END_STATEM, pLgcTbl->m_iLevel);
			//---------------------

			//---------------------
			result = AddFilterByInternalType (M4LDB_DT_END, pLgcTbl, M4_TRUE);

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}

			m_pstAux->append (" AND ~F >= ?");
			SetAuxVectors (M4LDB_CLAUSE_END_STATEM, pLgcTbl->m_iLevel);
			//---------------------

			ao_bExistsFilter = M4_TRUE;
		}

		m4bool_t bCorrected = M4LDB_BOOL (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_CORRECTED].Value.GetWithOutSec ());

		if ((bCorrected) && 
			((ai_iApplyAutoFilters & M4CL_NODE_AUTO_FILTER_CORRECTION) != 0))
		{
			#ifdef _M4LDB_DEBUG
				M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\t# Adding autofilter (correction). Logic object %s.", idLgcTbl);
			#endif

			//---------------------
			result = AddFilterByInternalType (M4LDB_DT_START_CORR, pLgcTbl, M4_TRUE);

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}

			m_pstAux->append (" AND ~F <= ? ");
			SetAuxVectors (M4LDB_CLAUSE_END_STATEM, pLgcTbl->m_iLevel);
			//---------------------

			//---------------------
			result = AddFilterByInternalType (M4LDB_DT_END_CORR, pLgcTbl, M4_TRUE);

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}

			m_pstAux->append (" AND ~F >= ?");
			SetAuxVectors (M4LDB_CLAUSE_END_STATEM, pLgcTbl->m_iLevel);
			//---------------------

			ao_bExistsFilter = M4_TRUE;
		}
	}

	//Vamos a pasar el auxiliar from al from y limpiamos el auxiliar.
	itVcClMtObject itFrmS = m_pvcAuxFrmObj->begin();
	itVcClMtObject itFrmE = m_pvcAuxFrmObj->end();
	m_pMtSent->m_pvcFrmObj->insert(m_pMtSent->m_pvcFrmObj->begin(), itFrmS, itFrmE);				
	m_pvcAuxFrmObj->erase(itFrmS, itFrmE);

	return (M4_SUCCESS);
}


m4return_t ClAPISQL::AddSocFilters		(m4bool_t &ao_bExistsFilter)
{
	
	if (m_pMtSent->IsReal())
	{
		return M4_SUCCESS;
	}

	/////////////////////////////////////////
	// Para el Insert, update y el Insert into Select y el update select
	m4bool_t bMarkAsAdded = M4_FALSE;

	if (m_pMtSent->GetStmType()==M4LDB_INSERT)
	{
		if (m_pMtSent->GetIsInsertIntoSelect() == M4_FALSE)
		{
			// Si es Insert y no es IntoSelect no añado nada.
			return M4_SUCCESS;
		}

		// Si es insert into select no se ponen filtros en la principal
		bMarkAsAdded = M4_TRUE ;
	}
	else if (m_pMtSent->GetStmType()==M4LDB_UPDATE && m_pMtSent->GetIsUpdateSelect() == M4_TRUE)
	{
		// Si es update select no se ponen filtros en la principal
		bMarkAsAdded = M4_TRUE ;
	}

	if( bMarkAsAdded == M4_TRUE )
	{
		// Insert into Select, update select.
		// Marco los objeto de nivel 0 como que tienen el filtro añadido. En estos objetos no se debe añadir filtro, porque es un INSERT.
		m4int32_t nObjects = m_pMtSent->GetNumObject();
		ClMetaObject *pObject=0;
		for (m4int32_t i=0;i<nObjects;++i)
		{
			pObject = m_pMtSent->GetObjectByPos(i);
			if (pObject->m_iLevel>0)
			{
				break;
			}
			else
			{
				pObject->m_bAddedSocControlFilter = M4_TRUE;
				pObject->m_bAddedSocFilter = M4_TRUE;
				pObject->m_bAddedLevelFilter = M4_TRUE;
			}
		}
	}
	/////////////////////////////////////////

	ClChannel * pChannel = m_pGlobVars->GetpAccessRecordSet()->GetpAccess()->GetpChannel();

	m4VariantType vOrganizationType;
	if (pChannel->OrganizationType.Get(vOrganizationType)!=M4_SUCCESS)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClAPISQL::AddSocFilters" << "Cannot get Soc_Type from Channel" << LogCat << (m4int32_t) vOrganizationType.Data.DoubleData);
		return M4_ERROR;
	}
	if (vOrganizationType.Type!=M4CL_CPP_TYPE_NUMBER)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClAPISQL::AddConnectorFilters" << "Wrong Type in OrganizationType in Channel : %d" << LogCat << vOrganizationType.Type);
		return (M4_ERROR);
	}

	m4int32_t iOrgType = (m4int32_t)vOrganizationType.Data.DoubleData;

	// Si el canal es mono o multi se añaden los filtros por nivel de tablas de herencia
	if( iOrgType != M4CL_ORGANIZATION_TYPE_NONE )
	{
		m4return_t	iResult = AddFilterOrgLevel( ao_bExistsFilter ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	m4uint8_t iAccessLevel = m_pGlobVars->GetpLConn()->GetpLdb()->GetMultiOrgAccessLevel();

	if( iAccessLevel > 0 )
	{
		m4uint32_t	iOwnerFlag = pChannel->GetpCMCR()->GetChannelOwnerFlag();

		if( iOrgType != M4CL_ORGANIZATION_TYPE_NONE || iOwnerFlag != 1 )
		{
			m4return_t	iResult = AddFilterControlSoc( iAccessLevel, iOrgType, ao_bExistsFilter );

			if( iResult != M4_SUCCESS )
			{
				return( M4_ERROR ) ;
			}
		}
	}

	switch (iOrgType)
	{
		case M4CL_ORGANIZATION_TYPE_MONO:
			return AddFilterChannelMonoSoc(ao_bExistsFilter);		// AND ID_ORGANIZATION = ?

		case M4CL_ORGANIZATION_TYPE_MULTI:
			return AddFilterChannelMultiSoc(ao_bExistsFilter);

		case M4CL_ORGANIZATION_TYPE_NONE:
		default:
			return M4_SUCCESS;
			break;
	}

	// Por si acaso.
	DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClAPISQL::AddSocFilters" << "Wrong Type in OrganizationType in Channel : %d" << LogCat << iOrgType);
	return M4_ERROR;
}


m4return_t	ClAPISQL::AddFilterOrgLevel( m4bool_t &ao_bExistsFilter )
{
	// Si las tablas son de herencia de datos añadimos el filtro AND ID_ORG_LEVEL = ?
	// Recorro el vector de alias, añadiendo el filtro para todos los necesarios.

	ClMetaAlias *pAlias = 0;
	ClMetaObject *pLgcTbl = 0;
	m4int32_t	nAlias = m_pMtSent->GetNumAlias();
	m4bool_t bBase = M4_FALSE;

	for (m4int32_t i=0;i<nAlias;++i)
	{
		pAlias = m_pMtSent->GetAliasByPos(i);
		if (pAlias->m_bReduce == M4_FALSE)
		{
			pLgcTbl = m_pMtSent->GetObjectByPos(pAlias->m_indObj);

			if (pLgcTbl->m_bIsBasis)
			{
				if (bBase)
				{
					// La tabla base ya ha sido añadida.
					continue;
				}
				else
				{
					bBase = M4_TRUE;
				}
			}

			if (pLgcTbl->m_bInheritsData == M4_TRUE)
			{
				if (pLgcTbl->m_bAddedLevelFilter)
				{
					// Ya esta añadido. Estamos en el filtro añadido por seguridad, pero ya se añadió a la TI.
					continue;
				}

				// Añadimos el filtro.
				SetAuxVectors (M4LDB_CLAUSE_FROM, pLgcTbl->m_iLevel);
				SetAuxVectors (M4LDB_CLAUSE_WHERE, pLgcTbl->m_iLevel, M4_TRUE);
				
				m4char_t aAlias[M4LDB_SIZE_DEFAULT_STRING];
				sprintf (aAlias, "~A_%ld", pLgcTbl->m_indAlias);
				m_pstAux->append (aAlias);

				if (AddFilterByInternalType (M4LDB_ID_ORG_LEVEL, pLgcTbl)!=M4_SUCCESS)
				{
					// El error sale dentro
					return M4_ERROR;
				}

				m_pstAux->append (" AND ~F = ?");

				SetAuxVectors (M4LDB_CLAUSE_END_STATEM, pLgcTbl->m_iLevel);
				ao_bExistsFilter = M4_TRUE;
				pLgcTbl->m_bAddedLevelFilter = M4_TRUE;
			}
		}
	}
	return M4_SUCCESS;

}


m4return_t ClAPISQL::AddFilterControlSoc(m4uint8_t ai_iAccessLevel, m4int8_t ai_iOrgType, m4bool_t &ao_bExistsFilter)
{
	m4id_t	pccRole = m_pGlobVars->GetIdSecRole();

	if (pccRole == NULL || !strcmpi (pccRole, M4CL_SEC_SUPER_USER))
	{
		// SuperRol.
		return M4_SUCCESS;
	}

	ClMetaAlias *pAlias = 0;
	ClMetaObject *pLgcTbl = 0;
	m4int32_t	nAlias = m_pMtSent->GetNumAlias();
	m4bool_t bBase = M4_FALSE;

	for (m4int32_t i=0;i<nAlias;++i)
	{
		pAlias = m_pMtSent->GetAliasByPos(i);
		if (pAlias->m_bReduce == M4_FALSE)
		{
			pLgcTbl = m_pMtSent->GetObjectByPos(pAlias->m_indObj);

			if (pLgcTbl->m_bIsBasis)
			{
				if (bBase)
				{
					// La tabla base ya ha sido añadida.
					continue;
				}
				else
				{
					bBase = M4_TRUE;
				}
			}

			if (pLgcTbl->m_bAddedSocControlFilter)
			{
				continue;
			}

			pLgcTbl->m_bAddedSocControlFilter = M4_TRUE ;

			// Situamos Logic Objects.
			m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[(ClRegisterIndex) pLgcTbl->m_indLogicTable];

			ClAccessRecordSet	*poRecordSet = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_ORG_FIELDS];

			m4bool_t	bOrganizationFound = M4_FALSE;
			m4uint32_t	j = 0;
			m4uint32_t	iLength = poRecordSet->Count();

			for( j = 0; j < iLength; j++ )
			{
				poRecordSet->Current[j];

				m4bool_t	bHasParent = M4LDB_NUMBER( poRecordSet->Current.Item[M4LDB_ITEM_SYS_DICT_ORG_FIELDS_HAS_PARENT].Value.GetWithOutSec() ) ? M4_TRUE : M4_FALSE;

				if( bHasParent == M4_TRUE )
				{
					// Si es un campo sociedad hija de otro campo se salta
					continue;
				}

				if( bOrganizationFound == M4_FALSE && ai_iOrgType != M4CL_ORGANIZATION_TYPE_NONE && pLgcTbl->m_iOrgType == M4LDB_MULTI_ORGANIZATION )
				{
					eFldType_t	eIntFld = (eFldType_t) M4LDB_NUMBER( poRecordSet->Current.Item[M4LDB_ITEM_SYS_DICT_ORG_FIELDS_ID_INTERNAL_FIELD].Value.GetWithOutSec() );

					if( eIntFld == M4LDB_ID_ORGANIZATION )
					{
						// Se saltan los campos sociedad ya tratados. Primer campo sociedad (tipo 64 de tablas multi) de canales mono y multi
						bOrganizationFound = M4_TRUE;
						continue;
					}
				}

				m4id_t		idField = M4LDB_ID( poRecordSet->Current.Item[M4LDB_ITEM_SYS_DICT_ORG_FIELDS_ID_FIELD].Value.GetWithOutSec() );
				m4bool_t	bIsPK = M4LDB_NUMBER( poRecordSet->Current.Item[M4LDB_ITEM_SYS_DICT_ORG_FIELDS_POS_PK].Value.GetWithOutSec() ) ? M4_TRUE : M4_FALSE;

				if( bIsPK == M4_FALSE && IsFieldInUse( idField, i ) == M4_FALSE )
				{
					// Si no es PK y no se está usando se salta
					continue;
				}

				if( IsFieldDominatedInJoin( idField, i ) == M4_TRUE )
				{
					// Si está dominado en un join se salta
					continue;
				}

				ClAccessRecordSet	*poChildRecordSet = m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_CHILD_ORG_FIELDS];

				m4bool_t	bChildDominated = M4_FALSE;
				m4uint32_t	k = 0;
				m4uint32_t	iChildLength = poChildRecordSet->Count();

				for( k = 0; k < iChildLength; k++ )
				{
					m4id_t		idChildField = M4LDB_ID( poChildRecordSet->Register[k].Item[M4LDB_ITEM_SYS_DICT_CHILD_ORG_FIELDS_ID_CHILD_FIELD].Value.GetWithOutSec() );

					if( IsFieldDominatedInJoin( idChildField, i ) == M4_TRUE )
					{
						// Si está dominado en un join se salta
						bChildDominated = M4_TRUE;
						break;
					}
				}

				if( bChildDominated == M4_TRUE )
				{
					// Si un hijo está dominado en un join se salta
					continue;
				}

				m4id_t iLgcTbl = pLgcTbl->m_stObject.c_str();
				m4id_t nLgcTbl = m_pGlobVars->GetpLConn()->GetnLgcTbl( pLgcTbl->m_indLogicTable );

				if( ai_iOrgType == M4CL_ORGANIZATION_TYPE_NONE )
				{
					#ifdef _M4LDB_DEBUG
						M4LDB_PRINT_DEBUG( m_pGlobVars->GetpLConn(), "\nUnrecoverable unauthorized organization use of [%s].", idField ) ;
						M4LDB_PRINT_DEBUG_DETAIL( M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn(), "\nUnrecoverable unauthorized organization use of [%s].", idField ) ;
					#endif

					DUMP_CHLOG_WARNINGF( M4_CH_ACCESS_UNAUTHORIZED_NO_RECOVER_ORG_USE, m_pstAPISQLStm->c_str() << idField << LOG_CAT_AND( iLgcTbl, nLgcTbl ) << DumpLogAndLdb( m_pGlobVars->GetpLConn() ) ) ;
				}
				else
				{
					if( ai_iAccessLevel == 1 )
					{
						#ifdef _M4LDB_DEBUG
							M4LDB_PRINT_DEBUG( m_pGlobVars->GetpLConn(), "\nRecoverable unauthorized organization use of [%s].", idField ) ;
							M4LDB_PRINT_DEBUG_DETAIL( M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn(), "\nRecoverable unauthorized organization use of [%s].", idField ) ;
						#endif

						DUMP_CHLOG_WARNINGF( M4_CH_ACCESS_UNAUTHORIZED_RECOVER_ORG_USE, m_pstAPISQLStm->c_str() << idField << LOG_CAT_AND( iLgcTbl, nLgcTbl ) << DumpLogAndLdb( m_pGlobVars->GetpLConn() ) ) ;
					}
					else
					{
						#ifdef _M4LDB_DEBUG
							M4LDB_PRINT_DEBUG( m_pGlobVars->GetpLConn(), "\nAdding filter for recoverable unauthorized organization use of [%s].", idField ) ;
							M4LDB_PRINT_DEBUG_DETAIL( M4LDB_DETAIL_LEVEL_SHOW_PREPARE, m_pGlobVars->GetpLConn(), "\nAdding filter for recoverable unauthorized organization use of [%s].", idField ) ;
						#endif

						// Añadir filtro
						if( AddFilterRoleOrganization( pLgcTbl, idField ) != M4_SUCCESS )
						{
							return M4_ERROR;
						}
						ao_bExistsFilter = M4_TRUE;

						DUMP_CHLOG_WARNINGF( M4_CH_ACCESS_UNAUTHORIZED_RECOVERED_ORG_USE, m_pstAPISQLStm->c_str() << idField << LOG_CAT_AND( iLgcTbl, nLgcTbl ) << DumpLogAndLdb( m_pGlobVars->GetpLConn() ) ) ;
					}
				}
			}
		}
	}

	return M4_SUCCESS;
}


m4return_t	ClAPISQL::AddFilterChannelMonoSoc	(m4bool_t &ao_bExistsFilter)
{
	// Si las tablas son multisociedad, pero el canal es mono, añadimos el filtro AND ID_SOC = ?, para las tablas multisociedad.
	// Recorro el vector de alias, añadiendo el filtro para todos los necesarios.
	ClMetaAlias *pAlias = 0;
	ClMetaObject *pLgcTbl = 0;
	m4int32_t	nAlias = m_pMtSent->GetNumAlias();
	m4bool_t bBase = M4_FALSE;

	for (m4int32_t i=0;i<nAlias;++i)
	{
		pAlias = m_pMtSent->GetAliasByPos(i);
		if (pAlias->m_bReduce == M4_FALSE)
		{
			pLgcTbl = m_pMtSent->GetObjectByPos(pAlias->m_indObj);

			if (pLgcTbl->m_bIsBasis)
			{
				if (bBase || m_bIsSecurityView == M4_TRUE)
				{
					// La tabla base ya ha sido añadida o no hay que añadirla porque es una vista de seguridad
					continue;
				}
				else
				{
					bBase = M4_TRUE;
				}
			}

			if (pLgcTbl->m_iOrgType==M4LDB_MULTI_ORGANIZATION)
			{
				if (pLgcTbl->m_bAddedSocFilter)
				{
					// Ya esta añadido. Estamos en el filtro añadido por seguridad, pero ya se añadió a la TI.
					
					//return M4_SUCCESS;
					// Lo añadimos para todas las tablas multi. Suponemos que vendran unidas por ID_ORGANIZATION.
					continue;
				}

				// Añadimos el filtro.
				SetAuxVectors (M4LDB_CLAUSE_FROM, pLgcTbl->m_iLevel);
				SetAuxVectors (M4LDB_CLAUSE_WHERE, pLgcTbl->m_iLevel, M4_TRUE);
				
				m4char_t aAlias[M4LDB_SIZE_DEFAULT_STRING];
				sprintf (aAlias, "~A_%ld", pLgcTbl->m_indAlias);
				m_pstAux->append (aAlias);

				if (AddFilterByInternalType (M4LDB_ID_ORGANIZATION, pLgcTbl)!=M4_SUCCESS)
				{
					// El error sale dentro
					return M4_ERROR;
				}

				m_pstAux->append (" AND ~F = ?");

				SetAuxVectors (M4LDB_CLAUSE_END_STATEM, pLgcTbl->m_iLevel);
				ao_bExistsFilter = M4_TRUE;
				pLgcTbl->m_bAddedSocFilter = M4_TRUE;
			}
		}
	}

	return M4_SUCCESS;
}


m4return_t	ClAPISQL::AddFilterChannelMultiSoc	(m4bool_t &ao_bExistsFilter)
{
	// Si el canal es multi, hay que añadir la union de una de las tablas multi con la tabla ROLE_SOC.
	// !!!! Lo añadimos solo a una de las multis, porque suponemos que estaran unidas entre sí.
	// Recorremos las tablas y se lo añadimos a la primera multi.
	
	//Bug 0095175
	//La comprobación no se realiza por RSM (m_pGlobVars->GetIdRSM ()), se realizará por el Role del usuario.
	//Si no hay Role definido no se aplicará ningún filtro.	
	m4id_t	pccRole = m_pGlobVars->GetIdSecRole();

	if (pccRole == NULL || strcmpi (pccRole, M4CL_SEC_SUPER_USER) == 0 || strcmpi (pccRole, "M4RAISE_PRIVILEGES") == 0)
	{
		// SuperRol.
		return M4_SUCCESS;
	}

	ClMetaObject *pObject = 0;
	m4int32_t nObjects = m_pMtSent->GetNumObject();
	m4bool_t bBase = M4_FALSE;

	for (m4int32_t i=0;i<nObjects;++i)
	{
		pObject = m_pMtSent->GetObjectByPos(i);

		if (pObject->m_bIsBasis)
		{
			if (bBase || m_bIsSecurityView == M4_TRUE)
			{
				// La tabla base ya ha sido añadida o no hay que añadirla porque es una vista de seguridad
				continue;
			}
			else
			{
				bBase = M4_TRUE;
			}
		}

		if (pObject->m_iOrgType==M4LDB_MULTI_ORGANIZATION)
		{
			if (pObject->m_bAddedSocFilter)
			{
				// Ya esta añadido. Estamos en el filtro añadido por seguridad, pero ya se añadió a la TI.
				
				//return M4_SUCCESS;
				// Lo añadimos para todas las tablas multi. Suponemos que vendran unidas por ID_ORGANIZATION.
				continue;
			}

			if (AddFilterRoleOrganization(pObject, NULL)!=M4_SUCCESS)
			{
				return M4_ERROR;
			}
			ao_bExistsFilter = M4_TRUE;
			pObject->m_bAddedSocFilter = M4_TRUE;
			//break;
		}
	}
	
	return M4_SUCCESS;
}


m4return_t ClAPISQL::AddOneFilterRoleOrganization(ClMetaObject *ai_pObject, m4id_t ai_idField, m4pcchar_t ai_pccFilter)
{
	m_pstAux->append(ai_pccFilter);

	if (AnalizeSessionItem(M4_ITEM_CURRENT_ROLE_ID, M4_FALSE)!=M4_SUCCESS)
	{
		return M4_ERROR;
	}

	if (AddFieldByInternalField(M4LDB_ID_ORGANIZATION, ai_pObject, ai_idField)!=M4_SUCCESS)
	{
		return M4_ERROR;
	}
	m_pvcAuxFld->back()->m_bIsOuter = M4_FALSE;
	m_pvcAuxFld->back()->m_bIsMultiSoc = M4_TRUE;

	if (AnalizeSessionItem(M4_ITEM_CURRENT_ROLE_ID, M4_FALSE)!=M4_SUCCESS)
	{
		return M4_ERROR;
	}

	if (AnalizeSessionItem(M4_ITEM_USER, M4_FALSE)!=M4_SUCCESS)
	{
		return M4_ERROR;
	}

	if (AddFieldByInternalField(M4LDB_ID_ORGANIZATION, ai_pObject, ai_idField)!=M4_SUCCESS)
	{
		return M4_ERROR;
	}
	m_pvcAuxFld->back()->m_bIsOuter = M4_FALSE;
	m_pvcAuxFld->back()->m_bIsMultiSoc = M4_TRUE;

	/* Bug 0182590
	Se consigue la función SHORT_TODAY y se pone el filtro de fechas
	*/
	eDDBBType_t eType = m_pGlobVars->GetDDBBType();
	ClLConn *pLConn = m_pGlobVars->GetpLConn();
	m4int32_t indDDBB = pLConn->FindIndexById (M4LDB_SYS_DICT_DDBBS, eType);

	pLConn->m_paRS[M4LDB_SYS_DICT_DDBBS]->Current[indDDBB];

	m4int32_t indFunction = pLConn->FindIndexById (M4LDB_SYS_DICT_FUNCTIONS, "SHORT_TODAY");

	if (indFunction == M4LDB_NULL_INDEX)
	{
		DUMP_LDBINSP_ERRORF (pLConn, M4_CH_LDB_WRONG_LOGIC_FUNCTION, "SHORT_TODAY" << eType);
		return (M4_ERROR);
	}

	m4id_t idShortToday = M4LDB_ID (pLConn->m_paRS[M4LDB_SYS_DICT_FUNCTIONS]->Register[indFunction].Item[M4LDB_ITEM_SYS_DICT_FUNCTIONS_FUNCTION_HEADER].Value.GetWithOutSec ());

	m_pstAux->append(" AND U.DT_START <= ");
	m_pstAux->append(idShortToday);
	m_pstAux->append(" AND U.DT_END >= ");
	m_pstAux->append(idShortToday);
	m_pstAux->append(")");

	return M4_SUCCESS;
}


m4return_t ClAPISQL::AddFilterRoleOrganization(ClMetaObject *ai_pObject, m4id_t ai_idField)
{
/*
Hay que añadir

AND
(
	ID_ORGANIZATION IN
	(
		SELECT R.ID_ORGANIZATION_US FROM M4RSC_ROLE_ORG R WHERE R.ID_APP_ROLE_USED = $ID_APP_ROLE
		UNION ALL
		SELECT U.ID_ORGANIZATION_US FROM M4RSC_APP_USR_ROLE U WHERE U.ID_APP_ROLE = $ID_APP_ROLE AND U.ID_APP_USR = $ID_APP_USER AND U.DT_START <= #TODAY() AND U.DT_END >= #TODAY()
	)
	OR
		ID_ORGANIZATION IS NULL
)

pero si la tabla es de herencia o de nivel superior al mínimo hay que filtrar también por sociedades padre  

AND
(
	ID_ORGANIZATION IN
	(
		SELECT T.ID_PAR_ORGANIZATIO FROM M4VCH_ORG_TREE T, M4RSC_ROLE_ORG R WHERE T.ID_ORGANIZATION = R.ID_ORGANIZATION_US AND R.ID_APP_ROLE_USED = $ID_APP_ROLE
		UNION ALL
		SELECT T.ID_PAR_ORGANIZATIO FROM M4VCH_ORG_TREE T, M4RSC_APP_USR_ROLE U WHERE T.ID_ORGANIZATION = U.ID_ORGANIZATION_US AND U.ID_APP_ROLE = $ID_APP_ROLE AND U.ID_APP_USR = $ID_APP_USER AND U.DT_START <= #TODAY() AND U.DT_END >= #TODAY()
	)
	OR
		ID_ORGANIZATION IS NULL
)

el NULL hay que mantenerlo por los outer joins
*/


	m4int32_t Level = ai_pObject->m_iLevel;
	
	SetAuxVectors (M4LDB_CLAUSE_FROM,Level);
	SetAuxVectors(M4LDB_CLAUSE_WHERE,Level,M4_TRUE);

	m4char_t aAlias[M4LDB_SIZE_DEFAULT_STRING];
	sprintf (aAlias, "~A_%ld", ai_pObject->m_indAlias);
	m_pstAux->append (aAlias);

	/* Bug 0156727
	Si la tabla es de herencia o es de nivel superior al mínimo hay que filtrar por sociedades padre
	*/
	/* Bug 0258708
	Se cambio el OR por UNION ALL que es más rápido
	*/
	m4pcchar_t	pccFilterRoleOrg = " AND ( ~F IN ( SELECT R.ID_ORGANIZATION_US FROM M4RSC_ROLE_ORG R WHERE R.ID_APP_ROLE_USED =" ;
	m4pcchar_t	pccFilterRoleOrg1 = " UNION ALL SELECT U.ID_ORGANIZATION_US FROM M4RSC_APP_USR_ROLE U WHERE U.ID_APP_ROLE =" ;

	if( ai_idField != NULL || ai_pObject->m_bInheritsData == M4_TRUE || ai_pObject->m_iOrgLevel > m_pGlobVars->GetpLConn()->GetpLdb()->GetMinOrgLevel() )
	{
		pccFilterRoleOrg = " AND ( ~F IN ( SELECT T.ID_PAR_ORGANIZATIO FROM M4VCH_ORG_TREE T, M4RSC_ROLE_ORG R WHERE T.ID_ORGANIZATION = R.ID_ORGANIZATION_US AND R.ID_APP_ROLE_USED =" ;
		pccFilterRoleOrg1 = " UNION ALL SELECT T.ID_PAR_ORGANIZATIO FROM M4VCH_ORG_TREE T, M4RSC_APP_USR_ROLE U WHERE T.ID_ORGANIZATION = U.ID_ORGANIZATION_US AND U.ID_APP_ROLE =" ;
	}

	m_pstAux->append(pccFilterRoleOrg);

	if (AddFieldByInternalField(M4LDB_ID_ORGANIZATION, ai_pObject, ai_idField)!=M4_SUCCESS)
	{
		return M4_ERROR;
	}
	m_pvcAuxFld->back()->m_bIsOuter = M4_FALSE;
	m_pvcAuxFld->back()->m_bIsMultiSoc = M4_TRUE;
	
	if (AnalizeSessionItem(M4_ITEM_CURRENT_ROLE_ID, M4_TRUE)!=M4_SUCCESS)
	{
		return M4_ERROR;
	}
	
	m_pstAux->append(pccFilterRoleOrg1);

	if (AnalizeSessionItem(M4_ITEM_CURRENT_ROLE_ID, M4_TRUE)!=M4_SUCCESS)
	{
		return M4_ERROR;
	}

	m_pstAux->append(" AND U.ID_APP_USR =");

	if (AnalizeSessionItem(M4_ITEM_USER, M4_TRUE)!=M4_SUCCESS)
	{
		return M4_ERROR;
	}

	/* Bug 0182590
	Se consigue la función TODAY y se pone el filtro de fechas
	*/
	eDDBBType_t eType = m_pGlobVars->GetDDBBType();
	ClLConn *pLConn = m_pGlobVars->GetpLConn();
	m4int32_t indDDBB = pLConn->FindIndexById (M4LDB_SYS_DICT_DDBBS, eType);

	pLConn->m_paRS[M4LDB_SYS_DICT_DDBBS]->Current[indDDBB];

	m4int32_t indFunction = pLConn->FindIndexById (M4LDB_SYS_DICT_FUNCTIONS, "TODAY");

	if (indFunction == M4LDB_NULL_INDEX)
	{
		DUMP_LDBINSP_ERRORF (pLConn, M4_CH_LDB_WRONG_LOGIC_FUNCTION, "TODAY" << eType);
		return (M4_ERROR);
	}

	m4id_t idToday = M4LDB_ID (pLConn->m_paRS[M4LDB_SYS_DICT_FUNCTIONS]->Register[indFunction].Item[M4LDB_ITEM_SYS_DICT_FUNCTIONS_FUNCTION_HEADER ].Value.GetWithOutSec ());

	m_pstAux->append(" AND U.DT_START <= ");
	m_pstAux->append(idToday);
	m_pstAux->append(" AND U.DT_END >= ");
	m_pstAux->append(idToday);

	/* Bug 0261269
	Hay que mantener el NULL porque en caso de outers puede llegar la sociedad nula
	*/
	m_pstAux->append(" ) OR ~F IS NULL )");

	if (AddFieldByInternalField(M4LDB_ID_ORGANIZATION, ai_pObject, ai_idField)!=M4_SUCCESS)
	{
		return M4_ERROR;
	}
	m_pvcAuxFld->back()->m_bIsOuter = M4_FALSE;
	m_pvcAuxFld->back()->m_bIsMultiSoc = M4_TRUE;

	SetAuxVectors(M4LDB_CLAUSE_END_STATEM);

	return M4_SUCCESS;
}


m4return_t ClAPISQL::AddTramitFilters(m4bool_t &ao_bExistsFilter)
{
	if (m_pMtSent->IsReal())
	{
		return M4_SUCCESS;
	}

	// Si no se está en tramitación no se añade nada
	m4pchar_t pcTramitation = NULL ;
	ClChannel * pChannel = m_pGlobVars->GetpAccessRecordSet()->GetpAccess()->GetpChannel();

	pChannel->Tramitation.Get( pcTramitation ) ;

	if( pcTramitation == NULL )
	{
		return M4_SUCCESS;
	}

	/////////////////////////////////////////
	// Para el Insert, update y el Insert into Select y el update select
	m4bool_t bMarkAsAdded = M4_FALSE;

	if (m_pMtSent->GetStmType()==M4LDB_INSERT)
	{
		if (m_pMtSent->GetIsInsertIntoSelect() == M4_FALSE)
		{
			// Si es Insert y no es IntoSelect no añado nada.
			return M4_SUCCESS;
		}

		// Si es insert into select no se ponen filtros en la principal
		bMarkAsAdded = M4_TRUE ;
	}
	else if (m_pMtSent->GetStmType()==M4LDB_UPDATE && m_pMtSent->GetIsUpdateSelect() == M4_TRUE)
	{
		// Si es update select no se ponen filtros en la principal
		bMarkAsAdded = M4_TRUE ;
	}

	if( bMarkAsAdded == M4_TRUE )
	{
		// Insert into Select, update select.
		// Marco los objeto de nivel 0 como que tienen el filtro añadido. En estos objetos no se debe añadir filtro, porque es un INSERT.
		m4int32_t nObjects = m_pMtSent->GetNumObject();
		ClMetaObject *pObject=0;
		for (m4int32_t i=0;i<nObjects;++i)
		{
			pObject = m_pMtSent->GetObjectByPos(i);
			if (pObject->m_iLevel>0)
			{
				break;
			}
			else
			{
				pObject->m_bAddedTramitFilter = M4_TRUE;
			}
		}
	}
	/////////////////////////////////////////

	// Si las tablas son tramitables añadimos el filtro AND ID_DATASET = ?
	// Recorro el vector de alias, añadiendo el filtro para todos los necesarios.

	ClMetaAlias *pAlias = 0;
	ClMetaObject *pLgcTbl = 0;
	m4int32_t	nAlias = m_pMtSent->GetNumAlias();
	m4bool_t bBase = M4_FALSE;

	for (m4int32_t i=0;i<nAlias;++i)
	{
		pAlias = m_pMtSent->GetAliasByPos(i);
		if (pAlias->m_bReduce == M4_FALSE)
		{
			pLgcTbl = m_pMtSent->GetObjectByPos(pAlias->m_indObj);

			if (pLgcTbl->m_bIsBasis)
			{
				if (bBase)
				{
					// La tabla base ya ha sido añadida.

					// En el caso especial de update select no se añade filtro para la tabla base
					// de la subselect porque su tabla base es la misma de la principal.
					// De momento eso es lo que se quiere para consolidar tramitaciones
					continue;
				}
				else
				{
					bBase = M4_TRUE;
				}
			}

			if (pLgcTbl->m_iMirrorType == 1)
			{
				if (pLgcTbl->m_bAddedTramitFilter)
				{
					// Ya esta añadido. Estamos en el filtro añadido por seguridad, pero ya se añadió a la TI.
					continue;
				}

				// Añadimos el filtro.
				SetAuxVectors (M4LDB_CLAUSE_FROM, pLgcTbl->m_iLevel);
				SetAuxVectors (M4LDB_CLAUSE_WHERE, pLgcTbl->m_iLevel, M4_TRUE);
				
				m4char_t aAlias[M4LDB_SIZE_DEFAULT_STRING];
				sprintf (aAlias, "~A_%ld", pLgcTbl->m_indAlias);
				m_pstAux->append (aAlias);

				if (AddFilterByInternalType (M4LDB_ID_DATASET, pLgcTbl)!=M4_SUCCESS)
				{
					// El error sale dentro
					return M4_ERROR;
				}

				m_pstAux->append (" AND ~F = ?");

				SetAuxVectors (M4LDB_CLAUSE_END_STATEM, pLgcTbl->m_iLevel);
				ao_bExistsFilter = M4_TRUE;
				pLgcTbl->m_bAddedTramitFilter = M4_TRUE;
			}
		}
	}

	return M4_SUCCESS;
}


m4bool_t ClAPISQL::IsFieldInUse (m4id_t ai_idField, m4int32_t ai_iAlias)
{
	if( m_pMtSent != NULL && m_pMtSent->m_pvcFld != NULL )
	{
		for( itVcClMtField itFld = m_pMtSent->m_pvcFld->begin(); itFld != m_pMtSent->m_pvcFld->end() ; itFld++ )
		{				
			if( (*itFld)->IsMetaData() == M4_TRUE && (*itFld)->m_indAlias == ai_iAlias && strcmpi( ai_idField, (*itFld)->m_stField.c_str() ) == 0 )
			{
				return M4_TRUE;
			}
		}
	}
	return M4_FALSE;
}


m4bool_t ClAPISQL::IsFieldDominatedInJoin (m4id_t ai_idField, m4int32_t ai_iAlias)
{
	if( m_pMtSent != NULL && m_pMtSent->m_pvcRel != NULL )
	{
		for( itVcClMtRelat itRel = m_pMtSent->m_pvcRel->begin(); itRel != m_pMtSent->m_pvcRel->end() ; itRel++ )
		{				
			if( (*itRel)->m_bReduce == M4_TRUE )
			{
				continue ;
			}
			
			if( (*itRel)->m_pMtField1->m_indAlias == ai_iAlias && strcmpi( ai_idField, (*itRel)->m_pMtField1->m_stField.c_str() ) == 0 )
			{
				if( (*itRel)->m_eJoinType == M4LDB_NORMAL_JOIN && (*itRel)->m_pMtField2->m_indAlias < ai_iAlias )
				{
					return M4_TRUE;
				}
			}
			else if( (*itRel)->m_pMtField2->m_indAlias == ai_iAlias && strcmpi( ai_idField, (*itRel)->m_pMtField2->m_stField.c_str() ) == 0 )
			{
				if( (*itRel)->m_eJoinType == M4LDB_OUTER_JOIN || (*itRel)->m_pMtField1->m_indAlias < ai_iAlias )
				{
					return M4_TRUE;
				}
			}
		}
	}
	return M4_FALSE;
}



//Bug 0071469
//Retocada para que en el caso de filtros automáticos, se busque el campo en las tablas de la herencia.
//Recibe un parametro opcional, por compatibilidad. Será M4_TRUE si estamos añadiendo los filtros automáticos.

m4return_t	ClAPISQL::AddFieldByInternalField(eFldType_t ai_eFldType, ClMetaObject *ai_pLgcObj, m4id_t ai_idField, m4bool_t ai_bAutomaticFilters, m4int8_t *ao_piM4Type)
{
	// Situamos Logic Objects.
	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[(ClRegisterIndex) ai_pLgcObj->m_indLogicTable];
	// Situamos Real Objects.
	m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[(ClRegisterIndex) 0];

	
	ClRegisterIndex indFld = M4LDB_NULL_INDEX;
	
	if (ai_idField != NULL)
	{
		indFld = m_pGlobVars->GetpLConn ()->FindIndexByIdField (ai_idField);
	}
	else
	{
		indFld = m_pGlobVars->GetpLConn ()->FindIndexByIdInternalField (ai_eFldType);
	}

	//Bug 0071469.
	//Sólo busco en la herencia si lo que estoy buscando es un tipo interno para añadir un autofiltro.
	if (ai_bAutomaticFilters == M4_TRUE)  //Bug 0072858
	{
		//Si no lo encontramos en la tabla actual, buscamos en las tabla padre.	
		//La padre tiene el valor de herencia a 0.
		m4bool_t bHasInheritance = M4LDB_BOOL (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_HAS_INHERITANCE].Value.GetWithOutSec ());
		//Obtenemos el id del objeto en que estamos.
		m4id_t idLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Item[ M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());
		m4int32_t indLogicTable = M4LDB_NULL_INDEX;
		m4return_t result;

		if ((indFld == M4LDB_NULL_INDEX) && (bHasInheritance))
		{
			m4id_t idParentLgcTbl;
			m4id_t idFieldParent;
			m4int32_t indParentLogicTable = M4LDB_NULL_INDEX;		
			ClRegisterIndex indFldParent = M4LDB_NULL_INDEX;

			//Vemos cuantos ascendentes y descendentes tenemos.	Tenemos que buscar el campo en los padres.
			//Nota: En realidad, en la carga del dccionario SOLO se cargan los ascendentes (padre, abuelo,...)
			//Ademas, en la SysLoad hay limitación a dos Apisql.
			//Por lo tanto, en el nodo sólo se cargan Padres!!!
			//A pesar de que en los M2 tenemos la posibilidad de diferenciar ascendientes y descendientes.
			m4int32_t nParents = m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_INHERITANCE]->Current.Count();
			for (m4int32_t iInheritance = 0; iInheritance < nParents; iInheritance ++)
			{				
				//Es un padre, abuelo,...
				m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_INHERITANCE]->Current[(ClRegisterIndex) iInheritance];
				
				idParentLgcTbl = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_INHERITANCE]->Current[(ClRegisterIndex) iInheritance].Item[M4LDB_ITEM_SYS_DICT_INHERITANCE_ID_AUX_OBJECT].Value.GetWithOutSec ());

				//Cargamos el diccionario del padre
				result = m_pGlobVars->GetpLConn ()->LoadDictionaryChannel (idParentLgcTbl, &indParentLogicTable, M4_TRUE);
				if (result == M4_ERROR)
				{
					return M4_ERROR;
				}

				//La tabla padre es la misma que la hija. Me olvido.
				//En principio esto no ocurre pq en la tabla plana que relaciona
				//padres-hijas no hay un registro con id_padre = id_hija. (Por los m2, nivel <> 0).
				if (ai_pLgcObj->m_indLogicTable == indParentLogicTable)
				{
					return M4_ERROR;
				}

				//Ahora buscamos el campo en la padre!!.
				m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[(ClRegisterIndex) indParentLogicTable];
				m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[(ClRegisterIndex) 0];

				//Buscamos el campo por tipo interno, pero esta vez estoy en el padre.
				//Esta función devuelve el índice del field!!.
				indFldParent = m_pGlobVars->GetpLConn ()->FindIndexByIdInternalField (ai_eFldType);

				//El campo no lo hemos encontrado en el padre, buscamos en el abuelo...
				//Iterando de nuevo en el bucle.
				if (indFldParent == M4LDB_NULL_INDEX)
				{
					//---------------------
					//Cada iteración, el diccionario debe quedar como estaba inicialmente.
					//Ademas, en la salida del bucle tambien debe dejarse todo como estaba inicialmente.
					result = m_pGlobVars->GetpLConn ()->LoadDictionaryChannel (idLgcTbl, &indLogicTable, M4_TRUE);
					if (result == M4_ERROR)
					{
						return M4_ERROR;
					}
					m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[(ClRegisterIndex) ai_pLgcObj->m_indLogicTable];
					m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[(ClRegisterIndex) 0];
					//---------------------
					continue;
				}
				else
				{
					//El campo se ha encontrado!!.
					
					//Necesitamos saber el id del campo y debe estar está en el padre.
					//Nos situamos y cogemos el id (en la tabla padre).
					idFieldParent = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indFldParent].Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_FIELD].Value.GetWithOutSec ());

					/* Bug 0284773
					En el caso de tabla padre también hay que calcular el tipo del dato
					*/
					if (ao_piM4Type != NULL )
					{
						m4id_t idType = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indFldParent].Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_TYPE].Value.GetWithOutSec ());
						m4int32_t indType = m_pGlobVars->GetpLConn ()->FindIndexById (M4LDB_SYS_DICT_EXTENDED_TYPES, idType);

						if (indType == M4LDB_NULL_INDEX)
						{
							DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_EXTENDED_TYPE_NOT_FOUND, idType << idFieldParent);
							return (M4_ERROR);
						}

						*ao_piM4Type = M4LDB_M4_TYPE ((m4int8_t) M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_EXTENDED_TYPES]->Register[indType].Item[M4LDB_ITEM_SYS_DICT_EXTENDED_TYPES_ID_M4_TYPE].Value.GetWithOutSec ()));
					}

					break;
				}

			}	//for (iInheritance = 0; iInheritance < nParentsChildren; iInheritance++)

			//Definitivamente no lo ha encontrado en el padre, abuelo, ...
			if (indFldParent == M4LDB_NULL_INDEX)
			{
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, ai_eFldType << ai_pLgcObj->m_stObject.c_str ());
				return M4_ERROR;
			}
			else
			{
				//--------------------------
				//Una vez encontrado el Id del campo, es necesario buscar en la herencia
				//a partir del objeto hijo!!.
				result = m_pGlobVars->GetpLConn ()->LoadDictionaryChannel (idLgcTbl, &indLogicTable, M4_TRUE);
				if (result == M4_ERROR)
				{
					return M4_ERROR;
				}
				m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[(ClRegisterIndex) ai_pLgcObj->m_indLogicTable];
				m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current[(ClRegisterIndex) 0];
				//--------------------------

				//Creamos el campo con el id que se ha calculado.			
				ClMetaField *pFld = new ClMetaField (ai_pLgcObj->m_iLevel, M4LDB_CLAUSE_WHERE, idFieldParent, M4LDB_NULL_INDEX, ai_pLgcObj->m_iFilter);
				//m_pvcAuxFld->push_back (pFld);

				//Indicamos:
				//Indice en el diccionario donde está el campo.
				//La tabla real será la 0.
				//El indice del objeto lógico en el diccionario.
				//El notify no se puede hacer aqui porque el objeto lógico padre
				//puede no estar creado, pero en la búsqueda en el diccionario ya
				//se realizará si es necesario.
				
				//Ojo, ponemos el alias a 0. Si lo ponemos a -1 buscará un alias para el campo.
				//El alias es -1 para que busque el alias apropiado para el campo.
					//pFld->m_indRealField = indFldParent;
					//pFld->m_indRealTable = 0;

				// Bug 0291152 Se pone el índice de la tabla pasada y no 0
				pFld->m_indAlias = ai_pLgcObj->m_indAlias;
				//Buscamos el id del campo a partir del objeto hijo.
				//La recursión ya encontrará el campo en el padre.
				pFld->m_indLogicTable = indLogicTable;
				pFld->m_indRealField = M4LDB_NULL_INDEX;
				pFld->m_indRealTable = M4LDB_NULL_INDEX;
				
				//Buscamos, esta funcion busca en la tabla padre.
				//Como estamos situados en el diccionario en el objeto hijo, nos ponemos
				//a buscar el campo.
				//Este miembro no solo busca también añade relaciones... si hace falta.
				//En nuestro caso sí hará falta porque puede ser que no hayamos seleccionado
				//ningún campo de la tabla padre, por lo tanto hace falta poner el join
				//con la tabla padre-hija y además meterá el field.

				result = FindFieldInTheDicticionary (pFld);

				if (result == M4_ERROR)
				{
					return M4_ERROR;
				}

				SetAuxVectors (M4LDB_CLAUSE_FROM, ai_pLgcObj->m_iLevel);
				SetAuxVectors (M4LDB_CLAUSE_WHERE, ai_pLgcObj->m_iLevel, M4_TRUE);
				
				m4char_t aAlias[M4LDB_SIZE_DEFAULT_STRING];
				sprintf (aAlias, "~A_%ld", pFld->m_indAlias);
				m_pstAux->append (aAlias);

				m_pvcAuxFld->push_back (pFld);
				//Pasamos datos del from al aux del from.

				//Explicació. Las llamadas son por este orden:
				//AddInfoFilter -> Necesita el vector de from (cuando quiero meter filtro en sentencia con ORs).
				//AddInfoObjects --> Necesita que el vector from este vacio pq tenemos un assert en el 
				//TransferMetaData. En concreto en ClMetaSentence::AddInfoObjects.
				//Porque funciona cuando estamos añadiendo los filtros de sociedad:
					//Cuando estamos insertando filtros, el vector from esta con elementos, pero el 
					//vector auxiliar de from está vacio. Por lo tanto en la llamada que se realiza
					//a AddInfoFilter (ok pq from hay datos).
					//Y en la llamada AddInfoObjects le pasamos como argumento el auxiliar de from, y
					//dentro de AddInfoObjects se chequea si el argumento está vacio (si esta vacio
					//retorna ok, si no está vacio está el assert.
				
				//En la creación de los alias se insertan elementos en el aux from, por lo tanto aqui tampoco
				//puedo eliminar el vector auxiliar de from.
				//Vamos a ir metiendo lo que tenemos en el aux from.				
				itVcClMtObject itFrmS = m_pMtSent->m_pvcFrmObj->begin();
				itVcClMtObject itFrmE = m_pMtSent->m_pvcFrmObj->end();
				m_pvcAuxFrmObj->insert(m_pvcAuxFrmObj->begin(), itFrmS, itFrmE);				
				m_pMtSent->m_pvcFrmObj->erase(itFrmS, itFrmE);

				//Cuando se termine la insercion de filtros automaticos, deberemos,
				//realizar el proceso inverso: Pasar el aux from al from y limpiar el from.

				//Podemos retornar porque hemos dejado del diccionario como estaba inicialmente.
				return result;
			}
		}	//if ((indFld == M4LDB_NULL_INDEX) && (bHasInheritance)) {
	}	//if (ai_bAutomaticFilters = M4_TRUE) 
	// Fin Bug 0071469 >>>	

	if (indFld == M4LDB_NULL_INDEX)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_LOGIC_FIELD_NOT_FOUND, ai_eFldType << ai_pLgcObj->m_stObject.c_str ());
		return (M4_ERROR);
	}

	//Si estoy añadiendo filtros automáticos hago set de los vectores auxiliares pq en el método invocante
	//AddAutomaticFilters no se hace.
	if (ai_bAutomaticFilters == M4_TRUE)  //Bug 0072858
	{
		SetAuxVectors (M4LDB_CLAUSE_FROM, ai_pLgcObj->m_iLevel);
		SetAuxVectors (M4LDB_CLAUSE_WHERE, ai_pLgcObj->m_iLevel, M4_TRUE);
		
		//Ojito. El caracter macabro ~ seguido del alias se utiliza en la función _AddInfoPartFilterAdd.
		//Se utiliza para saber dónde se mete el filtro en el caso de que haya ORs.
		//El espacio detras del alias podría estar relacionado con el bug 0068102 que se corrigió de otra forma.
		//Ahora no lo ponemos pq en principio el bug se cerro correctamente.
		m4char_t aAlias[M4LDB_SIZE_DEFAULT_STRING];
		sprintf (aAlias, "~A_%ld", ai_pLgcObj->m_indAlias);
		m_pstAux->append (aAlias);
	}


	m4id_t idField = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indFld].Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_FIELD].Value.GetWithOutSec ());

	ClMetaField *pMtFld = new ClMetaField (ai_pLgcObj->m_iLevel, M4LDB_CLAUSE_WHERE, idField, ai_pLgcObj->m_indAlias, ai_pLgcObj->m_iFilter);

	//Bug 0069558.	
	//A pesar de que el objeto tenga OJ, el campo lo dejamos a inner. La razón es pq se va a hacer
	//Campo = valor en la parte where, es decir, estamos buscando un valor concreto del campo, por lo
	//tanto el OJ no tiene sentido ya que si no es ese valor, el registro no tiene que traerse.
	//Bug 0071803.
	//Volvemos a descomentar el código. Para que no se corte el OJ.
	if (ai_pLgcObj->m_eJoinType==M4LDB_OUTER_JOIN)
	{
		pMtFld->m_bIsOuter = M4_TRUE;
	}

	m_pvcAuxFld->push_back (pMtFld);

	pMtFld->m_indRealField = indFld;
	pMtFld->m_indRealTable = 0;
	pMtFld->m_indLogicTable = ai_pLgcObj->m_indLogicTable;

	ai_pLgcObj->NotifyUsingRealTable (pMtFld->m_indRealTable);

	if (ao_piM4Type != NULL )
	{
		m4id_t idType = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register[indFld].Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_TYPE].Value.GetWithOutSec ());
		m4int32_t indType = m_pGlobVars->GetpLConn ()->FindIndexById (M4LDB_SYS_DICT_EXTENDED_TYPES, idType);

		if (indType == M4LDB_NULL_INDEX)
		{
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_EXTENDED_TYPE_NOT_FOUND, idType << idField);
			return (M4_ERROR);
		}

		*ao_piM4Type = M4LDB_M4_TYPE ((m4int8_t) M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_EXTENDED_TYPES]->Register[indType].Item[M4LDB_ITEM_SYS_DICT_EXTENDED_TYPES_ID_M4_TYPE].Value.GetWithOutSec ()));
	}
	
	return M4_SUCCESS;
}


m4return_t ClAPISQL::TransferMetaData (m4bool_t ai_bExistsMoreObjects, m4bool_t ai_bProcessOrs)
{
	//Pedro Bug 0069267
	//En versiones 6 se puso un control M4_CH_LDB_MAX_NUMBER_SUBQUERY_EXCEDED.
	//De forma que si el número de niveles era superior a 99 nos daba el error.
	//Parece ser que esto se hizo porque se reservaban 2 dígitos para la numeración
	//de bloques: F00|W00...
	//Esta limitación se corrigió, reservando 4 dígtos. En vez de cablear a 9999, 
	//podemos calcular ese valor en función de la constante M4LDB_MD_LENGTH_CLAUSE.
	//Para M4LDB_MD_LENGTH_CLAUSE = 6 tenemos la posibilidad de pow(10,4) = 10000.
	//Es decir 0..9999. Por lo tanto, el max no puede superar 9999.
	m4int32_t lMaxLevel = (m4int32_t) pow((double)10, (M4LDB_MD_LENGTH_CLAUSE - 2)) - 1;
	if (m_lMaxLevel > lMaxLevel)
	{
		DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_MAX_NUMBER_SUBQUERY_EXCEDED);		
		return (M4_ERROR);
	}

	m4uint32_t iColumnStart = 0;

	if( m_pvcAuxColItm != NULL )
	{
		iColumnStart = m_pvcAuxColItm->size();
	}

	if (m_pMtSent->AddInfoColumns(m_pvcAuxColItm,m_pvcAuxColPar,m_pvcAuxColFld,m_pstAuxCol,0)!=M4_SUCCESS)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClAPISQL::TransferMetaData" << "Vector of columns bad defined");
		return M4_ERROR;
	}
	if (m_pMtSent->AddInfoLongColumns(m_pvcAuxColLngItm,m_pvcAuxColLngPar,m_pvcAuxColLngFld,m_pstAuxColLng,iColumnStart)!=M4_SUCCESS)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClAPISQL::TransferMetaData" << "Vector of columns long bad defined");
		return M4_ERROR;
	}
	if (m_pMtSent->AddInfoFilter(m_pvcAuxWhrPar,m_pvcAuxWhrFld,m_pstAuxWhr, ai_bProcessOrs)!=M4_SUCCESS)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClAPISQL::TransferMetaData" << "Vector of filters bad defined");
		return M4_ERROR;
	}
	if (m_pMtSent->AddInfoObjects(m_pvcAuxFrmObj,m_pvcAuxFrmRel,(m_indAlsSec==M4LDB_NULL_INDEX) ? M4_FALSE : M4_TRUE)!=M4_SUCCESS)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClAPISQL::TransferMetaData" << "Vector of objects bad defined");
		return M4_ERROR;
	}
	if (ai_bExistsMoreObjects)
	{
		if (m_pMtSent->AdjustLogObj()==M4_ERROR)
		{
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClAPISQL::TransferMetaData" << "Vector of objects bad defined");
			return M4_ERROR;
		}
		if (m_pMtSent->AnalizeRelations()==M4_ERROR)
		{
//			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClAPISQL::TransferMetaData" << "Vector of relations bad defined");
			return M4_ERROR;
		}
	}
	// Borrado de objetos base que estuvieran repetidos.

	return M4_SUCCESS;
}


m4return_t ClAPISQL::_CreateAlias (ClMetaObject *ai_pMtObj, m4bool_t ai_bBasis)
{
	sprintf (m_pAuxString, "%s^", ai_pMtObj->m_stObject.c_str ());

	ai_pMtObj->m_indAlias = _FindIdxAlias (m_pAuxString, m_lFilter, m_lLevel, m_eClause);

	m4int32_t indObj = m_pvcAuxFrmObj->size ();
	
	ClMetaAlias * pAlias = m_pMtSent->GetAliasByPos(ai_pMtObj->m_indAlias);
	
	if (pAlias ->m_indObj!=M4_STR_NPOS)
	{
		// Venia FROM tabla1,tabla1
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_AMBIGOUS_ALIAS,ai_pMtObj->m_stObject.c_str());
		delete ai_pMtObj;
		return M4_ERROR;
	}

	pAlias->m_indObj = indObj;

	m_pvcAuxFrmObj->push_back (ai_pMtObj);


	if (ai_bBasis && m_indAlsBasis == M4LDB_NULL_INDEX)
	{
		m_indAlsBasis = ai_pMtObj->m_indAlias;
	}

	ai_pMtObj = NULL;

	return M4_SUCCESS;
}


m4int32_t ClAPISQL::_FindIdxAlias (m4pcchar_t ai_pAlias, m4int32_t ai_lFilter, m4int32_t ai_lLevel, eClauseType_t ai_eClause)
{
	m4bool_t bInc = M4_TRUE;
	if (*ai_pAlias == '^')
	{
		// Ha sido definido anteriormente.
		bInc = M4_FALSE;
	}

	m4int32_t indAls;
	m4char_t aAlias[M4LDB_SIZE_DEFAULT_STRING];
	m4int32_t lLevel = ai_lLevel;	
	m4int32_t numAls = m_pMtSent->GetNumAlias ();
	ClMetaAlias *pMtAlias = NULL;

	m4pcchar_t pAlias = ai_pAlias;
	do
	{
		if (bInc)
		{
			sprintf (aAlias, "^%ld_%ld_%s", lLevel, ai_lFilter, ai_pAlias);
			pAlias = aAlias;
		}

		for (indAls=0; indAls<numAls; indAls++)
		{
			pMtAlias = m_pMtSent->GetAliasByPos (indAls);
			if (!strcmpi (pAlias, pMtAlias->m_stAlias.c_str()))
			{
				indAls = m_pMtSent->GetNewAliasPosByAliasPos (indAls);
				m4bool_t bClose = M4_FALSE;
				if (lLevel != ai_lLevel)
				{
					// Lo hemos encontrado en un nivel inferior, pero ¿es anidado?
					if (m_pvcAuxWhrFld)
					{
						for (itVcClMtField it=m_pvcAuxWhrFld->begin (); it!=m_pvcAuxWhrFld->end(); it++)
						{
							if ((*it)->IsSeparator () == M4_TRUE && 
								(*it)->m_eClause == M4LDB_CLAUSE_END_STATEM && 
								(*it)->m_iLevel == lLevel && 
								(*it)->m_iFilter == ai_lFilter)
							{
								bClose = M4_TRUE;
								break;
							}
						}
					}
				}
				if (!bClose)
				{
					return (indAls);
				}
				break;
			}
		}

		// Aunque no lo hemos encontrado, puede ocurrir que esté en un nivel inferior.
		lLevel--;

	} while ((lLevel>=0) && ((ai_eClause == M4LDB_CLAUSE_WHERE) || (ai_eClause == M4LDB_CLAUSE_REST) || (ai_eClause == M4LDB_CLAUSE_ORDER_BY)));

	if (bInc)
	{
		/* Bug 0126937
		Después del algoritmo recursivo se ha perdido el nivel
		*/
		sprintf (aAlias, "^%ld_%ld_%s", ai_lLevel, ai_lFilter, ai_pAlias);

		ClMetaAlias *pstAlias = new ClMetaAlias (ai_lLevel, M4LDB_CLAUSE_WHERE, aAlias, ai_lFilter);
		m_pMtSent->AddAlias (pstAlias);
		return (numAls);
	}

	return (M4LDB_NULL_INDEX);
}


void ClAPISQL::FindOR (m4pchar_t &ao_pCurrent)
{
	m4return_t result;
	m4char_t cSeparator;
	m4bool_t bIsWord;
	m4int32_t lenToken;
	m4bool_t bExistsOR = M4_FALSE;
	m4bool_t bInsertOR = M4_FALSE;
	m4bool_t bExit = M4_FALSE;
	m4pchar_t pLeft = ao_pCurrent - 1; // Para la primera vez será una 'E', para las demás un '('.
	m4pchar_t pRight = ao_pCurrent; 

	do
	{
		m4pchar_t pAux = ao_pCurrent;
		result = _X_GetToken (ao_pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal ());
		if (result != M4_ERROR && bIsWord)
		{
			if (!strcmp (m_pAuxString, "OR"))
			{ 
				bExistsOR = M4_TRUE;
			}
			else
			{
				if (!bExistsOR && !strcmp (m_pAuxString, "WHERE"))
				{
					pLeft = ao_pCurrent - 2;	// Apuntamos a la última 'E' de WHERE.
				}
			}
		}


		switch( cSeparator )
		{
		case 0:
			return;
		case '(':
			FindOR (ao_pCurrent);
			break;
		case ')':
		case ';':
			if (bExistsOR)
			{
				pRight = ao_pCurrent - 1;
				bInsertOR = M4_TRUE;
			}
			bExit = M4_TRUE;
			break;
		}


		if (result != M4_ERROR && bIsWord)
		{
			if ((*m_pAuxString != 'W') && 
				(*m_pAuxString != 'S') &&		// No es ni WHERE ni SET
				//Bug 0071042. Tenemos dos bloques select con Ors y las unimos
				//con UNION (es palabra reservada despues de from)
				//Lo que ocurre es que el primer bloque where se marcan correctamente
				//los parentesis de los OR, pero el segundo bloque que va detras del union
				//no llega a analizarse.
				//Solucion: Considerar que si es UNION no entre en el if (que me
				//saca del bucle). Debo continuar analizando el segundo bloque where!!.
				(*m_pAuxString != 'U') &&		// No es UNION
				(IsKywAftFrom (m_pAuxString)))
			{
				if (bExistsOR)
				{
					pRight = pAux; // Apuntamos a la primera letra de la palabra reservada.
					bInsertOR = M4_TRUE;
				}
				bExit = M4_TRUE;
			}
		}
	}
	while (!bExit);

	if (bInsertOR)
	{
		*pLeft = *pLeft + 128;
		*pRight = *pRight + 128;
	}

	return;
}


m4return_t ClAPISQL::FindSeparator (m4char_t ai_cSeparator)
{
	m4char_t cSeparator = *(m_pCurrent - 1);

	if (cSeparator != ai_cSeparator)
	{
		// Vamos a quitar los blancos.
		for (; ((!cSeparator) || IsControlCharacter(cSeparator)); m_pCurrent++)
		{
			cSeparator = *m_pCurrent;
		}

		if (cSeparator != ai_cSeparator)
		{
			sprintf (m_pAuxString, "'%c'", ai_cSeparator);
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, m_pAuxString);
			return (M4_ERROR);
		}
	}

	return (M4_SUCCESS);
}


m4return_t	ClAPISQL::AnalizeOrderByWithItems(void)
{

	m4return_t result = M4_SUCCESS;
	m4int32_t	lenToken = 0;
	m4char_t	cSeparator = 0;
	m4bool_t	bIsWord = M4_FALSE;
	
	m4bool_t bFoundItem = M4_FALSE;
	m4bool_t bGo = M4_TRUE;

	while (bGo)
	{
		cSeparator=0;
		lenToken =0;
		
		result = _X_GetToken (m_pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal ());

		// Lo que venga tiene que ser un item. (Empezar con @);
		if (result == M4_SUCCESS)
		{
			if (bFoundItem==M4_FALSE)
			{
				if (*m_pAuxString=='@')
				{
					// OK.Tengo que analizar el item.
					if (AnalizeItemInOrderBy(m_pAuxString+1)!=M4_SUCCESS)
					{
						DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_BAD_ITEM_IN_ORDER_BY,m_pAuxString+1);
						return (M4_ERROR);
					}
					bFoundItem = M4_TRUE;
				}
				else
				{
					DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_ITEM_IN_ORDER_BY);
					return (M4_ERROR);
				}
			}
			else
			{
				// Puede ser un ASC, DESC o una coma.
				if (bIsWord)
				{
					// Tiene que venir ASC, DESC o , .
					if (!strcmp(m_pAuxString,"DESC") || !strcmp(m_pAuxString,"ASC"))
					{
						// !!!!!!!!
						// ¿Tengo que añadir el espacio?
						m_pstAux->append (" ");
						m_pstAux->append (m_pAuxString);
					}
					else
					{
						DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD,",|ASC|DESC");
						return (M4_ERROR);
					}

				}
			}
		}
		// Los demas separadores me dan igual.(Seran espacios o tabuladores, y me los puedo cargar).
		if (cSeparator==',')
		{
			m_pstAux->append (",");
			bFoundItem=M4_FALSE;
		}
		if (cSeparator==';'||cSeparator==0)
		{
			bGo = M4_FALSE;
		}
	}
	return M4_SUCCESS;
}


m4return_t	ClAPISQL::AnalizeItemInOrderBy(m4pcchar_t ai_pToken)
{
	
	if (ai_pToken==0 || m_pstAuxCol==0 || m_pvcAuxColItm==0)
	{
		// Necesitamos que se hayan analizado ya los items de las columnas.
		return M4_ERROR;
	}

	m4int32_t hItem = m_pGlobVars->GetItemHandleByIdItem (ai_pToken, m_bConnActivate);
	if (hItem==0)
	{
		return M4_ERROR;
	}
	// Tenemos que buscar una columna que tenga el mismo handle de item.
	
	itVcClMtColumn it= m_pvcAuxColItm->begin();
	itVcClMtColumn itend= m_pvcAuxColItm->end();

	ClMetaItemCol *pItemCol = 0;
	for (;it!=itend;it++)
	{
		if ((*it)->m_hItem==hItem)
		{
			// Tenemos la columna
			pItemCol = (*it);
			break;
		}
	}
	if (pItemCol == 0 )
	{
		// No existe ese item como columna.
		return M4_ERROR;
	}

	// Tenemos que:
	//	1.Copiar el contenido de la columna al order by.
	//	2.Copiar los campos que intervienen en esa columna.

	// 1. 
	m_pstAux->append(*m_pstAuxCol,pItemCol->m_lPos+1,pItemCol->m_lLength-1); // Copiamos quitando la coma inicial y la final.
	// 2.
	itVcClMtField itFld= m_pvcAuxColFld->begin();
	itVcClMtField itFldend= m_pvcAuxColFld->end();

	ClMetaField *pField = 0;
	m4bool_t bAdded = M4_FALSE;
	for (;itFld!=itFldend;itFld++)
	{
		pField = (*itFld);
		if (pField ->IsMetaData() == M4_TRUE)
		{
			if (pField ->m_iColumn==pItemCol->m_iColumn)
			{
				// Si pertenece a la misma columna. Lo copiamos.
				ClMetaField *pNewField = new ClMetaField(*pField);
				// Como pasa a estar en la parte Order By, le cambiamos sus propiedades.
				// Le asociamos al filtro 0 de nivel 0 (donde deberían estar las columnas).
				pNewField->m_eClause = M4LDB_CLAUSE_ORDER_BY;
				pNewField->m_iLevel = 0;
				pNewField->m_iFilter = 0;
				m_pvcAuxFld->push_back(pNewField);
				bAdded =M4_TRUE;
			}
			else
			{
				if (bAdded)
				{
					// Ya hemos pasado la columna en cuestion.
					break;
				}
			}
		}
	}		
	return M4_SUCCESS;
}


m4int32_t ClAPISQL::GetNumLgcTbl (void)
{
	// Puede ser que hayamos pasado los objetos al MetaSentence.
	if ((m_pvcAuxFrmObj) && (m_pvcAuxFrmObj->empty()==M4_FALSE))
	{
		return (m_pvcAuxFrmObj->size());
	}

	return (m_pMtSent->GetNumObject());
}


ClMetaObject *ClAPISQL::GetLgcTblByPos (m4int32_t ai_indTblLgc)
{
	// Puede ser que hayamos pasado los objetos al MetaSentence.
	if ((m_pvcAuxFrmObj) && (m_pvcAuxFrmObj->empty()==M4_FALSE))
	{
		return ( (*m_pvcAuxFrmObj)[ai_indTblLgc]);
	}

	return (m_pMtSent->GetObjectByPos(ai_indTblLgc));
}


m4return_t ClAPISQL::SetDDBBType (void)
{
	m4char_t cSeparator;
	m4bool_t bIsWord;
	m4int32_t lenToken;
	m4return_t result;

	// El tipo de base de datos por defecto lo sacamos de la conexion del sistema (indice 0)
	m4int32_t idRConn = 0;
	
	if (!m_pMtSent->IsReal ())
	{
		// Vamos a buscar la tabla base.
		idRConn = m_pGlobVars->GetIdRConn ();
		
		if (idRConn == M4LDB_NULL_INDEX)
		{
			m4pchar_t pCurrent = (m4pchar_t) m_pstAPISQLStm->c_str ();

			m4pchar_t pTblLgc = NULL;

			do
			{
				result = _X_GetToken (pCurrent, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal ());

				if (result !=M4_ERROR)
				{
					if (bIsWord)
					{
						switch (m_pMtSent->GetStmType ())
						{
							case M4LDB_SELECT:
								if (!strcmp (m_pAuxString, "FROM"))
								{
									m4int32_t lPos = m_pstAPISQLStm->find ("&", (m4int32_t) (pCurrent - (m4pchar_t) m_pstAPISQLStm->c_str ()));
									
									if (lPos == M4_STR_NPOS)
									{
										DUMP_LDBINSP_ERROR (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_BASIS_LOGIC_OBJECT);
										return (M4_ERROR);
									}
									pTblLgc = & ( (*m_pstAPISQLStm)[lPos+1]);
								}
								break;
								
							case M4LDB_DELETE:
								if (!strcmp (m_pAuxString, "DELETE"))
								{
									pTblLgc = pCurrent;
								}
								break;
								
							case M4LDB_INSERT:
								if (!strcmp (m_pAuxString, "INTO"))
								{
									pTblLgc = pCurrent;
								}
								break;
								
							case M4LDB_UPDATE:
								if (!strcmp (m_pAuxString, "UPDATE"))
								{
									pTblLgc = pCurrent;
								}
								break;
						}
					}
				}

			} while ((cSeparator != '\0') && (!pTblLgc));

			if (!pTblLgc)
			{
				switch (m_pMtSent->GetStmType ())
				{
					case M4LDB_SELECT:
						DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, "'FROM'");
						break;
						
					case M4LDB_DELETE:
						DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, "'DELETE'");
						break;
						
					case M4LDB_INSERT:
						DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, "'INTO'");
						break;
						
					case M4LDB_UPDATE:
						DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, "'UPDATE'");
						break;
					}
				return (M4_ERROR);
			}
					
			result = _X_GetToken (pTblLgc, m_pAuxString, lenToken, cSeparator, bIsWord, m_pMtSent->IsReal ());

			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
			
			result = m_pGlobVars->GetpLConn ()->LoadDictionaryChannel ((*m_pAuxString == '&') ? m_pAuxString+1 : m_pAuxString, NULL, M4_TRUE);
			
			if (result == M4_ERROR)
			{
				return (M4_ERROR);
			}
			
			// Obtenemos el identificador de la conexión.
			idRConn = M4LDB_NUMBER (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Register[(ClRegisterIndex) 0].Item[M4LDB_ITEM_SYS_DICT_REAL_OBJECTS_ID_CONN].Value.GetWithOutSec ());
		}		
	}

	m_pGlobVars->SetDDBBType (idRConn);

	return (M4_SUCCESS);
}


m4return_t ClAPISQL::CounterParenthesisAndQuotes (m4pchar_t ai_pStmt)
{
	m4bool_t bString = M4_FALSE;

	while (*ai_pStmt)
	{
		if (*ai_pStmt == '\'' && *(ai_pStmt-1) != '\\')
		{
			bString = M4_BOOL(!bString);
		}

		if (!bString)
		{
			// No forma parte de una cadena.
			if (*ai_pStmt == '(')
			{
				m_iopen++;
			}
			else
			{
				if (*ai_pStmt == ')')
				{
					m_iclose++;
				}
			}
		}

		ai_pStmt++;
	}

	if (m_iopen > m_iclose)
	{
		//Hay mas parentesis abiertos que cerrados
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, "')'");
		return (M4_ERROR);
	}

	if (m_iopen < m_iclose)
	{
		//Hay mas parentesis cerrados que abiertos
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, "'('");
		return (M4_ERROR);
	}

	if (bString == M4_TRUE)
	{
		//Hay una cadena abierta
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_MISSING_CHARACTER_OR_KEYWORD, " ' ");
		return (M4_ERROR);
	}

	return (M4_SUCCESS);
}



