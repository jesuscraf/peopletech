//==============================================================================
//
//  (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              ldb
// File:                metainfo.hpp
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


#ifndef _METAINFO_HPP_
#define _METAINFO_HPP_

#include "defmain.hpp"
#include "m4stl.hpp"

struct	StMtData;

class	ClGlobVars;
//class	ofstream;


m4cint32_t g_OrgPrecision		= 32;
m4cint32_t g_TramitPrecision	= 50;
m4cint32_t g_OrgLevelPrecision	= 6;
m4cint32_t g_RangePrecision		= 10;


m4pcchar_t ClauseType(eClauseType_t ai_Clause);


enum Const_t
{
	M4LDB_MINUS_INF=1,
	M4LDB_PLUS_INF
};


class _ClMetaBase
{
public :

	union
	{
		m4int32_t	m_iLevel;
		m4int32_t	m_iColumn;
	};

	eClauseType_t	m_eClause;
	
	virtual ~_ClMetaBase()
	{
	}

	virtual void Dump(ofstream &ofs)
	{
	}

	virtual _ClMetaBase *Clone (void)
	{
		return (NULL);
	}

	virtual m4bool_t IsSeparator()
	{
		return M4_FALSE;
	}

	virtual m4bool_t IsAddToOr()
	{
		return M4_FALSE;
	}

	virtual m4bool_t IsBegin()
	{
		return M4_FALSE;
	}

	virtual m4bool_t IsMetaData()
	{
		return M4_TRUE;
	}
	
	
protected :
	_ClMetaBase (m4int32_t ai_LevCol,eClauseType_t ai_eClause)
	{
		m_iLevel=ai_LevCol;
		m_eClause=ai_eClause;
	}	
};


class _ClMetaLogic : public _ClMetaBase
{

public :

	m4int32_t	m_iFilter;
	m4int32_t	m_indLogicTable;
	
	virtual ~_ClMetaLogic()
	{
	}

	virtual void Dump(ofstream &ofs)
	{
	}
	
	
protected :
	_ClMetaLogic (m4int32_t ai_iLevel, eClauseType_t ai_eClause, m4int32_t ai_iFilter, m4int32_t ai_indLogicTable)
		: _ClMetaBase (ai_iLevel,ai_eClause) 
	{
		m_iFilter = ai_iFilter;
		m_indLogicTable = ai_indLogicTable;
	}
	
	
};


class ClMetaField : public _ClMetaLogic
{
public :
	string		m_stField;
	m4int32_t	m_indAlias;
	m4int32_t	m_indRealTable;	// Indice del nodo Real_Objects.
	m4int32_t	m_indRealField;	// Indice del nodo Real_Fields.
	m4int32_t	m_indRealTable2;// Indice del nodo Real_Objects, para la traduccion.
	m4int32_t	m_indRealField2;// Indice del nodo Real_Fields, para la traduccion.
	m4bool_t	m_bIsOuter;
	m4bool_t	m_bIsMultiSoc;  // Para distinguir a la hora de quitar Outer joins. ( Informix ). De momento no se usa.

	
	virtual void Dump(ofstream &ofs);

	ClMetaField (m4int32_t ai_iLevel, eClauseType_t ai_eClause, m4pcchar_t ai_pField, m4int32_t ai_indAlias, m4int32_t ai_iFilter)
		: _ClMetaLogic (ai_iLevel, ai_eClause, ai_iFilter, M4LDB_NULL_INDEX) ,
		m_stField (ai_pField)
	{
		m_indAlias = ai_indAlias;
		m_indRealTable = M4LDB_NULL_INDEX;
		m_indRealField = M4LDB_NULL_INDEX;
		m_indRealTable2 = M4LDB_NULL_INDEX;
		m_indRealField2 = M4LDB_NULL_INDEX;
		m_bIsOuter = M4_FALSE;
		m_bIsMultiSoc = M4_FALSE;
	}

