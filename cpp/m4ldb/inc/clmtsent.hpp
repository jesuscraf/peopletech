//==============================================================================
//
//  (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb
// File:                clmtsent.hpp
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


#ifndef _X_CLMTSENT_HPP_
#define _X_CLMTSENT_HPP_

#include "clglbvar.hpp"

#include "m4stl.hpp"
#include "defmain.hpp"
#include "metainfo.hpp"
#include "cldinarr.hpp"




//--------------------------------------------------------------
const	m4pchar_t M4LDB_SYSHINT_APISQL_WORD				= "#M4HINT";

const	m4pchar_t M4LDB_SYSHINT_LITERAL_DB				= "DB";
const	m4pchar_t M4LDB_SYSHINT_LITERAL_ID				= "ID";
const	m4pchar_t M4LDB_SYSHINT_LITERAL_TYPE			= "TYPE";
const	m4pchar_t M4LDB_SYSHINT_LITERAL_TEXT			= "TEXT";

const	m4pchar_t M4LDB_SYSHINT_DB_ORACLE				= "ORACLE";
const	m4pchar_t M4LDB_SYSHINT_DB_SQLSERVER			= "SQLSERVER";
const	m4pchar_t M4LDB_SYSHINT_DB_INFORMIX				= "INFORMIX";
const	m4pchar_t M4LDB_SYSHINT_DB_DB2					= "DB2";

const	m4pchar_t M4LDB_SYSHINT_TYPE_SELECT				= "SELECT";
const	m4pchar_t M4LDB_SYSHINT_TYPE_FROM				= "FROM";
const	m4pchar_t M4LDB_SYSHINT_TYPE_WHERE				= "WHERE";
const	m4pchar_t M4LDB_SYSHINT_TYPE_GROUPBY			= "GROUPBY";
const	m4pchar_t M4LDB_SYSHINT_TYPE_ORDERBY			= "ORDERBY";

const	m4pchar_t M4LDB_SYSHINT_ORACLE_BEGIN			= "/*+";
const	m4pchar_t M4LDB_SYSHINT_ORACLE_END				= "*/";



class ClSysHintPk {
	string		m_sSysHintDBType;
	string		m_sSysHintId;	

public:
	ClSysHintPk( void );
	ClSysHintPk( const string & ai_sSysHintDDBBType, const string & ai_sSysHintId );
	~ClSysHintPk();
	ClSysHintPk( const ClSysHintPk & ai_oCopy );

	m4bool_t IsEqualByPk ( const ClSysHintPk & ai_oClSysHintPk ) const;
	m4bool_t IsEqualByDDBB( eDDBBType_t ai_eDDBBType ) const;

	const string & GetSysHintDBType( void ) const;
	m4return_t GetSysHintDBTypeByDDBB( eDDBBType_t & ao_eDDBBType ) const;
	const string & GetSysHintId( void ) const;	
	string GetComposedSysHintPk( void ) const;
};

class ClSysHint {
	m4bool_t	m_bIsAutomatic;
	ClSysHintPk * m_pClSysHintPk;
	string		m_sSysHintType;
	string m_sText;

public:
	ClSysHint( void );
	ClSysHint( ClSysHintPk * ai_pClSysHintPk, m4bool_t ai_bIsAutomatic, const string & ai_sSysHintType, const string & ai_sText );
	~ClSysHint();
	ClSysHint( const ClSysHint & ai_oCopy );

	m4bool_t IsEqualByPk( const ClSysHint * ai_pClSysHint ) const;
	m4bool_t IsEqualByPk( const ClSysHintPk & ai_oClSysHintPk ) const;
	m4bool_t IsEqualByDDBB( eDDBBType_t ai_eDDBBType ) const;

	ClSysHintPk * GetClSysHintPk( void ) const;
	m4bool_t GetIsAutomatic( void );
	const string & GetSysHintType( void ) const;
	const string & GetText( void ) const;
};

class ClDynamicHint
{

private:

	m4int32_t	m_iFilter ;
	string		m_sId ;

public:

	ClDynamicHint( m4int32_t ai_iFilter, m4pcchar_t ai_pccId )
	{
		m_iFilter = ai_iFilter ;
		m_sId = ai_pccId ;
	}

