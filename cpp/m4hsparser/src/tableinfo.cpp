//==============================================================================
//
// (c) Copyright  1991-2000 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4hsparser.dll
// File:                tableinfo.cpp
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

#include "tableinfo.hpp"
#include <malloc.h>
#include <stdlib.h>


TableInfo::TableInfo(m4pcchar_t ai_pccTable, m4pcchar_t ai_pccAlias)
{
	m_bIsBase = M4_FALSE;

	m_pcTable = strdup (ai_pccTable);
	m_pcAlias = strdup (ai_pccAlias);

	m_bIsReductible = M4_TRUE;

	m_eType = TABLEINFO;
}


TableInfo::~TableInfo()
{
	free( m_pcTable ) ;
	free( m_pcAlias ) ;
}


m4void_t TableInfo::SetIsBase (m4bool_t ai_bIsBase)
{
	m_bIsBase = ai_bIsBase;
}


m4bool_t TableInfo::IsBase ()
{
	return m_bIsBase;
}


m4void_t TableInfo::MyDump (ostream & ai_oStream)
{
	ai_oStream << " ";

	if (m_bIsBase)
		ai_oStream << "&";

	ai_oStream << m_pcTable;

	if (m_pcAlias != 0)
		ai_oStream << " " << m_pcAlias;
}


m4void_t TableInfo::MyChangeAlias (m4pcchar_t ai_pccOldAlias, m4pcchar_t ai_pccNewAlias)
{
	if (!strcmp (m_pcAlias, ai_pccOldAlias))
			SetAlias (ai_pccNewAlias);
}


m4pcchar_t TableInfo::GetAlias ()
{
	return m_pcAlias;
}


m4pcchar_t TableInfo::GetTable ()
{
	return m_pcTable;
}


m4void_t TableInfo::SetAlias (m4pcchar_t ai_pccAlias)
{
	free( m_pcAlias );
	m_pcAlias = strdup (ai_pccAlias);
}


m4void_t TableInfo::SetTable (m4pcchar_t ai_pccTable)
{
	free( m_pcTable );
	m_pcTable = strdup (ai_pccTable);
}


m4bool_t TableInfo::IsReductible ()
{
	return m_bIsReductible;
}


m4void_t TableInfo::SetReductible (m4bool_t ai_bIsReductible)
{
	m_bIsReductible = ai_bIsReductible;
}


m4bool_t TableInfo::operator == (const TableInfo &ai_oOtherTable) const
{
	if (!strcmp(m_pcTable, ai_oOtherTable.m_pcTable)) 
		return M4_TRUE;
	else
		return M4_FALSE;
}