	ClMetaField (ClMetaField &ai_MetaFld)
		:	_ClMetaLogic (ai_MetaFld.m_iLevel, ai_MetaFld.m_eClause, ai_MetaFld.m_iFilter, ai_MetaFld.m_indLogicTable),
			m_stField(ai_MetaFld.m_stField)
	{
		m_indAlias = ai_MetaFld.m_indAlias;
		m_indRealTable = ai_MetaFld.m_indRealTable;
		m_indRealField = ai_MetaFld.m_indRealField;
		m_indRealTable2 = ai_MetaFld.m_indRealTable2;
		m_indRealField2 = ai_MetaFld.m_indRealField2;
		m_bIsOuter = ai_MetaFld.m_bIsOuter;
		m_bIsMultiSoc = ai_MetaFld.m_bIsMultiSoc;
	}

	virtual _ClMetaBase *Clone (void)
	{
		ClMetaField *pFld = new ClMetaField (*this);
		pFld->m_bIsOuter = M4_FALSE;
		return (pFld);
	}

protected :

	ClMetaField (m4int32_t ai_Level, eClauseType_t ai_eClause)
		: _ClMetaLogic (ai_Level, ai_eClause, M4LDB_NULL_INDEX, M4LDB_NULL_INDEX)
	{
		m_indAlias = M4LDB_NULL_INDEX;
		m_indRealTable = M4LDB_NULL_INDEX;
		m_indRealField = M4LDB_NULL_INDEX;
		m_indRealTable2 = M4LDB_NULL_INDEX;
		m_indRealField2 = M4LDB_NULL_INDEX;
		m_bIsOuter = M4_FALSE;
		m_bIsMultiSoc = M4_FALSE;
	}
};


typedef class ClMetaField*					PClMetaField;
typedef vector<PClMetaField>				VcClMtField;
typedef VcClMtField*						PVcClMtField;
typedef VcClMtField::iterator				itVcClMtField;

typedef class ClMetaObject*					PClMetaObject;
typedef vector<PClMetaObject>				VcClMtObject;
typedef VcClMtObject*						PVcClMtObject;
typedef VcClMtObject::iterator				itVcClMtObject;

typedef class ClMetaParam*					PClMetaParam;
typedef vector<PClMetaParam>				VcClMtParam;
typedef VcClMtParam*						PVcClMtParam;
typedef VcClMtParam::iterator				itVcClMtParam;


class ClMetaObject : public _ClMetaLogic
{
public :
	string			m_stObject;
	string			m_sSecurityView;
	PVcClMtParam	m_pvcSecurityParams;
	m4int32_t		m_indAlias;
	eJoinType_t		m_eJoinType;
	m4int32_t		m_MaskRealTables;
	m4int32_t		m_iOrgType;
	m4bool_t		m_bInheritsData;
	m4int32_t		m_iOrgLevel;
	m4uint8_t		m_iMirrorType;
	m4bool_t		m_bIsBasis;
	m4bool_t		m_bHasSocFilter;
	m4bool_t		m_bAddedSocControlFilter;
	m4bool_t		m_bAddedSocFilter;
	m4bool_t		m_bAddedLevelFilter;
	m4bool_t		m_bAddedTramitFilter;
	m4uint8_t		m_nRelationSource;
	//Bug 0081842
	ClMetaObject * m_pParentObject;
	VcClMtObject m_oVcChildObject;
	// Bg 147470
	string			m_sFTTParams;