	m4int32_t	GetFilter( void ) const
	{
		return( m_iFilter ) ;
	}

	const string&		GetId( void ) const
	{
		return( m_sId ) ;
	}
};
//--------------------------------------------------------------



typedef set<ClMetaRelat *, less <ClMetaRelat *> > ClMetaRelatSet_t;
typedef ClMetaRelatSet_t::iterator ClMetaRelatSetIt_t;

class ClMetaSentence
{
	//Bug 0071469
	//Le hacemos amiga para que accede al vector de from. Filtros automáticos.
	friend class ClAPISQL;
	friend class ClLConn;
	friend class _AClSQLTranslator;
	friend class ClSQLTranslator_SQLServer;
	friend class ClSQLTranslator_Informix;

	m4bool_t m_bReal;
	m4bool_t m_bAPISQL;

	m4int32_t m_lSecMask;

	m4int32_t m_lAudMask;
	m4id_t m_idAudObj;

	ClDinArr *m_pParam;

	m4int32_t m_indInitCol;

	//Bug 0071661.
	m4bool_t m_bNoWait;

	m4bool_t m_bForUpdate;
	m4bool_t m_bIsDistinct;

	m4bool_t m_bHasRange;
	m4bool_t m_bPartialLoad;
	m4bool_t m_bHasValidation;

	// Para controlar que en un delete sin validaciones no pongamos Alias en la tabla principal.
	m4bool_t m_bDontUseAlias;
	// Optimización en el delete cuando no queremos validación.
	m4bool_t m_bExistsInheritance;

	m4bool_t m_bHasOr;

	ClGlobVars *m_pGlobVars;

	m4int32_t m_indDtLastUpdate;
	eStmType_t m_eStmType;	// Tipo de sentencia

	// Vectores con Info de la Select.
	PString			m_pstStatemCol;
	PVcClMtColumn	m_pvcColumns;
	PVcClMtParam	m_pvcParamCol;

	PString			m_pstStatemWhr;
	PVcClMtParam	m_pvcParamWhr;
	PVcClMtRelat	m_pvcRel;


	PVcClMtField	m_pvcFld;

	PVcClMtObject	m_pvcFrmObj;
	PVcClMtAlias	m_pvcFrmAls;

	PMpFieldAndValue m_pmpFieldAndValue;

	VcSysHint		*m_pVcSysHint;
	VcDynamicHint	*m_pVcDynamicHint;

	// Para no tenr que calcularlo continuamente.
	m4int32_t		m_numCol;
	m4int32_t		m_numColNVL;
	m4int32_t		m_numPar;
	// Tamaño delvector de Objetos que le he dejado al APISQL
	m4int32_t		m_iLastObjectSize;
	// En algunos casos hay sentencias auxiliares. El traductor me activa este flag.

	//----
	//Pedro: Caso soporte 15113. El problema está en la tabla de seguridad.
	//Al validar con la tabla de control de seguridad SDC_VAL_SEC_LDB.
	//Se añaden funciones miembro de modificación.
	m4bool_t	m_bIsValidatingSecurity;
	//----

	//Bug 0081719
	//Tendremos un Flag que permitira la ordenacion de parametros en el traductor.
	m4bool_t	m_bSortParamsColumnsLimitation;

	//Si el APISQL es un INSERT INTO ... SELECT
	m4bool_t		m_bIsInsertIntoSelect ;
	//Si el APISQL es un UPDATE SET .... SELECT
	m4bool_t		m_bIsUpdateSelect ;

	// Filtro de la sentencia principal
	m4int32_t	m_iMainFilter ;
	
	void		_JoinRealTablesForBasis  (m4int32_t ai_iLevel);

	void		_ClearColumnsElements	(void);
	void		_ClearFields			(void);
	void		_ClearWhereElements		(void);
	void		_ClearVector			(VcClMtRelat &ai_vRel,VcClMtParam &ai_vParamWhr,VcClMtField &ai_vFieldWhr);
	
	void		_DeleteFirstElement		(PVcClMtParam ai_vParamWhr,PVcClMtField ai_vFieldWhr);

