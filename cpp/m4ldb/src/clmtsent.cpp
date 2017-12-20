//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb 
// File:                clmtsent.cpp   
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


#include "clmtsent.hpp"
#include "cldinarr.hpp"
#include "cllgadap.hpp"
#include "traductor.hpp"
// Para la macro M4LDB_PRINT_DEBUG. ( Habria que moverla ).
#include "clstatem.hpp"
#include "access.hpp"

#include "ldb_log.hpp"
#include "clchkjoins.hpp"

#include "clmtsent.inl"




m4pcchar_t			M4LDB_LONG_CLAUSE_SEP				="|L";
m4pcchar_t			M4LDB_COLUMN_CLAUSE_SEP				="|C";

m4pcchar_t			M4LDB_GEN_SEPARATOR				="|";

//Pedro
//Bug 0069267. Con 4 dígitos estamos permitiendo 0..99 selects porque dos se reservan.
//Si incrementamos a 6 permitimos 0..9999 selects (10000 selects).
m4uint8_t			M4LDB_MD_LENGTH_CLAUSE			=	6;

//Esta función construye un string en función del entero que se le pasa.
//El tamaño del string incluye |x
//Es decir incluye dos caracteres adicionales. Por lo tanto, el espacio reservado
//a dígitos es M4LDB_MD_LENGTH_CLAUSE - 2
string GetStringM4LDB_MD_LENGTH_CLAUSE (m4int32_t ai_Level)
{
	m4char_t sLevel[M4LDB_SIZE_DEFAULT_STRING];	

	//Tendremos una cadena que representará el nivel
	//En el caso de que M4LDB_MD_LENGTH_CLAUSE = 6.
	//Tendremos 4 dígitos para representar el nivel:
	//Si nivel es 0..9			--> 000Nivel
	//Si nivel es 10..99		--> 00Nivel
	//Si nivel es 100..999		--> 0Nivel
	//Si nivel es 1000..9999	--> Nivel

	sprintf(sLevel, "%-0.*ld",M4LDB_MD_LENGTH_CLAUSE - 2, ai_Level);
	return sLevel;
}
//Fin Pedro

m4uint8_t			M4LDB_FIELD_LENGTH				=	2;
m4pcchar_t			M4LDB_POSIBLE_CLAUSES			="BCLFWROE";




m4pcchar_t ClauseType(eClauseType_t ai_Clause)
{
	switch (ai_Clause)
	{
	case M4LDB_CLAUSE_BEGIN_STATEM:
		return "BEGIN STATEMENT";

	case M4LDB_CLAUSE_COLUMN:
		return "COLUMN";

	case M4LDB_CLAUSE_LONG_COLUMN:
		return "LONG COLUMN";

	case M4LDB_CLAUSE_FROM:
		return "FROM";

	case M4LDB_CLAUSE_WHERE:
		return "WHERE";

	case M4LDB_CLAUSE_REST:
		return "REST";

	case M4LDB_CLAUSE_ORDER_BY:
		return "ORDER BY";

	case M4LDB_CLAUSE_END_STATEM:
		return "END STATEMENT";

	default:
		return "NULL";
	}
}


template<class Par>
void _ColumnDeleter<Par>::operator()(vector<Par* > * ai_pvcSource,m4int32_t ai_indCol)
{
	if (ai_pvcSource==0)
	{
		return;
	}
	if (ai_pvcSource->empty())
	{
		return;
	}
	
	M4_TYPE_ITERATOR vector<Par* >::iterator itvc = ai_pvcSource->begin();

	Par * pPar = 0;
	M4_TYPE_ITERATOR vector<Par* >::iterator ittodelete = ai_pvcSource->end();
	for (;itvc!=ai_pvcSource->end();++itvc)
	{
		pPar = (*itvc);
		if (pPar->IsMetaData() == M4_TRUE)
		{
			if (pPar->m_iColumn == ai_indCol)
			{
				if (pPar->m_eClause == M4LDB_CLAUSE_LONG_COLUMN || pPar->m_eClause == M4LDB_CLAUSE_COLUMN)
				{
					delete pPar;
					// me lo guardo para borrarl despues. Si lo borro ahora se 
					// me descuadra el iterador.
					ittodelete = itvc;
				}
				
			}
			else
			{
				if (pPar->m_iColumn > ai_indCol)
				{
					--pPar->m_iColumn;
				}
			}

		}
	}
	if (ittodelete!=ai_pvcSource->end())
	{
		ai_pvcSource->erase(ittodelete);
	}

	return;
}


template <class Par>
void _VectorDumper<Par>::operator()(ofstream &ofs,vector<Par* > &ai_vc)
{
	M4_TYPE_ITERATOR vector<Par* >::iterator	itVC =ai_vc.begin();
	for (;itVC!=ai_vc.end();++itVC)
	{
		(*itVC)->Dump(ofs);
		ofs<<endl;
	}
}


template <class Par>
m4return_t _ColumnAppender<Par>::operator()(vector<Par* > * ai_pvcSource,vector<Par* > * ai_pvcTarget,m4bool_t ai_bIsLong,m4int32_t ai_nCols)
{
	// Miramos si hay algo que hacer.
	if (ai_pvcSource==0)
	{
		return M4_SUCCESS;
	}
	if (ai_pvcSource->empty())
	{
		return M4_SUCCESS;
	}
	// Nos situamos en el vector de Param de Columna.
	
	M4_ASSERT(ai_pvcTarget);
	
	M4_TYPE_ITERATOR vector<Par* >::iterator itMt;

	// Nos situamos antes del separador de Long.
	itMt = ai_pvcTarget->begin();

	while (itMt!=ai_pvcTarget->end())
	{
		// Paramos cuando sea separador de tipo long;
		if ((*itMt)->IsSeparator()==M4_TRUE)
		{
			if ((*itMt)->m_eClause== M4LDB_CLAUSE_LONG_COLUMN)
			{
				break;
			}
		}
		++itMt;
	}
	if (ai_bIsLong == M4_TRUE)
	{
		// Seguimos hasta la siguiente clausula o hasta el final.
		++itMt;
		while (itMt!=ai_pvcTarget->end())
		{
			// Paramos en el sigiuente separador
			if ((*itMt)->IsSeparator()==M4_TRUE)
			{
				break;
			}
			++itMt;
		}
		M4_TYPE_ITERATOR vector<Par* >::iterator itMtSource = ai_pvcSource->begin();
		while (itMtSource!=ai_pvcSource->end())
		{
			(*itMtSource)->m_iColumn+=ai_nCols;
			++itMtSource;
		}
	}
	// Insertamos la info.
	// En caso de que soportemos añadir columnas en filtros, hay que cambiar los offsets de todos los que vamos a insertar.
	ai_pvcTarget->insert (itMt,ai_pvcSource->begin(),ai_pvcSource->end());
	ai_pvcSource->clear(); // Le quitamos las referencias al vector fuente.

	return M4_SUCCESS;
}


template <class Par>
m4return_t _VectorInserter<Par>::operator()(vector<Par* > *pvcSource,vector<Par* >  *pvcTarget,eClauseType_t ai_eClause,m4int32_t ai_Level)
{
	if (pvcSource==0||pvcTarget==0)
	{
		return M4_SUCCESS;
	}

	if (pvcSource->empty()){
		// Tiene que tener por lo menos marcas de END, despues de la clausula que tengamos que copiar.
		return M4_ERROR;
	}

	// Comprobamos que el iterador este en el separador de la clausula que necesitamos.
	M4_TYPE_ITERATOR vector<Par* >::iterator itSource = pvcSource->begin();

	if ( (*itSource)->IsMetaData() == M4_TRUE )
	{
		return M4_ERROR;
	}
	// No hay que añadir para esta clausula.
	if ( (*itSource)->m_eClause !=ai_eClause  )
	{
		return M4_SUCCESS;
	}
	// No hay que añadir para este nivel.
	if ( (*itSource)->m_iLevel!=ai_Level )
	{
		return M4_SUCCESS;
	}


	// Buscamos la posicion para insertar.
	M4_TYPE_ITERATOR vector<Par* >::iterator itTarget = pvcTarget->begin();
	
	for (;itTarget!=pvcTarget->end();++itTarget)
	{
		if ((*itTarget)->IsSeparator() == M4_TRUE)
		{
			if ((*itTarget)->m_eClause == ai_eClause )
			{
				if ((*itTarget)->m_iLevel == ai_Level )
				{
					// Encontrado.
					break;
				}

			}
		}
	}
	// Deberiamos estar posicionados.
	if (itTarget==pvcTarget->end())
	{
		// Deberiamos tener esa clausula.
		return M4_ERROR;
	}

	// Nos movemos al siguiente para insertar antes
	++itTarget;


	// Buscamos el siguiente separador con el mismo nivel en el vector fuente .
	// Por ejemplo si estuvieramos en un W00 copio hasta R00 aunque venga en medio B01...E01

	M4_TYPE_ITERATOR vector<Par* >::iterator itSourceEnd = itSource+1;

	for (;;++itSourceEnd)
	{
		if ((*itSourceEnd)->IsSeparator() == M4_TRUE )
		{
			if ((*itSourceEnd)->m_iLevel == ai_Level)
			{
				break;
			}
		}
	}

	pvcTarget->insert(itTarget,itSource+1,itSourceEnd);

	// Limpiamos el vector ( borramos el separador, y quitamos las entradas que hemos metido.
	// Los que no son separadores no los borro porque me he copiado las referencias.
	
	delete *(itSource);	// Separador.
	pvcSource->erase(itSource,itSourceEnd);

	return M4_SUCCESS;


}
template <class Par>
m4return_t _VectorInserter<Par>::operator()(vector<Par* > *pvcSource,vector<Par* >  *pvcTarget,eClauseType_t ai_eClause,m4int32_t ai_Level,m4int32_t ai_iAddToOr)
{
	if (pvcSource==0||pvcTarget==0)
	{
		return M4_SUCCESS;
	}

	if (pvcSource->empty())
	{
		// Tiene que tener por lo menos marcas de END, despues de la clausula que tengamos que copiar.
		return M4_ERROR;
	}

	// Comprobamos que el iterador este en el separador de la clausula que necesitamos.
	M4_TYPE_ITERATOR vector<Par* >::iterator itSource = pvcSource->begin();

	if ( (*itSource)->IsMetaData() == M4_TRUE )
	{
		return M4_ERROR;
	}
	// No hay que añadir para esta clausula.
	if ( (*itSource)->m_eClause !=ai_eClause )
	{
		return M4_SUCCESS;
	}
	// No hay que añadir para este nivel.
	if ( (*itSource)->m_iLevel!=ai_Level  )
	{
		return M4_SUCCESS;
	}


	m4int32_t iAddToOr = 0;
	// Buscamos la posicion para insertar.
	m4int32_t iTarget = 0;
	m4int32_t nTarget = pvcTarget->size ();
	
	for (iTarget = 0;iTarget<nTarget;iTarget++)
	{
		M4_TYPE_ITERATOR vector<Par* >::iterator itTarget = pvcTarget->begin() + iTarget;
		if ((*itTarget)->IsAddToOr () == M4_TRUE)
		{
			if ((*itTarget)->IsBegin () == M4_FALSE)
			{
				iAddToOr++;
				if (iAddToOr == ai_iAddToOr)
				{
					// Encontrado.
					break;
				}
			}
		}
	}

	// Deberiamos estar posicionados.
	if (iTarget==nTarget)
	{
		// Deberiamos tener esa clausula.
		return M4_ERROR;
	}

	// Buscamos el siguiente separador con el mismo nivel en el vector fuente .
	// Por ejemplo si estuvieramos en un W00 copio hasta R00 aunque venga en medio B01...E01

	M4_TYPE_ITERATOR vector<Par* >::iterator itSourceEnd = itSource+1;

	for (;;++itSourceEnd)
	{
		M4_TYPE_ITERATOR vector<Par* >::iterator itTarget = pvcTarget->begin() + iTarget;
		if ((*itSourceEnd)->IsSeparator() == M4_TRUE )
		{
			if ((*itSourceEnd)->m_iLevel == ai_Level)
			{
				break;
			}
		}
		Par* pSource = (Par*) (*itSourceEnd)->Clone ();
		pvcTarget->insert(itTarget,pSource);
		iTarget++;
	}

	return M4_SUCCESS;
}


template <class Par>
m4return_t _VectorInserter<Par>::operator()(Par* ai_pSource,vector<Par* >  *pvcTarget, m4int32_t ai_Level,m4int32_t ai_iAddToOr)
{
	if (ai_pSource==0||pvcTarget==0)
	{
		return M4_SUCCESS;
	}


	m4int32_t iAddToOr = 0;
	// Buscamos la posicion para insertar.
	m4int32_t iTarget = 0;
	m4int32_t nTarget = pvcTarget->size ();
	
	for (iTarget = 0;iTarget<nTarget;iTarget++)
	{
		M4_TYPE_ITERATOR vector<Par* >::iterator itTarget = pvcTarget->begin() + iTarget;
		if ((*itTarget)->IsAddToOr () == M4_TRUE)
		{
			if ((*itTarget)->IsBegin () == M4_FALSE)
			{
				iAddToOr++;
				if (iAddToOr == ai_iAddToOr)
				{
					// Encontrado.
					break;
				}
			}
		}
	}

	// Deberiamos estar posicionados.
	if (iTarget==nTarget)
	{
		// Deberiamos tener esa clausula.
		return M4_ERROR;
	}


	Par* pSource = (Par*) ai_pSource->Clone ();
	
	pvcTarget->insert(pvcTarget->begin() + iTarget,pSource);

	return M4_SUCCESS;
}


template <class Par>
m4return_t _VectorClearLevel<Par>::operator()(vector<Par* > *pvcSource,eClauseType_t ai_eClause,m4int32_t ai_Level)
{
	if (pvcSource==0)
	{
		return M4_SUCCESS;
	}

	if (pvcSource->empty())
	{
		// Tiene que tener por lo menos marcas de END, despues de la clausula que tengamos que copiar.
		return M4_ERROR;
	}

	// Comprobamos que el iterador este en el separador de la clausula que necesitamos.
	M4_TYPE_ITERATOR vector<Par* >::iterator itSource = pvcSource->begin();

	if ( (*itSource)->IsMetaData() == M4_TRUE )
	{
		return M4_ERROR;
	}
	// No hay que añadir para esta clausula.
	if ( (*itSource)->m_eClause !=ai_eClause )
	{
		return M4_SUCCESS;
	}
	// No hay que añadir para este nivel.
	if ( (*itSource)->m_iLevel!=ai_Level  )
	{
		return M4_SUCCESS;
	}

	M4_TYPE_ITERATOR vector<Par* >::iterator itSourceEnd = itSource+1;

	for (;;++itSourceEnd)
	{
		if ((*itSourceEnd)->IsSeparator() == M4_TRUE )
		{
			if ((*itSourceEnd)->m_iLevel == ai_Level)
			{
				break;
			}
		}
		delete *(itSourceEnd);
	}

	// Limpiamos el vector ( borramos el separador, y quitamos las entradas que hemos metido.
	// Los que no son separadores no los borro porque me he copiado las referencias.
	
	pvcSource->erase(itSource,itSourceEnd);

	return M4_SUCCESS;
}




template <class Par1,class Par2,class Par3>
_VectorClearer<Par1,Par2,Par3>::~_VectorClearer(void)
{
	if (m_pv1)
	{
		_VectorClearerOne<Par1>	oClP1(*m_pv1);
		m_pv1=0;
	}
	if (m_pv2)
	{
		_VectorClearerOne<Par2>	oClP2(*m_pv2);
		m_pv2=0;
	}
	if (m_pv3)
	{
		_VectorClearerOne<Par3>	oClP3(*m_pv3);
		m_pv3=0;
	}

}


ClMetaSentence::ClMetaSentence (void)
{ 
	m_bReal = M4_FALSE;
	m_pParam = 0;
	m_lSecMask = M4LDB_DEFAULT_SEC_MASK;
	m_lAudMask = M4LDB_SUPERROLE_AUD_MASK;
	m_indInitCol = M4LDB_NULL_INDEX;
	m_indDtLastUpdate = M4LDB_NULL_INDEX;
	//	m_bAuxMtSent = M4_FALSE;

	//Bug 0071661.
	m_bNoWait = M4_FALSE;

	m_bForUpdate = M4_FALSE;
	m_bIsDistinct= M4_FALSE;

	m_bHasRange = M4_FALSE;
	m_bPartialLoad = M4_FALSE;
	m_bHasValidation = M4_TRUE;
	m_bHasOr = M4_FALSE;

	m_bDontUseAlias = M4_FALSE;
	m_bExistsInheritance = M4_FALSE;

	m_pGlobVars = 0;
	m_eStmType = M4LDB_SELECT;
//	m_hConn = M4LDB_NULL_VALUE;
	
	// Columnas
	m_pstStatemCol = 0;
	m_pvcParamCol = 0;
	m_pvcColumns = 0;

	// Where
	m_pstStatemWhr = 0;
	m_pvcParamWhr = 0;
	m_pvcRel = 0;

	// Fields
	m_pvcFld = 0;

	// Objects
	m_pvcFrmObj = 0;
	m_pvcFrmAls = 0;
	m_iLastObjectSize = 0;

	m_pmpFieldAndValue = 0;

	m_numCol = -1;
	m_numPar = -1;
	m_numColNVL = 0;
	m_idAudObj = NULL;
		
	//Pedro.
	//Por defecto no se valida seguridad.
	m_bIsValidatingSecurity = M4_FALSE;

	//Bug 0081719
	//Tendremos un Flag que permitira la ordenacion de parametros en el traductor.
	m_bSortParamsColumnsLimitation = M4_FALSE;

	m_pVcSysHint = NULL;
	m_pVcDynamicHint = NULL;

	m_bIsInsertIntoSelect = M4_FALSE ;
	m_bIsUpdateSelect = M4_FALSE ;
	m_iMainFilter = 0;
}


ClMetaSentence::~ClMetaSentence (void)
{
	Clear ();
}


void ClMetaSentence::Clear (void)
{
	// Columnas
	_ClearColumnsElements();
	// Where
	_ClearWhereElements();
	// Fields
	_ClearFields();


	// Objects
	if (m_pvcFrmObj)
	{
		for (itVcClMtObject it=m_pvcFrmObj->begin (); it!=m_pvcFrmObj->end(); it++)
		{
			delete (*it);
		}
		delete m_pvcFrmObj;
	}
	m_pvcFrmObj = 0;

	if (m_pvcFrmAls)
	{
		for (itVcClMtAlias it=m_pvcFrmAls->begin (); it!=m_pvcFrmAls->end(); it++)
		{
			delete (*it);
		}
		delete m_pvcFrmAls;
	}
	m_pvcFrmAls = 0;

	if (m_pmpFieldAndValue)
	{
		for (itMpFieldAndValue it=m_pmpFieldAndValue->begin (); it!=m_pmpFieldAndValue->end(); it++)
		{
			delete (*it).second;
		}
		delete m_pmpFieldAndValue;
	}
	m_pmpFieldAndValue = 0;

	m_bDontUseAlias = M4_FALSE;

	if (m_pVcSysHint != NULL)
	{
		for (itVcSysHint it=m_pVcSysHint->begin(); it!=m_pVcSysHint->end(); it++)
		{
			ClSysHint * pClSysHint = (*it);
			if (pClSysHint != NULL)
			{
				delete pClSysHint;
			}
		}
		delete m_pVcSysHint;
		m_pVcSysHint = NULL;
	}

	if (m_pVcDynamicHint != NULL)
	{
		for (itVcDynamicHint it=m_pVcDynamicHint->begin(); it!=m_pVcDynamicHint->end(); it++)
		{
			ClDynamicHint * pClDynamicHint = (*it);
			if (pClDynamicHint != NULL)
			{
				delete pClDynamicHint;
			}
		}
		delete m_pVcDynamicHint;
		m_pVcDynamicHint = NULL;
	}
}


void ClMetaSentence::_ClearColumnsElements(void)
{
	// Columnas
	if (m_pstStatemCol)
	{
		delete m_pstStatemCol;
	}
	m_pstStatemCol= 0;

	{
		_VectorClearer<ClMetaItemCol,ClMetaParam,ClMetaParam> oClear(m_pvcColumns,m_pvcParamCol);
	}

	if (m_pvcColumns)
	{
		delete m_pvcColumns;
		m_pvcColumns=0;
	}
	if (m_pvcParamCol)
	{
		delete m_pvcParamCol;
		m_pvcParamCol=0;
	}
}


void ClMetaSentence::_ClearFields(void)
{
	{
		_VectorClearer<ClMetaField,ClMetaField,ClMetaField> oClean(m_pvcFld);
	}

	if (m_pvcFld){
		delete m_pvcFld;
		m_pvcFld = 0;
	}
}


void ClMetaSentence::_ClearWhereElements(void)
{
	if (m_pstStatemWhr)
	{
		delete m_pstStatemWhr;
	}
	m_pstStatemWhr= 0;

	{
		_VectorClearer<ClMetaParam,ClMetaRelat,ClMetaParam> oClean(m_pvcParamWhr,m_pvcRel);
	}

	if (m_pvcParamWhr)
	{
		delete m_pvcParamWhr;
		m_pvcParamWhr=0;
	}

	if (m_pvcRel)
	{
		delete m_pvcRel;
		m_pvcRel = 0;
	}
}


m4return_t ClMetaSentence::_CreateFld()
{
	if ( m_pvcFld == 0 )
	{
		m_pvcFld = new VcClMtField;
		m_pvcFld ->reserve(20);
	}

	_InsertFieldSeparator(M4LDB_CLAUSE_BEGIN_STATEM,0);
	_InsertFieldSeparator(M4LDB_CLAUSE_COLUMN,0);
	_InsertFieldSeparator(M4LDB_CLAUSE_LONG_COLUMN,0);
	_InsertFieldSeparator(M4LDB_CLAUSE_FROM,0);
	_InsertFieldSeparator(M4LDB_CLAUSE_WHERE,0);
	_InsertFieldSeparator(M4LDB_CLAUSE_REST,0);
	_InsertFieldSeparator(M4LDB_CLAUSE_ORDER_BY,0);
	_InsertFieldSeparator(M4LDB_CLAUSE_END_STATEM,0);

	return M4_SUCCESS;
}


m4return_t ClMetaSentence::_CreateColumnStatem(void)
{

	if (m_pstStatemCol != 0 || m_pvcColumns !=0|| m_pvcParamCol != 0 )
	{
		_ClearColumnsElements();
		_ClearFields();
	}

	//Pedro
	//Bug 0069267. Permitimos que haya 4 caracteres para el nivel	
	string sFillString;
	//Queremos que nos devuelva 0000 (si hubiera 4 caracteres reservados)
	sFillString = GetStringM4LDB_MD_LENGTH_CLAUSE(0);
	m_pstStatemCol = new string("|B");
	m_pstStatemCol->append(sFillString);
	m_pstStatemCol->append("|C");
	m_pstStatemCol->append(sFillString);
	m_pstStatemCol->append("|L");
	m_pstStatemCol->append(sFillString);
	//m_pstStatemCol = new string("|B0000|C0000|L0000");


	// Vector de columnas.
	m_pvcColumns = new VcClMtColumn;
	m_pvcColumns ->reserve(20);	// A ojillo.
	// Vector de Parametros de columnas.
	m_pvcParamCol = new VcClMtParam;
	m_pvcParamCol  ->reserve(10);	// A ojillo.
	if ( m_pvcFld == 0 )
	{
		_CreateFld();
	}
	
	_InsertColumnSeparator(M4LDB_CLAUSE_BEGIN_STATEM,0);
	_InsertColumnSeparator(M4LDB_CLAUSE_COLUMN,0);
	_InsertColumnSeparator(M4LDB_CLAUSE_LONG_COLUMN,0);

	return M4_SUCCESS;
}


