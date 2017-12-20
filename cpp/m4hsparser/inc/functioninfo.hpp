//==============================================================================
//
// (c) Copyright  1991-2000 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4hsparser.dll
// File:                functioninfo.hpp
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
//    Definición de una función en una sentencia select
//
//
//==============================================================================


#if !defined(AFX_FUNCTIONINFO_HPP__A43268E3_00C7_11D5_BEC3_00C04FA913F4__INCLUDED_)
#define AFX_FUNCTIONINFO_HPP__A43268E3_00C7_11D5_BEC3_00C04FA913F4__INCLUDED_


#include "elementinfo.hpp"


class FunctionInfo : public ElementInfo
{

public:

	FunctionInfo (m4pcchar_t ai_pccName);
	virtual ~FunctionInfo();

	m4void_t SetArgument ( ElementInfo * ai_poArgument );

protected:

	m4void_t MyDump (ostream & ai_oStream);

	m4void_t MyChangeAlias (m4pcchar_t ai_pccOldAlias, m4pcchar_t ai_pccNewAlias);

private:

	ElementInfo * m_poArgument;
	m4pchar_t m_pcName;

};

#endif // !defined(AFX_FUNCTIONINFO_HPP__A43268E3_00C7_11D5_BEC3_00C04FA913F4__INCLUDED_)