	m4return_t	_CreateColumnStatem		(void);
	m4return_t	_CreateFld				(void);
	m4return_t	_CreateWhereStatem		(void);
	m4return_t	_CreateSubSelect		(eClauseType_t ai_eClause,m4int32_t ai_Level,m4int32_t ai_NewLevel);

	m4return_t	_InsertColumnSeparator	(eClauseType_t ai_eClause,m4int32_t ai_Level);
	m4return_t	_InsertFieldSeparator	(eClauseType_t ai_eClause,m4int32_t ai_Level);
	m4return_t	_InsertWhereSeparator	(eClauseType_t ai_eClause,m4int32_t ai_Level);

	m4return_t	_PrepareColumnStatem	(void);
	m4return_t	_PrepareWhereStatem		(void);
	m4return_t	_AppendColumnsString	(string &ai_stColumns,m4bool_t ai_bLong);

	m4int32_t	_GetNextClause			(string &ai_stStatem,m4int32_t ai_lOrigin,eClauseType_t &ao_eClause,m4int32_t &ao_Level);
	m4bool_t	_FindClause				(string &ai_stBuf,const m4char_t * ai_pToFind,m4uint32_t &ao_IniPos);
	m4bool_t	_FindClause				(string &ai_stBuf,eClauseType_t ai_eClause,m4int32_t ai_Level,m4uint32_t &ao_IniPos);

	eClauseType_t _GetClauseType		(m4char_t ai_clause);
	m4return_t	_GetClauseAndLevel		(const m4char_t * ai_pClause, eClauseType_t &ao_eClause, m4int32_t &ao_Level);
	m4int32_t	_GetLevel				(const m4char_t *ai_Level);
	
	m4return_t	_AddInfoFilter			( PVcClMtParam ai_pvcParamWhr,PVcClMtField ai_pvcFieldWhr,string &ai_stFilter,eClauseType_t ai_eClause,m4int32_t ai_Level,m4int32_t &ao_lPos, m4bool_t ai_bProcessOrs);
	m4return_t	_AddInfoPartFilter		( PVcClMtParam ai_vParamWhr,PVcClMtField ai_vFieldWhr,string &ai_stFilter,eClauseType_t ai_eClause,m4int32_t ai_Level,m4int32_t &ao_lPos, m4bool_t ai_bProcessOrs);
	m4return_t	_AddInfoPartFilterAdd	( PVcClMtParam ai_vParamWhr,PVcClMtField ai_vFieldWhr,string &ai_stFilter,eClauseType_t ai_eClause,m4int32_t ai_Level,m4int32_t &ao_lPos, m4int32_t ai_lNextClausePos, m4bool_t ai_bProcessOrs, m4bool_t &ao_bOut);
	m4return_t	_ReplaceRelationAdd		( ClMetaRelat *ai_pRel, m4bool_t &ao_bOut);

	m4return_t	_AddInfoColumns				( PVcClMtColumn ai_pVcClMtCol,PVcClMtParam ai_pvcParamCol,PVcClMtField ai_pvcFieldCol,string *ai_stColumns,m4bool_t ai_bIsLong,m4uint32_t ai_iColumnStart);
	m4return_t	_AddInfoColumnsUpdateInsert	( PVcClMtParam ai_pvcParamCol,PVcClMtField ai_pvcFieldCol );

	m4return_t	_CreateOneParam			(ClDinArr *ai_pParam,ClMetaParam *pParam,m4int32_t ai_indParam, m4bool_t ai_bUseOldValue = M4_FALSE);
	
	ClRegister *_MoveToField			(ClMetaItemCol *ai_pCol);
	m4bool_t	_GetColumnEmulateNVL	(ClMetaItemCol *ai_pCol);
	
	ClMetaItemCol	*_GetColumn			(m4int32_t ai_indCol );
	