m4return_t ClMetaSentence::_CreateWhereStatem(void)
{
	if (m_pstStatemWhr!= 0 || m_pvcRel !=0|| m_pvcParamWhr != 0 )
	{
		_ClearWhereElements();
		_ClearFields();
	}

	//Pedro
	//Bug 0069267. Permitimos que haya 4 caracteres para el nivel	
	string sFillString;
	//Queremos que nos devuelva 0000 (si hubiera 4 caracteres reservados)
	sFillString = GetStringM4LDB_MD_LENGTH_CLAUSE(0);
	m_pstStatemWhr = new string("|F");
	m_pstStatemWhr->append(sFillString);
	m_pstStatemWhr->append("|W");
	m_pstStatemWhr->append(sFillString);
	m_pstStatemWhr->append("|R");
	m_pstStatemWhr->append(sFillString);
	m_pstStatemWhr->append("|O");
	m_pstStatemWhr->append(sFillString);
	m_pstStatemWhr->append("|E");
	m_pstStatemWhr->append(sFillString);
	//m_pstStatemWhr = new string("|F0000|W0000|R0000|O0000|E0000");		
	//Fin Pedro

	// Vector de relaciones.
	m_pvcRel = new VcClMtRelat;
	m_pvcRel ->reserve(20);	// A ojillo.

	// Vector de Parametros del filtro.
	m_pvcParamWhr = new VcClMtParam;
	m_pvcParamWhr  ->reserve(10);	// A ojillo.
	if ( m_pvcFld == 0 )
	{
		_CreateFld();
	}

	_InsertWhereSeparator(M4LDB_CLAUSE_FROM,0);
	_InsertWhereSeparator(M4LDB_CLAUSE_WHERE,0);
	_InsertWhereSeparator(M4LDB_CLAUSE_REST,0);
	_InsertWhereSeparator(M4LDB_CLAUSE_ORDER_BY,0);
	_InsertWhereSeparator(M4LDB_CLAUSE_END_STATEM,0);

	return M4_SUCCESS;
}


m4return_t ClMetaSentence::Create (ClGlobVars *ai_pGlobVars, eStmType_t ai_eStmType, m4bool_t ai_bAPISQL, m4bool_t ai_bReal, m4bool_t ai_bHasRange, m4bool_t ai_bPartialLoad, m4bool_t ai_bHasValidation)
{
	m_eStmType = ai_eStmType;
	m_bAPISQL = ai_bAPISQL;
	m_pGlobVars = ai_pGlobVars;
	m_eStmType = ai_eStmType;
	m_bReal = ai_bReal;
	m_bHasRange = ai_bHasRange;

	if( m_bHasRange == M4_FALSE )
	{
		// Si va por rangos no hay carga parcial
		m_bPartialLoad = ai_bPartialLoad;
	}

	m_bHasValidation = ai_bHasValidation;

	if (m_pvcFrmAls==0)
	{
		m_pvcFrmAls = new VcClMtAlias;
	}
	if (m_pvcFrmObj==0)
	{
		m_pvcFrmObj = new VcClMtObject;
	}

	return (M4_SUCCESS);
}


m4return_t ClMetaSentence::_AddInfoColumns	( PVcClMtColumn ai_pVcClMtCol,PVcClMtParam ai_pvcParamCol,PVcClMtField ai_pvcFieldCol,string *ai_pstColumns,m4bool_t ai_bIsLong,m4uint32_t ai_iColumnStart)
{
	_VectorClearer<ClMetaItemCol,ClMetaParam,ClMetaField> oClean(ai_pVcClMtCol,ai_pvcParamCol,ai_pvcFieldCol);
	_StringClearer ostClear(ai_pstColumns);

	if (!ai_pVcClMtCol && 
		!ai_pvcParamCol && 
		!ai_pvcFieldCol && 
		!ai_pstColumns)
	{
		return (M4_SUCCESS);
	}

	// Si estamos aquí presuponemos que ai_pvcFieldCol esta rellena, pero ai_pvcParamCol
	// no tiene por qué en el caso de un update select, insert select
	if ((m_eStmType == M4LDB_UPDATE || (m_eStmType == M4LDB_INSERT)) && ai_pvcParamCol &&(!ai_pvcParamCol->empty ()) )
	{
		return _AddInfoColumnsUpdateInsert(ai_pvcParamCol,ai_pvcFieldCol);
	}

	if (_PrepareColumnStatem()!=M4_SUCCESS)
	{
		return M4_ERROR;
	}

	// Añadimos la nueva informacion de las columnas.
	// String
	if (ai_pstColumns && ai_pstColumns->empty()==0)
	{
		if( m_bHasRange == M4_TRUE )
		{
			m4uint32_t	iIncrement = 0 ;
			m4uint32_t	i = 0 ;
			m4uint32_t	iLength = 0 ;
			m4uint32_t	iSize = ai_pVcClMtCol->size() ;
			m4char_t	acBuffer[ 128 + 1 ] ;

			for( i = 0 ; i < iSize ; i++ )
			{
				ClMetaItemCol *pCol = (*ai_pVcClMtCol)[ i ] ;

				sprintf( acBuffer, " C%d$", i + ai_iColumnStart + 1 ) ;
				iLength = strlen( acBuffer ) ;

				pCol->m_lPos += iIncrement ;
				ai_pstColumns->insert( pCol->m_lPos + pCol->m_lLength, acBuffer ) ;
				pCol->m_lLength += iLength ;
				iIncrement += iLength ;
			}
		}

		if (_AppendColumnsString(*ai_pstColumns,ai_bIsLong)!=M4_SUCCESS)
		{
			return M4_ERROR;
		}
	}
	// Columnas que llevamos hasta ahora.
	m4int32_t CurrentCols=0;


	if (ai_pVcClMtCol)
	{
		m4int32_t sizenewcols = ai_pVcClMtCol->size();
		if (sizenewcols)
		{
			// Columnas que añadimos ( no vienen separadores ).
			if (m_numCol==-1)
			{
				CurrentCols=0;
				m_numCol=sizenewcols;
			}
			else
			{
				CurrentCols= m_numCol;
				m_numCol+=sizenewcols;
			}

			// hay que añadir columnas por si hay que emular el NVL.
			if (ai_bIsLong && ai_pvcFieldCol)
			{
				itVcClMtField itFld = ai_pvcFieldCol->begin();
				while (itFld !=ai_pvcFieldCol->end())
				{
					if ((*itFld)->m_indRealTable2!=M4LDB_NULL_INDEX && (*itFld)->IsMetaData() == M4_TRUE)
					{
						++m_numColNVL;
					}
					++itFld;
				}
			}
		}
	}

	// Columns
	_ColumnAppender<ClMetaItemCol> oColAppender;
	if (oColAppender(ai_pVcClMtCol,m_pvcColumns,ai_bIsLong,CurrentCols)!=M4_SUCCESS)
	{
		return M4_ERROR;
	}
	// Param
	_ColumnAppender<ClMetaParam> oParamAppender;
	if (oParamAppender(ai_pvcParamCol,m_pvcParamCol,ai_bIsLong,CurrentCols)!=M4_SUCCESS)
	{
		return M4_ERROR;
	}
	// Fields.
	_ColumnAppender<ClMetaField> oFieldAppender;
	if (oFieldAppender(ai_pvcFieldCol,m_pvcFld,ai_bIsLong,CurrentCols)!=M4_SUCCESS)
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
}


m4return_t ClMetaSentence::_AppendColumnsString(string &ai_stColumns,m4bool_t ai_bIsLong)
{
	m4int32_t InsertPos =M4_STR_NPOS;
	M4_ASSERT(m_pstStatemCol);
	if (ai_bIsLong)
	{
		// Buscamos el final de los Long.
		InsertPos = m_pstStatemCol->size();
	}
	else
	{
		// Buscamos el comienzo de los Long.
		InsertPos = m_pstStatemCol->find(M4LDB_LONG_CLAUSE_SEP);
		if (InsertPos==M4_STR_NPOS)
		{
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (),M4_CH_LDB_INTERNAL_ERROR,"_AppendColumnsString"<<"LongClause not found");			
			return M4_ERROR;
		}
	}
	m_pstStatemCol->insert(InsertPos,ai_stColumns);

	return M4_SUCCESS;
}


m4return_t ClMetaSentence::_InsertColumnSeparator(eClauseType_t ai_eClause,m4int32_t ai_Level)
{
	// Insertamos en el vector de parametros y en el de columnas.
	m_pvcColumns -> push_back(new ClMetaItemCol_Sep(ai_Level,ai_eClause));
	m_pvcParamCol -> push_back(new ClMetaParam_Sep(ai_Level,ai_eClause));
	return M4_SUCCESS;
}


m4return_t ClMetaSentence::_InsertFieldSeparator(eClauseType_t ai_eClause,m4int32_t ai_Level)
{
	// Insertamos en el vector de columnas.
	m_pvcFld->push_back(new ClMetaField_Sep(ai_Level,ai_eClause));
	return M4_SUCCESS;
}


m4return_t ClMetaSentence::_InsertWhereSeparator	(eClauseType_t ai_eClause,m4int32_t ai_Level)
{
	m_pvcParamWhr->push_back(new ClMetaParam_Sep(ai_Level,ai_eClause));
	return M4_SUCCESS;
}


m4return_t ClMetaSentence::_PrepareColumnStatem(void)
{
	if (m_pstStatemCol == 0)
	{
		// Vamos a crearlo.
		if (_CreateColumnStatem()!=M4_SUCCESS)
		{
			return M4_ERROR;
		}
	}
	return M4_SUCCESS;
}


m4return_t ClMetaSentence::_PrepareWhereStatem(void)
{
	if (m_pstStatemWhr == 0)
	{
		// Vamos a crearlo.
		if (_CreateWhereStatem()!=M4_SUCCESS)
		{
			return M4_ERROR;
		}
	}
	return M4_SUCCESS;
}


m4return_t ClMetaSentence::AddInfoFilter( PVcClMtParam ai_vParamWhr,PVcClMtField ai_vFieldWhr,string *ai_pstFilter, m4bool_t ai_bProcessOrs)
{
	eClauseType_t eClause;
	m4int32_t iLevel;
	m4int32_t lFromPos = 0;	m4uint32_t FPos = 0;
	m4int32_t lNextClausePos = 0;
	_VectorClearer<ClMetaParam,ClMetaField,ClMetaField> oClear(ai_vParamWhr,ai_vFieldWhr,0);
	_StringClearer	ostClear(ai_pstFilter);

	// Por si no hubiera nada que hacer.
	if (ai_pstFilter==0 || 
		ai_pstFilter->empty())
	{
		return M4_SUCCESS;
	}

	if (_PrepareWhereStatem()!=M4_SUCCESS)
	{
		return M4_ERROR;
	}

	// Miramos a ver lo que tenemos :

	while (1)
	{
		// Nos vamos a recorrer todos los From, que vengan en el filtro añadido.
		// Para cada From vamos añadiendo en nuestros vectores.
		lFromPos = _GetNextClause(*ai_pstFilter,lFromPos,eClause,iLevel);
		if (lFromPos !=M4_STR_NPOS)
		{
			if (eClause!=M4LDB_CLAUSE_FROM)
			{
				// Nos tenían que haber dejado en un From. 
				// Si es la primera tiene que empezar en From.
				return M4_ERROR;
			}



			if (_AddInfoFilter(ai_vParamWhr, ai_vFieldWhr, *ai_pstFilter, eClause, iLevel, lFromPos, ai_bProcessOrs)!=M4_SUCCESS)
			{

				return M4_ERROR;
			}
		}
		else
		{
			break;		
		}
	}

#ifdef _DEBUG 
	// Comprobamos que esten vacios todos los vectores.
	if (ai_vParamWhr)
	{
		M4_ASSERT(ai_vParamWhr->empty());
	}
	if (ai_vFieldWhr)
	{
		M4_ASSERT(ai_vFieldWhr->empty());
	}
#endif

	return M4_SUCCESS;
}


m4int32_t ClMetaSentence::_GetNextClause(string &ai_stStatem,m4int32_t ai_lOrigin,eClauseType_t &ai_eClause,m4int32_t &ai_Level)
{
	if ((m4int32_t)ai_stStatem.size()<= ai_lOrigin)
	{
		return M4_STR_NPOS;
	}
	
	m4pcchar_t pBuf = ai_stStatem.c_str();
	const m4char_t * pTemp = pBuf + ai_lOrigin;

	m4char_t ccc = *pTemp;
	m4int32_t nCom=0;

	while (ccc)
	{
		switch (ccc)
		{
			case '\'':
				++nCom;
				break;
			case '|':
				// Lo unico que controlo es que el numero de comillas sea par.
				if (nCom%2==0)
				{
					m4char_t clause =*(pTemp+1);
					if (strchr(M4LDB_POSIBLE_CLAUSES,clause)!=0)
					{
						if (_GetClauseAndLevel(pTemp+1,ai_eClause,ai_Level)==M4_SUCCESS)
						{ 
							return (pTemp-pBuf);
						}
						// Seguimos buscando.
					}
				}
				break;
//			default:
//				break;
		}
		ccc = *(++pTemp);
	}


	return M4_STR_NPOS;
}


eClauseType_t ClMetaSentence::_GetClauseType (m4char_t ai_clause)
{
	switch (ai_clause)
	{
	case 'B':
		return M4LDB_CLAUSE_BEGIN_STATEM;

	case 'C':
		return M4LDB_CLAUSE_COLUMN;

	case 'L':
		return M4LDB_CLAUSE_LONG_COLUMN;

	case 'F':
		return M4LDB_CLAUSE_FROM;

	case 'W':
		return M4LDB_CLAUSE_WHERE;

	case 'R':
		return M4LDB_CLAUSE_REST;

	case 'O':
		return M4LDB_CLAUSE_ORDER_BY;

	case 'E':
		return M4LDB_CLAUSE_END_STATEM;

	default:
		return M4LDB_CLAUSE_END_STATEM;
	}
}


m4int32_t ClMetaSentence::_GetLevel (const m4char_t *ai_Level)
{
	//Pedro
	//Bug 0069267. Permitimos que haya 4 caracteres para el nivel
	m4int32_t Level = 0;
	m4int32_t Pow = 1;

	//Recordar que del tamaño se reservan dos dígitos para el pipe + caracter.
	//Si alguno no es dígito devolvemos error.
	//Si son dígitos, iremos construyendo el número.
	//Por ejemplo, para M4LDB_MD_LENGTH_CLAUSE = 6 (Son 4 dígitos):
	//Level = 1000*((*ai_Level) - '0');
	//Level += 100*(*(ai_Level+1)-'0');
	//Level += 10*(*(ai_Level+2)-'0');
	//Level += (*(ai_Level+3)-'0');

	//El primer caracter es el más significativo, empiezo por el último,
	//es decir, por las unidades.
	//Si el define es 6, la longitud es 4, por lo tanto vamos del tercer al dígito cero.
	for (m4int32_t iDigit = ((M4LDB_MD_LENGTH_CLAUSE - 2) - 1); iDigit >= 0; iDigit--)
	{
		if (isdigit(*(ai_Level+iDigit)) == 0)
		{
			return -1;
		}
		//El dígito 3 corresponde a unidades
		//El dígito 2 corresponde a decenas...
		//Es decir: Teniendo 3210
		//ai_Level		apunta al 3
		//ai_Level+1	apunta al 2		
		//Para dígito = 3 (unidades 0001)
		//Para dígito = 2 (decenas 0010)
		//El valor de iDigit se decrementa (empieza en unidades,...)
		//El valor de la potencia aumenta.
		
		Level += Pow * (*(ai_Level+iDigit)-'0');
		//Como estamos empezando por el último, le corresponde a la potencia de 10
		//miínima = 1. Es decir irá tomando: 1, 10, 100...
		Pow = 10 * Pow;
	}

	return Level;
	
}

	
m4return_t ClMetaSentence::_AddInfoFilter( PVcClMtParam ai_vParamWhr,PVcClMtField ai_vFieldWhr,string &ai_stFilter,eClauseType_t ai_eClause,m4int32_t ai_Level,m4int32_t &ao_lPos, m4bool_t ai_bProcessOrs)
{
	// Tenemos que tener el filtro incluido ya.
	M4_ASSERT( ai_eClause == M4LDB_CLAUSE_FROM );
	m4uint32_t IniPos=0;
	if (_FindClause(*m_pstStatemWhr,ai_stFilter.c_str()+ao_lPos,IniPos)!=M4_TRUE)
	{
		return M4_ERROR;
	}
	// Hay que copiar lo que venga despues del From. En caso de que sea real, vienen las tablas reales,
	// en caso de que no sea real no viene nada.
	// Miramos a ver si viene algo.
	
	if (ai_stFilter.at(ao_lPos+M4LDB_MD_LENGTH_CLAUSE)!='|')
	{
		m4int32_t lNextClausePos = ao_lPos+M4LDB_MD_LENGTH_CLAUSE;
		eClauseType_t eClause_kk;
		m4int32_t iLevel_kk;
		lNextClausePos = _GetNextClause(ai_stFilter,lNextClausePos,eClause_kk,iLevel_kk);
		if (lNextClausePos ==M4_STR_NPOS)
		{
			// Por lo menos tiene que venir el END
			return M4_ERROR;
		}

		m_pstStatemWhr->insert(IniPos+M4LDB_MD_LENGTH_CLAUSE,ai_stFilter,ao_lPos+M4LDB_MD_LENGTH_CLAUSE,lNextClausePos-ao_lPos-M4LDB_MD_LENGTH_CLAUSE);
	}

	// Borramos el From.
	_DeleteFirstElement(ai_vParamWhr,ai_vFieldWhr);

	eClauseType_t eClause;
	m4int32_t iLevel;
	m4int32_t ClausePos = ao_lPos+M4LDB_MD_LENGTH_CLAUSE;
	while (1)
	{
		// Sacamos la sigueinte clasula.
			ClausePos = _GetNextClause(ai_stFilter,ClausePos,eClause,iLevel);
			if (ClausePos!=M4_STR_NPOS)
			{
				// Hemos encontrado una clausula ( debe ser W R O E )
				if (eClause == M4LDB_CLAUSE_END_STATEM)
				{
					// Hemos acabado con este From.
					// Hay que incrementar los iteradores.

#ifdef _DEBUG
					// Los iteradores tienen que estar en END
					// del nivel en el que estemos.
					if (ai_vParamWhr)
					{
						itVcClMtParam	itPar = ai_vParamWhr->begin();
						if ((*itPar)->m_eClause != M4LDB_CLAUSE_END_STATEM || (*itPar)->m_iLevel!= ai_Level )
						{
							return M4_ERROR;
						}
					}

					if (ai_vFieldWhr)
					{
						itVcClMtField	itFld = ai_vFieldWhr->begin();
						if ((*itFld)->m_eClause != M4LDB_CLAUSE_END_STATEM || (*itFld)->m_iLevel!= ai_Level )
						{
							return M4_ERROR;
						}
					}
#endif
					_DeleteFirstElement(ai_vParamWhr,ai_vFieldWhr);
					ao_lPos = ClausePos+4;
					return M4_SUCCESS;
				}
				if (_AddInfoPartFilter(ai_vParamWhr, ai_vFieldWhr, ai_stFilter, eClause, iLevel, ClausePos, ai_bProcessOrs)!=M4_SUCCESS)
				{
					return M4_ERROR;
				}
			}
			else
			{
				return M4_ERROR;
			}
	}

	M4_ASSERT (0);
	return M4_ERROR;
}


// Añade la parte correspondiente al W R O
m4return_t ClMetaSentence::_AddInfoPartFilter( PVcClMtParam ai_vParamWhr,PVcClMtField ai_vFieldWhr,string &ai_stFilter,eClauseType_t ai_eClause,m4int32_t ai_Level,m4int32_t &ao_lPos, m4bool_t ai_bProcessOrs)
{
	// Hay que actualizar :
	// m_stStatemWhr
	// m_pvcRel
	// m_pvcFld
	// m_pvcParamWhr.

	// Buscamos la clausula en m_stStatemWhr

	// Busco la siguiente clausula en ai_stFilter.
	m4bool_t bExit = M4_FALSE;
	m4int32_t NextClause =0;
	eClauseType_t eClause;
	m4uint32_t IniPos = 0;

	if (_FindClause(*m_pstStatemWhr,ai_stFilter.c_str()+ao_lPos,IniPos)!=M4_TRUE)
	{
		return M4_ERROR;
	}

	// Busco hasta que encuentro la siguiente clausula del mismo nivel.
	m4int32_t iNewLevel = ai_Level;
	NextClause = ao_lPos; 
	do
	{
		NextClause+=M4LDB_MD_LENGTH_CLAUSE;
		NextClause = _GetNextClause(ai_stFilter,NextClause,eClause,iNewLevel);
		if (NextClause == M4_STR_NPOS)
		{
			// Tiene que haber siguiente clausula.
			return M4_ERROR;
		}
	}while (ai_Level != iNewLevel);
	// Tenemos en eClause la siguiente clausula.

	// Insetamos el nuevo filtro, con lo que venga ( subselect .... )
	// Me salto las clausulas actuales.
	IniPos+=M4LDB_MD_LENGTH_CLAUSE;
	ao_lPos+=M4LDB_MD_LENGTH_CLAUSE;

	m4bool_t bOut = M4_TRUE;

	if (_AddInfoPartFilterAdd(ai_vParamWhr, ai_vFieldWhr, ai_stFilter, ai_eClause, ai_Level, ao_lPos, NextClause, ai_bProcessOrs, bOut)!=M4_SUCCESS)
	{
		return M4_ERROR;
	}

	if (bOut)
	{
		/* Bug 0133593
		Si hay ORs hay que ponerlos entre paréntesis si no se pide procesar los OR
		*/
		if( m_bHasOr == M4_TRUE && ai_bProcessOrs == M4_FALSE )
		{
			m4uint32_t		iMainIniPos = 0 ;
			m4int32_t		iMainNextClause = 0 ;
			m4int32_t		iMainNewLevel = 0 ;
			eClauseType_t	eMainClause ;

			// Nos ponemos al final de la inserción
			iMainIniPos = IniPos ;

			// Busco hasta que encuentro la siguiente clausula del mismo nivel.
			iMainNewLevel = ai_Level ;
			iMainNextClause = iMainIniPos - M4LDB_MD_LENGTH_CLAUSE ;
			do
			{
				iMainNextClause += M4LDB_MD_LENGTH_CLAUSE ;
				iMainNextClause = _GetNextClause( *m_pstStatemWhr, iMainNextClause, eMainClause, iMainNewLevel ) ;

				if( iMainNextClause == M4_STR_NPOS )
				{
					// Tiene que haber siguiente clausula.
					return M4_ERROR;
				}
			}
			while( ai_Level != iMainNewLevel ) ;

			if( iMainNextClause != iMainIniPos )
			{
				// Si hay algo detrás puede ser un filtro
				if( memcmp( m_pstStatemWhr->c_str() + iMainIniPos, " AND", 4 ) == 0 )
				{
					// Si empieza por AND esque ya hay un filtro y hay que saltarlo
					m_pstStatemWhr->insert( iMainNextClause, ") ", 0, 2 ) ;
					m_pstStatemWhr->insert( iMainIniPos + 4, "( ", 0, 2 ) ;
				}
			}
		}

		m_pstStatemWhr->insert(IniPos,ai_stFilter,ao_lPos,NextClause-ao_lPos);

		// Tenemos que insertar Fld y Param.
		_VectorInserter<ClMetaField> oInsertFld;
		if (oInsertFld(ai_vFieldWhr,m_pvcFld,ai_eClause,ai_Level)!=M4_SUCCESS)
		{
			return M4_ERROR;
		}
		_VectorInserter<ClMetaParam> oInsertPar;
		if (oInsertPar(ai_vParamWhr,m_pvcParamWhr,ai_eClause,ai_Level)!=M4_SUCCESS)
		{
			return M4_ERROR;
		}
	}

	// Hemos terminado.
	ao_lPos = NextClause;
	return M4_SUCCESS;
}


m4return_t ClMetaSentence::_GetClauseAndLevel (const m4char_t * ai_pClause, eClauseType_t &ao_eClause, m4int32_t &ao_Level)
{
	ao_eClause = _GetClauseType(*(ai_pClause));
	ao_Level = _GetLevel(ai_pClause+1);
	if (ao_Level<0){
		return M4_ERROR;
	}
	return M4_SUCCESS;
}


m4bool_t ClMetaSentence::_FindClause(string &ai_stBuf,const m4char_t * ai_pToFind,m4uint32_t &ao_IniPos)
{

	m4pcchar_t pTemp = ai_stBuf.c_str();
	m4pcchar_t pIni = pTemp;

	m4char_t ccc = *pTemp;
	m4int32_t nCom=0;

	while (ccc)
	{
		switch (ccc)
		{
			case '\'':
				++nCom;
				break;
			case '|':
				// Lo unico que controlo es que el numero de comillas sea par.
				if (nCom%2==0)
				{
					if (strncmp(pTemp,ai_pToFind,M4LDB_MD_LENGTH_CLAUSE)==0)
					{
						ao_IniPos = pTemp-pIni;
						return M4_TRUE;
					}
				}
				break;
//			default:
//				break;
		}
		ccc = *(++pTemp);
	}


	return M4_FALSE;


}


