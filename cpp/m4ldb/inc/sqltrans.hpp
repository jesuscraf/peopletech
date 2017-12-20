#ifndef _SQLTRANS_HPP_
#define _SQLTRANS_HPP_

#include "metainfo.hpp"

class	ClMetaSentence;
class	ClGlobVars;
class	ClTranslatorUtils;
class	ClSysHint;

class	_AClSQLTranslator
{
public:
	_AClSQLTranslator(ClMetaSentence *ai_pMtSent, ClGlobVars *ai_pGlobVars, ClTranslatorUtils * ai_pTrUtils, m4bool_t ai_bIsSecurityView, string* ai_psTranslatedSQL);
	virtual ~_AClSQLTranslator(void);

	m4return_t		TranslateSentence(string &ao_stTrans,m4int32_t ai_indColIni,m4int32_t ai_indColEnd);
	virtual m4return_t		TranslateSentenceAfter(string &ao_stTrans);
	virtual	m4return_t		TranslateTable(string &ao_stTrans,m4int32_t ai_TableIndex,m4int32_t ai_precIndex)=0;
	virtual	m4return_t		TranslateCycleRelations		(string &ai_stTrans,m4int32_t ai_TableIndex1,m4int32_t ai_TableIndex2){return M4_SUCCESS;}
	virtual	void			AppendOJinTables_begin(string &ao_stTrans){}
	virtual	void			AppendOJinTables_end(string &ao_stTrans){}
	virtual	void			AppendIJinTables_begin(string &ao_stTrans){}
	virtual	void			AppendIJinTables_end(string &ao_stTrans){}

	//Bug 0077247. Una coma de mas en informix
	virtual	void			EraseOJinTables_begin(string &ao_stTrans){}
	virtual	void			EraseOJinTables_end(string &ao_stTrans){}
	virtual	void			EraseIJinTables_begin(string &ao_stTrans){}
	virtual	void			EraseIJinTables_end(string &ao_stTrans){}

	//Pedro. Para que TableAnalyzer::DumpInfoRelations pueda mostrar informacion en el ldbinsp.
	//No modifica ningún miembro de la clase.
	//Es un puntero a un objeto constante, es decir, no podremos modificar
	//el objeto apuntado por el puntero a GlobVars.
	ClGlobVars *			GetGlobVars (void) const {return m_pGlobVars;}

	//Cada gestor sabrá como traducir los hints. Por defecto no haremos nada.
	virtual m4return_t	TranslateAutomaticSysHints( eClauseType_t ai_eClause, m4int32_t ai_iLevel, string & aio_sTrans );
	virtual m4return_t	TranslateHintBySysHintId( m4int32_t ai_iFilter, const string & ai_sSysHintId, string & aio_sTrans );	
	virtual m4return_t	TranslateSysHintsBegin( string & aio_sTrans );
	virtual m4return_t	TranslateSysHintsEnd( string & aio_sTrans );
	virtual m4return_t	ArrangeSysHintsBeginEnd( string & aio_sTrans, string & aio_sRealHint );

	virtual m4return_t	MergeSysHints( const string & ai_sSourceString, string & aio_sSysHintTranslated, m4int32_t & aio_iPosBegin);

	m4return_t	TranslateHintsByReplacement( string & aio_sSourceString );

protected:

	m4return_t		_ArrangeOrderBy		(string &ao_stTrans, string &ao_stOrderBy);
	m4return_t		_TranslateRanges	(string &ao_stTrans, string &ai_stOrderBy);
	m4bool_t		_CheckToUseAlias	(void);
	m4bool_t		_CheckForOuterJoins	(void);
	//Bug 0078961. Este bug engloba varios cambios.
	//<<<
	m4bool_t		_CheckWithOJInLevel (m4int32_t ai_iLevel);
	m4bool_t		_CheckRelationsWithOJInLevel (m4int32_t ai_iLevel);
	m4bool_t		_CheckAliasWithOJInLevel (m4int32_t ai_iLevel);
	m4bool_t		_CheckFieldsWithOJInLevel (m4int32_t ai_iLevel);
	//>>>
	virtual m4return_t		_RemoveOuterSigns	(string &ao_stTrans);

