//==============================================================================
//
// (c) Copyright  1991-2000 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4hsparser.dll
// File:                columninfo.hpp
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
//    Definición de una columna en una sentencia select
//
//
//==============================================================================


#if !defined(AFX_COLUMNINFO_HPP__98467D43_F8E9_11D4_BEBB_00C04FA913F4__INCLUDED_)
#define AFX_COLUMNINFO_HPP__98467D43_F8E9_11D4_BEBB_00C04FA913F4__INCLUDED_


#include "elementinfo.hpp"


class ColumnInfo : public ElementInfo 
{

public:

	ColumnInfo(m4pcchar_t ai_pccAlias, m4pcchar_t ai_pccColumn);
	virtual ~ColumnInfo();

	m4void_t SetHasOuterJoin (m4bool_t ai_bHasOuterJoin);

	m4bool_t HasOuterJoin ();

	m4pcchar_t GetAlias ();
	m4pcchar_t GetColumn ();

	m4bool_t operator == (const ColumnInfo &ai_oOtherColumn) const;

protected:

	virtual m4void_t MyDump (ostream & ai_oStream);

	virtual m4void_t MyChangeAlias (m4pcchar_t ai_pccOldAlias, m4pcchar_t ai_pccNewAlias);

private: 

	// Alias de la tabla
	m4pchar_t m_pcAlias;
	// Nombre de la columna
	m4pchar_t m_pcColumn;

	m4bool_t m_bHasOuterJoin;

};

#endif // !defined(AFX_COLUMNINFO_HPP__98467D43_F8E9_11D4_BEBB_00C04FA913F4__INCLUDED_)