m4bool_t ClMetaSentence::_FindClause(string &ai_stBuf,eClauseType_t ai_eClause,m4int32_t ai_Level,m4uint32_t &ao_IniPos)
{
	eClauseType_t eClause;
	m4int32_t Level;
	m4pcchar_t pTemp = ai_stBuf.c_str();
	m4pcchar_t pIni = pTemp;

	m4char_t ccc = *pTemp;
	m4int32_t nCom=0;

	while (ccc)
	{
		switch (ccc)
		{
			case '\'':
				++nCom;
				break;
			case '|':
				// Lo unico que controlo es que el numero de comillas sea par.
				if (nCom%2==0)
				{
					m4char_t clause =*(pTemp+1);
					if (strchr(M4LDB_POSIBLE_CLAUSES,clause)!=0)
					{
						if (_GetClauseAndLevel(pTemp+1,eClause,Level)==M4_SUCCESS)
						{
							if (ai_eClause == eClause && ai_Level == Level )
							{
								ao_IniPos = pTemp - pIni;
								return M4_TRUE;
							}
						}
					}
				}
				break;
//			default:
//				break;
		}
		ccc = *(++pTemp);
	}


	return M4_FALSE;


}


void ClMetaSentence::_DeleteFirstElement(PVcClMtParam ai_vParamWhr,PVcClMtField ai_vFieldWhr)
{
	if (ai_vParamWhr)
	{
		itVcClMtParam	itPar = ai_vParamWhr->begin();
		if (itPar !=ai_vParamWhr->end())
		{
			M4_ASSERT((*itPar)->IsSeparator()==M4_TRUE);
			delete (*itPar);
			ai_vParamWhr->erase(itPar);
		}
	}
	if (ai_vFieldWhr)
	{
		itVcClMtField	itFld = ai_vFieldWhr->begin();
		if (itFld !=ai_vFieldWhr->end())
		{
			M4_ASSERT((*itFld)->IsSeparator()==M4_TRUE);
			delete (*itFld);
			ai_vFieldWhr->erase(itFld);
		}
	}

}


m4return_t ClMetaSentence::AddInfoObjects		( PVcClMtObject ai_VcClMtObj,PVcClMtRelat ai_vRel, m4bool_t ai_bSec)
{
	_VectorClearer<ClMetaObject,ClMetaRelat,ClMetaRelat> oClear(ai_VcClMtObj,ai_vRel,0);

	// Si no hay objetos  no puede haber relaciones.
	if (ai_VcClMtObj==0)
	{
		return M4_SUCCESS;
	}
	if (ai_VcClMtObj->empty())
	{
		return M4_SUCCESS;
	}

	itVcClMtObject itFrmS = ai_VcClMtObj->begin();
	itVcClMtObject endS = ai_VcClMtObj->end();

	M4_ASSERT(m_pvcFrmObj->empty());
	itFrmS+=m_iLastObjectSize; //Me coloco  al principio de los nuevos.
	m_iLastObjectSize=0;
	m_pvcFrmObj->insert(m_pvcFrmObj->end(),ai_VcClMtObj->begin(),itFrmS);
	ai_VcClMtObj->erase(ai_VcClMtObj->begin(),itFrmS);
	itVcClMtObject itFrm = m_pvcFrmObj->begin();
	itVcClMtObject end = m_pvcFrmObj->end();
	// Hempos podido cambiarlos
	itFrmS = ai_VcClMtObj->begin();
	endS = ai_VcClMtObj->end();
	m4int32_t Level;


	
	// Objetos logicos.
	while (itFrmS!=endS)
	{
		Level= (*itFrmS)->m_iLevel;
		// Nos colocamos en el nivel adecuado.
		for (;itFrm!=end;++itFrm)
		{
			if ((*itFrm)->m_iLevel>Level)
			{
				break;
			}
		}
		// buscamos el numero de ellos que hay que insertar.
		for (;itFrmS!=endS;++itFrmS)
		{
			if ((*itFrmS)->m_iLevel!=Level)
			{
				break;
			}
		}

		// Insertamos justo antes.( Estemos en END o no )
		m_pvcFrmObj->insert(itFrm,ai_VcClMtObj->begin(),itFrmS);
		ai_VcClMtObj->erase(ai_VcClMtObj->begin(),itFrmS);
		itFrmS=ai_VcClMtObj->begin();
		end = m_pvcFrmObj->end();
		itFrm = m_pvcFrmObj->begin();
		endS = ai_VcClMtObj->end();
	}

	//<<< Pedro. Bug 0065851.
	//Los cambios que se han producido en el vector m_pvcFrmObj
	//pueden hacer que la información del vector de alias m_pvcFrmAls se haya quedado
	//incoherente.
	//Es decir, el orden en el vector m_pvcFrmObj ha podido variar porque queda ordenado
	//por nivel de menor a mayor.
	//El vector de alias m_pvcFrmAls tiene el índice del vector de objetos. De esta forma,
	//dado el índice conozco qué objeto es el asociado.
	//De la misma manera, dado un objeto, sé qué indice dentro del vector de alias
	//al que está asociado el objeto (pero este último valor no lo hemos cambiado).
	//Recorreremos el vector de alias para reorganizar índices.
	itVcClMtObject itFrmCurrent = m_pvcFrmObj->begin();
	itVcClMtObject itFrmEnd = m_pvcFrmObj->end();
	m4int32_t indObj = 0;
	while (itFrmCurrent!=itFrmEnd)
	{
		//Tomamos el alias que me indica el objeto.
		ClMetaAlias *pAlias = (*m_pvcFrmAls)[((*itFrmCurrent)->m_indAlias)];
		//Establecemos el nuevo indice. Le indicamos al alias con qué índice, del
		//vector de objetos, está asociado. Es decir, dado el alias ahora conocemos
		//correctamente a qué objeto está asociado.
		pAlias->m_indObj = indObj;
		++indObj;
		itFrmCurrent ++;
	}
	//Pedro >>>

	ai_VcClMtObj->clear ();

	// Comprobamos los joins entre distintos tipos de tablas en cuanto al tipo de sociedad.
	if (_CheckOrgRelations(ai_vRel)!=M4_SUCCESS)
	{
		return M4_ERROR;
	}
	// Relaciones. De momento las copiamos a capon (sólo si no son de seguridad).
	if (ai_vRel)
	{
//		m4bool_t bFirst = M4_TRUE;
		for (itVcClMtRelat it = ai_vRel->begin (); it!=ai_vRel->end (); it++)
		{
			m4bool_t bOut = M4_TRUE;
			if (ai_bSec && _ReplaceRelationAdd(*it, bOut)!=M4_SUCCESS)
			{
				return M4_ERROR;
			}
			if (bOut)
			{
				m_pvcRel->push_back (*it);
			}
/*			else
			{
				bFirst = M4_FALSE;
			}
		}

		if (!bFirst)
		{
			DUMP_LDBINSP_DEBUG(m_pGlobVars->GetpLConn(),M4_CH_LDB_OUTER_JOIN_TO_INNER_JOIN_IV);
*/
		}

		ai_vRel->erase (ai_vRel->begin(),ai_vRel->end());
	}

	return M4_SUCCESS;
}


void ClMetaSentence::Dump(ofstream &ofs)
{
	if (m_pstStatemCol)
	{
		ofs << "Sentencia:(Columnas) "<<*m_pstStatemCol<<endl;
	}
	if (m_pstStatemWhr)
	{
		ofs << "Sentencia:(Filtro) "<<*m_pstStatemWhr<<endl;
	}

	if (m_pvcColumns)
	{
		ofs << "COLUMNAS:"<<endl<<endl;
		_VectorDumper<ClMetaItemCol>	oVDCol;
		oVDCol(ofs,*m_pvcColumns);
	}

	if (m_pvcParamCol)
	{
		ofs << "PARAMETROS COLUMNA:"<<endl<<endl;
		_VectorDumper<ClMetaParam>	oVDPar;
		oVDPar(ofs,*m_pvcParamCol);
	}

	if (m_pvcParamWhr)
	{
		ofs << "PARAMETROS FILTRO:"<<endl<<endl;
		_VectorDumper<ClMetaParam>	oVDPar;
		oVDPar(ofs,*m_pvcParamWhr);
	}

	if (m_pvcFld)
	{
		ofs << "FIELDS:"<<endl<<endl;
		_VectorDumper<ClMetaField>	oVDFld;
		oVDFld(ofs,*m_pvcFld);
	}

	if (m_pvcRel)
	{
		ofs << "RELACIONES:"<<endl<<endl;
		_VectorDumper<ClMetaRelat>	oVDRel;
		oVDRel(ofs,*m_pvcRel);
	}

	if (m_pvcFrmObj)
	{
		ofs << "OBJETOS:"<<endl<<endl;
		_VectorDumper<ClMetaObject>	oVDObj;
		oVDObj(ofs,*m_pvcFrmObj);
	}
}


m4return_t ClMetaSentence::AdjustLogObj( void )
{
	// Hay que ajustar los objetos logicos para que cada uno apunte a su alias y viceversa.
	// Tenemos el mismo numero de objetos logicos que de Alias. pero pueden estar en distinto orden
	// Los objetos logicos tienen el indice correcto del Alias al que pertenecen.
	itVcClMtObject itObj= m_pvcFrmObj->begin();
	itVcClMtObject itObjend = m_pvcFrmObj->end();

	m4int32_t indObj = 0;
	for (;itObj!=itObjend;++itObj)
	{
		ClMetaAlias *pAlias = (*m_pvcFrmAls)[(*itObj)->m_indAlias];
		pAlias->m_indObj=indObj;
		++indObj;
	}

	if (m_pvcRel)
	{
		// Hay que actualizar los tipos de OJ de los objetos segun los que tengn las relaciones.
		itVcClMtRelat itRel= m_pvcRel->begin();
		itVcClMtRelat itRelend = m_pvcRel->end();

		for (;itRel!=itRelend;++itRel)
		{
			if ((*itRel)->m_eJoinType==M4LDB_OUTER_JOIN)
			{
				// Actualizamos el objeto del Field2.
				indObj = GetObjectPosByAliasPos((*itRel)->m_pMtField2->m_indAlias);
				if (indObj!=M4_STR_NPOS)
				{
					(*m_pvcFrmObj)[indObj]->m_eJoinType=M4LDB_OUTER_JOIN;
				}
				else
				{
					// ¿Un campo sin objeto logico?
					return M4_ERROR;
				}
			}
		}
	}

	return M4_SUCCESS;
}


m4id_t ClMetaSentence::GetIdObjectByPos (m4int32_t ai_indObj)
{
	m4id_t idObj = M4LDB_EMPTY;

	if (m_eStmType != M4LDB_SELECT && !m_bAPISQL)
	{
		return (m_pGlobVars->GetWriteIdObject ());
	}

	if (ai_indObj > M4LDB_NULL_INDEX)
	{
		idObj = (*m_pvcFrmObj)[ai_indObj]->m_stObject.c_str();
	}

	return (idObj);
}


m4int32_t ClMetaSentence::GetPosBasis (void)
{
	m4int32_t numStmObj = m_pvcFrmObj->size ();

	for (m4int32_t indStmObj=0; indStmObj<numStmObj; indStmObj++)
	{
		if ((*m_pvcFrmObj)[indStmObj]->m_bIsBasis)
		{
			return (indStmObj);
		}
	}
	
	return (M4LDB_NULL_INDEX);
}


m4return_t ClMetaSentence::CreateParams(ClDinArr *ai_pParam, m4bool_t ai_bUseOldValue)
{
	// Recorro todos los parametros, creando su StMetaData.

	// Calculo el numero de parametros.
	m4return_t result = M4_ERROR;
	if (ai_pParam==0)
	{
		return M4_ERROR;
	}

	itVcClMtParam itPar;
	itVcClMtParam itParend;
	m4int32_t nPar=0;

	if (m_pvcParamWhr)
	{
		itPar = m_pvcParamWhr->begin();
		itParend = m_pvcParamWhr->end();

		for(;itPar!=itParend;++itPar)
		{
			if ((*itPar)->IsMetaData()==M4_TRUE)
			{
				++nPar;
			}
		}
	}
	// Hay que tener en cuenta que en el caso de las columnas Select, puede que no haya que incluir todos, solo
	// los que hagan referencia a las columnas que correspondan.
	if (m_pvcParamCol)
	{
		itPar = m_pvcParamCol->begin();
		itParend = m_pvcParamCol->end();
		for(;itPar!=itParend;++itPar)
		{
			if ((*itPar)->IsMetaData()==M4_TRUE)
			{
				++nPar;
			}
		}
	}

	ai_pParam->Init(nPar, M4_TRUE);

	// Recorremos los paramteros de las columnas y los de los filtros.
	nPar = 0;
	ClMetaParam *pParam =0;

	if (m_pvcParamCol)
	{
		itPar = m_pvcParamCol->begin();
		itParend = m_pvcParamCol->end();
		for(;itPar!=itParend;++itPar)
		{
			if ((*itPar)->IsMetaData()==M4_TRUE)
			{
				pParam = (*itPar);
				result = _CreateOneParam(ai_pParam,pParam,nPar);
				if (result!=M4_SUCCESS)
				{
					return result;
				}
				++nPar;
			}
		}
	}

	
	if (m_pvcParamWhr)
	{
		itPar = m_pvcParamWhr->begin();
		itParend = m_pvcParamWhr->end();
		
		for(;itPar!=itParend;++itPar)
		{
			if ((*itPar)->IsMetaData()==M4_TRUE)
			{
				pParam = (*itPar);
				result = _CreateOneParam(ai_pParam,pParam,nPar, ai_bUseOldValue);
				if (result!=M4_SUCCESS)
				{
					return result;
				}
				++nPar;
			}
		}
	}

	return M4_SUCCESS;
}


m4return_t ClMetaSentence::CreateColumns(ClDinArr *ai_pColumns)
{
	//Bug 0076002.
	//Se comprueba también si hay columnas.
	if (!m_pvcColumns)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_NO_REGISTER_ITEM, "");
		return M4_ERROR;
	}

	if (ai_pColumns==0)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClMetaSentence::CreateColumns" << "Null ClDinArr"  );
		return M4_ERROR;
	}
	m4bool_t bIsEncrypted=M4_FALSE;
	m4bool_t bEmulateNVL = M4_FALSE;
	m4int32_t hItem=0;
	m4int8_t m4Type = 0;
	m4int32_t lPrec = 0;
	m4int32_t lScale = 0;
	m4return_t result;
	
	m4int32_t nCol=GetNumItemCol();
	
	ai_pColumns->Init(nCol,M4_TRUE);

	itVcClMtColumn	itCol = m_pvcColumns->begin();
	itVcClMtColumn	itColend = m_pvcColumns->end();
	ClMetaItemCol *pCol= 0;
	
	nCol=0;
	for (;itCol!=itColend;++itCol)
	{
		pCol = (*itCol);
		if (pCol->IsSeparator()==M4_TRUE)
		{
			continue;
		}
		hItem = pCol->m_hItem;
		if (hItem==0)
		{
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClDinArr::CreateColumn" << "Null item handle in column = " << LogCat << nCol);
			return (M4_ERROR);
		}
		if (hItem!=-1)
		{	// DT_LAST_UPDATE
			if (m_pGlobVars->GetItemType (hItem)== M4CL_ITEM_TYPE_METHOD)
			{
				m4id_t idField = m_pGlobVars->GetItemReadIdField (hItem);
				DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_ITEM_IS_METHOD, hItem << idField);
				return (M4_ERROR);
			}
			m4Type = m_pGlobVars->GetItemM4Type (hItem);
			lPrec = m_pGlobVars->GetItemDBPrecision (hItem);
			lScale = m_pGlobVars->GetItemScale (hItem);
		}
		else
		{
			m4Type = M4CL_M4_TYPE_TIMESTAMP;;
			lPrec = M4LDB_SIZE_TIMESTAMP;
			lScale = 0;
		}


		// Si es real, terminamos ya.
		if (IsReal() || !pCol->m_bValidateType)
		{
			result = ai_pColumns->CreateData (nCol, NULL, hItem, m4Type, lPrec, lScale, NULL, NULL, M4LDB_PAR_ITEM, M4_TRUE);
		}
		else
		{
			ClRegister *pReg = _MoveToField (pCol);
			result = ai_pColumns->CreateData (nCol, pReg, hItem, m4Type, lPrec, lScale, NULL, NULL, M4LDB_PAR_ITEM, M4_TRUE);
			if (result !=M4_SUCCESS)
			{
				//!!!!!!!!!!!
				return M4_ERROR;
			}
			// Miramos a ver si hay que emular el NVL.
			if (_GetColumnEmulateNVL(pCol)==M4_TRUE)
			{
				ai_pColumns->SetEmulateNVL(nCol,M4LDB_NVL_FIRST);
				++nCol;
				result = ai_pColumns->CreateData (nCol, pReg, hItem, m4Type, lPrec, lScale, NULL, NULL, M4LDB_PAR_ITEM, M4_TRUE);
			}
		}
		
		if (result!=M4_SUCCESS)
		{
				//!!!!!!!!!!!
			return M4_ERROR;
		}
		++nCol;
	}
	return M4_SUCCESS;
}


ClRegister * ClMetaSentence::_MoveToField	(ClMetaItemCol *ai_pCol)
{
	m4int32_t nCol = ai_pCol->m_iColumn;
	itVcClMtField	itFld = m_pvcFld->begin();
	itVcClMtField	itFldend = m_pvcFld->end();
	for (;itFld!=itFldend;++itFld)
	{
		if ((*itFld)->m_iColumn == nCol)
		{
			if ((*itFld)->IsMetaData()== M4_TRUE)
			{
				// Encontrado.
				break;
			}
		}
	}
	
	if (itFld == itFldend || 
		(*itFld)->m_eClause != ai_pCol->m_eClause || 
		(*itFld)->m_iColumn != ai_pCol->m_iColumn)
	{
		// La columna no tiene field asocido.
		return NULL;
	}

	ClMetaField *pField = (*itFld);
	
	// Posicionamos el diccionario apuntando a este Field.
	m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.MoveTo(pField->m_indLogicTable);
	m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.MoveTo(pField->m_indRealTable);
	m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register.MoveTo(pField->m_indRealField);

	return (&m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Register);
}


m4bool_t ClMetaSentence::_GetColumnEmulateNVL(ClMetaItemCol *ai_pCol)
{
	
	// Hallamos el Field asociado a la columna. Empezamos por atras ( solo puede ser long ).
	if (ai_pCol->m_eClause != M4LDB_CLAUSE_LONG_COLUMN)
	{
		return M4_FALSE;
	}

	m4int32_t nCol = ai_pCol->m_iColumn;
	itVcClMtField	itFldbegin = m_pvcFld->begin();
	itVcClMtField	itFld = m_pvcFld->end();
	ClMetaField *pFld = 0;
	while (itFldbegin!=itFld)
	{
		--itFld;
		pFld = (*itFld);
		if (pFld ->m_iColumn == nCol)
		{
			if (pFld ->IsMetaData() == M4_TRUE)
			{
				if (pFld ->m_eClause  == M4LDB_CLAUSE_LONG_COLUMN)
				// Encontrado.
				break;
			}
		}
	}
	// Si tiene segunda tabla real, hay que emular NVL.
	return ( (*itFld)->m_indRealTable2!=M4LDB_NULL_INDEX ? M4_TRUE : M4_FALSE );

}


void ClMetaSentence::GetObjectCopy(VcClMtObject &ai_vcObj)
{
	ai_vcObj.clear();
	m_iLastObjectSize = m_pvcFrmObj->size();
	if (m_pvcFrmObj)
	{
		ai_vcObj.assign(m_pvcFrmObj->begin(),m_pvcFrmObj->end());
	}
	m_pvcFrmObj->clear();
}


m4int32_t ClMetaSentence::GetNumItemCol(void)
{
	if (!m_pvcColumns)
	{
		return (0);
	}
	// Hallamos el numero de columnas (incluyendo las añadidas para EmulateNVL).
	if (m_numCol==-1)
	{
		m4int32_t nCol=0;
		itVcClMtColumn	itCol = m_pvcColumns->begin();
		itVcClMtColumn	itColend = m_pvcColumns->end();
		for (;itCol!=itColend;++itCol)
		{
			if ((*itCol)->IsMetaData()==M4_TRUE)
			{
				++nCol;
			}
		}
		//Puedo tener columnas long que tengan que emualr el NVL. Esto añadeuna columna por cada una.
		itVcClMtField	itFld = m_pvcFld->begin();
		itVcClMtField	itFldend = m_pvcFld->end();
		ClMetaField * pField = 0;
		for (;itFld!=itFldend;++itFld)
		{
			pField = (*itFld);
			// Recorremos los Fld hasta llegar al From.
			if (pField->m_eClause==M4LDB_CLAUSE_FROM)
			{
				// Ya nos hemos pasado.
				break;
			}
			else
			{
				if (pField->m_eClause==M4LDB_CLAUSE_LONG_COLUMN)
				{
					if (pField->m_indRealTable2!=M4LDB_NULL_INDEX)
					{
						if (pField->IsMetaData()==M4_TRUE)
						{
							++nCol;
						}
					}
				}
			}
		}
		return nCol;
	}
	else
	{
		return m_numCol+m_numColNVL;
	}
}


m4return_t ClMetaSentence::CreateVirtualColumns( VcVirtualParam &ai_Columns,m4int32_t ai_indColIni,m4int32_t ai_indColEnd )
{

	m4int32_t LowerLimit;
	m4int32_t UpperLimit;

	if (ai_indColIni==-1)
	{
		LowerLimit = 0;
	}
	else
	{
		LowerLimit = ai_indColIni;
	}
	if (ai_indColEnd==-1)
	{
		UpperLimit = GetNumItemCol();
	}
	else
	{
		UpperLimit = ai_indColEnd;
	}
	//Insertamos los indices de las columnas.
	itVcClMtColumn itCol = m_pvcColumns->begin();
	itVcClMtColumn itColend = m_pvcColumns->end();
	m4int32_t nCol = 0;
	ClMetaItemCol *pCol =0;
	for (;itCol!=itColend;++itCol)
	{
		pCol = (*itCol);
		if (pCol->IsMetaData() == M4_TRUE)
		{
			if (pCol->m_eClause!=M4LDB_CLAUSE_LONG_COLUMN)
			{
				// Si esta dentro de los limites la metemos.
				if (pCol->m_iColumn>=LowerLimit)
				{
					if (pCol->m_iColumn<UpperLimit)
					{
						ai_Columns.push_back(nCol);
					}
					else
					{
						break;
					}
				}
			}
			else
			{
				// Si esta dentro de los limites la metemos.
				if (pCol->m_iColumn>=LowerLimit && pCol->m_iColumn<UpperLimit)
				{
					if (pCol->m_iColumn<UpperLimit)
					{
						ai_Columns.push_back(nCol);
						// Hay que comprobar que el campo long no tiene EmulateNVL.
						if (_GetColumnEmulateNVL(pCol))
						{
							++nCol;
							ai_Columns.push_back(nCol);
						}
					}
					else
					{
						break;
					}
				}
			}
			++nCol;
		}
	}

	return M4_SUCCESS;
}


m4return_t ClMetaSentence::CreateVirtualParams	( VcVirtualParam &ai_Params,m4int32_t ai_indColIni,m4int32_t ai_indColEnd)
{
	m4int32_t LowerLimit;
	m4int32_t UpperLimit;

	if (ai_indColIni==-1)
	{
		LowerLimit = 0;
	}
	else
	{
		LowerLimit = ai_indColIni;
	}
	if (ai_indColEnd==-1)
	{
		UpperLimit = GetNumItemCol();
	}
	else
	{
		UpperLimit = ai_indColEnd;
	}
	
	itVcClMtParam itPar;
	itVcClMtParam itParend;
	m4int32_t nPar = 0;
	ClMetaParam *pPar =0;

	if (m_pvcParamCol)
	{
		//Insertamos los indices de los parametros de las columnas.
		itPar = m_pvcParamCol->begin();
		itParend = m_pvcParamCol->end();
		
		// Tengo que llegar hasta el final para poder contarlos.
		for (;itPar!=itParend;++itPar)
		{
			pPar = (*itPar);
			if (pPar->IsMetaData() == M4_TRUE)
			{
				if ((pPar->m_iColumn>=LowerLimit) && (pPar->m_iColumn<UpperLimit))
				{
					ai_Params.push_back(nPar);
				}
				++nPar;
			}
		}
	}

	if (m_pvcParamWhr)
	{
		itPar = m_pvcParamWhr->begin();
		itParend = m_pvcParamWhr->end();

		// Metemos todos los del filtro.
		for (;itPar!=itParend;++itPar)
		{
			pPar = (*itPar);
			if (pPar->IsMetaData() == M4_TRUE)
			{
				ai_Params.push_back(nPar);
				++nPar;
			}
		}
	}

	return M4_SUCCESS;
}