	ClMetaObject (m4int32_t ai_iLevel, eClauseType_t ai_eClause, m4pcchar_t ai_pObject, m4bool_t ai_bIsBasis, m4int32_t ai_iFilter)
		: _ClMetaLogic (ai_iLevel, ai_eClause, ai_iFilter, M4LDB_NULL_INDEX),
		m_stObject(ai_pObject)
	{
		m_pvcSecurityParams = NULL;
		m_indAlias = M4LDB_NULL_INDEX;
		m_bIsBasis = ai_bIsBasis;
		m_eJoinType = M4LDB_NORMAL_JOIN;
		m_MaskRealTables = 0;
		if (m_bIsBasis)
		{
			NotifyUsingRealTable(0);
		}
		else{
			m_MaskRealTables = 0;
		}
		m_iOrgType = M4CL_ORGANIZATION_TYPE_MONO;
		m_bInheritsData = M4_FALSE;
		m_iOrgLevel = 1;
		m_iMirrorType = 0;
		m_bHasSocFilter = M4_FALSE;
		m_bAddedSocControlFilter = M4_FALSE;
		m_bAddedSocFilter = M4_FALSE;
		m_bAddedLevelFilter = M4_FALSE;
		m_bAddedTramitFilter = M4_FALSE;
		m_nRelationSource = M4LDB_APISQL_FROM_APISQL;
		m_pParentObject = NULL;
	}

	~ClMetaObject(void)
	{
		_CleanSecurityParams();
	}

	void CleanSecurity(void);
	void SetSecurityParams(PVcClMtParam ai_pcvParams);

	virtual void Dump(ofstream &ofs);

	m4bool_t IsUsingRealTable(m4int32_t ai_indRealTable)
	{
		m4int32_t  RealMask = m_MaskRealTables;
		RealMask &= 1 << ai_indRealTable;
		return ( RealMask ? M4_TRUE : M4_FALSE );
	}

	void NotifyUsingRealTable(m4int32_t ai_indRealTable)
	{
		m_MaskRealTables |= (1 << ai_indRealTable);
	}

	m4uint8_t GetAPISQLSource( void )
	{
		return m_nRelationSource;
	}

	void SetAPISQLSource(m4uint8_t ai_nRelationSource)
	{
		m_nRelationSource = ai_nRelationSource;
	}

	m4return_t AddChildObject( ClMetaObject * ai_pChildObject ) 
	{
		m_oVcChildObject.push_back( ai_pChildObject );
		return M4_SUCCESS;
	}

	m4bool_t MetaObjectIntoInheritanceChain( const ClMetaObject * ai_pDestinationObject );

protected:

	void _CleanSecurityParams(void);
};


class ClMetaAlias : public _ClMetaLogic
{
public:
	string		m_stAlias;
	m4int32_t	m_indObj;
	m4bool_t	m_bReduce;
	m4int32_t	m_indNewAlias;
	
	ClMetaAlias (m4int32_t ai_iLevel, eClauseType_t ai_eClause, m4pcchar_t ai_pAlias, m4int32_t ai_iFilter)
		: _ClMetaLogic (ai_iLevel, ai_eClause, ai_iFilter, M4LDB_NULL_INDEX)
	{
		m_stAlias = ai_pAlias;
		m_indObj = M4LDB_NULL_INDEX;
		m_bReduce = M4_FALSE;
		m_indNewAlias = M4LDB_NULL_INDEX;
	}
};


typedef class ClMetaAlias*					PClMetaAlias;
typedef vector<PClMetaAlias>				VcClMtAlias;
typedef VcClMtAlias*						PVcClMtAlias;
typedef VcClMtAlias::iterator				itVcClMtAlias;


class ClMetaRelat :public _ClMetaBase
{
public:
	PClMetaField		m_pMtField1;
	PClMetaField		m_pMtField2;
	eJoinType_t			m_eJoinType;
	m4bool_t			m_bReduce;
	//Bug 0077372, 0077633
	m4bool_t			m_bIsDummy;
	
	//<<<Pedro
	//Añadimos un miembro que indica de donde procede la relacion.	
	m4uint8_t			m_nRelationSource;	
	//>>>Pedro

	virtual void Dump(ofstream &ofs);
	