	//Bug 0099118
	m4bool_t		_AnalizeRelationsWithInheritObjects( void );
	m4bool_t		_AnalizeXJoinRelations( void );
	m4bool_t		_ReduceXJoinRelation( ClMetaRelatSet_t &aio_oRelationSet, const ClMetaRelat * ai_pStartRel, m4int32_t ai_iXJoinLeftAlias, m4int32_t ai_iXJoinRigthAlias, m4int32_t ai_iXJoinRigthField );
	m4bool_t		_AnalizeRelationsDuplicated (VcClMtRelat & ao_vRelations, m4int32_t ai_iAlias1r1, m4int32_t ai_iAlias2r1, m4int32_t ai_iAlias1r2, m4int32_t ai_iAlias2r2);
	m4return_t		_CheckOrgRelations	(PVcClMtRelat ai_vRel);
	m4bool_t		_CheckRelationsToReduce(m4int32_t ai_t1r1,m4int32_t ai_t2r1,m4int32_t ai_t1r2,m4int32_t ai_t2r2);
	m4int32_t		_GetIdInternalField	(ClMetaField *ai_pField);

	m4return_t _CleanOR (void);

	//Bug 0077223. OJ en conectores informix.
	m4return_t		_AnalizeObjectsToReduceAndArrangeFields (void);
	m4return_t		_ChangeFieldsPointedToObjectsBasis (void);

public:

	ClMetaSentence (void);
	~ClMetaSentence (void);
	void Clear (void);

	// Funciones necesarias para el ApiSQL.
	m4bool_t	IsReal			(void);
	m4bool_t	IsAPISQL		(void);

	//Si el APISQL es un INSERT INTO ... SELECT
	m4bool_t	GetIsInsertIntoSelect	(void) const;
	void		SetIsInsertIntoSelect	(m4bool_t ai_bInsertIntoSelect);

	//Si el APISQL es un UPDATE SET .... SELECT
	m4bool_t	GetIsUpdateSelect	(void) const;
	void		SetIsUpdateSelect	(m4bool_t ai_bIsUpdateSelect);

	ClGlobVars	*GetpGlobVars	(void);
	eStmType_t	GetStmType		(void);
	ClDinArr	*GetParam		(void);
	void		SetParam		(ClDinArr *ai_pParam);
	m4int32_t	GetIndCurFilter (void);

	//Bug 0071661.
	void		SetNoWait	(m4bool_t ai_bNoWait);
	m4bool_t	GetNoWait	(void);

	void		SetForUpdate	(m4bool_t ai_bForUpdate);
	m4bool_t	GetForUpdate	(void);
	void		SetDistinct		(m4bool_t ai_bIsDistinct);
	m4bool_t	GetDistinct		(void);
	void		GetObjectCopy	(VcClMtObject &ai_pvcObj);
	void		SetIndDtLastUpdate(m4int32_t ai_indDtLU);
	m4int32_t	GetIndDtLastUpdate(void);
	void		SetDontUseAlias	(m4bool_t ai_bDUA);
	m4bool_t	GetDontUseAlias	(void);

	//----
	//Pedro: Caso soporte 15113. El problema está en la tabla de seguridad.
	m4bool_t	GetValidatingSecurity(void) const
	{
		return m_bIsValidatingSecurity;
	}	
	void		SetValidatingSecurity(m4bool_t ai_bIsValidatingSecurity)
	{
		m_bIsValidatingSecurity = ai_bIsValidatingSecurity;
	}
	//----

	//Bug 0081719
	//Tendremos un Flag que permitira la ordenacion de parametros en el traductor.
	m4bool_t	GetSortParamsColumnsLimitation(void) const
	{
		return m_bSortParamsColumnsLimitation;
	}	
	void		SetSortParamsColumnsLimitation(m4bool_t ai_bSortParamsColumnsLimitation)
	{
		m_bSortParamsColumnsLimitation = ai_bSortParamsColumnsLimitation;
	}

	// Funciones de Alias y Objetos.
	// A extinguir.
	m4id_t			GetIdObjectByPos	(m4int32_t ai_indObj);
	m4id_t			GetIdObjectBasis	(void);
	m4int32_t		GetPosBasis			(void);
	m4pcchar_t		GetAliasByIdx (m4int32_t ai_indAlias);

	m4bool_t		HasValidation (void);
	m4bool_t		HasRange (void);
	m4bool_t		IsPartialLoad (void);

	void			SetOr (m4bool_t ai_bHasOr);
	m4bool_t		HasOr (void);


