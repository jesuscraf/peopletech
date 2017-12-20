
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4mdfac.dll
// File:                clttag.cpp   
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
//    En este módulo se define el objeto para representar los tags
//
//
//==============================================================================


#include "m4facres.hpp"

#include "clttag.hpp"
#include "cldates.hpp"
#include "cltnitem.hpp"



//=================================================================================
//
// ClTempTag
//
// Clase que implementa la definición de los tag
//
//=================================================================================



// Funciones de inicialización ================================================
	
ClTempTag :: ClTempTag( void )
{
	m_acTagId    [ 0 ] = '\0' ;
	m_acTagId    [ 1 ] = '\0' ;
	m_acTiId     [ 0 ] = '\0' ;
	m_acTiId     [ 1 ] = '\0' ;
	m_acItemId   [ 0 ] = '\0' ;
	m_acItemId   [ 1 ] = '\0' ;
	m_acRuleId   [ 0 ] = '\0' ;
	m_acRuleId   [ 1 ] = '\0' ;
	m_acDimItemId[ 0 ] = '\0' ;
	m_acDimItemId[ 1 ] = '\0' ;
	m_acZValue   [ 0 ] = '\0' ;
	m_acZValue   [ 1 ] = '\0' ;

	m_dStartDate = M4CL_MINUS_INFINITE_IN_JULIAN ;
	m_dEndDate = M4CL_PLUS_INFINITE_IN_JULIAN ;

	m_cOperator = 0 ;

	m_poNitem = NULL ;
}


// Funciones varias ===========================================================

m4bool_t	ClTempTag :: IsEqual( ClSlicedElement *ai_poElement )
{

	ClTempTag	*poTag = ( ClTempTag* ) ai_poElement ;


	m4pcchar_t	pccString1 = GetTagId() - 1 ;
	m4pcchar_t	pccString2 = poTag->GetTagId() - 1 ;

    
	if( *pccString1 != *pccString2 )
	{
		return( M4_FALSE ) ;
	}

	m4uchar_t uiSize1 = *pccString1 ;
	if( memcmp( pccString1 + 1, pccString2 + 1, uiSize1 ) != 0 )
	{
		return( M4_FALSE ) ;
	}

	/* Bug 0164624
	El identificado de la regla no está en la pk del tag
	*/
	pccString1 = GetRuleId() - 1 ;
	pccString2 = poTag->GetRuleId() - 1 ;
    
	if( *pccString1 != *pccString2 )
	{
		return( M4_FALSE ) ;
	}

	uiSize1 = *pccString1 ;
	if( memcmp( pccString1 + 1, pccString2 + 1, uiSize1 ) != 0 )
	{
		return( M4_FALSE ) ;
	}

	return( M4_TRUE ) ;
}


m4return_t	ClTempTag :: CheckConsistency( ClSlicedElement *ai_poElement, m4pvoid_t ai_pvInfo )
{
	return( M4_SUCCESS ) ;
}


m4return_t	ClTempTag :: CheckOverLap( ClSlicedElement *ai_poElement, m4date_t ai_dStartDate, m4date_t ai_dEndDate, m4pvoid_t ai_pvInfo )
{

    m4char_t		acStartDate[ M4_SECOND_END + 1 ] ;
    m4char_t		acEndDate  [ M4_SECOND_END + 1 ] ;
	ClTempTag		*poTag = ( ClTempTag* ) ai_poElement ;
	ClTempNitem		*poNitem = ( ClTempNitem* ) ai_pvInfo ;

	m4pcchar_t	pccString1 = m_acTiId ;
	m4pcchar_t	pccString2 = poTag->m_acTiId ;

    
	if( *pccString1 == *pccString2 && memcmp( pccString1 + 1, pccString2 + 1, *pccString1 ) == 0 )
	{
		// Si se sobreescribe en la misma TI es error. Sino es sobreescritura
		ClDateToString( ai_dStartDate, acStartDate, M4_SECOND_END + 1 ) ;
		ClDateToString( ai_dEndDate,   acEndDate,   M4_SECOND_END + 1 ) ;
		DUMP_CHLOG_ERRORF( M4_MDFAC_REPEATED_TAG, GetDimItemId() << GetRuleId() << GetItemId() << GetTiId() << acStartDate << acEndDate << LOG_CAT_AND( poNitem->iNode(), poNitem->nNode() ) << LOG_CAT_AND( poNitem->iM4Obj(), poNitem->nM4Obj() ) ) ;
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}