	//El constructor tiene un argumento por defecto que es el origen de la relación.
	//En principio puede ser por API o por seguridad, por defecto tomamos API.
	ClMetaRelat (m4int32_t ai_Level,eClauseType_t ai_eClause,PClMetaField ai_pFld1,PClMetaField ai_pFld2,eJoinType_t ai_eJoinType)
		:	_ClMetaBase (ai_Level,ai_eClause)
	{
		m_pMtField1 =ai_pFld1;
		m_pMtField2 =ai_pFld2;
		m_eJoinType = ai_eJoinType;
		m_bReduce = M4_FALSE;			
		//Bug 0077372, 0077633
		m_bIsDummy = M4_FALSE;
		//<<<Pedro
		//Inicialmente la relacion correspondera a un APISQL que no viene de seguridad.
		m_nRelationSource = M4LDB_APISQL_FROM_APISQL;		
		//Pedro>>>
	}

	//<<<Pedro
	//Funciones que establecen/devuelven el origen del API
	m4uint8_t GetAPISQLSource( void )
	{
		return m_nRelationSource;
	}

	void SetAPISQLSource (m4uint8_t ai_nRelationSource)
	{ 
		m_nRelationSource = ai_nRelationSource;
	}
	//>>>Pedro

	~ClMetaRelat (void)
	{
		if (m_pMtField1)
		{
			delete m_pMtField1;
			m_pMtField1=0;
		}
		if (m_pMtField2)
		{
			delete m_pMtField2;
			m_pMtField2=0;
		}
	}
	
protected :
	ClMetaRelat (m4int32_t ai_Level,eClauseType_t ai_eClause)
		: _ClMetaBase (ai_Level,ai_eClause)
	{
		m_pMtField1 =0;
		m_pMtField2 =0;
	}
};


typedef class ClMetaRelat*					PClMetaRelat;
typedef vector<PClMetaRelat>				VcClMtRelat;
typedef VcClMtRelat*						PVcClMtRelat;
typedef VcClMtRelat::iterator				itVcClMtRelat;

// Parametros.
class ClMetaParam : public _ClMetaBase
{
public:
	virtual eParamType_t	GetType() = 0;
	virtual void Dump(ofstream &ofs);
	ClMetaParam (m4int32_t ai_iLevel,eClauseType_t ai_eClause)
		:	_ClMetaBase (ai_iLevel,ai_eClause)
	{
	}

	ClMetaParam (ClMetaParam &ai_MetaParam)
		:	_ClMetaBase (ai_MetaParam.m_iLevel,ai_MetaParam.m_eClause)
	{
	}

	virtual m4bool_t	IsExtern ()
	{
		return M4_FALSE;
	}

	virtual m4int8_t	GetM4Type	(ClGlobVars *ai_pGlbVars)
	{
		return 0;
	}

	virtual m4int32_t	GetPrecision(ClGlobVars *ai_pGlbVars)
	{
		return 0;
	}

	virtual m4int32_t	GetScale	(ClGlobVars *ai_pGlbVars)
	{
		return 0;
	}

	virtual m4int32_t	GetHandle	(ClGlobVars *ai_pGlbVars)
	{
		return 0;
	}

	virtual m4int32_t	GetOrgLevel	(ClGlobVars *ai_pGlbVars)
	{
		return 0;
	}

	virtual m4bool_t	GetInheritsData	(ClGlobVars *ai_pGlbVars)
	{
		return M4_FALSE;
	}
	
protected :
};



class ClMetaItemCol : public ClMetaParam
{
public:
	m4int32_t		m_hItem;
	m4int32_t		m_lPos;
	m4int32_t		m_lLength;
	m4bool_t		m_bValidateType;

	virtual eParamType_t	GetType()
	{
		return M4LDB_PAR_UNDEF;
	}

	virtual void Dump(ofstream &ofs);

	ClMetaItemCol	(m4int32_t ai_iColumn,eClauseType_t ai_eClause, m4int32_t ai_hItem)
		:	ClMetaParam (ai_iColumn,ai_eClause)
	{
		m_hItem = ai_hItem;
		m_lPos = 0;
		m_lLength = 0;
		m_bValidateType = M4_TRUE;
	}