m4return_t ClMetaSentence::AnalizeRelations (void)
{
	if (m_pGlobVars->GetpLConn ()->m_pLdb->HasSecurityOptimization ())
	{
		return (_X_AnalizeRelations ());
	}

	if (!m_pvcRel || m_pvcRel->empty())
	{
		// Nada que analizar
		return (M4_SUCCESS);
	}
	

	m4bool_t bReduce = M4_FALSE;

	for (itVcClMtRelat itr1 = m_pvcRel->begin (); itr1!=m_pvcRel->end (); itr1++)
	{
		m4int32_t indAliast1r1 = (*itr1)->m_pMtField1->m_indAlias;
		
		m4int32_t indAliast2r1 = (*itr1)->m_pMtField2->m_indAlias;

		if ((( (*m_pvcFrmAls)[indAliast1r1]->m_bReduce) && 
			((*m_pvcFrmAls)[indAliast2r1]->m_bReduce)) || 
			((*m_pvcFrmAls)[indAliast1r1]->m_iLevel != (*m_pvcFrmAls)[indAliast2r1]->m_iLevel))
		{
			continue;
		}

		//Bug 0122107 
		//Comprobamos sociedad del canal y de las tablas.
		ClChannel * pChannel = m_pGlobVars->GetpAccessRecordSet()->GetpAccess()->GetpChannel();
		m4double_t dOrganizationType;
		pChannel->OrganizationType.Get(dOrganizationType);
		m4int32_t iOrgType = (m4int32_t)dOrganizationType;

		if ( (*itr1)->m_bReduce == FALSE && iOrgType == M4CL_ORGANIZATION_TYPE_MONO)
		{				
			ClMetaObject * pObj1 = (*m_pvcFrmObj)[GetObjectPosByAliasPos(indAliast1r1)];
			ClMetaObject * pObj2 = (*m_pvcFrmObj)[GetObjectPosByAliasPos(indAliast2r1)];

			//Si tienen el mismo comportamiento y no es dependencia avanzada.
			if (pObj2->m_iOrgType == pObj1->m_iOrgType 
				&& pObj1->m_iOrgType != M4CL_ORGANIZATION_TYPE_NONE)
			{
				//Si es relacion por organizacion la reducimos.
				if (_GetIdInternalField ((*itr1)->m_pMtField1) == M4LDB_ID_ORGANIZATION &&
					_GetIdInternalField ((*itr1)->m_pMtField2) == M4LDB_ID_ORGANIZATION)
				{				
					(*itr1)->m_bReduce = M4_TRUE;
					bReduce = M4_TRUE;
					continue;
				}

			}
		}
		 		
		m4id_t pObj1 = (*m_pvcFrmObj)[ (*m_pvcFrmAls)[indAliast1r1]->m_indObj ]->m_stObject.c_str ();
		
		m4id_t pObj2 = (*m_pvcFrmObj)[ (*m_pvcFrmAls)[indAliast2r1]->m_indObj ]->m_stObject.c_str ();

		
		for (itVcClMtRelat itr2 = itr1+1; itr2!=m_pvcRel->end (); itr2++)
		{
			
			if (((*itr1)->m_pMtField1->m_iLevel != (*itr2)->m_pMtField1->m_iLevel) || 
				((*itr1)->m_pMtField1->m_iFilter == (*itr2)->m_pMtField1->m_iFilter) || 
				(( (*m_pvcFrmAls)[(*itr2)->m_pMtField1->m_indAlias]->m_bReduce) && 
				((*m_pvcFrmAls)[(*itr2)->m_pMtField2->m_indAlias]->m_bReduce)) || 
				( (*m_pvcFrmAls)[(*itr2)->m_pMtField1->m_indAlias]->m_iLevel != (*m_pvcFrmAls)[(*itr2)->m_pMtField2->m_indAlias]->m_iLevel) || 
				(itr1 == itr2))
			{
				continue;
			}

			// Buscamos tablas duplicadas en las relaciones.
			m4int32_t indAliast1r2 = M4LDB_NULL_INDEX;
			
			m4int32_t indAliast2r2 = M4LDB_NULL_INDEX;
			
			m4id_t pObj3 = (*m_pvcFrmObj)[(*m_pvcFrmAls)[(*itr2)->m_pMtField1->m_indAlias]->m_indObj]->m_stObject.c_str ();
			
			m4id_t pObj4 = (*m_pvcFrmObj)[(*m_pvcFrmAls)[(*itr2)->m_pMtField2->m_indAlias]->m_indObj]->m_stObject.c_str ();

			if (!strcmpi (pObj1, pObj3) && !strcmpi (pObj2, pObj4))
			{
				indAliast1r2 = (*itr2)->m_pMtField1->m_indAlias;
				indAliast2r2 = (*itr2)->m_pMtField2->m_indAlias;				
			}
			else
			{
				if (!strcmpi (pObj1, pObj4) && !strcmpi (pObj2, pObj3))
				{
					indAliast2r2 = (*itr2)->m_pMtField1->m_indAlias;
					indAliast1r2 = (*itr2)->m_pMtField2->m_indAlias;
				}
				else
				{
					continue;
				}
			}

			// Hemos encontrado tablas duplicadas.
			// A partir de ahora, toda relación indAliast1r1,indAliast2r1 tiene que tener otra relación indAliast1r2,indAliast2r2.
			m4bool_t bExists = M4_FALSE;
			
			// RRRR
			// El comportamiento si hay conflictos entre Outer Joins y Joins es distintovsegun la base de datos.
			// Me creo un vector donde voy metiendo las relaciones que se vayan reducir.

			VcClMtRelat		vRelations;
			vRelations.reserve(8);

			//Bug 0089387. 09/12/2003
			//Vamos a meter todas las relaciones que tengan los alias en un vector para tratarlos
			bExists = _AnalizeRelationsDuplicated( vRelations, indAliast1r1, indAliast2r1, indAliast1r2, indAliast2r2 );

			if (bExists)
			{
				m4bool_t bRealReduce = M4_FALSE;
				// Solo comprobamos que son de filtros distintos cuando no viene de seguridad.
				if (indAliast1r1!=indAliast1r2 && indAliast2r1!=indAliast2r2)
				{ // No es un alias de seguridad.
					// Si indNewAlias == indAlias entonces tenemos un ciclo.
					if (( (*m_pvcFrmAls)[indAliast1r1]->m_iFilter != (*m_pvcFrmAls)[indAliast1r2]->m_iFilter) && 
					    ( (*m_pvcFrmAls)[indAliast2r1]->m_iFilter != (*m_pvcFrmAls)[indAliast2r2]->m_iFilter) && 
						( (*m_pvcFrmAls)[indAliast1r1]->m_indNewAlias != indAliast1r2) && 
						( (*m_pvcFrmAls)[indAliast2r1]->m_indNewAlias != indAliast2r2))
					{
						bRealReduce = M4_TRUE;
					}
				}
				// Hay un alias de seguridad.
				if (indAliast1r1==indAliast1r2)
				{
					// Comprueboque el otro sea de distinto filtro.
					if ( (*m_pvcFrmAls)[indAliast2r1]->m_iFilter != (*m_pvcFrmAls)[indAliast2r2]->m_iFilter)
					{
						bRealReduce = M4_TRUE;
					}
				}

				if (indAliast2r1==indAliast2r2)
				{
					// Comprueboque el otro sea de distinto filtro.
					if ( (*m_pvcFrmAls)[indAliast1r1]->m_iFilter != (*m_pvcFrmAls)[indAliast1r2]->m_iFilter)
					{
						bRealReduce = M4_TRUE;
					}
				}
				if (bRealReduce)
				{
					(*m_pvcFrmAls)[indAliast1r2]->m_bReduce = M4_TRUE;
					(*m_pvcFrmAls)[indAliast1r2]->m_indNewAlias = indAliast1r1;
					
					(*m_pvcFrmAls)[indAliast2r2]->m_bReduce = M4_TRUE;
					(*m_pvcFrmAls)[indAliast2r2]->m_indNewAlias = indAliast2r1;

					(*itr2)->m_bReduce = M4_TRUE;

					bReduce = M4_TRUE;

					for (itVcClMtRelat itr5 = itr2+1; itr5!=m_pvcRel->end (); itr5++)
					{
						if ((((*itr5)->m_pMtField1->m_indAlias==indAliast1r2) &&
							((*itr5)->m_pMtField2->m_indAlias==indAliast2r2)) ||
							(((*itr5)->m_pMtField1->m_indAlias==indAliast2r2) &&
							((*itr5)->m_pMtField2->m_indAlias==indAliast1r2)))
						{
							(*itr5)->m_bReduce = M4_TRUE;
						}
					}

					ClOuterJoinReductor_Oracle	oOJR;
					m4bool_t bHasChanges=M4_FALSE;
					if (oOJR.ReduceOuterJoins(vRelations,this,bHasChanges)!=M4_SUCCESS)
					{
						// Error. Hay Outer Joins en los dos sentidos,
						m4pcchar_t nlgcTbl1 = m_pGlobVars->GetpLConn ()->GetnLgcTbl( (*itr1)->m_pMtField1->m_indLogicTable);
						m4pcchar_t iLgcTbl1 = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[(*itr1)->m_pMtField1->m_indLogicTable].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());

						m4pcchar_t nlgcTbl2 = m_pGlobVars->GetpLConn ()->GetnLgcTbl( (*itr1)->m_pMtField2->m_indLogicTable);
						m4pcchar_t iLgcTbl2 = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[(*itr1)->m_pMtField2->m_indLogicTable].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());

						DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (),M4_CH_LDB_OUTER_JOIN_CONFLICT,
							LOG_CAT_AND (iLgcTbl1, nlgcTbl1) << 
							LOG_CAT_AND (iLgcTbl2, nlgcTbl2)
							);
						return M4_ERROR;
					}
					if (bHasChanges)
					{
						// Ha habido cambio. Me han podido cambiar el orden de los joins.
						// Actualizo los valores.
						indAliast1r1 = (*itr1)->m_pMtField1->m_indAlias;
						indAliast2r1 = (*itr1)->m_pMtField2->m_indAlias;
					}
				}
			}
			// Si no hay que reducir, borrro el vector auxiliar.
			vRelations.erase(vRelations.begin(),vRelations.end());

		}
	}

	//---------------------------------------------------------
	//Analizamos la posibilidad de eliminar relaciones por xjoin. Indicamos que si se ha reducido xjoin
	if (_AnalizeXJoinRelations () == M4_TRUE)
	{
		bReduce = M4_TRUE;
	}
	//---------------------------------------------------------
	
	//---------------------------------------------------------
	//Analizamos la posibilidad de eliminar relaciones con objetos relacionados por herencia.
	if (_AnalizeRelationsWithInheritObjects() == M4_TRUE)
	{
		bReduce = M4_TRUE;
	}
	//---------------------------------------------------------
	
	//---------------------------------------------------------
	//Volcamos como han quedado las relaciones.
	DumpInfoRelations();
	//---------------------------------------------------------

	if (bReduce)
	{
		// Esto tiene que estar antes que la reducción de objetos porque puede ser que se invalide la reducción.
		for (m4int32_t indRel = m_pvcRel->size ()-1; indRel>=0; indRel--)
		{
			itVcClMtRelat itRel = m_pvcRel->begin () + indRel;

			if ((*itRel)->m_bReduce)
			{
				delete (*itRel);
				m_pvcRel->erase (itRel);
			}
			else
			{
				while ( (*m_pvcFrmAls)[(*itRel)->m_pMtField1->m_indAlias]->m_bReduce)
				{
					if ((*itRel)->m_pMtField1->m_indAlias != (*m_pvcFrmAls)[(*itRel)->m_pMtField1->m_indAlias]->m_indNewAlias)
					{
						(*itRel)->m_pMtField1->m_indAlias = (*m_pvcFrmAls)[(*itRel)->m_pMtField1->m_indAlias]->m_indNewAlias;
					}
					else
					{
						// Cuando el alias es el mismo es porque es un alias proveniente de seguridad.
						break;
					}
				}
				while ( (*m_pvcFrmAls)[(*itRel)->m_pMtField2->m_indAlias]->m_bReduce)
				{
					if ((*itRel)->m_pMtField2->m_indAlias != (*m_pvcFrmAls)[(*itRel)->m_pMtField2->m_indAlias]->m_indNewAlias)
					{
						(*itRel)->m_pMtField2->m_indAlias = (*m_pvcFrmAls)[(*itRel)->m_pMtField2->m_indAlias]->m_indNewAlias;
					}
					else
					{
						// Cuando el alias es el mismo es porque es un alias proveniente de seguridad.
						break;
					}
				}
			}
		}

		//-----------------------
		//Refactorizamos, implementado el miembro _AnalizeObjectsToReduceAndArrangeFields ()
		if (_AnalizeObjectsToReduceAndArrangeFields () != M4_SUCCESS)
		{
			return M4_ERROR;
		}
		//-----------------------
	}

	//Bug 0077223. OJ en conectores informix. Necesitamos tener reducido tambien la tabla base para que
	//el campo apunte al alias de la tabla base correcta.
	//De esta forma CheckConsistency sí podrá detectar y solucionar problemas con OJ en conectores con tabla base.
	if (_ChangeFieldsPointedToObjectsBasis() != M4_SUCCESS)
	{
		return M4_ERROR;
	}

	ClCheckJoins CheckJoins(m_pvcFrmObj,m_pvcFrmAls,m_pvcRel,m_pvcFld,m_pGlobVars,m_pstStatemWhr);
	
	// Hay que distinguir la base de datos en la que estamos.
	// Para Oracle, SQLServer y DB2 solo vamos a sacar un Denug_Info
	// Par Informix y Sybase vamos a cambiar los Outer joins por Joins y sacamos un Debug Info.
	if (CheckJoins.CheckConsistency(m_pGlobVars->GetDDBBType ())!=M4_SUCCESS)
	{
		return M4_ERROR;
	}

	return (M4_SUCCESS);
}

m4return_t ClMetaSentence::_AnalizeObjectsToReduceAndArrangeFields (void)
{
	for (m4int32_t indObj = m_pvcFrmObj->size ()-1; indObj>=0; indObj--)
	{
		itVcClMtObject	itObj = m_pvcFrmObj->begin () + indObj;
		ClMetaObject	*poOldObject = *itObj ;

		if ( (*m_pvcFrmAls)[poOldObject->m_indAlias]->m_bReduce)
		{
			if (poOldObject->m_indAlias == (*m_pvcFrmAls)[poOldObject->m_indAlias]->m_indNewAlias)
			{
				// Cuando el alias es el mismo es porque es un alias proveniente de seguridad asi que no lo reducimos.
				(*m_pvcFrmAls)[poOldObject->m_indAlias]->m_bReduce = M4_FALSE;
			}
			else
			{
				///Para que al reducir no perdamos las marcas de tablas reales utilizadas.
				m4int32_t OldMask = (*m_pvcFrmObj)[indObj]->m_MaskRealTables;

				m4int32_t NewIndObj = -1;
				m4int32_t NewIndNewAlias = poOldObject->m_indAlias;

				while (NewIndObj==-1)
				{
					
					NewIndNewAlias = (*m_pvcFrmAls)[NewIndNewAlias]->m_indNewAlias;
					
					NewIndObj = (*m_pvcFrmAls )[NewIndNewAlias]->m_indObj;
				}

				ClMetaObject	*poNewObject = (*m_pvcFrmObj)[NewIndObj] ;

				poNewObject->m_MaskRealTables |= OldMask;

				// Bug 0299870 Se redirige la cadena de herencia del objeto borrado al nuevo
				if( poOldObject->m_pParentObject != NULL )
				{
					for( itVcClMtObject it = poOldObject->m_pParentObject->m_oVcChildObject.begin(); it != poOldObject->m_pParentObject->m_oVcChildObject.end(); it++ )
					{
						if( *it == poOldObject )
						{
							// Se quita el objeto que se borra de la lista de hijos de su padre
							poOldObject->m_pParentObject->m_oVcChildObject.erase( it ) ;
							break ;
						}
					}

					if( poNewObject->m_pParentObject == NULL )
					{
						// Si el nuevo objeto no tiene padre se pone el del que se va a borrar
						poNewObject->m_pParentObject = poOldObject->m_pParentObject ;
						poNewObject->m_pParentObject->m_oVcChildObject.push_back( poNewObject ) ;
					}

					poOldObject->m_pParentObject = NULL ;
				}

				if( poOldObject->m_oVcChildObject.size() > 0 )
				{
					for( itVcClMtObject it = poOldObject->m_oVcChildObject.begin(); it != poOldObject->m_oVcChildObject.end(); it++ )
					{
						if( (*it)->m_pParentObject == poOldObject )
						{
							// Se quitan los padres que sean el objeto a borrar
							(*it)->m_pParentObject = NULL ;
						}
					}

					if( poNewObject->m_oVcChildObject.size() == 0 )
					{
						// Si el nuevo objeto no tiene hijos se ponen los del que se va a borrar
						for( itVcClMtObject it = poOldObject->m_oVcChildObject.begin(); it != poOldObject->m_oVcChildObject.end(); it++ )
						{
							poNewObject->m_oVcChildObject.push_back( *it ) ;
							(*it)->m_pParentObject = poNewObject ;
						}
					}

					poOldObject->m_oVcChildObject.clear() ;
				}

				delete (poOldObject);

				m_pvcFrmObj->erase (itObj);

				for (itVcClMtAlias itAls = m_pvcFrmAls->begin (); itAls!=m_pvcFrmAls->end (); itAls++)
				{
					if ((*itAls)->m_indObj == indObj)
					{
						(*itAls)->m_indObj = M4LDB_NULL_INDEX;
					}

					if ((*itAls)->m_indObj > indObj)
					{
						(*itAls)->m_indObj--;
					}
				}
			}
		}
	}

	if (m_pvcFld)
	{
		for (itVcClMtField itFld = m_pvcFld->begin (); itFld!=m_pvcFld->end (); itFld++)
		{				
			if ((*itFld)->IsMetaData ()==M4_TRUE)
			{
				while ( (*m_pvcFrmAls)[(*itFld)->m_indAlias]->m_bReduce)
				{
					(*itFld)->m_indAlias = (*m_pvcFrmAls)[(*itFld)->m_indAlias]->m_indNewAlias;
				}
			}
		}
	}

	return M4_SUCCESS;
}


m4bool_t ClMetaSentence::CanReadByPos (m4int32_t ai_indCol)
{
	ClMetaItemCol *pMtItem = _GetColumn(ai_indCol);

	if (pMtItem->m_hItem != M4LDB_NULL_INDEX)
	{
		if (!m_pGlobVars->GetpMCR ()->GetItemSecRead (pMtItem->m_hItem))
		{
			return (M4_FALSE);
		}
	}

	return (M4_TRUE);
}


m4return_t ClMetaSentence::InvalidateColumn	( m4int32_t ai_indCol )
{
	// Guardamos la posicion y longitud de la columna.

	ClMetaItemCol *pCol=0;
	m4int32_t PosIni = -1;
	m4int32_t Length = -1;
	m4uint32_t PosClause = (m4uint32_t)-1;
	eClauseType_t eClause = M4LDB_CLAUSE_END_STATEM; 

	// Hay que actualizar las posiciones de las columnas que sigan, asi como los indices de columna.
	// Busco la columna en cuestion.
	m4bool_t bFound =M4_FALSE;
	itVcClMtColumn itCol = m_pvcColumns->begin();
	itVcClMtColumn itColend = m_pvcColumns->end();
	for (;itCol!=itColend;++itCol)
	{
		pCol = (*itCol);
		if (pCol ->IsMetaData() == M4_TRUE)
		{
			if (bFound == M4_TRUE)
			{
				if (pCol->m_eClause==eClause)
				{
					// Si es de la misma clausula, hay que restar longitud.
					pCol->m_lPos-=Length;
				}
			}
			else if (pCol ->m_iColumn == ai_indCol)
			{
				bFound = M4_TRUE;
				PosIni = pCol->m_lPos;
				Length = pCol->m_lLength;
				eClause = pCol->m_eClause;
			}
		}

	}
	

	if (eClause==M4LDB_CLAUSE_COLUMN)
	{
		if (_FindClause(*m_pstStatemCol,M4LDB_CLAUSE_COLUMN,0,PosClause)!=M4_TRUE)
		{
			return M4_ERROR;
		}
		PosIni+=PosClause+M4LDB_MD_LENGTH_CLAUSE;
	}

	if (eClause==M4LDB_CLAUSE_LONG_COLUMN)
	{
		if (_FindClause(*m_pstStatemCol,M4LDB_CLAUSE_LONG_COLUMN,0,PosClause)!=M4_TRUE)
		{
			return M4_ERROR;
		}
		PosIni+=PosClause+M4LDB_MD_LENGTH_CLAUSE;
	}

	m_pstStatemCol->erase(PosIni,Length);


	// Borramos todo lo referente a esa columna.
	_ColumnDeleter<ClMetaField>	oFldDel;
	oFldDel(m_pvcFld,ai_indCol);
	_ColumnDeleter<ClMetaParam>	oFldPar;
	oFldPar(m_pvcParamCol,ai_indCol);
	_ColumnDeleter<ClMetaItemCol>	oFldCol;
	oFldCol(m_pvcColumns,ai_indCol);

	--m_numCol;
	// Si la columna de DT_LAST_UPDATE es mayor que la que se invalida hay que actualizarlo.
	if (m_indDtLastUpdate>ai_indCol)
	{
		--m_indDtLastUpdate;
	}

	return M4_SUCCESS;
}


ClMetaItemCol * ClMetaSentence::_GetColumn	(m4int32_t ai_indCol)
{
	itVcClMtColumn itCol = m_pvcColumns->begin();
	itVcClMtColumn itColend = m_pvcColumns->end();
	ClMetaItemCol *pCol=0;
	for (;itCol!=itColend;++itCol)
	{
		pCol=(*itCol);
		if (pCol->m_iColumn==ai_indCol && pCol->IsMetaData()==M4_TRUE)
		{
			//Encontrado;
			return pCol;
		}
	}
	return 0;
}


m4return_t ClMetaSentence::_CreateOneParam			(ClDinArr *ai_pParam,ClMetaParam *pParam,m4int32_t ai_indParam, m4bool_t ai_bUseOldValue)
{
	m4int32_t hItem=0;
	m4return_t result;
	ClCompiledMCR *pMCR=0;


	switch (pParam->GetType())
	{
	case M4LDB_PAR_EXTERN:
		result = ai_pParam->CreateData (ai_indParam,pParam);
		break;

	case M4LDB_PAR_SESSION:
		hItem = pParam->GetHandle(m_pGlobVars);
		pMCR = m_pGlobVars->GetpLConn ()->m_pLogonAdaptor->GetSessionRecordSet ()->GetpNodeDef ()->_GetpCMCR ();
		if (pMCR->GetItemType (hItem) == M4CL_ITEM_TYPE_METHOD)
		{
			m4id_t idField = pMCR->GetItemReadFieldId (hItem);
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_ITEM_IS_METHOD, hItem << idField);
			return (M4_ERROR);
		}
		result = ai_pParam->CreateData (ai_indParam,pParam);
		break;

	case M4LDB_PAR_ITEM:
		hItem = pParam->GetHandle(m_pGlobVars);
		if (m_pGlobVars->GetItemType (hItem) == M4CL_ITEM_TYPE_METHOD)
		{
			m4id_t idField = m_pGlobVars->GetItemReadIdField (hItem);
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_ITEM_IS_METHOD, hItem << idField);
			return (M4_ERROR);
		}
		pMCR = m_pGlobVars->GetpMCR ();
		result = ai_pParam->CreateData (ai_indParam,pParam);
		break;

	case M4LDB_PAR_INTERN:
		if (GetParam ())
		{
			result = ai_pParam->CreateData (ai_indParam, GetParam ()->GetRefData (pParam->GetHandle(m_pGlobVars)));
		}
		else
		{
			DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClMetaSentence::_CreateOneParam" << "Wrong internal parameter = " << LogCat << ai_indParam);
			return (M4_ERROR);
		}
		break;

	case M4LDB_PAR_MINUS_INF:
		result = ai_pParam->CreateData (ai_indParam, NULL, M4LDB_INTERNAL_HANDLE, M4CL_M4_TYPE_TIMESTAMP, M4LDB_SIZE_TIMESTAMP, 0, (m4pchar_t) &M4LDB_MINUS_INF_DATE, NULL, M4LDB_PAR_MINUS_INF, M4_FALSE);
		break;

	case M4LDB_PAR_PLUS_INF:
		result = ai_pParam->CreateData (ai_indParam, NULL, M4LDB_INTERNAL_HANDLE, M4CL_M4_TYPE_TIMESTAMP, M4LDB_SIZE_TIMESTAMP, 0, (m4pchar_t) &M4LDB_PLUS_INF_DATE, NULL, M4LDB_PAR_PLUS_INF, M4_FALSE);
		break;

	case M4LDB_PAR_AUTO_FILTER:
		result = ai_pParam->CreateData (ai_indParam, NULL, pParam->GetHandle(m_pGlobVars), pParam->GetM4Type(m_pGlobVars), pParam->GetPrecision(m_pGlobVars), 0, NULL, NULL, M4LDB_PAR_AUTO_FILTER, M4_FALSE);

		// Se establece el nivel de la organización y la herencia de datos
		// No se hace en el CreateData para no cambiar el API
		ai_pParam->SetInfoOrgLevel(ai_indParam, pParam->GetOrgLevel(m_pGlobVars) );
		ai_pParam->SetInfoInheritsData(ai_indParam, pParam->GetInheritsData(m_pGlobVars) );
		break;

	case M4LDB_PAR_UNDEF:
	default:
		result = M4_ERROR;
		break;
	}
	if (result == M4_ERROR)
	{
		DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (), M4_CH_LDB_INTERNAL_ERROR, "ClMetaSentence::CreatOneParam"<<"Param Index = "<<LogCat<<ai_indParam);
		return (M4_ERROR);
	}