	m4int32_t		_GetNextField			( string &ai_stTrans,m4int32_t Pos,eClauseType_t &eClause,m4int32_t &iLevel);

	m4bool_t		_MainSelectHasGroupBy			(void);
	m4bool_t		_MainSelectHasOrderBy			(void);


	m4return_t		_Translate				( string *ai_stSource, string &ai_stTrans );
	m4return_t		_TranslateSomeColumns	( string *ai_stSource, string &ai_stTrans,m4int32_t ai_indColIni,m4int32_t ai_indColEnd );

	m4return_t		_TranslateField			(string &ao_stTrans ,eClauseType_t ai_eClause);
	m4return_t		_TranslateField			(string &ao_stTrans,ClMetaField *ai_pField);

	m4return_t		_TranslateAlias			(string &ao_stTrans,ClMetaAlias * ai_pAlias,m4int32_t ai_indRealTable,m4bool_t &ai_bAdded);

	m4return_t		_TranslateClausule		(string &ai_stTrans,eClauseType_t ai_eClause,m4int32_t ai_iLevel,m4int32_t &Incr);
	m4return_t		_TranslateWhere			(string &ai_stTrans,m4int32_t ai_iLevel,m4int32_t &ai_Incr);
	m4return_t		_TranslateOrderBy		(string &ai_stTrans,m4int32_t ai_iLevel,m4int32_t &ai_Incr);
	m4return_t		_TranslateBeginStatement(string &ai_stTrans,m4int32_t ai_iLevel);
	virtual m4return_t		_TranslateFrom			(string &ai_stTrans,m4int32_t ai_iLevel);

	virtual m4return_t		_TranslateRealJoins		(string &ai_stTrans,m4int32_t ai_iLevel,m4bool_t &bAddedWhere);
	virtual m4return_t		_TranslateRealJoins		(string &ai_stTrans,ClMetaObject *ai_pObj,m4bool_t &bAddedWhere);
	
	virtual	m4return_t		_TranslateRelations				(string &ai_stTrans,m4int32_t ai_iLevel,m4bool_t &bAddedWhere);
	// Traducción de "relaciones" que no son joins ( entre distintos niveles ). Solo tiene funcionalidad en SQLServer.
	virtual	m4return_t		_TranslateSpecialRelations		(string &ai_stTrans,m4int32_t ai_iLevel,m4bool_t &bAddedWhere){return M4_SUCCESS;}
	
	m4return_t		_TranslatePKs					(string &ai_stTrans, ClMetaObject* ai_pTable);

	m4return_t		_TranslateInsertSubSelect		(string &ai_stTrans);

	m4return_t		_CheckTableBaseInSubselect		(ClMetaObject* pTable);

	m4return_t		_TranslateHintText				(m4int32_t ai_iFilter, ClSysHint* ai_poHint, string &aio_sHint);

	void			_AddSecurityParams(PVcClMtParam ai_pcvParams, string* ai_psObjectsSQL);

	m4int32_t			m_CurrentFld;
	m4bool_t			m_bUsingAlias;
	m4bool_t			m_bOuterJoins;
	m4bool_t			m_bIsSecurityView;

	ClMetaSentence		*m_pMtSent;
	ClGlobVars			*m_pGlobVars;

	// Vectores con Info de la Select.
	PString				m_pstStatemCol;
	PVcClMtColumn		m_pvcColumns;
	PVcClMtParam		m_pvcParamCol;

	PString				m_pstStatemWhr;
	PVcClMtParam		m_pvcParamWhr;
	PVcClMtRelat		m_pvcRel;


	PVcClMtField		m_pvcFld;

	PVcClMtObject		m_pvcFrmObj;
	PVcClMtAlias		m_pvcFrmAls;

	PMpFieldAndValue	m_pmpFieldAndValue;

	ClTranslatorUtils	*m_pTrUtils;

	string*				m_psTranslatedSQL;
};

#endif