	ClMetaItemCol(ClMetaItemCol	&ai_MetaItemCol)
		:	ClMetaParam (ai_MetaItemCol)
	{
		m_hItem = ai_MetaItemCol.m_hItem;
		m_lPos = ai_MetaItemCol.m_lPos;
		m_lLength = ai_MetaItemCol.m_lLength ;
		m_bValidateType = ai_MetaItemCol.m_bValidateType;
	}

	virtual _ClMetaBase *Clone (void)
	{
		return (new ClMetaItemCol (*this));
	}
	
protected :
	ClMetaItemCol (m4int32_t ai_Level,eClauseType_t ai_eClause)
		: ClMetaParam (ai_Level,ai_eClause)
	{
		m_hItem = 0;
		m_lPos = 0;
		m_lLength = 0;
		m_bValidateType = M4_TRUE;
	}
};


typedef class ClMetaItemCol*				PClMetaItemCol;
typedef vector<PClMetaItemCol>				VcClMtColumn;
typedef VcClMtColumn*						PVcClMtColumn;
typedef VcClMtColumn::iterator				itVcClMtColumn;

class ClMetaItem : public ClMetaParam
{
public:
	m4int32_t	m_hItem;

	virtual eParamType_t	GetType()
	{
		return M4LDB_PAR_ITEM;
	}
	
	ClMetaItem	(m4int32_t ai_iLevel,eClauseType_t ai_eClause, m4int32_t ai_hItem)
		:	ClMetaParam (ai_iLevel,ai_eClause)
	{
		m_hItem = ai_hItem;
	}

	ClMetaItem	(ClMetaItem	&ai_MetaItem)
		:	ClMetaParam (ai_MetaItem)
	{
		m_hItem = ai_MetaItem.m_hItem;
	}

	virtual _ClMetaBase *Clone (void)
	{
		return (new ClMetaItem (*this));
	}

	virtual m4int8_t	GetM4Type	(ClGlobVars *ai_pGlbVars);
	virtual m4int32_t	GetPrecision(ClGlobVars *ai_pGlbVars);
	virtual m4int32_t	GetScale	(ClGlobVars *ai_pGlbVars);
	virtual m4int32_t	GetHandle	(ClGlobVars *ai_pGlbVars)
	{
		return m_hItem;
	}
};


class ClMetaItemSess : public ClMetaItem
{
public:
	ClMetaItemSess 	(m4int32_t ai_iLevel,eClauseType_t ai_eClause, m4int32_t ai_hItem)
		:	ClMetaItem (ai_iLevel, ai_eClause, ai_hItem)
	{
	}

	ClMetaItemSess 	(ClMetaItemSess &ai_MetaItemSess)
		:	ClMetaItem(ai_MetaItemSess)
	{
	}

	virtual _ClMetaBase *Clone (void)
	{
		return (new ClMetaItemSess  (*this));
	}

	virtual eParamType_t	GetType()
	{
		return M4LDB_PAR_SESSION;
	}

	virtual m4int8_t	GetM4Type	(ClGlobVars *ai_pGlbVars);
	virtual m4int32_t	GetPrecision(ClGlobVars *ai_pGlbVars);
	virtual m4int32_t	GetScale	(ClGlobVars *ai_pGlbVars);
};


class ClMetaExternPar : public ClMetaParam
{
public:
	m4int8_t	m_m4Type;
	m4int32_t	m_lPrec;
	m4int32_t	m_lScale;
	m4int32_t	m_iParIndex;
public:
	
	virtual eParamType_t	GetType()
	{
		return M4LDB_PAR_EXTERN;
	}