#ifdef _M4LDB_DEBUG
	M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\tData[%ld] -> Type[%s]. Size[%ld].", ai_indParam, g_aM4Type[ai_pParam->GetInfoM4Type (ai_indParam)], ai_pParam->GetInfoPrec (ai_indParam));

	if (pParam->GetType() == M4LDB_PAR_SESSION || pParam->GetType() == M4LDB_PAR_ITEM)
	{
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), " Itm[%s - %ld].", pMCR->GetItemId (hItem), hItem);
	}
#endif

	return M4_SUCCESS;
}


m4return_t ClMetaSentence::_AddInfoColumnsUpdateInsert(PVcClMtParam ai_pvcParamCol,PVcClMtField ai_pvcFieldCol)
{
	_VectorClearer<ClMetaField,ClMetaField,ClMetaField> oClean(ai_pvcFieldCol,0, 0);

	if (!ai_pvcParamCol || !ai_pvcFieldCol)
	{
		return (M4_SUCCESS);
	}

	if (!m_pmpFieldAndValue)
	{
		m_pmpFieldAndValue = new MpFieldAndValue;
	}

	m4int32_t numFld = ai_pvcFieldCol->size ();
	
	for (m4int32_t indFld=0; indFld<numFld; indFld++)
	{
		m_pmpFieldAndValue->insert (MpFieldAndValue::value_type ( (*ai_pvcFieldCol)[indFld]->m_stField, (*ai_pvcParamCol)[indFld]));
	}

	// Los parámetros de las columnas han sido traspasados.
	ai_pvcParamCol->clear ();

	return M4_SUCCESS;
}


ClMetaParam* ClMetaSentence::GetMetaParamByField (m4id_t ai_idField) 
{
	string idField (ai_idField);

	if (m_pmpFieldAndValue)
	{
		itMpFieldAndValue it = m_pmpFieldAndValue->find (idField);

		if (it == m_pmpFieldAndValue->end ())
		{
			return 0;
		}

		return ((*it).second);
	}
	return 0;
}


m4return_t ClMetaSentence::TranslateSentence( string &ai_stTrans, m4int32_t ai_indColIni, m4int32_t ai_indColEnd, m4bool_t ai_bIsSecurityView )
{
	m4return_t ret;

	ClTraductor	oTranslator(this, m_pGlobVars->GetDDBBType(), m_pGlobVars, ai_bIsSecurityView, &ai_stTrans);
	
	ret = _CleanOR ();
	if (ret!=M4_SUCCESS)
	{
		return ret;
	}

	ret = oTranslator.TranslateSentence(ai_stTrans,ai_indColIni,ai_indColEnd);
	if (ret!=M4_SUCCESS)
	{
		return ret;
	}

	// Hay que mirar el caso de las load auxiliares de Informix.
	return ret;

}

//Este miembro lo que hace es SIN eliminar los objetos base duplicados hacer que los campos
//apunten al alias de la tabla base correcto.
//Es un mix de ClearObjectsBasis y _AnalizeObjectsToReduceAndArrangeFields!!!.
m4return_t ClMetaSentence::_ChangeFieldsPointedToObjectsBasis (void)
{
	// Me recorro el vector de objetos, buscando las distintas copias que haya del objeto base.
	// Las que haya de más las limpio. ( Apunto el alias de cada una a su nuevo alias ).
	m4int32_t AliasBasis = M4LDB_NULL_INDEX;
	itVcClMtObject itObj= m_pvcFrmObj->begin();
	itVcClMtObject itObjend = m_pvcFrmObj->end();
	//_JoinRealTablesForBasis(0);

	//Declaramos una lista de indices de alias.
	list< m4int32_t >				vFrmAlsReduced;
	list< m4int32_t >::iterator		itFrmAlsReduced;
	m4int32_t						indAliasReduced;
	//Más peligro que Ferrero Roche en agosto!!.
	//En la reduccion, si hay un inner consideramos que todo es inner pq es más restrictivo.
	m4bool_t						bInnerJoin = M4_FALSE;

	//Bug 0080081
	m4bool_t	bFoundWhereFieldInner = M4_FALSE;

	ClMetaAlias * pAlias = NULL;

	ClMetaObject * pObj = 0;
	while (itObj != itObjend)
	{
		pObj = (*itObj);
		if (pObj->m_iLevel == 0)
		{
			// Solo puedo encontrar bases en el nivel 0.
			if (pObj->m_bIsBasis == M4_TRUE)
			{

				if (pObj->m_eJoinType == M4LDB_NORMAL_JOIN)
				{
					 bInnerJoin = M4_TRUE;
				}

				// Encontrado un base. ¿Es el primero?
				if (AliasBasis == M4LDB_NULL_INDEX)
				{
					AliasBasis = pObj->m_indAlias;

					//Importante!!
					//El primero del vector sera el objeto base que nos quedaremos finalmente!!!
					//Es decir, el primero no acabara siendo reducido!!.
					vFrmAlsReduced.push_front (AliasBasis);
				}
				else
				{
					//Metemos el alias en el vector temporal de alias reducidos!!.
					vFrmAlsReduced.push_back (pObj->m_indAlias);
				}
			}
			++itObj;
		}
		else
		{
			break;
		}
	}

	//-------------------------------
	//En este punto tenemos un vector de alias donde el primero es el alias al que apuntaran los campos
	//Nos recorremos el vector y hacemos que los campos apunten al alias correcto.

	//Si hay campos haremos la comprobación. Ademas hemos tenido que haber encontrado alias base!!.
	//Si hay alias base lo hemos puesto al principio de la lista.
	if ( (m_pvcFld) && (AliasBasis != M4LDB_NULL_INDEX) )	
	{
		for (itFrmAlsReduced = vFrmAlsReduced.begin (); itFrmAlsReduced != vFrmAlsReduced.end (); itFrmAlsReduced ++)
		{				
			indAliasReduced = (*itFrmAlsReduced);

			//Si es true significa que alguna base es inner, vamos a marcar todas las bases que se reduzcan como inner.
			//Incluso la de la posicion 0.
			if (bInnerJoin == M4_TRUE)
			{
				m4int32_t	indObjectPos = GetObjectPosByAliasPos (indAliasReduced);
				if (indObjectPos != M4LDB_NULL_INDEX)
				{
					(*m_pvcFrmObj) [indObjectPos]->m_eJoinType = M4LDB_NORMAL_JOIN;
				}
				else
				{
					return M4_ERROR;
				}
			}

			//Bug 0080081
			//Comentamos este bloque para forzar a que se recorra los campos de la tabla que nos quedamos, 
			//por si hay algun campo con inner.			
			//El primer elemento es el base, no se comprueban los campos.
			////if (indAliasReduced == AliasBasis) {
			////	continue;
			////}

			//Recorremos los campos y hacemos que apunten al nuevo alias.			
			for (itVcClMtField itFld = m_pvcFld->begin (); itFld != m_pvcFld->end (); itFld++)
			{				
				if ( (*itFld)->IsMetaData() == M4_TRUE && (*itFld)->m_indAlias == indAliasReduced )
				{
						//Bug 0080081
						//Si el campo es where y es inner, me lo apunto , pq alguien quiere forzar un inner.
						if ( (*itFld)->m_eClause==M4LDB_CLAUSE_WHERE && (*itFld)->m_bIsOuter == M4_FALSE )
						{
							bFoundWhereFieldInner = M4_TRUE;
						}
						//Si hemos encontrado un campo para reducir, hacemos que apunte a base que es el primero del vector.					
						(*itFld)->m_indAlias = AliasBasis;
					}				
			}
		}
	}
	//-------------------------------


	//-------------------------------
	//En Informix y SQLServer tenemos un problema:
	//Una sentencia + APISQL FROM &Base, Alias WHERE Base.A (+)=Alias.B Nunca haría el OJ
	//Porque la tabla base se reduce y no se marca como OJ. Para ambos gestores el miembro AddOuterJoinInCondition
	//no hace nada, por lo tanto no mete el OJ necesario.
	//Vamos a recorrer las relaciones y marcaremos de nuevo los objetos como OJ si lo indica la relación.
	itVcClMtRelat itRelation;
	ClMetaRelat * pRel = NULL;
	for (itRelation = m_pvcRel->begin (); itRelation != m_pvcRel->end (); itRelation++)
	{
		m4int32_t	indObjectPos = M4LDB_NULL_INDEX;
		m4int32_t	indAliasPos = M4LDB_NULL_INDEX;

		pRel = (*itRelation);
		if (pRel->m_eJoinType == M4LDB_OUTER_JOIN && pRel->m_bReduce != M4_TRUE)
		{
			//Se marca el campo de la derecha de la relacion que ya viene ordenada A=B(+)
			indAliasPos = pRel->m_pMtField2->m_indAlias;
			if (indAliasPos == M4LDB_NULL_INDEX)
			{
				continue;
			}
			m4int32_t	indObjectPos = GetObjectPosByAliasPos (indAliasPos);
			if (indObjectPos != M4LDB_NULL_INDEX)
			{
				//Bug 0080081
				//Si hay un campo de las reducidas que se ha puesto a inner, todas serán inner.
				m4bool_t	bFound = M4_FALSE;
				for (itFrmAlsReduced = vFrmAlsReduced.begin (); itFrmAlsReduced != vFrmAlsReduced.end (); itFrmAlsReduced ++)
				{
					indAliasReduced = (*itFrmAlsReduced);
					if ( (indAliasReduced == indAliasPos) && (bFoundWhereFieldInner == M4_TRUE) )
					{
						bFound = M4_TRUE;
						break;
					}
				}
				if (bFound == M4_TRUE)
				{
					continue;
				}

				(*m_pvcFrmObj) [indObjectPos]->m_eJoinType = M4LDB_OUTER_JOIN;
			}
		}		
	}
	//-------------------------------

	return M4_SUCCESS;
}


m4return_t ClMetaSentence::ClearObjectsBasis(void)
{
	// Me recorro el vector de objetos, buscando las distintas copias que haya del objeto base.
	// Las que haya de más las limpio. ( Apunto el alias de cada una a su nuevo alias ).
	m4int32_t AliasBasis = -1;
	itVcClMtObject itObj= m_pvcFrmObj->begin();
	itVcClMtObject itObjend = m_pvcFrmObj->end();
	_JoinRealTablesForBasis(0);


	ClMetaObject * pObj=0;
	while (itObj!=itObjend)
	{
		pObj =(*itObj);
		if (pObj->m_iLevel==0)
		{
			// Solo puedo encontrar bases en el nivel 0.
			if (pObj->m_bIsBasis==M4_TRUE)
			{
				// Encontrado un base. 
				// ¿Es el primero?
				if (AliasBasis==-1)
				{
					AliasBasis = pObj->m_indAlias;
				}
				else
				{
					// Pongo su alias apuntando al alias del objeto base con el que me quedo.
					ClMetaAlias *pAlias = (*m_pvcFrmAls)[pObj->m_indAlias];
					pAlias->m_indNewAlias = AliasBasis;
					m4int32_t indObj = pAlias->m_indObj; // Me lo guardo para posicionarme
					pAlias->m_indObj = -1;
					pAlias->m_bReduce=M4_TRUE;
					// Tengo que borrar el objeto.
					m_pvcFrmObj->erase (itObj);
					delete pObj;
					// Me recorro todos los Alias, actualizando los indices de los objetos.
					// Hay que restarles uno a todos los que tengan indice mayor.
					itVcClMtAlias itAls=  m_pvcFrmAls->begin();
					itVcClMtAlias itAlsend=  m_pvcFrmAls->end();
					while (itAls!=itAlsend)
					{
						M4_ASSERT((*itAls)->m_indObj!=indObj);
						if ((*itAls)->m_indObj>indObj)
						{
							--(*itAls)->m_indObj;
						}
						++itAls;
					}

					// Me vuelvo a posicionar ( el siguiente se habra quedado donde estaba ).
					itObj = m_pvcFrmObj->begin()+indObj -1; // Resto 1 porque luego se me sumara 1.
					itObjend = m_pvcFrmObj->end();
				}
			}
			++itObj;
		}
		else
		{
			break;
		}
	}

	return M4_SUCCESS;
}


void ClMetaSentence::_JoinRealTablesForBasis  (m4int32_t ai_iLevel)
{
	// Lo importante es que quede el primero con todos las tablas reales.
	// Empezamoss por atras.

	m4int32_t SizeObj = GetNumObject();
	m4int32_t AcumMask = 0;

	ClMetaObject *pObj = 0;

	// Lo recorremos para atras para dejarlo en el primero.
	for (m4int32_t i=SizeObj -1; i>=0;--i)
	{
		pObj = (*m_pvcFrmObj)[i];
		if ( pObj->m_bIsBasis && pObj->m_iLevel == ai_iLevel)
		{
			AcumMask |= pObj->m_MaskRealTables;
			pObj->m_MaskRealTables = AcumMask;
		}
	}

}


m4int32_t ClMetaSentence::GetNewAliasPosByAliasPos(m4int32_t ai_indAlias)
{
	m4int32_t indAls = ai_indAlias;
	if (ai_indAlias==-1)
	{
		return M4_ERROR;
	}
	ClMetaAlias * pAlias = (*m_pvcFrmAls)[ai_indAlias];
	while (pAlias->m_bReduce)
	{
		if (indAls == pAlias->m_indNewAlias)
		{
			// En el caso de filtros de seguridad, puede darse el caso de que un newAlias apunte a sí mismo,
			// ya que el alias utilizado para analizar el filtro de seguridad es el mismo que el de la sentencia de la TI.
			// Al reducir relaciones se puede dar esta situación.
			break;
		}
		indAls = pAlias->m_indNewAlias;
		pAlias = (*m_pvcFrmAls)[indAls];
	}
	return indAls;
}


m4return_t ClMetaSentence::_X_AnalizeRelations (void)
{
	if (!m_pvcRel || m_pvcRel->empty())
	{
		// Nada que analizar
		return (M4_SUCCESS);
	}
	

	for (m4int32_t indRel1 = m_pvcRel->size ()-1; indRel1>=0; indRel1--)
	{
		itVcClMtRelat itRel1 = m_pvcRel->begin () + indRel1;

		m4bool_t bFirst = M4_TRUE;

		for (m4int32_t indRel2 = indRel1-1; indRel2>=0; indRel2--)
		{
			itVcClMtRelat itRel2 = m_pvcRel->begin () + indRel2;

			//-------------------------------------------
			//No se esta detectando A.a=B.b(+) And B.c(+)=A.d
			//En este escenario tendremos tambien que dar error!. Es decir, no vamos a controlar los campos
			if ((((*itRel1)->m_pMtField1->m_indAlias == (*itRel2)->m_pMtField1->m_indAlias) && 
				((*itRel1)->m_pMtField2->m_indAlias == (*itRel2)->m_pMtField2->m_indAlias) && 				
				((*itRel1)->m_pMtField1->m_indRealTable == (*itRel2)->m_pMtField1->m_indRealTable) && 
				((*itRel1)->m_pMtField2->m_indRealTable == (*itRel2)->m_pMtField2->m_indRealTable)) || 
				(((*itRel1)->m_pMtField1->m_indAlias == (*itRel2)->m_pMtField2->m_indAlias) && 
				((*itRel1)->m_pMtField2->m_indAlias == (*itRel2)->m_pMtField1->m_indAlias) && 
				((*itRel1)->m_pMtField1->m_indRealTable == (*itRel2)->m_pMtField2->m_indRealTable) && 
				((*itRel1)->m_pMtField2->m_indRealTable == (*itRel2)->m_pMtField1->m_indRealTable)))
			{

					if ((*itRel1)->m_eJoinType==(*itRel2)->m_eJoinType)
					{
						// Si fueran Outer, tienen que ser en le mismo sentido:
						if ((*itRel1)->m_eJoinType==M4LDB_OUTER_JOIN)
						{
							if ((*itRel1)->m_pMtField1->m_indAlias != (*itRel2)->m_pMtField1->m_indAlias)
							{
								// Error. Hay Outer Joins en distinto sentido.
								m4pcchar_t nlgcTbl1 = m_pGlobVars->GetpLConn ()->GetnLgcTbl( (*itRel1)->m_pMtField1->m_indLogicTable);
								m4pcchar_t iLgcTbl1 = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[(*itRel1)->m_pMtField1->m_indLogicTable].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());

								m4pcchar_t nlgcTbl2 = m_pGlobVars->GetpLConn ()->GetnLgcTbl( (*itRel1)->m_pMtField2->m_indLogicTable);
								m4pcchar_t iLgcTbl2 = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[(*itRel1)->m_pMtField2->m_indLogicTable].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());

								DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (),M4_CH_LDB_OUTER_JOIN_CONFLICT,
									LOG_CAT_AND (iLgcTbl1, nlgcTbl1) << 
									LOG_CAT_AND (iLgcTbl2, nlgcTbl2)
									);
								return M4_ERROR;
							}
						}
					}
			}
			//-------------------------------------------

			// Quitamos las relaciones iguales que esten en un mismo filtro. ( Mismos alias y mismos campos )
			if ((((*itRel1)->m_pMtField1->m_indAlias == (*itRel2)->m_pMtField1->m_indAlias) && 
				((*itRel1)->m_pMtField2->m_indAlias == (*itRel2)->m_pMtField2->m_indAlias) && 
				((*itRel1)->m_pMtField1->m_indRealField == (*itRel2)->m_pMtField1->m_indRealField) && 
				((*itRel1)->m_pMtField2->m_indRealField == (*itRel2)->m_pMtField2->m_indRealField) && 
				((*itRel1)->m_pMtField1->m_indRealTable == (*itRel2)->m_pMtField1->m_indRealTable) && 
				((*itRel1)->m_pMtField2->m_indRealTable == (*itRel2)->m_pMtField2->m_indRealTable)) || 
				(((*itRel1)->m_pMtField1->m_indAlias == (*itRel2)->m_pMtField2->m_indAlias) && 
				((*itRel1)->m_pMtField2->m_indAlias == (*itRel2)->m_pMtField1->m_indAlias) && 
				((*itRel1)->m_pMtField1->m_indRealField == (*itRel2)->m_pMtField2->m_indRealField) && 
				((*itRel1)->m_pMtField2->m_indRealField == (*itRel2)->m_pMtField1->m_indRealField) && 
				((*itRel1)->m_pMtField1->m_indRealTable == (*itRel2)->m_pMtField2->m_indRealTable) && 
				((*itRel1)->m_pMtField2->m_indRealTable == (*itRel2)->m_pMtField1->m_indRealTable)))
			{
				// Comprobamos que los joins sean del mismo tipo.
				if ((*itRel1)->m_eJoinType==(*itRel2)->m_eJoinType)
				{
					// Si fueran Outer, tienen que ser en le mismo sentido:
					if ((*itRel1)->m_eJoinType==M4LDB_OUTER_JOIN)
					{
						if ((*itRel1)->m_pMtField1->m_indAlias != (*itRel2)->m_pMtField1->m_indAlias)
						{
							// Error. Hay Outer Joins en distinto sentido.
							m4pcchar_t nlgcTbl1 = m_pGlobVars->GetpLConn ()->GetnLgcTbl( (*itRel1)->m_pMtField1->m_indLogicTable);
							m4pcchar_t iLgcTbl1 = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[(*itRel1)->m_pMtField1->m_indLogicTable].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());

							m4pcchar_t nlgcTbl2 = m_pGlobVars->GetpLConn ()->GetnLgcTbl( (*itRel1)->m_pMtField2->m_indLogicTable);
							m4pcchar_t iLgcTbl2 = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[(*itRel1)->m_pMtField2->m_indLogicTable].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());

							DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (),M4_CH_LDB_OUTER_JOIN_CONFLICT,
								LOG_CAT_AND (iLgcTbl1, nlgcTbl1) << 
								LOG_CAT_AND (iLgcTbl2, nlgcTbl2)
								);
							return M4_ERROR;
						}
					}
				}
				else
				{
					// Hay un Inner y un Outer.
					//Bug 0083090
					// Vamos a quedarnos con el INNER!!. Que se de la primera relacion.
					if ((*itRel1)->m_eJoinType==M4LDB_NORMAL_JOIN)
					{
						// Tenemos que cambiarlo, porque nos quedamos con la primera relación.

						// Miramos elsentido del Outer Join.
						if ((*itRel1)->m_pMtField1->m_indAlias != (*itRel2)->m_pMtField1->m_indAlias)
						{
							// Hay que dar la vuelta a los campos de la relacion.
							ClMetaField *pField = (*itRel1)->m_pMtField1;
							(*itRel1)->m_pMtField1 = (*itRel1)->m_pMtField2;
							(*itRel1)->m_pMtField2 = pField;
						}
					}
				}

				//Bug 0087773
				//Es posible que no tengamos que eliminar la relacion:
				//Si las relaciones son de XJoin y cualquier otra, la de XJoin no debe provocar la eliminacion de la otra.
				if ( (*itRel2)->GetAPISQLSource( ) != M4LDB_APISQL_FROM_SECURITY_XJOIN
					&& (*itRel1)->GetAPISQLSource( ) == M4LDB_APISQL_FROM_SECURITY_XJOIN )
				{
					continue;
				}

				delete (*itRel2);
				m_pvcRel->erase (itRel2);
				if (bFirst)
				{
					bFirst = M4_FALSE;
				}
				else {
					indRel1--;
				}
			}
		}
	}


	m4bool_t bReduce = M4_FALSE;

	for (itVcClMtRelat itr1 = m_pvcRel->begin (); itr1!=m_pvcRel->end (); itr1++)
	{
		m4int32_t indAliast1r1 = (*itr1)->m_pMtField1->m_indAlias;
		
		m4int32_t indAliast2r1 = (*itr1)->m_pMtField2->m_indAlias;

		// Si ya hemos reducido los alias, o es una 'pseudo-relacion' (entre dos niveles distintos)
		// seguimos con la siguiente.
		if ((( (*m_pvcFrmAls)[indAliast1r1]->m_bReduce) && 
			( (*m_pvcFrmAls)[indAliast2r1]->m_bReduce)) || 
			( (*m_pvcFrmAls)[indAliast1r1]->m_iLevel != (*m_pvcFrmAls)[indAliast2r1]->m_iLevel))
		{
			continue;
		}

		//Bug 0122107 
		//Comprobamos sociedad del canal y de las tablas.
		ClChannel * pChannel = m_pGlobVars->GetpAccessRecordSet()->GetpAccess()->GetpChannel();
		m4double_t dOrganizationType;
		pChannel->OrganizationType.Get(dOrganizationType);
		m4int32_t iOrgType = (m4int32_t)dOrganizationType;

		if (( (*itr1)->m_bReduce == FALSE) && (iOrgType == M4CL_ORGANIZATION_TYPE_MONO))
		{				
			ClMetaObject * pObj1 = (*m_pvcFrmObj)[GetObjectPosByAliasPos(indAliast1r1)];
			ClMetaObject * pObj2 = (*m_pvcFrmObj)[GetObjectPosByAliasPos(indAliast2r1)];

			//Si tienen el mismo comportamiento y no es dependencia avanzada.
			if (pObj2->m_iOrgType == pObj1->m_iOrgType
				&& pObj1->m_iOrgType != M4CL_ORGANIZATION_TYPE_NONE)
			{
				//Si es relacion por organizacion la reducimos.
				if (_GetIdInternalField ((*itr1)->m_pMtField1) == M4LDB_ID_ORGANIZATION &&
					_GetIdInternalField ((*itr1)->m_pMtField2) == M4LDB_ID_ORGANIZATION)
				{				
					(*itr1)->m_bReduce = M4_TRUE;
					bReduce = M4_TRUE;
					continue;
				}

			}
		}

		for (itVcClMtRelat itr2 = itr1+1; itr2!=m_pvcRel->end (); itr2++)
		{
			// Si estamos en distinto nivel.
			if (
				((*itr1)->m_pMtField1->m_iLevel != (*itr2)->m_pMtField1->m_iLevel) || 
				((*itr1)->m_pMtField1->m_iFilter == (*itr2)->m_pMtField1->m_iFilter) || 
				(( (*m_pvcFrmAls)[(*itr2)->m_pMtField1->m_indAlias]->m_bReduce) && 
				( (*m_pvcFrmAls)[(*itr2)->m_pMtField2->m_indAlias]->m_bReduce)) || 
				( (*m_pvcFrmAls)[(*itr2)->m_pMtField1->m_indAlias]->m_iLevel != (*m_pvcFrmAls)[(*itr2)->m_pMtField2->m_indAlias]->m_iLevel) || 
				(itr1 == itr2))
			{
				continue;
			}

			// Buscamos tablas duplicadas en las relaciones.
			m4int32_t indAliast1r2 = M4LDB_NULL_INDEX;
			
			m4int32_t indAliast2r2 = M4LDB_NULL_INDEX;
			
			if ((*itr1)->m_pMtField1->m_indLogicTable == (*itr2)->m_pMtField1->m_indLogicTable && 
				(*itr1)->m_pMtField2->m_indLogicTable == (*itr2)->m_pMtField2->m_indLogicTable)
			{
				indAliast1r2 = (*itr2)->m_pMtField1->m_indAlias;
				indAliast2r2 = (*itr2)->m_pMtField2->m_indAlias;
			}
			else
			{
				if ((*itr1)->m_pMtField1->m_indLogicTable == (*itr2)->m_pMtField2->m_indLogicTable && 
					(*itr1)->m_pMtField2->m_indLogicTable == (*itr2)->m_pMtField1->m_indLogicTable)
				{
					if ((*itr1)->m_eJoinType == M4LDB_OUTER_JOIN && (*itr2)->m_eJoinType == M4LDB_OUTER_JOIN)
					{
						// Bug 0297278, 0298439 Los outers cruzados no se reducen
						continue;
					}
					else
					{
						indAliast2r2 = (*itr2)->m_pMtField1->m_indAlias;
						indAliast1r2 = (*itr2)->m_pMtField2->m_indAlias;
					}
				}
				else
				{
					continue;
				}
			}

			// Hemos encontrado tablas duplicadas.
			// A partir de ahora, toda relación indAliast1r1,indAliast2r1 tiene que tener otra relación indAliast1r2,indAliast2r2.
			m4bool_t bExists = M4_FALSE;
			
			// RRRR
			// El comportamiento si hay conflictos entre Outer Joins y Joins es distintovsegun la base de datos.
			// Me creo un vector donde voy metiendo las relaciones que se vayan reducir.

			VcClMtRelat		vRelations;
			vRelations.reserve(8);

			//Bug 0089387. 09/12/2003
			//Vamos a meter todas las relaciones que tengan los alias en un vector para tratarlos
			bExists = _AnalizeRelationsDuplicated( vRelations, indAliast1r1, indAliast2r1, indAliast1r2, indAliast2r2 );

			if (bExists && _CheckRelationsToReduce(indAliast1r1,indAliast2r1,indAliast1r2,indAliast2r2))
			{
				(*m_pvcFrmAls)[indAliast1r2]->m_bReduce = M4_TRUE;
				(*m_pvcFrmAls)[indAliast1r2]->m_indNewAlias = indAliast1r1;
				
				(*m_pvcFrmAls)[indAliast2r2]->m_bReduce = M4_TRUE;
				(*m_pvcFrmAls)[indAliast2r2]->m_indNewAlias = indAliast2r1;

				(*itr2)->m_bReduce = M4_TRUE;

				bReduce = M4_TRUE;

				for (itVcClMtRelat itr5 = itr2+1; itr5!=m_pvcRel->end (); itr5++)
				{
					if ((((*itr5)->m_pMtField1->m_indAlias==indAliast1r2) &&
						((*itr5)->m_pMtField2->m_indAlias==indAliast2r2)) ||
						(((*itr5)->m_pMtField1->m_indAlias==indAliast2r2) &&
						((*itr5)->m_pMtField2->m_indAlias==indAliast1r2)))
					{
						(*itr5)->m_bReduce = M4_TRUE;
					}
				}

				// Despues de reducir se ha podido cambiar el orden de los joins

				ClOuterJoinReductor_Oracle	oOJR;
				m4bool_t bHasChanges=M4_FALSE;
				if (oOJR.ReduceOuterJoins(vRelations,this,bHasChanges)!=M4_SUCCESS)
				{
					// Error. Hay Outer Joins en los dos sentidos,
					m4pcchar_t nlgcTbl1 = m_pGlobVars->GetpLConn ()->GetnLgcTbl( (*itr1)->m_pMtField1->m_indLogicTable);
					m4pcchar_t iLgcTbl1 = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[(*itr1)->m_pMtField1->m_indLogicTable].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());

					m4pcchar_t nlgcTbl2 = m_pGlobVars->GetpLConn ()->GetnLgcTbl( (*itr1)->m_pMtField2->m_indLogicTable);
					m4pcchar_t iLgcTbl2 = M4LDB_ID (m_pGlobVars->GetpLConn ()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current[(*itr1)->m_pMtField2->m_indLogicTable].Item[M4LDB_ITEM_SYS_DICT_LOGIC_OBJECTS_ID_OBJECT].Value.GetWithOutSec ());

					DUMP_LDBINSP_ERRORF (m_pGlobVars->GetpLConn (),M4_CH_LDB_OUTER_JOIN_CONFLICT,
						LOG_CAT_AND (iLgcTbl1, nlgcTbl1) << 
						LOG_CAT_AND (iLgcTbl2, nlgcTbl2)
						);
					return M4_ERROR;
				}
				if (bHasChanges)
				{
					// Ha habido cambio. Me han podido cambiar el orden de los joins.
					// Actualizo los valores.
					indAliast1r1 = (*itr1)->m_pMtField1->m_indAlias;
					indAliast2r1 = (*itr1)->m_pMtField2->m_indAlias;
				}
			}
			// Si no hay que reducir, borrro el vector auxiliar.
			vRelations.erase(vRelations.begin(),vRelations.end());

		}
	}

	//---------------------------------------------------------
	//Analizamos la posibilidad de eliminar relaciones por xjoin. Indicamos que si se ha reducido xjoin
	if (_AnalizeXJoinRelations () == M4_TRUE)
	{
		bReduce = M4_TRUE;
	}
	//---------------------------------------------------------
	
	//---------------------------------------------------------
	//Analizamos la posibilidad de eliminar relaciones con objetos relacionados por herencia.
	if (_AnalizeRelationsWithInheritObjects() == M4_TRUE)
	{
		bReduce = M4_TRUE;
	}
	//---------------------------------------------------------

	//---------------------------------------------------------
	//Volcamos como han quedado las relaciones.
	DumpInfoRelations();
	//---------------------------------------------------------

	if (bReduce)
	{
		// Esto tiene que estar antes que la reducción de objetos porque puede ser que se invalide la reducción.
		for (m4int32_t indRel = m_pvcRel->size ()-1; indRel>=0; indRel--)
		{
			itVcClMtRelat itRel = m_pvcRel->begin () + indRel;

			if ((*itRel)->m_bReduce)
			{
				delete (*itRel);
				m_pvcRel->erase (itRel);
			}
			else
			{
				while ( (*m_pvcFrmAls)[(*itRel)->m_pMtField1->m_indAlias]->m_bReduce)
				{
					if ((*itRel)->m_pMtField1->m_indAlias != (*m_pvcFrmAls)[(*itRel)->m_pMtField1->m_indAlias]->m_indNewAlias)
					{
						(*itRel)->m_pMtField1->m_indAlias = (*m_pvcFrmAls)[(*itRel)->m_pMtField1->m_indAlias]->m_indNewAlias;
					}
					else
					{
						// Cuando el alias es el mismo es porque es un alias proveniente de seguridad.
						break;
					}
				}
				while ( (*m_pvcFrmAls)[(*itRel)->m_pMtField2->m_indAlias]->m_bReduce)
				{
					if ((*itRel)->m_pMtField2->m_indAlias != (*m_pvcFrmAls)[(*itRel)->m_pMtField2->m_indAlias]->m_indNewAlias)
					{
						(*itRel)->m_pMtField2->m_indAlias = (*m_pvcFrmAls)[(*itRel)->m_pMtField2->m_indAlias]->m_indNewAlias;
					}
					else
					{
						// Cuando el alias es el mismo es porque es un alias proveniente de seguridad.
						break;
					}
				}
			}
		}

		//-----------------------
		//Refactorizamos, implementado el miembro _AnalizeObjectsToReduceAndArrangeFields ()
		if (_AnalizeObjectsToReduceAndArrangeFields () != M4_SUCCESS)
		{
			return M4_ERROR;
		}
		//-----------------------
	}

	//Bug 0077223. OJ en conectores informix. Necesitamos tener reducido tambien la tabla base para que
	//el campo apunte al alias de la tabla base correcta.
	//De esta forma CheckConsistency sí podrá detectar y solucionar problemas con OJ en conectores con tabla base.
	if (_ChangeFieldsPointedToObjectsBasis() != M4_SUCCESS)
	{
		return M4_ERROR;
	}

	ClCheckJoins CheckJoins(m_pvcFrmObj,m_pvcFrmAls,m_pvcRel,m_pvcFld,m_pGlobVars,m_pstStatemWhr);
	
	// Hay que distinguir la base de datos en la que estamos.
	// Para Oracle, SQLServer y DB2 solo vamos a sacar un Denug_Info
	// Par Informix y Sybase vamos a cambiar los Outer joins por Joins y sacamos un Debug Info.
	if (CheckJoins.CheckConsistency(m_pGlobVars->GetDDBBType ())!=M4_SUCCESS)
	{
		return M4_ERROR;
	}

	return (M4_SUCCESS);
}