	void SetAudMask (m4int32_t ai_lAudMask);
	m4int32_t GetAudMask (void);



	m4int32_t		GetNumItemCol (void);
	m4int32_t		GetNumObject			(void);
	m4int32_t		GetNumAlias				(void);
	m4int32_t		GetNumFields			(void);
	m4int32_t		GetNumRelations			(void);
	ClMetaObject	*GetObjectByPos			(m4int32_t ai_Pos);
	ClMetaAlias		*GetAliasByPos			(m4int32_t ai_Pos);
	ClMetaField		*GetFieldByPos			(m4int32_t ai_Pos);
	ClMetaRelat		*GetRelationByPos		(m4int32_t ai_Pos);
	m4bool_t		IsObjectBasisByAliasPos	(m4int32_t ai_indAlias);
	m4int32_t		GetObjectPosByAliasPos	(m4int32_t ai_indAlias);
	m4int32_t		GetNewAliasPosByAliasPos(m4int32_t ai_indAlias);
	m4return_t		AddAlias				(ClMetaAlias *ai_pstAlias);


	m4bool_t CanReadByPos (m4int32_t ai_indCol);

	m4return_t	Create (ClGlobVars *ai_pGlobVars, eStmType_t ai_eStmType, m4bool_t ai_bAPISQL, m4bool_t ai_bReal, m4bool_t ai_bHasRange, m4bool_t ai_bPartialLoad, m4bool_t ai_bHasValidation);

	m4return_t	AddInfoColumns		( PVcClMtColumn ai_pVcClMtCol,PVcClMtParam ai_pvcParamCol,PVcClMtField ai_pvcFieldCol,string *ai_stColumns,m4uint32_t ai_iColumnStart);
	m4return_t	AddInfoLongColumns	( PVcClMtColumn ai_pVcClMtCol,PVcClMtParam ai_pvcParamCol,PVcClMtField ai_pvcFieldCol,string *ai_stColumns,m4uint32_t ai_iColumnStart);

	m4return_t	AddInfoFilter		( PVcClMtParam ai_pvcParamWhr,PVcClMtField ai_pvcFieldWhr,string *ai_stWhere, m4bool_t ai_bProcessOrs);
	m4return_t	AddInfoObjects		( PVcClMtObject ai_pVcClMtObj,PVcClMtRelat ai_pVcClMtRel, m4bool_t ai_bSec);

	m4return_t	_X_AnalizeRelations	( void );
	m4return_t	AnalizeRelations	( void );
	m4return_t	AdjustLogObj		( void );
	m4return_t	TranslateSentence	( string &ai_stTrans, m4int32_t ai_indColIni = -1, m4int32_t ai_indColEnd = -1, m4bool_t ai_bIsSecurityView = M4_FALSE );

	m4return_t	ClearObjectsBasis	(void);
	m4return_t	CreateParams		( ClDinArr *ai_pParam, m4bool_t ai_bUseOldValue = M4_FALSE);
	m4return_t	CreateColumns		( ClDinArr *ai_pColumns);
	m4return_t	CreateVirtualColumns( VcVirtualParam &ai_pColumns,m4int32_t ai_indColIni=-1,m4int32_t ai_indColEnd=-1 );
	m4return_t	CreateVirtualParams	( VcVirtualParam &ai_pParams,m4int32_t ai_indColIni=-1,m4int32_t ai_indColEnd=-1 );
	m4return_t	InvalidateColumn	( m4int32_t ai_indCol );

	void		Dump(ofstream &ofs);
	void		Dump_Vector_Fld (PVcClMtField ai_vFieldWhr);

	m4int32_t GetItemPosFldColByPos (m4int32_t ai_indItem);
	m4int32_t GetNumField (void);
	ClMetaParam *GetMetaParamByField (m4id_t ai_idField);

	m4bool_t ExistsInheritance (void);
	void SetInheritance (void);

	ClMetaField *GetColumnByIdField (m4id_t ai_idField);

	void			DumpInfoRelations( void );

	m4int32_t		GetNumSysHint( void );
	ClSysHint *		GetSysHintByPos( m4int32_t ai_iPosition );
	ClSysHint *		GetSysHintByPk( const ClSysHintPk & ai_oClSysHintPk );	
	m4return_t		AddListOfSysHint( const VcSysHint & ai_VcSysHint );
	m4bool_t		IsSameSysHints( const VcSysHint & ai_VcSysHint );

