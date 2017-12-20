//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb
// File:                apisql.hpp
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


#ifndef __APISQL_HPP__
#define __APISQL_HPP__

#include "m4stl.hpp"
#include "defmain.hpp"
#include "clmtsent.hpp"

class ClLConn;

enum eSearchTypeIntoInheritanceTable_t {
	M4LDB_SEARCH_TYPE_INTO_INHERITANCE_TABLE_FIND_BOTH		= 0,
	M4LDB_SEARCH_TYPE_INTO_INHERITANCE_TABLE_FIND_PARENT	= 1,
	M4LDB_SEARCH_TYPE_INTO_INHERITANCE_TABLE_FIND_CHILD		= 2
};

m4return_t _X_GetToken (m4pchar_t &ao_pStm, m4pchar_t ao_pToken, m4int32_t &ao_lenToken, m4char_t &ao_cSeparator, m4bool_t &ao_bIsWord, m4bool_t ai_bIsReal, m4int32_t *ao_pInvRelation = NULL);

//Bug 0089536
//Tendremos un mapa donde tendremos handles de items columna invalidos y el alias asociado del 
//campo: @itemcol = alias.campo. En el mapa tendre el handle del item y el alias asociado!.
//Elegimos un mapa pq no tiene mucho sentido que un mismo item sea objetivo de dos campos, por lo tanto
//la asociacion será única.
typedef map<m4int32_t, string, less<m4int32_t> >	MapItemColAlias;
typedef MapItemColAlias::iterator					itMapItemColAlias;

class ClAPISQL
{
	ClMetaSentence *m_pMtSent;
	ClGlobVars *m_pGlobVars;
	m4pchar_t m_pCurrent;
	m4pchar_t m_pAuxString;
	PString m_pstAPISQLStm;
	eApiSqlType_t m_eApiSql;
	m4int32_t m_lFilter;
	m4bool_t m_bNoAddDtLastUpdate;
	PVcClMtColumn m_pvcAuxColItm;
	PVcClMtField m_pvcAuxColFld;
	PVcClMtParam m_pvcAuxColPar;
	PString m_pstAuxCol;

	PVcClMtColumn m_pvcAuxColLngItm;
	PVcClMtField m_pvcAuxColLngFld;
	PVcClMtParam m_pvcAuxColLngPar;
	PString m_pstAuxColLng;
	
	PVcClMtField m_pvcAuxWhrFld;
	PVcClMtParam m_pvcAuxWhrPar;
	PString m_pstAuxWhr;

	PVcClMtRelat m_pvcAuxFrmRel;
	PVcClMtObject m_pvcAuxFrmObj;

	PVcClMtColumn m_pvcAuxItm;
	PVcClMtField m_pvcAuxFld;
	PVcClMtParam m_pvcAuxPar;
	PString m_pstAux;
	eClauseType_t m_eClause;
	m4int32_t m_lMaxLevel;
	m4int32_t m_lLevel;
	m4int32_t m_lInitLevel;
	m4int32_t m_indAlsBasis;
	m4int32_t m_indAlsSec;
	m4bool_t m_bConnActivate;
	m4int32_t m_indFilterSelect;

	//Contadores de los parentesis de la sentencia APISQL.
	m4int32_t m_iopen;
	m4int32_t m_iclose;

	//Añadimos un miembro que indica de donde procede la relacion.
	m4uint8_t m_iAPISQLSource;
	m4bool_t m_bIsSecurityView;

	//Bug 0082678, 0082953
	m4bool_t m_bLeftOperand;
	m4bool_t m_bLeftOperandIsMultiOrganization;
	m4bool_t m_bRightOperandIsMultiOrganization;
	m4bool_t m_bAddedField;
	m4bool_t m_bAddedParam;

	//Bug 0089536.
	MapItemColAlias	m_oItemColAlias;

	//Bug 0083765
	//Poder volver a poner flags en valor inicial.
	void ResetFlagsMultiOrganization (void);

	//Bug 0081842.	
	m4bool_t ObjectsIntoInheritanceChain( ClMetaObject * ai_pSourceObject, ClMetaObject * ai_pDestinationObject, eSearchTypeIntoInheritanceTable_t ai_iSearchTypeIntoInheritanceTable );

	m4return_t _CreateAlias(ClMetaObject *ai_pMtObj,m4bool_t ai_bBasis);
	m4int32_t _FindIdxAlias (m4pcchar_t ai_pAlias, m4int32_t ai_lFilter, m4int32_t ai_lLevel, eClauseType_t ai_eClause);
	m4bool_t FindOuterJoin (void);
	m4return_t FindSeparator (m4char_t ai_cSeparator);
	void FindOR (m4pchar_t &ao_pCurrent);
	void AddSeparators (eClauseType_t ai_eClause, m4int32_t ai_lLevel);
	void AddToOr (m4bool_t ai_bIsBegin);
	void SetAuxVectors (eClauseType_t ai_eClause, m4int32_t ai_lLevel = M4LDB_NULL_INDEX, m4bool_t ai_bNoAdd = M4_FALSE);

	// Cuenta el número de parentesis que se abren y se cierran de la sentencia APISQL
	// y da un mensaje de error si no coinciden.
	m4return_t CounterParenthesisAndQuotes(m4pchar_t ai_pStmt);
		
	m4return_t DescribeFilterFunct (const string &ai_stFilterFunct,
								 m4int32_t ai_iBeginPos, 
								 m4int32_t &ao_iNumPar, 
								 m4int32_t &ao_iSizeFunct);
	m4return_t DescribeParametersFilterFunct (const string &ai_stFilterFunct,
											 m4int32_t ai_iBeginPos,
											 string *ao_pParameters);
	m4return_t ReplaceRealFunctParameter (string &ao_stRealFunct, 
										 m4int32_t ai_iPosPar, 
										 string ai_stPar);