	/* Bug 0110437
	La precisión y la escala estaban cambiados
	ClMetaExternPar (m4int32_t ai_iLevel,eClauseType_t ai_eClause, m4int8_t ai_m4Type, m4int32_t ai_Prec, m4int32_t ai_Scale)
	*/
	ClMetaExternPar (m4int32_t ai_iLevel,eClauseType_t ai_eClause, m4int8_t ai_m4Type,m4int32_t ai_Scale,m4int32_t ai_Prec)
		:	ClMetaParam (ai_iLevel,ai_eClause)
	{
		m_m4Type = ai_m4Type;
		m_lPrec = ai_Prec;
		m_lScale = ai_Scale;
		m_iParIndex = M4LDB_NULL_INDEX;
	}

	ClMetaExternPar (ClMetaExternPar &ai_MetaExt)
		:	ClMetaParam(ai_MetaExt)
	{
		m_m4Type = ai_MetaExt.m_m4Type;
		m_lPrec = ai_MetaExt.m_lPrec;
		m_lScale = ai_MetaExt.m_lScale ;
		m_iParIndex = ai_MetaExt.m_iParIndex;
	}

	virtual _ClMetaBase *Clone (void)
	{
		return (new ClMetaExternPar (*this));
	}

	virtual m4bool_t	IsExtern ()
	{
		return M4_TRUE;
	}

	virtual m4int8_t	GetM4Type	(ClGlobVars *ai_pGlbVars)
	{
		/* Bug 0126267
		Ahora ya se devuelve el tipo real, y no se cablea a cadena para los long, binarios y blob
		*/
		return m_m4Type;
	}

	virtual m4int32_t	GetPrecision(ClGlobVars *ai_pGlbVars)
	{
		/* Bug 0126267
		Ahora ya se devuelve la precisión real, y no se cablea a 255 para los long, binarios y blob
		*/
		return m_lPrec;
	}

	virtual m4int32_t	GetScale	(ClGlobVars *ai_pGlbVars)
	{
		return m_lScale;
	}

	virtual m4int32_t	GetHandle	(ClGlobVars *ai_pGlbVars)
	{
		return m_iParIndex;
	}
};


class ClMetaInternPar : public ClMetaParam
{
public:
	m4int32_t	m_indParam;

	ClMetaInternPar 	(m4int32_t ai_iLevel,eClauseType_t ai_eClause,m4int32_t ai_indParam)
		:	ClMetaParam (ai_iLevel,ai_eClause)
	{
		m_indParam = ai_indParam;
	}

	ClMetaInternPar (ClMetaInternPar &ai_MetaInt)
		:	ClMetaParam (ai_MetaInt)
	{
		m_indParam = ai_MetaInt.m_indParam;
	}

	virtual _ClMetaBase *Clone (void)
	{
		return (new ClMetaInternPar (*this));
	}

	virtual eParamType_t	GetType()
	{
		return M4LDB_PAR_INTERN;
	}

	virtual m4int32_t	GetHandle	(ClGlobVars *ai_pGlbVars)
	{
		return m_indParam;
	}

};


class ClMetaConst : public ClMetaParam
{
public:
	Const_t		m_const;

	virtual eParamType_t	GetType()
	{
		if (m_const==M4LDB_MINUS_INF)
		{
			return M4LDB_PAR_MINUS_INF;
		}
		else
		{
			if (m_const==M4LDB_PLUS_INF)
			{
				return M4LDB_PAR_PLUS_INF;
			}
			else
			{
				return M4LDB_PAR_UNDEF;
			}
		}
	}

	ClMetaConst (ClMetaConst &ai_MetaConst)
		:	ClMetaParam (ai_MetaConst)
	{
		m_const = ai_MetaConst.m_const;
	}
	
	ClMetaConst(m4int32_t ai_iLevel,eClauseType_t ai_eClause,Const_t ai_const)
		:	ClMetaParam (ai_iLevel,ai_eClause)
	{
		m_const = ai_const;
	}

	virtual _ClMetaBase *Clone (void)
	{
		return (new ClMetaConst (*this));
	}
	
	virtual m4int8_t	GetM4Type	(ClGlobVars *ai_pGlbVars)
	{
		return M4CL_M4_TYPE_TIMESTAMP;
	}