void ClMetaSentence::DumpInfoRelations( void )
{
	#ifdef _M4LDB_DEBUG

	M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn(), "\nRelations");
	M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_LOGIC_SENTENCES, m_pGlobVars->GetpLConn(), "\nRelations");

	for (itVcClMtRelat itRel1 = m_pvcRel->begin(); itRel1 != m_pvcRel->end(); itRel1++)
	{
		ClMetaRelat * pRel1 = (*itRel1);
		//Preguntamos por la activación de traza para ahorraros la busqueda de id,...
		if (m_pGlobVars->GetpLConn ()->m_lSysDebugLevel <= m_pGlobVars->GetpLConn ()->m_pLdb->GetSysDebugLevel ())
		{
			//Obtenemos el objeto izquierdo de la relacion.
			ClMetaObject	*pLgcObjLeft = GetObjectByPos(GetObjectPosByAliasPos(pRel1->m_pMtField1->m_indAlias));
			ClMetaObject	*pLgcObjRight = GetObjectByPos(GetObjectPosByAliasPos(pRel1->m_pMtField2->m_indAlias));
			if ((pLgcObjLeft) && (pLgcObjRight))
			{
				string sOuter = M4LDB_EMPTY;
				string sRelationSource = "[ApiSql]";
				string sReduced = M4LDB_EMPTY;
				if (pRel1->m_eJoinType == M4LDB_OUTER_JOIN)
				{
					sOuter = "(+)";
				}
				if (pRel1->m_nRelationSource == M4LDB_APISQL_FROM_SECURITY)
				{
					sRelationSource = "[Secur.]";
				}
				else
				{
					if (pRel1->m_nRelationSource == M4LDB_APISQL_FROM_SECURITY_XJOIN)
					{
						sRelationSource = "[XJoin]";
					}
				}
				if (pRel1->m_bReduce == M4_TRUE)
				{
					sReduced = "(Reduced)";
				}
				
				M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn(), "\n\t\tRelation: %s (%s)->%d.%s = (%s)->%d.%s %s %s",
																					sRelationSource.c_str(), 
																					pLgcObjLeft->m_stObject.c_str(), 
																					pRel1->m_pMtField1->m_indAlias, 
																					pRel1->m_pMtField1->m_stField.c_str(), 
																					pLgcObjRight->m_stObject.c_str(), 
																					pRel1->m_pMtField2->m_indAlias, 
																					pRel1->m_pMtField2->m_stField.c_str(), 
																					sOuter.c_str(),
																					sReduced.c_str());
				M4LDB_PRINT_DEBUG_DETAIL (M4LDB_DETAIL_LEVEL_SHOW_LOGIC_SENTENCES, m_pGlobVars->GetpLConn(), "\n\t\tRelation: %s (%s)->%d.%s = (%s)->%d.%s %s %s",
																					sRelationSource.c_str(), 
																					pLgcObjLeft->m_stObject.c_str(), 
																					pRel1->m_pMtField1->m_indAlias, 
																					pRel1->m_pMtField1->m_stField.c_str(), 
																					pLgcObjRight->m_stObject.c_str(), 
																					pRel1->m_pMtField2->m_indAlias, 
																					pRel1->m_pMtField2->m_stField.c_str(), 
																					sOuter.c_str(),
																					sReduced.c_str());
			}
		}
	}
	#endif
}


m4bool_t ClMetaSentence::_AnalizeRelationsWithInheritObjects( void )
{
	m4bool_t bHasReduction = M4_FALSE;

	//El escenario es el siguiente:
	//A.c1=C.c1(+)
	//B.c1=C.c1(+) 
	//En el caso de que B sea descendiente de A eliminaremos la relacion B=C(+).
	//En principio no restringimos a solo pk. Esto es una solucion chapuza pq evidentemente
	//hay casos sin controlar pero ya se sabe.
	
	ClMetaObject	*pLgcObjLeft1 = NULL;
	ClMetaObject	*pLgcObjRight1 = NULL;
	ClMetaObject	*pLgcObjLeft2 = NULL;
	ClMetaObject	*pLgcObjRight2 = NULL;

	m4int32_t iSize = m_pvcRel->size();
	ClMetaRelat * pRel1 = NULL;
	ClMetaRelat * pRel2 = NULL;
	for (m4int32_t indRel1=0; indRel1<iSize; indRel1++)
	{
		pRel1 = (*m_pvcRel)[indRel1];
		if ((pRel1 != NULL) && (pRel1->m_bReduce == M4_FALSE))
		{
			m4int32_t iAliasLeft1 = GetObjectPosByAliasPos( pRel1->m_pMtField1->m_indAlias );
			m4int32_t iAliasRight1 = GetObjectPosByAliasPos( pRel1->m_pMtField2->m_indAlias );
			if ((iAliasLeft1 == M4LDB_NULL_INDEX) || (iAliasRight1 == M4LDB_NULL_INDEX))
			{
				continue;
			}
			pLgcObjLeft1 = GetObjectByPos( iAliasLeft1 );
			pLgcObjRight1 = GetObjectByPos( iAliasRight1 );

			for (m4int32_t indRel2 = indRel1+1; indRel2<iSize; indRel2++)
			{
				pRel2 = (*m_pvcRel)[indRel2];
				if ((pRel2 != NULL) && (pRel2->m_bReduce == M4_FALSE))
				{
					//Minimizamos la chapuza para que solo se realice cuando hay OJ.
					//Tiene que ser las dos OJ para reducir.
					if (pRel1->m_eJoinType != M4LDB_OUTER_JOIN || pRel2->m_eJoinType != M4LDB_OUTER_JOIN)
					{
						continue;
					}

					m4bool_t bIsInheritTable = M4_FALSE;
					ClMetaObject	*pLgcObjRel1 = NULL;
					ClMetaObject	*pLgcObjRel2 = NULL;

					m4int32_t iAliasLeft2 = GetObjectPosByAliasPos( pRel2->m_pMtField1->m_indAlias );
					m4int32_t iAliasRight2 = GetObjectPosByAliasPos( pRel2->m_pMtField2->m_indAlias );
					if (iAliasLeft2 == M4LDB_NULL_INDEX || iAliasRight2 == M4LDB_NULL_INDEX)
					{
						continue;
					}

					pLgcObjLeft2 = GetObjectByPos( iAliasLeft2 );					
					pLgcObjRight2 = GetObjectByPos( iAliasRight2 );
					//-------------------------------------------

					/* Bug 0279407
					Sólo se pueden eliminar una relación si la otra va sobre los mismos campos, no sólo sobre las mismas tablas
					*/
					if (pLgcObjLeft1 == pLgcObjLeft2
						&& strcmpi(pRel1->m_pMtField1->m_stField.c_str(), pRel2->m_pMtField1->m_stField.c_str()) == 0
						&& strcmpi(pRel1->m_pMtField2->m_stField.c_str(), pRel2->m_pMtField2->m_stField.c_str()) == 0)
					{
						//C.c1=A.c1(+)
						//C.c1=B.c1(+) 
						pLgcObjRel1 = pLgcObjRight1;
						pLgcObjRel2 = pLgcObjRight2;
					}
					else if (pLgcObjLeft1 == pLgcObjRight2
						&& strcmpi(pRel1->m_pMtField1->m_stField.c_str(), pRel2->m_pMtField2->m_stField.c_str()) == 0
						&& strcmpi(pRel1->m_pMtField2->m_stField.c_str(), pRel2->m_pMtField1->m_stField.c_str()) == 0)
					{
						//C.c1=A.c1(+)
						//B.c1=C.c1(+) 
						pLgcObjRel1 = pLgcObjRight1;
						pLgcObjRel2 = pLgcObjLeft2;
					}
					else if (pLgcObjRight1 == pLgcObjRight2
						&& strcmpi(pRel1->m_pMtField1->m_stField.c_str(), pRel2->m_pMtField1->m_stField.c_str()) == 0
						&& strcmpi(pRel1->m_pMtField2->m_stField.c_str(), pRel2->m_pMtField2->m_stField.c_str()) == 0)
					{
						//A.c1=C.c1(+)
						//B.c1=C.c1(+) 
						pLgcObjRel1 = pLgcObjLeft1;
						pLgcObjRel2 = pLgcObjLeft2;
					}
					else if (pLgcObjRight1 == pLgcObjLeft2
						&& strcmpi(pRel1->m_pMtField1->m_stField.c_str(), pRel2->m_pMtField2->m_stField.c_str()) == 0
						&& strcmpi(pRel1->m_pMtField2->m_stField.c_str(), pRel2->m_pMtField1->m_stField.c_str()) == 0)
					{
						//A.c1=C.c1(+)
						//C.c1=B.c1(+) 
						pLgcObjRel1 = pLgcObjLeft1;
						pLgcObjRel2 = pLgcObjRight2;
					}
					
					if (pLgcObjRel1 == pLgcObjRel2)
					{
						//C.c1=A.c1(+)
						//C.c2=A.c2(+) 
						//Esto se cumple cuando son relaciones identicas
						continue;
					}
					//-------------------------------------------

					if (pLgcObjRel1 != NULL && pLgcObjRel2 != NULL)
					{
						//Ya tenemos los dos objetos sobre los que quiero comprobar si hay herencia.

						//Comprobamos si B hereda de A.
						bIsInheritTable = pLgcObjRel1->MetaObjectIntoInheritanceChain(pLgcObjRel2);
						if (bIsInheritTable == M4_TRUE)
						{
							//Marcamos la relacion hija para reducir.						
							pRel2->m_bReduce = M4_TRUE;
							bHasReduction = M4_TRUE;
							continue;
						}
						//Comprobamos si A hereda de B.
						bIsInheritTable = pLgcObjRel2->MetaObjectIntoInheritanceChain(pLgcObjRel1);
						if (bIsInheritTable == M4_TRUE)
						{
							//Marcamos la relacion hija para reducir.
							pRel1->m_bReduce = M4_TRUE;
							bHasReduction = M4_TRUE;
							continue;
						}
					}
				}
			}
		}
	}
	return bHasReduction;
}


m4bool_t ClMetaSentence::_AnalizeXJoinRelations( void )
{
	m4bool_t bReduce = M4_FALSE;

	//Bug 0087773
	//Vamos a añadir un control que recorrera las relaciones xjoin.
	//dada una relacion xjoin origen-destino, si existe un conjunto de relaciones (no reducidas) que permiten llegar de
	//origen al destino podré eliminar la de xjoin.!!
	for (itVcClMtRelat itRel1 = m_pvcRel->begin(); itRel1 != m_pvcRel->end(); itRel1++)
	{
		ClMetaRelat * pRel1 = (*itRel1);
		if (pRel1->GetAPISQLSource() == M4LDB_APISQL_FROM_SECURITY_XJOIN && pRel1->m_bReduce == M4_FALSE)
		{
			ClMetaRelatSet_t oRelationSet;
			//Insertamos la relacion en el set.
			oRelationSet.insert( pRel1 );

			//Buscamos a partir del izquierdo
			m4bool_t bReduceXJoin = _ReduceXJoinRelation( oRelationSet, pRel1, pRel1->m_pMtField1->m_indAlias, M4LDB_NULL_INDEX, M4LDB_NULL_INDEX );
			if (bReduceXJoin == M4_TRUE)
			{
				pRel1->m_bReduce = M4_TRUE;
				bReduce = M4_TRUE;
			}
		}
	}
	return bReduce;
}


m4bool_t ClMetaSentence::_ReduceXJoinRelation( ClMetaRelatSet_t &aio_oRelationSet, const ClMetaRelat * ai_pStartRel, m4int32_t ai_iXJoinLeftAlias, m4int32_t ai_iXJoinRigthAlias, m4int32_t ai_iXJoinRigthField )
{
	if (!m_pvcRel)
	{
		return M4_FALSE;
	}
	
	for (itVcClMtRelat itRel1 = m_pvcRel->begin(); itRel1 != m_pvcRel->end(); itRel1++)
	{
		ClMetaRelat * pRel1 = (*itRel1); 
		ClMetaRelatSetIt_t itSet = aio_oRelationSet.find( pRel1 );
		//Si lo encuentra en el set o ya está marcada como reducida debo seguir buscando.
		if (itSet != aio_oRelationSet.end() || pRel1->m_bReduce == M4_TRUE)
		{
			continue;
		}

		//Escenario1 (en este caso se retornaríamos true):
		//Start:	0-1
		//r1:		2-0
		//r2:		2-3
		//r3:		3-1

		//Si el izquierdo es igual al izquierdo o derecho es que hemos encontrado una posible relacion puente (r1).
		m4int32_t iLeft = pRel1->m_pMtField1->m_indAlias;
		m4int32_t iRigth = pRel1->m_pMtField2->m_indAlias;
		m4int32_t iFieldLeft = pRel1->m_pMtField1->m_indRealField;
		m4int32_t iFieldRigth = pRel1->m_pMtField2->m_indRealField;

		if (iFieldLeft == M4LDB_NULL_INDEX || iFieldRigth == M4LDB_NULL_INDEX)
		{
			return M4_FALSE;
		}

		//Puede ser que tengamos 0-1, 2-0, 2-3, 3-1 le daremos la vuelta a la relacion 3-1 para conseguir una lista
		//0-1, 1-3, 3-2, 2-0
		//En este ejemplo, la única relacion que puede ser puente es la 3-1, pero le vamos a dar la vuelta.
		//Bug 0089904
		//Tambien hay que intercambiar si estando en el caso inicial coinciden left de original con rigth de relacion.
		if ((ai_iXJoinRigthAlias == iRigth) || ((ai_iXJoinRigthAlias == M4LDB_NULL_INDEX) && (ai_iXJoinLeftAlias == iRigth)) )
		{
			//Intercambiamos para tenerlo en el siguiente orden:
			//0-1, 0-2, 2-3, 3-1
			m4int32_t iTemp = iLeft;
			iLeft = iRigth;
			iRigth = iTemp;
			m4int32_t iTempField = iFieldLeft;
			iFieldLeft = iFieldRigth;
			iFieldRigth = iTempField;
		}

		//En el ejemplo hay un camino desde 0 hasta 1. Lo que vamos a tener en cuenta es:
		//Si 0.campo1 = 1.campo11, comprobamos que la relacion 2-0 es relacion puente, vamos a obligar que sea 
		//2.campo2 = 0.campo1 el campo join de 0 sea el mismo que el de la relacion de start.
		//Y por último, el campo de la relacion 3-1, 3.campo3 = 1.campo11, el campo de 11 sea el mismo que el
		//campo fin de la relacion start.
		//Ojo:Escenario:
		//0.c0=1.c1
		//1.c11=2.c2
		//0.c0=2.c2
		//NO se eliminaría la relacion 0-2 pq el campo de 1 c1 es diferente al c11, por lo tanto, no existe un camino.

		//Este es el caso inicial!!. Es decir, se pasa como argumento 0-?
		//Tendremos que comparar el 0 con izquierdo y con derecho de la relacion.
		//En el caso inicial forzamos a que el campo sea el mismo!.
		m4bool_t bMatchFirst = M4_FALSE;
		if ((ai_iXJoinRigthAlias == M4LDB_NULL_INDEX) && (ai_iXJoinRigthField == M4LDB_NULL_INDEX) )
		{
			if (((ai_iXJoinLeftAlias == iLeft) && (ai_pStartRel->m_pMtField1->m_indRealField == iFieldLeft))
				|| ((ai_iXJoinLeftAlias == iRigth) && (ai_pStartRel->m_pMtField1->m_indRealField == iFieldRigth)) )
			{
				bMatchFirst = M4_TRUE;
			}			
		}

		//Se supone que la relacion ya está en el orden 0-1, 1-3.		
		if (((ai_iXJoinRigthAlias == iLeft) && (ai_iXJoinRigthField == iFieldLeft)) || (bMatchFirst == M4_TRUE))
		{
			//Lo insertamos en el set!.
			aio_oRelationSet.insert( pRel1 );

			//Comentamos este codigo!:
			//Escenario: Esto controla el caso de que haya joins por campos inverosímiles, la busqueda en el diccionario
			//sera la que determinaria si la relacion entra a formar parte del camino.
			////----------------------------------------------
			////Tiene que cumplirse otra condicion!!.
			////Dado Origen (tabla, campo) y Destino (tabla, campo) comprobamos si existe una relacion en el diccionario
			////que se relacione Origen.c1 = Destino.c2. Si existe significa que la relación es candidata y continuaremos.
			////Si no existe sginifica que podemos desechar este camino!!.
			////Nos guardamos el current para restaurarlo!.
			//m4int32_t indLgcTblCurrent = m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.Index;			
			//if (_FindRelationInDictionary (pRel1) == M4_FALSE) {
			//	m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.MoveTo(indLgcTblCurrent);
			//	return M4_FALSE;
			//}
			//m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.MoveTo(indLgcTblCurrent);
			////----------------------------------------------

			//Veamos si he alcanzado el destino inicial, en este caso habremos encontrado el camino.
			//Forzamos a que el campo fin sea el mismo!.
			if (iRigth == ai_pStartRel->m_pMtField2->m_indAlias && iFieldRigth == ai_pStartRel->m_pMtField2->m_indRealField)
			{
				return M4_TRUE;
			}
			else
			{
				//Si estamos aqui es pq no he alcanzado el destino final pero estoy en una relacion puente,
				//debo continuar buscando.
				if (_ReduceXJoinRelation( aio_oRelationSet, ai_pStartRel, iLeft, iRigth, iFieldRigth ) == M4_TRUE)
				{
					//Hemos encontrado el camino, retornamos true!.
					return M4_TRUE;
				}
			}
		}
	}

	return M4_FALSE;
}


