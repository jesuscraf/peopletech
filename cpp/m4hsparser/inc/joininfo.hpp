//==============================================================================
//
// (c) Copyright  1991-2000 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4hsparser.dll
// File:                joininfo.hpp
// Project:             mind x.x      
// Author:              Meta Software M.S. , S.A
// Language:            C++
// Operating System:    UNIX, WIN32
// Date:                22/2/01
// Design Document:     
//
//
// Definition:
//
//    Definición de un join en una sentencia select
//
//
//==============================================================================

#if !defined(AFX_JOININFO_HPP__1B222907_1C53_11D5_BEE3_00C04FA913F4__INCLUDED_)
#define AFX_JOININFO_HPP__1B222907_1C53_11D5_BEE3_00C04FA913F4__INCLUDED_


#include "m4stl.hpp"
#include "elementinfo.hpp"
#include "tableinfo.hpp"


typedef map<TableInfo*, TableInfo*, less<TableInfo*> > M4HS_JOIN_MATCHING_T;


class JoinInfo : public ElementInfo
{

public:

	JoinInfo ( m4int_t ai_iTreeLevel);
	virtual ~JoinInfo();

	m4return_t ResolveTables (vector<TableInfo*> &ai_vpoTables);

	m4bool_t operator == (const JoinInfo &ai_oOtherJoin);

	M4HS_JOIN_MATCHING_T& GetMatching ();

	m4void_t SetTreeLevel (m4int_t ai_iTreeLevel);

protected:

	virtual m4void_t MyDump (ostream & ai_oStream);

private:

	m4int_t m_iTreeLevel;

	TableInfo * m_poLeftTable;
	TableInfo * m_poRightTable;

	// Cuando ejecutamos el operador ==, si hay correspondencia guardamos aquí 
	// la información de matching. El primer elemento será el alias de la tabla 
	// y el segundo el alias por el que hay que sustituirla
	M4HS_JOIN_MATCHING_T m_mpcMatching;
};

#endif // !defined(AFX_JOININFO_HPP__1B222907_1C53_11D5_BEE3_00C04FA913F4__INCLUDED_)