	virtual m4int32_t	GetPrecision(ClGlobVars *ai_pGlbVars)
	{
		return M4LDB_SIZE_TIMESTAMP;
	}
};


class ClMetaAutoFilter : public ClMetaParam
{
public:
	m4int32_t	m_iIntID;
	m4int8_t	m_m4Type;
	m4int32_t	m_iOrgLevel;
	m4bool_t	m_bInheritsData;
	
	ClMetaAutoFilter 	(m4int32_t ai_iLevel, eClauseType_t ai_eClause, m4int32_t ai_iIntID, m4int8_t ai_m4Type, m4int32_t a_iOrgLevel, m4bool_t ai_bInheritsData)
		:	ClMetaParam (ai_iLevel,ai_eClause)
	{
		m_iIntID = ai_iIntID;
		m_m4Type = ai_m4Type;
		m_iOrgLevel = a_iOrgLevel;
		m_bInheritsData = ai_bInheritsData;
	}

	ClMetaAutoFilter (ClMetaAutoFilter &ai_MetaAuto)
		:	ClMetaParam (ai_MetaAuto)
	{
		m_iIntID = ai_MetaAuto.m_iIntID;
		m_m4Type = ai_MetaAuto.m_m4Type;
		m_iOrgLevel = ai_MetaAuto.m_iOrgLevel;
		m_bInheritsData = ai_MetaAuto.m_bInheritsData;
	}

	virtual _ClMetaBase *Clone (void)
	{
		return (new ClMetaAutoFilter (*this));
	}

	virtual eParamType_t	GetType()
	{
		return M4LDB_PAR_AUTO_FILTER;
	}

	virtual m4int8_t	GetM4Type	(ClGlobVars *ai_pGlbVars)
	{
		return m_m4Type;
	}

	virtual m4int32_t	GetPrecision(ClGlobVars *ai_pGlbVars)
	{
		if (m_iIntID == M4CL_INTERNAL_TYPE_ID_ORGANIZATION)
		{
			return g_OrgPrecision;
		}
		
		if( m_iIntID == M4CL_INTERNAL_TYPE_ID_DATASET )
		{
			return g_TramitPrecision;
		}
		
		if (m_iIntID == M4CL_INTERNAL_TYPE_ID_ORGANIZATION_LEVEL)
		{
			return g_OrgLevelPrecision;
		}

		if (m_iIntID == M4CL_INTERNAL_TYPE_RANGE_START || m_iIntID == M4CL_INTERNAL_TYPE_RANGE_END)
		{
			return g_RangePrecision;
		}

		if (m_m4Type == M4CL_M4_TYPE_DATE )
		{
			return M4LDB_SIZE_DATE;
		}

		return M4LDB_SIZE_TIMESTAMP;
	}

	virtual m4int32_t	GetHandle	(ClGlobVars *ai_pGlbVars)
	{
		return m_iIntID;
	}

	virtual m4int32_t	GetOrgLevel	(ClGlobVars *ai_pGlbVars)
	{
		return m_iOrgLevel;
	}

	virtual m4bool_t	GetInheritsData	(ClGlobVars *ai_pGlbVars)
	{
		return m_bInheritsData;
	}
};


template <class T>
class ClSeparator : public T
{
public:
	virtual m4bool_t IsSeparator()
	{
		return M4_TRUE;
	}

	virtual m4bool_t IsMetaData()
	{
		return M4_FALSE;
	}

	virtual eParamType_t	GetType()
	{
		return M4LDB_PAR_UNDEF;
	}

	ClSeparator (m4int32_t ai_iLevel,eClauseType_t ai_eClause)
		: T(ai_iLevel,ai_eClause)
	{
		
	}
	ClSeparator (ClSeparator<T>	&ai_Sep)
		:	T(ai_Sep)
	{
	}
	
	virtual _ClMetaBase *Clone (void)
	{ 
		return (new ClSeparator<T> (*this));
	}
	
	virtual void Dump(ofstream &ofs);

};