//Bug 0089387. 09/12/2003
m4bool_t ClMetaSentence::_AnalizeRelationsDuplicated (VcClMtRelat & ao_vRelations, m4int32_t ai_iAlias1r1, m4int32_t ai_iAlias2r1, m4int32_t ai_iAlias1r2, m4int32_t ai_iAlias2r2)
{
	//Escenario2:			//Escenario3:
		//0.id=1.id				//0.id=1.id
		//2.or=3.or (reduce)	//2.id=3.id (reduce)
		//2.id=3.id	(reduce)	//2.or=3.or (reduce)						

	//Escenario1:
		//0.id=1.id
		//0.or=1.or
		//2.id=3.id
	//El bucle r3 recorre las relaciones 0=1. El bucle r4 recorre las relaciones 2=3.
	//Dada una relacion 0=1 se busca su analoga 2=3. Ademas, para que al final se reduzca la relacion 2=3
	//debe cumplirse que todas las 0=1 se han reducido con alguna 2=3.
	//El bug se produce cuando el numero de relaciones 2=3 es mayor que el de 0=1 pq el bucle r5
	//eliminaría una relacion 2=3 que no tiene su analoga 0=1.
	//Para solucionarlo vamos a contar las relaciones 0=1 y 2=3 y solo se podrán reducir si hay el mismo numero de relaciones.

	m4bool_t bFoundRelationInVector2 = M4_FALSE;

	//Dados los 4 alias, buscaremos relaciones con esos alias.
	if (!m_pvcFrmAls || !m_pvcRel)
	{
		return M4_FALSE;
	}

	VcClMtRelat		vRelations1;
	VcClMtRelat		vRelations2;
	vRelations1.reserve(8);
	vRelations2.reserve(8);
	itVcClMtRelat itr3;
	itVcClMtRelat itr4;
	//Mejora: 0121700
	//Sara y Maria Luisa Gil han estudiado el tema y han llegado a la conclusión que para detectar duplicidad
	//solo se considere relaciones con organizacion si el canal NO es monosocietario.	
	VcClMtRelat		vRelations1Org;
	VcClMtRelat		vRelations2Org;


	for (itr3 = m_pvcRel->begin(); itr3 != m_pvcRel->end(); itr3++)
	{
		if ((( (*m_pvcFrmAls)[(*itr3)->m_pMtField1->m_indAlias]->m_bReduce) && 
			( (*m_pvcFrmAls)[(*itr3)->m_pMtField2->m_indAlias]->m_bReduce)) || 
			( (*m_pvcFrmAls)[(*itr3)->m_pMtField1->m_indAlias]->m_iLevel != (*m_pvcFrmAls)[(*itr3)->m_pMtField2->m_indAlias]->m_iLevel))
		{
			continue;
		}

		//Añadiremos en el vector1.
		if ( (((*itr3)->m_pMtField1->m_indAlias == ai_iAlias1r1) && ((*itr3)->m_pMtField2->m_indAlias == ai_iAlias2r1)) 
			|| (((*itr3)->m_pMtField1->m_indAlias == ai_iAlias2r1) && ((*itr3)->m_pMtField2->m_indAlias == ai_iAlias1r1)))
		{
				if (_GetIdInternalField ((*itr3)->m_pMtField1) == M4LDB_ID_ORGANIZATION &&
					_GetIdInternalField ((*itr3)->m_pMtField2) == M4LDB_ID_ORGANIZATION)
				{
					vRelations1Org.push_back(*itr3);
				}
				else
				{
					vRelations1.push_back(*itr3);
				}			
		}
		//Añadiremos en el vector2.
		if ( (((*itr3)->m_pMtField1->m_indAlias == ai_iAlias1r2) && ((*itr3)->m_pMtField2->m_indAlias == ai_iAlias2r2)) 
			|| (((*itr3)->m_pMtField1->m_indAlias == ai_iAlias2r2) && ((*itr3)->m_pMtField2->m_indAlias == ai_iAlias1r2)))
		{
				if (_GetIdInternalField ((*itr3)->m_pMtField1) == M4LDB_ID_ORGANIZATION &&
					_GetIdInternalField ((*itr3)->m_pMtField2) == M4LDB_ID_ORGANIZATION)
				{			
					vRelations2Org.push_back(*itr3);
				}
				else
				{
					vRelations2.push_back(*itr3);
				}			
		}
	}

	//Si el canal es multi lo dejamos como estaba.	
	ClChannel * pChannel = m_pGlobVars->GetpAccessRecordSet()->GetpAccess()->GetpChannel();

	m4double_t dOrganizationType;
	pChannel->OrganizationType.Get(dOrganizationType);
	m4int32_t iOrgType = (m4int32_t)dOrganizationType;
	m4bool_t bDoMonoOrg = M4_TRUE;

	//-------------------------------------------------------------------------------
	//Identificamos los escenarios para que se comporte como estaba o caso particular!.
	//Si es Multi o Independiente lo dejamos como estaba.
	//Por lo tanto solo debemos tratar como caso particular si es Mono.	
	if (iOrgType != M4CL_ORGANIZATION_TYPE_MONO)
	{		
		bDoMonoOrg = M4_FALSE;
	}
	//-------------------------------------------------------------------------------

	if (bDoMonoOrg == M4_FALSE)
	{
		//Hacemos que el tratamiento sea como estaba.
		vRelations1.insert(vRelations1.end(), vRelations1Org.begin(), vRelations1Org.end());
		vRelations2.insert(vRelations2.end(), vRelations2Org.begin(), vRelations2Org.end());
	}

	/* Bug 0127436
	La comprobación debe hacerse siempre, no sólo si es no monosocietario
	*/
	if (vRelations1.size() != vRelations2.size())
	{
		return M4_FALSE;
	}

	//Si estamos aqui es pq hay tantas relaciones con los alias1 que con los alias2.
	//Ahora, cada relacion de alias1 debe tener su análoga con alias2. y viceversa (se asume que si hay el mismo 
	//numero de relaciones y hay una con alias2 por cada una de alias1 tb tendremos una de alias1 por cada una de alias2.
	for (itr3 = vRelations1.begin(); itr3 != vRelations1.end (); itr3++)
	{
		PClMetaField pMtFieldt1r3 = NULL;
		PClMetaField pMtFieldt2r3 = NULL;
		
		if ((*itr3)->m_pMtField1->m_indAlias == ai_iAlias1r1 && 
			(*itr3)->m_pMtField2->m_indAlias == ai_iAlias2r1)
		{
			pMtFieldt1r3 = (*itr3)->m_pMtField1;
			pMtFieldt2r3 = (*itr3)->m_pMtField2;
		}
		else
		{
			if ((*itr3)->m_pMtField1->m_indAlias == ai_iAlias2r1 && 
				(*itr3)->m_pMtField2->m_indAlias == ai_iAlias1r1)
			{
				pMtFieldt2r3 = (*itr3)->m_pMtField1;
				pMtFieldt1r3 = (*itr3)->m_pMtField2;
			}
			else
			{
				continue;
			}
		}

		bFoundRelationInVector2 = M4_FALSE;
		for (itr4 = vRelations2.begin(); itr4 != vRelations2.end (); itr4++)
		{
			if ((*itr3)->m_pMtField1->m_iLevel != (*itr4)->m_pMtField1->m_iLevel || 
				(*itr3)->m_pMtField1->m_iFilter == (*itr4)->m_pMtField1->m_iFilter || 
				itr3 == itr4)
			{
				continue;
			}

			PClMetaField pMtFieldt1r4 = NULL;
			PClMetaField pMtFieldt2r4 = NULL;
			
			if ((*itr4)->m_pMtField1->m_indAlias == ai_iAlias1r2 && 
				(*itr4)->m_pMtField2->m_indAlias == ai_iAlias2r2)
			{
				pMtFieldt1r4 = (*itr4)->m_pMtField1;
				pMtFieldt2r4 = (*itr4)->m_pMtField2;
			}
			else
			{
				if ((*itr4)->m_pMtField1->m_indAlias == ai_iAlias2r2 && 
					(*itr4)->m_pMtField2->m_indAlias == ai_iAlias1r2)
				{
					pMtFieldt2r4 = (*itr4)->m_pMtField1;
					pMtFieldt1r4 = (*itr4)->m_pMtField2;
				}
				else
				{
					continue;
				}
			}

			if (pMtFieldt1r3->m_indRealField == pMtFieldt1r4->m_indRealField && 
				pMtFieldt2r3->m_indRealField == pMtFieldt2r4->m_indRealField && 
				pMtFieldt1r3->m_indRealTable == pMtFieldt1r4->m_indRealTable && 
				pMtFieldt2r3->m_indRealTable == pMtFieldt2r4->m_indRealTable) 
			{
					//Hemos encontrado la relación repetida.
					bFoundRelationInVector2 = M4_TRUE;
					ao_vRelations.push_back((*itr4));
					ao_vRelations.push_back((*itr3));

					//Si es Mono es cuando tiene un tratamiento especial.
					if (bDoMonoOrg == M4_TRUE)
					{					
						//Cada iterador va asociado con un vector de ORG.
						itVcClMtRelat itOrg1;
						for (itOrg1 = vRelations1Org.begin(); itOrg1 != vRelations1Org.end (); itOrg1++)
						{
							ClMetaRelat * pRel = (*itOrg1);
							if (((pMtFieldt1r3->m_indAlias == pRel->m_pMtField1->m_indAlias) && (pMtFieldt2r3->m_indAlias == pRel->m_pMtField2->m_indAlias))
								|| ((pMtFieldt1r3->m_indAlias == pRel->m_pMtField2->m_indAlias) && (pMtFieldt2r3->m_indAlias == pRel->m_pMtField1->m_indAlias)) )
							{
								ao_vRelations.push_back((*itOrg1));
								vRelations1Org.erase(itOrg1);
								break;
							}
						}
						itVcClMtRelat itOrg2;
						for (itOrg2 = vRelations2Org.begin(); itOrg2 != vRelations2Org.end (); itOrg2++)
						{
							ClMetaRelat * pRel = (*itOrg2);
							if (((pMtFieldt1r4->m_indAlias == pRel->m_pMtField1->m_indAlias) && (pMtFieldt2r4->m_indAlias == pRel->m_pMtField2->m_indAlias))
								|| ((pMtFieldt1r4->m_indAlias == pRel->m_pMtField2->m_indAlias) && (pMtFieldt2r4->m_indAlias == pRel->m_pMtField1->m_indAlias)) )
							{
								ao_vRelations.push_back((*itOrg2));
								vRelations2Org.erase(itOrg2);
								break;
							}
						}
					}
					break;
			}
		}

		//Si hemos terminado de recorrer el vector2 y no se ha encontrado su analogo... no tendré que 
		//reducir nada. Limpiamos el vector y nos vamos indicando que no se puede reducir.
		if (bFoundRelationInVector2 == M4_FALSE)
		{
			ao_vRelations.erase(ao_vRelations.begin(),ao_vRelations.end());			
			return M4_FALSE;
		}
	}

	//Devolvemos lo que diga la variable (es true si todas las relaciones han encontrado su analoga en vector2).
	//Si hay una sola que no se encuentra se retorna falso.
	return bFoundRelationInVector2;
}


m4return_t ClMetaSentence::_CheckOrgRelations	(PVcClMtRelat ai_vRel)
{
	// Miramos a ver que relaciones se pueden quitar por el tema de multisocoedad.
	// Segun el tipo de tablas :
	//		- Mono - Mono : es redundante, pero lo dejamos
	//		- Mono - Multi: hay que quitarlo.
	//		- Multi- Multi: hay que dejarlo, pero sólo si son del mismo nivel.

	ClMetaObject *pObj1=0,*pObj2=0;
	ClMetaRelat *pRel= 0;
	
	if (ai_vRel==0)
	{
		return M4_SUCCESS;
	}
	if (ai_vRel->empty())
	{
		return M4_SUCCESS;
	}

	// Cuando lleguemos aquí tienen que haber sido traspasados todos los objetos lógicos al vector correspondiente.
	
	itVcClMtRelat itRelbegin= ai_vRel->begin();
	itVcClMtRelat itRel= ai_vRel->end();
	// Iteramos de atras a adelante porque podemos ir borrando elementos.
	do
	{
		itRel--;
		pRel = (*itRel);
		// Miramos si los objetos son de distinto tipo.
		pObj1 = (*m_pvcFrmObj)[GetObjectPosByAliasPos(pRel->m_pMtField1->m_indAlias)];
		pObj2 = (*m_pvcFrmObj)[GetObjectPosByAliasPos(pRel->m_pMtField2->m_indAlias)];

		if (pObj2->m_iOrgType == pObj1->m_iOrgType)
		{
			/* Bug 0125485
			Si la relación es mono-mono se elimina si es de seguridad
			*/
			/* Bug 0131027
			Se eliminan siempre en las relaciones mono-mono
			*/
			if( pObj1->m_iOrgType == M4CL_ORGANIZATION_TYPE_NONE )
			{
				// No se elimina en las none-none
				continue;
			}

			if( pObj1->m_iOrgType == M4CL_ORGANIZATION_TYPE_MULTI )
			{
				if( pObj2->m_iOrgLevel == pObj1->m_iOrgLevel )
				{
					// No se eliminan en las multi-multi del mismo nivel
					continue;
				}
			}
		}

		// Miramos los internal type de los campos.
		if (_GetIdInternalField	(pRel->m_pMtField1)!=M4LDB_ID_ORGANIZATION )
		{
			continue;
		}

		if (_GetIdInternalField	(pRel->m_pMtField2)!=M4LDB_ID_ORGANIZATION )
		{
			continue;
		}

		// Tenemos que borrar la relacion.
		delete pRel;
		ai_vRel->erase(itRel);
	}
	while (itRel!=itRelbegin);

	return M4_SUCCESS;
}


m4int32_t ClMetaSentence::_GetIdInternalField(ClMetaField *ai_pField)
{
	if (ai_pField==0)
	{
		return M4LDB_NULL_INDEX;
	}
	// Nos posicionamos en Logic Objects.
	m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_LOGIC_OBJECTS]->Current.MoveTo(ai_pField->m_indLogicTable);
	m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_REAL_OBJECTS]->Current.MoveTo(ai_pField->m_indRealTable);
	

	return M4LDB_NUMBER(m_pGlobVars->GetpLConn()->m_paRS[M4LDB_SYS_DICT_FIELDS]->Current[ai_pField->m_indRealField].Item[M4LDB_ITEM_SYS_DICT_FIELDS_ID_INTERNAL_FIELD].Value.Get());
}


m4bool_t ClMetaSentence::_CheckRelationsToReduce(m4int32_t ai_t1r1,m4int32_t ai_t2r1,m4int32_t ai_t1r2,m4int32_t ai_t2r2)
{
	ClMetaAlias * pAlias1r1  = (*m_pvcFrmAls)[ai_t1r1];
	ClMetaAlias * pAlias2r1  = (*m_pvcFrmAls)[ai_t2r1];
	ClMetaAlias * pAlias1r2  = (*m_pvcFrmAls)[ai_t1r2];
	ClMetaAlias * pAlias2r2  = (*m_pvcFrmAls)[ai_t2r2];

	// Chequeamos que las relaciones se pueden reducir.
	if (pAlias1r1->m_indNewAlias == ai_t1r2)
	{
		// En seguridad se puede dar que un NewAlias apunte a sí mismo
		// Compruebo que sea por seguridad ( el Alias es el mismo ), si no no se puede reducir.
		if (pAlias1r1!=pAlias1r2)
		{
			// Nos meteríamos en una relación recursiva, ya que tendriamos un alias reducido a otro y viceversa.
			return M4_FALSE;
		}
	}

	if (pAlias2r1->m_indNewAlias == ai_t2r2)
	{
		// En seguridad se puede dar que un NewAlias apunte a sí mismo
		// Compruebo que sea por seguridad ( el Alias es el mismo ), si no no se puede reducir.
		if (pAlias2r1!=pAlias2r2)
		{
			// Nos meteríamos en una relación recursiva, ya que tendriamos un alias reducido a otro y viceversa.
			return M4_FALSE;
		}
	}
	
	if (pAlias1r1->m_iFilter != pAlias1r2->m_iFilter && 
		pAlias2r1->m_iFilter != pAlias2r2->m_iFilter)
	{
		// No hay problemas de seguridad, y lo que reducimos son relaciones de filtros distintos.
		return M4_TRUE; 
	}

	if (pAlias1r1->m_iFilter == pAlias1r2->m_iFilter)
	{
		// Podemos estar en una relacion de seguridad. ( Uno de los alias es comun ).
		// Si fuera esto, los alias tienen que ser iguales.
		if (pAlias1r1==pAlias1r2)
		{
			return M4_TRUE;
		}
		else
		{
			return M4_FALSE;
		}
	}

	if (pAlias2r1->m_iFilter == pAlias2r2->m_iFilter)
	{
		// Podemos estar en una relacion de seguridad. ( Uno de los alias es comun ).
		// Si fuera esto, los alias tienen que ser iguales.
		if (pAlias2r1==pAlias2r2)
		{
			return M4_TRUE;
		}
		else
		{
			return M4_FALSE;
		}
	}

	return M4_FALSE;
}


m4return_t ClMetaSentence::_CleanOR (void)
{
	if (m_bHasOr)
	{
		// Limpiamos las marcas del tratamiento de los OR.
		m4bool_t bExistsOR = M4_FALSE;
		m4int32_t iPos = M4_STR_NPOS;
		do
		{
			iPos = m_pstStatemWhr->find ("~X",iPos+1);
			
			if (iPos != M4_STR_NPOS)
			{
				m_pstStatemWhr->replace (iPos, 2, "");
				bExistsOR = M4_TRUE;
			}
		}
		while (iPos != M4_STR_NPOS);

		if (bExistsOR)
		{
			if (m_pvcParamWhr)
			{
				m4int32_t nPar = m_pvcParamWhr->size ();
				for (m4int32_t iPar = nPar-1; iPar>=0; iPar--)
				{
					itVcClMtParam itPar = m_pvcParamWhr->begin () + iPar;
					if ((*itPar)->IsAddToOr () == M4_TRUE)
					{
						delete (*itPar);
						m_pvcParamWhr->erase (itPar);
					}
				}
			}
			if (m_pvcFld)
			{
				m4int32_t nFld = m_pvcFld->size ();
				for (m4int32_t iFld = nFld-1; iFld>=0; iFld--)
				{
					itVcClMtField itFld = m_pvcFld->begin () + iFld;
					if ((*itFld)->IsAddToOr () == M4_TRUE)
					{
						delete (*itFld);
						m_pvcFld->erase (itFld);
					}
				}
			}
		}
	}

	return (M4_SUCCESS);
}


m4return_t ClMetaSentence::_AddInfoPartFilterAdd( PVcClMtParam ai_vParamWhr,PVcClMtField ai_vFieldWhr,string &ai_stFilter,eClauseType_t ai_eClause,m4int32_t ai_Level,m4int32_t &ao_lPos, m4int32_t ai_lNextClausePos, m4bool_t ai_bProcessOrs, m4bool_t &ao_bOut)
{
	if (ai_stFilter[ao_lPos] == '~' && ai_stFilter[ao_lPos+1] == 'A')
	{
		m4int32_t lPosAls = ao_lPos+3;

		ao_lPos = ai_stFilter.find (' ', lPosAls);

		//Pedro. Bug 0068102, 0093792.
		//Si no se encuentra el espacio (-1), devolveremos ai_lNextClausePos.
		//De esta forma, en la insercion en el string no se insertara nada.
		//La posición del blanco tiene que estar antes del token!.
		//Si nos pasamos del token, siempre que haya token, estableceremos la posicion al token encontrado.
		if ((ao_lPos == M4_STR_NPOS) || ((ao_lPos > ai_lNextClausePos) && (ai_lNextClausePos != M4_STR_NPOS)) )
		{
			ao_lPos = ai_lNextClausePos;
		}
		//fin Pedro
		
		/* Bug 0124976
		Hay casos en los que no se quieren procesar los ors para añadir el filtro
		De momento no se tienen en cuenta para auto filtros y sociedad
		*/
		if ( m_bHasOr && ai_bProcessOrs && m_pvcFrmObj->size () > 1)
		{
			m4int32_t iAls = atol (&ai_stFilter[lPosAls]);

			// Primero buscamos si existe algún campo con este alias fuera del ambito del OR.
			m4int32_t iOR = 0;
			ao_bOut = M4_FALSE;
			for (itVcClMtField itFld1 = m_pvcFld->begin (); (itFld1!=m_pvcFld->end ())&&(!ao_bOut); itFld1++)
			{
				if ((*itFld1)->m_iLevel == ai_Level)
				{
					if ((*itFld1)->IsAddToOr () == M4_TRUE)
					{
						if ((*itFld1)->IsBegin () == M4_TRUE)
						{
							iOR++;
						}
						else
						{
							iOR--;
						}
					}
					else
					{
						if (!iOR && (*itFld1)->m_indAlias == iAls)
						{
							ao_bOut = M4_TRUE;
						}
					}
				}
			}

			if (!ao_bOut)
			{
				// Ahora buscamos para ver en que OR tenemos que meter el filtro.
				m4bool_t bFound = M4_FALSE;

				m4int32_t nFld = m_pvcFld->size ();

				for (m4int32_t iFldAux0 = 0; iFldAux0<nFld; iFldAux0++)
				{

					itVcClMtField itFldAux0 = m_pvcFld->begin () + iFldAux0;

					if ((*itFldAux0)->IsAddToOr () == M4_TRUE && 
						(*itFldAux0)->IsBegin () == M4_TRUE && 
						(*itFldAux0)->m_iLevel == ai_Level)
					{
						// Buscamos por los ORs de nuestro nivel.

						m4bool_t bAdd = M4_FALSE;
						m4int32_t iAuxOR = 1;
						m4int32_t iFldAux1 = iFldAux0 + 1;

						for (; (iFldAux1<nFld) && (iAuxOR); iFldAux1++)
						{
							// Buscamos si existe el campo dentro del límite del OR.

							itVcClMtField itFldAux1 = m_pvcFld->begin () + iFldAux1;

							if ((*itFldAux1)->IsAddToOr () == M4_TRUE)
							{
								if ((*itFldAux1)->IsBegin () == M4_TRUE)
								{
									iAuxOR++;
								}
								else
								{
									iAuxOR--;
								}
							}
							else
							{
								if (iAuxOR == 1 && 
									(*itFldAux1)->m_indAlias == iAls && 
									(*itFldAux1)->m_iLevel == ai_Level)
								{
									// Campo encontrado dentro del OR actual.
									bAdd = M4_TRUE;
								}
							}
						}

						if (bAdd)
						{ 
							bFound = M4_TRUE;
							m4int32_t iAcumOR = 1;
							m4int32_t iFldAux2 = iFldAux1 - 2;

							for (; iFldAux2; iFldAux2--)
							{
								// Buscamos el OR al que añadir el filtro.

								itVcClMtField itFldAux2 = m_pvcFld->begin () + iFldAux2;

								if ((*itFldAux2)->IsAddToOr () == M4_TRUE && 
									(*itFldAux2)->IsBegin () == M4_FALSE)
								{
									iAcumOR++;
								}
							}

							m4int32_t nOR = 0;
							m4int32_t iPos = 0;

							// Sustituimos en la cadena.
							while (iPos != M4_STR_NPOS)
							{
								iPos = m_pstStatemWhr->find ("~X",iPos+1);
								
								if (iPos != M4_STR_NPOS)
								{
									nOR++;
									if (iAcumOR==nOR)
									{
										m_pstStatemWhr->insert(iPos,ai_stFilter,ao_lPos,ai_lNextClausePos-ao_lPos);
										break;
									}
								}
							}

							if (iPos != M4_STR_NPOS)
							{
								// Tenemos que insertar Fld y Param.
								_VectorInserter<ClMetaField> oInsertFld;
								if (oInsertFld(ai_vFieldWhr,m_pvcFld,ai_eClause,ai_Level,iAcumOR)!=M4_SUCCESS)
								{
									return M4_ERROR;
								}
								_VectorInserter<ClMetaParam> oInsertPar;
								if (oInsertPar(ai_vParamWhr,m_pvcParamWhr,ai_eClause,ai_Level,iAcumOR)!=M4_SUCCESS)
								{
									return M4_ERROR;
								}

								nFld = m_pvcFld->size ();
							}
						}
					}
				}

				if (!bFound)
				{
					// Dentro tampoco está. Lo dejamos fuera.
					ao_bOut = M4_TRUE;
				}
			}
		}
	}
	
	if (!ao_bOut)
	{
		// Tenemos que borrar Fld y Param.
		_VectorClearLevel<ClMetaField> oClearFld;
		if (oClearFld(ai_vFieldWhr,ai_eClause,ai_Level)!=M4_SUCCESS)
		{
			return M4_ERROR;
		}
		_VectorClearLevel<ClMetaParam> oClearPar;
		if (oClearPar(ai_vParamWhr,ai_eClause,ai_Level)!=M4_SUCCESS)
		{
			return M4_ERROR;
		}
	}

	if (1==2)
	{
		// Esto es para depurar con el Set Next Statement.
#ifdef _M4LDB_DEBUG
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\tWhere = ---%s---", m_pstStatemWhr->c_str ());
		Dump_Vector_Fld (ai_vFieldWhr);
#endif
	}

	return M4_SUCCESS;
}



