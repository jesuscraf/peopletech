//==============================================================================
//
// (c) Copyright  1991-2000 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4hsparser.dll
// File:                tableinfo.hpp
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
//    Definición de una tabla en una sentencia select
//
//
//==============================================================================


#if !defined(AFX_TABLEINFO_HPP__98467D44_F8E9_11D4_BEBB_00C04FA913F4__INCLUDED_)
#define AFX_TABLEINFO_HPP__98467D44_F8E9_11D4_BEBB_00C04FA913F4__INCLUDED_


#include "elementinfo.hpp"


class TableInfo : public ElementInfo
{
public:

	TableInfo (m4pcchar_t ai_pccTable, m4pcchar_t ai_pccAlias);
	virtual ~TableInfo();

	m4void_t SetIsBase (m4bool_t ai_bIsBase);
	m4bool_t IsBase ();

	m4pcchar_t GetAlias ();
	m4pcchar_t GetTable ();

	m4void_t SetAlias (m4pcchar_t ai_pccAlias);
	m4void_t SetTable (m4pcchar_t ai_pccTable);

	m4bool_t IsReductible ();
	m4void_t SetReductible (m4bool_t ai_bIsReductible);

	m4bool_t operator == (const TableInfo &ai_oOtherTable) const;

protected:

	virtual m4void_t MyDump (ostream & ai_oStream);

	virtual m4void_t MyChangeAlias (m4pcchar_t ai_pccOldAlias, m4pcchar_t ai_pccNewAlias);

private:

	m4pchar_t m_pcAlias;
	m4pchar_t m_pcTable;

	m4bool_t m_bIsBase;

	m4bool_t m_bIsReductible;

};

#endif // !defined(AFX_TABLEINFO_HPP__98467D44_F8E9_11D4_BEBB_00C04FA913F4__INCLUDED_)
