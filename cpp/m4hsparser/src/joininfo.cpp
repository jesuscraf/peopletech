//==============================================================================
//
// (c) Copyright  1991-2000 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4hsparser.dll
// File:                joininfo.cpp
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
//    Definición de un join en una sentencia select
//
//
//==============================================================================


#include "joininfo.hpp"
#include "columninfo.hpp"
#include "m4hsparser_errors.hpp"

#include "basiclog.hpp"

JoinInfo::JoinInfo (m4int_t ai_TreeLevel)
{
	m_iTreeLevel = ai_TreeLevel;

	m_poLeftTable = 0;
	m_poRightTable = 0;

	m_eType = JOININFO;
}


JoinInfo::~JoinInfo()
{
}


m4void_t JoinInfo::MyDump (ostream & ai_oStream)
{
	ai_oStream << " ";
	ai_oStream << "=";
}


m4return_t JoinInfo::ResolveTables ( vector<TableInfo*> &ai_vpoTables)
{
	for (m4uint_t i = 0; i < ai_vpoTables.size(); i++)
	{
		if (!strcmp ( ((ColumnInfo*)m_poElementInfoLeft)->GetAlias(), ai_vpoTables[i]->GetAlias()) )
			m_poLeftTable = ai_vpoTables[i];
		
		if (!strcmp ( ((ColumnInfo*)m_poElementInfoRight)->GetAlias(), ai_vpoTables[i]->GetAlias()) )
			m_poRightTable = ai_vpoTables[i];
	}

	if (m_poLeftTable == 0)
	{
		// Error no se ha encontrado la tabla, es imposible
		BL_ERRORF (M4HSPARSER_E_RESOLVEJOIN, ((ColumnInfo*)m_poElementInfoLeft)->GetAlias());
		return M4_ERROR;
	}

	// Como la tabla pertenece a un join ya no es reducible
	m_poLeftTable->SetReductible (M4_FALSE);

	if (m_poRightTable == 0)
	{
		// Error no se ha encontrado la tabla, es imposible
		BL_ERRORF (M4HSPARSER_E_RESOLVEJOIN, ((ColumnInfo*)m_poElementInfoRight)->GetAlias());
		return M4_ERROR;
	}

	// Como la tabla pertenece a un join ya no es reducible
	m_poRightTable->SetReductible (M4_FALSE);

	return M4_SUCCESS;
}


m4bool_t JoinInfo::operator == (const JoinInfo &ai_oOtherJoin)
{

	// Comprobamos so los joins están al mismo nivel
	if (m_iTreeLevel != ai_oOtherJoin.m_iTreeLevel)
		return M4_FALSE;

	// Comprobamos si las tablas son iguales
	if ( (*m_poLeftTable == *(ai_oOtherJoin.m_poLeftTable))
		&& (*m_poRightTable == *(ai_oOtherJoin.m_poRightTable)) )
		
			// Las tablas son las mismas comprobemos los columnas
			if ( ( *((ColumnInfo*)m_poElementInfoLeft) == *((ColumnInfo*)ai_oOtherJoin.m_poElementInfoLeft) ) 
				&& ( *((ColumnInfo*)m_poElementInfoRight) == *((ColumnInfo*)ai_oOtherJoin.m_poElementInfoRight) ) )
			{
				// Anotamos el matching
				m_mpcMatching.insert (M4HS_JOIN_MATCHING_T::value_type( m_poLeftTable, ai_oOtherJoin.m_poLeftTable));
				m_mpcMatching.insert (M4HS_JOIN_MATCHING_T::value_type(m_poRightTable, ai_oOtherJoin.m_poRightTable));
					
				return M4_TRUE;
			}

	// Comprobamos si las tablas son iguales
	if ( (*m_poLeftTable == *(ai_oOtherJoin.m_poRightTable))
		&& (*m_poRightTable == *(ai_oOtherJoin.m_poLeftTable)) )
		
			// Las tablas son las mismas comprobemos los columnas
			if ( ( *((ColumnInfo*)m_poElementInfoLeft) == *((ColumnInfo*)ai_oOtherJoin.m_poElementInfoRight) )
				&& ( *((ColumnInfo*)m_poElementInfoRight) == *((ColumnInfo*)ai_oOtherJoin.m_poElementInfoLeft) ) )
			{
				// Anotamos el matching que hemos encontrado
				m_mpcMatching.insert (M4HS_JOIN_MATCHING_T::value_type(m_poLeftTable, ai_oOtherJoin.m_poRightTable));
				m_mpcMatching.insert (M4HS_JOIN_MATCHING_T::value_type(m_poRightTable, ai_oOtherJoin.m_poLeftTable));
			
				return M4_TRUE;
			}
	
	return M4_FALSE;
}


M4HS_JOIN_MATCHING_T& JoinInfo::GetMatching ()
{
	return m_mpcMatching;
}


m4void_t JoinInfo::SetTreeLevel (m4int_t ai_iTreeLevel)
{
	m_iTreeLevel = ai_iTreeLevel;
}