	m4int32_t		GetNumDynamicHint( void );
	ClDynamicHint*	GetDynamicHintByPos( m4int32_t ai_iPosition );
	m4return_t		AddDynamicHint( m4int32_t ai_iFilter, m4pcchar_t ai_pccId );

	m4int32_t	GetMainFilter( void ) const
	{
		return( m_iMainFilter ) ;
	}

	void	SetMainFilter( m4int32_t ai_iMainFilter )
	{
		m_iMainFilter = ai_iMainFilter ;
	}

protected:
	m4return_t		_AddSysHint( ClSysHint * ai_pClSysHint );
	m4return_t		_ValidateSysHint( ClSysHint * ai_pClSysHint );
	m4return_t		_ValidateSysHintDBType( ClSysHint * ai_pClSysHint );
	m4return_t		_ValidateSysHintType( ClSysHint * ai_pClSysHint );
};
//  Quiero implementar metodos template.
//	No he encontrado otra forma que :
//	En Sun no se permiten templates definidos dentro de clases ( Lo suyo es que fuera dentro de la clase ).

template<class Par>
class _ColumnDeleter{
public:
	void operator () (vector<Par* > * ai_pvcSource,m4int32_t ai_indCols);
};


// Añadir columnas.
template<class Par>
class _ColumnAppender{
public:
	m4return_t	operator () (vector<Par* > * ai_pvcSource,vector<Par* > * ai_pvcTarget,m4bool_t ai_bIsLong,m4int32_t ai_nCols);
};


// Insertar en vectores.
template<class Par>
class _VectorInserter{
public:
	m4return_t	operator () (vector<Par* > *vcSource,vector<Par* >  *vcTarget,eClauseType_t ai_eClause,m4int32_t ai_Level);
	m4return_t	operator () (vector<Par* > *vcSource,vector<Par* >  *vcTarget,eClauseType_t ai_eClause,m4int32_t ai_Level,m4int32_t ai_iAddToOr);
	m4return_t	operator ()	(Par* ai_pSource,vector<Par* >  *pvcTarget, m4int32_t ai_Level,m4int32_t ai_iAddToOr);
};


// Limpiar vectores por niveles.
template<class Par>
class _VectorClearLevel{
public:
	m4return_t	operator () (vector<Par* > *vcSource,eClauseType_t ai_eClause,m4int32_t ai_Level);
};


// Volcado de vectores.
template<class Par>
class _VectorDumper{
public:
	void	operator () (ofstream &ofs,vector<Par* > &vc1);
};


class _StringClearer{
public:
	_StringClearer(string *ai_stToClean){
		m_pst=ai_stToClean;
	}
	~_StringClearer(void){
		if (m_pst){
			m_pst->erase();
		}
	}
private:
	string *m_pst;
};


// Borrado de vectores.
template<class Par>
class _VectorClearerOne{
public:
	_VectorClearerOne(vector<Par* > &vcToClean){
		m_pv = &vcToClean;
	}
	~_VectorClearerOne(void)
	{
		if (m_pv){
			M4_TYPE_ITERATOR	vector<Par* >::iterator	it;			
			for (it = m_pv->begin();it != m_pv->end();++it){
				delete (*it);
			}
			m_pv->clear();
			m_pv=0;
		}
	}
private:
	vector<Par* > *m_pv;
};


template <class Par1,class Par2,class Par3>
class _VectorClearer{
public:
	_VectorClearer(vector<Par1* > *pvcToClean1,vector<Par2* > *pvcToClean2=0,vector<Par3* > *pvcToClean3=0){
		m_pv1 = pvcToClean1;
		m_pv2 = pvcToClean2;
		m_pv3 = pvcToClean3;
	}
	~_VectorClearer(void);
private:
	vector<Par1* > *m_pv1;
	vector<Par2* > *m_pv2;
	vector<Par3* > *m_pv3;

};


ofstream &operator << (ofstream &ofs,ClMetaSentence &MetaSnt);


#endif

