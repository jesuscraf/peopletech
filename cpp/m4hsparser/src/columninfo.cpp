//==============================================================================
//
// (c) Copyright  1991-2000 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4hsparser.dll
// File:                columninfo.cpp
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

#include "columninfo.hpp"
#include <malloc.h>
#include <stdlib.h>


ColumnInfo::ColumnInfo(m4pcchar_t ai_pccAlias, m4pcchar_t ai_pccColumn)
{
	m_pcAlias = strdup (ai_pccAlias);
	m_pcColumn = strdup (ai_pccColumn);

	m_eType = COLUMNINFO;

	m_bHasOuterJoin = M4_FALSE;
}


ColumnInfo::~ColumnInfo()
{
	free( m_pcAlias ) ;
	free( m_pcColumn ) ;
}


m4void_t ColumnInfo::MyDump (ostream & ai_oStream)
{
	ai_oStream << " ";
	if (strcmp(m_pcAlias, ""))
		ai_oStream << m_pcAlias << ".";

	ai_oStream << m_pcColumn;

	if (m_bHasOuterJoin)
		ai_oStream << " (+)";

}


m4void_t ColumnInfo::MyChangeAlias (m4pcchar_t ai_pccOldAlias, m4pcchar_t ai_pccNewAlias)
{
	if (!strcmp (m_pcAlias, ai_pccOldAlias))
	{
		free( m_pcAlias );
		m_pcAlias =  strdup (ai_pccNewAlias);
	}
}


m4void_t ColumnInfo::SetHasOuterJoin (m4bool_t ai_bHasOuterJoin)
{
	m_bHasOuterJoin = ai_bHasOuterJoin;
}


m4bool_t ColumnInfo::HasOuterJoin ()
{
	return m_bHasOuterJoin;
}


m4pcchar_t ColumnInfo::GetAlias ()
{
	return m_pcAlias;
}


m4pcchar_t ColumnInfo::GetColumn ()
{
	return m_pcColumn;
}

m4bool_t ColumnInfo::operator == (const ColumnInfo &ai_oOtherColumn) const
{
	// Comprobamos si las columnas tienen el mismo nombre y el mismo outer join
	if ( (!strcmp (m_pcColumn, ai_oOtherColumn.m_pcColumn))
		&& (m_bHasOuterJoin == ai_oOtherColumn.m_bHasOuterJoin) )
	
		return M4_TRUE;
	else
		return M4_FALSE;
}