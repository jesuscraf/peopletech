
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                clttotal.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                18-04-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    En este módulo se define el objeto para representar los totales
//
//
//==============================================================================


#include "m4facres.hpp"

#include "clttotal.hpp"
#include "cldates.hpp"
#include "cltnitem.hpp"



//=================================================================================
//
// ClTempTotal
//
// Clase que implementa la definición de los totales
//
//=================================================================================



// Funciones de inicialización ================================================
	
ClTempTotal :: ClTempTotal( void )
{
	m_acTiId      [ 0 ] = '\0' ;
	m_acTiId      [ 1 ] = '\0' ;
	m_acItemId    [ 0 ] = '\0' ;
	m_acItemId    [ 1 ] = '\0' ;
	m_acItemUsedId[ 0 ] = '\0' ;
	m_acItemUsedId[ 1 ] = '\0' ;

	m_dStartDate = M4CL_MINUS_INFINITE_IN_JULIAN ;
	m_dEndDate = M4CL_PLUS_INFINITE_IN_JULIAN ;

	m_iHasBackPay = 0 ;
	m_iSumType = 1 ;

	m_poNitem = NULL ;
}


// Funciones varias ===========================================================

m4bool_t	ClTempTotal :: IsEqual( ClSlicedElement *ai_poElement )
{

	m4pcchar_t	pccString1 = GetItemUsedId() - 1 ;
	m4pcchar_t	pccString2 = ( ( ClTempTotal* )ai_poElement )->GetItemUsedId() - 1 ;

    
	if( *pccString1 != *pccString2 )
	{
		return( M4_FALSE ) ;
	}

    m4uchar_t uiSize1 = *pccString1 ;
	if( memcmp( pccString1 + 1, pccString2 + 1, uiSize1 ) != 0 )
	{
		return( M4_FALSE ) ;
	}

	return( M4_TRUE ) ;
}


m4return_t	ClTempTotal :: CheckConsistency( ClSlicedElement *ai_poElement, m4pvoid_t ai_pvInfo )
{
	return( M4_SUCCESS ) ;
}


m4return_t	ClTempTotal :: CheckOverLap( ClSlicedElement *ai_poElement, m4date_t ai_dStartDate, m4date_t ai_dEndDate, m4pvoid_t ai_pvInfo )
{

    m4char_t		acStartDate			[ M4_SECOND_END + 1 ] ;
    m4char_t		acEndDate			[ M4_SECOND_END + 1 ] ;
    m4char_t		acPreviousStartDate	[ M4_SECOND_END + 1 ] ;
    m4char_t		acPreviousEndDate	[ M4_SECOND_END + 1 ] ;
	ClTempTotal		*poTotal = ( ClTempTotal* ) ai_poElement ;
	ClTempNitem		*poNitem = ( ClTempNitem* ) ai_pvInfo ;

	m4pcchar_t	pccString1 = m_acTiId ;
	m4pcchar_t	pccString2 = poTotal->m_acTiId ;

	if( *pccString1 == *pccString2 && memcmp( pccString1 + 1, pccString2 + 1, *pccString1 ) == 0 )
	{
		// Si se sobreescribe en la misma TI es error. Sino es sobreescritura
		ClDateToString( ai_dStartDate, acStartDate, M4_SECOND_END + 1 ) ;
		ClDateToString( ai_dEndDate,   acEndDate,   M4_SECOND_END + 1 ) ;
		ClDateToString( poTotal->GetStartDate(), acPreviousStartDate, M4_SECOND_END + 1 ) ;
		ClDateToString( poTotal->GetEndDate(),   acPreviousEndDate,   M4_SECOND_END + 1 ) ;
		DUMP_CHLOG_ERRORF( M4_MDFAC_REPEATED_TOTAL, GetItemUsedId() << GetItemId() << GetTiId() << acStartDate << acEndDate << acPreviousStartDate << acPreviousEndDate << LOG_CAT_AND( poNitem->iNode(), poNitem->nNode() ) << LOG_CAT_AND( poNitem->iM4Obj(), poNitem->nM4Obj() ) ) ;
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}