	m4return_t AnalizeFunction (m4pchar_t ai_pToken);
	m4pcchar_t AnalizeParan (ClMetaParam *ai_poParam);
	m4return_t AnalizeArgument (m4char_t ai_cSeparator);
	m4return_t AnalizeFieldOrConst (m4pchar_t ai_pToken, m4bool_t *ai_pbIsFld);
	m4return_t AnalizeItemCol (m4pchar_t ai_pToken, m4int32_t &ao_hItem);
	m4return_t AnalizeItemInOrderBy	(m4pcchar_t ai_pToken);
	m4return_t AnalizeItem (m4pchar_t ai_pToken);
	m4return_t AnalizeSessionItem (m4pchar_t ai_pToken, m4bool_t ai_bAppendParam);
	m4return_t AnalizeInternalParam (m4pchar_t ai_pToken);
	m4return_t AnalizeSpecialParam (m4pchar_t ai_pToken);

	m4return_t AnalizeOrderByWithItems (void);
	void       AnalizeLiteral (m4pcchar_t ai_pccLiteral);
	m4return_t AnalizeFromClause (m4bool_t ai_bWhere, m4bool_t ai_bChangeSecTable);
	m4return_t AnalizeWhereClause (m4int32_t ai_iInvRelation = 0);
	m4return_t AnalizeColumnsClause (void);
	m4return_t AnalizeFieldsClause (void);
	m4return_t AnalizeValuesClause (void);
	m4return_t AnalizeFieldsAndValuesClause (void);

	m4return_t AnalizeMultiFilter (void);

	m4return_t AnalizeLogicElements (m4bool_t ai_bAdd, m4bool_t ai_bIsInsert);

	m4return_t AddDTLastUpdate (void);

	m4return_t AddConnectorFilters (void);
	m4return_t AddAutomaticFilters	(m4char_t ai_iApplyAutoFilters, m4bool_t &ao_bExistsFilter);
	m4return_t AddSocFilters		(m4bool_t &ao_bExistsFilter);
	m4return_t AddFilterOrgLevel 	(m4bool_t &ao_bExistsFilter);
	m4return_t AddFilterControlSoc	(m4uint8_t ai_iAccessLevel, m4int8_t ai_iOrgType, m4bool_t &ao_bExistsFilter);
	m4return_t AddFilterChannelMonoSoc	(m4bool_t &ao_bExistsFilter);
	m4return_t AddFilterChannelMultiSoc	(m4bool_t &ao_bExistsFilter);
	m4return_t AddOneFilterRoleOrganization(ClMetaObject *ai_pObject, m4id_t ai_idField, m4pcchar_t ai_pccFilter);
	m4return_t AddFilterRoleOrganization(ClMetaObject *ai_pObject, m4id_t ai_idField);
	m4return_t AddTramitFilters		(m4bool_t &ao_bExistsFilter);

	m4bool_t IsFieldInUse (m4id_t ai_idField, m4int32_t ai_iAlias);
	m4bool_t IsFieldDominatedInJoin (m4id_t ai_idField, m4int32_t ai_iAlias);

	//Bug 0071469
	//Se ha añadido un argumento opcional. Sólo valdrá para autofiltros.
	//La diferencia está en la recursion, en autofiltros sí buscare el campo con tipo interno en la jerarquia
	//ascendente. Para tipos internos de otro tipo en principio no se busca.
	m4return_t AddFilterByInternalType	(eFldType_t ai_eFldType, ClMetaObject *ai_pLgcObj, m4bool_t ai_bAutomaticFilters = M4_FALSE);
	m4return_t AddFieldByInternalField	(eFldType_t ai_eFldType, ClMetaObject *ai_pObject, m4id_t ai_idField, m4bool_t ai_bAutomaticFilters = M4_FALSE, m4int8_t *ao_piM4Type = NULL);

	m4return_t FindIndexFieldwoAlias (ClMetaField *ai_pMtField);
	m4return_t FindFieldInTheDicticionary (ClMetaField *ai_pMtField);
	m4return_t _FindIndexField (m4id_t ai_idField, m4int32_t ai_indLgcTbl, m4int32_t &ao_indFld, m4int32_t &ao_indRealTbl);
	m4return_t FindIndexField (ClMetaField *ai_pMtField, m4bool_t ai_bShowError, m4int32_t ai_posLgcTbl);
	m4return_t FindIntoInheritance (ClMetaField *ai_pMtField, m4bool_t ai_bShowError, m4int32_t ai_posLgcTbl);

	m4int32_t GetNumLgcTbl (void);
	ClMetaObject *GetLgcTblByPos (m4int32_t ai_indTblLgc);

	m4return_t SetDDBBType (void);

public:
	ClAPISQL (eApiSqlType_t ai_eApiSql);
	~ClAPISQL (void);
	
	//Funciones que establecen/devuelven el origen del API
	m4uint8_t GetAPISQLSource(void)
	{
		return m_iAPISQLSource;
	}

	void SetAPISQLSource(m4uint8_t ai_iAPISQLSource)
	{
		m_iAPISQLSource = ai_iAPISQLSource;
	}

	void SetIsSecurityView(void)
	{
		m_bIsSecurityView = M4_TRUE;
	}

	m4return_t AnalizeSQL (string &aio_stAPISQLStm, ClMetaSentence *ai_pMtSent, m4bool_t ai_bWithID, m4bool_t ai_bHasConnFilter = M4_FALSE, m4char_t ai_iApplyAutoFilters = 0);
	m4return_t TransferMetaData (m4bool_t ai_bExistsMoreObjects, m4bool_t ai_bProcessOrs);
};

#endif

