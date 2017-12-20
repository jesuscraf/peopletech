//==============================================================================
//
// (c) Copyright  1991-2000 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4hsparser.dll
// File:                selectinfo.hpp
// Project:             mind x.x      
// Author:              Meta Software M.S. , S.A
// Date:                22/2/01
// Language:            C++
// Operating System:    UNIX, WIN32
// Design Document:     
//
//
// Definition:
//
//    Definición de una select
//
//
//==============================================================================


#if !defined(AFX_SELECTINFO_H__AA42A4C3_F6A4_11D4_BEB8_00C04FA913F4__INCLUDED_)
#define AFX_SELECTINFO_H__AA42A4C3_F6A4_11D4_BEB8_00C04FA913F4__INCLUDED_


#include "m4stl.hpp"
#include "elementinfo.hpp"
#include "columninfo.hpp"
#include "iteminfo.hpp"
#include "tableinfo.hpp"
#include "textinfo.hpp"
#include "functioninfo.hpp"
#include "joininfo.hpp"



class	ClElementFactory ;



class SelectInfo : public ElementInfo
{

public:
	
	SelectInfo( ClElementFactory* ai_poFactory );
	virtual ~SelectInfo();

	m4void_t SetHasDistinct (m4bool_t ai_bHasDistinct);
	m4void_t AddColumn (ElementInfo * ai_poColumnInfo);

	m4void_t AddTable (TableInfo * ai_poTableInfo);

	m4void_t SetWhere (ElementInfo * ai_poWhere);

	m4void_t SetIsSubselect (m4bool_t ai_bIsSubselect);

	m4void_t AddOrderBy (ElementInfo * ai_poOrderBy);

	m4void_t SetGroupBy (ElementInfo * ai_poGroupBy);

	m4void_t SetHaving (ElementInfo * ai_poHaving);

	m4void_t AddUnion (SelectInfo * ai_poUnion);
	
	m4void_t AddIntersect (SelectInfo * ai_poIntersect);

	m4void_t AddJoin (JoinInfo * ai_poJoin);

	m4return_t Merge (SelectInfo * ai_poParent);

protected:

	m4void_t MyDump (ostream & ai_oStream);

	m4void_t MyChangeAlias (m4pcchar_t ai_pccOldAlias, m4pcchar_t ai_pccNewAlias);

private:

	m4void_t RenameAllAlias (m4bool_t ai_bInternal, m4bool_t ai_bReverse);

	m4return_t AddTotalization (m4pcchar_t ai_pccTotalizator);

	m4return_t MergeFrom (SelectInfo * ai_poParent);

	m4return_t ReduceBaseTable (SelectInfo * ai_poParent);

	m4return_t ReduceJoins (SelectInfo * ai_poParent);

	m4void_t ReduceTables (SelectInfo * ai_poParent);

	TableInfo * FindBaseTable ();

	m4return_t ResolveJoinTables ();

	m4void_t DeleteTablesByAlias (vector <m4pcchar_t> & ai_vpccAliasToDelete);

	m4void_t MergeOrderBy (SelectInfo * ai_poParent);

	m4return_t MergeGroupBy (SelectInfo * ai_poParent);

	m4void_t MergeWhere (SelectInfo * ai_poParent);

	m4void_t MergeHaving (SelectInfo * ai_poParent);

	m4void_t MergeSelect (SelectInfo * ai_poParent);

	m4void_t MergeDistinct (SelectInfo * ai_poParent);

	m4void_t MergeUnionIntersect (SelectInfo * ai_poParent);

	// Modificador en las columnas (DISTINCT)
	// ALL no se contempla, no se puede dar en APISQL
	m4bool_t m_bHasDistinct;

	m4bool_t m_bIsSubselect;
	
	// Columnas que se quieren traer en la select
	vector <ElementInfo*>  m_vpoColumns;
	// Tablas
	vector <TableInfo*> m_vpoTables;
		
	// Filtro
	ElementInfo * m_poWhere;
	
	vector <ElementInfo*> m_vpoOrderBy;
	
	ElementInfo * m_poGroupBy;

	ElementInfo * m_poHaving;

	// Union o Intersect
	SelectInfo * m_poUnion;
	SelectInfo * m_poIntersect;

	// Vector de punteros a los joins
	vector <JoinInfo*> m_vpoJoins;

	// Factoría de elementos
	ClElementFactory* m_poFactory ;

};

#endif // !defined(AFX_SELECTINFO_H__AA42A4C3_F6A4_11D4_BEB8_00C04FA913F4__INCLUDED_)
