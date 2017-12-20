
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              elementfactory.dll
// File:                m4mdrt.cpp   
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                21-02-2002
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Esta clase representa una factoría de objetos de tipo
//    ElementInfo y sus derivados.
//    El objetivo es que sea la encargada de crear y destruir esos objetos,
//    de forma que no se produzcan pérdidas de memoria
//
//
//==============================================================================



#include "elementfactory.hpp"

#include "elementinfo.hpp"
#include "columninfo.hpp"
#include "functioninfo.hpp"
#include "iteminfo.hpp"
#include "joininfo.hpp"
#include "selectinfo.hpp"
#include "tableinfo.hpp"
#include "textinfo.hpp"



// Tamaño inicial del array de elementos
#define M4_HSPARSER_ELEMENTS_SIZE				500



ClElementFactory :: ClElementFactory( void )
{
	m_vpoElements.reserve( M4_HSPARSER_ELEMENTS_SIZE ) ;
}


ClElementFactory :: ~ClElementFactory( void )
{

	m4uint32_t	i = 0 ;
	size_t		iLength = 0 ;
	ElementInfo	*poElement = NULL ;


	iLength = m_vpoElements.size() ;

	for( i = 0 ; i < iLength ; i++ )
	{
		poElement = m_vpoElements[ i ] ;

		if( poElement != NULL )
		{
			delete( poElement ) ;
			poElement = NULL ;
		}
	}
}



ColumnInfo*	ClElementFactory :: CreateColumnInfo( m4pcchar_t ai_pccAlias, m4pcchar_t ai_pccColumn )
{

	ColumnInfo	*poReturn = NULL ;


	poReturn = new ColumnInfo( ai_pccAlias, ai_pccColumn ) ;
	m_vpoElements.push_back( poReturn ) ;

	return( poReturn ) ;
}


FunctionInfo*	ClElementFactory :: CreateFunctionInfo( m4pcchar_t ai_pccName )
{

	FunctionInfo	*poReturn = NULL ;


	poReturn = new FunctionInfo( ai_pccName ) ;
	m_vpoElements.push_back( poReturn ) ;

	return( poReturn ) ;
}


ItemInfo*	ClElementFactory :: CreateItemInfo( m4pcchar_t pccItemName )
{

	ItemInfo	*poReturn = NULL ;


	poReturn = new ItemInfo( pccItemName ) ;
	m_vpoElements.push_back( poReturn ) ;

	return( poReturn ) ;
}


JoinInfo*	ClElementFactory :: CreateJoinInfo( m4int_t ai_iTreeLevel )
{

	JoinInfo	*poReturn = NULL ;


	poReturn = new JoinInfo( ai_iTreeLevel ) ;
	m_vpoElements.push_back( poReturn ) ;

	return( poReturn ) ;
}


SelectInfo*	ClElementFactory :: CreateSelectInfo( void )
{

	SelectInfo	*poReturn = NULL ;


	poReturn = new SelectInfo( this ) ;
	m_vpoElements.push_back( poReturn ) ;

	return( poReturn ) ;
}


TableInfo*	ClElementFactory :: CreateTableInfo( m4pcchar_t ai_pccTable, m4pcchar_t ai_pccAlias )
{

	TableInfo	*poReturn = NULL ;


	poReturn = new TableInfo( ai_pccTable, ai_pccAlias ) ;
	m_vpoElements.push_back( poReturn ) ;

	return( poReturn ) ;
}


TextInfo*	ClElementFactory :: CreateTextInfo( m4pcchar_t ai_pccText )
{

	TextInfo	*poReturn = NULL ;


	poReturn = new TextInfo( ai_pccText ) ;
	m_vpoElements.push_back( poReturn ) ;

	return( poReturn ) ;
}


