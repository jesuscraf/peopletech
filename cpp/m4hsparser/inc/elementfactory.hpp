
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              elementfactory.dll
// File:                m4mdrt.hpp   
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


#include "m4types.hpp"
#include "m4stl.hpp"

#ifndef __ELEMENTFACTORY_HPP__
#define __ELEMENTFACTORY_HPP__


class	ElementInfo ;
class	ColumnInfo ;
class	FunctionInfo ;
class	ItemInfo ;
class	JoinInfo ;
class	SelectInfo ;
class	TableInfo ;
class	TextInfo ;



//=================================================================================
//
// ClElementFactory
//
//=================================================================================

class	ClElementFactory
{

public:
	
	ClElementFactory( void ) ;
	virtual ~ClElementFactory( void ) ;


	ColumnInfo*		CreateColumnInfo( m4pcchar_t ai_pccAlias, m4pcchar_t ai_pccColumn ) ;
	FunctionInfo*	CreateFunctionInfo( m4pcchar_t ai_pccName ) ;
	ItemInfo*		CreateItemInfo( m4pcchar_t pccItemName ) ;
	JoinInfo*		CreateJoinInfo( m4int_t ai_iTreeLevel ) ;
	SelectInfo*		CreateSelectInfo( void ) ;
	TableInfo*		CreateTableInfo( m4pcchar_t ai_pccTable, m4pcchar_t ai_pccAlias ) ;
	TextInfo*		CreateTextInfo( m4pcchar_t ai_pccText ) ;

protected:

	// Vector de punteros a los elementos
	vector <ElementInfo*>	m_vpoElements ;

} ;


#endif