template <class T>
ofstream &operator << (ofstream &ofs,ClSeparator<T> *ai_Sep);


typedef ClSeparator<ClMetaRelat>				ClMetaRelat_Sep;
typedef ClSeparator<ClMetaField>				ClMetaField_Sep ;
typedef ClSeparator<ClMetaParam>				ClMetaParam_Sep  ;
typedef ClSeparator<ClMetaItemCol>				ClMetaItemCol_Sep  ;
typedef ClSeparator<ClMetaObject>				ClMetaObject_Sep  ;

typedef map<string, ClMetaParam *, less<string> >		MpFieldAndValue;
typedef MpFieldAndValue*								PMpFieldAndValue;
typedef MpFieldAndValue::const_iterator					itMpFieldAndValue;


template <class T>
void
ClSeparator<T>::Dump(ofstream &ofs)
{
	ofs << "\t"<<ClauseType(m_eClause)<<" Separator. Level :"<<m_iLevel;
}


// iterador sobre los vectores.
// Begin,me devuelve en el primero que no sea separador. Si no hay me devuekve End().
// End,	 me devuelve el end del vector, sea o no separador.
// Next, me devuelve el siguiente que no sea separador. Si no hay me devuelve End().

// Al igual que los iteradores normales, puede quedarse invalido si hay modificaciones en el vector.
template <class T>
class ClMetaIterator {
public:
	ClMetaIterator(vector<T*> * ai_pvcT)
	{
		m_pvcT = ai_pvcT;
		m_itT = ai_pvcT->begin();
		m_itTend = ai_pvcT->end();
	}
	T*	Begin(){
		
		m_itT=m_pvcT->begin();
		m_itTend=m_pvcT->end();
		while (m_itT!=m_itTend)
		{
			if ((*m_itT)->IsMetaData()==M4_TRUE)
			{
				break;
			}
			m_itT++;
		}
		return (*m_itT);
	}

	T*	End()
	{
		m_itTend=m_pvcT->end();
		return (*m_itTend);
	}

	T*	Next()
	{
		while (m_itT!=m_itTend)
		{
			m_itT++;
			if ((*m_itT)->IsMetaData()==M4_TRUE)
			{
				break;
			}
		}
		return (*m_itT);
	}

private:
	M4_TYPE_ITERATOR vector<T*>::iterator	m_itT;
	M4_TYPE_ITERATOR vector<T*>::iterator	m_itTend;
	vector<T*>	*			m_pvcT;

};


typedef	ClMetaIterator<ClMetaObject>			IteratorMtObject;
typedef	ClMetaIterator<ClMetaRelat>				IteratorMtRelat;
typedef	ClMetaIterator<ClMetaAlias>				IteratorMtAlias;
typedef	ClMetaIterator<ClMetaField>				IteratorMtField;


template <class T>
class ClAddToOr : public T
{
	m4bool_t m_bIsBegin;
public:
	virtual m4bool_t IsAddToOr()
	{
		return M4_TRUE;
	}

	virtual eParamType_t	GetType()
	{
		return M4LDB_PAR_UNDEF;
	}

	virtual m4bool_t IsBegin()
	{
		return m_bIsBegin;
	}

	virtual m4bool_t IsMetaData()
	{
		return M4_FALSE;
	}

	ClAddToOr (m4int32_t ai_iLevel,eClauseType_t ai_eClause,m4bool_t ai_bIsBegin)
		: T(ai_iLevel,ai_eClause)
	{
		m_bIsBegin = ai_bIsBegin;		
	}

	ClAddToOr (ClAddToOr<T>	&ai_AddToOr)
		:	T(ai_AddToOr)
	{
	}
	
	virtual _ClMetaBase *Clone (void)
	{ 
		return (new ClAddToOr<T> (*this));
	}
};

typedef ClAddToOr<ClMetaField>				ClMetaField_AddToOr ;
typedef ClAddToOr<ClMetaParam>				ClMetaParam_AddToOr ;

#endif