m4return_t ClMetaSentence::_ReplaceRelationAdd( ClMetaRelat *ai_pRel, m4bool_t &ao_bOut)
{
	if (m_bHasOr && m_pvcFrmObj->size () > 1)
	{
		if (ai_pRel->m_pMtField1->m_iLevel != ai_pRel->m_pMtField2->m_iLevel)
		{
			return (M4_SUCCESS);
		}

		m4int32_t lLevel = ai_pRel->m_pMtField1->m_iLevel;

		m4int32_t iOR = 0;
		m4bool_t bFound1 = M4_FALSE;
		m4bool_t bFound2 = M4_FALSE;
		ao_bOut = M4_FALSE;
		for (itVcClMtField itFld1 = m_pvcFld->begin (); (itFld1!=m_pvcFld->end ())&&(!ao_bOut); itFld1++)
		{
			if ((*itFld1)->m_iLevel == lLevel)
			{
				if ((*itFld1)->IsAddToOr () == M4_TRUE)
				{
					if ((*itFld1)->IsBegin () == M4_TRUE)
					{
						iOR++;
					}
					else
					{
						iOR--;
					}
				}
				else
				{
					if (!iOR)
					{
						if ((*itFld1)->m_indAlias == ai_pRel->m_pMtField1->m_indAlias)
						{
							bFound1 = M4_TRUE;
						}
						if ((*itFld1)->m_indAlias == ai_pRel->m_pMtField2->m_indAlias)
						{
							bFound2 = M4_TRUE;
						}
						if (bFound1 && bFound2)
						{
							ao_bOut = M4_TRUE;
						}
					}
				}
			}
		}

		if (!ao_bOut)
		{
			// Ahora buscamos para ver en que OR tenemos que meter el filtro.
			m4bool_t bFound = M4_FALSE;

			m4int32_t nFld = m_pvcFld->size ();

			for (m4int32_t iFldAux0 = 0; iFldAux0<nFld; iFldAux0++)
			{
				itVcClMtField itFldAux0 = m_pvcFld->begin () + iFldAux0;

				if ((*itFldAux0)->IsAddToOr () == M4_TRUE && 
					(*itFldAux0)->IsBegin () == M4_TRUE && 
					(*itFldAux0)->m_iLevel == lLevel)
				{
					// Buscamos por los ORs de nuestro nivel.

					bFound1 = M4_FALSE;
					bFound2 = M4_FALSE;
					m4bool_t bAdd = M4_FALSE;
					m4int32_t iAuxOR = 1;
					m4int32_t iFldAux1 = iFldAux0 + 1;

					for (; (iFldAux1<nFld) && (iAuxOR); iFldAux1++)
					{
						// Buscamos si existe el campo dentro del límite del OR.

						itVcClMtField itFldAux1 = m_pvcFld->begin () + iFldAux1;

						if ((*itFldAux1)->IsAddToOr () == M4_TRUE)
						{
							if ((*itFldAux1)->IsBegin () == M4_TRUE)
							{
								iAuxOR++;
							}
							else
							{
								iAuxOR--;
							}
						}
						else
						{
							if (iAuxOR == 1 && (*itFldAux1)->m_iLevel == lLevel)
							{
								if ((*itFldAux1)->m_indAlias == ai_pRel->m_pMtField1->m_indAlias)
								{
									bFound1 = M4_TRUE;
								}
								if ((*itFldAux1)->m_indAlias == ai_pRel->m_pMtField2->m_indAlias)
								{
									bFound2 = M4_TRUE;
								}
								if (bFound1 && bFound2)
								{
									bAdd = M4_TRUE;
								}
							}
						}
					}

					if (bAdd)
					{ 
						bFound = M4_TRUE;
						m4int32_t iAcumOR = 1;
						m4int32_t iFldAux2 = iFldAux1 - 2;

						for (; iFldAux2; iFldAux2--)
						{
							// Buscamos el OR al que añadir el filtro.

							itVcClMtField itFldAux2 = m_pvcFld->begin () + iFldAux2;

							if ((*itFldAux2)->IsAddToOr () == M4_TRUE && (*itFldAux2)->IsBegin () == M4_FALSE)
							{
								iAcumOR++;
							}
						}

						m4int32_t nOR = 0;
						m4int32_t iPos = 0;

						// Sustituimos en la cadena.
						while (iPos != M4_STR_NPOS)
						{
							iPos = m_pstStatemWhr->find ("~X",iPos+1);
							
							if (iPos != M4_STR_NPOS)
							{
								nOR++;
								if (iAcumOR==nOR)
								{
									m_pstStatemWhr->insert(iPos," AND ~F = ~F");
									break;
								}
							}
						}

						if (iPos != M4_STR_NPOS)
						{
							// Tenemos que insertar Fld,Rel y Param.
							_VectorInserter<ClMetaField> oInsertFld;
							if (oInsertFld(ai_pRel->m_pMtField1,m_pvcFld,lLevel,iAcumOR)!=M4_SUCCESS)
							{
								return M4_ERROR;
							}

							if (oInsertFld(ai_pRel->m_pMtField2,m_pvcFld,lLevel,iAcumOR)!=M4_SUCCESS)
							{
								return M4_ERROR;
							}

							nFld = m_pvcFld->size ();
						}
					}
				}
			}

			if (!bFound)
			{
				// Dentro tampoco está. Lo dejamos fuera.
				ao_bOut = M4_TRUE;
			}
		}
	}

	
	if (!ao_bOut)
	{
		delete ai_pRel;
	}

	return M4_SUCCESS;
}



void ClMetaSentence::Dump_Vector_Fld (PVcClMtField ai_vFieldWhr)
{
#ifdef _M4LDB_DEBUG
	m4int32_t nFld = m_pvcFld->size ();
	for (m4int32_t iFldAux = 0; iFldAux<nFld; iFldAux++)
	{
		itVcClMtField itFldAux = m_pvcFld->begin () + iFldAux;
		M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "\n\tInd[%ld] -> Level[%ld], Clause[%s], ", iFldAux, (*itFldAux)->m_iLevel, ClauseType((*itFldAux)->m_eClause));
		if ((*itFldAux)->IsAddToOr () == M4_TRUE)
		{
			if ((*itFldAux)->IsBegin () == M4_TRUE)
			{
				M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "Begin Or");
			}
			else
			{
				M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "End Or");
			}
		}
		else
		{
			if ((*itFldAux)->IsSeparator ())
			{
				M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "Separator");
			}
			else
			{
				M4LDB_PRINT_DEBUG (m_pGlobVars->GetpLConn (), "Field[%s]", (*itFldAux)->m_stField.c_str ());
			}
		}
	}
#endif
}


ClMetaField *ClMetaSentence::GetColumnByIdField (m4id_t ai_idField)
{
	// Esta función es sólo para el insert into select (así evitamos problemas).
	if (GetIsInsertIntoSelect() == M4_TRUE)
	{
		m4int32_t numFld = m_pvcFld->size ();

		for (m4int32_t indFld=0; indFld<numFld; indFld++)
		{
			ClMetaField	*pFld = (*m_pvcFld)[indFld];
			
			if (pFld->IsMetaData ()== M4_TRUE && 
				pFld->m_eClause == M4LDB_CLAUSE_COLUMN && 
				!strcmpi (pFld->m_stField.c_str (), ai_idField))
			{
				return (pFld);
			}
		}
	}
	return (NULL);
}


m4int32_t ClMetaSentence::GetNumSysHint( void )
{
	if (m_pVcSysHint != NULL)
	{
		return m_pVcSysHint->size();
	}
	
	return 0;
}

ClSysHint * ClMetaSentence::GetSysHintByPos( m4int32_t ai_iPosition )
{
	if (m_pVcSysHint != NULL)
	{
		if (ai_iPosition >= 0 && ai_iPosition < GetNumSysHint())
		{
			return (*m_pVcSysHint)[ai_iPosition];
		}
	}
	return NULL;
}


ClSysHint *	ClMetaSentence::GetSysHintByPk( const ClSysHintPk & ai_oClSysHintPk )
{
	for (itVcSysHint it=m_pVcSysHint->begin(); it!=m_pVcSysHint->end(); it++)
	{
		ClSysHint * pClSysHint = (*it);
		if (pClSysHint != NULL)
		{
			if (pClSysHint->IsEqualByPk(ai_oClSysHintPk) == M4_TRUE)
			{
				return pClSysHint;
			}
		}
	}
	return NULL;
}


m4return_t ClMetaSentence::_AddSysHint( ClSysHint * ai_pClSysHint )
{
	if (m_pVcSysHint == NULL)
	{
		m_pVcSysHint = new VcSysHint;
	}

	//Lo que tambien hacemos es validar el sys hint que queremos añadir.
	if (_ValidateSysHint(ai_pClSysHint) == M4_ERROR)
	{
		return M4_ERROR;
	}

	m_pVcSysHint->push_back(ai_pClSysHint);
	return M4_SUCCESS;
}


m4return_t ClMetaSentence::AddListOfSysHint( const VcSysHint & ai_VcSysHint )
{
	for (itConstVcSysHint it = ai_VcSysHint.begin(); it != ai_VcSysHint.end(); it++)
	{
		ClSysHint * pClSysHint = (*it);
		if (pClSysHint != NULL)
		{
			if (_AddSysHint( pClSysHint ) != M4_SUCCESS)
			{
				return M4_ERROR;				
			}
		}
	}

	return M4_SUCCESS;
}


m4bool_t ClMetaSentence::IsSameSysHints( const VcSysHint & ai_VcSysHint )
{
	//Compararemos los que sean los mismos hints.

	if (m_pVcSysHint == NULL)
	{
		//No había hints.
		if (ai_VcSysHint.size() > 0)
		{
			return M4_FALSE;
		}
		return M4_TRUE;
	}

	if (m_pVcSysHint->size() != ai_VcSysHint.size())
	{
		return M4_FALSE;
	}

	for (itConstVcSysHint it = m_pVcSysHint->begin(); it != m_pVcSysHint->end(); it++)
	{
		ClSysHint * pClSysHint = (*it);
		if (pClSysHint != NULL)
		{
			//Dado un Id buscamos el mismo Id y si el texto es el mismo.
			for (itConstVcSysHint itAux = ai_VcSysHint.begin(); itAux != ai_VcSysHint.end(); itAux++)
			{
				ClSysHint * pClSysHintAux = (*itAux);
				if (pClSysHintAux != NULL)
				{
					if (pClSysHint->IsEqualByPk(pClSysHintAux) == M4_TRUE)
					{
						//Encontrado el Syshint. Veamos si son iguales.
						if (pClSysHint->GetText() == pClSysHintAux->GetText())
						{
							//Mismo texto, por lo tanto no ha cambiado.							
							break;
						}
						else
						{
							//Diferente texto, por lo tanto lo han cambiado.
							return M4_FALSE;
						}
					}
				}
			}			
		}
	}

	return M4_TRUE;
}


m4int32_t ClMetaSentence::GetNumDynamicHint( void )
{
	if (m_pVcDynamicHint != NULL)
	{
		return m_pVcDynamicHint->size();
	}
	
	return 0;
}


ClDynamicHint * ClMetaSentence::GetDynamicHintByPos( m4int32_t ai_iPosition )
{
	if (m_pVcDynamicHint != NULL)
	{
		if (ai_iPosition >= 0 && ai_iPosition < GetNumDynamicHint())
		{
			return (*m_pVcDynamicHint)[ai_iPosition];
		}
	}
	return NULL;
}


m4return_t ClMetaSentence::AddDynamicHint( m4int32_t ai_iFilter, m4pcchar_t ai_pccId )
{
	if (m_pVcDynamicHint == NULL)
	{
		m_pVcDynamicHint = new VcDynamicHint;
	}

	m_pVcDynamicHint->push_back( new ClDynamicHint( ai_iFilter, ai_pccId ) );
	return M4_SUCCESS;
}


m4return_t ClMetaSentence::_ValidateSysHint( ClSysHint * ai_pClSysHint )
{
	if (ai_pClSysHint == NULL)
	{		
		return M4_ERROR;
	}

	//------------------------------------------------------------------------
	//Buscamos si ya existe un syshint repetido.
	//Si es automatico no se hace esta comprobacion.
	if (ai_pClSysHint->GetIsAutomatic() == M4_FALSE)
	{
		for (itVcSysHint it=m_pVcSysHint->begin(); it!=m_pVcSysHint->end(); it++)
		{
			ClSysHint * pClSysHint = (*it);
			if (pClSysHint != NULL)
			{			
				if (pClSysHint->IsEqualByPk(ai_pClSysHint) == M4_TRUE)
				{
					string sComposedSysHintPk("");
					ClSysHintPk * pClSysHintPk = pClSysHint->GetClSysHintPk();					
					if (pClSysHintPk != NULL)
					{
						sComposedSysHintPk = pClSysHintPk->GetComposedSysHintPk();
					}
					DUMP_CHLOG_ERRORF ( M4_CH_LDB_SYSHINT_IDHINT_DUPLICATED, sComposedSysHintPk.c_str() );
					return M4_ERROR;
				}
			}
		}
	}
	//------------------------------------------------------------------------

	//------------------------------------------------------------------------
	//Veamos que la BD y el tipo deben ser correctos.
	if (_ValidateSysHintDBType(ai_pClSysHint) == M4_ERROR)
	{
		return M4_ERROR;
	}
	
	if (_ValidateSysHintType(ai_pClSysHint) == M4_ERROR)
	{
		return M4_ERROR;
	}
	//------------------------------------------------------------------------

	return M4_SUCCESS;
}

m4return_t ClMetaSentence::_ValidateSysHintDBType( ClSysHint * ai_pClSysHint )
{
	if (ai_pClSysHint == NULL)
	{
		return M4_ERROR;
	}

	ClSysHintPk * pClSysHintPk = ai_pClSysHint->GetClSysHintPk();
	if (pClSysHintPk == NULL)
	{
		return M4_ERROR;
	}

	//De momento solo se implementa oracle.
	const string sSysHintDBType = pClSysHintPk->GetSysHintDBType();
	if (sSysHintDBType.compare(M4LDB_SYSHINT_DB_ORACLE) != 0)
	{
		DUMP_CHLOG_ERRORF ( M4_CH_LDB_SYSHINT_WRONG_DB, sSysHintDBType.c_str() );
		return M4_ERROR;
	}

	return M4_SUCCESS;
}

m4return_t ClMetaSentence::_ValidateSysHintType( ClSysHint * ai_pClSysHint )
{
	if (ai_pClSysHint == NULL)
	{
		return M4_ERROR;
	}

	const string sSysHintType = ai_pClSysHint->GetSysHintType();
	
	//Si el hint es automático necesitamos un tipo y que sea valido.
	if (ai_pClSysHint->GetIsAutomatic() == M4_TRUE)
	{		
		if (sSysHintType.size() == 0)
		{
			DUMP_CHLOG_ERRORF ( M4_CH_LDB_SYSHINT_WRONG_TYPE, "" );		
			return M4_ERROR;
		}
	}
	else
	{
		//Si no es automatico, no necesitamos tipo pero si lo hay tiene que ser valido.
		if (sSysHintType.size() == 0)
		{
			return M4_SUCCESS;
		}
	}

	//Aqui comprobamos que sea válido. De momento solo se implementa Select.	
	if (sSysHintType.compare(M4LDB_SYSHINT_TYPE_SELECT) != 0)
	{
		DUMP_CHLOG_ERRORF ( M4_CH_LDB_SYSHINT_WRONG_TYPE, sSysHintType.c_str() );		
		return M4_ERROR;
	}

	return M4_SUCCESS;
}


//---------- HintsPK ---------------------------------
ClSysHintPk::ClSysHintPk( void )
	: m_sSysHintDBType(""), m_sSysHintId("")
{
}


ClSysHintPk::ClSysHintPk( const string & ai_sSysHintDBType, const string & ai_sSysHintId )
	: m_sSysHintDBType(ai_sSysHintDBType), m_sSysHintId(ai_sSysHintId)
{}


ClSysHintPk::~ClSysHintPk( )
{}


ClSysHintPk::ClSysHintPk( const ClSysHintPk & ai_oCopy )
{
	m_sSysHintDBType = ai_oCopy.m_sSysHintDBType;
	m_sSysHintId = ai_oCopy.m_sSysHintId;	
}


m4bool_t ClSysHintPk::IsEqualByPk ( const ClSysHintPk & ai_oClSysHintPk ) const
{
	if (m_sSysHintDBType.compare(ai_oClSysHintPk.GetSysHintDBType()) == 0
		&& m_sSysHintId.compare(ai_oClSysHintPk.GetSysHintId()) == 0) 
	{
		return M4_TRUE;
	}
	return M4_FALSE;
}


m4bool_t ClSysHintPk::IsEqualByDDBB( eDDBBType_t ai_eDDBBType ) const
{
	eDDBBType_t eDDBBType;
	if (GetSysHintDBTypeByDDBB( eDDBBType ) == M4_ERROR)
	{
		return M4_FALSE;
	}

	if (eDDBBType != ai_eDDBBType)
	{
		return M4_FALSE;
	}

	return M4_TRUE;
}


const string & ClSysHintPk::GetSysHintDBType( void ) const
{
	return m_sSysHintDBType;
}


m4return_t ClSysHintPk::GetSysHintDBTypeByDDBB( eDDBBType_t & ao_eDDBBType ) const
{
	if (m_sSysHintDBType.compare(M4LDB_SYSHINT_DB_ORACLE) == 0)
	{
		ao_eDDBBType = M4DBW_ORACLE_DDBB;
		return M4_SUCCESS;
	}
	if (m_sSysHintDBType.compare(M4LDB_SYSHINT_DB_SQLSERVER) == 0)
	{
		ao_eDDBBType = M4DBW_SQL_SERVER_DDBB;
		return M4_SUCCESS;
	}
	if (m_sSysHintDBType.compare(M4LDB_SYSHINT_DB_INFORMIX) == 0)
	{
		ao_eDDBBType = M4DBW_INFORMIX_DDBB;
		return M4_SUCCESS;
	}
	if (m_sSysHintDBType.compare(M4LDB_SYSHINT_DB_DB2) == 0)
	{
		ao_eDDBBType = M4DBW_DB2_DDBB;
		return M4_SUCCESS;
	}	
	return M4_ERROR;
}


const string & ClSysHintPk::GetSysHintId( void ) const
{
	return m_sSysHintId;
}


string ClSysHintPk::GetComposedSysHintPk( void ) const
{
	string sReturn("");
	sReturn.append("(");
	sReturn.append(m_sSysHintDBType);
	sReturn.append(", ");
	sReturn.append(m_sSysHintId);
	sReturn.append(")");
	return sReturn;
}


//---------- Hints ---------------------------------
ClSysHint::ClSysHint( void )
	: m_sSysHintType(""), m_sText("")
{
	m_bIsAutomatic = M4_TRUE;
	m_pClSysHintPk = NULL;
}


ClSysHint::ClSysHint( ClSysHintPk * ai_pClSysHintPk, m4bool_t ai_bIsAutomatic, const string & ai_sSysHintType, const string & ai_sText )
	: m_sSysHintType(ai_sSysHintType), m_sText(ai_sText)
{	
	m_pClSysHintPk = ai_pClSysHintPk;
	m_bIsAutomatic = ai_bIsAutomatic;	
}


ClSysHint::~ClSysHint()
{
	if (m_pClSysHintPk != NULL)
	{
		delete m_pClSysHintPk;
		m_pClSysHintPk = NULL;
	}
}


ClSysHint::ClSysHint( const ClSysHint & ai_oCopy )
{
	m_bIsAutomatic = ai_oCopy.m_bIsAutomatic;
	m_pClSysHintPk = NULL;
	ClSysHintPk * pClSysHintPk = ai_oCopy.GetClSysHintPk();
	if (pClSysHintPk != NULL)
	{
		m_pClSysHintPk = new ClSysHintPk(pClSysHintPk->GetSysHintDBType(), pClSysHintPk->GetSysHintId());
	}
	m_sSysHintType = ai_oCopy.m_sSysHintType;
	m_sText = ai_oCopy.m_sText;
}


m4bool_t ClSysHint::IsEqualByPk( const ClSysHint * ai_pClSysHint ) const
{
	if (ai_pClSysHint == NULL)
	{
		return M4_FALSE;
	}

	ClSysHintPk  * pClSysHintPk = ai_pClSysHint->GetClSysHintPk();

	if (((m_pClSysHintPk == NULL) && (pClSysHintPk != NULL))
		|| ((m_pClSysHintPk != NULL) && (pClSysHintPk == NULL)))
	{
		return M4_FALSE;
	}
	if (m_pClSysHintPk == NULL && pClSysHintPk == NULL)
	{
		return M4_TRUE;
	}

	//Si estamos aqui seguro que no son nulos.
	return IsEqualByPk(*pClSysHintPk);
}


m4bool_t ClSysHint::IsEqualByPk ( const ClSysHintPk & ai_oClSysHintPk ) const
{
	if (m_pClSysHintPk == NULL)
	{
		return M4_FALSE;
	}

	return m_pClSysHintPk->IsEqualByPk(ai_oClSysHintPk);
}


m4bool_t ClSysHint::IsEqualByDDBB( eDDBBType_t ai_eDDBBType ) const
{
	if (m_pClSysHintPk == NULL)
	{
		return M4_FALSE;
	}

	return m_pClSysHintPk->IsEqualByDDBB(ai_eDDBBType);
}


ClSysHintPk * ClSysHint::GetClSysHintPk( void ) const
{
	return m_pClSysHintPk;
}


m4bool_t ClSysHint::GetIsAutomatic( void )
{
	return m_bIsAutomatic;
}


const string & ClSysHint::GetSysHintType( void ) const
{
	return m_sSysHintType;
}


const string & ClSysHint::GetText( void ) const
{
	return m_sText;
}


